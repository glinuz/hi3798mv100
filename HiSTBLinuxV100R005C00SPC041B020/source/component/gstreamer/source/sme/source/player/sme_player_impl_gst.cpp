/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_player_impl_gst.cpp
 * @brief   sme player gst implement
 * @author
 * @date    2014/4/6
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/4/6
 * Author :
 * Desc   : Created file
 *
******************************************************************************/
#define MOD_NAME "SmePlayerImplGst"
#include <string.h>
#include "osal_type.h"
#include "sme_macro.h"
#include "sme_log.h"
#include "osal_mem.h"
#include "osal_str.h"
#include "sme_player_gst_wrap.h"
#include "sme_player_impl_gst.h"

#define SME_URI_PROTO_HTTP_PREFIX           "http://"
#define SME_URI_PROTO_HTTPS_PREFIX          "https://"
#define SME_URI_PROTO_RTSP_PREFIX           "rtsp://"
#define SME_URI_PROTO_RTP_PREFIX            "rtp://"
#define SME_URI_PROTO_HLS_SUFFIX            ".m3u8"

#define SME_BUFFER_LOW_PERCENT              1
#define SME_BUFFER_HIGH_PERCENT             4
#define SME_BUFFER_TIME_DEFAULT             5000//5s
#define SME_BUFFER_MAX_TIME                 20000//20S
#define SME_BUFFER_THREHOKD_DOWN            (SME_BUFFER_TIME_DEFAULT * SME_BUFFER_LOW_PERCENT / 100)
#define SME_BUFFER_THREHOLD_UP_MIN          (SME_BUFFER_TIME_DEFAULT * SME_BUFFER_HIGH_PERCENT/ 100)
//#define SME_BUFFER_THREHOLD_UP_RATIO_MAX    50
#define SME_CACHE_PROC_INTERVAL             100//ms

//#define SME_BUFFER_THREHOLD_UP_CHK_TIME     3000//ms
//#define SME_BUFFER_THREHOLD_NORMAL          (V_UINT32)(SME_BUFFER_MAX*4.0/5)
#define SME_VDECOUT_INTERVAL_MAX            80//ms
#define MUTE_STATUS_OFF                     0


//lint -e1506   //Call to virtual function 'SmePlayerImplGst::Reset(void)' within a constructor or destructor
//lint -e1579   //Pointer member 'SmePlayerImplGst::m_*** might have been freed by a separate function but no '-sem(SmePlayerImplGst::Reset,cleanup)' was seen
//lint -e1058   //Call to virtual function 'SmePlayerImplGst::Reset(void)' within a constructor or destructor
//lint -e1732   //new in constructor for class 'SmePlayerImplGst' which has no assignment operator
//lint -e1733   //new in constructor for class 'SmePlayerImplGst' which has no copy constructor
//lint -e1774   //Could use dynamic_cast to downcast polymorphic type 'SmePlayerMsgBase'
//lint -esym(423, pobjNotify)  //Creation of memory leak in assignment to 'pobjNotify'
//lint -esym(429, pobjMsg, pobjNotify, pobjMsgOnPrepared, pobjMsgOnError, pobjSeekMsg)  //has not been freed or returned
//lint -esym(593, pobjMsg, pobjMsgNew, pobjCmd,pobjMsgOnPrepared,pobjMsgOnError)  //possibly not freed or returned
//lint -esym(438,pobjPlayer, pobjMsg)  //'pobjMsg' not used
//lint -e665   //Unparenthesized parameter 2 in macro 'UTILS_MLOGE_RET_VAL_IF' is passed an expression
//lint -e717 //do{}while(0)
//lint -e731   //Boolean argument to equal/not equal
//lint -e774   //Boolean within 'if' always evaluates to False
//lint -esym(838, pobjMsg, u32Ret, i32SeekToMs, u32Err)  //not used, return value
//lint -e1025
//lint -e1703
//lint -esym(40, NULL)
//lint -esym(593, pobjNotify)
//lint -esym(672, pobjNotify)
//lint -e582
//lint -e838
//lint -e438
//lint -e1055
//lint -e10
//lint -e864

#define SME_MAIN_VER ""SME_VERSION" [ "__DATE__" "__TIME__" ]"

#define URI_MAX_LENGTH 512

/*static*/
const V_CHAR* ISmePlayer::GetSmeVersionInfo()
{
    return SME_MAIN_VER;
}

#if 0
static V_UINT32 GetSmeBufferMin(V_UINT32 u32Bw, V_UINT32 u32StreamBps,
    V_UINT64& u64LastTime, V_UINT32 u32OldBuffMin)
{
    V_UINT32 u32Ret = SME_BUFFER_THREHOLD_UP_MIN*SME_BUFFER_THREHOLD_UP_RATIO_MAX;

    if(u32Bw >= (u32StreamBps << 1))
    {
        u32Ret = SME_BUFFER_THREHOLD_UP_MIN;
        //ICS_LOGD("network is perfect!");
    }
    else if(u32Bw >= (u32StreamBps * 3) / 2)
    {
        u32Ret = SME_BUFFER_THREHOLD_UP_MIN << 1;
        //ICS_LOGD("network is good!");
    }
    else if(u32Bw > u32StreamBps)
    {
        u32Ret = SME_BUFFER_THREHOLD_UP_MIN << 2;
        ICS_LOGD("network is just so so!");
    }
    else
    {
       ICS_LOGD("network is bad!");//max
    }

    if(M_SME_UINT32_MAX == u32StreamBps)
    {
        u64LastTime = M_SME_UINT64_MAX;
    }
    else
    {
        //Ã¿Ãë¸üÐÂ¡£
        if((M_SME_UINT64_MAX != u64LastTime) && (VOS_GetTimeTicks64()
            < u64LastTime + SME_BUFFER_THREHOLD_UP_CHK_TIME))
        {
            u32Ret = u32OldBuffMin;
        }
        else
        {
            u64LastTime = VOS_GetTimeTicks64();
        }
    }

    return u32Ret;
}
#endif

//ISmePlayer
ISmePlayer* ISmePlayer::CreateSmePlayer()
{
    return new SmePlayerImplGst();
}

V_VOID ISmePlayer::DestroySmePlayer(INOUT ISmePlayer * pobjPlayer)
{
    UTILS_MSAFEDEL(pobjPlayer);

    return;
}

//SmePlayerImplGst
SmePlayerImplGst::SmePlayerImplGst()
:m_pobjPlayerObs(NULL)
,m_pszURI(NULL)
,m_pvSurface(NULL)
,m_eVDispMode(E_SME_PLAYER_DIS_MODE_FULL)
,m_eVDispAngle(E_SME_PLAYER_DIS_ANGLE_0)
,m_eState(E_PLAYER_STATE_UNINIT)
,m_pobjCmdRunner(NULL)
,m_pvGstPipeline(NULL)
,m_pobjMsgRunner(NULL)
,m_eLogLevel(E_SME_PLAYER_LOG_LEVEL_INFO)
,m_pszPlgFullPath(NULL)
,m_bCacheCtrlEnable(ICS_FALSE)
,m_bBuffering(ICS_FALSE)
,m_u32BufferFull(SME_BUFFER_TIME_DEFAULT)
,m_u32BufferThdDown(SME_BUFFER_THREHOKD_DOWN)
,m_u32BufferThdUp(SME_BUFFER_THREHOLD_UP_MIN)
,m_u64LastUpdateThdUpTime(M_SME_UINT64_MAX)
,m_i32SeekTo(-1)
,m_mute_status(MUTE_STATUS_OFF)
,m_objTimer("SmeCacheTimer", CacheTimerProcFunc, static_cast<V_VOID*>(this))
,m_bPreParedNormal(ICS_FALSE)
,m_bSeeking(ICS_FALSE)
,m_bPausedInternal(ICS_FALSE)
,m_i32Duration(0)
,m_pszSubURI(NULL)
,m_u32Bandwidth(M_SME_UINT32_MAX)
,m_u32BufferLowPercent(SME_BUFFER_LOW_PERCENT)
,m_u32BufferHighPercent(SME_BUFFER_HIGH_PERCENT)
,m_u32NotifyPercent(0)
#ifdef __ENABLE_OTTOPT__
,m_pobjMsgChnApp(NULL)
,m_pobjCacheQuery(NULL)
,m_pobjCacheQueryObs(NULL)
,m_bIsConnected(ICS_FALSE)
#endif
,m_bLoop(ICS_FALSE)
,m_bLoopSeeking(ICS_FALSE)
#ifdef  SMEPLAYER_BENCHMARK_OPEN
,m_objBenchMark()
#endif
,m_procCommonInfo(NULL)
,m_u32ResolutionWidth(0)
,m_u32ResolutionHeight(0)
,m_u64StarTime(0)
,m_u64ResetTime(0)
,m_u64PrepareTime(0)
,m_u64FccTime(0)
,m_u64SeekTime(0)
{
    V_UINT32 u32Err = ICS_SUCCESS;
    ICS_LOGD("SmePlayerImplGst construct in");
    struct timeval tmp;
    gettimeofday(&tmp, NULL);
    m_u64StarTime = (V_UINT64)((V_INT64)(tmp.tv_sec) * 1000 + ((V_INT64)(tmp.tv_usec) + 500) / 1000);

    m_stDispRatio.u32DispRatioW = 0;
    m_stDispRatio.u32DispRatioH = 0;

    m_stVDispRect.i32StartX = -1;
    m_stVDispRect.i32StartY = -1;
    m_stVDispRect.i32Width = -1;
    m_stVDispRect.i32Height = -1;

    if (E_SME_MEM_OK != VOS_Memset_S(
        &m_stSubCb,
        sizeof(m_stSubCb),
        0,
        sizeof(m_stSubCb)))
    {
        ICS_LOGE("SECURE: VOS_Memset_S return failed.");
    }

    do
    {
        u32Err = VOS_InitThdMutex(&m_stMutex, NULL);
        UTILS_MLOGW_IF(ICS_SUCCESS != u32Err, ("Init thd mutex fail."));

        m_pobjCmdRunner = new SmePlayerMsgRunner(this);
        UTILS_MLOGE_BRK_VAL_IF(NULL == m_pobjCmdRunner, u32Err, ICS_FAIL,
            ("new failed"));

        u32Err = m_pobjCmdRunner->Init();
        UTILS_MLOGE_BRK_IF(ICS_SUCCESS != u32Err, ("init cmd runner failed"));

        m_pobjMsgRunner = new SmePlayerMsgRunner(this);
        UTILS_MLOGE_BRK_VAL_IF(NULL == m_pobjMsgRunner, u32Err, ICS_FAIL,
            ("new failed"));

        u32Err = m_pobjMsgRunner->Init();
        UTILS_MLOGE_BRK_IF(ICS_SUCCESS != u32Err, ("init msg runner failed"));

        /** BEGIN: Added for DTS2014101707996 by lizimian(l00251625), 2014/10/25 */
        /*
        MsgChannelLauncher* pobjMsgChnLauncher = new MsgChannelLauncher(this);
        UTILS_MLOGE_BRK_VAL_IF(NULL == pobjMsgChnLauncher, u32Err, ICS_FAIL,
            ("new msg channel app lauchner failed"));

        u32Err = pobjMsgChnLauncher->Create();
        UTILS_MLOGE_BRK_IF(ICS_SUCCESS != u32Err, ("create msg channel app launcher failed"));
        */
        /** END:   Added for DTS2014101707996 by lizimian(l00251625), 2014/10/25 */
    } while (ICS_FALSE);

    if(ICS_SUCCESS != u32Err)
    {
        if(NULL != m_pobjCmdRunner)
        {
            m_pobjCmdRunner->DeInit(ICS_TRUE);
            delete m_pobjCmdRunner;
            m_pobjCmdRunner = NULL;
        }

        if(NULL != m_pobjMsgRunner)
        {
            m_pobjMsgRunner->DeInit(ICS_TRUE);
            delete m_pobjMsgRunner;
            m_pobjMsgRunner = NULL;
        }

        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_ERROR);
    }



    ICS_LOGD("SmePlayerImplGst construct out");
}

//lint -e1551  //Function may throw exception '...' in destructor '
SmePlayerImplGst::~SmePlayerImplGst()
{
    ICS_LOGI("SmePlayerImplGst deconstruct in");

    Reset();

    /* BEGIN: Modified for DTS2016042502439, 2016/5/10 */
#ifdef  SMEPLAYER_BENCHMARK_OPEN
    m_objBenchMark.SME_DestroyBenchMark();
#endif
    /* END: Modified for DTS2016042502439, 2016/5/10 */
    if(NULL != m_pvGstPipeline)
    {
        SmePlayerGstWrap::Reset(m_pvGstPipeline);
        SmePlayerGstWrap::DeInitGst(m_pvGstPipeline);
        m_pvGstPipeline = NULL;
    }

    if(NULL != m_pobjMsgRunner)
    {
        m_pobjMsgRunner->DeInit(ICS_TRUE);
        delete m_pobjMsgRunner;
        m_pobjMsgRunner = NULL;
    }

    if(NULL != m_pobjCmdRunner)
    {
        m_pobjCmdRunner->DeInit(ICS_TRUE);
        delete m_pobjCmdRunner;
        m_pobjCmdRunner = NULL;
    }

    UTILS_MSAFEFREE(m_pszURI);
    UTILS_MSAFEFREE(m_pszSubURI);
    UTILS_MSAFEFREE(m_pszPlgFullPath);
    m_pobjPlayerObs = NULL;
    m_pvSurface = NULL;


    VOS_DeInitThdMutex(&m_stMutex);


    ICS_LOGI("SmePlayerImplGst deconstruct out");

    //BEGIN:flush log to file, for DTS2015072400092, by f00165155, 2015/7/25
    ICS_FlushLogBuff();
    //END:flush log to file, for DTS2015072400092, by f00165155, 2015/7/25
}
//lint +e1551

V_UINT32 SmePlayerImplGst::SetObs(IN const ISmePlayerObs* pobjObs)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetObs* pobjMsg = NULL;

    ICS_LOGD("SetObs in, pobjObs=%p, cur state=%d", pobjObs, m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetObs(SyncCmdCompletedCb,
                                    this,
                                    pobjObs);

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("SetObs out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::SetDataSource(IN const V_CHAR* pstURI)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetDs* pobjMsg = NULL;

    ICS_LOGD("SetDataSource in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetDs(SyncCmdCompletedCb,
                                    this,
                                    pstURI);

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("SetDataSource out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::SetSurface(IN const V_VOID* pvSurface)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetSf* pobjMsg = NULL;

    ICS_LOGD("SetSurface in, pvSurface=%p, cur state=%d", pvSurface, m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetSf(SyncCmdCompletedCb,
                                    this,
                                    pvSurface);

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("SetSurface out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::SetVDispRect(IN V_INT32 i32StartX,
                            IN V_INT32 i32StartY,
                            IN V_INT32 i32Width,
                            IN V_INT32 i32Height)
{
    if(i32Width < 0 || i32Height < 0)
    {
        ICS_LOGW("the rect of windows is error:height or width is 0");
        return E_SME_PLAYER_ERROR_INVAILD_RECT;
    }
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetVDispRect* pobjMsg = NULL;

    ICS_LOGI("SetVDispRect in, rect[%d:%d:%d:%d], cur state=%d",
        i32StartX, i32StartY, i32Width, i32Height, m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetVDispRect(SyncCmdCompletedCb,
                                    this,
                                    i32StartX,
                                    i32StartY,
                                    i32Width,
                                    i32Height);
    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGI("SetVDispRect out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::PrepareAsync()
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgPrepareAsync* pobjMsg = NULL;
    struct timeval prepare_start;

    ICS_LOGD("PrepareAsync in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    gettimeofday(&prepare_start, NULL);
    m_u64PrepareTime = (V_UINT64)((V_INT64)(prepare_start.tv_sec)* 1000 + ((V_INT64)(prepare_start.tv_usec) + 500) / 1000);
    pobjMsg = new SmePlayerMsgPrepareAsync(NULL, NULL);
    u32Ret = m_pobjCmdRunner->SendMsg(pobjMsg);
    if(E_SME_PLAYER_ERROR_NONE != u32Ret)
    {
        ICS_LOGW("send PrepareAsync Failed(%#x)!", u32Ret);
        UTILS_MTMPDEL(pobjMsg);
    }

    ICS_LOGD("PrepareAsync out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::Prepare()
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgPrepare* pobjMsg = NULL;
    struct timeval prepare_start, prepare_stop;
    V_UINT64 preparestop;

    gettimeofday(&prepare_start, NULL);
    m_u64PrepareTime = (V_UINT64)((V_INT64)(prepare_start.tv_sec) * 1000 + ((V_INT64)(prepare_start.tv_usec) + 500) / 1000);
    ICS_LOGD("Prepare in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgPrepare(SyncCmdCompletedCb, this);
    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    gettimeofday(&prepare_stop, NULL);
    preparestop = (V_UINT64)((V_INT64)(prepare_stop.tv_sec) * 1000 + ((V_INT64)(prepare_stop.tv_usec) + 500) / 1000);
    m_u64PrepareTime = preparestop - m_u64PrepareTime;

    ICS_LOGD("Prepare out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::Start()
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgStart* pobjMsg = NULL;
    struct timeval tmp;
    V_UINT64 stoptime;

    if (m_u64ResetTime)
    {
        gettimeofday(&tmp, NULL);
        m_u64StarTime = (V_UINT64)((V_INT64)(tmp.tv_sec) * 1000 + ((V_INT64)(tmp.tv_usec) + 500) / 1000);
    }

    ICS_LOGD("Start in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgStart(SyncCmdCompletedCb, this);
    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    gettimeofday(&tmp, NULL);
    stoptime = (V_UINT64)((V_INT64)(tmp.tv_sec) * 1000 + ((V_INT64)(tmp.tv_usec) + 500) / 1000);
    m_u64StarTime = stoptime - m_u64StarTime;

    if (m_u64ResetTime)
        m_u64StarTime = m_u64PrepareTime + m_u64StarTime;

    if(m_u64ResetTime)
        m_u64FccTime = m_u64ResetTime + m_u64StarTime;

    ICS_LOGD("Start out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::Stop()
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgStop* pobjMsg = NULL;

    ICS_LOGI("Stop in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgStop(SyncCmdCompletedCb, this);
    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("Stop out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::Pause()
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgPause* pobjMsg = NULL;

    ICS_LOGI("Pause in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgPause(SyncCmdCompletedCb, this);
    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGI("Pause out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::Resume()
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgResume* pobjMsg = NULL;

    ICS_LOGD("Resume in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgResume(SyncCmdCompletedCb, this);
    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("Resume out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::Reset()
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgReset* pobjMsg = NULL;
    struct timeval fcctime;
    V_UINT64 tmp;

    gettimeofday(&fcctime, NULL);
    m_u64ResetTime = (V_UINT64)((V_INT64)(fcctime.tv_sec) * 1000 + ((V_INT64)(fcctime.tv_usec) + 500) / 1000);

#ifdef __ENABLE_OTTOPT__
    V_BOOL bDeinitQuery = ICS_FALSE;
#endif

    ICS_LOGD("Reset in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

#ifdef __ENABLE_OTTOPT__
    VOS_ThdMutexLock(&m_stMutex);
    if(NULL != m_pobjCacheQuery)
    {
        bDeinitQuery = ICS_TRUE;
    }
    VOS_ThdMutexUnLock(&m_stMutex);
    if(bDeinitQuery)
    {
        m_pobjCacheQuery->DeInit();
        delete m_pobjCacheQuery;
        m_pobjCacheQuery = NULL;
    }

    m_bIsConnected = ICS_FALSE;
#endif

    SmePlayerGstWrap::ExitGstBlock(m_pvGstPipeline);
    pobjMsg = new SmePlayerMsgReset(SyncCmdCompletedCb, this);
    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    gettimeofday(&fcctime, NULL);
    tmp = (V_UINT64)((V_INT64)(fcctime.tv_sec) * 1000 + ((V_INT64)(fcctime.tv_usec) + 500) / 1000);
    m_u64ResetTime = tmp- m_u64ResetTime;

    ICS_LOGI("Reset out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::SeekTo(IN V_INT32 i32Ms)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    struct timeval sktime;

    ICS_LOGD("SeekTo in, seekto:%dms, cur state=%d", i32Ms, m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));
    gettimeofday(&sktime, NULL);
    m_u64SeekTime = (V_UINT64)((V_INT64)(sktime.tv_sec) * 1000 + ((V_INT64)(sktime.tv_usec) + 500) / 1000);
    VOS_ThdMutexLock(&m_stMutex);

    ICS_LOGD("SeekTo , seekto:%dms, cur state=%d, m_i32SeekTo=%d",
        i32Ms, m_eState, m_i32SeekTo);

    if(-1 == m_i32SeekTo)
    {
        SmePlayerMsgSeekTo* pobjMsg = NULL;
        m_i32SeekTo = i32Ms;
        pobjMsg = new SmePlayerMsgSeekTo(NULL, NULL, i32Ms);
        u32Ret = m_pobjCmdRunner->SendMsg(pobjMsg);
        if(E_SME_PLAYER_ERROR_NONE != u32Ret)
        {
            ICS_LOGW("SeekTo:%d Failed(%#x)!", i32Ms, u32Ret);
            UTILS_MTMPDEL(pobjMsg);
            m_i32SeekTo = -1;
        }
    }
    else
    {
        m_i32SeekTo = i32Ms;
        u32Ret = E_SME_PLAYER_ERROR_NONE;
    }

    VOS_ThdMutexUnLock(&m_stMutex);

    ICS_LOGD("SeekTo out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_INT32 SmePlayerImplGst::GetDuration()
{
    V_INT32 i32Ret = 0;
    SmePlayerMsgGetDur* pobjMsg = NULL;

    ICS_LOGD("GetDuration in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, i32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgGetDur(SyncCmdCompletedCb, this);
    i32Ret = (V_INT32)m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("GetDuration out, i32Ret=%dms", i32Ret);

    return i32Ret;
}

V_INT32 SmePlayerImplGst::GetCurrentPosition()
{
    V_INT32 i32Ret = 0;

    ICS_LOGD("GetCurrentPosition in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, i32Ret,
        ("construct failed, any cmd cannot execute!"));

    VOS_ThdMutexLock(&m_stMutex);
    if(-1 != m_i32SeekTo)
    {
        i32Ret = m_i32SeekTo;
        VOS_ThdMutexUnLock(&m_stMutex);
    }
    else
    {
        SmePlayerMsgGetCurPos* pobjMsg = NULL;

        VOS_ThdMutexUnLock(&m_stMutex);
        pobjMsg = new SmePlayerMsgGetCurPos(SyncCmdCompletedCb, this);
        i32Ret = (V_INT32)m_pobjCmdRunner->PostMsg(pobjMsg);

        UTILS_MSAFEDEL(pobjMsg);
    }

    ICS_LOGD("GetCurrentPosition out, i32Ret=%dms", i32Ret);

    return i32Ret;
}

V_INT32 SmePlayerImplGst::IsPlaying()
{
    V_INT32 i32Ret;

    ICS_LOGD("IsPlaying in, cur state=%d", m_eState);

    /* BEGIN: Modified for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */
    VOS_ThdMutexLock(&m_stMutex);
    i32Ret = (V_INT32)((E_PLAYER_STATE_STARTED == m_eState) ? 1 : 0);
    VOS_ThdMutexUnLock(&m_stMutex);
    /* END: Modified for DTS2014091906325 by liurongliang(l00180035), 2014/9/27 */

    ICS_LOGD("IsPlaying out, i32Ret=%d", i32Ret);

    return i32Ret;

}

V_BOOL SmePlayerImplGst::IsPlayingHls()
{
    V_BOOL bRet = ICS_FALSE;

    ICS_LOGD("IsPlayingHls in");

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pszURI, bRet, ("m_pszURI is null."))
    //VOS_ThdMutexLock(&m_stMutex);
    char* seperator = m_pszURI+strlen(m_pszURI)-strlen(SME_URI_PROTO_HLS_SUFFIX);
    if(!strncasecmp(m_pszURI, SME_URI_PROTO_HTTP_PREFIX, strlen(SME_URI_PROTO_HTTP_PREFIX))
        && !strncasecmp(seperator, SME_URI_PROTO_HLS_SUFFIX, strlen(SME_URI_PROTO_HLS_SUFFIX)))
    {
        bRet = ICS_TRUE;
        ICS_LOGI("is playing hls stream.");
    }

    //VOS_ThdMutexUnLock(&m_stMutex);

    ICS_LOGD("IsPlayingHls out, bRet=%d", (V_INT32)bRet);

    return bRet;
}


V_INT32 SmePlayerImplGst::GetVideoWidth()
{
    V_INT32 i32Ret = -1;
    SmePlayerMsgGetVideoWidth* pobjMsg = NULL;

    ICS_LOGD("GetVideoWidth in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, i32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgGetVideoWidth(SyncCmdCompletedCb, this);
    i32Ret = (V_INT32)m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("GetVideoWidth out, i32Ret=%d", i32Ret);

    return i32Ret;
}

V_INT32 SmePlayerImplGst::GetVideoHeight()
{
    V_INT32 i32Ret = -1;
    SmePlayerMsgGetVideoHeight* pobjMsg = NULL;

    ICS_LOGD("GetVideoHeight in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, i32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgGetVideoHeight(SyncCmdCompletedCb, this);
    i32Ret = (V_INT32)m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("GetVideoHeight out, i32Ret=%d", i32Ret);

    return i32Ret;
}

E_SME_PLAYER_STATE SmePlayerImplGst::GetPlayerState()
{
    E_SME_PLAYER_STATE eState;

    if (m_bBuffering)
    {
        eState = E_SME_PLAYER_STATE_BUFFERING;
    }else{
        switch (m_eState)
        {
            case E_PLAYER_STATE_STARTED:
                eState = E_SME_PLAYER_STATE_PLAYING;
                break;
            case E_PLAYER_STATE_PAUSED:
                eState = E_SME_PLAYER_STATE_PAUSE;
                break;
            case E_PLAYER_STATE_PREPARED:
            case E_PLAYER_STATE_PREPARING:
                eState = E_SME_PLAYER_STATE_STOP;
                break;
            default:
                eState = E_SME_PLAYER_STATE_STOP;
                break;
        }
    }

    return eState;
}


V_UINT32 SmePlayerImplGst::SetDisplayMode(IN E_SME_PLAYER_DISPLAY_MODE eMode,
    IN const ST_DISP_RATIO* pstDispRatio)
{
    ST_DISP_RATIO stDispRatioTmp;
    stDispRatioTmp.u32DispRatioW = 0;
    stDispRatioTmp.u32DispRatioH = 0;

    if(eMode != E_SME_PLAYER_DIS_MODE_FULL && eMode != E_SME_PLAYER_DIS_MODE_FITIN)
    {
        ICS_LOGW("SetDispMode : %d, we do not support,so do nothing", eMode);
        return E_SME_PLAYER_ERROR_UNSUPPORT_DISPLAY_MODE;
    }

    if(eMode == E_SME_PLAYER_DIS_MODE_FITIN)
    {
        if(pstDispRatio == NULL)
        {
            ICS_LOGW("when mode is fitin,pstDispRatio can not be null");
            return E_SME_PLAYER_ERROR_FITIN_PARAMETER;
        }
        if(pstDispRatio->u32DispRatioH == 0)
        {
            ICS_LOGE("when we set Fitin, we should have ratio,now ratio is NULL,we skip it");
            return E_SME_PLAYER_ERROR_FITIN_PARAMETER;
        }

        V_FLOAT fRatio = (V_FLOAT)pstDispRatio->u32DispRatioW / (V_FLOAT)pstDispRatio->u32DispRatioH;
        if(fRatio != ((V_FLOAT)16 / (V_FLOAT)9) && fRatio != ((V_FLOAT)5 / (V_FLOAT)4) &&
            fRatio != ((V_FLOAT)4 / (V_FLOAT)3) && fRatio != ((V_FLOAT)3 / (V_FLOAT)2))
        {
            ICS_LOGE("when we set Fitin, the ratio must be 16/9,5/4,4/3,3/2, now ratio is :[%u,%u],it is wrong ,we skip it",
                pstDispRatio->u32DispRatioW, pstDispRatio->u32DispRatioH);
            return E_SME_PLAYER_ERROR_FITIN_PARAMETER;
        }

        stDispRatioTmp.u32DispRatioW = pstDispRatio->u32DispRatioW;
        stDispRatioTmp.u32DispRatioH = pstDispRatio->u32DispRatioH;
    }

    m_eVDispMode = eMode;
    m_stDispRatio.u32DispRatioW = stDispRatioTmp.u32DispRatioW;
    m_stDispRatio.u32DispRatioH = stDispRatioTmp.u32DispRatioH;

    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetDisplayMode* pobjMsg = NULL;

    ICS_LOGI("SetDispMode in, DispMode is : %d, DispRadio is %p ", eMode, pstDispRatio);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetDisplayMode(SyncCmdCompletedCb,
                                    this,
                                    eMode,
                                    &stDispRatioTmp);

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);
    ICS_LOGI("SetDispMode out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst:: SetChannelChangeMode (IN E_SME_PLAYER_SWITCH_CHANNEL_MODE eSwitchMode)
{
    if(eSwitchMode != E_SME_PLAYER_SWITCH_CHANNEL_MODE_BLACK &&
        eSwitchMode != E_SME_PLAYER_SWITCH_CHANNEL_MODE_LAST )
    {
        ICS_LOGW("ERROR:switchMode is not black,either last");
        return E_SME_PLAYER_ERROR_INVALID_ARGS;
    }

    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetSwitchChannelMode* pobjMsg = NULL;

    ICS_LOGI("impl_gst, SetSwitchChannelMode  is : %d ", eSwitchMode);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetSwitchChannelMode(SyncCmdCompletedCb,
                                    this,
                                    eSwitchMode);

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);
    UTILS_MSAFEDEL(pobjMsg);

    return u32Ret;

}


V_UINT32 SmePlayerImplGst::SetDisplayAngle(IN E_SME_PLAYER_DISPLAY_ANGLE eAngle)
{
    UTILS_M_INVALID_ARGS(eAngle);
    return static_cast<V_UINT32>(E_SME_PLAYER_ERROR_UNSUPPORT_OP);
}


V_UINT32 SmePlayerImplGst::GetMetadata(INOUT SmeMetadata& robjSmeMetadata)
{
    UTILS_M_INVALID_ARGS(robjSmeMetadata);
    return static_cast<V_UINT32>(E_SME_PLAYER_ERROR_UNSUPPORT_OP);
}

V_UINT32 SmePlayerImplGst::SetBitrate(IN V_UINT32 bitrate)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetBitrate* pobjMsg = NULL;


    ICS_LOGI("SetBitrate in, cur state=%d", m_eState);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetBitrate(SyncCmdCompletedCb,
                                    this,
                                    bitrate);

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("SetBitrate out, u32Ret=%u", u32Ret);

    return u32Ret;
}


V_UINT32 SmePlayerImplGst::SetUserAgent(IN const V_CHAR* pstUserAgent)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetUserAgent* pobjMsg = NULL;

    ICS_LOGI("SetUserAgent in, cur state=%d", m_eState);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetUserAgent(SyncCmdCompletedCb,
                                    this,
                                    pstUserAgent);

    UTILS_MLOGE_RET_VAL_IF(NULL == pobjMsg, u32Ret,
        ("construct pobjMsg failed!"));

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("SetUsetAgent out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::SetReferer(IN const V_CHAR* pstSetReferer)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetReferer* pobjMsg = NULL;

    ICS_LOGI("SetReferer in, cur state=%d", m_eState);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetReferer(SyncCmdCompletedCb,
                                    this,
                                    pstSetReferer);

    UTILS_MLOGE_RET_VAL_IF(NULL == pobjMsg, u32Ret,
        ("construct pobjMsg failed!"));

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("SetReferer out, u32Ret=%u", u32Ret);

    return u32Ret;
}


V_UINT32 SmePlayerImplGst::SetCookies(IN V_CHAR** pstSetCookies)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetCookies* pobjMsg = NULL;

    ICS_LOGI("SetCookies in, cur state=%d", m_eState);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetCookies(SyncCmdCompletedCb,
                                    this,
                                    pstSetCookies);

    UTILS_MLOGE_RET_VAL_IF(NULL == pobjMsg, u32Ret,
        ("construct pobjMsg failed!"));

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("SetCookies out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::SetByteRange(IN V_BOOL bByteRange)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetByteRange* pobjMsg = NULL;

    ICS_LOGI("SetByteRange in, cur state=%d", m_eState);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetByteRange(SyncCmdCompletedCb,
                                    this,
                                    bByteRange);

    UTILS_MLOGE_RET_VAL_IF(NULL == pobjMsg, u32Ret,
        ("construct pobjMsg failed!"));

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("SetByteRange out, u32Ret=%u", u32Ret);

    return u32Ret;
}


V_UINT32 SmePlayerImplGst::RegManifestCompleteCb(
    IN PFN_StreamPlayModeCb pfPlaymodeCb, IN V_VOID* pvCtx)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgRegManifestCompleteCb* pobjMsg = NULL;


    ICS_LOGI("RegisterManifestCompleteCb in, cur state=%d", m_eState);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgRegManifestCompleteCb(SyncCmdCompletedCb,
        this, pfPlaymodeCb, pvCtx);

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("RegisterManifestCompleteCb out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::RegBitrateChangedCb (
    IN PFN_BitrateChangedCb pfnBtChangedCb,
    IN V_VOID* pvCtx)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgRegBtChangedCb* pobjMsg = NULL;


    ICS_LOGI("RegBitrateChangedCb in, cur state=%d", m_eState);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgRegBtChangedCb(SyncCmdCompletedCb,
        this, pfnBtChangedCb, pvCtx);

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("RegBitrateChangedCb out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::GetBandwidth()
{
    V_UINT32 u32Ret = 0;
    SmePlayerMsgGetBandwidth* pobjMsg = NULL;

    ICS_LOGD("GetBandwidth in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgGetBandwidth(SyncCmdCompletedCb, this);
    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("GetBandwidth out, i32Ret=%d", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::UpdateProcControlInfo(ST_PROC_COMMON_INFO * pstControlPointer)
{
    V_UINT32 u32Err = ICS_SUCCESS;
    E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;

    if (!pstControlPointer)
    {
        u32Err = ICS_FAIL;
        return u32Err;
    }
    m_procCommonInfo = pstControlPointer;

    if (m_procCommonInfo)
    {
        m_procCommonInfo->download_rate = m_u32Bandwidth;
        m_procCommonInfo->PlayerState = GetPlayerState();
        m_procCommonInfo->IsPlayerLoop = m_bLoop;
        m_procCommonInfo->prepare_cost_time = m_u64PrepareTime;
        m_procCommonInfo->start_cost_time = m_u64StarTime;
        m_procCommonInfo->reset_cost_time = m_u64ResetTime;
        m_procCommonInfo->fcc_cost_time = m_u64FccTime;
        m_procCommonInfo->seek_cost_time = m_u64SeekTime;
        eMemSecRet = VOS_Memcpy_S (m_procCommonInfo->unique_url,
            URI_MAX_LENGTH - 1,
            m_pszURI,
            (V_UINT32)strlen(m_pszURI));
        if (eMemSecRet != E_SME_MEM_OK)
        {
            ICS_LOGE("VOS_Memcpy_S unique_url Failed(%d)", eMemSecRet);
        }

        u32Err = GetCurrentBitrate(&m_procCommonInfo->current_bitrate);
        if (u32Err != ICS_SUCCESS)
            ICS_LOGE("GetCurrentBitrate Failed(%d)", u32Err);

        m_procCommonInfo->resolution_width = m_u32ResolutionWidth;
        m_procCommonInfo->resolution_height = m_u32ResolutionHeight;
        m_procCommonInfo->file_duration = m_i32Duration;
        m_procCommonInfo->curposition = GetCurrentPosition();

        u32Err = GetBufferStatus(&m_procCommonInfo->buffer_status);
        if (u32Err != ICS_SUCCESS)
            ICS_LOGE("GetBufferStatus Failed(%d)", u32Err);

        u32Err = GetVideoTrackInfo(&m_procCommonInfo->video_track_info);
        if (u32Err != ICS_SUCCESS)
            ICS_LOGE("GetVideoTrackInfo Failed(%d)", u32Err);
        u32Err = GetAudioTrackInfo(&m_procCommonInfo->audio_track_info);
        if (u32Err != ICS_SUCCESS)
            ICS_LOGE("GetAudioTrackInfo Failed(%d)", u32Err);
    }
    return u32Err;
}

#ifdef __LINUX__
V_UINT32 SmePlayerImplGst::SetAudioMute(IN const V_UINT32 mute)
{
   V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
   SmePlayerMsgSetAudioMute* pobjMsg = NULL;

   ICS_LOGD("Set Audio Mute in, cur state=%d", m_eState);

   UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
       ("construct failed, any cmd cannot execute!"));

   UTILS_MLOGE_RET_VAL_IF(1 < mute, E_SME_PLAYER_ERROR_INVALID_ARGS,
       ("args mute invalid!"));

   pobjMsg = new SmePlayerMsgSetAudioMute(SyncCmdCompletedCb,
                                              this,
                                              mute);

   u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

   m_mute_status = mute;

   UTILS_MSAFEDEL(pobjMsg);
   ICS_LOGD("Set Audio Mute out, i32Ret=%d", u32Ret);

   return u32Ret;
}

V_UINT32 SmePlayerImplGst::GetAudioMute(INOUT V_UINT32*mute)
{
   V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
   SmePlayerMsgGetAudioMute* pobjMsg = NULL;

   ICS_LOGD("Get Audio Mute in, cur state=%d", m_eState);

   UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
       ("construct failed, any cmd cannot execute!"));

   UTILS_MLOGE_RET_VAL_IF(NULL == mute, E_SME_PLAYER_ERROR_INVALID_ARGS,
       ("args mute NULL!"));

   pobjMsg = new SmePlayerMsgGetAudioMute(SyncCmdCompletedCb,
                                              this,
                                              mute);

   u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);


   UTILS_MSAFEDEL(pobjMsg);
   ICS_LOGD("Get Audio Mute out, i32Ret=%d", u32Ret);

   return u32Ret;
}

V_UINT32 SmePlayerImplGst::SetAudioVolume(IN const V_UINT32 volume)
{
   V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
   SmePlayerMsgSetAudioVolume* pobjMsg = NULL;

   ICS_LOGD("Set Audio Volume in, cur state=%d", m_eState);

   UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
       ("construct failed, any cmd cannot execute!"));

   UTILS_MLOGE_RET_VAL_IF(100 < volume, E_SME_PLAYER_ERROR_INVALID_ARGS,
       ("args volume invalid!"));

   pobjMsg = new SmePlayerMsgSetAudioVolume(SyncCmdCompletedCb,
                                              this,
                                              volume);

   u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

   UTILS_MSAFEDEL(pobjMsg);
   ICS_LOGD("Set Audio Volume out, i32Ret=%d", u32Ret);

   return u32Ret;
}

V_UINT32 SmePlayerImplGst::GetAudioVolume(INOUT V_UINT32*volume)
{
   V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
   SmePlayerMsgGetAudioVolume* pobjMsg = NULL;

   ICS_LOGD("Get Audio Volume in, cur state=%d", m_eState);

   UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
       ("construct failed, any cmd cannot execute!"));

   UTILS_MLOGE_RET_VAL_IF(NULL == volume, E_SME_PLAYER_ERROR_INVALID_ARGS,
       ("args volume NULL!"));

   pobjMsg = new SmePlayerMsgGetAudioVolume(SyncCmdCompletedCb,
                                              this,
                                              volume);

   u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

   UTILS_MSAFEDEL(pobjMsg);
   ICS_LOGD("Get Audio Volume out, i32Ret=%d", u32Ret);

   return u32Ret;
}

V_UINT32 SmePlayerImplGst::SetAudioChannelMode(IN const A_TRACK_CHANNEL_MODE_E mode)
{
   V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
   SmePlayerMsgSetAudioChannelMode* pobjMsg = NULL;

   ICS_LOGD("Set Audio ChannelMode in, cur state=%d", m_eState);

   UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
       ("construct failed, any cmd cannot execute!"));

  UTILS_MLOGE_RET_VAL_IF((V_INT32)mode >= (V_INT32)A_TRACK_MODE_BUTT,
       E_SME_PLAYER_ERROR_INVALID_ARGS, ("args mode invalid!"));

   pobjMsg = new SmePlayerMsgSetAudioChannelMode(SyncCmdCompletedCb,
                                              this,
                                              mode);

   u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

   UTILS_MSAFEDEL(pobjMsg);
   ICS_LOGD("Set Audio ChannelMode out, i32Ret=%d", u32Ret);

   return u32Ret;
}

V_UINT32 SmePlayerImplGst::GetAudioChannelMode(INOUT A_TRACK_CHANNEL_MODE_E*mode)
{
   V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
   SmePlayerMsgGetAudioChannelMode* pobjMsg = NULL;

   ICS_LOGD("Get Audio ChannelMode in, cur state=%d", m_eState);

   UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
       ("construct failed, any cmd cannot execute!"));

   UTILS_MLOGE_RET_VAL_IF(NULL == mode, E_SME_PLAYER_ERROR_INVALID_ARGS,
       ("args mode NULL!"));

   pobjMsg = new SmePlayerMsgGetAudioChannelMode(SyncCmdCompletedCb,
                                              this,
                                              mode);

   u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

   UTILS_MSAFEDEL(pobjMsg);
   ICS_LOGD("Get Audio ChannelMode out, i32Ret=%d", u32Ret);

   return u32Ret;
}
#endif

V_UINT32 SmePlayerImplGst::GetVideoDispMode(INOUT E_SME_PLAYER_DISPLAY_MODE& reMode, INOUT ST_DISP_RATIO& rstRatio)
{
   V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
   SmePlayerMsgGetVideoDispMode* pobjMsg = NULL;

   ICS_LOGD("impl Get Video DispMode in, cur state=%d", m_eState);

   UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
       ("construct failed, any cmd cannot execute!"));


   pobjMsg = new SmePlayerMsgGetVideoDispMode(SyncCmdCompletedCb,
                                              this,
                                              reMode,
                                              rstRatio);

   u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);
   UTILS_MSAFEDEL(pobjMsg);
   ICS_LOGD("impl Get Video DispMode out, i32Ret=%d", u32Ret);

   return u32Ret;
}

V_UINT32 SmePlayerImplGst::GetVideoDispRect(INOUT ST_SME_DISP_RECT& rstRect)
{
   V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
   SmePlayerMsgGetVideoDispRect* pobjMsg = NULL;

   ICS_LOGD("impl Get Video DispRect in, cur state=%d", m_eState);

   UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
       ("construct failed, any cmd cannot execute!"));

   pobjMsg = new SmePlayerMsgGetVideoDispRect(SyncCmdCompletedCb,
                                              this,
                                              rstRect);

   u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

   UTILS_MSAFEDEL(pobjMsg);
   ICS_LOGD("impl Get Video DispRect out, i32Ret=%d", u32Ret);

   return u32Ret;
}

V_UINT32 SmePlayerImplGst::SetAudioHdmiMode(IN const V_CHAR *pstAudioHdmiMode)
{
   V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
   SmePlayerMsgSetAudioHdmiMode* pobjMsg = NULL;

   ICS_LOGD("Set Audio Hdmi mode in, cur state=%d", m_eState);

   UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
       ("construct failed, any cmd cannot execute!"));

   pobjMsg = new SmePlayerMsgSetAudioHdmiMode(SyncCmdCompletedCb,
                                              this,
                                              pstAudioHdmiMode);

   u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

   UTILS_MSAFEDEL(pobjMsg);
   ICS_LOGD("Set Audio Hdmi mode out, i32Ret=%d", u32Ret);

   return u32Ret;
}

V_UINT32 SmePlayerImplGst::GetAudioHdmiMode(INOUT V_CHAR **pstAudioHdmiMode)
{
   V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
   SmePlayerMsgGetAudioHdmiMode* pobjMsg = NULL;

   ICS_LOGD("Get Audio Hdmi mode in, cur state=%d", m_eState);

   UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
       ("construct failed, any cmd cannot execute!"));

   pobjMsg = new SmePlayerMsgGetAudioHdmiMode(SyncCmdCompletedCb,
                                              this,
                                              pstAudioHdmiMode);

   u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

   UTILS_MSAFEDEL(pobjMsg);
   ICS_LOGD("Get Audio Hdmi mode out, i32Ret=%d", u32Ret);

   return u32Ret;
}


V_UINT32 SmePlayerImplGst::SetBufferTime(IN V_UINT32 u32BufferingTime)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;

    ICS_LOGI("SetBufferTime in, cur state=%d,buffertime:%d", m_eState,u32BufferingTime);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_INITED >  m_eState,
      (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must set datasource first!"));

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED <  m_eState,
        (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must before prepare!"));

    if (u32BufferingTime > SME_BUFFER_MAX_TIME ||
        u32BufferingTime == 0)
    {
        ICS_LOGW("Buffer time:%d is invalid, using default:%d",
                 SME_BUFFER_MAX_TIME,SME_BUFFER_TIME_DEFAULT);
        m_u32BufferFull = SME_BUFFER_TIME_DEFAULT;
    }
    else {
        m_u32BufferFull = u32BufferingTime;
    }

    ICS_LOGI("SetBufferTime:%d, u32Ret=%u", m_u32BufferFull,u32Ret);

    return u32Ret;

}

V_UINT32 SmePlayerImplGst::SetBufferWaterMark(IN V_UINT32 u32HighPercent,
                                              IN V_UINT32 u32LowPercent)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;

    ICS_LOGI("SetBufferWaterMark in, cur state=%d", m_eState);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_INITED >  m_eState,
      (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must set datasource first!"));

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED <  m_eState,
    (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must before prepare!"));

    if (u32LowPercent > 100
        || u32HighPercent > 100
        || u32LowPercent >= u32HighPercent)
    {
        ICS_LOGW("Percent should be 0-100,using default water mark");
        m_u32BufferLowPercent = SME_BUFFER_LOW_PERCENT;
        m_u32BufferHighPercent = SME_BUFFER_HIGH_PERCENT;
    }
    else {
        m_u32BufferLowPercent = u32LowPercent;
        m_u32BufferHighPercent = u32HighPercent;
    }
    ICS_LOGI("SetBufferWaterMark, u32LowPercent=%u,u32HighPercent:%d",
             u32LowPercent,u32HighPercent);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::GetBufferStatus(INOUT ST_BUFFER_STATUS *BufferStatus)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgGetBufferStatus* pobjMsg = NULL;


    ICS_LOGI("GetBufferStatus, cur state=%d", m_eState);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgGetBufferStatus(SyncCmdCompletedCb,
                                              this,
                                              BufferStatus);

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGI("GetBufferStatus, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::GetCurrentBitrate(INOUT ST_CURRENT_BITRATE *CurrentBitrate)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgGetBitrate* pobjMsg = NULL;

    ICS_LOGD("GetCurrentBitrate in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgGetBitrate(SyncCmdCompletedCb,
                                         this,
                                         CurrentBitrate);
    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("GetCurrentBitrate out, i32Ret=%d", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::GetVideoTrackInfo(
    INOUT ST_VIDEO_TRACK_INFO *VideoTrackInfo)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgGetVideoTrackInfo* pobjMsg = NULL;

    ICS_LOGI("GetVideoTrackInfo, cur state=%d", m_eState);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgGetVideoTrackInfo(SyncCmdCompletedCb,
                                                this,
                                                VideoTrackInfo);

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGI("GetVideoTrackInfo, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::GetAudioTrackInfo(
    INOUT ST_AUDIO_TRACK_INFO *AudioTrackInfo)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgGetAudioTrackInfo* pobjMsg = NULL;


    ICS_LOGI("GetAudioTrackInfo, cur state=%d", m_eState);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgGetAudioTrackInfo(SyncCmdCompletedCb,
                                                this,
                                                AudioTrackInfo);

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGI("GetAudioTrackInfo, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::SetAudioTrack(
    IN V_INT32 i32AudioTrack)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetAudioTrack* pobjMsg = NULL;


    ICS_LOGI("GetAudioTrackInfo, cur state=%d", m_eState);
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetAudioTrack(SyncCmdCompletedCb,
                                            this,
                                            i32AudioTrack);

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGI("SetAudioTrack, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::SetProperty(IN SmeProperty& robjProp)
{
    V_UINT32                u32Ret  = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetProp*    pobjMsg = NULL;

    ICS_LOGD("SetProperty in, key=%s, cur state=%d",
        robjProp.GetStringKey(), m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetProp(SyncCmdCompletedCb,
                                    this,
                                    const_cast<SmeProperty*>(&robjProp));

    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGD("SetProperty out, u32Ret=%u", u32Ret);

    return u32Ret;
}


V_UINT32 SmePlayerImplGst::GetProperty(IN SmeProperty& robjRequest,
                            INOUT SmeProperty& robjReply)
{
    V_UINT32                u32Ret  = E_SME_PLAYER_ERROR_STATE;

    ICS_LOGD("GetProperty in, key=%s, cur state=%d",
        robjRequest.GetStringKey(), m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    VOS_ThdMutexLock(&m_stMutex);
    if(-1 == m_i32SeekTo)
    {
        SmePlayerMsgGetProp*    pobjMsg = NULL;

        VOS_ThdMutexUnLock(&m_stMutex);
        pobjMsg = new SmePlayerMsgGetProp(SyncCmdCompletedCb,
                                    this,
                                    const_cast<SmeProperty*>(&robjRequest),
                                    const_cast<SmeProperty*>(&robjReply));
        u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);
        UTILS_MSAFEDEL(pobjMsg);
    }
    else
    {
        VOS_ThdMutexUnLock(&m_stMutex);
        //u32Ret = E_SME_PLAYER_ERROR_NONE;
        ICS_LOGE("when seeking, we cannot get property", u32Ret);
    }

    ICS_LOGD("GetProperty out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::SetPlayMode(IN E_SME_PLAYER_MODE eMode)
{
    UTILS_M_INVALID_ARGS(eMode);
    return static_cast<V_UINT32>(E_SME_PLAYER_ERROR_UNSUPPORT_OP);
}


V_UINT32 SmePlayerImplGst::GetCurrentVideoFrame(INOUT V_INT32 ai32Array[],
                            IN V_INT32 i32Size,
                            IN V_INT32 i32Type)
{
    UTILS_M_INVALID_ARGS(ai32Array);
    UTILS_M_INVALID_ARGS(i32Size);
    UTILS_M_INVALID_ARGS(i32Type);
    return static_cast<V_UINT32>(E_SME_PLAYER_ERROR_UNSUPPORT_OP);
}

/*static*/
V_VOID SmePlayerImplGst::SyncCmdCompletedCb(V_VOID* pvCtx)
{
    UTILS_M_INVALID_ARGS(pvCtx);
    //do nothing here

    return;
}

V_VOID SmePlayerImplGst::FreeMsg(SmePlayerMsgBase* pobjMsg) const
{
    UTILS_MSAFEDEL(pobjMsg);

    return;
}

V_VOID SmePlayerImplGst::OnMsgComing(SmePlayerMsgBase* pobjMsg)
{
    UTILS_MLOGE_RET_IF(NULL == pobjMsg, ("Err, null cmd!"));

    //ICS_LOGI("OnMsgComing, type:%d", pobjMsg->GetType());
    if(ICS_FALSE == pobjMsg->IsNotify())
    {
        this->OnCmdComing(pobjMsg);
    }
    else
    {
        this->OnNotifyComing(pobjMsg);
    }
    //ICS_LOGI("OnMsgComing out");

    return;
}

V_UINT32 SmePlayerImplGst::HandleSetObs(const SmePlayerMsgSetObs* pobjMsg)
{
    m_pobjPlayerObs = const_cast<ISmePlayerObs*>(pobjMsg->GetPlayerObs());

    return static_cast<V_UINT32>(E_SME_PLAYER_ERROR_NONE);
}

V_UINT32 SmePlayerImplGst::HandleSetDs(const SmePlayerMsgSetDs* pobjMsg)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;

    ICS_LOGI("Begin into handlesetDS");
    do
    {

        UTILS_M_BRK_VAL_IF(E_PLAYER_STATE_INITED ==  m_eState, u32Ret,
            E_SME_PLAYER_ERROR_NONE);

        UTILS_MLOGE_BRK_VAL_IF((E_PLAYER_STATE_UNINIT !=  m_eState)
            || (NULL == m_pobjPlayerObs), u32Ret, E_SME_PLAYER_ERROR_STATE,
            ("must reset and then set None Null obs first!"));

        UTILS_MLOGE_BRK_VAL_IF(NULL == pobjMsg->GetUri(), u32Ret,
            E_SME_PLAYER_ERROR_INVALID_ARGS, ("uri is null"));

        /* BEGIN: Modified for DTS2016062207092/DTS2016062206911, 2016/6/28 */
        UTILS_MLOGE_BRK_VAL_IF(!strncasecmp (pobjMsg->GetUri(), "https", 5), u32Ret,
            E_SME_PLAYER_ERROR_INVALID_ARGS, ("https protocol is not supported"));
        /* END: Modified for DTS2016062207092/DTS2016062206911, 2016/6/28 */

        UTILS_MSAFEFREE(m_pszURI);

        m_pszURI = strdup(pobjMsg->GetUri());
        UTILS_MLOGE_RET_VAL_IF(NULL == m_pszURI, E_SME_PLAYER_ERROR_NOMEM,
                ("strdup uri failed"));

        m_bCacheCtrlEnable = SmePlayerImplGst::IsCacheCtrlEnable(
            static_cast<V_VOID*>(this), m_pszURI);
        u32Ret = SmePlayerGstWrap::InitGst(m_pvGstPipeline, GstMsgHandler, this,
            m_eLogLevel, NULL, m_pszPlgFullPath);
        UTILS_MLOGE_BRK_VAL_IF(E_SME_PLAYER_ERROR_NONE !=  u32Ret, u32Ret, u32Ret,
            ("InitGst failed(%u)!", u32Ret));

#ifdef  SMEPLAYER_BENCHMARK_OPEN
        V_VOID *pstSmePlaybin;
        pstSmePlaybin = SmePlayerGstWrap::GetPlaybin(m_pvGstPipeline);

        u32Ret = (V_UINT32)(m_objBenchMark.SME_CreateBenchMark(pstSmePlaybin, this, E_SME_PLAYER));
        UTILS_MLOGW_IF(E_SME_PLAYER_ERROR_NONE != u32Ret, ("Create benchmark failed !"));
#else
        SmePlayerGstWrap::GetPlaybin(m_pvGstPipeline);
#endif

        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_INITED);
    } while (ICS_FALSE);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::HandleSetSf(const SmePlayerMsgSetSf* pobjMsg)
{
    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_INITED !=  m_eState,
        E_SME_PLAYER_ERROR_STATE, ("must set datasource first!"));

    m_pvSurface = const_cast<V_VOID*>(pobjMsg->GetSf());

    return E_SME_PLAYER_ERROR_NONE;

}

V_UINT32 SmePlayerImplGst::HandleSetVDispRect(const SmePlayerMsgSetVDispRect* pobjMsg)
{
    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_INITED >  m_eState,
        E_SME_PLAYER_ERROR_STATE, ("must set datasource first!"));
    V_UINT32 u32Res;

    if (E_SME_MEM_OK != VOS_Memset_S(
        &m_stVDispRect,
        sizeof(m_stVDispRect),
        0,
        sizeof(m_stVDispRect)))
    {
        ICS_LOGE("SECURE: VOS_Memset_S return failed.");
        return E_SME_PLAYER_ERROR_NOMEM;
    }

    m_stVDispRect.i32StartX = pobjMsg->GetStartX();
    m_stVDispRect.i32StartY = pobjMsg->GetStartY();
    m_stVDispRect.i32Height = pobjMsg->GetHeight();
    m_stVDispRect.i32Width = pobjMsg->GetWidth();

    ICS_LOGI("HandleSetVDispRect, m_pvGstPipeline=%p", m_pvGstPipeline);
    if(NULL != m_pvGstPipeline)
    {
        u32Res = SmePlayerGstWrap::SetDispRect(m_pvGstPipeline, m_stVDispRect);
        UTILS_MLOGE_RET_VAL_IF(ICS_SUCCESS != u32Res,
                               E_SME_PLAYER_ERROR_UNKOWN,
                               ("Set display rect failed."));
    }

    return E_SME_PLAYER_ERROR_NONE;
}

V_UINT32 SmePlayerImplGst::HandlePrepareAsync(SmePlayerMsgPrepareAsync* pobjMsg)
{
    V_UINT32 u32Ret;

    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGW_RET_VAL_IF(E_PLAYER_STATE_PREPARED ==  m_eState,
        E_SME_PLAYER_ERROR_NONE, ("already prepared!"));

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_INITED !=  m_eState,
        E_SME_PLAYER_ERROR_STATE, ("must set datasource first!"));

    u32Ret = StartCacheProcTimer();
    UTILS_MLOGE_RET_VAL_IF(ICS_SUCCESS !=  u32Ret,
        E_SME_PLAYER_ERROR_UNKOWN, ("set start cache failed!"));

    if(m_bCacheCtrlEnable)
    {

        //m_u32BufferFull = SME_BUFFER_TIME_DEFAULT;
        m_bBuffering = ICS_TRUE;
    }
    else
    {
        //m_u32BufferFull = SME_BUFFER_TIME_DEFAULT / 3;
        m_bBuffering = ICS_FALSE;
    }

    SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_PREPARING);
    //m_eState = E_PLAYER_STATE_PREPARING;

#ifdef __ENABLE_OTTOPT__
    if(ICS_TRUE == IsPlayingHls())
    {
        V_CHAR* pszQueryResult = NULL;
        V_UINT32 u32QueryRet = ICS_FAIL;

        VOS_ThdMutexLock(&m_stMutex);
        if (NULL == m_pobjMsgChnApp)
        {
            m_pobjMsgChnApp = IMsgChannelApp::GetInstance();
            if (NULL != m_pobjMsgChnApp)
            {
                u32Ret = m_pobjMsgChnApp->Init();
                UTILS_MLOGE_IF(ICS_SUCCESS != u32Ret, ("init msg channel app fail"));
            }
        }

        if(NULL != m_pobjCacheQuery && m_bIsConnected)
        {
            u32Ret = m_pobjCacheQuery->SendMsgSyncQueryCache((V_UINT8*)m_pszURI, u32QueryRet, (V_UINT8**)&pszQueryResult);
            if(u32Ret == ICS_SUCCESS)
            {
                if(u32QueryRet == ICS_SUCCESS && pszQueryResult != NULL)
                {
                    ICS_LOGD("SendMsgSyncQueryCache success, pszQueryResult:[%s]", pszQueryResult);
                    UTILS_MSAFEFREE(m_pszURI);
                    m_pszURI = strdup(pszQueryResult);
                    ICS_LOGD("SendMsgSyncQueryCache, before UTILS_MSAFEFREE(pszQueryResult)");
                    UTILS_MSAFEFREE(pszQueryResult);
                    ICS_LOGD("SendMsgSyncQueryCache, m_pszURI:[%s]", m_pszURI?m_pszURI:"null uri");
                }
                else
                {
                    ICS_LOGE("SendMsgSyncQueryCache failed! u32QueryRet=%u ", u32QueryRet);
                }
            }
            else
            {
                ICS_LOGE("Query cache failed! ret = %d", u32Ret);
            }
        }

        VOS_ThdMutexUnLock(&m_stMutex);
    }
#endif

    /* BEGIN: Modified for DTS2015102210493 by liurongliang(l00180035), 2015/10/23 */
    ICS_LOGD("HandlePrepareAsync(), in");
    /* END:   Modified for DTS2015102210493 by liurongliang(l00180035), 2015/10/23 */

    if(NULL != m_pvSurface)
    {
        u32Ret = SmePlayerGstWrap::Prepare(m_pvGstPipeline,
            const_cast<V_CHAR*>(m_pszURI), const_cast<V_VOID*>(m_pvSurface),
            m_u32BufferFull, m_u32BufferLowPercent, m_u32BufferHighPercent,
            ICS_FALSE, ICS_FALSE,
            &m_stSubCb, m_pszSubURI);
        if(m_stVDispRect.i32Width > 0
            && m_stVDispRect.i32Height > 0)
        {
            u32Ret = SmePlayerGstWrap::SetDispRect(m_pvGstPipeline, m_stVDispRect);
            UTILS_MLOGW_IF(ICS_SUCCESS != u32Ret, ("Set display rect failed."));
        }
    }
    else
    {
        u32Ret = SmePlayerGstWrap::Prepare(m_pvGstPipeline,
            const_cast<V_CHAR*>(m_pszURI), static_cast<V_VOID*>(&m_stVDispRect),
            m_u32BufferFull, m_u32BufferLowPercent, m_u32BufferHighPercent,
            ICS_TRUE, ICS_FALSE,
            &m_stSubCb, m_pszSubURI);
    }

    if(E_SME_PLAYER_ERROR_NONE == u32Ret)
    {
        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_PREPARED);

        ICS_LOGI("notify onPrepared!");
        SmePlayerNotifyOnPrepared* pobjMsgOnPrepared = new SmePlayerNotifyOnPrepared();

        u32Ret = m_pobjMsgRunner->SendMsg(pobjMsgOnPrepared);
        if(E_SME_PLAYER_ERROR_NONE != u32Ret)
        {
            ICS_LOGW("send OnPrepared Failed(%#x)!", u32Ret);
            UTILS_MTMPDEL(pobjMsgOnPrepared);
        }
    }
    else if (E_SME_PLAYER_ERROR_ASYNC == u32Ret)
    {
        u32Ret = E_SME_PLAYER_ERROR_NONE;
    }
    else
    {
        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_ERROR);

        SmePlayerNotifyOnError* pobjMsgOnError = new SmePlayerNotifyOnError(
            static_cast<E_SME_PLAYER_ERROR>(u32Ret));
        u32Ret = m_pobjMsgRunner->SendMsg(pobjMsgOnError);
        if(E_SME_PLAYER_ERROR_NONE != u32Ret)
        {
            ICS_LOGW("send OnError Failed(%#x)!", u32Ret);
            UTILS_MTMPDEL(pobjMsgOnError);
        }
        ICS_LOGE("prepare failed!");
    }

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::HandlePrepare(SmePlayerMsgPrepare* pobjMsg)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;

    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGW_RET_VAL_IF(E_PLAYER_STATE_PREPARED ==  m_eState,
        E_SME_PLAYER_ERROR_NONE, ("already prepared!"));

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_INITED !=  m_eState,
        E_SME_PLAYER_ERROR_STATE, ("must set datasource first!"));

    if(m_bCacheCtrlEnable)
    {
        //m_u32BufferFull = SME_BUFFER_MAX;
        m_bBuffering = ICS_TRUE;
    }
    else
    {
        //m_u32BufferFull = SME_BUFFER_MAX / 3;
        m_bBuffering = ICS_FALSE;
    }

    if(NULL != m_pvSurface)
    {
        u32Ret = SmePlayerGstWrap::Prepare(m_pvGstPipeline,
            const_cast<V_CHAR*>(m_pszURI), const_cast<V_VOID*>(m_pvSurface),
            m_u32BufferFull, m_u32BufferLowPercent, m_u32BufferHighPercent,
            ICS_FALSE, ICS_TRUE,
            &m_stSubCb, m_pszSubURI);
        if(m_stVDispRect.i32Width > 0
            && m_stVDispRect.i32Height > 0)
        {
           u32Ret = SmePlayerGstWrap::SetDispRect(m_pvGstPipeline, m_stVDispRect);
           UTILS_MLOGW_IF(ICS_SUCCESS != u32Ret, ("Set display rect failed."));
        }
    }
    else
    {
        u32Ret = SmePlayerGstWrap::Prepare(m_pvGstPipeline,
            const_cast<V_CHAR*>(m_pszURI), static_cast<V_VOID*>(&m_stVDispRect),
            m_u32BufferFull, m_u32BufferLowPercent, m_u32BufferHighPercent,
            ICS_TRUE, ICS_TRUE,
            &m_stSubCb, m_pszSubURI);
    }

    if(E_SME_PLAYER_ERROR_NONE == u32Ret)
    {
        //TODO:here we should wait for buffer up by lrl.
        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_PREPARED);
        //m_eState = E_PLAYER_STATE_PREPARED;
    }
    else
    {
        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_ERROR);
        ICS_LOGE("prepare failed!");
    }

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::HandleStart(SmePlayerMsgStart* pobjMsg)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;

    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGW_RET_VAL_IF(E_PLAYER_STATE_STARTED ==  m_eState,
        E_SME_PLAYER_ERROR_NONE, ("already started!"));

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED !=  m_eState,
        E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));

    VOS_ThdMutexLock(&m_stMutex);

    /** BEGIN: Modified for DTS2014081804355 by liurongliang(l00180035), 2014/8/25 */
    if(!m_bSeeking)
    {
        u32Ret = SmePlayerGstWrap::Start(m_pvGstPipeline);
        if(E_SME_PLAYER_ERROR_NONE == u32Ret)
        {
            SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_STARTED);
            //m_eState = E_PLAYER_STATE_STARTED;
            m_bPausedInternal = ICS_FALSE;
        }
        else
        {
            SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_ERROR);
            ICS_LOGE("start failed!");
        }
    }
    else
    {
        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_STARTED);
        //m_eState = E_PLAYER_STATE_STARTED;
        m_bPausedInternal = ICS_FALSE;
    }
    /** END: Modified for DTS2014081804355 by liurongliang(l00180035), 2014/8/25 */

    VOS_ThdMutexUnLock(&m_stMutex);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::HandleStop(SmePlayerMsgStop* pobjMsg)
{
    V_UINT32 u32Ret;

    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGW_RET_VAL_IF(E_PLAYER_STATE_PREPARED ==  m_eState,
        E_SME_PLAYER_ERROR_NONE, ("already stopped!"));

    UTILS_MLOGE_RET_VAL_IF((E_PLAYER_STATE_STARTED !=  m_eState)
        && (E_PLAYER_STATE_PAUSED !=  m_eState),
        E_SME_PLAYER_ERROR_STATE, ("must started first!"));

    VOS_ThdMutexLock(&m_stMutex);
    m_i32SeekTo = -1;
    m_bSeeking  = ICS_FALSE;
    m_bPausedInternal = ICS_FALSE;
    m_bBuffering = ICS_FALSE;
    VOS_ThdMutexUnLock(&m_stMutex);

    u32Ret = SmePlayerGstWrap::Stop(m_pvGstPipeline);
    if(E_SME_PLAYER_ERROR_NONE == u32Ret)
    {
        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_PREPARED);
        //m_eState = E_PLAYER_STATE_PREPARED;
    }
    else
    {
        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_ERROR);
        ICS_LOGE("stop failed!");
    }

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::HandlePause(SmePlayerMsgPause* pobjMsg)
{
    V_UINT32 u32Ret;

    ICS_LOGI("HandlePause in m_bSeeking=%d", m_bSeeking);
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGW_RET_VAL_IF(E_PLAYER_STATE_PAUSED ==  m_eState,
        E_SME_PLAYER_ERROR_NONE, ("already paused!"));

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_STARTED !=  m_eState,
        E_SME_PLAYER_ERROR_STATE, ("must started first!"));
    VOS_ThdMutexLock(&m_stMutex);

    /** BEGIN: Modified for DTS2015092411159 by liurongliang(l00180035), 2014/8/25 */
    //seek control move to sme_player_gst_warp.cpp.
    u32Ret = SmePlayerGstWrap::Pause(m_pvGstPipeline);
    if(E_SME_PLAYER_ERROR_NONE == u32Ret)
    {
        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_PAUSED);
        //m_eState = E_PLAYER_STATE_PAUSED;
        m_bPausedInternal = ICS_FALSE;
    }
    else
    {
        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_ERROR);
        ICS_LOGE("pause failed!");
    }
    /** END: Modified for DTS2015092411159 by liurongliang(l00180035), 2014/9/30 */
    VOS_ThdMutexUnLock(&m_stMutex);
    ICS_LOGI("HandlePause out u32Ret=%d", u32Ret);

    return u32Ret;
}

/* BEGIN: Modify for DTS2015071706676 by liurongliang(l00180035), 2015/7/22 */
V_UINT32 SmePlayerImplGst::HandleResume(SmePlayerMsgResume* pobjMsg)
{
    V_UINT32 u32Ret;

    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGW_RET_VAL_IF(E_PLAYER_STATE_STARTED ==  m_eState,
        E_SME_PLAYER_ERROR_NONE, ("already started!"));

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PAUSED !=  m_eState,
        E_SME_PLAYER_ERROR_STATE, ("must paused first!"));

    VOS_ThdMutexLock(&m_stMutex);
    u32Ret = SmePlayerGstWrap::Start(m_pvGstPipeline);
    if(E_SME_PLAYER_ERROR_NONE == u32Ret)
    {
        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_STARTED);
        //m_eState = E_PLAYER_STATE_STARTED;
        m_bPausedInternal = ICS_FALSE;
    }
    else
    {
        SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_ERROR);
        ICS_LOGE("resume failed!");
    }
    VOS_ThdMutexUnLock(&m_stMutex);

    return u32Ret;
}
/* END:   Modify for DTS2015071706676 by liurongliang(l00180035), 2015/7/22 */

V_UINT32 SmePlayerImplGst::HandleReset(SmePlayerMsgReset* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    ICS_LOGD("HandleReset in");

    UTILS_MLOGW_RET_VAL_IF(E_PLAYER_STATE_UNINIT ==  m_eState,
        E_SME_PLAYER_ERROR_NONE, ("already reset!"));

    m_bCacheCtrlEnable          = ICS_FALSE;
    m_bBuffering                = ICS_FALSE;
    m_u32BufferFull             = SME_BUFFER_TIME_DEFAULT;
    m_u32NotifyPercent          = 0;
    m_u32BufferThdDown          = m_u32BufferFull * m_u32BufferLowPercent / 100;
    m_u32BufferThdUp            = m_u32BufferFull * m_u32BufferHighPercent/ 100;
    m_u64LastUpdateThdUpTime    = M_SME_UINT64_MAX;
    StopCacheProcTimer();

    if(NULL != m_pvGstPipeline)
    {
        SmePlayerGstWrap::Reset(m_pvGstPipeline);
        SmePlayerGstWrap::DeInitGst(m_pvGstPipeline);
        m_pvGstPipeline = NULL;
    }

    UTILS_MSAFEFREE(m_pszURI);
    UTILS_MSAFEFREE(m_pszSubURI);
    m_pvSurface = NULL;
    m_stVDispRect.i32StartX = -1;
    m_stVDispRect.i32StartY = -1;
    m_stVDispRect.i32Width  = -1;
    m_stVDispRect.i32Height = -1;
    UTILS_MSAFEFREE(m_pszPlgFullPath);

    if (E_SME_MEM_OK != VOS_Memset_S(&m_stSubCb, sizeof(m_stSubCb), 0, sizeof(m_stSubCb)))
    {
        ICS_LOGE("SECURE: VOS_Memset_S return failed.");
    }

    SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_UNINIT);
    m_bPreParedNormal   = ICS_FALSE;
    /** BEGIN: Added by for live_seek liurongliang(l00180035), 2014/8/18 */
    m_i32Duration       = 0;
    /** END:   Added by for live_seek liurongliang(l00180035), 2014/8/18 */
    VOS_ThdMutexLock(&m_stMutex);
    m_i32SeekTo = -1;
    m_bSeeking  = ICS_FALSE;
    m_bPausedInternal = ICS_FALSE;
    m_bLoop = ICS_FALSE;
    m_bLoopSeeking = ICS_FALSE;
    VOS_ThdMutexUnLock(&m_stMutex);

    ICS_LOGD("HandleReset out");

    //BEGIN:flush log to file, for DTS2015072400092, by f00165155, 2015/7/25
    ICS_FlushLogBuff();
    //END:flush log to file, for DTS2015072400092, by f00165155, 2015/7/25

    return E_SME_PLAYER_ERROR_NONE;
}

/* BEGIN: Added for DTS2014091706218 by liurongliang(l00180035), 2014/9/22 */
V_UINT32 SmePlayerImplGst::CheckForSeek(const SmePlayerMsgSeekTo* pobjMsg)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;

    do
    {
        UTILS_MLOGW_BRK_VAL_IF((NULL ==  pobjMsg) || (pobjMsg->GetSeekToMs() < 0),
            u32Ret, E_SME_PLAYER_ERROR_INVALID_ARGS, ("SEEK MSG invalid args!"));

        UTILS_MLOGE_BRK_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
            u32Ret, E_SME_PLAYER_ERROR_STATE, ("must prepared first!"));

        UTILS_MLOGE_BRK_VAL_IF(NULL == m_pobjMsgRunner || NULL == m_pobjCmdRunner, u32Ret,
            E_SME_PLAYER_ERROR_STATE, ("construct failed, any cmd cannot execute!"));
    } while ( ICS_FALSE );

    UTILS_M_RET_VAL_IF(E_SME_PLAYER_ERROR_NONE == u32Ret, u32Ret);
    VOS_ThdMutexLock(&m_stMutex);
    m_i32SeekTo = -1;
    m_bSeeking  = ICS_FALSE;
    VOS_ThdMutexUnLock(&m_stMutex);

    return u32Ret;
}
/* END:   Added for DTS2014091706218 by liurongliang(l00180035), 2014/9/22 */


/** BEGIN: Modified for DTS2014072406838 by liurongliang(l00180035), 2014/7/24 */
V_UINT32 SmePlayerImplGst::HandleSeekTo(const SmePlayerMsgSeekTo* pobjMsg)
{
    V_UINT32 u32Ret;
    V_INT32 i32SeekToMs = 0;

    /* BEGIN: Modified for DTS2014091706218 by liurongliang(l00180035), 2014/9/22 */
    u32Ret = CheckForSeek(pobjMsg);
    UTILS_M_RET_VAL_IF(E_SME_PLAYER_ERROR_NONE != u32Ret, u32Ret);
    /* END:   Modified for DTS2014091706218 by liurongliang(l00180035), 2014/9/22 */

    i32SeekToMs = pobjMsg->GetSeekToMs();

    /** BEGIN: Added for live_seek by liurongliang(l00180035), 2014/8/18 */
    if(0 == m_i32Duration)
    {
        m_i32Duration = SmePlayerGstWrap::GetDuration(m_pvGstPipeline);
    }

    if(m_i32Duration <= 0)
    {
        VOS_ThdMutexLock(&m_stMutex);
        m_bSeeking  = ICS_FALSE;
        SmePlayerNotifyBase* pobjNotify = dynamic_cast<SmePlayerNotifyBase*>(
            new SmePlayerNotifyOnSeekCompleted(E_SME_PLAYER_ERROR_UNSUPPORT_SEEK,
            -1));
        V_UINT32 u32Err = m_pobjMsgRunner->SendMsg(pobjNotify);
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("send seekcompletly failed");
            UTILS_MTMPDEL(pobjNotify);
        }
        m_i32SeekTo = -1;
        VOS_ThdMutexUnLock(&m_stMutex);
        ICS_LOGW("is live playing,cannot support seek");

        return u32Ret;
    }
    /** END:   Added for live_seek by liurongliang(l00180035), 2014/8/18 */

    if((E_PLAYER_STATE_PAUSED == m_eState) || (E_PLAYER_STATE_PREPARED == m_eState))
    {
        u32Ret = (V_UINT32)SmePlayerGstWrap::MuteAudio(m_pvGstPipeline);
        if(E_SME_PLAYER_ERROR_NONE != u32Ret)
        {
            ICS_LOGE("SmePlayerGstWrap::mute failed(%d)", u32Ret);
        }
        m_bPausedInternal = ICS_FALSE;
    }
    else if(m_bPausedInternal)
    {
        /*notify up to buffering percent 100%*/
        V_UINT32 u32Percent = 100;
        SmePlayerNotifyBase* pobjNotify = dynamic_cast<SmePlayerNotifyBase*>(
            new SmePlayerNotifyOnBufferUpdate(u32Percent));
        u32Ret = m_pobjMsgRunner->SendMsg(pobjNotify);
        if(E_SME_PLAYER_ERROR_NONE != u32Ret)
        {
            ICS_LOGW("send OnBufferUpdate Failed(%#x)!", u32Ret);
            UTILS_MTMPDEL(pobjNotify);
        }

        /*notify up to buffering stop%*/
        pobjNotify = dynamic_cast<SmePlayerNotifyBase*>(
            new SmePlayerNotifyOnInfo(E_SME_PLAYER_INFO_BUFFERING_STOP,
            0));
        u32Ret = m_pobjMsgRunner->SendMsg(pobjNotify);
        if(E_SME_PLAYER_ERROR_NONE != u32Ret)
        {
            ICS_LOGW("send OnInfo Failed(%#x)!", u32Ret);
            UTILS_MTMPDEL(pobjNotify);
        }
        m_bPausedInternal   = ICS_FALSE;

        /* BEGIN: Added for DTS2015092207767 by liurongliang(l00180035), 2015/10/10 */
        //we should set m_bBuffering to TRUE after seek.
        m_bBuffering        = ICS_TRUE;
        /* END:   Added for DTS2015092207767 by liurongliang(l00180035), 2015/10/10 */
    }
    else
    {
       ;//do nothing
    }

    VOS_ThdMutexLock(&m_stMutex);
    m_bSeeking  = ICS_TRUE;
    VOS_ThdMutexUnLock(&m_stMutex);

    u32Ret = SmePlayerGstWrap::SeekTo(m_pvGstPipeline, i32SeekToMs);

    /*BEGIN: Added for DTS2014081802472 hanle the seek error ,add by z00213950 2014/08/19*/
    if ((E_SME_PLAYER_ERROR_ASYNC != u32Ret)
            && (E_SME_PLAYER_ERROR_NONE != u32Ret))
    {
        VOS_ThdMutexLock(&m_stMutex);
        m_bSeeking  = ICS_FALSE;
        SmePlayerNotifyBase* pobjNotify = dynamic_cast<SmePlayerNotifyBase*>(
            new SmePlayerNotifyOnSeekCompleted(E_SME_PLAYER_ERROR_UNSUPPORT_SEEK,
            -1));
        V_UINT32 u32Err = m_pobjMsgRunner->SendMsg(pobjNotify);
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("send seekcompletly failed");
            UTILS_MTMPDEL(pobjNotify);
        }
        m_i32SeekTo = -1;
        VOS_ThdMutexUnLock(&m_stMutex);
        ICS_LOGW("seek fail");
    }
    /*END: Added for DTS2014081802472 hanle the seek error ,add by z00213950 2014/08/19 */
    return u32Ret;
}
/** END:   Modified for DTS2014072406838 by liurongliang(l00180035), 2014/7/24 */

V_UINT32 SmePlayerImplGst::HandleGetDuration(SmePlayerMsgGetDur* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)0, ("must prepare first!"));

    m_i32Duration = SmePlayerGstWrap::GetDuration(m_pvGstPipeline);

    return (V_UINT32)m_i32Duration;
}

V_UINT32 SmePlayerImplGst::HandleGetCurPosition(SmePlayerMsgGetCurPos* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)-1, ("must prepare first!"));

    /** BEGIN: Added for DTS2014072905447 by zhoubing(z00213950), 2014/7/28 */
    if(m_bSeeking)
    {
        return (V_UINT32)m_i32SeekTo;
    }
    /** BEGIN: Added for DTS2014072905447 by zhoubing(z00213950), 2014/7/28 */

    return (V_UINT32)SmePlayerGstWrap::GetCurPosition(m_pvGstPipeline);
}

V_UINT32 SmePlayerImplGst::HandleIsPlaying(SmePlayerMsgIsPlay* pobjMsg)  const//mmq HandleIsPlaying could be made const
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    return (V_UINT32)((E_PLAYER_STATE_STARTED == m_eState) ? 1 : 0);
}

V_UINT32 SmePlayerImplGst::HandleSetBitrate(const SmePlayerMsgSetBitrate* pobjMsg)
{
    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
      (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));
    SmePlayerGstWrap::SetStreamBitrate(m_pvGstPipeline, pobjMsg->GetBitrate());

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerImplGst::HandleRegManifestCompleteCb(
    IN const SmePlayerMsgRegManifestCompleteCb* pobjMsg)
{
    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_INITED >  m_eState,
      (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must set datasource first!"));

    SmePlayerGstWrap::SetManifestCompleteCb(m_pvGstPipeline,
        pobjMsg->GetManifestCompleteCb(),
        pobjMsg->GetCbCtx());

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerImplGst::HandleRegBtChangedCb(
    IN const SmePlayerMsgRegBtChangedCb* pobjMsg)
{
    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_INITED >  m_eState,
      (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must set datasource first!"));

    SmePlayerGstWrap::SetBtChangedCb (m_pvGstPipeline,
        pobjMsg->GetBtChangedCb(),
        pobjMsg->GetCbCtx());

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerImplGst::HandleGetBandwidth(SmePlayerMsgGetBandwidth* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)-1, ("must prepare first!"));
    m_u32Bandwidth = SmePlayerGstWrap::GetBandwidth(m_pvGstPipeline);

    return m_u32Bandwidth;
}

V_UINT32 SmePlayerImplGst::HandleSetAudioMute(
    const SmePlayerMsgSetAudioMute* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));

    return (V_UINT32)SmePlayerGstWrap::SetAudioMute(m_pvGstPipeline,
                                              pobjMsg->GetAudioMute());
}

V_UINT32 SmePlayerImplGst::HandleGetAudioMute(
    SmePlayerMsgGetAudioMute* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED > m_eState,
        (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));

    return (V_UINT32)SmePlayerGstWrap::GetAudioMute(m_pvGstPipeline,
                                              pobjMsg->GetAudioMute());
}

V_UINT32 SmePlayerImplGst::HandleSetAudioVolume(
    const SmePlayerMsgSetAudioVolume* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));

    return (V_UINT32)SmePlayerGstWrap::SetAudioVolume(m_pvGstPipeline,
                                              pobjMsg->GetAudioVolume());
}

V_UINT32 SmePlayerImplGst::HandleGetAudioVolume(
    SmePlayerMsgGetAudioVolume* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED > m_eState,
        (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));

    return (V_UINT32)SmePlayerGstWrap::GetAudioVolume(m_pvGstPipeline,
                                              pobjMsg->GetAudioVolume());
}

V_UINT32 SmePlayerImplGst::HandleSetAudioChannelMode(
    const SmePlayerMsgSetAudioChannelMode* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);


    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));


    return (V_UINT32)SmePlayerGstWrap::SetAudioChannelMode(m_pvGstPipeline,
                                              pobjMsg->GetAudioChannelMode());
}

V_UINT32 SmePlayerImplGst::HandleGetAudioChannelMode(
    SmePlayerMsgGetAudioChannelMode* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED > m_eState,
        (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));

    return (V_UINT32)SmePlayerGstWrap::GetAudioChannelMode(m_pvGstPipeline,
                                              pobjMsg->GetAudioChannelMode());
}


V_UINT32 SmePlayerImplGst::HandleSetAudioHdmiMode(
    const SmePlayerMsgSetAudioHdmiMode* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));

    return SmePlayerGstWrap::SetAudioHdmiMode(m_pvGstPipeline,
                                              pobjMsg->GetAudioHdmiMode());
}

V_UINT32 SmePlayerImplGst::HandleGetAudioHdmiMode(
    SmePlayerMsgGetAudioHdmiMode* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED > m_eState,
        (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));

    return SmePlayerGstWrap::GetAudioHdmiMode(m_pvGstPipeline,
                                              pobjMsg->GetAudioHdmiMode());
}

V_UINT32 SmePlayerImplGst::HandleGetBufferStatus(SmePlayerMsgGetBufferStatus* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)-1, ("must prepare first!"));
    return SmePlayerGstWrap::GetBufferStatus(m_pvGstPipeline,
                                             pobjMsg->GetBufferStatus());
}

V_UINT32 SmePlayerImplGst::HandleGetCurrentBitrate(SmePlayerMsgGetBitrate* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)-1, ("must prepare first!"));
    return SmePlayerGstWrap::GetCurrentBitrate(m_pvGstPipeline,
                                               pobjMsg->GetCurrentBitrate());
}

V_UINT32 SmePlayerImplGst::HandleGetVideoTrackInfo(SmePlayerMsgGetVideoTrackInfo* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)-1, ("must prepare first!"));
    return SmePlayerGstWrap::GetVideoTrackInfo(m_pvGstPipeline,
                                               pobjMsg->GetVideoTrackInfo());
}

V_UINT32 SmePlayerImplGst::HandleGetAudioTrackInfo(SmePlayerMsgGetAudioTrackInfo* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)-1, ("must prepare first!"));
    return SmePlayerGstWrap::GetAudioTrackInfo(m_pvGstPipeline,
                                               pobjMsg->GetAudioTrackInfo());
}

V_UINT32 SmePlayerImplGst::HandleGetVideoDispMode(
    SmePlayerMsgGetVideoDispMode* pobjMsg)
{
    ICS_LOGD("HandleGetVideoDispMode in");

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED > m_eState,
        (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));

    return (V_UINT32)SmePlayerGstWrap::GetVideoDispMode(m_pvGstPipeline,
                                                       pobjMsg->GetVideoDispMode(),
                                                       pobjMsg->GetVideoDispRatio());
}

V_UINT32 SmePlayerImplGst::HandleGetVideoDispRect(
    SmePlayerMsgGetVideoDispRect* pobjMsg)
{
    ICS_LOGD("HandleGetVideoDispRect in");

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED > m_eState,
        (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));

    return (V_UINT32)SmePlayerGstWrap::GetVideoDispRect(m_pvGstPipeline,
                                                       pobjMsg->GetVideoDispRect());
}

V_UINT32 SmePlayerImplGst::HandleSetAudioTrack(SmePlayerMsgSetAudioTrack* pobjMsg)
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)-1, ("must prepare first!"));
    return SmePlayerGstWrap::SetActiveAudioTrack(m_pvGstPipeline,
                                                 (V_UINT32)pobjMsg->GetAudioTrack());
}

V_UINT32 SmePlayerImplGst::HandleSetProp(const SmePlayerMsgSetProp* pobjMsg)
{
    V_UINT32            u32Ret      = E_SME_PLAYER_ERROR_NONE;

    do
    {
        const SmeProperty*  pobjProp    = NULL;

        pobjProp = pobjMsg->GetProp();
        UTILS_MLOGE_BRK_VAL_IF(NULL == pobjProp, u32Ret,
            E_SME_PLAYER_ERROR_INVALID_ARGS, ("prop null"));
        if(!strcmp(pobjProp->GetStringKey(),
            SmeProperty::ConstStringKey::m_spszKeyLog))
        {
            UTILS_MLOGE_BRK_VAL_IF(SmeProperty::E_VAL_TYPE_UINT32
                != pobjProp->GetValType(), u32Ret, static_cast<V_UINT32>(
                E_SME_PLAYER_ERROR_INVALID_ARGS),
                ("log val type should be SmeProperty::E_VAL_TYPE_UINT32"));

            m_eLogLevel = static_cast<E_SME_PLAYER_LOG_LEVEL>(
                pobjProp->GetUint32Val());
            ICS_LOGI("gst log level=%d", m_eLogLevel);

            if(m_eState >= E_PLAYER_STATE_INITED && m_eState < E_PLAYER_STATE_ERROR)
            {
                //only can set log level after gst init
                u32Ret = SmePlayerGstWrap::SetLogLevel(m_pvGstPipeline, m_eLogLevel);
            }
        }
        else if(!strcmp(pobjProp->GetStringKey(),
            SmeProperty::ConstStringKey::m_spszKeyPlgPath))
        {
            UTILS_MLOGE_BRK_VAL_IF(SmeProperty::E_VAL_TYPE_STRING
                != pobjProp->GetValType(), u32Ret, static_cast<V_UINT32>(
                E_SME_PLAYER_ERROR_INVALID_ARGS),
                ("Plg type should be SmeProperty::E_VAL_TYPE_STRING"));

            UTILS_MLOGE_BRK_VAL_IF(NULL == pobjProp->GetStringVal(),
                u32Ret, static_cast<V_UINT32>(E_SME_PLAYER_ERROR_INVALID_ARGS),
                ("log val type should not be null"));

            m_pszPlgFullPath = strdup(pobjProp->GetStringVal());
            UTILS_MLOGE_BRK_VAL_IF(NULL == m_pszPlgFullPath,
                u32Ret, static_cast<V_UINT32>(E_SME_PLAYER_ERROR_NOMEM),
                ("strdup plg path failed"));

            ICS_LOGI("gst plg path =%s", m_pszPlgFullPath);
        }
        else
        {
            ICS_LOGE("not support prop key=%s", pobjProp->GetStringKey());
            u32Ret = static_cast<V_UINT32>(E_SME_PLAYER_ERROR_UNSUPPORT_OP);
        }

    } while (ICS_FALSE);


    return u32Ret;

}

V_UINT32 SmePlayerImplGst::HandleGetProp(const SmePlayerMsgGetProp* pobjMsg)
{
    V_UINT32            u32Ret      = E_SME_PLAYER_ERROR_NONE;

    ICS_LOGI("HandleGetProp in:");
    do
    {
        SmeProperty*  pobjRespProp    = NULL;

        pobjRespProp = pobjMsg->GetRespProp();
        UTILS_MLOGE_BRK_VAL_IF(NULL == pobjRespProp, u32Ret,
            E_SME_PLAYER_ERROR_INVALID_ARGS, ("prop null"));

        if(!strcmp(pobjRespProp->GetStringKey(), SmeProperty::ConstStringKey::m_spszKeyStartTime))
        {
            ICS_LOGE("not support prop key=%s", pobjRespProp->GetStringKey());
            u32Ret = static_cast<V_UINT32>(E_SME_PLAYER_ERROR_UNSUPPORT_OP);
        }
        else if(!strcmp(pobjRespProp->GetStringKey(), SmeProperty::ConstStringKey::m_spszKeyFps))
        {
            ICS_LOGE("not support prop key=%s", pobjRespProp->GetStringKey());
            u32Ret = static_cast<V_UINT32>(E_SME_PLAYER_ERROR_UNSUPPORT_OP);
        }
        else if(!strcmp(pobjRespProp->GetStringKey(), SmeProperty::ConstStringKey::m_spszKeyCurrFps))
        {
            ICS_LOGE("not support prop key=%s", pobjRespProp->GetStringKey());
            u32Ret = static_cast<V_UINT32>(E_SME_PLAYER_ERROR_UNSUPPORT_OP);
        }
        else if(!strcmp(pobjRespProp->GetStringKey(), SmeProperty::ConstStringKey::m_spszKeyDemuxFps))
        {
            ICS_LOGE("not support prop key=%s", pobjRespProp->GetStringKey());
            u32Ret = static_cast<V_UINT32>(E_SME_PLAYER_ERROR_UNSUPPORT_OP);
        }
        else if(!strcmp(pobjRespProp->GetStringKey(), SmeProperty::ConstStringKey::m_spszKeyDecodeFps))
        {
            ICS_LOGE("not support prop key=%s", pobjRespProp->GetStringKey());
            u32Ret = static_cast<V_UINT32>(E_SME_PLAYER_ERROR_UNSUPPORT_OP);
        }
        else if (!strcmp(pobjRespProp->GetStringKey(), SmeProperty::ConstStringKey::m_spszKeyLog))
        {
            UTILS_MLOGE_BRK_VAL_IF(SmeProperty::E_VAL_TYPE_UINT32
            != pobjRespProp->GetValType(), u32Ret, static_cast<V_UINT32>(
            E_SME_PLAYER_ERROR_INVALID_ARGS),
            ("log val type should be SmeProperty::E_VAL_TYPE_UINT32"));

            pobjRespProp->SetUint32Val(m_eLogLevel);
            ICS_LOGI("gst log level=%d", m_eLogLevel);
        }
        else
        {
            ICS_LOGE("not support prop key=%s", pobjRespProp->GetStringKey());
            u32Ret = static_cast<V_UINT32>(E_SME_PLAYER_ERROR_UNSUPPORT_OP);
        }
    } while (ICS_FALSE);
    ICS_LOGI("HandleGetProp out:");
    return u32Ret;

}

V_UINT32 SmePlayerImplGst::HandleSetSubDs(const SmePlayerMsgSetSubDs* pobjMsg)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;

    do
    {
        UTILS_MLOGE_BRK_VAL_IF((E_PLAYER_STATE_PREPARING <= m_eState)
            || (NULL == m_pobjPlayerObs), u32Ret, E_SME_PLAYER_ERROR_STATE,
            ("must reset and then set None Null obs first!"));

        UTILS_MLOGE_BRK_VAL_IF(NULL == pobjMsg->GetSubUri(), u32Ret,
            E_SME_PLAYER_ERROR_INVALID_ARGS, ("uri is null"));
        UTILS_MSAFEFREE(m_pszSubURI);
        m_pszSubURI = strdup(pobjMsg->GetSubUri());
        UTILS_MLOGE_RET_VAL_IF(NULL == m_pszSubURI, E_SME_PLAYER_ERROR_NOMEM,
                ("strdup suburi failed"));
    } while (ICS_FALSE);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::HandleRegSubCb(const SmePlayerMsgRegSubCb* pobjMsg)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;

    do
    {
        UTILS_MLOGE_BRK_VAL_IF((E_PLAYER_STATE_PREPARING <= m_eState)
            || (NULL == m_pobjPlayerObs), u32Ret, E_SME_PLAYER_ERROR_STATE,
            ("must reset and then set None Null obs first!"));

        UTILS_MLOGE_BRK_VAL_IF(NULL == pobjMsg->GetSubCb(), u32Ret,
            E_SME_PLAYER_ERROR_INVALID_ARGS, ("subCb is null"));
        m_stSubCb = *pobjMsg->GetSubCb();
    } while (ICS_FALSE);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::HandleSetDisplayMode(const SmePlayerMsgSetDisplayMode* pobjMsg)
{
    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
      (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must prepare first!"));

    V_UINT32 u32DisplayMode = 0;
    ST_DISP_RATIO* pstDispRatio = NULL;

    pobjMsg->GetDisplayModeAndRadio(&u32DisplayMode, &pstDispRatio);

    ICS_LOGI("Handle,DispMode is %d, pstDispRatio is %p ",u32DisplayMode, pstDispRatio);

    SmePlayerGstWrap::SetDispMode(m_pvGstPipeline, u32DisplayMode, pstDispRatio);

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerImplGst::HandleSetChannelChangeMode(const SmePlayerMsgSetSwitchChannelMode* pobjMsg)
{
    UTILS_MLOGE_RET_VAL_IF((E_PLAYER_STATE_UNINIT ==  m_eState || E_PLAYER_STATE_ERROR == m_eState),
      (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("must after setdatasource, and before reset,or before stop!"));

    V_UINT32 uRet;
    V_UINT32 u32SwitchChannelMode = 0;
    pobjMsg->GetSwitchChannelMode(&u32SwitchChannelMode);
    ICS_LOGI("Handle,GetSwitchChannelMode is %d", u32SwitchChannelMode);

    uRet = SmePlayerGstWrap::SetSwitchChannelMode(m_pvGstPipeline, u32SwitchChannelMode);

    return uRet;
}


V_UINT32 SmePlayerImplGst::HandleSetUserAgent(const SmePlayerMsgSetUserAgent* pobjMsg)
{
    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED <  m_eState,
      (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("already prepare!"));
    SmePlayerGstWrap::SetWrapUserAgent(m_pvGstPipeline, pobjMsg->GetUserAgent());

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerImplGst::HandleSetReferer(const SmePlayerMsgSetReferer* pobjMsg)
{
    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED <  m_eState,
      (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("already prepare!"));
    SmePlayerGstWrap::SetWrapReferer(m_pvGstPipeline, pobjMsg->GetReferer());

    return ICS_SUCCESS;
}


V_UINT32 SmePlayerImplGst::HandleSetCookies(SmePlayerMsgSetCookies* pobjMsg)
{
    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED <  m_eState,
      (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("already prepare!"));
    SmePlayerGstWrap::SetWrapCookies(m_pvGstPipeline, pobjMsg->GetCookies());

    return ICS_SUCCESS;
}

V_UINT32 SmePlayerImplGst::HandleSetByteRange(const SmePlayerMsgSetByteRange* pobjMsg)
{
    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED <  m_eState,
      (V_UINT32)E_SME_PLAYER_ERROR_STATE, ("already prepare!"));
    SmePlayerGstWrap::SetWrapByteRange(m_pvGstPipeline, pobjMsg->GetByteRange());

    return ICS_SUCCESS;
}


/** BEGIN:   Modified for DTS2014072406838 by liurongliang(l00180035), 2014/7/24 */
/*static*/
V_VOID SmePlayerImplGst::GstMsgHandler(IN V_VOID* pvCtx,
                            IN V_UINT32 u32Msg,
                            IN V_UINT32 u32Ext1,
                            IN V_UINT32 u32Ext2)
{
    SmePlayerImplGst*       pobjThis    = static_cast<SmePlayerImplGst*>(pvCtx);
    SmePlayerNotifyBase*    pobjMsg     = NULL;

    UTILS_MLOGE_RET_IF(NULL == pobjThis, ("null this"));

    switch(u32Msg)
    {
        case E_SME_PLAYER_NOTIFY_SEEK_COMPLETED:
        {
            VOS_ThdMutexLock(&pobjThis->m_stMutex);
            if((-1 != pobjThis->m_i32SeekTo)
                && ((V_INT32)u32Ext1 != pobjThis->m_i32SeekTo))
            {
                V_INT32 i32SeekTo = pobjThis->m_i32SeekTo;
                SmePlayerMsgBase*       pobjCmd     = NULL;

                ICS_LOGI("continual seek to = %d", i32SeekTo);

                pobjCmd = new SmePlayerMsgSeekTo(NULL, NULL, i32SeekTo);
                V_UINT32 u32Err = pobjThis->m_pobjCmdRunner->SendMsg(pobjCmd);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGW("SeekTo:%d Failed(%#x)!", i32SeekTo, u32Err);
                    UTILS_MTMPDEL(pobjCmd);
                    pobjThis->m_i32SeekTo = -1;
                    pobjThis->m_bSeeking  = ICS_FALSE;
                }
            }
            else
            {
                pobjThis->m_i32SeekTo = -1;
                pobjThis->m_bSeeking  = ICS_FALSE;

                pobjMsg = dynamic_cast<SmePlayerNotifyBase*>(
                    new SmePlayerNotifyOnSeekCompleted(E_SME_PLAYER_ERROR_NONE,
                    (V_INT32)u32Ext1));
                V_UINT32 u32Err = pobjThis->m_pobjMsgRunner->SendMsg(pobjMsg);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("send seekcompletly failed");
                    UTILS_MTMPDEL(pobjMsg);
                }

                if(pobjThis->m_bCacheCtrlEnable)
                {
                    u32Err = pobjThis->NoteCacheCtrl();
                    UTILS_MLOGW_IF(ICS_SUCCESS != u32Err,
                        ("NoteCacheCtrl Failed(%#x)", u32Err));
                }

                if (MUTE_STATUS_OFF == pobjThis->m_mute_status)
                {
                    u32Err = (V_UINT32)SmePlayerGstWrap::UnmuteAudio(pobjThis->m_pvGstPipeline);
                    if (E_SME_PLAYER_ERROR_NONE != u32Err)
                        ICS_LOGE("SmePlayerGstWrap::UnmuteAudio failed(%d)",u32Err);
                }
            }
            VOS_ThdMutexUnLock(&pobjThis->m_stMutex);
            break;
        }
        case E_SME_PLAYER_NOTIFY_PLAYBACK_COMPLETED:
        {
            VOS_ThdMutexLock(&pobjThis->m_stMutex);
            pobjThis->m_i32SeekTo = -1;
            pobjThis->m_bSeeking  = ICS_FALSE;
            VOS_ThdMutexUnLock(&pobjThis->m_stMutex);

            pobjMsg = dynamic_cast<SmePlayerNotifyBase*>(
                new SmePlayerNotifyOnEos());
            V_UINT32 u32Err = pobjThis->m_pobjMsgRunner->SendMsg(pobjMsg);
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("send playbackcompletly failed");
                UTILS_MTMPDEL(pobjMsg);
            }
            break;
        }
        case E_SME_PLAYER_NOTIFY_PREPARED:
        {
            if(E_PLAYER_STATE_PREPARING == pobjThis->m_eState)
            {
                ICS_LOGI("notify onPrepared!");
                pobjMsg = dynamic_cast<SmePlayerNotifyBase*>(
                    new SmePlayerNotifyOnPrepared());
                V_UINT32 u32Err = pobjThis->m_pobjMsgRunner->SendMsg(pobjMsg);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("send OnPrepared failed");
                    UTILS_MTMPDEL(pobjMsg);
                }
            }
            break;
        }
        case E_SME_PLAYER_NOTIFY_ERROR:
        {
            pobjMsg = dynamic_cast<SmePlayerNotifyBase*>(
                new SmePlayerNotifyOnError(static_cast<E_SME_PLAYER_ERROR>(
                u32Ext1)));
            V_UINT32 u32Err = pobjThis->m_pobjMsgRunner->SendMsg(pobjMsg);
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("send OnError failed");
                UTILS_MTMPDEL(pobjMsg);
            }
            break;
        }
        case E_SME_PLAYER_NOTIFY_VIDEO_SIZE_CHANGED:
        {
            ICS_LOGI("impl level :resolution changed: %d * %d", u32Ext1, u32Ext2);
            pobjMsg = dynamic_cast<SmePlayerNotifyBase*>(
                new SmePlayerNotifyOnResolutionChange(u32Ext1, u32Ext2));
            V_UINT32 u32Err = pobjThis->m_pobjMsgRunner->SendMsg(pobjMsg);
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("send OnResolutionChange failed");
                UTILS_MTMPDEL(pobjMsg);
            }
            break;
        }
        case E_SME_PLAYER_NOTIFY_HTTP_ERR:
        {
            ICS_LOGI("impl level :http error status code: %d", u32Ext1);
            pobjMsg = dynamic_cast<SmePlayerNotifyBase*>(
                new SmePlayerNotifyOnHttpErr(u32Ext1));
            V_UINT32 u32Err = pobjThis->m_pobjMsgRunner->SendMsg(pobjMsg);
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("send GetHttpErrStatusCode failed");
                UTILS_MTMPDEL(pobjMsg);
            }
            break;
        }
        case E_SME_PLAYER_NOTIFY_FRAGMENT_UPDATE:
        {
            ICS_LOGI("notify fragment info update:");
            ST_FRAGMENT_INFO *pstFragmentInfo = (ST_FRAGMENT_INFO *)u32Ext1;
            pobjMsg = dynamic_cast<SmePlayerNotifyBase*>(
                new SmePlayerNotifyOnFragmentChanged(pstFragmentInfo));
            V_UINT32 u32Err = pobjThis->m_pobjMsgRunner->SendMsg(pobjMsg);
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("send OnFragmentInfoChanged failed");
                UTILS_MTMPDEL(pobjMsg);
            }
            break;
        }
        default:
            ICS_LOGW("msg:%u not handle", u32Msg);
            break;
    }

    return ;
}
/** END:   Modified for DTS2014072406838 by liurongliang(l00180035), 2014/7/24 */

V_VOID SmePlayerImplGst::OnCmdComing(SmePlayerMsgBase* pobjMsg)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_UNKOWN;

    switch(pobjMsg->GetType())
    {
        case SmePlayerMsgBase::E_TYPE_GET_ISPLAYING:
            u32Ret = HandleIsPlaying(static_cast<SmePlayerMsgIsPlay*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_CURPOSITION:
            u32Ret = HandleGetCurPosition(static_cast<SmePlayerMsgGetCurPos*>(
                pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_CACHE_CTRL:
            u32Ret = HandleCacheCtrl(static_cast<SmePlayerMsgCacheCtrl*>(
                pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SEEKTO:
            ICS_LOGD("SmePlayerMsgBase::E_TYPE_SEEKTO");
            u32Ret = HandleSeekTo(static_cast<SmePlayerMsgSeekTo*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_PAUSE:
            ICS_LOGD("SmePlayerMsgBase::E_TYPE_PAUSE");
            u32Ret = HandlePause(static_cast<SmePlayerMsgPause*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_RESUME:
            ICS_LOGD("SmePlayerMsgBase::E_TYPE_RESUME");
            u32Ret = HandleResume(static_cast<SmePlayerMsgResume*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_START:
            ICS_LOGD("SmePlayerMsgBase::E_TYPE_START");
            u32Ret = HandleStart(static_cast<SmePlayerMsgStart*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_STOP:
            ICS_LOGD("SmePlayerMsgBase::E_TYPE_STOP");
            u32Ret = HandleStop(static_cast<SmePlayerMsgStop*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_RESET:
            ICS_LOGD("SmePlayerMsgBase::E_TYPE_RESET");
            u32Ret = HandleReset(static_cast<SmePlayerMsgReset*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_DURATION:
            u32Ret = HandleGetDuration(static_cast<SmePlayerMsgGetDur*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_PREPARE_ASYNC:
            ICS_LOGD("SmePlayerMsgBase::E_TYPE_PREPARE_ASYNC");
            u32Ret = HandlePrepareAsync(static_cast<SmePlayerMsgPrepareAsync*>(
                pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_DS:
            ICS_LOGD("SmePlayerMsgBase::E_TYPE_SET_DS");
            u32Ret = HandleSetDs(static_cast<SmePlayerMsgSetDs*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_SF:
            ICS_LOGD("SmePlayerMsgBase::E_TYPE_SET_SF");
            u32Ret = HandleSetSf(static_cast<SmePlayerMsgSetSf*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_VDISP_RECT:
            ICS_LOGD("SmePlayerMsgBase::E_TYPE_SET_VDISP_RECT");
            u32Ret = HandleSetVDispRect(static_cast<SmePlayerMsgSetVDispRect*>(
                pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_OBS:
            ICS_LOGD("SmePlayerMsgBase::E_TYPE_SET_OBS");
            u32Ret = HandleSetObs(static_cast<SmePlayerMsgSetObs*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_VIDEO_WIDTH:
            u32Ret = HandleGetVideoWidth(static_cast<SmePlayerMsgGetVideoWidth*>(
                pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_VIDEO_HEIGHT:
            u32Ret = HandleGetVideoHeight(
                static_cast<SmePlayerMsgGetVideoHeight*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_PREPARE:
            ICS_LOGD("SmePlayerMsgBase::E_TYPE_PREPARE");
            u32Ret = HandlePrepare(static_cast<SmePlayerMsgPrepare*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_PROP:
            u32Ret = HandleSetProp(static_cast<SmePlayerMsgSetProp*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_PROP:
            u32Ret = HandleGetProp(static_cast<SmePlayerMsgGetProp*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_SUB_DS:
            u32Ret = HandleSetSubDs(static_cast<SmePlayerMsgSetSubDs*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_REG_SUB_CB:
            u32Ret = HandleRegSubCb(static_cast<SmePlayerMsgRegSubCb*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_STREAM_BITRATE:
            u32Ret = HandleSetBitrate(static_cast<SmePlayerMsgSetBitrate*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_REG_MANIFEST_COMPLETE_CB:
            u32Ret = HandleRegManifestCompleteCb(static_cast<SmePlayerMsgRegManifestCompleteCb*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_REG_BITRATE_CHANGED_CB:
            u32Ret = HandleRegBtChangedCb(static_cast<SmePlayerMsgRegBtChangedCb*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_BANDWIDTH:
            u32Ret = HandleGetBandwidth(static_cast<SmePlayerMsgGetBandwidth*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_AUDIO_MUTE:
            u32Ret = HandleSetAudioMute(
                static_cast<SmePlayerMsgSetAudioMute*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_AUDIO_MUTE:
            u32Ret = HandleGetAudioMute(
                static_cast<SmePlayerMsgGetAudioMute*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_AUDIO_VOLUME:
            u32Ret = HandleSetAudioVolume(
                static_cast<SmePlayerMsgSetAudioVolume*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_AUDIO_VOLUME:
            u32Ret = HandleGetAudioVolume(
                static_cast<SmePlayerMsgGetAudioVolume*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_AUDIO_CHANNEL_MODE:
            u32Ret = HandleSetAudioChannelMode(
                static_cast<SmePlayerMsgSetAudioChannelMode*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_AUDIO_CHANNEL_MODE:
            u32Ret = HandleGetAudioChannelMode(
                static_cast<SmePlayerMsgGetAudioChannelMode*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_AUDIO_HDMI_MODE:
            u32Ret = HandleSetAudioHdmiMode(
                static_cast<SmePlayerMsgSetAudioHdmiMode*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_AUDIO_HDMI_MODE:
            u32Ret = HandleGetAudioHdmiMode(
                static_cast<SmePlayerMsgGetAudioHdmiMode*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_BUFFER_STATUS:
            u32Ret = HandleGetBufferStatus(
                static_cast<SmePlayerMsgGetBufferStatus*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_DISPMODE:
            u32Ret = HandleSetDisplayMode(static_cast<SmePlayerMsgSetDisplayMode*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_SWITCH_CHANNEL_MODE:
            u32Ret = HandleSetChannelChangeMode(static_cast<SmePlayerMsgSetSwitchChannelMode*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_LOOP:
            u32Ret = HandleSetLoop(static_cast<SmePlayerMsgSetLoop*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_VIDEO_TRACK_INFO:
            u32Ret = HandleGetVideoTrackInfo(
                static_cast<SmePlayerMsgGetVideoTrackInfo*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_AUDIO_TRACK_INFO:
            u32Ret = HandleGetAudioTrackInfo(
                static_cast<SmePlayerMsgGetAudioTrackInfo*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_AUDIO_TRACK:
            u32Ret = HandleSetAudioTrack (
                static_cast<SmePlayerMsgSetAudioTrack*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_USER_AGENT:
            u32Ret = HandleSetUserAgent(static_cast<SmePlayerMsgSetUserAgent*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_REFERER:
            u32Ret = HandleSetReferer(static_cast<SmePlayerMsgSetReferer*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_COOKIES:
            u32Ret = HandleSetCookies(static_cast<SmePlayerMsgSetCookies*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_SET_BYTE_RANGE:
            u32Ret = HandleSetByteRange(static_cast<SmePlayerMsgSetByteRange*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_BITRATE:
            u32Ret = HandleGetCurrentBitrate(static_cast<SmePlayerMsgGetBitrate*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_DISPMODE:
            u32Ret = HandleGetVideoDispMode(static_cast<SmePlayerMsgGetVideoDispMode*>(pobjMsg));
            break;
        case SmePlayerMsgBase::E_TYPE_GET_DISPRECT:
            u32Ret = HandleGetVideoDispRect(static_cast<SmePlayerMsgGetVideoDispRect*>(pobjMsg));
            break;
        default:
            ICS_LOGE("Unkown cmd=%d", pobjMsg->GetType());
            break;
    }

    pobjMsg->SetResult(u32Ret);

    return;
}

V_VOID SmePlayerImplGst::OnNotifyComing(SmePlayerMsgBase* pobjMsg)
{
    UTILS_M_RET_IF(NULL == pobjMsg);
    UTILS_M_RET_IF(NULL == m_pobjPlayerObs);
    V_UINT32 u32Res;

    switch(pobjMsg->GetType())
    {
        case SmePlayerMsgBase::E_TYPE_ON_INFO:
        {
            SmePlayerNotifyOnInfo* pobjInfoNotify
                = static_cast<SmePlayerNotifyOnInfo*>(pobjMsg);
            if(E_SME_PLAYER_NOTIFY_INFO == pobjInfoNotify->GetNotifyType())
            {
                u32Res = m_pobjPlayerObs->PlayerNotify(
                    static_cast<V_UINT32>(E_SME_PLAYER_NOTIFY_INFO),
                    static_cast<V_UINT32>(pobjInfoNotify->GetExtVal1()),
                    static_cast<V_UINT32>(pobjInfoNotify->GetExtVal2()));
                UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("Notify player info failed."));
            }

            break;
        }
        case SmePlayerMsgBase::E_TYPE_ON_BUFFER_UPDATE:
        {
            SmePlayerNotifyOnBufferUpdate* pobjBufUpdateNotify
                = static_cast<SmePlayerNotifyOnBufferUpdate*>(pobjMsg);
            u32Res = m_pobjPlayerObs->PlayerNotify(
                static_cast<V_UINT32>(E_SME_PLAYER_NOTIFY_BUFFER_UPDATE),
                static_cast<V_UINT32>(pobjBufUpdateNotify->GetBufferedPercent()),
                static_cast<V_UINT32>(0));
            UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("Notify buffer update failed."));

            break;
        }
        case SmePlayerMsgBase::E_TYPE_ON_SEEKCOMPLTED:
        {
            SmePlayerNotifyOnSeekCompleted* pobjNotifyMsg
                = static_cast<SmePlayerNotifyOnSeekCompleted*>(pobjMsg);
            struct timeval sktime;
            V_UINT64 skstoptime;

            if(!m_bLoopSeeking)
            {
                ICS_LOGI("seek completly, callback to app now!");
                u32Res = m_pobjPlayerObs->PlayerNotify(
                    static_cast<V_UINT32>(E_SME_PLAYER_NOTIFY_SEEK_COMPLETED),
                    pobjNotifyMsg->GetErrorVal(), static_cast<V_UINT32>(
                    pobjNotifyMsg->GetSeekRes()));
                UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("Notify seek completed failed."));
            }
            else
            {
                ICS_LOGI("loop back seek to 0 completly!, ret=%d",
                    pobjNotifyMsg->GetErrorVal());
                m_bLoopSeeking = ICS_FALSE;
                if(E_SME_PLAYER_ERROR_NONE != pobjNotifyMsg->GetErrorVal())
                {
                    ICS_LOGW("loop playback seek failed, why?");
                    u32Res = m_pobjPlayerObs->PlayerNotify(E_SME_PLAYER_NOTIFY_ERROR,
                        E_SME_PLAYER_ERROR_UNKOWN, 0);
                    UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("Notify player error failed."));
                }
            }
            gettimeofday(&sktime, NULL);
            skstoptime = (V_UINT64)((V_INT64)(sktime.tv_sec) * 1000 + ((V_INT64)(sktime.tv_usec) + 500) / 1000);
            m_u64SeekTime = skstoptime - m_u64SeekTime;
            break;
        }
        case SmePlayerMsgBase::E_TYPE_ON_EOS:
        {
            V_UINT32 u32What = E_SME_PLAYER_NOTIFY_PLAYBACK_COMPLETED;
            V_UINT32 u32Extra1 = 0;
            V_UINT32 u32Extra2 = 0;
            V_BOOL bNeedCallback = ICS_TRUE;
            struct timeval quitime;
            V_UINT64 qtime;

            VOS_ThdMutexLock(&m_stMutex);
            if(m_bLoop)
            {
                if((0 == m_i32Duration) && m_pvGstPipeline)
                {
                    ICS_LOGI("get duration now...");
                    m_i32Duration = SmePlayerGstWrap::GetDuration(m_pvGstPipeline);
                    ICS_LOGI("get duration end,duration=%d", m_i32Duration);
                }

                if(m_i32Duration > 0)
                {
                    ICS_LOGI("seek to 0, playback!");
                    m_i32SeekTo = 0;
                    m_bSeeking  = ICS_TRUE;
                    m_bLoopSeeking = ICS_TRUE;
                    SmePlayerMsgSeekTo* pobjCmd = new SmePlayerMsgSeekTo(NULL, NULL, m_i32SeekTo);
                    V_UINT32 u32Err = m_pobjCmdRunner->SendMsg(pobjCmd);
                    if(E_SME_PLAYER_ERROR_NONE != u32Err)
                    {
                        ICS_LOGW("SeekTo:%d Failed(%#x),loop playback failed!", m_i32SeekTo, u32Err);
                        UTILS_MTMPDEL(pobjCmd);
                        m_i32SeekTo = -1;
                        m_bSeeking  = ICS_FALSE;
                        m_bLoopSeeking = ICS_FALSE;
                        u32What = E_SME_PLAYER_NOTIFY_ERROR;
                        u32Extra1 = E_SME_PLAYER_ERROR_UNKOWN;
                    }
                    else
                    {
                        bNeedCallback = ICS_FALSE;
                    }
                }
                else
                {
                    u32What = E_SME_PLAYER_NOTIFY_ERROR;
                    u32Extra1 = E_SME_PLAYER_ERROR_UNSUPPORT_LOOP_PLAYBACK;
                    ICS_LOGW("duration =%d , cannot support loop playback!");
                }
            }
            VOS_ThdMutexUnLock(&m_stMutex);

            if(bNeedCallback)
            {
                u32Res = m_pobjPlayerObs->PlayerNotify(u32What, u32Extra1, u32Extra2);
                UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("Notify player eos failed."));
            }
            gettimeofday(&quitime, NULL);
            qtime = (V_UINT64)((V_INT64)(quitime.tv_sec) * 1000 + ((V_INT64)(quitime.tv_usec) + 500) / 1000);
            if (m_procCommonInfo)
                m_procCommonInfo->quit_cost_time = qtime - m_u64ResetTime;

            break;
        }
        case SmePlayerMsgBase::E_TYPE_ON_PREPARED:
        {
            if(ICS_FALSE == m_bBuffering)
            {
                SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE_PREPARED);
                //m_eState = E_PLAYER_STATE_PREPARED;
                u32Res = m_pobjPlayerObs->PlayerNotify(
                    E_SME_PLAYER_NOTIFY_PREPARED, 0, 0);
                UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("Notify player on prepared failed."));
                struct timeval prepare_stop;

                gettimeofday(&prepare_stop, NULL);
                if (m_u64PrepareTime)
                {
                    V_UINT64 preparestop = 0;
                    preparestop = (V_UINT64)((V_INT64)(prepare_stop.tv_sec) * 1000 + ((V_INT64)(prepare_stop.tv_usec) + 500) / 1000);
                    m_u64PrepareTime = preparestop - m_u64PrepareTime;
                }
            }
            else
            {
                ICS_LOGI("buffering now, not notify prepared!");
                m_bPreParedNormal = ICS_TRUE;
            }

            break;
        }
        case SmePlayerMsgBase::E_TYPE_ON_ERROR:
        {
            SmePlayerNotifyOnError* pobjNotifyMsg =
                static_cast<SmePlayerNotifyOnError*>(pobjMsg);
            u32Res = m_pobjPlayerObs->PlayerNotify(E_SME_PLAYER_NOTIFY_ERROR,
                pobjNotifyMsg->GetErrorVal(), 0);
            UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("Notify player on error failed."));

            break;
        }
        case SmePlayerMsgBase::E_TYPE_ON_RESOLUTION_CHANGED:
        {
            SmePlayerNotifyOnResolutionChange* pobjNotifyMsg =
                static_cast<SmePlayerNotifyOnResolutionChange*>(pobjMsg);
            pobjNotifyMsg->GetResolution(m_u32ResolutionWidth, m_u32ResolutionHeight);
            u32Res = m_pobjPlayerObs->PlayerNotify(E_SME_PLAYER_NOTIFY_VIDEO_SIZE_CHANGED,
                m_u32ResolutionWidth, m_u32ResolutionHeight);
            UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("Notify resolution changed failed."));
            break;
        }
        case SmePlayerMsgBase::E_TYPE_ON_HTTP_ERR:
        {
            V_UINT32 statuscode = 0;
            SmePlayerNotifyOnHttpErr* pobjNotifyMsg =
                static_cast<SmePlayerNotifyOnHttpErr*>(pobjMsg);
            pobjNotifyMsg->GetStatusCode(statuscode);
            u32Res = m_pobjPlayerObs->PlayerNotify(E_SME_PLAYER_NOTIFY_HTTP_ERR,
                statuscode, 0);
            UTILS_MLOGW_IF(ICS_SUCCESS != u32Res,("Notify http error failed."));
            break;
        }
        case SmePlayerMsgBase::E_TYPE_ON_FRAGMENT_INFO_UPDATED:
        {
            ST_FRAGMENT_INFO stFragmentInfo;
            E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;

            if (E_SME_MEM_OK != VOS_Memset_S(
                &stFragmentInfo,
                sizeof(ST_FRAGMENT_INFO),
                0,
                sizeof(ST_FRAGMENT_INFO)))
            {
                ICS_LOGE("SECURE: VOS_Memset_S return failed.");
            }

            SmePlayerNotifyOnFragmentChanged* pobjNotifyMsg =
                static_cast<SmePlayerNotifyOnFragmentChanged*>(pobjMsg);
            pobjNotifyMsg->GetFragmentInfo(&stFragmentInfo);
            if (m_procCommonInfo)
            {
                eMemSecRet = VOS_Memcpy_S(m_procCommonInfo->fragment_info.i8ManifestUri,
                                          URI_MAX_LENGTH - 1,
                                          stFragmentInfo.i8ManifestUri,
                                          (V_UINT32)strlen(stFragmentInfo.i8ManifestUri));
                if (eMemSecRet != E_SME_MEM_OK)
                {
                    ICS_LOGE("VOS_Memcpy_S i8ManifestUri Failed(%d)", eMemSecRet);
                }
                eMemSecRet = VOS_Memcpy_S(m_procCommonInfo->fragment_info.i8FragmentUri,
                                          URI_MAX_LENGTH - 1,
                                          stFragmentInfo.i8FragmentUri,
                                          (V_UINT32)strlen(stFragmentInfo.i8FragmentUri));
                if (eMemSecRet != E_SME_MEM_OK)
                {
                    ICS_LOGE("VOS_Memcpy_S i8FragmentUri Failed(%d)", eMemSecRet);
                }
                m_procCommonInfo->fragment_info.u64FragmentSize = stFragmentInfo.u64FragmentSize;
                m_procCommonInfo->fragment_info.u64DownloadTotalTime = stFragmentInfo.u64DownloadTotalTime;
            }
            u32Res = m_pobjPlayerObs->PlayerNotify(E_SME_PLAYER_NOTIFY_FRAGMENT_UPDATE,
                (V_UINT32)&stFragmentInfo,0);
            UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("Notify fragment info update failed."));
            break;
        }
        default :
            ICS_LOGW("unsupport msg:%d", pobjMsg->GetType());
            break;
    }

    return;
}

V_UINT32 SmePlayerImplGst::HandleGetVideoWidth(SmePlayerMsgGetVideoWidth* pobjMsg) const
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)-1, ("must prepare first!"));

    //DOTO get width
    return (V_UINT32)SmePlayerGstWrap::GetVideoWidth(m_pvGstPipeline);
}

V_UINT32 SmePlayerImplGst::HandleGetVideoHeight(SmePlayerMsgGetVideoHeight* pobjMsg) const
{
    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_MLOGE_RET_VAL_IF(E_PLAYER_STATE_PREPARED >  m_eState,
        (V_UINT32)-1, ("must prepare first!"));

    //DOTO get height
    return (V_UINT32)SmePlayerGstWrap::GetVideoHeight(m_pvGstPipeline);
}

/*static*/
V_BOOL SmePlayerImplGst::IsCacheCtrlEnable(IN V_VOID* pvCtx,  IN const V_CHAR* pszURI)
{
    V_BOOL bRet = ICS_FALSE;

    UTILS_M_INVALID_ARGS(pvCtx);

    if(!strncasecmp(pszURI, SME_URI_PROTO_HTTP_PREFIX,
        strlen(SME_URI_PROTO_HTTP_PREFIX))
        || !strncasecmp(pszURI, SME_URI_PROTO_HTTPS_PREFIX,
        strlen(SME_URI_PROTO_HTTP_PREFIX))
        || !strncasecmp(pszURI, SME_URI_PROTO_RTSP_PREFIX,
        strlen(SME_URI_PROTO_HTTP_PREFIX))
        || !strncasecmp(pszURI, SME_URI_PROTO_RTP_PREFIX,
		strlen(SME_URI_PROTO_RTP_PREFIX)))
    {
        bRet = ICS_TRUE;
        //ICS_LOGI("net playing:cache control enable!url=%s", pszURI);
        ICS_LOGI("net playing:cache control enable!");
    }

    return bRet;
}

V_UINT32 SmePlayerImplGst::NoteCacheCtrl()
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    UTILS_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret);

    SmePlayerMsgCacheCtrl* pobjMsg = NULL;

    pobjMsg = new SmePlayerMsgCacheCtrl(NULL, NULL);
    u32Ret = m_pobjCmdRunner->SendMsg(pobjMsg);
    if(E_SME_PLAYER_ERROR_NONE != u32Ret)
    {
        ICS_LOGE("send CacheCtrl failed");
        UTILS_MTMPDEL(pobjMsg);
    }

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::HandleCacheCtrl(SmePlayerMsgCacheCtrl* pobjMsg)
{
    V_UINT32 u32Ret                 = E_SME_PLAYER_ERROR_NONE;
    V_UINT32 u32CurBw               = 0;//bps
    V_UINT32 u32StreamBps           = 0;//bps
    V_UINT32 u32ABuffered           = 0;//a buffered
    V_UINT32 u32VBuffered           = 0;//v buffer
    V_UINT32 u32VDecInterval        = 0;//v dec interval
    V_UINT32 u32VDecInDuration      = 0;//v decin duration
    V_UINT32 u32MinBuffed           = 0;
    V_UINT32 u32MaxBuffed           = 0;
    V_UINT32 u32Percent             = 0;
    SmePlayerNotifyBase* pobjNotify = NULL;

    UTILS_M_INVALID_ARGS(pobjMsg);

    UTILS_RET_VAL_IF(NULL == m_pobjMsgRunner, u32Ret);

    do
    {
        UTILS_M_BRK_VAL_IF(ICS_FALSE == m_bCacheCtrlEnable, u32Ret,
            E_SME_PLAYER_ERROR_UNKOWN);
        UTILS_MLOGW_BRK_VAL_IF(m_bSeeking, u32Ret,
            E_SME_PLAYER_ERROR_NONE, ("seeking not ctrl cache now!"));

        u32Ret = SmePlayerGstWrap::GetCacheInfo(m_pvGstPipeline, u32CurBw,
            u32StreamBps, u32ABuffered, u32VBuffered, u32VDecInterval,
            u32VDecInDuration);
        UTILS_MLOGE_BRK_IF(E_SME_PLAYER_ERROR_NONE != u32Ret,
            ("Get cacheinfo Failed(%d)", u32Ret));
        UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
            ("construct failed, any cmd cannot execute!"));
        UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjMsgRunner, u32Ret,
            ("construct failed, any cmd cannot execute!"));

        if(0 == m_i32Duration
            && m_eState >= E_PLAYER_STATE_PREPARED && m_eState <= E_PLAYER_STATE_ERROR)
        {
            m_i32Duration = SmePlayerGstWrap::GetDuration(m_pvGstPipeline);
        }

        u32VBuffered +=u32VDecInterval;
        u32MinBuffed = M_SME_MIN(u32ABuffered, u32VBuffered);
        u32MaxBuffed = M_SME_MAX(u32ABuffered, u32VBuffered);
#if 0
        if(u32MinBuffed >= SME_BUFFER_THREHOLD_NORMAL)
        {
            //ICS_LOGD("network is good,do not need adujst BufferThdUp!");
        }
        else
        {
            m_u32BufferThdUp = GetSmeBufferMin(u32CurBw, u32StreamBps,
                m_u64LastUpdateThdUpTime, m_u32BufferThdUp);
            if(m_i32Duration < 0 && m_u32BufferThdUp < (SME_BUFFER_THREHOLD_UP_MIN*10))
            {
                m_u32BufferThdUp = SME_BUFFER_THREHOLD_UP_MIN*10;
            }
        }
#endif
        //ICS_LOGI("HandleCacheCtrl:u32CurBw=%u, u32StreamBps=%u, u32ABuffered=%u,"
        //    "u32VBuffered=%u, u32MinBuffed=%u, u32VDecInterval=%u,u32VDecInDuration=%u, "
        //    "u32ThreholdUpMin=%u,m_bBuffering=%d,m_eState=%d",
        //    u32CurBw, u32StreamBps, u32ABuffered, u32VBuffered, u32MinBuffed,
        //    u32VDecInterval, u32VDecInDuration,
        //    m_u32BufferThdUp,m_bBuffering,m_eState);

        if((!m_bBuffering) && ((E_PLAYER_STATE_STARTED == m_eState)
                || (E_PLAYER_STATE_PREPARING == m_eState)))
        {
            if((u32MinBuffed <= m_u32BufferFull * m_u32BufferLowPercent / 100)
               && (u32MaxBuffed < m_u32BufferFull)
                && u32VDecInterval >= SME_VDECOUT_INTERVAL_MAX)
            {
                /*×´Ì¬ÇÐ»»:buffered->buffering*/
                m_bBuffering = ICS_TRUE;
                ICS_LOGI("buffered->buffering,state=%d,water mark:%d",
                         m_eState, u32MinBuffed * 100 / m_u32BufferFull);

                /*set playbin pause, if current is playing*/
                if(E_PLAYER_STATE_STARTED == m_eState)
                {
                    u32Ret = SmePlayerGstWrap::Pause(m_pvGstPipeline);
                    if(E_SME_PLAYER_ERROR_NONE != u32Ret)
                    {
                        m_bBuffering = ICS_FALSE;
                        u32Ret = E_SME_PLAYER_ERROR_UNKOWN;
                        ICS_LOGE("Pause failed(%d)", u32Ret);
                        break;
                    }
                    m_bPausedInternal = ICS_TRUE;
                }

                /*notify up to buffering start*/
                pobjNotify = dynamic_cast<SmePlayerNotifyBase*>(
                    new SmePlayerNotifyOnInfo(E_SME_PLAYER_INFO_BUFFERING_START,
                    0));
                u32Ret = m_pobjMsgRunner->SendMsg(pobjNotify);
                if(E_SME_PLAYER_ERROR_NONE != u32Ret)
                {
                    ICS_LOGE("send OnInfo failed");
                    UTILS_MTMPDEL(pobjNotify);
                }
                /*notify up to buffering percent*/
                u32Percent = (V_UINT32)((u32MinBuffed * 100)/((m_u32BufferFull * m_u32BufferHighPercent) / 100));
                pobjNotify = dynamic_cast<SmePlayerNotifyBase*>(
                    new SmePlayerNotifyOnBufferUpdate(u32Percent));
                u32Ret = m_pobjMsgRunner->SendMsg(pobjNotify);
                if(E_SME_PLAYER_ERROR_NONE != u32Ret)
                {
                    ICS_LOGE("send OnBufferUpdate failed");
                    UTILS_MTMPDEL(pobjNotify);
                }
            }
            else
            {
                /*notify up to prepared*/
                if(E_PLAYER_STATE_PREPARING == m_eState)
                {
                    if(m_bPreParedNormal)
                    {
                        ICS_LOGI("notify onPrepared!");
                        pobjNotify = dynamic_cast<SmePlayerNotifyBase*>(
                            new SmePlayerNotifyOnPrepared());
                        u32Ret = m_pobjMsgRunner->SendMsg(pobjNotify);
                        if(E_SME_PLAYER_ERROR_NONE != u32Ret)
                        {
                            ICS_LOGE("send OnPrepared failed");
                            UTILS_MTMPDEL(pobjNotify);
                        }
                    }
                }
            }
        }
        else if(m_bBuffering)
        {
            if((u32MinBuffed >= m_u32BufferFull * m_u32BufferHighPercent / 100)
               || (u32MaxBuffed >= m_u32BufferFull))
            {
                /*×´Ì¬ÇÐ»»:buffering->buffered*/
                ICS_LOGI("buffering->buffered,state=%d,water mark:%d",
                         m_eState, u32MinBuffed * 100/ m_u32BufferFull);
                m_u32NotifyPercent = 0;
                /*set playbin start, if current is playing*/
                if(E_PLAYER_STATE_STARTED == m_eState)
                {
                    u32Ret = SmePlayerGstWrap::Start(m_pvGstPipeline);
                    if(E_SME_PLAYER_ERROR_NONE != u32Ret)
                    {
                        m_bBuffering = ICS_FALSE;
                        u32Ret = E_SME_PLAYER_ERROR_UNKOWN;
                        ICS_LOGE("Start failed(%d)", u32Ret);
                        break;
                    }
                    m_bPausedInternal = ICS_FALSE;
                }

                /*notify up to buffering percent 100%*/
                u32Percent = 100;
                pobjNotify = dynamic_cast<SmePlayerNotifyBase*>(
                    new SmePlayerNotifyOnBufferUpdate(u32Percent));
                u32Ret = m_pobjMsgRunner->SendMsg(pobjNotify);
                if(E_SME_PLAYER_ERROR_NONE != u32Ret)
                {
                    ICS_LOGE("send Update failed");
                    UTILS_MTMPDEL(pobjNotify);
                }

                /*notify up to buffering stop%*/
                pobjNotify = dynamic_cast<SmePlayerNotifyBase*>(
                    new SmePlayerNotifyOnInfo(E_SME_PLAYER_INFO_BUFFERING_STOP,
                    0));
                u32Ret = m_pobjMsgRunner->SendMsg(pobjNotify);
                if(E_SME_PLAYER_ERROR_NONE != u32Ret)
                {
                    ICS_LOGE("send OnInfo failed");
                    UTILS_MTMPDEL(pobjNotify);
                }

                m_bBuffering = ICS_FALSE;

                /*notify up to prepared*/
                if(E_PLAYER_STATE_PREPARING == m_eState)
                {
                    if(m_bPreParedNormal)
                    {
                        ICS_LOGI("notify onPrepared!");
                        pobjNotify = dynamic_cast<SmePlayerNotifyBase*>(
                            new SmePlayerNotifyOnPrepared());
                        u32Ret = m_pobjMsgRunner->SendMsg(pobjNotify);
                        if(E_SME_PLAYER_ERROR_NONE != u32Ret)
                        {
                            ICS_LOGE("send OnPrepared failed");
                            UTILS_MTMPDEL(pobjNotify);
                        }
                    }
                }
            }
            else
            {
                /*notify up to buffering percent*/
                u32Percent = (V_UINT32)((u32MinBuffed * 100)/(m_u32BufferFull * m_u32BufferHighPercent / 100)) ;
                /** BEGIN: Added for DTS2015102708085 by baihaili(b00311320), 2015/10/26 */
                /** make sure it increases in buffering state*/
                m_u32NotifyPercent =
                    m_u32NotifyPercent > u32Percent ? m_u32NotifyPercent : u32Percent;
                /** END:   Added by baihaili(b00311320), 2015/10/26 */
                ICS_LOGI("notify buffering percent:%d",m_u32NotifyPercent);
                pobjNotify = dynamic_cast<SmePlayerNotifyBase*>(
                    new SmePlayerNotifyOnBufferUpdate(m_u32NotifyPercent));
                u32Ret = m_pobjMsgRunner->SendMsg(pobjNotify);
                if(E_SME_PLAYER_ERROR_NONE != u32Ret)
                {
                    ICS_LOGE("send OnBufferUpdate failed");
                    UTILS_MTMPDEL(pobjNotify);
                }
            }
        }
        else
        {
            ;//do nothing;
        }

        //update downloading offset
        pobjNotify = dynamic_cast<SmePlayerNotifyBase*>(
            new SmePlayerNotifyOnInfo(E_SME_PLAYER_INFO_BUFFERING_OFFSET,
            u32MinBuffed));
        u32Ret = m_pobjMsgRunner->SendMsg(pobjNotify);
        if(E_SME_PLAYER_ERROR_NONE != u32Ret)
        {
            ICS_LOGE("send OnInfo failed");
            UTILS_MTMPDEL(pobjNotify);
        }

    } while (ICS_FALSE);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::StartCacheProcTimer()
{
    V_UINT32 u32Ret = ICS_FAIL;

    do
    {
        UTILS_M_BRK_VAL_IF(!m_bCacheCtrlEnable, u32Ret, ICS_SUCCESS);

        u32Ret = m_objTimer.Open();
        UTILS_MLOGE_BRK_IF(ICS_SUCCESS != u32Ret, ("open timer failed(%#x)",
            u32Ret));

        u32Ret = m_objTimer.SetIntervalVal(SME_CACHE_PROC_INTERVAL);
        UTILS_MLOGW_IF(ICS_FAIL == u32Ret, ("Set intetval value failed."));
        u32Ret = m_objTimer.Start();
    } while (ICS_FALSE);

    if(ICS_SUCCESS != u32Ret)
    {
        m_objTimer.Close();
    }

    return u32Ret;
}

V_VOID SmePlayerImplGst::StopCacheProcTimer()
{
    V_UINT32 u32Ret = ICS_FAIL;

    u32Ret = m_objTimer.Pause();
    UTILS_MLOGW_IF(ICS_FAIL == u32Ret, ("pause timer failed."));
    m_objTimer.Close();

    return;
}

/*static*/
V_VOID SmePlayerImplGst::SetPlayerState(E_PLAYER_STATE eState)
{
    E_SME_PLAYER_STATE eOldSmeState, eNewSmeState;
    V_UINT32 u32Res = ICS_SUCCESS;

    eOldSmeState = GetPlayerState();
    m_eState = eState;
    eNewSmeState = GetPlayerState();
    if (eOldSmeState != eNewSmeState)
    {
        u32Res = m_pobjPlayerObs->PlayerNotify(
            E_SME_PLAYER_NOTIFY_PLAYER_STATE_CHANGED,
            eOldSmeState, eNewSmeState);
        UTILS_MLOGW_IF(ICS_SUCCESS != u32Res, ("Notify player state changed failed."));
    }
}


/*static*/
V_VOID SmePlayerImplGst::CacheTimerProcFunc(IN V_VOID* pvCtx)
{
    SmePlayerImplGst* pobjThis = static_cast<SmePlayerImplGst*>(pvCtx);

    if(NULL != pobjThis)
    {
        pobjThis->CacheTimerProc();
    }

    return;
}

V_VOID SmePlayerImplGst::CacheTimerProc()
{
    V_UINT32 u32Err;
    u32Err = NoteCacheCtrl();
    UTILS_MLOGW_IF(ICS_SUCCESS != u32Err, ("NoteCacheCtrl Failed(%#x)", u32Err));

    return;
}

V_UINT32 SmePlayerImplGst::SetSubDataSource(IN const V_CHAR* pstSubURI)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetSubDs* pobjMsg = NULL;

    ICS_LOGI("SetDataSource_sub in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetSubDs(SyncCmdCompletedCb, this, pstSubURI);
    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGI("SetDataSource out, u32Ret=%u", u32Ret);

    return u32Ret;

}

V_UINT32 SmePlayerImplGst::RegisterSubCallback (IN ST_SubCallback & stSubCB)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgRegSubCb* pobjMsg = NULL;

    ICS_LOGI("RegisterSubCallback in, cur state=%d", m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgRegSubCb(SyncCmdCompletedCb, this, &stSubCB);
    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGI("RegisterSubCallback out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::SetLoop(IN V_BOOL bLoop)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_STATE;
    SmePlayerMsgSetLoop* pobjMsg = NULL;

    ICS_LOGI("SetLoop in, bLoop=%d, cur state=%d", (V_INT32)bLoop, m_eState);

    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjCmdRunner, u32Ret,
        ("construct failed, any cmd cannot execute!"));

    pobjMsg = new SmePlayerMsgSetLoop(SyncCmdCompletedCb, this, bLoop);
    u32Ret = m_pobjCmdRunner->PostMsg(pobjMsg);

    UTILS_MSAFEDEL(pobjMsg);

    ICS_LOGI("SetLoop out, u32Ret=%u", u32Ret);

    return u32Ret;
}

V_UINT32 SmePlayerImplGst::HandleSetLoop(const SmePlayerMsgSetLoop* pobjMsg)
{
    UTILS_MLOGE_RET_VAL_IF(NULL == m_pobjPlayerObs, E_SME_PLAYER_ERROR_STATE,
        ("please set obj first!"));

    VOS_ThdMutexLock(&m_stMutex);
    m_bLoop = pobjMsg->GetLoopVal();
    VOS_ThdMutexUnLock(&m_stMutex);

    return static_cast<V_UINT32>(E_SME_PLAYER_ERROR_NONE);
}
