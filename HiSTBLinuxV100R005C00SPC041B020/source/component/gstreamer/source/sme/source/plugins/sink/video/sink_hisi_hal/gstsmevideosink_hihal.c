/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmevideosink_hisi_hal.c
 * @brief   gstvideosink����halʵ��
 * @author  lidanjing(l00346954)
 * @date    2015/12/3
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2015/12/3
 * Author : lidanjing(l00346954)
 * Desc   : Created file
 *
******************************************************************************/
//#ifdef HAVE_CONFIG_H
//#include "config.h"
//    //for dynamic load smeplugin
//#ifdef GST_PLUGIN_BUILD_STATIC
//#undef GST_PLUGIN_BUILD_STATIC
//#endif
//#endif


//lint -e585
//lint -e528
//lint -e717 //do..while
//lint -e838
//lint -e826
//lint -e655 //¼æÈÝÎ»²Ù×÷·û e.g:G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
//lint -e777 //float equal
//lint -e749 //VSINK_PROP_0  VSINK_PROP_LAST VSINK_PROP_BENCHMARK  not use
//lint -e750 //DUMP_DECED_PATH GST_SME_VSINK_RENDER_BUF_CNT_MIN not use
//lint -e716 //while(1)
//lint -e551

#include <stdio.h>
#include "gstinfo.h"
#include "osal_mem.h"
#include "gstsme.h"
#include "sme_log.h"

#include "gstsmevideosink_hihal.h"
#include "gstsmevideosink_hihal_allcator.h"
//#include "sme_vdec_vdisp_pipe.h"

#define DUMP_DECED_PATH "/data/dump/deced_data.dat"

#define GST_SME_VSINK_RENDER_BUF_CNT_MIN 4
#define GST_SME_VSINK_VIRTUAL_SCREEN_WIDTH 1280
#define GST_SME_VISNK_VIRTUAL_SCREEN_HEIGHT 720
//#define GST_SME_BUFFER_CTRL_LATENESS        (80*GST_MSECOND)//ns, 80ms
#define GST_SME_VIDEO_MAX_LATENESS          (10*GST_SECOND)//ns,10000ms
#define GST_SME_VIDEO_LATENESS_NORM         (60)//ns,60ms
#define GST_SME_DROP_INTERLEAVE_L1          (25)
#define GST_SME_DROP_INTERLEAVE_L2          (50)
#define GST_SME_DROP_INTERLEAVE_L3          (75)
#define ENABLE_RENDER_DELAY
#define DEFAULT_FRAME_DURATION              (25)//ms
#define VHAL_RENDER_DELAY_DEFAULT (60*GST_MSECOND) //3*20 ms

#define GST_DEBUG_OBJECT_AVSYNC GST_INFO_OBJECT

#ifndef DEQUEUE_FRAME_ENABLE
#define DEQUEUE_FRAME_ENABLE
#endif

static gpointer DequeueThdFunc (gpointer data);

enum
{
    VSINK_PROP_0,
    VSINK_PROP_DISP_RECT,
    VSINK_PROP_BENCHMARK,
    VSINK_PROP_HIMPI_HDL,
    VSINK_PROP_DISP_SURFACE_ANDROID,
    //begin, biexh
    VSINK_PROP_DISP_MODE,
    VSINK_PROP_DISP_RATIO,
    //end, biexh
    PROP_VSINK_INFO,
    VSINK_PROP_CC_MODE,//channel switch
    VSINK_PROP_PTS_CALIBRATION_HDL,
    VSINK_PROP_FREEZE_MODE,
    VSINK_PROP_SLOW_PRESYNC,
    VSINK_PROP_WAIT_PRESYNC_MAXTIME,
    VSINK_PROP_SLOW_PRESYNC_RATE,
    VSINK_PROP_LAST,
};

/* global List ,put HiHalItem in it for change channels keep last picture */
typedef struct __tagHiHalItem
{
  ST_SME_DISP_RECT st_vdisp_rect; //��Ƶ��ʾ��Χ
  ST_SME_HAL_HI_VDISP_PIPE* hdl_sme_wr_hihal;//hihal wr hdl
}HiHalItem;

static GList * gHiHalList = NULL;
GList * gToBeFree = NULL;
static E_SME_PLAYER_SWITCH_CHANNEL_MODE gLastSwitchMode = E_SME_PLAYER_SWITCH_CHANNEL_MODE_BLACK;

static void
gst_smevideosink_hihal_class_init (GstSmeVideoSinkHiHalClass * klass);

static void
gst_smevideosink_hihal_init (GstSmeVideoSinkHiHal * vsink);

static void
gst_smevideosink_hihal_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);

static void
gst_smevideosink_hihal_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static void
gst_smevideosink_hihal_finalize(GObject * object);

static gboolean
gst_smevideosink_hihal_send_event(GstElement * element, GstEvent * event);

static GstStateChangeReturn
gst_smevideosink_hihal_change_state(GstElement * element, GstStateChange transition);

static GstFlowReturn
gst_smevideosink_hihal_render(GstBaseSink * bsink, GstBuffer * buffer);

static gboolean
gst_smevideosink_hihal_event(GstBaseSink * bsink, GstEvent * event);

static gboolean
gst_smevideosink_hihal_query(GstBaseSink * bsink, GstQuery * query);

//pad
static GstCaps *
gst_smevideosink_hihal_get_caps(GstBaseSink * sink, GstCaps * filter);

static gboolean
gst_smevideosink_hihal_set_caps(GstBaseSink * sink, GstCaps * caps);

static GstCaps *
gst_smevideosink_hihal_fixate(GstBaseSink * bsink, GstCaps * caps);

static GstFlowReturn
gst_smevideosink_hihal_wait_event (GstBaseSink * bsink, GstEvent * event);

static void
gst_smevideosink_hihal_get_times (GstBaseSink * basesink, GstBuffer * buffer,
    GstClockTime * start, GstClockTime * end);
static void
gst_smevideosink_hihal_cal_disprect(SME_HAL_VDISP_PIPE_HDL hdlPipe, E_SME_PLAYER_DISPLAY_MODE e_disp_mode, const ST_OUT_RECT* p_win_rect,
    const ST_DISP_RATIO* p_disp_radio, ST_OUT_RECT* p_disp_win);
static guint
gst_smevideosink_hihal_update_display_mode(GstSmeVideoSinkHiHal * vsink);

static GstClockTime
gst_smevideosink_hihal_get_render_delay(GstBaseSink *basesink);

static void
gst_smevideosink_hihal_calibrate_pts (GstBaseSink * bsink, GstBuffer * buffer,
  gboolean *bcalibrated);

//#define GST_SMEDEMUX_PARAMS_QDATA g_quark_from_static_string("smedemux-params")

#define GST_SMEVIDEOSINK_HIHAL_NAME "smevideosink_hihal"

GST_DEBUG_CATEGORY_STATIC (gst_smevideosink_hihal_debug);

//lint -e652
#ifdef GST_CAT_DEFAULT
#undef GST_CAT_DEFAULT
#endif
#define GST_CAT_DEFAULT gst_smevideosink_hihal_debug
//lint +e652


#define _do_init \
    GST_DEBUG_CATEGORY_INIT (gst_smevideosink_hihal_debug, GST_SMEVIDEOSINK_HIHAL_NAME, \
    0, "smevideosink hihal element");


//lint -e19
G_DEFINE_TYPE_WITH_CODE (GstSmeVideoSinkHiHal, gst_smevideosink_hihal, GST_TYPE_BASE_SINK, _do_init);
//lint +e19



#define parent_class gst_smevideosink_hihal_parent_class
#define GST_SMEVIDEOSINK_HIHAL_DEFAUT_SYNC      TRUE
#define GST_SMEVIDEOSINK_HIHAL_RENDER_LATENCY   ((GstClockTime)0)

static void
gst_smevideosink_hihal_class_init (GstSmeVideoSinkHiHalClass * klass)
{
    GObjectClass *          gobject_class = NULL;
    GstElementClass *       gstelement_class = NULL;
    GstBaseSinkClass*       gstbasesink_class = NULL;
    GstCaps*                sinkcaps = NULL;
    GstPadTemplate*         sinktempl = NULL;

    gobject_class       = G_OBJECT_CLASS(klass);
    gstelement_class    = GST_ELEMENT_CLASS(klass);
    gstbasesink_class   = GST_BASE_SINK_CLASS(klass);

    //klass->parent_class = g_type_class_peek_parent (klass);

    //gobject method
    gobject_class->set_property     = gst_smevideosink_hihal_set_property;
    gobject_class->get_property     = gst_smevideosink_hihal_get_property;
    gobject_class->finalize         = GST_DEBUG_FUNCPTR (gst_smevideosink_hihal_finalize);


    gst_element_class_set_static_metadata (gstelement_class,
        "Sme Video Sink HiHal",
        "Sink/Video",
        "sme player video render component",
        "gstsmevideosinkhihal element");

    sinkcaps = gst_caps_from_string ("video/x-raw,width = " GST_VIDEO_SIZE_RANGE ", "
        "height = " GST_VIDEO_SIZE_RANGE ", " "framerate = " GST_VIDEO_FPS_RANGE);
    if (!sinkcaps) {
        sinkcaps = gst_caps_from_string ("unknown/unknown");
        GST_WARNING("unkown video sink caps!");
    }else{
        gchar*caps_tostring = gst_caps_to_string(sinkcaps);
        GST_INFO("video sink caps=%s", caps_tostring);
        g_free(caps_tostring);
    }

    sinktempl = gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS, sinkcaps);
    gst_element_class_add_pad_template (gstelement_class, sinktempl);

    //element method
    gstelement_class->change_state      = gst_smevideosink_hihal_change_state;
    gstelement_class->send_event        = gst_smevideosink_hihal_send_event;

    //basesink method
    gstbasesink_class->get_caps         = GST_DEBUG_FUNCPTR (gst_smevideosink_hihal_get_caps);
    gstbasesink_class->set_caps         = GST_DEBUG_FUNCPTR (gst_smevideosink_hihal_set_caps);
    gstbasesink_class->event            = GST_DEBUG_FUNCPTR (gst_smevideosink_hihal_event);
    gstbasesink_class->render           = GST_DEBUG_FUNCPTR (gst_smevideosink_hihal_render);
    gstbasesink_class->query            = GST_DEBUG_FUNCPTR (gst_smevideosink_hihal_query);
    gstbasesink_class->get_times        = GST_DEBUG_FUNCPTR (gst_smevideosink_hihal_get_times);
    gstbasesink_class->fixate           = GST_DEBUG_FUNCPTR (gst_smevideosink_hihal_fixate);
    gstbasesink_class->wait_event       = GST_DEBUG_FUNCPTR (gst_smevideosink_hihal_wait_event);
    gstbasesink_class->calibrate_pts    = GST_DEBUG_FUNCPTR (gst_smevideosink_hihal_calibrate_pts);

    g_object_class_install_property (gobject_class, VSINK_PROP_DISP_RECT,
        g_param_spec_ulong (GST_SME_PROP_KEY_VDISP_RECT, "video disp rect",
            "video display disp rect instance ",
            0,
            (gulong)(-1),
            0,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));


    g_object_class_install_property (gobject_class, VSINK_PROP_HIMPI_HDL,
        g_param_spec_pointer(GST_SME_PROP_KEY_HIVPIPE_HDL, "himpi vpipe hdl",
            "himpi vpipe hdl",
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property(gobject_class, VSINK_PROP_DISP_SURFACE_ANDROID,
            g_param_spec_ulong (GST_SME_PROP_KEY_VDISP_SF, "vsink surface instance",
            "vsink video display surface instance ", 0, (gulong)(-1), 0, G_PARAM_READWRITE
            | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, VSINK_PROP_DISP_MODE,
            g_param_spec_uint(GST_SME_PROP_KEY_DISP_MODE, "video display mode",
              "video display mode", 0, 4, 4, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, VSINK_PROP_DISP_RATIO,
            g_param_spec_pointer(GST_SME_PROP_KEY_DISP_RADIO, "video display ratio",
              "video display radio", G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, PROP_VSINK_INFO,
        g_param_spec_pointer(GST_SME_BENCH_MARK_VSINK_INFO, "Vsink-Proc-Info",
            "Poniter of Video Sink",
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, VSINK_PROP_CC_MODE,
            g_param_spec_uint(GST_SME_PROP_KEY_SWITCH_CHANNEL_MODE, "video CC mode",
              "video channel change mode", 0, (guint)-1, 0,
              G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    g_object_class_install_property (gobject_class, VSINK_PROP_PTS_CALIBRATION_HDL,
        g_param_spec_pointer (GST_SME_PTS_CALIBRATION_HDL, "pts calibration hdl",
          "pts calibration hdl",
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, VSINK_PROP_FREEZE_MODE,
        g_param_spec_uint(GST_SME_PROP_KEY_VIDEO_FREEZE_MODE, "video freeze mode",
          "video freeze mode", 0, 1, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, VSINK_PROP_SLOW_PRESYNC,
        g_param_spec_uint(GST_SME_PROP_KEY_SLOW_PRESYNC,
          "vsink slow presync mode",
          "vsink slow presync mode",
          0, (guint)-1, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, VSINK_PROP_WAIT_PRESYNC_MAXTIME,
        g_param_spec_uint64(GST_SME_PROP_KEY_WAIT_PRESYNC_MAXTIME,
          "vsink wait-presync maxtime",
          "vsink wait-presync maxtime",
          0ULL, (guint64)-1, (guint64)-1, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (gobject_class, VSINK_PROP_SLOW_PRESYNC_RATE,
        g_param_spec_double(GST_SME_PROP_KEY_SLOW_PRESYNC_RATE,
          "vsink slow-presync-rate",
          "vsink slow-presync-rate",
          0.0, 1.0, 0.333333, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    return;
}

static void
gst_smevideosink_hihal_init (GstSmeVideoSinkHiHal * vsink)
{
    GstBaseSink* gstbase_sink = GST_BASE_SINK(vsink);
    E_SME_MEM_SECURE_RET eMemSecRet;

    GST_INFO_OBJECT(vsink, "gst_smevideosink_hihal_init in");
    eMemSecRet = VOS_Memset_S((void*)(&vsink->st_vdisp_rect), sizeof(ST_SME_DISP_RECT), 0, sizeof(ST_SME_DISP_RECT));
    if(eMemSecRet != E_SME_MEM_OK){
        GST_ERROR("init vsink->st_vdisp_rect failed");
        return;
    }

    eMemSecRet = VOS_Memset_S((void*)(&vsink->stVideoInfo), sizeof(ST_SME_VIDEOINFO), 0, sizeof(ST_SME_VIDEOINFO));
    if(eMemSecRet != E_SME_MEM_OK){
        GST_ERROR("init vsink->stVideoInfo failed");
        return;
    }

    gstbase_sink->can_activate_pull = TRUE;
    vsink->recieve_eos              = FALSE;
    vsink->hdl_sme_pipe_hihal       = NULL;
    vsink->android_surface          = NULL;
    vsink->dequeue_thd              = NULL;
    vsink->is_dequeue_thd_exit      = FALSE;

    //Begin, biexh
    vsink->switch_channel_mode       = gLastSwitchMode;
    eMemSecRet = VOS_Memset_S((&vsink->vsink_proc_info), sizeof(ST_VSINK_SHOW_INFO), 0, sizeof(ST_VSINK_SHOW_INFO));
    if(eMemSecRet != E_SME_MEM_OK){
        GST_ERROR("init vsink->vsink_proc_info failed");
        return;
    }
    //End, biexh

    g_queue_init(&vsink->q_rendered_buf);
    g_mutex_init(&vsink->rendered_buf_muxtex);
    g_queue_init(&vsink->q_mmz_is_vo);
    g_mutex_init(&vsink->mmz_is_vo_muxtex);
    g_queue_init(&vsink->q_to_be_Free);

    gst_base_sink_set_sync (GST_BASE_SINK (vsink), (gboolean)GST_SMEVIDEOSINK_HIHAL_DEFAUT_SYNC);
    gst_base_sink_set_render_delay (GST_BASE_SINK (vsink), GST_SMEVIDEOSINK_HIHAL_RENDER_LATENCY);
    //gst_base_sink_set_sync (GST_BASE_SINK (vsink), FALSE);
    gst_base_sink_set_max_lateness (gstbase_sink, GST_SME_VIDEO_MAX_LATENESS);
    gettimeofday(&vsink->vsink_proc_info.VideoSinkCreatedTimeMs, NULL);

    /* init display setting param */
    vsink->e_disp_mode = E_SME_PLAYER_DIS_MODE_DEFAULT;
    vsink->st_disp_ratio.u32DispRatioH = 3;
    vsink->st_disp_ratio.u32DispRatioW = 4;
    vsink->st_win_rect.u32OutHeight = GST_SME_VISNK_VIRTUAL_SCREEN_HEIGHT;
    vsink->st_win_rect.u32OutWidth = GST_SME_VSINK_VIRTUAL_SCREEN_WIDTH;
    vsink->st_win_rect.u32OutX = 0;
    vsink->st_win_rect.u32OutY = 0;

    vsink->e_old_disp_mode = E_SME_PLAYER_DIS_MODE_NONE;
    vsink->st_old_disp_ratio.u32DispRatioH = 3;
    vsink->st_old_disp_ratio.u32DispRatioW = 4;
    vsink->st_old_win_rect.u32OutHeight = GST_SME_VISNK_VIRTUAL_SCREEN_HEIGHT;
    vsink->st_old_win_rect.u32OutWidth = GST_SME_VSINK_VIRTUAL_SCREEN_WIDTH;
    vsink->st_old_win_rect.u32OutX = 0;
    vsink->st_old_win_rect.u32OutY = 0;

    vsink->first_frame_flag = TRUE;

    vsink->freeze_mode = E_SME_PLAYER_SWITCH_CHANNEL_MODE_BUTT;
    GST_INFO_OBJECT(vsink, "gst_smevideosink_himpi_init out");

    return;
}

static void
gst_smevideosink_hihal_finalize (GObject * object)
{
    GST_INFO_OBJECT(object, "gst_smevideosink_hihal_finalize in");

    GstSmeVideoSinkHiHal* vsink = GST_SME_VIDEOSINK_HIHAL(object);

    while(!g_queue_is_empty(&vsink->q_rendered_buf)){
        GstVideoFrameMap* item = g_queue_pop_head(&vsink->q_rendered_buf);
        if(item){
            gst_buffer_unref(item->gstvbuf);
            g_free(item);
        }
    }
    vsink->vsink_proc_info.VideoFinalPlayTimeMs = vsink->current_play_timeUs;
    g_queue_clear(&vsink->q_rendered_buf);
    g_mutex_clear(&vsink->rendered_buf_muxtex);
    g_mutex_clear(&vsink->mmz_is_vo_muxtex);

    if(vsink->pts_calibration_hdl){
      gst_sme_pts_calibration_entry_deinit(vsink->pts_calibration_hdl,
        E_ENTRY_TYPE_VIDEO);
    }

    G_OBJECT_CLASS (parent_class)->finalize (object);

    //todo: in change state and here should free left item of vsink->q_to_be_Free
    g_queue_clear(&vsink->q_to_be_Free);
    g_queue_clear(&vsink->q_mmz_is_vo);

    GST_INFO_OBJECT(object, "gst_smevideosink_hihal_finalize out");

    return;
}

static GstCaps *
gst_smevideosink_hihal_get_caps (GstBaseSink * sink, GstCaps * filter)
{
    (void)sink;
    (void)filter;
    return NULL;
}

//lint -e818
static gboolean
gst_smevideosink_hihal_set_caps (GstBaseSink * sink, GstCaps * caps)
{
    GstSmeVideoSinkHiHal*   vsink       = GST_SME_VIDEOSINK_HIHAL(sink);
    gboolean                bRet        = FALSE;
    gboolean                bEs         = FALSE;
    E_SME_MEM_SECURE_RET    eMemSecRet;

    if((NULL == sink) || (NULL == caps)){
        GST_ERROR("gst_smevideosink_hihal_set_caps invalid args!");
        return FALSE;
    }

    eMemSecRet = VOS_Memset_S((void*)(&vsink->stVideoInfo), sizeof(ST_SME_VIDEOINFO), 0, sizeof(ST_SME_VIDEOINFO));
    if(eMemSecRet != E_SME_MEM_OK){
        GST_ERROR("init vsink->stVideoInfo failed");
        return bRet;
    }

    bRet = gst_sme_caps_to_videoinfo ((const GstCaps*)caps,
                                    &vsink->stVideoInfo, &bEs);
    gchar* caps_tostring = gst_caps_to_string((const GstCaps*)caps);
    GST_INFO_OBJECT(vsink, "gst_sme_caps_to_mediainfo ,caps=%s,w=%d,h=%d,bEs=%d",
        caps_tostring, vsink->stVideoInfo.stVideoRes.i32Width, vsink->stVideoInfo.stVideoRes.i32Height,
        bEs);
    g_free(caps_tostring);

    return bRet;
}
//lint +e818

static void
gst_smevideosink_hihal_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
    GstSmeVideoSinkHiHal *   vsink = GST_SME_VIDEOSINK_HIHAL(object);

    GST_INFO_OBJECT(object, "gst_smevideosink_hihal set property:%d", prop_id);

    switch (prop_id) {
        case VSINK_PROP_DISP_RECT:{
            gulong ul_rect = g_value_get_ulong (value);
            if (ul_rect > 0) {
              g_mutex_lock(&vsink->rendered_buf_muxtex);
              vsink->st_vdisp_rect = *((ST_SME_DISP_RECT*)ul_rect);

              if(vsink->st_vdisp_rect.i32Height == 0 &&  vsink->st_vdisp_rect.i32Width == 0
              && vsink->st_vdisp_rect.i32StartX == 0 && vsink->st_vdisp_rect.i32StartY == 0)
              {
                  vsink->st_win_rect.u32OutHeight = GST_SME_VISNK_VIRTUAL_SCREEN_HEIGHT;
                  vsink->st_win_rect.u32OutWidth = GST_SME_VSINK_VIRTUAL_SCREEN_WIDTH;
                  vsink->st_win_rect.u32OutX = 0;
                  vsink->st_win_rect.u32OutY = 0;
              }
              else
              {
                  vsink->st_win_rect.u32OutHeight = (V_UINT32)vsink->st_vdisp_rect.i32Height;
                  vsink->st_win_rect.u32OutWidth = (V_UINT32)vsink->st_vdisp_rect.i32Width;
                  vsink->st_win_rect.u32OutX = (V_UINT32)vsink->st_vdisp_rect.i32StartX;
                  vsink->st_win_rect.u32OutY = (V_UINT32)vsink->st_vdisp_rect.i32StartY;
              }
              g_mutex_unlock(&vsink->rendered_buf_muxtex);

              GST_INFO("my rect:x=%d,y=%d,w=%d,h=%d",
                vsink->st_win_rect.u32OutX,
                vsink->st_win_rect.u32OutY,
                vsink->st_win_rect.u32OutWidth,
                vsink->st_win_rect.u32OutHeight);

            }
            break;
        }
        case VSINK_PROP_CC_MODE:{
            GST_INFO("set CC mode to videosink hihal : %d", g_value_get_uint(value));
            vsink->switch_channel_mode = (E_SME_PLAYER_SWITCH_CHANNEL_MODE)g_value_get_uint(value);
            gLastSwitchMode = vsink->switch_channel_mode;
            GST_INFO("begin set SwitchChannelMode to videosink hihal : %d",
              vsink->switch_channel_mode);
            if( vsink->switch_channel_mode == E_SME_PLAYER_SWITCH_CHANNEL_MODE_LAST){
              guint uHiHalListLen = g_list_length(gHiHalList);
              guint idx = 0;
              gboolean find_hivo = FALSE;
              GST_INFO("set_pro: the old gHiHalList len is %d", uHiHalListLen);
              for(idx = 0; idx < uHiHalListLen; idx++){
                HiHalItem* pHiHalItem = g_list_nth_data(gHiHalList, idx);
                if(pHiHalItem){
                  if(pHiHalItem->st_vdisp_rect.i32StartX == vsink->st_vdisp_rect.i32StartX &&
                      pHiHalItem->st_vdisp_rect.i32StartY == vsink->st_vdisp_rect.i32StartY &&
                      pHiHalItem->st_vdisp_rect.i32Width == vsink->st_vdisp_rect.i32Width &&
                      pHiHalItem->st_vdisp_rect.i32Height == vsink->st_vdisp_rect.i32Height &&
                      pHiHalItem->hdl_sme_wr_hihal == vsink->hdl_sme_pipe_hihal){
                      find_hivo = TRUE;
                      GST_INFO("SET_PRO: find_hivo is true");
                      break;
                  }
                }else{
                  GST_WARNING("pHiHalItem is NULL");
                }
              }

              if(!find_hivo){
                  //if not find,clear gHiHalList
                  uHiHalListLen = g_list_length(gHiHalList);
                  for(idx = 0; idx < uHiHalListLen; idx++){
                      HiHalItem* pHiHalItem = g_list_nth_data(gHiHalList, idx);
                      if(pHiHalItem){
                          g_free(pHiHalItem);
                      }
                  }
                  g_list_free(gHiHalList);
                  gHiHalList = NULL;
                  //put vsink->hdl_sme_pipe_hihal into gHiHalList
                  HiHalItem* pHiHalItemTmp = g_malloc(sizeof(HiHalItem));
                  if(!pHiHalItemTmp){
                      GST_ERROR("malloc HiHalItem failed");
                  }else{
                      pHiHalItemTmp->hdl_sme_wr_hihal = vsink->hdl_sme_pipe_hihal;
                      pHiHalItemTmp->st_vdisp_rect.i32StartX = vsink->st_vdisp_rect.i32StartX;
                      pHiHalItemTmp->st_vdisp_rect.i32StartY = vsink->st_vdisp_rect.i32StartY;
                      pHiHalItemTmp->st_vdisp_rect.i32Width = vsink->st_vdisp_rect.i32Width;
                      pHiHalItemTmp->st_vdisp_rect.i32Height = vsink->st_vdisp_rect.i32Height;
                      gHiHalList = g_list_append(gHiHalList, pHiHalItemTmp);
                      uHiHalListLen = g_list_length(gHiHalList);
                      GST_DEBUG("set_pro,Add new HiHalItem,the new gHiHalList len is %d", uHiHalListLen);
                  }
              }
            }
            GST_INFO("end set SwitchChannelMode to videosink hihal : %d",
              vsink->switch_channel_mode);
            break;
        }
        //begin, biexh
        case VSINK_PROP_DISP_MODE:{
            g_mutex_lock(&vsink->rendered_buf_muxtex);
            vsink->e_disp_mode = (E_SME_PLAYER_DISPLAY_MODE)g_value_get_uint(value);
            g_mutex_unlock(&vsink->rendered_buf_muxtex);
            GST_INFO("set DispMode to videosink hivo : %d", (guint)vsink->e_disp_mode);
            break;
        }
        case VSINK_PROP_DISP_RATIO:{
            ST_DISP_RATIO * p_disp_ratio= (ST_DISP_RATIO*)g_value_get_pointer(value);
            if(p_disp_ratio && p_disp_ratio->u32DispRatioW != 0 && p_disp_ratio->u32DispRatioH != 0)
            {
                g_mutex_lock(&vsink->rendered_buf_muxtex);
                vsink->st_disp_ratio.u32DispRatioW = p_disp_ratio->u32DispRatioW;
                vsink->st_disp_ratio.u32DispRatioH = p_disp_ratio->u32DispRatioH;
                GST_INFO("set DispMode, DispRatio to videosink hivo : %u %u\n", vsink->st_disp_ratio.u32DispRatioW,
                    vsink->st_disp_ratio.u32DispRatioH);
                g_mutex_unlock(&vsink->rendered_buf_muxtex);
            }
            break;
        }
        //end, biexh
        case VSINK_PROP_DISP_SURFACE_ANDROID:{
            vsink->android_surface = (void*)g_value_get_ulong (value);
            GST_INFO("set surface:%p", vsink->android_surface);
            break;
        }
        case PROP_VSINK_INFO:{
            ST_VIDEOSINK_PROC_INFO * pstVsinkProcInfo;
            ST_VSINK_CMD_INFO *pstVsinkCmdInfo;
            E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;

            pstVsinkProcInfo = g_value_get_pointer(value);
            //cmd type is show info
            if (pstVsinkProcInfo && pstVsinkProcInfo->eProcCmdType == E_SME_PROC_CMD_SHOW_INFO)
            {
                ST_VSINK_SHOW_INFO *pstVsinkShowInfo;
                pstVsinkShowInfo = &(pstVsinkProcInfo->stShowInfo);

                eMemSecRet = VOS_Memcpy_S(pstVsinkShowInfo,
                                          sizeof(ST_VSINK_SHOW_INFO),
                                          &vsink->vsink_proc_info,
                                          sizeof(vsink->vsink_proc_info));
                if (eMemSecRet != E_SME_MEM_OK)
                    GST_WARNING("VOS_Memset_S collect video sink info failed");

                GST_INFO("Collect video sink info for proc");
            }
            else if (pstVsinkProcInfo && pstVsinkProcInfo->eProcCmdType == E_SME_PROC_CMD_SET_CMD)
            {
                pstVsinkCmdInfo = &(pstVsinkProcInfo->stCmdInfo);
                GST_INFO("Video Sink bDumpSinkStream=%d", pstVsinkCmdInfo->bDumpSinkStream);
                GST_INFO("Video Sink bDumpSrcStream=%d", pstVsinkCmdInfo->bDumpSrcStream);
                GST_INFO("Video Sink bEnableSync=%#x", pstVsinkCmdInfo->i8EnableAVSync);
                GST_INFO("Video Sink cDumpFileStorePath=%s", pstVsinkCmdInfo->cDumpFileStorePath);
                if ((V_UINT8)pstVsinkCmdInfo->bDumpSinkStream != 0XFF)
                {
                    if (pstVsinkCmdInfo->bDumpSinkStream == 1)
                    {
                       //begin to start dump sink data;
                        struct timeval cur_time;
                        char dump_es_name[512];
                        gint ret;

                        gettimeofday(&cur_time, NULL);
                        ret = g_snprintf(dump_es_name,512, "%s/vsink_in_%ld.es",pstVsinkCmdInfo->cDumpFileStorePath,cur_time.tv_sec);
                        if(ret <= 0)
                        {
                            GST_WARNING("write name failed, we use default path");
                            eMemSecRet = VOS_Memcpy_S(dump_es_name, 512, "/data/vsink_in.raw",
                                sizeof("/data/vsink_in.raw"));
                            if (eMemSecRet != E_SME_MEM_OK)
                            {
                                GST_WARNING("VOS_Memset_S collect video sink info failed");
                                break;
                            }
                        }
                        vsink->dump_raw_loca = fopen(dump_es_name, "ab+");
                        if(vsink->dump_raw_loca == NULL)
                        {
                            GST_WARNING("dump es failed,open dump_es_loca failed");
                        }
                        else
                        {
                            vsink->dump_raw_switch = TRUE;
                        }


                    }
                    else if(pstVsinkCmdInfo->bDumpSinkStream == 0)
                    {
                      //stop dump sink data
                        vsink->dump_raw_switch = FALSE;
                        if(vsink->dump_raw_loca) {
                            fclose(vsink->dump_raw_loca);
                            vsink->dump_raw_loca = NULL;
                        }

                    }
                }

                if ((V_UINT8)pstVsinkCmdInfo->bDumpSrcStream != 0xFF) {
                    if (pstVsinkCmdInfo->bDumpSrcStream == 1)
                    {
                      //begin to start dump src data;
                    }
                    else if(pstVsinkCmdInfo->bDumpSrcStream == 0)
                    {
                      //stop dump src data
                    }
                }
                if((V_UINT8)pstVsinkCmdInfo->i8EnableAVSync != 0xFF)
                {
                    gboolean enable_sync = (0 == pstVsinkCmdInfo->i8EnableAVSync) ? FALSE : TRUE;
                    gst_base_sink_set_sync(GST_BASE_SINK(vsink), (gboolean)enable_sync);
                }
            }
            break;
        }
        case VSINK_PROP_PTS_CALIBRATION_HDL:{
            vsink->pts_calibration_hdl = (GstSmePTSCalibrateHDL)g_value_get_pointer(value);
            GST_INFO_OBJECT(vsink, "vsink VSINK_PROP_PTS_CALIBRATION_HDL:%p",
              vsink->pts_calibration_hdl);
            break;
        }
        case VSINK_PROP_FREEZE_MODE:{
            guint32 ret = ICS_FAIL;
            vsink->freeze_mode = (E_SME_PLAYER_SWITCH_CHANNEL_MODE)g_value_get_uint(value);
            E_SME_HIVO_SWITCH_TYPE reset_mode = (E_SME_HIVO_SWITCH_TYPE)vsink->freeze_mode;
            ret = SME_Hal_VDisp_Reset(vsink->hdl_sme_pipe_hihal, reset_mode);
            if(ICS_SUCCESS == ret){
                GST_INFO("set stop streamer mode :%u succeed\n", vsink->freeze_mode);
            }else{
                GST_WARNING("set stop streamer mode :%u failed\n", vsink->freeze_mode);
            }
            break;
        }
        case VSINK_PROP_SLOW_PRESYNC:{
            GstBaseSink* bsink = GST_BASE_SINK_CAST(vsink);
            bsink->slow_presync_info.slow_presync_state = g_value_get_uint(value);
            GST_WARNING_OBJECT(vsink, "pre-sync:vsink prop-slow-presync-mode:%d",
              bsink->slow_presync_info.slow_presync_state);

            if(vsink->pts_calibration_hdl){
              gst_sme_pts_calibration_entry_update_presync_info(vsink->pts_calibration_hdl,
                E_ENTRY_TYPE_AUDIO, bsink->slow_presync_info);
            }

            if(1 == bsink->slow_presync_info.slow_presync_state
              || 0 == bsink->slow_presync_info.slow_presync_state){
              bsink->slow_presync_info.last_presync_pts = GST_CLOCK_TIME_NONE;
              bsink->slow_presync_info.start_wait_presync_systime = GST_CLOCK_TIME_NONE;
              bsink->slow_presync_info.slow_presync_stime = GST_CLOCK_TIME_NONE;
            }

            break;
        }
        case VSINK_PROP_WAIT_PRESYNC_MAXTIME:{
            GstBaseSink* bsink = GST_BASE_SINK_CAST(vsink);
            bsink->slow_presync_info.wait_presync_max_time = g_value_get_uint64(value);
            GST_WARNING_OBJECT(vsink, "pre-sync:vsink prop-wait-presync-maxtime:%lld",
              bsink->slow_presync_info.wait_presync_max_time);
            break;
        }
        case VSINK_PROP_SLOW_PRESYNC_RATE:{
            GstBaseSink* bsink = GST_BASE_SINK_CAST(vsink);
            bsink->slow_presync_info.gdrate = g_value_get_double(value);
            GST_WARNING_OBJECT(vsink, "pre-sync:vsink prop-slow-presync-rate:%f",
              bsink->slow_presync_info.gdrate);
            break;
        }
        default:
            G_OBJECT_CLASS(parent_class)->set_property(object, prop_id, value, pspec);
            //G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }

    return;
}

static void
gst_smevideosink_hihal_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
    GstSmeVideoSinkHiHal *   vsink = GST_SME_VIDEOSINK_HIHAL(object);

    GST_INFO_OBJECT(object, "gst_smevideosink_hihal get property:%d", prop_id);

    switch(prop_id){
        case VSINK_PROP_DISP_RECT:{
            g_value_set_ulong (value, (gulong)(&vsink->st_old_win_rect));
            GST_INFO("VSINK_PROP_DISP_RECT get val = %lu", (gulong)(&vsink->st_old_win_rect));
            break;
        }
        case VSINK_PROP_DISP_MODE:{
            g_value_set_uint (value, (guint)vsink->e_old_disp_mode);
            GST_INFO("VSINK_PROP_DISP_MODE get val = %u", (guint)vsink->e_old_disp_mode);
            break;
        }
        case VSINK_PROP_DISP_RATIO:{
            g_value_set_pointer(value, (void *)(&vsink->st_old_disp_ratio));
            GST_INFO("VSINK_PROP_DISP_RECT get val = %p", (void *)(&vsink->st_old_disp_ratio));
            break;
        }
        default:
             G_OBJECT_CLASS(parent_class)->get_property(object, prop_id, value, pspec);
             break;
    }

    return;
}

static GstCaps *
gst_smevideosink_hihal_fixate (GstBaseSink * bsink, GstCaps * caps)
{
    GstCaps* pcaps = NULL;
    GST_INFO_OBJECT (bsink, "using sme video sink hihal caps fixate function in ");

    //ÉèÖÃpadÎª·Çflush
    GST_PAD_UNSET_FLUSHING(GST_BASE_SINK_PAD(bsink));
    GST_INFO_OBJECT (bsink, "using sme video sink hihal caps ,isflush=%d",
        (gboolean)GST_PAD_IS_FLUSHING(GST_BASE_SINK_PAD(bsink)));


    pcaps = gst_caps_fixate (caps);

    GST_INFO_OBJECT (bsink, "using sme video sink hihal caps ,isflush=%d out",
        (gboolean)GST_PAD_IS_FLUSHING(GST_BASE_SINK_PAD(bsink)));

    return pcaps;
}

/*
static  void* gst_smevideosink_hihal_get_phyaddr(GstBaseSink * bsink, void * vir_addr)
{
    GstSmeVideoSinkHiHal *  vsink       = GST_SME_VIDEOSINK_HIHAL(bsink);
    guint                   ilen        = 0;
    guint                   idx         = 0;

    g_mutex_lock(&vsink->mmz_is_vo_muxtex);

    ilen = g_queue_get_length(&vsink->q_mmz_is_vo);
    for(idx = 0; idx < ilen; idx++){
        GstMmzInfo* item = g_queue_peek_nth(&vsink->q_mmz_is_vo, idx);
        if(!item){
            GST_ERROR("peek GstVideoMmzVoFlag %d:is null, why?", idx);
            continue;
        }
        if(item->pVirAddr == vir_addr){
            g_mutex_unlock(&vsink->mmz_is_vo_muxtex);
            return item->pPhyAddr;
        }
    }
    g_mutex_unlock(&vsink->mmz_is_vo_muxtex);
    return NULL;

}
*/


/* default implementation to calculate the start and end
 * timestamps on a buffer, subclasses can override
 */
//lint -e715 //basesink not use
//lint -e818
static void
gst_smevideosink_hihal_get_times (GstBaseSink * basesink, GstBuffer * buffer,
    GstClockTime * start, GstClockTime * end)
{
    GstClockTime timestamp, duration;
    GstClockTime render_delay;

    /* first sync on PTS, else use DTS */
    timestamp = GST_BUFFER_PTS (buffer);
    if (!GST_CLOCK_TIME_IS_VALID (timestamp))
        timestamp = GST_BUFFER_DTS (buffer);

    if (GST_CLOCK_TIME_IS_VALID (timestamp)) {
        /* get duration to calculate end time */
        duration = GST_BUFFER_DURATION (buffer);
        if (GST_CLOCK_TIME_IS_VALID (duration)) {
            *end = timestamp + duration;
        }
        *start = timestamp;
    }

    // set render delay
    render_delay = gst_smevideosink_hihal_get_render_delay(basesink);
    if (GST_CLOCK_TIME_IS_VALID(render_delay))
    {
        gst_base_sink_set_render_delay(basesink, render_delay);
    }

    return;
}
//lint +e715
//lint +e818

#ifdef ENABLE_RENDER_DELAY
static GstClockTime
gst_smevideosink_hihal_get_render_delay(GstBaseSink *basesink)
{
  GstClockTime render_delay = GST_CLOCK_TIME_NONE;
  g_return_val_if_fail((basesink != NULL), GST_CLOCK_TIME_NONE);

  GstSmeVideoSinkHiHal *vsink = GST_SME_VIDEOSINK_HIHAL(basesink);

  g_return_val_if_fail((vsink->hdl_sme_pipe_hihal != NULL), GST_CLOCK_TIME_NONE);

  E_SME_HIVO_ERR eRet = SME_Hal_VDisp_GetRenderDelay(vsink->hdl_sme_pipe_hihal, &render_delay);
  if ( E_SME_HIVO_ERR_NONE != eRet ){
    //todo : if failed, return GST_CLOCK_TIME_NONE or 0?
    render_delay = GST_CLOCK_TIME_NONE;
  }

  if(DEFAULT_FRAME_DURATION*GST_MSECOND <= render_delay){
    render_delay -= (DEFAULT_FRAME_DURATION*GST_MSECOND);
  }

  GST_DEBUG_OBJECT_AVSYNC(basesink, "%s:+-get_video_render_delay total:%lld ms",
      AVSYNC_BY_AUDIO_TAG, GST_VALID_TIME_AS_MSECONDS(render_delay));
  //render_delay = GST_CLOCK_TIME_NONE;

  return render_delay;
}
#else
static GstClockTime
gst_smevideosink_hihal_get_render_delay(GstBaseSink *basesink)
{
  GstClockTime render_delay = VHAL_RENDER_DELAY_DEFAULT;
  g_return_val_if_fail((basesink != NULL), GST_CLOCK_TIME_NONE);

  GstSmeVideoSinkHiHal *vsink = GST_SME_VIDEOSINK_HIHAL(basesink);

  g_return_val_if_fail((vsink->hdl_sme_pipe_hihal != NULL), GST_CLOCK_TIME_NONE);

  GST_DEBUG_OBJECT_AVSYNC(basesink, "%s:get_video_render_delay total:%lld ns",
      AVSYNC_BY_AUDIO_TAG, GST_VALID_TIME_AS_MSECONDS(render_delay));

  return render_delay;
}
#endif

//lint -e529 //frame_len use in USE_DUMP_DECED_DATA
//lint -e438
//lint -e666
static GstFlowReturn
gst_smevideosink_hihal_render (GstBaseSink * bsink, GstBuffer * buffer)
{
    GstMapInfo              info;
    GstFlowReturn           eRet        = GST_FLOW_OK;
    GstSmeVideoSinkHiHal *  vsink       = GST_SME_VIDEOSINK_HIHAL(bsink);
    GstVideoMeta*           vmeta       = NULL;
    GstClockTime pts = GST_BUFFER_PTS(buffer);

#if 0
    /* video data queue overflow and try to catch up with audio's speed, added by xueping */
    if (gst_base_sink_get_buffer_status(bsink) == GST_SME_OVERRUN){
          GST_WARNING_OBJECT(bsink, "%s:video GST_SME_OVERRUN", AVSYNC_BY_AUDIO_TAG);
        gst_base_sink_set_max_lateness(bsink, GST_SME_BUFFER_CTRL_LATENESS);
    }else{
#ifdef AVSYNC_BY_AUDIO_QOS_UNDERRUN
        if(gst_base_sink_get_buffer_status(bsink) == GST_SME_UNDERRUN){
          GST_WARNING_OBJECT(bsink, "%s:video GST_SME_UNDERRUN", AVSYNC_BY_AUDIO_TAG);
        }
#endif
        gst_base_sink_set_max_lateness(bsink, GST_SME_VIDEO_MAX_LATENESS);
    }
#endif

    if(vsink->pts_calibration_hdl){
      gst_sme_pts_calibration_entry_update_presync_info(vsink->pts_calibration_hdl,
        E_ENTRY_TYPE_VIDEO, bsink->slow_presync_info);
    }
    vsink->vsink_proc_info.VideoPts = (V_INT64)pts;
    if(vsink->is_first_frame){
      vsink->is_first_frame = FALSE;
      gettimeofday(&vsink->vsink_proc_info.VideoSinkFirstFrameTimeMs, NULL);
    }
    vsink->vsink_proc_info.stAVSyncInfo.i64BaseTime = (gint64)GST_VALID_TIME_AS_MSECONDS(
      gst_element_get_base_time(GST_ELEMENT(bsink)));
    GST_DEBUG_OBJECT(vsink, "+-basetime=%lld", vsink->vsink_proc_info.stAVSyncInfo.i64BaseTime);
    if(GST_ELEMENT_CLOCK(GST_ELEMENT(bsink))){
      vsink->vsink_proc_info.stAVSyncInfo.i64ClockTime = (gint64)GST_VALID_TIME_AS_MSECONDS(
        gst_clock_get_time(GST_ELEMENT_CLOCK(GST_ELEMENT(bsink))));
    }else{
      vsink->vsink_proc_info.stAVSyncInfo.i64ClockTime = -1LL;
    }
    if(GST_CLOCK_TIME_IS_VALID(vsink->vsink_proc_info.stAVSyncInfo.i64ClockTime)){
      vsink->vsink_proc_info.stAVSyncInfo.i64ClockRunningTime =
        vsink->vsink_proc_info.stAVSyncInfo.i64ClockTime -
        vsink->vsink_proc_info.stAVSyncInfo.i64BaseTime;
      GST_DEBUG_OBJECT(vsink, "i64ClockRunningTime:%lld, clocktime:%lld, basetime:%lld",
        vsink->vsink_proc_info.stAVSyncInfo.i64ClockRunningTime,
        vsink->vsink_proc_info.stAVSyncInfo.i64ClockTime, vsink->vsink_proc_info.stAVSyncInfo.i64BaseTime);
    }else{
      vsink->vsink_proc_info.stAVSyncInfo.i64ClockRunningTime = -1LL;
    }
    vsink->vsink_proc_info.stAVSyncInfo.i64BufferPts = (gint64)GST_VALID_TIME_AS_MSECONDS(pts);
    vsink->vsink_proc_info.stAVSyncInfo.i64Duration = (gint64)GST_VALID_TIME_AS_MSECONDS(GST_BUFFER_DURATION(buffer));
    if(vsink->vsink_proc_info.stAVSyncInfo.dbRate != 0.0){

      if(0 == bsink->slow_presync_info.slow_presync_state){
        vsink->vsink_proc_info.stAVSyncInfo.i64BufferRunningTime = (gint64)((((gint64)GST_VALID_TIME_AS_MSECONDS(
          pts) - vsink->vsink_proc_info.stAVSyncInfo.i64SegStart) /
            ABS(vsink->vsink_proc_info.stAVSyncInfo.dbRate))
            + vsink->vsink_proc_info.stAVSyncInfo.i64Segbase);
      }else if (1 == bsink->slow_presync_info.slow_presync_state
        || !GST_CLOCK_TIME_IS_VALID(bsink->slow_presync_info.last_presync_pts)){
        gdouble tmp_rate = vsink->vsink_proc_info.stAVSyncInfo.dbRate;

        if(bsink->slow_presync_info.gdrate != 0.0){
          tmp_rate = bsink->slow_presync_info.gdrate;
        }
        vsink->vsink_proc_info.stAVSyncInfo.i64BufferRunningTime = (gint64)((((gint64)GST_VALID_TIME_AS_MSECONDS(
          pts) - vsink->vsink_proc_info.stAVSyncInfo.i64SegStart) /
          ABS(tmp_rate))
          + vsink->vsink_proc_info.stAVSyncInfo.i64Segbase);
        GST_DEBUG_OBJECT(vsink, "i64BufferRunningTime:%lld, pts:%lld, seg-start:%lld, tmp_rate:%f,segbase:%lld",
          vsink->vsink_proc_info.stAVSyncInfo.i64BufferRunningTime,
          pts, vsink->vsink_proc_info.stAVSyncInfo.i64SegStart,
          tmp_rate, vsink->vsink_proc_info.stAVSyncInfo.i64Segbase);
      }else{
        gint64 tmp_start = (gint64)GST_VALID_TIME_AS_MSECONDS(bsink->slow_presync_info.last_presync_pts);
        vsink->vsink_proc_info.stAVSyncInfo.i64BufferRunningTime = (gint64)((((gint64)GST_VALID_TIME_AS_MSECONDS(
          pts) - tmp_start) /
          ABS(vsink->vsink_proc_info.stAVSyncInfo.dbRate))
          + vsink->vsink_proc_info.stAVSyncInfo.i64Segbase
          + (gint64)GST_VALID_TIME_AS_MSECONDS(bsink->slow_presync_info.slow_presync_stime));
        GST_DEBUG_OBJECT(vsink, "2 i64BufferRunningTime:%lld, pts:%lld, seg-start:%lld, last_pts:%lld,tmp_rate:%f,segbase:%lld,stime:%lld",
          vsink->vsink_proc_info.stAVSyncInfo.i64BufferRunningTime,
          pts, vsink->vsink_proc_info.stAVSyncInfo.i64SegStart,
          bsink->slow_presync_info.last_presync_pts,
          vsink->vsink_proc_info.stAVSyncInfo.dbRate, vsink->vsink_proc_info.stAVSyncInfo.i64Segbase,
          bsink->slow_presync_info.slow_presync_stime);
      }
    }else{
      vsink->vsink_proc_info.stAVSyncInfo.i64BufferRunningTime = ((gint64)GST_VALID_TIME_AS_MSECONDS(
        pts) - vsink->vsink_proc_info.stAVSyncInfo.i64SegStart)
        + vsink->vsink_proc_info.stAVSyncInfo.i64Segbase;
    }
    vsink->vsink_proc_info.stAVSyncInfo.i64MaxLatency = (gint64)GST_VALID_TIME_AS_MSECONDS(
      gst_base_sink_get_max_lateness(bsink));
    vsink->vsink_proc_info.stAVSyncInfo.i64CurRenderDelay = (gint64)GST_VALID_TIME_AS_MSECONDS(
      gst_base_sink_get_render_delay(bsink));
    if(gst_base_sink_get_sync(bsink) && vsink->vsink_proc_info.stAVSyncInfo.i64ClockRunningTime != -1LL){
      vsink->vsink_proc_info.stAVSyncInfo.i64Diff = vsink->vsink_proc_info.stAVSyncInfo.i64ClockRunningTime -
        (vsink->vsink_proc_info.stAVSyncInfo.i64BufferRunningTime -
        vsink->vsink_proc_info.stAVSyncInfo.i64CurRenderDelay);
      GST_DEBUG_OBJECT(vsink, "i64Diff:%lld,i64ClockRunningTime:%lld, i64BufferRunningTime:%lld, i64CurRenderDelay:%lld",
        vsink->vsink_proc_info.stAVSyncInfo.i64Diff,
        vsink->vsink_proc_info.stAVSyncInfo.i64ClockRunningTime,
        vsink->vsink_proc_info.stAVSyncInfo.i64BufferRunningTime,
        vsink->vsink_proc_info.stAVSyncInfo.i64CurRenderDelay);

    }else{
      vsink->vsink_proc_info.stAVSyncInfo.i64Diff = 0LL;
    }

    //check to drop frame.
    if(0 == vsink->u32_late_count && !vsink->is_flushing){
      if(vsink->vsink_proc_info.stAVSyncInfo.i64Diff > (GST_SME_VIDEO_LATENESS_NORM * 3)){
        vsink->u32_drop_interleave = GST_SME_DROP_INTERLEAVE_L1;
      }else if (vsink->vsink_proc_info.stAVSyncInfo.i64Diff > (GST_SME_VIDEO_LATENESS_NORM * 2)){
        vsink->u32_drop_interleave = GST_SME_DROP_INTERLEAVE_L2;
      }else if (vsink->vsink_proc_info.stAVSyncInfo.i64Diff > (GST_SME_VIDEO_LATENESS_NORM)){
        vsink->u32_drop_interleave = GST_SME_DROP_INTERLEAVE_L3;
      }else{
        vsink->u32_drop_interleave = 0;
      }
    }

    if(0 != vsink->u32_drop_interleave){
      GST_WARNING_OBJECT(bsink, "too late: to observe,diff:%lld",
        vsink->vsink_proc_info.stAVSyncInfo.i64Diff);
      vsink->u32_late_count++;
    }

    if((0 != vsink->u32_late_count)
      && 0 == (vsink->u32_late_count % vsink->u32_drop_interleave)){
      GST_WARNING_OBJECT(bsink, "too late: drop it,diff:%lld",
        vsink->vsink_proc_info.stAVSyncInfo.i64Diff);
      vsink->u32_late_count = 0;

      return eRet;
    }

    //this func will cost sometime, so we should unlock, let state pause->playing more quick.
    GST_BASE_SINK_PREROLL_UNLOCK (bsink);
    g_mutex_lock(&vsink->rendered_buf_muxtex);
    vsink->is_flushing = FALSE;
    g_mutex_unlock(&vsink->rendered_buf_muxtex);
    vmeta = gst_buffer_get_video_meta (buffer);
    gst_buffer_map (buffer, &info, GST_MAP_READ /*| GST_MAP_WRITE*/);

    if(NULL != vsink->hdl_sme_pipe_hihal){
        guint       width             = 0;
        guint       height            = 0;
        E_SME_HIVO_ERR    eErr = E_SME_HIVO_ERR_NONE;
        ST_SME_VIDEO_FRAME stVideoFrame;
        E_SME_MEM_SECURE_RET eMemSecRet;
        guint u_ret;

        eMemSecRet = VOS_Memset_S(&stVideoFrame, sizeof(ST_SME_VIDEO_FRAME), 0, sizeof(ST_SME_VIDEO_FRAME));
        if(eMemSecRet != E_SME_MEM_OK){
            GST_ERROR("init stVideoFrame failed,ret:%d", eMemSecRet);
            return GST_FLOW_ERROR;
        }
        if(vmeta && vmeta->width > 0 && vmeta->height > 0 &&  vmeta->width != (guint)-1 && vmeta->height != (guint)-1){
            width = vmeta->width;
            height = vmeta->height;
        }else{
            width = (guint)vsink->stVideoInfo.stVideoRes.i32Width;
            height = (guint)vsink->stVideoInfo.stVideoRes.i32Height;
        }
        stVideoFrame.u32Width = width;
        stVideoFrame.u32Height = height;
        stVideoFrame.u32Len = info.size;
        stVideoFrame.pvAddr[0] = info.data;
        //stVideoFrame.pvAddr[1] = gst_smevideosink_hihal_get_phyaddr(bsink, stVideoFrame.pvAddr[0]);
        //if(stVideoFrame.pvAddr[1] == NULL){
        //    GST_ERROR("Get PhyAddr failed,Quit!");
        //    return GST_FLOW_ERROR;
        //}


        /* dump raw data*/
        //lint -e737
        if(vsink->dump_raw_switch){
            if(vsink->dump_raw_loca){
                if(fwrite(info.data, 1, info.size, vsink->dump_raw_loca) != info.size){
                    GST_WARNING("write raw data failed");
                }
            }
        }
        //lint +e737

#ifdef USE_DUMP_DECED_DATA
        gint frame_len = 0;
        frame_len = width * height;
        if(vsink->dump_deced_data != NULL){
           if(fwrite(info.data, 1, frame_len, vsink->dump_deced_data) != frame_len){
             GST_WARNING("write decodered data failed");
           }
        }
#endif

        /* BEGIN: Update disp rect when ratio/mode/winrect have changed */
        g_mutex_lock(&vsink->rendered_buf_muxtex);
        if(vsink->e_disp_mode == E_SME_PLAYER_DIS_MODE_DEFAULT &&
            (vsink->st_disp_ratio.u32DispRatioW != width || vsink->st_disp_ratio.u32DispRatioH != height)){
            vsink->st_disp_ratio.u32DispRatioW = width;
            vsink->st_disp_ratio.u32DispRatioH = height;
        }
        u_ret = gst_smevideosink_hihal_update_display_mode(vsink);
        g_mutex_unlock(&vsink->rendered_buf_muxtex);
        if(ICS_SUCCESS != u_ret){
            GST_WARNING("update display mode failed");
        }
        /* END: Update disp rect when ratio/mode/winrect have changed  */


        GST_DEBUG("begin hivo:pts=%lld, dts=%lld", GST_BUFFER_PTS(buffer), GST_BUFFER_DTS(buffer));
        do
        {
            //todo mv to befor do{}while...
            /* Queue ,set flag to q_mmz_is_vo */
            g_mutex_lock(&vsink->mmz_is_vo_muxtex);
            guint q_mmz_is_vo_len = 0;
            q_mmz_is_vo_len = g_queue_get_length(&vsink->q_mmz_is_vo);
            guint i = 0;
            for(i = 0; i < q_mmz_is_vo_len; i++){
                GstMmzInfo* item = g_queue_peek_nth(&vsink->q_mmz_is_vo, i);
                if(!item){
                    GST_ERROR("peek GstVideoMmzVoFlag :%d is null, why?", i);
                    continue;
                }
                if(item->pVirAddr == stVideoFrame.pvAddr[0]){
                  item->mmz_is_vo = TRUE;
                  break;
                }
            }
            g_mutex_unlock(&vsink->mmz_is_vo_muxtex);

            gettimeofday(&vsink->current_play_timeUs, NULL);
            if(vsink->is_first_render){
              vsink->is_first_render = FALSE;
              gettimeofday(&vsink->vsink_proc_info.VideoFirstPlayTimeMs, NULL);
            }

            if(GST_CLOCK_TIME_IS_VALID(GST_BUFFER_DURATION(buffer))
              && GST_BUFFER_DURATION(buffer) > GST_MSECOND){
              stVideoFrame.u32Duration = (guint)(GST_BUFFER_DURATION(buffer) / GST_MSECOND);
            }else{
              stVideoFrame.u32Duration = DEFAULT_FRAME_DURATION;
              GST_WARNING_OBJECT(bsink, "use default duration:%d,duration:%lld",
                DEFAULT_FRAME_DURATION, GST_BUFFER_DURATION(buffer));
            }

            GstVideoFrameMap* item = g_malloc(sizeof(GstVideoFrameMap));
            if(NULL == item){
              GST_ERROR("g_malloc failed(%d)", eErr);
              eRet = GST_FLOW_ERROR;
              break;
            }
            item->gstvbuf = gst_buffer_ref(buffer);
            item->pvAddr = info.data;
            g_mutex_lock(&vsink->rendered_buf_muxtex);
            g_queue_push_tail(&vsink->q_rendered_buf, item);
            g_mutex_unlock(&vsink->rendered_buf_muxtex);

            eErr = SME_Hal_VDisp_QueueFrame(vsink->hdl_sme_pipe_hihal, &stVideoFrame);
            if(E_SME_HIVO_ERR_NONE != eErr && E_SME_HIVO_ERR_BUF_FULL != eErr){
                GST_ERROR("SME_HiVO_QueueFrame failed(%d)", eErr);
                g_mutex_lock(&vsink->rendered_buf_muxtex);
                GstVideoFrameMap* item_tail = g_queue_pop_tail(&vsink->q_rendered_buf);
                if(item_tail != NULL)
                {
                    gst_buffer_unref(item_tail->gstvbuf);
                    g_free(item_tail);
                }
                g_mutex_unlock(&vsink->rendered_buf_muxtex);
                eRet = GST_FLOW_ERROR;
            }else if (E_SME_HIVO_ERR_NONE == eErr){
                eRet = GST_FLOW_OK;

#ifndef DEQUEUE_FRAME_ENABLE
                /* if dequeue failed ,we should use it to test */
                if(g_queue_get_length(&vsink->q_rendered_buf) >= GST_SME_VSINK_RENDER_BUF_CNT_MIN){
                    GstVideoFrameMap* itempop = g_queue_pop_head(&vsink->q_rendered_buf);
                    if(itempop){
                        if(itempop->gstvbuf){
                            gst_buffer_unref(itempop->gstvbuf);
                        }else{
                            GST_ERROR("itempop->gstvbuf is null ,why?");
                        }
                        g_free(itempop);
                    }else{
                        GST_ERROR("g_queue_pop_head failed");
                    }
                }
#endif

            }else{
                g_mutex_lock(&vsink->rendered_buf_muxtex);
                GstVideoFrameMap* item_tail = g_queue_pop_tail(&vsink->q_rendered_buf);
                if(item_tail != NULL)
                {
                    gst_buffer_unref(item_tail->gstvbuf);
                    g_free(item_tail);
                }
                g_mutex_unlock(&vsink->rendered_buf_muxtex);
                g_usleep(2000);
            }
        }while(E_SME_HIVO_ERR_BUF_FULL == eErr);
    }
    else
    {
        GST_ERROR("unsupport");
        eRet = GST_FLOW_ERROR;
    }

    //update sync infomation frome the basesink
    vsink->vsink_proc_info.DroppedVideoFrameCntAtSync = bsink->sync_info.DroppedVideoFrameCntAtSync;
    vsink->vsink_proc_info.VideoFrameIsDelay = (V_BOOL)bsink->sync_info.VideoFrameIsDelay;
    vsink->vsink_proc_info.IsSync = (V_BOOL)gst_base_sink_get_sync(bsink);
    if (GST_ELEMENT_CLOCK(GST_ELEMENT(vsink)))
      vsink->vsink_proc_info.ClockForSync = (V_INT64)gst_clock_get_time(GST_ELEMENT_CLOCK(GST_ELEMENT(vsink)));
    gst_buffer_unmap (buffer, &info);
    GST_DEBUG("gst_smevideosink_hihal_render out");
    GST_DEBUG("hivo:pts=%lld, dts=%lld", GST_BUFFER_PTS(buffer), GST_BUFFER_DTS(buffer));

    GST_BASE_SINK_PREROLL_LOCK (bsink);

    return eRet;

}
//lint +e529
//lint +e438
//lint +e666

static gboolean
gst_smevideosink_hihal_send_event (GstElement * element, GstEvent * event)
{
    return GST_ELEMENT_CLASS(parent_class)->send_event (element, event);
}

static gboolean
gst_smevideosink_hihal_event (GstBaseSink * bsink, GstEvent * event)
{
    GstSmeVideoSinkHiHal* vsink = GST_SME_VIDEOSINK_HIHAL(bsink);
    gboolean bres = FALSE;
    GstEventType event_type;
    GstSegment segment;

    event_type = GST_EVENT_TYPE(event);
    if(GST_EVENT_EOS == event_type)
    {
        /** BEGIN:   Added for DTS2016042901380 , 2016/5/4 */
        g_mutex_lock(&vsink->rendered_buf_muxtex);
        while(g_queue_get_length(&vsink->q_rendered_buf) > 1 ){
            /**wait for dequeue_thd to dequeue & release
               Here, we need to unlock the mutex because it will be used
               in Dequeue thread, or else it maybe dead lock*/
            g_mutex_unlock(&vsink->rendered_buf_muxtex);
            g_usleep(2000);
            g_mutex_lock(&vsink->rendered_buf_muxtex);
        }
        g_mutex_unlock(&vsink->rendered_buf_muxtex);
        /** END:   Added for DTS2016042901380 , 2016/5/4 */
        vsink->recieve_eos = TRUE;

    }else if ((GST_EVENT_FLUSH_START == event_type)
        || (GST_EVENT_FLUSH_STOP == event_type)){
        vsink->recieve_eos = FALSE;
    }else if (GST_EVENT_SEGMENT == event_type){
        gst_segment_init(&segment, GST_FORMAT_TIME);
        gst_event_copy_segment (event, &segment);
    }else{
        ;//do nothing
    }

    bres = GST_BASE_SINK_CLASS (parent_class)->event (bsink, event);

    if (GST_EVENT_FLUSH_STOP == event_type){
        g_mutex_lock(&vsink->rendered_buf_muxtex);
        vsink->is_flushing = TRUE;
        g_mutex_unlock(&vsink->rendered_buf_muxtex);
        if(vsink->pts_calibration_hdl){
          gst_sme_pts_calibration_entry_clear(vsink->pts_calibration_hdl,
            E_ENTRY_TYPE_VIDEO);
        }
        vsink->u32_late_count = 0;
        vsink->u32_drop_interleave = 0;
        vsink->is_first_frame = TRUE;
        vsink->is_first_render = TRUE;
    }else if (GST_EVENT_SEGMENT == event_type){
        if(vsink->pts_calibration_hdl){
          gst_sme_pts_calibration_entry_update_segment(vsink->pts_calibration_hdl,
            E_ENTRY_TYPE_VIDEO, segment);
          gst_sme_pts_calibration_entry_update_presync_info(vsink->pts_calibration_hdl,
            E_ENTRY_TYPE_VIDEO, bsink->slow_presync_info);
        }

        vsink->vsink_proc_info.stAVSyncInfo.i64Segbase = (gint64)GST_VALID_TIME_AS_MSECONDS(segment.base);
        vsink->vsink_proc_info.stAVSyncInfo.i64SegStart = (gint64)GST_VALID_TIME_AS_MSECONDS(segment.start);
        vsink->vsink_proc_info.stAVSyncInfo.i64SegStop = (gint64)GST_VALID_TIME_AS_MSECONDS(segment.stop);
        vsink->vsink_proc_info.stAVSyncInfo.dbRate = segment.rate;
    }

    return bres;
}

static gboolean
gst_smevideosink_hihal_query (GstBaseSink * bsink, GstQuery * query)
{
    GstSmeVideoSinkHiHal* vsink = GST_SME_VIDEOSINK_HIHAL(bsink);
    gboolean ret = TRUE;
    GST_INFO_OBJECT(bsink, "sink_hihal_query in,type=%s",
        GST_QUERY_TYPE_NAME(query));
    switch (GST_QUERY_TYPE (query)) {
      //ÕâÀïÄÜ×ÓÊµÏÖÐèÒª´¦ÀíµÄtype
      case GST_QUERY_DRAIN:
      {
          g_mutex_lock(&vsink->rendered_buf_muxtex);
          while(!g_queue_is_empty(&vsink->q_rendered_buf)){
              GstVideoFrameMap* item = g_queue_pop_head(&vsink->q_rendered_buf);
              if(item){
                  gst_buffer_unref(item->gstvbuf);
                  g_free(item);
              }
          }
          g_mutex_unlock(&vsink->rendered_buf_muxtex);
          break;
      }
    case GST_QUERY_ALLOCATION:
    {
        GST_INFO("GST_QUERY_ALLOCATION SINK QUERY IN");
//lint -e785 //_gst_reserved need not init
//lint -e64
        GstAllocationParams params = { 0, 0, 0, 0, };
//lint +e785
//lint +e64
        GstAllocator* pallocator = NULL;
        pallocator = (GstAllocator*)gst_allcator_sme_hivommz_new(vsink);
        if(pallocator){
          gst_query_add_allocation_param (query, pallocator, &params);
        }else{
          GST_ERROR("get allocator failed");
          ret = FALSE;
        }
        /* BEGIN: Added for DTS2016042806936,  2016/05/25 */
        gst_object_unref(pallocator);
        /* END: Added for DTS2016042806936,  2016/05/25 */
        break;
    }
      default:
        break;
    }

    if(ret){
        ret = GST_BASE_SINK_CLASS (parent_class)->query (bsink, query);
        GST_INFO_OBJECT(bsink, "sink_hihal_query out(%d),type=%s", ret,
            GST_QUERY_TYPE_NAME(query));
    }
    return ret;
}

static GstFlowReturn
gst_smevideosink_hihal_wait_event (GstBaseSink * bsink, GstEvent * event)
{
    GstFlowReturn           ret     = GST_FLOW_OK;
    GstSmeVideoSinkHiHal*   vsink   = GST_SME_VIDEOSINK_HIHAL(bsink);

    if(vsink->recieve_eos){
        GST_INFO("sme video sink hihal recv eos!");
    }

    ret = GST_BASE_SINK_CLASS (parent_class)->wait_event (bsink, event);

    return ret;
}
//lint -e850
static gpointer DequeueThdFunc (gpointer data)
{
    //GstSmeVideoSinkHiVO* vsink = (GstSmeVideoSinkHiVO*)data;
    GstSmeVideoSinkHiHal *  vsink = (GstSmeVideoSinkHiHal*)data;
    E_SME_HIVO_ERR eErr = E_SME_HIVO_ERR_NONE;
    guint uToBeFreeLen = 0;

    GST_INFO("DequeueThdFunc in.");
    if(!vsink){
        GST_ERROR("vsink is null!");
        return NULL;
    }
    while(!vsink->is_dequeue_thd_exit){
        ST_SME_VIDEO_FRAME stVideoFrame;
        guint q_rendered_buf_len = 0;
        guint i = 0;
        E_SME_MEM_SECURE_RET eMemSecRet;
        gboolean in_tobefree = FALSE;
        guint q_mmz_is_vo_len = 0;
        HI_S32 hMmzFree = HI_FAILURE;
        guint u_ret;

        g_mutex_lock(&vsink->rendered_buf_muxtex);
        u_ret = gst_smevideosink_hihal_update_display_mode(vsink);
        g_mutex_unlock(&vsink->rendered_buf_muxtex);
        if(ICS_SUCCESS != u_ret){
            GST_WARNING("update display mode failed");
        }


        eMemSecRet = VOS_Memset_S(&stVideoFrame, sizeof(ST_SME_VIDEO_FRAME), 0, sizeof(ST_SME_VIDEO_FRAME));
        if(eMemSecRet != E_SME_MEM_OK){
            GST_ERROR("init stVideoFrame failed,ret:%d", eMemSecRet);
            break;
        }
        eErr = SME_Hal_VDisp_DeQueueFrame(vsink->hdl_sme_pipe_hihal, &stVideoFrame);
        if(eErr != E_SME_HIVO_ERR_NONE){
            g_usleep(2000);
            continue;
        }

        /*find virtual addr by phy addr*/
        g_mutex_lock(&vsink->mmz_is_vo_muxtex);
        guint len_of_q_mmz = g_queue_get_length(&vsink->q_mmz_is_vo);
        for(i = 0; i < len_of_q_mmz; i++){
            GstMmzInfo* item = g_queue_peek_nth(&vsink->q_mmz_is_vo, i);
            if(!item){
                GST_ERROR("peek GstVideoMmzVoFlag :%d is null, why?", i);
                continue;
            }
            if(item->pPhyAddr == stVideoFrame.pvAddr[1]){
                stVideoFrame.pvAddr[0] = item->pVirAddr;
                break;
            }
        }
        g_mutex_unlock(&vsink->mmz_is_vo_muxtex);

       /* check if buf is in to_be_free, if true, free deeply*/
        g_mutex_lock(&vsink->mmz_is_vo_muxtex);
        uToBeFreeLen = g_list_length(gToBeFree);
        if(uToBeFreeLen > 0)
            GST_INFO("uToBeFreeLen is %d", uToBeFreeLen);

        for(i = 0; i < g_list_length(gToBeFree); i++){
            //GstMmzInfo* item = g_queue_peek_nth(&vsink->q_to_be_Free, idx);
            GstMmzInfo* item = g_list_nth_data(gToBeFree, i);
            if(!item){
                GST_ERROR("peek:%d is null, why?", i);
                continue;
            }

            if(item->pPhyAddr == stVideoFrame.pvAddr[1] || !item->mmz_is_vo){
                if(item->pPhyAddr == stVideoFrame.pvAddr[1]){
                    in_tobefree = TRUE;
                }
                hMmzFree = HI_MMZ_Free(&item->stMmzBuf);
                if(HI_FAILURE == hMmzFree){
                    GST_WARNING("VSINK,FREE MMZ FAILED %p", item->pVirAddr);
                }
                gToBeFree = g_list_remove(gToBeFree, item);
                g_free(item);
                i--;
                if(g_list_length(gToBeFree) == 0){
                    GST_INFO("deep free total succeed");
                }
            }
        }

        if(in_tobefree){
            g_mutex_unlock(&vsink->mmz_is_vo_muxtex);
            continue;
        }

        q_mmz_is_vo_len = g_queue_get_length(&vsink->q_mmz_is_vo);
        for(i = 0; i < q_mmz_is_vo_len; i++){
            GstMmzInfo* item = g_queue_peek_nth(&vsink->q_mmz_is_vo, i);
            if(!item){
                GST_ERROR("peek GstVideoMmzVoFlag :%d is null, why?", i);
                continue;
            }
            if(item->pVirAddr == stVideoFrame.pvAddr[0]){
                item->mmz_is_vo = FALSE;
                break;
            }
        }
        g_mutex_unlock(&vsink->mmz_is_vo_muxtex);

        g_mutex_lock(&vsink->rendered_buf_muxtex);
        q_rendered_buf_len = g_queue_get_length(&vsink->q_rendered_buf);
        for(i = 0; i < q_rendered_buf_len; i++){
            GstVideoFrameMap* item = g_queue_peek_nth(&vsink->q_rendered_buf, i);
            if(!item){
                GST_ERROR("peek:%d is null, why?", i);
                continue;
            }
            if(item->pvAddr == stVideoFrame.pvAddr[0]){
                if(NULL != item->gstvbuf){
                    GST_INFO("Q,UREF,pvVirAddr is %p",stVideoFrame.pvAddr[0]);
                    gst_buffer_unref(item->gstvbuf);
                }
                (void)g_queue_pop_nth(&vsink->q_rendered_buf, i);
                g_free(item);
                break;
            }
        }
        g_mutex_unlock(&vsink->rendered_buf_muxtex);
    }
    GST_INFO("DequeueThdFunc out.");

    return NULL;
}
//lint +e850
static V_UINT32 gst_smevideosink_hihal_wr_init(GstSmeVideoSinkHiHal* vsink,
    HI_RECT_S stRect, void *hdlDisp, gboolean is_Rect)
{
    V_UINT32 u32Ret = ICS_SUCCESS;
    guint uHiHalListLen = 0;
    guint idx = 0;
    gboolean find_hivo = FALSE;

    (void)hdlDisp;
    (void)is_Rect;

    uHiHalListLen = g_list_length(gHiHalList);
    GST_DEBUG("old uHiHalListLen is %d", uHiHalListLen);
    for(idx = 0; idx < uHiHalListLen; idx++){
        HiHalItem* pHiHalItem = g_list_nth_data(gHiHalList, idx);
        if(pHiHalItem){
            GST_INFO("pHiHalItem->st_vdisp_rect is [%d, %d, %d, %d]",
                pHiHalItem->st_vdisp_rect.i32StartX,
                pHiHalItem->st_vdisp_rect.i32StartY, pHiHalItem->st_vdisp_rect.i32Width,
                pHiHalItem->st_vdisp_rect.i32Height);
            GST_INFO("stRect is [%d, %d, %d, %d]",stRect.s32X,stRect.s32Y,
                stRect.s32Width, stRect.s32Height);

            if(pHiHalItem->st_vdisp_rect.i32StartX == stRect.s32X &&
                pHiHalItem->st_vdisp_rect.i32StartY == stRect.s32Y &&
                pHiHalItem->st_vdisp_rect.i32Width == stRect.s32Width &&
                pHiHalItem->st_vdisp_rect.i32Height == stRect.s32Height){
                vsink->hdl_sme_pipe_hihal = pHiHalItem->hdl_sme_wr_hihal;
                find_hivo = TRUE;
                GST_DEBUG("find_hivo is true");
                break;
            }
        }else{
            GST_WARNING("pHiHalItem is NULL");
        }
    }

    if(!find_hivo){
        //clear all old HiHalItem
        for(idx = 0; idx < uHiHalListLen; idx++){
            HiHalItem* pHiHalItem = g_list_nth_data(gHiHalList, idx);
            if(pHiHalItem){
                SME_Hal_VDisp_DeInit(pHiHalItem->hdl_sme_wr_hihal);
                g_free(pHiHalItem);
            }
        }
        g_list_free(gHiHalList);
        gHiHalList = NULL;
        //create new HiHalItem
        u32Ret = SME_Hal_VDisp_Init(vsink->hdl_sme_pipe_hihal);
        if(ICS_SUCCESS != u32Ret){
            GST_ERROR_OBJECT(vsink, "SME_HiVO_Init failed:%#x", u32Ret);
            return ICS_FAIL;
        }else{
            if(vsink->switch_channel_mode == E_SME_PLAYER_SWITCH_CHANNEL_MODE_LAST){
                HiHalItem* pHiHalItem = g_malloc(sizeof(HiHalItem));
                if(!pHiHalItem){
                    GST_ERROR("malloc HiHalItem failed");
                }else{
                    GST_WARNING("PUT gHiHalList into List");
                    pHiHalItem->hdl_sme_wr_hihal = vsink->hdl_sme_pipe_hihal;
                    pHiHalItem->st_vdisp_rect.i32StartX = stRect.s32X;
                    pHiHalItem->st_vdisp_rect.i32StartY = stRect.s32Y;
                    pHiHalItem->st_vdisp_rect.i32Width = stRect.s32Width;
                    pHiHalItem->st_vdisp_rect.i32Height = stRect.s32Height;
                    gHiHalList = g_list_append(gHiHalList, pHiHalItem);
                    uHiHalListLen = g_list_length(gHiHalList);
                    GST_DEBUG("Add new HiHalItem, the new gHiHalList len is %d", uHiHalListLen);
                }
            }
        }
    }

    return u32Ret;
}

static void gst_smevideosink_hihal_wr_deinit(GstSmeVideoSinkHiHal* vsink)
{
    guint q_mmz_is_vo_len = 0;
    guint all_is_ture = 0;
    guint idx = 0;
    guint uHiHalListLen = 0;

    vsink->is_flushing = FALSE;
    /* if switch mode is static,do not Deinit,if switch mode is black, do Deinit*/
    if(vsink->switch_channel_mode == E_SME_PLAYER_SWITCH_CHANNEL_MODE_BLACK){
        GST_WARNING("QUIT BLACK");
        SME_Hal_VDisp_Reset(vsink->hdl_sme_pipe_hihal, E_SME_HIVO_SWITCH_TYPE_BLACK);

        while(1){
            all_is_ture = 0;

            /* Make sure every buffer has been dequeue*/
            g_mutex_lock(&vsink->mmz_is_vo_muxtex);
            q_mmz_is_vo_len = g_queue_get_length(&vsink->q_mmz_is_vo);
            for(idx = 0; idx < q_mmz_is_vo_len; idx++){
                GstMmzInfo* item = g_queue_peek_nth(&vsink->q_mmz_is_vo, idx);
                if(!item){
                    GST_ERROR("peek GstVideoMmzVoFlag :%d is null, why?", idx);
                    continue;
                }
                if(item->mmz_is_vo == FALSE){
                    all_is_ture++;
                }
            }
            g_mutex_unlock(&vsink->mmz_is_vo_muxtex);

            GST_WARNING("when set balck reset,number of dequeued frames is :%d, total frames number is: %d",all_is_ture, q_mmz_is_vo_len);
            if(all_is_ture != q_mmz_is_vo_len){
                g_usleep(2000);
            }else{
                break;
            }
        }

        /* to make sure there is on frame in windows */
        GST_WARNING("MMZ to be deeply free in");
        g_mutex_lock(&vsink->mmz_is_vo_muxtex);
        while(g_list_length(gToBeFree) > 0){
            g_mutex_unlock(&vsink->mmz_is_vo_muxtex);
            g_usleep(2000);
            g_mutex_lock(&vsink->mmz_is_vo_muxtex);
        }
        g_mutex_unlock(&vsink->mmz_is_vo_muxtex);
        GST_WARNING("MMZ to be deeply free out");


        /* end the thread */
        vsink->is_dequeue_thd_exit = TRUE;
        g_thread_join(vsink->dequeue_thd);
        vsink->dequeue_thd = NULL;
        vsink->is_dequeue_thd_exit = FALSE;

        SME_Hal_VDisp_DeInit(vsink->hdl_sme_pipe_hihal);
        GST_INFO_OBJECT(vsink, "hihal_deinit end");
        SME_Hal_VDisp_Destory(vsink->hdl_sme_pipe_hihal);
        GST_INFO_OBJECT(vsink, "hivo_VDisp_Destory end");
        vsink->hdl_sme_pipe_hihal = NULL;

        //need clear gHiHalList
        uHiHalListLen = g_list_length(gHiHalList);
        for(idx = 0; idx < uHiHalListLen; idx++){
            HiHalItem* pHiHalItem = g_list_nth_data(gHiHalList, idx);
            if(pHiHalItem){
                g_free(pHiHalItem);
            }
        }
        g_list_free(gHiHalList);
        gHiHalList = NULL;

    }else if(vsink->switch_channel_mode == E_SME_PLAYER_SWITCH_CHANNEL_MODE_LAST){
        SME_Hal_VDisp_Reset(vsink->hdl_sme_pipe_hihal, E_SME_HIVO_SWITCH_TYPE_LAST);
        while(1){
            all_is_ture = 0;
            /* Make sure only one buffer has been left*/
            g_mutex_lock(&vsink->mmz_is_vo_muxtex);
            q_mmz_is_vo_len = g_queue_get_length(&vsink->q_mmz_is_vo);
            for(idx = 0; idx < q_mmz_is_vo_len; idx++){
                GstMmzInfo* item = g_queue_peek_nth(&vsink->q_mmz_is_vo, idx);
                if(!item){
                    GST_ERROR("peek GstVideoMmzVoFlag :%d is null, why?", idx);
                    continue;
                }
                if(item->mmz_is_vo == FALSE){
                    all_is_ture++;
                }
            }
            g_mutex_unlock(&vsink->mmz_is_vo_muxtex);
            if(all_is_ture != (q_mmz_is_vo_len - 1)){
                g_usleep(2000);
                GST_WARNING("STATIC TRUE BUFFERS LEN IS %d",all_is_ture);
            }else{
                break;
            }
        }

#ifdef DEQUEUE_FRAME_ENABLE
        /*end the thread */
        vsink->is_dequeue_thd_exit = TRUE;
        g_thread_join(vsink->dequeue_thd);
        vsink->dequeue_thd = NULL;
        vsink->is_dequeue_thd_exit = FALSE;
#endif
        /*do not forget the last frame into hivom,the len of vsink->q_rendered_buf is 1*/
        g_mutex_lock(&vsink->rendered_buf_muxtex);
        while(!g_queue_is_empty(&vsink->q_rendered_buf)){
            GstVideoFrameMap* item = g_queue_pop_head(&vsink->q_rendered_buf);
            if(item){
                gst_buffer_unref(item->gstvbuf);
                g_free(item);
            }
        }
        g_mutex_unlock(&vsink->rendered_buf_muxtex);

    }

    return ;
}

static void gst_smevideosink_hihal_cal_disprect(SME_HAL_VDISP_PIPE_HDL hdlPipe,  E_SME_PLAYER_DISPLAY_MODE e_disp_mode,  const ST_OUT_RECT* p_win_rect,
     const ST_DISP_RATIO* p_disp_radio, ST_OUT_RECT* p_disp_win)
{
    switch(e_disp_mode){
        case E_SME_PLAYER_DIS_MODE_FULL:{
            SME_Disp_Full(p_win_rect, p_disp_win);
            break;
        }
        /*the diff of default and fitin is that the ratio of former comes from source , the later comes form UI setting,
        at this point,we deal with them by the same way
        */
        case E_SME_PLAYER_DIS_MODE_DEFAULT:
        case E_SME_PLAYER_DIS_MODE_FITIN:{
            ST_DISP_HAL_RATIO stUIratio;
            stUIratio.u32DispRatioW = p_disp_radio->u32DispRatioW;
            stUIratio.u32DispRatioH = p_disp_radio->u32DispRatioH;
            SME_Disp_Fitin(hdlPipe, &stUIratio, p_win_rect, p_disp_win);
            break;
        }
        default:
            break;
    }

}
static guint
gst_smevideosink_hihal_update_display_mode(GstSmeVideoSinkHiHal * vsink)
{
    if(vsink->e_disp_mode != vsink->e_old_disp_mode
            || vsink->st_disp_ratio.u32DispRatioW != vsink->st_old_disp_ratio.u32DispRatioW
            || vsink->st_disp_ratio.u32DispRatioH != vsink->st_old_disp_ratio.u32DispRatioH
            || vsink->st_win_rect.u32OutX != vsink->st_old_win_rect.u32OutX
            || vsink->st_win_rect.u32OutY != vsink->st_old_win_rect.u32OutY
            || vsink->st_win_rect.u32OutWidth != vsink->st_old_win_rect.u32OutWidth
            || vsink->st_win_rect.u32OutHeight != vsink->st_old_win_rect.u32OutHeight
            || vsink->first_frame_flag)
        {
            /* need to update */
            ICS_LOGD("new mode is : %u, new ratio is [%u %u], new rect is [%u %u %u %u]",
            vsink->e_disp_mode,
            vsink->st_disp_ratio.u32DispRatioW,
            vsink->st_disp_ratio.u32DispRatioH,
            vsink->st_win_rect.u32OutX,
            vsink->st_win_rect.u32OutY,
            vsink->st_win_rect.u32OutWidth,
            vsink->st_win_rect.u32OutHeight);

            ST_OUT_RECT last_disp_rect;
            HI_RECT_S stRect;
            gst_smevideosink_hihal_cal_disprect(vsink->hdl_sme_pipe_hihal, vsink->e_disp_mode, &vsink->st_win_rect,
                &vsink->st_disp_ratio, &last_disp_rect);
            stRect.s32X = (gint)last_disp_rect.u32OutX;
            stRect.s32Y = (gint)last_disp_rect.u32OutY;
            stRect.s32Width = (gint)last_disp_rect.u32OutWidth;
            stRect.s32Height = (gint)last_disp_rect.u32OutHeight;
            if( vsink->first_frame_flag){
                vsink->first_frame_flag = FALSE;
            }
            ICS_LOGD("last rect is [%d,%d,%d,%d]", stRect.s32X, stRect.s32Y, stRect.s32Width, stRect.s32Height);

            if(ICS_SUCCESS != SME_Hal_VDisp_Updata_OutRect(vsink->hdl_sme_pipe_hihal, &stRect))
            {
                GST_ERROR("Updata OutRect Failed");
                vsink->e_disp_mode = vsink->e_old_disp_mode;
                vsink->st_disp_ratio.u32DispRatioW = vsink->st_old_disp_ratio.u32DispRatioW;
                vsink->st_disp_ratio.u32DispRatioH = vsink->st_old_disp_ratio.u32DispRatioH;
                vsink->st_win_rect.u32OutX = vsink->st_old_win_rect.u32OutX;
                vsink->st_win_rect.u32OutY = vsink->st_old_win_rect.u32OutY;
                vsink->st_win_rect.u32OutWidth = vsink->st_old_win_rect.u32OutWidth;
                vsink->st_win_rect.u32OutHeight = vsink->st_old_win_rect.u32OutHeight;
                return ICS_FAIL;
            }
            else
            {
                vsink->e_old_disp_mode = vsink->e_disp_mode;
                vsink->st_old_disp_ratio.u32DispRatioW = vsink->st_disp_ratio.u32DispRatioW;
                vsink->st_old_disp_ratio.u32DispRatioH = vsink->st_disp_ratio.u32DispRatioH;
                vsink->st_old_win_rect.u32OutX = vsink->st_win_rect.u32OutX;
                vsink->st_old_win_rect.u32OutY = vsink->st_win_rect.u32OutY;
                vsink->st_old_win_rect.u32OutWidth = vsink->st_win_rect.u32OutWidth;
                vsink->st_old_win_rect.u32OutHeight = vsink->st_win_rect.u32OutHeight;
            }

        }
    return ICS_SUCCESS;
}

static GstStateChangeReturn
gst_smevideosink_hihal_change_state (GstElement * element, GstStateChange transition)
{
    GstSmeVideoSinkHiHal*   vsink   = GST_SME_VIDEOSINK_HIHAL(element);
    GstStateChangeReturn    eret     = GST_STATE_CHANGE_SUCCESS;

    GST_INFO ("gst_smevideosink_himpi_change_state in:%s to %s,",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)));

    switch (transition) {
        case GST_STATE_CHANGE_NULL_TO_READY:
            break;
        case GST_STATE_CHANGE_READY_TO_PAUSED:{
            HI_RECT_S stRect;
            vsink->u32_late_count = 0;
            vsink->u32_drop_interleave = 0;
            vsink->is_first_frame = TRUE;
            vsink->is_first_render = TRUE;
            if(vsink->pts_calibration_hdl){
              gst_sme_pts_calibration_entry_init(vsink->pts_calibration_hdl,
                E_ENTRY_TYPE_VIDEO, "vsink", element);
            }

            vsink->is_flushing = TRUE;
            vsink->hdl_sme_pipe_hihal = SME_Hal_VDisp_Create();
            if(vsink->hdl_sme_pipe_hihal == NULL){
                GST_ERROR_OBJECT(vsink, "SME_Hal_VDisp_Create failed");
                return GST_STATE_CHANGE_FAILURE;
            }

            stRect.s32X     = vsink->st_vdisp_rect.i32StartX;
            stRect.s32Y     = vsink->st_vdisp_rect.i32StartY;
            stRect.s32Width = vsink->st_vdisp_rect.i32Width;
            stRect.s32Height= vsink->st_vdisp_rect.i32Height;

            if(ICS_SUCCESS != gst_smevideosink_hihal_wr_init(vsink,
              stRect, vsink->hdl_sme_pipe_hihal, (gboolean)TRUE)){
                GST_ERROR_OBJECT(vsink, "SME_Hal_VDisp_Init failed");
                SME_Hal_VDisp_Destory(vsink->hdl_sme_pipe_hihal);
                vsink->hdl_sme_pipe_hihal = NULL;
                return GST_STATE_CHANGE_FAILURE;
            }else{
                GST_INFO_OBJECT(vsink, "SME_Hal_VDisp_Init SUCCEED");
            }

            vsink->first_frame_flag = TRUE;

#ifdef USE_DUMP_DECED_DATA
            vsink->dump_deced_data = fopen(DUMP_DECED_PATH, "ab+");
            if(vsink->dump_deced_data == NULL){
                GST_ERROR("open dump_deced_path failed");
            }
#endif
            break;
        }
        case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
            break;
        case GST_STATE_CHANGE_PAUSED_TO_READY:
            vsink->recieve_eos = FALSE;
            break;
        default:
            break;
    }

    eret = GST_ELEMENT_CLASS(parent_class)->change_state (element, transition);

    GST_INFO ("gst_smevideosink_himpi_change_state:%s to %s,end,ret=%s",
        gst_element_state_get_name((GstState)((transition >> 3) & 0x07)),
        gst_element_state_get_name((GstState)(transition & 0x07)),
        gst_element_state_change_return_get_name(eret));

    switch (transition) {
        case GST_STATE_CHANGE_READY_TO_PAUSED:{
#ifdef DEQUEUE_FRAME_ENABLE
            if(eret != GST_STATE_CHANGE_FAILURE){
              vsink->is_dequeue_thd_exit = FALSE;
              vsink->dequeue_thd = g_thread_new("vsink_dequeue_thd", DequeueThdFunc, vsink);
            }
            if(NULL == vsink->dequeue_thd){
                GST_ERROR("new thread failed");
                gst_smevideosink_hihal_wr_deinit(vsink);
                return GST_STATE_CHANGE_FAILURE;
            }
#endif
            break;
        }
        case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
            break;
        case GST_STATE_CHANGE_PAUSED_TO_READY:
            GST_INFO_OBJECT(vsink, "hihal_deinit begin, ccmode=%d", vsink->switch_channel_mode);
            gst_smevideosink_hihal_wr_deinit(vsink);
            GST_INFO_OBJECT(vsink, "hihal_deinit end");
#ifdef USE_DUMP_DECED_DATA
            if(vsink->dump_deced_data) {
              fclose(vsink->dump_deced_data);
              vsink->dump_deced_data = NULL;
            }
#endif
            if(vsink->pts_calibration_hdl){
              gst_sme_pts_calibration_entry_deinit(vsink->pts_calibration_hdl,
                E_ENTRY_TYPE_VIDEO);
            }
            break;
        case GST_STATE_CHANGE_READY_TO_NULL:
            vsink->android_surface    = NULL;
            break;
        default:
            break;
    }

    return eret;
}

static void
gst_smevideosink_hihal_calibrate_pts (GstBaseSink * bsink, GstBuffer * buffer,
  gboolean *bcalibrated)
{
  GstSmeVideoSinkHiHal*   vsink   = GST_SME_VIDEOSINK_HIHAL(bsink);
  GstClockTime in_pts, out_pts;
  GstClockTime in_duration, out_duration;
  gboolean bcalibration = FALSE;

  if(NULL != bcalibrated){
    *bcalibrated = FALSE;
  }

  g_return_if_fail(NULL != buffer);

  if(vsink->pts_calibration_hdl){
    in_pts = GST_BUFFER_PTS(buffer);
    in_duration = GST_BUFFER_DURATION(buffer);
    bcalibration = gst_sme_pts_calibration_entry_execute(vsink->pts_calibration_hdl,
      E_ENTRY_TYPE_VIDEO, in_pts, in_duration, &out_pts, &out_duration);
    GST_BUFFER_PTS(buffer) = out_pts;
    GST_BUFFER_DTS(buffer) = out_pts;
    GST_BUFFER_DURATION(buffer) = out_duration;
  }
  if(bcalibrated){
    *bcalibrated = bcalibration;
  }

  return;
}

//register
gboolean
gst_smevideosink_hihal_register (GstPlugin * plugin)
{
    GST_ERROR_OBJECT(plugin, "registering sme video sink hihal in.");

    ICS_LOGE("hal register");

    if (!gst_element_register (plugin, GST_SMEVIDEOSINK_HIHAL_NAME,
        GST_RANK_SECONDARY + ((GST_RANK_PRIMARY - GST_RANK_SECONDARY) / 2 + 1),
        GST_TYPE_SME_VIDEOSINK_HIHAL)) {

        GST_WARNING ("Register of type %s failed", GST_SMEVIDEOSINK_HIHAL_NAME);
        return FALSE;
    }

    GST_ERROR("Register of type %s succeed", GST_SMEVIDEOSINK_HIHAL_NAME);
    GST_ERROR_OBJECT(plugin, "registering sme video sink hihal out.");

    return TRUE;
}
