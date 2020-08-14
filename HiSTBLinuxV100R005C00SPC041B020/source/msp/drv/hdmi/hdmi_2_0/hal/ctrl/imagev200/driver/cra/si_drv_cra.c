/******************************************************************************
*
* Copyright 2013, Silicon Image, Inc.  All rights reserved.
* No part of this work may be reproduced, modified, distributed, transmitted,
* transcribed, or translated into any language or computer format, in any form
* or by any means without written permission of
* Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
*
*****************************************************************************/
/**
* @file si_drv_cra.c
*
* @brief CRA (common register access) driver
*
*****************************************************************************/
//#define SII_DEBUG

/***** #include statements ***************************************************/

#include "si_drv_cra_api.h"
#include "si_lib_obj_api.h"
#include "si_lib_malloc_api.h"
#include "si_lib_time_api.h"
#include "hdmi_platform.h"
#ifdef HI_MINIBOOT_SUPPORT
#include "string.h"
#endif

/***** local data objects ****************************************************/
#define CRA_RETRY_COUNT	3

SII_LIB_OBJ_MODULE_DEF(si_cra);

typedef struct{
    SiiDrvCraConfig_t *pConfig;
}CraObj_t;

/***** public functions ******************************************************/
SiiInst_t SiiDrvCraCreate( SiiDrvCraConfig_t *pCfg)
{
    //uint8_t i;
    //SiiPlatformStatus_t platStat = SII_PLATFORM_STATUS__FAILED;
    CraObj_t *pObj = (CraObj_t *)SII_LIB_OBJ_CREATE("cra", sizeof(CraObj_t));
    SII_PLATFORM_DEBUG_ASSERT(pObj);

    pObj->pConfig = (SiiDrvCraConfig_t *) SiiLibMallocCreate(sizeof(SiiDrvCraConfig_t));
    SII_PLATFORM_DEBUG_ASSERT(pObj->pConfig);

    memcpy(pObj->pConfig, pCfg, sizeof(SiiDrvCraConfig_t));
    
    pObj->pConfig->pu32BaseAddrVirtual = (uint32_t *)HDMI_IO_MAP(HDMI_CTRL_BASE_ADDR, 0x4000); 
    
#if 0
    for(i = 0; i < CRA_RETRY_COUNT; i++)
    {
		if(pObj->pConfig->pInterfaceInfo->interfaceHardware == SII_PLATFORM_HARDWARE__BB)
		{
			int i;
			//if all gpios of BB header are not configured as BIDI, GPIO out functionality is not achieved.
			// also pin 3 and pin 5 (SII_PLATFORM_GPIO__1 and SII_PLATFORM_GPIO__2 correspondingly) are working for GPIO ouput. 
			for (i = SII_PLATFORM_GPIO__0; i <= SII_PLATFORM_GPIO__7; i++)
				platStat = SiiPlatformGPIOConfig(pObj->pConfig->pInterfaceInfo, (SiiPlatformGPIO_t)i, SII_PLATFORM_GPIO_DIRECTION__INOUT);
		}
		else
		{
			platStat = SiiPlatformGPIOConfig(pObj->pConfig->pInterfaceInfo, pObj->pConfig->intrPin, SII_PLATFORM_GPIO_DIRECTION__INPUT);
			platStat |= SiiPlatformGPIOConfig(pObj->pConfig->pInterfaceInfo, pObj->pConfig->rstPin, SII_PLATFORM_GPIO_DIRECTION__OUTPUT);
		}

		platStat |= SiiPlatformGPIOSet(pObj->pConfig->pInterfaceInfo, pObj->pConfig->rstPin, SII_PLATFORM_GPIO_LEVEL__HIGH);

        if (SII_PLATFORM_STATUS__SUCCESS == platStat)
        {
            return SII_LIB_OBJ_INST(pObj);
        }
    }
#endif    
    return SII_LIB_OBJ_INST(pObj);
#if 0
	if(pObj->pConfig->callBack)
		pObj->pConfig->callBack();
    return (SiiInst_t)NULL;
#endif    
}

void SiiDrvCraDelete(SiiInst_t craInst)
{
	CraObj_t *pObj = (CraObj_t *)SII_LIB_OBJ_PNTR(craInst);

    if(pObj->pConfig->pu32BaseAddrVirtual)
    {
        HDMI_IO_UNMAP(pObj->pConfig->pu32BaseAddrVirtual);
    }
    
	SiiLibMallocDelete(pObj->pConfig);
    SII_LIB_OBJ_DELETE(pObj);
}

uint32_t *SiiDrvVirtualAddrGet(SiiInst_t craInst)
{
    CraObj_t *pObj = (CraObj_t *)SII_LIB_OBJ_PNTR(craInst);

    return pObj->pConfig->pu32BaseAddrVirtual;
}


bool_t SiiDrvCraHardwareReset(SiiInst_t craInst)
{
#ifdef SIMG_PLATFORM
	SiiPlatformStatus_t platStatus;
	CraObj_t *pObj = (CraObj_t *)SII_LIB_OBJ_PNTR(craInst);
	platStatus = SiiPlatformGPIOSet(pObj->pConfig->pInterfaceInfo, pObj->pConfig->rstPin, SII_PLATFORM_GPIO_LEVEL__LOW);
	if(platStatus)
		return false;
	SiiLibTimeMilliDelay(100);
	platStatus = SiiPlatformGPIOSet(pObj->pConfig->pInterfaceInfo, pObj->pConfig->rstPin, SII_PLATFORM_GPIO_LEVEL__HIGH);
	if(platStatus)
		return false;
	return true;
#else
	return false;
#endif
}

void SiiDrvCraWrReg8(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t val )
{
	CraObj_t  *pObj= (CraObj_t  *) SII_LIB_OBJ_PNTR(inst);
	SiiPlatformStatus_t platStat;
	uint8_t i;
	for(i=0; i < CRA_RETRY_COUNT; i++)
	{
		platStat = SiiPlatformWrite(pObj->pConfig->pInterfaceInfo, 0x40, addr, &val, 1, pObj->pConfig->pu32BaseAddrVirtual);
		if(platStat == SII_PLATFORM_STATUS__SUCCESS)
			return;
	}

	if(pObj->pConfig->callBack)
		pObj->pConfig->callBack();
}

uint8_t SiiDrvCraRdReg8(SiiInst_t inst, SiiDrvCraAddr_t addr )
{
	CraObj_t  *pObj = (CraObj_t  *) SII_LIB_OBJ_PNTR(inst);
	SiiPlatformStatus_t platStat;
	uint8_t i;
	uint8_t retVal = 0;

	for(i=0; i < CRA_RETRY_COUNT; i++)
	{
		platStat = SiiPlatformRead(pObj->pConfig->pInterfaceInfo, 0x40, addr, &retVal, 1, pObj->pConfig->pu32BaseAddrVirtual);
		if(platStat == SII_PLATFORM_STATUS__SUCCESS)
			return retVal;
	}

	if(pObj->pConfig->callBack)
		pObj->pConfig->callBack();

	return -1;
}

void SiiDrvCraSetBit8(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t mask )
{
	uint8_t val;
	val = SiiDrvCraRdReg8(inst, addr) ;
	val = (val & ((uint8_t)~mask)) | mask;
	SiiDrvCraWrReg8(inst, addr, val);
}

void SiiDrvCraClrBit8(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t mask )
{
	uint8_t val;
	val = SiiDrvCraRdReg8(inst, addr) ;
	val = (val & ((uint8_t)~mask));
	SiiDrvCraWrReg8(inst, addr, val);
}

void SiiDrvCraPutBit8(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t mask, uint8_t val )
{ 
	uint8_t temp;
	temp = SiiDrvCraRdReg8(inst, addr);
	temp &= (~mask);
	temp |= (mask & val);
	SiiDrvCraWrReg8(inst, addr, temp);
}

void SiiDrvCraWrReg16(SiiInst_t inst, SiiDrvCraAddr_t addr, uint16_t val )
{   
	uint8_t writeVal;
	writeVal = val & 0xff;
	SiiDrvCraWrReg8(inst, addr, writeVal);

	writeVal = (val >> 8) & 0xff;;
	SiiDrvCraWrReg8(inst, (addr + 1), writeVal);
}

void SiiDrvCraWrReg24(SiiInst_t inst, SiiDrvCraAddr_t addr, uint32_t val )
{
	uint8_t writeVal;
	writeVal = val & 0xff;
	SiiDrvCraWrReg8(inst, addr, writeVal);

	writeVal = (val >> 8) & 0xff;;
	SiiDrvCraWrReg8(inst, (addr + 1), writeVal);

	writeVal = (val >> 16) & 0xff;;
	SiiDrvCraWrReg8(inst, (addr + 2), writeVal);
}

void SiiDrvCraFifoRead8(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t* pData, SiiDrvCraSize_t size )
{
	uint16_t i;
	for(i = 0; i < size; i++)
	{
		*pData = SiiDrvCraRdReg8(inst, addr);
		pData++;
	}
}

void SiiDrvCraFifoWrite8(SiiInst_t inst, SiiDrvCraAddr_t addr, const uint8_t* pData, SiiDrvCraSize_t size )
{
	uint16_t i;
	for(i = 0; i < size; i++)
	{
		SiiDrvCraWrReg8(inst, addr, *pData);
		pData++;
	}
}

void SiiDrvCraBlockWrite8(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t* pData, SiiDrvCraSize_t size )
{
	CraObj_t  *pObj = (CraObj_t  *) SII_LIB_OBJ_PNTR(inst);

	uint16_t i;
	SiiPlatformStatus_t platStat;
	for(i = 0; i < CRA_RETRY_COUNT; i++)
	{
		platStat = SiiPlatformWrite(pObj->pConfig->pInterfaceInfo, 0x40, addr, pData, size, pObj->pConfig->pu32BaseAddrVirtual);
		if(platStat == SII_PLATFORM_STATUS__SUCCESS)
			return;
	}

	if(pObj->pConfig->callBack)
		pObj->pConfig->callBack();
}

void SiiDrvCraBlockRead8(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t* pData, SiiDrvCraSize_t size )
{
	CraObj_t  *pObj = (CraObj_t  *) SII_LIB_OBJ_PNTR(inst);
	SiiPlatformStatus_t platStat;
	uint8_t i;
//	uint8_t retVal = 0;

	for(i=0; i < CRA_RETRY_COUNT; i++)
	{
		platStat = SiiPlatformRead(pObj->pConfig->pInterfaceInfo, 0x40, addr, pData, size, pObj->pConfig->pu32BaseAddrVirtual);
		if(platStat == SII_PLATFORM_STATUS__SUCCESS)
			return;
	}

	if(pObj->pConfig->callBack)
		pObj->pConfig->callBack();
}



bool_t SiiDCardRegBlockRead(SiiInst_t inst, uint8_t deviceId, uint16_t regAddr, uint8_t *pBuffer, uint16_t count)
{
	CraObj_t *pObj = (CraObj_t *)SII_LIB_OBJ_PNTR(inst);
	uint16_t i;
  //  bool_t status = true;

	SiiPlatformInterface_t *pebbelsInterfaceinfo;
	pebbelsInterfaceinfo = (SiiPlatformInterface_t*)SiiLibMallocCreate(sizeof(SiiPlatformInterface_t));
	memcpy(pebbelsInterfaceinfo, pObj->pConfig->pInterfaceInfo, sizeof(SiiPlatformInterface_t));
	pebbelsInterfaceinfo->interfaceType = SII_PLATFORM_TYPE__I2C;

    for (i=0; i < CRA_RETRY_COUNT; i++)
    {
        if( SII_PLATFORM_STATUS__SUCCESS == SiiPlatformRead(pebbelsInterfaceinfo, deviceId, regAddr+i, &pBuffer[i], 1, pObj->pConfig->pu32BaseAddrVirtual))
		{
			SiiLibMallocDelete(pebbelsInterfaceinfo);
            return true;
		}
    }

	SiiLibMallocDelete(pebbelsInterfaceinfo);
	if(pObj->pConfig->callBack)
		pObj->pConfig->callBack();
	
	return false;
}

bool_t SiiDCardRegBlockWrite(SiiInst_t inst, uint8_t deviceId, uint16_t regAddr, uint8_t *pBuffer, uint16_t count)
{
	CraObj_t *pObj = (CraObj_t *)SII_LIB_OBJ_PNTR(inst);

	uint16_t i;
    //bool_t status = true;
	
	SiiPlatformInterface_t *pebbelsInterfaceinfo;
	pebbelsInterfaceinfo = (SiiPlatformInterface_t*)SiiLibMallocCreate(sizeof(SiiPlatformInterface_t));
	memcpy(pebbelsInterfaceinfo,pObj->pConfig->pInterfaceInfo,sizeof(SiiPlatformInterface_t));
	pebbelsInterfaceinfo->interfaceType = SII_PLATFORM_TYPE__I2C;

    for (i=0; i < CRA_RETRY_COUNT; i++)
    {
		if( SII_PLATFORM_STATUS__SUCCESS == SiiPlatformWrite(pebbelsInterfaceinfo, deviceId, regAddr, pBuffer, count, pObj->pConfig->pu32BaseAddrVirtual))
		{
			SiiLibMallocDelete(pebbelsInterfaceinfo);
			return true;
		}
    }

	if(pObj->pConfig->callBack)
		pObj->pConfig->callBack();

	SiiLibMallocDelete(pebbelsInterfaceinfo);
	return false;
}

bool_t SiiDrvCraIsInterruptRcvd(SiiInst_t inst)
{
#ifdef SIMG_PLATFORM
    SiiPlatformGPIOLevel_t intrPinLevel;
    CraObj_t *pObj = (CraObj_t *) SII_LIB_OBJ_PNTR(inst);
	intrPinLevel = SiiPlatformGPIOStatusGet(pObj->pConfig->pInterfaceInfo, pObj->pConfig->intrPin);
	return !intrPinLevel? true:false;
#else
    return true;
#endif
}

/** END of File *********************************************************/
