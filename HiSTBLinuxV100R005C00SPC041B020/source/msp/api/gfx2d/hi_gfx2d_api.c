/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: unf_gfx2d.c
* Description: Graphic 2D engine API declaration.
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-08-02              NULL         Create this file.
*******************************************************************************/
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "hi_gfx2d_api.h"
#include "hi_gfx2d_errcode.h"
#include "drv_gfx2d_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

/* device path */
static const HI_CHAR *gs_pszGFX2DDevPath = "/dev/hi_gfx2d";

/* device lock */
static pthread_mutex_t gs_GFX2DDevMutex = PTHREAD_MUTEX_INITIALIZER;

/* device description symbol */
static HI_S32 gs_s32GFX2DDevFd = -1;

/* device quoted count */
static HI_U32 gs_u32GFX2DDevRef = 0;

/* check dev id is invalid or not */
#define GFX2D_CHECK_DEVID(enDevId) do \
    {\
        if (enDevId >= HI_GFX2D_DEV_ID_BUTT)\
        {\
            return HI_ERR_GFX2D_INVALID_DEVID;\
        }\
    }while(0)

/* check dev is opened or not */
#define GFX2D_CHECK_DEVSTATE() do \
    {\
        if (-1 == gs_s32GFX2DDevFd)\
        {\
            return HI_ERR_GFX2D_DEV_NOT_OPEN; \
        } \
    } while (0)


HI_S32 HI_GFX2D_Open(const HI_GFX2D_DEV_ID_E enDevId)
{
    HI_S32 s32Ret;

    GFX2D_CHECK_DEVID(enDevId);

    s32Ret = pthread_mutex_lock(&gs_GFX2DDevMutex);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_ERR_GFX2D_SYS;
    }

    /* device opened, return afrer adding ref */
    if (gs_s32GFX2DDevFd != -1)
    {
        gs_u32GFX2DDevRef++;
        s32Ret = pthread_mutex_unlock(&gs_GFX2DDevMutex);
        if (HI_SUCCESS != s32Ret)
        {
            return HI_ERR_GFX2D_SYS;
        }
        return HI_SUCCESS;
    }

    gs_s32GFX2DDevFd = open(gs_pszGFX2DDevPath, O_RDWR, 0);
    if (-1 == gs_s32GFX2DDevFd)
    {
        s32Ret = pthread_mutex_unlock(&gs_GFX2DDevMutex);
        if (HI_SUCCESS != s32Ret)
        {
            return HI_ERR_GFX2D_SYS;
        }

        return HI_ERR_GFX2D_DEV_PERMISSION;
    }

    gs_u32GFX2DDevRef++;

    s32Ret = pthread_mutex_unlock(&gs_GFX2DDevMutex);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_ERR_GFX2D_SYS;
    }

    return HI_SUCCESS;
}

HI_S32 HI_GFX2D_Close(const HI_GFX2D_DEV_ID_E enDevId)
{
    HI_S32 s32Ret;

    s32Ret = pthread_mutex_lock(&gs_GFX2DDevMutex);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_ERR_GFX2D_SYS;
    }

    /* device is not open,return */
    if (-1 == gs_s32GFX2DDevFd)
    {
        s32Ret = pthread_mutex_unlock(&gs_GFX2DDevMutex);
        if (HI_SUCCESS != s32Ret)
        {
            return HI_ERR_GFX2D_SYS;
        }
        return HI_SUCCESS;
    }

    if (--gs_u32GFX2DDevRef == 0)
    {
        GFX2D_WAITALLDONE_CMD_S stCmd;

        memset(&stCmd, 0, sizeof(stCmd));

        /*Before close dev, wait all task done!*/
        stCmd.enDevId = enDevId;
        s32Ret = ioctl(gs_s32GFX2DDevFd, GFX2D_WAITALLDONE, &stCmd);
            
        close(gs_s32GFX2DDevFd);
        gs_s32GFX2DDevFd = -1;
        s32Ret |= pthread_mutex_unlock(&gs_GFX2DDevMutex);
        if (HI_SUCCESS != s32Ret)
        {
            return HI_ERR_GFX2D_SYS;
        }

        return HI_SUCCESS;
    }

    s32Ret = pthread_mutex_unlock(&gs_GFX2DDevMutex);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_ERR_GFX2D_SYS;
    }

    return HI_SUCCESS;
}

/*缩放时，输入宽度，高度大于等于2.*/
HI_S32 HI_GFX2D_ComposeSync(const HI_GFX2D_DEV_ID_E enDevId,
                            HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                            HI_GFX2D_SURFACE_S *pstDst,
                            const HI_U32 u32Timeout)
{
    HI_S32 s32Ret;
    GFX2D_COMPOSE_CMD_S stCmd;

    GFX2D_CHECK_DEVID(enDevId);

    GFX2D_CHECK_DEVSTATE();

    if (NULL == pstComposeList || NULL == pstDst || NULL == pstComposeList->pstCompose)
    {
        return HI_ERR_GFX2D_NULL_PTR;
    }

    memset(&stCmd, 0, sizeof(stCmd));

    stCmd.enDevId        = enDevId;
    stCmd.stDst          = *pstDst;
    stCmd.u32Timeout     = u32Timeout;
    stCmd.bSync          = HI_TRUE;

    stCmd.stComposeList.u32BgColor    = pstComposeList->u32BgColor;
    stCmd.stComposeList.u32ComposeCnt = pstComposeList->u32ComposeCnt;
    stCmd.stComposeList.u64Compose = (HI_U64)(unsigned long)pstComposeList->pstCompose;

    s32Ret = ioctl(gs_s32GFX2DDevFd, GFX2D_COMPOSE, &stCmd);
    if (s32Ret < 0)
    {
        return s32Ret;
    }

    pstDst->s32ReleaseFenceFd = stCmd.stDst.s32ReleaseFenceFd;

    return HI_SUCCESS;

}

HI_S32 HI_GFX2D_ComposeAsync(const HI_GFX2D_DEV_ID_E enDevId,
                             HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                             HI_GFX2D_SURFACE_S *pstDst)
{
    HI_S32 s32Ret;
    GFX2D_COMPOSE_CMD_S stCmd;
#ifdef GFX2D_FENCE_SUPPORT
    HI_U32 i;
#endif

    GFX2D_CHECK_DEVID(enDevId);

    GFX2D_CHECK_DEVSTATE();

    if (NULL == pstComposeList || NULL == pstDst || NULL == pstComposeList->pstCompose)
    {
        return HI_ERR_GFX2D_NULL_PTR;
    }

    memset(&stCmd, 0, sizeof(stCmd));

    stCmd.enDevId   = enDevId;
    stCmd.stDst     = *pstDst;
    stCmd.bSync     = HI_FALSE;

    stCmd.stComposeList.u32BgColor    = pstComposeList->u32BgColor;
    stCmd.stComposeList.u32ComposeCnt = pstComposeList->u32ComposeCnt;
    stCmd.stComposeList.u64Compose = (HI_U64)(unsigned long)pstComposeList->pstCompose;

    s32Ret = ioctl(gs_s32GFX2DDevFd, GFX2D_COMPOSE, &stCmd);
    if (s32Ret < 0)
    {
        return s32Ret;
    }

    pstDst->s32ReleaseFenceFd = stCmd.stDst.s32ReleaseFenceFd;

#ifdef GFX2D_FENCE_SUPPORT
    for (i = 0; i < pstComposeList->u32ComposeCnt; i++)
    {

        pstComposeList->pstCompose[i].stSurface.s32ReleaseFenceFd = dup(pstDst->s32ReleaseFenceFd);
        if (pstComposeList->pstCompose[i].stSurface.s32ReleaseFenceFd < 0)
        {
            return HI_ERR_GFX2D_SYS;
        }

    }
#endif

    return HI_SUCCESS;
}

HI_S32 HI_GFX2D_WaitAllDone(const HI_GFX2D_DEV_ID_E enDevId, const HI_U32 u32TimeOut)
{
    HI_S32 s32Ret;
    GFX2D_WAITALLDONE_CMD_S stCmd;

    GFX2D_CHECK_DEVID(enDevId);

    GFX2D_CHECK_DEVSTATE();

    memset(&stCmd, 0, sizeof(stCmd));

    stCmd.enDevId = enDevId;
    stCmd.u32Timeout = u32TimeOut;
    s32Ret = ioctl(gs_s32GFX2DDevFd, GFX2D_WAITALLDONE, &stCmd);
    if (s32Ret < 0)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_GFX2D_GetCmpSurfaceSize(const HI_GFX2D_DEV_ID_E enDevId,
                                  const HI_U32 u32Width, const HI_U32 u32Height,
                                  const HI_GFX2D_FMT_E enFmt, HI_U32 *pu32Size)
{
    HI_U32 u32Stride;

    GFX2D_CHECK_DEVID(enDevId);

    GFX2D_CHECK_DEVSTATE();

    if (HI_NULL == pu32Size)
    {
        return HI_ERR_GFX2D_NULL_PTR;
    }

    if (HI_GFX2D_FMT_ARGB8888 != enFmt)
    {
        return HI_ERR_GFX2D_UNSUPPORT;
    }

    if ((u32Width < 1) || (u32Width > 2560)
        || (u32Height < 1) || (u32Height > 1600))
    {
        return HI_ERR_GFX2D_INVALID_SURFACE_RESO;
    }

    if (u32Width & 0x1)
    {
        return HI_ERR_GFX2D_UNSUPPORT;
    }

#if HWC_VERSION >= 0x500
    u32Stride = ((u32Width * 2 + 0xff) & 0xffffff00);   //256 byte align

    u32Stride = (u32Stride % 512) ? u32Stride : (u32Stride + 256);  //256 * odd

    *pu32Size = ((16 * u32Height + 0xff) & 0xffffff00) * 2 + (u32Stride * u32Height) * 2;    //512 byte : compress head
#elif HWC_VERSION == 0x400
    u32Stride = (((HI_U32)((u32Width * 4) * 1.3) + 0xf) & 0xfffffff0);
    if (u32Stride < 320)
    {
        u32Stride = 320;
    }

    *pu32Size = u32Stride * u32Height;
#else
    u32Stride = ((u32Width * 2 + 0xff) & 0xffffff00);

    *pu32Size = u32Stride * u32Height  + ((u32Stride / 256 + 0xf) & 0xfffffff0) * u32Height;
    *pu32Size *= 2;

#endif

    return HI_SUCCESS;
}

HI_S32 HI_GFX2D_GetCmpSurfaceStride(const HI_GFX2D_DEV_ID_E enDevId,
                                    const HI_U32 u32Width, const HI_GFX2D_FMT_E enFmt,
                                    HI_U32 *pu32Stride)
{
    GFX2D_CHECK_DEVID(enDevId);

    GFX2D_CHECK_DEVSTATE();

    if (HI_NULL == pu32Stride)
    {
        return HI_ERR_GFX2D_NULL_PTR;
    }

    if (HI_GFX2D_FMT_ARGB8888 != enFmt)
    {
        return HI_ERR_GFX2D_UNSUPPORT;
    }

    if ((u32Width < 1) || (u32Width > 2560))
    {
        return HI_ERR_GFX2D_INVALID_SURFACE_RESO;
    }

    if (u32Width & 0x1)
    {
        return HI_ERR_GFX2D_UNSUPPORT;
    }

#if HWC_VERSION >= 0x500
    *pu32Stride = ((u32Width * 2 + 0xff) & 0xffffff00);   //256 byte align

    *pu32Stride = (*pu32Stride % 512) ? *pu32Stride : (*pu32Stride + 256);  //256 * odd
#elif HWC_VERSION == 0x400
    *pu32Stride = (((HI_U32)((u32Width * 4) * 1.3) + 0xf) & 0xfffffff0);
    if (*pu32Stride < 320)
    {
        *pu32Stride = 320;
    }
#else
    *pu32Stride = ((u32Width * 2 + 0xff) & 0xffffff00);
#endif

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/
