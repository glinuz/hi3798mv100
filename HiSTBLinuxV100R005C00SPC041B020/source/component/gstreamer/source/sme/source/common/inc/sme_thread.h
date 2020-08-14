/******************************************************************************

                  版权所有 (C), 2001-2012, 华为技术有限公司

 ******************************************************************************
  文 件 名   : sme_thread.h
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2013年4月29日
  最近修改   :
  功能描述   : 线程类
  函数列表   :
  修改历史   :
  1.日    期   : 2013年4月29日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/
#ifndef __ICS_THREAD_H__
#define __ICS_THREAD_H__

#include "osal_thread.h"
#include "osal_type.h"

#define THREAD_NAME_BUF_SIZE    20

namespace ICS
{

class Thread
{
public:
    /*
     * bWait=true 需要调用StopThread停止线程或释放资源，同时需要手动释放创建的对象
     * bWait=false 则为异步过程，不需要调用StopThread，并且线程结束会自动释放对象
     */
    Thread(V_BOOL bWait = ICS_TRUE);
    virtual ~Thread();

    V_UINT32 Create(V_INT32 i32Sched = E_VOS_THD_SCHED_OTHER,
                    V_INT32 i32Pri = E_VOS_THD_PRI_INVALID);

    V_VOID SetThreadName(IN const V_CHAR* pszThreadName);
    const V_CHAR* GetThreadName() {return m_acThreadName;};

    V_VOID StopThread();
    V_VOID Sleep(V_UINT32 u32Milsecs);

protected:
    virtual V_VOID OnStartup() = 0;
    virtual V_VOID Process() = 0;
    virtual V_VOID OnExit() = 0;

    V_VOID GetLock();
    V_VOID ReleaseLock();

protected:
    volatile V_BOOL m_bStop;

private:
    V_BOOL IsCurrentThread() const;
    static V_UINT32 StaticThread(V_VOID* pvParam);
    V_BOOL m_bWaitFlag;
    VOSTHDHDL m_thdHdl;
    VOS_TID_T m_thdId;
    VOSEVENTHDL m_eventHdl;
    ST_VOS_THD_MUTEX m_stMutex;
    V_CHAR m_acThreadName[THREAD_NAME_BUF_SIZE];
};

}

#endif
