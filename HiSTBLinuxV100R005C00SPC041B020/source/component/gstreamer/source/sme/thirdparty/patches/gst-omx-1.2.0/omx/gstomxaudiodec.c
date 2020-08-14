/*
 * Copyright (C) 2011, Hewlett-Packard Development Company, L.P.
 *   Author: Sebastian Dröge <sebastian.droege@collabora.co.uk>, Collabora Ltd.
 * Copyright (C) 2013, Collabora Ltd.
 *   Author: Sebastian Dröge <sebastian.droege@collabora.co.uk>
 * Copyright (C) 2014, Sebastian Dröge <sebastian@centricular.com>
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

#include <string.h>

#include "gstomxaudiodec.h"

//#define SME_OMX_ADEC_DUMP_IN
//#define SME_OMX_ADEC_DUMP_OUT

#if defined (SME_OMX_ADEC_DUMP_IN) || defined (SME_OMX_ADEC_DUMP_OUT)
#include <stdio.h>
#endif

#if defined (SME_OMX_ADEC_DUMP_IN)
static FILE *in_file = NULL;
#endif

#if defined (SME_OMX_ADEC_DUMP_OUT)
static FILE *out_file = NULL;
#endif

GST_DEBUG_CATEGORY_STATIC (gst_omx_audio_dec_debug_category);
#define GST_CAT_DEFAULT gst_omx_audio_dec_debug_category

#ifndef SMEPLAYER_STAT_TAG_TIME
#define SMEPLAYER_STAT_TAG_TIME "PlayerTimeLineTrace"
#endif

/* prototypes */
static void gst_omx_audio_dec_finalize (GObject * object);

static GstStateChangeReturn
gst_omx_audio_dec_change_state (GstElement * element,
    GstStateChange transition);

static gboolean gst_omx_audio_dec_open (GstAudioDecoder * decoder);
static gboolean gst_omx_audio_dec_close (GstAudioDecoder * decoder);
static gboolean gst_omx_audio_dec_start (GstAudioDecoder * decoder);
static gboolean gst_omx_audio_dec_stop (GstAudioDecoder * decoder);
static gboolean gst_omx_audio_dec_set_format (GstAudioDecoder * decoder,
    GstCaps * caps);
static void gst_omx_audio_dec_flush (GstAudioDecoder * decoder, gboolean hard);
static GstFlowReturn gst_omx_audio_dec_handle_frame (GstAudioDecoder * decoder,
    GstBuffer * buffer);
static GstFlowReturn gst_omx_audio_dec_drain (GstOMXAudioDec * self,
    gboolean is_eos);

#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
static guint gst_omx_audio_dec_bitstream_get_pad(GstOMXAudioDec *self);
static guint gst_omx_audio_dec_bitstream_handle_buffer(GstOMXAudioDec *self,
                                                       GstOMXBuffer *buf,
                                                       guint pcm_len);
static guint gst_omx_audio_dec_bitstream_handle_event(GstOMXAudioDec *self,
                                                      GstEvent *event);
static void gst_omx_audio_dec_bitstream_src_loop(GstOMXAudioDec * self);
static void gst_omx_audio_dec_bitstream_queue_clean(GstOMXAudioDec *self,
                                                    gboolean stream_lock);
static gboolean gst_omx_audio_dec_bitstream_push_data(GstOMXAudioDec *self,
                                                      GstBuffer *gstbuffer);
static gboolean gst_omx_audio_dec_bitstream_push_event(GstOMXAudioDec *self,
                                                       GstEvent *event);
static void _data_bitstream_queue_item_destroy (GstDataQueueItem * item);
static gboolean _check_bitstream_queue_full (GstDataQueue * q,
                                             guint visible,
                                             guint bytes,
                                             guint64 total_time,
                                             gpointer checkdata);
#endif

static gboolean
gst_omx_audio_dec_sink_eventfunc (GstAudioDecoder * dec, GstEvent * event);
#ifdef GST_DDP_PASSTHROUGH_ONLY_SUPPORT
static gboolean
gst_omx_audio_dec_src_eventfunc (GstAudioDecoder * dec, GstEvent * event);
#endif

#define SME_DEC_DELAY_MAX_COUNT 100 //if input more than 100 frame and not dec out,we think dec not support.

enum
{
  PROP_0
};

/* class initialization */

#define DEBUG_INIT \
  GST_DEBUG_CATEGORY_INIT (gst_omx_audio_dec_debug_category, "omxaudiodec", 0, \
      "debug category for gst-omx audio decoder base class");


G_DEFINE_ABSTRACT_TYPE_WITH_CODE (GstOMXAudioDec, gst_omx_audio_dec,
    GST_TYPE_AUDIO_DECODER, DEBUG_INIT);

static void
gst_omx_audio_dec_class_init (GstOMXAudioDecClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
  GstAudioDecoderClass *audio_decoder_class = GST_AUDIO_DECODER_CLASS (klass);

  gobject_class->finalize = gst_omx_audio_dec_finalize;

  element_class->change_state =
      GST_DEBUG_FUNCPTR (gst_omx_audio_dec_change_state);

  audio_decoder_class->open = GST_DEBUG_FUNCPTR (gst_omx_audio_dec_open);
  audio_decoder_class->close = GST_DEBUG_FUNCPTR (gst_omx_audio_dec_close);
  audio_decoder_class->start = GST_DEBUG_FUNCPTR (gst_omx_audio_dec_start);
  audio_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_omx_audio_dec_stop);
  audio_decoder_class->flush = GST_DEBUG_FUNCPTR (gst_omx_audio_dec_flush);
  audio_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_omx_audio_dec_set_format);
  audio_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_omx_audio_dec_handle_frame);
  audio_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_omx_audio_dec_sink_eventfunc);
#ifdef GST_DDP_PASSTHROUGH_ONLY_SUPPORT
  audio_decoder_class->src_event =
      GST_DEBUG_FUNCPTR (gst_omx_audio_dec_src_eventfunc);
#endif

  klass->cdata.type = GST_OMX_COMPONENT_TYPE_FILTER;
  klass->cdata.default_src_template_caps =
      "audio/x-raw, "
      "rate = (int) [ 1, MAX ], "
      "channels = (int) [ 1, " G_STRINGIFY (OMX_AUDIO_MAXCHANNELS) " ], "
      "format = (string) " GST_AUDIO_FORMATS_ALL;
}

static void
gst_omx_audio_dec_init (GstOMXAudioDec * self)
{
  gst_audio_decoder_set_needs_format (GST_AUDIO_DECODER (self), TRUE);
  gst_audio_decoder_set_drainable (GST_AUDIO_DECODER (self), TRUE);

#if defined (SME_OMX_ADEC_DUMP_IN)
  in_file = fopen("/mnt/dump/adec_in.data", "wb");
  if (in_file == NULL)
  {
      GST_ERROR("fail to open file /mnt/dump/adec_in.data");
  }
#endif

#if defined (SME_OMX_ADEC_DUMP_OUT)
  out_file = fopen("/mnt/dump/adec_out.data", "wb");
  if (out_file == NULL)
  {
      GST_ERROR("fail to open file /mnt/dump/adec_out.data");
  }
#endif

  g_mutex_init (&self->drain_lock);
  g_mutex_init (&self->flush_lock);
  g_cond_init (&self->drain_cond);

#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
//  self->need_segment = TRUE;
#endif
}

static gboolean
gst_omx_audio_dec_open (GstAudioDecoder * decoder)
{
  GstOMXAudioDec *self = GST_OMX_AUDIO_DEC (decoder);
  GstOMXAudioDecClass *klass = GST_OMX_AUDIO_DEC_GET_CLASS (self);
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
        gst_omx_component_get_parameter (self->dec, OMX_IndexParamAudioInit,
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

  return TRUE;
}

static gboolean
gst_omx_audio_dec_shutdown (GstOMXAudioDec * self)
{
  OMX_STATETYPE state;

  GST_INFO_OBJECT (self, "Shutting down decoder");

  state = gst_omx_component_get_state (self->dec, 0);
  if (state > OMX_StateLoaded || state == OMX_StateInvalid) {
    if (state > OMX_StateIdle) {
      gst_omx_component_set_state (self->dec, OMX_StateIdle);
      gst_omx_component_get_state (self->dec, 5 * GST_SECOND);
    }
    gst_omx_component_set_state (self->dec, OMX_StateLoaded);
    gst_omx_port_deallocate_buffers (self->dec_in_port);
    gst_omx_port_deallocate_buffers (self->dec_out_port);
    if (state > OMX_StateLoaded)
      gst_omx_component_get_state (self->dec, 5 * GST_SECOND);
  }

  return TRUE;
}

static gboolean
gst_omx_audio_dec_close (GstAudioDecoder * decoder)
{
  GstOMXAudioDec *self = GST_OMX_AUDIO_DEC (decoder);

  GST_DEBUG_OBJECT (self, "Closing decoder");

  if (!gst_omx_audio_dec_shutdown (self))
    return FALSE;

  self->dec_in_port = NULL;
  self->dec_out_port = NULL;
  if (self->dec)
    gst_omx_component_free (self->dec);
  self->dec = NULL;

  self->started = FALSE;

  GST_DEBUG_OBJECT (self, "Closed decoder");

  return TRUE;
}

static void
gst_omx_audio_dec_finalize (GObject * object)
{
  GstOMXAudioDec *self = GST_OMX_AUDIO_DEC (object);

  g_mutex_clear (&self->drain_lock);
  g_mutex_clear (&self->flush_lock);
  g_cond_clear (&self->drain_cond);

#if defined (SME_OMX_ADEC_DUMP_IN)
  if (in_file != NULL)
  {
      fclose(in_file);
  }
#endif

#if defined (SME_OMX_ADEC_DUMP_OUT)
  if (out_file != NULL)
  {
      fclose(out_file);
  }
#endif

  G_OBJECT_CLASS (gst_omx_audio_dec_parent_class)->finalize (object);
}

static GstStateChangeReturn
gst_omx_audio_dec_change_state (GstElement * element, GstStateChange transition)
{
  GstOMXAudioDec *self;
  GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;

  g_return_val_if_fail (GST_IS_OMX_AUDIO_DEC (element),
      GST_STATE_CHANGE_FAILURE);
  self = GST_OMX_AUDIO_DEC (element);

  switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
      break;
    case GST_STATE_CHANGE_READY_TO_PAUSED:
      self->downstream_flow_ret = GST_FLOW_OK;
      self->draining = FALSE;
      self->started = FALSE;
      self->input_count = 0;
      break;
    case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
      break;
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      if (self->dec_in_port)
        gst_omx_port_set_flushing (self->dec_in_port, 5 * GST_SECOND, TRUE);
      if (self->dec_out_port)
        gst_omx_port_set_flushing (self->dec_out_port, 5 * GST_SECOND, TRUE);

      g_mutex_lock (&self->drain_lock);
      self->draining = FALSE;
      g_cond_broadcast (&self->drain_cond);
      g_mutex_unlock (&self->drain_lock);

#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
      gst_omx_audio_dec_bitstream_queue_clean(self, FALSE);
#endif
      break;
    default:
      break;
  }

  if (ret == GST_STATE_CHANGE_FAILURE)
    return ret;

  ret =
      GST_ELEMENT_CLASS (gst_omx_audio_dec_parent_class)->change_state
      (element, transition);

  if (ret == GST_STATE_CHANGE_FAILURE)
    return ret;

  switch (transition) {
    case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
      break;
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      self->downstream_flow_ret = GST_FLOW_FLUSHING;
      self->started = FALSE;

      if (!gst_omx_audio_dec_shutdown (self))
        ret = GST_STATE_CHANGE_FAILURE;
      break;
    case GST_STATE_CHANGE_READY_TO_NULL:
      break;
    default:
      break;
  }

  return ret;
}

static void
gst_omx_audio_dec_loop (GstOMXAudioDec * self)
{
  GstOMXPort *port = self->dec_out_port;
  GstOMXBuffer *buf = NULL;
  GstFlowReturn flow_ret = GST_FLOW_OK;
  GstOMXAcquireBufferReturn acq_return;
  OMX_ERRORTYPE err;

  GST_DEBUG("gst_omx_audio_dec_loop acquire begin");
  acq_return = gst_omx_port_acquire_buffer (port, &buf);
  GST_DEBUG("gst_omx_audio_dec_loop acquire end acq_return=%d", acq_return);
  if (acq_return == GST_OMX_ACQUIRE_BUFFER_ERROR) {
    goto component_error;
  } else if (acq_return == GST_OMX_ACQUIRE_BUFFER_FLUSHING) {
    goto flushing;
  } else if (acq_return == GST_OMX_ACQUIRE_BUFFER_EOS) {
    goto eos;
  }

  if (!gst_pad_has_current_caps (GST_AUDIO_DECODER_SRC_PAD (self)) ||
      acq_return == GST_OMX_ACQUIRE_BUFFER_RECONFIGURE) {
    OMX_PARAM_PORTDEFINITIONTYPE port_def;
    OMX_AUDIO_PARAM_PCMMODETYPE pcm_param;
    GstAudioChannelPosition omx_position[OMX_AUDIO_MAXCHANNELS];
    GstOMXAudioDecClass *klass = GST_OMX_AUDIO_DEC_GET_CLASS (self);
    gint i;

    GST_INFO_OBJECT (self, "Port settings have changed, updating caps,"
        " acq_return %d", acq_return);

    GST_INFO_OBJECT (self, "before locked");
    /* Just update caps */
    GST_AUDIO_DECODER_STREAM_LOCK (self);
    GST_INFO_OBJECT (self, "after locked");

    /* Reallocate all buffers */
    if (acq_return == GST_OMX_ACQUIRE_BUFFER_RECONFIGURE
        && gst_omx_port_is_enabled (port)) {
      err = gst_omx_port_set_enabled (port, FALSE);
      if (err != OMX_ErrorNone) {
        GST_AUDIO_DECODER_STREAM_UNLOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_wait_buffers_released (port, 5 * GST_SECOND);
      if (err != OMX_ErrorNone) {
        GST_AUDIO_DECODER_STREAM_UNLOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_deallocate_buffers (port);
      if (err != OMX_ErrorNone) {
        GST_AUDIO_DECODER_STREAM_UNLOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_wait_enabled (port, 1 * GST_SECOND);
      if (err != OMX_ErrorNone) {
        GST_AUDIO_DECODER_STREAM_UNLOCK (self);
        goto reconfigure_error;
      }
    }

    gst_omx_port_get_port_definition (port, &port_def);
    g_assert (port_def.format.audio.eEncoding == OMX_AUDIO_CodingPCM);

    GST_OMX_INIT_STRUCT (&pcm_param);
    pcm_param.nPortIndex = self->dec_out_port->index;
    err =
        gst_omx_component_get_parameter (self->dec, OMX_IndexParamAudioPcm,
        &pcm_param);
    if (err != OMX_ErrorNone) {
      GST_ERROR_OBJECT (self, "Failed to get PCM parameters: %s (0x%08x)",
          gst_omx_error_to_string (err), err);
      goto caps_failed;
    }

    g_assert (pcm_param.ePCMMode == OMX_AUDIO_PCMModeLinear);
    g_assert (pcm_param.bInterleaved == OMX_TRUE);

    gst_audio_info_init (&self->info);

    for (i = 0; i < pcm_param.nChannels; i++) {
      switch (pcm_param.eChannelMapping[i]) {
        case OMX_AUDIO_ChannelLF:
          omx_position[i] = GST_AUDIO_CHANNEL_POSITION_FRONT_LEFT;
          break;
        case OMX_AUDIO_ChannelRF:
          omx_position[i] = GST_AUDIO_CHANNEL_POSITION_FRONT_RIGHT;
          break;
        case OMX_AUDIO_ChannelCF:
          omx_position[i] = GST_AUDIO_CHANNEL_POSITION_FRONT_CENTER;
          break;
        case OMX_AUDIO_ChannelLS:
          omx_position[i] = GST_AUDIO_CHANNEL_POSITION_SIDE_LEFT;
          break;
        case OMX_AUDIO_ChannelRS:
          omx_position[i] = GST_AUDIO_CHANNEL_POSITION_SIDE_RIGHT;
          break;
        case OMX_AUDIO_ChannelLFE:
          omx_position[i] = GST_AUDIO_CHANNEL_POSITION_LFE1;
          break;
        case OMX_AUDIO_ChannelCS:
          omx_position[i] = GST_AUDIO_CHANNEL_POSITION_REAR_CENTER;
          break;
        case OMX_AUDIO_ChannelLR:
          omx_position[i] = GST_AUDIO_CHANNEL_POSITION_REAR_LEFT;
          break;
        case OMX_AUDIO_ChannelRR:
          omx_position[i] = GST_AUDIO_CHANNEL_POSITION_REAR_RIGHT;
          break;
        case OMX_AUDIO_ChannelNone:
        default:
          /* This will break the outer loop too as the
           * i == pcm_param.nChannels afterwards */
          for (i = 0; i < pcm_param.nChannels; i++)
            omx_position[i] = GST_AUDIO_CHANNEL_POSITION_NONE;
          break;
      }
    }
    if (pcm_param.nChannels == 1
        && omx_position[0] == GST_AUDIO_CHANNEL_POSITION_FRONT_CENTER)
      omx_position[0] = GST_AUDIO_CHANNEL_POSITION_MONO;

    if (omx_position[0] == GST_AUDIO_CHANNEL_POSITION_NONE
        && klass->get_channel_positions) {
      GST_WARNING_OBJECT (self,
          "Failed to get a valid channel layout, trying fallback");
      klass->get_channel_positions (self, self->dec_out_port, omx_position);
    }

    memcpy (self->position, omx_position, sizeof (omx_position));
    gst_audio_channel_positions_to_valid_order (self->position,
        pcm_param.nChannels);
    self->needs_reorder =
        (memcmp (self->position, omx_position,
            sizeof (GstAudioChannelPosition) * pcm_param.nChannels) != 0);
    if (self->needs_reorder)
      gst_audio_get_channel_reorder_map (pcm_param.nChannels, self->position,
          omx_position, self->reorder_map);

    gst_audio_info_set_format (&self->info,
        gst_audio_format_build_integer (pcm_param.eNumData ==
            OMX_NumericalDataSigned,
            /*pcm_param.eEndian ==
            OMX_EndianLittle ? G_LITTLE_ENDIAN : G_BIG_ENDIAN,*/
            G_LITTLE_ENDIAN,
            pcm_param.nBitPerSample, pcm_param.nBitPerSample),
        pcm_param.nSamplingRate, pcm_param.nChannels, self->position);

    GST_INFO_OBJECT (self,
        "Setting output state: format %s, rate %u, channels %u",
        gst_audio_format_to_string (self->info.finfo->format),
        (guint) pcm_param.nSamplingRate, (guint) pcm_param.nChannels);

#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
    /* create the bitstream pad before pcm src pad push anything to peer */
    gst_omx_audio_dec_bitstream_get_pad(self);
    /* push a null buffer to release the probe in playsink */
    gst_omx_audio_dec_bitstream_handle_buffer(self, NULL, 0);
#endif

    if (!gst_audio_decoder_set_output_format (GST_AUDIO_DECODER (self),
            &self->info)
        || !gst_audio_decoder_negotiate (GST_AUDIO_DECODER (self))) {
      if (buf)
        gst_omx_port_release_buffer (port, buf);
      if (G_UNLIKELY (GST_PAD_IS_FLUSHING (GST_AUDIO_DECODER_SRC_PAD (self)))) {
        GST_WARNING_OBJECT(self,
            "set caps fail due to flushing, don't treate is as error");
      } else {
        goto caps_failed;
      }
    }

    GST_AUDIO_DECODER_STREAM_UNLOCK (self);

    if (acq_return == GST_OMX_ACQUIRE_BUFFER_RECONFIGURE) {
      err = gst_omx_port_set_enabled (port, TRUE);
      if (err != OMX_ErrorNone)
        goto reconfigure_error;

      err = gst_omx_port_allocate_buffers (port);
      if (err != OMX_ErrorNone)
        goto reconfigure_error;

      err = gst_omx_port_wait_enabled (port, 5 * GST_SECOND);
      if (err != OMX_ErrorNone)
        goto reconfigure_error;

      err = gst_omx_port_populate (port);
      if (err != OMX_ErrorNone){
        if (port->flushing) {
          /* if port is in flushing state, don't treate it as error */
          GST_WARNING_OBJECT(self,
              "populate fail due to flushing, don't treate is as error");
        } else {
          goto reconfigure_error;
        }
      }

      err = gst_omx_port_mark_reconfigured (port);
      if (err != OMX_ErrorNone)
        goto reconfigure_error;
    }

    /* Now get a buffer */
    if (acq_return != GST_OMX_ACQUIRE_BUFFER_OK) {
      return;
    }
  }

  g_assert (acq_return == GST_OMX_ACQUIRE_BUFFER_OK);
  if (!buf) {
    g_assert ((klass->cdata.hacks & GST_OMX_HACK_NO_EMPTY_EOS_BUFFER));
    goto eos;
  }

  /* This prevents a deadlock between the srcpad stream
   * lock and the audiocodec stream lock, if ::reset()
   * is called at the wrong time
   */
  if (gst_omx_port_is_flushing (port)) {
    GST_DEBUG_OBJECT (self, "Flushing");
    gst_omx_port_release_buffer (port, buf);
    goto flushing;
  }

  GST_DEBUG_OBJECT (self, "Handling buffer: 0x%08x %" G_GUINT64_FORMAT", len=%d",
      (guint) buf->omx_buf->nFlags, (guint64) buf->omx_buf->nTimeStamp,
      buf->omx_buf->nFilledLen);

  GST_AUDIO_DECODER_STREAM_LOCK (self);

  if ((gint32)(buf->omx_buf->nFilledLen) > 0) {
    GstBuffer *outbuf;
    guint pcm_len = 0;
    gint nframes, spf;
    GstMapInfo minfo;
    GstOMXAudioDecClass *klass = GST_OMX_AUDIO_DEC_GET_CLASS (self);

    GST_DEBUG_OBJECT (self, "Handling output data");

    self->input_count = 0;

    pcm_len = buf->omx_buf->nFilledLen;

    if (klass->get_pcm_data_size) {
      pcm_len = klass->get_pcm_data_size(self, port, buf);
    }

    if (pcm_len % self->info.bpf != 0) {
      gst_omx_port_release_buffer (port, buf);
      goto invalid_buffer;
    }

#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
    gst_omx_audio_dec_bitstream_handle_buffer(self, buf, pcm_len);
#endif

    outbuf =
        gst_audio_decoder_allocate_output_buffer (GST_AUDIO_DECODER (self),
        pcm_len);

    gst_buffer_map (outbuf, &minfo, GST_MAP_WRITE);
    if (self->needs_reorder) {
      gint i, n_samples, c, n_channels;
      gint *reorder_map = self->reorder_map;
      gint16 *dest, *source;

      dest = (gint16 *) minfo.data;
      source = (gint16 *) (buf->omx_buf->pBuffer + buf->omx_buf->nOffset);
      n_samples = pcm_len / self->info.bpf;
      n_channels = self->info.channels;

      for (i = 0; i < n_samples; i++) {
        for (c = 0; c < n_channels; c++) {
          dest[i * n_channels + reorder_map[c]] = source[i * n_channels + c];
        }
      }
    } else {
      memcpy (minfo.data, buf->omx_buf->pBuffer + buf->omx_buf->nOffset,
          pcm_len);
    }

#if defined (SME_OMX_ADEC_DUMP_OUT)
    if (out_file)
    {
        fwrite(minfo.data, 1, pcm_len, out_file);
        GST_DEBUG("write %d bytes data", pcm_len);
    }
#endif

    gst_buffer_unmap (outbuf, &minfo);

    nframes = 1;
    spf = klass->get_samples_per_frame (self, self->dec_out_port);
    if (spf != -1) {
      nframes = pcm_len / self->info.bpf;
      if (nframes % spf != 0)
        GST_WARNING_OBJECT (self, "Output buffer does not contain an integer "
            "number of input frames (frames: %d, spf: %d)", nframes, spf);
      nframes = (nframes + spf - 1) / spf;
    }
    GST_DEBUG_OBJECT(self, "Handling buffer:nframes=%d", nframes);
#ifdef COMPUTE_OUT_PTS
    if (buf->omx_buf->nTickCount != 0)
      GST_BUFFER_DURATION (outbuf) =
          gst_util_uint64_scale (buf->omx_buf->nTickCount, GST_SECOND,
          OMX_TICKS_PER_SECOND);
    else {
      guint sample_num = pcm_len / self->info.bpf;
      GST_BUFFER_DURATION (outbuf) =
          gst_util_uint64_scale (sample_num, GST_SECOND,
                                 self->info.rate);

    }
    if(GST_CLOCK_TIME_NONE == self->last_out_pts){
      GST_BUFFER_DTS (outbuf) =
          GST_BUFFER_TIMESTAMP (outbuf) =
              gst_util_uint64_scale (buf->omx_buf->nTimeStamp, GST_SECOND,
              OMX_TICKS_PER_SECOND);
    } else {
      GST_BUFFER_DTS (outbuf) =
          GST_BUFFER_TIMESTAMP (outbuf) = self->last_out_pts;
      self->last_out_pts += GST_BUFFER_DURATION (outbuf);
    }
#else
    GST_BUFFER_DTS (outbuf) =
    GST_BUFFER_TIMESTAMP (outbuf) =
        gst_util_uint64_scale (buf->omx_buf->nTimeStamp, GST_SECOND,
        OMX_TICKS_PER_SECOND);
    if (buf->omx_buf->nTickCount != 0)
      GST_BUFFER_DURATION (outbuf) =
          gst_util_uint64_scale (buf->omx_buf->nTickCount, GST_SECOND,
          OMX_TICKS_PER_SECOND);
    else {
      guint sample_num = pcm_len / self->info.bpf;
      GST_BUFFER_DURATION (outbuf) =
          gst_util_uint64_scale (sample_num, GST_SECOND,
                                 self->info.rate);
    }
#endif

    GST_DEBUG_OBJECT (self, "audio decout buffer:pts=%lld,size=%d",
      GST_BUFFER_PTS(outbuf),
      gst_buffer_get_size(outbuf));

    flow_ret =
        gst_audio_decoder_finish_frame (GST_AUDIO_DECODER (self), outbuf,
        nframes);
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

  GST_AUDIO_DECODER_STREAM_UNLOCK (self);

  return;

component_error:
  {
    GST_ELEMENT_ERROR (self, LIBRARY, FAILED, (NULL),
        ("OpenMAX component in error state %s (0x%08x)",
            gst_omx_component_get_last_error_string (self->dec),
            gst_omx_component_get_last_error (self->dec)));
    gst_pad_push_event (GST_AUDIO_DECODER_SRC_PAD (self), gst_event_new_eos ());
    gst_pad_pause_task (GST_AUDIO_DECODER_SRC_PAD (self));

#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
    gst_omx_audio_dec_bitstream_queue_clean(self, FALSE);
#endif

    self->downstream_flow_ret = GST_FLOW_ERROR;
    self->started = FALSE;
    return;
  }

flushing:
  {
    GST_WARNING_OBJECT (self, "Flushing -- stopping task");
    gst_pad_pause_task (GST_AUDIO_DECODER_SRC_PAD (self));

#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
    gst_omx_audio_dec_bitstream_queue_clean(self, FALSE);
#endif

    /* BEGIN: Modified by liurongliang(l00180035), 2015/8/7 */
    /*
       fix issue:when EOS coming, here draining, then wait omx dec out finish.
       then seek occur,flush start, then gst_pad_push(dec out loop) return flush.
       the draining will block.
    */
    g_mutex_lock (&self->drain_lock);
    self->downstream_flow_ret = GST_FLOW_FLUSHING;
    self->started = FALSE;
    if (self->draining) {
      GST_WARNING_OBJECT (self, "Drained");
      self->draining = FALSE;
      g_cond_broadcast (&self->drain_cond);
    }
    g_mutex_unlock (&self->drain_lock);
    /* END:   Modified by liurongliang(l00180035), 2015/8/7 */

    return;
  }

eos:
  {
    g_mutex_lock (&self->drain_lock);
    if (self->draining) {
      GST_INFO_OBJECT (self, "Drained");
      self->draining = FALSE;
      g_cond_broadcast (&self->drain_cond);
      flow_ret = GST_FLOW_OK;
      gst_pad_pause_task (GST_AUDIO_DECODER_SRC_PAD (self));
#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
      gst_omx_audio_dec_bitstream_queue_clean(self, FALSE);
#endif
    } else {
      GST_INFO_OBJECT (self, "Component signalled EOS");
      flow_ret = GST_FLOW_EOS;
    }
    g_mutex_unlock (&self->drain_lock);

    GST_AUDIO_DECODER_STREAM_LOCK (self);
    self->downstream_flow_ret = flow_ret;

    /* Here we fallback and pause the task for the EOS case */
    if (flow_ret != GST_FLOW_OK)
      goto flow_error;

    GST_AUDIO_DECODER_STREAM_UNLOCK (self);

    return;
  }

flow_error:
  {
    if (flow_ret == GST_FLOW_EOS) {
      GST_WARNING_OBJECT (self, "EOS");

      gst_pad_push_event (GST_AUDIO_DECODER_SRC_PAD (self),
          gst_event_new_eos ());
      gst_pad_pause_task (GST_AUDIO_DECODER_SRC_PAD (self));
      self->started = FALSE;
      /* push eos and let src-loop to quit */
#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
      gst_omx_audio_dec_bitstream_handle_event (self,
          gst_event_new_eos());
#endif
    } else if (flow_ret < GST_FLOW_EOS) {
      /*we just post audiodec error warning*/
      GST_ELEMENT_WARNING(self, STREAM, DECODE,
        ("Internal data stream error."),
        ("audio_dec:stream stopped, reason %s", gst_flow_get_name (flow_ret)));

      gst_pad_push_event (GST_AUDIO_DECODER_SRC_PAD (self),
          gst_event_new_eos ());
      gst_pad_pause_task (GST_AUDIO_DECODER_SRC_PAD (self));
      self->started = FALSE;
      /* push eos and let src-loop to quit */
#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
      gst_omx_audio_dec_bitstream_handle_event (self,
          gst_event_new_eos());
#endif
    } else if (flow_ret == GST_FLOW_FLUSHING) {
      GST_WARNING_OBJECT (self, "Flushing -- stopping task");
      gst_pad_pause_task (GST_AUDIO_DECODER_SRC_PAD (self));

      /*we set all out buf to OMX-IL after flush start
        for DTS2015082004245
      */
      gst_omx_port_populate (port);

      /* BEGIN: Modified by liurongliang(l00180035), 2015/8/7 */
      /*
         fix issue:when EOS coming, here draining, then wait omx dec out finish.
         then seek occur,flush start, then gst_pad_push(dec out loop) return flush.
         the draining will block.
      */
      g_mutex_lock (&self->drain_lock);
      self->downstream_flow_ret = GST_FLOW_FLUSHING;
      self->started = FALSE;
      if (self->draining) {
        GST_WARNING_OBJECT (self, "Drained");
        self->draining = FALSE;
        g_cond_broadcast (&self->drain_cond);
      }
      g_mutex_unlock (&self->drain_lock);
      /* END:   Modified by liurongliang(l00180035), 2015/8/7 */

      /* flush bitstream data too */
#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
      gst_omx_audio_dec_bitstream_queue_clean(self, TRUE);
#endif
    }
    GST_AUDIO_DECODER_STREAM_UNLOCK (self);
    return;
  }

reconfigure_error:
  {
    GST_ELEMENT_ERROR (self, LIBRARY, SETTINGS, (NULL),
        ("Unable to reconfigure output port"));
    gst_pad_push_event (GST_AUDIO_DECODER_SRC_PAD (self), gst_event_new_eos ());
    gst_pad_pause_task (GST_AUDIO_DECODER_SRC_PAD (self));
    self->downstream_flow_ret = GST_FLOW_ERROR;
    self->started = FALSE;
    /* flush bitstream data too */
#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
    gst_omx_audio_dec_bitstream_queue_clean(self, FALSE);
#endif
    return;
  }

invalid_buffer:
  {
    GST_ELEMENT_ERROR (self, LIBRARY, SETTINGS, (NULL),
        ("Invalid sized input buffer"));
    gst_pad_push_event (GST_AUDIO_DECODER_SRC_PAD (self), gst_event_new_eos ());
    gst_pad_pause_task (GST_AUDIO_DECODER_SRC_PAD (self));
    self->downstream_flow_ret = GST_FLOW_NOT_NEGOTIATED;
    self->started = FALSE;
    /* flush bitstream data too */
#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
    gst_omx_audio_dec_bitstream_queue_clean(self, TRUE);
#endif
    GST_AUDIO_DECODER_STREAM_UNLOCK (self);
    return;
  }

caps_failed:
  {
    GST_ELEMENT_ERROR (self, LIBRARY, SETTINGS, (NULL), ("Failed to set caps"));
    gst_pad_push_event (GST_AUDIO_DECODER_SRC_PAD (self), gst_event_new_eos ());
    gst_pad_pause_task (GST_AUDIO_DECODER_SRC_PAD (self));
    /* flush bitstream data too */
#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
    gst_omx_audio_dec_bitstream_queue_clean(self, TRUE);
#endif
    GST_AUDIO_DECODER_STREAM_UNLOCK (self);
    self->downstream_flow_ret = GST_FLOW_NOT_NEGOTIATED;
    self->started = FALSE;
    return;
  }
release_error:
  {
    GST_ELEMENT_ERROR (self, LIBRARY, SETTINGS, (NULL),
        ("Failed to relase output buffer to component: %s (0x%08x)",
            gst_omx_error_to_string (err), err));
    gst_pad_push_event (GST_AUDIO_DECODER_SRC_PAD (self), gst_event_new_eos ());
    gst_pad_pause_task (GST_AUDIO_DECODER_SRC_PAD (self));
    self->downstream_flow_ret = GST_FLOW_ERROR;
    self->started = FALSE;
    /* flush bitstream data too */
#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
    gst_omx_audio_dec_bitstream_queue_clean(self, TRUE);
#endif
    GST_AUDIO_DECODER_STREAM_UNLOCK (self);
    return;
  }
}

static gboolean
gst_omx_audio_dec_start (GstAudioDecoder * decoder)
{
  GstOMXAudioDec *self;

  self = GST_OMX_AUDIO_DEC (decoder);

  self->last_upstream_ts = 0;
  self->eos = FALSE;
  self->downstream_flow_ret = GST_FLOW_OK;

  return TRUE;
}

static gboolean
gst_omx_audio_dec_stop (GstAudioDecoder * decoder)
{
  GstOMXAudioDec *self;

  self = GST_OMX_AUDIO_DEC (decoder);

  GST_INFO_OBJECT (self, "Stopping decoder");

  gst_omx_port_set_flushing (self->dec_in_port, 5 * GST_SECOND, TRUE);
  gst_omx_port_set_flushing (self->dec_out_port, 5 * GST_SECOND, TRUE);

#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
  gst_omx_audio_dec_bitstream_queue_clean(self, FALSE);

  if (self->bitstream_srcpad) {
    gst_pad_stop_task (self->bitstream_srcpad);
  }
  if (self->bitstream_queue) {
    g_object_unref (self->bitstream_queue);
    self->bitstream_queue = NULL;
  }
#endif

  gst_pad_stop_task (GST_AUDIO_DECODER_SRC_PAD (decoder));

  if (gst_omx_component_get_state (self->dec, 0) > OMX_StateIdle)
    gst_omx_component_set_state (self->dec, OMX_StateIdle);

  self->downstream_flow_ret = GST_FLOW_FLUSHING;
  self->started = FALSE;
  self->eos = FALSE;

  g_mutex_lock (&self->drain_lock);
  self->draining = FALSE;
  g_cond_broadcast (&self->drain_cond);
  g_mutex_unlock (&self->drain_lock);

  gst_omx_component_get_state (self->dec, 5 * GST_SECOND);

  gst_buffer_replace (&self->codec_data, NULL);

  GST_INFO_OBJECT (self, "Stopped decoder");

  return TRUE;
}

static gboolean
gst_omx_audio_dec_set_format (GstAudioDecoder * decoder, GstCaps * caps)
{
  GstOMXAudioDec *self;
  GstOMXAudioDecClass *klass;
  GstStructure *s;
  const GValue *codec_data;
  gboolean is_format_change = FALSE;
  gboolean needs_disable = FALSE;
  OMX_ERRORTYPE err;

  self = GST_OMX_AUDIO_DEC (decoder);
  klass = GST_OMX_AUDIO_DEC_GET_CLASS (decoder);

  GST_WARNING_OBJECT(self, "%s:a-dec set_format in...", SMEPLAYER_STAT_TAG_TIME);

  /* Check if the caps change is a real format change or if only irrelevant
   * parts of the caps have changed or nothing at all.
   */
  if (klass->is_format_change)
    is_format_change = klass->is_format_change (self, self->dec_in_port, caps);

  needs_disable =
      gst_omx_component_get_state (self->dec,
      GST_CLOCK_TIME_NONE) != OMX_StateLoaded;
  /* If the component is not in Loaded state and a real format change happens
   * we have to disable the port and re-allocate all buffers. If no real
   * format change happened we can just exit here.
   */
  if (needs_disable && !is_format_change) {
    GST_DEBUG_OBJECT (self,
        "Already running and caps did not change the format");
    return TRUE;
  }

  if (needs_disable && is_format_change) {
    GstOMXPort *out_port = self->dec_out_port;

    GST_DEBUG_OBJECT (self, "Need to disable and drain decoder");

    gst_omx_audio_dec_drain (self, FALSE);
    gst_omx_audio_dec_flush (decoder, FALSE);
    gst_omx_port_set_flushing (out_port, 5 * GST_SECOND, TRUE);

    if (klass->cdata.hacks & GST_OMX_HACK_NO_COMPONENT_RECONFIGURE) {
      GST_AUDIO_DECODER_STREAM_UNLOCK (self);
      gst_omx_audio_dec_stop (GST_AUDIO_DECODER (self));
      gst_omx_audio_dec_close (GST_AUDIO_DECODER (self));
      GST_AUDIO_DECODER_STREAM_LOCK (self);

      if (!gst_omx_audio_dec_open (GST_AUDIO_DECODER (self)))
        return FALSE;
      needs_disable = FALSE;
    } else {
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
      if (gst_omx_port_deallocate_buffers (self->dec_out_port) != OMX_ErrorNone)
        return FALSE;
      if (gst_omx_port_wait_enabled (self->dec_in_port,
              1 * GST_SECOND) != OMX_ErrorNone)
        return FALSE;
      if (gst_omx_port_wait_enabled (out_port, 1 * GST_SECOND) != OMX_ErrorNone)
        return FALSE;
    }

    GST_DEBUG_OBJECT (self, "Decoder drained and disabled");
  }

  if (klass->set_format) {
    if (!klass->set_format (self, self->dec_in_port, caps)) {
      GST_ERROR_OBJECT (self, "Subclass failed to set the new format");
      return FALSE;
    }
  }

  GST_DEBUG_OBJECT (self, "Updating outport port definition");
  if (gst_omx_port_update_port_definition (self->dec_out_port,
          NULL) != OMX_ErrorNone)
    return FALSE;

  /* Get codec data from caps */
  gst_buffer_replace (&self->codec_data, NULL);
  s = gst_caps_get_structure (caps, 0);
  codec_data = gst_structure_get_value (s, "codec_data");
  if (codec_data) {
    /* Vorbis and some other codecs have multiple buffers in
     * the stream-header field */
    self->codec_data = gst_value_get_buffer (codec_data);
    if (self->codec_data)
      gst_buffer_ref (self->codec_data);
  }

  GST_DEBUG_OBJECT (self, "Enabling component");

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
      if (gst_omx_component_set_state (self->dec,
              OMX_StateIdle) != OMX_ErrorNone)
        return FALSE;

      /* Need to allocate buffers to reach Idle state */
      if (gst_omx_port_allocate_buffers (self->dec_in_port) != OMX_ErrorNone)
        return FALSE;
      if (gst_omx_port_allocate_buffers (self->dec_out_port) != OMX_ErrorNone)
        return FALSE;
    }

    if (gst_omx_component_get_state (self->dec,
            GST_CLOCK_TIME_NONE) != OMX_StateIdle)
      return FALSE;

    if (gst_omx_component_set_state (self->dec,
            OMX_StateExecuting) != OMX_ErrorNone)
      return FALSE;

    if (gst_omx_component_get_state (self->dec,
            GST_CLOCK_TIME_NONE) != OMX_StateExecuting)
      return FALSE;
  }

  /* Unset flushing to allow ports to accept data again */
  gst_omx_port_set_flushing (self->dec_in_port, 5 * GST_SECOND, FALSE);
  gst_omx_port_set_flushing (self->dec_out_port, 5 * GST_SECOND, FALSE);

  if (gst_omx_component_get_last_error (self->dec) != OMX_ErrorNone) {
    GST_ERROR_OBJECT (self, "Component in error state: %s (0x%08x)",
        gst_omx_component_get_last_error_string (self->dec),
        gst_omx_component_get_last_error (self->dec));
    return FALSE;
  }

  GST_WARNING_OBJECT(self, "%s:a-dec populate outport begin...",
      SMEPLAYER_STAT_TAG_TIME);
  err = gst_omx_port_populate (self->dec_out_port);
  if (err != OMX_ErrorNone){
    GST_WARNING("POPULATE FAILED");
    gst_omx_port_deallocate_buffers (self->dec_out_port);
  }
  GST_WARNING_OBJECT(self, "%s:a-dec populate outport end",
      SMEPLAYER_STAT_TAG_TIME);

  /* Start the srcpad loop again */
  GST_DEBUG_OBJECT (self, "Starting task again");
  self->downstream_flow_ret = GST_FLOW_OK;
#ifdef COMPUTE_OUT_PTS
  self->last_out_pts = GST_CLOCK_TIME_NONE;
#endif
  gst_pad_start_task (GST_AUDIO_DECODER_SRC_PAD (self),
      (GstTaskFunction) gst_omx_audio_dec_loop, decoder, NULL);

  GST_WARNING_OBJECT(self, "%s:a-dec set_format out", SMEPLAYER_STAT_TAG_TIME);

  return TRUE;
}

static void
gst_omx_audio_dec_flush (GstAudioDecoder * decoder, gboolean hard)
{
  GstOMXAudioDec *self = GST_OMX_AUDIO_DEC (decoder);

  GST_INFO_OBJECT (self, "Resetting decoder");

  gst_omx_port_set_flushing (self->dec_in_port, 5 * GST_SECOND, TRUE);
  gst_omx_port_set_flushing (self->dec_out_port, 5 * GST_SECOND, TRUE);

  /* Wait until the srcpad loop is finished */
  GST_AUDIO_DECODER_STREAM_UNLOCK (self);
  //GST_PAD_STREAM_LOCK (GST_AUDIO_DECODER_SRC_PAD (self));
  //GST_PAD_STREAM_UNLOCK (GST_AUDIO_DECODER_SRC_PAD (self));
  gst_pad_stop_task (GST_AUDIO_DECODER_SRC_PAD (decoder));
  GST_AUDIO_DECODER_STREAM_LOCK (self);

#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)
  gst_omx_audio_dec_bitstream_queue_clean(self, TRUE);
  self->need_segment = TRUE;
#endif

  gst_omx_port_set_flushing (self->dec_in_port, 5 * GST_SECOND, FALSE);
  gst_omx_port_set_flushing (self->dec_out_port, 5 * GST_SECOND, FALSE);
  gst_omx_port_populate (self->dec_out_port);

  self->input_count = 0;

  /* Start the srcpad loop again */
  self->last_upstream_ts = 0;
  self->downstream_flow_ret = GST_FLOW_OK;
  self->eos = FALSE;
#ifdef COMPUTE_OUT_PTS
  self->last_out_pts = GST_CLOCK_TIME_NONE;
#endif
  gst_pad_start_task (GST_AUDIO_DECODER_SRC_PAD (self),
      (GstTaskFunction) gst_omx_audio_dec_loop, decoder, NULL);
}

/* BEGIN: Added for DTS2015071700522 by liurongliang(l00180035), 2015/8/24 */
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

static gboolean
gst_omx_audio_dec_sink_eventfunc (GstAudioDecoder * dec, GstEvent * event)
{
  GstOMXAudioDec *self = GST_OMX_AUDIO_DEC (dec);
  gboolean ret;
  GstEventType ev_type = GST_EVENT_TYPE (event);

  if (ev_type == GST_EVENT_FLUSH_START ||
      ev_type == GST_EVENT_SEGMENT) {
    g_mutex_lock (&self->flush_lock);
  }

  ret =  GST_AUDIO_DECODER_CLASS(gst_omx_audio_dec_parent_class)->sink_event (dec, event);

  if (ev_type == GST_EVENT_FLUSH_START ||
      ev_type == GST_EVENT_SEGMENT) {
    g_mutex_unlock (&self->flush_lock);
  }

  switch (ev_type) {
    case GST_EVENT_FLUSH_STOP:
      GST_INFO_OBJECT(self, "audio flush stop clear some var begin");
      GST_AUDIO_DECODER_STREAM_LOCK (self);
      self->input_count = 0;
      self->downstream_flow_ret = GST_FLOW_OK;
      GST_AUDIO_DECODER_STREAM_UNLOCK (self);
      GST_INFO_OBJECT(self, "audio flush stop clear some var begin");
      break;
    default:
      break;
  }

  return ret;
}
/* END:   Added for DTS2015071700522 by liurongliang(l00180035), 2015/8/24 */

#ifdef GST_DDP_PASSTHROUGH_ONLY_SUPPORT
static gboolean
gst_omx_audio_dec_src_eventfunc (GstAudioDecoder * dec, GstEvent * event)
{
  GstOMXAudioDec *self = GST_OMX_AUDIO_DEC (dec);
  GstOMXAudioDecClass *klass;
  gboolean ret;
  gint flag;
  GstStructure *structure;

  klass = GST_OMX_AUDIO_DEC_GET_CLASS (dec);

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_CUSTOM_UPSTREAM:
      structure = gst_event_get_structure (event);

      ret = gst_structure_get_int(structure,"render_flag",&flag);
      if (ret) {
        GST_INFO_OBJECT (self, "gst_omx_audio_dec_src_eventfunc GST_EVENT_CUSTOM_UPSTREAM flag %d",flag);
        if (klass->set_render_flag) {
            klass->set_render_flag(dec,flag);
        }
      } else {
        GST_INFO_OBJECT (self, "gst_omx_audio_dec_src_eventfunc GST_EVENT_CUSTOM_UPSTREAM error");
      }
      gst_event_unref(event);
      return ret;
    default:
      break;
  }

  ret =  GST_AUDIO_DECODER_CLASS(gst_omx_audio_dec_parent_class)->src_event (dec, event);

  return ret;
}
#endif


static GstFlowReturn
gst_omx_audio_dec_handle_frame (GstAudioDecoder * decoder, GstBuffer * inbuf)
{
  GstOMXAcquireBufferReturn acq_ret = GST_OMX_ACQUIRE_BUFFER_ERROR;
  GstOMXAudioDec *self;
  GstOMXPort *port;
  GstOMXBuffer *buf;
  GstBuffer *codec_data = NULL;
  guint offset = 0;
  guint buf_size = 0;

  GstClockTime timestamp, duration;
  OMX_ERRORTYPE err;

  self = GST_OMX_AUDIO_DEC (decoder);

  GST_DEBUG_OBJECT (self, "Handling frame");

  /* Make sure to keep a reference to the input here,
   * it can be unreffed from the other thread if
   * finish_frame() is called */
  if (inbuf)
    gst_buffer_ref (inbuf);

  if (self->eos) {
    GST_WARNING_OBJECT (self, "Got frame after EOS");
    if (inbuf)
      gst_buffer_unref (inbuf);
    return GST_FLOW_EOS;
  }

  if (self->downstream_flow_ret != GST_FLOW_OK) {
    if (inbuf)
      gst_buffer_unref (inbuf);
    return self->downstream_flow_ret;
  }

  if (inbuf == NULL)
    return gst_omx_audio_dec_drain (self, TRUE);

  timestamp = GST_BUFFER_TIMESTAMP (inbuf);
  duration = GST_BUFFER_DURATION (inbuf);

  port = self->dec_in_port;

  buf_size = gst_buffer_get_size(inbuf);

  while (offset < buf_size) {
    /* Make sure to release the base class stream lock, otherwise
     * _loop() can't call _finish_frame() and we might block forever
     * because no input buffers are released */
    GST_AUDIO_DECODER_STREAM_UNLOCK (self);
    acq_ret = gst_omx_port_acquire_buffer (port, &buf);

    if (acq_ret == GST_OMX_ACQUIRE_BUFFER_ERROR) {
      GST_AUDIO_DECODER_STREAM_LOCK (self);
      goto component_error;
    } else if (acq_ret == GST_OMX_ACQUIRE_BUFFER_FLUSHING) {
      GST_AUDIO_DECODER_STREAM_LOCK (self);
      goto flushing;
    } else if (acq_ret == GST_OMX_ACQUIRE_BUFFER_RECONFIGURE) {
      /* Reallocate all buffers */
      err = gst_omx_port_set_enabled (port, FALSE);
      if (err != OMX_ErrorNone) {
        GST_AUDIO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_wait_buffers_released (port, 5 * GST_SECOND);
      if (err != OMX_ErrorNone) {
        GST_AUDIO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_deallocate_buffers (port);
      if (err != OMX_ErrorNone) {
        GST_AUDIO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_wait_enabled (port, 1 * GST_SECOND);
      if (err != OMX_ErrorNone) {
        GST_AUDIO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_set_enabled (port, TRUE);
      if (err != OMX_ErrorNone) {
        GST_AUDIO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_allocate_buffers (port);
      if (err != OMX_ErrorNone) {
        GST_AUDIO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_wait_enabled (port, 5 * GST_SECOND);
      if (err != OMX_ErrorNone) {
        GST_AUDIO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      err = gst_omx_port_mark_reconfigured (port);
      if (err != OMX_ErrorNone) {
        GST_AUDIO_DECODER_STREAM_LOCK (self);
        goto reconfigure_error;
      }

      /* Now get a new buffer and fill it */
      GST_AUDIO_DECODER_STREAM_LOCK (self);
      continue;
    }
    GST_AUDIO_DECODER_STREAM_LOCK (self);

    g_assert (acq_ret == GST_OMX_ACQUIRE_BUFFER_OK && buf != NULL);
    //we should drop this frame comes here before flush-start.
    if(GST_PAD_IS_FLUSHING (GST_AUDIO_DECODER_SINK_PAD(self))){
      GstOMXComponent *comp = port->comp;
      GST_INFO_OBJECT(self, "flush adec drop this buffer reback to pool begin !");
      g_mutex_lock (&comp->lock);
      g_queue_push_tail (&port->pending_buffers, buf);
      g_mutex_unlock (&comp->lock);
      GST_INFO_OBJECT(self, "flush adec drop this buffer reback to pool end!");
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
      GST_DEBUG_OBJECT (self, "Passing codec data to the component");

      codec_data = self->codec_data;

      if (buf->omx_buf->nAllocLen - buf->omx_buf->nOffset <
          gst_buffer_get_size (codec_data)) {
        gst_omx_port_release_buffer (port, buf);
        goto too_large_codec_data;
      }

      buf->omx_buf->nFlags |= OMX_BUFFERFLAG_CODECCONFIG;
      buf->omx_buf->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
      buf->omx_buf->nFilledLen = gst_buffer_get_size (codec_data);
      gst_buffer_extract (codec_data, 0,
          buf->omx_buf->pBuffer + buf->omx_buf->nOffset,
          buf->omx_buf->nFilledLen);

#if defined (SME_OMX_ADEC_DUMP_IN)
      if (in_file)
      {
          fwrite(buf->omx_buf->pBuffer + buf->omx_buf->nOffset, 1,
                 buf->omx_buf->nFilledLen, in_file);
          GST_DEBUG("write %d bytes codecdata to file, offset %d, %d",
                    buf->omx_buf->nFilledLen,
                    offset, buf->omx_buf->nOffset);
      }
#endif

      if (GST_CLOCK_TIME_IS_VALID (timestamp))
        buf->omx_buf->nTimeStamp =
            gst_util_uint64_scale (timestamp, OMX_TICKS_PER_SECOND, GST_SECOND);
      else
        buf->omx_buf->nTimeStamp = -1;
      GST_INFO_OBJECT(self, "adec in,timestamp=%lld,inpts=%lld",
        timestamp, buf->omx_buf->nTimeStamp);
      buf->omx_buf->nTickCount = 0;

      self->started = TRUE;
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
        MIN (buf_size - offset,
        buf->omx_buf->nAllocLen - buf->omx_buf->nOffset);
#if defined HACK_IN_GST_BUF
    gst_buffer_extract (inbuf, data_offset,
        buf->omx_buf->pBuffer + buf->omx_buf->nOffset,
        buf->omx_buf->nFilledLen);
#else
    gst_buffer_extract (inbuf, offset,
        buf->omx_buf->pBuffer + buf->omx_buf->nOffset,
        buf->omx_buf->nFilledLen);
#endif

#if defined (SME_OMX_ADEC_DUMP_IN)
    if (in_file)
    {
        fwrite(buf->omx_buf->pBuffer + buf->omx_buf->nOffset, 1,
               buf->omx_buf->nFilledLen, in_file);
        GST_DEBUG("write %d bytes to file, offset %d, %d",
                  buf->omx_buf->nFilledLen,
                  offset, buf->omx_buf->nOffset);
    }
#endif

    if (timestamp != GST_CLOCK_TIME_NONE) {
      buf->omx_buf->nTimeStamp =
          gst_util_uint64_scale (timestamp, OMX_TICKS_PER_SECOND, GST_SECOND);
      self->last_upstream_ts = timestamp;
#ifdef COMPUTE_OUT_PTS
      if(GST_CLOCK_TIME_NONE == self->last_out_pts){
        self->last_out_pts = timestamp;
      }
#endif
    } else {
      buf->omx_buf->nTimeStamp = -1;
    }

    GST_DEBUG_OBJECT(self, "adec in,timestamp=%lld,inpts=%lld",
      timestamp, buf->omx_buf->nTimeStamp);

    if (duration != GST_CLOCK_TIME_NONE && offset == 0) {
      buf->omx_buf->nTickCount =
          gst_util_uint64_scale (duration, OMX_TICKS_PER_SECOND, GST_SECOND);
      self->last_upstream_ts += duration;
    } else {
      buf->omx_buf->nTickCount = 0;
    }

    if (offset == 0)
      buf->omx_buf->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;

    /* TODO: Set flags
     *   - OMX_BUFFERFLAG_DECODEONLY for buffers that are outside
     *     the segment
     */

    offset += buf->omx_buf->nFilledLen;

    if (offset == buf_size)
      buf->omx_buf->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;

    self->started = TRUE;
    err = gst_omx_port_release_buffer (port, buf);
    if (err != OMX_ErrorNone)
      goto release_error;
  }

  /* BEGIN: Added for DTS2015071700522 by liurongliang(l00180035), 2015/8/24 */
  self->input_count++;
  if(self->input_count >= SME_DEC_DELAY_MAX_COUNT){
    GstBuffer* empty_buffer = gen_decodeonly_gstbuffer();
    gboolean err ;
    GstEvent* ev_eos = gst_event_new_eos();

    gst_pad_push (GST_AUDIO_DECODER_SRC_PAD(self), empty_buffer);

    if(ev_eos){
      err = gst_pad_push_event(GST_AUDIO_DECODER_SRC_PAD(self), ev_eos);
      GST_INFO("push eos return err=%d", err);
      if(!err){
        gst_event_unref(ev_eos);
        GST_WARNING("push eos return err=%d", err);
      }
    }else{
      GST_WARNING("ev_eos is null");
    }

    GST_ELEMENT_WARNING(decoder, STREAM, DECODE,
                ("input frame is more than %d", SME_DEC_DELAY_MAX_COUNT),
                ("audio_dec:a dec not suport!"));
    self->downstream_flow_ret = GST_FLOW_EOS;
  }
  /* END:   Added for DTS2015071700522 by liurongliang(l00180035), 2015/8/24 */

  GST_DEBUG_OBJECT (self, "Passed frame to component");
  if (inbuf)
    gst_buffer_unref (inbuf);

  return self->downstream_flow_ret;

full_buffer:
  {
    if (inbuf)
      gst_buffer_unref (inbuf);

    GST_ELEMENT_ERROR (self, LIBRARY, FAILED, (NULL),
        ("Got OpenMAX buffer with no free space (%p, %u/%u)", buf,
            (guint) buf->omx_buf->nOffset, (guint) buf->omx_buf->nAllocLen));
    return GST_FLOW_ERROR;
  }

flow_error:
  {
    if (inbuf)
      gst_buffer_unref (inbuf);

    return self->downstream_flow_ret;
  }

too_large_codec_data:
  {
    if (inbuf)
      gst_buffer_unref (inbuf);

    GST_ELEMENT_ERROR (self, STREAM, FORMAT, (NULL),
        ("codec_data larger than supported by OpenMAX port "
            "(%" G_GSIZE_FORMAT " > %u)", gst_buffer_get_size (codec_data),
            (guint) self->dec_in_port->port_def.nBufferSize));
    return GST_FLOW_ERROR;
  }

component_error:
  {
    if (inbuf)
      gst_buffer_unref (inbuf);

    GST_ELEMENT_ERROR (self, LIBRARY, FAILED, (NULL),
        ("OpenMAX component in error state %s (0x%08x)",
            gst_omx_component_get_last_error_string (self->dec),
            gst_omx_component_get_last_error (self->dec)));
    return GST_FLOW_ERROR;
  }

flushing:
  {
    if (inbuf)
      gst_buffer_unref (inbuf);

    GST_INFO_OBJECT (self, "OMX IL Is Being Flushed -- still returning OK");
    return GST_FLOW_OK;
  }
reconfigure_error:
  {
    if (inbuf)
      gst_buffer_unref (inbuf);

    GST_ELEMENT_ERROR (self, LIBRARY, SETTINGS, (NULL),
        ("Unable to reconfigure input port"));
    return GST_FLOW_ERROR;
  }
release_error:
  {
    if (inbuf)
      gst_buffer_unref (inbuf);

    GST_ELEMENT_ERROR (self, LIBRARY, SETTINGS, (NULL),
        ("Failed to relase input buffer to component: %s (0x%08x)",
            gst_omx_error_to_string (err), err));

    return GST_FLOW_ERROR;
  }
}

static GstFlowReturn
gst_omx_audio_dec_drain (GstOMXAudioDec * self, gboolean is_eos)
{
  GstOMXAudioDecClass *klass;
  GstOMXBuffer *buf;
  GstOMXAcquireBufferReturn acq_ret;
  OMX_ERRORTYPE err;

  GST_WARNING_OBJECT (self, "Draining component");

  klass = GST_OMX_AUDIO_DEC_GET_CLASS (self);

  if (!self->started) {
    GST_DEBUG_OBJECT (self, "Component not started yet");
    return GST_FLOW_OK;
  }
  self->started = FALSE;

  /* Don't send EOS buffer twice, this doesn't work */
  if (self->eos) {
    GST_DEBUG_OBJECT (self, "Component is EOS already");
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
  GST_AUDIO_DECODER_STREAM_UNLOCK (self);

  /* Send an EOS buffer to the component and let the base
   * class drop the EOS event. We will send it later when
   * the EOS buffer arrives on the output port. */
  acq_ret = gst_omx_port_acquire_buffer (self->dec_in_port, &buf);
  if (acq_ret != GST_OMX_ACQUIRE_BUFFER_OK) {
    GST_AUDIO_DECODER_STREAM_LOCK (self);
    GST_ERROR_OBJECT (self, "Failed to acquire buffer for draining: %d",
        acq_ret);
    return GST_FLOW_ERROR;
  }

  g_mutex_lock (&self->drain_lock);

  /* BEGIN: Modified by liurongliang(l00180035), 2015/8/6 */
  /*
    when EOS coming after flush_start a little.
  */
  self->draining = TRUE;
  buf->omx_buf->nFilledLen = 0;
  if(GST_FLOW_FLUSHING == self->downstream_flow_ret){
    gst_omx_port_release_buffer (self->dec_in_port, buf);
    g_mutex_unlock (&self->drain_lock);
    GST_WARNING_OBJECT (self, "omx audio flushing now!");

    GST_AUDIO_DECODER_STREAM_LOCK (self);
    self->started = FALSE;

    return GST_FLOW_OK;
  }
  /* END:   Modified by liurongliang(l00180035), 2015/8/6 */

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
    GST_AUDIO_DECODER_STREAM_LOCK (self);
    return GST_FLOW_ERROR;
  }

  GST_WARNING_OBJECT (self, "Waiting until component is drained");

  if (G_UNLIKELY (self->dec->hacks & GST_OMX_HACK_DRAIN_MAY_NOT_RETURN)) {
    gint64 wait_until = g_get_monotonic_time () + G_TIME_SPAN_SECOND / 2;

    if (!g_cond_wait_until (&self->drain_cond, &self->drain_lock, wait_until))
      GST_WARNING_OBJECT (self, "Drain timed out");
    else
      GST_DEBUG_OBJECT (self, "Drained component");

  } else {
    g_cond_wait (&self->drain_cond, &self->drain_lock);
    GST_DEBUG_OBJECT (self, "Drained component");
  }

  g_mutex_unlock (&self->drain_lock);
  GST_AUDIO_DECODER_STREAM_LOCK (self);

  self->started = FALSE;

  GST_WARNING_OBJECT (self, "Draining component out");

  return GST_FLOW_OK;
}

#if defined (GST_AUDIO_DECODER_MULTI_OUTPUT)

static guint
gst_omx_audio_dec_bitstream_get_pad(GstOMXAudioDec *self)
{
    GstOMXAudioDecClass *klass = GST_OMX_AUDIO_DEC_GET_CLASS (self);
    GstOMXPort *port = self->dec_out_port;

    if (!self->bitstream_srcpad && klass->get_bitstream_srcpad) {
        self->bitstream_srcpad = klass->get_bitstream_srcpad(self, port);
        if (self->bitstream_srcpad) {
            if (!self->bitstream_queue) {
                self->bitstream_queue =
                    gst_data_queue_new (_check_bitstream_queue_full,
                                        NULL, NULL, self);
                if (self->bitstream_queue == NULL) {
                    GST_ERROR("fail to create bitstream queue");
                    /**TODO: do some thing here */
                }

                self->max_bitstream_queue_item_size = 1;
            }
        }
    }

    return 0;
}

static guint
gst_omx_audio_dec_bitstream_handle_buffer(GstOMXAudioDec *self,
                                          GstOMXBuffer *buf,
                                          guint pcm_len)
{
    GstPad *bitstream_srcpad = NULL;
    GstBuffer *bitstream_outbuf = NULL;
    gboolean bres = FALSE;
    GstOMXAudioDecClass *klass = GST_OMX_AUDIO_DEC_GET_CLASS (self);
    GstOMXPort *port = self->dec_out_port;

#define DUMMY_FRAME_SIZE 4

    bitstream_srcpad = self->bitstream_srcpad;

    /* start the bitstream src pad task if it hasn't been started yet */
    if (bitstream_srcpad && !self->bitstream_srcpad_task_started) {
        gst_data_queue_set_flushing(self->bitstream_queue, FALSE);
        bres = gst_pad_start_task (
            self->bitstream_srcpad,
            (GstTaskFunction)gst_omx_audio_dec_bitstream_src_loop,
            self,
            NULL);
        if (!bres) {
            GST_ERROR("fail to create bitstream source pad task");
            /*TODO: do something here */
        }
        self->bitstream_srcpad_task_started = TRUE;
        GST_INFO("start src loop task when push buffer");
    }

    if (bitstream_srcpad != NULL) {
        if (klass->get_bitstream_buffer && buf) {
            bitstream_outbuf = klass->get_bitstream_buffer(self, port, buf);
        }

        /* TODO: it's better to generate pause burst packets */
        if (buf == NULL)
        {
            guchar dummy_buf[DUMMY_FRAME_SIZE] = {0,};
            GstAudioDecoder *audiodecoder = NULL;
            GstAllocator * allocator = NULL;
            GstAllocationParams params = {0};

            audiodecoder= GST_AUDIO_DECODER(self);
            gst_audio_decoder_get_allocator(audiodecoder, &allocator, &params);
            bitstream_outbuf = gst_buffer_new_allocate (allocator,
                                                        DUMMY_FRAME_SIZE,
                                                        &params);
            if(bitstream_outbuf) {
                GST_BUFFER_FLAG_SET(bitstream_outbuf,
                                    GST_BUFFER_FLAG_DECODE_ONLY);
                GST_BUFFER_PTS(bitstream_outbuf) = 0;
                GST_BUFFER_DTS(bitstream_outbuf) = 0;
                GST_BUFFER_DURATION(bitstream_outbuf) = 0;
                memset(dummy_buf, 0, sizeof(dummy_buf));
                gst_buffer_fill(bitstream_outbuf, 0,
                                (gconstpointer)dummy_buf,
                                DUMMY_FRAME_SIZE);
            }
        }

        if (bitstream_outbuf) {
            guint sample_num = pcm_len / self->info.bpf;

            if (buf) {
                GST_BUFFER_DTS (bitstream_outbuf) =
                GST_BUFFER_PTS (bitstream_outbuf) =
                    gst_util_uint64_scale (buf->omx_buf->nTimeStamp, GST_SECOND,
                                           OMX_TICKS_PER_SECOND);
                GST_BUFFER_DURATION (bitstream_outbuf) =
                    gst_util_uint64_scale (sample_num, GST_SECOND,
                                           self->info.rate);
            }

            bres = gst_omx_audio_dec_bitstream_push_data(self,
                                                         bitstream_outbuf);
            if (!bres){
                GST_WARNING("push bitstream buffer failed");
            }
        }
    }
    return 0;
}

static guint
gst_omx_audio_dec_bitstream_handle_event(GstOMXAudioDec *self,
                                         GstEvent *event)
{
    GstPad *bitstream_srcpad = NULL;
    gboolean bres = FALSE;
    GstOMXAudioDecClass *klass = GST_OMX_AUDIO_DEC_GET_CLASS (self);
    GstOMXPort *port = self->dec_out_port;

    bitstream_srcpad = self->bitstream_srcpad;

    /* start the bitstream src pad task if it hasn't been started yet */
    if (bitstream_srcpad && !self->bitstream_srcpad_task_started) {
        gst_data_queue_set_flushing(self->bitstream_queue, FALSE);
        bres = gst_pad_start_task (
            self->bitstream_srcpad,
            (GstTaskFunction)gst_omx_audio_dec_bitstream_src_loop,
            self,
            NULL);
        if (!bres) {
            GST_ERROR("fail to create bitstream source pad task");
            /*TODO: do something here */
        }
        self->bitstream_srcpad_task_started = TRUE;
        GST_INFO("start src loop task when push event");
    }

    if (bitstream_srcpad != NULL) {
        bres = gst_omx_audio_dec_bitstream_push_data(self, event);
        if (!bres){
            GST_WARNING("push bitstream buffer failed");
        }
    }
    return 0;
}

static void
gst_omx_audio_dec_bitstream_src_loop(GstOMXAudioDec * self)
{
    GstFlowReturn flow_ret;
    GstDataQueueItem *item;
    GstPad *bitstream_srcpad = NULL;

    bitstream_srcpad = self->bitstream_srcpad;

    GST_DEBUG_OBJECT(self, "enter bitstream src loop");

    if (bitstream_srcpad == NULL) {
        GST_WARNING_OBJECT(self, "null bitstream src pad");
        return;
    }

    if (!gst_data_queue_peek(self->bitstream_queue, &item)) {
        GST_WARNING_OBJECT(self, "peek data fail due to flushing");
        goto flushing;
    }

    if (!gst_data_queue_pop(self->bitstream_queue, &item)) {
        GST_WARNING_OBJECT(self, "pop data fail, go to end");
        goto end;
    }

    if (G_LIKELY (GST_IS_BUFFER (item->object))) {
        /* buffer */
        GstBuffer *buffer;
        GstClockTime timestamp;

        buffer = GST_BUFFER_CAST (item->object);

        if (self->need_segment) {
            GstSegment segment;

            timestamp = GST_BUFFER_TIMESTAMP (buffer);

            /* And send a newsegment */
            GST_INFO_OBJECT (self, "Sending new-segment. segment start:%"
                             GST_TIME_FORMAT, GST_TIME_ARGS (timestamp));
            gst_segment_init (&segment, GST_FORMAT_TIME);
            segment.start = timestamp;
            segment.time = timestamp;
            segment.position = timestamp;

            gst_pad_push_event (bitstream_srcpad,
                                gst_event_new_segment (&segment));

            self->need_segment = FALSE;
        }

        flow_ret = gst_audio_decoder_pad_output(GST_AUDIO_DECODER(self),
                                                bitstream_srcpad,
                                                gst_buffer_ref(buffer),
                                                1);
        item->destroy(item);
        if (flow_ret != GST_FLOW_OK) {
        }
    } else {
        /* event */
        GstEvent *event = NULL;

        event = GST_EVENT_CAST (item->object);

        switch (GST_EVENT_TYPE (event)) {
            case GST_EVENT_EOS:
                gst_pad_push_event(bitstream_srcpad,
                                   gst_event_ref(bitstream_srcpad));
                item->destroy(item);
                goto eos;
                break;
            default:
                GST_WARNING_OBJECT(self, "not handled event");
                break;
        }
    }

    goto end;

eos:
    {
        GST_WARNING_OBJECT(self, "got eos, leave bitstream src loop");
        gst_pad_pause_task(bitstream_srcpad);

        GST_AUDIO_DECODER_STREAM_LOCK (self);
        self->bitstream_srcpad_task_started = FALSE;
        GST_AUDIO_DECODER_STREAM_UNLOCK (self);

        return;
    }

flushing:
    {
        GST_WARNING_OBJECT(self, "flushing, leave bitstream src loop");
        gst_pad_pause_task(bitstream_srcpad);

        GST_AUDIO_DECODER_STREAM_LOCK (self);
        self->bitstream_srcpad_task_started = FALSE;
        GST_AUDIO_DECODER_STREAM_UNLOCK (self);
        return;
    }

end:
    GST_DEBUG_OBJECT(self, "leave bitstream src loop");

    return;
}

static void
gst_omx_audio_dec_bitstream_queue_clean(GstOMXAudioDec *self,
                                        gboolean stream_lock)
{
    GST_INFO_OBJECT(self, "enter queue clean");

    if (self->bitstream_queue){
        gst_data_queue_set_flushing(self->bitstream_queue, TRUE);
    }

    if (self->bitstream_srcpad) {
        if (stream_lock) {
            GST_AUDIO_DECODER_STREAM_UNLOCK (self);
        }
        GST_PAD_STREAM_LOCK (self->bitstream_srcpad);
        GST_PAD_STREAM_UNLOCK (self->bitstream_srcpad);
        if (stream_lock) {
            GST_AUDIO_DECODER_STREAM_LOCK (self);
        }
    }

    if (self->bitstream_queue) {
        gst_data_queue_flush(self->bitstream_queue);
    }

    GST_INFO_OBJECT(self, "leave queue clean");
}


static gboolean
gst_omx_audio_dec_bitstream_push_data(GstOMXAudioDec *self,
                                      GstBuffer *gstbuffer)
{
    GstDataQueueItem *item = g_new (GstDataQueueItem, 1);
    gboolean ret;

    item->object = GST_MINI_OBJECT_CAST (gstbuffer);
    item->duration = GST_BUFFER_DURATION (gstbuffer);
    item->visible = TRUE;
    item->size = gst_buffer_get_size (gstbuffer);

    item->destroy = (GDestroyNotify) _data_bitstream_queue_item_destroy;

    ret = gst_data_queue_push (self->bitstream_queue, item);
    if (!ret) {
        GST_WARNING("Push data failed, destroy it");
        _data_bitstream_queue_item_destroy(item);
        return FALSE;
    }
    return TRUE;
}

static gboolean
gst_omx_audio_dec_bitstream_push_event(GstOMXAudioDec *self,
                                       GstEvent *event)
{
    GstDataQueueItem *item = g_new (GstDataQueueItem, 1);
    gboolean ret;

    item->object = GST_MINI_OBJECT_CAST (event);
    item->destroy = (GDestroyNotify) _data_bitstream_queue_item_destroy;

    ret = gst_data_queue_push_force (self->bitstream_queue, item);
    if (!ret) {
        GST_WARNING("Push event failed, destroy it");
        _data_bitstream_queue_item_destroy(item);
        return FALSE;
    }
    return TRUE;
}

static void
_data_bitstream_queue_item_destroy (GstDataQueueItem *item)
{
  gst_mini_object_unref (item->object);
  g_free (item);
}

static gboolean
_check_bitstream_queue_full (GstDataQueue * q,
                             guint visible,
                             guint bytes,
                             guint64 total_time,
                             gpointer checkdata)
{
    GstOMXAudioDec *self = checkdata;

    if (self->max_bitstream_queue_item_size == 0)
        return FALSE;

    return visible >= self->max_bitstream_queue_item_size;
}

#endif
