/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.
******************************************************************************
 * @file    sme_timer.cpp
 * @brief   用sme_thread实现timer类实现
 * @author
 * @date    2014/6/4
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/6/4
 * Author :
 * Desc   : Created file
 *
******************************************************************************/
#include "sme_macro.h"
#include "sme_log.h"
#include "sme_timer.h"
#include <string.h>

//lint -e716 // while(TRUE) 正常循环逻辑

using namespace ICS;

#define TIMER_NAME (m_pszTimerName ? m_pszTimerName : "IcsTimer")

Timer::Timer(IN const V_CHAR* pszTimerName, IN pfTimerFunc pfnTimer,
    IN V_VOID* pvCtx /*= NULL*/,
    IN V_UINT32 u32IntervalVal /*= ICS_TIMER_INTERVAL_DEFAULT*/)
:m_pfnTimer(pfnTimer)
,m_pvCtx(pvCtx)
,m_u32IntervalVal(u32IntervalVal)
,m_bRun(ICS_FALSE)
,m_bExit(ICS_FALSE)
,m_bInited(ICS_FALSE)
,m_pszTimerName(NULL)
,m_hdlCond(NULL)
{
    V_UINT32 u32Ret;

    if(0 == m_u32IntervalVal)
    {
        m_u32IntervalVal = ICS_TIMER_INTERVAL_MIN;
        ICS_LOGW("interval val = 0, set to default=%u", m_u32IntervalVal);
    }

    if(NULL != pszTimerName)
    {
        m_pszTimerName = strdup(pszTimerName);
    }

    u32Ret = VOS_InitThdMutex(&m_stMutex, NULL);
    if(u32Ret != E_VOS_THD_ERR_NONE)
    {
        ICS_LOGW("VOS_InitThdMutex fail:%u.", u32Ret);
    }
    m_hdlCond = VOS_CreateThdCond(NULL);

    ICS_LOGD("Timer:%s, constructor successful.", TIMER_NAME);
}

Timer::~Timer()
{
    m_pfnTimer  = NULL;
    m_pvCtx     = NULL;
    VOS_DeInitThdMutex(&m_stMutex);
    VOS_DestroyThdCond(m_hdlCond);
    m_hdlCond = NULL;
    UTILS_MSAFEFREE(m_pszTimerName);
    m_pszTimerName = NULL;

    ICS_LOGD("Timer:%s, deconstructor successful.", TIMER_NAME);
}

V_UINT32 Timer::SetIntervalVal(V_UINT32 u32Val)
{
    V_UINT32 u32Ret = ICS_FAIL;

    VOS_ThdMutexLock(&m_stMutex);

    if(m_bInited)
    {
        m_u32IntervalVal = u32Val;
        if(0 == m_u32IntervalVal)
        {
            m_u32IntervalVal = ICS_TIMER_INTERVAL_MIN;
            ICS_LOGW("interval val = 0, set to default=%u", m_u32IntervalVal);
        }
        u32Ret = ICS_SUCCESS;
    }

    VOS_ThdMutexUnLock(&m_stMutex);

    if(ICS_SUCCESS == u32Ret)
    {
        ICS_LOGD("Timer:%s, SetIntervalVal ok.interval=%d",
            TIMER_NAME, m_u32IntervalVal);
    }
    else
    {
        ICS_LOGE("Timer:%s, SetIntervalVal failed.interval=%d",
            TIMER_NAME, m_u32IntervalVal);
    }

    return u32Ret;
}

V_UINT32 Timer::Open()
{
    V_UINT32 u32Ret = ICS_FAIL;

    VOS_ThdMutexLock(&m_stMutex);

    if((!m_bInited) && (NULL != m_hdlCond))
    {
        m_bExit = ICS_FALSE;
        u32Ret = this->Create();
        if(ICS_SUCCESS == u32Ret)
        {
            m_bInited = ICS_TRUE;
        }
    }
    else if(NULL != m_hdlCond)
    {
        u32Ret = ICS_SUCCESS;
    }
    else
    {
        ICS_LOGE("Timer:%s, Open failed, m_hdlCond is null", TIMER_NAME);
    }

    VOS_ThdMutexUnLock(&m_stMutex);

    if(ICS_SUCCESS == u32Ret)
    {
        ICS_LOGD("Timer:%s, Open ok", TIMER_NAME);
    }
    else
    {
        ICS_LOGE("Timer:%s, Open failed", TIMER_NAME);
    }

    return u32Ret;
}

V_VOID Timer::Close()
{
    VOS_ThdMutexLock(&m_stMutex);
    if(!m_bInited)
    {
        VOS_ThdMutexUnLock(&m_stMutex);
        ICS_LOGD("Timer:%s, Close ok", TIMER_NAME);
        return;
    }
    m_bExit = ICS_TRUE;
    m_bRun = ICS_FALSE;
    VOS_ThdCondSignal(m_hdlCond);
    VOS_ThdMutexUnLock(&m_stMutex);

    this->StopThread();

    VOS_ThdMutexLock(&m_stMutex);
     m_bInited = ICS_FALSE;
    VOS_ThdMutexUnLock(&m_stMutex);

    ICS_LOGD("Timer:%s, Close ok", TIMER_NAME);

    return;
}

V_UINT32 Timer::Start()
{
    V_UINT32 u32Ret = ICS_FAIL;

    VOS_ThdMutexLock(&m_stMutex);
    if(m_bInited)
    {
        m_bRun = ICS_TRUE;
        VOS_ThdCondSignal(m_hdlCond);
        u32Ret = ICS_SUCCESS;
    }
    VOS_ThdMutexUnLock(&m_stMutex);

    if(ICS_SUCCESS == u32Ret)
    {
        ICS_LOGD("Timer:%s, Start ok", TIMER_NAME);
    }
    else
    {
        ICS_LOGE("Timer:%s, Start failed", TIMER_NAME);
    }

    return u32Ret;
}

V_UINT32 Timer::Pause()
{
    V_UINT32 u32Ret = ICS_FAIL;

    VOS_ThdMutexLock(&m_stMutex);
    if(m_bInited)
    {
        m_bRun = ICS_FALSE;
        u32Ret = ICS_SUCCESS;
    }
    VOS_ThdMutexUnLock(&m_stMutex);

    if(ICS_SUCCESS == u32Ret)
    {
        ICS_LOGD("Timer:%s, Pause ok", TIMER_NAME);
    }
    else
    {
        ICS_LOGE("Timer:%s, Pause failed", TIMER_NAME);
    }

    return u32Ret;

}

V_VOID Timer::Process()
{
    ICS_LOGI("timer proccess in.");

    while(ICS_TRUE)
    {
        VOS_ThdMutexLock(&m_stMutex);

        if((m_bExit) || (m_bStop))
        {
            VOS_ThdMutexUnLock(&m_stMutex);
            ICS_LOGI("timer proccess to exit:bExit=%d,bStop=%d", m_bExit, m_bStop);
            break;
        }

        if(NULL != m_pfnTimer)
        {
            m_pfnTimer(m_pvCtx);
        }
        else
        {
            ICS_LOGW("timer callback func is null");
        }

        if(!m_bRun)
        {
            VOS_ThdCondWait(m_hdlCond, &m_stMutex);
        }
        else
        {
            VOS_ThdCondTimeWait(m_hdlCond, &m_stMutex, m_u32IntervalVal);
        }

        VOS_ThdMutexUnLock(&m_stMutex);
    }

    ICS_LOGI("timer proccess out.");

    return;
}

V_VOID Timer::OnStartup()
{
    return;
}

V_VOID Timer::OnExit()
{
    return;
}

Timer::Timer()
:m_pfnTimer(NULL)
,m_pvCtx(NULL)
,m_u32IntervalVal(ICS_TIMER_INTERVAL_DEFAULT)
,m_bRun(ICS_FALSE)
,m_bExit(ICS_FALSE)
,m_bInited(ICS_FALSE)
,m_pszTimerName(NULL)
,m_hdlCond(NULL)
{
    V_UINT32 u32Ret;
    u32Ret = VOS_InitThdMutex(&m_stMutex, NULL);
    if(u32Ret != E_VOS_THD_ERR_NONE)
    {
        ICS_LOGW("VOS_InitThdMutex fail:%u.", u32Ret);
    }
}
