
/**
* @file sre_list.ph
*
* Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
*
* 描述：双向链表操作的内部头文件。 \n
*/

#ifndef _SRE_LIST_PH
#define _SRE_LIST_PH

#include "sre_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


typedef struct tagListObject {
	struct tagListObject *pstPrev;
	struct tagListObject *pstNext;
} LIST_OBJECT_S;



/**
 *@ingroup SRE_list
 *初始化链表(链表重用时的初始化)
 *
 *@param head [IN] 链表头结点的地址(The address of the head of a list )
 */
#define LIST_INIT(head) do {\
    (head)->pstNext = (head)->pstPrev = (head); \
}while(0)



#define LIST_OBJECT_INIT(stObject) { &(stObject), &(stObject) }

#define LIST_OBJECT(stObject) \
    LIST_OBJECT_S stObject = LIST_OBJECT_INIT(stObject)

#define INIT_LIST_OBJECT(pstObject) \
    do \
    { \
        (pstObject)->pstNext = (pstObject); \
        (pstObject)->pstPrev = (pstObject); \
    }while (0)

#define LIST_FIRST(pstObject) ((pstObject)->pstNext)

/**************************************************************************//**
 @Function      ListLowLevelAdd

 @Description   Insert a new_entry entry between two known consecutive entries.
                This is only for internal list manipulation where we know
                the prev/next entries already.

 @Param         new_entry  - New list object.
 @Param         next - Before new_entry.
 @Param         prev - After new_entry.

*//***************************************************************************/
OS_SEC_ALW_INLINE INLINE VOID ListLowLevelAdd(LIST_OBJECT_S *pstNewNode,
		LIST_OBJECT_S *pstPrev,
		LIST_OBJECT_S *pstNext)
{
	pstNewNode->pstNext = pstNext;
	pstNewNode->pstPrev = pstPrev;
	pstNext->pstPrev    = pstNewNode;
	pstPrev->pstNext    = pstNewNode;
}

/**************************************************************************//**
 @Function      ListAdd

 @Description   add a new_entry entry in list_object of list.

 @Param         new_entry  - new_entry entry to be added.
 @Param         list_object - list list_object to add it after.

*//***************************************************************************/
OS_SEC_ALW_INLINE INLINE VOID ListAdd(LIST_OBJECT_S *pstNewNode, LIST_OBJECT_S *pstListObject)
{
	ListLowLevelAdd(pstNewNode, pstListObject, pstListObject->pstNext);
}

/**************************************************************************//**
 @Function      ListTailAdd

 @Description   add a new_entry entry before list_object.

 @Param         new_entry  - new_entry entry to be added.
 @Param         list_object - list list_object to add it after.

*//***************************************************************************/
OS_SEC_ALW_INLINE INLINE VOID ListTailAdd(LIST_OBJECT_S *pstNewNode, LIST_OBJECT_S *pstListObject)
{
	ListLowLevelAdd(pstNewNode, pstListObject->pstPrev, pstListObject);
}



/**************************************************************************//**
 @Function      ListLowLevelDelete

 @Description   Delete a list entry by making the prev/next entries point to
                each other.

 @Param         prev - Before deleted item.
 @Param         next - After deleted item.

*//***************************************************************************/
OS_SEC_ALW_INLINE INLINE VOID ListLowLevelDelete(LIST_OBJECT_S *pstPrevNode, LIST_OBJECT_S *pstNextNode)
{
	pstNextNode->pstPrev = pstPrevNode;
	pstPrevNode->pstNext = pstNextNode;
}

/**************************************************************************//**
 @Function      ListDelete

 @Description   deletes entry from list.

 @Param         prev - Before deleted item.
 @Param         next - After deleted item.

*//***************************************************************************/
OS_SEC_ALW_INLINE INLINE VOID ListDelete(LIST_OBJECT_S *pstNode)
{
	ListLowLevelDelete(pstNode->pstPrev, pstNode->pstNext);
	pstNode->pstNext = NULL;
	pstNode->pstPrev = NULL;
}


/**************************************************************************//**
 @Function      ListMove

 @Description   delete from one list and add as another's list_object.

 @Param         list - the entry to move.
 @Param         list_object - the list_object that will precede our entry.

*//***************************************************************************/
OS_SEC_ALW_INLINE INLINE VOID ListMove(LIST_OBJECT_S *pstList, LIST_OBJECT_S *pstListObject)
{
	ListLowLevelDelete(pstList->pstPrev, pstList->pstNext);
	ListAdd(pstList, pstListObject);
}

/**************************************************************************//**
 @Function      ListMoveTail

 @Description   delete from one list and add as another's tail.

 @Param         list - the entry to move.
 @Param         list_object - the list_object that will precede our entry.

*//***************************************************************************/
OS_SEC_ALW_INLINE INLINE VOID ListMoveTail(LIST_OBJECT_S *pstList, LIST_OBJECT_S *pstListObject)
{
	ListLowLevelDelete(pstList->pstPrev, pstList->pstNext);
	ListTailAdd(pstList, pstListObject);
}

/**************************************************************************//**
 @Function      ListEmpty

 @Description   tests whether a list is empty.

 @Param         list_object - the list to test.

 @Retval        TRUE if empty.

*//***************************************************************************/
OS_SEC_ALW_INLINE INLINE BOOL ListEmpty(const LIST_OBJECT_S *pstListObject)
{
	return (BOOL)(pstListObject->pstNext == pstListObject);
}

typedef unsigned int    size_t;
#define OFFSET_OF_FIELD( type, field )    ((size_t)&(((type*)0)->field))

#define COMPLEX_OF(ptr, type, field) ((type *)( (char *)(ptr) - OFFSET_OF_FIELD(type,field) ))


/* 根据成员地址得到控制块首地址, ptr成员地址, type控制块结构, field成员名 */
#define LIST_COMPONENT(ptr, type, field) \
        COMPLEX_OF(ptr, type, field)


#define LIST_FOR_EACH(pos, list_object, type, field)            \
        for (pos = LIST_COMPONENT((list_object)->pstNext, type, field);          \
             &pos->field != (list_object);                            \
             pos = LIST_COMPONENT(pos->field.pstNext, type, field))

#define LIST_FOR_EACH_SAFE(pos, n, list_object, type, field)     \
        for (pos = LIST_COMPONENT((list_object)->pstNext, type, field),         \
                n = LIST_COMPONENT(pos->field.pstNext, type, field);    \
             &pos->field != (list_object);                                      \
             pos = n, n = LIST_COMPONENT(n->field.pstNext, type, field))


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* _SRE_LIST_PH */

/**
 * History:
 * 2008-02-20 l66919: Create this file.
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */

