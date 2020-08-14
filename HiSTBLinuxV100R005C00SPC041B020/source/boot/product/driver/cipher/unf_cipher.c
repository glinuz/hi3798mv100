/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : unf_cipher.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2014-09-16
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef HI_MINIBOOT_SUPPORT
    #include <common.h>
#else
    #include "string.h"
#endif
#include "drv_cipher_intf.h"
#define RSA_SIGN        1
#define ASN1_HASH_SHA1    "\x30\x21\x30\x09\x06\x05\x2b\x0e\x03\x02\x1a\x05\x00\x04\x14"
#define ASN1_HASH_SHA256  "\x30\x31\x30\x0d\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x01\x05\x00\x04\x20"
static const HI_S8 EMPTY_L_SHA1[]   = "\xda\x39\xa3\xee\x5e\x6b\x4b\x0d\x32\x55\xbf\xef\x95\x60\x18\x90\xaf\xd8\x07\x09";
static const HI_S8 EMPTY_L_SHA256[] = "\xe3\xb0\xc4\x42\x98\xfc\x1c\x14\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24\x27\xae\x41\xe4\x64\x9b\x93\x4c\xa4\x95\x99\x1b\x78\x52\xb8\x55";

HI_S32 HI_UNF_CIPHER_Init(HI_VOID)
{
	return Cipher_Init();
}

HI_S32 HI_UNF_CIPHER_DeInit(HI_VOID)
{
	(HI_VOID)Cipher_Exit();

	return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_CreateHandle(HI_HANDLE * phCipher, const HI_UNF_CIPHER_ATTS_S * pstCipherAttr)
{
	return Cipher_CreateHandle(phCipher, pstCipherAttr);
}

HI_S32 HI_UNF_CIPHER_DestroyHandle(HI_HANDLE hCipher)
{
	return Cipher_DestroyHandle(hCipher);
}

HI_S32 HI_UNF_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S * pstCtrl)
{
	return Cipher_ConfigHandle(hCipher, pstCtrl);
}

HI_S32 HI_UNF_CIPHER_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
	return Cipher_Encrypt(hCipher, u32SrcPhyAddr, u32DestPhyAddr, u32ByteLength);
}

HI_S32 HI_UNF_CIPHER_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
	return Cipher_Decrypt(hCipher, u32SrcPhyAddr, u32DestPhyAddr, u32ByteLength);
}

HI_S32 HI_UNF_CIPHER_HashInit(HI_UNF_CIPHER_HASH_ATTS_S * pstHashAttr, HI_HANDLE * pHashHandle)
{
	return Cipher_HashInit(pstHashAttr, pHashHandle);
}

HI_S32 HI_UNF_CIPHER_HashUpdate(HI_HANDLE hHashHandle, HI_U8 * pu8InputData, HI_U32 u32InputDataLen)
{
	return Cipher_HashUpdate(hHashHandle, pu8InputData, u32InputDataLen);
}

HI_S32 HI_UNF_CIPHER_HashFinal(HI_HANDLE hHashHandle, HI_U8 * pu8OutputHash)
{
	return Cipher_HashFinal(hHashHandle, pu8OutputHash);
}

/* for test, encrypt data buffer by a given cipher handle, in a loop inside */
HI_S32 HI_CIPHER_EncryptEx(HI_HANDLE hCipherHandle, HI_U8 *input, HI_U32 datalen, HI_U8 *output)
{
    HI_U32 u32EncryptDataLen = 0;
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 u32LeftDataLen;
    HI_U32 i;
    HI_U32 u32BlockNum;

    u32LeftDataLen = datalen;
    u32BlockNum = (datalen + MAX_DATA_LEN - 1) / MAX_DATA_LEN;
    for(i = 0; i < u32BlockNum; i++)
    {
        u32EncryptDataLen = u32LeftDataLen >= MAX_DATA_LEN ? MAX_DATA_LEN : u32LeftDataLen;
        u32LeftDataLen -= u32EncryptDataLen;

        Ret = HI_UNF_CIPHER_Encrypt(hCipherHandle, (HI_U32)(input + i * MAX_DATA_LEN), (HI_U32)(output + i * MAX_DATA_LEN), u32EncryptDataLen);
        if(Ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("Cipher encrypt failed!\n");
            return Ret;
        }
    }

    return Ret;
}

HI_S32 HI_UNF_CIPHER_GetRandomNumber(HI_U32 *pu32RandomNumber)
{
    if (NULL == pu32RandomNumber)
    {
        HI_ERR_CIPHER("Input pointer is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    return Cipher_GetRandomNumber(pu32RandomNumber);
}

HI_S32 HI_CIPHER_SHA(HI_UNF_CIPHER_HASH_TYPE_E enType,
						const unsigned char *input,
						int iBufLen,
						unsigned char *output,
						int oBufLen)
{
    HI_S32 Ret = HI_SUCCESS;
    
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_HANDLE HashHandle = 0;

	if(NULL == input || output == NULL)
	{
		HI_ERR_CIPHER("Invalid params, null pointer!\n");
		return HI_FAILURE;
	}
	
    if(HI_UNF_CIPHER_HASH_TYPE_SHA1 != enType
    && HI_UNF_CIPHER_HASH_TYPE_SHA256 != enType)
    {
		HI_ERR_CIPHER("Not supported hash type(%d)!\n", enType);
		return HI_FAILURE;
    }

    if(HI_UNF_CIPHER_HASH_TYPE_SHA1 == enType)
    {
		if(oBufLen < 20)
		{
			HI_ERR_CIPHER("Hash output buffer(%d) too small!\n", oBufLen);
			return HI_FAILURE;
		}
    }

    if(HI_UNF_CIPHER_HASH_TYPE_SHA256 == enType)
    {
		if(oBufLen < 32)
		{
			HI_ERR_CIPHER("Hash output buffer(%d) too small!\n", oBufLen);
			return HI_FAILURE;
		}
    }

    memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));
    stHashAttr.eShaType = enType;
    stHashAttr.u32TotalDataLen = iBufLen;
    Ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &HashHandle);
    if(Ret != HI_SUCCESS)
    {
    	HI_ERR_CIPHER("HI_UNF_CIPHER_HashInit failed!\n");
        HashHandle = -1;
        return Ret;
    }

    Ret = HI_UNF_CIPHER_HashUpdate(HashHandle, (HI_U8*)input, iBufLen);
    if(Ret != HI_SUCCESS)
    {
    	HI_ERR_CIPHER("HI_UNF_CIPHER_HashUpdate failed!\n");
        HashHandle = -1;
        return Ret;
    }
    Ret = HI_UNF_CIPHER_HashFinal(HashHandle, output);
    if(Ret != HI_SUCCESS)
    {
    	HI_ERR_CIPHER("HI_UNF_CIPHER_HashFinal failed!\n");
        HashHandle = -1;
        return Ret;
    }
    
    return HI_SUCCESS;
}

static HI_S32 RSA_GetAttr(HI_U32 u32SchEme, 
                          HI_U16 u16NLen,
                          HI_U32 *pu32HLen,
                          HI_U32 *pu32KeyLen,
                          HI_UNF_CIPHER_HASH_TYPE_E *pstenHashType)
{
    HI_S32 ret = HI_SUCCESS;

    
    if ((pu32HLen == HI_NULL) || (pu32KeyLen == HI_NULL) || (pstenHashType == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
    
    *pu32HLen = 0;
    *pu32KeyLen = 0;
    *pstenHashType = HI_UNF_CIPHER_HASH_TYPE_BUTT;

    if (u16NLen <= 512)
    {
        *pu32KeyLen = u16NLen;
    }
    else 
    {
        HI_ERR_CIPHER("u16NLen(0x%x) is invalid\n", u16NLen);
        return HI_ERR_CIPHER_INVALID_POINT;
    }
    
    switch(u32SchEme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:         
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5:
        *pu32HLen = 0;
        *pstenHashType = HI_UNF_CIPHER_HASH_TYPE_BUTT;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1:     
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1:
        *pu32HLen = 20;
        *pstenHashType = HI_UNF_CIPHER_HASH_TYPE_SHA1;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256:
        *pu32HLen = 32;
        *pstenHashType = HI_UNF_CIPHER_HASH_TYPE_SHA256;
        break;
    default:
        HI_ERR_CIPHER("RSA scheme (0x%x) is invalid.\n", u32SchEme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
    }

    return ret;
}

static HI_S32 RSA_GetRandomNumber(HI_U8 *pu8Rand, HI_U32 u32Size)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 i;
    HI_U32 u32Rand;

    memset(pu8Rand, 0, u32Size); 
    for(i=0; i<u32Size; i+=4)
    {
        ret = HI_UNF_CIPHER_GetRandomNumber(&u32Rand);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("Get random number failed, ret = %x\n", ret);
            return HI_FAILURE;
        }
        pu8Rand[i+3]   = (HI_U8)(u32Rand >> 24)& 0xFF;
        pu8Rand[i+2] = (HI_U8)(u32Rand >> 16)& 0xFF;
        pu8Rand[i+1] = (HI_U8)(u32Rand >> 8)& 0xFF;
        pu8Rand[i+0] = (HI_U8)(u32Rand) & 0xFF;
    }

    /*non-zero random octet string*/
    for(i=0; i<u32Size; i++)
    {
        if (pu8Rand[i] == 0x00)
        {
            ret = HI_UNF_CIPHER_GetRandomNumber(&u32Rand);
            if (ret != HI_SUCCESS)
            {
                HI_ERR_CIPHER("Get random number failed, ret = %x\n", ret);
                return HI_FAILURE;
            }
            pu8Rand[i] = (HI_U8)(u32Rand) & 0xFF;
            i = 0;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 RSA_PKCS1_MGF1(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                             HI_U8  *pu8Seed,  
                             HI_U32 u32Seedlen, 
                             HI_U8 *pu8Mask, HI_U32 u32MaskLen)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 i,j, u32Outlen = 0;
    HI_U8 u8Cnt[4];
    HI_U8 u8Md[32];
    HI_U32 u32MdLen;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_HANDLE HashHandle;

    if ((pu8Seed == HI_NULL) || (pu8Mask == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));

    /*PKCS#1 V2.1 only use sha1 function, Others allow for future expansion*/
    stHashAttr.eShaType = enHashType;
    if( enHashType == HI_UNF_CIPHER_HASH_TYPE_SHA1)
    {
        u32MdLen = 20;
    }
    else if( enHashType == HI_UNF_CIPHER_HASH_TYPE_SHA256)
    {
        u32MdLen = 32;
    }
    else
    {
        HI_ERR_CIPHER("hash type is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    for (i = 0; u32Outlen < u32MaskLen; i++) 
    {
        u8Cnt[0] = (HI_U8)((i >> 24) & 0xFF);
        u8Cnt[1] = (HI_U8)((i >> 16) & 0xFF);
        u8Cnt[2] = (HI_U8)((i >> 8)) & 0xFF;
        u8Cnt[3] = (HI_U8)(i & 0xFF);

        stHashAttr.u32TotalDataLen = u32Seedlen + 4;
        Ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &HashHandle);
        if (Ret != HI_SUCCESS)
        {
    		return Ret;
        }
        Ret = HI_UNF_CIPHER_HashUpdate(HashHandle, pu8Seed, u32Seedlen);
        if (Ret != HI_SUCCESS)
        {
    		return Ret;
        }
        Ret = HI_UNF_CIPHER_HashUpdate(HashHandle, u8Cnt, 4);
        if (Ret != HI_SUCCESS)
        {
    		return Ret;
        }
        Ret = HI_UNF_CIPHER_HashFinal(HashHandle, u8Md);
        if (Ret != HI_SUCCESS)
        {
    		return Ret;
        }
        for(j=0; (j<u32MdLen) && (u32Outlen < u32MaskLen); j++)
		{
			pu8Mask[u32Outlen++] ^= u8Md[j];	
		}
    }

    return HI_SUCCESS;
}

/*PKCS #1: EME-OAEP encoding*/
/*************************************************************
                    +----------+---------+--+-------+
               DB = |  lHash   |    PS   |01|   M   |
                    +----------+---------+--+-------+
                                   |
         +----------+              V
         |   seed   |--> MGF ---> xor
         +----------+              |
               |                   |
      +--+     V                   |
      |00|    xor <----- MGF <-----|
      +--+     |                   |
        |      |                   |
        V      V                   V
      +--+----------+----------------------------+
EM =  |00|maskedSeed|          maskedDB          |
      +--+----------+----------------------------+
       1     hlen              k - hlen- 1          

so: PS_LEN = k - hlen - 1 -  (hlen  + mlen + 1) = k - 2hlen - mlen - 2 > 0
so: mlen < k - 2hlen - 2 
*************************************************************/
static HI_S32 RSA_padding_add_PKCS1_OAEP(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                                  HI_U32 u32HLen,
                                  HI_U32 u32KeyLen,
                                  HI_U8 *pu8Input,
                                  HI_U32 u32InLen,
                                  HI_U8 *pu8Output,
                                  HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32DBLen;
    HI_U8 *pu8DB;
    HI_U8 *pu8Seed;
    const HI_S8 *pu8LHASH = EMPTY_L_SHA1;

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    /*In the v2.1 of PKCS #1, L is the empty string;    */
    /*other uses outside the scope of rsa specifications*/
    if( enHashType == HI_UNF_CIPHER_HASH_TYPE_SHA256)
    {   
        pu8LHASH = EMPTY_L_SHA256;
    }

    if (u32InLen > u32KeyLen - 2 * u32HLen - 2)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    *pu32OutLen = 0;
    pu8Output[0] = 0;
    pu8Seed = pu8Output + 1;
    pu8DB = pu8Output + u32HLen + 1;
    u32DBLen = u32KeyLen - u32HLen -1;
        
    memcpy(pu8DB, pu8LHASH, u32HLen);                                 /*set lHash*/
    memset(&pu8DB[u32HLen], 0, u32DBLen - u32InLen - u32HLen -1);     /*set PS with 0x00*/
    pu8DB[u32DBLen - u32InLen - 1] = 0x01;                            /*set 0x01 after PS*/
    memcpy(&pu8DB[u32DBLen - u32InLen], pu8Input, u32InLen);          /*set M*/
    ret = RSA_GetRandomNumber(pu8Seed, u32HLen);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_GetRandomNumber failed, ret = %x\n", ret);
        return HI_FAILURE;
    }
    ret = RSA_PKCS1_MGF1(enHashType, pu8Seed, u32HLen, pu8DB, u32KeyLen - u32HLen - 1);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return HI_FAILURE;
    }
    ret = RSA_PKCS1_MGF1(enHashType, pu8DB, u32KeyLen - u32HLen - 1, pu8Seed, u32HLen);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return HI_FAILURE;
    }
    *pu32OutLen = u32KeyLen;

     return HI_SUCCESS;
}

/*PKCS #1: RSAES-PKCS1-V1_5-ENCRYPT*/
/*************************************************
EM = 0x00 || 0x02 || PS || 0x00 || M

PS_LEN > 8, mlen < u32KeyLen - 11
*************************************************/
static HI_S32 RSA_padding_add_PKCS1_V15( HI_U32 u32KeyLen,
                                 HI_U8 *pu8Input,
                                 HI_U32 u32InLen,
                                 HI_U8 *pu8Output,
                                 HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32Index = 0;


    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32InLen > u32KeyLen - 11)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    *pu32OutLen = 0;
    pu8Output[u32Index++] = 0x00;
    pu8Output[u32Index++] = 0x02;
    ret = RSA_GetRandomNumber(&pu8Output[u32Index], u32KeyLen - u32InLen -3);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_GetRandomNumber failed, ret = %x\n", ret);
        return HI_FAILURE;
    }

    u32Index+=u32KeyLen - u32InLen -3;
    pu8Output[u32Index++] = 0x00;
    memcpy(&pu8Output[u32Index], pu8Input, u32InLen);

    *pu32OutLen = u32KeyLen;
    
    return HI_SUCCESS;
}

/*PKCS #1: block type 0,1,2 message padding*/
/*************************************************
EB = 00 || BT || PS || 00 || D

PS_LEN >= 8, mlen < u32KeyLen - 11
*************************************************/
static HI_S32 RSA_padding_add_PKCS1_type(HI_U32 u32KeyLen,
                               HI_U8  u8BT,
                               HI_U8 *pu8Input,
                               HI_U32 u32InLen,
                               HI_U8 *pu8Output,
                               HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32PLen;
    HI_U8 *pu8EB;

    if (u32InLen > u32KeyLen - 11)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    *pu32OutLen =0;
    pu8EB = pu8Output;
    
    *(pu8EB++) = 0;
    *(pu8EB++) = u8BT;                 /* Private Key BT (Block Type) */

    /* pad out with 0xff data */
    u32PLen = u32KeyLen - 3 - u32InLen;
    switch(u8BT)
    {
    case 0x00:
        memset(pu8EB, 0x00, u32PLen);
        break;
    case 0x01:
        memset(pu8EB, 0xFF, u32PLen);
        break;
    case 0x02:
        ret = RSA_GetRandomNumber(pu8EB, u32PLen);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("RSA_GetRandomNumber failed, ret = %x\n", ret);
            return HI_FAILURE;
        }
        break;
    default:
        HI_ERR_CIPHER("BT(0x%x) is invalid.\n", u8BT);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pu8EB += u32PLen;
    *(pu8EB++) = 0x00;
    memcpy(pu8EB, pu8Input, u32InLen);

    *pu32OutLen = u32KeyLen;
    
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_RsaPublicEncrypt(HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaEnc, 
								  HI_U8 *pu8Input, HI_U32 u32InLen, 
								  HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_RSA_DATA_S stRsaData;
    HI_U32 u32HLen;
    HI_U32 u32KeyLen;
    HI_U8  u8BT;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;
    HI_U8  u8EM[CIPHER_MAX_RSA_KEY_LEN];

    if ((pstRsaEnc == HI_NULL) || (pu8Input == HI_NULL) 
        || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32InLen == 0)
    {
        HI_ERR_CIPHER("inlen is 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }
  
    ret = RSA_GetAttr(pstRsaEnc->enScheme, pstRsaEnc->stPubKey.u16NLen, 
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if (u32InLen > u32KeyLen)
    {
        HI_ERR_CIPHER("inlen is too large.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    memset(u8EM, 0, sizeof(u8EM));

    switch(pstRsaEnc->enScheme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING:
        /*if u32InLen < u32KeyLen, padding 0 before input data*/
        *pu32OutLen = u32KeyLen;
        memcpy(u8EM+(u32KeyLen - u32InLen), pu8Input, u32InLen);
        ret = HI_SUCCESS;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaEnc->enScheme);
        HI_ERR_CIPHER("For a public key encryption operation, the block type shall be 02.\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2:
        u8BT = (HI_U8)(pstRsaEnc->enScheme - HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0);
        ret = RSA_padding_add_PKCS1_type(u32KeyLen, u8BT, pu8Input, u32InLen, u8EM, pu32OutLen);
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256:
        ret = RSA_padding_add_PKCS1_OAEP(enHashType, u32HLen, u32KeyLen, pu8Input, u32InLen, u8EM, pu32OutLen);
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5:
        ret = RSA_padding_add_PKCS1_V15(u32KeyLen, pu8Input, u32InLen, u8EM, pu32OutLen);
        break;
    default:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaEnc->enScheme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA padding error, ret = 0x%x.\n", ret);
        return ret;
    }

    if(*pu32OutLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32OutLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    stRsaData.pu8N = pstRsaEnc->stPubKey.pu8N;
    stRsaData.pu8K = pstRsaEnc->stPubKey.pu8E;
    stRsaData.u16NLen = pstRsaEnc->stPubKey.u16NLen;
    stRsaData.u16KLen = pstRsaEnc->stPubKey.u16ELen;
    stRsaData.pu8Input = u8EM;
    stRsaData.pu8Output = pu8Output;
    stRsaData.u32DataLen = u32KeyLen;

    ret = Cipher_CalcRsa(&stRsaData);
    
    return ret;
}

static HI_S32 RSA_padding_check_PKCS1_OAEP(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                                  HI_U32 u32HLen,
                                  HI_U32 u32KeyLen,
                                  HI_U8 *pu8Input, 
                                  HI_U32 u32InLen,
                                  HI_U8 *pu8Output,
                                  HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 i;
    const HI_S8 *pu8LHASH = EMPTY_L_SHA1;
    HI_U8 *pu8Seed;
    HI_U8 *pu8DB;
    HI_U8 *pu8MaskedDB;

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu8Output == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if(enHashType == HI_UNF_CIPHER_HASH_TYPE_SHA256)
    {   
        pu8LHASH = EMPTY_L_SHA256;
    }

    if (u32InLen != u32KeyLen)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (u32KeyLen < 2 * u32HLen + 2)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(pu8Input[0] != 0x00)
    {
        HI_ERR_CIPHER("EM[0] != 0.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    *pu32OutLen = 0;
    pu8MaskedDB= pu8Input + u32HLen + 1;
    pu8Seed = pu8Input + 1;
    pu8DB = pu8Input + u32HLen + 1;
    ret = RSA_PKCS1_MGF1(enHashType, pu8MaskedDB, u32KeyLen - u32HLen - 1, pu8Seed, u32HLen);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return HI_FAILURE;
    }
    ret = RSA_PKCS1_MGF1(enHashType, pu8Seed, u32HLen, pu8DB, u32KeyLen - u32HLen - 1);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return HI_FAILURE;
    }
    if(memcmp(pu8DB, pu8LHASH, u32HLen) != 0)
    {
        HI_ERR_CIPHER("LHASH error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    for(i=u32HLen; i < u32KeyLen - u32HLen - 1; i++)
    {
        if((pu8DB[i] == 0x01)) 
        {
            memcpy(pu8Output, pu8DB+i+1, u32KeyLen - u32HLen - i - 2); 
            *pu32OutLen = u32KeyLen - u32HLen - i - 2;
            break;
        } 
    }
    if (i >= u32KeyLen - u32HLen - 1)
    {
        HI_ERR_CIPHER("PS error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    return HI_SUCCESS;
}

static HI_S32 RSA_padding_check_PKCS1_V15(HI_U32 u32KeyLen,
                                          HI_U8 *pu8Input, 
                                          HI_U32 u32InLen,
                                          HI_U8 *pu8Output,
                                          HI_U32 *pu32OutLen)
{
    HI_U32 u32Index = 0;

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu8Output == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((u32InLen != u32KeyLen) || (u32KeyLen < 11))
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    
    if(pu8Input[u32Index] != 0x00)
    {
        HI_ERR_CIPHER("EM[0] != 0x00.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
    u32Index++;
    if(pu8Input[u32Index] != 0x02)
    {
        HI_ERR_CIPHER("EM[1] != 0x02.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
    u32Index++;
    for( ; u32Index < u32KeyLen; u32Index++)
    {
        if( (u32Index >= 10) //The length of PS is large than 8 octets
            && (pu8Input[u32Index] == 0x00)) 
        {
            memcpy(pu8Output, &pu8Input[u32Index+1], u32KeyLen - 1 - u32Index); 
            *pu32OutLen = u32KeyLen - 1 - u32Index;
            break;
        } 
    }
    if (u32Index >= u32KeyLen)
    {
        HI_ERR_CIPHER("PS error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
  
    return HI_SUCCESS;
}

static HI_S32 RSA_padding_check_PKCS1_type(HI_U32 u32KeyLen,
                               HI_U8  u8BT,
                               HI_U8 *pu8Input, 
                               HI_U32 u32InLen,
                               HI_U8 *pu8Output,
                               HI_U32 *pu32OutLen)
{
    HI_U8 *pu8EB;

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
	
	if (u32InLen != u32KeyLen)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    *pu32OutLen = 0x00;
    pu8EB = pu8Input;
    if(*pu8EB != 0x00)
    {
        HI_ERR_CIPHER("EB[0] != 0x00.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
    pu8EB++;
    if(*pu8EB != u8BT)
    {
        HI_ERR_CIPHER("EB[1] != BT(0x%x).\n", u8BT);
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
    pu8EB++;
    switch(u8BT)
    {
    case 0x00:
        for( ; pu8EB < pu8Input + u32InLen - 1; pu8EB++)
        {
            if( (*pu8EB == 0x00) && (*(pu8EB+1) != 0)) 
            {
                break;
            } 
        }
        break;
    case 0x01:
        for( ; pu8EB < pu8Input + u32InLen - 1; pu8EB++)
        {
            if(*pu8EB == 0xFF)
            {
                continue;
            }
            else if (*pu8EB == 0x00)
            {
                break;
            }
            else
            {
                pu8EB = pu8Input + u32InLen - 1;
                break;
            }
        }
        break;
    case 0x02:
        for( ; pu8EB < pu8Input + u32InLen - 1; pu8EB++)
        {
            if(*pu8EB == 0x00)
            {
                break; 
            }   
        }
        break;
    default:
        HI_ERR_CIPHER("BT(0x%x) is invalid.\n", u8BT);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(pu8EB >= (pu8Input + u32InLen - 1))
    {
        HI_ERR_CIPHER("PS Error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    pu8EB++;
    *pu32OutLen = pu8Input + u32KeyLen - pu8EB;
    memcpy(pu8Output, pu8EB, *pu32OutLen); 
    
    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_RsaPublicDecrypt(HI_UNF_CIPHER_RSA_PUB_ENC_S *pstRsaDec,
							   HI_U8 *pu8Input, HI_U32 u32InLen,
							   HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_RSA_DATA_S stRsaData;
    HI_U8 u8EM[CIPHER_MAX_RSA_KEY_LEN];
    HI_U32 u32HLen;
    HI_U32 u32KeyLen;
    HI_U8  u8BT;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;

    if ((pstRsaDec == HI_NULL) || (pu8Input == HI_NULL) 
        || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    ret = RSA_GetAttr(pstRsaDec->enScheme, pstRsaDec->stPubKey.u16NLen, 
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if(u32InLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32InLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
    
    stRsaData.pu8N = pstRsaDec->stPubKey.pu8N;
    stRsaData.pu8K = pstRsaDec->stPubKey.pu8E;
    stRsaData.u16NLen = pstRsaDec->stPubKey.u16NLen;
    stRsaData.u16KLen = pstRsaDec->stPubKey.u16ELen;
    stRsaData.pu8Input = pu8Input;
    stRsaData.pu8Output = u8EM;
    stRsaData.u32DataLen = u32KeyLen;

    ret = Cipher_CalcRsa(&stRsaData);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("CMD_CIPHER_CALCRSA failed, ret = %x\n", ret);
        return HI_FAILURE;
    }    
    
    switch(pstRsaDec->enScheme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING:
        *pu32OutLen = u32InLen;
        memcpy(pu8Output, u8EM, u32KeyLen);
        ret = HI_SUCCESS;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        u8BT = (HI_U8)(pstRsaDec->enScheme - HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0);
        ret = RSA_padding_check_PKCS1_type(u32KeyLen, u8BT, u8EM, u32InLen, pu8Output, pu32OutLen);
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaDec->enScheme);
        HI_ERR_CIPHER("For a public key decryption operation, the block type shall be 00 or 01.\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    default:
        HI_ERR_CIPHER("RSA scheme error, scheme = 0x%x.\n", pstRsaDec->enScheme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA padding error, ret = 0x%x.\n", ret);
        return ret;
    }
    
    return ret;
}

HI_S32 HI_UNF_CIPHER_RsaPrivateDecrypt(HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaDec,								  
								   HI_U8 *pu8Input, HI_U32 u32InLen, 
								   HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_RSA_DATA_S stRsaData;
    HI_U8 u8EM[CIPHER_MAX_RSA_KEY_LEN];
    HI_U32 u32HLen;
    HI_U32 u32KeyLen;
    HI_U8  u8BT;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;
    
    if ((pstRsaDec == HI_NULL) || (pu8Input == HI_NULL) 
        || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    ret = RSA_GetAttr(pstRsaDec->enScheme, pstRsaDec->stPriKey.u16NLen, 
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if(u32InLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32InLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
    
    stRsaData.pu8N = pstRsaDec->stPriKey.pu8N;
    stRsaData.pu8K = pstRsaDec->stPriKey.pu8D;
    stRsaData.u16NLen = pstRsaDec->stPriKey.u16NLen;
    stRsaData.u16KLen = pstRsaDec->stPriKey.u16DLen;
    stRsaData.pu8Input = pu8Input;
    stRsaData.pu8Output = u8EM;
    stRsaData.u32DataLen = u32KeyLen;

    ret = Cipher_CalcRsa(&stRsaData);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("CMD_CIPHER_CALCRSA failed, ret = %x\n", ret);
        return HI_FAILURE;
    }    
    
    switch(pstRsaDec->enScheme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING:
        *pu32OutLen = u32InLen;
        memcpy(pu8Output, u8EM, u32KeyLen);
        ret = HI_SUCCESS;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaDec->enScheme);
        HI_ERR_CIPHER("For a private key decryption operation, the block type shall be 02.\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2:
        u8BT = (HI_U8)(pstRsaDec->enScheme - HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0);
        ret = RSA_padding_check_PKCS1_type(u32KeyLen, u8BT, u8EM, u32InLen, pu8Output, pu32OutLen);
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256:
        ret = RSA_padding_check_PKCS1_OAEP(enHashType, u32HLen, u32KeyLen, u8EM, u32InLen, pu8Output, pu32OutLen);
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5:
        ret = RSA_padding_check_PKCS1_V15(u32KeyLen, u8EM, u32InLen, pu8Output, pu32OutLen);
        break;
    default:
        HI_ERR_CIPHER("RSA scheme error, scheme = 0x%x.\n", pstRsaDec->enScheme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA padding error, ret = 0x%x.\n", ret);
        return ret;
    }
    
    return ret;
}

HI_S32 HI_UNF_CIPHER_RsaPrivateEncrypt(HI_UNF_CIPHER_RSA_PRI_ENC_S *pstRsaEnc, 
							 	   HI_U8 *pu8Input, HI_U32 u32InLen, 
								   HI_U8 *pu8Output, HI_U32 *pu32OutLen)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_RSA_DATA_S stRsaData;
    HI_U32 u32HLen;
    HI_U32 u32KeyLen;
    HI_U8  u8BT;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;
    HI_U8  u8EM[CIPHER_MAX_RSA_KEY_LEN];

    if ((pstRsaEnc == HI_NULL) || (pu8Input == HI_NULL) 
        || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32InLen == 0)
    {
        HI_ERR_CIPHER("inlen is 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    
    ret = RSA_GetAttr(pstRsaEnc->enScheme, pstRsaEnc->stPriKey.u16NLen, 
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if (u32InLen > u32KeyLen)
    {
        HI_ERR_CIPHER("inlen is too large.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    memset(u8EM, 0, sizeof(u8EM));

    switch(pstRsaEnc->enScheme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING:
        /*if u32InLen < u32KeyLen, padding 0 before input data*/
        *pu32OutLen = u32KeyLen;
        memcpy(u8EM+(u32KeyLen - u32InLen), pu8Input, u32InLen);
        ret = HI_SUCCESS;
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        u8BT = (HI_U8)(pstRsaEnc->enScheme - HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0);
        ret = RSA_padding_add_PKCS1_type(u32KeyLen, u8BT, pu8Input, u32InLen, u8EM, pu32OutLen);
        break;
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaEnc->enScheme);
        HI_ERR_CIPHER("For a private- key encryption operation, the block type shall be 00 or 01.\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    default:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaEnc->enScheme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA padding error, ret = 0x%x.\n", ret);
        return ret;
    }

    if(*pu32OutLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32OutLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    stRsaData.pu8N = pstRsaEnc->stPriKey.pu8N;
    stRsaData.pu8K = pstRsaEnc->stPriKey.pu8D;
    stRsaData.u16NLen = pstRsaEnc->stPriKey.u16NLen;
    stRsaData.u16KLen = pstRsaEnc->stPriKey.u16DLen;
    stRsaData.pu8Input = u8EM;
    stRsaData.pu8Output = pu8Output;
    stRsaData.u32DataLen = u32KeyLen;
    
    ret = Cipher_CalcRsa(&stRsaData);
    
    return ret;

}

static HI_S32 RSA_padding_check_EMSA_PKCS1_v15(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                                        HI_U32 u32HLen,
                                        HI_U32 u32KeyLen,
                                        HI_U8 *pu8Input,
                                        HI_U32 u32InLen,
                                        HI_U8 *pu8Output,
                                        HI_U32 *pu32OutLen)
{
    HI_U32 ret = HI_SUCCESS;
    HI_U32 u32Len;
    HI_U8  *p;

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu8Output == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32KeyLen != u32InLen)
    {
        HI_ERR_CIPHER("Error, the u32InLen must be 0x%x\n", u32KeyLen);
        return ret;
    }

    *pu32OutLen = u32HLen;

    /*EM = 01 || PS || 00 || T*/
    p = pu8Input;

    if( *p++ != 0)
    {
        HI_ERR_CIPHER("RSA EM[0] must be 0\n");
        return HI_FAILURE;
    }
    if( *p++ != RSA_SIGN )
    {
        HI_ERR_CIPHER("RSA EM PS error\n");
        return HI_FAILURE;
    }

    while( *p != 0 )
    {
        if( p >= pu8Input + u32KeyLen - 1 || *p != 0xFF )
        {
            HI_ERR_CIPHER("RSA PS error\n");
            return HI_FAILURE;
        }
        p++;
    }
    p++; //skip 0x00

    u32Len = u32KeyLen - (HI_U32)( p - pu8Input);
    ret = HI_FAILURE;
    switch(enHashType)
    {
    case HI_UNF_CIPHER_HASH_TYPE_SHA1:
        if (u32Len != 35)
        {
            HI_ERR_CIPHER("RSA T len error: %d\n", u32Len);
            break;
        }
        if(memcmp(p, ASN1_HASH_SHA1, 15) == 0)
        {
            memcpy(pu8Output, p + 15, u32HLen);
            ret = HI_SUCCESS;
        }
        break;
    case HI_UNF_CIPHER_HASH_TYPE_SHA256:
        if (u32Len != 51)
        {
            HI_ERR_CIPHER("RSA T len error: %d\n", u32Len);
            break;
        }
        if(memcmp(p, ASN1_HASH_SHA256, 19) == 0)
        {
            memcpy(pu8Output, p + 19, u32HLen);
            ret = HI_SUCCESS;
        }
        break;
    default:
            HI_ERR_CIPHER("RSA unsuporrt hash type: 0x%x.\n", enHashType);
    }

    return ret;
}

/*PKCS #1: RSAES-PKCS1-V1_5-Signature*/
/*********************************************************
EM = 0x00 || 0x01 || PS || 0x00 || T

T ::= SEQUENCE {
          digestAlgorithm AlgorithmIdentifier,
          digest OCTET STRING
      }
The first field identifies the hash function and the second
contains the hash value
**********************************************************/
static HI_S32 RSA_padding_add_EMSA_PKCS1_v15(HI_UNF_CIPHER_HASH_TYPE_E enHashType,
                                        HI_U32 u32HLen,
                                        HI_U32 u32KeyLen,
                                        HI_U8 *pu8Input,
                                        HI_U32 u32InLen,
                                        HI_U8 *pu8Output,
                                        HI_U32 *pu32OutLen)
{
	HI_U32 u32PadLen;
    HI_U8  *p;

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu8Output == HI_NULL))
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (u32HLen != u32InLen)
    {
        HI_ERR_CIPHER("Error, the u32InLen must be 0x%x\n", u32HLen);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
        
    *pu32OutLen = u32KeyLen;

     p = pu8Output;
    *p++ = 0;
    *p++ = RSA_SIGN;
    switch(enHashType)
    {
    case HI_UNF_CIPHER_HASH_TYPE_SHA1:
        u32PadLen = u32KeyLen - 3 - 35;
        memset(p, 0xFF, u32PadLen);
        p += u32PadLen;
        *p++ = 0;
        memcpy(p, ASN1_HASH_SHA1, 15);
        memcpy(p + 15, pu8Input, u32InLen);
        break;
    case HI_UNF_CIPHER_HASH_TYPE_SHA256:
        u32PadLen = u32KeyLen - 3 - 51;
        memset(p, 0xFF, u32PadLen);
        p += u32PadLen;
        *p++ = 0;
        memcpy( p, ASN1_HASH_SHA256, 19);
        memcpy( p + 19, pu8Input, u32InLen);
        break;
    default:
        HI_ERR_CIPHER("RSA unsuporrt hash type: 0x%x.\n", enHashType);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_RsaSign(HI_UNF_CIPHER_RSA_SIGN_S *pstRsaSign, 
							 HI_U8 *pu8InData, HI_U32 u32InDataLen,
							 HI_U8 *pu8HashData,
							 HI_U8 *pu8OutSign, HI_U32 *pu32OutSignLen)
{
    HI_U32 u32KeyLen;
    HI_U32 u32HLen;
    HI_U8  u8Hash[32];
    HI_U8  u8EM[CIPHER_MAX_RSA_KEY_LEN];
    HI_S32 ret = HI_SUCCESS;
    CIPHER_RSA_DATA_S stRsaData;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_HANDLE HashHandle;
    HI_U8 *pHash;

    if (pstRsaSign == HI_NULL) 
    {
        HI_ERR_CIPHER("pstRsaSign is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
    if ((pstRsaSign->stPriKey.pu8N == HI_NULL) || (pstRsaSign->stPriKey.pu8D == HI_NULL)) 
    {
        HI_ERR_CIPHER("key is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if ((u32InDataLen == 0) && (pu8HashData == HI_NULL))
    {
        HI_ERR_CIPHER("inlen is 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    ret = RSA_GetAttr(pstRsaSign->enScheme, pstRsaSign->stPriKey.u16NLen, 
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    /*hash is NULl, need to calc by self*/
    if (pu8HashData == HI_NULL)
    {
    	memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));
        stHashAttr.eShaType = enHashType;
        stHashAttr.u32TotalDataLen = u32InDataLen;
        ret = Cipher_HashInit(&stHashAttr, &HashHandle);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("HI_MPI_CIPHER_HashInit error\n");
            return ret;
        }
        ret = Cipher_HashUpdate(HashHandle, pu8InData, u32InDataLen);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("HI_MPI_CIPHER_HashUpdate error\n");
            return ret;
        }
        ret = Cipher_HashFinal(HashHandle, u8Hash);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("HI_MPI_CIPHER_HashUpdate error\n");
            return ret;
        }
        pHash = u8Hash;
    }
    else
    {
        pHash = pu8HashData;
    }

    memset(u8EM, 0, sizeof(u8EM));
    switch(pstRsaSign->enScheme)
    {
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256:
        ret = RSA_padding_add_EMSA_PKCS1_v15(enHashType, u32HLen, u32KeyLen, 
            pHash, u32HLen, u8EM, pu32OutSignLen);
        break;
    default:
        HI_ERR_CIPHER("invalid scheme; 0x%x\n", pstRsaSign->enScheme);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(*pu32OutSignLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32InSigntLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
    
    stRsaData.pu8N = pstRsaSign->stPriKey.pu8N;
    stRsaData.pu8K = pstRsaSign->stPriKey.pu8D;
    stRsaData.u16NLen = pstRsaSign->stPriKey.u16NLen;
    stRsaData.u16KLen = pstRsaSign->stPriKey.u16DLen;
    stRsaData.pu8Input = u8EM;
    stRsaData.pu8Output = pu8OutSign;
    stRsaData.u32DataLen = u32KeyLen;

//    HI_PRINT_HEX ("N", stRsaData.pu8N, stRsaData.u16NLen);
//    HI_PRINT_HEX ("K", stRsaData.pu8K, stRsaData.u16KLen);
//    HI_PRINT_HEX ("M", stRsaData.pu8Input, stRsaData.u32DataLen);

    ret = Cipher_CalcRsa(&stRsaData);

    return ret;
}

HI_S32 HI_UNF_CIPHER_RsaVerify(HI_UNF_CIPHER_RSA_VERIFY_S *pstRsaVerify,
							   HI_U8 *pu8InData, HI_U32 u32InDataLen,
							   HI_U8 *pu8HashData,
							   HI_U8 *pu8InSign, HI_U32 u32InSignLen)
{
    HI_U32 ret = HI_SUCCESS;
    HI_U32 u32KeyLen;
    HI_U8  u8EM[CIPHER_MAX_RSA_KEY_LEN];
    CIPHER_RSA_DATA_S stRsaData;
    HI_UNF_CIPHER_HASH_TYPE_E enHashType;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_HANDLE HashHandle;
    HI_U8  u8Hash[32] = {0};
    HI_U8  u8SignHash[32] = {0};
    HI_U32 u32HLen;
    HI_U8  *pHash;

    if (pstRsaVerify == HI_NULL)
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
    if ((pstRsaVerify->stPubKey.pu8N == HI_NULL) || (pstRsaVerify->stPubKey.pu8E == HI_NULL)) 
    {
        HI_ERR_CIPHER("para is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
    
    ret = RSA_GetAttr(pstRsaVerify->enScheme, pstRsaVerify->stPubKey.u16NLen, 
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if(u32InSignLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32InSigntLen != u32KeyLen.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    stRsaData.pu8N = pstRsaVerify->stPubKey.pu8N;
    stRsaData.pu8K = pstRsaVerify->stPubKey.pu8E;
    stRsaData.u16NLen = pstRsaVerify->stPubKey.u16NLen;
    stRsaData.u16KLen = pstRsaVerify->stPubKey.u16ELen;
    stRsaData.pu8Input = pu8InSign;
    stRsaData.pu8Output = u8EM;
    stRsaData.u32DataLen = u32KeyLen;

    ret = Cipher_CalcRsa(&stRsaData);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("RSA verify dec error, ret=%d\n", ret);
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    if (pu8HashData == HI_NULL)
    {
    	memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));
        stHashAttr.eShaType = enHashType;
        stHashAttr.u32TotalDataLen = u32InDataLen;
        ret = Cipher_HashInit(&stHashAttr, &HashHandle);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("HI_MPI_CIPHER_HashInit error\n");
            return ret;
        }
        ret = Cipher_HashUpdate(HashHandle, pu8InData, u32InDataLen);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("HI_MPI_CIPHER_HashUpdate error\n");
            return ret;
        }
        ret = Cipher_HashFinal(HashHandle, u8Hash);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("HI_MPI_CIPHER_HashUpdate error\n");
            return ret;
        }
        pHash = u8Hash;
    }
    else
    {
        pHash = pu8HashData;
    }

    switch(pstRsaVerify->enScheme)
    {
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1:
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256:
        ret = RSA_padding_check_EMSA_PKCS1_v15(enHashType, u32HLen, u32KeyLen, u8EM, 
              u32InSignLen, u8SignHash, &u32HLen);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("RSA_padding_add_RSASSA_PKCS1_v15 error\n");
            return ret;
        }
        if (memcmp(pHash, u8SignHash, u32HLen) != 0)
        {
            HI_ERR_CIPHER("RSA verify, hash error\n");
            ret = HI_ERR_CIPHER_FAILED_DECRYPT;
        }
        break;
    default:
        HI_ERR_CIPHER("invalid scheme; 0x%x\n", pstRsaVerify->enScheme);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    
    return ret;
}

