#ifndef TIMER_H
#define TIMER_H

#include "list.h"

#define TEST_TIMER  0

struct timer_list {
	struct list_head entry;
	unsigned long long expires;
	unsigned long data;
	unsigned long long slack;
	void (*function)(unsigned long);
};

void init_timer(struct timer_list *timer);

int add_timer(struct timer_list *timer);

void del_timer(struct timer_list *timer);

int mod_timer(struct timer_list *timer, unsigned long expire_us);

#endif
