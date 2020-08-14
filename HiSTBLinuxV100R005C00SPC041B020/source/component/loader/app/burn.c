/*******************************************************************************
 *             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: burn.c
 * Description:
 *              abstract burn interface definition.
 * History:
 * Version   Date         Author     DefectNum    Description
 *
 ******************************************************************************/
#include "com_osal.h"
#include "com_debug.h"
#include "protocol_common.h"
#include "hi_flash.h"
#include "burn.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_U32 g_u32PartitionNum = 0;
FLASH_DATA_INFO_S* g_pstFlashDataInfo = HI_NULL_PTR;
PROT_PROCESS_CALLBACK s_pfnProgressCallback = HI_NULL_PTR;

static HI_S32 GetIndexById(HI_U32 id, HI_U32* pu32Index)
{
    HI_U8 ii = 0;

    for (ii = 0; ii < g_u32PartitionNum; ii++)
    {
        if (id == g_pstFlashDataInfo[ii].u32PartitionId)
        {
            *pu32Index = ii;
            return HI_SUCCESS;
        }
    }

    return HI_FAILURE;
}

static HI_S32 FlashDataInit(PROT_PARTITION_INFO_S* pstPartionInfo, FLASH_DATA_INFO_S* pstFlashDataInfo)
{
    HI_Flash_InterInfo_S stFlashInfo;
    HI_U64 u64FlashStartAddr = 0;
    HI_U64 u64FlashEndAddr = 0;
    HI_U32 u32FlashType = 0;
    HI_U32 u32FlashIndex = 0;

    HI_HANDLE hHandle = 0;
    HI_S32 s32Ret = HI_FAILURE;

    u64FlashStartAddr = pstPartionInfo->u64FlashStartAddr;
    u64FlashEndAddr = pstPartionInfo->u64FlashEndAddr;
    u32FlashType = pstPartionInfo->u32FlashType;
    u32FlashIndex = pstPartionInfo->u32FlashIndex;

    hHandle = HI_Flash_Open((HI_FLASH_TYPE_E)u32FlashType, NULL, u64FlashStartAddr, (u64FlashEndAddr - u64FlashStartAddr));
    if (HI_INVALID_HANDLE == hHandle)
    {
        COM_PrintFuncErr(HI_Flash_Open, COM_ERR_DEVICE);
        return COM_ERR_DEVICE;
    }

    COM_MemSet(&stFlashInfo, 0x00, sizeof(stFlashInfo));
    s32Ret = HI_Flash_GetInfo(hHandle, &stFlashInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_Flash_GetInfo, COM_ERR_DEVICE);

        HI_Flash_Close(hHandle);
        hHandle = HI_NULL_PTR;

        return COM_ERR_DEVICE;
    }

    if (u64FlashEndAddr > stFlashInfo.TotalSize)
    {
        HI_Flash_Close(hHandle);
        hHandle = HI_NULL_PTR;

        return COM_ERR_DEVICE;
    }

    pstFlashDataInfo->u32PartitionId = pstPartionInfo->u32PartitionId;
    pstFlashDataInfo->hFlashHandle = hHandle;
    pstFlashDataInfo->enFlashType = (HI_FLASH_TYPE_E)u32FlashType;
    pstFlashDataInfo->u64WriteOffset = 0;
    pstFlashDataInfo->bErased = HI_FALSE;
    pstFlashDataInfo->u64PartitionSize = u64FlashEndAddr - u64FlashStartAddr;
    pstFlashDataInfo->private = HI_NULL_PTR;
    pstFlashDataInfo->u32ImageSize = pstPartionInfo->u32ImageSize;

    /*Adjust  update file system */
    if (0 != ((u32FlashIndex) >> 16))
    {
        /*write yaffs filse system*/
        pstFlashDataInfo->u32WriteFlags = HI_FLASH_RW_FLAG_WITH_OOB;
        pstFlashDataInfo->u32DataBlockSize = (stFlashInfo.BlockSize / stFlashInfo.PageSize)
                                              * (stFlashInfo.PageSize + stFlashInfo.OobSize);
        pstFlashDataInfo->u32FlashBlockSize = stFlashInfo.BlockSize;
    }
    else
    {
        /*write row data.*/
        pstFlashDataInfo->u32WriteFlags = HI_FLASH_RW_FLAG_RAW;
        pstFlashDataInfo->u32DataBlockSize = stFlashInfo.BlockSize;
        pstFlashDataInfo->u32FlashBlockSize = stFlashInfo.BlockSize;
    }

#ifdef HI_LOADER_BOOTLOADER
    MMZ_BUFFER_S stMMZ;

    s32Ret = HI_MEM_Alloc(&stMMZ.u32StartPhyAddr, pstFlashDataInfo->u32DataBlockSize);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(s32Ret, s32Ret);

        HI_Flash_Close(hHandle);
        hHandle = HI_NULL_PTR;

        return HI_FAILURE;

    }

    pstFlashDataInfo->pu8Buff = (HI_U8*)stMMZ.u32StartPhyAddr;

#else
    pstFlashDataInfo->pu8Buff = (HI_U8*)COM_MemMalloc(pstFlashDataInfo->u32DataBlockSize);
    if (HI_NULL_PTR == pstFlashDataInfo->pu8Buff)
    {
        COM_PrintErrCode(COM_ERR_MEM);

        HI_Flash_Close(hHandle);
        hHandle = HI_NULL_PTR;

        return HI_FAILURE;
    }

#endif

    pstFlashDataInfo->u32BuffLen = 0;

    return HI_SUCCESS;
}

static HI_S32 FlashDataDeInit(FLASH_DATA_INFO_S* pstFlashDataInfo)
{

#ifdef HI_LOADER_BOOTLOADER
    /* no need free boot reserved memory */
#else
    if (HI_NULL_PTR != pstFlashDataInfo->pu8Buff)
    {
        COM_MemFreeSetNull(pstFlashDataInfo->pu8Buff);
    }

#endif

    if (HI_NULL_PTR != pstFlashDataInfo->private)
    {
        COM_MemFreeSetNull(pstFlashDataInfo->private);
    }


    COM_CHECK(HI_Flash_Close(pstFlashDataInfo->hFlashHandle));
    pstFlashDataInfo->hFlashHandle = HI_INVALID_HANDLE;

    return HI_SUCCESS;
}

/*
 * for different image style(ext4sp, raw, ca...) has different burn method implemention
 */
static HI_S32 WritePartitionData(FLASH_DATA_INFO_S* pstFlashDataInfo, COM_BUFFER_DATABLOCK_S* pstPartInfo)
{
    /* ext4sp */
    if (HI_TRUE == LOADER_BURN_Identify_ext4sp(pstPartInfo->pu8DataBuff, pstPartInfo->u32DataSize))
    {
        COM_CHECK(LOADER_BURN_WriteData_ext4sp(pstFlashDataInfo,
                                               pstPartInfo->pu8DataBuff,
                                               pstPartInfo->u32DataSize,
                                               pstPartInfo->bLastData));
    }
    /* raw */
    else
    {
        COM_CHECK(LOADER_BURN_WriteData_raw(pstFlashDataInfo,
                                            pstPartInfo->pu8DataBuff,
                                            pstPartInfo->u32DataSize,
                                            pstPartInfo->bLastData));
    }

    return HI_SUCCESS;
}

HI_S32 LOADER_BURN_Init(PROT_PARTITION_INFO_S* pstPartInfo, HI_U32 u32PartNum, PROT_PROCESS_CALLBACK pfnProgressCallback)
{
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 u32MemSize = 0;
    FLASH_DATA_INFO_S* pstFlashDataInfo = HI_NULL_PTR;

    HI_S32 s32Ret = HI_FAILURE;

    COM_CHECK_PARAM(HI_NULL_PTR == pstPartInfo);
    COM_CHECK_PARAM(0 == u32PartNum);

    u32MemSize = u32PartNum * sizeof(FLASH_DATA_INFO_S);
    pstFlashDataInfo = (FLASH_DATA_INFO_S*)COM_MemMalloc(u32MemSize);
    if (HI_NULL_PTR == pstFlashDataInfo)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return HI_FAILURE;
    }

    COM_MemSet(pstFlashDataInfo, 0x00, u32MemSize);

    for (i = 0; i < u32PartNum; i++)
    {
        s32Ret = FlashDataInit(&pstPartInfo[i], &pstFlashDataInfo[i]);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(FlashDataInit, s32Ret);

            for (j = 0; j < i; j++)
            {
                COM_CHECK(FlashDataDeInit(&pstFlashDataInfo[j]));
            }

            COM_MemFreeSetNull(pstFlashDataInfo);

            return s32Ret;
        }
    }

    g_u32PartitionNum   = u32PartNum;
    g_pstFlashDataInfo  = pstFlashDataInfo;
    s_pfnProgressCallback = pfnProgressCallback;

    return HI_SUCCESS;
}

HI_S32 LOADER_BURN_DeInit(HI_VOID)
{
    HI_U32 i = 0;

    for (i = 0; i < g_u32PartitionNum; i++)
    {
        FlashDataDeInit(&g_pstFlashDataInfo[i]);
    }

    COM_MemFreeSetNull(g_pstFlashDataInfo);
    g_u32PartitionNum = 0;

    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      LOADER_BURN_WriteData
* Description:   update data to flash
* Data Accessed:
* Data Updated:
* Input:
* Output:
* Return: HI_SUCCESS
* Others:
*****************************************************************************/
HI_S32 LOADER_BURN_WriteData(COM_BUFFER_DATABLOCK_S* pstPartInfo, HI_U32 u32PartNum)
{
    HI_U32 i = 0;
    HI_U32 u32Index = 0;
    HI_U32 u32BurnData = 0;
    HI_U32 u32TotalData = 0;
    HI_S32 s32Ret = HI_FAILURE;

    COM_CHECK_PARAM(HI_NULL_PTR == pstPartInfo);

    for (i = 0; i < u32PartNum; i++)
    {
        s32Ret = GetIndexById(pstPartInfo[i].u32ModuleID, &u32Index);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(GetIndexById, s32Ret);
            return s32Ret;
        }

        s32Ret = WritePartitionData(&g_pstFlashDataInfo[u32Index], &pstPartInfo[i]);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(WritePartitionData, s32Ret);
            return s32Ret;
        }

        g_pstFlashDataInfo[u32Index].u32BurnSize += pstPartInfo[i].u32DataSize;
    }


    if (HI_NULL_PTR != s_pfnProgressCallback)
    {
        for (i = 0; i < g_u32PartitionNum; i++)
        {
            u32BurnData += g_pstFlashDataInfo[i].u32BurnSize;
            u32TotalData += g_pstFlashDataInfo[i].u32ImageSize;
        }

        s32Ret = s_pfnProgressCallback(u32BurnData, u32TotalData);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(s_pfnProgressCallback, s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
