#ifndef __ICS_MSG_QUEUE_H__
#define __ICS_MSG_QUEUE_H__

#include "sme_queue.h"
#include "sme_thread.h"
#include "osal_thread.h"

namespace ICS
{

/* 单线程消息队列 */
class MsgQueue : private ICS::Thread, protected ICS::Queue
{
public:
    MsgQueue();
    virtual ~MsgQueue();

    V_UINT32 Start();
    V_VOID Stop();
    V_VOID ClearMsgQueue(V_BOOL bFree = ICS_TRUE);

    V_UINT32 PushMsg(IN V_VOID* pvMsg);

protected:
    virtual V_VOID OnGetAMsg(IN V_VOID* pvMsg) = 0;

    /* 当消息队列被销毁时，如果还有剩余的消息，使用该接口释放消息 */
    virtual V_VOID OnFreeMsg(IN V_VOID* pvMsg) {pvMsg=pvMsg;};

private:
    virtual V_VOID OnStartup();
    virtual V_VOID Process();
    virtual V_VOID OnExit();


private:
    VOSSEMHDL m_hSem;
    V_BOOL m_bInitOK;
};

}
#endif
