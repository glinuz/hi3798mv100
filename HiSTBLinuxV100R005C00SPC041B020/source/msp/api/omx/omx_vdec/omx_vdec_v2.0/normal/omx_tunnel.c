/*=================================================

Open MAX   Component: OMX Tunnel Implementation
This module contains the class definition for openMAX tunnel mode.
File:    omx_tunnel.c
Author:  yangyichang 00226912
Date:    23, 08, 2016.

=================================================*/

#include <string.h>

#include "omx_dbg.h"
#include "omx_vdec.h"
#include "omx_tunnel.h"
#include "hi_math.h"
#include "hi_common.h"
#include "hi_drv_stat.h"
#include "hi_mpi_stat.h"
#include "hi_mpi_avplay.h"
#include "hi_unf_avplay.h"
#include "hi_unf_vo.h"


/*============== INTERNAL FUNCTION =============*/

static HI_S32 omx_tunnel_create_win(HI_HANDLE *hWin)
{
    HI_UNF_WINDOW_ATTR_S WinAttr;
    HI_S32 ret = HI_FAILURE;

    memset(&WinAttr, 0, sizeof(HI_UNF_WINDOW_ATTR_S));

    WinAttr.enDisp = HI_UNF_DISPLAY1;
    WinAttr.bVirtual = HI_FALSE;
    WinAttr.stWinAspectAttr.enAspectCvrs = HI_UNF_VO_ASPECT_CVRS_LETTERBOX;
    WinAttr.stWinAspectAttr.bUserDefAspectRatio = HI_FALSE;
    WinAttr.stWinAspectAttr.u32UserAspectWidth  = 0;
    WinAttr.stWinAspectAttr.u32UserAspectHeight = 0;
    WinAttr.bUseCropRect = HI_FALSE;
    WinAttr.stInputRect.s32X = 0;
    WinAttr.stInputRect.s32Y = 0;
    WinAttr.stInputRect.s32Width = 0;
    WinAttr.stInputRect.s32Height = 0;
    memset(&WinAttr.stOutputRect, 0x0, sizeof(HI_RECT_S));

    ret = HI_UNF_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: call HI_UNF_VO_Init failed!\n");

        return HI_FAILURE;
    }

    ret = HI_UNF_VO_CreateWindow(&WinAttr, hWin);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: call HI_UNF_VO_CreateWindow failed!\n");
        HI_UNF_VO_DeInit();

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 omx_tunnel_destroy_win(HI_HANDLE hWin)
{
    HI_UNF_VO_DestroyWindow(hWin);
    HI_UNF_VO_DeInit();

    return HI_SUCCESS;
}

static HI_S32 omx_tunnel_attach_win(HI_HANDLE hWin, OMX_U32 nAudioHwSync)
{
    HI_S32 ret = HI_FAILURE;

    ret = HI_UNF_VO_AttachWindow(hWin, nAudioHwSync);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("call HI_UNF_VO_AttachWindow failed.\n");

        return HI_FAILURE;
    }

    ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("call HI_UNF_VO_SetWindowEnable failed.\n");
        HI_UNF_VO_DetachWindow(hWin, nAudioHwSync);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 omx_tunnel_detach_win(HI_HANDLE hWin, OMX_U32 nAudioHwSync)
{
    HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, nAudioHwSync);

    return HI_SUCCESS;
}

static HI_S32 omx_tunnel_enable_avplay(OMX_TUNNEL_INFO *ptunnel_info, OMXVDEC_DRV_CONTEXT *pdrv_ctx)
{
    HI_S32 ret = HI_FAILURE;
    HI_UNF_AVPLAY_OPEN_OPT_S stOpenOpt;
    HI_UNF_VCODEC_ATTR_S stVdecAttr;
    VDEC_CHAN_CFG_S* pchan_cfg = HI_NULL;
    HI_UNF_AVPLAY_ATTR_S stAvplayAttr;
    OMXVDEC_DRV_CFG *drv_cfg = &pdrv_ctx->drv_cfg;
    OMX_U32 nAudioHwSync = ptunnel_info->config_param.nAudioHwSync;

#ifdef HI_OMX_TEE_SUPPORT
    ret = HI_UNF_AVPLAY_SetAttr(nAudioHwSync, HI_UNF_AVPLAY_ATTR_ID_TVP, &(drv_cfg->is_tvp));
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: avplay set TVP attr failed! TVP enable: %d, avplay handle: %ld\n", drv_cfg->is_tvp, nAudioHwSync);

        return HI_FAILURE;
    }
#endif

    memset(&stAvplayAttr, 0, sizeof(HI_UNF_AVPLAY_ATTR_S));

    ret = HI_UNF_AVPLAY_GetAttr(nAudioHwSync, HI_UNF_AVPLAY_ATTR_ID_STREAM_MODE, &stAvplayAttr);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: avplay Get VID Stream Mode attr failed!, avplay handle: %ld\n", nAudioHwSync);

        return HI_FAILURE;
    }

    stAvplayAttr.stStreamAttr.enStreamType  = HI_UNF_AVPLAY_STREAM_TYPE_ES;
    stAvplayAttr.stStreamAttr.u32VidBufSize = 0;
    ret = HI_UNF_AVPLAY_SetAttr(nAudioHwSync, HI_UNF_AVPLAY_ATTR_ID_STREAM_MODE, &stAvplayAttr);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: avplay set VID Stream BUF failed!, StreamBufSize: %d, avplay handle: %ld\n", stAvplayAttr.stStreamAttr.u32VidBufSize, nAudioHwSync);

        return HI_FAILURE;
    }

    memset(&stOpenOpt, 0, sizeof(stOpenOpt));

    stOpenOpt.enDecType       = HI_UNF_VCODEC_DEC_TYPE_NORMAL;
    stOpenOpt.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_H264;

    if (drv_cfg->cfg_width * drv_cfg->cfg_height >= 1920 * 1088)
    {
        stOpenOpt.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_4096x2160;
    }
    else
    {
        stOpenOpt.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
    }

    ret = HI_UNF_AVPLAY_ChnOpen(nAudioHwSync, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stOpenOpt);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: AVPLAY ChnOpen failed! ret: %d, avplay handle: %ld\n", ret, nAudioHwSync);

        return HI_FAILURE;
    }

    ret = HI_MPI_AVPLAY_SetTunnelMode(nAudioHwSync, HI_UNF_AVPLAY_MEDIA_CHAN_VID, drv_cfg->TunnelModeEnable);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: avplay Set tunnel Mode failed, call avplay chanclose! avplay handle: %ld\n", nAudioHwSync);
        HI_UNF_AVPLAY_ChnClose(nAudioHwSync, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

        return HI_FAILURE;
    }

    pchan_cfg = &drv_cfg->chan_cfg;
    OMX_VIDEO_FORMAT_SWITCH(pchan_cfg->eVidStd, stVdecAttr.enType);
    stVdecAttr.enMode         = HI_UNF_VCODEC_MODE_NORMAL;
    stVdecAttr.bOrderOutput   = HI_FALSE;
    stVdecAttr.u32ErrCover    = pchan_cfg->s32ChanErrThr;
    stVdecAttr.u32Priority    = 1;
    stVdecAttr.pCodecContext  = pdrv_ctx;

    ret = HI_UNF_AVPLAY_SetAttr(nAudioHwSync, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVdecAttr);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: avplay set attr fail! avplay handle: %ld, omx handle: %d\n", nAudioHwSync, pdrv_ctx->chan_handle);
        HI_UNF_AVPLAY_ChnClose(nAudioHwSync, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

        return HI_FAILURE;
    }

    DEBUG_PRINT_TUNNELED("Tunnel: avplay set attr success! avplay handle: %ld, omx handle: %d\n", nAudioHwSync, pdrv_ctx->chan_handle);

    return HI_SUCCESS;
}

static HI_S32 omx_tunnel_disable_avplay(OMX_TUNNEL_INFO *ptunnel_info)
{
    OMX_U32 nAudioHwSync = ptunnel_info->config_param.nAudioHwSync;

    HI_UNF_AVPLAY_ChnClose(nAudioHwSync, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

    return HI_SUCCESS;
}

/*============== EXTERNAL FUNCTION =============*/

OMX_ERRORTYPE omx_tunnel_init(OMX_COMPONENTTYPE *pcomp, OMX_PTR pInitParam)
{
    HI_HANDLE hWin;
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    ConfigureVideoTunnelModeParams *pParams = (ConfigureVideoTunnelModeParams *)pInitParam;
    OMX_COMPONENT_PRIVATE *pcomp_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;
    OMX_TUNNEL_INFO *ptunnel_info = &pcomp_priv->tunnel_info;

    if (pParams->nPortIndex != OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("omx_tunnel_init: Bad port index %ud\n", (int)pParams->nPortIndex);

        return OMX_ErrorBadPortIndex;
    }

    if (pParams->nAudioHwSync == HI_INVALID_HANDLE)
    {
        DEBUG_PRINT_ERROR("omx_tunnel_init: Bad Handle %ld\n", pParams->nAudioHwSync);

        return OMX_ErrorBadParameter;
    }

    ret = omx_tunnel_create_win(&hWin);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: Create win failed!\n");

        return OMX_ErrorNotReady;
    }


    ptunnel_info->native_handle.version = sizeof(native_handle_t);
    ptunnel_info->native_handle.numFds  = 0;
    ptunnel_info->native_handle.numInts = 1;
    ptunnel_info->native_handle.data[0] = hWin;

    ptunnel_info->win_handle = hWin;
    ptunnel_info->play_handle = pParams->nAudioHwSync;
    pcomp_priv->drv_ctx.drv_cfg.TunnelModeEnable = 1;

    pParams->pSidebandWindow = &(ptunnel_info->native_handle);

    memcpy(&(ptunnel_info->config_param), pParams, sizeof(ConfigureVideoTunnelModeParams));

    return OMX_ErrorNone;
}

OMX_ERRORTYPE omx_tunnel_deinit(OMX_COMPONENTTYPE *pcomp)
{
    OMX_COMPONENT_PRIVATE *pcomp_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;

    omx_tunnel_destroy_win(pcomp_priv->tunnel_info.win_handle);

    return OMX_ErrorNone;
}

OMX_ERRORTYPE omx_tunnel_enable(OMX_COMPONENTTYPE *pcomp)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_COMPONENT_PRIVATE *pcomp_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;
    OMX_TUNNEL_INFO *ptunnel_info = &pcomp_priv->tunnel_info;
    OMXVDEC_DRV_CONTEXT *pdrv_ctx = &pcomp_priv->drv_ctx;
    OMX_U32 nAudioHwSync = pcomp_priv->tunnel_info.config_param.nAudioHwSync;

    ret = omx_tunnel_enable_avplay(ptunnel_info, pdrv_ctx);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: Enable avplay failed, Destroy win! avplay handle: %ld, win handle: %d, omx handle: %d\n", nAudioHwSync, ptunnel_info->win_handle, pcomp_priv->drv_ctx.chan_handle);
        omx_tunnel_destroy_win(ptunnel_info->win_handle);

        return OMX_ErrorInsufficientResources;
    }

    ret = omx_tunnel_attach_win(ptunnel_info->win_handle, nAudioHwSync);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: Enable avplay failed, disable avplay, destory win!, avplay handle: %ld, win handle: %d, omx handle: %d\n", nAudioHwSync, ptunnel_info->win_handle, pcomp_priv->drv_ctx.chan_handle);
        omx_tunnel_disable_avplay(ptunnel_info);
        omx_tunnel_destroy_win(ptunnel_info->win_handle);

        return OMX_ErrorInsufficientResources;
    }

    return OMX_ErrorNone;
}

OMX_ERRORTYPE omx_tunnel_disable(OMX_COMPONENTTYPE *pcomp)
{
    OMX_COMPONENT_PRIVATE *pcomp_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;
    OMX_TUNNEL_INFO *ptunnel_info = &pcomp_priv->tunnel_info;

    omx_tunnel_detach_win(ptunnel_info->win_handle, ptunnel_info->config_param.nAudioHwSync);
    omx_tunnel_disable_avplay(ptunnel_info);

    return OMX_ErrorNone;
}

OMX_ERRORTYPE omx_tunnel_start(OMX_COMPONENTTYPE *pcomp)
{
    HI_S32 ret = HI_FAILURE;
    OMX_COMPONENT_PRIVATE *pcomp_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;
    OMX_TUNNEL_INFO *ptunnel_info = &pcomp_priv->tunnel_info;
    HI_HANDLE hAvplay = ptunnel_info->play_handle;

    ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: Start avplay failed, avplay handle: %d\n", hAvplay);

        return OMX_ErrorInsufficientResources;
    }

    return OMX_ErrorNone;
}

OMX_ERRORTYPE omx_tunnel_stop(OMX_COMPONENTTYPE *pcomp)
{
    HI_S32 ret = HI_FAILURE;
    OMX_COMPONENT_PRIVATE *pcomp_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;
    OMX_TUNNEL_INFO *ptunnel_info = &pcomp_priv->tunnel_info;
    HI_HANDLE hAvplay = ptunnel_info->play_handle;

    ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel: Start avplay failed, avplay handle: %d\n", hAvplay);

        return OMX_ErrorInsufficientResources;
    }

    return OMX_ErrorNone;
}

OMX_ERRORTYPE omx_tunnel_flush(OMX_COMPONENTTYPE *pcomp)
{
    HI_S32 ret = HI_FAILURE;
    OMX_COMPONENT_PRIVATE *pcomp_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;
    OMX_U32 nAudioHwSync = pcomp_priv->tunnel_info.config_param.nAudioHwSync;

    ret = HI_UNF_AVPLAY_Stop(nAudioHwSync, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel:: avplay flush Chn Error, avplay stop failed! avplay handle: %ld, omx handle: %d\n", nAudioHwSync, pcomp_priv->drv_ctx.chan_handle);

        return OMX_ErrorInsufficientResources;
    }

    ret = HI_UNF_AVPLAY_Start(nAudioHwSync, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Tunnel:: avplay flush Chn Error, avplay start failed! avplay handle: %ld, omx handle: %d\n", nAudioHwSync, pcomp_priv->drv_ctx.chan_handle);

        return OMX_ErrorInsufficientResources;
    }

    return OMX_ErrorNone;
}

