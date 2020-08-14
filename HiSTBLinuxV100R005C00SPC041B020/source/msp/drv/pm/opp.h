/* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. */

#ifndef __LINUX_OPP_H__
#define __LINUX_OPP_H__

#include <linux/err.h>
#include <linux/cpufreq.h>
#include <linux/notifier.h>

struct opp;
struct device;

enum opp_event {
	OPP_EVENT_ADD, OPP_EVENT_ENABLE, OPP_EVENT_DISABLE,
};


unsigned long opp_get_voltage(struct opp *opp);

unsigned long opp_get_freq(struct opp *opp);

int opp_get_opp_count(struct device *dev);

struct opp *opp_find_freq_exact(struct device *dev, unsigned long freq,
				bool available);

struct opp *opp_find_freq_floor(struct device *dev, unsigned long *freq);

struct opp *opp_find_freq_ceil(struct device *dev, unsigned long *freq);

int opp_add(struct device *dev, unsigned long freq, unsigned long u_volt);

int opp_enable(struct device *dev, unsigned long freq);

int opp_disable(struct device *dev, unsigned long freq);

struct srcu_notifier_head *opp_get_notifier(struct device *dev);

int opp_init_cpufreq_table(struct device *dev,
			    struct cpufreq_frequency_table **table);
void opp_free_cpufreq_table(struct device *dev,
				struct cpufreq_frequency_table **table);
#endif		/* __LINUX_OPP_H__ */
