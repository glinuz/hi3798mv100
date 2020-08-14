/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : loader_dbinfo.c
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

#include "hi_flash.h"

#ifdef HI_LOADER_BOOTLOADER
#include "hi_drv_pdm.h"
#ifndef HI_MINIBOOT_SUPPORT
#include "common.h"
#else
#include "string.h"
extern char* getenv(const char* key);
#endif
#else
#include "hi_unf_pdm.h"
#include "stdio.h"
#endif
#include "loader_dbharden.h"
#include "loader_crc32.h"
#include "loader_debug.h"
#include "loader_osal.h"
#include "loader_dbinfo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LDDB_D_LEN_COMMAND              (1024)                  /**< Command line size according to arch/arm/include/asm/setup.h COMMAND_LINE_SIZE */
#define LDDB_D_LEN_VERIFYVALUE          (256)                   /**< Max length of Mac verify value */
#define LDDB_D_LEN_DEVICEITEM           (128)                   /**< Max length of device information items name */
#define LDDB_D_BOOTARGS_FILENAME        "/proc/cmdline"         /**< Path of cmdline file  */
#define LDDB_D_FLASHMAP_LOADERDB        "loaderdb"
#define LDDB_D_FLASHMAP_LOADERDBBAK     "loaderdbbak"
#define LDDB_D_FLASHMAP_SOFTWAREINFO    "softwareinfo"

#ifdef HI_ADVCA_SUPPORT
#define LDDB_D_HARDEN_TYPE              LDDB_HARDEN_TYPE_CIPHER
#else
#define LDDB_D_HARDEN_TYPE              LDDB_HARDEN_TYPE_COMMON
#endif

#define LDDB_READ_32BIT(pcByte, Result, Length)             \
{                                                           \
    (Result) = (HI_U32)((pcByte)[0] << 24)                  \
             | (HI_U32)((pcByte)[1] << 16)                  \
             | (HI_U32)((pcByte)[2] << 8)                   \
             | (HI_U32)((pcByte)[3]);                       \
    (pcByte) += 4;                                          \
    (Length) += 4;                                          \
}

#define LDDB_READ_BYTES(pcByte, Buffer, ByteCount, Length)  \
{                                                           \
    LDDB_MemCpy((Buffer), (pcByte), ByteCount);             \
    (pcByte) += (ByteCount);                                \
    (Length) += (ByteCount);                                \
}

#define LDDB_WRITE_32BIT(pcByte, u32Value, Length)          \
{                                                           \
    (pcByte)[0] = (HI_U8)(0x000000FF & (u32Value >> 24));   \
    (pcByte)[1] = (HI_U8)(0x000000FF & (u32Value >> 16));   \
    (pcByte)[2] = (HI_U8)(0x000000FF & (u32Value >> 8));    \
    (pcByte)[3] = (HI_U8)(0x000000FF & (u32Value >> 0));    \
    (pcByte) += 4;                                          \
    (Length) += 4;                                          \
}

#define LDDB_WRITE_BYTES(pcByte, Buffer, ByteCount, Length) \
{                                                           \
    LDDB_MemCpy((pcByte), (Buffer), ByteCount);             \
    (pcByte) += (ByteCount);                                \
    (Length) += (ByteCount);                                \
}

typedef struct tagLDDB_DATABLOCK_INFO_S
{
    HI_CHAR acPartitionName[LDDB_D_LEN_PARTNAME];       /**< The name of the partition data */
    HI_U32  u32Offset;                                  /**< The offset size in the partition */
    HI_U32  u32Size;                                    /**< The data size allocated in the partition */

} LDDB_DATABLOCK_INFO_S;

typedef struct tagLDDB_PARTITON_HEADER_S
{
    HI_U8  au8VerifyValue[LDDB_D_LEN_VERIFYVALUE];      /**< u32Crc, Hmac value   */
    HI_U32 u32HardenedDataSize;                         /**< Hardened data size */
    HI_U32 u32DataSize;                                 /**< Data size */

} LDDB_PARTITON_HEADER_S;

typedef struct tagLDDB_FLASH_SESSION_S
{
    HI_HANDLE   hFlashHandle;
    HI_U32      u32PartitionSize;
    HI_U64      u64PartitionOffset;

} LDDB_FLASH_SESSION_S;


#ifdef HI_LOADER_BOOTLOADER

#define lddbGetDeviceInfo(pstrCfgName, pstrCfgValue, u32Size) HI_DRV_PDM_GetDeviceInfo(pstrCfgName, pstrCfgValue, u32Size)

static HI_S32 lddbGetBootargs(HI_CHAR* pcBuffer, HI_U32 u32BufferSize)
{
    HI_CHAR* pcBootargs = HI_NULL_PTR;

    LDDB_DBG_FuncEnter();

    pcBootargs = (HI_CHAR*)getenv("bootargs");
    if (HI_NULL_PTR == pcBootargs)
    {
        LDDB_PrintFuncErr(getenv, LDDB_ERR_DEVICE);
        return LDDB_ERR_DEVICE;
    }

    LDDB_MemCpy(pcBuffer, pcBootargs, u32BufferSize);

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}
#else

#define lddbGetDeviceInfo(pstrCfgName, pstrCfgValue, u32Size) HI_UNF_PDM_GetDeviceInfo(pstrCfgName, pstrCfgValue, u32Size)

static HI_S32 lddbGetBootargs(HI_CHAR* pcBuffer, HI_U32 u32BufferSize)
{
    FILE* pFile = HI_NULL_PTR;
    HI_CHAR* pcRet = HI_NULL_PTR;

    LDDB_DBG_FuncEnter();

    pFile = fopen(LDDB_D_BOOTARGS_FILENAME, "r");
    if (HI_NULL_PTR == pFile)
    {
        LDDB_PrintFuncErr(fopen, LDDB_ERR_FILE);
        return LDDB_ERR_FILE;
    }

    pcRet = fgets(pcBuffer, (HI_S32)u32BufferSize, pFile);
    if (HI_NULL_PTR == pcRet)
    {
        LDDB_PrintFuncErr(fgets, LDDB_ERR_FILE);
        fclose(pFile);
        return LDDB_ERR_FILE;
    }

    fclose(pFile);

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}
#endif

/*
 * get loader config parameter from bootargs in secondary style: loaderdb=part_name,offset,size.
 */
static HI_S32 lddbGetBlockInfo(HI_CHAR* pcDataName, LDDB_DATABLOCK_INFO_S* pstDataInfo)
{
    HI_CHAR* pstr  = HI_NULL_PTR;
    HI_CHAR* pstr2 = HI_NULL_PTR;
    HI_CHAR  acBootArgs[LDDB_D_LEN_COMMAND] = {0};
    HI_S32   s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    s32Ret = lddbGetBootargs(acBootArgs, LDDB_D_LEN_COMMAND);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(lddbGetBootargs, s32Ret);
        return s32Ret;
    }

    pstr = LDDB_StrStr(acBootArgs, pcDataName);
    if (HI_NULL_PTR == pstr)
    {
        LDDB_PrintFuncErr(LDDB_StrStr, LDDB_ERR_DEVICE);
        return LDDB_ERR_DEVICE;
    }

    pstr += LDDB_StrLen(pcDataName);
    if ('=' != pstr[0])
    {
        LDDB_PrintFuncErr(LDDB_StrStr, LDDB_ERR_DEVICE);
        return LDDB_ERR_DEVICE;
    }

    pstr += 1;
    pstr2 = LDDB_StrStr(pstr, ",");
    if (HI_NULL_PTR == pstr2)
    {
        LDDB_PrintFuncErr(LDDB_StrStr, LDDB_ERR_DEVICE);
        return LDDB_ERR_DEVICE;
    }

    if (LDDB_D_LEN_PARTNAME < (pstr2 - pstr))
    {
        LDDB_PrintFuncErr(LDDB_StrStr, LDDB_ERR_OVERFLOW);
        return LDDB_ERR_OVERFLOW;
    }

    LDDB_MemCpy(pstDataInfo->acPartitionName, pstr, (HI_U32)(pstr2 - pstr));

    pstr  = pstr2 + 1;
    pstDataInfo->u32Offset = (HI_U32)LDDB_Strtoul(pstr, HI_NULL_PTR, 16);

    pstr = LDDB_StrStr(pstr, ",");
    if (HI_NULL_PTR == pstr)
    {
        LDDB_PrintFuncErr(LDDB_StrStr, LDDB_ERR_DEVICE);
        return LDDB_ERR_DEVICE;
    }

    pstr = pstr + 1;
    pstDataInfo->u32Size = (HI_U32)LDDB_Strtoul(pstr, HI_NULL_PTR, 16);

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 lddbGetLoaderDBInfo(HI_CHAR* pcPartitionName, LDDB_LOADER_INFO_S* pstLoaderInfo)
{
    HI_U32 u32RetSize = 0;
    HI_U32 u32PartitionSize = 0;
    LDDB_HANDLE hPartitionHandle = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    s32Ret = LDDB_PartitionOpen(pcPartitionName, &hPartitionHandle, &u32PartitionSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_PartitionOpen, s32Ret);
        return s32Ret;
    }

    if (u32PartitionSize < sizeof(LDDB_LOADER_INFO_S))
    {
        LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));
        LDDB_PrintErrCode(LDDB_ERR_OVERFLOW);
        return LDDB_ERR_OVERFLOW;
    }

    s32Ret = LDDB_PartitionDataRead(hPartitionHandle, (HI_U8*)pstLoaderInfo, sizeof(LDDB_LOADER_INFO_S), &u32RetSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_PartitionDataRead, s32Ret);
        LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));
        return s32Ret;
    }

    if (sizeof(LDDB_LOADER_INFO_S) != u32RetSize)
    {
        LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));
        LDDB_PrintErrCode(LDDB_ERR_DATABASE);
        return LDDB_ERR_DATABASE;
    }

    LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 lddbSetLoaderDBInfo(HI_CHAR* pcPartitionName, LDDB_LOADER_INFO_S* pstLoaderInfo)
{
    HI_U32 u32PartitionSize = 0;
    LDDB_HANDLE hPartitionHandle = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    s32Ret = LDDB_PartitionOpen(pcPartitionName, &hPartitionHandle, &u32PartitionSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_PartitionOpen, s32Ret);
        return s32Ret;
    }

    if (u32PartitionSize < sizeof(LDDB_LOADER_INFO_S))
    {
        LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));
        LDDB_PrintErrCode(LDDB_ERR_OVERFLOW);
        return LDDB_ERR_OVERFLOW;
    }

    s32Ret = LDDB_PartitionDataWrite(hPartitionHandle, (HI_U8*)pstLoaderInfo, sizeof(LDDB_LOADER_INFO_S));
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_PartitionDataWrite, s32Ret);
        LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));
        return s32Ret;
    }

    LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_FlashOpen(HI_CHAR* pcPartitionName, LDDB_HANDLE* phHandle, HI_U32* pu32Size)
{
    HI_U32 u32Size = 0;
    HI_U32 u32Offset = 0;
    HI_Flash_InterInfo_S stFlashInfo;
    LDDB_FLASH_SESSION_S* pstFlashSession = HI_NULL_PTR;
    HI_HANDLE hFlashHandle = HI_INVALID_HANDLE;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pcPartitionName);
    LDDB_CHECK_PARAM(HI_NULL_PTR == phHandle);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu32Size);

    hFlashHandle = HI_Flash_OpenByName(pcPartitionName);
    if (HI_INVALID_HANDLE == hFlashHandle)
    {
        LDDB_DATABLOCK_INFO_S stBlockInfo;

        LDDB_PrintFuncErr(HI_Flash_OpenByName, LDDB_ERR_DEVICE);

        LDDB_MemSet(&stBlockInfo, 0x00, sizeof(stBlockInfo));
        s32Ret = lddbGetBlockInfo(pcPartitionName, &stBlockInfo);
        if (HI_SUCCESS != s32Ret)
        {
            LDDB_PrintFuncErr(lddbGetBlockInfo, s32Ret);
            return s32Ret;
        }

        hFlashHandle = HI_Flash_OpenByName(stBlockInfo.acPartitionName);
        if (HI_INVALID_HANDLE == hFlashHandle)
        {
            LDDB_PrintFuncErr(HI_Flash_OpenByName, LDDB_ERR_DEVICE);
            return LDDB_ERR_DEVICE;
        }

        LDDB_MemSet(&stFlashInfo, 0x00, sizeof(stFlashInfo));
        s32Ret = HI_Flash_GetInfo(hFlashHandle, &stFlashInfo);
        if (HI_SUCCESS != s32Ret)
        {
            LDDB_PrintFuncErr(HI_Flash_GetInfo, s32Ret);
            HI_Flash_Close(hFlashHandle);
            return LDDB_ERR_DEVICE;
        }

        if ((stFlashInfo.BlockSize < stBlockInfo.u32Offset)
            || (stFlashInfo.BlockSize < (stBlockInfo.u32Offset + stBlockInfo.u32Size)))
        {
            HI_Flash_Close(hFlashHandle);
            LDDB_PrintErrCode(LDDB_ERR_DEVICE);
            return LDDB_ERR_DEVICE;
        }

        u32Offset = stBlockInfo.u32Offset;
        u32Size   = stBlockInfo.u32Size;
    }
    else
    {
        LDDB_MemSet(&stFlashInfo, 0x00, sizeof(stFlashInfo));
        s32Ret = HI_Flash_GetInfo(hFlashHandle, &stFlashInfo);
        if (HI_SUCCESS != s32Ret)
        {
            LDDB_PrintFuncErr(HI_Flash_GetInfo, s32Ret);
            HI_Flash_Close(hFlashHandle);
            return s32Ret;
        }

        u32Offset = 0;
        u32Size   = stFlashInfo.BlockSize;
    }

    pstFlashSession = LDDB_MemMalloc(sizeof(LDDB_FLASH_SESSION_S));
    if (HI_NULL_PTR == pstFlashSession)
    {
        HI_Flash_Close(hFlashHandle);
        LDDB_PrintErrCode(LDDB_ERR_MEM);
        return LDDB_ERR_MEM;
    }

    LDDB_MemSet(pstFlashSession, 0x00, sizeof(LDDB_FLASH_SESSION_S));
    pstFlashSession->hFlashHandle = hFlashHandle;
    pstFlashSession->u64PartitionOffset = u32Offset;
    pstFlashSession->u32PartitionSize = u32Size;

    *pu32Size = u32Size;
    *phHandle = (LDDB_HANDLE)(pstFlashSession);

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_FlashClose(LDDB_HANDLE hFlashHandle)
{
    HI_S32 s32Ret = HI_FAILURE;
    LDDB_FLASH_SESSION_S* pstFlashSession = HI_NULL_PTR;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == hFlashHandle);

    pstFlashSession = (LDDB_FLASH_SESSION_S*)(hFlashHandle);
    s32Ret = HI_Flash_Close(pstFlashSession->hFlashHandle);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(HI_Flash_Close, s32Ret);
        return LDDB_ERR_DEVICE;
    }

    LDDB_MemFreeSetNull(pstFlashSession);

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_FlashGetSize(LDDB_HANDLE hFlashHandle, HI_U32* pu32Size)
{
    LDDB_FLASH_SESSION_S* pstFlashSession = HI_NULL_PTR;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == hFlashHandle);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu32Size);

    pstFlashSession = (LDDB_FLASH_SESSION_S*)(hFlashHandle);

    *pu32Size = pstFlashSession->u32PartitionSize;

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_FlashRead(LDDB_HANDLE hFlashHandle, HI_U8* pu8Buffer, HI_U32 u32Size, HI_U32* pu32RetSize)
{
    HI_S32 s32Ret = HI_FAILURE;
    LDDB_FLASH_SESSION_S* pstFlashSession = HI_NULL_PTR;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == hFlashHandle);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8Buffer);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu32RetSize);
    LDDB_CHECK_PARAM(0 == u32Size);

    pstFlashSession = (LDDB_FLASH_SESSION_S*)(hFlashHandle);

    s32Ret = HI_Flash_Read(pstFlashSession->hFlashHandle, pstFlashSession->u64PartitionOffset, pu8Buffer, u32Size, HI_FLASH_RW_FLAG_RAW);
    if (0 >= s32Ret)
    {
        LDDB_PrintFuncErr(HI_Flash_Read, s32Ret);
        return LDDB_ERR_DEVICE;
    }

    *pu32RetSize = (HI_U32)s32Ret;

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_FlashWrite(LDDB_HANDLE hFlashHandle, HI_U8* pu8Buffer, HI_U32 u32Size)
{
    LDDB_FLASH_SESSION_S* pstFlashSession = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == hFlashHandle);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8Buffer);
    LDDB_CHECK_PARAM(0 == u32Size);

    pstFlashSession = (LDDB_FLASH_SESSION_S*)(hFlashHandle);
    if (u32Size > pstFlashSession->u32PartitionSize)
    {
        LDDB_PrintErrCode(LDDB_ERR_OVERFLOW);
        return LDDB_ERR_OVERFLOW;
    }

    s32Ret = HI_Flash_Erase(pstFlashSession->hFlashHandle, pstFlashSession->u64PartitionOffset, (HI_U64)u32Size);
    if (0 > s32Ret)
    {
        LDDB_PrintFuncErr(HI_Flash_Erase, s32Ret);
        return LDDB_ERR_DEVICE;
    }

    s32Ret = HI_Flash_Write(pstFlashSession->hFlashHandle, pstFlashSession->u64PartitionOffset, pu8Buffer, u32Size, 0);
    if (0 >= s32Ret)
    {
        LDDB_PrintFuncErr(HI_Flash_Write, s32Ret);
        return LDDB_ERR_DEVICE;
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_FlashDataSign
(
    LDDB_HARDEN_TYPE_E  enVerifyType,
    HI_U8*              pu8DataAddr,
    HI_U32              u32DataSize,
    HI_U8*              pu8FlashData,
    HI_U32              u32FlashSize
)
{
    HI_U32 u32TotalSize = 0;
    HI_U32 u32HardenedDataLen = 0;
    HI_U32 u32SignDataLen = 0;
    HI_U8* pucSignDataPos = HI_NULL_PTR;
    HI_U8* pu8DataData = HI_NULL_PTR;
    LDDB_PARTITON_HEADER_S* pstHeader = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8DataAddr);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8FlashData);
    LDDB_CHECK_PARAM(0 == u32DataSize);
    LDDB_CHECK_PARAM(0 == u32FlashSize);

    u32HardenedDataLen = (u32DataSize + LDDB_D_HARDEN_BYTEWIDTH - 1) & (~(LDDB_D_HARDEN_BYTEWIDTH - 1));
    u32TotalSize = sizeof(LDDB_PARTITON_HEADER_S) + u32HardenedDataLen;
    if (u32TotalSize > u32FlashSize)
    {
        LDDB_PrintErrCode(LDDB_ERR_OVERFLOW);
        return LDDB_ERR_OVERFLOW;
    }

    s32Ret = LDDB_HARDEN_Init();
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_HARDEN_Init, s32Ret);
        LDDB_MemFreeSetNull(pu8FlashData);
        return s32Ret;
    }

    pu8DataData = pu8FlashData + sizeof(LDDB_PARTITON_HEADER_S);
    LDDB_MemCpy(pu8DataData, pu8DataAddr, u32DataSize);

    s32Ret = LDDB_HARDEN_DataEncrypt(enVerifyType, pu8DataData, u32HardenedDataLen);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_HARDEN_DataEncrypt, s32Ret);
        LDDB_CHECK(LDDB_HARDEN_DeInit());
        LDDB_MemFreeSetNull(pu8FlashData);
        return s32Ret;
    }

    pstHeader = (LDDB_PARTITON_HEADER_S*)(HI_VOID*)pu8FlashData;

    pstHeader->u32HardenedDataSize = u32HardenedDataLen;
    pstHeader->u32DataSize = u32DataSize;

    pucSignDataPos = pu8FlashData + sizeof(pstHeader->au8VerifyValue);
    u32SignDataLen = u32TotalSize - sizeof(pstHeader->au8VerifyValue);

    s32Ret = LDDB_HARDEN_DataSignature(enVerifyType, pucSignDataPos, u32SignDataLen, pstHeader->au8VerifyValue);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_HARDEN_DataSignature, s32Ret);
        LDDB_CHECK(LDDB_HARDEN_DeInit());
        LDDB_MemFreeSetNull(pu8FlashData);
        return s32Ret;
    }

    LDDB_CHECK(LDDB_HARDEN_DeInit());

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_FlashDataVerify
(
    LDDB_HARDEN_TYPE_E  enVerifyType,
    HI_U8*              pu8FlashData,
    HI_U32              u32FlashSize,
    HI_U8**             ppu8DataAddr,
    HI_U32*             pu32DataSize
)
{
    HI_U32 u32HardenedDataLen = 0;
    HI_U32 u32VerifyDataLen = 0;
    HI_U8* pucVerifyDataPos = HI_NULL_PTR;
    HI_U8* pucDataPos = HI_NULL_PTR;
    LDDB_PARTITON_HEADER_S* pstHeader = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8FlashData);
    LDDB_CHECK_PARAM(HI_NULL_PTR == ppu8DataAddr);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu32DataSize);
    LDDB_CHECK_PARAM(0 == u32FlashSize);

    if (sizeof(LDDB_PARTITON_HEADER_S) >= u32FlashSize)
    {
        LDDB_PrintErrCode(LDDB_ERR_DATABASE);
        return LDDB_ERR_DATABASE;
    }

    pstHeader = (LDDB_PARTITON_HEADER_S*)(HI_VOID*)pu8FlashData;
    u32HardenedDataLen = (0 == pstHeader->u32HardenedDataSize) ? pstHeader->u32DataSize : pstHeader->u32HardenedDataSize;

    if ((0 == u32HardenedDataLen)
        || (u32HardenedDataLen < pstHeader->u32DataSize)
        || (u32HardenedDataLen > u32FlashSize - sizeof(LDDB_PARTITON_HEADER_S)))
    {
        LDDB_PrintErrCode(LDDB_ERR_DATABASE);
        return LDDB_ERR_DATABASE;
    }

    s32Ret = LDDB_HARDEN_Init();
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_HARDEN_Init, s32Ret);
        return s32Ret;
    }

    pucVerifyDataPos = pu8FlashData + sizeof(pstHeader->au8VerifyValue);
    u32VerifyDataLen = u32HardenedDataLen + sizeof(LDDB_PARTITON_HEADER_S) - sizeof(pstHeader->au8VerifyValue);
    s32Ret = LDDB_HARDEN_DataVerify(enVerifyType, pucVerifyDataPos, u32VerifyDataLen, pstHeader->au8VerifyValue);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_HARDEN_DataVerify, s32Ret);
        LDDB_CHECK(LDDB_HARDEN_DeInit());
        return s32Ret;
    }

    pucDataPos = pu8FlashData + sizeof(LDDB_PARTITON_HEADER_S);
    s32Ret = LDDB_HARDEN_DataDecrypt(enVerifyType, pucDataPos, u32HardenedDataLen);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_HARDEN_DataDecrypt, s32Ret);
        LDDB_CHECK(LDDB_HARDEN_DeInit());
        return s32Ret;
    }

    LDDB_CHECK(LDDB_HARDEN_DeInit());

    *ppu8DataAddr = pucDataPos;
    *pu32DataSize = pstHeader->u32DataSize;

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_PartitionOpen(HI_CHAR* pcPartitionName, LDDB_HANDLE* phPartitionHandle, HI_U32* pu32PartitionSize)
{
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pcPartitionName);
    LDDB_CHECK_PARAM(HI_NULL_PTR == phPartitionHandle);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu32PartitionSize);

    s32Ret = LDDB_FlashOpen(pcPartitionName, phPartitionHandle, pu32PartitionSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_FlashOpen, s32Ret);
        return s32Ret;
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_PartitionClose(LDDB_HANDLE hPartitionHandle)
{
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == hPartitionHandle);

    s32Ret = LDDB_FlashClose(hPartitionHandle);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_FlashClose, s32Ret);
        return LDDB_ERR_DEVICE;
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_PartitionGetSize(LDDB_HANDLE hPartitionHandle, HI_U32* pu32Size)
{
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == hPartitionHandle);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu32Size);

    s32Ret = LDDB_FlashGetSize(hPartitionHandle, pu32Size);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_FlashGetSize, s32Ret);
        return LDDB_ERR_DEVICE;
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_PartitionDataRead(LDDB_HANDLE hPartitionHandle, HI_U8* pu8Buffer, HI_U32 u32Size, HI_U32* pu32RetSize)
{
    HI_U32 u32RetSize = 0;
    HI_U32 u32DataSize = 0;
    HI_U32 u32PartitionSize = 0;
    HI_U8* pu8PartitionBuff = HI_NULL_PTR;
    HI_U8* pu8DataAddr = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == hPartitionHandle);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8Buffer);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu32RetSize);
    LDDB_CHECK_PARAM(0 == u32Size);

    s32Ret = LDDB_FlashGetSize(hPartitionHandle, &u32PartitionSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_FlashGetSize, s32Ret);
        return s32Ret;
    }

    pu8PartitionBuff = LDDB_MemMalloc(u32PartitionSize);
    if (HI_NULL_PTR == pu8PartitionBuff)
    {
        LDDB_PrintErrCode(LDDB_ERR_MEM);
        return LDDB_ERR_MEM;
    }

    LDDB_MemSet(pu8PartitionBuff, 0x00, u32PartitionSize);
    s32Ret = LDDB_FlashRead(hPartitionHandle, pu8PartitionBuff, u32PartitionSize, &u32RetSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_FlashRead, s32Ret);
        LDDB_MemFreeSetNull(pu8PartitionBuff);
        return s32Ret;
    }

    if (u32RetSize != u32PartitionSize)
    {
        LDDB_MemFreeSetNull(pu8PartitionBuff);
        LDDB_PrintErrCode(LDDB_ERR_DEVICE);
        return LDDB_ERR_DEVICE;
    }

    s32Ret = LDDB_FlashDataVerify(LDDB_D_HARDEN_TYPE, pu8PartitionBuff, u32PartitionSize, &pu8DataAddr, &u32DataSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_FlashDataVerify, s32Ret);
        LDDB_MemFreeSetNull(pu8PartitionBuff);
        return s32Ret;
    }

    u32RetSize = (u32Size > u32DataSize) ? u32DataSize : u32Size;

    LDDB_MemCpy(pu8Buffer, pu8DataAddr, u32RetSize);
    LDDB_MemFreeSetNull(pu8PartitionBuff);

    *pu32RetSize = u32RetSize;

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_PartitionDataWrite(LDDB_HANDLE hPartitionHandle, HI_U8* pu8Data, HI_U32 u32Size)
{
    HI_U32 u32PartitionSize = 0;
    HI_U8* pu8FlashData = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == hPartitionHandle);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8Data);
    LDDB_CHECK_PARAM(0 == u32Size);

    s32Ret = LDDB_FlashGetSize(hPartitionHandle, &u32PartitionSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_FlashGetSize, s32Ret);
        return s32Ret;
    }

    if (u32Size > u32PartitionSize)
    {
        LDDB_PrintErrCode(LDDB_ERR_OVERFLOW);
        return LDDB_ERR_OVERFLOW;
    }

    pu8FlashData = LDDB_MemMalloc(u32PartitionSize);
    if (HI_NULL_PTR == pu8FlashData)
    {
        LDDB_PrintErrCode(LDDB_ERR_MEM);
        return LDDB_ERR_MEM;
    }

    LDDB_MemSet(pu8FlashData, 0x00, u32PartitionSize);
    s32Ret = LDDB_FlashDataSign(LDDB_D_HARDEN_TYPE, pu8Data, u32Size, pu8FlashData, u32PartitionSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_FlashDataSign, s32Ret);
        LDDB_MemFreeSetNull(pu8FlashData);
        return s32Ret;
    }

    s32Ret = LDDB_FlashWrite(hPartitionHandle, pu8FlashData, u32PartitionSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_MemFreeSetNull(pu8FlashData);
        LDDB_PrintFuncErr(LDDB_FlashWrite, s32Ret);
        return s32Ret;
    }

    LDDB_MemFreeSetNull(pu8FlashData);

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;

}

HI_S32 LDDB_GetLoaderInfo(LDDB_LOADER_INFO_S* pstLoaderInfo)
{
    HI_BOOL bValidM = HI_TRUE;
    HI_BOOL bValidB = HI_TRUE;
    LDDB_LOADER_INFO_S stLoaderInfoM;
    LDDB_LOADER_INFO_S stLoaderInfoB;

    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pstLoaderInfo);

    LDDB_MemSet(&stLoaderInfoM, 0x00, sizeof(stLoaderInfoM));
    LDDB_MemSet(&stLoaderInfoB, 0x00, sizeof(stLoaderInfoB));

    s32Ret = lddbGetLoaderDBInfo(LDDB_D_FLASHMAP_LOADERDB, &stLoaderInfoM);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(lddbGetLoaderDBInfo, s32Ret);
        bValidM = HI_FALSE;
    }

    s32Ret = lddbGetLoaderDBInfo(LDDB_D_FLASHMAP_LOADERDBBAK, &stLoaderInfoB);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(lddbGetLoaderDBInfo, s32Ret);
        bValidB = HI_FALSE;
    }

    if ((HI_TRUE == bValidM)
        && (HI_TRUE != bValidB))
    {
        LDDB_CHECK(lddbSetLoaderDBInfo(LDDB_D_FLASHMAP_LOADERDBBAK, &stLoaderInfoM));
        LDDB_MemCpy(pstLoaderInfo, &stLoaderInfoM, sizeof(LDDB_LOADER_INFO_S));

        LDDB_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if ((HI_TRUE != bValidM)
        && (HI_TRUE == bValidB))
    {
        LDDB_CHECK(lddbSetLoaderDBInfo(LDDB_D_FLASHMAP_LOADERDB, &stLoaderInfoB));
        LDDB_MemCpy(pstLoaderInfo, &stLoaderInfoB, sizeof(LDDB_LOADER_INFO_S));

        LDDB_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if ((HI_TRUE != bValidM)
        && (HI_TRUE != bValidB))
    {
        LDDB_PrintErrCode(LDDB_ERR_DEVICE);
        return LDDB_ERR_DEVICE;
    }

    if (0 != LDDB_MemCmp(&stLoaderInfoM, &stLoaderInfoB, sizeof(stLoaderInfoM)))
    {
        LDDB_CHECK(lddbSetLoaderDBInfo(LDDB_D_FLASHMAP_LOADERDBBAK, &stLoaderInfoM));
    }

    LDDB_MemCpy(pstLoaderInfo, &stLoaderInfoM, sizeof(LDDB_LOADER_INFO_S));

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_SetLoaderInfo(LDDB_LOADER_INFO_S* pstLoaderInfo)
{
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pstLoaderInfo);

    s32Ret  = lddbSetLoaderDBInfo(LDDB_D_FLASHMAP_LOADERDB, pstLoaderInfo);
    s32Ret &= lddbSetLoaderDBInfo(LDDB_D_FLASHMAP_LOADERDBBAK, pstLoaderInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(lddbSetLoaderDBInfo, s32Ret);
        return s32Ret;
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_GetSoftwareInfo(LDDB_SOFTWARE_INFO_S* pstSoftwareInfo)
{
    HI_U32 i = 0;
    HI_U32 u32MemSize = 0;
    HI_U32 u32RetSize = 0;
    HI_U32 u32PartitionSize = 0;
    HI_U32 u32ReadLength = 0;
    LDDB_SOFTWARE_INFO_S stSoftwareInfo;
    HI_U8* pu8PartitionData = HI_NULL_PTR;
    HI_U8* pu8ReadPosition = HI_NULL_PTR;
    LDDB_HANDLE hPartitionHandle = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pstSoftwareInfo);

    s32Ret = LDDB_PartitionOpen(LDDB_D_FLASHMAP_SOFTWAREINFO, &hPartitionHandle, &u32PartitionSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_PartitionOpen, s32Ret);
        return s32Ret;
    }

    if (u32PartitionSize < sizeof(LDDB_SOFTWARE_INFO_S))
    {
        LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));
        LDDB_PrintErrCode(LDDB_ERR_OVERFLOW);
        return LDDB_ERR_OVERFLOW;
    }

    pu8PartitionData = LDDB_MemMalloc(u32PartitionSize);
    if (HI_NULL_PTR == pu8PartitionData)
    {
        LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));
        LDDB_PrintErrCode(LDDB_ERR_MEM);
        return LDDB_ERR_MEM;
    }

    LDDB_MemSet(pu8PartitionData, 0x00, u32PartitionSize);
    s32Ret = LDDB_PartitionDataRead(hPartitionHandle, (HI_U8*)pu8PartitionData, u32PartitionSize, &u32RetSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_PartitionDataRead, s32Ret);
        LDDB_MemFreeSetNull(pu8PartitionData);
        LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));
        return s32Ret;
    }

    pu8ReadPosition = pu8PartitionData;
    LDDB_MemSet(&stSoftwareInfo, 0x00, sizeof(stSoftwareInfo));
    LDDB_READ_32BIT(pu8ReadPosition, stSoftwareInfo.u32UpgradeIdentification, u32ReadLength);
    LDDB_READ_32BIT(pu8ReadPosition, stSoftwareInfo.u32SoftwareVersion, u32ReadLength);
    LDDB_READ_32BIT(pu8ReadPosition, stSoftwareInfo.u32PartitionCount, u32ReadLength);

    u32MemSize = stSoftwareInfo.u32PartitionCount * sizeof(LDDB_PARTITION_INFO_S);
    if (u32ReadLength + u32MemSize != u32RetSize)
    {
        LDDB_MemFreeSetNull(pu8PartitionData);
        LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));

        LDDB_PrintErrCode(LDDB_ERR_DEVICE);
        return LDDB_ERR_DEVICE;
    }

    if (0 < stSoftwareInfo.u32PartitionCount)
    {
        stSoftwareInfo.pstPartitionInfo = LDDB_MemMalloc(u32MemSize);
        if (HI_NULL_PTR == stSoftwareInfo.pstPartitionInfo)
        {
            LDDB_MemFreeSetNull(pu8PartitionData);
            LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));

            LDDB_PrintErrCode(LDDB_ERR_MEM);
            return LDDB_ERR_MEM;
        }

        LDDB_MemSet(stSoftwareInfo.pstPartitionInfo, 0x00, u32MemSize);
        for (i = 0; i < stSoftwareInfo.u32PartitionCount; i++)
        {
            LDDB_READ_BYTES(pu8ReadPosition, stSoftwareInfo.pstPartitionInfo[i].au8Name, LDDB_D_LEN_PARTNAME, u32ReadLength);
            LDDB_READ_32BIT(pu8ReadPosition, stSoftwareInfo.pstPartitionInfo[i].u32Version, u32ReadLength);
        }
    }

    LDDB_MemCpy(pstSoftwareInfo, &stSoftwareInfo, sizeof(LDDB_SOFTWARE_INFO_S));

    LDDB_MemFreeSetNull(pu8PartitionData);
    LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_SetSoftwareInfo(LDDB_SOFTWARE_INFO_S* pstSoftwareInfo)
{
    HI_U32 i = 0;
    HI_U32 u32WriteLength = 0;
    HI_U32 u32PartitionSize = 0;
    HI_U8* pu8PartitionData = HI_NULL_PTR;
    HI_U8* pu8WritePosition = HI_NULL_PTR;
    LDDB_HANDLE hPartitionHandle = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pstSoftwareInfo);

    s32Ret = LDDB_PartitionOpen(LDDB_D_FLASHMAP_SOFTWAREINFO, &hPartitionHandle, &u32PartitionSize);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_PartitionOpen, s32Ret);
        return s32Ret;
    }

    pu8PartitionData = LDDB_MemMalloc(u32PartitionSize);
    if (HI_NULL_PTR == pu8PartitionData)
    {
        LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));
        LDDB_PrintErrCode(LDDB_ERR_MEM);
        return LDDB_ERR_MEM;
    }

    LDDB_MemSet(pu8PartitionData, 0x00, u32PartitionSize);
    pu8WritePosition = pu8PartitionData;
    LDDB_WRITE_32BIT(pu8WritePosition, pstSoftwareInfo->u32UpgradeIdentification, u32WriteLength);
    LDDB_WRITE_32BIT(pu8WritePosition, pstSoftwareInfo->u32SoftwareVersion, u32WriteLength);
    LDDB_WRITE_32BIT(pu8WritePosition, pstSoftwareInfo->u32PartitionCount, u32WriteLength);

    if (u32PartitionSize < u32WriteLength + pstSoftwareInfo->u32PartitionCount * sizeof(LDDB_PARTITION_INFO_S))
    {
        LDDB_MemFreeSetNull(pu8PartitionData);
        LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));
        LDDB_PrintErrCode(LDDB_ERR_OVERFLOW);
        return LDDB_ERR_OVERFLOW;
    }

    for (i = 0; i < pstSoftwareInfo->u32PartitionCount; i++)
    {
        LDDB_WRITE_BYTES(pu8WritePosition, pstSoftwareInfo->pstPartitionInfo[i].au8Name, LDDB_D_LEN_PARTNAME, u32WriteLength);
        LDDB_WRITE_32BIT(pu8WritePosition, pstSoftwareInfo->pstPartitionInfo[i].u32Version, u32WriteLength);
    }

    s32Ret = LDDB_PartitionDataWrite(hPartitionHandle, pu8PartitionData, u32WriteLength);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_PartitionDataWrite, s32Ret);
        LDDB_MemFreeSetNull(pu8PartitionData);
        LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));
        return s32Ret;
    }

    LDDB_MemFreeSetNull(pu8PartitionData);
    LDDB_CHECK(LDDB_PartitionClose(hPartitionHandle));

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_FreeSoftwareInfo(LDDB_SOFTWARE_INFO_S* pstSoftwareInfo)
{
    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pstSoftwareInfo);

    if (HI_NULL_PTR != pstSoftwareInfo->pstPartitionInfo)
    {
        LDDB_MemFreeSetNull(pstSoftwareInfo->pstPartitionInfo);
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_GetDeviceInfo(LDDB_DEVICE_INFO_S* pstDeviceInfo)
{
    HI_CHAR ac8Buf[LDDB_D_LEN_DEVICEITEM] = {0};
    HI_S32  s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pstDeviceInfo);

    LDDB_MemSet(ac8Buf, 0x00, sizeof(ac8Buf));
    s32Ret = lddbGetDeviceInfo("OUI", ac8Buf, sizeof(ac8Buf));
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(lddbGetDeviceInfo, s32Ret);
        return s32Ret;
    }

    pstDeviceInfo->u32OUI = (HI_U32)LDDB_Strtoul(ac8Buf, HI_NULL_PTR, 16);

    LDDB_MemSet(ac8Buf, 0x00, sizeof(ac8Buf));
    s32Ret = lddbGetDeviceInfo("HW", ac8Buf, sizeof(ac8Buf));
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(lddbGetDeviceInfo, s32Ret);
        return s32Ret;
    }

    pstDeviceInfo->u32HardwareVersion = (HI_U32)LDDB_Strtoul(ac8Buf, HI_NULL_PTR, 16);

    LDDB_MemSet(ac8Buf, 0x00, sizeof(ac8Buf));
    s32Ret = lddbGetDeviceInfo("SN", ac8Buf, sizeof(ac8Buf));
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(lddbGetDeviceInfo, s32Ret);
        return s32Ret;
    }

    pstDeviceInfo->u32SerialNum = (HI_U32)LDDB_Strtoul(ac8Buf, HI_NULL_PTR, 16);

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

