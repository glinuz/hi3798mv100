/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : unf_cipher_tz.c
Version       : Initial
Author        : Hisilicon hisecurity team
Created       : 2014-04-04
Last Modified :
Description   : unf api of cipher in secure side
Function List :
History       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "hi_common.h"
#include "hi_mpi_cipher.h"
#include "hi_drv_struct.h"
#include "hi_error_mpi.h"
#include "drv_cipher_ioctl.h"
#include "DxTypes.h"
#include "tee_client_api.h"
#include "tee_client_id.h"
#include "hi_tz_common.h"

HI_S32 g_CipherDevFd = -1;
static pthread_mutex_t   g_CipherMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_CIPHER_LOCK()         (void)pthread_mutex_lock(&g_CipherMutex);
#define HI_CIPHER_UNLOCK()       (void)pthread_mutex_unlock(&g_CipherMutex);

#define CHECK_CIPHER_INIT()\
do{\
    HI_CIPHER_LOCK();\
    if (g_CipherDevFd < 0)\
    {\
        HI_ERR_CIPHER("CIPHER is not open.\n");\
        HI_CIPHER_UNLOCK();\
        return HI_ERR_CIPHER_NOT_INIT;\
    }\
    HI_CIPHER_UNLOCK();\
}while(0)

static HI_S32 g_HashDevFd = -1;
static CIPHER_HASH_DATA_S g_stCipherHashData;

#define HASH_BLOCK_SIZE     (64)
#define HASH_PAD_MAX_LEN    (64)
#define HASH1_SIGNATURE_SIZE        (20)
#define HASH256_SIGNATURE_SIZE      (32)
#define HASH_MMZ_BUF_LEN    (10*1024*1024)      //10M
#define HASH_MMZ_TAIL_LEN   (8*1024)            //8K

#ifndef SEC_MMZ_ZONE
#define SEC_MMZ_ZONE        "SEC-MMZ"
#endif

static HI_U8 g_au8HmacKey[HASH_BLOCK_SIZE];
static HI_U32 u32HmacKeyLen = 0;
static HI_U32 g_u32HashBaseBufferLen = HASH_MMZ_BUF_LEN;
static  TEEC_Context s_stTeecContext;
static  TEEC_Session s_stTeecSession;

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

/*********************************************************
 * The function below is added for AES CBC-MAC
 *
 *********************************************************/
#define MAX_DATA_LEN    (0x2000) //the max data length for encryption / decryption is  8k one time



#if 0
static HI_U8 const_Zero[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

/* For CMAC Calculation */

static HI_U8 const_Rb[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87
};

static const TEEC_UUID s_stTeecCipherUUID =
{
    0x79b77788,
    0x9789,
    0x4a7a,
    {0xa2, 0xbe, 0xb6, 0x01, 0x55, 0xee, 0xf5, 0xf6}
};

/* Basic Functions */
static HI_VOID xor_128(HI_U8 *a, HI_U8 *b, HI_U8 *out)
{
    HI_U32 i;

    if(a == NULL || b == NULL || out == NULL)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
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
        HI_ERR_CIPHER("Invalid parameter!\n");
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

static HI_VOID padding ( HI_U8 *lastb, HI_U8 *pad, HI_U32 length )
{
    HI_U32 j;

    if(lastb == NULL || pad == NULL)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
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

static HI_S32 Cipher_HashMmzInit(HI_VOID)
{
    HI_U32 u32Testcached = 0;
    HI_U32 i = 0;
    HI_U32 u32HashBufLen = 0;

    u32HashBufLen = g_u32HashBaseBufferLen + HASH_MMZ_TAIL_LEN;

    g_stCipherHashData.stMMZBuffer.u32StartPhyAddr = (HI_U32)HI_MMZ_New(u32HashBufLen, 0, NULL, "CIPHER_HashBuffer");
    if (0 == g_stCipherHashData.stMMZBuffer.u32StartPhyAddr)
    {
        g_u32HashBaseBufferLen = g_u32HashBaseBufferLen / 2;
        u32HashBufLen = g_u32HashBaseBufferLen + HASH_MMZ_TAIL_LEN;
        g_stCipherHashData.stMMZBuffer.u32StartPhyAddr = (HI_U32)HI_MMZ_New(u32HashBufLen, 0, NULL, "CIPHER_HashBuffer");
        if (0 == g_stCipherHashData.stMMZBuffer.u32StartPhyAddr)
        {
            g_u32HashBaseBufferLen = g_u32HashBaseBufferLen / 2;
            u32HashBufLen = g_u32HashBaseBufferLen + HASH_MMZ_TAIL_LEN;
            g_stCipherHashData.stMMZBuffer.u32StartPhyAddr = (HI_U32)HI_MMZ_New(u32HashBufLen, 0, NULL, "CIPHER_HashBuffer");
            if (0 == g_stCipherHashData.stMMZBuffer.u32StartPhyAddr)
            {
                HI_ERR_CIPHER("Error: Get mmz buffer for hash failed!\n");
                return HI_FAILURE;
            }
        }
    }

    g_stCipherHashData.stMMZBuffer.u32StartVirAddr = (HI_U32)HI_MMZ_Map(g_stCipherHashData.stMMZBuffer.u32StartPhyAddr, u32Testcached);
    if( 0 == (HI_VOID *)g_stCipherHashData.stMMZBuffer.u32StartVirAddr )
    {
        HI_ERR_CIPHER("Error: Map mmz buffer for hash failed!\n");
        HI_MMZ_Delete(g_stCipherHashData.stMMZBuffer.u32StartPhyAddr);
        memset(&(g_stCipherHashData.stMMZBuffer), 0, sizeof(g_stCipherHashData.stMMZBuffer));
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_VOID Cipher_HashMmzDeInit(HI_VOID)
{
    (HI_VOID)HI_MMZ_Unmap(g_stCipherHashData.stMMZBuffer.u32StartPhyAddr);
    (HI_VOID)HI_MMZ_Delete(g_stCipherHashData.stMMZBuffer.u32StartPhyAddr);
    memset(&g_stCipherHashData.stMMZBuffer, 0, sizeof(g_stCipherHashData.stMMZBuffer));
}

static HI_S32 Cipher_HashMsgPadding(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_U32 u32Tmp = 0;
    HI_U32 i = 0;
    HI_U8 u8PadLen[8];

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error! Null pointer input!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    memset(pCipherHashData->u8Padding, 0, sizeof(pCipherHashData->u8Padding));

    u32Tmp = pCipherHashData->u32TotalDataLen % HASH_PAD_MAX_LEN;
    /* 56 = 64 - 8, 120 = 56 + 64 */
    pCipherHashData->u32PaddingLen = (u32Tmp < 56) ? (56 - u32Tmp) : (120 - u32Tmp);
    /* add 8 bytes fix data length */
    pCipherHashData->u32PaddingLen += 8;

    /* Format(binary): {data|1000...00| fix_data_len(bits)} */
    memset(pCipherHashData->u8Padding, 0x80, 1);
    memset(pCipherHashData->u8Padding + 1, 0, pCipherHashData->u32PaddingLen - 1 - 8);
    /* write 8 bytes fix data length */
    memset(u8PadLen, 0, sizeof(u8PadLen));
    u32Tmp = pCipherHashData->u32TotalDataLen;

#if 0
    if( (HI_MPI_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType)
     || (HI_MPI_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType) )
    {
        for (i = 0; i < 8; i++)
        {
            u8PadLen[i] = ((u32Tmp * 8 + 512) >> (7 - i) * 8) & 0xff;
        }
    }
    else
#endif
    {
        for (i = 0; i < 8; i++)
        {
            u8PadLen[i] = ((u32Tmp * 8) >> (7 - i) * 8) & 0xff;
        }
    }

    memcpy(pCipherHashData->u8Padding + pCipherHashData->u32PaddingLen - 8, u8PadLen, 8);

    return HI_SUCCESS;
}

static HI_S32 CIPHER_HashInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_CIPHER_HASH_TYPE_E enShaTypeBak = HI_UNF_CIPHER_HASH_TYPE_BUTT;
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_HASHINIT;

    if( (NULL == pstHashAttr)
     || (NULL == pHashHandle)
     || (pstHashAttr->eShaType >= HI_UNF_CIPHER_HASH_TYPE_BUTT))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    enShaTypeBak = pstHashAttr->eShaType;

    CHECK_CIPHER_INIT();

    if(g_HashDevFd > 0)
    {
        HI_ERR_CIPHER("Hash module is busy!\n");
        return HI_FAILURE;
    }

    g_HashDevFd = 1;
    memset(&g_stCipherHashData, 0, sizeof(g_stCipherHashData));
    g_u32HashBaseBufferLen = HASH_MMZ_BUF_LEN;
    ret = Cipher_HashMmzInit();
    if ( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("Hash mmz buffer initial failed!\n");
        g_HashDevFd = -1;
        return HI_FAILURE;
    }

    if ( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstHashAttr->eShaType)
      || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pstHashAttr->eShaType ))
    {
        g_stCipherHashData.enShaType = HI_UNF_CIPHER_HASH_TYPE_SHA1;
        pstHashAttr->eShaType  = HI_UNF_CIPHER_HASH_TYPE_SHA1;
    }
    else if ( (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstHashAttr->eShaType)
         || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pstHashAttr->eShaType))
    {
        g_stCipherHashData.enShaType = HI_UNF_CIPHER_HASH_TYPE_SHA256;
        pstHashAttr->eShaType  = HI_UNF_CIPHER_HASH_TYPE_SHA256;
    }
    else
    {
        ;/* Invalid hash/hmac type, no need to be processed, it have been processed before */
    }

    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));
#if 0
    if(pstHashAttr->pu8HMACKey != HI_NULL)
    {
        stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_VALUE_INOUT,
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_NONE);
        stTeecOperation.params[0].tmpref.buffer = (HI_VOID *)pstHashAttr;
        stTeecOperation.params[0].tmpref.size = sizeof(HI_UNF_CIPHER_HASH_ATTS_S);
        stTeecOperation.params[1].value.a = *pHashHandle;
        stTeecOperation.params[2].tmpref.buffer = pstHashAttr->pu8HMACKey;
        stTeecOperation.params[2].tmpref.size= pstHashAttr->u32HMACKeyLen;
    }
    else
#endif
    {
        stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_VALUE_INOUT,
                                            TEEC_NONE,
                                            TEEC_NONE);
        stTeecOperation.params[0].tmpref.buffer = (HI_VOID *)pstHashAttr;
        stTeecOperation.params[0].tmpref.size = sizeof(HI_UNF_CIPHER_HASH_ATTS_S);
        stTeecOperation.params[1].value.a = *pHashHandle;
    }
    stTeecOperation.started = 1;
    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        ret =  HI_FAILURE;
    }

    *pHashHandle = stTeecOperation.params[1].value.a;
    g_stCipherHashData.enShaType = enShaTypeBak;

    return HI_SUCCESS;
}

static HI_S32 CIPHER_HashUpdate(HI_HANDLE hHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_HASH_DATA_S *pstCipherHashData = &g_stCipherHashData;
    HI_U32 u32Tmp = 0;
    HI_U32 u32OffsetBak = 0;
    HI_U32 u32BufInUsedLen = 0;
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_HASHUPDATE;
    HI_U32 i = 0;

    if((NULL == pu8InputData) || (NULL == pstCipherHashData))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        g_HashDevFd = -1;
        Cipher_HashMmzDeInit();
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (0 == u32InputDataLen)
    {
        if( pstCipherHashData->u32TotalDataLen > 0)
        {
            return HI_SUCCESS;
        }
        else
        {
            g_HashDevFd = -1;
            Cipher_HashMmzDeInit();
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    CHECK_CIPHER_INIT();

    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_VALUE_INPUT,
                                                TEEC_MEMREF_TEMP_INPUT,
                                                TEEC_NONE,
                                                TEEC_NONE);

    pstCipherHashData->u32TotalDataLen += u32InputDataLen;
    u32OffsetBak = pstCipherHashData->u32Offset;

    u32Tmp = (u32InputDataLen+ pstCipherHashData->u32Offset) / g_u32HashBaseBufferLen;
    if ( 0 == u32Tmp )
    {

        /* Total length is less than g_u32HashBaseBufferLen, move data to mmz buffer directly */
        memcpy((HI_U8 *)pstCipherHashData->stMMZBuffer.u32StartVirAddr + pstCipherHashData->u32Offset, pu8InputData, u32InputDataLen);
        pstCipherHashData->u32Offset += u32InputDataLen;
        return HI_SUCCESS;
    }
    else
    {
        /* Send data down piece by piece */
        for ( i = 0 ; i < u32Tmp; i++ )
        {
            memcpy((HI_U8 *)pstCipherHashData->stMMZBuffer.u32StartVirAddr + pstCipherHashData->u32Offset,
                    pu8InputData + u32BufInUsedLen,
                    g_u32HashBaseBufferLen - pstCipherHashData->u32Offset);
            u32BufInUsedLen += g_u32HashBaseBufferLen - pstCipherHashData->u32Offset;
            pstCipherHashData->u32InputDataLen = g_u32HashBaseBufferLen;
            pstCipherHashData->u32Offset = 0;

            stTeecOperation.params[0].value.a = hHashHandle;
            stTeecOperation.params[1].tmpref.buffer = (HI_U8*)(pstCipherHashData->stMMZBuffer.u32StartVirAddr);
            stTeecOperation.params[1].tmpref.size = pstCipherHashData->u32InputDataLen;
            stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
            if( DX_SUCCESS != stTeecResult)
            {
                HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
                ret =  HI_FAILURE;
            }
        }
    }

    /* Move tail data to mmz buffer */
    pstCipherHashData->u32Offset = u32OffsetBak;
    u32Tmp = (u32InputDataLen+ pstCipherHashData->u32Offset) % g_u32HashBaseBufferLen;
    memcpy((HI_U8 *)pstCipherHashData->stMMZBuffer.u32StartVirAddr, pu8InputData + u32BufInUsedLen, u32Tmp);
    pstCipherHashData->u32Offset = u32Tmp;

    return HI_SUCCESS;
}

static HI_S32 CIPHER_HashFinal(HI_HANDLE hHashHandle, HI_U8 *pu8OutputHash)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_HASH_DATA_S *pstCipherHashData = &g_stCipherHashData;
    HI_U32 u32Tmp = 0;
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;
    HI_U32 u32TeeCmd;

    if(( NULL == pu8OutputHash ) || ( NULL == pstCipherHashData ))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        g_HashDevFd = -1;
        Cipher_HashMmzDeInit();
        return HI_FAILURE;
    }

    CHECK_CIPHER_INIT();

#if 0
    ret = Cipher_HashMsgPadding(pstCipherHashData);
    if ( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("Error, hash message padding failed!\n");
        g_HashDevFd = -1;
        Cipher_HashMmzDeInit();
        return ret;
    }

    /* the remained data length must be 64 bytes aligned */
    u32Tmp = (pstCipherHashData->u32Offset + pstCipherHashData->u32PaddingLen) % HASH_BLOCK_SIZE;
    if ( 0 != u32Tmp)
    {
        HI_ERR_CIPHER("Error, invalid length for hash final!\n");
        g_HashDevFd = -1;
        Cipher_HashMmzDeInit();
        return HI_FAILURE;
    }

    /* copy all the remained data to mmz buffer */
    memcpy((HI_U8 *)pstCipherHashData->stMMZBuffer.u32StartVirAddr + pstCipherHashData->u32Offset,
                    pstCipherHashData->u8Padding,
                    pstCipherHashData->u32PaddingLen);

#endif
    pstCipherHashData->u32InputDataLen = pstCipherHashData->u32Offset;// + pstCipherHashData->u32PaddingLen;
    pstCipherHashData->pu8Output= pu8OutputHash;

    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    u32TeeCmd = CMD_TZ_CIPHER_HASHUPDATE;
    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                            TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_NONE,
                                            TEEC_NONE);
    stTeecOperation.params[0].value.a = hHashHandle;
    stTeecOperation.params[1].tmpref.buffer = (HI_U8*)(pstCipherHashData->stMMZBuffer.u32StartVirAddr);
    stTeecOperation.params[1].tmpref.size = pstCipherHashData->u32InputDataLen;

    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        ret =  HI_FAILURE;
    }

    switch(pstCipherHashData->enShaType)
    {
        case HI_UNF_CIPHER_HASH_TYPE_SHA1:
        case HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1:
        {
            u32Tmp = 20;
            break;
        }
        case HI_UNF_CIPHER_HASH_TYPE_SHA256:
        case HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256:
        {
            u32Tmp = 32;
            break;
        }
        case HI_UNF_CIPHER_HASH_TYPE_IRDETO_CBCMAC:
        {
            u32Tmp = 16;
            break;
        }
        default:
        {
            u32Tmp = 20;
            break;
        }
    }
    u32TeeCmd = CMD_TZ_CIPHER_HASHFINAL;
    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_VALUE_INPUT,
                                                TEEC_MEMREF_TEMP_OUTPUT,
                                                TEEC_NONE,
                                                TEEC_NONE);

    stTeecOperation.params[0].value.a = hHashHandle;
    stTeecOperation.params[1].tmpref.buffer = pu8OutputHash;
    stTeecOperation.params[1].tmpref.size = u32Tmp;

    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        ret =  HI_FAILURE;
    }
    memcpy(pu8OutputHash, stTeecOperation.params[1].tmpref.buffer, u32Tmp);

    g_HashDevFd = -1;
    Cipher_HashMmzDeInit();

    return HI_SUCCESS;
}

static HI_S32 CIPHER_HmacKeyInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_U8 au8Hmackey[HASH_BLOCK_SIZE])
{
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE hHash = 0;

    if((pstHashAttr == NULL)||(pstHashAttr->pu8HMACKey == NULL)||(pstHashAttr->u32HMACKeyLen == 0))
    {
        HI_ERR_CIPHER("Invalid parameters.\n");
        g_HashDevFd = -1;
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /*key length is less than 64bytes, copy directly*/
    if(pstHashAttr->u32HMACKeyLen <= HASH_BLOCK_SIZE)
    {
        memcpy(au8Hmackey, pstHashAttr->pu8HMACKey, pstHashAttr->u32HMACKeyLen);
        u32HmacKeyLen = pstHashAttr->u32HMACKeyLen;
        return HI_SUCCESS;
    }

    /*key length more than 64bytes, calcute the hash result*/
    ret = CIPHER_HashInit(pstHashAttr, &hHash);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Cipher_HashInit failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    ret = CIPHER_HashUpdate(hHash, pstHashAttr->pu8HMACKey, pstHashAttr->u32HMACKeyLen);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Cipher_HashUpdate failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    ret = CIPHER_HashFinal(hHash,au8Hmackey);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Cipher_HashFinal failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    if ( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pstHashAttr->eShaType )
    {
        u32HmacKeyLen = 20;
    }
    else
    {
        u32HmacKeyLen = 32;
    }

    return HI_SUCCESS;
}

static HI_S32 CIPHER_HmacInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 i_key_pad[HASH_BLOCK_SIZE];
    HI_U32 i = 0;

    if( (NULL == pstHashAttr)
     || (NULL == pHashHandle)
     || (pstHashAttr->eShaType >= HI_UNF_CIPHER_HASH_TYPE_BUTT)
     || (NULL == pstHashAttr->pu8HMACKey)
     || (0 == pstHashAttr->u32HMACKeyLen))
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_INIT();

    /* Init hmac key */
    memset(g_au8HmacKey, 0, sizeof(g_au8HmacKey));
    ret = CIPHER_HmacKeyInit(pstHashAttr, g_au8HmacKey);
    if ( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("Hmac key initial failed!\n");
        g_HashDevFd = -1;
        return HI_FAILURE;
    }

    /* hash i_key_pad and message start */
    ret = CIPHER_HashInit(pstHashAttr, pHashHandle);
    if ( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("hash i_key_pad and message start failed!\n");
        return HI_FAILURE;
    }

    /* generate i_key_pad */
    memset(i_key_pad, 0x36, HASH_BLOCK_SIZE);
    for(i=0; i < u32HmacKeyLen; i++)
    {
        i_key_pad[i] = i_key_pad[i] ^ g_au8HmacKey[i];
    }

    /* hash i_key_pad update */
    ret = CIPHER_HashUpdate(*pHashHandle, i_key_pad, sizeof(i_key_pad));
    if ( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("hash i_key_pad and message update failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


static HI_S32 CIPHER_HmacUpdate(HI_HANDLE hHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    HI_S32 ret = HI_SUCCESS;

    ret = CIPHER_HashUpdate(hHashHandle, pu8InputData, u32InputDataLen);
    if ( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("hmac message update failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 CIPHER_HmacFinal(HI_HANDLE hHashHandle, HI_U8 *pu8Output)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_HASH_DATA_S *pstCipherHashData = &g_stCipherHashData;
    HI_HANDLE hHash = HI_INVALID_HANDLE;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_U8 hash_sum_1[HASH256_SIGNATURE_SIZE];
    HI_U8 o_key_pad[HASH_BLOCK_SIZE];
    HI_U32 i;

    if ( NULL == pu8Output )
    {
        HI_ERR_CIPHER("Invalid param input, hmac final failed!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    /* hash i_key_pad+message finished */
    memset(hash_sum_1, 0, sizeof(hash_sum_1));
    ret = CIPHER_HashFinal(hHashHandle, hash_sum_1);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Final i_key_pad+message failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    /* generate o_key_pad */
    memset(o_key_pad, 0x5c, HASH_BLOCK_SIZE);
    for(i=0; i < u32HmacKeyLen; i++)
    {
        o_key_pad[i] = o_key_pad[i] ^ g_au8HmacKey[i];
    }

    /* hash o_key_pad+hash_sum_1 start */
    memset(&stHashAttr, 0, sizeof(stHashAttr));
    stHashAttr.eShaType = pstCipherHashData->enShaType;
    ret = CIPHER_HashInit(&stHashAttr, &hHash);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Init o_key_pad+hash_sum_1 failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    /* hash o_key_pad */
    ret = CIPHER_HashUpdate(hHash, (HI_U8 *)o_key_pad, HASH_BLOCK_SIZE);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Update o_key_pad failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    /* hash hash_sum_1 */
    if ( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pstCipherHashData->enShaType )
    {
        ret = CIPHER_HashUpdate(hHash, (HI_U8 *)hash_sum_1, HASH1_SIGNATURE_SIZE);
    }
    else
    {
        ret = CIPHER_HashUpdate(hHash, (HI_U8 *)hash_sum_1, HASH256_SIGNATURE_SIZE);
    }

    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Update hash_sum_1 failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    /* hash o_key_pad+hash_sum_1 finished */
    ret = CIPHER_HashFinal(hHash, pu8Output);
    if(ret != HI_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Final o_key_pad+hash_sum_1 failure, ret=%d\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

//the output is the last 8 bytes only
static HI_S32 AES_Encrypt(HI_HANDLE hCipherHandle, HI_U8 *input, HI_U32 datalen, HI_U8 *output)
{
    HI_U32 u32InputAddrPhy = 0;
    HI_U32 u32MmzCached = 0;
    HI_U8* pu8InputAddrVir = NULL;
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32EncryptDataLen = 0;
    HI_U32 u32LeftDataLen = 0;
    HI_U32 i = 0;
    HI_U32 u32OutPhy = 0;
    HI_U32 u32BlockNum = 0;

    u32InputAddrPhy = (HI_U32)HI_MMZ_New(MAX_DATA_LEN, 0, NULL, "CIPHER_BufIn");
    if (0 == u32InputAddrPhy)
    {
        HI_ERR_CIPHER("mmz new for u32InputAddrPhy failed!\n");
        return HI_FAILURE;
    }
    pu8InputAddrVir = HI_MMZ_Map(u32InputAddrPhy, u32MmzCached);
    if( NULL == pu8InputAddrVir )
    {
        HI_MMZ_Unmap(u32InputAddrPhy);
        HI_MMZ_Delete(u32InputAddrPhy);
        HI_ERR_CIPHER("mmz map for pu8InputAddrVir failed!\n");
        return HI_FAILURE;
    }

#ifdef HI_TEE_SUPPORT
    HI_U32 u32SecOutAddrPhy = 0;

    (HI_VOID)HI_SEC_MMZ_Init();

    u32SecOutAddrPhy = (HI_U32)HI_SEC_MMZ_New(MAX_DATA_LEN, SEC_MMZ_ZONE, "CIPHER_BufOut");
    if (0 == u32SecOutAddrPhy)
    {
        HI_ERR_CIPHER("Error: Get phyaddr for outPut failed!\n");
        HI_MMZ_Unmap(u32InputAddrPhy);
        HI_MMZ_Delete(u32InputAddrPhy);
        (HI_VOID)HI_SEC_MMZ_DeInit();
        return HI_FAILURE;;
    }
    u32OutPhy = u32SecOutAddrPhy;
#else
    HI_U32 u32OutPutAddrPhy = 0;
    HI_U8* pu8OutputAddrVir = NULL;

    u32OutPutAddrPhy = (HI_U32)HI_MMZ_New(MAX_DATA_LEN, 0, NULL, "CIPHER_BufOut");
    if (0 == u32OutPutAddrPhy)
    {
        HI_ERR_CIPHER("mmz new for u32OutPutAddrPhy failed!\n");
        HI_MMZ_Unmap(u32InputAddrPhy);
        HI_MMZ_Delete(u32InputAddrPhy);
        return HI_FAILURE;
    }
    pu8OutputAddrVir = HI_MMZ_Map(u32OutPutAddrPhy, u32MmzCached);
    if( NULL == pu8OutputAddrVir )
    {
        HI_ERR_CIPHER("mmz map for pu8OutputAddrVir failed!\n");
        HI_MMZ_Unmap(u32InputAddrPhy);
        HI_MMZ_Delete(u32InputAddrPhy);
        HI_MMZ_Delete(u32OutPutAddrPhy);
        return HI_FAILURE;
    }
    memset(pu8OutputAddrVir, 0, MAX_DATA_LEN);
    u32OutPhy = u32OutPutAddrPhy;
#endif

    u32LeftDataLen = datalen;
    u32BlockNum = (datalen + MAX_DATA_LEN - 1) / MAX_DATA_LEN;
    for(i = 0; i < u32BlockNum; i++)
    {
        u32EncryptDataLen = u32LeftDataLen >= MAX_DATA_LEN ? MAX_DATA_LEN : u32LeftDataLen;
        u32LeftDataLen -= u32EncryptDataLen;
        memcpy(pu8InputAddrVir, input + i * MAX_DATA_LEN, u32EncryptDataLen);
        ret = HI_MPI_CIPHER_Encrypt(hCipherHandle, u32InputAddrPhy, u32OutPhy, u32EncryptDataLen);
        if(ret != HI_SUCCESS)
        {
            HI_ERR_CIPHER("Cipher encrypt failed!\n");
            goto CIPHER_RELEASE_BUF;
        }
    }

#ifndef HI_TEE_SUPPORT
    memcpy(output, pu8OutputAddrVir + u32EncryptDataLen - 16, 16);
#endif

CIPHER_RELEASE_BUF:

#ifdef HI_TEE_SUPPORT
    HI_SEC_MMZ_Delete(u32SecOutAddrPhy);
   (HI_VOID)HI_SEC_MMZ_DeInit();
#else
    HI_MMZ_Unmap(u32OutPutAddrPhy);
    HI_MMZ_Delete(u32OutPutAddrPhy);
#endif

    HI_MMZ_Unmap(u32InputAddrPhy);
    HI_MMZ_Delete(u32InputAddrPhy);

    return ret;
}

static HI_S32 generate_subkey(HI_HANDLE hCipherHandle, HI_U8 *K1, HI_U8 *K2)
{
    HI_U8 L[16];
    HI_U8 Z[16];
    HI_U8 tmp[16];
    HI_U32 u32DataLen = 16;
    HI_S32 ret = HI_SUCCESS;

    if(K1 == NULL || K2 == NULL)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_FAILURE;
    }

    memset(tmp, 0x0, sizeof(tmp));
    memset(L, 0x0, sizeof(L));
    memset(Z, 0x0, sizeof(Z));

    ret = AES_Encrypt(hCipherHandle, Z, u32DataLen, L);
    if(ret != HI_SUCCESS)
    {
        return ret;
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

HI_S32 HI_MPI_CIPHER_Init(HI_VOID)
{
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_INIT;
    TEEC_Result stTeecResult;

    HI_CIPHER_LOCK();

    memset(&s_stTeecContext, 0, sizeof(s_stTeecContext));
    memset(&s_stTeecSession, 0, sizeof(s_stTeecSession));
    memset(&stTeecResult, 0, sizeof(stTeecResult));

    if (g_CipherDevFd > 0)
    {
        HI_CIPHER_UNLOCK();
        return HI_SUCCESS;
    }

    stTeecResult = TEEC_InitializeContext(NULL, &s_stTeecContext);
    if(TEEC_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec Initialize context failed!\n");
        HI_CIPHER_UNLOCK();
        return HI_FAILURE;;
    }

    stTeecResult = TEEC_OpenSession(&s_stTeecContext,
                                    &s_stTeecSession,
                                    &s_stTeecCipherUUID,
                                    TEEC_LOGIN_PUBLIC,
                                    NULL,
                                    NULL,
                                    NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec open session failed!\n");
        (void)TEEC_FinalizeContext(&s_stTeecContext);
        HI_CIPHER_UNLOCK();
        return HI_FAILURE;
    }

    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession,
                                        u32TeeCmd,
                                        NULL,
                                        NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", CMD_TZ_CIPHER_INIT);
        (void)TEEC_CloseSession(&s_stTeecSession);
        (void)TEEC_FinalizeContext(&s_stTeecContext);
        HI_CIPHER_UNLOCK();
        return HI_FAILURE;
    }

    g_CipherDevFd = 1;

    HI_CIPHER_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_DeInit(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    TEEC_Result stTeecResult;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_DEINIT;

    HI_CIPHER_LOCK();

    if (g_CipherDevFd <= 0)
    {
        HI_CIPHER_UNLOCK();
        return HI_SUCCESS;
    }

    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, NULL, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        HI_CIPHER_UNLOCK();
        return HI_FAILURE;
    }

    (HI_VOID)TEEC_CloseSession(&s_stTeecSession);
    (HI_VOID)TEEC_FinalizeContext(&s_stTeecContext);

    g_CipherDevFd = -1;

    HI_CIPHER_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_EncryptHDCPKey(HI_UNF_HDCP_HDCPKEY_S *pstHdcpKey, HI_BOOL bIsUseOTPRootKey, HI_U8 u8OutEncryptKey[332])
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_HDCP_KEY_TRANSFER_S stDecryptHdcpKeyTransfer;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_ENCRYPTHDCPKEY;
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;

    if ( NULL == u8OutEncryptKey)
    {
        HI_ERR_CIPHER("Invalid param , null pointer!\n");
        return HI_FAILURE;
    }

    if ( (HI_TRUE != bIsUseOTPRootKey) && (HI_FALSE != bIsUseOTPRootKey) )
    {
        HI_ERR_CIPHER("Invalid param, unexpected bIsUseOTPRootKey!");
        return HI_FAILURE;
    }

    if(g_CipherDevFd <0)
    {
        HI_ERR_CIPHER("Cipher not init!\n");
        return HI_FAILURE;
    }

    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));
    memset(&stDecryptHdcpKeyTransfer, 0x0, sizeof(CIPHER_HDCP_KEY_TRANSFER_S));

    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_MEMREF_TEMP_INOUT,
                                                TEEC_NONE,
                                                TEEC_NONE,
                                                TEEC_NONE);

    stDecryptHdcpKeyTransfer.bIsUseOTPRootKey = bIsUseOTPRootKey;
    memcpy(&stDecryptHdcpKeyTransfer.stHdcpKey, pstHdcpKey, sizeof(HI_UNF_HDCP_HDCPKEY_S));

    stTeecOperation.params[0].tmpref.buffer = &stDecryptHdcpKeyTransfer;
    stTeecOperation.params[0].tmpref.size = sizeof(stDecryptHdcpKeyTransfer);
    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        return HI_FAILURE;
    }

    memcpy(u8OutEncryptKey, stDecryptHdcpKeyTransfer.u8FlashEncryptedHdcpKey, 332);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_LoadHDCPKey(HI_U8 *pu8HDCPKey, HI_U32 u32Len)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_LOADHDCPKEY;
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;

    if ( (NULL == pu8HDCPKey) || (332 != u32Len))
    {
        HI_ERR_CIPHER("Invalid param , null pointer!\n");
        return HI_FAILURE;
    }

    if(g_CipherDevFd <0)
    {
        HI_ERR_CIPHER("Cipher is not init!\n");
        return HI_FAILURE;
    }

    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_MEMREF_TEMP_INOUT,
                                                TEEC_NONE,
                                                TEEC_NONE,
                                                TEEC_NONE);
    stTeecOperation.params[0].tmpref.buffer = pu8HDCPKey;
    stTeecOperation.params[0].tmpref.size = u32Len;
    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_CreateHandle(HI_HANDLE* phCipher, const HI_UNF_CIPHER_ATTS_S *pstCipherAttr)
{
    HI_S32 ret = HI_SUCCESS;
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_CREATEHANDLE;

    if ( ( NULL == phCipher ) || ( NULL == pstCipherAttr ) )
    {
        HI_ERR_CIPHER("Invalid param, null pointer!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_INIT();

    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_VALUE_INOUT,
                                                TEEC_MEMREF_TEMP_INPUT,
                                                TEEC_NONE,
                                                TEEC_NONE);
    stTeecOperation.params[0].value.a = *phCipher;
    stTeecOperation.params[1].tmpref.buffer = (HI_VOID *)pstCipherAttr;
    stTeecOperation.params[1].tmpref.size = sizeof(HI_UNF_CIPHER_ATTS_S);

    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        ret =  HI_FAILURE;
    }

    *phCipher = stTeecOperation.params[0].value.a;

    return ret;
}

HI_S32 HI_MPI_CIPHER_DestroyHandle(HI_HANDLE hCipher)
{
    HI_S32 ret = HI_SUCCESS;
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_DESTROYHANDLE;

    CHECK_CIPHER_INIT();

    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_VALUE_INPUT,
                                                TEEC_NONE,
                                                TEEC_NONE,
                                                TEEC_NONE);
    stTeecOperation.params[0].value.a = hCipher;
    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 HI_MPI_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl)
{
    HI_S32 ret = HI_SUCCESS;
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_CONFIGHANDLE;

    if (NULL == pstCtrl)
    {
        HI_ERR_CIPHER("para pstCtrl is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
#if 0
    if(HI_TRUE == pstCtrl->bKeyByCA)
    {
        ret = HI_MPI_ADVCA_SetKeyladderSessionKey(pstCtrl, hCipher);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CIPHER("mpi set keyladder session key failed!\n");
            return HI_FAILURE;
        }
    }
#endif

    CHECK_CIPHER_INIT();

    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_VALUE_INPUT,
                                                TEEC_MEMREF_TEMP_INPUT,
                                                TEEC_NONE,
                                                TEEC_NONE);
    stTeecOperation.params[0].value.a = (HI_U32)hCipher;
    stTeecOperation.params[1].tmpref.buffer = (HI_VOID *)pstCtrl;
    stTeecOperation.params[1].tmpref.size = sizeof(HI_UNF_CIPHER_CTRL_S);

    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_ENCRYPT;

    CHECK_CIPHER_INIT();

    if(0 == (hCipher & 0x00ff))
    {
        if ( 16 != u32ByteLength )
        {
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    if ( u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN || u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if((0 == u32SrcPhyAddr) || (0 == u32DestPhyAddr))
    {
        HI_ERR_CIPHER("Invali parameter, null address input!\n");
        return HI_FAILURE;
    }

    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_VALUE_INPUT,
                                                TEEC_VALUE_INPUT,
                                                TEEC_NONE,
                                                TEEC_NONE);
    stTeecOperation.params[0].value.a = (HI_U32)hCipher;
    stTeecOperation.params[0].value.b= u32SrcPhyAddr;
    stTeecOperation.params[1].value.a = u32DestPhyAddr;
    stTeecOperation.params[1].value.b = u32ByteLength;
    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 HI_MPI_CIPHER_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_DECRYPT;

    CHECK_CIPHER_INIT();

    if ( 0 == (hCipher & 0x00ff) )
    {
        if ( 16 != u32ByteLength )
        {
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }
    if (u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN || u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    if((0 == u32SrcPhyAddr) || (0 == u32DestPhyAddr))
    {
        HI_ERR_CIPHER("Invali parameter, null address input!\n");
        return HI_FAILURE;
    }
    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_VALUE_INPUT,
                                                TEEC_VALUE_INPUT,
                                                TEEC_NONE,
                                                TEEC_NONE);
    stTeecOperation.params[0].value.a = (HI_U32)hCipher;
    stTeecOperation.params[0].value.b= u32SrcPhyAddr;
    stTeecOperation.params[1].value.a = u32DestPhyAddr;
    stTeecOperation.params[1].value.b = u32ByteLength;

    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_EncryptMulti(HI_HANDLE hCipher, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum)
{
    HI_UNF_CIPHER_DATA_S *pPkgTmp = NULL;
    HI_U32 i = 0;
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_ENCRYPTMULTI;
    TEEC_Context stTeecContext;
    TEEC_Session stTeecSession;

    if((NULL == pstDataPkg) ||(0 == u32DataPkgNum))
    {
        HI_ERR_CIPHER("Invalid param input!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if ( 0 == (hCipher & 0x00ff) )
    {
        HI_ERR_CIPHER("invalid chnid 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_INIT();
    for (i = 0; i < u32DataPkgNum; i++)
    {
        pPkgTmp = pstDataPkg + i;
        if (pPkgTmp->u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN || pPkgTmp->u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
        {
            HI_ERR_CIPHER("Pkg%d 's length(%d) invalid.\n", i, pPkgTmp->u32ByteLength);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    stTeecResult = TEEC_InitializeContext(NULL, &stTeecContext);
    if(TEEC_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec Initialize context failed!\n");
        return HI_FAILURE;
    }

    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_VALUE_INPUT,
                                                TEEC_MEMREF_TEMP_INOUT,
                                                TEEC_VALUE_INPUT,
                                                TEEC_NONE);
    stTeecOperation.params[0].value.a = (HI_U32)hCipher;
    stTeecOperation.params[1].tmpref.buffer = (HI_VOID *)pstDataPkg;
    stTeecOperation.params[1].tmpref.size = u32DataPkgNum * sizeof(HI_UNF_CIPHER_DATA_S);
    stTeecOperation.params[2].value.a = u32DataPkgNum;

    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_DecryptMulti(HI_HANDLE hCipher, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum)
{
    HI_UNF_CIPHER_DATA_S *pPkgTmp = NULL;
    HI_U32 i = 0;
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_DECRYPTMULTI;

    if((NULL == pstDataPkg) ||(0 == u32DataPkgNum))
    {
        HI_ERR_CIPHER("Invalid param input!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(0 == (hCipher & 0x00ff))
    {
        HI_ERR_CIPHER("invalid chnid 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_INIT();
    for (i = 0; i < u32DataPkgNum; i++)
    {
        pPkgTmp = pstDataPkg + i;
        if (pPkgTmp->u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN || pPkgTmp->u32ByteLength > HI_UNF_CIPHER_MAX_CRYPT_LEN)
        {
            HI_ERR_CIPHER("Pkg[%d] 's length(=%d) invalid.\n", i, pPkgTmp->u32ByteLength);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }
    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    stTeecResult = TEEC_InitializeContext(NULL, &s_stTeecContext);
    if(TEEC_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec Initialize context failed!\n");
        return HI_FAILURE;
    }

    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_VALUE_INPUT,
                                                TEEC_MEMREF_TEMP_INOUT,
                                                TEEC_VALUE_INPUT,
                                                TEEC_NONE);
    stTeecOperation.params[0].value.a = (HI_U32)hCipher;
    stTeecOperation.params[1].tmpref.buffer = (HI_VOID *)pstDataPkg;
    stTeecOperation.params[1].tmpref.size = u32DataPkgNum * sizeof(HI_UNF_CIPHER_DATA_S);
    stTeecOperation.params[2].value.a = u32DataPkgNum;
    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_GetRandomNumber(HI_U32 *pu32RandomNumber, HI_U32 u32TimeOutUs)
{
    HI_S32 ret = HI_SUCCESS;
    HI_SYS_VERSION_S stVersion;
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_GETRANDOMNUMBER;

    if (NULL == pu32RandomNumber)
    {
        HI_ERR_CIPHER("pu32RandomNumber is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    CHECK_CIPHER_INIT();
    memset(&stVersion, 0, sizeof(stVersion));

    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_VALUE_INOUT,
                                                TEEC_NONE,
                                                TEEC_NONE,
                                                TEEC_NONE);
    stTeecOperation.params[0].value.a = *pu32RandomNumber;
    stTeecOperation.params[0].value.b = u32TimeOutUs;

    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        return HI_FAILURE;
    }

    *pu32RandomNumber = stTeecOperation.params[0].value.a;

    return HI_SUCCESS;
}


HI_S32 HI_MPI_CIPHER_GetHandleConfig(HI_HANDLE hCipherHandle, HI_UNF_CIPHER_CTRL_S* pstCtrl)
{
    TEEC_Result stTeecResult;
    TEEC_Operation stTeecOperation;
    HI_U32 u32TeeCmd = CMD_TZ_CIPHER_GETHANDLECONFIG;

    if(NULL == pstCtrl)
    {
        HI_ERR_CIPHER("Invalid param, null pointer input!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
    CHECK_CIPHER_INIT();

    memset(&stTeecResult, 0, sizeof(stTeecResult));
    memset(&stTeecOperation, 0, sizeof(stTeecOperation));

    stTeecOperation.started = 1;
    stTeecOperation.paramTypes = TEEC_PARAM_TYPES(
                                                TEEC_VALUE_INPUT,
                                                TEEC_MEMREF_TEMP_INOUT,
                                                TEEC_NONE,
                                                TEEC_NONE);
    stTeecOperation.params[0].value.a = (HI_U32)hCipherHandle;
    stTeecOperation.params[1].tmpref.buffer = (HI_VOID *)pstCtrl;
    stTeecOperation.params[1].tmpref.size = sizeof(HI_UNF_CIPHER_CTRL_S);
    stTeecResult = TEEC_InvokeCommand(&s_stTeecSession, u32TeeCmd, &stTeecOperation, NULL);
    if( DX_SUCCESS != stTeecResult)
    {
        HI_ERR_CIPHER("Teec invoke command failed, cmd = 0x%08x\n", u32TeeCmd);
        return HI_FAILURE;
    }

    *pstCtrl = *(HI_UNF_CIPHER_CTRL_S*)stTeecOperation.params[1].tmpref.buffer;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_CalcMAC(HI_HANDLE hCipherHandle,
                                HI_U8 *pInputData,
                                HI_U32 u32InputDataLen,
                                HI_U8 *pOutputMAC,
                                HI_BOOL bIsLastBlock)
{
    HI_U8 X[16], M_last[16], padded[16];
    static HI_U8 K1[16] = {0};
    static HI_U8 K2[16] = {0};
    HI_U32 n, i, flag;
    HI_U8 u8TmpBuf[16];
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_CIPHER_CTRL_S stCipherCtrl;
    static HI_BOOL bIsFirstBlock = HI_TRUE;

    CHECK_CIPHER_INIT();

    memset(&stCipherCtrl, 0, sizeof(stCipherCtrl));
    memset(u8TmpBuf, 0, sizeof(u8TmpBuf));
    memset(X, 0, sizeof(X));
    memset(M_last, 0, sizeof(M_last));
    memset(padded, 0, sizeof(padded));

    if(bIsFirstBlock) //if first block, reset the configure handle and generate the subkey again
    {
        ret = HI_MPI_CIPHER_GetHandleConfig(hCipherHandle, &stCipherCtrl);
        ret |= HI_MPI_CIPHER_ConfigHandle(hCipherHandle, &stCipherCtrl);
        if(ret != HI_SUCCESS)
        {
            return ret;
        }

        for(i=0; i<sizeof(HI_UNF_CIPHER_CTRL_S); i++)
        {
            HI_PRINT("%02X ", ((HI_U8*)&stCipherCtrl)[i]);
        }

        ret = generate_subkey(hCipherHandle, K1, K2);
        if(ret != HI_SUCCESS)
        {
            return ret;
        }

        //After genreate the subkey, reset the configure handle
        ret = HI_MPI_CIPHER_GetHandleConfig(hCipherHandle, &stCipherCtrl);
        ret |= HI_MPI_CIPHER_ConfigHandle(hCipherHandle, &stCipherCtrl);
        if(ret != HI_SUCCESS)
        {
            return ret;
        }
        bIsFirstBlock = HI_FALSE;
    }

    if(!bIsLastBlock)
    {
       ret = AES_Encrypt(hCipherHandle, pInputData, u32InputDataLen, u8TmpBuf); /* X := AES-128(KEY, Y); */
       if(ret != HI_SUCCESS)
       {
            return ret;
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
           ret = AES_Encrypt(hCipherHandle, pInputData, 16 * (n - 1), u8TmpBuf); /* X := AES-128(KEY, Y); */
           if(ret != HI_SUCCESS)
           {
                return ret;
           }
        }

        ret = AES_Encrypt(hCipherHandle, M_last, 16, X);
        if(ret != HI_SUCCESS)
        {
            return ret;
        }

        for ( i = 0; i < 16; i++ )
        {
            pOutputMAC[i] = X[i];
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_CIPHER_CbcMac_Auth(HI_U8 *pu8RefCbcMac, HI_U32 u32AppLen)
{
    HI_ERR_CIPHER("Unsupport!\n");

    return HI_FAILURE;
}

HI_S32 HI_MPI_CIPHER_HashInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    if ( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstHashAttr->eShaType) || (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstHashAttr->eShaType) )
    {
        return CIPHER_HashInit(pstHashAttr, pHashHandle);
    }
    else
    {
        return CIPHER_HmacInit(pstHashAttr, pHashHandle);
    }
}

HI_S32 HI_MPI_CIPHER_HashUpdate(HI_HANDLE hHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    CIPHER_HASH_DATA_S *pstCipherHashData = &g_stCipherHashData;

    if ( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstCipherHashData->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstCipherHashData->enShaType ) )
    {
        return CIPHER_HashUpdate(hHashHandle, pu8InputData, u32InputDataLen);
    }
    else
    {
        return CIPHER_HmacUpdate(hHashHandle, pu8InputData, u32InputDataLen);
    }
}

HI_S32 HI_MPI_CIPHER_HashFinal(HI_HANDLE hHashHandle, HI_U8 *pu8OutputHash)
{
    CIPHER_HASH_DATA_S *pstCipherHashData = &g_stCipherHashData;

    if ( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pstCipherHashData->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pstCipherHashData->enShaType) )
    {
        return CIPHER_HashFinal(hHashHandle, pu8OutputHash);
    }
    else
    {
        return CIPHER_HmacFinal(hHashHandle, pu8OutputHash);
    }
}

