/*=================================================

Open MAX   Component: Video Decoder
This module contains the class definition for openMAX decoder component.
File:    omx_drv_ctx.h
Author:  yangyichang 00226912
Date:    16, 03, 2013.

=================================================*/

#ifndef __VDEC_DRV_CTX_H__
#define __VDEC_DRV_CTX_H__

#include "drv_omxvdec.h"
#include "OMX_Types.h"


typedef struct SaveYuvOption {

    OMX_U32 ColorFormat;
    OMX_U8* pstChroml;
    OMX_U8 *pYaddress;
    OMX_U8 *pCaddress;
    OMX_U32 Width;
    OMX_U32 Height;
    OMX_U32 Stride;
}SaveYuvOption;

OMX_S32 vdec_init_drv_context(OMXVDEC_DRV_CONTEXT *drv_ctx);
void vdec_deinit_drv_context(OMXVDEC_DRV_CONTEXT *drv_ctx);
OMX_S32 channel_create(OMXVDEC_DRV_CONTEXT *drv_ctx);
OMX_S32 channel_release(OMXVDEC_DRV_CONTEXT *drv_ctx);
OMX_S32 channel_start(OMXVDEC_DRV_CONTEXT *drv_ctx);
OMX_S32 channel_stop(OMXVDEC_DRV_CONTEXT *drv_ctx);
OMX_S32 channel_pause(OMXVDEC_DRV_CONTEXT *drv_ctx);
OMX_S32 channel_resume(OMXVDEC_DRV_CONTEXT *drv_ctx);
OMX_S32 channel_flush_port(OMXVDEC_DRV_CONTEXT *drv_ctx, OMX_U32 flush_dir);
OMX_S32 channel_get_msg(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_MSG_INFO *msginfo);
OMX_S32 channel_submit_stream(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_BUF_DESC *puser_buf);
OMX_S32 channel_submit_frame(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_BUF_DESC *puser_buf);
OMX_S32 channel_bind_buffer(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_BUF_DESC *puser_buf);
OMX_S32 channel_unbind_buffer(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_BUF_DESC *puser_buf);
OMX_S8  channel_save_yuv(FILE* pFile, SaveYuvOption *pOption);
OMX_S32 channel_alloc_buffer(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_BUF_DESC *puser_buf);
void channel_release_buffer(OMXVDEC_DRV_CONTEXT *drv_ctx, OMXVDEC_BUF_DESC *puser_buf);
OMX_S32 channel_port_enable(OMXVDEC_DRV_CONTEXT* drv_ctx, OMX_BOOL bEnable);

#endif
