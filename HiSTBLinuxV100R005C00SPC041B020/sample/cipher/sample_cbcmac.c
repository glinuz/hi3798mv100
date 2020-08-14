/******************************************************************************
Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_cbcmac.c
Version       : Initial Draft
Author        : Hisilicon
Created       : 2012/07/10
Last Modified :
Description   : sample for aes cbc-mac
Function List :
History       :
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hi_type.h"
#include "hi_unf_cipher.h"
#include "hi_common.h"

#ifdef  CONFIG_SUPPORT_CA_RELEASE
#define HI_ERR_CIPHER(format, arg...)
#define HI_INFO_CIPHER(format, arg...)   printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#else
#define HI_ERR_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_CIPHER(format, arg...)   printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#endif

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

HI_S32 Setconfiginfo(HI_HANDLE chnHandle, HI_U8 *pu8Key)
{
    HI_UNF_CIPHER_CTRL_S CipherCtrl;
    HI_S32 s32Ret = HI_SUCCESS;

    memset(&CipherCtrl, 0, sizeof(CipherCtrl));

    CipherCtrl.bKeyByCA = HI_FALSE;
    CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
    CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
    CipherCtrl.stChangeFlags.bit1IV = 1;
    CipherCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_BLPK;

    memcpy(CipherCtrl.u32Key, pu8Key,16);
    memset(CipherCtrl.u32IV, 0x0, 16);

    s32Ret = HI_UNF_CIPHER_ConfigHandle(chnHandle, &CipherCtrl);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher Config Handle Failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 main(HI_S32 argc, char *argv[])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8MacValue[16];
    HI_U8 M[40] = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
        0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
        0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
        0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11
    };

    HI_U8 key[16] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    HI_U8 mac[16] = {0xdf, 0xa6, 0x67, 0x47, 0xde, 0x9a, 0xe6, 0x30, 0x30, 0xca, 0x32, 0x61, 0x14, 0x97, 0xc8, 0x27};
    HI_HANDLE hCipherHandle = 0;
    HI_UNF_CIPHER_ATTS_S stCipherAttr = {0};

    HI_SYS_Init();

    HI_UNF_CIPHER_Init();

    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hCipherHandle, &stCipherAttr);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher Create Handle Failed!\n");
        (HI_VOID)HI_SYS_DeInit();
        return HI_FAILURE;
    }

    s32Ret = Setconfiginfo(hCipherHandle, key);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher Setconfiginfo Failed!\n");
        HI_UNF_CIPHER_DeInit();
        (HI_VOID)HI_SYS_DeInit();
        return HI_FAILURE;
    }

    HI_UNF_CIPHER_CalcMAC(hCipherHandle, M, 16, u8MacValue, HI_FALSE);
    HI_UNF_CIPHER_CalcMAC(hCipherHandle, M + 16, 16, u8MacValue, HI_FALSE);
    HI_UNF_CIPHER_CalcMAC(hCipherHandle, M + 32, 8, u8MacValue, HI_TRUE);

    printBuffer("Cbcmac value:", u8MacValue, sizeof(u8MacValue));
    s32Ret = HI_UNF_CIPHER_DestroyHandle(hCipherHandle);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Cipher DestroyHandle Failed!\n");
        HI_UNF_CIPHER_DeInit();
        (HI_VOID)HI_SYS_DeInit();
        return HI_FAILURE;
    }
    if (memcmp(u8MacValue, mac, 16) != 0)
    {
        HI_ERR_CIPHER("Cipher cmac Failed!\n");
        HI_UNF_CIPHER_DeInit();
        (HI_VOID)HI_SYS_DeInit();
        return HI_FAILURE;
    }

    HI_UNF_CIPHER_DeInit();
    HI_SYS_DeInit();

    return HI_SUCCESS;
}

