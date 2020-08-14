/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : cx_rsa.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "hi_type.h"
#ifdef HI_MINIBOOT_SUPPORT
#include "app.h"
#else
#include "exports.h"
#endif

#include "hi_unf_cipher.h"
#include "cx_verify_def.h"
#include "cx_debug.h"
#include "cx_cfg.h"


HI_U32 HI_CX_ADP_VerifyImage(HI_U32 u32RamAddr,HI_U32 u32DataLen, HI_U8 *pu8Signature, AUTHALG_E eAuthType)
{
	int ret = 0;
	HI_UNF_CIPHER_RSA_VERIFY_S stRsaVerify;
	HI_U32 u32InSignLen;

	if(NULL == pu8Signature)
	{
		HI_ERR_CA("Invalid parameters, NULL pointer!\n");
		return HI_FAILURE;
	}

	ret = HI_UNF_CIPHER_Init();
	if(ret != HI_SUCCESS)
	{
		HI_ERR_CA("cipher init failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	memset(&stRsaVerify, 0, sizeof(HI_UNF_CIPHER_RSA_VERIFY_S));
	stRsaVerify.enScheme = HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256;
	ret = HI_CX_GetRSAKeyInfor(eAuthType, &stRsaVerify.stPubKey);
	if(ret != HI_SUCCESS)
	{
		HI_ERR_CA("Fail to get RSA key info:0x%x\n", ret);
		ret = HI_FAILURE;
		goto ERROR_EXIT;
	}
	u32InSignLen = 256;
	
	ret = HI_UNF_CIPHER_RsaVerify(&stRsaVerify, (HI_U8 *)u32RamAddr, u32DataLen, NULL, pu8Signature, u32InSignLen);
	if(HI_SUCCESS != ret )
	{
		HI_ERR_CA("Rsa verify failed, ret:0x%x\n", ret);
		ret = HI_FAILURE;
		goto ERROR_EXIT;
	}

	ret = HI_SUCCESS;

ERROR_EXIT:
	HI_UNF_CIPHER_DeInit();  
	return ret;	
}

