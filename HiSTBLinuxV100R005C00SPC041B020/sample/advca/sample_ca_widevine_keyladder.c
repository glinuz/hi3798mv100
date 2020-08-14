/******************************************************************************
 *
 * Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
 * ****************************************************************************
 * File Name     : sample_widevine_crypto.c
 * Version       : Initial
 * Author        : Hisilicon hisecurity team
 * Created       : 2014-02-17
 * Last Modified :
 * Description   :	sample for widevine encryption and decryption
 * Function List :
 * History       : 
 * ****************************************************************************/
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
#include "hi_unf_cipher.h"
#include "hi_unf_advca.h"
#include "sample_ca_common.h"
#include "hi_common.h"

#define TEST_DECRYPT
//#define TEST_ENCRYPT

static HI_U8 ClearDeviceKey[16] = {0x55, 0x66, 0x77, 0x88, 0x44, 0x44, 0x44, 0x44, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22};
static HI_U8 EncryptedDeviceKey[16] = {0X79, 0XAC, 0XE0, 0X5F, 0X26, 0X7F, 0XE1, 0X3F, 0XCA, 0X7F, 0XC4, 0X4A, 0X8A, 0X50, 0X46, 0X4B};

HI_U8 SessionKey_Lev1[16] = {0x24, 0x35, 0x15, 0x12, 0x81, 0x5e, 0x89, 0xc0, 0x09, 0xd6, 0x84, 0x84, 0x63, 0x56, 0xf0, 0xb1};
HI_U8 SessionKey_Lev2[16] = {0x0d, 0x7b, 0xb9, 0x06, 0x8d, 0x99, 0xdf, 0x46, 0x65, 0x84, 0xc2, 0xb2, 0x12, 0x52, 0x37, 0x89};
HI_U8 SessionKey_Lev3[32] = {0x3e, 0x89, 0x12, 0xff, 0xa6, 0xbc, 0xb1, 0x74, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12, 0x3e, 0x89, 0x12, 0xff, 0xa6, 0xbc, 0xb1, 0x74, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12};

HI_U8 CleanData[16]     = {0x4E, 0x6F, 0x77, 0x20, 0x69, 0x73, 0x20, 0x74, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
HI_U8 EncryptedData[16] = {0x9c, 0x6c, 0x4d, 0x73, 0xbb, 0x2e, 0xc7, 0xfc, 0x7f, 0x0a, 0x37, 0x52, 0x6b, 0xc5, 0xc4, 0x5c};

HI_U8 u8IVBuf[16] = {0x0, 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

static HI_VOID showusage()
{
    printf("Usage:\n"
           "To show widevine keyladder encrypt device key  : ./sample_ca_widevie_keyladder_crypto 0\n"
           "To show widevine keyladder encrypt/decrypt data: ./sample_ca_widevie_keyladder_crypto 1\n"
           );
}

//static HI_VOID Debug_Register(HI_VOID)
//{
//    usleep(100*1000);system("himd.l 0xF8A90000");usleep(100*1000);
//}

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

static HI_S32 printBuffer(HI_CHAR *string, HI_U8 *pu8Input, HI_U32 u32Length)
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

HI_S32 encryptDeviceKey()
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    HI_UNF_ADVCA_KEYLADDER_ATTR_S stGDRMAttr;
    memset(&stGDRMAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
    stGDRMAttr.unKlAttr.stGDRMAttr.enGDRMKlAttr = HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_ENABLE;
    stGDRMAttr.unKlAttr.stGDRMAttr.bEnable = HI_TRUE;
    s32Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_GDRM, &stGDRMAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("open GDRM keyladder failed\n");
    }

    memset(&stGDRMAttr, 0, sizeof (stGDRMAttr));
    memcpy(stGDRMAttr.unKlAttr.stGDRMAttr.au8SessionKey, ClearDeviceKey, 16);
    stGDRMAttr.unKlAttr.stGDRMAttr.enGDRMKlAttr = HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_ENCRYPT;
    stGDRMAttr.unKlAttr.stGDRMAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV1;
    s32Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_GDRM, &stGDRMAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("setDevice key failed\n");
        return HI_FAILURE;
    }

    printBuffer("Clear Device Key:", ClearDeviceKey, 16);
    printBuffer("Encrypted Device key:", stGDRMAttr.unKlAttr.stGDRMAttr.au8Output, 16);
    printf("if the OEMRootKey is 0x0, the compare below will be success\n");
    compare_hex("Compare encrypted device key", stGDRMAttr.unKlAttr.stGDRMAttr.au8Output, EncryptedDeviceKey, 16);

    usleep(100*1000);
    memset(&stGDRMAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
    stGDRMAttr.unKlAttr.stGDRMAttr.enGDRMKlAttr = HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_ENABLE;
    stGDRMAttr.unKlAttr.stGDRMAttr.bEnable = HI_FALSE;
    s32Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_GDRM, &stGDRMAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("close GDRM keyladder failed\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 SetSessionKey(HI_HANDLE hCipherHandle)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    HI_UNF_ADVCA_KEYLADDER_ATTR_S stKeyAttr;

    memset(&stKeyAttr, 0x0, sizeof (stKeyAttr));
    memcpy(stKeyAttr.unKlAttr.stGDRMAttr.au8SessionKey, SessionKey_Lev1, 16);
    stKeyAttr.unKlAttr.stGDRMAttr.enGDRMKlAttr = HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_SESSION_KEY;
    stKeyAttr.unKlAttr.stGDRMAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV1;
    s32Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_GDRM, &stKeyAttr);
	if (HI_SUCCESS != s32Ret)
	{
	    HI_DEBUG_ADVCA("set session lev1 failed\n");
		return HI_FAILURE;
	}
    //Debug_Register();
    usleep(100*1000);
    printBuffer("Level1 Session Key:", SessionKey_Lev1, 16);

    memset(&stKeyAttr, 0x0, sizeof (stKeyAttr));
    memcpy(stKeyAttr.unKlAttr.stGDRMAttr.au8SessionKey, SessionKey_Lev2, 16);
    stKeyAttr.unKlAttr.stGDRMAttr.enGDRMKlAttr = HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_SESSION_KEY;
    stKeyAttr.unKlAttr.stGDRMAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV2;
    s32Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_GDRM, &stKeyAttr);
	if (HI_SUCCESS != s32Ret)
	{
	    HI_DEBUG_ADVCA("set session lev2 failed\n");
		return HI_FAILURE;
	}
    //Debug_Register();
    usleep(100*1000);
    printBuffer("Level2 Session Key:", SessionKey_Lev2, 16);


    memset(&stKeyAttr, 0x0, sizeof (stKeyAttr));
    memcpy(stKeyAttr.unKlAttr.stGDRMAttr.au8SessionKey, SessionKey_Lev3, 32);
    stKeyAttr.unKlAttr.stGDRMAttr.enGDRMKlAttr = HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_SESSION_KEY;
    stKeyAttr.unKlAttr.stGDRMAttr.hCipherHandle = hCipherHandle;
    stKeyAttr.unKlAttr.stGDRMAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV3;

    s32Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_GDRM, &stKeyAttr);
    if(HI_SUCCESS != s32Ret)
	{
        HI_DEBUG_ADVCA("set session lev3 failed\n");
		return HI_FAILURE;	
	}
    //Debug_Register();
    usleep(100*1000);

    memset(&stKeyAttr, 0x0, sizeof (stKeyAttr));
    stKeyAttr.unKlAttr.stGDRMAttr.enGDRMKlAttr = HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_GETFLAG;
    s32Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_GDRM, &stKeyAttr);
    if(HI_SUCCESS != s32Ret)
	{
        HI_DEBUG_ADVCA("set session lev3 failed\n");
		return HI_FAILURE;	
	}
    printBuffer("Level3 Session Key:", SessionKey_Lev3, 32);
    printBuffer("GDRM Flag:", stKeyAttr.unKlAttr.stGDRMAttr.au8Output, 4);
    usleep(100*1000);
    return HI_SUCCESS;  
}

HI_S32 ConfigCipherAttr(HI_HANDLE hCipherHandle)
{
    HI_UNF_CIPHER_CTRL_S CipherCtrl;
    memset(&CipherCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    CipherCtrl.bKeyByCA = HI_TRUE;
    CipherCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_GDRM;
    CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
    CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
    CipherCtrl.stChangeFlags.bit1IV = 1;
    memset(CipherCtrl.u32IV, 0, 16);

    return HI_UNF_CIPHER_ConfigHandle(hCipherHandle, &CipherCtrl);
}

HI_S32 DecryptData(HI_HANDLE hCipherHandle)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_MMZ_BUF_S PlainBuf,EncrytedBuf;

    strcpy(PlainBuf.bufname,"plain_text");
    PlainBuf.bufsize = 0x1000;
    PlainBuf.overflow_threshold = 100;
    PlainBuf.underflow_threshold = 0;
    s32Ret = HI_MMZ_Malloc(&PlainBuf);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("HI_MMZ_Malloc failed:%x\n",s32Ret);
        return HI_FAILURE;
    }

    strcpy(EncrytedBuf.bufname,"encrypted_text");
    EncrytedBuf.bufsize = 0x1000;
    EncrytedBuf.overflow_threshold = 100;
    EncrytedBuf.underflow_threshold = 0;
    s32Ret = HI_MMZ_Malloc(&EncrytedBuf);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("HI_MMZ_Malloc failed:%x\n",s32Ret);
        (HI_VOID)HI_MMZ_Free(&PlainBuf);
        return HI_FAILURE;
    }


    s32Ret = ConfigCipherAttr(hCipherHandle);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("config cipher failed!\n");
        goto __CIPHER_EXIT__;
    }
    memcpy(PlainBuf.user_viraddr, EncryptedData, 16);
    memset(EncrytedBuf.user_viraddr, 0, EncrytedBuf.bufsize);   
    s32Ret = HI_UNF_CIPHER_Decrypt(hCipherHandle,PlainBuf.phyaddr, EncrytedBuf.phyaddr, 16);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("Encrypt data failed!\n");
        goto __CIPHER_EXIT__;
    }


    printf("=============== Decrypt result =================\n");
    printBuffer("Encrypted data:", EncryptedData, 16);
    printBuffer("Clear data:", EncrytedBuf.user_viraddr, 16);
    printf("if the OEMRootKey is 0x0, the compare below will be success\n");
    compare_hex("Compare decrypted data", EncrytedBuf.user_viraddr, CleanData, 16);

__CIPHER_EXIT__:
    (HI_VOID)HI_MMZ_Free(&PlainBuf);
    (HI_VOID)HI_MMZ_Free(&EncrytedBuf);  

    return s32Ret;
}

HI_S32 EncryptData(HI_HANDLE hCipherHandle)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_MMZ_BUF_S PlainBuf,EncrytedBuf;

    strcpy(PlainBuf.bufname,"plain_text");
    PlainBuf.bufsize = 0x10;
    PlainBuf.overflow_threshold = 100;
    PlainBuf.underflow_threshold = 0;
    s32Ret = HI_MMZ_Malloc(&PlainBuf);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("HI_MMZ_Malloc failed:%x\n",s32Ret);
        return HI_FAILURE;
    }

    strcpy(EncrytedBuf.bufname,"encrypted_text");
    EncrytedBuf.bufsize = 0x10;
    EncrytedBuf.overflow_threshold = 100;
    EncrytedBuf.underflow_threshold = 0;
    s32Ret = HI_MMZ_Malloc(&EncrytedBuf);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("HI_MMZ_Malloc failed:%x\n",s32Ret);
        (HI_VOID)HI_MMZ_Free(&PlainBuf);
        return HI_FAILURE;
    }


    s32Ret = ConfigCipherAttr(hCipherHandle);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("config cipher failed!\n");
        goto __CIPHER_EXIT__;
    }
    memcpy(PlainBuf.user_viraddr, CleanData, 16);
    memset(EncrytedBuf.user_viraddr, 0, EncrytedBuf.bufsize);   
    s32Ret = HI_UNF_CIPHER_Encrypt(hCipherHandle,PlainBuf.phyaddr, EncrytedBuf.phyaddr, 16);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("Encrypt data failed!\n");
        goto __CIPHER_EXIT__;
    }


    printf("=============== Encrypt result =================\n");
    printBuffer("Clean data:", CleanData, 16);
    printBuffer("Encrypted data:", EncrytedBuf.user_viraddr, 16);
    printf("if the OEMRootKey is 0x0, the compare below will be success\n");
    compare_hex("Compare encrypted data", EncrytedBuf.user_viraddr, EncryptedData, 16);

__CIPHER_EXIT__:
    (HI_VOID)HI_MMZ_Free(&PlainBuf);
    (HI_VOID)HI_MMZ_Free(&EncrytedBuf);  

    return s32Ret;
}

HI_S32 EncryptandDecryptData(HI_HANDLE hCipherHandle)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_ADVCA_KEYLADDER_ATTR_S stGDRMAttr;    

    memset(&stGDRMAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
    stGDRMAttr.unKlAttr.stGDRMAttr.enGDRMKlAttr = HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_ENABLE;
    stGDRMAttr.unKlAttr.stGDRMAttr.bEnable = HI_TRUE;
    s32Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_GDRM, &stGDRMAttr);	
	if (HI_SUCCESS != s32Ret)
	{
	    HI_DEBUG_ADVCA("open GDRM keyladder failed\n");
		return HI_FAILURE;
	}

    s32Ret = SetSessionKey(hCipherHandle);
	if (HI_SUCCESS != s32Ret)
	{
	    HI_DEBUG_ADVCA("Set session key failed\n");
		return HI_FAILURE;
	}
#ifdef TEST_ENCRYPT
    s32Ret = EncryptData(hCipherHandle);
	if (HI_SUCCESS != s32Ret)
	{
	    HI_DEBUG_ADVCA("Encrypt data fail\n");
		return HI_FAILURE;
	}
#endif    
    usleep(100*1000);
#ifdef TEST_DECRYPT
    s32Ret = DecryptData(hCipherHandle);
	if (HI_SUCCESS != s32Ret)
	{
	    HI_DEBUG_ADVCA("Decrypt data fail\n");
		return HI_FAILURE;
	}

    usleep(100*1000);
#endif    
    memset(&stGDRMAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
    stGDRMAttr.unKlAttr.stGDRMAttr.enGDRMKlAttr = HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_ENABLE;
    stGDRMAttr.unKlAttr.stGDRMAttr.bEnable = HI_FALSE;
    s32Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_GDRM, &stGDRMAttr);	
	if (HI_SUCCESS != s32Ret)
	{
	    HI_DEBUG_ADVCA("open GDRM keyladder failed\n");
		return HI_FAILURE;
	}
    return HI_SUCCESS;
}


HI_S32 main(int argc, char * argv[])
{
	HI_S32 ret = HI_SUCCESS;
	HI_S32 s32Number = 0;
	HI_UNF_CIPHER_ATTS_S stCipher;
	HI_HANDLE hCipherHandle = 0xffffffff;

    if (argc < 2)
    {
        showusage();
        return HI_SUCCESS;
    }

	ret = HI_SYS_Init();
	if( HI_SUCCESS != ret )
	{
		HI_DEBUG_ADVCA("System init failed!\n");
		return HI_FAILURE;
	}

	ret = HI_UNF_CIPHER_Init();
	if ( HI_SUCCESS != ret )
	{
	    HI_DEBUG_ADVCA("Cipher init failed\n");
		return HI_FAILURE;
	}

	ret = HI_UNF_ADVCA_Init();
	if( HI_SUCCESS != ret )
	{
		HI_DEBUG_ADVCA("Advca init failed!\n");
		return HI_FAILURE;
	}

	stCipher.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
	ret = HI_UNF_CIPHER_CreateHandle(&hCipherHandle, &stCipher);
	if ( HI_SUCCESS != ret )
	{
	    HI_DEBUG_ADVCA("CIPHER Create Handle failed!\n");
		return HI_FAILURE;
	}
    
    s32Number = (0 == atoi(argv[1]))?(0):(1);

    if (0 == s32Number)
    {
        encryptDeviceKey();
    }
    else
    {
        EncryptandDecryptData(hCipherHandle);
    }
    
	(HI_VOID)HI_UNF_CIPHER_DestroyHandle(hCipherHandle);
	(HI_VOID)HI_UNF_CIPHER_DeInit();
	(HI_VOID)HI_UNF_ADVCA_DeInit();
	(HI_VOID)HI_SYS_DeInit();

	return ret;
}

