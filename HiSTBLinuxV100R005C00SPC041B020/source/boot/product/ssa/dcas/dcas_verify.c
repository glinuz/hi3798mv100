/******************************************************************************

          Copyright (C), 2001-2015, Hisilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : dcas_verify.c
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2016-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2016-07-29  l00163273  N/A               Create this file.
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

#include <malloc.h>

#include "dcas_debug.h"
#include "dcas_config.h"
#include "dcas_cipher.h"
#include "dcas_verify.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SSA_CRC_POLYNOMIAL          (0x04c11db7L)
#define SSA_CRC_TABLELEN            (256)

static HI_ULONG s_aulCrcTable[SSA_CRC_TABLELEN] = {0};

static HI_U32 ssaCRC32(HI_U8* pu8Buffer, HI_U32 u32Length)
{
    HI_ULONG ulCRC = 0xffffffff;
    HI_ULONG ulCrcAccum = 0;
    HI_U32 j = 0;

    static HI_U32 i = 0;

    for (i = 0; i < SSA_CRC_TABLELEN; i++)
    {
        ulCrcAccum = (i << 24);

        for (j = 0; j < 8;  j++)
        {
            if (ulCrcAccum & 0x80000000L)
            {
                ulCrcAccum = ( ulCrcAccum << 1 ) ^ SSA_CRC_POLYNOMIAL;
            }
            else
            {
                ulCrcAccum = (ulCrcAccum << 1);
            }
        }

        s_aulCrcTable[i] = ulCrcAccum;
    }

    while (u32Length--)
    {
        ulCRC = (ulCRC << 8) ^ s_aulCrcTable[((ulCRC >> 24) ^ *pu8Buffer ++) & 0xff];
    }

    return ulCRC;
}

HI_S32 SSA_ConfigExternKey(SSA_FLASH_OPENPARAM_S* pstOpenParam, HI_U8* pu8Buffer, HI_U32 u32Size)
{
    HI_U32 u32DataSize = 0;
    HI_HANDLE hFlashHandle = HI_INVALID_HANDLE;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_CHECK_PARAM(HI_NULL_PTR == pstOpenParam);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu8Buffer);
    SSA_CHECK_PARAM(SSA_DCAS_LEN_RSAPUBLICKEY > u32Size);

    hFlashHandle = SSA_FLASH_Open(pstOpenParam);
    if (HI_INVALID_HANDLE == hFlashHandle)
    {
        SSA_PrintFuncErr(SSA_FLASH_Open, SSA_ERR_FLASH);
        return SSA_ERR_FLASH;
    }

    u32DataSize = SSA_FLASH_Read(hFlashHandle, 0ULL, pu8Buffer, u32Size, SSA_DCAS_LEN_RSAPUBLICKEY);
    if (SSA_DCAS_LEN_RSAPUBLICKEY > u32DataSize)
    {
        SSA_PrintFuncErr(SSA_FLASH_Read, SSA_ERR_FLASH);

        SSA_FLASH_Close(hFlashHandle);
        hFlashHandle = HI_NULL_PTR;

        return SSA_ERR_FLASH;
    }

    SSA_FLASH_Close(hFlashHandle);
    hFlashHandle = HI_NULL_PTR;

    s32Ret = SSA_CIPHER_SetRSAPublicKey(pu8Buffer, SSA_DCAS_LEN_RSAPUBLICKEY);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_CIPHER_SetRSAPublicKey, s32Ret);
        return s32Ret;
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSA_VerifySectionTail(HI_U8* pu8PrevTail, HI_U8* pu8CurrTail)
{
    HI_U32 u32CRCValue = 0;
    SSA_SECTIONTAIL_S* pstPrevHead = (SSA_SECTIONTAIL_S*)pu8PrevTail;
    SSA_SECTIONTAIL_S* pstCurrTail = (SSA_SECTIONTAIL_S*)pu8CurrTail;

    SSA_DBG_FuncEnter();

    SSA_CHECK_PARAM(HI_NULL_PTR == pu8CurrTail);

    if (!SSA_CHECK_TAILSECTION(pu8CurrTail))
    {
        SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
        return SSA_ERR_ILLEGAL_IMAGE;
    }

    u32CRCValue = ssaCRC32(pu8CurrTail, sizeof(SSA_SECTIONTAIL_S) - sizeof(pstCurrTail->u32CRC32));
    if (u32CRCValue != pstCurrTail->u32CRC32)
    {
        SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
        return SSA_ERR_ILLEGAL_IMAGE;
    }

    SSA_Print32Hex(pstCurrTail->u32CRC32);
    SSA_Print32Hex(u32CRCValue);

    if ((0 == pstCurrTail->u32Totalsection)
        || (0 == pstCurrTail->u32SignatureLen)
        || (pstCurrTail->u32CurrentsectionID >= pstCurrTail->u32Totalsection))
    {
        SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
        return SSA_ERR_ILLEGAL_IMAGE;
    }

    if ((1 < pstCurrTail->u32Totalsection)
        && (0 == pstCurrTail->u32SectionSize))
    {
        SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
        return SSA_ERR_ILLEGAL_IMAGE;
    }

    if (HI_NULL_PTR != pstPrevHead)
    {
        if ((pstPrevHead->u32SectionSize != pstCurrTail->u32SectionSize)
            || (pstPrevHead->u32Totalsection != pstCurrTail->u32Totalsection))
        {
            SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
            return SSA_ERR_ILLEGAL_IMAGE;
        }
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSA_VerifySectionData(HI_U8* pu8Data, HI_U8* pu8Tail)
{
    SSA_SECTIONTAIL_S* pstTail = (SSA_SECTIONTAIL_S*)pu8Tail;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    SSA_CHECK_PARAM(HI_NULL_PTR == pu8Data);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu8Tail);

    s32Ret = SSA_CIPHER_HashVerify(pstTail->u32HashType, pu8Data, pstTail->u32SignedDataLength, pstTail->au8Sha);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_CIPHER_HashVerify, s32Ret);
        return s32Ret;
    }

    SSA_PrintBuffer("RSR Signature:", pstTail->au8Signature, pstTail->u32SignatureLen);
    s32Ret = SSA_CIPHER_RSAVerify(pu8Data, pstTail->u32SignedDataLength, pstTail->au8Signature, pstTail->u32SignatureLen);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_CIPHER_RSAVerify, s32Ret);
        return s32Ret;
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

