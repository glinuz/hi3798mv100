/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_drv_mem.c
Version            : Initial Draft
Author            :
Created            : 2016/02/02
Description        :
Function List     :


History           :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef HI_BUILD_IN_BOOT

/*********************************add include here******************************/

#include "hi_jpeg_config.h"
#include "jpeg_drv_parse.h"
#include "jpeg_drv_mem.h"

#ifdef CONFIG_JPEG_OMX_FUNCTION

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/


/******************************* API forward declarations *******************/

/******************************* API realization *****************************/
HI_VOID* DRV_JPEG_AllocMem(HI_U32 *pPhyBuf)
{
    HI_U32 u32LayerSize = JPGD_DRV_STREAM_BUFFER;
    HI_CHAR* pDataVir   = NULL;
    HI_U32   u32DataPhy = 0;
    HI_BOOL bMemMMUType = HI_FALSE;

    u32DataPhy = HI_GFX_AllocMem("JPEG_STREAM_BUF", "iommu", u32LayerSize, &bMemMMUType);
    if (0 == u32DataPhy)
    {
        return NULL;
    }

#ifdef CONFIG_GFX_MMU_SUPPORT
    if (HI_TRUE != bMemMMUType)
    {
        HI_GFX_FreeMem(u32DataPhy, bMemMMUType);
        return NULL;
    }
#endif

    pDataVir = HI_GFX_MapCached(u32DataPhy,u32LayerSize, bMemMMUType);
    if (NULL == pDataVir)
    {
        HI_GFX_FreeMem(u32DataPhy, bMemMMUType);
        return NULL;
    }

    *pPhyBuf = u32DataPhy;

    return pDataVir;
}


HI_VOID DRV_JPEG_FremMem(HI_U32 PhyBuf, HI_CHAR* pVirBuf, HI_BOOL bSmmu)
{
    if (NULL != pVirBuf){
        HI_GFX_Unmap((HI_VOID*)pVirBuf, bSmmu);
    }

    if (0 != PhyBuf){
        HI_GFX_FreeMem(PhyBuf, bSmmu);
    }
}
#endif/** CONFIG_JPEG_OMX_FUNCTION **/

#endif/** HI_BUILD_IN_BOOT **/
