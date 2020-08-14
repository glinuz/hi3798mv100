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
#ifndef __HI_CA_ADP_H__
#define __HI_CA_ADP_H__

#include "hi_type.h"
#include "hi_unf_advca.h"
#include "hi_unf_cipher.h"
#include "ca_verify_def.h"

#define MAX_DATA_LEN (1024*1024 - 16) //the max data length for encryption / decryption



HI_S32 HI_CA_ADP_DataEncrypt(HI_CA_CryptoInfo_S *pstCryptoInfo, HI_U8 *pPlainText, HI_U32 u32Len, HI_U8 *pCipherText);
HI_S32 HI_CA_ADP_DataDecrypt(HI_CA_CryptoInfo_S *pstCryptoInfo, HI_U8 *pCipherText, HI_U32 u32Len, HI_U8 *pPlainText);
HI_U32 HI_CA_ADP_VerifyImage(HI_U32 u32RamAddr, HI_U32 u32DataLen,HI_U8 *pu8Signature, AUTHALG_E eAuthType);
HI_U32 HI_CA_ADP_CalcHASH(HI_U8 *pu8Data, HI_U32 u32DataLen, HI_U8 *pu8Output, HI_Algorithm_EN eHashType);


#endif/*__HI_CA_ADP_H__*/
