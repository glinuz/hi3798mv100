/**************************************************************************//**

  Copyright (C), 2001-2016, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    -gstsmeffprodmx.c
 * @brief    -sme demux element for ffmpeg protocol source file
 * @author
 * @date    -2016/07/26
 * @version -V1.00
 *
 * History:
 *
 * Number : V1.00
 * Date   : 2016/07/26
 * Author :
 * Desc   : Created file (porting from gstsmedemux.c, including some bug fix, just like DTS... )
 *
******************************************************************************/
#include <string.h>
#include <stdlib.h>

#include "osal_str.h"
#include "osal_mem.h"
#include "gstsmecodecmap.h"
#include "gstsmeffprodmx.h"

static void
gst_smedmx_ffpro_class_init (GstSmeDemuxFfProClass * klass);
static void
gst_smedmx_ffpro_base_init (GstSmeDemuxFfProClass * klass);
static void
gst_smedmx_ffpro_init (GstSmeDemuxFfPro * demux);
static void
gst_smedmx_ffpro_finalize (GObject * object);
static gboolean
gst_smedmx_ffpro_sink_activate (GstPad * sinkpad, GstObject * parent);
static gboolean
gst_smedmx_ffpro_sink_activate_mode (GstPad * sinkpad,
    GstObject * parent, GstPadMode mode, gboolean active);
static void
gst_smedmx_ffpro_loop (GstSmeDemuxFfPro * demux);
static gboolean
gst_smedmx_ffpro_send_event (GstElement * element, GstEvent * event);
static GstStateChangeReturn
gst_smedmx_ffpro_change_state (GstElement * element, GstStateChange transition);
static gboolean
gst_smedmx_ffpro_push_event (GstSmeDemuxFfPro * demux, GstEvent * event);
static gboolean
gst_smedmx_ffpro_perform_seek (GstSmeDemuxFfPro * demux, GstEvent * event);

//espool
static gpointer
gst_smedmx_ffpro_espool_lockbuf(gpointer gp_ctx, guint gu32_size, gint gi32_streamindex);

static void
gst_smedmx_ffpro_espool_unlockbuf(gpointer gp_ctx, gpointer gp_buffer);

static GstCaps *
gst_smedmx_ffpro_query_srcpad_caps (const GstSmeDemuxFfPro * demux, GstPad * pad,
                            const GstCaps * filter);

static void
gst_smedmx_ffpro_set_property (GObject *object, guint prop_id, const GValue *value,
    GParamSpec *pspec);

static void
gst_smedmx_ffpro_get_property (GObject *object, guint prop_id, GValue *value,
    GParamSpec *pspec);

#if !defined (SME_MULTI_AUDIO_TRACK)
static V_BOOL
gst_smedmx_ffpro_have_audio_srcpad_available(const GstSmeDemuxFfPro * demux);
#endif

static V_BOOL
gst_smedmx_ffpro_have_video_srcpad_available(const GstSmeDemuxFfPro * demux);

static V_INT32
gst_smedmx_ffpro_get_max_video_duration
        (const GstSmeDemuxFfPro * demux, V_UINT32 u32StreamIdx);

static void
gst_smedmx_ffpro_check_src_pad_push_eos(GstSmeDemuxFfPro* demux);

static GstFlowReturn
gst_smedmx_ffpro_chain (GstPad * sinkpad, GstObject * parent, GstBuffer * buffer);

static gboolean
gst_smedmx_ffpro_sink_activate_push (GstPad * sinkpad, GstObject * parent,
    gboolean active);

static gboolean
gst_smedmx_ffpro_sink_event (GstPad * sinkpad, GstObject * parent,
    GstEvent * event);

static gboolean
gst_smedmx_ffpro_audio_tag_list(GstSmeDemuxFfPro *demux,
                            GstSmeStream *stream);
static gboolean
gst_smedmx_ffpro_post_unsupported_audio_track_info(GstSmeDemuxFfPro *demux,
                                               const ST_SME_AUDIOINFO *audioinfo);

static gboolean
gst_smedmx_ffpro_post_video_track_info(GstSmeDemuxFfPro *demux,
                                   const ST_SME_VIDEOINFO *videoinfo, V_BOOL is_supported);

static const gchar *gst_smedmx_ffpro_getaudiomime(const ST_SME_MEDIAINFO *mediainfo);

enum
{
    PROP_0 = 0,
    PROP_BUF_FULL,
    PROP_STREAM_BPS,
    PROP_BUFFERED_AUDIO,
    PROP_BUFFERED_VIDEO,
    PROP_EXIT_BLOCK,
    PROP_BENCHMARK,
    PROP_SME_DEMUX_INFO,
    PROP_LAST
};

typedef struct _tagGstAllocCtx{
    GstSmeDemuxFfPro*    pst_demux;
    GstBuffer*      pst_gstbuffer;
    GstMapInfo      st_info;
    GstSmeStream*   stream;
}GstAllocCtx;



typedef struct _tagGstSmeEsQueueItem{
    gint64      gi64_pts;
    gint64      gi64_dts;
    guint32     gu32_duration;
    guint       gu32_size;
    gpointer    gp_buffer; //gpv buffer
}GstSmeEsQueueItem;

static GstElementClass *parent_class = NULL;

#define GST_SME_FFPRO_STATIC_CAPS "media/ff_pro"

static void
gst_smedmx_ffpro_base_init (GstSmeDemuxFfProClass * klass)
{
    GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
    GstPadTemplate *sinktempl, *audiosrctempl, *videosrctempl;

    /* construct the element details struct */
    gst_element_class_set_details_simple (element_class, "smedmx_ff_pro",
      "Codec/Demuxer", "Sme demux for ffmpeg protocol",
      "anonymous author");

    sinktempl = gst_pad_template_new ("sink",
      GST_PAD_SINK, GST_PAD_ALWAYS, gst_caps_new_empty_simple(GST_SME_FFPRO_STATIC_CAPS));

    videosrctempl = gst_pad_template_new ("video_%u",
      GST_PAD_SRC, GST_PAD_SOMETIMES, GST_CAPS_ANY);
    audiosrctempl = gst_pad_template_new ("audio_%u",
      GST_PAD_SRC, GST_PAD_SOMETIMES, GST_CAPS_ANY);

    gst_element_class_add_pad_template (element_class, videosrctempl);
    gst_element_class_add_pad_template (element_class, audiosrctempl);
    gst_element_class_add_pad_template (element_class, sinktempl);

    klass->videosrctempl = videosrctempl;
    klass->audiosrctempl = audiosrctempl;
    klass->sinktempl = sinktempl;
}

static void
gst_smedmx_ffpro_class_init (GstSmeDemuxFfProClass * klass)
{
    GObjectClass *gobject_class;
    GstElementClass *gstelement_class;

    gobject_class = (GObjectClass *) klass;
    gstelement_class = (GstElementClass *) klass;

    parent_class = g_type_class_peek_parent (klass);

    gobject_class->set_property = gst_smedmx_ffpro_set_property;
    gobject_class->get_property = gst_smedmx_ffpro_get_property;
    gobject_class->finalize = GST_DEBUG_FUNCPTR (gst_smedmx_ffpro_finalize);

    gstelement_class->change_state = gst_smedmx_ffpro_change_state;
    gstelement_class->send_event = gst_smedmx_ffpro_send_event;

    g_object_class_install_property(gobject_class, PROP_BUF_FULL,
        g_param_spec_uint(GST_SME_PROP_KEY_BUFFER_FULL, "demux buffer ful",
        "demux buffer ful", 0, (guint)(-1), 0, G_PARAM_READWRITE
        | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property(gobject_class, PROP_STREAM_BPS,
        g_param_spec_uint(GST_SME_PROP_KEY_STREAM_BPS, "stream bps",
        "stream bps", 0, (guint)(-1), 0, G_PARAM_READWRITE
        | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property(gobject_class, PROP_BUFFERED_AUDIO,
        g_param_spec_uint(GST_SME_PROP_KEY_BUFFERED_AUDIO, "demux buffered audio",
        "demux buffered audio", 0, (guint)(-1), 0, G_PARAM_READWRITE
        | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property(gobject_class, PROP_BUFFERED_VIDEO,
        g_param_spec_uint(GST_SME_PROP_KEY_BUFFERED_VIDEO, "demux buffered video",
        "demux buffered video", 0, (guint)(-1), 0, G_PARAM_READWRITE
        | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property(gobject_class, PROP_EXIT_BLOCK,
        g_param_spec_int(GST_SME_PROP_KEY_EXIT_BLOCK, "EXIT BLOCK",
        "demux exit block", 0, (gint)(G_MAXINT32), 0, G_PARAM_READWRITE
        | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class,
                    PROP_BENCHMARK,
                    g_param_spec_pointer(GST_SME_PROP_KEY_BENCHMARK, "benchmark",
                                    "benchmark",
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class, PROP_SME_DEMUX_INFO,
        g_param_spec_pointer ("sme-demux-info" , "Sme-Demux-Info",
          "Poniter of SmeDemux",
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}

static void
gst_smedmx_ffpro_init (GstSmeDemuxFfPro * demux)
{
    GstSmeDemuxFfProClass *oclass =
        (GstSmeDemuxFfProClass *) (G_OBJECT_GET_CLASS (demux));
    gint n;

    GST_INFO("smedmx_ffpro init name =%s", GST_ELEMENT_NAME(demux));

    demux->sinkpad = gst_pad_new_from_template (oclass->sinktempl, "sink");

    gst_pad_set_activate_function (demux->sinkpad,
      GST_DEBUG_FUNCPTR (gst_smedmx_ffpro_sink_activate));

    gst_pad_set_activatemode_function (demux->sinkpad,
      GST_DEBUG_FUNCPTR (gst_smedmx_ffpro_sink_activate_mode));

    gst_element_add_pad (GST_ELEMENT (demux), demux->sinkpad);

    gst_pad_set_event_function (demux->sinkpad,
      GST_DEBUG_FUNCPTR (gst_smedmx_ffpro_sink_event));

    gst_pad_set_chain_function (demux->sinkpad,
      GST_DEBUG_FUNCPTR (gst_smedmx_ffpro_chain));

    demux->opened = FALSE;
    demux->demuxer = NULL;

    demux->play_url = NULL;

    for (n = 0; n < M_SME_STREAM_NUM_MAX; n++) {
        demux->streams[n] = NULL;
    }
    demux->videopads = 0;
    demux->audiopads = 0;

    demux->seek_event = NULL;
    demux->src_pad_mode = GST_PAD_MODE_NONE;
    gst_segment_init (&demux->segment, GST_FORMAT_TIME);

    demux->last_seqnum = 0;
    demux->first_after_seek = FALSE;

    /** BEGIN:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    demux->is_to_deactive = FALSE;
    g_rec_mutex_init(&demux->st_rec_demux_mutex);
    g_atomic_int_set(&demux->io_interrupt, 0);
    /** END:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */

    demux->task = gst_task_new ((GstTaskFunction) gst_smedmx_ffpro_loop, demux, NULL);
    g_rec_mutex_init (&demux->task_lock);
    gst_task_set_lock (demux->task, &demux->task_lock);

    demux->dmx_new_segment = FALSE;

    demux->seekable = FALSE;

#if defined (SME_FF_PRO_DUMP_ES_VIDEO) || defined (SME_FF_PRO_DUMP_ES_AUDIO)
    if (E_SME_MEM_OK != VOS_Memset_S(&demux->demux_proc_info,
                                     sizeof(demux->demux_proc_info),
                                     0,
                                     sizeof(demux->demux_proc_info))) {
        GST_ERROR("VOS_Memset_S return failed");
    }

    // to init dump video/audio data
    demux->enable_video_dump = FALSE;
    demux->enable_audio_dump = FALSE;
    demux->video_es_file = NULL;
    demux->audio_es_file = NULL;
    demux->dump_path = NULL;
    demux->video_dump_index = 0;
    demux->audio_dump_index = 0;
    gettimeofday(&demux->demux_proc_info.sme_demux_created_time, NULL);
#endif
}

static void
gst_smedmx_ffpro_finalize (GObject * object)
{
    GstSmeDemuxFfPro * demux = (GstSmeDemuxFfPro *)object;

    GST_INFO("smedmx_ffpro finalize name =%s", GST_ELEMENT_NAME(demux));

    if (demux->play_url) {
        g_free(demux->play_url);
        demux->play_url = NULL;
    }

    /** BEGIN:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    g_rec_mutex_clear(&demux->st_rec_demux_mutex);
    /** END:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */

    (void)gst_task_join (demux->task);
    gst_object_unref (demux->task);
    g_rec_mutex_clear (&demux->task_lock);

    G_OBJECT_CLASS (parent_class)->finalize (object);
}

/* check if all streams are eos */
static gboolean
gst_smedmx_ffpro_is_eos (const GstSmeDemuxFfPro * demux)
{
    gint Idx;

    for (Idx = 0; Idx < M_SME_STREAM_NUM_MAX; Idx++) {
        GstSmeStream *s = demux->streams[Idx];
        if (NULL != s) {
            GST_DEBUG ("stream %d %p eos:%d", Idx, s, s->eos);
            if (!s->eos)
                return FALSE;
        }
    }

    return TRUE;
}

/* Returns True if we at least outputted one buffer */
static gboolean
gst_smedmx_ffpro_has_outputted (const GstSmeDemuxFfPro * demux)
{
    gint Idx;

    for (Idx = 0; Idx < M_SME_STREAM_NUM_MAX; Idx++) {
        GstSmeStream *s = demux->streams[Idx];
        if (NULL != s) {
            if (GST_CLOCK_TIME_IS_VALID (s->last_ts))
                return TRUE;
        }
    }
    return FALSE;
}

static gboolean
gst_smedmx_ffpro_send_event (GstElement * element, GstEvent * event)
{
    GstSmeDemuxFfPro *demux = (GstSmeDemuxFfPro *) (element);
    gboolean res = TRUE;

    switch (GST_EVENT_TYPE (event)) {
        case GST_EVENT_SEEK:
            GST_OBJECT_LOCK (demux);
            if (!demux->opened) {
                GstEvent **event_p;

                GST_DEBUG_OBJECT (demux, "[smedmx_ffpro]caching seek event");
                event_p = &demux->seek_event;
                gst_event_replace (event_p, event);
                GST_OBJECT_UNLOCK (demux);

                res = TRUE;
            } else {
                GST_OBJECT_UNLOCK (demux);
                res = gst_smedmx_ffpro_perform_seek (demux, event);
                gst_event_unref (event);
            }
            break;
        default:
            res = FALSE;
            break;
    }

    return res;
}

static GstFlowReturn
gst_smedmx_ffpro_aggregated_flow (GstSmeDemuxFfPro * demux)
{
    gint Idx;
    GstFlowReturn res = GST_FLOW_OK;
    gboolean have_ok = FALSE;

    for (Idx = 0; Idx < M_SME_STREAM_NUM_MAX; Idx++) {
        GstSmeStream *s = demux->streams[Idx];

        if (s) {
            res = MIN (res, s->last_flow);

            if (s->last_flow == GST_FLOW_OK)
                have_ok = TRUE;
        }
    }

    /* NOT_LINKED is OK, if at least one pad is linked */
    if (res == GST_FLOW_NOT_LINKED && have_ok)
        res = GST_FLOW_OK;

    GST_DEBUG_OBJECT (demux, "Returning aggregated value of %s",
            gst_flow_get_name (res));

    return res;
}

static gchar *
gst_smedmx_ffpro_create_padname (const gchar * templ, gint n)
{
    GString *string = NULL;

    /* FIXME, we just want to printf the number according to the template but
    * then the format string is not a literal and we can't check arguments and
    * this generates a compiler error */
    string = g_string_new (templ);
    if(string == NULL)
        return NULL;

    g_string_truncate (string, string->len - 2);
    g_string_append_printf (string, "%u", n);

    return g_string_free (string, FALSE);
}

/* set flags on all streams */
static void
gst_smedmx_ffpro_set_flags (const GstSmeDemuxFfPro * demux, gboolean discont,
    gboolean eos)
{
    gint Idx;

    for (Idx = 0; Idx < M_SME_STREAM_NUM_MAX; Idx++) {
        GstSmeStream *s = demux->streams[Idx];
        if (NULL != s) {
            s->discont = discont;
            s->eos = eos;
        }
    }
}

static gboolean
gst_smedmx_ffpro_do_seek (GstSmeDemuxFfPro * demux, GstSegment * segment, GstSeekFlags flags)
{
    gboolean ret = TRUE;
    gint seekret;
    guint64 target;
    gint32 smetarget;

    const ST_SME_DEMUX_INF* demuxInf = SME_GetDemuxInf();

    GST_INFO_OBJECT (demux, "gst_smedmx_ffpro_do_seek in");

    /* initial seek position */
    target = segment->position;

    GST_INFO_OBJECT (demux, "target %llu", target);

    /* calc position to ms */
    smetarget = (gint32)(target/M_SME_MS_TO_NS);

    GST_INFO_OBJECT (demux, "smetarget %d", smetarget);

    if (flags & GST_SEEK_FLAG_SNAP_BEFORE) {
        seekret = demuxInf->SME_SeekTo(demux->demuxer, smetarget, -1, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_SYNC);
    } else {
        seekret = demuxInf->SME_SeekTo(demux->demuxer, smetarget, -1, AVSEEK_FLAG_SYNC);
    }
    if (seekret < 0)
        goto seek_failed;

    GST_INFO_OBJECT (demux, "SME_SeekTo success, returned %d", seekret);

    target = (guint64)(gint64)seekret;
    target *= M_SME_MS_TO_NS;
    segment->position = target;
    segment->time = target;
    segment->start = target;

    GST_INFO_OBJECT (demux, "gst_smedmx_ffpro_do_seek out");

    return ret;

/* ERRORS */
seek_failed:
    {
        GST_WARNING_OBJECT (demux, "seek failed : %d", seekret);
        return FALSE;
    }
}

static gboolean
gst_smedmx_ffpro_perform_seek (GstSmeDemuxFfPro * demux, GstEvent * event)
{
    gboolean res = TRUE;
    gdouble rate = 0;
    GstFormat format = GST_FORMAT_UNDEFINED;
    GstSeekFlags flags = GST_SEEK_FLAG_NONE;
    GstSeekType cur_type = GST_SEEK_TYPE_NONE;
    GstSeekType stop_type = GST_SEEK_TYPE_NONE;
    gint64 cur = 0;
    gint64 stop = 0;
    gboolean flush = FALSE;
    gboolean update= FALSE;
    GstSegment seeksegment;

    GST_INFO_OBJECT (demux, "SEEK_demux begin");

    const ST_SME_DEMUX_INF* demuxInf = SME_GetDemuxInf();
    if (0 == demuxInf->SME_IsSeekable(demux->demuxer, -1)) {
        /* Seek operation is not available for this stream, post a warning to app.
         * This function will not return false, if return false the decoder will do seek by bytes.
         */
        GST_ELEMENT_WARNING (demux, STREAM, SEEK, ("seek:failed"), ("seek:failed:ffmpeg"));

        GST_INFO_OBJECT (demux,
            "SEEK_demux end: this stream not support seek");

        return TRUE;
    }

    if (event) {
        gst_event_parse_seek (event, &rate, &format, &flags,
            &cur_type, &cur, &stop_type, &stop);

        /* we have to have a format as the segment format. Try to convert
        * if not. */
        if (demux->segment.format != format) {
            GstFormat fmt = demux->segment.format;

            res = TRUE;
            /* FIXME, use source pad */
            if (cur_type != GST_SEEK_TYPE_NONE && cur != -1)
                res = gst_pad_query_convert (demux->sinkpad, format, cur, fmt, &cur);
            if (res && stop_type != GST_SEEK_TYPE_NONE && stop != -1)
                res = gst_pad_query_convert (demux->sinkpad, format, stop, fmt, &stop);
            if (!res)
                goto no_format;

            format = fmt;
        }
    } else {
        flags = GST_SEEK_FLAG_NONE;
    }

    flush = flags & GST_SEEK_FLAG_FLUSH;
    /* BEGIN: Added for DTS2015071706676 by liurongliang(l00180035), 2015/7/22 */
    /*
       we must flush start before pause the sinkpad loop,because the loop maybe blocked
       when push data if the multiqueue if full.
    */
    if (flush) {
        /* send flush start to downstream */
        GstEvent * flush_start_event = gst_event_new_flush_start ();
        if(NULL != flush_start_event) {
           gst_smedmx_ffpro_push_event (demux, flush_start_event);
        }else {
            GST_WARNING("new flush failed why?");
            GST_ELEMENT_WARNING (demux, STREAM, SEEK, ("seek:failed"), ("seek:failed:ffmpeg"));
            //we cannot return false;
            return TRUE;
        }
    }
    /* END:   Added for DTS2015071706676 by liurongliang(l00180035), 2015/7/22 */


    GST_INFO_OBJECT (demux, "will pause demux sinkpad task(%p).",
        GST_PAD_TASK(demux->sinkpad));
    gst_pad_pause_task (demux->sinkpad);
    GST_INFO_OBJECT (demux, "demux sinkpad task paused");

    if (flush) {
        /* mark flushing so that the streaming thread can react on it */
        GST_OBJECT_LOCK (demux);
        demux->flushing = TRUE;
        demux->eos = FALSE;
        GST_OBJECT_UNLOCK (demux);
    }

    /* send flush start to up stream */
    /*
    if (flush) {
        GST_INFO_OBJECT (demux, "send flush start to upstream.");
        gst_pad_push_event (demux->sinkpad, gst_event_new_flush_start ());
    }
    */

    /* grab streaming lock, this should eventually be possible, either
    * because the task is paused or our streaming thread stopped
    * because our peer is flushing. */
    GST_INFO_OBJECT (demux, "GST_PAD_STREAM_LOCK(pad:%p)", demux->sinkpad);
    GST_PAD_STREAM_LOCK (demux->sinkpad);

    /* make copy into temp structure, we can only update the main one
    * when we actually could do the seek. */
    if (E_SME_MEM_OK != VOS_Memcpy_S (&seeksegment, sizeof (GstSegment), &demux->segment, sizeof (GstSegment)))
    {
        GST_ERROR("SECURE: VOS_Memcpy_S return failed.");
    }

    GST_INFO_OBJECT (demux, "gst_segment_do_seek(cur:%lld, stop:%lld)", cur, stop);

    /* now configure the seek segment */
    if (event) {
        gst_segment_do_seek (&seeksegment, rate, format, flags,
            cur_type, (guint64)cur, stop_type, (guint64)stop, &update);
    }

    GST_INFO_OBJECT (demux, "segment configured from %" G_GINT64_FORMAT
        " to %" G_GINT64_FORMAT ", position %" G_GINT64_FORMAT,
        seeksegment.start, seeksegment.stop, seeksegment.position);

    /* make the sinkpad available for data passing since we might need
    * it when doing the seek */
    /*
    if (flush) {
        //GST_OBJECT_LOCK (demux);
        //demux->flushing = FALSE;
        //GST_OBJECT_UNLOCK (demux);
        GST_INFO_OBJECT (demux, "send flush stop to upstream.");
        gst_pad_push_event (demux->sinkpad, gst_event_new_flush_stop ((gboolean)TRUE));
    }
    */

    /* do the seek, segment.position contains new position. */
    res = gst_smedmx_ffpro_do_seek (demux, &seeksegment, flags);

    GST_INFO_OBJECT(demux, "gst_smedmx_ffpro_do_seek return. res=%d ", res);

    /* Seek succeed, flush downstream, and prepare to continue streaming */
    if (flush) {
        gint n;
        /* send flush stop to down stream, peer will accept data and events again. We
        * are not yet providing data as we still have the STREAM_LOCK. */
        GST_INFO_OBJECT (demux, "send flush stop to downstream.");

        GstEvent * flush_stop_event = gst_event_new_flush_stop ((gboolean)TRUE);
        if(NULL != flush_stop_event) {
            gst_smedmx_ffpro_push_event (demux, flush_stop_event);
        }else {
            GST_PAD_STREAM_UNLOCK (demux->sinkpad);
            return FALSE;
        }

        for (n = 0; n < M_SME_STREAM_NUM_MAX; ++n) {
            if (demux->streams[n])
            demux->streams[n]->last_flow = GST_FLOW_OK;
        }
    }

    /* if successfull seek, we update our real segment and push
    * out the new segment. */
    if (!res) {
        /* Seek failed, post a warning to app.
         * This function will not return false, if return false the decoder will do seek by bytes.
         */
        GST_ELEMENT_WARNING (demux, STREAM, SEEK, ("seek:failed"), ("seek:failed:ffmpeg"));
    }
    demux->first_after_seek = TRUE;

    /* Mark discont on all srcpads and remove eos */
    gst_smedmx_ffpro_set_flags ((const GstSmeDemuxFfPro *)demux, (gboolean)TRUE, (gboolean)FALSE);

    if (flush) {
        /* mark flushing so that the streaming thread can react on it */
        GST_OBJECT_LOCK (demux);
        demux->flushing = FALSE;
        GST_OBJECT_UNLOCK (demux);
    }

    /* and restart the task in case it got paused explicitely or by
    * the FLUSH_START event we pushed out. */
    GST_INFO_OBJECT (demux, "starting demux sinkpad task(%p)", GST_PAD_TASK(demux->sinkpad));
    res = gst_pad_start_task (demux->sinkpad, (GstTaskFunction) gst_smedmx_ffpro_loop,
        demux->sinkpad, NULL);
    GST_INFO_OBJECT (demux, "start demux sinkpad task return %d", res);

    /* and release the lock again so we can continue streaming */
    GST_PAD_STREAM_UNLOCK (demux->sinkpad);

    GST_INFO_OBJECT (demux, "SEEK_demux end");

    return res;

/* ERROR */
no_format:
    {
        GST_DEBUG_OBJECT (demux, "undefined format given, seek aborted.");
        return FALSE;
    }
}

static gboolean
gst_smedmx_ffpro_src_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
    GstSmeDemuxFfPro *demux;
    GstSmeStream *stream;
    gboolean res = TRUE;

    GST_INFO("gst_smedmx_ffpro_src_event in(event:%s)", GST_EVENT_TYPE_NAME(event));
    GST_INFO("gst_smedmx_ffpro_src_event in(event seq:%u)", event->seqnum);

    stream = gst_pad_get_element_private (pad);
    if (!stream)
        return FALSE;

    demux = (GstSmeDemuxFfPro *) parent;

    switch (GST_EVENT_TYPE (event)) {
        case GST_EVENT_SEEK:
            GST_INFO_OBJECT (demux, "GST_EVENT_SEEK received in src pad:%p", pad);

            /* will receive seek event twice(one from videosink, another from audiosink),
             * and the seqnum is different, will ignore it when secondly received.
             */
            if(event->seqnum != demux->last_seqnum){
                res = gst_smedmx_ffpro_perform_seek (demux, event);
                demux->last_seqnum = event->seqnum;
            }else{
                GST_WARNING_OBJECT (demux, "same seek event, ignore it.");
            }

            gst_event_unref (event);
            break;
        case GST_EVENT_LATENCY:
            res = gst_pad_push_event (demux->sinkpad, event);
            break;
        case GST_EVENT_NAVIGATION:
        case GST_EVENT_QOS:
        default:
            res = FALSE;
            gst_event_unref (event);
            break;
    }

    GST_INFO("gst_smedmx_ffpro_src_event out");

    return res;
}

static gboolean
gst_smedmx_ffpro_src_query (GstPad * pad, GstObject * parent, GstQuery * query)
{
    GstSmeDemuxFfPro *demux;
    GstSmeStream *stream;
    gboolean res = FALSE;

    GST_INFO("gst_smedmx_ffpro_src_query in(query:%s)", GST_QUERY_TYPE_NAME(query));

    stream = gst_pad_get_element_private (pad);
    if (NULL == stream)
        return FALSE;

    demux = (GstSmeDemuxFfPro *) parent;

    switch (GST_QUERY_TYPE (query)) {
    case GST_QUERY_POSITION:
    {
        GstFormat format = GST_FORMAT_UNDEFINED;
        gint64 timeposition;

        gst_query_parse_position (query, &format, NULL);

        timeposition = (gint64)stream->last_ts;
        if (!(GST_CLOCK_TIME_IS_VALID (timeposition)))
            break;

        switch (format) {
        case GST_FORMAT_TIME:
            gst_query_set_position (query, GST_FORMAT_TIME, timeposition);
            res = TRUE;
            break;
        case GST_FORMAT_DEFAULT:
            GST_ERROR("GST_FORMAT_DEFAULT not support.");
            //gst_query_set_position (query, GST_FORMAT_DEFAULT,
            //gst_util_uint64_scale (timeposition, avstream->r_frame_rate.num,
            //    GST_SECOND * avstream->r_frame_rate.den));
            res = TRUE;
            break;
        case GST_FORMAT_BYTES:
            if (demux->videopads + demux->audiopads == 1 &&
                GST_PAD_PEER (demux->sinkpad) != NULL)
            res = gst_pad_query_default (pad, parent, query);
            break;
        default:
            break;
        }
    }
    break;
    case GST_QUERY_DURATION:
    {
        GstFormat format = GST_FORMAT_UNDEFINED;
        gint64 timeduration;

        gst_query_parse_duration (query, &format, NULL);

        timeduration = (gint64)demux->duration;

        /** BEGIN: Modified for DTS2014081802192 by liurongliang(l00180035), 2014/8/18 */
        GST_INFO("gst_smedmx_ffpro GST_QUERY_DURATION timeduration=%lld",
            timeduration);
        //if (!(GST_CLOCK_TIME_IS_VALID (timeduration)))
        //    break;
        /** END:   Modified for DTS2014081802192 by liurongliang(l00180035), 2014/8/18 */

        switch (format) {
        case GST_FORMAT_TIME:
            gst_query_set_duration (query, GST_FORMAT_TIME, timeduration);
            res = TRUE;
            break;
        case GST_FORMAT_DEFAULT:
            GST_ERROR("GST_FORMAT_DEFAULT not support.");
            //gst_query_set_duration (query, GST_FORMAT_DEFAULT,
            //gst_util_uint64_scale (timeduration, mediainfo->r_frame_rate.num,
            //    GST_SECOND * mediainfo->r_frame_rate.den));
            res = TRUE;
            break;
        case GST_FORMAT_BYTES:
            if (demux->videopads + demux->audiopads == 1 &&
                GST_PAD_PEER (demux->sinkpad) != NULL)
            res = gst_pad_query_default (pad, parent, query);
            break;
        default:
            break;
        }
    }
        break;
    case GST_QUERY_SEEKING:
    {
        GstFormat format = GST_FORMAT_UNDEFINED;
        gboolean seekable = demux->seekable;
        gint64 dur = -1;

        gst_query_parse_seeking (query, &format, NULL, NULL, NULL);
        if (!gst_pad_query_duration (pad, format, &dur)) {
            /* unlikely that we don't know duration but can seek */
            GST_WARNING("gst_pad_query_duration failed");
            seekable = FALSE;
            dur = -1;
        }
        gst_query_set_seeking (query, format, seekable, (gint64)0, dur);
        res = TRUE;
        break;
    }
    case GST_QUERY_SCHEDULING:{
        gst_query_add_scheduling_mode (query, GST_PAD_MODE_PUSH);
        res = TRUE;
        break;
    }
    case GST_QUERY_CAPS:{
        GstCaps *caps = NULL;
        GstCaps *filter = NULL;

        gst_query_parse_caps (query, &filter);
        GST_INFO("pad=%p, stream->pad=%p", pad, stream->pad);
        caps = gst_smedmx_ffpro_query_srcpad_caps ((GstSmeDemuxFfPro *)demux, pad,
            (const GstCaps *)filter);
        if(NULL == caps){
            caps = gst_caps_ref (stream->caps);
        }
        gst_query_set_caps_result (query, caps);
        gst_caps_unref (caps);
        res = TRUE;
        break;
    }
    default:
        GST_INFO("gst_smedmx_ffpro_src_query defaut(%#x)", GST_QUERY_TYPE (query));
        /* FIXME : ADD GST_QUERY_CONVERT */
        res = gst_pad_query_default (pad, parent, query);
        break;
    }
    GST_INFO("gst_smedmx_ffpro_src_query out");

    return res;
}

/* send an event to all the source pads .
 * Takes ownership of the event.
 *
 * Returns FALSE if none of the source pads handled the event.
 */
static gboolean
gst_smedmx_ffpro_push_event (GstSmeDemuxFfPro * demux, GstEvent * event)
{
    gboolean res = TRUE;
    guint Idx;

    for (Idx = 0; Idx < M_SME_STREAM_NUM_MAX; Idx++) {
        GstSmeStream *s = demux->streams[Idx];

        if (s && s->pad) {
            gst_event_ref (event);
            GST_WARNING("PAD(%s), push event(%s)", GST_PAD_NAME(s->pad),
                GST_EVENT_TYPE_NAME(event));
            res &= gst_pad_push_event (s->pad, event);
            GST_WARNING("PAD(%s), push event(%s) end, res=%d", GST_PAD_NAME(s->pad),
                 GST_EVENT_TYPE_NAME(event), res);
        }
    }
    gst_event_unref (event);

    return res;
}

#if !defined (SME_MULTI_AUDIO_TRACK)
static V_BOOL
gst_smedmx_ffpro_have_audio_srcpad_available(const GstSmeDemuxFfPro * demux)
{
    V_UINT32 i32StreamIdx = demux->smestreams->u32NumStream;
    V_UINT32 Idx = 0;

    if(0 == demux->audiopads)
    {
        return FALSE;
    }

    for(; Idx < i32StreamIdx; Idx++)
    {
        GstSmeStream *stream = demux->streams[Idx];
        if( stream && stream->mediainfo
            && E_SME_MEDIA_TYPE_AUDIO == stream->mediainfo->eMediaType)
            return TRUE;
    }

    return FALSE;
}
#endif

static V_BOOL
gst_smedmx_ffpro_have_video_srcpad_available(const GstSmeDemuxFfPro * demux)
{
    V_UINT32 i32StreamIdx = demux->smestreams->u32NumStream;
    V_UINT32 i = 0;

    if(0 == demux->videopads)
    {
        return FALSE;
    }
    for(;i<i32StreamIdx;i++)
    {
        GstSmeStream *stream = demux->streams[i];
        if( stream && stream->mediainfo
            && E_SME_MEDIA_TYPE_VIDEO == stream->mediainfo->eMediaType)
            return TRUE;
    }
    return FALSE;
}

static V_INT32
gst_smedmx_ffpro_get_max_video_duration
        (const GstSmeDemuxFfPro * demux, V_UINT32 u32StreamIdx)
{
    V_UINT32 i = 0;
    V_INT32 i32Duration = 0;
    GstSmeStream *stream = NULL;
    for(;i<u32StreamIdx;i++)
    {
        stream = demux->streams[i];
        if( stream && stream->mediainfo
            && E_SME_MEDIA_TYPE_VIDEO == stream->mediainfo->eMediaType)
        {
            if(i32Duration < stream->mediainfo->i32Duration)
            {
                i32Duration = stream->mediainfo->i32Duration;
            }
        }
    }
    return i32Duration;
}

static GstSmeStream *
gst_smedmx_ffpro_create_stream (GstSmeDemuxFfPro * demux, ST_SME_MEDIAINFO *mediainfo)
{
    GstSmeDemuxFfProClass *oclass;
    GstPadTemplate *templ = NULL;
    GstPad *pad = NULL;
    GstCaps *caps = NULL;
    gint num = -1;
    gchar *padname = NULL;
    GstSmeStream *stream = NULL;
    gboolean bres = FALSE;

    oclass = (GstSmeDemuxFfProClass *) G_OBJECT_GET_CLASS (demux);

    if(NULL == mediainfo){
        GST_INFO("mediainfo is null, get_stream failed.");
        return NULL;
    }

    if (demux->streams[mediainfo->i32StreamIdx] != NULL)
    {
        return demux->streams[mediainfo->i32StreamIdx];
    }

    switch (mediainfo->eMediaType) {
        case E_SME_MEDIA_TYPE_VIDEO:
            if (gst_smedmx_ffpro_have_video_srcpad_available((const GstSmeDemuxFfPro *)demux)) {
                return NULL;
            }

            break;

        case E_SME_MEDIA_TYPE_AUDIO:
#if !defined (SME_MULTI_AUDIO_TRACK)
            if (gst_smedmx_ffpro_have_audio_srcpad_available((const GstSmeDemuxFfPro *)demux)) {
                return NULL;
            }
#endif

            break;

        case E_SME_MEDIA_TYPE_TEXT:
            GST_WARNING("codec_id=%d, not support MEDIA_TYPE_TEXT, eMediaType=%d",
                mediainfo->eCodecId, mediainfo->eMediaType);
            return NULL;
        default:
            GST_WARNING("codec_id=%d, not support eMediaType=%d",
                mediainfo->eCodecId, mediainfo->eMediaType);
            return NULL;
    }

    /* get caps that belongs to this stream */
    caps = gst_sme_codecid_to_caps (mediainfo->eCodecId, mediainfo, (gboolean)TRUE);
    if(caps == NULL){
        switch (mediainfo->eMediaType){
            case E_SME_MEDIA_TYPE_AUDIO:
                break;
            case E_SME_MEDIA_TYPE_VIDEO:
                GST_ELEMENT_WARNING(demux, STREAM, DECODE,
                    ("No caps found for video codecid=%d", (int)(mediainfo->eCodecId)),
                    ("video_dec:v format is not support!"));

                gst_smedmx_ffpro_post_video_track_info(demux, (ST_SME_VIDEOINFO *)mediainfo, (V_BOOL)FALSE);

                break;
            default:
                GST_ELEMENT_WARNING(demux, STREAM, DECODE,
                    ("No caps found for codecid=%d", (int)(mediainfo->eCodecId)),
                    ("unknown media type"));
                break;
        }

        GST_WARNING("codecid=%d,caps is NULL", (int)(mediainfo->eCodecId));

        return NULL;
    }

    gchar* caps_tostring = gst_caps_to_string(caps);
    GST_INFO("caps=%s, CODEC_ID=%d, eMediaType=%d, streamIdx=%d",
        caps_tostring, mediainfo->eCodecId, mediainfo->eMediaType, mediainfo->i32StreamIdx);
    g_free(caps_tostring);

    /* create new stream */
    stream = g_new0 (GstSmeStream, 1);

    stream->discont     = TRUE;
    stream->mediainfo   = mediainfo;
    stream->last_ts     = GST_CLOCK_TIME_NONE;
    stream->last_flow   = GST_FLOW_OK;
    stream->tags        = NULL;
    stream->tags_update = FALSE;
    stream->b_exit      = FALSE;
    stream->eos         = FALSE;

    switch (mediainfo->eMediaType) {
        case E_SME_MEDIA_TYPE_VIDEO:
            templ = oclass->videosrctempl;
            num = demux->videopads ++;

            gst_smedmx_ffpro_post_video_track_info(demux, (ST_SME_VIDEOINFO *)mediainfo, (V_BOOL)TRUE);

            break;

        case E_SME_MEDIA_TYPE_AUDIO:
            templ = oclass->audiosrctempl;
            num = demux->audiopads++;

            /* post message to up layer about the audio track information */
            bres = gst_smedmx_ffpro_audio_tag_list(demux, stream);
            if (!bres)
            {
                GST_WARNING("fail to post audio info message");
            }

            break;

        default:
            GST_WARNING("codec_id=%d, eMediaType=%d, should_not_go_here",
                mediainfo->eCodecId, mediainfo->eMediaType);
            gst_caps_unref (caps);

            g_free(stream);

            return NULL;
    }

    /* create new pad for this stream */
    padname =
        gst_smedmx_ffpro_create_padname (GST_PAD_TEMPLATE_NAME_TEMPLATE (templ),
        num);
    pad = gst_pad_new_from_template (templ, padname);
    g_free (padname);

    if(NULL == pad){
        gst_caps_unref (caps);
        g_free(stream);
        return NULL;
    }

    gst_pad_use_fixed_caps (pad);

    (void)gst_pad_set_active (pad, (gboolean)TRUE);
    (void)gst_pad_set_caps (pad, caps);

    gst_pad_set_query_function (pad, gst_smedmx_ffpro_src_query);
    gst_pad_set_event_function (pad, gst_smedmx_ffpro_src_event);

    /* store pad internally */
    stream->pad = pad;
    stream->caps = gst_caps_ref (caps);
    gst_pad_set_element_private (pad, stream);
    gst_caps_unref (caps);

    /* activate and add */
    (void)gst_element_add_pad (GST_ELEMENT (demux), pad);

    /** BEGIN: Added for DTS2014061902682, 2014/6/24 */
    if(G_UNLIKELY(!GST_PAD_IS_LINKED(pad))){
        switch (mediainfo->eMediaType) {
            case E_SME_MEDIA_TYPE_VIDEO:
                demux->videopads--;
                break;
            case E_SME_MEDIA_TYPE_AUDIO:
                demux->audiopads--;
                break;
            case E_SME_MEDIA_TYPE_TEXT:
            default:
                break;
        }

        g_free(stream);
        gst_caps_unref (caps);

        GST_WARNING("GST_PAD_IS_LINKED return false");

        /** BEGIN: Added for DTS2014081101027 by liurongliang(l00180035), 2014/8/1 */
        gst_pad_set_active (pad, (gboolean)FALSE);
        gst_element_remove_pad (GST_ELEMENT (demux), pad);
        /** END: Added for DTS2014081101027 by liurongliang(l00180035), 2014/8/1 */

        return NULL;
    }
    /** END:   Added for DTS2014061902682, 2014/6/24 */

    demux->src_pad_mode = GST_PAD_MODE_PUSH;

    GST_INFO("Stream created: CODEC_ID=%d, eMediaType=%d, streamIdx=%d",
        mediainfo->eCodecId, mediainfo->eMediaType, mediainfo->i32StreamIdx);

    return stream;
}

static void
gst_smedmx_ffpro_close (GstSmeDemuxFfPro * demux)
{
    guint Idx;
    GstEvent **event_p;
    const ST_SME_DEMUX_INF* demuxintf = SME_GetDemuxInf();

    GST_INFO("gst_smedmx_ffpro_close in, open=%d", demux->opened);

    if (!demux->opened){
        return;
    }

    /* remove pads from ourselves */
    for (Idx = 0; Idx < M_SME_STREAM_NUM_MAX; Idx++) {
        GstSmeStream *stream;

        stream = demux->streams[Idx];
        if (stream) {
            if (stream->pad){
                stream->b_exit = TRUE;
                gst_element_remove_pad (GST_ELEMENT (demux), stream->pad);
            }
            if (stream->tags)
                gst_tag_list_free (stream->tags);
            if(stream->caps){
                gst_caps_unref(stream->caps);
                stream->caps = NULL;
            }

            g_free (stream);
        }
        demux->streams[Idx] = NULL;
    }
    demux->videopads = 0;
    demux->audiopads = 0;

    /* close demuxer context from sme */
    demuxintf->SME_CloseDemux(demux->demuxer);
    demuxintf->SME_DestroyDemux(demux->demuxer);
    demux->demuxer = NULL;

    /** BEGIN: Added for DTS2014061909146 by liurongliang(l00180035), 2014/6/28 */
    /*memleak*/
    if(NULL != demux->smestreams){
        PST_SME_STREAMINFO pstStreamInfo = demux->smestreams;
        for(Idx = 0; Idx < pstStreamInfo->u32NumStream; Idx++){
            if(NULL == pstStreamInfo->pstMedia[Idx]){
                continue;
            }

            if(pstStreamInfo->pstMedia[Idx]->stCfgData.pu8CfgData){
                free(pstStreamInfo->pstMedia[Idx]->stCfgData.pu8CfgData);
                pstStreamInfo->pstMedia[Idx]->stCfgData.pu8CfgData = NULL;
            }
            pstStreamInfo->pstMedia[Idx]->stCfgData.u32CfgDataSize = 0;
            free((void *)(pstStreamInfo->pstMedia[Idx]));
            pstStreamInfo->pstMedia[Idx] = NULL;
        }
        free((void *)pstStreamInfo);
        demux->smestreams = NULL;
    }
    /** END:   Added for DTS2014061909146 by liurongliang(l00180035), 2014/6/28 */

    GST_OBJECT_LOCK (demux);

    demux->opened = FALSE;

    event_p = &demux->seek_event;
    gst_event_replace (event_p, NULL);

    GST_OBJECT_UNLOCK (demux);

    gst_segment_init (&demux->segment, GST_FORMAT_TIME);

#if defined (SME_FF_PRO_DUMP_ES_VIDEO) || defined (SME_FF_PRO_DUMP_ES_AUDIO)
    // to clean dumping parameter of video/audio ES data
    demux->enable_video_dump = FALSE;
    demux->enable_audio_dump = FALSE;

    if (demux->video_es_file)
    {
        fclose(demux->video_es_file);
        demux->video_es_file = NULL;
    }

    if (demux->audio_es_file)
    {
        fclose(demux->audio_es_file);
        demux->audio_es_file = NULL;
    }

    if (demux->dump_path)
    {
        g_free(demux->dump_path);
        demux->dump_path = NULL;
    }

#endif

    GST_INFO("gst_smedmx_ffpro_close out");
}

// same to ffmpeg: libavformat/avio.h
typedef enum {
    AVIO_CBTYPE_CHECK_INTERRUPT = 0,
    AVIO_CBTYPE_REPORT_EVENT,
} AVIOCBType;

static V_INT32 gst_smedmx_ffpro_iointerrupt_cb_func(void *opaque, V_INT32 callback_type, V_INT32 param1, void *ptr_param)
{
  if (callback_type == AVIO_CBTYPE_CHECK_INTERRUPT)
  {
      GstSmeDemuxFfPro* pst_demux = (GstSmeDemuxFfPro*)opaque;
      gint gi_interrupt = 0;

      gi_interrupt = g_atomic_int_get(&pst_demux->io_interrupt);

      if(0 != gi_interrupt){
          GST_INFO("io interrupt val=%d", gi_interrupt);
      }

      return gi_interrupt;
  }
  else if (callback_type == AVIO_CBTYPE_REPORT_EVENT)
  {
      //return _SVR_FORMAT_EventCB(opaque, param1, (char *)ptr_param);
      return 0;
  }

  return -1;
}

static gboolean
gst_smedmx_ffpro_open (GstSmeDemuxFfPro * demux)
{
    gint res = -1;
    guint n_streams = 0;
    guint Idx = 0, order_idx = 0;
    GstEvent *event = NULL;

    const ST_SME_DEMUX_INF* demuxInf = SME_GetDemuxInf();
    PST_SME_STREAMINFO pstStreamInfo;

    GST_INFO_OBJECT (demux, "gst_smedmx_ffpro_open in");

    /** BEGIN:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    while(!g_rec_mutex_trylock(&demux->st_rec_demux_mutex)){
        if(demux->is_to_deactive){
            GST_INFO_OBJECT (demux, "gst_smedmx_ffpro_open out to deactive!");
            return FALSE;
        }
        GST_WARNING_OBJECT (demux, "gst_smedmx_ffpro_open cannot get mutex,why?");
        g_usleep(2000);
    }
    /** END:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */

    /* to be sure... */
    gst_smedmx_ffpro_close (demux);

    demux->eos = FALSE;

    /** BEGIN:   Modified for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    ST_SME_IOINTERRUPTCB st_io_interrupt_cb;

    st_io_interrupt_cb.callback = gst_smedmx_ffpro_iointerrupt_cb_func;
    st_io_interrupt_cb.opaque = demux;

    demux->demuxer = demuxInf->SME_CreateDemux((const ST_SME_IOINTERRUPTCB*)&st_io_interrupt_cb);
    if(demux->demuxer == NULL)
        goto create_failed;
    /** END:   Modified for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */

    // location is the url:
    if (!demux->play_url) {
        GST_ERROR ("play_url not exist");
        goto open_failed;
    }

    GST_INFO_OBJECT (demux, "location:%p", demux->play_url);

    res = demuxInf->SME_OpenDemux(demux->demuxer, demux->play_url, NULL, NULL);

    if (res < 0)
        goto open_failed;

    pstStreamInfo = (PST_SME_STREAMINFO)malloc(sizeof(ST_SME_STREAMINFO));
    if(NULL == pstStreamInfo)
        goto no_stream;

    if (E_SME_MEM_OK != VOS_Memset_S((void*)pstStreamInfo, sizeof(ST_SME_STREAMINFO), 0, sizeof(ST_SME_STREAMINFO)))
    {
        free((void *)pstStreamInfo);
        goto no_stream;
    }

    res = demuxInf->SME_GetStreamInfo(demux->demuxer, pstStreamInfo);
    if (res != E_SME_DEMUX_RET_SUCCESS)
    {
        for(Idx = 0; Idx < pstStreamInfo->u32NumStream; Idx++){
            if(NULL == pstStreamInfo->pstMedia[Idx]){
                continue;
            }

            if(pstStreamInfo->pstMedia[Idx]->stCfgData.pu8CfgData){
                free(pstStreamInfo->pstMedia[Idx]->stCfgData.pu8CfgData);
                pstStreamInfo->pstMedia[Idx]->stCfgData.pu8CfgData = NULL;
            }
            pstStreamInfo->pstMedia[Idx]->stCfgData.u32CfgDataSize = 0;
            free((void *)(pstStreamInfo->pstMedia[Idx]));
            pstStreamInfo->pstMedia[Idx] = NULL;
        }
        free((void *)pstStreamInfo);
        demux->smestreams = NULL;
        goto no_stream;
    }

    /* save smestreams */
    demux->smestreams = pstStreamInfo;

    //n_streams = pstStreamInfo->u32NumStream;
    GST_INFO_OBJECT (demux,
        "STREAM_info: find %u streams from ffmpeg", pstStreamInfo->u32NumStream);

    gboolean audio_stream_avail_flag = FALSE;
    guint    last_audio_stream_idx = (guint)(-1);
    for (Idx = 0; Idx < pstStreamInfo->u32NumStream; Idx++) {
        GstSmeStream * stream = NULL;
        order_idx = pstStreamInfo->u32IdxOrderInPos[Idx];

        stream = gst_smedmx_ffpro_create_stream (demux,
                                          pstStreamInfo->pstMedia[order_idx]);

        demux->streams[order_idx] = stream;

        if (pstStreamInfo->pstMedia[order_idx] && E_SME_MEDIA_TYPE_AUDIO == pstStreamInfo->pstMedia[order_idx]->eMediaType) {
            last_audio_stream_idx = order_idx;
            if (stream) {
                audio_stream_avail_flag = TRUE; // got valid audio stream
            }
        }

        if (stream) {
            n_streams ++;
        }
    }

    /* exist audio stream, but all the audio codec not supported, then no audio stream is available. */
    if (!audio_stream_avail_flag && (guint)(-1) != last_audio_stream_idx) {
        GST_ELEMENT_WARNING(demux, STREAM, DECODE,
            ("No caps found for audio codecid=%d", (int)(pstStreamInfo->pstMedia[last_audio_stream_idx]->eCodecId)),
            ("audio_dec:a format is not support!"));

        /* post message with audio info for unsupported audio tracks */
        gst_smedmx_ffpro_post_unsupported_audio_track_info(demux, (ST_SME_AUDIOINFO *)pstStreamInfo->pstMedia[last_audio_stream_idx]);
    }

    GST_INFO_OBJECT (demux,
        "STREAM_info: support %u streams by SME", n_streams);

    gst_element_no_more_pads (GST_ELEMENT (demux));

    if(0 == n_streams) {
        if(NULL != demux->smestreams){
            //PST_SME_STREAMINFO pstStreamInfo = demux->smestreams;
            for(Idx = 0; Idx < pstStreamInfo->u32NumStream; Idx++){
                if(NULL == pstStreamInfo->pstMedia[Idx]){
                    continue;
                }

                if(pstStreamInfo->pstMedia[Idx]->stCfgData.pu8CfgData){
                    free(pstStreamInfo->pstMedia[Idx]->stCfgData.pu8CfgData);
                    pstStreamInfo->pstMedia[Idx]->stCfgData.pu8CfgData = NULL;
                }
                pstStreamInfo->pstMedia[Idx]->stCfgData.u32CfgDataSize = 0;
                free((void *)(pstStreamInfo->pstMedia[Idx]));
                pstStreamInfo->pstMedia[Idx] = NULL;
            }
            free((void *)pstStreamInfo);
            demux->smestreams = NULL;
        }
        goto no_stream;
    }

    if(demux->videopads > 1){
        pstStreamInfo->i32Duration = gst_smedmx_ffpro_get_max_video_duration(
                                        (const GstSmeDemuxFfPro * )demux,
                                            pstStreamInfo->u32NumStream);
    }

    if(pstStreamInfo->i32Duration > 0){
        demux->duration = (GstClockTime) gst_util_uint64_scale_int (
            (guint64)(gint64)pstStreamInfo->i32Duration,
            (gint)GST_SECOND,  (gint)GST_USECOND); //AV_TIME_BASE

        GST_INFO("duration=%llu", demux->duration);
    } else {
        GST_INFO("duration is 0, try to get duration later");
    }

    demux->segment.duration = demux->duration;

    GST_OBJECT_LOCK (demux);
    demux->opened = TRUE;
    event = demux->seek_event;
    demux->seek_event = NULL;
    GST_OBJECT_UNLOCK (demux);

    if (event) {
        gst_smedmx_ffpro_perform_seek (demux, event);
        gst_event_unref (event);
    }

    g_rec_mutex_unlock(&demux->st_rec_demux_mutex);

    GST_INFO_OBJECT (demux, "gst_smedmx_ffpro_open out(success)");

    return TRUE;

/* error handle:
 * no stream found, release resource step by step.
 */
no_stream:
    {
        GST_ERROR_OBJECT (demux, "gst_smedmx_ffpro_open out(no stream info)");
        demuxInf->SME_CloseDemux(demux->demuxer);
    }

/* failed to open demuxer */
open_failed:
    {
        GST_ERROR_OBJECT (demux, "gst_smedmx_ffpro_open out(open demuxer failed)");
        demuxInf->SME_DestroyDemux(demux->demuxer);
        demux->demuxer = NULL;

        /*report error*/
        GST_ELEMENT_ERROR(demux, STREAM, FORMAT, ("open stream failed"),
            ("not open"));
    }

/* failed to create demuxer */
create_failed:
    {
        GST_ERROR_OBJECT (demux, "gst_smedmx_ffpro_open out(create demuxer failed)");
    }

    g_rec_mutex_unlock(&demux->st_rec_demux_mutex);
    return FALSE;
}

#if defined (SME_FF_PRO_DUMP_ES_VIDEO)

// create dumping file for video.
// is_seeking:  If seeking or first playing, it is true; else, it is false.
//              When it is true, dumping file will be closed and created again.
static void gst_smedmx_ffpro_dump_video_file_handle (GstSmeDemuxFfPro * demux, gboolean is_seeking)
{
    if (is_seeking) {
        gettimeofday(&demux->demux_proc_info.first_output_i_frame_time, NULL);
    }

    if (demux->enable_video_dump) {
        if (is_seeking && demux->video_es_file) {
            fclose(demux->video_es_file);
            demux->video_es_file = NULL;
        }

        // create dumping file for video.
        if (NULL == demux->video_es_file) {
            V_UINT32 i = 0;
            gchar *dump_file_name = g_strdup_printf ("%s/%s_video_%u", demux->dump_path,
            //gchar *dump_file_name = g_strdup_printf ("/data/dump/%s_video_%u", //demux->dump_path,
                                      GST_ELEMENT_NAME(demux),
                                      demux->video_dump_index);

            if (NULL == dump_file_name) {
                GST_ERROR("g_strdup_printf return NULL");
                return;
            }

            // if file exist, will overwrite.
            demux->video_es_file = fopen(dump_file_name, "wb");

            for (i = 0; i < demux->smestreams->u32NumStream; i ++) {
                if(demux->smestreams->pstMedia[i]
                    && demux->smestreams->pstMedia[i]->eMediaType == E_SME_MEDIA_TYPE_VIDEO) {
                    break;
                }
            }

            // firstly append cfgdata when file created.
            if (i < demux->smestreams->u32NumStream && demux->smestreams->pstMedia[i]->stCfgData.u32CfgDataSize > 0) {
                V_UINT32 len = (V_UINT32)fwrite(demux->smestreams->pstMedia[i]->stCfgData.pu8CfgData, 1,
                    demux->smestreams->pstMedia[i]->stCfgData.u32CfgDataSize,
                    demux->video_es_file);

                if (len != demux->smestreams->pstMedia[i]->stCfgData.u32CfgDataSize) {
                    GST_WARNING("fwrite failed");
                }
            }

            GST_INFO("video dump_file_name=%s created, CfgDataSize=%d written, video_dump_index=%u",
                dump_file_name,
                demux->smestreams->pstMedia[i]->stCfgData.u32CfgDataSize,
                demux->video_dump_index);

            demux->video_dump_index ++;

            g_free(dump_file_name);
        }
    } else if (demux->video_es_file){
        // stop dumping video.
        fclose(demux->video_es_file);
        demux->video_es_file = NULL;

        GST_INFO("video_dump stop ok");
    }
}

// dumping video ES.
//
static void gst_smedmx_ffpro_dump_video (const GstSmeDemuxFfPro * demux, const GstSmeEsQueueItem * st_item_ptr, E_SME_MEDIA_TYPE media_type)
{
    if(demux->enable_video_dump && demux->video_es_file && E_SME_MEDIA_TYPE_VIDEO == media_type) {
        GstMapInfo st_mapinfo_video;
        if (gst_buffer_map(st_item_ptr->gp_buffer, &st_mapinfo_video, GST_MAP_READ)) {
            V_UINT32 len = (V_UINT32)fwrite(st_mapinfo_video.data, 1, st_mapinfo_video.size, demux->video_es_file);

            if (len != (V_UINT32)st_mapinfo_video.size) {
                GST_WARNING("fwrite failed");
            } else {
                GST_INFO("dump_es_video, pts=%lld, frame_size=%u",
                    st_item_ptr->gi64_pts, st_mapinfo_video.size);
            }

            gst_buffer_unmap(st_item_ptr->gp_buffer, &st_mapinfo_video);
        } else {
            GST_ERROR("gst_buffer_map error for dump_es_video");
        }
    }
}

#endif

#if defined (SME_FF_PRO_DUMP_ES_AUDIO)

// create dumping file for audio.
// is_seeking:  If seeking or first playing, it is true; else, it is false.
//              When it is true, dumping file will be closed and created again.
static void gst_smedmx_ffpro_dump_audio_file_handle (GstSmeDemuxFfPro * demux, V_INT32 stream_idx, gboolean is_seeking)
{
    if (demux->enable_audio_dump) {
        if (is_seeking && demux->audio_es_file) {
            fclose(demux->audio_es_file);
            demux->audio_es_file = NULL;
        }

        // create dumping file for audio.
        if (NULL == demux->audio_es_file) {
            //gchar *dump_file_name = g_strdup_printf ("/data/%s_audio_%d_%u",
            gchar *dump_file_name = g_strdup_printf ("%s/%s_audio_%d_%u", demux->dump_path,
                          GST_ELEMENT_NAME(demux),
                          stream_idx,
                          demux->audio_dump_index);

            if (NULL == dump_file_name) {
                GST_ERROR("g_strdup_printf return NULL");
                return;
            }

            demux->audio_es_file = fopen(dump_file_name, "wb");  // if file exist, will overwrite.

            GST_INFO("audio dump_file_name=%s created, audio_dump_index=%u",
                dump_file_name,
                demux->audio_dump_index);

            demux->audio_dump_index ++;

            g_free(dump_file_name);
        }
    } else if (demux->audio_es_file){
        // stop dumping audio.
        fclose(demux->audio_es_file);
        demux->audio_es_file = NULL;

        GST_INFO("audio_dump stop ok");
    }
}

// dumping audio ES.
//
static void gst_smedmx_ffpro_dump_audio (const GstSmeDemuxFfPro * demux, const GstSmeEsQueueItem * st_item_ptr, E_SME_MEDIA_TYPE media_type)
{
    if(demux->enable_audio_dump && demux->audio_es_file && E_SME_MEDIA_TYPE_AUDIO == media_type) {
        GstMapInfo st_mapinfo_audio;
        if (gst_buffer_map(st_item_ptr->gp_buffer, &st_mapinfo_audio, GST_MAP_READ)) {
            V_UINT32 len = (V_UINT32)fwrite(st_mapinfo_audio.data, 1, st_mapinfo_audio.size, demux->audio_es_file);

            if (len != (V_UINT32)st_mapinfo_audio.size) {
                GST_WARNING("fwrite failed");
            } else {
                GST_INFO("dump_es_audio, pts=%lld, frame_size=%u",
                    st_item_ptr->gi64_pts, st_mapinfo_audio.size);
            }

            gst_buffer_unmap(st_item_ptr->gp_buffer, &st_mapinfo_audio);
        } else {
            GST_ERROR("gst_buffer_map error for dump_es_audio");
        }
    }
}

#endif

static void gst_smedmx_ffpro_loop (GstSmeDemuxFfPro * demux)
{
    GstFlowReturn ret = GST_FLOW_CUSTOM_SUCCESS;
    E_SME_DEMUX_RET res = E_SME_DEMUX_RET_SUCCESS;

    GstSmeStream *stream;
    ST_SME_FRAMEBUFFER stSmeFrame;

    GstBuffer *outbuf = NULL;
    GstAllocCtx st_allocator_ctx;
    GstSmeEsQueueItem st_item;

    const ST_SME_DEMUX_INF* demuxInf = SME_GetDemuxInf();

    //create a demuxer and find the stream info.
    if (!demux->opened){
        if (!gst_smedmx_ffpro_open (demux)){
            GST_ERROR_OBJECT (demux, "demuxer open failed");
            ret = GST_FLOW_ERROR;
            goto loop_pause;
        }
    }

    if (E_SME_MEM_OK != VOS_Memset_S(&stSmeFrame, sizeof(stSmeFrame), 0, sizeof(stSmeFrame)))
    {
        GST_ERROR("VOS_Memset_S return failed.");
        return;
    }

    st_allocator_ctx.pst_demux = demux;
    st_allocator_ctx.pst_gstbuffer = NULL;
    st_allocator_ctx.stream = NULL;
    ST_SME_FRAME_ALLOCATOR st_allocator = {&st_allocator_ctx,
                                        gst_smedmx_ffpro_espool_lockbuf,
                                        gst_smedmx_ffpro_espool_unlockbuf};

    stSmeFrame.stAllocator = st_allocator;

    res = demuxInf->SME_ReadFrame(demux->demuxer, -1, &stSmeFrame);

    if (res != E_SME_DEMUX_RET_SUCCESS){
        if(NULL != st_allocator_ctx.pst_gstbuffer){
            gst_buffer_unref(st_allocator_ctx.pst_gstbuffer);
        }

        /* invalid stream: read the data in ffmpeg, but codec of the stream is not supported. */
        if((NULL == st_allocator_ctx.stream) && (E_SME_DEMUX_RET_UN_SUPPORT_STREAM == res)){
            return;
        }

        /* something went wrong... */
        GST_WARNING_OBJECT (demux, "av_read_frame returned %d", res);

        /* handle read_failed case */

        GST_OBJECT_LOCK (demux);

        /* pause appropriatly based on if we are flushing or not */
        if (demux->flushing)
            ret = GST_FLOW_FLUSHING;
        else if (gst_smedmx_ffpro_has_outputted ((const GstSmeDemuxFfPro *)demux)
                 || gst_smedmx_ffpro_is_eos ((const GstSmeDemuxFfPro *)demux)) {
            GST_WARNING_OBJECT (demux, "ReadFrame return EOS");
            ret = GST_FLOW_EOS;
        } else
            ret = GST_FLOW_ERROR;

        GST_OBJECT_UNLOCK (demux);

        goto loop_pause;
    }

    /** BEGIN: Modified for DTS2014071000465 by liurongliang(l00180035), 2014/7/10 */
    if(demux->first_after_seek){
       if(/*(E_SME_MEDIA_TYPE_VIDEO == stSmeFrame.pstMediaFrame->eType)
            && */(E_SME_MEDIA_FRAME_TYPE_IDR
            & (V_UINT32)stSmeFrame.pstMediaFrame->stInfo.eFrameType)){

            demux->first_after_seek = FALSE;

            demux->dmx_new_segment = TRUE; /**< Re-send segment after IDR received,
                                         *   since the segment returned from
                                         *   seek method may not correct when
                                         *   IDR is not found.
                                         */

#if defined (SME_FF_PRO_DUMP_ES_VIDEO)
            gst_smedmx_ffpro_dump_video_file_handle (demux, (gboolean)TRUE);
#endif

#if defined (SME_FF_PRO_DUMP_ES_AUDIO)
            gst_smedmx_ffpro_dump_audio_file_handle (demux, stSmeFrame.pstMediaFrame->i32StreamIdx, (gboolean)TRUE);
#endif

        }else if(E_SME_MEDIA_TYPE_VIDEO == stSmeFrame.pstMediaFrame->eType /*||
                 E_SME_MEDIA_TYPE_AUDIO == stSmeFrame.pstMediaFrame->eType*/){
            //seek 之后若为非关键帧，丢弃
            /**< discard audio buffer too */
            if(NULL != st_allocator_ctx.pst_gstbuffer){
                gst_buffer_unref(st_allocator_ctx.pst_gstbuffer);
            }
            GST_WARNING("not IDR-FRAME after seek!!!");
            return;
        }
    }

    /** END:   Modified for DTS2014071000465 by liurongliang(l00180035), 2014/7/10 */

    //get gst stream
    if (demux->streams[stSmeFrame.pstMediaFrame->i32StreamIdx] != NULL)
    {
        stream = demux->streams[stSmeFrame.pstMediaFrame->i32StreamIdx];
    } else {
        GST_WARNING("no stream found");
        return ;
    }

    outbuf = st_allocator_ctx.pst_gstbuffer;

    if(NULL == outbuf){
        g_usleep(1000);
        return;
    }

    if ((ret = gst_smedmx_ffpro_aggregated_flow (demux)) != GST_FLOW_OK){
        /** BEGIN: Modified for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */
        gst_buffer_unref(st_allocator_ctx.pst_gstbuffer);
        st_allocator_ctx.pst_gstbuffer = NULL;
        /** END: Modified for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */

        GST_WARNING_OBJECT (demux, "no buffer");
        goto loop_pause;
    }

    /* Mark discont */
    if (stream->discont) {
        GST_DEBUG_OBJECT (demux, "marking DISCONT");
        GST_BUFFER_FLAG_SET (outbuf, GST_BUFFER_FLAG_DISCONT);
        stream->discont = FALSE;
    }

#if defined (SME_FF_PRO_DUMP_ES_VIDEO)
    gst_smedmx_ffpro_dump_video_file_handle (demux, FALSE);
#endif

#if defined (SME_FF_PRO_DUMP_ES_AUDIO)
    gst_smedmx_ffpro_dump_audio_file_handle (demux, stSmeFrame.pstMediaFrame->i32StreamIdx, FALSE);
#endif

    GST_INFO("Demux_push_begin:mediatype=%d,"
        " type:%d time=%08lld(%08lld),"
        "duration=%08lld,size=%d,streamduration=%d,"
        "realframeduration=%d,num=%d,den=%d,streamindx=%d",
        stSmeFrame.pstMediaFrame->eType,
        stSmeFrame.pstMediaFrame->stInfo.eFrameType,
        M_SME_TS_RESCALE(stSmeFrame.pstMediaFrame->stInfo.i64Pts, 1,
            stSmeFrame.pstMediaFrame->stInfo.i32TsBaseNum,
            stSmeFrame.pstMediaFrame->stInfo.i32TsBaseDen),
        M_SME_TS_RESCALE(stSmeFrame.pstMediaFrame->stInfo.i64Dts, 1,
            stSmeFrame.pstMediaFrame->stInfo.i32TsBaseNum,
            stSmeFrame.pstMediaFrame->stInfo.i32TsBaseDen),
        M_SME_TS_RESCALE(stSmeFrame.pstMediaFrame->stInfo.u32Duration, 1,
            stSmeFrame.pstMediaFrame->stInfo.i32TsBaseNum,
            stSmeFrame.pstMediaFrame->stInfo.i32TsBaseDen),
        stSmeFrame.pstMediaFrame->u32DataLen,
        stream->mediainfo->i32Duration,
        stSmeFrame.pstMediaFrame->stInfo.u32Duration,
        stSmeFrame.pstMediaFrame->stInfo.i32TsBaseNum,
        stSmeFrame.pstMediaFrame->stInfo.i32TsBaseDen,
        stSmeFrame.pstMediaFrame->i32StreamIdx);

    if(G_UNLIKELY(!GST_PAD_IS_LINKED(demux->streams[stSmeFrame.pstMediaFrame->i32StreamIdx]->pad))){
        gst_buffer_unref(st_allocator_ctx.pst_gstbuffer);
        return;
    }

    if((GST_PAD_MODE_PULL == demux->src_pad_mode)
        || (GST_PAD_MODE_PUSH == demux->src_pad_mode)){
        if (E_SME_MEM_OK != VOS_Memset_S(&st_item, sizeof(st_item), 0, sizeof(st_item)))
        {
            GST_ERROR("VOS_Memset_S return failed !!!");
            gst_buffer_unref(st_allocator_ctx.pst_gstbuffer);
            st_allocator_ctx.pst_gstbuffer = NULL;
            return;
        }

        GST_BUFFER_PTS(outbuf) =  (GstClockTime)M_SME_TS_RESCALE(stSmeFrame.pstMediaFrame->stInfo.i64Pts, M_SME_MS_TO_NS,
                stSmeFrame.pstMediaFrame->stInfo.i32TsBaseNum,
                stSmeFrame.pstMediaFrame->stInfo.i32TsBaseDen);
        GST_BUFFER_DTS(outbuf) = (GstClockTime)M_SME_TS_RESCALE(stSmeFrame.pstMediaFrame->stInfo.i64Dts, M_SME_MS_TO_NS,
                stSmeFrame.pstMediaFrame->stInfo.i32TsBaseNum,
                stSmeFrame.pstMediaFrame->stInfo.i32TsBaseDen);
        GST_BUFFER_DURATION (outbuf) = (GstClockTime)M_SME_TS_RESCALE(stSmeFrame.pstMediaFrame->stInfo.u32Duration, M_SME_MS_TO_NS,
                stSmeFrame.pstMediaFrame->stInfo.i32TsBaseNum,
                stSmeFrame.pstMediaFrame->stInfo.i32TsBaseDen);

        st_item.gu32_size = stSmeFrame.pstMediaFrame->u32DataLen;

        //unit: ms
        st_item.gu32_duration = ((stSmeFrame.pstMediaFrame->stInfo.u32Duration) *
           1 *  (guint32)stSmeFrame.pstMediaFrame->stInfo.i32TsBaseNum) /
           (guint32)stSmeFrame.pstMediaFrame->stInfo.i32TsBaseDen;

        st_item.gi64_pts = (stSmeFrame.pstMediaFrame->stInfo.i64Pts *
           1 *  stSmeFrame.pstMediaFrame->stInfo.i32TsBaseNum) /
           stSmeFrame.pstMediaFrame->stInfo.i32TsBaseDen;

        if(stSmeFrame.pstMediaFrame->stInfo.i64Dts >= 0){
            st_item.gi64_dts = (stSmeFrame.pstMediaFrame->stInfo.i64Dts *
               1 *  stSmeFrame.pstMediaFrame->stInfo.i32TsBaseNum) /
               stSmeFrame.pstMediaFrame->stInfo.i32TsBaseDen;
        }else{
            st_item.gi64_dts = -1;
        }

        st_item.gp_buffer = outbuf;

        if(GST_FLOW_OK == stream->last_flow){
            if (demux->dmx_new_segment)
            {
                if(GST_PAD_MODE_PUSH == demux->src_pad_mode){
                    demux->segment.position = GST_BUFFER_PTS(outbuf);
                    demux->segment.time = GST_BUFFER_PTS(outbuf);
                    demux->segment.start = GST_BUFFER_PTS(outbuf);
                    demux->segment.format = GST_FORMAT_TIME;
                    GST_INFO_OBJECT (demux, "Sending segment %" GST_SEGMENT_FORMAT",and start is %lld",
                        &demux->segment, demux->segment.start);
                    gst_smedmx_ffpro_push_event (demux, gst_event_new_segment (&demux->segment));
                    GST_INFO_OBJECT (demux, "Sending segment end!");
                }
                demux->dmx_new_segment = FALSE;
            } else if (stSmeFrame.pstMediaFrame->stInfo.bIsDiscont) {
                if(GST_PAD_MODE_PUSH == demux->src_pad_mode){
                    GST_BUFFER_FLAG_SET (outbuf, GST_BUFFER_FLAG_DISCONT);
                    GST_INFO_OBJECT (demux, "Set DISCONT");

                    demux->segment.position = GST_BUFFER_PTS(outbuf);
                    demux->segment.time = GST_BUFFER_PTS(outbuf);
                    demux->segment.start = GST_BUFFER_PTS(outbuf);
                    demux->segment.format = GST_FORMAT_TIME;
                    GST_INFO_OBJECT (demux, "Sending segment %" GST_SEGMENT_FORMAT",and start is %lld",
                        &demux->segment, demux->segment.start);

                    gst_smedmx_ffpro_push_event (demux, gst_event_new_segment (&demux->segment));
                    GST_INFO_OBJECT (demux, "Sending segment end!");
                }
                stSmeFrame.pstMediaFrame->stInfo.bIsDiscont = FALSE;
            }

            if((GST_BUFFER_PTS(outbuf) < demux->segment.start)
                 && (E_SME_MEDIA_TYPE_AUDIO == stSmeFrame.pstMediaFrame->eType))
            {
                if(NULL != st_allocator_ctx.pst_gstbuffer){
                    gst_buffer_unref(st_allocator_ctx.pst_gstbuffer);
                }
                GST_WARNING("pts is out of segment!");
                return;
            }

            if((E_SME_MEDIA_TYPE_VIDEO == stSmeFrame.pstMediaFrame->eType)
              && (!(E_SME_MEDIA_FRAME_TYPE_IDR & (V_UINT32)stSmeFrame.pstMediaFrame->stInfo.eFrameType))){
                GST_BUFFER_FLAG_SET(st_item.gp_buffer, GST_BUFFER_FLAG_DELTA_UNIT);
            }

#ifndef __SME_BUFFER__
            gsize buffer_offset = G_STRUCT_OFFSET(ST_SME_MEDIA_FRAME, pu8DataBuf);
            gst_buffer_resize(st_item.gp_buffer, buffer_offset, stSmeFrame.pstMediaFrame->u32DataLen);
#endif

#if defined (SME_FF_PRO_DUMP_ES_VIDEO)
            gst_smedmx_ffpro_dump_video (demux, &st_item, stSmeFrame.pstMediaFrame->eType);
#endif

#if defined (SME_FF_PRO_DUMP_ES_AUDIO)
            gst_smedmx_ffpro_dump_audio (demux, &st_item, stSmeFrame.pstMediaFrame->eType);
#endif

            GstSmeStream *s = demux->streams[stSmeFrame.pstMediaFrame->i32StreamIdx];
            if (s && s->pad) {
                if (s->tags_update) {
                    GST_INFO_OBJECT (s, "push tag to downstream");
                    if (s->tags && !gst_pad_push_event(s->pad, gst_event_new_tag(gst_tag_list_ref(s->tags)))) {
                        GST_WARNING("gst_pad_push_event tag_event not handled");
                    }

                    s->tags_update = FALSE;
                }

                // push es to down stream.
                ret = gst_pad_push(s->pad, st_item.gp_buffer);
                if (ret != GST_FLOW_OK) {
                    GST_ERROR("gst_pad_push failed, ret=%d", ret);
                }
            }else{
                GST_WARNING("pad is invalid");
                gst_buffer_unref(st_item.gp_buffer);
            }

        }else{
            gst_buffer_unref(st_allocator_ctx.pst_gstbuffer);
            st_allocator_ctx.pst_gstbuffer = NULL;
        }
    }else{
        GST_ERROR("current src pad mode=%d", demux->src_pad_mode);
        gst_buffer_unref(st_allocator_ctx.pst_gstbuffer);
        st_allocator_ctx.pst_gstbuffer = NULL;
        ret = GST_FLOW_NOT_NEGOTIATED;
    }

    /* if a pad is in e.g. WRONG_STATE, we want to pause to unlock the STREAM_LOCK */
    if(GST_FLOW_FLUSHING == ret){
        goto loop_pause;
    }
    if ((ret != GST_FLOW_OK)
        && ((ret = gst_smedmx_ffpro_aggregated_flow (demux)) != GST_FLOW_OK)) {
        GST_WARNING_OBJECT (demux, "stream_movi flow: %s / %s",
            gst_flow_get_name (stream->last_flow), gst_flow_get_name (ret));
        goto loop_pause;
    }

    return;

loop_pause:
    {
        GST_WARNING_OBJECT(demux, "pausing task, reason %d (%s)",
            ret, gst_flow_get_name (ret));

        /* pause dmx_loop task */
        gst_pad_pause_task (demux->sinkpad);

        /* pause task and make sure loop stops */
        //gst_task_pause (demux->task);
        g_rec_mutex_lock (&demux->task_lock);
        g_rec_mutex_unlock (&demux->task_lock);

        if(GST_FLOW_FLUSHING != ret){
            demux->eos = TRUE;
            gst_smedmx_ffpro_check_src_pad_push_eos(demux);
        }

        return;
    }
}

static gboolean
gst_smedmx_ffpro_query_uri(GstObject * parent)
{
    GstQuery *query = NULL;
    GstSmeDemuxFfPro *demux = (GstSmeDemuxFfPro *) (parent);

    if(!demux->sinkpad)
    {
        GST_WARNING_OBJECT(demux, "sinkpad is not exist!");
        goto error;
    }

    query = gst_query_new_uri ();
    if(!query){
        GST_WARNING_OBJECT(demux, "new query fail");
        goto error;
    }

    if (!gst_pad_peer_query (demux->sinkpad, query)) {
        GST_WARNING_OBJECT(demux, "gst_pad_peer_query fail");
        gst_query_unref (query);
        goto error;
    }

    if (demux->play_url) {
        GST_WARNING("url exist, remove it");
        g_free(demux->play_url);
        demux->play_url = NULL;
    }

    gst_query_parse_uri(query, &demux->play_url);

    gst_query_unref (query);

    GST_INFO_OBJECT(demux,"query location is %p ", demux->play_url);
    return TRUE;

error:
    return FALSE;
}

static gboolean
gst_smedmx_ffpro_sink_activate (GstPad * sinkpad, GstObject* parent)
{
    (void)parent;

    GST_INFO_OBJECT (sinkpad, "activating push");
    return gst_pad_activate_mode (sinkpad, GST_PAD_MODE_PUSH, (gboolean)TRUE);
}

#if 0
static gboolean
gst_smedmx_ffpro_sink_activate_push (const GstPad * sinkpad, GstObject * parent,
        gboolean active)
{
    GstSmeDemuxFfPro *demux;
    gboolean res = FALSE;
    (void)sinkpad;
    demux = (GstSmeDemuxFfPro *) (parent);

    if (active) {
        res = gst_task_start (demux->task);
    } else {
        /* stop the loop task  */
        gst_task_stop (demux->task);
        g_rec_mutex_lock (&demux->task_lock);
        g_rec_mutex_unlock (&demux->task_lock);
        res = gst_task_join (demux->task);
    }

    return res;
}
#endif

static gboolean
gst_smedmx_ffpro_sink_activate_push (GstPad * sinkpad, GstObject * parent,
    gboolean active)
{
    GstSmeDemuxFfPro *demux;
    gboolean res = FALSE;

    demux = (GstSmeDemuxFfPro *) parent;

    if (active) {
        demux->seekable = TRUE;
        res = gst_pad_start_task (sinkpad,
                                (GstTaskFunction) gst_smedmx_ffpro_loop,
                                demux, NULL);
    } else {
        res = gst_pad_stop_task (sinkpad);
        demux->seekable = FALSE;
    }

    return res;
}

static gboolean
gst_smedmx_ffpro_sink_event (GstPad * sinkpad, GstObject * parent,
        GstEvent * event)
{
    (void)parent;

    GST_INFO("gst_smedmx_ffpro_sink_event in(event:%s)", GST_EVENT_TYPE_NAME(event));

    switch (GST_EVENT_TYPE (event)) {
        case GST_EVENT_CAPS: {
                // suppose should not go here.
                GstCaps *caps = NULL;
                gst_event_parse_caps (event, &caps);

                if (caps){
                  gchar* caps_string = NULL;
                  caps_string = gst_caps_to_string (caps);
                  if (NULL != caps_string) {
                      GST_INFO("caps_string=%s", caps_string);
                      g_free(caps_string);
                  }
                }

                gst_event_unref (event);
                goto done;
            }
        default: {
                /* eat this event for now,
                 * ignore the event sent from down stream.
                 */
                gst_event_unref (event);
                goto done;
            }
    }

done:
    return TRUE;
}

static GstFlowReturn
gst_smedmx_ffpro_chain (GstPad * sinkpad, GstObject * parent, GstBuffer * buffer)
{
    (void)sinkpad;
    (void)parent;

    GST_INFO ("ignore received buffer of %" G_GSIZE_FORMAT " bytes",
            gst_buffer_get_size (buffer));

    // ignore the data from up stream.
    if (buffer)
        gst_buffer_unref (buffer);

    return GST_FLOW_OK;
}


static gboolean
gst_smedmx_ffpro_sink_activate_mode (GstPad * sinkpad, GstObject * parent,
    GstPadMode mode, gboolean active)
{
    gboolean res = FALSE;

    GST_INFO_OBJECT(sinkpad, "gst_smedmx_ffpro_sink_activate_mode in(mode:%d, active:%d)",
        mode, active);

    switch (mode) {
        case GST_PAD_MODE_PUSH:
            if (active) {
                res = gst_smedmx_ffpro_query_uri(parent);
                if (!res) {
                    GST_ERROR("gst_smedmx_ffpro_query_uri failed");
                    break;
                }
            }

            res = gst_smedmx_ffpro_sink_activate_push (sinkpad, parent, active);
            break;
        case GST_PAD_MODE_PULL:
            GST_WARNING("not support now");
            break;
        default:
            res = FALSE;
            break;
    }

    GST_INFO_OBJECT(sinkpad, "gst_smedmx_ffpro_sink_activate_mode out");

    return res;
}

static GstStateChangeReturn
gst_smedmx_ffpro_change_state (GstElement * element, GstStateChange transition)
{
    GstSmeDemuxFfPro *demux = (GstSmeDemuxFfPro *) (element);
    GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;

    GST_INFO ("gstsmedmx_ffpro chage_state in:%s to %s,",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)));

    switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:{
        /** BEGIN: Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
        demux->is_to_deactive = TRUE;
        g_atomic_int_set(&demux->io_interrupt, 1);
        g_rec_mutex_lock(&demux->st_rec_demux_mutex);
        /** END:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
        break;
    }
    /** BEGIN: Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    case GST_STATE_CHANGE_READY_TO_PAUSED:
        g_atomic_int_set(&demux->io_interrupt, 0);
        demux->is_to_deactive = FALSE;
        /** BEGIN: Added for cannot play mkv file by liurongliang(l00180035), 2014/8/18 */
        demux->first_after_seek = TRUE;
        /** END:   Added for cannot play mkv file by liurongliang(l00180035), 2014/8/18 */
        break;
    /** END:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    case GST_STATE_CHANGE_NULL_TO_READY:
        demux->first_after_seek = FALSE;
        break;
    default:
        break;
    }

    GST_INFO ("gstsmedmx_ffpro chage_state:%s to %s,begin...",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)));

    ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

    GST_INFO ("gstsmedmx_ffpro chage_state:%s to %s,end,ret=%s",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)),
        gst_element_state_change_return_get_name(ret));

    switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:
        gst_smedmx_ffpro_close (demux);
        g_rec_mutex_unlock(&demux->st_rec_demux_mutex);
        break;
    default:
        break;
    }

    GST_INFO ("gstsmedmx_ffpro chage_state out:%s to %s,",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)));

    return ret;
}

gboolean
gst_smedmx_ffpro_register (GstPlugin * plugin)
{
    GType type;
    gchar *type_name;
    GTypeInfo typeinfo = {
        sizeof (GstSmeDemuxFfProClass),
        (GBaseInitFunc) gst_smedmx_ffpro_base_init,
        NULL,
        (GClassInitFunc) gst_smedmx_ffpro_class_init,
        NULL,
        NULL,
        sizeof (GstSmeDemuxFfPro),
        0,
        (GInstanceInitFunc) gst_smedmx_ffpro_init,
        NULL,
    };

    GST_INFO ("gst_smedmx_ffpro_register in");

    type_name = g_strdup("smedmx_ffpro_ffmpeg");
    if(NULL == type_name)
    {
        GST_WARNING ("Register failed for g_strdup failed");
        return FALSE;
    }

    if (g_type_from_name (type_name)) {
        GST_WARNING ("type_name=%s exist, register failed", type_name);

        g_free (type_name);
        return FALSE;
    }

    type = g_type_register_static (GST_TYPE_ELEMENT, type_name, &typeinfo, (GTypeFlags)0);

    if (!gst_element_register (plugin, type_name, GST_RANK_PRIMARY, type)) {
        GST_WARNING ("Register of type smedmx_ffpro_ffmpeg failed");

        g_free (type_name);
        return FALSE;
    }

    g_free (type_name);

    GST_INFO ("gst_smedmx_ffpro_register out");

    return TRUE;
}

static gpointer
gst_smedmx_ffpro_espool_lockbuf(gpointer gp_ctx, guint gu32_size, gint i32_stream_index)
{
    GstAllocCtx* pst_allocator = (GstAllocCtx*)gp_ctx;
    gboolean b_res = FALSE;

    g_return_val_if_fail(G_UNLIKELY(NULL != pst_allocator), NULL);
    g_return_val_if_fail(G_UNLIKELY(NULL != pst_allocator->pst_demux), NULL);
    g_return_val_if_fail(G_UNLIKELY((i32_stream_index >=0) && (i32_stream_index < M_SME_STREAM_NUM_MAX)), NULL);

    pst_allocator->stream = NULL;

    if(G_UNLIKELY(!pst_allocator->pst_demux->streams[i32_stream_index])){
        return NULL;
    }

    pst_allocator->stream = pst_allocator->pst_demux->streams[i32_stream_index];
    pst_allocator->pst_gstbuffer = gst_buffer_new_and_alloc(gu32_size);

    g_return_val_if_fail(G_UNLIKELY(NULL != pst_allocator->pst_gstbuffer), NULL);

    if(NULL != pst_allocator->pst_gstbuffer->pool){
        gst_object_unref(pst_allocator->pst_gstbuffer->pool);
        pst_allocator->pst_gstbuffer->pool = NULL;
    }

    if (E_SME_MEM_OK != VOS_Memset_S(&pst_allocator->st_info, sizeof(pst_allocator->st_info), 0, sizeof(pst_allocator->st_info)))
    {
        gst_buffer_unref(pst_allocator->pst_gstbuffer);
        pst_allocator->pst_gstbuffer = NULL;
        GST_ERROR("VOS_Memset_S return failed !!!");
        return NULL;
    }

    b_res = gst_buffer_map(pst_allocator->pst_gstbuffer, &pst_allocator->st_info,
        (GstMapFlags)GST_LOCK_FLAG_WRITE);

    if(G_UNLIKELY(FALSE == b_res)){
        gst_buffer_unref(pst_allocator->pst_gstbuffer);
        pst_allocator->pst_gstbuffer = NULL;
        GST_ERROR_OBJECT(pst_allocator, "map failed");
        return NULL;
    }

    return pst_allocator->st_info.data;
}

static void
gst_smedmx_ffpro_espool_unlockbuf(gpointer gp_ctx, gpointer gp_buffer)
{
    GstAllocCtx* pst_allocator = (GstAllocCtx*)gp_ctx;

    (void)gp_buffer;

    g_return_if_fail(G_UNLIKELY(NULL != pst_allocator));
    g_return_if_fail(G_UNLIKELY(NULL != pst_allocator->pst_gstbuffer));

    gst_buffer_unmap(pst_allocator->pst_gstbuffer, &pst_allocator->st_info);

    return;
}

static GstCaps *
gst_smedmx_ffpro_query_srcpad_caps (const GstSmeDemuxFfPro * demux, GstPad * pad, const GstCaps * filter)
{
    GstCaps *caps = NULL;
    gboolean fixed;

    (void)demux;
    (void)filter;

    fixed = GST_PAD_IS_FIXED_CAPS (pad);

    if (fixed) {
    /* if we are operating in pull mode or fixed caps, we only accept the
     * currently negotiated caps */
        caps = gst_pad_get_current_caps (pad);
        gchar* caps_tostring = gst_caps_to_string(caps);
        GST_INFO("gst_smedmx_ffpro_query_srcpad_caps fixed, caps=%s", caps_tostring);
        g_free(caps_tostring);

    }else{
        GST_INFO("gst_smedmx_ffpro_query_srcpad_caps not fixed");
    }

    return caps;
}

static void
gst_smedmx_ffpro_check_src_pad_push_eos(GstSmeDemuxFfPro* demux)
{
    guint idx = 0;

    g_return_if_fail(demux->smestreams);

    for(idx = 0; idx < demux->smestreams->u32NumStream; idx++){
        if(G_UNLIKELY(NULL == demux->streams[idx])){
            continue;
        }

        if(G_UNLIKELY(NULL == demux->streams[idx]->pad)){
            continue;
        }

        GST_INFO_OBJECT (demux, "PAD(%s) push eos",
            GST_PAD_NAME(demux->streams[idx]->pad));

        GstEvent* evt_eos = gst_event_new_eos ();
        if(NULL != evt_eos){
            if(!gst_pad_push_event (demux->streams[idx]->pad, evt_eos)){
                /** evt_eos has been freed in gst_pad_push_event */
                //gst_event_unref(evt_eos);
            }
        }
    }

    return;
}

static void
gst_smedmx_ffpro_set_property (GObject *object, guint prop_id,
    const GValue *value,  GParamSpec *pspec)
{
    GstSmeDemuxFfPro* demux = (GstSmeDemuxFfPro*)object;

    (void)pspec;

    switch (prop_id) {
        case PROP_EXIT_BLOCK:{
            gint exit_block = g_value_get_int(value);
            if(exit_block){
                if (demux->demuxer) {
                    const ST_SME_DEMUX_INF* demuxInf = SME_GetDemuxInf();
                    demuxInf->SME_StopRead(demux->demuxer);
                    GST_WARNING("stop reading data");
                } else {
                    GST_ERROR("no demuxer, why?");
                }

                g_atomic_int_set(&demux->io_interrupt, 1);
            }else{
                g_atomic_int_set(&demux->io_interrupt, 0);
            }
            GST_INFO("set smedmx_ffpro exit block=%d ", exit_block);

            break;
        }
        case PROP_BENCHMARK:
        {
            GST_INFO("set BENCHMARK to demux, unsupport now");
            break;
        }
#if defined (SME_FF_PRO_DUMP_ES_VIDEO) || defined (SME_FF_PRO_DUMP_ES_AUDIO)
        case PROP_SME_DEMUX_INFO:
        {
            ST_SMEDEMUX_PROC_INFO *pstDemuxProcInfo;
            ST_SMEDEMUX_CMD_INFO *pstDemuxCmdInfo;
            E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;

            pstDemuxProcInfo = g_value_get_pointer(value);

            if(pstDemuxProcInfo && pstDemuxProcInfo->eProcCmdType == E_SME_PROC_CMD_SHOW_INFO)
            {
                ST_SMEDEMUX_SHOW_INFO *pstDemuxShowInfo = &(pstDemuxProcInfo->stShowInfo);
                eMemSecRet = VOS_Memcpy_S(pstDemuxShowInfo,
                                          sizeof(ST_SMEDEMUX_SHOW_INFO),
                                          &demux->demux_proc_info,
                                          sizeof(demux->demux_proc_info));
                if (eMemSecRet != E_SME_MEM_OK)
                    GST_WARNING("VOS_Memset_S collect smedmx_ffpro info failed");
                GST_INFO("Collect sme demux info for proc");
            }
            else if (pstDemuxProcInfo && pstDemuxProcInfo->eProcCmdType == E_SME_PROC_CMD_SET_CMD)
            {
                pstDemuxCmdInfo = &(pstDemuxProcInfo->stCmdInfo);

                GST_INFO("Sme Demux bDumpSrcAudioStream=%d", pstDemuxCmdInfo->bDumpSrcAudioStream);
                GST_INFO("Sme Demux bDumpSrcVideoStream=%d", pstDemuxCmdInfo->bDumpSrcVideoStream);
                if (pstDemuxCmdInfo->bDumpSrcAudioStream != (V_INT8)0XFF) {
                    if (pstDemuxCmdInfo->bDumpSrcAudioStream == 1)
                    {
                        //begin to start dump audio data;
                        demux->enable_audio_dump = TRUE;

                        GST_INFO("Sme Demux cDumpFileStorePath=%s", pstDemuxCmdInfo->cDumpFileStorePath);

                        if (demux->dump_path)
                            g_free(demux->dump_path);
                        demux->dump_path = g_strdup(pstDemuxCmdInfo->cDumpFileStorePath);
                    }
                    else if(pstDemuxCmdInfo->bDumpSrcAudioStream == 0)
                    {
                        //stop dump audio data
                        demux->enable_audio_dump = FALSE;
                    }
                }

                if (pstDemuxCmdInfo->bDumpSrcVideoStream != (V_INT8)0xFF) {
                    if (pstDemuxCmdInfo->bDumpSrcVideoStream == 1)
                    {
                        //begin to start dump video data;
                        demux->enable_video_dump = TRUE;

                        GST_INFO("Sme Demux cDumpFileStorePath=%s", pstDemuxCmdInfo->cDumpFileStorePath);

                        if (demux->dump_path)
                            g_free(demux->dump_path);
                        demux->dump_path = g_strdup(pstDemuxCmdInfo->cDumpFileStorePath);
                    }
                    else if(pstDemuxCmdInfo->bDumpSrcVideoStream == 0)
                    {
                        //stop dump video data
                        demux->enable_video_dump = FALSE;
                    }
                }
            }
            GST_INFO("get smedmx_ffpro info from demux");
            break;
        }
#endif
        default:
            GST_WARNING("gstsmedmx_ffpro not support setproperty=%d", prop_id);
            break;
    }

    return;
}

static void gst_smedmx_ffpro_get_property (GObject *object, guint prop_id,
    GValue *value, GParamSpec *pspec)
{
    GstSmeDemuxFfPro* demux = (GstSmeDemuxFfPro*)object;

    (void)pspec;

    switch (prop_id) {
        case PROP_STREAM_BPS:
        {
            guint u_stream_bps = (guint)-1;

            if(demux->opened){
                if((NULL != demux->smestreams) && (demux->smestreams->u32BitRate > 0))
                {
                    u_stream_bps = demux->smestreams->u32BitRate;
                }
            }
            g_value_set_uint(value, u_stream_bps);

            //GST_INFO("get stream bps=%u bps", u_stream_bps);

            break;
        }

        default:
            GST_WARNING("gstsmedmx_ffpro not support getproperty=%d", prop_id);
            break;
    }

    return;
}


/* post message to up layer about the audio track information */
static gboolean
gst_smedmx_ffpro_audio_tag_list(GstSmeDemuxFfPro *demux,
                            GstSmeStream *stream)
{
    ST_SME_MEDIAINFO *mediainfo;
    ST_SME_AUDIOINFO *audioinfo = NULL;
    GstTagList *tag_list = NULL;

    g_return_val_if_fail(G_UNLIKELY(NULL != demux), FALSE);
    g_return_val_if_fail(G_UNLIKELY(NULL != stream), FALSE);

    GST_INFO_OBJECT(demux, "audio track info");

    mediainfo = stream->mediainfo;
    audioinfo = (ST_SME_AUDIOINFO *)mediainfo;

    GST_INFO_OBJECT(demux, "audio-track-info: stream-index=%d, "
                    "audio-type=%s, audio-bitrate=%d, "
                    "audio-sample-rate=%d, audio-channels=%d, lang %s",
                    mediainfo->i32StreamIdx,
                    mediainfo->acMime,
                    mediainfo->u32Bps,
                    audioinfo->u32SampleRate,
                    audioinfo->u32Channels,
                    audioinfo->cLang);

    tag_list = gst_tag_list_new_empty ();
    if (tag_list == NULL)
    {
        GST_ERROR("fail to allocate tag list");
        return FALSE;
    }

    gst_tag_list_add (tag_list, GST_TAG_MERGE_REPLACE,
                      GST_TAG_BITRATE,
                      mediainfo->u32Bps,
                      GST_TAG_AUDIO_SAMPLERATE,
                      audioinfo->u32SampleRate,
                      GST_TAG_AUDIO_CHANNELNUM,
                      audioinfo->u32Channels,
                      GST_TAG_AUDIO_CODEC,
                      gst_smedmx_ffpro_getaudiomime(mediainfo),
                      GST_TAG_LANGUAGE_NAME,
                      audioinfo->cLang,
                      NULL);

    if (stream->tags)
        gst_tag_list_free (stream->tags);
    stream->tags = tag_list;
    stream->tags_update = TRUE;

    return TRUE;
}

static gboolean
gst_smedmx_ffpro_post_unsupported_audio_track_info(GstSmeDemuxFfPro *demux,
                                               const ST_SME_AUDIOINFO *audioinfo)
{
    GstStructure *s;
    GstMessage *m;

    g_return_val_if_fail(G_UNLIKELY(NULL != demux), FALSE);
    g_return_val_if_fail(G_UNLIKELY(NULL != audioinfo), FALSE);

    GST_INFO_OBJECT(demux, "audio-track-info: stream-index=%d, "
                    "audio-type=%s, audio-bitrate=%d, "
                    "audio-sample-rate=%d, audio-channels=%d",
                    audioinfo->stBase.i32StreamIdx,
                    audioinfo->stBase.acMime,
                    audioinfo->stBase.u32Bps,
                    audioinfo->u32SampleRate,
                    audioinfo->u32Channels);


    s = gst_structure_new ("audio-track-info",
                           "audio-track-id", G_TYPE_UINT,
                           audioinfo->stBase.i32StreamIdx,
                           "audio-type", G_TYPE_STRING,
                           gst_smedmx_ffpro_getaudiomime(&(audioinfo->stBase)),
                           "audio-bitrate", G_TYPE_UINT,
                           audioinfo->stBase.u32Bps,
                           "audio-sample-rate", G_TYPE_UINT,
                           audioinfo->u32SampleRate,
                           "audio-channels", G_TYPE_UINT,
                           audioinfo->u32Channels,
                           "audio-lang", G_TYPE_STRING,
                           audioinfo->cLang,
                           NULL);

    if (s == NULL)
    {
        GST_ERROR("fail to create a new structure");
        return FALSE;
    }

    m = gst_message_new_element (GST_OBJECT_CAST (demux), s);

    if (m == NULL)
    {
        GST_ERROR("fail to create a new message");
        return FALSE;
    }

    return gst_element_post_message (GST_ELEMENT_CAST (demux), m);
}

static gboolean
gst_smedmx_ffpro_post_video_track_info(GstSmeDemuxFfPro *demux,
                                   const ST_SME_VIDEOINFO *videoinfo, V_BOOL is_supported)
{
    GstStructure *s;
    GstMessage *m;
    PST_SME_STREAMINFO pstStreamInfo = demux->smestreams;

    g_return_val_if_fail(G_UNLIKELY(NULL != videoinfo), FALSE);

    GST_INFO_OBJECT(demux, "video_track_info: stream-index=%d, "
                    "video-type=%s, bitrate=%d, w=%d, h=%d, is_supported=%d",
                    videoinfo->stBase.i32StreamIdx,
                    videoinfo->stBase.acMime,
                    videoinfo->stBase.u32Bps,
                    videoinfo->stVideoRes.i32Width,
                    videoinfo->stVideoRes.i32Height,
                    is_supported);

    s = gst_structure_new ("video-track-info",
                           "video-track-id", G_TYPE_UINT,
                           videoinfo->stBase.i32StreamIdx,
                           "video-type", G_TYPE_STRING,
                           videoinfo->stBase.acMime,
                           "video-bitrate", G_TYPE_UINT,
                           videoinfo->stBase.u32Bps,       // notice: not accurate in ffmpeg
                           "media-bitrate", G_TYPE_UINT,
                           pstStreamInfo->u32BitRate,
                           "video-width", G_TYPE_UINT,
                           videoinfo->stVideoRes.i32Width,
                           "video-height", G_TYPE_UINT,
                           videoinfo->stVideoRes.i32Height,
                           "codec-is_supported", G_TYPE_BOOLEAN,
                           is_supported,
                           NULL);

    if (s == NULL)
    {
        GST_ERROR("fail to create a new structure");
        return FALSE;
    }

    m = gst_message_new_element (GST_OBJECT_CAST (demux), s);

    if (m == NULL)
    {
        GST_ERROR("fail to create a new message");
        return FALSE;
    }

    return gst_element_post_message (GST_ELEMENT_CAST (demux), m);
}

static const gchar *gst_smedmx_ffpro_getaudiomime(const ST_SME_MEDIAINFO *mediainfo)
{
    gchar *audiomime = NULL;

    switch(mediainfo->eCodecId)
    {
        case E_SME_MEDIA_CODEC_ID_PCM_S16LE:
            audiomime = "audio/pcm_s16le";
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_S16BE:
            audiomime = "audio/pcm_s16be";
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_U8:
            audiomime = "audio/pcm_u8";
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_MULAW:
            audiomime = "audio/pcm_mulaw";
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_ALAW:
            audiomime = "audio/pcm_alaw";
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_S24LE:
            audiomime = "audio/pcm_s24le";
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_S24BE:
            audiomime = "audio/pcm_s24be";
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_DVD:
            audiomime = "audio/pcm_dvd";
            break;
        case E_SME_MEDIA_CODEC_ID_AMR_NB:
            audiomime = "audio/amr_nb";
            break;
        case E_SME_MEDIA_CODEC_ID_AMR_WB:
            audiomime = "audio/amr_wb";
            break;
        case E_SME_MEDIA_CODEC_ID_MP2:
            audiomime = "audio/mp2";
            break;
        case E_SME_MEDIA_CODEC_ID_MP3:
            audiomime = "audio/mp3";
            break;
        case E_SME_MEDIA_CODEC_ID_AAC:
            audiomime = "audio/aac";
            break;
        case E_SME_MEDIA_CODEC_ID_AC3:
            audiomime = "audio/ac3";
            break;
        case E_SME_MEDIA_CODEC_ID_VORBIS:
            audiomime = "audio/vorbis";
            break;
        case E_SME_MEDIA_CODEC_ID_WMAV1:
            audiomime = "audio/wmav1";
            break;
        case E_SME_MEDIA_CODEC_ID_WMAV2:
            audiomime = "audio/wmav2";
            break;
        case E_SME_MEDIA_CODEC_ID_FLAC:
            audiomime = "audio/flac";
            break;
        case E_SME_MEDIA_CODEC_ID_EAC3:
            audiomime = "audio/eac3";
            break;
        case E_SME_MEDIA_CODEC_ID_DTS:
            audiomime = "audio/dts";
            break;
        case E_SME_MEDIA_CODEC_ID_TRUEHD:
            audiomime = "audio/truehd";
            break;
        case E_SME_MEDIA_CODEC_ID_DRA:
            audiomime = "audio/dra";
            break;
        default:
            break;
    }

    return audiomime;
}

///////////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H

#include "config.h"

//for dynamic load smeplugin
#ifdef GST_PLUGIN_BUILD_STATIC
#undef GST_PLUGIN_BUILD_STATIC
#endif

#endif

#include "gstsme.h"

static gboolean
smedmx_ffpro_plugin_init (GstPlugin * plugin)
{
    GST_INFO("register smedmx_ffpro_plugin begin...");

    //register demux element
    if (!gst_smedmx_ffpro_register (plugin))
        GST_WARNING("register smedmx_ffpro fail.");

    GST_INFO("register smedmx_ffpro_plugin end.");

    return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    smedmx_ffpro,
    "smedmx_ffpro plugin of mediaos. (info about sme: http://www.huawei.com)",
    smedmx_ffpro_plugin_init, VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
