/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_otp_ext.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/

#ifndef __DRV_OTP_EXT_H__
#define __DRV_OTP_EXT_H__

//#include "drv_otp_ioctl.h"
#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef HI_U32 (*FN_OTP_V200_Read)(HI_U32 addr);
typedef HI_U8  (*FN_OTP_V200_ReadByte)(HI_U32 addr);
typedef HI_S32 (*FN_OTP_V200_Write)(HI_U32 addr,HI_U32 tdata);
typedef HI_S32 (*FN_OTP_V200_WriteByte)(HI_U32 addr, HI_U8 tdata);
typedef HI_S32 (*FN_OTP_V200_WriteBit)(HI_U32 addr, HI_U32 bit_pos, HI_U32  bit_value);

typedef HI_S32 (*FN_OTP_V100_WriteByte)(HI_U32 addr, HI_U32 tdata);
typedef HI_S32 (*FN_OTP_V100_WriteBit)(HI_U32 addr, HI_U32 bit_pos, HI_U32 bit_value);
typedef HI_S32 (*FN_OTP_V100_SetWriteProtect)(HI_VOID);
typedef HI_S32 (*FN_OTP_V100_GetWriteProtect)(HI_U32 *penable);
typedef HI_U32 (*FN_OTP_V100_Read)(HI_U32 addr);
typedef HI_S32 (*FN_OTP_V100_GetSrBit)(HI_S32 pos,HI_S32 *pvalue);
typedef HI_S32 (*FN_OTP_V100_SetSrBit)(HI_S32 pos);
typedef HI_S32 (*FN_OTP_V100_FuncDisable)(HI_U32 bit_pos, HI_U32 bit_value);
typedef HI_S32 (*FN_OTP_V100_Reset)(void);

typedef int (*FN_OTP_Suspend)(HI_VOID);
typedef int (*FN_OTP_Resume)(HI_VOID);

typedef struct
{
    FN_OTP_V200_Read            HAL_OTP_V200_Read;
    FN_OTP_V200_ReadByte        HAL_OTP_V200_ReadByte;
    FN_OTP_V200_Write           HAL_OTP_V200_Write;
    FN_OTP_V200_WriteByte       HAL_OTP_V200_WriteByte;
    FN_OTP_V200_WriteBit        HAL_OTP_V200_WriteBit;
    FN_OTP_V100_WriteByte       HAL_OTP_V100_WriteByte;
    FN_OTP_V100_WriteBit        HAL_OTP_V100_WriteBit;
    FN_OTP_V100_SetWriteProtect HAL_OTP_V100_SetWriteProtect;
    FN_OTP_V100_GetWriteProtect HAL_OTP_V100_GetWriteProtect;
    FN_OTP_V100_Read            HAL_OTP_V100_Read;
    FN_OTP_V100_GetSrBit        HAL_OTP_V100_GetSrBit;
    FN_OTP_V100_SetSrBit        HAL_OTP_V100_SetSrBit;
    FN_OTP_V100_FuncDisable     HAL_OTP_V100_FuncDisable;
    FN_OTP_V100_Reset           HAL_OTP_V100_Reset;
	FN_OTP_Suspend				pfnOTPSuspend;
	FN_OTP_Suspend				pfnOTPResume;
}OTP_EXPORT_FUNC_S;

HI_S32	OTP_DRV_ModInit(HI_VOID);
HI_VOID	OTP_DRV_ModExit(HI_VOID);


#ifdef __cplusplus
}
#endif
#endif /* __DRV_OTP_EXT_H__ */

