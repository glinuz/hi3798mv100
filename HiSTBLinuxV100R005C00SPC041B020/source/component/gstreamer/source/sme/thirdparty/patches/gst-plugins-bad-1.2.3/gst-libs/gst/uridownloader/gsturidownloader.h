/* GStreamer
 * Copyright (C) 2011 Andoni Morales Alastruey <ylatuya@gmail.com>
 *
 * gsturidownloader.h:
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
 * Youshould have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __GSTURI_DOWNLOADER_H__
#define __GSTURI_DOWNLOADER_H__

#include <glib-object.h>
#include <gst/gst.h>
#include "gstfragment.h"

G_BEGIN_DECLS

#define GST_TYPE_URI_DOWNLOADER (gst_uri_downloader_get_type())
#define GST_URI_DOWNLOADER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_URI_DOWNLOADER,GstUriDownloader))
#define GST_URI_DOWNLOADER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_URI_DOWNLOADER,GstUriDownloaderClass))
#define GST_IS_URI_DOWNLOADER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_URI_DOWNLOADER))
#define GST_IS_URI_DOWNLOADER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_URI_DOWNLOADER))

typedef struct _GstUriDownloader GstUriDownloader;
typedef struct _GstUriDownloaderPrivate GstUriDownloaderPrivate;
typedef struct _GstUriDownloaderClass GstUriDownloaderClass;

typedef void (*uridownloader_cb)(void *hander, GstFragment *download);

struct _GstUriDownloader
{
  GstObject parent;

  GstUriDownloaderPrivate *priv;
  guint timeout_val;
  guint pre_bandwidth;
  gchar * user_agent;
  gchar **cookies;
  gchar * referer;
};

struct _GstUriDownloaderClass
{
  GstObjectClass parent_class;

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GType gst_uri_downloader_get_type (void);

GstUriDownloader * gst_uri_downloader_new (void);
/* BEGIN: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
GstUriDownloader * gst_uri_downloader_new_with_parent (GstElement * parent);
/* END: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
GstFragment * gst_uri_downloader_fetch_uri (GstUriDownloader * downloader, const gchar * uri);
GstFragment * gst_uri_downloader_fetch_uri_with_range (GstUriDownloader * downloader, const gchar * uri, gint64 range_start, gint64 range_end);
void gst_uri_downloader_reset (GstUriDownloader *downloader);
void gst_uri_downloader_cancel (GstUriDownloader *downloader);
void gst_uri_downloader_free (GstUriDownloader *downloader);
void gst_wait_eos_event (GstUriDownloader * downloader);

/** BEGIN: Added for DTS2015042203653  by baihaili(b00311320), 2015/4/24 */
gboolean gst_uri_downloader_set_uri_for_first_fragment(
    GstUriDownloader * downloader,
    const gchar * uri,
    uridownloader_cb cb,
    void *hander);
gboolean gst_uri_downloader_set_uri_for_first_fragment_with_range(
    GstUriDownloader * downloader,
    const gchar * uri,
    uridownloader_cb cb,
    void * hander,
    gint64 range_start,
    gint64 range_end);
/** END: Added for DTS201504220365322  by baihaili(b00311320), 2015/4/24 */



/* Modified BEGIN: DTS2014081803527, zhoubing z00213950 2014/08/19*/
gboolean gst_uri_downloader_can_eos(GstUriDownloader * downloader);
/* Modified END: DTS2014081803527, zhoubing z00213950 2014/08/19*/

G_END_DECLS
#endif /* __GSTURIDOWNLOADER_H__ */
