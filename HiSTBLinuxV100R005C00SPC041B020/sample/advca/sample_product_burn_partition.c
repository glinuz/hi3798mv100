/**
\file
\brief r2r hardware cryption
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author f00172091
\date 2013-1-22
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hi_type.h"
#include "hi_common.h"
#include "sample_ca_common.h"
#include "hi_flash.h"

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/
/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/



HI_S32 main(HI_S32 argc,HI_CHAR **argv)
{
    HI_S32 Ret = 0;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    
    FILE * fp_image = NULL;
    char *pPartitionName = NULL;
    char *pImageName = NULL;
    HI_U32 u32ImageSize, u32PartitionSize, u32BlockSize;
    HI_U32 u32ReadLen, u32Offset;
    HI_Flash_InterInfo_S stFlashInfo;
    HI_U8 * buf = NULL;
        
    if (3 == argc)
    {
        pPartitionName   = argv[1];
        pImageName   = argv[2];
    }
    else
    {
        printf("Usage: ./product_burn_partition  partition_name  image_file\n");
        return HI_FAILURE;
    }

    HI_DEBUG_ADVCA("\n************ Burn Partition Start *************\n");
    HI_DEBUG_ADVCA("%s %s %s\n",argv[0],argv[1],argv[2]);

    fp_image = fopen(pImageName,"rb");
    if(!fp_image)
    {
    	HI_DEBUG_ADVCA("open file %s error!\n",pImageName);
       (HI_VOID)HI_Flash_Close(hFlash);
       return(HI_FAILURE);
    }

    fseek(fp_image,0,SEEK_END);
    u32ImageSize = (int)ftell(fp_image);
    rewind(fp_image);

    hFlash = HI_Flash_OpenByName(pPartitionName);
    if(HI_INVALID_HANDLE == hFlash)
    {
    	HI_DEBUG_ADVCA("open partition %s error!\n",pPartitionName);
       fclose(fp_image);
       return HI_FAILURE;
    }

    Ret = HI_Flash_GetInfo(hFlash, &stFlashInfo);
    if(HI_SUCCESS != Ret)
    {
    	HI_DEBUG_ADVCA("Get flash info error!\n");
       Ret = HI_FAILURE;
       goto EXIT_CLOSE_FILE;
    }

    u32PartitionSize = (HI_U32)stFlashInfo.PartSize;
    u32BlockSize = (HI_U32)stFlashInfo.BlockSize;

    HI_DEBUG_ADVCA("Partition size [0x%x], Block size [0x%x]\n", u32PartitionSize, u32BlockSize);
    
    if(0 == strncmp(pPartitionName, "kernel", 6))
    {
        if(u32ImageSize > u32PartitionSize)
        {
            /*May be need to burn "system" image from "kernel" partition when in debug system*/
    	    HI_DEBUG_ADVCA("Image size [0x%x] is bigger than partition [%s] size, Reopen flash by address and length!\n", u32ImageSize, pPartitionName);
            HI_FLASH_TYPE_E FlashType = stFlashInfo.FlashType;
            HI_U64 u64PartitionAddr = stFlashInfo.OpenAddr;
            HI_U64 length = ((u32ImageSize + u32BlockSize -1)/u32BlockSize)*u32BlockSize;

    	     HI_DEBUG_ADVCA("Flash type [%d], Address [0x%llx], Length [0x%llx], ImageSize [0x%x]\n", FlashType, u64PartitionAddr, length, u32ImageSize);
            
            (HI_VOID)HI_Flash_Close(hFlash);
            hFlash = HI_Flash_OpenByTypeAndAddr(FlashType,  u64PartitionAddr, length);
            if(HI_INVALID_HANDLE == hFlash)
            {
            	 HI_DEBUG_ADVCA("open partition %s by address and length error!\n",pPartitionName);
               Ret = HI_FAILURE;
               goto EXIT_CLOSE_FILE;
            }
        }
    }
    else if(u32ImageSize > u32PartitionSize)
    {
    	HI_DEBUG_ADVCA("Image size [0x%x] is bigger than partition [%s] size!\n", u32ImageSize, pPartitionName);
       Ret = HI_FAILURE;
       goto EXIT_CLOSE_FILE;
    }

    buf = (HI_U8 *)malloc(u32BlockSize);
    if(NULL == buf)
    {
    	HI_DEBUG_ADVCA("Malloc buffer failure!\n");
       Ret = HI_FAILURE;
       goto EXIT_CLOSE_FILE;
    }

//    Ret = HI_Flash_Erase(hFlash, 0, u32PartitionSize);
    u32Offset = 0;
    while(1)
    {
        memset(buf, 0xff, u32BlockSize);
        u32ReadLen = fread(buf,1, u32BlockSize, fp_image);
        if(u32ReadLen<=0)
        {
            HI_DEBUG_ADVCA("Read to file end.\n");
            goto EXIT_FREE_BUF;
        }
        else if((u32ReadLen != u32BlockSize) && (0==feof(fp_image)))
        {
            HI_DEBUG_ADVCA("Not EOF and the count %x is not align to BlockSize!\n",u32ReadLen);
            goto EXIT_FREE_BUF;
        }
        else
        {
            Ret = HI_Flash_Erase(hFlash, (HI_U64)u32Offset, u32BlockSize);
            Ret = HI_Flash_Write(hFlash, (HI_U64)u32Offset, buf, u32BlockSize, HI_FLASH_RW_FLAG_RAW);
            u32Offset += u32ReadLen;
        }
    }    


EXIT_FREE_BUF:
    if(NULL != buf)
    {
        free(buf);    
    }

EXIT_CLOSE_FILE:
    if(HI_NULL != fp_image)
    {
        fclose(fp_image);
    }
    
    if(HI_INVALID_HANDLE != hFlash)
    {
        (HI_VOID)HI_Flash_Close(hFlash);
    }

    HI_DEBUG_ADVCA("************ Burn Partition End, Ret=0x%x *************\n\n", Ret);

    return Ret;
}


