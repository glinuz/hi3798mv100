/******************************************************************************

Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_adp.h
Version       : Initial
Author        : Hisilicon hisecurity team
Created       : 2015-12-24
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef __HI_CX_ADP_H__
#define __HI_CX_ADP_H__

#include "hi_type.h"
#include "hi_unf_advca.h"
#include "hi_unf_cipher.h"
#include "cx_verify_def.h"

#define MAX_DATA_LEN (1024*1024 - 16) //the max data length for encryption / decryption



HI_S32 HI_CX_ADP_DataEncrypt(HI_CA_CryptoInfo_S *pstCryptoInfo, HI_U8 *pPlainText, HI_U32 u32Len, HI_U8 *pCipherText);
HI_S32 HI_CX_ADP_DataDecrypt(HI_CA_CryptoInfo_S *pstCryptoInfo, HI_U8 *pCipherText, HI_U32 u32Len, HI_U8 *pPlainText);
HI_S32 HI_CX_ADP_CalcAesCMAC(HI_U8 *pInputData, HI_U32 u32Len, HI_U8 pu8Key[16], HI_U8 pOutputMAC[16]);

HI_U32 HI_CX_ADP_VerifyImage(HI_U32 u32RamAddr,HI_U32 u32DataLen, HI_U8 *pu8Signature, AUTHALG_E eAuthType);


#endif/*__HI_CA_DRV_H__*/
