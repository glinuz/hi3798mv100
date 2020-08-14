/******************************************************************************

          Copyright (C), 2001-2015, Hisilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : dcas_flash.c
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
#include "dcas_flash.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SSA_BYTEALIGN(u32DataSize, u32AlignSize) (((u32DataSize) + (u32AlignSize) - 1) & (~((u32AlignSize) - 1)))

HI_HANDLE SSA_FLASH_Open(SSA_FLASH_OPENPARAM_S* pstOpenParam)
{
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    SSA_DBG_FuncEnter();

    SSA_CHECK_PARAM(HI_NULL_PTR == pstOpenParam);

    hHandle = HI_Flash_Open(pstOpenParam->enFlashType,
                            pstOpenParam->pcPartitionName,
                            pstOpenParam->u64Address,
                            pstOpenParam->u32Length);
    if (HI_INVALID_HANDLE == hHandle)
    {
        SSA_PrintFuncErr(HI_Flash_Open, SSA_ERR_FLASH);
        return HI_INVALID_HANDLE;
    }

    SSA_DBG_FuncExit();
    return hHandle;
}

HI_S32 SSA_FLASH_Close(HI_HANDLE hHandle)
{
    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    SSA_CHECK_PARAM(HI_INVALID_HANDLE == hHandle);

    s32Ret = HI_Flash_Close(hHandle);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_Close, s32Ret);
        return HI_FAILURE;
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_U32 SSA_FLASH_Read(HI_HANDLE hHandle, HI_U64 u64Address, HI_U8* pu8Buffer, HI_U32 u32BufferSize, HI_U32 u32ReadSize)
{
    HI_U32 u32DataSize = 0;
    HI_U32 u32PageSize = 0;
    HI_Flash_InterInfo_S stFlashInfo;
    HI_S32 s32Ret = HI_FAILURE;

    SSA_CHECK_PARAM(HI_INVALID_HANDLE == hHandle);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu8Buffer);
    SSA_CHECK_PARAM(0 == u32BufferSize);
    SSA_CHECK_PARAM(0 == u32ReadSize);

    memset(&stFlashInfo, 0x00, sizeof(stFlashInfo));
    s32Ret = HI_Flash_GetInfo(hHandle, &stFlashInfo);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_GetInfo, s32Ret);
        return 0;
    }

    u32PageSize = (HI_FLASH_TYPE_EMMC_0 == stFlashInfo.FlashType) ? stFlashInfo.BlockSize : stFlashInfo.PageSize;
    u32DataSize = SSA_BYTEALIGN(u32ReadSize, u32PageSize);

    if (u32BufferSize < u32DataSize)
    {
        SSA_PrintErrCode(SSA_ERR_OVERFLOW);
        return 0;
    }

    s32Ret = HI_Flash_Read(hHandle, u64Address, pu8Buffer, u32DataSize, HI_FLASH_RW_FLAG_RAW);
    if (0 >= s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_Read, s32Ret);
        return 0;
    }

    SSA_DBG_FuncExit();
    return (HI_U32)s32Ret;
}

HI_S32 SSA_FLASH_Write(HI_HANDLE hHandle, HI_U64 u64Address, HI_U8* pu8Data, HI_U32 u32Size)
{
    HI_U32 u32WriteSize = 0;
    HI_U32 u32EraseSize = 0;
    HI_Flash_InterInfo_S stFlashInfo;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    SSA_CHECK_PARAM(HI_INVALID_HANDLE == hHandle);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu8Data);
    SSA_CHECK_PARAM(0 == u32Size);

    memset(&stFlashInfo, 0x00, sizeof(stFlashInfo));

    s32Ret = HI_Flash_GetInfo(hHandle, &stFlashInfo);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_GetInfo, s32Ret);
        return SSA_ERR_FLASH;
    }

    if (HI_FLASH_TYPE_EMMC_0 != stFlashInfo.FlashType)
    {
        u32EraseSize = SSA_BYTEALIGN(u32Size, stFlashInfo.BlockSize);
        u32WriteSize = SSA_BYTEALIGN(u32Size, stFlashInfo.PageSize);

        s32Ret = HI_Flash_Erase(hHandle, u64Address, (HI_U64)u32EraseSize);
        if ((HI_S32)u32EraseSize != s32Ret)
        {
            SSA_PrintFuncErr(HI_Flash_Erase, s32Ret);
            return SSA_ERR_FLASH;
        }
    }
    else
    {
        u32WriteSize = SSA_BYTEALIGN(u32Size, stFlashInfo.BlockSize);
    }

    s32Ret = HI_Flash_Write(hHandle, u64Address, pu8Data, u32WriteSize, HI_FLASH_RW_FLAG_RAW);
    if ((HI_S32)u32WriteSize != s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_Write, s32Ret);
        return SSA_ERR_FLASH;
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSA_FLASH_GetPartitionSize(HI_HANDLE hHandle, HI_U32* pu32Size)
{
    HI_Flash_InterInfo_S stFlashInfo;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    SSA_CHECK_PARAM(HI_INVALID_HANDLE == hHandle);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu32Size);

    memset(&stFlashInfo, 0x00, sizeof(stFlashInfo));

    s32Ret = HI_Flash_GetInfo(hHandle, &stFlashInfo);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_GetInfo, s32Ret);
        return SSA_ERR_FLASH;
    }

    *pu32Size = (HI_U32)stFlashInfo.PartSize;

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

