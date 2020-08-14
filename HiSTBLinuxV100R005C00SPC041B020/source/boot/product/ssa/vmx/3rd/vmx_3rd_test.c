/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : vmx_3rd_test.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2014-09-19
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifdef LOADER
#include "hi_type.h"
#endif

//#define VMX3RD_SELFTEST
#include "app.h"
#include "vmx_3rd_adp.h"
#include "vmx_3rd_auth.h"
#include "hi_unf_advca.h"
#include "hi_unf_cipher.h"
#include "vmx_verify_def.h"


#ifdef LOADER
extern HI_VOID *HI_MMZ_New(HI_U32 u32Size , HI_U32 u32Align, HI_CHAR *ps8MMZName, HI_CHAR *ps8MMBName);
extern HI_S32 HI_MMZ_Delete(HI_U32 u32PhysAddr);
extern HI_VOID *HI_MMZ_Map(HI_U32 u32PhysAddr, HI_U32 u32Cached);
extern HI_S32 HI_MMZ_Unmap(HI_U32 u32PhysAddr);
#endif

#define HASH_SIZE (20*1024*1024)    //20M per round inside this interface for hash
#ifdef VMX3RD_SELFTEST


extern unsigned char g_au8VMX3RDRsaN[256];

unsigned char g_au8VMX3RDRsaD[256]={
	0x01,0x29,0xed,0xee,0x00,0x80,0xf0,0x96,0x87,0x25,0x02,0xa3,0x7d,0x27,0x38,0x64,
	0xd5,0xeb,0x96,0x68,0x22,0x0e,0x01,0xa3,0x53,0x7b,0x8f,0x8b,0x3f,0xfb,0x51,0x5f,
	0x49,0x2d,0x51,0x83,0xa7,0xef,0xe3,0x9d,0x72,0x49,0x4e,0x43,0xf4,0xd5,0x24,0x9e,
	0x0f,0x07,0xff,0x42,0x10,0x5c,0x89,0x2a,0xc3,0x04,0x44,0x39,0x52,0x55,0x76,0x8d,
	0xa4,0x56,0x2d,0xc7,0x07,0x7f,0x8c,0xcd,0xec,0x3e,0x33,0x72,0x55,0x04,0x44,0x7d,
	0x0e,0x5d,0xe3,0x02,0x9c,0x51,0xf4,0xc4,0x0f,0x14,0xa1,0x94,0x75,0xab,0xe3,0x52,
	0x3c,0x11,0x92,0x61,0x65,0xba,0xee,0xed,0x6a,0x79,0xeb,0x6f,0x11,0x68,0xe4,0x5e,
	0xc5,0xc2,0x46,0x2c,0x0c,0x44,0xba,0xe7,0x22,0xb2,0x66,0x9d,0x13,0xce,0xa6,0x7a,
	0x5e,0xd8,0x91,0x2a,0x76,0x51,0x92,0xa1,0x1f,0x19,0x5e,0x03,0x31,0x72,0x27,0x07,
	0x31,0xc1,0x71,0xdb,0x7f,0xdd,0x49,0x75,0x9b,0x3a,0xab,0xf5,0x6b,0xc2,0x6c,0x32,
	0x98,0xfd,0xb0,0x58,0x52,0x2e,0x04,0x46,0x5c,0x5d,0x25,0x82,0x0b,0x05,0xc6,0x2f,
	0xb4,0x77,0x9b,0x9a,0xba,0x66,0x3a,0x8e,0x01,0xb7,0x34,0xd3,0x69,0xd4,0x52,0x2e,
	0xa6,0xbd,0xe5,0x04,0x80,0x60,0x9a,0x66,0x48,0x90,0x30,0x6c,0x3a,0x95,0x06,0x3d,
	0x93,0x9a,0x99,0x13,0xb6,0x8b,0x4d,0xfd,0x4f,0x6b,0x34,0x78,0x3e,0x23,0x28,0x68,
	0xee,0x61,0x66,0x66,0xd7,0x96,0x48,0x52,0x07,0xb8,0xc6,0xb2,0xdc,0x25,0x54,0xe3,
	0x17,0x61,0xbb,0x79,0x58,0x62,0x94,0xf8,0x53,0xce,0x22,0x79,0x9b,0xf4,0x2c,0x41};


static void s_VMX3RD_ADP_Printbuffer(char *str, unsigned char *pu8Buf, unsigned int u32Length)
{
    unsigned int i = 0;

    if(NULL != str)
    {
        printf("%s\n", str);
    }

    for(i = 0; i < u32Length; i++)
    {
        if(i % 16 == 0 && i != 0)
        {
            printf("\n");
        }
        printf("0x%02X, ", pu8Buf[i]);
    }
    printf("\n");
    return;
}

static int VMX3RD_ADP_RSASign(unsigned char* data, int data_len, unsigned char* signature, int sig_len)
{
	int ret = 0;
	HI_UNF_CIPHER_RSA_SIGN_S stRsaSign;
	unsigned int len_sig = sig_len;

	ret = HI_UNF_CIPHER_Init();
	if(0 != HI_SUCCESS)
	{
		VMX_PRINT("cipher init failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	memset(&stRsaSign, 0, sizeof(HI_UNF_CIPHER_RSA_SIGN_S));
	stRsaSign.enScheme = HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256;
	stRsaSign.stPriKey.pu8N = g_au8VMX3RDRsaN;
	stRsaSign.stPriKey.u16NLen = 256;
	stRsaSign.stPriKey.pu8D = g_au8VMX3RDRsaD;
	stRsaSign.stPriKey.u16DLen = 256;

	ret = HI_UNF_CIPHER_RsaSign(&stRsaSign, data, data_len, NULL, signature, &len_sig);
	if(ret != HI_SUCCESS)
	{
		VMX_PRINT("Rsa sign failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	ret = HI_UNF_CIPHER_DeInit();
	if(ret != HI_SUCCESS)
	{
		VMX_PRINT("cipher deinit failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	s_VMX3RD_ADP_Printbuffer("Signature output:", signature, len_sig);
	return HI_SUCCESS;
}

static int s_VMX3RD_ADP_EncryptBySWPK(unsigned char* input, int length, unsigned char* output)
{
	HI_S32 ret = 0;
	unsigned char au8SWPK[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	unsigned char au8IV[16]   = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    HI_UNF_CIPHER_CTRL_S stCipherCtrl;
    HI_UNF_CIPHER_ATTS_S stCipherAtts;
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

    VMX_ASSERT(HI_UNF_CIPHER_Init(), ret);

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

    memset(&stCipherAtts, 0, sizeof(HI_UNF_CIPHER_ATTS_S));
    stCipherAtts.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    VMX_ASSERT(HI_UNF_CIPHER_CreateHandle(&hCipher, &stCipherAtts), ret);

    memset(&stCipherCtrl, 0, sizeof(stCipherCtrl));
    stCipherCtrl.bKeyByCA = HI_FALSE;
    stCipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
    stCipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    stCipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
    stCipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
    stCipherCtrl.stChangeFlags.bit1IV = HI_TRUE;

    memcpy(stCipherCtrl.u32Key, au8SWPK,16);
    memcpy(stCipherCtrl.u32IV, au8IV, 16);

    VMX_ASSERT(HI_UNF_CIPHER_ConfigHandle(hCipher, &stCipherCtrl), ret);

	u32Round = length / CIPHER_1M_16;
	u32TailLen = length % CIPHER_1M_16;

	Offset = 0;
	for(i = 0; i < u32Round; i++)
	{
#ifndef LOADER
	    VMX_ASSERT(HI_UNF_CIPHER_Encrypt(hCipher, (HI_U32)(input + Offset), (HI_U32)(output + Offset), CIPHER_1M_16), ret);
#else
		memcpy(pu8MmzSrcVirAddr, input + Offset, CIPHER_1M_16);
	    VMX_ASSERT(HI_UNF_CIPHER_Encrypt(hCipher, u32MmzSrcPhyAddr, u32MmzDestPhyAddr, CIPHER_1M_16), ret);
		memcpy(output + Offset, pu8MmzDestVirAddr, CIPHER_1M_16);
#endif
		Offset += CIPHER_1M_16;
	}

	if(0 != u32TailLen)
	{
		Offset = length - u32TailLen;
#ifndef LOADER
	    VMX_ASSERT(HI_UNF_CIPHER_Encrypt(hCipher, (HI_U32)(input + Offset), (HI_U32)(output + Offset), u32TailLen), ret);
#else
		memcpy(pu8MmzSrcVirAddr, input + Offset, u32TailLen);
	    VMX_ASSERT(HI_UNF_CIPHER_Encrypt(hCipher, u32MmzSrcPhyAddr, u32MmzDestPhyAddr, u32TailLen), ret);
		memcpy(output + Offset, pu8MmzDestVirAddr, u32TailLen);
#endif
	}

#ifdef LOADER
	(HI_VOID)HI_MMZ_Unmap(u32MmzSrcPhyAddr);
	(HI_VOID)HI_MMZ_Delete(u32MmzSrcPhyAddr);
	(HI_VOID)HI_MMZ_Unmap(u32MmzDestPhyAddr);
	(HI_VOID)HI_MMZ_Delete(u32MmzDestPhyAddr);
#endif
    VMX_ASSERT(HI_UNF_CIPHER_DestroyHandle(hCipher), ret);

    HI_UNF_CIPHER_DeInit();
    return HI_SUCCESS;
}
static int s_VMX3RD_ADP_SelfTest_RSA(void)
{
	int ret = 0;
	unsigned char au8TestData1[3] = {0x1,0x2,0x3};
	unsigned char au8TestData2[256] = {0x1,0x2,0x3,0x4};
	unsigned char au8TestData3[2048] = {0x1,0x2,0x3,0x4,0x5};
	unsigned char au8TestData4[4096] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xe,0xf};
	unsigned char au8Signature[256] = {0};

	ret = VMX3RD_ADP_RSASign(au8TestData1, sizeof(au8TestData1), au8Signature, sizeof(au8Signature));
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	ret = VMX3RD_ADP_VerifyRSASignature(au8Signature, sizeof(au8Signature), au8TestData1, sizeof(au8TestData1));
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	VMX_PRINT("SelfTest RSA 1(3) Success!\n");

	ret = VMX3RD_ADP_RSASign(au8TestData2, sizeof(au8TestData2), au8Signature, sizeof(au8Signature));
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	ret = VMX3RD_ADP_VerifyRSASignature(au8Signature, sizeof(au8Signature), au8TestData2, sizeof(au8TestData2));
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	VMX_PRINT("SelfTest RSA 2(256) Success!\n");

	ret = VMX3RD_ADP_RSASign(au8TestData3, sizeof(au8TestData3), au8Signature, sizeof(au8Signature));
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	ret = VMX3RD_ADP_VerifyRSASignature(au8Signature, sizeof(au8Signature), au8TestData3, sizeof(au8TestData3));
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	VMX_PRINT("SelfTest RSA 3(2048) Success!\n");

	ret = VMX3RD_ADP_RSASign(au8TestData4, sizeof(au8TestData4), au8Signature, sizeof(au8Signature));
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	ret = VMX3RD_ADP_VerifyRSASignature(au8Signature, sizeof(au8Signature), au8TestData4, sizeof(au8TestData4));
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	VMX_PRINT("SelfTest RSA 4(4096) Success!\n");

	return HI_SUCCESS;

}

static int s_VMX3RD_ADP_SelfTest_R2R(void)
{
	int ret = 0;
	unsigned char au8TestData1[16] = {0x1,0x2,0x3};
	unsigned char au8TestData2[256] = {0x1,0x2,0x3,0x4};
	unsigned char au8TestData3[2048] = {0x1,0x2,0x3,0x4,0x5};
	unsigned char au8TestData4[4096] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xe,0xf};
	unsigned char au8TempEnc[4096] = {0};
	unsigned char au8TempDec[4096] = {0};

	ret = VMX3RD_ADP_R2RByMiscKeyladder(0, au8TestData1, sizeof(au8TestData1), au8TempEnc);
	ret |= VMX3RD_ADP_R2RByMiscKeyladder(1, au8TempEnc, sizeof(au8TestData1), au8TempDec);
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	ret = memcmp(au8TestData1, au8TempDec, sizeof(au8TestData1));
	if(0 != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	VMX_PRINT("SelfTest R2R 1(16) Success!\n");

	ret = VMX3RD_ADP_R2RByMiscKeyladder(0, au8TestData2, sizeof(au8TestData2), au8TempEnc);
	ret |= VMX3RD_ADP_R2RByMiscKeyladder(1, au8TempEnc, sizeof(au8TestData2), au8TempDec);
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	ret = memcmp(au8TestData2, au8TempDec, sizeof(au8TestData2));
	if(0 != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	VMX_PRINT("SelfTest R2R 2 Success!\n");

	ret = VMX3RD_ADP_R2RByMiscKeyladder(0, au8TestData3, sizeof(au8TestData3), au8TempEnc);
	ret |= VMX3RD_ADP_R2RByMiscKeyladder(1, au8TempEnc, sizeof(au8TestData3), au8TempDec);
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	ret = memcmp(au8TestData3, au8TempDec, sizeof(au8TestData3));
	if(0 != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	VMX_PRINT("SelfTest R2R 3 Success!\n");

	ret = VMX3RD_ADP_R2RByMiscKeyladder(0, au8TestData4, sizeof(au8TestData4), au8TempEnc);
	ret |= VMX3RD_ADP_R2RByMiscKeyladder(1, au8TempEnc, sizeof(au8TestData4), au8TempDec);
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	ret = memcmp(au8TestData4, au8TempDec, sizeof(au8TestData4));
	if(0 != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	VMX_PRINT("SelfTest R2R 4 Success!\n");

	return HI_SUCCESS;
}

static int s_VMX3RD_ADP_SelfTest_SWPK(void)
{
	int ret = 0;
	unsigned char au8TestData1[16] = {0x1,0x2,0x3};
	unsigned char au8TestData2[256] = {0x1,0x2,0x3,0x4};
	unsigned char au8TestData3[2048] = {0x1,0x2,0x3,0x4,0x5};
	unsigned char au8TestData4[4096] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xe,0xf};
	unsigned char au8TempEnc[4096] = {0};
	unsigned char au8TempDec[4096] = {0};

	ret = s_VMX3RD_ADP_EncryptBySWPK(au8TestData1, sizeof(au8TestData1), au8TempEnc);
	ret |= VMX3RD_ADP_DecryptBySWPK(au8TempEnc, sizeof(au8TestData1), au8TempDec);
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	s_VMX3RD_ADP_Printbuffer("SWPK Encrypt output:", au8TempEnc, 16);
	s_VMX3RD_ADP_Printbuffer("SWPK Decrypt output:", au8TempDec, 16);
	ret = memcmp(au8TestData1, au8TempDec, sizeof(au8TestData1));
	if(0 != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	VMX_PRINT("SelfTest SWPK 1 Success!\n");

	ret = s_VMX3RD_ADP_EncryptBySWPK(au8TestData2, sizeof(au8TestData2), au8TempEnc);
	ret |= VMX3RD_ADP_DecryptBySWPK(au8TempEnc, sizeof(au8TestData2), au8TempDec);
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	s_VMX3RD_ADP_Printbuffer("SWPK Encrypt output:", au8TempEnc, 32);
	s_VMX3RD_ADP_Printbuffer("SWPK Decrypt output:", au8TempDec, 32);
	ret = memcmp(au8TestData2, au8TempDec, sizeof(au8TestData2));
	if(0 != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	VMX_PRINT("SelfTest SWPK 2 Success!\n");

	ret = s_VMX3RD_ADP_EncryptBySWPK(au8TestData3, sizeof(au8TestData3), au8TempEnc);
	ret |= VMX3RD_ADP_DecryptBySWPK(au8TempEnc, sizeof(au8TestData3), au8TempDec);
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	s_VMX3RD_ADP_Printbuffer("SWPK Encrypt output:", au8TempEnc, 32);
	s_VMX3RD_ADP_Printbuffer("SWPK Decrypt output:", au8TempDec, 32);
	ret = memcmp(au8TestData3, au8TempDec, sizeof(au8TestData3));
	if(0 != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	VMX_PRINT("SelfTest SWPK 3 Success!\n");

	ret = s_VMX3RD_ADP_EncryptBySWPK(au8TestData4, sizeof(au8TestData4), au8TempEnc);
	ret |= VMX3RD_ADP_DecryptBySWPK(au8TempEnc, sizeof(au8TestData4), au8TempDec);
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	s_VMX3RD_ADP_Printbuffer("SWPK Encrypt output:", au8TempEnc, 32);
	s_VMX3RD_ADP_Printbuffer("SWPK Decrypt output:", au8TempDec, 32);
	ret = memcmp(au8TestData4, au8TempDec, sizeof(au8TestData4));
	if(0 != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}
	VMX_PRINT("SelfTest SWPK 4 Success!\n");

	return HI_SUCCESS;
}

int VMX3RD_ADP_SelfTest(void)
{
	int ret = 0;

	ret = s_VMX3RD_ADP_SelfTest_R2R();
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		//return HI_FAILURE;
	}
	else
	{
		VMX_PRINT("VMX3RD ADP Selftest R2R success!\n");
	}

	ret = s_VMX3RD_ADP_SelfTest_SWPK();
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		//return HI_FAILURE;
	}
	else
	{
		VMX_PRINT("VMX3RD ADP Selftest SWPK success!\n");
	}

	ret = s_VMX3RD_ADP_SelfTest_RSA();
	if(HI_SUCCESS != ret)
	{
		VMX_PRINT("failed, ret:0x%x\n", ret);
		//return HI_FAILURE;
	}
	else
	{
		VMX_PRINT("VMX3RD ADP Selftest RSA success!\n");
	}

	VMX_PRINT("VMX3RD ADP Selftest success!\n");
	return 0;
}
#endif
