/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_vi.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/03/26
  Description   :
  History       :
  1.Date        : 2010/03/26
    Author      : j00131665
    Modification: Created file

 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>

#include "hi_drv_struct.h"
#include "hi_drv_vi.h"
#include "hi_mpi_vi.h"
#include "hi_mpi_win.h"
#include "hi_mpi_venc.h"
#include "hi_mpi_mem.h"
#include "hi_drv_win.h"
#include "drv_venc_ext.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C"
{
 #endif
#endif

static HI_S32 g_ViDevFd = -1;
static const HI_CHAR g_ViDevName[] = "/dev/" UMAP_DEVNAME_VI;
static VI_S g_Vi[MAX_VI_PORT][MAX_VI_CHN];
static HI_U32 portLoop, chnLoop;
static pthread_mutex_t g_ViMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_VI_LOCK() (HI_VOID)pthread_mutex_lock(&g_ViMutex);
#define HI_VI_UNLOCK() (HI_VOID)pthread_mutex_unlock(&g_ViMutex);

#define CHECK_VI_INIT() \
    do {\
        HI_VI_LOCK(); \
        if (g_ViDevFd < 0)\
        {\
            HI_VI_UNLOCK(); \
            return HI_ERR_VI_NO_INIT; \
        } \
        HI_VI_UNLOCK(); \
    } while (0)

#define CHECK_VI_NULL_PTR(ptr) \
    do {\
        if (NULL == (ptr))\
        {\
            HI_ERR_VI("PTR('%s') is NULL.\n", # ptr); \
            return HI_ERR_VI_NULL_PTR; \
        } \
    } while (0)

#define CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, ViChn) \
    do { \
        ViChn  = ((hVi) & 0xff); \
        enPort = (HI_UNF_VI_E)(((hVi) & 0xff00) >> 8); \
        if (MAX_VI_CHN <= ViChn) \
        { \
            HI_ERR_VI("VI handle(%#x) is invalid, channel error.\n", (hVi)); \
            return HI_ERR_VI_CHN_NOT_EXIST; \
        } \
        if (HI_UNF_VI_BUTT <= enPort)\
        {\
            HI_ERR_VI("VI handle(%#x) is invalid, port error.\n", (hVi)); \
            return HI_ERR_VI_CHN_NOT_EXIST; \
        } \
        if ((HI_INVALID_HANDLE == (hVi)) || (HI_NULL == (hVi)))\
        {\
            HI_ERR_VI("VI handle(%#x) is invalid.\n", (hVi)); \
            return HI_ERR_VI_CHN_NOT_EXIST; \
        } \
        if (HI_ID_VI != ((hVi) >> 16)) \
        {\
            HI_ERR_VI("VI handle(%#x) is invalid, modID error.\n", (hVi)); \
            return HI_ERR_VI_CHN_NOT_EXIST; \
        } \
        for (portLoop = 0; portLoop < MAX_VI_PORT; portLoop++) \
        {\
            for (chnLoop = 0; chnLoop < MAX_VI_CHN; chnLoop++) \
            {\
                if (g_Vi[portLoop][chnLoop].hVi == (hVi)) \
                { \
                    goto FIND_VI_HANDLE; \
                } \
            } \
        } \
FIND_VI_HANDLE: \
        if ((MAX_VI_PORT == portLoop) && (MAX_VI_CHN == chnLoop)) \
        {\
            return HI_ERR_VI_CHN_NOT_EXIST; \
        } \
    } while (0)

static HI_S32 VI_CheckAttr(HI_UNF_VI_E enPort,  HI_UNF_VI_ATTR_S *pstAttr)
{
    CHECK_VI_NULL_PTR(pstAttr);

    if ((pstAttr->bVirtual != HI_FALSE)
        && (pstAttr->bVirtual != HI_TRUE))
    {
        HI_ERR_VI("invalid vi attr pstAttr->bVirtual %d\n", pstAttr->bVirtual);
        return HI_ERR_VI_INVALID_PARA;
    }

    if ((pstAttr->u32BufNum < MIN_VI_FB_NUM)
        || (pstAttr->u32BufNum > MAX_VI_FB_NUM))
    {
        HI_ERR_VI("invalid vi buf number %d.\n", pstAttr->u32BufNum);
        return HI_ERR_VI_INVALID_PARA;
    }
    
    if (pstAttr->enBufMgmtMode >= HI_UNF_VI_BUF_BUTT) 
    {
        HI_ERR_VI("invalid vi buf management mode %d.\n", pstAttr->enBufMgmtMode);
        return HI_ERR_VI_INVALID_PARA;
    }
    
#ifndef HI_VI_REAL_SUPPORT
    if (pstAttr->bVirtual == HI_FALSE)
    {
        HI_ERR_VI("not support REAL VI.\n");
        return HI_ERR_VI_NOT_SUPPORT;
    }
#endif

    if (pstAttr->bVirtual == HI_FALSE)
    {
        if (HI_UNF_VI_PORT0 != enPort)
        {
            HI_ERR_VI("Only support REAL VI by port0.\n");
            return HI_ERR_VI_INVALID_PARA;
        }

        if ((pstAttr->stInputRect.s32X < 0)
            || (pstAttr->stInputRect.s32Y < 0)
            || (pstAttr->stInputRect.s32Width <= 0)
            || (pstAttr->stInputRect.s32Height <= 0))
        {
            HI_ERR_VI("invalid vi input rect x(%d) y(%d) width(%d) height(%d).\n",
                      pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
                      pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height);
            return HI_ERR_VI_INVALID_PARA;
        }

        if ((pstAttr->stInputRect.s32Width & 0x03)
             || (pstAttr->stInputRect.s32Height & 0x01))
         {
             HI_WARN_VI("invalid vi input  width(%d%4 != 0) or height(%d%2 != 0).\n",
                       pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height);
              pstAttr->stInputRect.s32Width &= 0xFFFFFFFC;
              pstAttr->stInputRect.s32Height &= 0xFFFFFFFE;
         }

        if (pstAttr->enInputMode >= HI_UNF_VI_MODE_BUTT)
        {
            HI_ERR_VI("invalid vi input mode %d.\n", pstAttr->enInputMode);
            return HI_ERR_VI_INVALID_PARA;
        }

#ifdef HI_HDMI_RX_SUPPORT
        if (HI_UNF_VI_MODE_HDMIRX != pstAttr->enInputMode)
        {
            HI_ERR_VI("invalid vi input mode: %d != HI_UNF_VI_MODE_HDMIRX \n", pstAttr->enInputMode);
            return HI_ERR_VI_INVALID_PARA;
        }
        if((pstAttr->stInputRect.s32X + pstAttr->stInputRect.s32Width > pstAttr->u32Width)
                || (pstAttr->stInputRect.s32Y + pstAttr->stInputRect.s32Height > pstAttr->u32Height))
        {
            HI_ERR_VI("input rect out of range, x(%d), y(%d), width(%d), height(%d).\n",
                      pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
                      pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height);
            return HI_ERR_VI_INVALID_PARA;
        }
        if ((HI_UNF_FORMAT_YUV_SEMIPLANAR_422 != pstAttr->enVideoFormat))
        {
            HI_ERR_VI("invalid vi store format %d.\n", pstAttr->enVideoFormat);
            return HI_ERR_VI_INVALID_PARA;
        }
        if ((HI_UNF_VI_BUF_ALLOC != pstAttr->enBufMgmtMode))
        {
            HI_ERR_VI("invalid vi buf management mode %d.\n", pstAttr->enBufMgmtMode);
            return HI_ERR_VI_INVALID_PARA;
        }
#else
        if ((HI_UNF_VI_MODE_BT601_576I == pstAttr->enInputMode)
            || (HI_UNF_VI_MODE_BT601_480I == pstAttr->enInputMode))
        {
            HI_ERR_VI("not support BT601 mode %d\n", pstAttr->enInputMode);
            return HI_ERR_VI_INVALID_PARA;
        }

        if ((HI_UNF_VI_MODE_BT1120_640X480P == pstAttr->enInputMode)
            && ((pstAttr->stInputRect.s32X + pstAttr->stInputRect.s32Width > 640)
                || (pstAttr->stInputRect.s32Y + pstAttr->stInputRect.s32Height > 480)))
        {
            HI_ERR_VI("input rect out of range, x(%d), y(%d), width(%d), height(%d).\n",
                      pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
                      pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height);
            return HI_ERR_VI_INVALID_PARA;
        }

        if (((HI_UNF_VI_MODE_BT656_576I == pstAttr->enInputMode)
             || (HI_UNF_VI_MODE_BT601_576I == pstAttr->enInputMode)
             || (HI_UNF_VI_MODE_BT1120_576P == pstAttr->enInputMode)
             || (HI_UNF_VI_MODE_DIGITAL_CAMERA_576P_30 == pstAttr->enInputMode))
            && ((pstAttr->stInputRect.s32X + pstAttr->stInputRect.s32Width > 720)
                || (pstAttr->stInputRect.s32Y + pstAttr->stInputRect.s32Height > 576)))
        {
            HI_ERR_VI("input rect out of range, x(%d), y(%d), width(%d), height(%d).\n",
                      pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
                      pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height);
            return HI_ERR_VI_INVALID_PARA;
        }

        if (((HI_UNF_VI_MODE_BT656_480I == pstAttr->enInputMode)
             || (HI_UNF_VI_MODE_BT601_480I == pstAttr->enInputMode)
             || (HI_UNF_VI_MODE_BT1120_480P == pstAttr->enInputMode)
             || (HI_UNF_VI_MODE_DIGITAL_CAMERA_480P_30 == pstAttr->enInputMode))
            && ((pstAttr->stInputRect.s32X + pstAttr->stInputRect.s32Width > 720)
                || (pstAttr->stInputRect.s32Y + pstAttr->stInputRect.s32Height > 480)))
        {
            HI_ERR_VI("input rect out of range, x(%d), y(%d), width(%d), height(%d).\n",
                      pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
                      pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height);
            return HI_ERR_VI_INVALID_PARA;
        }

        if (((HI_UNF_VI_MODE_BT1120_720P_50 == pstAttr->enInputMode)
             || (HI_UNF_VI_MODE_BT1120_720P_60 == pstAttr->enInputMode)
             || (HI_UNF_VI_MODE_DIGITAL_CAMERA_720P_30 == pstAttr->enInputMode))
            && ((pstAttr->stInputRect.s32X + pstAttr->stInputRect.s32Width > 1280)
                || (pstAttr->stInputRect.s32Y + pstAttr->stInputRect.s32Height > 720)))
        {
            HI_ERR_VI("input rect out of range, x(%d), y(%d), width(%d), height(%d).\n",
                      pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
                      pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height);
            return HI_ERR_VI_INVALID_PARA;
        }

       if ((((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_1080I_50)
             && (pstAttr->enInputMode <= HI_UNF_VI_MODE_BT1120_1080P_60))
             || (HI_UNF_VI_MODE_DIGITAL_CAMERA_1080P_30 == pstAttr->enInputMode))
            && ((pstAttr->stInputRect.s32X + pstAttr->stInputRect.s32Width > 1920)
                || (pstAttr->stInputRect.s32Y + pstAttr->stInputRect.s32Height > 1080)))
        {
            HI_ERR_VI("input rect out of range, x(%d), y(%d), width(%d), height(%d).\n",
                      pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
                      pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height);
            return HI_ERR_VI_INVALID_PARA;
        }

        if ((HI_UNF_VI_MODE_HDMIRX == pstAttr->enInputMode)
             &&((pstAttr->stInputRect.s32X + pstAttr->stInputRect.s32Width > pstAttr->u32Width)
                || (pstAttr->stInputRect.s32Y + pstAttr->stInputRect.s32Height > pstAttr->u32Height)
                || (1200 < pstAttr->u32Height)
                || (1920 < pstAttr->u32Width)))
        {
            HI_ERR_VI("input rect out of range, x(%d), y(%d), width(%d), height(%d).\n",
                      pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
                      pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height);
            return HI_ERR_VI_INVALID_PARA;
        }

        if ((HI_UNF_FORMAT_YUV_SEMIPLANAR_422 != pstAttr->enVideoFormat)
            && (HI_UNF_FORMAT_YUV_SEMIPLANAR_420 != pstAttr->enVideoFormat))
        {
            HI_ERR_VI("invalid vi store format %d.\n", pstAttr->enVideoFormat);
            return HI_ERR_VI_INVALID_PARA;
        }

        if ((HI_UNF_VI_BUF_ALLOC != pstAttr->enBufMgmtMode)
            && (HI_UNF_VI_BUF_MMAP != pstAttr->enBufMgmtMode))
        {
            HI_ERR_VI("invalid vi buf management mode %d.\n", pstAttr->enBufMgmtMode);
            return HI_ERR_VI_INVALID_PARA;
        }
#endif
    }

    return HI_SUCCESS;
}

static HI_S32 VI_AttachToVpss(HI_HANDLE hVi, HI_HANDLE hDst)
{
    HI_S32 Ret = HI_FAILURE;
    HI_DRV_WIN_SRC_HANDLE_S stWinSrc;
    VI_VPSS_PORT_S stVpssPort;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;
    HI_U32 i;

    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    for (i = 0; i < VI_MAX_VPSS_PORT; i++)
    {
        if (HI_INVALID_HANDLE == g_Vi[enPort][u8Chn].stPortParam[i].hPort)
        {
            break;
        }
    }

    if (i == VI_MAX_VPSS_PORT)
    {
        HI_ERR_VI("create vpss port failed, max number reached!\n");
        return HI_FAILURE;
    }

    stVpssPort.hVi   = g_Vi[enPort][u8Chn].hVi;
    stVpssPort.hVpss = g_Vi[enPort][u8Chn].hVpss;
    stVpssPort.stPortParam.hDst = hDst;
    Ret = ioctl(g_ViDevFd, CMD_VI_CREATE_VPSS_PORT, &stVpssPort);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_VI("create vpss port failed, ret = 0x%08x\n", Ret);
        return HI_FAILURE;
    }

    memcpy(&g_Vi[enPort][u8Chn].stPortParam[i], &stVpssPort.stPortParam, sizeof(VI_VPSS_PORT_PARAM_S));

    switch ((hDst >> 16) & 0xff)
    {
    case HI_ID_VO:
    {
        memset(&stWinSrc, 0, sizeof(HI_DRV_WIN_SRC_HANDLE_S));
        stWinSrc.hSrc = g_Vi[enPort][u8Chn].stPortParam[i].hPort;
        stWinSrc.hSecondSrc = HI_INVALID_HANDLE;
        stWinSrc.enSrcMode = HI_ID_VI;
        stWinSrc.u32Resrve1 = 0;
        
        Ret = HI_MPI_WIN_SetSource(hDst, &stWinSrc);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_VI("HI_MPI_WIN_SetSource failed, ret = 0x%08x\n", Ret);
            return HI_FAILURE;
        }

        break;
    }
#ifdef HI_VENC_SUPPORT
    case HI_ID_VENC:
    {
        HI_DRV_VENC_SRC_HANDLE_S stVencSrc;

        memset(&stVencSrc, 0, sizeof(HI_DRV_VENC_SRC_HANDLE_S));
        stVencSrc.hSrc = g_Vi[enPort][u8Chn].stPortParam[i].hPort;
        stVencSrc.enSrcMode   = HI_ID_VI;

        Ret = HI_MPI_VENC_SetSource(hDst, &stVencSrc);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_VI("HI_MPI_VENC_SetSource failed, ret = 0x%08x\n", Ret);
            return HI_FAILURE;
        }

        break;
    }
#endif
    default:
    {
        HI_ERR_VI("unrecognized handle[0x%08x]\n", hDst);
        return HI_FAILURE;
    }
    }

    return HI_SUCCESS;
}

static HI_S32 VI_DetachFromVpss(HI_HANDLE hVi, HI_HANDLE hDst)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_DRV_WIN_SRC_HANDLE_S stWinSrc;
    VI_VPSS_PORT_S stVpssPort;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;
    HI_U32 i;

    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    for (i = 0; i < VI_MAX_VPSS_PORT; i++)
    {
        if (hDst == g_Vi[enPort][u8Chn].stPortParam[i].hDst)
        {
            break;
        }
    }

    if (i == VI_MAX_VPSS_PORT)
    {
        HI_ERR_VI("this is not a attached window.\n");
        return HI_FAILURE;
    }

    switch ((hDst >> 16) & 0xff)
    {
    case HI_ID_VO:
    {
        memset(&stWinSrc, 0, sizeof(HI_DRV_WIN_SRC_HANDLE_S));

        stWinSrc.hSrc = HI_INVALID_HANDLE; //g_Vi[enPort][u8Chn].stPortParam[i].hPort;
        stWinSrc.hSecondSrc = HI_INVALID_HANDLE;
        stWinSrc.enSrcMode = HI_ID_VI;
        stWinSrc.u32Resrve1 = 0;
        
        Ret = HI_MPI_WIN_SetSource(hDst, &stWinSrc);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_VI("ERR: HI_MPI_WIN_SetSource.\n");
        }

        break;
    }
    case HI_ID_VENC:
    {
        /* no need to set source */
        break;
    }
    default:
    {
        HI_ERR_VI("unrecognized handle[0x%08x]\n", hDst);
        return HI_FAILURE;
    }
    }

    stVpssPort.hVi = hVi;
    stVpssPort.stPortParam.hDst = hDst;
    Ret = ioctl(g_ViDevFd, CMD_VI_DESTROY_VPSS_PORT, &stVpssPort);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_VI("CMD_VI_CREATE_VPSS_PORT failed, ret = 0x%08x\n", Ret);
        return HI_FAILURE;
    }

    memset(&g_Vi[enPort][u8Chn].stPortParam[i], 0, sizeof(VI_VPSS_PORT_PARAM_S));
    g_Vi[enPort][u8Chn].stPortParam[i].hDst  = HI_INVALID_HANDLE;
    g_Vi[enPort][u8Chn].stPortParam[i].hPort = HI_INVALID_HANDLE;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_Init(HI_VOID)
{
    struct stat st;
    HI_U32 iPort, iChn;

    HI_VI_LOCK();

    /* Already opened in the process */
    if (g_ViDevFd > 0)
    {
        HI_VI_UNLOCK();
        return HI_SUCCESS;
    }
    
    g_ViDevFd = open(g_ViDevName, O_RDWR | O_NONBLOCK, 0);
    if (g_ViDevFd < 0)
    {
        HI_FATAL_VI("open VI err.\n");
        HI_VI_UNLOCK();
        return HI_FAILURE;
    }

    if (HI_FAILURE == fstat(g_ViDevFd, &st))
    {
        HI_FATAL_VI("VI is not exist.\n");
        HI_VI_UNLOCK();
        return HI_ERR_VI_DEV_NOT_EXIST;
    }

    if (!S_ISCHR (st.st_mode))
    {
        HI_FATAL_VI("VI is not device.\n");
        HI_VI_UNLOCK();
        return HI_ERR_VI_NOT_DEV_FILE;
    }

    for (iPort = 0; iPort < MAX_VI_PORT; iPort++)
    {
        for (iChn = 0; iChn < MAX_VI_CHN; iChn++)
        {
            g_Vi[iPort][iChn].hVi = HI_INVALID_HANDLE;
        }
    }

    HI_VI_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_DeInit(HI_VOID)
{
    HI_VI_LOCK();

    if (g_ViDevFd < 0)
    {
        HI_VI_UNLOCK();
        return HI_SUCCESS;
    }

    close(g_ViDevFd);

    g_ViDevFd = -1;
    HI_VI_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_Create(HI_UNF_VI_E enPort, HI_UNF_VI_ATTR_S *pstAttr, HI_HANDLE *phVi)
{
    HI_S32 Ret = HI_FAILURE;
    VI_CREATE_S stCreate;
    HI_U8 u8Chn;
    HI_U32 i;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(phVi);
    CHECK_VI_NULL_PTR(pstAttr);

    if ((HI_UNF_VI_PORT0 != enPort) && (HI_UNF_VI_PORT1 != enPort))
    {
        HI_ERR_VI("invalid vi port %d.\n", enPort);
        return HI_ERR_VI_INVALID_PARA;
    }

    Ret = VI_CheckAttr(enPort, pstAttr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    stCreate.enPort = enPort;
    stCreate.hVi   = HI_INVALID_HANDLE;
    stCreate.hVpss = VPSS_INVALID_HANDLE;
    memcpy(&(stCreate.stViAttr), pstAttr, sizeof(HI_UNF_VI_ATTR_S));

    Ret = ioctl(g_ViDevFd, CMD_VI_CREATE, &stCreate);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_VI("CMD_VI_CREATE Ret:%x  %d.\n", Ret);
        return Ret;
    }

    *phVi  = stCreate.hVi;
    u8Chn = (HI_U8)((stCreate.hVi) & 0xFF);
    if (MAX_VI_CHN <= u8Chn)
    {
        HI_ERR_VI("VI handle(%#x) is invalid, channel error.\n", (stCreate.hVi));
        Ret = ioctl(g_ViDevFd, CMD_VI_DESTROY, phVi);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
        return HI_ERR_VI_CHN_NOT_EXIST;
    }

    g_Vi[enPort][u8Chn].hVi   = stCreate.hVi;
    g_Vi[enPort][u8Chn].hVpss = stCreate.hVpss;
    g_Vi[enPort][u8Chn].u32State = 0;
    memcpy(&g_Vi[enPort][u8Chn].stAttr, pstAttr, sizeof(HI_UNF_VI_ATTR_S));

    for (i = 0; i < VI_MAX_VPSS_PORT; i++)
    {
        g_Vi[enPort][u8Chn].stPortParam[i].hDst  = HI_INVALID_HANDLE;
        g_Vi[enPort][u8Chn].stPortParam[i].hPort = HI_INVALID_HANDLE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_Destroy(HI_HANDLE hVi)
{
    HI_S32 Ret = HI_FAILURE;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;
    HI_U32 j;

    CHECK_VI_INIT();
    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);
    if (VI_ENGINE_STARTED & g_Vi[enPort][u8Chn].u32State)
    {
        HI_ERR_VI("VI is still started, stop first.\n");
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    if (VI_ENGINE_ATTACHED & g_Vi[enPort][u8Chn].u32State)
    {
        HI_ERR_VI("VI is still in attach mode\n");
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    Ret = ioctl(g_ViDevFd, CMD_VI_DESTROY, &hVi);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memset(&g_Vi[enPort][u8Chn], 0x0, sizeof(VI_S));
    g_Vi[enPort][u8Chn].hVi   = HI_INVALID_HANDLE;
    g_Vi[enPort][u8Chn].hVpss = VPSS_INVALID_HANDLE;
    for (j = 0; j < VI_MAX_VPSS_PORT; j++)
    {
        g_Vi[enPort][u8Chn].stPortParam[j].hDst  = HI_INVALID_HANDLE;
        g_Vi[enPort][u8Chn].stPortParam[j].hPort = HI_INVALID_HANDLE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_SetAttr(HI_HANDLE hVi, HI_UNF_VI_ATTR_S *pstAttr)
{
    HI_S32 Ret = HI_FAILURE;
    VI_ATTR_S stViAttr;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pstAttr);
    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    Ret = VI_CheckAttr(enPort, pstAttr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    if (VI_ENGINE_STARTED & g_Vi[enPort][u8Chn].u32State)
    {
        HI_ERR_VI("unsupport set attributes in start mode\n");
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    stViAttr.hVi = hVi;
    memcpy(&(stViAttr.stAttr), pstAttr, sizeof(HI_UNF_VI_ATTR_S));

    Ret = ioctl(g_ViDevFd, CMD_VI_SET_ATTR, &stViAttr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(&g_Vi[enPort][u8Chn].stAttr, pstAttr, sizeof(HI_UNF_VI_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_GetAttr(HI_HANDLE hVi, HI_UNF_VI_ATTR_S *pstAttr)
{
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pstAttr);
    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    memcpy(pstAttr, &g_Vi[enPort][u8Chn].stAttr, sizeof(HI_UNF_VI_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_Attach(HI_HANDLE hVi, HI_HANDLE hDst)
{
    HI_S32 Ret = HI_FAILURE;
    HI_DRV_WIN_INFO_S stWinInfo;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;
    HI_U32 i;

    CHECK_VI_INIT();
    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    if (HI_INVALID_HANDLE == hDst)
    {
        HI_ERR_VI("para hDst is invalid.\n");
        return HI_ERR_VI_INVALID_PARA;
    }

    switch ((hDst >> 16) & 0xff)
    {
    case HI_ID_VO:
    {
        Ret = HI_MPI_WIN_GetInfo(hDst, &stWinInfo);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_VI("HI_MPI_WIN_GetInfo failed, ret = 0x%08x\n", Ret);
            return Ret;
        }

        if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == stWinInfo.eType)
        {
            for (i = 0; i < VI_MAX_VPSS_PORT; i++)
            {
                if ((g_Vi[enPort][u8Chn].stPortParam[i].hDst == stWinInfo.hPrim)
                    || (g_Vi[enPort][u8Chn].stPortParam[i].hDst == stWinInfo.hSec))
                {
                    HI_ERR_VI("this window is already attached.\n");
                    return HI_FAILURE;
                }
            }

            Ret = VI_AttachToVpss(hVi, stWinInfo.hPrim);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_VI("VI_AttachToVpss failed, ret = 0x%08x\n", Ret);
                return Ret;
            }

#ifdef HI_VI_SD_CREATE_WINDOW

            Ret = VI_AttachToVpss(hVi, stWinInfo.hSec);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_VI("VI_AttachToVpss failed, ret = 0x%08x\n", Ret);
                return Ret;
            }
#endif
        }
        else if (HI_DRV_WIN_ACTIVE_SINGLE == stWinInfo.eType)
        {
            for (i = 0; i < VI_MAX_VPSS_PORT; i++)
            {
                if (g_Vi[enPort][u8Chn].stPortParam[i].hDst == hDst)
                {
                    HI_ERR_VI("this window is already attached.\n");
                    return HI_FAILURE;
                }
            }

            Ret = VI_AttachToVpss(hVi, hDst);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_VI("VI_AttachToVpss failed, ret = 0x%08x\n", Ret);
                return Ret;
            }
        }
        else
        {
            HI_ERR_VI("vi does not support attach virtual window.\n");
            return HI_ERR_VI_NOT_SUPPORT;
        }

        break;
    }
    case HI_ID_VENC:
    {
        for (i = 0; i < VI_MAX_VPSS_PORT; i++)
        {
            if (g_Vi[enPort][u8Chn].stPortParam[i].hDst == hDst)
            {
                HI_ERR_VI("this VENC is already attached.\n");
                return HI_FAILURE;
            }
        }

        Ret = VI_AttachToVpss(hVi, hDst);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_VI("VI_AttachToVpss failed, ret = 0x%08x\n", Ret);
            return Ret;
        }

        break;
    }
    default:
    {
        HI_ERR_VI("vi only support attach to win or venc.\n");
        return HI_ERR_VI_NOT_SUPPORT;
    }
    }

    g_Vi[enPort][u8Chn].u32State |= VI_ENGINE_ATTACHED;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_Detach(HI_HANDLE hVi, HI_HANDLE hDst)
{
    HI_S32 Ret = HI_FAILURE;
    HI_DRV_WIN_INFO_S WinInfo;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;
    HI_U32 i;

    CHECK_VI_INIT();
    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    if (HI_INVALID_HANDLE == hDst)
    {
        HI_ERR_VI("para hDst is invalid.\n");
        return HI_ERR_VI_INVALID_PARA;
    }

    switch ((hDst >> 16) & 0xff)
    {
    case HI_ID_VO:
    {
        Ret = HI_MPI_WIN_GetInfo(hDst, &WinInfo);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_VI("HI_MPI_WIN_GetInfo failed, ret = 0x%08x\n", Ret);
            return Ret;
        }

        if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinInfo.eType)
        {
            Ret = VI_DetachFromVpss(hVi, WinInfo.hPrim);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_VI("vi detach window failed, ret = 0x%08x.\n", Ret);
                return Ret;
            }
#ifdef HI_VI_SD_CREATE_WINDOW
            Ret = VI_DetachFromVpss(hVi, WinInfo.hSec);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_VI("vi detach window failed, ret = 0x%08x.\n", Ret);
                return Ret;
            }
#endif
        }
        else if (HI_DRV_WIN_ACTIVE_SINGLE == WinInfo.eType)
        {
            Ret = VI_DetachFromVpss(hVi, hDst);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_VI("vi detach window failed, ret = 0x%08x.\n", Ret);
                return Ret;
            }
        }
        else
        {
            HI_ERR_VI("vi does not support detach virtual window.\n");
            return HI_FAILURE;
        }

        break;
    }
    case HI_ID_VENC:
    {
        Ret = VI_DetachFromVpss(hVi, hDst);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_VI("vi detach venc failed, ret = 0x%08x.\n", Ret);
            return Ret;
        }

        break;
    }
    default:
    {
        HI_ERR_VI("vi only support detach from win or venc.\n");
        return HI_ERR_VI_NOT_SUPPORT;
    }
    }

    for (i = 0; i < VI_MAX_VPSS_PORT; i++)
    {
        if (g_Vi[enPort][u8Chn].stPortParam[i].hDst != HI_INVALID_HANDLE)
        {
            break;
        }
    }

    if (i == VI_MAX_VPSS_PORT)
    {
        g_Vi[enPort][u8Chn].u32State &= ~VI_ENGINE_ATTACHED;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_SetExternBuffer(HI_HANDLE hVi, HI_UNF_VI_BUFFER_ATTR_S* pstBufAttr)
{
    HI_S32 Ret = HI_FAILURE;
    VI_BUF_ATTR_S stViBufAttr;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;
    HI_U32 i;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pstBufAttr);
    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    if ((pstBufAttr->u32BufNum > MAX_VI_FB_NUM) || (pstBufAttr->u32BufNum < MIN_VI_FB_NUM))
    {
        HI_ERR_VI("invalid buffer number %d.\n", pstBufAttr->u32BufNum);
        return HI_ERR_VI_INVALID_PARA;
    }

    for (i = 0; i < pstBufAttr->u32BufNum; i++)
    {
        if (!pstBufAttr->u32PhyAddr[i])
        {
            HI_ERR_VI("invalid u32PhyAddr[%d] 0.\n", i);
            return HI_ERR_VI_INVALID_PARA;
        }
    }

    if ((HI_TRUE == g_Vi[enPort][u8Chn].stAttr.bVirtual)
        || ((HI_FALSE == g_Vi[enPort][u8Chn].stAttr.bVirtual)
            && (HI_UNF_VI_BUF_MMAP != g_Vi[enPort][u8Chn].stAttr.enBufMgmtMode)))
    {
        HI_ERR_VI("only support in MMAP mode of REAL VI\n");
        return HI_ERR_VI_NOT_SUPPORT;
    }

    if (VI_ENGINE_STARTED & g_Vi[enPort][u8Chn].u32State)
    {
        HI_ERR_VI("unsupport set ext buffer in start mode\n");
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    stViBufAttr.hVi = hVi;
    memcpy(&(stViBufAttr.stBufAttr), pstBufAttr, sizeof(HI_UNF_VI_BUFFER_ATTR_S));
    Ret = ioctl(g_ViDevFd, CMD_VI_SET_BUF, &stViBufAttr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    g_Vi[enPort][u8Chn].u32State |= VI_ENGINE_BUFSET;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_Start(HI_HANDLE hVi)
{
    HI_S32 Ret = HI_FAILURE;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;

    CHECK_VI_INIT();
    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    if (VI_ENGINE_STARTED & g_Vi[enPort][u8Chn].u32State)
    {
        return HI_SUCCESS;
    }

    if ((!(VI_ENGINE_BUFSET & g_Vi[enPort][u8Chn].u32State)) && (HI_FALSE == g_Vi[enPort][u8Chn].stAttr.bVirtual)
        && (HI_UNF_VI_BUF_MMAP == g_Vi[enPort][u8Chn].stAttr.enBufMgmtMode))
    {
        HI_ERR_VI("external buffer should be set in MMAP mode of REAL VI\n");
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    Ret = ioctl(g_ViDevFd, CMD_VI_START, &hVi);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    g_Vi[enPort][u8Chn].u32State |= VI_ENGINE_STARTED;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_Stop(HI_HANDLE hVi)
{
    HI_S32 Ret = HI_FAILURE;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;

    CHECK_VI_INIT();
    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    if (!(VI_ENGINE_STARTED & g_Vi[enPort][u8Chn].u32State))
    {
        HI_ERR_VI("VI is not started.\n");
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    Ret = ioctl(g_ViDevFd, CMD_VI_STOP, &hVi);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    g_Vi[enPort][u8Chn].u32State &= ~VI_ENGINE_STARTED;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_QueueFrame(HI_HANDLE hVi, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 Ret = HI_FAILURE;
    VI_FRAME_S ViFrame;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pFrameInfo);
    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    if (HI_FALSE == g_Vi[enPort][u8Chn].stAttr.bVirtual)
    {
        HI_ERR_VI("not support in real VI scene\n");
        return HI_ERR_VI_NOT_SUPPORT;
    }

    if (!(VI_ENGINE_STARTED & g_Vi[enPort][u8Chn].u32State))
    {
        HI_WARN_VI("VI is not started.\n");
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    ViFrame.hVi = hVi;
    memcpy(&(ViFrame.stViFrame), pFrameInfo, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));
    Ret = ioctl(g_ViDevFd, CMD_VI_Q_FRAME, &ViFrame);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_DequeueFrame(HI_HANDLE hVi, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 Ret = HI_FAILURE;
    VI_FRAME_S ViFrame;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pFrameInfo);
    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    if (HI_FALSE == g_Vi[enPort][u8Chn].stAttr.bVirtual)
    {
        HI_ERR_VI("not support in real VI scene\n");
        return HI_ERR_VI_NOT_SUPPORT;
    }

    if (!(VI_ENGINE_STARTED & g_Vi[enPort][u8Chn].u32State))
    {
        HI_WARN_VI("VI is not started.\n");
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    ViFrame.hVi = hVi;
    Ret = ioctl(g_ViDevFd, CMD_VI_DQ_FRAME, &ViFrame);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(pFrameInfo, &(ViFrame.stViFrame), sizeof(HI_UNF_VIDEO_FRAME_INFO_S));
    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_AcquireFrame(HI_HANDLE hVi, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo, HI_U32 u32TimeoutMs)
{
    HI_S32 Ret = HI_FAILURE;
    VI_FRAME_S ViFrame;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pFrameInfo);
    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    if (!(VI_ENGINE_STARTED & g_Vi[enPort][u8Chn].u32State))
    {
        HI_WARN_VI("VI is not started.\n");
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    if (VI_ENGINE_ATTACHED & g_Vi[enPort][u8Chn].u32State)
    {
        HI_ERR_VI("unsupport acquire frame in attach mode\n");
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    ViFrame.hVi = hVi;
    ViFrame.u32TimeoutMs = u32TimeoutMs;
    Ret = ioctl(g_ViDevFd, CMD_VI_ACQUIRE_FRAME, &ViFrame);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(pFrameInfo, &(ViFrame.stViFrame), sizeof(HI_UNF_VIDEO_FRAME_INFO_S));
    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_ReleaseFrame(HI_HANDLE hVi, const HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 Ret = HI_FAILURE;
    VI_FRAME_S ViFrame;
    HI_UNF_VI_E enPort;
    HI_U8 u8Chn;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pFrameInfo);
    CHECK_HANDLE_AND_GET_PORT_CHN(hVi, enPort, u8Chn);

    if (!(VI_ENGINE_STARTED & g_Vi[enPort][u8Chn].u32State))
    {
        HI_WARN_VI("VI is not started.\n");
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    if (VI_ENGINE_ATTACHED & g_Vi[enPort][u8Chn].u32State)
    {
        HI_ERR_VI("unsupport release frame in attach mode\n");
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    ViFrame.hVi = hVi;
    memcpy(&(ViFrame.stViFrame), pFrameInfo, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));

    Ret = ioctl(g_ViDevFd, CMD_VI_RELEASE_FRAME, &ViFrame);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
