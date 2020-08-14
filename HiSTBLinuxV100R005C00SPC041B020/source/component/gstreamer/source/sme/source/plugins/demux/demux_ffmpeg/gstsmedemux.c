/**************************************************************************//**

  Copyright (C), 2001-2014, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    -gstsmedemux.c
 * @brief    -sme demux element source file
 * @author
 * @date    -2014/3/27
 * @version -V1.00
 *
 * History:
 *
 * Number : V1.00
 * Date   : 2014/3/27
 * Author :
 * Desc   : Created file
 *
******************************************************************************/
#include <string.h>
#include <stdlib.h>

#include "osal_str.h"
#include "osal_mem.h"
#include "gstsmecodecmap.h"
#include "gstsmedemux.h"


//lint -e826 //不等价指针转换，这里为安全转换
//lint -e655 //使用枚举类型进行位操作
//lint -e717 //do{}while(0)
//lint -e826 //不等价指针转换，这里为安全转换
//lint -e801 //goto, 这里可以使用goto,gstreamer里面也是这么使用的。
//lint -e585 (可变参数...报错，为gstreamer中的日志宏，无法修改。)
//lint -e838 //前一次赋值未使用(通常是在变量定义时赋空值，无需修改)
//lint -efunc(818,gst_smedemux_push_event) //参数能声明成const 指针，这里集成基类的，不能修改。
//lint -efunc(818,gst_smedemux_get_stream) //参数能声明成const 指针，这里集成基类的，不能修改。
//lint -efunc(818,gst_smedemux_sink_activate) //参数能声明成const 指针，这里集成基类的，不能修改。
//lint -efunc(818,gst_smedemux_src_getrange, gst_smedemux_chain) //参数能声明成const 指针，这里集成基类的，不能修改。
//lint -esym(749,PROP_0) //重定义宏，这里已经undef了
//lint -esym(749,PROP_LAST) //重定义宏，这里已经undef了
//lint -e64  //赋值类型不匹配，这里都是GstCaps*之间的赋值，没有类型转换
//lint -e570
//lint -e57  //gstreamer do like this.

static void
gst_smedemux_class_init (GstSmeDemuxClass * klass);
static void
gst_smedemux_base_init (GstSmeDemuxClass * klass);
static void
gst_smedemux_init (GstSmeDemux * demux);
static void
gst_smedemux_finalize (GObject * object);
static gboolean
gst_smedemux_sink_activate (GstPad * sinkpad, GstObject * parent);
static gboolean
gst_smedemux_sink_activate_mode (GstPad * sinkpad,
    GstObject * parent, GstPadMode mode, gboolean active);
static void
gst_smedemux_loop (GstSmeDemux * demux);
static gboolean
gst_smedemux_send_event (GstElement * element, GstEvent * event);
static GstStateChangeReturn
gst_smedemux_change_state (GstElement * element, GstStateChange transition);
static gboolean
gst_smedemux_push_event (GstSmeDemux * demux, GstEvent * event);
static gboolean
gst_smedemux_perform_seek (GstSmeDemux * demux, GstEvent * event);

//espool
static gpointer
gst_smedemux_espool_lockbuf(gpointer gp_ctx, guint gu32_size, gint gi32_streamindex);

static void
gst_smedemux_espool_unlockbuf(gpointer gp_ctx, gpointer gp_buffer);

static GstCaps *
gst_smedemux_query_srcpad_caps (const GstSmeDemux * demux, GstPad * pad,
                            const GstCaps * filter);

static void
gst_smedemux_set_property (GObject *object, guint prop_id, const GValue *value,
    GParamSpec *pspec);

static void
gst_smedemux_get_property (GObject *object, guint prop_id, GValue *value,
    GParamSpec *pspec);

#if !defined (SME_MULTI_AUDIO_TRACK)
static V_BOOL
gst_smedemux_have_audio_srcpad_available(const GstSmeDemux * demux);
#endif

static V_BOOL
gst_smedemux_have_video_srcpad_available(const GstSmeDemux * demux);

static V_INT32
gst_smedemux_get_max_video_duration
        (const GstSmeDemux * demux, V_UINT32 u32StreamIdx);

static void
gst_smedemux_check_src_pad_push_eos(GstSmeDemux* demux);

static GstFlowReturn
gst_smedemux_chain (GstPad * sinkpad, GstObject * parent, GstBuffer * buffer);

static gboolean
gst_smedemux_sink_activate_push (const GstPad * sinkpad, GstObject * parent,
    gboolean active);

static gboolean
gst_smedemux_sink_event (GstPad * sinkpad, GstObject * parent,
    GstEvent * event);

static gboolean
gst_smedemux_audio_tag_list(GstSmeDemux *demux,
                            GstSmeStream *stream);
static gboolean
gst_smedemux_post_unsupported_audio_track_info(GstSmeDemux *demux,
                                               const ST_SME_AUDIOINFO *audioinfo);

static gboolean
gst_smedemux_post_video_track_info(GstSmeDemux *demux,
                                   const ST_SME_VIDEOINFO *videoinfo, V_BOOL is_supported);

static const gchar *gst_smedemux_getaudiomime(const ST_SME_MEDIAINFO *mediainfo);

static GstClockTime
gst_smedemux_query_duration(GstSmeDemux * demux);

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
    GstSmeDemux*    pst_demux;
    GstBuffer*      pst_gstbuffer;
    GstMapInfo      st_info;
    GstSmeStream*   stream;
}GstAllocCtx;

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

typedef struct _tagGstSmeEsQueueItem{
    gint64      gi64_pts;
    gint64      gi64_dts;
    guint32     gu32_duration;
    guint       gu32_size;
    gpointer    gp_buffer; //gpv buffer
}GstSmeEsQueueItem;

#define GST_SMEDEMUX_PARAMS_QDATA g_quark_from_static_string("smedemux-params")


static GstElementClass *parent_class = NULL;

static void
gst_smedemux_base_init (GstSmeDemuxClass * klass)
{
    GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
    gchar *p, *name;
    GstCaps *sinkcaps;
    GstPadTemplate *sinktempl, *audiosrctempl, *videosrctempl;
    gchar *longname, *description;

    const ST_SME_DEMUX_INF* demuxintf = SME_GetDemuxInf();
    SMEDEMUXFMT demuxfmt;

    demuxfmt = (SMEDEMUXFMT)
      g_type_get_qdata (G_OBJECT_CLASS_TYPE (klass), GST_SMEDEMUX_PARAMS_QDATA);
    g_assert (demuxfmt != NULL);

    p = name = g_strdup (demuxintf->SME_GetDemuxName(demuxfmt));

    if(!p)
        return;

    while (*p) {
        if (*p == '.' || *p == ',')
          *p = '_';
        p++;
    }

    /* construct the element details struct */
    longname = g_strdup_printf ("Sme %s demuxer", demuxintf->SME_GetDemuxDescription(demuxfmt));
    if (!longname)
    {
        g_free(name);
        return;
    }
    description = g_strdup_printf ("Sme %s demuxer", demuxintf->SME_GetDemuxDescription(demuxfmt));
    if (!description)
    {
        g_free(name);
        g_free(longname);
        return;
    }

    gst_element_class_set_details_simple (element_class, longname,
      "Codec/Demuxer", description,
      "anonymous author");
    g_free (longname);
    g_free (description);

    /* pad templates */
    sinkcaps = gst_sme_formatid_to_caps (name);
    g_free (name);
    if(sinkcaps == NULL)
    {
        GST_WARNING("no sinkcaps gst_sme_formatid_to_caps!");
    }
    gchar* caps_tostring = gst_caps_to_string(sinkcaps);
    GST_INFO("sinkcaps gst_sme_formatid_to_caps=%s", caps_tostring);
    g_free(caps_tostring);
    sinktempl = gst_pad_template_new ("sink",
      GST_PAD_SINK, GST_PAD_ALWAYS, sinkcaps);
    videosrctempl = gst_pad_template_new ("video_%u",
      GST_PAD_SRC, GST_PAD_SOMETIMES, GST_CAPS_ANY);
    audiosrctempl = gst_pad_template_new ("audio_%u",
      GST_PAD_SRC, GST_PAD_SOMETIMES, GST_CAPS_ANY);

    gst_element_class_add_pad_template (element_class, videosrctempl);
    gst_element_class_add_pad_template (element_class, audiosrctempl);
    gst_element_class_add_pad_template (element_class, sinktempl);

    klass->demuxfmt = demuxfmt;
    klass->videosrctempl = videosrctempl;
    klass->audiosrctempl = audiosrctempl;
    klass->sinktempl = sinktempl;
}

static void
gst_smedemux_class_init (GstSmeDemuxClass * klass)
{
    GObjectClass *gobject_class;
    GstElementClass *gstelement_class;

    gobject_class = (GObjectClass *) klass;
    gstelement_class = (GstElementClass *) klass;

    parent_class = g_type_class_peek_parent (klass);

    gobject_class->set_property = gst_smedemux_set_property;
    gobject_class->get_property = gst_smedemux_get_property;
    gobject_class->finalize = GST_DEBUG_FUNCPTR (gst_smedemux_finalize);

    gstelement_class->change_state = gst_smedemux_change_state;
    gstelement_class->send_event = gst_smedemux_send_event;

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
gst_smedemux_init (GstSmeDemux * demux)
{
    GstSmeDemuxClass *oclass =
        (GstSmeDemuxClass *) (G_OBJECT_GET_CLASS (demux));
    gint n;

    GST_INFO("smedemux init name =%s", GST_ELEMENT_NAME(demux));
    demux->sinkpad = gst_pad_new_from_template (oclass->sinktempl, "sink");
    gst_pad_set_activate_function (demux->sinkpad,
      GST_DEBUG_FUNCPTR (gst_smedemux_sink_activate));
    gst_pad_set_activatemode_function (demux->sinkpad,
      GST_DEBUG_FUNCPTR (gst_smedemux_sink_activate_mode));
    gst_element_add_pad (GST_ELEMENT (demux), demux->sinkpad);

    gst_pad_set_event_function (demux->sinkpad,
      GST_DEBUG_FUNCPTR (gst_smedemux_sink_event));
    gst_pad_set_chain_function (demux->sinkpad,
      GST_DEBUG_FUNCPTR (gst_smedemux_chain));

    demux->opened = FALSE;
    demux->demuxer = NULL;
    /** BEGIN: Added by baihaili(b00311320),2015/2/10
    Set flag for fragmented mp4 parsing in order to
    get right duration(dash/mss/etc.)*/
    demux->dash_mode = FALSE;
    /** END:   Added by baihaili(b00311320), 2015/2/10 */

    /* BEGIN: Modified for DTS2015072310170 by lizimian(l00251625), 2015/7/28 */
    demux->hls_mode = FALSE;
    demux->hls_mp3 = FALSE;
    demux->hls_mp3_seek_flag = FALSE;
    /* END:   Modified for DTS2015072310170 by lizimian(l00251625), 2015/7/28 */

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

    demux->task = gst_task_new ((GstTaskFunction) gst_smedemux_loop, demux, NULL);
    g_rec_mutex_init (&demux->task_lock);
    gst_task_set_lock (demux->task, &demux->task_lock);

    /* Modified BEGIN: DTS2014081803342, zhoubing z00213950 2014/08/19 */
    demux->recv_segment = FALSE;
    /* Modified END: DTS2014081803342, zhoubing z00213950 2014/08/19 */

    g_mutex_init (&demux->smepipe.tlock);
    g_cond_init (&demux->smepipe.cond);
    demux->smepipe.adapter = gst_adapter_new ();

#if defined (SME_DUMP_ES_VIDEO) || defined (SME_DUMP_ES_AUDIO)
    if (E_SME_MEM_OK != VOS_Memset_S(&demux->demux_proc_info, sizeof(demux->demux_proc_info), 0, sizeof(demux->demux_proc_info))) {
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
gst_smedemux_finalize (GObject * object)
{
    GstSmeDemux * demux = (GstSmeDemux *)object;

    GST_INFO("smedemux finalize name =%s", GST_ELEMENT_NAME(demux));
    /** BEGIN:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    g_rec_mutex_clear(&demux->st_rec_demux_mutex);
    /** END:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */

    g_mutex_clear (&demux->smepipe.tlock);
    g_cond_clear (&demux->smepipe.cond);
    gst_object_unref (demux->smepipe.adapter);

    (void)gst_task_join (demux->task);
    gst_object_unref (demux->task);
    g_rec_mutex_clear (&demux->task_lock);

    G_OBJECT_CLASS (parent_class)->finalize (object);
}

/* check if all streams are eos */
static gboolean
gst_smedemux_is_eos (const GstSmeDemux * demux)
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
gst_smedemux_has_outputted (const GstSmeDemux * demux)
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
gst_smedemux_send_event (GstElement * element, GstEvent * event)
{
    GstSmeDemux *demux = (GstSmeDemux *) (element);
    gboolean res = TRUE;

    switch (GST_EVENT_TYPE (event)) {
        case GST_EVENT_SEEK:
            GST_OBJECT_LOCK (demux);
            if (!demux->opened) {
                GstEvent **event_p;

                GST_DEBUG_OBJECT (demux, "[smedemux]caching seek event");
                event_p = &demux->seek_event;
                gst_event_replace (event_p, event);
                GST_OBJECT_UNLOCK (demux);

                res = TRUE;
            } else {
                GST_OBJECT_UNLOCK (demux);
                res = gst_smedemux_perform_seek (demux, event);
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
gst_smedemux_aggregated_flow (GstSmeDemux * demux)
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
gst_smedemux_create_padname (const gchar * templ, gint n)
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
gst_smedemux_set_flags (const GstSmeDemux * demux, gboolean discont,
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
gst_smedemux_do_seek (GstSmeDemux * demux, GstSegment * segment, GstSeekFlags flags)
{
    gboolean ret = TRUE;
    gint seekret;
    guint64 target;
    gint32 smetarget;

    const ST_SME_DEMUX_INF* demuxInf = SME_GetDemuxInf();

    GST_INFO_OBJECT (demux, "gst_smedemux_do_seek in");

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

    GST_INFO_OBJECT (demux, "gst_smedemux_do_seek out");

    return ret;

/* ERRORS */
seek_failed:
    {
        GST_WARNING_OBJECT (demux, "seek failed : %d", seekret);
        return FALSE;
    }
}

static gboolean
gst_smedemux_perform_seek (GstSmeDemux * demux, GstEvent * event)
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

    if (!demux->seekable) {
        GST_WARNING_OBJECT (demux, "in push mode; ignoring seek");
        return FALSE;
    }

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
           gst_smedemux_push_event (demux, flush_start_event);
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
    if (flush) {
        GST_INFO_OBJECT (demux, "send flush start to upstream.");
        gst_pad_push_event (demux->sinkpad, gst_event_new_flush_start ());
    }

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
    if (flush) {
        //GST_OBJECT_LOCK (demux);
        //demux->flushing = FALSE;
        //GST_OBJECT_UNLOCK (demux);
        GST_INFO_OBJECT (demux, "send flush stop to upstream.");
        gst_pad_push_event (demux->sinkpad, gst_event_new_flush_stop ((gboolean)TRUE));
    }

    /* do the seek, segment.position contains new position. */
    res = gst_smedemux_do_seek (demux, &seeksegment, flags);

    GST_INFO_OBJECT(demux, "gst_smedemux_do_seek return. res=%d ", res);

    /* Seek succeed, flush downstream, and prepare to continue streaming */
    if (flush) {
        gint n;
        /* send flush stop, peer will accept data and events again. We
        * are not yet providing data as we still have the STREAM_LOCK. */
        GST_INFO_OBJECT (demux, "send flush stop to downstream.");

        GstEvent * flush_stop_event = gst_event_new_flush_stop ((gboolean)TRUE);
        if(NULL != flush_stop_event) {
            gst_smedemux_push_event (demux, flush_stop_event);
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
    gst_smedemux_set_flags ((const GstSmeDemux *)demux, (gboolean)TRUE, (gboolean)FALSE);

    if (flush) {
        /* mark flushing so that the streaming thread can react on it */
        GST_OBJECT_LOCK (demux);
        demux->flushing = FALSE;
        GST_OBJECT_UNLOCK (demux);
    }

    /* and restart the task in case it got paused explicitely or by
    * the FLUSH_START event we pushed out. */
    GST_INFO_OBJECT (demux, "starting demux sinkpad task(%p)", GST_PAD_TASK(demux->sinkpad));
    res = gst_pad_start_task (demux->sinkpad, (GstTaskFunction) gst_smedemux_loop,
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
gst_smedemux_src_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
    GstSmeDemux *demux;
    GstSmeStream *stream;
    gboolean res = TRUE;

    GST_INFO("gst_smedemux_src_event in(event:%s)", GST_EVENT_TYPE_NAME(event));
    GST_INFO("gst_smedemux_src_event in(event seq:%u)", event->seqnum);

    stream = gst_pad_get_element_private (pad);
    if (!stream)
        return FALSE;

    demux = (GstSmeDemux *) parent;

    switch (GST_EVENT_TYPE (event)) {
        case GST_EVENT_SEEK:
            GST_INFO_OBJECT (demux, "case GST_EVENT_SEEK");

            /*目前一次seek动作会收到两次seek event(来自audiosink和videosink)
              两次seek的seqnum相同，需屏蔽第二次*/
            if(event->seqnum != demux->last_seqnum){
                if (!demux->seekable) {
                    res = gst_pad_push_event (demux->sinkpad, event);
                }
                else{
                    res = gst_smedemux_perform_seek (demux, event);
                }
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

    GST_INFO("gst_smedemux_src_event out");

    return res;
}

static gboolean
gst_smedemux_src_query (GstPad * pad, GstObject * parent, GstQuery * query)
{
    GstSmeDemux *demux;
    GstSmeStream *stream;
    gboolean res = FALSE;

    GST_INFO("gst_smedemux_src_query in(query:%s)", GST_QUERY_TYPE_NAME(query));

    stream = gst_pad_get_element_private (pad);
    if (NULL == stream)
        return FALSE;

    demux = (GstSmeDemux *) parent;

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
        GST_INFO("gst_smedemux GST_QUERY_DURATION timeduration=%lld",
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
        gboolean seekable;
        gint64 dur = -1;

        gst_query_parse_seeking (query, &format, NULL, NULL, NULL);
        seekable = demux->seekable;
        if (!gst_pad_query_duration (pad, format, &dur)) {
            /* unlikely that we don't know duration but can seek */
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
        caps = gst_smedemux_query_srcpad_caps ((GstSmeDemux *)demux, pad,
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
        GST_INFO("gst_smedemux_src_query defaut(%#x)", GST_QUERY_TYPE (query));
        /* FIXME : ADD GST_QUERY_CONVERT */
        res = gst_pad_query_default (pad, parent, query);
        break;
    }
    GST_INFO("gst_smedemux_src_query out");

    return res;
}

/* send an event to all the source pads .
 * Takes ownership of the event.
 *
 * Returns FALSE if none of the source pads handled the event.
 */
static gboolean
gst_smedemux_push_event (GstSmeDemux * demux, GstEvent * event)
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
gst_smedemux_have_audio_srcpad_available(const GstSmeDemux * demux)
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
gst_smedemux_have_video_srcpad_available(const GstSmeDemux * demux)
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
gst_smedemux_get_max_video_duration
        (const GstSmeDemux * demux, V_UINT32 u32StreamIdx)
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
gst_smedemux_get_stream (GstSmeDemux * demux, ST_SME_MEDIAINFO *mediainfo)
{
    GstSmeDemuxClass *oclass;
    GstPadTemplate *templ = NULL;
    GstPad *pad = NULL;
    GstCaps *caps = NULL;
    gint num = -1;
    gchar *padname = NULL;
    GstSmeStream *stream = NULL;
    gboolean bres = FALSE;

    oclass = (GstSmeDemuxClass *) G_OBJECT_GET_CLASS (demux);

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
            if (gst_smedemux_have_video_srcpad_available((const GstSmeDemux *)demux)) {
                return NULL;
            }

            break;

        case E_SME_MEDIA_TYPE_AUDIO:
#if !defined (SME_MULTI_AUDIO_TRACK)
            if (gst_smedemux_have_audio_srcpad_available((const GstSmeDemux *)demux)) {
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

                gst_smedemux_post_video_track_info(demux, (ST_SME_VIDEOINFO *)mediainfo, (V_BOOL)FALSE);

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

            gst_smedemux_post_video_track_info(demux, (ST_SME_VIDEOINFO *)mediainfo, (V_BOOL)TRUE);

            break;

        case E_SME_MEDIA_TYPE_AUDIO:
            templ = oclass->audiosrctempl;
            num = demux->audiopads++;

            /* post message to up layer about the audio track information */
            bres = gst_smedemux_audio_tag_list(demux, stream);
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
        gst_smedemux_create_padname (GST_PAD_TEMPLATE_NAME_TEMPLATE (templ),
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

    gst_pad_set_query_function (pad, gst_smedemux_src_query);
    gst_pad_set_event_function (pad, gst_smedemux_src_event);

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
gst_smedemux_close (GstSmeDemux * demux)
{
    guint Idx;
    GstEvent **event_p;
    const ST_SME_DEMUX_INF* demuxintf = SME_GetDemuxInf();

    GST_INFO("gst_smedemux_close in, open=%d", demux->opened);
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

    /** BEGIN: Added by baihaili(b00311320),2015/2/10
    Set flag for fragmented mp4 parsing in order to
    get right duration(dash/mss/etc.)*/
    demux->dash_mode = FALSE;
    /** END:   Added by baihaili(b00311320), 2015/2/10 */

    /* BEGIN: Modified for DTS2015072310170 by lizimian(l00251625), 2015/7/28 */
    demux->hls_mode = FALSE;
    demux->hls_mp3 = FALSE;
    demux->hls_mp3_seek_flag = FALSE;
    /* END:   Modified for DTS2015072310170 by lizimian(l00251625), 2015/7/28 */

    event_p = &demux->seek_event;
    gst_event_replace (event_p, NULL);
    GST_OBJECT_UNLOCK (demux);
    gst_segment_init (&demux->segment, GST_FORMAT_TIME);

#if defined (SME_DUMP_ES_VIDEO) || defined (SME_DUMP_ES_AUDIO)
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

    GST_INFO("gst_smedemux_close out");
}

// same to ffmpeg: libavformat/avio.h
typedef enum {
    AVIO_CBTYPE_CHECK_INTERRUPT = 0,
    AVIO_CBTYPE_REPORT_EVENT,
} AVIOCBType;

static V_INT32 gst_smedemux_iointerrupt_cb_func_ffmpeg(void *opaque, V_INT32 callback_type, V_INT32 param1, void *ptr_param)
{
    if ((AVIOCBType)callback_type == AVIO_CBTYPE_CHECK_INTERRUPT)
    {
        GstSmeDemux* pst_demux = (GstSmeDemux*)opaque;
        gint gi_interrupt = 0;

        gi_interrupt = g_atomic_int_get(&pst_demux->io_interrupt);

        if(0 != gi_interrupt){
            GST_INFO("io interrupt val=%d", gi_interrupt);
        }

        return gi_interrupt;
    }
    else if ((AVIOCBType)callback_type == AVIO_CBTYPE_REPORT_EVENT)
    {
        //return _SVR_FORMAT_EventCB(opaque, param1, (char *)ptr_param);
        (void)param1;
        (void)ptr_param;
        return 0;
    }

    return -1;
}

static gboolean
gst_smedemux_open (GstSmeDemux * demux)
{
    GstSmeDemuxClass *oclass = (GstSmeDemuxClass *) G_OBJECT_GET_CLASS (demux);
    gchar *location = NULL;
    gint res = -1;
    guint n_streams = 0;
    guint Idx = 0, order_idx = 0;
    GstEvent *event = NULL;
    GList *cached_events = NULL;

    const ST_SME_DEMUX_INF* demuxInf = SME_GetDemuxInf();
    PST_SME_STREAMINFO pstStreamInfo;

    GST_INFO_OBJECT (demux, "gst_smedemux_open in");

    /** BEGIN:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    while(!g_rec_mutex_trylock(&demux->st_rec_demux_mutex)){
        if(demux->is_to_deactive){
            GST_INFO_OBJECT (demux, "gst_smedemux_open out to deactive!");
            return FALSE;
        }
        GST_WARNING_OBJECT (demux, "gst_smedemux_open cannot get mutex,why?");
        g_usleep(2000);
    }
    /** END:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    /* to be sure... */
    gst_smedemux_close (demux);

    demux->eos = FALSE;

    /** BEGIN:   Modified for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
    ST_SME_IOINTERRUPTCB st_io_interrupt_cb;

    st_io_interrupt_cb.callback = gst_smedemux_iointerrupt_cb_func_ffmpeg;
    st_io_interrupt_cb.opaque = demux;

    demux->demuxer = demuxInf->SME_CreateDemux((const ST_SME_IOINTERRUPTCB*)&st_io_interrupt_cb);
    if(demux->demuxer == NULL)
        goto create_failed;
    /** END:   Modified for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */

    if(demux->seekable)
        location = g_strdup_printf ("gstreamer://%p", demux->sinkpad);
    else
        location = g_strdup_printf ("gstpipe://%p", &demux->smepipe);
    if (!location)
        goto open_failed;

    GST_INFO_OBJECT (demux, "location:%s", location);

    if(demux->seekable)
        res = demuxInf->SME_OpenDemux(demux->demuxer, location, &gst_protocol, oclass->demuxfmt);
    else
        res = demuxInf->SME_OpenDemux(demux->demuxer, location, &gstpipe_protocol, oclass->demuxfmt);
    g_free (location);
    if (res != E_SME_DEMUX_RET_SUCCESS)
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

    /* 保存smestreams */
    demux->smestreams = pstStreamInfo;

    //n_streams = pstStreamInfo->u32NumStream;
    GST_INFO_OBJECT (demux,
        "STREAM_info: find %u streams from ffmpeg", pstStreamInfo->u32NumStream);

    gboolean audio_stream_avail_flag = FALSE;
    guint    last_audio_stream_idx = (guint)(-1);
    for (Idx = 0; Idx < pstStreamInfo->u32NumStream; Idx++) {
        GstSmeStream * stream = NULL;
        order_idx = pstStreamInfo->u32IdxOrderInPos[Idx];

        stream = gst_smedemux_get_stream (demux,
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

    /* exist audio stream, but no audio stream is available. */
    if (!audio_stream_avail_flag && (guint)(-1) != last_audio_stream_idx) {
        GST_ELEMENT_WARNING(demux, STREAM, DECODE,
            ("No caps found for audio codecid=%d", (int)(pstStreamInfo->pstMedia[last_audio_stream_idx]->eCodecId)),
            ("audio_dec:a format is not support!"));

        /* post message with audio info for unsupported audio tracks */
        gst_smedemux_post_unsupported_audio_track_info(demux, (ST_SME_AUDIOINFO *)pstStreamInfo->pstMedia[last_audio_stream_idx]);
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
        pstStreamInfo->i32Duration = gst_smedemux_get_max_video_duration(
                                        (const GstSmeDemux * )demux,
                                            pstStreamInfo->u32NumStream);
    }
    /** BEGIN: Added by baihaili(b00311320),2015/2/10
    Set flag for fragmented mp4 parsing in order to
    get right duration(dash/mss/etc.)*/
    if((pstStreamInfo->i32Duration > 0)
       && !demux->dash_mode) {
    /** END:   Added by baihaili(b00311320), 2015/2/10 */
        demux->duration = (GstClockTime) gst_util_uint64_scale_int ((guint64)(gint64)pstStreamInfo->i32Duration,
        (gint)GST_SECOND,  (gint)GST_USECOND); //AV_TIME_BASE
    }
    else {
        demux->duration = gst_smedemux_query_duration(demux);
    }

    demux->segment.duration = demux->duration;

    GST_OBJECT_LOCK (demux);
    demux->opened = TRUE;
    event = demux->seek_event;
    demux->seek_event = NULL;
    cached_events = demux->cached_events;
    demux->cached_events = NULL;
    GST_OBJECT_UNLOCK (demux);

    if (event) {
        gst_smedemux_perform_seek (demux, event);
        gst_event_unref (event);
    } else {
#if 0
        if(GST_PAD_MODE_PUSH == demux->src_pad_mode){
            GST_INFO_OBJECT (demux, "Sending segment %" GST_SEGMENT_FORMAT,
                &demux->segment);
            GstEvent * segment_event = gst_event_new_segment (&demux->segment);
            if(NULL != segment_event) {
                gst_smedemux_push_event (demux, segment_event);
            }else {
                goto no_stream;
            }
        }
#endif
    }

    while (cached_events){
        event = cached_events->data;
        GST_INFO_OBJECT (demux, "pushing cached event: %" GST_PTR_FORMAT, event);

        /** BEGIN: Modified for DTS2015042206034  by baihaili(b00311320), 2015/5/29 */
        gst_smedemux_push_event (demux, event);
        /** END:   Modified for DTS2015042206034 by baihaili(b00311320), 2015/5/29 */

        cached_events = g_list_delete_link (cached_events, cached_events);
    }

    g_rec_mutex_unlock(&demux->st_rec_demux_mutex);
    GST_INFO_OBJECT (demux, "gst_smedemux_open out(success)");

    return TRUE;

/* ERRORS */
/* 没有找到媒体流 */
no_stream:
    {
        GST_ERROR_OBJECT (demux, "gst_smedemux_open out(no stream info)");
        demuxInf->SME_CloseDemux(demux->demuxer);
    }
/* 解析器打开失败 */
open_failed:
    {
        GST_ERROR_OBJECT (demux, "gst_smedemux_open out(open demuxer failed)");
        demuxInf->SME_DestroyDemux(demux->demuxer);
        demux->demuxer = NULL;
        /** BEGIN: Added by liurongliang(l00180035), 2014/7/5 */
        /*report error*/
        GST_ELEMENT_ERROR(demux, STREAM, FORMAT, ("open stream failed"),
            ("not open"));
        /** END:   Added by liurongliang(l00180035), 2014/7/5 */
    }
/* 解析器创建失败 */
create_failed:
    {
        GST_ERROR_OBJECT (demux, "gst_smedemux_open out(create demuxer failed)");
        g_rec_mutex_unlock(&demux->st_rec_demux_mutex);
        return FALSE;
    }
}

#define GST_SME_TYPE_FIND_SIZE 4096
#define GST_SME_TYPE_FIND_MIN_SIZE 256
#define GST_SME_TYPE_FIND_MAX_SIZE (2UL<<20)//add

static void
gst_smedemux_type_find (GstTypeFind * tf, gpointer priv)
{
    const guint8 *data;
    SMEDEMUXFMT demuxfmt = (SMEDEMUXFMT) priv;
    gint res = 0;
    guint64 length = 0;
    gchar *fmtname = NULL;

    const ST_SME_DEMUX_INF* demuxintf = SME_GetDemuxInf();

    GST_INFO ("gst_smedemux_type_find in");

    /* We want GST_FFMPEG_TYPE_FIND_SIZE bytes, but if the file is shorter than
    * that we'll give it a try... */
    length = gst_type_find_get_length (tf);

    if (length == 0)
        length = GST_SME_TYPE_FIND_SIZE;
    else
        length = MIN(length, GST_SME_TYPE_FIND_MAX_SIZE);

    /* The ffmpeg typefinders assume there's a certain minimum amount of data
    * and will happily do invalid memory access if there isn't, so let's just
    * skip the ffmpeg typefinders if the data available is too short
    * (in which case it's unlikely to be a media file anyway) */
    if (length < GST_SME_TYPE_FIND_MIN_SIZE) {
        GST_LOG ("not typefinding %" G_GUINT64_FORMAT " bytes, too short", length);
        return;
    }

    fmtname = (gchar *)demuxintf->SME_GetDemuxName(demuxfmt);

    GST_INFO ("typefinding %" G_GUINT64_FORMAT " bytes, name=%s", length, fmtname);
    if (demuxintf->SME_ProbeByData &&
        (data = gst_type_find_peek (tf, (gint64)0, (guint)length)) != NULL) {

        res = demuxintf->SME_ProbeByData(demuxfmt, data, (guint32)length);
        GST_INFO ("name=%s,SME_ProbeByData res=%d", fmtname, res);
        if (res > 0) {
            //coverity modified by f00165155:DEAD_CODE
            //res = MAX (1, res * GST_TYPE_FIND_MAXIMUM / 100);
            res = res * GST_TYPE_FIND_MAXIMUM / 100;
            if(res == GST_TYPE_FIND_MAXIMUM){
                res -= 1;
            }
            /* Restrict the probability for MPEG-TS streams, because there is
             * probably a better version in plugins-base, if the user has a recent
             * plugins-base (in fact we shouldn't even get here for ffmpeg mpegts or
             * mpegtsraw typefinders, since we blacklist them)
             * if (g_str_has_prefix (fmtname, "mpegts"))
             *     res = MIN (res, GST_TYPE_FIND_POSSIBLE);
             */

            GstCaps *sinkcaps = gst_sme_formatid_to_caps (fmtname);
            if(sinkcaps == NULL){
                GST_WARNING ("no caps, gst_smedemux_type_find out, fmtname:%s", fmtname);
            }
            gst_type_find_suggest (tf, (guint)res, sinkcaps);
            gst_caps_unref (sinkcaps);
        }
    }

    GST_INFO ("gst_smedemux_type_find out");
}

static void
gst_smedemux_revise_hls_mp3_seek_pts(const GstSmeDemux * demux, GstBuffer * gst_buffer)
{
    if (demux->hls_mode
        && demux->hls_mp3
        && demux->hls_mp3_seek_flag)
    {
        GST_BUFFER_PTS(gst_buffer) += demux->segment.start ;
        GST_BUFFER_DTS(gst_buffer) += demux->segment.start ;
    }
}

#if defined (SME_DUMP_ES_VIDEO)

// create dumping file for video.
// is_seeking:  If seeking or first playing, it is true; else, it is false.
//              When it is true, dumping file will be closed and created again.
static void gst_smedemux_dump_video_file_handle (GstSmeDemux * demux, gboolean is_seeking)
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
static void gst_smedemux_dump_video (const GstSmeDemux * demux, const GstSmeEsQueueItem * st_item_ptr, E_SME_MEDIA_TYPE media_type)
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

#if defined (SME_DUMP_ES_AUDIO)

// create dumping file for audio.
// is_seeking:  If seeking or first playing, it is true; else, it is false.
//              When it is true, dumping file will be closed and created again.
static void gst_smedemux_dump_audio_file_handle (GstSmeDemux * demux, V_INT32 stream_idx, gboolean is_seeking)
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
static void gst_smedemux_dump_audio (const GstSmeDemux * demux, const GstSmeEsQueueItem * st_item_ptr, E_SME_MEDIA_TYPE media_type)
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

static void gst_smedemux_loop (GstSmeDemux * demux)
{
    GstFlowReturn ret = GST_FLOW_CUSTOM_SUCCESS;
    E_SME_DEMUX_RET res = E_SME_DEMUX_RET_SUCCESS;

    GstSmeStream *stream;
    ST_SME_FRAMEBUFFER stSmeFrame;

    GstBuffer *outbuf = NULL;
    GstAllocCtx st_allocator_ctx;
    GstSmeEsQueueItem st_item;

    const ST_SME_DEMUX_INF* demuxInf = SME_GetDemuxInf();

    //create a demuxer and open
    if (!demux->opened){
        if (!gst_smedemux_open (demux)){
            goto open_failed;
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
                                        gst_smedemux_espool_lockbuf,
                                        gst_smedemux_espool_unlockbuf};

    stSmeFrame.stAllocator = st_allocator;

    res = demuxInf->SME_ReadFrame(demux->demuxer, -1, &stSmeFrame);

    if (res != E_SME_DEMUX_RET_SUCCESS){
        if(NULL != st_allocator_ctx.pst_gstbuffer){
            gst_buffer_unref(st_allocator_ctx.pst_gstbuffer);
        }

        //invalid stream
        if((NULL == st_allocator_ctx.stream) && (E_SME_DEMUX_RET_UN_SUPPORT_STREAM == res)){
            return;
        }

        GST_INFO("duration = %lld; res = %d smepipe.eos=%d, steamduration = %d",
            demux->duration, res, demux->smepipe.eos, demux->smestreams->i32Duration);
        /** BEGIN: Modified for DTS2015041002963 by baihaili(b00311320), 2015/4/10 */
        if(((demux->sinkpad->mode != GST_PAD_MODE_PUSH))
        /** END:   Modified for DTS2015041002963 by baihaili(b00311320), 2015/4/10 */
            || (E_SME_DEMUX_RET_EOS != res)
            || (demux->smepipe.eos)){
                goto read_failed;
        }
        g_usleep(1000);
        return;
    }

    /** BEGIN: Modified for DTS2014071000465 by liurongliang(l00180035), 2014/7/10 */
    if(demux->first_after_seek){
       if(/*(E_SME_MEDIA_TYPE_VIDEO == stSmeFrame.pstMediaFrame->eType)
            && */(E_SME_MEDIA_FRAME_TYPE_IDR
            & (V_UINT32)stSmeFrame.pstMediaFrame->stInfo.eFrameType)){

            demux->first_after_seek = FALSE;

            demux->recv_segment = TRUE; /**< Re-send segment after IDR received,
                                         *   since the segment returned from
                                         *   seek method may not correct when
                                         *   IDR is not found.
                                         */

#if defined (SME_DUMP_ES_VIDEO)
            gst_smedemux_dump_video_file_handle (demux, (gboolean)TRUE);
#endif

#if defined (SME_DUMP_ES_AUDIO)
            gst_smedemux_dump_audio_file_handle (demux, stSmeFrame.pstMediaFrame->i32StreamIdx, (gboolean)TRUE);
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
    stream = gst_smedemux_get_stream (demux,
        demux->smestreams->pstMedia[stSmeFrame.pstMediaFrame->i32StreamIdx]);

    if(NULL == stream){
        return ;
    }

    outbuf = st_allocator_ctx.pst_gstbuffer;

    if(NULL == outbuf){
        g_usleep(1000);
        return;
    }

    if ((ret = gst_smedemux_aggregated_flow (demux)) != GST_FLOW_OK){
        /** BEGIN: Modified for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */
        gst_buffer_unref(st_allocator_ctx.pst_gstbuffer);
        st_allocator_ctx.pst_gstbuffer = NULL;
        /** END: Modified for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */
        goto no_buffer;
    }

    /* Mark discont */
    if (stream->discont) {
        GST_DEBUG_OBJECT (demux, "marking DISCONT");
        GST_BUFFER_FLAG_SET (outbuf, GST_BUFFER_FLAG_DISCONT);
        stream->discont = FALSE;
    }

#if defined (SME_DUMP_ES_VIDEO)
    gst_smedemux_dump_video_file_handle (demux, FALSE);
#endif

#if defined (SME_DUMP_ES_AUDIO)
    gst_smedemux_dump_audio_file_handle (demux, stSmeFrame.pstMediaFrame->i32StreamIdx, FALSE);
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

        gint64 gi64streamduration = (gint64)stream->mediainfo->i32Duration;
        /** BEGIN: Added by baihaili(b00311320),2015/2/10
        Fix:the judgement of "stream->eos" used in gst_sme_stream_is_buf_full
        is not correct if the streaming is fragmented mp4.*/
        // comments: gst_sme_stream_is_buf_full() is deleted for it will never
        // be used while MultiQueue used. 2016/02/01th
        if((gi64streamduration > 0)
            && ((gint64)st_item.gu32_duration + st_item.gi64_pts + 150 >= gi64streamduration)
            && !demux->dash_mode){
        /** END:   Added by baihaili(b00311320), 2015/2/10 */
            stream->eos = TRUE;
        }
        else
        {
            stream->eos = FALSE;
        }

        gst_smedemux_revise_hls_mp3_seek_pts(demux,outbuf);
        st_item.gp_buffer = outbuf;

        if(GST_FLOW_OK == stream->last_flow){
            /* Modified BEGIN: DTS2014081803342, zhoubing z00213950 2014/08/19 */
            if (demux->recv_segment)
            {
                if(GST_PAD_MODE_PUSH == demux->src_pad_mode){
                    demux->segment.position = GST_BUFFER_PTS(outbuf);
                    demux->segment.time = GST_BUFFER_PTS(outbuf);
                    demux->segment.start = GST_BUFFER_PTS(outbuf);
                    demux->segment.format = GST_FORMAT_TIME;
                    GST_INFO_OBJECT (demux, "Sending segment %" GST_SEGMENT_FORMAT",and start is %lld",
                        &demux->segment, demux->segment.start);
                    gst_smedemux_push_event (demux, gst_event_new_segment (&demux->segment));
                    GST_INFO_OBJECT (demux, "Sending segment end!");
                }
                demux->recv_segment = FALSE;
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

                    gst_smedemux_push_event (demux, gst_event_new_segment (&demux->segment));
                    GST_INFO_OBJECT (demux, "Sending segment end!");
                }
                stSmeFrame.pstMediaFrame->stInfo.bIsDiscont = FALSE;
            }
            /* Modified END: DTS2014081803342, zhoubing z00213950 2014/08/19 */
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

#if defined (SME_DUMP_ES_VIDEO)
            gst_smedemux_dump_video (demux, &st_item, stSmeFrame.pstMediaFrame->eType);
#endif

#if defined (SME_DUMP_ES_AUDIO)
            gst_smedemux_dump_audio (demux, &st_item, stSmeFrame.pstMediaFrame->eType);
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
        goto pause;
    }
    if ((ret != GST_FLOW_OK)
        && ((ret = gst_smedemux_aggregated_flow (demux)) != GST_FLOW_OK)) {
        GST_WARNING_OBJECT (demux, "stream_movi flow: %s / %s",
            gst_flow_get_name (stream->last_flow), gst_flow_get_name (ret));
        goto pause;
    }

    return;
/* ERRORS */
pause:
    {
        GST_INFO_OBJECT (demux, "pausing task, reason %d (%s)", ret,
        gst_flow_get_name (ret));
        if (demux->seekable || GST_FLOW_FLUSHING == ret)
              gst_pad_pause_task (demux->sinkpad);
        else {
            GstSmePipe *smepipe = &demux->smepipe;
            GST_SME_PIPE_MUTEX_LOCK (smepipe);
            /* pause task and make sure loop stops */
            gst_task_pause (demux->task);
            g_rec_mutex_lock (&demux->task_lock);
            g_rec_mutex_unlock (&demux->task_lock);
            demux->smepipe.srcresult = ret;
            GST_SME_PIPE_MUTEX_UNLOCK (smepipe);
       }
       if(GST_FLOW_FLUSHING != ret){
           demux->eos = TRUE;
           gst_smedemux_check_src_pad_push_eos(demux);
       }
       return;
    }
open_failed:
    {
        GST_WARNING_OBJECT (demux, "open failed");
        ret = GST_FLOW_ERROR;
        goto pause;
    }
read_failed:
    {
        /* something went wrong... */
        GST_WARNING_OBJECT (demux, "av_read_frame returned %d", res);

        GST_OBJECT_LOCK (demux);
        /* pause appropriatly based on if we are flushing or not */
        if (demux->flushing)
            ret = GST_FLOW_FLUSHING;
        else if (gst_smedemux_has_outputted ((const GstSmeDemux *)demux)
        || gst_smedemux_is_eos ((const GstSmeDemux *)demux)) {
            GST_INFO_OBJECT (demux, "We are EOS");
            ret = GST_FLOW_EOS;
        } else
            ret = GST_FLOW_ERROR;
        GST_OBJECT_UNLOCK (demux);

        goto pause;
    }
no_buffer:
    {
        GST_WARNING_OBJECT (demux, "no buffer");
        goto pause;
    }
}

static GstClockTime
gst_smedemux_query_duration(GstSmeDemux * demux)
{
    GstQuery *query = NULL;
    GstClockTime duration;
    GstFormat eFmt = GST_FORMAT_TIME;

    if(!demux->sinkpad)
    {
        GST_WARNING_OBJECT(demux, "sinkpad is not exist!");
        goto error;
    }

    query = gst_query_new_duration (GST_FORMAT_TIME);
    if(!query){
        GST_WARNING_OBJECT(demux, "new query fail");
        goto error;
    }

    if (!gst_pad_peer_query (demux->sinkpad, query)) {
        gst_query_unref (query);
        goto error;
    }
    gst_query_parse_duration (query, &eFmt, (gint64 *)&duration);
    gst_query_unref (query);
    GST_INFO_OBJECT(demux,"query duration is %lld",duration);
    return duration;
error:
    return GST_CLOCK_TIME_NONE;
}

static gboolean
gst_smedemux_sink_activate (GstPad * sinkpad, GstObject* parent)
{
    GstQuery *query = NULL;
    gboolean pull_mode = FALSE;

    (void)parent;

    query = gst_query_new_scheduling ();

    if (!gst_pad_peer_query (sinkpad, query)) {
        gst_query_unref (query);
        goto activate_push;
    }

    pull_mode = gst_query_has_scheduling_mode (query, GST_PAD_MODE_PULL);
    gst_query_unref (query);

    if (!pull_mode)
        goto activate_push;

    GST_INFO_OBJECT (sinkpad, "activating pull");
    return gst_pad_activate_mode (sinkpad, GST_PAD_MODE_PULL, (gboolean)TRUE);

activate_push:
    {
        GST_INFO_OBJECT (sinkpad, "activating push");
        return gst_pad_activate_mode (sinkpad, GST_PAD_MODE_PUSH, (gboolean)TRUE);
    }
}

/* pull mode:
 * - seekable
 * - use gstreamer protocol, like ffmpeg's file protocol
 * - task driving ffmpeg based on sink pad
 */
static gboolean
gst_smedemux_sink_activate_pull (GstPad * sinkpad, GstObject * parent,
    gboolean active)
{
    GstSmeDemux *demux;
    gboolean res = FALSE;

    demux = (GstSmeDemux *) parent;

    if (active) {
        demux->seekable = TRUE;
        res = gst_pad_start_task (sinkpad,
                                (GstTaskFunction) gst_smedemux_loop,
                                demux, NULL);
    } else {
        res = gst_pad_stop_task (sinkpad);
        demux->seekable = FALSE;
    }

    return res;
}

/** BEGIN: Added by baihaili(b00311320),2015/2/10
Set flag for fragmented mp4 parsing */
static void
gst_smedemux_set_fragment_tag (GstSmeDemux *demux, const GstCaps * caps)
{
    GstStructure *structure = NULL;
    const gchar *variant = NULL;
    gchar* caps_string = NULL;

    structure = gst_caps_get_structure (caps, 0);
    if (structure)
    {
        variant = gst_structure_get_string (structure, "variant");
    }
    if (variant && strstr (variant, "dash-fragmented"))
    {
        GST_INFO_OBJECT (demux, "set smedemux dash mode to TRUE");
        demux->dash_mode = TRUE;
    }
    else if (variant && strstr (variant, "hls-fragmented"))
    {
        GST_INFO_OBJECT (demux, "set smedemux hls mode to TRUE");
        demux->hls_mode = TRUE;
    }

    caps_string = gst_caps_to_string (caps);

    if (NULL != caps_string)
    {
        if (strstr (caps_string, "application/x-id3"))
        {
           GST_INFO_OBJECT (demux, "set smedemux dash mp3 flag to TRUE");
           demux->hls_mp3 = TRUE;
        }
        g_free(caps_string);
    }
    else
    {
        GST_WARNING_OBJECT(demux, "caps_string is NULL");
    }
}
 /** END:   Added by baihaili(b00311320), 2015/2/10 */
static gboolean
gst_smedemux_sink_activate_push (const GstPad * sinkpad, GstObject * parent,
        gboolean active)
{
    GstSmeDemux *demux;
    gboolean res = FALSE;
    (void)sinkpad;
    demux = (GstSmeDemux *) (parent);

    if (active) {
#if 0
        if (demux->can_push == FALSE) {
            GST_WARNING_OBJECT (demux, "Demuxer can't reliably operate in push-mode");
            goto beach;
        }
#endif
        demux->smepipe.eos = FALSE;
        demux->smepipe.flush = FALSE;
        demux->smepipe.srcresult = GST_FLOW_OK;
        demux->smepipe.needed = 0;
        demux->seekable = FALSE;
        res = gst_task_start (demux->task);
    } else {
        GstSmePipe *smepipe = &demux->smepipe;

        /* release chain and loop */
        GST_SME_PIPE_MUTEX_LOCK (smepipe);
        demux->smepipe.srcresult = GST_FLOW_FLUSHING;
        /* end streaming by making ffmpeg believe eos */
        demux->smepipe.eos = TRUE;
        GST_SME_PIPE_SIGNAL (smepipe);
        GST_SME_PIPE_MUTEX_UNLOCK (smepipe);

        /* make sure streaming ends */
        gst_task_stop (demux->task);
        g_rec_mutex_lock (&demux->task_lock);
        g_rec_mutex_unlock (&demux->task_lock);
        res = gst_task_join (demux->task);
        demux->seekable = FALSE;
    }

    return res;
}

static gboolean
gst_smedemux_sink_event (GstPad * sinkpad, GstObject * parent,
        GstEvent * event)
{
    GstSmeDemux *demux;
    GstSmePipe *smepipe;
    gboolean result = TRUE;
    guint32 i = 0;
    const ST_SME_DEMUX_INF* demuxInf = SME_GetDemuxInf();

    demux = (GstSmeDemux *) parent;
    smepipe = &(demux->smepipe);

    GST_INFO("gst_smedemux_sink_event in(event:%s)", GST_EVENT_TYPE_NAME(event));

    switch (GST_EVENT_TYPE (event)) {
        /** BEGIN: Modified for DTS2014072905447 by zhoubing(z00213950), 2014/7/28 */
        case GST_EVENT_FLUSH_START:
            GST_OBJECT_LOCK (demux);
            demux->flushing = TRUE;
            demux->eos = FALSE;
            GST_OBJECT_UNLOCK (demux);

            gst_pad_event_default (sinkpad, parent, event);

            /* now unblock the chain function */
            GST_SME_PIPE_MUTEX_LOCK (smepipe);
            smepipe->srcresult = GST_FLOW_FLUSHING;
            /* Modified BEGIN: DTS2014081803527, zhoubing z00213950 2014/08/19*/
            smepipe->eos = FALSE;
            /* Modified END: DTS2014081803527, zhoubing z00213950 2014/08/19*/
            GST_SME_PIPE_SIGNAL (smepipe);
            GST_SME_PIPE_MUTEX_UNLOCK (smepipe);

            goto done;
        case GST_EVENT_FLUSH_STOP:
            /*1.set pipe flush flag
              2.send pipe signal.
              3.pause sink loop.
              4.clear queue.
            */
            GST_SME_PIPE_MUTEX_LOCK (smepipe);
            smepipe->flush = TRUE;
            GST_SME_PIPE_SIGNAL (smepipe);
            GST_SME_PIPE_MUTEX_UNLOCK (smepipe);

            gst_task_pause (demux->task);
            g_rec_mutex_lock (&demux->task_lock);
            g_rec_mutex_unlock (&demux->task_lock);
            //demuxInf->SME_SeekTo(demux->demuxer, 0, 0, SME_DEMUX_FLUSH_FLAG);

            gst_pad_event_default (sinkpad, parent, event);

            GST_OBJECT_LOCK (demux);
            g_list_foreach (demux->cached_events, (GFunc) gst_mini_object_unref,
                    NULL);
            g_list_free (demux->cached_events);
            GST_OBJECT_UNLOCK (demux);

            demux->first_after_seek = TRUE;
            /* Mark discont on all srcpads and remove eos */
            gst_smedemux_set_flags ((const GstSmeDemux *)demux, (gboolean)TRUE, (gboolean)FALSE);

            for (i = 0; i < M_SME_STREAM_NUM_MAX; ++i) {
                if (demux->streams[i])
                    demux->streams[i]->last_flow = GST_FLOW_OK;
            }

            GST_SME_PIPE_MUTEX_LOCK (smepipe);
            gst_adapter_clear (smepipe->adapter);
            smepipe->flush = FALSE;
            smepipe->srcresult = GST_FLOW_OK;

            //flush the data in ffmpeg and ffmpeg adapter
            demuxInf->SME_Flush(demux->demuxer);

            /* BEGIN: Modified for DTS2015072310170 by lizimian(l00251625), 2015/7/28 */
            if (demux->hls_mode
                && demux->hls_mp3) {
                demux->hls_mp3_seek_flag = TRUE;
            }
            /* END:   Modified for DTS2015072310170 by lizimian(l00251625), 2015/7/28 */

            gst_task_start (demux->task);
            GST_INFO_OBJECT (demux, "loop started");
            GST_SME_PIPE_MUTEX_UNLOCK (smepipe);

            GST_OBJECT_LOCK (demux);
            demux->flushing = FALSE;
            GST_OBJECT_UNLOCK (demux);
            goto done;
            /** END: Modified for DTS2014072905447 by zhoubing(z00213950), 2014/7/28 */
        case GST_EVENT_EOS:
            /* inform the src task that it can stop now */
            GST_SME_PIPE_MUTEX_LOCK (smepipe);
            smepipe->eos = TRUE;
            GST_SME_PIPE_SIGNAL (smepipe);
            GST_SME_PIPE_MUTEX_UNLOCK (smepipe);

            /* eat this event for now, task will send eos when finished */
            gst_event_unref (event);
            goto done;
        case GST_EVENT_SEGMENT:{
            GstSegment segment;

            gst_event_copy_segment (event, &segment);
            /* Modified BEGIN: DTS2014081803342, zhoubing z00213950 2014/08/19 */
            switch (segment.format) {
                case GST_FORMAT_TIME:
                    demux->recv_segment = TRUE;
                    GST_INFO("GST_FORMAT_TIME");
                    break;
                case GST_FORMAT_BYTES:
                    GST_INFO("GST_FORMAT_BYTES");
                    break;//todo
                default:
                    /* invalid format */
                    goto done;
            }

            GST_DEBUG("SEGMENT in time %" GST_SEGMENT_FORMAT,&segment);

            /* and store the values */
            GST_INFO ("demux segment, position:%lld, start:%lld",
                    segment.position, segment.start);
            gst_segment_copy_into (&segment, &demux->segment);
            //break;
            gst_event_unref (event);
            goto done;
            /* Modified END: DTS2014081803342, zhoubing z00213950 2014/08/19 */
        }
        /** BEGIN: Added by baihaili(b00311320),2015/2/10
        Set flag for fragmented mp4 parsing in order to
        get right duration,(dash/mss/etc.)*/
        case GST_EVENT_CAPS:{
          GstCaps *caps = NULL;
          gst_event_parse_caps (event, &caps);
          if (caps){
              gst_smedemux_set_fragment_tag (demux, caps);
          }
          gst_event_unref (event);
          goto done;
        }
        /** END:   Added by baihaili(b00311320), 2015/2/10 */
        default:
            /* for a serialized event, wait until an earlier data is gone,
             * though this is no guarantee as to when task is done with it.
             *
             * If the demuxer isn't opened, push straight away, since we'll
             * be waiting against a cond that will never be signalled. */
            if (GST_EVENT_IS_SERIALIZED (event)) {
                if (demux->opened) {
                    GST_SME_PIPE_MUTEX_LOCK (smepipe);
                    while (!smepipe->needed && !smepipe->eos)
                        GST_SME_PIPE_WAIT (smepipe);
                    GST_SME_PIPE_MUTEX_UNLOCK (smepipe);
                } else {
                    /* queue events and send them later (esp. tag events) */
                    GST_OBJECT_LOCK (demux);
                    demux->cached_events = g_list_append (demux->cached_events, event);
                    GST_OBJECT_UNLOCK (demux);
                    goto done;
                }
            }
            break;
    }

    result = gst_pad_event_default (sinkpad, parent, event);

done:
    return result;
}

static GstFlowReturn
gst_smedemux_chain (GstPad * sinkpad, GstObject * parent, GstBuffer * buffer)
{
    GstSmeDemux *demux;
    GstSmePipe *smepipe;

    (void)sinkpad;
    demux = (GstSmeDemux *) parent;
    smepipe = &demux->smepipe;

    GST_SME_PIPE_MUTEX_LOCK (smepipe);

    if (G_UNLIKELY (smepipe->eos))
        goto eos;

    if (G_UNLIKELY (smepipe->srcresult != GST_FLOW_OK))
        goto ignore;

    GST_DEBUG ("Giving a buffer of %" G_GSIZE_FORMAT " bytes",
            gst_buffer_get_size (buffer));
    gst_adapter_push (smepipe->adapter, buffer);
    buffer = NULL;
    while (gst_adapter_available (smepipe->adapter) >= smepipe->needed
        && !smepipe->eos) {
        GST_DEBUG ("Adapter has more than requested (smepipe->needed:%d)",
                smepipe->needed);
        GST_SME_PIPE_SIGNAL (smepipe);
        GST_SME_PIPE_WAIT (smepipe);
        /* may have become flushing */
        if (G_UNLIKELY (smepipe->srcresult != GST_FLOW_OK)){
            GST_INFO_OBJECT (demux, "smepipe->srcresult = %d",
                smepipe->srcresult);
            goto ignore;
        }
    }
    GST_SME_PIPE_MUTEX_UNLOCK (smepipe);
    GST_DEBUG ("Giving a buffer end");

    return GST_FLOW_OK;

    /* special cases */
eos:
    {
        GST_INFO_OBJECT (demux, "ignoring buffer at end-of-stream");
        GST_SME_PIPE_MUTEX_UNLOCK (smepipe);

        gst_buffer_unref (buffer);
        return GST_FLOW_EOS;
    }
ignore:
    {
        GST_INFO_OBJECT (demux, "ignoring buffer because src task encountered %s",
                gst_flow_get_name (smepipe->srcresult));
        GST_SME_PIPE_MUTEX_UNLOCK (smepipe);

        if (buffer)
            gst_buffer_unref (buffer);
        return GST_FLOW_FLUSHING;
    }
}


static gboolean
gst_smedemux_sink_activate_mode (GstPad * sinkpad, GstObject * parent,
    GstPadMode mode, gboolean active)
{
    gboolean res = FALSE;

    GST_INFO_OBJECT(sinkpad, "gst_smedemux_sink_activate_mode in(mode:%d, active:%d)",
        mode, active);

    switch (mode) {
    case GST_PAD_MODE_PUSH:
        res = gst_smedemux_sink_activate_push ((const GstPad*)sinkpad, parent, active);
        break;
    case GST_PAD_MODE_PULL:
        res = gst_smedemux_sink_activate_pull (sinkpad, parent, active);
        break;
    default:
        res = FALSE;
        break;
    }

    GST_INFO_OBJECT(sinkpad, "gst_smedemux_sink_activate_mode out");

    return res;
}

static GstStateChangeReturn
gst_smedemux_change_state (GstElement * element, GstStateChange transition)
{
    GstSmeDemux *demux = (GstSmeDemux *) (element);
    GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;
    GstSmePipe *smepipe;
    smepipe = &demux->smepipe;

    GST_INFO ("gstsmedemux chage_state in:%s to %s,",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)));

    switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:{
        /** BEGIN: Added for DTS2015102107790  by baihaili(b00311320), 2015/10/22 */
        GST_SME_PIPE_MUTEX_LOCK (smepipe);
        smepipe->eos = TRUE;
        GST_SME_PIPE_SIGNAL (smepipe);
        GST_SME_PIPE_MUTEX_UNLOCK (smepipe);
        /** END:   Added for DTS2015102107790  by baihaili(b00311320), 2015/10/22 */

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
        demux->smepipe.eos = FALSE;
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


    GST_INFO ("gstsmedemux chage_state:%s to %s,begin...",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)));

    ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

    GST_INFO ("gstsmedemux chage_state:%s to %s,end,ret=%s",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)),
        gst_element_state_change_return_get_name(ret));

    switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:
        gst_smedemux_close (demux);
        gst_adapter_clear (demux->smepipe.adapter);
        g_list_foreach (demux->cached_events, (GFunc) gst_mini_object_unref, NULL);
        g_list_free (demux->cached_events);
        demux->cached_events = NULL;
        g_rec_mutex_unlock(&demux->st_rec_demux_mutex);
        break;
    default:
        break;
    }

    GST_INFO ("gstsmedemux chage_state out:%s to %s,",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)));

    return ret;
}

gboolean
gst_smedemux_register (GstPlugin * plugin)
{
    GType type;

    CSMEDEMUXFMT demuxfmt;
    GTypeInfo typeinfo = {
        sizeof (GstSmeDemuxClass),
        (GBaseInitFunc) gst_smedemux_base_init,
        NULL,
        (GClassInitFunc) gst_smedemux_class_init,
        NULL,
        NULL,
        sizeof (GstSmeDemux),
        0,
        (GInstanceInitFunc) gst_smedemux_init,
        NULL,
    };

    const ST_SME_DEMUX_INF* demuxintf = SME_GetDemuxInf();

    GST_INFO ("gst_smedemux_register in");

    demuxfmt = demuxintf->SME_GetNextDemux(NULL);

    while(demuxfmt){
        gchar *type_name, *typefind_name;
        gchar *name, *p;
        guint rank;

        p = name =g_strdup(demuxintf->SME_GetDemuxName(demuxfmt));

        if(!p)
            goto next;

        while (*p) {
            if (*p == '.' || *p == ',')
                *p = '_';
            p++;
        }

        //rank = GST_RANK_PRIMARY;
        rank = GST_RANK_SECONDARY;

        type_name = g_strdup_printf ("smedemux_%s", name);
        if(NULL == type_name)
        {
            goto next;
        }

        if (g_type_from_name (type_name)) {
            g_free (type_name);
            goto next;
        }

        typefind_name = g_strdup_printf ("smetype_%s", name);
        if (NULL == typefind_name)
        {
            g_free (type_name);
            goto next;
        }

        type = g_type_register_static (GST_TYPE_ELEMENT, type_name, &typeinfo, (GTypeFlags)0);
        g_type_set_qdata (type, GST_SMEDEMUX_PARAMS_QDATA, (gpointer)demuxfmt);

        if (!gst_element_register (plugin, type_name, rank, type) ||
                (!gst_type_find_register (plugin, typefind_name, rank,
                    gst_smedemux_type_find, NULL, NULL, (gpointer)demuxfmt, NULL))) {
            GST_WARNING ("Register of type smedemux_%s failed", name);
            g_free (name);
            g_free (type_name);
            g_free (typefind_name);
            return FALSE;
        }

        g_free (type_name);
        g_free (typefind_name);

next:
          g_free (name);
        demuxfmt = demuxintf->SME_GetNextDemux(demuxfmt);
    }

    GST_INFO ("gst_smedemux_register out");

  return TRUE;
}

static gpointer
gst_smedemux_espool_lockbuf(gpointer gp_ctx, guint gu32_size, gint i32_stream_index)
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
gst_smedemux_espool_unlockbuf(gpointer gp_ctx, gpointer gp_buffer)
{
    GstAllocCtx* pst_allocator = (GstAllocCtx*)gp_ctx;

    (void)gp_buffer;

    g_return_if_fail(G_UNLIKELY(NULL != pst_allocator));
    g_return_if_fail(G_UNLIKELY(NULL != pst_allocator->pst_gstbuffer));

    gst_buffer_unmap(pst_allocator->pst_gstbuffer, &pst_allocator->st_info);

    return;
}

static GstCaps *
gst_smedemux_query_srcpad_caps (const GstSmeDemux * demux, GstPad * pad, const GstCaps * filter)
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
        GST_INFO("gst_smedemux_query_srcpad_caps fixed, caps=%s", caps_tostring);
        g_free(caps_tostring);

    }else{
        GST_INFO("gst_smedemux_query_srcpad_caps not fixed");
    }

    return caps;
}

static void
gst_smedemux_check_src_pad_push_eos(GstSmeDemux* demux)
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
gst_smedemux_set_property (GObject *object, guint prop_id,
    const GValue *value,  GParamSpec *pspec)
{
    GstSmeDemux* demux = (GstSmeDemux*)object;

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

            GST_INFO("set smedemux exit block=%d ", exit_block);

            break;
        }
        case PROP_BENCHMARK:
        {
            GST_INFO("set BENCHMARK to demux, unsupport now");
            break;
        }
#if defined (SME_DUMP_ES_VIDEO) || defined (SME_DUMP_ES_AUDIO)
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
                    GST_WARNING("VOS_Memset_S collect smedemux info failed");
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
            GST_INFO("get smedemux info from demux");
            break;
        }
#endif
        default:
            GST_WARNING("gstsmedemux not support setproperty=%d", prop_id);
            break;
    }

    return;
}

static void gst_smedemux_get_property (GObject *object, guint prop_id,
    GValue *value, GParamSpec *pspec)
{
    GstSmeDemux* demux = (GstSmeDemux*)object;

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
            GST_WARNING("gstsmedemux not support getproperty=%d", prop_id);
            break;
    }

    return;
}

/* post message to up layer about the audio track information */
static gboolean
gst_smedemux_audio_tag_list(GstSmeDemux *demux,
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
                      gst_smedemux_getaudiomime(mediainfo),
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
gst_smedemux_post_unsupported_audio_track_info(GstSmeDemux *demux,
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
                           gst_smedemux_getaudiomime(&(audioinfo->stBase)),
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
gst_smedemux_post_video_track_info(GstSmeDemux *demux,
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

static const gchar *gst_smedemux_getaudiomime(const ST_SME_MEDIAINFO *mediainfo)
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
        case E_SME_MEDIA_CODEC_ID_WMAVpro:
            audiomime = "audio/wmapro";
            break;
        case E_SME_MEDIA_CODEC_ID_WMAVLOSSLESS:
            audiomime = "audio/wmalossless";
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_BLURAY:
            audiomime = "audio/pcmbluray";
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
smedemux_plugin_init (GstPlugin * plugin)
{
    GST_INFO("register smedemux_plugin begin...");

    //register demux element
    if (!gst_smedemux_register (plugin))
        GST_WARNING("register smedemux fail.");

    GST_INFO("register smedemux_plugin end.");

    return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    smedemux,
    "smedemux plugin of mediaos. (info about sme: http://www.huawei.com)",
    smedemux_plugin_init, VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
