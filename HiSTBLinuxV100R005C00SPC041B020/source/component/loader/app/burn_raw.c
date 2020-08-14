/*******************************************************************************
 *             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: burn.c
 * Description:
 *              burn raw image implemention.
 * History:
 * Version   Date         Author     DefectNum    Description
 *
 ******************************************************************************/
#include "com_debug.h"
#include "com_osal.h"
#include "hi_flash.h"
#include "burn.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
* Function:      LOADER_BURN_WriteData_raw
* Description:   update data to flash
* Data Accessed:
* Data Updated:
* Input:
* Output:
* Return: HI_SUCCESS
* Others:
*****************************************************************************/
HI_S32 LOADER_BURN_WriteData_raw(FLASH_DATA_INFO_S* pstFlashDataInfo, HI_U8* pucSrcDataBuff, HI_U32 u32SrcDataLen, HI_VOID* private)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S64 s64Ret = HI_SUCCESS;
    HI_U8* p = HI_NULL_PTR;
    HI_U32 u32len = 0;
    HI_U32 u32RemainLen = 0;
    HI_U32 u32DataBlockLen  = 0;
    HI_U32 u32FlashBlockLen = 0;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    HI_U32 u32DataLen = 0;
    HI_U32 write_len = 0;
    HI_U32 u32AdjustValue = 0;
    HI_U8*  pucBuff = HI_NULL_PTR;
    HI_BOOL bLastData = (HI_BOOL)((HI_U32)private);

    COM_CHECK_PARAM(HI_NULL_PTR == pucSrcDataBuff);
    COM_CHECK_PARAM(HI_NULL_PTR == pstFlashDataInfo);

    if (u32SrcDataLen > pstFlashDataInfo->u64PartitionSize)
    {
        COM_ERROR("burn size has overflow the partiton size,burn size is %#x, partition size is %#x\n",
                  u32SrcDataLen, pstFlashDataInfo->u64PartitionSize);
        return COM_ERR_FLASH_WRITE;
    }

    /*record  buffer  point*/
    u32len = 0;
    hHandle = pstFlashDataInfo->hFlashHandle;
    u32DataBlockLen  = pstFlashDataInfo->u32DataBlockSize;
    u32FlashBlockLen = pstFlashDataInfo->u32FlashBlockSize;

    if (HI_TRUE != pstFlashDataInfo->bErased)
    {
        s64Ret = HI_Flash_Erase64(hHandle, (HI_U64)0, pstFlashDataInfo->u64PartitionSize);
        if (0 > s64Ret)
        {
            COM_ERROR("Burn Erase flash error! s64Ret:0x%x\n", s64Ret);
            return COM_ERR_FLASH_WRITE;
        }

        pstFlashDataInfo->bErased = HI_TRUE;
    }

    pucBuff = (HI_U8*)COM_MemMalloc(u32DataBlockLen);
    if (HI_NULL_PTR == pucBuff)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    COM_MemSet(pucBuff, 0x00, u32DataBlockLen);
    u32RemainLen = pstFlashDataInfo->u32BuffLen;
    p = pucSrcDataBuff;
    u32DataLen = u32SrcDataLen;

    /*把上次剩下的数据写入flash*/
    if (0 != u32RemainLen)
    {
        COM_MemCpy(pucBuff, pstFlashDataInfo->pu8Buff, u32RemainLen);

        if (u32DataLen > u32DataBlockLen - u32RemainLen)
        {
            COM_MemCpy(pucBuff + u32RemainLen, pucSrcDataBuff, u32DataBlockLen - u32RemainLen);
            u32DataLen -= u32DataBlockLen - u32RemainLen;
        }
        else
        {
            COM_MemCpy(pucBuff + u32RemainLen, pucSrcDataBuff, u32DataLen);
            COM_MemSet(pucBuff + u32RemainLen + u32DataLen, 0xff, u32DataBlockLen - u32RemainLen - u32DataLen);
            u32DataLen = 0;
        }

        s32Ret = HI_Flash_Write(hHandle,
                                pstFlashDataInfo->u64WriteOffset,
                                pucBuff,
                                u32DataBlockLen,
                                pstFlashDataInfo->u32WriteFlags);

        if (0 >= s32Ret)
        {
            COM_ERROR("Write flash error!\n");
            COM_MemFreeSetNull(pucBuff);
            return (HI_S32)COM_ERR_FLASH_WRITE;
        }

        pstFlashDataInfo->u64WriteOffset += u32FlashBlockLen;

        if (0 < u32DataLen)
        {
            p = pucSrcDataBuff + u32DataBlockLen - u32RemainLen;
        }
    }

    while (u32len < u32DataLen)
    {
        /*Insure Write len accuracy*/
        if ((u32DataLen - u32len) >= u32DataBlockLen)
        {
            write_len = u32DataBlockLen;
        }
        else
        {
            write_len = u32DataLen - u32len;

            /*为了做到块对齐,如果不是最后的数据,将不足一块的数据拷贝到缓冲区*/
            if (HI_FALSE == bLastData)
            {
                COM_MemCpy(pstFlashDataInfo->pu8Buff, p + u32len, write_len);
                pstFlashDataInfo->u32BuffLen = write_len;
                break;
            }
        }

        if (write_len < u32DataBlockLen)
        {
            COM_MemSet(pucBuff, 0xff, u32DataBlockLen);
        }

        COM_MemCpy(pucBuff, (p + u32len), write_len);
        COM_DEBUG("adjust after, data addrs :"
                  "0x%x,data len :%d,"
                  "adjust addrs : 0x%x,"
                  " adjust offset : %d\n",
                  (HI_U32)p + u32len,
                  u32len,
                  (HI_U32)pucBuff,
                  u32AdjustValue);

        s32Ret = HI_Flash_Write(hHandle,
                                pstFlashDataInfo->u64WriteOffset,
                                pucBuff,
                                u32DataBlockLen,
                                pstFlashDataInfo->u32WriteFlags);

        if (0 >= s32Ret)
        {
            COM_ERROR("\n[ERROR]: Flash write fail!\n");
            COM_MemFreeSetNull(pucBuff);
            return (HI_S32)COM_ERR_FLASH_WRITE;
        }

        u32len += write_len;
        pstFlashDataInfo->u64WriteOffset += u32FlashBlockLen;


    }

    COM_MemFreeSetNull(pucBuff);

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
