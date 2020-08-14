/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

 ******************************************************************************
  文 件 名   : gstsme.h
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2014年3月25日
  最近修改   :
  功能描述   : sme公共头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2014年3月25日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/

#ifndef __GST_SME_H__
#define __GST_SME_H__

#include <gst/gst.h>
#include "sme_media_type.h"

/*****************************************
 * Struct Definition
 ****************************************/
typedef enum
{
   GST_SME_OVERRUN,
   GST_SME_UNDERRUN,
   GST_SME_NORMAL,
   GST_SME_LAST
} GstSmeQueueStatus;

//GST_DEBUG_CATEGORY_EXTERN(sme_debug);
//#define GST_CAT_DEFAULT sme_debug

G_BEGIN_DECLS

extern gboolean gst_smehttpsrc_register (GstPlugin * plugin);
extern gboolean gst_smedemux_register (GstPlugin * plugin);
extern gboolean gst_smeaudiodecoder_register (GstPlugin * plugin);
extern gboolean gst_smevideodec_register (GstPlugin * plugin);
extern gboolean gst_smeaudiosink_register (GstPlugin * plugin);
extern gboolean gst_smevideosink_register (GstPlugin * plugin);
extern gboolean gst_smeplaybin_register (GstPlugin * plugin);
extern gboolean gst_smehttpsrc_register (GstPlugin * plugin);
extern gboolean gst_smebtvlivesrc_register(GstPlugin * plugin);
extern gboolean gst_smevideosink_himpi_register (GstPlugin * plugin);

G_END_DECLS

#define SME_URL_RDONLY 1  /**< read-only */
#define SME_URL_WRONLY 2  /**< write-only */
#define SME_URL_RDWR   (SME_URL_RDONLY|SME_URL_WRONLY)  /**< read-write */
#define SME_AV_SEEK_SIZE 0x10000

#define GST_SME_URL_STREAMHEADER 16

#ifndef SMEPLAYER_STAT_TAG_TIME
#define SMEPLAYER_STAT_TAG_TIME "PlayerTimeLineTrace"
#endif

#define GST_SAFE_STRING(str) ((NULL == (str)) ? "null str" : (str))
#define AVSYNC_BY_AUDIO_TAG "avsync_by_audio:"
//#define AVSYNC_BY_AUDIO_QOS_UNDERRUN

#define GST_SME_PROP_KEY_URI                "gst-sme-prop-key-uri"
#define GST_SME_PROP_KEY_VDISP_SF           "gst-sme-prop-key-vdisp-surface"
#define GST_SME_PROP_KEY_VDISP_RECT         "gst-sme-prop-key-vdisp-rect"
#define GST_SME_PROP_KEY_BUFFER_FULL        "gst-sme-prop-key-buffer-full"
#define GST_SME_PROP_KEY_BUFFER_STATUS      "gst-sme-prop-key-buffer-status"
#define GST_SME_PROP_KEY_CUR_BW             "gst-sme-prop-key-current-bandwidth"
#define GST_SME_PROP_KEY_STREAM_BPS         "gst-sme-prop-key-stream-bps"
#define GST_SME_PROP_KEY_BUFFERED_AUDIO     "gst-sme-prop-key-buffered-audio"
#define GST_SME_PROP_KEY_BUFFERED_VIDEO     "gst-sme-prop-key-buffered-video"
#define GST_SME_PROP_KEY_VIDEO_WIDTH        "gst-sme-prop-key-video-width"
#define GST_SME_PROP_KEY_VIDEO_HEIGHT       "gst-sme-prop-key-video-height"
#define GST_SEM_PROP_KEY_TIMEOUT            "timeout"
#define GST_SME_PROP_KEY_BENCHMARK          "gst-sme-prop-key-benchmark"
#define GST_SME_PROP_KEY_HIVPIPE_HDL        "gst-sme-prop-key-hivpipe-hdl"
#define GST_SME_PROP_KEY_DECOUT_INTERVAL    "gst-sme-prop-key-hivpipe-dec-interval"
#define GST_SME_PROP_KEY_DECIN_DURATION     "gst-sme-prop-key-hivpipe-decin-duration"
#define GST_KEY_RING_BUFFER_MAXSIZE         "ring-buffer-max-size" //与queue2中保持一致
#define GST_KEY_MAX_LEVEL_MAXSIZE           "max-size-bytes"       //与queue2中保持一致
#define GST_SME_PROP_KEY_EXIT_BLOCK         "gst-sme-prop-key-exit-block"
#define GST_SME_PROP_KEY_CACHE_PROC         "gst-sme-prop-key-cache-proc"
#define GST_SME_PROP_KEY_MUTE_AUDIO         "gst-sme-prop-key-mute-audio"
#define GST_SME_PROP_KEY_AUDIO_VOLUME       "gst-sme-prop-key-audio-volume"
#define GST_SME_PROP_KEY_AUDIO_CHANNEL_MODE "gst-sme-prop-key-audio-channel-mode"
#define GST_SME_PROP_KEY_AUDIO_CLOCK        "gst-sme-prop-key-audio-clock"
//#define GST_SME_PROP_KEY_ADAPTIVE_STREAM_BT "gst-sme-prop-key-adaptive-stream-bitrate"
#define GST_SME_PROP_KEY_ADAPTIVE_STREAM_BT "connection-speed"
#define GST_SME_PROP_KEY_AUDIO_SINK         "audio-sink"
#define GST_SME_PROP_KEY_VIDEO_SINK         "video-sink"
#define GST_SME_PROP_KEY_AUDIO_HDMI_MODE    "audio-hdmi-mode"
#define GST_SME_PROP_KEY_DISP_MODE          "display-mode"
#define GST_SME_PROP_KEY_DISP_RADIO         "disp-ratio"
#define GST_SME_PROP_KEY_SWITCH_CHANNEL_MODE  "switch-channel-mode"
#define GST_SME_STRAM_TAG_ADEC              "audio_dec:"
#define GST_SME_STRAM_TAG_VDEC              "video_dec:"
#define GST_SME_PROP_KEY_USER_AGENT         "user-agent"
#define GST_SME_PROP_KEY_BYTE_RANGE         "byte-range"
#define GST_SME_PROP_KEY_COOKIES            "cookies"
#define GST_SME_PROP_KEY_REFERER            "referer"
#define GST_SME_BENCH_MARK_DEMUX_INFO       "sme-demux-info"
#define GST_SME_BENCH_MARK_HWTSDEMUX_INFO   "sme-hwtsdemux-info"
#define GST_SME_BENCH_MARK_VDEC_INFO        "video-decoder-info"
#define GST_SME_BENCH_MARK_ADEC_INFO        "audio-decoder-info"
#define GST_SME_BENCH_MARK_VSINK_INFO       "vsink-proc-info"
#define GST_SME_BENCH_MARK_ASINK_INFO       "asink-proc-info"
#define GST_SME_PROP_KEY_VIDEO_FREEZE_MODE  "freeze-mode"

/*
**value type:guint
**value means:0:disable;1:enable;2(other):end of slow pre-sync
*/
#define GST_SME_PROP_KEY_SLOW_PRESYNC  "slow-pre-sync"

/*
**value type:GstClockTime
**value means:GST_TIME_CLOCK_NONE:wait infinitely. unit is ns.
*/
#define GST_SME_PROP_KEY_WAIT_PRESYNC_MAXTIME  "wait-presync-max-time"

/*
**value type:gdouble
**value means:slow paly rate to presync. the range is between (0,1).
*/
#define GST_SME_PROP_KEY_SLOW_PRESYNC_RATE  "slow-presync-rate"

/*
**value type:gboolean
**value means:FALSE:stop wait-presync;TRUE:start wait-presync.
*/
#define GST_SME_PROP_KEY_START_WAIT_PRESYNC  "start-wait-presync"

/*
**value type:gboolean
**value means:FALSE:disable video to do sync;TRUE:enable video to do sync.
*/
#define GST_SME_PROP_KEY_VID_SYNC  "video-do-sync"

/****************************begin for dvb****************************************/
//参数ST_DVBProgMediaInfo*
#define GST_SME_PROP_KEY_DVB_MEDIA_INFO "gst-sme-prop-key-dvb-media-info"
//参数ST_DVBProgAudioInfo*
#define GST_SME_PROP_KEY_DVB_AUDIO_INFO "gst-sme-prop-key-dvb-audio-info"
//参数ST_DVBProgCAInfo*
#define GST_SME_PROP_KEY_DVB_CA_INFO "gst-sme-prop-key-dvb-ca-info"
//参数ST_CAKeyParam*
#define GST_SME_PROP_KEY_DVB_AUDIO_ODD_KEY "gst-sme-prop-key-dvb-audio-odd-key-info"
//参数ST_CAKeyParam*
#define GST_SME_PROP_KEY_DVB_AUDIO_EVEN_KEY "gst-sme-prop-key-dvb-audio-even-key-info"
//参数ST_CAKeyParam*
#define GST_SME_PROP_KEY_DVB_VIDEO_ODD_KEY "gst-sme-prop-key-dvb-video-odd-key-info"
//参数ST_CAKeyParam*
#define GST_SME_PROP_KEY_DVB_VIDEO_EVEN_KEY "gst-sme-prop-key-dvb-video-even-key-info"
//参数ST_CAIVParam*
#define GST_SME_PROP_KEY_DVB_AUDIO_ODD_KEY_IV "gst-sme-prop-key-dvb-audio-odd-key-iv"
//参数ST_CAIVParam*
#define GST_SME_PROP_KEY_DVB_AUDIO_EVEN_KEY_IV "gst-sme-prop-key-dvb-audio-even-key-iv"
//参数ST_CAIVParam*
#define GST_SME_PROP_KEY_DVB_VIDEO_ODD_KEY_IV "gst-sme-prop-key-dvb-video-odd-key-iv"
//参数ST_CAIVParam*
#define GST_SME_PROP_KEY_DVB_VIDEO_EVEN_KEY_IV "gst-sme-prop-key-dvb-video-even-key-iv"
#define GST_SME_PTS_CALIBRATION_HDL "gst-sme-prop-pts-calibration-hdl"
/******************************end for dvb*****************************************/

#endif /* __GST_SME_H__ */
