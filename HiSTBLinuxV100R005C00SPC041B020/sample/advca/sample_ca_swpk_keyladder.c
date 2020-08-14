/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_swpk_keyladder.c
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
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <assert.h>

#include "hi_type.h"
#include "hi_unf_ecs.h"
#include "hi_unf_advca.h"
#include "hi_common.h"
#include "hi_adp.h"
#include "sample_ca_common.h"

#define DATA_SRC_LEN 32
#define DATA_DST_LEN (((DATA_SRC_LEN - 1) / 8  + 1) * 8)

HI_U8 PlainText[DATA_DST_LEN] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                                 0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
                                 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17};
HI_U8 CipherText[DATA_DST_LEN] = {0};
HI_U8 clear_key[16] = {0x3E,0x48,0xdc,0x02,0xd8,0x57,0x57,0x3d,0xb9,0x06,0x84,0x02,0xee,0xa3,0x18,0x85};
HI_U8 encrypt_key[16] = {0x3E,0x48,0xdc,0x02,0xd8,0x57,0x57,0x3d,0xb9,0x06,0x84,0x02,0xee,0xa3,0x18,0x85};

HI_S32 Setconfiginfo(HI_HANDLE chnHandle)
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

    memcpy(CipherCtrl.u32Key,encrypt_key,16);
    memset(CipherCtrl.u32IV,0,16);

    HIAPI_RUN_RETURN(HI_UNF_CIPHER_ConfigHandle(chnHandle, &CipherCtrl));

    return HI_SUCCESS;
}

int main(int argc, char* argv[])
{
    HI_S32 i;
    HI_U32 Testlen = 1024;

    HI_U32 inputAddrPhy;
    HI_U32 outPutAddrPhy;

    HI_U32 Testcached = 0;

    HI_U8* inputAddrVir;
    HI_U8* outputAddrVir;

    HI_HANDLE TestchnidEnc = 0;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;

    memset(&stCipherAttr, 0, sizeof(stCipherAttr));
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;

    (HI_VOID)HI_SYS_Init();
    (HI_VOID)HI_UNF_ADVCA_Init();

    HIAPI_RUN_RETURN(HI_UNF_CIPHER_Init());
    HIAPI_RUN_RETURN(HI_UNF_CIPHER_CreateHandle(&TestchnidEnc, &stCipherAttr));

    (HI_VOID)HI_UNF_ADVCA_EncryptSWPK(clear_key, encrypt_key);

    HIAPI_RUN_RETURN(Setconfiginfo(TestchnidEnc));

    /*************************************************************/
    inputAddrPhy = (HI_U32)((HI_ULONG)HI_MMZ_New(Testlen, 0, NULL, "cipherIn"));
    if (0 == inputAddrPhy)
    {
        HI_DEBUG_ADVCA("get phyaddr for Input!\n");
        return HI_FAILURE;
    }
    inputAddrVir = (HI_U8*)HI_MMZ_Map(inputAddrPhy, Testcached);

    outPutAddrPhy = (HI_U32)((HI_ULONG)HI_MMZ_New(Testlen, 0, NULL, "cipherOut"));
    if (0 == outPutAddrPhy)
    {
        HI_DEBUG_ADVCA("get phyaddr for OutPut!\n");
        return HI_FAILURE;
    }
    outputAddrVir = (HI_U8*)HI_MMZ_Map(outPutAddrPhy, Testcached);
    memcpy(inputAddrVir,PlainText,DATA_DST_LEN);
    memset(outputAddrVir, 0, DATA_DST_LEN);
    //Encrypt
    HIAPI_RUN_RETURN(HI_UNF_CIPHER_Encrypt(TestchnidEnc, inputAddrPhy, outPutAddrPhy, DATA_DST_LEN));

    printf("Encrypt by BLPK keyladder:\n");
    for(i = 0 ; i < DATA_DST_LEN;i++)
    {
        printf("0x%02x,",*(HI_U8*)(outputAddrVir + i));

    }
    printf("\n");

    //Decrypt
    memset(inputAddrVir, 0, DATA_DST_LEN);
    HIAPI_RUN_RETURN(HI_UNF_CIPHER_Decrypt(TestchnidEnc, outPutAddrPhy, inputAddrPhy, DATA_DST_LEN));

    printf("Decrypt by BLPK keyladder:\n");
    for(i = 0 ; i < DATA_DST_LEN;i++)
    {
        printf("0x%02x,",*(HI_U8*)(inputAddrVir + i));

    }
    printf("\n");

    (HI_VOID)HI_MMZ_Unmap(inputAddrPhy);
    (HI_VOID)HI_MMZ_Delete(inputAddrPhy);

    (HI_VOID)HI_MMZ_Unmap(outPutAddrPhy);
    (HI_VOID)HI_MMZ_Delete(outPutAddrPhy);

    HIAPI_RUN_RETURN(HI_UNF_CIPHER_DestroyHandle(TestchnidEnc));
    HIAPI_RUN_RETURN(HI_UNF_CIPHER_DeInit());

    (HI_VOID)HI_UNF_ADVCA_DeInit();

    (HI_VOID)HI_SYS_DeInit();
    return HI_SUCCESS;
}
