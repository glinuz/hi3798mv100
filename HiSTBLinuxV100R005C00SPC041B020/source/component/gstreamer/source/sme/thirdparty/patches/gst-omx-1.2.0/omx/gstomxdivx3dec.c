#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>

#include "gstomxdivx3dec.h"

GST_DEBUG_CATEGORY_STATIC (gst_omx_divx3_dec_debug_category);
#define GST_CAT_DEFAULT gst_omx_divx3_dec_debug_category

/* prototypes */
static gboolean gst_omx_divx3_dec_is_format_change (GstOMXVideoDec * dec,
    GstOMXPort * port, GstVideoCodecState * state);
static gboolean gst_omx_divx3_dec_set_format (GstOMXVideoDec * dec,
    GstOMXPort * port, GstVideoCodecState * state);

enum
{
  PROP_0
};

/* class initialization */

#define DEBUG_INIT \
  GST_DEBUG_CATEGORY_INIT (gst_omx_divx3_dec_debug_category, "omxdivx3dec", 0, \
      "debug category for gst-omx video decoder base class");

G_DEFINE_TYPE_WITH_CODE (GstOMXDIVX3Dec, gst_omx_divx3_dec,
    GST_TYPE_OMX_VIDEO_DEC, DEBUG_INIT);

static void
gst_omx_divx3_dec_class_init (GstOMXDIVX3DecClass * klass)
{
  GstOMXVideoDecClass *videodec_class = GST_OMX_VIDEO_DEC_CLASS (klass);
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);

  GST_INFO("gst_omx_divx3_dec_class_init");
  videodec_class->is_format_change =
      GST_DEBUG_FUNCPTR (gst_omx_divx3_dec_is_format_change);
  videodec_class->set_format = GST_DEBUG_FUNCPTR (gst_omx_divx3_dec_set_format);

  videodec_class->cdata.default_sink_template_caps = "video/divx3, "
      "width=(int) [1,MAX], " "height=(int) [1,MAX], "
      "framerate=" GST_VIDEO_FPS_RANGE;


  gst_element_class_set_static_metadata (element_class,
      "OpenMAX DIVX3 Video Decoder",
      "Codec/Decoder/Video",
      "Decode DIVX3 video streams",
      "Sebastian Dröge <sebastian.droege@collabora.co.uk>");

  gst_omx_set_default_role (&videodec_class->cdata, "video_decoder.divx3");
}

static void
gst_omx_divx3_dec_init (GstOMXDIVX3Dec * self)
{
}

static gboolean
gst_omx_divx3_dec_is_format_change (GstOMXVideoDec * dec,
    GstOMXPort * port, GstVideoCodecState * state)
{
  return FALSE;
}

static gboolean
gst_omx_divx3_dec_set_format (GstOMXVideoDec * dec, GstOMXPort * port,
    GstVideoCodecState * state)
{
  gboolean ret;
  OMX_PARAM_PORTDEFINITIONTYPE port_def;

  GST_INFO("gst_omx_divx3_dec_set_format");
  gst_omx_port_get_port_definition (port, &port_def);
  port_def.format.video.eCompressionFormat = OMX_VIDEO_CodingDIVX3;
  ret = gst_omx_port_update_port_definition (port, &port_def) == OMX_ErrorNone;

  return ret;
}
