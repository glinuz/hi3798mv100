/******************************************************************************
<hisi Tec.>
Copyright (c) 2016 - 2020 Huawei Software, Inc. All rights reserved.
All software, firmware and related documentation herein ("Huawei Tec.") are
intellectual property of hisi Tec. and protected by
law, including, but not limited to, copyright law and international treaties.
Any use, modification, reproduction, retransmission, or republication of all
or part of hisi Tec. is expressly prohibited, unless prior written
permission has been granted by hisi Tec.
*******************************************************************************/

//used in dvbplaybin

#include <stdio.h>
#include <gst/gst.h>
#include "gstsme.h"
#include "gst/base/gstadapter.h"
#include "tvos_hal_av.h"
#include "osal_mem.h"
#ifdef ENABLE_PCR_CLOCK
#include "gstsmetsdmxclock.h"
#endif
#include "gstsmetsdmx.h"

//lint -e717
//lint -e826
//lint -e585
//lint -e801
//lint -e413
//lint -e459
//lint -e457
//lint -esym(749, PROP_0, PROP_LAST)
//lint -esym(528, gst_smetsdmx_get_instance_private)
//lint -esym(652, GST_CAT_DEFAULT)
enum
{
    PROP_0 = 0,
    PROP_PRG_MEDIA_INFO,
    PROP_PRG_CA_INFO,

    PROP_VIDEO_EVEN_KEY_INFO,
    PROP_VIDEO_ODD_KEY_INFO,
    PROP_AUDIO_EVEN_KEY_INFO,
    PROP_AUDIO_ODD_KEY_INFO,

#ifdef ENABLE_KEY_IV
    PROP_VIDEO_EVEN_KEY_IV,
    PROP_VIDEO_ODD_KEY_IV,
    PROP_AUDIO_EVEN_KEY_IV,
    PROP_AUDIO_ODD_KEY_IV,
#endif
    PROP_USE_PCR_CLOCK,
    PROP_EXIT_BLOCK,
    PROP_PRG_AUDIO_INFO,
    PROP_HWTSDMX_INFO,
    PROP_LAST,
};

/* Element signals and args */
enum
{
    PAD_CAPS_CHANGED = 0,
    /* add more above */
    LAST_SIGNAL
};

#define MAPI_DMX_INVALID_PID  (0x1fff)
//#define DMX_FILE_SOURCE_ID    (0x80)
//#define DMX_TUNER_SOURCE_ID   (0x20)//default is for public
#define DEFAULT_USE_PCR_CLOCK FALSE
#define AUDIO_USE_DEFAULT_PROP //audio use default channel/samplerate/layer...
#define TSDMXHAL_INVALID_PTS (4294967295ULL)
#define TSDMX_HAVE_NO_STREAM_TIME (2 * 1000)

#ifndef SMEPLAYER_STAT_TAG_TIME
#define SMEPLAYER_STAT_TAG_TIME "PlayerTimeLineTrace"
#endif

#ifndef LINUX_OS
static hw_module_t const* g_dmx_module = NULL;
#endif
static guint g_smetsdmx_signals[LAST_SIGNAL] = {0};

static guint g_su32tstunerid = 0x20; //default is for public.

GST_DEBUG_CATEGORY_STATIC (gst_smetsdmx_debug);
#define GST_CAT_DEFAULT gst_smetsdmx_debug

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate g_sink_caps_tmpl = GST_STATIC_PAD_TEMPLATE ("sink",
        GST_PAD_SINK,
        GST_PAD_ALWAYS,
        GST_STATIC_CAPS ("video/mpegts_hw"));

static GstStaticPadTemplate g_video_src_caps_tmpl =
    GST_STATIC_PAD_TEMPLATE ("video_src_%02d",
                             GST_PAD_SRC,
                             GST_PAD_SOMETIMES,
                             GST_STATIC_CAPS_ANY);

static GstStaticPadTemplate g_audio_src_caps_tmpl =
    GST_STATIC_PAD_TEMPLATE ("audio_src_%02d",
                             GST_PAD_SRC,
                             GST_PAD_SOMETIMES,
                             GST_STATIC_CAPS_ANY);

#define gst_smetsdmx_parent_class parent_class

//lint -e19
G_DEFINE_TYPE (GstSmeTSDMX, gst_smetsdmx, GST_TYPE_ELEMENT);
//lint +e19

static void gst_smetsdmx_set_property (GObject* object, guint prop_id,
                                       const GValue* value, GParamSpec* pspec);
static void gst_smetsdmx_get_property (GObject* object, guint prop_id,
                                       GValue* value, GParamSpec* pspec);
static void
gst_smetsdmx_finalize (GObject* object);

static GstStateChangeReturn
gst_smetsdmx_change_state (GstElement* element, GstStateChange transition);

#ifdef ENABLE_PCR_CLOCK
static GstClock*
gst_smetsdmx_provide_clock (GstElement* element);
static GstClockTime
gst_smetsdmx_get_time (GstClock* clock, GstSmeTSDMX* tsdmx);
static void
gst_smetsdmx_set_provide_clock (GstSmeTSDMX* tsdmx, gboolean provide);
#endif

static gboolean
gst_smetsdmx_send_event(GstElement* element, GstEvent* event);

#if 0
static gboolean
gst_smetsdmx_sink_event (GstPad* pad, GstObject* parent, GstEvent* event);
static GstFlowReturn
gst_smetsdmx_chain (GstPad* pad, GstObject* parent, GstBuffer* buf);
#endif

static gboolean
gst_smetsdmx_open_pcr(GstSmeTSDMX* tsdmx);
static gboolean
gst_smetsdmx_close_pcr(GstSmeTSDMX* tsdmx);

static void*
proc_video_stream_thread_func(void* arg);

static void*
proc_audio_stream_thread_func(void* arg);

static void
destroy_video_audio_thread(GstSmeTSDMX* tsdmx);

static gboolean
gst_smetsdmx_audio_flush_start(GstSmeTSDMX* tsdmx, GstEvent* event);

static gboolean
gst_smetsdmx_audio_flush_stop(GstSmeTSDMX* tsdmx, GstEvent* event);

static guint64
gst_smetsdmx_get_cur_systime();


static void
open_dump_v(GstSmeTSDMX* tsdmx)
{
#ifdef TSDMX_DUMP_ES_STREAM
    gchar filename[512] = {0,};

    if(!tsdmx->stProcInfo.stCmdInfo.bDumpSrcVideoStream)
    {
        return;
    }

    if (tsdmx->esfile_v)
    {
        fclose(tsdmx->esfile_v);
        tsdmx->esfile_v = NULL;
    }

    if (MAPI_DMX_INVALID_PID == tsdmx->prog_media_info.vpid)
    {
        GST_WARNING_OBJECT(tsdmx, "no video");
        return;
    }

    if(g_snprintf(filename, sizeof(filename) - 1, "%s/video_%d.es",
               tsdmx->stProcInfo.stCmdInfo.cDumpFileStorePath,
               tsdmx->prog_media_info.vpid) <= 0)
    {
        GST_WARNING_OBJECT(tsdmx, "dump video file name failed, why?");
    }
    tsdmx->esfile_v = fopen(filename, "wb+");
    if (NULL == tsdmx->esfile_v)
    {
        GST_ERROR_OBJECT(tsdmx, "open video dump file failed,errno=%d,filename:%s",
            errno, filename);
    }
#else
    (void)tsdmx;
#endif
    return;
}

static void
close_dump_v(GstSmeTSDMX* tsdmx)
{
#ifdef TSDMX_DUMP_ES_STREAM
    if (tsdmx->esfile_v)
    {
        fclose(tsdmx->esfile_v);
        tsdmx->esfile_v = NULL;
    }
#else
    (void)tsdmx;
#endif

    return;
}

static void
dump_v(GstSmeTSDMX* tsdmx, const guchar* data, guint len)
{
#ifdef TSDMX_DUMP_ES_STREAM
    gint ret;

    if(!tsdmx->stProcInfo.stCmdInfo.bDumpSrcVideoStream)
    {
        close_dump_v(tsdmx);
        return;
    }

    if(!tsdmx->esfile_v)
    {
        open_dump_v(tsdmx);
    }

    if (tsdmx->esfile_v)
    {
        ret = fwrite(data, 1, len, tsdmx->esfile_v);
        if (ret == 0)
        {
            GST_WARNING_OBJECT(tsdmx, "dump video data,fwrite failed, why?");
        }
    }
#else
    (void)tsdmx;
    (void)data;
    (void)len;
#endif

    return;
}

static void
open_dump_a(GstSmeTSDMX* tsdmx)
{
#ifdef TSDMX_DUMP_ES_STREAM
    gchar filename[512] = {0,};

    if(!tsdmx->stProcInfo.stCmdInfo.bDumpSrcAudioStream)
    {
        return;
    }

    if (tsdmx->esfile_a)
    {
        fclose(tsdmx->esfile_a);
        tsdmx->esfile_a = NULL;
    }

    if (MAPI_DMX_INVALID_PID == tsdmx->prog_media_info.apid)
    {
        GST_WARNING_OBJECT(tsdmx, "no audio");
        return;
    }

    if (g_snprintf(filename, sizeof(filename) - 1, "%s/audio_%d.es",
                tsdmx->stProcInfo.stCmdInfo.cDumpFileStorePath,
                   tsdmx->prog_media_info.apid) <= 0)
    {
        GST_WARNING_OBJECT(tsdmx, "dump audio file name failed, why?");
    }
    tsdmx->esfile_a = fopen(filename, "wb+");
    if (NULL == tsdmx->esfile_a)
    {
        GST_ERROR_OBJECT(tsdmx, "open audio dump file failed,errno=%d,filename:%s",
            errno, filename);
    }
#else
    (void)tsdmx;
#endif
    return;
}

static void
close_dump_a(GstSmeTSDMX* tsdmx)
{
#ifdef TSDMX_DUMP_ES_STREAM
    if (tsdmx->esfile_a)
    {
        fclose(tsdmx->esfile_a);
        tsdmx->esfile_a = NULL;
    }
#else
    (void)tsdmx;
#endif

    return;
}

static void
dump_a(GstSmeTSDMX* tsdmx, const guchar* data, guint len)
{
#ifdef TSDMX_DUMP_ES_STREAM
    gint ret;
    if(!tsdmx->stProcInfo.stCmdInfo.bDumpSrcAudioStream)
    {
        close_dump_a(tsdmx);
        return;
    }

    if(!tsdmx->esfile_a)
    {
        open_dump_a(tsdmx);
    }

    if (tsdmx->esfile_a)
    {
        ret = fwrite(data, 1, len, tsdmx->esfile_a);
        if (ret == 0)
        {
            GST_WARNING_OBJECT(tsdmx, "dump audio data,fwrite failed, why?");
        }

    }
#else
    (void)tsdmx;
    (void)data;
    (void)len;
#endif

    return;
}

static void
gst_smetsdmx_get_vcodec_name(gchar* name, guint size, E_DVB_VID_CODEC_ID vcodec_id)
{
    gint res;

    switch (vcodec_id)
    {
        case E_DVB_VID_CODEC_TYPE_MPEG1:
            res = g_snprintf(name, size - 1, "%s", "video/mpeg1");
            break;
        case E_DVB_VID_CODEC_TYPE_MPEG2:
            res = g_snprintf(name, size - 1, "%s", "video/mpeg2");
            break;
        case E_DVB_VID_CODEC_TYPE_MPEG4:
            res = g_snprintf(name, size - 1, "%s", "video/mpeg4");
            break;
        case E_DVB_VID_CODEC_TYPE_H264:
            res = g_snprintf(name, size - 1, "%s", "video/H.264");
            break;
        case E_DVB_VID_CODEC_TYPE_MVC:
            res = g_snprintf(name, size - 1, "%s", "video/mvc");
            break;
        case E_DVB_VID_CODEC_TYPE_H265:
            res = g_snprintf(name, size - 1, "%s", "video/H.265");
            break;
        case E_DVB_VID_CODEC_TYPE_AVS:
            res = g_snprintf(name, size - 1, "%s", "video/avs");
            break;
        case E_DVB_VID_CODEC_TYPE_WMV:
            res = g_snprintf(name, size - 1, "%s", "video/wmv");
            break;
        case E_DVB_VID_CODEC_TYPE_AVS_PLUS:
            res = g_snprintf(name, size - 1, "%s", "video/avs+");
            break;
        default:
            res = g_snprintf(name, size - 1, "(%d)not support!", vcodec_id);
            break;
    }

    if(res <= 0)
    {
        GST_INFO("dump audio codec name failed why?");
    }

    return;
}

static void
gst_smetsdmx_get_acodec_name(gchar* name, guint size, E_DVB_AUD_CODEC_ID acodec_id)
{
    gint res;
    switch (acodec_id)
    {
        case E_DVB_AUD_CODEC_TYPE_MP1:
            res = g_snprintf(name, size - 1, "%s", "audio/mpeg1");
            break;
        case E_DVB_AUD_CODEC_TYPE_MP2:
            res = g_snprintf(name, size - 1, "%s", "audio/mpeg2");
            break;
        case E_DVB_AUD_CODEC_TYPE_MP3:
            res = g_snprintf(name, size - 1, "%s", "audio/mpeg3");
            break;
        case E_DVB_AUD_CODEC_TYPE_LPCM:
            res = g_snprintf(name, size - 1, "%s", "audio/lpcm");
            break;
        case E_DVB_AUD_CODEC_TYPE_AC3:
            res = g_snprintf(name, size - 1, "%s", "audio/ac3");
            break;
        case E_DVB_AUD_CODEC_TYPE_EAC3:
            res = g_snprintf(name, size - 1, "%s", "audio/eac3");
            break;
        case E_DVB_AUD_CODEC_TYPE_DTS:
            res = g_snprintf(name, size - 1, "%s", "audio/dts");
            break;
        case E_DVB_AUD_CODEC_TYPE_DRA:
            res = g_snprintf(name, size - 1, "%s", "audio/dra");
            break;
        case E_DVB_AUD_CODEC_TYPE_AAC:
            res = g_snprintf(name, size - 1, "%s", "audio/aac");
            break;
        default:
            res = g_snprintf(name, size - 1, "(%d)not support!", acodec_id);
            break;
    }

    if(res <= 0)
    {
        GST_INFO("dump audio codec name failed why?");
    }

    return;
}

static void
gst_smetsdmx_set_proc_info(GstSmeTSDMX* tsdmx,
  ST_SMEHWTSDEMUX_PROC_INFO* proc_info)
{
    if(E_SME_PROC_CMD_SET_CMD == proc_info->eProcCmdType)
    {
        tsdmx->stProcInfo.stCmdInfo = proc_info->stCmdInfo;

        GST_INFO_OBJECT(tsdmx, "set tsdmxid:%d,dumpvideo:%d,dumpaudio:%d,cur_path:%s,toset path:%s",
            tsdmx->stProcInfo.stCmdInfo.u32TunerID,
            tsdmx->stProcInfo.stCmdInfo.bDumpSrcVideoStream,
            tsdmx->stProcInfo.stCmdInfo.bDumpSrcAudioStream,
            tsdmx->stProcInfo.stCmdInfo.cDumpFileStorePath,
            proc_info->stCmdInfo.cDumpFileStorePath);
    }
    else
    {
        proc_info->stShowInfo.i32PcrSync = (gint)GST_OBJECT_FLAG_IS_SET (tsdmx,
            GST_ELEMENT_FLAG_PROVIDE_CLOCK);
        proc_info->stShowInfo.i32AudioIsCa = (gint)tsdmx->prog_media_info.audio_is_ca;
        proc_info->stShowInfo.i32VideoIsCa = (gint)tsdmx->prog_media_info.video_is_ca;
        proc_info->stShowInfo.u32Apid = tsdmx->prog_media_info.apid;
        proc_info->stShowInfo.u32Vpid = tsdmx->prog_media_info.vpid;
        proc_info->stShowInfo.u32TunerID = g_su32tstunerid;
        gst_smetsdmx_get_vcodec_name(proc_info->stShowInfo.acVCodecName,
            sizeof(proc_info->stShowInfo.acVCodecName),
            tsdmx->prog_media_info.e_vtype);
        gst_smetsdmx_get_acodec_name(proc_info->stShowInfo.acACodecName,
            sizeof(proc_info->stShowInfo.acACodecName),
            tsdmx->prog_media_info.e_atype);
    }

    return;
}

/* initialize the tsdmx's class */
static void
gst_smetsdmx_class_init (GstSmeTSDMXClass* klass)
{
    GObjectClass* gobject_class;
    GstElementClass* gstelement_class;

    GST_INFO("gst_smetsdmx_class_init in");

    gobject_class = (GObjectClass*) klass;
    gstelement_class = (GstElementClass*) klass;

    gobject_class->set_property = gst_smetsdmx_set_property;
    gobject_class->get_property = gst_smetsdmx_get_property;
    gobject_class->finalize = GST_DEBUG_FUNCPTR (gst_smetsdmx_finalize);

    gstelement_class->change_state = gst_smetsdmx_change_state;
#ifdef ENABLE_PCR_CLOCK
    gstelement_class->provide_clock = GST_DEBUG_FUNCPTR (gst_smetsdmx_provide_clock);
#endif
    gstelement_class->send_event = gst_smetsdmx_send_event;
    //lint -e655
    // 0. The property of ts program media info
    g_object_class_install_property(gobject_class, PROP_PRG_MEDIA_INFO,
                                    g_param_spec_pointer(GST_SME_PROP_KEY_DVB_MEDIA_INFO,
                                            "program media infomation", "program media infomation",
                                            (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    // 1. The property of ts program ca info
    g_object_class_install_property(gobject_class, PROP_PRG_CA_INFO,
                                    g_param_spec_pointer(GST_SME_PROP_KEY_DVB_CA_INFO, "program CA infomation",
                                            "program CA infomation",
                                            (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    // 2. The property of video even key info
    g_object_class_install_property(gobject_class, PROP_VIDEO_EVEN_KEY_INFO,
                                    g_param_spec_pointer(GST_SME_PROP_KEY_DVB_VIDEO_EVEN_KEY,
                                            "video even key info", "set video even key info",
                                            (GParamFlags)G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

    // 3. The property of video odd key info
    g_object_class_install_property(gobject_class, PROP_VIDEO_ODD_KEY_INFO,
                                    g_param_spec_pointer(GST_SME_PROP_KEY_DVB_VIDEO_ODD_KEY, "video odd key info",
                                            "set video odd key info",
                                            (GParamFlags)G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

    // 4. The property of audio even key info
    g_object_class_install_property(gobject_class, PROP_AUDIO_EVEN_KEY_INFO,
                                    g_param_spec_pointer(GST_SME_PROP_KEY_DVB_AUDIO_EVEN_KEY,
                                            "audio even key info", "set audio even key info",
                                            (GParamFlags)G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

    // 5. The property of audio odd key info
    g_object_class_install_property(gobject_class, PROP_AUDIO_ODD_KEY_INFO,
                                    g_param_spec_pointer(GST_SME_PROP_KEY_DVB_AUDIO_ODD_KEY,
                                            "audio odd key info", "set audio odd key info",
                                            (GParamFlags)G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));
#ifdef ENABLE_KEY_IV
    // 6. The property of set even iv information
    g_object_class_install_property(gobject_class, PROP_VIDEO_EVEN_KEY_IV,
                                    g_param_spec_pointer(GST_SME_PROP_KEY_DVB_VIDEO_EVEN_KEY_IV,
                                            "set video even key iv",
                                            "descrambler set video even key iv",
                                            (GParamFlags)G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

    // 7. The property of set even iv information
    g_object_class_install_property(gobject_class, PROP_VIDEO_ODD_KEY_IV,
                                    g_param_spec_pointer(GST_SME_PROP_KEY_DVB_VIDEO_ODD_KEY_IV,
                                            "set video odd key iv",
                                            "descrambler set video odd key iv",
                                            (GParamFlags)G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

    // 8. The property of set even iv information
    g_object_class_install_property(gobject_class, PROP_AUDIO_EVEN_KEY_IV,
                                    g_param_spec_pointer(GST_SME_PROP_KEY_DVB_AUDIO_EVEN_KEY_IV,
                                            "set audio even key iv",
                                            "descrambler set audio even key iv",
                                            (GParamFlags)G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

    // 9. The property of set even iv information
    g_object_class_install_property(gobject_class, PROP_AUDIO_ODD_KEY_IV,
                                    g_param_spec_pointer(GST_SME_PROP_KEY_DVB_AUDIO_ODD_KEY_IV,
                                            "set audio odd key iv",
                                            "descrambler set audio odd key iv",
                                            (GParamFlags)G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));
#endif
    // 10. The property of provide clock
    g_object_class_install_property (gobject_class, PROP_USE_PCR_CLOCK,
                                     g_param_spec_boolean ("use-pcr-clock", "Use PCR Clock",
                                             "Use PCR clock to provide AV sync reference clock",
                                             DEFAULT_USE_PCR_CLOCK, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    // 11. The property of provide clock
    g_object_class_install_property (gobject_class, PROP_EXIT_BLOCK,
                                     g_param_spec_boolean ("gst-sme-prop-key-exit-block", "exit play",
                                             "exit play", FALSE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    // 12. The property of ts program audio info
    g_object_class_install_property(gobject_class, PROP_PRG_AUDIO_INFO,
                                    g_param_spec_pointer(GST_SME_PROP_KEY_DVB_AUDIO_INFO,
                                            "program audio infomation", "program audio infomation",
                                            (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_HWTSDMX_INFO,
        g_param_spec_pointer (GST_SME_BENCH_MARK_HWTSDEMUX_INFO, "hwtsdmx proc info",
          "poniter of hwtsdmx proc info",
          (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    //lint +e655
    g_smetsdmx_signals[PAD_CAPS_CHANGED] =
        g_signal_new ("caps-changed", G_TYPE_FROM_CLASS (klass), G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (GstSmeTSDMXClass, pad_caps_changed), NULL, NULL,
                      g_cclosure_marshal_generic, G_TYPE_NONE, 2, GST_TYPE_PAD, GST_TYPE_CAPS);

    gst_element_class_set_details_simple(gstelement_class,
                                         "smetsdmx", "hisisemi", "hisisemi Element", " <<user@hisisemi.com>>");

    gst_element_class_add_pad_template (gstelement_class,
                                        gst_static_pad_template_get (&g_video_src_caps_tmpl));
    gst_element_class_add_pad_template (gstelement_class,
                                        gst_static_pad_template_get (&g_audio_src_caps_tmpl));
    gst_element_class_add_pad_template (gstelement_class,
                                        gst_static_pad_template_get (&g_sink_caps_tmpl));

    gst_element_class_set_static_metadata (gstelement_class,
                                           "MPEG transport stream demuxer", "Codec/Demuxer", "Demuxes TS streams",
                                           "hisisemi <@hisisemi.com>");

    GST_INFO("gst_smetsdmx_class_init out");
}

#ifdef ENABLE_PCR_CLOCK
static GstClock*
gst_smetsdmx_provide_clock (GstElement* element)
{
    GstSmeTSDMX* tsdmx = GST_SMETSDMX(element);

    GST_OBJECT_LOCK (tsdmx);

    if (!GST_OBJECT_FLAG_IS_SET (tsdmx, GST_ELEMENT_FLAG_PROVIDE_CLOCK))
    {
        GST_OBJECT_UNLOCK (tsdmx);
        return NULL;
    }

    GST_OBJECT_UNLOCK (tsdmx);

    return GST_CLOCK_CAST (gst_object_ref (tsdmx->provided_clock));
}
#endif

static void
gst_smetsdmx_reset(GstSmeTSDMX* tsdmx)
{
    (void)VOS_Memset_S(&tsdmx->prog_media_info, sizeof(tsdmx->prog_media_info),
                       0, sizeof(tsdmx->prog_media_info));
    tsdmx->prog_media_info.vpid = MAPI_DMX_INVALID_PID;
    tsdmx->prog_media_info.apid = MAPI_DMX_INVALID_PID;

    (void)VOS_Memset_S(&tsdmx->applied_prog_media_info, sizeof(tsdmx->applied_prog_media_info),
                       0, sizeof(tsdmx->applied_prog_media_info));
    tsdmx->applied_prog_media_info.vpid = MAPI_DMX_INVALID_PID;
    tsdmx->applied_prog_media_info.apid = MAPI_DMX_INVALID_PID;

    (void)VOS_Memset_S(&tsdmx->prog_ca_key_info, sizeof(tsdmx->prog_ca_key_info),
                       0, sizeof(tsdmx->prog_ca_key_info));
    tsdmx->prog_ca_key_info.video_descramble_id = (U32)-1;
    tsdmx->prog_ca_key_info.audio_descramble_id = (U32)-1;
    (void)VOS_Memset_S(&tsdmx->prog_ca_info, sizeof(tsdmx->prog_ca_info),
                       0, sizeof(tsdmx->prog_ca_info));

    tsdmx->video_proc_thread_id = (pthread_t)0;
    tsdmx->evideo_proc_thread_state = E_TSDMX_THD_STATE_IDLE;
    tsdmx->audio_proc_thread_id = (pthread_t)0;
    tsdmx->eaudio_proc_thread_state = E_TSDMX_THD_STATE_IDLE;

    tsdmx->bexit = FALSE;
    tsdmx->bvideo_need_new_segment = TRUE;
    tsdmx->baudio_need_new_segment = TRUE;

    tsdmx->v_first_pts = GST_CLOCK_TIME_NONE;
    tsdmx->a_first_pts = GST_CLOCK_TIME_NONE;

    tsdmx->bchanging_audio_track = FALSE;

    tsdmx->v_last_no_str_t = 0;
    tsdmx->v_cur_no_str_t = 0;
    tsdmx->a_last_no_str_t = 0;
    tsdmx->a_cur_no_str_t = 0;
    tsdmx->have_no_audio = FALSE;
    tsdmx->have_no_video = FALSE;
    tsdmx->post_no_streamer = FALSE;
    tsdmx->audio_need_wait_validpts = FALSE;

    if (tsdmx->audio_srcpad)
    {
        gst_element_remove_pad(GST_ELEMENT_CAST(tsdmx), tsdmx->audio_srcpad);
        tsdmx->audio_srcpad = NULL;
    }

    if (tsdmx->video_srcpad)
    {
        gst_element_remove_pad(GST_ELEMENT_CAST(tsdmx), tsdmx->video_srcpad);
        tsdmx->video_srcpad = NULL;
    }
}

static void
gst_smetsdmx_init (GstSmeTSDMX* tsdmx)
{
    E_SME_MEM_SECURE_RET eRet;

    GST_INFO_OBJECT(tsdmx, "tsmdx_init in");

    tsdmx->sinkpad = gst_pad_new_from_static_template (&g_sink_caps_tmpl, "sink");
#ifdef ENABLE_PCR_CLOCK
    tsdmx->provided_clock = gst_smetsdmx_clock_new("smetsdmxclock",
                            (GstSmeTSDMXClockGetTimeFunc)gst_smetsdmx_get_time, tsdmx);
    gst_object_set_parent (GST_OBJECT (tsdmx->provided_clock), GST_OBJECT (tsdmx));
    //lint -e774
    if (DEFAULT_USE_PCR_CLOCK)
    {
        GST_OBJECT_FLAG_SET (tsdmx, GST_ELEMENT_FLAG_PROVIDE_CLOCK);
    }
    else
#endif
    {
        GST_OBJECT_FLAG_UNSET (tsdmx, GST_ELEMENT_FLAG_PROVIDE_CLOCK);
    }
    //lint +e774
#if 0
    gst_pad_set_event_function (tsdmx->sinkpad,
                                GST_DEBUG_FUNCPTR(gst_smetsdmx_sink_event));
    gst_pad_set_chain_function (tsdmx->sinkpad,
                                GST_DEBUG_FUNCPTR(gst_smetsdmx_chain));
#endif
    gst_element_add_pad (GST_ELEMENT (tsdmx), tsdmx->sinkpad);


    g_mutex_init(&tsdmx->video_proc_mutex);
    g_cond_init(&tsdmx->video_proc_cond);
    g_mutex_init(&tsdmx->audio_proc_mutex);
    g_cond_init(&tsdmx->audio_proc_cond);
    g_mutex_init(&tsdmx->av_proc_mutex);

    eRet = VOS_Memset_S(&tsdmx->tsdmxhal, sizeof(tsdmx->tsdmxhal),
                       0, sizeof(tsdmx->tsdmxhal));
    if(eRet != E_SME_MEM_OK)
    {
        GST_WARNING_OBJECT(tsdmx, "VOS_Memset_S tsdmx hal failed!");
    }

    eRet = VOS_Memset_S(&tsdmx->stProcInfo, sizeof(tsdmx->stProcInfo),
                       0, sizeof(tsdmx->stProcInfo));
    if(eRet != E_SME_MEM_OK)
    {
        GST_WARNING_OBJECT(tsdmx, "VOS_Memset_S tsdmx stProcInfo failed!");
    }

    if(g_snprintf(tsdmx->stProcInfo.stCmdInfo.cDumpFileStorePath,
      sizeof(tsdmx->stProcInfo.stCmdInfo.cDumpFileStorePath) - 1,
      "%s", TSDMX_ES_STREAM_DIR) <= 0)
    {
        GST_WARNING_OBJECT(tsdmx, "dump dir failed why ?");
    }
    tsdmx->tsdmxhal.video_filter_id = (U32)-1;
    tsdmx->tsdmxhal.audio_filter_id = (U32)-1;
    tsdmx->tsdmxhal.pcr_filter_id   = (U32)-1;
    tsdmx->bchanging_audio_track = FALSE;

    gst_smetsdmx_reset(tsdmx);
    GST_INFO_OBJECT(tsdmx, "tsmdx_init out");
}

static void
gst_smetsdmx_finalize (GObject* object)
{
    GstSmeTSDMX* tsdmx = GST_SMETSDMX(object);

    GST_INFO("gst_smetsdmx_finalize object=%p, name =%s",
             object, GST_ELEMENT_NAME(tsdmx));

    destroy_video_audio_thread(tsdmx);

#ifdef ENABLE_PCR_CLOCK
    if (tsdmx->provided_clock)
    {
        gst_object_unparent(GST_OBJECT_CAST(tsdmx->provided_clock));
        gst_smetsdmx_clock_invalidate(tsdmx->provided_clock);
        gst_object_unref(tsdmx->provided_clock);
        tsdmx->provided_clock = NULL;
    }
#endif
    g_mutex_clear(&tsdmx->video_proc_mutex);
    g_cond_clear(&tsdmx->video_proc_cond);
    g_mutex_clear(&tsdmx->audio_proc_mutex);
    g_cond_clear(&tsdmx->audio_proc_cond);
    g_mutex_clear(&tsdmx->av_proc_mutex);
    G_OBJECT_CLASS (parent_class)->finalize (object);

    GST_INFO("gst_smetsdmx_finalize object=%p", object);
}

#ifdef ENABLE_PCR_CLOCK
static GstClockTime calibrate_pcrtime(GstClockTime pcrtime, GstClockTime scrtime)
{
    // pcrtime frequency : 90KHz
    // scrtime frequency : 1KHz
    // ((pcrtime / 90) + scrtime ) * 1000000ULL;
    GST_WARNING("@cat_clock@calibrate@ pcrtime : %"GST_TIME_FORMAT", scrtime : %"GST_TIME_FORMAT,
        GST_TIME_ARGS(pcrtime),
        GST_TIME_ARGS(scrtime));
    return ((pcrtime * 100000ULL) /9) + scrtime * 1000000ULL;
}


//lint -e529
static GstClockTime
gst_smetsdmx_get_time (GstClock* pcr_clock, GstSmeTSDMX* tsdmx)
{
    GstClockTime pcrtime;
    GstClockTime scrtime;
    S32 err;

    (void)pcr_clock;
    /*if (tsdmx->bSetPcrBaseTime == FALSE)
    {
      pcrtime = tsdmx->u64PcrBaseTime;
      GST_INFO_OBJECT(tsdmx, "return BaseTime to clock %lld", pcrtime);
      tsdmx->bSetPcrBaseTime = TRUE;
      return pcrtime;
    }*/
#ifdef LINUX_OS
    scrtime = GST_CLOCK_TIME_NONE;
    err = tsdmx->tsdmxhal.hal_handle->dmx_pcr_get(tsdmx->tsdmxhal.hal_handle,
            tsdmx->tsdmxhal.pcr_filter_id, &pcrtime);

    if (err != SUCCESS)
    {
        GST_ERROR_OBJECT(tsdmx, "Failed to dmx_pcr_get pid%d",
                         tsdmx->prog_media_info.pcrpid);
    }
    else
    {
        pcrtime = calibrate_pcrtime(pcrtime, scrtime);
        //_internal_should_resetTime(tsdmx, &pcrtime);
    }
#else
    err = tsdmx->tsdmxhal.hal_handle->dmx_pcr_get(tsdmx->tsdmxhal.hal_handle,
            tsdmx->tsdmxhal.pcr_filter_id, &pcrtime, &scrtime);

    if (err != SUCCESS)
    {
        GST_ERROR_OBJECT(tsdmx, "Failed to dmx_pcr_get pid%d",
                         tsdmx->prog_media_info.pcrpid);
    }
    else
    {
        pcrtime = calibrate_pcrtime(pcrtime, scrtime);
        //_internal_should_resetTime(tsdmx, &pcrtime);
    }
#endif

    return pcrtime;
}
//lint +e529

static void
gst_smetsdmx_set_provide_clock (GstSmeTSDMX* tsdmx, gboolean provide)
{
    GST_OBJECT_LOCK (tsdmx);

    if (provide)
    {
        GST_OBJECT_FLAG_SET (tsdmx, GST_ELEMENT_FLAG_PROVIDE_CLOCK);
    }
    else
    {
        GST_OBJECT_FLAG_UNSET (tsdmx, GST_ELEMENT_FLAG_PROVIDE_CLOCK);
    }

    GST_OBJECT_UNLOCK (tsdmx);

    return;
}
#endif

static AV_VID_STREAM_TYPE_E
trans_video_codec_to_hal(E_DVB_VID_CODEC_ID e_vid_codec_id)
{
    AV_VID_STREAM_TYPE_E eret = AV_VID_STREAM_TYPE_BUTT;

    switch (e_vid_codec_id)
    {
        case E_DVB_VID_CODEC_TYPE_MPEG1://no break
        case E_DVB_VID_CODEC_TYPE_MPEG2:
            eret = AV_VID_STREAM_TYPE_MPEG2;
            break;

        case E_DVB_VID_CODEC_TYPE_MPEG4:
            eret = AV_VID_STREAM_TYPE_MPEG4;
            break;

        case E_DVB_VID_CODEC_TYPE_H264:
            eret = AV_VID_STREAM_TYPE_H264;
            break;

        case E_DVB_VID_CODEC_TYPE_MVC:
            eret = AV_VID_STREAM_TYPE_MVC;
            break;

        case E_DVB_VID_CODEC_TYPE_H265:
            eret = AV_VID_STREAM_TYPE_HEVC;
            break;

        case E_DVB_VID_CODEC_TYPE_AVS:
            eret = AV_VID_STREAM_TYPE_AVS;
            break;

        case E_DVB_VID_CODEC_TYPE_WMV:
            eret = AV_VID_STREAM_TYPE_VC1;
            break;

        case E_DVB_VID_CODEC_TYPE_AVS_PLUS:
            eret = AV_VID_STREAM_TYPE_AVSPLUS;
            break;

        default:
            GST_WARNING( "video codec id(%d) not support", e_vid_codec_id);
            break;
    }

    return eret;
}

static AV_AUD_STREAM_TYPE_E
trans_audio_codec_to_hal(E_DVB_AUD_CODEC_ID e_aud_codec_id)
{
    AV_AUD_STREAM_TYPE_E eret = AV_AUD_STREAM_TYPE_BUTT;

    switch (e_aud_codec_id)
    {
        case E_DVB_AUD_CODEC_TYPE_MP1:
            eret = AV_AUD_STREAM_TYPE_MP1;
            break;

        case E_DVB_AUD_CODEC_TYPE_MP2:
            eret = AV_AUD_STREAM_TYPE_MP2;
            break;

        case E_DVB_AUD_CODEC_TYPE_MP3:
            eret = AV_AUD_STREAM_TYPE_MP3;
            break;

        case E_DVB_AUD_CODEC_TYPE_LPCM:
            eret = AV_AUD_STREAM_TYPE_PCM;
            break;

        case E_DVB_AUD_CODEC_TYPE_AC3:
            eret = AV_AUD_STREAM_TYPE_AC3;
            break;

        case E_DVB_AUD_CODEC_TYPE_EAC3:
            eret = AV_AUD_STREAM_TYPE_EAC3;
            break;

        case E_DVB_AUD_CODEC_TYPE_DTS:
            eret = AV_AUD_STREAM_TYPE_DTS;
            break;

        case E_DVB_AUD_CODEC_TYPE_DRA:
            eret = AV_AUD_STREAM_TYPE_DRA;
            break;

        case E_DVB_AUD_CODEC_TYPE_AAC:
            eret = AV_AUD_STREAM_TYPE_AAC;
            break;

        default:
            GST_WARNING( "audio codec id(%d) not support", e_aud_codec_id);
            break;
    }

    return eret;
}

static gboolean
set_ca_key_info(GstSmeTSDMX* tsdmx, U32 descramble_id, guint8 ca_type,
                U32 ca_vendor_id, GstCAKeyInfoItem* ca_key_info, gboolean is_even_key)
{
    S32 err = SUCCESS;

    g_assert(tsdmx->tsdmxhal.hal_handle);
    g_assert(descramble_id != (U32)-1);
    g_assert(ca_type <= 3);

    //ca_type, 0 no CA, 1 clear CW, 2 TDES keyladder, 3 AES keyladder
    if (0 == ca_type)
    {
        GST_WARNING_OBJECT(tsdmx, "ca type is 0 !");
        return TRUE;
    }

#ifdef ENABLE_KEY_IV
    if (ca_key_info->u8_iv_valid)
    {
        if (is_even_key)
        {
            err = tsdmx->tsdmxhal.hal_handle->dmx_descrambler_set_even_iv(
                      tsdmx->tsdmxhal.hal_handle, descramble_id, ca_key_info->au8_IV,
                      TVOS_MAX_KEY_LENGTH);
        }
        else
        {
            err = tsdmx->tsdmxhal.hal_handle->dmx_descrambler_set_odd_iv(
                      tsdmx->tsdmxhal.hal_handle, descramble_id, ca_key_info->au8_IV,
                      TVOS_MAX_KEY_LENGTH);
        }

        if (err != SUCCESS)
        {
            GST_WARNING_OBJECT(tsdmx, "call:%s failed for descramble_id:%u",
                               is_even_key ? "set_even_iv" : "set_odd_iv", descramble_id);
            return FALSE;
        }

        ca_key_info->u8_iv_valid = 0;
    }
#endif

    if (ca_key_info->u8_key_valid == 0)
    {
        return TRUE;
    }

    if (2 == ca_type || 3 == ca_type)
    {
        DMX_DCAS_KEYLADDER_SETTING_S dacas_keyladder_settings;

        if (ca_type == 2)
        {
            dacas_keyladder_settings.enKLAlg = DMX_KL_ALG_TDES;
        }
        else
        {
            dacas_keyladder_settings.enKLAlg = DMX_KL_ALG_AES;
        }

        dacas_keyladder_settings.u32CAVid = ca_vendor_id;
        dacas_keyladder_settings.pu8EK1 = ca_key_info->au8_ek1;
        dacas_keyladder_settings.pu8EK2 = ca_key_info->au8_ek2;

        if (is_even_key)
        {
            dacas_keyladder_settings.pu8EvenKey = ca_key_info->au8_ecw;
            dacas_keyladder_settings.pu8OddKey = NULL;
        }
        else
        {
            dacas_keyladder_settings.pu8OddKey = ca_key_info->au8_ecw;
            dacas_keyladder_settings.pu8EvenKey = NULL;
        }

        GST_INFO_OBJECT(tsdmx, "enKLAlg: %d, ca_vener_id: %d ,ek1: %s , ek2: %s ",
                        dacas_keyladder_settings.enKLAlg, dacas_keyladder_settings.u32CAVid,
                        dacas_keyladder_settings.pu8EK1, dacas_keyladder_settings.pu8EK2);
        GST_INFO_OBJECT(tsdmx, "is_even_key: %d , key : %s ",
                        is_even_key, ca_key_info->au8_ecw);
        err = tsdmx->tsdmxhal.hal_handle->dmx_dcas_keyladder_config(
                  tsdmx->tsdmxhal.hal_handle, descramble_id, &dacas_keyladder_settings);

        if (err != SUCCESS)
        {
            GST_WARNING_OBJECT(tsdmx, "failed to call dmx_dcas_keyladder_config "
                               "for descramble_id:%u", descramble_id);
            return FALSE;
        }
    }
    else if (1 == ca_type)
    {
        //Clear CW
        GST_INFO_OBJECT(tsdmx, "is_even_key: %d , key : %s ",
                        is_even_key, ca_key_info->au8_ecw);

        if (is_even_key)
        {
            err = tsdmx->tsdmxhal.hal_handle->dmx_descrambler_set_even_key(
                      tsdmx->tsdmxhal.hal_handle, descramble_id, ca_key_info->au8_ecw,
                      TVOS_MAX_KEY_LENGTH, 0);
        }
        else
        {
            err = tsdmx->tsdmxhal.hal_handle->dmx_descrambler_set_odd_key(
                      tsdmx->tsdmxhal.hal_handle, descramble_id, ca_key_info->au8_ecw,
                      TVOS_MAX_KEY_LENGTH, 0);
        }

        if (SUCCESS != err)
        {
            GST_WARNING_OBJECT(tsdmx, "call %s failed for descramble_id:%u",
                               is_even_key ? "set_even_key" : "set_even_key", descramble_id);
            return FALSE;
        }
    }

    ca_key_info->u8_key_valid = 0;

    return TRUE;
}
static void
update_video_ca_key_info(GstSmeTSDMX* tsdmx)
{
    gboolean bres;

    GST_WARNING_OBJECT(tsdmx, "%s:update_video_ca_key_info in", SMEPLAYER_STAT_TAG_TIME);

    if ((U32)-1 == tsdmx->prog_ca_key_info.video_descramble_id)
    {
        GST_WARNING_OBJECT(tsdmx, "invalid video_descramble_id:-1");
        return;
    }

    bres = set_ca_key_info(tsdmx, tsdmx->prog_ca_key_info.video_descramble_id,
                           tsdmx->prog_ca_info.video_ca_type, tsdmx->prog_ca_info.ca_vendor_id,
                           &tsdmx->prog_ca_key_info.video_evenKey, (gboolean)TRUE);

    if (!bres)
    {
        GST_WARNING_OBJECT(tsdmx, "failed to set video even key info, descramble_id:%u",
                           tsdmx->prog_ca_key_info.video_descramble_id);
    }

    bres = set_ca_key_info(tsdmx, tsdmx->prog_ca_key_info.video_descramble_id,
                           tsdmx->prog_ca_info.video_ca_type, tsdmx->prog_ca_info.ca_vendor_id,
                           &tsdmx->prog_ca_key_info.video_oddKey, FALSE);

    if (!bres)
    {
        GST_WARNING_OBJECT(tsdmx, "failed to set video odd key info, descramble_id:%u",
                           tsdmx->prog_ca_key_info.video_descramble_id);
    }

    GST_WARNING_OBJECT(tsdmx, "%s:update_video_ca_key_info out", SMEPLAYER_STAT_TAG_TIME);

    return;
}

static void
update_audio_ca_key_info(GstSmeTSDMX* tsdmx)
{
    gboolean bres;

    GST_WARNING_OBJECT(tsdmx, "%s:update_audio_ca_key_info in", SMEPLAYER_STAT_TAG_TIME);

    if ((U32)-1 == tsdmx->prog_ca_key_info.audio_descramble_id)
    {
        GST_WARNING_OBJECT(tsdmx, "invalid audio_descramble_id:-1");
        return;
    }

    bres = set_ca_key_info(tsdmx, tsdmx->prog_ca_key_info.audio_descramble_id,
                           tsdmx->prog_ca_info.audio_ca_type, tsdmx->prog_ca_info.ca_vendor_id,
                           &tsdmx->prog_ca_key_info.audio_evenKey, (gboolean)TRUE);

    if (!bres)
    {
        GST_WARNING_OBJECT(tsdmx, "failed to set audio even key info, descramble_id:%u",
                           tsdmx->prog_ca_key_info.audio_descramble_id);
    }

    bres = set_ca_key_info(tsdmx, tsdmx->prog_ca_key_info.audio_descramble_id,
                           tsdmx->prog_ca_info.audio_ca_type, tsdmx->prog_ca_info.ca_vendor_id,
                           &tsdmx->prog_ca_key_info.audio_oddKey, (gboolean)FALSE);

    if (!bres)
    {
        GST_WARNING_OBJECT(tsdmx, "failed to set audio odd key info, descramble_id:%u",
                           tsdmx->prog_ca_key_info.audio_descramble_id);
    }

    GST_WARNING_OBJECT(tsdmx, "%s:update_audio_ca_key_info out", SMEPLAYER_STAT_TAG_TIME);

    return;
}

static void
gst_smetsdmx_set_prog_media_info (GstSmeTSDMX* tsdmx,
                                  const ST_DVBProgMediaInfo* prog_media_info)
{
    U32 error;
    gboolean bres;
    DMX_SOURCE_PARAMS_S tsdmx_source;
    gchar acodec_name[128] = {0,};
    gchar vcodec_name[128] = {0,};
    GstMessage* msg_resettime;

    GST_INFO_OBJECT(tsdmx, "old:apid=%d, vpid=%d, pcrpid=%d, e_atype=%d,"
                    " e_vtype=%d, demux_id=%d, audio_ca=%d, video_ca=%d",
                    tsdmx->prog_media_info.apid, tsdmx->prog_media_info.vpid,
                    tsdmx->prog_media_info.pcrpid, tsdmx->prog_media_info.e_atype,
                    tsdmx->prog_media_info.e_vtype, tsdmx->prog_media_info.demux_id,
                    tsdmx->prog_media_info.audio_is_ca,
                    prog_media_info->video_is_ca);
    gst_smetsdmx_get_acodec_name(acodec_name, sizeof(acodec_name) - 1, prog_media_info->e_atype);
    gst_smetsdmx_get_vcodec_name(vcodec_name, sizeof(vcodec_name) - 1, prog_media_info->e_vtype);
    GST_INFO_OBJECT(tsdmx, "new:apid=%d, vpid=%d, pcrpid=%d, e_atype=%d(%s),"
                    " e_vtype=%d(%s), demux_id=%d, audio_ca=%d, video_ca=%d",
                    prog_media_info->apid, prog_media_info->vpid,
                    prog_media_info->pcrpid, prog_media_info->e_atype,
                    acodec_name,
                    prog_media_info->e_vtype,
                    vcodec_name,
                    prog_media_info->demux_id,
                    prog_media_info->audio_is_ca,
                    prog_media_info->video_is_ca);

    //we should reset start-time when change-channel
    msg_resettime = gst_message_new_reset_time((GstObject*)tsdmx, 0);
    if(msg_resettime){
      GST_WARNING_OBJECT(tsdmx, "cc post reset-time message now...");
      gst_element_post_message(GST_ELEMENT_CAST(tsdmx), msg_resettime);
    }

    g_mutex_lock(&tsdmx->audio_proc_mutex);
    g_mutex_lock(&tsdmx->video_proc_mutex);
    tsdmx->prog_media_info.video_is_ca = prog_media_info->video_is_ca;
    tsdmx->prog_media_info.audio_is_ca = prog_media_info->audio_is_ca;
    tsdmx->prog_media_info.pcrpid = prog_media_info->pcrpid;
    tsdmx->prog_media_info.demux_id = prog_media_info->demux_id;

    tsdmx->prog_media_info.e_vtype = prog_media_info->e_vtype;
    tsdmx->prog_media_info.vpid = prog_media_info->vpid;
    if (!(tsdmx->prog_media_info.e_vtype > E_DVB_VID_CODEC_TYPE_UNKNOWN
        && tsdmx->prog_media_info.e_vtype <= E_DVB_VID_CODEC_TYPE_AVS_PLUS
        && tsdmx->prog_media_info.e_vtype != E_DVB_VID_CODEC_TYPE_MVC))
    {
        tsdmx->prog_media_info.e_vtype = E_DVB_VID_CODEC_TYPE_UNKNOWN;
        tsdmx->have_no_video = TRUE;
    }

    tsdmx->prog_media_info.e_atype = prog_media_info->e_atype;
    tsdmx->prog_media_info.apid = prog_media_info->apid;
    tsdmx->prog_media_info.audio_bits_per_sample = prog_media_info->audio_bits_per_sample;
    tsdmx->prog_media_info.audio_channel = prog_media_info->audio_channel;
    tsdmx->prog_media_info.audio_samplerate = prog_media_info->audio_samplerate;
    if (!(tsdmx->prog_media_info.e_atype > E_DVB_AUD_CODEC_TYPE_UNKNOWN
        && tsdmx->prog_media_info.e_atype < E_DVB_AUD_CODEC_TYPE_MAX))
    {
        tsdmx->prog_media_info.e_atype = E_DVB_AUD_CODEC_TYPE_UNKNOWN;
        tsdmx->have_no_audio = TRUE;
    }

    g_mutex_unlock(&tsdmx->audio_proc_mutex);
    g_mutex_unlock(&tsdmx->video_proc_mutex);

    bres = gst_smetsdmx_close_pcr(tsdmx);

    if (!bres)
    {
        GST_ELEMENT_WARNING(tsdmx, STREAM, DEMUX,
                            ("close pcr failed"), ("close pcr failed"));
    }

    tsdmx_source.enSourceType = DMX_SOURCE_TUNER;
    if(tsdmx->stProcInfo.stCmdInfo.u32TunerID != 0)
    {
        g_su32tstunerid = tsdmx->stProcInfo.stCmdInfo.u32TunerID;
    }
    tsdmx_source.DMX_SOURCE_U.hSource = g_su32tstunerid;
    //tsdmx_source.enSourceType = DMX_SOURCE_FILE;
    //tsdmx_source.DMX_SOURCE_U.hSource =  DMX_FILE_SOURCE_ID;
    error = (U32)tsdmx->tsdmxhal.hal_handle->dmx_set_source_params(
                tsdmx->tsdmxhal.hal_handle,
                (DMX_ID_E)tsdmx->prog_media_info.demux_id, &tsdmx_source);

    if (error != SUCCESS)
    {
        GST_ERROR_OBJECT(tsdmx, "dmx_set_source_params error!! ret = %#x", error);
        GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                          ("set tsdmx source failed:%#x", error), ("set failed"));
        return ;
    }

    bres = gst_smetsdmx_open_pcr(tsdmx);

    if (!bres)
    {
        GST_ELEMENT_WARNING(tsdmx, STREAM, DEMUX,
                            ("open pcr failed"), ("open pcr failed"));
    }

    g_mutex_lock(&tsdmx->video_proc_mutex);
    GST_INFO("set video thread stat to E_TSDMX_THD_STATE_INIT_HAL");
    tsdmx->evideo_proc_thread_state = E_TSDMX_THD_STATE_TO_IDLE_THEN_INIT;
    g_cond_signal(&tsdmx->video_proc_cond);
    g_mutex_unlock(&tsdmx->video_proc_mutex);

    g_mutex_lock(&tsdmx->audio_proc_mutex);
    GST_INFO("set audio thread stat to E_TSDMX_THD_STATE_INIT_HAL");
    tsdmx->eaudio_proc_thread_state = E_TSDMX_THD_STATE_TO_IDLE_THEN_INIT;
    g_cond_signal(&tsdmx->audio_proc_cond);
    g_mutex_unlock(&tsdmx->audio_proc_mutex);

    return;
}

static void
gst_smetsdmx_set_prog_audio_info (GstSmeTSDMX* tsdmx,
                                  const ST_DVBProgAudioInfo* prog_audio_info)
{
    gboolean bres;
    GstEvent* pevent_flush_start;
    GstEvent* pevent_flush_stop;

    GST_INFO_OBJECT(tsdmx, "old:apid=%d, e_atype=%d, audio_is_ca=%d",
                    tsdmx->prog_media_info.apid,
                    tsdmx->prog_media_info.e_atype,
                    tsdmx->prog_media_info.audio_is_ca);
    GST_INFO_OBJECT(tsdmx, "new:apid=%d, e_atype=%d, is_ca=%d",
                    prog_audio_info->apid,
                    prog_audio_info->e_atype,
                    prog_audio_info->is_ca);

    g_mutex_lock(&tsdmx->audio_proc_mutex);
    tsdmx->prog_media_info.audio_is_ca = prog_audio_info->is_ca;

    tsdmx->prog_media_info.e_atype = prog_audio_info->e_atype;
    tsdmx->prog_media_info.apid = prog_audio_info->apid;
    if (!(tsdmx->prog_media_info.e_atype > E_DVB_AUD_CODEC_TYPE_UNKNOWN
        && tsdmx->prog_media_info.e_atype < E_DVB_AUD_CODEC_TYPE_MAX))
    {
        tsdmx->prog_media_info.e_atype = E_DVB_AUD_CODEC_TYPE_UNKNOWN;
    }
    tsdmx->bchanging_audio_track = TRUE;
    g_mutex_unlock(&tsdmx->audio_proc_mutex);

    tsdmx->baudio_flushing = TRUE;
    GST_INFO("send audio flush_start");
    pevent_flush_start = gst_event_new_flush_start();
    bres = gst_smetsdmx_audio_flush_start(tsdmx, pevent_flush_start);
    if (!bres) {
        GST_ERROR("send audio flush_start failed,res:%d",bres);
    }

    GST_INFO("send audio flush_stop");
    pevent_flush_stop = gst_event_new_flush_stop(FALSE);
    bres = gst_smetsdmx_audio_flush_stop(tsdmx, pevent_flush_stop);
    GST_INFO("send audio flush_stop out");
    if (!bres) {
        GST_ERROR("send audio flush_stop failed,res:%d",bres);
    }

    tsdmx->baudio_flushing = FALSE;
    GST_INFO("gst_smetsdmx_set_prog_audio_info out ");
    return;
}

static void
gst_smetsdmx_set_prog_ca_info (GstSmeTSDMX* tsdmx,
                               const ST_DVBProgCAInfo* prog_ca_info)
{
    if (!tsdmx->prog_media_info.audio_is_ca &&
        !tsdmx->prog_media_info.video_is_ca)
    {
        GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                          ("prog_media_info.is_ca = 0,why come here"),
                          ("prog_media_info.is_ca = 0,why come here"));
        return;
    }

    GST_INFO_OBJECT(tsdmx, "old:acatype=%d, ades_type=%d,"
                    " vcatype=%d, vdes_type=%d, ca_vendor_id=%d",
                    tsdmx->prog_ca_info.audio_ca_type,
                    tsdmx->prog_ca_info.audio_stream_descramble_type,
                    tsdmx->prog_ca_info.video_ca_type,
                    tsdmx->prog_ca_info.video_stream_descramble_type,
                    tsdmx->prog_ca_info.ca_vendor_id);
    GST_INFO_OBJECT(tsdmx, "new:acatype=%d, ades_type=%d,"
                    " vcatype=%d, vdes_type=%d, ca_vendor_id=%d",
                    prog_ca_info->audio_ca_type,
                    prog_ca_info->audio_stream_descramble_type,
                    prog_ca_info->video_ca_type,
                    prog_ca_info->video_stream_descramble_type,
                    prog_ca_info->ca_vendor_id);

    tsdmx->prog_ca_info = *prog_ca_info;

    if (tsdmx->prog_ca_info.video_ca_type > 3)
    {
        GST_WARNING_OBJECT(tsdmx, "invalid CA type for video(%d), force to be zero",
                           tsdmx->prog_ca_info.video_ca_type);
        tsdmx->prog_ca_info.video_ca_type = 0;
    }

    if (tsdmx->prog_ca_info.audio_ca_type > 3)
    {
        GST_WARNING_OBJECT(tsdmx, "invalid CA type for audio(%d) , force to be zero",
                           tsdmx->prog_ca_info.audio_ca_type);
        tsdmx->prog_ca_info.audio_ca_type = 0;
    }

    if (tsdmx->prog_ca_info.video_ca_type
        && tsdmx->prog_ca_info.video_stream_descramble_type >= DMX_DESC_TYPE_BUTT)
    {
        GST_WARNING_OBJECT(tsdmx, "invalid descramble type for video(%d), force to be zero",
                           tsdmx->prog_ca_info.video_stream_descramble_type);
    }

    //by lrl:force to DMX_DESC_TYPE_CSA2 ???
    tsdmx->prog_ca_info.video_stream_descramble_type = DMX_DESC_TYPE_CSA2;

    if (tsdmx->prog_ca_info.audio_ca_type
        && tsdmx->prog_ca_info.audio_stream_descramble_type >= DMX_DESC_TYPE_BUTT)
    {
        GST_ERROR_OBJECT(tsdmx, "invalid descramble type for audio(%d), force to be zero",
                         tsdmx->prog_ca_info.audio_stream_descramble_type);
    }

    //by lrl:force to DMX_DESC_TYPE_CSA2 ???
    tsdmx->prog_ca_info.audio_stream_descramble_type = DMX_DESC_TYPE_CSA2;

    return;
}

static void
gst_smetsdmx_set_vid_even_key (GstSmeTSDMX* tsdmx,
                              const ST_CAKeyParam* ca_key_param)
{
    GST_INFO_OBJECT(tsdmx, "set video even key, key_info->u8_ek1:%s,"
                    " key_info->u8_ek2: %s, key_info->u8_ecw : %s",
                    ca_key_param->au8_ek1, ca_key_param->au8_ek2, ca_key_param->au8_ecw);

    g_mutex_lock(&tsdmx->video_proc_mutex);
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.video_evenKey.au8_ek1,
                       sizeof(tsdmx->prog_ca_key_info.video_evenKey.au8_ek1),
                       ca_key_param->au8_ek1,
                       sizeof(tsdmx->prog_ca_key_info.video_evenKey.au8_ek1));
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.video_evenKey.au8_ek2,
                       sizeof(tsdmx->prog_ca_key_info.video_evenKey.au8_ek2),
                       ca_key_param->au8_ek2,
                       sizeof(tsdmx->prog_ca_key_info.video_evenKey.au8_ek2));
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.video_evenKey.au8_ecw,
                       sizeof(tsdmx->prog_ca_key_info.video_evenKey.au8_ecw),
                       ca_key_param->au8_ecw,
                       sizeof(tsdmx->prog_ca_key_info.video_evenKey.au8_ecw));
    tsdmx->prog_ca_key_info.video_evenKey.u8_key_valid = 1;
    update_video_ca_key_info(tsdmx);
    g_mutex_unlock(&tsdmx->video_proc_mutex);

    return;
}

static void
gst_smetsdmx_set_vid_odd_key (GstSmeTSDMX* tsdmx,
                              const ST_CAKeyParam* ca_key_param)
{
    GST_INFO_OBJECT(tsdmx, "set video odd key, key_info->u8_ek1:%s,"
                    " key_info->u8_ek2: %s, key_info->u8_ecw : %s",
                    ca_key_param->au8_ek1, ca_key_param->au8_ek2, ca_key_param->au8_ecw);

    g_mutex_lock(&tsdmx->video_proc_mutex);
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.video_oddKey.au8_ek1,
                       sizeof(tsdmx->prog_ca_key_info.video_oddKey.au8_ek1),
                       ca_key_param->au8_ek1,
                       sizeof(tsdmx->prog_ca_key_info.video_oddKey.au8_ek1));
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.video_oddKey.au8_ek2,
                       sizeof(tsdmx->prog_ca_key_info.video_oddKey.au8_ek2),
                       ca_key_param->au8_ek2,
                       sizeof(tsdmx->prog_ca_key_info.video_oddKey.au8_ek2));
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.video_oddKey.au8_ecw,
                       sizeof(tsdmx->prog_ca_key_info.video_oddKey.au8_ecw),
                       ca_key_param->au8_ecw,
                       sizeof(tsdmx->prog_ca_key_info.video_oddKey.au8_ecw));
    tsdmx->prog_ca_key_info.video_oddKey.u8_key_valid = 1;
    update_video_ca_key_info(tsdmx);
    g_mutex_unlock(&tsdmx->video_proc_mutex);

    return;
}

static void
gst_smetsdmx_set_aud_even_key (GstSmeTSDMX* tsdmx,
                               const ST_CAKeyParam* ca_key_param)
{
    GST_INFO_OBJECT(tsdmx, "set audio even key, key_info->u8_ek1:%s,"
                    " key_info->u8_ek2: %s, key_info->u8_ecw : %s",
                    ca_key_param->au8_ek1, ca_key_param->au8_ek2, ca_key_param->au8_ecw);

    g_mutex_lock(&tsdmx->audio_proc_mutex);
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.audio_evenKey.au8_ek1,
                       sizeof(tsdmx->prog_ca_key_info.audio_evenKey.au8_ek1),
                       ca_key_param->au8_ek1,
                       sizeof(tsdmx->prog_ca_key_info.audio_evenKey.au8_ek1));
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.audio_evenKey.au8_ek2,
                       sizeof(tsdmx->prog_ca_key_info.audio_evenKey.au8_ek2),
                       ca_key_param->au8_ek2,
                       sizeof(tsdmx->prog_ca_key_info.audio_evenKey.au8_ek2));
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.audio_evenKey.au8_ecw,
                       sizeof(tsdmx->prog_ca_key_info.audio_evenKey.au8_ecw),
                       ca_key_param->au8_ecw,
                       sizeof(tsdmx->prog_ca_key_info.audio_evenKey.au8_ecw));
    tsdmx->prog_ca_key_info.audio_evenKey.u8_key_valid = 1;
    update_audio_ca_key_info(tsdmx);
    g_mutex_unlock(&tsdmx->audio_proc_mutex);

    return;
}

static void
gst_smetsdmx_set_aud_odd_key (GstSmeTSDMX* tsdmx,
                              const ST_CAKeyParam* ca_key_param)
{
    GST_INFO_OBJECT(tsdmx, "set audio odd key, key_info->u8_ek1:%s,"
                    " key_info->u8_ek2: %s, key_info->u8_ecw : %s",
                    ca_key_param->au8_ek1, ca_key_param->au8_ek2, ca_key_param->au8_ecw);

    g_mutex_lock(&tsdmx->audio_proc_mutex);
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.audio_oddKey.au8_ek1,
                       sizeof(tsdmx->prog_ca_key_info.audio_oddKey.au8_ek1),
                       ca_key_param->au8_ek1,
                       sizeof(tsdmx->prog_ca_key_info.audio_oddKey.au8_ek1));
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.audio_oddKey.au8_ek2,
                       sizeof(tsdmx->prog_ca_key_info.audio_oddKey.au8_ek2),
                       ca_key_param->au8_ek2,
                       sizeof(tsdmx->prog_ca_key_info.audio_oddKey.au8_ek2));
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.audio_oddKey.au8_ecw,
                       sizeof(tsdmx->prog_ca_key_info.audio_oddKey.au8_ecw),
                       ca_key_param->au8_ecw,
                       sizeof(tsdmx->prog_ca_key_info.audio_oddKey.au8_ecw));
    tsdmx->prog_ca_key_info.audio_oddKey.u8_key_valid = 1;
    update_audio_ca_key_info(tsdmx);
    g_mutex_unlock(&tsdmx->audio_proc_mutex);

    return;
}

#ifdef ENABLE_KEY_IV
static void
gst_smetsdmx_set_vid_even_key_iv (GstSmeTSDMX* tsdmx,
                                  const ST_CAIVParam* ca_iv_param)
{
    g_mutex_lock(&tsdmx->video_proc_mutex);
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.video_evenKey.au8_IV,
                       sizeof(tsdmx->prog_ca_key_info.video_evenKey.au8_IV),
                       ca_iv_param->au8_iv, sizeof(tsdmx->prog_ca_key_info.video_evenKey.au8_IV));
    tsdmx->prog_ca_key_info.video_evenKey.u8_iv_valid = 1;
    update_video_ca_key_info(tsdmx);
    g_mutex_unlock(&tsdmx->video_proc_mutex);

    return;
}

static void
gst_smetsdmx_set_vid_odd_key_iv (GstSmeTSDMX* tsdmx,
                                 const ST_CAIVParam* ca_iv_param)
{
    g_mutex_lock(&tsdmx->video_proc_mutex);
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.video_oddKey.au8_IV,
                       sizeof(tsdmx->prog_ca_key_info.video_oddKey.au8_IV),
                       ca_iv_param->au8_iv, sizeof(tsdmx->prog_ca_key_info.video_oddKey.au8_IV));
    tsdmx->prog_ca_key_info.video_oddKey.u8_iv_valid = 1;
    update_video_ca_key_info(tsdmx);
    g_mutex_unlock(&tsdmx->video_proc_mutex);

    return;
}

static void
gst_smetsdmx_set_aud_even_key_iv (GstSmeTSDMX* tsdmx,
                                  const ST_CAIVParam* ca_iv_param)
{
    g_mutex_lock(&tsdmx->audio_proc_mutex);
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.audio_evenKey.au8_IV,
                       sizeof(tsdmx->prog_ca_key_info.audio_evenKey.au8_IV),
                       ca_iv_param->au8_iv, sizeof(tsdmx->prog_ca_key_info.audio_evenKey.au8_IV));
    tsdmx->prog_ca_key_info.audio_evenKey.u8_iv_valid = 1;
    update_audio_ca_key_info(tsdmx);
    g_mutex_unlock(&tsdmx->audio_proc_mutex);

    return;
}

static void
gst_smetsdmx_set_aud_odd_key_iv (GstSmeTSDMX* tsdmx,
                                 const ST_CAIVParam* ca_iv_param)
{
    g_mutex_lock(&tsdmx->audio_proc_mutex);
    (void)VOS_Memcpy_S(tsdmx->prog_ca_key_info.audio_oddKey.au8_IV,
                       sizeof(tsdmx->prog_ca_key_info.audio_oddKey.au8_IV),
                       ca_iv_param->au8_iv, sizeof(tsdmx->prog_ca_key_info.audio_oddKey.au8_IV));
    tsdmx->prog_ca_key_info.audio_oddKey.u8_iv_valid = 1;
    update_audio_ca_key_info(tsdmx);
    g_mutex_unlock(&tsdmx->audio_proc_mutex);

    return;
}
#endif

static void
gst_smetsdmx_set_property (GObject* object, guint prop_id,
                           const GValue* value, GParamSpec* pspec)
{
    GstSmeTSDMX* tsdmx = GST_SMETSDMX (object);

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_set_property, prop_id=%d in", prop_id);

    switch (prop_id)
    {
        case PROP_PRG_MEDIA_INFO:
        {
            const ST_DVBProgMediaInfo* prog_media_info
                = (const ST_DVBProgMediaInfo*)g_value_get_pointer (value);

            if (NULL == prog_media_info)
            {
                GST_WARNING_OBJECT(tsdmx, "prog_media_info is null, why ?");
                break;
            }

            gst_smetsdmx_set_prog_media_info(tsdmx, prog_media_info);
            break;
        }

        case PROP_PRG_CA_INFO:
        {
            const ST_DVBProgCAInfo* prog_ca_info
                = (const ST_DVBProgCAInfo*)g_value_get_pointer (value);

            if (NULL == prog_ca_info)
            {
                GST_WARNING_OBJECT(tsdmx, "prog_ca_info is null, why ?");
                break;
            }

            gst_smetsdmx_set_prog_ca_info(tsdmx, prog_ca_info);
            break;
        }

        case PROP_VIDEO_EVEN_KEY_INFO:
        {
            const ST_CAKeyParam* ca_key_param = (const ST_CAKeyParam*)g_value_get_pointer (value);

            if (NULL == ca_key_param)
            {
                GST_WARNING_OBJECT(tsdmx, "vid even ca_key_param is null, why ?");
                break;
            }

            gst_smetsdmx_set_vid_even_key(tsdmx, ca_key_param);
            break;
        }

        case PROP_VIDEO_ODD_KEY_INFO:
        {
            const ST_CAKeyParam* ca_key_param = (const ST_CAKeyParam*)g_value_get_pointer (value);

            if (NULL == ca_key_param)
            {
                GST_WARNING_OBJECT(tsdmx, "vid odd ca_key_param is null, why ?");
                break;
            }

            gst_smetsdmx_set_vid_odd_key(tsdmx, ca_key_param);
            break;
        }

        case PROP_AUDIO_EVEN_KEY_INFO:
        {
            const ST_CAKeyParam* ca_key_param = (const ST_CAKeyParam*)g_value_get_pointer (value);

            if (NULL == ca_key_param)
            {
                GST_WARNING_OBJECT(tsdmx, "aud even ca_key_param is null, why ?");
                break;
            }

            gst_smetsdmx_set_aud_even_key(tsdmx, ca_key_param);
            break;
        }

        case PROP_AUDIO_ODD_KEY_INFO:
        {
            const ST_CAKeyParam* ca_key_param = (const ST_CAKeyParam*)g_value_get_pointer (value);

            if (NULL == ca_key_param)
            {
                GST_WARNING_OBJECT(tsdmx, "aud odd ca_key_param is null, why ?");
                break;
            }

            gst_smetsdmx_set_aud_odd_key(tsdmx, ca_key_param);
            break;
        }
#ifdef ENABLE_KEY_IV
        case PROP_VIDEO_EVEN_KEY_IV:
        {
            const ST_CAIVParam* ca_iv_param = (const ST_CAIVParam*)g_value_get_pointer (value);

            if (NULL == ca_iv_param)
            {
                GST_WARNING_OBJECT(tsdmx, "vid even ca_iv_param is null, why ?");
                break;
            }

            gst_smetsdmx_set_vid_even_key_iv(tsdmx, ca_iv_param);
            break;
        }

        case PROP_VIDEO_ODD_KEY_IV:
        {
            const ST_CAIVParam* ca_iv_param = (const ST_CAIVParam*)g_value_get_pointer (value);

            if (NULL == ca_iv_param)
            {
                GST_WARNING_OBJECT(tsdmx, "vid odd ca_iv_param is null, why ?");
                break;
            }

            gst_smetsdmx_set_vid_odd_key_iv(tsdmx, ca_iv_param);
            break;
        }

        case PROP_AUDIO_EVEN_KEY_IV:
        {
            const ST_CAIVParam* ca_iv_param = (const ST_CAIVParam*)g_value_get_pointer (value);

            if (NULL == ca_iv_param)
            {
                GST_WARNING_OBJECT(tsdmx, "aud even ca_iv_param is null, why ?");
                break;
            }

            gst_smetsdmx_set_aud_even_key_iv(tsdmx, ca_iv_param);
            break;
        }

        case PROP_AUDIO_ODD_KEY_IV:
        {
            const ST_CAIVParam* ca_iv_param = (const ST_CAIVParam*)g_value_get_pointer (value);

            if (NULL == ca_iv_param)
            {
                GST_WARNING_OBJECT(tsdmx, "aud odd ca_iv_param is null, why ?");
                break;
            }

            gst_smetsdmx_set_aud_odd_key_iv(tsdmx, ca_iv_param);
            break;
        }
#endif
#ifdef ENABLE_PCR_CLOCK
        case PROP_USE_PCR_CLOCK:
            gst_smetsdmx_set_provide_clock (tsdmx, g_value_get_boolean (value));
            break;
        case PROP_EXIT_BLOCK:
            tsdmx->bexit = g_value_get_boolean (value);
            break;
#endif
        case PROP_PRG_AUDIO_INFO:
        {
            const ST_DVBProgAudioInfo* pstProgAudioInfo
                = (const ST_DVBProgAudioInfo*)g_value_get_pointer (value);
            unsigned int new_apid;
            unsigned int prog_apid;

            if (NULL == pstProgAudioInfo)
            {
                GST_WARNING_OBJECT(tsdmx, "pstProgAudioInfo is null, why ?");
            }else {
                new_apid = pstProgAudioInfo->apid;
                prog_apid = tsdmx->prog_media_info.apid;

                GST_WARNING_OBJECT(tsdmx, "set audio info,apid:%d,prog apid:%d",new_apid, prog_apid);
                if (new_apid != prog_apid)
                    gst_smetsdmx_set_prog_audio_info(tsdmx, pstProgAudioInfo);
            }
            break;
        }
        case PROP_HWTSDMX_INFO:
        {
            ST_SMEHWTSDEMUX_PROC_INFO* pstProcInfo
                = (ST_SMEHWTSDEMUX_PROC_INFO*)g_value_get_pointer (value);
            GST_INFO_OBJECT(tsdmx, "set hwtsdmx proc info");
            if (NULL == pstProcInfo)
            {
                GST_WARNING_OBJECT(tsdmx, "pstProcInfo is null, why ?");
                break;
            }
            gst_smetsdmx_set_proc_info(tsdmx, pstProcInfo);
            break;
        }
        default:
            GST_WARNING_OBJECT(tsdmx, "gst_smetsdmx_set_property, prop_id=%d "
                               "not support, ask for parent_ckass", prop_id);
            G_OBJECT_CLASS(parent_class)->set_property(object, prop_id, value, pspec);
            break;
    }

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_set_property, prop_id=%d out", prop_id);

    return;
}

static void
gst_smetsdmx_get_property (GObject* object, guint prop_id,
                           GValue* value, GParamSpec* pspec)
{
    GstSmeTSDMX* tsdmx = GST_SMETSDMX (object);

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_get_property, prop_id=%d in", prop_id);
    switch (prop_id)
    {
        case PROP_PRG_MEDIA_INFO:
        {
            GST_INFO_OBJECT(tsdmx, "apid=%d, e_atype=%d,audio_is_ca=%d,video_is_ca:%d",
                            tsdmx->prog_media_info.apid,
                            tsdmx->prog_media_info.e_atype,
                            tsdmx->prog_media_info.audio_is_ca,
                            tsdmx->prog_media_info.video_is_ca);

            g_value_set_pointer(value, &tsdmx->prog_media_info);
            break;
        }
        case PROP_HWTSDMX_INFO:
        {
            GST_INFO_OBJECT(tsdmx, "get hwtsdmx proc info");
            g_value_set_pointer(value, &tsdmx->stProcInfo);
            break;
        }
        default:
            GST_WARNING_OBJECT(tsdmx, "gst_smetsdmx_get_property, prop_id=%d "
                               "not support, ask for parent_ckass", prop_id);
            G_OBJECT_CLASS(parent_class)->get_property(object, prop_id, value, pspec);
            break;
    }

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_get_property, prop_id=%d out", prop_id);

    return;
}

static gboolean
gst_smetsdmx_open_pcr(GstSmeTSDMX* tsdmx)
{
    S32 s32Ret;
    const DMX_ID_E edemux_id = (DMX_ID_E)tsdmx->prog_media_info.demux_id;

    if ((U32)-1 != tsdmx->tsdmxhal.pcr_filter_id)
    {
        GST_WARNING_OBJECT(tsdmx, "already open pcr(%#x)!",
                           tsdmx->tsdmxhal.pcr_filter_id);
        return TRUE;
    }

    if (MAPI_DMX_INVALID_PID == tsdmx->prog_media_info.pcrpid)
    {
        GST_ERROR_OBJECT(tsdmx, "pcr id invalid");
        return FALSE;
    }

    if (!tsdmx->tsdmxhal.hal_handle->dmx_pcr_open)
    {
        GST_ERROR_OBJECT(tsdmx, "dmx_pcr_open is null, why ?");
        return FALSE;
    }

    s32Ret = tsdmx->tsdmxhal.hal_handle->dmx_pcr_open(
                 tsdmx->tsdmxhal.hal_handle, edemux_id, &tsdmx->tsdmxhal.pcr_filter_id,
                 tsdmx->prog_media_info.pcrpid);

    if (s32Ret != SUCCESS)
    {
        GST_ERROR_OBJECT(tsdmx, "Failed to open pcr for pid:%d",
                         tsdmx->prog_media_info.pcrpid);
        return FALSE;
    }

    GST_INFO_OBJECT(tsdmx, "pcrpid open success, pcrFilterId: %#x!",
                    tsdmx->tsdmxhal.pcr_filter_id);

    return TRUE;
}

static gboolean
gst_smetsdmx_close_pcr(GstSmeTSDMX* tsdmx)
{
    S32 s32Ret;

    if ((U32)-1 == tsdmx->tsdmxhal.pcr_filter_id)
    {
        GST_WARNING_OBJECT(tsdmx, "pcr not open !");
        return TRUE;
    }

    if (!tsdmx->tsdmxhal.hal_handle->dmx_pcr_close)
    {
        GST_ERROR_OBJECT(tsdmx, "dmx_pcr_close is null, why ?");
        return FALSE;
    }

    s32Ret = tsdmx->tsdmxhal.hal_handle->dmx_pcr_close(
                 tsdmx->tsdmxhal.hal_handle, tsdmx->tsdmxhal.pcr_filter_id);

    if (s32Ret != SUCCESS)
    {
        GST_ERROR_OBJECT(tsdmx, "Failed to close pcr for pcrid:%d",
                         tsdmx->prog_media_info.pcrpid);
        return FALSE;
    }

    tsdmx->tsdmxhal.pcr_filter_id = (U32)-1;

    return TRUE;
}
#if 0
static GstFlowReturn
gst_smetsdmx_chain (GstPad* pad, GstObject* parent, GstBuffer* buf)
{
    (void)pad;
    (void)parent;

    g_usleep(2 * 1000);
    gst_buffer_unref(buf);

    return GST_FLOW_OK;
}

static gboolean
gst_smetsdmx_sink_event (GstPad* pad, GstObject* parent, GstEvent* event)
{
    (void)pad;
    (void)parent;
    (void)event;

    return TRUE;
}
#endif

static gboolean
gst_smetsdmx_audio_flush_start(GstSmeTSDMX* tsdmx, GstEvent* event)
{
    gboolean bret = FALSE;

    if (tsdmx->prog_media_info.e_atype != E_DVB_AUD_CODEC_TYPE_UNKNOWN)
    {
       g_mutex_lock(&tsdmx->audio_proc_mutex);
       tsdmx->eaudio_proc_thread_state = E_TSDMX_THD_STATE_IDLE;
       if (tsdmx->audio_srcpad && GST_PAD_IS_LINKED(tsdmx->audio_srcpad))
       {
           bret = gst_pad_push_event(tsdmx->audio_srcpad, gst_event_ref(event));
           if (!bret)
           {
               GST_ERROR_OBJECT(tsdmx, "audio send flush_start failed, why ?");
           }
       }
       g_mutex_unlock(&tsdmx->audio_proc_mutex);
    }
    gst_event_unref(event);
    return bret;
}

static gboolean
gst_smetsdmx_flush_start(GstSmeTSDMX* tsdmx, GstEvent* event)
{
    gboolean bret = FALSE;

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_flush_start in ...");

    if (tsdmx->prog_media_info.e_vtype != E_DVB_VID_CODEC_TYPE_UNKNOWN)
    {
        g_mutex_lock(&tsdmx->video_proc_mutex);
        tsdmx->evideo_proc_thread_state = E_TSDMX_THD_STATE_IDLE;
        if (tsdmx->video_srcpad && GST_PAD_IS_LINKED(tsdmx->video_srcpad))
        {
            bret = gst_pad_push_event(tsdmx->video_srcpad, gst_event_ref(event));
            if (!bret)
            {
                GST_ERROR_OBJECT(tsdmx, "video send flush_start failed, why ?");
            }
        }
        g_mutex_unlock(&tsdmx->video_proc_mutex);
    }

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_flush_start video end !");
    bret = gst_smetsdmx_audio_flush_start(tsdmx, gst_event_ref(event));

    gst_event_unref(event);

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_flush_start audio end !");
    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_flush_start out");

    return bret;
}

static gboolean
gst_smetsdmx_audio_flush_stop(GstSmeTSDMX* tsdmx, GstEvent* event)
{
    gboolean bret = FALSE;

    if (tsdmx->prog_media_info.e_atype != E_DVB_AUD_CODEC_TYPE_UNKNOWN)
    {
        if (tsdmx->audio_srcpad) {
          GST_PAD_STREAM_LOCK(tsdmx->audio_srcpad);
        }
        if (tsdmx->audio_srcpad && GST_PAD_IS_LINKED(tsdmx->audio_srcpad))
        {
            bret = gst_pad_push_event(tsdmx->audio_srcpad, gst_event_ref(event));
            if (!bret)
            {
                GST_ERROR_OBJECT(tsdmx, "audio send evt_flush_stop failed, why ?");
            }
        }
        if (tsdmx->audio_srcpad) {
          GST_PAD_STREAM_UNLOCK(tsdmx->audio_srcpad);
        }
    }
    gst_event_unref(event);
    return bret;

}


static gboolean
gst_smetsdmx_flush_stop(GstSmeTSDMX* tsdmx, GstEvent* event)
{
    gboolean bret = FALSE;

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_flush_stop in ...");

    if (tsdmx->prog_media_info.e_vtype != E_DVB_VID_CODEC_TYPE_UNKNOWN)
    {
        if (tsdmx->video_srcpad) {
          GST_PAD_STREAM_LOCK(tsdmx->video_srcpad);
        }

        if (tsdmx->video_srcpad && GST_PAD_IS_LINKED(tsdmx->video_srcpad))
        {
            bret = gst_pad_push_event(tsdmx->video_srcpad, gst_event_ref(event));
            if (!bret)
            {
                GST_ERROR_OBJECT(tsdmx, "video send evt_flush_stop failed, why ?");
            }
        }

        if (tsdmx->video_srcpad) {
          GST_PAD_STREAM_UNLOCK(tsdmx->video_srcpad);
        }
    }

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_flush_stop video end !");

    bret = gst_smetsdmx_audio_flush_stop(tsdmx, gst_event_ref(event));

    gst_event_unref(event);

    tsdmx->v_last_no_str_t = 0;
    tsdmx->v_cur_no_str_t = 0;
    tsdmx->a_last_no_str_t = 0;
    tsdmx->a_cur_no_str_t = 0;
    tsdmx->have_no_audio = FALSE;
    tsdmx->have_no_video = FALSE;
    tsdmx->post_no_streamer = FALSE;

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_flush_stop audio end !");

    //todo:wait to a/v thread goto idle.

    return bret;
}

static gboolean
gst_smetsdmx_stop_hal_filters(GstSmeTSDMX* tsdmx, E_DVB_MEDIA_TYPE emedia_type)
{
    S32 res;
    U32* filter_id = NULL;

    if (!tsdmx->tsdmxhal.hal_handle->dmx_avfilter_disable
        || !tsdmx->tsdmxhal.hal_handle->dmx_avfilter_close)
    {
        GST_ERROR_OBJECT(tsdmx, "dmx_avfilter_disable && dmx_avfilter_close"
                         " cannot be null !");
        return FALSE;
    }

    if (E_DVB_MEDIA_TYPE_VIDEO == emedia_type)
    {
        filter_id = &tsdmx->tsdmxhal.video_filter_id;
    }
    else if (E_DVB_MEDIA_TYPE_AUDIO == emedia_type)
    {
        filter_id = &tsdmx->tsdmxhal.audio_filter_id;
    }

    if (NULL == filter_id || (U32)-1 == *filter_id)
    {
        GST_WARNING_OBJECT(tsdmx, "filter of mediatype(%d) not open !", emedia_type);
        return TRUE;
    }

    /* disable av tsdmx */
    res = tsdmx->tsdmxhal.hal_handle->dmx_avfilter_disable(
              tsdmx->tsdmxhal.hal_handle, *filter_id);
    GST_INFO_OBJECT (tsdmx, "Filter:%d disable result:%#x", *filter_id, res);

    res = tsdmx->tsdmxhal.hal_handle->dmx_avfilter_close(
              tsdmx->tsdmxhal.hal_handle, *filter_id);
    GST_INFO_OBJECT (tsdmx, "Filter:%d close result:%#x", *filter_id, res);
    *filter_id = (U32)-1;

    return TRUE;
}

static void
gst_smetsdmx_destory_hal_descrambles(const GstSmeTSDMX* tsdmx, U32 descramble_id)
{
    (void)tsdmx;

    tsdmx->tsdmxhal.hal_handle->dmx_descrambler_disable(tsdmx->tsdmxhal.hal_handle,
            descramble_id);
    tsdmx->tsdmxhal.hal_handle->dmx_descrambler_close(tsdmx->tsdmxhal.hal_handle,
            descramble_id);

    return;
}

static gboolean
gst_smetsdmx_start_hal_filters(GstSmeTSDMX* tsdmx, E_DVB_MEDIA_TYPE emedia_type)
{
    /* open av filter */
    DMX_PARSER_FILTER_OPEN_PARAM_S hal_filter_param;
    S32 err;
    guint32 pid = MAPI_DMX_INVALID_PID;
    guint32* filter_id = NULL;
    const DMX_ID_E edemux_id = (const DMX_ID_E)tsdmx->prog_media_info.demux_id;
    DMX_CHANNEL_TYPE_E e_channel_type = DMX_AUDIO_CHANNEL;

    GST_INFO_OBJECT(tsdmx, "start_hal_filters in, media_type=%d", emedia_type);

    if (E_DVB_MEDIA_TYPE_VIDEO == emedia_type)
    {
        pid = tsdmx->applied_prog_media_info.vpid;
        filter_id = &tsdmx->tsdmxhal.video_filter_id;
        e_channel_type = DMX_VIDEO_CHANNEL;
    }
    else if (E_DVB_MEDIA_TYPE_AUDIO == emedia_type)
    {
        pid = tsdmx->applied_prog_media_info.apid;
        filter_id = &tsdmx->tsdmxhal.audio_filter_id;
        e_channel_type = DMX_AUDIO_CHANNEL;
    }

    if (NULL == filter_id || MAPI_DMX_INVALID_PID == pid)
    {
        GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                          ("start_hal_filters unsupport mediatype(%d) or pid(%#x) invalid",
                           emedia_type, pid),
                          ("start_hal_filters unsupport mediatype(%d) or pid(%#x) invalid",
                           emedia_type, pid));

        return FALSE;
    }

    if ((U32)-1 != *filter_id)
    {
        GST_WARNING_OBJECT(tsdmx, "start_hal_filters mediatype(%d), filter_id(%d) "
                           "already start", emedia_type, *filter_id);
        return TRUE;
    }

    if (!tsdmx->tsdmxhal.hal_handle->dmx_avfilter_open
        || !tsdmx->tsdmxhal.hal_handle->dmx_avfilter_enable)
    {
        GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                          ("dmx_avfilter_open(%p) & dmx_avfilter_enable(%p),emedia_type=%d"
                           " cannot be null", tsdmx->tsdmxhal.hal_handle->dmx_avfilter_open,
                           tsdmx->tsdmxhal.hal_handle->dmx_avfilter_enable, emedia_type),
                          ("dmx_avfilter_open(%p) & dmx_avfilter_enable(%p), emedia_type=%d"
                           " cannot be null", tsdmx->tsdmxhal.hal_handle->dmx_avfilter_open,
                           tsdmx->tsdmxhal.hal_handle->dmx_avfilter_enable, emedia_type));

        return FALSE;
    }

    hal_filter_param.u32Pid = pid;
    hal_filter_param.enType = e_channel_type;
    err = tsdmx->tsdmxhal.hal_handle->dmx_avfilter_open(
              tsdmx->tsdmxhal.hal_handle, edemux_id, filter_id,
              (const DMX_PARSER_FILTER_OPEN_PARAM_S*)&hal_filter_param);

    if (err != SUCCESS)
    {
        GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                          ("open filter %d,&u32VideoFilterId:%p, pid:%u,enChType:%d,"
                           " enDemuxId:%d, emedia_type=%d", err, filter_id,
                           hal_filter_param.u32Pid, hal_filter_param.enType, edemux_id,
                           emedia_type),
                          ("open filter %d,&u32VideoFilterId:%p, pid:%u,enChType:%d,"
                           " enDemuxId:%d, emedia_type=%d", err, filter_id,
                           hal_filter_param.u32Pid, hal_filter_param.enType, edemux_id,
                           emedia_type));
        return FALSE;
    }

    GST_INFO_OBJECT(tsdmx, "open filter %d,u32VideoFilterId:%u, pid:%u,"
                    "enChType:%d, enDemuxId:%d, emedia_type=%d", err, *filter_id,
                    hal_filter_param.u32Pid, hal_filter_param.enType, edemux_id,
                    emedia_type);

    err = tsdmx->tsdmxhal.hal_handle->dmx_avfilter_enable(
              tsdmx->tsdmxhal.hal_handle, *filter_id);

    if (err != SUCCESS)
    {
        tsdmx->tsdmxhal.hal_handle->dmx_avfilter_close(
            tsdmx->tsdmxhal.hal_handle, *filter_id);
        *filter_id = (U32)-1;
        GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                          ("Failed to enable avFilter for pid%d, channelType%d, emedia_type=%d",
                           pid, e_channel_type, emedia_type),
                          ("Failed to enable avFilter for pid%d, channelType%d, emedia_type=%d",
                           pid, e_channel_type, emedia_type));

        return FALSE;
    }

    GST_INFO_OBJECT (tsdmx, "enable filter %d,u32FilterId:%u", err, *filter_id);

    return TRUE;
}

static gboolean
gst_smetsdmx_create_hal_descrambles(GstSmeTSDMX* tsdmx, E_DVB_MEDIA_TYPE emedia_type)
{
    guchar ca_type = 0;
    DMX_DESC_TYPE_E edesc_type = DMX_DESC_TYPE_BUTT;
    const DMX_ID_E edemux_id = (DMX_ID_E)tsdmx->prog_media_info.demux_id;
    U32 pid = 0;
    U32* descramble_id = NULL;
    S32 err;
    U32 hal_channel_id = (U32)-1;
    DMX_DESCRAMBLER_ATTR_S desc_attr;
    DMX_DESC_ASSOCIATE_PARAMS_S  desc_assoc_param;

    GST_INFO_OBJECT(tsdmx, "create_hal_descrambles in, media_type=%d", emedia_type);

    if (E_DVB_MEDIA_TYPE_VIDEO == emedia_type)
    {
        ca_type = tsdmx->prog_ca_info.video_ca_type;
        edesc_type = (DMX_DESC_TYPE_E)tsdmx->prog_ca_info.video_stream_descramble_type;
        descramble_id = &tsdmx->prog_ca_key_info.video_descramble_id;
        pid = tsdmx->applied_prog_media_info.vpid;
    }
    else if (E_DVB_MEDIA_TYPE_AUDIO == emedia_type)
    {
        ca_type = tsdmx->prog_ca_info.audio_ca_type;
        edesc_type = (DMX_DESC_TYPE_E)tsdmx->prog_ca_info.audio_stream_descramble_type;
        descramble_id = &tsdmx->prog_ca_key_info.audio_descramble_id;
        pid = tsdmx->applied_prog_media_info.apid;
    }
    else
    {
        GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                          ("hal_descrambles unsupport mediatype(%d)", emedia_type),
                          ("hal_descrambles unsupport mediatype(%d)", emedia_type));
        return FALSE;
    }

    if (0 == ca_type)
    {
        GST_INFO_OBJECT(tsdmx, "no ca_type");
        return FALSE;
    }

    if ((U32)-1 != *descramble_id)
    {
        GST_WARNING_OBJECT(tsdmx, "descramble_id(%d) already open", *descramble_id);
        return TRUE;
    }

    err = tsdmx->tsdmxhal.hal_handle->dmx_channel_query(tsdmx->tsdmxhal.hal_handle,
            edemux_id, &hal_channel_id, (U16)pid);

    if (err != SUCCESS)
    {
        GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                          ("failed to query channelid for pid:%u, mediatype:%d",
                           pid, emedia_type),
                          ("failed to query channelid for pid:%u, mediatype:%d",
                           pid, emedia_type));
        return FALSE;
    }

    GST_INFO_OBJECT(tsdmx, "get channel:%d,for pid:%d", hal_channel_id, pid);

    g_assert(ca_type >= 1 && ca_type <= 3);
    g_assert(edesc_type < DMX_DESC_TYPE_BUTT);

    //0 no CA, 1 clear CW, 2 TDES, 3 AES
    if (ca_type == 1)
    {
        desc_attr.enCaType = DMX_CA_NORMAL_DESCRAMBLER;
    }
    else
    {
        desc_attr.enCaType = DMX_CA_ADVANCE_DESCRAMBLER;
    }

    desc_attr.enDescramblerType = edesc_type;
    desc_attr.enEntropyReduction = DMX_CA_ENTROPY_REDUCTION_CLOSE;
    err = tsdmx->tsdmxhal.hal_handle->dmx_descrambler_open_ex(
              tsdmx->tsdmxhal.hal_handle, edemux_id, descramble_id, &desc_attr);

    if (err != SUCCESS)
    {
        GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                          ("failed to dmx_descrambler_open_ex %x", err),
                          ("failed to dmx_descrambler_open_ex %x", err));
        return FALSE;
    }

    GST_INFO_OBJECT(tsdmx, "open desc:%d ", *descramble_id);

    desc_assoc_param.enMode = DMX_DESCRAMBLER_ASSOCIATE_WITH_CHANNEL;
    desc_assoc_param.DMX_ASSOCIATE_U.u32ChannelId = hal_channel_id;
    err = tsdmx->tsdmxhal.hal_handle->dmx_descrambler_associate(
              tsdmx->tsdmxhal.hal_handle, *descramble_id, &desc_assoc_param);

    if (err != SUCCESS)
    {
        tsdmx->tsdmxhal.hal_handle->dmx_descrambler_close(tsdmx->tsdmxhal.hal_handle,
                *descramble_id);
        *descramble_id = (U32)-1;
        GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                          ("failed to dmx_descrambler_associate %x", err),
                          ("failed to dmx_descrambler_associate %x", err));

        return FALSE;
    }

    err = tsdmx->tsdmxhal.hal_handle->dmx_descrambler_enable(
              tsdmx->tsdmxhal.hal_handle, *descramble_id);

    if (err != SUCCESS)
    {
        tsdmx->tsdmxhal.hal_handle->dmx_descrambler_close(
            tsdmx->tsdmxhal.hal_handle, *descramble_id);
        *descramble_id = (U32)-1;
        GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                          ("failed to dmx_descrambler_enable %x", err),
                          ("failed to dmx_descrambler_enable %x", err));

        return FALSE;
    }

    GST_INFO_OBJECT(tsdmx, "create_hal_descrambles out, media_type=%d", emedia_type);

    return TRUE;
}

static GstCaps*
gst_smetsdmx_set_video_caps(GstSmeTSDMX* tsdmx)
{
    GstCaps* caps = NULL;
    char *caps_str;

    AV_VID_STREAM_TYPE_E evideo_codec_id = trans_video_codec_to_hal(
            tsdmx->prog_media_info.e_vtype);

    switch (evideo_codec_id)
    {
        case AV_VID_STREAM_TYPE_MPEG2:
        {
            caps = gst_caps_new_empty_simple ("video/mpeg");
            gst_caps_set_simple (caps, "mpegversion", G_TYPE_INT, 2,
                                 "systemstream", G_TYPE_BOOLEAN, FALSE, NULL);
            break;
        }

        case AV_VID_STREAM_TYPE_MPEG4:
        {
            caps = gst_caps_new_empty_simple ("video/mpeg");
            gst_caps_set_simple (caps, "mpegversion", G_TYPE_INT, 4,
                                 "systemstream", G_TYPE_BOOLEAN, FALSE, NULL);
            break;
        }

        case AV_VID_STREAM_TYPE_H264:
        {
            caps = gst_caps_new_empty_simple ("video/x-h264");
            break;
        }

        case AV_VID_STREAM_TYPE_MVC:
        {
            caps = gst_caps_new_empty_simple ("video/tee-mvc");
            break;
        }

        case AV_VID_STREAM_TYPE_HEVC:
        {
            caps = gst_caps_new_empty_simple ("video/x-h265");
            break;
        }

        case AV_VID_STREAM_TYPE_AVSPLUS://no break;
        case AV_VID_STREAM_TYPE_AVS:
        {
            caps = gst_caps_new_empty_simple ("video/avs");
            break;
        }

        case AV_VID_STREAM_TYPE_VC1:
        {
            caps = gst_caps_new_empty_simple ("video/tee-wmv");
            break;
        }

        default:
        {
            GST_ELEMENT_WARNING(tsdmx, STREAM, CODEC_NOT_FOUND,
              ("%s:unsupport vdec error (%d)", GST_SME_STRAM_TAG_VDEC,
                tsdmx->prog_media_info.e_vtype),
              ("%s:unsupport vdec error (%d)", GST_SME_STRAM_TAG_VDEC,
                tsdmx->prog_media_info.e_vtype));
            return NULL;
        }
    }

    caps_str = gst_caps_to_string((const GstCaps *)caps);
    GST_INFO_OBJECT(tsdmx, "video caps:%s", caps_str);
    g_free(caps_str);

    if (caps)
    {
        //es stream for dvb
        gst_caps_set_simple (caps, "is_frame", G_TYPE_BOOLEAN, FALSE, NULL);
    }

    gst_pad_set_caps (tsdmx->video_srcpad, caps);

    return caps;
}

#ifdef AUDIO_USE_DEFAULT_PROP
static GstCaps*
gst_smetsdmx_set_audio_caps(GstSmeTSDMX* tsdmx)
{
    GstCaps* caps = NULL;
    char *caps_str;

    AV_AUD_STREAM_TYPE_E eaudio_codec_id = trans_audio_codec_to_hal(
            tsdmx->prog_media_info.e_atype);

    switch (eaudio_codec_id)
    {
        case AV_AUD_STREAM_TYPE_MP1:
        {
            caps = gst_caps_new_empty_simple ("audio/mpeg");
            gst_caps_set_simple (caps,
                                 "rate", G_TYPE_INT, 48000, "channels", G_TYPE_INT, 2,
                                 "mpegversion", G_TYPE_INT, 1, "layer", G_TYPE_INT, 2, NULL);
            break;
        }

        case AV_AUD_STREAM_TYPE_MP2:
        {
            caps = gst_caps_new_empty_simple ("audio/mpeg");
            gst_caps_set_simple (caps,
                                 "rate", G_TYPE_INT, 48000, "channels", G_TYPE_INT, 2,
                                 "mpegversion", G_TYPE_INT, 2, "layer", G_TYPE_INT, 2, NULL);
            break;
        }

        case AV_AUD_STREAM_TYPE_MP3:
        {
            caps = gst_caps_new_empty_simple ("audio/mpeg");
            gst_caps_set_simple (caps,
                                 "rate", G_TYPE_INT, 48000, "channels", G_TYPE_INT, 2,
                                 "mpegversion", G_TYPE_INT, 3, "layer", G_TYPE_INT, 2, NULL);
            break;
        }

        case AV_AUD_STREAM_TYPE_AC3:
        {
            caps = gst_caps_new_empty_simple ("audio/ac3");
            gst_caps_set_simple (caps, "channels", G_TYPE_INT, 2,
                                 "rate", G_TYPE_INT, 48000, NULL);
            break;
        }

        case AV_AUD_STREAM_TYPE_EAC3:
        {
            caps = gst_caps_new_empty_simple ("audio/eac3");
            gst_caps_set_simple (caps, "channels", G_TYPE_INT, 2,
                                 "rate", G_TYPE_INT, 48000, NULL);
            break;
        }

        case AV_AUD_STREAM_TYPE_DTS:
        {
            caps = gst_caps_new_empty_simple ("audio/dts");
            break;
        }

        case AV_AUD_STREAM_TYPE_DRA:
        {
            caps = gst_caps_new_empty_simple ("audio/dra");
            gst_caps_set_simple (caps, "channels", G_TYPE_INT, 2,
                "rate", G_TYPE_INT, 48000, NULL);
            break;
        }

        case AV_AUD_STREAM_TYPE_AAC:
        {
            caps = gst_caps_new_empty_simple ("audio/mpeg");
            gst_caps_set_simple (caps, "mpegversion", G_TYPE_INT, 4,
                                 "stream-format", G_TYPE_STRING, "raw",
                                 "base-profile", G_TYPE_STRING, "lc",
                                 "rate", G_TYPE_INT, 48000,
                                 "channels", G_TYPE_INT, 2, NULL);
            break;
        }

        default:
        GST_ELEMENT_WARNING(tsdmx, STREAM, CODEC_NOT_FOUND,
          ("%s:unsupport adec error (%d)", GST_SME_STRAM_TAG_ADEC,
            tsdmx->prog_media_info.e_vtype),
          ("%s:unsupport adec error (%d)", GST_SME_STRAM_TAG_ADEC,
            tsdmx->prog_media_info.e_vtype));

            return NULL;
    }

    caps_str = gst_caps_to_string((const GstCaps *)caps);
    GST_INFO_OBJECT(tsdmx, "audio caps:%s", caps_str);
    g_free(caps_str);

    gst_pad_set_caps (tsdmx->audio_srcpad, caps);

    return caps;
}
#else
static GstCaps*
gst_smetsdmx_set_audio_caps(GstSmeTSDMX* tsdmx)
{
    GstCaps* caps = NULL;
    AV_AUD_STREAM_TYPE_E eaudio_codec_id = trans_audio_codec_to_hal(
            tsdmx->prog_media_info.e_atype);

    switch (eaudio_codec_id)
    {
        case AV_AUD_STREAM_TYPE_MP1:
        {
            caps = gst_caps_new_empty_simple ("audio/mpeg");
            gst_caps_set_simple (caps, "mpegversion", G_TYPE_INT, 1, NULL);
            break;
        }

        case AV_AUD_STREAM_TYPE_MP2:
        {
            caps = gst_caps_new_empty_simple ("audio/mpeg");
            gst_caps_set_simple (caps, "mpegversion", G_TYPE_INT, 2, NULL);
            break;
        }

        case AV_AUD_STREAM_TYPE_MP3:
        {
            caps = gst_caps_new_empty_simple ("audio/mpeg");
            gst_caps_set_simple (caps, "mpegversion", G_TYPE_INT, 3, NULL);
            break;
        }

        case AV_AUD_STREAM_TYPE_AC3:
        {
            caps = gst_caps_new_empty_simple ("audio/ac3");
            break;
        }

        case AV_AUD_STREAM_TYPE_EAC3:
        {
            caps = gst_caps_new_empty_simple ("audio/eac3");
            break;
        }

        case AV_AUD_STREAM_TYPE_DTS:
        {
            caps = gst_caps_new_empty_simple ("audio/dts");
            break;
        }

        case AV_AUD_STREAM_TYPE_DRA:
        {
            caps = gst_caps_new_empty_simple ("audio/dra");
            break;
        }

        case AV_AUD_STREAM_TYPE_AAC:
        {
            caps = gst_caps_new_empty_simple ("audio/aac");
            gst_caps_set_simple (caps, "mpegversion", G_TYPE_INT, 4, NULL);
            break;
        }

        default:
          GST_ELEMENT_WARNING(tsdmx, STREAM, CODEC_NOT_FOUND,
            ("%s:unsupport adec error (%d)", GST_SME_STRAM_TAG_ADEC,
              tsdmx->prog_media_info.e_vtype),
            ("%s:unsupport adec error (%d)", GST_SME_STRAM_TAG_ADEC,
              tsdmx->prog_media_info.e_vtype));

            return NULL;
    }

    GST_INFO_OBJECT(tsdmx, "audio caps:%s", gst_caps_to_string(caps));

    gst_pad_set_caps (tsdmx->audio_srcpad, caps);

    return caps;
}
#endif

static void
gst_smetsdmx_emit_vid_srcpad_info(GstSmeTSDMX* tsdmx)
{
    GstPadTemplate* srcpad_template = NULL;
    gchar* name = NULL;
    gchar* stream_id = NULL;;
    gboolean exist_pad = tsdmx->video_srcpad ? TRUE : FALSE;
    GstCaps* caps = NULL;
    gboolean audio_done;

    GST_INFO("exist_pad:%d,tsdmx->prog_media_info.e_vtype:%d,tsdmx->video_srcpad:%p",
        exist_pad,tsdmx->prog_media_info.e_vtype, tsdmx->video_srcpad);

    if (!exist_pad && tsdmx->prog_media_info.e_vtype != E_DVB_VID_CODEC_TYPE_UNKNOWN)
    {
        name = g_strdup_printf ("video_%04x", tsdmx->prog_media_info.vpid);
        srcpad_template = gst_static_pad_template_get (&g_video_src_caps_tmpl);
        if (name)
            tsdmx->video_srcpad = gst_pad_new_from_template (srcpad_template, name);
        else
            tsdmx->video_srcpad = gst_pad_new_from_template (srcpad_template, "unknown");
        GST_PAD_ELEMENT_PRIVATE (tsdmx->video_srcpad) = tsdmx;
        gst_pad_use_fixed_caps (tsdmx->video_srcpad);

        stream_id = gst_pad_create_stream_id_printf (tsdmx->video_srcpad,
                    GST_ELEMENT_CAST (tsdmx), "%08x", tsdmx->prog_media_info.vpid);

        (void)gst_pad_set_active(tsdmx->video_srcpad, (gboolean)TRUE);
    }

    GST_INFO("exist_pad:%d,tsdmx->prog_media_info.e_vtype:%d,tsdmx->video_srcpad:%p",
      exist_pad,tsdmx->prog_media_info.e_vtype, tsdmx->video_srcpad);
    if (tsdmx->applied_prog_media_info.e_vtype != tsdmx->prog_media_info.e_vtype
        || E_DVB_VID_CODEC_TYPE_UNKNOWN == tsdmx->prog_media_info.e_vtype)
    {
        caps = gst_smetsdmx_set_video_caps (tsdmx);
    }

    GST_INFO("exist_pad:%d,tsdmx->prog_media_info.e_vtype:%d,tsdmx->video_srcpad:%p",
        exist_pad,tsdmx->prog_media_info.e_vtype, tsdmx->video_srcpad);

    if (!exist_pad && tsdmx->video_srcpad)
    {
        GST_WARNING_OBJECT(tsdmx, "%s:pad_add video begin...", SMEPLAYER_STAT_TAG_TIME);
        if (!gst_element_add_pad (GST_ELEMENT (tsdmx), tsdmx->video_srcpad))
        {
            GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                              ("add video pad failed error (%d)", tsdmx->prog_media_info.e_vtype),
                              ("add video pad failed error (%d)", tsdmx->prog_media_info.e_vtype));
        }
        else
        {
            GST_WARNING_OBJECT(tsdmx, "%s:pad_add video end", SMEPLAYER_STAT_TAG_TIME);
        }
    }
    else if (tsdmx->applied_prog_media_info.e_vtype != tsdmx->prog_media_info.e_vtype
        && tsdmx->video_srcpad)
    {
        GST_WARNING_OBJECT(tsdmx, "%s:pad_caps_change video begin...", SMEPLAYER_STAT_TAG_TIME);
        g_signal_emit (tsdmx, g_smetsdmx_signals[PAD_CAPS_CHANGED], 0,
                       tsdmx->video_srcpad, caps);
        GST_WARNING_OBJECT(tsdmx, "%s:pad_caps_change video end", SMEPLAYER_STAT_TAG_TIME);
    }

    if (name)
    {
        g_free (name);
    }

    if (srcpad_template)
    {
        gst_object_unref (srcpad_template);
    }

    if (stream_id)
    {
        g_free (stream_id);
    }

    if (caps)
    {
        gst_caps_unref(caps);
    }

    g_mutex_lock(&tsdmx->av_proc_mutex);
    tsdmx->applied_prog_media_info.e_vtype = tsdmx->prog_media_info.e_vtype;
    tsdmx->applied_prog_media_info.vpid = tsdmx->prog_media_info.vpid;
    audio_done = (tsdmx->applied_prog_media_info.e_atype == tsdmx->prog_media_info.e_atype);
    g_mutex_unlock(&tsdmx->av_proc_mutex);

    if (audio_done)
    {
        gst_element_no_more_pads(GST_ELEMENT(tsdmx));
        GST_WARNING_OBJECT(tsdmx, "%s:no_more_pad v-thread end", SMEPLAYER_STAT_TAG_TIME);
    }

    return;
}

static gboolean
gst_smetsdmx_do_video_init_hal(GstSmeTSDMX* tsdmx)
{
    g_mutex_lock(&tsdmx->video_proc_mutex);

    if(E_TSDMX_THD_STATE_IDLE != tsdmx->evideo_proc_thread_state)
    {
        //1.emit srcpad info:pad_add or pad_caps_changed
        gst_smetsdmx_emit_vid_srcpad_info(tsdmx);
    }
    else
    {
        g_mutex_unlock(&tsdmx->video_proc_mutex);
        return TRUE;
    }
    g_mutex_unlock(&tsdmx->video_proc_mutex);

    //2.start video hal-filter
    if (tsdmx->prog_media_info.e_vtype != E_DVB_VID_CODEC_TYPE_UNKNOWN)
    {
        GST_WARNING_OBJECT(tsdmx, "%s:start tsdmx-hal-filter video begin...", SMEPLAYER_STAT_TAG_TIME);
        (void)gst_smetsdmx_start_hal_filters(tsdmx, E_DVB_MEDIA_TYPE_VIDEO);
        GST_WARNING_OBJECT(tsdmx, "%s:start tsdmx-hal-filter video end", SMEPLAYER_STAT_TAG_TIME);

        return TRUE;
    }

    return FALSE;
}

//if return false, we should sleep 3ms.
static gboolean
gst_smetsdmx_do_video_update_ca_key(GstSmeTSDMX* tsdmx)
{
    gboolean bret;

    //1. create vid-hal-descramble
    bret = gst_smetsdmx_create_hal_descrambles(tsdmx, E_DVB_MEDIA_TYPE_VIDEO);

    if (!bret)
    {
        return FALSE;
    }

    //2. update video-ca-key
    update_video_ca_key_info(tsdmx);

    return TRUE;
}

//lint -e571
//lint -e838
static guint64 gst_smetsdmx_get_cur_systime()
{
    GTimeVal timeval;
    guint64 timetick = 0;
    g_get_current_time (&timeval);
    timetick = (guint64)timeval.tv_sec * 1000 +
        ((guint64)timeval.tv_usec + 500) / 1000;
    return timetick;
}
//lint +e571
//lint +e838

//if return false, we should sleep 3ms.
static gboolean
gst_smetsdmx_do_video_process_av(GstSmeTSDMX* tsdmx)
{
    S32 err;
    gboolean bres = TRUE;
    GstFlowReturn push_res = GST_FLOW_OK;
    GstBuffer* outbuf = NULL;
    DMX_ESFRAME_INFO_S hal_frame_info;
    guint buf_size = 0;

    (void)VOS_Memset_S(&hal_frame_info, sizeof(hal_frame_info),
                       0, sizeof(hal_frame_info));

    err = tsdmx->tsdmxhal.hal_handle->dmx_avfilter_get_esframe(
              tsdmx->tsdmxhal.hal_handle, tsdmx->tsdmxhal.video_filter_id,
              &hal_frame_info);

    if (SUCCESS != err)
    {
        if(tsdmx->v_last_no_str_t == 0)
        {
            tsdmx->v_last_no_str_t = gst_smetsdmx_get_cur_systime();
        }

        tsdmx->v_cur_no_str_t = gst_smetsdmx_get_cur_systime();
        if(tsdmx->v_cur_no_str_t - tsdmx->v_last_no_str_t >= TSDMX_HAVE_NO_STREAM_TIME)
        {
            g_mutex_lock(&tsdmx->av_proc_mutex);
            tsdmx->have_no_video = TRUE;
            g_mutex_unlock(&tsdmx->av_proc_mutex);
        }

        g_mutex_lock(&tsdmx->av_proc_mutex);
        if(tsdmx->have_no_audio && tsdmx->have_no_video && (!tsdmx->post_no_streamer))
        {
            GST_WARNING("post no stream by video");
            GstMessage *msg_have_no_stream = gst_message_have_no_stream ((GstObject*)tsdmx);
            if(msg_have_no_stream)
            {
                gst_element_post_message ((GstElement *)tsdmx, msg_have_no_stream);
            }
            tsdmx->post_no_streamer = TRUE;
        }
        g_mutex_unlock(&tsdmx->av_proc_mutex);

        GST_DEBUG_OBJECT(tsdmx, "video get_esframe failed(%#x)", err);
        return FALSE;
    }
    /*code to here means streamer come, so last_no_str_t should be reset*/
    tsdmx->v_last_no_str_t = 0;
    g_mutex_lock(&tsdmx->av_proc_mutex);
    tsdmx->have_no_video = FALSE;
    if(tsdmx->post_no_streamer)
    {
        GST_WARNING("post stream recover by video");
        GstMessage *msg_stream_recover = gst_message_stream_recover ((GstObject*)tsdmx);
        if(msg_stream_recover)
        {
            gst_element_post_message ((GstElement *)tsdmx, msg_stream_recover);
        }

        tsdmx->post_no_streamer = FALSE;
    }
    g_mutex_unlock(&tsdmx->av_proc_mutex);


    if (GST_CLOCK_TIME_NONE == tsdmx->v_first_pts)
    {
        if (hal_frame_info.u64Timestamp != TSDMXHAL_INVALID_PTS)
        {
            tsdmx->v_first_pts = hal_frame_info.u64Timestamp * GST_MSECOND;
        }
        else
        {
            GST_INFO_OBJECT(tsdmx, "first invalid video pts esframe, discard !");
            dump_v(tsdmx,
                   (const guchar*) (glong)(hal_frame_info.u64BufferAddr & 0xFFFFFFFF),
                   hal_frame_info.u32Lenght);
            goto relase_esframe;
        }
    }

    if (tsdmx->bvideo_need_new_segment)
    {
        GstEvent* evt_segment;

        //here segment will should use first video pts.
        //and if audio pts < video pts we should discard audio frame.
        gst_segment_init(&tsdmx->video_segment, GST_FORMAT_TIME);
        tsdmx->video_segment.start = (GstClockTime)tsdmx->v_first_pts;
        tsdmx->video_segment.position = (GstClockTime)tsdmx->v_first_pts;
        tsdmx->video_segment.time = (GstClockTime)0;
        tsdmx->video_segment.stop = (GstClockTime)GST_CLOCK_TIME_NONE;
        tsdmx->video_segment.format = GST_FORMAT_TIME;
        evt_segment = gst_event_new_segment (&tsdmx->video_segment);

        if (evt_segment)
        {
            GST_WARNING_OBJECT(tsdmx, "%s:send video segment begin...,start=%lld",
                            SMEPLAYER_STAT_TAG_TIME, tsdmx->video_segment.start);
            bres = gst_pad_push_event (tsdmx->video_srcpad, evt_segment);
            GST_WARNING_OBJECT(tsdmx, "%s:send video segment end,bres=%d",
                            SMEPLAYER_STAT_TAG_TIME, bres);

            if (!bres)
            {
                GST_ERROR_OBJECT(tsdmx, "gst_pad_push video segment failed !");
                goto relase_esframe;
            }
            else
            {
                tsdmx->bvideo_need_new_segment = FALSE;
            }
        }
        else
        {
            GST_ERROR_OBJECT(tsdmx, "new video segment failed, why ?");
        }
    }

    buf_size = ((hal_frame_info.u32Lenght + 0xFFF) & 0xFFFFF000);//align by 4096
    outbuf = gst_buffer_new_and_alloc(buf_size);

    if (NULL == outbuf)
    {
        GST_ERROR_OBJECT(tsdmx, "gst_buffer alloc(%u) failed why ?", buf_size);
        bres = FALSE;
        goto relase_esframe;
    }

    buf_size = gst_buffer_fill(outbuf, 0,
                               (gpointer)(glong)(hal_frame_info.u64BufferAddr & 0xFFFFFFFF),
                               hal_frame_info.u32Lenght);

    if (buf_size != hal_frame_info.u32Lenght)
    {
        GST_WARNING_OBJECT(tsdmx, "fill gstbuffer failed !");
    }

    gst_buffer_resize(outbuf, 0, (glong)buf_size);
    dump_v(tsdmx, (const guchar*) (glong)(hal_frame_info.u64BufferAddr & 0xFFFFFFFF),
           hal_frame_info.u32Lenght);

    if (hal_frame_info.u64Timestamp != TSDMXHAL_INVALID_PTS)
    {
        GST_BUFFER_TIMESTAMP(outbuf) = hal_frame_info.u64Timestamp * GST_MSECOND;
    }
    else
    {
        GST_BUFFER_TIMESTAMP(outbuf) = GST_CLOCK_TIME_NONE;
    }

    GST_BUFFER_DURATION(outbuf) = GST_CLOCK_TIME_NONE;

    GST_INFO_OBJECT(tsdmx, "push video begin...,pts=%lld,len=%d",
                    GST_BUFFER_TIMESTAMP(outbuf), hal_frame_info.u32Lenght);

    GST_PAD_STREAM_LOCK(tsdmx->video_srcpad);
    if (! tsdmx->bvideo_need_new_segment &&
        E_TSDMX_THD_STATE_PROCCESS_AV == tsdmx->evideo_proc_thread_state)
    {
        push_res = gst_pad_push (tsdmx->video_srcpad, outbuf);
    }
    else
    {
        push_res = GST_FLOW_FLUSHING;
        GST_WARNING_OBJECT(tsdmx, "Must send segment first!");
    }

    GST_PAD_STREAM_UNLOCK(tsdmx->video_srcpad);
    GST_INFO_OBJECT(tsdmx, "push video end,push_res=%d", push_res);

    if (GST_FLOW_OK != push_res)
    {
        GST_WARNING_OBJECT(tsdmx, "gst_pad_push video failed(%d)", push_res);
        if(GST_FLOW_NOT_LINKED == push_res){
          GST_ELEMENT_WARNING(tsdmx, STREAM, CODEC_NOT_FOUND,
            ("%s:unsupport vdec error (%d)", GST_SME_STRAM_TAG_VDEC,
              tsdmx->prog_media_info.e_vtype),
            ("%s:unsupport vdec error (%d)", GST_SME_STRAM_TAG_VDEC,
              tsdmx->prog_media_info.e_vtype));
          g_mutex_lock(&tsdmx->video_proc_mutex);
          tsdmx->evideo_proc_thread_state = E_TSDMX_THD_STATE_IDLE;
          g_mutex_unlock(&tsdmx->video_proc_mutex);
        }
    }

relase_esframe:
    err = tsdmx->tsdmxhal.hal_handle->dmx_avfilter_release_esframe(
              tsdmx->tsdmxhal.hal_handle, tsdmx->tsdmxhal.video_filter_id,
              &hal_frame_info);

    if (SUCCESS != err)
    {
        GST_DEBUG_OBJECT(tsdmx, "release_esframe video failed(%#x) why ?", err);
    }

    return bres;
}

//by lrl:todo check tsdmx->video_proc_mutex is the best ?
static void*
proc_video_stream_thread_func(void* arg)
{
    GstSmeTSDMX* tsdmx = GST_SMETSDMX (arg);
    gboolean berr = TRUE;

    GST_INFO_OBJECT(tsdmx, "proc_video_stream_thread_func in");

    while (TRUE)
    {
        berr = TRUE;

        switch (tsdmx->evideo_proc_thread_state)
        {
            case E_TSDMX_THD_STATE_TO_IDLE_THEN_INIT://no break;
            case E_TSDMX_THD_STATE_IDLE:
            {
                close_dump_v(tsdmx);
                tsdmx->v_first_pts = GST_CLOCK_TIME_NONE;
                tsdmx->bvideo_need_new_segment = TRUE;

                //1.destroy video hal-descrambles here?
                if ((U32)-1 != tsdmx->prog_ca_key_info.video_descramble_id)
                {
                    GST_WARNING_OBJECT(tsdmx, "%s:destroy tsdmx-hal-descramble video begin...", SMEPLAYER_STAT_TAG_TIME);
                    gst_smetsdmx_destory_hal_descrambles(tsdmx,
                                                         tsdmx->prog_ca_key_info.video_descramble_id);
                    tsdmx->prog_ca_key_info.video_descramble_id = (U32)-1;
                    GST_WARNING_OBJECT(tsdmx, "%s:destroy descramble video end", SMEPLAYER_STAT_TAG_TIME);
                }

                //2.stop video hal-filter
                GST_WARNING_OBJECT(tsdmx, "%s:stop tsdmx-hal-filter video begin...", SMEPLAYER_STAT_TAG_TIME);
                (void)gst_smetsdmx_stop_hal_filters(tsdmx, E_DVB_MEDIA_TYPE_VIDEO);
                GST_WARNING_OBJECT(tsdmx, "%s:stop tsdmx-hal-filter video end", SMEPLAYER_STAT_TAG_TIME);
                g_mutex_lock(&tsdmx->video_proc_mutex);

                if (E_TSDMX_THD_STATE_IDLE == tsdmx->evideo_proc_thread_state)
                {
                    g_cond_wait_until(&tsdmx->video_proc_cond, &tsdmx->video_proc_mutex,
                                      g_get_monotonic_time () + 1000 * G_TIME_SPAN_MILLISECOND);
                }

                if (E_TSDMX_THD_STATE_TO_IDLE_THEN_INIT == tsdmx->evideo_proc_thread_state)
                {
                    tsdmx->evideo_proc_thread_state = E_TSDMX_THD_STATE_INIT_HAL;
                }
                g_mutex_unlock(&tsdmx->video_proc_mutex);
                break;
            }

            case E_TSDMX_THD_STATE_INIT_HAL:
            {
                berr = gst_smetsdmx_do_video_init_hal(tsdmx);
                g_mutex_lock(&tsdmx->video_proc_mutex);

                if (!berr)
                {
                    tsdmx->evideo_proc_thread_state = E_TSDMX_THD_STATE_IDLE;
                }
                else
                {
                    if (E_TSDMX_THD_STATE_INIT_HAL == tsdmx->evideo_proc_thread_state)
                    {
                        if(tsdmx->applied_prog_media_info.vpid != tsdmx->prog_media_info.vpid){
                          //we should do init hal again for CC crazily case , it will play last channel.
                          g_mutex_unlock(&tsdmx->video_proc_mutex);
                          GST_WARNING_OBJECT(tsdmx, "%s:simultaneous accur. stop tsdmx-hal-filter video begin...", SMEPLAYER_STAT_TAG_TIME);
                          (void)gst_smetsdmx_stop_hal_filters(tsdmx, E_DVB_MEDIA_TYPE_VIDEO);
                          GST_WARNING_OBJECT(tsdmx, "%s:simultaneous accur. stop tsdmx-hal-filter video end", SMEPLAYER_STAT_TAG_TIME);
                          break;
                        }

                        if (tsdmx->prog_media_info.video_is_ca)
                        {
                            tsdmx->evideo_proc_thread_state = E_TSDMX_THD_STATE_UPDATE_CA_KEY;
                        }
                        else
                        {
                            tsdmx->evideo_proc_thread_state = E_TSDMX_THD_STATE_PROCCESS_AV;
                            open_dump_v(tsdmx);
                        }
                    }
                }

                g_mutex_unlock(&tsdmx->video_proc_mutex);
                break;
            }

            case E_TSDMX_THD_STATE_UPDATE_CA_KEY:
            {
                g_mutex_lock(&tsdmx->video_proc_mutex);

                if (E_TSDMX_THD_STATE_UPDATE_CA_KEY == tsdmx->evideo_proc_thread_state)
                {
                    berr = gst_smetsdmx_do_video_update_ca_key(tsdmx);

                    if (berr)
                    {
                        tsdmx->evideo_proc_thread_state = E_TSDMX_THD_STATE_PROCCESS_AV;
                        open_dump_v(tsdmx);
                    }
                }

                g_mutex_unlock(&tsdmx->video_proc_mutex);
                break;
            }

            case E_TSDMX_THD_STATE_PROCCESS_AV:
            {
                berr = gst_smetsdmx_do_video_process_av(tsdmx);
                break;
            }

            default:
                break;
        }

        if (tsdmx->bexit)
        {
            break;
        }

        if (!berr)
        {
            g_usleep(5 * 1000);
        }
    }

    close_dump_v(tsdmx);

    //1.destroy video hal-descrambles here?
    if ((U32)-1 != tsdmx->prog_ca_key_info.video_descramble_id)
    {
        gst_smetsdmx_destory_hal_descrambles(tsdmx,
                                             tsdmx->prog_ca_key_info.video_descramble_id);
        tsdmx->prog_ca_key_info.video_descramble_id = (U32)-1;
    }

    //2.stop video hal-filter
    (void)gst_smetsdmx_stop_hal_filters(tsdmx, E_DVB_MEDIA_TYPE_VIDEO);

    GST_INFO_OBJECT(tsdmx, "proc_video_stream_thread_func out");

    return NULL;
}

static void
gst_smetsdmx_emit_aud_srcpad_info(GstSmeTSDMX* tsdmx)
{
    GstPadTemplate* srcpad_template = NULL;
    gchar* name = NULL;
    gchar* stream_id = NULL;
    gboolean exist_pad = tsdmx->audio_srcpad ? TRUE : FALSE;
    GstCaps* caps = NULL;
    gboolean video_done = FALSE;

    if (!exist_pad && tsdmx->prog_media_info.e_atype != E_DVB_AUD_CODEC_TYPE_UNKNOWN)
    {
        name = g_strdup_printf ("audio_%04x", tsdmx->prog_media_info.apid);
        srcpad_template = gst_static_pad_template_get (&g_audio_src_caps_tmpl);
        if (name)
            tsdmx->audio_srcpad = gst_pad_new_from_template (srcpad_template, name);
        else
            tsdmx->audio_srcpad = gst_pad_new_from_template (srcpad_template, "unknown");
        GST_PAD_ELEMENT_PRIVATE (tsdmx->audio_srcpad) = tsdmx;
        gst_pad_use_fixed_caps (tsdmx->audio_srcpad);

        stream_id = gst_pad_create_stream_id_printf (tsdmx->audio_srcpad,
                    GST_ELEMENT_CAST (tsdmx), "%08x", tsdmx->prog_media_info.apid);

        gst_pad_set_active(tsdmx->audio_srcpad, (gboolean)TRUE);
    }


    if (tsdmx->applied_prog_media_info.e_atype != tsdmx->prog_media_info.e_atype
        /* BEGIN: Added for DTS2016062400087 , 2016/6/28 */
        && tsdmx->prog_media_info.e_atype != E_DVB_AUD_CODEC_TYPE_UNKNOWN )
        /* END: Added for DTS2016062400087 , 2016/6/28 */
    {
        caps = gst_smetsdmx_set_audio_caps (tsdmx);
    }

    if (!exist_pad && tsdmx->audio_srcpad)
    {
        GST_WARNING_OBJECT(tsdmx, "%s:pad_add audio begin...", SMEPLAYER_STAT_TAG_TIME);
        if (!gst_element_add_pad (GST_ELEMENT (tsdmx), tsdmx->audio_srcpad))
        {
            GST_ELEMENT_ERROR(tsdmx, STREAM, DEMUX,
                              ("add audio pad failed error (%d)", tsdmx->prog_media_info.e_atype),
                              ("add audio pad failed error (%d)", tsdmx->prog_media_info.e_atype));
            goto exit ;
        }
        GST_WARNING_OBJECT(tsdmx, "%s:pad_add audio end", SMEPLAYER_STAT_TAG_TIME);
    }
    else if (tsdmx->applied_prog_media_info.e_atype != tsdmx->prog_media_info.e_atype
        && tsdmx->audio_srcpad)
    {
        GST_WARNING_OBJECT(tsdmx, "%s:pad_caps_change audio begin...", SMEPLAYER_STAT_TAG_TIME);
        g_signal_emit (tsdmx, g_smetsdmx_signals[PAD_CAPS_CHANGED], 0,
                       tsdmx->audio_srcpad, caps);
        GST_WARNING_OBJECT(tsdmx, "%s:pad_caps_change audio end", SMEPLAYER_STAT_TAG_TIME);
    }

exit:
    if (name)
    {
        g_free (name);
    }

    if (srcpad_template)
    {
        gst_object_unref (srcpad_template);
    }

    if (stream_id)
    {
        g_free (stream_id);
    }
    if (caps)
    {
        gst_caps_unref(caps);
    }

    g_mutex_lock(&tsdmx->av_proc_mutex);
    tsdmx->applied_prog_media_info.e_atype = tsdmx->prog_media_info.e_atype;
    tsdmx->applied_prog_media_info.apid = tsdmx->prog_media_info.apid;
    video_done = (tsdmx->applied_prog_media_info.e_vtype == tsdmx->prog_media_info.e_vtype);
    g_mutex_unlock(&tsdmx->av_proc_mutex);

    if (video_done)
    {
        //may be delock with setprogaudioinfo
        g_mutex_unlock(&tsdmx->audio_proc_mutex);
        gst_element_no_more_pads(GST_ELEMENT(tsdmx));
        g_mutex_lock(&tsdmx->audio_proc_mutex);
        GST_WARNING_OBJECT(tsdmx, "%s:no_more_pad a-thread end", SMEPLAYER_STAT_TAG_TIME);
    }

    return;
}

static gboolean
gst_smetsdmx_do_audio_init_hal(GstSmeTSDMX* tsdmx)
{
    g_mutex_lock(&tsdmx->audio_proc_mutex);
    if(tsdmx->eaudio_proc_thread_state != E_TSDMX_THD_STATE_IDLE)
    {
        //1.emit srcpad info:pad_add or pad_caps_changed
        gst_smetsdmx_emit_aud_srcpad_info(tsdmx);
    }
    else
    {
        g_mutex_unlock(&tsdmx->audio_proc_mutex);
        return TRUE;
    }
    g_mutex_unlock(&tsdmx->audio_proc_mutex);

    //2.start audio hal-filter
    if (tsdmx->prog_media_info.e_atype != E_DVB_AUD_CODEC_TYPE_UNKNOWN)
    {
        GST_WARNING_OBJECT(tsdmx, "%s:start tsdmx-hal-filter audio begin...", SMEPLAYER_STAT_TAG_TIME);
        (void)gst_smetsdmx_start_hal_filters(tsdmx, E_DVB_MEDIA_TYPE_AUDIO);
        GST_WARNING_OBJECT(tsdmx, "%s:start tsdmx-hal-filter audio end", SMEPLAYER_STAT_TAG_TIME);

        return TRUE;
    }

    return FALSE;
}

static gboolean
gst_smetsdmx_do_audio_update_ca_key(GstSmeTSDMX* tsdmx)
{
    gboolean bret;

    //1. create aud-hal-descramble
    bret = gst_smetsdmx_create_hal_descrambles(tsdmx, E_DVB_MEDIA_TYPE_AUDIO);

    if (!bret)
    {
        return FALSE;
    }

    //2. update audios-ca-key
    update_audio_ca_key_info(tsdmx);

    return TRUE;
}

//if return false, we should sleep 3ms.
static gboolean
gst_smetsdmx_do_audio_process_av(GstSmeTSDMX* tsdmx)
{
    S32 err;
    gboolean bres = TRUE;
    GstFlowReturn push_res = GST_FLOW_OK;
    GstBuffer* outbuf = NULL;
    DMX_ESFRAME_INFO_S hal_frame_info;
    guint buf_size = 0;

    (void)VOS_Memset_S(&hal_frame_info, sizeof(hal_frame_info),
                       0, sizeof(hal_frame_info));

    err = tsdmx->tsdmxhal.hal_handle->dmx_avfilter_get_esframe(
              tsdmx->tsdmxhal.hal_handle, tsdmx->tsdmxhal.audio_filter_id,
              &hal_frame_info);

    if (SUCCESS != err)
    {
        if(tsdmx->a_last_no_str_t == 0)
        {
            tsdmx->a_last_no_str_t = gst_smetsdmx_get_cur_systime();
        }

        tsdmx->a_cur_no_str_t = gst_smetsdmx_get_cur_systime();
        if(tsdmx->a_cur_no_str_t - tsdmx->a_last_no_str_t >= TSDMX_HAVE_NO_STREAM_TIME)
        {
            g_mutex_lock(&tsdmx->av_proc_mutex);
            tsdmx->have_no_audio = TRUE;
            g_mutex_unlock(&tsdmx->av_proc_mutex);
        }

        g_mutex_lock(&tsdmx->av_proc_mutex);
        if(tsdmx->have_no_audio && tsdmx->have_no_video && (!tsdmx->post_no_streamer))
        {
            GST_WARNING("post no streamer by audio");
            GstMessage *msg_have_no_stream = gst_message_have_no_stream ((GstObject*)tsdmx);
            if(msg_have_no_stream)
            {
                gst_element_post_message ((GstElement *)tsdmx, msg_have_no_stream);
            }
            tsdmx->post_no_streamer = TRUE;
        }
        g_mutex_unlock(&tsdmx->av_proc_mutex);

        GST_DEBUG_OBJECT(tsdmx, "audio get_esframe failed(%#x)", err);
        return FALSE;
    }

    /*code to here means streamer come, so last_no_str_t should be reset*/
    tsdmx->a_last_no_str_t = 0;
    g_mutex_lock(&tsdmx->av_proc_mutex);
    tsdmx->have_no_audio = FALSE;
    if(tsdmx->post_no_streamer)
    {
        GST_WARNING("post stream recover by audio");
        GstMessage *msg_stream_recover = gst_message_stream_recover ((GstObject*)tsdmx);
        if(msg_stream_recover)
        {
            gst_element_post_message ((GstElement *)tsdmx, msg_stream_recover);
        }

        tsdmx->post_no_streamer = FALSE;
    }
    g_mutex_unlock(&tsdmx->av_proc_mutex);

    if (GST_CLOCK_TIME_NONE == tsdmx->a_first_pts
      || tsdmx->audio_need_wait_validpts)
    {
        if (hal_frame_info.u64Timestamp != TSDMXHAL_INVALID_PTS /*
      && (E_DVB_VID_CODEC_TYPE_UNKNOWN == tsdmx->prog_media_info.e_vtype
        || (tsdmx->v_first_pts != GST_CLOCK_TIME_NONE
          && (hal_frame_info.u64Timestamp * GST_MSECOND) >= tsdmx->v_first_pts))*/)
        {
            if(GST_CLOCK_TIME_NONE == tsdmx->a_first_pts)
            {
                tsdmx->a_first_pts = hal_frame_info.u64Timestamp * GST_MSECOND;
            }
            tsdmx->audio_need_wait_validpts = FALSE;
            GST_INFO_OBJECT(tsdmx, "set first pts(%lld) audio esframe !",
                            tsdmx->a_first_pts);
        }
        else
        {
            GST_INFO_OBJECT(tsdmx, "first invalid pts(%lld) audio esframe, discard !",
                            hal_frame_info.u64Timestamp);
            dump_a(tsdmx,
                   (const guchar*) (glong)(hal_frame_info.u64BufferAddr & 0xFFFFFFFF),
                   hal_frame_info.u32Lenght);
            goto relase_esframe;
        }
    }

    if (tsdmx->baudio_need_new_segment)
    {
        GstEvent* evt_segment;

        //here segment will should use first video pts.
        //and if audio pts < video pts we should discard audio frame.
        gst_segment_init(&tsdmx->audio_segment, GST_FORMAT_TIME);
        /*if(tsdmx->v_first_pts != GST_CLOCK_TIME_NONE){
          tsdmx->audio_segment.start = (GstClockTime)tsdmx->v_first_pts;//use video
        } else*/
        {
            tsdmx->audio_segment.start = (GstClockTime)tsdmx->a_first_pts;//use audio
        }
        tsdmx->audio_segment.position = (GstClockTime)tsdmx->a_first_pts;
        tsdmx->audio_segment.time = (GstClockTime)0;
        tsdmx->audio_segment.stop = (GstClockTime)GST_CLOCK_TIME_NONE;
        tsdmx->audio_segment.format = GST_FORMAT_TIME;
        evt_segment = gst_event_new_segment (&tsdmx->audio_segment);

        if (evt_segment)
        {
            GST_WARNING_OBJECT(tsdmx, "%s:send audio segment begin...,start=%lld,1stpts=%lld,time=%lld",
                            SMEPLAYER_STAT_TAG_TIME,
                            tsdmx->audio_segment.start, tsdmx->a_first_pts,
                            tsdmx->audio_segment.time);
            bres = gst_pad_push_event (tsdmx->audio_srcpad, evt_segment);
            GST_WARNING_OBJECT(tsdmx, "%s:send audio segment end,bres=%d",
                            SMEPLAYER_STAT_TAG_TIME, bres);

            if (!bres)
            {
                GST_ERROR_OBJECT(tsdmx, "gst_pad_push audio segment failed !");
                goto relase_esframe;
            }
            else
            {
                tsdmx->baudio_need_new_segment = FALSE;
            }
        }
        else
        {
            GST_ERROR_OBJECT(tsdmx, "new audio segment failed, why ?");
        }
    }

    buf_size = ((hal_frame_info.u32Lenght + 0xFFF) & 0xFFFFF000);//align by 4096
    outbuf = gst_buffer_new_and_alloc(buf_size);

    if (NULL == outbuf)
    {
        GST_ERROR_OBJECT(tsdmx, "gst_buffer alloc(%u) failed why ?", buf_size);
        bres = FALSE;
        goto relase_esframe;
    }

    buf_size = gst_buffer_fill(outbuf, 0,
                               (gpointer)(glong)(hal_frame_info.u64BufferAddr & 0xFFFFFFFF),
                               hal_frame_info.u32Lenght);

    if (buf_size != hal_frame_info.u32Lenght)
    {
        GST_WARNING_OBJECT(tsdmx, "fill gstbuffer failed !");
    }

    gst_buffer_resize(outbuf, 0, (glong)buf_size);
    dump_a(tsdmx, (const guchar*) (glong)(hal_frame_info.u64BufferAddr & 0xFFFFFFFF),
           hal_frame_info.u32Lenght);

    if ( hal_frame_info.u64Timestamp != TSDMXHAL_INVALID_PTS)
    {
        GST_BUFFER_TIMESTAMP(outbuf) = hal_frame_info.u64Timestamp * GST_MSECOND;
    }
    else
    {
        GST_BUFFER_TIMESTAMP(outbuf) = GST_CLOCK_TIME_NONE;
    }

    GST_BUFFER_DURATION(outbuf) = GST_CLOCK_TIME_NONE;
    GST_INFO_OBJECT(tsdmx, "push audio begin...,pts=%lld,len=%d",
                    GST_BUFFER_TIMESTAMP(outbuf), hal_frame_info.u32Lenght);

    GST_PAD_STREAM_LOCK(tsdmx->audio_srcpad);

    if (!tsdmx->baudio_need_new_segment &&
        E_TSDMX_THD_STATE_PROCCESS_AV == tsdmx->eaudio_proc_thread_state)
    {
        push_res = gst_pad_push (tsdmx->audio_srcpad, outbuf);
    }
    else
    {
        push_res = GST_FLOW_FLUSHING;
        GST_WARNING_OBJECT(tsdmx, "Must send segment first!");
    }
    GST_PAD_STREAM_UNLOCK(tsdmx->audio_srcpad);
    GST_INFO_OBJECT(tsdmx, "push audio end, push_res=%d", push_res);

    if (GST_FLOW_OK != push_res)
    {
        GST_WARNING_OBJECT(tsdmx, "gst_pad_push audio failed(%d)", push_res);
        if(GST_FLOW_NOT_LINKED == push_res){
          GST_ELEMENT_WARNING(tsdmx, STREAM, CODEC_NOT_FOUND,
            ("%s:unsupport adec error (%d)", GST_SME_STRAM_TAG_ADEC,
              tsdmx->prog_media_info.e_vtype),
            ("%s:unsupport adec error (%d)", GST_SME_STRAM_TAG_ADEC,
              tsdmx->prog_media_info.e_vtype));
          g_mutex_lock(&tsdmx->audio_proc_mutex);
          tsdmx->eaudio_proc_thread_state = E_TSDMX_THD_STATE_IDLE;
          g_mutex_unlock(&tsdmx->audio_proc_mutex);
        }
    }

relase_esframe:
    err = tsdmx->tsdmxhal.hal_handle->dmx_avfilter_release_esframe(
              tsdmx->tsdmxhal.hal_handle, tsdmx->tsdmxhal.audio_filter_id,
              &hal_frame_info);

    if (SUCCESS != err)
    {
        GST_DEBUG_OBJECT(tsdmx, "release_esframe audio failed(%#x) why ?", err);
    }

    return bres;
}

static void*
proc_audio_stream_thread_func(void* arg)
{
    GstSmeTSDMX* tsdmx = GST_SMETSDMX (arg);
    gboolean berr = TRUE;

    GST_INFO_OBJECT(tsdmx, "proc_audio_stream_thread_func in");

    while (TRUE)
    {
        berr = TRUE;

        switch (tsdmx->eaudio_proc_thread_state)
        {
            case E_TSDMX_THD_STATE_TO_IDLE_THEN_INIT://no break;
            case E_TSDMX_THD_STATE_IDLE:
            {
                close_dump_a(tsdmx);
                g_mutex_lock(&tsdmx->audio_proc_mutex);
                if (!tsdmx->bchanging_audio_track) {
                    tsdmx->a_first_pts = GST_CLOCK_TIME_NONE;
                    tsdmx->baudio_need_new_segment = TRUE;
                    tsdmx->audio_need_wait_validpts = FALSE;
                }
                else
                {

                    //wait until flush finished in set_prog_audio
                    while(tsdmx->baudio_flushing)
                    {
                        g_mutex_unlock(&tsdmx->audio_proc_mutex);
                        g_usleep(1*1000);
                        g_mutex_lock(&tsdmx->audio_proc_mutex);
                    }
                    //doing switch audio track,can't reset a_first_pts and
                    //reset need_segment according to whether codec is changed
                    if (tsdmx->audio_srcpad)
                    {
                        GST_INFO("set need_new_segment TRUE");
                        tsdmx->baudio_need_new_segment = TRUE;
                    }
                    tsdmx->bchanging_audio_track = FALSE;
                    tsdmx->audio_need_wait_validpts = TRUE;
                    tsdmx->eaudio_proc_thread_state = E_TSDMX_THD_STATE_INIT_HAL;
                }
                g_mutex_unlock(&tsdmx->audio_proc_mutex);

                //1.destroy audio hal-descrambles here?
                if ((U32)-1 != tsdmx->prog_ca_key_info.audio_descramble_id)
                {
                    GST_WARNING_OBJECT(tsdmx, "%s:destroy tsdmx-hal-descramble audio begin...", SMEPLAYER_STAT_TAG_TIME);
                    gst_smetsdmx_destory_hal_descrambles(tsdmx,
                                                         tsdmx->prog_ca_key_info.audio_descramble_id);
                    tsdmx->prog_ca_key_info.audio_descramble_id = (U32)-1;
                    GST_WARNING_OBJECT(tsdmx, "%s:destroy tsdmx-hal-descramble audio end", SMEPLAYER_STAT_TAG_TIME);
                }

                //2.stop audio hal-filter
                GST_WARNING_OBJECT(tsdmx, "%s:stop tsdmx-hal-filter audio begin...", SMEPLAYER_STAT_TAG_TIME);
                (void)gst_smetsdmx_stop_hal_filters(tsdmx, E_DVB_MEDIA_TYPE_AUDIO);
                GST_WARNING_OBJECT(tsdmx, "%s:stop tsdmx-hal-filter audio end", SMEPLAYER_STAT_TAG_TIME);
                g_mutex_lock(&tsdmx->audio_proc_mutex);

                if (E_TSDMX_THD_STATE_IDLE == tsdmx->eaudio_proc_thread_state)
                {
                    g_cond_wait_until(&tsdmx->audio_proc_cond, &tsdmx->audio_proc_mutex,
                                      g_get_monotonic_time () + 1000 * G_TIME_SPAN_MILLISECOND);
                }

                if (E_TSDMX_THD_STATE_TO_IDLE_THEN_INIT == tsdmx->eaudio_proc_thread_state)
                {
                    tsdmx->eaudio_proc_thread_state = E_TSDMX_THD_STATE_INIT_HAL;
                }
                g_mutex_unlock(&tsdmx->audio_proc_mutex);
                break;
            }

            case E_TSDMX_THD_STATE_INIT_HAL:
            {
                berr = gst_smetsdmx_do_audio_init_hal(tsdmx);
                g_mutex_lock(&tsdmx->audio_proc_mutex);

                if (!berr)
                {
                    tsdmx->eaudio_proc_thread_state = E_TSDMX_THD_STATE_IDLE;
                }
                else
                {
                    if (E_TSDMX_THD_STATE_INIT_HAL == tsdmx->eaudio_proc_thread_state)
                    {
                        if(tsdmx->applied_prog_media_info.apid != tsdmx->prog_media_info.apid){
                          //we should do init hal again for CC crazily case , it will play last channel.
                          g_mutex_unlock(&tsdmx->audio_proc_mutex);
                          GST_WARNING_OBJECT(tsdmx, "%s:simultaneous accur. stop tsdmx-hal-filter video begin...", SMEPLAYER_STAT_TAG_TIME);
                          (void)gst_smetsdmx_stop_hal_filters(tsdmx, E_DVB_MEDIA_TYPE_AUDIO);
                          GST_WARNING_OBJECT(tsdmx, "%s:simultaneous accur. stop tsdmx-hal-filter video end", SMEPLAYER_STAT_TAG_TIME);
                          break;
                        }

                        if (tsdmx->prog_media_info.audio_is_ca)
                        {
                            tsdmx->eaudio_proc_thread_state = E_TSDMX_THD_STATE_UPDATE_CA_KEY;
                        }
                        else
                        {
                            tsdmx->eaudio_proc_thread_state = E_TSDMX_THD_STATE_PROCCESS_AV;
                            open_dump_a(tsdmx);
                        }
                    }
                }

                g_mutex_unlock(&tsdmx->audio_proc_mutex);
                break;
            }

            case E_TSDMX_THD_STATE_UPDATE_CA_KEY:
            {
                g_mutex_lock(&tsdmx->audio_proc_mutex);

                if (E_TSDMX_THD_STATE_UPDATE_CA_KEY == tsdmx->eaudio_proc_thread_state)
                {
                    berr = gst_smetsdmx_do_audio_update_ca_key(tsdmx);

                    if (berr)
                    {
                        tsdmx->eaudio_proc_thread_state = E_TSDMX_THD_STATE_PROCCESS_AV;
                        open_dump_a(tsdmx);
                    }
                }

                g_mutex_unlock(&tsdmx->audio_proc_mutex);
                break;
            }

            case E_TSDMX_THD_STATE_PROCCESS_AV:
            {
                berr = gst_smetsdmx_do_audio_process_av(tsdmx);
                break;
            }

            default:
                break;
        }

        if (tsdmx->bexit)
        {
            break;
        }

        if (!berr)
        {
            g_usleep(5 * 1000);
        }
    }

    close_dump_a(tsdmx);

    //1.destroy audio hal-descrambles here?
    if ((U32)-1 != tsdmx->prog_ca_key_info.audio_descramble_id)
    {
        gst_smetsdmx_destory_hal_descrambles(tsdmx,
                                             tsdmx->prog_ca_key_info.audio_descramble_id);
        tsdmx->prog_ca_key_info.audio_descramble_id = (U32)-1;
    }

    //2.stop audio hal-filter
    (void)gst_smetsdmx_stop_hal_filters(tsdmx, E_DVB_MEDIA_TYPE_AUDIO);

    GST_INFO_OBJECT(tsdmx, "proc_audio_stream_thread_func out");
    return NULL;
}

static gboolean
gst_smetsdmx_send_event(GstElement* element, GstEvent* event)
{
    gboolean bret = FALSE;
    GstSmeTSDMX* tsdmx = GST_SMETSDMX (element);

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_send_event in,event:%s",
                    event ? GST_EVENT_TYPE_NAME(event) : "null event");

    if (!event)
    {
        GST_ERROR_OBJECT(tsdmx, "event is null !");
        return FALSE;
    }

    gst_event_ref(event);
    switch (GST_EVENT_TYPE(event))
    {
        case GST_EVENT_FLUSH_START:
        {
            bret = gst_smetsdmx_flush_start(tsdmx, event);
            break;
        }

        case GST_EVENT_FLUSH_STOP:
        {
            bret = gst_smetsdmx_flush_stop(tsdmx, event);
            break;
        }

        default:
            bret = GST_ELEMENT_CLASS(parent_class)->send_event(element, event);
            break;
    }

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_send_event out,event:%s",
                    event ? GST_EVENT_TYPE_NAME(event) : "null event");

    gst_event_unref(event);
    return bret;
}

static GstStateChangeReturn
gst_smetsdmx_change_state (GstElement* element, GstStateChange transition)
{
    GstStateChangeReturn ret;
    GstSmeTSDMX* tsdmx;
    U32 err;

    tsdmx = GST_SMETSDMX (element);

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_change_state in:%s to %s,tsdmx=%p",
                    gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
                    gst_element_state_get_name((GstState)(transition & 0x07)), tsdmx);

    switch (transition)
    {
        case GST_STATE_CHANGE_NULL_TO_READY:
        {
            /* open demux module */
#ifndef LINUX_OS
            g_assert(g_dmx_module);
            GST_WARNING_OBJECT(tsdmx, "%s:open tsdmx-hal-module begin...", SMEPLAYER_STAT_TAG_TIME);
            err = (U32)demux_open(g_dmx_module, &tsdmx->tsdmxhal.hal_handle);
            GST_WARNING_OBJECT(tsdmx, "%s:open tsdmx-hal-module end", SMEPLAYER_STAT_TAG_TIME);
            if (err != SUCCESS)
            {
                GST_ERROR_OBJECT(tsdmx, "demux_open error!! err = %#x", err);
                return GST_STATE_CHANGE_FAILURE;
            }
#else
            tsdmx->tsdmxhal.hal_handle = getDemuxDevice();
#endif
            /* demux module init */
            GST_WARNING_OBJECT(tsdmx, "%s:init tsdmx-hal begin...", SMEPLAYER_STAT_TAG_TIME);
            err = (U32)tsdmx->tsdmxhal.hal_handle->dmx_init(tsdmx->tsdmxhal.hal_handle,
                    (const DMX_INIT_PARAMS_S * const)NULL);
            GST_WARNING_OBJECT(tsdmx, "%s:init tsdmx-hal end", SMEPLAYER_STAT_TAG_TIME);

            if (err != SUCCESS)
            {
                demux_close(tsdmx->tsdmxhal.hal_handle);
                tsdmx->tsdmxhal.hal_handle = NULL;
                GST_ERROR_OBJECT(tsdmx, "dmx_init error!! err = %#x", err);
                return GST_STATE_CHANGE_FAILURE;
            }

            break;
        }

        case GST_STATE_CHANGE_READY_TO_PAUSED:
        {
            err = (U32)pthread_create(&tsdmx->video_proc_thread_id, NULL,
                                 proc_video_stream_thread_func, tsdmx);

            if (err != 0)
            {
                GST_ERROR_OBJECT(tsdmx, "failed to create video_proc_thread");
                tsdmx->video_proc_thread_id = (pthread_t)0;
                return GST_STATE_CHANGE_FAILURE;
            }

            err = (U32)pthread_create(&tsdmx->audio_proc_thread_id, NULL,
                                 proc_audio_stream_thread_func, tsdmx);

            if (err != 0)
            {
                GST_ERROR_OBJECT(tsdmx, "failed to create audio_proc_thread");
                tsdmx->audio_proc_thread_id = (pthread_t)0;
                tsdmx->bexit = TRUE;
                g_mutex_lock(&tsdmx->video_proc_mutex);
                g_cond_signal(&tsdmx->video_proc_cond);
                g_mutex_unlock(&tsdmx->video_proc_mutex);
                GST_INFO_OBJECT(tsdmx, "error join video_proc_thread begin ...");
                pthread_join(tsdmx->video_proc_thread_id, NULL);
                GST_INFO_OBJECT(tsdmx, "error join video_proc_thread end");
                tsdmx->video_proc_thread_id = (pthread_t)0;
                tsdmx->bexit = FALSE;

                return GST_STATE_CHANGE_FAILURE;
            }

            break;
        }

        case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
            break;

        default:
            break;
    }

    GST_INFO_OBJECT (tsdmx, "gst_smetsdmx_change_state:%s to %s,begin...",
                     gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
                     gst_element_state_get_name((GstState)(transition & 0x07)));

    ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

    GST_INFO_OBJECT (element, "gst_smetsdmx_change_state:%s to %s,end,ret=%s",
                     gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
                     gst_element_state_get_name((GstState)(transition & 0x07)),
                     gst_element_state_change_return_get_name(ret));

    if (ret != GST_STATE_CHANGE_SUCCESS)
    {
        //by lrl:we should do deinit relate to what we do before parent_class->change_state
        GST_ERROR_OBJECT(tsdmx, "tsdmx change status error !");
        return ret;
    }

    switch (transition)
    {
        case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
        {
            break;
        }

        case GST_STATE_CHANGE_PAUSED_TO_READY:
        {
            destroy_video_audio_thread(tsdmx);
            gst_smetsdmx_reset(tsdmx);

            break;
        }

        case GST_STATE_CHANGE_READY_TO_NULL:
        {
            DMX_TERM_PARAM_S dmx_term_param;
            (void)VOS_Memset_S(&dmx_term_param, sizeof(DMX_TERM_PARAM_S), 0,
                               sizeof(DMX_TERM_PARAM_S));
            GST_WARNING_OBJECT(tsdmx, "%s:term tsdmx-hal begin...", SMEPLAYER_STAT_TAG_TIME);
            tsdmx->tsdmxhal.hal_handle->dmx_term(tsdmx->tsdmxhal.hal_handle, &dmx_term_param);
            GST_WARNING_OBJECT(tsdmx, "%s:term tsdmx-hal end", SMEPLAYER_STAT_TAG_TIME);
            GST_WARNING_OBJECT(tsdmx, "%s:close tsdmx-hal-module begin...", SMEPLAYER_STAT_TAG_TIME);

#ifndef LINUX_OS
            demux_close(tsdmx->tsdmxhal.hal_handle);
#endif
            GST_WARNING_OBJECT(tsdmx, "%s:term tsdmx-hal-module end", SMEPLAYER_STAT_TAG_TIME);
            tsdmx->tsdmxhal.hal_handle = NULL;
            break;
        }

        default:
            break;
    }

    GST_INFO_OBJECT(tsdmx, "gst_smetsdmx_change_state out:%s to %s,",
                    gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
                    gst_element_state_get_name((GstState)(transition & 0x07)));

    return ret;
}

static void
destroy_video_audio_thread(GstSmeTSDMX* tsdmx)
{
    GST_INFO_OBJECT(tsdmx, "destroy_video_audio_thread in");

    tsdmx->bexit = TRUE;
    g_mutex_lock(&tsdmx->video_proc_mutex);
    g_cond_signal(&tsdmx->video_proc_cond);
    g_mutex_unlock(&tsdmx->video_proc_mutex);

    g_mutex_lock(&tsdmx->audio_proc_mutex);
    g_cond_signal(&tsdmx->audio_proc_cond);
    g_mutex_unlock(&tsdmx->audio_proc_mutex);

    if (tsdmx->video_proc_thread_id)
    {
        GST_INFO_OBJECT(tsdmx, "join video_proc_thread begin ...");
        pthread_join(tsdmx->video_proc_thread_id, NULL);
        GST_INFO_OBJECT(tsdmx, "join video_proc_thread end");
        tsdmx->video_proc_thread_id = (pthread_t)0;
    }

    if (tsdmx->audio_proc_thread_id)
    {
        GST_INFO_OBJECT(tsdmx, "join audio_proc_thread begin ...");
        pthread_join(tsdmx->audio_proc_thread_id, NULL);
        GST_INFO_OBJECT(tsdmx, "join audio_proc_thread end");
        tsdmx->audio_proc_thread_id = (pthread_t)0;
    }

    tsdmx->bexit = FALSE;

    GST_INFO_OBJECT(tsdmx, "destroy_video_audio_thread out");

    return;
}


//plugin definition
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

static gboolean
smetsdmx_hw_pugin_init (GstPlugin* tsdmx)
{
#ifndef LINUX_OS
    gint ret;
#endif
    gboolean bret;

    GST_DEBUG_CATEGORY_INIT (gst_smetsdmx_debug, "smetsdmx", 0, "hisi tsdmx");

    GST_INFO_OBJECT(tsdmx, "tsdmx_hw_pugin_init in");
#ifndef LINUX_OS
    ret = hw_get_module(DEMUX_HARDWARE_MODULE_ID, &g_dmx_module);
    if (ret != SUCCESS || !g_dmx_module)
    {
        g_print("Fatal, failed to open dmx module\n");
        return FALSE;
    }
#endif
    bret = gst_element_register (tsdmx, "smetsdmx", GST_RANK_PRIMARY + 1 ,
                                 GST_TYPE_SMETSDMX);

    GST_INFO_OBJECT(tsdmx, "tsdmx_hw_pugin_init out, bret=%d", bret);

    return bret;
}

//lint -e19
GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    smetsdmx_hw,
    "hisi TS Demux",
    smetsdmx_hw_pugin_init,
    "1.0",
    "LGPL",
    "hisi GStreamer Plugin",
    "http://hisisemi.com/"
);
//lint +e19
