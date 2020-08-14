#ifndef __HI_LIST_H__
#define __HI_LIST_H__

#include "linux_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */


typedef struct list_head List_Head_S;

typedef List_Head_S HI_S_List_Head;
// jimuzhou add for dvb use
typedef List_Head_S HI_List_Head_S;
//jimuzhou add end
/*
#define LIST_HEAD_INIT(name) { &(name), &(name) }
*/
#define HI_LIST_HEAD_INIT_DEFINE(name) LIST_HEAD_INIT(name)

/*
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)
*/
#define HI_LIST_HEAD_DEFINE(name) LIST_HEAD(name)

/*
#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)
*/
#define HI_LIST_HEAD_INIT_PTR(ptr) INIT_LIST_HEAD(ptr)

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
/*
static __inline__ void __list_add(struct list_head * new,
    struct list_head * prev,
    struct list_head * next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}
*/
/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
/*
static __inline__ void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}
*/
#define HI_List_Add(pstruNew, pstruHead) list_add(pstruNew, pstruHead)

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
/*
static __inline__ void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}
*/
#define HI_List_Add_Tail(pstruNew, pstruHead) list_add_tail(pstruNew, pstruHead)

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
/*
static __inline__ void __list_del(struct list_head * prev,
                  struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}
*/
/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
 */
/*
static __inline__ void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = entry->prev = 0;
}
*/
#define HI_List_Del(pstruEntry) list_del(pstruEntry)

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
/*
static __inline__ void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}
*/
#define HI_List_Del_Init(pstruEntry) list_del_init(pstruEntry)

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
/*
static __inline__ int list_empty(struct list_head *head)
{
    return head->next == head;
}
*/
#define HI_List_Empty(pstruHead) list_empty(pstruHead)

/**
 * list_splice - join two lists
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
/*
static __inline__ void list_splice(struct list_head *list, struct list_head *head)
{
    struct list_head *first = list->next;

    if (first != list) {
        struct list_head *last = list->prev;
        struct list_head *at = head->next;

        first->prev = head;
        head->next = first;

        last->next = at;
        at->prev = last;
    }
}
*/
#define HI_List_Splice(pstruList, pstruHead) list_splice(pstruList, pstruHead)

/**
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_struct within the struct.
 */
/*
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
*/
#define HI_LIST_ENTRY(ptr, type, member) list_entry(ptr, type, member)

/**
 * list_for_each    -    iterate over a list
 * @pos:    the &struct list_head to use as a loop counter.
 * @head:    the head for your list.
 */
/*
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); \
            pos = pos->next)
*/
#define HI_List_For_Each(pos, head) list_for_each(pos, head)

/**
 * list_for_each_safe    -    iterate over a list safe against removal of list entry
 * @pos:    the &struct list_head to use as a loop counter.
 * @n:        another &struct list_head to use as temporary storage
 * @head:    the head for your list.
 */
/*
#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)
*/
#define HI_List_For_Each_Safe(pos, n, head) list_for_each_safe(pos, n, head)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__HI_LIST_H__*/
