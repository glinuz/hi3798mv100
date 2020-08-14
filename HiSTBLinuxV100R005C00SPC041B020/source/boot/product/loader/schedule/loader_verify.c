/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : loader_verify.c
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
#include "app.h"
#include "crc32.h"
#else
#include "common.h"
#include "exports.h"
#endif

#include "hi_flash.h"
#include "loader_debug.h"
#include "loader_verify.h"

#define FIP_MAX_FILES               (10)
#define FIP_TOCHEADER_NAME          (0xAA640001)
#define FIP_UUID_NODE_LEN           (6)
#define FIP_UUID_BL33               {0xa7eed0d6, 0xeafc, 0x4bd5, 0x97, 0x82, {0x99, 0x34, 0xf2, 0x34, 0xb6, 0xe4}}

#define LOADER_IMAGE_HEAD_SIZE      (64)
#define LOADER_IMAGE_POS_DATACRC    (24)
#define LOADER_IMAGE_POS_DATASIZE   (12)


#define LOADER_HtoNL(u32Value)      ((0xff000000 & ((u32Value) << 24))  \
                                     | (0x00ff0000 & ((u32Value) << 8))  \
                                     | (0x0000ff00 & ((u32Value) >> 8))  \
                                     | (0x000000ff & ((u32Value) >> 24)))

#define LOADER_GetImageDataCrc(ImageData)    LOADER_HtoNL(*(HI_U32*)(HI_VOID*)((ImageData) + LOADER_IMAGE_POS_DATACRC))
#define LOADER_GetImageDataSize(ImageData)   LOADER_HtoNL(*(HI_U32*)(HI_VOID*)((ImageData) + LOADER_IMAGE_POS_DATASIZE))

typedef struct tagFIP_UUID_S
{
    HI_U32      u32TimeLow;
    HI_U16      u16TimeMid;
    HI_U16      u16TimeHiAndVersion;
    HI_U8       u8ClockSeqHiAndReserved;
    HI_U8       u8ClockSeqLow;
    HI_U8       u8Node[FIP_UUID_NODE_LEN];

} FIP_UUID_S;

typedef struct tagFIP_TOC_HEADER_S
{
    HI_U32      u32Name;
    HI_U32      s32SerialNumber;
    HI_U64      u64Flags;

} FIP_TOC_HEADER_S;

typedef struct tagFIP_TOC_ENTRY_S
{
    FIP_UUID_S  stUuid;
    HI_U64      u64OffsetAddress;
    HI_U64      u64Size;
    HI_U64      u64Flags;

} FIP_TOC_ENTRY_S;

static HI_BOOL loaderIsFip(HI_U8* pu8Buffer)
{
    FIP_TOC_HEADER_S* pstHeader = (FIP_TOC_HEADER_S*)(HI_VOID*)pu8Buffer;

    if ((FIP_TOCHEADER_NAME == pstHeader->u32Name)
        && (0 != pstHeader->s32SerialNumber))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

static HI_S32 loaderVerifyImage(HI_U8* pu8ImageData, HI_U32 u32ImageSize)
{
    HI_U32  u32ImageDataSize = 0;
    HI_U32  u32ImageDataCrc1 = 0;
    HI_U32  u32ImageDataCrc2 = 0;

    LOADER_DBG_FuncEnter();

    if (LOADER_IMAGE_HEAD_SIZE >= u32ImageSize)
    {
        LOADER_PrintErrCode(LOADER_ERR_ILLEGAL_IMAGE);
        return LOADER_ERR_ILLEGAL_IMAGE;
    }

    u32ImageDataSize = LOADER_GetImageDataSize(pu8ImageData);
    if (u32ImageDataSize > u32ImageSize - LOADER_IMAGE_HEAD_SIZE)
    {
        LOADER_PrintErrCode(LOADER_ERR_OVERFLOW);
        return LOADER_ERR_OVERFLOW;
    }

    u32ImageDataCrc1 = LOADER_GetImageDataCrc(pu8ImageData);
    u32ImageDataCrc2 = crc32(0, pu8ImageData + LOADER_IMAGE_HEAD_SIZE, u32ImageDataSize);
    if (u32ImageDataCrc1 != u32ImageDataCrc2)
    {
        LOADER_PrintErrCode(LOADER_ERR_ILLEGAL_IMAGE);
        return LOADER_ERR_ILLEGAL_IMAGE;
    }

    return HI_SUCCESS;
}

static HI_S32 loaderGetImageData
(
    HI_U8*  pu8PartitionData,
    HI_U32  u32PartitionSize,
    HI_U32* pu32ImageOffset,
    HI_U32* pu32ImageSize
)
{
    HI_U32 u32Times = 0;
    FIP_TOC_HEADER_S* pstTocHeader = HI_NULL_PTR;
    FIP_TOC_ENTRY_S* pstTocEntry = HI_NULL_PTR;

    FIP_UUID_S stLoaderUUID = FIP_UUID_BL33;
    FIP_UUID_S stNullUUID = {0};

    pstTocHeader = (FIP_TOC_HEADER_S*)pu8PartitionData;
    pstTocEntry  = (FIP_TOC_ENTRY_S*)(pstTocHeader + 1);

    LOADER_DBG_FuncEnter();

    while ((FIP_MAX_FILES > u32Times)
           && (HI_NULL_PTR != pstTocEntry))
    {
        if (0 == memcmp(&pstTocEntry->stUuid, &stNullUUID, sizeof(FIP_UUID_S)))
        {
            LOADER_PrintErrCode(LOADER_ERR_ILLEGAL_IMAGE);
            return LOADER_ERR_ILLEGAL_IMAGE;
        }

        if (0 == memcmp(&pstTocEntry->stUuid, &stLoaderUUID, sizeof(FIP_UUID_S)))
        {
            if ((pstTocEntry->u64OffsetAddress >= u32PartitionSize)
                || (pstTocEntry->u64Size >= u32PartitionSize))
            {
                LOADER_PrintErrCode(LOADER_ERR_ILLEGAL_IMAGE);
                return LOADER_ERR_ILLEGAL_IMAGE;
            }

            *pu32ImageOffset = (HI_U32)pstTocEntry->u64OffsetAddress;
            *pu32ImageSize   = (HI_U32)pstTocEntry->u64Size;

            LOADER_DBG_FuncExit();
            return HI_SUCCESS;
        }

        pstTocEntry ++;
        u32Times ++;
    }

    LOADER_PrintErrCode(LOADER_ERR_ILLEGAL_IMAGE);
    return LOADER_ERR_ILLEGAL_IMAGE;
}

static HI_FLASH_TYPE_E loaderGetFlashType(HI_VOID)
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

HI_HANDLE loaderPartitionOpen(HI_CHAR* pcPartName, HI_U32 u32PartAddress, HI_U32 u32PartSize)
{
    HI_HANDLE hFlashHandle = HI_INVALID_HANDLE;
    HI_FLASH_TYPE_E enFlashType = HI_FLASH_TYPE_BUTT;

    LOADER_DBG_FuncEnter();

    if (HI_NULL_PTR != pcPartName)
    {
        hFlashHandle = HI_Flash_OpenByName(pcPartName);
        if (HI_INVALID_HANDLE == hFlashHandle)
        {
            LOADER_PrintFuncErr(HI_Flash_OpenByName, LOADER_ERR_FLASH);
            return HI_INVALID_HANDLE;
        }
    }
    else
    {
        enFlashType = loaderGetFlashType();
        if (HI_FLASH_TYPE_BUTT == enFlashType)
        {
            LOADER_PrintFuncErr(loaderGetFlashType, LOADER_ERR_FLASH);
            return HI_INVALID_HANDLE;
        }

        hFlashHandle = HI_Flash_OpenByTypeAndAddr(enFlashType, (HI_U64)u32PartAddress, (HI_U64)u32PartSize);
        if (HI_INVALID_HANDLE == hFlashHandle)
        {
            LOADER_PrintFuncErr(HI_Flash_OpenByTypeAndAddr, LOADER_ERR_FLASH);
            return HI_INVALID_HANDLE;
        }
    }

    LOADER_DBG_FuncExit();
    return hFlashHandle;
}

HI_S32 loaderPartitionClose(HI_HANDLE hFlashHandle)
{
    HI_S32 s32Ret = HI_FAILURE;

    LOADER_DBG_FuncEnter();

    s32Ret = HI_Flash_Close(hFlashHandle);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(HI_Flash_Close, s32Ret);
        return HI_FAILURE;
    }

    LOADER_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 loaderPartitionRead
(
    HI_HANDLE   hFlashHandle,
    HI_U8*      pu8Buffer,
    HI_U32      u32BufferSize,
    HI_U32*     pu32DataOffset,
    HI_U32*     pu32DataSize
)
{
    HI_S32 s32ReadSize = 0;
    HI_U32 u32DataSize = 0;
    HI_Flash_InterInfo_S stFlashInfo;

    HI_S32 s32Ret = HI_SUCCESS;

    LOADER_DBG_FuncEnter();

    memset(&stFlashInfo, 0x00, sizeof(stFlashInfo));
    s32Ret = HI_Flash_GetInfo(hFlashHandle, &stFlashInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(HI_Flash_GetInfo, s32Ret);
        return LOADER_ERR_FLASH;
    }

    if (u32BufferSize < stFlashInfo.PartSize)
    {
        LOADER_PrintErrCode(LOADER_ERR_OVERFLOW);
        return LOADER_ERR_OVERFLOW;
    }

    s32ReadSize = HI_Flash_Read(hFlashHandle, 0ULL, pu8Buffer, (HI_U32)stFlashInfo.PartSize, HI_FLASH_RW_FLAG_RAW);
    if (0 >= s32ReadSize)
    {
        LOADER_PrintFuncErr(HI_Flash_Read, s32Ret);
        return LOADER_ERR_FLASH;
    }

    if (HI_TRUE == loaderIsFip(pu8Buffer))
    {
        s32Ret = loaderGetImageData(pu8Buffer, s32ReadSize, pu32DataOffset, pu32DataSize);
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(loaderGetImageData, s32Ret);
            return s32Ret;
        }
    }
    else
    {
        if (LOADER_IMAGE_HEAD_SIZE >= s32ReadSize)
        {
            LOADER_PrintErrCode(LOADER_ERR_FLASH);
            return LOADER_ERR_FLASH;
        }

        u32DataSize = LOADER_GetImageDataSize(pu8Buffer);
        if (u32DataSize > s32ReadSize - LOADER_IMAGE_HEAD_SIZE)
        {
            LOADER_PrintErrCode(LOADER_ERR_OVERFLOW);
            return LOADER_ERR_OVERFLOW;
        }

        *pu32DataOffset = 0;
        *pu32DataSize   = u32DataSize + LOADER_IMAGE_HEAD_SIZE;
    }

    LOADER_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 loaderPartitionWrite(HI_HANDLE hFlashHandle, HI_U8* pu8Data, HI_U32 u32Size)
{
    HI_U32 u32WriteSize = 0;
    HI_U32 u32EraseSize = 0;
    HI_Flash_InterInfo_S stFlashInfo;

    HI_S32 s32Ret = HI_FAILURE;

    LOADER_DBG_FuncEnter();

    memset(&stFlashInfo, 0x00, sizeof(stFlashInfo));
    s32Ret = HI_Flash_GetInfo(hFlashHandle, &stFlashInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(HI_Flash_GetInfo, s32Ret);
        return LOADER_ERR_FLASH;
    }

    if (HI_FLASH_TYPE_EMMC_0 != stFlashInfo.FlashType)
    {
        u32EraseSize = (u32Size + stFlashInfo.BlockSize - 1) & (~(stFlashInfo.BlockSize - 1));
        u32WriteSize = (u32Size + stFlashInfo.PageSize - 1) & (~(stFlashInfo.PageSize - 1));
        s32Ret = HI_Flash_Erase(hFlashHandle, 0ULL, (HI_U64)u32EraseSize);
        if ((HI_S32)u32EraseSize != s32Ret)
        {
            LOADER_PrintFuncErr(HI_Flash_Erase, s32Ret);
            return LOADER_ERR_FLASH;
        }
    }
    else
    {
        u32WriteSize = (u32Size + stFlashInfo.BlockSize - 1) & (~(stFlashInfo.BlockSize - 1));
    }

    s32Ret = HI_Flash_Write(hFlashHandle, 0ULL, pu8Data, u32WriteSize, HI_FLASH_RW_FLAG_RAW);
    if ((HI_S32)u32WriteSize != s32Ret)
    {
        LOADER_PrintFuncErr(HI_Flash_Write, s32Ret);
        return LOADER_ERR_FLASH;
    }

    LOADER_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LOADER_GetImageData
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
    HI_HANDLE hFlashHandle = HI_INVALID_HANDLE;

    HI_S32 s32Ret = HI_FAILURE;

    LOADER_DBG_FuncEnter();

    hFlashHandle = loaderPartitionOpen(pcPartName, u32Address, u32Length);
    if (HI_INVALID_HANDLE == hFlashHandle)
    {
        LOADER_PrintFuncErr(loaderPartitionOpen, s32Ret);
        return HI_FAILURE;
    }

    s32Ret = loaderPartitionRead(hFlashHandle, pu8Buffer, u32BufferSize, &u32DataOffset, &u32DataSize);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(loaderPartitionRead, s32Ret);
        loaderPartitionClose(hFlashHandle);
        return s32Ret;
    }

    s32Ret = loaderVerifyImage(pu8Buffer + u32DataOffset, u32DataSize);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(loaderVerifyImage, s32Ret);
        loaderPartitionClose(hFlashHandle);
        return s32Ret;
    }

    loaderPartitionClose(hFlashHandle);

    *pu32DataSize = u32DataSize;
    *pu32DataOffset = u32DataOffset;

    LOADER_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_BOOL LOADER_CpyImageData
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

    LOADER_DBG_FuncEnter();

    hFlashHandleDst = loaderPartitionOpen(pcPartNameDst, 0, 0);
    if (HI_INVALID_HANDLE == hFlashHandleDst)
    {
        LOADER_PrintFuncErr(loaderPartitionOpen, s32Ret);
        return LOADER_ERR_FLASH;
    }

    hFlashHandleSrc = loaderPartitionOpen(pcPartNameSrc, 0, 0);
    if (HI_INVALID_HANDLE == hFlashHandleSrc)
    {
        LOADER_PrintFuncErr(loaderPartitionOpen, s32Ret);
        loaderPartitionClose(hFlashHandleDst);
        return LOADER_ERR_FLASH;
    }

    s32Ret = loaderPartitionRead(hFlashHandleSrc, pu8Buffer, u32BufferSize, &u32DataSize, &u32DataOffset);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(loaderPartitionRead, s32Ret);
        loaderPartitionClose(hFlashHandleDst);
        loaderPartitionClose(hFlashHandleSrc);
        return s32Ret;
    }

    s32Ret = loaderPartitionWrite(hFlashHandleDst, pu8Buffer, u32DataSize + u32DataOffset);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(loaderPartitionWrite, s32Ret);
        loaderPartitionClose(hFlashHandleDst);
        loaderPartitionClose(hFlashHandleSrc);
        return s32Ret;
    }

    loaderPartitionClose(hFlashHandleDst);
    loaderPartitionClose(hFlashHandleSrc);

    LOADER_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_BOOL LOADER_CmpImageData
(
    HI_U8*      pu8ImageData1,
    HI_U8*      pu8ImageData2,
    HI_U32      u32DataOffset,
    HI_U32      u32DataSize
)
{
    HI_U32 u32ImageDataCrc1 = LOADER_GetImageDataCrc(pu8ImageData1 + u32DataOffset);
    HI_U32 u32ImageDataCrc2 = LOADER_GetImageDataCrc(pu8ImageData2 + u32DataOffset);

    if (u32ImageDataCrc1 == u32ImageDataCrc2)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}


