/******************************************************************************

                  版权所有 (C), 2001-2012, 华为技术有限公司

 ******************************************************************************
  文 件 名   : sme_queue.cpp
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2013年4月29日
  最近修改   :
  功能描述   : 基于双向链表适配的队列
  函数列表   :
  修改历史   :
  1.日    期   : 2013年4月29日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/
/*lint -e717*/          // do while(0)
/*lint -e1551*/         // 析构函数内不抛异常

#include "sme_queue.h"
#include "sme_dlist.h"
#include "sme_log.h"
#include "sme_macro.h"

#undef MOD_NAME
#define MOD_NAME "utils"

using namespace ICS;

namespace ICS
{

class QueDList : public DList
{
public:
    QueDList(Queue* pobjQue) : DList(pobjQue) {};
    QueDList() {};
    virtual ~QueDList() {};

    virtual V_VOID OnItemFree(IN V_VOID* pvItem, IN V_VOID* pvUserData)
    {
        if (NULL == pvUserData)
        {
            ICS_LOGE("error param(pvUserData)");
            return;
        }

        ((Queue*)pvUserData)->OnFreeMsg(pvItem);
        return;
    };

    virtual V_UINT32 OnCompareItem(IN V_VOID* /*pvItemA*/, IN V_VOID* /*pvItemB*/, IN V_VOID* /*pvUserData*/)
    {
        return 0;
    };
};

}

/*lint -save -e* */
Queue::Queue(V_UINT32 u32MaxSize/* = 0*/)
{
    m_u32MaxSize = u32MaxSize;
    V_UINT32 u32Ret = VOS_InitThdMutex(&m_stMutex, NULL);
    if (E_VOS_THD_ERR_NONE != u32Ret)
    {
        return;
    }

    m_pobjDlist = new QueDList(this);
    if (NULL == m_pobjDlist)
    {
        VOS_DeInitThdMutex(&m_stMutex);
        ICS_LOGE("new dlist fail");
    }
}
/*lint -restore * */

Queue::~Queue()
{
    if (NULL != m_pobjDlist)
    {
        m_pobjDlist->DelAll(ICS_TRUE);
        delete m_pobjDlist;
        m_pobjDlist = NULL;
        VOS_DeInitThdMutex(&m_stMutex);
    }
}

V_UINT32 Queue::Push(V_VOID* pvItem)
{
    UTILS_RET_VAL_IF(NULL == m_pobjDlist, ICS_FAIL);
    if ((0 != m_u32MaxSize) && (this->GetSize() >= m_u32MaxSize))
    {
        ICS_LOGW("add tail to dlist fail, queue if full");
        return ICS_FAIL;
    }

    VOS_ThdMutexLock(&m_stMutex);
    DList::Node* pNode = m_pobjDlist->AddTail(pvItem);
    VOS_ThdMutexUnLock(&m_stMutex);
    if (NULL == pNode)
    {
        ICS_LOGW("add tail to dlist fail");
        return ICS_FAIL;
    }

    return ICS_SUCCESS;
}

V_UINT32 Queue::Pop(V_VOID*& pvItem)
{
    V_UINT32 u32Ret = ICS_FAIL;
    UTILS_RET_VAL_IF(NULL == m_pobjDlist, ICS_FAIL);

    VOS_ThdMutexLock(&m_stMutex);
    do
    {
        DList::Node* pNode = m_pobjDlist->GetHead();
        if (NULL == pNode)
        {
            ICS_LOGW("get head from dlist fail");
            break;
        }

        pvItem = m_pobjDlist->DelNode(pNode, ICS_FALSE);
        u32Ret = ICS_SUCCESS;
    }while(ICS_FALSE);
    VOS_ThdMutexUnLock(&m_stMutex);

    return u32Ret;
}

V_VOID Queue::ClearQueue(V_BOOL bFree/* = ICS_TRUE*/)
{
    UTILS_MLOGE_RET_IF(NULL == m_pobjDlist, ("uninited"));

    VOS_ThdMutexLock(&m_stMutex);
    m_pobjDlist->DelAll(bFree);
    VOS_ThdMutexUnLock(&m_stMutex);
}


/*V_UINT32 Queue::Peek(V_VOID*& pvItem)
{
    UTILS_RET_VAL_IF(NULL == m_pobjDlist, ICS_FAIL);
    DList::Node* pNode = m_pobjDlist->GetHead();
    if (NULL == pNode)
    {
        ICS_LOGW("get head from dlist fail");
        return ICS_FAIL;
    }

    pvItem = pNode->pvItem;
    return ICS_SUCCESS;
}*/

V_UINT32 Queue::GetSize()
{
    UTILS_RET_VAL_IF(NULL == m_pobjDlist, ICS_FAIL);
    return m_pobjDlist->GetSize();
}
