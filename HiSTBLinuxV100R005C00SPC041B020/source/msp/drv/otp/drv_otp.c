/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_otp.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <asm/delay.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/memory.h>
#include "hi_type.h"
#include "drv_otp.h"
#include "drv_otp_ext.h"
#include "drv_otp_common.h"
#include "drv_otp_v200.h"
#include "drv_otp_reg_v200.h"
#if defined(CHIP_TYPE_hi3716mv310)
#include "drv_dev_ext.h"
#else
#include "hi_drv_dev.h"
#endif
#include "hi_drv_module.h"
#include "drv_cipher_ext.h"
#include "hi_reg_common.h"

#define OTP_NAME "HI_OTP"

static OTP_EXPORT_FUNC_S s_OTPExportFuncs =
{
	.HAL_OTP_V200_Read 		        =	HAL_OTP_V200_Read,
	.HAL_OTP_V200_ReadByte	        =   HAL_OTP_V200_ReadByte,
	.HAL_OTP_V200_Write		        =   HAL_OTP_V200_Write,
	.HAL_OTP_V200_WriteByte	        =   HAL_OTP_V200_WriteByte,
	.HAL_OTP_V200_WriteBit	        =   HAL_OTP_V200_WriteBit,
	.pfnOTPSuspend					=	HI_DRV_OTP_Suspend,
	.pfnOTPResume					=	HI_DRV_OTP_Resume,
};

extern HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipVersion);

static OTP_VERSION_E gOTPVesion = OTP_VERSION_200;

static HI_VOID DRV_OTP_ClockConfig(HI_VOID)
{
#ifdef CHIP_TYPE_hi3716cv200es
    U_PERI_CRG48 unOTPCrg;

    unOTPCrg.u32 = 0;
    unOTPCrg.u32 = g_pstRegCrg->PERI_CRG48.u32;
    if( 0 == unOTPCrg.bits.otp_bus_cken )
    {
        unOTPCrg.bits.otp_bus_cken = 1;
        g_pstRegCrg->PERI_CRG48.u32 = unOTPCrg.u32;
    }
#endif
    return;
}

HI_S32 DRV_OTP_Init(void)
{
    HI_S32 ret = HI_SUCCESS;

    (HI_VOID)DRV_OTP_ClockConfig();

    ret = HI_DRV_MODULE_Register(HI_ID_OTP, OTP_NAME, (HI_VOID*)&s_OTPExportFuncs);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_OTP("HI_DRV_MODULE_Register otp failed\n");
        return ret;
    }

    gOTPVesion = OTP_VERSION_200;

    return HI_SUCCESS;
}

HI_S32 DRV_OTP_DeInit(void)
{
    HI_DRV_MODULE_UnRegister(HI_ID_OTP);
    return HI_SUCCESS;
}


HI_U32 DRV_OTP_Read(HI_U32 Addr)
{
    return HAL_OTP_V200_Read(Addr);
}

HI_U8 DRV_OTP_ReadByte(HI_U32 Addr)
{
    return HAL_OTP_V200_ReadByte(Addr);
}

HI_S32 DRV_OTP_Write(HI_U32 Addr, HI_U32 value)
{
    HI_S32 ErrorReturn = HI_SUCCESS;

    HI_U8 *pByte = (HI_U8*)&value;
    ErrorReturn = HAL_OTP_V200_WriteByte((Addr + 0), pByte[0]);
    ErrorReturn |= HAL_OTP_V200_WriteByte((Addr + 1), pByte[1]);
    ErrorReturn |= HAL_OTP_V200_WriteByte((Addr + 2), pByte[2]);
    ErrorReturn |= HAL_OTP_V200_WriteByte((Addr + 3), pByte[3]);

    return ErrorReturn;
}

HI_S32 DRV_OTP_Write_Byte(HI_U32 Addr, HI_U8 value)
{
    return HAL_OTP_V200_WriteByte(Addr, value);;
}

HI_S32 DRV_OTP_Write_Bit(HI_U32 Addr, HI_U32 BitPos, HI_U32 BitValue)
{
	if (BitPos >= 8)
	{
		HI_ERR_OTP("Write OTP bit ERROR! BitPos >= 8\n");
		return HI_FAILURE;
	}

	if (BitValue > 1)
	{
		HI_ERR_OTP("Write OTP bit ERROR! BitValue > 1\n");
		return HI_FAILURE;
	}

    return HAL_OTP_V200_WriteBit(Addr, BitPos, BitValue);
}

HI_S32 DRV_OTP_Reset(HI_VOID)
{
    return OTP_V200_Reset();
}

HI_S32 DRV_OTP_Set_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey)
{
    return OTP_V200_Set_CustomerKey(pCustomerKey);
}

HI_S32 DRV_OTP_Get_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey)
{
    return OTP_V200_Get_CustomerKey(pCustomerKey);
}

HI_S32 DRV_OTP_Get_DDPLUS_Flag(HI_BOOL *pDDPLUSFlag)
{
 	return OTP_V200_Get_DDPLUS_Flag(pDDPLUSFlag);
}

HI_S32 DRV_OTP_Get_DTS_Flag(HI_BOOL *pDTSFlag)
{
 	return OTP_V200_Get_DTS_Flag(pDTSFlag);
}

HI_S32 DRV_OTP_Set_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData)
{
     return OTP_V200_Set_StbPrivData(pStbPrivData);
}

HI_S32 DRV_OTP_Get_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData)
{
     return OTP_V200_Get_StbPrivData(pStbPrivData);
}

HI_S32 DRV_OTP_DieID_Check(HI_VOID)
{
    HI_S32 ErrorReturn = HI_FAILURE;

    ErrorReturn = OTP_V200_DieID_Check();
    ErrorReturn |= OTP_V200_Reset();  

    return ErrorReturn;
}

HI_S32 HI_DRV_OTP_Suspend(HI_VOID)
{
	return HI_SUCCESS;
}

HI_S32 HI_DRV_OTP_Resume(HI_VOID)
{
	return HI_SUCCESS;
}

EXPORT_SYMBOL(HI_DRV_OTP_Suspend);
EXPORT_SYMBOL(HI_DRV_OTP_Resume);
