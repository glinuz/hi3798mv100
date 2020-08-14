/* gstsmeaudiosink2.h */

#ifndef __GST_SME_AUDIO_SINK_H__
#define __GST_SME_AUDIO_SINK_H__

#include <gst/gst.h>
#include <gst/audio/audio.h>
#include <sme_media_type.h>
#include <sys/time.h>
#include "sme_proc.h"
#include "gstsmeptscalibration.h"

G_BEGIN_DECLS

#define GST_TYPE_SME_AUDIOSINK2 (gst_smeaudiosink2_get_type())
#define GST_SME_AUDIOSINK2(obj)                                         \
    (G_TYPE_CHECK_INSTANCE_CAST(                                        \
        (obj),GST_TYPE_SME_AUDIOSINK2, GstSmeAudiosink2))
#define GST_SME_AUDIOSINK2_CLASS(klass)                                 \
    (G_TYPE_CHECK_CLASS_CAST(                                           \
        (klass),GST_TYPE_SME_AUDIOSINK2, GstSmeAudiosink2Class))
#define GST_SME_AUDIOSINK2_GET_CLASS(obj)                               \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                                  \
                                GST_TYPE_SME_AUDIOSINK2,                \
                                GstSmeAudiosink2Class))
#define GST_IS_SME_AUDIOSINK2(obj)                          \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SME_AUDIOSINK2))
#define GST_IS_SME_AUDIOSINK2_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SME_AUDIOSINK2))
#define GST_SME_AUDIOSINK2_CAST(obj)       ((GstSmeAudiosink2 *) (obj))

typedef struct _GstSmeAudiosink2 GstSmeAudiosink2;
typedef struct _GstSmeAudiosink2Class GstSmeAudiosink2Class;

typedef V_VOID (*pfnPullAudio)(IN V_VOID* pvCtx,
                               INOUT V_UINT8* pu8Buf,
                               INOUT V_UINT32* pu32BufSize);
typedef enum
{
    E_SME_AUDIO_PLAYOUT_RET_SUCCESS = ICS_SUCCESS,//成功
    E_SME_AUDIO_PLAYOUT_RET_UNSUPPORT,//不支持
    E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG,//非法参数
    E_SME_AUDIO_PLAYOUT_RET_NOMEM,//分配内存错误
    E_SME_AUDIO_PLAYOUT_RET_INVALID_OPERATION,//非法操作
    E_SME_AUDIO_PLAYOUT_RET_FATAL,//致命错误，不可恢复
} E_SME_AUDIO_PLAYOUT_RET;


//#define GST_DELAY_SINK_GET_LOCK(obj)              \
//    (&GST_SME_AUDIOSINK2_CAST (obj)->delay_lock)
//#define GST_DELAY_SINK_LOCK(obj)                  \
//    (g_mutex_lock (GST_DELAY_SINK_GET_LOCK (obj)))
//#define GST_DELAY_SINK_UNLOCK(obj)                    \
//    (g_mutex_unlock (GST_DELAY_SINK_GET_LOCK (obj)))

/**
 * GstSmeAudiosink2:
 *
 * sme audio sink base element
 */
struct _GstSmeAudiosink2
{
    GstBaseSink element;
    GstClockTime start_time;
    GstClockTime duration;
    GstClock* provided_clock;  /* clock */

    ST_SME_AUDIOINFO audio_info;
    ST_SME_LOG_CB audio_log_ctx;
    E_SME_AUDIO_RAW_FMT audio_format;

    guint period_time;
    gboolean recieve_eos;

    gboolean need_sync;
    gint sync_count;
    gint flushing_flag;

    gint first_write_flag;
    guint8 *write_cache;
    guint   write_cache_len;
    GstClockTime first_cache_pts;
    GstClockTime current_cache_pts;
    gint64  cache_audio_dur_time;

    guint mute;
    guint volume;
    A_TRACK_CHANNEL_MODE_E mode;
    GstSmePTSCalibrateHDL pts_calibration_hdl;

    GMutex lock;

#ifdef SMEPLAYER_BENCHMARK_OPEN
    FILE        *dump_sink_file;
    gboolean    dump_sink_flag;
    char        dump_sink_file_name[128];
    ST_ASINK_SHOW_INFO audio_sink_info;
#endif
};

struct _GstSmeAudiosink2Class
{
    GstBaseSinkClass parent_class;

    /* virtual methods of gstsmeaudiosink2 */
    gint (*create)(GstSmeAudiosink2 *sink);
    gint (*destroy)(GstSmeAudiosink2 *sink);
    gint (*set)(GstSmeAudiosink2 *sink,
                gint stream_type,
                gint sample_rate,
                gint channels,
                gint bytes_per_sample,
                pfnPullAudio fun_callback,
                void *context);
    gint (*start)(GstSmeAudiosink2 *sink);
    gint (*stop)(GstSmeAudiosink2 *sink);
    gint (*pause)(GstSmeAudiosink2 *sink);
    gint (*write)(GstSmeAudiosink2 *sink, const gchar *data, gint length);
    gint (*flush)(GstSmeAudiosink2 *sink);
    gint (*set_volume)(GstSmeAudiosink2 *sink, guint volume);
    gint (*get_volume)(GstSmeAudiosink2 *sink, guint *volume);
    gint (*set_mute)(GstSmeAudiosink2 *sink, guint mute);
    gint (*get_mute)(GstSmeAudiosink2 *sink, guint *mute);
    gint (*set_channel_mode)(GstSmeAudiosink2 *sink, A_TRACK_CHANNEL_MODE_E mode);
    gint (*get_channel_mode)(GstSmeAudiosink2 *sink, A_TRACK_CHANNEL_MODE_E *mode);
    gint (*set_hdmi_mode)(GstSmeAudiosink2 *sink, gchar *hdmi_mode);
    gint (*get_hdmi_mode)(GstSmeAudiosink2 *sink, gchar **hdmi_mode);
    gint (*get_delay)(GstSmeAudiosink2 *sink, GstClockTime *delay);
#ifdef __DDP_PT__
    gboolean (*is_ddp_sink)(GstSmeAudiosink2 *sink);
#endif
};

GType gst_smeaudiosink2_get_type (void);

G_END_DECLS

#endif /* endof __GST_SME_AUDIO_SINK_H__*/
