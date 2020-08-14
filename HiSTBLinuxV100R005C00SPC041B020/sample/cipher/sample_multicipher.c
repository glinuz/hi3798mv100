/******************************************************************************
Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_rng.c
Version       : Initial Draft
Author        : Hisilicon
Created       : 2012/07/10
Last Modified :
Description   : sample for multicipher
Function List :
History       :
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_type.h"
#include "hi_unf_cipher.h"
#include "hi_common.h"
#include "hi_adp.h"

#ifdef  CONFIG_SUPPORT_CA_RELEASE
#define HI_ERR_CIPHER(format, arg...)
#define HI_INFO_CIPHER(format, arg...)   printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#else
#define HI_ERR_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_CIPHER(format, arg...)   printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#endif
#define U32_TO_POINT(addr)  ((HI_VOID*)((HI_SIZE_T)(addr)))
#define POINT_TO_U32(addr)  ((HI_U32)((HI_SIZE_T)(addr)))


static const HI_U8 des_key[32] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
static const HI_U8 des_IV[32] = {0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
static const HI_U8 des_ecb_enc_src_buf[16] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
static const HI_U8 des_ecb_enc_dst_buf[16] = {0x17, 0x66, 0x8D, 0xFC, 0x72, 0x92, 0x53, 0x2D};
static const HI_U8 des_ecb_dec_src_buf[16] = {0x17, 0x66, 0x8D, 0xFC, 0x72, 0x92, 0x53, 0x2D};
static const HI_U8 des_ecb_dec_dst_buf[16] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
static HI_U8 aes_key[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};
static HI_U8 aes_IV[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
static HI_U8 aes_cbc_enc_src_buf[16] = {0x6B,0xC1,0xBE,0xE2,0x2E,0x40,0x9F,0x96,0xE9,0x3D,0x7E,0x11,0x73,0x93,0x17,0x2A};
static HI_U8 aes_cbc_enc_dst_buf[16] = {0x76,0x49,0xAB,0xAC,0x81,0x19,0xB2,0x46,0xCE,0xE9,0x8E,0x9B,0x12,0xE9,0x19,0x7D};
static HI_U8 aes_cbc_dec_src_buf[16] = {0x76,0x49,0xAB,0xAC,0x81,0x19,0xB2,0x46,0xCE,0xE9,0x8E,0x9B,0x12,0xE9,0x19,0x7D};
static HI_U8 aes_cbc_dec_dst_buf[16] = {0x6B,0xC1,0xBE,0xE2,0x2E,0x40,0x9F,0x96,0xE9,0x3D,0x7E,0x11,0x73,0x93,0x17,0x2A};

static HI_S32 printBuffer(const HI_CHAR *string, const HI_U8 *pu8Input, HI_U32 u32Length)
{
    HI_U32 i = 0;
    
    if ( NULL != string )
    {
        printf("%s\n", string);
    }

    for ( i = 0 ; i < u32Length; i++ )
    {
        if( (i % 16 == 0) && (i != 0)) printf("\n");
        printf("0x%02x ", pu8Input[i]);
    }
    printf("\n");

    return HI_SUCCESS;
}

HI_S32 Setconfiginfo(HI_HANDLE chnHandle,
                        HI_BOOL isKeyByCA,
                        HI_UNF_CIPHER_ALG_E alg,
                        HI_UNF_CIPHER_WORK_MODE_E mode,
                        HI_UNF_CIPHER_KEY_LENGTH_E keyLen,
                        const HI_U8 u8KeyBuf[16],
                        const HI_U8 u8IVBuf[16])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_CIPHER_CTRL_S CipherCtrl;

    memset(&CipherCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    CipherCtrl.bKeyByCA = isKeyByCA;
    CipherCtrl.enAlg = alg;
    CipherCtrl.enWorkMode = mode;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_64BIT;
    CipherCtrl.enKeyLen = keyLen;
    if(CipherCtrl.enWorkMode != HI_UNF_CIPHER_WORK_MODE_ECB)
    {
        CipherCtrl.stChangeFlags.bit1IV = 1;  //must set for CBC , CFB mode
        memcpy(CipherCtrl.u32IV, u8IVBuf, 16);
    }

    if ( HI_FALSE == isKeyByCA )
    {
        memcpy(CipherCtrl.u32Key, u8KeyBuf, 32);
    }

    s32Ret = HI_UNF_CIPHER_ConfigHandle(chnHandle, &CipherCtrl);
    if(HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 multiCipher_DES_ECB_ENC()
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 8;
    HI_U32 u32Testcached = 0;

    HI_U8 *pInputAddrVir[3];
    HI_U8 *pOutputAddrVir[3];

    HI_HANDLE hTestchnid = 0;

    HI_UNF_CIPHER_DATA_S cipherDataArray[3];
    HI_U32 i;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;

    (HI_VOID)HI_SYS_Init();

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher init failed.\n");
        (HI_VOID)HI_SYS_DeInit();
        return HI_FAILURE;
    }
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid, &stCipherAttr);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher create handle failed.\n");
        goto __CIPHER_EXIT__;
    }

    for(i = 0; i < 3; i++)
    {
        cipherDataArray[i].u32SrcPhyAddr = POINT_TO_U32(HI_MMZ_New(u32TestDataLen, 0, NULL, "CIPHER_BufIn"));
        if (0 == cipherDataArray[i].u32SrcPhyAddr)
        {
            HI_ERR_CIPHER("Error: Get phyaddr for input failed!\n");
            s32Ret = HI_FAILURE;
            goto __CIPHER_EXIT__;
        }
        pInputAddrVir[i] = HI_MMZ_Map(cipherDataArray[i].u32SrcPhyAddr, u32Testcached);

        cipherDataArray[i].u32DestPhyAddr = POINT_TO_U32(HI_MMZ_New(u32TestDataLen, 0, NULL, "CIPHER_BufOut"));
        if (0 == cipherDataArray[i].u32DestPhyAddr)
        {
            HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
            s32Ret = HI_FAILURE;
            goto __CIPHER_EXIT__;
        }
        pOutputAddrVir[i] = HI_MMZ_Map(cipherDataArray[i].u32DestPhyAddr, u32Testcached);

        cipherDataArray[i].u32ByteLength = u32TestDataLen;
        memset(pInputAddrVir[i], 0x0, u32TestDataLen);
        memcpy(pInputAddrVir[i], des_ecb_enc_src_buf, u32TestDataLen);
        memset(pOutputAddrVir[i], 0x0, u32TestDataLen);
    }

    s32Ret = Setconfiginfo(hTestchnid,
                                    HI_FALSE,
                                    HI_UNF_CIPHER_ALG_DES,
                                    HI_UNF_CIPHER_WORK_MODE_ECB,
                                    HI_UNF_CIPHER_KEY_DES_2KEY,
                                    des_key,
                                    des_IV);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher setconfiginfo failed.\n");
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_EncryptMulti(hTestchnid, cipherDataArray, 3);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher encryptMulti failed.\n");
        goto __CIPHER_EXIT__;
    }

    for(i = 0; i < 3; i++)
    {
        if (0 != memcmp(pOutputAddrVir[i], des_ecb_enc_dst_buf, u32TestDataLen))
        {
            HI_ERR_CIPHER("MultiCipher DES ECB Encryption run failed!\n");
            s32Ret = HI_FAILURE;
            goto __CIPHER_EXIT__;
        }
    }

    HI_INFO_CIPHER("MultiCipher DES ECB Encryption run success!\n");

__CIPHER_EXIT__:

    for(i = 0; i < 3; i++)
    {
        HI_MMZ_Unmap(cipherDataArray[i].u32SrcPhyAddr);
        HI_MMZ_Delete(cipherDataArray[i].u32SrcPhyAddr);

        HI_MMZ_Unmap(cipherDataArray[i].u32DestPhyAddr);
        HI_MMZ_Delete(cipherDataArray[i].u32DestPhyAddr);
    }

    HI_UNF_CIPHER_DestroyHandle(hTestchnid);
    HI_UNF_CIPHER_DeInit();
    (HI_VOID)HI_SYS_DeInit();

    return s32Ret;
}

HI_S32 multiCipher_DES_ECB_DEC()
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 8;
    HI_U32 u32Testcached = 0;

    HI_U8 *pInputAddrVir[3];
    HI_U8 *pOutputAddrVir[3];

    HI_HANDLE hTestchnid = 0;

    HI_UNF_CIPHER_DATA_S cipherDataArray[3];
    HI_U32 i;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;

    (HI_VOID)HI_SYS_Init();
    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher Init failed.\n");
        (HI_VOID)HI_SYS_DeInit();
        return HI_FAILURE;
    }

    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid, &stCipherAttr);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher Create handle failed.\n");
        goto __CIPHER_EXIT__;
    }


    for(i = 0; i < 3; i++)
    {
        cipherDataArray[i].u32SrcPhyAddr = POINT_TO_U32(HI_MMZ_New(u32TestDataLen, 0, NULL, "cipherIn"));
        if (0 == cipherDataArray[i].u32SrcPhyAddr)
        {
            HI_ERR_CIPHER("Error: Get phyaddr for input failed!\n");
            s32Ret = HI_FAILURE;
            goto __CIPHER_EXIT__;
        }
        pInputAddrVir[i] = HI_MMZ_Map(cipherDataArray[i].u32SrcPhyAddr, u32Testcached);

        cipherDataArray[i].u32DestPhyAddr = POINT_TO_U32(HI_MMZ_New(u32TestDataLen, 0, NULL, "cipherOut"));
        if (0 == cipherDataArray[i].u32DestPhyAddr)
        {
            HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
            s32Ret = HI_FAILURE;
            goto __CIPHER_EXIT__;
        }
        pOutputAddrVir[i] = HI_MMZ_Map(cipherDataArray[i].u32DestPhyAddr, u32Testcached);

        cipherDataArray[i].u32ByteLength = u32TestDataLen;
        memset(pInputAddrVir[i], 0x0, u32TestDataLen);
        memcpy(pInputAddrVir[i], des_ecb_dec_src_buf, u32TestDataLen);
        memset(pOutputAddrVir[i], 0x0, u32TestDataLen);
    }

    s32Ret = Setconfiginfo(hTestchnid,
                                    HI_FALSE,
                                    HI_UNF_CIPHER_ALG_DES,
                                    HI_UNF_CIPHER_WORK_MODE_ECB,
                                    HI_UNF_CIPHER_KEY_DES_2KEY,
                                    des_key, des_IV);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher Setconfiginfo failed.\n");
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_DecryptMulti(hTestchnid, cipherDataArray, 3);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher DecryptMulti failed.\n");
        goto __CIPHER_EXIT__;
    }

    for(i = 0; i < 3; i++)
    {
        if (0 != memcmp(pOutputAddrVir[i], des_ecb_dec_dst_buf, u32TestDataLen))
        {
            HI_ERR_CIPHER("MultiCipher DES ECB Decryption run failed!\n");
            s32Ret = HI_FAILURE;
            goto __CIPHER_EXIT__;
        }
    }

    HI_INFO_CIPHER("MultiCipher DES ECB Decryption run success!\n");

__CIPHER_EXIT__:

    for(i = 0; i < 3; i++)
    {
        HI_MMZ_Unmap(cipherDataArray[i].u32SrcPhyAddr);
        HI_MMZ_Delete(cipherDataArray[i].u32SrcPhyAddr);

        HI_MMZ_Unmap(cipherDataArray[i].u32DestPhyAddr);
        HI_MMZ_Delete(cipherDataArray[i].u32DestPhyAddr);
    }

    HI_UNF_CIPHER_DestroyHandle(hTestchnid);
    HI_UNF_CIPHER_DeInit();
    (HI_VOID)HI_SYS_DeInit();

    return s32Ret;
}

HI_S32 multiCipher_ex_AES_CBC_ENC()
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 16;
    HI_U32 u32Testcached = 0;
    HI_U8 *pInputAddrVir[3] = {0};
    HI_U8 *pOutputAddrVir[3] = {0};
    HI_HANDLE hTestchnid = 0;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;
    HI_UNF_CIPHER_DATA_S cipherDataArray[3];
    HI_U32 i;
    
    memset(&cipherDataArray, 0, sizeof(cipherDataArray));

    (HI_VOID)HI_SYS_Init();

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher init failed.\n");
        (HI_VOID)HI_SYS_DeInit();
        return HI_FAILURE;
    }
    
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid, &stCipherAttr);
    if(HI_SUCCESS != s32Ret)
    {
    	HI_ERR_CIPHER("Cipher create handle failed.\n");
    	goto __CIPHER_EXIT__;
    }

    for(i = 0; i < 3; i++)
    {
        cipherDataArray[i].u32SrcPhyAddr = POINT_TO_U32(HI_MMZ_New(u32TestDataLen, 0, NULL, "CIPHER_BufIn"));
        if (0 == cipherDataArray[i].u32SrcPhyAddr)
        {
            HI_ERR_CIPHER("Error: Get phyaddr for input failed!\n");
            s32Ret = HI_FAILURE;
            goto __CIPHER_EXIT__;
        }
        pInputAddrVir[i] = HI_MMZ_Map(cipherDataArray[i].u32SrcPhyAddr, u32Testcached);

        cipherDataArray[i].u32DestPhyAddr = POINT_TO_U32(HI_MMZ_New(u32TestDataLen, 0, NULL, "CIPHER_BufOut"));
        if (0 == cipherDataArray[i].u32DestPhyAddr)
        {
            HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
            s32Ret = HI_FAILURE;
            goto __CIPHER_EXIT__;
        }
        pOutputAddrVir[i] = HI_MMZ_Map(cipherDataArray[i].u32DestPhyAddr, u32Testcached);

        cipherDataArray[i].u32ByteLength = u32TestDataLen;
        memset(pInputAddrVir[i], 0x0, u32TestDataLen);
        memcpy(pInputAddrVir[i], aes_cbc_enc_src_buf, u32TestDataLen);        
        memset(pOutputAddrVir[i], 0x0, u32TestDataLen);
    }    

    s32Ret = Setconfiginfo(hTestchnid,
                            HI_FALSE,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CBC,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key,
                            aes_IV);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher setconfiginfo failed.\n");
        goto __CIPHER_EXIT__;
    }

    s32Ret = HI_UNF_CIPHER_EncryptMulti(hTestchnid, cipherDataArray, 3);
    if(HI_SUCCESS != s32Ret)
    {
    	HI_ERR_CIPHER("Cipher encryptMulti failed.\n");
    	goto __CIPHER_EXIT__;
    }

    for(i = 0; i < 3; i++)
    {
        if (0 != memcmp(pOutputAddrVir[i], aes_cbc_enc_dst_buf, u32TestDataLen))
        {
            HI_ERR_CIPHER("MultiCipher AES CBC Encryption run failed!\n");
            printBuffer("ENC", pOutputAddrVir[i], u32TestDataLen);
            printBuffer("GOLD", aes_cbc_enc_dst_buf, u32TestDataLen);
            s32Ret = HI_FAILURE;
            goto __CIPHER_EXIT__;
        }
    }

    HI_INFO_CIPHER("MultiCipher AES CBC Encryption run success!\n");

__CIPHER_EXIT__:

    for(i = 0; i < 3; i++)
    {
        if(cipherDataArray[i].u32SrcPhyAddr > 0)
        {
            HI_MMZ_Unmap(cipherDataArray[i].u32SrcPhyAddr);
            HI_MMZ_Delete(cipherDataArray[i].u32SrcPhyAddr);
        }
        if(cipherDataArray[i].u32DestPhyAddr > 0)
        {
            HI_MMZ_Unmap(cipherDataArray[i].u32DestPhyAddr);
            HI_MMZ_Delete(cipherDataArray[i].u32DestPhyAddr);
        }
    }

    HI_UNF_CIPHER_DestroyHandle(hTestchnid);
    HI_UNF_CIPHER_DeInit();
    
    return s32Ret;
}

HI_S32 multiCipher_ex_AES_CBC_DEC()
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 16;
    HI_U32 u32Testcached = 0;
    HI_U8 *pInputAddrVir[3] = {0};    
    HI_U8 *pOutputAddrVir[3] = {0};
    HI_HANDLE hTestchnid = 0;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;
    HI_UNF_CIPHER_DATA_S cipherDataArray[3];
    HI_U32 i;
    
    memset(&cipherDataArray, 0, sizeof(cipherDataArray));

    (HI_VOID)HI_SYS_Init();

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher init failed.\n");
        (HI_VOID)HI_SYS_DeInit();
        return HI_FAILURE;
    }
    
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid, &stCipherAttr);
    if(HI_SUCCESS != s32Ret)
    {
    	HI_ERR_CIPHER("Cipher Create handle failed.\n");
    	goto __CIPHER_EXIT__;
    }

    for(i = 0; i < 3; i++)
    {
        cipherDataArray[i].u32SrcPhyAddr = POINT_TO_U32(HI_MMZ_New(u32TestDataLen, 0, NULL, "cipherIn"));
        if (0 == cipherDataArray[i].u32SrcPhyAddr)
        {
            HI_ERR_CIPHER("Error: Get phyaddr for input failed!\n");
            s32Ret = HI_FAILURE;
            goto __CIPHER_EXIT__;
        }
        pInputAddrVir[i] = HI_MMZ_Map(cipherDataArray[i].u32SrcPhyAddr, u32Testcached);

        cipherDataArray[i].u32DestPhyAddr = POINT_TO_U32(HI_MMZ_New(u32TestDataLen, 0, NULL, "cipherOut"));
        if (0 == cipherDataArray[i].u32DestPhyAddr)
        {
            HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
            s32Ret = HI_FAILURE;
            goto __CIPHER_EXIT__;
        }
        pOutputAddrVir[i] = HI_MMZ_Map(cipherDataArray[i].u32DestPhyAddr, u32Testcached);

        cipherDataArray[i].u32ByteLength = u32TestDataLen;
        memset(pInputAddrVir[i], 0x0, u32TestDataLen);
        memcpy(pInputAddrVir[i], aes_cbc_dec_src_buf, u32TestDataLen);        
        memset(pOutputAddrVir[i], 0x0, u32TestDataLen);
    }

    s32Ret = Setconfiginfo(hTestchnid,
                            HI_FALSE,
                            HI_UNF_CIPHER_ALG_AES,
                            HI_UNF_CIPHER_WORK_MODE_CBC,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            aes_key,
                            aes_IV);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher setconfiginfo failed.\n");
        goto __CIPHER_EXIT__;
    }
    
    s32Ret = HI_UNF_CIPHER_DecryptMulti(hTestchnid, cipherDataArray, 3);
    if(HI_SUCCESS != s32Ret)
    {
    	HI_ERR_CIPHER("Cipher DecryptMulti failed.\n");
    	goto __CIPHER_EXIT__;
    }

    for(i = 0; i < 3; i++)
    {
        if (0 != memcmp(pOutputAddrVir[i], aes_cbc_dec_dst_buf, u32TestDataLen))
        {
            HI_ERR_CIPHER("MultiCipher AES CBC Decryption run failed!\n");
            s32Ret = HI_FAILURE;
            goto __CIPHER_EXIT__;
        }
    }

    HI_INFO_CIPHER("MultiCipher AES CBC Decryption run success!\n");

__CIPHER_EXIT__:

    for(i = 0; i < 3; i++)
    {
        if(cipherDataArray[i].u32SrcPhyAddr > 0)
        {
        HI_MMZ_Unmap(cipherDataArray[i].u32SrcPhyAddr);
        HI_MMZ_Delete(cipherDataArray[i].u32SrcPhyAddr);
        }
        if(cipherDataArray[i].u32DestPhyAddr > 0)
        {
        HI_MMZ_Unmap(cipherDataArray[i].u32DestPhyAddr);
        HI_MMZ_Delete(cipherDataArray[i].u32DestPhyAddr);
        }
    }

    HI_UNF_CIPHER_DestroyHandle(hTestchnid);
    HI_UNF_CIPHER_DeInit();
    
    return s32Ret;
}

int main(int argc, char* argv[])
{    
    
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 funcNumber;

    if ( argc != 2)
    {        
        HI_INFO_CIPHER("Usage: %s funcNumber\n\n", argv[0]);                
        HI_INFO_CIPHER("#########cipher sample##########\n"
                        "[0] multiCipher_DES_ECB_ENC\n"
                        "[1] multiCipher_DES_ECB_DEC\n"
                        "[2] multiCipher_AES_CBC\n"
                        "[3] multiCipher_ex_AES_CBC\n"
                        "[4] all test sample\n");
        return HI_SUCCESS;    
    }
    
    funcNumber = strtol(argv[1],NULL,0);
    
    if ( 0 == funcNumber )
    {
        s32Ret = multiCipher_DES_ECB_ENC();
    }
    else if ( 1 == funcNumber )
    {
        s32Ret = multiCipher_DES_ECB_DEC();
    }
    else if ( 2 == funcNumber )
    {
        s32Ret = multiCipher_ex_AES_CBC_ENC();
    }
    else if ( 3 == funcNumber )
    {
        s32Ret = multiCipher_ex_AES_CBC_DEC();
    }
    else    
    {      
        s32Ret = multiCipher_DES_ECB_ENC();
        s32Ret|= multiCipher_DES_ECB_DEC();
        s32Ret|= multiCipher_ex_AES_CBC_ENC();
        s32Ret|= multiCipher_ex_AES_CBC_DEC();
    }

    if(HI_SUCCESS != s32Ret)    
    {        
        HI_INFO_CIPHER("funcNumber %d run failure.\n",funcNumber);        
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

