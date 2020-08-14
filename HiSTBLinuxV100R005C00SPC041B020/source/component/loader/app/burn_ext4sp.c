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

/*
 * common definition for ext4 sparse, copy from fastboot/include/sparse.h
 */
typedef struct sparse_header
{
    HI_U32 magic;           /* 0xed26ff3a */
    HI_U16 major_version;   /* (0x1) - reject images with higher major versions */
    HI_U16 minor_version;   /* (0x0) - allow images with higer minor versions */
    HI_U16 file_hdr_sz;     /* 28 bytes for first revision of the file format */
    HI_U16 chunk_hdr_sz;    /* 12 bytes for first revision of the file format */
    HI_U32 blk_sz;          /* block size in bytes, must be a multiple of 4 (4096) */
    HI_U32 total_blks;      /* total blocks in the non-sparse output image */
    HI_U32 total_chunks;    /* total chunks in the sparse input image */
    HI_U32 image_checksum;  /* CRC32 checksum of the original data, counting "don't care" */
                            /* as 0. Standard 802.3 polynomial, use a Public Domain */
                            /* table implementation */
} sparse_header_t;

#define SPARSE_HEADER_MAGIC     0xed26ff3a

#define CHUNK_TYPE_RAW          0xCAC1
#define CHUNK_TYPE_FILL         0xCAC2
#define CHUNK_TYPE_DONT_CARE    0xCAC3

typedef struct chunk_header
{
    HI_U16 chunk_type;    /* 0xCAC1 -> raw; 0xCAC2 -> fill; 0xCAC3 -> don't care */
    HI_U16 reserved1;
    HI_U32 chunk_sz;      /* in blocks in output image */
    HI_U32 total_sz;      /* in bytes of chunk input file including chunk header and data */
} chunk_header_t;

#define SPARSE_HEADER_MAJOR_VER               1

#define IS_SPARSE(_sparse_hdr) \
    (_sparse_hdr->magic == SPARSE_HEADER_MAGIC \
     && _sparse_hdr->major_version == SPARSE_HEADER_MAJOR_VER \
     && _sparse_hdr->file_hdr_sz == sizeof(sparse_header_t) \
     && _sparse_hdr->chunk_hdr_sz == sizeof(chunk_header_t))

/***************************************************************/

typedef struct sparse_chunk_ctl
{
    sparse_header_t sparseHeader;
    chunk_header_t chunkHeader;
    HI_U32           u32ChunkIndex;
    HI_BOOL         bActived;             /* chunk header valid tag */
    HI_U32           u32HeaderOffset;   /* if chunk header incomplete, valid part offset */
    HI_U32           u32DataOffset;      /* if chunk data incomplete, valid part offset */
} sparse_chunk_ctl_t;

/*
 * check and return true if it is ext4 sparse image
 */
HI_BOOL LOADER_BURN_Identify_ext4sp(HI_U8* pucBuff, HI_U32 u32DataLen)
{
    sparse_header_t* header = (sparse_header_t*)pucBuff ;

    if (!header)
    {
        return HI_FALSE;
    }

    /* data len must be big than sparse head */
    if (sizeof(sparse_header_t) > u32DataLen)
    {
        COM_DEBUG("invalid EXT4 sparse header .\n");
        return HI_FALSE;
    }

    if (!IS_SPARSE((header)))
    {
        COM_DEBUG("invalid EXT4 sparse format.\n");
        return HI_FALSE;
    }

    return HI_TRUE;
}

/*
 * create new ext4 sparse chunk control
 */
static sparse_chunk_ctl_t* LOADER_BURN_New_sparseCtl(HI_U8* pucDataBuff, HI_U32 u32DataLen)
{
    sparse_chunk_ctl_t* pstSparseCtl = HI_NULL_PTR;
    sparse_header_t* header = HI_NULL_PTR;
    HI_BOOL bRet = HI_FALSE;

    bRet = LOADER_BURN_Identify_ext4sp(pucDataBuff, u32DataLen);
    if (HI_TRUE != bRet)
    {
        COM_ERROR("no found valid sparse header.\n");
        return HI_NULL_PTR;
    }

    header = (sparse_header_t*)pucDataBuff;

    pstSparseCtl = COM_MemMalloc(sizeof(sparse_chunk_ctl_t));
    if (HI_NULL_PTR == pstSparseCtl)
    {
        COM_ERROR("COM_MemMalloc failed.\n");
        return HI_NULL_PTR;
    }

    COM_MemCpy(&pstSparseCtl->sparseHeader, header, header->file_hdr_sz);
    pstSparseCtl->bActived = HI_FALSE;
    pstSparseCtl->u32ChunkIndex = (HI_U32) - 1;
    pstSparseCtl->u32DataOffset = 0;
    pstSparseCtl->u32HeaderOffset = 0;

    return pstSparseCtl;
}

/*
 * check chunk whether valid
 */
static HI_S32 LOADER_BURN_Verify_chunk(sparse_chunk_ctl_t* pstSparseCtl )
{
    HI_S32 s32Ret = HI_FAILURE;

    if ( (pstSparseCtl->chunkHeader.total_sz != pstSparseCtl->sparseHeader.chunk_hdr_sz + pstSparseCtl->chunkHeader.chunk_sz * pstSparseCtl->sparseHeader.blk_sz)  &&
         (pstSparseCtl->chunkHeader.total_sz != pstSparseCtl->sparseHeader.chunk_hdr_sz))
    {
        COM_ERROR("found invalid chunk:index(%d), chunk data length(0x%x bytes), chunk total length(0x%x bytes) \n.",
                  pstSparseCtl->u32ChunkIndex,
                  pstSparseCtl->chunkHeader.chunk_sz * pstSparseCtl->sparseHeader.blk_sz,
                  pstSparseCtl->chunkHeader.total_sz);

        s32Ret = COM_ERR_FLASH_WRITE;
        goto err;
    }

    s32Ret = HI_SUCCESS;

err:

    return s32Ret;
}


/*
 * u64Address maybe not block aligned .
 */
static HI_S32 HI_Flash_Write_Wrapper(HI_HANDLE hFlash, HI_U64 u64Address, HI_U8* pBuf, HI_U32 u32Len, HI_U32 u32Flags, HI_U32 u32blkSZ)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U64 u64AlignedWriteOffset = u64Address & ( ~((HI_U64)u32blkSZ - 1) );

    /* write offset non block aligned */
    if (u64Address != u64AlignedWriteOffset)
    {
        HI_U8* pucBuf = HI_NULL_PTR;
        HI_U32 u32BlockInDataLen = u32Len > (u32blkSZ - (u64Address - u64AlignedWriteOffset)) ?
                                   u32blkSZ - (u64Address - u64AlignedWriteOffset) :  u32Len;

        pucBuf = COM_MemMalloc(u32blkSZ);
        if (HI_NULL_PTR == pucBuf)
        {
            COM_ERROR("COM_MemMalloc failed.\n");
            return COM_ERR_MEM;
        }

        COM_DEBUG("write non block aligned raw data, address(0x%llx), len(0x%x), alignedAddress(0x%llx), blockindatalen(0x%x).\n", u64Address, u32Len,
                  u64AlignedWriteOffset, u32BlockInDataLen);

        s32Ret = HI_Flash_Read(hFlash, u64AlignedWriteOffset, pucBuf, u32blkSZ, u32Flags);
        if (0 >= s32Ret)
        {
            COM_ERROR("read block data failed(%d).\n", s32Ret);
            COM_MemFreeSetNull(pucBuf);
            return COM_ERR_FLASH_WRITE;
        }

        COM_MemCpy(pucBuf + (u64Address - u64AlignedWriteOffset), pBuf, u32BlockInDataLen);

        s32Ret = HI_Flash_Write(hFlash, u64AlignedWriteOffset, pucBuf, u32blkSZ, u32Flags);
        if (0 >= s32Ret)
        {
            COM_ERROR("write block data failed(%d).\n", s32Ret);
            COM_MemFreeSetNull(pucBuf);
            return COM_ERR_FLASH_WRITE;
        }

        u64Address += u32BlockInDataLen;
        pBuf +=  u32BlockInDataLen;
        u32Len -= u32BlockInDataLen;

        COM_MemFreeSetNull(pucBuf);
    }

    /* write left data */
    if (0 < u32Len)
    {
        COM_DEBUG("write block aligned raw data, aligned address(0x%llx), len(0x%x).\n", u64Address, u32Len);

        s32Ret = HI_Flash_Write(hFlash, u64Address , pBuf, u32Len, u32Flags);
        if (0 >= s32Ret)
        {
            COM_ERROR("write data failed(%d).\n", s32Ret);
            return  COM_ERR_FLASH_WRITE;
        }
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      LOADER_BURN_WriteData_ext4sp
* Description:   update data to flash
* Data Accessed:
* Data Updated:
* Input:
* Output:
* Return: HI_SUCCESS
* Others:
*****************************************************************************/
HI_S32 LOADER_BURN_WriteData_ext4sp(FLASH_DATA_INFO_S* pstFlashDataInfo, HI_U8* pucSrcDataBuff, HI_U32 u32SrcDataLen, HI_VOID* private)
{
    HI_U8* pucDataBuff = HI_NULL_PTR;
    HI_U32  u32DataLen = 0;
    sparse_chunk_ctl_t* pstSparseCtl = HI_NULL_PTR;
    HI_S32 s32Ret = COM_ERR_FLASH_WRITE;

    COM_CHECK_PARAM(HI_NULL_PTR == pucSrcDataBuff);
    COM_CHECK_PARAM(HI_NULL_PTR == pstFlashDataInfo);

    if (u32SrcDataLen > pstFlashDataInfo->u64PartitionSize)
    {
        COM_ERROR("burn size has overflow the partiton size,burn size is %#x, partition size is %#x\n",
                  u32SrcDataLen, pstFlashDataInfo->u64PartitionSize);
        return COM_ERR_FLASH_WRITE;
    }

    pucDataBuff = pucSrcDataBuff;
    u32DataLen = u32SrcDataLen;
    pstSparseCtl = (sparse_chunk_ctl_t*)pstFlashDataInfo->private;

    /* new sparse chunk ctl */
    if (HI_NULL_PTR == pstSparseCtl)
    {
        pstSparseCtl = LOADER_BURN_New_sparseCtl(pucDataBuff, u32DataLen);

        if (HI_NULL_PTR == pstSparseCtl)
        {
            return COM_ERR_FLASH_WRITE;
        }

        pucDataBuff += pstSparseCtl->sparseHeader.file_hdr_sz;
        u32DataLen -= pstSparseCtl->sparseHeader.file_hdr_sz;

        pstFlashDataInfo->private = pstSparseCtl;

        COM_DEBUG("found ext4 sparse header(blk_sz:%d bytes, total_blks:%d, total_chunk:%d).\n", pstSparseCtl->sparseHeader.blk_sz,
                  pstSparseCtl->sparseHeader.total_blks, pstSparseCtl->sparseHeader.total_chunks);

    }

search_chunk:

    /*
    * search chunk, maybe incomplete.
    * we assume that it can read chunk header in two suitation:
    * 1/read entire header at first time.
    * 2/read partial header at first time, read left data at second time.
    */
    if (HI_TRUE != pstSparseCtl->bActived)
    {
        /* save chunk header */
        if (u32DataLen >= pstSparseCtl->sparseHeader.chunk_hdr_sz)
        {
            COM_MemCpy(&pstSparseCtl->chunkHeader + pstSparseCtl->u32HeaderOffset , pucDataBuff,
                       pstSparseCtl->sparseHeader.chunk_hdr_sz - pstSparseCtl->u32HeaderOffset);

            /* verify chunk valid */
            if (HI_SUCCESS != LOADER_BURN_Verify_chunk(pstSparseCtl))
            {
                return COM_ERR_FLASH_WRITE;
            }

            pstSparseCtl->u32HeaderOffset = 0;
            pstSparseCtl->u32DataOffset = 0;
            pstSparseCtl->u32ChunkIndex ++;
            pstSparseCtl->bActived = HI_TRUE;

            COM_DEBUG("found complete chunk header:index(%d), valid_header_offset(%d), chunk data length(0x%x bytes). chunk total length(0x%x bytes) \n",
                      pstSparseCtl->u32ChunkIndex,
                      pstSparseCtl->u32HeaderOffset,
                      pstSparseCtl->chunkHeader.chunk_sz * pstSparseCtl->sparseHeader.blk_sz,
                      pstSparseCtl->chunkHeader.total_sz);

            pucDataBuff += pstSparseCtl->sparseHeader.chunk_hdr_sz - pstSparseCtl->u32HeaderOffset;
            u32DataLen -= pstSparseCtl->sparseHeader.chunk_hdr_sz - pstSparseCtl->u32HeaderOffset;
        }
        else
        {
			return HI_SUCCESS;
        }
    }

    /* traverse each chunk */
    if (HI_TRUE == pstSparseCtl->bActived)
    {
        if (pstSparseCtl->u32ChunkIndex < pstSparseCtl->sparseHeader.total_chunks)
        {
            /* RAW */
            switch (pstSparseCtl->chunkHeader.chunk_type)
            {
                case CHUNK_TYPE_RAW:
                {
                    HI_U32 u32ChunkDataLen = pstSparseCtl->chunkHeader.chunk_sz * pstSparseCtl->sparseHeader.blk_sz - pstSparseCtl->u32DataOffset < u32DataLen ?
                                             pstSparseCtl->chunkHeader.chunk_sz * pstSparseCtl->sparseHeader.blk_sz - pstSparseCtl->u32DataOffset : u32DataLen;

                    s32Ret = HI_Flash_Write_Wrapper(pstFlashDataInfo->hFlashHandle,
                                                    pstFlashDataInfo->u64WriteOffset,
                                                    pucDataBuff, u32ChunkDataLen,  pstFlashDataInfo->u32WriteFlags,
                                                    pstSparseCtl->sparseHeader.blk_sz);
                    if (HI_SUCCESS != s32Ret)
                    {
                        COM_ERROR("write data failed(%d).\n", s32Ret);
                        return COM_ERR_FLASH_WRITE;
                    }

                    pstSparseCtl->u32DataOffset += u32ChunkDataLen;
                    pstFlashDataInfo->u64WriteOffset += u32ChunkDataLen;

                    /* try search next chunk if it still has data in buff */
                    if (u32ChunkDataLen < u32DataLen)
                    {
                        pucDataBuff += u32ChunkDataLen;
                        u32DataLen -= u32ChunkDataLen;

                        /* find next chunk */
                        pstSparseCtl->bActived = HI_FALSE;

                        goto search_chunk;
                    }

                    break;
                }

                case CHUNK_TYPE_DONT_CARE:
                {
                    HI_U32 u32ChunkDataLen = pstSparseCtl->chunkHeader.chunk_sz * pstSparseCtl->sparseHeader.blk_sz;

                    COM_DEBUG("write null data offset:0x%llx, data_len:0x%x.\n", pstFlashDataInfo->u64WriteOffset, u32ChunkDataLen);

                    pstFlashDataInfo->u64WriteOffset += u32ChunkDataLen;

                    /* find next chunk */
                    pstSparseCtl->bActived = HI_FALSE;

                    goto search_chunk;
                }

                default:
                    COM_ERROR("invalid sparse chunk type.\n");
                    return COM_ERR_FLASH_WRITE;
            }
        }
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

