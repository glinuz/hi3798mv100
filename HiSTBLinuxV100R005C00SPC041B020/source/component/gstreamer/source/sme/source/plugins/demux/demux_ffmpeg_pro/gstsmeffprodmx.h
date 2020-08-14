/**************************************************************************//**

  Copyright (C), 2001-2016, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file	      -gstsmeffprodmx.h
 * @brief	      -sme demux element for ffmpeg protocol header file
 * @author
 * @date	      -2016/07/26
 * @version       -V1.00
 *
 * History:
 *
 * Number  : V1.00
 * Date      : 2016/7/26
 * Author    :
 * Desc      : Created file
 *
******************************************************************************/

#ifndef __GST_SME_DEMUX_FF_PRO_H__
#define __GST_SME_DEMUX_FF_PRO_H__

#define SME_FF_PRO_DUMP_ES_VIDEO
#define SME_FF_PRO_DUMP_ES_AUDIO

#include <stdio.h>
#include "gstsme.h"
#include "sme_demux_inf.h"
#include "gstsmepipe.h"

#if defined (SME_FF_PRO_DUMP_ES_VIDEO) || defined (SME_FF_PRO_DUMP_ES_AUDIO)
#include <gst/gst.h>
#include "sme_proc.h"
#endif

G_BEGIN_DECLS

#define GST_SME_DEMUX(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_SME_DEMUX, GstSmeDemuxFfPro))
#define GST_SME_DEMUX_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_SME_DEMUX, GstSmeDemuxFfPro))
#define GST_IS_SME_DEMUX(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SME_DEMUX))
#define GST_IS_SME_DEMUX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SME_DEMUX))

typedef struct _GstSmeStream GstSmeStream;
typedef struct _GstSmeDemuxFfPro GstSmeDemuxFfPro;
typedef struct _GstSmeDemuxFfProClass GstSmeDemuxFfProClass;

struct _GstSmeStream
{
    GstPad*             pad;
    GstCaps*            caps;
    ST_SME_MEDIAINFO*   mediainfo;
    gboolean            discont;
    gboolean            eos;

    GstClockTime        last_ts;
    GstFlowReturn       last_flow;

    GstTagList *        tags;
    gboolean            tags_update;

    gboolean            b_exit;
};

struct _GstSmeDemuxFfPro
{
    GstElement element;

    GstPad *sinkpad;

    SMEDEMUXHDL demuxer;
    ST_SME_STREAMINFO *smestreams;

    gboolean opened;
    gboolean seekable;
    gboolean flushing;

    gint videopads;
    gint audiopads;

    GstSmeStream *streams[M_SME_STREAM_NUM_MAX];

    GstClockTime start_time;
    GstClockTime duration;

    gchar *play_url;

    GstSegment segment;

    /* cached seek in READY */
    GstEvent *seek_event;

    /* cached upstream events */
    //GList *cached_events;
    gboolean eos;
    //only push or pull is ok
    GstPadMode src_pad_mode;

    /* last seek seqnum */
    guint last_seqnum;
    gboolean first_after_seek;

    /** BEGIN: Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    gboolean is_to_deactive;
    GRecMutex st_rec_demux_mutex;  //demux element lock
    gint io_interrupt;             /* whether avio will abort blocking when reading data
                                    * 1: will abort blocking, return immediately;
                                    * 0: will block
                                    *
                                    * see AVIOInterruptCB in ffmpeg: libavformat/avio.h
                                    *
                                    * Callback for checking whether to abort blocking functions.
                                    * AVERROR_EXIT is returned in this case by the interrupted
                                    * function. During blocking operations, callback is called with
                                    * opaque as parameter. If the callback returns 1, the
                                    * blocking operation will be aborted.
                                    */
    /** END:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */

    GstTask *task;
    GRecMutex task_lock;

    /* whether send new segment to down stream */
    gboolean dmx_new_segment;

#if defined (SME_FF_PRO_DUMP_ES_VIDEO) || defined (SME_FF_PRO_DUMP_ES_AUDIO)
    ST_SMEDEMUX_SHOW_INFO demux_proc_info;

    // to dump video/audio data
    gboolean enable_video_dump;
    gboolean enable_audio_dump;
    FILE *video_es_file;
    FILE *audio_es_file;
    gchar *dump_path;
    guint video_dump_index;
    guint audio_dump_index;
#endif
};

struct _GstSmeDemuxFfProClass
{
    GstElementClass parent_class;

    GstPadTemplate *sinktempl;
    GstPadTemplate *videosrctempl;
    GstPadTemplate *audiosrctempl;
};

G_END_DECLS


#endif
