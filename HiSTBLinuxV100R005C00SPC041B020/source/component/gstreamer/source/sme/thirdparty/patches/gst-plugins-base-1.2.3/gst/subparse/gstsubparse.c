/* GStreamer
 * Copyright (C) <1999> Erik Walthinsen <omega@cse.ogi.edu>
 * Copyright (C) 2004 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2006 Tim-Philipp Müller <tim centricular net>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <glib.h>

#include "gstsubparse.h"
#include "gstssaparse.h"
#include "samiparse.h"
#include "tmplayerparse.h"
#include "mpl2parse.h"
#include "qttextparse.h"

//use hisi charset for
#ifdef __USE_HISI_CHAR_DET__
#include "hi_svr_charset.h"
#include "hi_svr_charset_norm.h"
#define TYPEFIND_SIZE 1025
static GMutex g_shisi_char_dec_mutex;
static gboolean g_sbinit_mutex = FALSE;
#else
#define TYPEFIND_SIZE 129
#endif

GST_DEBUG_CATEGORY (sub_parse_debug);

#define DEFAULT_ENCODING   NULL

enum
{
  PROP_0,
  PROP_ENCODING,
  PROP_VIDEOFPS
};

static void
gst_sub_parse_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void
gst_sub_parse_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);


static GstStaticPadTemplate sink_templ = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("application/x-subtitle; application/x-subtitle-sami; "
        "application/x-subtitle-tmplayer; application/x-subtitle-mpl2; "
        "application/x-subtitle-dks; application/x-subtitle-qttext")
    );

static GstStaticPadTemplate src_templ = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("text/x-raw, format= { pango-markup, utf8 }")
    );


static gboolean gst_sub_parse_src_event (GstPad * pad, GstObject * parent,
    GstEvent * event);
static gboolean gst_sub_parse_src_query (GstPad * pad, GstObject * parent,
    GstQuery * query);
static gboolean gst_sub_parse_sink_event (GstPad * pad, GstObject * parent,
    GstEvent * event);

static GstStateChangeReturn gst_sub_parse_change_state (GstElement * element,
    GstStateChange transition);

static GstFlowReturn gst_sub_parse_chain (GstPad * sinkpad, GstObject * parent,
    GstBuffer * buf);

static gchar*
gst_convert_with_fallback (const gchar *str,
		 gssize       len,
		 const gchar *to_codeset,
		 const gchar *from_codeset,
		 const gchar *fallback,
		 gsize       *bytes_read,
		 gsize       *bytes_written,
		 GError     **error);

#define gst_sub_parse_parent_class parent_class
G_DEFINE_TYPE (GstSubParse, gst_sub_parse, GST_TYPE_ELEMENT);

static void
gst_sub_parse_dispose (GObject * object)
{
  GstSubParse *subparse = GST_SUBPARSE (object);

  GST_DEBUG_OBJECT (subparse, "cleaning up subtitle parser");

  if (subparse->encoding) {
    g_free (subparse->encoding);
    subparse->encoding = NULL;
  }

  if (subparse->detected_encoding) {
    g_free (subparse->detected_encoding);
    subparse->detected_encoding = NULL;
  }

  if (subparse->adapter) {
    g_object_unref (subparse->adapter);
    subparse->adapter = NULL;
  }

  if (subparse->textbuf) {
    g_string_free (subparse->textbuf, TRUE);
    subparse->textbuf = NULL;
  }

  GST_CALL_PARENT (G_OBJECT_CLASS, dispose, (object));
}

static void
gst_sub_parse_class_init (GstSubParseClass * klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);

  object_class->dispose = gst_sub_parse_dispose;
  object_class->set_property = gst_sub_parse_set_property;
  object_class->get_property = gst_sub_parse_get_property;

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&sink_templ));
  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&src_templ));
  gst_element_class_set_static_metadata (element_class,
      "Subtitle parser", "Codec/Parser/Subtitle",
      "Parses subtitle (.sub) files into text streams",
      "Gustavo J. A. M. Carneiro <gjc@inescporto.pt>, "
      "GStreamer maintainers <gstreamer-devel@lists.sourceforge.net>");

  element_class->change_state = gst_sub_parse_change_state;

  g_object_class_install_property (object_class, PROP_ENCODING,
      g_param_spec_string ("subtitle-encoding", "subtitle charset encoding",
          "Encoding to assume if input subtitles are not in UTF-8 or any other "
          "Unicode encoding. If not set, the GST_SUBTITLE_ENCODING environment "
          "variable will be checked for an encoding to use. If that is not set "
          "either, ISO-8859-15 will be assumed.", DEFAULT_ENCODING,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_VIDEOFPS,
      gst_param_spec_fraction ("video-fps", "Video framerate",
          "Framerate of the video stream. This is needed by some subtitle "
          "formats to synchronize subtitles and video properly. If not set "
          "and the subtitle format requires it subtitles may be out of sync.",
          0, 1, G_MAXINT, 1, 24000, 1001,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}

static void
gst_sub_parse_init (GstSubParse * subparse)
{
  subparse->sinkpad = gst_pad_new_from_static_template (&sink_templ, "sink");
  gst_pad_set_chain_function (subparse->sinkpad,
      GST_DEBUG_FUNCPTR (gst_sub_parse_chain));
  gst_pad_set_event_function (subparse->sinkpad,
      GST_DEBUG_FUNCPTR (gst_sub_parse_sink_event));
  gst_element_add_pad (GST_ELEMENT (subparse), subparse->sinkpad);

  subparse->srcpad = gst_pad_new_from_static_template (&src_templ, "src");
  gst_pad_set_event_function (subparse->srcpad,
      GST_DEBUG_FUNCPTR (gst_sub_parse_src_event));
  gst_pad_set_query_function (subparse->srcpad,
      GST_DEBUG_FUNCPTR (gst_sub_parse_src_query));
  gst_element_add_pad (GST_ELEMENT (subparse), subparse->srcpad);

  subparse->textbuf = g_string_new (NULL);
  subparse->parser_type = GST_SUB_PARSE_FORMAT_UNKNOWN;
  subparse->flushing = FALSE;
  gst_segment_init (&subparse->segment, GST_FORMAT_TIME);
  subparse->need_segment = TRUE;
  subparse->encoding = g_strdup (DEFAULT_ENCODING);
  subparse->detected_encoding = NULL;
  subparse->adapter = gst_adapter_new ();

  subparse->fps_n = 24000;
  subparse->fps_d = 1001;
}

/*
 * Source pad functions.
 */

static gboolean
gst_sub_parse_src_query (GstPad * pad, GstObject * parent, GstQuery * query)
{
  GstSubParse *self = GST_SUBPARSE (parent);
  gboolean ret = FALSE;

  GST_DEBUG ("Handling %s query", GST_QUERY_TYPE_NAME (query));

  switch (GST_QUERY_TYPE (query)) {
    case GST_QUERY_POSITION:{
      GstFormat fmt;

      gst_query_parse_position (query, &fmt, NULL);
      if (fmt != GST_FORMAT_TIME) {
        ret = gst_pad_peer_query (self->sinkpad, query);
      } else {
        ret = TRUE;
        gst_query_set_position (query, GST_FORMAT_TIME, self->segment.position);
      }
      break;
    }
    case GST_QUERY_SEEKING:
    {
      GstFormat fmt;
      gboolean seekable = FALSE;

      ret = TRUE;

      gst_query_parse_seeking (query, &fmt, NULL, NULL, NULL);
      if (fmt == GST_FORMAT_TIME) {
        GstQuery *peerquery = gst_query_new_seeking (GST_FORMAT_BYTES);

        seekable = gst_pad_peer_query (self->sinkpad, peerquery);
        if (seekable)
          gst_query_parse_seeking (peerquery, NULL, &seekable, NULL, NULL);
        gst_query_unref (peerquery);
      }

      gst_query_set_seeking (query, fmt, seekable, seekable ? 0 : -1, -1);
      break;
    }
    default:
      ret = gst_pad_query_default (pad, parent, query);
      break;
  }

  return ret;
}

static gboolean
gst_sub_parse_src_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
  GstSubParse *self = GST_SUBPARSE (parent);
  gboolean ret = FALSE;

  GST_INFO ("Handling %s event", GST_EVENT_TYPE_NAME (event));

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_SEEK:
    {
      GstFormat format;
      GstSeekFlags flags;
      GstSeekType start_type, stop_type;
      gint64 start, stop;
      gdouble rate;
      gboolean update;

      gst_event_parse_seek (event, &rate, &format, &flags,
          &start_type, &start, &stop_type, &stop);

      if (format != GST_FORMAT_TIME) {
        GST_WARNING_OBJECT (self, "we only support seeking in TIME format");
        gst_event_unref (event);
        goto beach;
      }

      /* Convert that seek to a seeking in bytes at position 0,
         FIXME: could use an index */
      ret = gst_pad_push_event (self->sinkpad,
          gst_event_new_seek (rate, GST_FORMAT_BYTES, flags,
              GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_NONE, 0));

      if (ret) {
        /* Apply the seek to our segment */
        gst_segment_do_seek (&self->segment, rate, format, flags,
            start_type, start, stop_type, stop, &update);

        GST_INFO_OBJECT (self, "segment after seek: %" GST_SEGMENT_FORMAT,
            &self->segment);

        self->need_segment = TRUE;
      } else {
        GST_WARNING_OBJECT (self, "seek to 0 bytes failed");
      }

      gst_event_unref (event);
      break;
    }
    default:
      ret = gst_pad_event_default (pad, parent, event);
      break;
  }

beach:
  return ret;
}

static void
gst_sub_parse_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstSubParse *subparse = GST_SUBPARSE (object);

  GST_OBJECT_LOCK (subparse);
  switch (prop_id) {
    case PROP_ENCODING:
      g_free (subparse->encoding);
      subparse->encoding = g_value_dup_string (value);
      GST_LOG_OBJECT (object, "subtitle encoding set to %s",
          GST_STR_NULL (subparse->encoding));
      break;
    case PROP_VIDEOFPS:
    {
      subparse->fps_n = gst_value_get_fraction_numerator (value);
      subparse->fps_d = gst_value_get_fraction_denominator (value);
      GST_DEBUG_OBJECT (object, "video framerate set to %d/%d", subparse->fps_n,
          subparse->fps_d);

      if (!subparse->state.have_internal_fps) {
        subparse->state.fps_n = subparse->fps_n;
        subparse->state.fps_d = subparse->fps_d;
      }
      break;
    }
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
  GST_OBJECT_UNLOCK (subparse);
}

static void
gst_sub_parse_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstSubParse *subparse = GST_SUBPARSE (object);

  GST_OBJECT_LOCK (subparse);
  switch (prop_id) {
    case PROP_ENCODING:
      g_value_set_string (value, subparse->encoding);
      break;
    case PROP_VIDEOFPS:
      gst_value_set_fraction (value, subparse->fps_n, subparse->fps_d);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
  GST_OBJECT_UNLOCK (subparse);
}

static const gchar *
gst_sub_parse_get_format_description (GstSubParseFormat format)
{
  switch (format) {
    case GST_SUB_PARSE_FORMAT_MDVDSUB:
      return "MicroDVD";
    case GST_SUB_PARSE_FORMAT_SUBRIP:
      return "SubRip";
    case GST_SUB_PARSE_FORMAT_MPSUB:
      return "MPSub";
    case GST_SUB_PARSE_FORMAT_SAMI:
      return "SAMI";
    case GST_SUB_PARSE_FORMAT_TMPLAYER:
      return "TMPlayer";
    case GST_SUB_PARSE_FORMAT_MPL2:
      return "MPL2";
    case GST_SUB_PARSE_FORMAT_SUBVIEWER:
      return "SubViewer";
    case GST_SUB_PARSE_FORMAT_DKS:
      return "DKS";
    case GST_SUB_PARSE_FORMAT_QTTEXT:
      return "QTtext";
    default:
    case GST_SUB_PARSE_FORMAT_UNKNOWN:
      break;
  }
  return NULL;
}

static gchar *
gst_convert_to_utf8 (const gchar * str, gsize len, const gchar * encoding,
    gsize * consumed, GError ** err)
{
  gchar *ret = NULL;

  *consumed = 0;
  /* The char cast is necessary in glib < 2.24 */
#if 0
  ret =
      g_convert_with_fallback (str, len, "UTF-8", encoding, (char *) "*",
      consumed, NULL, err);
#else
  ret = gst_convert_with_fallback (str, len, "UTF-8", encoding, (char *) "*",
      consumed, NULL, err);
#endif
  if (ret == NULL)
    return ret;

  /* + 3 to skip UTF-8 BOM if it was added */
  len = strlen (ret);
  if (len >= 3 && (guint8) ret[0] == 0xEF && (guint8) ret[1] == 0xBB
      && (guint8) ret[2] == 0xBF)
    memmove (ret, ret + 3, len + 1 - 3);

  return ret;
}

#ifdef __USE_HISI_CHAR_DET__
static gchar* hicodetype_to_iconv_type(HI_CHARSET_CODETYPE_E eCodeType)
{
  gchar * ret = NULL;

  switch(eCodeType){
    case HI_CHARSET_CODETYPE_BIG5:{
      ret = g_strdup("BIG-5");
      break;
    }
    case HI_CHARSET_CODETYPE_UTF8:{
      ret = g_strdup("UTF-8");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_1:{
      ret = g_strdup("ISO8859-1");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_2:{
      ret = g_strdup("ISO8859-2");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_3:{
      ret = g_strdup("ISO8859-3");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_4:{
      ret = g_strdup("ISO8859-4");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_5:{
      ret = g_strdup("ISO8859-5");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_6:{
      ret = g_strdup("ISO8859-6");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_7:{
      ret = g_strdup("ISO8859-7");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_8:{
      ret = g_strdup("ISO8859-8");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_9:{
      ret = g_strdup("ISO8859-9");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_10:{
      ret = g_strdup("ISO8859-10");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_11:{
      ret = g_strdup("ISO8859-11");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_13:{
      ret = g_strdup("ISO8859-13");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_14:{
      ret = g_strdup("ISO8859-14");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_15:{
      ret = g_strdup("ISO8859-15");
      break;
    }
    case HI_CHARSET_CODETYPE_ISO8859_16:{
      ret = g_strdup("ISO8859-16");
      break;
    }
    case HI_CHARSET_CODETYPE_UNICODE_16LE:{
      ret = g_strdup("UCS-2LE");
      break;
    }
    case HI_CHARSET_CODETYPE_UNICODE_16BE:{
      ret = g_strdup("UCS-2BE");
      break;
    }
    case HI_CHARSET_CODETYPE_GBK:{
      ret = g_strdup("GBK");
      break;
    }
    case HI_CHARSET_CODETYPE_CP1250:{
      ret = g_strdup("CP1250");
      break;
    }
    case HI_CHARSET_CODETYPE_CP1251:{
      ret = g_strdup("CP1251");
      break;
    }
    case HI_CHARSET_CODETYPE_CP1252:{
      ret = g_strdup("CP1252");
      break;
    }
    case HI_CHARSET_CODETYPE_CP1253:{
      ret = g_strdup("CP1253");
      break;
    }
    case HI_CHARSET_CODETYPE_CP1254:{
      ret = g_strdup("CP1254");
      break;
    }
    case HI_CHARSET_CODETYPE_CP1255:{
      ret = g_strdup("CP1255");
      break;
    }
    case HI_CHARSET_CODETYPE_CP1256:{
      ret = g_strdup("CP1256");
      break;
    }
    case HI_CHARSET_CODETYPE_CP1257:{
      ret = g_strdup("CP1257");
      break;
    }
    case HI_CHARSET_CODETYPE_CP1258:{
      ret = g_strdup("CP1258");
      break;
    }
    case HI_CHARSET_CODETYPE_CP874:{
      ret = g_strdup("CP874");
      break;
    }
    case HI_CHARSET_CODETYPE_UNICODE_32LE:{
      ret = g_strdup("UCS-4LE");
      break;
    }
    case HI_CHARSET_CODETYPE_UNICODE_32BE:{
      ret = g_strdup("UCS-4BE");
      break;
    }
    case HI_CHARSET_CODETYPE_CP949:{
      ret = g_strdup("CP949");
      break;
    }
    default:
      GST_WARNING("eCodeType=%d, not support!", eCodeType);
  }

  return ret;
}
#endif

static gchar *
detect_encoding (const gchar * str, gsize len)
{
#ifdef __USE_HISI_CHAR_DET__
  HI_S32 s_err = HI_SUCCESS;
  gchar* ret = NULL;
  HI_HANDLE det_handl = HI_INVALID_HANDLE;
  HI_CHARSET_CODETYPE_E eCodeType = HI_CHARSET_CODETYPE_NONE;
  HI_U32 userdata = 1;
  gboolean inited_charset = FALSE;

#endif

#ifdef __USE_HISI_CHAR_DET__

  //detect if utf8
  if (len >= 3 && (guint8) str[0] == 0xEF && (guint8) str[1] == 0xBB
      && (guint8) str[2] == 0xBF){
    GST_INFO("utf-8 detected!");
    return g_strdup ("UTF-8");
  }

  /* BEGIN: Modified for DTS2015072705793by liurongliang(l00180035), 2015/7/27 */
#ifdef __USE_HISI_CHAR_DET__
  g_mutex_lock(&g_shisi_char_dec_mutex);
#endif
  do{
    GST_WARNING("now use charset to detect");
    s_err = HI_CHARSET_Init();
    if(HI_SUCCESS != s_err){
      GST_WARNING("HI_CHARSET_Init failed,s_err = %#x", s_err);
      break;
    }
    s_err = HI_CHARSET_RegisterDynamic("libhi_charset.so");
    if(HI_SUCCESS != s_err){
      GST_WARNING_OBJECT("HI_CHARSET_RegisterDynamic, failed,s_err=%#x", s_err);
      break;
    }
    s_err = g_stCharsetMgr_s.Create(&det_handl, userdata);
    if(HI_SUCCESS != s_err){
      GST_WARNING("Create charset failed!");
      break;
    }
    s_err = g_stCharsetMgr_s.IdentStream(det_handl, str, len, userdata, (HI_S32*)&eCodeType);
    if(HI_SUCCESS != s_err){
      GST_WARNING("IdentStream failed, s_err=%#x", s_err);
      break;
    }
    GST_INFO("IdentStream result, eCodeType=%d", eCodeType);
    ret = hicodetype_to_iconv_type(eCodeType);
    if(!ret){
      GST_WARNING("hicodetype_to_iconv_type failed of type =%d", eCodeType);
    }else{
      GST_WARNING("hicodetype_to_iconv_type result:%s", ret);
    }
  }while(FALSE);

  if(HI_INVALID_HANDLE != det_handl){
    (void)g_stCharsetMgr_s.Destroy(det_handl);
    det_handl = HI_INVALID_HANDLE;
  }
  (void)HI_CHARSET_Deinit();
#ifdef __USE_HISI_CHAR_DET__
  g_mutex_unlock(&g_shisi_char_dec_mutex);
#endif
  /* END:   Modified for DTS2015072705793by liurongliang(l00180035), 2015/7/27 */

  if(!ret){
    if (len >= 2 && (guint8) str[0] == 0xFE && (guint8) str[1] == 0xFF)
      return g_strdup ("UTF-16BE");

    if (len >= 2 && (guint8) str[0] == 0xFF && (guint8) str[1] == 0xFE)
      return g_strdup ("UTF-16LE");

    if (len >= 4 && (guint8) str[0] == 0x00 && (guint8) str[1] == 0x00
        && (guint8) str[2] == 0xFE && (guint8) str[3] == 0xFF)
      return g_strdup ("UTF-32BE");

    if (len >= 4 && (guint8) str[0] == 0xFF && (guint8) str[1] == 0xFE
        && (guint8) str[2] == 0x00 && (guint8) str[3] == 0x00)
      return g_strdup ("UTF-32LE");
  }

  return ret;
#else
  if (len >= 3 && (guint8) str[0] == 0xEF && (guint8) str[1] == 0xBB
      && (guint8) str[2] == 0xBF)
    return g_strdup ("UTF-8");

  if (len >= 2 && (guint8) str[0] == 0xFE && (guint8) str[1] == 0xFF)
    return g_strdup ("UTF-16BE");

  if (len >= 2 && (guint8) str[0] == 0xFF && (guint8) str[1] == 0xFE)
    return g_strdup ("UTF-16LE");

  if (len >= 4 && (guint8) str[0] == 0x00 && (guint8) str[1] == 0x00
      && (guint8) str[2] == 0xFE && (guint8) str[3] == 0xFF)
    return g_strdup ("UTF-32BE");

  if (len >= 4 && (guint8) str[0] == 0xFF && (guint8) str[1] == 0xFE
      && (guint8) str[2] == 0x00 && (guint8) str[3] == 0x00)
    return g_strdup ("UTF-32LE");

  return NULL;
#endif
}

static gchar *
convert_encoding (GstSubParse * self, const gchar * str, gsize len,
    gsize * consumed)
{
  const gchar *encoding;
  GError *err = NULL;
  gchar *ret = NULL;

  *consumed = 0;

  /* First try any detected encoding */
  if (self->detected_encoding) {
    GST_INFO("detect char encode, detected_encoding=%s", self->detected_encoding);
    ret =
        gst_convert_to_utf8 (str, len, self->detected_encoding, consumed, &err);

    if (!err)
      return ret;

    GST_WARNING_OBJECT (self, "could not convert string from '%s' to UTF-8: %s",
        self->detected_encoding, err->message);
    g_free (self->detected_encoding);
    self->detected_encoding = NULL;
    g_error_free (err);
    err = NULL;
  }else{
    GST_WARNING("not detect char encode");
  }

  /* Otherwise check if it's UTF8 */
  if (self->valid_utf8) {
    if (g_utf8_validate (str, len, NULL)) {
      GST_LOG_OBJECT (self, "valid UTF-8, no conversion needed");
      *consumed = len;
      return g_strndup (str, len);
    }
    GST_INFO_OBJECT (self, "invalid UTF-8!");
    self->valid_utf8 = FALSE;
  }

  /* Else try fallback */
  encoding = self->encoding;
  if (encoding == NULL || *encoding == '\0') {
    encoding = g_getenv ("GST_SUBTITLE_ENCODING");
  }
  if (encoding == NULL || *encoding == '\0') {
    /* if local encoding is UTF-8 and no encoding specified
     * via the environment variable, assume ISO-8859-15 */
    if (g_get_charset (&encoding)) {
      encoding = "ISO-8859-15";
    }
  }

  ret = gst_convert_to_utf8 (str, len, encoding, consumed, &err);

  if (err) {
    GST_WARNING_OBJECT (self, "could not convert string from '%s' to UTF-8: %s",
        encoding, err->message);
    g_error_free (err);
    err = NULL;

    /* invalid input encoding, fall back to ISO-8859-15 (always succeeds) */
    ret = gst_convert_to_utf8 (str, len, "ISO-8859-15", consumed, NULL);
  }

  GST_INFO_OBJECT (self,
      "successfully converted %" G_GSIZE_FORMAT " characters from %s to UTF-8"
      "%s,ret=%p", len, encoding, (err) ? " , using ISO-8859-15 as fallback" : "",ret);

  return ret;
}

static gchar *
get_next_line (GstSubParse * self)
{
  char *line = NULL;
  const char *line_end;
  int line_len;
  gboolean have_r = FALSE;

  line_end = strchr (self->textbuf->str, '\n');

  if (!line_end) {
    /* end-of-line not found; return for more data */
    return NULL;
  }

  /* get rid of '\r' */
  if (line_end != self->textbuf->str && *(line_end - 1) == '\r') {
    line_end--;
    have_r = TRUE;
  }

  line_len = line_end - self->textbuf->str;
  line = g_strndup (self->textbuf->str, line_len);
  self->textbuf = g_string_erase (self->textbuf, 0,
      line_len + (have_r ? 2 : 1));
  return line;
}

static gchar *
parse_mdvdsub (ParserState * state, const gchar * line)
{
  const gchar *line_split;
  gchar *line_chunk;
  guint start_frame, end_frame;
  guint64 clip_start = 0, clip_stop = 0;
  gboolean in_seg = FALSE;
  GString *markup;
  gchar *ret;

  /* style variables */
  gboolean italic;
  gboolean bold;
  guint fontsize;
  gdouble fps = 0.0;

  if (sscanf (line, "{%u}{%u}", &start_frame, &end_frame) != 2) {
    g_warning ("Parse of the following line, assumed to be in microdvd .sub"
        " format, failed:\n%s", line);
    return NULL;
  }

  /* skip the {%u}{%u} part */
  line = strchr (line, '}') + 1;
  line = strchr (line, '}') + 1;

  /* see if there's a first line with a framerate */
  if (start_frame == 1 && end_frame == 1) {
    gchar *rest, *end = NULL;

    rest = g_strdup (line);
    g_strdelimit (rest, ",", '.');
    fps = g_ascii_strtod (rest, &end);
    if (end != rest) {
      gst_util_double_to_fraction (fps, &state->fps_n, &state->fps_d);
      GST_INFO ("framerate from file: %d/%d ('%s')", state->fps_n,
          state->fps_d, rest);
    }
    g_free (rest);
    return NULL;
  }

  state->start_time =
      gst_util_uint64_scale (start_frame, GST_SECOND * state->fps_d,
      state->fps_n);
  state->duration =
      gst_util_uint64_scale (end_frame - start_frame, GST_SECOND * state->fps_d,
      state->fps_n);

  /* Check our segment start/stop */
  in_seg = gst_segment_clip (state->segment, GST_FORMAT_TIME,
      state->start_time, state->start_time + state->duration, &clip_start,
      &clip_stop);

  /* No need to parse that text if it's out of segment */
  if (in_seg) {
    state->start_time = clip_start;
    state->duration = clip_stop - clip_start;
  } else {
    return NULL;
  }

  markup = g_string_new (NULL);
  while (1) {
    italic = FALSE;
    bold = FALSE;
    fontsize = 0;
    /* parse style markup */
    if (strncmp (line, "{y:i}", 5) == 0) {
      italic = TRUE;
      line = strchr (line, '}') + 1;
    }
    if (strncmp (line, "{y:b}", 5) == 0) {
      bold = TRUE;
      line = strchr (line, '}') + 1;
    }
    if (sscanf (line, "{s:%u}", &fontsize) == 1) {
      line = strchr (line, '}') + 1;
    }
    /* forward slashes at beginning/end signify italics too */
    if (g_str_has_prefix (line, "/")) {
      italic = TRUE;
      ++line;
    }
    if ((line_split = strchr (line, '|')))
      line_chunk = g_markup_escape_text (line, line_split - line);
    else
      line_chunk = g_markup_escape_text (line, strlen (line));

    /* Remove italics markers at end of line/stanza (CHECKME: are end slashes
     * always at the end of a line or can they span multiple lines?) */
    if (g_str_has_suffix (line_chunk, "/")) {
      line_chunk[strlen (line_chunk) - 1] = '\0';
    }

    markup = g_string_append (markup, "<span");
    if (italic)
      g_string_append (markup, " style=\"italic\"");
    if (bold)
      g_string_append (markup, " weight=\"bold\"");
    if (fontsize)
      g_string_append_printf (markup, " size=\"%u\"", fontsize * 1000);
    g_string_append_printf (markup, ">%s</span>", line_chunk);
    g_free (line_chunk);
    if (line_split) {
      g_string_append (markup, "\n");
      line = line_split + 1;
    } else {
      break;
    }
  }
  ret = markup->str;
  g_string_free (markup, FALSE);
  GST_DEBUG ("parse_mdvdsub returning (%f+%f): %s",
      state->start_time / (double) GST_SECOND,
      state->duration / (double) GST_SECOND, ret);
  return ret;
}

static void
strip_trailing_newlines (gchar * txt)
{
  if (txt) {
    guint len;

    len = strlen (txt);
    while (len > 1 && txt[len - 1] == '\n') {
      txt[len - 1] = '\0';
      --len;
    }
  }
}

/* we want to escape text in general, but retain basic markup like
 * <i></i>, <u></u>, and <b></b>. The easiest and safest way is to
 * just unescape a white list of allowed markups again after
 * escaping everything (the text between these simple markers isn't
 * necessarily escaped, so it seems best to do it like this) */
static void
subrip_unescape_formatting (gchar * txt)
{
  gchar *pos;

  for (pos = txt; pos != NULL && *pos != '\0'; ++pos) {
    if (g_ascii_strncasecmp (pos, "&lt;u&gt;", 9) == 0 ||
        g_ascii_strncasecmp (pos, "&lt;i&gt;", 9) == 0 ||
        g_ascii_strncasecmp (pos, "&lt;b&gt;", 9) == 0) {
      pos[0] = '<';
      pos[1] = g_ascii_tolower (pos[4]);
      pos[2] = '>';
      /* move NUL terminator as well */
      memmove (pos + 3, pos + 9, strlen (pos + 9) + 1);
      pos += 2;
    }
  }

  for (pos = txt; pos != NULL && *pos != '\0'; ++pos) {
    if (g_ascii_strncasecmp (pos, "&lt;/u&gt;", 10) == 0 ||
        g_ascii_strncasecmp (pos, "&lt;/i&gt;", 10) == 0 ||
        g_ascii_strncasecmp (pos, "&lt;/b&gt;", 10) == 0) {
      pos[0] = '<';
      pos[1] = '/';
      pos[2] = g_ascii_tolower (pos[5]);
      pos[3] = '>';
      /* move NUL terminator as well */
      memmove (pos + 4, pos + 10, strlen (pos + 10) + 1);
      pos += 3;
    }
  }
}


static gboolean
subrip_remove_unhandled_tag (gchar * start, gchar * stop)
{
  gchar *tag, saved;

  tag = start + strlen ("&lt;");
  if (*tag == '/')
    ++tag;

  if (g_ascii_tolower (*tag) < 'a' || g_ascii_tolower (*tag) > 'z')
    return FALSE;

  saved = *stop;
  *stop = '\0';
  GST_LOG ("removing unhandled tag '%s'", start);
  *stop = saved;
  memmove (start, stop, strlen (stop) + 1);
  return TRUE;
}

/* remove tags we haven't explicitly allowed earlier on, like font tags
 * for example */
static void
subrip_remove_unhandled_tags (gchar * txt)
{
  gchar *pos, *gt;

  for (pos = txt; pos != NULL && *pos != '\0'; ++pos) {
    if (strncmp (pos, "&lt;", 4) == 0 && (gt = strstr (pos + 4, "&gt;"))) {
      if (subrip_remove_unhandled_tag (pos, gt + strlen ("&gt;")))
        --pos;
    }
  }
}

/* we only allow <i>, <u> and <b>, so let's take a simple approach. This code
 * assumes the input has been escaped and subrip_unescape_formatting() has then
 * been run over the input! This function adds missing closing markup tags and
 * removes broken closing tags for tags that have never been opened. */
static void
subrip_fix_up_markup (gchar ** p_txt)
{
  gchar *cur, *next_tag;
  gchar open_tags[32];
  guint num_open_tags = 0;

  g_assert (*p_txt != NULL);

  cur = *p_txt;
  while (*cur != '\0') {
    next_tag = strchr (cur, '<');
    if (next_tag == NULL)
      break;
    ++next_tag;
    switch (*next_tag) {
      case '/':{
        ++next_tag;
        if (num_open_tags == 0 || open_tags[num_open_tags - 1] != *next_tag) {
          GST_INFO ("broken input, closing tag '%c' is not open", *next_tag);
#if 0
          // do not remove here.
          memmove (next_tag - 2, next_tag + 2, strlen (next_tag + 2) + 1);
          next_tag -= 2;
#endif
        } else {
          /* it's all good, closing tag which is open */
          --num_open_tags;
        }
        break;
      }
      case 'i':
      case 'b':
      case 'u':
        if (num_open_tags == G_N_ELEMENTS (open_tags))
          return;               /* something dodgy is going on, stop parsing */
        open_tags[num_open_tags] = *next_tag;
        ++num_open_tags;
        break;
      default:
        GST_ERROR ("unexpected tag '%c' (%s)", *next_tag, next_tag);
        g_assert_not_reached ();
        break;
    }
    cur = next_tag;
  }

  if (num_open_tags > 0) {
    GString *s;

    s = g_string_new (*p_txt);
    while (num_open_tags > 0) {
      GST_LOG ("adding missing closing tag '%c'", open_tags[num_open_tags - 1]);
      g_string_append_c (s, '<');
      g_string_append_c (s, '/');
      g_string_append_c (s, open_tags[num_open_tags - 1]);
      g_string_append_c (s, '>');
      --num_open_tags;
    }
    g_free (*p_txt);
    *p_txt = g_string_free (s, FALSE);
  }
}

static gboolean
parse_subrip_time (const gchar * ts_string, GstClockTime * t)
{
  gchar s[128] = { '\0', };
  gchar *end, *p;
  guint hour, min, sec, msec, len;

  while (*ts_string == ' ')
    ++ts_string;

  g_strlcpy (s, ts_string, sizeof (s));
  if ((end = strstr (s, "-->")))
    *end = '\0';
  g_strchomp (s);

  /* ms may be in these formats:
   * hh:mm:ss,500 = 500ms
   * hh:mm:ss,  5 =   5ms
   * hh:mm:ss, 5  =  50ms
   * hh:mm:ss, 50 =  50ms
   * hh:mm:ss,5   = 500ms
   * and the same with . instead of ,.
   * sscanf() doesn't differentiate between '  5' and '5' so munge
   * the white spaces within the timestamp to '0' (I'm sure there's a
   * way to make sscanf() do this for us, but how?)
   */
  g_strdelimit (s, " ", '0');
  g_strdelimit (s, ".", ',');

  /* make sure we have exactly three digits after he comma */
  p = strchr (s, ',');
  g_assert (p != NULL);
  ++p;
  len = strlen (p);
  if (len > 3) {
    p[3] = '\0';
  } else
    while (len < 3) {
      g_strlcat (&p[len], "0", 2);
      ++len;
    }

  GST_LOG ("parsing timestamp '%s'", s);
  if (sscanf (s, "%u:%u:%u,%u", &hour, &min, &sec, &msec) != 4) {
    GST_WARNING ("failed to parse subrip timestamp string '%s'", s);
    return FALSE;
  }

  *t = ((hour * 3600) + (min * 60) + sec) * GST_SECOND + msec * GST_MSECOND;
  return TRUE;
}

static gchar *
parse_subrip (ParserState * state, const gchar * line)
{
  int subnum;
  gchar *ret;

  switch (state->state) {
    case 0:
      /* looking for a single integer */
      if (sscanf (line, "%u", &subnum) == 1)
        state->state = 1;
      return NULL;
    case 1:
    {
      GstClockTime ts_start, ts_end;
      gchar *end_time;

      /* looking for start_time --> end_time */
      if ((end_time = strstr (line, " --> ")) &&
          parse_subrip_time (line, &ts_start) &&
          parse_subrip_time (end_time + strlen (" --> "), &ts_end) &&
          state->start_time <= ts_end) {
        state->state = 2;
        state->start_time = ts_start;
        state->duration = ts_end - ts_start;
      } else {
        GST_DEBUG ("error parsing subrip time line '%s'", line);
        state->state = 0;
      }
      return NULL;
    }
    case 2:
    {
      /* No need to parse that text if it's out of segment */
      guint64 clip_start = 0, clip_stop = 0;
      gboolean in_seg = FALSE;

      /* Check our segment start/stop */
      in_seg = gst_segment_clip (state->segment, GST_FORMAT_TIME,
          state->start_time, state->start_time + state->duration,
          &clip_start, &clip_stop);

      if (in_seg) {
        state->start_time = clip_start;
        state->duration = clip_stop - clip_start;
      } else {
        state->state = 0;
        return NULL;
      }
    }
      /* looking for subtitle text; empty line ends this subtitle entry */
      if (state->buf->len)
        g_string_append_c (state->buf, '\n');
      g_string_append (state->buf, line);
      if (strlen (line) == 0) {
        //it will make some character error.
        //ret = g_markup_escape_text (state->buf->str, state->buf->len);
        if(strlen(state->buf->str)){
          ret = g_strdup(state->buf->str);
          if(!ret){
            GST_ERROR("strdup failed!");
            return NULL;
          }
        }else{
          ret = g_realloc(NULL, 2);
          if(!ret){
            GST_ERROR("g_realloc failed!");
            return NULL;
          }
          ret[0] = 0;
        }

        g_string_truncate (state->buf, 0);
        state->state = 0;
        subrip_unescape_formatting (ret);
        subrip_remove_unhandled_tags (ret);
        strip_trailing_newlines (ret);
        subrip_fix_up_markup (&ret);
        return ret;
      }
      return NULL;
    default:
      g_return_val_if_reached (NULL);
  }
}

static void
unescape_newlines_br (gchar * read)
{
  gchar *write = read;

  /* Replace all occurences of '[br]' with a newline as version 2
   * of the subviewer format uses this for newlines */

  if (read[0] == '\0' || read[1] == '\0' || read[2] == '\0' || read[3] == '\0')
    return;

  do {
    if (strncmp (read, "[br]", 4) == 0) {
      *write = '\n';
      read += 4;
    } else {
      *write = *read;
      read++;
    }
    write++;
  } while (*read);

  *write = '\0';
}

static gchar *
parse_subviewer (ParserState * state, const gchar * line)
{
  guint h1, m1, s1, ms1;
  guint h2, m2, s2, ms2;
  gchar *ret;

  /* TODO: Maybe also parse the fields in the header, especially DELAY.
   * For examples see the unit test or
   * http://www.doom9.org/index.html?/sub.htm */

  switch (state->state) {
    case 0:
      /* looking for start_time,end_time */
      if (sscanf (line, "%u:%u:%u.%u,%u:%u:%u.%u",
              &h1, &m1, &s1, &ms1, &h2, &m2, &s2, &ms2) == 8) {
        state->state = 1;
        state->start_time =
            (((guint64) h1) * 3600 + m1 * 60 + s1) * GST_SECOND +
            ms1 * GST_MSECOND;
        state->duration =
            (((guint64) h2) * 3600 + m2 * 60 + s2) * GST_SECOND +
            ms2 * GST_MSECOND - state->start_time;
      }
      return NULL;
    case 1:
    {
      /* No need to parse that text if it's out of segment */
      guint64 clip_start = 0, clip_stop = 0;
      gboolean in_seg = FALSE;

      /* Check our segment start/stop */
      in_seg = gst_segment_clip (state->segment, GST_FORMAT_TIME,
          state->start_time, state->start_time + state->duration,
          &clip_start, &clip_stop);

      if (in_seg) {
        state->start_time = clip_start;
        state->duration = clip_stop - clip_start;
      } else {
        state->state = 0;
        return NULL;
      }
    }
      /* looking for subtitle text; empty line ends this subtitle entry */
      if (state->buf->len)
        g_string_append_c (state->buf, '\n');
      g_string_append (state->buf, line);
      if (strlen (line) == 0) {
        ret = g_strdup (state->buf->str);
        unescape_newlines_br (ret);
        strip_trailing_newlines (ret);
        g_string_truncate (state->buf, 0);
        state->state = 0;
        return ret;
      }
      return NULL;
    default:
      g_assert_not_reached ();
      return NULL;
  }
}

static gchar *
parse_mpsub (ParserState * state, const gchar * line)
{
  gchar *ret;
  float t1, t2;

  switch (state->state) {
    case 0:
      /* looking for two floats (offset, duration) */
      if (sscanf (line, "%f %f", &t1, &t2) == 2) {
        state->state = 1;
        state->start_time += state->duration + GST_SECOND * t1;
        state->duration = GST_SECOND * t2;
      }
      return NULL;
    case 1:
    {                           /* No need to parse that text if it's out of segment */
      guint64 clip_start = 0, clip_stop = 0;
      gboolean in_seg = FALSE;

      /* Check our segment start/stop */
      in_seg = gst_segment_clip (state->segment, GST_FORMAT_TIME,
          state->start_time, state->start_time + state->duration,
          &clip_start, &clip_stop);

      if (in_seg) {
        state->start_time = clip_start;
        state->duration = clip_stop - clip_start;
      } else {
        state->state = 0;
        return NULL;
      }
    }
      /* looking for subtitle text; empty line ends this
       * subtitle entry */
      if (state->buf->len)
        g_string_append_c (state->buf, '\n');
      g_string_append (state->buf, line);
      if (strlen (line) == 0) {
        ret = g_strdup (state->buf->str);
        g_string_truncate (state->buf, 0);
        state->state = 0;
        return ret;
      }
      return NULL;
    default:
      g_assert_not_reached ();
      return NULL;
  }
}

static const gchar *
dks_skip_timestamp (const gchar * line)
{
  while (*line && *line != ']')
    line++;
  if (*line == ']')
    line++;
  return line;
}

static gchar *
parse_dks (ParserState * state, const gchar * line)
{
  guint h, m, s;

  switch (state->state) {
    case 0:
      /* Looking for the start time and text */
      if (sscanf (line, "[%u:%u:%u]", &h, &m, &s) == 3) {
        const gchar *text;
        state->start_time = (((guint64) h) * 3600 + m * 60 + s) * GST_SECOND;
        text = dks_skip_timestamp (line);
        if (*text) {
          state->state = 1;
          g_string_append (state->buf, text);
        }
      }
      return NULL;
    case 1:
    {
      guint64 clip_start = 0, clip_stop = 0;
      gboolean in_seg;
      gchar *ret;

      /* Looking for the end time */
      if (sscanf (line, "[%u:%u:%u]", &h, &m, &s) == 3) {
        state->state = 0;
        state->duration = (((guint64) h) * 3600 + m * 60 + s) * GST_SECOND -
            state->start_time;
      } else {
        GST_WARNING ("Failed to parse subtitle end time");
        return NULL;
      }

      /* Check if this subtitle is out of the current segment */
      in_seg = gst_segment_clip (state->segment, GST_FORMAT_TIME,
          state->start_time, state->start_time + state->duration,
          &clip_start, &clip_stop);

      if (!in_seg) {
        return NULL;
      }

      state->start_time = clip_start;
      state->duration = clip_stop - clip_start;

      ret = g_strdup (state->buf->str);
      g_string_truncate (state->buf, 0);
      unescape_newlines_br (ret);
      return ret;
    }
    default:
      g_assert_not_reached ();
      return NULL;
  }
}

static void
parser_state_init (ParserState * state)
{
  GST_DEBUG ("initialising parser");

  if (state->buf) {
    g_string_truncate (state->buf, 0);
  } else {
    state->buf = g_string_new (NULL);
  }

  state->start_time = 0;
  state->duration = 0;
  state->max_duration = 0;      /* no limit */
  state->state = 0;
  state->segment = NULL;
}

static void
parser_state_dispose (GstSubParse * self, ParserState * state)
{
  if (state->buf) {
    g_string_free (state->buf, TRUE);
    state->buf = NULL;
  }
  if (state->user_data) {
    switch (self->parser_type) {
      case GST_SUB_PARSE_FORMAT_QTTEXT:
        qttext_context_deinit (state);
        break;
      case GST_SUB_PARSE_FORMAT_SAMI:
        sami_context_deinit (state);
        break;
      default:
        break;
    }
  }
}

/* regex type enum */
typedef enum
{
  GST_SUB_PARSE_REGEX_UNKNOWN = 0,
  GST_SUB_PARSE_REGEX_MDVDSUB = 1,
  GST_SUB_PARSE_REGEX_SUBRIP = 2,
  GST_SUB_PARSE_REGEX_DKS = 3,
} GstSubParseRegex;

static gpointer
gst_sub_parse_data_format_autodetect_regex_once (GstSubParseRegex regtype)
{
  gpointer result = NULL;
  GError *gerr = NULL;
  switch (regtype) {
    case GST_SUB_PARSE_REGEX_MDVDSUB:
      result =
          (gpointer) g_regex_new ("^\\{[0-9]+\\}\\{[0-9]+\\}",
          G_REGEX_RAW | G_REGEX_OPTIMIZE, 0, &gerr);
      if (result == NULL) {
        g_warning ("Compilation of mdvd regex failed: %s", gerr->message);
        g_error_free (gerr);
      }
      break;
    case GST_SUB_PARSE_REGEX_SUBRIP:
      result = (gpointer)
          g_regex_new ("^[\\s\\n]*[\\n]? {0,3}[ 0-9]{1,4}\\s*(\x0d)?\x0a"
          " ?[0-9]{1,2}: ?[0-9]{1,2}: ?[0-9]{1,2}[,.] {0,2}[0-9]{1,3}"
          " +--> +[0-9]{1,2}: ?[0-9]{1,2}: ?[0-9]{1,2}[,.] {0,2}[0-9]{1,2}",
          G_REGEX_RAW | G_REGEX_OPTIMIZE, 0, &gerr);
      if (result == NULL) {
        g_warning ("Compilation of subrip regex failed: %s", gerr->message);
        g_error_free (gerr);
      }
      break;
    case GST_SUB_PARSE_REGEX_DKS:
      result = (gpointer) g_regex_new ("^\\[[0-9]+:[0-9]+:[0-9]+\\].*",
          G_REGEX_RAW | G_REGEX_OPTIMIZE, 0, &gerr);
      if (result == NULL) {
        g_warning ("Compilation of dks regex failed: %s", gerr->message);
        g_error_free (gerr);
      }
      break;
    default:
      GST_WARNING ("Trying to allocate regex of unknown type %u", regtype);
  }
  return result;
}

/*
 * FIXME: maybe we should pass along a second argument, the preceding
 * text buffer, because that is how this originally worked, even though
 * I don't really see the use of that.
 */

static GstSubParseFormat
gst_sub_parse_data_format_autodetect (gchar * match_str)
{
  guint n1, n2, n3;

  static GOnce mdvd_rx_once = G_ONCE_INIT;
  static GOnce subrip_rx_once = G_ONCE_INIT;
  static GOnce dks_rx_once = G_ONCE_INIT;

  GRegex *mdvd_grx;
  GRegex *subrip_grx;
  GRegex *dks_grx;

  g_once (&mdvd_rx_once,
      (GThreadFunc) gst_sub_parse_data_format_autodetect_regex_once,
      (gpointer) GST_SUB_PARSE_REGEX_MDVDSUB);
  g_once (&subrip_rx_once,
      (GThreadFunc) gst_sub_parse_data_format_autodetect_regex_once,
      (gpointer) GST_SUB_PARSE_REGEX_SUBRIP);
  g_once (&dks_rx_once,
      (GThreadFunc) gst_sub_parse_data_format_autodetect_regex_once,
      (gpointer) GST_SUB_PARSE_REGEX_DKS);

  mdvd_grx = (GRegex *) mdvd_rx_once.retval;
  subrip_grx = (GRegex *) subrip_rx_once.retval;
  dks_grx = (GRegex *) dks_rx_once.retval;

  if (g_regex_match (mdvd_grx, match_str, 0, NULL) == TRUE) {
    GST_LOG ("MicroDVD (frame based) format detected");
    return GST_SUB_PARSE_FORMAT_MDVDSUB;
  }
  if (g_regex_match (subrip_grx, match_str, 0, NULL) == TRUE) {
    GST_LOG ("SubRip (time based) format detected");
    return GST_SUB_PARSE_FORMAT_SUBRIP;
  }
  if (g_regex_match (dks_grx, match_str, 0, NULL) == TRUE) {
    GST_LOG ("DKS (time based) format detected");
    return GST_SUB_PARSE_FORMAT_DKS;
  }

  if (!strncmp (match_str, "FORMAT=TIME", 11)) {
    GST_LOG ("MPSub (time based) format detected");
    return GST_SUB_PARSE_FORMAT_MPSUB;
  }
  if (strstr (match_str, "<SAMI>") != NULL ||
      strstr (match_str, "<sami>") != NULL) {
    GST_LOG ("SAMI (time based) format detected");
    return GST_SUB_PARSE_FORMAT_SAMI;
  }
  /* we're boldly assuming the first subtitle appears within the first hour */
  if (sscanf (match_str, "0:%02u:%02u:", &n1, &n2) == 2 ||
      sscanf (match_str, "0:%02u:%02u=", &n1, &n2) == 2 ||
      sscanf (match_str, "00:%02u:%02u:", &n1, &n2) == 2 ||
      sscanf (match_str, "00:%02u:%02u=", &n1, &n2) == 2 ||
      sscanf (match_str, "00:%02u:%02u,%u=", &n1, &n2, &n3) == 3) {
    GST_LOG ("TMPlayer (time based) format detected");
    return GST_SUB_PARSE_FORMAT_TMPLAYER;
  }
  if (sscanf (match_str, "[%u][%u]", &n1, &n2) == 2) {
    GST_LOG ("MPL2 (time based) format detected");
    return GST_SUB_PARSE_FORMAT_MPL2;
  }
  if (strstr (match_str, "[INFORMATION]") != NULL) {
    GST_LOG ("SubViewer (time based) format detected");
    return GST_SUB_PARSE_FORMAT_SUBVIEWER;
  }
  if (strstr (match_str, "{QTtext}") != NULL) {
    GST_LOG ("QTtext (time based) format detected");
    return GST_SUB_PARSE_FORMAT_QTTEXT;
  }

  GST_DEBUG ("no subtitle format detected");
  return GST_SUB_PARSE_FORMAT_UNKNOWN;
}

static GstCaps *
gst_sub_parse_format_autodetect (GstSubParse * self)
{
  gchar *data;
  GstSubParseFormat format;

  if (strlen (self->textbuf->str) < 30) {
    GST_DEBUG ("File too small to be a subtitles file");
    return NULL;
  }

  data = g_strndup (self->textbuf->str, 35);
  format = gst_sub_parse_data_format_autodetect (data);
  g_free (data);

  self->parser_type = format;
  self->subtitle_codec = gst_sub_parse_get_format_description (format);
  parser_state_init (&self->state);

  switch (format) {
    case GST_SUB_PARSE_FORMAT_MDVDSUB:
      self->parse_line = parse_mdvdsub;
      return gst_caps_new_simple ("text/x-raw",
          "format", G_TYPE_STRING, "pango-markup", NULL);
    case GST_SUB_PARSE_FORMAT_SUBRIP:
      self->parse_line = parse_subrip;
      return gst_caps_new_simple ("text/x-raw",
          "format", G_TYPE_STRING, "pango-markup", NULL);
    case GST_SUB_PARSE_FORMAT_MPSUB:
      self->parse_line = parse_mpsub;
      return gst_caps_new_simple ("text/x-raw",
          "format", G_TYPE_STRING, "utf8", NULL);
    case GST_SUB_PARSE_FORMAT_SAMI:
      self->parse_line = parse_sami;
      sami_context_init (&self->state);
      return gst_caps_new_simple ("text/x-raw",
          "format", G_TYPE_STRING, "pango-markup", NULL);
    case GST_SUB_PARSE_FORMAT_TMPLAYER:
      self->parse_line = parse_tmplayer;
      self->state.max_duration = 5 * GST_SECOND;
      return gst_caps_new_simple ("text/x-raw",
          "format", G_TYPE_STRING, "utf8", NULL);
    case GST_SUB_PARSE_FORMAT_MPL2:
      self->parse_line = parse_mpl2;
      return gst_caps_new_simple ("text/x-raw",
          "format", G_TYPE_STRING, "pango-markup", NULL);
    case GST_SUB_PARSE_FORMAT_DKS:
      self->parse_line = parse_dks;
      return gst_caps_new_simple ("text/x-raw",
          "format", G_TYPE_STRING, "utf8", NULL);
    case GST_SUB_PARSE_FORMAT_SUBVIEWER:
      self->parse_line = parse_subviewer;
      return gst_caps_new_simple ("text/x-raw",
          "format", G_TYPE_STRING, "utf8", NULL);
    case GST_SUB_PARSE_FORMAT_QTTEXT:
      self->parse_line = parse_qttext;
      qttext_context_init (&self->state);
      return gst_caps_new_simple ("text/x-raw",
          "format", G_TYPE_STRING, "pango-markup", NULL);
    case GST_SUB_PARSE_FORMAT_UNKNOWN:
    default:
      GST_DEBUG ("no subtitle format detected");
      GST_ELEMENT_ERROR (self, STREAM, WRONG_TYPE,
          ("The input is not a valid/supported subtitle file"), (NULL));
      return NULL;
  }
}

static void
feed_textbuf (GstSubParse * self, GstBuffer * buf)
{
  gboolean discont;
  gsize consumed;
  gchar *input = NULL;
  const guint8 *data;
  gsize avail;

  discont = GST_BUFFER_IS_DISCONT (buf);

  if (GST_BUFFER_OFFSET_IS_VALID (buf) &&
      GST_BUFFER_OFFSET (buf) != self->offset) {
    self->offset = GST_BUFFER_OFFSET (buf);
    discont = TRUE;
  }

  if (discont) {
    GST_INFO ("discontinuity");
    /* flush the parser state */
    parser_state_init (&self->state);
    g_string_truncate (self->textbuf, 0);
    gst_adapter_clear (self->adapter);
    if (self->parser_type == GST_SUB_PARSE_FORMAT_SAMI)
      sami_context_reset (&self->state);
    /* we could set a flag to make sure that the next buffer we push out also
     * has the DISCONT flag set, but there's no point really given that it's
     * subtitles which are discontinuous by nature. */
  }

  self->offset += gst_buffer_get_size (buf);

  gst_adapter_push (self->adapter, buf);

  avail = gst_adapter_available (self->adapter);
  data = gst_adapter_map (self->adapter, avail);
  input = convert_encoding (self, (const gchar *) data, avail, &consumed);

  if (input && consumed > 0) {
    self->textbuf = g_string_append (self->textbuf, input);
    gst_adapter_unmap (self->adapter);
    gst_adapter_flush (self->adapter, consumed);
  } else {
    gst_adapter_unmap (self->adapter);
  }

  g_free (input);
}

static GstFlowReturn
handle_buffer (GstSubParse * self, GstBuffer * buf)
{
  GstFlowReturn ret = GST_FLOW_OK;
  GstCaps *caps = NULL;
  gchar *line, *subtitle;
  gboolean need_tags = FALSE;

  if (self->first_buffer) {
    GstMapInfo map;

    gst_buffer_map (buf, &map, GST_MAP_READ);
    self->detected_encoding = detect_encoding ((gchar *) map.data, map.size);
    gst_buffer_unmap (buf, &map);
    self->first_buffer = FALSE;
    self->state.fps_n = self->fps_n;
    self->state.fps_d = self->fps_d;
  }

  feed_textbuf (self, buf);

  /* make sure we know the format */
  if (G_UNLIKELY (self->parser_type == GST_SUB_PARSE_FORMAT_UNKNOWN)) {
    if (!(caps = gst_sub_parse_format_autodetect (self))) {

      GST_INFO("handle_buffer gst_sub_parse_format_autodetect failed EOS");
      GST_ELEMENT_WARNING (self, STREAM, SUBTILE, ("subparse:sub failed"), ("subparse:sub failed"));
      return GST_FLOW_EOS;
    }
    if (!gst_pad_set_caps (self->srcpad, caps)) {
      gst_caps_unref (caps);

      GST_INFO("handle_buffer gst_pad_set_caps failed EOS");
      return GST_FLOW_EOS;
    }
    gst_caps_unref (caps);
    need_tags = TRUE;
  }

  /* Push newsegment if needed */
  if (self->need_segment) {
    GST_INFO_OBJECT (self, "begin pushing newsegment event with %" GST_SEGMENT_FORMAT,
        &self->segment);

    gst_pad_push_event (self->srcpad, gst_event_new_segment (&self->segment));
    GST_INFO_OBJECT (self, "end pushing newsegment event with %" GST_SEGMENT_FORMAT,
        &self->segment);
    self->need_segment = FALSE;
  }

  if (need_tags) {
    /* push tags */
    if (self->subtitle_codec != NULL) {
      GstTagList *tags;

      tags = gst_tag_list_new (GST_TAG_SUBTITLE_CODEC, self->subtitle_codec,
          NULL);
      gst_pad_push_event (self->srcpad, gst_event_new_tag (tags));
    }
  }

  while (!self->flushing && (line = get_next_line (self))) {
    guint offset = 0;

    /* Set segment on our parser state machine */
    self->state.segment = &self->segment;
    /* Now parse the line, out of segment lines will just return NULL */
    GST_LOG_OBJECT (self, "Parsing line '%s'", line + offset);
    subtitle = self->parse_line (&self->state, line + offset);
    g_free (line);

    if (subtitle) {
      guint subtitle_len = strlen (subtitle);

      /* +1 for terminating NUL character */
      buf = gst_buffer_new_and_alloc (subtitle_len + 1);

      /* copy terminating NUL character as well */
      gst_buffer_fill (buf, 0, subtitle, subtitle_len + 1);
      gst_buffer_set_size (buf, subtitle_len);

      GST_BUFFER_TIMESTAMP (buf) = self->state.start_time;
      GST_BUFFER_DURATION (buf) = self->state.duration;

      /* in some cases (e.g. tmplayer) we can only determine the duration
       * of a text chunk from the timestamp of the next text chunk; in those
       * cases, we probably want to limit the duration to something
       * reasonable, so we don't end up showing some text for e.g. 40 seconds
       * just because nothing else is being said during that time */
      if (self->state.max_duration > 0 && GST_BUFFER_DURATION_IS_VALID (buf)) {
        if (GST_BUFFER_DURATION (buf) > self->state.max_duration)
          GST_BUFFER_DURATION (buf) = self->state.max_duration;
      }

      self->segment.position = self->state.start_time;

      GST_DEBUG_OBJECT (self, "Sending text '%s', %" GST_TIME_FORMAT " + %"
          GST_TIME_FORMAT, subtitle, GST_TIME_ARGS (self->state.start_time),
          GST_TIME_ARGS (self->state.duration));

      /* BEGIN: Added for DTS2015072205123 by liurongliang(l00180035), 2015/7/26 */
      /*
        if  while (!self->flushing && (line = get_next_line (self))) we do seek.
        we should here to check to send segment
      */
      if (self->need_segment) {
        GST_INFO_OBJECT (self, "begin pushing newsegment event with %" GST_SEGMENT_FORMAT,
            &self->segment);

        gst_pad_push_event (self->srcpad, gst_event_new_segment (&self->segment));
        GST_INFO_OBJECT (self, "end pushing newsegment event with %" GST_SEGMENT_FORMAT,
            &self->segment);
        self->need_segment = FALSE;
      }
      /* END:   Added for DTS2015072205123 by liurongliang(l00180035), 2015/7/26 */

      ret = gst_pad_push (self->srcpad, buf);

      /* move this forward (the tmplayer parser needs this) */
      if (self->state.duration != GST_CLOCK_TIME_NONE)
        self->state.start_time += self->state.duration;

      g_free (subtitle);
      subtitle = NULL;

      if (ret != GST_FLOW_OK) {
        GST_DEBUG_OBJECT (self, "flow: %s", gst_flow_get_name (ret));
        break;
      }
    }
  }

  return ret;
}

static GstFlowReturn
gst_sub_parse_chain (GstPad * sinkpad, GstObject * parent, GstBuffer * buf)
{
  GstFlowReturn ret;
  GstSubParse *self;

  self = GST_SUBPARSE (parent);

  ret = handle_buffer (self, buf);

  return ret;
}

static gboolean
gst_sub_parse_sink_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
  GstSubParse *self = GST_SUBPARSE (parent);
  gboolean ret = FALSE;

  GST_INFO ("Handling %s event", GST_EVENT_TYPE_NAME (event));

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_EOS:{
      /* Make sure the last subrip chunk is pushed out even
       * if the file does not have an empty line at the end */
      if (self->parser_type == GST_SUB_PARSE_FORMAT_SUBRIP ||
          self->parser_type == GST_SUB_PARSE_FORMAT_TMPLAYER ||
          self->parser_type == GST_SUB_PARSE_FORMAT_MPL2 ||
          self->parser_type == GST_SUB_PARSE_FORMAT_QTTEXT) {
        gchar term_chars[] = { '\n', '\n', '\0' };
        GstBuffer *buf = gst_buffer_new_and_alloc (2 + 1);

        GST_DEBUG ("EOS. Pushing remaining text (if any)");
        gst_buffer_fill (buf, 0, term_chars, 3);
        gst_buffer_set_size (buf, 2);

        GST_BUFFER_OFFSET (buf) = self->offset;
        gst_sub_parse_chain (pad, parent, buf);
      }
      ret = gst_pad_event_default (pad, parent, event);
      break;
    }
    case GST_EVENT_SEGMENT:
    {
      const GstSegment *s;
      gst_event_parse_segment (event, &s);
      if (s->format == GST_FORMAT_TIME)
        gst_event_copy_segment (event, &self->segment);
      GST_DEBUG_OBJECT (self, "newsegment (%s)",
          gst_format_get_name (self->segment.format));

      /* if not time format, we'll either start with a 0 timestamp anyway or
       * it's following a seek in which case we'll have saved the requested
       * seek segment and don't want to overwrite it (remember that on a seek
       * we always just seek back to the start in BYTES format and just throw
       * away all text that's before the requested position; if the subtitles
       * come from an upstream demuxer, it won't be able to handle our BYTES
       * seek request and instead send us a newsegment from the seek request
       * it received via its video pads instead, so all is fine then too) */
      ret = TRUE;
      gst_event_unref (event);
      break;
    }
    case GST_EVENT_FLUSH_START:
    {
      self->flushing = TRUE;

      ret = gst_pad_event_default (pad, parent, event);
      break;
    }
    case GST_EVENT_FLUSH_STOP:
    {
      self->flushing = FALSE;

      ret = gst_pad_event_default (pad, parent, event);
      break;
    }
    default:
      ret = gst_pad_event_default (pad, parent, event);
      break;
  }

  return ret;
}


static GstStateChangeReturn
gst_sub_parse_change_state (GstElement * element, GstStateChange transition)
{
  GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;
  GstSubParse *self = GST_SUBPARSE (element);

  switch (transition) {
    case GST_STATE_CHANGE_READY_TO_PAUSED:
      /* format detection will init the parser state */
      self->offset = 0;
      self->parser_type = GST_SUB_PARSE_FORMAT_UNKNOWN;
      self->valid_utf8 = TRUE;
      self->first_buffer = TRUE;
      g_free (self->detected_encoding);
      self->detected_encoding = NULL;
      g_string_truncate (self->textbuf, 0);
      gst_adapter_clear (self->adapter);
      break;
    default:
      break;
  }

  ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);
  if (ret == GST_STATE_CHANGE_FAILURE)
    return ret;

  switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      parser_state_dispose (self, &self->state);
      self->parser_type = GST_SUB_PARSE_FORMAT_UNKNOWN;
      break;
    default:
      break;
  }

  return ret;
}

/*
 * Typefind support.
 */

/* FIXME 0.11: these caps are ugly, use app/x-subtitle + type field or so;
 * also, give different  subtitle formats really different types */
static GstStaticCaps mpl2_caps =
GST_STATIC_CAPS ("application/x-subtitle-mpl2");
#define SUB_CAPS (gst_static_caps_get (&sub_caps))

static GstStaticCaps tmp_caps =
GST_STATIC_CAPS ("application/x-subtitle-tmplayer");
#define TMP_CAPS (gst_static_caps_get (&tmp_caps))

static GstStaticCaps sub_caps = GST_STATIC_CAPS ("application/x-subtitle");
#define MPL2_CAPS (gst_static_caps_get (&mpl2_caps))

static GstStaticCaps smi_caps = GST_STATIC_CAPS ("application/x-subtitle-sami");
#define SAMI_CAPS (gst_static_caps_get (&smi_caps))

static GstStaticCaps dks_caps = GST_STATIC_CAPS ("application/x-subtitle-dks");
#define DKS_CAPS (gst_static_caps_get (&dks_caps))

static GstStaticCaps qttext_caps =
GST_STATIC_CAPS ("application/x-subtitle-qttext");
#define QTTEXT_CAPS (gst_static_caps_get (&qttext_caps))

static void
gst_subparse_type_find (GstTypeFind * tf, gpointer private)
{
  GstSubParseFormat format;
  const guint8 *data;
  GstCaps *caps;
  gchar *str;
  gchar *encoding = NULL;
  const gchar *end;
  gint tf_len = 0;

  tf_len = gst_type_find_get_length (tf);
  GST_WARNING("tf_len=%d", tf_len);
  tf_len = tf_len >= TYPEFIND_SIZE ? TYPEFIND_SIZE : tf_len;

  if(tf_len <= 8){
    GST_ERROR("tf_len < 8 ignore");
    return;
  }

  if (!(data = gst_type_find_peek (tf, 0, tf_len)))
    return;

  /* make sure string passed to _autodetect() is NUL-terminated */
  str = g_malloc0 (tf_len);
  memcpy (str, data, tf_len-1);

  if ((encoding = detect_encoding (str, tf_len-1)) != NULL) {
    gchar *converted_str;
    GError *err = NULL;
    gsize tmp;

    converted_str = gst_convert_to_utf8 (str, tf_len-1, encoding, &tmp, &err);
    if (converted_str == NULL) {
      GST_DEBUG ("Encoding '%s' detected but conversion failed: %s", encoding,
          err->message);
      g_error_free (err);
    } else {
      g_free (str);
      str = converted_str;
    }
    g_free (encoding);
  }

  /* Check if at least the first 120 chars are valid UTF8,
   * otherwise convert as always */
  if (!g_utf8_validate (str, tf_len-1, &end) && (end - str) < tf_len-9) {
    gchar *converted_str;
    gsize tmp;
    const gchar *enc;

    enc = g_getenv ("GST_SUBTITLE_ENCODING");
    if (enc == NULL || *enc == '\0') {
      /* if local encoding is UTF-8 and no encoding specified
       * via the environment variable, assume ISO-8859-15 */
      if (g_get_charset (&enc)) {
        enc = "ISO-8859-15";
      }
    }
    converted_str = gst_convert_to_utf8 (str, tf_len-1, enc, &tmp, NULL);
    if (converted_str != NULL) {
      g_free (str);
      str = converted_str;
    }
  }

  format = gst_sub_parse_data_format_autodetect (str);
  g_free (str);

  switch (format) {
    case GST_SUB_PARSE_FORMAT_MDVDSUB:
      GST_DEBUG ("MicroDVD format detected");
      caps = SUB_CAPS;
      break;
    case GST_SUB_PARSE_FORMAT_SUBRIP:
      GST_DEBUG ("SubRip format detected");
      caps = SUB_CAPS;
      break;
    case GST_SUB_PARSE_FORMAT_MPSUB:
      GST_DEBUG ("MPSub format detected");
      caps = SUB_CAPS;
      break;
    case GST_SUB_PARSE_FORMAT_SAMI:
      GST_DEBUG ("SAMI (time-based) format detected");
      caps = SAMI_CAPS;
      break;
    case GST_SUB_PARSE_FORMAT_TMPLAYER:
      GST_DEBUG ("TMPlayer (time based) format detected");
      caps = TMP_CAPS;
      break;
      /* FIXME: our MPL2 typefinding is not really good enough to warrant
       * returning a high probability (however, since we registered our
       * typefinder here with a rank of MARGINAL we should pretty much only
       * be called if most other typefinders have already run */
    case GST_SUB_PARSE_FORMAT_MPL2:
      GST_DEBUG ("MPL2 (time based) format detected");
      caps = MPL2_CAPS;
      break;
    case GST_SUB_PARSE_FORMAT_SUBVIEWER:
      GST_DEBUG ("SubViewer format detected");
      caps = SUB_CAPS;
      break;
    case GST_SUB_PARSE_FORMAT_DKS:
      GST_DEBUG ("DKS format detected");
      caps = DKS_CAPS;
      break;
    case GST_SUB_PARSE_FORMAT_QTTEXT:
      GST_DEBUG ("QTtext format detected");
      caps = QTTEXT_CAPS;
      break;
    default:
    case GST_SUB_PARSE_FORMAT_UNKNOWN:
      GST_DEBUG ("no subtitle format detected");
      return;
  }
  GST_INFO("gst_subparse_type_find,format=%d", format);
  /* if we're here, it's ok */
  gst_type_find_suggest (tf, GST_TYPE_FIND_MAXIMUM, caps);
}

#define NUL_TERMINATOR_LENGTH 4

/**
 * gst_convert_with_iconv:
 * @from_codeset:  the string encode type
 * @str:           the string to convert
 * @len:           the length of the string, or -1 if the string is
 *                 nul-terminated<footnoteref linkend="nul-unsafe"/>.
 * @converter:     conversion descriptor from g_iconv_open()
 * @bytes_read:    location to store the number of bytes in the
 *                 input string that were successfully converted, or %NULL.
 *                 Even if the conversion was successful, this may be
 *                 less than @len if there were partial characters
 *                 at the end of the input. If the error
 *                 #G_CONVERT_ERROR_ILLEGAL_SEQUENCE occurs, the value
 *                 stored will the byte offset after the last valid
 *                 input sequence.
 * @bytes_written: the number of bytes stored in the output buffer (not
 *                 including the terminating nul).
 * @error:         location to store the error occurring, or %NULL to ignore
 *                 errors. Any of the errors in #GConvertError may occur.
 *
 * Converts a string from one character set to another.
 *
 * Note that you should use g_iconv() for streaming
 * conversions<footnote id="streaming-state">
 *  <para>
 * Despite the fact that @byes_read can return information about partial
 * characters, the <literal>g_convert_...</literal> functions
 * are not generally suitable for streaming. If the underlying converter
 * being used maintains internal state, then this won't be preserved
 * across successive calls to g_convert(), g_convert_with_iconv() or
 * g_convert_with_fallback(). (An example of this is the GNU C converter
 * for CP1255 which does not emit a base character until it knows that
 * the next character is not a mark that could combine with the base
 * character.)
 *  </para>
 * </footnote>.
 * if from_codeset is utf8 or unicode, it will skip the unkown character.
 *
 * Return value: If the conversion was successful, a newly allocated
 *               nul-terminated string, which must be freed with
 *               g_free(). Otherwise %NULL and @error will be set.
 **/
static gchar*
gst_convert_with_iconv (const gchar *from_codeset,
          const gchar *str,
          gssize       len,
          GIConv       converter,
          gsize       *bytes_read,
          gsize       *bytes_written,
          GError     **error)
{
  gchar *dest;
  gchar *outp;
  const gchar *p;
  gsize inbytes_remaining;
  gsize outbytes_remaining;
  gsize err;
  gsize outbuf_size;
  gboolean have_error = FALSE;
  gboolean done = FALSE;
  gboolean reset = FALSE;

  g_return_val_if_fail (converter != (GIConv) -1, NULL);

  if (len < 0){
    len = strlen (str);
  }

  p = str;
  inbytes_remaining = len;
  outbuf_size = len + NUL_TERMINATOR_LENGTH;

  outbytes_remaining = outbuf_size - NUL_TERMINATOR_LENGTH;
  dest = g_malloc (outbuf_size);

  //for ICSL
  g_return_val_if_fail (NULL != dest, NULL);
  outp = dest;

  while (!done && !have_error){
    if (reset)
      err = g_iconv (converter, NULL, &inbytes_remaining, &outp, &outbytes_remaining);
    else
      err = g_iconv (converter, (char **)&p, &inbytes_remaining, &outp, &outbytes_remaining);

    if (err == (gsize) -1){
      switch (errno){
        case EINVAL:
          /* Incomplete text, do not report an error */
          done = TRUE;
          break;
        case E2BIG:{
          gsize used = outp - dest;

          outbuf_size *= 2;
          dest = g_realloc (dest, outbuf_size);
          if(!dest){
            g_set_error_literal (error, G_CONVERT_ERROR, G_CONVERT_ERROR_ILLEGAL_SEQUENCE,
            ("alloc failed"));
            have_error = TRUE;
            break;
          }
          outp = dest + used;
          outbytes_remaining = outbuf_size - used - NUL_TERMINATOR_LENGTH;

          break;
        }
        case EILSEQ:{
          gint err_offset = 0;

          if(!from_codeset){
            err_offset = 0;
          }else if(!g_strcmp0(from_codeset, "UTF-8")){
            err_offset = 1;
          }else if(!g_strcmp0(from_codeset, "UCS-2LE")
                || !g_strcmp0(from_codeset, "UCS-2BE")){
            err_offset = 2;
          }else{
            ;//do nothing
          }

          if(err_offset <= 0 || err_offset > 2){
            g_set_error_literal (error, G_CONVERT_ERROR, G_CONVERT_ERROR_ILLEGAL_SEQUENCE,
            ("Invalid byte sequence in conversion input"));
            have_error = TRUE;
          }else{
            gsize used = outp - dest;

            outp = dest + used;
            outbytes_remaining = outbuf_size - used - NUL_TERMINATOR_LENGTH;
            p += err_offset;
            inbytes_remaining = len - (p - str);
          }

          break;
        }
        default:{
          int errsv = errno;

          g_set_error (error, G_CONVERT_ERROR, G_CONVERT_ERROR_FAILED,
            ("Error during conversion: %s"), g_strerror (errsv));
          have_error = TRUE;

          break;
        }
      }
    }else{
      if (!reset){
        /* call g_iconv with NULL inbuf to cleanup shift state */
        reset = TRUE;
        inbytes_remaining = 0;
      }else{
        done = TRUE;
      }
    }
  }

  memset (outp, 0, NUL_TERMINATOR_LENGTH);

  if (bytes_read){
    *bytes_read = p - str;
  }else{
    if ((p - str) != len){
      if (!have_error){
        g_set_error_literal (error, G_CONVERT_ERROR, G_CONVERT_ERROR_PARTIAL_INPUT,
          ("Partial character sequence at end of input"));
        have_error = TRUE;
      }
    }
  }

  if (bytes_written){
    *bytes_written = outp - dest;	/* Doesn't include '\0' */
  }

  if (have_error){
    g_free (dest);
    return NULL;
  }

  return dest;
}

static GIConv
gst_open_converter (const gchar *to_codeset,
  const gchar *from_codeset,
  GError     **error)
{
  GIConv cd;

  cd = g_iconv_open (to_codeset, from_codeset);

  if (cd == (GIConv) -1){
    /* Something went wrong.  */
    if (error){
      if (errno == EINVAL){
        g_set_error (error, G_CONVERT_ERROR, G_CONVERT_ERROR_NO_CONVERSION,
          ("Conversion from character set '%s' to '%s' is not supported"),
          from_codeset, to_codeset);
      }else{
        g_set_error (error, G_CONVERT_ERROR, G_CONVERT_ERROR_FAILED,
          ("Could not open converter from '%s' to '%s'"),
          from_codeset, to_codeset);
      }
    }
  }

  return cd;
}

/**
 * gst_convert:
 * @str:           the string to convert
 * @len:           the length of the string, or -1 if the string is
 *                 nul-terminated<footnote id="nul-unsafe">
                     <para>
                       Note that some encodings may allow nul bytes to
                       occur inside strings. In that case, using -1 for
                       the @len parameter is unsafe.
                     </para>
                   </footnote>.
 * @to_codeset:    name of character set into which to convert @str
 * @from_codeset:  character set of @str.
 * @bytes_read: (out):   location to store the number of bytes in the
 *                 input string that were successfully converted, or %NULL.
 *                 Even if the conversion was successful, this may be
 *                 less than @len if there were partial characters
 *                 at the end of the input. If the error
 *                 #G_CONVERT_ERROR_ILLEGAL_SEQUENCE occurs, the value
 *                 stored will the byte offset after the last valid
 *                 input sequence.
 * @bytes_written: (out): the number of bytes stored in the output buffer (not
 *                 including the terminating nul).
 * @error:         location to store the error occurring, or %NULL to ignore
 *                 errors. Any of the errors in #GConvertError may occur.
 *
 * Converts a string from one character set to another.
 *
 * Note that you should use g_iconv() for streaming
 * conversions<footnoteref linkend="streaming-state"/>.
 *
 * Return value: If the conversion was successful, a newly allocated
 *               nul-terminated string, which must be freed with
 *               g_free(). Otherwise %NULL and @error will be set.
 **/
static gchar*
gst_convert (const gchar *str,
        gssize       len,
        const gchar *to_codeset,
        const gchar *from_codeset,
        gsize       *bytes_read,
        gsize       *bytes_written,
        GError     **error)
{
  gchar *res;
  GIConv cd;

  g_return_val_if_fail (str != NULL, NULL);
  g_return_val_if_fail (to_codeset != NULL, NULL);
  g_return_val_if_fail (from_codeset != NULL, NULL);

  cd = gst_open_converter (to_codeset, from_codeset, error);

  if (cd == (GIConv) -1){
    if (bytes_read){
      *bytes_read = 0;
    }
    if (bytes_written){
      *bytes_written = 0;
    }
    return NULL;
  }

  res = gst_convert_with_iconv (from_codeset, str, len, cd, bytes_read, bytes_written,
    error);

  g_iconv_close (cd);

  return res;
}

/**
 * gst_convert_with_fallback:
 * @str:          the string to convert
 * @len:          the length of the string, or -1 if the string is
 *                nul-terminated<footnoteref linkend="nul-unsafe"/>.
 * @to_codeset:   name of character set into which to convert @str
 * @from_codeset: character set of @str.
 * @fallback:     UTF-8 string to use in place of character not
 *                present in the target encoding. (The string must be
 *                representable in the target encoding).
                  If %NULL, characters not in the target encoding will
                  be represented as Unicode escapes \uxxxx or \Uxxxxyyyy.
 * @bytes_read:   location to store the number of bytes in the
 *                input string that were successfully converted, or %NULL.
 *                Even if the conversion was successful, this may be
 *                less than @len if there were partial characters
 *                at the end of the input.
 * @bytes_written: the number of bytes stored in the output buffer (not
 *                including the terminating nul).
 * @error:        location to store the error occurring, or %NULL to ignore
 *                errors. Any of the errors in #GConvertError may occur.
 *
 * Converts a string from one character set to another, possibly
 * including fallback sequences for characters not representable
 * in the output. Note that it is not guaranteed that the specification
 * for the fallback sequences in @fallback will be honored. Some
 * systems may do an approximate conversion from @from_codeset
 * to @to_codeset in their iconv() functions,
 * in which case GLib will simply return that approximate conversion.
 *
 * Note that you should use g_iconv() for streaming
 * conversions<footnoteref linkend="streaming-state"/>.
 *
 * Return value: If the conversion was successful, a newly allocated
 *               nul-terminated string, which must be freed with
 *               g_free(). Otherwise %NULL and @error will be set.
 **/
static gchar*
gst_convert_with_fallback (const gchar *str,
        gssize       len,
        const gchar *to_codeset,
        const gchar *from_codeset,
        const gchar *fallback,
        gsize       *bytes_read,
        gsize       *bytes_written,
        GError     **error)
{
  gchar *utf8;
  gchar *dest;
  gchar *outp;
  const gchar *insert_str = NULL;
  const gchar *p;
  gsize inbytes_remaining;
  const gchar *save_p = NULL;
  gsize save_inbytes = 0;
  gsize outbytes_remaining;
  gsize err;
  GIConv cd;
  gsize outbuf_size;
  gboolean have_error = FALSE;
  gboolean done = FALSE;

  GError *local_error = NULL;

  g_return_val_if_fail (str != NULL, NULL);
  g_return_val_if_fail (to_codeset != NULL, NULL);
  g_return_val_if_fail (from_codeset != NULL, NULL);

  if (len < 0){
    len = strlen (str);
  }

  /* Try an exact conversion; we only proceed if this fails
  * due to an illegal sequence in the input string.
  */
  dest = gst_convert (str, len, to_codeset, from_codeset,
  bytes_read, bytes_written, &local_error);
  if (!local_error){
    return dest;
  }

  if (!g_error_matches (local_error, G_CONVERT_ERROR, G_CONVERT_ERROR_ILLEGAL_SEQUENCE)){
    GST_ERROR("gst_convert_with_fallback failed !");
    g_propagate_error (error, local_error);
    return NULL;
  }

  g_error_free (local_error);
  local_error = NULL;

  /* No go; to proceed, we need a converter from "UTF-8" to
  * to_codeset, and the string as UTF-8.
  */
  cd = gst_open_converter (to_codeset, "UTF-8", error);
  if (cd == (GIConv) -1){
    if (bytes_read){
      *bytes_read = 0;
    }

    if (bytes_written){
      *bytes_written = 0;
    }

    GST_ERROR("gst_convert_with_fallback gst_open_converter failed!");

    return NULL;
  }

  utf8 = gst_convert (str, len, "UTF-8", from_codeset, bytes_read, &inbytes_remaining,
    error);
  if (!utf8){
    g_iconv_close (cd);
    if (bytes_written){
      *bytes_written = 0;
    }
    GST_ERROR("gst_convert_with_fallback g_convert failed !UTF-8");

    return NULL;
  }

  /* Now the heart of the code. We loop through the UTF-8 string, and
  * whenever we hit an offending character, we form fallback, convert
  * the fallback to the target codeset, and then go back to
  * converting the original string after finishing with the fallback.
  *
  * The variables save_p and save_inbytes store the input state
  * for the original string while we are converting the fallback
  */
  p = utf8;

  outbuf_size = len + NUL_TERMINATOR_LENGTH;
  outbytes_remaining = outbuf_size - NUL_TERMINATOR_LENGTH;
  dest = g_malloc (outbuf_size);

  //for ICSL
  g_return_val_if_fail (NULL != dest, NULL);
  outp = dest;

  while (!done && !have_error){
    gsize inbytes_tmp = inbytes_remaining;
    err = g_iconv (cd, (char **)&p, &inbytes_tmp, &outp, &outbytes_remaining);
    inbytes_remaining = inbytes_tmp;
    if (err == (gsize) -1){
      switch (errno){
        case EINVAL:
          g_assert_not_reached();
          break;
        case E2BIG:{
          gsize used = outp - dest;

          outbuf_size *= 2;
          dest = g_realloc (dest, outbuf_size);
          if(!dest){
            g_set_error_literal (error, G_CONVERT_ERROR, G_CONVERT_ERROR_ILLEGAL_SEQUENCE,
            ("alloc failed"));
            have_error = TRUE;
            break;
          }
          outp = dest + used;
          outbytes_remaining = outbuf_size - used - NUL_TERMINATOR_LENGTH;

          break;
        }
        case EILSEQ:
          if (save_p){
            /* Error converting fallback string - fatal
            */
            g_set_error (error, G_CONVERT_ERROR, G_CONVERT_ERROR_ILLEGAL_SEQUENCE,
              ("Cannot convert fallback '%s' to codeset '%s'"), insert_str, to_codeset);
            have_error = TRUE;
            break;
          }else if (p){
            if (!fallback){
              gunichar ch = g_utf8_get_char (p);
              insert_str = g_strdup_printf (ch < 0x10000 ? "\\u%04x" : "\\U%08x", ch);
            }else{
              insert_str = fallback;
            }
            save_p = g_utf8_next_char (p);
            save_inbytes = inbytes_remaining - (save_p - p);
            p = insert_str;
            inbytes_remaining = strlen (p);
            break;
        }
        /* fall thru if p is NULL */
        default:{
          {
            int errsv = errno;

            g_set_error (error, G_CONVERT_ERROR, G_CONVERT_ERROR_FAILED,
              ("Error during conversion: %s"), g_strerror (errsv));
          }
          have_error = TRUE;
          break;
        }
      }
    }else{
      if (save_p){
        if (!fallback){
          g_free ((gchar *)insert_str);
        }
        p = save_p;
        inbytes_remaining = save_inbytes;
        save_p = NULL;
      }else if (p){
        /* call g_iconv with NULL inbuf to cleanup shift state */
        p = NULL;
        inbytes_remaining = 0;
      }else{
        done = TRUE;
      }
    }
  }

  /* Cleanup
  */
  memset (outp, 0, NUL_TERMINATOR_LENGTH);

  g_iconv_close (cd);

  if (bytes_written){
    *bytes_written = outp - dest;	/* Doesn't include '\0' */
  }

  g_free (utf8);

  if (have_error){
    if (save_p && !fallback){
      g_free ((gchar *)insert_str);
    }
    g_free (dest);
    return NULL;
  }

  return dest;
}

static gboolean
plugin_init (GstPlugin * plugin)
{
  GST_DEBUG_CATEGORY_INIT (sub_parse_debug, "subparse", 0, ".sub parser");

  //here will not support multithead.
  /* BEGIN: Added for DTS2015072705793 by liurongliang(l00180035), 2015/7/27 */
#ifdef __USE_HISI_CHAR_DET__ // y00342710
  if(!g_sbinit_mutex){
    GST_WARNING("init g_shisi_char_dec_mutex");
    g_mutex_init(&g_shisi_char_dec_mutex);
    g_sbinit_mutex = TRUE;
  }
#endif
  /* END:   Added for DTS2015072705793 by liurongliang(l00180035), 2015/7/27 */

  if (!gst_type_find_register (plugin, "subparse_typefind", GST_RANK_MARGINAL,
          gst_subparse_type_find, "srt,sub,mpsub,mdvd,smi,txt,dks", SUB_CAPS,
          NULL, NULL))
    return FALSE;

  if (!gst_element_register (plugin, "subparse",
          GST_RANK_PRIMARY, GST_TYPE_SUBPARSE)/* ||
      !gst_element_register (plugin, "ssaparse",
          GST_RANK_PRIMARY, GST_TYPE_SSA_PARSE)*/) {
    return FALSE;
  }

  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    subparse,
    "Subtitle parsing",
    plugin_init, VERSION, "LGPL", GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
