#include <flash.h>
#include <hi_flash.h>
#include <hi_debug.h>
#include <malloc.h>
#include <string.h>

#define HI_FLASH_HANDLE_TO_STRUCT(handle)       ((struct flash_intf_t * )(handle))
#define HI_FLASH_STRUCT_TO_HANDLE(handle)       ((HI_HANDLE)(handle))

HI_HANDLE HI_Flash_OpenByName(HI_CHAR *pPartitionName)
{
	HI_S32 index = 0;
	HI_HANDLE hFlashHandle = HI_INVALID_HANDLE;

	for(index = 0; index < HI_FLASH_TYPE_BUTT; index++)
	{
		hFlashHandle = HI_Flash_OpenByTypeAndName((HI_FLASH_TYPE_E)index, pPartitionName);
		if(HI_INVALID_HANDLE != hFlashHandle)
		{
			return hFlashHandle;
		}
	}
	return HI_INVALID_HANDLE;
}

HI_HANDLE HI_Flash_OpenByTypeAndName(HI_FLASH_TYPE_E enFlashType, HI_CHAR *pPartitionName)
{
	struct flash_intf_t *flash_handle;

	flash_handle = (struct flash_intf_t * )flash_open_part(enFlashType, pPartitionName);
	if (!flash_handle) {
		return HI_INVALID_HANDLE;
	}

	return HI_FLASH_STRUCT_TO_HANDLE(flash_handle);
}

HI_HANDLE HI_Flash_OpenByTypeAndAddr(HI_FLASH_TYPE_E enFlashType, HI_U64 u64Address, HI_U64 u64Len)
{
    struct flash_intf_t *flash_handle;

    if((enFlashType == HI_FLASH_TYPE_BUTT) || (u64Len == 0))
    {
        return HI_INVALID_HANDLE;
    }

    flash_handle = (struct flash_intf_t * )flash_open_range(enFlashType, u64Address, u64Len);
    if (!flash_handle) {
		return HI_INVALID_HANDLE;
    }

    return HI_FLASH_STRUCT_TO_HANDLE(flash_handle);
}

HI_HANDLE HI_Flash_Open(HI_FLASH_TYPE_E enFlashType, HI_CHAR *pPartitionName, HI_U64 u64Address, HI_U64 u64Len)
{
	if(HI_NULL != pPartitionName) {
		if( enFlashType == HI_FLASH_TYPE_BUTT)
			return HI_Flash_OpenByName(pPartitionName);
		else
			return HI_Flash_OpenByTypeAndName(enFlashType, pPartitionName);
	} else
		return HI_Flash_OpenByTypeAndAddr(enFlashType, u64Address, u64Len);
}

HI_S32 HI_Flash_Close(HI_HANDLE hFlash)
{
    struct flash_intf_t *flash_handle = HI_FLASH_HANDLE_TO_STRUCT(hFlash);

    flash_close(flash_handle);
   
    return HI_SUCCESS;
}

HI_S32 HI_Flash_Erase(HI_HANDLE hFlash, HI_U64 u64Offset, HI_U64 u64Len)
{
    struct flash_intf_t *flash_handle = HI_FLASH_HANDLE_TO_STRUCT(hFlash);

    return flash_erase(flash_handle, u64Offset, u64Len);
}

static HI_S32 HI_Adpt_Nand_read(HI_HANDLE hFlash, HI_U64 u64Offset, HI_U8 *pBuf,
			    HI_U32 u32Len, HI_U32 u32Flags)
{
	HI_U32 u32Ret = 0;
	HI_U32 u32ReadLength;
	HI_U32 u32PageSize;
	HI_U32 u32PageOffset;
	HI_U32 u32PageOOBOffset;
	HI_U32 u32ReadLenWithoutOOB;
	HI_U32 u32ReadLenWithOOB;
	HI_U32 u32PageNumber;
	HI_CHAR *pucTmpBuff = NULL;

	HI_S32 s32Withoob = u32Flags & HI_FLASH_RW_FLAG_WITH_OOB;
	struct flash_intf_t *flash_handle = HI_FLASH_HANDLE_TO_STRUCT(hFlash);
	struct flash_info_t *flash_info = (struct flash_info_t *)flash_handle->info;

	if (s32Withoob) {
		u32PageSize = flash_info->pagesize;
		u32PageNumber = u32Len / (flash_info->pagesize + flash_info->oobused);
		u32ReadLenWithoutOOB = u32PageNumber * flash_info->pagesize;
		u32PageOOBOffset = u32Len % (flash_info->pagesize + flash_info->oobused);
		u32ReadLenWithOOB = u32PageNumber * (flash_info->pagesize + flash_info->oobused);

		if (u32PageOOBOffset) {
			HI_DBG_PRINT(HI_ID_FLASH, "HI_Adpt_Nand_read:Read length is not aligned with (pagesize + oobsize), "
					 "request read length: 0x%x\n"
					 "pagesize: 0x%x, oobsize: 0x%x. "
					 "Real read length: 0x%x!\n",
					 u32Len,
					 flash_info->pagesize,
					 flash_info->oobused,
					 u32ReadLenWithOOB);
		}
		if (u32PageNumber) {
			u32Ret = flash_read(flash_handle, u64Offset, u32ReadLenWithoutOOB, (char *)pBuf, s32Withoob);
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
			if( NULL ==(pucTmpBuff = (HI_CHAR*)malloc(u32PageSize + flash_info->oobused))) {
				HI_ERR_PRINT(HI_ID_FLASH, "Failed to allocate memory.");
				return HI_FAILURE;
			}

			u32Ret = flash_read(flash_handle, u64Offset, u32PageSize, pucTmpBuff, s32Withoob);
			if( 0 >= u32Ret) {
				free(pucTmpBuff);
				return HI_FAILURE;
			}

			memcpy((pBuf + u32ReadLength), pucTmpBuff, u32PageOOBOffset);
			free(pucTmpBuff);
		}

		return u32Len;
	} else {
		u32PageSize = flash_info->pagesize;
		u32PageOffset = u32Len & (u32PageSize - 1);

		u32ReadLength = u32Len - u32PageOffset;
		if( u32ReadLength ) {
			u32Ret = flash_read(flash_handle, u64Offset, u32ReadLength, (char *)pBuf, 0);

			if( 0 >= u32Ret)
				return HI_FAILURE;
			else if (u32ReadLength != u32Ret)
				return u32Ret;
		}

		u64Offset += u32ReadLength;
		u32ReadLength = u32Ret;

		if( u32PageOffset ) {
			if( NULL ==(pucTmpBuff = (HI_CHAR*)malloc(u32PageSize))) {
				HI_ERR_PRINT(HI_ID_FLASH, "Failed to allocate memory.");
				return HI_FAILURE;
			}

			u32Ret = flash_read(flash_handle, u64Offset, u32PageSize, pucTmpBuff, 0);
			if( 0 >= u32Ret) {
				free(pucTmpBuff);
				return HI_FAILURE;
			}

			memcpy((pBuf + u32ReadLength), pucTmpBuff, u32PageOffset);
			free(pucTmpBuff);
		}

		return u32Len;
	}
}

HI_S32 HI_Flash_Read(HI_HANDLE hFlash, HI_U64 u64Offset, HI_U8 *pBuf,
                     HI_U32 u32Len, HI_U32 u32Flags)
{
	struct flash_intf_t *flash_handle = HI_FLASH_HANDLE_TO_STRUCT(hFlash);
	struct flash_info_t *flash_info = (struct flash_info_t *)flash_handle->info;
	if (flash_info->type == FT_NAND)
		return HI_Adpt_Nand_read(hFlash, u64Offset, (HI_U8 *)pBuf, (uint64)u32Len, u32Flags);

	return flash_read(flash_handle, u64Offset, (uint64)u32Len, (char *)pBuf, u32Flags);
}

HI_S32 HI_Flash_Write(HI_HANDLE hFlash, HI_U64 u64Offset,
                      HI_U8 *pBuf, HI_U32 u32Len, HI_U32 u32Flags)
{
	struct flash_intf_t *flash_handle = HI_FLASH_HANDLE_TO_STRUCT(hFlash);

	return flash_write(flash_handle, u64Offset, (uint64)u32Len, (char *)pBuf, u32Flags);
}

HI_S32 HI_Flash_GetInfo(HI_HANDLE hFlash, HI_Flash_InterInfo_S *pFlashInfo )
{

	struct flash_intf_t *flash_handle = HI_FLASH_HANDLE_TO_STRUCT(hFlash);

	pFlashInfo->TotalSize = flash_handle->info->chipsize;
	pFlashInfo->OpenAddr = flash_handle->from;
	pFlashInfo->OpenLeng = flash_handle->length;
	pFlashInfo->PartSize = flash_handle->length;
	pFlashInfo->BlockSize = flash_handle->info->blocksize;
	pFlashInfo->PageSize = flash_handle->info->pagesize;
	pFlashInfo->fd = 0;
    pFlashInfo->FlashType = (HI_FLASH_TYPE_E)flash_handle->info->type;
	return HI_SUCCESS;
}
