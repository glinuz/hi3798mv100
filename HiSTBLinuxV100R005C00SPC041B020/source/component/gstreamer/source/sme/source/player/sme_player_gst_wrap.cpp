 /**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
/ * @file    sme_player_gst_wrap.cpp
 * @brief   gstsmeplaybin封装
 * @author
 * @date    2014/4/7
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/4/7
 * Author :
 * Desc   : Created file
 *
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "gst.h"
#include "gstsme.h"
#include "sme_log_adpt.h"
#include "sme_log.h"
#include "sme_macro.h"
#include "osal_thread.h"
#include "osal_str.h"
#include "osal_mem.h"
#include "sme_timer.h"
#include "sme_media_type.h"
#include "sme_player_gst_wrap.h"


//#define M_SME_SEEK_RATE 1.0

//  GST_PLAY_FLAG_VIDEO | GST_PLAY_FLAG_AUDIO | GST_PLAY_FLAG_TEXT
// | GST_PLAY_FLAG_NATIVE_AUDIO | GST_PLAY_FLAG_NATIVE_VIDEO | GST_PLAY_FLAG_BUFFERING
#define SME_GST_PLAYBIN_FLAGS (0x167)
#ifndef __LINUX__
#define SME_GST_DVB_FLAGS (0x060)
#endif
#define SME_GST_PLAYBIN_RING_QUEUE_MAX_SIZE (5*1024*1024ULL)

#define SME_VDECOUT_INTERVAL_MAX            80//ms
#define SME_DEFAULT_CUR_BW                  100000000

//multiqueuesize
#define SME_GST_PLAYBIN_QUEUE_MAX_SIZE (100*1024*1024U)//Bytes
//#define SME_GST_PLAYBIN_QUEUE_DURATION (10*1000*1000*1000ULL)//NS
//buffered data = u64BufDuration*percent

//lint -e10   //va_list
//lint -e40   //va_list
//lint -e1013   //va_list
//lint -e601   //va_list
//lint -e1704   //Constructor 'SmePlayerGstWrPipe::SmePlayerGstWrPipe(void)' has private access specification
//lint -esym(438, pobjSmePlayerGstWrPipe, pppArgv, pszGstUri, pszGstSubUri)  //not used
//lint -esym(593, pobjSmePlayerGstWrPipe)  //will delete in DeinitGst()
//lint -esym(613, pobjSmePlayerGstWrPipe, pppArgv)  //Possible use of null pointer
//lint -e655   //bit-wise operation uses (compatible) enum's
//lint -e717 //do{}while(0)
//lint -e731   //Boolean argument to equal/not equal
//lint -e754   //message of _tag_DebugMessage is not used
//lint -e774   //Boolean within 'if' always evaluates to False
//lint -e826   //指针转换
//lint -efunc(1551, SmeInitGst::~SmeInitGst)
//lint -esym(818, Element, pvPipeline)
//lint -esym(528, g_spobjInitGst)
//lint -e1055
//lint -e732

typedef enum gst_wrap_op{
    E_OP_NONE = 0,
    E_OP_PAUSING,
    E_OP_PLAYING,
}E_GST_WRAP_OP;



#ifdef DENABLE_SUBTITLE_PATH
#include "sme_gstappsink_sub_wrap.h"
#endif

#define M_SME_PLAYBIN_NAME "playbin"

struct _AdaptiveStreamStartupMode
{
    V_UINT64 StartupBitrate;
    V_BOOL IsAdaptive;
};

struct _AdaptiveStreamBitrateInfo
{
    V_UINT64 *VideoBitrateList;
    V_UINT32 VideoBitrateNum;
    V_UINT64 *AudioBitrateList;
    V_UINT32 AudioBitrateNum;
};

typedef struct _AdaptiveStreamBitrateInfo AdaptiveBitrateInfo;
typedef struct _AdaptiveStreamStartupMode AdaptiveStartupMode;

class SmePlayerGstWrPipe
{
public:
    SmePlayerGstWrPipe(GstElement* pstSmePlaybin,
        SmePlayerGstWrap::pfnGstMsgHandler pfnMsgHandler,
        V_VOID* pvMsgCtx,
        guint u32BusWatchId)
    :m_pstSmePlaybin(pstSmePlaybin)
    ,m_pfnMsgHandler(pfnMsgHandler)
    ,m_pvMsgCtx(pvMsgCtx)
    ,m_u32BusWatchId(u32BusWatchId)
    ,m_hdlSem(NULL)
    ,m_bSyncPrepare(ICS_FALSE)
    ,m_i32SeekTo(-1)
    ,m_pstMainloop(NULL)
    ,m_hdlRunMainLoopThd(NULL)
    ,m_bExitBlock(ICS_FALSE)
    ,m_bStarting(ICS_FALSE)
    ,m_i32BufPercent(0)
    ,m_u32BufferFull(0)
#ifdef DENABLE_SUBTITLE_PATH
    ,m_pobjTextSink(NULL)
#endif
    ,m_pfnManifestCompleteCb(NULL)
    ,m_pvManifestCompleteCbCtx(NULL)
    ,m_pfnBtChangedCb(NULL)
    ,m_pvBtChangedCbCtx(NULL)
    ,m_u32Bandwidth(0)
    ,m_eGstWrapOp(E_OP_NONE)
    ,m_i32LastBufPercent(0)
    ,m_u32Error(E_SME_PLAYER_ERROR_NONE)
    {
        V_UINT32 u32Res;
        u32Res = VOS_InitThdMutex(&m_stMutex, NULL);
        UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("Init thd mutex fail."));

        if (E_SME_MEM_OK != VOS_Memset_S(
            &m_stBufferStatus,
            sizeof(ST_BUFFER_STATUS),
            0,
            sizeof(ST_BUFFER_STATUS)))
        {
            ICS_LOGE("SECURE: VOS_Memset_S return failed.");
        }
        if (E_SME_MEM_OK != VOS_Memset_S(
            &m_stAudioTrackInfo,
            sizeof(ST_AUDIO_TRACK_INFO),
            0,
            sizeof(ST_AUDIO_TRACK_INFO)))
        {
            ICS_LOGE("SECURE: VOS_Memset_S return failed.");
        }
        if (E_SME_MEM_OK != VOS_Memset_S(
            &m_stVideoTrackInfo,
            sizeof(ST_VIDEO_TRACK_INFO),
            0,
            sizeof(ST_VIDEO_TRACK_INFO)))
        {
            ICS_LOGE("SECURE: VOS_Memset_S return failed.");
        }

        m_stStartupMode.IsAdaptive = TRUE;
        m_stStartupMode.StartupBitrate = 0;

        if (E_SME_MEM_OK != VOS_Memset_S(
            &m_stCurrentBitrate,
            sizeof(ST_CURRENT_BITRATE),
            0,
            sizeof(ST_CURRENT_BITRATE)))
        {
            ICS_LOGE("SECURE: VOS_Memset_S return failed.");
        }
    }

    virtual ~SmePlayerGstWrPipe()
    {
        m_pstSmePlaybin = NULL;
        m_pvMsgCtx = NULL;
        m_pstMainloop = NULL;
        m_hdlRunMainLoopThd = NULL;
        m_hdlSem = NULL;
#ifdef DENABLE_SUBTITLE_PATH
        m_pobjTextSink = NULL;
#endif
        m_pfnManifestCompleteCb = NULL;
        m_pvManifestCompleteCbCtx = NULL;
        m_pfnBtChangedCb = NULL;
        m_pvBtChangedCbCtx = NULL;
        VOS_DeInitThdMutex(&m_stMutex);
    };

    GstElement*                         m_pstSmePlaybin;
    SmePlayerGstWrap::pfnGstMsgHandler  m_pfnMsgHandler;
    V_VOID*                             m_pvMsgCtx;
    guint                               m_u32BusWatchId;
    VOSSEMHDL                           m_hdlSem;
    V_BOOL                              m_bSyncPrepare;
    V_INT32                             m_i32SeekTo;
    GMainLoop *                         m_pstMainloop;
    VOSTHDHDL                           m_hdlRunMainLoopThd;
    V_BOOL                              m_bExitBlock;
    V_BOOL                              m_bStarting;
    ST_VOS_THD_MUTEX                    m_stMutex;
    V_INT32                             m_i32BufPercent;
    V_UINT32                            m_u32BufferFull;
#ifdef DENABLE_SUBTITLE_PATH
    SmeGstAppsinkOfSubWr*               m_pobjTextSink;
#endif
    PFN_StreamPlayModeCb                m_pfnManifestCompleteCb;
    V_VOID*                             m_pvManifestCompleteCbCtx;
    PFN_BitrateChangedCb                m_pfnBtChangedCb;
    V_VOID*                             m_pvBtChangedCbCtx;
    V_UINT32                            m_u32Bandwidth;
    ST_BUFFER_STATUS                    m_stBufferStatus;
    E_GST_WRAP_OP                       m_eGstWrapOp;
    ST_AUDIO_TRACK_INFO                 m_stAudioTrackInfo;
    ST_VIDEO_TRACK_INFO                 m_stVideoTrackInfo;
    /** BEGIN: Added for DTS2015103005728 by baihaili(b00311320), 2015/10/30 */
    AdaptiveStartupMode                 m_stStartupMode;
    /* END: Added for DTS2015103005728 by baihaili(b00311320), 2015/10/30 */
    V_INT32                             m_i32LastBufPercent;//bufpercent before seek
    ST_CURRENT_BITRATE                  m_stCurrentBitrate;
    V_UINT32                            m_u32Error;

    static V_VOID ManifestParseCompleteCb(GstElement * Element,
                gpointer Input, gpointer Result, gpointer pCtx);
private:
    SmePlayerGstWrPipe()
    :m_pstSmePlaybin(NULL)
    ,m_pfnMsgHandler(NULL)
    ,m_pvMsgCtx(NULL)
    ,m_u32BusWatchId(0)
    ,m_hdlSem(NULL)
    ,m_bSyncPrepare(ICS_FALSE)
    ,m_i32SeekTo(-1)
    ,m_pstMainloop(NULL)
    ,m_hdlRunMainLoopThd(NULL)
    ,m_bExitBlock(ICS_FALSE)
    ,m_bStarting(ICS_FALSE)
    ,m_i32BufPercent(0)
    ,m_u32BufferFull(0)
#ifdef DENABLE_SUBTITLE_PATH
    ,m_pobjTextSink(NULL)
#endif
    ,m_pfnManifestCompleteCb(NULL)
    ,m_pvManifestCompleteCbCtx(NULL)
    ,m_pfnBtChangedCb(NULL)
    ,m_pvBtChangedCbCtx(NULL)
    ,m_u32Bandwidth(0)
    ,m_eGstWrapOp(E_OP_NONE)
    ,m_i32LastBufPercent(0)
    ,m_u32Error(E_SME_PLAYER_ERROR_NONE)
    {
        V_UINT32 u32Res;
        u32Res = VOS_InitThdMutex(&m_stMutex, NULL);
        UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("Init thd mutex fail."));

        if (E_SME_MEM_OK != VOS_Memset_S(
            &m_stBufferStatus,
            sizeof(ST_BUFFER_STATUS),
            0,
            sizeof(ST_BUFFER_STATUS)))
        {
            ICS_LOGE("SECURE: VOS_Memset_S return failed.");
        }
        if (E_SME_MEM_OK != VOS_Memset_S(
            &m_stAudioTrackInfo,
            sizeof(ST_AUDIO_TRACK_INFO),
            0,
            sizeof(ST_AUDIO_TRACK_INFO)))
        {
            ICS_LOGE("SECURE: VOS_Memset_S return failed.");
        }
        if (E_SME_MEM_OK != VOS_Memset_S(
            &m_stVideoTrackInfo,
            sizeof(ST_VIDEO_TRACK_INFO),
            0,
            sizeof(ST_VIDEO_TRACK_INFO)))
        {
            ICS_LOGE("SECURE: VOS_Memset_S return failed.");
        }

        /** BEGIN: Added for DTS2015103005728 by baihaili(b00311320), 2015/10/30 */
        m_stStartupMode.IsAdaptive = TRUE;
        m_stStartupMode.StartupBitrate = 0;
        /* END: Added for DTS2015103005728 by baihaili(b00311320), 2015/10/30 */

        if (E_SME_MEM_OK != VOS_Memset_S(
            &m_stCurrentBitrate,
            sizeof(ST_CURRENT_BITRATE),
            0,
            sizeof(ST_CURRENT_BITRATE)))
        {
            ICS_LOGE("SECURE: VOS_Memset_S return failed.");
        }
    }
};

/*static*/
V_VOID SmePlayerGstWrPipe::ManifestParseCompleteCb(GstElement * Element,
    gpointer Input, gpointer Result, gpointer pCtx)
{
    AdaptiveBitrateInfo* pstBitrateInfo = (AdaptiveBitrateInfo*) Input;
    SmePlayerGstWrPipe* pWrap = static_cast<SmePlayerGstWrPipe*>(pCtx);

    UTILS_M_INVALID_ARGS(Element);

    UTILS_MLOGE_RET_IF((NULL == pstBitrateInfo) || (NULL == Result), ("null pointer"));

    ICS_LOGI("Video num :%d,Audio num:%d",pstBitrateInfo->VideoBitrateNum,pstBitrateInfo->AudioBitrateNum);

    for (guint i = 0; i < pstBitrateInfo->VideoBitrateNum;i++)
    {
        ICS_LOGI("Video[%d] bitrate:[%llu]",i,pstBitrateInfo->VideoBitrateList[i]);
    }
    for (guint i = 0; i < pstBitrateInfo->AudioBitrateNum;i++)
    {
        ICS_LOGI("Audio[%d] bitrate:[%llu]",i,pstBitrateInfo->AudioBitrateList[i]);
    }
    /** BEGIN: Added for DTS2015103005728 by baihaili(b00311320), 2015/10/30 */
    if (pWrap->m_pfnManifestCompleteCb)
    {
        pWrap->m_pfnManifestCompleteCb(pWrap->m_pvManifestCompleteCbCtx,
                                       pstBitrateInfo->VideoBitrateList,
                                       pstBitrateInfo->VideoBitrateNum,
                                       &(pWrap->m_stStartupMode.IsAdaptive),
                                       &(pWrap->m_stStartupMode.StartupBitrate));
        pWrap->m_pfnManifestCompleteCb = NULL;
    }

    ICS_LOGW("Startup configuration: isAdaptive:%d, StartupBitrate:%llu",
             pWrap->m_stStartupMode.IsAdaptive,
             pWrap->m_stStartupMode.StartupBitrate);
    ((AdaptiveStartupMode *)Result)->StartupBitrate =
        pWrap->m_stStartupMode.StartupBitrate;
    ((AdaptiveStartupMode *)Result)->IsAdaptive =
        pWrap->m_stStartupMode.IsAdaptive;
    /* END: Added for DTS2015103005728 by baihaili(b00311320), 2015/10/9 */
    return;
}


static ST_VOS_THD_MUTEX g_sstLogMutex = VOS_PTHREAD_MUTEX_INITIALIZER;


static gboolean bus_call(GstBus * bus, GstMessage * msg, gpointer data)
{
    SmePlayerGstWrPipe* pobjThis = static_cast<SmePlayerGstWrPipe*>(data);

    UTILS_M_INVALID_ARGS(bus);

    GstObject *src_obj = GST_MESSAGE_SRC (msg);
    guint32 seqnum = gst_message_get_seqnum (msg);

    if (GST_IS_ELEMENT (src_obj)) {
        ICS_LOGI("bus_call, Got message #%u from element \"%s\" (%s): ",
            (guint) seqnum, GST_ELEMENT_NAME (src_obj),
            GST_MESSAGE_TYPE_NAME(msg));
    } else if (GST_IS_PAD (src_obj)) {
        ICS_LOGI("bus_call, Got message #%u from pad \"%s:%s\" (%s): ",
            (guint) seqnum, GST_DEBUG_PAD_NAME (src_obj),
            GST_MESSAGE_TYPE_NAME(msg));
    } else if (GST_IS_OBJECT (src_obj)) {
        ICS_LOGI("bus_call, Got message #%u from object \"%s\" (%s): ",
            (guint) seqnum, GST_OBJECT_NAME (src_obj),
            GST_MESSAGE_TYPE_NAME(msg));
    } else {
        ICS_LOGI("bus_call, Got message #%u (%s): ",
            (guint) seqnum,
            GST_MESSAGE_TYPE_NAME(msg));
    }

    //ICS_LOGI("bus_call, msg=%#x, typename:%s", GST_MESSAGE_TYPE (msg),
    //    GST_MESSAGE_TYPE_NAME(msg));

    switch (GST_MESSAGE_TYPE (msg))
    {
        /* BEGIN: Added for DTS2015071706676 by liurongliang(l00180035), 2015/7/22 */
        case GST_MESSAGE_ASYNC_DONE:
        {
            ICS_LOGI("bus_call,ASYNC_DONE, msg=%#x, typename:%s,srcname=%s",
                GST_MESSAGE_TYPE (msg), GST_MESSAGE_TYPE_NAME(msg),
                GST_MESSAGE_SRC_NAME(msg));
            if(GST_ELEMENT(GST_MESSAGE_SRC(msg)) == pobjThis->m_pstSmePlaybin)
            {
                VOS_ThdMutexLock(&pobjThis->m_stMutex);
                if((-1 != pobjThis->m_i32SeekTo) && (E_OP_PAUSING == pobjThis->m_eGstWrapOp))
                {
                    V_INT32 i32SeekTo = pobjThis->m_i32SeekTo;

                    ICS_LOGI("ASYNC_DONE, after seeking!");
                    pobjThis->m_i32SeekTo = -1;
                    VOS_ThdMutexUnLock(&pobjThis->m_stMutex);
                    pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                        E_SME_PLAYER_NOTIFY_SEEK_COMPLETED, (V_UINT32)i32SeekTo, 0);
                }
                else
                {
                    ICS_LOGI("ASYNC_DONE, no seeking!");
                    VOS_ThdMutexUnLock(&pobjThis->m_stMutex);
                }
            }
            break;
        }
        /* END: Added for DTS2015071706676 by liurongliang(l00180035), 2015/7/22 */
        case GST_MESSAGE_STATE_CHANGED:
        {
            GstState eOld = GST_STATE_VOID_PENDING;
            GstState eCur = GST_STATE_VOID_PENDING;
            GstState ePending = GST_STATE_VOID_PENDING;

            VOS_ThdMutexLock(&pobjThis->m_stMutex);

            gst_message_parse_state_changed (msg, &eOld, &eCur, &ePending);

            ICS_LOGI("bus_call state_changed, msg=%#x, typename:%s,srcname=%s, "
                "old=%d, cur=%d, pending=%d", GST_MESSAGE_TYPE (msg),
                GST_MESSAGE_TYPE_NAME(msg), GST_MESSAGE_SRC_NAME(msg),
                eOld, eCur, ePending);

            if((GST_STATE_PAUSED == eCur)
                && (GST_ELEMENT(GST_MESSAGE_SRC(msg)) == pobjThis->m_pstSmePlaybin))
            {
#ifdef DENABLE_SUBTITLE_PATH
                if(pobjThis->m_pobjTextSink)
                {
                    pobjThis->m_pobjTextSink->BeginPause();
                }
#endif
                GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pobjThis->m_pstSmePlaybin),
                    GST_DEBUG_GRAPH_SHOW_ALL, "gst-launch.pause");
                VOS_ThdMutexUnLock(&pobjThis->m_stMutex);
                ICS_LOGI("PAUSED");
                if(pobjThis->m_bSyncPrepare)
                {
                    VOS_ThdSemPost(pobjThis->m_hdlSem);
                }

                pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                    E_SME_PLAYER_NOTIFY_PREPARED, 0, 0);
            }
            /** BEGIN: Added for DTS2014061603916, 2014/6/24 */
            else if((GST_STATE_PLAYING == eCur)
                && (GST_ELEMENT(GST_MESSAGE_SRC(msg)) == pobjThis->m_pstSmePlaybin))
            {
#ifdef DENABLE_SUBTITLE_PATH
                if(pobjThis->m_pobjTextSink)
                {
                    pobjThis->m_pobjTextSink->EndPause();
                }
#endif
                GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pobjThis->m_pstSmePlaybin),
                    GST_DEBUG_GRAPH_SHOW_ALL, "gst-launch.playing");
                if(pobjThis->m_bStarting)
                {
                    ICS_LOGI("PLAYING, after start!");
                    pobjThis->m_bStarting = ICS_FALSE;
                    VOS_ThdSemPost(pobjThis->m_hdlSem);
                }
                if(-1 != pobjThis->m_i32SeekTo)
                {
                    V_INT32 i32SeekTo = pobjThis->m_i32SeekTo;

                    ICS_LOGI("PLAYING, after seeking!");
                    pobjThis->m_i32SeekTo = -1;
                    VOS_ThdMutexUnLock(&pobjThis->m_stMutex);
                    pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                        E_SME_PLAYER_NOTIFY_SEEK_COMPLETED, (V_UINT32)i32SeekTo, 0);
                }
                else
                {
                    ICS_LOGI("PLAYING, no seeking!");
                    VOS_ThdMutexUnLock(&pobjThis->m_stMutex);
                }
            }
            /** END: Added for DTS2014061603916, 2014/6/24 */
            else
            {
                VOS_ThdMutexUnLock(&pobjThis->m_stMutex);
            }
            break;
        }
        case GST_MESSAGE_EOS:
        {
            ICS_LOGI("stream eos!");
            VOS_ThdMutexLock(&pobjThis->m_stMutex);
            if(pobjThis->m_bStarting)
            {
                ICS_LOGI("PLAYING, after start!");
                pobjThis->m_bStarting = ICS_FALSE;
                VOS_ThdSemPost(pobjThis->m_hdlSem);
            }
            else if(-1 != pobjThis->m_i32SeekTo)
            {
                ICS_LOGI("seeking!");
                pobjThis->m_i32SeekTo = -1;
            }
            VOS_ThdMutexUnLock(&pobjThis->m_stMutex);

            pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                E_SME_PLAYER_NOTIFY_PLAYBACK_COMPLETED, 0, 0);
            break;
        }
        /** BEGIN: Modified for DTS2014081202737 by liurongliang(l00180035), 2014/8/12 */
        case GST_MESSAGE_ERROR:
        {
            gchar *debug;
            GError *err;
            E_SME_PLAYER_ERROR eSmePlayerError = E_SME_PLAYER_ERROR_UNKOWN;
            gst_message_parse_error (msg, &err, &debug);
#ifdef SME_RLS
            ICS_LOGE("Debugging info,Error:%s", err->message);
#else
            ICS_LOGE("Debugging info:%s,Error:%s", (debug) ? debug : "none",
                err->message);
#endif
            GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pobjThis->m_pstSmePlaybin),
                GST_DEBUG_GRAPH_SHOW_ALL, "gst-launch.error");

            ICS_LOGE ("err.domain:%d, err.code:%d", err->domain, err->code);
            /* begin: DTS2014062402283  2014.06.23 */
            /* begin: DTS2014061804621/DTS2014061804815 2014.06.23 */
            if (err->domain == GST_STREAM_ERROR)
            {
                ICS_LOGE("domain:GST_STREAM_ERROR");
                if (err->code == GST_STREAM_ERROR_FORMAT)
                {
                    ICS_LOGE("code:GST_STREAM_ERROR_FORMAT");
                    eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_FORMAT;
                }
                else if (err->code == GST_STREAM_ERROR_TYPE_NOT_FOUND)
                {
                    // got from typefind
                    ICS_LOGE("code:GST_STREAM_ERROR_TYPE_NOT_FOUND");
                    eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_FORMAT;
                }
                else if (err->code == GST_STREAM_ERROR_DECODE)
                {
                    ICS_LOGE("code:GST_STREAM_ERROR_DECODE");
                    /* BEGIN: Modified for DTS2014102405113 by liurongliang(l00180035), 2014/10/25 */
                    if(debug == NULL)
                     {
                         eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_FORMAT;
                     }
                     else if(strstr(debug, GST_SME_STRAM_TAG_ADEC))
                     {
                         eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_AUDIO_CODEC;
                     }
                     else if(strstr(debug, GST_SME_STRAM_TAG_VDEC))
                     {
                         eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_VIDEO_CODEC;
                     }
                     else
                     {
                         eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_FORMAT;
                     }
                     /* END:   Modified for DTS2014102405113 by liurongliang(l00180035), 2014/10/25 */
                }
                else if(err->code == GST_STREAM_ERROR_CODEC_NOT_FOUND)
                {
                    if(debug == NULL)
                    {
                        eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_FORMAT;
                    }
                    else if(strstr(debug, GST_SME_STRAM_TAG_ADEC))
                    {
                        eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_AUDIO_CODEC;
                    }
                    else if(strstr(debug, GST_SME_STRAM_TAG_VDEC))
                    {
                        eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_VIDEO_CODEC;
                    }
                    else
                    {
                        eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_FORMAT;
                    }
                }
                else
                {
                    eSmePlayerError = E_SME_PLAYER_ERROR_UNKOWN;
                }
            }
            /* end: DTS2014061804621/DTS2014061804815 2014.06.23 */
            else if (err->domain == GST_RESOURCE_ERROR)
            {
                ICS_LOGE("domain:GST_RESOURCE_ERROR");
                if (err->code == GST_RESOURCE_ERROR_NOT_FOUND)
                {
                    ICS_LOGE("code:GST_RESOURCE_ERROR_NOT_FOUND");
                    eSmePlayerError = E_SME_PLAYER_ERROR_TIME_OUT;
                }
                else if (err->code == GST_RESOURCE_ERROR_OPEN_READ)
                {
                    ICS_LOGE("code:GST_RESOURCE_ERROR_OPEN_READ");
                    eSmePlayerError = E_SME_PLAYER_ERROR_TIME_OUT;
                }
                else
                {
                    eSmePlayerError = E_SME_PLAYER_ERROR_UNKOWN;
                }
            }
            else if (err->domain == GST_LIBRARY_ERROR)
            {
                ICS_LOGE("domain:GST_LIBRARY_ERROR");
                eSmePlayerError = E_SME_PLAYER_ERROR_UNKOWN;
            }
            else if (err->domain == GST_CORE_ERROR)
            {
                ICS_LOGE("domain:GST_CORE_ERROR");
                eSmePlayerError = E_SME_PLAYER_ERROR_UNKOWN;
            }
            else
            {
                ICS_LOGE("unknown error domain");
                eSmePlayerError = E_SME_PLAYER_ERROR_UNKOWN;
            }
            /* end: DTS2014062402283  2014.06.23 */

            g_free (debug);
            g_error_free (err);

            VOS_ThdMutexLock(&pobjThis->m_stMutex);
            if(pobjThis->m_bStarting)
            {
                ICS_LOGI("PLAYING, after start!");
                pobjThis->m_bStarting = ICS_FALSE;
                VOS_ThdSemPost(pobjThis->m_hdlSem);
            }
            else if(-1 != pobjThis->m_i32SeekTo)
            {
                ICS_LOGI("seeking!");
                pobjThis->m_i32SeekTo = -1;
            }
            VOS_ThdMutexUnLock(&pobjThis->m_stMutex);
            //if error when sync preparing, here need post sem to let prepare func go!
            pobjThis->m_u32Error = (V_UINT32)eSmePlayerError;
            if(pobjThis->m_bSyncPrepare
                && E_OP_PAUSING == pobjThis->m_eGstWrapOp){
                VOS_ThdSemPost(pobjThis->m_hdlSem);
            }

            pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                E_SME_PLAYER_NOTIFY_ERROR, eSmePlayerError, 0);
            break;
        }
        /** END:   Modified for DTS2014081202737 by liurongliang(l00180035), 2014/8/12 */
        case GST_MESSAGE_STREAM_STATUS:
        {
            GstStreamStatusType eType =  GST_STREAM_STATUS_TYPE_CREATE;
            GstElement* pEle = NULL;

            gst_message_parse_stream_status (msg, &eType, &pEle);
            ICS_LOGD("push event, msg=%#x, typename:%s,srcname=%s, "
                "eType=%d, pEleOwner=%s", GST_MESSAGE_TYPE (msg),
                GST_MESSAGE_TYPE_NAME(msg), GST_MESSAGE_SRC_NAME(msg),
                eType, GST_ELEMENT_NAME(pEle));

            break;
        }
        case GST_MESSAGE_BUFFERING:
        {
            gint percent = 0;
            gst_message_parse_buffering (msg, &percent);
            VOS_ThdMutexLock(&pobjThis->m_stMutex);
            pobjThis->m_i32BufPercent = percent;
            pobjThis->m_i32LastBufPercent = pobjThis->m_i32BufPercent;
            VOS_ThdMutexUnLock(&pobjThis->m_stMutex);
            //ICS_LOGW("buffering percent=%d,msgsrc=%s", percent, GST_MESSAGE_SRC_NAME(msg));
            break;
        }
        case GST_MESSAGE_WARNING:
        {
            gchar *debug = NULL;
            GError *err = NULL;
            gst_message_parse_warning(msg, &err, &debug);
#ifdef SME_RLS
            ICS_LOGW("Debugging info,Error:%s", err->message);
#else
            ICS_LOGW("Debugging info:%s,Error:%s", (debug) ? debug : "none",
                err->message);
#endif
            ICS_LOGW ("err.domain:%d, err.code:%d", err->domain, err->code);
            if (err->domain == GST_STREAM_ERROR)
            {
                ICS_LOGW("domain:GST_STREAM_ERROR");
                if (GST_STREAM_ERROR_SEEK == err->code)
                {
                    ICS_LOGW("code:GST_STREAM_ERROR_SEEK");
                    VOS_ThdMutexLock(&pobjThis->m_stMutex);
                    if(-1 != pobjThis->m_i32SeekTo)
                    {
                        V_INT32 i32SeekTo = pobjThis->m_i32SeekTo;

                        ICS_LOGW("seeking failed, SeekTo=%d ms,lastpercent=%d",
                            i32SeekTo, pobjThis->m_i32LastBufPercent);
                        pobjThis->m_i32SeekTo = -1;
                        //if failed we use pobjThis->m_i32LastBufPercent.
                        pobjThis->m_i32BufPercent = pobjThis->m_i32LastBufPercent;
                        VOS_ThdMutexUnLock(&pobjThis->m_stMutex);
                        pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                            E_SME_PLAYER_NOTIFY_SEEK_COMPLETED, (V_UINT32)i32SeekTo,
                            (V_UINT32)-1);
                    }
                    else
                    {
                        ICS_LOGW("no seeking ?");
                        VOS_ThdMutexUnLock(&pobjThis->m_stMutex);
                    }
                }
                else if(GST_STREAM_ERROR_SUBTILE == err->code)
                {
                    ICS_LOGW("subtile error,msg=%s", err->message?err->message:"unknown!");
                    pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                        E_SME_PLAYER_NOTIFY_ERROR,
                        E_SME_PLAYER_ERROR_UNSUPPORT_TEXT_CODEC,
                        0);
                }
                else if (GST_STREAM_ERROR_WRONG_TYPE == err->code)
                {
                    if(err->message
                        && strstr(err->message, "This appears to be a text file"))
                    {
                        ICS_LOGW("subtile error,msg=%s", err->message);
                        pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                            E_SME_PLAYER_NOTIFY_ERROR,
                            E_SME_PLAYER_ERROR_UNSUPPORT_TEXT_CODEC,
                            0);
                    }
                }
                else if(GST_STREAM_ERROR_CODEC_NOT_FOUND == err->code)
                {
                    /** BEGIN: Added for DTS2015082905391  by baihaili(b00311320), 2015/8/29 */
                    E_SME_PLAYER_ERROR eSmePlayerError = E_SME_PLAYER_ERROR_NONE;
                    if(err->message
                        && (strstr(err->message, "text/")
                        || (strstr(err->message, "application/x-ssa,"))))
                    {
                        ICS_LOGW("subtile error,msg=%s", err->message);
                        eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_TEXT_CODEC;
                    }
                    else if (err->message
                             && (strstr(err->message, "video/")))
                    {
                        ICS_LOGW("video error,msg=%s", err->message);
                        eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_VIDEO_CODEC;
                    }
                    else if(err->message
                            && (strstr(err->message, "audio/")))
                    {
                        ICS_LOGW("audio error,msg=%s", err->message);
                        eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_AUDIO_CODEC;

                    }
                    else
                    {
                        //other do not care.
                    }

                    if(E_SME_PLAYER_ERROR_NONE != eSmePlayerError)
                    {
                        pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                            E_SME_PLAYER_NOTIFY_ERROR,
                            eSmePlayerError,
                            0);
                    }
                    /* END: DTS2015082905391  Added by baihaili(b00311320), 2015/8/29 */
                }
                else if (GST_STREAM_ERROR_DECODE == err->code)
                {
                    ICS_LOGW("code:GST_STREAM_ERROR_DECODE");
                    E_SME_PLAYER_ERROR eSmePlayerError = E_SME_PLAYER_ERROR_NONE;
                    if(debug == NULL)
                     {
                         eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_FORMAT;
                     }
                     else if(strstr(debug, GST_SME_STRAM_TAG_ADEC))
                     {
                         eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_AUDIO_CODEC;
                     }
                     else if(strstr(debug, GST_SME_STRAM_TAG_VDEC))
                     {
                         eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_VIDEO_CODEC;
                     }
                     else
                     {
                         //other do not care.
                     }

                     if(E_SME_PLAYER_ERROR_NONE != eSmePlayerError)
                     {
                         pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                             E_SME_PLAYER_NOTIFY_ERROR,
                             eSmePlayerError,
                             0);
                     }
                }
                /* BEGIN: Added for DTS2015102208148 by liurongliang(l00180035), 2015/10/23 */
                else if (GST_STREAM_ERROR_TYPE_NOT_FOUND == err->code)
                {
                    E_SME_PLAYER_ERROR eSmePlayerError = E_SME_PLAYER_ERROR_NONE;

                    ICS_LOGW("GST_STREAM_ERROR_TYPE_NOT_FOUND !");
                    if(debug && strstr(debug, "subtitle/"))
                    {
                        ICS_LOGW("subtitle GST_STREAM_ERROR_TYPE_NOT_FOUND");
                        eSmePlayerError = E_SME_PLAYER_ERROR_UNSUPPORT_TEXT_CODEC;
                    }

                    if(E_SME_PLAYER_ERROR_NONE != eSmePlayerError)
                    {
                        pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                            E_SME_PLAYER_NOTIFY_ERROR, eSmePlayerError, 0);
                    }

                }
                /* END:   Added for DTS2015102208148 by liurongliang(l00180035), 2015/10/23 */
            }
            /* BEGIN: Added for DTS2015090203423 by liurongliang(l00180035), 2015/9/2 */
            else if (err->domain == GST_RESOURCE_ERROR)
            {
                E_SME_PLAYER_ERROR eSmePlayerError = E_SME_PLAYER_ERROR_NONE;

                ICS_LOGW("domain:GST_RESOURCE_ERROR");
                /* BEGIN: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
                if (GST_RESOURCE_ERROR_NO_DATA == err->code)
                {
                    pobjThis->m_u32Bandwidth = 0;
                }
                /* END: Added for DTS2015100805194 by baihaili(b00311320), 2015/10/9 */
                if(debug && strstr(debug, "subtitle/"))
                {
                    if (err->code == GST_RESOURCE_ERROR_NOT_FOUND)
                    {
                        ICS_LOGW("code:GST_RESOURCE_ERROR_NOT_FOUND");
                        eSmePlayerError = E_SME_PLAYER_ERROR_SUBTILE_NOT_FOUND;
                    }
                    else
                    {
                        ICS_LOGW("code:GST_RESOURCE_ERROR_OPEN_READ");
                        eSmePlayerError = E_SME_PLAYER_ERROR_SUBTILE_ACCESS;
                    }
                }

                if(E_SME_PLAYER_ERROR_NONE != eSmePlayerError)
                {
                    pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                        E_SME_PLAYER_NOTIFY_ERROR, eSmePlayerError, 0);
                }
            }
            /* END:   Added for DTS2015090203423 by liurongliang(l00180035), 2015/9/2 */
            /* BEGIN: Added for  by baihaili(b00311320), 2015/11/3 */
            g_error_free(err);
            g_free(debug);
            /* END: Added for  by baihaili(b00311320), 2015/11/3 */
            break;
        }
        case GST_MESSAGE_NEW_BANDWIDTH:
        {
            guint bandwidth = 0;
            gst_message_parse_new_bandwidth(msg, &bandwidth);
            pobjThis->m_u32Bandwidth = bandwidth;
            ICS_LOGD("bandwidth=%d,msgsrc=%s", bandwidth, GST_MESSAGE_SRC_NAME(msg));
            break;
        }
        case GST_MESSAGE_NEW_HTTP_ERR:
        {
            guint statuscode = 0;
            gst_message_parse_new_http_err(msg, &statuscode);
            ICS_LOGI ("HTTP error happened ,status code: %u , msgsrc is %s",
                statuscode , GST_MESSAGE_SRC_NAME(msg));
            pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                E_SME_PLAYER_NOTIFY_HTTP_ERR,
                (V_UINT32)statuscode, 0);
            break;
        }
        case GST_MESSAGE_TAG:
        {
            guint bitrate = 0;
            GstTagList *taglist = NULL;
            GstElement *audio_sink = NULL;
            GstElement *video_sink = NULL;

            g_object_get( pobjThis->m_pstSmePlaybin,
                GST_SME_PROP_KEY_AUDIO_SINK, &audio_sink, NULL);

            g_object_get( pobjThis->m_pstSmePlaybin,
                GST_SME_PROP_KEY_VIDEO_SINK, &video_sink, NULL);

            gst_message_parse_tag (msg, &taglist);

            if (gst_tag_list_get_uint (taglist, GST_TAG_BITRATE, &bitrate))
            {
                if ((GstObject*)audio_sink == GST_MESSAGE_SRC(msg)
                    && pobjThis->m_pfnBtChangedCb)
                {
                    pobjThis->m_pfnBtChangedCb(pobjThis->m_pvBtChangedCbCtx, 0LL,
                        (V_UINT64)bitrate);
                    pobjThis->m_stCurrentBitrate.audio = bitrate;
                }
                if ((GstObject*)video_sink == GST_MESSAGE_SRC(msg) &&
                    pobjThis->m_pfnBtChangedCb)
                {
                    pobjThis->m_pfnBtChangedCb(pobjThis->m_pvBtChangedCbCtx,
                        (V_UINT64)bitrate, 0ULL);
                    pobjThis->m_stCurrentBitrate.video = bitrate;
                }
            }
            ICS_LOGD("tag bitrate=%d,msgsrc=%s", bitrate, GST_MESSAGE_SRC_NAME(msg));

            if (taglist)
                gst_tag_list_unref(taglist);
            /* BEGIN: Added by liurongliang(l00180035), 2015/11/6 */
            if (audio_sink)
                gst_object_unref(audio_sink);
            if (video_sink)
                gst_object_unref(video_sink);
            /* END:   Added by liurongliang(l00180035), 2015/11/6 */

            break;
        }
        case GST_MESSAGE_RESOLUTION_CHANGED:
        {
            gulong width = 0;
            gulong height = 0;
            gst_message_parse_resolution_changed(msg, &width, &height);
            ICS_LOGI("the resolution has changed: %d * %d, msgsrc is %s", width, height, GST_MESSAGE_SRC_NAME(msg));
            pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                E_SME_PLAYER_NOTIFY_VIDEO_SIZE_CHANGED,
                (V_UINT32)width, (V_UINT32)height);
            break;
        }
        case GST_MESSAGE_ELEMENT:
        {
             const GstStructure *s = gst_message_get_structure (msg);

             if (gst_structure_get_name (s) && !strcmp (gst_structure_get_name (s),
                "adaptive-streaming-statistics"))
             {
                 ST_FRAGMENT_INFO  stFragmentInfo;
                 const GValue *value;
                 const gchar* pStrUri;
                 guint str_len = 0;

                if (E_SME_MEM_OK != VOS_Memset_S(
                    &stFragmentInfo,
                    sizeof(ST_FRAGMENT_INFO),
                    0,
                    sizeof(ST_FRAGMENT_INFO)))
                {
                    ICS_LOGE("SECURE: VOS_Memset_S return failed.");
                }

                 pStrUri = gst_structure_get_string (s, "manifest-uri");
                 if(pStrUri)
                 {
                     gchar *tmp;
                     str_len = strlen(pStrUri);
                     tmp = g_utf8_strrchr(pStrUri, (gint)str_len, '/');
                     if (tmp) {
                         tmp++;//skip '/'
                         str_len = strlen(tmp);

                         if (E_VOS_STR_ENONE != VOS_Strncpy_S(
                            stFragmentInfo.i8ManifestUri,
                            URI_MAX_LENGTH,
                            tmp,
                            MIN(URI_MAX_LENGTH - 1, (gint)str_len)))
                        {
                            ICS_LOGE("SECURE: VOS_Strncpy_S return failed.");
                        }
                     }
                 }
                 pStrUri = gst_structure_get_string (s, "uri");
                 if(pStrUri)
                 {
                     gchar *tmp;
                     str_len = strlen(pStrUri);
                     tmp = g_utf8_strrchr(pStrUri, (gint)str_len, '/');
                     if (tmp) {
                         tmp++;
                         str_len = strlen(tmp);

                         if (E_VOS_STR_ENONE != VOS_Strncpy_S(
                            stFragmentInfo.i8FragmentUri,
                            URI_MAX_LENGTH,
                            tmp,
                            MIN(URI_MAX_LENGTH - 1, (gint)str_len)))
                        {
                            ICS_LOGE("SECURE: VOS_Strncpy_S return failed.");
                        }
                     }
                 }
                 value = gst_structure_get_value(s, "fragment-size");
                 if (value != NULL)
                 {
                     stFragmentInfo.u64FragmentSize = g_value_get_uint64(value);
                 }

                 gst_structure_get_clock_time(s,
                     "fragment-start-time",
                     &(stFragmentInfo.u64DownloadStartTime));

                 gst_structure_get_clock_time(s,
                     "fragment-stop-time",
                     &(stFragmentInfo.u64DownloadEndTime));

                 gst_structure_get_clock_time(s,
                     "download-total-time",
                     &(stFragmentInfo.u64DownloadTotalTime));

                 /* BEGIN: Modified for DTS2015102210493 by liurongliang(l00180035), 2015/10/23 */
                 ICS_LOGI("notify fragment info:"
                          "size:%d, start:%llu,end:%llu, download time:%llu",
                          stFragmentInfo.u64FragmentSize,
                          stFragmentInfo.u64DownloadStartTime,
                          stFragmentInfo.u64DownloadEndTime,
                          stFragmentInfo.u64DownloadTotalTime);
                 /* END: Modified for DTS2015102210493 by liurongliang(l00180035), 2015/10/23 */
                 pobjThis->m_pfnMsgHandler(pobjThis->m_pvMsgCtx,
                     E_SME_PLAYER_NOTIFY_FRAGMENT_UPDATE,
                     (V_UINT32)&stFragmentInfo,
                     0);
             }else if (gst_structure_get_name (s) && !strcmp (gst_structure_get_name (s),
                "multiqueue-buffer-statistics")) {
                 const GValue *value;
                 const gchar* pStrUri;
                 ST_SINGLE_QUEUE_STATUS stQueueStatus;

                if (E_SME_MEM_OK != VOS_Memset_S(
                    &stQueueStatus,
                    sizeof(ST_SINGLE_QUEUE_STATUS),
                    0,
                    sizeof(ST_SINGLE_QUEUE_STATUS)))
                {
                    ICS_LOGE("SECURE: VOS_Memset_S return failed.");
                }

                 pStrUri = gst_structure_get_string (s, "queue-type");
                 if (!pStrUri) {
                     break;
                 }
                 value = gst_structure_get_value(s, "buffer-size");
                 if (value != NULL)
                 {
                     stQueueStatus.u32BufferSize = g_value_get_uint(value);
                 }

                 gst_structure_get_clock_time(s, "buffer-play-time",
                                              &(stQueueStatus.u64BufferPlayingTime));
                 gst_structure_get_clock_time(s, "buffer-last-pts",
                                              &(stQueueStatus.u64LastBufferPTS));
                 if (!strncmp(pStrUri, "video",strlen("video"))) {
                     if (E_SME_MEM_OK != VOS_Memcpy_S(&(pobjThis->m_stBufferStatus.stVBufferStatus),
                                sizeof(ST_SINGLE_QUEUE_STATUS),
                                &stQueueStatus,
                                sizeof(ST_SINGLE_QUEUE_STATUS)))
                     {
                         ICS_LOGE("SECURE: VOS_Memcpy_S return failed.");
                     }
                 }
                 else if(!strncmp(pStrUri, "audio",strlen("audio"))){
                     if (E_SME_MEM_OK != VOS_Memcpy_S(
                                &(pobjThis->m_stBufferStatus.stABufferStatus),
                                sizeof(ST_SINGLE_QUEUE_STATUS),
                                &stQueueStatus,
                                sizeof(ST_SINGLE_QUEUE_STATUS)))
                     {
                         ICS_LOGE("SECURE: VOS_Memcpy_S return failed.");
                     }
                 }
             } else if (!strcmp(gst_structure_get_name(s),
                         "audio-track-info")) {
                 const gchar *audio_type, *lang;
                 guint trackid, bitrate, samplerate, channel_num;
                 guint trackidx, str_len;

                 ST_SINGLE_AUDIO_TRACK_INFO *pSingleAudioTrackInfo;
                 ST_AUDIO_TRACK_INFO *pAudioTrackInfo;

                 pAudioTrackInfo = &pobjThis->m_stAudioTrackInfo;

                 if (pAudioTrackInfo->u32UnSupportedTrackNum >= MAX_AUDIO_TRACK) {
                     ICS_LOGE("Exceeds the maximum audio track number");
                     break;
                 }

                 trackidx = pAudioTrackInfo->u32UnSupportedTrackNum++;
                 pSingleAudioTrackInfo =
                     &pAudioTrackInfo->stUnSprtSingleAudioTrackInfo[trackidx];

                 if (!gst_structure_get_uint(s, "audio-track-id", &trackid)) {
                     ICS_LOGI("fail to get audio-track-id");
                 }
                 audio_type = gst_structure_get_string(s, "audio-type");
                 if (!audio_type) {
                     ICS_LOGI("fail to get audio-type");
                 }

                 if (!gst_structure_get_uint(s, "audio-bitrate", &bitrate)) {
                     ICS_LOGI("fail to get audio-bitrate");
                 }

                 if (!gst_structure_get_uint(s, "audio-sample-rate",
                                             &samplerate)) {
                     ICS_LOGI("fail to get audio-sample-rate");
                 }

                 if (!gst_structure_get_uint(s, "audio-channels",
                                             &channel_num)) {
                     ICS_LOGI("fail to get audio-channels");
                 }
                 lang = gst_structure_get_string(s, "audio-lang");
                 if (!lang) {
                     ICS_LOGI("fail to get audio-lang");
                 }

                 ICS_LOGI("audio-track-info: "
                          "audio-track-id=%d, audio-type=%s, audio-bitrate=%d, "
                          "audio-sample-rate=%d, audio-channels=%d, audio-lang=%s",
                          trackid,
                          audio_type,
                          bitrate,
                          samplerate,
                          channel_num,
                          lang);

                 /* track id means nothing for unsupported tracks */
                 pSingleAudioTrackInfo->u32TrackId = 0xFF;
                 pSingleAudioTrackInfo->u32Bitrate = bitrate;
                 pSingleAudioTrackInfo->u32SampleRate = samplerate;
                 pSingleAudioTrackInfo->u32ChannelNum = channel_num;
                 if (audio_type) {
                     E_SME_MEM_SECURE_RET eSecRet;
                     str_len = strlen(audio_type);
                     eSecRet = VOS_Memcpy_S(pSingleAudioTrackInfo->cAudioType, AUDIO_TRACK_ARRAY_MAX_LEN,
                                audio_type, MIN((gint)str_len, AUDIO_TRACK_ARRAY_MAX_LEN - 1));
                     if(eSecRet != E_SME_MEM_OK)
                     {
                         ICS_LOGE("VOS_Memcpy_S fail.");
                     }
                 }

                 if (lang) {
                     E_SME_MEM_SECURE_RET eSecRet;
                     str_len = strlen(lang);
                     eSecRet = VOS_Memcpy_S(pSingleAudioTrackInfo->cLang, AUDIO_TRACK_ARRAY_MAX_LEN,
                                lang, MIN((gint)str_len, AUDIO_TRACK_ARRAY_MAX_LEN - 1));
                     if(eSecRet != E_SME_MEM_OK)
                     {
                         ICS_LOGE("VOS_Memcpy_S fail.");
                     }
                 }
             } else if (!strcmp(gst_structure_get_name(s),
                         "video-track-info")) {
                 const gchar *video_type;
                 guint trackid = 0;
                 guint media_bitrate = 0;
                 guint bitrate = 0;
                 guint width = 0, height = 0;
                 guint str_len;
                 gboolean isSupported = 1;

                 ST_VIDEO_TRACK_INFO *pVideoTrackInfo = &pobjThis->m_stVideoTrackInfo;

                 if (!gst_structure_get_uint(s, "video-track-id", &trackid)) {
                     ICS_LOGI("fail to get video-track-id");
                 } else {
                     pVideoTrackInfo->u32TrackId = trackid;
                 }
                 video_type = gst_structure_get_string(s, "video-type");
                 if (!video_type) {
                     ICS_LOGI("fail to get video-type");
                 } else {
                     E_SME_MEM_SECURE_RET eSecRet;
                     str_len = strlen(video_type);
                     eSecRet = VOS_Memcpy_S(pVideoTrackInfo->cVideoType, M_SME_MIME_LEN_MAX,
                                video_type, MIN((gint)str_len, M_SME_MIME_LEN_MAX - 1));
                     if(eSecRet != E_SME_MEM_OK)
                     {
                         ICS_LOGE("VOS_Memcpy_S fail.");
                     }
                 }

                 if (!gst_structure_get_uint(s, "media-bitrate", &media_bitrate)) {
                     ICS_LOGI("fail to get media-bitrate");
                 } else {
                     pVideoTrackInfo->u32Bps_media = media_bitrate;
                 }

                 if (!gst_structure_get_uint(s, "video-bitrate", &bitrate)) {
                     ICS_LOGI("fail to get video-bitrate");
                 } else {
                     pVideoTrackInfo->u32Bps = bitrate;
                 }

                 if (!gst_structure_get_uint(s, "video-width", &width)) {
                     ICS_LOGI("fail to get video-width");
                 } else {
                     pVideoTrackInfo->u32Width = width;
                 }

                 if (!gst_structure_get_uint(s, "video-height", &height)) {
                     ICS_LOGI("fail to get video-height");
                 } else {
                     pVideoTrackInfo->u32Height = height;
                 }

                 if (!gst_structure_get_boolean(s, "codec-is_supported", &isSupported)) {
                     ICS_LOGI("fail to get codec-is_supported");
                 } else {
                     pVideoTrackInfo->bIsSupported = (V_BOOL)isSupported;
                 }

                 ICS_LOGI("video_track_info: "
                      "track-id=%d, type=%s, bitrate=%d, [w=%d, h=%d], "
                      "codec-is_supported=%d",
                      trackid,
                      video_type,
                      bitrate,
                      width, height, isSupported);
             }

             break;
        }
        default:
            ICS_LOGD("drop msg=%d", GST_MESSAGE_TYPE (msg));
            break;
    }

    ICS_LOGI("bus_call out, msg=%#x, typename:%s", GST_MESSAGE_TYPE (msg),
        GST_MESSAGE_TYPE_NAME(msg));

    return TRUE;
}
/** END:   Modified for DTS2014072406838 by liurongliang(l00180035), 2014/7/24 */

static V_UINT32 RunMainLoopThd(INOUT V_VOID* pvCtx)
{
    SmePlayerGstWrPipe* pobjThis = static_cast<SmePlayerGstWrPipe*>(pvCtx);

    ICS_LOGI("RunMainLoopThd in, pobjThis=%p", pobjThis);

    VOS_ThdMutexLock(&pobjThis->m_stMutex);
    if(ICS_FALSE != pobjThis->m_bExitBlock)
    {
        g_main_loop_unref (pobjThis->m_pstMainloop);
        pobjThis->m_pstMainloop = NULL;
        VOS_ThdMutexUnLock(&pobjThis->m_stMutex);

        ICS_LOGI("RunMainLoopThd out, not runloop because of exitblock!");

        return 0;
    }
    VOS_ThdMutexUnLock(&pobjThis->m_stMutex);

    VOS_ThdSemPost(pobjThis->m_hdlSem);
    g_main_loop_run (pobjThis->m_pstMainloop);

    ICS_LOGI("RunMainLoopThd out");

    return 0;
}

/*static*/
V_UINT32 SmePlayerGstWrap::InitGst(INOUT V_VOID*& pvPipeline,
    IN pfnGstMsgHandler pfnHandler, IN V_VOID* pvCtx,
    IN E_SME_PLAYER_LOG_LEVEL eLevel, IN const V_CHAR* pszLogFileName /*= NULL*/,
    IN const V_CHAR* pszDynamicLoadLibs /*= NULL*/)
{
    V_UINT32            u32Ret                  = static_cast<V_UINT32>(
                                                    E_SME_PLAYER_ERROR_NONE);
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe  = NULL;
    GstElement*         pstSmePlaybin           = NULL;
    guint               u32BusWatchId           = 0;
    VOSSEMHDL           hdlSem                  = NULL;

    ICS_LOGD("InitGst in, eLevel=%d", eLevel);

    UTILS_M_INVALID_ARGS(pszLogFileName);
    UTILS_M_INVALID_ARGS(pszDynamicLoadLibs);
    pvPipeline = NULL;
    do
    {
        GstBus*             pstPipelineBus;

        VOS_ThdMutexLock(&g_sstLogMutex);
        ICS_SetLogLevel(SMELogLevelToICSLogLevel(eLevel));
        gst_debug_set_default_threshold (SmeLogLevelToGstLogLevel(eLevel));
        VOS_ThdMutexUnLock(&g_sstLogMutex);

        ICS_LOGI("make gstsmeplaybin begin ...");
        pstSmePlaybin = gst_element_factory_make (M_SME_PLAYBIN_NAME, NULL);
        ICS_LOGI("make gstsmeplaybin end, pstSmePlaybin=%p", pstSmePlaybin);
        UTILS_MLOGE_BRK_VAL_IF(pstSmePlaybin == NULL, u32Ret,
            E_SME_PLAYER_ERROR_NOMEM, ("make %s failed", M_SME_PLAYBIN_NAME));

        pstPipelineBus = gst_element_get_bus(pstSmePlaybin);
        UTILS_MLOGE_BRK_VAL_IF(pstPipelineBus == NULL, u32Ret,
            E_SME_PLAYER_ERROR_NOMEM, ("get pipeline bus failed"));

        pobjSmePlayerGstWrPipe = new SmePlayerGstWrPipe(pstSmePlaybin,
            pfnHandler, pvCtx, (V_UINT32)-1);
        UTILS_MLOGE_BRK_VAL_IF(NULL == pobjSmePlayerGstWrPipe, u32Ret,
            E_SME_PLAYER_ERROR_NOMEM, ("new SmePlayerGstWrPipe failed"));

        g_signal_connect (pstSmePlaybin,"manifest-parse-complete",
            G_CALLBACK (SmePlayerGstWrPipe::ManifestParseCompleteCb),
            pobjSmePlayerGstWrPipe);

        hdlSem = VOS_CreateThdSem(0);
        UTILS_MLOGE_BRK_VAL_IF(NULL == hdlSem, u32Ret, E_SME_PLAYER_ERROR_NOMEM,
            ("new SmePlayerGstWrPipe failed"));
        pobjSmePlayerGstWrPipe->m_hdlSem = hdlSem;

        u32BusWatchId = gst_bus_add_watch (pstPipelineBus, bus_call,
            pobjSmePlayerGstWrPipe);
        gst_object_unref (pstPipelineBus);
        pobjSmePlayerGstWrPipe->m_u32BusWatchId = u32BusWatchId;

        pvPipeline = pobjSmePlayerGstWrPipe;
        u32Ret = static_cast<V_UINT32>(E_SME_PLAYER_ERROR_NONE);
    } while (ICS_FALSE);

    if(static_cast<V_UINT32>(E_SME_PLAYER_ERROR_NONE) != u32Ret)
    {
        if(pstSmePlaybin != NULL )
        {
            g_object_unref (pstSmePlaybin);
        }

        VOS_DestroyThdSem(hdlSem);

        UTILS_MSAFEDEL(pobjSmePlayerGstWrPipe);

        //gst_deinit();
    }
    ICS_LOGD("InitGst out");

    return u32Ret;
}

/*static*/
V_VOID SmePlayerGstWrap::DeInitGst(INOUT V_VOID* pvPipeline)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    ICS_LOGI("DeInitGst in, pvPipeline=%p", pvPipeline);

    UTILS_M_RET_IF(NULL == pvPipeline);

    VOS_ThdSemPost(pobjSmePlayerGstWrPipe->m_hdlSem);

    if(0 != pobjSmePlayerGstWrPipe->m_u32BusWatchId)
    {
        g_source_remove (pobjSmePlayerGstWrPipe->m_u32BusWatchId);
        pobjSmePlayerGstWrPipe->m_u32BusWatchId = 0;
    }

    if(pobjSmePlayerGstWrPipe->m_pstSmePlaybin != NULL)
    {
        ICS_LOGI("DeInitGst g_object_unref m_pstSmePlaybin begin...");
        g_object_unref (pobjSmePlayerGstWrPipe->m_pstSmePlaybin);
        ICS_LOGI("DeInitGst g_object_unref m_pstSmePlaybin end");
        pobjSmePlayerGstWrPipe->m_pstSmePlaybin = NULL;
    }

    VOS_DestroyThdSem(pobjSmePlayerGstWrPipe->m_hdlSem);
    UTILS_MSAFEDEL(pobjSmePlayerGstWrPipe);

    //TODO gst deinit 是进程唯一这里按照对象来的，故需要考虑此问题
    //gst_deinit();

    ICS_LOGI("DeInitGst out");

    return;
}

/*static*/
V_VOID SmePlayerGstWrap::ExitGstBlock(IN V_VOID* pvPipeline)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    ICS_LOGD("ExitGstBlock in, pvPipeline=%p", pvPipeline);

    UTILS_M_RET_IF(NULL == pvPipeline);

    VOS_ThdMutexLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    pobjSmePlayerGstWrPipe->m_bExitBlock = ICS_TRUE;
    VOS_ThdSemPost(pobjSmePlayerGstWrPipe->m_hdlSem);
    VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);

    /* BEGIN: Added for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */
    if(pobjSmePlayerGstWrPipe->m_pstSmePlaybin != NULL)
    {
        //let all element exit block.
        ICS_LOGD("ExitGstBlock, pvPipeline=%p, set exit block begin ...", pvPipeline);
        g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin, GST_SME_PROP_KEY_EXIT_BLOCK,
            1, NULL);
        ICS_LOGD("ExitGstBlock, pvPipeline=%p, set exit block end", pvPipeline);
    }
    /* END:   Added for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */

    ICS_LOGD("ExitGstBlock out--");

    return;
}

/*static*/
V_UINT32 SmePlayerGstWrap::Prepare(IN V_VOID* pvPipeline, IN const V_CHAR* pszUri,
    IN const V_VOID* pvSurface, IN V_UINT32 u32BufferFull,
    IN V_UINT32 u32LowPercent, IN V_UINT32 u32HighPercent,
    IN V_BOOL bRect /*= ICS_FALSE*/, IN V_BOOL bSync /*= ICS_TRUE*/,
    IN const ST_SubCallback* pstSubCb /*= NULL*/, IN const V_CHAR* pszSubUri /*= NULL*/)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);
    gchar* pszGstUri = NULL;
    gchar* pszGstSubUri = NULL;
    V_UINT64 u64BufDuration = ((V_UINT64)u32BufferFull)*M_SME_MS_TO_NS;
    GstElement* text_sink = NULL;
    gchar *protocol = NULL;
    (void)pszSubUri;
    (void)pstSubCb;

    UTILS_M_INVALID_ARGS(bRect);
    UTILS_M_INVALID_ARGS(pvSurface);
    ICS_LOGD("Prepare in, pvPipeline=%p, pszUri=%p, bSync=%d, bufffull=%u",
        pvPipeline, pszUri, (V_INT32)bSync, u32BufferFull);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline) || (NULL == pszUri),
        E_SME_PLAYER_ERROR_INVALID_ARGS, ("null pointer"));
    do
    {
        pobjSmePlayerGstWrPipe->m_eGstWrapOp = E_OP_NONE;
        if (gst_uri_is_valid (pszUri))
        {
            pszGstUri = g_strdup (pszUri);
        }
        else
        {
            pszGstUri = gst_filename_to_uri (pszUri, NULL);
        }
        UTILS_MLOGE_BRK_VAL_IF(NULL == pszGstUri, u32Ret,
            E_SME_PLAYER_ERROR_INVALID_ARGS, ("url is not valid"));
            //E_SME_PLAYER_ERROR_INVALID_ARGS, ("url=%s is not valid", pszUri));
#ifdef DENABLE_SUBTITLE_PATH
        if(pszSubUri)
        {
            if (gst_uri_is_valid (pszSubUri))
            {
                pszGstSubUri = g_strdup (pszSubUri);
            }
            else
            {
                pszGstSubUri = gst_filename_to_uri (pszSubUri, NULL);
            }
        }

        if(pszGstSubUri)
        {
            pobjSmePlayerGstWrPipe->m_pobjTextSink = new SmeGstAppsinkOfSubWr();
            if(NULL == pobjSmePlayerGstWrPipe->m_pobjTextSink)
            {
                ICS_LOGW("new pobjSmePlayerGstWrPipe->m_pobjTextSink failed why?");
            }
            else
            {
                u32Ret = pobjSmePlayerGstWrPipe->m_pobjTextSink->Init();
                if(ICS_SUCCESS != u32Ret)
                {
                    ICS_LOGW("new pobjSmePlayerGstWrPipe->m_pobjTextSink init failed why?");
                    UTILS_MSAFEDEL(pobjSmePlayerGstWrPipe->m_pobjTextSink);
                }
                else
                {
                    if(NULL != pstSubCb)
                    {
                        V_UINT32 u32Res;
                        u32Res = pobjSmePlayerGstWrPipe->m_pobjTextSink->RegisterCb(*pstSubCb);
                        UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("RegisterCb fail."));
                    }

                    g_object_set (pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                        "suburi", pszGstSubUri,
                        NULL);
                    text_sink = pobjSmePlayerGstWrPipe->m_pobjTextSink->GetAppSinkInstance();
                    ICS_LOGI("GetAppSinkInstance text_sink = %p", text_sink);
                    //ref(text_sink)
                    g_object_set (pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                        "text-sink", gst_object_ref(text_sink),
                        NULL);
                    ICS_LOGI("GetAppSinkInstance text_sink refcount = %d",
                        ((GstMiniObject*)text_sink)->refcount);
                }
            }
        }
#endif
        //ICS_LOGD("pszUri=%s, pszGstUri=%s", pszUri, pszGstUri);
        VOS_ThdMutexLock(&pobjSmePlayerGstWrPipe->m_stMutex);

        pobjSmePlayerGstWrPipe->m_pstMainloop = g_main_loop_new (NULL, FALSE);
        if(NULL == pobjSmePlayerGstWrPipe->m_pstMainloop)
        {
            VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);
            u32Ret =  E_SME_PLAYER_ERROR_NOMEM;
            ICS_LOGE("new main loop failed");
            break;
        }

        u32Ret = VOS_CreateThread(&pobjSmePlayerGstWrPipe->m_hdlRunMainLoopThd,
            NULL, RunMainLoopThd, pobjSmePlayerGstWrPipe);
        if(NULL == pobjSmePlayerGstWrPipe->m_hdlRunMainLoopThd
           || u32Ret != ICS_SUCCESS)
        {
            g_main_loop_unref (pobjSmePlayerGstWrPipe->m_pstMainloop);
            pobjSmePlayerGstWrPipe->m_pstMainloop = NULL;
            VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);
            u32Ret = E_SME_PLAYER_ERROR_NOMEM;
            ICS_LOGE("create thd failed");
            break;
        }
        VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);

        VOS_ThdSemWait(pobjSmePlayerGstWrPipe->m_hdlSem);
#ifndef __LINUX__
        protocol = gst_uri_get_protocol(pszUri);
        if (protocol && !VOS_Memcmp(protocol, "dvb", 3)) {
            g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                         "flags", SME_GST_DVB_FLAGS, NULL);
        }else {
            g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                         "flags", SME_GST_PLAYBIN_FLAGS, NULL);
        }
        if (protocol) {
            g_free(protocol);
            protocol = NULL;
        }
#else
        g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
            "flags", SME_GST_PLAYBIN_FLAGS, NULL);
#endif
        g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
            "ring-buffer-max-size", SME_GST_PLAYBIN_RING_QUEUE_MAX_SIZE,
            "buffer-size", SME_GST_PLAYBIN_QUEUE_MAX_SIZE,
            "buffer-duration", (V_INT64)u64BufDuration,
            "low-percent", u32LowPercent,
            "high-percent", u32HighPercent,
            NULL);
        pobjSmePlayerGstWrPipe->m_u32BufferFull = u32BufferFull;
        pobjSmePlayerGstWrPipe->m_stBufferStatus.u32MaxBufferTime = u32BufferFull;
        pobjSmePlayerGstWrPipe->m_stBufferStatus.u32LowPercent = u32LowPercent;
        pobjSmePlayerGstWrPipe->m_stBufferStatus.u32HighPercent = u32HighPercent;
        g_object_set (pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
            "uri", pszGstUri, NULL);

#if !defined (__LINUX__)
        //set surface
        if(ICS_FALSE == bRect)
        {
            g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                GST_SME_PROP_KEY_VDISP_SF, (gulong)pvSurface, NULL);
        }
        else
        {
            g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                GST_SME_PROP_KEY_VDISP_RECT, (gulong)pvSurface, NULL);
        }
#endif

#if 0
        //set demux buffer full.
        g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
            GST_SME_PROP_KEY_BUFFER_FULL, u32BufferFull, NULL);
#endif
        pobjSmePlayerGstWrPipe->m_bSyncPrepare = bSync;
        pobjSmePlayerGstWrPipe->m_eGstWrapOp = E_OP_PAUSING;
        GstStateChangeReturn eResult = gst_element_set_state (
            pobjSmePlayerGstWrPipe->m_pstSmePlaybin, GST_STATE_PAUSED);
        ICS_LOGI("prepare change to pause eResult=%s", gst_element_state_change_return_get_name(eResult));
        if(GST_STATE_CHANGE_ASYNC == eResult)
        {
            if(bSync)
            {
                VOS_ThdSemWait(pobjSmePlayerGstWrPipe->m_hdlSem);
                //if sync prepare failed,here will return the error.
                if(E_SME_PLAYER_ERROR_NONE != pobjSmePlayerGstWrPipe->m_u32Error){
                    u32Ret = pobjSmePlayerGstWrPipe->m_u32Error;
                    pobjSmePlayerGstWrPipe->m_u32Error = E_SME_PLAYER_ERROR_NONE;
                }else{
                    u32Ret = E_SME_PLAYER_ERROR_NONE;
                }
            }
            else
            {
                u32Ret = E_SME_PLAYER_ERROR_ASYNC;
            }
        }
        else if(GST_STATE_CHANGE_SUCCESS == eResult)
        {
            u32Ret = E_SME_PLAYER_ERROR_NONE;
        }
        else
        {
            ICS_LOGE("Prepare gst_element_set_state failed(%#x)", u32Ret);
            u32Ret = E_SME_PLAYER_ERROR_UNKOWN;
            pobjSmePlayerGstWrPipe->m_eGstWrapOp = E_OP_NONE;
        }
    } while (ICS_FALSE);

    pobjSmePlayerGstWrPipe->m_bSyncPrepare = ICS_FALSE;

    if((E_SME_PLAYER_ERROR_NONE != u32Ret) && (E_SME_PLAYER_ERROR_ASYNC != u32Ret))
    {
        if(NULL != pobjSmePlayerGstWrPipe->m_pstMainloop)
        {
            /** BEGIN: Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
            ICS_LOGD("Prepare, pvPipeline=%p,wait mainloop running...", pvPipeline);
            while(!g_main_loop_is_running(pobjSmePlayerGstWrPipe->m_pstMainloop))
            {
                VOS_MSleep(2);
            }
            ICS_LOGD("Prepare, pvPipeline=%p,mainloop is running,quit now!",
                pvPipeline);
            /** END:   Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */

            g_main_loop_quit(pobjSmePlayerGstWrPipe->m_pstMainloop);
            ICS_LOGD("Prepare join thd begin...");
            VOS_JoinThread(pobjSmePlayerGstWrPipe->m_hdlRunMainLoopThd);
            ICS_LOGD("Prepare join thd end");
            pobjSmePlayerGstWrPipe->m_hdlRunMainLoopThd = NULL;
            g_main_loop_unref (pobjSmePlayerGstWrPipe->m_pstMainloop);
            pobjSmePlayerGstWrPipe->m_pstMainloop = NULL;
        }
    }

    g_free(pszGstUri);
    pszGstUri = NULL;

    g_free(pszGstSubUri);
    pszGstSubUri = NULL;

    (void)text_sink;
    (void)protocol;

    ICS_LOGD("Prepare out,u32Ret=%u", u32Ret);

    return u32Ret;
}

/*static*/
V_UINT32 SmePlayerGstWrap::RegisterSubCb(IN V_VOID* pvPipeline, IN const ST_SubCallback* pstSubCb)
{
#ifdef DENABLE_SUBTITLE_PATH
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);
    ST_SubCallback stCb;
    UTILS_MLOGE_RET_VAL_IF(NULL == pvPipeline
        || NULL == pobjSmePlayerGstWrPipe->m_pobjTextSink, ICS_FAIL,
        ("RegisterSubCb invalid args pvPipeline=%p", pvPipeline));

    if (E_SME_MEM_OK != VOS_Memset_S(
        &stCb,
        sizeof(stCb),
        0,
        sizeof(stCb)))
    {
        ICS_LOGE("SECURE: VOS_Memset_S return failed. why ?");
    }

    if(NULL != pstSubCb)
    {
        stCb = *pstSubCb;
    }

    return pobjSmePlayerGstWrPipe->m_pobjTextSink->RegisterCb(stCb);
#else
    (void)pvPipeline;
    (void)pstSubCb;
    return ICS_FAIL;
#endif
}

/*static*/
V_VOID SmePlayerGstWrap::Reset(IN V_VOID* pvPipeline)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);
    GstStateChangeReturn result;

    ICS_LOGD("Reset in, pvPipeline=%p", pvPipeline);
    UTILS_M_RET_IF(NULL == pvPipeline);

    VOS_ThdSemPost(pobjSmePlayerGstWrPipe->m_hdlSem);

    if(NULL != pobjSmePlayerGstWrPipe->m_pstMainloop)
    {
        /** BEGIN: Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */
        ICS_LOGD("Reset, pvPipeline=%p,wait mainloop running...", pvPipeline);
        while(!g_main_loop_is_running(pobjSmePlayerGstWrPipe->m_pstMainloop))
        {
            VOS_MSleep(2);
        }
        ICS_LOGD("Reset, pvPipeline=%p,mainloop is running,quit now!", pvPipeline);
        /** END: Added for DTS2014061909048 by liurongliang(l00180035), 2014/7/7 */

        g_main_loop_quit (pobjSmePlayerGstWrPipe->m_pstMainloop);
        VOS_JoinThread(pobjSmePlayerGstWrPipe->m_hdlRunMainLoopThd);
        g_main_loop_unref (pobjSmePlayerGstWrPipe->m_pstMainloop);
        pobjSmePlayerGstWrPipe->m_pstMainloop = NULL;
        pobjSmePlayerGstWrPipe->m_hdlRunMainLoopThd = NULL;
    }

    VOS_ThdMutexLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    pobjSmePlayerGstWrPipe->m_bExitBlock    = ICS_FALSE;
    pobjSmePlayerGstWrPipe->m_bStarting     = ICS_FALSE;
    pobjSmePlayerGstWrPipe->m_i32SeekTo     = -1;
    pobjSmePlayerGstWrPipe->m_eGstWrapOp    = E_OP_NONE;
    pobjSmePlayerGstWrPipe->m_u32Error      = E_SME_PLAYER_ERROR_NONE;
    VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);

    if(NULL != pobjSmePlayerGstWrPipe->m_pstSmePlaybin)
    {
        /* BEGIN: Modified for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */
        result = gst_element_set_state (pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
            GST_STATE_NULL);
        UTILS_MLOGW_IF(GST_STATE_CHANGE_FAILURE == result, ("Set state failed."));
        g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin, GST_SME_PROP_KEY_EXIT_BLOCK,
            0, NULL);
        /* END:   Modified for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */
    }
#ifdef DENABLE_SUBTITLE_PATH
    if(NULL != pobjSmePlayerGstWrPipe->m_pobjTextSink)
    {
        pobjSmePlayerGstWrPipe->m_pobjTextSink->DeInit();
        delete pobjSmePlayerGstWrPipe->m_pobjTextSink;
        pobjSmePlayerGstWrPipe->m_pobjTextSink = NULL;
    }
#endif
    pobjSmePlayerGstWrPipe->m_i32BufPercent = 0;
    pobjSmePlayerGstWrPipe->m_u32BufferFull = 0;

    ICS_LOGD("Reset out");
    return ;
}

/*static*/
V_UINT32 SmePlayerGstWrap::Start(IN V_VOID* pvPipeline)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    ICS_LOGI("Start in, pvPipeline=%p", pvPipeline);
    UTILS_MLOGE_RET_VAL_IF((NULL == pobjSmePlayerGstWrPipe)
        || (NULL == pobjSmePlayerGstWrPipe->m_pstSmePlaybin)
        , E_SME_PLAYER_ERROR_INVALID_ARGS, ("null pointer"));

    /** BEGIN: Modified for DTS2014081400222 by liurongliang(l00180035), 2014/8/14 */
    if((GST_STATE_PLAYING == GST_STATE(pobjSmePlayerGstWrPipe->m_pstSmePlaybin))
        && (GST_STATE_VOID_PENDING == GST_STATE_PENDING(pobjSmePlayerGstWrPipe->m_pstSmePlaybin)))
    {
        ICS_LOGI("already started!");
        return u32Ret;
    }
    /** END: Modified for DTS2014081400222 by liurongliang(l00180035), 2014/8/14 */

    VOS_ThdMutexLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    pobjSmePlayerGstWrPipe->m_bStarting = ICS_TRUE;
    E_GST_WRAP_OP eLast = pobjSmePlayerGstWrPipe->m_eGstWrapOp;
    pobjSmePlayerGstWrPipe->m_eGstWrapOp = E_OP_PLAYING;
    VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    GstStateChangeReturn eResult = gst_element_set_state(
        pobjSmePlayerGstWrPipe->m_pstSmePlaybin, GST_STATE_PLAYING);
    if((GST_STATE_CHANGE_ASYNC != eResult) && (GST_STATE_CHANGE_SUCCESS != eResult))
    {
        ICS_LOGE("Start set gst state to playing failed(%#x)", eResult);
        u32Ret = E_SME_PLAYER_ERROR_UNKOWN;
        VOS_ThdMutexLock(&pobjSmePlayerGstWrPipe->m_stMutex);
        pobjSmePlayerGstWrPipe->m_bStarting = ICS_FALSE;
        pobjSmePlayerGstWrPipe->m_eGstWrapOp = eLast;
        VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    }
    else
    {
        /** BEGIN: Modified for DTS2014081400222 by liurongliang(l00180035), 2014/8/14 */
        //wait for playing callback
        if(GST_STATE_PAUSED != GST_STATE_PENDING(pobjSmePlayerGstWrPipe->m_pstSmePlaybin))
        {
            ICS_LOGD("Start wait for playing callback begin ...");
            VOS_ThdSemWait(pobjSmePlayerGstWrPipe->m_hdlSem);
            ICS_LOGD("Start wait for playing callback end");
        }
        else
        {
            VOS_ThdMutexLock(&pobjSmePlayerGstWrPipe->m_stMutex);
            pobjSmePlayerGstWrPipe->m_bStarting = ICS_FALSE;
            VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);
        }
        /** END: Modified for DTS2014081400222 by liurongliang(l00180035), 2014/8/14 */
    }

    ICS_LOGI("Start out, u32Ret=%u", u32Ret);

    return u32Ret;
}

/*static*/
V_UINT32 SmePlayerGstWrap::Pause(IN V_VOID* pvPipeline)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    ICS_LOGD("Pause in, pvPipeline=%p", pvPipeline);
    UTILS_MLOGE_RET_VAL_IF((NULL == pobjSmePlayerGstWrPipe)
        || (NULL == pobjSmePlayerGstWrPipe->m_pstSmePlaybin),
        E_SME_PLAYER_ERROR_INVALID_ARGS, ("null pointer"));

    VOS_ThdMutexLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    /* BEGIN: Added for DTS2015071706676 by liurongliang(l00180035), 2015/7/22 */
    E_GST_WRAP_OP eLast = pobjSmePlayerGstWrPipe->m_eGstWrapOp;
    pobjSmePlayerGstWrPipe->m_eGstWrapOp = E_OP_PAUSING;
    /* END:   Added for DTS2015071706676 by liurongliang(l00180035), 2015/7/22 */
    VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    GstStateChangeReturn eResult = gst_element_set_state (
        pobjSmePlayerGstWrPipe->m_pstSmePlaybin, GST_STATE_PAUSED);
    if((GST_STATE_CHANGE_ASYNC != eResult) && (GST_STATE_CHANGE_SUCCESS != eResult))
    {
        u32Ret = E_SME_PLAYER_ERROR_UNKOWN;
        VOS_ThdMutexLock(&pobjSmePlayerGstWrPipe->m_stMutex);
        pobjSmePlayerGstWrPipe->m_eGstWrapOp = eLast;
        VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    }

    ICS_LOGD("Pause out, u32Ret=%u", u32Ret);

    return u32Ret;
}

/** BEGIN: Modified for DTS2014073008993 by liurongliang(l00180035), 2014/7/31 */
/*static*/
V_UINT32 SmePlayerGstWrap::Stop(IN V_VOID* pvPipeline)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    ICS_LOGI("Stop in, pvPipeline=%p", pvPipeline);
    UTILS_MLOGE_RET_VAL_IF((NULL == pobjSmePlayerGstWrPipe)
        || (NULL == pobjSmePlayerGstWrPipe->m_pstSmePlaybin),
        E_SME_PLAYER_ERROR_INVALID_ARGS, ("null pointer"));
    VOS_ThdMutexLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    pobjSmePlayerGstWrPipe->m_eGstWrapOp = E_OP_NONE;
    VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    GstStateChangeReturn eResult = gst_element_set_state (
        pobjSmePlayerGstWrPipe->m_pstSmePlaybin, GST_STATE_READY);
    if(GST_STATE_CHANGE_SUCCESS != eResult)
    {
        u32Ret = E_SME_PLAYER_ERROR_UNKOWN;
        ICS_LOGE("Stop set gst state to paused failed(%#x)", eResult);
    }

    pobjSmePlayerGstWrPipe->m_i32BufPercent = 0;

    ICS_LOGI("Stop out, u32Ret=%u", u32Ret);

    return u32Ret;
}
/** END:   Modified for DTS2014073008993 by liurongliang(l00180035), 2014/7/31 */

/** BEGIN:   Modified for DTS2014072406838/DTS2014081202737 by liurongliang(l00180035), 2014/7/24 */
/*static*/
V_UINT32 SmePlayerGstWrap::SeekTo(IN V_VOID* pvPipeline, IN V_INT32 i32Ms)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;
    gint64 i64SeekToNs = i32Ms;
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);
    V_INT32 i32CurPos = 0;   //unit: ms

    ICS_LOGI("SeekTo in, pvPipeline=%p, SeekTo=%d ms, CurPos=%d ms",
        pvPipeline, i32Ms, i32CurPos);

    UTILS_MLOGE_RET_VAL_IF((NULL == pobjSmePlayerGstWrPipe)
        || (NULL == pobjSmePlayerGstWrPipe->m_pstSmePlaybin)
        , E_SME_PLAYER_ERROR_INVALID_ARGS, ("null pointer"));

    i32CurPos = GetCurPosition(pobjSmePlayerGstWrPipe);

    VOS_ThdMutexLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    if(-1 != pobjSmePlayerGstWrPipe->m_i32SeekTo)
    {
        //表示目前正在seek，等待seek完成，上面再来seek。
        VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);
        ICS_LOGI("SeekTo out, already seeking now");
        return E_SME_PLAYER_ERROR_ASYNC;
    }

    pobjSmePlayerGstWrPipe->m_i32SeekTo = i32Ms;
    //VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);

    i64SeekToNs *= M_SME_MS_TO_NS;

    GstSeekFlags eFlg;

    if (i32Ms <= i32CurPos) {
        eFlg = (GstSeekFlags)(GST_SEEK_FLAG_KEY_UNIT | GST_SEEK_FLAG_SNAP_BEFORE | GST_SEEK_FLAG_FLUSH);
    } else {
        eFlg = (GstSeekFlags)(GST_SEEK_FLAG_KEY_UNIT | GST_SEEK_FLAG_SNAP_AFTER | GST_SEEK_FLAG_FLUSH);
    }

    gboolean bResult = gst_element_seek_simple(
        pobjSmePlayerGstWrPipe->m_pstSmePlaybin, GST_FORMAT_TIME, eFlg,
        i64SeekToNs);
    if(TRUE != bResult)
    {
        u32Ret = E_SME_PLAYER_ERROR_UNKOWN;
        //VOS_ThdMutexLock(&pobjSmePlayerGstWrPipe->m_stMutex);
        pobjSmePlayerGstWrPipe->m_i32SeekTo = -1;
        //VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    }
    else
    {
        u32Ret = E_SME_PLAYER_ERROR_NONE;
        //VOS_ThdMutexLock(&pobjSmePlayerGstWrPipe->m_stMutex);
        pobjSmePlayerGstWrPipe->m_i32LastBufPercent = pobjSmePlayerGstWrPipe->m_i32BufPercent;
        pobjSmePlayerGstWrPipe->m_i32BufPercent = 0;
        //VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);
    }
    VOS_ThdMutexUnLock(&pobjSmePlayerGstWrPipe->m_stMutex);
#ifdef DENABLE_SUBTITLE_PATH
    if(pobjSmePlayerGstWrPipe->m_pobjTextSink)
    {
        pobjSmePlayerGstWrPipe->m_pobjTextSink->ClearLastDisp();
    }
#endif
    ICS_LOGI("SeekTo out, u32Ret=%u", u32Ret);

    return u32Ret;
}
/** END:   Modified for DTS2014072406838/DTS2014081202737 by liurongliang(l00180035), 2014/7/24 */

/*static*/
V_INT32 SmePlayerGstWrap::GetDuration(IN V_VOID* pvPipeline)
{
    V_INT32 i32Ret = 0;
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    ICS_LOGD("GetDuration in, pvPipeline=%p", pvPipeline);
    UTILS_MLOGE_RET_VAL_IF((NULL == pobjSmePlayerGstWrPipe)
        || (NULL == pobjSmePlayerGstWrPipe->m_pstSmePlaybin), 0, ("null pointer"));

    GstQuery* pstQuery = gst_query_new_duration (GST_FORMAT_TIME);
    UTILS_MLOGE_RET_VAL_IF(pstQuery == NULL, E_SME_PLAYER_ERROR_NOMEM,
        ("new query failed pointer"));

    gboolean bRes = gst_element_query (pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        pstQuery);
    if(!bRes)
    {
        i32Ret = 0;
        ICS_LOGE("query duration failed,bin=%p!",pobjSmePlayerGstWrPipe->m_pstSmePlaybin);
    }
    else
    {
        gint64 i64Ns = 0;
        GstFormat eFmt = GST_FORMAT_TIME;

        /** BEGIN: Modified for DTS2014081802192 by liurongliang(l00180035), 2014/8/18 */
        gst_query_parse_duration (pstQuery, &eFmt, &i64Ns);
        if(i64Ns >=0)
        {
            i32Ret = (V_INT32)(i64Ns / M_SME_MS_TO_NS);
        }
        else
        {
            i32Ret = -1;
        }
        /** END:   Modified for DTS2014081802192 by liurongliang(l00180035), 2014/8/18 */

        ICS_LOGI("gst_query_parse_duration i64Ns = %lld", i64Ns);
    }

    /** BEGIN: Added for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */
    gst_query_unref (pstQuery);
    /** END:   Added for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */

    ICS_LOGD("GetDuration out, i32Ret=%dms", i32Ret);

    return i32Ret;
}

/*static*/
V_INT32 SmePlayerGstWrap::GetCurPosition(IN V_VOID* pvPipeline)
{
    V_INT32 i32Ret = 0;
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    ICS_LOGD("GetCurPosition in, pvPipeline=%p", pvPipeline);
    UTILS_MLOGE_RET_VAL_IF((NULL == pobjSmePlayerGstWrPipe)
        || (NULL == pobjSmePlayerGstWrPipe->m_pstSmePlaybin), -1, ("null pointer"));

    GstQuery* pstQuery = gst_query_new_position (GST_FORMAT_TIME);
    UTILS_MLOGE_RET_VAL_IF(pstQuery == NULL, E_SME_PLAYER_ERROR_NOMEM,
        ("new query failed pointer"));

    gboolean bRes = gst_element_query (pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        pstQuery);
    if(!bRes)
    {
        i32Ret = 0;
    }
    else
    {
        gint64 i64Ns = 0;
        GstFormat eFmt = GST_FORMAT_TIME;
        gst_query_parse_position(pstQuery, &eFmt, &i64Ns);
        i32Ret = (V_INT32)(i64Ns / M_SME_MS_TO_NS);
    }
    /** BEGIN: Added for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */
    gst_query_unref (pstQuery);
    /** END:   Added for DTS2014081101027 by liurongliang(l00180035), 2014/8/11 */
    ICS_LOGD("GetCurPosition out, i32Ret=%dms", i32Ret);

    return i32Ret;
}

/*static*/
V_UINT32 SmePlayerGstWrap::GetCacheInfo(IN V_VOID* pvPipeline,
    INOUT V_UINT32& u32CurBw, INOUT V_UINT32& u32StreamBps,
    INOUT V_UINT32& u32ABuffered, INOUT V_UINT32& u32VBuffered,
    INOUT V_UINT32& u32VDecInterval, INOUT V_UINT32& u32VDecInDuration)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline), E_SME_PLAYER_ERROR_INVALID_ARGS,
        ("null pointer"));

    //curBw and StreamBps are not used now, set it static value
    u32CurBw = SME_DEFAULT_CUR_BW;
    u32StreamBps = u32CurBw;

    u32VDecInterval = SME_VDECOUT_INTERVAL_MAX;
    u32VDecInDuration = u32VDecInterval;

    // following properties only exist in MPI
    /*
      V_INT64 i64VDecInDuration = 0;
      u32VDecInterval = 33;
      u32VDecInDuration = 33;
      g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
      GST_SME_PROP_KEY_DECOUT_INTERVAL, &u32VDecInterval, NULL);
      g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
      GST_SME_PROP_KEY_DECIN_DURATION, &i64VDecInDuration, NULL);
      u32VDecInDuration = (V_UINT32)i64VDecInDuration;
    */

    u32ABuffered = u32VBuffered = (V_UINT32)(((V_UINT32)pobjSmePlayerGstWrPipe->m_i32BufPercent *
                                              pobjSmePlayerGstWrPipe->m_u32BufferFull) / 100);
    ICS_LOGD("percent = %d, full=%d, abufferd=%d",
             pobjSmePlayerGstWrPipe->m_i32BufPercent,
             pobjSmePlayerGstWrPipe->m_u32BufferFull,
             u32ABuffered);

    return static_cast<V_UINT32>(E_SME_PLAYER_ERROR_NONE);
}

/* static */
V_INT32 SmePlayerGstWrap::GetVideoWidth(IN V_VOID* pvPipeline)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);
    V_INT32 i32Width = -1;
    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline), E_SME_PLAYER_ERROR_INVALID_ARGS,
        ("null pointer"));

    g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_VIDEO_WIDTH, &i32Width, NULL);

    ICS_LOGD("GetVideoWidth=%d", i32Width);

    return i32Width;
}

/*  static */
V_INT32 SmePlayerGstWrap::GetVideoHeight(IN V_VOID* pvPipeline)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);
    V_INT32 i32Height = -1;
    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline), E_SME_PLAYER_ERROR_INVALID_ARGS,
        ("null pointer"));

    g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_VIDEO_HEIGHT, &i32Height, NULL);

    ICS_LOGD("GetVideoHeight=%d", i32Height);

    return i32Height;
}

/*static*/
V_UINT32 SmePlayerGstWrap::SetDispRect(IN V_VOID* pvPipeline, IN const ST_SME_DISP_RECT& stDispRest)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    ICS_LOGI("SetDispRect to wrap,x:y:w:h[%d:%d:%d:%d]",
        stDispRest.i32StartX, stDispRest.i32StartY,
        stDispRest.i32Width, stDispRest.i32Height);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline), E_SME_PLAYER_ERROR_INVALID_ARGS,
        ("null pointer"));

    ICS_LOGI("Set DispRect to playbin2: %d",(gulong)&stDispRest);
    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_VDISP_RECT, (gulong)&stDispRest, NULL);

    return ICS_SUCCESS;
}

V_INT32 SmePlayerGstWrap::MuteAudio(IN V_VOID *pvPipeline)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);
    V_UINT32 mute = TRUE;

    ICS_LOGW("MuteAudio");

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_MUTE_AUDIO, mute, NULL);

    return ICS_SUCCESS;
}


V_INT32 SmePlayerGstWrap::UnmuteAudio(IN V_VOID *pvPipeline)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);
    V_UINT32 mute = FALSE;

    ICS_LOGW("UnmuteAudio");

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_MUTE_AUDIO, mute, NULL);

    return ICS_SUCCESS;
}

V_INT32 SmePlayerGstWrap::SetAudioMute(IN V_VOID *pvPipeline, IN V_UINT32 mute)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    ICS_LOGW("set mute %d",mute);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_MUTE_AUDIO, mute, NULL);

    return ICS_SUCCESS;
}


V_INT32 SmePlayerGstWrap::GetAudioMute(IN V_VOID *pvPipeline, INOUT V_UINT32* mute)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_MUTE_AUDIO, mute, NULL);

    ICS_LOGW("Get Audio Mute: %u", *mute);

    return ICS_SUCCESS;
}

V_INT32 SmePlayerGstWrap::SetAudioVolume(IN V_VOID *pvPipeline, IN V_UINT32 volume)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    ICS_LOGW("set volume %d",volume);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_AUDIO_VOLUME, volume, NULL);

    return ICS_SUCCESS;
}

V_INT32 SmePlayerGstWrap::GetAudioVolume(IN V_VOID *pvPipeline, INOUT V_UINT32* volume)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_AUDIO_VOLUME, volume, NULL);

    ICS_LOGW("Get Audio Volume: %u", *volume);

    return ICS_SUCCESS;
}

V_INT32 SmePlayerGstWrap::GetVideoDispMode(IN V_VOID *pvPipeline,
    INOUT E_SME_PLAYER_DISPLAY_MODE* peMode,
    INOUT ST_DISP_RATIO* pstRatio)
{
    ICS_LOGD("wrap GetVideoDispMode in");
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    ST_DISP_RATIO* pstRatioTmp = NULL;

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));
    g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_DISP_MODE, peMode, NULL);
    g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_DISP_RADIO, &pstRatioTmp, NULL);
    if(NULL != pstRatioTmp){
        pstRatio->u32DispRatioW = pstRatioTmp->u32DispRatioW;
        pstRatio->u32DispRatioH = pstRatioTmp->u32DispRatioH;
    }else{
        ICS_LOGE("get disp mode failed in wrap");
    }

    ICS_LOGD("wrap GetVideoDispMode out");

    return ICS_SUCCESS;
}

V_INT32 SmePlayerGstWrap::GetVideoDispRect(IN V_VOID *pvPipeline,
    INOUT ST_SME_DISP_RECT* pstRect)
{
    ICS_LOGD("wrap GetVideoDispRect in");
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    ST_SME_DISP_RECT* pstRectTmp = NULL;

    g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_VDISP_RECT, &pstRectTmp, NULL);
    if(pstRectTmp != NULL)
    {
        pstRect->i32StartX = pstRectTmp->i32StartX;
        pstRect->i32StartY = pstRectTmp->i32StartY;
        pstRect->i32Width = pstRectTmp->i32Width;
        pstRect->i32Height = pstRectTmp->i32Height;
    }else{
        ICS_LOGE("get video disp rect failed in WRAP");
    }
    ICS_LOGD("wrap GetVideoDispMode out");

    return ICS_SUCCESS;
}


V_INT32 SmePlayerGstWrap::SetAudioChannelMode(IN V_VOID *pvPipeline, IN A_TRACK_CHANNEL_MODE_E mode)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    ICS_LOGW("set audio mode %d",mode);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_AUDIO_CHANNEL_MODE, mode, NULL);

    return ICS_SUCCESS;
}

V_INT32 SmePlayerGstWrap::GetAudioChannelMode(IN V_VOID *pvPipeline, INOUT A_TRACK_CHANNEL_MODE_E* mode)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_AUDIO_CHANNEL_MODE, mode, NULL);

    ICS_LOGW("Get Audio Mode: %u", *mode);

    return ICS_SUCCESS;
}



V_UINT32 SmePlayerGstWrap::SetLogLevel(IN V_VOID* pvPipeline, E_SME_PLAYER_LOG_LEVEL eLevel)
{
    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                       E_SME_PLAYER_ERROR_INVALID_ARGS,
                       ("null pointer"));
    ICS_LOGE("SetLogLevel, level:%d", (V_INT32)eLevel);

    VOS_ThdMutexLock(&g_sstLogMutex);
    ICS_SetLogLevel(SMELogLevelToICSLogLevel(eLevel));
    gst_debug_set_default_threshold (SmeLogLevelToGstLogLevel(eLevel));
    VOS_ThdMutexUnLock(&g_sstLogMutex);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::SetStreamBitrate(IN V_VOID* pvPipeline, IN V_UINT32 u32Bitrate)
{
    V_UINT64 u64Bitrate = u32Bitrate;
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));
    ICS_LOGW("SetStreamBitrate:u32Bitrate:%d kbps", u32Bitrate);
    /** BEGIN: Added for DTS2015103005728 by baihaili(b00311320), 2015/10/30 */
    if (u64Bitrate) {
        pobjSmePlayerGstWrPipe->m_stStartupMode.IsAdaptive = (V_BOOL)FALSE;
        pobjSmePlayerGstWrPipe->m_stStartupMode.StartupBitrate = u64Bitrate * 1000;
    } else {
        pobjSmePlayerGstWrPipe->m_stStartupMode.IsAdaptive = TRUE;
        pobjSmePlayerGstWrPipe->m_stStartupMode.StartupBitrate = 0;
    }
    /* END: Added for DTS2015103005728 by baihaili(b00311320), 2015/10/30 */
    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                 GST_SME_PROP_KEY_ADAPTIVE_STREAM_BT, u64Bitrate, NULL);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::SetWrapUserAgent(IN V_VOID* pvPipeline, IN const V_CHAR* pstUserAgent)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    UTILS_MLOGE_RET_VAL_IF((NULL == pstUserAgent),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    ICS_LOGI("SetWrapUserAgent:pstUserAgent:%s", pstUserAgent);

    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                 GST_SME_PROP_KEY_USER_AGENT, pstUserAgent, NULL);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::SetWrapReferer(IN V_VOID* pvPipeline, IN const V_CHAR* pstSetReferer)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    UTILS_MLOGE_RET_VAL_IF((NULL == pstSetReferer),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    ICS_LOGI("SetWrapReferer:pstSetReferer:%s", pstSetReferer);

    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                 GST_SME_PROP_KEY_REFERER, pstSetReferer, NULL);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::SetWrapCookies(IN V_VOID* pvPipeline, IN V_CHAR** pstCookies)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    UTILS_MLOGE_RET_VAL_IF((NULL == pstCookies),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));


    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                 GST_SME_PROP_KEY_COOKIES, pstCookies, NULL);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::SetWrapByteRange(IN V_VOID* pvPipeline, IN V_BOOL bByteRange)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    ICS_LOGI("SetWrapByteRange:bByteRange:%d", (V_INT32)bByteRange);

    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                 GST_SME_PROP_KEY_BYTE_RANGE, (gboolean)bByteRange, NULL);

    return ICS_SUCCESS;
}


V_VOID SmePlayerGstWrap::SetManifestCompleteCb(IN V_VOID* pvPipeline,
        IN PFN_StreamPlayModeCb pFnCb, IN V_VOID* pvCtx)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_IF((NULL == pvPipeline),("null pointer"));

    if (!pobjSmePlayerGstWrPipe->m_pfnManifestCompleteCb) {
       pobjSmePlayerGstWrPipe->m_pfnManifestCompleteCb = pFnCb;
    }

    pobjSmePlayerGstWrPipe->m_pvManifestCompleteCbCtx = pvCtx;

    return;
}

V_VOID SmePlayerGstWrap::SetBtChangedCb(IN V_VOID* pvPipeline,
        IN PFN_BitrateChangedCb pFnCb, IN V_VOID* pvCtx)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_IF((NULL == pvPipeline),("null pointer"));

    if (!pobjSmePlayerGstWrPipe->m_pfnBtChangedCb) {
       pobjSmePlayerGstWrPipe->m_pfnBtChangedCb = pFnCb;
    }

    pobjSmePlayerGstWrPipe->m_pvBtChangedCbCtx = pvCtx;

    return;
}

V_UINT32 SmePlayerGstWrap::GetBandwidth(IN V_VOID* pvPipeline)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF(NULL == pvPipeline, M_SME_UINT32_MAX, ("null pointer"));

    return pobjSmePlayerGstWrPipe->m_u32Bandwidth;
}

V_VOID *SmePlayerGstWrap::GetPlaybin(IN V_VOID* pvPipeline)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    return pobjSmePlayerGstWrPipe->m_pstSmePlaybin;
}

V_UINT32 SmePlayerGstWrap::SetAudioHdmiMode(IN V_VOID *pvPipeline,
                                           IN const V_INT8 *pi8HdmiMode)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    UTILS_MLOGE_RET_VAL_IF((NULL == pi8HdmiMode),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    ICS_LOGW("Set Audio Hdmi mode:%s", pi8HdmiMode);

    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_AUDIO_HDMI_MODE, pi8HdmiMode, NULL);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::GetAudioHdmiMode(IN V_VOID *pvPipeline,
                                           IN V_INT8 **pi8HdmiMode)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    UTILS_MLOGE_RET_VAL_IF((NULL == pi8HdmiMode),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_AUDIO_HDMI_MODE, pi8HdmiMode, NULL);

    ICS_LOGW("Get Audio Hdmi mode:%s", *pi8HdmiMode);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::GetBufferStatus(IN V_VOID *pvPipeline,
                                           INOUT ST_BUFFER_STATUS *pstBufferStatus)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    UTILS_MLOGE_RET_VAL_IF((NULL == pstBufferStatus),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    if (E_SME_MEM_OK != VOS_Memcpy_S(
                pstBufferStatus,
                sizeof(ST_BUFFER_STATUS),
                &(pobjSmePlayerGstWrPipe->m_stBufferStatus),
                sizeof(ST_BUFFER_STATUS)))
    {
        ICS_LOGE("SECURE: VOS_Memcpy_S return failed.");
    }

    ICS_LOGI("Get Buffer Status,Video buffer:size:%d,playtime:%lld,pts:%lld",
             pstBufferStatus->stVBufferStatus.u32BufferSize,
             pstBufferStatus->stVBufferStatus.u64BufferPlayingTime,
             pstBufferStatus->stVBufferStatus.u64LastBufferPTS);

    ICS_LOGI("Get Buffer Status,Audio buffer:size:%d,playtime:%lld,pts:%lld",
             pstBufferStatus->stABufferStatus.u32BufferSize,
             pstBufferStatus->stABufferStatus.u64BufferPlayingTime,
             pstBufferStatus->stABufferStatus.u64LastBufferPTS);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::GetCurrentBitrate(IN V_VOID* pvPipeline,
                                             INOUT ST_CURRENT_BITRATE *pstCurrentBitrate)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    UTILS_MLOGE_RET_VAL_IF((NULL == pstCurrentBitrate),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    VOS_Memcpy(pstCurrentBitrate,
               &(pobjSmePlayerGstWrPipe->m_stCurrentBitrate),
               sizeof(ST_CURRENT_BITRATE));

    ICS_LOGI("Get Current Bitrate,Audio Bitrate:%u,Video Bitrate:%u",
             pstCurrentBitrate->audio,
             pstCurrentBitrate->video);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::GetVideoTrackInfo(
    IN V_VOID *pvPipeline,
    INOUT ST_VIDEO_TRACK_INFO *pstVideoTrackInfo)
{
    E_SME_MEM_SECURE_RET eSecRet;
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    UTILS_MLOGE_RET_VAL_IF((NULL == pstVideoTrackInfo),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    ICS_LOGI("GetVideoTrackInfo: %s, w=%d, h=%d",
        pobjSmePlayerGstWrPipe->m_stVideoTrackInfo.cVideoType,
        pobjSmePlayerGstWrPipe->m_stVideoTrackInfo.u32Width,
        pobjSmePlayerGstWrPipe->m_stVideoTrackInfo.u32Height);

    eSecRet = VOS_Memcpy_S(pstVideoTrackInfo, sizeof(ST_VIDEO_TRACK_INFO),
               &(pobjSmePlayerGstWrPipe->m_stVideoTrackInfo),
               sizeof(ST_VIDEO_TRACK_INFO));
    if(eSecRet != E_SME_MEM_OK)
    {
        ICS_LOGE("VOS_Memcpy_S fail.");
    }

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::GetAudioTrackInfo(
    IN V_VOID *pvPipeline,
    INOUT ST_AUDIO_TRACK_INFO *pstAudioTrackInfo)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);
    V_INT32 i32AudioNum, i32AudioIndex;
    V_UINT32 u32Bitrate, u32SampleRate, u32ChannelNum;
    ST_AUDIO_TRACK_INFO *pstLocalAudioTrackInfo;
    V_CHAR *cAudioType, *cLang;
    GstTagList *tags;
    V_UINT32 u32StrLen;
    E_SME_MEM_SECURE_RET eSecRet;

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    UTILS_MLOGE_RET_VAL_IF((NULL == pstAudioTrackInfo),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));
    pstLocalAudioTrackInfo =
        &pobjSmePlayerGstWrPipe->m_stAudioTrackInfo;

    g_object_get (pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                  "n-audio", &i32AudioNum, NULL);

    pstLocalAudioTrackInfo->u32TrackNum = (V_UINT32)i32AudioNum;

    ICS_LOGI ("audio track number %d", i32AudioNum);

    if (i32AudioNum > MAX_AUDIO_TRACK)
    {
        ICS_LOGE("Audio track number(%d) exceeds maximum allowed number(%d)",
                 i32AudioNum, MAX_AUDIO_TRACK);
        return ICS_FAIL;
    }

    for (i32AudioIndex = 0; i32AudioIndex < i32AudioNum; i32AudioIndex++)
    {
        ST_SINGLE_AUDIO_TRACK_INFO *pstSingleAudioTrackInfo;

        pstSingleAudioTrackInfo =
            &pstLocalAudioTrackInfo->stSingleAudioTrackInfo[i32AudioIndex];
        pstSingleAudioTrackInfo->u32TrackId = (V_UINT32)i32AudioIndex;
        g_signal_emit_by_name (pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                               "get-audio-tags",
                               i32AudioIndex, &tags);
        if (tags) {
            gchar *str = gst_tag_list_to_string (tags);
            ICS_LOGI ("audio %d: %s", i32AudioIndex, str);
            g_free (str);

            if (gst_tag_list_get_uint (tags, GST_TAG_BITRATE, &u32Bitrate))
            {
                pstSingleAudioTrackInfo->u32Bitrate = u32Bitrate;
            }
            if (gst_tag_list_get_uint (tags, GST_TAG_AUDIO_SAMPLERATE,
                &u32SampleRate))
            {
                pstSingleAudioTrackInfo->u32SampleRate = u32SampleRate;
            }
            if (gst_tag_list_get_uint (tags, GST_TAG_AUDIO_CHANNELNUM,
            &u32ChannelNum))
            {
                pstSingleAudioTrackInfo->u32ChannelNum = u32ChannelNum;
            }
            if (gst_tag_list_get_string (tags, GST_TAG_AUDIO_CODEC, &cAudioType))
            {
                u32StrLen = strlen(cAudioType);
                eSecRet = VOS_Memcpy_S(pstSingleAudioTrackInfo->cAudioType, AUDIO_TRACK_ARRAY_MAX_LEN,
                    cAudioType, MIN((gint)u32StrLen, AUDIO_TRACK_ARRAY_MAX_LEN - 1));
                if(eSecRet != E_SME_MEM_OK)
                {
                    ICS_LOGE("VOS_Memcpy_S fail.");
                }
            }
            if (gst_tag_list_get_string (tags, GST_TAG_LANGUAGE_NAME, &cLang))
            {
                u32StrLen = strlen(cLang);
                eSecRet = VOS_Memcpy_S(pstSingleAudioTrackInfo->cLang, AUDIO_TRACK_ARRAY_MAX_LEN,
                    cLang, MIN((gint)u32StrLen, AUDIO_TRACK_ARRAY_MAX_LEN - 1));
                if(eSecRet != E_SME_MEM_OK)
                {
                    ICS_LOGE("VOS_Memcpy_S fail.");
                }
            }
        } else {
            ICS_LOGW("Couldn't get tag");
        }
    }

    g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                 "current-audio",
                 &pstLocalAudioTrackInfo->u32CurrentTrackId,
                 NULL);

    eSecRet = VOS_Memcpy_S(pstAudioTrackInfo, sizeof(ST_AUDIO_TRACK_INFO),
               &(pobjSmePlayerGstWrPipe->m_stAudioTrackInfo),
               sizeof(ST_AUDIO_TRACK_INFO));
    if(eSecRet != E_SME_MEM_OK)
    {
        ICS_LOGE("VOS_Memcpy_S fail.");
    }

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::SetActiveAudioTrack(
    IN V_VOID *pvPipeline,
    IN V_UINT32 u32AudioTrack)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);
    ST_AUDIO_TRACK_INFO *pstLocalAudioTrackInfo;
    ST_SINGLE_AUDIO_TRACK_INFO *pstSingleAudioTrackInfo;
    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));

    pstLocalAudioTrackInfo =
        &pobjSmePlayerGstWrPipe->m_stAudioTrackInfo;

    UTILS_MLOGE_RET_VAL_IF(
        (u32AudioTrack >= pstLocalAudioTrackInfo->u32TrackNum),
        E_SME_PLAYER_ERROR_INVALID_ARGS,
        ("invalid audio track index"));

    pstSingleAudioTrackInfo =
        &pstLocalAudioTrackInfo->stSingleAudioTrackInfo[u32AudioTrack];

    ICS_LOGI("audio-track-id=%d, audio-type=%s, "
             "audio-bitrate=%d, audio-sample-rate=%d, "
             "audio-channels=%d, audio-lang=%s \n",
             pstSingleAudioTrackInfo->u32TrackId,
             pstSingleAudioTrackInfo->cAudioType,
             pstSingleAudioTrackInfo->u32Bitrate,
             pstSingleAudioTrackInfo->u32SampleRate,
             pstSingleAudioTrackInfo->u32ChannelNum,
             pstSingleAudioTrackInfo->cLang);

    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
                 "current-audio",
                 u32AudioTrack,
                 NULL);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::SetDispMode(IN V_VOID *pvPipeline, IN V_UINT32 u32DisplayMode, IN ST_DISP_RATIO* pstDispRatio)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);
    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));
    if(pstDispRatio != NULL)
    {
        ICS_LOGI("Wrap,Dismode is : %d, W&H is %d & %d ", u32DisplayMode,
          pstDispRatio->u32DispRatioW, pstDispRatio->u32DispRatioH);
    }
    else
    {
        ICS_LOGI("wrap: the Radio is NULL");
    }

    ICS_LOGI("Set Disp Mode to playbin2: %d, Set Disp Ratio to playbin2: %p",u32DisplayMode, pstDispRatio);
    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_DISP_MODE, u32DisplayMode,
        GST_SME_PROP_KEY_DISP_RADIO, pstDispRatio, NULL);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::SetSwitchChannelMode(IN V_VOID *pvPipeline, IN V_UINT32 u32SwitchChannelMode)
{
    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));
    ICS_LOGI("Set SwitchChannelMode playbin2: %d", u32SwitchChannelMode);
    g_object_set(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_SWITCH_CHANNEL_MODE, u32SwitchChannelMode, NULL);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerGstWrap::GetDispMode(IN V_VOID *pvPipeline, IN V_UINT32* pDisplayMode, IN ST_DISP_RATIO** ppstDispRatio)
{
    V_UINT32 u32DisplayModeTmp = 0;
    ST_DISP_RATIO* pstDispRatioTmp = NULL;

    SmePlayerGstWrPipe* pobjSmePlayerGstWrPipe
        = static_cast<SmePlayerGstWrPipe*>(pvPipeline);

    UTILS_MLOGE_RET_VAL_IF((NULL == pvPipeline),
                           E_SME_PLAYER_ERROR_INVALID_ARGS,
                           ("null pointer"));
    g_object_get(pobjSmePlayerGstWrPipe->m_pstSmePlaybin,
        GST_SME_PROP_KEY_DISP_MODE, &u32DisplayModeTmp,
        GST_SME_PROP_KEY_DISP_RADIO, &pstDispRatioTmp, NULL);

    if(pDisplayMode != NULL)
    {
         *pDisplayMode = u32DisplayModeTmp;
    }
    if(ppstDispRatio != NULL)
    {
         *ppstDispRatio = pstDispRatioTmp;
    }

    return ICS_SUCCESS;
}
