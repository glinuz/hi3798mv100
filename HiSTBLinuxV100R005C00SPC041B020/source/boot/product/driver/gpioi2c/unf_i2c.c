/***********************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_unf_i2c.c
 * Description:
 *
 * History:
 * Version   Date                Author     DefectNum    Description
 * main\1    2008-10-10   w00142069      NULL         Create this file.
 ******************************************************************************/
#include "hi_type.h"
#include "hi_drv_gpioi2c.h"	 
#include "hi_drv_i2c.h"	 
#include "hi_unf_i2c.h"
#include "hi_error_mpi.h"


/*******************************************
Function:              HI_UNF_GPIOI2CI2C_Init
Description:  init i2c device
Calls:        HI_DRV_GPIOI2C_Init
Data Accessed:      NA
Data Updated:        NA
Input:                  NA
Output:                NA
Return:         ErrorCode(reference to document)
Others:                  NA
*******************************************/
HI_S32 HI_UNF_I2C_Init (HI_VOID)
{
	HI_S32 Ret=0;
	Ret=HI_DRV_I2C_Init();
    Ret|=HI_DRV_GPIOI2C_Init();
	return Ret;
}

/*******************************************
Function:              HI_UNF_I2C_DeInit
Description:  deinit i2c device
Calls:        HI_DRV_GPIOI2C_DeInit
Data Accessed:      NA
Data Updated:        NA
Input:                  NA
Output:                NA
Return:         ErrorCode(reference to document)
Others:                  NA
*******************************************/
HI_S32 HI_UNF_I2C_DeInit (HI_VOID)
{
	HI_DRV_I2C_Exit();
    HI_DRV_GPIOI2C_DeInit();
	return HI_SUCCESS;
}

/*******************************************
Function:              HI_UNF_I2C_GetCapability
Description:  Call this API to get the number of I2C module befor read/write data
Calls:        
Data Accessed:      NA
Data Updated:        NA
Input:                  NA
Output:pu32I2cNum - the number of I2C module
Return:         ErrorCode(reference to document)
Others:                  NA
*******************************************/
HI_S32 HI_UNF_I2C_GetCapability(HI_U32 *pu32I2cNum)
{
	if (HI_NULL != pu32I2cNum)
	{
		*pu32I2cNum = HI_STD_I2C_NUM;
		return HI_SUCCESS;
	}

	return HI_FAILURE;
}


/*******************************************
Function:       HI_UNF_I2C_CreateGpioI2c
Description:    config param
Calls:          HI_DRV_GPIOI2C_CreateGpioI2c
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:
Return:         ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_I2C_CreateGpioI2c(HI_U32 *pu32I2cNum, HI_U32 u32SCLGpioNo, HI_U32 u32SDAGpioNo)
{
    if (HI_NULL == pu32I2cNum)
    {
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (u32SCLGpioNo == u32SDAGpioNo)
    {
        return HI_ERR_I2C_INVALID_PARA;
    }
  
   return HI_DRV_GPIOI2C_CreateGpioI2c(pu32I2cNum,u32SCLGpioNo,u32SDAGpioNo);
	
}


/*******************************************
Function:       HI_UNF_I2C_DestroyGpioI2c
Description:    config param
Calls:          HI_DRV_GPIOI2C_DestroyGpioI2c
Data Accessed:  NA
Data Updated:   NA
Input:          NA
Output:
Return:         ErrorCode(reference to document)
Others:         NA
*******************************************/
HI_S32 HI_UNF_I2C_DestroyGpioI2c(HI_U32 u32I2cNum)
{
	
	if ((HI_STD_I2C_NUM > u32I2cNum) || (HI_I2C_MAX_NUM_USER <= u32I2cNum))
	{
		 return HI_ERR_I2C_INVALID_PARA;
	}
	return HI_DRV_GPIOI2C_DestroyGpioI2c(u32I2cNum);
}



/*******************************************
Function:              HI_UNF_I2C_Read
Description:  read data via i2c
Calls:        HI_DRV_GPIOI2C_ReadExt
Data Accessed:      NA
Data Updated:        NA
Input:                  NA
Output:                NA
Return:         ErrorCode(reference to document)
Others:                  NA
*******************************************/

HI_S32 HI_UNF_I2C_Read(HI_U32 u32I2cNum, HI_U8 u8DevAddress, HI_U32 u32RegAddr,
                       HI_U32 u32RegAddrCount, HI_U8 *pu8Buf, HI_U32 u32Length)
{
    if (u32I2cNum > HI_I2C_MAX_NUM_USER)
    {
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (u32RegAddrCount > 4)
    {
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (!pu8Buf)
    {
        return HI_ERR_I2C_NULL_PTR;
    }

    if ((u32Length > HI_I2C_MAX_LENGTH) || (0 == u32Length))
    {
        return HI_ERR_I2C_INVALID_PARA;
    }
	
	if(u32I2cNum<HI_STD_I2C_NUM)
		return HI_DRV_I2C_Read(u32I2cNum,u8DevAddress,u32RegAddr,u32RegAddrCount,pu8Buf,u32Length);
	else
		return HI_DRV_GPIOI2C_ReadExt(u32I2cNum,u8DevAddress,u32RegAddr,u32RegAddrCount,pu8Buf,u32Length);
		
}

/*******************************************
Function:              HI_UNF_I2C_Write
Description:  write data via i2c
Calls:        HI_DRV_GPIOI2C_WriteExt
Data Accessed:      NA
Data Updated:        NA
Input:                  NA
Output:                NA
Return:         ErrorCode(reference to document)
Others:                  NA
*******************************************/
HI_S32 HI_UNF_I2C_Write(HI_U32 u32I2cNum, HI_U8 u8DevAddress, HI_U32 u32RegAddr,
                        HI_U32 u32RegAddrCount, HI_U8 *pu8Buf, HI_U32 u32Length)
{
    if (u32I2cNum > HI_I2C_MAX_NUM_USER)
    {
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (u32RegAddrCount > 4)
    {
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (!pu8Buf)
    {
        return HI_ERR_I2C_NULL_PTR;
    }

    if ((u32Length > HI_I2C_MAX_LENGTH) || (0 == u32Length))
    {
        return HI_ERR_I2C_INVALID_PARA;
    }

	if(u32I2cNum<HI_STD_I2C_NUM)
		return HI_DRV_I2C_Write(u32I2cNum,u8DevAddress,u32RegAddr,u32RegAddrCount,pu8Buf,u32Length);
	else
		return HI_DRV_GPIOI2C_WriteExt(u32I2cNum,u8DevAddress,u32RegAddr,u32RegAddrCount,pu8Buf,u32Length);
    
}


/*******************************************
Function:              HI_UNF_I2C_SetRate
Description:  set the rate
Calls:        HI_I2C_SetRate
Data Accessed:      NA
Data Updated:        NA
Input:                  NA
Output:                NA
Return:         ErrorCode(reference to document)
Others:                  NA
*******************************************/
HI_S32 HI_UNF_I2C_SetRate(HI_U32 u32I2cNum, HI_UNF_I2C_RATE_E enI2cRate)
{
    HI_U32 I2cRate;

    if (u32I2cNum >= HI_STD_I2C_NUM)
    {
        return HI_ERR_I2C_INVALID_PARA;
    }

	
	switch (enI2cRate)
	   {
	   case HI_UNF_I2C_RATE_10K:
		   I2cRate = 10000;
		   break;
	   case HI_UNF_I2C_RATE_50K:
		   I2cRate = 50000;
		   break;
	   case HI_UNF_I2C_RATE_100K:
		   I2cRate = 100000;
		   break;
	   case HI_UNF_I2C_RATE_200K:
		   I2cRate = 200000;
		   break;
	   case HI_UNF_I2C_RATE_300K:
		   I2cRate = 300000;
		   break;
	   case HI_UNF_I2C_RATE_400K:
		   I2cRate = 400000;
		   break;
	   default:
		   return HI_ERR_I2C_INVALID_PARA;
	   }

   return HI_DRV_I2C_SetRate(u32I2cNum,I2cRate);
}

/*******************************************
Function:              HI_UNF_I2C_SetRateEx
Description:  set the rate
Calls:        HI_I2C_SetRate
Data Accessed:      NA
Data Updated:        NA
Input:                  NA
Output:                NA
Return:         ErrorCode(reference to document)
Others:                  NA
*******************************************/
HI_S32 HI_UNF_I2C_SetRateEx(HI_U32 u32I2cNum, HI_U32 u32I2cRate)
{
  
    if (u32I2cNum >= HI_STD_I2C_NUM)
    {
        return HI_ERR_I2C_INVALID_PARA;
    }

    if (u32I2cRate > 400 * 1000)
    {
        return HI_ERR_I2C_INVALID_PARA;
    }
	
    return HI_DRV_I2C_SetRate(u32I2cNum,u32I2cRate);

    return HI_SUCCESS;
}



