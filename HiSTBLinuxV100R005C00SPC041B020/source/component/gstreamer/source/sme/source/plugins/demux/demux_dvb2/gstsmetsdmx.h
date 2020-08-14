/******************************************************************************
<hisi Tec.>
Copyright (c) 2016 - 2020 hisi Software, Inc. All rights reserved.
All software, firmware and related documentation herein ("hisi Tec.") are
intellectual property of hisi Tec. and protected by
law, including, but not limited to, copyright law and international treaties.
Any use, modification, reproduction, retransmission, or republication of all
or part of hisi Tec. is expressly prohibited, unless prior written
permission has been granted by hisi Tec.
*******************************************************************************/


#ifndef __GST_SMETSDMX_H__
#define __GST_SMETSDMX_H__

#include <gst/gst.h>
#include "tvos_hal_dmx.h"
#include "gst/base/gstadapter.h"
#include "sme_dvbplayer_type.h"
#include "sme_proc.h"

G_BEGIN_DECLS

/* #defines don't like whitespacey bits */
#define GST_TYPE_SMETSDMX \
    (gst_smetsdmx_get_type())

#define GST_SMETSDMX(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_SMETSDMX, GstSmeTSDMX))

#define GST_SMETSDMX_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_SMETSDMX,GstSmeTSDMXClass))

#define GST_IS_SMETSDMX(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SMETSDMX))

#define GST_IS_SMETSDMX_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SMETSDMX))
;
typedef struct _GstSmeTSDMX      GstSmeTSDMX;
typedef struct _GstSmeTSDMXClass GstSmeTSDMXClass;

#define TSDMX_DUMP_ES_STREAM
#ifdef TSDMX_DUMP_ES_STREAM
#include <stdio.h>
#define TSDMX_ES_STREAM_DIR "/data/dump"
#define TSDMX_ES_STREAM_A_PREFIX "audio_"
#define TSDMX_ES_STREAM_V_PREFIX "video_"
#endif

typedef struct
{
    guchar u8_iv_valid;
    guchar u8_key_valid;

    guchar au8_IV[TVOS_MAX_KEY_LENGTH];
    guchar au8_ek1[TVOS_MAX_KEY_LENGTH];
    guchar au8_ek2[TVOS_MAX_KEY_LENGTH];
    //in case    u8_xxxx_ca_type==1(clear CW), this array saves cw instead of ecw
    guchar au8_ecw[TVOS_MAX_KEY_LENGTH];
} GstCAKeyInfoItem;

typedef struct
{
    guint video_descramble_id;
    guint audio_descramble_id;

    GstCAKeyInfoItem video_evenKey;
    GstCAKeyInfoItem video_oddKey;
    GstCAKeyInfoItem audio_evenKey;
    GstCAKeyInfoItem audio_oddKey;
} GstCAKeyInfo;

typedef enum
{
    E_TSDMX_THD_STATE_IDLE = 0,       //stop last hal(filter & descramble) & wait
    E_TSDMX_THD_STATE_INIT_HAL,       //start new.
    E_TSDMX_THD_STATE_UPDATE_CA_KEY,  //update key.
    E_TSDMX_THD_STATE_PROCCESS_AV,    //get esbuffer from hal & push buffer downstream.
    E_TSDMX_THD_STATE_TO_IDLE_THEN_INIT,//need to first goto idle then to init.
} GstTsdmxThdStateEm;

typedef struct _GstSmeTSDMXHal
{
#ifdef LINUX_OS
    DEMUX_DEVICE_S* hal_handle; //NULL is invalid.
#else
    DEMUS_DEVICE_S* hal_handle; //NULL is invalid.
#endif
    U32 video_filter_id;        //-1 is invalid.
    U32 audio_filter_id;        //-1 is invalid.
    U32 pcr_filter_id;          //-1 is invalid.
} GstSmeTSDMXHal;

struct _GstSmeTSDMX
{
    GstElement parent_element;
    GstPad* sinkpad;
    GstPad* video_srcpad;
    GstPad* audio_srcpad;
    GstSegment video_segment;
    GstSegment audio_segment;
#ifdef ENABLE_PCR_CLOCK
    GstClock* provided_clock;  /* clock */
#endif
    ST_DVBProgMediaInfo prog_media_info; // info need to set
    ST_DVBProgMediaInfo applied_prog_media_info;//info already report
    ST_DVBProgCAInfo prog_ca_info;
    GstCAKeyInfo prog_ca_key_info;
    pthread_t video_proc_thread_id;
    GstTsdmxThdStateEm evideo_proc_thread_state;
    pthread_t audio_proc_thread_id;
    GstTsdmxThdStateEm eaudio_proc_thread_state;
    GstSmeTSDMXHal tsdmxhal;
    gboolean bexit;
    gboolean bvideo_need_new_segment;
    gboolean baudio_need_new_segment;
    GMutex video_proc_mutex;//property & change state VS video thread
    GCond video_proc_cond;
    GMutex audio_proc_mutex;//property & change state VS audio thread
    GCond audio_proc_cond;
    GMutex av_proc_mutex;//video thread VS audio thread
    GstClockTime v_first_pts;//first valid video pes pkg pts
    GstClockTime a_first_pts;//first valid audio pes pkg pts
#ifdef TSDMX_DUMP_ES_STREAM
  FILE* esfile_v;
  FILE* esfile_a;
#endif
    gboolean bchanging_audio_track;//doing switching audio track flag
    gboolean baudio_flushing;
    ST_SMEHWTSDEMUX_PROC_INFO stProcInfo;
    GstClockTime v_last_no_str_t;
    GstClockTime v_cur_no_str_t;
    GstClockTime a_last_no_str_t;
    GstClockTime a_cur_no_str_t;
    gboolean have_no_audio;
    gboolean have_no_video;
    gboolean post_no_streamer;
    gboolean audio_need_wait_validpts;//when switch audiotrack
};

struct _GstSmeTSDMXClass
{
    GstElementClass parent_class;

    /*< private >*/
    /* signal callbacks */
    void (*pad_caps_changed)(GstElement* element, GstPad* pad, GstCaps* new_caps);
};


GType gst_smetsdmx_get_type (void);

G_END_DECLS

#endif /* __GST_SMETSDMX_H__ */
