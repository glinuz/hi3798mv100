/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_comm.c
  Version       : Initial Draft
  Author        : Hisilicon PQ software group
  Created       : 2013/06/04
  Description   : pq common define

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "hi_type.h"
#include "pq_comm.h"
#include "hi_common.h"
#include "hi_debug.h"
#include "hi_flash.h"
#include "hi_osal.h"
#include "hi_drv_pq.h"
#include "hi_mpi_pq.h"
#ifdef HI_SMMU_SUPPORT
#include "mpi_mmz.h"
#endif

#define PQ_SPECIAL_REGS_ADDR   0xfffe0000UL
#define PQ_REG_BASE_ADDR_MASK  0xffff0000UL

/* PQ文件描述符 */
HI_S32 g_s32PqFd = -1;
/* PQ 初始化标志位*/
HI_BOOL g_bPQInitFlag = HI_FALSE;

/*PQ BIN 文件内容*/
static PQ_PARAM_S sg_stPqParam;
static  HI_U32 sg_u32DemoFlags = PQ_DBG_DEMO_ALL_OFF;
static  HI_U32 sg_u32InMode  = PQ_DBG_SOURCE_MODE_NO;
static  HI_U32 sg_u32OutMode = PQ_DBG_OUTPUT_MODE_NO;

static  HI_U32 sg_u32ReadRegType = PQ_DBG_READ_TYPE_PHY;

static HI_VOID PQ_DRV_ChangTypeU32ToU16(HI_DBG_CSC_GAMM_PARA_S* pSrc, HI_PQ_CSC_GAMM_PARA_S* pDst)
{
    HI_U32 i;
    for (i = 0; i < sizeof(HI_PQ_CSC_GAMM_PARA_S) / sizeof(HI_U16); i++)
    {
        pDst->u16GammCoef[i] = pSrc->u32GammCoef[i];
    }

    return;
}

static HI_VOID PQ_DRV_ChangTypeU16ToU32(HI_PQ_CSC_GAMM_PARA_S* pSrc, HI_DBG_CSC_GAMM_PARA_S* pDst)
{
    HI_U32 i;
    for (i = 0; i < sizeof(HI_DBG_CSC_GAMM_PARA_S) / sizeof(HI_U32); i++)
    {
        pDst->u32GammCoef[i] = pSrc->u16GammCoef[i];
    }

    return;
}


HI_S32 PQ_DRV_SetHD(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Brightness = 0;
    HI_U32 u32Hue = 0;
    HI_U32 u32Contrast = 0;
    HI_U32 u32Saturation = 0;

    CHECK_PTR_REINT(pu8Buffer);
    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!\n");
        return HI_FAILURE;
    }

    /*Brightness*/
    u32Brightness = *(HI_U32*)pu8Buffer;
    //u32Brightness = (u32Brightness * 255 + 50) / 100;
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_HD_BRIGHTNESS, &u32Brightness);
    CHECK_RETURN(s32Ret);

    /*Hue*/
    u32Hue = *(HI_U32*)(pu8Buffer + 1 * sizeof(HI_U32));
    //u32Hue = (u32Hue * 255 + 50) / 100;
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_HD_HUE, &u32Hue);
    CHECK_RETURN(s32Ret);

    /*Contrast*/
    u32Contrast = *(HI_U32*)(pu8Buffer + 2 * sizeof(HI_U32));
    //u32Contrast = (u32Contrast * 255 + 50) / 100;
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_HD_CONTRAST, &u32Contrast);
    CHECK_RETURN(s32Ret);

    /*Saturation*/
    u32Saturation = *(HI_U32*)(pu8Buffer + 3 * sizeof(HI_U32));
    //u32Saturation = (u32Saturation * 255 + 50) / 100;
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_HD_SATURATION, &u32Saturation);

    return s32Ret;
}

HI_S32 PQ_DRV_GetHD(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Brightness = 0;
    HI_U32 u32Hue = 0;
    HI_U32 u32Contrast = 0;
    HI_U32 u32Saturation = 0;

    CHECK_PTR_REINT(pu8Buffer);
    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!\n");
        return HI_FAILURE;
    }

    /*Brightness*/
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_HD_BRIGHTNESS, &u32Brightness);
    CHECK_RETURN(s32Ret);
    //u32Brightness = (u32Brightness * 100 + 127) / 255;
    memcpy(pu8Buffer, &u32Brightness, sizeof(HI_U32));

    /*Hue*/
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_HD_HUE, &u32Hue);
    CHECK_RETURN(s32Ret);
    //u32Hue = (u32Hue * 100 + 127) / 255;
    memcpy(pu8Buffer + 1 * sizeof(HI_U32), &u32Hue, sizeof(HI_U32));

    /*Contrast*/
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_HD_CONTRAST, &u32Contrast);
    CHECK_RETURN(s32Ret);
    //u32Contrast = (u32Contrast * 100 + 127) / 255;
    memcpy(pu8Buffer + 2 * sizeof(HI_U32), &u32Contrast, sizeof(HI_U32));

    /*Saturation*/
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_HD_SATURATION, &u32Saturation);
    CHECK_RETURN(s32Ret);
    //u32Saturation = (u32Saturation * 100 + 127) / 255;
    memcpy(pu8Buffer + 3 * sizeof(HI_U32), &u32Saturation, sizeof(HI_U32));

    return s32Ret;
}

HI_S32 PQ_DRV_SetSD(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Brightness = 0;
    HI_U32 u32Hue = 0;
    HI_U32 u32Contrast = 0;
    HI_U32 u32Saturation = 0;

    CHECK_PTR_REINT(pu8Buffer);
    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!\n");
        return HI_FAILURE;
    }

    /*Brightness*/
    u32Brightness = *(HI_U32*)pu8Buffer;
    //u32Brightness = (u32Brightness * 255 + 50) / 100;
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_SD_BRIGHTNESS, &u32Brightness);
    CHECK_RETURN(s32Ret);

    /*Hue*/
    u32Hue = *(HI_U32*)(pu8Buffer + 1 * sizeof(HI_U32));
    //u32Hue = (u32Hue * 255 + 50) / 100;
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_SD_HUE, &u32Hue);
    CHECK_RETURN(s32Ret);

    /*Contrast*/
    u32Contrast = *(HI_U32*)(pu8Buffer + 2 * sizeof(HI_U32));
    //u32Contrast = (u32Contrast * 255 + 50) / 100;
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_SD_CONTRAST, &u32Contrast);
    CHECK_RETURN(s32Ret);

    /*Saturation*/
    u32Saturation = *(HI_U32*)(pu8Buffer + 3 * sizeof(HI_U32));
    //u32Saturation = (u32Saturation * 255 + 50) / 100;
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_SD_SATURATION, &u32Saturation);

    return s32Ret;
}


HI_S32 PQ_DRV_GetSD(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Brightness = 0;
    HI_U32 u32Hue = 0;
    HI_U32 u32Contrast = 0;
    HI_U32 u32Saturation = 0;

    CHECK_PTR_REINT(pu8Buffer);
    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!\n");
        return HI_FAILURE;
    }

    /*Brightness*/
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_SD_BRIGHTNESS, &u32Brightness);
    CHECK_RETURN(s32Ret);
    //u32Brightness = (u32Brightness * 100 + 127) / 255;
    memcpy(pu8Buffer, &u32Brightness, sizeof(HI_U32));

    /*Hue*/
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_SD_HUE, &u32Hue);
    CHECK_RETURN(s32Ret);
    //u32Hue = (u32Hue * 100 + 127) / 255;
    memcpy(pu8Buffer + 1 * sizeof(HI_U32), &u32Hue, sizeof(HI_U32));

    /*Contrast*/
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_SD_CONTRAST, &u32Contrast);
    CHECK_RETURN(s32Ret);
    //u32Contrast = (u32Contrast * 100 + 127) / 255;
    memcpy(pu8Buffer + 2 * sizeof(HI_U32), &u32Contrast, sizeof(HI_U32));

    /*Saturation*/
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_SD_SATURATION, &u32Saturation);
    CHECK_RETURN(s32Ret);
    //u32Saturation = (u32Saturation * 100 + 127) / 255;
    memcpy(pu8Buffer + 3 * sizeof(HI_U32), &u32Saturation, sizeof(HI_U32));

    return s32Ret;
}

HI_S32 PQ_DRV_SetDemo(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32DemoFlags = 0;
    HI_U32 u32On = 1;
    HI_U32 u32Off = 0;
    CHECK_PTR_REINT(pu8Buffer);

    u32DemoFlags = *(HI_U32*)pu8Buffer;

    if (PQ_DBG_DEMO_BUTT <= u32DemoFlags)
    {
        HI_ERR_PQ("PQ Demo[%d] error!\n", u32DemoFlags);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);

    if (PQ_DBG_DEMO_ALL_OFF == u32DemoFlags)
    {
        s32Ret = HI_MPI_PQ_SetDemo(HI_UNF_PQ_DEMO_ALL, u32Off);
    }
    else
    {
        s32Ret = HI_MPI_PQ_SetDemo((HI_UNF_PQ_DEMO_E)u32DemoFlags, u32On);
    }

    if (HI_FAILURE == s32Ret)
    {
        HI_MPI_PQ_DeInit();
        return HI_FAILURE;
    }
    sg_u32DemoFlags = u32DemoFlags;
    HI_INFO_PQ("Set Demo Flags:%d\n", u32DemoFlags);

    s32Ret = HI_MPI_PQ_DeInit();
    return s32Ret;
}

HI_S32 PQ_DRV_GetDemo(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);
    memcpy(pu8Buffer, &sg_u32DemoFlags, sizeof(HI_U32));
    HI_INFO_PQ("Get Demo Flags:%d\n", sg_u32DemoFlags);

    return HI_SUCCESS;
}

HI_S32 PQ_DRV_WriteACC(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;

    CHECK_PTR_REINT(pu8Buffer);

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!\n");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_ACC_LUT, (PQ_ACC_LUT_S*)pu8Buffer);

    return s32Ret;
}

HI_S32 PQ_DRV_ReadACC(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;

    CHECK_PTR_REINT(pu8Buffer);

    PQ_ACC_LUT_S stACCCoef;
    memset(&stACCCoef, 0 , sizeof(PQ_ACC_LUT_S));

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!\n");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_ACC_LUT, &stACCCoef);
    CHECK_RETURN(s32Ret);

    memcpy(pu8Buffer, &stACCCoef, sizeof(PQ_ACC_LUT_S));
    return s32Ret;
}


HI_S32 PQ_DRV_WriteDCI(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;

    CHECK_PTR_REINT(pu8Buffer);

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!\n");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_DCI, (HI_PQ_DCI_WGT_S*)pu8Buffer);

    return s32Ret;
}

HI_S32 PQ_DRV_ReadDCI(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;

    CHECK_PTR_REINT(pu8Buffer);

    HI_PQ_DCI_WGT_S stDciCoef;
    memset(&stDciCoef, 0 , sizeof(HI_PQ_DCI_WGT_S));

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!\n");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_DCI, &stDciCoef);
    CHECK_RETURN(s32Ret);

    memcpy(pu8Buffer, &stDciCoef, sizeof(HI_PQ_DCI_WGT_S));
    return s32Ret;
}

HI_S32 PQ_DRV_WriteDciBs(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;
    CHECK_PTR_REINT(pu8Buffer);

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!\n");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_DCI_BS, (HI_PQ_DCI_BS_LUT_S*)pu8Buffer);

    return s32Ret;
}

HI_S32 PQ_DRV_ReadDciBs(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;
    CHECK_PTR_REINT(pu8Buffer);

    HI_PQ_DCI_BS_LUT_S stDciCoef;
    memset(&stDciCoef, 0 , sizeof(HI_PQ_DCI_BS_LUT_S));

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_DCI_BS, &stDciCoef);
    CHECK_RETURN(s32Ret);

    memcpy(pu8Buffer, &stDciCoef, sizeof(HI_PQ_DCI_BS_LUT_S));
    return s32Ret;
}


HI_S32 PQ_DRV_WriteACM(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;

    MPI_ACM_PARAM_S* pstAcmTable = (MPI_ACM_PARAM_S*)pu8Buffer;

    HI_MPI_PQ_Init(HI_NULL);
    s32Ret = HI_MPI_PQ_SetAcmTable(u32RegAddr, pstAcmTable);
    HI_MPI_PQ_DeInit();

    return s32Ret;
}

HI_S32 PQ_DRV_ReadACM(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;
    MPI_ACM_PARAM_S* pstACMValue = NULL;

    s32Ret = HI_MPI_PQ_Init(HI_NULL);
    CHECK_RETURN(s32Ret);

    pstACMValue = (MPI_ACM_PARAM_S*)malloc(sizeof(MPI_COLOR_PARAM_S));
    if (NULL == pstACMValue)
    {
        HI_ERR_PQ("pstACMValue malloc error\n");
        return HI_FAILURE;
    }
    memset(pstACMValue, 0, sizeof(MPI_ACM_PARAM_S));

    s32Ret = HI_MPI_PQ_GetAcmTable(pstACMValue);
    if (HI_FAILURE == s32Ret)
    {
        HI_MPI_PQ_DeInit();
        free(pstACMValue);
        pstACMValue = HI_NULL;
        return HI_FAILURE;
    }

    if (sizeof(MPI_ACM_PARAM_S) == u32BufferLen)
    {
        memcpy(pu8Buffer, pstACMValue, sizeof(MPI_ACM_PARAM_S));
    }

    free(pstACMValue);
    pstACMValue = HI_NULL;

    s32Ret = HI_MPI_PQ_DeInit();
    return s32Ret;

}

HI_S32 PQ_DRV_SetSourceMode(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_U32 u32Mode;

    CHECK_PTR_REINT(pu8Buffer);
    u32Mode = *(HI_U32*)pu8Buffer;
    sg_u32InMode = u32Mode;

    HI_INFO_PQ("Set Source Mode:%d\n", u32Mode);

    return HI_SUCCESS;
}


HI_S32 PQ_DRV_GetSourceMode(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);
    memcpy(pu8Buffer, &sg_u32InMode, sizeof(HI_U32));
    HI_INFO_PQ("Get Source Mode:%d\n", sg_u32InMode);

    return HI_SUCCESS;
}

HI_S32 PQ_DRV_SetOutputMode(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_U32 u32Mode;

    CHECK_PTR_REINT(pu8Buffer);
    u32Mode = *(HI_U32*)pu8Buffer;
    sg_u32OutMode = u32Mode;

    HI_INFO_PQ("Set Output Mode:%d\n", u32Mode);

    return HI_SUCCESS;
}


HI_S32 PQ_DRV_GetOutputMode(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);
    memcpy(pu8Buffer, &sg_u32OutMode, sizeof(HI_U32));
    HI_INFO_PQ("Get Output Mode:%d\n", sg_u32OutMode);

    return HI_SUCCESS;
}

HI_S32 PQ_DRV_SetReadRegType(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_U32 u32Type;

    CHECK_PTR_REINT(pu8Buffer);
    u32Type = *(HI_U32*)pu8Buffer;
    sg_u32ReadRegType = u32Type;

    HI_INFO_PQ("Set Reg Type:%d\n", u32Type);

    return HI_SUCCESS;
}

HI_S32 PQ_DRV_GetReadRegType(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);
    memcpy(pu8Buffer, &sg_u32ReadRegType, sizeof(HI_U32));
    HI_INFO_PQ("Get Reg Type:%d\n", sg_u32ReadRegType);

    return HI_SUCCESS;
}

HI_S32 PQ_DRV_SetVGammaPara(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_PQ_CSC_GAMM_PARA_S stPara;
    HI_DBG_CSC_GAMM_PARA_S* pstPara = (HI_DBG_CSC_GAMM_PARA_S*)pu8Buffer;
    PQ_DRV_ChangTypeU32ToU16(pstPara, &stPara);


    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_GAMM_PARA, &stPara);
    return s32Ret;
}

HI_S32 PQ_DRV_GetVGammaPara(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_PQ_CSC_GAMM_PARA_S stPara;

    memset(&stPara, 0, sizeof(HI_PQ_CSC_GAMM_PARA_S));

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_GAMM_PARA, &stPara);
    CHECK_RETURN(s32Ret);

    PQ_DRV_ChangTypeU16ToU32(&stPara, (HI_DBG_CSC_GAMM_PARA_S*)pu8Buffer);
    //memcpy(pu8Buffer, &stPara, sizeof(HI_PQ_CSC_GAMM_PARA_S));
    return s32Ret;
}

HI_S32 PQ_DRV_SetVDeGammaPara(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_PQ_CSC_GAMM_PARA_S stPara;
    HI_DBG_CSC_GAMM_PARA_S* pstPara = (HI_DBG_CSC_GAMM_PARA_S*)pu8Buffer;

    PQ_DRV_ChangTypeU32ToU16(pstPara, &stPara);

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_DEGAMM_PARA, &stPara);
    return s32Ret;
}

HI_S32 PQ_DRV_GetVDeGammaPara(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_PQ_CSC_GAMM_PARA_S stPara;

    memset(&stPara, 0, sizeof(HI_PQ_CSC_GAMM_PARA_S));

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_DEGAMM_PARA, &stPara);
    CHECK_RETURN(s32Ret);

    PQ_DRV_ChangTypeU16ToU32(&stPara, (HI_DBG_CSC_GAMM_PARA_S*)pu8Buffer);
    //memcpy(pu8Buffer, &stPara, sizeof(HI_PQ_CSC_GAMM_PARA_S));

    return s32Ret;
}

HI_S32 PQ_DRV_SetGfxGammaPara(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_PQ_CSC_GAMM_PARA_S stPara;
    HI_DBG_CSC_GAMM_PARA_S* pstPara = (HI_DBG_CSC_GAMM_PARA_S*)pu8Buffer;

    PQ_DRV_ChangTypeU32ToU16(pstPara, &stPara);


    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_GFX_GAMM_PARA, &stPara);
    return s32Ret;
}

HI_S32 PQ_DRV_GetGfxGammaPara(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_PQ_CSC_GAMM_PARA_S stPara;

    memset(&stPara, 0, sizeof(HI_PQ_CSC_GAMM_PARA_S));

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_GFX_GAMM_PARA, &stPara);
    CHECK_RETURN(s32Ret);

    PQ_DRV_ChangTypeU16ToU32(&stPara, (HI_DBG_CSC_GAMM_PARA_S*)pu8Buffer);
    //memcpy(pu8Buffer, &stPara, sizeof(HI_PQ_CSC_GAMM_PARA_S));

    return s32Ret;
}

HI_S32 PQ_DRV_SetGfxDeGammaPara(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_PQ_CSC_GAMM_PARA_S stPara;
    HI_DBG_CSC_GAMM_PARA_S* pstPara = (HI_DBG_CSC_GAMM_PARA_S*)pu8Buffer;

    PQ_DRV_ChangTypeU32ToU16(pstPara, &stPara);

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_GFX_DEGAMM_PARA, &stPara);
    return s32Ret;
}

HI_S32 PQ_DRV_GetGfxDeGammaPara(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    CHECK_PTR_REINT(pu8Buffer);

    HI_S32 s32Ret = HI_FAILURE;
    HI_PQ_CSC_GAMM_PARA_S stPara;

    memset(&stPara, 0, sizeof(HI_PQ_CSC_GAMM_PARA_S));

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_GFX_DEGAMM_PARA, &stPara);
    CHECK_RETURN(s32Ret);

    PQ_DRV_ChangTypeU16ToU32(&stPara, (HI_DBG_CSC_GAMM_PARA_S*)pu8Buffer);
    //memcpy(pu8Buffer, &stPara, sizeof(HI_PQ_CSC_GAMM_PARA_S));

    return s32Ret;
}

HI_S32 PQ_DRV_SetHdrTmLut(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;

    CHECK_PTR_REINT(pu8Buffer);

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not  init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_HDR_TMLUT, (HI_PQ_HDR_TM_LUT_S*)pu8Buffer);

    return s32Ret;
}

HI_S32 PQ_DRV_GetHdrTmLut(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = HI_FAILURE;

    CHECK_PTR_REINT(pu8Buffer);

    HI_PQ_HDR_TM_LUT_S stTmLut;
    memset(&stTmLut, 0 , sizeof(HI_PQ_HDR_TM_LUT_S));

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not  init!");
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_HDR_TMLUT, &stTmLut);
    CHECK_RETURN(s32Ret);

    memcpy(pu8Buffer, &stTmLut, sizeof(HI_PQ_HDR_TM_LUT_S));
    return s32Ret;
}

/*PQ 设置虚拟寄存器对应的实现函数*/
PQ_DBG_REG_TYPE_S stPQSetRegTypeFun[] =
{
    {PQ_CMD_VIRTUAL_HD_CTRL,             PQ_DRV_SetHD},
    {PQ_CMD_VIRTUAL_SD_CTRL,             PQ_DRV_SetSD},
    {PQ_CMD_VIRTUAL_DEMO_CTRL,           PQ_DRV_SetDemo},
    {PQ_CMD_VIRTUAL_DCI_LUT,             PQ_DRV_WriteDCI},
    {PQ_CMD_VIRTUAL_DCI_BS_LUT,          PQ_DRV_WriteDciBs},
    {PQ_CMD_VIRTUAL_ACC_LUT,             PQ_DRV_WriteACC},
    {PQ_CMD_VIRTUAL_SOURCE_SELECT,       PQ_DRV_SetSourceMode},
    {PQ_CMD_VIRTUAL_OUT_MODE,            PQ_DRV_SetOutputMode},
    {PQ_CMD_VIRTUAL_READ_REGTYPE,        PQ_DRV_SetReadRegType},
    {PQ_CMD_VIRTUAL_GAMM_PARA,           PQ_DRV_SetVGammaPara},
    {PQ_CMD_VIRTUAL_DEGAMM_PARA,         PQ_DRV_SetVDeGammaPara},
    {PQ_CMD_VIRTUAL_GFX_GAMM_PARA,       PQ_DRV_SetGfxGammaPara},
    {PQ_CMD_VIRTUAL_GFX_DEGAMM_PARA,     PQ_DRV_SetGfxDeGammaPara},
    {PQ_CMD_VIRTUAL_HDR_TM_LUT,          PQ_DRV_SetHdrTmLut},


    //{PQ_CMD_BUTT, NULL},
};

/*PQ 获取 虚拟寄存器类型对应的实现函数*/
PQ_DBG_REG_TYPE_S stPQGetRegTypeFun[] =
{
    {PQ_CMD_VIRTUAL_HD_CTRL,             PQ_DRV_GetHD},
    {PQ_CMD_VIRTUAL_SD_CTRL,             PQ_DRV_GetSD},
    {PQ_CMD_VIRTUAL_DEMO_CTRL,           PQ_DRV_GetDemo},
    {PQ_CMD_VIRTUAL_ACC_LUT,             PQ_DRV_ReadACC},
    {PQ_CMD_VIRTUAL_DCI_LUT,             PQ_DRV_ReadDCI},
    {PQ_CMD_VIRTUAL_DCI_BS_LUT,          PQ_DRV_ReadDciBs},
    {PQ_CMD_VIRTUAL_SOURCE_SELECT,       PQ_DRV_GetSourceMode},
    {PQ_CMD_VIRTUAL_OUT_MODE,            PQ_DRV_GetOutputMode},
    {PQ_CMD_VIRTUAL_READ_REGTYPE,        PQ_DRV_GetReadRegType},
    {PQ_CMD_VIRTUAL_GAMM_PARA,           PQ_DRV_GetVGammaPara},
    {PQ_CMD_VIRTUAL_DEGAMM_PARA,         PQ_DRV_GetVDeGammaPara},
    {PQ_CMD_VIRTUAL_GFX_GAMM_PARA,       PQ_DRV_GetGfxGammaPara},
    {PQ_CMD_VIRTUAL_GFX_DEGAMM_PARA,     PQ_DRV_GetGfxDeGammaPara},
    {PQ_CMD_VIRTUAL_HDR_TM_LUT,          PQ_DRV_GetHdrTmLut},

    //{PQ_CMD_BUTT, NULL},
};


/* PQ 设置虚拟寄存器解析命令 */
HI_S32 PQ_DRV_ProcessSetRegType(HI_U32 u32RegType, HI_U8* pu8Buf , HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32TypeItem;
    HI_U32 u32TypeNum = 0;

    CHECK_PTR_REINT(pu8Buf);

    u32TypeNum = sizeof(stPQSetRegTypeFun) / sizeof(PQ_DBG_REG_TYPE_S);
    for (u32TypeItem = 0; u32TypeItem < u32TypeNum; u32TypeItem++)
    {
        if (u32RegType != stPQSetRegTypeFun[u32TypeItem].u32CmdRegType )
        {
            continue;
        }
        if (HI_NULL == stPQSetRegTypeFun[u32TypeItem].pfRegTypeFun)
        {
            continue;
        }

        s32Ret = stPQSetRegTypeFun[u32TypeItem].pfRegTypeFun(u32RegType, pu8Buf, u32BufferLen);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("stPQSetRegTypeFun failed! [%i]\n", s32Ret);
            return HI_FAILURE;
        }
        else
        {
            return HI_SUCCESS;
        }
    }

    HI_ERR_PQ("set PQRegType Function not found!\n");
    return HI_FAILURE;
}

/* PQ 获取虚拟寄存器解析命令 */
HI_S32 PQ_DRV_ProcessGetRegType(HI_U32 u32RegType, HI_U8* pu8Buf , HI_U32 u32BufferLen)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32TypeItem;
    HI_U32 u32TypeNum = 0;

    CHECK_PTR_REINT(pu8Buf);

    u32TypeNum = sizeof(stPQGetRegTypeFun) / sizeof(PQ_DBG_REG_TYPE_S);
    for (u32TypeItem = 0; u32TypeItem < u32TypeNum; u32TypeItem++)
    {
        if (u32RegType != stPQGetRegTypeFun[u32TypeItem].u32CmdRegType )
        {
            continue;
        }
        if (HI_NULL == stPQGetRegTypeFun[u32TypeItem].pfRegTypeFun)
        {
            continue;
        }

        s32Ret = stPQGetRegTypeFun[u32TypeItem].pfRegTypeFun(u32RegType, pu8Buf, u32BufferLen);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("stPQRegTypeFun failed! [%i]\n", s32Ret);
            return HI_FAILURE;
        }
        else
        {
            return HI_SUCCESS;
        }
    }

    HI_ERR_PQ("get PQRegType Function not found!\n");
    return HI_FAILURE;
}

static HI_S32 PQ_DRV_GetValueByBitRange(HI_S32 nOriginValue, HI_S32 bSigned, HI_U32 nBitStart, HI_U32 nBitEnd)
{
    HI_S32 nProcValue = 0;

    HI_S32 bits = nBitEnd - nBitStart + 1;
    if (32 == bits)
    {
        nProcValue = nOriginValue & 0xFFFFFFFF;
    }
    else
    {
        if (bSigned && bits > 1)
        {
            // Get the value
            nProcValue = nOriginValue;
            HI_S32 nMask = ((1 << bits) - 1) << nBitStart;
            HI_S32 nValue = nProcValue & nMask;
            nValue = nValue >> nBitStart;

            // Get the sign
            HI_S32 sign = nValue >> (bits - 1);
            if (1 == sign)
            {
                HI_S32 nNextMask = 0xFFFFFFFF << (nBitEnd - nBitStart);
                nProcValue = nNextMask  | nValue;
            }
            else
            {
                nProcValue = nValue;
            }
        }
        else
        {
            nProcValue = (nOriginValue >> nBitStart) & ((1 << bits) - 1);
        }
    }

    return nProcValue;
}

/*static HI_U32 PQ_DRV_ModifyBits(HI_U32 nOriginValue, HI_U32 nModifyValue, HI_U32 nBitStart, HI_U32 nBitEnd)
{
    HI_U32 nRealValue = 0;
    HI_U32 bits = nBitEnd - nBitStart + 1;
    HI_U32 nMask = 0;

    // Make mask
    if (32 == bits)
    {
        nMask = 0xFFFFFFFF;
    }
    else
    {
        nMask = ((1 << bits) - 1) << nBitStart;
    }
    nRealValue = nModifyValue << nBitStart;

    nOriginValue &= ~nMask;
    nRealValue &= nMask;
    nRealValue |= nOriginValue;

    return nRealValue;
}*/


/*--------------------------------------------------------------------------------*
Function        :HI_S32  PQ_DRV_Init()
Description     : 初始化PQ设备 模块
Input           :NA
Output
Return          :HI_SUCCESS, 初始化成功
                   :HI_FAILURE, 初始化失败
Others          :NA
*--------------------------------------------------------------------------------*/
HI_S32  PQ_DRV_Init(HI_VOID)
{
    if (HI_TRUE == g_bPQInitFlag)
    {
        return HI_SUCCESS;
    }

    /*pq设备初始化*/
    g_s32PqFd = open(VDP_PQ_NAME, O_RDWR);
    if (-1 == g_s32PqFd)
    {
        HI_ERR_PQ("open Pq device error !\n");
        return HI_FAILURE;
    }

    HI_SYS_Init();
    memset((HI_VOID*)&sg_stPqParam, 0, sizeof(PQ_PARAM_S));

    g_bPQInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

/*--------------------------------------------------------------------------------*
Function        :HI_S32  PQ_DRV_Deinit()
Description     : 去初始化PQ设备 模块
Input           :NA
Output          :NA
Return          :HI_SUCCESS, 去初始化成功
                   :HI_FAILURE, 去初始化失败
Others          :NA
*--------------------------------------------------------------------------------*/

HI_S32  PQ_DRV_Deinit(HI_VOID)
{
    HI_S32 s32Ret;
    s32Ret = HI_FAILURE;
    if (HI_FALSE == g_bPQInitFlag)
    {
        return HI_SUCCESS;
    }

    HI_SYS_DeInit();
    s32Ret  = close(g_s32PqFd);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("close PQ DEVICE error!\n");
        return HI_FAILURE;
    }

    g_bPQInitFlag = HI_FALSE;

    return HI_SUCCESS;
}

/*--------------------------------------------------------------------------------*
Function        :PQ_DRV_WriteReg(HI_U32 u32RegAddr, HI_U32 u32Value)
Description     : 写物理寄存器
Input           :u32RegAddr, 需要写入的寄存器地址u32Value,写入的值
Output          :NA
Return          :HI_SUCCESS
                   :HI_FAILURE
Others          :NA
*--------------------------------------------------------------------------------*/
HI_S32 PQ_DRV_WriteReg(HI_U32 u32RegAddr, HI_U32 u32Value, PQ_DBG_BIT_RANGE_S* pstBitRange)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_PQ_REGISTER_S stRegister = {0};

    if (HI_NULL == pstBitRange)
    {
        return HI_FAILURE;
    }

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    stRegister.u32RegAddr      = u32RegAddr;
    stRegister.u32Value        = u32Value;
    stRegister.u8Lsb           = pstBitRange->u32OrgBit;
    stRegister.u8Msb           = pstBitRange->u32EndBit;
    stRegister.u8SourceMode    = sg_u32InMode;
    stRegister.u8OutputMode    = sg_u32OutMode;

    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_S_REGISTER, &stRegister);
    return s32Ret;
}


/*--------------------------------------------------------------------------------*
Function        :PQ_DRV_ReadReg(HI_U32 u32RegAddr, HI_U32 u32Value)
Description     : 读物理寄存器
Input           :u32RegAddr, 需要读取的寄存器地址
Output          :pu32Value，读取的值
Return          :HI_SUCCESS
                   :HI_FAILURE
Others          :NA
*--------------------------------------------------------------------------------*/
HI_S32 PQ_DRV_ReadReg(HI_U32 u32RegAddr, HI_U32* pu32Value, PQ_DBG_BIT_RANGE_S* pstBitRange)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_PQ_REGISTER_S stRegister = {0};
    HI_U32 u32Value = 0;
    if ((HI_NULL == pu32Value) || (HI_NULL == pstBitRange))
    {
        return HI_FAILURE;
    }

    if ((sg_u32ReadRegType == PQ_DBG_READ_TYPE_PHY) &&
        ((u32RegAddr & PQ_REG_BASE_ADDR_MASK) != PQ_SPECIAL_REGS_ADDR))
    {
        s32Ret = HI_SYS_ReadRegister(u32RegAddr, &u32Value);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("HI_SYS_ReadRegister  is error!!");
            return HI_FAILURE;
        }

        *pu32Value = PQ_DRV_GetValueByBitRange(u32Value, pstBitRange->u32Signed, pstBitRange->u32OrgBit, pstBitRange->u32EndBit);
        return HI_SUCCESS;
    }

    if (g_bPQInitFlag == HI_FALSE)
    {
        HI_ERR_PQ("PQ not init!");
        return HI_FAILURE;
    }

    stRegister.u32RegAddr    = u32RegAddr;
    stRegister.u8Lsb         = pstBitRange->u32OrgBit;
    stRegister.u8Msb         = pstBitRange->u32EndBit;
    stRegister.u8SourceMode  = sg_u32InMode;
    stRegister.u8OutputMode  = sg_u32OutMode;
    s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_REGISTER, &stRegister);
    *pu32Value = stRegister.u32Value;
    return HI_SUCCESS;
}


/*--------------------------------------------------------------------------------*
   Function        :PQ_DRV_CheckCmd(HI_U32 u32RegAddr, HI_U32 u32Len)
   Description     : 校验读取参数组的长度是否合法
   Input             :u32RegAddr, 需要读取的虚拟寄存器地址(可理解为参数组ID)
   Input             :u32Len，读取的长度
   Return          :HI_SUCCESS
                      :HI_FAILURE
   Others          :NA
   *--------------------------------------------------------------------------------*/
HI_S32 PQ_DRV_CheckCmd(HI_U32 u32RegAddr, HI_U32 u32Len)
{
    HI_S32 s32Ret = HI_FAILURE;
    switch (u32RegAddr)
    {
        case PQ_CMD_VIRTUAL_HD_CTRL:
        case PQ_CMD_VIRTUAL_SD_CTRL:
        {
            if (u32Len != 4 * sizeof(HI_U32))
            {
                return HI_FAILURE;
            }
            break;
        }
        case PQ_CMD_VIRTUAL_DEMO_CTRL:
        case PQ_CMD_VIRTUAL_SOURCE_SELECT:
        case PQ_CMD_VIRTUAL_READ_REGTYPE:
        case PQ_CMD_VIRTUAL_OUT_MODE:
        {
            if (u32Len != sizeof(HI_U32))
            {
                return HI_FAILURE;
            }
            break;
        }
        case PQ_CMD_VIRTUAL_ACM_RECMD_LUT:
        case PQ_CMD_VIRTUAL_ACM_BLUE_LUT:
        case PQ_CMD_VIRTUAL_ACM_GREEN_LUT:
        case PQ_CMD_VIRTUAL_ACM_BG_LUT:
        case PQ_CMD_VIRTUAL_ACM_FLESH_LUT:
        case PQ_CMD_VIRTUAL_ACM_6BCOLOR_LUT:
        {
            if (u32Len != sizeof(MPI_ACM_PARAM_S))
            {
                return HI_FAILURE;
            }
            break;
        }
        case PQ_CMD_VIRTUAL_DCI_LUT:
        {
            if (u32Len != sizeof(HI_PQ_DCI_WGT_S))
            {
                return HI_FAILURE;
            }
            break;
        }
        case PQ_CMD_VIRTUAL_DCI_BS_LUT:
        {
            if (u32Len != sizeof(HI_PQ_DCI_BS_LUT_S))
            {
                return HI_FAILURE;
            }
            break;
        }
        case PQ_CMD_VIRTUAL_ACC_LUT:
        {
            if (u32Len != sizeof(PQ_ACC_LUT_S))
            {
                return HI_FAILURE;
            }
            break;
        }
        case PQ_CMD_VIRTUAL_HDR_TM_LUT:
        {
            if (u32Len != sizeof(HI_PQ_HDR_TM_LUT_S))
            {
                return HI_FAILURE;
            }

            break;
        }
        case PQ_CMD_VIRTUAL_BIN_FIXED:
        {
            //HI_INFO_PQ("u32Len:%d, sizeof(PQ_PARAM_S):%d\n", u32Len, sizeof(PQ_PARAM_S));
            break;
        }
        case PQ_CMD_VIRTUAL_BIN_IMPORT:
        case PQ_CMD_VIRTUAL_BIN_EXPORT:
        {
            if (u32Len != sizeof(PQ_PARAM_S))
            {
                return HI_FAILURE;
            }
            break;
        }
        case PQ_CMD_VIRTUAL_GAMM_PARA:
        case PQ_CMD_VIRTUAL_DEGAMM_PARA:
        case PQ_CMD_VIRTUAL_GFX_GAMM_PARA:
        case PQ_CMD_VIRTUAL_GFX_DEGAMM_PARA:
        {
            if (u32Len != sizeof(HI_DBG_CSC_GAMM_PARA_S))
            {
                return HI_FAILURE;
            }
            break;
        }

        default:
        {
            HI_ERR_PQ("Check  cmd error! unknown virtual address = 0x%x\n", u32RegAddr);
            return s32Ret;
        }


    }
    return HI_SUCCESS;
}


/*--------------------------------------------------------------------------------*
Function        :HI_S32  PQ_DRV_CreateBinFile(PQ_PARAM_S *pstPqParam)
Description     : 生成BIN文件，调试使用
Input           :stPqParam,bin文件内容
Output         :cb200_bin_file.bin
Return          :HI_SUCCESS, 初始化成功
                   :HI_FAILURE, 初始化失败
Others          :NA
*--------------------------------------------------------------------------------
HI_S32 PQ_DRV_CreateBinFile(PQ_PARAM_S* pstPqParam)
{
    char binfilename[256] = "CV200.bin";
    FILE* outfile = fopen(binfilename, "wb+");

    if ((FILE*)NULL == outfile)
    {
        HI_ERR_PQ("export file:creat bin file failed\r\n");
        return HI_FAILURE;
    }

    if (pstPqParam->stPQFileHeader.u32ParamSize != fwrite(pstPqParam, 1, pstPqParam->stPQFileHeader.u32ParamSize, outfile))
    {
        HI_ERR_PQ("export file:write bin file failed\r\n");
        fclose(outfile);
        return HI_FAILURE;
    }

    fclose(outfile);
    return HI_SUCCESS;

}
*/

HI_S32 PQ_DRV_GetSdkVersion(PQ_FILE_HEADER_S* pstPqFileHead)
{
    HI_U32 i = 0;
    HI_SYS_VERSION_S stVersion;

    if (HI_SUCCESS != HI_SYS_GetVersion(&stVersion))
    {
        HI_ERR_PQ("GetSdkVersion failed:get sdkversion failed\r\n");
        return HI_FAILURE;
    }

    if (HI_CHIP_TYPE_HI3716M == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3716M", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3716H == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3716H", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3716C == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3716C", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3720 == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3720", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3712 == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3712", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3718C == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3718C", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3718M == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3718M", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3719C == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3719C", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3719M == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3719M", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3798C == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3798C", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3798M == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3798M", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3796M == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3796M", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3796C == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3796C", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else if (HI_CHIP_TYPE_HI3798C_A == stVersion.enChipTypeSoft)
    {
        strncpy(pstPqFileHead->u8ChipName , "HI_CHIP_TYPE_HI3798C_A", sizeof(pstPqFileHead->u8ChipName) - 1);
    }
    else
    {
        HI_ERR_PQ("GetSdkVersion failed:unknown chipname,ChipTypeSoft = %d\r\n", stVersion.enChipTypeSoft);
        return HI_FAILURE;
    }

    while (i < strlen(stVersion.aVersion))
    {
        if (' ' == stVersion.aVersion[i])
        {
            stVersion.aVersion[i] = '\0';
            break;
        }
        i++;
    }

    strncpy(pstPqFileHead->u8SDKVersion, stVersion.aVersion, sizeof(pstPqFileHead->u8SDKVersion) - 1);
    strncpy(pstPqFileHead->u8Version, PQ_VERSION, sizeof(pstPqFileHead->u8Version) - 1);

    return HI_SUCCESS;
}

HI_VOID PQ_DRV_FillBinFileHead(PQ_PARAM_S* pstPqParam)
{
    HI_U32 u32CheckPos;
    HI_U32 u32CheckSize;
    HI_U32 u32CheckSum;

    u32CheckSum = 0;
    u32CheckPos = (HI_U8*) & (pstPqParam->stPQCoef) - (HI_U8*)pstPqParam;
    u32CheckSize = sizeof(PQ_PARAM_S);

    while (u32CheckPos < u32CheckSize)
    {
        u32CheckSum += *(HI_U8*)(((HI_U8*)pstPqParam) + u32CheckPos);
        u32CheckPos++;
    }

    pstPqParam->stPQFileHeader.u32FileCheckSum = u32CheckSum;
    pstPqParam->stPQFileHeader.u32ParamSize = u32CheckSize;
}

HI_S32 PQ_DRV_CheckImportBin(const HI_U32* pu32Data, HI_U32 u32Len)
{
    HI_U32 u32CheckSum = 0;
    HI_U32 u32CheckSize;
    HI_U32 u32CheckVar;
    PQ_FILE_HEADER_S stPqFileHeadTmp;
    PQ_PARAM_S* pstPqParam = (PQ_PARAM_S*)pu32Data;

    //begin to check len
    if (u32Len != sizeof(PQ_PARAM_S))
    {
        HI_ERR_PQ("ImportParaToFlash failed:length error\r\n");
        return HI_FAILURE;
    }

    //check checksum
    u32CheckVar = (HI_U8*) & (sg_stPqParam.stPQCoef) - (HI_U8*)&sg_stPqParam;
    u32CheckSize = sizeof(PQ_PARAM_S);

    while (u32CheckVar < u32CheckSize)
    {
        u32CheckSum += *(HI_U8*)(((HI_U8*)pstPqParam) + u32CheckVar);
        u32CheckVar++;
    }
    if (u32CheckSum != pstPqParam->stPQFileHeader.u32FileCheckSum)
    {
        HI_ERR_PQ("ImportParaToFlash failed:Checksum error,Calc check sum = %d, bin file check sum = %d\r\n", u32CheckSum, pstPqParam->stPQFileHeader.u32FileCheckSum);
        return HI_FAILURE;
    }

    //check version
    if (HI_SUCCESS != PQ_DRV_GetSdkVersion(&stPqFileHeadTmp))
    {
        HI_ERR_PQ("ImportParaToFlash failed:get sdkversion failed\r\n");
        return HI_FAILURE;
    }

    /*if (0 != strncmp(pstPqParam->stPQFileHeader.u8Version, PQ_VERSION, strlen(PQ_VERSION)))
    {
        HI_ERR_PQ("ImportParaToFlash failed:version mismatching\r\n");
        return HI_FAILURE;
    }*/

    if (0 != strncmp(stPqFileHeadTmp.u8ChipName, pstPqParam->stPQFileHeader.u8ChipName, strlen(pstPqParam->stPQFileHeader.u8ChipName)))
    {
        HI_ERR_PQ("ImportParaToFlash failed:chipname mismatching\r\n");
        return HI_FAILURE;
    }
    /*
        if (0 != strncmp(stPqFileHeadTmp.u8SDKVersion, pstPqParam->stPQFileHeader.u8SDKVersion, strlen(pstPqParam->stPQFileHeader.u8SDKVersion)))
        {
            HI_ERR_PQ("ImportParaToFlash failed:sdkversion mismatching\r\n");
            return HI_FAILURE;
        }
    */
    return HI_SUCCESS;

}



static HI_U32 str_to_flashsize(HI_CHAR* strsize)
{
    char* p, *q;
    char tmp[32];
    int size;
    p = strsize;
    q = strsize + strlen(strsize) - 1;

    if (strlen(strsize) <= 1)
    {
        return 0;
    }

    if (sizeof(tmp) < strlen(strsize))
    {
        return 0;
    }

    memset(tmp, 0x0, sizeof(tmp));
    memcpy(tmp, p, strlen(strsize) - 1);

    size = strtoul(tmp, HI_NULL, 10);
    if (*q == 'K' || *q == 'k')
    {
        size = size * 1024;
    }
    else if (*q == 'M' || *q == 'm')
    {
        size = size * 1024 * 1024;
    }
    else
    {
        size = 0;
    }

    return size;
}

HI_S32 PQ_DRV_GetFlashInfo(HI_CHAR* DataName, PQ_FLASH_INFO_S* pstInfo)
{
    HI_CHAR         Bootargs[512];
    FILE*            pf = HI_NULL;
    HI_CHAR*       p = HI_NULL, *q =  HI_NULL;
    HI_CHAR         tmp[32];
    HI_S32          ReadLen = 0;

    if ((HI_NULL == DataName) || (HI_NULL == pstInfo))
    {
        return HI_FAILURE;
    }

    if (strlen(DataName) >= sizeof(tmp))
    {
        return HI_FAILURE;
    }

    pf = fopen("/proc/cmdline", "r");
    if (HI_NULL == pf)
    {
        return HI_FAILURE;
    }

    memset(Bootargs, 0x0, 512);

    ReadLen = fread(Bootargs, sizeof(HI_CHAR), 512, pf);
    if (ReadLen <= 0)
    {
        fclose(pf);
        pf = HI_NULL;
        return HI_FAILURE;
    }

    fclose(pf);
    pf = HI_NULL;

    Bootargs[511] = '\0';

    HI_OSAL_Snprintf(tmp, sizeof(tmp), "(%s)", DataName);
    tmp[sizeof(tmp) - 1] = '\0';

    p = strstr((HI_CHAR*)Bootargs, tmp);
    if (HI_NULL != p)
    {
        for (q = p; q > Bootargs; q--)
        {
            if (*q == ',' || *q == ':')
            {
                break;
            }
        }

        memset(tmp, 0, sizeof(tmp));

        if ((p <= q) || ((HI_U32)(p - q - 1) >= sizeof(tmp) - 1 ))
        {
            return HI_FAILURE;
        }

        memcpy(tmp, q + 1, (HI_U32)(p - q - 1));
        tmp[sizeof(tmp) - 1] = '\0';

        memset(pstInfo->Name, 0x0, sizeof(pstInfo->Name));

        memcpy(pstInfo->Name, DataName, strlen(DataName));
        tmp[sizeof(pstInfo->Name) - 1] = '\0';
        pstInfo->u32Size = str_to_flashsize(tmp);
        pstInfo->u32Offset = 0;
        return HI_SUCCESS;
    }

    snprintf(tmp, sizeof(tmp), " %s", DataName);
    tmp[sizeof(tmp) - 1] = '\0';
    p = strstr((HI_CHAR*)Bootargs, tmp);
    if (HI_NULL == p)
    {
        return HI_FAILURE;
    }

    p = strstr(p, "=");
    if (HI_NULL == p)
    {
        return HI_FAILURE;
    }

    p++;

    q = strstr(p, ",");
    if (HI_NULL == q)
    {
        return HI_FAILURE;
    }

    memset(pstInfo->Name, 0x0, sizeof(pstInfo->Name));

    if ((q <= p) || ((HI_U32)(q - p ) >= sizeof(tmp) - 1))
    {
        return HI_FAILURE;
    }

    memcpy(pstInfo->Name, p, (HI_U32)(q - p));
    tmp[sizeof(pstInfo->Name) - 1] = '\0';

    p = q + 1;
    q = strstr(p, ",");
    if ((HI_NULL == q) || (q <= p) || ((HI_U32)(q - p ) >= sizeof(tmp) - 1))
    {
        return HI_FAILURE;
    }

    memset(tmp, 0, sizeof(tmp));

    memcpy(tmp, p, (HI_U32)(q - p));
    tmp[sizeof(tmp) - 1] = '\0';
    pstInfo->u32Offset = strtoul(tmp, HI_NULL, 16);

    p = q + 1;

    q = strstr(p, " ");
    if (HI_NULL == q)
    {
        Bootargs[511] = '\0';
        q = strstr(p, "\0");
    }

    if ((q <= p) || ((HI_U32)(q - p ) >= sizeof(tmp) - 1))
    {
        return HI_FAILURE;
    }

    memset(tmp, 0, sizeof(tmp));

    memcpy(tmp, p, (HI_U32)(q - p));
    tmp[sizeof(tmp) - 1] = '\0';
    pstInfo->u32Size = strtoul(tmp, HI_NULL, 16);

    return HI_SUCCESS;
}

HI_S32  PQ_DRV_SetPqParam(HI_VOID* pData, HI_U32 u32Size)
{
    HI_S32                      Ret;
    HI_HANDLE                   hFlash;
    HI_Flash_InterInfo_S        FlashInfo;
    HI_S32                      Size;
    PQ_FLASH_INFO_S            PqFlashInfo;
    HI_U32                      StartPos, EndPos;
    HI_U32*                    pMallocAddr;

    pMallocAddr = HI_NULL;
    Ret = PQ_DRV_GetFlashInfo(PQ_DEF_NAME, &PqFlashInfo);

    if (HI_SUCCESS != Ret)
    {
        HI_ERR_PQ("ERR: PQ_DRV_GetFlashInfo failed!");
        return Ret;
    }

    hFlash = HI_Flash_OpenByName(PqFlashInfo.Name);

    if (HI_INVALID_HANDLE == hFlash)
    {
        HI_ERR_PQ("ERR: HI_Flash_Open!");
        Ret = HI_FAILURE;
        goto ERR0;
    }

    Ret = HI_Flash_GetInfo(hFlash, &FlashInfo);

    if (HI_SUCCESS != Ret)
    {
        HI_ERR_PQ("ERR: HI_Flash_GetInfo!");
        Ret = HI_FAILURE;
        goto ERR1;
    }

    if (u32Size > PqFlashInfo.u32Size)
    {
        HI_ERR_PQ("HI_UNF_PDM_SetPqParam!,PqFlashInfo.u32Offset = %d,PqFlashInfo.u32Size = %d,FlashInfo.BlockSize = %d,readsize = %d\r\n", PqFlashInfo.u32Offset, PqFlashInfo.u32Size, FlashInfo.BlockSize, u32Size);
    }

    StartPos = PqFlashInfo.u32Offset - PqFlashInfo.u32Offset % FlashInfo.BlockSize;

    if (0 == (PqFlashInfo.u32Offset + u32Size) % FlashInfo.BlockSize)
    {
        EndPos = PqFlashInfo.u32Offset + u32Size;
    }
    else
    {
        EndPos = PqFlashInfo.u32Offset + u32Size + FlashInfo.BlockSize -
                 (PqFlashInfo.u32Offset + u32Size) % FlashInfo.BlockSize;
    }

    if (EndPos <= StartPos)
    {
        Ret = HI_FAILURE;
        goto ERR2;
    }

    if (HI_FLASH_TYPE_EMMC_0 != FlashInfo.FlashType)
    {
        Size = HI_Flash_Erase(hFlash, StartPos, EndPos - StartPos);

        if (Size <= 0)
        {
            HI_ERR_PQ("ERR: HI_Flash_Erase!");
            Ret = HI_FAILURE;
            goto ERR2;
        }
    }

    pMallocAddr = (HI_U32*)malloc(EndPos - StartPos);

    if (HI_NULL == pMallocAddr)
    {
        HI_ERR_PQ("ERR: Malloc fail!");
        Ret = HI_FAILURE;
        goto ERR2;
    }

    memset(pMallocAddr, 0 , (EndPos - StartPos));
    memcpy(pMallocAddr, pData, u32Size);

    Size = HI_Flash_Write(hFlash, StartPos, (HI_U8*)pMallocAddr, EndPos - StartPos, HI_FLASH_RW_FLAG_RAW);
    if (Size <= 0)
    {
        HI_ERR_PQ("ERR: HI_Flash_Write!");
        Ret = HI_FAILURE;
        goto ERR2;
    }

ERR2:
ERR1:
    HI_Flash_Close(hFlash);

    if (HI_NULL != pMallocAddr)
    {
        free(pMallocAddr);
    }

ERR0:
    return Ret;
}




/*--------------------------------------------------------------------------------*
Function        :PQ_DRV_FixParaToFlash(const HI_U32* pu32Data)
Description     : 固化BIN文件，将允许中的参数固化到FLASH中
Input             :pu32Data, PCTOOLS传递过来担版本号，参数调试着签名，版本描述，生成时间等数据
Return          :HI_SUCCESS
                   :HI_FAILURE
Others          :NA
*--------------------------------------------------------------------------------*/
HI_S32 PQ_DRV_FixParaToFlash(const HI_U32* pu32Data)
{
    HI_U32 u32CheckPos;
    HI_U32 u32CheckSize;
    HI_S32 s32Ret;
    HI_U32 u32CheckSum = 0;
    PQ_FILE_HEADER_S stPqFileHead;

    //---------------------------------------begin to get sdkversion-------------------------------//
    memset((HI_VOID*)&stPqFileHead, 0, sizeof(PQ_FILE_HEADER_S));

    if (HI_SUCCESS != PQ_DRV_GetSdkVersion(&stPqFileHead))
    {
        HI_ERR_PQ("FixParaToFlash failed:get sdkversion failed\r\n");
        return HI_FAILURE;
    }


    //---------------------------------------begin to calc head-------------------------------//*/
#if defined(PQ_ACC_SUPPORT)
    sg_stPqParam.stPQFileHeader.u32Offset[PQ_ALG_MODULE_DCI] = (HI_U8*)(&sg_stPqParam.stPQCoef.stACCCoef) - (HI_U8*)(&sg_stPqParam);
#else
    sg_stPqParam.stPQFileHeader.u32Offset[PQ_ALG_MODULE_DCI] = (HI_U8*)(&sg_stPqParam.stPQCoef.stDciCoef) - (HI_U8*)(&sg_stPqParam);
#endif
    sg_stPqParam.stPQFileHeader.u32Offset[PQ_ALG_MODULE_ACM] = (HI_U8*)(&sg_stPqParam.stPQCoef.stAcmCoef) - (HI_U8*)(&sg_stPqParam);
    sg_stPqParam.stPQFileHeader.u32PhyOffset                 = (HI_U8*)(sg_stPqParam.stPQPhyReg) - (HI_U8*)(&sg_stPqParam);

    stPqFileHead.u32ParamSize = sizeof(sg_stPqParam);
    u32CheckPos = (HI_U8*) & (sg_stPqParam.stPQCoef) - (HI_U8*)&sg_stPqParam;
    u32CheckSize = sizeof(sg_stPqParam);

    while (u32CheckPos < u32CheckSize)
    {
        u32CheckSum += *(HI_U8*)(((HI_U8*)&sg_stPqParam) + u32CheckPos);
        u32CheckPos++;
    }

    stPqFileHead.u32FileCheckSum = u32CheckSum;

    memcpy((HI_VOID*)stPqFileHead.u8Author, (HI_VOID*)((HI_U8*)pu32Data), sizeof(stPqFileHead.u8Author) - 1);
    memcpy((HI_VOID*)stPqFileHead.u8Desc, (HI_VOID*)((HI_U8*)pu32Data + STR_LEN_32), sizeof(stPqFileHead.u8Desc) - 1);
    memcpy((HI_VOID*)stPqFileHead.u8Time, (HI_VOID*)((HI_U8*)pu32Data + STR_LEN_32 + STR_LEN_1024), sizeof(stPqFileHead.u8Time) - 1);
    sg_stPqParam.stPQFileHeader = stPqFileHead;

    HI_INFO_PQ("------------------------------------------------------Fixe to flash--------------------------------------------\r\n");
    HI_INFO_PQ("------------------PQBin Size  = %d\r\n", sg_stPqParam.stPQFileHeader.u32ParamSize);
    HI_INFO_PQ("------------------Chip Name   = %s\r\n", sg_stPqParam.stPQFileHeader.u8ChipName);
    HI_INFO_PQ("------------------SDK Version = %s\r\n", sg_stPqParam.stPQFileHeader.u8SDKVersion);
    HI_INFO_PQ("------------------Author      = %s\r\n", sg_stPqParam.stPQFileHeader.u8Author);
    HI_INFO_PQ("------------------Description = %s\r\n", sg_stPqParam.stPQFileHeader.u8Desc);
    HI_INFO_PQ("------------------Time        = %s\r\n", sg_stPqParam.stPQFileHeader.u8Time);
    HI_INFO_PQ("------------------------------------------------------Fixe to flash--------------------------------------------\r\n");

    //fix bin file
    s32Ret = PQ_DRV_SetPqParam((HI_VOID*)&sg_stPqParam, sizeof(sg_stPqParam));
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("fixed bin file fail\r\n");
        return HI_FAILURE;
    }
    else
    {
        HI_INFO_PQ("fixed bin file success\r\n");
    }

    return HI_SUCCESS;

}

/*--------------------------------------------------------------------------------*
Function        :PQ_DRV_WriteToPqDriver(HI_U32 u32RegAddr, const HI_U32* pu32Value,HI_U32 u32Len)
Description     : 将用户配置的数据写入PQ DRIVER
Input             :u32RegAddr, 需要写入的虚拟寄存器地址(可理解为参数组ID)
Output          :pu32Value，写入的值
Return          :HI_SUCCESS
                   :HI_FAILURE
Others          :NA
*--------------------------------------------------------------------------------*/
HI_S32 PQ_DRV_WriteToPqDriver(HI_U32 u32RegAddr, const HI_U32* pu32Data, HI_U32 u32Len)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BinPhyAddr = 0;
    HI_U8* pVirAddr = NULL;

    if (PQ_DRV_CheckCmd(u32RegAddr, u32Len))
    {
        HI_ERR_PQ("Error,PQ_DRV_WriteGroupPara,len invalid\n");
        return HI_FAILURE;
    }

    if (u32RegAddr == PQ_CMD_VIRTUAL_BIN_FIXED)
    {
        s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_BIN_ADDR, &u32BinPhyAddr);

        if (s32Ret)
        {
            HI_ERR_PQ("Get pq bin ioctl failed\n");
            return s32Ret;
        }

        HI_INFO_PQ("Pq driver bin phy addr = %x\r\n", u32BinPhyAddr);

#ifdef HI_SMMU_SUPPORT
        pVirAddr = HI_MPI_SMMU_Map(u32BinPhyAddr, sizeof(PQ_PARAM_S));
#else
        pVirAddr = HI_MEM_Map(u32BinPhyAddr, sizeof(PQ_PARAM_S));
#endif

        if (HI_NULL == pVirAddr)
        {
            HI_ERR_PQ("Map failed\n");
            return HI_FAILURE;
        }

        memcpy((HI_VOID*)(&sg_stPqParam), (HI_VOID*)pVirAddr, sizeof(PQ_PARAM_S));

        s32Ret = PQ_DRV_FixParaToFlash(pu32Data);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("PQ_DRV_SetPqParam fail,Pq driver phy addr = %x, vir addr = %d\r\n", u32BinPhyAddr, pVirAddr);
            return HI_FAILURE;
        }

        memcpy((HI_VOID*)pVirAddr, (HI_VOID*)(&sg_stPqParam), sizeof(PQ_PARAM_S));

#ifdef HI_SMMU_SUPPORT
        HI_MPI_SMMU_Unmap(u32BinPhyAddr);
#else
        HI_MEM_Unmap(pVirAddr);
#endif

        pVirAddr = NULL;

        return HI_SUCCESS;
    }
    else if (u32RegAddr == PQ_CMD_VIRTUAL_BIN_IMPORT)
    {
        if (HI_SUCCESS != PQ_DRV_CheckImportBin(pu32Data, u32Len))
        {
            HI_ERR_PQ("The import bin is invalid\n");
            return HI_FAILURE;
        }

        s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_BIN_ADDR, &u32BinPhyAddr);

        if (s32Ret)
        {
            HI_ERR_PQ("Get pq bin io failed\n");
            return s32Ret;
        }

        HI_INFO_PQ("Pq driver bin phy addr = %x\r\n", u32BinPhyAddr);

#ifdef HI_SMMU_SUPPORT
        pVirAddr = HI_MPI_SMMU_Map(u32BinPhyAddr, sizeof(PQ_PARAM_S));
#else
        pVirAddr = HI_MEM_Map(u32BinPhyAddr, sizeof(PQ_PARAM_S));
#endif

        if (HI_NULL == pVirAddr)
        {
            HI_ERR_PQ("Map failed\n");
            return HI_FAILURE;
        }

        memcpy((HI_VOID*)pVirAddr, (HI_VOID*)pu32Data, sizeof(PQ_PARAM_S));

#ifdef HI_SMMU_SUPPORT
        HI_MPI_SMMU_Unmap(u32BinPhyAddr);
#else
        HI_MEM_Unmap(pVirAddr);
#endif

        pVirAddr = NULL;

        return HI_SUCCESS;
    }
    else
    {

        s32Ret = PQ_DRV_ProcessSetRegType(u32RegAddr, (HI_U8*)pu32Data, u32Len);
        if (s32Ret)
        {
            HI_ERR_PQ("ProcessSetRegType failed!\n");
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}


/*--------------------------------------------------------------------------------*
Function        :PQ_DRV_ReadFromPqDriver(HI_U32 u32RegAddr, HI_U32* pu32Value)
Description     : 读去PQ DRIVER中PQ bin值
Input           :u32RegAddr, 需要读取的虚拟寄存器地址(可理解为参数组ID)
Output          :pu32Value，读取的值
Return          :HI_SUCCESS
                :HI_FAILURE
Others          :NA
*--------------------------------------------------------------------------------*/
HI_S32 PQ_DRV_ReadFromPqDriver(HI_U32 u32RegAddr, HI_U32* pu32Data, HI_U32 u32Len)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BinPhyAddr = 0;
    HI_U8* pVirAddr = NULL;

    if (PQ_DRV_CheckCmd(u32RegAddr, u32Len))
    {
        HI_ERR_PQ("Error,PQ_DRV_ReadGroupPara,len invalid\n");
        return HI_FAILURE;

    }

    if (u32RegAddr == PQ_CMD_VIRTUAL_DRIVER_VERSION)
    {
        memcpy((HI_VOID*)pu32Data, (HI_VOID*)PQ_VERSION, strlen(PQ_VERSION));
    }
    else if (u32RegAddr == PQ_CMD_VIRTUAL_BIN_EXPORT)
    {
        s32Ret = ioctl(g_s32PqFd, HIIOC_PQ_G_BIN_ADDR, &u32BinPhyAddr);

        if (s32Ret)
        {
            HI_ERR_PQ("Get pq bin ioctl failed\n");
            return s32Ret;
        }

        HI_INFO_PQ("Pq driver bin phy addr = %x\r\n", u32BinPhyAddr);

#ifdef HI_SMMU_SUPPORT
        pVirAddr = HI_MPI_SMMU_Map(u32BinPhyAddr, sizeof(PQ_PARAM_S));
#else
        pVirAddr = HI_MEM_Map(u32BinPhyAddr, sizeof(PQ_PARAM_S));
#endif

        if (HI_NULL == pVirAddr)
        {
            HI_ERR_PQ("Map failed\n");
            return HI_FAILURE;
        }

        memcpy((HI_VOID*)(&sg_stPqParam), (HI_VOID*)pVirAddr, sizeof(PQ_PARAM_S));

        PQ_DRV_FillBinFileHead(&sg_stPqParam);
        memcpy((HI_VOID*)pu32Data, (HI_VOID*)&sg_stPqParam, sizeof(PQ_PARAM_S));


#ifdef HI_SMMU_SUPPORT
        HI_MPI_SMMU_Unmap(u32BinPhyAddr);
#else
        HI_MEM_Unmap(pVirAddr);
#endif

        pVirAddr = NULL;

        return HI_SUCCESS;
    }
    else
    {
        s32Ret = PQ_DRV_ProcessGetRegType(u32RegAddr, (HI_U8*)pu32Data, u32Len);
        if (s32Ret)
        {
            HI_ERR_PQ("ProcessSetRegType failed!\n");
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}


