/******************************************************************************

Copyright (C), 2005-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : ca_flash.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2014-01-05
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef __CA_FLASH_H__
#define __CA_FLASH_H__

#include "hi_type.h"
#include "hi_flash.h"

HI_S32 CA_flash_GetPartitionSize(HI_CHAR *pPartionName, HI_U32 *size);
HI_S32 CA_flash_read(HI_CHAR *pPartionName, HI_U64 offset, HI_U32 bytes, HI_U8 * buf, HI_Flash_InterInfo_S *flashInfo);

#endif /*__CA_FLASH_H__*/
