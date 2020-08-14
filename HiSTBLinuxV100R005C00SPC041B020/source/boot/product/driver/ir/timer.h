#ifndef HI_MINIBOOT_SUPPORT
#include <linux/list.h>
#include <common.h>
#else
#include "app.h"
#endif

#ifdef HI_MINIBOOT_SUPPORT
struct list_head {
	struct list_head *next, *prev;
};
#endif

struct timer_list {
	struct list_head entry;
	unsigned long expires;
	void (*function)(unsigned long);
	unsigned long data;
};
#define del_timer_sync(t) del_timer(t)
void add_timer(struct timer_list *timer);
void del_timer(struct timer_list *timer);
void init_timer(struct timer_list *timer);
void proc_timer_list(void);

#define timer_error(arg...)\
do {\
	printf(arg);\
}while(0)

#ifdef HI_MINIBOOT_SUPPORT
extern unsigned int get_timer_clock(void);

#define CONFIG_SYS_HZ           (get_timer_clock() >> 8)
#endif

#define time_after_eq(a, b) ((long)(a) - (long)(b) >= 0)
#define time_after(a, b)        ((long)(b) - (long)(a) < 0)
#define jiffies ((unsigned long)(get_ticks()))   //current loadvalue of timer..
#define msecs_to_jiffies(m) ((m) * CONFIG_SYS_HZ / 1000) // one HZ<-->1s, so: ms/1000 *HZ

#ifdef HI_MINIBOOT_SUPPORT

#ifndef ARCH_HAS_PREFETCH
#define ARCH_HAS_PREFETCH
static inline void __attribute__((unused)) prefetch(const void *x) {;}
#endif

/**
 * list_empty_careful - tests whether a list is empty and not being modified
 * @head: the list to test
 *
 * Description:
 * tests whether a list is empty _and_ checks that no other CPU might be
 * in the process of modifying either member (next or prev)
 *
 * NOTE: using list_empty_careful() without synchronization
 * can only be safe if the only activity that can happen
 * to the list entry is list_del_init(). Eg. it cannot be used
 * if another CPU could re-list_add() it.
 */
static inline int __attribute__((unused)) list_empty_careful(const struct list_head *head)
{
	struct list_head *next = head->next;
	return (next == head) && (next == head->prev);
}

#undef offsetof
#define offsetof(__type, __member) ((unsigned int) &((__type *)0)->__member)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#undef container_of
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * list_first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

/**
 * list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int __attribute__((unused)) list_is_last(const struct list_head *list,
				const struct list_head *head)
{
	return list->next == head;
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void __attribute__((unused)) list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void __attribute__((unused)) list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void __attribute__((unused)) list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int __attribute__((unused)) list_empty(const struct list_head *head)
{
	return head->next == head;
}

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; prefetch(pos->next), pos != (head); \
		pos = pos->next)


#endif

