/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_hdmirx.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/11/19
  Description   :
  History       :
  1.Date        : 2014/01/16
    Author      : t00202585
    Modification: Created file
******************************************************************************/
//#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "hi_unf_hdmirx.h"
#include "hi_mpi_hdmirx.h"
#include "hi_drv_hdmirx.h"

#ifdef __cplusplus
 #if __cplusplus
        extern "C" {
 #endif
#endif /* __cplusplus */
static HI_MPI_HDMIRX_CTX_S s_stHdmirxMpiCtx = {HI_FALSE, HI_FALSE, -1};

HI_S32 HI_MPI_HDMIRX_Init(HI_VOID)
{
    HI_S32 s32Fd;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();

    if (HI_TRUE == pstCtx->bInited)
    {
        return HI_SUCCESS;
    }

    s32Fd = open("/dev/hi_hdmirx", O_RDWR | O_NONBLOCK);
    if (s32Fd < 0)
    {
        return HI_FAILURE;
    }

    pstCtx->s32Fd = s32Fd;
    pstCtx->bInited = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_HDMIRX_DeInit(HI_VOID)
{
    HI_S32 s32Ret;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();

    if (HI_FALSE == pstCtx->bInited)
    {
        return HI_SUCCESS;
    }

    pstCtx->bInited = HI_FALSE;

    s32Ret = close(pstCtx->s32Fd);

    return s32Ret;
}
HI_S32 HI_MPI_HDMIRX_Connect(HI_UNF_HDMIRX_PORT_E enPortIdx)
{
    HI_S32 s32Ret;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();

    HDMIRX_MPI_CHECK_INITED();

    if (enPortIdx >= HI_UNF_HDMIRX_PORT_BUTT)
    {
        return HI_FAILURE;
    }

    if (pstCtx->bConnected)
    {
        return HI_SUCCESS;
    }
    //HI_INFO_HDMIRX("enPortIdx = %d \n",enPortIdx);
    s32Ret = ioctl(pstCtx->s32Fd, HIIOC_HDMIRX_CONNECT, &enPortIdx);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    pstCtx->bConnected = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_HDMIRX_DisConnect(HI_VOID)
{
    HI_S32 s32Ret;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();

    HDMIRX_MPI_CHECK_INITED();

    if (HI_TRUE != pstCtx->bConnected)
    {
        return HI_SUCCESS;
    }

    s32Ret = ioctl(pstCtx->s32Fd, HIIOC_HDMIRX_DISCONNECT);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    pstCtx->bConnected = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_HDMIRX_GetHdcpStatus(HI_BOOL *pbHdcpSta)
{
    HI_S32 s32Ret;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();

    HDMIRX_MPI_CHECK_INITED();
    HDMIRX_MPI_CHECK_CONNECTED();

    if (NULL == pbHdcpSta)
    {
        return HI_FAILURE;
    }

    s32Ret = ioctl(pstCtx->s32Fd, HIIOC_HDMIRX_G_HDCP_STATUS, pbHdcpSta);

    return s32Ret;
}
HI_S32 HI_MPI_HDMIRX_GetTimingInfo(HI_UNF_HDMIRX_TIMING_INFO_S *pstTimingInfo)
{
    HI_S32 s32Ret;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();

    HDMIRX_MPI_CHECK_INITED();
    HDMIRX_MPI_CHECK_CONNECTED();

    if (NULL == pstTimingInfo)
    {
        return HI_FAILURE;
    }

    s32Ret = ioctl(pstCtx->s32Fd, HIIOC_HDMIRX_G_TIMING_INFO, pstTimingInfo);

    return s32Ret;
}

HI_S32 HI_MPI_HDMIRX_GetAudioInfo(HI_UNF_AI_HDMI_ATTR_S *pstAudioInfo)
{
    HI_S32 s32Ret;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();

    HDMIRX_MPI_CHECK_INITED();
    HDMIRX_MPI_CHECK_CONNECTED();

    if (NULL == pstAudioInfo)
    {
        return HI_FAILURE;
    }

    s32Ret = ioctl(pstCtx->s32Fd, HIIOC_HDMIRX_G_AUDIO_INFO, pstAudioInfo);

    return s32Ret;
}

HI_S32 HI_MPI_HDMIRX_GetSigStatus(HI_UNF_SIG_STATUS_E *penSignalStatus)
{
    HI_S32 s32Ret;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();

    HDMIRX_MPI_CHECK_INITED();
    HDMIRX_MPI_CHECK_CONNECTED();

    if (NULL == penSignalStatus)
    {
        return HI_FAILURE;
    }

    s32Ret = ioctl(pstCtx->s32Fd, HIIOC_HDMIRX_G_STATUS, penSignalStatus);

    return s32Ret;
}

HI_S32 HI_MPI_HDMIRX_GetAudioStatus(HI_UNF_SIG_STATUS_E *penStatus)
{
    HI_S32 s32Ret;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();

    HDMIRX_MPI_CHECK_INITED();
    HDMIRX_MPI_CHECK_CONNECTED();

    if (NULL == penStatus)
    {
        return HI_FAILURE;
    }

    s32Ret = ioctl(pstCtx->s32Fd, HIIOC_HDMIRX_G_AudioSTATUS, penStatus);

    return s32Ret;
}

HI_S32 HI_MPI_HDMIRX_UpdateEdid(HI_UNF_HDMIRX_EDID_S *pstEdid)
{
    HI_S32 s32Ret;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();

    HDMIRX_MPI_CHECK_INITED();
    if (NULL == pstEdid)
    {
        return HI_FAILURE;
    }

    s32Ret = ioctl(pstCtx->s32Fd, HIIOC_HDMIRX_S_UPDATE_EDID, pstEdid);

    return s32Ret;
}
/*
HI_S32 HI_MPI_HDMIRX_EnMonitor(HI_BOOL bEnable)
{
    HI_S32 s32Ret;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();

    HDMIRX_MPI_CHECK_INITED();
    HDMIRX_MPI_CHECK_CONNECTED();

    s32Ret = ioctl(pstCtx->s32Fd, HIIOC_HDMIRX_EN_MONITOR, &bEnable);

    return s32Ret;
}
*/
HI_S32 HI_MPI_HDMIRX_GetOfflineDetStatus(HI_UNF_HDMIRX_PORT_E enPort, HI_BOOL *pbConnected)
{
    HI_S32 s32Ret;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();
    HI_UNF_HDMIRX_OFF_LINE_DET_S stOffLineDet;

    if (NULL == pbConnected)
    {
        return HI_FAILURE;
    }
    *pbConnected = HI_FALSE;
    HDMIRX_MPI_CHECK_INITED();

    if (enPort >= HI_UNF_HDMIRX_PORT_BUTT)
    {
        return HI_FAILURE;
    }
    stOffLineDet.enPort = enPort;
    s32Ret = ioctl(pstCtx->s32Fd, HIIOC_HDMIRX_G_OFF_LINE_DET_STA, &stOffLineDet);

    if(s32Ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }
    *pbConnected = stOffLineDet.bConnected;
    return HI_SUCCESS;
}
HI_S32 HI_MPI_HDMIRX_LoadHdcp(HI_UNF_HDMIRX_HDCP_S *pstHdcp)
{
    HI_S32 s32Ret;
    HI_MPI_HDMIRX_CTX_S *pstCtx = HDMIRX_MPI_GET_CTX();

    HDMIRX_MPI_CHECK_INITED();
    //HDMIRX_MPI_CHECK_CONNECTED();
    if (NULL == pstHdcp)
    {
        return HI_FAILURE;
    }

    s32Ret = ioctl(pstCtx->s32Fd, HIIOC_HDMIRX_S_LOAD_HDCP, pstHdcp);

    return s32Ret;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
