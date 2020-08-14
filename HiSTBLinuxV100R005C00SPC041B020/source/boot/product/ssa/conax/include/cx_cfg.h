/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : cx_cfg.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2016-01-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef __CX_CFG_H__
#define __CX_CFG_H__

#include "hi_type.h"
#include "cx_verify_def.h"

HI_S32 HI_CX_GetKeyladderKey(HI_CA_KEY_GROUP_E enKeyGroup, HI_U8 u8KeyladderKey1[16], HI_U8 u8KeyladderKey2[16], HI_U8 u8CipherKey[16]);
HI_S32 HI_CX_GetBLPKInfor(HI_U8 u8BLPK[16]);
HI_S32 HI_CX_GetRSAKeyInfor(AUTHALG_E eAuthType, HI_UNF_CIPHER_RSA_PUB_KEY_S *pstPubKey);

#endif /*__CX_CFG_H__*/
