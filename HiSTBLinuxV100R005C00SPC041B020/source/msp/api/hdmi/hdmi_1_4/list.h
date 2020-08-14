/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : list.h
  Version       : v1.0
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/05 
  Author        : 
  Description   :
  History       :
  UpDate        : 
  Author        : 
  Modification  :
*******************************************************************************/

#ifndef _TX_LIST_H_
#define _TX_LIST_H_

#ifndef _LINUX_LIST_H

#ifndef TX_INLINE
#define TX_INLINE __inline
#endif

typedef unsigned long TX_UPTR;

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define INIT_LIST_HEAD(ptr) do { (ptr)->next = (ptr); (ptr)->prev = (ptr); } while (0)

static TX_INLINE void __list_add(struct list_head * _new, struct list_head * prev, struct list_head * next)
{
	next->prev = _new;
	_new->next = next;
	_new->prev = prev;
	prev->next = _new;
}

static TX_INLINE void list_add(struct list_head *_new, struct list_head *head)
{
	__list_add(_new, head, head->next);
}

static TX_INLINE void list_add_tail(struct list_head *_new, struct list_head *head)
{
	__list_add(_new, head->prev, head);
}

static TX_INLINE void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

static TX_INLINE void list_del(const struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}

static TX_INLINE void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry); 
}

static TX_INLINE int list_empty(const struct list_head *head)
{
	return head->next == head;
}

static TX_INLINE void __list_splice(const struct list_head *list,
				 struct list_head *head)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;
	struct list_head *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

static TX_INLINE void list_splice(const struct list_head *list, struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head);
}

static TX_INLINE void list_splice_init(struct list_head *list, struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head);
		INIT_LIST_HEAD(list);
	}
}

/*#define list_entry(ptr, type, member) \
 ((type *)((char *)(ptr)-(HPT_UPTR)(&((type *)0)->member))) */
#define list_entry(ptr, type, member) \
	((type *)((unsigned long)(ptr)-((unsigned long)(&((type *)4)->member) - 4)))

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

#define get_first_item(attached, type, member) \
	((type *)((char *)((attached)->next)-(TX_UPTR)(&((type *)0)->member)))

#endif

#endif
