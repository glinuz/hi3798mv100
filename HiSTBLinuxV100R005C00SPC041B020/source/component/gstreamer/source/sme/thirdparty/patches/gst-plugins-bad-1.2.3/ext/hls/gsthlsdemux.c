/* GStreamer
 * Copyright (C) 2010 Marc-Andre Lureau <marcandre.lureau@gmail.com>
 * Copyright (C) 2010 Andoni Morales Alastruey <ylatuya@gmail.com>
 * Copyright (C) 2011, Hewlett-Packard Development Company, L.P.
 *  Author: Youness Alaoui <youness.alaoui@collabora.co.uk>, Collabora Ltd.
 *  Author: Sebastian Dröge <sebastian.droege@collabora.co.uk>, Collabora Ltd.
 *
 * Gsthlsdemux.c:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
/**
 * SECTION:element-hlsdemux
 *
 * HTTP Live Streaming demuxer element.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch souphttpsrc location=http://devimages.apple.com/iphone/samples/bipbop/gear4/prog_index.m3u8 ! hlsdemux ! decodebin2 ! videoconvert ! videoscale ! autovideosink
 * ]|
 * </refsect2>
 *
 * Last reviewed on 2010-10-07
 */

/*lint -e19*/ //unused defined macro G_DEFINE_TYPE
/*lint -e528*/ //unused define gst_hls_demux_instance_private
/*lint -e571*/ //type cast
/*lint -e647*/ //type cut
/*lint -e652*/ //already defined macro GST_CAT_DEFAULT
/*lint -e655*/ //enum for bit operation
/*lint -e717*/ //do while (0)
/*lint -e749*/ //unused enum PROP_0, PROP_LAST
/*lint -e774*/ //condition always TURE
/*lint -e801*/ //goto
/*lint -e826*/ //point cast
/*lint -e878*/ //variable uninitialized

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

/* FIXME 0.11: suppress warnings for deprecated API such as GStaticRecMutex
 * with newer GLib versions (>= 2.31.0) */
#define GLIB_DISABLE_DEPRECATION_WARNINGS

#include <string.h>
#include <gst/glib-compat-private.h>
#include <gnutls/gnutls.h>
#include <gnutls/crypto.h>
#include "gsthlsdemux.h"

static GstStaticPadTemplate srctemplate = GST_STATIC_PAD_TEMPLATE ("src_%u",
    GST_PAD_SRC,
    GST_PAD_SOMETIMES,
    GST_STATIC_CAPS_ANY);

static GstStaticPadTemplate sinktemplate = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("application/x-hls"));

GST_DEBUG_CATEGORY_STATIC (gst_hls_demux_debug);
#define GST_CAT_DEFAULT gst_hls_demux_debug

enum
{
  PROP_0,
  PROP_FRAGMENTS_CACHE,
  PROP_BITRATE_LIMIT,
  PROP_CONNECTION_SPEED,
  PROP_TIMEOUT,
  PROP_CUR_BW,
  PROP_SME_CACHE_PROC,
  PROP_USER_AGENT,
  PROP_COOKIES,
  PROP_REFERER,
  PROP_LAST
};

enum
{
  SIGNAL_M3U8_PARSE_COMPLETE,
  LAST_SIGNAL
};

typedef struct _GstHLSDemuxStartupMode GstHLSDemuxStartupMode;

struct _GstHLSDemuxStartupMode
{
  guint64 startup_bitrate;
  gboolean is_adaptive;
};

#define DEFAULT_FRAGMENTS_CACHE 3
#define DEFAULT_BITRATE_LIMIT 0.8
#define DEFAULT_CONNECTION_SPEED 0
#define DEFAULT_TIMEOUT_TIME 15
#define DEFAULT_UPDATE_INTERVAL_FACTOR 0.5
#define DEFAULT_FAILED_COUNT 3
#define STATISTICS_MESSAGE_NAME "adaptive-streaming-statistics"
#define MAX_DATA_QUEUE_SIZE (2 * 1024 * 1024) //2MB

/* GObject */
static void gst_hls_demux_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_hls_demux_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);
static void gst_hls_demux_dispose (GObject * obj);

/* GstElement */
static GstStateChangeReturn
gst_hls_demux_change_state (GstElement * element, GstStateChange transition);

/* GstHLSDemux */
static GstFlowReturn gst_hls_demux_chain (GstPad * pad, GstObject * parent,
    GstBuffer * buf);
static gboolean gst_hls_demux_sink_event (GstPad * pad, GstObject * parent,
    GstEvent * event);
static gboolean gst_hls_demux_src_event (GstPad * pad, GstObject * parent,
    GstEvent * event);
static gboolean gst_hls_demux_src_query (GstPad * pad, GstObject * parent,
    GstQuery * query);
static void gst_hls_demux_stream_loop (GstHLSDemuxStream * stream);
static void gst_hls_demux_updates_loop (GstHLSDemuxStream * stream);
static void gst_hls_demux_stop (GstHLSDemux * demux);
static void gst_hls_demux_pause_tasks (GstHLSDemuxStream * stream, gboolean caching);
static gboolean gst_hls_demux_switch_playlist (GstHLSDemuxStream * stream);
static gboolean gst_hls_demux_get_next_fragment (GstHLSDemuxStream * stream,
    gboolean caching);
static gboolean gst_hls_demux_update_playlist (GstHLSDemuxStream * stream,
    gboolean update);
static void gst_hls_demux_reset (GstHLSDemux * demux, gboolean dispose);
static gboolean gst_hls_demux_set_location (GstHLSDemux * demux,
    const gchar * uri);
static gchar *gst_hls_src_buf_to_utf8_playlist (GstBuffer * buf);
void gst_hls_demux_get_buffer_cb(void * hander, GstFragment *download);

/* BEGIN: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */
static void gst_hls_demux_get_header_for_I_frames_only (const GstHLSDemuxStream * stream,
    gint64 range_start, const gchar * uri);
/* END: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */

#define gst_hls_demux_parent_class parent_class
G_DEFINE_TYPE (GstHLSDemux, gst_hls_demux, GST_TYPE_ELEMENT);

/** BEGIN: Added for DTS2014072906154by liurongliang(l00180035), 2014/7/30 */
GstFragment *
gst_hls_downloader_fetch_uri_check_timeout(GstHLSDemuxStream * stream, const gchar * uri);

#define TRY_INTERVAL (200000)//us
#define SLEEP_INTERVAL (10000)//us
#define UPDATE_SLEEP_TIME (20000)//us
#define MAX_FRAG_QUEUE_LENGTH (4)
#define TIME_OUT_KEY "timeout"
#define USER_AGENT "user-agent"
#define COOKIES "cookies"
#define REFERER "referer"

#define GST_SME_PROP_KEY_CUR_BW "gst-sme-prop-key-current-bandwidth"
#define GST_SME_PROP_KEY_CACHE_PROC "gst-sme-prop-key-cache-proc"

#define SLEEP_IF_NOT_CANCEL(stream, sleep_time)\
  do {\
    guint sleep_interval = SLEEP_INTERVAL;\
    guint count = (sleep_time) / sleep_interval;\
    if (0 == count) {\
      if ((sleep_time) > 0) {\
        sleep_interval = (sleep_time);\
      }\
      count = 1;\
    }\
    for (guint i = 0; i < count; i++) {\
      if (stream->cancelled) {\
        GST_WARNING ("wait but canceled");\
        goto quit;\
      }\
      g_usleep (sleep_interval);\
    }\
  } while (FALSE)

static guint gst_hls_demux_signals[LAST_SIGNAL] = { 0 };

/* BEGIN: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
static gboolean
_check_queue_full (GstDataQueue * q, guint visible, guint bytes, guint64 total_time,
    GstHLSDemux * demux)
{
  GST_DEBUG_OBJECT (demux, "now bytes %u, visible %u", bytes, visible);
  return bytes >= MAX_DATA_QUEUE_SIZE;
}
/* END: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */

static void
_hls_demux_data_queue_item_destroy (GstDataQueueItem * item)
{
  gst_mini_object_unref (item->object);
  if (item->_gst_reserved[0]) {
    g_free(item->_gst_reserved[0]);
    item->_gst_reserved[0] = NULL;
  }
  g_free (item);
}

static guint64
gst_get_sys_time (void)
{
  GTimeVal timeval;
  guint64 timetick = 0;

  g_get_current_time (&timeval);

  timetick = (guint64) timeval.tv_sec * 1000 + ((guint64) timeval.tv_usec + 500) / 1000;

  return timetick;
}
/** END: Added for DTS2014072906154by liurongliang(l00180035), 2014/7/30 */

static void
gst_hls_demux_dispose (GObject * obj)
{
  GstHLSDemux *demux = GST_HLS_DEMUX (obj);
  gboolean dispose = TRUE;

  GST_INFO ("hls demux dispose in.");

  gst_hls_demux_reset (demux, dispose);

  /* BEGIN: Added for DTS2015092302576/DTS2015092303479 by lizimian(l00251625), 2015/10/6 */
  g_mutex_clear (&demux->streams_lock);
  /* END: Added for DTS2015092302576/DTS2015092303479 by lizimian(l00251625), 2015/10/6 */

  G_OBJECT_CLASS (parent_class)->dispose (obj);

  g_free (demux->user_agent);
  demux->user_agent = NULL;
  g_free (demux->referer);
  demux->referer = NULL;
  if (demux->cookies) {
    g_strfreev (demux->cookies);
    demux->cookies = NULL;
  }
  GST_INFO ("hls demux dispose out.");
}

static void
gst_hls_demux_class_init (GstHLSDemuxClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *element_class;

  gobject_class = (GObjectClass *) klass;
  element_class = (GstElementClass *) klass;

  gobject_class->set_property = gst_hls_demux_set_property;
  gobject_class->get_property = gst_hls_demux_get_property;
  gobject_class->dispose = gst_hls_demux_dispose;

  g_object_class_install_property (gobject_class, PROP_FRAGMENTS_CACHE,
      g_param_spec_uint ("fragments-cache", "Fragments cache",
          "Number of fragments needed to be cached to start playing",
          2, G_MAXUINT, DEFAULT_FRAGMENTS_CACHE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_BITRATE_LIMIT,
      g_param_spec_float ("bitrate-limit",
          "Bitrate limit in %",
          "Limit of the available bitrate to use when switching to alternates.",
          (gfloat) 0, (gfloat) 1, (gfloat) DEFAULT_BITRATE_LIMIT,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_CONNECTION_SPEED,
      g_param_spec_uint64 ("connection-speed", "Connection Speed",
          "Network connection speed in kbps (0 = unknown)",
          0, G_MAXUINT64 / 1000, DEFAULT_CONNECTION_SPEED,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_TIMEOUT,
      g_param_spec_uint (TIME_OUT_KEY, TIME_OUT_KEY,
          "Value in seconds to timeout (0 = No timeout).", 0,
          3600, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property(gobject_class, PROP_CUR_BW,
      g_param_spec_uint (GST_SME_PROP_KEY_CUR_BW, "current bandwidth",
      "current bandwidth", 0, (guint)(-1), 0, G_PARAM_READWRITE
      | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property(gobject_class, PROP_SME_CACHE_PROC,
        g_param_spec_string (GST_SME_PROP_KEY_CACHE_PROC, "CACHE_PROC_URI",
        "URI of the first fragment to play", NULL, G_PARAM_READWRITE
        | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_USER_AGENT,
        g_param_spec_string ("user-agent", "User-Agent",
            "Value of the User-Agent HTTP request header field",
            NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_REFERER,
        g_param_spec_string ("referer", "Referer",
            "Value of the Referer HTTP request header field",
            NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_COOKIES,
        g_param_spec_boxed ("cookies", "Cookies", "HTTP request cookies",
            G_TYPE_STRV, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  gst_hls_demux_signals[SIGNAL_M3U8_PARSE_COMPLETE] =
    g_signal_new ("manifest-parse-complete",
        G_TYPE_FROM_CLASS (klass), G_SIGNAL_RUN_FIRST,
        0, NULL, NULL,
        g_cclosure_marshal_generic, G_TYPE_NONE, 2,
        G_TYPE_POINTER,  G_TYPE_POINTER);

  element_class->change_state = GST_DEBUG_FUNCPTR (gst_hls_demux_change_state);

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&srctemplate));

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&sinktemplate));

  gst_element_class_set_static_metadata (element_class,
      "HLS Demuxer",
      "Demuxer/URIList",
      "HTTP Live Streaming demuxer",
      "Marc-Andre Lureau <marcandre.lureau@gmail.com>\n"
      "Andoni Morales Alastruey <ylatuya@gmail.com>");

  GST_DEBUG_CATEGORY_INIT (gst_hls_demux_debug, "hlsdemux", 0,
      "hlsdemux element");
}

static void
gst_hls_demux_init (GstHLSDemux * demux)
{
  GST_INFO ("hls demux init in.");
  /* sink pad */
  demux->sinkpad = gst_pad_new_from_static_template (&sinktemplate, "sink");
  gst_pad_set_chain_function (demux->sinkpad,
      GST_DEBUG_FUNCPTR (gst_hls_demux_chain));
  gst_pad_set_event_function (demux->sinkpad,
      GST_DEBUG_FUNCPTR (gst_hls_demux_sink_event));
  gst_element_add_pad (GST_ELEMENT (demux), demux->sinkpad);

  /* Properties */
  demux->bitrate_limit = DEFAULT_BITRATE_LIMIT;
  demux->connection_speed = DEFAULT_CONNECTION_SPEED;
  /* BEGIN: Modified for DTS2015092408899/DTS2015092308532 by lizimian(l00251625), 2015/9/25 */
  demux->timeout_val = DEFAULT_TIMEOUT_TIME;
  /* END: Modified for DTS2015092408899/DTS2015092308532 by lizimian(l00251625), 2015/9/25 */

  /* BEGIN: Added for DTS2015092302576/DTS2015092303479 by lizimian(l00251625), 2015/10/6 */
  g_mutex_init (&demux->streams_lock);
  /* END: Added for DTS2015092302576/DTS2015092303479 by lizimian(l00251625), 2015/10/6 */

  demux->user_agent = NULL;
  demux->referer = NULL;
  demux->cookies = NULL;

  demux->all_pads_added = FALSE;

  GST_INFO ("hls demux init out.");
}

static void
gst_hls_demux_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstHLSDemux *demux = GST_HLS_DEMUX (object);
  gchar* parse_uri = NULL;
  GSList *streams = demux->streams;
  GSList *iter;

  switch (prop_id) {
    case PROP_BITRATE_LIMIT:
      demux->bitrate_limit = g_value_get_float (value);
      break;
    case PROP_CONNECTION_SPEED:
      demux->connection_speed = g_value_get_uint64 (value) * 1000;
      GST_INFO_OBJECT (demux, "Connection speed set to %u",
          demux->connection_speed);
      break;
    case PROP_TIMEOUT:
      demux->timeout_val = g_value_get_uint (value);
      GST_INFO ("hlsdemux set timeoutval = %u", demux->timeout_val);
      /* BEGIN: Modified for DTS2015092408899/DTS2015092308532 by lizimian(l00251625), 2015/9/25 */
      for (iter = streams; iter; iter = g_slist_next (iter)) {
        GstHLSDemuxStream *stream = iter->data;
        if (stream->downloader)
          g_object_set (stream->downloader, TIME_OUT_KEY, demux->timeout_val, NULL);
      }
      /* END: Modified for DTS2015092408899/DTS2015092308532 by lizimian(l00251625), 2015/9/25 */
      break;
    case PROP_SME_CACHE_PROC:
      parse_uri = g_strdup (g_value_get_string (value));
      if (NULL == parse_uri) {
         GST_ERROR ("no mem, strdup failed");
         break;
      }
      g_free (parse_uri);
      break;
    case PROP_USER_AGENT:
      if (demux->user_agent)
        g_free (demux->user_agent);
      demux->user_agent = g_value_dup_string (value);
      break;
    case PROP_REFERER:
      if (demux->referer)
        g_free (demux->referer);
      demux->referer = g_value_dup_string (value);
      break;
    case PROP_COOKIES:
      g_strfreev (demux->cookies);
      demux->cookies = g_strdupv (g_value_get_boxed (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_hls_demux_get_property (GObject * object, guint prop_id, GValue * value,
    GParamSpec * pspec)
{
  GstHLSDemux *demux = GST_HLS_DEMUX (object);
  guint u_cur_bw = 0;
  GSList *streams = demux->streams;

  switch (prop_id) {
    case PROP_BITRATE_LIMIT:
      g_value_set_float (value, demux->bitrate_limit);
      break;
    case PROP_CONNECTION_SPEED:
      g_value_set_uint64 (value, demux->connection_speed / 1000);
      break;
    case PROP_TIMEOUT:
      g_value_set_uint (value, demux->timeout_val);
      break;
    case PROP_CUR_BW:
      if (streams) {
        GstHLSDemuxStream *stream = streams->data;
        if (stream && stream->downloader)
          g_object_get (stream->downloader, GST_SME_PROP_KEY_CUR_BW, &u_cur_bw, NULL);
      }
      g_value_set_uint (value, u_cur_bw);
      break;
    case PROP_USER_AGENT:
      g_value_set_string (value, demux->user_agent);
      break;
    case PROP_REFERER:
      g_value_set_string (value, demux->referer);
      break;
    case PROP_COOKIES:
      g_value_set_boxed (value, demux->cookies);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static GstStateChangeReturn
gst_hls_demux_change_state (GstElement * element, GstStateChange transition)
{
  GstStateChangeReturn ret;
  GstHLSDemux *demux = GST_HLS_DEMUX (element);

  switch (transition) {
    /* BEGIN: Modified for DTS2015110502261/DTS2015110502298 by lizimian(l00251625), 2015/11/09 */
    case GST_STATE_CHANGE_READY_TO_PAUSED:
      break;
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      gst_hls_demux_reset (demux, FALSE);
      break;
    /* END: Modified for DTS2015110502261/DTS2015110502298 by lizimian(l00251625), 2015/11/09 */
    default:
      break;
  }

  ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

  switch (transition) {
    case GST_STATE_CHANGE_READY_TO_PAUSED:
      break;
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      break;
    default:
      break;
  }
  return ret;
}

static GstHLSDemuxStream *
gst_hls_demux_create_stream (GstHLSDemux * demux, gboolean video)
{
  GstHLSDemuxStream *stream;

  GST_INFO ("hls demux create stream, video %d.", video);

  stream = g_new0 (GstHLSDemuxStream, 1);
  stream->parent = demux;
  /* BEGIN: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
  stream->downloader = gst_uri_downloader_new_with_parent (GST_ELEMENT(demux));
  /* END: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
  /* BEGIN: Modified for DTS2015092408899/DTS2015092308532 by lizimian(l00251625), 2015/9/25 */
  g_object_set (stream->downloader, TIME_OUT_KEY, demux->timeout_val, NULL);
  /* END: Modified for DTS2015092408899/DTS2015092308532 by lizimian(l00251625), 2015/9/25 */
  stream->do_typefind = TRUE;

  /* BEGIN: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
  stream->queue =
      gst_data_queue_new ((GstDataQueueCheckFullFunction) _check_queue_full,
      NULL, NULL, demux);
  /* END: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */

  /* Updates task */
  g_rec_mutex_init (&stream->updates_lock);
  stream->updates_task =
      gst_task_new ((GstTaskFunction) gst_hls_demux_updates_loop, stream, NULL);
  gst_task_set_lock (stream->updates_task, &stream->updates_lock);
  g_mutex_init (&stream->updates_timed_lock);

  /* Streaming task */
  g_rec_mutex_init (&stream->stream_lock);
  stream->stream_task =
      gst_task_new ((GstTaskFunction) gst_hls_demux_stream_loop, stream, NULL);
  gst_task_set_lock (stream->stream_task, &stream->stream_lock);

  stream->have_group_id = FALSE;
  stream->group_id = G_MAXUINT;

  /* BEGIN: Modified for SME-110 by wanghua(w00347020), 2015/8/7 */
  stream->end_of_playlist = FALSE;
  stream->cancelled= FALSE;
  /* END: Modified for SME-110 by wanghua(w00347020), 2015/8/7 */

  stream->first_cache_pts = G_MAXUINT64;
  stream->last_timeout_time = G_MAXUINT64;

  stream->video = video;
  stream->pad_switched = FALSE;
  stream->inited = FALSE;

  return stream;
}

/* BEGIN: Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */
static gboolean
gst_hls_demux_setup_all_streams(GstHLSDemux * demux)
{
  GstHLSDemuxStream *stream;
  gboolean video = TRUE;
  gboolean audio = FALSE;

  /* BEGIN: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */
  demux->need_header = FALSE;
  /* END: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */

  // first setup main stream, usually it would be the video stream
  stream = gst_hls_demux_create_stream (demux, video);
  demux->streams = g_slist_append (demux->streams, stream);

  /* setup multi audio/subtitle streams if any */
  /* only setup the first of them for now */
  if (demux->client->current_audio) {
    stream = gst_hls_demux_create_stream (demux, audio);
    demux->streams = g_slist_append (demux->streams, stream);
  }

  GST_INFO ("hls demux add %d streams", g_slist_length (demux->streams));

  return TRUE;
}
/* END:   Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */

static gboolean
gst_hls_demux_src_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
  GstHLSDemux *demux = GST_HLS_DEMUX (parent);
  GSList *streams = demux->streams;
  GSList *iter;

  switch (event->type) {
    case GST_EVENT_SEEK:
    {
      gdouble rate;
      GstFormat format;
      GstSeekFlags flags;
      GstSeekType start_type, stop_type;
      gint64 start, stop;
      GList *walk, *walk_audio = NULL;
      GstClockTime position, current_pos, target_pos;
      GstClockTime position_audio, current_pos_audio, target_pos_audio = 0;
      gint current_sequence;
      gint current_sequence_audio = 0;
      GstM3U8MediaFile *file, *file_audio;
      gboolean video = TRUE;
      gboolean audio = FALSE;
      gboolean reset_time = TRUE;

      GST_INFO_OBJECT (demux, "Received GST_EVENT_SEEK");

      if (gst_m3u8_client_is_live (demux->client)) {
        GST_WARNING_OBJECT (demux, "Received seek event for live stream");
        return FALSE;
      }

      gst_event_parse_seek (event, &rate, &format, &flags, &start_type, &start,
          &stop_type, &stop);

      if (format != GST_FORMAT_TIME)
        return FALSE;

      GST_INFO_OBJECT (demux, "seek event, rate: %f start: %" GST_TIME_FORMAT
          " stop: %" GST_TIME_FORMAT, rate, GST_TIME_ARGS (start),
          GST_TIME_ARGS (stop));

      GST_M3U8_CLIENT_LOCK (demux->client);

      file = GST_M3U8_MEDIA_FILE (demux->client->current->files->data);
      current_sequence = file->sequence;
      current_pos = 0;
      target_pos = (GstClockTime) start;
      for (walk = demux->client->current->files; walk; walk = walk->next) {
        file = walk->data;

        current_sequence = file->sequence;
        if (current_pos <= target_pos
            && target_pos < current_pos + file->duration) {
          break;
        }
        current_pos += file->duration;
      }

      /* BEGIN: Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */
      /* BEGIN: Modified for DTS2015102708202 by lizimian(l00251625), 2015/10/28 */
      if (demux->client->current_audio &&
          demux->client->current_audio->files) {
      /* END: Modified for DTS2015102708202 by lizimian(l00251625), 2015/10/28 */
        file_audio = GST_M3U8_MEDIA_FILE (demux->client->current_audio->files->data);
        current_sequence_audio = file_audio->sequence;
        current_pos_audio = 0;
        target_pos_audio = (GstClockTime) start;
        for (walk_audio = demux->client->current_audio->files; walk_audio;
            walk_audio = walk_audio->next) {
          file_audio = walk_audio->data;

          current_sequence_audio = file_audio->sequence;
          if (current_pos_audio <= target_pos_audio
              && target_pos_audio < current_pos_audio + file_audio->duration) {
            break;
          }
          current_pos_audio += file_audio->duration;
        }
      }
      /* END: Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */
      GST_M3U8_CLIENT_UNLOCK (demux->client);

      /* BEGIN: Modified for DTS2014081802636 by z00213950 2014/08/19 */
      if (walk == NULL && target_pos != gst_m3u8_client_get_duration (demux->client, video)) {
        GST_WARNING_OBJECT (demux, "Could not find seeked video fragment");
        demux->client->sequence = current_sequence;
      }
      /* END: Modified for DTS2014081802636 by z00213950 2014/08/19 */
      if (demux->client->current_audio &&
          walk_audio == NULL &&
          target_pos_audio != gst_m3u8_client_get_duration (demux->client, audio)) {
        GST_WARNING_OBJECT (demux, "Could not find seeked audio fragment");
        demux->client->sequence_audio = current_sequence_audio;
      }

      for (iter = streams; iter; iter = g_slist_next (iter)) {
        GstHLSDemuxStream *stream = iter->data;

        if (flags & GST_SEEK_FLAG_FLUSH) {
          GST_INFO_OBJECT (demux, "sending flush start");
          gst_pad_push_event (stream->srcpad, gst_event_new_flush_start ());
        }
      }

      for (iter = demux->streams; iter; iter = g_slist_next (iter)) {
        GstHLSDemuxStream *stream = iter->data;
        /* BEGIN: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
        gst_data_queue_set_flushing (stream->queue, TRUE);
        /* END: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
      }

      for (iter = streams; iter; iter = g_slist_next (iter)) {
        GstHLSDemuxStream *stream = iter->data;

        stream->cancelled = TRUE;
        gst_task_pause (stream->stream_task);
        gst_uri_downloader_cancel (stream->downloader);
        gst_task_stop (stream->updates_task);
        g_mutex_lock (&stream->updates_timed_lock);
        GST_TASK_SIGNAL (stream->updates_task);
        g_mutex_unlock (&stream->updates_timed_lock);
        g_rec_mutex_lock (&stream->updates_lock);
        g_rec_mutex_unlock (&stream->updates_lock);
        gst_task_pause (stream->stream_task);

        /* wait for streaming to finish */
        g_rec_mutex_lock (&stream->stream_lock);

        /* BEGIN: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
        gst_data_queue_flush (stream->queue);
        /* END: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
      }

      GST_M3U8_CLIENT_LOCK (demux->client);
      GST_INFO_OBJECT (demux, "seeking to video sequence %d", current_sequence);
      demux->client->sequence = current_sequence;
      gst_m3u8_client_get_current_position (demux->client, &position, video);

      /* BEGIN: Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */
      if (demux->client->current_audio) {
        demux->client->sequence_audio = current_sequence_audio;
        gst_m3u8_client_get_current_position (demux->client, &position_audio, audio);
      }
      /* END: Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */

      for (iter = streams; iter; iter = g_slist_next (iter)) {
        GstHLSDemuxStream *stream = iter->data;
        stream->need_segment = TRUE;
        stream->first_cache_pts = G_MAXUINT64;
      }
      GST_M3U8_CLIENT_UNLOCK (demux->client);

      for (iter = streams; iter; iter = g_slist_next (iter)) {
        GstHLSDemuxStream *stream = iter->data;
        if (flags & GST_SEEK_FLAG_FLUSH) {
          GST_INFO_OBJECT (stream, "sending flush stop");
          gst_pad_push_event (stream->srcpad, gst_event_new_flush_stop (reset_time));
        }
      }
      for (iter = demux->streams; iter; iter = g_slist_next (iter)) {
        GstHLSDemuxStream *stream = iter->data;
        /* BEGIN: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
        gst_data_queue_set_flushing (stream->queue, FALSE);
        /* END: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
      }

      for (iter = streams; iter; iter = g_slist_next (iter)) {
        GstHLSDemuxStream *stream = iter->data;

        stream->cancelled = FALSE;
        /* BEGIN: Modified for SME-110 by wanghua(w00347020), 2015/8/7 */
        stream->end_of_playlist = FALSE;
        /* END: Modified for SME-110 by wanghua(w00347020), 2015/8/7 */

        stream->inited = FALSE;

        gst_uri_downloader_reset (stream->downloader);
        gst_task_start (stream->updates_task);
        g_rec_mutex_unlock (&stream->stream_lock);
      }

      return TRUE;
    }
    default:
      break;
  }

  return gst_pad_event_default (pad, parent, event);
}

static void
gst_hls_demux_free_bitrate_info(GSTHLSDemuxBitrateInfo *bitrate_info)
{
  if (bitrate_info && bitrate_info->video_bitrate_list) {
    g_free (bitrate_info->video_bitrate_list);
    bitrate_info->video_bitrate_list = NULL;
    bitrate_info->video_bitrates_num = 0;
  }

   if (bitrate_info && bitrate_info->audio_bitrate_list) {
    g_free (bitrate_info->audio_bitrate_list);
    bitrate_info->audio_bitrate_list = NULL;
    bitrate_info->audio_bitrate_num = 0;
   }
}

static gboolean
gst_hls_demux_get_bitrate_info (GstHLSDemux *hlsdemux,
  GSTHLSDemuxBitrateInfo *bitrate_info)
{
  GSList *iter;
  gint idx = 0;
  GList *rep_list = NULL;
  GList *list;
  GstM3U8 *representation;

  for (iter = hlsdemux->streams; iter; iter = g_slist_next (iter)) {
    GstHLSDemuxStream *stream = iter->data;
    if (stream->video) {
      if (hlsdemux->client->main)
        rep_list = hlsdemux->client->main->lists;
      if (!rep_list) {
        continue;
      }

      bitrate_info->video_bitrates_num = g_list_length (rep_list);
      GST_INFO_OBJECT (hlsdemux, "There are %d bitrates for video",
        bitrate_info->video_bitrates_num);
      if (bitrate_info->video_bitrates_num > 0) {
        bitrate_info->video_bitrate_list = g_malloc0 (
          sizeof(guint64) * bitrate_info->video_bitrates_num);
        if (!bitrate_info->video_bitrate_list) {
          GST_WARNING ("malloc video_bitrate_list failed. GST_STREAM_VIDEO");
          continue;
        }
        memset (bitrate_info->video_bitrate_list, 0,
          sizeof(guint64) * bitrate_info->video_bitrates_num);
        idx = 0;
        for (list = g_list_first (hlsdemux->client->main->lists); list; list = g_list_next (list)) {
          representation = GST_M3U8 (list->data);
          bitrate_info->video_bitrate_list[idx] = (guint64)representation->bandwidth;
          idx++;
          GST_INFO_OBJECT (hlsdemux, "video bitrates[%d]:%d ",
            idx, representation->bandwidth);
        }
      }
    } else {
      if (hlsdemux->client->main->group_audio)
        rep_list = hlsdemux->client->main->group_audio->lists;
      if (!rep_list) {
        continue;
      }

      bitrate_info->audio_bitrate_num = g_list_length (rep_list);

      GST_INFO_OBJECT (hlsdemux, "There are %d bitrates for audio",
        bitrate_info->audio_bitrate_num);
      if (bitrate_info->audio_bitrate_num > 0) {
        bitrate_info->audio_bitrate_list = g_malloc0 (
          sizeof(guint64) * bitrate_info->audio_bitrate_num);
        if (!bitrate_info->audio_bitrate_list) {
          GST_WARNING ("malloc audio_bitrate_list failed. GST_STREAM_AUDIO");
          continue;
        }
        memset (bitrate_info->audio_bitrate_list, 0,
          sizeof(guint64) * bitrate_info->audio_bitrate_num);
        idx = 0;
        for (list = g_list_first (hlsdemux->client->main->group_audio->lists); list; list = g_list_next (list)) {
          representation = GST_M3U8 (list->data);
          bitrate_info->audio_bitrate_list[idx] = (guint64) representation->bandwidth;
          idx++;
          GST_INFO_OBJECT (hlsdemux, "audio bitrates[%d]:%d ",
            idx, representation->bandwidth);
        }
      }
    }
  }

  if (bitrate_info->audio_bitrate_num || bitrate_info->video_bitrates_num)
    return TRUE;
  else
    return FALSE;
}

static gboolean
gst_hls_demux_sink_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
  GstHLSDemux *demux;
  GstQuery *query;
  gboolean ret;
  gchar *uri;
  GSList *iter;
  gboolean video = TRUE;
  GSTHLSDemuxBitrateInfo stream_bitrate_info;
  GstHLSDemuxStartupMode startup_mode;
  GList *current_variant;

  demux = GST_HLS_DEMUX (parent);

  switch (event->type) {
    case GST_EVENT_EOS: {
      gchar *playlist = NULL;

      if (demux->playlist == NULL) {
        GST_WARNING_OBJECT (demux, "Received EOS without a playlist.");
        break;
      }

      GST_INFO_OBJECT (demux,
          "Got EOS on the sink pad: main playlist fetched");

      query = gst_query_new_uri ();
      ret = gst_pad_peer_query (demux->sinkpad, query);
      if (ret) {
        gst_query_parse_uri_redirection (query, &uri);
        if (uri == NULL)
          gst_query_parse_uri (query, &uri);
        gst_hls_demux_set_location (demux, uri);
        g_free (uri);
      }
      gst_query_unref (query);

      playlist = gst_hls_src_buf_to_utf8_playlist (demux->playlist);
      demux->playlist = NULL;
      /* BEGIN: Modified for DTS2015110905847  by wanghua(w00347020), 2015/11/12 */
      if (playlist == NULL || !gst_m3u8_client_update (demux->client, playlist, video)) {
      /* END: Modified for DTS2015110905847  by wanghua(w00347020), 2015/11/12 */
        /* In most cases, this will happen if we set a wrong url in the
         * source element and we have received the 404 HTML response instead of
         * the playlist */
        GST_ELEMENT_ERROR (demux, STREAM, DECODE, ("Invalid playlist."),
            (NULL));
        return FALSE;
      }

      gst_hls_demux_setup_all_streams(demux);

      /* get the startup mode through callback, default action:startup from
       * the lowest bitrate and do adaptive according to bandwidth */
      startup_mode.startup_bitrate = 0;
      startup_mode.is_adaptive = TRUE;
      memset (&stream_bitrate_info, 0, sizeof (GSTHLSDemuxBitrateInfo));

      if (gst_hls_demux_get_bitrate_info (demux, &stream_bitrate_info)) {
        GST_INFO_OBJECT (demux, "Send to user,video bitrate n:%u,auido bitrate n:%u",
          stream_bitrate_info.video_bitrates_num, stream_bitrate_info.audio_bitrate_num);
        g_signal_emit (GST_ELEMENT (demux),
          gst_hls_demux_signals[SIGNAL_M3U8_PARSE_COMPLETE], 0,
          &stream_bitrate_info,&startup_mode);

        GST_INFO_OBJECT (demux, "Got User feedback:startup bitrate:%llu, isadaptive:%d",
          startup_mode.startup_bitrate, startup_mode.is_adaptive);
        demux->startup_bitrate = startup_mode.startup_bitrate;

        /* TODO:audio stream bitrate usage is negligible as compared to the video
         * stream one,but we need to consider it soon*/

        if (demux->startup_bitrate) {
          for (iter = demux->streams; iter; iter = g_slist_next (iter)) {
            GstHLSDemuxStream *stream = iter->data;
            if (stream->video) {
              current_variant = gst_m3u8_client_get_playlist_for_bitrate (
                demux->client, demux->startup_bitrate, stream->video);
              demux->client->main->current_variant = current_variant;
            }
          }
        }

        if (!startup_mode.is_adaptive)
          demux->connection_speed = startup_mode.startup_bitrate;

        gst_hls_demux_free_bitrate_info(&stream_bitrate_info);
      }

      if (!ret && gst_m3u8_client_is_live (demux->client)) {
        GST_ELEMENT_ERROR (demux, RESOURCE, NOT_FOUND,
          ("Failed querying the playlist uri, required for live sources."),
          (NULL));
        return FALSE;
      }

      for (iter = demux->streams; iter; iter = g_slist_next (iter)) {
        GstHLSDemuxStream *stream = iter->data;
        if (stream->downloader && !gst_m3u8_client_is_live (demux->client)) {
          /* BEGIN: Modified for DTS2015092408899/DTS2015092308532 by lizimian(l00251625), 2015/9/25 */
          g_object_set (stream->downloader, TIME_OUT_KEY, demux->timeout_val,
              USER_AGENT, demux->user_agent,
              REFERER, demux->referer, NULL);
          if (demux->cookies)
            g_object_set (stream->downloader, COOKIES, demux->cookies, NULL);
          /* END: Modified for DTS2015092408899/DTS2015092308532 by lizimian(l00251625), 2015/9/25 */
        }
      }

      /* BEGIN: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */
      demux->need_header = TRUE;
      /* END: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */
      for (iter = demux->streams; iter; iter = g_slist_next (iter)) {
        GstHLSDemuxStream *stream = iter->data;
        gst_task_start (stream->updates_task);
      }
      gst_event_unref (event);
      return TRUE;
    }
    case GST_EVENT_SEGMENT:
      /* Swallow newsegments, we'll push our own */
      gst_event_unref (event);
      return TRUE;
    default:
      break;
  }

  return gst_pad_event_default (pad, parent, event);
}

static gboolean
gst_hls_demux_src_query (GstPad * pad, GstObject * parent, GstQuery * query)
{
  GstHLSDemux *hlsdemux;
  gboolean ret = FALSE;
  gboolean video = TRUE;

  pad = pad;

  if (query == NULL)
    return FALSE;

  hlsdemux = GST_HLS_DEMUX (parent);

  switch (query->type) {
    case GST_QUERY_DURATION:{
      GstClockTime duration = 0;
      GstFormat fmt;

      gst_query_parse_duration (query, &fmt, NULL);
      if (fmt == GST_FORMAT_TIME) {
        /* BEGIN: Modified for DTS2015110206627 by lizimian(l00251625), 2015/11/2 */
        duration = hlsdemux->client->duration_video;
        /* END: Modified for DTS2015110206627 by lizimian(l00251625), 2015/11/2 */
        if (GST_CLOCK_TIME_IS_VALID (duration) && duration > 0) {
          gst_query_set_duration (query, GST_FORMAT_TIME, (gint64) duration);
          ret = TRUE;
        }
      }
      GST_INFO_OBJECT (hlsdemux, "GST_QUERY_DURATION returns %s with duration %"
          GST_TIME_FORMAT, ret ? "TRUE" : "FALSE", GST_TIME_ARGS (duration));
      break;
    }
    case GST_QUERY_URI:
      if (hlsdemux->client) {
        /* FIXME: Do we answer with the variant playlist, with the current
         * playlist or the the uri of the least downlowaded fragment? */
        gst_query_set_uri (query, gst_m3u8_client_get_uri (hlsdemux->client));
        ret = TRUE;
      }
      break;
    case GST_QUERY_SEEKING:{
      GstFormat fmt;
      gint64 stop = -1;
      gboolean live = TRUE;

      gst_query_parse_seeking (query, &fmt, NULL, NULL, NULL);
      GST_INFO_OBJECT (hlsdemux, "Received GST_QUERY_SEEKING with format %d",
          fmt);
      if (fmt == GST_FORMAT_TIME) {
        GstClockTime duration;

        duration = gst_m3u8_client_get_duration (hlsdemux->client, video);
        if (GST_CLOCK_TIME_IS_VALID (duration) && duration > 0)
          stop = (gint64) duration;

        live = !gst_m3u8_client_is_live (hlsdemux->client);
        gst_query_set_seeking (query, fmt, live, (gint64) 0, stop);
        ret = TRUE;
        GST_INFO_OBJECT (hlsdemux, "GST_QUERY_SEEKING returning with stop : %"
            GST_TIME_FORMAT, GST_TIME_ARGS (stop));
      }
      break;
    }
    default:
      /* Don't fordward queries upstream because of the special nature of this
       * "demuxer", which relies on the upstream element only to be fed with the
       * first playlist */
      break;
  }

  return ret;
}

static GstFlowReturn
gst_hls_demux_chain (GstPad * pad, GstObject * parent, GstBuffer * buf)
{
  GstHLSDemux *demux = GST_HLS_DEMUX (parent);
  pad = pad;

  if (demux->playlist == NULL)
    demux->playlist = buf;
  else
    demux->playlist = gst_buffer_append (demux->playlist, buf);

  return GST_FLOW_OK;
}

static void
gst_hls_demux_pause_tasks (GstHLSDemuxStream * stream, gboolean caching)
{
  if (GST_TASK_STATE (stream->updates_task) != GST_TASK_STOPPED) {
    stream->cancelled = TRUE;
    gst_uri_downloader_cancel (stream->downloader);
    gst_task_pause (stream->updates_task);
    if (!caching)
      g_mutex_lock (&stream->updates_timed_lock);
    GST_TASK_SIGNAL (stream->updates_task);
    if (!caching)
      g_mutex_unlock (&stream->updates_timed_lock);
  }

  if (GST_TASK_STATE (stream->stream_task) != GST_TASK_STOPPED)
    gst_task_pause (stream->stream_task);
}

static void
gst_hls_demux_stop (GstHLSDemux * demux)
{
  GSList *streams = demux->streams;
  GSList *iter;

  GST_INFO ("hls demux stop.");

  for (iter = demux->streams; iter; iter = g_slist_next (iter)) {
    GstHLSDemuxStream *stream = iter->data;
    /* BEGIN: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
    gst_data_queue_set_flushing (stream->queue, TRUE);
    /* END: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
  }

  for (iter = demux->streams; iter; iter = g_slist_next (iter)) {
    GstHLSDemuxStream *stream = iter->data;
    if (stream->downloader)
      gst_uri_downloader_cancel (stream->downloader);
  }

  for (iter = streams; iter; iter = g_slist_next (iter)) {
    GstHLSDemuxStream *stream = iter->data;

    if (GST_TASK_STATE (stream->stream_task) != GST_TASK_STOPPED) {
      gst_task_stop (stream->stream_task);
      g_rec_mutex_lock (&stream->stream_lock);
      g_rec_mutex_unlock (&stream->stream_lock);
      gst_task_join (stream->stream_task);
    }

    if (GST_TASK_STATE (stream->updates_task) != GST_TASK_STOPPED) {
      stream->cancelled = TRUE;
      gst_task_stop (stream->updates_task);
      g_mutex_lock (&stream->updates_timed_lock);
      GST_TASK_SIGNAL (stream->updates_task);
      g_mutex_unlock (&stream->updates_timed_lock);
      g_rec_mutex_lock (&stream->updates_lock);
      g_rec_mutex_unlock (&stream->updates_lock);
      gst_task_join (stream->updates_task);
    }
  }

  for (iter = demux->streams; iter; iter = g_slist_next (iter)) {
    GstHLSDemuxStream *stream = iter->data;
    /* BEGIN: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
    gst_data_queue_flush (stream->queue);
    /* END: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
  }

  GST_INFO ("hls demux stop done.");
}

static void
gst_hls_demux_create_src_pads (GstHLSDemuxStream * stream, GstCaps * newcaps)
{
  GstHLSDemux *demux = stream->parent;
  GstEvent *event, *event_caps;
  gchar *stream_id;
  GstCaps *caps = NULL;
  gchar *caps_string = NULL;
  gboolean active = TRUE;

  GST_INFO ("Creating pad with caps: %" GST_PTR_FORMAT, newcaps);

  /* First create and activate new pad */
  stream->srcpad = gst_pad_new_from_static_template (&srctemplate, NULL);
  gst_pad_set_event_function (stream->srcpad,
      GST_DEBUG_FUNCPTR (gst_hls_demux_src_event));
  gst_pad_set_query_function (stream->srcpad,
      GST_DEBUG_FUNCPTR (gst_hls_demux_src_query));
  gst_pad_set_element_private (stream->srcpad, stream);
  gst_pad_set_active (stream->srcpad, active);

  stream_id =
      gst_pad_create_stream_id (stream->srcpad, GST_ELEMENT_CAST (stream), NULL);

  event = gst_pad_get_sticky_event (demux->sinkpad, GST_EVENT_STREAM_START, 0);
  if (event) {
    if (gst_event_parse_group_id (event, &stream->group_id))
      stream->have_group_id = TRUE;
    else
      stream->have_group_id = FALSE;
    gst_event_unref (event);
  } else if (!stream->have_group_id) {
    stream->have_group_id = TRUE;
    stream->group_id = gst_util_group_id_next ();
  }
  event = gst_event_new_stream_start (stream_id);
  if (stream->have_group_id)
    gst_event_set_group_id (event, stream->group_id);

  gst_pad_push_event (stream->srcpad, event);
  g_free (stream_id);

  gst_pad_set_caps (stream->srcpad, newcaps);

  /* BEGIN: Modified for DTS2015072310170 by lizimian(l00251625), 2015/7/28 */
  caps_string = gst_caps_to_string (newcaps);
  caps = gst_caps_new_simple (caps_string,
              "variant", G_TYPE_STRING, "hls-fragmented", NULL);
  g_free (caps_string);
  event_caps = gst_event_new_caps(caps);
  gst_pad_push_event (stream->srcpad, event_caps);
  gst_caps_unref (caps);
  /* END:   Modified for DTS2015072310170 by lizimian(l00251625), 2015/7/28 */

  /* BEGIN: Added for DTS2015092302576/DTS2015092303479 by lizimian(l00251625), 2015/10/6 */
  g_mutex_lock (&demux->streams_lock);
  stream->pad_switched = TRUE;
  g_mutex_unlock (&demux->streams_lock);
  /* END: Added for DTS2015092302576/DTS2015092303479 by lizimian(l00251625), 2015/10/6 */
}

static guint64
gst_hls_demux_stream_update_current_bitrate (GstHLSDemuxStream * stream)
{
  gint i;
  guint64 current_download_rate = 0;
  guint64 average_download_rate = 0;
  GstHLSDemux *demux = stream->parent;

  current_download_rate =
      (guint64) ((stream->fragment_total_size * 8 * demux->bitrate_limit) /
      ((double) stream->fragment_total_time / GST_SECOND));
  GST_INFO ("fragment_size:%llu, time:%llu, download rate:%llu",
    stream->fragment_total_size, stream->fragment_total_time,
    current_download_rate);
  stream->fragment_total_size = 0;
  stream->fragment_total_time = 0;

  if (stream->download_rate_num == GST_HLS_DOWNLOAD_RATE_WINDOW) {
    for(i = 0; i < GST_HLS_DOWNLOAD_RATE_WINDOW - 1; i++)
      stream->latest_download_rate [i] = stream->latest_download_rate [i + 1];
    stream->download_rate_num = GST_HLS_DOWNLOAD_RATE_WINDOW - 1;
  }
  stream->latest_download_rate[stream->download_rate_num] = current_download_rate;
  stream->download_rate_num++;

  for (i = 0; i < stream->download_rate_num; i++)
    average_download_rate += stream->latest_download_rate[i];
  stream->current_download_rate = average_download_rate / (guint64) stream->download_rate_num;

  GST_INFO ("download rate %llu", stream->current_download_rate);
  return stream->current_download_rate;
}

static guint64
gst_hls_demux_get_item_priv(GstDataQueueItem *item)
{
  if (item && item->_gst_reserved[0])
      return *(guint64*)(item->_gst_reserved[0]);
  else
    return 0;
}

static void
gst_hls_demux_stream_loop (GstHLSDemuxStream * stream)
{
  GstBuffer *buf;
  GstFlowReturn ret;
  GstCaps *srccaps = NULL;
  GstHLSDemux *demux = stream->parent;
  GstDataQueueItem *item = NULL;
  gboolean all_pads_added = TRUE;
  GSList *streams = demux->streams;
  GSList *iter;

  /* Loop for the source pad task. The task is started when we have
   * received the main playlist from the source element. It tries first to
   * cache the first fragments and then it waits until it has more data in the
   * queue. This task is woken up when we push a new fragment to the queue or
   * when we reached the end of the playlist  */
  GST_DEBUG_OBJECT (stream, "Enter task");

  if (gst_data_queue_is_empty (stream->queue)) {
    /* BEGIN: Modified for SME-110 by wanghua(w00347020), 2015/8/7 */
    if (stream->end_of_playlist && gst_uri_downloader_can_eos (stream->downloader))
      goto end_of_playlist;
    /* END: Modified for SME-110 by wanghua(w00347020), 2015/8/7 */

    goto pause_task;
  }

  /* BEGIN: Modified for DTS2015110206394 by lizimian(l00251625), 2015/11/3 */
  if (!gst_data_queue_peek (stream->queue, &item)) {
    /* flushing */
    goto flushing;
  }
  /* END: Modified for DTS2015110206394 by lizimian(l00251625), 2015/11/3 */

  /* Figure out if we need to create/switch pads */
  if (G_LIKELY (stream->srcpad))
    srccaps = gst_pad_get_current_caps (stream->srcpad);

  if (G_UNLIKELY (!srccaps)) {
    gst_hls_demux_create_src_pads (stream, stream->input_caps);
    stream->need_segment = TRUE;
  }

  if (G_LIKELY (srccaps))
    gst_caps_unref (srccaps);

  g_mutex_lock (&demux->streams_lock);
  if (!demux->all_pads_added) {
    for (iter = streams; iter; iter = g_slist_next (iter)) {
      GstHLSDemuxStream *tmp_stream = iter->data;
      all_pads_added = all_pads_added & tmp_stream->pad_switched;
    }

    if (all_pads_added) {
      for (iter = streams; iter; iter = g_slist_next (iter)) {
        GstHLSDemuxStream *tmp_stream = iter->data;
        gst_element_add_pad (GST_ELEMENT (demux), tmp_stream->srcpad);
      }
      gst_element_no_more_pads (GST_ELEMENT (demux));
      demux->all_pads_added = TRUE;
      GST_INFO ("hls demux emit no-more-pads");
    } else {
      g_mutex_unlock (&demux->streams_lock);
      goto end;
    }
  }
  g_mutex_unlock (&demux->streams_lock);

  /* BEGIN: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
  gst_data_queue_pop (stream->queue, &item);
  /* END: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */

  if (!item)
    goto end;

  if (GST_IS_BUFFER (item->object)) {
    buf = GST_BUFFER_CAST (item->object);

    if (stream->last_notify_bitrate != gst_hls_demux_get_item_priv(item)) {
      GstTagList *tag_list = NULL;
      tag_list = gst_tag_list_new_empty ();
      if (!tag_list)
        goto end;
      gst_tag_list_add (tag_list, GST_TAG_MERGE_REPLACE, GST_TAG_BITRATE,
        (guint) gst_hls_demux_get_item_priv(item), NULL);
      GST_INFO ("bitrate change from %lld to %lld",
        stream->last_notify_bitrate, gst_hls_demux_get_item_priv(item));
      gst_pad_push_event (stream->srcpad, gst_event_new_tag (tag_list));
      stream->last_notify_bitrate = gst_hls_demux_get_item_priv(item);
    }

    if (stream->need_segment) {
      GstSegment segment;
      GstClockTime start = (stream->first_cache_pts == G_MAXUINT64) \
          ? GST_BUFFER_PTS (buf) : stream->first_cache_pts;

      /* And send a newsegment */
      GST_INFO_OBJECT (stream, "Sending new-segment. segment start:%"
        GST_TIME_FORMAT, GST_TIME_ARGS (start));
      gst_segment_init (&segment, GST_FORMAT_TIME);
      segment.start = start;
      segment.time = start;
      segment.position = start;
      gst_pad_push_event (stream->srcpad, gst_event_new_segment (&segment));
      stream->need_segment = FALSE;
      stream->first_cache_pts = G_MAXUINT64;
    }

    GST_DEBUG_OBJECT (stream, "Pushing buffer %p", buf);

    ret = gst_pad_push (stream->srcpad, gst_buffer_ref (buf));
    item->destroy (item);
    if (ret != GST_FLOW_OK)
      goto error_pushing;

    GST_DEBUG_OBJECT (stream, "Pushed buffer");
  } else {
    GST_WARNING ("item isn't a fragment, just drop it");
    item->destroy (item);
  }

end:
  GST_DEBUG_OBJECT (stream, "leave task");
  return;

end_of_playlist:
  {
    GST_INFO_OBJECT (stream, "Reached end of playlist, sending EOS");
    gst_pad_push_event (stream->srcpad, gst_event_new_eos ());
    gst_hls_demux_pause_tasks (stream, FALSE);
    return;
  }

error_pushing:
  {
    if (ret == GST_FLOW_NOT_LINKED || ret < GST_FLOW_EOS) {
      GST_ELEMENT_ERROR (demux, STREAM, FAILED, (NULL),
          ("stream stopped, reason %s", gst_flow_get_name (ret)));
      gst_pad_push_event (stream->srcpad, gst_event_new_eos ());
    } else {
      GST_INFO_OBJECT (stream, "stream stopped, reason %s",
          gst_flow_get_name (ret));
    }
    gst_hls_demux_pause_tasks (stream, FALSE);
    return;
  }

/* BEGIN: Modified for DTS2015110206394 by lizimian(l00251625), 2015/11/3 */
flushing:
  {
    GST_WARNING_OBJECT (demux, "Flushing, leaving stream task");
    gst_task_stop (stream->stream_task);
    return;
  }
/* END: Modified for DTS2015110206394 by lizimian(l00251625), 2015/11/3 */

pause_task:
  {
    GST_DEBUG_OBJECT (stream, "Pause task");
    gst_task_pause (stream->stream_task);
    return;
  }
}

static void
gst_hls_demux_stream_free (GstHLSDemuxStream * stream)
{
  if (stream->input_caps) {
    gst_caps_unref (stream->input_caps);
    stream->input_caps = NULL;
  }

  if (stream->queue) {
    gst_object_unref (stream->queue);
    stream->queue = NULL;
  }

  if (stream->stream_task) {
    gst_object_unref (stream->stream_task);
    g_rec_mutex_clear (&stream->stream_lock);
    stream->stream_task = NULL;
  }

  if (stream->updates_task) {
    gst_object_unref (stream->updates_task);
    g_rec_mutex_clear (&stream->updates_lock);
    g_mutex_clear (&stream->updates_timed_lock);
    stream->updates_task = NULL;
  }

  if (stream->downloader) {
    gst_object_unref (stream->downloader);
    stream->downloader = NULL;
  }

  g_free (stream);
}

/**
 * gst_hls_demux_reset:
 * @demux: The #GstHLSDemux to reset
 * @dispose: whether to realloc a m3u8 client or not
 *
 * Reset @demux. This method will reset all properties in @demux,
 * also reset flags, unref caps and clear queue in streams,
 * but will not reset uri downloader and last timeout time of streams.
 *
 * NOT MT safe.
 */
static void
gst_hls_demux_reset (GstHLSDemux * demux, gboolean dispose)
{
  GSList *streams = demux->streams;
  GSList *iter;

  GST_INFO ("hls demux reset.");

  gst_hls_demux_stop (demux);

  for (iter = demux->streams; iter; iter = g_slist_next (iter)) {
    GstHLSDemuxStream *stream = iter->data;
    if (stream->downloader)
      gst_uri_downloader_reset (stream->downloader);
  }

  for (iter = streams; iter; iter = g_slist_next (iter)) {
    GstHLSDemuxStream *stream = iter->data;

    stream->cancelled = FALSE;
    stream->do_typefind = TRUE;
    /* BEGIN: Modified for SME-110 by wanghua(w00347020), 2015/8/7 */
    stream->end_of_playlist = FALSE;
    /* END: Modified for SME-110 by wanghua(w00347020), 2015/8/7 */

    stream->need_segment = TRUE;

    stream->have_group_id = FALSE;
    stream->group_id = G_MAXUINT;

    stream->first_cache_pts = G_MAXUINT64;

    if (stream->srcpad) {
      GST_INFO_OBJECT (demux, "Removing hls demux stream pad %s:%s",
          GST_DEBUG_PAD_NAME (stream->srcpad));
      gst_element_remove_pad (GST_ELEMENT (demux), stream->srcpad);
    }

    gst_hls_demux_stream_free (stream);
  }
  g_slist_free (demux->streams);
  demux->streams = NULL;

  if (demux->playlist) {
    gst_buffer_unref (demux->playlist);
    demux->playlist = NULL;
  }

  if (demux->client) {
    gst_m3u8_client_free (demux->client);
    demux->client = NULL;
  }

  if (!dispose) {
    demux->client = gst_m3u8_client_new ("", NULL);
  }

  GST_INFO ("hls demux reset done.");
}

static gboolean
gst_hls_demux_set_location (GstHLSDemux * demux, const gchar * uri)
{
  if (demux->client)
    gst_m3u8_client_free (demux->client);
  demux->client = gst_m3u8_client_new (uri, NULL);

  /* BEGIN: Modified for DTS2015102210493 by liurongliang(l00180035), 2015/10/23 */
  GST_DEBUG_OBJECT (demux, "Changed location .");
  /* END: Modified for DTS2015102210493 by liurongliang(l00180035), 2015/10/23 */

  return TRUE;
}

static gboolean
gst_hls_demux_init_stream (GstHLSDemuxStream * stream)
{
  GstHLSDemux *demux = stream->parent;

  /* If this playlist is a variant playlist, select the first one
   * and update it */
  if (gst_m3u8_client_has_variant_playlist (demux->client)) {
    GstM3U8 *child = NULL;

    if (demux->connection_speed == 0) {
      GST_M3U8_CLIENT_LOCK (demux->client);
      if (stream->video)
        child = GST_M3U8 (demux->client->main->current_variant->data);
      else
        child = GST_M3U8 (g_list_first (demux->client->main->group_audio->lists)->data);
      GST_M3U8_CLIENT_UNLOCK (demux->client);
    } else {
      GList *tmp = gst_m3u8_client_get_playlist_for_bitrate (demux->client,
          demux->connection_speed, stream->video);
      child = GST_M3U8 (tmp->data);
    }

    gst_m3u8_client_set_current (demux->client, child, stream->video);

    if (!gst_hls_demux_update_playlist (stream, FALSE)) {
      GST_ERROR_OBJECT (demux, "Could not fetch the child playlist");
      return FALSE;
    }
  }

  if (!gst_m3u8_client_is_live (demux->client)) {
    /* TODO: how to handle the condition when audio and video duration is not equal */
    GstClockTime duration = gst_m3u8_client_get_duration (demux->client, stream->video);

    /* BEGIN: Modified for DTS2015110206627 by lizimian(l00251625), 2015/11/2 */
    if (stream->video)
      demux->client->duration_video = duration;
    else
      demux->client->duration_audio = duration;
    /* END: Modified for DTS2015110206627 by lizimian(l00251625), 2015/11/2 */

    GST_DEBUG_OBJECT (demux, "Sending duration message : %" GST_TIME_FORMAT,
        GST_TIME_ARGS (duration));
    if (duration != GST_CLOCK_TIME_NONE)
      gst_element_post_message (GST_ELEMENT (demux),
          gst_message_new_duration_changed (GST_OBJECT (demux)));
  }

  stream->inited = TRUE;

  return TRUE;
}

void
gst_hls_demux_updates_loop (GstHLSDemuxStream * stream)
{
  /* Loop for the updates. It's started when the first fragments are cached and
   * schedules the next update of the playlist (for lives sources) and the next
   * update of fragments. When a new fragment is downloaded, it compares the
   * download time with the next scheduled update to check if we can or should
   * switch to a different bitrate */
  GstHLSDemux *demux = stream->parent;
  gboolean b_queue_empty;
  gboolean update = TRUE;
  gboolean caching = TRUE;
  gboolean init = FALSE;
  GstDataQueueSize queue_size;
  GstStructure *s;
  GstMessage *m;

  /* BEGIN: Modified for DTS2015111707747 by wanghua(w00347020), 2015/11/18 */
  if (!stream->inited) {
      init = gst_hls_demux_init_stream (stream);
    if (!init) {
      GST_ELEMENT_ERROR (demux, STREAM, DECODE, ("init failed, Invalid playlist."), (NULL));
      return;
    }
  }
  /* END: Modified for DTS2015111707747 by wanghua(w00347020), 2015/11/18 */

  /* block until the next scheduled update or the signal to quit this thread */
  g_mutex_lock (&stream->updates_timed_lock);
  GST_DEBUG_OBJECT (stream, "Started updates task");
  while (TRUE) {
    if (stream->cancelled)
      goto quit;

    /* BEGIN: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
    b_queue_empty = gst_data_queue_is_empty (stream->queue);
    gst_data_queue_get_level (stream->queue, &queue_size);
    /* END: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */

    if (queue_size.visible < MAX_FRAG_QUEUE_LENGTH && !stream->end_of_playlist) {
      gboolean ret = TRUE;
      GTimeVal now;
      GTimeVal start;

      memset(&now, 0, sizeof(GTimeVal));
      memset(&start, 0, sizeof(GTimeVal));

      g_get_current_time (&start);
      ret = gst_hls_demux_get_next_fragment (stream, FALSE);
      gst_wait_eos_event (stream->downloader);
      g_get_current_time (&now);

      if (stream->video && ret) {
        stream->fragment_total_time =
            (guint64) (GST_TIMEVAL_TO_TIME (now) - GST_TIMEVAL_TO_TIME (start));
        s = gst_structure_new (STATISTICS_MESSAGE_NAME,
                 "manifest-uri", G_TYPE_STRING,
                           demux->client->current->uri,
                 "uri", G_TYPE_STRING,
                           demux->next_frag_uri,
                 "fragment-start-time",GST_TYPE_CLOCK_TIME,
                           (guint64)GST_TIMEVAL_TO_TIME (start),
                 "fragment-stop-time", GST_TYPE_CLOCK_TIME,
                           (guint64)GST_TIMEVAL_TO_TIME (now),
                 "fragment-size", G_TYPE_UINT64,
                           stream->fragment_total_size,
                 "download-total-time",GST_TYPE_CLOCK_TIME,
                           stream->fragment_total_time,
                 NULL);
        m = gst_message_new_element (GST_OBJECT_CAST (demux),s);
        gst_element_post_message (GST_ELEMENT_CAST (demux),m);

        gst_hls_demux_stream_update_current_bitrate (stream);
        gst_element_post_message (GST_ELEMENT (demux),
            gst_message_new_new_bandwidth(GST_OBJECT (demux),
            (guint) stream->current_download_rate));
      }

      if (stream->cancelled) {
        goto quit;
      }

      /** BEGIN: Modified for DTS2015092509721 by lizimian(l00251625), 2015/10/13 */
      if (ret) {
        demux->client->update_failed_count = 0;
        /* update the playlist for live sources */
        if (gst_m3u8_client_is_live (demux->client)) {
          if (!gst_hls_demux_update_playlist (stream, update)) {
            if (stream->cancelled)
              goto quit;
            demux->client->update_failed_count++;
            if (demux->client->update_failed_count < DEFAULT_FAILED_COUNT) {
              GST_WARNING_OBJECT (stream, "Could not update the playlist");
              continue;
            } else {
              GST_ELEMENT_ERROR (demux, RESOURCE, NOT_FOUND,
                  ("Could not update the playlist"), (NULL));
              goto error;
            }
          }
        }
        gst_hls_demux_switch_playlist (stream);
        SLEEP_IF_NOT_CANCEL(stream, UPDATE_SLEEP_TIME * 5);
      } else if (stream->end_of_playlist) {
        goto quit;
      } else {
        demux->client->update_failed_count++;
        if (demux->client->update_failed_count < demux->client->current->filesequence) {
          GST_WARNING_OBJECT (stream, "Could not fetch the next fragment");
          gst_uri_downloader_reset (stream->downloader);
        } else {
          GST_ELEMENT_ERROR (demux, RESOURCE, NOT_FOUND,
            ("Could not fetch the next fragment"), (NULL));
          goto error;
        }
      }
      /** END: Modified for DTS2015092509721 by lizimian(l00251625), 2015/10/13 */
    } else {
      SLEEP_IF_NOT_CANCEL(stream, UPDATE_SLEEP_TIME);
    }
  }

quit:
  {
    GST_DEBUG_OBJECT (stream, "Stopped updates task");
    g_mutex_unlock (&stream->updates_timed_lock);
    return;
  }

error:
  {
    GST_INFO_OBJECT (stream, "Stopped updates task because of error");
    gst_hls_demux_pause_tasks (stream, caching);
    g_mutex_unlock (&stream->updates_timed_lock);
  }
}

static gchar *
gst_hls_src_buf_to_utf8_playlist (GstBuffer * buf)
{
  GstMapInfo info;
  gchar *playlist;

  if (!gst_buffer_map (buf, &info, GST_MAP_READ))
    goto map_error;

  if (!g_utf8_validate ((gchar *) info.data, (gint) info.size, NULL))
    goto validate_error;

  /* alloc size + 1 to end with a null character */
  playlist = g_malloc0 (info.size + 1);
  memcpy (playlist, info.data, info.size);

  gst_buffer_unmap (buf, &info);
  gst_buffer_unref (buf);
  return playlist;

validate_error:
  gst_buffer_unmap (buf, &info);
map_error:
  gst_buffer_unref (buf);
  return NULL;
}

static gboolean
gst_hls_demux_update_playlist (GstHLSDemuxStream * stream, gboolean update)
{
  GstFragment *download;
  GstBuffer *buf;
  gchar *playlist;
  gboolean updated = FALSE;
  GstHLSDemux *demux = NULL;
  const gchar *uri = NULL;
  GList *is_reversal = NULL;

  g_return_val_if_fail (stream != NULL, FALSE);
  demux = stream->parent;

  if (stream->video)
    g_return_val_if_fail (demux->client->current != NULL, FALSE);
  else
    g_return_val_if_fail (demux->client->current_audio != NULL, FALSE);

  uri = gst_m3u8_client_get_current_uri (demux->client, stream->video);

  /** BEGIN: Modified for DTS2014072906154 by liurongliang(l00180035), 2014/7/30 */
  download = gst_hls_downloader_fetch_uri_check_timeout(stream, uri);
  /** END:   Modified for DTS2014072906154 by liurongliang(l00180035), 2014/7/30 */

  if (download == NULL)
    return FALSE;

  buf = gst_fragment_get_buffer (download);
  playlist = gst_hls_src_buf_to_utf8_playlist (buf);
  gst_object_unref (download);

  if (playlist == NULL) {
    GST_WARNING_OBJECT (demux, "Couldn't not validate playlist encoding");
    return FALSE;
  }

  updated = gst_m3u8_client_update (demux->client, playlist, stream->video);

  /*  If it's a live source, do not let the sequence number go beyond
   * three fragments before the end of the list */
  is_reversal = g_list_find_custom (demux->client->current->files, MAX_SEQUENCE, _find_max_sequence);
  if (!is_reversal && updated && !update && gst_m3u8_client_is_live (demux->client)) {
    gint last_sequence;

    GST_M3U8_CLIENT_LOCK (demux->client);

    if (stream->video)
      last_sequence =
          GST_M3U8_MEDIA_FILE (g_list_last (demux->client->current->files)->data)->sequence;
    else
      last_sequence =
          GST_M3U8_MEDIA_FILE (g_list_last (demux->client->current_audio->files)->data)->sequence;

    if (demux->client->sequence >= last_sequence - 3) {
      GST_DEBUG_OBJECT (demux, "Sequence is beyond playlist. Moving back to %d",
          last_sequence - 3);
      stream->need_segment = TRUE;
      demux->client->sequence = last_sequence - 3;
    }
    GST_M3U8_CLIENT_UNLOCK (demux->client);
  }

  return updated;
}

static gboolean
gst_hls_demux_change_playlist (GstHLSDemuxStream * stream, guint64 max_bitrate, gboolean video)
{
  GList *previous_variant, *current_variant;
  gint old_bandwidth, new_bandwidth;
  GstHLSDemux *demux = stream->parent;

  /* If user specifies a connection speed never use a playlist with a bandwidth
   * superior than it */

  /* BEGIN: Modified for DTS2015092303415 by wanghua(w00347020), 2015/10/8*/
  if (demux->connection_speed != 0)
    max_bitrate = demux->connection_speed;
  /* END: Modified for DTS2015092303415 by wanghua(w00347020), 2015/10/8*/

  if (video)
    previous_variant = demux->client->main->current_variant;
  else
    previous_variant = g_list_first (demux->client->main->group_audio->lists);

  current_variant = gst_m3u8_client_get_playlist_for_bitrate (demux->client,
      max_bitrate, video);

retry_failover_protection:
  old_bandwidth = GST_M3U8 (previous_variant->data)->bandwidth;
  new_bandwidth = GST_M3U8 (current_variant->data)->bandwidth;

  /* Don't do anything else if the playlist is the same */
  if (new_bandwidth == old_bandwidth) {
    return TRUE;
  }

  GST_M3U8_CLIENT_LOCK (demux->client);
  demux->client->main->current_variant = current_variant;
  stream->cur_segment_bitrate = (guint64) new_bandwidth;
  GST_M3U8_CLIENT_UNLOCK (demux->client);

  gst_m3u8_client_set_current (demux->client, current_variant->data, stream->video);

  GST_INFO_OBJECT (demux, "Client was on %dbps, max allowed is %llubps, switching"
      " to bitrate %dbps", old_bandwidth, max_bitrate, new_bandwidth);

  if (gst_hls_demux_update_playlist (stream, FALSE)) {
    GstStructure *s;

    s = gst_structure_new ("playlist",
        "uri", G_TYPE_STRING, gst_m3u8_client_get_current_uri (demux->client, video),
        "bitrate", G_TYPE_INT, new_bandwidth, NULL);
    gst_element_post_message (GST_ELEMENT_CAST (demux),
        gst_message_new_element (GST_OBJECT_CAST (demux), s));
  } else {
    GList *failover = NULL;

    GST_INFO_OBJECT (demux, "Unable to update playlist. Switching back");
    GST_M3U8_CLIENT_LOCK (demux->client);

    failover = g_list_previous (current_variant);
    if (failover && new_bandwidth == GST_M3U8 (failover->data)->bandwidth) {
      current_variant = failover;
      GST_M3U8_CLIENT_UNLOCK (demux->client);
      goto retry_failover_protection;
    }

    demux->client->main->current_variant = previous_variant;
    GST_M3U8_CLIENT_UNLOCK (demux->client);

    gst_m3u8_client_set_current (demux->client, previous_variant->data, stream->video);
    /*  Try a lower bitrate (or stop if we just tried the lowest) */
    if (new_bandwidth ==
        GST_M3U8 (g_list_first (demux->client->main->lists)->data)->bandwidth)
      return FALSE;
    else
      return gst_hls_demux_change_playlist (stream, (guint64) new_bandwidth - 1, video);
  }
  /* Force typefinding since we might have changed media type */
  stream->do_typefind = TRUE;

  return TRUE;
}

static gboolean
_find_sequence (GstM3U8MediaFile * file, GstM3U8Client * client)
{
  if (client->sequence == file->sequence)
    return FALSE;
  else
    return TRUE;
}

static gboolean
gst_hls_demux_switch_playlist (GstHLSDemuxStream * stream)
{
  guint64 bitrate;
  GstHLSDemux *demux = stream->parent;

  GST_M3U8_CLIENT_LOCK (demux->client);
  if (!demux->client->main->lists) {
    GST_M3U8_CLIENT_UNLOCK (demux->client);
    return TRUE;
  }
  GST_M3U8_CLIENT_UNLOCK (demux->client);

  bitrate = stream->current_download_rate;
  if (demux->startup_bitrate && bitrate == 0)
    bitrate = demux->startup_bitrate;

  GST_INFO ("Downloaded bandwidth in %llu", bitrate);

  return gst_hls_demux_change_playlist (stream, bitrate, stream->video);
}

static GstFragment *
gst_hls_demux_decrypt_fragment (GstHLSDemuxStream * stream,
    GstFragment * encrypted_fragment, const gchar * key, const guint8 * iv)
{
  GstFragment *key_fragment, *ret = NULL;
  GstBuffer *key_buffer, *encrypted_buffer, *decrypted_buffer;
  GstMapInfo key_info, encrypted_info, decrypted_info;
  gnutls_cipher_hd_t aes_ctx;
  gnutls_datum_t key_d, iv_d;
  gsize unpadded_size;

  GST_INFO_OBJECT (stream, "Fetching key");

  /** BEGIN: Modified for DTS2014072906154 by liurongliang(l00180035), 2014/7/30 */
  key_fragment = gst_hls_downloader_fetch_uri_check_timeout(stream, key);
  /** END: Modified for DTS2014072906154 by liurongliang(l00180035), 2014/7/30 */

  if (key_fragment == NULL)
    goto key_failed;

  key_buffer = gst_fragment_get_buffer (key_fragment);
  encrypted_buffer = gst_fragment_get_buffer (encrypted_fragment);
  decrypted_buffer =
      gst_buffer_new_allocate (NULL, gst_buffer_get_size (encrypted_buffer),
      NULL);

  gst_buffer_map (key_buffer, &key_info, GST_MAP_READ);
  gst_buffer_map (encrypted_buffer, &encrypted_info, GST_MAP_READ);
  gst_buffer_map (decrypted_buffer, &decrypted_info, GST_MAP_WRITE);

  key_d.data = key_info.data;
  key_d.size = 16;
  iv_d.data = (unsigned char *) iv;
  iv_d.size = 16;
  gnutls_cipher_init (&aes_ctx, gnutls_cipher_get_id ("AES-128-CBC"), &key_d,
      &iv_d);
  gnutls_cipher_decrypt2 (aes_ctx, encrypted_info.data, encrypted_info.size,
      decrypted_info.data, decrypted_info.size);
  gnutls_cipher_deinit (aes_ctx);

  /* Handle pkcs7 unpadding here */
  unpadded_size =
      decrypted_info.size - decrypted_info.data[decrypted_info.size - 1];

  gst_buffer_unmap (decrypted_buffer, &decrypted_info);
  gst_buffer_unmap (encrypted_buffer, &encrypted_info);
  gst_buffer_unmap (key_buffer, &key_info);

  gst_buffer_resize (decrypted_buffer, 0, (gint) unpadded_size);

  gst_buffer_unref (key_buffer);
  gst_buffer_unref (encrypted_buffer);
  gst_object_unref (key_fragment);

  ret = gst_fragment_new ();
  gst_fragment_add_buffer (ret, decrypted_buffer);
  ret->completed = TRUE;
key_failed:
  gst_object_unref (encrypted_fragment);
  return ret;
}

static void
gst_hls_demux_set_item_priv(GstDataQueueItem *item, guint64 privdata)
{
  if (!item)
    return;

  if (!item->_gst_reserved[0])
    item->_gst_reserved[0] = g_malloc0(sizeof(guint64));

  if (item->_gst_reserved[0]) {
      *(guint64*)(item->_gst_reserved[0]) = privdata;
  }
}

/* BEGIN: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */
static gboolean
gst_hls_demux_stream_push_data (GstHLSDemuxStream * stream,
    GstBuffer * buffer, guint64 duration, guint size)
{
  GstDataQueueItem *item = g_new (GstDataQueueItem, 1);
  gboolean ret;

  item->object = GST_MINI_OBJECT_CAST (buffer);
  item->duration = duration;
  item->visible = TRUE;
  item->size = size;

  item->destroy = (GDestroyNotify) _hls_demux_data_queue_item_destroy;
  item->_gst_reserved[0] = NULL;
  gst_hls_demux_set_item_priv(item, stream->cur_segment_bitrate);

  ret = gst_data_queue_push (stream->queue, item);
  if (!ret) {
    GST_WARNING("Push data failed, destroy it");
    item->destroy (item);
    return FALSE;
  }

  return TRUE;
}
/* END: Modified for DTS2015102603354 by lizimian(l00251625), 2015/10/26 */

void gst_hls_demux_get_buffer_cb (void * hander, GstFragment * download)
{
  GstHLSDemuxStream *stream = (GstHLSDemuxStream *)hander;
  GstBuffer *buffer;
  guint64 duration;
  gsize buffer_size;
  GstHLSDemux *demux = stream->parent;

  if (!stream || !download)
    return;

  /* BEGIN: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */
  if (demux->need_header && demux->header) {
    GstBuffer *tmp_buffer = gst_fragment_get_buffer (download);
    demux->header->completed = FALSE;
    if (gst_fragment_add_buffer (demux->header, tmp_buffer)) {
      gst_object_unref (download);
      download = demux->header;
    } else {
      gst_buffer_unref (tmp_buffer);
    }
    demux->header->completed = TRUE;
    demux->need_header = FALSE;
  }
  /* END: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */

  if (G_UNLIKELY (stream->do_typefind)) {
    GstCaps *caps = gst_fragment_get_caps (download);

    if (!stream->input_caps || !gst_caps_is_equal (caps, stream->input_caps)) {
      gst_caps_replace (&stream->input_caps, caps);
      GST_INFO_OBJECT (stream, "Input source caps: %" GST_PTR_FORMAT, stream->input_caps);
      stream->do_typefind = FALSE;
    }
    gst_caps_unref (caps);
  }

  buffer = gst_fragment_get_buffer (download);
  /* BEGIN: Modified for DTS2015111102692 by lizimian(l00251625), 2015/11/10 */
  gst_object_unref (download);
  /* END: Modified for DTS2015111102692 by lizimian(l00251625), 2015/11/10 */
  duration = GST_BUFFER_DURATION (buffer);
  buffer_size = gst_buffer_get_size (buffer);

  if (gst_hls_demux_stream_push_data (stream, buffer, duration, buffer_size)) {
      stream->fragment_total_size += buffer_size;
  }
  gst_task_start (stream->stream_task);

  return;
}

static gboolean
gst_hls_demux_get_next_fragment (GstHLSDemuxStream * stream, gboolean caching)
{
  GstFragment *download;
  gchar *next_fragment_uri;
  GstClockTime duration;
  GstClockTime timestamp;
  gint64 range_start;
  gint64 range_end;
  GstBuffer *buf;
  gboolean discont;
  gchar *key = NULL;
  guint8 *iv = NULL;
  gboolean forward = FALSE;
  GstHLSDemux *demux = stream->parent;
  GTimeVal now;
  GTimeVal start;
  gsize buffer_size;
  GstStructure *s;
  GstMessage *m;

  memset(&now, 0, sizeof(GTimeVal));
  memset(&start, 0, sizeof(GTimeVal));

  if (!gst_m3u8_client_get_next_fragment (demux->client, &discont,
          &next_fragment_uri, &duration, &timestamp,
          &range_start, &range_end, &key, &iv, forward, stream->video)) {
    GST_INFO_OBJECT (demux, "This playlist doesn't contain more fragments");

    //update download rate to 0 if no more fragments need to be downloaded
    gst_element_post_message (GST_ELEMENT (demux),
        gst_message_new_new_bandwidth(GST_OBJECT (demux),
        (guint) 0));

    /* BEGIN: Modified for SME-110 by wanghua(w00347020), 2015/8/7 */
    stream->end_of_playlist = TRUE;
    /* END: Modified for SME-110 by wanghua(w00347020), 2015/8/7 */
    gst_task_start (stream->stream_task);
    return FALSE;
  }

  demux->next_frag_uri = next_fragment_uri;
  GST_INFO_OBJECT (demux, "Fetching next fragment, start %lld, end %lld",
      range_start, range_end);

  g_get_current_time (&start);

  if (!key) {
    /* BEGIN: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */
    gst_hls_demux_get_header_for_I_frames_only (stream, range_start, next_fragment_uri);
    /* END: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */
    if (!gst_uri_downloader_set_uri_for_first_fragment_with_range (stream->downloader,
                                            next_fragment_uri,
                                            gst_hls_demux_get_buffer_cb,
                                            (void*)stream, range_start, range_end))
       return FALSE;

  if (G_MAXUINT64 == stream->first_cache_pts)
    stream->first_cache_pts = timestamp;

    return TRUE;
  }

  /** BEGIN: Modified for DTS2014072906154 by liurongliang(l00180035), 2014/7/30 */
  download = gst_hls_downloader_fetch_uri_check_timeout(stream, next_fragment_uri);
  /** END: Modified for DTS2014072906154 by liurongliang(l00180035), 2014/7/30 */

  if (stream->cancelled)
    goto error;

  if (download && key)
    download = gst_hls_demux_decrypt_fragment (stream, download, key, iv);
  g_get_current_time (&now);
  stream->fragment_total_time =
        (guint64) (GST_TIMEVAL_TO_TIME (now) - GST_TIMEVAL_TO_TIME (start));

  if (download == NULL)
    goto error;

  buf = gst_fragment_get_buffer (download);

  buffer_size = gst_buffer_get_size (buf);
  stream->fragment_total_size = buffer_size;
  stream->last_fragment_size = buffer_size;

  if (stream->video) {
    gst_hls_demux_stream_update_current_bitrate (stream);
    gst_element_post_message (GST_ELEMENT (demux),
        gst_message_new_new_bandwidth(GST_OBJECT (demux),
        (guint) stream->current_download_rate));
  }

  GST_BUFFER_DURATION (buf) = duration;
  GST_BUFFER_PTS (buf) = timestamp;

  /* We actually need to do this every time we switch bitrate */
  if (G_UNLIKELY (stream->do_typefind)) {
    GstCaps *caps = gst_fragment_get_caps (download);

    if (!stream->input_caps || !gst_caps_is_equal (caps, stream->input_caps)) {
      gst_caps_replace (&stream->input_caps, caps);
      GST_INFO_OBJECT (stream, "Input source caps: %" GST_PTR_FORMAT,
          stream->input_caps);
      stream->do_typefind = FALSE;
    }
    gst_caps_unref (caps);
  }

  if (discont) {
    GST_INFO_OBJECT (demux, "Marking fragment as discontinuous");
    GST_BUFFER_FLAG_SET (buf, GST_BUFFER_FLAG_DISCONT);
  }

  /* The buffer ref is still kept inside the fragment download */
  /* BEGIN: Modified for DTS2015111102692 by lizimian(l00251625), 2015/11/10 */
  gst_object_unref (download);
  /* END: Modified for DTS2015111102692 by lizimian(l00251625), 2015/11/10 */

  gst_hls_demux_stream_push_data (stream, buf, duration, buffer_size);

  if (!caching) {
    GST_TASK_SIGNAL (stream->updates_task);
    gst_task_start (stream->stream_task);
  }
  return TRUE;

error:
  {
    return FALSE;
  }
}

/** BEGIN: Added for DTS2014072906154by liurongliang(l00180035), 2014/7/30 */
GstFragment *
gst_hls_downloader_fetch_uri_check_timeout(GstHLSDemuxStream * stream, const gchar * uri)
{
  GstFragment *download = NULL;
  GstHLSDemux *demux = stream->parent;

  stream->last_timeout_time = G_MAXUINT64;

  while (TRUE) {
    guint64 cur_sys_time = gst_get_sys_time();

    download = gst_uri_downloader_fetch_uri (stream->downloader, uri);
    if (download != NULL)
      break;

    if ((G_MAXUINT64 != stream->last_timeout_time) &&
         (cur_sys_time + 1000 >= stream->last_timeout_time + demux->timeout_val * 1000)) {
      break;
    }

    if (G_MAXUINT64 == stream->last_timeout_time) {
      stream->last_timeout_time = cur_sys_time;
    }
    SLEEP_IF_NOT_CANCEL(stream, TRY_INTERVAL);
    gst_uri_downloader_reset(stream->downloader);
  }
quit:
  return download;
}
/** END:   Added for DTS2014072906154by liurongliang(l00180035), 2014/7/30 */

/* BEGIN: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */
void
gst_hls_demux_get_header_for_I_frames_only (const GstHLSDemuxStream * stream, gint64 range_start, const gchar * uri)
{
  GstHLSDemux *demux = stream->parent;

  /* BEGIN: Modified for DTS2015092303415 by wanghua(w00347020), 2015/10/8*/
  if (stream->video && demux->client->first_fragment_sequence == (demux->client->sequence - 1)
       && range_start != 0 && !demux->header)
    demux->header = gst_uri_downloader_fetch_uri_with_range (
        stream->downloader, uri, (guint64) 0, range_start);
  /* END: Modified for DTS2015092303415 by wanghua(w00347020), 2015/10/8*/
}
/* END: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */
