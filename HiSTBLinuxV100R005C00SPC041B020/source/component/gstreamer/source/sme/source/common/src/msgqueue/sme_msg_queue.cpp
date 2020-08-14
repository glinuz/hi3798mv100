#include "sme_msg_queue.h"
#include "sme_log.h"
#include "sme_macro.h"

#undef MOD_NAME
#define MOD_NAME "utils"

using namespace ICS;

MsgQueue::MsgQueue() : Thread(ICS_TRUE), Queue(0), m_bInitOK(ICS_FALSE)
{
    m_hSem = VOS_CreateThdSem(0);
    if (NULL == m_hSem)
    {
        ICS_LOGW("init semaphore fail");
    }
    else
    {
        m_bInitOK = ICS_TRUE;
    }
}

MsgQueue::~MsgQueue()
{
    if (m_bInitOK)
    {
        VOS_DestroyThdSem(m_hSem);
    }
    m_hSem = NULL;
    m_bInitOK = ICS_FALSE;
}

V_UINT32 MsgQueue::Start()
{
    return this->Create();
}

V_VOID MsgQueue::Stop()
{
    m_bStop = ICS_TRUE;
    VOS_ThdSemPost(m_hSem);
    this->StopThread();
}

V_VOID MsgQueue::ClearMsgQueue(V_BOOL bFree/* = ICS_TRUE*/)
{
    this->ClearQueue(bFree);
}

V_UINT32 MsgQueue::PushMsg(IN V_VOID* pvMsg)
{
    UTILS_MLOGW_RET_VAL_IF(!m_bInitOK, ICS_FAIL, ("init failed"));
    V_UINT32 u32Ret = this->Push(pvMsg);
    if (ICS_SUCCESS == u32Ret)
    {
        VOS_ThdSemPost(m_hSem);
    }

    return u32Ret;
}

V_VOID MsgQueue::OnStartup()
{
    return;
}

V_VOID MsgQueue::Process()
{
    V_UINT32 u32Ret = 0;
    V_VOID* pvMsg = NULL;
    while(!m_bStop)
    {
        u32Ret = VOS_ThdSemWait(m_hSem);
        if(u32Ret != E_VOS_THD_ERR_NONE)
        {
            ICS_LOGW("VOS_ThdSemWait fail:%u.", u32Ret);
        }

        if (m_bStop)
        {
            ICS_LOGD("stop signal detected");
            break;
        }

        u32Ret = this->Pop(pvMsg);
        if (ICS_SUCCESS == u32Ret)
        {
            this->OnGetAMsg(pvMsg);
        }
    }

    return;
}

V_VOID MsgQueue::OnExit()
{
    this->ClearMsgQueue(ICS_TRUE);
    return;
}
