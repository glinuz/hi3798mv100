/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_go_common.c
Version             : Initial Draft
Author              :
Created             : 2016/01/12
Description         :
                      CNcomment: 开机logo公共实现文件 CNend\n
Function List   :
History         :
Date                        Author                  Modification
2016/01/12                  y00181162               Created file
******************************************************************************/


/*********************************add include here******************************/

#include "hi_go_common.h"
#include "hi_common.h"
#include "hi_drv_pdm.h"

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/



/**********************       API realization       **************************/

/*****************************************************************************
* func            : HI_GFX_LOGO_RESERVE_Alloc
* description     : 分配保留内存
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_CHAR* HI_GFX_LOGO_RESERVE_Alloc(HI_U32 u32Size,HI_CHAR* pModuleName)
{
    HI_S32 Ret = HI_SUCCESS;
    MMZ_BUFFER_S psMBuf;

    Ret = HI_DRV_PDM_AllocReserveMem(pModuleName, u32Size, &psMBuf.u32StartPhyAddr);
    if (HI_SUCCESS != Ret)
    {
        return NULL;
    }
    return (HI_CHAR*)psMBuf.u32StartPhyAddr;
}

/*****************************************************************************
* func            : HI_GFX_LOGO_MMZ_Alloc
* description     : 分配连续的物理地址，boot下连续地址也可以做虚拟地址，可直接读写
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_CHAR* HI_GFX_LOGO_MMZ_Alloc(HI_U32 u32Size,HI_CHAR* pModuleName)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MMZ_BUFFER_S psMBuf;
#if 0
    HI_GFX_Print("\n=================================================\n");
    HI_GFX_Print("pModuleName = %s\n",pModuleName);
    HI_GFX_Print("u32Size     = %d\n",u32Size);
    HI_GFX_Print("=================================================\n");
#endif
    if (0 == u32Size)
    {
        return NULL;
    }

    /** 这种内存在内核起来之后会被回收，定位问题使用保留内存 **/
    //s32Ret = HI_DRV_PDM_AllocReserveMem("decode-buffer", u32Size, &psMBuf.u32StartPhyAddr);
    s32Ret = HI_MEM_Alloc(&psMBuf.u32StartPhyAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        return NULL;
    }

    return (HI_CHAR*)psMBuf.u32StartPhyAddr;
}

/*****************************************************************************
* func            : HI_GFX_LOGO_MMZ_Free
* description     : 释放地址
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_VOID HI_GFX_LOGO_MMZ_Free(HI_CHAR* pPhyAddr)
{
     return;
}
/*****************************************************************************
* func            : HI_GFX_LOGO_Malloc
* description     : 使用malloc分配内存，使用完释放给其它功能使用,小内存
                    使用该接口比如handle等，减少内存碎片
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_CHAR* HI_GFX_LOGO_Malloc(HI_U32 u32Size,HI_CHAR* pModuleName)
{
#if 0
    HI_GFX_Print("\n=================================================\n");
    HI_GFX_Print("pModuleName = %s\n",pModuleName);
    HI_GFX_Print("u32Size     = %d\n",u32Size);
    HI_GFX_Print("=================================================\n");
#endif
    if (0 == u32Size)
    {
        return NULL;
    }
    return (HI_CHAR*)malloc(u32Size);
}

/*****************************************************************************
* func            : HI_GFX_LOGO_MMZ_Free
* description     : 释放地址
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_VOID HI_GFX_LOGO_Free(HI_CHAR* pPhyAddr)
{
    if (NULL != pPhyAddr)
    {
        free(pPhyAddr);
    }
    return;
}
