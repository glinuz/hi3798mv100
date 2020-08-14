/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmeffprosrc.c
 * @brief   fake src element for protocol (http/hls...) in ffmpeg
 * @author
 * @date    2016/07/22th
 * @version
 * History:
 *
 * Number : V1.00
 * Date   : 2016/07/22th
 * Author :
 * Desc   : Created file
 *
******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <gst/gstelement.h>
#include "gstsmeffprosrc.h"

#define GST_SME_FFPRO_SRC_NAME "gstsmeffprosrc"

static GstStaticPadTemplate srctemplate =
    GST_STATIC_PAD_TEMPLATE ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
    GST_STATIC_CAPS("media/ff_pro"));

static void gst_sme_ffpro_src_uri_handler_init (gpointer g_iface, const gpointer iface_data);

static void
gst_sme_ffpro_src_init (GstSmeFfProSrc * src);

static void
gst_sme_ffpro_src_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);

static void
gst_sme_ffpro_src_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);


static void
gst_sme_ffpro_src_class_init (GstSmeFfProSrcClass * klass);

static gboolean
gst_sme_ffpro_src_set_location (GstSmeFfProSrc * src, const gchar * uri,
    GError ** error);

static const gchar *const *
gst_sme_ffpro_src_uri_get_protocols (GType type);

static gchar *
gst_sme_ffpro_src_uri_get_uri (GstURIHandler * handler);


static gboolean
gst_sme_ffpro_src_uri_set_uri (GstURIHandler * handler, const gchar * uri,
    GError ** error);


#define gst_sme_ffpro_src_parent_class parent_class
G_DEFINE_TYPE_WITH_CODE (GstSmeFfProSrc, gst_sme_ffpro_src, GST_TYPE_BASE_SRC,
    G_IMPLEMENT_INTERFACE (GST_TYPE_URI_HANDLER,
        gst_sme_ffpro_src_uri_handler_init));


static void
gst_sme_ffpro_src_init (GstSmeFfProSrc * src)
{
    GST_INFO("src_init");
}

static void
gst_sme_ffpro_src_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
    GstSmeFfProSrc *src = GST_SME_FF_PRO_SRC (object);

    switch (prop_id) {
        case PROP_LOCATION:
        {
            const gchar *location;

            location = g_value_get_string (value);

            if (location == NULL) {
                GST_WARNING ("location property cannot be NULL");
                goto done;
            }
            if (!gst_sme_ffpro_src_set_location (src, location, NULL)) {
                GST_WARNING ("badly formatted location");
                goto done;
            }
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
gst_sme_ffpro_src_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
    GstSmeFfProSrc *src = GST_SME_FF_PRO_SRC (object);

    switch (prop_id) {
        case PROP_LOCATION:
            g_value_set_string (value, src->location);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

static void
gst_sme_ffpro_src_finalize (GObject * gobject)
{
    GstSmeFfProSrc *src = GST_SME_FF_PRO_SRC (gobject);

    GST_INFO_OBJECT (src, "finalize");

    G_OBJECT_CLASS (parent_class)->finalize (gobject);
}



static GstFlowReturn
gst_sme_ffpro_src_create (GstBaseSrc * bsrc, guint64 offset,
        guint length, GstBuffer ** outbuf)
{
    GstSmeFfProSrc *src = GST_SME_FF_PRO_SRC (bsrc);

    usleep(1000000);

    *outbuf = gst_buffer_new_and_alloc(length);
    if(NULL != (*outbuf)->pool){
        gst_object_unref((*outbuf)->pool);
        (*outbuf)->pool = NULL;
    }

    GST_INFO_OBJECT(src, "offset=%llu, length=%u, outbuf=%p",
        offset, length, outbuf);

    return GST_FLOW_OK;
}

static gboolean
gst_sme_ffpro_src_set_location (GstSmeFfProSrc * src, const gchar * uri,
    GError ** error)
{
    (void)error;

    if (src->location) {
        g_free (src->location);
        src->location = NULL;
    }

    if (uri == NULL)
        return FALSE;

    src->location = g_strdup (uri);

    return TRUE;
}

static GstURIType
gst_sme_ffpro_src_uri_get_type (GType type)
{
    (void)type;
    return GST_URI_SRC;
}

static const gchar *const *
gst_sme_ffpro_src_uri_get_protocols (GType type)
{
    (void)type;
    static const gchar *protocols[] = { "http", "rtsp", "rtp", "udp", "https", NULL };

    GST_INFO("get_protocols supported in ffmpeg");

    return protocols;
}

static gchar *
gst_sme_ffpro_src_uri_get_uri (GstURIHandler * handler)
{
    GstSmeFfProSrc *src = GST_SME_FF_PRO_SRC (handler);

    /* FIXME: make thread-safe */
    return g_strdup (src->location);
}

static gboolean
gst_sme_ffpro_src_uri_set_uri (GstURIHandler * handler, const gchar * uri,
    GError ** error)
{
    GstSmeFfProSrc *src = GST_SME_FF_PRO_SRC (handler);

    GST_INFO("set_uri in");

    return gst_sme_ffpro_src_set_location (src, uri, error);
}

static void
gst_sme_ffpro_src_uri_handler_init (gpointer g_iface, const gpointer iface_data)
{
    (void)iface_data;
    GstURIHandlerInterface *iface = (GstURIHandlerInterface *) g_iface;

    iface->get_type = gst_sme_ffpro_src_uri_get_type;
    iface->get_protocols = gst_sme_ffpro_src_uri_get_protocols;
    iface->get_uri = gst_sme_ffpro_src_uri_get_uri;
    iface->set_uri = gst_sme_ffpro_src_uri_set_uri;
}

gboolean
gst_smeffprosrc_register (GstPlugin * plugin)
{
    GST_INFO_OBJECT(plugin, "registering sme ffpro src in.");

    if (!gst_element_register (plugin, GST_SME_FFPRO_SRC_NAME,
                            GST_RANK_PRIMARY - 1, GST_TYPE_SME_FF_PRO_SRC)) {
        GST_WARNING ("Register of type %s failed", GST_SME_FFPRO_SRC_NAME);
        return FALSE;
    }

    GST_INFO_OBJECT(plugin, "element: %s register ok", GST_SME_FFPRO_SRC_NAME);

    return TRUE;
}

/*
static gboolean
gst_sme_ffpro_src_is_seekable (GstBaseSrc * bsrc)
{
    GST_INFO("is_seekable = TRUE");
    return TRUE;
}
*/

static gboolean
gst_sme_ffpro_src_query (GstBaseSrc * bsrc, GstQuery * query)
{
    gboolean ret;
    GstSchedulingFlags flags;
    gint minsize, maxsize, align;

    ret = GST_BASE_SRC_CLASS (parent_class)->query (bsrc, query);

    switch (GST_QUERY_TYPE (query)) {
      case GST_QUERY_SCHEDULING:
          gst_query_parse_scheduling (query, &flags, &minsize, &maxsize, &align);

          /* 1. in gen_source_element() //gsturidecodebin.c
           *    it will check GST_SCHEDULING_FLAG_BANDWIDTH_LIMITED
           *    decoder->is_stream = flags & GST_SCHEDULING_FLAG_BANDWIDTH_LIMITED;
           * 2. in make_decoder() //gsturidecodebin.c
           *    if decoder->is_stream is FALSE, then, decoder->use_buffering = FALSE;
           *    g_object_set (decodebin, "use-buffering", decoder->use_buffering, NULL);
           * 3. in gst_decode_group_new()  //gstdecodebin2.c
           *    it will set property "use-buffering" to multiqueue.
           * so, here will cause whether multiqueue is use-buffering.
           *
           * in another way:
           * 1. in SmePlayerImplGst::IsCacheCtrlEnable  //sme_player_impl_gst.cpp
           *    for http/rtsp, m_bCacheCtrlEnable is enabled,
           *    so, "use-buffering" is enabled in HandlePrepareAsync()  //sme_player_impl_gst.cpp
           * 2. OnNotifyComing() //sme_player_impl_gst.cpp
           *    case SmePlayerMsgBase::E_TYPE_ON_PREPARED:
           *       if m_bBuffering is true, this notify is ignore.
           *
           * so, for http/rtsp playing, this flag must be set;
           *     otherwise, PREPARE_DONE("E_SME_PLAYER_NOTIFY_PREPARED") notify will not be sent to player,
           *     and, it will cause "Prepare" action is not completed.
           */
          flags |= GST_SCHEDULING_FLAG_BANDWIDTH_LIMITED;

          gst_query_set_scheduling (query, flags, minsize, maxsize, align);
          ret = TRUE;
          break;
      default:
          break;
    }

    return ret;
}

static void
gst_sme_ffpro_src_class_init (GstSmeFfProSrcClass * klass)
{
    GObjectClass *gobject_class = NULL;
    GstElementClass *gstelement_class = NULL;
    GstBaseSrcClass *gstbasesrc_class = NULL;

    gobject_class = (GObjectClass *) klass;
    gstelement_class = (GstElementClass *) klass;
    gstbasesrc_class = (GstBaseSrcClass *) klass;

    GST_INFO("class_init");

    gobject_class->set_property = gst_sme_ffpro_src_set_property;
    gobject_class->get_property = gst_sme_ffpro_src_get_property;
    gobject_class->finalize = gst_sme_ffpro_src_finalize;

    g_object_class_install_property (gobject_class,
        PROP_LOCATION,
        g_param_spec_string ("location", "Location",
            "Location to read from", "",
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));


    gst_element_class_add_pad_template (gstelement_class,
        gst_static_pad_template_get (&srctemplate));

    gst_element_class_set_static_metadata (gstelement_class, "ffmpeg_protocol_src",
        "Source/Network",
        "fake source element for network protocol(http/hls/rtsp/rtp, etc) using FFmpeg",
        "sme_team");

    gstbasesrc_class->create = GST_DEBUG_FUNCPTR (gst_sme_ffpro_src_create);
    gstbasesrc_class->query = GST_DEBUG_FUNCPTR (gst_sme_ffpro_src_query);
//    gstbasesrc_class->is_seekable = GST_DEBUG_FUNCPTR (gst_sme_ffpro_src_is_seekable);

    GST_INFO("gstbasesrc_class obj.addr=%p function.addr=%p",
        gstbasesrc_class, gstbasesrc_class->create);
}

///////////////////////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H

#include "config.h"

//for dynamic load smeffprosrc plugin
#ifdef GST_PLUGIN_BUILD_STATIC
#undef GST_PLUGIN_BUILD_STATIC
#endif

#endif

#include "gstsme.h"

static gboolean
plugin_init (GstPlugin * plugin)
{
    GST_INFO("register sme_ffpro_src begin...");
    //register ffprosrc element
    if (!gst_smeffprosrc_register (plugin))
        GST_WARNING("register sme_ffpro_src fail.");

    GST_INFO("register sme_ffpro_src end.");

    return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    smeffprosrc,
    "sme ffpro_src plugin of mediaos. (info about sme: http://www.huawei.com)",
    plugin_init, VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
