/******************************************************************************
Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_anticopy.c
Version       : Initial Draft
Author        : Hisilicon
Created       : 2013/09/23
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_unf_cipher.h"
#include "hi_unf_otp.h"
#include "hi_adp.h"

#ifdef  HI_ADVCA_FUNCION_RELEASE
#define HI_ERR_CIPHER(format, arg...)
#define HI_INFO_CIPHER(format, arg...)
#else
#define HI_ERR_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_CIPHER(format, arg...)   printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#endif
#define U32_TO_POINT(addr)  ((HI_VOID*)((HI_SIZE_T)(addr)))
#define POINT_TO_U32(addr)  ((HI_U32)((HI_SIZE_T)(addr)))

static const HI_U8 g_aes_128_cbc_IV[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const HI_U8 g_au8DataInSource[16] = {0x12, 0x34, 0x56, 0x78, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
HI_U8 g_au8DataInDest[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
HI_U8 g_au8DataOut[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const HI_U8 g_au8StbRootKey[16] = {0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

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


static HI_S32 Cipher_Setconfiginfo(HI_HANDLE                    hHandle,
                        HI_BOOL                     isKeyByCA,
                        HI_UNF_CIPHER_CA_TYPE_E enCaType,
                        HI_UNF_CIPHER_ALG_E         alg,
                        HI_UNF_CIPHER_WORK_MODE_E   mode,
                        HI_UNF_CIPHER_KEY_LENGTH_E  keyLen,
                        const HI_U8                 au8KeyBuf[16],
                        const HI_U8                 au8IVBuf[16])
{
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_CIPHER_CTRL_S CipherCtrl;

    memset(&CipherCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    CipherCtrl.bKeyByCA = isKeyByCA;
    CipherCtrl.enCaType = enCaType;
    CipherCtrl.enAlg = alg;
    CipherCtrl.enWorkMode = mode;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_64BIT;
    CipherCtrl.enKeyLen = keyLen;
    if(CipherCtrl.enWorkMode != HI_UNF_CIPHER_WORK_MODE_ECB)
    {
        CipherCtrl.stChangeFlags.bit1IV = 1;  //must set for CBC , CFB mode
        memcpy(CipherCtrl.u32IV, au8IVBuf, 16);
    }

    if( HI_FALSE == isKeyByCA )
    {
        memcpy(CipherCtrl.u32Key, au8KeyBuf, 32);
    }

    ret = HI_UNF_CIPHER_ConfigHandle(hHandle, &CipherCtrl);
    if(HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* define by customer */
static HI_S32 Customer_CompareDataCalculate(const HI_U8 au8DataInSource[16], HI_U8 au8DataInDest[16])
{
    if((NULL == au8DataInSource) || (NULL == au8DataInDest))
    {
        HI_ERR_CIPHER("Invalid param, null pointer!\n");
        return HI_FAILURE;
    }

    au8DataInDest[15] = au8DataInSource[0];
    au8DataInDest[14] = au8DataInSource[1];
    au8DataInDest[13] = au8DataInSource[2];
    au8DataInDest[12] = au8DataInSource[3];
    au8DataInDest[11] = au8DataInSource[4];
    au8DataInDest[10] = au8DataInSource[5];
    au8DataInDest[9]  = au8DataInSource[6];
    au8DataInDest[8]  = au8DataInSource[7];
    au8DataInDest[7]  = au8DataInSource[8];
    au8DataInDest[6]  = au8DataInSource[9];
    au8DataInDest[5]  = au8DataInSource[10];
    au8DataInDest[4]  = au8DataInSource[11];
    au8DataInDest[3]  = au8DataInSource[12];
    au8DataInDest[2]  = au8DataInSource[13];
    au8DataInDest[1]  = au8DataInSource[14];
    au8DataInDest[0]  = au8DataInSource[15];

    printBuffer("au8DataInSource :", au8DataInSource, 16);
    printBuffer("au8DataInDest :\n", au8DataInDest, 16);

    return HI_SUCCESS;
}

static HI_S32 BurnStbRootKey(const HI_U8 au8StbRootKey[16])
{
    HI_S32 ret = HI_SUCCESS;

    if(NULL == au8StbRootKey)
    {
        HI_ERR_CIPHER("Invalid param, null pointer!\n");
        return HI_FAILURE;
    }

    ret = HI_UNF_OTP_Init();
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Init otp failed!\n");
        return HI_FAILURE;
    }

    ret = HI_UNF_OTP_WriteStbRootKey((HI_U8*)au8StbRootKey, 16);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Write stb root key failed!\n");
        (HI_VOID)HI_UNF_OTP_DeInit();
        return HI_FAILURE;
    }
    else
    {
        printBuffer("burn stb root key:", au8StbRootKey, 16);
    }

    (HI_VOID)HI_UNF_OTP_DeInit();

    return HI_SUCCESS;
}

static HI_S32 GenerateAndBurnCompareDataToOTP(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32TestDataLen = 16;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U32 u32Testcached = 0;
    HI_U8 *pInputAddrVir = NULL;
    HI_U8 *pOutputAddrVir = NULL;
    HI_HANDLE hTestchnid = 0;
    HI_UNF_CIPHER_ATTS_S stCipherAttr = {0};

/* 1 calculate g_au8DataInDest */
    (HI_VOID)Customer_CompareDataCalculate(g_au8DataInSource, g_au8DataInDest);

/* 2 gernerate compare data */
    ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != ret)
    {
        (HI_VOID)HI_SYS_DeInit();
        return HI_FAILURE;
    }

//    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_COPY_AVOID;
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid, &stCipherAttr);
    if(HI_SUCCESS != ret)
    {
        (HI_VOID)HI_UNF_CIPHER_DeInit();
        return HI_FAILURE;
    }

    u32InputAddrPhy = POINT_TO_U32(HI_MMZ_New(u32TestDataLen, 0, NULL, "CIPHER_BufIn"));
    if (0 == u32InputAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for input failed!\n");
        goto __CIPHER_EXIT__;
    }
    pInputAddrVir = HI_MMZ_Map(u32InputAddrPhy, u32Testcached);

    u32OutPutAddrPhy = POINT_TO_U32(HI_MMZ_New(u32TestDataLen, 0, NULL, "CIPHER_BufOut"));
    if (0 == u32OutPutAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
        goto __CIPHER_EXIT__;
    }
    pOutputAddrVir = HI_MMZ_Map(u32OutPutAddrPhy, u32Testcached);

    ret = Cipher_Setconfiginfo(hTestchnid,
                                    HI_TRUE,
                                    HI_UNF_CIPHER_CA_TYPE_STBROOTKEY,
                                    HI_UNF_CIPHER_ALG_AES,
                                    HI_UNF_CIPHER_WORK_MODE_CBC,
                                    HI_UNF_CIPHER_KEY_AES_128BIT,
                                    NULL,
                                    g_aes_128_cbc_IV);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Set config info failed.\n");
        goto __CIPHER_EXIT__;
    }

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, g_au8DataInDest, u32TestDataLen);
    printBuffer("clear text:", g_au8DataInDest, 16);

    memset(pOutputAddrVir, 0x0, u32TestDataLen);
    ret = HI_UNF_CIPHER_Encrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher encrypt failed.\n");
        ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    memcpy(g_au8DataOut, pOutputAddrVir, u32TestDataLen);
    printBuffer("encrypted text:", g_au8DataOut, 16);

/* 3 set compare data to customer key */
    ret = HI_UNF_OTP_Init();
    ret |= HI_UNF_OTP_SetCustomerKey(g_au8DataOut, 16);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("set customer key failed.\n");
        (HI_VOID)HI_UNF_OTP_DeInit();
        ret = HI_FAILURE;
    }
    else
    {
        printBuffer("set customer key:", g_au8DataOut, 16);
    }

    (HI_VOID)HI_UNF_OTP_DeInit();

__CIPHER_EXIT__:
    HI_MMZ_Unmap(u32InputAddrPhy);
    HI_MMZ_Delete(u32InputAddrPhy);
    HI_MMZ_Unmap(u32OutPutAddrPhy);
    HI_MMZ_Delete(u32OutPutAddrPhy);

    HI_UNF_CIPHER_DestroyHandle(hTestchnid);
    HI_UNF_CIPHER_DeInit();

    return ret;
}

static HI_S32 VerifyCompareData(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 au8CustomerKey[16] = {0};
    HI_U32 u32TestDataLen = 16;
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U32 u32Testcached = 0;
    HI_U8 *pInputAddrVir = NULL;
    HI_U8 *pOutputAddrVir = NULL;
    HI_HANDLE hTestchnid = 0;
    HI_UNF_CIPHER_ATTS_S stCipherAttr = {0};

/* 1 Get customer key */
    ret = HI_UNF_OTP_Init();
    ret |= HI_UNF_OTP_GetCustomerKey(au8CustomerKey,16);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Get customer key failed.\n");
        (HI_VOID)HI_UNF_OTP_DeInit();
        ret = HI_FAILURE;
    }
    (HI_VOID)HI_UNF_OTP_DeInit();

/* 2 calculate g_au8DataInDest */
    (HI_VOID)Customer_CompareDataCalculate(g_au8DataInSource, g_au8DataInDest);

/* 3 generate compare data */
    ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != ret)
    {
        (HI_VOID)HI_SYS_DeInit();
        return HI_FAILURE;
    }

//    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_COPY_AVOID;
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid, &stCipherAttr);
    if(HI_SUCCESS != ret)
    {
        (HI_VOID)HI_UNF_CIPHER_DeInit();
        return HI_FAILURE;
    }

    u32InputAddrPhy = POINT_TO_U32(HI_MMZ_New(u32TestDataLen, 0, NULL, "CIPHER_BufIn"));
    if (0 == u32InputAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for input failed!\n");
        goto __CIPHER_EXIT__;
    }
    pInputAddrVir = HI_MMZ_Map(u32InputAddrPhy, u32Testcached);

    u32OutPutAddrPhy = POINT_TO_U32(HI_MMZ_New(u32TestDataLen, 0, NULL, "CIPHER_BufOut"));
    if (0 == u32OutPutAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
        goto __CIPHER_EXIT__;
    }
    pOutputAddrVir = HI_MMZ_Map(u32OutPutAddrPhy, u32Testcached);

    ret = Cipher_Setconfiginfo(hTestchnid,
                                    HI_TRUE,
                                    HI_UNF_CIPHER_CA_TYPE_STBROOTKEY,
                                    HI_UNF_CIPHER_ALG_AES,
                                    HI_UNF_CIPHER_WORK_MODE_CBC,
                                    HI_UNF_CIPHER_KEY_AES_128BIT,
                                    NULL,
                                    g_aes_128_cbc_IV);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Set config info failed.\n");
        goto __CIPHER_EXIT__;
    }

    memset(pInputAddrVir, 0x0, u32TestDataLen);
    memcpy(pInputAddrVir, au8CustomerKey, u32TestDataLen);
    printBuffer("clear text:", au8CustomerKey, sizeof(au8CustomerKey));

    memset(pOutputAddrVir, 0x0, u32TestDataLen);

    ret = HI_UNF_CIPHER_Decrypt(hTestchnid, u32InputAddrPhy, u32OutPutAddrPhy, u32TestDataLen);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher encrypt failed.\n");
        ret = HI_FAILURE;
        goto __CIPHER_EXIT__;
    }

    memcpy(g_au8DataOut, pOutputAddrVir, u32TestDataLen);
    printBuffer("decrypted text:", g_au8DataOut, sizeof(g_au8DataOut));

/* 4 compare to customer key */
    ret = memcmp(g_au8DataOut, g_au8DataInDest, sizeof(g_au8DataOut));
    if(0 != ret)
    {
        HI_ERR_CIPHER("memcmp failed!\n");
        HI_MMZ_Unmap(u32InputAddrPhy);
        HI_MMZ_Delete(u32InputAddrPhy);
        HI_MMZ_Unmap(u32OutPutAddrPhy);
        HI_MMZ_Delete(u32OutPutAddrPhy);
        HI_UNF_CIPHER_DestroyHandle(hTestchnid);
        HI_UNF_CIPHER_DeInit();
        /* compare failed, must reboot now */
        system("reboot");
    }
    else
    {
        HI_INFO_CIPHER("memcmp success!\n");
        ret = HI_SUCCESS;
    }

__CIPHER_EXIT__:
    HI_MMZ_Unmap(u32InputAddrPhy);
    HI_MMZ_Delete(u32InputAddrPhy);
    HI_MMZ_Unmap(u32OutPutAddrPhy);
    HI_MMZ_Delete(u32OutPutAddrPhy);

    HI_UNF_CIPHER_DestroyHandle(hTestchnid);
    HI_UNF_CIPHER_DeInit();

    return ret;
}


HI_S32 main(int argc, char* argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 funcNumber = 0;

    (HI_VOID)HI_SYS_Init();

    if(2 != argc)
    {
        HI_ERR_CIPHER("Invalid param, usage:\n");
        printf("    %s num. Attenttion: Please follow the steps below to test the anti-copy function.\n", argv[0]);
        printf("    step 1 :burn Stb Root Key. Attention:if burned already, please skip this step \n");
        printf("    step 2 :generate compared data and burn to otp. Attention:if step 1 finished, please makesure reboot system before this step \n");
        printf("    step 3 :verify compare data from otp.\n");
        return HI_FAILURE;
    }

    funcNumber = strtol(argv[1],NULL,0);
    if ( 1 == funcNumber )
    {
        ret = BurnStbRootKey(g_au8StbRootKey);
    }
    else if ( 2 == funcNumber )
    {
        ret = GenerateAndBurnCompareDataToOTP();
    }
    else if ( 3 == funcNumber )
    {
        ret = VerifyCompareData();
    }
    else
    {
        HI_ERR_CIPHER("Invalid param, usage:\n");
        printf("    %s num. Attenttion: Please follow the steps below to test the anti-copy function.\n", argv[0]);
        printf("    step 1 :burn Stb Root Key. Attention:if burned already, please skip this step \n");
        printf("    step 2 :generate compared data and burn to otp. Attention:if step 1 finished, please makesure reboot system before this step \n");
        printf("    step 3 :verify compare data from otp.\n");
        ret = HI_SUCCESS;
    }

    (HI_VOID)HI_SYS_DeInit();
    return ret;
}

