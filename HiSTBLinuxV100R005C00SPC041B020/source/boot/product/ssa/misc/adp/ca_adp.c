/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : ca_adp.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2016-01-04
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "hi_type.h"
#ifdef HI_MINIBOOT_SUPPORT
#include "app.h"
#else
#include "exports.h"
#endif
#include "hi_unf_advca.h"
#include "hi_unf_cipher.h"
#include "hi_unf_otp.h"
#include "ca_debug.h"
#include "ca_adp.h"

static HI_U32 CipherBlockSize = 1024*1024-16;   //cipher block must < 1M BYTE and be times of 16BYTE

static HI_HANDLE  g_hCipher = -1;


extern HI_S32 HI_CA_GetKeyladderKey(HI_CA_KEY_GROUP_E enKeyGroup, HI_U8 u8KeyladderKey1[16], HI_U8 u8KeyladderKey2[16], HI_U8 u8CipherKey[16]);

static HI_S32 printBuffer(HI_CHAR *string, HI_U8 *pu8Input, HI_U32 u32Length)
{
    HI_U32 i = 0;

    if ( NULL != string )
    {
        HI_PRINT("%s\n", string);
    }

    for ( i = 0 ; i < u32Length; i++ )
    {
        if( (i % 16 == 0) && (i != 0)) HI_PRINT("\n");
        HI_PRINT("0x%02x ", pu8Input[i]);
    }
    HI_PRINT("\n");

    return HI_SUCCESS;
}

extern HI_U8  g_customer_rsa_public_key_N[256];
extern HI_U32  g_customer_rsa_public_key_E;


HI_S32 CA_AdpInit(HI_CA_CryptoInfo_S *pstCryptoInfo)
{
    HI_S32 ret = 0;
    HI_U8  SessionKey1[16];
    HI_U8  SessionKey2[16];
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage;
    HI_UNF_CIPHER_CTRL_S CipherCtrl;
    HI_UNF_CIPHER_ATTS_S stCipherAtts;

    if (pstCryptoInfo->enKeyGroup > HI_CA_KEY_GROUP_MAX)
    {
       HI_ERR_CA("key group input parameter error: \n");
       return HI_FAILURE;
    }

    memset(SessionKey1,0,sizeof(SessionKey1));
    memset(SessionKey2,0,sizeof(SessionKey2));
    memset(&CipherCtrl,0,sizeof(HI_UNF_CIPHER_CTRL_S));

    HI_INFO_CA("enCipherAlg: %d, enKeyGroup: %d, enKLDAlg: %d\n", pstCryptoInfo->enCipherAlg, pstCryptoInfo->enKeyGroup, pstCryptoInfo->enKLDAlg);
    /*open and config keyladder*/
    CA_ASSERT(HI_UNF_CIPHER_Init(),ret);
    ret = HI_UNF_ADVCA_Init();
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("Fail to call adva init\n");
        HI_UNF_CIPHER_DeInit();
        return HI_FAILURE;
    }

    ret = HI_CA_GetKeyladderKey(pstCryptoInfo->enKeyGroup, SessionKey1, SessionKey2, (HI_U8*)CipherCtrl.u32Key);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("Fail to get key ladder key\n");
        goto ERROR_EXIT;
    }

    printBuffer("session1", SessionKey1, 16);
    printBuffer("session2", SessionKey2, 16);
    printBuffer("CiptherCtrKey", (HI_U8 *)CipherCtrl.u32Key, 16);

    if (pstCryptoInfo->bIsUseKLD == HI_TRUE)
    {
        ret =  HI_UNF_ADVCA_SetR2RAlg(pstCryptoInfo->enKLDAlg);
        ret |= HI_UNF_ADVCA_GetR2RKeyLadderStage(&enStage);
        ret |= HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1,SessionKey1);
        if(enStage == HI_UNF_ADVCA_KEYLADDER_LEV3)
        {
            ret |= HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV2,SessionKey2);
        }

        if (ret != HI_SUCCESS)
        {
            HI_ERR_CA("Fail to configure R2R key ladder\n");
            goto ERROR_EXIT;
        }

        CipherCtrl.bKeyByCA = HI_TRUE;
    }
    else
    {
        CipherCtrl.bKeyByCA = HI_FALSE;
    }

    /*config cipher*/
    memset(&stCipherAtts, 0, sizeof(HI_UNF_CIPHER_ATTS_S));
    stCipherAtts.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    ret = HI_UNF_CIPHER_CreateHandle(&g_hCipher, &stCipherAtts);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("Fail to create cipher handle\n");
        goto ERROR_EXIT;
    }

    if(HI_UNF_CIPHER_ALG_3DES == pstCryptoInfo->enCipherAlg)
    {
        CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_3DES;
        CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
        CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
        CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_DES_2KEY;
        CipherCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
    }
    else if(HI_UNF_CIPHER_ALG_DES == pstCryptoInfo->enCipherAlg)
    {
        CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_DES;
        CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_64BIT;
        CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
        CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_DES_2KEY;
        CipherCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
    }
    else if(HI_UNF_CIPHER_ALG_AES == pstCryptoInfo->enCipherAlg)
    {
        CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
        CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
        CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
        CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
        CipherCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
    }
    else
    {
        HI_ERR_CA("Cipher Algorithem error\n");
        goto ERROR_EXIT;
    }

    ret = HI_UNF_CIPHER_ConfigHandle(g_hCipher,&CipherCtrl);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("Fail to config cipher handle\n");
        goto ERROR_EXIT;
    }

    return HI_SUCCESS;

ERROR_EXIT:
    if (g_hCipher != -1)
    {
       HI_UNF_CIPHER_DestroyHandle(g_hCipher);
       g_hCipher = -1;
    }
    HI_UNF_CIPHER_DeInit();
    HI_UNF_ADVCA_DeInit();
    return HI_FAILURE;
}

HI_VOID CA_AdpDeInit(HI_VOID)
{
    HI_UNF_CIPHER_DestroyHandle(g_hCipher);
    g_hCipher = -1;
    HI_UNF_CIPHER_DeInit();
    HI_UNF_ADVCA_DeInit();
    return;
}

HI_S32 CA_DataEncrypt(HI_U8 *pPlainText, HI_U32 TextLen, HI_U8 *pCipherText)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 CipherBlockNum = 0;
    HI_U32 index = 0;

    CA_CheckPointer(pPlainText);
    CA_CheckPointer(pCipherText);

    if(-1 == g_hCipher)
    {
        HI_ERR_CA("not ca init function\n");
        return ret;
    }
    if((TextLen < 16) || (TextLen%16 != 0))
    {
        HI_ERR_CA("Data length must be times of 16BYTE\n");
        return ret;
    }
    CipherBlockNum = TextLen/CipherBlockSize;
    if(CipherBlockNum > 0)
    {
        for(index = 0;index <CipherBlockNum;index++)
        {
            CA_ASSERT(HI_UNF_CIPHER_Encrypt(g_hCipher,((HI_U32)pPlainText+index*CipherBlockSize),((HI_U32)pCipherText+index*CipherBlockSize),CipherBlockSize),ret);
        }
        if(TextLen > index*CipherBlockSize)
        {
            CA_ASSERT(HI_UNF_CIPHER_Encrypt(g_hCipher,((HI_U32)pPlainText+index*CipherBlockSize),((HI_U32)pCipherText+index*CipherBlockSize),(TextLen-index*CipherBlockSize)),ret);
        }
    }
    else
    {
        CA_ASSERT(HI_UNF_CIPHER_Encrypt(g_hCipher,(HI_U32)pPlainText,(HI_U32)pCipherText,TextLen),ret);
    }
    return ret;
}
HI_S32 CA_DataDecrypt(HI_U8 *pCipherText, HI_U32 TextLen, HI_U8 *pPlainText)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 CipherBlockNum = 0;
    HI_U32 index = 0;

    CA_CheckPointer(pPlainText);
    CA_CheckPointer(pCipherText);
    if(-1 == g_hCipher)
    {
        HI_ERR_CA("not ca init function\n");
        return ret;
    }
    if((TextLen < 16) || (TextLen % 16 != 0))
    {
        HI_ERR_CA("Data length must be times of 16BYTE\n");
        return ret;
    }
    CipherBlockNum = TextLen / CipherBlockSize;
    if(CipherBlockNum > 0)
    {
        for(index = 0;index <CipherBlockNum;index++)
        {
            CA_ASSERT(HI_UNF_CIPHER_Decrypt(g_hCipher,((HI_U32)pCipherText+index*CipherBlockSize),((HI_U32)pPlainText+index*CipherBlockSize),CipherBlockSize),ret);
        }
        if(TextLen > index*CipherBlockSize)
        {
            CA_ASSERT(HI_UNF_CIPHER_Decrypt(g_hCipher,((HI_U32)pCipherText+index*CipherBlockSize),((HI_U32)pPlainText+index*CipherBlockSize),(TextLen-index*CipherBlockSize)),ret);
        }
    }
    else
    {
        CA_ASSERT(HI_UNF_CIPHER_Decrypt(g_hCipher,(HI_U32)pCipherText,(HI_U32)pPlainText,TextLen),ret);
    }
    return ret;
}


/*******************The following are exported interfaces for usage*******************************************/
HI_S32 HI_CA_ADP_DataEncrypt(HI_CA_CryptoInfo_S *pstCryptoInfo, HI_U8 *pPlainText, HI_U32 u32Len, HI_U8 *pCipherText)
{
   HI_S32 ret = HI_FAILURE;

   CA_CheckPointer(pstCryptoInfo);

   ret = CA_AdpInit(pstCryptoInfo);
   if (ret != HI_SUCCESS)
   {
      HI_ERR_CA("Fail to call init function\n");
      return HI_FAILURE;
   }

   ret = CA_DataEncrypt(pPlainText,u32Len,pCipherText);
   if (ret != HI_SUCCESS)
   {
      HI_ERR_CA("Fail to call CA_DataEncrypt\n");
      CA_AdpDeInit();
      return HI_FAILURE;
   }

   CA_AdpDeInit();

   return HI_SUCCESS;
}

HI_S32 HI_CA_ADP_DataDecrypt(HI_CA_CryptoInfo_S *pstCryptoInfo, HI_U8 *pCipherText, HI_U32 u32Len, HI_U8 *pPlainText)
{
   HI_S32 ret = HI_FAILURE;

   CA_CheckPointer(pstCryptoInfo);

   ret = CA_AdpInit(pstCryptoInfo);
   if (ret != HI_SUCCESS)
   {
      HI_ERR_CA("Fail to call init function\n");
      return HI_FAILURE;
   }
   ret = CA_DataDecrypt(pCipherText,u32Len,pPlainText);
   if (ret != HI_SUCCESS)
   {
       HI_ERR_CA("Fail to call CA_DataEncrypt\n");
       CA_AdpDeInit();
      return HI_FAILURE;
   }

   CA_AdpDeInit();

   return HI_SUCCESS;
}


