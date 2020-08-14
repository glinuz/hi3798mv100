/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_lpk.c
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
#include <string.h>
#include "hi_type.h"
#include "hi_unf_advca.h"
//#include "hi_unf_advca_restrict.h"
#include "sample_ca_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define CIPHER_TEXT_LEN  23
/* 000102030405060708090a0b0c0d0e0f
   101112131415161718191a1b1c1d1e1f
   202122232425262728292a2b2c2d */
static HI_U8 g_ClearParam[CIPHER_TEXT_LEN] = {};

/* 0x02dc483e 0x3d5757d8 0x028406b9 0x8518a3ee */
//static HI_U8 g_ClearLpk[16] = {};

/* 00000080010000800200008003000080 */
static HI_U8 g_EncrpytedLpk[16] = {0x00,0x00,0x00,0x80,
                                   0x01,0x00,0x00,0x80,
                                   0x02,0x00,0x00,0x80,
                                   0x03,0x00,0x00,0x80};

static HI_U8 g_EncryptedParam[CIPHER_TEXT_LEN] = {0xfb,0xcd,0xe7,0xa0,0x96,0xec,0x3b,0x30,0x0e,0x0b,0x2a,0x55,0xe7,0xd5,0x56,0xb8,0x9c,0xc7,0x54,0xdd,0x3b,0x95,0x05,};

HI_S32 main(HI_S32 argc,HI_CHAR** argv)
{
    HI_S32 Ret,i;

    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed:%#x\n",Ret);
        return HI_FAILURE;
    }
    HI_DEBUG_ADVCA("The root key should be:0xba,0x15,0xc7,0x54,0xfb,0xe9,0x4a,0x9d,0x7a,0xf1,0x02,0xd0,0xda,0xda,0x10,0xb3\n");
    printf("Press any key to load lpk\n");
    getchar();
    
    Ret = HI_UNF_ADVCA_LoadLpk(g_EncrpytedLpk);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_LoadLpk failed:%#x\n",Ret);
        goto OUT;
    }

    printf("Press any key to decrypt\n");
    getchar();

    memset(g_ClearParam, 0x0, CIPHER_TEXT_LEN);
	Ret = HI_UNF_ADVCA_DecryptLptParam(g_EncryptedParam, CIPHER_TEXT_LEN, g_ClearParam);
	if(HI_SUCCESS != Ret)
	{
		HI_DEBUG_ADVCA("HI_UNF_ADVCA_DecryptLptParam failure:%#x\n",Ret);
		goto OUT;
	}

    HI_DEBUG_ADVCA("==========ClearParam============\n");
    HI_DEBUG_ADVCA("The result should be:000102030405060708090a0b0c0d0e0f10111213141516\n");
    for (i = 0 ; i < CIPHER_TEXT_LEN; i++)
    {
        HI_DEBUG_ADVCA("%02x", g_ClearParam[i]);
        if ((i + 1) % 16 == 0)
        {
            HI_DEBUG_ADVCA("\n");
        }
    }
    HI_DEBUG_ADVCA("\n");
    
OUT:
    Ret = HI_UNF_ADVCA_DeInit();
    return Ret;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

