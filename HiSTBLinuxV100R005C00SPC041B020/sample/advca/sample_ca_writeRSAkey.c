/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_writersakey.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "hi_type.h"
#include "hi_unf_advca.h"
#include "sample_ca_common.h"

static unsigned char rsa_key[512] = {0};

HI_S32 main(HI_S32 argc,HI_CHAR** argv)
{
    HI_S32 Ret;
    HI_U8 u8RSAKey[512] = {0};
    HI_U32 i = 0;

    FILE* fp = NULL;

    if (argc != 2)
    {
        printf("Usage: sample_ca_writeRSAkey keyfile (If no keyfile, do nothing!)\n");
        printf("for example: sample_ca_writeRSAkey /sdcard/root_rsa_pub.bin\n");
        return HI_FAILURE;
    }
    if ((fp = fopen(argv[1], "rb")) == NULL)
    {
        printf("Open root_rsa_pub.bin failed!\n");
        return HI_FAILURE;
    }

    fread(rsa_key, 1, 512, fp);
/*
    printf("The rsa_key are:\n");
    for (i = 0; i < 512; i++)
    {
        printf("%02x%s", rsa_key[i], ((i + 1) % 16) ? " " : "\n");
    }
*/
    fclose(fp);
    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_ADVCA_SetRSAKey(rsa_key);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("set RSA key failed\n");
        (HI_VOID)HI_UNF_ADVCA_DeInit();
        return HI_FAILURE;
    }

    Ret = HI_UNF_ADVCA_GetRSAKey(u8RSAKey);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("get RSA key failed\n");
        (HI_VOID)HI_UNF_ADVCA_DeInit();
        return HI_FAILURE;
    }
    // The host CPU cann't read the RSA Key after the lock operation.
    Ret = HI_UNF_ADVCA_ConfigLockFlag(HI_UNF_ADVCA_LOCK_RSA_KEY, HI_NULL, 0);
    if(HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Lock RSA key failed\n");
        (HI_VOID)HI_UNF_ADVCA_DeInit();
        return HI_FAILURE;
    }

    printf("The RSA Key are:\n");
    for (i = 0; i < 512; i++)
    {
        printf("%02x%s", u8RSAKey[i], ((i + 1) % 16) ? " " : "\n");
    }

    (HI_VOID)HI_UNF_ADVCA_DeInit();
    return HI_SUCCESS;
}


