/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmevideosink_hihal.h
 * @brief   android hisi37xx vdec&vdisppipe
 * @author  XXX
 * @date    2015/12/4
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2015/12/4
 * Author : XXX
 * Desc   : Created file
 *
******************************************************************************/
#ifndef __GST_SME_VIDEO_SINK_HIHAL_H__
#define __GST_SME_VIDEO_SINK_HIHAL_H__

#include "sme_proc.h"
#include "gstsmecodecmap.h"
#include <sys/time.h>
#include "sme_wrap_hisi_hal_vo.h"
#include "gstsmeptscalibration.h"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#define GST_TYPE_SME_VIDEOSINK_HIHAL (gst_smevideosink_hihal_get_type())
#define GST_SME_VIDEOSINK_HIHAL(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_SME_VIDEOSINK_HIHAL, GstSmeVideoSinkHiHal))
#define GST_SME_VIDEOSINK_HIHAL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_SME_VIDEOSINK_HIHAL, GstSmeVideoSinkHiHal))
#define GST_IS_SME_VIDEOSINK_HIHAL(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SME_VIDEOSINK_HIHAL))
#define GST_IS_SME_VIDEOSINK_HIHAL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SME_VIDEOSINK_HIHAL))

typedef struct _GstSmeVideoSinkHiHal GstSmeVideoSinkHiHal;
typedef struct _GstSmeVideoSinkHiHalClass GstSmeVideoSinkHiHalClass;

typedef struct _GstVideoFrameMap
{
    gpointer pvAddr;
    GstBuffer* gstvbuf;
}GstVideoFrameMap;

typedef struct _GstMmzInfo
{
    HI_MMZ_BUF_S stMmzBuf;
    gpointer pVirAddr;
    gpointer pPhyAddr;
    gboolean mmz_is_vo;
}GstMmzInfo;

struct _GstSmeVideoSinkHiHal
{
    GstBaseSink             element;

    ST_SME_DISP_RECT        st_vdisp_rect;       // ”∆µœ‘ æ∑∂Œß
    SME_HAL_VDISP_PIPE_HDL  hdl_sme_pipe_hihal;
    gboolean                recieve_eos;
    void*                   android_surface;

    ST_SME_VIDEOINFO        stVideoInfo;
    GThread*                dequeue_thd;
    gboolean                is_dequeue_thd_exit;
    GMutex                  rendered_buf_muxtex;
    GQueue                 q_rendered_buf;
    GQueue                  q_to_be_Free;
    GQueue                 q_mmz_is_vo;
    GMutex                  mmz_is_vo_muxtex;
#ifdef USE_DUMP_DECED_DATA
    FILE*                   dump_deced_data;
#endif


    /* BEGIN:for display setting */
    E_SME_PLAYER_DISPLAY_MODE e_disp_mode;
    ST_DISP_RATIO st_disp_ratio;
    ST_OUT_RECT st_win_rect;

    E_SME_PLAYER_DISPLAY_MODE e_old_disp_mode;
    ST_DISP_RATIO st_old_disp_ratio;
    ST_OUT_RECT st_old_win_rect;
    gboolean first_frame_flag;
    /* END:for display setting */

    ST_VSINK_SHOW_INFO      vsink_proc_info;
    struct timeval          current_play_timeUs;

    //refresh
    gboolean                needs_refresh;

    gboolean                is_flushing;
    E_SME_PLAYER_SWITCH_CHANNEL_MODE  switch_channel_mode;
    gboolean                dump_raw_switch;
    FILE                    *dump_raw_loca;
    GstSmePTSCalibrateHDL pts_calibration_hdl;
    guint32               u32_late_count;
    guint32               u32_drop_interleave;
    E_SME_PLAYER_SWITCH_CHANNEL_MODE freeze_mode;
    gboolean              is_first_frame;
    gboolean              is_first_render;
};

struct _GstSmeVideoSinkHiHalClass
{
    GstBaseSinkClass parent_class;
};


gboolean
gst_smevideosink_hihal_register (GstPlugin * plugin);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif//__GST_SME_VIDEO_SINK_HIHAL_H__
