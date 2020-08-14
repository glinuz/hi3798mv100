/*
 * hisilicon clock Management framework Routines
 *
 *
 * Copyright (C) 2012 Hisilicon Instruments, Inc.
 *
 
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/clk.h>
#include <linux/mutex.h>

#include <asm/clkdev.h>
#include <linux/io.h>
#include "clock.h"

static DEFINE_MUTEX(clock_list_lock);
static struct list_head hiclocks;
static DEFINE_MUTEX(clocks_mutex);
static DEFINE_SPINLOCK(clocks_lock);

static inline void clk_lock_init(struct clk *c)
{
	mutex_init(&c->mutex);
	spin_lock_init(&c->spinlock);
}

unsigned long hi_clk_get_rate(struct clk *c)
{
	unsigned long flags;
	unsigned long rate;

    if (c == 0)
    {
        return -EINVAL;
    }

    spin_lock_irqsave(&clocks_lock, flags);

	rate = c->rate;

	spin_unlock_irqrestore(&clocks_lock, flags);

	return rate;
}
EXPORT_SYMBOL(hi_clk_get_rate);

int hi_clk_enable(struct clk *c)
{
	int ret = 0;

	unsigned long flags;

    if (c == 0)
    {
        return -EINVAL;
    }

	/*
	 * if c->refcnt == 0,clk hasn't been enabled ,
	 * shoud enable if ,else just refcnt ++
	 */
	if (c->refcnt == 0) {
		if (c->parent) {
			ret = hi_clk_enable(c->parent); /* enable parent */
			if (ret) {
				WARN(1, "enable clock:%s failed", c->name);
				goto out;
			}
		}

		if (c->ops && c->ops->enable) {
			ret = c->ops->enable(c);
			if (ret) { /* if enable faild ,disable the parent */
				if (c->parent)
					hi_clk_disable(c->parent);
				WARN(1, "enable clock:%s failed", c->name);
				goto out;
			}
		}
		spin_lock_irqsave(&clocks_lock, flags);
		c->state = ON;
		c->set = true;
		spin_unlock_irqrestore(&clocks_lock, flags);
	}
	spin_lock_irqsave(&clocks_lock, flags);
	c->refcnt++;
	spin_unlock_irqrestore(&clocks_lock, flags);
out:
	return ret;
}
EXPORT_SYMBOL(hi_clk_enable);

void hi_clk_disable(struct clk *c)
{
	unsigned long flags;
    
    if (c == 0)
    {
        return;
    }

	if (c->refcnt == 0) {
		WARN(1, "Attempting to disable clock %s with refcnt 0",
			c->name);
		return;
	}
	if (c->refcnt == 1) {
		if (c->ops && c->ops->disable)
			c->ops->disable(c);

		if (c->parent)
			hi_clk_disable(c->parent);
		spin_lock_irqsave(&clocks_lock, flags);
		c->state = OFF;
		spin_unlock_irqrestore(&clocks_lock, flags);
	}
	spin_lock_irqsave(&clocks_lock, flags);
	c->refcnt--;
	spin_unlock_irqrestore(&clocks_lock, flags);
}
EXPORT_SYMBOL(hi_clk_disable);

int hi_clk_set_parent(struct clk *c, struct clk *parent)
{
	int ret = 0;

    if((c == 0) || (parent == 0))
    {
        return -EINVAL;
    }

	if (!c->ops || !c->ops->set_parent) {
		ret = -ENOSYS;
		goto out;
	}

	ret = c->ops->set_parent(c, parent);
out:
	return ret;
}
EXPORT_SYMBOL(hi_clk_set_parent);

struct clk *hi_clk_get_parent(struct clk *c)
{
	return c->parent;
}
EXPORT_SYMBOL(hi_clk_get_parent);

int hi_clk_set_rate(struct clk *c, unsigned long rate)
{
	int ret = 0;

    if (!c)
    {
        return -EINVAL;
    }

	if (!c->ops || !c->ops->set_rate) {
		ret = -ENOSYS;
		goto out;
	}

	if (rate > c->max_rate)
		rate = c->max_rate;

	ret = c->ops->set_rate(c, rate);
	if (ret)
		goto out;

out:
	return ret;
}
EXPORT_SYMBOL(hi_clk_set_rate);

long hi_clk_round_rate(struct clk *c, unsigned long rate)
{
	long ret;

    if (c == 0)
    {
        return -EINVAL;
    }

	if (!c->ops || !c->ops->round_rate) {
		ret = -ENOSYS;
		goto out;
	}

	if (rate > c->max_rate)
		rate = c->max_rate;

	ret = c->ops->round_rate(c, rate);

out:
	return ret;
}
EXPORT_SYMBOL(hi_clk_round_rate);

void hi_clk_init(struct clk *c)
{
	INIT_LIST_HEAD(&hiclocks);

    if (c == 0)
    {
        return;
    }
    
	clk_lock_init(c);

	if (c->ops && c->ops->init)
		c->ops->init(c);

	if (!c->ops || !c->ops->enable) {

		c->refcnt++;
		c->set = true;

		if (c->parent)
			c->state = c->parent->state;
		else
			c->state = ON;
	}

	mutex_lock(&clock_list_lock);
	list_add(&c->node, &hiclocks);
	mutex_unlock(&clock_list_lock);
}
EXPORT_SYMBOL(hi_clk_init);

void hi_clk_exit(struct clk *c)
{
    if (c == 0)
    {
        return;
    }
    
	mutex_lock(&clock_list_lock);
	list_del(&c->node);
	mutex_unlock(&clock_list_lock);
	return;
}
EXPORT_SYMBOL(hi_clk_exit);
