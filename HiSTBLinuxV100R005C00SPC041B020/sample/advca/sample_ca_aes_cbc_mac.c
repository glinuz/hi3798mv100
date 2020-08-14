/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_aes_cbc_mac.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
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
#include <sys/mman.h>

#include "hi_type.h"
#include "hi_unf_ecs.h"
#include "hi_unf_advca.h"
#include "hi_adp.h"
#include "sample_ca_common.h"


HI_U8 g_M[64] = {
0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
};

HI_U8 g_K[16] = {
0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};

HI_U8 AEC_CBC_MAC_0[16] = {
0xBB, 0x1D, 0x69, 0x29, 0xE9, 0x59, 0x37, 0x28,
0x7F, 0xA3, 0x7D, 0x12, 0x9B, 0x75, 0x67, 0x46
};

HI_U8 AEC_CBC_MAC_16[16] = {
0x07, 0x0A, 0x16, 0xB4, 0x6B, 0x4D, 0x41, 0x44,
0xF7, 0x9B, 0xDD, 0x9D, 0xD0, 0x4A, 0x28, 0x7C
};

HI_U8 AEC_CBC_MAC_40[16] = {
0xDF, 0xA6, 0x67, 0x47, 0xDE, 0x9A, 0xE6, 0x30,
0x30, 0xCA, 0x32, 0x61, 0x14, 0x97, 0xC8, 0x27
};

HI_U8 AEC_CBC_MAC_64[16] = {
0x51, 0xF0, 0xBE, 0xBF, 0x7E, 0x3B, 0x9D, 0x92,
0xFC, 0x49, 0x74, 0x17, 0x79, 0x36, 0x3C, 0xFE
};

HI_S32 Setconfiginfo(HI_HANDLE chnHandle, HI_U8 *pu8Key)
{
    HI_UNF_CIPHER_CTRL_S CipherCtrl;

    memset(&CipherCtrl, 0, sizeof(CipherCtrl));

    CipherCtrl.bKeyByCA = HI_TRUE;
    CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
    CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
    CipherCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_BLPK;
    CipherCtrl.stChangeFlags.bit1IV = 1;

    memcpy(CipherCtrl.u32Key, pu8Key,16);
    memset(CipherCtrl.u32IV, 0x0, 16);

    HIAPI_RUN_RETURN(HI_UNF_CIPHER_ConfigHandle(chnHandle, &CipherCtrl));

    return HI_SUCCESS;
}

HI_S32 generate_encrypt_key(HI_U8 *clear_key, HI_U8 *encrypt_key)
{   
    (HI_VOID)HI_UNF_ADVCA_EncryptSWPK(clear_key, encrypt_key);
    return HI_SUCCESS;
}

HI_VOID printf_hex(HI_U8 *pu8Buf, HI_U32 u32Length)
{
    HI_U32 i;

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

HI_VOID compare_hex(char *title, HI_U8 *pu8Src, HI_U8 *pu8Dst, HI_U32 u32Length)
{
    HI_U32 i;

    for(i = 0; i < u32Length; i++)
    {
        if(pu8Src[i] != pu8Dst[i])
        {
           printf("\033[0;31m" "%s failed!\n" "\033[0m", title);
           return;
        }
    }
    printf("%s success!\n", title);
    return;
}


HI_S32 main(HI_S32 argc, char *argv[])
{
    HI_U8 u8EncryptKey[16];
    HI_U8 T[16];
    HI_U8 M[64] = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
        0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
        0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
        0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
        0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
        0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
        0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
    };
    HI_U8 key[16] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    HI_HANDLE hCipherHandle = 0;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;

    memset(&stCipherAttr, 0, sizeof(stCipherAttr));
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;

    (HI_VOID)HI_SYS_Init();
    (HI_VOID)HI_UNF_ADVCA_Init();

    //encrypt the key first, the swpk keyladder should use the encrypted key
    (HI_VOID)generate_encrypt_key(key, u8EncryptKey);
 
    HIAPI_RUN_RETURN(HI_UNF_CIPHER_Init());
    HIAPI_RUN_RETURN(HI_UNF_CIPHER_CreateHandle(&hCipherHandle, &stCipherAttr));
    HIAPI_RUN_RETURN(Setconfiginfo(hCipherHandle, u8EncryptKey));
   
    HI_DEBUG_ADVCA("--------------------------------------------------\n");

    printf("\nExample 1: len = 0\n");
    printf("M "); printf("<empty string>\n");
    (HI_VOID)HI_UNF_CIPHER_CalcMAC(hCipherHandle, M, 0, T, HI_TRUE);
    printf("AES_CMAC "); printf_hex(T, 16); printf("\n");
    (HI_VOID)compare_hex("M_0", AEC_CBC_MAC_0, T, 16);
    
    printf("\nExample 2: len = 16\n");
    printf("M "); printf_hex(M, 16);
    (HI_VOID)HI_UNF_CIPHER_CalcMAC(hCipherHandle, M, 16, T, HI_TRUE);
    printf("AES_CMAC "); printf_hex(T, 16); printf("\n");
    (HI_VOID)compare_hex("M_16", AEC_CBC_MAC_16, T, 16);
    
    
    printf("\nExample 3: len = 40\n");
    printf("M "); printf_hex(M, 40);
    
    (HI_VOID)HI_UNF_CIPHER_CalcMAC(hCipherHandle,M, 16, T, HI_FALSE);
    (HI_VOID)HI_UNF_CIPHER_CalcMAC(hCipherHandle,M + 16, 16, T, HI_FALSE);
    (HI_VOID)HI_UNF_CIPHER_CalcMAC(hCipherHandle,M + 32, 8, T, HI_TRUE);
    
    printf("AES_CMAC "); printf_hex(T, 16); printf("\n");
    (HI_VOID)compare_hex("M_40", AEC_CBC_MAC_40, T, 16);
  
    printf("\nExample 4: len = 64\n");
    (HI_VOID)HI_UNF_CIPHER_CalcMAC(hCipherHandle, M, 64, T, HI_TRUE);
    printf("AES_CMAC "); printf_hex(T, 16); printf("\n");
    (HI_VOID)compare_hex("M_64", AEC_CBC_MAC_64, T, 16);
    printf("--------------------------------------------------\n");

    HIAPI_RUN_RETURN(HI_UNF_CIPHER_DestroyHandle(hCipherHandle));
    HIAPI_RUN_RETURN(HI_UNF_CIPHER_DeInit());
    (HI_VOID)HI_UNF_ADVCA_DeInit();
    (HI_VOID)HI_SYS_DeInit();
    return 0;
}

