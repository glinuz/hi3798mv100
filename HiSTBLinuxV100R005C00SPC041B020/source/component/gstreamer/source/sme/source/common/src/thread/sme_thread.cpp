/******************************************************************************

                  版权所有 (C), 2001-2012, 华为技术有限公司

 ******************************************************************************
  文 件 名   : sme_thread.cpp
  版 本 号   : 初稿
  作    者   : h00207960
  生成日期   : 2013年4月29日
  最近修改   :
  功能描述   : 线程类
  函数列表   :
  修改历史   :
  1.日    期   : 2013年4月29日
    作    者   : h00207960
    修改内容   : 创建文件

******************************************************************************/
/*lint -e717*/ // do while(0)
/*lint -e838*/ // initial
#include "sme_thread.h"
#include "osal_thread.h"
#include "sme_macro.h"
#include "sme_log.h"
#include "osal_str.h"
#include "osal_mem.h"

#undef MOD_NAME
#define MOD_NAME "utils"

#define DEFAULT_THREAD_NAME "sme_thread"

using namespace ICS;

V_UINT32 Thread::StaticThread(V_VOID* pvParam)
{
    Thread* pThread = (Thread*)pvParam;
    if (NULL == pThread)
    {
        return ICS_FAIL;
    }

    pThread->m_thdId = VOS_GetSelfThreadId();

    pThread->OnStartup();
    pThread->Process();
    pThread->OnExit();

    if (!pThread->m_bWaitFlag)
    {
        VOS_DetachThread(pThread->m_thdHdl);
        pThread->m_thdHdl = NULL;

        pThread->GetLock();
        pThread->ReleaseLock();

        delete pThread;
    }

    return ICS_SUCCESS;
}

Thread::Thread(V_BOOL bWait/* = ICS_TRUE*/) :
m_bStop(ICS_FALSE),
m_bWaitFlag(bWait),
m_thdHdl(NULL),
m_thdId(0)
{
    E_SME_MEM_SECURE_RET eRet = E_SME_MEM_OK;
    E_VOS_STR_ERR eErr = E_VOS_STR_ENONE;
    V_UINT32 u32Ret;

    u32Ret = VOS_InitThdMutex(&m_stMutex, NULL);
    if(u32Ret != E_VOS_THD_ERR_NONE)
    {
        ICS_LOGW("VOS_InitThdMutex fail:%u.", u32Ret);
    }

    m_eventHdl = VOS_CreateThdEvent(NULL);
    eRet = VOS_Memset_S(m_acThreadName, sizeof(m_acThreadName), 0, sizeof(m_acThreadName));
    if(E_SME_MEM_OK != eRet)
    {
        ICS_LOGW("VOS_Memset_S failed:%d", eRet);
    }

    eErr = VOS_Strncpy_S(m_acThreadName, sizeof(m_acThreadName),
        DEFAULT_THREAD_NAME, sizeof(m_acThreadName) - 1);
    if(eErr != E_VOS_STR_ENONE)
    {
        ICS_LOGW("VOS_Strncpy_S failed:%d", eErr);
    }
}

Thread::~Thread()
{
    VOS_DeInitThdMutex(&m_stMutex);
    VOS_DestroyThdEvent(m_eventHdl);
    m_thdHdl = NULL;
    m_eventHdl = NULL;
}

V_UINT32 Thread::Create(V_INT32 i32Sched /*= E_VOS_THD_SCHED_OTHER*/,
                    V_INT32 i32Pri /*= E_VOS_THD_PRI_INVALID*/)
{
    V_UINT32 u32Ret = ICS_FAIL;

    VOS_ThdMutexLock(&m_stMutex);
    do
    {
        if (NULL != m_thdHdl)
        {
            ICS_LOGW("thread is already running");
            break;
        }

        m_bStop = ICS_FALSE;
        ST_VOS_THDATTR stThdAttr;
        stThdAttr.i32StackSize = -1;//忽略，采用默认
        stThdAttr.i32Sched = i32Sched;
        stThdAttr.i32Prioty = i32Pri;
        u32Ret = VOS_CreateThread(&m_thdHdl,
                                (const ST_VOS_THDATTR*)&stThdAttr,
                                (PFNVOSTHD)StaticThread,
                                this);
        if (E_VOS_THD_ERR_NONE != u32Ret)
        {
            ICS_LOGW("create thread fail");
        }

    }while(ICS_FALSE);
    VOS_ThdMutexUnLock(&m_stMutex);

    if (E_VOS_THD_ERR_NONE == u32Ret)
    {
        return ICS_SUCCESS;
    }
    else
    {
        return ICS_FAIL;
    }
}

V_VOID Thread::SetThreadName(IN const V_CHAR* pszThreadName)
{
    E_SME_MEM_SECURE_RET eRet = E_SME_MEM_OK;
    E_VOS_STR_ERR eErr = E_VOS_STR_ENONE;

    UTILS_MLOGE_RET_IF(NULL == pszThreadName, ("ics_thread SetThreadName fail, invalid param"));
    eRet = VOS_Memset_S(m_acThreadName, sizeof(m_acThreadName), 0, sizeof(m_acThreadName));
    if(E_SME_MEM_OK != eRet)
    {
        ICS_LOGW("VOS_Memset_S failed:%d", eRet);
    }

    eErr = VOS_Strncpy_S(m_acThreadName, sizeof(m_acThreadName),
        pszThreadName, sizeof(m_acThreadName) - 1);
    if(eErr != E_VOS_STR_ENONE)
    {
        ICS_LOGW("VOS_Strncpy_S failed:%d", eErr);
    }
}

V_VOID Thread::StopThread()
{
    V_UINT32 u32Ret;

    VOS_ThdMutexLock(&m_stMutex);
    if (NULL != m_thdHdl)
    {
        m_bStop = ICS_TRUE;
        VOS_ThdEventSignal(m_eventHdl);
        if (m_bWaitFlag)
        {
            u32Ret = VOS_JoinThread(m_thdHdl);
            if(u32Ret != E_VOS_THD_ERR_NONE)
            {
                ICS_LOGW("VOS_JoinThread failed:%u", u32Ret);
            }
            m_thdHdl = NULL;
        }
    }
    else
    {
        ICS_LOGW("thread is not running");
    }
    VOS_ThdMutexUnLock(&m_stMutex);

    return;
}

V_VOID Thread::GetLock()
{
    VOS_ThdMutexLock(&m_stMutex);
}

V_VOID Thread::ReleaseLock()
{
    VOS_ThdMutexUnLock(&m_stMutex);
}

V_VOID Thread::Sleep(IN V_UINT32 u32Milsecs)
{
    V_UINT32 u32Ret;

    if((u32Milsecs > 10) && IsCurrentThread())
    {
        u32Ret = VOS_ThdEventTimeWait(m_eventHdl, u32Milsecs);
        if(u32Ret != ICS_SUCCESS)
        {
            ICS_LOGW("VOS_ThdEventTimeWait fail:%u.", u32Ret);
        }
    }
    else
    {
        VOS_MSleep(u32Milsecs);
    }
}

V_BOOL Thread::IsCurrentThread() const
{
    if (VOS_GetSelfThreadId() == m_thdId)
    {
        return ICS_TRUE;
    }
    else
    {
        return ICS_FALSE;
    }
}
