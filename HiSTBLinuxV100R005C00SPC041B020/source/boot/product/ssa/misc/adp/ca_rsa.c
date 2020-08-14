/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : ca_rsa.c
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
#include "ca_verify_def.h"
#include "ca_debug.h"
#include "ca_cfg.h"


HI_U32 HI_CA_ADP_VerifyImage(HI_U32 u32RamAddr,HI_U32 u32DataLen, HI_U8 *pu8Signature, AUTHALG_E eAuthType)
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
	ret = HI_CA_GetRSAKeyInfor(eAuthType, &stRsaVerify.stPubKey);
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

HI_U32 HI_CA_ADP_CalcHASH(HI_U8 *pu8Data, HI_U32 u32DataLen, HI_U8 *pu8Output, HI_Algorithm_EN eHashType)
{
	int ret = 0;
	HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
	HI_HANDLE HashHandle;

	if(NULL == pu8Data || NULL == pu8Output || eHashType > Algoritm_BUTT)
	{
		HI_ERR_CA("Invalid parameters!\n");
		return HI_FAILURE;
	}

	ret = HI_UNF_CIPHER_Init();
	if(ret != HI_SUCCESS)
	{
		HI_ERR_CA("cipher init failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	
	memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));
	
	if (eHashType == Algoritm_Hash1)
	{
      stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_SHA1;
    }
    else if (eHashType == Algoritm_Hash2)
    {
      stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_SHA256;
    }

    stHashAttr.u32TotalDataLen = u32DataLen;
    
    ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &HashHandle);
    if(ret != HI_SUCCESS)
	{
		HI_ERR_CA("Fail to init Hash, ret:0x%x\n", ret);
		ret = HI_FAILURE;
	    goto ERROR_EXIT;	
	}
	
    ret = HI_UNF_CIPHER_HashUpdate(HashHandle, pu8Data, u32DataLen);
    if(ret != HI_SUCCESS)
    {
		HI_ERR_CA("Fail to call update Hash, ret:0x%x\n", ret);
		ret = HI_FAILURE;
	    goto ERROR_EXIT;	
	}

    ret = HI_UNF_CIPHER_HashFinal(HashHandle, pu8Output);
    if(ret != HI_SUCCESS)
    {
		HI_ERR_CA("Fail to call final Hash, ret:0x%x\n", ret);
		ret = HI_FAILURE;
	    goto ERROR_EXIT;	
	}
    
	ret = HI_SUCCESS;

ERROR_EXIT:
	HI_UNF_CIPHER_DeInit();  
	return ret;	
}