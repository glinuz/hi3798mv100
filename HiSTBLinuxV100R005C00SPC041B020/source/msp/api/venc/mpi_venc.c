/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : mpi_venc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/03/31
  Description   :
  History       :
  1.Date        : 2010/03/31
    Author      : j00131665
    Modification: Created file

******************************************************************************/

#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>

#include "hi_mpi_venc.h"
#include "hi_error_mpi.h"
#include "hi_type.h"
#include "hi_mpi_mem.h"
#include "drv_venc_ioctl.h"
#include "hi_drv_struct.h"

#include "hi_mpi_vi.h"
#include "hi_mpi_win.h"
#include "hi_mpi_disp.h"
#include "mpi_mmz.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


#ifdef __VENC_S5V100_CONFIG__
#define VEDU_MMZ_ALIGN       (256)
#else
#define VEDU_MMZ_ALIGN       (64)
#endif

static HI_S32 g_VEncDevFd = -1;
static const HI_CHAR g_VEncDevName[] = "/dev/" UMAP_DEVNAME_VENC;
static pthread_mutex_t g_VEncMutex = PTHREAD_MUTEX_INITIALIZER;
static VENC_CHN_INFO_S s_asrVeChnInfo[VENC_MAX_CHN_NUM];
static VENC_CHN_VirAddr_S s_asrVeChnVirAddr[VENC_MAX_CHN_NUM];

#define HI_VENC_LOCK() (void)pthread_mutex_lock(&g_VEncMutex);
#define HI_VENC_UNLOCK() (void)pthread_mutex_unlock(&g_VEncMutex);

#define CHECK_VENC_INIT() \
    do {\
        HI_VENC_LOCK(); \
        if (g_VEncDevFd < 0)\
        {\
            HI_VENC_UNLOCK(); \
            return HI_ERR_VENC_NO_INIT; \
        } \
        HI_VENC_UNLOCK(); \
    } while (0)

#define CHECK_CHANNEL_EXIST(i,max,ChnHandle) \
    do {\
        for(i = 0; i < max; i++ )\
        {\
            if (s_asrVeChnInfo[i].handle == ChnHandle)\
                break;\
        }\
        if (max == i)\
            return HI_ERR_VENC_CHN_NOT_EXIST;\
    } while (0)

#define VENC_ALIGN_UP(val, align) ( (val+((align)-1))&~((align)-1) )

static HI_S32 MpiVencCheckPicSize(const HI_UNF_VENC_CHN_ATTR_S *pstAttr)
{	
    if ((pstAttr->u32Height < HI_VENC_MIN_HEIGTH) || (pstAttr->u32Width < HI_VENC_MIN_WIDTH))
    {
        HI_ERR_VENC("Picture Width(%u) or Heigth(%u) too small.\n", pstAttr->u32Width, pstAttr->u32Height);

        return HI_ERR_VENC_INVALID_PARA;
    }

    if ((pstAttr->u32Height > HI_VENC_MAX_HEIGTH) || (pstAttr->u32Width > HI_VENC_MAX_WIDTH))
    {
        HI_ERR_VENC("Picture Width(%u) or Heigth(%u) too large.\n", pstAttr->u32Width, pstAttr->u32Height);

        return HI_ERR_VENC_INVALID_PARA;
    }

    if ((pstAttr->u32Height % HI_VENC_PIC_SZIE_ALIGN) || (pstAttr->u32Width % HI_VENC_PIC_SZIE_ALIGN))
    {
        HI_ERR_VENC("Picture Width(%u) or Heigth(%u) invalid, should N*%d.\n", pstAttr->u32Width, pstAttr->u32Height,
                    HI_VENC_PIC_SZIE_ALIGN);

        return HI_ERR_VENC_INVALID_PARA;
    }

    return HI_SUCCESS;
}

static HI_S32 MpiVencCheckCaplevel(const HI_UNF_VENC_CHN_ATTR_S *pstAttr, HI_U32 *BufMinSize)
{
    HI_U32 TotalMB = 0;

    TotalMB = (VENC_ALIGN_UP(pstAttr->u32Height, 16) >> 4) * (VENC_ALIGN_UP(pstAttr->u32Width, 16) >> 4);
    switch (pstAttr->enCapLevel)
    {
#ifndef HI_VENC_HEVC_SUPPORT
        case HI_UNF_VCODEC_CAP_LEVEL_QCIF:
            if (TotalMB > 99)
            {
                HI_ERR_VENC("Picture Width(%u) or Heigth(%u) too large for enCapLevel(%d) .\n",
                            pstAttr->u32Width, pstAttr->u32Height, pstAttr->enCapLevel);

                return HI_ERR_VENC_INVALID_PARA;
            }

            *BufMinSize = 176 * 144 * 2;
            break;
#endif

        case HI_UNF_VCODEC_CAP_LEVEL_CIF:
            if (TotalMB > 396)
            {
                HI_ERR_VENC("Picture Width(%u) or Heigth(%u) too large for enCapLevel(%d) .\n",
                            pstAttr->u32Width, pstAttr->u32Height, pstAttr->enCapLevel);

                return HI_ERR_VENC_INVALID_PARA;
            }

            *BufMinSize = 352 * 288 * 2;
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_D1:
            if (TotalMB > 1620)
            {
                HI_ERR_VENC("Picture Width(%u) or Heigth(%u) too large for enCapLevel(%d) .\n",
                            pstAttr->u32Width, pstAttr->u32Height, pstAttr->enCapLevel);

                return HI_ERR_VENC_INVALID_PARA;
            }

            *BufMinSize = 720 * 576 * 2;
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_720P:
            if (TotalMB > 3600)
            {
                HI_ERR_VENC("Picture Width(%u) or Heigth(%u) too large for enCapLevel(%d) .\n",
                            pstAttr->u32Width, pstAttr->u32Height, pstAttr->enCapLevel);

                return HI_ERR_VENC_INVALID_PARA;
            }

            *BufMinSize = 1280 * 720 * 2;
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_FULLHD:
            if (TotalMB > 8160)
            {
                HI_ERR_VENC("Picture Width(%u) or Heigth(%u) too large for enCapLevel(%d) .\n",
                            pstAttr->u32Width, pstAttr->u32Height, pstAttr->enCapLevel);

                return HI_ERR_VENC_INVALID_PARA;
            }

            *BufMinSize = 1920 * 1080 * 2;
            break;

        default:
            HI_ERR_VENC("enCapLevel(%d) invalid.\n", pstAttr->enCapLevel);

            return HI_ERR_VENC_INVALID_PARA;
    }

    return HI_SUCCESS;
}
static HI_S32 MpiVencCheckCommonAttr(const HI_UNF_VENC_CHN_ATTR_S* pstAttr, HI_U32 u32BufMinSize)
{
    if ((pstAttr->u32StrmBufSize < u32BufMinSize)
        || (pstAttr->u32StrmBufSize > HI_VENC_MAX_BUF_SIZE))
    {
        HI_ERR_VENC("u32StrmBufSize(%u) invalid.u32StrmBufSize should larger than %d and smaller than %d\n",
                     pstAttr->u32StrmBufSize, u32BufMinSize, HI_VENC_MAX_BUF_SIZE);

        return HI_ERR_VENC_INVALID_PARA;
    }

    if ((pstAttr->u32InputFrmRate < HI_VENC_MIN_fps)
        || (pstAttr->u32InputFrmRate > HI_VENC_MAX_fps))
    {
        HI_ERR_VENC("u32InputFrmRate(%u) is invalid.\n", pstAttr->u32InputFrmRate);

        return HI_ERR_VENC_INVALID_PARA;
    }

    if ((pstAttr->u32TargetFrmRate < HI_VENC_MIN_fps)
        || (pstAttr->u32TargetFrmRate > pstAttr->u32InputFrmRate))
    {
        HI_ERR_VENC("u32TargetFrmRate(%u) is invalid, should 1~inputFrameRate.\n", pstAttr->u32TargetFrmRate);

        return HI_ERR_VENC_INVALID_PARA;
    }

    if (pstAttr->u8Priority >= HI_VENC_MAX_PRIORITY )
    {
        HI_ERR_VENC("u8Priority(%u) is invalid, should < u8MaxPriority(%u).\n", pstAttr->u8Priority , HI_VENC_MAX_PRIORITY);

        return HI_ERR_VENC_INVALID_PARA;
    }

    if (!((HI_TRUE == pstAttr->bQuickEncode) || (HI_FALSE == pstAttr->bQuickEncode)))
    {
        HI_ERR_VENC("bQuickEncode(%u) is invalid.\n", pstAttr->bQuickEncode);

        return HI_ERR_VENC_INVALID_PARA;
    }

    return HI_SUCCESS;
}

static HI_S32 MpiVencCheckPrivateAttr(const HI_UNF_VENC_CHN_ATTR_S *pstAttr)
{
    if (HI_UNF_VCODEC_TYPE_JPEG != pstAttr->enVencType)   /* H264 */
    {
        if (pstAttr->u32RotationAngle != 0)
        {
            HI_ERR_VENC("u32RotationAngle(%u)of H264 Encoder only support 0 now.\n", pstAttr->u32RotationAngle);

            return HI_ERR_VENC_NOT_SUPPORT;
        }

        if (pstAttr->enVencProfile >= HI_UNF_H264_PROFILE_BUTT)
        {
            HI_ERR_VENC("enVencProfile(%d) invalid.\n", pstAttr->enVencProfile);

            return HI_ERR_VENC_INVALID_PARA;
        }

        if (pstAttr->enVencProfile == HI_UNF_H264_PROFILE_EXTENDED)
        {
            HI_ERR_VENC("enVencProfile(%d) not support.\n", pstAttr->enVencProfile);

            return HI_ERR_VENC_NOT_SUPPORT;
        }

        if ((pstAttr->u32TargetBitRate < HI_VENC_MIN_bps) || (pstAttr->u32TargetBitRate > HI_VENC_MAX_bps))
        {
            HI_ERR_VENC("u32TargetBitRate(%u) is invalid.\n", pstAttr->u32TargetBitRate);

            return HI_ERR_VENC_INVALID_PARA;
        }

        if (pstAttr->u32Gop < HI_VENC_MIN_GOP)
        {
            HI_ERR_VENC("u32Gop(%u) is invalid, should > %u.\n", pstAttr->u32Gop, HI_VENC_MIN_GOP);

            return HI_ERR_VENC_INVALID_PARA;
        }

        if (pstAttr->u32MinQp > HI_VENC_MAX_QP)
        {
            HI_ERR_VENC("u32MinQp(%u) is invalid, should < %u.\n", pstAttr->u32MinQp, HI_VENC_MAX_QP);

            return HI_ERR_VENC_INVALID_PARA;
        }

        if (pstAttr->u32MaxQp > HI_VENC_MAX_QP)
        {
            HI_ERR_VENC("u32MaxQp(%u) is invalid, should < %u.\n", pstAttr->u32MaxQp, HI_VENC_MAX_QP);

            return HI_ERR_VENC_INVALID_PARA;
        }

        if (pstAttr->u32MinQp >= pstAttr->u32MaxQp)
        {
            HI_ERR_VENC("u32MinQp(%u) is invalid, should < u32MaxQp(%u).\n", pstAttr->u32MinQp , pstAttr->u32MaxQp);

            return HI_ERR_VENC_INVALID_PARA;
        }

        if (!((HI_TRUE == pstAttr->bSlcSplitEn) || (HI_FALSE == pstAttr->bSlcSplitEn)))
        {
            HI_ERR_VENC("bSlcSplitEn(%u) is invalid.\n", pstAttr->bSlcSplitEn);

            return HI_ERR_VENC_INVALID_PARA;
        }

        if (pstAttr->u32DriftRateThr != (-1))
        {
            if (pstAttr->u32DriftRateThr > HI_VENC_MAX_RcThr)
            {
                HI_ERR_VENC("u32DriftRateThr(%u) is invalid,should smaller than %d\n", pstAttr->u32DriftRateThr, HI_VENC_MAX_RcThr);

                return HI_ERR_VENC_INVALID_PARA;
            }
        }
    }
    else                                                  /* JPEG */
    {
        if (pstAttr->u32RotationAngle != 0)
        {
            HI_ERR_VENC("u32RotationAngle(%u)of JPEG Encoder only support 0 now.\n", pstAttr->u32RotationAngle);

            return HI_ERR_VENC_INVALID_PARA;
        }

        if ((pstAttr->u32Qlevel < HI_VENC_MIN_Q_VALUE) || (pstAttr->u32Qlevel > HI_VENC_MAX_Q_VALUE))
        {
            HI_ERR_VENC("u32Qlevel(%u) is invalid.\n", pstAttr->u32Qlevel);

            return HI_ERR_VENC_INVALID_PARA;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 MpiVencChechConfig(const HI_UNF_VENC_CHN_ATTR_S *pstAttr)
{
    HI_U32 u32BufMinSize = 0;
	HI_S32 Ret;

    if (pstAttr->enVencType >= HI_UNF_VCODEC_TYPE_BUTT)
    {
        HI_ERR_VENC("enVencType(%d) invalid.\n", pstAttr->enVencType);

        return HI_ERR_VENC_INVALID_PARA;
    }

    if ((pstAttr->enVencType != HI_UNF_VCODEC_TYPE_H264) 
#ifdef HI_VENC_HEVC_SUPPORT		
		&& (pstAttr->enVencType != HI_UNF_VCODEC_TYPE_HEVC)
#endif
		&& (pstAttr->enVencType != HI_UNF_VCODEC_TYPE_JPEG))
    {
        HI_ERR_VENC("NOT support Type(%d) VENC channel.\n", pstAttr->enVencType);
        return HI_ERR_VENC_NOT_SUPPORT;
    }

    if (pstAttr->enCapLevel > HI_UNF_VCODEC_CAP_LEVEL_FULLHD)
    {
        HI_ERR_VENC("enCapLevel(%d) invalid.\n", pstAttr->enCapLevel);

        return HI_ERR_VENC_INVALID_PARA;
    }

    Ret = MpiVencCheckPicSize(pstAttr);
    if(Ret != HI_SUCCESS)
    {
        return Ret;
	}

    Ret = MpiVencCheckCaplevel(pstAttr, &u32BufMinSize);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    Ret = MpiVencCheckCommonAttr(pstAttr, u32BufMinSize);
    if(Ret != HI_SUCCESS)
    {
        return Ret;
	}

    Ret = MpiVencCheckPrivateAttr(pstAttr);
    if(Ret != HI_SUCCESS)
    {
        return Ret;
	}

    if (pstAttr->bSlcSplitEn)
    {
        if ((pstAttr->u32SplitSize > HI_VENC_MAX_SPLIT_BYTE_SIZE) || (pstAttr->u32SplitSize < HI_VENC_MIN_SPLIT_BYTE_SIZE))
        {
            HI_ERR_VENC("u32SplitSize(%u) is invalid.Should < %u && > %u .\n", pstAttr->u32SplitSize, 
                        HI_VENC_MAX_SPLIT_BYTE_SIZE,HI_VENC_MIN_SPLIT_BYTE_SIZE);

            return HI_ERR_VENC_INVALID_PARA;
        }
    }

    return HI_SUCCESS;
}


HI_S32 HI_MPI_VENC_Init(HI_VOID)
{
    struct stat st;
    HI_S32 i = 0;

    HI_VENC_LOCK();

    /* Already Opened in the process*/
    if (g_VEncDevFd > 0)
    {
        HI_VENC_UNLOCK();
        return HI_SUCCESS;
    }

    g_VEncDevFd = open(g_VEncDevName, O_RDWR | O_NONBLOCK, 0);

    if (g_VEncDevFd < 0)
    {
        HI_FATAL_VENC("open VEDU err.\n");
        HI_VENC_UNLOCK();
        return HI_FAILURE;
    }


    if (HI_FAILURE == fstat(g_VEncDevFd, &st))
    {
        HI_FATAL_VENC("VEDU is not exist.\n");
        HI_VENC_UNLOCK();
        return HI_ERR_VENC_DEV_NOT_EXIST;
    }

    if (!S_ISCHR (st.st_mode))
    {
        HI_FATAL_VENC("VEDU is not device.\n");
        HI_VENC_UNLOCK();
        return HI_ERR_VENC_NOT_DEV_FILE;
    }


    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        s_asrVeChnInfo[i].handle = HI_INVALID_HANDLE;
        s_asrVeChnVirAddr[i].pStrmBufVirAddr = HI_NULL;
		s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg  = HI_NULL;
        s_asrVeChnInfo[i].u32StrmBufPhyAddr    = HI_NULL;
        s_asrVeChnInfo[i].u32StrmBufPhyAddrJpeg = HI_NULL;
        s_asrVeChnInfo[i].u32BufSize           = 0;
        s_asrVeChnInfo[i].u32BufSizeJpeg       = 0;
    }

    HI_VENC_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_DeInit(HI_VOID)
{
    HI_U32 i;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32PhyAddrBase = 0;

    HI_VENC_LOCK();

    if (g_VEncDevFd < 0)
    {
        HI_VENC_UNLOCK();

        return HI_SUCCESS;
    }

    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        if (s_asrVeChnInfo[i].handle != HI_INVALID_HANDLE)
        {
            u32PhyAddrBase = s_asrVeChnInfo[i].u32StrmBufPhyAddr - VEDU_MMZ_ALIGN;
            s32Ret = HI_MMZ_Unmap(u32PhyAddrBase);

            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_VENC("venc unmap failed!(PhyAddr = 0x%x)\n", u32PhyAddrBase);
            }

            s_asrVeChnVirAddr[i].pStrmBufVirAddr = NULL;
        }
    }

    close(g_VEncDevFd);

    g_VEncDevFd = -1;
    HI_VENC_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_SetAttr(HI_HANDLE hVencChn, const HI_UNF_VENC_CHN_ATTR_S* pstAttr)
{
    HI_S32 s32Ret, i = 0;
    VENC_INFO_CREATE_S stVencChnCreate;
    memset(&stVencChnCreate, 0, sizeof(VENC_INFO_CREATE_S));

    if (!pstAttr)
    {
        HI_ERR_VENC("para pstAttr is NULL.\n");
        return HI_ERR_VENC_NULL_PTR;
    }

    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);

    s32Ret = MpiVencChechConfig(pstAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    stVencChnCreate.hVencChn = hVencChn;
    stVencChnCreate.stAttr.VencUnfAttr = *pstAttr;
    stVencChnCreate.stAttr.bOmxChn = HI_FALSE;
    stVencChnCreate.stAttr.bAutoRequestIFrm = HI_FALSE;
	stVencChnCreate.stAttr.bPrependSpsPps   = HI_TRUE;
    stVencChnCreate.stAttr.ControlRateType  = VENC_DRV_ControlRateConstant;  //CBR;
    memcpy(&stVencChnCreate.stVeInfo, &s_asrVeChnInfo[i], sizeof(VENC_CHN_INFO_S));
    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_SET_CHN_ATTR, &stVencChnCreate);
    if (HI_SUCCESS == s32Ret)
    {
	   if ((stVencChnCreate.stVeInfo.u32StrmBufPhyAddrJpeg != HI_NULL) && (s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg == HI_NULL))  /*h264->jpeg*/
	   {
            s_asrVeChnInfo[i].u32StrmBufPhyAddrJpeg = stVencChnCreate.stVeInfo.u32StrmBufPhyAddrJpeg;
            s_asrVeChnInfo[i].u32BufSizeJpeg        = stVencChnCreate.stVeInfo.u32BufSizeJpeg;
	        s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg = HI_MMAP(s_asrVeChnInfo[i].u32StrmBufPhyAddrJpeg, s_asrVeChnInfo[i].u32BufSizeJpeg); 
		    if (NULL == s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg)
            {
                HI_ERR_VENC("memmap JPEG's stream buffer failed.\n");
                (void)ioctl(g_VEncDevFd, CMD_VENC_DESTROY_CHN, &(stVencChnCreate.hVencChn));
                return HI_FAILURE;
            }


	   }
	   else if ((stVencChnCreate.stVeInfo.u32StrmBufPhyAddrJpeg == HI_NULL) && (s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg != HI_NULL)) /*jpeg->h264*/
	   {
	        s32Ret = HI_MUNMAP(s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg);
            if (HI_SUCCESS != s32Ret)
            {
                return s32Ret;
            }
			s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg = HI_NULL;
            s_asrVeChnInfo[i].u32StrmBufPhyAddrJpeg = HI_NULL;
            s_asrVeChnInfo[i].u32BufSizeJpeg        = 0;
	   }


    }

    return s32Ret;
}

HI_S32 HI_MPI_VENC_GetAttr(HI_HANDLE hVencChn, HI_UNF_VENC_CHN_ATTR_S* pstAttr)
{
    HI_S32 s32Ret, i = 0;
    VENC_INFO_CREATE_S stVencChnCreate;

    if (!pstAttr)
    {
        HI_ERR_VENC("para pstAttr is NULL.\n");
        return HI_ERR_VENC_NULL_PTR;
    }
    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);
    stVencChnCreate.hVencChn = hVencChn;
    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_GET_CHN_ATTR, &stVencChnCreate);
    if (HI_SUCCESS == s32Ret)
    {
        *pstAttr = stVencChnCreate.stAttr.VencUnfAttr;
    }

    return s32Ret;
}

HI_S32 HI_MPI_VENC_Create(HI_HANDLE* phVencChn, const HI_UNF_VENC_CHN_ATTR_S* pstAttr)
{
    HI_S32 s32Ret = 0;
	HI_S32 i = 0;
    VENC_INFO_CREATE_S stVencChnCreate;
	
    HI_U32 u32PhyAddrBase = 0;
	HI_VOID *pUsrVirAddr = NULL;

    if (!pstAttr)
    {
        HI_ERR_VENC("para pstAttr is NULL.\n");
        return HI_ERR_VENC_NULL_PTR;
    }

    if (!phVencChn)
    {
        HI_ERR_VENC("para phVencChn is NULL.\n");
        return HI_ERR_VENC_NULL_PTR;
    }

    CHECK_VENC_INIT();
    s32Ret = MpiVencChechConfig(pstAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        if (s_asrVeChnInfo[i].handle == HI_INVALID_HANDLE)
        {
            break;
        }
    }

    if (VENC_MAX_CHN_NUM == i)
    {
        HI_ERR_VENC("mpi create err, \n");
        return HI_ERR_VENC_CREATE_ERR;
    }

    stVencChnCreate.bOMXChn = HI_FALSE;
    stVencChnCreate.stAttr.bAutoRequestIFrm = HI_FALSE;
	stVencChnCreate.stAttr.bPrependSpsPps   = HI_TRUE;
    stVencChnCreate.stAttr.bOmxChn = HI_FALSE;
    stVencChnCreate.stAttr.ControlRateType = 2;
    memcpy(&(stVencChnCreate.stAttr.VencUnfAttr), pstAttr, sizeof(HI_UNF_VENC_CHN_ATTR_S));

    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_CREATE_CHN, &stVencChnCreate);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    memcpy(&(s_asrVeChnInfo[i]), &(stVencChnCreate.stVeInfo), sizeof(VENC_CHN_INFO_S));
    s_asrVeChnInfo[i].hSource = HI_INVALID_HANDLE;

    if (HI_UNF_VCODEC_TYPE_JPEG == pstAttr->enVencType)
    {
	    /*jpeg not support SMMU*/
        u32PhyAddrBase = s_asrVeChnInfo[i].u32StrmBufPhyAddr - VEDU_MMZ_ALIGN;   // 'VEDU_MMZ_ALIGN' is the offset with the base PhyAddr
        pUsrVirAddr = (HI_VOID*)(HI_SIZE_T)HI_MPI_MMZ_Map(u32PhyAddrBase, 0 /*bCache*/ );  
        s_asrVeChnVirAddr[i].pStrmBufVirAddr = pUsrVirAddr + VEDU_MMZ_ALIGN;
        
        if (NULL == pUsrVirAddr)
        {
            HI_ERR_VENC("memmap VENC's stream buffer failed.\n");
            (void)ioctl(g_VEncDevFd, CMD_VENC_DESTROY_CHN, &(stVencChnCreate.hVencChn));
            return HI_ERR_VENC_CREATE_ERR;
        }

        u32PhyAddrBase = s_asrVeChnInfo[i].u32StrmBufPhyAddrJpeg;
        pUsrVirAddr =  (HI_VOID*)(HI_SIZE_T)HI_MMZ_Map(u32PhyAddrBase, 0); 
        s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg =pUsrVirAddr;
        if (NULL == pUsrVirAddr)
        {
            HI_ERR_VENC("memmap JPEG's stream buffer failed.\n");
            (void)ioctl(g_VEncDevFd, CMD_VENC_DESTROY_CHN, &(stVencChnCreate.hVencChn));
            return HI_ERR_VENC_CREATE_ERR;
        }
        
    }
    else
    {
        u32PhyAddrBase = s_asrVeChnInfo[i].u32StrmBufPhyAddr - VEDU_MMZ_ALIGN;   // 'VEDU_MMZ_ALIGN' is the offset with the base PhyAddr
        pUsrVirAddr = (HI_VOID*)(HI_SIZE_T)HI_MMZ_Map(u32PhyAddrBase, 0 /*bCache*/ );
        s_asrVeChnVirAddr[i].pStrmBufVirAddr = pUsrVirAddr + VEDU_MMZ_ALIGN;
        
        if (NULL == pUsrVirAddr)
        {
            HI_ERR_VENC("memmap VENC's stream buffer failed.\n");
            (void)ioctl(g_VEncDevFd, CMD_VENC_DESTROY_CHN, &(stVencChnCreate.hVencChn));
            return HI_ERR_VENC_CREATE_ERR;
        }
    }
    
    *phVencChn = stVencChnCreate.hVencChn;
    s_asrVeChnInfo[i].handle = *phVencChn;

    HI_INFO_VENC("createOK, Chn%d, BufAddr: %#x / %p, Len:%#x.\n",
                 i, s_asrVeChnInfo[i].u32StrmBufPhyAddr,
                 s_asrVeChnVirAddr[i].pStrmBufVirAddr, s_asrVeChnInfo[i].u32BufSize);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_Destroy(HI_HANDLE hVencChn)
{
    HI_S32 s32Ret = 0, i = 0;
    HI_MOD_ID_E enModID = HI_ID_BUTT;
    HI_U32 u32PhyAddrBase = 0;
    if (HI_INVALID_HANDLE == hVencChn)
    {
        HI_ERR_VENC("para hVencChn is invalid.\n");
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);

    enModID = (HI_MOD_ID_E)((s_asrVeChnInfo[i].hSource & 0xff0000) >> 16);
    switch (enModID)
    {
#ifdef HI_VI_SUPPORT
        case HI_ID_VI:
            s32Ret =  HI_MPI_VI_Detach(s_asrVeChnInfo[i].hSource, hVencChn);
            break;
#endif
        case HI_ID_VO:
            s32Ret =  HI_MPI_WIN_DetachWinSink(s_asrVeChnInfo[i].hSource, hVencChn);
            break;
        case HI_ID_DISP:
            s32Ret =  HI_MPI_DISP_ExtDeAttach(s_asrVeChnInfo[i].hSource, hVencChn);
            break;
        default:
            break;
    }
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VENC("VENC still attach Module(%d),please detach first!\n", enModID);
        return HI_ERR_VENC_CHN_INVALID_STAT;
    }

    if (s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg != NULL)
    {
        u32PhyAddrBase = s_asrVeChnInfo[i].u32StrmBufPhyAddr  - VEDU_MMZ_ALIGN;
        s32Ret = HI_MPI_MMZ_Unmap(u32PhyAddrBase);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VENC("venc unmap failed!(PhyAddr = 0x%x)\n",u32PhyAddrBase);
            return s32Ret;
        }
        s_asrVeChnVirAddr[i].pStrmBufVirAddr = NULL; 
        
        u32PhyAddrBase = s_asrVeChnInfo[i].u32StrmBufPhyAddrJpeg;
	    s32Ret = HI_MMZ_Unmap(u32PhyAddrBase);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VENC("jpge unmap failed!(PhyAddr = 0x%x)\n",u32PhyAddrBase);
            return s32Ret;
        }
		s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg = NULL;
    }
    else
    {
        u32PhyAddrBase = s_asrVeChnInfo[i].u32StrmBufPhyAddr  - VEDU_MMZ_ALIGN;
        s32Ret = HI_MMZ_Unmap(u32PhyAddrBase);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VENC("venc unmap failed!(PhyAddr = 0x%x)\n",u32PhyAddrBase);
            return s32Ret;
        }
        s_asrVeChnVirAddr[i].pStrmBufVirAddr = NULL;          
    }

    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_DESTROY_CHN, &hVencChn);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }


    s_asrVeChnInfo[i].handle = HI_INVALID_HANDLE;
    s_asrVeChnInfo[i].hSource = HI_INVALID_HANDLE;

    return s32Ret;
}

HI_S32 HI_MPI_VENC_AttachInput(HI_HANDLE hVencChn, HI_HANDLE hSrc)
{
    HI_S32 s32Ret = 0, i = 0;
    VENC_INFO_ATTACH_S stVencAttachInfo;
    HI_MOD_ID_E enModID = HI_ID_BUTT;

    if (HI_INVALID_HANDLE == hSrc)
    {
        HI_ERR_VENC("para hSrc is invalid.\n");
        return HI_ERR_VENC_INVALID_PARA;
    }

    if (HI_INVALID_HANDLE == hVencChn)
    {
        HI_ERR_VENC("para hVencChn is invalid.\n");
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);

    enModID = (HI_MOD_ID_E)((hSrc & 0xff0000) >> 16);

    stVencAttachInfo.hSrc = hSrc;
    stVencAttachInfo.hVencChn = hVencChn;
    stVencAttachInfo.enModId = enModID;

    s32Ret  = ioctl(g_VEncDevFd, CMD_VENC_ATTACH_INPUT, &stVencAttachInfo);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    switch (enModID)
    {
        case HI_ID_VI:
#ifdef HI_VI_SUPPORT
            s32Ret = HI_MPI_VI_Attach(hSrc , hVencChn);
#else
            HI_ERR_VENC("venc not support attach VI in this version!\n", enModID);
            s32Ret = HI_ERR_VENC_NOT_SUPPORT;
#endif
            break;

        case HI_ID_VO:
        {
#if 1
            s32Ret = HI_MPI_WIN_AttachWinSink(hSrc, hVencChn);
#endif
        }
        break;
        case HI_ID_DISP:
        {
            s32Ret = HI_MPI_DISP_ExtAttach(hSrc, hVencChn);
        }
        break;
        default:
            HI_ERR_VENC("ModId not surpport now, enModId=%x!\n", enModID);
            s32Ret = HI_ERR_VENC_INVALID_PARA;
            break;
    }

    if (HI_SUCCESS == s32Ret)
    {
        s_asrVeChnInfo[i].hSource = hSrc;
        s32Ret = HI_MPI_VENC_Start(hVencChn);
    }
    else
    {
        if (HI_ID_VO == enModID)
        {
            s32Ret = HI_MPI_WIN_DetachWinSink(hSrc, hVencChn);
        }
        else if (HI_ID_DISP == enModID)
        {
            s32Ret = HI_MPI_DISP_ExtDeAttach(hSrc, hVencChn);
        }
        else if (HI_ID_VI == enModID)
        {
#ifdef HI_VI_SUPPORT
            s32Ret = HI_MPI_VI_Detach(hSrc, hVencChn);
#endif
        }

        if (s32Ret != HI_SUCCESS)
        {
            HI_WARN_VENC("Mode(%d) detach func return failed!", enModID);
        }
        if (HI_SUCCESS != ioctl(g_VEncDevFd, CMD_VENC_DETACH_INPUT, &stVencAttachInfo))
        {
            HI_INFO_VENC("detach input error\n");
        }        
    }
    return s32Ret;
}

HI_S32 HI_MPI_VENC_SetSource(HI_HANDLE hVencChn, HI_DRV_VENC_SRC_HANDLE_S* pstSrc)
{
    HI_S32 s32Ret = 0, i = 0;
    VENC_SET_SRC_S stSetSrcInfo;

    if (HI_INVALID_HANDLE == hVencChn)
    {
        HI_ERR_VENC("para hSrc is invalid.\n");
        return HI_ERR_VENC_INVALID_PARA;
    }

    if (!pstSrc)
    {
        HI_ERR_VENC("para pstStream is NULL.\n");
        return HI_ERR_VENC_NULL_PTR;
    }

    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);

    stSetSrcInfo.hVencChn      = hVencChn;
    memcpy(&stSetSrcInfo.stVencSrc, pstSrc, sizeof(HI_DRV_VENC_SRC_HANDLE_S));
    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_SET_SRC, &stSetSrcInfo);
    if (HI_SUCCESS != s32Ret)
    {
        s_asrVeChnInfo[i].hSource = HI_INVALID_HANDLE;
    }
    return s32Ret;
}


HI_S32 HI_MPI_VENC_DetachInput(HI_HANDLE hVencChn/*, HI_HANDLE hSrc*/)
{
    HI_S32 s32Ret = 0, i = 0;
    VENC_INFO_ATTACH_S stVencAttachInfo;
    HI_MOD_ID_E enModID = HI_ID_BUTT;

    if (HI_INVALID_HANDLE == hVencChn)
    {
        HI_ERR_VENC("para hVencChn is invalid.\n");
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }
    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);

    stVencAttachInfo.hVencChn = hVencChn;

    enModID = (HI_MOD_ID_E)((s_asrVeChnInfo[i].hSource & 0xff0000) >> 16);

    switch (enModID)
    {
#ifdef HI_VI_SUPPORT
        case HI_ID_VI:
            s32Ret =  HI_MPI_VI_Detach(s_asrVeChnInfo[i].hSource, hVencChn);
            break;
#endif
        case HI_ID_VO:
            s32Ret =  HI_MPI_WIN_DetachWinSink(s_asrVeChnInfo[i].hSource, hVencChn);
            break;
        case HI_ID_DISP:
            s32Ret =  HI_MPI_DISP_ExtDeAttach(s_asrVeChnInfo[i].hSource, hVencChn);
            break;
        case HI_ID_BUTT:
            return HI_ERR_VENC_CHN_NO_ATTACH;
        default:
            HI_ERR_VENC("Venc Detach, ModId not surpport now, enModId=%x!\n", enModID);
            return HI_ERR_VENC_INVALID_PARA;
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VENC("venc can't detach with ModID(%x) now.\n", enModID);
        return HI_FAILURE;
    }

    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_DETACH_INPUT, &stVencAttachInfo);
	
    if (HI_SUCCESS == s32Ret)
    {
        s_asrVeChnInfo[i].hSource = HI_INVALID_HANDLE;
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 HI_MPI_VENC_AcquireStream(HI_HANDLE hVencChn, HI_UNF_VENC_STREAM_S* pstStream, HI_U32 u32TimeoutMs)
{
    HI_S32 s32Ret = 0, i = 0;
    VENC_INFO_ACQUIRE_STREAM_S stVencAcquireStream;
    VENC_BUF_OFFSET_S stBufOffSet;

    if (HI_INVALID_HANDLE == hVencChn)
    {
        HI_ERR_VENC("para hVencChn is invalid.\n");
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    if (HI_NULL == pstStream)
    {
        HI_ERR_VENC("para pstStream is NULL.\n");
        return HI_ERR_VENC_NULL_PTR;
    }

    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);

    stVencAcquireStream.hVencChn = hVencChn;
    stVencAcquireStream.u32BlockFlag = u32TimeoutMs;
    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_ACQUIRE_STREAM, &stVencAcquireStream);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    memcpy(&stBufOffSet, &(stVencAcquireStream.stBufOffSet), sizeof(stBufOffSet));
    //memcpy(pstStream, &(stVencAcquireStream.stStream), sizeof(HI_UNF_VENC_STREAM_S));
    //pstStream->pu8Addr = stVencAcquireStream.stStream.pu8Addr;
    pstStream->u32SlcLen = stVencAcquireStream.stStream.u32SlcLen;
    pstStream->u32PtsMs = stVencAcquireStream.stStream.u32PtsMs;
    pstStream->bFrameEnd = stVencAcquireStream.stStream.bFrameEnd;
    pstStream->enDataType = stVencAcquireStream.stStream.enDataType;

    if (pstStream->u32SlcLen > 0)
    {
        if (s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg != NULL)
        {
            pstStream->pu8Addr = (HI_U8*)s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg;
        }
        else
        {
		    pstStream->pu8Addr = (HI_U8*)(s_asrVeChnVirAddr[i].pStrmBufVirAddr) + stBufOffSet.u32StrmBufOffset[0];
        }
    }
    /*
    HI_INFO_VENC("GetOK, Chn%d, %d,%#x / %d,%#x.\n", i,
                    pstStream->u32SlcLen[0], pstStream->u32PhyAddr[0],
                    pstStream->u32SlcLen[1], pstStream->u32PhyAddr[1]);
     */
    return HI_SUCCESS;
}

HI_S32 HI_MPI_VENC_ReleaseStream(HI_HANDLE hVencChn, const HI_UNF_VENC_STREAM_S* pstStream)
{
    HI_S32 s32Ret = 0, i = 0;
    VENC_INFO_ACQUIRE_STREAM_S stVencAcquireStream;

    if (HI_INVALID_HANDLE == hVencChn)
    {
        HI_ERR_VENC("para hVencChn is invalid.\n");
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    if (HI_NULL == pstStream)
    {
        HI_ERR_VENC("para pstStream is NULL.\n");
        return HI_ERR_VENC_NULL_PTR;
    }
    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);

    stVencAcquireStream.hVencChn = hVencChn;
    //memcpy(&(stVencAcquireStream.stStream), pstStream, sizeof(HI_UNF_VENC_STREAM_S));
    //stVencAcquireStream.stStream.pu8Addr = pstStream->pu8Addr;
    stVencAcquireStream.stStream.u32SlcLen = pstStream->u32SlcLen;
    stVencAcquireStream.stStream.u32PtsMs = pstStream->u32PtsMs;
    stVencAcquireStream.stStream.bFrameEnd = pstStream->bFrameEnd;
    stVencAcquireStream.stStream.enDataType = pstStream->enDataType;
    if (s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg != HI_NULL)
    {
        if (pstStream->pu8Addr != (HI_U8*)s_asrVeChnVirAddr[i].pStrmBufVirAddrJpeg)
        {
            HI_ERR_VENC("Release stream failed.\n");
            return HI_ERR_VENC_CHN_RELEASE_ERR;
        }
        stVencAcquireStream.Jpeg_Phy_Addr = s_asrVeChnInfo[i].u32StrmBufPhyAddrJpeg;


    }
    else
    {
        stVencAcquireStream.H264_stream_off = (pstStream->pu8Addr - (HI_U8*)(s_asrVeChnVirAddr[i].pStrmBufVirAddr));

    }

    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_RELEASE_STREAM, &stVencAcquireStream);

    return s32Ret;
}

HI_S32 HI_MPI_VENC_Start(HI_HANDLE hVencChn)
{
    HI_S32 s32Ret = 0, i = 0;

    if ((hVencChn & 0xffff0000) != (HI_ID_VENC << 16))
    {
        HI_ERR_VENC("para hVencChn is invalid.\n");
        return HI_ERR_VENC_INVALID_PARA;
    }

    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);

    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_START_RECV_PIC, &hVencChn);

    return s32Ret;
}

HI_S32 HI_MPI_VENC_Stop(HI_HANDLE hVencChn)
{
    HI_S32 s32Ret = 0, i = 0;
    if ((hVencChn & 0xffff0000) != (HI_ID_VENC << 16))
    {
        HI_ERR_VENC("para hVencChn is invalid.\n");
        return HI_ERR_VENC_INVALID_PARA;
    }

    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);
    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_STOP_RECV_PIC, &hVencChn);

    return s32Ret;
}

HI_S32 HI_MPI_VENC_RequestIFrame(HI_HANDLE hVencChn)
{
    HI_S32 s32Ret = 0, i = 0;

    if ((hVencChn & 0xffff0000) != (HI_ID_VENC << 16))
    {
        HI_ERR_VENC("para hVencChn is invalid.\n");
        return HI_ERR_VENC_INVALID_PARA;
    }

    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);

    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_REQUEST_I_FRAME, &hVencChn);

    return s32Ret;
}

HI_S32 HI_MPI_VENC_QueueFrame(HI_HANDLE hVencChn, HI_UNF_VIDEO_FRAME_INFO_S* pstFrameinfo)  //liminqi
{
    HI_S32 s32Ret = 0, i = 0;
    VENC_INFO_QUEUE_FRAME_S stVencQueueFrame;
    if (HI_INVALID_HANDLE == hVencChn)
    {
        HI_ERR_VENC("para hVencChn is invalid.\n");
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }
    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);

    if (HI_NULL == pstFrameinfo)
    {
        HI_ERR_VENC("para pstStream is NULL.\n");
        return HI_ERR_VENC_NULL_PTR;
    }

    stVencQueueFrame.hVencChn = hVencChn;
    memcpy(&(stVencQueueFrame.stVencFrame), pstFrameinfo, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));
    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_QUEUE_FRAME, &stVencQueueFrame);   //LIMINQI
    return s32Ret;
}

HI_S32 HI_MPI_VENC_DequeueFrame(HI_HANDLE hVencChn, HI_UNF_VIDEO_FRAME_INFO_S* pstFrameinfo)  //liminqi
{
    HI_S32 s32Ret = 0, i = 0;
    VENC_INFO_QUEUE_FRAME_S stVencQueueFrame;
    if (HI_INVALID_HANDLE == hVencChn)
    {
        HI_ERR_VENC("para hVencChn is invalid.\n");
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }
    CHECK_VENC_INIT();
    CHECK_CHANNEL_EXIST(i, VENC_MAX_CHN_NUM, hVencChn);
    if (HI_NULL == pstFrameinfo)
    {
        HI_ERR_VENC("para pstStream is NULL.\n");
        return HI_ERR_VENC_NULL_PTR;
    }
    stVencQueueFrame.hVencChn = hVencChn;

    s32Ret = ioctl(g_VEncDevFd, CMD_VENC_DEQUEUE_FRAME, &stVencQueueFrame);   //LIMINQI

    if (!s32Ret)
    {
        memcpy(pstFrameinfo, &(stVencQueueFrame.stVencFrame), sizeof(HI_UNF_VIDEO_FRAME_INFO_S));
    }
    return s32Ret;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

