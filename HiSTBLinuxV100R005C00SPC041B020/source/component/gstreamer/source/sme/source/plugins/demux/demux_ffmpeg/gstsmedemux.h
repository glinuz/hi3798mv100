/**************************************************************************//**

  Copyright (C), 2001-2014, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file	      -gstsmedemux.h
 * @brief	      -sme demux element header file
 * @author    -fengke(f00165155)
 * @date	      -2014/3/27
 * @version   -V1.00
 *
 * History:
 *
 * Number  : V1.00
 * Date      : 2014/3/27
 * Author    : fengke(f00165155)
 * Desc      : Created file
 *
******************************************************************************/

#ifndef __GST_SME_DEMUX_H__
#define __GST_SME_DEMUX_H__

#define SME_DUMP_ES_VIDEO
#define SME_DUMP_ES_AUDIO

#include <stdio.h>
#include "gstsme.h"
#include "sme_demux_inf.h"
#include "gstsmepipe.h"

#if defined (SME_DUMP_ES_VIDEO) || defined (SME_DUMP_ES_AUDIO)
#include <gst/gst.h>
#include "sme_proc.h"
#endif

G_BEGIN_DECLS

#define GST_SME_DEMUX(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_SME_DEMUX, GstSmeDemux))
#define GST_SME_DEMUX_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_SME_DEMUX, GstSmeDemux))
#define GST_IS_SME_DEMUX(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SME_DEMUX))
#define GST_IS_SME_DEMUX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SME_DEMUX))

typedef struct _GstSmeStream GstSmeStream;
typedef struct _GstSmeDemux GstSmeDemux;
typedef struct _GstSmeDemuxClass GstSmeDemuxClass;

struct _GstSmeDemux
{
	GstElement element;

	GstPad *sinkpad;

	SMEDEMUXHDL demuxer;
	ST_SME_STREAMINFO *smestreams;

	gboolean opened;
	gboolean seekable;
	gboolean flushing;

    /** BEGIN: Added by baihaili(b00311320),2015/2/10
    Set flag for fragmented mp4 parsing in order to
    get right duration(dash/mss/etc.)*/
    gboolean dash_mode;
    /** END:   Added by baihaili(b00311320), 2015/2/10 */

    /* BEGIN: Modified for DTS2015072310170 by lizimian(l00251625), 2015/7/28
     * For MP3 stream seeking in HLS mode, we need to adjust the pts
     * due to the pts is always started from 0 after the first seek action.
     * So here we add a flag to let us konw whether we need to adjust the pts.
     */
    gboolean hls_mode;
    gboolean hls_mp3;
    gboolean hls_mp3_seek_flag;
    /* END:   Modified for DTS2015072310170 by lizimian(l00251625), 2015/7/28 */

	gint videopads;
	gint audiopads;

	GstSmeStream *streams[M_SME_STREAM_NUM_MAX];

	GstClockTime start_time;
	GstClockTime duration;

	GstSegment segment;

	/* cached seek in READY */
	GstEvent *seek_event;

	/* cached upstream events */
	GList *cached_events;
    gboolean eos;
    //src mode只允许都为push或者都为pull
    GstPadMode src_pad_mode;

    /* last seek seqnum */
    guint last_seqnum;
    gboolean first_after_seek;

    /** BEGIN: Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    gboolean is_to_deactive;
    GRecMutex st_rec_demux_mutex;   //demux 状态锁
    gint io_interrupt;              //demux io是否立即中断阻塞，1中断，0不中断。
    /** END:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    GstSmePipe smepipe;
    GstTask *task;
    GRecMutex task_lock;
    /* Modified BEGIN: DTS2014081803342, zhoubing z00213950 2014/08/19 */
    gboolean recv_segment;
    /* Modified END: DTS2014081803342, zhoubing z00213950 2014/08/19 */

#if defined (SME_DUMP_ES_VIDEO) || defined (SME_DUMP_ES_AUDIO)
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

struct _GstSmeDemuxClass
{
	GstElementClass parent_class;

	SMEDEMUXFMT    demuxfmt;
	GstPadTemplate *sinktempl;
	GstPadTemplate *videosrctempl;
	GstPadTemplate *audiosrctempl;
};

G_END_DECLS

extern ST_SME_IOPROTO gst_protocol;
extern ST_SME_IOPROTO gstpipe_protocol;

#endif
