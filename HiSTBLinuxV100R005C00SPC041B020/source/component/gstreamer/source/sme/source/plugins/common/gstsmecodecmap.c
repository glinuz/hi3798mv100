/**************************************************************************//**

  Copyright (C), 2001-2014, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file          -gstsmecodecmap.c
 * @brief          -sme codec map to gstreamer
 * @author
 * @date          -2014/3/27
 * @version   -V1.00
 *
 * History:
 *
 * Number  : V1.00
 * Date      : 2014/3/27
 * Author    :
 * Desc      : Created file
 *
******************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdlib.h>
#include <string.h>
#include <gst/gst.h>
#include <gst/pbutils/pbutils.h>
#include "osal_mem.h"
#include "osal_str.h"

//#include "sme_media_type.h"
#include "gstsmecodecmap.h"

//lint -e585 (可变参数...报错，为gstreamer中的日志宏，无法修改。)
//lint -e40   //va_list不识别
//lint -e10   //va_list不识别导致报变量定义缺少";"
//lint -e522  //va_list不识别导致
//lint -e717  //do{}while(0)
//lint -e838  //前一次赋值未使用，一般用于变量定义时赋空
//lint -esym(530, var_args)
//lint -e730  //函数的布尔类型参数。
//lint -e708  //union initialization

/* this macro makes a caps width fixed or unfixed width/height
 * properties depending on whether we've got a context.
 *
 * See below for why we use this.
 *
 * We should actually do this stuff at the end, like in riff-media.c,
 * but I'm too lazy today. Maybe later.
 */
GstCaps *
gst_sme_vid_caps_new (ST_SME_MEDIAINFO * context, E_SME_MEDIA_CODEC_ID codec_id,
    const char *mimetype, const char *fieldname, ...)
{
    GstStructure *structure = NULL;
    GstCaps *caps = NULL;
    va_list var_args;
    guint idx = 0;

    PST_SME_VIDEOINFO pstVideoInfo = (PST_SME_VIDEOINFO)context;

    GST_INFO ("context:%p, codec_id:%d, mimetype:%s", context, codec_id, mimetype);

    /* fixed, non probing context */
    if (pstVideoInfo != NULL && pstVideoInfo->stVideoRes.i32Width > 0){
        gint num, denom;

        caps = gst_caps_new_simple (mimetype,
            "width", G_TYPE_INT, pstVideoInfo->stVideoRes.i32Width,
            "height", G_TYPE_INT, pstVideoInfo->stVideoRes.i32Height, NULL);

        //???
        num = pstVideoInfo->i32FpsNum;
        denom = pstVideoInfo->i32FpsDen;
        if (!denom) {
            GST_LOG ("invalid framerate: %d/0, -> %d/1", num, num);
            denom = 1;
        }
        if (gst_util_fraction_compare (num, denom, 1000, 1) > 0) {
            GST_LOG ("excessive framerate: %d/%d, -> 0/1", num, denom);
            num = 0;
            denom = 1;
        }

        gst_caps_set_simple (caps, "framerate", GST_TYPE_FRACTION, num, denom, NULL);
    }else{
        /* so we are after restricted caps in this case */
        switch (codec_id){
            case E_SME_MEDIA_CODEC_ID_H263:{
                /* 128x96, 176x144, 352x288, 704x576, and 1408x1152. slightly reordered
                * because we want automatic negotiation to go as close to 320x240 as
                * possible. */
                static const gint widths[] = { 352, 704, 176, 1408, 128 };
                static const gint heights[] = { 288, 576, 144, 1152, 96 };
                GstCaps *temp;
                gint n_sizes = G_N_ELEMENTS (widths);
                gint i = 0;
                caps = gst_caps_new_empty ();
                for (i = 0; i < n_sizes; i++){
                temp = gst_caps_new_simple (mimetype,
                    "width", G_TYPE_INT, widths[i],
                    "height", G_TYPE_INT, heights[i],
                    "framerate", GST_TYPE_FRACTION_RANGE, 0, 1, G_MAXINT, 1, NULL);

                    gst_caps_append (caps, temp);
                }
                break;
            }
            default:
                break;
        }
    }

    /* no fixed caps or special restrictions applied;
    * default unfixed setting */
    if (!caps) {
        GST_DEBUG ("Creating default caps");
        caps = gst_caps_new_simple (mimetype, NULL, NULL, NULL);
    }

    for (idx = 0; idx < gst_caps_get_size (caps); idx++) {
        va_start (var_args, fieldname);
        structure = gst_caps_get_structure (caps, idx);
        gst_structure_set_valist (structure, fieldname, var_args);
        va_end (var_args);
    }

    return caps;
}


/* same for audio - now with channels/sample rate
 */
static GstCaps *
gst_sme_aud_caps_new (ST_SME_MEDIAINFO * context, E_SME_MEDIA_CODEC_ID codec_id,
    const char *mimetype, const char *fieldname, ...)
{
    GstCaps *caps = NULL;
    guint idx = 0;
    va_list var_args;
    E_SME_MEM_SECURE_RET eReturn = E_SME_MEM_OK;

    PST_SME_AUDIOINFO pstAudioInfo = (PST_SME_AUDIOINFO)context;

    /* fixed, non-probing context */
    if (pstAudioInfo != NULL && pstAudioInfo->u32Channels != (guint)-1) {
        GST_INFO("codec_id=%d, rate=%d, channels=%d",
            codec_id, pstAudioInfo->u32SampleRate, pstAudioInfo->u32Channels);

        caps = gst_caps_new_simple (mimetype,
        "rate", G_TYPE_INT, pstAudioInfo->u32SampleRate,
        "channels", G_TYPE_INT, pstAudioInfo->u32Channels, NULL);
        if (codec_id == E_SME_MEDIA_CODEC_ID_ADPCM_IMA_WAV)
        {
            GValue item = G_VALUE_INIT;
            g_value_init(&item, G_TYPE_INT);
            g_value_set_int(&item, (gint)pstAudioInfo->u32BlockAlign);
            gst_caps_set_value (caps, "block_align", &item);

            g_value_reset(&item);
            g_value_init(&item, G_TYPE_INT);
            g_value_set_int(&item, (gint)pstAudioInfo->u32BitsPerCodecSample);

            gst_caps_set_value (caps, "bits_per_codec_sample", &item);
        }
        else if (codec_id == E_SME_MEDIA_CODEC_ID_ADPCM_MS)
        {
            GValue item = G_VALUE_INIT;
            g_value_init(&item, G_TYPE_INT);
            g_value_set_int(&item, (gint)pstAudioInfo->u32BlockAlign);

            gst_caps_set_value (caps, "block_align", &item);
        }
    } else {
        gint maxchannels = 2;
        const gint *rates = NULL;
        gint n_rates = 0;
        gint min_sample = 4000;
        gint max_sample = 96000;

        /* so we must be after restricted caps in this case */
        switch (codec_id) {
            case E_SME_MEDIA_CODEC_ID_AAC:
            case E_SME_MEDIA_CODEC_ID_MP2:{
                /* begin: modified for DTS2014061703194 */
                static const gint l_rates[] =
                {96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000, 12000, 11025, 8000, 7350 };
                /* end: modified for DTS2014061703194 */
                n_rates = G_N_ELEMENTS (l_rates);
                rates = l_rates;
                break;
            }
            case E_SME_MEDIA_CODEC_ID_AMR_NB:{
                static const gint l_rates[] = { 8000 };
                maxchannels = 1;
                n_rates = G_N_ELEMENTS (l_rates);
                rates = l_rates;
                break;
            }
            case E_SME_MEDIA_CODEC_ID_AMR_WB:{
                static const gint l_rates[] = { 16000 };
                maxchannels = 1;
                n_rates = G_N_ELEMENTS (l_rates);
                rates = l_rates;
                break;
            }
            case E_SME_MEDIA_CODEC_ID_AC3:{
                static const gint l_rates[] = {48000, 44100, 32000};
                maxchannels = 6;
                n_rates = G_N_ELEMENTS (l_rates);
                rates = l_rates;
                break;
            }
            case E_SME_MEDIA_CODEC_ID_EAC3:{
                static const gint l_rates[] = {48000, 44100, 32000};
                maxchannels = 8;
                n_rates = G_N_ELEMENTS (l_rates);
                rates = l_rates;
                break;
            }
            case E_SME_MEDIA_CODEC_ID_DRA:{
                maxchannels = 6;
                max_sample = 192000;
                break;
            }
            case E_SME_MEDIA_CODEC_ID_FLAC: {
                maxchannels = 8;
                max_sample = 192000;
                break;
            }
            case E_SME_MEDIA_CODEC_ID_VORBIS: {
                maxchannels = 6;
                break;
            }
            case E_SME_MEDIA_CODEC_ID_WMAVpro:{
                maxchannels = 8;
                break;
            }
            case E_SME_MEDIA_CODEC_ID_PCM_BLURAY:{
                maxchannels = 8;
                break;
            }
            case E_SME_MEDIA_CODEC_ID_WMAVLOSSLESS:{
                maxchannels = 8;
                break;
            }
            case E_SME_MEDIA_CODEC_ID_DTS: {
                maxchannels = 6;
                break;
            }
            case E_SME_MEDIA_CODEC_ID_PCM_S16LE:
            case E_SME_MEDIA_CODEC_ID_PCM_S16BE:
            case E_SME_MEDIA_CODEC_ID_PCM_MULAW:
            case E_SME_MEDIA_CODEC_ID_PCM_ALAW:
            case E_SME_MEDIA_CODEC_ID_PCM_S32LE:
            case E_SME_MEDIA_CODEC_ID_PCM_S24LE:
            case E_SME_MEDIA_CODEC_ID_PCM_S24BE:
            case E_SME_MEDIA_CODEC_ID_PCM_U8:
            case E_SME_MEDIA_CODEC_ID_PCM_DVD:
            case E_SME_MEDIA_CODEC_ID_PCM_F32BE:
            case E_SME_MEDIA_CODEC_ID_PCM_F32LE:{
                maxchannels = 6;
                max_sample = 384000;
                break;
            }
            default:
                break;
        }

        if (maxchannels == 1){
            caps = gst_caps_new_simple (mimetype, "channels", G_TYPE_INT, maxchannels,
                NULL);
        }else{
            caps = gst_caps_new_simple (mimetype, "channels", GST_TYPE_INT_RANGE, 1,
                maxchannels, NULL);
        }

        if (n_rates && rates) {
            gint i = 0;
            GValue list;
            GstStructure *structTmp = NULL;

            eReturn = VOS_Memset_S((void*)(&list), sizeof(list), 0, sizeof(list));
            if(E_SME_MEM_OK != eReturn){
                GST_WARNING("VOS_Memset_S list Failed(%d)", eReturn);
                return caps;
            }

            g_value_init (&list, GST_TYPE_LIST);
            for (i = 0; i < n_rates; i++) {
                GValue v;

                eReturn = VOS_Memset_S(&v, sizeof(v), 0, sizeof(v));
                if(E_SME_MEM_OK != eReturn){
                    GST_WARNING("VOS_Memset_S v Failed(%d)", eReturn);
                    return caps;
                }

                g_value_init (&v, G_TYPE_INT);
                g_value_set_int (&v, rates[i]);
                gst_value_list_append_value (&list, &v);
                g_value_unset (&v);
            }
            structTmp = gst_caps_get_structure (caps, 0);
            gst_structure_set_value (structTmp, "rate", &list);
            g_value_unset (&list);
        } else {
            gst_caps_set_simple (caps, "rate", GST_TYPE_INT_RANGE, min_sample, max_sample, NULL);
        }
    }

    for (idx = 0; idx < gst_caps_get_size (caps); idx++) {
        GstStructure *structure;
        va_start (var_args, fieldname);
        structure = gst_caps_get_structure (caps, idx);
        gst_structure_set_valist (structure, fieldname, var_args);
        va_end (var_args);
    }

    return caps;
}


GstCaps *
gst_sme_codecid_to_caps (E_SME_MEDIA_CODEC_ID codec_id,
    ST_SME_MEDIAINFO * context, gboolean encode)
{
    GstCaps *caps = NULL;
    E_SME_MEM_SECURE_RET eReturn = E_SME_MEM_OK;
    int mimelen = 0;
    E_VOS_STR_ERR eErr = E_VOS_STR_ENONE;

    if(context != NULL){
        mimelen = sizeof(context->acMime);
    }

    GST_INFO("codec_id:%d, context:%p, encode:%d", codec_id, context, encode);

    switch (codec_id) {
        case E_SME_MEDIA_CODEC_ID_MPEG1VIDEO:
            /* FIXME: bitrate */
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "video/mpeg",
                      (guint)strlen("video/mpeg"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy video/mpeg failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_vid_caps_new (context, codec_id, context->acMime,
                    "mpegversion", G_TYPE_INT, 1, "systemstream", G_TYPE_BOOLEAN, FALSE,
                    NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_MPEG2VIDEO:
            if (encode) {
                /* FIXME: bitrate */
                if (context){
                    if(0 == context->acMime[0] && mimelen > 0){
                        eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "video/mpeg",
                          (guint)strlen("video/mpeg"));
                        if(eErr != E_VOS_STR_ENONE){
                            GST_WARNING("copy video/mpeg failed(%d)", eErr);
                            break;
                        }
                    }
                    caps = gst_sme_vid_caps_new (context, codec_id, context->acMime,
                        "mpegversion", G_TYPE_INT, 2, "systemstream", G_TYPE_BOOLEAN,
                        FALSE, NULL);
                }
            }else {
                /* decode both MPEG-1 and MPEG-2; width/height/fps are all in
                * the MPEG video stream headers, so may be omitted from caps. */
                caps = gst_caps_new_simple ("video/mpeg",
                    "mpegversion", GST_TYPE_INT_RANGE, 1, 2,
                    "systemstream", G_TYPE_BOOLEAN, FALSE, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_H263:
            if (context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "video/x-h263",
                      (guint)strlen("video/x-h263"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy video/x-h263 failed(%d)", eErr);
                        break;
                    }
                }

                if (encode) {
                    caps = gst_sme_vid_caps_new (context, codec_id, context->acMime,
                         "variant", G_TYPE_STRING, "itu",
                         "h263version", G_TYPE_STRING, "h263",
                         "parsed", G_TYPE_BOOLEAN, TRUE, NULL);
                } else {
                    /* don't pass codec_id, we can decode other variants with the H263
                    * decoder that don't have specific size requirements
                    */
                    caps = gst_sme_vid_caps_new (context, E_SME_MEDIA_CODEC_ID_NONE,
                        context->acMime, "variant", G_TYPE_STRING, "itu", NULL);
                }
            }
            break;
        case E_SME_MEDIA_CODEC_ID_MP1:
            /* FIXME: bitrate */
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/mpeg",
                      (guint)strlen("audio/mpeg"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/mpeg failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                    "mpegversion", G_TYPE_INT, 1, "layer", G_TYPE_INT, 1, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_MP2:
            /* FIXME: bitrate */
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/mpeg",
                      (guint)strlen("audio/mpeg"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/mpeg failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                    "mpegversion", G_TYPE_INT, 1, "layer", G_TYPE_INT, 2, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_MP3:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/mpeg",
                      (guint)strlen("audio/mpeg"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/mpeg failed(%d)", eErr);
                        break;
                    }
                }
            }

            if (encode) {
                /* FIXME: bitrate */
                if(context){
                    caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                         "mpegversion", G_TYPE_INT, 1, "layer", G_TYPE_INT, 3, NULL);
                }
            } else {
                /* Decodes MPEG-1 layer 1/2/3. Samplerate, channels et al are
                * in the MPEG audio header, so may be omitted from caps. */
                caps = gst_caps_new_simple ("audio/mpeg",
                    "mpegversion", G_TYPE_INT, 1, "layer", GST_TYPE_INT_RANGE, 1, 3, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_MPEG4:
            if (encode && context) {
                /* I'm not exactly sure what ffmpeg outputs... ffmpeg itself uses
                * the AVI fourcc 'DIVX', but 'mp4v' for Quicktime... */
                //if (strcmp(context->acMime, "video/x-divx")) {
                if(context->u32CodecForcc == GST_MAKE_FOURCC ('D', 'I', 'V', 'X')){
                    caps = gst_sme_vid_caps_new (context, codec_id, context->acMime,
                        "divxversion", G_TYPE_INT, 5, NULL);
                } else {
                    /* FIXME: bitrate */
                    caps = gst_sme_vid_caps_new (context, codec_id, context->acMime,
                         "systemstream", G_TYPE_BOOLEAN, FALSE,
                         "mpegversion", G_TYPE_INT, 4, NULL);
                    break;
                }
            } else {
                /* The trick here is to separate xvid, divx, mpeg4, 3ivx et al */
                if (context){
                    if(0 == context->acMime[0] && mimelen > 0){
                        eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "video/mpeg",
                          (guint)strlen("video/mpeg"));
                        if(eErr != E_VOS_STR_ENONE){
                            GST_WARNING("copy video/mpeg failed(%d)", eErr);
                            break;
                        }
                    }
                    caps = gst_sme_vid_caps_new (context, codec_id, context->acMime,
                         "mpegversion", G_TYPE_INT, 4, "systemstream", G_TYPE_BOOLEAN,
                         FALSE, NULL);
                }

                if (encode) {
                    gst_caps_append (caps, gst_sme_vid_caps_new (context, codec_id,
                        "video/x-divx", "divxversion", G_TYPE_INT, 5, NULL));
                } else {
                    gst_caps_append (caps, gst_sme_vid_caps_new (context, codec_id,
                        "video/x-divx", "divxversion", GST_TYPE_INT_RANGE, 4, 5, NULL));
                    gst_caps_append (caps, gst_sme_vid_caps_new (context, codec_id,
                        "video/x-xvid", NULL));
                    gst_caps_append (caps, gst_sme_vid_caps_new (context, codec_id,
                        "video/x-3ivx", NULL));
                }
            }
            break;
        case E_SME_MEDIA_CODEC_ID_MSMPEG4V3:
            if (context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "video/divx3",
                      (guint)strlen("video/divx3"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy video/divx3 failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_vid_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_H264:
            if (context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "video/x-h264",
                      (guint)strlen("video/x-h264"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy video/x-h264 failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_vid_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_CAVS:
        case E_SME_MEDIA_CODEC_ID_AVS:
            if (context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "video/avs",
                      (guint)strlen("video/avs"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy video/avs failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_vid_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_RV30:
            if (context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "video/rv30",
                      (guint)strlen("video/rv30"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy video/rv30 failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_vid_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_HEVC:
            if (context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "video/x-h265",
                      (guint)strlen("video/x-h265"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy video/x-h265 failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_vid_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_VC1:
        case E_SME_MEDIA_CODEC_ID_WMV3:
            if (context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "video/x-wmv",
                      (guint)strlen("video/x-wmv"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy video/x-wmv failed(%d)", eErr);
                        break;
                    }
                }
                if(encode){
                    V_CHAR* pszProfile = (V_CHAR*)"VC1SP";
                    if(E_SME_MEDIA_CODEC_ID_VC1 == codec_id){
                        if(E_SME_VC1_PROFILE_ADVANCED == context->i32Profile){
                            pszProfile = (V_CHAR*)"VC1AP";
                        }else if(E_SME_VC1_PROFILE_COMPLEX == context->i32Profile){
                            pszProfile = (V_CHAR*)"VC1CP";
                        }else if(E_SME_VC1_PROFILE_MAIN == context->i32Profile){
                            pszProfile = (V_CHAR*)"VC1MP";
                        }else{
                            pszProfile = (V_CHAR*)"VC1SP";
                        }
                    }
                    GST_INFO("vc1 profile=%s", pszProfile);
                    caps = gst_sme_vid_caps_new (context, codec_id, context->acMime,
                        "wmvversion", G_TYPE_INT, 3,
                        "profile", G_TYPE_STRING, pszProfile, NULL);
                }else{
                    caps = gst_sme_vid_caps_new (context, codec_id, context->acMime, NULL);
                }
            }
            break;
        case E_SME_MEDIA_CODEC_ID_VP8:
            if (context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "video/x-vp8",
                      (guint)strlen("video/x-vp8"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy video/x-vp8 failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_vid_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_VP6:
        case E_SME_MEDIA_CODEC_ID_VP6F:
        case E_SME_MEDIA_CODEC_ID_VP6A:
            if (context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "video/x-vp6",
                      (guint)strlen("video/x-vp6"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy video/x-vp6 failed(%d)", eErr);
                        break;
                    }
                }
                if(encode){
                    V_CHAR* pszFmt = (V_CHAR*)"vp6";
                    if(E_SME_MEDIA_CODEC_ID_VP6F == codec_id){
                        pszFmt = (V_CHAR*)"vp6F";
                    }else if(E_SME_MEDIA_CODEC_ID_VP6A == codec_id){
                        pszFmt = (V_CHAR*)"vp6A";
                    }else{
                        pszFmt = (V_CHAR*)"vp6";
                    }
                    GST_INFO("vp6 format=%s", pszFmt);
                    caps = gst_sme_vid_caps_new (context, codec_id, context->acMime,
                        "vp6_format", G_TYPE_STRING, pszFmt, NULL);
                }else{
                    caps = gst_sme_vid_caps_new (context, codec_id, context->acMime, NULL);
                }
            }
            break;
        case E_SME_MEDIA_CODEC_ID_VORBIS:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-vorbis",
                      (guint)strlen("audio/x-vorbis"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-vorbis failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_S16LE:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-pcm",
                      (guint)strlen("audio/x-pcm"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-pcm failed(%d)", eErr);
                        break;
                    }
                }

                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                    "format", G_TYPE_STRING, "S16LE", "layout", G_TYPE_STRING,
                    "interleaved", NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_S16BE:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-pcm",
                      (guint)strlen("audio/x-pcm"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-pcm failed(%d)", eErr);
                        break;
                    }
                }

                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                    "format", G_TYPE_STRING, "S16BE", "layout", G_TYPE_STRING,
                    "interleaved", NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_MULAW:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-pcm",
                      (guint)strlen("audio/x-pcm"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-pcm failed(%d)", eErr);
                        break;
                    }
                }

                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                    "format", G_TYPE_STRING, "MULAW", "layout", G_TYPE_STRING,
                    "interleaved", NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_ALAW:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-pcm",
                      (guint)strlen("audio/x-pcm"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-pcm failed(%d)", eErr);
                        break;
                    }
                }

                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                    "format", G_TYPE_STRING, "ALAW", "layout", G_TYPE_STRING,
                    "interleaved", NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_S32LE:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-pcm",
                      (guint)strlen("audio/x-pcm"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-pcm failed(%d)", eErr);
                        break;
                    }
                }

                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                    "format", G_TYPE_STRING, "S32LE", "layout", G_TYPE_STRING,
                    "interleaved", NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_S24LE:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-pcm",
                      (guint)strlen("audio/x-pcm"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-pcm failed(%d)", eErr);
                        break;
                    }
                }

                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                    "format", G_TYPE_STRING, "S24LE", "layout", G_TYPE_STRING,
                    "interleaved", NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_S24BE:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-pcm",
                      (guint)strlen("audio/x-pcm"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-pcm failed(%d)", eErr);
                        break;
                    }
                }

                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                    "format", G_TYPE_STRING, "S24BE", "layout", G_TYPE_STRING,
                    "interleaved", NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_U8:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-pcm",
                      (guint)strlen("audio/x-pcm"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-pcm failed(%d)", eErr);
                        break;
                    }
                }

                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                    "format", G_TYPE_STRING, "U8", "layout", G_TYPE_STRING,
                    "interleaved", NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_DVD:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/pcm-dvd",
                        (guint)strlen("audio/pcm-dvd"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/pcm-dvd failed(%d)", eErr);
                        break;
                    }
                }

                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_PCM_BLURAY:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/pcm-bluray",
                        (guint)strlen("audio/pcm-bluray"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/pcm-bluray failed(%d)", eErr);
                        break;
                    }
                }

                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
            case E_SME_MEDIA_CODEC_ID_PCM_F32BE:
                if(context){
                    if(0 == context->acMime[0] && mimelen > 0){
                        eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-pcm",
                          (guint)strlen("audio/x-pcm"));
                        if(eErr != E_VOS_STR_ENONE){
                            GST_WARNING("copy audio/x-pcm failed(%d)", eErr);
                            break;
                        }
                    }
                    caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                        "format", G_TYPE_STRING, "F32BE", "layout", G_TYPE_STRING,
                        "interleaved", NULL);
                }
                break;
            case E_SME_MEDIA_CODEC_ID_PCM_F32LE:
                if(context){
                    if(0 == context->acMime[0] && mimelen > 0){
                        eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-pcm",
                          (guint)strlen("audio/x-pcm"));
                        if(eErr != E_VOS_STR_ENONE){
                            GST_WARNING("copy audio/x-pcm failed(%d)", eErr);
                            break;
                        }
                    }
                    caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,
                        "format", G_TYPE_STRING, "F32LE", "layout", G_TYPE_STRING,
                        "interleaved", NULL);
                }
                break;
        case E_SME_MEDIA_CODEC_ID_JPEG:
        case E_SME_MEDIA_CODEC_ID_MJPEG:
            if(context){
                caps = gst_sme_vid_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_AMR_NB:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/AMR-NB",
                        (guint)strlen("audio/AMR-NB"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/AMR-NB failed(%d)", eErr);
                        break;
                    }
                }

                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_AMR_WB:
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/AMR-WB",
                        (guint)strlen("audio/AMR-WB"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/AMR-WB failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        case E_SME_MEDIA_CODEC_ID_AAC:{
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/mpeg",
                        (guint)strlen("audio/mpeg"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/mpeg failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime, NULL);
            }

            if (!encode) {
                GValue arr;
                GValue item;

                eReturn = VOS_Memset_S(&arr, sizeof(arr), 0, sizeof(arr));
                if(E_SME_MEM_OK != eReturn){
                    GST_WARNING("VOS_Memset_S arr Failed(%d)", eReturn);
                }
                eReturn = VOS_Memset_S(&item, sizeof(item), 0, sizeof(item));
                if(E_SME_MEM_OK != eReturn){
                    GST_WARNING("VOS_Memset_S arr Failed(%d)", eReturn);
                }

                g_value_init (&arr, GST_TYPE_LIST);
                g_value_init (&item, G_TYPE_INT);
                g_value_set_int (&item, 2);
                gst_value_list_append_value (&arr, &item);
                g_value_set_int (&item, 4);
                gst_value_list_append_value (&arr, &item);
                g_value_unset (&item);

                gst_caps_set_value (caps, "mpegversion", &arr);
                g_value_unset (&arr);

                g_value_init (&arr, GST_TYPE_LIST);
                g_value_init (&item, G_TYPE_STRING);
                g_value_set_string (&item, "raw");
                gst_value_list_append_value (&arr, &item);
                g_value_set_string (&item, "adts");
                gst_value_list_append_value (&arr, &item);
                g_value_set_string (&item, "adif");
                gst_value_list_append_value (&arr, &item);
                g_value_unset (&item);

                gst_caps_set_value (caps, "stream-format", &arr);
                g_value_unset (&arr);
            } else {
                gst_caps_set_simple (caps, "mpegversion", G_TYPE_INT, 4,
                    "stream-format", G_TYPE_STRING, "raw",
                    "base-profile", G_TYPE_STRING, "lc", NULL);

                if (context && context->stCfgData.u32CfgDataSize > 0){
                    gst_codec_utils_aac_caps_set_level_and_profile (caps,
                        context->stCfgData.pu8CfgData, context->stCfgData.u32CfgDataSize);
                }
            }
            break;
        }
        /**< FIXME:
        *   Are there other attributions to be specified?
        */
        case E_SME_MEDIA_CODEC_ID_AC3: {
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/ac3",
                        (guint)strlen("audio/ac3"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/ac3 failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        }
        case E_SME_MEDIA_CODEC_ID_EAC3: {
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/eac3",
                        (guint)strlen("audio/eac3"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/eac3 failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime, NULL);
            }
            /**< add ac3 caps to eac3 decoder too */
            if (!encode && context){
                GstCaps *subcaps;
                subcaps = gst_sme_aud_caps_new(context, E_SME_MEDIA_CODEC_ID_AC3,
                    "audio/ac3", NULL);
                gst_caps_append(caps, subcaps);
            }
            break;
        }
        case E_SME_MEDIA_CODEC_ID_DRA: {
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/dra",
                        (guint)strlen("audio/dra"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/dra failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        }
        case E_SME_MEDIA_CODEC_ID_WMAV1:
        {
            gint version = 1;

            if (context && encode) {
                gint block_align = (gint)(((PST_SME_AUDIOINFO)context)->u32BlockAlign);
                gint sample_fmt = (gint)(((PST_SME_AUDIOINFO)context)->eSampleFmt);
                caps = gst_sme_aud_caps_new (context, codec_id, "audio/x-wma",
                     "wmaversion", G_TYPE_INT, version,
                     "block_align", G_TYPE_INT,
                     block_align,
                     "bitrate", G_TYPE_INT,
                     context->u32Bps,
                     "sample_fmt", G_TYPE_INT,
                     sample_fmt, NULL);
            }
            else {
                caps = gst_sme_aud_caps_new (context, codec_id, "audio/x-wma",
                     "wmaversion", G_TYPE_INT, version,
                     "block_align", GST_TYPE_INT_RANGE,
                     0, G_MAXINT,
                     "bitrate", GST_TYPE_INT_RANGE,
                     0, G_MAXINT,
                     "sample_fmt", GST_TYPE_INT_RANGE,
                     0, G_MAXINT, NULL);
            }
            break;
        }
        case E_SME_MEDIA_CODEC_ID_WMAV2:
        {
            gint version = 2;

            if (context && encode) {
                gint block_align = (gint)(((PST_SME_AUDIOINFO)context)->u32BlockAlign);
                gint sample_fmt = (gint)(((PST_SME_AUDIOINFO)context)->eSampleFmt);
                caps = gst_sme_aud_caps_new (context, codec_id, "audio/x-wma",
                     "wmaversion", G_TYPE_INT, version,
                     "block_align", G_TYPE_INT,
                     block_align,
                     "bitrate", G_TYPE_INT,
                     context->u32Bps,
                     "sample_fmt", G_TYPE_INT,
                     sample_fmt, NULL);
            }
            else {
                caps = gst_sme_aud_caps_new (context, codec_id, "audio/x-wma",
                     "wmaversion", G_TYPE_INT, version,
                     "block_align", GST_TYPE_INT_RANGE,
                     0, G_MAXINT,
                     "bitrate", GST_TYPE_INT_RANGE,
                     0, G_MAXINT,
                     "sample_fmt", GST_TYPE_INT_RANGE,
                     0, G_MAXINT, NULL);
            }
            break;
        }
        case E_SME_MEDIA_CODEC_ID_WMAVpro:{
            gint version = 3;

            if (context && encode) {
                gint block_align = (gint)(((PST_SME_AUDIOINFO)context)->u32BlockAlign);
                gint sample_fmt = (gint)(((PST_SME_AUDIOINFO)context)->eSampleFmt);
                caps = gst_sme_aud_caps_new (context, codec_id, "audio/x-wma",
                     "wmaversion", G_TYPE_INT, version,
                     "block_align", G_TYPE_INT,
                     block_align,
                     "bitrate", G_TYPE_INT,
                     context->u32Bps,
                     "sample_fmt", G_TYPE_INT,
                     sample_fmt, NULL);
            }
            else {
                caps = gst_sme_aud_caps_new (context, codec_id, "audio/x-wma",
                     "wmaversion", G_TYPE_INT, version,
                     "block_align", GST_TYPE_INT_RANGE,
                     0, G_MAXINT,
                     "bitrate", GST_TYPE_INT_RANGE,
                     0, G_MAXINT,
                     "sample_fmt", GST_TYPE_INT_RANGE,
                     0, G_MAXINT, NULL);
            }
            break;
        }
        case E_SME_MEDIA_CODEC_ID_WMAVLOSSLESS:{
            gint version = 4;

            if (context && encode) {
                gint block_align = (gint)(((PST_SME_AUDIOINFO)context)->u32BlockAlign);
                gint sample_fmt = (gint)(((PST_SME_AUDIOINFO)context)->eSampleFmt);
                caps = gst_sme_aud_caps_new (context, codec_id, "audio/x-wma",
                     "wmaversion", G_TYPE_INT, version,
                     "block_align", G_TYPE_INT,
                     block_align,
                     "bitrate", G_TYPE_INT,
                     context->u32Bps,
                     "sample_fmt", G_TYPE_INT,
                     sample_fmt, NULL);
            }
            else {
                caps = gst_sme_aud_caps_new (context, codec_id, "audio/x-wma",
                     "wmaversion", G_TYPE_INT, version,
                     "block_align", GST_TYPE_INT_RANGE,
                     0, G_MAXINT,
                     "bitrate", GST_TYPE_INT_RANGE,
                     0, G_MAXINT,
                     "sample_fmt", GST_TYPE_INT_RANGE,
                     0, G_MAXINT, NULL);
            }
            break;
        }
        case E_SME_MEDIA_CODEC_ID_FLAC: {
            if(context){
                GST_INFO("codec=%d, FLAC, acMime=%s", codec_id, context->acMime);
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/flac",
                        (guint)strlen("audio/flac"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/flac failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime, NULL);
            }
            break;
        }
        case E_SME_MEDIA_CODEC_ID_ADPCM_IMA_WAV: {  /*support WAV: ADPCM*/
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-adpcm_ima_wav",
                        (guint)strlen("audio/x-adpcm_ima_wav"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-adpcm_ima_wav failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,NULL);
            }
            break;
        }
        case E_SME_MEDIA_CODEC_ID_ADPCM_MS: {  /*support WAV: ADPCM_MS*/
            if(context){
                if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-adpcm_ms",
                        (guint)strlen("audio/x-adpcm_ms"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-adpcm_ms failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,NULL);
            }
            break;
        }
        case E_SME_MEDIA_CODEC_ID_GSM: {
            if (context){
                 if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-gsm",
                        (guint)strlen("audio/x-gsm"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-gsm failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,NULL);
            }
            break;
        }
        case E_SME_MEDIA_CODEC_ID_GSM_MS: {
            if (context){
                 if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-gsm_ms",
                        (guint)strlen("audio/x-gsm_ms"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-gsm_ms failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,NULL);
            }
            break;
        }
        case E_SME_MEDIA_CODEC_ID_DTS: {
            if (context){
                 if(0 == context->acMime[0] && mimelen > 0){
                    eErr = VOS_Strncpy_S(context->acMime, (guint)mimelen, "audio/x-dts",
                        (guint)strlen("audio/x-dts"));
                    if(eErr != E_VOS_STR_ENONE){
                        GST_WARNING("copy audio/x-dts failed(%d)", eErr);
                        break;
                    }
                }
                caps = gst_sme_aud_caps_new (context, codec_id, context->acMime,NULL);
            }
            break;
        }
        default:
            GST_WARNING("codec=%d, not support", codec_id);
            break;
    }

    if (caps != NULL) {
        /* set private data */
        if (context && context->stCfgData.u32CfgDataSize > 0) {
            GstBuffer *data = gst_buffer_new_and_alloc (context->stCfgData.u32CfgDataSize);
            gst_buffer_fill (data, 0, context->stCfgData.pu8CfgData,
                context->stCfgData.u32CfgDataSize);
            gst_caps_set_simple (caps, "codec_data", GST_TYPE_BUFFER, data, NULL);
            gst_buffer_unref (data);
        }

        if (context && (E_SME_MEDIA_TYPE_VIDEO == context->eMediaType)) {
            gst_caps_set_simple (caps, "is_frame", G_TYPE_BOOLEAN, TRUE, NULL);
        }

        if (context && (E_SME_MEDIA_TYPE_VIDEO == context->eMediaType)
            && (((PST_SME_VIDEOINFO)context)->rotate)) {
            gst_caps_set_simple (caps,
                "rotate", G_TYPE_UINT, ((PST_SME_VIDEOINFO)context)->rotate, NULL);
        }

        if (context) {
            gchar* caps_tostring = gst_caps_to_string(caps);
            GST_INFO("codecid=%d, caps=%s", context->eCodecId, caps_tostring);
            g_free(caps_tostring);
        }
    } else {
        GST_WARNING ("No caps found for codec_id=%d", codec_id);
    }

    return caps;
}


/* _formatid_to_caps () is meant for muxers/demuxers, it
 * transforms a name (ffmpeg way of ID'ing these, why don't
 * they have unique numerical IDs?) to the corresponding
 * caps belonging to that mux-format
 *
 * Note: we don't need any additional info because the caps
 * isn't supposed to contain any useful info besides the
 * media type anyway
 */

GstCaps *
gst_sme_formatid_to_caps (const gchar * format_name)
{
    GstCaps *caps = NULL;

    if (!strcmp (format_name, "mpeg")) {
        caps = gst_caps_new_simple ("video/mpeg",
            "systemstream", G_TYPE_BOOLEAN, (gboolean)TRUE, NULL);
    } else if (!strcmp (format_name, "mpegts")) {
        caps = gst_caps_new_simple ("video/mpegts",
            "systemstream", G_TYPE_BOOLEAN, (gboolean)TRUE, NULL);
    } else if (!strcmp (format_name, "mpegvideo")) {
        caps = gst_caps_new_simple ("video/rawmpegvideo",
            "systemstream", G_TYPE_BOOLEAN, (gboolean)TRUE, NULL);
    } else if (!strcmp (format_name, "rm")) {
        caps = gst_caps_new_simple ("application/vnd.rn-realmedia", NULL);
    } else if (!strcmp (format_name, "asf")) {
        caps = gst_caps_new_empty_simple ("video/x-ms-asf");
    } else if (!strcmp (format_name, "avi")) {
        caps = gst_caps_new_empty_simple ("video/x-msvideo");
    } else if (!strcmp (format_name, "wav")){
        caps = gst_caps_new_empty_simple ("audio/x-wav");
    } else if (!strcmp (format_name, "ape")) {
        caps = gst_caps_new_empty_simple ("application/x-ape");
    } else if (!strcmp (format_name, "swf")) {
        caps = gst_caps_new_empty_simple ("application/x-shockwave-flash");
    } else if (!strcmp (format_name, "au")) {
        caps = gst_caps_new_empty_simple ("audio/x-au");
    } else if (!strcmp (format_name, "dv")) {
        caps = gst_caps_new_simple ("video/x-dv",
            "systemstream", G_TYPE_BOOLEAN, (gboolean)TRUE, NULL);
    } else if (!strcmp (format_name, "4xm")) {
        caps = gst_caps_new_empty_simple ("video/x-4xm");
    } else if (!strcmp (format_name, "matroska")) {
        caps = gst_caps_new_empty_simple ("video/x-matroska");
    } else if (!strcmp (format_name, "matroska_webm")
        || !strcmp (format_name, "matroska,webm")) {
        caps = gst_caps_new_empty_simple ("video/x-matroska");
    } else if (!strcmp (format_name, "mp3")) {
        caps = gst_caps_new_empty_simple ("application/x-id3");
    } else if (!strcmp (format_name, "flic")) {
        caps = gst_caps_new_empty_simple ("video/x-fli");
    } else if (!strcmp (format_name, "flv")) {
        caps = gst_caps_new_empty_simple ("video/x-flv");
    } else if (!strcmp (format_name, "tta")) {
        caps = gst_caps_new_empty_simple ("audio/x-ttafile");
    } else if (!strcmp (format_name, "aiff")) {
        caps = gst_caps_new_empty_simple ("audio/x-aiff");
    } else if (!strcmp (format_name, "mov_mp4_m4a_3gp_3g2")) {
        caps = gst_caps_from_string("application/x-3gp; video/quicktime; audio/x-m4a");
    } else if (!strcmp (format_name, "mov_mp4_m4a_3gp_3g2_mj2")) {
        caps = gst_caps_from_string("application/x-3gp; video/quicktime; audio/x-m4a");
    }else if (!strcmp (format_name, "mov,mp4,m4a,3gp,3g2,mj2")) {
        caps = gst_caps_from_string("video/quicktime,variant=(string)iso");
    } else if (!strcmp (format_name, "mov")) {
        caps = gst_caps_from_string ("video/quicktime,variant=(string)apple");
    } else if (!strcmp (format_name, "mp4")) {
        caps = gst_caps_from_string ("video/quicktime,variant=(string)iso");
    } else if (!strcmp (format_name, "3gp")) {
        caps = gst_caps_from_string ("video/quicktime,variant=(string)3gpp");
    } else if (!strcmp (format_name, "3g2")) {
        caps = gst_caps_from_string ("video/quicktime,variant=(string)3g2");
    } else if (!strcmp (format_name, "psp")) {
        caps = gst_caps_from_string ("video/quicktime,variant=(string)psp");
    } else if (!strcmp (format_name, "ipod")) {
        caps = gst_caps_from_string ("video/quicktime,variant=(string)ipod");
    } else if (!strcmp (format_name, "aac")) {
        //caps = gst_caps_new_simple ("audio/mpeg", "mpegversion", G_TYPE_INT, 4, NULL);
        caps = gst_caps_new_simple ("audio/aac", NULL);
    } else if (!strcmp (format_name, "gif")) {
        caps = gst_caps_from_string ("image/gif");
    } else if (!strcmp (format_name, "ogg")) {
        caps = gst_caps_from_string ("audio/ogg");
    } else if (!strcmp (format_name, "mxf") || !strcmp (format_name, "mxf_d10")) {
        caps = gst_caps_from_string ("application/mxf");
    } else if (!strcmp (format_name, "gxf")) {
        caps = gst_caps_from_string ("application/gxf");
    } else if (!strcmp (format_name, "yuv4mpegpipe")) {
        caps = gst_caps_new_simple ("application/x-yuv4mpeg", "y4mversion", G_TYPE_INT,
            2, NULL);
    } else if (!strcmp (format_name, "mpc")) {
        caps = gst_caps_from_string ("audio/x-musepack, streamversion = (int) 7");
    } else if (!strcmp (format_name, "vqf")) {
        caps = gst_caps_from_string ("audio/x-vqf");
    } else if (!strcmp (format_name, "nsv")) {
        caps = gst_caps_from_string ("video/x-nsv");
    } else if (!strcmp (format_name, "amr")) {
        caps = gst_caps_from_string ("audio/x-amr-nb-sh");
    } else if (!strcmp (format_name, "webm")) {
        caps = gst_caps_from_string ("video/webm");
    } else if (!strcmp (format_name, "voc")) {
        caps = gst_caps_from_string ("audio/x-voc");
    } else if (!strcmp (format_name, "ac3")) {
        caps = gst_caps_from_string ("audio/x-ac3");
    } else if (!strcmp (format_name, "dra")) {
        caps = gst_caps_from_string ("audio/x-dra");

    }else{
        gchar *name;

        GST_INFO("Could not create stream format caps for %s", format_name);
        name = g_strdup_printf ("application/x-gst_ff-%s", format_name);
        if(name){
            caps = gst_caps_new_empty_simple (name);
        }

        g_free (name);
    }

    return caps;
}

static GstCaps *
gst_sme_smpfmt_to_caps (E_SME_AUDIO_RAW_FMT sample_fmt,
    ST_SME_MEDIAINFO * context, E_SME_MEDIA_CODEC_ID codec_id)
{
    GstCaps *caps = NULL;
    GstAudioFormat format;

    switch (sample_fmt) {
        case E_SME_AUDIO_RAW_FMT_PCM_8BIT:
        case E_SME_AUDIO_RAW_FMT_PCM_8BITP:
            format = GST_AUDIO_FORMAT_U8;
            break;
        case E_SME_AUDIO_RAW_FMT_PCM_S16BIT:
        case E_SME_AUDIO_RAW_FMT_PCM_S16BITP:
            format = GST_AUDIO_FORMAT_S16;
            break;
        case E_SME_AUDIO_RAW_FMT_PCM_S32BIT:
        case E_SME_AUDIO_RAW_FMT_PCM_S32BITP:
            format = GST_AUDIO_FORMAT_S32;
            break;
        case E_SME_AUDIO_RAW_FMT_PCM_FLT:
        case E_SME_AUDIO_RAW_FMT_PCM_FLTP:
            format = GST_AUDIO_FORMAT_F32;
            break;
        case E_SME_AUDIO_RAW_FMT_PCM_DBL:
        case E_SME_AUDIO_RAW_FMT_PCM_DBLP:
            format = GST_AUDIO_FORMAT_F64;
            break;
#ifdef __DDP_PT__
        case E_SME_AUDIO_RAW_FMT_PT_AC3:
            format = GST_AUDIO_FORMAT_ENCODED;
        break;
        case E_SME_AUDIO_RAW_FMT_PT_E_AC3:
            format = GST_AUDIO_FORMAT_ENCODED;
        break;
#endif
        default:
            /* .. */
            format = GST_AUDIO_FORMAT_UNKNOWN;
            break;
    }

#ifdef __DDP_PT__
    if (format == GST_AUDIO_FORMAT_ENCODED) {
        gchar * format_str = NULL;
        switch(codec_id) {
            case E_SME_MEDIA_CODEC_ID_AC3:
                format_str = "ac3";
            break;
            case E_SME_MEDIA_CODEC_ID_EAC3:
                format_str = "eac3";
            break;
            case E_SME_MEDIA_CODEC_ID_TRUEHD:
                format_str = "truehd";
            break;
            default:
                format_str = "unknown";
            break;
       }
        caps = gst_sme_aud_caps_new (context, codec_id, "audio/bitstream",
           "format", G_TYPE_STRING, format_str,
            NULL);
        GST_INFO ("caps for PT sample_fmt=%d: %s", sample_fmt, gst_caps_to_string(caps));
    } else
#endif
    if (format != GST_AUDIO_FORMAT_UNKNOWN) {
        caps = gst_sme_aud_caps_new (context, codec_id, "audio/x-raw",
            "format", G_TYPE_STRING, gst_audio_format_to_string (format),
            "layout", G_TYPE_STRING, "interleaved", NULL);
        GST_INFO ("caps for sample_fmt=%d: %" GST_PTR_FORMAT, sample_fmt, caps);
    } else {
        GST_INFO ("No caps found for sample_fmt=%d", sample_fmt);
    }

    return caps;
}


GstCaps *
gst_sme_codectype_to_audio_caps (E_SME_MEDIA_CODEC_ID codec_id, ST_SME_MEDIAINFO * context,
    gboolean encode,E_SME_AUDIO_RAW_FMT *samplefmts)
{
    GstCaps *caps = NULL;

    GST_INFO ("context:%p, codec_id:%d, encode:%d, codec:%p", context, codec_id, encode,
        samplefmts);

    if (samplefmts) {
        int i;

        caps = gst_caps_new_empty ();
        for (i = 0; samplefmts[i] != (E_SME_AUDIO_RAW_FMT)-1; i++) {
            GstCaps *temp;
            temp = gst_sme_smpfmt_to_caps (samplefmts[i], context, codec_id);
            if (temp != NULL){
                gst_caps_append (caps, temp);
            }
        }
    } else {
        E_SME_AUDIO_RAW_FMT i;

        caps = gst_caps_new_empty ();
        for (i = E_SME_AUDIO_RAW_FMT_PCM_8BIT; i <= E_SME_AUDIO_RAW_FMT_PCM_DBLP; i++) {
            GstCaps *temp;
            temp = gst_sme_smpfmt_to_caps (i,context,codec_id);
            if (temp != NULL) {
                gst_caps_append (caps, temp);
            }
        }
    }

    return caps;
}

/** BEGIN: Added by liurongliang(l00180035), 2014/4/2 */
/*由caps 填充pstVideoInfo，如果是ST_SME_CFG_DATA不为空的话那么需要外部释放*/
gboolean
gst_sme_caps_to_videoinfo (const GstCaps * caps, ST_SME_VIDEOINFO* pstVideoInfo,
    gboolean* bEs)
{
    gboolean bRet = TRUE;
    const gchar *mimetype = NULL;
    const GstStructure *structure = NULL;
    //guint mimelen = 0;
    ST_SME_MEDIAINFO* pstMediaInfo = NULL;
    E_SME_MEM_SECURE_RET eReturn;
    E_VOS_STR_ERR eErr = E_VOS_STR_ENONE;

    g_return_val_if_fail (caps != NULL, FALSE);
    g_return_val_if_fail (bEs != NULL, FALSE);
    g_return_val_if_fail (pstVideoInfo != NULL, FALSE);
    g_return_val_if_fail (gst_caps_get_size (caps) == 1, FALSE);

    eReturn = VOS_Memset_S(pstVideoInfo, sizeof(ST_SME_MEDIAINFO), 0, sizeof(ST_SME_MEDIAINFO));
    if(E_SME_MEM_OK != eReturn){
        GST_WARNING("VOS_Memset_S pstVideoInfo Failed(%d)", eReturn);
    }
    pstMediaInfo = &pstVideoInfo->stBase;

    structure = gst_caps_get_structure (caps, 0);
    mimetype = gst_structure_get_name (structure);
    if(NULL == mimetype){
        return FALSE;
    }

    GST_INFO_OBJECT(pstVideoInfo, "mimetype:%s", mimetype);
    //mimelen = (guint)strlen(mimetype);
    //mimelen = MIN(mimelen, sizeof(pstMediaInfo->acMime) - 1);

    *bEs = TRUE;
    if (!strcmp (mimetype, "video/x-raw")) {
        *bEs = FALSE;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy video/x-raw failed(%d)", eErr);
            bRet = FALSE;
        }
    } else if (!strcmp (mimetype, "video/x-h263")) {
        pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_H263;
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_VIDEO;
        pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('H', '2', '6' , '3');
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy video/x-h263 failed(%d)", eErr);
            bRet = FALSE;
        }
    }
    else if (!strcmp (mimetype, "video/mpeg"))
    {
        gboolean sys_strm;
        gint mpegversion;
        pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_MPEG4;
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_VIDEO;
        pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('M', 'P', '4' , 'V');
        if (gst_structure_get_boolean (structure, "systemstream", &sys_strm) &&
            gst_structure_get_int (structure, "mpegversion", &mpegversion) &&
            !sys_strm) {
            switch (mpegversion) {
                case 1:
                    pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_MPEG1VIDEO;
                    pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('M', 'P', '1' , 'V');
                    break;
                case 2:
                    pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_MPEG2VIDEO;
                    pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('M', 'P', '2' , 'V');
                    break;
                case 4:
                    break;
                default:
                    bRet = FALSE;
                    break;
            }
        }
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }
    } else if (!strcmp (mimetype, "image/jpeg")) {
        pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_JPEG;
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_IMG;
        pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('J', 'P', 'E' , 'G');
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }
    } else if (!strcmp (mimetype, "audio/x-vorbis")) {
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_VORBIS;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('V', 'B', 'I' , 'S');
        }
    } else if (!strcmp (mimetype, "video/x-divx")) {
        gint divxversion = 0;
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_VIDEO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
          if (gst_structure_get_int (structure, "divxversion", &divxversion)) {
              switch (divxversion) {
                case 4:
                case 5:
                    pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_MPEG4;
                    pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('D', 'I', 'V' , 'X');
                  break;
                default:
                  bRet = FALSE;
                  break;
              }
          }
        }
    } else if (!strcmp (mimetype, "video/x-3ivx")) {
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_VIDEO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_MPEG4;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('3', 'I', 'V' , 'X');
        }
    } else if (!strcmp (mimetype, "video/x-xvid")) {
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_VIDEO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_MPEG4;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('X', 'V', 'I' , 'D');
        }
    } else if (!strcmp (mimetype, "video/x-h264")) {
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_VIDEO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_H264;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('H', '2', '6' , '4');
            GST_INFO_OBJECT(pstVideoInfo, "pstMediaInfo->acMime=%s", pstMediaInfo->acMime);
        }
    } else if (!strcmp (mimetype, "video/x-h265")) {
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_VIDEO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_HEVC;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('H', '2', '6' , '5');
            GST_INFO_OBJECT(pstVideoInfo, "pstMediaInfo->acMime=%s", pstMediaInfo->acMime);
        }
    }else if(!strcmp (mimetype, "video/x-wmv")){
        gint wmvversion = 0;
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_VIDEO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
          if (gst_structure_get_int (structure, "wmvversion", &wmvversion)) {
              switch (wmvversion) {
                case 3:
                    pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_VC1;
                    pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('V', 'C', '1' , ' ');
                  break;
                default:
                    GST_INFO("wmvversion=%d unsupport!", wmvversion);
                    bRet = FALSE;
                    break;
              }
          }
        }
    }else{
        bRet = FALSE;
    }

    if(bRet){
        const GValue *value;
        const GValue *fps;

        /* extradata parsing (esds [mpeg4], wma/wmv, msmpeg4v1/2/3, etc.) */
        value = gst_structure_get_value (structure, "codec_data");
        if (NULL != value) {
            GstMapInfo map;
            GstBuffer *buf = NULL;

            buf = gst_value_get_buffer (value);
            gst_buffer_map (buf, &map, GST_MAP_READ);

            if(map.size > 0){
                /* free the old one if it is there */
                if (NULL != pstMediaInfo->stCfgData.pu8CfgData){
                    free(pstMediaInfo->stCfgData.pu8CfgData);
                    pstMediaInfo->stCfgData.pu8CfgData = NULL;
                    pstMediaInfo->stCfgData.u32CfgDataSize = 0;
                }

                /* allocate with enough padding */
                GST_DEBUG ("copy codec_data");
                pstMediaInfo->stCfgData.pu8CfgData = (V_UINT8*)malloc (map.size);
                if(NULL != pstMediaInfo->stCfgData.pu8CfgData){
                    eReturn = VOS_Memcpy_S(pstMediaInfo->stCfgData.pu8CfgData,
                      map.size, map.data, map.size);
                    if(E_SME_MEM_OK != eReturn){
                        GST_WARNING("VOS_Memcpy_S pstAudioInfo Failed(%d)", eReturn);
                    }

                    pstMediaInfo->stCfgData.u32CfgDataSize = map.size;
                }
                else{
                    GST_WARNING ("malloc cfg data failed, size %" G_GSIZE_FORMAT, map.size);
                }
                GST_DEBUG ("have codec data of size %" G_GSIZE_FORMAT, map.size);
            }
            gst_buffer_unmap (buf, &map);
        }

        fps = gst_structure_get_value (structure, "framerate");
        if (fps != NULL) {
            pstVideoInfo->i32FpsNum = gst_value_get_fraction_numerator (fps);
            pstVideoInfo->i32FpsDen = gst_value_get_fraction_denominator (fps);
            GST_INFO_OBJECT (pstVideoInfo, "caps have framerate of %d/%d",
                pstVideoInfo->i32FpsNum, pstVideoInfo->i32FpsDen);
        } else {
            GST_WARNING_OBJECT (pstVideoInfo, "no input framerate ");
            pstVideoInfo->i32FpsNum = 0;
            pstVideoInfo->i32FpsDen = 1;
        }
        if (!gst_structure_get_int (structure, "width", &pstVideoInfo->stVideoRes.i32Width)){
            pstVideoInfo->stVideoRes.i32Width = -1;
        }

        if (!gst_structure_get_int (structure, "height", &pstVideoInfo->stVideoRes.i32Height)){
            pstVideoInfo->stVideoRes.i32Height = -1;
        }
        GST_DEBUG_OBJECT (pstVideoInfo, "clipping to %dx%d",
            pstVideoInfo->stVideoRes.i32Width, pstVideoInfo->stVideoRes.i32Height);
    }

    return bRet;
}

/*
E_SME_AUDIO_RAW_FMT
gst_sme_caps_to_smpfmt (const GstCaps * caps)
{
    GstStructure *structure = NULL;
    GstAudioFormat format = GST_AUDIO_FORMAT_UNKNOWN;
    E_SME_AUDIO_RAW_FMT e_smpfmt = E_SME_AUDIO_RAW_FMT_PCM_BUTTON;
    gboolean bRet;

    g_return_val_if_fail (1 == gst_caps_get_size (caps), E_SME_AUDIO_RAW_FMT_PCM_BUTTON);

    structure = gst_caps_get_structure (caps, 0);

    bRet = gst_structure_has_name (structure, "audio/x-raw");
    if (bRet) {
        const gchar *fmt;
        fmt = gst_structure_get_string(structure, "format");
        if (fmt) {
          format = gst_audio_format_from_string (fmt);
        }
    }

    switch (format) {
        case GST_AUDIO_FORMAT_F32:
            e_smpfmt = E_SME_AUDIO_RAW_FMT_PCM_FLT;
            break;
        case GST_AUDIO_FORMAT_F64:
            e_smpfmt = E_SME_AUDIO_RAW_FMT_PCM_DBL;
            break;
        case GST_AUDIO_FORMAT_S32:
            e_smpfmt = E_SME_AUDIO_RAW_FMT_PCM_S32BIT;
            break;
        case GST_AUDIO_FORMAT_S16:
            e_smpfmt = E_SME_AUDIO_RAW_FMT_PCM_S16BIT;
            break;
        default:
            break;
    }

    return e_smpfmt;
}
*/


gboolean
gst_sme_caps_to_audioinfo (const GstCaps * caps, ST_SME_AUDIOINFO* pstAudioInfo, gboolean* bEs)
{
    gboolean bRet = TRUE;
    const gchar *mimetype = NULL;
    const gchar *fmtStr = NULL;
    const GstStructure *structure = NULL;
    //guint mimelen = 0;
    ST_SME_MEDIAINFO* pstMediaInfo = NULL;
    E_SME_MEM_SECURE_RET eReturn;
    E_VOS_STR_ERR eErr = E_VOS_STR_ENONE;

    g_return_val_if_fail (caps != NULL, FALSE);
    g_return_val_if_fail (bEs != NULL, FALSE);
    g_return_val_if_fail (pstAudioInfo != NULL, FALSE);
    g_return_val_if_fail (gst_caps_get_size (caps) == 1, FALSE);

    eReturn = VOS_Memset_S(pstAudioInfo, sizeof(ST_SME_AUDIOINFO), 0, sizeof(ST_SME_AUDIOINFO));
    if(E_SME_MEM_OK != eReturn){
        GST_WARNING("VOS_Memset_S pstAudioInfo Failed(%d)", eReturn);
    }
    pstMediaInfo = &pstAudioInfo->stBase;
    structure = gst_caps_get_structure (caps, 0);
    mimetype = gst_structure_get_name (structure);
    if(NULL == mimetype)
    {
        return FALSE;
    }

    //mimelen = (guint)strlen(mimetype);
    //mimelen = MIN(mimelen, sizeof(pstMediaInfo->acMime) - 1);

    *bEs = TRUE;
    if (!strcmp (mimetype, "audio/x-raw")) {
        *bEs = FALSE;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            return FALSE;
        }
        fmtStr = gst_structure_get_string(structure, "format");
        if(NULL == fmtStr){
            bRet = FALSE;
            return bRet;
        }
        GST_WARNING("gst_sme_caps_to_audioinfo, Fmt:%s" , fmtStr);
        if (!strcmp (fmtStr, "S8")|| !strcmp (fmtStr, "U8")){
            pstAudioInfo->eSampleFmt = E_SME_AUDIO_RAW_FMT_PCM_8BIT;
        }else if(!strcmp (fmtStr, "S16LE")|| !strcmp (fmtStr, "S16BE")){
            pstAudioInfo->eSampleFmt = E_SME_AUDIO_RAW_FMT_PCM_S16BIT;
        }else if(!strcmp (fmtStr, "S32LE")|| !strcmp (fmtStr, "S32BE")){
            pstAudioInfo->eSampleFmt = E_SME_AUDIO_RAW_FMT_PCM_S32BIT;
        }else if(!strcmp (fmtStr, "F32LE")|| !strcmp (fmtStr, "F32BE")){
            pstAudioInfo->eSampleFmt = E_SME_AUDIO_RAW_FMT_PCM_FLT;
        }else{
            GST_WARNING("unknown Fmt , Fmt:%s" , fmtStr);
        }
    }
    else if (!strcmp (mimetype, "audio/mpeg"))
    {
        gint layer = 0;
        gint mpegversion = 0;
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            if (gst_structure_get_int (structure, "mpegversion", &mpegversion)) {
                switch (mpegversion) {
                    case 2:                /* ffmpeg uses faad for both... */
                    case 4:
                        pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('A', 'A', 'C' , ' ');
                        pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_AAC;
                        break;
                    case 1:
                        if (gst_structure_get_int (structure, "layer", &layer)) {
                            switch (layer) {
                                case 1:
                                    pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('M', 'P', '1' , 'A');
                                    pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_MP3;
                                    break;
                                case 2:
                                    pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('M', 'P', '2' , 'A');
                                    pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_MP2;
                                    break;
                                case 3:
                                    pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('M', 'P', '3' , 'A');
                                    pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_MP3;
                                    break;
                                default:
                                    bRet = FALSE;
                                    break;
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }else if (!strcmp (mimetype, "audio/AMR-NB")) {
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_AMR_NB;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('A', 'M', 'R' , 'N');
        }
    } else if (!strcmp (mimetype, "audio/AMR-WB")) {
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_AMR_WB;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('A', 'M', 'R' , 'W');
        }
    } else if (!strcmp (mimetype, "audio/x-vorbis")) {
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_VORBIS;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('V', 'O', 'B', 'S');
        }
    } else if (!strcmp (mimetype, "audio/flac")) {
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_FLAC;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('F', 'L', 'A', 'C');
        }
    } else if (!strcmp (mimetype, "audio/ac3")) {
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_AC3;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('A', 'C', '3', ' ');
        }
    } else if (!strcmp (mimetype, "audio/eac3")) {
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_EAC3;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('E','A', 'C', '3');
        }
    } else if (!strcmp (mimetype, "audio/dra")) {
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_DRA;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('D', 'R', 'A', '1');
        }
    } else if (!strcmp(mimetype, "audio/x-wma")) {
        gint wmaversion = 0;
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            if (gst_structure_get_int (structure, "wmaversion", &wmaversion)) {
                switch (wmaversion) {
                    case 1:
                        pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_WMAV1;
                        pstMediaInfo->u32CodecForcc =
                            GST_MAKE_FOURCC('W','M', 'A', '1');
                        break;
                    case 2:
                        pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_WMAV2;
                        pstMediaInfo->u32CodecForcc =
                            GST_MAKE_FOURCC('W','M', 'A', '2');
                        break;
                    case 3:
                        pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_WMAVpro;
                        pstMediaInfo->u32CodecForcc =
                            GST_MAKE_FOURCC('W','M', 'A', 'p');
                        break;
                    case 4:
                        pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_WMAVLOSSLESS;
                        pstMediaInfo->u32CodecForcc =
                            GST_MAKE_FOURCC('W','M', 'A', 'L');
                        break;
                    default:
                        break;
                }
            }
        }
    } else if (!strcmp (mimetype, "audio/x-pcm")){
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
            return bRet;
        }
        fmtStr = gst_structure_get_string(structure, "format");
        if(NULL == fmtStr){
            bRet = FALSE;
            return bRet;
        }
        GST_WARNING("gst_sme_caps_to_audioinfo, Fmt:%s" , fmtStr);
        if (!strcmp (fmtStr, "S16LE")){
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_PCM_S16LE;
        }else if(!strcmp (fmtStr, "MULAW")){
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_PCM_MULAW;
        }else if(!strcmp (fmtStr, "ALAW")){
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_PCM_ALAW;
        }else if (!strcmp (fmtStr, "S32LE")){
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_PCM_S32LE;
        }else if (!strcmp (fmtStr, "S24LE")){
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_PCM_S24LE;
        }else if (!strcmp (fmtStr, "U8")){
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_PCM_U8;
        }else if (!strcmp (fmtStr, "S16BE")){
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_PCM_S16BE;
        }else if (!strcmp (fmtStr, "S24BE")){
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_PCM_S24BE;
        }else if (!strcmp (fmtStr, "F32BE")){
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_PCM_F32BE;
        }else if (!strcmp (fmtStr, "F32LE")){
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_PCM_F32LE;
        }else{
            GST_WARNING("unknown Fmt , Fmt:%s" , fmtStr);
        }
        pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('W','A', 'V', ' ');
    } else if (!strcmp (mimetype, "audio/pcm-dvd")){
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_PCM_DVD;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('P', 'C', 'M', 'D');
        }
    } else if (!strcmp (mimetype, "audio/pcm-bluray")){
        pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_PCM_BLURAY;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('P', 'C', 'M', 'B');
        }
#ifdef __DDP_PT__
    } else if (!strcmp (mimetype, "audio/bitstream")) {
        *bEs = FALSE;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
            return bRet;
        }
        fmtStr = gst_structure_get_string(structure, "format");
        if(NULL == fmtStr){
            bRet = FALSE;
            return bRet;
        }
        GST_WARNING("gst_sme_caps_to_audioinfo, Fmt:%s" , fmtStr);
        if (!strcmp (fmtStr, "ac3")){
            GST_WARNING("E_SME_AUDIO_RAW_FMT_PT_AC3, Fmt:%s" , fmtStr);
            pstAudioInfo->eSampleFmt = E_SME_AUDIO_RAW_FMT_PT_AC3;
        }else if (!strcmp (fmtStr, "eac3")) {
            GST_WARNING("E_SME_AUDIO_RAW_FMT_PT_E_AC3 , Fmt:%s" , fmtStr);
            pstAudioInfo->eSampleFmt = E_SME_AUDIO_RAW_FMT_PT_E_AC3;
        } else {
            GST_WARNING("unknown Fmt , Fmt:%s" , fmtStr);
        }

#endif
    } else if (!strcmp (mimetype, "audio-b/bitstream")) {
        *bEs = TRUE;
        eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
            return bRet;
        }
        fmtStr = gst_structure_get_string(structure, "format");
        if(NULL == fmtStr){
            bRet = FALSE;
            return bRet;
        }
        GST_WARNING("gst_sme_caps_to_audioinfo, Fmt:%s" , fmtStr);
        if (!strcmp (fmtStr, "ac3")){
            GST_WARNING("ac3 Fmt , Fmt:%s" , fmtStr);
        }else{
            GST_WARNING("unknown Fmt , Fmt:%s" , fmtStr);
        }
    } else if (!strcmp (mimetype, "audio/x-adpcm_ima_wav")) {
         pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
         eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_ADPCM_IMA_WAV;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('W','A', 'V', ' ');
        }
    } else if (!strcmp (mimetype, "audio/x-adpcm_ms")) {
         pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
         eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_ADPCM_MS;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('W','A', 'V', ' ');
        }
    } else if (!strcmp (mimetype, "audio/x-gsm")) {
         pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
         eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_GSM;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('W','A', 'V', ' ');
        }
    } else if (!strcmp (mimetype, "audio/x-gsm_ms")) {
         pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
         eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_GSM_MS;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('W','A', 'V', ' ');
        }
    } else if (!strcmp (mimetype, "audio/x-dts")) {
         pstMediaInfo->eMediaType = E_SME_MEDIA_TYPE_AUDIO;
         eErr = VOS_Strncpy_S(pstMediaInfo->acMime, sizeof(pstMediaInfo->acMime), mimetype,
            (guint)strlen(mimetype));
        if(eErr != E_VOS_STR_ENONE){
            GST_WARNING("copy %s failed(%d)", mimetype, eErr);
            bRet = FALSE;
        }else{
            pstMediaInfo->eCodecId = E_SME_MEDIA_CODEC_ID_DTS;
            pstMediaInfo->u32CodecForcc = GST_MAKE_FOURCC('W','A', 'V', ' ');
        }
    }
    else {
        bRet = FALSE;
    }

    if(bRet){
        const GValue *value = NULL;

        /* extradata parsing (esds [mpeg4], wma/wmv, msmpeg4v1/2/3, etc.) */
        value = gst_structure_get_value (structure, "codec_data");
        if (value) {
            GstMapInfo map;
            GstBuffer *buf = NULL;

            buf = gst_value_get_buffer (value);
            gst_buffer_map (buf, &map, GST_MAP_READ);

            if(map.size > 0){
                /* free the old one if it is there */
                if (NULL != pstMediaInfo->stCfgData.pu8CfgData){
                    free(pstMediaInfo->stCfgData.pu8CfgData);
                    pstMediaInfo->stCfgData.pu8CfgData = NULL;
                    pstMediaInfo->stCfgData.u32CfgDataSize = 0;
                }

                /* allocate with enough padding */
                GST_DEBUG ("copy codec_data");
                pstMediaInfo->stCfgData.pu8CfgData = (V_UINT8*)malloc(map.size);
                if(NULL != pstMediaInfo->stCfgData.pu8CfgData){
                    eReturn = VOS_Memcpy_S(pstMediaInfo->stCfgData.pu8CfgData,
                      map.size, map.data, map.size);
                    if(E_SME_MEM_OK != eReturn){
                        GST_WARNING("VOS_Memcpy_S stCfgData.pu8CfgData Failed(%d)", eReturn);
                    }

                    pstMediaInfo->stCfgData.u32CfgDataSize = map.size;
                }
                else{
                    GST_WARNING ("malloc cfg data failed, size %" G_GSIZE_FORMAT, map.size);
                }
                GST_DEBUG ("have codec data of size %" G_GSIZE_FORMAT, map.size);
            }
            gst_buffer_unmap (buf, &map);
        }
        (void)gst_structure_get_int (structure, "channels",
            (gint*)&pstAudioInfo->u32Channels);
        (void)gst_structure_get_int (structure, "rate",
            (gint*)&pstAudioInfo->u32SampleRate);
        (void)gst_structure_get_int (structure, "bitrate",
            (gint*)&pstAudioInfo->stBase.u32Bps);
        (void)gst_structure_get_int (structure, "block_align",
            (gint*)&pstAudioInfo->u32BlockAlign);
        (void)gst_structure_get_int (structure, "sample_fmt",
            (gint*)&pstAudioInfo->eSampleFmt);
        (void)gst_structure_get_int (structure, "bits_per_codec_sample",
            (gint*)&pstAudioInfo->u32BitsPerCodecSample);

        GST_WARNING("pstAudioInfo->eSampleFmt:%d" , pstAudioInfo->eSampleFmt);
    }

    return bRet;
}

gint
sme_smp_format_to_depth (E_SME_AUDIO_RAW_FMT samplefmt)
{
    gint depth = -1;
    switch (samplefmt) {
        case E_SME_AUDIO_RAW_FMT_PCM_8BIT:
            depth = 1;
            break;
        case E_SME_AUDIO_RAW_FMT_PCM_S16BIT:
            depth = 2;
            break;
        case E_SME_AUDIO_RAW_FMT_PCM_S32BIT:
        case E_SME_AUDIO_RAW_FMT_PCM_FLT:
            depth = 4;
            break;
        case E_SME_AUDIO_RAW_FMT_PCM_DBL:
            depth = 8;
            break;
        default:
            GST_ERROR ("UNHANDLED SAMPLE FORMAT !");
            break;
    }
    return depth;
}
/** END:   Added by liurongliang(l00180035), 2014/4/2 */
