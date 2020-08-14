/* GStreamer
 * Copyright (C) 2011 Andoni Morales Alastruey <ylatuya@gmail.com>
 *
 * gstfragment.c:
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

#include <glib.h>
#include "gstfragment.h"
#include "gsturidownloader.h"
#include "gsturidownloader_debug.h"

#define GST_CAT_DEFAULT uridownloader_debug
GST_DEBUG_CATEGORY (uridownloader_debug);

#define GST_URI_DOWNLOADER_GET_PRIVATE(obj)  \
   (G_TYPE_INSTANCE_GET_PRIVATE ((obj), \
    GST_TYPE_URI_DOWNLOADER, GstUriDownloaderPrivate))

#define TIME_OUT_KEY "timeout"
#define GST_SME_PROP_KEY_CUR_BW         "gst-sme-prop-key-current-bandwidth"
#define USER_AGENT "user-agent"
#define COOKIES "cookies"
#define REFERER "referer"


/* GObject */
static void gst_uri_downloader_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_uri_downloader_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

enum
{
  PROP_0,
  PROP_TIMEOUT,
  PROP_CUR_BW,
  PROP_USER_AGENT,
  PROP_COOKIES,
  PROP_REFERER,
  PROP_LAST
};


struct _GstUriDownloaderPrivate
{
  /* Fragments fetcher */
  GstElement *urisrc;
  GstBus *bus;
  GstPad *pad;
  GTimeVal *timeout;
  GstFragment *download;
  GMutex download_lock;         /* used to restrict to one download only */

  void *handler;
  uridownloader_cb cb;

  GCond cond;
  gboolean cancelled;
  /* BEGIN: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
  GstElement *parent;
  /* END: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
};

static void gst_uri_downloader_finalize (GObject * object);
static void gst_uri_downloader_dispose (GObject * object);

static GstFlowReturn gst_uri_downloader_chain (GstPad * pad, GstObject * parent,
    GstBuffer * buf);
static gboolean gst_uri_downloader_sink_event (GstPad * pad, GstObject * parent,
    GstEvent * event);
static GstBusSyncReply gst_uri_downloader_bus_handler (GstBus * bus,
    GstMessage * message, gpointer data);
static void
gst_uri_downloader_reset_for_first_fragment(GstUriDownloader * downloader);


static GstStaticPadTemplate sinkpadtemplate = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS_ANY);

#define _do_init \
{ \
  GST_DEBUG_CATEGORY_INIT (uridownloader_debug, "uridownloader", 0, "URI downloader"); \
}

G_DEFINE_TYPE_WITH_CODE (GstUriDownloader, gst_uri_downloader, GST_TYPE_OBJECT,
    _do_init);

void gst_wait_eos_event (GstUriDownloader * downloader)
{
  g_return_if_fail (downloader != NULL);

  if (downloader->priv->handler && downloader->priv->cb)
    if(downloader->priv->download) {
      GST_OBJECT_LOCK (downloader);
      while (!downloader->priv->cancelled && !downloader->priv->download->completed)
        g_cond_wait (&downloader->priv->cond, GST_OBJECT_GET_LOCK (downloader));
      GST_OBJECT_UNLOCK(downloader);
    }
    gst_uri_downloader_reset_for_first_fragment (downloader);
}


static void
gst_uri_downloader_class_init (GstUriDownloaderClass * klass)
{
  GObjectClass *gobject_class;

  gobject_class = (GObjectClass *) klass;

  g_type_class_add_private (klass, sizeof (GstUriDownloaderPrivate));

  gobject_class->dispose = gst_uri_downloader_dispose;
  gobject_class->finalize = gst_uri_downloader_finalize;
  gobject_class->set_property = gst_uri_downloader_set_property;
  gobject_class->get_property = gst_uri_downloader_get_property;

  g_object_class_install_property (gobject_class, PROP_TIMEOUT,
      g_param_spec_uint (TIME_OUT_KEY, TIME_OUT_KEY,
          "Value in seconds to timeout (0 = No timeout).", 0,
          3600, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
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
  g_object_class_install_property(gobject_class, PROP_CUR_BW,
      g_param_spec_uint (GST_SME_PROP_KEY_CUR_BW, "current bandwidth",
      "current bandwidth", 0, (guint)(-1), 0, G_PARAM_READWRITE
      | G_PARAM_STATIC_STRINGS));
}

static void
gst_uri_downloader_init (GstUriDownloader * downloader)
{
  downloader->priv = GST_URI_DOWNLOADER_GET_PRIVATE (downloader);

  /* Initialize the sink pad. This pad will be connected to the src pad of the
   * element created with gst_element_make_from_uri and will handle the download */
  downloader->priv->pad =
      gst_pad_new_from_static_template (&sinkpadtemplate, "sink");
  gst_pad_set_chain_function (downloader->priv->pad,
      GST_DEBUG_FUNCPTR (gst_uri_downloader_chain));
  gst_pad_set_event_function (downloader->priv->pad,
      GST_DEBUG_FUNCPTR (gst_uri_downloader_sink_event));
  gst_pad_set_element_private (downloader->priv->pad, downloader);
  gst_pad_set_active (downloader->priv->pad, TRUE);

  /* Create a bus to handle error and warning message from the source element */
  downloader->priv->bus = gst_bus_new ();
  g_mutex_init (&downloader->priv->download_lock);
  g_cond_init (&downloader->priv->cond);
  downloader->priv->cb = NULL;
  downloader->priv->handler = NULL;

  downloader->timeout_val = 0;
  downloader->pre_bandwidth = G_MAXUINT32;

  downloader->user_agent = NULL;
  downloader->cookies = NULL;
  downloader->referer = NULL;
}

static void
gst_uri_downloader_dispose (GObject * object)
{
  GstUriDownloader *downloader = GST_URI_DOWNLOADER (object);

  if (downloader->priv->urisrc != NULL) {
    gst_object_unref (downloader->priv->urisrc);
    downloader->priv->urisrc = NULL;
  }

  if (downloader->priv->bus != NULL) {
    gst_object_unref (downloader->priv->bus);
    downloader->priv->bus = NULL;
  }

  if (downloader->priv->pad) {
    gst_object_unref (downloader->priv->pad);
    downloader->priv->pad = NULL;
  }

  if (downloader->priv->download) {
    g_object_unref (downloader->priv->download);
    downloader->priv->download = NULL;
  }
  if (downloader->priv->handler)
    downloader->priv->handler = NULL;

  if (downloader->priv->cb)
    downloader->priv->cb = NULL;

  g_free (downloader->user_agent);
  downloader->user_agent = NULL;
  g_free (downloader->referer);
  downloader->referer = NULL;
  if (downloader->cookies) {
    g_strfreev (downloader->cookies);
    downloader->cookies = NULL;
  }
  G_OBJECT_CLASS (gst_uri_downloader_parent_class)->dispose (object);
}

static void
gst_uri_downloader_finalize (GObject * object)
{
  GstUriDownloader *downloader = GST_URI_DOWNLOADER (object);

  g_mutex_clear (&downloader->priv->download_lock);
  g_cond_clear (&downloader->priv->cond);

  G_OBJECT_CLASS (gst_uri_downloader_parent_class)->finalize (object);
}

GstUriDownloader *
gst_uri_downloader_new (void)
{
  return g_object_new (GST_TYPE_URI_DOWNLOADER, NULL);
}


GstUriDownloader *
gst_uri_downloader_new_with_parent (GstElement * parent)
{
  GstUriDownloader * downloader;

  downloader = g_object_new (GST_TYPE_URI_DOWNLOADER, NULL);
  if (downloader && parent) {
      downloader->priv->parent = parent;
  }
  return downloader;
}


static gboolean
gst_uri_downloader_sink_event (GstPad * pad, GstObject * parent,
    GstEvent * event)
{
  gboolean ret = FALSE;
  GstUriDownloader *downloader;

  downloader = GST_URI_DOWNLOADER (gst_pad_get_element_private (pad));

  switch (event->type) {
    case GST_EVENT_EOS:{
      GST_OBJECT_LOCK (downloader);
      GST_DEBUG_OBJECT (downloader, "Got EOS on the fetcher pad");
      if(downloader->priv->handler && downloader->priv->cb)
      {
        g_object_get(downloader->priv->urisrc, GST_SME_PROP_KEY_CUR_BW, &downloader->pre_bandwidth, NULL);
      }
      GST_DEBUG("hls current bandwidth:%d",downloader->pre_bandwidth);
      if (downloader->priv->download != NULL) {
        /* signal we have fetched the URI */
        downloader->priv->download->completed = TRUE;
        downloader->priv->download->download_stop_time =
            gst_util_get_timestamp ();
        GST_DEBUG_OBJECT (downloader, "Signaling chain funtion");
        g_cond_signal (&downloader->priv->cond);
      }
      GST_OBJECT_UNLOCK (downloader);
      gst_event_unref (event);
      break;
    }
    default:
      ret = gst_pad_event_default (pad, parent, event);
      break;
  }

  return ret;
}

static GstBusSyncReply
gst_uri_downloader_bus_handler (GstBus * bus,
    GstMessage * message, gpointer data)
{
  GstUriDownloader *downloader = (GstUriDownloader *) (data);

  /* BEGIN: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
  GError *err = NULL;
  gchar *dbg_info = NULL;

  /** foroward http err message to parent bus */
  if (GST_MESSAGE_TYPE (message) == GST_MESSAGE_NEW_HTTP_ERR)
  {
      gst_element_post_message (GST_ELEMENT (downloader->priv->parent),
                                message);
      return  GST_BUS_DROP;
  }

  if (GST_MESSAGE_TYPE (message) == GST_MESSAGE_WARNING)
  {
    gst_message_parse_warning (message, &err, &dbg_info);
    GST_WARNING_OBJECT (downloader,
        "Received warning: %s from %s, the download will be cancelled",
        GST_OBJECT_NAME (message->src), err->message);

    if (GST_RESOURCE_ERROR == err->domain
        && GST_RESOURCE_ERROR_NO_DATA == err->code
        && downloader->priv->parent)
    GST_ELEMENT_WARNING (downloader->priv->parent, RESOURCE, NO_DATA, (NULL), (NULL));

    g_error_free (err);
    g_free (dbg_info);
    goto end;
  }

  /* END: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
  if (GST_MESSAGE_TYPE (message) == GST_MESSAGE_ERROR ||
       GST_MESSAGE_TYPE (message) == GST_MESSAGE_WARNING) {

    if (GST_MESSAGE_TYPE (message) == GST_MESSAGE_ERROR)
      gst_message_parse_error (message, &err, &dbg_info);
    else
      gst_message_parse_warning (message, &err, &dbg_info);

    GST_WARNING_OBJECT (downloader,
        "Received error: %s from %s, the download will be cancelled",
        GST_OBJECT_NAME (message->src), err->message);
    GST_DEBUG ("Debugging info: %s\n", (dbg_info) ? dbg_info : "none");
    g_error_free (err);
    g_free (dbg_info);

    /* remove the sync handler to avoid duplicated messages */
    gst_bus_set_sync_handler (downloader->priv->bus, NULL, NULL, NULL);

    /* stop the download */
    GST_OBJECT_LOCK (downloader);
    if (downloader->priv->download != NULL) {
      GST_DEBUG_OBJECT (downloader, "Stopping download");
      g_object_unref (downloader->priv->download);
      downloader->priv->download = NULL;
      downloader->priv->cancelled = TRUE;
      g_cond_signal (&downloader->priv->cond);
    }
    GST_OBJECT_UNLOCK (downloader);
  }

  end:
    gst_message_unref (message);
    return GST_BUS_DROP;
}

static GstFlowReturn
gst_uri_downloader_chain (GstPad * pad, GstObject * parent, GstBuffer * buf)
{
  GstUriDownloader *downloader;

  downloader = GST_URI_DOWNLOADER (gst_pad_get_element_private (pad));

  /* HTML errors (404, 500, etc...) are also pushed through this pad as
   * response but the source element will also post a warning or error message
   * in the bus, which is handled synchronously cancelling the download.
   */
  GST_OBJECT_LOCK (downloader);

  if (downloader->priv->cancelled) {
     GST_OBJECT_UNLOCK (downloader);
     gst_buffer_unref(buf);
     goto done;
  }

  if (downloader->priv->handler && downloader->priv->cb) {
    GstFragment *download = gst_fragment_new ();
    if (NULL == download) {
        GST_OBJECT_UNLOCK (downloader);
        gst_buffer_unref(buf);
        goto done;
    }
    if (!gst_fragment_add_buffer (download, buf)) {
        /** BEGIN: Modified for DTS2015042206034  by baihaili(b00311320), 2015/5/29 */
        gst_buffer_unref(buf);
        /** END:   Modified for DTS2015042206034 by baihaili(b00311320), 2015/5/29 */
        GST_WARNING_OBJECT (downloader, "Could not add buffer to fragment");
    }
    download->completed = TRUE;
    downloader->priv->cb(downloader->priv->handler, download);
  }
  else if (downloader->priv->download == NULL) {
    /* Download cancelled, quit */
    GST_OBJECT_UNLOCK (downloader);
    gst_buffer_unref(buf);
    goto done;
  }
  else {
    GST_LOG_OBJECT (downloader, "The uri fetcher received a new buffer "
        "of size %" G_GSIZE_FORMAT, gst_buffer_get_size (buf));
    if (!gst_fragment_add_buffer (downloader->priv->download, buf))
      GST_WARNING_OBJECT (downloader, "Could not add buffer to fragment");
  }
  GST_OBJECT_UNLOCK (downloader);

done:
  {
    return GST_FLOW_OK;
  }
}

void
gst_uri_downloader_reset (GstUriDownloader * downloader)
{
  g_return_if_fail (downloader != NULL);

  GST_OBJECT_LOCK (downloader);
  downloader->priv->cancelled = FALSE;
  GST_OBJECT_UNLOCK (downloader);
  gst_uri_downloader_reset_for_first_fragment(downloader);
}

void
gst_uri_downloader_cancel (GstUriDownloader * downloader)
{
  GST_OBJECT_LOCK (downloader);
  if (downloader->priv->download != NULL) {
    GST_INFO_OBJECT (downloader, "Cancelling download");
    g_object_unref (downloader->priv->download);
    downloader->priv->download = NULL;
    downloader->priv->cancelled = TRUE;
    GST_INFO_OBJECT (downloader, "Signaling chain funtion");
    g_cond_signal (&downloader->priv->cond);
  } else {
    gboolean cancelled;

    cancelled = downloader->priv->cancelled;
    downloader->priv->cancelled = TRUE;
    if (cancelled)
      GST_DEBUG_OBJECT (downloader,
          "Trying to cancel a download that was alredy cancelled");
  }
  GST_OBJECT_UNLOCK (downloader);
}

static gboolean
gst_uri_downloader_set_range (GstUriDownloader * downloader,
    gint64 range_start, gint64 range_end)
{
  g_return_val_if_fail (range_start >= 0, FALSE);
  g_return_val_if_fail (range_end >= -1, FALSE);

  if (range_start || (range_end >= 0)) {
    GstEvent *seek;

    seek = gst_event_new_seek (1.0, GST_FORMAT_BYTES, GST_SEEK_FLAG_FLUSH,
        GST_SEEK_TYPE_SET, range_start, GST_SEEK_TYPE_SET, range_end);

    return gst_element_send_event (downloader->priv->urisrc, seek);
  }
  return TRUE;
}

static gboolean
gst_uri_downloader_set_uri (GstUriDownloader * downloader, const gchar * uri)
{
  GstPad *pad;

  if (!gst_uri_is_valid (uri))
    return FALSE;

  g_assert (downloader->priv->urisrc == NULL);

//  GST_DEBUG_OBJECT (downloader, "Creating source element for the URI:%s", uri);
  downloader->priv->urisrc =
      gst_element_make_from_uri (GST_URI_SRC, uri, NULL, NULL);
  if (!downloader->priv->urisrc)
    return FALSE;

  /** BEGIN: Add for DTS2014072906154 by liurongliang(l00180035), 2014/7/30 */
  g_object_set (downloader->priv->urisrc, TIME_OUT_KEY, downloader->timeout_val,
      USER_AGENT, downloader->user_agent,
      REFERER, downloader->referer, NULL);
  if (downloader->cookies)
    g_object_set (downloader->priv->urisrc, COOKIES, downloader->cookies, NULL);
  /** END: Add for DTS2014072906154 by liurongliang(l00180035), 2014/7/30 */

  /* add a sync handler for the bus messages to detect errors in the download */
  gst_element_set_bus (GST_ELEMENT (downloader->priv->urisrc),
      downloader->priv->bus);
  gst_bus_set_sync_handler (downloader->priv->bus,
      gst_uri_downloader_bus_handler, downloader, NULL);

  pad = gst_element_get_static_pad (downloader->priv->urisrc, "src");
  if (!pad)
    return FALSE;
  gst_pad_link (pad, downloader->priv->pad);
  gst_object_unref (pad);
  return TRUE;
}

GstFragment *
gst_uri_downloader_fetch_uri (GstUriDownloader * downloader, const gchar * uri)
{
  gst_wait_eos_event (downloader);
  return gst_uri_downloader_fetch_uri_with_range (downloader, uri, 0, -1);
}

/**
 * gst_uri_downloader_fetch_uri_with_range:
 * @downloader: the #GstUriDownloader
 * @uri: the uri
 * @range_start: the starting byte index
 * @range_end: the final byte index, use -1 for unspecified
 *
 * Returns the downloaded #GstFragment
 */
GstFragment *
gst_uri_downloader_fetch_uri_with_range (GstUriDownloader * downloader,
    const gchar * uri, gint64 range_start, gint64 range_end)
{
  GstStateChangeReturn ret;
  GstFragment *download = NULL;

//  GST_DEBUG_OBJECT (downloader, "Fetching URI %s", uri);

  g_mutex_lock (&downloader->priv->download_lock);

  GST_OBJECT_LOCK (downloader);
  if (downloader->priv->cancelled) {
    GST_DEBUG_OBJECT (downloader, "Cancelled, aborting fetch");
    goto quit;
  }

  if (!gst_uri_downloader_set_uri (downloader, uri)) {
    GST_WARNING_OBJECT (downloader, "Failed to set URI");
    goto quit;
  }

  gst_bus_set_flushing (downloader->priv->bus, FALSE);
  downloader->priv->download = gst_fragment_new ();
  GST_OBJECT_UNLOCK (downloader);
  ret = gst_element_set_state (downloader->priv->urisrc, GST_STATE_READY);
  GST_OBJECT_LOCK (downloader);
  if (ret == GST_STATE_CHANGE_FAILURE || downloader->priv->download == NULL) {
    GST_WARNING_OBJECT (downloader, "Failed to set src to READY");
    goto quit;
  }

  /* might have been cancelled because of failures in state change */
  if (downloader->priv->cancelled) {
    goto quit;
  }

  if (!gst_uri_downloader_set_range (downloader, range_start, range_end)) {
    GST_WARNING_OBJECT (downloader, "Failed to set range");
    goto quit;
  }

  GST_OBJECT_UNLOCK (downloader);
  ret = gst_element_set_state (downloader->priv->urisrc, GST_STATE_PLAYING);
  GST_OBJECT_LOCK (downloader);
  if (ret == GST_STATE_CHANGE_FAILURE){
    if (downloader->priv->download) {
      g_object_unref (downloader->priv->download);
      downloader->priv->download = NULL;
    }
    GST_WARNING_OBJECT (downloader, "Failed to set src to PLAYING");
    goto quit;
  }

  /* might have been cancelled because of failures in state change */
  if (downloader->priv->cancelled) {
    GST_WARNING_OBJECT (downloader, "cancelled");
    goto quit;
  }

  /* wait until:
   *   - the download succeed (EOS in the src pad)
   *   - the download failed (Error message on the fetcher bus)
   *   - the download was canceled
   */
//  GST_DEBUG_OBJECT (downloader, "Waiting to fetch the URI %s", uri);
  while (!downloader->priv->cancelled && !downloader->priv->download->completed)
    g_cond_wait (&downloader->priv->cond, GST_OBJECT_GET_LOCK (downloader));

  if (downloader->priv->cancelled) {
    if (downloader->priv->download) {
      g_object_unref (downloader->priv->download);
      downloader->priv->download = NULL;
    }
    goto quit;
  }

  download = downloader->priv->download;
  downloader->priv->download = NULL;

  if (download != NULL)
    GST_INFO_OBJECT (downloader, "URI fetched successfully");
  else
    GST_INFO_OBJECT (downloader, "Error fetching URI");

quit:
  {
    if (downloader->priv->urisrc) {
      GstPad *pad;
      GstElement *urisrc;

      GST_DEBUG_OBJECT (downloader, "Stopping source element %s",
          GST_ELEMENT_NAME (downloader->priv->urisrc));

      /* remove the bus' sync handler */
      gst_bus_set_sync_handler (downloader->priv->bus, NULL, NULL, NULL);
      /* unlink the source element from the internal pad */
      pad = gst_pad_get_peer (downloader->priv->pad);
      if (pad) {
        gst_pad_unlink (pad, downloader->priv->pad);
        gst_object_unref (pad);
      }
      urisrc = downloader->priv->urisrc;
      downloader->priv->urisrc = NULL;
      GST_OBJECT_UNLOCK (downloader);

      GST_DEBUG_OBJECT (downloader, "Stopping source element %s",
          GST_ELEMENT_NAME (urisrc));

      /* set the element state to NULL */
      gst_bus_set_flushing (downloader->priv->bus, TRUE);
      gst_element_set_state (urisrc, GST_STATE_NULL);
      gst_element_get_state (urisrc, NULL, NULL, GST_CLOCK_TIME_NONE);
      gst_element_set_bus (urisrc, NULL);
      gst_object_unref (urisrc);
    } else {
      GST_OBJECT_UNLOCK (downloader);
    }

    g_mutex_unlock (&downloader->priv->download_lock);
    return download;
  }
}

/** BEGIN: Add for DTS2014072906154 by liurongliang(l00180035), 2014/7/30 */
/* GObject */
static void
gst_uri_downloader_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
    GstUriDownloader *downloader = GST_URI_DOWNLOADER (object);

    switch (prop_id) {
      case PROP_TIMEOUT:
        downloader->timeout_val = g_value_get_uint (value);
        GST_INFO("urldown set timeoutval = %u, urisrc = %p",
            downloader->timeout_val, downloader->priv->urisrc);
        if(downloader->priv->urisrc){
          g_object_set (downloader->priv->urisrc, TIME_OUT_KEY,
            downloader->timeout_val, NULL);
        }
        break;
      case PROP_USER_AGENT:
        if (downloader->user_agent)
          g_free (downloader->user_agent);
        downloader->user_agent = g_value_dup_string (value);
        break;
      case PROP_REFERER:
        if (downloader->referer)
          g_free (downloader->referer);
        downloader->referer = g_value_dup_string (value);
        break;
      case PROP_COOKIES:
        g_strfreev (downloader->cookies);
        downloader->cookies = g_strdupv (g_value_get_boxed (value));
        break;

      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }

    return ;
}

static void
gst_uri_downloader_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
    GstUriDownloader *downloader = GST_URI_DOWNLOADER (object);

    switch (prop_id) {
      case PROP_TIMEOUT:
        g_value_set_uint (value, downloader->timeout_val);
        break;
      case PROP_CUR_BW:
      {
          guint u_cur_bw = (guint)(-1);
          if (downloader->priv && downloader->priv->urisrc ) {
              g_object_get(downloader->priv->urisrc, GST_SME_PROP_KEY_CUR_BW, &u_cur_bw, NULL);
          }
          g_value_set_uint(value, u_cur_bw);
          break;
      }
      case PROP_USER_AGENT:
        g_value_set_string (value, downloader->user_agent);
        break;
      case PROP_REFERER:
        g_value_set_string (value, downloader->referer);
        break;
      case PROP_COOKIES:
        g_value_set_boxed (value, downloader->cookies);
        break;
      default:
        break;
    }
    return ;
}
/** END: Add for DTS2014072906154 by liurongliang(l00180035), 2014/7/30 */

static void
gst_uri_downloader_reset_for_first_fragment(GstUriDownloader * downloader)
{
   if (NULL == downloader)
     return;

   GST_OBJECT_LOCK (downloader);

   /*Modify BEGIN:for DTS2014082500401 by zhoubing 00213950 20140825*/
   if (!downloader->priv->handler && !downloader->priv->cb)
   {
      GST_OBJECT_UNLOCK (downloader);
      return;
   }

   downloader->priv->handler = NULL;
   downloader->priv->cb = NULL;
   /*Modify END:for DTS2014082500401 by zhoubing 00213950 20140825*/

   if (downloader->priv->download){
     g_object_unref (downloader->priv->download);
     downloader->priv->download = NULL;
   }

   if (downloader->priv->urisrc) {
      GstPad *pad;
      GstElement *urisrc;

      GST_DEBUG_OBJECT (downloader, "Stopping source element %s",
          GST_ELEMENT_NAME (downloader->priv->urisrc));

      /* remove the bus' sync handler */
      gst_bus_set_sync_handler (downloader->priv->bus, NULL, NULL, NULL);
      /* unlink the source element from the internal pad */
      pad = gst_pad_get_peer (downloader->priv->pad);
      if (pad) {
        gst_pad_unlink (pad, downloader->priv->pad);
        gst_object_unref (pad);
      }
      urisrc = downloader->priv->urisrc;
      downloader->priv->urisrc = NULL;
      GST_OBJECT_UNLOCK (downloader);

      GST_DEBUG_OBJECT (downloader, "Stopping source element %s",
          GST_ELEMENT_NAME (urisrc));

      /* set the element state to NULL */
      gst_bus_set_flushing (downloader->priv->bus, TRUE);
      gst_element_set_state (urisrc, GST_STATE_NULL);
      gst_element_get_state (urisrc, NULL, NULL, GST_CLOCK_TIME_NONE);
      gst_element_set_bus (urisrc, NULL);
      gst_object_unref (urisrc);
    } else {
      GST_OBJECT_UNLOCK (downloader);
    }
}

/** BEGIN: Added for DTS2015042203653  by baihaili(b00311320), 2015/4/24 */
gboolean
gst_uri_downloader_set_uri_for_first_fragment(GstUriDownloader * downloader,
    const gchar * uri, uridownloader_cb cb, void* hander)
{
  //gst_wait_eos_event (downloader);
  return gst_uri_downloader_set_uri_for_first_fragment_with_range(downloader,
      uri, cb, hander, 0, -1);
}


gboolean
gst_uri_downloader_set_uri_for_first_fragment_with_range(GstUriDownloader * downloader,
    const gchar * uri, uridownloader_cb cb, void * hander,
    gint64 range_start, gint64 range_end)
{

    GstStateChangeReturn ret;

    if (NULL == cb || NULL == hander )
        return FALSE;

    //WAIT_EOS_IF_EXIST_CB(downloader);

    //GST_DEBUG_OBJECT (downloader, "Fetching URI %s", uri);
    g_mutex_lock (&downloader->priv->download_lock);
    GST_OBJECT_LOCK (downloader);

    if (downloader->priv->cancelled) {
        GST_DEBUG_OBJECT (downloader, "Cancelled, aborting fetch");
        goto quit;
    }

    if (!gst_uri_downloader_set_uri (downloader, uri)) {
        GST_WARNING_OBJECT (downloader, "Failed to set URI");
        goto quit;
    }

    gst_bus_set_flushing (downloader->priv->bus, FALSE);

    downloader->priv->handler = hander;
    downloader->priv->cb = cb;

    /** BEGIN: Modified for DTS2015042206034  by baihaili(b00311320), 2015/5/29 */
    if (downloader->priv->download)
      g_object_unref(downloader->priv->download);
    /** END:   Modified for DTS2015042206034 by baihaili(b00311320), 2015/5/29 */

    downloader->priv->download = gst_fragment_new ();

    GST_OBJECT_UNLOCK (downloader);

    ret = gst_element_set_state (downloader->priv->urisrc, GST_STATE_READY);
    GST_OBJECT_LOCK (downloader);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        GST_WARNING_OBJECT (downloader, "Failed to set src to READY");
        goto quit;
    }

    /* might have been cancelled because of failures in state change */
    if (downloader->priv->cancelled) {
        goto quit;
    }

    if (!gst_uri_downloader_set_range (downloader, range_start, range_end)) {
      GST_WARNING_OBJECT (downloader, "Failed to set range");
      goto quit;
    }

    GST_OBJECT_UNLOCK (downloader);
    ret = gst_element_set_state (downloader->priv->urisrc, GST_STATE_PLAYING);
    GST_OBJECT_LOCK (downloader);
    if (ret == GST_STATE_CHANGE_FAILURE){
        GST_WARNING_OBJECT (downloader, "Failed to set src to PLAYING");
        goto quit;
    }

    /* might have been cancelled because of failures in state change */
    if (downloader->priv->cancelled) {
        GST_WARNING_OBJECT (downloader, "cancelled");
        goto quit;
    }

    g_mutex_unlock (&downloader->priv->download_lock);
    //g_usleep(20000);
    GST_OBJECT_UNLOCK (downloader);
    return TRUE;

quit:
    if (downloader->priv->urisrc) {
        GstPad *pad;
        GstElement *urisrc;

        GST_DEBUG_OBJECT (downloader, "Stopping source element %s",
          GST_ELEMENT_NAME (downloader->priv->urisrc));

        /* remove the bus' sync handler */
        gst_bus_set_sync_handler (downloader->priv->bus, NULL, NULL, NULL);
        /* unlink the source element from the internal pad */
        pad = gst_pad_get_peer (downloader->priv->pad);
        if (pad) {
        gst_pad_unlink (pad, downloader->priv->pad);
        gst_object_unref (pad);
        }
        urisrc = downloader->priv->urisrc;
        downloader->priv->urisrc = NULL;
        GST_OBJECT_UNLOCK (downloader);

        GST_DEBUG_OBJECT (downloader, "Stopping source element %s",
          GST_ELEMENT_NAME (urisrc));

        /* set the element state to NULL */
        gst_bus_set_flushing (downloader->priv->bus, TRUE);
        gst_element_set_state (urisrc, GST_STATE_NULL);
        gst_element_get_state (urisrc, NULL, NULL, GST_CLOCK_TIME_NONE);
        gst_element_set_bus (urisrc, NULL);
        gst_object_unref (urisrc);
    } else {
        GST_OBJECT_UNLOCK (downloader);
    }

    g_mutex_unlock (&downloader->priv->download_lock);
    return FALSE;
}
/** END: Added for DTS201504220365322  by baihaili(b00311320), 2015/4/24 */


/* Added BEGIN: DTS2014081803527, zhoubing z00213950 2014/08/19*/
gboolean
gst_uri_downloader_can_eos(GstUriDownloader * downloader)
{
    if (downloader && downloader->priv && downloader->priv->handler && downloader->priv->cb)
    {
        if (downloader->priv->download)
        {
            if (!downloader->priv->cancelled && !downloader->priv->download->completed)
                return FALSE;
        }
    }
    return TRUE;
}
/* Added END: DTS2014081803527, zhoubing z00213950 2014/08/19*/
