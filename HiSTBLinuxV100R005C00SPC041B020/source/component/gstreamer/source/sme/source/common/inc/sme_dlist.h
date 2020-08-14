/******************************************************************************

                  版权所有 (C), 2001-2012, 华为技术有限公司

 ******************************************************************************
  文 件 名   : sme_dlist.h
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2013年4月29日
  最近修改   :
  功能描述   : 基于upnp协议栈中的C双向链表封装的C++双向链表
  函数列表   :
  修改历史   :
  1.日    期   : 2013年4月29日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/

#ifndef __ICS_DLIST_H__
#define __ICS_DLIST_H__

#include "osal_type.h"
#include <stdio.h>

namespace ICS
{

class DList
{
public:
    struct Node
    {
        Node* pPrev;
        Node* pNext;
        V_VOID* pvItem;
    };

public:
    DList(IN V_VOID* pvUserData = NULL);
    virtual ~DList();

    Node* AddHead(IN V_VOID* pvItem);

    Node* AddTail(IN V_VOID* pvItem);

    Node* AddAfter(IN V_VOID* pvItem, IN Node* pNode);

    Node* AddBefore(IN V_VOID* pvItem, IN Node* pNode);

    /* 如果bFreeItem=TRUE则返回NULL，如果bFreeItem=FALSE,则返回节点中的item */
    V_VOID* DelNode(IN Node* pNode, IN V_BOOL bFreeItem);

    V_VOID DelAll(IN V_BOOL bFreeItem);

    Node* GetHead() const;

    Node* GetTail() const;

    Node* GetNext(IN Node* pNode) const;

    Node* GetPrev(IN Node* pNode) const ;

    Node* Find(IN V_VOID* pvItem, IN Node* pNodeStart = NULL);

    V_UINT32 GetSize();

public:
    virtual V_VOID OnItemFree(IN V_VOID* pvItem, IN V_VOID* pvUserData) = 0;

    /* 返回 =0表示两个参数相等 !=0表示两个参数不等(用于查找用) */
    virtual V_UINT32 OnCompareItem(IN V_VOID* pvItemA, IN V_VOID* pvItemB, IN V_VOID* pvUserData) = 0;

private:
    V_VOID* m_pvListHdl;
};

}

#endif
