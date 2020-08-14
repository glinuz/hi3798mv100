/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : vmx_image.c
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

#ifdef HI_MINIBOOT_SUPPORT
#include "boot.h"
#include "app.h"
#include "cpuinfo.h"
#else
#include "common.h"
#include "exports.h"
#endif

#include "hi_type.h"
#include "hi_flash.h"
#include "vmx_debug.h"
#include "vmx_config.h"
#include "vmx_image.h"
#include "vmx_verify_api.h"

#define VMX_ImageDataVerify(pu8ImageData, u32ImageSize, pucErrorCode) \
    verifySignature((pu8ImageData) + SSA_VMX_SIG_OFFSET, \
                    (pu8ImageData) + SSA_VMX_IMG_OFFSET, \
                    SSA_VERIFY_ADDR_RESERVE,  \
                    (u32ImageSize),  \
                    (u32ImageSize),  \
                    SSA_VMX_VERIFY_MODE,  \
                    (pucErrorCode))

static HI_VOID vmxReboot(HI_VOID)
{
#ifdef HI_MINIBOOT_SUPPORT
    reboot();
#else
    do_reset (NULL, 0, 0, NULL);
    do_reset (NULL, 0, 0, NULL);
    do_reset (NULL, 0, 0, NULL);

    while (1)
    {

    }
#endif
}

static HI_FLASH_TYPE_E vmxGetFlashType(HI_VOID)
{
    HI_S32 enBootMediaType = BOOT_MEDIA_UNKNOWN;

    enBootMediaType = get_bootmedia(HI_NULL_PTR, HI_NULL_PTR);

    switch (enBootMediaType)
    {
        case BOOT_MEDIA_NAND:
        {
            return HI_FLASH_TYPE_NAND_0;
        }
        case BOOT_MEDIA_SPIFLASH:
        {
            return HI_FLASH_TYPE_SPI_0;
        }
        case BOOT_MEDIA_EMMC:
        {
            return HI_FLASH_TYPE_EMMC_0;
        }
        case BOOT_MEDIA_SD:
        {
            return HI_FLASH_TYPE_SD;
        }
        default:
        {
            return HI_FLASH_TYPE_BUTT;
        }
    }
}

HI_HANDLE vmxPartitionOpen(HI_CHAR* pcPartName, HI_U32 u32PartAddress, HI_U32 u32PartSize)
{
    HI_HANDLE hFlashHandle = HI_INVALID_HANDLE;
    HI_FLASH_TYPE_E enFlashType = HI_FLASH_TYPE_BUTT;

    SSA_DBG_FuncEnter();

    if (HI_NULL_PTR != pcPartName)
    {
        hFlashHandle = HI_Flash_OpenByName(pcPartName);
        if (HI_INVALID_HANDLE == hFlashHandle)
        {
            SSA_PrintFuncErr(HI_Flash_OpenByName, SSA_ERR_FLASH);
            return HI_INVALID_HANDLE;
        }
    }
    else
    {
        enFlashType = vmxGetFlashType();
        if (HI_FLASH_TYPE_BUTT == enFlashType)
        {
            SSA_PrintFuncErr(vmxGetFlashType, SSA_ERR_FLASH);
            return HI_INVALID_HANDLE;
        }

        hFlashHandle = HI_Flash_OpenByTypeAndAddr(enFlashType, (HI_U64)u32PartAddress, (HI_U64)u32PartSize);
        if (HI_INVALID_HANDLE == hFlashHandle)
        {
            SSA_PrintFuncErr(HI_Flash_OpenByTypeAndAddr, SSA_ERR_FLASH);
            return HI_INVALID_HANDLE;
        }
    }

    SSA_DBG_FuncExit();
    return hFlashHandle;
}

HI_S32 vmxPartitionClose(HI_HANDLE hFlashHandle)
{
    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    s32Ret = HI_Flash_Close(hFlashHandle);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_Close, s32Ret);
        return HI_FAILURE;
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 vmxPartitionRead
(
    HI_HANDLE   hFlashHandle,
    HI_U8*      pu8Buffer,
    HI_U32      u32BufferSize,
    HI_U32*     pu32DataOffset,
    HI_U32*     pu32DataSize
)
{
    HI_U32 u32ReadSize = 0;
    HI_U32 u32DataSize = 0;
    HI_U32 u32leftSize = 0;
    HI_Flash_InterInfo_S stFlashInfo;
    HI_S32 s32Ret = HI_FAILURE;

    memset(&stFlashInfo, 0x00, sizeof(stFlashInfo));
    s32Ret = HI_Flash_GetInfo(hFlashHandle, &stFlashInfo);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_GetInfo, s32Ret);
        return SSA_ERR_FLASH;
    }

    u32ReadSize = (HI_FLASH_TYPE_EMMC_0 == stFlashInfo.FlashType) ? stFlashInfo.BlockSize : stFlashInfo.PageSize;
    if (u32BufferSize < u32ReadSize)
    {
        SSA_PrintErrCode(SSA_ERR_OVERFLOW);
        return SSA_ERR_OVERFLOW;
    }

    s32Ret = HI_Flash_Read(hFlashHandle, 0ULL, pu8Buffer, u32ReadSize, HI_FLASH_RW_FLAG_RAW);
    if ((HI_S32)u32ReadSize != s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_Read, s32Ret);
        return SSA_ERR_FLASH;
    }

    u32DataSize = *(HI_U32*)pu8Buffer;
    if (0 != (u32DataSize & SSA_VMX_ALIGN_BYTE))
    {
        SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
        return SSA_ERR_ILLEGAL_IMAGE;
    }

    if ((u32DataSize > (HI_U32)(stFlashInfo.PartSize - SSA_VMX_IMG_OFFSET))
        || (u32DataSize > (u32BufferSize - SSA_VMX_IMG_OFFSET)))
    {
        SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
        return SSA_ERR_ILLEGAL_IMAGE;
    }

    if (u32ReadSize < (u32DataSize + SSA_VMX_IMG_OFFSET))
    {
        u32leftSize = (u32DataSize + SSA_VMX_IMG_OFFSET) - u32ReadSize;
        u32leftSize = (u32leftSize + u32ReadSize - 1) & (~(u32ReadSize - 1));
        s32Ret = HI_Flash_Read(hFlashHandle, (HI_U64)u32ReadSize, pu8Buffer + u32ReadSize, u32leftSize, HI_FLASH_RW_FLAG_RAW);
        if ((HI_S32)u32leftSize != s32Ret)
        {
            SSA_PrintFuncErr(HI_Flash_Read, s32Ret);
            return SSA_ERR_FLASH;
        }
    }

    *pu32DataOffset = SSA_VMX_IMG_OFFSET;
    *pu32DataSize   = u32DataSize;

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 vmxPartitionWrite(HI_HANDLE hFlashHandle, HI_U8* pu8Data, HI_U32 u32Size)
{
    HI_U32 u32WriteSize = 0;
    HI_U32 u32EraseSize = 0;
    HI_Flash_InterInfo_S stFlashInfo;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    memset(&stFlashInfo, 0x00, sizeof(stFlashInfo));
    s32Ret = HI_Flash_GetInfo(hFlashHandle, &stFlashInfo);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_GetInfo, s32Ret);
        return SSA_ERR_FLASH;
    }

    if (HI_FLASH_TYPE_EMMC_0 != stFlashInfo.FlashType)
    {
        u32EraseSize = (u32Size + stFlashInfo.BlockSize - 1) & (~(stFlashInfo.BlockSize - 1));
        u32WriteSize = (u32Size + stFlashInfo.PageSize - 1) & (~(stFlashInfo.PageSize - 1));
        s32Ret = HI_Flash_Erase(hFlashHandle, 0ULL, (HI_U64)u32EraseSize);
        if ((HI_S32)u32EraseSize != s32Ret)
        {
            SSA_PrintFuncErr(HI_Flash_Erase, s32Ret);
            return SSA_ERR_FLASH;
        }
    }
    else
    {
        u32WriteSize = (u32Size + stFlashInfo.BlockSize - 1) & (~(stFlashInfo.BlockSize - 1));
    }

    s32Ret = HI_Flash_Write(hFlashHandle, 0ULL, pu8Data, u32WriteSize, HI_FLASH_RW_FLAG_RAW);
    if ((HI_S32)u32WriteSize != s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_Write, s32Ret);
        return SSA_ERR_FLASH;
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 VMX_GetImageData
(
    HI_CHAR*    pcPartName,
    HI_U32      u32Address,
    HI_U32      u32Length,
    HI_U8*      pu8Buffer,
    HI_U32      u32BufferSize,
    HI_U32*     pu32DataOffset,
    HI_U32*     pu32DataSize
)
{
    HI_U32 u32DataSize = 0;
    HI_U32 u32DataOffset = 0;
    HI_UCHAR ucErrorCode = 0;
    HI_HANDLE hFlashHandle = HI_INVALID_HANDLE;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    hFlashHandle = vmxPartitionOpen(pcPartName, u32Address, u32Length);
    if (HI_INVALID_HANDLE == hFlashHandle)
    {
        SSA_PrintFuncErr(vmxPartitionOpen, s32Ret);
        return HI_FAILURE;
    }

    s32Ret = vmxPartitionRead(hFlashHandle, pu8Buffer, u32BufferSize, &u32DataOffset, &u32DataSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(vmxPartitionRead, s32Ret);
        vmxPartitionClose(hFlashHandle);
        return s32Ret;
    }

    s32Ret = VMX_ImageDataVerify(pu8Buffer, u32DataSize, &ucErrorCode);
    if (1 != s32Ret)
    {
        if ((0 == s32Ret)
            && (2 == ucErrorCode))
        {
            s32Ret = vmxPartitionWrite(hFlashHandle, pu8Buffer, (u32DataOffset + u32DataSize));
            if (HI_SUCCESS != s32Ret)
            {
                SSA_PrintFuncErr(vmxPartitionWrite, s32Ret);
                vmxPartitionClose(hFlashHandle);
                return s32Ret;
            }

            vmxReboot();

            vmxPartitionClose(hFlashHandle);

            SSA_DBG_FuncExit();
            return HI_SUCCESS;
        }

        SSA_PrintFuncErr(VMX_ImageDataVerify, s32Ret);
        vmxPartitionClose(hFlashHandle);
        return s32Ret;
    }

    vmxPartitionClose(hFlashHandle);

    *pu32DataSize = u32DataSize;
    *pu32DataOffset = u32DataOffset;

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_BOOL VMX_CpyImageData
(
    HI_CHAR*    pcPartNameDst,
    HI_CHAR*    pcPartNameSrc,
    HI_U8*      pu8Buffer,
    HI_U32      u32BufferSize
)
{
    HI_HANDLE hFlashHandleDst = HI_INVALID_HANDLE;
    HI_HANDLE hFlashHandleSrc = HI_INVALID_HANDLE;
    HI_U32 u32DataSize = 0;
    HI_U32 u32DataOffset = 0;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    hFlashHandleDst = vmxPartitionOpen(pcPartNameDst, 0, 0);
    if (HI_INVALID_HANDLE == hFlashHandleDst)
    {
        SSA_PrintFuncErr(vmxPartitionOpen, s32Ret);
        return SSA_ERR_FLASH;
    }

    hFlashHandleSrc = vmxPartitionOpen(pcPartNameSrc, 0, 0);
    if (HI_INVALID_HANDLE == hFlashHandleSrc)
    {
        SSA_PrintFuncErr(vmxPartitionOpen, s32Ret);
        vmxPartitionClose(hFlashHandleDst);
        return SSA_ERR_FLASH;
    }

    s32Ret = vmxPartitionRead(hFlashHandleSrc, pu8Buffer, u32BufferSize, &u32DataSize, &u32DataOffset);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(vmxPartitionRead, s32Ret);
        vmxPartitionClose(hFlashHandleDst);
        vmxPartitionClose(hFlashHandleSrc);
        return s32Ret;
    }

    s32Ret = vmxPartitionWrite(hFlashHandleDst, pu8Buffer, u32DataSize + u32DataOffset);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(vmxPartitionWrite, s32Ret);
        vmxPartitionClose(hFlashHandleDst);
        vmxPartitionClose(hFlashHandleSrc);
        return s32Ret;
    }

    vmxPartitionClose(hFlashHandleDst);
    vmxPartitionClose(hFlashHandleSrc);

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_BOOL VMX_CmpImageData
(
    HI_U8*      pu8ImageData1,
    HI_U8*      pu8ImageData2,
    HI_U32      u32DataOffset,
    HI_U32      u32DataSize
)
{
    if (0 == memcmp(pu8ImageData1, pu8ImageData2, u32DataOffset))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}


