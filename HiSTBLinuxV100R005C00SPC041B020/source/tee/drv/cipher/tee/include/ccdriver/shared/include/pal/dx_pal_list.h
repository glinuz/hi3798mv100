/***************************************************************
*  Copyright 2014 (c) Discretix Technologies Ltd.              *
*  This software is protected by copyright, international      *
*  treaties and various patents. Any copy, reproduction or     *
*  otherwise use of this software must be authorized in a      *
*  license agreement and include this Copyright Notice and any *
*  other notices specified in the license agreement.           *
*  Any redistribution in binary form must be authorized in the *
*  license agreement and include this Copyright Notice and     *
*  any other notices specified in the license agreement and/or *
*  in materials provided with the binary distribution.         *
****************************************************************/

#ifndef  DX_PAL_LIST_H
#define  DX_PAL_LIST_H


typedef struct DX_PAL_ListItem {
	struct DX_PAL_ListItem *prev;
	struct DX_PAL_ListItem *next;
} DX_PAL_ListItem_s;

/*!
 * Initializes a list. Prev/Next points to the same head object.
 * 
 * \param head The head of the list.
 */
static inline void DX_PAL_ListInit(DX_PAL_ListItem_s *head)
{
	head->prev = head;
	head->next = head;
}

/*!
 * Add a new list item after head of list.
 * 
 * \param new New entry to be added
 * \param head List head to add it after
 */
static inline void DX_PAL_ListAdd(DX_PAL_ListItem_s *new, DX_PAL_ListItem_s *head)
{
	DX_PAL_ListItem_s *next = head->next;
	
	next->prev = new;
	new->next = next;
	new->prev = head;
	head->next = new;
}

/*!
 * Add a new list item after head of list.
 * 
 * \param new New entry to be added
 * \param head List head to add it after
 */
static inline void DX_PAL_ListAddTail(DX_PAL_ListItem_s *new, DX_PAL_ListItem_s *head)
{
	DX_PAL_ListItem_s *prev = head->prev;

	prev->next = new;
	new->next = head;
	new->prev = prev;
	head->prev = new;
}

/*!
 * Deletes entry from list.
 * 
 * \param item The item to delete from the list.
 */
static inline void DX_PAL_ListDel(DX_PAL_ListItem_s *item)
{
	DX_PAL_ListItem_s *prev = item->prev;
	DX_PAL_ListItem_s *next = item->next;

	prev->next = next;
	next->prev = prev;

	item->next = item;
	item->prev = item;
}

/*!
 * Checks whether a list is empty.
 * 
 * \param head The list's head
 * 
 * \return int True if empty list, False otherwise.
 */
static inline int DX_PAL_IsListEmpty(const DX_PAL_ListItem_s *head)
{
	return (head->next == head);
}

#endif

