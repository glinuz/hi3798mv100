/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmehttpsrc.h
 * @brief   httpsrc 头文件定义
 * @author  zhangzhen(z00175754)
 * @date    2014/6/4
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/5/14
 * Author : zhangzhen(z00175754)
 * Desc   : Created file
 *
******************************************************************************/



#ifndef __GST_SME_HTTP_SRC_H__
#define __GST_SME_HTTP_SRC_H__

#include <gst/gst.h>
#include <glib.h>
#include <gst/base/gstbasesrc.h>
G_BEGIN_DECLS

#include "gstsme.h"
#include "soup-session-private.h"

#define GST_TYPE_SME_HTTP_SRC \
  (gst_sme_http_src_get_type())
#define GST_SME_HTTP_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_SME_HTTP_SRC,GstSmeHttpSrc))
#define GST_SME_HTTP_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), \
      GST_TYPE_SME_HTTP_SRC,GstSmeHttpSrcClass))
#define GST_IS_SME_HTTP_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SME_HTTP_SRC))
#define GST_IS_SME_HTTP_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SME_HTTP_SRC))

typedef struct _GstSmeHttpSrc GstSmeHttpSrc;
typedef struct _GstSmeHttpSrcClass GstSmeHttpSrcClass;

typedef enum {
    GST_SME_HTTP_SRC_SESSION_IO_STATUS_IDLE,
    GST_SME_HTTP_SRC_SESSION_IO_STATUS_QUEUED,
    GST_SME_HTTP_SRC_SESSION_IO_STATUS_RUNNING,
    GST_SME_HTTP_SRC_SESSION_IO_STATUS_CANCELLED,
} GstSmeHttpSrcSessionIOStatus;

typedef struct
{
   gint64  start;
   gint64  end;
   gint64  collect_freq;
   guint32 transfer_bytes;
   gint32  bandwidth;
   /* BEGIN: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
   gint64 get_data_timestamp;
   /* END: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
} GstSmeBandWidthEntry;

enum
{
    PROP_0,
    PROP_LOCATION,
    PROP_IS_LIVE,
    PROP_USER_AGENT,
    PROP_AUTOMATIC_REDIRECT,
    PROP_PROXY,
    PROP_USER_ID,
    PROP_USER_PW,
    PROP_PROXY_ID,
    PROP_PROXY_PW,
    PROP_COOKIES,
    PROP_IRADIO_MODE,
    PROP_TIMEOUT,
    PROP_BANDWIDTH,
    PROP_EXTRA_HEADERS,
    PROP_BYTE_RANGE,
    PROP_REFERER
};

struct _GstSmeHttpSrc {
    GstBaseSrc element;

    gchar *location;             /* Full URI. */
    gchar *redirection_uri;      /* Full URI after redirections. */
    gchar *user_agent;           /* User-Agent HTTP header. */
    gboolean byte_range;
    gchar * referer;
    gboolean automatic_redirect; /* Follow redirects. */
    SoupURI *proxy;              /* HTTP proxy URI. */
    gchar *user_id;              /* Authentication user id for location URI. */
    gchar *user_pw;
    gchar *proxy_id;             /* Authentication user id for proxy URI. */
    gchar *proxy_pw;
    gchar **cookies;             /* HTTP request cookies. */
    GMainContext *context;       /* I/O context. */
    GMainLoop *loop;             /* Event loop. */
    SoupSession *session;        /* Async context. */
    GstSmeHttpSrcSessionIOStatus session_io_status;
                               /* Async I/O status. */
    SoupMessage *msg;            /* Request message. */
    GstFlowReturn ret;           /* Return code from callback. */
    GstBuffer **outbuf;          /* Return buffer allocated by callback. */
    GstBuffer *preread_buffer;      /*store the extra data*/
    gboolean interrupted;        /* Signal unlock(). */
    gboolean retry;              /* Should attempt to reconnect. */

    gboolean got_headers;        /* Already received headers from the server */
    gboolean have_size;          /* Received and parsed Content-Length
                                  header. */
    guint64 content_size;        /* Value of Content-Length header. */
    guint64 read_position;       /* Current position. */
    gboolean seekable;           /* FALSE if the server does not support
                                  Range. */
    guint64 request_position;    /* Seek to this position. */
    guint64 stop_position;       /* Stop at this position. */
    gboolean have_body;          /* Indicates if it has just been signaled the
                                * end of the message body. This is used to
                                * decide if an out of range request should be
                                * handled as an error or EOS when the content
                                * size is unknown */

    /* Shoutcast/icecast metadata extraction handling. */
    gboolean iradio_mode;
    GstCaps *src_caps;
    gchar *iradio_name;
    gchar *iradio_genre;
    gchar *iradio_url;

    GstStructure *extra_headers;

    guint timeout;

    GMutex mutex;
    GCond request_finished_cond;
    gboolean is_query_schedule;
    GstSmeBandWidthEntry bandwidth_entry;
    /* BEGIN: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
    GstTask *bitrate_watch_task;
    GRecMutex bitrate_watch_task_lock;
    gboolean is_download_complete;
    /* END: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
    gboolean is_getting_headers;
    /* BEGIN: Added for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */
    gboolean is_first_chunck;
    /* END:   Added for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */
    guint64 start_req_time;

    /* BEGIN: Modified for DTS2015100804623 by wanghua(w00347020), 2015/10/9 */
    guint last_status_code;
    /* END: Modified for DTS2015100804623 by wanghua(w00347020), 2015/10/9 */
    GMutex msg_mutex;
    SoupMessageQueueItem* cur_item;
    gboolean is_finished;
};

struct _GstSmeHttpSrcClass {
    GstBaseSrcClass parent_class;
};

GType gst_sme_http_src_get_type (void);

G_END_DECLS

#endif /* __GST_SME_HTTP_SRC_H__ */
