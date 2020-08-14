/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : ca_flash.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#include "hi_type.h"
#ifndef HI_MINIBOOT_SUPPORT
#include "common.h"
#endif
#include "ca_debug.h"
#include "ca_flash.h"


extern HI_FLASH_TYPE_E g_enFlashType ;
extern HI_U32 g_MaxBlockSize;
//extern int get_bootmedia(char **media, void *arg);
extern int find_flash_part(char *mtdparts, char *media_name, char *ptn_name, HI_U64 *start, HI_U64 *length);

HI_FLASH_TYPE_E CA_get_env_flash_type(HI_VOID)
{
#if 1
//#ifdef HI_ADVCA_TYPE_VERIMATRIX
//	return HI_FLASH_TYPE_SPI_0;
//#else
    HI_FLASH_TYPE_E EnvFlashType = HI_FLASH_TYPE_BUTT;
    switch (get_bootmedia(NULL, NULL)) 
    {
        default:
            EnvFlashType = HI_FLASH_TYPE_BUTT;
            break;
        case BOOT_MEDIA_NAND:
            EnvFlashType = HI_FLASH_TYPE_NAND_0;
            break;
        case BOOT_MEDIA_SPIFLASH:
            EnvFlashType = HI_FLASH_TYPE_SPI_0;
            break;
        case BOOT_MEDIA_EMMC:
            EnvFlashType = HI_FLASH_TYPE_EMMC_0;
            break;
        case BOOT_MEDIA_SD:
            EnvFlashType = HI_FLASH_TYPE_SD;
            break;
    }

    return EnvFlashType;
#endif
}


HI_S32 CA_flash_GetSize(HI_CHAR *pPartionName, HI_U32 *Size)
{
    HI_S32 Ret = 0;
    HI_HANDLE flashhandle = HI_INVALID_HANDLE;
    HI_Flash_InterInfo_S FlashInfo;
    
    *Size = 0;
    
    flashhandle = HI_Flash_OpenByName(pPartionName);
    if ((0 == flashhandle) || (HI_INVALID_HANDLE == flashhandle))
    {
        HI_ERR_CA("HI_Flash_Open(%s) error\n", pPartionName);
        return -1;
    }

    Ret = HI_Flash_GetInfo(flashhandle, &FlashInfo);
    if (Ret)
    {
        HI_ERR_CA("HI_Flash_GetInfo partion %s error\n", pPartionName);
        HI_Flash_Close(flashhandle);
        return -1;
    }
    *Size = (HI_U32)FlashInfo.PartSize;
    
    HI_Flash_Close(flashhandle);
    return 0;
}

HI_S32 CA_flash_GetPageSize(HI_CHAR *PartitionSignName, HI_U32 *u32PageSize)
{
    HI_S32 Ret = 0;
    HI_HANDLE flashhandle = HI_INVALID_HANDLE;
    HI_Flash_InterInfo_S FlashInfo;
    
    *u32PageSize = 0;
    
    flashhandle = HI_Flash_OpenByName(PartitionSignName);
    if ((0 == flashhandle) || (HI_INVALID_HANDLE == flashhandle))
    {
        HI_ERR_CA("HI_Flash_Open(%s) error\n", PartitionSignName);
        return -1;
    }

    Ret = HI_Flash_GetInfo(flashhandle, &FlashInfo);
    if (Ret)
    {
        HI_ERR_CA("HI_Flash_GetInfo partion %s error\n", PartitionSignName);
        HI_Flash_Close(flashhandle);
        return -1;
    }
    *u32PageSize = (HI_U32)FlashInfo.PageSize;
    
    HI_Flash_Close(flashhandle);
    return 0;
}


HI_S32 CA_flash_GetFlashAddressFromParitonName(HI_FLASH_TYPE_E enFlashType, HI_CHAR *pPartitionName, HI_U32 *Address)
{
    HI_U64 u64Address, u64Len;
    char *ptr;
    const char *bootargs_str = getenv("bootargs");
    char media_name[20];
    
	if(bootargs_str == HI_NULL)
	{
	    HI_ERR_CA("pPartitionName =%s\n",pPartitionName);
	    return HI_INVALID_HANDLE;
	}

	switch (enFlashType)
    {
    case HI_FLASH_TYPE_SPI_0 :
        if ((ptr = strstr(bootargs_str, "mtdparts=")) == NULL)
        {
        	HI_ERR_CA("Cannot find bootargs\n");
        	return HI_INVALID_HANDLE;
        }

        ptr += strlen("mtdparts=");
        memset(media_name, 0 , 20);
        memcpy(media_name, "hi_sfc", 6);
        if (find_flash_part(ptr, media_name, pPartitionName, &u64Address, &u64Len) == 0)
        {
        	HI_ERR_CA("Cannot find partition: %s\n", pPartitionName);
        	return HI_INVALID_HANDLE;
        }
        *Address = (HI_U32)u64Address;
        break;
   case HI_FLASH_TYPE_NAND_0 :
        if ((ptr = strstr(bootargs_str, "mtdparts=")) == NULL)
        {
        	HI_ERR_CA("Cannot find bootargs\n");
        	return HI_INVALID_HANDLE;
        }
    
        ptr += strlen("mtdparts=");
        memset(media_name, 0 , 20);
        memcpy(media_name, "hinand", 6);
    
        if (find_flash_part(ptr, media_name, pPartitionName, &u64Address, &u64Len) == 0)
        {
        	HI_ERR_CA("Cannot find partition: %s\n", pPartitionName);
        	return HI_INVALID_HANDLE;
        }
        *Address = (HI_U32)u64Address;
        break;
   case HI_FLASH_TYPE_EMMC_0 :
   case HI_FLASH_TYPE_SD :
        if ((ptr = strstr(bootargs_str, "blkdevparts=")) == NULL)
        {
        	HI_ERR_CA("Cannot find bootargs\n");
        	return HI_INVALID_HANDLE;
        }
    
        ptr += strlen("blkdevparts=");
        memset(media_name, 0 , 20);
        memcpy(media_name, "mmcblk0", 7);
    
        if (find_flash_part(ptr, media_name, pPartitionName, &u64Address, &u64Len) == 0)
        {
        	HI_ERR_CA("Cannot find partition: %s\n", pPartitionName);
        	return HI_INVALID_HANDLE;
        }
        *Address = (HI_U32)u64Address;
        break;
    default:
        HI_ERR_CA("Do not support FlashType:%d\n", enFlashType);
    }
    
    return 0;
}


HI_S32 CA_flash_getinfo(HI_CHAR *pPartionName, HI_U32 *PartionSize, HI_U32 *BlockSize, HI_U32 *PageSize, HI_U32 *OobSize)
{
    HI_S32 Ret = 0;
    HI_HANDLE flashhandle;
    HI_Flash_InterInfo_S FlashInfo;
    
    flashhandle = HI_Flash_OpenByName(pPartionName);
    if ((0 == flashhandle) || (HI_INVALID_HANDLE == flashhandle))
    {
        HI_ERR_CA("HI_Flash_Open error\n");
        return -1;
    }

    Ret = HI_Flash_GetInfo(flashhandle, &FlashInfo);
    if (Ret)
    {
        HI_ERR_CA("HI_Flash_GetInfo partion %s error\n", pPartionName);
        HI_Flash_Close(flashhandle);
        return -1;
    }
    *PartionSize = (HI_U32)FlashInfo.PartSize;
    *BlockSize = FlashInfo.BlockSize;
    *PageSize = FlashInfo.PageSize;
    *OobSize = FlashInfo.OobSize;
    
    HI_Flash_Close(flashhandle);
    return 0;
}

HI_S32 CA_flash_read(HI_CHAR *pPartionName, HI_U64 offset, HI_U32 bytes, HI_U8 * buf, HI_Flash_InterInfo_S *flashInfo)
{
    HI_S32 Ret = 0;
    HI_HANDLE flashhandle;

    flashhandle = HI_Flash_OpenByName(pPartionName);
    if ((0 == flashhandle) || (HI_INVALID_HANDLE == flashhandle))
    {
        HI_ERR_CA("HI_Flash_Open error, pPartionName:%s\n", pPartionName);
        return -1;
    }

    if (HI_NULL != flashInfo)
    {
        Ret = HI_Flash_GetInfo(flashhandle, flashInfo);
        if (Ret)
        {
            HI_ERR_CA("HI_Flash_GetInfo partion %s error\n", pPartionName);
            HI_Flash_Close(flashhandle);
            return -1;
        }
    }
    
    Ret = HI_Flash_Read(flashhandle, offset, buf, bytes, HI_FLASH_RW_FLAG_RAW);
    if (Ret <= 0)
    {
        HI_ERR_CA("HI_Flash_Read partion %s error\n", pPartionName);
        HI_Flash_Close(flashhandle);
        return -1;
    }
    HI_Flash_Close(flashhandle);
    return 0;
}

HI_S32 CA_flash_readwithoob(HI_CHAR *pPartionName, HI_U64 offset, HI_U32 bytes, HI_U8 * buf)
{
    HI_S32 Ret = 0;
    HI_HANDLE flashhandle;
    flashhandle = HI_Flash_OpenByName(pPartionName);
    if ((0 == flashhandle) || (HI_INVALID_HANDLE == flashhandle))
    {
        HI_ERR_CA("HI_Flash_Open error\n");
        return -1;
    }

    Ret = HI_Flash_Read(flashhandle, offset, buf, bytes, HI_FLASH_RW_FLAG_WITH_OOB);
    if (Ret <= 0)
    {
        HI_ERR_CA("HI_Flash_Read partion %s error\n", pPartionName);
        HI_Flash_Close(flashhandle);
        return -1;
    }
    HI_Flash_Close(flashhandle);
    return 0;
}

HI_S32 CA_flash_write(HI_CHAR *pPartionName, HI_U64 offset, HI_U32 bytes, HI_U8 * buf)
{
    HI_S32 Ret = 0;
    HI_HANDLE flashhandle;
    flashhandle = HI_Flash_OpenByName(pPartionName);

    if ((0 == flashhandle) || (HI_INVALID_HANDLE == flashhandle))
    {
        HI_ERR_CA("HI_Flash_Open error\n");
        return -1;
    }

    HI_INFO_CA("CA_flash_write pPartionName:%s, handleddd = 0x%08x, bytes = 0x%x\n", pPartionName, flashhandle, bytes);
	HI_INFO_CA("offset = 0x%llx\n", offset);

#ifndef HI_MINIBOOT_SUPPORT
    Ret = HI_Flash_Write(flashhandle, offset, buf, bytes, HI_FLASH_RW_FLAG_ERASE_FIRST);
    if (Ret != bytes)
    {
        HI_ERR_CA("HI_Flash_Write partion %s error\n", pPartionName);
        HI_Flash_Close(flashhandle);
        return -1;
    }
#else
	Ret = HI_Flash_Erase(flashhandle, (HI_U64)offset, (HI_U64)bytes);
	if(bytes != Ret)
	{
        HI_ERR_CA("HI_Flash_Erase partion %s error, offset:0x%llx, bytes:0x%x, Ret:0x%x\n", pPartionName, offset, bytes, Ret);
        return -1;
	}
	
    Ret = HI_Flash_Write(flashhandle, offset, buf, bytes, HI_FLASH_RW_FLAG_RAW);
    if (Ret != bytes)
    {
        HI_ERR_CA("HI_Flash_Write partion %s error\n", pPartionName);
        HI_Flash_Close(flashhandle);
        return -1;
    }
#endif

    HI_Flash_Close(flashhandle);

    return 0;

}
HI_HANDLE CA_flash_open_addr(HI_U64 u64addr,HI_U64 u64Len)
{
    HI_HANDLE hFlashHandle;
    HI_Flash_InterInfo_S stFlashInfo;
    HI_S32 ret = HI_SUCCESS;
    HI_U64 u64OpenSize;
	HI_FLASH_TYPE_E enFlashType = HI_FLASH_TYPE_BUTT;
    
    u64Len = (u64Len + g_MaxBlockSize -1)& (~(g_MaxBlockSize -1) );//¿é¶ÔÆë  
    
    enFlashType = CA_get_env_flash_type();
    HI_INFO_CA("Open addr, enFlashType: 0x%08x\n", enFlashType);

    hFlashHandle = HI_Flash_OpenByTypeAndAddr(enFlashType,u64addr,u64Len);
    if(HI_INVALID_HANDLE == hFlashHandle)
    {
		return HI_INVALID_HANDLE;
    }    
      
	ret = HI_Flash_GetInfo(hFlashHandle, &stFlashInfo);
	if(HI_SUCCESS != ret)
	{
		HI_ERR_CA("HI_Flash_GetInfo() error, result %x!\n",ret);
		(HI_VOID)HI_Flash_Close(hFlashHandle);
		return HI_INVALID_HANDLE;
	}
	(HI_VOID)HI_Flash_Close(hFlashHandle);
	
	u64OpenSize = (HI_U64)stFlashInfo.TotalSize - u64addr;
    hFlashHandle = HI_Flash_OpenByTypeAndAddr(enFlashType,u64addr,u64OpenSize);
    if(HI_INVALID_HANDLE == hFlashHandle)
    {
		return HI_INVALID_HANDLE;
    }
	
	return hFlashHandle;    
}
HI_S32 CA_flash_read_addr(HI_U32 u32addr, HI_U32 u32Len, HI_U8 * buf, HI_Flash_InterInfo_S *flashInfo)
{
    HI_S32 Ret = 0;
    HI_HANDLE flashhandle;
    
    flashhandle = CA_flash_open_addr(u32addr,u32Len);
    if ((0 == flashhandle) || (HI_INVALID_HANDLE == flashhandle))
    {
        HI_ERR_CA("HI_Flash_Open error\n");
        return -1;
    }

    if (HI_NULL != flashInfo)
    {
        Ret = HI_Flash_GetInfo(flashhandle, flashInfo);
        if (Ret)
        {
            HI_ERR_CA("HI_Flash_GetInfo partion error\n");
            HI_Flash_Close(flashhandle);
            return -1;
        }
    }
    
    Ret = HI_Flash_Read(flashhandle, 0ULL, buf, u32Len, HI_FLASH_RW_FLAG_RAW);
    if (Ret <= 0)
    {
        HI_ERR_CA("HI_Flash_Read addr 0x%X error\n", u32addr);
        HI_Flash_Close(flashhandle);
        return -1;
    }
    HI_Flash_Close(flashhandle);
    return 0;
}
HI_S32 CA_flash_write_addr(HI_U32 u32addr, HI_U32 u32Len, HI_U8 * buf)
{
    HI_S32 Ret = 0;
    HI_HANDLE flashhandle;
    
    flashhandle = CA_flash_open_addr(u32addr,u32Len);
    if ((0 == flashhandle) || (HI_INVALID_HANDLE == flashhandle))
    {
        HI_ERR_CA("HI_Flash_Open error\n");
        return -1;
    }

    Ret = HI_Flash_Write(flashhandle, 0ULL, buf, u32Len, HI_FLASH_RW_FLAG_ERASE_FIRST);
    if (Ret <= 0)
    {
        HI_ERR_CA("HI_Flash_Write addr 0x%X error\n", u32addr);
        HI_Flash_Close(flashhandle);
        return -1;
    }
    HI_Flash_Close(flashhandle);
    return 0;
}

