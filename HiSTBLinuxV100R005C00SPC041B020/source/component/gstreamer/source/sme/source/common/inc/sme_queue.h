/******************************************************************************

                  版权所有 (C), 2001-2012, 华为技术有限公司

 ******************************************************************************
  文 件 名   : sme_queue.h
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
#ifndef __ICS_QUEUE_H__
#define __ICS_QUEUE_H__

#include "osal_type.h"
#include "sme_dlist.h"
#include "osal_thread.h"

namespace ICS
{

class QueDList;

class Queue
{
friend class QueDList;
public:
    /* u32MaxSize=0不限制队列大小 */
    Queue(V_UINT32 u32MaxSize = 0);
    virtual ~Queue();

    V_UINT32 Push(V_VOID* pvItem);

    V_UINT32 Pop(V_VOID*& pvItem);

    V_VOID ClearQueue(V_BOOL bFree = ICS_TRUE);

    //V_UINT32 Peek(V_VOID*& pvItem);

    V_UINT32 GetSize();

protected:
    virtual V_VOID OnFreeMsg(V_VOID* pvItem) {pvItem = pvItem;};

private:
    DList* m_pobjDlist;
    V_UINT32 m_u32MaxSize;
    ST_VOS_THD_MUTEX m_stMutex;
};

}

#endif
