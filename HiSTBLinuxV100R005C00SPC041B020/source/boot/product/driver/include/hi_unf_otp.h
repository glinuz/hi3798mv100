/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_unf_otp.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2014-09-16
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __HI_UNF_OTP_H__
#define __HI_UNF_OTP_H__

#include "otp.h"

HI_S32 HI_UNF_OTP_Init(HI_VOID);

HI_S32 HI_UNF_OTP_DeInit(HI_VOID);

HI_S32 HI_UNF_OTP_SetStbPrivData(HI_U32 u32Offset, HI_U8 u8Data);

HI_S32 HI_UNF_OTP_GetStbPrivData(HI_U32 u32Offset, HI_U8 *pu8Data);

HI_S32 HI_UNF_OTP_Get_CustomerKey(HI_U8 *pKey, HI_U32 u32KeyLen);

HI_S32 HI_UNF_OTP_GetIDWordLockFlag(HI_BOOL *pbLockFlag);

HI_S32 HI_UNF_OTP_LockIDWord(HI_VOID);

HI_S32 HI_UNF_OTP_BurnToNormalChipset(HI_VOID);

HI_S32 HI_UNF_OTP_BurnToSecureChipset(HI_VOID);

HI_U32 HI_UNF_OTP_Read(HI_U32 addr);

HI_U8  HI_UNF_OTP_ReadByte(HI_U32 addr);

HI_S32 HI_UNF_OTP_Write(HI_U32 addr, HI_U32 u32Data);

HI_S32 HI_UNF_OTP_WriteByte(HI_U32 addr, HI_U8 u8data);

/* The following interfaces are not considered as UNF */
HI_S32 HI_OTP_WriteByte(HI_U32 addr, HI_U8 u8data);

HI_U32 HI_OTP_Read(HI_U32 addr);

HI_U8  HI_OTP_ReadByte(HI_U32 addr);



#endif

