/**************************************************************************//**

  Copyright (C), 2003-2014, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmeaudiosink_hihal.c
 * @brief   audio sink element over hal api
 * @author  b00311320
 * @date    2015/12/13
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2015/12/13
 * Author : b00311320
 * Desc   : Created file
 *
******************************************************************************/

#include <sys/time.h>
#include <math.h>
#include <string.h>
#include "tvos_hal_aout.h"
#include "tvos_hal_type.h"

#include "gstsmeaudiosink_hihal.h"
#include "config.h"

#include "osal_type.h"
#include "sme_macro.h"
#include "sme_media_type.h"
#include "sme_log_adpt.h"
#include "osal_thread.h"
#include "osal_mem.h"
#include "gstsme.h"

//lint -e19     //无用的声明
//lint -e438    //unkown
//lint -e522    //unkown
//lint -e585    //unkown
//lint -esym(652,GST_CAT_DEFAULT)
//lint -e717    //do ... while(0)
//lint -e774    //Boolean within 'if' always evaluates to True
//lint -e826    //可疑的指针到指针转换（区域过小）
//lint -e838    //unkown
//lint -e818

#define A_TRACK_VOLUME_LINER_MODE (1)

#define TIMEOUT_CNT 64
//#define BITSOFBYTE 8
#define DEFAULT_SAMPLERATE 48000
#define DEFAULT_CHANNEL 2
#define AOUT_TRACK_BUFFER_MS 300 //300ms
//#define TIMECONV 1000

#define SME_LOG_AVSYNC SME_LOGI_ADPT
#define ENABLE_RENDER_DELAY

#ifdef ENABLE_RENDER_DELAY
#define SME_HIHAL_DEFAULT_RENDER_DELAY (100*GST_MSECOND) //100MS in nano second
#else
#define SME_HIHAL_DEFAULT_RENDER_DELAY (32*GST_MSECOND)//64MS,in nano second
#endif

typedef struct _tag_SME_AUDIO_OUT_HDL_INFO
{
    AOUT_DEVICE_S *pstAoutdev;
    HANDLE psthAout;
    HANDLE psthTrack;
    guint  volume;
    AOUT_TRACK_PARAMS_S stParams;
    V_BOOL bStop;
    V_BOOL bOpen;
    V_UINT64 u64LastTime;
    V_UINT64 u64LastPeroidTime;
#ifdef __DDP_PT__
    gboolean is_ddp_sink;
#endif
} ST_SME_AUDIO_OUT_HDL_INFO, *PST_SME_AUDIO_OUT_HDL_INFO;

#ifdef __DDP_PT__
static GstStaticPadTemplate smeaudiosink_hihal_sink_factory =
    GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw, "
        "format = (string) " GST_AUDIO_FORMATS_ALL ", "
        "layout = (string) interleaved, "
        "rate = (int) [ 1, MAX ], " "channels = (int) [ 1, MAX ]; "
        "audio/bitstream, format = (string){ac3, eac3, truehd};"
        )
    );
#else
static GstStaticPadTemplate smeaudiosink_hihal_sink_factory =
    GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw, "
        "format = (string) " GST_AUDIO_FORMATS_ALL ", "
        "layout = (string) interleaved, "
        "rate = (int) [ 1, MAX ], " "channels = (int) [ 1, MAX ]; "
        )
    );
#endif


static void
gst_smeaudiosink_hihal_class_init(GstSmeAudioSinkHiHalClass *klass);

static void
gst_smeaudiosink_hihal_init(GstSmeAudioSinkHiHal *sink);

static gint gst_smeaudiosink_hihal_create(GstSmeAudiosink2 *sink);

static gint gst_smeaudiosink_hihal_destroy(GstSmeAudiosink2 *sink);

static gint gst_smeaudiosink_hihal_set(GstSmeAudiosink2 *sink,
                                       gint stream_type,
                                       gint sample_rate,
                                       gint channels,
                                       gint bytes_per_sample,
                                       pfnPullAudio fun_callback,
                                       void *context);

static gint gst_smeaudiosink_hihal_start(GstSmeAudiosink2 *sink);

static gint gst_smeaudiosink_hihal_stop(GstSmeAudiosink2 *sink);

static gint gst_smeaudiosink_hihal_pause(GstSmeAudiosink2 *sink);

static gint gst_smeaudiosink_hihal_write(GstSmeAudiosink2 *sink,
                                         const gchar *data,
                                         gint length);

static gint gst_smeaudiosink_hihal_flush(GstSmeAudiosink2 *sink);

static gint gst_smeaudiosink_hihal_set_volume(GstSmeAudiosink2 *sink, guint volume);
static gint gst_smeaudiosink_hihal_get_volume(GstSmeAudiosink2 *sink, guint *volume);
static gint gst_smeaudiosink_hihal_set_mute(GstSmeAudiosink2 *sink, guint mute);
static gint gst_smeaudiosink_hihal_get_mute(GstSmeAudiosink2 *sink, guint *mute);
static gint gst_smeaudiosink_hihal_set_channel_mode(GstSmeAudiosink2 *sink, A_TRACK_CHANNEL_MODE_E mode);
static gint gst_smeaudiosink_hihal_get_channel_mode(GstSmeAudiosink2 *sink, A_TRACK_CHANNEL_MODE_E *mode);
static gint gst_smeaudiosink_hihai_get_render_delay(GstSmeAudiosink2 *bsink, GstClockTime *delay);

#ifdef __DDP_PT__
static gboolean gst_smeaudiosink_hihal_is_ddp_sink(GstSmeAudiosink2 *sink);
#endif

static void *
sme_hihal_aout_create();

static V_VOID
sme_hihal_aout_destroy(INOUT void * hdlAOut);

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_set(
                                IN GstSmeAudioSinkHiHal * hsink,
                                IN void * hdlAOut,
                                IN V_INT32 i32StreamType,
                                IN V_INT32 i32SampleRate,
                                IN V_INT32 i32Channels,
                                IN V_INT32 i32BytesPerSample,
                                IN pfnPullAudio pfnCB,
                                IN V_VOID* pvCtx);

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_start(IN void * hdlAOut);

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_stop(IN void * hdlAOut);

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_pause(IN void * hdlAOut);

static E_SME_AUDIO_PLAYOUT_RET
sme_hihal_aout_write(IN void * hdlAOut,
                        IN const V_UINT8* pu8Data,
                        IN V_UINT32 u32DataLen);

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_set_volume(IN void * hdlAOut, guint volume);
static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_get_volume(IN void * hdlAOut, guint *volume);
static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_set_mute(IN void * hdlAOut, guint mute);
static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_get_mute(IN void * hdlAOut, guint *mute);
static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_set_channel_mode(IN void * hdlAOut, A_TRACK_CHANNEL_MODE_E mode);
static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_get_channel_mode(IN void * hdlAOut, A_TRACK_CHANNEL_MODE_E *mode);
static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_get_render_delay(IN void * hdlAOut, GstClockTime * delay);

#define GST_SMEAUDIOSINK_HIHAL_NAME "smeaudiosink_hihal"
GST_DEBUG_CATEGORY_STATIC (gst_smeaudiosink_hihal_debug);

#ifdef GST_CAT_DEFAULT
#undef GST_CAT_DEFAULT
#endif
#define GST_CAT_DEFAULT gst_smeaudiosink_hihal_debug

#define _do_init \
        GST_DEBUG_CATEGORY_INIT (gst_smeaudiosink_hihal_debug,  \
                                 GST_SMEAUDIOSINK_HIHAL_NAME,   \
                                 0, "smehisihalaudiosink element");
#define gst_smeaudiosink_hihal_parent_class parent_class

G_DEFINE_TYPE_WITH_CODE (GstSmeAudioSinkHiHal,
                         gst_smeaudiosink_hihal,
                         GST_TYPE_SME_AUDIOSINK2,
                         _do_init);

static void
gst_smeaudiosink_hihal_class_init(GstSmeAudioSinkHiHalClass *klass)
{
    GstElementClass *gstelement_class = NULL;
    GstSmeAudiosink2Class *gstsmeaudiosink2_class = NULL;
    GstPadTemplate* sinktempl = NULL;

    (void)parent_class;

    gstelement_class = GST_ELEMENT_CLASS(klass);
    gstsmeaudiosink2_class = GST_SME_AUDIOSINK2_CLASS(klass);

    /* gobject method */

    gst_element_class_set_static_metadata (gstelement_class,
        "Sme Audio Sink Hisi Hal",
        "Sink/Audio",
        "Sme Audio sink element based on Hisi Hal",
        "SME TEAM ");

    sinktempl = gst_static_pad_template_get (&smeaudiosink_hihal_sink_factory);
    gst_element_class_add_pad_template (gstelement_class, sinktempl);

    gstsmeaudiosink2_class->create =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_create);
    gstsmeaudiosink2_class->destroy =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_destroy);
    gstsmeaudiosink2_class->set =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_set);
    gstsmeaudiosink2_class->start =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_start);
    gstsmeaudiosink2_class->stop =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_stop);
    gstsmeaudiosink2_class->pause =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_pause);
    gstsmeaudiosink2_class->write =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_write);
    gstsmeaudiosink2_class->flush =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_flush);
    gstsmeaudiosink2_class->set_volume =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_set_volume);
    gstsmeaudiosink2_class->get_volume =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_get_volume);
    gstsmeaudiosink2_class->set_mute =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_set_mute);
    gstsmeaudiosink2_class->get_mute =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_get_mute);
    gstsmeaudiosink2_class->set_channel_mode =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_set_channel_mode);
    gstsmeaudiosink2_class->get_channel_mode =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_get_channel_mode);
    gstsmeaudiosink2_class->set_hdmi_mode = NULL;
    gstsmeaudiosink2_class->get_hdmi_mode = NULL;
#ifdef __DDP_PT__
    gstsmeaudiosink2_class->is_ddp_sink =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihal_is_ddp_sink);
#endif
    gstsmeaudiosink2_class->get_delay =
        GST_DEBUG_FUNCPTR(gst_smeaudiosink_hihai_get_render_delay);
}


static void
gst_smeaudiosink_hihal_init(GstSmeAudioSinkHiHal *sink)
{
    (void)sink;
    gst_smeaudiosink_hihal_get_instance_private(sink);
    gst_base_sink_set_sync (GST_BASE_SINK (sink), TRUE);
    gst_base_sink_set_render_delay(GST_BASE_SINK(sink),
                                   SME_HIHAL_DEFAULT_RENDER_DELAY);
}

static gint gst_smeaudiosink_hihal_create(GstSmeAudiosink2 *sink)
{
    gint ret = E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink!=NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    if (hsink->aout_handle)
    {
        GST_INFO("Destroy the existed hisi hal aout handle");
        sme_hihal_aout_destroy(hsink->aout_handle);
        hsink->aout_handle = NULL;
    }

    hsink->aout_handle = sme_hihal_aout_create();
    if (hsink->aout_handle == NULL)
    {
        GST_WARNING("Fail to create hisi hal aout handle");
        ret = E_SME_AUDIO_PLAYOUT_RET_FATAL;
    }

    return ret;
}

static gint gst_smeaudiosink_hihal_destroy(GstSmeAudiosink2 *sink)
{
    gint ret = E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink!=NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    if (hsink->aout_handle)
    {
        GST_INFO("Destroy hisi hal aout handle");
        sme_hihal_aout_destroy(hsink->aout_handle);
        hsink->aout_handle = NULL;
    }
    else
    {
        GST_WARNING("NULL, hisi hal aout handle");
        ret = E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG;
    }

    return ret;
}

static gint gst_smeaudiosink_hihal_set(GstSmeAudiosink2 *sink,
                                       gint stream_type,
                                       gint sample_rate,
                                       gint channels,
                                       gint bytes_per_sample,
                                       pfnPullAudio fun_callback,
                                       void *context)
{
    gint ret;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    g_return_val_if_fail((hsink->aout_handle != NULL),
                         E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    ret = sme_hihal_aout_set(hsink,hsink->aout_handle,
                             stream_type,
                             sample_rate,
                             channels,
                             bytes_per_sample,
                             fun_callback,
                             context);

    return ret;
}

static gint gst_smeaudiosink_hihal_start(GstSmeAudiosink2 *sink)
{
    gint ret;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    g_return_val_if_fail((hsink->aout_handle != NULL),
                         E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    ret = sme_hihal_aout_start(hsink->aout_handle);

    return ret;
}

static gint gst_smeaudiosink_hihal_stop(GstSmeAudiosink2 *sink)
{
    gint ret;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    g_return_val_if_fail((hsink->aout_handle != NULL),
                         E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    ret = sme_hihal_aout_stop(hsink->aout_handle);

    return ret;
}

static gint gst_smeaudiosink_hihal_pause(GstSmeAudiosink2 *sink)
{
    gint ret;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    g_return_val_if_fail((hsink->aout_handle != NULL),
                         E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    ret = sme_hihal_aout_pause(hsink->aout_handle);

    return ret;
}

static gint gst_smeaudiosink_hihal_write(GstSmeAudiosink2 *sink,
                                         const gchar *data,
                                         gint length)
{
    gint ret;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    g_return_val_if_fail((hsink->aout_handle != NULL),
                         E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    ret = sme_hihal_aout_write(hsink->aout_handle,
                                  (const guchar *)data,
                                  (guint)length);

    return ret;
}

static gint gst_smeaudiosink_hihal_flush(GstSmeAudiosink2 *sink)
{
    gint ret = E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
    (void)sink;
    return ret;
}

static gint gst_smeaudiosink_hihal_set_volume(GstSmeAudiosink2 *sink, guint volume)
{
    gint ret;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    g_return_val_if_fail((hsink->aout_handle != NULL),
                         E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    ret = sme_hihal_aout_set_volume(hsink->aout_handle,volume);

    return ret;
}

static gint gst_smeaudiosink_hihal_get_volume(GstSmeAudiosink2 *sink, guint *volume)
{
    gint ret;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    g_return_val_if_fail((hsink->aout_handle != NULL),
                         E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    ret = sme_hihal_aout_get_volume(hsink->aout_handle,volume);

    return ret;
}

static gint gst_smeaudiosink_hihal_set_mute(GstSmeAudiosink2 *sink, guint mute)
{
    gint ret;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    g_return_val_if_fail((hsink->aout_handle != NULL),
                         E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    ret = sme_hihal_aout_set_mute(hsink->aout_handle,mute);

    return ret;
}

static gint gst_smeaudiosink_hihal_get_mute(GstSmeAudiosink2 *sink, guint *mute)
{
    gint ret;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    g_return_val_if_fail((hsink->aout_handle != NULL),
                         E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    ret = sme_hihal_aout_get_mute(hsink->aout_handle,mute);

    return ret;
}

static gint gst_smeaudiosink_hihal_set_channel_mode(GstSmeAudiosink2 *sink, A_TRACK_CHANNEL_MODE_E mode)
{
    gint ret;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    g_return_val_if_fail((hsink->aout_handle != NULL),
                         E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    ret = sme_hihal_aout_set_channel_mode(hsink->aout_handle,mode);

    return ret;
}

static gint gst_smeaudiosink_hihal_get_channel_mode(GstSmeAudiosink2 *sink, A_TRACK_CHANNEL_MODE_E *mode)
{
    gint ret;
    GstSmeAudioSinkHiHal *hsink = NULL;

    g_return_val_if_fail((sink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    g_return_val_if_fail((hsink->aout_handle != NULL),
                         E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    ret = sme_hihal_aout_get_channel_mode(hsink->aout_handle,mode);

    return ret;
}



static V_INT32
sme_hihal_init_aout_module(AOUT_DEVICE_S **ppstAoutdev)
{
#ifndef LINUX_OS
    hw_module_t const* pstModule;
#endif
    V_INT32 i32Ret;
    PST_SME_LOG_CB pstThisLogCtx = NULL;

#ifndef LINUX_OS

    if (hw_get_module(AOUT_HARDWARE_MODULE_ID, &pstModule) != 0){
        SME_LOGE_ADPT(" %s module not found", AOUT_HARDWARE_MODULE_ID);
        return FAILURE;
    }

    i32Ret = aout_open(pstModule, ppstAoutdev);
    if (FAILURE == i32Ret)
#else
    *ppstAoutdev = getAOutDevice();
    i32Ret = 0;

    if (NULL == *ppstAoutdev)
#endif
    {
        SME_LOGE_ADPT("%s device failed to initialize (%s)\n",
                      AOUT_HARDWARE_AOUT0, strerror(i32Ret));
        //goto ERR;
        *ppstAoutdev = NULL;
        return i32Ret;
    }

    (void)pstThisLogCtx;

    return SUCCESS;

//ERR:
//    *ppstAoutdev = NULL;
//    return i32Ret;
}

static V_INT32
sme_hihal_deinit_aout_module(const AOUT_DEVICE_S *pstAoutdev)
{
    g_return_val_if_fail((pstAoutdev != NULL), FAILURE);
#ifndef LINUX_OS
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    V_INT32 i32Ret;
    i32Ret = aout_close (pstAoutdev);
    if (FAILURE == i32Ret) {
        SME_LOGE_ADPT("%s device failed to deinit (%s)\n",
                      AOUT_HARDWARE_AOUT0, strerror(i32Ret));
        return i32Ret;
    }
#endif
    return SUCCESS;

}

#ifdef __DDP_PT__
static gboolean gst_smeaudiosink_hihal_is_ddp_sink(GstSmeAudiosink2 *sink)
{

    GstSmeAudioSinkHiHal *hsink = NULL;
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL;

    g_return_val_if_fail((sink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    hsink = GST_SME_AUDIOSINK_HIHAL(sink);

    g_return_val_if_fail((hsink->aout_handle != NULL),
                         E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)(hsink->aout_handle);

    return pstAuOutHdlInfo->is_ddp_sink;

}
#endif



static void * sme_hihal_aout_create()
{
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_OPEN_PARAMS_S stOpenParams;
    AOUT_CAPABILITY_S stCapability;
    AOUT_DEVICE_S *pstAoutdev = NULL;
    V_INT32 i32Ret = ICS_FAIL;
    E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;
    //V_INT32 i = 0;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)
        malloc(sizeof(ST_SME_AUDIO_OUT_HDL_INFO));
    UTILS_MLOGE_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                           NULL,
                           ("pstAuOutHdlInfo Malloc Failed!"))

    eMemSecRet = VOS_Memset_S(pstAuOutHdlInfo, sizeof(ST_SME_AUDIO_OUT_HDL_INFO), 0, sizeof(ST_SME_AUDIO_OUT_HDL_INFO));
    if(eMemSecRet != E_SME_MEM_OK)
    {
        GST_ERROR("VOS_Memset_S failed:%d", eMemSecRet);
    }

    // load aout module
    SME_LOGI_ADPT("enter sme_hihal_aout_create");
    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;
    i32Ret = sme_hihal_init_aout_module(&pstAoutdev);
    if(SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("error, hisi hal aout open failed!");
        UTILS_MSAFEFREE(pstAuOutHdlInfo);
        return NULL;
    }
    pstAuOutHdlInfo->pstAoutdev = pstAoutdev;
    pstAuOutHdlInfo->volume = 100;
    SME_LOGI_ADPT("load aout module success!pstAoutdev:%p",pstAoutdev);

    i32Ret = pstAoutdev->aout_init(NULL, NULL);
    if (SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("error, aout init failed!");
        sme_hihal_deinit_aout_module(pstAoutdev);
        UTILS_MSAFEFREE(pstAuOutHdlInfo);
        return NULL;
    }
    SME_LOGI_ADPT("ok, aout init success!");

    //get aout support track num
    i32Ret = pstAoutdev->aout_get_capability(NULL, &stCapability);
    if(SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("aout_get_capability failed 0x%x",i32Ret);
        sme_hihal_deinit_aout_module(pstAoutdev);
        pstAoutdev->aout_term(NULL,NULL);
        UTILS_MSAFEFREE(pstAuOutHdlInfo);
        return NULL;
    }

/*
    //for(i = 0; i < stCapability.u8SupportedId; i++)
    for (i = 0; i < stCapability.u8SupportedIdNum; i++)
    {
        SME_LOGE_ADPT("%d aOutputDevice:%d u8SupportedTrackNum:%d",
                      i,
                      //stCapability.aenOutputDevice[i],
                      stCapability.enOutputDevice[i],
                      stCapability.u8SupportedTrackNum[i]);
    }
*/

    stOpenParams.enId = AOUT_ID_0;
    stOpenParams.stSettings.enOutputDevice = AOUT_DEVICE_ALL;
    i32Ret = pstAoutdev->aout_open(NULL,
                                   &(pstAuOutHdlInfo->psthAout),
                                   &stOpenParams);
    if(SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("aout_open failed 0x%x",i32Ret);
        sme_hihal_deinit_aout_module(pstAoutdev);
        pstAoutdev->aout_term(NULL,NULL);
        UTILS_MSAFEFREE(pstAuOutHdlInfo);
        return NULL;
    }

    /*to create an hTrack*/
#ifdef LINUX_OS
    pstAuOutHdlInfo->stParams.enFormat = AOUT_DATA_FORMAT_LE_PCM_16_BIT;
#else
    pstAuOutHdlInfo->stParams.enFormat = 0;
#endif
    pstAuOutHdlInfo->stParams.u32SampleRate = DEFAULT_SAMPLERATE;
    pstAuOutHdlInfo->stParams.u32Channels = DEFAULT_CHANNEL;
    pstAuOutHdlInfo->stParams.u32BufferSize =
        DEFAULT_CHANNEL * 2 * DEFAULT_SAMPLERATE * AOUT_TRACK_BUFFER_MS /1000; //300ms

    i32Ret = pstAoutdev->track_create(NULL,
                                      pstAuOutHdlInfo->psthAout,
                                      &(pstAuOutHdlInfo->psthTrack),
                                      &(pstAuOutHdlInfo->stParams));
    if(SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("track_create failed 0x%x", i32Ret);
        sme_hihal_deinit_aout_module(pstAoutdev);
        pstAoutdev->aout_term(NULL, NULL);
        pstAoutdev->aout_close(NULL, pstAuOutHdlInfo->psthAout, NULL);
        UTILS_MSAFEFREE(pstAuOutHdlInfo);
        return NULL;
    }
    SME_LOGI_ADPT("track_create ghTrack:0x%x ok.",pstAuOutHdlInfo->psthTrack);

    pstAuOutHdlInfo->bStop = ICS_FALSE;
    pstAuOutHdlInfo->bOpen = ICS_TRUE;
    pstAuOutHdlInfo->u64LastTime = 0;
    pstAuOutHdlInfo->u64LastPeroidTime = 0;

    (void)pstThisLogCtx;

    return (void *)pstAuOutHdlInfo;
}

static V_VOID sme_hihal_aout_destroy(INOUT void * hdlAOut)
{
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL;
    HANDLE psthTrack = 0;
    AOUT_DEVICE_S *pstAoutdev = NULL;
    PST_SME_LOG_CB pstThisLogCtx = NULL;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_M_RET_IF(NULL == pstAuOutHdlInfo);

    psthTrack = pstAuOutHdlInfo->psthTrack;
    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;

    SME_LOGI_ADPT("sme_hihal_aout_destroy");

    if(pstAuOutHdlInfo->bOpen)
    {
        pstAoutdev->track_destroy(NULL, psthTrack);
        pstAoutdev->aout_close(NULL, pstAuOutHdlInfo->psthAout, NULL);
        pstAoutdev->aout_term(NULL, NULL);
        sme_hihal_deinit_aout_module(pstAoutdev);
    }

    free(pstAuOutHdlInfo);
    pstAuOutHdlInfo = NULL;
    (void)pstThisLogCtx;

    return ;
}

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_set(
    IN GstSmeAudioSinkHiHal * hsink,
    IN void * hdlAOut,
    IN V_INT32 i32StreamType,
    IN V_INT32 i32SampleRate,
    IN V_INT32 i32Channels,
    IN V_INT32 i32BytesPerSample,
    IN pfnPullAudio pfnCB,
    IN V_VOID* pvCtx)
{
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev = NULL;
    V_INT32 i32Ret;

    //AOUT_CAPABILITY_S stCapability;
    //V_INT32 i = 0;


    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);


    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;

#ifdef __DDP_PT__
    pstAuOutHdlInfo->is_ddp_sink = FALSE;
#endif

    SME_LOGW_ADPT("sme_hihal_aout_set, i32SampleRate:%d,"
                  " i32Channels:%d, i32BytesPerSample:%d",
                  i32SampleRate,i32Channels, i32BytesPerSample);

#ifdef __DDP_PT__
    pstAuOutHdlInfo->stParams.u32SampleRate = (V_UINT32)i32SampleRate;
    pstAuOutHdlInfo->stParams.u32Channels = (V_UINT32)i32Channels;
    pstAuOutHdlInfo->stParams.u32BufferSize =
        (V_UINT32)i32Channels * 2 * (V_UINT32)i32SampleRate * AOUT_TRACK_BUFFER_MS /1000;    //300ms

    if(E_SME_AUDIO_RAW_FMT_PT_AC3 == i32BytesPerSample)
    {
        pstAuOutHdlInfo->stParams.enFormat = AOUT_DATA_FORMAT_AC3;
        pstAuOutHdlInfo->stParams.bPtsSync = TRUE;
        pstAuOutHdlInfo->is_ddp_sink = TRUE;
        SME_LOGW_ADPT("AUDIO_FORMAT_AC3, %p",hdlAOut);
    }
    else if(E_SME_AUDIO_RAW_FMT_PT_E_AC3 == i32BytesPerSample)
    {
        pstAuOutHdlInfo->stParams.enFormat = AOUT_DATA_FORMAT_E_AC3;
        pstAuOutHdlInfo->stParams.bPtsSync = TRUE;
        pstAuOutHdlInfo->is_ddp_sink = TRUE;
        SME_LOGW_ADPT("AUDIO_FORMAT_E_AC3, %p",hdlAOut);
    }
    else {
        pstAuOutHdlInfo->stParams.enFormat = AOUT_DATA_FORMAT_LE_PCM_16_BIT;
        pstAuOutHdlInfo->is_ddp_sink = FALSE;
        SME_LOGE_ADPT("set: data fmt use default, 16bit pcm, %p",hdlAOut);
    }
    if (pstAuOutHdlInfo->is_ddp_sink)
    {
        gst_base_sink_set_render_delay(GST_BASE_SINK(hsink),
            SME_HIHAL_DEFAULT_RENDER_DELAY*40);
    } else {
        gst_base_sink_set_render_delay(GST_BASE_SINK(hsink),
            SME_HIHAL_DEFAULT_RENDER_DELAY);
    }
#else
#ifdef LINUX_OS
    pstAuOutHdlInfo->stParams.enFormat = AOUT_DATA_FORMAT_LE_PCM_16_BIT;
#else
    pstAuOutHdlInfo->stParams.enFormat = 0;
#endif
    pstAuOutHdlInfo->stParams.u32SampleRate = (V_UINT32)i32SampleRate;
    pstAuOutHdlInfo->stParams.u32Channels = (V_UINT32)i32Channels;
    pstAuOutHdlInfo->stParams.u32BufferSize =
        (V_UINT32)i32Channels * 2 * (V_UINT32)i32SampleRate * AOUT_TRACK_BUFFER_MS /1000;    //300ms
#endif

    SME_LOGW_ADPT("begin to track_stop pstAoutdev:%p, track handle:0x%x",
                  pstAoutdev,
                  pstAuOutHdlInfo->psthTrack);

#ifdef __DDP_PT__
    AOUT_OPEN_PARAMS_S stOpenParams;
    i32Ret = pstAoutdev->track_stop(pstAoutdev, pstAuOutHdlInfo->psthTrack);
    if (SUCCESS != i32Ret) {
      SME_LOGE_ADPT("track stop failed :%d, can not set params",i32Ret);
      return E_SME_AUDIO_PLAYOUT_RET_INVALID_OPERATION;
    }

    pstAoutdev->track_destroy(NULL, pstAuOutHdlInfo->psthTrack);
    pstAuOutHdlInfo->psthTrack = NULL;
    pstAoutdev->aout_close(NULL, pstAuOutHdlInfo->psthAout, NULL);
    pstAoutdev->aout_term(NULL, NULL);


    i32Ret = pstAoutdev->aout_init(NULL, NULL);
    if (SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("error, aout init failed!");
        return E_SME_AUDIO_PLAYOUT_RET_INVALID_OPERATION;
    }

    stOpenParams.enId = AOUT_ID_0;
    stOpenParams.stSettings.enOutputDevice = AOUT_DEVICE_ALL;
    i32Ret = pstAoutdev->aout_open(NULL,
                                   &(pstAuOutHdlInfo->psthAout),
                                   &stOpenParams);
    if(SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("aout_open failed 0x%x",i32Ret);
        return E_SME_AUDIO_PLAYOUT_RET_INVALID_OPERATION;
    }
    i32Ret = pstAoutdev->track_create(NULL,
                                      pstAuOutHdlInfo->psthAout,
                                      &(pstAuOutHdlInfo->psthTrack),
                                      &(pstAuOutHdlInfo->stParams));
    if (SUCCESS != i32Ret) {
      SME_LOGE_ADPT("track set params failed :%d, track handle:0x%x", i32Ret,pstAuOutHdlInfo->psthTrack);
      return E_SME_AUDIO_PLAYOUT_RET_INVALID_OPERATION;
    }
    SME_LOGW_ADPT("track_set_params ok.");
#else
    i32Ret = pstAoutdev->track_stop(pstAoutdev, pstAuOutHdlInfo->psthTrack);
    if (SUCCESS != i32Ret) {
      SME_LOGE_ADPT("track stop failed :%d, can not set params",i32Ret);
      return E_SME_AUDIO_PLAYOUT_RET_INVALID_OPERATION;
    }

    SME_LOGI_ADPT("track_stop ok, begin to set params, BufferSize:%u",
                  pstAuOutHdlInfo->stParams.u32BufferSize);
    i32Ret = pstAoutdev->track_set_params(pstAoutdev,
                                          pstAuOutHdlInfo->psthTrack,
                                          &(pstAuOutHdlInfo->stParams));
    if (SUCCESS != i32Ret) {
      SME_LOGE_ADPT("track set params failed :%d", i32Ret);
      return E_SME_AUDIO_PLAYOUT_RET_INVALID_OPERATION;
    }
    SME_LOGI_ADPT("track_set_params ok.");
#endif
    UTILS_M_INVALID_ARGS(i32StreamType);
    UTILS_M_INVALID_ARGS(pfnCB);
    UTILS_M_INVALID_ARGS(pvCtx);

    (void)hsink;
    (void)pstThisLogCtx;
    return E_SME_AUDIO_PLAYOUT_RET_SUCCESS;

}

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_start(IN void * hdlAOut)
{

    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL ;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev = NULL;
    V_INT32 i32Ret;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    pstAuOutHdlInfo->bStop = ICS_FALSE;

    SME_LOGI_ADPT(" sme_hihal_aout_start : start");
    (void)pstThisLogCtx;
    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;
    i32Ret = pstAoutdev->track_start(pstAoutdev, pstAuOutHdlInfo->psthTrack);
    if (SUCCESS != i32Ret) {
      SME_LOGE_ADPT("track start failed :%d, can not set params",i32Ret);
      return E_SME_AUDIO_PLAYOUT_RET_INVALID_OPERATION;
    }

    return E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
}

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_stop(IN void * hdlAOut)
{
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL ;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev = NULL;
    V_INT32 i32Ret;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    pstAuOutHdlInfo->bStop = ICS_TRUE;

    SME_LOGI_ADPT(" sme_hihal_aout_stop : stop");
    (void)pstThisLogCtx;
    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;
    i32Ret = pstAoutdev->track_stop(pstAoutdev, pstAuOutHdlInfo->psthTrack);
    if (SUCCESS != i32Ret) {
      SME_LOGE_ADPT("track stop failed :%d, can not set params",i32Ret);
      return E_SME_AUDIO_PLAYOUT_RET_INVALID_OPERATION;
    }

    return E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
}

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_pause(IN void * hdlAOut)
{
#ifdef __LINUX__
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL ;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev = NULL;
    V_INT32 i32Ret;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    pstAuOutHdlInfo->bStop = ICS_TRUE;

    SME_LOGI_ADPT("sme_hihal_aout_pause : pause");
    (void)pstThisLogCtx;
    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;
    i32Ret = pstAoutdev->track_pause(pstAoutdev, pstAuOutHdlInfo->psthTrack);
    if (SUCCESS != i32Ret) {
      SME_LOGE_ADPT("track pause failed :%d, can not set params",i32Ret);
      return E_SME_AUDIO_PLAYOUT_RET_INVALID_OPERATION;
    }
#endif
    return E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
}

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_write(IN void * hdlAOut,
                                                       IN const V_UINT8* pu8Data,
                                                       IN V_UINT32 u32DataLen)
{
    V_INT32 i32Ret = ICS_FAIL;
    E_SME_AUDIO_PLAYOUT_RET eRet = E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev;
    V_INT32 i32Count = 0;
    V_UINT32 u32AvailBytes = 0;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    UTILS_RET_VAL_IF(NULL == pu8Data, E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    UTILS_RET_VAL_IF(0 == u32DataLen, E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);


    SME_LOGD_ADPT("track_write, u32DataLen:%d"
                  " data:0x%x, 0x%x, 0x%x, 0x%x",
                  u32DataLen,
                  *((V_UINT32 *)pu8Data+1), *((V_UINT32 *)pu8Data+2),
                  *((V_UINT32 *)pu8Data+3), *((V_UINT32 *)pu8Data+4));

    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;
    while(!(pstAuOutHdlInfo->bStop) && (i32Count++) < TIMEOUT_CNT)
    {
        //1.get available size
        i32Ret = pstAoutdev->track_get_buf_avail(NULL,
                                                 pstAuOutHdlInfo->psthTrack,
                                                 &u32AvailBytes);
        if(SUCCESS != i32Ret)
        {
            SME_LOGE_ADPT("track_get_buf_avail failed 0x%x\n",i32Ret);
            eRet = E_SME_AUDIO_PLAYOUT_RET_FATAL;
            //goto try_again;
            VOS_MSleep((V_UINT32)20);
            continue;
        }
        SME_LOGD_ADPT("track_get_buf_avail:%d",u32AvailBytes);
        //2. send data
        if (u32AvailBytes >= u32DataLen)
        {
            SME_LOGD_ADPT("track_write: before");
            i32Ret = pstAoutdev->track_write(NULL,
                                             pstAuOutHdlInfo->psthTrack,
                                             pu8Data,
                                             u32DataLen);
            SME_LOGD_ADPT("track_write: return %d", i32Ret);
            if (SUCCESS == i32Ret)
            {
                eRet = E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
                break;
            }else {
                SME_LOGE_ADPT("track write failed: return %d",i32Ret);
                //goto try_again;
                VOS_MSleep((V_UINT32)20);
                continue;
            }
        }

      //try_again:
      //  VOS_MSleep((V_UINT32)20);

    }

    (void)pstThisLogCtx;

    return eRet;
}

#if A_TRACK_VOLUME_LINER_MODE
static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_set_volume(IN void * hdlAOut, guint volume)
{
#ifdef LINUX_OS
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL ;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev;
    V_INT32 i32Ret;

    TRACK_GAIN_ATTR_S stTrackGainAttr;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    if (volume > 100)
    {
        volume = 100;
    }

    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;

    stTrackGainAttr.bLinearMode = TRUE;
    stTrackGainAttr.s32Gain = (gint)volume;

    i32Ret = pstAoutdev->track_set_weight(pstAoutdev,
                                         pstAuOutHdlInfo->psthTrack,
                                         stTrackGainAttr);

    if(SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("sme_hihal_aout_set_volume failed: volume %u,  errcode 0x%x",volume,i32Ret);
    } else
    {
        pstAuOutHdlInfo->volume = volume;
        SME_LOGI_ADPT("sme_hihal_aout_set_volume ok: volume %u",volume);
    }

    (void)pstThisLogCtx;
#endif
    return E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
}

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_get_volume(IN void * hdlAOut, guint *volume)
{
#ifdef LINUX_OS
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL ;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev;
    V_INT32 i32Ret;
    TRACK_GAIN_ATTR_S stTrackGainAttr;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;

    i32Ret = pstAoutdev->track_get_weight(pstAoutdev,
                                         pstAuOutHdlInfo->psthTrack,
                                         &stTrackGainAttr);

    if(SUCCESS != i32Ret)
    {
        *volume = pstAuOutHdlInfo->volume;
        SME_LOGE_ADPT("sme_hihal_aout_get_volume failed:  volume %u, , errcode 0x%x",*volume,i32Ret);
    } else
    {
        *volume = (guint)stTrackGainAttr.s32Gain;
        pstAuOutHdlInfo->volume = *volume;
        SME_LOGI_ADPT("sme_hihal_aout_get_volume ok: volume %u, liner mode %s,",*volume,stTrackGainAttr.bLinearMode?"yes":"no");
    }

    (void)pstThisLogCtx;
#endif
    return E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
}


#else
#define DB_MAX (AOUT_VOL_DB_ZERO)
#define DB_MIN (-50)
//#define DB_MIN (AOUT_VOL_DB_MIN)

static void volume_to_db(guint volume, gint* s32IntGain, gint* s32DecGain)
{
    // convert volume steps to natural log scale
    // change this value to change volume scaling
    const int dBDuration = (DB_MAX - DB_MIN);
    //static const float dBPerStep = 0.50f;
    const float dBPerStep = dBDuration/100.0;
    // shouldn't need to touch these
    const float dBConvert = -dBPerStep * 2.302585093f / 20.0f;
    const float dBConvertInverse = 1.0f / dBConvert;

    float db;

    if (volume < 0)
    {
        volume = 0;
    } else if (volume > 100)
    {
        volume = 100;
    }

#if 0
    db = volume ? exp((float)(100 - volume) * dBConvert) : 0; // 0.0 ~ 1.0
#else
    db = volume ? (100 - (int)(dBConvertInverse * log(volume/100.0) + 0.5))/100.0 : 0;
#endif

    db = db*dBDuration + DB_MIN;

    if (db <= 0)
    {
        *s32IntGain = (gint)(-db);
        *s32DecGain = (-db - *s32IntGain)/0.125f; // 0 ~ 7
        *s32IntGain = -(*s32IntGain);               // DB_MIN ~ DB_MAX
    } else {
        *s32IntGain = (gint)(db);               // DB_MIN ~ DB_MAX
        *s32DecGain = (db - *s32IntGain)/0.125f; // 0 ~ 7
    }

}

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_set_volume(IN void * hdlAOut, guint volume)
{

    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL ;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev;
    V_INT32 i32Ret;
    gint s32IntGain;
    gint s32DecGain;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;

    volume_to_db(volume,&s32IntGain,&s32DecGain);

    i32Ret = pstAoutdev->aout_set_volume(pstAoutdev,
                                         pstAuOutHdlInfo->psthAout,
                                         AOUT_DEVICE_ALL,
                                         s32IntGain,
                                         s32DecGain);

    if(SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("sme_hihal_aout_set_volume failed: volume %u, db %d/%d, errcode 0x%x",volume,s32IntGain,s32DecGain,i32Ret);
    } else
    {
        pstAuOutHdlInfo->volume = volume;
        SME_LOGI_ADPT("sme_hihal_aout_set_volume ok: volume %u, db %d/%d",volume,s32IntGain,s32DecGain);
    }

    (void)pstThisLogCtx;

    return E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
}

static void  volume_from_db(guint* volume, gint s32IntGain,gint s32DecGain )
{
    // convert volume steps to natural log scale
    // change this value to change volume scaling
    const int dBDuration = (DB_MAX - DB_MIN);
    //static const float dBPerStep = 0.50f;
    const float dBPerStep = dBDuration/100.0;
    // shouldn't need to touch these
    const float dBConvert = -dBPerStep * 2.302585093f / 20.0f;
    const float dBConvertInverse = 1.0f / dBConvert;

    float db;
    float volume_log;

    if (s32DecGain < 0)
    {
        s32DecGain = 0;
    } else if (s32DecGain > 7)
    {
        s32DecGain = 7;
    }

    if (s32IntGain < DB_MIN)
    {
        s32IntGain = DB_MIN;
        s32DecGain = 0;
    } else if (s32IntGain > DB_MAX)
    {
        s32IntGain = DB_MAX;
        s32DecGain = 0;
    }

    if (s32IntGain <= 0)
    {
        db = -(-s32IntGain + s32DecGain*0.125f);
    } else {
        db = s32IntGain + s32DecGain*0.125f;
    }

    volume_log = (db-DB_MIN)/dBDuration;

#if 0
    *volume = volume_log ? 100 - (int)(dBConvertInverse * log(volume_log) + 0.5) : 0;
#else
    *volume = volume_log ? exp((float)(100 - volume_log) * dBConvert) : 0; // 0.0 ~ 1.0
#endif

}


static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_get_volume(IN void * hdlAOut, guint *volume)
{

    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL ;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev;
    V_INT32 i32Ret;
    gint s32IntGain;
    gint s32DecGain;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;

    i32Ret = pstAoutdev->aout_get_volume(pstAoutdev,
                                         pstAuOutHdlInfo->psthAout,
                                         AOUT_DEVICE_ALL,
                                         &s32IntGain,
                                         &s32DecGain);

    if(SUCCESS != i32Ret)
    {
        *volume = pstAuOutHdlInfo->volume;
        SME_LOGE_ADPT("sme_hihal_aout_get_volume failed:  volume %u, , errcode 0x%x",*volume,i32Ret);
    } else
    {
        volume_from_db(volume,s32IntGain,s32DecGain);
        pstAuOutHdlInfo->volume = *volume;
        SME_LOGI_ADPT("sme_hihal_aout_get_volume ok: volume %u, db %d/%d",*volume,s32IntGain,s32DecGain);
    }

    (void)pstThisLogCtx;

    return E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
}
#endif

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_set_mute(IN void * hdlAOut, guint mute)
{
#ifdef LINUX_OS
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL ;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev;
    V_INT32 i32Ret;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;
    i32Ret = pstAoutdev->track_set_mute(pstAoutdev,pstAuOutHdlInfo->psthTrack,(BOOL)mute);
    if(SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("sme_hihal_aout_set_mute failed 0x%x\n",i32Ret);
    } else
    {
        SME_LOGI_ADPT("sme_hihal_aout_set_mute: %u",mute);
    }

    (void)pstThisLogCtx;
#endif
    return E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
}

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_get_mute(IN void * hdlAOut, guint *mute)
{
#ifdef LINUX_OS
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL ;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev;
    V_INT32 i32Ret;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);
    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;
    i32Ret = pstAoutdev->track_get_mute(pstAoutdev,pstAuOutHdlInfo->psthTrack,(BOOL*)mute);
    if(SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("track_get_mute failed 0x%x\n",i32Ret);
    } else
    {
        SME_LOGI_ADPT("track_get_mute: %u",*mute);
    }

    (void)pstThisLogCtx;
#endif
    return E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
}

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_set_channel_mode(IN void * hdlAOut, A_TRACK_CHANNEL_MODE_E mode)
{
#ifdef LINUX_OS
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL ;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev;
    V_INT32 i32Ret;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);
    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;
    i32Ret = pstAoutdev->track_set_channel_mode(pstAoutdev,pstAuOutHdlInfo->psthTrack,(TRACK_CHANNEL_MODE_E)mode);
    if(SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("track_set_channel_mode failed 0x%x\n",i32Ret);
    } else
    {
        SME_LOGI_ADPT("track_set_channel_mode: %u",mode);
    }

    (void)pstThisLogCtx;
#endif
    return E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
}

static E_SME_AUDIO_PLAYOUT_RET sme_hihal_aout_get_channel_mode(IN void * hdlAOut, A_TRACK_CHANNEL_MODE_E *mode)
{
#ifdef LINUX_OS
    PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL ;
    PST_SME_LOG_CB pstThisLogCtx = NULL;
    AOUT_DEVICE_S *pstAoutdev;
    V_INT32 i32Ret;

    pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
    UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                     E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);
    pstAoutdev = pstAuOutHdlInfo->pstAoutdev;
    i32Ret = pstAoutdev->track_get_channel_mode(pstAoutdev,pstAuOutHdlInfo->psthTrack,(TRACK_CHANNEL_MODE_E*)mode);
    if(SUCCESS != i32Ret)
    {
        SME_LOGE_ADPT("track_get_channel_mode failed 0x%x\n",i32Ret);
    } else
    {
        SME_LOGI_ADPT("track_get_channel_mode: %u",*mode);
    }

    (void)pstThisLogCtx;
#endif
    return E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
}

static E_SME_AUDIO_PLAYOUT_RET
sme_hihal_aout_get_render_delay(IN void * hdlAOut, GstClockTime * delay)
{
  E_SME_AUDIO_PLAYOUT_RET eRet = E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
#ifdef LINUX_OS
#ifdef ENABLE_RENDER_DELAY
  PST_SME_AUDIO_OUT_HDL_INFO pstAuOutHdlInfo = NULL ;
  PST_SME_LOG_CB pstThisLogCtx = NULL;
  AOUT_DEVICE_S *pstAoutdev;
  V_INT32 i32Ret;
  U32 latency;

  pstAuOutHdlInfo = (PST_SME_AUDIO_OUT_HDL_INFO)hdlAOut;
  UTILS_RET_VAL_IF(NULL == pstAuOutHdlInfo,
                   E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);
  pstAoutdev = pstAuOutHdlInfo->pstAoutdev;
  i32Ret = pstAoutdev->track_get_latency(pstAoutdev,pstAuOutHdlInfo->psthTrack,
    &latency);
  if(SUCCESS != i32Ret){
    SME_LOGE_ADPT("track_get_channel_mode failed 0x%x, use default", i32Ret);
    *delay = SME_HIHAL_DEFAULT_RENDER_DELAY;
    eRet = E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG;
  } else {
    SME_LOG_AVSYNC("%s:get_audio_render_delay ,latency:%d ms",
        AVSYNC_BY_AUDIO_TAG, latency);
    *delay = latency * GST_MSECOND;
  }

  (void)pstThisLogCtx;
#else
  *delay = SME_HIHAL_DEFAULT_RENDER_DELAY;
#endif
#else
  *delay = SME_HIHAL_DEFAULT_RENDER_DELAY;
#endif
    return eRet;
}

static gint
gst_smeaudiosink_hihai_get_render_delay(GstSmeAudiosink2 *bsink, GstClockTime *delay)
{
  GstSmeAudioSinkHiHal *hsink = NULL;

  E_SME_AUDIO_PLAYOUT_RET eRet = E_SME_AUDIO_PLAYOUT_RET_SUCCESS;
  g_return_val_if_fail((bsink != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);
  g_return_val_if_fail((delay != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

  *delay = GST_CLOCK_TIME_NONE;

  hsink = GST_SME_AUDIOSINK_HIHAL(bsink);

  g_return_val_if_fail((hsink->aout_handle != NULL), E_SME_AUDIO_PLAYOUT_RET_INVALID_ARG);

  eRet = sme_hihal_aout_get_render_delay(hsink->aout_handle, delay);

  return eRet;
}


//register
gboolean
gst_smeaudiosink_hihal_register (GstPlugin * plugin)
{
    GST_INFO ("registering sme audio hisi hal sink in.");

    if (!gst_element_register (plugin, GST_SMEAUDIOSINK_HIHAL_NAME,
                               GST_RANK_SECONDARY +
                               ((GST_RANK_PRIMARY - GST_RANK_SECONDARY) >> 1),
                               GST_TYPE_SME_AUDIOSINK_HIHAL)) {
        GST_WARNING ("Register of type %s failed", GST_SMEAUDIOSINK_HIHAL_NAME);
        return FALSE;
    }
    GST_INFO ("registering sme audio hisi hal sink out.");

  return TRUE;
}

#ifdef HAVE_CONFIG_H
//#include "config.h"
//for dynamic load smeplugin
#ifdef GST_PLUGIN_BUILD_STATIC
#undef GST_PLUGIN_BUILD_STATIC
#endif
#endif

static gboolean
plugin_init (GstPlugin * plugin)
{
    GST_INFO("register sme audio sink based on hisi hal  begin...");
    if (!gst_smeaudiosink_hihal_register (plugin))
        GST_WARNING("register sme hisi-hal audio sink  fail.");
    GST_WARNING("register sme audio sink based on hisi end");
    return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    sme_gstplugin_asink_hisi_hal,
    "sme plugin asink based on hisi hal driver.",
    plugin_init, VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
