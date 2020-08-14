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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>

#include "gstomxwmvdec.h"

GST_DEBUG_CATEGORY_STATIC (gst_omx_wmv_dec_debug_category);
#define GST_CAT_DEFAULT gst_omx_wmv_dec_debug_category

/* prototypes */
static gboolean gst_omx_wmv_dec_is_format_change (GstOMXVideoDec * dec,
    GstOMXPort * port, GstVideoCodecState * state);
static gboolean gst_omx_wmv_dec_set_format (GstOMXVideoDec * dec,
    GstOMXPort * port, GstVideoCodecState * state);

enum
{
  PROP_0
};

/* class initialization */

#define DEBUG_INIT \
  GST_DEBUG_CATEGORY_INIT (gst_omx_wmv_dec_debug_category, "omxwmvdec", 0, \
      "debug category for gst-omx video decoder base class");

G_DEFINE_TYPE_WITH_CODE (GstOMXWMVDec, gst_omx_wmv_dec,
    GST_TYPE_OMX_VIDEO_DEC, DEBUG_INIT);

static void
gst_omx_wmv_dec_class_init (GstOMXWMVDecClass * klass)
{
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
  GstOMXVideoDecClass *videodec_class = GST_OMX_VIDEO_DEC_CLASS (klass);

  GST_INFO("gst_omx_wmv_dec_class_init");

  videodec_class->is_format_change =
      GST_DEBUG_FUNCPTR (gst_omx_wmv_dec_is_format_change);
  videodec_class->set_format = GST_DEBUG_FUNCPTR (gst_omx_wmv_dec_set_format);

#ifndef SME_GST_OMX
  videodec_class->cdata.default_sink_template_caps = "video/x-wmv, "
      "width=(int) [1,MAX], " "height=(int) [1,MAX]";
#else
  videodec_class->cdata.default_sink_template_caps = "video/x-wmv, "
      "width=(int) [1,MAX], " "height=(int) [1,MAX]", "wmvversion=(int)3" ;
#endif
  gst_element_class_set_static_metadata (element_class,
      "OpenMAX WMV Video Decoder",
      "Codec/Decoder/Video",
      "Decode WMV video streams",
      "Sebastian Dröge <sebastian.droege@collabora.co.uk>");

  gst_omx_set_default_role (&videodec_class->cdata, "video_decoder.wmv");
}

static void
gst_omx_wmv_dec_init (GstOMXWMVDec * self)
{
}

static gboolean
gst_omx_wmv_dec_is_format_change (GstOMXVideoDec * dec,
    GstOMXPort * port, GstVideoCodecState * state)
{
  return FALSE;
}

static gboolean
gst_omx_wmv_dec_set_format (GstOMXVideoDec * dec, GstOMXPort * port,
    GstVideoCodecState * state)
{
  gboolean ret;
  OMX_PARAM_PORTDEFINITIONTYPE port_def;
  OMX_VIDEO_PARAM_VC1TYPE VideoVC1;

  GST_INFO("gst_omx_wmv_dec_set_format");

  gst_omx_port_get_port_definition (port, &port_def);
  port_def.format.video.eCompressionFormat = OMX_VIDEO_CodingVC1;
  ret = gst_omx_port_update_port_definition (port, &port_def) == OMX_ErrorNone;

  /* BEGIN: Added by liurongliang(l00180035), 2015/4/1 */
  if(state->caps){
    GstStructure *structure = gst_caps_get_structure (state->caps, 0);
    const gchar *profile = NULL;
    if (gst_structure_has_name (structure, "video/x-wmv")) {
      if (NULL != (profile = gst_structure_get_string (structure, "profile"))){
        GST_INFO("wmv profile=%s", profile);
        if(!g_ascii_strncasecmp(profile, "VC1AP", MIN(sizeof(profile),sizeof("VC1AP")))){
          gst_omx_component_get_parameter (port->comp, OMX_IndexParamVideoVC1, &VideoVC1);
          GST_INFO("get OMX_IndexParamVideoVC1: VideoVC1.nPortIndex=%ld,profile=%d",
            VideoVC1.nPortIndex, VideoVC1.eProfile);
          VideoVC1.eProfile = OMX_VIDEO_VC1ProfileAdvanced;
          VideoVC1.nSize   = sizeof(OMX_VIDEO_PARAM_VC1TYPE);
          gst_omx_component_set_parameter (port->comp, OMX_IndexParamVideoVC1, &VideoVC1);
          gst_omx_component_get_parameter (port->comp, OMX_IndexParamVideoVC1, &VideoVC1);
          GST_INFO("after set, get OMX_IndexParamVideoVC1: VideoVC1.nPortIndex=%ld, profile=%d",
            VideoVC1.nPortIndex, VideoVC1.eProfile);
        }
      }
    }
  }
  /* END:   Added by liurongliang(l00180035), 2015/4/1 */

  return ret;
}
