/*
 * Copyright (C) 2011, Hewlett-Packard Development Company, L.P.
 *   Author: Sebastian Dröge <sebastian.droege@collabora.co.uk>, Collabora Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#ifndef __GST_OMX_VIDEO_DEC_H__
#define __GST_OMX_VIDEO_DEC_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideodecoder.h>

#include "gstomx.h"
#include <stdio.h>
#include <sys/time.h>

G_BEGIN_DECLS

#define GST_TYPE_OMX_VIDEO_DEC \
  (gst_omx_video_dec_get_type())
#define GST_OMX_VIDEO_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMX_VIDEO_DEC,GstOMXVideoDec))
#define GST_OMX_VIDEO_DEC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OMX_VIDEO_DEC,GstOMXVideoDecClass))
#define GST_OMX_VIDEO_DEC_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_OMX_VIDEO_DEC,GstOMXVideoDecClass))
#define GST_IS_OMX_VIDEO_DEC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMX_VIDEO_DEC))
#define GST_IS_OMX_VIDEO_DEC_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OMX_VIDEO_DEC))

typedef struct _GstOMXVideoDec GstOMXVideoDec;
typedef struct _GstOMXVideoDecClass GstOMXVideoDecClass;

#define VDEC_MAX_LENGTH 64
#define VDEC_DUMP_FILE_STORE_PATH_LENGTH 256

typedef enum __SmePlayerProcType
{
    E_SME_PROC_CMD_SHOW_INFO = 0,
    E_SME_PROC_CMD_SET_CMD
}E_SME_PROC_CMD_TYPE_E;

typedef struct _ProcVdecInfo
{
    struct timeval     vdec_create_time;
    unsigned long      file_frame_rate;
    unsigned long      cur_frame_rate;
    unsigned long      vdec_err_cnt;
    GstClockTime       fir_Iframe_vdecT;
    struct timeval     get_first_I_frame;
    unsigned long      Iframe_cnt;
}ST_VDEC_SHOW_INFO;

typedef struct __VideoDecCmdInfo
{
    char bDumpSinkStream;  // 0:stop dump sink data, 1:start dump sink data;
    char bDumpSrcStream;
    char cDumpFileStorePath[VDEC_DUMP_FILE_STORE_PATH_LENGTH];
}ST_VDEC_CMD_INFO;

typedef struct __VideoDecProcInifo {
    E_SME_PROC_CMD_TYPE_E   eProcCmdType;
    void*                   pBenchmarkHandle;

    ST_VDEC_SHOW_INFO    stShowInfo;
    ST_VDEC_CMD_INFO     stCmdInfo;
}ST_VIDEODEC_PROC_INFO;

struct _GstOMXVideoDec
{
  GstVideoDecoder parent;

  /* < protected > */
  GstOMXComponent *dec;
  GstOMXPort *dec_in_port, *dec_out_port;

  GstBufferPool *in_port_pool, *out_port_pool;

  /* < private > */
  GstVideoCodecState *input_state;
  GstBuffer *codec_data;
  /* TRUE if the component is configured and saw
   * the first buffer */
  gboolean started;
  gboolean first_frame;
  gulong last_width;
  gulong last_height;

  GstClockTime last_upstream_ts;

  /* Draining state */
  GMutex drain_lock;
  GCond drain_cond;
  /* TRUE if EOS buffers shouldn't be forwarded */
  gboolean draining;

  /* TRUE if upstream is EOS */
  gboolean eos;

  GstFlowReturn downstream_flow_ret;
#ifdef USE_OMX_TARGET_RPI
  GstOMXComponent *egl_render;
  GstOMXPort *egl_in_port, *egl_out_port;
  gboolean eglimage;
#endif

/* BEGIN: Added by liurongliang(l00180035), 2015/7/20 */
  guint input_count;
  gboolean is_dec_out;
/* END:   Added by liurongliang(l00180035), 2015/7/20 */
#ifdef USE_OMX_DUMP_DATA
  FILE *dump_codec_data;
  FILE *sk_frame_data;
  FILE *total_frame_data;
  gboolean flag_seek;
#endif
  ST_VDEC_SHOW_INFO vdec_proc_info;
  GMutex       cur_framerate_muxtex;
  gboolean     is_frame;

  gboolean     es_dump_switch;
  gboolean     raw_dump_switch;
  gboolean     is_need_codec;
  guint8 *     codec_data_bak;
  glong        code_data_bak_len;
  FILE *       dump_es_file;
  FILE *       dump_raw_file;
  gchar        dump_es_path[VDEC_DUMP_FILE_STORE_PATH_LENGTH];
  gchar        dump_raw_path[VDEC_DUMP_FILE_STORE_PATH_LENGTH];
  guint8       dump_es_index;
  guint8       dump_raw_index;

};

struct _GstOMXVideoDecClass
{
  GstVideoDecoderClass parent_class;

  GstOMXClassData cdata;

  gboolean (*is_format_change) (GstOMXVideoDec * self, GstOMXPort * port, GstVideoCodecState * state);
  gboolean (*set_format)       (GstOMXVideoDec * self, GstOMXPort * port, GstVideoCodecState * state);
  GstFlowReturn (*prepare_frame)   (GstOMXVideoDec * self, GstVideoCodecFrame *frame);
};

GType gst_omx_video_dec_get_type (void);

G_END_DECLS

#endif /* __GST_OMX_VIDEO_DEC_H__ */
