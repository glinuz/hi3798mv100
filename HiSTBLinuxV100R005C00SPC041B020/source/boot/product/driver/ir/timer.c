#include "timer.h"
#include "hi_type.h"

#if 0
struct timer_list {
	struct list_head entry;
	unsigned long expires;
	void (*function)(unsigned long);
	unsigned long data;
};
#endif
static struct list_head timer_head = {&timer_head, &timer_head};

void add_timer(struct timer_list *timer)
{
	if (!timer)
		return ;
	if ((timer->entry.prev != &timer->entry && timer->entry.prev != NULL)
		|| (timer->entry.next != &timer->entry && timer->entry.next != NULL)) {
		timer_error("Del timer first before add_timer");
		return;
	}

	list_add(&timer->entry, &timer_head);
}
void del_timer(struct timer_list *timer)
{
	if (!timer)
		return;
	if ((timer->entry.prev == NULL || timer->entry.prev == &timer->entry) 
		&& (timer->entry.next == NULL || timer->entry.next == &timer->entry)) {
		//timer_error("Timer have been deleted!\n");
		return;
	}

	list_del_init(&timer->entry);
}
void init_timer(struct timer_list *timer)
{
	if (!timer)
		return;
	timer->entry.next = timer->entry.prev = &timer->entry;
}
void proc_timer_list(void)
{
	struct list_head *cn;
	struct timer_list *ci ;
	//unsigned long curr_ms;
	//curr_ms = (get_ticks() * 1000) / CONFIG_SYS_HZ;
	if (list_empty_careful(&timer_head))
		return;
	ci = list_first_entry(&timer_head, struct timer_list, entry);
	do {
		if (ci->expires <= jiffies) {
			if (ci->function)
				ci->function(ci->data);
			cn = ci->entry.next;
			del_timer(ci);
			if (list_is_last(cn, &timer_head))
				break;
			ci = list_entry(cn, struct timer_list, entry);
		} 
		else if (list_is_last(&ci->entry, &timer_head))
			break;
		else
			ci = list_entry(ci->entry.next, struct timer_list, entry);
	} while(!list_is_last(&ci->entry, &timer_head));
}


