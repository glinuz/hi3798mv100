/**************************************************************************//**

  Copyright (C), 2016-2026, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_dvbplayer_impl_gst.cpp
 * @brief   sme player gst implement
 * @date    2016/4/20
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2016/4/20
 * Desc   : Created file
 *
******************************************************************************/
#include <string.h>
#include "osal_type.h"
#include "sme_log.h"
#include "sme_log_adpt.h"
#include "sme_macro.h"
#include "osal_mem.h"
#include "osal_str.h"
#include "sme_dvbplayer_impl.h"

//lint -e826 //gstreamer ÀàÐÍ×ª»»ºê
//lint -e717 //glib do{}while(0)
//lint -e801
//lint -e1773
//lint -esym(1740, SmeDvbPlayerImpl::m_pobjPlayerObs, SmeDvbPlayerImpl::m_pobjGstPipeline)
//lint -esym(1740, SmeDvbPlayerImpl::m_hdlCmdCond, SmeDvbPlayerImpl::m_pstMainloop)
//lint -esym(1740, SmeDvbPlayerImpl::m_hdlRunMainLoopThd)
//lint -e838 //Previously assigned value to variable 'u32Ret' has not been used
//lint -e1506  //Call to virtual function 'SmePlayerImplGst::Reset(void)' within a constructor or destructor
//lint -e1579  //Pointer member 'SmePlayerImplGst::m_*** might have been freed by a separate function but no '-sem(SmePlayerImplGst::Reset,cleanup)' was seen
//lint -e1740 //m_stDvbProcCommonInfo doesn't malloc space, it doesn't not need to release space
//lint -e585 //The sequence is not a valid Trigraph sequence

#ifdef MOD_NAME
#undef MOD_NAME
#define MOD_NAME "SmeDvbPlayer"
#endif

#ifndef SMEPLAYER_STAT_TAG_TIME
#define SMEPLAYER_STAT_TAG_TIME "PlayerTimeLineTrace"
#endif

#define M_SME_DVBPLAYBIN_NAME "smedvbplaybin"
#define M_SME_DVBPLAYER_START_TIMEOUT (10*1000)//ms
#define DISP_RATIO_16TO9_WIDTH 16
#define DISP_RATIO_16TO9_HIGTH 9


#define SME_DVB_MAIN_VER ""SME_VERSION" [ "__DATE__" "__TIME__" ]"

static V_VOID setDispRatio(ST_DispRatio& rstRatio, V_UINT32 u32W, V_UINT32 u32H)
{
    rstRatio.u32DispRatioW = u32W;
    rstRatio.u32DispRatioH = u32H;
    return;
}

/*static*/
const V_CHAR* ISmeDvbPlayer::GetVersionInfo()
{
//lint -e10
    return SME_DVB_MAIN_VER;
//lint +e10
}

//ISmeDvbPlayer
ISmeDvbPlayer* ISmeDvbPlayer::CreateSmeDvbPlayer(
                                  E_DVB_TYPE eDvbType/* = E_DVB_TYPE_LIVE*/)
{
    return new SmeDvbPlayerImpl(eDvbType);
}

V_VOID ISmeDvbPlayer::DestroySmeDvbPlayer(INOUT ISmeDvbPlayer * pobjPlayer)
{
    UTILS_MTMPDEL(pobjPlayer);

    return;
}

/*static*/
V_UINT32 SmeDvbPlayerImpl::RunMainLoopThd(V_VOID* pvCtx)
{
    SmeDvbPlayerImpl* pobjThis = static_cast<SmeDvbPlayerImpl*>(pvCtx);

    ICS_LOGI("RunMainLoopThd in, pobjThis=%p", pobjThis);

    g_main_loop_run (pobjThis->m_pstMainloop);

    ICS_LOGI("RunMainLoopThd out, pobjThis=%p", pobjThis);

    return 0;
}

/*static*/
gboolean
SmeDvbPlayerImpl::bus_call(GstBus * bus, GstMessage * msg, gpointer data)
{
    SmeDvbPlayerImpl* pobjThis = static_cast<SmeDvbPlayerImpl*>(data);

    UTILS_M_INVALID_ARGS(bus);

    ICS_LOGI("bus_call, msg=%#x, typename:%s", GST_MESSAGE_TYPE (msg),
        GST_MESSAGE_TYPE_NAME(msg));

    switch (GST_MESSAGE_TYPE (msg))
    {
        case GST_MESSAGE_ASYNC_DONE:
        {
            ICS_LOGI("bus_call,ASYNC_DONE, msg=%#x, typename:%s,srcname=%s",
                GST_MESSAGE_TYPE (msg), GST_MESSAGE_TYPE_NAME(msg),
                GST_MESSAGE_SRC_NAME(msg));
            break;
        }
        case GST_MESSAGE_STATE_CHANGED:
        {
            GstState eOld = GST_STATE_VOID_PENDING;
            GstState eCur = GST_STATE_VOID_PENDING;
            GstState ePending = GST_STATE_VOID_PENDING;

            gst_message_parse_state_changed (msg, &eOld, &eCur, &ePending);

            ICS_LOGI("bus_call state_changed, msg=%#x, typename:%s,srcname=%s, "
                "old=%d, cur=%d, pending=%d", GST_MESSAGE_TYPE (msg),
                GST_MESSAGE_TYPE_NAME(msg), GST_MESSAGE_SRC_NAME(msg),
                eOld, eCur, ePending);

            VOS_ThdMutexLock(&pobjThis->m_stCmdMutex);

            if((GST_STATE_PLAYING == eCur)
                && (GST_ELEMENT(GST_MESSAGE_SRC(msg)) == pobjThis->m_pobjGstPipeline))
            {
                GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pobjThis->m_pobjGstPipeline),
                    GST_DEBUG_GRAPH_SHOW_ALL, "dvb.playing");
                ICS_LOGI("Playing");
                if(pobjThis->m_bWaitStartedSig)
                {
                    VOS_ThdCondSignal(pobjThis->m_hdlCmdCond);
                    pobjThis->m_bWaitStartedSig = ICS_FALSE;
                }
                else
                {
                    V_UINT64 u64CurTime = VOS_GetTimeTicks64();
                    V_UINT64 u64CostTime = 0;

                    if(M_SME_UINT64_MAX != pobjThis->m_u64TimeLineStartBg
                        && u64CurTime >= pobjThis->m_u64TimeLineStartBg)
                    {
                        u64CostTime = u64CurTime - pobjThis->m_u64TimeLineStartBg;
                        pobjThis->m_u64StartTime = u64CostTime;
                        ICS_LOGW("%s:PlayerTimeCostTrace-Start:%lld ms",
                            SMEPLAYER_STAT_TAG_TIME, u64CostTime);
                    }
                    else if(M_SME_UINT64_MAX != pobjThis->m_u64TimeLineCCBg
                        && u64CurTime >= pobjThis->m_u64TimeLineCCBg)
                    {
                        u64CostTime = u64CurTime - pobjThis->m_u64TimeLineCCBg;
                        pobjThis->m_u64FccTime = u64CostTime;
                        ICS_LOGW("%s:PlayerTimeCostTrace-CC:%lld ms",
                            SMEPLAYER_STAT_TAG_TIME, u64CostTime);
                    }
                    else
                    {
                        ICS_LOGW("%s:Started !", SMEPLAYER_STAT_TAG_TIME);
                    }
                    pobjThis->SetPlayerState(E_DVBPLAYER_STATE_STARTED);
                }
            }

            VOS_ThdMutexUnLock(&pobjThis->m_stCmdMutex);
            break;
        }
        case GST_MESSAGE_ERROR:
        {
            gchar *debug;
            GError *err;
            E_SME_DVBPLAYER_ERROR eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNKOWN;
            V_UINT32 u32Ret;

            gst_message_parse_error (msg, &err, &debug);
            ICS_LOGE("Debugging info:%s,Error:%s", (debug) ? debug : "none",
                err->message);

            GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pobjThis->m_pobjGstPipeline),
                GST_DEBUG_GRAPH_SHOW_ALL, "dvb.error");

            ICS_LOGE ("err.domain:%d, err.code:%d", err->domain, err->code);
            if (err->domain == GST_STREAM_ERROR)
            {
                ICS_LOGE("domain:GST_STREAM_ERROR");
                if (err->code == GST_STREAM_ERROR_FORMAT)
                {
                    ICS_LOGE("code:GST_STREAM_ERROR_FORMAT");
                    eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNSUPPORT_FORMAT;
                }
                else if (err->code == GST_STREAM_ERROR_DECODE)
                {
                    ICS_LOGE("code:GST_STREAM_ERROR_DECODE");
                    if(debug == NULL)
                     {
                         eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNSUPPORT_FORMAT;
                     }
                     else if(strstr(debug, GST_SME_STRAM_TAG_ADEC))
                     {
                         eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNSUPPORT_AUDIO_CODEC;
                     }
                     else if(strstr(debug, GST_SME_STRAM_TAG_VDEC))
                     {
                         eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNSUPPORT_VIDEO_CODEC;
                     }
                     else
                     {
                         eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNSUPPORT_FORMAT;
                     }
                }
                else if(err->code == GST_STREAM_ERROR_CODEC_NOT_FOUND)
                {
                    if(debug == NULL)
                    {
                        eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNSUPPORT_FORMAT;
                    }
                    else if(strstr(debug, GST_SME_STRAM_TAG_ADEC))
                    {
                        eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNSUPPORT_AUDIO_CODEC;
                    }
                    else if(strstr(debug, GST_SME_STRAM_TAG_VDEC))
                    {
                        eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNSUPPORT_VIDEO_CODEC;
                    }
                    else
                    {
                        eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNSUPPORT_FORMAT;
                    }
                }
                else
                {
                    eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNKOWN;
                }
            }
            else if (err->domain == GST_RESOURCE_ERROR)
            {
                ICS_LOGE("domain:GST_RESOURCE_ERROR");
                if (err->code == GST_RESOURCE_ERROR_NOT_FOUND)
                {
                    ICS_LOGE("code:GST_RESOURCE_ERROR_NOT_FOUND");
                    eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_TIME_OUT;
                }
                else if (err->code == GST_RESOURCE_ERROR_OPEN_READ)
                {
                    ICS_LOGE("code:GST_RESOURCE_ERROR_OPEN_READ");
                    eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_TIME_OUT;
                }
                else
                {
                    eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNKOWN;
                }
            }
            else if (err->domain == GST_LIBRARY_ERROR)
            {
                ICS_LOGE("domain:GST_LIBRARY_ERROR");
                eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNKOWN;
            }
            else if (err->domain == GST_CORE_ERROR)
            {
                ICS_LOGE("domain:GST_CORE_ERROR");
                eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNKOWN;
            }
            else
            {
                ICS_LOGE("unknown error domain");
                eSmeDvbPlayerError = E_SME_DVBPLAYER_ERROR_UNKOWN;
            }

            g_free (debug);
            g_error_free (err);

            if(pobjThis->m_bWaitStartedSig)
            {
                VOS_ThdMutexLock(&pobjThis->m_stCmdMutex);
                VOS_ThdCondSignal(pobjThis->m_hdlCmdCond);
                pobjThis->m_bWaitStartedSig = ICS_FALSE;
                VOS_ThdMutexUnLock(&pobjThis->m_stCmdMutex);
            }

            if(pobjThis->m_pobjPlayerObs)
            {
                u32Ret = pobjThis->m_pobjPlayerObs->PlayerNotify(
                    E_SME_DVBPLAYER_NOTIFY_ERROR, eSmeDvbPlayerError, 0);
                UTILS_MLOGW_IF(u32Ret != ICS_SUCCESS, ("call PlayerNotify fail."));
            }
            break;
        }
        case GST_MESSAGE_WARNING:
        {
            gchar *debug = NULL;
            GError *err = NULL;

            gst_message_parse_warning(msg, &err, &debug);
            ICS_LOGW("Debugging info:%s,Error:%s", (debug) ? debug : "none",
                err->message);
            ICS_LOGW ("err.domain:%d, err.code:%d", err->domain, err->code);
            if(err && err->code == GST_STREAM_ERROR_CODEC_NOT_FOUND)
            {
                if(debug && strstr(debug, GST_SME_STRAM_TAG_ADEC))
                {
                    ICS_LOGW("unsupport audio decoder !");
                    if(pobjThis->m_pobjPlayerObs)
                    {
                        V_UINT32 u32Ret = pobjThis->m_pobjPlayerObs->PlayerNotify(
                            E_SME_DVBPLAYER_NOTIFY_ERROR,
                            E_SME_DVBPLAYER_ERROR_UNSUPPORT_AUDIO_CODEC, 0);
                        UTILS_MLOGW_IF(u32Ret != ICS_SUCCESS, ("call PlayerNotify fail."));
                    }
                }
                else if(debug && strstr(debug, GST_SME_STRAM_TAG_VDEC))
                {
                    ICS_LOGW("unsupport video decoder !");
                    if(pobjThis->m_pobjPlayerObs)
                    {
                        V_UINT32 u32Ret = pobjThis->m_pobjPlayerObs->PlayerNotify(
                            E_SME_DVBPLAYER_NOTIFY_ERROR,
                            E_SME_DVBPLAYER_ERROR_UNSUPPORT_VIDEO_CODEC, 0);
                        UTILS_MLOGW_IF(u32Ret != ICS_SUCCESS, ("call PlayerNotify fail."));
                    }
                }
            }

            if(err)
            {
                g_error_free (err);
            }

            if(debug)
            {
                g_free (debug);
            }

            break;
        }
        case GST_MESSAGE_RESOLUTION_CHANGED:
        {
            gulong width, height;
            gst_message_parse_resolution_changed(msg, &width, &height);
            ICS_LOGI("the resolution has changed: %d * %d, msgsrc is %s",
                      width, height, GST_MESSAGE_SRC_NAME(msg));
            pobjThis->m_u32ResWidth = width;
            pobjThis->m_u32ResHeight = height;
            break;
        }
        case GST_MESSAGE_HAVE_NO_STREAM:
        {
            V_UINT32 u32Ret;
            ICS_LOGI("tsdmx_have_no_stream come to bus_call of impl");
            if(pobjThis->m_pobjPlayerObs)
            {
                u32Ret = pobjThis->m_pobjPlayerObs->PlayerNotify(
                    E_SME_DVBPLAYER_NOTIFY_HAVE_NO_STREAM, 0, 0);
                UTILS_MLOGW_IF(u32Ret != ICS_SUCCESS, ("call PlayerNotify fail."));
            }
            break;
        }
        case GST_MESSAGE_STREAM_RECOVER:
        {
            V_UINT32 u32Ret;
            ICS_LOGI("tsdmx_stream_recover come to bus_call of impl");
            if(pobjThis->m_pobjPlayerObs)
            {
                u32Ret = pobjThis->m_pobjPlayerObs->PlayerNotify(
                    E_SME_DVBPLAYER_NOTIFY_STREAM_RECOVER, 0, 0);
                UTILS_MLOGW_IF(u32Ret != ICS_SUCCESS, ("call PlayerNotify fail."));
            }
            break;
        }
        case GST_MESSAGE_ELEMENT:
        {
            const GstStructure *s = gst_message_get_structure (msg);
            if (gst_structure_get_name (s) && !strcmp (gst_structure_get_name (s),
                "missing-plugin"))
            {
               const GValue *value;
               const GstCaps* pCaps;
               gchar* str_caps;
               const gchar* pType;
               const gchar* pName;

               ICS_LOGI("GST_MESSAGE_ELEMENT: missing-plugin");

               pType = gst_structure_get_string (s, "type");
               if(pType && strstr(pType, "decoder"))
               {
                   ICS_LOGI("type:%s",pType);

                   value = gst_structure_get_value(s, "detail");
                   if(value)
                   {
                       pCaps = gst_value_get_caps(value);
                       if (pCaps)
                       {
                           str_caps = gst_caps_to_string((const GstCaps *) pCaps);
                           if (str_caps)
                           {
                               if (strstr(str_caps, "audio/"))
                               {
                                   ICS_LOGW("unsupport audio decoder,caps:%s!", str_caps);
                                    if(pobjThis->m_pobjPlayerObs)
                                    {
                                        V_UINT32 u32Ret = pobjThis->m_pobjPlayerObs->PlayerNotify(
                                            E_SME_DVBPLAYER_NOTIFY_ERROR,
                                            E_SME_DVBPLAYER_ERROR_UNSUPPORT_AUDIO_CODEC, 0);
                                        UTILS_MLOGW_IF(u32Ret != ICS_SUCCESS, ("call PlayerNotify fail."));
                                    }
                               }
                               else if (strstr(str_caps, "video/"))
                               {
                                   ICS_LOGW("unsupport video decoder,caps:%s!",str_caps);
                                   if(pobjThis->m_pobjPlayerObs)
                                   {
                                       V_UINT32 u32Ret = pobjThis->m_pobjPlayerObs->PlayerNotify(
                                           E_SME_DVBPLAYER_NOTIFY_ERROR,
                                           E_SME_DVBPLAYER_ERROR_UNSUPPORT_VIDEO_CODEC, 0);
                                       UTILS_MLOGW_IF(u32Ret != ICS_SUCCESS, ("call PlayerNotify fail."));
                                   }
                               }
                               g_free(str_caps);
                           }
                       }
                   }
                   pName = gst_structure_get_string (s, "name");
                   if(pName)
                   {
                       ICS_LOGI("caps description:%s",pName);
                   }
               }
            }
            break;
        }

        default:
            ICS_LOGI("bus_call drop msg=%d", GST_MESSAGE_TYPE (msg));
            break;
    }

    ICS_LOGI("bus_call out, msg=%#x, typename:%s", GST_MESSAGE_TYPE (msg),
        GST_MESSAGE_TYPE_NAME(msg));

    return TRUE;
}

SmeDvbPlayerImpl::SmeDvbPlayerImpl(E_DVB_TYPE eDvbType /*= E_DVB_TYPE_LIVE*/)
:m_pobjPlayerObs(NULL)
,m_eVDispMode(E_SME_DVBPLAYER_DIS_MODE_FULL)
,m_eState(E_DVBPLAYER_STATE_UNINIT)
,m_pobjGstPipeline(NULL)
,m_eLogLevel(E_SME_DVBPLAYER_LOG_LEVEL_WARNING)
,m_hdlCmdCond(NULL)
,m_u32BusWatchId(0)
,m_pstMainloop(NULL)
,m_hdlRunMainLoopThd(VOS_INVALIDE_THD_HANDLE)
,m_bWaitStartedSig(ICS_FALSE)
,m_eDisplayMode(E_SME_DVBPLAYER_DIS_MODE_FULL)
,m_bNeedSetProgMediaInfo(ICS_FALSE)
#ifdef  SMEPLAYER_BENCHMARK_OPEN
,m_objBenchMark()
#endif
,m_u64StartTime(0)
,m_u64FccTime(0)
,m_u32ResWidth(0)
,m_u32ResHeight(0)
,m_eFreezeMode(E_DVB_FREEZE_MODE_BLACK)
,m_eDvbType(eDvbType)
,m_u64TimeLineStartBg(M_SME_UINT64_MAX)
,m_u64TimeLineCCBg(M_SME_UINT64_MAX)
{
    ICS_LOGW("%s:SmeDvbPlayerImpl in, this=%p", SMEPLAYER_STAT_TAG_TIME, this);

    E_SME_MEM_SECURE_RET eMemRet;
    V_UINT32 u32Ret;
    GstBus* pstPipelineBus = NULL;
    m_u64TimeLineStartBg= VOS_GetTimeTicks64();

#ifdef SME_RLS
    m_eLogLevel = E_SME_DVBPLAYER_LOG_LEVEL_ERROR;
#endif

    ICS_SetLogLevel(SMEDVBLogLevelToICSLogLevel(m_eLogLevel));
    gst_debug_set_default_threshold (SmeDVBLogLevelToGstLogLevel(m_eLogLevel));

    eMemRet = VOS_Memset_S(&m_stVDispRect, sizeof(m_stVDispRect), 0,
        sizeof(m_stVDispRect));
    UTILS_MLOGW_IF(eMemRet != E_SME_MEM_OK, ("call VOS_Memset_S fail."));

    eMemRet = VOS_Memset_S(&m_stDVBProgMediaInfo, sizeof(m_stDVBProgMediaInfo), 0,
        sizeof(m_stDVBProgMediaInfo));
    UTILS_MLOGW_IF(eMemRet != E_SME_MEM_OK, ("call VOS_Memset_S fail."));

    u32Ret = VOS_InitThdMutex(&m_stCmdMutex, NULL);
    UTILS_MLOGW_IF(u32Ret != E_VOS_THD_ERR_NONE, ("call VOS_InitThdMutex fail."));

    do{
        m_hdlCmdCond = VOS_CreateThdCond(NULL);
        UTILS_MLOGE_BRK_IF(NULL == m_hdlCmdCond,
            ("VOS_CreateThdCond is null why ?"));

        ICS_LOGI("make smedvbplaybin begin ...");
        m_pobjGstPipeline = gst_element_factory_make (M_SME_DVBPLAYBIN_NAME, NULL);
        UTILS_MLOGE_BRK_IF(NULL == m_pobjGstPipeline,
            ("cannot make:%s, miss plugin?", M_SME_DVBPLAYBIN_NAME));
#ifdef  SMEPLAYER_BENCHMARK_OPEN
        u32Ret = E_SME_PLAYER_ERROR_NONE;
        u32Ret = (V_UINT32)(m_objBenchMark.SME_CreateBenchMark(m_pobjGstPipeline, this, E_DVB_PLAYER));
        UTILS_MLOGW_IF(E_SME_PLAYER_ERROR_NONE != u32Ret, ("Create benchmark failed !"));
#endif
        pstPipelineBus = gst_element_get_bus(m_pobjGstPipeline);
        UTILS_MLOGE_BRK_IF(NULL == pstPipelineBus,
            ("%s:not find bus, why ?", M_SME_DVBPLAYBIN_NAME));

        m_u32BusWatchId = gst_bus_add_watch (pstPipelineBus, bus_call, this);
        UTILS_MLOGE_BRK_IF(0 == m_u32BusWatchId,
                    ("%s:add watch failed why ?", M_SME_DVBPLAYBIN_NAME));

        m_pstMainloop = g_main_loop_new (NULL, (V_BOOL)ICS_FALSE);
        UTILS_MLOGE_BRK_IF(NULL == m_pstMainloop,
            ("%s:new mainloop failed, why ?", M_SME_DVBPLAYBIN_NAME));

        u32Ret = VOS_CreateThread(&m_hdlRunMainLoopThd, NULL, RunMainLoopThd, this);
        UTILS_MLOGW_IF(E_VOS_THD_ERR_NONE != u32Ret, ("call VOS_CreateThread failed."));

        UTILS_MLOGE_BRK_IF(VOS_INVALIDE_THD_HANDLE == m_hdlRunMainLoopThd,
             ("%s:create RunMainLoopThd failed, why ?", M_SME_DVBPLAYBIN_NAME));

        SetPlayerState(E_DVBPLAYER_STATE_INITED);

        setDispRatio(m_stDispRatio, DISP_RATIO_16TO9_WIDTH, DISP_RATIO_16TO9_HIGTH);
    }while(ICS_FALSE);

    if(pstPipelineBus)
    {
        gst_object_unref (pstPipelineBus);
    }

    ICS_LOGW("%s:SmeDvbPlayerImpl out, this=%p", SMEPLAYER_STAT_TAG_TIME, this);
}

//lint -e1551  //Function may throw exception '...' in destructor '
SmeDvbPlayerImpl::~SmeDvbPlayerImpl()
{
    ICS_LOGW("%s:~SmeDvbPlayerImpl in, this=%p, watchid=%u",
        SMEPLAYER_STAT_TAG_TIME, this, m_u32BusWatchId);

    /** BEGIN: Added for DTS2016070607972  2016/07/06 */
    Reset();
    /** END: Added for DTS2016070607972  2016/07/06 */

    if(NULL != m_pstMainloop)
    {
        ICS_LOGI("~SmeDvbPlayerImpl, this=%p,wait mainloop running...", this);

        while(NULL != m_hdlRunMainLoopThd
            && !g_main_loop_is_running(m_pstMainloop))
        {
            VOS_MSleep(1);
        }

        ICS_LOGI("~SmeDvbPlayerImpl, this=%p, mainloop quit begin ...", this);

        g_main_loop_quit(m_pstMainloop);

        if(NULL != m_hdlRunMainLoopThd)
        {
            ICS_LOGI("~SmeDvbPlayerImpl, this=%p, join RunMainLoopThd begin...",
                this);

            VOS_JoinThread(m_hdlRunMainLoopThd);
            m_hdlRunMainLoopThd = NULL;

            ICS_LOGI("~SmeDvbPlayerImpl, this=%p, join RunMainLoopThd end",
            this);
        }

        g_main_loop_unref (m_pstMainloop);
        m_pstMainloop = NULL;
    }

    if(m_u32BusWatchId != 0)
    {
        g_source_remove (m_u32BusWatchId);
        m_u32BusWatchId = 0;
    }

    if(NULL != m_pobjGstPipeline)
    {
        g_object_unref (m_pobjGstPipeline);
        m_pobjGstPipeline = NULL;
    }

    VOS_DeInitThdMutex(&m_stCmdMutex);
    VOS_DestroyThdCond(m_hdlCmdCond);
    m_hdlCmdCond = NULL;

    m_pobjPlayerObs = NULL;

    ICS_LOGW("%s:~SmeDvbPlayerImpl out, this=%p", SMEPLAYER_STAT_TAG_TIME, this);
    ICS_FlushLogBuff();
}
//lint +e1551

V_UINT32 SmeDvbPlayerImpl::SetObs(IN const ISmeDvbPlayerObs* pobjObs)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGI("SetObs in, this=%p, pobjObs=%p, m_pobjPlayerObs=%p",
        this, pobjObs, m_pobjPlayerObs);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if(NULL == m_pobjPlayerObs)
    {
        m_pobjPlayerObs = const_cast<ISmeDvbPlayerObs*>(pobjObs);
        u32Ret = ICS_SUCCESS;
    }
    else if(m_pobjPlayerObs != pobjObs)
    {
        ICS_LOGE("already setobs, cannot set again.this=%p, "
            "pobjObs=%p, m_pobjPlayerObs=%p",
             this, pobjObs, m_pobjPlayerObs);
    }

    VOS_ThdMutexUnLock(&m_stCmdMutex);

    ICS_LOGI("SetObs out, this=%p, u32Ret=%u, m_pobjPlayerObs=%p",
        this, u32Ret, m_pobjPlayerObs);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::SetVDispRect(IN const ST_SME_DVBDISP_RECT& rstDispRect)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("%s:SetVDispRect in, this=%p, [%d:%d:%d:%d]", SMEPLAYER_STAT_TAG_TIME,
        this, rstDispRect.i32StartX, rstDispRect.i32StartY,
        rstDispRect.i32Width, rstDispRect.i32Height);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if(m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        m_stVDispRect = rstDispRect;
        g_object_set(m_pobjGstPipeline, GST_SME_PROP_KEY_VDISP_RECT,
            (gulong)&m_stVDispRect, NULL);
        u32Ret = ICS_SUCCESS;
    }

    VOS_ThdMutexUnLock(&m_stCmdMutex);

    ICS_LOGW("%s:SetVDispRect out, this=%p, [%d:%d:%d:%d], ret:%u",
        SMEPLAYER_STAT_TAG_TIME, this, m_stVDispRect.i32StartX,
        m_stVDispRect.i32StartY, m_stVDispRect.i32Width, m_stVDispRect.i32Height,
         u32Ret);

    return u32Ret;
}

V_UINT32
SmeDvbPlayerImpl::SetVFreezeMode(IN E_DVB_FREEZE_MODE eMode)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGI("%s in, this=%p, mode:%d", __FUNCTION__, this, eMode);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if(m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        m_eFreezeMode = eMode;
        GST_INFO("set freeze mode to palybin : %u, in inmpl", m_eFreezeMode);
        g_object_set(m_pobjGstPipeline, GST_SME_PROP_KEY_VIDEO_FREEZE_MODE,
            (guint)m_eFreezeMode, NULL);
        u32Ret = ICS_SUCCESS;
    }

    VOS_ThdMutexUnLock(&m_stCmdMutex);
    ICS_LOGI("%s out, this=%p", __FUNCTION__, this);

    return u32Ret;
}


gboolean SmeDvbPlayerImpl::FlushPipeline()
{
    gboolean bret = FALSE;
    GstEvent* pEvtFlushStart;
    GstEvent* pEvtFlushStop;
    GstElementClass* pipelineClass = GST_ELEMENT_GET_CLASS(
        GST_ELEMENT(m_pobjGstPipeline));

    ICS_LOGW("FlushPipeline in:%p", pipelineClass);

    if(NULL == pipelineClass)
    {
        ICS_LOGE("get pipeline class failed !");
        return FALSE;
    }

    pEvtFlushStart = gst_event_new_flush_start();
    if(NULL != pEvtFlushStart)
    {
        bret = pipelineClass->send_event(GST_ELEMENT(m_pobjGstPipeline), pEvtFlushStart);
        ICS_LOGW("FlushPipeline send flush start end,bret=%d", bret);
        pEvtFlushStop = gst_event_new_flush_stop((V_BOOL)ICS_TRUE);
        if(NULL != pEvtFlushStop)
        {
            bret = pipelineClass->send_event(GST_ELEMENT(m_pobjGstPipeline), pEvtFlushStop);
            ICS_LOGW("FlushPipeline send flush stop end,bret=%d", bret);
        }
        else
        {
            bret = ICS_FALSE;
            ICS_LOGE("FlushPipeline, new flush-stop failed, why?");
        }
    }
    else
    {
        bret = ICS_FALSE;
        ICS_LOGE("FlushPipeline, new flush-start failed, why?");
    }

    ICS_LOGW("FlushPipeline out");

    return bret;
}

V_UINT32 SmeDvbPlayerImpl::SetProgMediaInfo(
    IN const ST_DVBProgMediaInfo& rstProgMediaInfo)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("%s:SetProgMediaInfo in, this=%p,demux_id[%d],pcrpid[%d],vpid[%d],"
        "vtype[%d], apid[%d],atype[%d],audio_is_ca[%d],video_is_ca[%d],m_eState=%d",
        SMEPLAYER_STAT_TAG_TIME,
        this, rstProgMediaInfo.demux_id, rstProgMediaInfo.pcrpid,
        rstProgMediaInfo.vpid, rstProgMediaInfo.e_vtype,
        rstProgMediaInfo.apid, rstProgMediaInfo.e_atype,
        rstProgMediaInfo.audio_is_ca, rstProgMediaInfo.video_is_ca,
        m_eState);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if(m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        g_object_set(m_pobjGstPipeline, GST_SME_PROP_KEY_DVB_MEDIA_INFO,
            (gpointer)&rstProgMediaInfo, NULL);
        m_stDVBProgMediaInfo = rstProgMediaInfo;
        m_bNeedSetProgMediaInfo = ICS_FALSE;
        u32Ret = ICS_SUCCESS;
    }

    VOS_ThdMutexUnLock(&m_stCmdMutex);

    SetPlayerState(E_DVBPLAYER_STATE_SRCSET);

    ICS_LOGW("%s:SetProgMediaInfo out, this=%p, u32Ret=%d",
        SMEPLAYER_STAT_TAG_TIME, this, u32Ret);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::SetProgAudioInfo(
    IN const ST_DVBProgAudioInfo& rstProgAudioInfo)
{
    V_UINT32 u32Ret = ICS_FAIL;
    ST_DVBProgMediaInfo *pstCheckInfo = NULL;

    ICS_LOGW("SetProgAudio in, this=%p,apid[%d],atype[%d],is_ca[%d],m_eState=%d",
        this,
        rstProgAudioInfo.apid, rstProgAudioInfo.e_atype,
        rstProgAudioInfo.is_ca, m_eState);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if(m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        g_object_set(m_pobjGstPipeline, GST_SME_PROP_KEY_DVB_AUDIO_INFO,
            (gpointer)&rstProgAudioInfo, NULL);

        m_stDVBProgMediaInfo.apid = rstProgAudioInfo.apid;
        m_stDVBProgMediaInfo.e_atype = rstProgAudioInfo.e_atype;
        m_stDVBProgMediaInfo.audio_is_ca = rstProgAudioInfo.is_ca;

        g_object_get(m_pobjGstPipeline, GST_SME_PROP_KEY_DVB_MEDIA_INFO,
                     &pstCheckInfo, NULL);

        if (pstCheckInfo &&
            pstCheckInfo->apid == rstProgAudioInfo.apid)
        {

            ICS_LOGI("set apid=%d, get apid=%d",rstProgAudioInfo.apid,
                     pstCheckInfo->apid);
            u32Ret = ICS_SUCCESS;
        }
        else
          u32Ret = ICS_FAIL;
    }

    VOS_ThdMutexUnLock(&m_stCmdMutex);

    ICS_LOGW("SetProgAudioInfo out, this=%p, u32Ret=%d",this, u32Ret);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::PrepareAsync()
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("%s:PrepareAsync in, this=%p, m_eState = %d", SMEPLAYER_STAT_TAG_TIME,
        this, m_eState);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if (E_DVBPLAYER_STATE_RESETTING == GetPlayerState()) {
        ICS_LOGI("Resetting, could not prepare async");
        goto done;
    }

    if(m_eState >= E_DVBPLAYER_STATE_SRCSET)
    {
        SetPlayerState(E_DVBPLAYER_STATE_PREPARED);
        VOS_ThdMutexUnLock(&m_stCmdMutex);
        /** BEGIN: Added for DTS2016070601853  2016/07/06 */
        if (m_pobjPlayerObs) {
        /** END: Added for DTS2016070601853 2016/07/06 */
            u32Ret = m_pobjPlayerObs->PlayerNotify(
                E_SME_DVBPLAYER_NOTIFY_PREPARED, 0, 0);
            UTILS_MLOGW_IF(u32Ret != ICS_SUCCESS, ("call PlayerNotify fail."));
        }
        VOS_ThdMutexLock(&m_stCmdMutex);
        u32Ret = ICS_SUCCESS;
    }

done:
    VOS_ThdMutexUnLock(&m_stCmdMutex);

    ICS_LOGW("%s:PrepareAsync out, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, m_eState);

    return u32Ret;

}

V_UINT32 SmeDvbPlayerImpl::Prepare()
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("%s:Prepare in, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if (GetPlayerState() == E_DVBPLAYER_STATE_RESETTING) {
        ICS_LOGI("Resetting, could not prepare async");
        goto done;
    }

    if(m_eState >= E_DVBPLAYER_STATE_SRCSET)
    {
        SetPlayerState(E_DVBPLAYER_STATE_PREPARED);
        u32Ret = ICS_SUCCESS;
    }

done:
    VOS_ThdMutexUnLock(&m_stCmdMutex);

    ICS_LOGW("%s:Prepare out, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::Pause()
{
    V_UINT32 u32Ret = ICS_SUCCESS;
    GstStateChangeReturn eRet = GST_STATE_CHANGE_SUCCESS;

    ICS_LOGW("%s:Pause in, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if (m_eDvbType == E_DVB_TYPE_LIVE)
    {
        ICS_LOGW("Live TV, do not support pause!!");
        u32Ret = ICS_FAIL;
        goto done;
    }

    if (GetPlayerState() == E_DVBPLAYER_STATE_RESETTING) {
        ICS_LOGI("Resetting, could not start");
        goto done;
    }

    if(m_eState >= E_DVBPLAYER_STATE_STARTED)
    {
        eRet = gst_element_set_state(GST_ELEMENT(m_pobjGstPipeline),
            GST_STATE_PAUSED);

        if(GST_STATE_CHANGE_FAILURE == eRet)
        {
            u32Ret = ICS_FAIL;
        }
        else
        {
	        ICS_LOGI("set state started");
	        SetPlayerState(E_DVBPLAYER_STATE_PAUSED);
        }
    }

done:
    VOS_ThdMutexUnLock(&m_stCmdMutex);

    ICS_LOGW("%s:Pause out, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::Resume()
{
    V_UINT32 u32Ret = ICS_SUCCESS;
    GstStateChangeReturn eRet = GST_STATE_CHANGE_SUCCESS;

    ICS_LOGW("%s:Resume in, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if (m_eDvbType == E_DVB_TYPE_LIVE)
    {
        ICS_LOGW("Live TV, do not support resume!!");
        u32Ret = ICS_FAIL;
        goto done;
    }

    if (GetPlayerState() == E_DVBPLAYER_STATE_RESETTING) {
        ICS_LOGI("Resetting, could not start");
        goto done;
    }

    if(m_eState == E_DVBPLAYER_STATE_PAUSED)
    {
        eRet = gst_element_set_state(GST_ELEMENT(m_pobjGstPipeline),
            GST_STATE_PLAYING);

        if(GST_STATE_CHANGE_FAILURE == eRet)
        {
            u32Ret = ICS_FAIL;
        }
        else
        {
            ICS_LOGI("set state started");
            SetPlayerState(E_DVBPLAYER_STATE_STARTED);
        }
    }

done:
    VOS_ThdMutexUnLock(&m_stCmdMutex);
    ICS_LOGW("%s:Resume out, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::SeekTo(V_UINT64 seekTime)
{
    V_UINT32 u32Ret = ICS_SUCCESS;

    ICS_LOGW("%s:SeekTo in, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState);

    if (m_eDvbType == E_DVB_TYPE_LIVE)
    {
        ICS_LOGW("Live TV, do not support SeekTo!!");
        u32Ret = ICS_FAIL;
        goto done;
    }

    if (GetPlayerState() == E_DVBPLAYER_STATE_RESETTING) {
        ICS_LOGI("Resetting, could not SeekTo");
        goto done;
    }

    if(m_eState >= E_DVBPLAYER_STATE_PREPARED
        && m_eState <= E_DVBPLAYER_STATE_STOPPED)
    {
        u32Ret = Stop();
        u32Ret |= Start();
    }

done:
    ICS_LOGW("%s:SeekTo out, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState);
    return u32Ret;
}


V_UINT32 SmeDvbPlayerImpl::Start()
{
    V_UINT32 u32Ret = ICS_FAIL;
    GstStateChangeReturn eRet = GST_STATE_CHANGE_SUCCESS;
    V_UINT64 u64CurTime = 0;
    V_UINT64 u64CostTime = 0;

    ICS_LOGW("%s:Start in, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if (GetPlayerState() == E_DVBPLAYER_STATE_RESETTING) {
        ICS_LOGI("Resetting, could not start");
        goto done;
    }

    if(m_eState >= E_DVBPLAYER_STATE_PREPARED)
    {
        if(m_bNeedSetProgMediaInfo)
        {
            g_object_set(m_pobjGstPipeline, GST_SME_PROP_KEY_DVB_MEDIA_INFO,
                (gpointer)&m_stDVBProgMediaInfo, NULL);
            m_bNeedSetProgMediaInfo = ICS_FALSE;
        }

        SetPlayerState(E_DVBPLAYER_STATE_STARTING);
        eRet = gst_element_set_state(GST_ELEMENT(m_pobjGstPipeline),
            GST_STATE_PLAYING);

        if(GST_STATE_CHANGE_FAILURE == eRet)
        {
            u32Ret = ICS_FAIL;
        }
        else if (GST_STATE_CHANGE_ASYNC == eRet)
        {
            //wait for playing state
            ICS_LOGI("start timewait begin ...");
            m_bWaitStartedSig = ICS_TRUE;
            u32Ret = VOS_ThdCondTimeWait(m_hdlCmdCond, &m_stCmdMutex,
                M_SME_DVBPLAYER_START_TIMEOUT);
            ICS_LOGI("start timewait end, u32Ret=%d", u32Ret);

            if(E_VOS_THD_ERR_COND_TIMEOUT == u32Ret)
            {
                ICS_LOGW("start timewait, timeout");
                u32Ret  = E_SME_DVBPLAYER_ERROR_TIME_OUT;
            }
            else
            {
                m_bWaitStartedSig = ICS_FALSE;
                u32Ret  = ICS_SUCCESS;
            }
        }
        else
        {
            u32Ret  = ICS_SUCCESS;
        }
    }

    if(ICS_SUCCESS == u32Ret)
    {
        ICS_LOGI("set state started");
        SetPlayerState(E_DVBPLAYER_STATE_STARTED);
    }
done:
    VOS_ThdMutexUnLock(&m_stCmdMutex);

    u64CurTime = VOS_GetTimeTicks64();

    if(M_SME_UINT64_MAX != m_u64TimeLineStartBg && u64CurTime >= m_u64TimeLineStartBg)
    {
        u64CostTime = u64CurTime - m_u64TimeLineStartBg;
        ICS_LOGW("%s:PlayerTimeCostTrace-Start:%lld ms,Start out!"
            "this=%p, m_eState = %d, u32Ret:%u",
            SMEPLAYER_STAT_TAG_TIME, u64CostTime, this, m_eState, u32Ret);
    }
    else if(M_SME_UINT64_MAX != m_u64TimeLineCCBg && u64CurTime >= m_u64TimeLineCCBg)
    {
        u64CostTime = u64CurTime - m_u64TimeLineCCBg;
        ICS_LOGW("%s:PlayerTimeCostTrace-CC:%lld ms,Start out!"
            "this=%p, m_eState = %d, u32Ret:%u",
            SMEPLAYER_STAT_TAG_TIME, u64CostTime, this, m_eState, u32Ret);
    }
    else
    {
        ICS_LOGW("%s:Start out, this=%p, m_eState = %d, u32Ret:%u",
            SMEPLAYER_STAT_TAG_TIME, this, m_eState, u32Ret);
    }

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::StartAsync()
{
    V_UINT32 u32Ret = ICS_FAIL;
    GstStateChangeReturn eRet = GST_STATE_CHANGE_SUCCESS;

    ICS_LOGW("%s:StartAsync in, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if(m_eState >= E_DVBPLAYER_STATE_PREPARED)
    {
        if(m_bNeedSetProgMediaInfo)
        {
            g_object_set(m_pobjGstPipeline, GST_SME_PROP_KEY_DVB_MEDIA_INFO,
                (gpointer)&m_stDVBProgMediaInfo, NULL);
            m_bNeedSetProgMediaInfo = ICS_FALSE;
        }

        SetPlayerState(E_DVBPLAYER_STATE_STARTING);
        eRet = gst_element_set_state(GST_ELEMENT(m_pobjGstPipeline),
            GST_STATE_PLAYING);

        if(GST_STATE_CHANGE_FAILURE == eRet)
        {
            u32Ret = ICS_FAIL;
        }
        else if (GST_STATE_CHANGE_ASYNC == eRet)
        {
            if(m_bWaitStartedSig)
            {
                VOS_ThdCondSignal(m_hdlCmdCond);
                m_bWaitStartedSig = ICS_FALSE;
            }

            u32Ret  = E_SME_DVBPLAYER_ERROR_TIME_OUT;
        }
        else
        {
            u32Ret  = ICS_SUCCESS;
        }
    }

    if(ICS_SUCCESS == u32Ret)
    {
        ICS_LOGW("set state started");
        SetPlayerState(E_DVBPLAYER_STATE_STARTED);
    }

    VOS_ThdMutexUnLock(&m_stCmdMutex);

    ICS_LOGW("%s:StartAsync out, this=%p, m_eState = %d, u32Ret:%u",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState, u32Ret);

    return ICS_SUCCESS;
}

//todo, if starting, stop call, have problem ???
V_UINT32 SmeDvbPlayerImpl::Stop(E_DVB_CC_MODE eCCMode/* = E_DVB_CC_MODE_LAST*/)
{
    V_UINT32 u32Ret = ICS_FAIL;
    GstStateChangeReturn eRet = GST_STATE_CHANGE_SUCCESS;
    gboolean bRet = ICS_FALSE;
    V_UINT32 u32OldCCMode;

    m_u64TimeLineCCBg = VOS_GetTimeTicks64();

    ICS_LOGW("%s:Stop in, this=%p, m_eState = %d, eCCMode = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState, eCCMode);

    VOS_ThdMutexLock(&m_stCmdMutex);
    m_u64TimeLineStartBg = M_SME_UINT64_MAX;

    if (GetPlayerState() == E_DVBPLAYER_STATE_RESETTING) {
        ICS_LOGI("Resetting, stop return");
        goto done;
    }

    if(m_eState >= E_DVBPLAYER_STATE_STARTING)
    {
        bRet = FlushPipeline();
        UTILS_MLOGW_IF(!bRet, ("call FlushPipeline fail."));

        if(m_bWaitStartedSig)
        {
            VOS_ThdCondSignal(m_hdlCmdCond);
            m_bWaitStartedSig = ICS_FALSE;
        }

        g_object_get(m_pobjGstPipeline,
            GST_SME_PROP_KEY_SWITCH_CHANNEL_MODE, &u32OldCCMode, NULL);
        if (u32OldCCMode != (guint)eCCMode) {
            g_object_set(m_pobjGstPipeline,
                GST_SME_PROP_KEY_SWITCH_CHANNEL_MODE, (guint)eCCMode, NULL);
        }
        ICS_LOGI("set to pause begin ...");
        eRet = gst_element_set_state(GST_ELEMENT(m_pobjGstPipeline),
            GST_STATE_PAUSED);
        ICS_LOGI("set to pause end, ret=%s",
            gst_element_state_change_return_get_name(eRet));

        if(GST_STATE_CHANGE_FAILURE == eRet)
        {
            u32Ret = ICS_FAIL;
        }
        else
        {
            u32Ret  = ICS_SUCCESS;
            //just E_DVBPLAYER_STATE_PAUSINIG;
            SetPlayerState(E_DVBPLAYER_STATE_STOPPED);
            m_bNeedSetProgMediaInfo = ICS_TRUE;
        }
    }
    else if(m_eState < E_DVBPLAYER_STATE_STARTING
        && m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        u32Ret  = ICS_SUCCESS;
    }

done:
    VOS_ThdMutexUnLock(&m_stCmdMutex);

    ICS_LOGW("%s:Stop out, this=%p, m_eState = %d, u32Ret:%d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState, u32Ret);

    return u32Ret;

}

V_UINT32 SmeDvbPlayerImpl::SetPace(V_UINT32 pace)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGI("SetPace in, pace:%d, m_eState = %d", pace, m_eState);
    VOS_ThdMutexLock(&m_stCmdMutex);
    if (m_eDvbType == E_DVB_TYPE_LIVE)
    {
        ICS_LOGW("Live TV, do not support SetPace!!");
        u32Ret = ICS_FAIL;
        goto done;
    }

    if(m_eState == E_DVBPLAYER_STATE_STARTED)
    {
        if (pace == 1)
        {
            ICS_LOGI("enable av sync");
            g_object_set(G_OBJECT(m_pobjGstPipeline),
                GST_SME_PROP_KEY_VID_SYNC,
                TRUE, NULL);
        }
        else
        {
            ICS_LOGI("disable av sync");
            g_object_set(G_OBJECT(m_pobjGstPipeline),
                GST_SME_PROP_KEY_VID_SYNC,
                FALSE, NULL);
        }
        u32Ret = ICS_SUCCESS;
    }

done:
    VOS_ThdMutexUnLock(&m_stCmdMutex);
    ICS_LOGI("SetPace out, u32Ret:%d",u32Ret);
    return u32Ret;
}


V_UINT32 SmeDvbPlayerImpl::Reset()
{
    V_UINT32 u32Ret = ICS_FAIL;
    GstStateChangeReturn eRet = GST_STATE_CHANGE_SUCCESS;
    E_SME_MEM_SECURE_RET eMemRet;

    ICS_LOGW("%s:Reset in, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if(m_eState > E_DVBPLAYER_STATE_INITED)
    {
        if(m_bWaitStartedSig)
        {
            VOS_ThdCondSignal(m_hdlCmdCond);
            m_bWaitStartedSig = ICS_FALSE;
        }

        SetPlayerState(E_DVBPLAYER_STATE_RESETTING);
        g_object_set(G_OBJECT(m_pobjGstPipeline),
            GST_SME_PROP_KEY_SWITCH_CHANNEL_MODE,
            E_DVB_CC_MODE_BLACK, NULL);
        eRet = gst_element_set_state(GST_ELEMENT(m_pobjGstPipeline),
            GST_STATE_NULL);

        ICS_LOGI("set state to NULL eRet:%d", eRet);

        if(GST_STATE_CHANGE_FAILURE == eRet)
        {
            u32Ret = ICS_FAIL;
        }
        else
        {
            u32Ret  = ICS_SUCCESS;
            SetPlayerState(E_DVBPLAYER_STATE_INITED);
        }
    }
    else
    {
        u32Ret  = ICS_SUCCESS;
    }

    m_bNeedSetProgMediaInfo = ICS_FALSE;
    m_u64TimeLineStartBg = M_SME_UINT64_MAX;
    m_u64TimeLineCCBg = M_SME_UINT64_MAX;
    m_pobjPlayerObs = NULL;
    setDispRatio(m_stDispRatio, DISP_RATIO_16TO9_WIDTH, DISP_RATIO_16TO9_HIGTH);
    /** BEGIN: Added for DTS2016082905668  2016/09/02 */
    eMemRet = VOS_Memset_S(&m_stDVBProgMediaInfo, sizeof(m_stDVBProgMediaInfo), 0,
        sizeof(m_stDVBProgMediaInfo));
    UTILS_MLOGW_IF(eMemRet != E_SME_MEM_OK, ("call VOS_Memset_S fail."));
    /** END: Added for DTS2016082905668  2016/09/02 */
    VOS_ThdMutexUnLock(&m_stCmdMutex);
#ifdef  SMEPLAYER_BENCHMARK_OPEN
    m_objBenchMark.SME_DestroyBenchMark();
#endif

    ICS_LOGW("%s:Reset out, this=%p, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState);
    ICS_FlushLogBuff();

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::SetProgCaInfo(
    IN const ST_DVBProgCAInfo& rProgCAInfo)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("%s:SetProgCaInfo in, this=%p, m_eState = %d,"
        "vendor_id[%d],audio_ca_type[%d],audio_stream_desc[%d],"
        "video_ca_type[%d],video_stream_desc[%d]",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState,
        rProgCAInfo.ca_vendor_id, rProgCAInfo.audio_ca_type,
        rProgCAInfo.audio_stream_descramble_type,
        rProgCAInfo.video_ca_type, rProgCAInfo.video_stream_descramble_type);

    if(m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        VOS_ThdMutexLock(&m_stCmdMutex);
        g_object_set(m_pobjGstPipeline, GST_SME_PROP_KEY_DVB_CA_INFO,
            (gpointer)&rProgCAInfo, NULL);
        VOS_ThdMutexUnLock(&m_stCmdMutex);
        u32Ret = ICS_SUCCESS;
    }

    ICS_LOGW("%s:SetProgCaInfo out, this=%p, ret:%u",
        SMEPLAYER_STAT_TAG_TIME, this, u32Ret);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::SetProgKeyInfo(IN E_DVB_CA_KEY_ID ekeyId,
    IN const ST_CAKeyParam& rKeyInfo)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("%s:SetProgKeyInfo in, this=%p, ekeyId = %d, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, ekeyId, m_eState);

    if(m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        VOS_ThdMutexLock(&m_stCmdMutex);

        switch(ekeyId)
        {
            case E_DVB_CA_SET_AUDIO_ODD_KEY:
            {
                g_object_set(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_DVB_AUDIO_ODD_KEY, (gpointer)&rKeyInfo, NULL);
                u32Ret = ICS_SUCCESS;
                break;
            }
            case E_DVB_CA_SET_AUDIO_EVEN_KEY:
            {
                g_object_set(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_DVB_AUDIO_EVEN_KEY, (gpointer)&rKeyInfo, NULL);
                u32Ret = ICS_SUCCESS;
                break;
            }
            case E_DVB_CA_SET_VIDEO_ODD_KEY:
            {
                g_object_set(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_DVB_VIDEO_ODD_KEY, (gpointer)&rKeyInfo, NULL);
                u32Ret = ICS_SUCCESS;
                break;
            }
            case E_DVB_CA_SET_VIDEO_EVEN_KEY:
            {
                g_object_set(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_DVB_VIDEO_EVEN_KEY, (gpointer)&rKeyInfo, NULL);
                u32Ret = ICS_SUCCESS;
                break;
            }
            default:
                ICS_LOGW("unknown ca key info id:%d", ekeyId);
                break;
        }

        VOS_ThdMutexUnLock(&m_stCmdMutex);
    }

    ICS_LOGW("%s:SetProgKeyInfo out, this=%p, m_eState=%d, ret:%u",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState, u32Ret);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::SetProgKeyIV(IN E_DVB_CA_KEY_IV_ID ekeyIvId,
    IN const ST_CAIVParam& rKeyIV)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("%s:SetProgKeyIV in, this=%p, ekeyIvId = %d, m_eState = %d",
        SMEPLAYER_STAT_TAG_TIME, this, ekeyIvId, m_eState);

    if(m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        VOS_ThdMutexLock(&m_stCmdMutex);

        switch(ekeyIvId)
        {
            case E_DVB_CA_SET_AUDIO_ODD_KEY_IV:
            {
                g_object_set(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_DVB_AUDIO_ODD_KEY_IV,
                    (gpointer)&rKeyIV, NULL);
                u32Ret = ICS_SUCCESS;
                break;
            }
            case E_DVB_CA_SET_AUDIO_EVEN_KEY_IV:
            {
                g_object_set(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_DVB_AUDIO_EVEN_KEY_IV,
                    (gpointer)&rKeyIV, NULL);
                u32Ret = ICS_SUCCESS;
                break;
            }
            case E_DVB_CA_SET_VIDEO_ODD_KEY_IV:
            {
                g_object_set(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_DVB_VIDEO_ODD_KEY_IV,
                    (gpointer)&rKeyIV, NULL);
                u32Ret = ICS_SUCCESS;
                break;
            }
            case E_DVB_CA_SET_VIDEO_EVEN_KEY_IV:
            {
                g_object_set(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_DVB_VIDEO_EVEN_KEY_IV,
                    (gpointer)&rKeyIV, NULL);
                u32Ret = ICS_SUCCESS;
                break;
            }
            default:
                ICS_LOGW("unknown ca key IV id:%d", ekeyIvId);
                break;
        }

        VOS_ThdMutexUnLock(&m_stCmdMutex);
    }

    ICS_LOGW("%s:SetProgKeyIV out, this=%p, m_eState=%d, ret:%u",
        SMEPLAYER_STAT_TAG_TIME, this, m_eState, u32Ret);

    return u32Ret;
}

E_SME_DVBPLAYER_STATE SmeDvbPlayerImpl::GetDvbPlayerState()
{

    E_SME_DVBPLAYER_STATE eState;

    switch (m_eState)
    {
        case E_DVBPLAYER_STATE_STARTED:
            eState = E_SME_DVBPLAYER_STATE_PLAYING;
            break;
        case E_DVBPLAYER_STATE_PAUSED:
            eState = E_SME_DVBPLAYER_STATE_PAUSE;
            break;
        case E_DVBPLAYER_STATE_STOPPED:
        case E_DVBPLAYER_STATE_PREPARED:
        case E_DVBPLAYER_STATE_PREPARING:
            eState = E_SME_DVBPLAYER_STATE_STOP;
            break;
        default:
            eState = E_SME_DVBPLAYER_STATE_STOP;
            break;
    }

    return eState;
}


SmeDvbPlayerImpl::E_DVBPLAYER_STATE SmeDvbPlayerImpl::GetPlayerState()const
{
   return m_eState;
}


V_VOID SmeDvbPlayerImpl::SetPlayerState(E_DVBPLAYER_STATE eState)
{
    E_SME_DVBPLAYER_STATE eOldSmeDVBState, eNewSmeDVBState;
    V_UINT32 u32Ret;

    eOldSmeDVBState = GetDvbPlayerState();
    m_eState = eState;
    eNewSmeDVBState = GetDvbPlayerState();

    ICS_LOGI("SetPlayerState in, this=%p, old State = %d, new state = %d",
        this, eOldSmeDVBState, eNewSmeDVBState);
    /** BEGIN: Added for DTS2016070800098  2016/07/06 */
    if (eOldSmeDVBState != eNewSmeDVBState && m_pobjPlayerObs)
    {
    /** END: Added for DTS2016070800098  2016/07/06 */
        u32Ret = m_pobjPlayerObs->PlayerNotify(
            E_SME_DVBPLAYER_NOTIFY_PLAYER_STATE_CHANGED,
            eOldSmeDVBState, eNewSmeDVBState);
        UTILS_MLOGW_IF(u32Ret != ICS_SUCCESS, ("call PlayerNotify fail."));
    }

    ICS_LOGI("SetPlayerState out, this=%p, old State = %d, new state = %d",
             this, eOldSmeDVBState, eNewSmeDVBState);

    return;
}

V_INT32 SmeDvbPlayerImpl::IsPlaying()
{
    ICS_LOGI("IsPlaying, this=%p, m_eState = %d", this, m_eState);

    return (V_INT32)(m_eState == E_DVBPLAYER_STATE_STARTED);
}

V_UINT32
SmeDvbPlayerImpl::SetDisplayMode(IN E_SME_DVBPLAYER_DISPLAY_MODE eMode)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGI("%s in, this=%p, mode:%d", __FUNCTION__, this, eMode);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if(eMode != E_SME_DVBPLAYER_DIS_MODE_FULL && eMode != E_SME_DVBPLAYER_DIS_MODE_FITIN)
    {
        ICS_LOGE("video display mode should be fitin or full, now is %u", (V_UINT32)eMode);
        VOS_ThdMutexUnLock(&m_stCmdMutex);
        return ICS_FAIL;
    }

    if(m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        m_eDisplayMode = eMode;
        g_object_set(m_pobjGstPipeline, GST_SME_PROP_KEY_DISP_MODE,
            (guint)m_eDisplayMode, NULL);
        u32Ret = ICS_SUCCESS;
    }

    VOS_ThdMutexUnLock(&m_stCmdMutex);
    ICS_LOGI("%s out, this=%p", __FUNCTION__, this);

    return u32Ret;
}

V_UINT32
SmeDvbPlayerImpl::GetDisplayMode(OUT E_SME_DVBPLAYER_DISPLAY_MODE& reMode)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGI("%s in, this=%p", __FUNCTION__, this);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if(m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        g_object_get(m_pobjGstPipeline,
        GST_SME_PROP_KEY_DISP_MODE, &reMode, NULL);
        u32Ret = ICS_SUCCESS;
    }

    VOS_ThdMutexUnLock(&m_stCmdMutex);
    ICS_LOGI("%s out, this=%p", __FUNCTION__, this);

    return u32Ret;
}

V_UINT32
SmeDvbPlayerImpl::GetDisplayRatio(OUT ST_DispRatio& rstDispRatio)
{
    V_UINT32 u32Ret = ICS_FAIL;
    ST_DispRatio* pstDispRatioTmp = NULL;

    ICS_LOGI("%s in, this=%p", __FUNCTION__, this);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if(m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        g_object_get(m_pobjGstPipeline,
        GST_SME_PROP_KEY_DISP_RADIO, &pstDispRatioTmp, NULL);
        if(NULL != pstDispRatioTmp){
            rstDispRatio.u32DispRatioW = pstDispRatioTmp->u32DispRatioW;
            rstDispRatio.u32DispRatioH = pstDispRatioTmp->u32DispRatioH;
            u32Ret = ICS_SUCCESS;
        }
        else
        {
            ICS_LOGE("get disp ratio failed in wrap");
        }
    }

    VOS_ThdMutexUnLock(&m_stCmdMutex);
    ICS_LOGI("%s out, this=%p", __FUNCTION__, this);

    return u32Ret;
}


V_UINT32
SmeDvbPlayerImpl::GetDisplayRect(OUT ST_SME_DVBDISP_RECT& rstDispRect)
{
    V_UINT32 u32Ret = ICS_FAIL;
    ST_SME_DVBDISP_RECT* pstDispRectTmp = NULL;

    ICS_LOGI("%s in, this=%p", __FUNCTION__, this);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if(m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        g_object_get(m_pobjGstPipeline,
        GST_SME_PROP_KEY_VDISP_RECT, &pstDispRectTmp, NULL);
        if(NULL != pstDispRectTmp){
            rstDispRect.i32StartX = pstDispRectTmp->i32StartX;
            rstDispRect.i32StartY = pstDispRectTmp->i32StartY;
            rstDispRect.i32Width = pstDispRectTmp->i32Width;
            rstDispRect.i32Height = pstDispRectTmp->i32Height;
            u32Ret = ICS_SUCCESS;
        }
        else
        {
            ICS_LOGE("get disp rect failed in wrap");
        }
    }

    VOS_ThdMutexUnLock(&m_stCmdMutex);
    ICS_LOGI("%s out, this=%p", __FUNCTION__, this);

    return u32Ret;
}


V_UINT32
SmeDvbPlayerImpl::SetDisplayRatio(IN ST_DispRatio* pstRatio)
{
    V_UINT32 u32Ret = ICS_FAIL;
    V_FLOAT fRatio;

    ICS_LOGI("%s in, this=%p, ra", __FUNCTION__, this);

    VOS_ThdMutexLock(&m_stCmdMutex);

    if(pstRatio == NULL || pstRatio->u32DispRatioH == 0)
    {
        printf("the u32DispRatioH ratio is 0,it si wrong,so we skip it\n");
        VOS_ThdMutexUnLock(&m_stCmdMutex);
        return ICS_FAIL;
    }
    fRatio = (V_FLOAT)pstRatio->u32DispRatioW / (V_FLOAT)pstRatio->u32DispRatioH;
    if(fRatio != ((V_FLOAT)16 / (V_FLOAT)9) && fRatio != ((V_FLOAT)5 / (V_FLOAT)4) &&
        fRatio != ((V_FLOAT)4 / (V_FLOAT)3) && fRatio != ((V_FLOAT)3 / (V_FLOAT)2))
    {
        printf("when we set Fitin, the ratio must be 16/9,5/4,4/3,3/2, now ratio is :[%u,%u],it is wrong ,we skip it\n",
            pstRatio->u32DispRatioW, pstRatio->u32DispRatioH);
        VOS_ThdMutexUnLock(&m_stCmdMutex);
        return ICS_FAIL;
    }

    setDispRatio(m_stDispRatio, pstRatio->u32DispRatioW, pstRatio->u32DispRatioH);

    if(m_eState >= E_DVBPLAYER_STATE_INITED)
    {
        ICS_LOGI("set Ratio: w(%d), h(%d)", pstRatio->u32DispRatioW,
            pstRatio->u32DispRatioH);
        g_object_set(m_pobjGstPipeline, GST_SME_PROP_KEY_DISP_RADIO,
            (gpointer)&m_stDispRatio, NULL);

        u32Ret = ICS_SUCCESS;
    }

    VOS_ThdMutexUnLock(&m_stCmdMutex);
    ICS_LOGI("%s out, this=%p", __FUNCTION__, this);;

    return u32Ret;
}

#ifdef __LINUX__
V_UINT32 SmeDvbPlayerImpl::SetAudioMute(IN const V_UINT32 mute)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("SetAudioMute in, this=%p, mute = %u, m_eState = %d",
        this, mute, m_eState);

    if(m_eState >= E_DVBPLAYER_STATE_STARTED && m_eState <= E_DVBPLAYER_STATE_PAUSED)
    {
        VOS_ThdMutexLock(&m_stCmdMutex);

        g_object_set(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_MUTE_AUDIO, mute, NULL);
        u32Ret = ICS_SUCCESS;

        VOS_ThdMutexUnLock(&m_stCmdMutex);
    }

    ICS_LOGW("SetAudioMute out, this=%p, m_eState=%d, ret:%u",
        this, m_eState, u32Ret);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::GetAudioMute(INOUT V_UINT32*mute)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("GetAudioMute in, this=%p, m_eState = %d",
        this, m_eState);

    if (NULL == mute) {
        return u32Ret;
    }

    if(m_eState >= E_DVBPLAYER_STATE_STARTED && m_eState <= E_DVBPLAYER_STATE_PAUSED)
    {
        VOS_ThdMutexLock(&m_stCmdMutex);

        g_object_get(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_MUTE_AUDIO, mute, NULL);
        u32Ret = ICS_SUCCESS;

        VOS_ThdMutexUnLock(&m_stCmdMutex);
    }

    ICS_LOGW("GetAudioMute out, this=%p, m_eState=%d, mute = %u, ret:%u",
        this, m_eState, *mute, u32Ret);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::SetAudioVolume(IN const V_UINT32 volume)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("SetAudioVolume in, this=%p, volume = %u, m_eState = %d",
        this, volume, m_eState);

    if(m_eState >= E_DVBPLAYER_STATE_STARTED && m_eState <= E_DVBPLAYER_STATE_PAUSED)
    {
        VOS_ThdMutexLock(&m_stCmdMutex);

        g_object_set(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_AUDIO_VOLUME, volume, NULL);
        u32Ret = ICS_SUCCESS;

        VOS_ThdMutexUnLock(&m_stCmdMutex);
    }

    ICS_LOGW("SetAudioVolume out, this=%p, m_eState=%d, ret:%u",
        this, m_eState, u32Ret);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::GetAudioVolume(INOUT V_UINT32*volume)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("GetAudioVolume in, this=%p, m_eState = %d",
        this, m_eState);

    if (NULL == volume) {
        return u32Ret;
    }

    if(m_eState >= E_DVBPLAYER_STATE_STARTED && m_eState <= E_DVBPLAYER_STATE_PAUSED)
    {
        VOS_ThdMutexLock(&m_stCmdMutex);

        g_object_get(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_AUDIO_VOLUME, volume, NULL);
        u32Ret = ICS_SUCCESS;

        VOS_ThdMutexUnLock(&m_stCmdMutex);
    }

    ICS_LOGW("GetAudioVolume out, this=%p, m_eState=%d, volume = %u, ret:%u",
        this, m_eState, *volume, u32Ret);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::SetAudioChannelMode(IN const A_TRACK_CHANNEL_MODE_E mode)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("SetAudioChannelMode in, this=%p, volume = %u, m_eState = %d",
        this, mode, m_eState);

    if(m_eState >= E_DVBPLAYER_STATE_STARTED && m_eState <= E_DVBPLAYER_STATE_PAUSED)
    {
        VOS_ThdMutexLock(&m_stCmdMutex);

        g_object_set(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_AUDIO_CHANNEL_MODE, mode, NULL);
        u32Ret = ICS_SUCCESS;

        VOS_ThdMutexUnLock(&m_stCmdMutex);
    }

    ICS_LOGW("SetAudioChannelMode out, this=%p, m_eState=%d, ret:%u",
        this, m_eState, u32Ret);

    return u32Ret;
}

V_UINT32 SmeDvbPlayerImpl::GetAudioChannelMode(INOUT A_TRACK_CHANNEL_MODE_E*mode)
{
    V_UINT32 u32Ret = ICS_FAIL;

    ICS_LOGW("GetAudioChannelMode in, this=%p, m_eState = %d",
        this, m_eState);

    if (NULL == mode) {
        return u32Ret;
    }

    if(m_eState >= E_DVBPLAYER_STATE_STARTED && m_eState <= E_DVBPLAYER_STATE_PAUSED)
    {
        VOS_ThdMutexLock(&m_stCmdMutex);

        g_object_get(m_pobjGstPipeline,
                    GST_SME_PROP_KEY_AUDIO_CHANNEL_MODE, mode, NULL);
        u32Ret = ICS_SUCCESS;

        VOS_ThdMutexUnLock(&m_stCmdMutex);
    }

    ICS_LOGW("GetAudioChannelMode out, this=%p, m_eState=%d, mode = %u, ret:%u",
        this, m_eState, *mode, u32Ret);

    return u32Ret;
}
#endif

#ifdef  SMEPLAYER_BENCHMARK_OPEN
V_VOID SmeDvbPlayerImpl::UpdateDvbProcInfo(ST_PROC_COMMON_INFO * pstCommonPointer)
{
    UTILS_M_RET_IF(NULL == pstCommonPointer);

    pstCommonPointer->DvbPlayerState = GetDvbPlayerState();
    pstCommonPointer->start_cost_time = m_u64StartTime;
    pstCommonPointer->fcc_cost_time = m_u64FccTime;
    pstCommonPointer->resolution_width = m_u32ResWidth;
    pstCommonPointer->resolution_height = m_u32ResHeight;
}
#endif
