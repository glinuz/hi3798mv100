/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : unf_jpge.c
Version             : Initial Draft
Author              :
Created             : 2016/03/03
Description         :
                      CNcomment:  CNend\n
Function List   :


History         :
Date                        Author                  Modification
2016/03/03                  y00181162               Created file
******************************************************************************/


/*********************************add include here******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/types.h>

#include "hi_debug.h"
#include "hi_jpge_api.h"
#include "drv_jpge_ioctl.h"
#include "hi_jpge_errcode.h"

#ifdef HI_SMMU_SUPPORT
#include "mpi_mmz.h"
#endif

/***************************** Macro Definition ******************************/

#define JPGE_CHECK_FD() \
    do {                                         \
        if (-1 == g_s32JPGEFd)                    \
        {                                        \
            return HI_ERR_JPGE_DEV_NOT_OPEN;      \
        }                                        \
    } while (0)




/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/

static const char *g_pszJPGEDevName = "/dev/hi_jpge";

static HI_S32 g_s32JPGEFd = -1;              /* jpge device handle */

static HI_S32 g_s32JPGERef = 0;



/********************** API forward declarations     **************************/


/********************** API forward release          **************************/


#ifdef HI_SMMU_SUPPORT
/***************************************************************************
* func          : HI_JPGE_FreeMem
* description   : free the mem that has alloced
                  CNcomment: 释放分配过的内存 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static HI_S32 HI_JPGE_FreeMem(HI_U32 u32PhyAddr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    if (0 == u32PhyAddr)
    {
        return HI_FAILURE;
    }
    s32Ret = HI_MPI_MMZ_Delete(u32PhyAddr);
    return s32Ret;
}


/***************************************************************************
* func          : HI_JPGE_AllocMem
* description   : alloc the mem that need
                  CNcomment: 分配需要的内存 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static HI_U32 HI_JPGE_AllocMem(HI_U32 u32Size, HI_U32 u32Align,HI_CHAR *pName)
{
    HI_U32 u32PhyAddr = 0;
    if (0 == u32Size)
    {
       return 0;
    }
    u32PhyAddr = HI_MPI_MMZ_New(u32Size, u32Align,NULL,pName);
    return u32PhyAddr;
}

/***************************************************************************
* func          : HI_JPGE_Map
* description   :
                  CNcomment: 映射不带cache的虚拟内存 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static HI_VOID *HI_JPGE_Map(HI_U32 u32PhyAddr)
{
    if (0 == u32PhyAddr)
    {
        return NULL;
    }
    return HI_MPI_MMZ_Map(u32PhyAddr, HI_FALSE);
}

/***************************************************************************
* func          : HI_JPGE_Unmap
* description   : un map phy ddr
                  CNcomment: 逆映射物理地址 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static HI_S32 HI_JPGE_Unmap(HI_U32 u32PhyAddr)
{
    if (0 == u32PhyAddr)
    {
        return HI_FAILURE;
    }
    return HI_MPI_MMZ_Unmap(u32PhyAddr);
}
#endif

/***************************************************************************
* func          : HI_JPGE_Open
* description   : api open jpge dev call this function
                  CNcomment: 打开设备文件 CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_S32 HI_JPGE_Open(HI_VOID)
{
#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100)
    return HI_ERR_JPGE_UNSUPPORT;
#endif
    if (-1 != g_s32JPGEFd)
    {
        g_s32JPGERef++;
        return HI_SUCCESS;
    }

    g_s32JPGEFd = open(g_pszJPGEDevName, O_RDWR, 0);
    if (-1 == g_s32JPGEFd)
    {
        return HI_ERR_JPGE_DEV_OPEN_FAILED;
    }

    g_s32JPGERef++;

    return HI_SUCCESS;
}

/***************************************************************************
* func          : HI_JPGE_Close
* description   : close open jpge dev call this function
                  CNcomment: 关闭jpge编码设备 CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_VOID HI_JPGE_Close(HI_VOID)
{
#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100)
    return;
#endif
    if (-1 == g_s32JPGEFd)
    {
        return;
    }
    g_s32JPGERef--;

    if (g_s32JPGERef > 0)
    {
        return ;
    }
    else
    {
        g_s32JPGERef = 0;
    }

    close(g_s32JPGEFd);

    g_s32JPGEFd = -1;

    return;

}


/***************************************************************************
* func          : HI_JPGE_Create
* description   : create jpge encode
                  CNcomment: 创建jpeg编码器器 CNend\n
* param[out]    : *pEncHandle  CNcomment: 输出解码器句柄   CNend\n
* param[in]     : pEncCfg      CNcomment: 输入的编码信息   CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_S32 HI_JPGE_Create(HI_U32 *pEncHandle, const Jpge_EncCfg_S *pEncCfg )
{
    HI_S32 s32Ret          = HI_SUCCESS;
    HI_U32 u32EncHandle    = 0;
    Jpge_EncCfg_S stEncCfg = {0};

    JPGE_CHECK_FD();
    if ((NULL == pEncHandle) || (NULL == pEncCfg))
    {
        return HI_ERR_JPGE_NULL_PTR;
    }

    s32Ret = ioctl(g_s32JPGEFd, JPGE_CREATEHANDLE_CMD, &u32EncHandle);
    if(s32Ret < 0){
        return HI_FAILURE;
    }

    memcpy(&stEncCfg, pEncCfg,sizeof(Jpge_EncCfg_S));
    s32Ret = ioctl(g_s32JPGEFd, JPGE_CREATE_CMD, &stEncCfg);
    if(s32Ret < 0){
        return HI_FAILURE;
    }

    *pEncHandle = u32EncHandle;

    return s32Ret;

}

/***************************************************************************
* func          : HI_JPGE_Encode
* description   : begin to encode
                  CNcomment: 开始编码 CNend\n
* param[in]     : EncHandle  CNcomment: 解码器句柄       CNend\n
* param[in]     : pEncIn     CNcomment: 输入的编码信息   CNend\n
* param[in]     : pEncOut    CNcomment: 输入的编码信息   CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_S32 HI_JPGE_Encode(HI_U32 EncHandle, const Jpge_EncIn_S *pEncIn, Jpge_EncOut_S *pEncOut)
{
    HI_S32 s32Ret = HI_SUCCESS;
    Jpge_EncInfo_S stEncInfo = {0};

#ifdef HI_SMMU_SUPPORT
    HI_U32 u32OutPhyBuf = 0;
    HI_U8* pu8OutVirBuf = NULL;
    HI_U32 u32TmpPhyBuf = 0;
    HI_U8* pu8TmpVirBuf = NULL;
#endif

    JPGE_CHECK_FD();
    if ((NULL == pEncIn) || (NULL == pEncOut))
    {
        return HI_ERR_JPGE_NULL_PTR;
    }

#ifdef HI_SMMU_SUPPORT
    u32TmpPhyBuf = pEncIn->BusOutBuf;
    pu8TmpVirBuf = pEncIn->pOutBuf;
#endif

    stEncInfo.EncHandle = (JPGE_HANDLE)EncHandle;

    memcpy(&stEncInfo.EncIn , pEncIn,sizeof(Jpge_EncIn_S));

#ifdef HI_SMMU_SUPPORT
    u32OutPhyBuf = HI_JPGE_AllocMem(pEncIn->OutBufSize,64,"JPGE-OUT-BUF");
    if (0 == u32OutPhyBuf)
    {
        return HI_FAILURE;
    }

    pu8OutVirBuf = (HI_U8*)HI_JPGE_Map(u32OutPhyBuf);
    if (NULL == pu8OutVirBuf)
    {
        HI_JPGE_FreeMem(u32OutPhyBuf);
        return HI_FAILURE;
    }
    stEncInfo.EncIn.BusOutBuf = u32OutPhyBuf;
    stEncInfo.EncIn.pOutBuf   = pu8OutVirBuf;
#endif

    s32Ret = ioctl(g_s32JPGEFd, JPGE_ENCODE_CMD, &stEncInfo);
    memcpy(pEncOut,&stEncInfo.EncOut ,sizeof(Jpge_EncOut_S));

#ifdef HI_SMMU_SUPPORT
    memcpy(pu8TmpVirBuf,pEncOut->pStream,pEncOut->StrmSize);
    pEncOut->BusStream = u32TmpPhyBuf;
    pEncOut->pStream   = pu8TmpVirBuf;
    HI_JPGE_Unmap(u32OutPhyBuf);
    HI_JPGE_FreeMem(u32OutPhyBuf);
#endif

    return s32Ret;

}

HI_S32 HI_JPGE_Destroy(HI_U32 EncHandle)
{
#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100)
    return HI_ERR_JPGE_UNSUPPORT;
#else
    return ioctl(g_s32JPGEFd, JPGE_DESTROY_CMD, &EncHandle);
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */
