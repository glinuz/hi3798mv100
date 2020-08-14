/*
Copyright (C) 2010 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          main.c
*
* \date          %modify_time%
*
* \brief         Sample application code of the NXP OM3910 driver.
*
* REFERENCE DOCUMENTS :
*
* Detailed description may be added here.
*
* \section info Change Information
*
*/

//*--------------------------------------------------------------------------------------
//* Include Standard files
//*--------------------------------------------------------------------------------------
#include <linux/delay.h>
#include <linux/semaphore.h>

#include <linux/types.h>
#include <asm/div64.h> 
#include <linux/math64.h>
#include <linux/kernel.h>


#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"
#include "hi_type.h"
#include "drv_i2c_ext.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"
#include "hi_drv_proc.h"
#include "hi_drv_sys.h"

#include "hi_type.h"
#include "hi_unf_i2c.h"
#include "drv_tuner_ext.h"
#include "drv_demod.h"

#include "drv_i2c_ext.h"
#include "drv_gpioi2c_ext.h"
#include "drv_gpio_ext.h"
#include "hi_drv_gpioi2c.h"
#include "hi_debug.h"

//*--------------------------------------------------------------------------------------
//* Include Driver files
//*--------------------------------------------------------------------------------------
#include "tmbslTDA18280.h"
#include "tmbslTDA18204.h"
#include "tmutilOM3990.h"
static HI_U32 s_u32Tda18280TunerPort = 0;
static HI_U32 s_u32tda18280ConnectTimeout = 0;
#define I2C_BUFFER_SIZE (500)
static int static_switch = 0xff;
static UInt8 addrTDA18204 = 0xC6;
static UInt8 addrTDA18280 = 0xC4;
#define MAX_TUNER (4)



typedef unsigned int         UINT32;
typedef unsigned long long   UINT64;

#define u64mod(x, y)			 (	\
										{  \
										  UINT64 out = 0; \
										  UINT64 x_1 = (UINT64) x; \
										  UINT32 y_1 = (UINT32) y; \
										  if (y) \
										  { \
											out = do_div(x_1,y_1); \
										  } \
										  x_1; \
									   } \
										)

//static Bool tda18280_read (UInt8 uAddress, UInt8 uSubAddress, UInt32 uNbData, UInt8 * pDataBuff);
//static Bool tda18280_write (UInt8 uAddress, UInt8 uSubAddress, UInt32 uNbData, UInt8 * pDataBuff);
static Bool tda18280_wait (UInt32 ms);
//*--------------------------------------------------------------------------------------
//* Prototype of function to be provided by customer
//*--------------------------------------------------------------------------------------
tmErrorCode_t   UserWrittenI2CSetSwitch (UInt8 SwitchPos);
tmErrorCode_t 	UserWrittenI2CReadTDA18280(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
tmErrorCode_t 	UserWrittenI2CWriteTDA18280 (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
tmErrorCode_t 	UserWrittenI2CReadTDA18204(tmUnitSelect_t tUnit,UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData);
tmErrorCode_t 	UserWrittenI2CWriteTDA18204 (tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData);
tmErrorCode_t 	UserWrittenWait_Tda18280(tmUnitSelect_t tUnit, UInt32 tms);
tmErrorCode_t 	UserWrittenPrint_Tda18280(UInt32 level, const char* format, ...);
tmErrorCode_t  	UserWrittenMutexInit_Tda18280(ptmbslFrontEndMutexHandle *ppMutexHandle);
tmErrorCode_t  	UserWrittenMutexDeInit_Tda18280( ptmbslFrontEndMutexHandle pMutex);
tmErrorCode_t  	UserWrittenMutexAcquire_Tda18280(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut);
tmErrorCode_t  	UserWrittenMutexRelease_Tda18280(ptmbslFrontEndMutexHandle pMutex);
tmErrorCode_t   UserWrittenSystemCalibrationNVRAMWrite (UInt32 WriteLen, UInt32* pData);
tmErrorCode_t   UserWrittenSystemCalibrationNVRAMRead(UInt32 ReadLen, UInt32* pData);
tmErrorCode_t   UserWrittenSystemCalibrationWait(UInt32 tms);

#define USERLEVEL 	0
#define USERPRINTF	0


/**************************************************************************/
/*      LOCAL VARIABLES:                                                  */
/**************************************************************************/
typedef enum _tmswitchOM3990_t 
{
    switchTDA18204,       
    switchTDA18280,     
} tmswitchOM3990_t, *ptmswitchOM3990_t;


/* this NVRAM emulation is just an example with 1 header + 20 samples */
static    UInt32 NVRAM[42] =  \
{ \
0, /* Header : version */ \
0, /* Header : NbPoints */ \
0, /* sample 1 : uRF */ \
0, /* sample 1 : uOffsetPowerLevel */ \
0, /* sample 2 : uRF */ \
0, /* sample 2 : uOffsetPowerLevel */ \
0, /* sample 3 : uRF */ \
0, /* sample 3 : uOffsetPowerLevel */ \
0, /* sample 4 : uRF */ \
0, /* sample 4 : uOffsetPowerLevel */ \
0, /* sample 5 : uRF */ \
0, /* sample 5 : uOffsetPowerLevel */ \
0, /* sample 6 : uRF */ \
0, /* sample 6 : uOffsetPowerLevel */ \
0, /* sample 7 : uRF */ \
0, /* sample 7 : uOffsetPowerLevel */ \
0, /* sample 8 : uRF */ \
0, /* sample 8 : uOffsetPowerLevel */ \
0, /* sample 9 : uRF */ \
0, /* sample 9 : uOffsetPowerLevel */ \
0, /* sample 10 : uRF */ \
0, /* sample 10 : uOffsetPowerLevel */ \
0, /* sample 11 : uRF */ \
0, /* sample 11 : uOffsetPowerLevel */ \
0, /* sample 12 : uRF */ \
0, /* sample 12 : uOffsetPowerLevel */ \
0, /* sample 13 : uRF */ \
0, /* sample 13 : uOffsetPowerLevel */ \
0, /* sample 14 : uRF */ \
0, /* sample 14 : uOffsetPowerLevel */ \
0, /* sample 15 : uRF */ \
0, /* sample 15 : uOffsetPowerLevel */ \
0, /* sample 16 : uRF */ \
0, /* sample 16 : uOffsetPowerLevel */ \
0, /* sample 17 : uRF */ \
0, /* sample 17 : uOffsetPowerLevel */ \
0, /* sample 18 : uRF */ \
0, /* sample 18 : uOffsetPowerLevel */ \
0, /* sample 19 : uRF */ \
0, /* sample 19 : uOffsetPowerLevel */ \
0, /* sample 20 : uRF */ \
0, /* sample 20 : uOffsetPowerLevel */ \
};

//*--------------------------------------------------------------------------------------
//* Function Name       : Main
//* Object              : Software entry point
//* Input Parameters    : none.
//* Output Parameters   : none.
//*--------------------------------------------------------------------------------------
tmErrorCode_t     I2CWrite(UInt8 Addr, UInt32 AddrSize, UInt8* pAddr, UInt32 WriteLen, UInt8* pData)
{
    tmErrorCode_t err = TM_OK;
    //int           errI2C;
   // UInt8         uSubAddress = 0;
    UInt16        uSubAddress16 = 0;
    UInt32        uCounter = 0;
    UInt8         WriteBuffer[I2C_BUFFER_SIZE+1] = {0};
    //UInt32        count = 0;
    //UInt32        remain = 0;
	HI_S32  s32Ret = HI_SUCCESS;

	HI_U32 		  u32I2cNo = 0;

	u32I2cNo = g_stTunerOps[s_u32Tda18280TunerPort].enI2cChannel;
    //for(count=0; count<WriteLen; count += remain)
    {
        //remain = min(WriteLen-count, I2C_BUFFER_SIZE);

        // copy I2CMap data in WriteBuffer
        switch(AddrSize)
        {
        default:
        case 0:
        case 1:
            {
                uSubAddress16 = (*pAddr)&0xFF;
                //WriteBuffer[0] = uSubAddress;
                for (uCounter = 0; uCounter < WriteLen; uCounter++)
                {
                    WriteBuffer[uCounter] = pData[uCounter];
                }
            }
            break;

        case 2:
			
            {
                //TARGET PLATFORM LITTLE INDIAN
                uSubAddress16 = pAddr[0]<<8 | pAddr[1];

               // WriteBuffer[0] = (uSubAddress16 >> 8) & 0xFF;
               //WriteBuffer[1] = (uSubAddress16) & 0xFF;

                for (uCounter = 0; uCounter < WriteLen; uCounter+=2)
                {
                    WriteBuffer[uCounter] = pData[uCounter];
                    WriteBuffer[uCounter+1] = pData[uCounter+1];
                }
            }
            break;
        }

       // errI2C = i2c_tuner.write((int)Addr, (const char*)&WriteBuffer[0], (int)(WriteLen+AddrSize));

	   if(HI_STD_I2C_NUM >u32I2cNo)
	   {
		   //s32Ret = HI_DRV_I2C_Write(u32I2cNo, (HI_U8)u32DevAddr, u32RegAddr, u32RegNum, pData, u32DataLen);
		   s32Ret = s_tuner_pI2cFunc->pfnI2cWrite(u32I2cNo,Addr, uSubAddress16, AddrSize, WriteBuffer, WriteLen);
   
		   if(HI_SUCCESS != s32Ret)
		   {
			   return s32Ret;
		   }
	   }
	   else
	   {
		   //s32Ret = HI_DRV_GPIOI2C_WriteExt(u32I2cNo,(HI_U8)u32DevAddr, (HI_U8)u32RegAddr, u32RegNum, pData, u32DataLen);
		   s32Ret = s_tuner_pGpioI2cFunc->pfnGpioI2cWriteExt(u32I2cNo,Addr, uSubAddress16, AddrSize, WriteBuffer, WriteLen);
		   if(HI_SUCCESS != s32Ret)
		   {
			   return s32Ret;
		   }
	   }


        if (s32Ret != TM_OK) 
        {
            err = TM_ERR_WRITE;
        }
    }

    return err;
}

tmErrorCode_t     I2CRead(UInt8 Addr, UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData)
{
    tmErrorCode_t err = TM_OK;
    //int           errI2C;
    UInt8         uSubAddress = 0;
    UInt16        uSubAddress16 = 0;
    UInt8         uBytesBuffer[I2C_BUFFER_SIZE];
    //UInt8         WriteBuffer[2];
    UInt32        i = 0;
    HI_U32 		  u32I2cNo = 0;
    HI_S32        s32Ret = HI_SUCCESS;
	u32I2cNo = g_stTunerOps[s_u32Tda18280TunerPort].enI2cChannel;
    switch(AddrSize)
    {
    default:
    case 0:
        uSubAddress = 0;
        break;
    case 1:
		uSubAddress16 = (*pAddr)&0xFF;
        uSubAddress = (*pAddr)&0xFF;
       // WriteBuffer[0] = uSubAddress;
        break;
    case 2:                                                                            
        //TARGET PLATFORM LITTLE INDIAN
        uSubAddress16 = pAddr[0]<<8 | pAddr[1];
       // WriteBuffer[0] = (uSubAddress16 >> 8) & 0xFF;
       // WriteBuffer[1] = (uSubAddress16) & 0xFF;       
        break;
    }

    if (ReadLen > I2C_BUFFER_SIZE) 
    {
        err = TM_ERR_READ;
    }
#if 0
    if (err == TM_OK) 
    {
        /* for a read operation, first write sub address */
        errI2C = i2c_tuner.write((int)Addr, (char*)&WriteBuffer[0], AddrSize, true);
    }
    if (err == TM_OK) 
    {
        errI2C = i2c_tuner.read((int)Addr, (char*)&uBytesBuffer[0], (int)ReadLen);
    }
#endif

	if(HI_STD_I2C_NUM > u32I2cNo)
	{
		//s32Ret = HI_DRV_I2C_Read(u32I2cNo, (HI_U8)u32DevAddr, u32RegAddr, u32RegNum, pu8Data, u32DataLen);
		s32Ret = s_tuner_pI2cFunc->pfnI2cRead(u32I2cNo, (HI_U8)Addr, uSubAddress16, AddrSize, &uBytesBuffer[0], ReadLen);
		if(HI_SUCCESS != s32Ret)
		{
			return s32Ret;
		}
	}
	else
	{
		//s32Ret = HI_DRV_GPIOI2C_ReadExt(u32I2cNo, (HI_U8)u32DevAddr, (HI_U8)u32RegAddr, u32RegNum, pu8Data, u32DataLen);
		s32Ret =  s_tuner_pGpioI2cFunc->pfnGpioI2cReadExt(u32I2cNo, (HI_U8)Addr, uSubAddress16, AddrSize, &uBytesBuffer[0], ReadLen);
		if(HI_SUCCESS != s32Ret)
		{
			return s32Ret;
		}
	}


    if (s32Ret == TM_OK) 
    {

		switch(AddrSize)
        {
        case 1 :
			for (i=0 ; i<ReadLen ; i++) 
       	 	{
           	 	*(pData + i) = (UInt8)uBytesBuffer[i];
        	}
			break;
		case 2 :
		
			for (i=0 ; i<ReadLen ; i=i+2) 
       	 	{
           	 	*(pData + i) = (UInt8)uBytesBuffer[i];
				*(pData + i+1) = (UInt8)uBytesBuffer[i+1];			
        	}			
			break;
		default :
			break;
		}
        				
    } 
    else
    {
        err = TM_ERR_READ;
    }

    return err;
}


static Bool tda18280_wait (UInt32 ms)
{
	msleep(ms);

	return HI_SUCCESS;
}
HI_S32 tda18280_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{

    /* Variable declarations */
    tmErrorCode_t err = TM_OK;
    tmbslFrontEndDependency_t sSrvTunerFunc;
    //OM3990_SystemCalibrationIoFunc_t sSystemCalibrationFunc;
    //OM3990_InitSystemCalibrationIoFunc_t sInitSystemCalibrationFunc;    
    //tmTDA18280_PathCfg_t tPathCfg ; 
	
	#if 0
    float RSSI;

    /* this table is just an example to check calibration algorithm */
    OM3990_RefPoint_t RefPoint[20] = 
    {
        {44000000,  50.0}, /* 0 */
        {100000000, 82.0}, /* 1 */
        {150000000, 71.0}, /* 2 */
        {200000000, 50.0}, /* 3 */
        {250000000, 50.0}, /* 4 */
        {300000000, 50.0}, /* 5 */
        {350000000, 50.0}, /* 6 */
        {400000000, 50.0}, /* 7 */
        {450000000, 50.0}, /* 8 */
        {500000000, 50.0}, /* 9 */
        {550000000, 57.0}, /* 10 */
        {600000000, 66.0}, /* 11 */
        {650000000, 50.0}, /* 12 */
        {700000000, 50.0}, /* 13 */
        {750000000, 50.0}, /* 14 */
        {800000000, 50.0}, /* 15 */
        {850000000, 50.0}, /* 16 */
        {900000000, 50.0}, /* 17 */
        {950000000, 50.0}, /* 18 */
        {999000000, 50.0}  /* 19 */
    };
    OM3990_RefVector_t RefVector;
#endif

    s_u32Tda18280TunerPort = u32TunerPort;

	addrTDA18204 = g_stTunerOps[s_u32Tda18280TunerPort].u32TunerAddress;
    addrTDA18280 = g_stTunerOps[s_u32Tda18280TunerPort].u32DemodAddress;

  //  RefVector.uNbPoints = 20;
   // RefVector.tRefPoints = RefPoint;

  
	/* Example on how to terminate the application*/
	if(err == TM_OK)
	{
	  /* DeInitialize TDA18280 Driver */
	  err = tmbslTDA18280_DeviceCloseInstance(0);
	}

	if(err == TM_OK)
	{
	  /* DeInitialize TDA18204 Driver */
	  err = tmbslTDA18204_Close(0);
	}

    /* Driver layer struct set-up to link with user written functions */
    sSrvTunerFunc.sTime.Get             = Null;
    sSrvTunerFunc.sTime.Wait            = UserWrittenWait_Tda18280;
    sSrvTunerFunc.sDebug.Print          = UserWrittenPrint_Tda18280;
    sSrvTunerFunc.sMutex.Init           = UserWrittenMutexInit_Tda18280;
    sSrvTunerFunc.sMutex.DeInit         = UserWrittenMutexDeInit_Tda18280;
    sSrvTunerFunc.sMutex.Acquire        = UserWrittenMutexAcquire_Tda18280;
    sSrvTunerFunc.sMutex.Release        = UserWrittenMutexRelease_Tda18280;
    sSrvTunerFunc.dwAdditionalDataSize  = 0;
    sSrvTunerFunc.pAdditionalData       = Null;

    /* TDA18204 Driver initialization  */
    sSrvTunerFunc.sIo.Write             = UserWrittenI2CWriteTDA18204;
    sSrvTunerFunc.sIo.Read              = UserWrittenI2CReadTDA18204;
   
    err = tmbslTDA18204_Open(0, &sSrvTunerFunc);
	if (err != TM_OK)
	{
       HI_ERR_TUNER("[tda18280 err %d]:%d  tmbslTDA18204_Open  \n",__LINE__,err);
	}

	
    /* TDA18204 Hardware initialization */
    err = tmbslTDA18204_HwInit(0);
    
    if (err != TM_OK)
    {
       HI_ERR_TUNER("[tda18280 err %d]:%d  tmbslTDA18204_HwInit  \n",__LINE__,err);
    }
    
        // set TDA18204 in WB mode
    err = tmbslTDA18204_SetLLPowerState(0, TDA18204_PowerSciNormalMode);
    
    if (err != TM_OK)
    {
       HI_ERR_TUNER("[tda18280 err %d]:%d  tmbslTDA18204_SetLLPowerState  \n",__LINE__,err);
    }
	   
    /* TDA18280 Driver initialization  */
    sSrvTunerFunc.sIo.Write             = UserWrittenI2CWriteTDA18280;
    sSrvTunerFunc.sIo.Read              = UserWrittenI2CReadTDA18280;
    err = tmbslTDA18280_DeviceOpenInstance(0, &sSrvTunerFunc);
  
    if (err != TM_OK)
    {
       HI_ERR_TUNER("[tda18280 err %d]:%d  tmbslTDA18280_DeviceOpenInstance  \n",__LINE__,err);
    }
    
    /* TDA18280 Hardware initialization */
    err = tmbslTDA18280_DeviceHwInit(0);
   
	if (err != TM_OK)
	{
	   HI_ERR_TUNER("[tda18280 err %d]:%d  tmbslTDA18280_DeviceHwInit	\n",__LINE__,err);
	}
	

  
    /* TDA18280 Start-UP the device*/
    err = tmbslTDA18280_DeviceSetPowerState(0, tmTDA18280_PowerWBOnly);
  
	if (err != TM_OK)
	{
	   HI_ERR_TUNER("[tda18280 err %d]:%d  tmbslTDA18280_DeviceSetPowerState	\n",__LINE__,err);
	}
	
    return err;

}


HI_S32 tda18280_connect (HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    tmErrorCode_t err = TM_OK;
	tmTDA18280_PathCfg_t tPathCfg ; 
	//HI_U32 k=0;
	//tmbslFrontEndState_t enLockStatus;

	TDA18280_ExtendSymbolRateMode_t eMode;
    //float RSSI;
      /* Example how to set a path (O in this case)*/
   
	if(u32TunerPort >= MAX_TUNER)
	{
	   HI_ERR_TUNER( "u32TunerPort(%d) goes over the top(%d) \n",u32TunerPort,MAX_TUNER);
	   return HI_FAILURE;
	} 
	g_stTunerOps[u32TunerPort].u32CurrQamMode = pstChannel->enQamType;


											  
    switch(g_stTunerOps[u32TunerPort].enSigType)
	{ 
	    case HI_UNF_TUNER_SIG_TYPE_CAB:
		     tPathCfg.eFECMode = tmFrontEndFECModeAnnexA; /* DVBC */
		     tPathCfg.tBW = tmTDA18280_8MHz;
		     break;
	    case HI_UNF_TUNER_SIG_TYPE_J83B:
		     tPathCfg.eFECMode = tmFrontEndFECModeAnnexB;
		     tPathCfg.tBW = tmTDA18280_6MHz;
		     break;
	    default:
		     break;
    }
      	
    tPathCfg.uSR = pstChannel->unSRBW.u32SymbolRate; /* bps */
	tPathCfg.uRF = pstChannel->u32Frequency * 1000;  /* Hz */
   
	eMode = TDA18280_ExtendSymbolRateModeEnable1500ppm;
		
	err = tmbslTDA18280_PathSetDemodExtendSymbolRateRange(0,u32TunerPort,eMode);
	if (err != TM_OK)
	{
	   HI_ERR_TUNER("[tda18280 err %d]:%d  tmbslTDA18280_PathSetDemodExtendSymbolRateRange	\n",__LINE__,err);
	}
		
    switch(pstChannel->enQamType)
    {
        case QAM_TYPE_64: 
			tPathCfg.eMod = tmFrontEndModulationQam64;/*64QAM*/              
            break;
        case QAM_TYPE_128:                         /*128QAM*/
            tPathCfg.eMod = tmFrontEndModulationQam128;
            break;
        case QAM_TYPE_256:                         /*256QAM*/
            tPathCfg.eMod = tmFrontEndModulationQam256;
            break;
		case QAM_TYPE_32:
			tPathCfg.eMod = tmFrontEndModulationQam32;
            break;
		case QAM_TYPE_16:
			tPathCfg.eMod = tmFrontEndModulationQam16;
            break;
        default:
            tPathCfg.eMod = tmFrontEndModulationQam64;
            break;
    }


    if(err == TM_OK)
    {
        tPathCfg.tPathStatus = tmTDA18280_ON;     

                   
        tPathCfg.tGainAutoMode = tmTDA18280_ON;   
       
        tPathCfg.eSI = tmFrontEndSpecInvAuto;

        err = tmbslTDA18280_PathSet(0, u32TunerPort, &tPathCfg);
		if (err != TM_OK)
		{
		   HI_ERR_TUNER("[tda18280 err %d]:%d  tmbslTDA18280_PathSet	\n",__LINE__,err);
		}
    }
   return err;
   
}
HI_S32 tda18280_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{


    tmErrorCode_t err;
	tmTDA18280_Path_t     tPathNumber;
	tmbslFrontEndState_t enLockStatus;
	
	switch(u32TunerPort)
	{
	    case 0:
		tPathNumber = tmTDA18280_Path_0;
		
		break;
	    case 1:
		tPathNumber = tmTDA18280_Path_1;

		break;
	    case 2:
		tPathNumber = tmTDA18280_Path_2;
		
		break;
	    case 3:
		tPathNumber = tmTDA18280_Path_3;
	
		break;
	    default:
		HI_ERR_TUNER( "u32TunerPort(%d) goes over the top(%d) \n",u32TunerPort,MAX_TUNER);
		return HI_FAILURE;
		break;
				
	}
     err = tmbslTDA18280_PathGetDemodLockStatus(0,tPathNumber,&enLockStatus );
	 if(err == TM_OK)
	 {

        if(enLockStatus == tmbslFrontEndStateLocked)
        {
            *penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
        }
		else
	    {
	        *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
	    }
		
    }
	else
	{ 
	    HI_ERR_TUNER( "TDA18280:u32TunerPort(%d) GetDemodLockStatus failed\n",u32TunerPort);
		return HI_FAILURE;
	}
    
    return HI_SUCCESS;
}
HI_S32 tda18280_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
	 return 0; 
}
HI_VOID tda18280_connect_timeout(HI_U32 u32ConnectTimeout)
{
    s_u32tda18280ConnectTimeout = u32ConnectTimeout;
    return;
}

HI_S32 tda18280_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{

    tmErrorCode_t err;
    UInt32          uRSSI = 0;
    err = tmutilOM3990GetRSSI( 0,0,u32TunerPort,&uRSSI);
	if(err != TM_OK)
    { 
 	    HI_ERR_TUNER( "TDA18280:u32TunerPort(%d) tmutilOM3990GetRSSI failed\n",u32TunerPort);
 		return HI_FAILURE;
 	}
	pu32SignalStrength[1] = ((uRSSI + 500) / 100 -5 );
     /**< O: Pointer to the RSSI in dBuV *100. */
	 return HI_SUCCESS;

}

HI_S32 tda18280_get_ber(HI_U32 u32TunerPort, HI_U32 *pu32ber)
{
    tmErrorCode_t err;
    tmTDA18280_Path_t     tPathNumber;
	tmFrontEndFracNb32_t tsBER;
	UInt32               u32Uncors = 0;
    UInt32  i = 0;
	UInt32  j = 0;
	HI_U32 au32Tmp = 0;
    HI_U32 au32ber = 0;
	//HI_U32 loop = 0;
	switch(u32TunerPort)
	{
	    case 0:
		tPathNumber = tmTDA18280_Path_0;
		
		break;
	    case 1:
		tPathNumber = tmTDA18280_Path_1;

		break;
	    case 2:
		tPathNumber = tmTDA18280_Path_2;
		
		break;
	    case 3:
		tPathNumber = tmTDA18280_Path_3;
	
		break;
	    default:
		HI_ERR_TUNER( "u32TunerPort(%d) goes over the top(%d) \n",u32TunerPort,MAX_TUNER);
		return HI_FAILURE;
		break;
				
	}
   // err = tmbslTDA18280_PathGetDemodBER(0,tPathNumber,&tsBER,&u32Uncors);
    err = tmbslTDA18280_PathGetDemodBER(0,u32TunerPort,&tsBER,&u32Uncors);
 
	if(err != TM_OK)
    { 
 	    HI_ERR_TUNER( "TDA18280:u32TunerPort(%d) tmbslTDA18280_PathGetDemodBER failed\n",u32TunerPort);
 		return HI_FAILURE;
 	}
  
    au32Tmp = (HI_U32)u64mod((((HI_U64)(tsBER.lInteger)) * 1000*1000*1000*10),tsBER.uDivider);
	
    au32ber = au32Tmp;
    
	if (au32ber != 0)
	{
		while (au32ber >= 10)
		{
		    au32ber = au32ber /10;
		    i++;
			if(au32ber >= 1000)
			{ 
			   j = i;
			   au32Tmp = au32ber;
			   
			}
		}
	}

    pu32ber[0] = (HI_U32)au32ber;
    pu32ber[1] = (HI_U32)(au32Tmp % 1000 );
    pu32ber[2] = (HI_U32)(10 - i);
	
    return HI_SUCCESS;
}
HI_S32 tda18280_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
	tmFrontEndFracNb32_t Esno;
	tmErrorCode_t err;

	err = tmbslTDA18280_PathGetDemodChannelEsNo(0,u32TunerPort,&Esno);
	if(err != TM_OK)
	{ 
	    HI_ERR_TUNER( "TDA18280:u32TunerPort(%d) tmbslTDA18280_PathGetDemodChannelEsNo failed\n",u32TunerPort);
		return HI_FAILURE;
	}

	 *pu32SNR =(HI_U32)u64mod((((HI_U64)(Esno.lInteger))),Esno.uDivider);
    //*pu32SNR = (Esno.lInteger * 10) / Esno.uDivider);
	 return HI_SUCCESS;
}

HI_S32 tda18280_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb )
{
    UInt32                u32SR = 0;
	Int32				  s32IFOffset = 0;
	tmErrorCode_t err;
    HI_TUNER_CHECKPOINTER(pu32Freq);
    HI_TUNER_CHECKPOINTER(pu32Symb);

	
	err = tmbslTDA18280_PathGetDemodSR(0,u32TunerPort,&u32SR); 
	if(err != TM_OK)
	{ 
	    HI_ERR_TUNER( "TDA18280:u32TunerPort(%d) tmbslTDA18280_PathGetDemodSR failed\n",u32TunerPort);
		return HI_FAILURE;
	}
    *pu32Symb = u32SR;
	
	err = tmbslTDA18280_PathGetDemodIFOffset(0,u32TunerPort,&s32IFOffset); 
	if(err != TM_OK)
	{ 
	    HI_ERR_TUNER( "TDA18280:u32TunerPort(%d) tmbslTDA18280_PathGetDemodIFOffset failed\n",u32TunerPort);
		return HI_FAILURE;
	}
	*pu32Freq = s32IFOffset;
		
	return  HI_SUCCESS;
} 


//*--------------------------------------------------------------------------------------
//* Template of function to be provided by customer
//*--------------------------------------------------------------------------------------
//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CSetSwitch
//* Object              : Manage eventual I2C switches to enable or not I2C traffic towards a device
//* Input Parameters    : UInt8 SwitchPos
//* Output Parameters   : tmErrorCode_t err
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CSetSwitch (UInt8 SwitchPos)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    /* Customer code here */
    /* ...*/
    /* Variable declarations */
  // tmErrorCode_t err = TM_OK;
   UInt8 pAddr[2];
   UInt8 pData[2];
   UInt16 Register_Address=0x000a;
   UInt16 Register_Value = 0; 
   err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenI2CSetSwitch position: %d",SwitchPos);

   if (static_switch!=SwitchPos)
   {
        switch (SwitchPos)
        {
            case switchTDA18204:
                Register_Value = (addrTDA18204-0xc0)+1;
            break;

            case switchTDA18280:
                Register_Value = 0;
               break;

            default :
                break;
        }
   
		pAddr[0] = (Register_Address >> 8) & 0xFF;
        pAddr[1] = (Register_Address) & 0xFF;
        pData[0] = (Register_Value >> 8) & 0xFF;
        pData[1] = (Register_Value) & 0xFF;

		
        err = I2CWrite (addrTDA18280, 2, pAddr, 2, pData);
    
        if (err == TM_OK)
        {
            static_switch=SwitchPos;
        }
   }
    return err;
    /* ...*/
    /* End of Customer code here */

    return err;

}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CReadTDA18280
//* Object              : 
//* Input Parameters    :   tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 ReadLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CReadTDA18280(tmUnitSelect_t tUnit,  UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenI2CReadTDA18280 tUnit: %d AddrSize:%d pAddr:(%08X) ReadLen:%d pData:%(%08X)",tUnit, AddrSize,*pAddr,ReadLen, *pData);

    err |= UserWrittenI2CSetSwitch (switchTDA18280);


    err |= I2CRead (addrTDA18280, AddrSize, pAddr,ReadLen, pData);


    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CWriteTDA18280
//* Object              : 
//* Input Parameters    :   tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 WriteLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CWriteTDA18280 (tmUnitSelect_t tUnit,    UInt32 AddrSize, UInt8* pAddr,UInt32 WriteLen, UInt8* pData)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenI2CWriteTDA18280 tUnit: %d AddrSize:%d pAddr:(%08X) WriteLen:%d pData:%(%08X)",tUnit, AddrSize,*pAddr,WriteLen, *pData);

    err |= UserWrittenI2CSetSwitch (switchTDA18280);

    err |= I2CWrite (addrTDA18280, AddrSize, pAddr,WriteLen, pData);
   
    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CReadTDA18204
//* Object              : 
//* Input Parameters    :   tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 ReadLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CReadTDA18204(tmUnitSelect_t tUnit,  UInt32 AddrSize, UInt8* pAddr,UInt32 ReadLen, UInt8* pData)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenI2CReadTDA18204 tUnit: %d AddrSize:%d pAddr:(%08X) ReadLen:%d pData:%(%08X)",tUnit, AddrSize,*pAddr,ReadLen, *pData);

    err = UserWrittenI2CSetSwitch (switchTDA18204);

    /* Customer code here */
    /* ...*/
    //err = tda18280_read (g_stTunerOps[s_u32Tda18280TunerPort].u32TunerAddress, *pAddr, ReadLen, pData);
    err = I2CRead (addrTDA18204, 1, pAddr,ReadLen, pData);
	/* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenI2CWriteTDA18204
//* Object              : 
//* Input Parameters    :   tmUnitSelect_t tUnit
//*                         UInt32 AddrSize,
//*                         UInt8* pAddr,
//*                         UInt32 WriteLen,
//*                         UInt8* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenI2CWriteTDA18204 (tmUnitSelect_t tUnit,    UInt32 AddrSize, UInt8* pAddr,
                                           UInt32 WriteLen, UInt8* pData)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenI2CWriteTDA18204 tUnit: %d AddrSize:%d pAddr:(%08X) WriteLen:%d pData:%(%08X)",tUnit, AddrSize,*pAddr,WriteLen, *pData);

    err = UserWrittenI2CSetSwitch (switchTDA18204);

    /* Customer code here */
    /* ...*/
	//err = tda18280_write (g_stTunerOps[s_u32Tda18280TunerPort].u32TunerAddress, *pAddr, WriteLen, pData);
     
    err = I2CWrite (addrTDA18204, 1, pAddr,WriteLen, pData);
    
    /* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenWait
//* Object              : 
//* Input Parameters    : 	tmUnitSelect_t tUnit
//* 						UInt32 tms
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenWait_Tda18280(tmUnitSelect_t tUnit, UInt32 tms)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenWait_Tda18280 tUnit:%d tms:%d ",tUnit,tms);

    /* Customer code here */
    /* ...*/
     tda18280_wait(tms);
    /* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenPrint
//* Object              : 
//* Input Parameters    : 	UInt32 level, const char* format, ...
//* 						
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenPrint_Tda18280(UInt32 level, const char* format, ...)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;
    UInt32 mylevel;
    const char* myformat;

#if USERPRINTF
    static char     buf[1040] = "\0";
    va_list         ap;
#endif
    mylevel = level;
    myformat = format;

    if(level<DEBUGLVL_MAX)
    {
    }
    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexInit
//* Object              : 
//* Input Parameters    : 	ptmbslFrontEndMutexHandle *ppMutexHandle
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenMutexInit_Tda18280(ptmbslFrontEndMutexHandle *ppMutexHandle)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenMutexInit_Tda18280 ppMutexHandle:(08X)", *ppMutexHandle);

    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    return err;
}


//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexDeInit
//* Object              : 
//* Input Parameters    : 	 ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenMutexDeInit_Tda18280( ptmbslFrontEndMutexHandle pMutex)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenMutexDeInit_Tda18280 pMutex:(08X)", pMutex);

    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    return err;
}


struct semaphore tda18280_sem;

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexAcquire
//* Object              : 
//* Input Parameters    : 	ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenMutexAcquire_Tda18280(ptmbslFrontEndMutexHandle pMutex, UInt32 timeOut)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenMutexAcquire_Tda18280 pMutex:(08X) timeOut: %d", pMutex, timeOut);

    /* Customer code here */
    /* ...*/
	err = down_interruptible (&tda18280_sem);

    /* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenMutexRelease
//* Object              : 
//* Input Parameters    : 	ptmbslFrontEndMutexHandle pMutex
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t  UserWrittenMutexRelease_Tda18280(ptmbslFrontEndMutexHandle pMutex)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenMutexRelease_Tda18280 pMutex:(08X)", pMutex);

    /* Customer code here */
    /* ...*/
    up (&tda18280_sem);
    /* ...*/
    /* End of Customer code here */

    return err;
}


//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenSystemCalibrationNVRAMWrite
//* Object              : 
//* Input Parameters    :   UInt32 WriteLen,
//*                         UInt32* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenSystemCalibrationNVRAMWrite (UInt32 WriteLen, UInt32* pData)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;
	UInt32 i = 0;

    err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenSystemCalibrationNVRAMWrite WriteLen:%d ",WriteLen);

	/* Example for test */
	for (i=0 ; i < WriteLen ; i++) 
	{
		NVRAM[i] = *(pData + i);
	}

    /* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenSystemCalibrationNVRAMRead
//* Object              : 
//* Input Parameters    :   UInt32 ReadLen,
//*                         UInt32* pData
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenSystemCalibrationNVRAMRead(UInt32 ReadLen, UInt32* pData)
{
	/* Variable declarations */
	tmErrorCode_t err = TM_OK;
	UInt32             i = 0;

	err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenSystemCalibrationNVRAMRead ReadLen:%d ",ReadLen);

	/* Example for test */
	for (i=0 ; i < ReadLen ; i++) 
	{
		*(pData + i) = NVRAM[i];
	}

	/* Customer code here */
	/* ...*/

	/* ...*/
	/* End of Customer code here */

	return err;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : UserWrittenSystemCalibrationWait
//* Object              : 
//* Input Parameters    : 	UInt32 tms
//* Output Parameters   : tmErrorCode_t.
//*--------------------------------------------------------------------------------------
tmErrorCode_t UserWrittenSystemCalibrationWait(UInt32 tms)
{
    /* Variable declarations */
    tmErrorCode_t err = TM_OK;

    err = UserWrittenPrint_Tda18280 (USERLEVEL, "UserWrittenSystemCalibrationWait tms:%d ",tms);

	/* Customer code here */
    /* ...*/

    /* ...*/
    /* End of Customer code here */

    return err;
}

