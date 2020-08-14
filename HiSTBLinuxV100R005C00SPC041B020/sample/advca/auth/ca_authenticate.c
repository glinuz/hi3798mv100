#include <stdio.h>
#include <stdlib.h>
#include <cutils/log.h>

#include "alg.h"
#include "authdefine.h"

#include "ca_flash.h"
#include "hi_common.h"

#define BOOT_PARTITION      "fastboot"
#define SPARSE_EXT4         1 /* 1: sparse ext4, 0: non-spares ext4 */

#ifdef SPARSE_EXT4
#include "sparse.h"
#define SYSTEM_SIGN_PARTITION  "systemsign"
HI_U32 IsSparseFlag = 0;  /* 0->not init, 1->sparse, 2->not sparse */
HI_U8 *pSignbuffer = HI_NULL;
HI_U32 *pSignChunkInfo = HI_NULL;
sparse_header_t *pSparseHead = HI_NULL;
sign_chunk_header_t *pChunkHead = HI_NULL;
#endif 

static rsa_context g_customer_key;

static HI_S32 CA_common_getExternRsaKey(rsa_context *rsa)
{
    HI_S32 i, ret = 0;
	HI_HANDLE handle = INVALID_FD;
    HI_S8 buffer[RSA_2048_LEN * 2] = {0};
    HI_SYS_VERSION_S enChipVersion;
    HI_Flash_InterInfo_S enFlashInfo;

    memset(&enChipVersion, 0, sizeof(HI_SYS_VERSION_S));
    memset(&enFlashInfo, 0, sizeof(HI_Flash_InterInfo_S));

    ret = HI_SYS_GetVersion(&enChipVersion);
    if (HI_FAILURE == ret)
    {
        SAMPLE_PRINTF("Can't get chip info!\n");
        return HI_FAILURE;
    }
    
    handle = HI_Flash_OpenByName(BOOT_PARTITION);
    if (HI_INVALID_HANDLE == handle)
    {
        SAMPLE_PRINTF("Can't open fastboot partition\n");
        return HI_FAILURE;
    }
    ret = HI_Flash_GetInfo(handle, &enFlashInfo);
    if (HI_FAILURE == ret)
    {
        SAMPLE_PRINTF("Can't get chip info!\n");
        HI_Flash_Close(handle);
        return HI_FAILURE;
    }
    
    if (HI_FLASH_TYPE_EMMC_0 == enFlashInfo.FlashType)
    {
        if ((HI_CHIP_TYPE_HI3719M == enChipVersion.enChipTypeHardWare) && (HI_CHIP_VERSION_V100 == enChipVersion.enChipVersion))
        {
            ret = HI_Flash_Read(handle, 0, (HI_U8 *)buffer, 512, HI_FLASH_RW_FLAG_RAW);
        }
        else
        {
            ret = HI_Flash_Read(handle, 512, (HI_U8 *)buffer, 512, HI_FLASH_RW_FLAG_RAW);
        }
    }
    else
    {
        ret = HI_Flash_Read(handle, 0, (HI_U8 *)buffer, 512, HI_FLASH_RW_FLAG_RAW);
    }
    
    if (ret == HI_FAILURE || ret == HI_FLASH_END_DUETO_BADBLOCK)
    {
        SAMPLE_PRINTF("Read fastboot partition failed\n");
        HI_Flash_Close(handle);
        return HI_FAILURE;
    }

    HI_Flash_Close(handle);
    
    rsa_init(rsa, RSA_PKCS_V15, 0);    /* get rsa N and E */  
    CA_ASSERT(mpi_read_binary((mpi *)&rsa->N, (const unsigned char *)buffer, RSA_2048_LEN), ret);
    CA_ASSERT(mpi_read_binary((mpi *)&rsa->E, (const unsigned char *)(buffer+ RSA_2048_LEN), RSA_2048_LEN), ret);
    rsa->len = (mpi_msb(&rsa->N) + 7 ) >> 3;     

    return HI_SUCCESS;
}

static HI_S32 CA_common_Search_SignHeader(HI_CHAR *PartitionSignName, HI_CASignImageTail_S *pSignImageInfo, HI_U32 *pOffset)
{
    HI_U32 index = 0;
    HI_U32 PartitionSize = 0;
    HI_U32 offset = 0;
    HI_S32 ret = 0;
    HI_S32 ReadSize = CAIMG_HEAD_BLOCK_SIZE;
    HI_U8 CAImageArea[CAIMG_HEAD_BLOCK_SIZE] = {0}; 

    offset = *pOffset;
    
    SAMPLE_PRINTF("PartitionSignName:%s\n", PartitionSignName);
    CA_ASSERT(CA_flash_GetPartitionSize(PartitionSignName, &PartitionSize), ret);
    
    for (index = offset / ReadSize; index < PartitionSize / ReadSize; index++)
    {
        CA_ASSERT(CA_flash_read(PartitionSignName, ReadSize * index, ReadSize, (HI_U8 *)CAImageArea, HI_NULL), ret);
        memcpy(pSignImageInfo, CAImageArea, sizeof(HI_CASignImageTail_S));
        if (!memcmp(pSignImageInfo->u8MagicNumber, CAIMGHEAD_MAGICNUMBER, sizeof(pSignImageInfo->u8MagicNumber)))
        {
            HI_U32 CRC32Value;
            SAMPLE_PRINTF("Sign Header found!\n");
            //Clacluate CRC32!
            CRC32Value = CRC32((HI_U8 *)pSignImageInfo, sizeof(HI_CASignImageTail_S) - 4);
            if (CRC32Value == pSignImageInfo->CRC32)
            {
                *pOffset = ReadSize * index;
                SAMPLE_PRINTF("CRC32 check ok\n");
            }
            else
            {
                SAMPLE_PRINTF("CRC32Value:%x != pSignImageInfo->CRC32:%x\n", CRC32Value, pSignImageInfo->CRC32);
                return HI_FAILURE;
            }
#ifdef SPARSE_EXT4
            if (0 == IsSparseFlag && !memcmp(PartitionSignName, SYSTEM_SIGN_PARTITION, strlen(SYSTEM_SIGN_PARTITION)))
            {
                pSignbuffer = (HI_U8 *)malloc(PartitionSize - ReadSize * index);
                CA_CheckPointer(pSignbuffer);
                CA_ASSERT(CA_flash_read(PartitionSignName, ReadSize * index, PartitionSize - ReadSize * index, pSignbuffer, HI_NULL), ret);
                pSparseHead = (sparse_header_t *)(pSignbuffer + pSignImageInfo->u32TotalsectionID * ReadSize);
                if (IS_SPARSE(pSparseHead))  /* sparse ext4 */
                {
                    IsSparseFlag = 1;
                    pChunkHead = (sign_chunk_header_t *)(pSignbuffer + pSignImageInfo->u32TotalsectionID * ReadSize + pSparseHead->file_hdr_sz);
                    pSignChunkInfo = (HI_U32 *)(pSignbuffer + pSignImageInfo->u32TotalsectionID * ReadSize + pSparseHead->file_hdr_sz + pSparseHead->chunk_hdr_sz * pSparseHead->total_chunks);
                }
                else  /* not sparse ext4 */
                {
                    IsSparseFlag = 2;
                    free(pSignbuffer);
                }
            }
#endif
            return HI_SUCCESS;
        }
        else
        {
            continue;
        }
    }
    return HI_FAILURE;
}

static HI_VOID CA_common_PrintHeadValue(HI_CASignImageTail_S *CAimgHead)
{
    HI_U32 index = 0;
    
    //SAMPLE_PRINTF("u8Version:%s\n", CAimgHead->u8Version);
    SAMPLE_PRINTF("u32CreateDay:0x%x\n", CAimgHead->u32CreateDay);
    SAMPLE_PRINTF("u32CreateTime:0x%x\n", CAimgHead->u32CreateTime);
    SAMPLE_PRINTF("u32HeadLength:0x%x\n", CAimgHead->u32HeadLength);
    SAMPLE_PRINTF("u32SignedDataLength:%d:0x%x\n", CAimgHead->u32SignedDataLength, CAimgHead->u32SignedDataLength);
    SAMPLE_PRINTF("u32IsYaffsImage:%d\n", CAimgHead->u32IsYaffsImage);
    SAMPLE_PRINTF("u32IsConfigNandBlock:%d\n", CAimgHead->u32IsConfigNandBlock);
    SAMPLE_PRINTF("u32NandBlockType:%d\n", CAimgHead->u32NandBlockType);
    SAMPLE_PRINTF("u32IsNeedEncrypt:%d\n", CAimgHead->u32IsNeedEncrypt);
    SAMPLE_PRINTF("u32IsEncrypted:%d\n", CAimgHead->u32IsEncrypted);
    SAMPLE_PRINTF("u32HashType:%d\n", CAimgHead->u32HashType);
    SAMPLE_PRINTF("u32SignatureLen:%d\n", CAimgHead->u32SignatureLen);
    
    for(index = 0; index < HASH256_LEN; index ++)
    {   
        SAMPLE_PRINTF("%02x ", CAimgHead->u8Sha[index]); 
    } 
    for(index = 0; index < CAimgHead->u32SignatureLen; index ++)   
    {
        SAMPLE_PRINTF("%02x ", CAimgHead->u8Signature[index]);   
    }
    
    //SAMPLE_PRINTF("OrignalImagePath:%s\n", CAimgHead->OrignalImagePath);  
    //SAMPLE_PRINTF("RSAPrivateKeyPath:%s\n", CAimgHead->RSAPrivateKeyPath);
    SAMPLE_PRINTF("u32CurrentsectionID:%d\n", CAimgHead->u32CurrentsectionID);
    SAMPLE_PRINTF("u32SectionSize:%d\n", CAimgHead->u32SectionSize);
    SAMPLE_PRINTF("u32TotalsectionID:%d\n", CAimgHead->u32TotalsectionID);
    SAMPLE_PRINTF("CRC32:%x\n", CAimgHead->CRC32);
}

static HI_S32 CA_common_GetImageData(HI_CHAR *PartitionImageName, HI_CASignImageTail_S *pSignImageInfo, HI_CA_NEW_SignParam_S *pSignParam)
{
    HI_S32 ret = 0;
    HI_U32 index = 0;
    HI_S32 PartitionImagSize = 0;
    HI_U64 PartitionImageOffset = 0;
    HI_U8 *pCAImageArea = HI_NULL;
    HI_U8 TmpSha[32];
#ifdef SPARSE_EXT4
    HI_U32 u32CurChunkNum = 0;
    HI_U32 u32NextChunkNum = 0;
    HI_U32 u32ChunkNum = 0;
    HI_U32 u32MergeDataLen = 0;
    HI_U32 u32MergeDataOffset = 0;
    sign_chunk_header_t * tmpChunkHead = HI_NULL;
#endif
    
    SAMPLE_PRINTF("PartitionImageName:%s, pSignImageInfo->u32SignedDataLength:%d\n", PartitionImageName, pSignImageInfo->u32SignedDataLength);
    PartitionImagSize = (pSignImageInfo->u32SignedDataLength + CAIMG_HEAD_BLOCK_SIZE - 1) / CAIMG_HEAD_BLOCK_SIZE * CAIMG_HEAD_BLOCK_SIZE;
    PartitionImageOffset = pSignImageInfo->u32CurrentsectionID * pSignImageInfo->u32SectionSize;

    pCAImageArea = (HI_U8 *)malloc(pSignImageInfo->u32SignedDataLength);
    if (NULL == pCAImageArea)
    {
        SAMPLE_PRINTF("can not allocate memory\n");
        return HI_FAILURE;
    }
    memset(pCAImageArea, 0x0, pSignImageInfo->u32SignedDataLength);
    if (pSignImageInfo->u32IsYaffsImage)
    {
        //SAMPLE_PRINTF("Yaffs FileSystem, We will check sign of the whole yaffs-image:%s\n", PartitionImageName);
        //CA_ASSERT(CA_GetYAFFSFlashData((HI_U8*)PartitionImageName, pCAImageArea, pSignImageInfo->u32SignedDataLength),ret);
        SAMPLE_PRINTF("Is Yaffs Image!!\n");
        free(pCAImageArea);
        return HI_FAILURE;
    }
    else
    {
        CA_ASSERT(CA_flash_read(PartitionImageName, PartitionImageOffset, PartitionImagSize,(HI_U8*)pCAImageArea, HI_NULL), ret);
    }

#ifdef SPARSE_EXT4
    if (1 == IsSparseFlag)
    {
        /* Get authenticate data begin in which sparse chunk */
        u32CurChunkNum = *(pSignChunkInfo + pSignImageInfo->u32CurrentsectionID);
        u32NextChunkNum = *(pSignChunkInfo + pSignImageInfo->u32CurrentsectionID + 1);
        u32ChunkNum = u32NextChunkNum - u32CurChunkNum;
        
        tmpChunkHead = (sign_chunk_header_t *)(pChunkHead + u32CurChunkNum);
        if (tmpChunkHead->num != u32CurChunkNum)
        {
            SAMPLE_PRINTF("Get sparse ext4 info error!\n");
            free(pCAImageArea);
            return HI_FAILURE;
        }
        
        /* set the data of CHUNK_TYPE_DONT_CARE to zero */
        if (0 == u32ChunkNum)
        {
            if (tmpChunkHead->chunk_type == CHUNK_TYPE_DONT_CARE)
            {
                memset(pCAImageArea, 0, pSignImageInfo->u32SignedDataLength);
            }
        }
        else
        {
            if (tmpChunkHead->chunk_type == CHUNK_TYPE_DONT_CARE)
                u32MergeDataOffset = 0;
            else
                u32MergeDataOffset = tmpChunkHead->offset + tmpChunkHead->chunk_sz * pSparseHead->blk_sz - pSignImageInfo->u32CurrentsectionID * pSignImageInfo->u32SignedDataLength;
            u32MergeDataLen = 0;
            for (index = 0; index <= u32ChunkNum; index++)
            {
                if (tmpChunkHead->chunk_type == CHUNK_TYPE_DONT_CARE)
                {
                    if (0 == index)
                        u32MergeDataLen = tmpChunkHead->offset + tmpChunkHead->chunk_sz * pSparseHead->blk_sz - pSignImageInfo->u32CurrentsectionID * pSignImageInfo->u32SignedDataLength;
                    else if (u32ChunkNum == index)    
                        u32MergeDataLen = pSignImageInfo->u32SignedDataLength - u32MergeDataOffset;
                    else
                        u32MergeDataLen = tmpChunkHead->offset + tmpChunkHead->chunk_sz * pSparseHead->blk_sz - pSignImageInfo->u32CurrentsectionID * pSignImageInfo->u32SignedDataLength - u32MergeDataOffset;
                    memset((HI_U8 *)(pCAImageArea + u32MergeDataOffset), 0, u32MergeDataLen);
                    u32MergeDataOffset += u32MergeDataLen;
                }
                else
                {
                    if (0 != index)
                        u32MergeDataOffset +=  tmpChunkHead->chunk_sz * pSparseHead->blk_sz;
                }
                tmpChunkHead++;
            }
        }
    }
#endif

    if(pSignImageInfo->u32IsEncrypted == HI_TRUE)
    {
        SAMPLE_PRINTF("Need to Decrypt image \n");        
        //CA_DataDecrypt(pCAImageArea, PartitionImagSize, pCAImageArea);
        return HI_SUCCESS;
    }
    
    //We can check SHA value!
    if(pSignImageInfo->u32HashType == AUTH_SHA1)
    {
        sha1((const unsigned char *)pCAImageArea, pSignImageInfo->u32SignedDataLength, TmpSha);
        for(index = 0; index < 5; index ++)
        {
            if(TmpSha[index] != pSignImageInfo->u8Sha[index])
            {
                SAMPLE_PRINTF("error index:%d sha:0x%2x != 0x%2x\n",index, TmpSha[index], pSignImageInfo->u8Sha[index]);
                return HI_FAILURE;
            }
        }
    }
    else
    {
        SAMPLE_PRINTF("begin sha2, length:%d\n", pSignImageInfo->u32SignedDataLength);
        sha2((HI_U8 *)pCAImageArea, pSignImageInfo->u32SignedDataLength, TmpSha, 0);
        for(index = 0; index < 32; index ++)
        {
            if(TmpSha[index] != pSignImageInfo->u8Sha[index])
            {
                SAMPLE_PRINTF("error index:%d sha:0x%2x != 0x%2x\n", index, TmpSha[index], pSignImageInfo->u8Sha[index]);
                return HI_FAILURE;
            }
        }
    }
    SAMPLE_PRINTF("Sha check ok\n");
    
    //Now, We got clean image data!
    pSignParam->pImageData = pCAImageArea;
    pSignParam->ImageDataLength = pSignImageInfo->u32SignedDataLength;
    pSignParam->pCASignTail = pSignImageInfo;

    return HI_SUCCESS;
}



static HI_S32 auth(rsa_context *rsa, AUTHALG_SHA_E hash_id, const unsigned char *hash, unsigned char *sign)
{
    HI_S32 ret;
   
    if (AUTH_SHA2 == hash_id)
    {
        ret = rsa_pkcs1_verify(rsa, RSA_PUBLIC, SIG_RSA_SHA256, 32, hash, sign);  
    }
    else
    {
        ret = rsa_pkcs1_verify(rsa, RSA_PUBLIC, SIG_RSA_SHA1, 20, hash, sign);
    }

    if (0 == ret)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}


static HI_U32 CA_common_DataAuthenticate(HI_U32 StartAddr, HI_U32 length, HI_CASignImageTail_S *pCASignTail )    
{
    HI_S32 ret;
    HI_U32 index = 0;
   
    HI_U8 hash[32] = {0}; 

    if (AUTH_SHA2 == pCASignTail->u32HashType)
    {
        sha2((const unsigned char *)StartAddr, length, hash, 0);
        for(index = 0; index < 32; index ++)
        {
            if(hash[index] != pCASignTail->u8Sha[index])
            {
                SAMPLE_PRINTF("error index:%d sha:0x%2x != 0x%2x\n", index, hash[index], pCASignTail->u8Sha[index]);
                return HI_FAILURE;
            }
        }
    }
    else
    {
        sha1((const unsigned char *)StartAddr, length, hash);
        for(index = 0; index < 5; index ++)
        {
            if (hash[index] != pCASignTail->u8Sha[index])
            {
                SAMPLE_PRINTF("error index:%d sha:0x%2x != 0x%2x\n",index, hash[index], pCASignTail->u8Sha[index]);
                return HI_FAILURE;
            }
        }
    }
    
    return auth(&g_customer_key, pCASignTail->u32HashType, (const unsigned char *)hash, pCASignTail->u8Signature);
}

HI_S32 CA_common_authPartition(HI_CHAR *verifyPartition, HI_CHAR *signPartition, HI_U32 offset)
{
    HI_S32 ret = 0;
    HI_U32 index = 0;
    HI_U32 u32TotalSection = 0;
    HI_CASignImageTail_S SignImageInfo;
    HI_CA_NEW_SignParam_S SignParam = {0};
    
    //Verify Partition
    CA_CheckPointer(verifyPartition);
    CA_CheckPointer(signPartition);

    CA_ASSERT(CA_common_getExternRsaKey(&g_customer_key), ret);

    CA_ASSERT(CA_common_Search_SignHeader(signPartition, &SignImageInfo, &offset), ret);
    CA_common_PrintHeadValue(&SignImageInfo);
    
    u32TotalSection = SignImageInfo.u32TotalsectionID;
    if (u32TotalSection >= 1)
    {
        for (index = 0; index < u32TotalSection; index++)
        {
            CA_ASSERT(CA_common_Search_SignHeader(signPartition, &SignImageInfo, &offset), ret);
            CA_common_PrintHeadValue(&SignImageInfo);
            //Get image data
            CA_ASSERT(CA_common_GetImageData(verifyPartition, &SignImageInfo, &SignParam), ret);
            //Verify image signature
            CA_ASSERT(CA_common_DataAuthenticate((HI_U32)(SignParam.pImageData), SignParam.ImageDataLength, SignParam.pCASignTail), ret);
            free(SignParam.pImageData);
            offset += CAIMG_HEAD_BLOCK_SIZE;
            sleep(1);
        }
#ifdef SPARSE_EXT4
        if (1 == IsSparseFlag)
        {
            free(pSignbuffer);
        }
#endif
    }
    else
    {
        SAMPLE_PRINTF("Err u32TotalsectionID = %d\n", u32TotalSection);
    }

    return HI_SUCCESS;

}
