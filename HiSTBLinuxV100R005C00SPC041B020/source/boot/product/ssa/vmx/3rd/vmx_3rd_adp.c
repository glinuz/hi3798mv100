/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : vmx_3rd_auth.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2014-09-19
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifdef LOADER
#include <string.h>
#include "hi_common.h"
#endif

#include "hi_debug.h"

#ifdef HI_MINIBOOT_SUPPORT
#include "app.h"
#endif

#ifdef HI_UBOOT_SUPPORT
#include "exports.h"
#endif

#include "vmx_3rd_adp.h"
#include "vmx_3rd_auth.h"
#include "vmx_3rd_adp.h"
#include "hi_unf_advca.h"
#include "hi_unf_cipher.h"

#include "vmx_3rd_config.h"


#define VMX_PRINT(fmt...)  HI_INFO_PRINT(HI_ID_CA, fmt)



#define HASH_SIZE (20*1024*1024)    //20M per round inside this interface for hash

static HI_BOOL s_bCipherInit = HI_FALSE;
static HI_HANDLE s_hSWPKCipher = HI_INVALID_HANDLE;
static HI_HANDLE s_hMISCCipher = HI_INVALID_HANDLE;


static HI_S32 VMX3RD_ADP_R2RCipherInit(HI_VOID)
{
    HI_S32 ret;
    if (HI_TRUE == s_bCipherInit)
    {
        return HI_SUCCESS;
    }

    ret = HI_UNF_CIPHER_Init();
    if (HI_SUCCESS != ret)
    {
		VMX_PRINT("HI_UNF_CIPHER_Init error ret:0x%x\n", ret);
        s_bCipherInit = HI_FALSE;
        return HI_FAILURE;
    }

    s_bCipherInit = HI_TRUE;

    return HI_SUCCESS;
}

static HI_S32 CreateCipherHandle(HI_HANDLE *pCipherHandle)
{
    HI_UNF_CIPHER_ATTS_S stCipherAtts;
    HI_HANDLE hCipher = HI_INVALID_HANDLE;
    HI_S32 ret;

    if (HI_FALSE == s_bCipherInit)
    {
		VMX_PRINT("Cipher not init\n");
        return HI_FAILURE;
    }

    memset(&stCipherAtts, 0, sizeof(HI_UNF_CIPHER_ATTS_S));
    stCipherAtts.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    ret = HI_UNF_CIPHER_CreateHandle(&hCipher, &stCipherAtts);
    if (HI_SUCCESS != ret)
    {
		VMX_PRINT("Create cipher handle error ret:%x\n", ret);
        return HI_FAILURE;
    }
    
    *pCipherHandle = hCipher;

    return HI_SUCCESS;
}

static HI_HANDLE VMX3RD_ADP_GetMISCCipherHandle(HI_VOID)
{
    HI_S32 ret;
    HI_HANDLE hCipherHandle;

    if (HI_INVALID_HANDLE != s_hMISCCipher)
    {
        return s_hMISCCipher;
    }

    ret = CreateCipherHandle(&hCipherHandle);
    if (HI_SUCCESS != ret)
    {
        return HI_INVALID_HANDLE;
    }

    s_hMISCCipher = hCipherHandle;
    return s_hMISCCipher;
}

static HI_HANDLE VMX3RD_ADP_GetSWPKCipherHandle(HI_VOID)
{
    HI_S32 ret;
    HI_HANDLE hCipherHandle;

    if (HI_INVALID_HANDLE != s_hSWPKCipher)
    {
        return s_hSWPKCipher;
    }

    ret = CreateCipherHandle(&hCipherHandle);
    if (HI_SUCCESS != ret)
    {
        return HI_INVALID_HANDLE;
    }

    s_hSWPKCipher = hCipherHandle;
    return s_hSWPKCipher;
}

int VMX3RD_ADP_R2RByMiscKeyladder(unsigned char bR2REncDec, unsigned char* input, int length, unsigned char* output)
{
	HI_S32 ret = 0;
	HI_UNF_ADVCA_KEYLADDER_ATTR_S stKLAttr;
    HI_UNF_CIPHER_CTRL_S CipherCtrl;
    HI_HANDLE  hCipher = HI_INVALID_HANDLE;
	HI_U32 u32Round = 0;
	HI_U32 u32TailLen = 0;
	HI_S32 i = 0;
	HI_U32 Offset = 0;
#ifdef LOADER
	HI_U32 u32MmzSrcPhyAddr = 0;
	HI_U32 u32MmzDestPhyAddr = 0;
	HI_U8 *pu8MmzSrcVirAddr = 0;
	HI_U8 *pu8MmzDestVirAddr = 0;
#endif

	if( (NULL == input) || (NULL == output) || (0 == length) || (0xFFFFFFFF == length) || (0 != (length % 16)) )
	{
		VMX_PRINT("Invalid parameters, length: 0x%x\n", length);
		return HI_FAILURE;
	}

    VMX_ASSERT(HI_UNF_ADVCA_Init(), ret);
    VMX_ASSERT(VMX3RD_ADP_R2RCipherInit(), ret);
	/* open keyladder channel and configure keyladder session keys */
	memset(&stKLAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
	stKLAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_ENABLE;
	stKLAttr.unKlAttr.stMiscKlAttr.bEnable = HI_TRUE;
	VMX_ASSERT(HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKLAttr), ret);

	memset(&stKLAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
	stKLAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_ALG;
	stKLAttr.unKlAttr.stMiscKlAttr.enAlgType = HI_UNF_ADVCA_ALG_TYPE_AES;
	VMX_ASSERT(HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKLAttr), ret);

    /* Set Session Key */
	memset(&stKLAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
	stKLAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_SESSION_KEY;
	stKLAttr.unKlAttr.stMiscKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV1;
	memcpy(stKLAttr.unKlAttr.stMiscKlAttr.u8SessionKey, g_u8MiscInput1, 16);
	VMX_ASSERT(HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKLAttr), ret);

	memset(&stKLAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
	stKLAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_SESSION_KEY;
	stKLAttr.unKlAttr.stMiscKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV2;
	memcpy(stKLAttr.unKlAttr.stMiscKlAttr.u8SessionKey, g_u8MiscInput2, 16);
	VMX_ASSERT(HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKLAttr), ret);

	memset(&stKLAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
	stKLAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_SESSION_KEY;
	stKLAttr.unKlAttr.stMiscKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV3;
	memcpy(stKLAttr.unKlAttr.stMiscKlAttr.u8SessionKey, g_u8MiscInput3, 16);
	VMX_ASSERT(HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKLAttr), ret);

	memset(&stKLAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
	stKLAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_SESSION_KEY;
	stKLAttr.unKlAttr.stMiscKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV4;
	memcpy(stKLAttr.unKlAttr.stMiscKlAttr.u8SessionKey, g_u8MiscInput4, 16);
	VMX_ASSERT(HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKLAttr), ret);

    hCipher = VMX3RD_ADP_GetMISCCipherHandle();
    if (HI_INVALID_HANDLE == hCipher)
    {
        VMX_PRINT("GetMiscCipherHandle failed\n");
        return HI_FAILURE;
    }

    /* Use cakey, and AES-CBC alg */
    memset(&CipherCtrl, 0, sizeof(CipherCtrl));
    CipherCtrl.bKeyByCA = HI_TRUE;
    CipherCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
    CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
    CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
    CipherCtrl.stChangeFlags.bit1IV = HI_TRUE;

    memcpy(CipherCtrl.u32Key, g_u8MiscInput5,16);
    memcpy(CipherCtrl.u32IV, g_u8MiscInput6, 16);

#ifdef LOADER
	u32MmzSrcPhyAddr = (HI_U32)HI_MMZ_New(CIPHER_1M_16, 4, NULL, "src_kl_r2r");
	if(0 == u32MmzSrcPhyAddr)
	{
		VMX_PRINT("mmz new for src_kl failed, u32MmzSrcPhyAddr:0x%x\n", u32MmzSrcPhyAddr);
		return HI_FAILURE;
	}

	pu8MmzSrcVirAddr = (HI_U8 *)HI_MMZ_Map(u32MmzSrcPhyAddr, 0);
	if(NULL == pu8MmzSrcVirAddr)
	{
		VMX_PRINT("mmz map for src_kl failed\n");
		return HI_FAILURE;
	}

	u32MmzDestPhyAddr = (HI_U32)HI_MMZ_New(CIPHER_1M_16, 4, NULL, "dest_kl_r2r");
	if(0 == u32MmzDestPhyAddr)
	{
		VMX_PRINT("mmz new for dest_kl failed, u32MmzDestPhyAddr:0x%x\n", u32MmzDestPhyAddr);
		return HI_FAILURE;
	}

	pu8MmzDestVirAddr = (HI_U8 *)HI_MMZ_Map(u32MmzDestPhyAddr, 0);
	if(NULL == pu8MmzDestVirAddr)
	{
		VMX_PRINT("mmz map for dest_kl failed\n");
		return HI_FAILURE;
	}
#endif

    VMX_ASSERT(HI_UNF_CIPHER_ConfigHandle(hCipher, &CipherCtrl),ret);

	u32Round = length / CIPHER_1M_16;
	u32TailLen = length % CIPHER_1M_16;

	Offset = 0;
	for(i = 0; i < u32Round; i++)
	{
	    if(bR2REncDec == 0) //encryption (0) or decryption (1)
	    {
#ifndef LOADER
	        VMX_ASSERT(HI_UNF_CIPHER_Encrypt(hCipher, (HI_U32)(input + Offset), (HI_U32)(output + Offset), CIPHER_1M_16),ret);
#else
			memcpy(pu8MmzSrcVirAddr, input + Offset, CIPHER_1M_16);
	        VMX_ASSERT(HI_UNF_CIPHER_Encrypt(hCipher, u32MmzSrcPhyAddr, u32MmzDestPhyAddr, CIPHER_1M_16),ret);
			memcpy(output + Offset, pu8MmzDestVirAddr, CIPHER_1M_16);
#endif
	    }
	    else
	    {
#ifndef LOADER
	        VMX_ASSERT(HI_UNF_CIPHER_Decrypt(hCipher, (HI_U32)(input + Offset), (HI_U32)(output + Offset), CIPHER_1M_16),ret);
#else
			memcpy(pu8MmzSrcVirAddr, input + Offset, CIPHER_1M_16);
	        VMX_ASSERT(HI_UNF_CIPHER_Decrypt(hCipher, u32MmzSrcPhyAddr, u32MmzDestPhyAddr, CIPHER_1M_16),ret);
			memcpy(output + Offset, pu8MmzDestVirAddr, CIPHER_1M_16);
#endif
	    }

		Offset += CIPHER_1M_16;
	}

	if(0 != u32TailLen)
	{
		Offset = length - u32TailLen;

	    if(bR2REncDec == 0) //encryption (0) or decryption (1)
	    {
#ifndef LOADER
	        VMX_ASSERT(HI_UNF_CIPHER_Encrypt(hCipher, (HI_U32)(input + Offset), (HI_U32)(output + Offset), u32TailLen),ret);
#else
			memcpy(pu8MmzSrcVirAddr, input + Offset, u32TailLen);
	        VMX_ASSERT(HI_UNF_CIPHER_Encrypt(hCipher, u32MmzSrcPhyAddr, u32MmzDestPhyAddr, u32TailLen),ret);
			memcpy(output + Offset, pu8MmzDestVirAddr, u32TailLen);
#endif
	    }
	    else
	    {
#ifndef LOADER
	        VMX_ASSERT(HI_UNF_CIPHER_Decrypt(hCipher, (HI_U32)(input + Offset), (HI_U32)(output + Offset), u32TailLen),ret);
#else
			memcpy(pu8MmzSrcVirAddr, input + Offset, u32TailLen);
	        VMX_ASSERT(HI_UNF_CIPHER_Decrypt(hCipher, u32MmzSrcPhyAddr, u32MmzDestPhyAddr, u32TailLen),ret);
			memcpy(output + Offset, pu8MmzDestVirAddr, u32TailLen);
#endif
	    }
	}

#ifdef LOADER
	(HI_VOID)HI_MMZ_Unmap(u32MmzSrcPhyAddr);
	(HI_VOID)HI_MMZ_Delete(u32MmzSrcPhyAddr);
	(HI_VOID)HI_MMZ_Unmap(u32MmzDestPhyAddr);
	(HI_VOID)HI_MMZ_Delete(u32MmzDestPhyAddr);
#endif

	/* close keyladder channel */
	memset(&stKLAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
	stKLAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_ENABLE;
	stKLAttr.unKlAttr.stMiscKlAttr.bEnable = HI_FALSE;
	VMX_ASSERT(HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKLAttr), ret);

    (HI_VOID)HI_UNF_ADVCA_DeInit();

    return HI_SUCCESS;
}



#ifdef LOADER
extern unsigned int g_u32HashBaseBufferLen;
#define VMX_HASH_MMZ_SIZE (50*1024*1024) //50M PER ROUND
#endif

int VMX3RD_ADP_VerifyRSASignature(unsigned char* signature, int sig_len, unsigned char* data, int data_len)
{
	int ret = 0;
	HI_UNF_CIPHER_RSA_VERIFY_S stRsaVerify;

	if( (NULL == signature) || (NULL == data) || (0 == sig_len) || (0xFFFFFFFF == sig_len) || (0 == data_len) || (0xFFFFFFFF == data_len) )
	{
		VMX_PRINT("Invalid parameters, NULL pointer!\n");
		return HI_FAILURE;
	}

#ifdef LOADER
	g_u32HashBaseBufferLen = VMX_HASH_MMZ_SIZE;
#endif

	ret = VMX3RD_ADP_R2RCipherInit();
	if(ret != HI_SUCCESS)
	{
		VMX_PRINT("cipher init failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	memset(&stRsaVerify, 0, sizeof(HI_UNF_CIPHER_RSA_VERIFY_S));
	stRsaVerify.enScheme = HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256;
	stRsaVerify.stPubKey.pu8N = g_au8VMX3RDRsaN;
	stRsaVerify.stPubKey.u16NLen = 256;
	stRsaVerify.stPubKey.pu8E = g_au8VMX3RDRsaE;
	stRsaVerify.stPubKey.u16ELen = 256;

	ret = HI_UNF_CIPHER_RsaVerify(&stRsaVerify, data, data_len, NULL, signature, sig_len);
	if(HI_SUCCESS != ret )
	{
		VMX_PRINT("Rsa verify failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

int VMX3RD_ADP_DecryptBySWPK(unsigned char* input, int length, unsigned char* output)
{
	HI_S32 ret = 0;
    HI_UNF_CIPHER_CTRL_S stCipherCtrl;
    HI_HANDLE hCipher = -1;
	HI_U32 u32Round = 0;
	HI_U32 u32TailLen = 0;
	HI_S32 i = 0;
	HI_U32 Offset = 0;
#ifdef LOADER
	HI_U32 u32MmzSrcPhyAddr = 0;
	HI_U32 u32MmzDestPhyAddr = 0;
	HI_U8 *pu8MmzSrcVirAddr = 0;
	HI_U8 *pu8MmzDestVirAddr = 0;
#endif

	if((NULL == input) || (NULL == output) || (0 == length) || (0xFFFFFFFF == length) || (0 != (length % 16)) )
	{
		VMX_PRINT("Invalid parameters, ldatalen: 0x%x.\n", length);
		return HI_FAILURE;
	}

    VMX_ASSERT(VMX3RD_ADP_R2RCipherInit(), ret);

#ifdef LOADER
	u32MmzSrcPhyAddr = (HI_U32)HI_MMZ_New(CIPHER_1M_16, 4, NULL, "src_cipher_r2r");
	if(0 == u32MmzSrcPhyAddr)
	{
		VMX_PRINT("mmz new for src_cipher failed, u32MmzSrcPhyAddr:0x%x\n", u32MmzSrcPhyAddr);
		return HI_FAILURE;
	}

	pu8MmzSrcVirAddr = (HI_U8 *)HI_MMZ_Map(u32MmzSrcPhyAddr, 0);
	if(NULL == pu8MmzSrcVirAddr)
	{
		VMX_PRINT("mmz map for src_cipher failed\n");
		return HI_FAILURE;
	}

	u32MmzDestPhyAddr = (HI_U32)HI_MMZ_New(CIPHER_1M_16, 4, NULL, "dest_cipher_r2r");
	if(0 == u32MmzDestPhyAddr)
	{
		VMX_PRINT("mmz new for dest_cipher failed, u32MmzDestPhyAddr:0x%x\n", u32MmzDestPhyAddr);
		return HI_FAILURE;
	}

	pu8MmzDestVirAddr = (HI_U8 *)HI_MMZ_Map(u32MmzDestPhyAddr, 0);
	if(NULL == pu8MmzDestVirAddr)
	{
		VMX_PRINT("mmz map for dest_cipher failed\n");
		return HI_FAILURE;
	}
#endif

    hCipher = VMX3RD_ADP_GetSWPKCipherHandle();
    if (HI_INVALID_HANDLE == hCipher)
    {
        VMX_PRINT("GetMiscCipherHandle failed\n");
        return HI_FAILURE;
    }

    memset(&stCipherCtrl, 0, sizeof(stCipherCtrl));
    stCipherCtrl.bKeyByCA = HI_TRUE;
    stCipherCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_BLPK;
    stCipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
    stCipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    stCipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
    stCipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
    stCipherCtrl.stChangeFlags.bit1IV = HI_TRUE;

    memcpy(stCipherCtrl.u32Key, g_u8EncryptedSWPK,16);
    memcpy(stCipherCtrl.u32IV, g_u8MiscInput6, 16);

    VMX_ASSERT(HI_UNF_CIPHER_ConfigHandle(hCipher, &stCipherCtrl), ret);

	u32Round = length / CIPHER_1M_16;
	u32TailLen = length % CIPHER_1M_16;

	Offset = 0;
	for(i = 0; i < u32Round; i++)
	{
#ifndef LOADER
	    VMX_ASSERT(HI_UNF_CIPHER_Decrypt(hCipher, (HI_U32)(input + Offset), (HI_U32)(output + Offset), CIPHER_1M_16), ret);
#else
		memcpy(pu8MmzSrcVirAddr, input + Offset, CIPHER_1M_16);
	    VMX_ASSERT(HI_UNF_CIPHER_Decrypt(hCipher, u32MmzSrcPhyAddr, u32MmzDestPhyAddr, CIPHER_1M_16), ret);
		memcpy(output + Offset, pu8MmzDestVirAddr, CIPHER_1M_16);
#endif
		Offset += CIPHER_1M_16;
	}

	if(0 != u32TailLen)
	{
		Offset = length - u32TailLen;
#ifndef LOADER
	    VMX_ASSERT(HI_UNF_CIPHER_Decrypt(hCipher, (HI_U32)(input + Offset), (HI_U32)(output + Offset), u32TailLen), ret);
#else
		memcpy(pu8MmzSrcVirAddr, input + Offset, u32TailLen);
	    VMX_ASSERT(HI_UNF_CIPHER_Decrypt(hCipher, u32MmzSrcPhyAddr, u32MmzDestPhyAddr, u32TailLen), ret);
		memcpy(output + Offset, pu8MmzDestVirAddr, u32TailLen);
#endif
	}

#ifdef LOADER
	(HI_VOID)HI_MMZ_Unmap(u32MmzSrcPhyAddr);
	(HI_VOID)HI_MMZ_Delete(u32MmzSrcPhyAddr);
	(HI_VOID)HI_MMZ_Unmap(u32MmzDestPhyAddr);
	(HI_VOID)HI_MMZ_Delete(u32MmzDestPhyAddr);
#endif
    return HI_SUCCESS;
}

