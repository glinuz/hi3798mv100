/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmevideosink.c
 * @brief   gstvideosink实现
 * @author
 * @date    2014/4/14
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/4/14
 * Author :
 * Desc   : Created file
 *
******************************************************************************/
#include "osal_mem.h"
#include "gstsmeaudiodec.h"
#ifdef __DDP_PT__
#include <unistd.h>
#endif

//lint -e585 //(可变参数...报错，为gstreamer中的日志宏，无法修改。)
//lint -e655 //位操作使用（兼容的）枚举类型
//lint -e717 //do{}while(0)
//lint -e826 //不等价指针转换，这里为安全转换
//lint -e838 //增加指针容量（Context）
//lint -e801 //goto, 这里可以使用goto,gstreamer里面也是这么使用的。
//lint -e774 //new 判断为空报，始终不为空,gstreamer 宏无法修改。
//lint -e64  //赋值类型不匹配，这里都是GstCaps*之间的赋值，没有类型转换

enum
{
  PROP_0,
  PROP_ADEC_INFO
};


#define GST_SME_AUDIODEC_PARAMS_QDATA               \
    g_quark_from_static_string("smeaduiodec-params")
#define GST_SME_AUDIODEC_LIBTYPE_QDATA              \
    g_quark_from_static_string("smeaduiodec-libtype")
#define FF_INPUT_BUFFER_PADDING_SIZE 8
#define GST_MAX_ADEC_ERROR 30

static GstElementClass *parent_class = NULL;
static const GstSmeTSInfo sme_ts_info_none = {(gint)-1, (GstClockTime)-1, (GstClockTime)-1, (gint64)-1};

static void gst_sme_audiodec_base_init (GstSmeAudioDecoderClass* klass);

static void
gst_smeaudiodecoder_finalize (GObject * object);

static void gst_sem_audiodecoder_set_property (GObject * object,
    guint prop_id, const GValue * value, GParamSpec * pspec);

static void gst_sem_audiodecoder_get_property (GObject * object,
    guint prop_id, GValue * value, GParamSpec * pspec);

static GstStateChangeReturn
    gst_sem_audiodecoder_change_state (GstElement * element, GstStateChange transition);

static void
gst_sme_audiodec_class_init (GstSmeAudioDecoderClass * klass);

static gboolean
gst_sme_audiodec_setcaps (GstSmeAudioDecoder *smeaudiodecoder, const GstCaps * caps);

static void
gst_sme_audiodecoder_drain (GstSmeAudioDecoder *smeaudiodecoder);

static gboolean
gst_sme_audiodecoder_sink_event (GstPad * pad, GstObject * parent, GstEvent * event);

static gboolean
gst_sme_audiodecoder_sink_query (GstPad * pad, GstObject * parent, GstQuery * query);


static void gst_sem_audiodec_update_qos (GstSmeAudioDecoder *smeaudiodecoder, gdouble proportion,
    GstClockTime timestamp);

static gboolean
gst_sme_audiodecoder_src_event (GstPad * pad, GstObject * parent, GstEvent * event);

static gboolean
gst_sme_audiodecoder_src_query (GstPad * pad, GstObject * parent, GstQuery * query);

static gboolean
gst_sem_audiodecoder_negotiate (GstSmeAudioDecoder * smeaudiodecoder, gboolean force);

static const GstSmeTSInfo *
gst_sme_ts_info_store (GstSmeAudioDecoder * dec, GstClockTime timestamp,
    GstClockTime duration, gint64 offset);

static gint
gst_sme_audio_decode (GstSmeAudioDecoder * smeaudiodecoder, guint8 * data, guint size,
    const GstSmeTSInfo * dec_info, GstBuffer ** outbuf, GstFlowReturn * ret, gint* got_data);

static gint
gst_sme_audiodec_frame (GstSmeAudioDecoder * smeaudiodecoder,
    guint8 * data, guint size, gint * got_data, const GstSmeTSInfo * dec_info,
    GstFlowReturn * ret);

static GstFlowReturn
gst_sme_audiodecoder_chain (GstPad * pad, GstObject * parent, GstBuffer * inbuf);

static void
gst_sme_audiodec_init (GstSmeAudioDecoder * smeaudiodecoder);

static void*
sme_allocator_lock_buf(gpointer gpctx, guint u32bufSize, gint i32stream_idx);

static void
sme_allocator_unlock_buf(gpointer gpctx, gpointer gpdata_buf);

static gboolean
gst_sme_audiodec_sink_pad_negotiate_pull (GstSmeAudioDecoder *smeaudiodecoder);

static gboolean
gst_sme_audiodec_sink_activate_pull(GstPad * pad, GstObject * parent,
                                gboolean active);
static gboolean
gst_sme_audiodec_sink_activate_push (GstPad * pad, GstObject * parent,
                                    gboolean active);

static gboolean
gst_sme_audiodec_sink_pad_activate(GstPad * pad, GstObject * parent);

static gboolean
gst_sme_audiodec_sink_pad_activatemode(GstPad *pad, GstObject *parent,
                                 GstPadMode mode, gboolean active);

static void
gst_sme_audiodec_sink_loop (GstPad * pad);

gboolean
gst_smeaudiodecoder_register (GstPlugin * plugin);

static ST_SME_AUDIO_DEC_INF
*gst_sme_audiodec_getinf(E_SME_ADEC_LIB_TYPE adeclibtype);

typedef struct _GstDecAllocatorCtx{
    GstBuffer* pstbuffer;
    GstMapInfo stmap_info;
}GstDecAllocatorCtx;

#ifdef SMEPLAYER_BENCHMARK_OPEN
static void gst_audio_dump_sink_file(GstSmeAudioDecoder * dec, const char * data, int len)
{
    if (dec->dump_sink_flag) {
        if (NULL == dec->dump_sink_file) {
            dec->dump_sink_file = fopen(dec->dump_sink_file_name, "wb");
        }
        if (dec->dump_sink_file) {
            if (fwrite(data, 1, len, dec->dump_sink_file) != len) {
                GST_WARNING_OBJECT(dec,"gst_audio_dump_sink_file error len = %d!",len);
            }
        }
    } else {
        if (dec->dump_sink_file) {
            fclose(dec->dump_sink_file);
            dec->dump_sink_file = NULL;
        }
    }
}

static void gst_audio_dump_src_file(GstSmeAudioDecoder * dec, const char * data, int len)
{
    if (dec->dump_src_flag) {
        if (NULL == dec->dump_src_file) {
            dec->dump_src_file = fopen(dec->dump_src_file_name, "wb");
        }
        //GST_WARNING_OBJECT(dec,"gst_audio_dump_src_file %p!",dec->dump_src_file);
        if (dec->dump_src_file) {
            if (fwrite(data, 1, len, dec->dump_src_file) != len) {
                GST_WARNING_OBJECT(dec,"gst_audio_dump_src_file error len = %d!",len);
            }
        }
    } else {
        if (dec->dump_src_file) {
            fclose(dec->dump_src_file);
            dec->dump_src_file = NULL;
        }
    }
}

#endif


static void
gst_sme_audiodec_base_init (GstSmeAudioDecoderClass * klass)
{
    GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
    GstPadTemplate *sinktempl = NULL;
    GstPadTemplate *srctempl = NULL;
    GstCaps *sinkcaps = NULL;
    GstCaps *srccaps = NULL;
    SMECODECFMT codecfmt = NULL;
    gchar *codec_name = NULL;
    const E_SME_AUDIO_RAW_FMT *samplefmts = NULL;
    E_SME_MEDIA_CODEC_ID  codecid = E_SME_MEDIA_CODEC_ID_NONE;
    ST_SME_AUDIO_DEC_INF *smeaudiodecinf = NULL;
    ST_SME_AUDIOINFO smemediainfo;
    gchar *longname = NULL;
    gchar *description = NULL;
    gchar* caps_tostring = NULL;
    E_SME_ADEC_LIB_TYPE adeclibtype = E_SME_ADEC_LIB_MAX;
    E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;

    GST_INFO("gst_sme_audiodec_base_init");

    if(NULL == element_class){
        GST_ERROR("element_class is NULL");
        return;
    }

    codecfmt = (SMECODECFMT) g_type_get_qdata (G_OBJECT_CLASS_TYPE (klass),
        GST_SME_AUDIODEC_PARAMS_QDATA);
    g_assert (codecfmt != NULL);

    adeclibtype = GPOINTER_TO_INT(g_type_get_qdata(
                                      G_OBJECT_CLASS_TYPE(klass),
                                      GST_SME_AUDIODEC_LIBTYPE_QDATA));

    smeaudiodecinf = gst_sme_audiodec_getinf(adeclibtype);
    if (NULL == smeaudiodecinf) {
        GST_ERROR("can't get audio dec information");
        return;
    }

    codec_name = (gchar *)smeaudiodecinf->stBase.SME_GetCodecName(codecfmt);
    longname = g_strdup_printf ("sme %s decoder", codec_name);
    if(NULL == longname){
        GST_ERROR("malloc longname failed");
        return;
    }

    description = g_strdup_printf ("sme %s decoder", codec_name);
    if(NULL == description){
        GST_ERROR("malloc description failed");
        g_free (longname);
        return;
    }
    gst_element_class_set_details_simple (element_class, longname, "Codec/Decoder/Audio",
      description,"anonymous author");
    g_free (longname);
    g_free (description);

    codecid = smeaudiodecinf->stBase.SME_GetCodecId(codecfmt);
    samplefmts = smeaudiodecinf->stBase.SME_GetSampleFmts(codecfmt);

    eMemSecRet = VOS_Memset_S((void*)(&smemediainfo),sizeof(smemediainfo),0,sizeof(smemediainfo));
    if(eMemSecRet != E_SME_MEM_OK)
    {
        GST_ERROR("VOS_Memset_S failed:%d", eMemSecRet);
    }
    smemediainfo.u32Channels = (V_UINT32)-1;

    GST_INFO("codecid=%d,samplefmts=%p",codecid,samplefmts);

    /* get the caps */
    smemediainfo.stBase.eMediaType = E_SME_MEDIA_TYPE_AUDIO;
    sinkcaps = gst_sme_codecid_to_caps (codecid,(ST_SME_MEDIAINFO *)&smemediainfo,FALSE);
    if (!sinkcaps) {
        sinkcaps = gst_caps_from_string ("unknown/unknown");
    }
    caps_tostring = gst_caps_to_string(sinkcaps);
    GST_INFO("gst sme audio sinkpad caps=%s", caps_tostring);
    g_free(caps_tostring);

    srccaps = gst_sme_codectype_to_audio_caps (codecid,
        (ST_SME_MEDIAINFO *)(&smemediainfo), FALSE,
        (E_SME_AUDIO_RAW_FMT *)samplefmts);
    if (!srccaps) {
        srccaps = gst_caps_from_string ("unknown/unknown");
    }
    caps_tostring = gst_caps_to_string(srccaps);
    GST_INFO("gst sme audio srcpad caps=%s", caps_tostring);
    g_free(caps_tostring);

    /* pad templates */
    sinktempl = gst_pad_template_new ("sink", GST_PAD_SINK,GST_PAD_ALWAYS, sinkcaps);
    if(NULL == sinktempl){
        GST_ERROR("creat sinktempl failed");
        g_free(smemediainfo.stBase.stCfgData.pu8CfgData);
        return;
    }
    srctempl = gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS, srccaps);
    if(NULL == srctempl){
        GST_ERROR("creat srctempl failed");
        g_free(smemediainfo.stBase.stCfgData.pu8CfgData);
        return;
    }

    gst_element_class_add_pad_template (element_class, srctempl);
    gst_element_class_add_pad_template (element_class, sinktempl);

    klass->codecfmt = codecfmt;
    klass->srctempl = srctempl;
    klass->sinktempl = sinktempl;

    return;
}

static void
gst_smeaudiodecoder_finalize (GObject * object)
{
    GstSmeAudioDecoder *smeaudiodecoder = (GstSmeAudioDecoder *) object;

    if(NULL == smeaudiodecoder){
        GST_ERROR("gst_smeaudiodecoder_finalize args object is null");
        return;
    }

#ifdef SMEPLAYER_BENCHMARK_OPEN
    smeaudiodecoder->audio_decoder_info.ADecGetFirstIFrameTimeFlag = 0;
    smeaudiodecoder->audio_decoder_info.ADecFirstPlayTimeFlag = 0;
    smeaudiodecoder->audio_decoder_info.ADecErrorCnt = 0;
#endif

    G_OBJECT_CLASS (parent_class)->finalize (object);

    return;
}

static void
gst_sem_audiodecoder_set_property (GObject * object,
    guint prop_id, const GValue * value, GParamSpec * pspec)
{
  GST_INFO("gst_sem_audiodecoder_set_property");
#ifdef SMEPLAYER_BENCHMARK_OPEN

  GstSmeAudioDecoder *dec;
  E_SME_MEM_SECURE_RET eMemSecRet;

  dec = (GstSmeAudioDecoder *)(object);
#endif

  switch (prop_id) {
    case PROP_0:
        break;
#ifdef SMEPLAYER_BENCHMARK_OPEN
    case PROP_ADEC_INFO:
    {
      ST_AUDIODEC_PROC_INFO *pstAdecProcInfo;
      ST_ADEC_CMD_INFO *pstAdecCmdInfo;

      pstAdecProcInfo = g_value_get_pointer (value);

      if (pstAdecProcInfo && pstAdecProcInfo->eProcCmdType == E_SME_PROC_CMD_SHOW_INFO) {
        ST_ADEC_SHOW_INFO *pstAdecShowInfo;
        pstAdecShowInfo = &(pstAdecProcInfo->stShowInfo);

        eMemSecRet = VOS_Memcpy_S(pstAdecShowInfo,
                                    sizeof(dec->audio_decoder_info),
                                    &dec->audio_decoder_info,
                                    sizeof(dec->audio_decoder_info));
        if(eMemSecRet != E_SME_MEM_OK) {
            GST_ERROR("VOS_Memcpy_S failed:%d", eMemSecRet);
        }

        GST_INFO("Collect audio decoder info for proc");
      }
      else if (pstAdecProcInfo && pstAdecProcInfo->eProcCmdType == E_SME_PROC_CMD_SET_CMD)
      {
          pstAdecCmdInfo = &(pstAdecProcInfo->stCmdInfo);
          GST_WARNING("Audio Decoder bDumpSinkStream=%d", pstAdecCmdInfo->bDumpSinkStream);
          GST_WARNING("Audio Decoder bDumpSrcStream=%d", pstAdecCmdInfo->bDumpSrcStream);
          GST_WARNING("Audio Decoder cDumpFileStorePath=%s", pstAdecCmdInfo->cDumpFileStorePath);
          if (pstAdecCmdInfo->bDumpSinkStream != -1) {
              if (pstAdecCmdInfo->bDumpSinkStream == 1) {
                //begin to start dump sink data;
                struct timeval cur_time;
                gettimeofday(&cur_time, NULL);
                VOS_Snprintf_S(dec->dump_sink_file_name,
                                sizeof(dec->dump_sink_file_name),
                                sizeof(dec->dump_sink_file_name),
                                "%s/adec_%s_in_%d_%d.es",
                                pstAdecCmdInfo->cDumpFileStorePath,
                                GST_ELEMENT_NAME(dec),
                                cur_time.tv_sec,
                                cur_time.tv_usec);
                dec->dump_sink_flag = TRUE;
              } else if(pstAdecCmdInfo->bDumpSinkStream == 0) {
                //stop dump sink data
                dec->dump_sink_flag = FALSE;
              }
          }

          if (pstAdecCmdInfo->bDumpSrcStream != -1) {
              if (pstAdecCmdInfo->bDumpSrcStream == 1) {
                //begin to start dump src data;
                struct timeval cur_time;
                gettimeofday(&cur_time, NULL);
                VOS_Snprintf_S(dec->dump_src_file_name,
                                sizeof(dec->dump_src_file_name),
                                sizeof(dec->dump_src_file_name),
                                "%s/adec_%s_out_%d_%d.pcm",
                                pstAdecCmdInfo->cDumpFileStorePath,
                                GST_ELEMENT_NAME(dec),
                                cur_time.tv_sec,
                                cur_time.tv_usec);
                dec->dump_src_flag = TRUE;
              } else if(pstAdecCmdInfo->bDumpSrcStream == 0) {
                //stop dump src data
                dec->dump_src_flag = FALSE;
              }
          }

      }
      break;
    }
#endif
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }

  return;
}

static void
gst_sem_audiodecoder_get_property (GObject * object,
    guint prop_id, GValue * value, GParamSpec * pspec)
{
    (void)object;
    (void)prop_id;
    (void)value;
    (void)pspec;
    GST_INFO("gst_sem_audiodecoder_get_property");

    return;
}

static GstStateChangeReturn
gst_sem_audiodecoder_change_state (GstElement * element, GstStateChange transition)
{
    GstSmeAudioDecoder *smeaudiodecoder = (GstSmeAudioDecoder *) element;
    GstStateChangeReturn ret = GST_STATE_CHANGE_FAILURE;

    GST_INFO ("gstsmeadec chage_state in:%s to %s,",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)));

    if(NULL == smeaudiodecoder){
        GST_ERROR("gst_sem_audiodecoder_change_state args element is null");
        return ret;
    }

    if(GST_STATE_CHANGE_READY_TO_PAUSED == transition){
        smeaudiodecoder->dec_err_count = 0;
#ifdef SMEPLAYER_BENCHMARK_OPEN
        smeaudiodecoder->audio_decoder_info.ADecErrorCnt = 0;
#endif
    }

    GST_INFO ("gstsmeadec chage_state:%s to %s,begin...",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)));

    ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

    GST_INFO ("gstsmeadec chage_state:%s to %s,end ret=%s",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)),
        gst_element_state_change_return_get_name(ret));

    switch (transition) {
        case GST_STATE_CHANGE_PAUSED_TO_READY:
            GST_OBJECT_LOCK (smeaudiodecoder);

            if((gboolean)TRUE == smeaudiodecoder->opened){
                smeaudiodecoder->smeaudiodecinf->SME_CloseAudioDec(smeaudiodecoder->semcodechdl);
                smeaudiodecoder->smeaudiodecinf->SME_DestroyAudioDec(smeaudiodecoder->semcodechdl);
                g_free(smeaudiodecoder->audioinfo.stBase.stCfgData.pu8CfgData);
                smeaudiodecoder->opened = FALSE;
            }

            gst_caps_replace (&smeaudiodecoder->last_caps, NULL);
            smeaudiodecoder->semcodechdl = NULL;

            GST_OBJECT_UNLOCK (smeaudiodecoder);
            break;
        case GST_STATE_CHANGE_READY_TO_NULL:
#ifdef SMEPLAYER_BENCHMARK_OPEN
          if (smeaudiodecoder->dump_sink_file) {
              fclose(smeaudiodecoder->dump_sink_file);
              smeaudiodecoder->dump_sink_file = NULL;
          }
          smeaudiodecoder->dump_sink_flag = FALSE;

          if (smeaudiodecoder->dump_src_file) {
              fclose(smeaudiodecoder->dump_src_file);
              smeaudiodecoder->dump_src_file = NULL;
          }
          smeaudiodecoder->dump_src_flag = FALSE;
#endif
          break;
        default:
            break;
    }

    GST_INFO ("gstsmeadec chage_state out:%s to %s,end ret=%s",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)),
        gst_element_state_change_return_get_name(ret));

    return ret;
}

static void
gst_sme_audiodec_class_init (GstSmeAudioDecoderClass * klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    GstElementClass *gstelement_class = GST_ELEMENT_CLASS (klass);

    GST_INFO("gst_sme_audiodec_class_init");

    if(NULL == gobject_class){
        GST_ERROR("gst_sme_audiodec_class_init args gobject_class is null");
        return;
    }
    if(NULL == gstelement_class){
        GST_ERROR("gst_sme_audiodec_class_init args gstelement_class is null");
        return;
    }

    parent_class = g_type_class_peek_parent (klass);
    gobject_class->finalize = gst_smeaudiodecoder_finalize;
    gobject_class->set_property = gst_sem_audiodecoder_set_property;
    gobject_class->get_property = gst_sem_audiodecoder_get_property;
    gstelement_class->change_state = gst_sem_audiodecoder_change_state;

#ifdef SMEPLAYER_BENCHMARK_OPEN
    g_object_class_install_property (gobject_class, PROP_ADEC_INFO,
        g_param_spec_pointer ("gst-audio-decoder-info", "Gst_Audio_Decoder_Info",
          "Poniter of Audio Decoder",
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
#endif

}


static gboolean
gst_sme_audiodec_setcaps (GstSmeAudioDecoder *smeaudiodecoder, const GstCaps * caps)
{
    E_SME_ADEC_RET ret = E_SME_ADEC_RET_SUCCESS;
    gboolean bEs = FALSE;
    gboolean return_value = FALSE;
    gchar* caps_tostring = NULL;

    GST_INFO("setcaps called start");
    //g_return_val_if_fail(G_UNLIKELY(NULL != smeaudiodecoder), FALSE);

    GST_OBJECT_LOCK (smeaudiodecoder);
    /* close old session */
    GST_INFO("smeaudiodecoder->opened=%d\n,smeaudiodecoder->semcodechdl:%p",
                smeaudiodecoder->opened,smeaudiodecoder->semcodechdl);
    if (smeaudiodecoder->opened && smeaudiodecoder->semcodechdl) {
        smeaudiodecoder->smeaudiodecinf->SME_CloseAudioDec(smeaudiodecoder->semcodechdl);
        smeaudiodecoder->smeaudiodecinf->SME_DestroyAudioDec(smeaudiodecoder->semcodechdl);
        gst_caps_replace (&smeaudiodecoder->last_caps, NULL);
        smeaudiodecoder->semcodechdl = NULL;
        smeaudiodecoder->opened = FALSE;
    }

    smeaudiodecoder->semcodechdl = smeaudiodecoder->smeaudiodecinf->SME_CreateAudioDec(NULL);
    if(!smeaudiodecoder->semcodechdl){
        GST_ERROR("Failed to creat audio codec");
        GST_OBJECT_UNLOCK (smeaudiodecoder);
        return FALSE;
    }
    /** BEGIN: Modified for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */
    caps_tostring = gst_caps_to_string(caps);
    GST_INFO("sink_cap=%s", caps_tostring);
    g_free(caps_tostring);
    /** END: Modified for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */
    return_value = gst_sme_caps_to_audioinfo (caps, &smeaudiodecoder->audioinfo, &bEs);
    if(!return_value){
        GST_ERROR("Failed set audioinfo from caps");
        smeaudiodecoder->smeaudiodecinf->SME_DestroyAudioDec(smeaudiodecoder->semcodechdl);
        smeaudiodecoder->semcodechdl = NULL;
        GST_OBJECT_UNLOCK (smeaudiodecoder);
        return FALSE;
    }

    GST_INFO("smeaudiodecoder->audioinfo.stBase.stCfgData.pu8CfgData=%p,size=%d",
        smeaudiodecoder->audioinfo.stBase.stCfgData.pu8CfgData,
        smeaudiodecoder->audioinfo.stBase.stCfgData.u32CfgDataSize);
    ret = smeaudiodecoder->smeaudiodecinf->SME_SetAudioInfo(smeaudiodecoder->semcodechdl,
            &(smeaudiodecoder->audioinfo));
    if(E_SME_ADEC_RET_SUCCESS != ret){
        GST_ERROR("Failed set audioinfo to audio decoder");
        smeaudiodecoder->smeaudiodecinf->SME_DestroyAudioDec(smeaudiodecoder->semcodechdl);
        smeaudiodecoder->semcodechdl = NULL;
        GST_OBJECT_UNLOCK (smeaudiodecoder);
        return FALSE;
    }

    //todo need to query audiosink pad's caps
    ret = smeaudiodecoder->smeaudiodecinf->SME_OpenAudioDec(smeaudiodecoder->semcodechdl);
    if (E_SME_ADEC_RET_SUCCESS != ret){
        GST_ERROR("Failed to open adudio decoder");
        smeaudiodecoder->smeaudiodecinf->SME_DestroyAudioDec(smeaudiodecoder->semcodechdl);
        smeaudiodecoder->semcodechdl = NULL;
        GST_OBJECT_UNLOCK (smeaudiodecoder);
        return FALSE;
    }
    smeaudiodecoder->opened = TRUE;
    smeaudiodecoder->proportion = 0.5;
    smeaudiodecoder->earliest_time = (GstClockTime)-1;
    GST_OBJECT_UNLOCK (smeaudiodecoder);
    GST_INFO("setcaps called end");

    return TRUE;
}

static void
gst_sme_audiodecoder_drain (GstSmeAudioDecoder *smeaudiodecoder)
{
    gint len = 0;
    GstFlowReturn ret = GST_FLOW_OK;
    gint have_data = 0;
    gint try = 0;

    //g_return_if_fail(G_UNLIKELY(NULL != smeaudiodecoder));

    do{
        len = gst_sme_audiodec_frame(smeaudiodecoder, NULL, 0, &have_data, &sme_ts_info_none, &ret);
        if (len < 0 || have_data == 0)
        break;
    } while (try++ < 10);

    return;

}

static gboolean
gst_sme_audiodecoder_sink_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
    GstSmeAudioDecoder *smeaudiodecoder;
    gboolean ret = FALSE;
    smeaudiodecoder = (GstSmeAudioDecoder*) parent;
    (void)pad;

    //g_return_val_if_fail(G_UNLIKELY(NULL != smeaudiodecoder), FALSE);
    //g_return_val_if_fail(G_UNLIKELY(NULL != pad), FALSE);
    //g_return_val_if_fail(G_UNLIKELY(NULL != event), FALSE);
    if(NULL == event){
        return FALSE;
    }
      GST_INFO("Handling %s event",GST_EVENT_TYPE_NAME (event));

      switch (GST_EVENT_TYPE (event)) {
        case GST_EVENT_EOS:
        {
              gst_sme_audiodecoder_drain(smeaudiodecoder);
              break;
        }
        case GST_EVENT_FLUSH_STOP:
        {
            if (smeaudiodecoder->opened) {
                GST_INFO("will SME_FlushAudioDec...");
                E_SME_ADEC_RET eRet;
                eRet = smeaudiodecoder->smeaudiodecinf->SME_FlushAudioDec(smeaudiodecoder->semcodechdl);
                GST_INFO("SME_FlushAudioDec return %d", eRet);
            }
            gst_sem_audiodec_update_qos (smeaudiodecoder, 0.5, GST_CLOCK_TIME_NONE);
            gst_segment_init (&smeaudiodecoder->segment, GST_FORMAT_TIME);
            break;
        }
        case GST_EVENT_CAPS:
        {
            GstCaps *caps;

            gst_event_parse_caps (event, &caps);

            if (!smeaudiodecoder->last_caps
              || !gst_caps_is_equal (smeaudiodecoder->last_caps, caps)) {
                ret = gst_sme_audiodec_setcaps (smeaudiodecoder, caps);
                if (ret) {
                    gst_caps_replace (&smeaudiodecoder->last_caps, caps);
                }
            } else {
                ret = TRUE;
            }

            gst_event_unref (event);
            goto done;
        }
        case GST_EVENT_SEGMENT:
        {
              GstSegment segment;

              gst_event_copy_segment (event, &segment);

              switch (segment.format) {
                case GST_FORMAT_TIME:
                  /* fine, our native segment format */
                      GST_INFO("GST_FORMAT_TIME");
                      break;
                case GST_FORMAT_BYTES:{
                       GST_INFO("GST_FORMAT_BYTES");
                      break;//todo
                }
                default:
                  /* invalid format */
                  goto invalid_format;
            }

            GST_DEBUG("SEGMENT in time %" GST_SEGMENT_FORMAT,&segment);

            /* and store the values */

            gst_segment_copy_into (&segment, &smeaudiodecoder->segment);
            GST_INFO ("smeaudiodec segment, position:%lld, start:%lld,duration=%lld",
                smeaudiodecoder->segment.position, smeaudiodecoder->segment.start,
                smeaudiodecoder->segment.duration);

            //push segment event when decout before push data to sink element.
            smeaudiodecoder->bsend_segment_downstream = TRUE;
            gst_event_unref (event);
            event = NULL;
            ret = TRUE;
            break;
        }
        default:
              break;
    }

    /* and push segment downstream */
    if(event){
        ret = gst_pad_push_event (smeaudiodecoder->srcpad, event);
    }

done:

    return ret;

invalid_format:

    GST_WARNING("unknown format received in NEWSEGMENT");
    gst_event_unref (event);
    goto done;
}


static gboolean
gst_sme_audiodecoder_sink_query (GstPad * pad, GstObject * parent, GstQuery * query)
{
    gboolean ret = FALSE;

    GST_INFO("Handling %s query", GST_QUERY_TYPE_NAME (query));
    switch (GST_QUERY_TYPE (query)) {
        case GST_QUERY_ACCEPT_CAPS:
        {
              GstPadTemplate *templ;
              ret = FALSE;

              if (NULL != (templ = GST_PAD_PAD_TEMPLATE (pad))) {
                GstCaps *tcaps;
                if (NULL != (tcaps = GST_PAD_TEMPLATE_CAPS (templ))) {
                      GstCaps *caps;
                      gboolean res;
                      gst_query_parse_accept_caps (query, &caps);
                      res = gst_caps_is_subset (caps, tcaps);
                      if(res){
                        res = TRUE;
                      }
                      gst_query_set_accept_caps_result (query, res);
                      ret = TRUE;
                }
              }
               break;
        }
        case GST_QUERY_ALLOCATION:
        {
            GstAllocationParams params;

              gst_allocation_params_init (&params);
              params.flags = GST_MEMORY_FLAG_ZERO_PADDED;
              params.padding = FF_INPUT_BUFFER_PADDING_SIZE;

          /* we would like to have some padding so that we don't have to
           * copy memory. We don't suggest an allocator. */
              gst_query_add_allocation_param (query, NULL, &params);
              ret = TRUE;
              break;
        }
        default:
              ret = gst_pad_query_default (pad, parent, query);
              break;
    }

    return ret;
}

static void
gst_sem_audiodec_update_qos (GstSmeAudioDecoder *smeaudiodecoder, gdouble proportion,
    GstClockTime timestamp)
{
    GST_INFO("update QOS: %f, %" GST_TIME_FORMAT,proportion, GST_TIME_ARGS (timestamp));

    GST_OBJECT_LOCK (smeaudiodecoder);
    smeaudiodecoder->proportion = proportion;
    smeaudiodecoder->earliest_time = timestamp;
    GST_OBJECT_UNLOCK (smeaudiodecoder);
}

static gboolean
gst_sme_audiodecoder_src_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
    GstSmeAudioDecoder *smeaudiodecoder = NULL;
    gboolean res = FALSE;
    (void)pad;

    smeaudiodecoder = (GstSmeAudioDecoder*) parent;
    //g_return_val_if_fail(G_UNLIKELY(NULL != smeaudiodecoder), FALSE);
    //g_return_val_if_fail(G_UNLIKELY(NULL != pad), FALSE);
    //g_return_val_if_fail(G_UNLIKELY(NULL != event), FALSE);

    switch (GST_EVENT_TYPE (event)) {
        case GST_EVENT_QOS:
        {
          GstQOSType type;
          gdouble proportion;
          GstClockTimeDiff diff;
          GstClockTime timestamp;

            gst_event_parse_qos (event, &type, &proportion, &diff, &timestamp);

          /* update our QoS values */
              gst_sem_audiodec_update_qos (smeaudiodecoder, proportion, (timestamp + (GstClockTime)diff));

          /* forward upstream */
              res = gst_pad_push_event (smeaudiodecoder->sinkpad, event);
              break;
        }
        default:
          /* forward upstream */
              res = gst_pad_push_event (smeaudiodecoder->sinkpad, event);
              break;
    }
    GST_INFO("gst_sme_audiodecoder_src_event res = %d",res);
    return res;
}


static gboolean
gst_sme_audiodecoder_src_query (GstPad * pad, GstObject * parent, GstQuery * query)
{
    gboolean res = FALSE;

    switch (GST_QUERY_TYPE (query)) {
        case GST_QUERY_LATENCY:
              break;
        default:
              res = gst_pad_query_default (pad, parent, query);
              break;
    }

    return res;
}

static gboolean
gst_sem_audiodecoder_negotiate (GstSmeAudioDecoder * smeaudiodecoder, gboolean force)
{
    GstSmeAudioDecoderClass *oclass = NULL;
    oclass = (GstSmeAudioDecoderClass *) (G_OBJECT_GET_CLASS (smeaudiodecoder));
    GstCaps *caps = NULL;
    E_SME_MEDIA_CODEC_ID codecid;
#ifdef __DDP_PT__
    const E_SME_AUDIO_RAW_FMT *samplefmts = NULL;
#else
    E_SME_AUDIO_RAW_FMT samplefmts[] = {E_SME_AUDIO_RAW_FMT_PCM_S16BIT,(E_SME_AUDIO_RAW_FMT)-1};//to do need get samplefmt
#endif
    gboolean ret = TRUE;

    (void)force;

    //g_return_val_if_fail(G_UNLIKELY(NULL != oclass), FALSE);

    codecid = smeaudiodecoder->smeaudiodecinf->stBase.SME_GetCodecId(oclass->codecfmt);

    //to do updata audio info smeaudiodecoder->smeaudiodecinf->SME_GetAudioInfo();
    if(1 != smeaudiodecoder->audioinfo.u32Version){
        smeaudiodecoder->smeaudiodecinf->SME_GetAudioInfo(smeaudiodecoder->semcodechdl,
            &smeaudiodecoder->audioinfo);
    }else{
        return TRUE;
    }

#ifdef __DDP_PT__
    samplefmts = smeaudiodecoder->smeaudiodecinf->stBase.SME_GetSampleFmts(oclass->codecfmt);
#endif

    /* convert the raw output format to caps */
#ifdef __DDP_PT__
    caps = gst_sme_codectype_to_audio_caps (codecid,
        (ST_SME_MEDIAINFO *)&smeaudiodecoder->audioinfo,
        FALSE,
        samplefmts);
#else
    caps = gst_sme_codectype_to_audio_caps (codecid,
        (ST_SME_MEDIAINFO *)&smeaudiodecoder->audioinfo,
        FALSE,
        (E_SME_AUDIO_RAW_FMT*)&samplefmts);
#endif
    if (caps == NULL){
        GST_ERROR ("Could not find caps mapping");
        ret =  FALSE;
    }
#ifdef __DDP_PT__
    else {
        GST_INFO("gst sme audio srcpad caps=%s,codec_id 0x%x, samplefmts = %p,samplefmt=%x", gst_caps_to_string(caps),codecid,samplefmts,smeaudiodecoder->audioinfo.eSampleFmt);
    }
#endif
    if (!gst_pad_set_caps (smeaudiodecoder->srcpad, caps)){
        GST_ERROR ("set audiodecoder srcpad caps failed");
        ret =  FALSE;
    }

    gst_caps_unref (caps);

    return ret;
}

static const GstSmeTSInfo *
gst_sme_ts_info_store (GstSmeAudioDecoder * dec, GstClockTime timestamp,
    GstClockTime duration, gint64 offset)
{
    //g_return_val_if_fail(G_UNLIKELY(NULL != dec), NULL);

    gint idx = dec->ts_idx;
    dec->ts_info[idx].idx = idx;
    dec->ts_info[idx].timestamp = timestamp;
    dec->ts_info[idx].duration = duration;
    dec->ts_info[idx].offset = offset;
    dec->ts_idx = (idx + 1) & MAX_TS_MASK;

    return &dec->ts_info[idx];
}

static gint
gst_sme_audio_decode (GstSmeAudioDecoder * smeaudiodecoder,
                      guint8 * data,
                      guint size,
                      const GstSmeTSInfo * dec_info,
                      GstBuffer ** outbuf,
                      GstFlowReturn * ret,
                      gint *got_data)
{
    gint len = -1;
    gint decodedLen = 0;
    GstClockTime out_timestamp = GST_CLOCK_TIME_NONE;
    GstClockTime out_duration = GST_CLOCK_TIME_NONE;
    gint64 out_offset = 0;

    ST_SME_ARAW_FRAMEBUFFER stOutBuffer;
    ST_SME_MEDIA_FRAME *pstInBuffer = NULL;
    GstDecAllocatorCtx stdec_allocatorctx;

    E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;

    GST_DEBUG("size:%d, offset:%lld, ts:%" GST_TIME_FORMAT ", dur:%"
              GST_TIME_FORMAT ", smeaudiodecoder->next_out:%"
              GST_TIME_FORMAT,
              size,
              dec_info->offset, GST_TIME_ARGS (dec_info->timestamp),
              GST_TIME_ARGS (dec_info->duration),
              GST_TIME_ARGS (smeaudiodecoder->next_out));

#ifdef __SME_BUFFER__
    if(smeaudiodecoder->bis_smebuffer){
        pstInBuffer = (ST_SME_MEDIA_FRAME*)data;
    }else{
#endif
        pstInBuffer =
            (ST_SME_MEDIA_FRAME*)g_malloc(sizeof(ST_SME_MEDIA_FRAME)+size);
        if (!pstInBuffer){
            GST_ERROR("malloc sme audio InBuffer failed");
            return len;
        }
        eMemSecRet = VOS_Memset_S(pstInBuffer,(sizeof(ST_SME_MEDIA_FRAME)+size),
                                    0,sizeof(ST_SME_MEDIA_FRAME)+size);
        if(eMemSecRet != E_SME_MEM_OK)
        {
            GST_ERROR("VOS_Memset_S failed:%d", eMemSecRet);
        }
        if (data != NULL) {
            eMemSecRet = VOS_Memcpy_S(pstInBuffer->pu8DataBuf,size,data,size);
            if(eMemSecRet != E_SME_MEM_OK)
            {
                GST_ERROR("VOS_Memcpy_S failed:%d", eMemSecRet);
            }
        }
        pstInBuffer->u32DataLen = size;
#ifdef __SME_BUFFER__
    }
#endif

    eMemSecRet = VOS_Memset_S((void*)&stOutBuffer, sizeof(ST_SME_ARAW_FRAMEBUFFER),
        0, sizeof(ST_SME_ARAW_FRAMEBUFFER));
    if(eMemSecRet != E_SME_MEM_OK)
    {
        GST_ERROR("VOS_Memset_S failed:%d", eMemSecRet);
    }
    stdec_allocatorctx.pstbuffer = NULL;
    stOutBuffer.stAllocator.pvAllocCtx = &stdec_allocatorctx;
    stOutBuffer.stAllocator.pfnLockBuf = sme_allocator_lock_buf;
    stOutBuffer.stAllocator.pfnUnLockBuf = sme_allocator_unlock_buf;

    /*decode audio frame*/
    E_SME_ADEC_RET result = smeaudiodecoder->smeaudiodecinf->SME_DecAudioFrame(
        smeaudiodecoder->semcodechdl, pstInBuffer, &stOutBuffer, &decodedLen, got_data, (const void *)dec_info);

    GST_DEBUG("Dec result(%d)", result);

    if(E_SME_ADEC_RET_SUCCESS == result){
        len = (gint)stOutBuffer.pstARawFrame->u32DataLen;
        *outbuf = stdec_allocatorctx.pstbuffer;
        gst_buffer_resize(*outbuf, sizeof(ST_SME_ARAW_FRAME), len);

        /*与sink element协商*/
        if (!gst_sem_audiodecoder_negotiate (smeaudiodecoder, FALSE)) {

            /** BEGIN: Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/8 */
            gst_buffer_unref (*outbuf);
            *outbuf = NULL;
            /** END:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/8 */

            len = -1;
            goto error;
        }

        /*
         * Timestamps:
         *
         *  1) Copy input timestamp if valid
         *  2) else interpolate from previous input timestamp
         */
        /* always take timestamps from the input buffer if any */
        if (GST_CLOCK_TIME_IS_VALID (dec_info->timestamp)) {
            out_timestamp = dec_info->timestamp;
        } else {
            out_timestamp = smeaudiodecoder->next_out;
        }

        /*
         * Duration:Just copy  todo check
         *
         *  1) calculate based on number of samples
         */
        /* out_duration = gst_util_uint64_scale (have_data, GST_SECOND,
            ffmpegdec->format.audio.depth * ffmpegdec->format.audio.channels *
            ffmpegdec->format.audio.samplerate);*/

        /* offset:
         *
         * Just copy
         */
        out_offset = dec_info->offset;
        out_duration = dec_info->duration;
        GST_DEBUG(
            "Buffer created. Size:%d , timestamp:%" GST_TIME_FORMAT " , duration:%"
           GST_TIME_FORMAT",pts=%lld,send_seg=%d", len,
           GST_TIME_ARGS (out_timestamp), GST_TIME_ARGS (out_duration), out_timestamp,
           smeaudiodecoder->bsend_segment_downstream);
        if (NULL == *outbuf) {
            goto error;
        }
        GST_BUFFER_TIMESTAMP (*outbuf)  = out_timestamp;
        GST_BUFFER_DURATION (*outbuf)   = out_duration;
        GST_BUFFER_OFFSET (*outbuf)     = (guint64)out_offset;

        //revise the segment by dec out time.
        if(smeaudiodecoder->bsend_segment_downstream){
            smeaudiodecoder->segment.position   = GST_BUFFER_PTS(*outbuf);
            smeaudiodecoder->segment.time       = GST_BUFFER_PTS(*outbuf);
            smeaudiodecoder->segment.start      = GST_BUFFER_PTS(*outbuf);
            smeaudiodecoder->segment.format     = GST_FORMAT_TIME;
            GST_INFO ("smeaudiodec out segment, position:%lld, start:%lld,duration=%lld",
                smeaudiodecoder->segment.position, smeaudiodecoder->segment.start,
                smeaudiodecoder->segment.duration);

            if(gst_pad_push_event (smeaudiodecoder->srcpad,
                gst_event_new_segment (&smeaudiodecoder->segment))){
                smeaudiodecoder->bsend_segment_downstream = FALSE;
            }else{
                GST_WARNING("gstsme audio dec push segment down stream failed");
            }
        }

        /* the next timestamp we'll use when interpolating */
        if (GST_CLOCK_TIME_IS_VALID (out_timestamp))
            smeaudiodecoder->next_out = out_timestamp + out_duration;

        //if success, return the length of data consumed
        len = decodedLen;
        goto beach;
    }else if(E_SME_ADEC_RET_NOT_COMPLETE == result){//decode not complete got_frame<=0
        GST_ERROR("decode not completed");
         //if decoding not completed, return the length of data consumed, which is used
        //to set the offset the input data, preparing for the next decoding
        len = decodedLen;
    }else{//decode error len <0
        GST_ERROR("Dec Error(%d)", result);
        len = -1;
    }

error:
    if(G_LIKELY(NULL != *outbuf)){
        gst_buffer_unmap (*outbuf, &stdec_allocatorctx.stmap_info);
        gst_buffer_unref (*outbuf);
        *outbuf = NULL;
    }

beach:
#ifdef __SME_BUFFER__
    if(!smeaudiodecoder->bis_smebuffer)
#endif
    {
        g_free(pstInBuffer);
        pstInBuffer = NULL;
    }
    (void)pstInBuffer;
    (void)ret;
    return len;
}

static gint
gst_sme_audiodec_frame (GstSmeAudioDecoder * smeaudiodecoder,
    guint8 * data, guint size, gint * got_data, const GstSmeTSInfo * dec_info,
    GstFlowReturn * ret)
{
    GstBuffer *outbuf = NULL;
    gint have_data = 0;
    gint len = 0;

    g_return_val_if_fail(G_UNLIKELY(NULL != ret), -1);
    *ret = GST_FLOW_OK;

    GST_DEBUG("data:%p, size:%d, id:%d", data, size,dec_info->idx);

    //NOTE:this revision is mainly for two issues, which is also the ffmpeg-suggested way.
    //1:the packet may contain many frames to decode, we must decode the packet completely;
    //2:for audio codec wmapro, no audio frame got while decoding the packet in the first time,
    //it should not stop decoding until the packet is consumed completely.
    while(size > 0 && data){
        outbuf = NULL;
        len = gst_sme_audio_decode(smeaudiodecoder, data, size,dec_info, &outbuf, ret, &have_data);

        GST_DEBUG("len=%d, ret=%d, outbuf=%p",len, *ret, outbuf);

        /* BEGIN: Modified for DTS2014102405113 by liurongliang(l00180035), 2014/10/25 */
        if (outbuf){
            smeaudiodecoder->dec_err_count = 0;
#ifdef SMEPLAYER_BENCHMARK_OPEN
            smeaudiodecoder->audio_decoder_info.ADecErrorCnt = 0;
#endif
            have_data = 1;
        }
        if (len < 0) {
            GST_ERROR("decoding error (len: %d, have_data: %d)",len, have_data);
            smeaudiodecoder->dec_err_count++;
#ifdef SMEPLAYER_BENCHMARK_OPEN
            smeaudiodecoder->audio_decoder_info.ADecErrorCnt = (V_UINT32)smeaudiodecoder->dec_err_count;
            VOS_Sprintf_S(smeaudiodecoder->audio_decoder_info.ADecErrorInfo,
                            sizeof(smeaudiodecoder->audio_decoder_info.ADecErrorInfo),
                            "audio frame error");
#endif
            if(smeaudiodecoder->dec_err_count >= GST_MAX_ADEC_ERROR){
                *ret = GST_FLOW_CUSTOM_ERROR;
                GST_ELEMENT_ERROR (smeaudiodecoder, STREAM, DECODE, (NULL),
                    ("%s SME_DEC_ERR", GST_SME_STRAM_TAG_ADEC));
            }
            *got_data = 0;
            goto beach;
        } else if (len == 0 && have_data == 0) {
            *got_data = 0;
            smeaudiodecoder->dec_err_count = 0;
#ifdef SMEPLAYER_BENCHMARK_OPEN
            smeaudiodecoder->audio_decoder_info.ADecErrorCnt = 0;
#endif
            goto beach;
        } else {
        /* this is where I lost my last clue on ffmpeg... */
            *got_data = 1;
        }
        /* END:   Modified for DTS2014102405113 by liurongliang(l00180035), 2014/10/25 */

        if (outbuf) {
            GST_DEBUG(
                "Decoded data, now pushing buffer %p with offset %" G_GINT64_FORMAT
                ", timestamp %" GST_TIME_FORMAT " and duration %" GST_TIME_FORMAT,
                outbuf, GST_BUFFER_OFFSET (outbuf),
                GST_TIME_ARGS (GST_BUFFER_TIMESTAMP (outbuf)),
                GST_TIME_ARGS (GST_BUFFER_DURATION (outbuf)));

            if (smeaudiodecoder->segment.rate > 0.0){
#ifdef SMEPLAYER_BENCHMARK_OPEN
                if (smeaudiodecoder->dump_src_flag) {
                    GstMapInfo info;
                    gst_buffer_map (outbuf, &info, GST_MAP_READ);
                    gst_audio_dump_src_file(smeaudiodecoder,info.data,(int)info.size);
                    smeaudiodecoder->audio_decoder_info.MaxBufferSize = info.size;
                    gst_buffer_unmap (outbuf, &info);
                } else {
                    gst_audio_dump_src_file(smeaudiodecoder,NULL,0);
                }

                if (!smeaudiodecoder->audio_decoder_info.ADecFirstPlayTimeFlag) {
                    smeaudiodecoder->audio_decoder_info.ADecFirstPlayTimeFlag = 1;
                    gettimeofday(&smeaudiodecoder->audio_decoder_info.ADecFirstPlayTime, NULL);
                }
#endif
                *ret = gst_pad_push (smeaudiodecoder->srcpad, outbuf);
            }
        } else {
            GST_ERROR("We didn't get a decoded buffer");
        }

        smeaudiodecoder->dec_err_count = 0;
#ifdef SMEPLAYER_BENCHMARK_OPEN
        smeaudiodecoder->audio_decoder_info.ADecErrorCnt = 0;
#endif
        //update the remained size & offset for the next decoding
        size -= (guint)len;
        data += len;
    }

beach:
    return len;
}

static GstFlowReturn
gst_sme_audiodecoder_chain (GstPad * pad, GstObject * parent, GstBuffer * inbuf)
{
    GstFlowReturn ret = GST_FLOW_OK;
    GstClockTime in_timestamp;
    GstClockTime in_duration;
    gint64 in_offset;
    const GstSmeTSInfo *in_info = NULL;
    GstSmeAudioDecoder *smeaudiodecoder = NULL;
    gint len = 0;
    gint have_data = 0;
    PST_SME_MEDIA_FRAME pstsme_frame = NULL;
    GstMapInfo map;
    (void)pad;

    smeaudiodecoder = (GstSmeAudioDecoder*) parent;

    if (G_UNLIKELY (!smeaudiodecoder->opened)){
        GST_ERROR ("audio decoder was not open before start decoding");
        goto not_negotiated;
    }

#ifdef __SME_BUFFER__
    gst_buffer_map (inbuf, &map, GST_MAP_READ);
    pstsme_frame = (PST_SME_MEDIA_FRAME)map.data;
    smeaudiodecoder->bis_smebuffer = TRUE;
    if(pstsme_frame->stInfo.i32TsBaseDen > 0){
        in_timestamp = (GstClockTime)M_SME_TS_RESCALE(pstsme_frame->stInfo.i64Pts,
                                        M_SME_MS_TO_NS,
                                        pstsme_frame->stInfo.i32TsBaseNum,
                                        pstsme_frame->stInfo.i32TsBaseDen);
    }else{
        in_timestamp = (GstClockTime)pstsme_frame->stInfo.i64Pts * M_SME_MS_TO_NS;
    }

    if(pstsme_frame->stInfo.i32TsBaseDen > 0){
        in_duration = (GstClockTime)M_SME_TS_RESCALE(pstsme_frame->stInfo.u32Duration,
                                    M_SME_MS_TO_NS,
                                    pstsme_frame->stInfo.i32TsBaseNum,
                                    pstsme_frame->stInfo.i32TsBaseDen);
    }else{
        in_duration = pstsme_frame->stInfo.u32Duration;
        in_duration *= M_SME_MS_TO_NS;
    }
    in_offset = (gint64)GST_BUFFER_OFFSET (inbuf);
    in_info = gst_sme_ts_info_store (smeaudiodecoder, in_timestamp, in_duration, in_offset);
    len = gst_sme_audiodec_frame (smeaudiodecoder, map.data, map.size, &have_data, in_info, &ret);
    if(len < 0){
        GST_WARNING ("invalid data Decoding error");
    }
#else
    /* decode a frame of audio/video now  use default gstbuffer */
    in_timestamp = GST_BUFFER_TIMESTAMP (inbuf);
    in_duration = GST_BUFFER_DURATION (inbuf);
    in_offset = GST_BUFFER_OFFSET (inbuf);
    in_info = gst_sme_ts_info_store (smeaudiodecoder, in_timestamp, in_duration, in_offset);
    smeaudiodecoder->bis_smebuffer = FALSE;
    gst_buffer_map (inbuf, &map, GST_MAP_READ);
    len = gst_sme_audiodec_frame (smeaudiodecoder, map.data, map.size, &have_data, in_info, &ret);
    if(len < 0){
        GST_WARNING("Decoding error, breaking");
    }
    else if (len == 0 && !have_data) {
        GST_WARNING("Decoding didn't return any data, breaking");
    }
#endif

#ifdef SMEPLAYER_BENCHMARK_OPEN
    if (!smeaudiodecoder->audio_decoder_info.ADecGetFirstIFrameTimeFlag) {
        smeaudiodecoder->audio_decoder_info.ADecGetFirstIFrameTimeFlag = 1;
        gettimeofday(&smeaudiodecoder->audio_decoder_info.ADecGetFirstIFrameTime, NULL);
    }
    gst_audio_dump_sink_file(smeaudiodecoder,map.data,(int)map.size);
#endif


    gst_buffer_unmap (inbuf, &map);
    gst_buffer_unref (inbuf);

    return ret;

  /* ERRORS */
not_negotiated:

    GST_ERROR ("input format was not set before data start");
    gst_buffer_unref (inbuf);
    return GST_FLOW_NOT_NEGOTIATED;
}

static void
gst_sme_audiodec_init (GstSmeAudioDecoder * smeaudiodecoder)
{
    GstSmeAudioDecoderClass *oclass = NULL;
    E_SME_ADEC_LIB_TYPE adeclibtype = E_SME_ADEC_LIB_MAX;
    E_SME_MEM_SECURE_RET eMemSecRet;

    oclass = (GstSmeAudioDecoderClass *) (G_OBJECT_GET_CLASS (smeaudiodecoder));

    /* setup pads */
    smeaudiodecoder->sinkpad = gst_pad_new_from_template (oclass->sinktempl, "sink");
    gst_pad_set_event_function (smeaudiodecoder->sinkpad,
        GST_DEBUG_FUNCPTR (gst_sme_audiodecoder_sink_event));
    gst_pad_set_query_function (smeaudiodecoder->sinkpad,
        GST_DEBUG_FUNCPTR (gst_sme_audiodecoder_sink_query));
    gst_pad_set_chain_function (smeaudiodecoder->sinkpad,
        GST_DEBUG_FUNCPTR (gst_sme_audiodecoder_chain));
    gst_pad_set_activate_function (smeaudiodecoder->sinkpad, gst_sme_audiodec_sink_pad_activate);
    gst_pad_set_activatemode_function(smeaudiodecoder->sinkpad, gst_sme_audiodec_sink_pad_activatemode);

    gst_element_add_pad (GST_ELEMENT (smeaudiodecoder), smeaudiodecoder->sinkpad);

    smeaudiodecoder->srcpad = gst_pad_new_from_template (oclass->srctempl, "src");
    gst_pad_use_fixed_caps (smeaudiodecoder->srcpad);
    gst_pad_set_event_function (smeaudiodecoder->srcpad,
        GST_DEBUG_FUNCPTR (gst_sme_audiodecoder_src_event));
    gst_pad_set_query_function (smeaudiodecoder->srcpad,
        GST_DEBUG_FUNCPTR (gst_sme_audiodecoder_src_query));
    gst_element_add_pad (GST_ELEMENT (smeaudiodecoder), smeaudiodecoder->srcpad);

    eMemSecRet = VOS_Memset_S((void*)&smeaudiodecoder->audioinfo, sizeof(ST_SME_AUDIOINFO),
        0, sizeof(ST_SME_AUDIOINFO));
    if(eMemSecRet != E_SME_MEM_OK)
    {
        GST_ERROR("VOS_Memset_S failed:%d", eMemSecRet);
    }

    adeclibtype = GPOINTER_TO_INT(g_type_get_qdata(
                                      G_OBJECT_CLASS_TYPE(oclass),
                                      GST_SME_AUDIODEC_LIBTYPE_QDATA));

    smeaudiodecoder->smeaudiodecinf = gst_sme_audiodec_getinf(adeclibtype);
    smeaudiodecoder->opened = FALSE;
    smeaudiodecoder->semcodechdl = NULL;
    gst_segment_init (&smeaudiodecoder->segment, GST_FORMAT_TIME);
    smeaudiodecoder->next_out = GST_CLOCK_TIME_NONE;
    smeaudiodecoder->last_caps = NULL;
    smeaudiodecoder->esink_pad_mode = GST_PAD_MODE_NONE;
    smeaudiodecoder->bis_smebuffer = FALSE;
    smeaudiodecoder->bsend_segment_downstream = FALSE;
    smeaudiodecoder->dec_err_count = 0;
#ifdef SMEPLAYER_BENCHMARK_OPEN
    eMemSecRet = VOS_Memset_S(&smeaudiodecoder->audio_decoder_info,
                                sizeof(smeaudiodecoder->audio_decoder_info),
                                0,
                                sizeof(smeaudiodecoder->audio_decoder_info));
    if(eMemSecRet != E_SME_MEM_OK)
    {
        GST_ERROR("VOS_Memset_S failed:%d", eMemSecRet);
    }
    gettimeofday(&smeaudiodecoder->audio_decoder_info.CreateAdecTime, NULL);

    smeaudiodecoder->dump_sink_file = NULL;
    smeaudiodecoder->dump_sink_flag = FALSE;
    VOS_Snprintf_S(smeaudiodecoder->dump_sink_file_name,
                    sizeof(smeaudiodecoder->dump_sink_file_name),
                    sizeof(smeaudiodecoder->dump_sink_file_name),
                    "/mnt/adec_in.es");

    smeaudiodecoder->dump_src_file = NULL;
    smeaudiodecoder->dump_src_flag = FALSE;
    VOS_Snprintf_S(smeaudiodecoder->dump_src_file_name,
                    sizeof(smeaudiodecoder->dump_src_file_name),
                    sizeof(smeaudiodecoder->dump_src_file_name),
                    "/mnt/adec_out.pcm");

  //GST_WARNING_OBJECT (dec, "dec %p, gst_audio_decoder_init dump_src_flag %s: %p,%s!",
  //                      smeaudiodecoder, smeaudiodecoder->dump_src_flag?"true":"false",smeaudiodecoder->dump_src_file,smeaudiodecoder->dump_src_file_name);
#endif
}

static void*
sme_allocator_lock_buf(gpointer gpctx, guint u32bufSize, gint i32stream_idx)
{
    g_return_val_if_fail(G_UNLIKELY(NULL != gpctx), NULL);

    GstDecAllocatorCtx* pstallocator = (GstDecAllocatorCtx*)gpctx;
    gboolean bres = FALSE;

    (void)i32stream_idx;


    pstallocator->pstbuffer = gst_buffer_new_and_alloc (u32bufSize);
    if(G_UNLIKELY(NULL == pstallocator->pstbuffer)){
        GST_ERROR("alloc sme audio decoder gstbuffer failed");
        return NULL;
    }

    bres = gst_buffer_map (pstallocator->pstbuffer, &pstallocator->stmap_info, GST_MAP_WRITE);
    if(G_UNLIKELY(FALSE == bres)){
        GST_ERROR("map sme audio decoder gstbuffer failed");
        gst_buffer_unref(pstallocator->pstbuffer);
        pstallocator->pstbuffer = NULL;
        return NULL;
    }

    if(G_UNLIKELY(NULL == pstallocator->stmap_info.data)){
        GST_ERROR("map sme audio decoder gstbuffer failed");
        gst_buffer_unref(pstallocator->pstbuffer);
        pstallocator->pstbuffer = NULL;
        return NULL;
    }

    return pstallocator->stmap_info.data;
}

static void
sme_allocator_unlock_buf(gpointer gpctx, gpointer gpdata_buf)
{
    g_return_if_fail(G_UNLIKELY(NULL != gpctx));

    GstDecAllocatorCtx* pstallocator = (GstDecAllocatorCtx*)gpctx;

    (void)gpdata_buf;

    //gst_buffer_resize (pstallocator->pstbuffer, 0, pstallocator->stmap_info.size);
    gst_buffer_unmap(pstallocator->pstbuffer, &pstallocator->stmap_info);

    return ;
}

static gboolean
gst_sme_audiodec_sink_pad_negotiate_pull (GstSmeAudioDecoder *smeaudiodecoder)
{
    GstCaps *caps = NULL;
    gboolean result = FALSE;
    gchar* caps_tostring = NULL;

    /* this returns the intersection between our caps and the peer caps. If there
    * is no peer, it returns NULL and we can't operate in pull mode so we can
    * fail the negotiation. */
    caps = gst_pad_get_allowed_caps (smeaudiodecoder->sinkpad);
    if (caps == NULL || gst_caps_is_empty (caps))
        goto no_caps_possible;

    caps_tostring = gst_caps_to_string(caps);
    GST_INFO_OBJECT (smeaudiodecoder, "adec allowed caps: %s" , caps_tostring);
    g_free(caps_tostring);

    if (gst_caps_is_any (caps)) {
        GST_INFO_OBJECT (smeaudiodecoder, "caps were ANY after fixating, allowing pull()");
        /* neither side has template caps in this case, so they are prepared for
           pull() without setcaps() */
        result = TRUE;
    } else {
        if (gst_caps_is_fixed (caps)) {
             if (!gst_pad_set_caps (smeaudiodecoder->sinkpad, caps)){
                goto could_not_set_caps;
             }
            result = TRUE;
        }else{
            caps_tostring = gst_caps_to_string(caps);
            GST_ERROR_OBJECT (smeaudiodecoder, "adec allowed caps: %s,not fixed",
                gst_caps_to_string(caps));
            g_free(caps_tostring);
            result = FALSE;
        }
    }
    gst_caps_unref (caps);

    return result;

no_caps_possible:
    GST_INFO_OBJECT (smeaudiodecoder, "Pipeline could not agree on caps");
    GST_INFO_OBJECT (smeaudiodecoder, "get_allowed_caps() returned EMPTY");
    if (caps){
        gst_caps_unref (caps);
    }

    return FALSE;

could_not_set_caps:
    GST_INFO_OBJECT (smeaudiodecoder, "Could not set caps: %" GST_PTR_FORMAT, caps);
    gst_caps_unref (caps);

    return FALSE;
}


static gboolean
gst_sme_audiodec_sink_pad_activate (GstPad * pad, GstObject * parent)
{
    gboolean result = FALSE;
    GstSmeAudioDecoder *smeaudiodecoder = NULL;
    GstQuery *query = NULL;
    gboolean pull_mode = FALSE;

    smeaudiodecoder = (GstSmeAudioDecoder*) parent;
    //g_return_val_if_fail(G_UNLIKELY(NULL != smeaudiodecoder), FALSE);
    //g_return_val_if_fail(G_UNLIKELY(NULL != pad), FALSE);

    GST_INFO_OBJECT (smeaudiodecoder, "smeaudiodecoder Trying pull mode first");

    /* check if downstreams supports pull mode at all */
    query = gst_query_new_scheduling ();
    if (!gst_pad_peer_query (pad, query)) {
        gst_query_unref (query);
        GST_INFO_OBJECT (smeaudiodecoder, "smeaudiodecoder peer query faild, no pull mode");
        goto fallback;
    }

    /* parse result of the query */
    pull_mode = gst_query_has_scheduling_mode (query, GST_PAD_MODE_PULL);
    gst_query_unref (query);
    if (!pull_mode) {
        GST_INFO_OBJECT (smeaudiodecoder, "pull mode not supported");
        goto fallback;
    }

    /* set the pad mode before starting the task so that it's in the
    * correct state for the new thread. also the sink set_caps and get_caps
    * function checks this */
    smeaudiodecoder->esink_pad_mode = GST_PAD_MODE_PULL;

    /* ok activate now */
    if (!gst_pad_activate_mode (pad, GST_PAD_MODE_PULL, (gboolean)TRUE)) {
    /* clear any pending caps */
        GST_OBJECT_LOCK (smeaudiodecoder);
        gst_caps_replace (&smeaudiodecoder->last_caps, NULL);
        GST_OBJECT_UNLOCK (smeaudiodecoder);
        GST_INFO_OBJECT (smeaudiodecoder, "failed to activate in pull mode");
        goto fallback;
    }

    GST_INFO_OBJECT (smeaudiodecoder, "Success activating pull mode");
    result = TRUE;

    goto done;

    /* push mode fallback */
fallback:
    GST_INFO_OBJECT (smeaudiodecoder, "Falling back to push mode");
    if (FALSE != (result = gst_pad_activate_mode (pad, GST_PAD_MODE_PUSH, (gboolean)TRUE))) {
        GST_DEBUG_OBJECT (smeaudiodecoder, "Success activating push mode");
    }

done:
    if (!result) {
        GST_WARNING_OBJECT (smeaudiodecoder, "Could not activate pad in either mode");
    }

    return result;

}

static gboolean
gst_sme_audiodec_sink_pad_activatemode(GstPad *pad, GstObject *parent,
                                 GstPadMode mode, gboolean active)
{
    gboolean res = FALSE;

    switch (mode) {
    case GST_PAD_MODE_PULL:
        res = gst_sme_audiodec_sink_activate_pull (pad, parent, active);
        break;
    case GST_PAD_MODE_PUSH:
        res = gst_sme_audiodec_sink_activate_push (pad, parent, active);
        break;
    default:
        GST_ERROR_OBJECT (pad, "unknown activation mode %d", mode);
        res = FALSE;
        break;
    }

    return res;
}

static gboolean
gst_sme_audiodec_sink_activate_pull(GstPad * pad, GstObject * parent,
                                gboolean active)
{
    gboolean result = FALSE;
    GstSmeAudioDecoder *smeaudiodecoder = (GstSmeAudioDecoder *)(parent);

    if (active){
        gint64 duration = 0;


        /* we first try to negotiate a format so that when we try to activate
        * downstream, it knows about our format */
        if (!gst_sme_audiodec_sink_pad_negotiate_pull (smeaudiodecoder)) {
            GST_INFO_OBJECT (smeaudiodecoder, "failed to negotiate in pull mode");
            goto activate_failed;
        }
        /* we mark we have a newsegment here because pull based
         * mode works just fine without having a newsegment before the
         * first buffer */
        gst_segment_init (&smeaudiodecoder->segment, GST_FORMAT_TIME);

        /* get the peer duration in bytes */
        result = gst_pad_peer_query_duration (pad, GST_FORMAT_TIME, &duration);
        if (result) {
            GST_INFO_OBJECT (smeaudiodecoder,
              "setting duration in time to %" G_GINT64_FORMAT, duration);
            smeaudiodecoder->segment.duration = (guint64)duration;
            smeaudiodecoder->bsend_segment_downstream = TRUE;
        } else {
            GST_WARNING_OBJECT (smeaudiodecoder, "unknown duration");
            goto activate_failed;
        }

        //start loop
        result = gst_pad_start_task (pad,
                                    (GstTaskFunction) gst_sme_audiodec_sink_loop,
                                    pad,
                                    NULL);
        if (!result){
          goto activate_failed;
        }

    } else {
        if (G_UNLIKELY (smeaudiodecoder->esink_pad_mode != GST_PAD_MODE_PULL)) {
            GST_WARNING("gstsmeaudiodec Internal GStreamer activation error!!!");
            result = FALSE;
        } else {
            //stop loop
            result = gst_pad_stop_task (pad);
            smeaudiodecoder->esink_pad_mode = GST_PAD_MODE_NONE;
        }
    }

    return result;

    /* ERRORS */
activate_failed:
    /* reset, as starting the thread failed */
    smeaudiodecoder->esink_pad_mode = GST_PAD_MODE_NONE;
    GST_ERROR_OBJECT (smeaudiodecoder, "subclass failed to activate in pull mode");

    return FALSE;
}

static gboolean
gst_sme_audiodec_sink_activate_push (GstPad * pad, GstObject * parent,
                                    gboolean active)
{
    GstSmeAudioDecoder *smeaudiodecoder = (GstSmeAudioDecoder *)parent;
    (void)pad;

    if (active) {
        smeaudiodecoder->esink_pad_mode = GST_PAD_MODE_PUSH;
        GST_INFO("sme audiodec sink pad active push mode");
        return TRUE;
    }

    if (G_UNLIKELY (smeaudiodecoder->esink_pad_mode != GST_PAD_MODE_PUSH)) {
        GST_ERROR("Internal GStreamer activation error!!!");
        return FALSE;
    }

    smeaudiodecoder->esink_pad_mode = GST_PAD_MODE_NONE;
    GST_INFO("sme audiodec sink pad deactive push mode");

    return TRUE;
}

static void
gst_sme_audiodec_sink_loop (GstPad * pad)
{
    GstObject *parent = NULL;
    GstSmeAudioDecoder *smeaudiodecoder = NULL;
    GstBuffer *buf = NULL;
    GstFlowReturn result = GST_FLOW_OK;
    guint blocksize = 1;
    guint64 offset = 0;

    parent = GST_OBJECT_PARENT (pad);
    smeaudiodecoder = (GstSmeAudioDecoder *)parent;

    g_assert (GST_PAD_MODE_PULL == smeaudiodecoder->esink_pad_mode);

    if (G_UNLIKELY (!smeaudiodecoder->opened)){
        GST_ERROR ("audio decoder was not open before start decoding");
        goto no_buffer;
    }

    offset = smeaudiodecoder->segment.position;
    result = gst_pad_pull_range (pad, offset, blocksize, &buf);
    if (G_UNLIKELY (result != GST_FLOW_OK)){
        goto paused;
    }

    if (G_UNLIKELY (buf == NULL)){
        goto no_buffer;
    }

    offset += 1;
    smeaudiodecoder->segment.position = offset;
    result = gst_sme_audiodecoder_chain (pad, parent, buf);
    if (G_UNLIKELY (result != GST_FLOW_OK)){
        goto paused;
    }

    return;

/* ERRORS */
paused:
    GST_INFO_OBJECT (smeaudiodecoder, "pausing task, reason %s",
        gst_flow_get_name (result));
    gst_pad_pause_task (pad);
    if (result == GST_FLOW_EOS) {
        /* perform EOS logic */
        if (smeaudiodecoder->segment.flags & GST_SEGMENT_FLAG_SEGMENT) {
            gst_element_post_message (GST_ELEMENT_CAST (smeaudiodecoder),
                gst_message_new_segment_done (GST_OBJECT_CAST (smeaudiodecoder),
                    smeaudiodecoder->segment.format, (gint64)smeaudiodecoder->segment.position));
            gst_sme_audiodecoder_sink_event (pad, parent,
                gst_event_new_segment_done (smeaudiodecoder->segment.format,
                    (gint64)smeaudiodecoder->segment.position));
        } else {
            gst_sme_audiodecoder_sink_event (pad, parent, gst_event_new_eos ());
        }
    } else if (result == GST_FLOW_NOT_LINKED || result <= GST_FLOW_EOS) {
        /* for fatal errors we post an error message, post the error
        * first so the app knows about the error first.
        * wrong-state is not a fatal error because it happens due to
        * flushing and posting an error message in that case is the
        * wrong thing to do, e.g. when basesrc is doing a flushing
        * seek. */
        GST_ELEMENT_ERROR (smeaudiodecoder, STREAM, FAILED,
            ("Internal data stream error."),
            ("stream stopped, reason %s", gst_flow_get_name (result)));
        gst_sme_audiodecoder_sink_event(pad, parent, gst_event_new_eos ());
    }

    return;

no_buffer:
    GST_ERROR_OBJECT (smeaudiodecoder, "no buffer, pausing");
    GST_ELEMENT_ERROR (smeaudiodecoder, STREAM, FAILED,
        ("Internal data flow error."), ("element returned NULL buffer"));
    result = GST_FLOW_ERROR;
    (void)result;
    goto paused;
}


gboolean
gst_smeaudiodecoder_register (GstPlugin * plugin)
{
    GType type;
    CSMECODECFMT codecfmt = NULL;
    E_SME_MEDIA_CODEC_ID  codecid;
    gchar *codec_name = NULL;
    ST_SME_AUDIO_DEC_INF *smeaudiodecinf = NULL;
    guint baserank = GST_RANK_SECONDARY+1, rank = 0;
    gchar *type_name = NULL;
    gchar *plugin_name = NULL;
    GTypeInfo typeinfo = {
        sizeof (GstSmeAudioDecoderClass),
        (GBaseInitFunc) gst_sme_audiodec_base_init,
        NULL,
        (GClassInitFunc) gst_sme_audiodec_class_init,
        NULL,
        NULL,
        sizeof (GstSmeAudioDecoder),
        0,
        (GInstanceInitFunc) gst_sme_audiodec_init,
        NULL
    };
    E_SME_ADEC_LIB_TYPE adeclibtype = E_SME_ADEC_LIB_BASE;

    GST_INFO ("Registering decoders");

    g_return_val_if_fail(G_UNLIKELY(NULL != plugin), FALSE);

    for (adeclibtype = E_SME_ADEC_LIB_BASE;
         adeclibtype < E_SME_ADEC_LIB_MAX; adeclibtype++)
    {
        smeaudiodecinf = gst_sme_audiodec_getinf(adeclibtype);
        if (smeaudiodecinf == NULL)
        {
            continue;
        }

        /**< let hisi audio decoder's rank be higher than ffmpeg */
        rank = baserank + adeclibtype;

        codecfmt = smeaudiodecinf->stBase.SME_GetNextCodec(NULL,NULL);
        g_return_val_if_fail(G_UNLIKELY(NULL != codecfmt), TRUE);
        while (codecfmt) {
            codec_name = (gchar *)smeaudiodecinf->stBase.SME_GetCodecName(codecfmt);
            codecid = smeaudiodecinf->stBase.SME_GetCodecId(codecfmt);

            GST_INFO("codecid=%d,codec_name=%s", codecid,codec_name);

            plugin_name = g_strdup ((gchar *) codec_name);
            g_return_val_if_fail(G_UNLIKELY(NULL != plugin_name), FALSE);
            g_strdelimit (plugin_name, NULL, '_');
            type_name = g_strdup_printf ("smedec_%s", plugin_name);
            if(NULL == type_name){
                g_free (plugin_name);
                return FALSE;
            }
            g_free (plugin_name);

            GST_INFO("Trying plugin %s", type_name);

            /* construct the type */
            type = g_type_from_name (type_name);
            if (!type) {
                /* create the gtype now */
                type = g_type_register_static (GST_TYPE_ELEMENT,
                                               type_name,
                                               &typeinfo,
                                               (GTypeFlags)0);
                g_type_set_qdata (type,
                                  (GQuark)GST_SME_AUDIODEC_PARAMS_QDATA,
                                  (gpointer)codecfmt);
                g_type_set_qdata (type,
                                  (GQuark)GST_SME_AUDIODEC_LIBTYPE_QDATA,
                                  GINT_TO_POINTER(adeclibtype));
            }

            if (!gst_element_register (plugin, type_name, rank, type)) {
                GST_ERROR("Failed to register %s", type_name);
#ifdef __DDP_PT__
#else
                g_free (type_name);
                return FALSE;
#endif
            }
            g_free (type_name);

            codecfmt = smeaudiodecinf->stBase.SME_GetNextCodec(codecfmt,NULL);
        }
    }

    GST_INFO ("Finished Registering decoders");

    return TRUE;
}

static ST_SME_AUDIO_DEC_INF
*gst_sme_audiodec_getinf(E_SME_ADEC_LIB_TYPE adeclibtype)
{
    PST_SME_AUDIO_DEC_INF pRet = NULL;

    if (adeclibtype == E_SME_ADEC_LIB_FFMPEG)
    {
        pRet = (PST_SME_AUDIO_DEC_INF)SME_GetAudioDecInfOfFFMpeg();
    }
#if 0
    else if (adeclibtype == E_SME_ADEC_LIB_HISI)
    {
        pRet = (PST_SME_AUDIO_DEC_INF)SME_GetAudioDecInfOfHisi();
    }
#endif
#ifdef __DDP_PT__
    else if (adeclibtype == E_SME_ADEC_LIB_PT)
    {
#ifdef __LINUX__
        if (access("/usr/lib/libHA.AUDIO.DOLBYPLUS.decode.so",0)>=0)
#else
        if (access("/system/lib/libHA.AUDIO.DOLBYPLUS.decode.so",0)>=0)
#endif
        {
            GST_INFO ("libHA.AUDIO.DOLBYPLUS.decode.so exists!");
            pRet = (PST_SME_AUDIO_DEC_INF)SME_GetAudioDecInfOfPT();
        }
        else {
            GST_INFO ("libHA.AUDIO.DOLBYPLUS.decode.so not exists!");
        }
    }
#endif

    return pRet;
}

///////////////////////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H

#include "config.h"

//for dynamic load smehttpsrc plugin
#ifdef GST_PLUGIN_BUILD_STATIC
#undef GST_PLUGIN_BUILD_STATIC
#endif

#endif

#include "gstsme.h"

static gboolean
plugin_init (GstPlugin * plugin)
{
    GST_INFO("register smeaudiodecoder begin...");

    //register audio decode element
    if (!gst_smeaudiodecoder_register (plugin))
        GST_WARNING("register smeaudiodec fail.");

    GST_INFO("register smeaudiodecoder end.");

    return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    smeaudiodecoder,
    "sme audiodecoder plugin of mediaos. (info about sme: http://www.huawei.com)",
    plugin_init, VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
