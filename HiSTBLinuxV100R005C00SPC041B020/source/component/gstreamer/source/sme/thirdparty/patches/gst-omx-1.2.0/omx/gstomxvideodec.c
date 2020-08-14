/*
 * Copyright (C) 2011, Hewlett-Packard Development Company, L.P.
 *   Author: Sebastian Dr枚ge <sebastian.droege@collabora.co.uk>, Collabora Ltd.
 * Copyright (C) 2013, Collabora Ltd.
 *   Author: Sebastian Dr枚ge <sebastian.droege@collabora.co.uk>
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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>


#if defined (USE_OMX_TARGET_RPI) && defined(__GNUC__)
#ifndef __VCCOREVER__
#define __VCCOREVER__ 0x04000000
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wredundant-decls"
#pragma GCC optimize ("gnu89-inline")
#endif

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
#include <gst/gl/gl.h>
#include <gst/gl/egl/gsteglimagememory.h>
#endif

#if defined (USE_OMX_TARGET_RPI) && defined(__GNUC__)
#pragma GCC reset_options
#pragma GCC diagnostic pop
#endif

#include <string.h>

#include "gstomxbufferpool.h"
#include "gstomxvideo.h"
#include "gstomxvideodec.h"

#include "gstomxvideobufferpool_hivo.h"

#ifndef SMEPLAYER_STAT_TAG_TIME
#define SMEPLAYER_STAT_TAG_TIME "PlayerTimeLineTrace"
#endif

#ifdef USE_OMX_DUMP_DATA
#define DUMP_CODEC_PATH "/mnt/codec.es"
#define DUMP_SK_DATA_PATH "/mnt/sk_data.es"
//#define DUMP_TOTAL_DATA_PATH "/mnt/dump/TotalFrameData.dat"
#define DUMP_TOTAL_DATA_PATH "/mnt/dec_in.es"
#endif

//#define GST_DEBUG GST_ERROR
//#define GST_INFO GST_ERROR
//#define GST_WARNING GST_ERROR

//#define GST_DEBUG_OBJECT GST_ERROR_OBJECT
//#define GST_DEBUG_INFO GST_ERROR_OBJECT
//#define GST_DEBUG_WARNING GST_ERROR_OBJECT

gulong huge_size = 0;
gulong bak_size = 0;

#define INVAILD_TIME_STAMP 4294967295000000
//#define INVAILD_TIME_STAMP -1
guint first_sync_frame = 0;
guint vaild_pts_num = 0;

GstClockTime last_vaild_pts = INVAILD_TIME_STAMP;

#define INPUT_OMX_SIZE 2097216

gint first_in = 0;

GstClockTime dec_pts = 0;
GstClockTime dec_in_cur_time = 0;
GstClockTime dec_in_last_time = 0;



GST_DEBUG_CATEGORY_STATIC (gst_omx_video_dec_debug_category);
#define GST_CAT_DEFAULT gst_omx_video_dec_debug_category

/* prototypes */
static guint64 gst_omx_video_dec_get_cur_systime()
{
    GTimeVal timeval;
    guint64 timetick = 0;
    g_get_current_time (&timeval);
    timetick = (guint64)timeval.tv_sec * 1000 +
        (timeval.tv_usec + 500) / 1000;
    return timetick;
}
static void gst_omx_video_dec_finalize (GObject * object);
static void gst_omx_vdec_set_property (GObject *object, guint prop_id, const GValue *value,
    GParamSpec *pspec);
static void gst_omx_vdec_get_property (GObject *object, guint prop_id, GValue *value,  GParamSpec *pspec);

static GstStateChangeReturn
gst_omx_video_dec_change_state (GstElement * element,
    GstStateChange transition);

static gboolean gst_omx_video_dec_open (GstVideoDecoder * decoder);
static gboolean gst_omx_video_dec_close (GstVideoDecoder * decoder);
static gboolean gst_omx_video_dec_start (GstVideoDecoder * decoder);
static gboolean gst_omx_video_dec_stop (GstVideoDecoder * decoder);
static gboolean gst_omx_video_dec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state);
static gboolean gst_omx_video_dec_flush (GstVideoDecoder * decoder);
static GstFlowReturn gst_omx_video_dec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame);
static GstFlowReturn gst_omx_video_dec_finish (GstVideoDecoder * decoder);
static gboolean gst_omx_video_dec_decide_allocation (GstVideoDecoder * bdec,
    GstQuery * query);

static GstFlowReturn gst_omx_video_dec_drain (GstOMXVideoDec * self,
    gboolean is_eos);

static OMX_ERRORTYPE gst_omx_video_dec_allocate_output_buffers (GstOMXVideoDec *
    self, GstAllocator* frame_allocator);
static OMX_ERRORTYPE gst_omx_video_dec_deallocate_output_buffers (GstOMXVideoDec
    * self);

static void gst_omx_video_dec_clean_all_frames (GstVideoDecoder * decoder);
static gboolean
gst_omx_video_dec_sink_event (GstVideoDecoder * decoder,
    GstEvent * event);

/* BEGIN: Added by liurongliang(l00180035), 2015/3/11 */
#define GST_MAX_INPUT_BUFFER_4K (5*1024*1024)
#define GST_MAX_INPUT_BUFFER_FHD (2*1024*1024)
#define GST_MAX_INPUT_BUFFER_ES (32*1024)
#define GST_FHD_WIDTH 1920
#define GST_FHD_HEIGHT 1080
#define GST_MIN_WIDTH 32
#define GST_MIN_HEIGHT 32
#define SME_DEC_DELAY_MAX_COUNT 1000000 //if input more than 100 frame and not dec out,we think dec not support.
#define SME_MAX_WIDTH_4K 3840
#define SME_MAX_HEIGHT_4K 2160
/* END:   Added by liurongliang(l00180035), 2015/3/11 */

enum
{
  PROP_0,
  PROP_VDEC_INFO,
  PROP_LAST
};

/* class initialization */

#define DEBUG_INIT \
  GST_DEBUG_CATEGORY_INIT (gst_omx_video_dec_debug_category, "omxvideodec", 0, \
      "debug category for gst-omx video decoder base class");


G_DEFINE_ABSTRACT_TYPE_WITH_CODE (GstOMXVideoDec, gst_omx_video_dec,
    GST_TYPE_VIDEO_DECODER, DEBUG_INIT);

static void
gst_omx_video_dec_class_init (GstOMXVideoDecClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
  GstVideoDecoderClass *video_decoder_class = GST_VIDEO_DECODER_CLASS (klass);

  gobject_class->finalize = gst_omx_video_dec_finalize;
  gobject_class->set_property = gst_omx_vdec_set_property;
  gobject_class->get_property = gst_omx_vdec_get_property;

  element_class->change_state =
      GST_DEBUG_FUNCPTR (gst_omx_video_dec_change_state);

  video_decoder_class->open = GST_DEBUG_FUNCPTR (gst_omx_video_dec_open);
  video_decoder_class->close = GST_DEBUG_FUNCPTR (gst_omx_video_dec_close);
  video_decoder_class->start = GST_DEBUG_FUNCPTR (gst_omx_video_dec_start);
  video_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_omx_video_dec_stop);
  video_decoder_class->flush = GST_DEBUG_FUNCPTR (gst_omx_video_dec_flush);
  video_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_omx_video_dec_set_format);
  video_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_omx_video_dec_handle_frame);
  video_decoder_class->finish = GST_DEBUG_FUNCPTR (gst_omx_video_dec_finish);
  video_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_omx_video_dec_decide_allocation);
  video_decoder_class->sink_event = GST_DEBUG_FUNCPTR (gst_omx_video_dec_sink_event);

  g_object_class_install_property (gobject_class, PROP_VDEC_INFO,
        g_param_spec_pointer ("omx-video-decoder-info", "OMX-Video-Decoder-Info",
          "Poniter of Video Decoder",
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  klass->cdata.type = GST_OMX_COMPONENT_TYPE_FILTER;
  klass->cdata.default_src_template_caps =
#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
      GST_VIDEO_CAPS_MAKE_WITH_FEATURES (GST_CAPS_FEATURE_MEMORY_EGL_IMAGE,
      "RGBA") "; "
#endif
      "video/x-raw, "
      "width = " GST_VIDEO_SIZE_RANGE ", "
      "height = " GST_VIDEO_SIZE_RANGE ", " "framerate = " GST_VIDEO_FPS_RANGE;
}

static void
gst_omx_video_dec_init (GstOMXVideoDec * self)
{
  GST_INFO_OBJECT(self, "gst_omx_video_dec_init in");

  gst_video_decoder_set_packetized (GST_VIDEO_DECODER (self), TRUE);

  g_mutex_init (&self->drain_lock);
  g_cond_init (&self->drain_cond);
  g_mutex_init(&self->cur_framerate_muxtex);
  memset(&self->vdec_proc_info, 0, sizeof(self->vdec_proc_info));
  self->vdec_proc_info.vdec_err_cnt = 1;
  self->es_dump_switch = FALSE;
  self->raw_dump_switch = FALSE;
  self->is_need_codec = FALSE;
  self->codec_data_bak = NULL;
  self->code_data_bak_len = 0;
  self->dump_es_index = 0;
  self->dump_raw_index = 0;
  GST_INFO_OBJECT(self, "gst_omx_video_dec_init out");
}

static gboolean
gst_omx_video_dec_sink_event (GstVideoDecoder * decoder,
    GstEvent * event)
{
  gboolean ret;
  GstOMXVideoDec *self = GST_OMX_VIDEO_DEC (decoder);
  GstEventType ev_type = GST_EVENT_TYPE (event);

  ret = GST_VIDEO_DECODER_CLASS(gst_omx_video_dec_parent_class)->sink_event (
    decoder, event);
  if(GST_EVENT_FLUSH_STOP == ev_type){
    GST_INFO_OBJECT(self, "video flush stop clear some var begin");
    GST_VIDEO_DECODER_STREAM_LOCK (decoder);
    self->eos = FALSE;
    self->downstream_flow_ret = GST_FLOW_OK;
    self->input_count = 0;
    self->is_dec_out = FALSE;
    GST_VIDEO_DECODER_STREAM_UNLOCK (decoder);
    GST_INFO_OBJECT(self, "video flush stop clear some var end");
  }

  return ret;
}

static void
gst_omx_vdec_set_property (GObject *object, guint prop_id,
    const GValue *value,  GParamSpec *pspec)
{
    GstOMXVideoDec* self = (GstOMXVideoDec*)object;
    (void)pspec;

    switch (prop_id) {
        case PROP_VDEC_INFO:
        {
            ST_VIDEODEC_PROC_INFO *pstVdecProcInfo;
            ST_VDEC_SHOW_INFO *pstVdecShowInfo;
            ST_VDEC_CMD_INFO *pstVdecCmdInfo;

            pstVdecProcInfo = g_value_get_pointer (value);

            if (pstVdecProcInfo && pstVdecProcInfo->eProcCmdType == E_SME_PROC_CMD_SHOW_INFO)
            {
                pstVdecShowInfo = &(pstVdecProcInfo->stShowInfo);

                memcpy(pstVdecShowInfo, &self->vdec_proc_info, sizeof(self->vdec_proc_info));

                GST_INFO("Collect video decoder info for proc");
            }
            else if (pstVdecProcInfo && pstVdecProcInfo->eProcCmdType == E_SME_PROC_CMD_SET_CMD)
            {
                pstVdecCmdInfo = &(pstVdecProcInfo->stCmdInfo);
                GST_INFO("Video Decoder bDumpSinkStream=%d", pstVdecCmdInfo->bDumpSinkStream);
                GST_INFO("Video Decoder bDumpSrcStream=%d", pstVdecCmdInfo->bDumpSrcStream);
                GST_INFO("Video Decoder cDumpFileStorePath=%s", pstVdecCmdInfo->cDumpFileStorePath);
                if (pstVdecCmdInfo->bDumpSinkStream != 0XFF) {
                    if (pstVdecCmdInfo->bDumpSinkStream == 1)
                    {
                      //begin to start dump sink data;
                      self->is_need_codec = TRUE;
                      memcpy(self->dump_es_path, pstVdecCmdInfo->cDumpFileStorePath,VDEC_DUMP_FILE_STORE_PATH_LENGTH);
                      self->es_dump_switch = TRUE;
                    }
                    else if(pstVdecCmdInfo->bDumpSinkStream == 0)
                    {
                      //stop dump sink data
                      self->es_dump_switch = FALSE;
                      self->is_need_codec = FALSE;
                    }
                }

                if (pstVdecCmdInfo->bDumpSrcStream != 0xFF) {
                    if (pstVdecCmdInfo->bDumpSrcStream == 1)
                    {
                      //begin to start dump src data;
                      memcpy(self->dump_raw_path, pstVdecCmdInfo->cDumpFileStorePath,VDEC_DUMP_FILE_STORE_PATH_LENGTH);
                      self->raw_dump_switch = TRUE;
                    }
                    else if(pstVdecCmdInfo->bDumpSrcStream == 0)
                    {
                      //stop dump src data
                      self->raw_dump_switch = FALSE;
                    }
                }
            }
            break;
        }
        default:
            GST_WARNING("gstomxvdec not support setproperty=%d", prop_id);
            break;
    }

    return;
}

static void
gst_omx_vdec_get_property (GObject *object, guint prop_id,
    GValue *value,  GParamSpec *pspec)
{
    return;
}


static gboolean
gst_omx_video_dec_open (GstVideoDecoder * decoder)
{
  GstOMXVideoDec *self = GST_OMX_VIDEO_DEC (decoder);
  GstOMXVideoDecClass *klass = GST_OMX_VIDEO_DEC_GET_CLASS (self);
  gint in_port_index, out_port_index;

  GST_DEBUG_OBJECT (self, "Opening decoder");

  self->dec =
      gst_omx_component_new (GST_OBJECT_CAST (self), klass->cdata.core_name,
      klass->cdata.component_name, klass->cdata.component_role,
      klass->cdata.hacks);
  self->started = FALSE;

  if (!self->dec)
    return FALSE;

  if (gst_omx_component_get_state (self->dec,
          GST_CLOCK_TIME_NONE) != OMX_StateLoaded)
    return FALSE;

  in_port_index = klass->cdata.in_port_index;
  out_port_index = klass->cdata.out_port_index;

  if (in_port_index == -1 || out_port_index == -1) {
    OMX_PORT_PARAM_TYPE param;
    OMX_ERRORTYPE err;

    GST_OMX_INIT_STRUCT (&param);

    err =
        gst_omx_component_get_parameter (self->dec, OMX_IndexParamVideoInit,
        &param);
    if (err != OMX_ErrorNone) {
      GST_WARNING_OBJECT (self, "Couldn't get port information: %s (0x%08x)",
          gst_omx_error_to_string (err), err);
      /* Fallback */
      in_port_index = 0;
      out_port_index = 1;
    } else {
      GST_DEBUG_OBJECT (self, "Detected %u ports, starting at %u",
          (guint) param.nPorts, (guint) param.nStartPortNumber);
      in_port_index = param.nStartPortNumber + 0;
      out_port_index = param.nStartPortNumber + 1;
    }
  }
  self->dec_in_port = gst_omx_component_add_port (self->dec, in_port_index);
  self->dec_out_port = gst_omx_component_add_port (self->dec, out_port_index);

  if (!self->dec_in_port || !self->dec_out_port)
    return FALSE;

  GST_DEBUG_OBJECT (self, "Opened decoder");

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  GST_DEBUG_OBJECT (self, "Opening EGL renderer");
  self->egl_render =
      gst_omx_component_new (GST_OBJECT_CAST (self), klass->cdata.core_name,
      "OMX.broadcom.egl_render", NULL, klass->cdata.hacks);

  if (!self->egl_render)
    return FALSE;

  if (gst_omx_component_get_state (self->egl_render,
          GST_CLOCK_TIME_NONE) != OMX_StateLoaded)
    return FALSE;

  {
    OMX_PORT_PARAM_TYPE param;
    OMX_ERRORTYPE err;

    GST_OMX_INIT_STRUCT (&param);

    err =
        gst_omx_component_get_parameter (self->egl_render,
        OMX_IndexParamVideoInit, &param);
    if (err != OMX_ErrorNone) {
      GST_WARNING_OBJECT (self, "Couldn't get port information: %s (0x%08x)",
          gst_omx_error_to_string (err), err);
      /* Fallback */
      in_port_index = 0;
      out_port_index = 1;
    } else {
      GST_DEBUG_OBJECT (self, "Detected %u ports, starting at %u", param.nPorts,
          param.nStartPortNumber);
      in_port_index = param.nStartPortNumber + 0;
      out_port_index = param.nStartPortNumber + 1;
    }
  }

  self->egl_in_port =
      gst_omx_component_add_port (self->egl_render, in_port_index);
  self->egl_out_port =
      gst_omx_component_add_port (self->egl_render, out_port_index);

  if (!self->egl_in_port || !self->egl_out_port)
    return FALSE;

  GST_DEBUG_OBJECT (self, "Opened EGL renderer");
#endif

  return TRUE;
}

static gboolean
gst_omx_video_dec_shutdown (GstOMXVideoDec * self)
{
  OMX_STATETYPE state;

  GST_DEBUG_OBJECT (self, "Shutting down decoder");

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  state = gst_omx_component_get_state (self->egl_render, 0);
  if (state > OMX_StateLoaded || state == OMX_StateInvalid) {
    if (state > OMX_StateIdle) {
      gst_omx_component_set_state (self->egl_render, OMX_StateIdle);
      gst_omx_component_set_state (self->dec, OMX_StateIdle);
      gst_omx_component_get_state (self->egl_render, 5 * GST_SECOND);
      gst_omx_component_get_state (self->dec, 1 * GST_SECOND);
    }
    gst_omx_component_set_state (self->egl_render, OMX_StateLoaded);
    gst_omx_component_set_state (self->dec, OMX_StateLoaded);

    gst_omx_port_deallocate_buffers (self->dec_in_port);
    gst_omx_video_dec_deallocate_output_buffers (self);
    gst_omx_close_tunnel (self->dec_out_port, self->egl_in_port);
    if (state > OMX_StateLoaded) {
      gst_omx_component_get_state (self->egl_render, 5 * GST_SECOND);
      gst_omx_component_get_state (self->dec, 1 * GST_SECOND);
    }
  }

  /* Otherwise we didn't use EGL and just fall back to
   * shutting down the decoder */
#endif

  state = gst_omx_component_get_state (self->dec, 0);
  if (state > OMX_StateLoaded || state == OMX_StateInvalid) {
    if (state > OMX_StateIdle) {
      gst_omx_component_set_state (self->dec, OMX_StateIdle);
      gst_omx_component_get_state (self->dec, 5 * GST_SECOND);
    }
    gst_omx_component_set_state (self->dec, OMX_StateLoaded);
    gst_omx_port_deallocate_buffers (self->dec_in_port);
    {
      GstQuery *query = gst_query_new_drain ();
      GST_INFO_OBJECT (self, "Drain begin...");
      if (!gst_pad_peer_query (GST_VIDEO_DECODER_SRC_PAD (self), query))
        GST_ERROR_OBJECT (self, "drain query failed");
      gst_query_unref (query);

      GST_WARNING_OBJECT (self, "Drained");
    }
    gst_omx_video_dec_deallocate_output_buffers (self);
    if (state > OMX_StateLoaded)
      gst_omx_component_get_state (self->dec, 5 * GST_SECOND);
  }

  return TRUE;
}

static gboolean
gst_omx_video_dec_close (GstVideoDecoder * decoder)
{
  GstOMXVideoDec *self = GST_OMX_VIDEO_DEC (decoder);

  GST_DEBUG_OBJECT (self, "Closing decoder");

  if (!gst_omx_video_dec_shutdown (self))
    return FALSE;

  self->dec_in_port = NULL;
  self->dec_out_port = NULL;

  GST_WARNING_OBJECT(decoder, "%s:v-dec omx_free begin...", SMEPLAYER_STAT_TAG_TIME);
  if (self->dec)
    gst_omx_component_free (self->dec);
  self->dec = NULL;
  GST_WARNING_OBJECT(decoder, "%s:v-dec omx_free end", SMEPLAYER_STAT_TAG_TIME);

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  self->egl_in_port = NULL;
  self->egl_out_port = NULL;
  if (self->egl_render)
    gst_omx_component_free (self->egl_render);
  self->egl_render = NULL;
#endif

  self->started = FALSE;

  GST_DEBUG_OBJECT (self, "Closed decoder");

  return TRUE;
}

static void
gst_omx_video_dec_finalize (GObject * object)
{
  GstOMXVideoDec *self = GST_OMX_VIDEO_DEC (object);

  g_mutex_clear (&self->drain_lock);
  g_cond_clear (&self->drain_cond);
  g_mutex_clear(&self->cur_framerate_muxtex);

  G_OBJECT_CLASS (gst_omx_video_dec_parent_class)->finalize (object);
}

static GstStateChangeReturn
gst_omx_video_dec_change_state (GstElement * element, GstStateChange transition)
{
  GstOMXVideoDec *self;
  GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;
  guint flag_null_ready = 0;

  GST_INFO ("video decoder chage_state in:%s to %s,",
    gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
    gst_element_state_get_name((GstState)(transition & 0x07)));

  g_return_val_if_fail (GST_IS_OMX_VIDEO_DEC (element),
      GST_STATE_CHANGE_FAILURE);
  self = GST_OMX_VIDEO_DEC (element);

  switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
      flag_null_ready = 1;
      break;
    case GST_STATE_CHANGE_READY_TO_PAUSED:
      self->downstream_flow_ret = GST_FLOW_OK;
      self->draining = FALSE;
      self->started = FALSE;
      /* BEGIN: Added by liurongliang(l00180035), 2015/7/20 */
      self->input_count = 0;
      self->is_dec_out = FALSE;
      /* END:   Added by liurongliang(l00180035), 2015/7/20 */

#ifdef USE_OMX_DUMP_DATA
      self->dump_codec_data = fopen(DUMP_CODEC_PATH, "ab+");
      self->sk_frame_data = fopen(DUMP_SK_DATA_PATH, "ab+");
      self->total_frame_data = fopen(DUMP_TOTAL_DATA_PATH, "ab+");
#endif


      break;
    case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
      break;
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      if (self->dec_in_port)
        gst_omx_port_set_flushing (self->dec_in_port, 5 * GST_SECOND, TRUE);
      if (self->dec_out_port)
        gst_omx_port_set_flushing (self->dec_out_port, 5 * GST_SECOND, TRUE);
#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
      if (self->egl_in_port)
        gst_omx_port_set_flushing (self->egl_in_port, 5 * GST_SECOND, TRUE);
      if (self->egl_out_port)
        gst_omx_port_set_flushing (self->egl_out_port, 5 * GST_SECOND, TRUE);
#endif

      g_mutex_lock (&self->drain_lock);
      self->draining = FALSE;
      g_cond_broadcast (&self->drain_cond);
      g_mutex_unlock (&self->drain_lock);

      if(self->codec_data_bak != NULL){
        g_free(self->codec_data_bak);
        self->code_data_bak_len = 0;
      }
      if(self->dump_es_file) {
        fclose(self->dump_es_file);
        self->dump_es_file = NULL;
      }
      if(self->dump_raw_file) {
        fclose(self->dump_raw_file);
        self->dump_raw_file = NULL;
      }
      break;
    default:
      break;
  }

  if (ret == GST_STATE_CHANGE_FAILURE)
    return ret;
  if(flag_null_ready == 1){
    gettimeofday(&self->vdec_proc_info.vdec_create_time, NULL);

  }

  GST_INFO ("video decoder chage_state father in: %s to %s,",
    gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
    gst_element_state_get_name((GstState)(transition & 0x07)));

  ret =
      GST_ELEMENT_CLASS (gst_omx_video_dec_parent_class)->change_state
      (element, transition);

  GST_INFO ("video decoder chage_state father out: %s to %s,",
    gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
    gst_element_state_get_name((GstState)(transition & 0x07)));

  if (ret == GST_STATE_CHANGE_FAILURE) {
#ifdef USE_OMX_DUMP_DATA
    if(self->dump_codec_data) {
      fclose(self->dump_codec_data);
      self->dump_codec_data = NULL;
    }
    if(self->sk_frame_data) {
      fclose(self->sk_frame_data);
      self->sk_frame_data = NULL;
    }
    if(self->total_frame_data) {
      fclose(self->total_frame_data);
      self->total_frame_data = NULL;
    }
#endif
    return ret;
  }

  switch (transition) {
    case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
      break;
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      self->downstream_flow_ret = GST_FLOW_FLUSHING;
      self->started = FALSE;

      if (!gst_omx_video_dec_shutdown (self))
        ret = GST_STATE_CHANGE_FAILURE;

#ifdef USE_OMX_DUMP_DATA
      if(self->dump_codec_data) {
        fclose(self->dump_codec_data);
        self->dump_codec_data = NULL;
      }
      if(self->sk_frame_data) {
        fclose(self->sk_frame_data);
        self->sk_frame_data = NULL;
      }
      if(self->total_frame_data) {
        fclose(self->total_frame_data);
        self->total_frame_data = NULL;
      }
#endif

      break;
    case GST_STATE_CHANGE_READY_TO_NULL:
      break;
    default:
      break;
  }

  GST_INFO ("video decoder chage_state out:%s to %s,",
    gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
    gst_element_state_get_name((GstState)(transition & 0x07)));

  return ret;
}

static gboolean
gst_omx_video_dec_fill_buffer (GstOMXVideoDec * self,
    GstOMXBuffer * inbuf, GstBuffer * outbuf)
{
  GstVideoCodecState *state =
      gst_video_decoder_get_output_state (GST_VIDEO_DECODER (self));
  GstVideoInfo *vinfo = &state->info;
  OMX_PARAM_PORTDEFINITIONTYPE *port_def = &self->dec_out_port->port_def;
  gboolean ret = FALSE;
  GstVideoFrame frame;

  if (vinfo->width != port_def->format.video.nFrameWidth ||
      vinfo->height != port_def->format.video.nFrameHeight) {
    GST_ERROR_OBJECT (self, "Resolution do not match: port=%ux%u vinfo=%dx%d",
        (guint) port_def->format.video.nFrameWidth,
        (guint) port_def->format.video.nFrameHeight,
        vinfo->width, vinfo->height);
    goto done;
  }

  /* Same strides and everything */
  GST_DEBUG("buffer size :%d, omx len is :%ld", gst_buffer_get_size (outbuf),
      inbuf->omx_buf->nFilledLen);
  if (gst_buffer_get_size (outbuf) == inbuf->omx_buf->nFilledLen) {
    GstMapInfo map = GST_MAP_INFO_INIT;

    if (!gst_buffer_map (outbuf, &map, GST_MAP_WRITE)) {
      GST_ERROR_OBJECT (self, "Failed to map output buffer");
      goto done;
    }

    memcpy (map.data,
        inbuf->omx_buf->pBuffer + inbuf->omx_buf->nOffset,
        inbuf->omx_buf->nFilledLen);
    gst_buffer_unmap (outbuf, &map);
    ret = TRUE;
    goto done;
  }

  /* Different strides */
  if (gst_video_frame_map (&frame, vinfo, outbuf, GST_MAP_WRITE)) {
    const guint nstride = port_def->format.video.nStride;
    const guint nslice = port_def->format.video.nSliceHeight;
    guint src_stride[GST_VIDEO_MAX_PLANES] = { nstride, 0, };
    guint src_size[GST_VIDEO_MAX_PLANES] = { nstride * nslice, 0, };
    gint dst_width[GST_VIDEO_MAX_PLANES] = { 0, };
    gint dst_height[GST_VIDEO_MAX_PLANES] =
        { GST_VIDEO_INFO_HEIGHT (vinfo), 0, };
    const guint8 *src;
    guint p;

    switch (GST_VIDEO_INFO_FORMAT (vinfo)) {
      case GST_VIDEO_FORMAT_ABGR:
      case GST_VIDEO_FORMAT_ARGB:
        dst_width[0] = GST_VIDEO_INFO_WIDTH (vinfo) * 4;
        break;
      case GST_VIDEO_FORMAT_RGB16:
      case GST_VIDEO_FORMAT_BGR16:
      case GST_VIDEO_FORMAT_YUY2:
      case GST_VIDEO_FORMAT_UYVY:
      case GST_VIDEO_FORMAT_YVYU:
        dst_width[0] = GST_VIDEO_INFO_WIDTH (vinfo) * 2;
        break;
      case GST_VIDEO_FORMAT_GRAY8:
        dst_width[0] = GST_VIDEO_INFO_WIDTH (vinfo);
        break;
      case GST_VIDEO_FORMAT_I420:
        dst_width[0] = GST_VIDEO_INFO_WIDTH (vinfo);
        src_stride[1] = nstride / 2;
        src_size[1] = (src_stride[1] * nslice) / 2;
        dst_width[1] = GST_VIDEO_INFO_WIDTH (vinfo) / 2;
        dst_height[1] = GST_VIDEO_INFO_HEIGHT (vinfo) / 2;
        src_stride[2] = nstride / 2;
        src_size[2] = (src_stride[1] * nslice) / 2;
        dst_width[2] = GST_VIDEO_INFO_WIDTH (vinfo) / 2;
        dst_height[2] = GST_VIDEO_INFO_HEIGHT (vinfo) / 2;
        break;
      case GST_VIDEO_FORMAT_NV12:
        dst_width[0] = GST_VIDEO_INFO_WIDTH (vinfo);
        src_stride[1] = nstride;
        src_size[1] = src_stride[1] * nslice / 2;
        dst_width[1] = GST_VIDEO_INFO_WIDTH (vinfo);
        dst_height[1] = GST_VIDEO_INFO_HEIGHT (vinfo) / 2;
        break;
      case GST_VIDEO_FORMAT_NV16:
        dst_width[0] = GST_VIDEO_INFO_WIDTH (vinfo);
        src_stride[1] = nstride;
        src_size[1] = src_stride[1] * nslice;
        dst_width[1] = GST_VIDEO_INFO_WIDTH (vinfo);
        dst_height[1] = GST_VIDEO_INFO_HEIGHT (vinfo);
        break;
      default:
        g_assert_not_reached ();
        break;
    }

    src = inbuf->omx_buf->pBuffer + inbuf->omx_buf->nOffset;
    for (p = 0; p < GST_VIDEO_INFO_N_PLANES (vinfo); p++) {
      const guint8 *data;
      guint8 *dst;
      guint h;

      dst = GST_VIDEO_FRAME_PLANE_DATA (&frame, p);
      data = src;
      for (h = 0; h < dst_height[p]; h++) {
        memcpy (dst, data, dst_width[p]);
        dst += GST_VIDEO_INFO_PLANE_STRIDE (vinfo, p);
        data += src_stride[p];
      }
      src += src_size[p];
    }

    gst_video_frame_unmap (&frame);
    ret = TRUE;
  } else {
    GST_ERROR_OBJECT (self, "Can't map output buffer to frame");
    goto done;
  }

done:
  if (ret) {
    GST_BUFFER_PTS (outbuf) =
        gst_util_uint64_scale (inbuf->omx_buf->nTimeStamp, GST_SECOND,
        OMX_TICKS_PER_SECOND);
    if(self->vdec_proc_info.file_frame_rate != 0){
      GST_BUFFER_DURATION (outbuf) =
          (1000 / (self->vdec_proc_info.file_frame_rate / 1000)) * 1000 * 1000;
    }else{
      GST_BUFFER_DURATION (outbuf) =
          gst_util_uint64_scale (inbuf->omx_buf->nTickCount, GST_SECOND, OMX_TICKS_PER_SECOND);
    }

    GST_DEBUG("decoder output chain PTS %" GST_TIME_FORMAT ", DTS %" GST_TIME_FORMAT " duration %"
        GST_TIME_FORMAT " size %" G_GSIZE_FORMAT ,
        GST_TIME_ARGS (GST_BUFFER_PTS (outbuf)),
        GST_TIME_ARGS (GST_BUFFER_DTS (outbuf)),
        GST_TIME_ARGS (GST_BUFFER_DURATION (outbuf)), gst_buffer_get_size (outbuf));

  }

  gst_video_codec_state_unref (state);

  return ret;
}



static OMX_ERRORTYPE
gst_omx_video_dec_allocate_output_buffers (GstOMXVideoDec * self, GstAllocator* frame_allocator)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;
  GList * real_data = NULL;
  GstOMXPort *port;
  GstBufferPool *pool;
  GstStructure *config;
  gboolean eglimage = FALSE, add_videometa = FALSE;
  GstCaps *caps = NULL;
  guint min = 0, max = 0;
  GstVideoCodecState *state =
      gst_video_decoder_get_output_state (GST_VIDEO_DECODER (self));

  gboolean was_enabled = TRUE;


#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  port = self->eglimage ? self->egl_out_port : self->dec_out_port;
#else
  port = self->dec_out_port;
#endif

  pool = gst_video_decoder_get_buffer_pool (GST_VIDEO_DECODER (self));
  if (pool) {
    GstAllocator *allocator;

    config = gst_buffer_pool_get_config (pool);
    if (!gst_buffer_pool_config_get_params (config, &caps, NULL, &min, &max)) {
      GST_ERROR_OBJECT (self, "Can't get buffer pool params");
      gst_structure_free (config);
      err = OMX_ErrorUndefined;
      goto done;
    }
    if (!gst_buffer_pool_config_get_allocator (config, &allocator, NULL)) {
      GST_ERROR_OBJECT (self, "Can't get buffer pool allocator");
      gst_structure_free (config);
      err = OMX_ErrorUndefined;
      goto done;
    }

    /* Need at least 2 buffers for anything meaningful */
    min = MAX (MAX (min, port->port_def.nBufferCountMin), 4);
    if (max == 0) {
    /*
      for DTS2015082004245
      //we just use port->port_def.nBufferCountActual
      for
    */
#if 0
      if(g_queue_is_empty(&port->pending_buffers)){
        max = min;
      }else{
        max = port->port_def.nBufferCountActual;
        min = max;
      }
#else
      max = port->port_def.nBufferCountActual;
      min = max;
#endif
    } else if (max < port->port_def.nBufferCountMin || max < 2) {
      /* Can't use pool because can't have enough buffers */
      gst_caps_replace (&caps, NULL);
    } else {
      min = max;
    }

    add_videometa = gst_buffer_pool_config_has_option (config,
        GST_BUFFER_POOL_OPTION_VIDEO_META);
    GST_INFO("gst_omx_video_dec_allocate_output_buffers add_videometa=%d", add_videometa);
    gst_structure_free (config);

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
    eglimage = self->eglimage && (allocator
        && g_strcmp0 (allocator->mem_type, GST_EGL_IMAGE_MEMORY_TYPE) == 0);
#else
    /* TODO: Implement something that works for other targets too */
    eglimage = FALSE;
#endif
    caps = caps ? gst_caps_ref (caps) : NULL;

    GST_DEBUG_OBJECT (self, "Trying to use pool %p with caps %" GST_PTR_FORMAT
        " and memory type %s", pool, caps,
        (allocator ? allocator->mem_type : "(null)"));
  } else {
    gst_caps_replace (&caps, NULL);
    min = max = port->port_def.nBufferCountMin;
    GST_DEBUG_OBJECT (self, "No pool available, not negotiated yet");
  }

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  /* Will retry without EGLImage */
  if (self->eglimage && !eglimage) {
    GST_DEBUG_OBJECT (self,
        "Wanted to use EGLImage but downstream doesn't support it");
    err = OMX_ErrorUndefined;
    goto done;
  }
#endif

  if (caps)
    self->out_port_pool =
          gst_omx_video_buffer_pool_hivo_new (GST_ELEMENT_CAST (self), self->dec, port, frame_allocator);
          //gst_omx_buffer_pool_new (GST_ELEMENT_CAST (self), self->dec, port);
  else{
    GST_WARNING_OBJECT (self, "caps is null");

  }
#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  if (eglimage) {
    GList *buffers = NULL;
    GList *images = NULL;
    gint i;
    GstBufferPoolAcquireParams params = { 0, };
    EGLDisplay egl_display = EGL_NO_DISPLAY;

    GST_DEBUG_OBJECT (self, "Trying to allocate %d EGLImages", min);

    for (i = 0; i < min; i++) {
      GstBuffer *buffer;
      GstMemory *mem;

      if (gst_buffer_pool_acquire_buffer (pool, &buffer, &params) != GST_FLOW_OK
          || gst_buffer_n_memory (buffer) != 1
          || !(mem = gst_buffer_peek_memory (buffer, 0))
          || g_strcmp0 (mem->allocator->mem_type,
              GST_EGL_IMAGE_MEMORY_TYPE) != 0) {
        GST_INFO_OBJECT (self, "Failed to allocated %d-th EGLImage", i);
        g_list_free_full (buffers, (GDestroyNotify) gst_buffer_unref);
        g_list_free (images);
        buffers = NULL;
        images = NULL;
        /* TODO: For non-RPi targets we want to use the normal memory code below */
        /* Retry without EGLImage */
        err = OMX_ErrorUndefined;
        goto done;
      }

      buffers = g_list_append (buffers, buffer);
      gst_egl_image_memory_set_orientation (mem,
          GST_VIDEO_GL_TEXTURE_ORIENTATION_X_NORMAL_Y_FLIP);
      images = g_list_append (images, gst_egl_image_memory_get_image (mem));
      if (egl_display == EGL_NO_DISPLAY)
        egl_display = gst_egl_image_memory_get_display (mem);
    }

    GST_DEBUG_OBJECT (self, "Allocated %d EGLImages successfully", min);

    /* Everything went fine? */
    if (eglimage) {
      GST_DEBUG_OBJECT (self, "Setting EGLDisplay");
      self->egl_out_port->port_def.format.video.pNativeWindow = egl_display;
      err =
          gst_omx_port_update_port_definition (self->egl_out_port,
          &self->egl_out_port->port_def);
      if (err != OMX_ErrorNone) {
        GST_INFO_OBJECT (self,
            "Failed to set EGLDisplay on port: %s (0x%08x)",
            gst_omx_error_to_string (err), err);
        g_list_free_full (buffers, (GDestroyNotify) gst_buffer_unref);
        g_list_free (images);
        /* TODO: For non-RPi targets we want to use the normal memory code below */
        /* Retry without EGLImage */
        goto done;
      } else {
        GList *l;

        if (min != port->port_def.nBufferCountActual) {
          err = gst_omx_port_update_port_definition (port, NULL);
          if (err == OMX_ErrorNone) {
            port->port_def.nBufferCountActual = min;
            err = gst_omx_port_update_port_definition (port, &port->port_def);
          }

          if (err != OMX_ErrorNone) {
            GST_INFO_OBJECT (self,
                "Failed to configure %u output buffers: %s (0x%08x)", min,
                gst_omx_error_to_string (err), err);
            g_list_free_full (buffers, (GDestroyNotify) gst_buffer_unref);
            g_list_free (images);
            /* TODO: For non-RPi targets we want to use the normal memory code below */
            /* Retry without EGLImage */

            goto done;
          }
        }

        if (!gst_omx_port_is_enabled (port)) {
          err = gst_omx_port_set_enabled (port, TRUE);
          if (err != OMX_ErrorNone) {
            GST_INFO_OBJECT (self,
                "Failed to enable port: %s (0x%08x)",
                gst_omx_error_to_string (err), err);
            g_list_free_full (buffers, (GDestroyNotify) gst_buffer_unref);
            g_list_free (images);
            /* TODO: For non-RPi targets we want to use the normal memory code below */
            /* Retry without EGLImage */
            goto done;
          }
        }

        err = gst_omx_port_use_eglimages (port, images);
        g_list_free (images);

        if (err != OMX_ErrorNone) {
          GST_INFO_OBJECT (self,
              "Failed to pass EGLImages to port: %s (0x%08x)",
              gst_omx_error_to_string (err), err);
          g_list_free_full (buffers, (GDestroyNotify) gst_buffer_unref);
          /* TODO: For non-RPi targets we want to use the normal memory code below */
          /* Retry without EGLImage */
          goto done;
        }

        err = gst_omx_port_wait_enabled (port, 2 * GST_SECOND);
        if (err != OMX_ErrorNone) {
          GST_INFO_OBJECT (self,
              "Failed to wait until port is enabled: %s (0x%08x)",
              gst_omx_error_to_string (err), err);
          g_list_free_full (buffers, (GDestroyNotify) gst_buffer_unref);
          /* TODO: For non-RPi targets we want to use the normal memory code below */
          /* Retry without EGLImage */
          goto done;
        }

        GST_DEBUG_OBJECT (self, "Populating internal buffer pool");
        GST_OMX_VIDEO_BUFFER_POOL_HIVO (self->out_port_pool)->other_pool =
            GST_BUFFER_POOL (gst_object_ref (pool));
        for (l = buffers; l; l = l->next) {
          g_ptr_array_add (GST_OMX_VIDEO_BUFFER_POOL_HIVO (self->out_port_pool)->buffers,
              l->data);
        }
        g_list_free (buffers);
        /* All good and done, set caps below */
      }
    }
  }
#endif

  err = OMX_ErrorNone;
  GST_INFO("caps = %p, add_videometa=%d,min:max[%d:%d],minbuf:actualbuf[%ld:%ld]",
    caps, add_videometa, min, max,
    port->port_def.nBufferCountMin, port->port_def.nBufferCountActual);
  if (caps) {
    config = gst_buffer_pool_get_config (self->out_port_pool);

    if (add_videometa)
      gst_buffer_pool_config_add_option (config,
          GST_BUFFER_POOL_OPTION_VIDEO_META);

    gst_buffer_pool_config_set_params (config, caps,
        self->dec_out_port->port_def.nBufferSize, min, max);

    if (!gst_buffer_pool_set_config (self->out_port_pool, config)) {
      GST_INFO_OBJECT (self, "Failed to set config on internal pool");
      gst_object_unref (self->out_port_pool);
      self->out_port_pool = NULL;
      goto done;
    }

    GST_OMX_VIDEO_BUFFER_POOL_HIVO (self->out_port_pool)->allocating = TRUE;
    /* This now allocates all the buffers */
    if (!gst_buffer_pool_set_active (self->out_port_pool, TRUE)) {
      GST_INFO_OBJECT (self, "Failed to activate internal pool");
      gst_object_unref (self->out_port_pool);
      self->out_port_pool = NULL;
      err = OMX_ErrorInsufficientResources;
    } else {
      GST_OMX_VIDEO_BUFFER_POOL_HIVO (self->out_port_pool)->allocating = FALSE;

      if (!gst_omx_port_is_enabled (self->dec_out_port)) {
        err = gst_omx_port_set_enabled (self->dec_out_port, TRUE);
        if (err != OMX_ErrorNone) {
          GST_WARNING_OBJECT (self,
              "Failed to enable port: %s (0x%08x)",
              gst_omx_error_to_string (err), err);
          goto done;
        }
        was_enabled = FALSE;
      }

      /* updata outport definition */
      err = gst_omx_port_get_port_definition (port, &port->port_def);
      if (err == OMX_ErrorNone) {
        /* stride need 16 byte alignment */
        port->port_def.format.video.nStride = ((port->port_def.format.video.nFrameWidth + 15) & ~15);
        err = gst_omx_port_update_port_definition (port, &port->port_def);
        if (err != OMX_ErrorNone) {
          GST_ERROR_OBJECT (self,
              "Failed to configure %u output buffers: %s (0x%08x)", min,
              gst_omx_error_to_string (err), err);
          goto done;
        }
        GST_INFO_OBJECT (self,
          "Setting output outputdefinition:  (%d), width %u, height %u, stride: %d",
          port->port_def.format.video.eColorFormat,
          (guint) port->port_def.format.video.nFrameWidth,
          (guint) port->port_def.format.video.nFrameHeight,
          (guint) port->port_def.format.video.nStride);

      }else{
          GST_WARNING("Get outport definition failed");
      }

      /* we put buffers allocated by ourselves into OMX-IL */
      real_data = gst_omx_video_buffer_pool_hivo_get_real_data(self->out_port_pool);

      if(real_data){
        OMX_ERRORTYPE err_tmp;
        err_tmp = gst_omx_port_use_buffers(self->dec_out_port, real_data->next);
        if(err_tmp != OMX_ErrorNone){
          GST_WARNING("USE BUFFER ERROR");
        }
      }else{
        GST_WARNING("GET REAL_DATA FAILED");
      }

      if (!was_enabled) {
        err = gst_omx_port_wait_enabled (port, 2 * GST_SECOND);
        if (err != OMX_ErrorNone) {
          GST_ERROR_OBJECT (self,
              "Failed to wait until port is enabled: %s (0x%08x)",
              gst_omx_error_to_string (err), err);
          goto done;
        }
      }
      GST_INFO("allocating = %d",  GST_OMX_VIDEO_BUFFER_POOL_HIVO (self->out_port_pool)->allocating);
    }
  } else if (self->out_port_pool) {
    gst_object_unref (self->out_port_pool);
    self->out_port_pool = NULL;
  }

done:
  if (!self->out_port_pool && err == OMX_ErrorNone)
    GST_DEBUG_OBJECT (self,
        "Not using our internal pool and copying buffers for downstream");

  if (caps)
    gst_caps_unref (caps);
  if (pool)
    gst_object_unref (pool);
  if (state)
    gst_video_codec_state_unref (state);

  return err;
}

static OMX_ERRORTYPE
gst_omx_video_dec_deallocate_output_buffers (GstOMXVideoDec * self)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;

  if (self->out_port_pool) {
    err = gst_omx_port_deallocate_buffers (self->dec_out_port);
    gst_buffer_pool_set_active (self->out_port_pool, FALSE);
#if 0
    gst_buffer_pool_wait_released (self->out_port_pool);
#endif
    GST_OMX_VIDEO_BUFFER_POOL_HIVO (self->out_port_pool)->deactivated = TRUE;
    gst_object_unref (self->out_port_pool);
    self->out_port_pool = NULL;

  }
#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  err =
      gst_omx_port_deallocate_buffers (self->
      eglimage ? self->egl_out_port : self->dec_out_port);
#else

#endif


  return err;
}

static OMX_ERRORTYPE
gst_omx_video_dec_reconfigure_output_port (GstOMXVideoDec * self)
{
  GstOMXPort *port;
  OMX_ERRORTYPE err;
  GstVideoCodecState *state;
  OMX_PARAM_PORTDEFINITIONTYPE port_def;
  GstVideoFormat format;
  GstAllocator * allocator_hivo = NULL;

  /* At this point the decoder output port is disabled */

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  {
    OMX_STATETYPE egl_state;

    if (self->eglimage) {
      /* Nothing to do here, we could however fall back to non-EGLImage in theory */
      port = self->egl_out_port;
      err = OMX_ErrorNone;
      goto enable_port;
    } else {
      /* Set up egl_render */

      self->eglimage = TRUE;

      gst_omx_port_get_port_definition (self->dec_out_port, &port_def);
      GST_VIDEO_DECODER_STREAM_LOCK (self);
      state = gst_video_decoder_set_output_state (GST_VIDEO_DECODER (self),
          GST_VIDEO_FORMAT_RGBA, port_def.format.video.nFrameWidth,
          port_def.format.video.nFrameHeight, self->input_state);

      /* at this point state->caps is NULL */
      if (state->caps)
        gst_caps_unref (state->caps);
      state->caps = gst_video_info_to_caps (&state->info);
      gst_caps_set_features (state->caps, 0,
          gst_caps_features_new (GST_CAPS_FEATURE_MEMORY_EGL_IMAGE, NULL));

      /* try to negotiate with caps feature */
      if (!gst_video_decoder_negotiate (GST_VIDEO_DECODER (self))) {

        GST_DEBUG_OBJECT (self,
            "Failed to negotiate with feature %s",
            GST_CAPS_FEATURE_MEMORY_EGL_IMAGE);

        if (state->caps)
          gst_caps_replace (&state->caps, NULL);

        /* fallback: try to use EGLImage even if it is not in the caps feature */
        if (!gst_video_decoder_negotiate (GST_VIDEO_DECODER (self))) {
          gst_video_codec_state_unref (state);
          GST_ERROR_OBJECT (self, "Failed to negotiate RGBA for EGLImage");
          GST_VIDEO_DECODER_STREAM_UNLOCK (self);
          goto no_egl;
        }
      }

      gst_video_codec_state_unref (state);
      GST_VIDEO_DECODER_STREAM_UNLOCK (self);

      /* Now link it all together */

      err = gst_omx_port_set_enabled (self->egl_in_port, FALSE);
      if (err != OMX_ErrorNone)
        goto no_egl;

      err = gst_omx_port_wait_enabled (self->egl_in_port, 1 * GST_SECOND);
      if (err != OMX_ErrorNone)
        goto no_egl;

      err = gst_omx_port_set_enabled (self->egl_out_port, FALSE);
      if (err != OMX_ErrorNone)
        goto no_egl;

      err = gst_omx_port_wait_enabled (self->egl_out_port, 1 * GST_SECOND);
      if (err != OMX_ErrorNone)
        goto no_egl;

      {
#define OMX_IndexParamBrcmVideoEGLRenderDiscardMode 0x7f0000db
        OMX_CONFIG_PORTBOOLEANTYPE discardMode;
        memset (&discardMode, 0, sizeof (discardMode));
        discardMode.nSize = sizeof (discardMode);
        discardMode.nPortIndex = 220;
        discardMode.nVersion.nVersion = OMX_VERSION;
        discardMode.bEnabled = OMX_FALSE;
        if (gst_omx_component_set_parameter (self->egl_render,
                OMX_IndexParamBrcmVideoEGLRenderDiscardMode,
                &discardMode) != OMX_ErrorNone)
          goto no_egl;
#undef OMX_IndexParamBrcmVideoEGLRenderDiscardMode
      }

      err = gst_omx_setup_tunnel (self->dec_out_port, self->egl_in_port);
      if (err != OMX_ErrorNone)
        goto no_egl;

      err = gst_omx_port_set_enabled (self->egl_in_port, TRUE);
      if (err != OMX_ErrorNone)
        goto no_egl;

      err = gst_omx_component_set_state (self->egl_render, OMX_StateIdle);
      if (err != OMX_ErrorNone)
        goto no_egl;

      err = gst_omx_port_wait_enabled (self->egl_in_port, 1 * GST_SECOND);
      if (err != OMX_ErrorNone)
        goto no_egl;

      if (gst_omx_component_get_state (self->egl_render,
              GST_CLOCK_TIME_NONE) != OMX_StateIdle)
        goto no_egl;

      err = gst_omx_video_dec_allocate_output_buffers (self);
      if (err != OMX_ErrorNone)
        goto no_egl;

      if (gst_omx_component_set_state (self->egl_render,
              OMX_StateExecuting) != OMX_ErrorNone)
        goto no_egl;

      if (gst_omx_component_get_state (self->egl_render,
              GST_CLOCK_TIME_NONE) != OMX_StateExecuting)
        goto no_egl;

      err =
          gst_omx_port_set_flushing (self->dec_out_port, 5 * GST_SECOND, FALSE);
      if (err != OMX_ErrorNone)
        goto no_egl;

      err =
          gst_omx_port_set_flushing (self->egl_in_port, 5 * GST_SECOND, FALSE);
      if (err != OMX_ErrorNone)
        goto no_egl;

      err =
          gst_omx_port_set_flushing (self->egl_out_port, 5 * GST_SECOND, FALSE);
      if (err != OMX_ErrorNone)
        goto no_egl;

      err = gst_omx_port_populate (self->egl_out_port);
      if (err != OMX_ErrorNone)
        goto no_egl;

      err = gst_omx_port_set_enabled (self->dec_out_port, TRUE);
      if (err != OMX_ErrorNone)
        goto no_egl;

      err = gst_omx_port_wait_enabled (self->dec_out_port, 1 * GST_SECOND);
      if (err != OMX_ErrorNone)
        goto no_egl;


      err = gst_omx_port_mark_reconfigured (self->dec_out_port);
      if (err != OMX_ErrorNone)
        goto no_egl;

      err = gst_omx_port_mark_reconfigured (self->egl_out_port);
      if (err != OMX_ErrorNone)
        goto no_egl;

      goto done;
    }

  no_egl:

    gst_omx_port_set_enabled (self->dec_out_port, FALSE);
    gst_omx_port_wait_enabled (self->dec_out_port, 1 * GST_SECOND);
    egl_state = gst_omx_component_get_state (self->egl_render, 0);
    if (egl_state > OMX_StateLoaded || egl_state == OMX_StateInvalid) {
      if (egl_state > OMX_StateIdle) {
        gst_omx_component_set_state (self->egl_render, OMX_StateIdle);
        gst_omx_component_get_state (self->egl_render, 5 * GST_SECOND);
      }
      gst_omx_component_set_state (self->egl_render, OMX_StateLoaded);

      gst_omx_video_dec_deallocate_output_buffers (self);
      gst_omx_close_tunnel (self->dec_out_port, self->egl_in_port);

      if (egl_state > OMX_StateLoaded) {
        gst_omx_component_get_state (self->egl_render, 5 * GST_SECOND);
      }
    }

    /* After this egl_render should be deactivated
     * and the decoder's output port disabled */
    self->eglimage = FALSE;
  }
#endif
  port = self->dec_out_port;

  /* Update caps */
  GST_VIDEO_DECODER_STREAM_LOCK (self);

  gst_omx_port_get_port_definition (port, &port_def);
  g_assert (port_def.format.video.eCompressionFormat == OMX_VIDEO_CodingUnused);

  format =
      gst_omx_video_get_format_from_omx (port_def.format.video.eColorFormat);

  if (format == GST_VIDEO_FORMAT_UNKNOWN) {
    GST_ERROR_OBJECT (self, "Unsupported color format: %d",
        port_def.format.video.eColorFormat);
    GST_VIDEO_DECODER_STREAM_UNLOCK (self);
    err = OMX_ErrorUndefined;
    goto done;
  }

  GST_INFO_OBJECT (self,
      "Setting output state: format %s (%d), width %u, height %u",
      gst_video_format_to_string (format),
      port_def.format.video.eColorFormat,
      (guint) port_def.format.video.nFrameWidth,
      (guint) port_def.format.video.nFrameHeight);

  state = gst_video_decoder_set_output_state (GST_VIDEO_DECODER (self),
      format, port_def.format.video.nFrameWidth,
      port_def.format.video.nFrameHeight, self->input_state);

  if (!gst_video_decoder_negotiate (GST_VIDEO_DECODER (self))) {
    gst_video_codec_state_unref (state);
    GST_ERROR_OBJECT (self, "Failed to negotiate");
    err = OMX_ErrorUndefined;
    GST_VIDEO_DECODER_STREAM_UNLOCK (self);
    goto done;
  }
  gst_video_codec_state_unref (state);

  GST_VIDEO_DECODER_STREAM_UNLOCK (self);

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
enable_port:
#endif

  gst_video_decoder_get_allocator(GST_VIDEO_DECODER (self), &allocator_hivo, NULL);
  err = gst_omx_video_dec_allocate_output_buffers(self, allocator_hivo);
  if (err != OMX_ErrorNone){
    GST_WARNING("REALLOCATE OUTPUT BUFFER ERROR");
    goto done;
  }

  err = gst_omx_port_populate (port);
  if (err != OMX_ErrorNone)
    goto done;

  err = gst_omx_port_mark_reconfigured (port);
  if (err != OMX_ErrorNone)
    goto done;

done:

  if (allocator_hivo)
      gst_object_unref(allocator_hivo);
  return err;
}


//static void
//gst_omx_video_dec_clean_older_frames (GstOMXVideoDec * self,
//    GstOMXBuffer * buf, GList * frames)
//{
//  GList *l;
//  GstClockTime timestamp;
//
//  timestamp = gst_util_uint64_scale (buf->omx_buf->nTimeStamp, GST_SECOND,
//      OMX_TICKS_PER_SECOND);
//
//  if (GST_CLOCK_TIME_IS_VALID (timestamp)) {
//    /* We could release all frames stored with pts < timestamp since the
//     * decoder will likely output frames in display order */
//    for (l = frames; l; l = l->next) {
//      GstVideoCodecFrame *tmp = l->data;
//
//      if (tmp->pts < timestamp) {
//        gst_video_decoder_release_frame (GST_VIDEO_DECODER (self), tmp);
//        GST_LOG_OBJECT (self,
//            "discarding ghost frame %p (#%d) PTS:%" GST_TIME_FORMAT " DTS:%"
//            GST_TIME_FORMAT, tmp, tmp->system_frame_number,
//            GST_TIME_ARGS (tmp->pts), GST_TIME_ARGS (tmp->dts));
//      } else {
//        gst_video_codec_frame_unref (tmp);
//      }
//    }
//  } else {
//    /* We will release all frames with invalid timestamp because we don't even
//     * know if they will be output some day. */
//    for (l = frames; l; l = l->next) {
//      GstVideoCodecFrame *tmp = l->data;
//
//      if (!GST_CLOCK_TIME_IS_VALID (tmp->pts)) {
//        gst_video_decoder_release_frame (GST_VIDEO_DECODER (self), tmp);
//        GST_LOG_OBJECT (self,
//            "discarding frame %p (#%d) with invalid PTS:%" GST_TIME_FORMAT
//            " DTS:%" GST_TIME_FORMAT, tmp, tmp->system_frame_number,
//            GST_TIME_ARGS (tmp->pts), GST_TIME_ARGS (tmp->dts));
//      } else {
//        gst_video_codec_frame_unref (tmp);
//      }
//    }
//  }
//
//  g_list_free (frames);
//}


static GstBuffer *
copy_frame (const GstVideoInfo * info, GstBuffer * outbuf)
{
  GstVideoInfo out_info, tmp_info;
  GstBuffer *tmpbuf;
  GstVideoFrame out_frame, tmp_frame;

  out_info = *info;
  tmp_info = *info;

  tmpbuf = gst_buffer_new_and_alloc (out_info.size);

  gst_video_frame_map (&out_frame, &out_info, outbuf, GST_MAP_READ);
  gst_video_frame_map (&tmp_frame, &tmp_info, tmpbuf, GST_MAP_WRITE);
  gst_video_frame_copy (&tmp_frame, &out_frame);
  gst_video_frame_unmap (&out_frame);
  gst_video_frame_unmap (&tmp_frame);

  gst_buffer_unref (outbuf);

  return tmpbuf;
}


static GstVideoCodecFrame *
gst_omx_video_dec_new_frame ()
{
  GstVideoCodecFrame *frame;

  GST_DEBUG("new_frame in");

  frame = g_slice_new0 (GstVideoCodecFrame);

  frame->ref_count = 1;

  frame->system_frame_number = 0;
  frame->decode_frame_number = 0;

  frame->dts = GST_CLOCK_TIME_NONE;
  frame->pts = GST_CLOCK_TIME_NONE;
  frame->duration = GST_CLOCK_TIME_NONE;
  frame->events = NULL;

  GST_DEBUG("new_frame out");
  return frame;
}

static void gst_omx_video_store_codec(GstOMXVideoDec *self, void* data, glong data_len)
{
  self->codec_data_bak = g_malloc(sizeof(guint) * data_len);
  memcpy (self->codec_data_bak, (guint8 *)(data), data_len);
  self->code_data_bak_len = data_len;
  return;
}

static void gst_omx_video_dump_raw(GstOMXVideoDec *self,  GstOMXBuffer *buf)
{
  if(self->raw_dump_switch){
    if(self->dump_raw_file == NULL){
      char dump_file_name[512];
      g_snprintf(dump_file_name, 512, "%s/%s_out_%u.raw", self->dump_raw_path,
        GST_ELEMENT_NAME(self), self->dump_raw_index++);
      self->dump_raw_file = fopen(dump_file_name, "ab+");
      if(self->dump_raw_file == NULL){
        GST_WARNING("dump  failed,open %s failed", dump_file_name);
        return;
      }

    }

    if(self->dump_raw_file != NULL){
      if(fwrite(buf->omx_buf->pBuffer + buf->omx_buf->nOffset, 1,
        buf->omx_buf->nFilledLen, self->dump_raw_file) != buf->omx_buf->nFilledLen){
        GST_WARNING("write data failed");
      }
    }

  }else{
    if(self->dump_raw_file) {
      fclose(self->dump_raw_file);
      self->dump_raw_file = NULL;
    }
  }
  return;
}
static void
gst_omx_video_dec_loop (GstOMXVideoDec * self)
{
  GstOMXPort *port;
  GstOMXBuffer *buf = NULL;
  GstVideoCodecFrame *frame;
  GstFlowReturn flow_ret = GST_FLOW_OK;
  GstOMXAcquireBufferReturn acq_return;
  GstClockTimeDiff deadline;
  OMX_ERRORTYPE err;
  GstVideoMeta *video_meta = NULL;
  OMX_PARAM_PORTDEFINITIONTYPE outport_reset;
  GstVideoCodecFrame *out_frame;

  static long dec_last_time = 0;
  static long dec_cur_time = 0;
  static long cur_frame_rate = 0;
  GstClockTime dec_out_diff = 0;
  static gboolean  flag_fir_out = TRUE;
  OMX_PARAM_PORTDEFINITIONTYPE port_getWH;


#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  port = self->eglimage ? self->egl_out_port : self->dec_out_port;
#else
  port = self->dec_out_port;
#endif

  acq_return = gst_omx_port_acquire_buffer (port, &buf);
  //GST_ERROR("333, acquire buffer is %p", buf);
  if (acq_return == GST_OMX_ACQUIRE_BUFFER_ERROR) {
    goto component_error;
  } else if (acq_return == GST_OMX_ACQUIRE_BUFFER_FLUSHING) {
    goto flushing;
  } else if (acq_return == GST_OMX_ACQUIRE_BUFFER_EOS) {
    goto eos;
  }

  /* Now we have caps */
  if (acq_return == GST_OMX_ACQUIRE_BUFFER_RECONFIGURE ) {

    GST_DEBUG_OBJECT (self, "Port settings have changed, updating caps");

    /* Reallocate all buffers */
    if ( gst_omx_port_is_enabled (port)) {
      err = gst_omx_port_set_enabled (port, FALSE);
      if (err != OMX_ErrorNone)
        goto reconfigure_error;
      {
        GstQuery *query = gst_query_new_drain ();
        GST_INFO_OBJECT (self, "Drain begin...");
        if (!gst_pad_peer_query (GST_VIDEO_DECODER_SRC_PAD (self), query))
          GST_ERROR_OBJECT (self, "drain query failed");
        gst_query_unref (query);

        GST_WARNING_OBJECT (self, "Drained");
      }
      err = gst_omx_port_wait_buffers_released (port, 5 * GST_SECOND);
      if (err != OMX_ErrorNone)
        goto reconfigure_error;

      err = gst_omx_video_dec_deallocate_output_buffers (self);
      if (err != OMX_ErrorNone)
        goto reconfigure_error;

      err = gst_omx_port_wait_enabled (port, 1 * GST_SECOND);
      if (err != OMX_ErrorNone)
        goto reconfigure_error;

    }

    err = gst_omx_video_dec_reconfigure_output_port (self);
    if (err != OMX_ErrorNone){
      if (gst_omx_port_is_flushing(port)){
        gst_omx_port_mark_reconfigured(port);
        GST_WARNING("Flushing");
      }
      GST_WARNING("vdec reconfig error");
      goto reconfigure_error;
    }
    gst_omx_port_get_port_definition (self->dec_out_port, &outport_reset);
    GST_INFO("resolution has been changed : %ld * %ld", outport_reset.format.video.nFrameWidth,
    outport_reset.format.video.nFrameHeight);
    /* Now get a buffer */
    return;
  }



  if(buf->omx_buf->nFilledLen <= 0){
    gst_omx_port_release_buffer (port, buf);
    GST_INFO("buf:%p not filled",buf);

    return;
  }
    /* video decoder output diff time */
  if(flag_fir_out){
     GstClockTime outtime;
     gettimeofday(&self->vdec_proc_info.get_first_I_frame, NULL);
     outtime = gst_omx_video_dec_get_cur_systime();
     self->vdec_proc_info.fir_Iframe_vdecT =
        outtime - self->vdec_proc_info.fir_Iframe_vdecT;
     GST_DEBUG(" vdec out time: %lld, vdec diff time: %lld ", outtime, self->vdec_proc_info.fir_Iframe_vdecT);
     flag_fir_out = FALSE;
  }
    //dec_last_time = dec_cur_time;
{
  dec_cur_time = gst_clock_get_time (GST_ELEMENT_CLOCK (self));
  dec_out_diff = (dec_cur_time - dec_last_time) / 1000000;
  cur_frame_rate++;
  if(dec_out_diff > 1000){
    if(self->vdec_proc_info.cur_frame_rate != cur_frame_rate){
      self->vdec_proc_info.cur_frame_rate = cur_frame_rate;
    }
    GST_DEBUG("vdec current frame rate %ld",self->vdec_proc_info.cur_frame_rate);
    cur_frame_rate = 0;
    dec_last_time = dec_cur_time;
  }
}

{
  OMX_HISI_PARAM_CHANNELATTRIBUTES para_chan;
  OMX_ERRORTYPE err_tmp;
  err_tmp = gst_omx_component_get_parameter(self->dec_out_port->comp, OMX_HisiIndexChannelAttributes, &para_chan);
  if(err_tmp == OMX_ErrorNone){
    if(self->vdec_proc_info.file_frame_rate != para_chan.xFramerate){
        self->vdec_proc_info.file_frame_rate = para_chan.xFramerate;
    }
    GST_DEBUG("frame rate is %ld", self->vdec_proc_info.file_frame_rate);
  }else{
    GST_DEBUG("get frame rate failed:%d", err_tmp);
  }
}
  g_assert (acq_return == GST_OMX_ACQUIRE_BUFFER_OK);
  self->is_dec_out = TRUE;
  /* This prevents a deadlock between the srcpad stream
   * lock and the videocodec stream lock, if ::reset()
   * is called at the wrong time
   */
  if (gst_omx_port_is_flushing (port)) {
    GST_INFO_OBJECT (self, "Flushing");
    gst_omx_port_mark_reconfigured(port);
    gst_omx_port_release_buffer (port, buf);
    goto flushing;
  }

  gst_omx_port_get_port_definition (port, &port_getWH);
  GST_DEBUG_OBJECT (self,
    "Setting output state:  Mywidth %u, Myheight %u",
    (guint) port_getWH.format.video.nFrameWidth,
    (guint) port_getWH.format.video.nFrameHeight);
  if(!self->first_frame) {
      GstMessage *mes_reconfigure;
      mes_reconfigure = gst_message_new_resolution_changed((GstObject * )self, port_getWH.format.video.nFrameWidth, port_getWH.format.video.nFrameHeight);
      gst_element_post_message ((GstElement *)self, mes_reconfigure);
      self->last_width =  port_getWH.format.video.nFrameWidth;
      self->last_height = port_getWH.format.video.nFrameHeight;
      self->first_frame = 1;
  }else{
      if(!(port_getWH.format.video.nFrameWidth == self->last_width &&
        port_getWH.format.video.nFrameHeight == self->last_height)) {
          GstMessage *mes_reconfigure;
          mes_reconfigure = gst_message_new_resolution_changed((GstObject *)self, port_getWH.format.video.nFrameWidth, port_getWH.format.video.nFrameHeight);
          gst_element_post_message ((GstElement *)self, mes_reconfigure);
          self->last_width =  port_getWH.format.video.nFrameWidth;
          self->last_height = port_getWH.format.video.nFrameHeight;
        }
    }

  GST_DEBUG_OBJECT (self, "Handling buffer video: 0x%08x %" G_GUINT64_FORMAT,
      (guint) buf->omx_buf->nFlags, (guint64) buf->omx_buf->nTimeStamp);

  GST_VIDEO_DECODER_STREAM_LOCK (self);

  /* dump raw data*/
  gst_omx_video_dump_raw(self, buf);


  /* So we have a timestamped OMX buffer and get, or not, corresponding frame.
   * Assuming decoder output frames in display order, frames preceding this
   * frame could be discarded as they seems useless due to e.g interlaced
   * stream, corrupted input data...
   * In any cases, not likely to be seen again. so drop it before they pile up
   * and use all the memory. */

  frame = NULL;

  if (frame
      && (deadline = gst_video_decoder_get_max_decode_time
          (GST_VIDEO_DECODER (self), frame)) < 0) {
    GST_WARNING_OBJECT (self,
        "Frame is too late, dropping (deadline %" GST_TIME_FORMAT ")",
        GST_TIME_ARGS (-deadline));
    flow_ret = gst_video_decoder_drop_frame (GST_VIDEO_DECODER (self), frame);
    frame = NULL;
  } else if (!frame && (buf->omx_buf->nFilledLen > 0 || buf->eglimage)) {
    GstBuffer *outbuf = NULL;

  /* This sometimes happens at EOS or if the input is not properly framed,
     * let's handle it gracefully by allocating a new buffer for the current
     * caps and filling it
     */

    if (self->out_port_pool) {
      gint i, n;
      GstBufferPoolAcquireParams params = { 0, };

      n = port->buffers->len;
      for (i = 0; i < n; i++) {
        GstOMXBuffer *tmp = g_ptr_array_index (port->buffers, i);

        if (tmp == buf)
          break;
      }
      g_assert (i != n);

      GST_OMX_VIDEO_BUFFER_POOL_HIVO (self->out_port_pool)->cur_frame_index = i;
      flow_ret =
          gst_buffer_pool_acquire_buffer (self->out_port_pool, &outbuf,
          &params);
      if (flow_ret != GST_FLOW_OK) {
        gst_omx_port_release_buffer (port, buf);
        goto invalid_buffer;
      }

      //buf = NULL;
    } else {
      outbuf =
          gst_video_decoder_allocate_output_buffer (GST_VIDEO_DECODER (self));

      if (!gst_omx_video_dec_fill_buffer (self, buf, outbuf)) {
        gst_buffer_unref (outbuf);
        gst_omx_port_release_buffer (port, buf);
        goto invalid_buffer;
      }
    }


    video_meta = gst_buffer_get_video_meta (outbuf);
    if (!video_meta) {
      gst_buffer_add_video_meta ((GstBuffer *)buf, GST_VIDEO_FRAME_FLAG_NONE,
        GST_VIDEO_FORMAT_NV12,
        port_getWH.format.video.nFrameWidth,
        port_getWH.format.video.nFrameHeight);
    }else{
      video_meta->width = port_getWH.format.video.nFrameWidth;
      video_meta->height = port_getWH.format.video.nFrameHeight;
    }

   /*because of discarding frame list,add finish funcs*/
    out_frame = gst_omx_video_dec_new_frame();
    if(out_frame == NULL){
      GST_ERROR("new GstVideoCodecFrame failed");
      goto flow_error;
    }else{
      out_frame->output_buffer = outbuf;
    }

    GST_BUFFER_PTS (outbuf) =
        gst_util_uint64_scale (buf->omx_buf->nTimeStamp, GST_SECOND, OMX_TICKS_PER_SECOND);
    if(self->vdec_proc_info.file_frame_rate != 0){
      GST_BUFFER_DURATION (outbuf) =
          (1000 / (self->vdec_proc_info.file_frame_rate / 1000)) * 1000 * 1000;
    }else{
      GST_BUFFER_DURATION (outbuf) =
          gst_util_uint64_scale (buf->omx_buf->nTickCount, GST_SECOND, OMX_TICKS_PER_SECOND);
    }

    GST_DEBUG("decoder output chain PTS %" GST_TIME_FORMAT ", DTS %" GST_TIME_FORMAT " duration %"
        GST_TIME_FORMAT " size %" G_GSIZE_FORMAT ,
        GST_TIME_ARGS (GST_BUFFER_PTS (outbuf)),
        GST_TIME_ARGS (GST_BUFFER_DTS (outbuf)),
        GST_TIME_ARGS (GST_BUFFER_DURATION (outbuf)), gst_buffer_get_size (outbuf));

    flow_ret =
          gst_video_decoder_finish_frame (GST_VIDEO_DECODER (self), out_frame);
    out_frame = NULL;
    buf = NULL;

    if(flow_ret != GST_FLOW_OK){
      GST_ERROR("finish frame failed");
    }
    /*END*/

  } else if (buf->omx_buf->nFilledLen > 0 || buf->eglimage) {
    if (self->out_port_pool) {
      gint i, n;
      GstBuffer *outbuf;
      GstBufferPoolAcquireParams params = { 0, };

      n = port->buffers->len;
      for (i = 0; i < n; i++) {
        GstOMXBuffer *tmp = g_ptr_array_index (port->buffers, i);

        if (tmp == buf)
          break;
      }
      g_assert (i != n);
      //GST_OMX_VIDEO_BUFFER_POOL_HIVO (self->out_port_pool)->current_buffer_index = i;
      GST_OMX_VIDEO_BUFFER_POOL_HIVO (self->out_port_pool)->cur_frame_index = i;
      flow_ret =
          gst_buffer_pool_acquire_buffer (self->out_port_pool,
          &outbuf, &params);
      if (flow_ret != GST_FLOW_OK) {
        flow_ret =
            gst_video_decoder_drop_frame (GST_VIDEO_DECODER (self), frame);
        frame = NULL;
      //{
      //    FILE* fp = fopen("/data/omx.log.txt", "ab");
      //    if(fp){
      //      fprintf(fp,"2 flow_ret != GST_FLOW_OK:gstomxbuf:%p, omxbuffer:%p, FILL count: %d, FILLDONE count: %d \r\n",
      //  buf, buf->omx_buf, buf->fill_count, buf->fill_done_count);
      //      fclose(fp);
      //    }
      //}
        gst_omx_port_release_buffer (port, buf);
        goto invalid_buffer;
      }
      /* BEGIN: Added by liurongliang(l00180035), 2015/3/6 */
      /*这里赋值解码后的pts，在解码后的第一帧的PTS确定segment的开始时间*/
      GST_BUFFER_TIMESTAMP(outbuf) = gst_util_uint64_scale (buf->omx_buf->nTimeStamp,
        GST_SECOND, OMX_TICKS_PER_SECOND);
      /* END:   Added by liurongliang(l00180035), 2015/3/6 */
      //if (GST_OMX_BUFFER_POOL (self->out_port_pool)->need_copy)
      //  outbuf =
      //      copy_frame (&GST_OMX_BUFFER_POOL (self->out_port_pool)->video_info,
      //      outbuf);

      frame->output_buffer = outbuf;;
      video_meta = gst_buffer_get_video_meta (outbuf);
      if (!video_meta) {
        gst_buffer_add_video_meta ((GstBuffer *)buf, GST_VIDEO_FRAME_FLAG_NONE,
         GST_VIDEO_FORMAT_NV12,
         port_getWH.format.video.nFrameWidth,
         port_getWH.format.video.nFrameHeight);
      }else{
        video_meta->width = port_getWH.format.video.nFrameWidth;
        video_meta->height = port_getWH.format.video.nFrameHeight;
      }
      flow_ret =
          gst_video_decoder_finish_frame (GST_VIDEO_DECODER (self), frame);

      frame = NULL;
      buf = NULL;
    } else {
      if ((flow_ret =
              gst_video_decoder_allocate_output_frame (GST_VIDEO_DECODER
                  (self), frame)) == GST_FLOW_OK) {
        /* FIXME: This currently happens because of a race condition too.
         * We first need to reconfigure the output port and then the input
         * port if both need reconfiguration.
         */
        if (!gst_omx_video_dec_fill_buffer (self, buf, frame->output_buffer)) {
          gst_buffer_replace (&frame->output_buffer, NULL);
          flow_ret =
              gst_video_decoder_drop_frame (GST_VIDEO_DECODER (self), frame);
          frame = NULL;
      //{
      //    FILE* fp = fopen("/data/omx.log.txt", "ab");
      //    if(fp){
      //      fprintf(fp,"3 flow_ret != GST_FLOW_OK:gstomxbuf:%p, omxbuffer:%p, FILL count: %d, FILLDONE count: %d \r\n",
      //  buf, buf->omx_buf, buf->fill_count, buf->fill_done_count);
      //      fclose(fp);
      //    }
      //}
          gst_omx_port_release_buffer (port, buf);
          goto invalid_buffer;
        }
        flow_ret =
            gst_video_decoder_finish_frame (GST_VIDEO_DECODER (self), frame);
        frame = NULL;
      }
    }
  } else if (frame != NULL) {
    GST_INFO_OBJECT (self, "frame is null ,drop frame");
    flow_ret = gst_video_decoder_drop_frame (GST_VIDEO_DECODER (self), frame);
    frame = NULL;
  }

  GST_DEBUG_OBJECT (self, "Read frame from component");

  GST_DEBUG_OBJECT (self, "Finished frame: %s", gst_flow_get_name (flow_ret));

  if (buf) {
    err = gst_omx_port_release_buffer (port, buf);
    if (err != OMX_ErrorNone)
      goto release_error;
  }

  self->downstream_flow_ret = flow_ret;

  if (flow_ret != GST_FLOW_OK)
    goto flow_error;

  GST_VIDEO_DECODER_STREAM_UNLOCK (self);

  return;

component_error:
  {
    GST_ELEMENT_ERROR (self, LIBRARY, FAILED, (NULL),
        ("OpenMAX component in error state %s (0x%08x)",
            gst_omx_component_get_last_error_string (self->dec),
            gst_omx_component_get_last_error (self->dec)));
    gst_pad_push_event (GST_VIDEO_DECODER_SRC_PAD (self), gst_event_new_eos ());
    gst_pad_pause_task (GST_VIDEO_DECODER_SRC_PAD (self));
    self->downstream_flow_ret = GST_FLOW_ERROR;
    self->started = FALSE;
    return;
  }

flushing:
  {
    GST_WARNING_OBJECT (self, "Flushing -- stopping task");
    gst_pad_pause_task (GST_VIDEO_DECODER_SRC_PAD (self));

    g_mutex_lock (&self->drain_lock);
    self->downstream_flow_ret = GST_FLOW_FLUSHING;
    self->started = FALSE;

    /* BEGIN: Added by liurongliang(l00180035), 2015/8/6 */
    /*
       fix issue:when EOS coming, here draining, then wait omx dec out finish.
       then seek occur,flush start, then gst_pad_push(dec out loop) return flush.
       the draining will block.
    */
    if (self->draining) {
      self->draining = FALSE;
      GST_WARNING_OBJECT (self, "Drained");
      g_cond_broadcast (&self->drain_cond);
    }
    /* END:   Added by liurongliang(l00180035), 2015/8/6 */
    g_mutex_unlock (&self->drain_lock);

    return;
  }

eos:
  {
    g_mutex_lock (&self->drain_lock);
    if (self->draining) {
      GstQuery *query = gst_query_new_drain ();

      /* Drain the pipeline to reclaim all memories back to the pool */
      if (!gst_pad_peer_query (GST_VIDEO_DECODER_SRC_PAD (self), query))
        GST_ERROR_OBJECT (self, "drain query failed");
      gst_query_unref (query);

      GST_INFO_OBJECT (self, "Drained");
      self->draining = FALSE;
      g_cond_broadcast (&self->drain_cond);
      flow_ret = GST_FLOW_OK;
      gst_pad_pause_task (GST_VIDEO_DECODER_SRC_PAD (self));
    } else {
      GST_INFO_OBJECT (self, "Component signalled EOS");
      flow_ret = GST_FLOW_EOS;
    }
    g_mutex_unlock (&self->drain_lock);

    GST_VIDEO_DECODER_STREAM_LOCK (self);
    self->downstream_flow_ret = flow_ret;

    /* Here we fallback and pause the task for the EOS case */
    if (flow_ret != GST_FLOW_OK)
      goto flow_error;

    GST_VIDEO_DECODER_STREAM_UNLOCK (self);

    return;
  }

flow_error:
  {
    if (flow_ret == GST_FLOW_EOS) {
      GST_WARNING_OBJECT (self, "EOS");

      gst_pad_push_event (GST_VIDEO_DECODER_SRC_PAD (self),
          gst_event_new_eos ());
      gst_pad_pause_task (GST_VIDEO_DECODER_SRC_PAD (self));
      self->started = FALSE;
    } else if (flow_ret < GST_FLOW_EOS) {
      GST_ELEMENT_ERROR (self, STREAM, FAILED,
          ("Internal data stream error."), ("stream stopped, reason %s",
              gst_flow_get_name (flow_ret)));

      gst_pad_push_event (GST_VIDEO_DECODER_SRC_PAD (self),
          gst_event_new_eos ());
      gst_pad_pause_task (GST_VIDEO_DECODER_SRC_PAD (self));
      self->started = FALSE;
    } else if (flow_ret == GST_FLOW_FLUSHING) {
      GST_WARNING_OBJECT (self, "Flushing -- stopping task");
      gst_pad_pause_task (GST_VIDEO_DECODER_SRC_PAD (self));
      self->started = FALSE;

      g_mutex_lock (&self->drain_lock);
      /* BEGIN: Added by liurongliang(l00180035), 2015/8/6 */
      /*
         fix issue:when EOS coming, here draining, then wait omx dec out finish.
         then seek occur,flush start, then gst_pad_push(dec out loop) return flush.
         the draining will block.
      */
      if (self->draining) {
        self->draining = FALSE;
        GST_WARNING_OBJECT (self, "Drained");
        g_cond_broadcast (&self->drain_cond);
      }
      self->downstream_flow_ret = GST_FLOW_FLUSHING;
      /* END:   Added by liurongliang(l00180035), 2015/8/6 */
      g_mutex_unlock (&self->drain_lock);
    }
    GST_VIDEO_DECODER_STREAM_UNLOCK (self);
    return;
  }

reconfigure_error:
  {
    GST_ELEMENT_ERROR (self, LIBRARY, SETTINGS, (NULL),
        ("Unable to reconfigure output port"));
    gst_pad_push_event (GST_VIDEO_DECODER_SRC_PAD (self), gst_event_new_eos ());
    GST_WARNING_OBJECT (self, "reconfigure_error, pausing task");
    gst_pad_pause_task (GST_VIDEO_DECODER_SRC_PAD (self));
    self->downstream_flow_ret = GST_FLOW_ERROR;
    self->started = FALSE;
    return;
  }

invalid_buffer:
  {
    GST_ELEMENT_ERROR (self, LIBRARY, SETTINGS, (NULL),
        ("Invalid sized input buffer"));
    gst_pad_push_event (GST_VIDEO_DECODER_SRC_PAD (self), gst_event_new_eos ());
    gst_pad_pause_task (GST_VIDEO_DECODER_SRC_PAD (self));
    self->downstream_flow_ret = GST_FLOW_NOT_NEGOTIATED;
    self->started = FALSE;
    GST_VIDEO_DECODER_STREAM_UNLOCK (self);
    return;
  }

caps_failed:
  {
    GST_ELEMENT_ERROR (self, LIBRARY, SETTINGS, (NULL), ("Failed to set caps"));
    gst_pad_push_event (GST_VIDEO_DECODER_SRC_PAD (self), gst_event_new_eos ());
    gst_pad_pause_task (GST_VIDEO_DECODER_SRC_PAD (self));
    GST_VIDEO_DECODER_STREAM_UNLOCK (self);
    self->downstream_flow_ret = GST_FLOW_NOT_NEGOTIATED;
    self->started = FALSE;
    return;
  }
release_error:
  {
    GST_ELEMENT_ERROR (self, LIBRARY, SETTINGS, (NULL),
        ("Failed to relase output buffer to component: %s (0x%08x)",
            gst_omx_error_to_string (err), err));
    gst_pad_push_event (GST_VIDEO_DECODER_SRC_PAD (self), gst_event_new_eos ());
    gst_pad_pause_task (GST_VIDEO_DECODER_SRC_PAD (self));
    self->downstream_flow_ret = GST_FLOW_ERROR;
    self->started = FALSE;
    GST_VIDEO_DECODER_STREAM_UNLOCK (self);
    return;
  }
}

static gboolean
gst_omx_video_dec_start (GstVideoDecoder * decoder)
{
  GstOMXVideoDec *self;

  self = GST_OMX_VIDEO_DEC (decoder);

  self->last_upstream_ts = 0;
  self->eos = FALSE;
  self->downstream_flow_ret = GST_FLOW_OK;

  return TRUE;
}

static gboolean
gst_omx_video_dec_stop (GstVideoDecoder * decoder)
{
  GstOMXVideoDec *self;

  self = GST_OMX_VIDEO_DEC (decoder);

  GST_INFO_OBJECT (self, "Stopping decoder");

  gst_omx_port_set_flushing (self->dec_in_port, 5 * GST_SECOND, TRUE);
  gst_omx_port_set_flushing (self->dec_out_port, 5 * GST_SECOND, TRUE);

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  gst_omx_port_set_flushing (self->egl_in_port, 5 * GST_SECOND, TRUE);
  gst_omx_port_set_flushing (self->egl_out_port, 5 * GST_SECOND, TRUE);
#endif

  gst_pad_stop_task (GST_VIDEO_DECODER_SRC_PAD (decoder));

  if (gst_omx_component_get_state (self->dec, 0) > OMX_StateIdle)
    gst_omx_component_set_state (self->dec, OMX_StateIdle);
#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  if (gst_omx_component_get_state (self->egl_render, 0) > OMX_StateIdle)
    gst_omx_component_set_state (self->egl_render, OMX_StateIdle);
#endif

  self->downstream_flow_ret = GST_FLOW_FLUSHING;
  self->started = FALSE;
  self->eos = FALSE;

  g_mutex_lock (&self->drain_lock);
  self->draining = FALSE;
  g_cond_broadcast (&self->drain_cond);
  g_mutex_unlock (&self->drain_lock);

  gst_omx_component_get_state (self->dec, 5 * GST_SECOND);
#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  gst_omx_component_get_state (self->egl_render, 1 * GST_SECOND);
#endif

  gst_buffer_replace (&self->codec_data, NULL);

  if (self->input_state)
    gst_video_codec_state_unref (self->input_state);
  self->input_state = NULL;

  GST_DEBUG_OBJECT (self, "Stopped decoder");

  return TRUE;
}

static gboolean
gst_omx_video_dec_negotiate (GstOMXVideoDec * self)
{
  OMX_VIDEO_PARAM_PORTFORMATTYPE param;
  OMX_ERRORTYPE err;
  GstCaps *comp_supported_caps;
  GList *negotiation_map = NULL, *l;
  GstCaps *templ_caps, *intersection;
  GstVideoFormat format;
  GstStructure *s;
  const gchar *format_str;

  GST_DEBUG_OBJECT (self, "Trying to negotiate a video format with downstream");

  templ_caps = gst_pad_get_pad_template_caps (GST_VIDEO_DECODER_SRC_PAD (self));
  intersection =
      gst_pad_peer_query_caps (GST_VIDEO_DECODER_SRC_PAD (self), templ_caps);
  gst_caps_unref (templ_caps);

  GST_DEBUG_OBJECT (self, "Allowed downstream caps: %" GST_PTR_FORMAT,
      intersection);

  negotiation_map =
      gst_omx_video_get_supported_colorformats (self->dec_out_port,
      self->input_state);

  comp_supported_caps = gst_omx_video_get_caps_for_map (negotiation_map);

  if (!gst_caps_is_empty (comp_supported_caps)) {
    GstCaps *tmp;

    tmp = gst_caps_intersect (comp_supported_caps, intersection);
    gst_caps_unref (intersection);
    intersection = tmp;
  }
  gst_caps_unref (comp_supported_caps);

  if (gst_caps_is_empty (intersection)) {
    gst_caps_unref (intersection);
    GST_ERROR_OBJECT (self, "Empty caps");
    g_list_free_full (negotiation_map,
        (GDestroyNotify) gst_omx_video_negotiation_map_free);
    return FALSE;
  }

  intersection = gst_caps_truncate (intersection);
  intersection = gst_caps_fixate (intersection);

  s = gst_caps_get_structure (intersection, 0);
  format_str = gst_structure_get_string (s, "format");
  if (!format_str ||
      (format =
          gst_video_format_from_string (format_str)) ==
      GST_VIDEO_FORMAT_UNKNOWN) {
    GST_ERROR_OBJECT (self, "Invalid caps: %" GST_PTR_FORMAT, intersection);
    gst_caps_unref (intersection);
    g_list_free_full (negotiation_map,
        (GDestroyNotify) gst_omx_video_negotiation_map_free);
    return FALSE;
  }

  GST_OMX_INIT_STRUCT (&param);
  param.nPortIndex = self->dec_out_port->index;

  for (l = negotiation_map; l; l = l->next) {
    GstOMXVideoNegotiationMap *m = l->data;

    if (m->format == format) {
      param.eColorFormat = m->type;
      break;
    }
  }

  GST_DEBUG_OBJECT (self, "Negotiating color format %s (%d)", format_str,
      param.eColorFormat);

  /* We must find something here */
  g_assert (l != NULL);
  g_list_free_full (negotiation_map,
      (GDestroyNotify) gst_omx_video_negotiation_map_free);

  err =
      gst_omx_component_set_parameter (self->dec,
      OMX_IndexParamVideoPortFormat, &param);
  if (err != OMX_ErrorNone) {
    GST_ERROR_OBJECT (self, "Failed to set video port format: %s (0x%08x)",
        gst_omx_error_to_string (err), err);
  }

  gst_caps_unref (intersection);
  return (err == OMX_ErrorNone);
}

static gboolean
gst_omx_video_dec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state)
{
  GstOMXVideoDec *self;
  GstOMXVideoDecClass *klass;
  GstVideoInfo *info = &state->info;
  gboolean is_format_change = FALSE;
  gboolean needs_disable = FALSE;
  OMX_PARAM_PORTDEFINITIONTYPE port_def;
  OMX_PARAM_PORTDEFINITIONTYPE out_port_def;
  GstVideoFormat format;
  GstVideoCodecState *out_state;
  OMX_ERRORTYPE err;
  gboolean is_negotiation_success = FALSE;
  GstAllocator * allocator_hivo = NULL;

  self = GST_OMX_VIDEO_DEC (decoder);
  klass = GST_OMX_VIDEO_DEC_GET_CLASS (decoder);

  GST_WARNING_OBJECT(self, "%s:v-dec set_format in...", SMEPLAYER_STAT_TAG_TIME);

  /*set the way of demux send streamer*/
  self->is_frame = info->is_frame;

  gst_omx_port_get_port_definition (self->dec_in_port, &port_def);

  /* Check if the caps change is a real format change or if only irrelevant
   * parts of the caps have changed or nothing at all.
   */
  is_format_change |= port_def.format.video.nFrameWidth != info->width;
  is_format_change |= port_def.format.video.nFrameHeight != info->height;
  is_format_change |= (port_def.format.video.xFramerate == 0
      && info->fps_n != 0)
      || (port_def.format.video.xFramerate !=
      (info->fps_n << 16) / (info->fps_d));
  is_format_change |= (self->codec_data != state->codec_data);
  if (klass->is_format_change)
    is_format_change |=
        klass->is_format_change (self, self->dec_in_port, state);

  needs_disable =
      gst_omx_component_get_state (self->dec,
      GST_CLOCK_TIME_NONE) != OMX_StateLoaded;
  /* If the component is not in Loaded state and a real format change happens
   * we have to disable the port and re-allocate all buffers. If no real
   * format change happened we can just exit here.
   */
  GST_INFO_OBJECT(self, "needs_disable=%d, is_format_change=%d",
    needs_disable, is_format_change);
  if (needs_disable && !is_format_change) {
    GST_DEBUG_OBJECT (self,
        "Already running and caps did not change the format");
    if (self->input_state)
      gst_video_codec_state_unref (self->input_state);
    self->input_state = gst_video_codec_state_ref (state);
    return TRUE;
  }

  if (needs_disable && is_format_change) {
#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
    GstOMXPort *out_port =
        self->eglimage ? self->egl_out_port : self->dec_out_port;
#else
    GstOMXPort *out_port = self->dec_out_port;
#endif

    GST_DEBUG_OBJECT (self, "Need to disable and drain decoder");

    gst_omx_video_dec_drain (self, FALSE);
    gst_omx_video_dec_flush (decoder);
    gst_omx_port_set_flushing (out_port, 5 * GST_SECOND, TRUE);

    if (klass->cdata.hacks & GST_OMX_HACK_NO_COMPONENT_RECONFIGURE) {
      GST_VIDEO_DECODER_STREAM_UNLOCK (self);
      gst_omx_video_dec_stop (GST_VIDEO_DECODER (self));
      gst_omx_video_dec_close (GST_VIDEO_DECODER (self));
      GST_VIDEO_DECODER_STREAM_LOCK (self);

      if (!gst_omx_video_dec_open (GST_VIDEO_DECODER (self)))
        return FALSE;
      needs_disable = FALSE;
    } else {
#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
      if (self->eglimage) {
        gst_omx_port_set_flushing (self->dec_in_port, 5 * GST_SECOND, TRUE);
        gst_omx_port_set_flushing (self->dec_out_port, 5 * GST_SECOND, TRUE);
        gst_omx_port_set_flushing (self->egl_in_port, 5 * GST_SECOND, TRUE);
        gst_omx_port_set_flushing (self->egl_out_port, 5 * GST_SECOND, TRUE);
      }
#endif

      if (gst_omx_port_set_enabled (self->dec_in_port, FALSE) != OMX_ErrorNone)
        return FALSE;
      if (gst_omx_port_set_enabled (out_port, FALSE) != OMX_ErrorNone)
        return FALSE;
      if (gst_omx_port_wait_buffers_released (self->dec_in_port,
              5 * GST_SECOND) != OMX_ErrorNone)
        return FALSE;
      if (gst_omx_port_wait_buffers_released (out_port,
              1 * GST_SECOND) != OMX_ErrorNone)
        return FALSE;
      if (gst_omx_port_deallocate_buffers (self->dec_in_port) != OMX_ErrorNone)
        return FALSE;
      if (gst_omx_video_dec_deallocate_output_buffers (self) != OMX_ErrorNone)
        return FALSE;
      if (gst_omx_port_wait_enabled (self->dec_in_port,
              1 * GST_SECOND) != OMX_ErrorNone)
        return FALSE;
      if (gst_omx_port_wait_enabled (out_port, 1 * GST_SECOND) != OMX_ErrorNone)
        return FALSE;

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
      if (self->eglimage) {
        OMX_STATETYPE egl_state;

        egl_state = gst_omx_component_get_state (self->egl_render, 0);
        if (egl_state > OMX_StateLoaded || egl_state == OMX_StateInvalid) {

          if (egl_state > OMX_StateIdle) {
            gst_omx_component_set_state (self->egl_render, OMX_StateIdle);
            gst_omx_component_set_state (self->dec, OMX_StateIdle);
            egl_state = gst_omx_component_get_state (self->egl_render,
                5 * GST_SECOND);
            gst_omx_component_get_state (self->dec, 1 * GST_SECOND);
          }
          gst_omx_component_set_state (self->egl_render, OMX_StateLoaded);
          gst_omx_component_set_state (self->dec, OMX_StateLoaded);

          gst_omx_close_tunnel (self->dec_out_port, self->egl_in_port);

          if (egl_state > OMX_StateLoaded) {
            gst_omx_component_get_state (self->egl_render, 5 * GST_SECOND);
          }

          gst_omx_component_set_state (self->dec, OMX_StateIdle);

          gst_omx_component_set_state (self->dec, OMX_StateExecuting);
          gst_omx_component_get_state (self->dec, GST_CLOCK_TIME_NONE);
        }
        self->eglimage = FALSE;
      }
#endif
    }
    if (self->input_state)
      gst_video_codec_state_unref (self->input_state);
    self->input_state = NULL;

    GST_DEBUG_OBJECT (self, "Decoder drained and disabled");
  }

  /* if demux do not pass W&H to decoder,we set a default one to allocate input and output buffer */
  if(0 != info->width  && 0 != info->height){
    port_def.format.video.nFrameWidth = info->width;
    port_def.format.video.nFrameHeight = info->height;
  }else{
    port_def.format.video.nFrameWidth = 1920;
    port_def.format.video.nFrameHeight = 1080;
  }


  GST_ERROR("info->width:%d, info->height:%d ",info->width, info->height);
  if (info->fps_n == 0)
    port_def.format.video.xFramerate = 0;
  else
    port_def.format.video.xFramerate = (info->fps_n << 16) / (info->fps_d);

  if(!self->is_frame){
    port_def.nBufferSize = GST_MAX_INPUT_BUFFER_ES;
  } else {
    /* BEGIN: Added by liurongliang(l00180035), 2015/3/11 */
    //for 4k we must enlarge the input buffer to 5MB
    if(((port_def.format.video.nFrameWidth > GST_FHD_WIDTH && port_def.format.video.nFrameHeight > GST_FHD_HEIGHT)
      || (port_def.format.video.nFrameWidth > GST_FHD_HEIGHT && port_def.format.video.nFrameHeight > GST_FHD_WIDTH))
      && port_def.nBufferSize < GST_MAX_INPUT_BUFFER_4K){
      port_def.nBufferSize = GST_MAX_INPUT_BUFFER_4K;
    }else{
      port_def.nBufferSize = GST_MAX_INPUT_BUFFER_FHD;
    }
    /* END:   Added by liurongliang(l00180035), 2015/3/11 */
  }

  GST_DEBUG_OBJECT (self, "Setting inport port definition");

  if (gst_omx_port_update_port_definition (self->dec_in_port,
          &port_def) != OMX_ErrorNone)
    return FALSE;

  if (klass->set_format) {
    if (!klass->set_format (self, self->dec_in_port, state)) {
      GST_ERROR_OBJECT (self, "Subclass failed to set the new format");
      return FALSE;
    }
  }

  GST_INFO_OBJECT (self, "Updating outport port definition");
  if (gst_omx_port_update_port_definition (self->dec_out_port,
          NULL) != OMX_ErrorNone)
    return FALSE;

  GST_INFO("outport,size=%ld, min=%ld, actual=%ld",
    self->dec_out_port->port_def.nBufferSize,
    self->dec_out_port->port_def.nBufferCountMin,
    self->dec_out_port->port_def.nBufferCountActual);

  /* BEGIN:  Added by lidanjing(l00346954),  2015/9/1 */
  // Add Features of video rotate
  if (info->rotation > 0) {
    OMX_PARAM_PORTDEFINITIONTYPE outport_def;
    OMX_CONFIG_ROTATIONTYPE rotate;
    OMX_ERRORTYPE rotate_config_res = OMX_ErrorNone;

    gst_omx_port_get_port_definition (self->dec_out_port, &outport_def);

    rotate.nPortIndex = outport_def.nPortIndex;
    rotate.nRotation = info->rotation;
    rotate.nSize = sizeof(OMX_CONFIG_ROTATIONTYPE);
    rotate.nVersion = outport_def.nVersion;

    GST_INFO("PortIndex: %ld,  nRotation: %ld,  nSize: %ld, nVersion: %ld",
      rotate.nPortIndex, rotate.nRotation,
      rotate.nSize, rotate.nVersion.nVersion);

    rotate_config_res = gst_omx_component_set_config (self->dec, OMX_IndexConfigCommonRotate, &rotate);
    if(rotate_config_res != OMX_ErrorNone) {
      GST_WARNING("set rotation config failed,the video may be upside down");
    }

  }
  /* END:  Added by lidanjing(l00346954),  2015/9/1 */
  GST_ERROR("state->codec_data:%p", state->codec_data);
  gst_buffer_replace (&self->codec_data, state->codec_data);
  self->input_state = gst_video_codec_state_ref (state);

  GST_INFO_OBJECT (self, "Enabling component klass->cdata.hacks=%llu, needs_disable=%d",
    klass->cdata.hacks, needs_disable);

  if (needs_disable) {
    if (gst_omx_port_set_enabled (self->dec_in_port, TRUE) != OMX_ErrorNone)
      return FALSE;
    if (gst_omx_port_allocate_buffers (self->dec_in_port) != OMX_ErrorNone)
      return FALSE;

    if ((klass->cdata.hacks & GST_OMX_HACK_NO_DISABLE_OUTPORT)) {
      if (gst_omx_port_set_enabled (self->dec_out_port, TRUE) != OMX_ErrorNone)
        return FALSE;
      if (gst_omx_port_allocate_buffers (self->dec_out_port) != OMX_ErrorNone)
        return FALSE;

      if (gst_omx_port_wait_enabled (self->dec_out_port,
              5 * GST_SECOND) != OMX_ErrorNone)
        return FALSE;
    }

    if (gst_omx_port_wait_enabled (self->dec_in_port,
            5 * GST_SECOND) != OMX_ErrorNone)
      return FALSE;
    if (gst_omx_port_mark_reconfigured (self->dec_in_port) != OMX_ErrorNone)
      return FALSE;
  } else {
    if (!gst_omx_video_dec_negotiate (self))
      GST_LOG_OBJECT (self, "Negotiation failed, will get output format later");

    if (!(klass->cdata.hacks & GST_OMX_HACK_NO_DISABLE_OUTPORT)) {
      /* Disable output port */
      if (gst_omx_port_set_enabled (self->dec_out_port, FALSE) != OMX_ErrorNone)
        return FALSE;

      if (gst_omx_port_wait_enabled (self->dec_out_port,
              1 * GST_SECOND) != OMX_ErrorNone)
        return FALSE;

      if (gst_omx_component_set_state (self->dec,
              OMX_StateIdle) != OMX_ErrorNone)
        return FALSE;

      /* Need to allocate buffers to reach Idle state */
      if (gst_omx_port_allocate_buffers (self->dec_in_port) != OMX_ErrorNone)
        return FALSE;
    } else {
      GST_WARNING_OBJECT(self, "%s:v-dec setting ->idle begin...",
          SMEPLAYER_STAT_TAG_TIME);
      OMX_HISI_PARAM_CHANNELATTRIBUTES para_chan;
      OMX_ERRORTYPE err_tmp;
      OMX_INDEXTYPE index_type;
      err_tmp = gst_omx_component_get_extension_index(self->dec_out_port->comp,
          "OMX.Hisi.Param.Index.ChannelAttributes", &index_type);
      if (err_tmp == OMX_ErrorNone) {
        err_tmp = gst_omx_component_get_parameter(self->dec_out_port->comp, index_type, &para_chan);

        if (err_tmp == OMX_ErrorNone) {
          para_chan.nErrorThreshold = 0;
          err_tmp = gst_omx_component_set_parameter(self->dec_out_port->comp, index_type, &para_chan);
          GST_WARNING_OBJECT(self, "gst_omx_component_set_parameter: set nErrorThreshold failed,err:%d",err_tmp);
        }else{
          GST_WARNING_OBJECT(self, "gst_omx_component_get_parameter: failed index_type:%d,err:%d", index_type, err_tmp);
        }
      }else{
        GST_ERROR_OBJECT(self, "gst_omx_component_get_extension_index failed,err:%d",err_tmp);
      }

      if (gst_omx_component_set_state (self->dec,
              OMX_StateIdle) != OMX_ErrorNone)
        return FALSE;
      GST_WARNING_OBJECT(self, "%s:v-dec setting ->idle end",
          SMEPLAYER_STAT_TAG_TIME);

      /* Need to allocate buffers to reach Idle state */
      GST_WARNING_OBJECT(self, "%s:v-dec alloc inport buffer begin...",
          SMEPLAYER_STAT_TAG_TIME);
      if (gst_omx_port_allocate_buffers (self->dec_in_port) != OMX_ErrorNone){
        GST_ERROR_OBJECT(self, "allocate inport buffer failed");
        if (gst_omx_component_set_state (self->dec, OMX_StateLoaded) != OMX_ErrorNone){
          GST_WARNING_OBJECT(self, "set_state to Loaded failed!");
        }

        return FALSE;
      }
      GST_WARNING_OBJECT(self, "%s:v-dec alloc inport buffer end",
          SMEPLAYER_STAT_TAG_TIME);
      /* We do not allocate outport buffers here, in fact, we allocate them when active pool with MMZ*/

      /* BEGIN:  Change channels keeping last frame , Added by lidanjing(l00346954),  2015/11/3 */
      GST_VIDEO_DECODER_STREAM_LOCK (self);

      gst_omx_port_get_port_definition (self->dec_out_port, &out_port_def);
      g_assert (out_port_def.format.video.eCompressionFormat ==
          OMX_VIDEO_CodingUnused);

      format =
          gst_omx_video_get_format_from_omx (out_port_def.format.
          video.eColorFormat);

      if (format == GST_VIDEO_FORMAT_UNKNOWN) {
        GST_ERROR_OBJECT (self, "Unsupported color format: %d",
            out_port_def.format.video.eColorFormat);
      }

      GST_DEBUG_OBJECT (self,
          "Setting output state: format %s (%d), width %u, height %u, stride: %u, actualbuffer : %lu, Minbuffer :%lu",
          gst_video_format_to_string (format),
          out_port_def.format.video.eColorFormat,
          (guint) out_port_def.format.video.nFrameWidth,
          (guint) out_port_def.format.video.nFrameHeight,
          (guint) out_port_def.format.video.nStride,
          out_port_def.nBufferCountActual,
          out_port_def.nBufferCountMin);

      out_state = gst_video_decoder_set_output_state (GST_VIDEO_DECODER (self),
          format, out_port_def.format.video.nFrameWidth,
          out_port_def.format.video.nFrameHeight, self->input_state);

      // Take framerate and pixel-aspect-ratio from sinkpad caps

      is_negotiation_success = gst_video_decoder_negotiate (GST_VIDEO_DECODER (self));

      if (is_negotiation_success == FALSE) {
        GST_WARNING("negotiation failed,why?");
      }
      gst_video_codec_state_unref (out_state);

      gst_video_decoder_get_allocator(decoder, &allocator_hivo, NULL);
      if(allocator_hivo == NULL)
      {
        GST_ERROR("get allocator failed, allocat output buffers failed");
        if(gst_omx_component_set_state (self->dec, OMX_StateLoaded) != OMX_ErrorNone){
          GST_WARNING_OBJECT(self, "idleing -> loaded failed !");
        }
        gst_omx_port_deallocate_buffers(self->dec_in_port);
        if (gst_omx_component_get_state (self->dec, 5*GST_SECOND) !=  OMX_StateLoaded){
            GST_WARNING_OBJECT(self, "idleing -> loaded failed !");
        }
        GST_VIDEO_DECODER_STREAM_UNLOCK (self);
        return FALSE;
      }

      GST_WARNING_OBJECT(self, "%s:v-dec alloc outport buffer begin...",
          SMEPLAYER_STAT_TAG_TIME);
      err = gst_omx_video_dec_allocate_output_buffers(self, allocator_hivo);
      GST_WARNING_OBJECT(self, "%s:v-dec alloc outport buffer end(%#x)",
          SMEPLAYER_STAT_TAG_TIME, err);
      if (err != OMX_ErrorNone){
        GST_WARNING("ALLOCATE OUTPUT BUFFER ERROR");
        if(gst_omx_component_set_state (self->dec, OMX_StateLoaded) != OMX_ErrorNone){
          GST_WARNING_OBJECT(self, "idleing -> loaded failed !");
        }
        gst_omx_port_deallocate_buffers(self->dec_in_port);
        if (gst_omx_component_get_state (self->dec, 5*GST_SECOND) !=  OMX_StateLoaded){
            GST_WARNING_OBJECT(self, "idleing -> loaded failed !");
        }
        gst_object_unref(allocator_hivo);
        GST_VIDEO_DECODER_STREAM_UNLOCK (self);
        return FALSE;
      }
      /* BEGIN: Added for DTS2016042806936,  2016/05/25 */
      gst_object_unref(allocator_hivo);
      /* END: Added for DTS2016042806936,  2016/05/25 */
      GST_VIDEO_DECODER_STREAM_UNLOCK (self);

     /* END:  Change channels keeping last frame , Added by lidanjing(l00346954),  2015/11/3 */

      }
    }
      GST_WARNING_OBJECT(self, "%s:v-dec get ->idle begin...",
          SMEPLAYER_STAT_TAG_TIME);
    if (gst_omx_component_get_state (self->dec,
            GST_CLOCK_TIME_NONE) != OMX_StateIdle)
      return FALSE;
      GST_WARNING_OBJECT(self, "%s:v-dec get ->idle end",
          SMEPLAYER_STAT_TAG_TIME);

    if (gst_omx_component_set_state (self->dec,
            OMX_StateExecuting) != OMX_ErrorNone)
      return FALSE;
    GST_WARNING_OBJECT(self, "%s:v-dec get ->Executing begin...",
        SMEPLAYER_STAT_TAG_TIME);
    if (gst_omx_component_get_state (self->dec,
            GST_CLOCK_TIME_NONE) != OMX_StateExecuting)
      return FALSE;
    GST_WARNING_OBJECT(self, "%s:v-dec get ->Executing end",
        SMEPLAYER_STAT_TAG_TIME);
  /* Unset flushing to allow ports to accept data again */
  gst_omx_port_set_flushing (self->dec_in_port, 5 * GST_SECOND, FALSE);
  gst_omx_port_set_flushing (self->dec_out_port, 5 * GST_SECOND, FALSE);

  if (gst_omx_component_get_last_error (self->dec) != OMX_ErrorNone) {
    GST_ERROR_OBJECT (self, "Component in error state: %s (0x%08x)",
        gst_omx_component_get_last_error_string (self->dec),
        gst_omx_component_get_last_error (self->dec));
    return FALSE;
  }

  GST_WARNING_OBJECT(self, "%s:v-dec populate outport begin...",
      SMEPLAYER_STAT_TAG_TIME);
  err = gst_omx_port_populate (self->dec_out_port);
  if (err != OMX_ErrorNone){
    GST_WARNING("POPULATE FAILED");
    gst_omx_video_dec_deallocate_output_buffers (self);
  }
  GST_WARNING_OBJECT(self, "%s:v-dec populate outport end",
      SMEPLAYER_STAT_TAG_TIME);

  self->downstream_flow_ret = GST_FLOW_OK;

  GST_WARNING_OBJECT(self, "%s:v-dec set_format out", SMEPLAYER_STAT_TAG_TIME);

  return TRUE;
}

static gboolean
gst_omx_video_dec_flush (GstVideoDecoder * decoder)
{
  GstOMXVideoDec *self = GST_OMX_VIDEO_DEC (decoder);
  OMX_ERRORTYPE err = OMX_ErrorNone;

  GST_INFO_OBJECT (self, "Flushing decoder");

  if (gst_omx_component_get_state (self->dec, 0) == OMX_StateLoaded)
    return TRUE;

  /* BEGIN: Modified by liurongliang(l00180035), 2015/3/26 */
  /*first flush,then stop out loop*/
  /* 1) Pause the components */
  /*
  if (gst_omx_component_get_state (self->dec, 0) == OMX_StateExecuting) {
    gst_omx_component_set_state (self->dec, OMX_StatePause);
    gst_omx_component_get_state (self->dec, GST_CLOCK_TIME_NONE);
  }
  */
#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  if (self->eglimage) {
    if (gst_omx_component_get_state (self->egl_render, 0) == OMX_StateExecuting) {
      gst_omx_component_set_state (self->egl_render, OMX_StatePause);
      gst_omx_component_get_state (self->egl_render, GST_CLOCK_TIME_NONE);
    }
  }
#endif

  /* 2) Flush the ports */
  GST_INFO_OBJECT (self, "flushing ports");
  gst_omx_port_set_flushing (self->dec_in_port, 5 * GST_SECOND, TRUE);
  gst_omx_port_set_flushing (self->dec_out_port, 5 * GST_SECOND, TRUE);

  GST_INFO_OBJECT (self, "Flushing -- task stopp ...");
  /* 0) Wait until the srcpad loop is stopped,
   * unlock GST_VIDEO_DECODER_STREAM_LOCK to prevent deadlocks
   * caused by using this lock from inside the loop function */
  GST_VIDEO_DECODER_STREAM_UNLOCK (self);
  GST_INFO_OBJECT (self, "Flushing -- task stopp(end unlock) ...");
  gst_pad_stop_task (GST_VIDEO_DECODER_SRC_PAD (decoder));
  GST_INFO_OBJECT (self, "Flushing -- task stopped");
  GST_VIDEO_DECODER_STREAM_LOCK (self);
  /* END:   Modified by liurongliang(l00180035), 2015/3/26 */

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  if (self->eglimage) {
    gst_omx_port_set_flushing (self->egl_in_port, 5 * GST_SECOND, TRUE);
    gst_omx_port_set_flushing (self->egl_out_port, 5 * GST_SECOND, TRUE);
  }
#endif

  /* 3) Resume components */
  //gst_omx_component_set_state (self->dec, OMX_StateExecuting);
  //gst_omx_component_get_state (self->dec, GST_CLOCK_TIME_NONE);
#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  if (self->eglimage) {
    gst_omx_component_set_state (self->egl_render, OMX_StateExecuting);
    gst_omx_component_get_state (self->egl_render, GST_CLOCK_TIME_NONE);
  }
#endif

  /* 4) Unset flushing to allow ports to accept data again */
  gst_omx_port_set_flushing (self->dec_in_port, 5 * GST_SECOND, FALSE);
  gst_omx_port_set_flushing (self->dec_out_port, 5 * GST_SECOND, FALSE);

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  if (self->eglimage) {
    gst_omx_port_set_flushing (self->egl_in_port, 5 * GST_SECOND, FALSE);
    gst_omx_port_set_flushing (self->egl_out_port, 5 * GST_SECOND, FALSE);
    err = gst_omx_port_populate (self->egl_out_port);
    gst_omx_port_mark_reconfigured (self->egl_out_port);
  } else {
    err = gst_omx_port_populate (self->dec_out_port);
  }
#else
  err = gst_omx_port_populate (self->dec_out_port);
  gst_omx_port_mark_reconfigured (self->dec_out_port);
#endif

  if (err != OMX_ErrorNone) {
    GST_WARNING_OBJECT (self, "Failed to populate output port: %s (0x%08x)",
        gst_omx_error_to_string (err), err);
  }

  /* Reset our state */
  self->last_upstream_ts = 0;
  self->eos = FALSE;
  self->downstream_flow_ret = GST_FLOW_OK;
  self->started = FALSE;

  self->input_count = 0;
  self->is_dec_out = FALSE;

#ifdef USE_OMX_DUMP_DATA
  self->flag_seek = TRUE;
#endif
  GST_INFO_OBJECT (self, "Flush finished");

  return TRUE;
}

/* BEGIN: Added for DTS2015071507912 by liurongliang(l00180035), 2015/7/26 */
//generate a decode only buffer to let sink do aysnc done,but not render.
#define DECODE_ONLY_BUF_SIZE 4
static GstBuffer* gen_decodeonly_gstbuffer()
{
  GstBuffer* deconly_buf = gst_buffer_new_and_alloc(DECODE_ONLY_BUF_SIZE);
  guchar buf[DECODE_ONLY_BUF_SIZE] = {0,};

  if(deconly_buf){
    GST_BUFFER_FLAG_SET(deconly_buf, GST_BUFFER_FLAG_DECODE_ONLY);
    GST_BUFFER_PTS(deconly_buf) = 0;
    GST_BUFFER_DTS(deconly_buf) = 0;
    GST_BUFFER_DURATION(deconly_buf) = 0;
    memset(buf, 0, sizeof(buf));
    gst_buffer_fill(deconly_buf, 0, (gconstpointer)buf, DECODE_ONLY_BUF_SIZE);
  }
  GST_INFO("gen en GenADecodeOnlyBuffer buffer,deconly_buf=%p", deconly_buf);

  return deconly_buf;
}
/* END:   Added for DTS2015071507912 by liurongliang(l00180035), 2015/7/26 */


/* BEGIN: Added for DTS2015082509727 by lidanjing(l00346954), 2015/8/31 */
/* send decoder only buf and eos event */
static gboolean send_decbuf_and_eos_event(GstOMXVideoDec *self)
{
  GstBuffer* empty_buffer = gen_decodeonly_gstbuffer();
  gboolean err = FALSE;

  GstEvent* ev_eos = gst_event_new_eos();
  gst_pad_push (GST_VIDEO_DECODER_SRC_PAD(self), empty_buffer);
  if(ev_eos){
    err = gst_pad_push_event(GST_VIDEO_DECODER_SRC_PAD(self), ev_eos);
    GST_INFO("push eos return err=%d", err);
  }else{
    GST_WARNING("ev_eos is null");
  }

  return err;
}

/* clear priv->frames */

static void
gst_omx_video_dec_clean_all_frames (GstVideoDecoder * decoder)
{
  GList *priv_frames = gst_video_decoder_get_frames(decoder);

  if(priv_frames){
    for (GList *l = priv_frames; l; l = l->next) {
      GstVideoCodecFrame *tmp = l->data;
      gst_video_decoder_release_frame (decoder, tmp);
    }
    g_list_free (priv_frames);
  }
  return;

}

/* END: Added for DTS2015082509727 by lidanjing(l00346954), 2015/8/31 */

static void gst_omx_video_dump_es(GstOMXVideoDec *self,  GstOMXBuffer *buf)
{
 if(self->es_dump_switch){
      if(self->dump_es_file == NULL){
        char dump_file_name[512];
        g_snprintf(dump_file_name, 512, "%s/%s_in_%u.es", self->dump_es_path,
            GST_ELEMENT_NAME(self), self->dump_es_index++);
        self->dump_es_file = fopen(dump_file_name, "ab+");
        if(self->dump_es_file == NULL){
          GST_WARNING("dump failed,open %s failed", dump_file_name);
          return;
        }else{
          /* dump codec data */
          if(self->codec_data_bak && self->is_need_codec){
            if(fwrite(self->codec_data_bak, 1, self->code_data_bak_len,
              self->dump_es_file) != self->code_data_bak_len){
              GST_WARNING("write data failed");
              return;
            }
            self->is_need_codec = FALSE;
          }
        }
      }

      if(self->dump_es_file != NULL){
        if(fwrite(buf->omx_buf->pBuffer + buf->omx_buf->nOffset, 1,
            buf->omx_buf->nFilledLen, self->dump_es_file) != buf->omx_buf->nFilledLen){
          GST_WARNING("write data failed");
        }
      }

    }else{
      if(self->dump_es_file) {
        fclose(self->dump_es_file);
        self->dump_es_file = NULL;
      }
    }
  return;

}


static GstFlowReturn
gst_omx_video_dec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame)
{
  GstOMXAcquireBufferReturn acq_ret = GST_OMX_ACQUIRE_BUFFER_ERROR;
  GstOMXVideoDec *self;
  GstOMXVideoDecClass *klass;
  GstOMXPort *port;
  GstOMXBuffer *buf;
  GstBuffer *codec_data = NULL;
  guint offset = 0, size;
  GstClockTime timestamp, duration;
  OMX_ERRORTYPE err;
  static guint frame_number = 0;
  static long dec_in_num = 0;

  //static int ping_num = 0;

  self = GST_OMX_VIDEO_DEC (decoder);
  klass = GST_OMX_VIDEO_DEC_GET_CLASS (self);


  /*clear frame list*/
  gst_omx_video_dec_clean_all_frames(decoder);
  //usleep(1000000);us 10^-6  //ms  10^-3


  /* print vdec input diff time */
  /*
  GstClockTime dec_in_out_diff = 0;
  dec_in_last_time = dec_in_cur_time;
  dec_in_cur_time = gst_clock_get_time (GST_ELEMENT_CLOCK (self));
  dec_in_out_diff = (dec_in_cur_time - dec_in_last_time);
  GST_ERROR("ldj dec in : %lld", dec_in_out_diff);
  */

  /* print vdec input PTS */
  /*
  guint dec_in_ns = 0;
  guint dec_in_ms = 0;
  dec_in_ns = (guint)(((GstClockTime)(GST_BUFFER_PTS (frame->input_buffer))) % GST_SECOND );
  dec_in_ms = (guint) ((((GstClockTime)(GST_BUFFER_PTS (frame->input_buffer))) / GST_SECOND) % 60) * 1000;
  dec_in_ms = dec_in_ns / 1000000 + dec_in_ms;
  //GST_ERROR("ldj diff ms : %u", dec_in_ms);
  if((dec_in_ms - last_pts)  != 40)
  {
    GST_ERROR("ldj diff PTS3 : %u", dec_in_ms - last_pts);
  }
  */

  GST_DEBUG_OBJECT(decoder,
    "decoder input chain PTS %" GST_TIME_FORMAT ", DTS %" GST_TIME_FORMAT " duration %"
    GST_TIME_FORMAT " size %" G_GSIZE_FORMAT " count: %ld ",
    GST_TIME_ARGS (GST_BUFFER_PTS (frame->input_buffer)),
    GST_TIME_ARGS (GST_BUFFER_DTS (frame->input_buffer)),
    GST_TIME_ARGS (GST_BUFFER_DURATION (frame->input_buffer)), gst_buffer_get_size (frame->input_buffer)
    ,dec_in_num++);


    /*put some frame into one, test for different way of send streamer*/
/*
    GstMapInfo input_info;
    if(ping_num < 8){
      gst_buffer_map (frame->input_buffer, &input_info, GST_MAP_READ);

      memcpy (hugedata + huge_size, (guint8 *) input_info.data, input_info.size);
      huge_size += input_info.size;
      ping_num++;

      gst_buffer_unmap(frame->input_buffer, &input_info);
      gst_video_codec_frame_unref (frame);
      return GST_FLOW_OK;
    }
    else if(ping_num == 8){
      gst_buffer_map (frame->input_buffer, &input_info, GST_MAP_READ);

      memcpy (hugedata + huge_size, (guint8 *) input_info.data, input_info.size);
      huge_size += input_info.size;
      ping_num = 0;

      gst_buffer_unmap(frame->input_buffer, &input_info);
    }
*/


  /* dump decoder input data */
/*
  gst_buffer_map (frame->input_buffer, &input_info, GST_MAP_READ);
  if(self->total_frame_data != NULL){
    if(fwrite(input_info.data, 1, input_info.size, self->total_frame_data) !=  input_info.size){
      GST_ERROR("write decoder input  failed");
    }
  }
  gst_buffer_unmap(frame->input_buffer, &input_info);
  gst_video_codec_frame_unref (frame);
  return GST_FLOW_OK;
*/



/* put data into omx ,between two vaild pts */
/*
  gst_buffer_map (frame->input_buffer, &input_info, GST_MAP_READ);
  GST_ERROR("input_info.size : %d", input_info.size);
  if( GST_BUFFER_PTS (frame->input_buffer) == INVAILD_TIME_STAMP && NULL != hugedata){
      //FILE* fp = fopen("/data/dump/handle.es", "ab");
      //fwrite(input_info.data, 1 , input_info.size, fp);
      //fclose(fp);
      if(vaild_pts_num == 3){
        memcpy (hugedata + huge_size, (guint8 *) hugedata_bak, bak_size);
        huge_size += bak_size;

        bak_size = 0;
        vaild_pts_num = 1;
      }

      memcpy (hugedata + huge_size, (guint8 *) input_info.data, input_info.size);
      huge_size += input_info.size;

      gst_buffer_unmap(frame->input_buffer, &input_info);
      gst_video_codec_frame_unref (frame);
      return GST_FLOW_OK;
  }else{

      vaild_pts_num++;
      if(vaild_pts_num == 1){
        memcpy (hugedata + huge_size, (guint8 *) input_info.data, input_info.size);
        huge_size += input_info.size;
        return GST_FLOW_OK;
      }

      if(vaild_pts_num == 2){
        memcpy (hugedata_bak, (guint8 *) input_info.data, input_info.size);
        bak_size = input_info.size;

        vaild_pts_num = 3;

      }

      if(vaild_pts_num == 4){
        memcpy (hugedata + huge_size, (guint8 *) hugedata_bak, bak_size);
        huge_size += bak_size;

        memcpy (hugedata_bak, (guint8 *) input_info.data, input_info.size);
        bak_size = input_info.size;

        vaild_pts_num = 3;
      }
  }
*/

  /*put 2097216 into OMX*/

  /*
  gst_buffer_map (frame->input_buffer, &input_info, GST_MAP_READ);

  if(bak_size > 0){
    memcpy (hugedata + huge_size, (guint8 *) hugedata_bak, bak_size);
    huge_size += bak_size;

    bak_size = 0;
  }

  if(huge_size + input_info.size < INPUT_OMX_SIZE){

    memcpy (hugedata + huge_size, (guint8 *) input_info.data, input_info.size);
    huge_size += input_info.size;
    gst_buffer_unmap(frame->input_buffer, &input_info);
    gst_video_codec_frame_unref (frame);
    return GST_FLOW_OK;

  }else if(huge_size + input_info.size == INPUT_OMX_SIZE){

    memcpy (hugedata + huge_size, (guint8 *) input_info.data, input_info.size);
    huge_size += input_info.size;

  }else{

    memcpy (hugedata + huge_size, (guint8 *) input_info.data, INPUT_OMX_SIZE - huge_size);
    huge_size += INPUT_OMX_SIZE - huge_size;


    memcpy (hugedata_bak + bak_size, (guint8 *) (input_info.data + INPUT_OMX_SIZE - huge_size),
        huge_size + input_info.size - INPUT_OMX_SIZE);
    bak_size += huge_size + input_info.size - INPUT_OMX_SIZE;

  }

  gst_buffer_unmap(frame->input_buffer, &input_info);
  */

  if(GST_VIDEO_CODEC_FRAME_IS_SYNC_POINT (frame)){
    self->vdec_proc_info.Iframe_cnt++;
    if(self->vdec_proc_info.Iframe_cnt == 1)
    {
      self->vdec_proc_info.fir_Iframe_vdecT = gst_omx_video_dec_get_cur_systime();
      GST_DEBUG("vdec in time : %lld", self->vdec_proc_info.fir_Iframe_vdecT);
    }
    GST_DEBUG("receive I frame number is %ld", self->vdec_proc_info.Iframe_cnt);
  }

  /* BEGIN: Added for DTS2015042111461 by liurongliang(l00180035), 2015/4/25 */
  if(self->dec_in_port->port_def.format.video.nFrameWidth < GST_MIN_WIDTH
    || self->dec_in_port->port_def.format.video.nFrameHeight < GST_MIN_HEIGHT){

    if(!send_decbuf_and_eos_event(self)){
      GST_WARNING("push eos message failed");
    }
    gst_omx_video_dec_clean_all_frames(decoder);
    GST_ELEMENT_WARNING(decoder, STREAM, DECODE,
      ("warning:w:h[%ld:%ld],cannot support",
        self->dec_in_port->port_def.format.video.nFrameWidth,
        self->dec_in_port->port_def.format.video.nFrameHeight),
        ("video_dec:"));

      return GST_FLOW_ERROR;
  }
  /* END:   Added for DTS2015042111461 by liurongliang(l00180035), 2015/4/25 */

  if (self->eos) {
    GST_WARNING_OBJECT (self, "Got frame after EOS");
    gst_video_codec_frame_unref (frame);
    return GST_FLOW_EOS;
  }

  if (!self->started) {
    if (!GST_VIDEO_CODEC_FRAME_IS_SYNC_POINT (frame)) {
      gst_video_decoder_drop_frame (GST_VIDEO_DECODER (self), frame);
      return GST_FLOW_OK;
    }
    GST_INFO_OBJECT (self, "Starting task");
    gst_pad_start_task (GST_VIDEO_DECODER_SRC_PAD (self),
        (GstTaskFunction) gst_omx_video_dec_loop, decoder, NULL);

    /* BEGIN: Added for DTS2015092207767 by liurongliang(l00180035), 2015/10/10 */
    //we should set self->started TRUE here
    //or it will ocur next seq:
    //1. run to "gst_omx_port_acquire_buffer (port, &buf);" of this func
    //2. run to "gst_omx_video_dec_flush" and complete this func.
    //3. run to after "gst_omx_port_acquire_buffer (port, &buf);" of this func
    // if this occur. the "gst_omx_video_dec_loop" task will not be start,until next seek.
    self->started = TRUE;
    /* END:   Added for DTS2015092207767 by liurongliang(l00180035), 2015/10/10 */
  }


  timestamp = GST_BUFFER_PTS (frame->input_buffer);
  duration = GST_BUFFER_DURATION (frame->input_buffer);

  if (self->downstream_flow_ret != GST_FLOW_OK) {
    gst_video_codec_frame_unref (frame);
    return self->downstream_flow_ret;
  }

  if (klass->prepare_frame) {
    GstFlowReturn ret;

    ret = klass->prepare_frame (self, frame);
    if (ret != GST_FLOW_OK) {
      GST_ERROR_OBJECT (self, "Preparing frame failed: %s",
          gst_flow_get_name (ret));
      gst_video_codec_frame_unref (frame);
      return ret;
    }
  }

  port = self->dec_in_port;

  size = gst_buffer_get_size (frame->input_buffer);
//  GST_ERROR("input_buffer size : %d", size);
  while (offset < size /*offset < huge_size*/) {
    /* Make sure to release the base class stream lock, otherwise
     * _loop() can't call _finish_frame() and we might block forever
     * because no input buffers are released */
    GST_VIDEO_DECODER_STREAM_UNLOCK (self);

    GST_DEBUG("SIZE : %d, OFFSET : %d",size, offset);
    acq_ret = gst_omx_port_acquire_buffer (port, &buf);

    if (acq_ret == GST_OMX_ACQUIRE_BUFFER_ERROR) {
      GST_VIDEO_DECODER_STREAM_LOCK (self);
      goto component_error;
    } else if (acq_ret == GST_OMX_ACQUIRE_BUFFER_FLUSHING) {
      GST_VIDEO_DECODER_STREAM_LOCK (self);
      goto flushing;
    } else if (acq_ret == GST_OMX_ACQUIRE_BUFFER_RECONFIGURE) {
      /* Reallocate all buffers */
      err = gst_omx_port_set_enabled (port, FALSE);
      if (err != OMX_ErrorNone) {
        GST_VIDEO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_wait_buffers_released (port, 5 * GST_SECOND);
      if (err != OMX_ErrorNone) {
        GST_VIDEO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_deallocate_buffers (port);
      if (err != OMX_ErrorNone) {
        GST_VIDEO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_wait_enabled (port, 1 * GST_SECOND);
      if (err != OMX_ErrorNone) {
        GST_VIDEO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_set_enabled (port, TRUE);
      if (err != OMX_ErrorNone) {
        GST_VIDEO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_allocate_buffers (port);
      if (err != OMX_ErrorNone) {
        GST_VIDEO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_wait_enabled (port, 5 * GST_SECOND);
      if (err != OMX_ErrorNone) {
        GST_VIDEO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_mark_reconfigured (port);
      if (err != OMX_ErrorNone) {
        GST_VIDEO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      /* Now get a new buffer and fill it */
      GST_VIDEO_DECODER_STREAM_LOCK (self);
      continue;
    }
    GST_VIDEO_DECODER_STREAM_LOCK (self);

    g_assert (acq_ret == GST_OMX_ACQUIRE_BUFFER_OK && buf != NULL);
    //we should drop this frame comes here before flush-start.
    if(GST_PAD_IS_FLUSHING (GST_VIDEO_DECODER_SINK_PAD(self))){
      GstOMXComponent *comp = port->comp;
      GST_INFO_OBJECT(self, "flush vdec drop this buffer reback to pool begin !");
      g_mutex_lock (&comp->lock);
      g_queue_push_tail (&port->pending_buffers, buf);
      g_mutex_unlock (&comp->lock);
      GST_INFO_OBJECT(self, "flush vdec drop this buffer reback to pool end!");
      goto flushing;
    }
    if (buf->omx_buf->nAllocLen - buf->omx_buf->nOffset <= 0) {
      gst_omx_port_release_buffer (port, buf);
      goto full_buffer;
    }

    if (self->downstream_flow_ret != GST_FLOW_OK) {
      gst_omx_port_release_buffer (port, buf);
      goto flow_error;
    }

    if (self->codec_data) {
      GST_INFO_OBJECT (self, "Passing codec data to the component");

      codec_data = self->codec_data;

      if (buf->omx_buf->nAllocLen - buf->omx_buf->nOffset <
          gst_buffer_get_size (codec_data)) {
        gst_omx_port_release_buffer (port, buf);
        goto too_large_codec_data;
      }

      buf->omx_buf->nFlags |= OMX_BUFFERFLAG_CODECCONFIG;
      buf->omx_buf->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
      buf->omx_buf->nFilledLen = gst_buffer_get_size (codec_data);;
      gst_buffer_extract (codec_data, 0,
          buf->omx_buf->pBuffer + buf->omx_buf->nOffset,
          buf->omx_buf->nFilledLen);
#ifdef USE_OMX_DUMP_DATA
      if(self->dump_codec_data != NULL){
        if(fwrite(buf->omx_buf->pBuffer + buf->omx_buf->nOffset, 1, buf->omx_buf->nFilledLen, self->dump_codec_data) != buf->omx_buf->nFilledLen){
          GST_WARNING("write CodecData failed");
        }
      }
#endif
      GST_INFO_OBJECT (self, "Passing codec data to the component,len=%ld",
        buf->omx_buf->nFilledLen);

     /* store codec_data */
      gst_omx_video_store_codec(self, buf->omx_buf->pBuffer + buf->omx_buf->nOffset, buf->omx_buf->nFilledLen);

      if (timestamp != GST_CLOCK_TIME_NONE) {
        buf->omx_buf->nTimeStamp =
            gst_util_uint64_scale (timestamp, OMX_TICKS_PER_SECOND, GST_SECOND);
        self->last_upstream_ts = timestamp;
      } else {
        buf->omx_buf->nTimeStamp = -1;
      }
      buf->omx_buf->nTickCount = 0;

      err = gst_omx_port_release_buffer (port, buf);
      gst_buffer_replace (&self->codec_data, NULL);
      if (err != OMX_ErrorNone)
        goto release_error;
      /* Acquire new buffer for the actual frame */
      continue;
    }

    /* Now handle the frame */
    GST_DEBUG_OBJECT (self, "Passing frame offset %d to the component", offset);

    /* Copy the buffer content in chunks of size as requested
     * by the port */

    buf->omx_buf->nFilledLen =
        MIN (size - offset, buf->omx_buf->nAllocLen - buf->omx_buf->nOffset);
    gst_buffer_extract (frame->input_buffer, offset,
        buf->omx_buf->pBuffer + buf->omx_buf->nOffset,
        buf->omx_buf->nFilledLen);

#ifdef USE_OMX_DUMP_DATA
      if(self->total_frame_data != NULL){
        if(fwrite(buf->omx_buf->pBuffer + buf->omx_buf->nOffset, 1, buf->omx_buf->nFilledLen, self->total_frame_data) != buf->omx_buf->nFilledLen){
          GST_WARNING("write dec_data failed");
        }
      }
#endif

    /* dump es data */
    gst_omx_video_dump_es(self, buf);


//    buf->omx_buf->nFilledLen =
//        MIN (huge_size - offset, buf->omx_buf->nAllocLen - buf->omx_buf->nOffset);
//
//    GST_DEBUG("huge_size:%d, offset:%d, omx_buf->nOffset:%d, buf->omx_buf->nFilledLen:%d",
//        huge_size, offset,buf->omx_buf->nAllocLen - buf->omx_buf->nOffset, buf->omx_buf->nFilledLen);
//
//    memcpy(buf->omx_buf->pBuffer + buf->omx_buf->nOffset, (guint8 *)hugedata + offset, buf->omx_buf->nFilledLen);



    if (timestamp != GST_CLOCK_TIME_NONE) {
      buf->omx_buf->nTimeStamp =
          gst_util_uint64_scale (timestamp, OMX_TICKS_PER_SECOND, GST_SECOND);
      self->last_upstream_ts = timestamp;
    } else {
      buf->omx_buf->nTimeStamp = -1;
    }

    if (duration != GST_CLOCK_TIME_NONE && offset == 0) {
      buf->omx_buf->nTickCount =
          gst_util_uint64_scale (duration, OMX_TICKS_PER_SECOND, GST_SECOND);
      self->last_upstream_ts += duration;
    } else {
      buf->omx_buf->nTickCount = 0;
    }

    if (offset == 0 && GST_VIDEO_CODEC_FRAME_IS_SYNC_POINT (frame)){
      buf->omx_buf->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;
      GST_DEBUG("key frame!");
    }

    /* TODO: Set flags
     *   - OMX_BUFFERFLAG_DECODEONLY for buffers that are outside
     *     the segment
     */
    GST_DEBUG("buf->omx_buf->nFilledLen: %ld", buf->omx_buf->nFilledLen);
    offset += buf->omx_buf->nFilledLen;

    /*
    if demux send streamer by frame,we need to set OMX_BUFFERFLAG_ENDOFFRAME;
    else if demux send streamer by packet(like DVB),we do not set it
    */
    if(self->is_frame){
      if (offset == size)
        buf->omx_buf->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
      else
        GST_INFO("partial frame");
    }

    err = gst_omx_port_release_buffer (port, buf);
    if (err != OMX_ErrorNone)
      goto release_error;
  }


  /* BEGIN: Added for DTS2015071507912 by liurongliang(l00180035), 2015/7/20 */
  self->input_count++;
  if(!self->is_dec_out && self->input_count >= SME_DEC_DELAY_MAX_COUNT){
    GstBuffer* empty_buffer = gen_decodeonly_gstbuffer();

    gst_pad_push (GST_VIDEO_DECODER_SRC_PAD(self), empty_buffer);

    if(!send_decbuf_and_eos_event(self)){
      GST_WARNING("push eos message failed");
    }
    gst_omx_video_dec_clean_all_frames(decoder);
    GST_ELEMENT_WARNING(decoder, STREAM, DECODE,
                ("input frame is more than %d", SME_DEC_DELAY_MAX_COUNT),
                ("video_dec:v dec not suport!"));
    self->downstream_flow_ret = GST_FLOW_ERROR;
  }else if(self->is_dec_out){
    self->input_count = 0;
  }
  /* END:   Added for DTS2015071507912 by liurongliang(l00180035), 2015/7/20 */

  huge_size = 0;
  GST_DEBUG("input frame number is %d", ++frame_number);
  gst_video_codec_frame_unref (frame);

  GST_DEBUG_OBJECT (self, "Passed frame to component");

  return self->downstream_flow_ret;

full_buffer:
  {
    gst_video_codec_frame_unref (frame);
    GST_ELEMENT_ERROR (self, LIBRARY, FAILED, (NULL),
        ("Got OpenMAX buffer with no free space (%p, %u/%u)", buf,
            (guint) buf->omx_buf->nOffset, (guint) buf->omx_buf->nAllocLen));
    return GST_FLOW_ERROR;
  }

flow_error:
  {
    gst_video_codec_frame_unref (frame);
    GST_ERROR_OBJECT (self, "flow_error");
    return self->downstream_flow_ret;
  }

too_large_codec_data:
  {
    gst_video_codec_frame_unref (frame);
    GST_ELEMENT_ERROR (self, STREAM, FORMAT, (NULL),
        ("codec_data larger than supported by OpenMAX port "
            "(%" G_GSIZE_FORMAT " > %u)", gst_buffer_get_size (codec_data),
            (guint) self->dec_in_port->port_def.nBufferSize));
    return GST_FLOW_ERROR;
  }

component_error:
  {
    gst_video_codec_frame_unref (frame);
    GST_ELEMENT_ERROR (self, LIBRARY, FAILED, (NULL),
        ("OpenMAX component in error state %s (0x%08x)",
            gst_omx_component_get_last_error_string (self->dec),
            gst_omx_component_get_last_error (self->dec)));
    return GST_FLOW_ERROR;
  }

flushing:
  {
    gst_video_codec_frame_unref (frame);
    GST_DEBUG_OBJECT (self, "Flushing -- returning FLUSHING");
    return GST_FLOW_FLUSHING;
  }
reconfigure_error:
  {
    gst_video_codec_frame_unref (frame);
    GST_ELEMENT_ERROR (self, LIBRARY, SETTINGS, (NULL),
        ("Unable to reconfigure input port"));
    return GST_FLOW_ERROR;
  }
release_error:
  {
    gst_video_codec_frame_unref (frame);
    GST_ELEMENT_ERROR (self, LIBRARY, SETTINGS, (NULL),
        ("Failed to relase input buffer to component: %s (0x%08x)",
            gst_omx_error_to_string (err), err));
    return GST_FLOW_ERROR;
  }
}

static GstFlowReturn
gst_omx_video_dec_finish (GstVideoDecoder * decoder)
{
  GstOMXVideoDec *self;

  self = GST_OMX_VIDEO_DEC (decoder);

  return gst_omx_video_dec_drain (self, TRUE);
}

static GstFlowReturn
gst_omx_video_dec_drain (GstOMXVideoDec * self, gboolean is_eos)
{
  GstOMXVideoDecClass *klass;
  GstOMXBuffer *buf;
  GstOMXAcquireBufferReturn acq_ret;
  OMX_ERRORTYPE err;

  GST_WARNING_OBJECT (self, "Draining component in");

  klass = GST_OMX_VIDEO_DEC_GET_CLASS (self);

  if (!self->started) {
    GST_ERROR_OBJECT (self, "Component not started yet");
    return GST_FLOW_OK;
  }
  self->started = FALSE;

  /* Don't send EOS buffer twice, this doesn't work */
  if (self->eos) {
    GST_WARNING_OBJECT (self, "Component is EOS already");
    return GST_FLOW_OK;
  }
  if (is_eos)
    self->eos = TRUE;

  if ((klass->cdata.hacks & GST_OMX_HACK_NO_EMPTY_EOS_BUFFER)) {
    GST_WARNING_OBJECT (self, "Component does not support empty EOS buffers");
    return GST_FLOW_OK;
  }

  /* Make sure to release the base class stream lock, otherwise
   * _loop() can't call _finish_frame() and we might block forever
   * because no input buffers are released */
  GST_INFO_OBJECT (self, "Draining component unlock ...");
  GST_VIDEO_DECODER_STREAM_UNLOCK (self);

  /* Send an EOS buffer to the component and let the base
   * class drop the EOS event. We will send it later when
   * the EOS buffer arrives on the output port. */
  GST_INFO_OBJECT (self, "Draining component acquire buffer ...");
  acq_ret = gst_omx_port_acquire_buffer (self->dec_in_port, &buf);
  GST_INFO_OBJECT (self, "Draining component acquire buffer end(%#x)", acq_ret);
  if (acq_ret != GST_OMX_ACQUIRE_BUFFER_OK) {
    GST_VIDEO_DECODER_STREAM_LOCK (self);
    GST_ERROR_OBJECT (self, "Failed to acquire buffer for draining: %d",
        acq_ret);
    return GST_FLOW_ERROR;
  }

  GST_INFO_OBJECT (self, "Draining component drain_lock ...");
  g_mutex_lock (&self->drain_lock);
  GST_INFO_OBJECT (self, "Draining component drain_lock end");

  /* BEGIN: Added by liurongliang(l00180035), 2015/8/6 */
  /*
    when EOS coming after flush_start a little.
  */
  self->draining = TRUE;
  buf->omx_buf->nFilledLen = 0;
  if(GST_FLOW_FLUSHING == self->downstream_flow_ret){
    gst_omx_port_release_buffer (self->dec_in_port, buf);
    g_mutex_unlock (&self->drain_lock);
    GST_WARNING_OBJECT (self, "omx video flushing now!");

    GST_VIDEO_DECODER_STREAM_LOCK (self);
    self->started = FALSE;

    return GST_FLOW_OK;
  }
  /* END:   Added by liurongliang(l00180035), 2015/8/6 */

  buf->omx_buf->nTimeStamp =
      gst_util_uint64_scale (self->last_upstream_ts, OMX_TICKS_PER_SECOND,
      GST_SECOND);
  buf->omx_buf->nTickCount = 0;
  buf->omx_buf->nFlags |= OMX_BUFFERFLAG_EOS;
  err = gst_omx_port_release_buffer (self->dec_in_port, buf);
  if (err != OMX_ErrorNone) {
    GST_ERROR_OBJECT (self, "Failed to drain component: %s (0x%08x)",
        gst_omx_error_to_string (err), err);
    g_mutex_unlock (&self->drain_lock);
    GST_VIDEO_DECODER_STREAM_LOCK (self);
    return GST_FLOW_ERROR;
  }

  GST_WARNING_OBJECT (self, "Waiting until component is drained");

  if (G_UNLIKELY (self->dec->hacks & GST_OMX_HACK_DRAIN_MAY_NOT_RETURN)) {
    gint64 wait_until = g_get_monotonic_time () + G_TIME_SPAN_SECOND / 2;

    if (!g_cond_wait_until (&self->drain_cond, &self->drain_lock, wait_until))
      GST_WARNING_OBJECT (self, "Drain timed out");
    else
      GST_WARNING_OBJECT (self, "Drained component");

  } else {
    g_cond_wait (&self->drain_cond, &self->drain_lock);
    GST_WARNING_OBJECT (self, "Drained component");
  }

  g_mutex_unlock (&self->drain_lock);
  GST_VIDEO_DECODER_STREAM_LOCK (self);

  self->started = FALSE;

  GST_WARNING_OBJECT (self, "Drained component out");

  return GST_FLOW_OK;
}

static gboolean
gst_omx_video_dec_decide_allocation (GstVideoDecoder * bdec, GstQuery * query)
{
  GstBufferPool *pool;
  GstStructure *config;

#if defined (USE_OMX_TARGET_RPI) && defined (HAVE_GST_GL)
  {
    GstCaps *caps;
    gint i, n;
    GstVideoInfo info;

    gst_query_parse_allocation (query, &caps, NULL);
    if (caps && gst_video_info_from_caps (&info, caps)
        && info.finfo->format == GST_VIDEO_FORMAT_RGBA) {
      gboolean found = FALSE;
      GstCapsFeatures *feature = gst_caps_get_features (caps, 0);
      /* Prefer an EGLImage allocator if available and we want to use it */
      n = gst_query_get_n_allocation_params (query);
      for (i = 0; i < n; i++) {
        GstAllocator *allocator;
        GstAllocationParams params;

        gst_query_parse_nth_allocation_param (query, i, &allocator, &params);
        if (allocator
            && g_strcmp0 (allocator->mem_type,
                GST_EGL_IMAGE_MEMORY_TYPE) == 0) {
          found = TRUE;
          gst_query_set_nth_allocation_param (query, 0, allocator, &params);
          while (gst_query_get_n_allocation_params (query) > 1)
            gst_query_remove_nth_allocation_param (query, 1);
          break;
        }
      }

      /* if try to negotiate with caps feature memory:EGLImage
       * and if allocator is not of type memory EGLImage then fails */
      if (feature
          && gst_caps_features_contains (feature,
              GST_CAPS_FEATURE_MEMORY_EGL_IMAGE) && !found) {
        return FALSE;
      }
    }
  }
#endif

  if (!GST_VIDEO_DECODER_CLASS
      (gst_omx_video_dec_parent_class)->decide_allocation (bdec, query))
    return FALSE;

  g_assert (gst_query_get_n_allocation_pools (query) > 0);
  gst_query_parse_nth_allocation_pool (query, 0, &pool, NULL, NULL, NULL);
  g_assert (pool != NULL);

  config = gst_buffer_pool_get_config (pool);
  GST_INFO("query, GST_VIDEO_META_API_TYPE");
  if (gst_query_find_allocation_meta (query, GST_VIDEO_META_API_TYPE, NULL)) {
    GST_INFO("query, add GST_VIDEO_META_API_TYPE");
    gst_buffer_pool_config_add_option (config,
        GST_BUFFER_POOL_OPTION_VIDEO_META);
  }

  /* BEGIN: Added by liurongliang(l00180035), 2015/3/6 */
  /*for hisi we force add GST_BUFFER_POOL_OPTION_VIDEO_META*/
  gst_buffer_pool_config_add_option (config,
      GST_BUFFER_POOL_OPTION_VIDEO_META);
  /* END:   Added by liurongliang(l00180035), 2015/3/6 */

  gst_buffer_pool_set_config (pool, config);
  gst_object_unref (pool);

  return TRUE;
}
