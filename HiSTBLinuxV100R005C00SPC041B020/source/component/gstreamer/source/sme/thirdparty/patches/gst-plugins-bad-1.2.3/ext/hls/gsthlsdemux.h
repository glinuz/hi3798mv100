/* GStreamer
 * Copyright (C) 2010 Marc-Andre Lureau <marcandre.lureau@gmail.com>
 * Copyright (C) 2010 Andoni Morales Alastruey <ylatuya@gmail.com>
 *
 * gsthlsdemux.h:
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

#ifndef __GST_HLS_DEMUX_H__
#define __GST_HLS_DEMUX_H__

#include <gst/gst.h>
#include <gst/base/gstadapter.h>
#include <gst/base/gstdataqueue.h>
#include "m3u8.h"
#include "gstfragmented.h"
#include <gst/uridownloader/gsturidownloader.h>

G_BEGIN_DECLS
#define GST_TYPE_HLS_DEMUX \
  (gst_hls_demux_get_type())
#define GST_HLS_DEMUX(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_HLS_DEMUX,GstHLSDemux))
#define GST_HLS_DEMUX_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_HLS_DEMUX,GstHLSDemuxClass))
#define GST_IS_HLS_DEMUX(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_HLS_DEMUX))
#define GST_IS_HLS_DEMUX_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_HLS_DEMUX))
#define GST_HLS_DEMUX_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj),GST_TYPE_HLS_DEMUX,GstHLSDemuxClass))
#define GST_HLS_DOWNLOAD_RATE_WINDOW 3

typedef struct _GstHLSDemuxStream GstHLSDemuxStream;
typedef struct _GstHLSDemux GstHLSDemux;
typedef struct _GstHLSDemuxClass GstHLSDemuxClass;
typedef struct _GSTHLSDemuxBitrateInfo GSTHLSDemuxBitrateInfo;

struct _GSTHLSDemuxBitrateInfo
{
  guint64 *video_bitrate_list;
  guint video_bitrates_num;
  guint64 *audio_bitrate_list;
  guint audio_bitrate_num;
};

/* BEGIN: Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */
struct _GstHLSDemuxStream
{
  GstHLSDemux *parent;

  gboolean video;              /* video stream or not */

  GstPad *srcpad;

  gboolean have_group_id;
  guint group_id;

  GstCaps *input_caps;
  GstUriDownloader *downloader;
  GstDataQueue *queue;

  gboolean do_typefind;         /* Whether we need to typefind the next buffer */
  gboolean pad_switched;        /* Whether we need to switch pad before push data*/
  gboolean inited;              /* Whether we need to init stream */

  /* Streaming task */
  GstTask *stream_task;
  GRecMutex stream_lock;

  /* Updates task */
  GstTask *updates_task;
  GRecMutex updates_lock;
  GMutex updates_timed_lock;

  /* flags to quit */
  gboolean cancelled;
  /* BEGIN: Modified for SME-110 by wanghua(w00347020), 2015/8/7 */
  gboolean end_of_playlist;
  /* END: Modified for SME-110 by wanghua(w00347020), 2015/8/7 */

  /* Position in the stream */
  GstClockTime first_cache_pts;
  gboolean need_segment;
  guint64 last_timeout_time;

  gsize last_fragment_size;

  /* Per fragment download information */
  guint64 fragment_total_time;
  guint64 fragment_total_size;
  guint64 current_download_rate;
  guint64 latest_download_rate[GST_HLS_DOWNLOAD_RATE_WINDOW];
  gint download_rate_num;
  guint64 cur_segment_bitrate;
  guint64 last_notify_bitrate;
};
/* END:   Modified for DTS2015072200692 by lizimian(l00251625), 2015/7/25 */

/**
 * GstHLSDemux:
 *
 * Opaque #GstHLSDemux data structure.
 */
struct _GstHLSDemux
{
  GstElement parent;

  GstPad *sinkpad;

  GstBuffer *playlist;          /* buffer contains m3u8 file */
  GstM3U8Client *client;        /* M3U8 client */

  /* Properties */
  gfloat bitrate_limit;         /* limit of the available bitrate to use */
  /* BEGIN: Added for DTS2016042509776, 2016/4/27 */
  guint64 connection_speed;       /* Network connection speed in kbps (0 = unknown) */
  guint64 startup_bitrate;        /* manually select the bandwidth */
  /* END: Added for DTS2016042509776, 2016/4/27 */
  guint timeout_val;            /* timeout value for uri downloader */

  GSList *streams;
  /* BEGIN: Added for DTS2015092302576 by lizimian(l00251625), 2015/10/6 */
  GMutex streams_lock;
  /* END: Added for DTS2015092302576 by lizimian(l00251625), 2015/10/6 */

  /* BEGIN: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */
  /* header */
  gboolean need_header;
  GstFragment *header;
  /* END: Modified for DTS2015081807636 by wanghua(w00347020), 2015/8/20 */

  gboolean all_pads_added;

  gchar *user_agent;
  gchar **cookies;
  gchar *referer;
  gchar * next_frag_uri;
};

struct _GstHLSDemuxClass
{
  GstElementClass parent_class;
};

GType gst_hls_demux_get_type (void);

G_END_DECLS
#endif /* __GST_HLS_DEMUX_H__ */
