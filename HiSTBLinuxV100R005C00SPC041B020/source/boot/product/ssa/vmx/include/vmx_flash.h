/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : vmx_flash.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef __VMX_FLASH_H__
#define __VMX_FLASH_H__

#include "hi_flash.h"

HI_S32 CA_flash_read(HI_CHAR *pPartionName, HI_U64 offset, HI_U32 bytes, HI_U8 * buf, HI_Flash_InterInfo_S *flashInfo);
HI_S32 CA_flash_readwithoob(HI_CHAR *pPartionName, HI_U64 offset, HI_U32 bytes, HI_U8 * buf);
HI_S32 CA_flash_write(HI_CHAR *pPartionName, HI_U64 offset, HI_U32 bytes, HI_U8 * buf);
HI_HANDLE CA_flash_open_addr(HI_U64 u64addr,HI_U64 u64Len);
HI_S32 CA_flash_read_addr(HI_U32 u32addr, HI_U32 u32Len, HI_U8 * buf, HI_Flash_InterInfo_S *flashInfo);
HI_S32 CA_flash_write_addr(HI_U32 u32addr, HI_U32 u32Len, HI_U8 * buf);
HI_S32 CA_flash_GetSize(HI_CHAR *pPartionName, HI_U32 *Size);
HI_S32 CA_flash_getinfo(HI_CHAR *pPartionName, HI_U32 *PartionSize, HI_U32 *BlockSize, HI_U32 *PageSize, HI_U32 *OobSize);
HI_S32 CA_flash_GetPageSize(HI_CHAR *PartitionSignName, HI_U32 *u32PageSize);

#endif /*__VMX_FLASH_H__*/
