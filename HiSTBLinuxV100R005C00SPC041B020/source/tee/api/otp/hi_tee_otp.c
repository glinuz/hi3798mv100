/******************************************************************************
 *
 * Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
 * ******************************************************************************
 * File Name     : hi_tee_otp.c
 * Version       : Initial
 * Author        : Hisilicon hisecurity team
 * Created       : 2013-12-28
 * Last Modified :
 * Description   : 
 * Function List :
 * History       :
 * ******************************************************************************/

#include "hi_tee_otp.h"
#include "sre_syscalls_chip.h"
#include "hi_type.h"
#include "tee_log.h"

#define HI_ERR_OTP(fmt...)    uart_printf_func(fmt)
#define HI_INFO_OTP(fmt...)    uart_printf_func(fmt)

HI_S32 HI_TEE_OTP_Init(HI_VOID)
{
    return __OTP_Init();
}

HI_S32 HI_TEE_OTP_DeInit(HI_VOID)
{
    return __OTP_DeInit();
}

HI_S32 HI_TEE_OTP_Reset(HI_VOID)
{
    return __OTP_Reset();
}

HI_S32 HI_TEE_OTP_GetChipID(HI_U8 *pu8ChipID)
{
    if (HI_NULL == pu8ChipID)
    {
        HI_ERR_OTP("pu8ChipID is null\n");
        return HI_FAILURE;
    }

    return __OTP_GetChipID(pu8ChipID);
}

HI_S32 HI_TEE_OTP_ReadWord(HI_U32 Addr, HI_U32 *pu32Value)
{
    if (HI_NULL == pu32Value)
    {
        HI_ERR_OTP("pu32Value is null\n");
        return HI_FAILURE;
    }

    return __OTP_ReadWord(Addr, pu32Value);
}

HI_S32 HI_TEE_OTP_ReadByte(HI_U32 Addr, HI_U8 *pu8Value)
{
    if (HI_NULL == pu8Value)
    {
        HI_ERR_OTP("pu8Value is null\n");
        return HI_FAILURE;
    }

    return __OTP_ReadByte(Addr, pu8Value);
}

HI_S32 HI_TEE_OTP_WriteWord(HI_U32 Addr, HI_U32 u32Value)
{
    return __OTP_WriteWord(Addr, u32Value);
}

HI_S32 HI_TEE_OTP_WriteByte(HI_U32 Addr, HI_U8 u8Value)
{
    return __OTP_WriteByte(Addr, u8Value);
}


