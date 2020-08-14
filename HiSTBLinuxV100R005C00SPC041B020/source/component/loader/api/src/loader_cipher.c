/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : loader_cipher.c
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/
#ifdef HI_LOADER_BOOTLOADER
#ifdef HI_MINIBOOT_SUPPORT
#include <stdio.h>
#include <string.h>
#else
#include "common.h"
#endif
#else
#include <stdio.h>
#include <string.h>
#endif

#include "hi_type.h"
#include "hi_common.h"
#include "hi_unf_advca.h"
#include "hi_unf_cipher.h"
#include "loader_debug.h"
#include "loader_osal.h"
#include "loader_cipher.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LDDB_D_LEN_CIPHERKEY        (16)
#define LDDB_D_SIZE_ENCRYPTBLOCK    (1024 * 1024 - 16)

#define LDDB_D_LEN_HMAXVALUE        (32)
#define LDDB_D_LEN_HMAXKEY          (16)
#define LDDB_D_SIZE_HMAXBLOCK       (1024 * 1024)

#if (LDDB_D_CIPHER_BYTEWIDTH == 16)
#define LDDB_D_CIPHER_BITWIDTH      HI_UNF_CIPHER_BIT_WIDTH_128BIT
#elif (LDDB_D_CIPHER_BYTEWIDTH == 1)
#define LDDB_D_CIPHER_BITWIDTH      HI_UNF_CIPHER_BIT_WIDTH_8BIT
#elif (LDDB_D_CIPHER_BYTEWIDTH == 8)
#define LDDB_D_CIPHER_BITWIDTH      HI_UNF_CIPHER_BIT_WIDTH_64BIT
#else
#error "Cipher bit width config error!"
#endif


static HI_U8 s_au8HMACSha2Key[LDDB_D_LEN_HMAXKEY] =
{
    0x16, 0x37, 0xa4, 0x54, 0x64, 0xda, 0xe9, 0x19,
    0x03, 0x69, 0xe4, 0x1f, 0xea, 0xa9, 0xfa, 0x0a
};

static HI_U8 s_au8CipherKey[LDDB_D_LEN_CIPHERKEY] =
{
    0x5b, 0x45, 0x14, 0x37, 0x6d, 0xed, 0x1d, 0x08,
    0x64, 0x97, 0xbc, 0xba, 0xe6, 0x7f, 0x90, 0x78
};

static HI_U8 s_au8KeyLadderKey1[LDDB_D_LEN_CIPHERKEY] =
{
    0x1c, 0x21, 0x42, 0xa2, 0x9e, 0x31, 0x57, 0x8d,
    0xff, 0xa4, 0xe1, 0xdf, 0x2e, 0x36, 0xa0, 0x6e
};

static HI_U8 s_au8KeyLadderKey2[LDDB_D_LEN_CIPHERKEY] =
{
    0x7c, 0x76, 0x1a, 0x38, 0xc1, 0x46, 0x4e, 0x53,
    0x8c, 0xa3, 0xd7, 0xae, 0x56, 0xee, 0x27, 0xb7
};

static HI_U8 s_auIV[LDDB_D_LEN_CIPHERKEY] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

static HI_HANDLE s_hCipher = HI_INVALID_HANDLE;

static HI_S32 lddbCipherHmax(HI_U8* pu8Buffer, HI_U32 u32Len, HI_U8* pcHashValue)
{
    HI_U32 u32Index = 0;
    HI_U32 u32Round = 0;
    HI_U32 u32TailSize = 0;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_U8* pu8Position = HI_NULL_PTR;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_MemSet(&stHashAttr, 0x00, sizeof(stHashAttr));

#ifdef HI_LOADER_BOOTLOADER
    stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256;
    stHashAttr.u32TotalDataLen = u32Len;
    LDDB_MemCpy(stHashAttr.u8HMACKey, s_au8HMACSha2Key, LDDB_D_LEN_HMAXKEY);
#else
    stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256;
    stHashAttr.u32HMACKeyLen = LDDB_D_LEN_HMAXKEY;
    stHashAttr.pu8HMACKey = s_au8HMACSha2Key;
#endif

    s32Ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(HI_UNF_CIPHER_HashInit, s32Ret);
        return LDDB_ERR_EXTERNAL;
    }

    u32Round = u32Len / LDDB_D_SIZE_HMAXBLOCK;
    pu8Position = pu8Buffer;

    for (u32Index = 0; u32Index < u32Round; u32Index ++)
    {
        s32Ret = HI_UNF_CIPHER_HashUpdate(hHandle, pu8Position, LDDB_D_SIZE_HMAXBLOCK);
        if (HI_SUCCESS != s32Ret)
        {
            LDDB_PrintFuncErr(HI_UNF_CIPHER_HashUpdate, s32Ret);
            LDDB_CHECK(HI_UNF_CIPHER_HashFinal(hHandle, pcHashValue));
            return LDDB_ERR_EXTERNAL;
        }

        pu8Position += LDDB_D_SIZE_HMAXBLOCK;
    }

    u32TailSize = u32Len % LDDB_D_SIZE_HMAXBLOCK;
    if (0 != u32TailSize)
    {
        s32Ret = HI_UNF_CIPHER_HashUpdate(hHandle, pu8Position, u32TailSize);
        if (HI_SUCCESS != s32Ret)
        {
            LDDB_PrintFuncErr(HI_UNF_CIPHER_HashUpdate, s32Ret);
            LDDB_CHECK(HI_UNF_CIPHER_HashFinal(hHandle, pcHashValue));
            return LDDB_ERR_EXTERNAL;
        }
    }

    s32Ret = HI_UNF_CIPHER_HashFinal(hHandle, pcHashValue);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(HI_UNF_CIPHER_HashFinal, s32Ret);
        return LDDB_ERR_EXTERNAL;
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 lddbCipherCryptoInit(HI_VOID)
{
    HI_UNF_ADVCA_KEYLADDER_LEV_E enKeyladderLevel = HI_UNF_ADVCA_KEYLADDER_BUTT;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;

    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    s32Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != s32Ret )
    {
        LDDB_PrintFuncErr(HI_UNF_ADVCA_Init, s32Ret);
        return LDDB_ERR_EXTERNAL;
    }

    s32Ret = HI_UNF_ADVCA_SetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_AES);
    if (HI_SUCCESS != s32Ret )
    {
        LDDB_PrintFuncErr(HI_UNF_ADVCA_SetR2RAlg, s32Ret);
        LDDB_CHECK(HI_UNF_ADVCA_DeInit());
        return LDDB_ERR_EXTERNAL;
    }

    s32Ret = HI_UNF_ADVCA_GetR2RKeyLadderStage(&enKeyladderLevel);
    if (HI_SUCCESS != s32Ret )
    {
        LDDB_PrintFuncErr(HI_UNF_ADVCA_GetR2RKeyLadderStage, s32Ret);
        LDDB_CHECK(HI_UNF_ADVCA_DeInit());
        return LDDB_ERR_EXTERNAL;
    }

    s32Ret = HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1, s_au8KeyLadderKey1);
    if (HI_SUCCESS != s32Ret )
    {
        LDDB_PrintFuncErr(HI_UNF_ADVCA_SetR2RSessionKey, s32Ret);
        LDDB_CHECK(HI_UNF_ADVCA_DeInit());
        return LDDB_ERR_EXTERNAL;
    }

    if (HI_UNF_ADVCA_KEYLADDER_LEV3 == enKeyladderLevel)
    {
        s32Ret = HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV2, s_au8KeyLadderKey2);
        if (HI_SUCCESS != s32Ret )
        {
            LDDB_PrintFuncErr(HI_UNF_ADVCA_SetR2RSessionKey, s32Ret);
            LDDB_CHECK(HI_UNF_ADVCA_DeInit());
            return LDDB_ERR_EXTERNAL;
        }
    }

    LDDB_MemSet(&stCipherAttr, 0x00, sizeof(stCipherAttr));
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    s32Ret = HI_UNF_CIPHER_CreateHandle(&s_hCipher, &stCipherAttr);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(HI_UNF_CIPHER_CreateHandle, s32Ret);
        LDDB_CHECK(HI_UNF_ADVCA_DeInit());
        return LDDB_ERR_EXTERNAL;
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 lddbCipherCryptoDeInit(HI_VOID)
{
    LDDB_DBG_FuncEnter();

    if (HI_INVALID_HANDLE != s_hCipher)
    {
        LDDB_CHECK(HI_UNF_CIPHER_DestroyHandle(s_hCipher));
        LDDB_CHECK(HI_UNF_ADVCA_DeInit());

        s_hCipher = HI_INVALID_HANDLE;
    }


    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 lddbCipherCrytoConfig(HI_VOID)
{
    HI_UNF_CIPHER_CTRL_S stCipherCtrl;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_MemSet(&stCipherCtrl, 0x00, sizeof(stCipherCtrl));
    stCipherCtrl.bKeyByCA   = HI_TRUE;
    stCipherCtrl.enAlg      = HI_UNF_CIPHER_ALG_AES;
    stCipherCtrl.enBitWidth = LDDB_D_CIPHER_BITWIDTH;
    stCipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CTR;
    stCipherCtrl.enKeyLen   = HI_UNF_CIPHER_KEY_AES_128BIT;
    stCipherCtrl.enCaType   = HI_UNF_CIPHER_CA_TYPE_R2R;
    stCipherCtrl.stChangeFlags.bit1IV = 1;
    LDDB_MemCpy(stCipherCtrl.u32IV, s_auIV, LDDB_D_LEN_CIPHERKEY);
    LDDB_MemCpy(stCipherCtrl.u32Key, s_au8CipherKey, LDDB_D_LEN_CIPHERKEY);

    s32Ret = HI_UNF_CIPHER_ConfigHandle(s_hCipher, &stCipherCtrl);
    if (HI_SUCCESS != s32Ret )
    {
        LDDB_PrintFuncErr(HI_UNF_CIPHER_ConfigHandle, s32Ret);
        return LDDB_ERR_EXTERNAL;
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 lddbCipherCryptoProcess(HI_BOOL bEncrypt, HI_U8* pu8Buffer, HI_U32 u32Size)
{
    HI_U32 u32Index = 0;
    HI_U32 u32Round = 0;
    HI_U32 u32TailSize = 0;
    HI_U32 u32SrcPhyAddr = 0;
    HI_U32 u32DestPhyAddr = 0;
    HI_U8* pu8SrcVirAddr = HI_NULL_PTR;
    HI_U8* pu8DestVirAddr = HI_NULL_PTR;
    HI_U8* pu8Position = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    s32Ret = lddbCipherCrytoConfig();
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(lddbCipherCrytoConfig, s32Ret);
        return s32Ret;
    }

    s32Ret = LDDB_MMZ_Malloc(LDDB_D_SIZE_ENCRYPTBLOCK, &u32SrcPhyAddr, &pu8SrcVirAddr);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_MMZ_Malloc, s32Ret);
        return LDDB_ERR_MEM;
    }

    s32Ret = LDDB_MMZ_Malloc(LDDB_D_SIZE_ENCRYPTBLOCK, &u32DestPhyAddr, &pu8DestVirAddr);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_MMZ_Malloc, s32Ret);
        LDDB_MMZ_Free(u32SrcPhyAddr);
        return LDDB_ERR_MEM;
    }

    pu8Position = pu8Buffer;

    u32Round = u32Size / LDDB_D_SIZE_ENCRYPTBLOCK;
    for (u32Index = 0; u32Index < u32Round; u32Index ++)
    {
        LDDB_MemCpy(pu8SrcVirAddr, pu8Position, LDDB_D_SIZE_ENCRYPTBLOCK);

        if (HI_TRUE == bEncrypt)
        {
            s32Ret = HI_UNF_CIPHER_Encrypt(s_hCipher, u32SrcPhyAddr, u32DestPhyAddr, LDDB_D_SIZE_ENCRYPTBLOCK);
            if (HI_SUCCESS != s32Ret)
            {
                LDDB_PrintFuncErr(HI_UNF_CIPHER_Encrypt, s32Ret);
                LDDB_MMZ_Free(u32SrcPhyAddr);
                LDDB_MMZ_Free(u32DestPhyAddr);
                return LDDB_ERR_MEM;
            }
        }
        else
        {
            s32Ret = HI_UNF_CIPHER_Decrypt(s_hCipher, u32SrcPhyAddr, u32DestPhyAddr, LDDB_D_SIZE_ENCRYPTBLOCK);
            if (HI_SUCCESS != s32Ret)
            {
                LDDB_PrintFuncErr(HI_UNF_CIPHER_Decrypt, s32Ret);
                LDDB_MMZ_Free(u32SrcPhyAddr);
                LDDB_MMZ_Free(u32DestPhyAddr);
                return LDDB_ERR_MEM;
            }
        }

        LDDB_MemCpy(pu8Position, pu8DestVirAddr, LDDB_D_SIZE_ENCRYPTBLOCK);
        pu8Position += LDDB_D_SIZE_ENCRYPTBLOCK;
    }

    u32TailSize = u32Size % LDDB_D_SIZE_ENCRYPTBLOCK;
    if (0 < u32TailSize)
    {
        LDDB_MemCpy(pu8SrcVirAddr, pu8Position, u32TailSize);
        if (HI_TRUE == bEncrypt)
        {
            s32Ret = HI_UNF_CIPHER_Encrypt(s_hCipher, u32SrcPhyAddr, u32DestPhyAddr, u32TailSize);
            if (HI_SUCCESS != s32Ret)
            {
                LDDB_PrintFuncErr(HI_UNF_CIPHER_Encrypt, s32Ret);
                LDDB_MMZ_Free(u32SrcPhyAddr);
                LDDB_MMZ_Free(u32DestPhyAddr);
                return LDDB_ERR_MEM;
            }
        }
        else
        {
            s32Ret = HI_UNF_CIPHER_Decrypt(s_hCipher, u32SrcPhyAddr, u32DestPhyAddr, u32TailSize);
            if (HI_SUCCESS != s32Ret)
            {
                LDDB_PrintFuncErr(HI_UNF_CIPHER_Decrypt, s32Ret);
                LDDB_MMZ_Free(u32SrcPhyAddr);
                LDDB_MMZ_Free(u32DestPhyAddr);
                return LDDB_ERR_MEM;
            }
        }

        LDDB_MemCpy(pu8Position, pu8DestVirAddr, u32TailSize);
    }

    LDDB_MMZ_Free(u32SrcPhyAddr);
    LDDB_MMZ_Free(u32DestPhyAddr);

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_CIPHER_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    s32Ret = HI_UNF_CIPHER_Init();
    if (HI_SUCCESS != s32Ret )
    {
        LDDB_PrintFuncErr(HI_UNF_CIPHER_Init, s32Ret);
        return LDDB_ERR_EXTERNAL;
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_CIPHER_DeInit(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    s32Ret = HI_UNF_CIPHER_DeInit();
    if (HI_SUCCESS != s32Ret )
    {
        LDDB_PrintFuncErr(HI_UNF_CIPHER_DeInit, s32Ret);
        return LDDB_ERR_EXTERNAL;
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_CIPHER_BlockVerify(HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue)
{
    HI_U8  au8HashValue[LDDB_D_LEN_HMAXVALUE] = {0};
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8Data);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8VerifyValue);
    LDDB_CHECK_PARAM(0 == u32Size);

    s32Ret = lddbCipherHmax(pu8Data, u32Size, au8HashValue);
    if (HI_SUCCESS != s32Ret )
    {
        LDDB_PrintFuncErr(lddbCipherHmax, s32Ret);
        return s32Ret;
    }

    if (0 != LDDB_MemCmp(au8HashValue, pu8VerifyValue, LDDB_D_LEN_HMAXVALUE))
    {
        LDDB_PrintErrCode(LDDB_ERR_DATABASE);
        return LDDB_ERR_DATABASE;
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_CIPHER_BlockSignature(HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue)
{
    HI_U8  u8HashValue[LDDB_D_LEN_HMAXVALUE] = {0};
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8Data);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8VerifyValue);
    LDDB_CHECK_PARAM(0 == u32Size);

    s32Ret = lddbCipherHmax(pu8Data, u32Size, u8HashValue);
    if (HI_SUCCESS != s32Ret )
    {
        LDDB_PrintFuncErr(lddbCipherHmax, s32Ret);
        return s32Ret;
    }

    LDDB_MemCpy(pu8VerifyValue, u8HashValue, LDDB_D_LEN_HMAXVALUE);

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_CIPHER_BlockEncrypt(HI_U8* pu8Data, HI_U32 u32Size)
{
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8Data);
    LDDB_CHECK_PARAM(0 == u32Size);

    s32Ret = lddbCipherCryptoInit();
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(lddbCipherCryptoInit, s32Ret);
        return s32Ret;
    }

    s32Ret = lddbCipherCryptoProcess(HI_TRUE, pu8Data, u32Size);
    if (HI_SUCCESS != s32Ret )
    {
        LDDB_PrintFuncErr(lddbCipherCryptoProcess, s32Ret);
        LDDB_CHECK(lddbCipherCryptoDeInit());
        return s32Ret;
    }

    LDDB_CHECK(lddbCipherCryptoDeInit());

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_CIPHER_BlockDecrypt(HI_U8* pu8Data, HI_U32 u32Size)
{
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8Data);
    LDDB_CHECK_PARAM(0 == u32Size);

    s32Ret = lddbCipherCryptoInit();
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(lddbCipherCryptoInit, s32Ret);
        return s32Ret;
    }

    s32Ret = lddbCipherCryptoProcess(HI_FALSE, pu8Data, u32Size);
    if ( HI_SUCCESS != s32Ret )
    {
        LDDB_PrintFuncErr(lddbCipherCryptoProcess, s32Ret);
        LDDB_CHECK(lddbCipherCryptoDeInit());
        return s32Ret;
    }

    LDDB_CHECK(lddbCipherCryptoDeInit());

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

