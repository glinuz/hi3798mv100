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

#include "dcas_debug.h"
#include "dcas_config.h"
#include "dcas_flash.h"
#include "dcas_cipher.h"
#include "dcas_verify.h"
#include "dcas_image.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static HI_S32 ssaGetSignData
(
    HI_HANDLE           hFlashHandle,
    HI_U8*              pu8Buffer,
    HI_U32              u32BufferSize
)
{
    HI_U32 u32Index = 0;
    HI_U32 u32LeftSize = 0;
    HI_U32 u32ReadSize = 0;
    HI_U32 u32PartSize = 0;
    HI_U64 u32ReadAddr = 0;
    HI_U32 u32TotalSize = 0;
    HI_U8* pu8CurrTail = HI_NULL_PTR;
    HI_U8* pu8PrevTail = HI_NULL_PTR;
    SSA_SECTIONTAIL_S* pstTailHead = HI_NULL_PTR;
    HI_U8* pu8TailHead = pu8Buffer;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    s32Ret = SSA_FLASH_GetPartitionSize(hFlashHandle, &u32PartSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_FLASH_GetPartitionSize, s32Ret);
        return s32Ret;
    }

    if (SSA_DCAS_FIX_PAGESIZE > u32PartSize)
    {
        SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
        return SSA_ERR_ILLEGAL_IMAGE;
    }

    while (u32ReadAddr <= u32PartSize - SSA_DCAS_FIX_PAGESIZE)
    {
        u32ReadSize = SSA_FLASH_Read(hFlashHandle, u32ReadAddr, pu8Buffer, u32BufferSize, SSA_DCAS_FIX_PAGESIZE);
        if (SSA_DCAS_FIX_PAGESIZE > u32ReadSize)
        {
            SSA_PrintFuncErr(SSA_FLASH_Read, SSA_ERR_FLASH);
            return SSA_ERR_FLASH;
        }

        pu8TailHead = pu8Buffer;
        u32LeftSize = u32ReadSize;

        while (u32LeftSize >= SSA_DCAS_FIX_PAGESIZE)
        {
            if (SSA_CHECK_TAILSECTION(pu8TailHead))
            {
                pstTailHead = (SSA_SECTIONTAIL_S*)pu8TailHead;
                break;
            }

            pu8TailHead += SSA_DCAS_FIX_PAGESIZE;
            u32ReadAddr += SSA_DCAS_FIX_PAGESIZE;
            u32LeftSize -= SSA_DCAS_FIX_PAGESIZE;

            if (HI_NULL_PTR != pstTailHead)
            {
                break;
            }
        }

        if (HI_NULL_PTR != pstTailHead)
        {
            break;
        }
    }

    if ((HI_NULL_PTR == pstTailHead)
        || (0 == pstTailHead->u32Totalsection))
    {
        SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
        return SSA_ERR_ILLEGAL_IMAGE;
    }

    u32TotalSize = pstTailHead->u32Totalsection * SSA_DCAS_FIX_PAGESIZE;
    if (u32TotalSize > u32LeftSize)
    {
        SSA_PrintInfo("No need to read again!");

        u32ReadSize  = SSA_FLASH_Read(hFlashHandle, u32ReadAddr, pu8Buffer, u32BufferSize, u32TotalSize);
        if (u32TotalSize > u32ReadSize)
        {
            SSA_PrintFuncErr(SSA_FLASH_Read, SSA_ERR_FLASH);
            return SSA_ERR_FLASH;
        }

        pu8TailHead = pu8Buffer;
        pstTailHead = (SSA_SECTIONTAIL_S*)pu8TailHead;

        if (0 == pstTailHead->u32Totalsection)
        {
            SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
            return SSA_ERR_ILLEGAL_IMAGE;
        }
    }

    pu8CurrTail = pu8TailHead;

    for (u32Index = 0; u32Index < pstTailHead->u32Totalsection; u32Index ++)
    {
        s32Ret = SSA_VerifySectionTail(pu8PrevTail, pu8CurrTail);
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(SSA_VerifySectionTail, s32Ret);
            return s32Ret;
        }
        pu8CurrTail = pu8CurrTail + SSA_DCAS_FIX_PAGESIZE;
        pu8PrevTail = pu8CurrTail;
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 ssaImageVerify
(
    HI_HANDLE           hDataFlashHandle,
    HI_HANDLE           hSignFlashHandle,
    HI_U8*              pu8DataBuffer,
    HI_U32              u32DataBufferSize,
    HI_U8*              pu8SignBuffer,
    HI_U32              u32SignBufferSize,
    SSA_VERIFYMODE_E    enVerifyMode
)
{
    HI_U32 u32Index = 0;
    HI_U32 u32Random = 0;
    HI_U32 u32StepSize = 1;
    HI_U32 u32FistStepSize = 1;
    HI_U32 u32ReadSize = 0;
    HI_U64 u64SectionHead = 0;
    HI_U32 u32ReturnSize = 0;
    HI_U8* pu8SectionTail = HI_NULL_PTR;
    SSA_SECTIONTAIL_S* pstSectionTail = (SSA_SECTIONTAIL_S*)pu8SignBuffer;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    s32Ret = SSA_CIPHER_Init();
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_CIPHER_Init, s32Ret);
        return s32Ret;
    }

    if (SSA_VERIFYMODE_SAMPLING == enVerifyMode)
    {
        s32Ret = SSA_CIPHER_GetRandomNum(&u32Random);
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(SSA_CIPHER_GetRandomNum, s32Ret);

            SSA_CHECK(SSA_CIPHER_DeInit());
            return s32Ret;
        }

        u32FistStepSize = u32Random % SSA_DCAS_SAMPLING_STEPSIZE + 1;
        u32StepSize = SSA_DCAS_SAMPLING_STEPSIZE;
    }

    s32Ret = ssaGetSignData(hSignFlashHandle, pu8SignBuffer, u32SignBufferSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(ssaGetSignData, s32Ret);

        SSA_CHECK(SSA_CIPHER_DeInit());
        return s32Ret;
    }

    u32ReadSize = (1 == pstSectionTail->u32Totalsection) ? pstSectionTail->u32SignedDataLength : pstSectionTail->u32SectionSize;
    while (u32Index < pstSectionTail->u32Totalsection)
    {
        u64SectionHead = (HI_U64)u32Index * u32ReadSize;
        pu8SectionTail = pu8SignBuffer + u32Index * SSA_DCAS_FIX_PAGESIZE;

        u32ReturnSize = SSA_FLASH_Read(hDataFlashHandle, u64SectionHead, pu8DataBuffer, u32DataBufferSize, u32ReadSize);
        if (pstSectionTail->u32SectionSize > u32ReturnSize)
        {
            SSA_PrintFuncErr(SSA_FLASH_Read, SSA_ERR_FLASH);

            SSA_CHECK(SSA_CIPHER_DeInit());
            return SSA_ERR_FLASH;
        }

        s32Ret = SSA_VerifySectionData(pu8DataBuffer, pu8SectionTail);
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(SSA_VerifySectionData, s32Ret);

            SSA_CHECK(SSA_CIPHER_DeInit());
            return s32Ret;
        }

        u32Index += (0 == u32Index) ? u32FistStepSize : u32StepSize;
    }

    SSA_CHECK(SSA_CIPHER_DeInit());

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 ssaImageRead
(
    HI_HANDLE           hSignFlashHandle,
    HI_HANDLE           hDataFlashHandle,
    HI_U8*              pu8DataBuffer,
    HI_U32              u32DataBufferSize,
    HI_U8*              pu8SignBuffer,
    HI_U32              u32SignBufferSize
)
{
    HI_U32 u32Index = 0;
    HI_U32 u32DataSize = 0;
    HI_U32 u32ReadSize = 0;
    HI_U8* pu8SectionTail = HI_NULL_PTR;
    HI_U8* pu8SectionData = pu8DataBuffer;
    SSA_SECTIONTAIL_S* pstSectionTail = (SSA_SECTIONTAIL_S*)pu8SignBuffer;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    s32Ret = SSA_CIPHER_Init();
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_CIPHER_Init, s32Ret);
        return s32Ret;
    }

    s32Ret = ssaGetSignData(hSignFlashHandle, pu8SignBuffer, u32SignBufferSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(ssaGetSignData, s32Ret);

        SSA_CHECK(SSA_CIPHER_DeInit());
        return s32Ret;
    }

    u32ReadSize = (1 == pstSectionTail->u32Totalsection) ? pstSectionTail->u32SignedDataLength : pstSectionTail->u32Totalsection * pstSectionTail->u32SectionSize;
    u32DataSize = SSA_FLASH_Read(hDataFlashHandle, 0ULL, pu8DataBuffer, u32DataBufferSize, u32ReadSize);
    if (u32ReadSize > u32DataSize)
    {
        SSA_PrintFuncErr(SSA_FLASH_Read, SSA_ERR_FLASH);

        SSA_CHECK(SSA_CIPHER_DeInit());
        return SSA_ERR_FLASH;
    }

    for (u32Index = 0; u32Index < pstSectionTail->u32Totalsection; u32Index ++)
    {
        pu8SectionData = pu8DataBuffer + u32Index * pstSectionTail->u32SectionSize;
        pu8SectionTail = pu8SignBuffer + u32Index * SSA_DCAS_FIX_PAGESIZE;
        s32Ret = SSA_VerifySectionData(pu8SectionData, pu8SectionTail);
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(SSA_VerifySectionData, s32Ret);

            SSA_CHECK(SSA_CIPHER_DeInit());
            return s32Ret;
        }
    }

    SSA_CHECK(SSA_CIPHER_DeInit());

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSA_IMG_Verify
(
    SSA_FLASH_OPENPARAM_S*  pstDataParam,
    SSA_FLASH_OPENPARAM_S*  pstSignParam,
    HI_U8*                  pu8DataBuffer,
    HI_U32                  u32DataBufferSize,
    HI_U8*                  pu8SignBuffer,
    HI_U32                  u32SignBufferSize,
    SSA_VERIFYMODE_E        enVerifyMode
)
{
    HI_HANDLE hDataFlashHandle = HI_INVALID_HANDLE;
    HI_HANDLE hSignFlashHandle = HI_INVALID_HANDLE;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    SSA_CHECK_PARAM(HI_NULL_PTR == pstDataParam);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu8DataBuffer);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu8SignBuffer);

    SSA_CHECK_PARAM(0 == u32DataBufferSize);
    SSA_CHECK_PARAM(0 == u32SignBufferSize);

    if ((SSA_VERIFYMODE_COMPLETE != enVerifyMode)
        && (SSA_VERIFYMODE_SAMPLING != enVerifyMode))
    {
        SSA_PrintErrCode(SSA_ERR_PARAM);
        return SSA_ERR_PARAM;
    }

    hDataFlashHandle = SSA_FLASH_Open(pstDataParam);
    if (HI_INVALID_HANDLE == hDataFlashHandle)
    {
        SSA_PrintFuncErr(SSA_FLASH_Open, SSA_ERR_FLASH);
        return SSA_ERR_FLASH;
    }

    if (HI_NULL_PTR != pstSignParam)
    {
        hSignFlashHandle = SSA_FLASH_Open(pstSignParam);
        if (HI_INVALID_HANDLE == hSignFlashHandle)
        {
            SSA_PrintFuncErr(SSA_FLASH_Open, SSA_ERR_FLASH);

            SSA_FLASH_Close(hDataFlashHandle);
            hDataFlashHandle = HI_INVALID_HANDLE;

            return SSA_ERR_FLASH;
        }
    }
    else
    {
        hSignFlashHandle = hDataFlashHandle;
    }

    s32Ret = ssaImageVerify(hDataFlashHandle,
                            hSignFlashHandle,
                            pu8DataBuffer,
                            u32DataBufferSize,
                            pu8SignBuffer,
                            u32SignBufferSize,
                            enVerifyMode);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(ssaImageVerify, s32Ret);

        if (HI_NULL_PTR != pstSignParam)
        {
            SSA_FLASH_Close(hSignFlashHandle);
            hSignFlashHandle = HI_INVALID_HANDLE;
        }

        SSA_FLASH_Close(hDataFlashHandle);
        hDataFlashHandle = HI_INVALID_HANDLE;

        return s32Ret;
    }

    if (HI_NULL_PTR != pstSignParam)
    {
        SSA_FLASH_Close(hSignFlashHandle);
        hSignFlashHandle = HI_INVALID_HANDLE;
    }

    SSA_FLASH_Close(hDataFlashHandle);
    hDataFlashHandle = HI_INVALID_HANDLE;

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSA_IMG_Read
(
    SSA_FLASH_OPENPARAM_S*  pstDataParam,
    SSA_FLASH_OPENPARAM_S*  pstSignParam,
    HI_U8*                  pu8DataBuffer,
    HI_U32                  u32DataBufferSize,
    HI_U8*                  pu8SignBuffer,
    HI_U32                  u32SignBufferSize
)
{
    HI_HANDLE hDataFlashHandle = HI_INVALID_HANDLE;
    HI_HANDLE hSignFlashHandle = HI_INVALID_HANDLE;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    SSA_CHECK_PARAM(HI_NULL_PTR == pstDataParam);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu8DataBuffer);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu8SignBuffer);

    SSA_CHECK_PARAM(0 == u32DataBufferSize);
    SSA_CHECK_PARAM(0 == u32SignBufferSize);

    hDataFlashHandle = SSA_FLASH_Open(pstDataParam);
    if (HI_INVALID_HANDLE == hDataFlashHandle)
    {
        SSA_PrintFuncErr(SSA_FLASH_Open, SSA_ERR_FLASH);
        return SSA_ERR_FLASH;
    }

    if (HI_NULL_PTR != pstSignParam)
    {
        hSignFlashHandle = SSA_FLASH_Open(pstSignParam);
        if (HI_INVALID_HANDLE == hSignFlashHandle)
        {
            SSA_PrintFuncErr(SSA_FLASH_Open, SSA_ERR_FLASH);

            SSA_FLASH_Close(hDataFlashHandle);
            hDataFlashHandle = HI_INVALID_HANDLE;

            return SSA_ERR_FLASH;
        }
    }
    else
    {
        hSignFlashHandle = hDataFlashHandle;
    }

    s32Ret = ssaImageRead(hDataFlashHandle,
                          hSignFlashHandle,
                          pu8DataBuffer,
                          u32DataBufferSize,
                          pu8SignBuffer,
                          u32SignBufferSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(ssaImageRead, s32Ret);

        if (HI_NULL_PTR != pstSignParam)
        {
            SSA_FLASH_Close(hSignFlashHandle);
            hSignFlashHandle = HI_INVALID_HANDLE;
        }

        SSA_FLASH_Close(hDataFlashHandle);
        hDataFlashHandle = HI_INVALID_HANDLE;

        return s32Ret;
    }

    if (HI_NULL_PTR != pstSignParam)
    {
        SSA_FLASH_Close(hSignFlashHandle);
        hSignFlashHandle = HI_INVALID_HANDLE;
    }

    SSA_FLASH_Close(hDataFlashHandle);
    hDataFlashHandle = HI_INVALID_HANDLE;

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_BOOL SSA_IMG_Clone
(
    SSA_FLASH_OPENPARAM_S*  pstParamDst,
    SSA_FLASH_OPENPARAM_S*  pstParamSrc,
    HI_U8*                  pu8DataBuffer,
    HI_U32                  u32DataBufferSize
)
{
    HI_U32 u32PartSize = 0;
    HI_U32 u32DataSize = 0;
    HI_HANDLE hFlashHandleDst = HI_INVALID_HANDLE;
    HI_HANDLE hFlashHandleSrc = HI_INVALID_HANDLE;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    SSA_CHECK_PARAM(HI_NULL_PTR == pstParamDst);
    SSA_CHECK_PARAM(HI_NULL_PTR == pstParamSrc);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu8DataBuffer);

    SSA_CHECK_PARAM(0 == u32DataBufferSize);

    hFlashHandleDst = SSA_FLASH_Open(pstParamDst);
    if (HI_INVALID_HANDLE == hFlashHandleDst)
    {
        SSA_PrintFuncErr(SSA_FLASH_Open, SSA_ERR_FLASH);
        return SSA_ERR_FLASH;
    }

    hFlashHandleSrc = SSA_FLASH_Open(pstParamSrc);
    if (HI_INVALID_HANDLE == hFlashHandleSrc)
    {
        SSA_PrintFuncErr(SSA_FLASH_Open, SSA_ERR_FLASH);

        SSA_FLASH_Close(hFlashHandleDst);
        hFlashHandleDst = HI_NULL_PTR;

        return SSA_ERR_FLASH;
    }

    s32Ret = SSA_FLASH_GetPartitionSize(hFlashHandleSrc, &u32PartSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_FLASH_GetPartitionSize, s32Ret);

        SSA_FLASH_Close(hFlashHandleDst);
        hFlashHandleDst = HI_NULL_PTR;

        SSA_FLASH_Close(hFlashHandleSrc);
        hFlashHandleSrc = HI_NULL_PTR;

        return s32Ret;
    }

    u32DataSize = SSA_FLASH_Read(hFlashHandleSrc, 0ULL, pu8DataBuffer, u32DataBufferSize, u32PartSize);
    if (0 == u32DataSize)
    {
        SSA_PrintFuncErr(SSA_FLASH_Read, SSA_ERR_FLASH);

        SSA_FLASH_Close(hFlashHandleDst);
        hFlashHandleDst = HI_NULL_PTR;

        SSA_FLASH_Close(hFlashHandleSrc);
        hFlashHandleSrc = HI_NULL_PTR;

        return SSA_ERR_FLASH;
    }

    s32Ret = SSA_FLASH_Write(hFlashHandleDst, 0ULL, pu8DataBuffer, u32DataSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_Erase, s32Ret);

        SSA_FLASH_Close(hFlashHandleDst);
        hFlashHandleDst = HI_NULL_PTR;

        SSA_FLASH_Close(hFlashHandleSrc);
        hFlashHandleSrc = HI_NULL_PTR;

        return SSA_ERR_FLASH;
    }

    SSA_FLASH_Close(hFlashHandleDst);
    hFlashHandleDst = HI_NULL_PTR;

    SSA_FLASH_Close(hFlashHandleSrc);
    hFlashHandleSrc = HI_NULL_PTR;

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_BOOL SSA_IMG_Compare(HI_U8* pu8SignBuffer1, HI_U8* pu8SignBuffer2)
{
    SSA_SECTIONTAIL_S* pstSectionTail1 = (SSA_SECTIONTAIL_S*)pu8SignBuffer1;
    SSA_SECTIONTAIL_S* pstSectionTail2 = (SSA_SECTIONTAIL_S*)pu8SignBuffer2;

    if ((HI_NULL_PTR == pu8SignBuffer1)
        || (HI_NULL_PTR == pu8SignBuffer2))
    {
        return HI_FALSE;
    }

    if (0 != memcmp(pstSectionTail1, pstSectionTail2, sizeof(SSA_SECTIONTAIL_S)))
    {
        return HI_FALSE;
    }

    if (0 != memcmp(pstSectionTail1, pstSectionTail2, pstSectionTail1->u32Totalsection * SSA_DCAS_FIX_PAGESIZE))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

