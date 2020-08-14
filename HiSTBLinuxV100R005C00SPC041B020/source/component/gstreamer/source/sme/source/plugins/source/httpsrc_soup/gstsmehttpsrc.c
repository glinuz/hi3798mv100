/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmehttpsrc.c
 * @brief   httpsrc 实现
 * @author  zhangzhen(z00175754)
 * @date    2014/06/04
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/06/04
 * Author : zhangzhen(z00175754)
 * Desc   : Created file
 *
******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <gst/gstelement.h>
#include <libsoup/soup.h>
#include "gstsmehttpsrc.h"
#include "sme_player_type.h"
#include "soup-message-queue.h"

//lint -e585 (可变参数...报错，为gstreamer中的日志宏，无法修改。)
//lint -e655
//lint -e717 //do{}while(0)
//lint -e826 //不等价指针转换，这里为安全转换
//lint -e838
//lint -e801 //goto, 这里可以使用goto,gstreamer里面也是这么使用的。
//lint -e774 //new 判断为空报，始终不为空,gstreamer 宏无法修改。
//lint -e19  //gstreamer 自身的宏G_DEFINE_TYPE_WITH_CODE
//lint -e528 //gstreamer 自身的宏G_DEFINE_TYPE_WITH_CODE
//lint -e818 //inherence or callback, cannot modify the poiter args to const

typedef struct
{
    GstBuffer *buffer;
    GstMapInfo map;
} SmeGstChunk;

#define GST_SMEHTTPSRC_NAME "gstsmehttpsrc"
#define DEFAULT_USER_AGENT  "GStreamer smehttpsrc "
#define DEFAULT_IRADIO_MODE TRUE
#define DEFAULT_HTTP_BUF_BLOCK_SIZE (8*1024)
#define BANDWIDTH_COLLECT_FREQ 500000
#define TIMEOUT_DEVIATION 2
#define BANDWIDTH_UPDATE_TIMEOUT 3000000// 3s
#define CANCEL_SOUP_WAIT_TIMEOUT 3000000//us
static GstStaticPadTemplate srctemplate = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS_ANY);

static void gst_sme_http_src_uri_handler_init (gpointer g_iface, const gpointer iface_data);

static gboolean
gst_sme_http_src_set_proxy (GstSmeHttpSrc * src, const gchar * uri);

static void
gst_sme_http_src_reset (GstSmeHttpSrc * src);

static void
gst_sme_http_src_init (GstSmeHttpSrc * src);

static void
gst_sme_http_src_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);

static void
gst_sme_http_src_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static void
gst_sme_http_src_parse_status (const SoupMessage * msg, GstSmeHttpSrc * src);

static void
gst_sme_http_src_got_headers_cb (const SoupMessage * msg, GstSmeHttpSrc * src);

static void
gst_sme_http_src_got_body_cb (const SoupMessage * msg, GstSmeHttpSrc * src);

static void
gst_sme_http_src_finished_cb (const SoupMessage * msg, GstSmeHttpSrc * src);

static void
gst_sme_http_src_chunk_free (gpointer user_data);

static SoupBuffer *
gst_sme_http_src_chunk_allocator (SoupMessage * msg, gsize max_len,
    gpointer user_data);

static void
gst_sme_http_src_got_chunk_cb (const SoupMessage * msg, SoupBuffer * chunk,
    GstSmeHttpSrc * src);

static void
gst_sme_http_src_response_cb (SoupSession * session, const SoupMessage * msg,
    GstSmeHttpSrc * src);

static void
gst_sme_http_src_headers_foreach (const gchar * name, const gchar * val,
    gpointer src);

static gboolean
gst_sme_http_src_build_message (GstSmeHttpSrc * src, const gchar * method);

static gboolean
gst_sme_http_src_add_range_header (GstSmeHttpSrc * src, guint64 offset,
    guint64 stop_offset);

static void
gst_sme_http_src_session_unpause_message (const GstSmeHttpSrc * src);

static void
gst_sme_http_src_session_pause_message (const GstSmeHttpSrc * src);

static void
gst_sme_http_src_queue_message (GstSmeHttpSrc * src);

static GstFlowReturn
gst_sme_http_src_create (GstBaseSrc * bsrc, guint64 offset,
    guint length, GstBuffer ** outbuf);

static GstFlowReturn
gst_sme_http_src_do_request (GstSmeHttpSrc * src, const gchar * method,
    guint64 offset, guint length, GstBuffer ** outbuf);

static void
gst_sme_http_src_cancel_message (GstSmeHttpSrc * src);

static void
gst_sme_http_src_check_seekable (GstSmeHttpSrc * src);

static void
gst_sme_http_src_authenticate_cb (SoupSession * session, const SoupMessage * msg,
    SoupAuth * auth, gboolean retrying, const GstSmeHttpSrc * src);

static gboolean
gst_sme_http_src_session_open (GstSmeHttpSrc * src);

static gboolean
gst_sme_http_src_get_size (GstBaseSrc * bsrc, guint64 * size);

static gboolean
gst_sme_http_src_start (GstBaseSrc * bsrc);

static gboolean
gst_sme_http_src_stop (GstBaseSrc * bsrc);

static gboolean
gst_sme_http_src_unlock (GstBaseSrc * bsrc);

static gboolean
gst_sme_http_src_unlock_stop (GstBaseSrc * bsrc);

static gboolean
gst_sme_http_src_do_seek (GstBaseSrc * bsrc, GstSegment * segment);

static gboolean
gst_sme_http_src_query (GstBaseSrc * bsrc, GstQuery * query);

static void
gst_sme_http_src_finalize (GObject * gobject);

static void
gst_sme_http_src_class_init (GstSmeHttpSrcClass * klass);

static gboolean
gst_sme_http_src_set_location (GstSmeHttpSrc * src, const gchar * uri,
    GError ** error);

static GstURIType
gst_sme_http_src_uri_get_type (GType type);

static const gchar *const *
gst_sme_http_src_uri_get_protocols (GType type);

static gchar *
gst_sme_http_src_uri_get_uri (GstURIHandler * handler);

static void
gst_sme_http_src_bitrate_watch_task (GstSmeHttpSrc *src);

static gboolean
gst_sme_http_src_uri_set_uri (GstURIHandler * handler, const gchar * uri,
    GError ** error);

static gboolean
gst_sme_http_src_get_data_from_prepared_buf (GstSmeHttpSrc * src,
        guint64 offset, guint length, GstBuffer ** outbuf);


#define gst_sme_http_src_parent_class parent_class
G_DEFINE_TYPE_WITH_CODE (GstSmeHttpSrc, gst_sme_http_src, GST_TYPE_BASE_SRC,
    G_IMPLEMENT_INTERFACE (GST_TYPE_URI_HANDLER,
        gst_sme_http_src_uri_handler_init));

#define  gst_sme_http_src_bandwidth_measurement \
{                                                  \
    if (src->bandwidth_entry.start ==0 && src->bandwidth_entry.end == 0) {  \
        src->bandwidth_entry.start = src->bandwidth_entry.end = g_get_real_time();  \
    } else {    \
        src->bandwidth_entry.end = g_get_real_time();   \
    }   \
    src->bandwidth_entry.transfer_bytes = src->bandwidth_entry.transfer_bytes + chunk->length;  \
    if( src->bandwidth_entry.end - src->bandwidth_entry.start >= src->bandwidth_entry.collect_freq)    \
    {   \
        src->bandwidth_entry.bandwidth = (gint32)((gint64)(src->bandwidth_entry.transfer_bytes)*8E6 /(src->bandwidth_entry.end  - src->bandwidth_entry.start)); \
        GST_DEBUG ("bandwidth:%d",src->bandwidth_entry.bandwidth); \
         gst_element_post_message (GST_ELEMENT (src),\
                gst_message_new_new_bandwidth(GST_OBJECT (src),\
                (guint) src->bandwidth_entry.bandwidth));\
        src->bandwidth_entry.start = src->bandwidth_entry.end  = 0; \
        src->bandwidth_entry.transfer_bytes = 0;    \
    }   \
    src->bandwidth_entry.get_data_timestamp = g_get_monotonic_time(); \
}

static guint64 get_cur_time(){
    guint64 ret_time = 0;
    GTimeVal gtv;

    gtv.tv_sec = 0;
    gtv.tv_usec = 0;

    g_get_current_time(&gtv);
    //for pclint by l00180035
    ret_time = (guint64)(gulong)gtv.tv_sec;
    ret_time = ret_time * 1000 + (guint64)(gulong)((gtv.tv_usec + 500) / 1000);

    return ret_time;
}

/* BEGIN: Modified for DTS2015100804623 by wanghua(w00347020), 2015/10/9 */
static void
_transport_soup_error_to_http(const SoupMessage * msg, GstSmeHttpSrc * src)
{
  if (msg->status_code == SOUP_STATUS_IO_ERROR || msg->status_code == SOUP_STATUS_CANT_CONNECT) {
    guint tmp_code;
    if (msg->status_code == SOUP_STATUS_IO_ERROR)
      tmp_code = SOUP_STATUS_REQUEST_TIMEOUT;
    else
      tmp_code = SOUP_STATUS_INTERNAL_SERVER_ERROR;
    if (src->last_status_code != msg->status_code) {
      gst_element_post_message (GST_ELEMENT (src),
          gst_message_new_http_err(GST_OBJECT (src),
          tmp_code));
      src->last_status_code = msg->status_code;
    }
  }
}
/* END: Modified for DTS2015100804623 by wanghua(w00347020), 2015/10/9 */

static gboolean
gst_sme_http_src_set_proxy (GstSmeHttpSrc * src, const gchar * uri)
{
    if (src->proxy) {
        soup_uri_free (src->proxy);
        src->proxy = NULL;
    }
    if (g_str_has_prefix (uri, "http://")) {
        src->proxy = soup_uri_new (uri);
    } else {
        gchar *new_uri = g_strconcat ("http://", uri, NULL);
        if (NULL == new_uri) {
            src->proxy = NULL;
        } else {
            src->proxy = soup_uri_new (new_uri);
        }
        g_free (new_uri);
    }

    return TRUE;
}

static void
gst_sme_http_src_reset (GstSmeHttpSrc * src)
{
    src->interrupted = FALSE;
    src->retry = FALSE;
    src->have_size = FALSE;
    src->got_headers = FALSE;
    src->is_getting_headers = FALSE;
    src->seekable = FALSE;
    src->read_position = 0;
    src->request_position = 0;
    src->stop_position = (guint64)(-1);
    src->content_size = 0;
    src->have_body = FALSE;
    src->bandwidth_entry.start = 0;
    src->bandwidth_entry.end = 0;
    src->bandwidth_entry.collect_freq = BANDWIDTH_COLLECT_FREQ;
    src->bandwidth_entry.transfer_bytes = 0;
    src->bandwidth_entry.bandwidth = -1;

    gst_caps_replace (&src->src_caps, NULL);
    g_free (src->iradio_name);
    src->iradio_name = NULL;
    g_free (src->iradio_genre);
    src->iradio_genre = NULL;
    g_free (src->iradio_url);
    src->iradio_url = NULL;
}

static void
gst_sme_http_src_init (GstSmeHttpSrc * src)
{
    const gchar *proxy = NULL;

    g_mutex_init (&src->mutex);
    g_cond_init (&src->request_finished_cond);
    src->location = NULL;
    src->redirection_uri = NULL;
    src->automatic_redirect = TRUE;
    src->user_agent = g_strdup (DEFAULT_USER_AGENT);
    src->user_id = NULL;
    src->user_pw = NULL;
    src->proxy_id = NULL;
    src->proxy_pw = NULL;
    src->cookies = NULL;
    src->iradio_mode = DEFAULT_IRADIO_MODE;
    src->loop = NULL;
    src->context = NULL;
    src->session = NULL;
    src->cur_item = NULL;
    g_mutex_init (&src->msg_mutex);
    src->is_finished = FALSE;
    src->msg = NULL;
    src->preread_buffer = NULL;
    src->byte_range = TRUE;
    src->referer = NULL;
    /* BEGIN: Modified for DTS2015100804623 by wanghua(w00347020), 2015/10/9 */
    src->last_status_code = 0;
    /* END: Modified for DTS2015100804623 by wanghua(w00347020), 2015/10/9 */
    proxy = g_getenv ("http_proxy");
    if (proxy && !gst_sme_http_src_set_proxy (src, proxy)) {
        GST_WARNING_OBJECT (src,
            "The proxy in the http_proxy env var cannot be parsed.");
    }

    gst_sme_http_src_reset (src);
    gst_base_src_set_blocksize (GST_BASE_SRC(src), DEFAULT_HTTP_BUF_BLOCK_SIZE);
    src->is_query_schedule = FALSE;
    src->is_getting_headers = FALSE;
    src->is_first_chunck = TRUE;
    src->start_req_time = G_MAXUINT64;
    src->bitrate_watch_task =
        gst_task_new ((GstTaskFunction) gst_sme_http_src_bitrate_watch_task, src, NULL);
    g_rec_mutex_init (&src->bitrate_watch_task_lock);
    gst_task_set_lock (src->bitrate_watch_task, &src->bitrate_watch_task_lock);
    src->is_download_complete = FALSE;
    src->bandwidth_entry.get_data_timestamp = 0;
}

static void
gst_sme_http_src_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
    GstSmeHttpSrc *src = GST_SME_HTTP_SRC (object);

    switch (prop_id) {
        case PROP_LOCATION:
        {
            const gchar *location;

            location = g_value_get_string (value);

            if (location == NULL) {
                GST_WARNING ("location property cannot be NULL");
                goto done;
            }
            if (!gst_sme_http_src_set_location (src, location, NULL)) {
                GST_WARNING ("badly formatted location");
                goto done;
            }
            break;
        }
        case PROP_USER_AGENT:
            if (src->user_agent)
                g_free (src->user_agent);
            src->user_agent = g_value_dup_string (value);
            break;
        case PROP_BYTE_RANGE:
            src->byte_range = g_value_get_boolean (value);
            break;
        case PROP_REFERER:
            if (src->referer)
                g_free (src->referer);
            src->referer = g_value_dup_string (value);
            break;
        case PROP_IRADIO_MODE:
            src->iradio_mode = g_value_get_boolean (value);
            break;
        case PROP_AUTOMATIC_REDIRECT:
            src->automatic_redirect = g_value_get_boolean (value);
            break;
        case PROP_PROXY:
        {
            const gchar *proxy;

            proxy = g_value_get_string (value);

            if (proxy == NULL) {
                GST_WARNING ("proxy property cannot be NULL");
                goto done;
            }
            if (!gst_sme_http_src_set_proxy (src, proxy)) {
                GST_WARNING ("badly formatted proxy URI");
                goto done;
            }
            break;
        }
        case PROP_COOKIES:
            g_strfreev (src->cookies);
            src->cookies = g_strdupv (g_value_get_boxed (value));
            break;
        case PROP_IS_LIVE:
            gst_base_src_set_live (GST_BASE_SRC (src), g_value_get_boolean (value));
            break;
        case PROP_USER_ID:
            if (src->user_id)
                g_free (src->user_id);
            src->user_id = g_value_dup_string (value);
            break;
        case PROP_USER_PW:
            if (src->user_pw)
                g_free (src->user_pw);
            src->user_pw = g_value_dup_string (value);
            break;
        case PROP_PROXY_ID:
            if (src->proxy_id)
                g_free (src->proxy_id);
            src->proxy_id = g_value_dup_string (value);
            break;
        case PROP_PROXY_PW:
            if (src->proxy_pw)
                g_free (src->proxy_pw);
            src->proxy_pw = g_value_dup_string (value);
            break;
        case PROP_TIMEOUT:
            src->timeout = g_value_get_uint (value);
            GST_INFO("set timeout value:%d",src->timeout);
            break;
        case PROP_EXTRA_HEADERS:{
            const GstStructure *s = gst_value_get_structure (value);

            if (src->extra_headers)
                gst_structure_free (src->extra_headers);

            src->extra_headers = s ? gst_structure_copy (s) : NULL;
            break;
        }
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
      }
    done:
        return;
}

static void
gst_sme_http_src_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
    GstSmeHttpSrc *src = GST_SME_HTTP_SRC (object);

    switch (prop_id) {
        case PROP_LOCATION:
            g_value_set_string (value, src->location);
            break;
        case PROP_USER_AGENT:
            g_value_set_string (value, src->user_agent);
            break;
        case PROP_BYTE_RANGE:
            g_value_set_boolean (value, src->byte_range);
            break;
        case PROP_REFERER:
            g_value_set_string (value, src->referer);
            break;
        case PROP_AUTOMATIC_REDIRECT:
            g_value_set_boolean (value, src->automatic_redirect);
            break;
        case PROP_PROXY:
            if (src->proxy == NULL)
                g_value_set_static_string (value, "");
            else {
                char *proxy = soup_uri_to_string (src->proxy, FALSE);

                g_value_set_string (value, proxy);
                g_free (proxy);
            }
            break;
        case PROP_COOKIES:
            /* BEGIN: Modified for DTS2016042502439, 2016/5/10 */
            g_value_set_boxed (value, src->cookies);
            /* END: Modified for DTS2016042502439, 2016/5/10 */
            break;
        case PROP_IS_LIVE:
            g_value_set_boolean (value, gst_base_src_is_live (GST_BASE_SRC (src)));
            break;
        case PROP_IRADIO_MODE:
            g_value_set_boolean (value, src->iradio_mode);
            break;
        case PROP_USER_ID:
            g_value_set_string (value, src->user_id);
            break;
        case PROP_USER_PW:
            g_value_set_string (value, src->user_pw);
            break;
        case PROP_PROXY_ID:
            g_value_set_string (value, src->proxy_id);
            break;
        case PROP_PROXY_PW:
            g_value_set_string (value, src->proxy_pw);
            break;
        case PROP_TIMEOUT:
            g_value_set_uint (value, src->timeout);
            break;
        case PROP_BANDWIDTH:
            GST_DEBUG("rc->bandwidth_entry.bandwidth:%d",src->bandwidth_entry.bandwidth);
            g_value_set_uint (value, (guint)(src->bandwidth_entry.bandwidth));
            break;
        case PROP_EXTRA_HEADERS:
            gst_value_set_structure (value, src->extra_headers);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}


/*
static void gst_sme_element_error(GstElement * element, GQuark domain, gint code, gchar * message)
{
    GError* err = NULL;
    GstMessage *gst_message = NULL;

    g_return_if_fail (message != NULL);
    g_return_if_fail (domain != 0);
    g_return_if_fail (GST_IS_ELEMENT (element));

    err = g_slice_new (GError);

    if (NULL != err) {
        err->domain = domain;
        err->code = code;
        err->message = g_strdup (message);
        gst_message = gst_message_new_error (GST_OBJECT_CAST (element), err, NULL);
        if (NULL != gst_message) {
            gst_element_post_message (element, gst_message);
        }
    }
    g_error_free (err);
}
*/
static void
gst_sme_http_src_parse_status (const SoupMessage * msg, GstSmeHttpSrc * src)
{
    if (msg->method == SOUP_METHOD_HEAD) {
        if (!SOUP_STATUS_IS_SUCCESSFUL (msg->status_code)) {
            if(!src->interrupted){
                GST_ELEMENT_ERROR (src, RESOURCE, NOT_FOUND,
                    ("reason"),
                    ("libsoup status code %d", msg->status_code));
            }else{
                GST_WARNING("libsoup status code %d", msg->status_code);
            }
        } else {
             GST_WARNING("libsoup method status code %d", msg->status_code);
        }
    } else if (SOUP_STATUS_IS_TRANSPORT_ERROR (msg->status_code)) {
        switch (msg->status_code) {
            case SOUP_STATUS_CANT_RESOLVE:
            case SOUP_STATUS_CANT_RESOLVE_PROXY:
                GST_ERROR_OBJECT (src,"Could not resolve server name.");
                src->ret = GST_FLOW_ERROR;
                break;
            case SOUP_STATUS_CANT_CONNECT:
            case SOUP_STATUS_CANT_CONNECT_PROXY:
                GST_ERROR_OBJECT (src, "Could not establish connection to server.");
                src->ret = GST_FLOW_ERROR;
                break;
            case SOUP_STATUS_SSL_FAILED:
                GST_ERROR_OBJECT (src, "Secure connection setup failed.");
                src->ret = GST_FLOW_ERROR;
                break;
            case SOUP_STATUS_IO_ERROR:
                GST_ERROR_OBJECT (src, "Soup io err,reason, libsoup status = %d",
                    msg->status_code);
                src->ret = GST_FLOW_ERROR;
                break;
            case SOUP_STATUS_MALFORMED:
                GST_ERROR_OBJECT (src, "Server sent bad data.");
                src->ret = GST_FLOW_ERROR;
                break;
            case SOUP_STATUS_CANCELLED:
                /* No error message when interrupted by program. */
                GST_WARNING_OBJECT (src, "SOUP_STATUS_CANCELLED.");
                break;
            default:
                GST_WARNING_OBJECT (src, "msg->status_code=%d.", msg->status_code);
                break;
        }
    } else if (SOUP_STATUS_IS_CLIENT_ERROR (msg->status_code) ||
        SOUP_STATUS_IS_REDIRECTION (msg->status_code) ||
        SOUP_STATUS_IS_SERVER_ERROR (msg->status_code)) {
          /* Report HTTP error. */

          /* when content_size is unknown and we have just finished receiving
           * a body message, requests that go beyond the content limits will result
           * in an error. Here we convert those to EOS */
          if (msg->status_code == SOUP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE &&
              src->have_body && src->have_size) {
              GST_DEBUG_OBJECT (src, "Requested range out of limits and received full "
                  "body, returning EOS");
              src->ret = GST_FLOW_EOS;
              return;
          }

          /*
            if not utf-8 we do not operation as string.
          */
          if(!g_utf8_validate(src->location, -1, NULL)){
            GST_WARNING("not utf-8 uri");
          }
          /* FIXME: reason_phrase is not translated and not suitable for user
           * error dialog according to libsoup documentation.
           * FIXME: error code (OPEN_READ vs. READ) should depend on http status? */
          gst_element_post_message (GST_ELEMENT (src),
              gst_message_new_http_err(GST_OBJECT (src),
              msg->status_code));
          /* BEGIN: Modified for DTS2015102210493 by liurongliang(l00180035), 2015/10/23 */
          //remove uri log for the security request.
          if(!src->interrupted){
              GST_ELEMENT_ERROR (src, RESOURCE, OPEN_READ,
                  ("reason"),
                  ("reason (%d)", msg->status_code));
          }else{
              GST_WARNING("libsoup status code %d", msg->status_code);
          }
          /* END:   Modified for DTS2015102210493 by liurongliang(l00180035), 2015/10/23 */

          src->ret = GST_FLOW_ERROR;
    }
}

static void
gst_sme_http_src_got_headers_cb (const SoupMessage * msg, GstSmeHttpSrc * src)
{
    const char *value = NULL;
    GstTagList *tag_list = NULL;
    guint64 newsize;
    GHashTable *params = NULL;

    GST_INFO_OBJECT (src, "got headers:");

    /* BEGIN: Modified for DTS2015100804657 by wanghua(w00347020), 2015/10/9 */
    if (src->last_status_code != msg->status_code &&
        (msg->status_code >= SOUP_STATUS_MULTIPLE_CHOICES
        && msg->status_code <= SOUP_STATUS_NOT_EXTENDED)) {
     gst_element_post_message (GST_ELEMENT (src),
           gst_message_new_http_err(GST_OBJECT (src),
           msg->status_code));
     src->last_status_code = msg->status_code;
    }
    /* END: Modified for DTS2015100804657 by wanghua(w00347020), 2015/10/9 */
    soup_message_headers_foreach (msg->response_headers,
        gst_sme_http_src_headers_foreach, src);

    if (msg->status_code == 407 && src->proxy_id && src->proxy_pw)
      return;

    if (src->automatic_redirect && SOUP_STATUS_IS_REDIRECTION (msg->status_code)) {
        src->redirection_uri = g_strdup (soup_message_headers_get_one
            (msg->response_headers, "Location"));
        return;
  }

  if (msg->status_code == SOUP_STATUS_UNAUTHORIZED)
    return;

  src->session_io_status = GST_SME_HTTP_SRC_SESSION_IO_STATUS_RUNNING;
  src->got_headers = TRUE;
  src->is_getting_headers = FALSE;

  /* Parse Content-Length. */
  if (soup_message_headers_get_encoding (msg->response_headers) ==
      SOUP_ENCODING_CONTENT_LENGTH) {
      newsize = src->request_position +
          (guint64)soup_message_headers_get_content_length (msg->response_headers);
      if (!src->have_size || (src->content_size != newsize)) {
          src->content_size = newsize;
          /* BEGIN: Modified for DTS2016042807048, 2016/5/9 */
          if (src->last_status_code >= 400 && src->last_status_code < 600)
              src->content_size = 0;
          /* END: Modified for DTS2016042807048, 2016/5/9 */
          src->have_size = TRUE;
          src->seekable = TRUE;
          GST_INFO_OBJECT (src, "size = %" G_GUINT64_FORMAT, src->content_size);

          /* BEGIN: Modified for DTS2016042502439, 2016/5/10 */
          GstBaseSrc *basesrc;
          /* END: Modified for DTS2016042502439, 2016/5/10 */
          basesrc = GST_BASE_SRC_CAST (src);
          basesrc->segment.duration = src->content_size;
          gst_element_post_message (GST_ELEMENT (src),
              gst_message_new_duration_changed (GST_OBJECT (src)));
      }
  }

  /* Icecast stuff */
  tag_list = gst_tag_list_new_empty ();

  if ((value =
          soup_message_headers_get_one (msg->response_headers,
              "icy-metaint")) != NULL) {
      gint icy_metaint = atoi (value);
      GST_INFO_OBJECT (src, "icy-metaint (parsed: %d)", icy_metaint);
      if (icy_metaint > 0) {
          if (src->src_caps)
            gst_caps_unref (src->src_caps);

          src->src_caps = gst_caps_new_simple ("application/x-icy",
              "metadata-interval", G_TYPE_INT, icy_metaint, NULL);

          gst_base_src_set_caps (GST_BASE_SRC (src), src->src_caps);
      }
  }
  if ((value =
          soup_message_headers_get_content_type (msg->response_headers,
              &params)) != NULL) {
      GST_DEBUG_OBJECT (src, "Content-Type");
      if (g_ascii_strcasecmp (value, "audio/L16") == 0) {
          gint channels = 2;
          gint rate = 44100;
          char *param;

          if (src->src_caps)
            gst_caps_unref (src->src_caps);

          param = g_hash_table_lookup (params, "channels");
          if (param != NULL)
            channels = atol (param);

          param = g_hash_table_lookup (params, "rate");
          if (param != NULL)
            rate = atol (param);

          src->src_caps = gst_caps_new_simple ("audio/x-raw",
              "format", G_TYPE_STRING, "S16BE",
              "layout", G_TYPE_STRING, "interleaved",
              "channels", G_TYPE_INT, channels, "rate", G_TYPE_INT, rate, NULL);

          gst_base_src_set_caps (GST_BASE_SRC (src), src->src_caps);
      } else {
          /* Set the Content-Type field on the caps */
          if (src->src_caps) {
              src->src_caps = gst_caps_make_writable (src->src_caps);
              gst_caps_set_simple (src->src_caps, "content-type", G_TYPE_STRING,
                  value, NULL);
              gst_base_src_set_caps (GST_BASE_SRC (src), src->src_caps);
          }
      }
  }

  if (params != NULL)
      g_hash_table_destroy (params);

  if (!gst_tag_list_is_empty (tag_list)) {
      GST_INFO_OBJECT (src,
          "calling gst_element_found_tags with %" GST_PTR_FORMAT, tag_list);
      gst_pad_push_event (GST_BASE_SRC_PAD (src), gst_event_new_tag (tag_list));
  } else {
      gst_tag_list_unref (tag_list);
  }

  /* Handle HTTP errors. */
  gst_sme_http_src_parse_status (msg, src);
  /* BEGIN: Added for DTS2016042808251, 2016/4/28 */
  _transport_soup_error_to_http(msg, src);
  /* END: Added for DTS2016042808251, 2016/4/28 */
  /* Check if Range header was respected. */
  if (src->ret == GST_FLOW_CUSTOM_ERROR &&
      src->read_position && msg->status_code != SOUP_STATUS_PARTIAL_CONTENT) {
      src->seekable = FALSE;
      if(!src->interrupted){
          GST_ELEMENT_ERROR (src, RESOURCE, SEEK,
              ("Server does not support seeking."),
              ("Server does not accept Range HTTP header"));
      }else{
          GST_WARNING("Server does not accept Range HTTP header");
      }
      src->ret = GST_FLOW_ERROR;
  }

  /* If we are going to error out, stop all processing right here, so we
   * don't output any data (such as an error html page), and return
   * GST_FLOW_ERROR from the create function instead of having
   * got_chunk_cb overwrite src->ret with FLOW_OK again. */
  if (src->ret == GST_FLOW_ERROR || src->ret == GST_FLOW_EOS) {
      gst_sme_http_src_session_pause_message (src);

      if (src->loop)
          g_main_loop_quit (src->loop);
  }
  src->ret = GST_FLOW_EOS;
  //g_main_loop_quit (src->loop);
  g_cond_signal (&src->request_finished_cond);
}

/* Have body. Signal EOS. */
static void
gst_sme_http_src_got_body_cb (const SoupMessage * msg, GstSmeHttpSrc * src)
{
    if (G_UNLIKELY (msg != src->msg)) {
        GST_DEBUG_OBJECT (src, "got body, but not for current message");
        return;
    }
    if (G_UNLIKELY (src->session_io_status !=
            GST_SME_HTTP_SRC_SESSION_IO_STATUS_RUNNING)) {
      /* Probably a redirect. */
        return;
    }
    GST_INFO_OBJECT (src, "got body");
    src->ret = GST_FLOW_EOS;
    src->have_body = TRUE;

    /* no need to interrupt the message here, we do it on the
     * finished_cb anyway if needed. And getting the body might mean
     * that the connection was hang up before finished. This happens when
     * the pipeline is stalled for too long (long pauses during playback).
     * Best to let it continue from here and pause because it reached the
     * final bytes based on content_size or received an out of range error */
}

/* Finished. Signal EOS. */
static void
gst_sme_http_src_finished_cb (const SoupMessage * msg, GstSmeHttpSrc * src)
{
    g_mutex_lock (&src->msg_mutex);
    src->is_finished = TRUE;
    g_cond_signal(&src->request_finished_cond);
    GST_INFO("gst_sme_http_src_finished_cb g_cond_signal !");
    g_mutex_unlock (&src->msg_mutex);


    if (G_UNLIKELY (msg != src->msg)) {
        GST_WARNING_OBJECT (src, "finished, but not for current message");
        return;
    }
    GST_INFO_OBJECT (src, "finished,soup status code = %d", msg->status_code);
    src->ret = GST_FLOW_EOS;

    if (src->session_io_status == GST_SME_HTTP_SRC_SESSION_IO_STATUS_CANCELLED) {
      /* gst_soup_http_src_cancel_message() triggered this; probably a seek
       * that occurred in the QUEUEING state; i.e. before the connection setup
       * was complete. Do nothing */
    } else if (src->session_io_status ==
        GST_SME_HTTP_SRC_SESSION_IO_STATUS_RUNNING && src->read_position > 0 &&
        (!src->have_size || src->read_position < src->content_size)) {
      /* The server disconnected while streaming. Reconnect and seeking to the
       * last location. */
        src->retry = TRUE;
        src->ret = GST_FLOW_CUSTOM_ERROR;
    } else if (G_UNLIKELY (src->session_io_status !=
            GST_SME_HTTP_SRC_SESSION_IO_STATUS_RUNNING)) {
      if (msg->method == SOUP_METHOD_HEAD) {
          GST_INFO_OBJECT (src, "Ignoring error %d during HEAD request",
              msg->status_code);
      } else {
        /* FIXME: reason_phrase is not translated, add proper error message */
          /* BEGIN: Modified for DTS2014102102466 by liurongliang(l00180035), 2014/10/27 */
          guint64 cur_time = get_cur_time() / 1000;
          if(src->start_req_time != G_MAXUINT64
            && (cur_time + TIMEOUT_DEVIATION < src->start_req_time + (guint64)((guint)src->timeout))
            && (SOUP_STATUS_CANT_CONNECT == msg->status_code
            || SOUP_STATUS_IO_ERROR == msg->status_code)
            )  {
              src->retry = TRUE;
              src->ret = GST_FLOW_CUSTOM_ERROR;
              GST_WARNING("SOUP_STATUS_CANT_CONNECT retry");
              g_usleep(20000);
            }else{
                src->start_req_time = G_MAXUINT64;
                /* BEGIN: Modified for DTS2015100804623 by wanghua(w00347020), 2015/10/9 */
                _transport_soup_error_to_http(msg, src);
                /* END: Modified for DTS2015100804623 by wanghua(w00347020), 2015/10/9 */
                if(!src->interrupted){
                    GST_ELEMENT_ERROR (src, RESOURCE, NOT_FOUND,
                        ("reason"),
                        ("libsoup status code %d,iostatus=%d",
                        msg->status_code, src->session_io_status));
                }else{
                    GST_WARNING("libsoup status code %d,iostatus=%d",
                        msg->status_code, src->session_io_status);
                }
          }
          /* END:   Modified for DTS2014102102466 by liurongliang(l00180035), 2014/10/27 */
      }
    }
    if (src->loop)
        g_main_loop_quit (src->loop);
    g_cond_signal (&src->request_finished_cond);
    GST_INFO_OBJECT (src, "finished,soup status code = %d out", msg->status_code);
}

static void
gst_sme_http_src_chunk_free (gpointer user_data)
{
    SmeGstChunk *chunk = (SmeGstChunk *) user_data;
    gst_buffer_unmap (chunk->buffer, &chunk->map);
    gst_buffer_unref (chunk->buffer);
    g_slice_free (SmeGstChunk, chunk);
}

static SoupBuffer *
gst_sme_http_src_chunk_allocator (SoupMessage * msg, gsize max_len,
    gpointer user_data)
{
    (void)msg;
    GstSmeHttpSrc *src = (GstSmeHttpSrc *) user_data;
    GstBaseSrc *basesrc = GST_BASE_SRC_CAST (src);
    GstBuffer *gstbuf = NULL;
    SoupBuffer *soupbuf = NULL;
    gsize length;
    GstFlowReturn rc;
    SmeGstChunk *chunk = NULL;

    if (max_len)
        length = MIN (basesrc->blocksize, max_len);
    else
        length = basesrc->blocksize;


    rc = GST_BASE_SRC_CLASS (parent_class)->alloc (basesrc, (guint64)-1, length, &gstbuf);
    if (G_UNLIKELY (rc != GST_FLOW_OK)) {
        GST_ERROR("Failed to allocate sme http chunk buffer");
        src->ret = rc;
        g_main_loop_quit (src->loop);
        return NULL;
    }
    GST_DEBUG_OBJECT (src, "alloc %" G_GSIZE_FORMAT " bytes <= %" G_GSIZE_FORMAT",gstbuf=%p",
        length, max_len, gstbuf);

    chunk = g_slice_new0 (SmeGstChunk);

    if (G_UNLIKELY(NULL == chunk)) {
        GST_ERROR("Failed to allocate sme http chunk structure");
        gst_buffer_unref (gstbuf);
        g_main_loop_quit (src->loop);
        return NULL;
    }

    chunk->buffer = gstbuf;
    gst_buffer_map (gstbuf, &chunk->map, GST_MAP_READWRITE);

    soupbuf = soup_buffer_new_with_owner (chunk->map.data, chunk->map.size,
        chunk, gst_sme_http_src_chunk_free);

    return soupbuf;
}

static void
gst_sme_http_src_got_chunk_cb (const SoupMessage * msg, SoupBuffer * chunk,
    GstSmeHttpSrc * src)
{
    GstBaseSrc *basesrc = NULL;
    guint64 new_position;
    SmeGstChunk *gchunk = NULL;

    if (G_UNLIKELY (msg != src->msg)) {
        GST_INFO_OBJECT (src, "got chunk, but not for current message");
        return;
    }
    src->have_body = FALSE;
    if (G_UNLIKELY (src->session_io_status !=
            GST_SME_HTTP_SRC_SESSION_IO_STATUS_RUNNING)) {
      /* Probably a redirect. */
        return;
    }
    basesrc = GST_BASE_SRC_CAST (src);
    GST_DEBUG_OBJECT (src, "got chunk of %" G_GSIZE_FORMAT " bytes",
        chunk->length);

    /* Extract the GstBuffer from the SoupBuffer and set its fields. */
    gchunk = (SmeGstChunk *) soup_buffer_get_owner (chunk);
    if (G_UNLIKELY(NULL == gchunk)) {
        GST_ERROR("Extract the GstBuffer from the SoupBuffer error");
        src->ret = GST_FLOW_OK;
        g_main_loop_quit (src->loop);
        gst_sme_http_src_session_pause_message (src);
        return;
    }
    GST_DEBUG("src->outbuf:%p",src->outbuf);
    if (!src->is_first_chunck && NULL != src->outbuf) {
        *src->outbuf = gchunk->buffer;

        gst_sme_http_src_bandwidth_measurement

        gst_buffer_resize (*src->outbuf, (gssize)0, (gssize)chunk->length);
        GST_BUFFER_OFFSET (*src->outbuf) = basesrc->segment.position;

        gst_buffer_ref (*src->outbuf);

        new_position = src->read_position + chunk->length;

        if (G_LIKELY (src->request_position == src->read_position))
            src->request_position = new_position;
        src->read_position = new_position;

        if (src->content_size != 0 && new_position > src->content_size) {
            GST_DEBUG_OBJECT (src, "Got position previous estimated content size "
                "(%" G_GINT64_FORMAT " > %" G_GINT64_FORMAT ")", new_position,
                src->content_size);
            src->content_size = new_position;
            basesrc->segment.duration = src->content_size;
            gst_element_post_message (GST_ELEMENT (src),
                gst_message_new_duration_changed (GST_OBJECT (src)));
        }

        src->ret = GST_FLOW_OK;
        g_main_loop_quit (src->loop);
        gst_sme_http_src_session_pause_message (src);
    } else {
        gst_buffer_resize (gchunk->buffer, (gssize)0, (gssize)chunk->length);
        if (NULL == src->preread_buffer) {
            src->preread_buffer = gst_buffer_ref(gchunk->buffer);
        } else {
            /** BEGIN: Modified for DTS2015042206034  by baihaili(b00311320), 2015/5/29 */
            gst_buffer_ref(gchunk->buffer);
            /** END:   Modified for DTS2015042206034 by baihaili(b00311320), 2015/5/29 */
            src->preread_buffer = gst_buffer_append(src->preread_buffer,
                gchunk->buffer);
        }

        if (NULL == src->preread_buffer)
            return;

        gsize size = gst_buffer_get_size(src->preread_buffer);
        GST_BUFFER_OFFSET (src->preread_buffer) = basesrc->segment.position;
        src->read_position = size;

        GST_INFO("=src->preread_buffer:%p,preread@buffer:%d",
            src->preread_buffer, size);
        if (size >= 4096 || size >= src->content_size)
        {
            src->ret = GST_FLOW_OK;
            if(src->is_first_chunck){
                src->is_first_chunck = FALSE;
            }
            g_main_loop_quit (src->loop);
            gst_sme_http_src_session_pause_message (src);
        }
    }


}

static void
gst_sme_http_src_response_cb (SoupSession * session, const SoupMessage * msg,
    GstSmeHttpSrc * src)
{
    (void)session;

    if (G_UNLIKELY (msg != src->msg)) {
        GST_WARNING_OBJECT (src, "got response %d, but not for current message",
            msg->status_code);
        return;
    }
    if (G_UNLIKELY (src->session_io_status !=
            GST_SME_HTTP_SRC_SESSION_IO_STATUS_RUNNING)
        && SOUP_STATUS_IS_REDIRECTION (msg->status_code)) {
      /* Ignore redirections. */
        return;
    }
    GST_INFO_OBJECT (src, "got response %d", msg->status_code);
    if (src->session_io_status == GST_SME_HTTP_SRC_SESSION_IO_STATUS_RUNNING &&
        src->read_position > 0) {
      /* The server disconnected while streaming. Reconnect and seeking to the
       * last location. */
        src->retry = TRUE;
    } else{
        GST_INFO ("gst_sme_http_src_parse_status");
        gst_sme_http_src_parse_status (msg, src);
        /* BEGIN: Modified for DTS2014102102466 by liurongliang(l00180035), 2014/10/27 */
        if(SOUP_STATUS_CANT_CONNECT == msg->status_code
            || SOUP_STATUS_IO_ERROR == msg->status_code){
            guint64 cur_time = get_cur_time() / 1000;
            if(src->start_req_time != G_MAXUINT64
            && (cur_time + TIMEOUT_DEVIATION < src->start_req_time + (guint64)((guint)src->timeout)))
            {
                src->retry = TRUE;
                src->ret = GST_FLOW_CUSTOM_ERROR;
                GST_WARNING("SOUP_STATUS_CANT_CONNECT retry");
                g_usleep(20000);
            }else{
                src->start_req_time = G_MAXUINT64;
                src->retry = FALSE;
                if(!src->interrupted){
                    GST_ELEMENT_ERROR (src, RESOURCE, NOT_FOUND,
                        ("reason"),
                        ("libsoup status code %d", msg->status_code));
                }else{
                    GST_WARNING("libsoup status code %d", msg->status_code);
                }
                /* BEGIN: Added for DTS2016042808251, 2016/4/28 */
                _transport_soup_error_to_http(msg, src);
                /* END: Added for DTS2016042808251, 2016/4/28 */
            }
        }
        /* END:   Modified for DTS2014102102466 by liurongliang(l00180035), 2014/10/27 */
    }
    /* The session's SoupMessage object expires after this callback returns. */

    //we owned msg, so we should unref it when cancel this msg or abort the session.
    //src->msg = NULL;
    g_main_loop_quit (src->loop);
}

static void
gst_sme_http_src_headers_foreach (const gchar * name, const gchar * val,
    gpointer src)
{
    (void)name;
    (void)val;
    (void)src;
}

static gboolean
gst_sme_http_src_build_message (GstSmeHttpSrc * src, const gchar * method)
{
   g_mutex_lock (&src->msg_mutex);
   if (src->msg)
     gst_sme_http_src_cancel_message(src);

    src->msg = soup_message_new (method, src->location);
    if (!src->msg) {
        g_mutex_unlock (&src->msg_mutex);
        GST_ELEMENT_ERROR (src, RESOURCE, OPEN_READ,
            ("Error parsing URL."), ("URL"));
        return FALSE;
    }

    GST_INFO("soup_message_new session = %p, msg=%p", src->session, src->msg);
    src->session_io_status = GST_SME_HTTP_SRC_SESSION_IO_STATUS_IDLE;
    soup_message_headers_append (src->msg->request_headers, "Connection",
        "close");
    if (src->iradio_mode) {
        soup_message_headers_append (src->msg->request_headers, "icy-metadata",
            "1");
    }
    if (src->cookies) {
        gchar **cookie;

        for (cookie = src->cookies; *cookie != NULL; cookie++) {
            soup_message_headers_append (src->msg->request_headers, "Cookie",
                *cookie);
        }
    }

    if (src->referer) {
      soup_message_headers_append (src->msg->request_headers, "Referer",
                src->referer);
    }

    src->retry = FALSE;

    g_signal_connect (src->msg, "got_headers",
        G_CALLBACK (gst_sme_http_src_got_headers_cb), src);
    g_signal_connect (src->msg, "got_body",
        G_CALLBACK (gst_sme_http_src_got_body_cb), src);
    g_signal_connect (src->msg, "finished",
        G_CALLBACK (gst_sme_http_src_finished_cb), src);
    g_signal_connect (src->msg, "got_chunk",
        G_CALLBACK (gst_sme_http_src_got_chunk_cb), src);
    soup_message_set_flags (src->msg, SOUP_MESSAGE_OVERWRITE_CHUNKS |
        (src->automatic_redirect ? 0 : SOUP_MESSAGE_NO_REDIRECT));
    soup_message_set_chunk_allocator (src->msg,
        gst_sme_http_src_chunk_allocator, src, NULL);
    gst_sme_http_src_add_range_header (src, src->request_position,
        src->stop_position);

    //gst_sme_http_src_add_extra_headers (src);

    GST_DEBUG_OBJECT (src, "request headers:");
    soup_message_headers_foreach (src->msg->request_headers,
        gst_sme_http_src_headers_foreach, src);
    g_mutex_unlock (&src->msg_mutex);

    return TRUE;
}

static gboolean
gst_sme_http_src_add_range_header (GstSmeHttpSrc * src, guint64 offset,
    guint64 stop_offset)
{
    gchar buf[64] = {0,};

    gint rc;

    soup_message_headers_remove (src->msg->request_headers, "Range");
    if (offset || stop_offset != (guint64)-1) {
        if (stop_offset != (guint64)-1) {
            rc = g_snprintf (buf, sizeof (buf), "bytes=%" G_GUINT64_FORMAT "-%"
                G_GUINT64_FORMAT, offset, stop_offset);
        } else {
            rc = g_snprintf (buf, sizeof (buf), "bytes=%" G_GUINT64_FORMAT "-",
                offset);
        }
        if (rc > (gint)sizeof (buf) || rc < 0)
            return FALSE;

        if (src->byte_range)
          soup_message_headers_append (src->msg->request_headers, "Range", buf);
    }
    src->read_position = offset;
    return TRUE;
}

static void
gst_sme_http_src_session_unpause_message (const GstSmeHttpSrc * src)
{
    soup_session_unpause_message (src->session, src->msg);
}

static void
gst_sme_http_src_session_pause_message (const GstSmeHttpSrc * src)
{
    soup_session_pause_message (src->session, src->msg);
}

static void
gst_sme_http_src_queue_message (GstSmeHttpSrc * src)
{
    SoupMessageQueue* msgqueue = NULL;

    //we should ref this msg and msg_item, so that we can hold them.
    g_mutex_lock (&src->msg_mutex);
    src->is_finished = FALSE;
    soup_session_queue_message (src->session, g_object_ref(src->msg),
        (SoupSessionCallback) gst_sme_http_src_response_cb, src);
    src->session_io_status = GST_SME_HTTP_SRC_SESSION_IO_STATUS_QUEUED;
    if(src->cur_item){
        soup_message_queue_item_unref(src->cur_item);
        src->cur_item = NULL;
    }
    msgqueue = soup_session_get_queue(src->session);
    if(msgqueue){
        src->cur_item =  soup_message_queue_lookup(msgqueue, src->msg);
        GST_INFO("get an msg item:%p, cancel=%p",
            src->cur_item, src->cur_item ? src->cur_item->cancellable:0x0);
    }else{
        GST_INFO("not get an item !");
    }
    g_mutex_unlock (&src->msg_mutex);
}

/* BEGIN: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
static void
gst_sme_http_src_bitrate_watch_task (GstSmeHttpSrc *src)
{
    gint64 cur_time = g_get_monotonic_time();
    static gboolean notify_flag = FALSE;

    if (!notify_flag
        && src
        && !src->is_download_complete
        && src->bandwidth_entry.get_data_timestamp
        && src->bandwidth_entry.get_data_timestamp + BANDWIDTH_UPDATE_TIMEOUT < cur_time) {
        GST_WARNING ("No data received for %d us,set download bitreate 0",
                     BANDWIDTH_UPDATE_TIMEOUT);
        GST_ELEMENT_WARNING (src, RESOURCE, NO_DATA, (NULL), (NULL));
        notify_flag = TRUE;
    }
    else if (src
             && src->bandwidth_entry.get_data_timestamp + BANDWIDTH_UPDATE_TIMEOUT > cur_time) {
        notify_flag = FALSE;
    }

    g_usleep(20000);//20ms
}
/* END: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */

static GstFlowReturn
gst_sme_http_src_create (GstBaseSrc * bsrc, guint64 offset,
        guint length, GstBuffer ** outbuf)
{
    GstSmeHttpSrc *src = NULL;
    GstFlowReturn ret;

    src = GST_SME_HTTP_SRC (bsrc);

    g_mutex_lock (&src->mutex);
    ret = gst_sme_http_src_do_request (src, SOUP_METHOD_GET,offset, length, outbuf);

    g_mutex_unlock (&src->mutex);

    return ret;
}

/* BEGIN: Added for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */
static gboolean
gst_sme_http_src_get_data_from_prepared_buf (GstSmeHttpSrc * src,
        guint64 offset, guint length, GstBuffer ** outbuf)
{
    gboolean bCompleted = FALSE;
    guint64 buf_offset = 0;
    gsize size = 0;
    gsize read_offset = 0;
    gsize read_len = 0;
    guint64 src_size = 0;
    GstBaseSrc *basesrc = NULL;

    if (NULL != src->preread_buffer && NULL != outbuf){
        size = gst_buffer_get_size(src->preread_buffer);
        buf_offset = GST_BUFFER_OFFSET(src->preread_buffer);
        if((offset >= buf_offset) && (offset < buf_offset + size)){
            read_offset = (gsize)(offset - buf_offset);
            read_len = (gsize)(buf_offset + size - offset);
            read_len = read_len > length ? length : read_len;
            *outbuf = gst_buffer_copy_region (src->preread_buffer,
                GST_BUFFER_COPY_MEMORY|GST_BUFFER_COPY_DEEP,
                read_offset, read_len);
            if(NULL == *outbuf){
                GST_ERROR("no mem!");
                src->ret = GST_FLOW_CUSTOM_ERROR;
                bCompleted = TRUE;
            }else{
                GST_BUFFER_OFFSET(*outbuf) = offset;
                GST_DEBUG("preread_buffer offset=%lld, outbuf offset = %lld, length=%d",
                    GST_BUFFER_OFFSET(*outbuf), buf_offset, read_len);

                src->ret = GST_FLOW_OK;
                bCompleted = TRUE;
            }
        }else{
            gst_buffer_unref(src->preread_buffer);
            src->preread_buffer = NULL;
        }

        basesrc = GST_BASE_SRC_CAST (src);
        if (gst_sme_http_src_get_size(basesrc, &src_size)) {
            if (offset + read_len >= src_size) {
                src->is_download_complete = TRUE;
                gst_element_post_message (GST_ELEMENT (src),
                                          gst_message_new_new_bandwidth(GST_OBJECT (src),
                                                                        (guint)0));
            }
        }

    }

    return bCompleted;
}
/* END:   Added for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */

static GstFlowReturn
gst_sme_http_src_do_request (GstSmeHttpSrc * src, const gchar * method,
        guint64 offset, guint length, GstBuffer ** outbuf)
{
    gsize size = 0;
    guint64 buf_offset = 0;
    guint64 src_size = 0;
    GstBaseSrc *basesrc = NULL;

    GST_DEBUG ("gst_soup_http_src_do_request");
    GST_DEBUG_OBJECT (src, "Running request for method");
    GST_DEBUG_OBJECT (src, "offset:%llu,length:%d",offset, length);
    GST_DEBUG("src->msg:%p",src->msg);

    if(gst_sme_http_src_get_data_from_prepared_buf (src, offset, length, outbuf)){
        if(GST_FLOW_OK != src->ret && outbuf && NULL != *outbuf){
          gst_buffer_unref(*outbuf);
          *outbuf = NULL;
        }
        return src->ret;
     }

    src->request_position = offset;

    GST_DEBUG_OBJECT (src, "request_position:%llu,read_position:%llu",
        src->request_position,src->read_position);

    /* BEGIN: Modified for DTS2015122306953 2015/12/25
     * cancle message which cancled in gst_sme_http_src_unlock in order to
     * rebuild it in next step(L1377)*/
    if (src->msg &&
        (src->request_position != src->read_position || src->is_finished)) {
    /* END: Modified for DTS2015122306953 2015/12/25 */
        /* BEGIN: Modified for DTS2015102106448  by baihaili(b00311320), 2015/10/22 */
        src->is_download_complete = FALSE;
        /* END:   Modified for DTS2015102106448  by baihaili(b00311320), 2015/10/22 */
        if (src->session_io_status == GST_SME_HTTP_SRC_SESSION_IO_STATUS_IDLE) {
            gst_sme_http_src_add_range_header (src, src->request_position,
                src->stop_position);
        } else {
            GST_INFO_OBJECT (src, "Seek from position %" G_GUINT64_FORMAT
                " to %" G_GUINT64_FORMAT ": requeueing connection request",
            src->read_position, src->request_position);
            gst_sme_http_src_cancel_message (src);
        }
    }
    /* BEGIN: Modified for DTS2014102102466 by liurongliang(l00180035), 2014/10/27 */
    src->ret = GST_FLOW_CUSTOM_ERROR;
    src->outbuf = outbuf;
    src->start_req_time = get_cur_time() / 1000;
    do {
        if (!src->msg)
            if (!gst_sme_http_src_build_message (src, method)) {
                return GST_FLOW_ERROR;
            }
        if (src->interrupted) {
            GST_INFO_OBJECT (src, "interrupted");
            break;
        }
        if (src->retry) {
            GST_INFO_OBJECT (src, "Reconnecting");
            if (!gst_sme_http_src_build_message (src, method)) {
                return GST_FLOW_ERROR;
            }
            src->retry = FALSE;
            continue;
        }
        if (!src->msg) {
            GST_INFO_OBJECT (src, "EOS reached");
            break;
        }

        GST_DEBUG("session_io_status:%d",src->session_io_status);
        switch (src->session_io_status) {
            case GST_SME_HTTP_SRC_SESSION_IO_STATUS_IDLE:
              GST_INFO_OBJECT (src, "Queueing connection request");
              gst_sme_http_src_queue_message (src);
              break;
            case GST_SME_HTTP_SRC_SESSION_IO_STATUS_QUEUED:
              GST_INFO_OBJECT (src, "Queueing QUEUED request");
              break;
            case GST_SME_HTTP_SRC_SESSION_IO_STATUS_RUNNING:
              gst_sme_http_src_session_unpause_message (src);
              break;
            case GST_SME_HTTP_SRC_SESSION_IO_STATUS_CANCELLED:
              /* Impossible. */
              break;
        }
        GST_DEBUG_OBJECT (src, "src->ret = %d", src->ret);
        /* BEGIN: Modified for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */
        if (src->ret == GST_FLOW_CUSTOM_ERROR){
            if(NULL != outbuf){
                GST_DEBUG_OBJECT (src, "run loop begin");
                g_main_loop_run (src->loop);
                //g_usleep(2000);
                GST_DEBUG_OBJECT (src, "run loop end");
            }else{
                src->ret = GST_FLOW_OK;
                GST_INFO_OBJECT (src, "send geting.");
            }
        }
        /* END:   Modified for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */

        GST_DEBUG_OBJECT (src, "00 src->ret = %d", src->ret);
    } while (src->ret == GST_FLOW_CUSTOM_ERROR);
    /* END: Modified for DTS2014102102466 by liurongliang(l00180035), 2014/10/27 */
    src->start_req_time = G_MAXUINT64;

   /*if the return buffer data size > len ,add a gstbuffer to store */
    GST_DEBUG("src->outbuf:%p",src->outbuf);
    /* BEGIN: Modified for DTS2014091906325  by liurongliang(l00180035), 2014/9/27 */
    if(NULL != src->outbuf){
        if(!*src->outbuf){
            (void)gst_sme_http_src_get_data_from_prepared_buf (src, offset, length,
                src->outbuf);
        }else{
            size = gst_buffer_get_size(*src->outbuf);
            GST_DEBUG("size:%d,length:%u",size, length);

            if (size > length) {
                src->preread_buffer = gst_buffer_copy_region (*src->outbuf,
                                      GST_BUFFER_COPY_MEMORY|GST_BUFFER_COPY_DEEP,
                                      (gsize)length, size-length);
                if (NULL == src->preread_buffer) {
                   GST_ERROR("malloc http buffer failed!");
                   src->ret = GST_FLOW_CUSTOM_ERROR;
                }else{
                   buf_offset = GST_BUFFER_OFFSET(*src->outbuf);
                   GST_DEBUG("preread_buffer offset=%lld, outbuf offset = %lld, length=%d",
                       GST_BUFFER_OFFSET(src->preread_buffer), buf_offset, length);
                   gst_buffer_resize (*src->outbuf, 0, (gssize)length);
                   GST_BUFFER_OFFSET(src->preread_buffer) = buf_offset + length;
                }
            }
        }
        /*lint -save -esym(613,outbuf) */
        *outbuf = *src->outbuf;
        basesrc = GST_BASE_SRC_CAST (src);
        if (gst_sme_http_src_get_size(basesrc, &src_size)) {
            if (offset + length >= src_size) {
                src->is_download_complete = TRUE;
                gst_element_post_message (GST_ELEMENT (src),
                                          gst_message_new_new_bandwidth(GST_OBJECT (src),
                                                                        (guint)0));
            }
        }
        /*lint -restore */
    }

    if (src->ret == GST_FLOW_CUSTOM_ERROR)
      src->ret = GST_FLOW_EOS;
    g_cond_signal (&src->request_finished_cond);

    if(GST_FLOW_OK != src->ret && outbuf && NULL != *outbuf){
      gst_buffer_unref(*outbuf);
      *outbuf = NULL;
    }

    return src->ret;
}

static void
gst_sme_http_src_cancel_message (GstSmeHttpSrc * src)
{
    if (src->msg != NULL) {
        src->session_io_status = GST_SME_HTTP_SRC_SESSION_IO_STATUS_CANCELLED;
        GST_INFO("cancel_message session = %p, msg=%p", src->session, src->msg);
        soup_session_cancel_message (src->session, src->msg, SOUP_STATUS_CANCELLED);

        //we should release what we owned!
        gst_object_unref(src->msg);
        src->msg = NULL;
    }
    src->session_io_status = GST_SME_HTTP_SRC_SESSION_IO_STATUS_IDLE;
}

/* BEGIN: Modified for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */
static void
gst_sme_http_src_check_seekable (GstSmeHttpSrc * src)
{
    GstFlowReturn ret = GST_FLOW_OK;

    if (!src->got_headers && !src->is_getting_headers) {
        g_mutex_lock (&src->mutex);
        while (!src->got_headers && !src->interrupted && !src->is_getting_headers && ret == GST_FLOW_OK) {
            if ((src->msg && src->msg->method != SOUP_METHOD_HEAD) &&
                src->session_io_status != GST_SME_HTTP_SRC_SESSION_IO_STATUS_IDLE) {
              /* wait for the current request to finish */
                g_cond_wait (&src->request_finished_cond, &src->mutex);
            } else {
                if (gst_sme_http_src_session_open (src)) {
                    GST_INFO("gst_sme_http_src_check_seekable begin request...");
                    ret = gst_sme_http_src_do_request (src, SOUP_METHOD_GET, (guint64)0, (guint)-1, NULL);
                    if(GST_FLOW_OK == ret){
                        //all http cases now are seekable.
                        src->seekable = TRUE;
                        src->is_getting_headers = TRUE;
                    }
                    GST_INFO("gst_sme_http_src_check_seekable end request");
                }
            }
        }

        if (src->ret == GST_FLOW_EOS) {
          /* A HEAD request shouldn't lead to EOS */
          src->ret = GST_FLOW_OK;
        }
        /* resets status to idle */
        //gst_sme_http_src_cancel_message (src);
        g_mutex_unlock (&src->mutex);
  }

}
/* END:   Modified for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */

static gboolean
gst_sme_http_src_is_seekable (GstBaseSrc * bsrc)
{
    GstSmeHttpSrc *src = GST_SME_HTTP_SRC (bsrc);
    GST_INFO("src->seekable:%d",src->seekable);
    if (src->seekable) {
        return TRUE;
    }
    gint64 start_time = g_get_real_time();
    gst_sme_http_src_check_seekable (src);
    gint64 end_time = g_get_real_time() - start_time;

    GST_INFO ("seek time:%lld,src->seekable:%d", end_time,src->seekable);
    return src->seekable;
}

static void
gst_sme_http_src_authenticate_cb (SoupSession * session, const SoupMessage * msg,
    SoupAuth * auth, gboolean retrying, const GstSmeHttpSrc * src)
{
    (void)session;
    if (!retrying) {
        /* First time authentication only, if we fail and are called again with retry true fall through */
        if (msg->status_code == SOUP_STATUS_UNAUTHORIZED) {
            if (src->user_id && src->user_pw)
                soup_auth_authenticate (auth, src->user_id, src->user_pw);
        } else if (msg->status_code == SOUP_STATUS_PROXY_AUTHENTICATION_REQUIRED) {
            if (src->proxy_id && src->proxy_pw)
                soup_auth_authenticate (auth, src->proxy_id, src->proxy_pw);
        }
    }
}

static gboolean
gst_sme_http_src_session_open (GstSmeHttpSrc * src)
{
    if (src->session) {
        GST_INFO_OBJECT (src, "Session is already open");
        return TRUE;
    }

    if (!src->location) {
        GST_ELEMENT_ERROR (src, RESOURCE, OPEN_READ, ("No URL set."),
            ("Missing location property"));
        return FALSE;
    }

    src->context = g_main_context_new ();
    GST_INFO_OBJECT (src, "g_main_context_new,context=%p", src->context);

    src->loop = g_main_loop_new (src->context, (gboolean)FALSE);
    if (!src->loop) {
        GST_ELEMENT_ERROR (src, LIBRARY, INIT,
            (NULL), ("Failed to start GMainLoop"));

        GST_INFO_OBJECT (src, "1 g_main_context_unref,context=%p", src->context);
        g_main_context_unref (src->context);
        src->context = NULL;
        return FALSE;
    }
    src->is_getting_headers = FALSE;
    src->is_first_chunck = TRUE;
    GST_INFO_OBJECT (src, "Creating session");
#if 1
    if (src->proxy == NULL) {
        src->session =
            soup_session_async_new_with_options (SOUP_SESSION_ASYNC_CONTEXT,
            src->context, SOUP_SESSION_USER_AGENT, src->user_agent,
            SOUP_SESSION_TIMEOUT, src->timeout,
            SOUP_SESSION_ADD_FEATURE_BY_TYPE, SOUP_TYPE_PROXY_RESOLVER_DEFAULT,
            NULL);
        GST_INFO("no proxy src->session = %p", src->session);
    } else {
        src->session =
            soup_session_async_new_with_options (SOUP_SESSION_ASYNC_CONTEXT,
            src->context, SOUP_SESSION_PROXY_URI, src->proxy,
            SOUP_SESSION_TIMEOUT, src->timeout,
            SOUP_SESSION_USER_AGENT, src->user_agent, NULL);
        GST_INFO("proxy src->session = %p", src->session);

    }
#else
    GST_INFO ("use new interface creat session");
    src->session = g_object_new (SOUP_TYPE_SESSION,NULL);
#endif
    if (!src->session) {
        GST_ELEMENT_ERROR (src, LIBRARY, INIT,
            (NULL), ("Failed to create async session"));
        return FALSE;
    }

    g_signal_connect (src->session, "authenticate",
        G_CALLBACK (gst_sme_http_src_authenticate_cb), src);
    return TRUE;
}

static gboolean
gst_sme_http_src_get_size (GstBaseSrc * bsrc, guint64 * size)
{
    GstSmeHttpSrc *src = NULL;

    src = GST_SME_HTTP_SRC (bsrc);

    if (src->have_size) {
        GST_DEBUG_OBJECT (src, "get_size() = %" G_GUINT64_FORMAT,
            src->content_size);
        *size = src->content_size;
        return TRUE;
    }
    GST_INFO_OBJECT (src, "get_size() = FALSE");
    return FALSE;
}

static gboolean
gst_sme_http_src_start (GstBaseSrc * bsrc)
{
    GstSmeHttpSrc *src = GST_SME_HTTP_SRC (bsrc);

    //GST_INFO_OBJECT (src, "start(\"%s\")", src->location);
    /* BEGIN: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
    gst_task_start(src->bitrate_watch_task);
    src->bandwidth_entry.get_data_timestamp = g_get_monotonic_time();
    /* END: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
    return gst_sme_http_src_session_open (src);
}

static void
gst_sme_http_src_session_close (GstSmeHttpSrc * src)
{
  if (src->session) {
    GST_INFO("session_close session = %p, msg=%p", src->session, src->msg);
    soup_session_abort (src->session);  /* This unrefs the message. */

    GST_INFO("-session_close session = %p, msg=%p", src->session, src->msg);
    g_object_unref (src->session);

    GST_INFO("--session_close session = %p, msg=%p", src->session, src->msg);
    src->session = NULL;

    if(src->cur_item){
        soup_message_queue_item_unref(src->cur_item);
        src->cur_item = NULL;
    }

    if(src->msg){
        gst_object_unref(src->msg);
        src->msg = NULL;
    }

  }
}

static gboolean
gst_sme_http_src_stop (GstBaseSrc * bsrc)
{
    GstSmeHttpSrc *src = NULL;

    src = GST_SME_HTTP_SRC (bsrc);
    if (!src->is_query_schedule) {
        GST_INFO_OBJECT (src, "stop(), src->msg=%p, src->cur_item=%p", src->msg, src->cur_item);

        gst_sme_http_src_session_close (src);
        if (src->loop) {
            g_main_loop_unref (src->loop);
            GST_INFO_OBJECT (src, "g_main_context_unref,context=%p", src->context);
            g_main_context_unref (src->context);
            src->loop = NULL;
            src->context = NULL;
        }

        if (src->extra_headers) {
            gst_structure_free (src->extra_headers);
            src->extra_headers = NULL;
        }

        gst_sme_http_src_reset (src);
        /* BEGIN: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
        if (GST_TASK_STATE (src->bitrate_watch_task) != GST_TASK_STOPPED) {
            gst_task_stop (src->bitrate_watch_task);
            g_rec_mutex_lock (&src->bitrate_watch_task_lock);
            g_rec_mutex_unlock (&src->bitrate_watch_task_lock);
            gst_task_join (src->bitrate_watch_task);
        /* END: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
        }
    }
    return TRUE;
}

static gboolean
gst_sme_http_src_unlock (GstBaseSrc * bsrc)
{
    GstSmeHttpSrc *src;

    src = GST_SME_HTTP_SRC (bsrc);
    GST_INFO_OBJECT (src, "unlock()");

    src->interrupted = TRUE;
    if (src->loop){
        g_mutex_lock (&src->msg_mutex);
        if(!src->is_finished && g_main_loop_is_running(src->loop)){
            if(src->msg && src->cur_item && src->cur_item->cancellable){
                GST_INFO("src->cur_item->cancellable begin...");
                g_cancellable_cancel (src->cur_item->cancellable);
                GST_INFO("src->cur_item->cancellable end");
            }
            GST_INFO("g_cond_wait_until begin...");
            g_cond_wait_until(&src->request_finished_cond,
                &src->msg_mutex, g_get_monotonic_time() + CANCEL_SOUP_WAIT_TIMEOUT);
            GST_INFO("g_cond_wait_until end");
        }
        g_mutex_unlock (&src->msg_mutex);
        g_main_loop_quit (src->loop);
    }
    g_cond_signal (&src->request_finished_cond);

    return TRUE;
}

/* Interrupt interrupt. */
static gboolean
gst_sme_http_src_unlock_stop (GstBaseSrc * bsrc)
{
    GstSmeHttpSrc *src;

    src = GST_SME_HTTP_SRC (bsrc);
    GST_DEBUG_OBJECT (src, "unlock_stop()");

    src->interrupted = FALSE;
    return TRUE;
}

static gboolean
gst_sme_http_src_do_seek (GstBaseSrc * bsrc, GstSegment * segment)
{
    GstSmeHttpSrc *src = GST_SME_HTTP_SRC (bsrc);

    GST_DEBUG_OBJECT (src, "do_seek(%" G_GUINT64_FORMAT "-%" G_GUINT64_FORMAT
        ")", segment->start, segment->stop);
    if (src->read_position == segment->start &&
        src->request_position == src->read_position &&
        src->stop_position == segment->stop) {
        GST_DEBUG_OBJECT (src,
            "Seek to current read/end position and no seek pending");
        return TRUE;
    }

    gst_sme_http_src_check_seekable (src);

    /* If we have no headers we don't know yet if it is seekable or not.
     * Store the start position and error out later if it isn't */
    if (src->got_headers && !src->seekable) {
        GST_WARNING_OBJECT (src, "Not seekable");
        return FALSE;
    }

    if (segment->rate < 0.0 || segment->format != GST_FORMAT_BYTES) {
        GST_WARNING_OBJECT (src, "Invalid seek segment");
        return FALSE;
    }

    if (src->content_size != 0 && segment->start >= src->content_size) {
        GST_WARNING_OBJECT (src,
            "Potentially seeking behind end of file, might EOS immediately");
    }

    /* Wait for create() to handle the jump in offset. */
    src->request_position = segment->start;
    src->stop_position = segment->stop;

    return TRUE;
}

static gboolean
gst_sme_http_src_query (GstBaseSrc * bsrc, GstQuery * query)
{
    GstSmeHttpSrc *src = GST_SME_HTTP_SRC (bsrc);
    gboolean ret;
    GstSchedulingFlags flags;
    gint minsize, maxsize, align;

    switch (GST_QUERY_TYPE (query)) {
        case GST_QUERY_URI:
            gst_query_set_uri (query, src->location);
            if (src->redirection_uri != NULL)
              gst_query_set_uri_redirection (query, src->redirection_uri);
            ret = TRUE;
            break;
        default:
            ret = FALSE;
            break;
    }

    if (!ret){
        if(GST_QUERY_SCHEDULING == GST_QUERY_TYPE (query)){
            src->is_query_schedule = TRUE;
        }

        ret = GST_BASE_SRC_CLASS (parent_class)->query (bsrc, query);

        if(GST_QUERY_SCHEDULING == GST_QUERY_TYPE (query)){
            src->is_query_schedule = FALSE;
        }
    }

    switch (GST_QUERY_TYPE (query)) {
      case GST_QUERY_SCHEDULING:
          gst_query_parse_scheduling (query, &flags, &minsize, &maxsize, &align);
          flags |= GST_SCHEDULING_FLAG_BANDWIDTH_LIMITED;
          gst_query_set_scheduling (query, flags, minsize, maxsize, align);
          break;
      default:
          break;
    }

    return ret;
}

static void
gst_sme_http_src_finalize (GObject * gobject)
{
    GstSmeHttpSrc *src = GST_SME_HTTP_SRC (gobject);

    GST_DEBUG_OBJECT (src, "finalize");

    /** BEGIN: Modified for DTS2015042206034  by baihaili(b00311320), 2015/5/29 */
    if (src->preread_buffer) {
        gst_buffer_unref(src->preread_buffer);
        src->preread_buffer = NULL;
    }
    /** END:   Modified for DTS2015042206034 by baihaili(b00311320), 2015/5/29 */

    if(src->msg){
        gst_object_unref(src->msg);
        src->msg = NULL;
    }
    if(src->cur_item){
        soup_message_queue_item_unref(src->cur_item);
        src->cur_item = NULL;
    }

    g_mutex_clear (&src->msg_mutex);
    g_mutex_clear (&src->mutex);
    g_cond_clear (&src->request_finished_cond);
    g_free (src->location);
    if (src->redirection_uri) {
        g_free (src->redirection_uri);
    }
    g_free (src->user_agent);
    src->user_agent = NULL;
    if (src->proxy != NULL) {
        soup_uri_free (src->proxy);
    }
    g_free (src->user_id);
    g_free (src->user_pw);
    g_free (src->proxy_id);
    g_free (src->proxy_pw);
    g_strfreev (src->cookies);
    src->cookies = NULL;
    g_free (src->referer);
    src->referer = NULL;
    /* BEGIN: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
    if (src->bitrate_watch_task) {
        gst_object_unref (src->bitrate_watch_task);
        g_rec_mutex_clear (&src->bitrate_watch_task_lock);
        src->bitrate_watch_task = NULL;
    }
    /* END: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
    G_OBJECT_CLASS (parent_class)->finalize (gobject);
}

static void
gst_sme_http_src_class_init (GstSmeHttpSrcClass * klass)
{
    GObjectClass *gobject_class = NULL;
    GstElementClass *gstelement_class = NULL;
    GstBaseSrcClass *gstbasesrc_class = NULL;

    gobject_class = (GObjectClass *) klass;
    gstelement_class = (GstElementClass *) klass;
    gstbasesrc_class = (GstBaseSrcClass *) klass;

    gobject_class->set_property = gst_sme_http_src_set_property;
    gobject_class->get_property = gst_sme_http_src_get_property;
    gobject_class->finalize = gst_sme_http_src_finalize;

    g_object_class_install_property (gobject_class,
        PROP_LOCATION,
        g_param_spec_string ("location", "Location",
            "Location to read from", "",
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class,
        PROP_USER_AGENT,
        g_param_spec_string ("user-agent", "User-Agent",
            "Value of the User-Agent HTTP request header field",
            NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class, PROP_BYTE_RANGE,
        g_param_spec_boolean ("byte-range", "Byte-Range",
          "Value of the Byte-Range HTTP request header field", (gboolean)TRUE,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class,
        PROP_REFERER,
        g_param_spec_string ("referer", "Referer",
            "Value of the Referer HTTP request header field",
            NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class,
        PROP_AUTOMATIC_REDIRECT,
        g_param_spec_boolean ("automatic-redirect", "automatic-redirect",
            "Automatically follow HTTP redirects (HTTP Status Code 3xx)",
            (gboolean)TRUE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class,
        PROP_PROXY,
        g_param_spec_string ("proxy", "Proxy",
            "HTTP proxy server URI", "",
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class,
        PROP_USER_ID,
        g_param_spec_string ("user-id", "user-id",
            "HTTP location URI user id for authentication", "",
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class, PROP_USER_PW,
        g_param_spec_string ("user-pw", "user-pw",
            "HTTP location URI user password for authentication", "",
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class, PROP_PROXY_ID,
        g_param_spec_string ("proxy-id", "proxy-id",
            "HTTP proxy URI user id for authentication", "",
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class, PROP_PROXY_PW,
        g_param_spec_string ("proxy-pw", "proxy-pw",
            "HTTP proxy URI user password for authentication", "",
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class, PROP_COOKIES,
        g_param_spec_boxed ("cookies", "Cookies", "HTTP request cookies",
            G_TYPE_STRV, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class, PROP_IS_LIVE,
        g_param_spec_boolean ("is-live", "is-live", "Act like a live source",
            FALSE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class, PROP_TIMEOUT,
        g_param_spec_uint ("timeout", "timeout",
            "Value in seconds to timeout a blocking I/O (0 = No timeout).", 0,
            3600, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class, PROP_BANDWIDTH,
        g_param_spec_uint (GST_SME_PROP_KEY_CUR_BW, GST_SME_PROP_KEY_CUR_BW,
            "http download bandwidth.", 0,
            G_MAXUINT, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class, PROP_EXTRA_HEADERS,
        g_param_spec_boxed ("extra-headers", "Extra Headers",
            "Extra headers to append to the HTTP request",
            GST_TYPE_STRUCTURE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class, PROP_IRADIO_MODE,
        g_param_spec_boolean ("iradio-mode", "iradio-mode",
            "Enable internet radio mode (ask server to send shoutcast/icecast "
            "metadata interleaved with the actual stream data)",
            (gboolean)DEFAULT_IRADIO_MODE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    gst_element_class_add_pad_template (gstelement_class,
        gst_static_pad_template_get (&srctemplate));

    gst_element_class_set_static_metadata (gstelement_class, "HTTP client source",
        "Source/Network",
        "Receive data as a client over the network via HTTP using SOUP",
        "httpsrc element");

    gstbasesrc_class->start = GST_DEBUG_FUNCPTR (gst_sme_http_src_start);
    gstbasesrc_class->stop = GST_DEBUG_FUNCPTR (gst_sme_http_src_stop);
    gstbasesrc_class->unlock = GST_DEBUG_FUNCPTR (gst_sme_http_src_unlock);
    gstbasesrc_class->unlock_stop =
        GST_DEBUG_FUNCPTR (gst_sme_http_src_unlock_stop);
    gstbasesrc_class->get_size = GST_DEBUG_FUNCPTR (gst_sme_http_src_get_size);
    gstbasesrc_class->is_seekable =
        GST_DEBUG_FUNCPTR (gst_sme_http_src_is_seekable);
    gstbasesrc_class->create = GST_DEBUG_FUNCPTR (gst_sme_http_src_create);
    gstbasesrc_class->do_seek = GST_DEBUG_FUNCPTR (gst_sme_http_src_do_seek);
    gstbasesrc_class->query = GST_DEBUG_FUNCPTR (gst_sme_http_src_query);
}

static gboolean
gst_sme_http_src_set_location (GstSmeHttpSrc * src, const gchar * uri,
    GError ** error)
{
    const char *alt_schemes[] = { "icy://", "icyx://" };
    guint i;

    (void)error;
    if (src->location) {
        g_free (src->location);
        src->location = NULL;
    }

    if (uri == NULL)
        return FALSE;

    for (i = 0; i < G_N_ELEMENTS (alt_schemes); i++) {
        if (g_str_has_prefix (uri, alt_schemes[i])) {
            src->location =
                g_strdup_printf ("http://%s", uri + strlen (alt_schemes[i]));
            return TRUE;
        }
    }

    if (src->redirection_uri) {
        g_free (src->redirection_uri);
        src->redirection_uri = NULL;
    }

    src->location = g_strdup (uri);

    return TRUE;
}

static GstURIType
gst_sme_http_src_uri_get_type (GType type)
{
    (void)type;
    return GST_URI_SRC;
}

static const gchar *const *
gst_sme_http_src_uri_get_protocols (GType type)
{
    (void)type;
    static const gchar *protocols[] = { "http", "icy", "icyx", NULL };

    return protocols;
}

static gchar *
gst_sme_http_src_uri_get_uri (GstURIHandler * handler)
{
    GstSmeHttpSrc *src = GST_SME_HTTP_SRC (handler);

    /* FIXME: make thread-safe */
    return g_strdup (src->location);
}

static gboolean
gst_sme_http_src_uri_set_uri (GstURIHandler * handler, const gchar * uri,
    GError ** error)
{
    GstSmeHttpSrc *src = GST_SME_HTTP_SRC (handler);

    return gst_sme_http_src_set_location (src, uri, error);
}

static void
gst_sme_http_src_uri_handler_init (gpointer g_iface, const gpointer iface_data)
{
    (void)iface_data;
    GstURIHandlerInterface *iface = (GstURIHandlerInterface *) g_iface;

    iface->get_type = gst_sme_http_src_uri_get_type;
    iface->get_protocols = gst_sme_http_src_uri_get_protocols;
    iface->get_uri = gst_sme_http_src_uri_get_uri;
    iface->set_uri = gst_sme_http_src_uri_set_uri;
}

gboolean
gst_smehttpsrc_register (GstPlugin * plugin)
{
    GST_INFO_OBJECT(plugin, "registering sme http src in.");

    if (!gst_element_register (plugin, GST_SMEHTTPSRC_NAME,
                            GST_RANK_PRIMARY, GST_TYPE_SME_HTTP_SRC)) {
        GST_WARNING ("Register of type %s failed", GST_SMEHTTPSRC_NAME);
        return FALSE;
    }

    GST_INFO_OBJECT(plugin, "registering sme http src out.");

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H

#include "config.h"

//for dynamic load smehttpsrc plugin
#ifdef GST_PLUGIN_BUILD_STATIC
#undef GST_PLUGIN_BUILD_STATIC
#endif

#endif

#include "gstsme.h"

static gboolean
plugin_init (GstPlugin * plugin)
{
    GST_INFO("register smehttpsrc begin...");
    //register httpsrc element
    if (!gst_smehttpsrc_register (plugin))
        GST_WARNING("register smehttpsrc fail.");

    GST_INFO("register smehttpsrc end.");

    return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    smehttpsrc,
    "sme httpsrc plugin of mediaos. (info about sme: http://www.huawei.com)",
    plugin_init, VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
