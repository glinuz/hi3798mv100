/*lint -e717*/  //do ... while(0)
/*lint -e774*/  //always FALSE

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <gst/gst.h>

#include "gstfragmented.h"
#include "gsthlsdemux.h"
#include "gsthlssink.h"

GST_DEBUG_CATEGORY (fragmented_debug);

static GstStaticCaps hls_caps = GST_STATIC_CAPS ("application/x-hls");
#define HLS_CAPS (gst_static_caps_get(&hls_caps))

#define GST_SME_TYPE_FIND_SIZE 4096
#define GST_SME_TYPE_FIND_MIN_SIZE 256
#define GST_SME_TYPE_FIND_MAX_SIZE (2<<20)

#define M3U8_BUFFER_SIZE 16
#define M3U8_INC_BUFFER {                                                \
  pos++;                                                                \
  if (pos == M3U8_BUFFER_SIZE) {                                         \
    pos = 0;                                                            \
    offset += M3U8_BUFFER_SIZE;                                          \
    data = gst_type_find_peek (tf, offset, M3U8_BUFFER_SIZE);            \
    if (data == NULL) return FALSE;                                     \
  } else {                                                              \
    data++;                                                             \
  }                                                                     \
}

static gboolean
m3u8_check_first_element (GstTypeFind * tf, const gchar * element, guint elen,
    gboolean strict, guint64 typefind_size)
{
  gboolean got_m3u8;
  const guint8 *data;
  guint64 offset = 0;
  guint64 pos = 0;

  data = gst_type_find_peek (tf, 0, M3U8_BUFFER_SIZE);
  if (!data)
    return FALSE;

  got_m3u8 = (memcmp (data, "#EXTM3U", 7) == 0);

  if (strict && !got_m3u8)
    return FALSE;

  while (data && (offset + pos) < typefind_size) {
    while (*data != '#' && (offset + pos) < typefind_size) {
      M3U8_INC_BUFFER;
    }

    M3U8_INC_BUFFER;
    if (!g_ascii_isalpha (*data)) {
      /* if not alphabetic, it's a PI or an element / attribute declaration
       * like <?xxx or <!xxx */
      M3U8_INC_BUFFER;
      continue;
    }

    /* the first normal element, check if it's the one asked for */
    data = gst_type_find_peek (tf, offset + pos, elen + 1);

    return (data && element && strncmp ((char *) data, element, elen) == 0);
  }

  return FALSE;
}

/* See http://tools.ietf.org/html/draft-pantos-http-live-streaming-05 */
static void
hls_type_find (GstTypeFind * tf, gpointer unused)
{
  guint64 length = 0;
  length = gst_type_find_get_length (tf);

  if (length == 0)
    length = GST_SME_TYPE_FIND_SIZE;
  else
    length = MIN(length, GST_SME_TYPE_FIND_MAX_SIZE);

  if (m3u8_check_first_element (tf, "EXTM3U", 6, FALSE, length)) {
    gst_type_find_suggest (tf, GST_TYPE_FIND_MAXIMUM, HLS_CAPS);
  }
}

static gboolean
fragmented_init (GstPlugin * plugin)
{
  GST_DEBUG_CATEGORY_INIT (fragmented_debug, "fragmented", 0, "fragmented");

  if (!gst_element_register (plugin, "hlsdemux", GST_RANK_PRIMARY,
          GST_TYPE_HLS_DEMUX) || FALSE)
    return FALSE;

  gst_type_find_register (plugin, "application/x-hls", GST_RANK_MARGINAL,
    hls_type_find, "m3u8", HLS_CAPS, NULL, NULL);

  if (!gst_hls_sink_plugin_init (plugin))
    return FALSE;

  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    fragmented,
    "Fragmented streaming plugins",
    fragmented_init, VERSION, "LGPL", PACKAGE_NAME, "http://www.gstreamer.org/")
