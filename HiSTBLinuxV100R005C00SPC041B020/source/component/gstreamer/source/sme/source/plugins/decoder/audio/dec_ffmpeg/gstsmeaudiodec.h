/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmevideosink.h
 * @brief   sme audio decoder head file declares
 * @author  zhangzhen(z00175754)
 * @date    2014/4/14
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/4/14
 * Author : zhangzhen(z00175754)
 * Desc   : Created file
 *
******************************************************************************/

#ifndef __GST_SME_AUDIO_DEC_H__
#define __GST_SME_AUDIO_DEC_H__

#include "gstsme.h"
#include "sme_adec_inf.h"
#include "sme_proc.h"


G_BEGIN_DECLS



typedef struct _GstSmeTSInfo GstSmeTSInfo;
typedef struct _GstSmeAudioDecoder GstSmeAudioDecoder;
typedef struct _GstSmeAudioDecoderClass GstSmeAudioDecoderClass;

#define MAX_TS_MASK 0xff

struct _GstSmeTSInfo
{
    gint idx;
    GstClockTime timestamp;
    GstClockTime duration;
    gint64 offset;
};

struct _GstSmeAudioDecoder
{
    GstElement parent;
    GstPad *srcpad;
    GstPad *sinkpad;
    gboolean opened;
    ST_SME_AUDIOINFO audioinfo;
    const ST_SME_AUDIO_DEC_INF *smeaudiodecinf;
    SMECODECHDL semcodechdl;
    GstSegment segment;
    GstClockTime next_out;
    GstSmeTSInfo ts_info[MAX_TS_MASK + 1];
    gint ts_idx;
    gdouble proportion;
    GstClockTime earliest_time;
    GstCaps *last_caps;
    GstPadMode esink_pad_mode;
    gboolean bis_smebuffer;
    gboolean bsend_segment_downstream;
    gint dec_err_count;
#ifdef SMEPLAYER_BENCHMARK_OPEN
    FILE        *dump_sink_file;
    gboolean    dump_sink_flag;
    char        dump_sink_file_name[128];
    FILE        *dump_src_file;
    gboolean    dump_src_flag;
    char        dump_src_file_name[128];
    ST_ADEC_SHOW_INFO  audio_decoder_info;
#endif
};

struct _GstSmeAudioDecoderClass
{
  GstElementClass parent_class;

  SMECODECFMT codecfmt;

  GstPadTemplate *srctempl, *sinktempl;
  gboolean turnoff_parser;      /* used for turning off aac raw parsing*/
};

G_END_DECLS

#endif //__GST_SME_AUDIO_DEC_H__
