/**************************************************************************//**

  Copyright (C), 2001-2016, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmedvbplaybin.c
 * @brief   gstsmedvbplaybin实现
 * @author
 * @date    2016/4/18
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2016/4/18
 * Author :
 * Desc   : Created file
 *
******************************************************************************/

#define GLIB_DISABLE_DEPRECATION_WARNINGS

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <gst/gst.h>
#include <gst/pbutils/pbutils.h>
#include "gstsme.h"
#include "osal_mem.h"
#include "sme_dvbplayer_type.h"
#include "gstsmeptscalibration.h"

//lint -e19     //无用的声明
//lint -e655    //位操作使用（兼容的）枚举类型
//lint -e717    //do ... while(0)
//lint -e730    //函数的布尔类型参数
//lint -e801    //不赞成goto的使用
//lint -e818    //指针参数可被声明为const的指针
//lint -e826    //可疑的指针到指针转换（区域过小）
//lint -esym(749, PROP_0, PROP_LAST, GST_PLAY_SINK_TYPE_LAST)
//lint -e754    //parent_class not referenced

/**
 * GstPlaySinkType:
 * @GST_PLAY_SINK_TYPE_AUDIO: an audio pad
 * @GST_PLAY_SINK_TYPE_VIDEO: a video pad
 * @GST_PLAY_SINK_TYPE_LAST: the last type
 *
 * Types of pads that can be requested from the sinks.
 */
typedef enum {
  GST_PLAY_SINK_TYPE_AUDIO = 0,
  GST_PLAY_SINK_TYPE_VIDEO,
  GST_PLAY_SINK_TYPE_TEXT,
  GST_PLAY_SINK_TYPE_LAST,
} GstPlaySinkType;

/**
 * GstAutoplugSelectResult:
 * @GST_AUTOPLUG_SELECT_TRY: try to autoplug the current factory
 * @GST_AUTOPLUG_SELECT_EXPOSE: expose the pad as a raw stream
 * @GST_AUTOPLUG_SELECT_SKIP: skip the current factory
 *
 * return values for the autoplug-select signal.
 */
typedef enum {
  GST_AUTOPLUG_SELECT_TRY,
  GST_AUTOPLUG_SELECT_EXPOSE,
  GST_AUTOPLUG_SELECT_SKIP
} GstAutoplugSelectResult;



GST_DEBUG_CATEGORY_STATIC (gst_dvbplay_bin_debug);

#define GST_TYPE_DVBPLAY_BIN               (gst_dvbplay_bin_get_type())
#define GST_DVBPLAY_BIN(obj)               (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DVBPLAY_BIN,GstDvbPlayBin))
//#define GST_DVBPLAY_BIN_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_DVBPLAY_BIN,GstDvbPlayBinClass))
//#define GST_IS_DVBPLAY_BIN(obj)            (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DVBPLAY_BIN))
//#define GST_IS_DVBPLAY_BIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_DVBPLAY_BIN))

#define MAPI_DMX_INVALID_PID  (0x1fff) //invalid pid
#define USE_SME_AUDIO_CLOCK
#define SLOW_PRESYNC_MODE_DEFAULT 1
#define WAIT_PRESYNC_MODE_MAX_TIME (500*GST_MSECOND)
#define SLOW_PRESYNC_RATE (1.0/2)
#define DVB_CLOCK_MAX_BACKWARD (200*GST_MSECOND)//参考时钟最大滞后,也就是流超前

//只有视频时到playing 等待时间,解决没有音频库时视频会顿。
//先临时修改，后续加入动态慢放机制彻底解决。
#define ONLY_VIDEO_TO_PLAYING_TIME (300*1000)
typedef struct _GstDvbPlayBin GstDvbPlayBin;
typedef struct _GstDvbPlayBinClass GstDvbPlayBinClass;
typedef struct _GstDvbSourceCombine GstSourceCombine;
typedef struct _GstDvbPendingPad GstDvbPendingPad;


typedef GstCaps *(*SourceCombineGetMediaCapsFunc) (void);

/* has the info for a combiner and provides the link to the sink */
struct _GstDvbSourceCombine
{
  const gchar *media_list[8];   /* the media types for the combiner */
  SourceCombineGetMediaCapsFunc get_media_caps; /* more complex caps for the combiner */
  GstPlaySinkType type;         /* the sink pad type of the combiner */

  GstElement *combiner;         /* the combiner */
  GPtrArray *channels;
  GstPad *srcpad;               /* the source pad of the combiner */
  GstPad *sinkpad;              /* the sinkpad of the sink when the combiner
                                 * is linked
                                 */
  gulong data_block_id;
  gulong event_block_id;

  GstEvent *event;
};

enum
{
  PLAYBIN_STREAM_AUDIO = 0,
  PLAYBIN_STREAM_VIDEO,
  PLAYBIN_STREAM_LAST
};

static void avelements_free (gpointer data);
static GSequence *avelements_create (GstDvbPlayBin * playbin,
    gboolean isaudioelement);

/* The GstAudioVideoElement structure holding the audio/video decoder
 * and the audio/video sink factories together with field indicating
 * the number of common caps features */
typedef struct
{
  GstElementFactory *dec;       /* audio:video decoder */
  GstElementFactory *sink;      /* audio:video sink */
  guint n_comm_cf;              /* number of common caps features */
} GstAVElement;


#define SME_DVBPLAY_MQ_SIZE_BYTES (15 * 1024 * 1024)//Byte
#define SME_DVBPLAY_MQ_SIZE_TIME (5*GST_SECOND) //5 Secs

#define GST_DVBPLAY_BIN_GET_LOCK(bin) (&((GstDvbPlayBin*)(bin))->lock)
#define GST_DVBPLAY_BIN_LOCK(bin) (g_rec_mutex_lock (GST_DVBPLAY_BIN_GET_LOCK(bin)))
#define GST_DVBPLAY_BIN_UNLOCK(bin) (g_rec_mutex_unlock (GST_DVBPLAY_BIN_GET_LOCK(bin)))

/* lock to protect dynamic callbacks, like no-more-pads */
#define GST_DVBPLAY_BIN_DYN_LOCK(bin)    g_mutex_lock (&(bin)->dyn_lock)
#define GST_DVBPLAY_BIN_DYN_UNLOCK(bin)  g_mutex_unlock (&(bin)->dyn_lock)

/* lock for shutdown */
#define GST_DVBPLAY_BIN_SHUTDOWN_LOCK(bin,label)           \
G_STMT_START {                                          \
  if (G_UNLIKELY (g_atomic_int_get (&bin->shutdown))) { \
    /** BEGIN: Added for DTS2016072506162  2016/07/025 */ \
    GST_DVBPLAY_BIN_DYN_LOCK (bin);                       \
    /** END: Added for DTS2016072506162  2016/07/025 */   \
    goto label;                                           \
  }                                                       \
  GST_DVBPLAY_BIN_DYN_LOCK (bin);                         \
  if (G_UNLIKELY (g_atomic_int_get (&bin->shutdown))) { \
    GST_DVBPLAY_BIN_DYN_UNLOCK (bin);                      \
    goto label;                                         \
  }                                                     \
} G_STMT_END

/* unlock for shutdown */
#define GST_DVBPLAY_BIN_SHUTDOWN_UNLOCK(bin)         \
  GST_DVBPLAY_BIN_DYN_UNLOCK (bin);                  \



#ifndef __LINUX__
typedef struct _tagGstSmeDispRect
{
  gint start_x;
  gint start_y;
  gint width;
  gint height;
} GstSmeDispRect;
#endif


struct _GstDvbPendingPad
{
  GstPad *pad;
  GstDvbPlayBin *playbin;
  gulong event_probe_id;
  gulong notify_caps_id;
};


/**
 * GstDvbPlayBin:
 *
 * dvbplaybin element structure
 */
struct _GstDvbPlayBin
{
  GstPipeline parent;

  GRecMutex lock;
  /* lock protecting dynamic adding/removing */
  GMutex dyn_lock;
  GMutex event_lock;

  /* properties */
  guint switch_channel_mode; /* Change channel mode: freeze frame or black screen*/
  guint disp_mode;  /*display mode:fit-in,fit-out,fit-ful*/
  gpointer disp_ratio; /*display ratio,4:3,16:9*/
  gulong ul_playbin2_rect;

  /* if we are shutting down or not */
  gint shutdown;

  GMutex elements_lock;
  guint32 elements_cookie;
  GList *elements;              /* factories we can use for selecting elements */

  GstElement *dvb_demux;
  GstElement *multiqueue;
  GstElement *audio_sink;       /* configured audio sink, or NULL      */
  GstElement *video_sink;       /* configured video sink, or NULL      */
  GstElement *video_decoder;    /* configured video decoder, or NULL      */
  GstElement *audio_decoder;    /* configured audio decoder, or NULL      */

  GSequence *aelements;         /* a list of GstAVElements for audio stream */
  GSequence *velements;         /* a list of GstAVElements for video stream */

  gboolean have_selector;

  GstSegment segment;
  gboolean received_video_segment;
  gboolean received_audio_segment;

  GList *pending_pads;          /* Pads that have no fixed caps yet */

  GstSourceCombine combiner[PLAYBIN_STREAM_LAST];/* combiners for different streams */
  gboolean have_audio;
  gboolean have_video;
#ifndef __LINUX__
  gpointer vdisp_surface;
  GstSmeDispRect vdisp_rect;
#endif
  gint32 switching_track_flag;

  gpointer vdec_proc_info;
  gpointer adec_proc_info;
  gpointer vsink_proc_info;
  gpointer asink_proc_info;

  gulong overrun_id;
  gulong underrun_id;
  gulong normal_id;
  GstSmePTSCalibrateHDL pts_calibration_hdl;
  gboolean recieved_no_more_pad;
  gboolean enable_vsink_sync;
};

struct _GstDvbPlayBinClass
{
  GstPipelineClass parent_class;
};

/* props */
enum
{
  PROP_0,
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

  PROP_DISP_RECT,
  PROP_SWITCH_CHANNEL_MODE,
  PROP_DISP_MODE,
  PROP_DISP_RATIO,
#ifdef __LINUX__
  PROP_AUDIO_MUTE,
  PROP_AUDIO_VOLUME,
  PROP_AUDIO_CHANNEL_MODE,
#endif
  PROP_PRG_AUDIO_INFO,
  PROP_VDEC_INFO,
  PROP_ADEC_INFO,
  PROP_VSINK_INFO,
  PROP_ASINK_INFO,
  PROP_HWTSDMX_INFO,
  PROP_FREEZE_MODE,
  PROP_VID_SYNC,
  PROP_LAST
};

static GstStaticCaps raw_audio_caps = GST_STATIC_CAPS ("audio/x-raw");
static GstStaticCaps raw_video_caps = GST_STATIC_CAPS ("video/x-raw");

static GstStaticCaps dvbdemux_sink_caps = GST_STATIC_CAPS ("video/mpegts_hw");

static void gst_dvbplay_bin_class_init (GstDvbPlayBinClass * klass);
static void gst_dvbplay_bin_init (GstDvbPlayBin * playbin);
static void gst_dvbplay_bin_finalize (GObject * object);

static void gst_dvbplay_bin_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * spec);
static void gst_dvbplay_bin_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * spec);

static GstStateChangeReturn gst_dvbplay_bin_change_state (GstElement * element,
    GstStateChange transition);
static gboolean gst_dvbplay_bin_send_event (GstElement * element,
    GstEvent * event);

static void pad_added_cb (GstElement * element, GstPad * pad,
    GstDvbPlayBin * playbin);
static void pad_removed_cb (GstElement * element, GstPad * pad,
    GstDvbPlayBin * playbin);
static void no_more_pads_cb (GstElement * element, GstDvbPlayBin * playbin);

static void caps_changed_cb (GstElement * element, GstPad * pad,
    GstCaps *new_cap,GstDvbPlayBin * playbin);
static gboolean setup_dvbdemux(GstCaps * caps, GstDvbPlayBin * playbin);
static void
    gst_pending_pad_free (GstDvbPendingPad * ppad);

static GstPadProbeReturn
    pad_event_cb (GstPad * pad, GstPadProbeInfo * info, gpointer data);


static void
analyze_new_pad (GstDvbPlayBin * playbin, GstElement * src, GstPad * pad,
    GstCaps * caps);

static GstAutoplugSelectResult
autoplug_select (GstDvbPlayBin * playbin, GstPad * pad,
    GstCaps * caps, GstElementFactory * factory);
static gboolean
autoplug_continue (GstDvbPlayBin * playbin, GstPad * pad, GstCaps * caps);
static void
gst_dvbplay_bin_combiners_event_unblock (GstDvbPlayBin * playbin);
static void
complete_connect_combine(GstSourceCombine *combine, GstDvbPlayBin * playbin);
static void
gst_dvbplay_bin_handle_message(GstBin * bin, GstMessage * msg);
static void
gst_dvbplay_bin_combiners_data_unblock (GstDvbPlayBin * playbin);
static void
gst_dvbplay_bin_combiners_event_block (GstDvbPlayBin * playbin);

static GstElementClass *parent_class;

static void set_presync_info(GstDvbPlayBin * playbin)
{
  if(playbin->have_audio && playbin->have_video){
    if(playbin->audio_sink){
      GST_WARNING_OBJECT(playbin, "pre-sync:asink:set slow_presync:%d,"
        "wait-presync-max-time:%lld, slow-presync-rate:%f",
        SLOW_PRESYNC_MODE_DEFAULT, WAIT_PRESYNC_MODE_MAX_TIME,
        SLOW_PRESYNC_RATE);
      g_object_set(playbin->audio_sink,
        GST_SME_PROP_KEY_SLOW_PRESYNC, SLOW_PRESYNC_MODE_DEFAULT,
        GST_SME_PROP_KEY_WAIT_PRESYNC_MAXTIME, WAIT_PRESYNC_MODE_MAX_TIME,
        GST_SME_PROP_KEY_SLOW_PRESYNC_RATE, SLOW_PRESYNC_RATE, NULL);
    }
    if(playbin->video_sink){
      GST_WARNING_OBJECT(playbin, "pre-sync:vsink:set slow_presync:%d,"
        "wait-presync-max-time:%lld, slow-presync-rate:%f",
        SLOW_PRESYNC_MODE_DEFAULT, WAIT_PRESYNC_MODE_MAX_TIME,
        SLOW_PRESYNC_RATE);
      g_object_set(playbin->video_sink,
        GST_SME_PROP_KEY_SLOW_PRESYNC, SLOW_PRESYNC_MODE_DEFAULT,
        GST_SME_PROP_KEY_WAIT_PRESYNC_MAXTIME, WAIT_PRESYNC_MODE_MAX_TIME,
        GST_SME_PROP_KEY_SLOW_PRESYNC_RATE, SLOW_PRESYNC_RATE, NULL);
    }
  }else{
    if(playbin->audio_sink){
      GST_WARNING_OBJECT(playbin, "pre-sync:asink:disable slow_presync");
      g_object_set(playbin->audio_sink,
        GST_SME_PROP_KEY_SLOW_PRESYNC, 0, NULL);
    }
    if(playbin->video_sink){
      GST_WARNING_OBJECT(playbin, "pre-sync:vsink:disable slow_presync");
      g_object_set(playbin->video_sink,
        GST_SME_PROP_KEY_SLOW_PRESYNC, 0, NULL);
    }
  }

  return;
}

static void end_presync_info(GstDvbPlayBin * playbin)
{
  if(!playbin->have_audio || !playbin->have_video){
    if(GST_STATE_PLAYING == GST_STATE(playbin)
      || (playbin->have_video
          && playbin->video_sink
          && GST_STATE_PLAYING == GST_STATE(playbin->video_sink))){
      GST_WARNING_OBJECT(playbin, "pre-sync:%s, end slow-pre-sync !",
        !playbin->have_video ? "no video" : "no audio");
      g_object_set(playbin->video_sink,
        GST_SME_PROP_KEY_SLOW_PRESYNC, 2, NULL);
      if(playbin->audio_sink){
        g_object_set(playbin->audio_sink,
          GST_SME_PROP_KEY_SLOW_PRESYNC, 2, NULL);
      }
    }else{
      GST_WARNING_OBJECT(playbin, "pre-sync:%s, disable slow-pre-sync !",
        !playbin->have_video ? "no video" : "no audio");
      g_object_set(playbin->video_sink,
        GST_SME_PROP_KEY_SLOW_PRESYNC, 0, NULL);
      if(playbin->audio_sink){
        g_object_set(playbin->audio_sink,
          GST_SME_PROP_KEY_SLOW_PRESYNC, 0, NULL);
      }
    }
  }

  return;
}

static GType
gst_dvbplay_bin_get_type (void)
{
  static GType gst_dvbplay_bin_type = 0;

  if (!gst_dvbplay_bin_type) {
    static const GTypeInfo gst_dvbplay_bin_info = {
      sizeof (GstDvbPlayBinClass),
      NULL,
      NULL,
      (GClassInitFunc) gst_dvbplay_bin_class_init,
      NULL,
      NULL,
      sizeof (GstDvbPlayBin),
      0,
      (GInstanceInitFunc) gst_dvbplay_bin_init,
      NULL
    };

    gst_dvbplay_bin_type = g_type_register_static (GST_TYPE_PIPELINE,
                                                   "GstDvbPlayBin",
                                                   &gst_dvbplay_bin_info,
                                                   (GTypeFlags)0);

  }

  return gst_dvbplay_bin_type;
}

/* helper function to lookup stuff in lists */
static gboolean
array_has_value (const gchar * values[], const gchar * value, gboolean exact)
{
  gint i;

  for (i = 0; values[i]; i++) {
    if (exact && !g_strcmp0(value, values[i]))
      return TRUE;
    if (!exact && g_str_has_prefix (value, values[i]))
      return TRUE;
  }
  return FALSE;
}

static GstCaps *
get_pad_caps (GstPad * pad)
{
  GstCaps *caps;

  /* first check the pad caps, if this is set, we are positively sure it is
   * fixed and exactly what the element will produce. */
  caps = gst_pad_get_current_caps (pad);

  /* then use the getcaps function if we don't have caps. These caps might not
   * be fixed in some cases, in which case analyze_new_pad will set up a
   * notify::caps signal to continue autoplugging. */
  if (caps == NULL)
    caps = gst_pad_query_caps (pad, NULL);

  return caps;
}
/*****
 * convenience functions
 *****/

/* find_sink_pad
 *
 * Returns the first sink pad of the given element, or NULL if it doesn't have
 * any.
 */

static GstPad *
find_sink_pad (GstElement * element)
{
  GstIterator *it;
  GstPad *pad = NULL;
  GValue item = { 0, };

  it = gst_element_iterate_sink_pads (element);

  if ((gst_iterator_next (it, &item)) == GST_ITERATOR_OK)
    pad = g_value_dup_object (&item);

  if(G_IS_VALUE (&item)){
    g_value_unset (&item);
  }

  if (it){
    gst_iterator_free (it);
  }

  return pad;
}

static void
gst_dvbplay_bin_class_init (GstDvbPlayBinClass * klass)
{
  GObjectClass *gobject_klass;
  GstElementClass *gstelement_klass;
  GstBinClass *gstbin_klass;

  gobject_klass = (GObjectClass *) klass;
  gstelement_klass = (GstElementClass *) klass;
  gstbin_klass = GST_BIN_CLASS(klass);

  parent_class = g_type_class_peek_parent (klass);

  gobject_klass->set_property = gst_dvbplay_bin_set_property;
  gobject_klass->get_property = gst_dvbplay_bin_get_property;

  gobject_klass->finalize = gst_dvbplay_bin_finalize;


  // 1. The property of ts program info
  g_object_class_install_property(gobject_klass, PROP_PRG_MEDIA_INFO,
      g_param_spec_pointer(GST_SME_PROP_KEY_DVB_MEDIA_INFO, "program media info",
      "program media infomation",
      (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  // 2. The property of ts audio info
  g_object_class_install_property(gobject_klass, PROP_PRG_AUDIO_INFO,
      g_param_spec_pointer(GST_SME_PROP_KEY_DVB_AUDIO_INFO, "program audio info",
      "program audio infomation",
      (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  // 3. The property of ts ca info
  g_object_class_install_property(gobject_klass, PROP_PRG_CA_INFO,
      g_param_spec_pointer(GST_SME_PROP_KEY_DVB_CA_INFO, "program ca info",
          "program ca infomation",
          (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  // 4. The property of even key info
  g_object_class_install_property(gobject_klass, PROP_VIDEO_EVEN_KEY_INFO,
        g_param_spec_pointer(GST_SME_PROP_KEY_DVB_VIDEO_EVEN_KEY,
            "video even key info", "set video even key info",
            (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  // 5. The property of odd key info
  g_object_class_install_property(gobject_klass, PROP_VIDEO_ODD_KEY_INFO,
        g_param_spec_pointer(GST_SME_PROP_KEY_DVB_VIDEO_ODD_KEY,
            "video odd key info", "set video odd key info",
            (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  // 6. The property of even key info
  g_object_class_install_property(gobject_klass, PROP_AUDIO_EVEN_KEY_INFO,
        g_param_spec_pointer(GST_SME_PROP_KEY_DVB_AUDIO_EVEN_KEY,
            "audio even key info", "set audio even key info",
            (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  // 7. The property of odd key info
  g_object_class_install_property(gobject_klass, PROP_AUDIO_ODD_KEY_INFO,
        g_param_spec_pointer(GST_SME_PROP_KEY_DVB_AUDIO_ODD_KEY,
            "audio odd key info", "set audio odd key info",
            (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

#ifdef ENABLE_KEY_IV
  // 8. The property of set even iv information
  g_object_class_install_property(gobject_klass, PROP_VIDEO_EVEN_KEY_IV,
        g_param_spec_pointer(GST_SME_PROP_KEY_DVB_VIDEO_EVEN_KEY_IV,
            "set video even key iv", "descrambler set video even key iv",
            (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  // 9. The property of set even iv information
  g_object_class_install_property(gobject_klass, PROP_VIDEO_ODD_KEY_IV,
        g_param_spec_pointer(GST_SME_PROP_KEY_DVB_VIDEO_ODD_KEY_IV,
            "set vidde odd key iv", "descrambler set video odd key iv",
            (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  // 10. The property of set even iv information
  g_object_class_install_property(gobject_klass, PROP_AUDIO_EVEN_KEY_IV,
        g_param_spec_pointer(GST_SME_PROP_KEY_DVB_AUDIO_EVEN_KEY_IV,
            "set audio even key iv", "descrambler set audio even key iv",
            (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  // 11. The property of set even iv information
  g_object_class_install_property(gobject_klass, PROP_AUDIO_ODD_KEY_IV,
        g_param_spec_pointer(GST_SME_PROP_KEY_DVB_AUDIO_ODD_KEY_IV,
            "set audio odd key iv", "descrambler set audio odd key iv",
            (GParamFlags)G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
#endif

  // 12. The property of video display rect
  g_object_class_install_property (gobject_klass, PROP_DISP_RECT,
        g_param_spec_ulong ("gst-sme-prop-key-vdisp-rect", "video disp rect",
            "video display disp rect instance ",
            0, (gulong)(-1), 0,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  // 13. The property of switch channel mode
  g_object_class_install_property (gobject_klass, PROP_SWITCH_CHANNEL_MODE,
          g_param_spec_uint("switch-channel-mode", "switch channel mode",
            "switch channel mode", 0, 1, 0,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_klass, PROP_DISP_MODE,
          g_param_spec_uint(GST_SME_PROP_KEY_DISP_MODE, "video display mode",
            "video display mode", 0, 4, 4, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_klass, PROP_DISP_RATIO,
          g_param_spec_pointer(GST_SME_PROP_KEY_DISP_RADIO, "video display ratio",
            "video display radio", G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_klass, PROP_FREEZE_MODE,
        g_param_spec_uint(GST_SME_PROP_KEY_VIDEO_FREEZE_MODE, "video freeze mode",
          "video freeze mode", 0, 1, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));


#ifdef __LINUX__
  // 13. audio output setting
  g_object_class_install_property(gobject_klass, PROP_AUDIO_MUTE,
      g_param_spec_uint (GST_SME_PROP_KEY_MUTE_AUDIO, "mute-audio",
      "mute-audio", 0, 1, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property(gobject_klass, PROP_AUDIO_VOLUME,
      g_param_spec_uint (GST_SME_PROP_KEY_AUDIO_VOLUME, "audio-volume",
      "audio-volume", 0, 100, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property(gobject_klass, PROP_AUDIO_CHANNEL_MODE,
      g_param_spec_uint (GST_SME_PROP_KEY_AUDIO_CHANNEL_MODE, "audio-channel-mode",
      "audio-channel-mode", 0, 8, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));//A_TRACK_MODE_BUTT
#endif
g_object_class_install_property (gobject_klass, PROP_VDEC_INFO,
        g_param_spec_pointer (GST_SME_BENCH_MARK_VDEC_INFO, "omx video decoder info",
          "poniter of video decoder",
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_klass, PROP_ADEC_INFO,
        g_param_spec_pointer (GST_SME_BENCH_MARK_ADEC_INFO, "audio decoder info",
          "poniter of audio decoder",
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_klass, PROP_VSINK_INFO,
        g_param_spec_pointer (GST_SME_BENCH_MARK_VSINK_INFO, "video sink proc info",
          "poniter of video sink",
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_klass, PROP_ASINK_INFO,
        g_param_spec_pointer (GST_SME_BENCH_MARK_ASINK_INFO, "audio sink proc info",
          "poniter of audio sink",
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_klass, PROP_HWTSDMX_INFO,
        g_param_spec_pointer (GST_SME_BENCH_MARK_HWTSDEMUX_INFO, "hwtsdmx proc info",
          "poniter of hwtsdmx proc info",
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_klass, PROP_VID_SYNC,
        g_param_spec_boolean(GST_SME_PROP_KEY_VID_SYNC, "video do sync",
        "video do sync", TRUE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  gst_element_class_set_static_metadata (gstelement_klass,
      "DVB Player Bin 2", "Generic/Bin/DVB Player",
      "play dvb stream",
      "Huawei");

  gstelement_klass->change_state =
      GST_DEBUG_FUNCPTR (gst_dvbplay_bin_change_state);

  gstelement_klass->send_event =
      GST_DEBUG_FUNCPTR (gst_dvbplay_bin_send_event);
  gstbin_klass->handle_message =
      GST_DEBUG_FUNCPTR (gst_dvbplay_bin_handle_message);
}

static void
gst_dvbplay_bin_handle_message(GstBin * bin, GstMessage * msg)
{
  GstDvbPlayBin *playbin = GST_DVBPLAY_BIN (bin);

  if ((GST_MESSAGE_ERROR == GST_MESSAGE_TYPE (msg))
    || (GST_MESSAGE_WARNING == GST_MESSAGE_TYPE (msg))){
    GstObject* obj_src = GST_MESSAGE_SRC(msg);
    gboolean need_check_v = FALSE;
    gboolean need_check_a = FALSE;

    GST_WARNING_OBJECT(playbin, "bus_call, msg=%#x, typename:%s, obj_src:%p,"
        "audio_decoder=%p,video_decoder=%p, dvb_demux=%p",
        GST_MESSAGE_TYPE (msg), GST_MESSAGE_TYPE_NAME(msg),
        obj_src, playbin->audio_decoder,
        playbin->video_decoder, playbin->dvb_demux);

    if(GST_IS_ELEMENT(obj_src)){
      GstElement* element = GST_ELEMENT_CAST(obj_src);
      if (element == playbin->dvb_demux) {
         GST_WARNING_OBJECT(playbin, "demux warning or error !");
        if(GST_MESSAGE_ERROR == GST_MESSAGE_TYPE (msg)){
          GError *err;
          gchar *debug = NULL;
          GstMessage *new_msg;

          gst_message_parse_error (msg, &err, &debug);
          new_msg = gst_message_new_warning (msg->src, err, "demux/error");
          gst_message_unref (msg);
          g_error_free (err);
          g_free (debug);
          msg = new_msg;
        }else {
          //warning
          GError *err = NULL;
          gchar *debug = NULL;

          gst_message_parse_warning(msg, &err, &debug);
          if(err && err->code == GST_STREAM_ERROR_CODEC_NOT_FOUND){
            if(debug && strstr(debug, GST_SME_STRAM_TAG_ADEC)){
              GST_WARNING_OBJECT(playbin, "unsupport audio decoder !");
              need_check_a = TRUE;
            }else if(debug && strstr(debug, GST_SME_STRAM_TAG_VDEC)){
              GST_WARNING_OBJECT(playbin, "unsupport video decoder !");
              need_check_v = TRUE;
            }
          }
          if(debug){
            g_free (debug);
          }
          if(err){
            g_error_free (err);
          }
        }
      }

      if (need_check_v || element == playbin->video_decoder){
        gboolean need_force_async_done;
        //unblock combiner event probe
        GST_WARNING_OBJECT(playbin, "video warning or error !");
        g_mutex_lock(&playbin->event_lock);
        playbin->have_video = FALSE;
        need_force_async_done = playbin->recieved_no_more_pad;
        g_mutex_unlock(&playbin->event_lock);
        gst_dvbplay_bin_combiners_event_unblock (playbin);
        //force video_sink async_done if async_start send
        //force video_sink send async_done after async_start
        //if async_start not send.
        if(need_force_async_done && playbin->video_sink){
          g_object_set(playbin->video_sink, "force-async-done",
            TRUE, NULL);
          end_presync_info(playbin);
        }

        if(GST_MESSAGE_ERROR == GST_MESSAGE_TYPE (msg) && playbin->have_audio){
          GError *err = NULL;
          gchar *debug = NULL;
          GstMessage *new_msg;

          gst_message_parse_error (msg, &err, &debug);
          new_msg = gst_message_new_warning (msg->src, err, "video/error");
          gst_message_unref (msg);
          if(debug){
            g_free (debug);
          }
          if(err){
            g_error_free (err);
          }
          msg = new_msg;
        }
      }else if (need_check_a || element == playbin->audio_decoder) {
        gboolean need_force_async_done;
        GST_WARNING_OBJECT(playbin, "audio warning or error !");
        g_mutex_lock(&playbin->event_lock);
        playbin->have_audio = FALSE;
        need_force_async_done = playbin->recieved_no_more_pad;
        g_mutex_unlock(&playbin->event_lock);
        //force audio_sink async_done if async_start send
        //force audio_sink send async_done after async_start
        //if async_start not send.
        if(need_force_async_done && playbin->audio_sink){
          g_object_set(playbin->audio_sink, "force-async-done", TRUE, NULL);
          end_presync_info(playbin);
        }
        if(GST_MESSAGE_ERROR == GST_MESSAGE_TYPE (msg) && playbin->have_video){
          GError *err = NULL;
          gchar *debug = NULL;
          GstMessage *new_msg;

          gst_message_parse_error (msg, &err, &debug);
          new_msg = gst_message_new_warning (msg->src, err, "audio/error");
          gst_message_unref (msg);
          if(debug){
            g_free (debug);
          }
          if(err){
            g_error_free (err);
          }
          msg = new_msg;
        }
      }
    }
  }else if(GST_MESSAGE_RENDER_FIRST_VIDEO_FRAME == GST_MESSAGE_TYPE (msg)){
    GstObject* obj_src = GST_MESSAGE_SRC(msg);

    if(NULL != obj_src){
      GstElement* element = GST_ELEMENT_CAST(obj_src);

      if(element == playbin->video_sink){
        GST_WARNING_OBJECT(playbin, "pre-sync:vsink:1st render frame,asink:%p",
          playbin->audio_sink);
        if(playbin->audio_sink){
          g_object_set(playbin->audio_sink, GST_SME_PROP_KEY_START_WAIT_PRESYNC, TRUE, NULL);
        }
      }else{
        GST_INFO_OBJECT(playbin, "not vsink, not asink");
      }
    }
  }else if(GST_MESSAGE_RENDER_FIRST_AUDIO_FRAME == GST_MESSAGE_TYPE (msg)){
    GstObject* obj_src = GST_MESSAGE_SRC(msg);

    if(NULL != obj_src){
      GstElement* element = GST_ELEMENT_CAST(obj_src);

      if(element == playbin->audio_sink){
        GST_WARNING_OBJECT(playbin, "pre-sync:asink:first render frame,vsink:%p",
          playbin->video_sink);
        if(playbin->video_sink){
          g_object_set(playbin->video_sink, GST_SME_PROP_KEY_SLOW_PRESYNC, 2, NULL);
        }
        g_object_set(playbin->audio_sink, GST_SME_PROP_KEY_SLOW_PRESYNC, 2, NULL);
      }else{
        GST_WARNING_OBJECT(playbin, "element is not audio_sink, why ?");
      }
    }
  }
#if SLOW_PRESYNC_MODE_DEFAULT
  else if(GST_MESSAGE_RENDER_FIRST_VIDEO_FRAME == GST_MESSAGE_TYPE (msg)){
    GstObject* obj_src = GST_MESSAGE_SRC(msg);

    if(NULL != obj_src){
      GstElement* element = GST_ELEMENT_CAST(obj_src);

      if(element == playbin->video_sink){
        GST_WARNING_OBJECT(playbin, "pre-sync:vsink:1st render frame,asink:%p",
          playbin->audio_sink);
        if(playbin->audio_sink){
          g_object_set(playbin->audio_sink, GST_SME_PROP_KEY_START_WAIT_PRESYNC, TRUE, NULL);
        }
      }else{
        GST_INFO_OBJECT(playbin, "not vsink, not asink");
      }
    }
  }else if(GST_MESSAGE_RENDER_FIRST_AUDIO_FRAME == GST_MESSAGE_TYPE (msg)){
    GstObject* obj_src = GST_MESSAGE_SRC(msg);

    if(NULL != obj_src){
      GstElement* element = GST_ELEMENT_CAST(obj_src);

      if(element == playbin->audio_sink){
        GST_WARNING_OBJECT(playbin, "pre-sync:asink:first render frame,vsink:%p",
          playbin->video_sink);
        if(playbin->video_sink){
          g_object_set(playbin->video_sink, GST_SME_PROP_KEY_SLOW_PRESYNC, 2, NULL);
        }
        g_object_set(playbin->audio_sink, GST_SME_PROP_KEY_SLOW_PRESYNC, 2, NULL);
      }else{
        GST_WARNING_OBJECT(playbin, "element is not audio_sink, why ?");
      }
    }
  }
#endif

  GST_BIN_CLASS (parent_class)->handle_message (bin, msg);

  return;
}

static gint
compare_factories_func (gconstpointer p1, gconstpointer p2)
{
  GstPluginFeature *f1, *f2;
  gboolean is_sink1, is_sink2;
  gboolean is_parser1, is_parser2;

  f1 = (GstPluginFeature *) p1;
  f2 = (GstPluginFeature *) p2;

  is_sink1 = gst_element_factory_list_is_type (GST_ELEMENT_FACTORY_CAST (f1),
      GST_ELEMENT_FACTORY_TYPE_SINK);
  is_sink2 = gst_element_factory_list_is_type (GST_ELEMENT_FACTORY_CAST (f2),
      GST_ELEMENT_FACTORY_TYPE_SINK);
  is_parser1 = gst_element_factory_list_is_type (GST_ELEMENT_FACTORY_CAST (f1),
      GST_ELEMENT_FACTORY_TYPE_PARSER);
  is_parser2 = gst_element_factory_list_is_type (GST_ELEMENT_FACTORY_CAST (f2),
      GST_ELEMENT_FACTORY_TYPE_PARSER);

  /* First we want all sinks as we prefer a sink if it directly
   * supports the current caps */
  if (is_sink1 && !is_sink2)
    return -1;
  else if (!is_sink1 && is_sink2)
    return 1;

  /* Then we want all parsers as we always want to plug parsers
   * before decoders */
  if (is_parser1 && !is_parser2)
    return -1;
  else if (!is_parser1 && is_parser2)
    return 1;

  /* And if it's a both a parser or sink we first sort by rank
   * and then by factory name */
  return gst_plugin_feature_rank_compare_func (p1, p2);
}

static GstPadProbeReturn
_combiner_event_probe (GstPad * pad, GstPadProbeInfo * info, gpointer udata)
{
  GstPadProbeReturn ret = GST_PAD_PROBE_OK;
  GstSegment segment;
  GstSegment *audio_segment = NULL;
  GstDvbPlayBin *playbin = udata;
  gboolean isaudio = (pad == playbin->combiner[PLAYBIN_STREAM_AUDIO].srcpad);
  gboolean isvideo = (pad == playbin->combiner[PLAYBIN_STREAM_VIDEO].srcpad);
  gboolean have_video = playbin->have_video;
  gboolean have_audio = playbin->have_audio;
  GstEvent *event = GST_PAD_PROBE_INFO_DATA (info);

  gst_segment_init(&segment,GST_FORMAT_TIME);

  GST_INFO("_combiner_event_probe in(event:%s)", GST_EVENT_TYPE_NAME(event));

  switch (GST_EVENT_TYPE (event)) {
    /** Process logic of segment: */
    /** 1. Make the audio segment the same as vidoe's value for av sync*/
    /** 2. Block the segment event until received both audio and video's
    event*/
    /** 3. If there is only audio or only video in pipeline, then unblock it if
    received segment event*/
    /** 4. Segment event is either for audio or for video, so it is impossible
    both "isaudio" and "isvideo" are TRUE */
    /********************************/
    case GST_EVENT_SEGMENT:{
      g_mutex_lock (&playbin->event_lock);
      gst_event_copy_segment(event, &segment);
      if(isaudio) {
        GST_INFO("segment event from audio!");
        playbin->received_audio_segment = TRUE;
        if(playbin->combiner[PLAYBIN_STREAM_AUDIO].event){
          gst_event_unref(playbin->combiner[PLAYBIN_STREAM_AUDIO].event);
          playbin->combiner[PLAYBIN_STREAM_AUDIO].event = NULL;
        }

        playbin->combiner[PLAYBIN_STREAM_AUDIO].event = gst_event_ref(event);

        ret = GST_PAD_PROBE_PASS;
        if(have_video && playbin->received_video_segment) {
          /* modify the segment according to video segment*/
          gst_event_parse_segment(playbin->combiner[PLAYBIN_STREAM_AUDIO].event,
            (const GstSegment**)&audio_segment);
          GST_INFO("update segment event,segment->start from %lld to %lld",
                   segment.start,playbin->segment.start);

          if (audio_segment) {
            audio_segment->start = playbin->segment.start;
            segment.start = playbin->segment.start;
            GST_INFO("already received video segment,unblock auido pad!");
          } else {
            GST_WARNING("Segment is NULL");
          }

          gst_dvbplay_bin_combiners_event_unblock(playbin);
        } else if (have_video) {
          GST_INFO("wait for video segment,continue to block!");
          ret = GST_PAD_PROBE_OK;
        } else {
          GST_INFO("only audio, unblock audio pad");
          gst_dvbplay_bin_combiners_event_unblock(playbin);
        }
      }

      if(isvideo){
        GST_INFO("segment event from video!");
        playbin->received_video_segment = TRUE;
        gst_event_copy_segment(event, &playbin->segment);
        ret = GST_PAD_PROBE_PASS;

        if (have_audio && playbin->received_audio_segment) {
          GST_INFO("already received audio segement,update it and unblock auido pad!");
          /* got audio segment before, modify it */
          gst_event_parse_segment(playbin->combiner[PLAYBIN_STREAM_AUDIO].event,
            (const GstSegment**)&audio_segment);
          if (audio_segment) {
            GST_INFO("update segment->start from %lld to %lld",
                     audio_segment->start,playbin->segment.start);
            audio_segment->start = playbin->segment.start;
            gst_dvbplay_bin_combiners_event_unblock(playbin);
          }
        }else if (!have_audio) {
          GST_INFO("only have video segment,unblock pad!");
          gst_dvbplay_bin_combiners_event_unblock(playbin);
        }
      }
      g_mutex_unlock(&playbin->event_lock);

      if (ret == GST_PAD_PROBE_PASS)
        GST_INFO("send segment: start:%lld, position:%lld",
           segment.start, segment.position);
      break;
    }
    default:
      ret = GST_PAD_PROBE_PASS;
      break;
  }
  GST_INFO("_combiner_event_probe out(event:%s)", GST_EVENT_TYPE_NAME(event));
  return ret;

}



/* Must be called with elements lock! */
static void
gst_dvbplay_bin_update_elements_list (GstDvbPlayBin * playbin)
{
  GList *res, *tmp;
  guint cookie;

  cookie = gst_registry_get_feature_list_cookie (gst_registry_get ());

  if (!playbin->elements || playbin->elements_cookie != cookie) {
    if (playbin->elements) {
      gst_plugin_feature_list_free (playbin->elements);
      playbin->elements = NULL;
    }
    res =
        gst_element_factory_list_get_elements
        (GST_ELEMENT_FACTORY_TYPE_DECODABLE, GST_RANK_MARGINAL);
    tmp =
        gst_element_factory_list_get_elements
        (GST_ELEMENT_FACTORY_TYPE_AUDIOVIDEO_SINKS, GST_RANK_MARGINAL);
    playbin->elements = g_list_concat (res, tmp);
    playbin->elements = g_list_sort (playbin->elements, compare_factories_func);
  }

  if (!playbin->aelements || playbin->elements_cookie != cookie) {
    if (playbin->aelements)
      g_sequence_free (playbin->aelements);
    playbin->aelements = avelements_create (playbin, TRUE);
  }

  if (!playbin->velements || playbin->elements_cookie != cookie) {
    if (playbin->velements)
      g_sequence_free (playbin->velements);
    playbin->velements = avelements_create (playbin, FALSE);
  }

  playbin->elements_cookie = cookie;
}

static gboolean
dvbplaybin_deinit(GstDvbPlayBin * playbin)
{
  if (playbin->elements) {
    gst_plugin_feature_list_free (playbin->elements);
    playbin->elements = NULL;
  }

  if (playbin->aelements){
    g_sequence_free (playbin->aelements);
    playbin->aelements = NULL;
  }

  if (playbin->velements) {
    g_sequence_free (playbin->velements);
    playbin->velements = NULL;
  }

  return TRUE;
}

/*
  *  multiqueue overrun callback
  */
static void
multiqueue_overrun_cb (GstElement * multiqueue, GstDvbPlayBin * playbin)
{
  g_return_if_fail (multiqueue);
  g_return_if_fail (playbin);
  if (!playbin->audio_sink)
    return;

  GST_WARNING_OBJECT (playbin, "dvbplaybin overrun callback-");

  g_object_set (playbin->audio_sink, "buffer-status", GST_SME_OVERRUN, NULL);
  if(playbin->video_sink){
    g_object_set (playbin->video_sink, "buffer-status", GST_SME_OVERRUN, NULL);
  }
}

/*
  *  multiqueue underrun callback
  */
static void
multiqueue_underrun_cb (GstElement * multiqueue, GstDvbPlayBin * playbin)
{
  g_return_if_fail (multiqueue);
  g_return_if_fail (playbin);

  if (!playbin->audio_sink)
    return;

  GST_DEBUG_OBJECT (playbin, "dvbplaybin underrun callback");

  g_object_set (playbin->audio_sink, "buffer-status", GST_SME_UNDERRUN, NULL);
  if(playbin->video_sink){
    g_object_set (playbin->video_sink, "buffer-status", GST_SME_UNDERRUN, NULL);
  }
}

/*
  *  multiqueue underrun callback
  */
static void
multiqueue_normalising_cb (GstElement * multiqueue, GstDvbPlayBin * playbin)
{
  g_return_if_fail (multiqueue);
  g_return_if_fail (playbin);
  if (!playbin->audio_sink)
    return;

  GST_DEBUG_OBJECT (playbin, "dvbplaybin normal callback");

  g_object_set (playbin->audio_sink, "buffer-status", GST_SME_NORMAL, NULL);

  if(playbin->video_sink){
    g_object_set (playbin->video_sink, "buffer-status", GST_SME_NORMAL, NULL);
  }
}

static void
gst_dvbplay_bin_init (GstDvbPlayBin * playbin)
{
  GstCaps *tsdmx_caps;

  g_rec_mutex_init (&playbin->lock);
  g_mutex_init (&playbin->dyn_lock);
  g_mutex_init (&playbin->event_lock);

  playbin->have_selector = TRUE;
  /* first filter out the interesting element factories */
  g_mutex_init (&playbin->elements_lock);
  playbin->underrun_id = 0;
  playbin->overrun_id = 0;
  playbin->switch_channel_mode = E_DVB_CC_MODE_LAST;//default freeze frame
  playbin->disp_mode = 0;
  playbin->disp_ratio = NULL;
  gst_dvbplay_bin_update_elements_list(playbin);
  tsdmx_caps = gst_static_caps_get(&dvbdemux_sink_caps);
  setup_dvbdemux(tsdmx_caps, playbin);
  playbin->multiqueue = gst_element_factory_make ("multiqueue", NULL);
  if (playbin->multiqueue) {
    g_object_set (playbin->multiqueue, "use-buffering", FALSE, NULL);
    g_object_set (playbin->multiqueue,
        "max-size-bytes", SME_DVBPLAY_MQ_SIZE_BYTES,
        "max-size-time", SME_DVBPLAY_MQ_SIZE_TIME, NULL);
    gst_element_set_state (playbin->multiqueue, GST_STATE_PAUSED);
    gst_bin_add (GST_BIN (playbin), gst_object_ref (playbin->multiqueue));
    /* register overrun callback */
    playbin->overrun_id = g_signal_connect(
        G_OBJECT(playbin->multiqueue),
        "overrun",
        G_CALLBACK(multiqueue_overrun_cb),
        playbin );
    /* register underrun callback */
    playbin->underrun_id = g_signal_connect(
        G_OBJECT(playbin->multiqueue),
        "underrun",
        G_CALLBACK(multiqueue_underrun_cb),
        playbin );
    /* register normal callback */
    playbin->normal_id = g_signal_connect(
        G_OBJECT(playbin->multiqueue),
        "normalising",
        G_CALLBACK(multiqueue_normalising_cb),
        playbin );
  } else {
    playbin->multiqueue = NULL;
    GST_WARNING("no multiqueue!");
  }

  /* If you add any items to these lists, check that media_list[] is defined
   * above to be large enough to hold MAX(items)+1, so as to accommodate a
   * NULL terminator (set when the memory is zeroed on allocation) */
  playbin->combiner[PLAYBIN_STREAM_AUDIO].media_list[0] = "audio/";
  playbin->combiner[PLAYBIN_STREAM_AUDIO].type = GST_PLAY_SINK_TYPE_AUDIO;
  playbin->combiner[PLAYBIN_STREAM_AUDIO].channels = 0;
  playbin->combiner[PLAYBIN_STREAM_VIDEO].media_list[0] = "video/";
  playbin->combiner[PLAYBIN_STREAM_VIDEO].media_list[1] = "image/";
  playbin->combiner[PLAYBIN_STREAM_VIDEO].type = GST_PLAY_SINK_TYPE_VIDEO;
  playbin->combiner[PLAYBIN_STREAM_VIDEO].channels = 0;

  if (tsdmx_caps)
    gst_caps_unref (tsdmx_caps);

  playbin->audio_decoder = NULL;
  playbin->video_decoder = NULL;

  playbin->received_video_segment = FALSE;
  playbin->received_audio_segment = FALSE;
  playbin->pts_calibration_hdl = gst_sme_pts_calibration_create("smedvbplaybin");
  playbin->enable_vsink_sync = TRUE;
}

static void
release_combiner(GstDvbPlayBin * playbin)
{
  gint i;
  GstSourceCombine *combine = NULL;

  for (i = 0; i < PLAYBIN_STREAM_LAST; i++) {
     combine = &playbin->combiner[i];
     GST_INFO_OBJECT (playbin, "release_combiner combiner %s", combine->media_list[0]);
     if (combine->srcpad) {
       gst_object_unref (combine->srcpad);
       combine->srcpad = NULL;
     }

     if (combine->sinkpad) {
       gst_object_unref (combine->sinkpad);
       combine->sinkpad = NULL;
     }

     if (combine->combiner) {
       gst_element_set_state (combine->combiner, GST_STATE_NULL);
       gst_object_unref (combine->combiner);
       gst_bin_remove(GST_BIN(playbin),combine->combiner);
       combine->combiner = NULL;
     }

     if (combine->event) {
       gst_event_unref(combine->event);
       combine->event = NULL;
     }
  }
}

static void
remove_decoder(GstDvbPlayBin * playbin, GstElement * decoder)
{
   GstPad *dec_sinkpad;
   GstPad *mq_src_pad;

   if (decoder) {
     if((dec_sinkpad =
        gst_element_get_static_pad(decoder, "sink")) != NULL)
     {
        mq_src_pad = gst_pad_get_peer(dec_sinkpad);
        if (mq_src_pad) {
          gst_pad_unlink(mq_src_pad, dec_sinkpad);
          gst_object_unref(mq_src_pad);
        }
        gst_object_unref(dec_sinkpad);
     }
     gst_element_set_state (decoder, GST_STATE_NULL);
     gst_object_unref (decoder);
     gst_bin_remove(GST_BIN(playbin), decoder);
   }

}

static void
gst_dvbplay_bin_finalize (GObject * object)
{
  GstDvbPlayBin *playbin;

  playbin = GST_DVBPLAY_BIN (object);

  GST_INFO_OBJECT(playbin, "gst_dvbplay_bin_finalize in");

  /* Setting states to NULL is safe here because playsink
   * will already be gone and none of these sinks will be
   * a child of playsink
   */
  if (playbin->dvb_demux) {
    gst_element_set_state (playbin->dvb_demux, GST_STATE_NULL);
    gst_object_unref (playbin->dvb_demux);
  }

  if (playbin->multiqueue) {
    g_signal_handler_disconnect (G_OBJECT(playbin->multiqueue), playbin->underrun_id);
    playbin->underrun_id = 0;
    g_signal_handler_disconnect (G_OBJECT(playbin->multiqueue), playbin->overrun_id);
    playbin->overrun_id = 0;
    g_signal_handler_disconnect (G_OBJECT(playbin->multiqueue), playbin->normal_id);
    playbin->normal_id = 0;
    gst_element_set_state (playbin->multiqueue, GST_STATE_NULL);
    gst_object_unref (playbin->multiqueue);
  }

  if (playbin->video_sink) {
    gst_element_set_state (playbin->video_sink, GST_STATE_NULL);
    gst_object_unref (playbin->video_sink);
  }

  if (playbin->audio_sink) {
    gst_element_set_state (playbin->audio_sink, GST_STATE_NULL);
    gst_object_unref (playbin->audio_sink);
  }

  if (playbin->video_decoder) {
    gst_element_set_state (playbin->video_decoder, GST_STATE_NULL);
    gst_object_unref (playbin->video_decoder);
  }

  if (playbin->audio_decoder) {
    gst_element_set_state (playbin->audio_decoder, GST_STATE_NULL);
    gst_object_unref (playbin->audio_decoder);
  }
  dvbplaybin_deinit(playbin);

  release_combiner(playbin);

  if(playbin->pts_calibration_hdl){
    gst_sme_pts_calibration_destroy(&playbin->pts_calibration_hdl);
  }

  g_mutex_clear (&playbin->elements_lock);
  g_rec_mutex_clear (&playbin->lock);
  g_mutex_clear (&playbin->dyn_lock);
  g_mutex_clear (&playbin->event_lock);

  G_OBJECT_CLASS (parent_class)->finalize (object);
  GST_INFO_OBJECT(playbin, "gst_dvbplay_bin_finalize out");
}

static void
gst_dvbplay_bin_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstDvbPlayBin *playbin = GST_DVBPLAY_BIN (object);

  GST_INFO_OBJECT(playbin, "gst_dvbplay_bin_set_property, prop_id=%d in",
      prop_id);

  if (!playbin->dvb_demux || !playbin->multiqueue) {
    GST_ERROR_OBJECT(playbin, "No dvbdemux in dvbplaybin! return");
    return;
  }

  if (PROP_PRG_MEDIA_INFO == prop_id || PROP_PRG_AUDIO_INFO == prop_id)
  {
    if (GST_STATE_NULL == GST_STATE(playbin->dvb_demux)) {
      GST_WARNING_OBJECT (playbin, "tsdmx state is NULL, try to set it READY");
      if ((gst_element_set_state (playbin->dvb_demux,
                GST_STATE_READY)) == GST_STATE_CHANGE_FAILURE) {
        GST_WARNING_OBJECT (playbin, "Couldn't set %s to READY",
            GST_ELEMENT_NAME (playbin->dvb_demux));
        return;
      }
    }

     if (GST_STATE_PAUSED > GST_STATE(playbin->multiqueue)) {
      if ((gst_element_set_state (playbin->multiqueue,
                GST_STATE_PAUSED)) == GST_STATE_CHANGE_FAILURE) {
        GST_WARNING_OBJECT (playbin, "Couldn't set %s to READY",
            GST_ELEMENT_NAME (playbin->multiqueue));
        return;
      }
    }
  }


  switch (prop_id) {
    case PROP_PRG_MEDIA_INFO: {
      ST_DVBProgMediaInfo* media_info = g_value_get_pointer (value);
      g_mutex_lock(&playbin->event_lock);
      playbin->recieved_no_more_pad = FALSE;
      if(NULL != media_info){
        /* BEGIN: Added for DTS2016062400087 , 2016/6/28 */
        if(media_info->vpid < MAPI_DMX_INVALID_PID){
        /* END: Added for DTS2016062400087 , 2016/6/28 */
          playbin->have_video = TRUE;
        } else {
          GST_WARNING_OBJECT(playbin, "video pid is invalid! have_video:%d",playbin->have_video);
          /** BEGIN: Added for DTS2016070606611  2016/07/06 */
          if(playbin->video_sink){
          /** END: Added for DTS2016070606611  2016/07/06 */
            g_object_set(playbin->video_sink, "force-async-done",
              TRUE, NULL);
          }
          playbin->have_video = FALSE;
        }
        /* BEGIN: Added for DTS2016062400087 , 2016/6/28 */
        if(media_info->apid < MAPI_DMX_INVALID_PID){
        /* END: Added for DTS2016062400087 , 2016/6/28 */
          playbin->have_audio = TRUE;
        } else {
          if(playbin->audio_sink){
            g_object_set(playbin->audio_sink, "force-async-done",
              TRUE, NULL);
          }
          playbin->have_audio = FALSE;
        }
      }
      set_presync_info(playbin);

      g_mutex_unlock(&playbin->event_lock);
      if (playbin->video_sink) {
        GST_WARNING_OBJECT(playbin, "enable vsink do sync !");
        g_object_set(playbin->video_sink, "sync", TRUE, NULL);
      }
      g_object_set(playbin->dvb_demux, GST_SME_PROP_KEY_DVB_MEDIA_INFO,
          g_value_get_pointer (value),NULL);
      break;
    }
    case PROP_PRG_AUDIO_INFO: {
      ST_DVBProgAudioInfo* pstAudioInfo = g_value_get_pointer (value);
      ST_DVBProgMediaInfo* pstMediaoInfo;

      g_object_get(playbin->dvb_demux, GST_SME_PROP_KEY_DVB_MEDIA_INFO,
                   &pstMediaoInfo, NULL);

      GST_INFO("set audio info, flag:%d",g_atomic_int_get(&playbin->switching_track_flag));
      if (pstAudioInfo && pstMediaoInfo
          && pstMediaoInfo->apid != pstAudioInfo->apid
          && !g_atomic_int_get(&playbin->switching_track_flag)) {
        g_mutex_lock(&playbin->event_lock);
        gst_dvbplay_bin_combiners_event_block(playbin);
        g_mutex_unlock(&playbin->event_lock);

        g_object_set(playbin->dvb_demux, GST_SME_PROP_KEY_DVB_AUDIO_INFO,
                           g_value_get_pointer (value),NULL);
        g_atomic_int_set(&playbin->switching_track_flag, 1);
      }
      break;
    }
    case PROP_PRG_CA_INFO:{
      g_object_set(playbin->dvb_demux, GST_SME_PROP_KEY_DVB_CA_INFO,
          g_value_get_pointer (value),NULL);
      break;
    }
    case PROP_VIDEO_EVEN_KEY_INFO:{
      g_object_set(playbin->dvb_demux, GST_SME_PROP_KEY_DVB_VIDEO_EVEN_KEY,
          g_value_get_pointer (value),NULL);
      break;
    }
    case PROP_VIDEO_ODD_KEY_INFO:{
      g_object_set(playbin->dvb_demux, GST_SME_PROP_KEY_DVB_VIDEO_ODD_KEY,
          g_value_get_pointer (value),NULL);
      break;
    }
    case PROP_AUDIO_EVEN_KEY_INFO:{
      g_object_set(playbin->dvb_demux, GST_SME_PROP_KEY_DVB_AUDIO_EVEN_KEY,
          g_value_get_pointer (value),NULL);
      break;
    }
    case PROP_AUDIO_ODD_KEY_INFO:{
      g_object_set(playbin->dvb_demux, GST_SME_PROP_KEY_DVB_AUDIO_ODD_KEY,
          g_value_get_pointer (value),NULL);
      break;
    }
#ifdef ENABLE_KEY_IV
    case PROP_VIDEO_EVEN_KEY_IV:{
      g_object_set(playbin->dvb_demux, "video_even_key_iv", g_value_get_pointer (value),NULL);
      break;
    }
    case PROP_VIDEO_ODD_KEY_IV:{
      g_object_set(playbin->dvb_demux, "video_odd_key_iv", g_value_get_pointer (value),NULL);
      break;
    }
    case PROP_AUDIO_EVEN_KEY_IV:{
      g_object_set(playbin->dvb_demux, "audio_even_key_iv", g_value_get_pointer (value),NULL);
      break;
    }
    case PROP_AUDIO_ODD_KEY_IV:{
      g_object_set(playbin->dvb_demux, "audio_even_key_v", g_value_get_pointer (value),NULL);
      break;
    }
#endif
    case PROP_SWITCH_CHANNEL_MODE:{
      playbin->switch_channel_mode = g_value_get_uint(value);
      if(playbin->video_sink){
        GST_INFO_OBJECT(playbin, "set switch_channel_mode to vsink : %d",
          playbin->switch_channel_mode);
        g_object_set(playbin->video_sink, "switch-channel-mode",
          playbin->switch_channel_mode, NULL);
      }else {
        GST_WARNING_OBJECT(playbin, "No video sink, set vsink channel change mode failed");
      }

      break;
    }

#ifndef __LINUX__
    case PROP_DISP_RECT:{
      gulong ul_rect = g_value_get_ulong (value);
      GST_INFO("PROP_DISP_RECT val = %ld", ul_rect);
      playbin->vdisp_rect = *((GstSmeDispRect*)ul_rect);
      if(playbin->video_sink){
          g_object_set(playbin->video_sink,
                       GST_SME_PROP_KEY_VDISP_RECT,
                       (gint)&playbin->vdisp_rect, NULL);
          g_object_set(playbin->video_sink, GST_SME_PROP_KEY_DISP_MODE,
                      (guint)playbin->disp_mode, NULL);
      }
      break;
    }
#else
    case PROP_DISP_RECT:{
        playbin->ul_playbin2_rect = g_value_get_ulong (value);
        GST_INFO("set rect to vsink_rect:%ld, video_sink: %p",
          playbin->ul_playbin2_rect, playbin->video_sink);
        if(playbin->video_sink){
          g_object_set(playbin->video_sink, GST_SME_PROP_KEY_VDISP_RECT,
            playbin->ul_playbin2_rect, NULL);
          g_object_set(playbin->video_sink, GST_SME_PROP_KEY_DISP_MODE,
                      (guint)playbin->disp_mode, NULL);
        }
        break;
    }
#endif
    case PROP_DISP_MODE:{
        playbin->disp_mode = g_value_get_uint (value);
        GST_INFO("set disp_mode to disp_mode:%u,video_sink: %p",
          playbin->disp_mode, playbin->video_sink);
        if(playbin->video_sink){
          g_object_set(playbin->video_sink, GST_SME_PROP_KEY_DISP_MODE,
              playbin->disp_mode, NULL);
        }
        break;
    }
    case PROP_FREEZE_MODE:{
        guint freeze_mode = g_value_get_uint (value);
        GST_INFO("set freeze_mode to vsink :%u,video_sink: %p",
            freeze_mode, playbin->video_sink);
        if(playbin->video_sink){
          g_object_set(playbin->video_sink, GST_SME_PROP_KEY_VIDEO_FREEZE_MODE,
              freeze_mode, NULL);
        }
        break;
    }
    case PROP_DISP_RATIO:{
        playbin->disp_ratio = g_value_get_pointer(value);
        GST_INFO("set disp_ratio to disp_ratio:%p,video_sink: %p",
          playbin->disp_ratio, playbin->video_sink);
        if(playbin->video_sink){
          g_object_set(playbin->video_sink, GST_SME_PROP_KEY_DISP_RADIO,
            playbin->disp_ratio, NULL);
        }
        break;
    }
    case PROP_VDEC_INFO:{
        playbin->vdec_proc_info = g_value_get_pointer(value);
        GST_INFO("get proc info from video decoder vdec_proc_info:%p, video decoder:%p",
                  playbin->vdec_proc_info, playbin->video_decoder);
        if (playbin->video_decoder) {
          g_object_set(playbin->video_decoder, "omx-video-decoder-info",
          playbin->vdec_proc_info, NULL);
        }
        break;
    }
    case PROP_ADEC_INFO:{
        playbin->adec_proc_info = g_value_get_pointer(value);
        GST_INFO("get proc info from audio decoder adec_proc_info:%p, audio decoder:%p",
                  playbin->adec_proc_info, playbin->audio_decoder);
        if (playbin->audio_decoder) {
          g_object_set(playbin->audio_decoder, "gst-audio-decoder-info",
          playbin->adec_proc_info, NULL);
        }
        break;
    }
    case PROP_VSINK_INFO:{
        playbin->vsink_proc_info = g_value_get_pointer(value);
        GST_INFO("get proc info from video sink, proc info:%p,video_sink: %p",
            playbin->vsink_proc_info, playbin->video_sink);
        if (playbin->video_sink) {
          g_object_set(playbin->video_sink, GST_SME_BENCH_MARK_VSINK_INFO,
            playbin->vsink_proc_info, NULL);
        }
        break;
    }
    case PROP_ASINK_INFO:{
        playbin->asink_proc_info = g_value_get_pointer(value);
        GST_INFO("get proc info from audio sink, proc info:%p,audio_sink: %p",
            playbin->asink_proc_info, playbin->audio_sink);
        if (playbin->audio_sink) {
          g_object_set(playbin->audio_sink, GST_SME_BENCH_MARK_ASINK_INFO,
            playbin->asink_proc_info, NULL);
        }
        break;
    }
    case PROP_HWTSDMX_INFO:{
      if(playbin->dvb_demux){
        g_object_set(playbin->dvb_demux, GST_SME_BENCH_MARK_HWTSDEMUX_INFO,
          g_value_get_pointer(value), NULL);
      }else{
        GST_WARNING_OBJECT(playbin, "dvb_demux is null, why?");
      }
      break;
    }
#ifdef __LINUX__
    case PROP_AUDIO_MUTE:{
        if(playbin->audio_sink){
          guint mute = g_value_get_uint(value);
          GST_INFO_OBJECT(playbin, "set PROP_AUDIO_MUTE to asink : %u", mute);
          g_object_set(playbin->audio_sink, GST_SME_PROP_KEY_MUTE_AUDIO,
            mute, NULL);
        }else {
          GST_WARNING_OBJECT(playbin, "No audio sink, set PROP_AUDIO_MUTE failed");
        }
        break;
      }

    case PROP_AUDIO_VOLUME:{
        if(playbin->audio_sink){
          guint volume = g_value_get_uint(value);
          GST_INFO_OBJECT(playbin, "set PROP_AUDIO_VOLUME to asink : %u", volume);
          g_object_set(playbin->audio_sink, GST_SME_PROP_KEY_AUDIO_VOLUME,
            volume, NULL);
        }else {
          GST_WARNING_OBJECT(playbin, "No audio sink, set PROP_AUDIO_VOLUME failed");
        }
        break;
      }

    case PROP_AUDIO_CHANNEL_MODE:{
        if(playbin->audio_sink){
          guint mode = g_value_get_uint(value);
          GST_INFO_OBJECT(playbin, "set PROP_AUDIO_CHANNEL_MODE to asink : %u", mode);
          g_object_set(playbin->audio_sink, GST_SME_PROP_KEY_AUDIO_CHANNEL_MODE,
            mode, NULL);
        }else {
          GST_WARNING_OBJECT(playbin, "No audio sink, set PROP_AUDIO_CHANNEL_MODE failed");
        }
        break;
      }
#endif
    case PROP_VID_SYNC:{
        playbin->enable_vsink_sync = g_value_get_boolean(value);
        GST_WARNING_OBJECT(playbin, "enable_vsink_sync:%d,video_sink: %p",
          playbin->enable_vsink_sync, playbin->video_sink);
        if (playbin->video_sink) {
          g_object_set(playbin->video_sink, "sync", playbin->enable_vsink_sync,
            NULL);
        }
        break;
      break;
    }
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }

  GST_INFO_OBJECT(playbin, "gst_dvbplay_bin_set_property, prop_id=%d out",
      prop_id);
}


static void
gst_dvbplay_bin_get_property (GObject * object, guint prop_id, GValue * value,
    GParamSpec * pspec)
{
  GstDvbPlayBin *playbin = GST_DVBPLAY_BIN (object);

  switch (prop_id) {
    case PROP_DISP_RECT:{
      if (playbin->video_sink)
      {
          ST_SME_DVBDISP_RECT* prect;
          g_object_get (playbin->video_sink,
                    "gst-sme-prop-key-vdisp-rect",
                    &prect, NULL);
          g_value_set_ulong(value, (gulong)prect);
          GST_INFO("get video display rect  %p", prect);
      }
      break;
    }
    case PROP_SWITCH_CHANNEL_MODE:{
      GST_INFO("get PROP_SWITCH_CHANNEL_MODE");
      g_value_set_uint(value, playbin->switch_channel_mode);
      break;
    }
    case PROP_DISP_MODE:{
      if (playbin->video_sink)
      {
          guint emode;;
          g_object_get (playbin->video_sink,
                    "display-mode",
                    &emode, NULL);
          g_value_set_uint(value, emode);
          GST_INFO("get video display mode in dvbplaybin %u", emode);
      }
      break;
    }
    case PROP_DISP_RATIO:{
      if (playbin->video_sink)
      {
          ST_DispRatio * pratio;
          g_object_get (playbin->video_sink,
                    "disp-ratio",
                    &pratio, NULL);
          g_value_set_pointer(value, pratio);
          GST_INFO("get video display ratio  %p", pratio);
      }
      break;
    }

#ifdef __LINUX__
    case PROP_AUDIO_MUTE:{
         if(playbin->audio_sink){
            guint mute;
            g_object_get(playbin->audio_sink, GST_SME_PROP_KEY_MUTE_AUDIO,
                        &mute, NULL);
            g_value_set_uint(value, mute);
            GST_INFO_OBJECT(playbin, "get PROP_AUDIO_MUTE to asink : %u", mute);
         }else {
            GST_WARNING_OBJECT(playbin, "No audio sink, get PROP_AUDIO_MUTE failed");
         }
         break;
       }

    case PROP_AUDIO_VOLUME:{
         if(playbin->audio_sink){
           guint volume;
           g_object_get(playbin->audio_sink, GST_SME_PROP_KEY_AUDIO_VOLUME,
                        &volume, NULL);
           g_value_set_uint(value, volume);
           GST_INFO_OBJECT(playbin, "get PROP_AUDIO_VOLUME to asink : %u", volume);
         }else {
           GST_WARNING_OBJECT(playbin, "No audio sink, get PROP_AUDIO_VOLUME failed");
         }
         break;
       }

    case PROP_AUDIO_CHANNEL_MODE:{
         if(playbin->audio_sink){
           guint mode;
           g_object_get(playbin->audio_sink, GST_SME_PROP_KEY_AUDIO_CHANNEL_MODE,
                        &mode, NULL);
           g_value_set_uint(value, mode);
           GST_INFO_OBJECT(playbin, "get PROP_AUDIO_CHANNEL_MODE to asink : %u", mode);
         }else {
           GST_WARNING_OBJECT(playbin, "No audio sink, get PROP_AUDIO_CHANNEL_MODE failed");
         }
         break;
       }
#endif
    case PROP_PRG_MEDIA_INFO:{
        ST_DVBProgMediaInfo *pstMediaInfo = NULL;
        if (playbin->dvb_demux)
          g_object_get(playbin->dvb_demux, GST_SME_PROP_KEY_DVB_MEDIA_INFO,
                         &pstMediaInfo, NULL);

        if (pstMediaInfo)
            GST_INFO_OBJECT(playbin,
                "%p:get from dvbdemux:apid=%d, e_atype=%d, audio_is_ca=%d, video_is_ca=%d",
                            pstMediaInfo,
                            pstMediaInfo->apid,
                            pstMediaInfo->e_atype,
                            pstMediaInfo->audio_is_ca,
                            pstMediaInfo->video_is_ca);

        g_value_set_pointer(value, pstMediaInfo);
        break;
    }
    case PROP_HWTSDMX_INFO:{
      gpointer *pstHWTSDMX_INFO = NULL;
      if(playbin->dvb_demux){
        g_object_get(playbin->dvb_demux, GST_SME_BENCH_MARK_HWTSDEMUX_INFO,
          &pstHWTSDMX_INFO, NULL);
      }else{
        GST_WARNING_OBJECT(playbin, "dvb_demux is null, why?");
      }
      g_value_set_pointer(value, pstHWTSDMX_INFO);
      break;
    }
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}


static void
gst_dvbplay_bin_combiners_set_blocked(
GstDvbPlayBin * playbin, gboolean isdata, gboolean isblock)
{
  GstSourceCombine *combine = NULL;
  gint i;

  for (i = 0; i < PLAYBIN_STREAM_LAST; i++) {
    combine = &playbin->combiner[i];

    if (combine->srcpad) {
      if (!combine->data_block_id && isdata && isblock) {
        combine->data_block_id =
          gst_pad_add_probe (combine->srcpad, GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM,
            NULL, NULL, NULL);
      } else if (combine->data_block_id  && isdata && !isblock) {
        gst_pad_remove_probe (combine->srcpad, combine->data_block_id);
        combine->data_block_id = 0;
      } else if (!combine->event_block_id && !isdata && isblock) {
        combine->event_block_id =
          gst_pad_add_probe (combine->srcpad,
          GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM | GST_PAD_PROBE_TYPE_BLOCK,
          _combiner_event_probe, playbin, NULL);
      } else if (combine->event_block_id  && !isdata && !isblock) {
        gst_pad_remove_probe (combine->srcpad, combine->event_block_id);
        combine->event_block_id = 0;
      }
    }
  }
}


static void
gst_dvbplay_bin_reset(GstDvbPlayBin * playbin)
{
   GST_INFO_OBJECT(playbin, "gst_dvbplay_bin_reset in");

  if (playbin->video_sink) {
     gst_element_set_state (playbin->video_sink, GST_STATE_NULL);
     g_object_set(playbin->video_sink, GST_SME_PTS_CALIBRATION_HDL,
       NULL, NULL);
     gst_object_unref (playbin->video_sink);
     gst_bin_remove(GST_BIN(playbin), playbin->video_sink);
     playbin->video_sink = NULL;
  }

  if (playbin->audio_sink) {
    gst_element_set_state (playbin->audio_sink, GST_STATE_NULL);
    g_object_set(playbin->audio_sink, GST_SME_PTS_CALIBRATION_HDL,
      NULL, NULL);
    gst_object_unref (playbin->audio_sink);
    gst_bin_remove(GST_BIN(playbin), playbin->audio_sink);
    playbin->audio_sink = NULL;
  }

  release_combiner(playbin);

  if (playbin->video_decoder) {
    remove_decoder(playbin, playbin->video_decoder);
    playbin->video_decoder = NULL;
  }

  if (playbin->audio_decoder) {
    remove_decoder(playbin, playbin->audio_decoder);
    playbin->audio_decoder = NULL;
  }

   if(playbin->pts_calibration_hdl){
    gst_sme_pts_calibration_destroy(&playbin->pts_calibration_hdl);
    playbin->pts_calibration_hdl = NULL;
  }

  GST_INFO_OBJECT(playbin, "gst_dvbplay_bin_reset out");

}


static void
gst_dvbplay_bin_combiners_data_unblock (GstDvbPlayBin * playbin)
{
  GST_INFO_OBJECT(playbin, "dvbplaybin unblock data");
  gst_dvbplay_bin_combiners_set_blocked (playbin, TRUE, FALSE);
}


static void
gst_dvbplay_bin_combiners_data_block (GstDvbPlayBin * playbin)
{
  GST_INFO_OBJECT(playbin, "dvbplaybin block data");
  gst_dvbplay_bin_combiners_set_blocked (playbin, TRUE, TRUE);
}


static void
gst_dvbplay_bin_combiners_event_unblock (GstDvbPlayBin * playbin)
{
  GST_INFO_OBJECT(playbin, "dvbplaybin unblock event");
  gst_dvbplay_bin_combiners_set_blocked (playbin, FALSE, FALSE);
}


static void
gst_dvbplay_bin_combiners_event_block (GstDvbPlayBin * playbin)
{
  GST_INFO_OBJECT(playbin, "dvbplaybin block event");
  gst_dvbplay_bin_combiners_set_blocked (playbin, FALSE, TRUE);
}

static void
gst_dvbplay_bin_pending_pads_free(GstDvbPlayBin * playbin)
{
  GList *l;

  if (NULL == playbin->pending_pads)
    return;

  for (l = playbin->pending_pads; l; l = l->next) {
    GstDvbPendingPad *ppad = l->data;
    gst_pending_pad_free (ppad);
    l->data = NULL;
  }
  g_list_free (playbin->pending_pads);
  playbin->pending_pads = NULL;
}

static void
gst_pending_pad_free (GstDvbPendingPad * ppad)
{
  if (!ppad || !ppad->pad)
    return;

  if (ppad->event_probe_id != 0)
    gst_pad_remove_probe (ppad->pad, ppad->event_probe_id);
  if (ppad->notify_caps_id)
    g_signal_handler_disconnect (ppad->pad, ppad->notify_caps_id);

  gst_object_unref (ppad->pad);
  //lint -e774
  g_slice_free (GstDvbPendingPad, ppad);
  //lint +e774
}

/*
useage:
if return value != NULL, caller will free 'factories', then use the return val
*/
static GValueArray *
sort_factories(GstDvbPlayBin * playbin, GValueArray * factories)
{
  //do nothing now, so here we should return NULL.
  (void)playbin;
  (void)factories;

  return NULL;
}


static GValueArray *
get_all_factories(GstDvbPlayBin * playbin, GstCaps * caps)
{
  GValueArray *factories = NULL;
  GList* factory_list;
  GList* tmp = NULL;
  gchar* str_caps = gst_caps_to_string((const GstCaps *) caps);

  g_mutex_lock (&playbin->elements_lock);
  gst_dvbplay_bin_update_elements_list(playbin);
  factory_list = gst_element_factory_list_filter (playbin->elements, caps,
      GST_PAD_SINK, gst_caps_is_fixed (caps));
  g_mutex_unlock (&playbin->elements_lock);

  if(factory_list){
    factories = g_value_array_new (g_list_length (factory_list));
    for (tmp = factory_list; tmp; tmp = tmp->next) {
      GstElementFactory *factory = GST_ELEMENT_FACTORY_CAST (tmp->data);
      GValue val = { 0, };
      GST_INFO_OBJECT(playbin, "[factory_list]::name:%s,caps:%s",
        gst_plugin_feature_get_name(factory), str_caps);
      g_value_init (&val, G_TYPE_OBJECT);
      g_value_set_object (&val, factory);
      g_value_array_append (factories, &val);
    }
    gst_plugin_feature_list_free (factory_list);
  }

  g_free(str_caps);

  return factories;
}


static void
release_all_factories(GValueArray *factories)
{

  while (factories->n_values > 0) {
    GValue* val = g_value_array_get_nth (factories, 0);
    if(val){
      g_value_unset (val);
    }
    g_value_array_remove (factories, 0);
  }
  g_value_array_free (factories);
}

static void
mq_pad_unlinked(GstPad *pad, GstPad *peer, GstDvbPlayBin * playbin)
{
  gchar *pad_name;

  (void)peer;
  if (!playbin || !pad)
    return;

  pad_name = gst_pad_get_name (pad);
  if (pad_name) {
    GST_INFO_OBJECT (playbin, "Pad %s was (un)linked. Renegotiating", pad_name);
    g_free (pad_name);
  }

  gst_element_release_request_pad(playbin->multiqueue, pad);
}

/* require_mutliqueue_pad
 *
 * Adds a demuxer srcpad to dvbplaybin.
 *
 * Returns the srcpad of the multiqueue corresponding the given pad.
 * Returns NULL if there was an error.
 */
static GstPad *
require_mutliqueue_pad (GstDvbPlayBin * playbin, GstPad * pad)
{
  GstPad *srcpad = NULL, *sinkpad;
  GstIterator *it = NULL;
  GValue item = { 0, };

  GST_LOG_OBJECT (playbin, "require mq pad %s:%s", GST_DEBUG_PAD_NAME (pad));


  if (G_UNLIKELY (!playbin->multiqueue))
    return NULL;

  sinkpad = gst_element_get_request_pad (playbin->multiqueue, "sink_%u");
  if (!sinkpad) {
    GST_ERROR_OBJECT (playbin, "Couldn't get sinkpad from multiqueue");
    return NULL;
  }

  g_signal_connect(sinkpad, "unlinked", G_CALLBACK(mq_pad_unlinked), playbin);

  if ((gst_pad_link (pad, sinkpad) != GST_PAD_LINK_OK)) {
    GST_ERROR_OBJECT (playbin, "Couldn't link dvbdemux and multiqueue");
    goto error;
  }

  it = gst_pad_iterate_internal_links (sinkpad);

  if (!it || (gst_iterator_next (it, &item)) != GST_ITERATOR_OK
      || ((srcpad = g_value_dup_object (&item)) == NULL)) {
    GST_ERROR_OBJECT (playbin,
        "Couldn't get srcpad from multiqueue for sinkpad %" GST_PTR_FORMAT,
        sinkpad);
    goto error;
  }

beach:
  if (G_IS_VALUE (&item))
    g_value_unset (&item);
  if (it)
    gst_iterator_free (it);
  gst_object_unref (sinkpad);
  return srcpad;

error:
  gst_element_release_request_pad (playbin->multiqueue, sinkpad);
  goto beach;
}


static gboolean
activate_sink (GstDvbPlayBin * playbin, GstElement * sink)
{
  GstState state;
  GstStateChangeReturn sret;
  gboolean ret = FALSE;

  GST_OBJECT_LOCK (sink);
  state = GST_STATE (sink);
  GST_OBJECT_UNLOCK (sink);
  if (state >= GST_STATE_READY) {
    ret = TRUE;
    goto done;
  }

  sret = gst_element_set_state (sink, GST_STATE_PAUSED);
  if (sret == GST_STATE_CHANGE_FAILURE)
    goto done;

  ret = TRUE;

  gst_bin_add (GST_BIN_CAST (playbin), GST_ELEMENT_CAST (sink));

done:
  return ret;
}

static void
complete_chain (GstDvbPlayBin * playbin, GstElement * src, GstPad * pad)
{
  GstCaps *caps;
  const GstStructure *s;
  const gchar *name;
  GstPad *sinkpad;
  GstPadLinkReturn res;
  GstSourceCombine *combine = NULL;
  gint i, pass;

  (void)src;
  caps = gst_pad_get_current_caps (pad);
  if (!caps)
    caps = gst_pad_query_caps (pad, NULL);
  s = gst_caps_get_structure (caps, 0);
  name = gst_structure_get_name (s);

  GST_INFO_OBJECT (playbin,
      "pad %s:%s with caps %" GST_PTR_FORMAT " added",
      GST_DEBUG_PAD_NAME (pad), caps);

  /* major type of the pad, this determines the combiner to use,
     try exact match first */
  for (pass = 0; !combine && pass < 2; pass++) {
    for (i = 0; i < PLAYBIN_STREAM_LAST; i++) {
      if (array_has_value (playbin->combiner[i].media_list, name, pass == 0)) {
        combine = &playbin->combiner[i];
        break;
      } else if (playbin->combiner[i].get_media_caps) {
        GstCaps *media_caps = playbin->combiner[i].get_media_caps ();

        if (media_caps && gst_caps_can_intersect (media_caps, caps)) {
          combine = &playbin->combiner[i];
          gst_caps_unref (media_caps);
          break;
        }
        gst_caps_unref (media_caps);
      }
    }
  }

  /* no combiner found for the media type, don't bother linking it to a
   * combiner. This will leave the pad unlinked and thus ignored. */
  if (combine == NULL) {
    GST_DVBPLAY_BIN_SHUTDOWN_UNLOCK (playbin);
    goto unknown_type;
  }

  if (combine->combiner == NULL && playbin->have_selector) {
    /* no combiner, create one */
    GST_INFO_OBJECT (playbin, "creating new input selector");
    combine->combiner = gst_element_factory_make ("input-selector", NULL);

    if (combine->combiner == NULL) {
      /* post the missing input-selector message only once */
      playbin->have_selector = FALSE;
      gst_element_post_message (GST_ELEMENT_CAST (playbin),
          gst_missing_element_message_new (GST_ELEMENT_CAST (playbin),
              "input-selector"));
      GST_ELEMENT_WARNING (playbin, CORE, MISSING_PLUGIN,
          ("Missing element input-selector - check your GStreamer installation."),
          (NULL));
    } else {
      /* sync-mode=1, use clock */
      if (combine->type == GST_PLAY_SINK_TYPE_TEXT)
        g_object_set (combine->combiner, "sync-streams", TRUE,
            "sync-mode", 1, "cache-buffers", TRUE, NULL);
      else
      /* FIXME: set sync-mode to clock, since there is a problem when mode
       * is active-segment.
       */
        g_object_set (combine->combiner, "sync-streams", TRUE,
            "sync-mode", 1,  NULL);

      GST_INFO_OBJECT (playbin, "adding new stream combiner %p",
          combine->combiner);
      gst_element_set_state (combine->combiner, GST_STATE_PAUSED);
      gst_object_ref(combine->combiner);
      gst_bin_add (GST_BIN_CAST (playbin), combine->combiner);
    }
  }

  if (combine->srcpad == NULL) {
    if (combine->combiner) {
      /* save source pad of the combiner */
      combine->srcpad = gst_element_get_static_pad (combine->combiner, "src");
    } else {
      /* no combiner, use the pad as the source pad then */
      combine->srcpad = gst_object_ref (pad);
    }

     /* block the combiner srcpad. It's possible that multiple decodebins start
     * pushing data into the combiners before we have a chance to collect all
     * streams and connect the sinks, resulting in not-linked errors. After we
     * configured the sinks we will unblock them all. */
    GST_INFO_OBJECT (playbin, "blocking %" GST_PTR_FORMAT, combine->srcpad);
    gst_dvbplay_bin_combiners_data_block(playbin);
    gst_dvbplay_bin_combiners_event_block(playbin);
  }

  /* get sinkpad for the new stream */
  if (combine->combiner) {
      sinkpad = gst_element_get_request_pad (combine->combiner, "sink_%u");
      if (sinkpad) {
      GST_INFO_OBJECT (playbin, "got pad %s:%s from combiner",
          GST_DEBUG_PAD_NAME (sinkpad));

      /* store the combiner for the pad */
      g_object_set_data (G_OBJECT (sinkpad), "dvbplaybin.combine", combine);

      res = gst_pad_link (pad, sinkpad);
      if (GST_PAD_LINK_FAILED (res))
        goto link_failed;

      /* store combiner pad so we can release it */
      g_object_set_data (G_OBJECT (pad), "dvbplaybin.sinkpad", sinkpad);
      GST_INFO_OBJECT (playbin, "linked pad %s:%s to combiner %p",
          GST_DEBUG_PAD_NAME (pad), combine->combiner);
      gst_pad_set_active(sinkpad, TRUE);
      complete_connect_combine(combine, playbin);
      gst_object_unref(sinkpad);
    } else {
      goto request_pad_failed;
    }
  } else {
    /* no combiner, don't configure anything, we'll link the new pad directly to
     * the sink. */
    /* store the combiner for the pad */
    g_object_set_data (G_OBJECT (pad), "dvbplaybin.combine", combine);
  }

done:
  gst_caps_unref (caps);
  GST_INFO_OBJECT (playbin, "complete chain out.");
  return;

  /* ERRORS */
unknown_type:
  {
    GST_ERROR_OBJECT (playbin, "unknown type %s for pad %s:%s",
        name, GST_DEBUG_PAD_NAME (pad));
    goto done;
  }
link_failed:
  {
    GST_ERROR_OBJECT (playbin,
        "failed to link pad %s:%s to combiner, reason %d",
        GST_DEBUG_PAD_NAME (pad), res);
    goto done;
  }
request_pad_failed:
  GST_ELEMENT_ERROR (playbin, CORE, PAD,
      ("Internal playbin error."),
      ("Failed to get request pad from combiner %p.", combine->combiner));
  goto done;
}

static gboolean
connect_element (GstDvbPlayBin * playbin, GstElement *element)
{
  GList *pads;
  gboolean res = TRUE;
  gboolean dynamic = FALSE;
  GList *to_connect = NULL;

  GST_INFO_OBJECT (playbin, "Attempting to connect element %s further",
      GST_ELEMENT_NAME (element));

  /* 1. Loop over pad templates, grabbing existing pads along the way */
  for (pads = GST_ELEMENT_GET_CLASS (element)->padtemplates; pads;
      pads = g_list_next (pads)) {
    GstPadTemplate *templ = GST_PAD_TEMPLATE (pads->data);
    const gchar *templ_name;

    /* we are only interested in source pads */
    if (GST_PAD_TEMPLATE_DIRECTION (templ) != GST_PAD_SRC)
      continue;

    templ_name = GST_PAD_TEMPLATE_NAME_TEMPLATE (templ);
    GST_INFO_OBJECT (playbin, "got a source pad template: %s", templ_name);

    /* figure out what kind of pad this is */
    switch (GST_PAD_TEMPLATE_PRESENCE (templ)) {
      case GST_PAD_ALWAYS:
      {
        /* get the pad that we need to autoplug */
        GstPad *pad = gst_element_get_static_pad (element, templ_name);

        if (pad) {
          /* here is the pad, we need to autoplug it */
          to_connect = g_list_prepend (to_connect, pad);

          GST_INFO_OBJECT (playbin,
              "got the pad for always template:[%s]",templ_name);
        } else {
          /* strange, pad is marked as always but it's not
           * there. Fix the element */
          GST_WARNING_OBJECT (playbin,
              "could not get the pad for always template %s", templ_name);
        }
        break;
      }
      case GST_PAD_SOMETIMES:
      {
        /* try to get the pad to see if it is already created or
         * not */
        GstPad *pad = gst_element_get_static_pad (element, templ_name);

        if (pad) {
          GST_INFO_OBJECT (playbin, "got the pad for sometimes template %s",
              templ_name);
          /* the pad is created, we need to autoplug it */
          to_connect = g_list_prepend (to_connect, pad);
        } else {
          GST_DEBUG_OBJECT (playbin,
              "did not get the sometimes pad of template %s", templ_name);
          /* we have an element that will create dynamic pads */
          dynamic = TRUE;
        }
        break;
      }
      case GST_PAD_REQUEST:
        /* ignore request pads */
        GST_INFO_OBJECT (playbin, "ignoring request padtemplate %s", templ_name);
        break;
    }
  }

  /* 2. if there are more potential pads, connect to relevant signals */
  if (dynamic) {
    GST_INFO_OBJECT (playbin, "Adding signals to element %s ",
        GST_ELEMENT_NAME (element));
    g_signal_connect (element, "pad-added", G_CALLBACK (pad_added_cb), playbin);
    g_signal_connect (element, "pad-removed", G_CALLBACK (pad_removed_cb), playbin);
    g_signal_connect (element, "no-more-pads", G_CALLBACK (no_more_pads_cb), playbin);
  }

  /* 3. for every available pad, connect it */
  for (pads = to_connect; pads; pads = g_list_next (pads)) {
    GstPad *pad = GST_PAD_CAST (pads->data);
    GstCaps *caps;

    caps = get_pad_caps (pad);
    GST_INFO_OBJECT (playbin, "try available pad...begin analyaze_new_pad ...");
    analyze_new_pad (playbin, element, pad, caps);
    if (caps)
      gst_caps_unref (caps);

    gst_object_unref (pad);
  }
  g_list_free (to_connect);

  return res;
}

/* connect_pad:
 *
 * Try to connect the given pad to an element created from one of the factories,
 * and recursively.
 *
 * Note that dpad is ghosting pad, and so pad is linked; be sure to unset dpad's
 * target before trying to link pad.
 *
 * Returns TRUE if an element was properly created and linked
 */
static gboolean
connect_pad (GstDvbPlayBin * playbin, GstElement * src, GstPad * pad,
    GstCaps * caps, GValueArray * factories)
{
  gboolean res = FALSE;
  GstPad *mqpad = NULL;
  gboolean is_demuxer = (src == playbin->dvb_demux);      /* is dvbdemux */
  GstStateChangeReturn sret;
  GstElement** tmp_dec = NULL;

  g_return_val_if_fail (factories != NULL, FALSE);
  g_return_val_if_fail (factories->n_values > 0, FALSE);

  GST_INFO_OBJECT (playbin,
      "pad %s:%s , %d factories, caps %" GST_PTR_FORMAT,
      GST_DEBUG_PAD_NAME (pad),  factories->n_values, caps);

  /* 1.a  is element demuxer */
  if (is_demuxer) {
    GST_INFO_OBJECT (src,
        "is a demuxer, connecting the pad through multiqueue '%s'",
        GST_OBJECT_NAME (playbin->multiqueue));

    mqpad = require_mutliqueue_pad (playbin, pad);
    if (!mqpad)
      goto beach;
    src = playbin->multiqueue;
    pad = mqpad;
  }

  /* 2. Try to create an element and link to it */
  while (factories->n_values > 0) {
    GstAutoplugSelectResult ret;
    GstElementFactory *factory;
    GstElement *element;
    GstPad *sinkpad;

    /* take first factory */
    factory = g_value_get_object (g_value_array_get_nth (factories, 0));
    GST_INFO_OBJECT (src, "trying factory: %" GST_PTR_FORMAT, factory);
    /* Remove selected factory from the list. */
    g_value_array_remove (factories, 0);

    /* Check if the caps are really supported by the factory. The
     * factory list is non-empty-subset filtered while caps
     * are only accepted by a pad if they are a subset of the
     * pad caps.
     *
     * FIXME: Only do this for fixed caps here. Non-fixed caps
     * can happen if a Parser/Converter was autoplugged before
     * this. We then assume that it will be able to convert to
     * everything that the decoder would want.
     *
     * A subset check will fail here because the parser caps
     * will be generic and while the decoder will only
     * support a subset of the parser caps.
     */
    if (gst_caps_is_fixed (caps)) {
      const GList *templs;
      gboolean skip = FALSE;

      templs = gst_element_factory_get_static_pad_templates (factory);

      while (templs) {
        GstStaticPadTemplate *templ = (GstStaticPadTemplate *) templs->data;

        if (templ->direction == GST_PAD_SINK) {
          GstCaps *templcaps = gst_static_caps_get (&templ->static_caps);

          if (!gst_caps_is_subset (caps, templcaps)) {
            GST_DEBUG_OBJECT (src,
                "caps %" GST_PTR_FORMAT " not subset of %" GST_PTR_FORMAT, caps,
                templcaps);
            gst_caps_unref (templcaps);
            skip = TRUE;
            break;
          }

          gst_caps_unref (templcaps);
        }
        templs = g_list_next (templs);
      }
      if (skip) {
        gst_object_unref(factory);
        continue;
      }
    }

     /* autoplug-select to see what we should do with it. */
    GST_INFO_OBJECT (playbin, "connect_pad==>1. begin to auto select...");
    ret = autoplug_select(playbin, pad, caps, factory);

    switch (ret) {
      case GST_AUTOPLUG_SELECT_TRY:
        GST_INFO_OBJECT (playbin, "autoplug select requested try");
        break;
      case GST_AUTOPLUG_SELECT_EXPOSE:
        GST_INFO_OBJECT (playbin, "autoplug select requested expose");
        complete_chain(playbin, src, pad/*, caps*/);
        res = TRUE;
        goto beach;
      case GST_AUTOPLUG_SELECT_SKIP:
        GST_INFO_OBJECT (playbin, "autoplug select requested skip");
        gst_object_unref(factory);
        continue;
      default:
        GST_WARNING_OBJECT (playbin, "autoplug select returned unhandled %d", ret);
        break;
    }

    GST_INFO_OBJECT (playbin, "connect_pad==>2. try to create element");
    /* 2.1. Try to create an element */
    if ((element = gst_element_factory_create (factory, NULL)) == NULL) {
      GST_WARNING_OBJECT (playbin, "Could not create an element from %s",
          gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (factory)));
      gst_object_unref(factory);
      continue;
    }

    GST_INFO_OBJECT (playbin, "connect_pad==>3. add it to dvbplaybin");
    /* We don't yet want the bin to control the element's state */
    gst_element_set_locked_state (element, TRUE);
    GST_INFO_OBJECT (playbin, "add:%s to dvbplay bin",
        gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (factory)));

    /* ... add it ... */
    if (!(gst_bin_add (GST_BIN_CAST (playbin), element))) {
      GST_WARNING_OBJECT (playbin, "Couldn't add %s to the bin",
          GST_ELEMENT_NAME (element));
      gst_object_unref (element);
      gst_object_unref(factory);
      continue;
    }

    if(factory && gst_element_factory_list_is_type (factory, GST_ELEMENT_FACTORY_TYPE_DECODER |
                                               GST_ELEMENT_FACTORY_TYPE_MEDIA_VIDEO)) {
      /* BEGIN: Added for DTS2016070705204 , 2016/7/8 */
      if (playbin->video_decoder) {
        gst_element_set_state (playbin->video_decoder, GST_STATE_NULL);
        gst_object_unref (playbin->video_decoder);
        playbin->video_decoder = NULL;
      }
      /* END: Added for DTS2016070705204 , 2016/7/8 */
      playbin->video_decoder = gst_object_ref(element);
      tmp_dec = &playbin->video_decoder;
    }
    if (factory && gst_element_factory_list_is_type (factory, GST_ELEMENT_FACTORY_TYPE_DECODER |
                                                GST_ELEMENT_FACTORY_TYPE_MEDIA_AUDIO)) {
      /* BEGIN: Added for DTS2016070705204 , 2016/7/8 */
      if (playbin->audio_decoder) {
        gst_element_set_state (playbin->audio_decoder, GST_STATE_NULL);
        gst_object_unref (playbin->audio_decoder);
        playbin->audio_decoder = NULL;
      }
      /* END: Added for DTS2016070705204 , 2016/7/8 */
      playbin->audio_decoder = gst_object_ref(element);
      tmp_dec = &playbin->audio_decoder;
    }

    GST_INFO_OBJECT (playbin, "connect_pad==>4. find sink pad");
    /* Find its sink pad. */
    sinkpad = find_sink_pad (element);
    if (!sinkpad) {
      GST_WARNING_OBJECT (playbin, "Element %s doesn't have a sink pad",
          GST_ELEMENT_NAME (element));
      gst_bin_remove (GST_BIN (playbin), element);
      gst_object_unref(factory);
      if (tmp_dec && *tmp_dec) {
        gst_element_set_state (*tmp_dec, GST_STATE_NULL);
        gst_object_unref (*tmp_dec);
        *tmp_dec = NULL;
      }
      continue;
    }

    GST_INFO_OBJECT (playbin, "connect_pad==>5. link ");
    /* ... and try to link */
    if ((gst_pad_link (pad, sinkpad)) != GST_PAD_LINK_OK) {
      GST_WARNING_OBJECT (playbin, "Link failed on pad %s:%s",
          GST_DEBUG_PAD_NAME (sinkpad));
      gst_object_unref (sinkpad);
      gst_bin_remove (GST_BIN (playbin), element);
      gst_object_unref(factory);
      if (tmp_dec && *tmp_dec) {
        gst_element_set_state (*tmp_dec, GST_STATE_NULL);
        gst_object_unref (*tmp_dec);
        *tmp_dec = NULL;
      }
      continue;
    }

    GST_INFO_OBJECT (playbin, "connect_pad==>6. set it to READY ");
    /* ... activate it ... */
    if ((gst_element_set_state (element,
                GST_STATE_READY)) == GST_STATE_CHANGE_FAILURE) {
      GST_WARNING_OBJECT (playbin, "Couldn't set %s to READY",
          GST_ELEMENT_NAME (element));
      gst_object_unref (sinkpad);
      gst_bin_remove (GST_BIN (playbin), element);
      gst_object_unref(factory);
      if (tmp_dec && *tmp_dec) {
        gst_element_set_state (*tmp_dec, GST_STATE_NULL);
        gst_object_unref (*tmp_dec);
        *tmp_dec = NULL;
      }
      continue;
    }

    /* check if we still accept the caps on the pad after setting
     * the element to READY */
    if (!gst_pad_query_accept_caps (sinkpad, caps)) {
      GST_WARNING_OBJECT (playbin, "Element %s does not accept caps",
          GST_ELEMENT_NAME (element));
      gst_element_set_state (element, GST_STATE_NULL);
      gst_object_unref (sinkpad);
      gst_bin_remove (GST_BIN (playbin), element);
      gst_object_unref(factory);
      if (tmp_dec && *tmp_dec) {
        gst_element_set_state (*tmp_dec, GST_STATE_NULL);
        gst_object_unref (*tmp_dec);
        *tmp_dec = NULL;
      }
      continue;
    }

    gst_object_unref(factory);
    gst_object_unref (sinkpad);
    GST_INFO_OBJECT (playbin, "linked on pad %s:%s", GST_DEBUG_PAD_NAME (pad));

    GST_INFO_OBJECT (playbin, "connect_pad==>7. connect element further ");

    /* link this element further */
    res = connect_element (playbin, element);
    if (!res) {
        GST_WARNING_OBJECT (playbin, "connect element %s failed",
          GST_ELEMENT_NAME (element));
        continue;
    }

    /* Bring the element to the state of the parent */
    if (G_UNLIKELY(g_atomic_int_get(&playbin->switching_track_flag))) {
      GST_INFO("Set decoder state to playing");
      sret = gst_element_set_state (element, GST_STATE_PLAYING);
    }
    else {
      sret = gst_element_set_state (element, GST_STATE_PAUSED);
    }

    if (sret == GST_STATE_CHANGE_FAILURE) {
      GST_WARNING_OBJECT (playbin, "Couldn't set %s to PAUSED",
          GST_ELEMENT_NAME (element));

      continue;
    }

    /* Now let the bin handle the state */
    gst_element_set_locked_state (element, FALSE);
    res = TRUE;
    break;
  }

beach:
  if (mqpad) {
    gst_object_unref (mqpad);
  }

  return res;
}


static void
caps_notify_cb (GstPad * pad, GParamSpec * unused, GstDvbPlayBin * playbin)
{
  GstElement *element;
  GList *l;

  (void)unused;
  GST_INFO_OBJECT (pad, "Notified caps for pad %s:%s", GST_DEBUG_PAD_NAME (pad));

  /* Disconnect this; if we still need it, we'll reconnect to this in
   * analyze_new_pad */
  element = GST_ELEMENT_CAST (gst_pad_get_parent (pad));

  GST_DVBPLAY_BIN_LOCK (playbin);
  for (l = playbin->pending_pads; l; l = l->next) {
    GstDvbPendingPad *ppad = l->data;
    if (ppad && ppad->pad == pad) {
      gst_pending_pad_free (ppad);
      playbin->pending_pads = g_list_delete_link (playbin->pending_pads, l);
      break;
    }
  }
  GST_DVBPLAY_BIN_UNLOCK(playbin);

  pad_added_cb (element, pad, playbin);

  gst_object_unref (element);
}


static void
analyze_new_pad (GstDvbPlayBin * playbin, GstElement * src, GstPad * pad,
    GstCaps * caps)
{
  gboolean apcontinue = TRUE;
  GValueArray *factories = NULL, *result = NULL;
  gboolean res;
  gchar* str_caps;

  if ((caps == NULL) || gst_caps_is_empty (caps))
    goto unknown_type;

  str_caps = gst_caps_to_string((const GstCaps *) caps);
  GST_INFO_OBJECT (playbin, "Pad [%s]:%s caps:%" GST_PTR_FORMAT",str_caps=%s",
                   GST_DEBUG_PAD_NAME (pad), caps,str_caps);
  g_free(str_caps);

  if (gst_caps_is_any (caps))
    goto any_caps;

  /* 1.  'autoplug-continue' the result will tell us if this pads needs
   * further autoplugging. */
  GST_INFO_OBJECT(playbin, "1. check auto continue...");
  apcontinue = autoplug_continue(playbin, pad, caps);

  /* if autoplug-continue is FALSE */
  if (!apcontinue)
    goto complete_chain;

  GST_INFO_OBJECT(playbin, "auto continue return TRUE, go on... ");
  /*  when the caps are not fixed yet, we can't be sure what element to
   * connect. We delay autoplugging until the caps are fixed */
  if (!gst_caps_is_fixed (caps))
    goto non_fixed;

  factories = get_all_factories(playbin, caps);

  /* NULL means that we can expose the pad=== */
  if (factories == NULL)
    /** BEGIN: Added for DTS2016080202446  2016/08/02 */
    goto unknown_type;
    /** END: Added for DTS2016080202446  2016/08/02 */

  /* if the array is empty, we have a type for which we have no decoder */
  if (factories->n_values == 0) {
    /* if not we have a unhandled type with no compatible factories */
    g_value_array_free (factories);
    goto unknown_type;
  }

  /* sort some more. */
  result = sort_factories(playbin, factories);
  if (result) {
    g_value_array_free (factories);
    factories = result;
  }

  GST_INFO_OBJECT (pad, "2. Let's continue discovery on this pad, connect_pad");
  res = connect_pad (playbin, src, pad, caps, factories);

  release_all_factories(factories);

  if (!res)
    goto unknown_type;

  return;

complete_chain:
  {
    GST_INFO_OBJECT (playbin, "Pad is final. autoplug-continue:%d", apcontinue);
    complete_chain (playbin, src,  pad);
    return;
  }


unknown_type:
  {
    GST_INFO_OBJECT (pad, "Unknown type, posting message and firing signal");
    gst_element_post_message (GST_ELEMENT_CAST (playbin),
        gst_missing_decoder_message_new (GST_ELEMENT_CAST (playbin), caps));


    /* Try to expose anything */

    return;
  }
non_fixed:
  {
    GST_INFO_OBJECT (pad, "pad has non-fixed caps delay autoplugging");
    goto setup_caps_delay;
  }
any_caps:
  {
    GST_INFO_OBJECT (pad, "pad has ANY caps, delaying auto-pluggin");
    goto setup_caps_delay;
  }
setup_caps_delay:
  {
    GstDvbPendingPad *ppad;

    /* connect to caps notification */
    //GST_DVBPLAY_BIN_DYN_LOCK (playbin);
    GST_LOG_OBJECT (playbin, "dvbplaybin has now %d dynamic pads",
        g_list_length (playbin->pending_pads));
    ppad = g_slice_new0 (GstDvbPendingPad);
    if (ppad) {
      ppad->pad = gst_object_ref (pad);
      ppad->event_probe_id = gst_pad_add_probe (pad,
                                                GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM,
                                                pad_event_cb, ppad, NULL);
      playbin->pending_pads = g_list_prepend (playbin->pending_pads, ppad);
      ppad->notify_caps_id = g_signal_connect (pad,
                                               "notify::caps",
                                               G_CALLBACK (caps_notify_cb),
                                               playbin);
    }
    //GST_DVBPLAY_BIN_DYN_UNLOCK (playbin);
    return;
  }

}

static GstPadProbeReturn
pad_event_cb (GstPad * pad, GstPadProbeInfo * info, gpointer data)
{
  GstEvent *event = GST_PAD_PROBE_INFO_EVENT (info);
  GstDvbPendingPad *ppad = (GstDvbPendingPad *) data;
  GstDvbPlayBin *playbin = ppad->playbin;

  (void)pad;
  //g_assert (ppad);
  //g_assert (playbin);
  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_EOS:
      GST_DEBUG_OBJECT (playbin, "Received EOS on a non final pad, this stream "
          "ended too early");
      /*====TODO ===========*/

      break;
    default:
      break;
  }
  return GST_PAD_PROBE_OK;
}

static void
complete_connect_combine(GstSourceCombine *combine, GstDvbPlayBin * playbin)
{
  GstStateChangeReturn sret;
  GstElement * sink = NULL;    //by lrl: is initialize here ok ?
  GstPadLinkReturn res;

  /* check if the specific media type was detected and thus has a combiner
   * created for it. If there is the media type, get a sinkpad from the sink
   * and link it. We only do this if we have not yet requested the sinkpad
   * before. */
  if (combine->srcpad && combine->sinkpad == NULL) {
    GST_INFO_OBJECT (playbin, "requesting new sink pad %d", combine->type);
    if (combine->type == GST_PLAY_SINK_TYPE_AUDIO) {
      combine->sinkpad = gst_element_get_static_pad(playbin->audio_sink, "sink");
      sink = playbin->audio_sink;
    }
    else if (combine->type == GST_PLAY_SINK_TYPE_VIDEO) {
      combine->sinkpad = gst_element_get_static_pad(playbin->video_sink, "sink");
      sink = playbin->video_sink;
    }
  }  else if (combine->sinkpad && combine->srcpad == NULL) {
    GST_INFO_OBJECT (playbin, "releasing sink pad %d", combine->type);
    gst_object_unref (combine->sinkpad);
    combine->sinkpad = NULL;
  }

  if (combine->sinkpad && combine->srcpad &&
      !gst_pad_is_linked (combine->srcpad)) {
    res = gst_pad_link (combine->srcpad, combine->sinkpad);
    GST_INFO_OBJECT (playbin, "linked type %s, result: %d",
        combine->media_list[0], res);
    if (res != GST_PAD_LINK_OK) {
      GST_ELEMENT_ERROR (playbin, CORE, PAD,
          ("Internal playbin error."),
          ("Failed to link combiner to sink. Error %d", res));
    }

    sret = gst_element_set_state (sink, GST_STATE_PAUSED);
    if (sret == GST_STATE_CHANGE_FAILURE)
      GST_ERROR_OBJECT (playbin, "change sink state failed");

  }

  return;
}

/* this function is called when a new pad is added to dvbplaybin. We check the
 * type of the pad and add it to the playbin.
 */
static void
pad_added_cb (GstElement * element, GstPad * pad, GstDvbPlayBin * playbin)
{
  GstCaps *caps;
  gchar* str_caps;

  caps = get_pad_caps (pad);
  str_caps = gst_caps_to_string((const GstCaps *) caps);
  GST_INFO_OBJECT (pad, "pad added in(%p), caps:%s", pad, str_caps);
  if(!g_ascii_strncasecmp(str_caps, "audio/", (guint)strlen("audio/")) && playbin->audio_sink){
    g_object_set(playbin->audio_sink, "force-async-done", FALSE, NULL);
  }else if(!g_ascii_strncasecmp(str_caps, "video/", (guint)strlen("video/")) && playbin->video_sink){
    g_object_set(playbin->video_sink, "force-async-done", FALSE, NULL);
  }

  GST_DVBPLAY_BIN_SHUTDOWN_LOCK(playbin, shutdown);
  analyze_new_pad (playbin, element, pad, caps);
  gst_dvbplay_bin_combiners_data_block(playbin);
  gst_dvbplay_bin_combiners_event_block(playbin);

shutdown:
  GST_DVBPLAY_BIN_SHUTDOWN_UNLOCK(playbin);
  if (caps)
    gst_caps_unref (caps);

  g_free(str_caps);

  GST_INFO_OBJECT (pad, "pad added out(%p)", pad);
}

static void
pad_removed_cb (GstElement * element, GstPad * pad, GstDvbPlayBin * playbin)
{
  (void)element;
  if (!pad || !playbin)
    return;

  GList *l;

  GST_INFO_OBJECT (pad, "pad removed");

  for (l = playbin->pending_pads; l; l = l->next) {
    GstDvbPendingPad *ppad = l->data;
    GstPad *opad = ppad->pad;

    if (pad == opad) {
      gst_pending_pad_free (ppad);
      playbin->pending_pads = g_list_delete_link (playbin->pending_pads, l);
      break;
    }
  }
  return;
}

/* we get called when all pads are available and we must connect the sinks to
 * them.
 * The main purpose of the code is to see if we have video/audio and subtitles
 * and pick the right pipelines to display them.
 *
 * The combiners installed on the group tell us about the presence of
 * audio/video and subtitle streams. This allows us to see if we need
 * visualisation, video or/and audio.
 */
static void
no_more_pads_cb (GstElement * element, GstDvbPlayBin * playbin)
{
  //gint i;
  //GstPadLinkReturn res;
  //GstElement * sink;
  //GstStateChangeReturn sret;

  if (!playbin || !element || element != playbin->dvb_demux)
    return;

  GST_DVBPLAY_BIN_SHUTDOWN_LOCK(playbin, shutdown);

  gst_dvbplay_bin_combiners_data_unblock(playbin);


  if (G_UNLIKELY(g_atomic_int_get(&playbin->switching_track_flag))) {
    if (playbin->combiner[PLAYBIN_STREAM_AUDIO].combiner)
      gst_element_set_state (playbin->combiner[PLAYBIN_STREAM_AUDIO].combiner, GST_STATE_PLAYING);
    GST_INFO_OBJECT (playbin, "switching track===set audio sink to playing");
    gst_element_set_state (playbin->audio_sink, GST_STATE_PLAYING);
    g_atomic_int_set(&playbin->switching_track_flag, 0);
  }
  g_mutex_lock(&playbin->event_lock);
  if(!playbin->have_audio && playbin->audio_sink){
    g_object_set(playbin->audio_sink, "force-async-done", TRUE, NULL);
    end_presync_info(playbin);
  }
  if(!playbin->have_video && playbin->video_sink){
    g_object_set(playbin->video_sink, "force-async-done", TRUE, NULL);
    end_presync_info(playbin);
  }
  playbin->recieved_no_more_pad = TRUE;
  g_mutex_unlock(&playbin->event_lock);

  GST_INFO_OBJECT(playbin, "no_more_pads_cb normal !");
shutdown:
  GST_DVBPLAY_BIN_SHUTDOWN_UNLOCK(playbin);

  return;
}

static void
avelements_free (gpointer avelement)
{
  GstAVElement *elm = (GstAVElement *) avelement;

  if (elm->dec)
    gst_object_unref (elm->dec);
  if (elm->sink)
    gst_object_unref (elm->sink);

  //lint -e774
  g_slice_free (GstAVElement, elm);
  //lint +e774
}

//todo,by lrl: to many memleak, must check carefully !
static void
caps_changed_cb (GstElement * element, GstPad * pad,
    GstCaps *new_caps,GstDvbPlayBin * playbin)
{
  GstPad *peer;
  GstPad *mq_srcpad = NULL;
  GstIterator *it = NULL;
  GValue item = { 0, };
  GstPad *dec_srcpad;
  GstPad *dec_sinkpad;
  GstCaps *mq_src_caps = NULL;
  GstElement* dec_element = NULL;
  GstCaps* caps;
  gchar* str_caps;
  gboolean is_audio;

  (void)element;
  GST_INFO_OBJECT(playbin, "caps changed cb in... ...");
  caps = get_pad_caps (pad);
  str_caps = gst_caps_to_string((const GstCaps *) caps);
  if (str_caps)
    GST_INFO_OBJECT(playbin, "caps :%s", str_caps);
  if(!g_ascii_strncasecmp(str_caps, "audio/", (guint)strlen("audio/")) && playbin->audio_sink){
    g_object_set(playbin->audio_sink, "force-async-done", FALSE, NULL);
    is_audio = TRUE;
  }else if(!g_ascii_strncasecmp(str_caps, "video/", (guint)strlen("video/")) && playbin->video_sink){
    g_object_set(playbin->video_sink, "force-async-done", FALSE, NULL);
    is_audio = FALSE;
  }
  else{
    is_audio = FALSE;
  }

  if(caps){
    gst_caps_unref(caps);
    caps = NULL;
  }
  g_free(str_caps);

  GST_DVBPLAY_BIN_SHUTDOWN_LOCK(playbin, shutdown);
  peer = gst_pad_get_peer(pad);
  if (!peer){
    if((is_audio && playbin->audio_decoder)
      || (!is_audio && playbin->video_decoder)){
      goto not_linked;
    }
    analyze_new_pad(playbin, element, pad, new_caps);
  }else{
    it = gst_pad_iterate_internal_links (peer);
    if (!it || (gst_iterator_next (it, &item)) != GST_ITERATOR_OK
      || ((mq_srcpad = g_value_dup_object (&item)) == NULL)) {
      GST_ERROR_OBJECT (playbin,
          "Couldn't get srcpad from multiqueue for sinkpad %" GST_PTR_FORMAT,
          peer);
      gst_object_unref (peer);
      peer = NULL;

      if (G_IS_VALUE (&item)){
        g_value_unset(&item);
      }

      if(it){
        gst_iterator_free(it);
        it = NULL;
      }

      goto error;
    }
    if (G_IS_VALUE (&item)){
      g_value_unset(&item);
    }
    gst_iterator_free(it);
    it = NULL;
    gst_object_unref (peer);
    peer = NULL;

    mq_src_caps = gst_pad_get_current_caps(mq_srcpad);
    if (!new_caps && !mq_src_caps) {
      GST_WARNING_OBJECT (playbin,"Caps NULL --> NULL, no changing!");
      gst_object_unref(mq_src_caps);
      return;
    }

    GST_INFO_OBJECT(playbin, "======CAPS changed! update decoder!!=====");
    printf("======CAPS changed! update decoder!!=====\n");
    /* unlink mq and decoder */
    dec_sinkpad = gst_pad_get_peer(mq_srcpad);
    if (dec_sinkpad) {
      gst_pad_unlink(mq_srcpad, dec_sinkpad);
      dec_element = GST_PAD_PARENT(dec_sinkpad);
      gst_object_unref(dec_sinkpad);
    }

    //by lrl:here we should lock and ref dec_element if not NULL;

    /* unlink decoder and combiner*/
    if (dec_element &&
        ((dec_srcpad = gst_element_get_static_pad(dec_element, "src")) != NULL)){
      if ((peer = gst_pad_get_peer(dec_srcpad)) != NULL) {
        gint i;
        GstSourceCombine* combine_set;

        GST_INFO_OBJECT(playbin, "unlink decoder and combiner.");
        gst_element_set_state(dec_element, GST_STATE_NULL);
        gst_pad_unlink(dec_srcpad, peer);
        gst_object_unref(dec_srcpad);

        combine_set = (GstSourceCombine*)g_object_get_data(G_OBJECT (peer),
            "dvbplaybin.combine");
        for (i = 0; combine_set && i < PLAYBIN_STREAM_LAST; i++) {
          if (&playbin->combiner[i] == combine_set) {
            GST_INFO_OBJECT(playbin, "find combiner, release this pad.");
            gst_element_release_request_pad (combine_set->combiner, peer);
            break;
          }
        }
        gst_object_unref (peer);
        peer = NULL;
      }
    }

    //by lrl:here we should remove dec element.
    if(dec_element){
      GST_INFO_OBJECT(playbin, "set decoder to NULL.");
      gst_element_set_state(dec_element, GST_STATE_NULL);

      if (is_audio && playbin->audio_decoder) {
          gst_object_unref(playbin->audio_decoder);
          playbin->audio_decoder = NULL;
      }
      else if (!is_audio && playbin->video_decoder){
          gst_object_unref(playbin->video_decoder);
          playbin->video_decoder = NULL;
      }
      gst_bin_remove(GST_BIN(playbin), dec_element);
      GST_INFO_OBJECT(playbin, "remove decoder.");
    }

    if (!new_caps && mq_src_caps){
      GST_INFO_OBJECT(playbin, "New caps is NULL, complete caps changed!");
      goto exit;
    }

    analyze_new_pad(playbin, playbin->multiqueue, mq_srcpad, new_caps);
  }
shutdown:
exit:
    GST_DVBPLAY_BIN_SHUTDOWN_UNLOCK(playbin);
    if (mq_src_caps)
      gst_caps_unref(mq_src_caps);

    if (mq_srcpad)
      gst_object_unref(mq_srcpad);

    GST_INFO_OBJECT(playbin, "caps changed cb out ... ...");
    return;
error:

not_linked:
  {
    GST_INFO_OBJECT (playbin, "pad not linked");
    goto exit;
  }
}

static gint
avelement_compare_decoder (gconstpointer p1, gconstpointer p2,
    gpointer user_data)
{
  GstAVElement *v1, *v2;

  (void)user_data;
  v1 = (GstAVElement *) p1;
  v2 = (GstAVElement *) p2;

  return strcmp (GST_OBJECT_NAME (v1->dec), GST_OBJECT_NAME (v2->dec));
}

static gint
avelement_lookup_decoder (gconstpointer p1, gconstpointer p2,
    gpointer user_data)
{
  GstAVElement *v1;
  GstElementFactory *f2;

  (void)user_data;
  v1 = (GstAVElement *) p1;
  f2 = (GstElementFactory *) p2;

  return strcmp (GST_OBJECT_NAME (v1->dec), GST_OBJECT_NAME (f2));
}

static gint
avelement_compare (gconstpointer p1, gconstpointer p2)
{
  GstAVElement *v1, *v2;
  GstPluginFeature *fd1, *fd2, *fs1, *fs2;
  gint64 diff, v1_rank, v2_rank;

  v1 = (GstAVElement *) p1;
  v2 = (GstAVElement *) p2;

  fd1 = (GstPluginFeature *) v1->dec;
  fd2 = (GstPluginFeature *) v2->dec;

  /* If both have a sink, we also compare their ranks */
  if (v1->sink && v2->sink) {
    fs1 = (GstPluginFeature *) v1->sink;
    fs2 = (GstPluginFeature *) v2->sink;
    v1_rank = (gint64)gst_plugin_feature_get_rank (fd1) *
        (gint64)gst_plugin_feature_get_rank (fs1);
    v2_rank = (gint64)gst_plugin_feature_get_rank (fd2) *
        (gint64)gst_plugin_feature_get_rank (fs2);
  } else {
    v1_rank = gst_plugin_feature_get_rank (fd1);
    v2_rank = gst_plugin_feature_get_rank (fd2);
    fs1 = fs2 = NULL;
  }

  /* comparison based on the rank */
  diff = v2_rank - v1_rank;
  if (diff < 0)
    return -1;
  else if (diff > 0)
    return 1;

  /* comparison based on number of common caps features */
  diff = v2->n_comm_cf - v1->n_comm_cf;
  if (diff != 0)
    return (gint)diff;

  if (fs1 && fs2) {
    /* comparison based on the name of sink elements */
    diff = strcmp (GST_OBJECT_NAME (fs1), GST_OBJECT_NAME (fs2));
    if (diff != 0)
      return (gint)diff;
  }

  /* comparison based on the name of decoder elements */
  return strcmp (GST_OBJECT_NAME (fd1), GST_OBJECT_NAME (fd2));
}


/* unref the caps after usage */
static GstCaps *
get_template_caps (GstElementFactory * factory, GstPadDirection direction)
{
  const GList *templates;
  GstStaticPadTemplate *templ = NULL;
  GList *walk;

  templates = gst_element_factory_get_static_pad_templates (factory);
  for (walk = (GList *) templates; walk; walk = g_list_next (walk)) {
    templ = walk->data;
    if (templ->direction == direction)
      break;
  }
  if (templ)
    return gst_static_caps_get (&templ->static_caps);
  else
    return NULL;
}

static gboolean
is_included (GList * list, GstCapsFeatures * cf)
{
  for (; list; list = list->next) {
    if (gst_caps_features_is_equal ((GstCapsFeatures *) list->data, cf))
      return TRUE;
  }
  return FALSE;
}

/* compute the number of common caps features */
static guint
get_n_common_capsfeatures (GstDvbPlayBin * playbin, GstElementFactory * dec,
    GstElementFactory * sink, gboolean isaudioelement)
{
  GstCaps *d_tmpl_caps, *s_tmpl_caps;
  GstCapsFeatures *d_features, *s_features;
  GstStructure *d_struct, *s_struct;
  GList *cf_list = NULL;
  guint d_caps_size, s_caps_size;
  guint i, j, n_common_cf = 0;
  GstCaps *raw_caps =
      (isaudioelement) ? gst_static_caps_get (&raw_audio_caps) :
      gst_static_caps_get (&raw_video_caps);
  GstStructure *raw_struct = gst_caps_get_structure (raw_caps, 0);
  gboolean native_raw = isaudioelement;

  d_tmpl_caps = get_template_caps (dec, GST_PAD_SRC);
  s_tmpl_caps = get_template_caps (sink, GST_PAD_SINK);

  if (!d_tmpl_caps || !s_tmpl_caps) {
    GST_ERROR ("Failed to get template caps from decoder or sink");
    return 0;
  }

  char *d_tmpl_caps_str;
  char *s_tmpl_caps_str;

  d_tmpl_caps_str = gst_caps_to_string(d_tmpl_caps);
  s_tmpl_caps_str = gst_caps_to_string(s_tmpl_caps);

  GST_INFO_OBJECT (playbin, "checking decode factory [%s], sink factory %s",
      GST_OBJECT_NAME (dec), GST_OBJECT_NAME (sink));
  GST_INFO_OBJECT(playbin, "decode caps: %s \n, sink caps: %s",
      d_tmpl_caps_str, s_tmpl_caps_str);

  g_free(d_tmpl_caps_str);
  g_free(s_tmpl_caps_str);

  d_caps_size = gst_caps_get_size (d_tmpl_caps);
  s_caps_size = gst_caps_get_size (s_tmpl_caps);

  for (i = 0; i < d_caps_size; i++) {
    d_features = gst_caps_get_features ((const GstCaps *) d_tmpl_caps, i);
    d_struct = gst_caps_get_structure ((const GstCaps *) d_tmpl_caps, i);
    for (j = 0; j < s_caps_size; j++) {

      s_features = gst_caps_get_features ((const GstCaps *) s_tmpl_caps, j);
      s_struct = gst_caps_get_structure ((const GstCaps *) s_tmpl_caps, j);

      /* A common caps feature is given if the caps features are equal
       * and the structures can intersect. If the NATIVE_AUDIO/NATIVE_VIDEO
       * flags are not set we also allow if both structures are raw caps with
       * system memory caps features, because in that case we have converters in
       * place.
       */
      if (gst_caps_features_is_equal (d_features, s_features) &&
          (gst_structure_can_intersect (d_struct, s_struct) ||
              (!native_raw
                  && gst_caps_features_is_equal (d_features,
                      GST_CAPS_FEATURES_MEMORY_SYSTEM_MEMORY)
                  && gst_structure_can_intersect (raw_struct, d_struct)
                  && gst_structure_can_intersect (raw_struct, s_struct)))
          && !is_included (cf_list, s_features)) {
        cf_list = g_list_prepend (cf_list, s_features);
        n_common_cf++;
      }
    }
  }
  if (cf_list)
    g_list_free (cf_list);

  gst_caps_unref (d_tmpl_caps);
  gst_caps_unref (s_tmpl_caps);

  GST_INFO_OBJECT (playbin, "%s:%s====>n_common_cf=%d",
      GST_OBJECT_NAME (dec), GST_OBJECT_NAME (sink), n_common_cf);

  return n_common_cf;
}

static GSequence *
avelements_create (GstDvbPlayBin * playbin, gboolean isaudioelement)
{
  GstElementFactory *d_factory, *s_factory;
  GList *dec_list, *sink_list, *dl, *sl;
  GSequence *ave_seq;
  GstAVElement *ave;
  guint n_common_cf = 0;

  if (isaudioelement) {
    sink_list = gst_element_factory_list_get_elements
        (GST_ELEMENT_FACTORY_TYPE_SINK |
        GST_ELEMENT_FACTORY_TYPE_MEDIA_AUDIO, GST_RANK_MARGINAL);
    dec_list =
        gst_element_factory_list_get_elements (GST_ELEMENT_FACTORY_TYPE_DECODER
        | GST_ELEMENT_FACTORY_TYPE_MEDIA_AUDIO, GST_RANK_MARGINAL);
  } else {
    sink_list = gst_element_factory_list_get_elements
        (GST_ELEMENT_FACTORY_TYPE_SINK |
        GST_ELEMENT_FACTORY_TYPE_MEDIA_VIDEO |
        GST_ELEMENT_FACTORY_TYPE_MEDIA_IMAGE, GST_RANK_MARGINAL);

    dec_list =
        gst_element_factory_list_get_elements (GST_ELEMENT_FACTORY_TYPE_DECODER
        | GST_ELEMENT_FACTORY_TYPE_MEDIA_VIDEO |
        GST_ELEMENT_FACTORY_TYPE_MEDIA_IMAGE, GST_RANK_MARGINAL);
  }

  /* create a list of audio/video elements. Each element in the list
   * is holding an audio/video decoder and an auido/video sink in which
   * the decoders srcpad template caps and sink element's sinkpad template
   * caps are compatible */
  dl = dec_list;
  sl = sink_list;

  ave_seq = g_sequence_new ((GDestroyNotify) avelements_free);

  for (; dl; dl = dl->next) {
    d_factory = (GstElementFactory *) dl->data;
    for (; sl; sl = sl->next) {
      s_factory = (GstElementFactory *) sl->data;
      n_common_cf =
          get_n_common_capsfeatures (playbin, d_factory, s_factory,
                                     isaudioelement);
      if (n_common_cf < 1)
        continue;

      ave = g_slice_new (GstAVElement);
      if (!ave) {
        GST_ERROR_OBJECT(playbin, "g_slice_new failed");
        goto error;
      }
      if (E_SME_MEM_OK !=
          VOS_Memset_S(ave, sizeof(GstAVElement),
                       0, sizeof(GstAVElement))) {
        GST_WARNING_OBJECT(playbin, "SECURE: VOS_Memset_S return failed.");
        goto error;
      }

      ave->dec = gst_object_ref (d_factory);
      g_sequence_append (ave_seq, ave);
      ave->sink = gst_object_ref (s_factory);
      ave->n_comm_cf = n_common_cf;
    }
    sl = sink_list;
  }
  g_sequence_sort (ave_seq, (GCompareDataFunc) avelement_compare_decoder, NULL);

error:
  gst_plugin_feature_list_free (dec_list);
  gst_plugin_feature_list_free (sink_list);

  return ave_seq;
}

static gboolean
avelement_iter_is_equal (GSequenceIter * iter, GstElementFactory * factory)
{
  GstAVElement *ave;

  if (!iter)
    return FALSE;

  ave = g_sequence_get (iter);
  if (!ave)
    return FALSE;

  return strcmp (GST_OBJECT_NAME (ave->dec), GST_OBJECT_NAME (factory)) == 0;
}



static gboolean
setup_dvbdemux (GstCaps * caps, GstDvbPlayBin* playbin)
{
  GList *factory_list;
  GstElement *element= NULL;
  GstPad *sinkpad;
  gboolean res = FALSE;
  GList *tmp = NULL;
  guint i = 0;

  if (!caps) {
    return FALSE;
  }

  /* filter out the elements based on the caps. */
  g_mutex_lock (&playbin->elements_lock);
  factory_list = gst_element_factory_list_filter (playbin->elements, caps,
      GST_PAD_SINK, gst_caps_is_fixed (caps));
  g_mutex_unlock (&playbin->elements_lock);

  GST_INFO_OBJECT (playbin, "found factories %p", factory_list);
  GST_PLUGIN_FEATURE_LIST_DEBUG (factory_list);

  if (factory_list == NULL) {
    GST_WARNING_OBJECT(playbin, "Not found dvbdemux!");
    goto not_found_dvbdemux;
  }

  /* sort it according to rank*/
  factory_list = g_list_sort (factory_list, compare_factories_func);
  for(i = 0; i < g_list_length(factory_list); i++){
    GstElementFactory *factory;
    tmp = g_list_nth(factory_list, i);
    if(NULL == tmp){
      break;
    }

    factory = GST_ELEMENT_FACTORY_CAST (tmp->data);

    /* Try to create an element */
    if ((element = gst_element_factory_create (factory, NULL)) == NULL) {
      GST_WARNING_OBJECT (playbin, "Could not create an element from %s",
          gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (factory)));
      continue;
    }

    /* ... add it ... */
    if (!(gst_bin_add (GST_BIN_CAST (playbin), element))) {
        GST_WARNING_OBJECT (playbin, "Couldn't add %s to the bin",
            GST_ELEMENT_NAME (element));
        gst_object_unref (element);
        continue;
    }

    /* Find its sink pad. */
    sinkpad = find_sink_pad (element);
    if (!sinkpad) {
      GST_WARNING_OBJECT (playbin, "Element %s doesn't have a sink pad",
          GST_ELEMENT_NAME (element));
      gst_object_unref (element);
      gst_bin_remove (GST_BIN (playbin), element);
      continue;
    }

    /* ... activate it ... */
    if ((gst_element_set_state (element,
              GST_STATE_READY)) == GST_STATE_CHANGE_FAILURE) {
      GST_WARNING_OBJECT (playbin, "Couldn't set %s to READY",
          GST_ELEMENT_NAME (element));
      gst_object_unref (sinkpad);
      gst_object_unref (element);
      gst_bin_remove (GST_BIN (playbin), element);
      continue;
    }

    gst_object_unref(sinkpad);
    res = TRUE;
    break;
  }

  if (element && res) {
    g_signal_connect (element, "pad-added", G_CALLBACK (pad_added_cb), playbin);
    g_signal_connect (element, "pad-removed", G_CALLBACK (pad_removed_cb), playbin);
    g_signal_connect (element, "no-more-pads", G_CALLBACK (no_more_pads_cb), playbin);
    g_signal_connect (element, "caps-changed", G_CALLBACK (caps_changed_cb), playbin);
  }

  gst_plugin_feature_list_free (factory_list);
  playbin->dvb_demux = gst_object_ref(element);

not_found_dvbdemux:
  return res;
}

static gboolean
sink_accepts_caps (GstDvbPlayBin * playbin, GstElement * sink, GstCaps * caps)
{
  GstPad *sinkpad;

  (void)playbin;
  sinkpad = gst_element_get_static_pad (sink, "sink");
  if (sinkpad) {
    /* Got the sink pad, now let's see if the element actually does accept the
     * caps that we have */
    if (!gst_pad_query_accept_caps (sinkpad, caps)) {
      gst_object_unref (sinkpad);
      return FALSE;
    }
    gst_object_unref (sinkpad);
  }

  return TRUE;
}

//by lrl:for autoplug_xxxx: multi-thread support?
static GstAutoplugSelectResult
autoplug_select (GstDvbPlayBin * playbin, GstPad * pad,
    GstCaps * caps, GstElementFactory * factory)
{
  GstElement *element;
  const gchar *klass;
  GstElement **sinkp;
  GList *ave_list = NULL, *l;
  GstAVElement *ave = NULL;
  GSequence *ave_seq = NULL;
  GSequenceIter *seq_iter;

  (void)pad;
  GST_INFO_OBJECT (playbin, "checking factory :[%s]", GST_OBJECT_NAME (factory));

  /* if it's not a sink, we make sure the element is compatible with
   * the fixed sink */
  if (!gst_element_factory_list_is_type (factory,
          GST_ELEMENT_FACTORY_TYPE_SINK)) {
    gboolean isvideodec = gst_element_factory_list_is_type (factory,
        GST_ELEMENT_FACTORY_TYPE_DECODER |
        GST_ELEMENT_FACTORY_TYPE_MEDIA_VIDEO |
        GST_ELEMENT_FACTORY_TYPE_MEDIA_IMAGE);
    gboolean isaudiodec = gst_element_factory_list_is_type (factory,
        GST_ELEMENT_FACTORY_TYPE_DECODER |
        GST_ELEMENT_FACTORY_TYPE_MEDIA_AUDIO);


    if (!isvideodec && !isaudiodec){
      GST_INFO_OBJECT(playbin, "not a vdec & not a adec, try it !");
      return GST_AUTOPLUG_SELECT_TRY;
    }

    g_mutex_lock (&playbin->elements_lock);

    if (isaudiodec) {
      ave_seq = playbin->aelements;
      sinkp = &playbin->audio_sink;
    } else {
      ave_seq = playbin->velements;
      sinkp = &playbin->video_sink;
    }

    seq_iter =
        g_sequence_lookup (ave_seq, factory,
        (GCompareDataFunc) avelement_lookup_decoder, NULL);
    if (seq_iter) {
      /* Go to first iter with that decoder */
      do {
        GSequenceIter *tmp_seq_iter;

        tmp_seq_iter = g_sequence_iter_prev (seq_iter);
        if (!avelement_iter_is_equal (tmp_seq_iter, factory))
          break;
        seq_iter = tmp_seq_iter;
      } while (!g_sequence_iter_is_begin (seq_iter));

      while (!g_sequence_iter_is_end (seq_iter)
          && avelement_iter_is_equal (seq_iter, factory)) {
        ave = g_sequence_get (seq_iter);
        if(ave){
          GST_INFO_OBJECT (playbin,
              "fac:[%s],ave->dec:[%s],ave->sink:[%s]",
              gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (factory)),
              ave->dec?gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (ave->dec)):"no_dec",
              ave->sink?gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (ave->sink)):"no_sink");
        } else {
          GST_INFO_OBJECT(playbin, "ave is null, why ?");
        }
        ave_list = g_list_prepend (ave_list, ave);
        seq_iter = g_sequence_iter_next (seq_iter);
      }

      /* Sort all GstAVElements by their relative ranks and insert
       * into the decoders list */
      ave_list = g_list_sort (ave_list, (GCompareFunc) avelement_compare);
    }

    if(NULL == ave_list){
      g_mutex_unlock (&playbin->elements_lock);
      GST_INFO_OBJECT(playbin, "not find in avelements, why ?");
      return GST_AUTOPLUG_SELECT_TRY;
    }

    /* if it is a decoder and we don't have a fixed sink, then find out
     * the matching audio/video sink from GstAVElements list */
    for (l = ave_list; l; l = l->next) {
      gboolean created_sink = FALSE;

      ave = (GstAVElement *) l->data;
      if (((isaudiodec && !playbin->audio_sink) ||
           (isvideodec && !playbin->video_sink))) {
        if (ave && ave->sink) {
          GST_INFO_OBJECT (playbin,
              "Trying to create sink '%s' for decoder '%s'",
              gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (ave->sink)),
              gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (factory)));
          if ((*sinkp = gst_element_factory_create (ave->sink, NULL)) == NULL) {
            GST_WARNING_OBJECT (playbin,
                "Could not create an element from %s",
                gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (ave->sink)));
            continue;
          } else {
            if (isvideodec) {
#ifndef __LINUX__
              if(NULL != playbin->vdisp_surface){
                g_object_set(*sinkp,
                              GST_SME_PROP_KEY_VDISP_RECT,
                              (gulong)playbin->vdisp_surface, NULL);
                }else{
                  g_object_set(*sinkp,
                              GST_SME_PROP_KEY_VDISP_RECT,
                              (gulong)&playbin->vdisp_rect, NULL);
                  g_object_set(*sinkp, GST_SME_PROP_KEY_DISP_MODE,
                              (guint)playbin->disp_mode, NULL);
                  g_object_set(playbin->video_sink, GST_SME_PROP_KEY_DISP_RADIO,
                              playbin->disp_ratio, NULL);
                }
#else
              g_object_set(*sinkp,
                          GST_SME_PROP_KEY_VDISP_RECT,
                          (gulong)playbin->ul_playbin2_rect, NULL);
              g_object_set(*sinkp, GST_SME_PROP_KEY_DISP_MODE,
                          (guint)playbin->disp_mode, NULL);
              g_object_set(playbin->video_sink, GST_SME_PROP_KEY_DISP_RADIO,
                           playbin->disp_ratio, NULL);
#endif
            }
#ifdef USE_SME_AUDIO_CLOCK
            if(sinkp == &playbin->audio_sink){
              GST_WARNING_OBJECT(playbin, "%s:enable audio clock", AVSYNC_BY_AUDIO_TAG);
              g_object_set(playbin->audio_sink, GST_SME_PROP_KEY_AUDIO_CLOCK,
                TRUE, NULL);
            } else if(sinkp == &playbin->video_sink){
              GST_WARNING_OBJECT(playbin, "vsink sync:%d", playbin->enable_vsink_sync);
              g_object_set(playbin->video_sink, "sync", playbin->enable_vsink_sync,
                NULL);
            }
            if(playbin->pts_calibration_hdl){
              g_object_set(*sinkp, GST_SME_PTS_CALIBRATION_HDL,
                playbin->pts_calibration_hdl, NULL);
            }
#endif
            GST_WARNING_OBJECT(playbin, "pre-sync:%s:set presync_info:slow_presync:%d,"
              "wait-presync-max-time:%lld, slow-presync-rate:%f",
              (sinkp == &playbin->audio_sink) ? "asink" : "vsink",
              SLOW_PRESYNC_MODE_DEFAULT, WAIT_PRESYNC_MODE_MAX_TIME,
              SLOW_PRESYNC_RATE);
            g_object_set(*sinkp,
              GST_SME_PROP_KEY_SLOW_PRESYNC, SLOW_PRESYNC_MODE_DEFAULT,
              GST_SME_PROP_KEY_WAIT_PRESYNC_MAXTIME, WAIT_PRESYNC_MODE_MAX_TIME,
              GST_SME_PROP_KEY_SLOW_PRESYNC_RATE, SLOW_PRESYNC_RATE, NULL);

            if (!activate_sink (playbin, *sinkp)) {
              gst_object_unref (*sinkp);
              *sinkp = NULL;
              GST_WARNING_OBJECT (playbin,
                  "Could not activate sink %s",
                  gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (ave->sink)));
              continue;
            }
            gst_object_ref(*sinkp);
            created_sink = TRUE;
          }
        }
      }

      /* If it is a decoder and we have a fixed sink for the media
       * type it outputs, check that the decoder is compatible with this sink */
      if ((isaudiodec && playbin->audio_sink) ||
          (isvideodec && playbin->video_sink)) {
        gboolean compatible = FALSE;
        GstPad *sinkpad;
        GstCaps *tmp_caps;
        GstElement *sink;
        sink = *sinkp;

        sinkpad = gst_element_get_static_pad (sink, "sink");
        if (sinkpad) {
         GST_INFO_OBJECT (playbin, "check sink sinkpad compatible...");
          tmp_caps = gst_pad_query_caps (sinkpad, NULL);
          compatible = gst_element_factory_can_src_any_caps (factory, tmp_caps);
          GST_INFO_OBJECT (playbin, "check result compatible:%d", compatible);
          gst_object_unref (sinkpad);
          gst_caps_unref (tmp_caps);
        }

        if (compatible)
          break;

        GST_INFO_OBJECT (playbin, "%s not compatible with the fixed sink",
            GST_OBJECT_NAME (factory));

        /* If it is not compatible, either continue with the next possible
         * sink or if we have a fixed sink, skip the decoder */
        if (created_sink) {
          gst_element_set_state (*sinkp, GST_STATE_NULL);
          gst_object_unref (*sinkp);
          *sinkp = NULL;
        } else {
          g_list_free (ave_list);
          g_mutex_unlock (&playbin->elements_lock);
          return GST_AUTOPLUG_SELECT_SKIP;
        }
      }
    }

    g_list_free (ave_list);
    g_mutex_unlock (&playbin->elements_lock);

    if(isaudiodec && playbin->audio_sink){
      GST_INFO_OBJECT (playbin, "asink is ready return GST_AUTOPLUG_SELECT_TRY");
    } else {
      GST_INFO_OBJECT (playbin, "asink not ready return GST_AUTOPLUG_SELECT_TRY");
    }

    if(isvideodec && playbin->video_sink){
      GST_INFO_OBJECT (playbin, "vsink is ready return GST_AUTOPLUG_SELECT_TRY");
    } else {
      GST_INFO_OBJECT (playbin, "vsink not ready return GST_AUTOPLUG_SELECT_TRY");
    }

    return GST_AUTOPLUG_SELECT_TRY;
  }

  /* it's a sink, see if an instance of it actually works */
  GST_INFO_OBJECT (playbin, "we found a sink '%s'", GST_OBJECT_NAME (factory));

  klass =
      gst_element_factory_get_metadata (factory, GST_ELEMENT_METADATA_KLASS);

  /* figure out the klass */
  if (strstr (klass, "Audio")) {
    GST_INFO_OBJECT (playbin, "we found an audio sink");
    sinkp = &playbin->audio_sink;
  } else if (strstr (klass, "Video")) {
    GST_INFO_OBJECT (playbin, "we found a video sink");
    sinkp = &playbin->video_sink;
  } else {
    /* unknown klass, skip this element */
    GST_WARNING_OBJECT (playbin, "unknown sink klass %s found", klass);
    return GST_AUTOPLUG_SELECT_SKIP;
  }

/* now see if we already have a sink element */
  if (*sinkp) {
    GstElement *sink = gst_object_ref (*sinkp);

    if (sink_accepts_caps (playbin, sink, caps)) {
      GST_INFO_OBJECT (playbin,
          "Existing sink '%s' accepts caps: %" GST_PTR_FORMAT,
          GST_ELEMENT_NAME (sink), caps);
      gst_object_unref (sink);
      return GST_AUTOPLUG_SELECT_EXPOSE;
    } else {
      GST_INFO_OBJECT (playbin,
          "Existing sink '%s' does not accept caps: %" GST_PTR_FORMAT,
          GST_ELEMENT_NAME (sink), caps);
      gst_object_unref (sink);
      return GST_AUTOPLUG_SELECT_SKIP;
    }
  }
  GST_INFO_OBJECT (playbin, "we have no pending sink, try to create '%s'",
      gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (factory)));

  if ((*sinkp = gst_element_factory_create (factory, NULL)) == NULL) {
    GST_WARNING_OBJECT (playbin, "Could not create an element from %s",
        gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (factory)));
    return GST_AUTOPLUG_SELECT_SKIP;
  }

#ifdef USE_SME_AUDIO_CLOCK
  if(sinkp == &playbin->audio_sink){
    GST_WARNING_OBJECT(playbin, "%s:enable audio clock", AVSYNC_BY_AUDIO_TAG);
    g_object_set(playbin->audio_sink, GST_SME_PROP_KEY_AUDIO_CLOCK,
      TRUE, NULL);
  }else if(sinkp == &playbin->video_sink){
    GST_WARNING_OBJECT(playbin, "vsink sync:%d", playbin->enable_vsink_sync);
    g_object_set(playbin->video_sink, "sync", playbin->enable_vsink_sync,
      NULL);
  }
  if(playbin->pts_calibration_hdl){
    g_object_set(*sinkp, GST_SME_PTS_CALIBRATION_HDL,
      playbin->pts_calibration_hdl, NULL);
  }
#endif

  GST_WARNING_OBJECT(playbin, "pre-sync:%s:set presync_info:slow_presync:%d,"
    "wait-presync-max-time:%lld, slow-presync-rate:%f",
    (sinkp == &playbin->audio_sink) ? "asink" : "vsink",
    SLOW_PRESYNC_MODE_DEFAULT, WAIT_PRESYNC_MODE_MAX_TIME,
    SLOW_PRESYNC_RATE);
  g_object_set(*sinkp,
    GST_SME_PROP_KEY_SLOW_PRESYNC, SLOW_PRESYNC_MODE_DEFAULT,
    GST_SME_PROP_KEY_WAIT_PRESYNC_MAXTIME, WAIT_PRESYNC_MODE_MAX_TIME,
    GST_SME_PROP_KEY_SLOW_PRESYNC_RATE, SLOW_PRESYNC_RATE, NULL);

  element = *sinkp;


  if (!activate_sink (playbin, element)) {
    GST_WARNING_OBJECT (playbin, "Could not activate sink %s",
        gst_plugin_feature_get_name (GST_PLUGIN_FEATURE (factory)));
    *sinkp = NULL;
    gst_object_unref (element);
    GST_INFO_OBJECT (playbin, "activate sink failed return GST_AUTOPLUG_SELECT_SKIP");
    return GST_AUTOPLUG_SELECT_SKIP;
  }

  /*
   *
   * store the sink in the playbin */
  GST_INFO_OBJECT (playbin, "remember sink");
  gst_object_ref (element);

  /* Check if the selected sink actually supports the
   * caps and can be set to READY*/
  if (!sink_accepts_caps (playbin, element, caps)) {
    *sinkp = NULL;
    gst_bin_remove (GST_BIN_CAST (playbin), element);
    gst_element_set_state (element, GST_STATE_NULL);
    gst_object_unref (element);
    GST_INFO_OBJECT (playbin, "not accepts return GST_AUTOPLUG_SELECT_SKIP");
    return GST_AUTOPLUG_SELECT_SKIP;
  }
  GST_INFO_OBJECT (playbin, "we found a working sink, expose pad");
  return GST_AUTOPLUG_SELECT_EXPOSE;
}

static gboolean
autoplug_continue (GstDvbPlayBin * playbin, GstPad * pad, GstCaps * caps)
{
  gboolean ret = TRUE;
  GstElement *sink;
  GstPad *sinkpad = NULL;

  sink = playbin->audio_sink;
  if (sink) {
    GST_INFO_OBJECT(playbin, "dvbplaybin already has audio sink");
    sinkpad = gst_element_get_static_pad (sink, "sink");
    if (sinkpad) {
      GstCaps *sinkcaps;

      sinkcaps = gst_pad_query_caps (sinkpad, NULL);
      if (!gst_caps_is_any (sinkcaps))
        ret = !gst_pad_query_accept_caps (sinkpad, caps);
      gst_caps_unref (sinkcaps);
      gst_object_unref (sinkpad);
    }
  }

  if (!ret) {
    GST_INFO_OBJECT(playbin, "audio sink accept caps,auto_continue FALSE, goto done");
    goto done;
  }


  sink = playbin->video_sink;
  if (sink) {
    GST_INFO_OBJECT(playbin, "dvbplaybin already has video sink");
    sinkpad = gst_element_get_static_pad (sink, "sink");
    if (sinkpad) {
      GstCaps *sinkcaps;

      sinkcaps = gst_pad_query_caps (sinkpad, NULL);
      if (!gst_caps_is_any (sinkcaps))
        ret = !gst_pad_query_accept_caps (sinkpad, caps);
      gst_caps_unref (sinkcaps);
      gst_object_unref (sinkpad);
    }
  }

done:

  GST_INFO_OBJECT (playbin,
      "continue autoplugging dvbplaybin %p for %s:%s, %" GST_PTR_FORMAT ": %d",
      playbin, GST_DEBUG_PAD_NAME (pad), caps, ret);

  return ret;
}

static GstStateChangeReturn
gst_dvbplay_bin_change_state (GstElement * element, GstStateChange transition)
{
  GstStateChangeReturn ret;
  GstDvbPlayBin *playbin;

  playbin = GST_DVBPLAY_BIN (element);

  GST_INFO_OBJECT(playbin, "gst_dvbplay_bin_change_state in:%s to %s,playbin=%p",
      gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
      gst_element_state_get_name((GstState)(transition & 0x07)), playbin);

  switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
      break;
    case GST_STATE_CHANGE_READY_TO_PAUSED:
      playbin->recieved_no_more_pad = FALSE;
      playbin->enable_vsink_sync = TRUE;
      if (!playbin->pts_calibration_hdl)
          playbin->pts_calibration_hdl = gst_sme_pts_calibration_create("smedvbplaybin");

      if(playbin->pts_calibration_hdl){
        gst_sme_pts_calibration_clear(playbin->pts_calibration_hdl);
      }
      g_atomic_int_set (&playbin->shutdown, 0);
      gst_dvbplay_bin_pending_pads_free (playbin);
      break;
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      GST_INFO_OBJECT (playbin, "setting shutdown flag");
      g_atomic_int_set(&playbin->switching_track_flag, 0);
      g_atomic_int_set (&playbin->shutdown, 1);

      /* wait for all callbacks to end by taking the lock.
       * No dynamic (critical) new callbacks will
       * be able to happen as we set the shutdown flag. */
      GST_DVBPLAY_BIN_DYN_LOCK (playbin);
      GST_INFO_OBJECT (playbin, "dynamic lock taken, we can continue shutdown");
      GST_DVBPLAY_BIN_DYN_UNLOCK (playbin);

      break;
    case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
      if(!playbin->have_audio){
        GST_WARNING_OBJECT(playbin, "no audio, sleep to smooth video play.");
        g_usleep(ONLY_VIDEO_TO_PLAYING_TIME);
      }
      break;
    default:
      break;
  }

  GST_INFO_OBJECT (playbin, "gst_dvbplay_bin_change_state:%s to %s,begin...",
      gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
      gst_element_state_get_name((GstState)(transition & 0x07)));


  ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

  GST_INFO_OBJECT (playbin, "gst_dvbplay_bin_change_state:%s to %s,end,ret=%s",
      gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
      gst_element_state_get_name((GstState)(transition & 0x07)),
      gst_element_state_change_return_get_name(ret));

  if (ret == GST_STATE_CHANGE_FAILURE)
    goto failure;

  switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_PLAYING:{
      GstClock* pipeline_clock = GST_ELEMENT_CLOCK(playbin);
      if(pipeline_clock){
        GST_WARNING_OBJECT(playbin, "set clock max-backward:%lld", DVB_CLOCK_MAX_BACKWARD);
        g_object_set(pipeline_clock, "max-backward", DVB_CLOCK_MAX_BACKWARD, NULL);
      }
      break;
    }
    case GST_STATE_CHANGE_READY_TO_PAUSED:
      ret = GST_STATE_CHANGE_ASYNC;
      break;
    case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
      break;
    case GST_STATE_CHANGE_PAUSED_TO_READY: {
      GstClock* gst_cc = GST_ELEMENT_CLOCK(playbin);

      gst_dvbplay_bin_pending_pads_free (playbin);
      playbin->received_video_segment = FALSE;
      playbin->received_audio_segment = FALSE;
      gst_dvbplay_bin_reset(playbin);
      g_atomic_int_set (&playbin->shutdown, 0);

      if(gst_cc){
        GST_WARNING_OBJECT(playbin, "clear clock last time begin...");
        gst_clock_clear_last_time(gst_cc);
        GST_WARNING_OBJECT(playbin, "clear clock last time end");
      }else{
        GST_WARNING_OBJECT(playbin, "clock is null !");
      }
      break;
    }
    case GST_STATE_CHANGE_READY_TO_NULL:
    {
      break;
    }
    default:
      break;
  }

  goto done;

failure:
  ret = GST_STATE_CHANGE_FAILURE;

  GST_INFO_OBJECT(playbin, "gst_dvbplay_bin_change_state out:%s to %s,ret=%s",
      gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
      gst_element_state_get_name((GstState)(transition & 0x07)),
      gst_element_state_change_return_get_name(ret));

  goto done;

done:
  return ret;

}

static gboolean
gst_dvbplay_bin_send_event (GstElement * element, GstEvent * event)
{
  GstDvbPlayBin *playbin = (GstDvbPlayBin *) (element);
  gboolean res = TRUE;
  GstElementClass* tsdmxclass = NULL;

  if (playbin->dvb_demux)
    tsdmxclass = GST_ELEMENT_GET_CLASS(GST_ELEMENT(playbin->dvb_demux));

  GST_INFO("dvbplaybin event in(event:%s)", GST_EVENT_TYPE_NAME(event));

  gst_event_ref(event);
  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_FLUSH_START://no break;
    case GST_EVENT_FLUSH_STOP:
      if(tsdmxclass){
        res = tsdmxclass->send_event(playbin->dvb_demux, event);
      } else {
        gst_event_unref(event);
        res = FALSE;
      }

      g_mutex_lock(&playbin->event_lock);
      if(playbin->combiner[PLAYBIN_STREAM_AUDIO].event){
        gst_event_unref(playbin->combiner[PLAYBIN_STREAM_AUDIO].event);
        playbin->combiner[PLAYBIN_STREAM_AUDIO].event = NULL;
      }
      playbin->received_video_segment = FALSE;
      playbin->received_audio_segment = FALSE;
      gst_dvbplay_bin_pending_pads_free(playbin);
      if(GST_EVENT_FLUSH_STOP == GST_EVENT_TYPE (event)){
        GstClock* gst_cc = GST_ELEMENT_CLOCK(playbin);

        gst_dvbplay_bin_combiners_data_block(playbin);
        gst_dvbplay_bin_combiners_event_block(playbin);
        if(playbin->pts_calibration_hdl){
          gst_sme_pts_calibration_clear(playbin->pts_calibration_hdl);
        }
        if(gst_cc){
          GST_WARNING_OBJECT(playbin, "clear clock last time begin...");
          gst_clock_clear_last_time(gst_cc);
          GST_WARNING_OBJECT(playbin, "clear clock last time end");
        }else{
          GST_WARNING_OBJECT(playbin, "clock is null !");
        }
      }
      g_mutex_unlock(&playbin->event_lock);
      break;
    default:
      res = GST_ELEMENT_CLASS(parent_class)->send_event(element, event);
      break;
  }

  GST_INFO("dvbplaybin event out (event:%s) res:%d",
    GST_EVENT_TYPE_NAME(event), res);
  gst_event_unref(event);
  return res;
}

gboolean
gst_dvbplay_bin_plugin_init (GstPlugin * plugin)
{
  gboolean bret;
  GST_DEBUG_CATEGORY_INIT (gst_dvbplay_bin_debug, "dvbplaybin", 0, "dvb play bin");

  GST_INFO_OBJECT(plugin, "gst_dvbplay_bin_plugin_init in,plugin=%p", plugin);

  bret = gst_element_register (plugin, "smedvbplaybin", GST_RANK_SECONDARY,
      GST_TYPE_DVBPLAY_BIN);

  GST_INFO_OBJECT(plugin, "gst_dvbplay_bin_plugin_init out");

  return bret;
}

GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    smedvbplaybin,
    "hw dvbplaybin",
    gst_dvbplay_bin_plugin_init,
    "1.0",
    "LGPL",
    "hw GStreamer Plugin",
    "http://huawei.com/"
);
