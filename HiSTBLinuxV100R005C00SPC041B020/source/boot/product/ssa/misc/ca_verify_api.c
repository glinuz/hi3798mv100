/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : ca_verify_api.c
Version       : Initial
Author        : Hisilicon hisecurity team
Created       : 2016-01-15
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "hi_type.h"
#ifdef HI_MINIBOOT_SUPPORT
#include "run.h"
#include "config.h"
#include "platform.h"
#include "app.h"
#else
#include "exports.h"
#endif
#include "hi_common.h"
#include "ca_debug.h"
#include "ca_flash.h"
#include "ca_adp.h"
#include "ca_verify_api.h"


#define MAX_STRLEN(a, b) ((strlen(a) > strlen(b)) ? strlen(a) : strlen(b))
#define CFG_TIMERBASE23         REG_BASE_TIMER23
#define CFG_TIMER2_CTRL         0xC2
#define READ_TIMER2         \
     (*(volatile unsigned long *)(CFG_TIMERBASE23 + REG_TIMER_VALUE))


#define SPARSE_EXT4

#ifdef SPARSE_EXT4
#include "ca_sparse.h"
#define SYSTEM_SIGN_PARTITION  "systemsign"
#define SYSTEM_IMG_PARTITION  "system"

HI_U32 IsSparseFlag = 0xFF;  /* 0->init, 1->sparse, 2->not sparse */
HI_U32 *pSignChunkInfo = HI_NULL;
sparse_header_t *pSparseHead = HI_NULL;
sign_chunk_header_t *pChunkHead = HI_NULL;
#endif

static unsigned long rstate = 1;

static HI_U8 CAIMGHEAD_MAGICNUMBER[32]  =  "Hisilicon_ADVCA_ImgHead_MagicNum";

//extern HI_U32 g_DDRSize ; /*At preset, the value is configured to the fixed size:256M. We must optimize it later*/


//extern void sha1( const unsigned char *input, int ilen, unsigned char output[20] );
extern unsigned long DVB_CRC32(unsigned char *buf, int length);


static HI_VOID printf_hex(HI_U8 *pu8Buf, HI_U32 u32Length)
{
    HI_U32 i;

    for(i = 0; i < u32Length; i++)
    {
        if(i % 16 == 0 && i != 0)
        {
            HI_SIMPLEINFO_CA("\n");
        }
        HI_SIMPLEINFO_CA("0x%02X, ", pu8Buf[i]);
    }
    HI_SIMPLEINFO_CA("\n");

    return;
}


/* trivial congruential random generators. */
static void ca_srandom(unsigned long seed)
{
    rstate = seed ? seed : 1;  /* dont allow a 0 seed */
}

static unsigned long ca_random(void)
{
    unsigned int next = rstate;
    int result;

    next *= 1103515245;
    next += 12345;
    result = (unsigned int) (next / 65536) % 2048;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int) (next / 65536) % 1024;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int) (next / 65536) % 1024;

    rstate = next;

    return result;
}



HI_S32 Debug_CASignImageTail_Value(HI_CASignImageTail_S *pCASignTail)
{
    unsigned char Data[256];

    memset(Data, 0, 256);
    memcpy(Data, pCASignTail->u8MagicNumber, 32);
    HI_SIMPLEINFO_CA("\nu8MagicNumber:%s\n", Data);
    memset(Data, 0, 256);
    memcpy(Data, pCASignTail->u8Version, 8);

    HI_SIMPLEINFO_CA("u8Version:%s\n", Data);
    HI_SIMPLEINFO_CA("u32CreateDay:0x%x\n", pCASignTail->u32CreateDay);
    HI_SIMPLEINFO_CA("u32CreateTime:0x%x\n", pCASignTail->u32CreateTime);
    HI_SIMPLEINFO_CA("u32HeadLength:0x%x\n", pCASignTail->u32HeadLength);
    HI_SIMPLEINFO_CA("u32SignedDataLength:%d:0x%x\n", pCASignTail->u32SignedDataLength, pCASignTail->u32SignedDataLength);
    HI_SIMPLEINFO_CA("u32IsYaffsImage:%d\n", pCASignTail->u32IsYaffsImage);
    HI_SIMPLEINFO_CA("u32IsConfigNandBlock:%d\n", pCASignTail->u32IsConfigNandBlock);
    HI_SIMPLEINFO_CA("u32NandBlockType:%d\n", pCASignTail->u32NandBlockType);
    HI_SIMPLEINFO_CA("u32IsNeedEncrypt:%d\n", pCASignTail->u32IsNeedEncrypt);
    HI_SIMPLEINFO_CA("u32IsEncrypted:%d\n", pCASignTail->u32IsEncrypted);
    HI_SIMPLEINFO_CA("u32HashType:%d\n", pCASignTail->u32HashType);
    HI_SIMPLEINFO_CA("u32CurrentsectionID:%d\n", pCASignTail->u32CurrentsectionID);
    HI_SIMPLEINFO_CA("u32TotalsectionID:%d\n", pCASignTail->u32TotalsectionID);
    HI_SIMPLEINFO_CA("u32SectionSize:%d\n", pCASignTail->u32SectionSize);
    HI_SIMPLEINFO_CA("u8Sha:\n");
    if(pCASignTail->u32HashType == Algoritm_Hash2)
    {
        printf_hex(pCASignTail->u8Sha, 32);
    }
    else
    {
        printf_hex(pCASignTail->u8Sha, 20);
    }
    HI_SIMPLEINFO_CA("u32SignatureLen:%d\n", pCASignTail->u32SignatureLen);
    HI_SIMPLEINFO_CA("u8Signature:\n");
    printf_hex(pCASignTail->u8Signature, pCASignTail->u32SignatureLen);

    HI_SIMPLEINFO_CA("OrignalImagePath:%s\n", pCASignTail->OrignalImagePath);
    HI_SIMPLEINFO_CA("RSAPrivateKeyPath:%s\n", pCASignTail->RSAPrivateKeyPath);
    HI_SIMPLEINFO_CA("CRC32:%x\n", pCASignTail->CRC32);

    return 0;
}

HI_S32 CA_Search_SignHeader(HI_CHAR *PartitionSignName, HI_CASignImageTail_S *pSignImageInfo, HI_U32 *pOffset)
{
    HI_S32 ret = 0, index = 0, i = 0;
    HI_U32 PartitionSize = 0;
    HI_U8 *CAImageArea = HI_NULL;
    HI_U32 offset;
    HI_U32 PageSize = 0;

    CA_CheckPointer(pOffset);
    offset = *pOffset;

    CA_CheckPointer(PartitionSignName);
    HI_INFO_CA("PartitionSignName:%s\n", PartitionSignName);
    CA_ASSERT(CA_flash_GetSize((HI_CHAR *)PartitionSignName, &PartitionSize),ret);
    CA_ASSERT(CA_flash_GetPageSize((HI_CHAR *)PartitionSignName, &PageSize), ret);

    if (PageSize <= NAND_PAGE_SIZE)
    {
        PageSize = NAND_PAGE_SIZE;
		if(NAND_PAGE_SIZE % PageSize != 0)
		{
		    HI_ERR_CA("NAND_PAGE_SIZE(%d) PageSize(%d) != 0\n", NAND_PAGE_SIZE, PageSize);
		    return HI_FAILURE;
		}
    }

    CAImageArea = (HI_U8 *)malloc(PageSize);
    if (NULL == CAImageArea)
    {
        HI_ERR_CA("malloc CAImageArea Fail\n");
        return HI_FAILURE;
    }
    
    for(index = offset / PageSize; index < PartitionSize / PageSize; index ++)
    {
        CA_ASSERT(CA_flash_read((HI_CHAR*)PartitionSignName, PageSize * index, PageSize,(HI_U8*)CAImageArea, HI_NULL),ret);
        for (i = 0; i < PageSize / NAND_PAGE_SIZE; i++)  //PageSize must >= NAND_PAGE_SIZE
        {
            memcpy(pSignImageInfo, CAImageArea + i * NAND_PAGE_SIZE, sizeof(HI_CASignImageTail_S));
            if(!memcmp(pSignImageInfo->u8MagicNumber, CAIMGHEAD_MAGICNUMBER, sizeof(pSignImageInfo->u8MagicNumber)))
            {
                unsigned int CRC32Value;
                HI_INFO_CA("Sign Header found!\n");
                //Clacluate CRC32!
                CRC32Value = DVB_CRC32((HI_U8 *)pSignImageInfo, (sizeof(HI_CASignImageTail_S) - 4));
                if(CRC32Value == pSignImageInfo->CRC32)
                {                    
                    *pOffset = index * PageSize + i * NAND_PAGE_SIZE;
                    HI_INFO_CA("CRC32 check ok\n");
#ifdef SPARSE_EXT4
                    if (0 == IsSparseFlag && !memcmp(PartitionSignName, SYSTEM_SIGN_PARTITION, strlen(SYSTEM_SIGN_PARTITION)))
                    {
                        HI_U32 pTmp = 0;
                        ret = HI_MEM_Alloc(&pTmp, PartitionSize - PageSize * index);
                        if (HI_SUCCESS != ret)
                        {
                            free (CAImageArea);
                            return HI_FAILURE;
                        }
                        CA_CheckPointer((HI_VOID *)pTmp);
                        CA_ASSERT(CA_flash_read((HI_CHAR*)PartitionSignName, PageSize * index, PartitionSize - PageSize * index, (HI_U8 *)pTmp, HI_NULL), ret);
                        pSparseHead = (sparse_header_t *)(pTmp + i * NAND_PAGE_SIZE + pSignImageInfo->u32TotalsectionID * NAND_PAGE_SIZE);
                        if (IS_SPARSE(pSparseHead))  /* sparse ext4 */
                        {
                            IsSparseFlag = 1;
                            pChunkHead = (sign_chunk_header_t *)(pTmp + i * NAND_PAGE_SIZE + pSignImageInfo->u32TotalsectionID * NAND_PAGE_SIZE + pSparseHead->file_hdr_sz);
                            pSignChunkInfo = (HI_U32 *)(pTmp + i * NAND_PAGE_SIZE + pSignImageInfo->u32TotalsectionID * NAND_PAGE_SIZE + pSparseHead->file_hdr_sz + pSparseHead->chunk_hdr_sz * pSparseHead->total_chunks);
                        }
                        else  /* not sparse ext4 */
                        {
                            IsSparseFlag = 2;
                        }
                    }
#endif
    				free (CAImageArea);
                    return HI_SUCCESS;
                }
                else
                {
                    HI_ERR_CA("CRC32Value:%x != pSignImageInfo->CRC32:%x\n", CRC32Value, pSignImageInfo->CRC32);
                    free (CAImageArea);
                    return HI_FAILURE; 
                }
            }
        }
    }
 
    free(CAImageArea);
    return HI_FAILURE;
}

static HI_S32 CA_GetYAFFSFlashData(HI_U8 * PartitionImagenName, HI_U8 *DDRAddress, HI_U32 Length)
{
    HI_S32 ret = 0, index;
    HI_U8 *pCAImageArea;
    HI_U32 Address;
    HI_U32 PartionSize;
    HI_U32 PageSize;
    HI_U32 BlockSize;
    HI_U32 OobSize;

    char argv[5][20];
    char command[256];

    CA_ASSERT(CA_flash_getinfo((HI_CHAR *)PartitionImagenName, &PartionSize, &BlockSize, &PageSize, &OobSize),ret);
    if(Length == 0)
    {
        Length = PartionSize;
    }

    pCAImageArea = (HI_U8 *)DDRAddress;
    if(pCAImageArea == NULL)
    {
        HI_ERR_CA("can not allocate data\n");
        return HI_FAILURE;
    }


    CA_flash_GetFlashAddressFromParitonName(HI_FLASH_TYPE_NAND_0, (HI_CHAR*)PartitionImagenName, &Address);

    memset(argv, 0, 5*20);
    strncpy(argv[0], "nand", sizeof(argv[0]));
    strncpy(argv[1], "read.yaffs", sizeof(argv[1]));
    snprintf(argv[2], sizeof(argv[2]), "%x ", pCAImageArea);
    snprintf(argv[3], sizeof(argv[3]), "%x ", Address);
    snprintf(argv[4], sizeof(argv[4]), "%x ", Length);

    for(index = 0; index < 5; index ++)
    {
        HI_SIMPLEINFO_CA("%s ", argv[index]);
    }

#if 1
    memset(command, 0, 256);
    snprintf(command, sizeof(command), "nand read.yaffs 0x%x 0x%x 0x%x ", pCAImageArea, Address, Length);
    run_command(command, 0);
#else
    do_get_nand_read(&(argv[1]), &(argv[2]), &(argv[3]), &(argv[4]));
#endif

    HI_SIMPLEINFO_CA("Get %s to %x, length:0x%x\n", PartitionImagenName, (int)DDRAddress, Length);

    return ret;
}

HI_S32 CA_GetImageData(HI_U32 u32MemAddr, HI_CHAR *PartitionImageName, HI_CASignImageTail_S *pSignImageInfo, HI_CA_NEW_SignParam_S *pSignParam)
{
    HI_S32 ret = 0, index = 0;
    HI_S32 PartitionImagSize = 0;
    HI_U64 PartitionImageOffset = 0;
    HI_U8 *pCAImageArea = NULL;
    HI_U8 TmpSha[32];
#ifdef SPARSE_EXT4
    HI_U32 u32CurChunkNum = 0;
    HI_U32 u32NextChunkNum = 0;
    HI_U32 u32ChunkNum = 0;
    HI_U32 u32MergeDataLen = 0;
    HI_U32 u32MergeDataOffset = 0;
    sign_chunk_header_t * tmpChunkHead = HI_NULL;
#endif
    HI_CA_CryptoInfo_S stCryptoInfo;
   
    CA_CheckPointer(PartitionImageName);

    HI_INFO_CA("PartitionImageName:%s, pSignImageInfo->u32SignedDataLength:%d\n", PartitionImageName, pSignImageInfo->u32SignedDataLength);
    PartitionImagSize = (pSignImageInfo->u32SignedDataLength + NAND_PAGE_SIZE - 1) / NAND_PAGE_SIZE * NAND_PAGE_SIZE;
    PartitionImageOffset = pSignImageInfo->u32CurrentsectionID * pSignImageInfo->u32SectionSize;

    //pCAImageArea = (HI_U8 *)IMG_VERIFY_ADDRESS;
    pCAImageArea = (HI_U8 *)u32MemAddr;    

    memset(pCAImageArea, 0x00, PartitionImagSize);
    if(pSignImageInfo->u32IsYaffsImage)
    {
        HI_INFO_CA("Yaffs FileSystem, We will check sign of the whole yaffs-image:%s\n", PartitionImageName);
        CA_ASSERT(CA_GetYAFFSFlashData((HI_U8*)PartitionImageName, pCAImageArea, pSignImageInfo->u32SignedDataLength), ret);
    }
    else
    {
        CA_ASSERT(CA_flash_read((HI_CHAR*)PartitionImageName, PartitionImageOffset, PartitionImagSize,(HI_U8*)pCAImageArea, HI_NULL), ret);
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
            HI_ERR_CA("Get sparse ext4 info error!\n");
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
        HI_INFO_CA("Need to Decrypt image \n");
        stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
        stCryptoInfo.bIsUseKLD  = HI_TRUE;
        stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
        stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;    
        HI_CA_ADP_DataDecrypt(&stCryptoInfo, pCAImageArea, PartitionImagSize, pCAImageArea);
    }
    
    //We can check SHA value!
    if(pSignImageInfo->u32HashType == Algoritm_Hash1)
    {
        //dcache_enable(g_DDRSize);
        //sha1((HI_U8 *)pCAImageArea, pSignImageInfo->u32SignedDataLength, TmpSha);
        ret = HI_CA_ADP_CalcHASH((HI_U8 *)pCAImageArea, pSignImageInfo->u32SignedDataLength, TmpSha, Algoritm_Hash1);
        if (ret != HI_SUCCESS)
        {
           HI_ERR_CA("Fail to calc Hash\n");
           return HI_FAILURE;
        }        
        //dcache_disable();
        
        for(index = 0; index < 5; index ++)
        {
            if(TmpSha[index] != pSignImageInfo->u8Sha[index])
            {
                HI_ERR_CA("error index:%d sha:0x%2x != 0x%2x\n", index, TmpSha[index], pSignImageInfo->u8Sha[index]);
                return HI_FAILURE;
            }
        }
    }
    else
    {
        HI_INFO_CA("begin sha2, length:%d\n", pSignImageInfo->u32SignedDataLength);

        //g_DDRSize = 512*1024*1024;
        //dcache_enable(g_DDRSize);
        //sha2((HI_U8 *)pCAImageArea, pSignImageInfo->u32SignedDataLength, TmpSha, 0);
        ret = HI_CA_ADP_CalcHASH((HI_U8 *)pCAImageArea, pSignImageInfo->u32SignedDataLength, TmpSha, Algoritm_Hash2);
        if (ret != HI_SUCCESS)
        {
           HI_ERR_CA("Fail to calc Hash\n");
           return HI_FAILURE;
        }
        //dcache_disable();

        for(index = 0; index < 32; index ++)
        {
            if(TmpSha[index] != pSignImageInfo->u8Sha[index])
            {
                HI_ERR_CA("error index:%d sha:0x%2x != 0x%2x\n", index, TmpSha[index], pSignImageInfo->u8Sha[index]);
                return HI_FAILURE;
            }
        }
    }
    HI_INFO_CA("Sha check ok\n");
    //Now, We got clean image data!
    pSignParam->pImageData = pCAImageArea;
    pSignParam->ImageDataLength = pSignImageInfo->u32SignedDataLength;
    pSignParam->pCASignTail = pSignImageInfo;

    return ret;
}

HI_S32 HI_CA_Verify_Signature(HI_U32 u32MemAddr, HI_CHAR *PartitionImagenName, HI_CHAR *PartitionSignName, HI_U32 offset)
{
    HI_S32 ret = 0, index = 0;
    HI_U32 u32TotalSection = 0;
    HI_CASignImageTail_S SignImageInfo;
    HI_CA_NEW_SignParam_S SignParam;

    //Verify Partition
    CA_CheckPointer(PartitionImagenName);
    CA_CheckPointer(PartitionSignName);

    CA_ASSERT(CA_Search_SignHeader(PartitionSignName, &SignImageInfo, &offset),ret);
    CA_ASSERT(Debug_CASignImageTail_Value(&SignImageInfo),ret);

    u32TotalSection = SignImageInfo.u32TotalsectionID;
    if(u32TotalSection >= 1)
    {
        for(index = 0; index < u32TotalSection; index++)
        {
            CA_ASSERT(CA_Search_SignHeader(PartitionSignName, &SignImageInfo, &offset),ret);
            CA_ASSERT(Debug_CASignImageTail_Value(&SignImageInfo),ret);

            //Get image data
            CA_ASSERT(CA_GetImageData(u32MemAddr, PartitionImagenName, &SignImageInfo, &SignParam),ret);
            //Verify image signature
            CA_ASSERT(HI_CA_ADP_VerifyImage((HI_U32)(SignParam.pImageData), SignParam.ImageDataLength, SignParam.pCASignTail->u8Signature, AUTH_SHA2),ret);

            offset += NAND_PAGE_SIZE;
        }
    }
    else
    {
        HI_ERR_CA("Err u32TotalsectionID = %d\n", u32TotalSection);
    }

    HI_INFO_CA("Check Authenitication is ok\n");
    
    return ret;
}

HI_S32 HI_CA_Verify_Signature_Ex(HI_U32 u32MemAddr, HI_CHAR *PartitionImagenName, HI_CHAR *PartitionSignName, HI_U32 offset)
{
    HI_S32 ret = 0, index = 0;
    HI_U32 u32TotalSection = 0;
    HI_U32 u32random;
    AUTHALG_E eAuthType = AUTH_SHA2;
    HI_CASignImageTail_S SignImageInfo;
    HI_CA_NEW_SignParam_S SignParam;

    //Verify Partition
    CA_CheckPointer(PartitionImagenName);
    CA_CheckPointer(PartitionSignName);

    if ((0xFF == IsSparseFlag) &&
        (!memcmp(PartitionSignName, SYSTEM_SIGN_PARTITION, strlen(SYSTEM_SIGN_PARTITION))) &&
        (!memcmp(PartitionSignName, SYSTEM_IMG_PARTITION, strlen(SYSTEM_IMG_PARTITION)))
        )
    {
        IsSparseFlag = 0;
    }
    CA_ASSERT(CA_Search_SignHeader(PartitionSignName, &SignImageInfo, &offset),ret);
    CA_ASSERT(Debug_CASignImageTail_Value(&SignImageInfo),ret);

    u32TotalSection = SignImageInfo.u32TotalsectionID;
    ca_srandom(READ_TIMER2 & 0xFFFFFFFF);
    u32random = ca_random() % 10 + 1;

    if(u32TotalSection >= 1)
    {
        for(index = 0; index < u32TotalSection;)
        {
            CA_ASSERT(CA_Search_SignHeader(PartitionSignName, &SignImageInfo, &offset),ret);
            CA_ASSERT(Debug_CASignImageTail_Value(&SignImageInfo),ret);

            //Get image data
            CA_ASSERT(CA_GetImageData(u32MemAddr, PartitionImagenName, &SignImageInfo, &SignParam),ret);
            //Verify image signature
            CA_ASSERT(HI_CA_ADP_VerifyImage((HI_U32)(SignParam.pImageData), SignParam.ImageDataLength, SignParam.pCASignTail->u8Signature, eAuthType),ret);
            if (0 == index)
            {
                index = u32random;
            }
            else
            {
                index = index + 10;
            }
            offset = NAND_PAGE_SIZE * index;
        }
    }
    else
    {
        HI_ERR_CA("Err u32TotalsectionID = %d\n", u32TotalSection);
    }

    HI_INFO_CA("Check Authenitication is ok\n");
    return ret;
}


HI_S32 HI_CA_Verify_BootPara(HI_U32 u32MemAddr, HI_U32 u32ParaPartionAddr, HI_U32 u32ParaParitonSize)
{
    HI_S32 ret = 0, index;
    HI_CASignImageTail_S SignImageInfo;
    HI_U8 *ParaBase, *CAImageArea;
    HI_U32 IsFoundFlag = HI_FALSE;
    HI_U8 TmpSha[32];
    AUTHALG_E eAuthType = AUTH_SHA2;

    //ParaBase = (HI_U8*)IMG_VERIFY_ADDRESS;
    ParaBase = (HI_U8*)u32MemAddr;

    //Read out flash data
    CA_ASSERT(CA_flash_read_addr(u32ParaPartionAddr, u32ParaParitonSize, ParaBase, HI_NULL),ret);
    //Get Signature of bottargrs pariton!
    for(index = 0; index < u32ParaParitonSize/NAND_PAGE_SIZE; index ++)
    {
        CAImageArea = ParaBase + index * NAND_PAGE_SIZE;
        memcpy(&SignImageInfo, CAImageArea, sizeof(HI_CASignImageTail_S));
        if(!memcmp(SignImageInfo.u8MagicNumber, CAIMGHEAD_MAGICNUMBER, sizeof(SignImageInfo.u8MagicNumber)))
        {
            unsigned int CRC32Value;
            HI_INFO_CA("Sign Header found!\n");
            IsFoundFlag = HI_TRUE;
            //Clacluate CRC32!
            CRC32Value = DVB_CRC32((HI_U8 *)&SignImageInfo, (sizeof(HI_CASignImageTail_S) - 4));
            if(CRC32Value == SignImageInfo.CRC32)
            {
                HI_INFO_CA("CRC32 check ok\n");
            }
            else
            {
                HI_ERR_CA("CRC32Value:%x != SignImageInfo.CRC32:%x\n", CRC32Value, SignImageInfo.CRC32);
                return HI_FAILURE;
            }
            break;
        }
        else
        {
            continue;
        }
    }
    if(IsFoundFlag != HI_TRUE)
    {
        HI_INFO_CA("Sign Header is not correct!\n");
        return HI_FAILURE;
    }
    //We can check SHA value!
    if(SignImageInfo.u32HashType == Algoritm_Hash1)
    {
        //dcache_enable(g_DDRSize);
        //sha1((HI_U8 *)ParaBase, SignImageInfo.u32SignedDataLength, TmpSha);
        ret = HI_CA_ADP_CalcHASH((HI_U8 *)ParaBase, SignImageInfo.u32SignedDataLength, TmpSha, Algoritm_Hash1);
        if (ret != HI_SUCCESS)
        {
           HI_ERR_CA("%s: Fail to calc Hash\n", __FUNCTION__);
           return HI_FAILURE;
        }
        //dcache_disable();
        for(index = 0; index < 5; index ++)
        {
            if(TmpSha[index] != SignImageInfo.u8Sha[index])
            {
                HI_ERR_CA("error index:%d sha:0x%2x != 0x%2x\n", index, TmpSha[index], SignImageInfo.u8Sha[index]);
                return HI_FAILURE;
            }
        }
    }
    else
    {
        HI_INFO_CA("begin sha2, length:%d\n", SignImageInfo.u32SignedDataLength);
        //dcache_enable(g_DDRSize);
        //sha2((HI_U8 *)ParaBase, SignImageInfo.u32SignedDataLength, TmpSha, 0);
        ret = HI_CA_ADP_CalcHASH((HI_U8 *)ParaBase, SignImageInfo.u32SignedDataLength, TmpSha, Algoritm_Hash2);
        if (ret != HI_SUCCESS)
        {
           HI_ERR_CA("%s: Fail to calc Hash\n", __FUNCTION__);
           return HI_FAILURE;
        }
        //dcache_disable();

        for(index = 0; index < 32; index ++)
        {
            if(TmpSha[index] != SignImageInfo.u8Sha[index])
            {
                HI_ERR_CA("error index:%d sha:0x%2x != 0x%2x\n", index, TmpSha[index], SignImageInfo.u8Sha[index]);
                return HI_FAILURE;
            }
        }
    }
    HI_INFO_CA("Sha check ok\n");
    CA_ASSERT(Debug_CASignImageTail_Value(&SignImageInfo),ret);

    //Verify image signature
    CA_ASSERT(HI_CA_ADP_VerifyImage(u32MemAddr, SignImageInfo.u32SignedDataLength, SignImageInfo.u8Signature, eAuthType),ret);

    HI_INFO_CA("Check Authenitication is ok\n");
    
    return ret;
}

/************************************************************************************************
*
* below define advca functions for advca model
*
*************************************************************************************************/
static HI_S32  CA_VerifyAdvcaImageHeader(HI_CA_AuthInfo_S *pstAuthInfo, HI_U8 *pu8ImgHeaderBuf, HI_U32 u32ImageMaxLen, HI_BOOL *pIsEncrypt)
{
    HI_S32 ret = 0;
    HI_U32 u32CRC32Value = 0;
    HI_CAImgHead_S* pstImgInfo;

    CA_CheckPointer(pstAuthInfo);
    CA_CheckPointer(pu8ImgHeaderBuf);

    pstImgInfo = (HI_CAImgHead_S*)pu8ImgHeaderBuf;    
    if(!memcmp(pstImgInfo->u8MagicNumber,CAIMGHEAD_MAGICNUMBER,sizeof(pstImgInfo->u8MagicNumber)))
    {
        HI_INFO_CA("Find Magic number, Image has not been encrypted\n");
        *pIsEncrypt = HI_FALSE;
    }
    else
    {
	    HI_INFO_CA("Image has been encrypted, decrypt...\n");
        CA_ASSERT(HI_CA_ADP_DataDecrypt(&pstAuthInfo->stCryptoInfo, pu8ImgHeaderBuf,CA_IMAGE_HEADER_AREA_LEN,pu8ImgHeaderBuf),ret);
        udelay(100000);//wait fo decrypt end,this code is essential
        if(0 != memcmp(pstImgInfo->u8MagicNumber,CAIMGHEAD_MAGICNUMBER,sizeof(pstImgInfo->u8MagicNumber)))
        {
            HI_INFO_CA("Not find Magic number, get Image information failed\n");
            return HI_FAILURE;
        }
        HI_INFO_CA("Find Magic number\n");
        *pIsEncrypt = HI_TRUE;
    }

    
    /*Check if the total length is valid*/
    if ((pstImgInfo->u32TotalLen > u32ImageMaxLen) || (pstImgInfo->u32TotalLen < CA_IMAGE_HEADER_AREA_LEN))
    {
        HI_ERR_CA("The total length of image invalid!\n");
        return HI_FAILURE;
    }

    /*Check if the other lengthes of the header are valid*/
    if ((pstImgInfo->u32CodeOffset != CA_IMAGE_HEADER_AREA_LEN) || (pstImgInfo->u32SignedImageLen == 0) || (pstImgInfo->u32SignedImageLen > pstImgInfo->u32TotalLen) ||
            (pstImgInfo->u32SignatureOffset == 0) || (pstImgInfo->u32SignatureOffset > pstImgInfo->u32TotalLen))
    {
        HI_ERR_CA("The header data of image invalid!\n");
        return HI_FAILURE;
    }

    //Clacluate CRC32!
    u32CRC32Value = DVB_CRC32((HI_U8 *)pstImgInfo, (sizeof(HI_CAImgHead_S) - 4));
    if(u32CRC32Value != pstImgInfo->u32CRC32)
    {
        HI_ERR_CA("Fail to check CRC, CRC32Value:%x != pstImgInfo->CRC32:%x\n", u32CRC32Value, pstImgInfo->u32CRC32);
        return HI_FAILURE;
    }

    return ret;
}

HI_S32 HI_CA_GetAdvcaImgInfoByAddr(HI_U32 PartionAddr, HI_CA_AuthInfo_S *pstAuthInfo, HI_CAImgHead_S* pstImgInfo, HI_BOOL *pIsEncrypt)
{
    HI_S32 ret = 0;
    HI_U8 pu8ImgHeaderBuf[CA_IMAGE_HEADER_AREA_LEN] = {0};
    HI_Flash_InterInfo_S flashInfo;    

    CA_CheckPointer(pstImgInfo);
    CA_CheckPointer(pstAuthInfo);
    
    CA_ASSERT(CA_flash_read_addr(PartionAddr,CA_IMAGE_HEADER_AREA_LEN,(HI_U8*)pu8ImgHeaderBuf, &flashInfo),ret);
    ret = CA_VerifyAdvcaImageHeader(pstAuthInfo, pu8ImgHeaderBuf, flashInfo.PartSize, pIsEncrypt);
    if (ret != HI_SUCCESS)
    {
        HI_INFO_CA("%s: Fail to verify image header\n", __FUNCTION__);
        return HI_FAILURE;
    }
    
    memcpy(pstImgInfo, pu8ImgHeaderBuf,sizeof(HI_CAImgHead_S));
    return HI_SUCCESS;

}


HI_S32 HI_CA_GetAdvcaImgInfoByPartName(HI_U8* pPartionName, HI_CA_AuthInfo_S *pstAuthInfo, HI_CAImgHead_S* pstImgInfo, HI_BOOL *pIsEncrypt)
{
    HI_S32 ret = 0;
    HI_U8 pu8ImgHeaderBuf[CA_IMAGE_HEADER_AREA_LEN] = {0};
    HI_Flash_InterInfo_S flashInfo;

    CA_CheckPointer(pPartionName);
    CA_CheckPointer(pstAuthInfo);
    
    CA_ASSERT(CA_flash_read((HI_CHAR*)pPartionName, 0ULL, CA_IMAGE_HEADER_AREA_LEN,(HI_U8*)pu8ImgHeaderBuf, &flashInfo),ret);

    ret = CA_VerifyAdvcaImageHeader(pstAuthInfo, pu8ImgHeaderBuf, flashInfo.PartSize, pIsEncrypt);
    if (ret != HI_SUCCESS)
    {
        HI_INFO_CA("%s: Fail to verify image header\n", __FUNCTION__);
        return HI_FAILURE;
    }
    
    memcpy(pstImgInfo, pu8ImgHeaderBuf,sizeof(HI_CAImgHead_S));
    return ret;
}

HI_S32 HI_CA_AdvcaVerifyByPartAddr(HI_U32 PartitionAddr,HI_U32 u32RamAddr, HI_CA_AuthInfo_S *pstAuthInfo,HI_U32 *pu32CodeAddr)
{
    HI_S32 ret = 0;
    HI_CAImgHead_S ImgInfo;
    HI_U8 u8Signature[SIGNATURE_LEN] = {0};
    HI_U8* pImageSignature = NULL;
    HI_BOOL bEncrypt = HI_TRUE;

    CA_CheckPointer(pstAuthInfo);
    memset(&ImgInfo,0,sizeof(ImgInfo));

    CA_ASSERT(HI_CA_GetAdvcaImgInfoByAddr(PartitionAddr, pstAuthInfo, &ImgInfo, &bEncrypt),ret);
    CA_ASSERT(CA_flash_read_addr(PartitionAddr,ImgInfo.u32TotalLen,(HI_U8*)u32RamAddr, HI_NULL),ret);

    if (HI_TRUE == bEncrypt)
    {
        CA_ASSERT(HI_CA_ADP_DataDecrypt(&pstAuthInfo->stCryptoInfo, (HI_U8 *)u32RamAddr,ImgInfo.u32TotalLen,(HI_U8 *)u32RamAddr),ret);
    }

    pImageSignature = (HI_U8*)(u32RamAddr + ImgInfo.u32SignatureOffset );
    memcpy(u8Signature,pImageSignature,SIGNATURE_LEN);

    CA_ASSERT(HI_CA_ADP_VerifyImage(u32RamAddr,ImgInfo.u32SignedImageLen,u8Signature, pstAuthInfo->eAuthAlg),ret);
    
    if(pu32CodeAddr)
    {
        *pu32CodeAddr = u32RamAddr + ImgInfo.u32CodeOffset ;
    }

    HI_INFO_CA("%s:  0x%x  successed\n",__FUNCTION__, PartitionAddr);    

    return ret;
}


HI_S32 HI_CA_AdvcaVerifyByPartName(HI_U8* pPartionName,HI_U32 u32RamAddr, HI_CA_AuthInfo_S *pstAuthInfo, HI_U32 *pu32CodeAddr)
{
    HI_S32 ret = 0;
    HI_CAImgHead_S ImgInfo;
    HI_U8 u8Signature[SIGNATURE_LEN] = {0};
    HI_U8* pImageSignature = NULL;
    HI_BOOL bEncrypt = HI_TRUE;

    CA_CheckPointer(pPartionName);
    CA_CheckPointer(pstAuthInfo);
    
    memset(&ImgInfo,0,sizeof(ImgInfo));

    CA_ASSERT(HI_CA_GetAdvcaImgInfoByPartName(pPartionName, pstAuthInfo, &ImgInfo, &bEncrypt),ret);   
    CA_ASSERT(CA_flash_read((HI_CHAR*)pPartionName,0ULL,ImgInfo.u32TotalLen,(HI_U8*)u32RamAddr, HI_NULL),ret);

    if (HI_TRUE == bEncrypt)
    {
        CA_ASSERT(HI_CA_ADP_DataDecrypt(&pstAuthInfo->stCryptoInfo, (HI_U8 *)u32RamAddr,ImgInfo.u32TotalLen,(HI_U8 *)u32RamAddr),ret);
    }

    pImageSignature = (HI_U8*)(u32RamAddr + ImgInfo.u32SignatureOffset );
    memcpy(u8Signature,pImageSignature,SIGNATURE_LEN);

    CA_ASSERT(HI_CA_ADP_VerifyImage(u32RamAddr,ImgInfo.u32SignedImageLen,u8Signature, pstAuthInfo->eAuthAlg),ret);
    if(pu32CodeAddr)
    {
        *pu32CodeAddr = u32RamAddr + ImgInfo.u32CodeOffset ;
    }

    HI_INFO_CA("%s: %s successed\n", __FUNCTION__, pPartionName);
    
    return ret;
}


int load_direct_env(void *env, unsigned int offset, unsigned int size)
{
   return -1; /*return Failure and fastboot will continue to read env by normal way*/
}

