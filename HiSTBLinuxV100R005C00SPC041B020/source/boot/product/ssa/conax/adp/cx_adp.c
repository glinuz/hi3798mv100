/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : cx_adp.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
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
//#include "hi_common.h"
#include "hi_unf_advca.h"
#include "hi_unf_cipher.h"
#include "cx_debug.h"
#include "cx_verify_def.h"
#include "cx_adp.h"

static HI_U32 CipherBlockSize = 1024*1024-16;   //cipher block must < 1M BYTE and be times of 16BYTE

static HI_HANDLE  g_hCipher = -1;

extern HI_S32 HI_MEM_Alloc(HI_U32 *pu32PhyAddr, HI_U32 Len);

extern HI_S32 HI_CX_GetKeyladderKey(HI_CA_KEY_GROUP_E enKeyGroup, HI_U8 u8KeyladderKey1[16], HI_U8 u8KeyladderKey2[16], HI_U8 u8CipherKey[16]);

HI_S32 CX_AdpInit(HI_CA_CryptoInfo_S *pstCryptoInfo)
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

    /*open and config keyladder*/
    CA_ASSERT(HI_UNF_CIPHER_Init(),ret);
    ret = HI_UNF_ADVCA_Init();
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("Fail to call adva init\n");
        HI_UNF_ADVCA_DeInit();
        return HI_FAILURE;
    }

    ret = HI_CX_GetKeyladderKey(pstCryptoInfo->enKeyGroup, SessionKey1, SessionKey2, (HI_U8*)CipherCtrl.u32Key);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("Fail to get key ladder key\n");
        goto ERROR_EXIT;
    }

    if (pstCryptoInfo->bIsUseKLD == HI_TRUE)
    {    
        ret = HI_UNF_ADVCA_SetR2RAlg(pstCryptoInfo->enKLDAlg);
        ret |= HI_UNF_ADVCA_GetR2RKeyLadderStage(&enStage);    
        ret |= HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1,SessionKey1);
        if(enStage == HI_UNF_ADVCA_KEYLADDER_LEV3)
        {
            ret |= HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV2,SessionKey2);
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
    }
    else if(HI_UNF_CIPHER_ALG_DES == pstCryptoInfo->enCipherAlg)
    {
        CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_DES;
        CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_64BIT;
        CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
        CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_DES_2KEY;
    }
    else if(HI_UNF_CIPHER_ALG_AES == pstCryptoInfo->enCipherAlg)
    {
        CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
        CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
        CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
        CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
    }
    else
    {
        HI_ERR_CA("Cipher Algorithem error\n");
        return -1;
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

HI_VOID CX_AdpDeInit(HI_VOID)
{
    HI_UNF_CIPHER_DestroyHandle(g_hCipher);
    g_hCipher = -1;
    HI_UNF_CIPHER_DeInit();
    HI_UNF_ADVCA_DeInit();
    return;
}

HI_S32 CX_DataEncrypt(HI_U8 *pPlainText, HI_U32 TextLen, HI_U8 *pCipherText)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 CipherBlockNum = 0;
    HI_U32 index = 0;

    CA_CheckPointer(pPlainText);
    CA_CheckPointer(pCipherText);

    if(-1 == g_hCipher)
    {
        HI_ERR_CA("Fail to call init function\n");
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
HI_S32 CX_DataDecrypt(HI_U8 *pCipherText, HI_U32 TextLen, HI_U8 *pPlainText)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 CipherBlockNum = 0;
    HI_U32 index = 0;

    CA_CheckPointer(pPlainText);
    CA_CheckPointer(pCipherText);
    if(-1 == g_hCipher)
    {
        HI_ERR_CA("Fail to call init function\n");
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

/*************************************************************************
 *
 * The function below is added for AES CBC-MAC, for Irdeto MSR1.9 chipset.
 *
 *************************************************************************/
/* For CMAC Calculation */
static HI_U8 const_Rb[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87
};

/* Basic Functions */
static HI_VOID xor_128(HI_U8 *a, HI_U8 *b, HI_U8 *out)
{
    HI_U32 i;

    if(a == NULL || b == NULL || out == NULL)
    {
        HI_ERR_CA("Invalid parameter!\n");
        return;
    }

    for (i = 0; i < 16; i++)
    {
        out[i] = a[i] ^ b[i];
    }
}

/* AES-CMAC Generation Function */
static HI_VOID leftshift_onebit(HI_U8 *input, HI_U8 *output)
{
    HI_S32 i;
    HI_U8 overflow = 0;

    if(input == NULL || output == NULL)
    {
        HI_ERR_CA("Invalid parameter!\n");
        return;
    }

    for ( i = 15; i >= 0; i-- )
    {
        output[i] = input[i] << 1;
        output[i] |= overflow;
        overflow = (input[i] & 0x80) ? 1 : 0;
    }
    return;
}

static HI_S32 generate_subkey(HI_HANDLE hCipherHandle, HI_U8 *K1, HI_U8 *K2)
{
    HI_U8 L[16];
    HI_U8 Z[16];
    HI_U8 tmp[16];
    HI_U32 u32DataLen = 16;
    HI_S32 Ret = HI_SUCCESS;

    if(K1 == NULL || K2 == NULL)
    {
        HI_ERR_CA("Invalid parameter!\n");
        return HI_FAILURE;
    }

    memset(Z, 0x0, sizeof(Z));
    Ret = HI_CIPHER_EncryptEx(hCipherHandle, Z, u32DataLen, L);
    if(Ret != HI_SUCCESS)
    {
        return Ret;
    }

    if ( (L[0] & 0x80) == 0 ) /* If MSB(L) = 0, then K1 = L << 1 */
    {
        leftshift_onebit(L, K1);
    }
    else  /* Else K1 = ( L << 1 ) (+) Rb */
    {
        leftshift_onebit(L, tmp);
        xor_128(tmp, const_Rb, K1);
    }

    if ( (K1[0] & 0x80) == 0 )
    {
        leftshift_onebit(K1,K2);
    }
    else
    {
        leftshift_onebit(K1, tmp);
        xor_128(tmp, const_Rb, K2);
    }

    return HI_SUCCESS;
}

static HI_VOID padding ( HI_U8 *lastb, HI_U8 *pad, HI_U32 length )
{
    HI_U32 j;

    if(lastb == NULL || pad == NULL)
    {
        HI_ERR_CA("Invalid parameter!\n");
        return;
    }

    /* original last block */
    for ( j = 0; j < 16; j++ )
    {
        if ( j < length )
        {
            pad[j] = lastb[j];
        }
        else if ( j == length )
        {
            pad[j] = 0x80;
        }
        else
        {
            pad[j] = 0x00;
        }
    }
}

HI_S32 HI_CA_CalcMAC(HI_U8 *pu8Key, HI_U8 *pInputData, HI_U32 u32InputDataLen,
                                        HI_U8 *pOutputMAC, HI_BOOL bIsLastBlock)
{
    HI_U8 X[16], M_last[16], padded[16];
    static HI_U8 K1[16], K2[16];
    HI_U32 n, i, flag;
    HI_U8 u8TmpBuf[16];
    HI_S32 Ret = HI_SUCCESS;
    HI_UNF_CIPHER_CTRL_S stCipherCtrl;
    static HI_BOOL bIsFirstBlock = HI_TRUE;
    HI_S32 ret = HI_SUCCESS;
    static HI_HANDLE hCipherHandle;
    HI_UNF_CIPHER_ATTS_S stCipherAtts;
    static HI_U32 u32MmzPhyAddr = 0;
    static HI_BOOL bIsDstMmzInit = HI_FALSE;
	HI_U8 au8SessionKey1[16] = {0x23,0x34,0x34,0x52,0x34,0x55,0x45,0x84,0x54,0x54,0x84,0x53,0x34,0x47,0x34,0x47};
	HI_U8 au8SessionKey2[16] = {0x56,0x34,0x88,0x52,0x34,0x89,0x45,0xa0,0x54,0x54,0x77,0x53,0x34,0x47,0x34,0x91};
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage = 0;

     if(pu8Key == NULL || pInputData == NULL || pOutputMAC == NULL)
    {
        HI_ERR_CA("Invalid parameter!\n");
        return -1;
    }

  
    if(bIsFirstBlock)
    {
        CA_ASSERT(HI_UNF_CIPHER_Init(), ret);
        HI_UNF_ADVCA_Init();

        CA_ASSERT(HI_UNF_ADVCA_SetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_AES),ret);    
        CA_ASSERT(HI_UNF_ADVCA_GetR2RKeyLadderStage(&enStage),ret);

        if(enStage >= HI_UNF_ADVCA_KEYLADDER_LEV2)
        {
        	ret = HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1, au8SessionKey1);
            if (ret != HI_SUCCESS)
            {
        		HI_ERR_CA("Fail to set R2R session key, level %d\n", HI_UNF_ADVCA_KEYLADDER_LEV1);
                return HI_FAILURE;
            }
        }

        if(enStage >= HI_UNF_ADVCA_KEYLADDER_LEV3)
        {
        	ret = HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV2, au8SessionKey2);
            if (ret != HI_SUCCESS)
            {
               HI_ERR_CA("Fail to set R2R session key, level %d\n", HI_UNF_ADVCA_KEYLADDER_LEV2);
               return HI_FAILURE;
            }
        }
                
        memset(&stCipherAtts, 0, sizeof(HI_UNF_CIPHER_ATTS_S));
        stCipherAtts.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
        CA_ASSERT(HI_UNF_CIPHER_CreateHandle(&hCipherHandle, &stCipherAtts),ret);
        memcpy(stCipherCtrl.u32Key, pu8Key, 16);
        memset(stCipherCtrl.u32IV, 0, sizeof(stCipherCtrl.u32IV));
        stCipherCtrl.bKeyByCA = HI_TRUE;
        stCipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
        stCipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
        stCipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
        stCipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
        stCipherCtrl.stChangeFlags.bit1IV = 1;
        stCipherCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;       
        CA_ASSERT(HI_UNF_CIPHER_ConfigHandle(hCipherHandle, &stCipherCtrl), ret);

        Ret = generate_subkey(hCipherHandle, K1, K2);
        if(Ret != HI_SUCCESS)
        {
            return Ret;
        }

        //After genreate the subkey, reset the configure handle
        memcpy(stCipherCtrl.u32Key, pu8Key, 16);
        memset(stCipherCtrl.u32IV, 0, sizeof(stCipherCtrl.u32IV));
        stCipherCtrl.bKeyByCA = HI_TRUE;
        stCipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
        stCipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
        stCipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
        stCipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
        stCipherCtrl.stChangeFlags.bit1IV = 1;
        stCipherCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
        CA_ASSERT(HI_UNF_CIPHER_ConfigHandle(hCipherHandle, &stCipherCtrl), ret);
        bIsFirstBlock = HI_FALSE;
    }

    if(!bIsDstMmzInit)
    {
        Ret = HI_MEM_Alloc(&u32MmzPhyAddr, MAX_DATA_LEN);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_CA("malloc input memory failed!\n");
            return HI_FAILURE;
        }
	    memset((void *)u32MmzPhyAddr, 0, MAX_DATA_LEN);

        bIsDstMmzInit = HI_TRUE;
    }

    if(!bIsLastBlock)
    {
       Ret = HI_CIPHER_EncryptEx(hCipherHandle, pInputData, u32InputDataLen, (HI_U8 *)u32MmzPhyAddr); /* X := AES-128(KEY, Y); */
       memcpy(u8TmpBuf, (HI_U8 *)(u32MmzPhyAddr + u32InputDataLen - 16), 16);
       if(Ret != HI_SUCCESS)
       {
            return Ret;
       }
    }
    else
    {
        bIsFirstBlock = HI_TRUE;

        n = (u32InputDataLen + 15) / 16; /* n is number of rounds */
        if ( n == 0 )
        {
            n = 1;
            flag = 0;
        }
        else
        {
            if ( (u32InputDataLen % 16) == 0 ) /* last block is a complete block */
            {
                flag = 1;
            }
            else /* last block is not complete block */
            {
                flag = 0;
            }
        }

        if ( flag )  /* last block is complete block */
        {
            xor_128(&pInputData[16 * (n - 1)], K1, M_last);
        }
        else
        {
            padding(&pInputData[16 * (n - 1)], padded, u32InputDataLen % 16);
            xor_128(padded, K2, M_last);
        }

        if(n > 1)
        {
           Ret = HI_CIPHER_EncryptEx(hCipherHandle, pInputData, 16 * (n - 1), (HI_U8 *)u32MmzPhyAddr); /* X := AES-128(KEY, Y); */
           memcpy(u8TmpBuf, (HI_U8 *)(u32MmzPhyAddr + 16 * (n - 1) - 16), 16);
           if(Ret != HI_SUCCESS)
           {
                return Ret;
           }
        }

        HI_CIPHER_EncryptEx(hCipherHandle, M_last, 16, X);
        if(Ret != HI_SUCCESS)
        {
            return Ret;
        }

        for ( i = 0; i < 16; i++ )
        {
            pOutputMAC[i] = X[i];
        }

        CA_ASSERT(HI_UNF_CIPHER_DestroyHandle(hCipherHandle), ret);
    }

    return HI_SUCCESS;
}
/**********************End CBC-MAC function*******************************************************************/

/*******************The following are exported infaces for usage*******************************************/
HI_S32 HI_CX_ADP_DataEncrypt(HI_CA_CryptoInfo_S *pstCryptoInfo, HI_U8 *pPlainText, HI_U32 u32Len, HI_U8 *pCipherText)
{
   HI_S32 ret = HI_FAILURE;

   CA_CheckPointer(pstCryptoInfo);

   ret = CX_AdpInit(pstCryptoInfo);
   if (ret != HI_SUCCESS)
   {
      HI_ERR_CA("Fail to call ca init function\n");
      return HI_FAILURE;
   }
   
   ret = CX_DataEncrypt(pPlainText,u32Len,pCipherText);
   if (ret != HI_SUCCESS)
   {
      HI_ERR_CA("Fail to call CA_DataEncrypt\n");
      CX_AdpDeInit();
      return HI_FAILURE;
   }
   
   CX_AdpDeInit();

   return HI_SUCCESS;
}

HI_S32 HI_CX_ADP_DataDecrypt(HI_CA_CryptoInfo_S *pstCryptoInfo, HI_U8 *pCipherText, HI_U32 u32Len, HI_U8 *pPlainText)
{
   HI_S32 ret = HI_FAILURE;

   CA_CheckPointer(pstCryptoInfo);

   ret = CX_AdpInit(pstCryptoInfo);
   if (ret != HI_SUCCESS)
   {
      HI_ERR_CA("Fail to call ca init function\n");
      return HI_FAILURE;
   }
   ret = CX_DataDecrypt(pCipherText,u32Len,pPlainText);
   if (ret != HI_SUCCESS)
   {
       HI_ERR_CA("Fail to call CA_DataEncrypt\n");
       CX_AdpDeInit();
      return HI_FAILURE;
   }
   
   CX_AdpDeInit();  

   return HI_SUCCESS;
}

HI_S32 HI_CX_ADP_CalcAesCMAC(HI_U8 *pInputData, HI_U32 u32Len, HI_U8 pu8Key[16], HI_U8 pOutputMAC[16])
{
    if(pu8Key == NULL || pInputData == NULL || pOutputMAC == NULL)
    {
        HI_ERR_CA("Invalid parameter!\n");
        return -1;
    }
    
    return HI_CA_CalcMAC(pu8Key, pInputData, u32Len, pOutputMAC, HI_TRUE);
}


