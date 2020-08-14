
#include <uboot.h>

#ifdef CONFIG_GENERIC_SF
extern struct mtd_info_ex * get_spiflash_info(void);
#endif

#ifdef CONFIG_GENERIC_NAND
extern struct mtd_info_ex * get_nand_info(void);
#endif

#if 0
#define ERR_FALSH_TRACE(fmt, arg...)    printf( "%s,%d: " fmt , __FUNCTION__, __LINE__, ## arg)
#define INFO_FLASH_TRACE(fmt, arg...)    printf( "%s,%d: " fmt , __FUNCTION__, __LINE__, ## arg)
#else
#define ERR_FALSH_TRACE(fmt, arg...)
#define INFO_FLASH_TRACE(fmt, arg...)
#endif
/*  */
typedef struct hiHI_FLASH_HANDLE_S
{
	HI_FLASH_TYPE_E	type;
	union
	{
		nand_logic_t      *nand;
		spiflash_logic_t  *spiflash;
		emmc_logic_t      *emmc;
	} u;
} HI_FLASH_HANDLE_S;

#define HI_FLASH_HANDLE_TO_STRUCT(handle)	((HI_FLASH_HANDLE_S *)(handle))
#define HI_FLASH_STRUCT_TO_HANDLE(handle)	((HI_HANDLE)(handle))
#if 0
static HI_U32 Flash_GetTypeFromName(HI_CHAR *pPartitionName, HI_FLASH_TYPE_E *penFlashType)
{
	char *ptr, *ptr_2;
	const char *bootargs_str = getenv("bootargs");
	char tmp[100];
	HI_U64 Address = 0;
	HI_U64 Len = 0;

	memset(tmp, 0, 100);
	if ( NULL == (ptr = strstr(bootargs_str, "mtdparts=")))
	{
		ERR_FALSH_TRACE("Cannot find bootargs\n");
		return HI_INVALID_HANDLE;
	}

	ptr += strlen("mtdparts=");
	if ((ptr = strstr(bootargs_str, "hinand:")) != NULL)
	{
	    //Fist, we search NAND
	    //nand
	    INFO_FLASH_TRACE("nand:ptr = %s\n",ptr);
	    if ((ptr_2 = strstr(ptr, "hi_sfc:")) != NULL)
	    {
	        //copy out!
	        memcpy(tmp, ptr, (ptr_2 - ptr));
	        ptr = tmp;
	    }
	    //parse nand
		char *media_name = "hinand";
		if (find_flash_part(ptr, media_name, pPartitionName, &Address, &Len) == 0)
		{
			  INFO_FLASH_TRACE ("1.Cannot find partition: %s\n", pPartitionName);
			 *penFlashType = HI_FLASH_TYPE_BUTT;
			//return HI_INVALID_HANDLE;
		}
		else
		{
		    INFO_FLASH_TRACE("Detect Flash Type from name, use: NAND\n");
		    *penFlashType = HI_FLASH_TYPE_NAND_0;
		}

	}
    if ((ptr = strstr(bootargs_str,"hi_sfc:")) != NULL)
	{
	    //Second, we search spi
	    //spi
	    INFO_FLASH_TRACE("spi:ptr = %s\n",ptr);
	    if ((ptr_2 = strstr(ptr, "hinand:")) != NULL)
	    {
	        //copy out!, Impossible!
	        memcpy(tmp, ptr, (ptr_2 - ptr));
	        ptr = tmp;
	    }
	    //parse spi.
		char *media_name = "hi_sfc";
		if (find_flash_part(ptr, media_name, pPartitionName, &Address, &Len) == 0)
		{
			 INFO_FLASH_TRACE("2.Cannot find partition: %s\n", pPartitionName);
        	 *penFlashType = HI_FLASH_TYPE_BUTT;
		}
		else
		{
		    INFO_FLASH_TRACE("Detect Flash Type from name, use: spi\n");
		    *penFlashType = HI_FLASH_TYPE_SPI_0;
		}

	}
	return 0;
}
#endif
static int _HI_Flash_Init(void)
{
	char *ptr, *p1, *p2;
	char *flash_part = NULL;
	char *bootargs_tmp = NULL;

	const char *bootargs = (const char *)getenv("bootargs");
	if (NULL == bootargs) {
		ERR_FALSH_TRACE("No bootargs!\n");
		goto fail;
	}

	flash_part = (char *)malloc(strlen(bootargs) + 1);
	if (!flash_part) {
		ERR_FALSH_TRACE("Fail to malloc for bootargs!\n");
		goto fail;
	}
	strncpy(flash_part, bootargs, strlen(bootargs) + 1);

	bootargs_tmp = (char *)malloc(strlen(bootargs) + 1);
	if (!bootargs_tmp) {
		ERR_FALSH_TRACE("Fail to malloc for bootargs!\n");
		goto fail;
	}
	strncpy(bootargs_tmp, bootargs, strlen(bootargs) + 1);

	p1 = (char *)strstr(bootargs_tmp, "mtdparts=");
	p2 = (char *)strstr(bootargs_tmp, "blkdevparts=");
	if ((p1 == NULL) && (p2 == NULL)) {
		ERR_FALSH_TRACE("Incorrect bootargs!You should define mtdparts or blkdevparts");
		goto fail;
	}

	memset(flash_part, 0, strlen(flash_part));
	if (p1) {
		ptr = NULL;
		ptr = strchr(p1, ' ');
		if (ptr)
			*ptr = '\0';
		p1 += strlen("mtdparts=");
		strncpy(flash_part, p1, strlen(p1));
		*(flash_part + strlen(p1) + 1) = '\0'; 
		strncat(flash_part, ";", sizeof(";") - 1);
		if (ptr)
			*ptr = ' ';
	}

	if (p2) {
		ptr = NULL;
		ptr = strchr(p2, ' ');
		if (ptr)
			*ptr = '\0';
		p2 += strlen("blkdevparts=");
		strncat(flash_part, p2, strlen(p2));
		if (ptr)
			*ptr = ' ';
	}

	INFO_FLASH_TRACE("flash part:%s\n", flash_part);
	INFO_FLASH_TRACE("bootargs:%s\n", bootargs);

	if (flash_part_init(flash_part) < 0) {
		ERR_FALSH_TRACE("Fail to init flash part!\n");
		goto fail;
	}

	if (flash_part)
		free(flash_part);
	if (bootargs_tmp)
		free(bootargs_tmp);
	return 0;
fail:
	if (flash_part)
		free(flash_part);
	if (bootargs_tmp)
		free(bootargs_tmp);
	return -1;
}


HI_HANDLE HI_Flash_OpenByName(HI_CHAR *pPartitionName)
{
    HI_S32 index = 0;
    HI_HANDLE hFlashHandle = HI_INVALID_HANDLE;

    for(index = 0; index<HI_FLASH_TYPE_BUTT; index++)
    {
        hFlashHandle = HI_Flash_OpenByTypeAndName((HI_FLASH_TYPE_E)index,pPartitionName);
        if(HI_INVALID_HANDLE != hFlashHandle)
        {
          return hFlashHandle;
        }
    }
    return HI_INVALID_HANDLE;
}

HI_HANDLE HI_Flash_OpenByTypeAndName(HI_FLASH_TYPE_E enFlashType, HI_CHAR *pPartitionName)
{
    static HI_FLASH_HANDLE_S *flash_handle;
    HI_U64 u64Address = 0;
    HI_U64 u64Len = 0;
    char *ptr;

#if defined(CONFIG_GENERIC_SF) || defined(CONFIG_GENERIC_NAND)
    struct mtd_info_ex *mtdinfo;
#endif
    const char *bootargs_str = (const char *)getenv("bootargs");
    char media_name[20];

    INFO_FLASH_TRACE("enFlashType = %d,pPartitionName =%s\n",enFlashType, pPartitionName);
	if((pPartitionName == HI_NULL) || (enFlashType == HI_FLASH_TYPE_BUTT) || (bootargs_str == HI_NULL))
	{
	    ERR_FALSH_TRACE("enFlashType = %d,pPartitionName =%s\n",enFlashType,pPartitionName);
	    return (HI_HANDLE)HI_INVALID_HANDLE;
	}

	/* to avoid codecc: buffer overflow. */
	ptr = malloc(sizeof(HI_FLASH_HANDLE_S));
	if (!ptr)
	{
		ERR_FALSH_TRACE("no many memory.\n");
		return (HI_HANDLE)HI_INVALID_HANDLE;
	}
	memset(ptr, 0, sizeof(HI_FLASH_HANDLE_S));
	flash_handle = (HI_FLASH_HANDLE_S *)(HI_ULONG)ptr;

	if (_HI_Flash_Init() < 0) {
		free(flash_handle);
		return (HI_HANDLE)HI_INVALID_HANDLE;
	}

	flash_handle->type = enFlashType;

	switch (enFlashType)
    {
#ifdef CONFIG_GENERIC_SF
        case HI_FLASH_TYPE_SPI_0 :
            if ((ptr = (char *)strstr(bootargs_str, "mtdparts=")) == NULL)
            {
            	ERR_FALSH_TRACE("Cannot find bootargs\n");
            	free(flash_handle);
            	return (HI_HANDLE)HI_INVALID_HANDLE;
            }

            ptr += strlen("mtdparts=");
            memset(media_name, 0 , 20);
            memcpy(media_name, "hi_sfc", 6);
            if (find_flash_part(ptr, media_name, pPartitionName, &u64Address, &u64Len) == 0)
            {
            	ERR_FALSH_TRACE("Cannot find partition: %s\n", pPartitionName);
            	free(flash_handle);
            	return (HI_HANDLE)HI_INVALID_HANDLE;
            }

            if(u64Len == (HI_U64)(-1))
            {
            	mtdinfo = get_spiflash_info();
            	if (mtdinfo == NULL)
            	{
            		ERR_FALSH_TRACE("get spi flash info failed\n");
                    free(flash_handle);

            		return (HI_HANDLE)HI_INVALID_HANDLE;
            	}

            	u64Len = mtdinfo->chipsize - u64Address;
            }

            flash_handle->u.spiflash = spiflash_logic_open(u64Address, u64Len);
            if(flash_handle->u.spiflash == NULL)
            {
            	free(flash_handle);
            	return (HI_HANDLE)HI_INVALID_HANDLE;
            }
            break;
#endif
#ifdef CONFIG_GENERIC_NAND
        case HI_FLASH_TYPE_NAND_0 :
            if ((ptr = (char *)strstr(bootargs_str, "mtdparts=")) == NULL)
            {
            	ERR_FALSH_TRACE("Cannot find bootargs\n");
            	free(flash_handle);
            	return (HI_HANDLE)HI_INVALID_HANDLE;
            }

            ptr += strlen("mtdparts=");
            memset(media_name, 0 , 20);
            memcpy(media_name, "hinand", 6);

            if (find_flash_part(ptr, media_name, pPartitionName, &u64Address, &u64Len) == 0)
            {
            	ERR_FALSH_TRACE("Cannot find partition: %s\n", pPartitionName);
            	free(flash_handle);
            	return (HI_HANDLE)HI_INVALID_HANDLE;
            }

            if(u64Len == (HI_U64)(-1))
            {
            	mtdinfo = get_nand_info();
            	if (mtdinfo == NULL)
            	{
            		ERR_FALSH_TRACE("get spi flash info failed\n");
                    free(flash_handle);
            		return (HI_HANDLE)HI_INVALID_HANDLE;
            	}

            	u64Len = mtdinfo->chipsize - u64Address;
            }

            flash_handle->u.nand = nand_logic_open(u64Address, u64Len);
            if(flash_handle->u.nand == NULL)
            {
            	free(flash_handle);
            	return (HI_HANDLE)HI_INVALID_HANDLE;
            }
            break;
#endif
#ifdef CONFIG_GENERIC_MMC
        case HI_FLASH_TYPE_EMMC_0 :
        case HI_FLASH_TYPE_SD :
            if ((ptr = strstr(bootargs_str, "blkdevparts=")) == NULL)
            {
            	ERR_FALSH_TRACE("Cannot find bootargs\n");
            	free(flash_handle);
            	return (HI_HANDLE)HI_INVALID_HANDLE;
            }

            ptr += strlen("blkdevparts=");
            memset(media_name, 0 , 20);
            memcpy(media_name, "mmcblk0", strlen("mmcblk0"));
            if (find_flash_part(ptr, media_name, pPartitionName, &u64Address, &u64Len) == 0)
            {
            	ERR_FALSH_TRACE("Cannot find partition: %s\n", pPartitionName);
            	free(flash_handle);
                return (HI_HANDLE)HI_INVALID_HANDLE;
            }

            flash_handle->u.emmc = emmc_logic_open(u64Address, u64Len);
            if( NULL == flash_handle->u.emmc)
            {
                free(flash_handle);
                return (HI_HANDLE)HI_INVALID_HANDLE;
            }
            break;
#endif

        default:
            free(flash_handle);
            return (HI_HANDLE)HI_INVALID_HANDLE;
    }

    return HI_FLASH_STRUCT_TO_HANDLE((HI_ULONG)flash_handle);
}

HI_HANDLE HI_Flash_OpenByTypeAndAddr(HI_FLASH_TYPE_E enFlashType, HI_U64 u64Address, HI_U64 u64Len)
{
    INFO_FLASH_TRACE("enFlashType = %d,u64Address = 0x%llx,u64Len =0x%llx\n",enFlashType,u64Address,u64Len);
    HI_FLASH_HANDLE_S *flash_handle;

	if((enFlashType == HI_FLASH_TYPE_BUTT) || (u64Len == 0))
	{
	    return HI_INVALID_HANDLE;
	}

	if ((flash_handle = (HI_FLASH_HANDLE_S *)malloc(sizeof(HI_FLASH_HANDLE_S))) == NULL)
	{
		ERR_FALSH_TRACE("no many memory.\n");
		return HI_INVALID_HANDLE;
	}

	flash_handle->type = enFlashType;

	switch (enFlashType)
    {
#ifdef CONFIG_GENERIC_SF
        case HI_FLASH_TYPE_SPI_0:
        	flash_handle->u.spiflash = spiflash_logic_open(u64Address, u64Len);
        	if(flash_handle->u.spiflash == NULL)
        	{
        		free(flash_handle);
        		return HI_INVALID_HANDLE;
        	}
        	break;
#endif
#ifdef CONFIG_GENERIC_NAND
        case HI_FLASH_TYPE_NAND_0:
        	flash_handle->u.nand = nand_logic_open(u64Address, u64Len);
        	if(flash_handle->u.nand == NULL)
        	{
        		free(flash_handle);
        		return HI_INVALID_HANDLE;
        	}
            break;
#endif
#ifdef CONFIG_GENERIC_MMC
        case HI_FLASH_TYPE_EMMC_0:
        case HI_FLASH_TYPE_SD :
            flash_handle->u.emmc = emmc_logic_open(u64Address, u64Len);
            if( NULL == flash_handle->u.emmc)
            {
            	free(flash_handle);
            	return HI_INVALID_HANDLE;
            }
            break;
#endif

        default:
        	free(flash_handle);
        	return HI_INVALID_HANDLE;
        	break;
	}

	return HI_FLASH_STRUCT_TO_HANDLE(flash_handle);
}

HI_HANDLE HI_Flash_Open(HI_FLASH_TYPE_E enFlashType, HI_CHAR *pPartitionName, HI_U64 u64Address, HI_U64 u64Len)
{
	if(HI_NULL != pPartitionName)
	{
	    if( enFlashType == HI_FLASH_TYPE_BUTT)
		{
		    return HI_Flash_OpenByName(pPartitionName);
		}
		else
		{
		    return HI_Flash_OpenByTypeAndName(enFlashType, pPartitionName);
		}
	}
	else
	{
		return HI_Flash_OpenByTypeAndAddr(enFlashType, u64Address, u64Len);
	}
}

HI_S32 HI_Flash_Close(HI_HANDLE hFlash)
{
	HI_FLASH_HANDLE_S *flash_handle = HI_FLASH_HANDLE_TO_STRUCT(hFlash);

	switch (flash_handle->type)
	{
#ifdef CONFIG_GENERIC_SF
        case HI_FLASH_TYPE_SPI_0 :
        	spiflash_logic_close(flash_handle->u.spiflash);
        	break;
#endif
#ifdef CONFIG_GENERIC_NAND
        case HI_FLASH_TYPE_NAND_0 :
        	nand_logic_close(flash_handle->u.nand);
        	break;
#endif
#ifdef CONFIG_GENERIC_MMC
        case HI_FLASH_TYPE_EMMC_0:
        case HI_FLASH_TYPE_SD:
            emmc_logic_close(flash_handle->u.emmc);
            break;
#endif
        default:
        	return HI_FAILURE;
        	break;
	}

	free(flash_handle);
	return HI_SUCCESS;
}

HI_S32 HI_Flash_Erase(HI_HANDLE hFlash, HI_U64 Offset, HI_U64 Len)
{
	HI_FLASH_HANDLE_S *flash_handle = HI_FLASH_HANDLE_TO_STRUCT(hFlash);

	if (Len >= 0x80000000) {
		ERR_FALSH_TRACE("Len is too big(0x%llx), should use HI_Flash_Erase64() instead.\n", Len);
		return HI_FAILURE;
	}
	switch (flash_handle->type)
	{
#ifdef CONFIG_GENERIC_SF
        case HI_FLASH_TYPE_SPI_0 :
            return (HI_S32)spiflash_logic_erase(flash_handle->u.spiflash, Offset, Len);
#endif
#ifdef CONFIG_GENERIC_NAND
        case HI_FLASH_TYPE_NAND_0 :
            return (HI_S32)nand_logic_erase(flash_handle->u.nand, Offset, Len);
#endif
#ifdef CONFIG_GENERIC_MMC
        case HI_FLASH_TYPE_EMMC_0:
        case HI_FLASH_TYPE_SD:
            return (HI_S32)Len;
#endif
        default:
            return HI_FAILURE;
	}
}

HI_S64 HI_Flash_Erase64(HI_HANDLE hFlash, HI_U64 Offset, HI_U64 Len)
{
	HI_FLASH_HANDLE_S *flash_handle = HI_FLASH_HANDLE_TO_STRUCT(hFlash);

	switch (flash_handle->type)
	{
#ifdef CONFIG_GENERIC_SF
        case HI_FLASH_TYPE_SPI_0 :
            return (HI_S64)spiflash_logic_erase(flash_handle->u.spiflash, Offset, Len);
#endif
#ifdef CONFIG_GENERIC_NAND
        case HI_FLASH_TYPE_NAND_0 :
            return (HI_S64)nand_logic_erase(flash_handle->u.nand, Offset, Len);
#endif
#ifdef CONFIG_GENERIC_MMC
        case HI_FLASH_TYPE_EMMC_0:
        case HI_FLASH_TYPE_SD:
            return (HI_S64)Len;
#endif
        default:
            return HI_FAILURE;
	}
}

#ifdef CONFIG_GENERIC_NAND
HI_S32 HI_Adpt_Nand_read( nand_logic_t *pstNandLog,
                           HI_U64 u64Offset,
                           HI_U32 u32Len,
                           HI_UCHAR  *pucBuf,
                           HI_S32 s32Withoob )
{
	HI_U32 u32Ret = 0;
	HI_U32 u32ReadLength;
	HI_U32 u32PageSize;
	HI_U32 u32PageOffset;
	HI_U32 u32PageOOBOffset;
	HI_U32 u32ReadLenWithoutOOB;
	HI_U32 u32ReadLenWithOOB;
	HI_U32 u32PageNumber;
	HI_UCHAR *pucTmpBuff = NULL;
	nand_info_t *nand = NULL;

	if( NULL == pstNandLog || NULL == pucBuf) {
		ERR_FALSH_TRACE("Pointer is null.");
		return HI_FAILURE;
	}

	nand = (nand_info_t *) pstNandLog->nand;

	if (s32Withoob) {
		u32PageSize = nand->writesize;
		u32PageNumber = u32Len / (nand->writesize + nand->oobused);
		u32ReadLenWithoutOOB = u32PageNumber * nand->writesize;
		u32PageOOBOffset = u32Len % (nand->writesize + nand->oobused);
		u32ReadLenWithOOB = u32PageNumber * (nand->writesize + nand->oobused);

		if (u32PageOOBOffset) {
			INFO_FLASH_TRACE("HI_Adpt_Nand_read:Read length is not aligned with (pagesize + oobsize), "
					 "request read length: 0x%x\n"
					 "pagesize: 0x%x, oobsize: 0x%x. "
					 "Real read length: 0x%x!\n",
					 u32Len,
					 nand->writesize,
					 nand->oobused,
					 u32ReadLenWithOOB);
		}
		if (u32PageNumber) {
			u32Ret = nand_logic_read(pstNandLog, u64Offset, u32ReadLenWithoutOOB, pucBuf, s32Withoob);
			if ( 0 >= u32Ret)
				return HI_FAILURE;
			else if (u32ReadLenWithOOB != u32Ret) {
				/* in case that read out of flash area. */
				return u32Ret;
			}
		}

		u64Offset += u32ReadLenWithoutOOB;
		u32ReadLength = u32Ret;
		if (u32PageOOBOffset) {
			if( NULL ==(pucTmpBuff = (HI_UCHAR*)malloc(u32PageSize + nand->oobused))) {
				ERR_FALSH_TRACE("Failed to allocate memory.");
				return HI_FAILURE;
			}

			u32Ret = nand_logic_read(pstNandLog, u64Offset, u32PageSize, pucTmpBuff, s32Withoob);
			if( 0 >= u32Ret) {
				free(pucTmpBuff);
				return HI_FAILURE;
			}

			memcpy((pucBuf + u32ReadLength), pucTmpBuff, u32PageOOBOffset);
			free(pucTmpBuff);
		}

		return u32Len;
	} else {
		u32PageSize = nand->writesize;
		u32PageOffset = u32Len & (u32PageSize - 1);

		u32ReadLength = u32Len - u32PageOffset;
		if( u32ReadLength ) {
			u32Ret = nand_logic_read(pstNandLog, u64Offset, u32ReadLength, pucBuf, 0);

			if( 0 >= u32Ret)
				return HI_FAILURE;
			else if (u32ReadLength != u32Ret)
				return u32Ret;
		}

		u64Offset += u32ReadLength;
		u32ReadLength = u32Ret;

		if( u32PageOffset ) {
			if( NULL ==(pucTmpBuff = (HI_UCHAR*)malloc(u32PageSize))) {
				ERR_FALSH_TRACE("Failed to allocate memory.");
				return HI_FAILURE;
			}

			u32Ret = nand_logic_read(pstNandLog, u64Offset, u32PageSize, pucTmpBuff, 0);
			if( 0 >= u32Ret) {
				free(pucTmpBuff);
				return HI_FAILURE;
			}

			memcpy((pucBuf + u32ReadLength), pucTmpBuff, u32PageOffset);
			free(pucTmpBuff);
		}

		return u32Len;
	}
}

#if 0
HI_S32 HI_Adpt_Nand_read_with_oob( nand_logic_t *pstNandLog,
                                   HI_U64 u64Offset,
                                   HI_U32 u32Len,
                                   HI_UCHAR *pucBuf,
                                   HI_U32 u32BufLen)
{
	HI_U32 u32Ret;
	HI_U32 u32PageSize;
	HI_U32 u32OOBSize;
	HI_U32 u32ReadLenWithOOB;
	HI_S32 s32Withoob = 1;
	nand_info_t *nand = (nand_info_t *) pstNandLog->nand;

	if( NULL == pstNandLog || NULL == pucBuf) {
		ERR_FALSH_TRACE("Pointer is null.");
		return HI_FAILURE;
	}

	if ((u64Offset % nand->writesize)
	   || (u32Len % nand->writesize)) {
		ERR_FALSH_TRACE("offset and length should be page aligned!"
		" Offset:0x%llx, Length:0x%llx\n",
		u64Offset, u32Len);
	}

	u32PageSize = nand->writesize;
	u32OOBSize = nand->oobused;
	u32ReadLenWithOOB = u32Len / u32PageSize *
			    (u32PageSize + u32OOBSize);
	if (u32BufLen < u32ReadLenWithOOB) {
		ERR_FALSH_TRACE("buffer is too small! "
		"Allocated buffer length(0x%x) is smaller "
		"than requred(0x%x)!\n",
		u32BufLen, u32ReadLenWithOOB);
	}

	u32Ret = nand_logic_read(pstNandLog, u64Offset, u32Len, pucBuf, s32Withoob);
	if ( 0 >= u32Ret)
		return HI_FAILURE;
	return u32Ret;
}
#endif

#endif

#ifdef CONFIG_GENERIC_MMC
HI_S32 HI_Adpt_emmc_read(emmc_logic_t *pstEmmcLog,
                         HI_U64 u64Offset,
                         HI_U32 u32Len,
                         HI_UCHAR *pucBuf)
{
    HI_S32 s32Ret;
    HI_U32 u32BlkSize;
    HI_U32 u32BlkOffset;
    HI_U32 u32ReadLen;
    HI_UCHAR *pucTmpBuff = NULL;

    if( NULL == pstEmmcLog
       || NULL == pucBuf)
    {
        ERR_FALSH_TRACE("Pointer is null.");
        return HI_FAILURE;
    }

    u32BlkSize = (HI_U32)pstEmmcLog->blocksize;
    u32BlkOffset = u32Len & (HI_U32)(u32BlkSize - 1);

    u32ReadLen = u32Len - u32BlkOffset;
    if(u32ReadLen)
    {
        s32Ret = emmc_logic_read(pstEmmcLog, u64Offset, u32ReadLen, pucBuf);
        if( HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }
    }

    if( u32BlkOffset )
    {
        if( NULL ==(pucTmpBuff = (HI_UCHAR*)malloc(u32BlkSize)))
        {
            ERR_FALSH_TRACE("Failed to allocate memory.");
            return HI_FAILURE;
        }

        memset(pucTmpBuff, 0, u32BlkSize);
        s32Ret = emmc_logic_read(pstEmmcLog, u64Offset + u32ReadLen, u32BlkSize, pucTmpBuff);
        if( HI_SUCCESS != s32Ret)
        {
            free(pucTmpBuff);
            return s32Ret;
        }

        memcpy((pucBuf + u32ReadLen), pucTmpBuff, u32BlkOffset);
        free(pucTmpBuff);
    }

    return (HI_S32)u32Len;
}
#endif

HI_S32 HI_Flash_Read(HI_HANDLE hFlash, HI_U64 Offset, HI_U8 *pBuf,
                     HI_U32 Len, HI_U32 Flags)
{
	/* to avoid codecc: buffer overflow */
	HI_FLASH_HANDLE_S *flash_handle = HI_FLASH_HANDLE_TO_STRUCT((HI_ULONG)hFlash);

    INFO_FLASH_TRACE("HI_Flash_Read:hFlash:%d, Offset:0x%llx, pBuf:0x%x, Len:0x%x,Flags:%d \r\n",flash_handle->type,Offset ,(HI_U32)pBuf,Len,Flags);
	switch (flash_handle->type)
	{
#ifdef CONFIG_GENERIC_SF
        case HI_FLASH_TYPE_SPI_0:
            return spiflash_logic_read(flash_handle->u.spiflash,
            	Offset, Len, pBuf);
#endif
#ifdef CONFIG_GENERIC_NAND

        case HI_FLASH_TYPE_NAND_0:
            return HI_Adpt_Nand_read(flash_handle->u.nand,
            	Offset, Len, pBuf, (Flags & HI_FLASH_RW_FLAG_WITH_OOB));
#endif
#ifdef CONFIG_GENERIC_MMC
        case HI_FLASH_TYPE_EMMC_0:
        case HI_FLASH_TYPE_SD:
            return HI_Adpt_emmc_read(flash_handle->u.emmc,
                                   Offset, Len, pBuf);
#endif

        default:
        return HI_FAILURE;
    }
}
#ifdef CONFIG_GENERIC_NAND
HI_S32 HI_Adpt_Nand_write( nand_logic_t *pstNandLog,
                          HI_U64 u64Offset,
                          HI_U32 u32Len,
                          HI_UCHAR  *pucBuf,
                          HI_S32 s32Withoob )
{
	HI_U32 u32Ret;
	nand_info_t *nand = NULL;

	if( NULL == pstNandLog || NULL == pucBuf) {
		ERR_FALSH_TRACE("Pointer is null.");
		return HI_FAILURE;
	}

	nand = (nand_info_t *) pstNandLog->nand;

	if ((u64Offset % nand->writesize)
	   || (u32Len % nand->writesize)) {
		ERR_FALSH_TRACE("offset and length should be page aligned!"
		" Offset:0x%llx, Length:0x%x\n",
		u64Offset, u32Len);
		return HI_FAILURE;
	}

	u32Ret = nand_logic_write(pstNandLog, u64Offset, u32Len, pucBuf, s32Withoob);
	return u32Ret;
}

#if 0
HI_S32 HI_Adpt_Nand_write_with_oob( nand_logic_t *pstNandLog,
                                    HI_U64 u64Offset,
                                    HI_U32 u32Len,
                                    HI_UCHAR *pucBuf,
                                    HI_U32 u32BufLen )
{
	HI_S32 s32Withoob = 1;
	HI_U32 u32Ret;
	HI_U32 u32PageSize;
	HI_U32 u32OOBSize;
	HI_U32 u32ReadLenWithOOB;
	nand_info_t *nand = (nand_info_t *) pstNandLog->nand;

	if( NULL == pstNandLog || NULL == pucBuf) {
		ERR_FALSH_TRACE("Pointer is null.");
		return HI_FAILURE;
	}

	if ((u64Offset % nand->writesize)
	   || (u32Len % nand->writesize)) {
		ERR_FALSH_TRACE("offset and length should be page aligned!"
		" Offset:0x%llx, Length:0x%llx\n",
		u64Offset, u32Len);
		return HI_FAILURE;
	}

	u32PageSize = nand->writesize;
	u32OOBSize = nand->oobused;
	u32ReadLenWithOOB = u32Len / u32PageSize *
			    (u32PageSize + u32OOBSize);
	if (u32BufLen < u32ReadLenWithOOB) {
		ERR_FALSH_TRACE("buffer is too small! "
		"Allocated buffer length(0x%x) is smaller "
		"than requred(0x%x)!\n",
		u32BufLen, u32ReadLenWithOOB);
	}

	u32Ret = nand_logic_write(pstNandLog, u64Offset, u32Len, pucBuf, s32Withoob);
	return u32Ret;
}

#endif

#endif

#ifdef CONFIG_GENERIC_MMC
HI_S32 HI_Adpt_emmc_write(emmc_logic_t *pstEmmcLog,
                          HI_U64 u64Offset,
                          HI_U32 u32Len,
                          HI_UCHAR *pucBuf)
{
    HI_S32 s32Ret;
    HI_U32 u32BlkSize;
    HI_U32 u32BlkOffset;
    HI_U32 u32WriteLen;
    HI_UCHAR *pucTmpBuff;

    if( NULL == pstEmmcLog
       || NULL == pucBuf)
    {
        ERR_FALSH_TRACE("Pointer is null.");
        return HI_FAILURE;
    }

    u32BlkSize = (HI_U32)pstEmmcLog->blocksize;
    u32BlkOffset = u32Len & (HI_U32)(u32BlkSize - 1);
    INFO_FLASH_TRACE("emmc blk offset is %u\n", u32BlkOffset);
    u32WriteLen = u32Len - u32BlkOffset;
    INFO_FLASH_TRACE("emmc write length is %u\n", u32WriteLen);
    if(u32WriteLen)
    {
        s32Ret = emmc_logic_write(pstEmmcLog, u64Offset, u32WriteLen, pucBuf);
        if( HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }
    }

    if( u32BlkOffset )
    {
        if( NULL ==(pucTmpBuff = (HI_UCHAR*)malloc(u32BlkSize)))
        {
            ERR_FALSH_TRACE("Failed to allocate memory.");
            return HI_FAILURE;
        }

        memset(pucTmpBuff, 0xFF, u32BlkSize);
        memcpy(pucTmpBuff, pucBuf + u32WriteLen, u32BlkOffset);
        s32Ret = emmc_logic_write(pstEmmcLog, u64Offset + u32WriteLen, u32BlkSize, pucTmpBuff);
        if( HI_SUCCESS != s32Ret)
        {
            free(pucTmpBuff);
            return s32Ret;
        }
        free(pucTmpBuff);
    }
    return u32Len;
}
#endif

HI_S32 HI_Flash_Write(HI_HANDLE hFlash, HI_U64 Offset,
                      HI_U8 *pBuf, HI_U32 Len, HI_U32 Flags)
{
	HI_S32 ret, erase;
	HI_U32 BlockSize_new = 0;
	HI_U32 u32EraseLen = Len;
	HI_FLASH_HANDLE_S *flash_handle = HI_FLASH_HANDLE_TO_STRUCT(hFlash);

	if (HI_FLASH_TYPE_NAND_0 == flash_handle->type) {
		nand_info_t *nand = flash_handle->u.nand->nand;
		if(HI_FLASH_RW_FLAG_ERASE_FIRST == (Flags & HI_FLASH_RW_FLAG_ERASE_FIRST))
			erase = 1;
		else
			erase = 0;

		if (HI_FLASH_RW_FLAG_WITH_OOB == (Flags & HI_FLASH_RW_FLAG_WITH_OOB)) {
			BlockSize_new = nand->erasesize + nand->oobused * 
					(nand->erasesize / nand->writesize);
			if (Len % (nand->writesize + nand->oobused)) {
				printf("Write length should be "
					"(pagesize + oobsize):%08x aligned:\n",
					nand->writesize + nand->oobused);
				return HI_FAILURE;
			}
			Len = Len / (nand->writesize + nand->oobused) * nand->writesize;
		}
		else {
			BlockSize_new = nand->erasesize;
			if (Len % nand->writesize) {
				printf("Write length should be "
					"(pagesize):%08x aligned:\n",
					nand->writesize);
				return HI_FAILURE;
			}
		}


		if (erase) {
			if (Len % nand->erasesize)
				u32EraseLen += BlockSize_new;
			u32EraseLen = u32EraseLen / BlockSize_new * nand->erasesize;

			ret = HI_Flash_Erase(hFlash, Offset, u32EraseLen);
			if (0 >= ret){
				printf ("earse fail!\n");
				return ret;
			}
		}
	} else if (HI_FLASH_TYPE_SPI_0 == flash_handle->type) {
		if (HI_FLASH_RW_FLAG_ERASE_FIRST == (Flags & HI_FLASH_RW_FLAG_ERASE_FIRST)) {
			HI_U32 Erasesize = flash_handle->u.spiflash->erasesize;
			u32EraseLen = (u32EraseLen + Erasesize - 1)
				      & (~(Erasesize - 1));
			ret = HI_Flash_Erase(hFlash, Offset, u32EraseLen);
			if (0 >= ret) {
				printf ("earse fail!\n");
				return ret;
			}
		}
	}

	switch (flash_handle->type)
	{
#ifdef CONFIG_GENERIC_SF
        case HI_FLASH_TYPE_SPI_0:
    		return spiflash_logic_write(flash_handle->u.spiflash,
    			                        Offset, Len, pBuf);
#endif
#ifdef CONFIG_GENERIC_NAND

        case HI_FLASH_TYPE_NAND_0:
    		return HI_Adpt_Nand_write(flash_handle->u.nand,
    			                    Offset, Len, pBuf, (Flags & HI_FLASH_RW_FLAG_WITH_OOB));
#endif

#ifdef CONFIG_GENERIC_MMC
        case HI_FLASH_TYPE_EMMC_0:
        case HI_FLASH_TYPE_SD:
            return HI_Adpt_emmc_write(flash_handle->u.emmc,
                                    Offset, Len, pBuf);
#endif
        default:
		    return HI_FAILURE;
	}
}

HI_S32 HI_Flash_GetInfo(HI_HANDLE hFlash, HI_Flash_InterInfo_S *pFlashInfo )
{
	HI_FLASH_HANDLE_S *flash_handle = HI_FLASH_HANDLE_TO_STRUCT(hFlash);
#if defined(CONFIG_GENERIC_SF) || defined(CONFIG_GENERIC_NAND)
	struct mtd_info_ex * pMtd_Info_Ex = NULL;
#endif

#ifdef CONFIG_GENERIC_NAND
	nand_info_t *nand = (nand_info_t *) flash_handle->u.nand->nand;
#endif
	switch (flash_handle->type)
	{
#ifdef CONFIG_GENERIC_SF
        case HI_FLASH_TYPE_SPI_0:
    		pMtd_Info_Ex = get_spiflash_info();
            if (NULL != pMtd_Info_Ex)
            {
                pFlashInfo->TotalSize = pMtd_Info_Ex->chipsize;
                pFlashInfo->OobSize = pMtd_Info_Ex->oobsize;
            }
            else
            {
        		pFlashInfo->TotalSize = 0;
                pFlashInfo->OobSize = 0;
            }

    		pFlashInfo->PartSize = flash_handle->u.spiflash->length;
    		pFlashInfo->BlockSize = flash_handle->u.spiflash->erasesize;
    		pFlashInfo->PageSize = 0;
    		pFlashInfo->fd = 0;
              pFlashInfo->FlashType = HI_FLASH_TYPE_SPI_0;
    		break;
#endif

#ifdef CONFIG_GENERIC_NAND
        case HI_FLASH_TYPE_NAND_0:
            pMtd_Info_Ex = get_nand_info();
            if (NULL != pMtd_Info_Ex)
            {
                pFlashInfo->TotalSize = pMtd_Info_Ex->chipsize;
                pFlashInfo->OobSize = pMtd_Info_Ex->oobused;
            }
            else
            {
                pFlashInfo->TotalSize = 0;
                pFlashInfo->OobSize = 0;
            }
    		pFlashInfo->OpenAddr = flash_handle->u.nand->address;
    		pFlashInfo->OpenLeng = flash_handle->u.nand->length;

    		pFlashInfo->PartSize = flash_handle->u.nand->length;
    		pFlashInfo->BlockSize = nand->erasesize;
    		pFlashInfo->PageSize = nand->writesize;
    		pFlashInfo->fd = 0;
              pFlashInfo->FlashType = HI_FLASH_TYPE_NAND_0;
    		break;
#endif

#ifdef CONFIG_GENERIC_MMC
         case HI_FLASH_TYPE_EMMC_0:
             pFlashInfo->TotalSize = ((struct mmc *)(flash_handle->u.emmc->mmc))->capacity;
             pFlashInfo->OobSize = 0;
             pFlashInfo->PartSize = flash_handle->u.emmc->length;
             pFlashInfo->BlockSize = (flash_handle->u.emmc->blocksize * 16);
             pFlashInfo->PageSize = 0;
             pFlashInfo->fd = 0;
             pFlashInfo->OpenAddr  = flash_handle->u.emmc->address;
             pFlashInfo->pFlashopt = NULL;
             pFlashInfo->FlashType = HI_FLASH_TYPE_EMMC_0;
             break;
        case HI_FLASH_TYPE_SD:
             pFlashInfo->TotalSize = ((struct mmc *)(flash_handle->u.emmc->mmc))->capacity;
             pFlashInfo->OobSize = 0;
             pFlashInfo->PartSize = flash_handle->u.emmc->length;
             pFlashInfo->BlockSize = (flash_handle->u.emmc->blocksize * 16);
             pFlashInfo->PageSize = 0;
             pFlashInfo->fd = 0;
             pFlashInfo->OpenAddr  = flash_handle->u.emmc->address;
             pFlashInfo->pFlashopt = NULL;
             pFlashInfo->FlashType = HI_FLASH_TYPE_SD;
             break;
#endif
        default:
    		return HI_FAILURE;
    		break;
	}

	return HI_SUCCESS;
}

