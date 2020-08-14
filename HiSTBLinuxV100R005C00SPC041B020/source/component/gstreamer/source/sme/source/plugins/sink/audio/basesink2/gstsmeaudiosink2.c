/**************************************************************************//**

  Copyright (C), 2003-2014, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmeaudiosink2.c
 * @brief   sme audio sink2
 * @author
 * @date    2015/7/6
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2015/7/6
 * Author :
 * Desc   : Created file based on gstsmeaudiosink.c
 *
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gstinfo.h"

#include "gstsmecodecmap.h"
#include "gstsmeaudiosink2.h"
#include "osal_mem.h"
#include "gstsmeaudiosinkclock.h"

//lint -e19     //无用的声明
//lint -e438    //unkown
//lint -esym(528,gst_smeaudiosink2_get_instance_private)
//lint -e585    //unkown
//lint -esym(652,GST_CAT_DEFAULT)
//lint -efunc(715,gst_smeaudiosink2_get_times)
//lint -e717    //do ... while(0)
//lint -e730    //函数的布尔类型参数
//lint -e801    //不赞成goto的使用
//lint -efunc(818,gst_smeaudiosink2_get_caps,gst_smeaudiosink2_set_caps,gst_smeaudiosink2_set_property)
//lint -efunc(818,gst_smeaudiosink2_get_property,gst_smeaudiosink2_get_times)
//lint -e826    //可疑的指针到指针转换（区域过小）
//lint -e838    //unkown
//lint -e655    //位操作使用（兼容的）枚举类型
//lint -esym(749,PROP_0)    //重定义宏，这里已经undef了
//lint -esym(749,PROP_LAST) //重定义宏，这里已经undef了
//lint -e818    //指针参数可被声明为const的指针
//lint -e774
//lint -e750

#define SME_DEFAULT_AUDIO_LATENESS (20*GST_MSECOND) /**< in nano second */

/* default sync frame count number */
#define SME_ASINK2_DEFAULT_SYNC_COUNT  3

#define DEFAULT_AUDIO_MUTE (0)
#define DEFAULT_AUDIO_VOLUME (100)
#define DEFAULT_AUDIO_CHANNEL_MODE (A_TRACK_MODE_STEREO)

#define DEFAULT_USE_AUDIO_CLOCK           FALSE
#define GST_SME_AUDIO_MAX_LATENESS        500000000ULL

#define SPEED_UP_COEFFICIENT (1.03) //when overrun we use this to adjust play rate
#define SPEED_DOWN_COEFFICIENT (0.97)//when underrun we use this to adjust play rate
#define DEFAULT_CACHE_AUDIO_DUR_TIME (100*1000000LL)    // 100ms
#define MAX_CACHE_SIZE (128000)                         // 128K

enum
{
    PROP_0 = 0,
#if 0
    PROP_AUDIO_MUTE,
    PROP_AUDIO_VOLUME,
#endif
#ifdef __LINUX__
    PROP_AUDIO_CHANNEL_MODE,
#endif
    PROP_ASINK_INFO,
    PROP_ASINK_PROVIDE_AUDIO_CLOCK,
    PROP_ASINK_PTS_CALIBRATION_HDL,
    PROP_ASINK_SLOW_PRESYNC,
    PROP_ASINK_WAIT_PRESYNC_MAXTIME,
    PROP_ASINK_SLOW_PRESYNC_RATE,
    PROP_ASINK_START_WAIT_PRESYNC,
    PROP_LAST
};

static void
gst_smeaudiosink2_class_init (GstSmeAudiosink2Class * klass);

static void
gst_smeaudiosink2_init (GstSmeAudiosink2 * sink);

static void
gst_smeaudiosink2_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);

static void
gst_smeaudiosink2_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static void
gst_smeaudiosink2_finalize (GObject * object);

static gboolean
gst_smeaudiosink2_send_event (GstElement * element, GstEvent * event);

static GstStateChangeReturn
gst_smeaudiosink2_change_state (GstElement * element, GstStateChange transition);

static GstFlowReturn
gst_smeaudiosink2_render (GstBaseSink * bsink, GstBuffer * buffer);

static gboolean
gst_smeaudiosink2_event (GstBaseSink * bsink, GstEvent * event);

static gboolean
gst_smeaudiosink2_query (GstBaseSink * bsink, GstQuery * query);

static void
gst_smeaudiosink2_get_times (GstBaseSink * basesink,
    GstBuffer * buffer, GstClockTime * start, GstClockTime * end);

//pad
static GstCaps *
gst_smeaudiosink2_get_caps (GstBaseSink * sink, GstCaps * filter);

static gboolean
gst_smeaudiosink2_set_caps (GstBaseSink * sink, GstCaps * caps);

static void
gst_smeaudiosink2_calibrate_pts (GstBaseSink * bsink, GstBuffer * buffer,
  gboolean *bcalibrated);

#if 0
static gboolean
gst_smeaudiosink2_check_sync (GstBaseSink * bsink, GstBuffer * buffer);
#endif

#define GST_SMEAUDIOSINK2_NAME "smeaudiosink2"
    GST_DEBUG_CATEGORY_STATIC (gst_smeaudiosink2_debug);

#ifdef GST_CAT_DEFAULT
#undef GST_CAT_DEFAULT
#endif
#define GST_CAT_DEFAULT gst_smeaudiosink2_debug

#define _do_init \
        GST_DEBUG_CATEGORY_INIT (gst_smeaudiosink2_debug,       \
                                 GST_SMEAUDIOSINK2_NAME,        \
                                 0,                             \
                                 "smeaudiosink2 element");
#define gst_smeaudiosink2_parent_class parent_class
G_DEFINE_TYPE_WITH_CODE (GstSmeAudiosink2,
                         gst_smeaudiosink2,
                         GST_TYPE_BASE_SINK,
                         _do_init);

/*
*asink:audio sink object
*psrc: original data pointer
*srclen: original data bytes.
*pdst: dest data pointer
*dstlen: dest data bytes.
*speed:speed to adjust
*return E_SME_AUDIO_PLAYOUT_RET_SUCCESS means success,others for failed.
*only support 1 ~ 2 channel && 2Bytes per sample
*and only support interleave format.
*/
static gint
gst_smeaudiosink_change_speed(GstSmeAudiosink2 * asink, guint8 *psrc, guint srclen,
  guint8 *pdst, guint *dstlen, gfloat speed)
{
  guint i = 0;
  guint dlen;
  gint ret = E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
  guint bytes_ps = 2;
  guint channel;

  g_return_val_if_fail((psrc!=NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);
  g_return_val_if_fail((pdst!=NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);
  g_return_val_if_fail((dstlen!=NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

  if(asink->audio_info.u32Channels > 2
    || asink->audio_info.eSampleFmt != E_SME_AUDIO_RAW_FMT_PCM_S16BIT){
    E_SME_MEM_SECURE_RET eMemSecRet;
    eMemSecRet = VOS_Memcpy_S(pdst, (V_SIZE)srclen, psrc, (V_SIZE)srclen);
    if (eMemSecRet != E_SME_MEM_OK){
      GST_WARNING("VOS_Memset_S collect audio sink info failed");
    }
    *dstlen = srclen;

    return ret;
  }

  channel = asink->audio_info.u32Channels;
  dlen = (guint)(srclen / (bytes_ps * (channel * speed)));

  if(2 == channel){
    //for 2 channel
    gint* psrc_tmp = (gint*)psrc;
    gint* pdst_tmp = (gint*)pdst;

    for(i=0; i < dlen ; i++){
      guint indx = (guint)(i * speed);
      pdst_tmp[i] = psrc_tmp[indx];
    }
    *dstlen = dlen * bytes_ps * channel;
  }else{
    //for 1 channel
    gshort* psrc_tmp = (gshort*)psrc;
    gshort* pdst_tmp = (gshort*)pdst;

    for(i=0; i < dlen ; i++){
      guint indx = (guint)(i * speed);
      pdst_tmp[i] = psrc_tmp[indx];
    }
    *dstlen = dlen * bytes_ps;
  }

  return ret;
}

//if audio not coming,use systemclock as ref-clock.
//first audio must do avsync after start play or after seek(flush with reset time).
//lint -e571
static GstClockTime
gst_smeaudiosink2_get_clock_time (GstClock* gst_clock, GstSmeAudiosink2* asink)
{
    GstBaseSink *basesink = GST_BASE_SINK(asink);
    GstClockTime last_render_abuffer_runningtime;
    guint64 cur_systime;
    GstClockTime last_audio_clocktime;
    GstClockTime last_audio_systime;
    GstClockTime res_clocktime;

    (void)gst_clock;

    gst_base_sink_get_buffer_time_info(basesink, &last_render_abuffer_runningtime,
      &last_audio_systime);

    //if runningtime == GST_CLOCK_TIME_NONE.return system clock time.
    //else use audio pts to calc clocktime.
    //todo whether sub audio_delay.
    if(G_LIKELY(GST_CLOCK_TIME_IS_VALID(last_render_abuffer_runningtime))){
      GstClockTime sysclk_diff;

      last_audio_clocktime = gst_element_get_base_time(GST_ELEMENT(asink))
        + last_render_abuffer_runningtime;

      cur_systime = GST_USECOND * (guint64)g_get_monotonic_time();
      if(cur_systime >= last_audio_systime){
        sysclk_diff = cur_systime - last_audio_systime;
      }else{
        GST_ERROR_OBJECT(asink, "%s:last_audio_system(%lld) >= cur_systime(%lld)",
          AVSYNC_BY_AUDIO_TAG, last_audio_systime, cur_systime);
        sysclk_diff = 0;
      }
      res_clocktime = last_audio_clocktime + sysclk_diff;
    }else{
      res_clocktime = cur_systime = GST_USECOND * (guint64)g_get_monotonic_time();
      GST_WARNING_OBJECT(asink, "%s:running time is invalid,use system clock:%lld",
        AVSYNC_BY_AUDIO_TAG, res_clocktime);
    }

    GST_DEBUG_OBJECT(asink, "@audiosink@ basetime:%lld, "
        "runningtime:%lld, cur_systime:%lld,"
        " systime:%lld, clocktime: %lld",
        gst_element_get_base_time((GstElement *)asink),
        last_render_abuffer_runningtime, cur_systime, last_audio_systime,
        res_clocktime);

    return res_clocktime;
}
//lint +e571

static GstClock*
gst_smeaudiosink2_provide_clock (GstElement* element)
{
    GstSmeAudiosink2 *asink = GST_SME_AUDIOSINK2(element);

    GST_OBJECT_LOCK (asink);

    if (!GST_OBJECT_FLAG_IS_SET (asink, GST_ELEMENT_FLAG_PROVIDE_CLOCK))
    {
        GST_OBJECT_UNLOCK (asink);
        return NULL;
    }

    GST_OBJECT_UNLOCK (asink);

    return GST_CLOCK_CAST (gst_object_ref (asink->provided_clock));
}

static void
gst_smeaudiosink2_set_provide_clock (GstSmeAudiosink2* asink, gboolean provide)
{
  GST_INFO_OBJECT(asink, "%s:smeaudiosink2_set_provide_clock, %s audio clock!",
    AVSYNC_BY_AUDIO_TAG, provide ? "enable" : "disable");
  GST_OBJECT_LOCK (asink);
  if (provide){
    GST_OBJECT_FLAG_SET (asink, GST_ELEMENT_FLAG_PROVIDE_CLOCK);
    if(NULL == asink->provided_clock){
      /* Set audio clock*/
      asink->provided_clock = gst_smeaudiosink_clock_new("smeaudiosinkclock",
        (GstSmeAudioSinkClockGetTimeFunc)gst_smeaudiosink2_get_clock_time, asink);

      gst_object_set_parent (GST_OBJECT (asink->provided_clock), GST_OBJECT (asink));

      //no max_lateness
      GST_OBJECT_UNLOCK (asink);
      gst_base_sink_set_max_lateness (GST_BASE_SINK (asink), 0LL);
      GST_OBJECT_LOCK (asink);
    }

  }else{
    GST_OBJECT_FLAG_UNSET (asink, GST_ELEMENT_FLAG_PROVIDE_CLOCK);
    if(asink->provided_clock){
      gst_object_unparent(GST_OBJECT(asink->provided_clock));
      gst_object_unref (asink->provided_clock);
      asink->provided_clock = NULL;
    }
  }

  GST_OBJECT_UNLOCK (asink);

  return;
}

static void
gst_smeaudiosink2_class_init (GstSmeAudiosink2Class * klass)
{
    GObjectClass *gobject_class = NULL;
    GstElementClass *gstelement_class = NULL;
    GstBaseSinkClass* gstbasesink_class = NULL;

    gobject_class = G_OBJECT_CLASS(klass);
    gstelement_class = GST_ELEMENT_CLASS(klass);
    gstbasesink_class = GST_BASE_SINK_CLASS(klass);

    //klass->parent_class = g_type_class_peek_parent (klass);

    //gobject method
    gobject_class->set_property = gst_smeaudiosink2_set_property;
    gobject_class->get_property = gst_smeaudiosink2_get_property;
    gobject_class->finalize = GST_DEBUG_FUNCPTR (gst_smeaudiosink2_finalize);

    // audio clock provider
    gstelement_class->provide_clock =
        GST_DEBUG_FUNCPTR (gst_smeaudiosink2_provide_clock);

#if 0
    g_object_class_install_property(gobject_class, PROP_AUDIO_MUTE,
        g_param_spec_uint (GST_SME_PROP_KEY_MUTE_AUDIO, "mute-audio",
        "mute-audio", 0, 1, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property(gobject_class, PROP_AUDIO_VOLUME,
        g_param_spec_uint (GST_SME_PROP_KEY_AUDIO_VOLUME, "audio-volume",
        "audio-volume", 0, 100, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
#endif
#ifdef __LINUX__
    g_object_class_install_property(gobject_class, PROP_AUDIO_CHANNEL_MODE,
        g_param_spec_uint (GST_SME_PROP_KEY_AUDIO_CHANNEL_MODE, "audio-channel-mode",
        "audio-channel-mode", 0, A_TRACK_MODE_BUTT, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
#endif
    g_object_class_install_property (gobject_class, PROP_ASINK_INFO,
        g_param_spec_pointer ("asink-proc-info", "Asink-Proc-Info",
          "Poniter of Audio Sink",
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, PROP_ASINK_PROVIDE_AUDIO_CLOCK,
        g_param_spec_boolean (GST_SME_PROP_KEY_AUDIO_CLOCK, "use sme audio clock",
            "use sme audio clock", FALSE,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, PROP_ASINK_PTS_CALIBRATION_HDL,
        g_param_spec_pointer (GST_SME_PTS_CALIBRATION_HDL, "pts calibration hdl",
          "pts calibration hdl",
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, PROP_ASINK_SLOW_PRESYNC,
        g_param_spec_uint(GST_SME_PROP_KEY_SLOW_PRESYNC,
          "asink slow presync mode",
          "asink slow presync mode",
          0, (guint)-1, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, PROP_ASINK_WAIT_PRESYNC_MAXTIME,
        g_param_spec_uint64(GST_SME_PROP_KEY_WAIT_PRESYNC_MAXTIME,
          "asink wait-presync maxtime",
          "asink wait-presync maxtime",
          0ULL, (guint64)-1, (guint64)-1, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, PROP_ASINK_SLOW_PRESYNC_RATE,
        g_param_spec_double(GST_SME_PROP_KEY_SLOW_PRESYNC_RATE,
          "asink slow-presync-rate",
          "asink slow-presync-rate",
          0.0, 1.0, 0.333333, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, PROP_ASINK_START_WAIT_PRESYNC,
        g_param_spec_boolean(GST_SME_PROP_KEY_START_WAIT_PRESYNC,
          "asink start wait_presync",
          "asink start wait_presync",
          FALSE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    //element method
    gstelement_class->change_state = gst_smeaudiosink2_change_state;
    gstelement_class->send_event = gst_smeaudiosink2_send_event;

    //basesink method
    gstbasesink_class->activate_pull = NULL;
    gstbasesink_class->get_caps =
        GST_DEBUG_FUNCPTR (gst_smeaudiosink2_get_caps);
    gstbasesink_class->set_caps =
        GST_DEBUG_FUNCPTR (gst_smeaudiosink2_set_caps);
    gstbasesink_class->event = GST_DEBUG_FUNCPTR (gst_smeaudiosink2_event);
    gstbasesink_class->render = GST_DEBUG_FUNCPTR (gst_smeaudiosink2_render);
    gstbasesink_class->query = GST_DEBUG_FUNCPTR (gst_smeaudiosink2_query);
    gstbasesink_class->get_times =
        GST_DEBUG_FUNCPTR (gst_smeaudiosink2_get_times);
    gstbasesink_class->fixate = NULL;
    gstbasesink_class->wait_event = NULL;
    gstbasesink_class->calibrate_pts =
        GST_DEBUG_FUNCPTR (gst_smeaudiosink2_calibrate_pts);

    return;
}

static void
gst_smeaudiosink2_init (GstSmeAudiosink2 * asink)
{
    E_SME_MEM_SECURE_RET eReturn;

    GST_INFO("gst_smeaudiosink2_init() IN");

    asink->provided_clock = NULL;

    if (DEFAULT_USE_AUDIO_CLOCK){
        GST_OBJECT_FLAG_SET (asink, GST_ELEMENT_FLAG_PROVIDE_CLOCK);
    }else{
        GST_OBJECT_FLAG_UNSET (asink, GST_ELEMENT_FLAG_PROVIDE_CLOCK);
    }

    if (GST_OBJECT_FLAG_IS_SET (asink, GST_ELEMENT_FLAG_PROVIDE_CLOCK)){
        /* Set audio clock*/
        asink->provided_clock = gst_smeaudiosink_clock_new("smeaudiosinkclock",
                            (GstSmeAudioSinkClockGetTimeFunc)gst_smeaudiosink2_get_clock_time, asink);
        gst_object_set_parent (GST_OBJECT (asink->provided_clock), GST_OBJECT (asink));

        //no max_lateness
        gst_base_sink_set_max_lateness (GST_BASE_SINK (asink), SME_DEFAULT_AUDIO_LATENESS);
    }else{
        gst_base_sink_set_max_lateness (GST_BASE_SINK (asink), GST_SME_AUDIO_MAX_LATENESS);
        g_atomic_int_set(&asink->sync_count, SME_ASINK2_DEFAULT_SYNC_COUNT);
    }
    GST_WARNING_OBJECT(asink, "%s:enable audio avsync by default !", AVSYNC_BY_AUDIO_TAG);
    gst_base_sink_set_sync (GST_BASE_SINK (asink), TRUE);
    asink->need_sync = TRUE;

    eReturn = VOS_Memset_S((void*)(&asink->audio_info),
        sizeof(ST_SME_AUDIOINFO), 0,  sizeof(ST_SME_AUDIOINFO));
    if(E_SME_MEM_OK != eReturn)
    {
        GST_WARNING("VOS_Memset_S asink->audio_info Failed(%d)", eReturn);
    }

    eReturn = VOS_Memset_S((void*)(&asink->audio_log_ctx),
        sizeof(ST_SME_LOG_CB), 0,  sizeof(ST_SME_LOG_CB));
    if(E_SME_MEM_OK != eReturn)
    {
        GST_WARNING("VOS_Memset_S asink->audio_log_ctx Failed(%d)", eReturn);
    }

    asink->recieve_eos = FALSE;

    //gst_base_sink_set_render_delay(GST_BASE_SINK(asink),
    //                               SME_DEFAULT_RENDER_DELAY);


    //g_atomic_int_set(&asink->sync_count, SME_ASINK2_DEFAULT_SYNC_COUNT);
    //asink->need_sync = TRUE;

    asink->mute     = DEFAULT_AUDIO_MUTE;
    asink->volume   = DEFAULT_AUDIO_VOLUME;
    asink->mode     = DEFAULT_AUDIO_CHANNEL_MODE;

    g_mutex_init (&asink->lock);

#ifdef SMEPLAYER_BENCHMARK_OPEN
    if (VOS_Memset_S(&asink->audio_sink_info,
                sizeof(asink->audio_sink_info),
                0,
                sizeof(asink->audio_sink_info))
            != E_SME_MEM_OK)
    {
        return ;
    }
    asink->audio_sink_info.AudioMute = asink->mute;
    asink->audio_sink_info.AudioVolume = asink->volume;
    asink->audio_sink_info.AudioChannelMode = asink->mode;
    gettimeofday(&asink->audio_sink_info.CreateASinkTime, NULL);

    asink->dump_sink_file = NULL;
    asink->dump_sink_flag = FALSE;
    if (VOS_Snprintf_S(asink->dump_sink_file_name,sizeof(asink->dump_sink_file_name),
        sizeof(asink->dump_sink_file_name)-1,"/mnt/asink_in.pcm") <= 0)
    {
        GST_WARNING_OBJECT(asink, "VOS_Snprintf_S failed!");
    }
#endif

    asink->first_write_flag = 1;
    asink->write_cache = (guint8 *)malloc(MAX_CACHE_SIZE);
    asink->write_cache_len = 0;
    asink->first_cache_pts = GST_CLOCK_TIME_NONE;
    asink->current_cache_pts = GST_CLOCK_TIME_NONE;
    asink->cache_audio_dur_time = DEFAULT_CACHE_AUDIO_DUR_TIME;

    asink->audio_info.u32SampleRate = 0;
    asink->audio_info.u32Channels =  0;
    asink->audio_info.eSampleFmt  =  E_SME_AUDIO_RAW_FMT_NONE;

    GST_INFO("gst_smeaudiosink2_init() OUT");

    return;
}

static void
gst_smeaudiosink2_finalize (GObject * object)
{
    GstSmeAudiosink2 *asink = (GstSmeAudiosink2 *)object;

    GST_INFO_OBJECT(asink, "gst_smeaudiosink2_finalize in ");

    if (asink == NULL) {
        GST_ERROR("gst_smeaudiosink2_finalize args object is null");
        return;
    }

#ifdef SMEPLAYER_BENCHMARK_OPEN
    if (asink->dump_sink_file) {
        fclose(asink->dump_sink_file);
        asink->dump_sink_file = NULL;
    }
    asink->dump_sink_flag = FALSE;
#endif

    if (asink->provided_clock)
    {
        gst_object_unparent(GST_OBJECT(asink->provided_clock));
        gst_smeaudiosink_clock_invalidate(asink->provided_clock);
        gst_object_unref (asink->provided_clock);
        asink->provided_clock = NULL;
    }

    if(asink->pts_calibration_hdl){
      gst_sme_pts_calibration_entry_deinit(asink->pts_calibration_hdl,
        E_ENTRY_TYPE_AUDIO);
    }
    g_mutex_clear (&asink->lock);

    if (asink->write_cache) {
        free(asink->write_cache);
        asink->write_cache = NULL;
    }

    G_OBJECT_CLASS (parent_class)->finalize (object);

    GST_INFO_OBJECT(asink, "gst_smeaudiosink2_finalize out");
}

static GstStateChangeReturn
gst_smeaudiosink2_change_state (GstElement * element, GstStateChange transition)
{
    GstSmeAudiosink2 *asink = GST_SME_AUDIOSINK2(element);
    GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;
    GstSmeAudiosink2Class *asink_klass = GST_SME_AUDIOSINK2_GET_CLASS(asink);
    gint err = 0;

    GST_INFO_OBJECT(asink, "change state to:%#x", transition);

    switch (transition) {
        case GST_STATE_CHANGE_READY_TO_PAUSED:
            GST_INFO_OBJECT(asink, "***READY -> PAUSED");
            if(asink->pts_calibration_hdl){
              gst_sme_pts_calibration_entry_init(asink->pts_calibration_hdl,
                E_ENTRY_TYPE_AUDIO, "asink", element);
            }
            break;
        case GST_STATE_CHANGE_NULL_TO_READY:
            GST_INFO_OBJECT(asink, "***NULL -> READY");
            if (asink_klass->create) {
                g_mutex_lock(&asink->lock);
                err = asink_klass->create(asink);
                if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                    ret = GST_STATE_CHANGE_FAILURE;
                    GST_ERROR_OBJECT(asink,
                                     "create failed(%#x)", err);
                    g_mutex_unlock(&asink->lock);
                    return ret;
                }
#if 0
                if (DEFAULT_AUDIO_MUTE != asink->mute && asink_klass->set_mute) {
                    err = asink_klass->set_mute(asink, asink->mute);
                    if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                        ret = GST_STATE_CHANGE_FAILURE;
                        GST_ERROR_OBJECT(asink,
                                         "set_mute failed(%#x)", err);
                        if (asink_klass->destroy) {
                            err = asink_klass->destroy(asink);
                            if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                                GST_ERROR_OBJECT(asink,
                                                 "destroy failed(%#x)",
                                                 err);
                            }
                        }
                        g_mutex_unlock(&asink->lock);
                        return ret;
                    }
                }
                if (DEFAULT_AUDIO_VOLUME != asink->volume && asink_klass->set_volume) {
                    err = asink_klass->set_volume(asink, asink->volume);
                    if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                        ret = GST_STATE_CHANGE_FAILURE;
                        GST_ERROR_OBJECT(asink,
                                         "set_volume failed(%#x)", err);
                        if (asink_klass->destroy) {
                            err = asink_klass->destroy(asink);
                            if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                                GST_ERROR_OBJECT(asink,
                                                 "destroy failed(%#x)",
                                                 err);
                            }
                        }
                        g_mutex_unlock(&asink->lock);
                        return ret;
                    }
                }
#endif
#ifdef __LINUX__
                if (DEFAULT_AUDIO_CHANNEL_MODE != asink->mode && asink_klass->set_channel_mode) {
                    err = asink_klass->set_channel_mode(asink, asink->mode);
                    if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                        ret = GST_STATE_CHANGE_FAILURE;
                        GST_ERROR_OBJECT(asink,
                                         "set_channel_mode failed(%#x)", err);
                        if (asink_klass->destroy) {
                            err = asink_klass->destroy(asink);
                            if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                                GST_ERROR_OBJECT(asink,
                                                 "destroy failed(%#x)",
                                                 err);
                            }
                        }
                        g_mutex_unlock(&asink->lock);
                        return ret;
                    }
                }
#endif
                g_mutex_unlock(&asink->lock);
            }
            asink->recieve_eos = FALSE;
            break;

        case GST_STATE_CHANGE_PAUSED_TO_READY:
            asink->recieve_eos = FALSE;
            break;
        case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
            g_atomic_int_set(&asink->sync_count, SME_ASINK2_DEFAULT_SYNC_COUNT);
            asink->need_sync = TRUE;

            /** BEGIN: Added by baihaili(b00311320),2015/2/10 */
            GST_INFO_OBJECT(asink, ("***PAUSED -> PLAYING "));
            if (asink_klass->start) {
                g_mutex_lock(&asink->lock);
                err = asink_klass->start(asink);
                g_mutex_unlock(&asink->lock);
                if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                    ret = GST_STATE_CHANGE_FAILURE;
                    GST_ERROR_OBJECT(asink,
                                     "start failed(%#x)", err);
                    return ret;
                }
            }
            /** END:   Added by baihaili(b00311320), 2015/2/10 */
            break;
        default:
            break;
   }

    ret = GST_ELEMENT_CLASS(parent_class)->change_state (element, transition);

    GST_INFO_OBJECT(asink, "change state ok(transition:%d).", transition);

    switch (transition) {
        case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
            /** BEGIN: Changed by baihaili(b00311320),2015/2/10 */
            if (ret == GST_STATE_CHANGE_FAILURE) {
                if (asink_klass->pause) {
                    g_mutex_lock(&asink->lock);
                    err = asink_klass->pause(asink);
                    g_mutex_unlock(&asink->lock);
                    if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                        ret = GST_STATE_CHANGE_FAILURE;
                        GST_ERROR_OBJECT(asink,
                                         "pause failed(%#x)",
                                         err);
                    }
                }
            }
            /** END:   Changed by baihaili(b00311320), 2015/2/10 */
            break;
        case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
            GST_INFO_OBJECT(asink, ("***PLAYING -> PAUSED in!"));
            if (asink_klass->pause) {
                g_mutex_lock(&asink->lock);
                err = asink_klass->pause(asink);
                g_mutex_unlock(&asink->lock);
                if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                    ret = GST_STATE_CHANGE_FAILURE;
                    GST_ERROR_OBJECT(asink,
                                     "pause failed(%#x)",
                                     err);
                }
            }
            GST_INFO_OBJECT(asink, ("***PLAYING -> PAUSED out!"));
            break;
        case GST_STATE_CHANGE_PAUSED_TO_READY:
            GST_INFO_OBJECT(asink, ("***PAUSED -> READY in!"));

            if (asink_klass->pause) {
                g_mutex_lock(&asink->lock);
                err = asink_klass->pause(asink);
                g_mutex_unlock(&asink->lock);
                if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                    ret = GST_STATE_CHANGE_FAILURE;
                    GST_ERROR_OBJECT(asink,
                                     "pause failed(%#x)",
                                     err);
                }
            }

            if (asink_klass->stop) {
                g_mutex_lock(&asink->lock);
                err = asink_klass->stop(asink);
                g_mutex_unlock(&asink->lock);
                if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                    ret = GST_STATE_CHANGE_FAILURE;
                    GST_ERROR_OBJECT(asink,
                                     "stop failed(%#x)",
                                     err);
                }
            #ifdef SMEPLAYER_BENCHMARK_OPEN
            gettimeofday(&asink->audio_sink_info.ASinkFinalPlayTime, NULL);
            #endif
            }
            if(asink->pts_calibration_hdl){
              gst_sme_pts_calibration_entry_deinit(asink->pts_calibration_hdl,
                E_ENTRY_TYPE_AUDIO);
            }
            break;
        case GST_STATE_CHANGE_READY_TO_NULL:
            if (asink_klass->destroy) {
                g_mutex_lock(&asink->lock);
                err = asink_klass->destroy(asink);
                g_mutex_unlock(&asink->lock);
                if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS != err) {
                    ret = GST_STATE_CHANGE_FAILURE;
                    GST_ERROR_OBJECT(asink,
                                     "destroy failed(%#x)",
                                     err);
                }
            }
            break;
        default:
            break;
    }

    return ret;
}

static GstCaps *
gst_smeaudiosink2_get_caps (GstBaseSink * sink, GstCaps * caps)
{
    GST_INFO("gst_smeaudiosink2_get_caps ..., sink %p, caps %p",sink,caps);

    return NULL;
}

static gboolean
gst_smeaudiosink2_set_caps (GstBaseSink * sink, GstCaps * caps)
{
    GstSmeAudiosink2* asink = GST_SME_AUDIOSINK2(sink);
    GstSmeAudiosink2Class *asink_klass = GST_SME_AUDIOSINK2_GET_CLASS(asink);
    ST_SME_AUDIOINFO audioInfo;
    gboolean bEs = FALSE;
    gboolean bRet = FALSE;
    gint eErr = E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG;
    gchar* caps_tostring = NULL;
    E_SME_MEM_SECURE_RET eReturn = E_SME_MEM_OK;
    gboolean bResetCaps = FALSE;

    GST_INFO("gst_smeaudiosink2_set_caps: in");

    if((NULL == sink) || (NULL == caps)){
        GST_ERROR("gst_smeaudiosink2_set_caps invalid args!");
        return bRet;
    }

    /** BEGIN: Modified for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */
    caps_tostring = gst_caps_to_string((const GstCaps*)caps);
    eReturn = VOS_Memset_S((void*)(&audioInfo),
        sizeof(audioInfo), 0,  sizeof(audioInfo));
    if(E_SME_MEM_OK != eReturn)
    {
        GST_WARNING("VOS_Memset_S audio_info Failed(%d)", eReturn);
    }

    bRet = gst_sme_caps_to_audioinfo((const GstCaps*)caps,
                                    &audioInfo, &bEs);
    if(FALSE == bRet){
        GST_ERROR_OBJECT(asink, "gst_sme_caps_to_mediainfo failed,caps=%s",
            caps_tostring);

        /** BEGIN: Modified for DTS2015042206034  by baihaili(b00311320), 2015/5/29 */
        g_free(caps_tostring);
        /** END:   Modified for DTS2015042206034 by baihaili(b00311320), 2015/5/29 */
        return bRet;
    }

    GST_WARNING_OBJECT (asink,"caps:%s", caps_tostring);
    g_free(caps_tostring);
    /** END: Modified for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */

    if (asink->audio_info.u32SampleRate != audioInfo.u32SampleRate) {
        asink->audio_info.u32SampleRate = audioInfo.u32SampleRate;
        bResetCaps = TRUE;
    }
    if (asink->audio_info.u32Channels !=  audioInfo.u32Channels) {
        asink->audio_info.u32Channels =  audioInfo.u32Channels;
        bResetCaps = TRUE;
    }
    if (!bEs) {
        if (asink->audio_info.eSampleFmt  !=  audioInfo.eSampleFmt) {
            asink->audio_info.eSampleFmt  =  audioInfo.eSampleFmt;
            bResetCaps = TRUE;
        }
    } else {
        /* give a default value */
        if (asink->audio_info.eSampleFmt != E_SME_AUDIO_RAW_FMT_PCM_S16BIT) {
            asink->audio_info.eSampleFmt = E_SME_AUDIO_RAW_FMT_PCM_S16BIT;
            bResetCaps = TRUE;
        }
    }

    GST_WARNING_OBJECT (asink, "u32SampleRate:%d u32Channels:%d eSampleFmt: %d bps:%d ",
                        asink->audio_info.u32SampleRate ,
                        asink->audio_info.u32Channels,
                        asink->audio_info.eSampleFmt,
                        sme_smp_format_to_depth(asink->audio_info.eSampleFmt));

    if (bResetCaps && asink_klass->set) {
        g_mutex_lock(&asink->lock);
        eErr = asink_klass->set(asink,
                                (V_INT32)bEs,
                                (V_INT32)asink->audio_info.u32SampleRate,
                                (V_INT32)asink->audio_info.u32Channels,
#ifdef __DDP_PT__
                                asink->audio_info.eSampleFmt,
#else
                                sme_smp_format_to_depth(asink->audio_info.eSampleFmt),
#endif
                                NULL,
                                NULL);
        g_mutex_unlock(&asink->lock);
        if(E_SME_AUDIO_PLAYOUT_RET_SUCCESS != eErr){
            bRet = FALSE;
            GST_ERROR_OBJECT(asink, "SME_SetAudioPlayout failed(%#x)", eErr);
        }
    }

    GST_INFO("gst_smeaudiosink2_set_caps: out ,bRet :%d", bRet);

    return bRet;
}

static void
gst_smeaudiosink2_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
    GstSmeAudiosink2* asink = GST_SME_AUDIOSINK2(object);
    GstSmeAudiosink2Class *klass = GST_SME_AUDIOSINK2_GET_CLASS(asink);

    GST_INFO_OBJECT(object,
                    "set property:%d, value:%p, pspec:%p",
                    prop_id, value, pspec);
    switch (prop_id)
    {
#if 0
        case PROP_AUDIO_MUTE:
        {
            V_UINT32 mute = g_value_get_uint(value);
            GST_WARNING("PROP_AUDIO_MUTE set val = %d", mute);

            asink->mute = mute;
#ifdef SMEPLAYER_BENCHMARK_OPEN
            asink->audio_sink_info.AudioMute = asink->mute;
#endif

            if (klass->set_mute)
            {
                g_mutex_lock(&asink->lock);
                klass->set_mute(asink, mute);
                g_mutex_unlock(&asink->lock);
            }
            break;
        }
        case PROP_AUDIO_VOLUME:
        {
            V_UINT32 volume = g_value_get_uint(value);
            GST_WARNING("PROP_AUDIO_VOLUME set val = %d", volume);

            asink->volume = volume;
#ifdef SMEPLAYER_BENCHMARK_OPEN
            asink->audio_sink_info.AudioVolume = asink->volume;
#endif


            if (klass->set_volume)
            {
                g_mutex_lock(&asink->lock);
                klass->set_volume(asink, volume);
                g_mutex_unlock(&asink->lock);
            }
            break;
        }
#endif
#ifdef __LINUX__
        case PROP_AUDIO_CHANNEL_MODE:
        {
            V_UINT32 mode = g_value_get_uint(value);
            GST_WARNING("PROP_AUDIO_CHANNEL_MODE set val = %d", mode);

            asink->mode = (A_TRACK_CHANNEL_MODE_E)mode;
#ifdef SMEPLAYER_BENCHMARK_OPEN
            asink->audio_sink_info.AudioChannelMode = asink->mode;
#endif


            if (klass->set_channel_mode)
            {
                g_mutex_lock(&asink->lock);
                klass->set_channel_mode(asink, (A_TRACK_CHANNEL_MODE_E)mode);
                g_mutex_unlock(&asink->lock);
            }
            break;
        }
#endif

#ifdef SMEPLAYER_BENCHMARK_OPEN
        case PROP_ASINK_INFO:
        {
            ST_AUDIOSINK_PROC_INFO *pstAsinkProcInfo;
            ST_ASINK_CMD_INFO *pstAsinkCmdInfo;
            E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;

            pstAsinkProcInfo = g_value_get_pointer(value);

            if (pstAsinkProcInfo && pstAsinkProcInfo->eProcCmdType == E_SME_PROC_CMD_SHOW_INFO)
            {
                ST_ASINK_SHOW_INFO *pstAsinkShowInfo;
                pstAsinkShowInfo = &(pstAsinkProcInfo->stShowInfo);

                eMemSecRet = VOS_Memcpy_S(pstAsinkShowInfo,
                                          sizeof(ST_ASINK_SHOW_INFO),
                                          &asink->audio_sink_info,
                                          sizeof(asink->audio_sink_info));
                if (eMemSecRet != E_SME_MEM_OK)
                    GST_WARNING("VOS_Memset_S collect audio sink info failed");

                GST_INFO("Collect audio sink info for proc");
            }
            else if (pstAsinkProcInfo && pstAsinkProcInfo->eProcCmdType == E_SME_PROC_CMD_SET_CMD)
            {
                pstAsinkCmdInfo = &(pstAsinkProcInfo->stCmdInfo);
                GST_INFO("Audio Sink bDumpSinkStream=%d", pstAsinkCmdInfo->bDumpSinkStream);
                GST_INFO("Audio Sink bDumpSrcStream=%d", pstAsinkCmdInfo->bDumpSrcStream);
                GST_INFO("Audio Sink cDumpFileStorePath=%s", pstAsinkCmdInfo->cDumpFileStorePath);
                if (pstAsinkCmdInfo->bDumpSinkStream != -1) {
                    if (pstAsinkCmdInfo->bDumpSinkStream == 1)
                    {
                      //begin to start dump sink data;
                      struct timeval cur_time;
                      gettimeofday(&cur_time, NULL);
                      //lint +fva
                      VOS_Snprintf_S(asink->dump_sink_file_name,sizeof(asink->dump_sink_file_name),sizeof(asink->dump_sink_file_name)-1,
                            "%s/asink_in_%d_%d.pcm",pstAsinkCmdInfo->cDumpFileStorePath,cur_time.tv_sec,cur_time.tv_usec);
                      asink->dump_sink_flag = TRUE;
                    }
                    else if(pstAsinkCmdInfo->bDumpSinkStream == 0)
                    {
                      //stop dump sink data
                      asink->dump_sink_flag = FALSE;
                    }
                }

                if (pstAsinkCmdInfo->bDumpSrcStream != -1) {
                    if (pstAsinkCmdInfo->bDumpSrcStream == 1)
                    {
                      //begin to start dump src data;
                    }
                    else if(pstAsinkCmdInfo->bDumpSrcStream == 0)
                    {
                      //stop dump src data
                    }
                }
            }

            break;
        }
#endif
        case PROP_ASINK_PROVIDE_AUDIO_CLOCK:{
            //it must be called before pipeline to playing
            gboolean isprovide = g_value_get_boolean (value);
            gst_smeaudiosink2_set_provide_clock (asink, isprovide);
            break;
        }
        case PROP_ASINK_PTS_CALIBRATION_HDL:{
            asink->pts_calibration_hdl = (GstSmePTSCalibrateHDL)g_value_get_pointer(value);
            GST_INFO_OBJECT(asink, "asink PROP_ASINK_PTS_CALIBRATION_HDL:%p",
              asink->pts_calibration_hdl);
            break;
        }
        case PROP_ASINK_SLOW_PRESYNC:{
            GstBaseSink* bsink = GST_BASE_SINK_CAST(asink);
            bsink->slow_presync_info.slow_presync_state = g_value_get_uint(value);
            if(asink->pts_calibration_hdl){
              gst_sme_pts_calibration_entry_update_presync_info(asink->pts_calibration_hdl,
                E_ENTRY_TYPE_AUDIO, bsink->slow_presync_info);
            }

            if(1 == bsink->slow_presync_info.slow_presync_state
              || 0 == bsink->slow_presync_info.slow_presync_state){
              bsink->slow_presync_info.last_presync_pts = GST_CLOCK_TIME_NONE;
              bsink->slow_presync_info.start_wait_presync_systime = GST_CLOCK_TIME_NONE;
              bsink->slow_presync_info.slow_presync_stime = GST_CLOCK_TIME_NONE;
            }

            GST_WARNING_OBJECT(asink, "pre-sync:asink prop-slow-presync-mode:%d",
              bsink->slow_presync_info.slow_presync_state);
            break;
        }
        case PROP_ASINK_WAIT_PRESYNC_MAXTIME:{
            GstBaseSink* bsink = GST_BASE_SINK_CAST(asink);
            bsink->slow_presync_info.wait_presync_max_time = g_value_get_uint64(value);
            GST_WARNING_OBJECT(asink, "pre-sync:asink prop-wait-presync-maxtime:%lld",
              bsink->slow_presync_info.wait_presync_max_time);
            break;
        }
        case PROP_ASINK_SLOW_PRESYNC_RATE:{
            GstBaseSink* bsink = GST_BASE_SINK_CAST(asink);
            bsink->slow_presync_info.gdrate = g_value_get_double(value);
            GST_WARNING_OBJECT(asink, "pre-sync:asink prop-slow-presync-rate:%f",
              bsink->slow_presync_info.gdrate);
            break;
        }
        case PROP_ASINK_START_WAIT_PRESYNC:{
            GstBaseSink* bsink = GST_BASE_SINK_CAST(asink);
            if(g_value_get_boolean(value)){
              bsink->slow_presync_info.start_wait_presync_systime
                = GST_USECOND*(guint64)g_get_monotonic_time();
            }else{
              bsink->slow_presync_info.start_wait_presync_systime = GST_CLOCK_TIME_NONE;
            }
            GST_WARNING_OBJECT(asink, "pre-sync:asink start_wait_presync:%llu",
              bsink->slow_presync_info.start_wait_presync_systime);
            break;
        }
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }

    return;
}

static void
gst_smeaudiosink2_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
    GstSmeAudiosink2* asink = GST_SME_AUDIOSINK2(object);
    GstSmeAudiosink2Class *klass = GST_SME_AUDIOSINK2_GET_CLASS(asink);

    GST_INFO_OBJECT(object,
                    "get property:%d, value:%p, pspec:%p",
                    prop_id, value, pspec);
    switch (prop_id)
    {
#if 0
        case PROP_AUDIO_MUTE:
        {
            guint mute = asink->mute;
            if (klass->get_mute)
            {
                g_mutex_lock(&asink->lock);
                klass->get_mute(asink, &mute);
                g_mutex_unlock(&asink->lock);
            }
            g_value_set_uint (value, mute);
            GST_WARNING("PROP_AUDIO_MUTE get val = %d", mute);
            break;
        }
        case PROP_AUDIO_VOLUME:
        {
            guint volume = asink->volume;
            if (klass->get_volume)
            {
                g_mutex_lock(&asink->lock);
                klass->get_volume(asink, &volume);
                g_mutex_unlock(&asink->lock);
            }
            g_value_set_uint (value, volume);
            GST_WARNING("PROP_AUDIO_VOLUME get val = %d", volume);
            break;
        }
#endif
#ifdef __LINUX__
        case PROP_AUDIO_CHANNEL_MODE:
        {
            A_TRACK_CHANNEL_MODE_E mode = asink->mode;
            if (klass->get_channel_mode)
            {
                g_mutex_lock(&asink->lock);
                klass->get_channel_mode(asink, &mode);
                g_mutex_unlock(&asink->lock);
            }
            g_value_set_uint (value, (guint)mode);
            GST_WARNING("PROP_AUDIO_CHANNEL_MODE get val = %d", mode);
            break;
        }
#endif

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }

    return;
}

#ifdef SMEPLAYER_BENCHMARK_OPEN
static void gst_smeaudiosink2_dump_sink_file(GstBaseSink * bsink, const guchar * data, guint len)
{
    GstSmeAudiosink2 *asink = GST_SME_AUDIOSINK2(bsink);

    if (asink->dump_sink_flag) {
        if (NULL == asink->dump_sink_file) {
            asink->dump_sink_file = fopen(asink->dump_sink_file_name, "wb");
        }
    } else {
        if (asink->dump_sink_file) {
            fclose(asink->dump_sink_file);
            asink->dump_sink_file = NULL;
        }
    }

    if (asink->dump_sink_file) {
        if (fwrite(data, 1, len, asink->dump_sink_file) != (gint)len) {
            GST_WARNING_OBJECT(bsink,"gst_smeaudiosink2_dump_sink_file error len = %d!",len);
        }
    }
}
#endif

//lint -e666
static GstFlowReturn
gst_smeaudiosink2_render (GstBaseSink * bsink, GstBuffer * buffer)
{
  GstMapInfo info;
  GstFlowReturn eRet = GST_FLOW_OK;
  gint eSmeRet = E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
  GstSmeAudiosink2 *asink = GST_SME_AUDIOSINK2(bsink);
  GstSmeAudiosink2Class *klass = GST_SME_AUDIOSINK2_GET_CLASS(asink);
  guchar* pu8InBuffer = NULL;
  guint inbuffer_len = 0;
  guchar* pu8Data = NULL;
  E_SME_MEM_SECURE_RET eReturn = E_SME_MEM_OK;
  gboolean write_flag = TRUE;

  GST_INFO_OBJECT(asink, "gst_smeaudiosink2_render in,flags=%#x, issync=%d",
    GST_OBJECT_FLAGS (bsink), gst_base_sink_get_sync(bsink));

  if (g_atomic_int_get(&asink->flushing_flag)) {
       eRet = GST_FLOW_FLUSHING;
       GST_ERROR_OBJECT(asink, "SME_WriteAudio: GST_FLOW_FLUSHING, return 0x%x",
             eSmeRet);
       return eRet;
  }

  if(asink->pts_calibration_hdl){
    gst_sme_pts_calibration_entry_update_presync_info(asink->pts_calibration_hdl,
      E_ENTRY_TYPE_AUDIO, bsink->slow_presync_info);
  }

  /**
   * Don't support sme buffer anymore, always use gst generic buffer.
   * Write the data to child element anyway.
   */
  if(!GST_CLOCK_TIME_IS_VALID (GST_BUFFER_DTS (buffer))
     || !GST_CLOCK_TIME_IS_VALID (GST_BUFFER_PTS (buffer))
     || !GST_CLOCK_TIME_IS_VALID (GST_BUFFER_DURATION (buffer))){
      GST_DEBUG_OBJECT(asink, "invalid time stamps");
  }

  if(GST_OBJECT_FLAG_IS_SET (asink, GST_ELEMENT_FLAG_PROVIDE_CLOCK)
    && gst_base_sink_get_sync(GST_BASE_SINK (asink))){
    //no max_lateness
    gst_base_sink_set_max_lateness (GST_BASE_SINK (asink), SME_DEFAULT_AUDIO_LATENESS);
    gst_base_sink_set_sync(GST_BASE_SINK (asink), FALSE);
    GST_WARNING_OBJECT(asink, "%s:disable audio avsync !", AVSYNC_BY_AUDIO_TAG);
  }

  //gstreamer默认的buffer
  gst_buffer_map (buffer, &info, GST_MAP_READ);
  if(info.size <= 4){
    GST_WARNING_OBJECT(bsink, "gst_smeaudiosink2_render size:%d, too small",
      info.size);

    goto exit;
  }
  GST_DEBUG_OBJECT(asink, "gst_smeaudiosink2_render pts:%lld, size:%d",
    GST_BUFFER_PTS(buffer), info.size);

#ifdef SMEPLAYER_BENCHMARK_OPEN
  if (GST_CLOCK_TIME_IS_VALID (GST_BUFFER_PTS (buffer))) {
      asink->audio_sink_info.ASinkOutputPTS = (V_INT64)GST_BUFFER_PTS (buffer);
  }
  if (!asink->audio_sink_info.ASinkFirstPlayTimeFlag) {
      asink->audio_sink_info.ASinkFirstPlayTimeFlag = 1;
      gettimeofday(&asink->audio_sink_info.ASinkFirstPlayTime, NULL);
  }
  asink->audio_sink_info.stAVSyncInfo.i64BaseTime = (gint64)GST_VALID_TIME_AS_MSECONDS(
    gst_element_get_base_time(GST_ELEMENT(bsink)));
  if(GST_ELEMENT_CLOCK(GST_ELEMENT(bsink))){
    asink->audio_sink_info.stAVSyncInfo.i64ClockTime = (gint64)GST_VALID_TIME_AS_MSECONDS(
      gst_clock_get_time(GST_ELEMENT_CLOCK(GST_ELEMENT(bsink))));
  }else{
    asink->audio_sink_info.stAVSyncInfo.i64ClockTime = -1LL;
  }
  if(GST_CLOCK_TIME_IS_VALID(asink->audio_sink_info.stAVSyncInfo.i64ClockTime)){
    asink->audio_sink_info.stAVSyncInfo.i64ClockRunningTime =
      asink->audio_sink_info.stAVSyncInfo.i64ClockTime -
      asink->audio_sink_info.stAVSyncInfo.i64BaseTime;
  }
  asink->audio_sink_info.stAVSyncInfo.i64BufferPts = (gint64)GST_VALID_TIME_AS_MSECONDS(GST_BUFFER_PTS (buffer));
  asink->audio_sink_info.stAVSyncInfo.i64Duration = (gint64)GST_VALID_TIME_AS_MSECONDS(GST_BUFFER_DURATION(buffer));

  if(asink->audio_sink_info.stAVSyncInfo.dbRate != 0.0){

    if(0 == bsink->slow_presync_info.slow_presync_state){
      asink->audio_sink_info.stAVSyncInfo.i64BufferRunningTime = (gint64)((((gint64)GST_VALID_TIME_AS_MSECONDS(
        GST_BUFFER_PTS (buffer)) - asink->audio_sink_info.stAVSyncInfo.i64SegStart) /
          ABS(asink->audio_sink_info.stAVSyncInfo.dbRate))
          + asink->audio_sink_info.stAVSyncInfo.i64Segbase);
    }else if (1 == bsink->slow_presync_info.slow_presync_state){
      gdouble tmp_rate = asink->audio_sink_info.stAVSyncInfo.dbRate;

      if(bsink->slow_presync_info.gdrate != 0.0){
        tmp_rate = bsink->slow_presync_info.gdrate;
      }
      asink->audio_sink_info.stAVSyncInfo.i64BufferRunningTime = (gint64)((((gint64)GST_VALID_TIME_AS_MSECONDS(
        GST_BUFFER_PTS (buffer)) - asink->audio_sink_info.stAVSyncInfo.i64SegStart) /
        ABS(tmp_rate))
        + asink->audio_sink_info.stAVSyncInfo.i64Segbase);
    }else{
      gint64 tmp_start = (gint64)GST_VALID_TIME_AS_MSECONDS(bsink->slow_presync_info.last_presync_pts);
      asink->audio_sink_info.stAVSyncInfo.i64BufferRunningTime = (gint64)((((gint64)GST_VALID_TIME_AS_MSECONDS(
        GST_BUFFER_PTS (buffer)) - tmp_start) /
        ABS(asink->audio_sink_info.stAVSyncInfo.dbRate))
        + asink->audio_sink_info.stAVSyncInfo.i64Segbase
        + (gint64)GST_VALID_TIME_AS_MSECONDS(bsink->slow_presync_info.slow_presync_stime));
    }
  }else{
    asink->audio_sink_info.stAVSyncInfo.i64BufferRunningTime = ((gint64)GST_VALID_TIME_AS_MSECONDS(
      GST_BUFFER_PTS (buffer)) - asink->audio_sink_info.stAVSyncInfo.i64SegStart)
      + asink->audio_sink_info.stAVSyncInfo.i64Segbase;
  }

  asink->audio_sink_info.stAVSyncInfo.i64MaxLatency = (gint64)GST_VALID_TIME_AS_MSECONDS(
    gst_base_sink_get_max_lateness(bsink));
  asink->audio_sink_info.stAVSyncInfo.i64CurRenderDelay = (gint64)GST_VALID_TIME_AS_MSECONDS(
    gst_base_sink_get_render_delay(bsink));
  if(gst_base_sink_get_sync(bsink)){
    asink->audio_sink_info.stAVSyncInfo.i64Diff = asink->audio_sink_info.stAVSyncInfo.i64ClockRunningTime -
      (asink->audio_sink_info.stAVSyncInfo.i64BufferRunningTime -
      asink->audio_sink_info.stAVSyncInfo.i64CurRenderDelay);
  }else{
    asink->audio_sink_info.stAVSyncInfo.i64Diff = 0LL;
  }

  gst_smeaudiosink2_dump_sink_file(bsink, (const guchar *)(info.data), info.size);
#endif

  GST_BASE_SINK_PREROLL_UNLOCK (bsink);
  pu8Data = info.data;
  inbuffer_len = info.size;

  if(GST_SME_OVERRUN != gst_base_sink_get_buffer_status(bsink)
#ifdef AVSYNC_BY_AUDIO_QOS_UNDERRUN
    && GST_SME_UNDERRUN != gst_base_sink_get_buffer_status(bsink)
#endif
  ){
  }else{
    gfloat change_speed_coef;

    if (GST_SME_OVERRUN == gst_base_sink_get_buffer_status(bsink)){
      change_speed_coef = SPEED_UP_COEFFICIENT;
      pu8InBuffer = (guchar*)malloc(info.size);
    }else{
      change_speed_coef = SPEED_DOWN_COEFFICIENT;
      pu8InBuffer = (guchar*)malloc((guint)(info.size / SPEED_DOWN_COEFFICIENT));
    }

    if(NULL == pu8InBuffer){
      GST_ERROR_OBJECT(bsink, "malloc size:%d failed", info.size);
      goto exit;
    }
    inbuffer_len = 0;

    (void)gst_smeaudiosink_change_speed(asink, info.data,
      info.size, pu8InBuffer,
      &inbuffer_len, change_speed_coef);
    pu8Data = pu8InBuffer;

    if (GST_SME_OVERRUN == gst_base_sink_get_buffer_status(bsink)){
      GST_WARNING_OBJECT(asink, "%s:audio GST_SME_OVERRUN,len:%d",
        AVSYNC_BY_AUDIO_TAG, inbuffer_len);
    }else{
      GST_WARNING_OBJECT(asink, "%s:audio GST_SME_UNDERRUN,len:%d",
        AVSYNC_BY_AUDIO_TAG, inbuffer_len);
    }
  }

  if (asink->first_write_flag) {
        gboolean cache_time_enough_flag = TRUE;
        asink->current_cache_pts = GST_BUFFER_PTS(buffer);
        if (GST_CLOCK_TIME_NONE == asink->first_cache_pts) {
            asink->first_cache_pts = GST_BUFFER_PTS(buffer);
        }
        if (GST_CLOCK_TIME_NONE != asink->first_cache_pts
          && GST_CLOCK_TIME_NONE != asink->current_cache_pts) {
            if ((gint64)(asink->current_cache_pts - asink->first_cache_pts) <= asink->cache_audio_dur_time
              && asink->cache_audio_dur_time > 0LL) {
                cache_time_enough_flag = FALSE;
            }
        }
        if (asink->write_cache_len + inbuffer_len <= MAX_CACHE_SIZE
          && (!cache_time_enough_flag)) { // copy to cache
            eReturn = VOS_Memcpy_S(asink->write_cache + asink->write_cache_len, inbuffer_len, pu8Data, inbuffer_len);
            if(E_SME_MEM_OK != eReturn){
              GST_WARNING_OBJECT(asink, "VOS_Memcpy_S asink->write_cache Failed(%d)", eReturn);
            }
            asink->write_cache_len += inbuffer_len;
            write_flag = FALSE;
        } else { // cache enough, write cache
            GST_WARNING_OBJECT(asink, "write audio cache: pts %lld-%lld=%lld, cache_len %d",
                asink->current_cache_pts,
                asink->first_cache_pts,
                asink->current_cache_pts-asink->first_cache_pts,
                asink->write_cache_len);

            if (asink->write_cache_len > 0 && klass->write) {
                guint offset,write_len;
                write_len = inbuffer_len;
                for (offset = 0; offset+write_len <= asink->write_cache_len; offset += write_len) {
                    if (offset+write_len >= asink->write_cache_len)
                        write_len = asink->write_cache_len - offset;
                    if (write_len > 0)
                        eSmeRet = klass->write(asink, (const gchar *)(asink->write_cache+offset), (gint)write_len);
                    //GST_INFO_OBJECT(asink, "gst_smeaudiosink2_render write first pts:%lld/%lld, size:%p,%d",asink->first_cache_pts,
                    //    asink->current_cache_pts, asink->write_cache+offset,write_len);
                }
            }
            asink->first_write_flag = 0;
            asink->write_cache_len = 0;
        }
  }

  if (write_flag && klass->write) {
    //g_mutex_lock(&asink->lock);
    eSmeRet = klass->write(asink, (const gchar *)pu8Data, (gint)inbuffer_len);
    //g_mutex_unlock(&asink->lock);
  }

exit:
  gst_buffer_unmap (buffer, &info);
  if(pu8InBuffer){
    free(pu8InBuffer);
  }

    if(E_SME_AUDIO_PLAYOUT_RET_SUCCESS != eSmeRet)
    {
        if (!g_atomic_int_get(&asink->flushing_flag)) {
            eRet = GST_FLOW_ERROR;
            GST_ERROR_OBJECT(asink, "SME_WriteAudio: Failed, return 0x%x",
                         eSmeRet);
        } else {
            eRet = GST_FLOW_FLUSHING;
            GST_ERROR_OBJECT(asink, "SME_WriteAudio: GST_FLOW_FLUSHING, return 0x%x",
                         eSmeRet);
        }
    }

  GST_INFO_OBJECT(asink, "gst_smeaudiosink2_render out");
  GST_BASE_SINK_PREROLL_LOCK (bsink);
  return eRet;
}
//lint +e666

static gboolean
gst_smeaudiosink2_send_event (GstElement * element, GstEvent * event)
{
    return GST_ELEMENT_CLASS(parent_class)->send_event (element, event);
}

static gboolean
gst_smeaudiosink2_event (GstBaseSink * bsink, GstEvent * event)
{
    GstSmeAudiosink2* asink = GST_SME_AUDIOSINK2(bsink);
    GstSmeAudiosink2Class *klass = GST_SME_AUDIOSINK2_GET_CLASS(asink);
    gboolean bres;
    GstEventType event_type;
    GstSegment segment;

    GST_INFO("receive event %s", GST_EVENT_TYPE_NAME(event));
    event_type = GST_EVENT_TYPE(event);
    if(GST_EVENT_EOS == event_type) {
        asink->recieve_eos = TRUE;
    } else if (GST_EVENT_FLUSH_START == GST_EVENT_TYPE(event)) {
        g_atomic_int_set(&asink->flushing_flag, 1);
        if (klass->stop) {
            GST_INFO("FLUSH_START begin flush ....");
            g_mutex_lock(&asink->lock);
            klass->stop(asink);
            g_mutex_unlock(&asink->lock);
            GST_INFO("FLUSH_START begin end");
        }
    } else if (GST_EVENT_FLUSH_STOP == event_type){
        asink->first_write_flag = 1;
        asink->write_cache_len = 0;
        asink->first_cache_pts = GST_CLOCK_TIME_NONE;
        asink->current_cache_pts = GST_CLOCK_TIME_NONE;
        if (klass->flush) {
            klass->flush(asink);
        }
        if (klass->start) {
            GST_INFO("FLUSH_STOP begin flush ....");
            g_mutex_lock(&asink->lock);
            klass->start(asink);
            g_mutex_unlock(&asink->lock);
            GST_INFO("FLUSH_STOP begin end");
        }
        asink->recieve_eos = FALSE;
        g_atomic_int_set(&asink->sync_count, SME_ASINK2_DEFAULT_SYNC_COUNT);
        asink->need_sync = TRUE;
        g_atomic_int_set(&asink->flushing_flag, 0);

    }else if (GST_EVENT_SEGMENT == event_type){
        gst_segment_init(&segment, GST_FORMAT_TIME);
        gst_event_copy_segment (event, &segment);
    }else{
        ;//do nothing
    }

    bres = GST_BASE_SINK_CLASS (parent_class)->event (bsink, event);

    if (GST_EVENT_FLUSH_STOP == event_type){
      //gboolean reset_time;
      //gst_event_parse_flush_stop (event, &reset_time);
      if(GST_OBJECT_FLAG_IS_SET (asink, GST_ELEMENT_FLAG_PROVIDE_CLOCK)){
        //if flush stop we will sync the first audio,and if reset time = 0
        //we will reset audio buffer running time in basesink.
        //no max_lateness
        gst_base_sink_set_max_lateness (GST_BASE_SINK (asink),
          SME_DEFAULT_AUDIO_LATENESS);
        gst_base_sink_set_sync(GST_BASE_SINK (asink), TRUE);
        asink->need_sync = TRUE;
        GST_WARNING_OBJECT(asink, "%s:enable audio avsync !", AVSYNC_BY_AUDIO_TAG);
      }
      if(asink->pts_calibration_hdl){
        gst_sme_pts_calibration_entry_clear(asink->pts_calibration_hdl,
          E_ENTRY_TYPE_AUDIO);
      }
    }else if (GST_EVENT_SEGMENT == event_type){
        if(asink->pts_calibration_hdl){
          gst_sme_pts_calibration_entry_update_segment(asink->pts_calibration_hdl,
            E_ENTRY_TYPE_AUDIO, segment);
          gst_sme_pts_calibration_entry_update_presync_info(asink->pts_calibration_hdl,
            E_ENTRY_TYPE_AUDIO, bsink->slow_presync_info);
        }
#ifdef SMEPLAYER_BENCHMARK_OPEN
        asink->audio_sink_info.stAVSyncInfo.i64Segbase = (gint64)GST_VALID_TIME_AS_MSECONDS(segment.base);
        asink->audio_sink_info.stAVSyncInfo.i64SegStart = (gint64)GST_VALID_TIME_AS_MSECONDS(segment.start);
        asink->audio_sink_info.stAVSyncInfo.i64SegStop = (gint64)GST_VALID_TIME_AS_MSECONDS(segment.stop);
        asink->audio_sink_info.stAVSyncInfo.dbRate = segment.rate;
#endif
    }

    return bres;
}

static gboolean
gst_smeaudiosink2_query (GstBaseSink * bsink, GstQuery * query)
{
  gboolean ret = TRUE;

  switch (GST_QUERY_TYPE (query)) {
    //这里能子实现需要处理的type
    default:
      ret = GST_BASE_SINK_CLASS (parent_class)->query (bsink, query);
      break;
  }

  return ret;
}

static void
gst_smeaudiosink2_get_times (GstBaseSink * basesink,
    GstBuffer * buffer, GstClockTime * start, GstClockTime * end)
{
    GstClockTime timestamp = 0, duration = 0;
    GstSmeAudiosink2* asink = GST_SME_AUDIOSINK2(basesink);
    GstSmeAudiosink2Class *aclass = GST_SME_AUDIOSINK2_GET_CLASS(asink);

    /* if the buffer's discontinuity is set, re-sync */
    if (GST_BUFFER_FLAG_IS_SET (buffer, GST_BUFFER_FLAG_DISCONT)) {
        g_atomic_int_set(&asink->sync_count, SME_ASINK2_DEFAULT_SYNC_COUNT);
        asink->need_sync = TRUE;
    }

    /** Do sync the buffer when need_sync flag is set,
     * otherwise, just free run.
     */
    if (asink->need_sync) {
        /* first sync on DTS, else use PTS */
        timestamp = GST_BUFFER_DTS (buffer);
        if (!GST_CLOCK_TIME_IS_VALID (timestamp)){
            timestamp = GST_BUFFER_PTS (buffer);
        }

        if (GST_CLOCK_TIME_IS_VALID (timestamp)) {
            /* get duration to calculate end time */
            duration = GST_BUFFER_DURATION (buffer);
            if (GST_CLOCK_TIME_IS_VALID (duration)) {
                *end = timestamp + duration;
            }
            *start = timestamp;
        } else {
            /**
             * Don't support sme buffer format anymore.
             * The format should be generic.
             */
            *start = GST_CLOCK_TIME_NONE;
            *end = GST_CLOCK_TIME_NONE;
        }
    } else {
        *start = GST_CLOCK_TIME_NONE;
        *end = GST_CLOCK_TIME_NONE;
    }

    // Get render delay
    if (aclass->get_delay){
      GstClockTime render_delay = GST_CLOCK_TIME_NONE;
      if (E_SME_AUDIO_PLAYOUT_RET_SUCCESS == aclass->get_delay(asink, &render_delay)){
        if (GST_CLOCK_TIME_NONE != render_delay){
          gst_base_sink_set_render_delay(basesink, render_delay);
        }
      }
    }

    return ;
}

static void
gst_smeaudiosink2_calibrate_pts (GstBaseSink * bsink, GstBuffer * buffer,
  gboolean *bcalibrated)
{
  GstSmeAudiosink2* asink = GST_SME_AUDIOSINK2(bsink);
  GstClockTime in_pts, out_pts;
  GstClockTime in_duration, out_duration;
  gboolean bcalibration = FALSE;

  if(NULL != bcalibrated){
    *bcalibrated = FALSE;
  }

  g_return_if_fail(NULL != buffer);
  if(asink->pts_calibration_hdl){
    in_pts = GST_BUFFER_PTS(buffer);
    in_duration = GST_BUFFER_DURATION(buffer);
    bcalibration = gst_sme_pts_calibration_entry_execute(asink->pts_calibration_hdl,
      E_ENTRY_TYPE_AUDIO, in_pts, in_duration, &out_pts, &out_duration);
    GST_BUFFER_PTS(buffer) = out_pts;
    GST_BUFFER_DTS(buffer) = out_pts;
    GST_BUFFER_DURATION(buffer) = out_duration;
  }

  if(bcalibrated){
    *bcalibrated = bcalibration;
  }

  return;
}

#if 0
/* 100ms */
#define SME_ASINK2_SYNC_THRESH  100000000
static gboolean
gst_smeaudiosink2_check_sync (GstBaseSink * bsink, GstBuffer * buffer)
{
    gboolean ret = FALSE;
    GstSegment *segment;
    guint64 cstart, cstop;        /* clipped raw timestamps */
    GstClockTime start, stop;     /* raw start/stop timestamps */
    GstClockTime timestamp = 0, duration = 0;
    GstClock *local_clock;
    guint64 cur_running_time;
    guint64 rstart;
    gint delay_ms = 0;
    GstSmeAudiosink2 *asink = GST_SME_AUDIOSINK2(bsink);
    GstSmeAudiosink2Class *klass = GST_SME_AUDIOSINK2_GET_CLASS(asink);
    GstClockTime render_delay, latency;
    GstClockTimeDiff ts_offset;

    segment = &bsink->segment;

    /* first sync on DTS, else use PTS */
    timestamp = GST_BUFFER_DTS (buffer);
    if (!GST_CLOCK_TIME_IS_VALID (timestamp)){
        timestamp = GST_BUFFER_PTS (buffer);
    }

    if (GST_CLOCK_TIME_IS_VALID (timestamp)) {
        /* get duration to calculate end time */
        duration = GST_BUFFER_DURATION (buffer);
        if (GST_CLOCK_TIME_IS_VALID (duration)) {
            stop = timestamp + duration;
        } else {
            stop = timestamp;
        }

        start = timestamp;
    } else {
        return FALSE;
    }

    if (segment->format != GST_FORMAT_TIME) {
        return FALSE;
    }

    gst_segment_clip (segment, segment->format,
                      start, stop, &cstart, &cstop);

    rstart = gst_segment_to_running_time (segment, segment->format, cstart);

    GST_DEBUG_OBJECT (bsink, "start %" GST_TIME_FORMAT ", cstart %"
                      GST_TIME_FORMAT ", rstart %" GST_TIME_FORMAT
                      " duration %" GST_TIME_FORMAT,
                      GST_TIME_ARGS(start), GST_TIME_ARGS(cstart),
                      GST_TIME_ARGS(rstart), GST_TIME_ARGS(duration));

    local_clock = gst_element_get_clock (GST_ELEMENT_CAST (bsink));
    if (local_clock) {
        GstClockTime base_time;
        cur_running_time = gst_clock_get_time (local_clock);
#ifdef SMEPLAYER_BENCHMARK_OPEN
        asink->audio_sink_info.ClockForSync = (gint64)cur_running_time;
        asink->audio_sink_info.AudioRunningTime = cur_running_time - gst_element_get_base_time(GST_ELEMENT(asink));
#endif
        base_time = gst_element_get_base_time (GST_ELEMENT_CAST (bsink));
        GST_DEBUG_OBJECT (bsink, "base time %" GST_TIME_FORMAT,
                          GST_TIME_ARGS(base_time));
        if (base_time <= cur_running_time)
            cur_running_time -= base_time;
        else
            cur_running_time = 0;
        GST_DEBUG_OBJECT (bsink, "current clock %" GST_TIME_FORMAT,
                          GST_TIME_ARGS(cur_running_time));
        gst_object_unref (local_clock);
    } else {
        return FALSE;
    }

    if (klass->get_delay) {
        klass->get_delay(asink, &delay_ms);
        GST_DEBUG_OBJECT (bsink, "delay %d ms", delay_ms);
    }

    /* adjust the time with delay and offset */
    latency = gst_base_sink_get_latency (bsink);
    rstart += latency;

    ts_offset = gst_base_sink_get_ts_offset(bsink);
    if (ts_offset < 0) {
        ts_offset = -ts_offset;
        if (ts_offset < (gint64)rstart)
            rstart -= (guint64)ts_offset;
        else
            rstart = 0;
    } else {
        rstart += (guint64)ts_offset;
    }

    render_delay = gst_base_sink_get_render_delay (bsink);
    render_delay += ((guint64)(guint)delay_ms)*GST_MSECOND;
    if (rstart > render_delay) {
        rstart -= render_delay;
    } else {
        rstart = 0;
    }

    GST_DEBUG_OBJECT (bsink, "latency %" GST_TIME_FORMAT
                      " ts_offset %" GST_TIME_FORMAT
                      " render_delay %" GST_TIME_FORMAT,
                      GST_TIME_ARGS(latency),
                      GST_TIME_ARGS(ts_offset),
                      GST_TIME_ARGS(render_delay));

    if (((rstart > cur_running_time) &&
         (rstart - cur_running_time > SME_ASINK2_SYNC_THRESH)) ||
        ((rstart < cur_running_time) &&
         (cur_running_time - rstart > SME_ASINK2_SYNC_THRESH))) {
        ret = TRUE;
        GST_INFO_OBJECT (bsink, "timestamp %" GST_TIME_FORMAT
                         ", diffs with clock %" GST_TIME_FORMAT
                         ": need re-sync, delay %d ms",
                         GST_TIME_ARGS(rstart),
                         GST_TIME_ARGS(cur_running_time),
                         delay_ms);
    }

#ifdef SMEPLAYER_BENCHMARK_OPEN
    if ( ret ) {
        asink->audio_sink_info.AudioDiscardCnt++;
        asink->audio_sink_info.AudioFrameIsDelay = 1;
    } else {
        asink->audio_sink_info.AudioFrameIsDelay = 0;
    }
#endif

    return ret;
}
#endif
