/*=================================================

Open MAX   Component: Video Decoder
This module contains the class definition for openMAX decoder component.
File:      vdec_drv_ctx.c
Author:    yangyichang 00226912
Date:      16, 03, 2013.

=================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <semaphore.h>
#include <errno.h>

#include "hi_math.h"

#include "omx_drv_ctx.h"
#include "omx_vdec.h"
#include "omx_dbg.h"


OMX_S32 channel_create(OMXVDEC_DRV_CONTEXT *drv_ctx)
{
    OMX_S32 vdec_fd = -1;
    OMX_S32 chan_handle = -1;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx)
    {
        DEBUG_PRINT_ERROR("%s failed param null\n", __func__);
        return -1;
    }

    msg.chan_num = -1;
    msg.in       = &drv_ctx->drv_cfg;
    msg.out      = &chan_handle;

    vdec_fd = drv_ctx->video_driver_fd;
    if (ioctl(vdec_fd, VDEC_IOCTL_CHAN_CREATE, &msg) < 0)
    {
        DEBUG_PRINT_ERROR("%s failed\n", __func__);
        return -1;
    }

    if(chan_handle < 0)
    {
        DEBUG_PRINT_ERROR("%s chan invalid\n", __func__);
        return -1;
    }

    drv_ctx->chan_handle = chan_handle;

    return 0;
}


OMX_S32 channel_release(OMXVDEC_DRV_CONTEXT *drv_ctx)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx || (drv_ctx->chan_handle < 0))
    {
        DEBUG_PRINT_ERROR("%s param invalid\n", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.in       = NULL;
    msg.out      = NULL;

    if (ioctl(vdec_fd, VDEC_IOCTL_CHAN_RELEASE, &msg) < 0)
    {
        DEBUG_PRINT_ERROR("%s ioctl failed\n", __func__);
        return -1;
    }

    drv_ctx->chan_handle = -1;

    return 0;
}


OMX_S32 channel_start(OMXVDEC_DRV_CONTEXT *drv_ctx)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx || (drv_ctx->chan_handle < 0))
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.in       = NULL;
    msg.out      = NULL;

    return ioctl(vdec_fd, VDEC_IOCTL_CHAN_START, &msg);
}


OMX_S32 channel_stop(OMXVDEC_DRV_CONTEXT *drv_ctx)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx || (drv_ctx->chan_handle < 0))
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.in       = NULL;
    msg.out      = NULL;

    return ioctl(vdec_fd, VDEC_IOCTL_CHAN_STOP, &msg);
}


OMX_S32 channel_pause(OMXVDEC_DRV_CONTEXT *drv_ctx)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx ||  (drv_ctx->chan_handle < 0))
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.in       = NULL;
    msg.out      = NULL;

    return ioctl(vdec_fd, VDEC_IOCTL_CHAN_PAUSE, &msg);
}


OMX_S32 channel_resume(OMXVDEC_DRV_CONTEXT *drv_ctx)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx || (drv_ctx->chan_handle < 0))
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.in       = NULL;
    msg.out      = NULL;

    return ioctl(vdec_fd, VDEC_IOCTL_CHAN_RESUME, &msg);
}


OMX_S32 channel_flush_port(OMXVDEC_DRV_CONTEXT *drv_ctx, OMX_U32 flush_dir)
{
    OMX_S32 dir;
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx || (drv_ctx->chan_handle < 0))
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    dir          = flush_dir;
    msg.in       = &dir;
    msg.out      = NULL;

    return ioctl(vdec_fd, VDEC_IOCTL_FLUSH_PORT, (void *)&msg);
}


OMX_S32 channel_get_msg(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_MSG_INFO *msginfo)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx || NULL == msginfo)
    {
        DEBUG_PRINT_ERROR("%s param invalid! drv_ctx(%p), msginfo(%p)",
                          __func__, drv_ctx, msginfo);
        return -1;
    }

    if (drv_ctx->chan_handle < 0)
    {
        DEBUG_PRINT_ERROR("%s chan_handle(%d) invalid!", __func__, drv_ctx->chan_handle);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.out      = msginfo;

    return ioctl(vdec_fd, VDEC_IOCTL_CHAN_GET_MSG, (void *)&msg);
}


OMX_S32 channel_stop_msg(OMXVDEC_DRV_CONTEXT *drv_ctx)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx || drv_ctx->chan_handle < 0)
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;

    return ioctl(vdec_fd, VDEC_IOCTL_CHAN_STOP_MSG, (void *)&msg);
}


OMX_S32 channel_submit_stream(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_BUF_DESC *puser_buf)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx || NULL == puser_buf || drv_ctx->chan_handle < 0)
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.in       = puser_buf;
    msg.out      = NULL;

    return ioctl(vdec_fd, VDEC_IOCTL_EMPTY_INPUT_STREAM, (void *)&msg);
}


OMX_S32 channel_submit_frame(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_BUF_DESC *puser_buf)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx || NULL == puser_buf || drv_ctx->chan_handle < 0)
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.in       = puser_buf;
    msg.out      = NULL;

    return ioctl(vdec_fd, VDEC_IOCTL_FILL_OUTPUT_FRAME, (void *)&msg);
}


OMX_S32 channel_bind_buffer(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_BUF_DESC *puser_buf)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_BUF_DESC *pbuf_desc = puser_buf;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx || NULL == pbuf_desc || drv_ctx->chan_handle < 0)
    {
        DEBUG_PRINT_ERROR("%s invalid param.", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.in       = pbuf_desc;
    msg.out      = NULL;

    return ioctl(vdec_fd, VDEC_IOCTL_CHAN_BIND_BUFFER, (void *)&msg);
}


OMX_S32 channel_port_enable(OMXVDEC_DRV_CONTEXT* drv_ctx, OMX_BOOL bEnable)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, 0, 0};
    OMX_BOOL bPortEnable = bEnable;

    if (NULL == drv_ctx ||  (drv_ctx->chan_handle < 0))
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.in       = &bPortEnable;
    msg.out      = 0;
    DEBUG_PRINT_ALWS("%s,%d, bPortEnable = %d\n",__func__,__LINE__,bEnable);
    return ioctl(vdec_fd, VDEC_IOCTL_CHAN_PORT_ENABLE, &msg);
}


OMX_S32 channel_unbind_buffer(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_BUF_DESC *puser_buf)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, NULL, NULL};

    if (NULL == drv_ctx || NULL == puser_buf || drv_ctx->chan_handle < 0)
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.in       = puser_buf;
    msg.out      = NULL;

    return ioctl(vdec_fd, VDEC_IOCTL_CHAN_UNBIND_BUFFER, (void *)&msg);
}


OMX_S32 channel_alloc_buffer(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_BUF_DESC *puser_buf)
{
    OMX_S32 ret = 0;
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, 0, 0};

    if (NULL == drv_ctx || NULL == puser_buf || drv_ctx->chan_handle < 0)
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return -1;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.in       = puser_buf;
    msg.out      = puser_buf;

    ret = ioctl(vdec_fd, VDEC_IOCTL_CHAN_ALLOC_BUF, (void *)&msg);
    if (ret < 0)
    {
        DEBUG_PRINT_ERROR("alloc buffer failed\n");
        return ret;
    }
    /*
    DEBUG_PRINT_ALWS("Alloc buffer  vir:0x%p phy:0x%x, size:%d dir:%d sec:%d success!\n", \
         (void *)puser_buf->bufferaddr, puser_buf->phyaddr, puser_buf->buffer_len, puser_buf->dir, puser_buf->is_sec);
    */
    return ret;
}


void channel_release_buffer(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_BUF_DESC *puser_buf)
{
    OMX_S32 vdec_fd = -1;
    OMXVDEC_IOCTL_MSG msg = {0, 0, 0};
    OMX_S32 ret = 0;

    if (NULL == drv_ctx || NULL == puser_buf || drv_ctx->chan_handle < 0)
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return;
    }

    vdec_fd      = drv_ctx->video_driver_fd;
    msg.chan_num = drv_ctx->chan_handle;
    msg.in       = puser_buf;
    msg.out      = 0;

    ret = ioctl(vdec_fd, VDEC_IOCTL_CHAN_RELEASE_BUF, (void *)&msg);
    if (ret < 0)
    {
        DEBUG_PRINT_ERROR("release buffer failed\n");
        return;
    }
    /*
    DEBUG_PRINT_ALWS("Free buffer phy:0x%x dir:%d  sec:%d success!\n", \
                  puser_buf->phyaddr, puser_buf->dir, puser_buf->is_sec);
    */
}


void vdec_deinit_drv_context(OMXVDEC_DRV_CONTEXT  *drv_ctx)
{
    OMX_S32 ret;

    if (NULL == drv_ctx)
    {
        DEBUG_PRINT_ERROR("%s failed", __func__);
        return;
    }

    if (drv_ctx->chan_handle >= 0)
    {
        ret = channel_release(drv_ctx);
        if (ret != 0)
        {
            DEBUG_PRINT_ERROR("channel_release handle(%d) return error\n", drv_ctx->chan_handle);
        }
    }

    if (drv_ctx->video_driver_fd >= 0)
    {
        close(drv_ctx->video_driver_fd);
        drv_ctx->video_driver_fd = -1;
    }

    if (drv_ctx->yuv_fp != NULL)
    {
        fclose(drv_ctx->yuv_fp);
        drv_ctx->yuv_fp = NULL;
    }

    if (drv_ctx->chrom_l != NULL)
    {
        free(drv_ctx->chrom_l);
        drv_ctx->chrom_l = NULL;
        drv_ctx->chrom_l_size = 0;
    }

    return;
}


OMX_S32 vdec_init_drv_context(OMXVDEC_DRV_CONTEXT *drv_ctx)
{
    if (!drv_ctx)
    {
        DEBUG_PRINT_ERROR("%s invalid param\n", __func__);
        return -1;
    }

    drv_ctx->video_driver_fd = open(DRIVER_PATH, O_RDWR);
    if (drv_ctx->video_driver_fd < 0)
    {
        DEBUG_PRINT_ERROR("%s open %s failed, return %s\n", __func__, DRIVER_PATH, ((strerror(errno)!=NULL)? strerror(errno):"nothing"));
        return -1;
    }

    drv_ctx->chan_handle                  = -1;

    memset(&drv_ctx->drv_cfg, 0, sizeof(drv_ctx->drv_cfg));
#ifdef HI_OMX_TEE_SUPPORT
    drv_ctx->drv_cfg.is_tvp               = HI_TRUE;
#endif
    drv_ctx->drv_cfg.cfg_width            = DEFAULT_FRAME_WIDTH;
    drv_ctx->drv_cfg.cfg_height           = DEFAULT_FRAME_HEIGHT;
    drv_ctx->drv_cfg.cfg_stride           = HI_SYS_GET_STRIDE(DEFAULT_FRAME_WIDTH);
    drv_ctx->drv_cfg.cfg_inbuf_num        = MAX_IN_BUF_SLOT_NUM;
    drv_ctx->drv_cfg.cfg_outbuf_num       = MAX_OUT_BUF_SLOT_NUM;
    drv_ctx->drv_cfg.cfg_color_format     = HI_DRV_PIX_FMT_NV21;

    VDEC_CHAN_CFG_S *pchan_cfg = &(drv_ctx->drv_cfg.chan_cfg);
    memset(pchan_cfg, 0, sizeof(VDEC_CHAN_CFG_S));

    pchan_cfg->s32ChanPriority     = 1;
    pchan_cfg->s32ChanErrThr       = 100;
    pchan_cfg->s32DecMode          = IPB_MODE;
    pchan_cfg->s32DecOrderOutput   = 0;
    pchan_cfg->s32DnrDispOutEnable = 0;
    pchan_cfg->s32DnrTfEnable      = 0;
    pchan_cfg->s32ChanStrmOFThr    = 0;
    pchan_cfg->s32VcmpEn           = 0;
    pchan_cfg->s32VcmpWmStartLine  = 0;
    pchan_cfg->s32WmEn             = 0;
    pchan_cfg->s32VcmpWmEndLine    = 0;
    pchan_cfg->s32Btl1Dt2DEnable   = 1;
    pchan_cfg->s32BtlDbdrEnable    = 1;
    pchan_cfg->s32IsOmxPath        = 1;
    pchan_cfg->s32MaxRawPacketNum  = DEF_MAX_IN_BUF_CNT;
    pchan_cfg->s32MaxRawPacketSize = DEFAULT_IN_BUF_SIZE;

    return 0;
}


OMX_S8 channel_save_yuv(FILE* pFile, SaveYuvOption *pOption)
{
    OMX_U32 i, j;
    OMX_U32 tmp_len;
    OMX_U8* ul = NULL;
    OMX_U8* vl = NULL;
    OMX_U32 chrom_width = pOption->Width/2;
    OMX_U32 chrom_height = pOption->Height/2;

    if (!pFile || !pOption->pstChroml || !pOption->pYaddress || !pOption->pCaddress)
    {
        DEBUG_PRINT_ERROR("%s: INVALID PARAM!\n", __func__);
        return -1;
    }

    /*write y*/
    for (i=0; i<pOption->Height; i++)
    {
        if(pOption->Width != fwrite(pOption->pYaddress, 1, pOption->Width, pFile))
        {
            DEBUG_PRINT_ERROR("Y line %d: fwrite fail!\n",__LINE__);
            return -1;
        }
        pOption->pYaddress += pOption->Stride;
    }

    ul = pOption->pstChroml;
    vl = pOption->pstChroml + chrom_height*chrom_width;

    if (OMX_PIX_FMT_NV12 == pOption->ColorFormat)
    {
        for (i=0; i<chrom_height; i++)
        {
            for (j=0; j<chrom_width; j++)
            {
                 ul[i*chrom_width+j] = pOption->pCaddress[2*j];
                 vl[i*chrom_width+j] = pOption->pCaddress[2*j+1];
            }
            pOption->pCaddress += pOption->Stride;
        }
    }
    else if (OMX_PIX_FMT_NV21 == pOption->ColorFormat)
    {
        for (i=0; i<chrom_height; i++)
        {
            for (j=0; j<chrom_width; j++)
            {
                 vl[i*chrom_width+j] = pOption->pCaddress[2*j];
                 ul[i*chrom_width+j] = pOption->pCaddress[2*j+1];
            }
            pOption->pCaddress += pOption->Stride;
        }
    }
    else if (OMX_PIX_FMT_YUV420Planar== pOption->ColorFormat)
    {
        for (i=0; i<chrom_height; i++)
        {
            for (j=0; j<chrom_width; j++)
            {
                 ul[i*chrom_width+j] = pOption->pCaddress[j];
            }
            pOption->pCaddress += pOption->Stride/2;
        }
        for (i=0; i<chrom_height; i++)
        {
            for (j=0; j<chrom_width; j++)
            {
                 vl[i*chrom_width+j] = pOption->pCaddress[j];
            }
            pOption->pCaddress += pOption->Stride/2;
        }
    }

    tmp_len = chrom_width * chrom_height;
    if(tmp_len != fwrite(ul, 1, tmp_len, pFile))
    {
        DEBUG_PRINT_ERROR("U line %d: fwrite fail!\n",__LINE__);
        return -1;
    }

    if(tmp_len != fwrite(vl, 1, tmp_len, pFile))
    {
        DEBUG_PRINT_ERROR("V line %d: fwrite fail!\n",__LINE__);
        return -1;
    }

    fflush(pFile);

    return 0;
}

