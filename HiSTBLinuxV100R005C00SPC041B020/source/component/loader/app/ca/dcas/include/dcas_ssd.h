/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : vmx_ssd.h
Version       : Initial Draft
Author        : Hisilicon hisecurity team
Created       : 2014/09/24
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#ifndef __VMX_SSD_H__
#define __VMX_SSD_H__

#include "hi_type.h"

HI_S32 DCAS_SSD_Init(HI_VOID);

HI_S32 DCAS_SSD_DeInit(HI_VOID);

HI_S32 DCAS_SSD_WholeImageVerfy
(
    HI_U8*      pu8ImageData,
    HI_U32      u32ImageSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize
);

HI_S32 DCAS_SSD_PartitionImageVerfy
(
    HI_U8*      pu8PartitionName,
    HI_U64      u64FlashStartAddr,
    HI_U8*      pu8ImageData,
    HI_U32      u32ImageSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize,
    HI_U32*     pu32ImageVersion,
    HI_BOOL*    pbNeedUpgrade
);


#endif /* __VMX_SSD_H__ */

