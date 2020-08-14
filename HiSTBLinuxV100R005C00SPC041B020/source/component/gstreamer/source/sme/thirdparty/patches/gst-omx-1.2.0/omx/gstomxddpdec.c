#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include "gstomxddpdec.h"

#if defined (GST_OMX_DDP_DEC_ENABLE)
#include "OMX_Audio_Hisi.h"
#include "OMX_AudioExt.h"

GST_DEBUG_CATEGORY_STATIC (gst_omx_ddp_dec_debug_category);
#define GST_CAT_DEFAULT gst_omx_ddp_dec_debug_category

/* prototypes */
#ifdef GST_DDP_PASSTHROUGH_ONLY_SUPPORT
static void gst_omx_ddp_dec_set_render_flag (GstOMXAudioDec * dec,
                                            gint flag);
#endif
static gboolean gst_omx_ddp_dec_set_format (GstOMXAudioDec * dec,
                                            GstOMXPort * port, GstCaps * caps);
static gboolean gst_omx_ddp_dec_is_format_change (GstOMXAudioDec * dec,
                                                  GstOMXPort * port, GstCaps * caps);
static gint gst_omx_ddp_dec_get_samples_per_frame (GstOMXAudioDec * dec,
                                                   GstOMXPort * port);
static gboolean gst_omx_ddp_dec_get_channel_positions (GstOMXAudioDec * dec,
                                                       GstOMXPort * port, GstAudioChannelPosition position[OMX_AUDIO_MAXCHANNELS]);

static int gst_omx_ddp_dec_get_pcm_data_size(GstOMXAudioDec *dec,
                                             GstOMXPort *port,
                                             GstOMXBuffer *buf);

#if defined (GST_OMX_DDP_DEC_BITSTREAM_ENABLE)
#if defined (GST_OMX_DDP_DEC_REQ_PAD_ENAEBL)
static GstPad * gst_omx_ddp_dec_request_new_pad(GstElement * element,
                                                GstPadTemplate * templ,
                                                const gchar * name,
                                                const GstCaps * caps);
static void gst_omx_ddp_dec_release_pad(GstElement *element,
                                        GstPad *pad);
#endif

static GstCaps* gst_omx_ddp_dec_get_bitstream_srcpad(GstOMXAudioDec *dec,
                                                     GstOMXPort *port);
static GstBuffer* gst_omx_ddp_dec_get_bitstream_buffer(GstOMXAudioDec *dec,
                                                       GstOMXPort *port,
                                                       GstOMXBuffer *buf);
#endif

/* class initialization */

#define DEBUG_INIT                                                      \
    GST_DEBUG_CATEGORY_INIT (gst_omx_ddp_dec_debug_category, "omxddpdec", 0, \
                             "debug category for gst-omx ddp audio decoder");

G_DEFINE_TYPE_WITH_CODE (GstOMXDDPDec, gst_omx_ddp_dec,
                         GST_TYPE_OMX_AUDIO_DEC, DEBUG_INIT);


static void
gst_omx_ddp_dec_class_init (GstOMXDDPDecClass * klass)
{
    GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
    GstOMXAudioDecClass *audiodec_class = GST_OMX_AUDIO_DEC_CLASS (klass);
#if defined (GST_OMX_DDP_DEC_BITSTREAM_ENABLE)
    gchar *bitstream_src_temp_caps = NULL;
    GstCaps *bitstream_src_caps = NULL;
    GstPadTemplate *templ = NULL;
#endif

    audiodec_class->set_format = GST_DEBUG_FUNCPTR (gst_omx_ddp_dec_set_format);
#ifdef GST_DDP_PASSTHROUGH_ONLY_SUPPORT
    audiodec_class->set_render_flag = GST_DEBUG_FUNCPTR (gst_omx_ddp_dec_set_render_flag);
#endif
    audiodec_class->is_format_change =
        GST_DEBUG_FUNCPTR (gst_omx_ddp_dec_is_format_change);
    audiodec_class->get_samples_per_frame =
        GST_DEBUG_FUNCPTR (gst_omx_ddp_dec_get_samples_per_frame);
    audiodec_class->get_channel_positions =
        GST_DEBUG_FUNCPTR (gst_omx_ddp_dec_get_channel_positions);
    audiodec_class->get_pcm_data_size =
        GST_DEBUG_FUNCPTR (gst_omx_ddp_dec_get_pcm_data_size);
#if defined (GST_OMX_DDP_DEC_BITSTREAM_ENABLE)
    audiodec_class->get_bitstream_srcpad =
        GST_DEBUG_FUNCPTR (gst_omx_ddp_dec_get_bitstream_srcpad);
    audiodec_class->get_bitstream_buffer =
        GST_DEBUG_FUNCPTR (gst_omx_ddp_dec_get_bitstream_buffer);
#endif
    audiodec_class->cdata.default_sink_template_caps =
        "audio/ac3, "
        "rate=(int)[32000,48000], "
        "channels=(int)[1,6];"
        "audio/eac3, "
        "rate=(int)[32000,48000], "
        "channels=(int)[1,8]";

#if defined (GST_OMX_DDP_DEC_BITSTREAM_ENABLE)
    /**< initilize bitstream source pad template
     */
    bitstream_src_temp_caps =
        "audio-b/bitstream, "
        "rate = (int) [ 32000, 48000 ], "
        "channels = (int) [ 1, 8 ], "
        "format = (string) {ac3, eac3}" ;

    bitstream_src_caps = gst_caps_from_string (bitstream_src_temp_caps);

    if (bitstream_src_caps == NULL)
    {
        GST_ERROR("fail to parse caps '%s'", bitstream_src_temp_caps);
    }

    templ = gst_pad_template_new ("bitstream-src",
                                  GST_PAD_SRC,
#if defined (GST_OMX_DDP_DEC_REQ_PAD_ENAEBL)
                                  GST_PAD_REQUEST,
#else
                                  GST_PAD_SOMETIMES,
#endif
                                  bitstream_src_caps);

    gst_element_class_add_pad_template (element_class, templ);

#if defined (GST_OMX_DDP_DEC_REQ_PAD_ENAEBL)
    element_class->request_new_pad =
        GST_DEBUG_FUNCPTR(gst_omx_ddp_dec_request_new_pad);
    element_class->release_pad =
        GST_DEBUG_FUNCPTR(gst_omx_ddp_dec_release_pad);
#endif
#endif

    gst_element_class_set_static_metadata (element_class,
                                           "OpenMAX DDP Audio Decoder",
                                           "Codec/Decoder/Audio",
                                           "Decode DDP audio streams",
                                           "anonymous author");

    gst_omx_set_default_role (&audiodec_class->cdata, "audio_decoder.ddp");
}

static void
gst_omx_ddp_dec_init (GstOMXDDPDec * self)
{
    self->spf = -1;
}

#ifdef GST_DDP_PASSTHROUGH_ONLY_SUPPORT
static void
gst_omx_ddp_dec_set_render_flag (GstOMXAudioDec * dec, gint flag)
{
    GstOMXDDPDec *self = GST_OMX_DDP_DEC (dec);
    OMX_ERRORTYPE err;
    OMX_HA_PARAME_PASSTHROUGHOUTPUTSTATUS ddp_param;
    ddp_param.nStatus = flag;
    err =
        gst_omx_component_set_parameter (dec->dec, OMX_HA_EnablePassThroughOutput,
                                         &ddp_param);
    if (err != OMX_ErrorNone) {
        GST_ERROR_OBJECT (self, "Error setting DDP passthrough render_flag parameters: %s (0x%08x)",
                          gst_omx_error_to_string (err), err);
    }
}
#endif

static gboolean
gst_omx_ddp_dec_set_format (GstOMXAudioDec * dec, GstOMXPort * port,
                            GstCaps * caps)
{
    GstOMXDDPDec *self = GST_OMX_DDP_DEC (dec);
    OMX_PARAM_PORTDEFINITIONTYPE port_def;
    OMX_HA_PARAM_DOLBYTYPE ddp_param;
    OMX_ERRORTYPE err;
    GstStructure *s;
    gint rate, channels;

    gst_omx_port_get_port_definition (port, &port_def);
//#ifdef __HI3796MV100__
#if 1
    port_def.format.audio.eEncoding = OMX_AUDIO_CodingAndroidEAC3;
#else
    port_def.format.audio.eEncoding = OMX_AUDIO_CodingDDP;
#endif
    err = gst_omx_port_update_port_definition (port, &port_def);
    if (err != OMX_ErrorNone) {
        GST_ERROR_OBJECT (self,
                          "Failed to set DDP format on component: %s (0x%08x)",
                          gst_omx_error_to_string (err), err);
        return FALSE;
    }

    GST_OMX_INIT_STRUCT (&ddp_param);
    ddp_param.nPortIndex = port->index;

    err =
        gst_omx_component_get_parameter (dec->dec, OMX_HA_IndexParamAudioDolby,
                                         &ddp_param);
    if (err != OMX_ErrorNone) {
        GST_ERROR_OBJECT (self,
                          "Failed to get DDP parameters from component: "
                          "%s (0x%08x)",
                          gst_omx_error_to_string (err), err);
        return FALSE;
    }

    s = gst_caps_get_structure (caps, 0);

    if (!gst_structure_get_int (s, "rate", &rate) ||
        !gst_structure_get_int (s, "channels", &channels)) {
        GST_ERROR_OBJECT (self, "Incomplete caps");
        return FALSE;
    }

    ddp_param.nChannels = channels;
    ddp_param.nBitRate = 0;       /* unknown */
    ddp_param.nSampleRate = rate;

    /** TODO: other parameters such as DRC may need to be set */

    err =
        gst_omx_component_set_parameter (dec->dec, OMX_HA_IndexParamAudioDolby,
                                         &ddp_param);
    if (err != OMX_ErrorNone) {
        GST_ERROR_OBJECT (self, "Error setting DDP parameters: %s (0x%08x)",
                          gst_omx_error_to_string (err), err);
        return FALSE;
    }

    return TRUE;
}

static gboolean
gst_omx_ddp_dec_is_format_change (GstOMXAudioDec * dec, GstOMXPort * port,
                                  GstCaps * caps)
{
    GstOMXDDPDec *self = GST_OMX_DDP_DEC (dec);
    OMX_HA_PARAM_DOLBYTYPE ddp_param;
    OMX_ERRORTYPE err;
    GstStructure *s;
    gint rate, channels;

    GST_OMX_INIT_STRUCT (&ddp_param);
    ddp_param.nPortIndex = port->index;

    err =
        gst_omx_component_get_parameter (dec->dec, OMX_HA_IndexParamAudioDolby,
                                         &ddp_param);
    if (err != OMX_ErrorNone) {
        GST_ERROR_OBJECT (self,
                          "Failed to get DDP parameters from component: "
                          "%s (0x%08x)",
                          gst_omx_error_to_string (err), err);
        return FALSE;
    }

    s = gst_caps_get_structure (caps, 0);

    if (!gst_structure_get_int (s, "rate", &rate) ||
        !gst_structure_get_int (s, "channels", &channels)) {
        GST_ERROR_OBJECT (self, "Incomplete caps");
        return FALSE;
    }

    if (ddp_param.nChannels != channels)
        return TRUE;

    if (ddp_param.nSampleRate != rate)
        return TRUE;

    return FALSE;
}

static gint
gst_omx_ddp_dec_get_samples_per_frame (GstOMXAudioDec * dec, GstOMXPort * port)
{
    return GST_OMX_DDP_DEC (dec)->spf;
}

static gboolean
gst_omx_ddp_dec_get_channel_positions (GstOMXAudioDec * dec,
                                       GstOMXPort * port, GstAudioChannelPosition position[OMX_AUDIO_MAXCHANNELS])
{
    OMX_AUDIO_PARAM_PCMMODETYPE pcm_param;
    OMX_ERRORTYPE err;

    GST_OMX_INIT_STRUCT (&pcm_param);
    pcm_param.nPortIndex = port->index;
    err =
        gst_omx_component_get_parameter (dec->dec, OMX_IndexParamAudioPcm,
                                         &pcm_param);
    if (err != OMX_ErrorNone) {
        GST_ERROR_OBJECT (dec, "Failed to get PCM parameters: %s (0x%08x)",
                          gst_omx_error_to_string (err), err);
        return FALSE;
    }

    switch (pcm_param.nChannels) {
        case 1:
            position[0] = GST_AUDIO_CHANNEL_POSITION_MONO;
            break;
        case 2:
            position[0] = GST_AUDIO_CHANNEL_POSITION_FRONT_LEFT;
            position[1] = GST_AUDIO_CHANNEL_POSITION_FRONT_RIGHT;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

static int gst_omx_ddp_dec_get_pcm_data_size(GstOMXAudioDec *dec,
                                             GstOMXPort *port,
                                             GstOMXBuffer *buf)
{
    GstOMXComponent *comp;
    OMX_ERRORTYPE err = OMX_ErrorNone;
    OMX_COMPONENTTYPE *pComponent = NULL;
    //HI_AUDDATATYPE *pHAData = NULL;

    g_return_val_if_fail (dec != NULL, OMX_ErrorUndefined);
    g_return_val_if_fail (port != NULL, OMX_ErrorUndefined);
    g_return_val_if_fail (buf != NULL, OMX_ErrorUndefined);
    g_return_val_if_fail (buf->port == port, OMX_ErrorUndefined);


    comp = port->comp;
    pComponent = (OMX_COMPONENTTYPE *)comp->handle;

//    pHAData = (HI_AUDDATATYPE*)(pComponent->pComponentPrivate);

    /** FIXME:
     *  the data length should be got from Hisi API, however, it is contained
     *  in some private structure.
     */
    return 6144;
}

#if defined (GST_OMX_DDP_DEC_BITSTREAM_ENABLE)
#if defined (GST_OMX_DDP_DEC_REQ_PAD_ENAEBL)
static GstPad * gst_omx_ddp_dec_request_new_pad(GstElement * element,
                                                GstPadTemplate * templ,
                                                const gchar * name,
                                                const GstCaps * caps)
{
    GstOMXDDPDec *dec = GST_OMX_DDP_DEC(element);
    GstAudioDecoder *audio_decoder = GST_AUDIO_DECODER(dec);
    GstElementClass *klass =
        GST_ELEMENT_CLASS(GST_OMX_DDP_DEC_GET_CLASS(element));
    GstPad *pad = NULL;

    if (templ == gst_element_class_get_pad_template (klass, "bitstream-src"))
    {
        GST_INFO("request new pad of bitstream-src");

        pad = gst_pad_new_from_template(templ, name);
        if (pad == NULL)
        {
            GST_ERROR("Fail to create new pad!");
            return NULL;
        }

        dec->bitstream_output_enable = TRUE;
        audio_decoder->bitstream_srcpad = pad;

        gst_pad_set_active (pad, TRUE);
        gst_pad_set_event_function (pad,
            GST_DEBUG_FUNCPTR (gst_audio_decoder_default_src_event));
        gst_pad_set_query_function (pad,
            GST_DEBUG_FUNCPTR (gst_audio_decoder_default_src_query));

        gst_pad_use_fixed_caps (pad);

        gst_element_add_pad(element, pad);
    }
    else
    {
        GST_ERROR("Not supported template!");
    }

    return pad;
}

static void gst_omx_ddp_dec_release_pad(GstElement *element,
                                        GstPad *pad)
{
    GstOMXDDPDec *dec = GST_OMX_DDP_DEC(element);
    GstAudioDecoder *audio_decoder = GST_AUDIO_DECODER(dec);

    if (gst_element_remove_pad(element, pad))
    {
        dec->bitstream_output_enable = FALSE;
        audio_decoder->bitstream_srcpad = NULL;
    }
    else
    {
        GST_WARNING("Fail to remove pad!");
    }
    return;
}
#endif /* end of GST_OMX_DDP_DEC_REQ_PAD_ENABLE */

static GstCaps* gst_omx_ddp_dec_get_bitstream_srcpad(GstOMXAudioDec *dec,
                                                     GstOMXPort *port)
{
    GstOMXDDPDec *self = NULL;
    GstAudioDecoder *audiodecoder = NULL;
    GstElement *element = NULL;
    GstElementClass *element_class =NULL;
    GstPad *pad = NULL;
    GstPadTemplate *templ = NULL;
    GstCaps *caps = NULL;
    OMX_HA_PARAM_DOLBYTYPE ddp_param;
    OMX_ERRORTYPE err = OMX_ErrorNone;
    HI_AUDDATATYPE *pHAData = NULL;
    GstOMXComponent *comp;
    OMX_COMPONENTTYPE *pComponent;

    g_return_val_if_fail (dec != NULL, NULL);
    g_return_val_if_fail (port != NULL, NULL);

    self = GST_OMX_DDP_DEC (dec);
    audiodecoder= GST_AUDIO_DECODER(self);
    element = GST_ELEMENT(audiodecoder);

    if (audiodecoder->bitstream_srcpad)
    {
        return audiodecoder->bitstream_srcpad;
    }

    GST_WARNING_OBJECT(self, "creating bitstream pad");

    element_class = GST_ELEMENT_GET_CLASS(element);

    comp = port->comp;
    pComponent = (OMX_COMPONENTTYPE *)comp->handle;

    pHAData = (HI_AUDDATATYPE*)(pComponent->pComponentPrivate);
    if (pHAData->stAdec.u32BitstreamOffset == 0)
    {
        return NULL;
    }

    GST_OMX_INIT_STRUCT (&ddp_param);
    ddp_param.nPortIndex = port->index;

    err =
        gst_omx_component_get_parameter (dec->dec, OMX_HA_IndexParamAudioDolby,
                                         &ddp_param);
    if (err != OMX_ErrorNone) {
        GST_ERROR_OBJECT (self,
                          "Failed to get DDP parameters from component: "
                          "%s (0x%08x)",
                          gst_omx_error_to_string (err), err);
        return NULL;
    }

    caps = gst_caps_new_simple("audio-b/bitstream",
                               "rate", G_TYPE_INT, ddp_param.nSampleRate,
                               "channels", G_TYPE_INT, ddp_param.nChannels,
                               /** FIXME: hard code to ac3 */
                               "format", G_TYPE_STRING, "ac3", NULL);
    if (caps == NULL)
    {
        GST_ERROR_OBJECT(self, "fail to create caps for bitstream src pad");
        return NULL;
    }

    templ = gst_element_class_get_pad_template (element_class, "bitstream-src");
    if (templ == NULL)
    {
        GST_ERROR_OBJECT(self, "fail to create templ for bitstream src pad");
        goto templ_fail;
    }

    pad = gst_pad_new_from_template(templ, "bitstream-src");
    if (pad == NULL)
    {
        GST_ERROR_OBJECT(self, "fail to create bitstream src pad");
        goto pad_fail;
    }

    gst_pad_use_fixed_caps (pad);
    (void)gst_pad_set_active (pad, (gboolean)TRUE);
    (void)gst_pad_set_caps (pad, caps);
    gst_pad_set_query_function (pad, gst_audio_decoder_default_src_query);
    gst_pad_set_event_function (pad, gst_audio_decoder_default_src_event);

    audiodecoder->bitstream_srcpad = pad;
    gst_caps_unref (caps);
    (void)gst_element_add_pad (element, pad);

    self->bitstream_output_enable = TRUE;

    gst_element_no_more_pads (GST_ELEMENT (element));

    GST_WARNING_OBJECT(self, "bitstream pad created");

    return pad;

pad_fail:
templ_fail:
    gst_caps_unref(caps);
    return NULL;
}

static GstBuffer* gst_omx_ddp_dec_get_bitstream_buffer(GstOMXAudioDec *dec,
                                                       GstOMXPort *port,
                                                       GstOMXBuffer *buf)
{
    GstOMXDDPDec *self = NULL;
    GstAudioDecoder *audiodecoder = NULL;
    OMX_HA_PARAM_DOLBYTYPE ddp_param = {0};
    OMX_ERRORTYPE err = OMX_ErrorNone;
    HI_AUDDATATYPE *pHAData = NULL;
    GstOMXComponent *comp = NULL;
    OMX_COMPONENTTYPE *pComponent = NULL;
    GstBuffer *bitstream_buf = NULL;
    guint bitstream_len = 0, lbr_len = 0, hbr_len = 0;
    GstAllocator * allocator = NULL;
    GstAllocationParams params = {0};
    GstMapInfo minfo = {0};
    guint8 *buffer_base = NULL;

#define DDP_DUMMY_FRAME_SIZE 4

    g_return_val_if_fail (dec != NULL, NULL);
    g_return_val_if_fail (port != NULL, NULL);

    self = GST_OMX_DDP_DEC (dec);
    audiodecoder= GST_AUDIO_DECODER(self);

    comp = port->comp;
    pComponent = (OMX_COMPONENTTYPE *)comp->handle;

    gst_audio_decoder_get_allocator(audiodecoder, &allocator, &params);

    if (buf == NULL)
    {
        guchar dummy_buf[DDP_DUMMY_FRAME_SIZE] = {0,};
        bitstream_buf = gst_buffer_new_allocate (allocator,
                                                 DDP_DUMMY_FRAME_SIZE,
                                                 &params);
        if(bitstream_buf) {
            GST_BUFFER_FLAG_SET(bitstream_buf, GST_BUFFER_FLAG_DECODE_ONLY);
            GST_BUFFER_PTS(bitstream_buf) = 0;
            GST_BUFFER_DTS(bitstream_buf) = 0;
            GST_BUFFER_DURATION(bitstream_buf) = 0;
            memset(dummy_buf, 0, sizeof(dummy_buf));
            gst_buffer_fill(bitstream_buf, 0,
                            (gconstpointer)dummy_buf,
                            DDP_DUMMY_FRAME_SIZE);
        }

        return bitstream_buf;
    }

    pHAData = (HI_AUDDATATYPE*)(pComponent->pComponentPrivate);
    if (pHAData->stAdec.u32BitstreamOffset == 0)
    {
        return NULL;
    }

    /* The most significant 16 bits represent hbr bitstream length,
     * and the least significant 16 bits represent lbr bitstream length.
     */
    bitstream_len = buf->omx_buf->nFilledLen -
        pHAData->sPcm.nSize * 2 * pHAData->sPcm.nChannels; /* 16 bits sample */
    lbr_len = bitstream_len & 0xffff;
    hbr_len = (bitstream_len & 0xffff0000) >> 16;

    if (bitstream_len <= 0)
    {
        GST_ERROR_OBJECT(self, "no bitstream data");
        goto buffer_fail;
    }

    /** Hisi specific bitstream length has to be transfered to sink element,
     *  thus we insert the length before the valid bitstream data.
     *  It is ugly, however no better method has been found yet.
     */
    bitstream_buf = gst_buffer_new_allocate (allocator,
                                             lbr_len + hbr_len + sizeof(guint),
                                             &params);
    if (bitstream_buf == NULL)
    {
        GST_ERROR_OBJECT(self,
                         "fail to create gst buffer of size %d",
                         bitstream_len);
        goto buffer_fail;
    }

    if (!gst_buffer_map (bitstream_buf, &minfo, GST_MAP_WRITE))
    {
        GST_ERROR_OBJECT(self, "fail to map gst buffer");
        goto map_fail;
    }

    buffer_base = buf->omx_buf->pBuffer +
        buf->omx_buf->nOffset +
        pHAData->stAdec.u32BitstreamOffset;

    /* hisi specific length first and then data */
    memcpy(minfo.data, &bitstream_len, sizeof(guint));
    memcpy(minfo.data + sizeof(guint), buffer_base, lbr_len + hbr_len);

    gst_buffer_unmap(bitstream_buf, &minfo);

    return bitstream_buf;

map_fail:
    gst_buffer_unref(bitstream_buf);

buffer_fail:
    return NULL;
}

#endif /* end of GST_OMX_DDP_DEC_BITSTREAM_ENABLE */

#endif
