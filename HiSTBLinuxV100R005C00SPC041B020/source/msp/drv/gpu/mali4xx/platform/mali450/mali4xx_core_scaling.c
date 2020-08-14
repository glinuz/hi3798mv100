/*
 * Copyright (C) 2013 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @file arm_core_scaling.c
 * Example core scaling policy.
 */

#include <linux/mali/mali_utgard.h>
#include <linux/workqueue.h>

#include "mali_kernel_common.h"

#include "mali4xx_core_scaling.h"
#include "mali4xx_pmu.h"

static int num_cores_total;
static int num_cores_enabled;

static struct work_struct wq_work;

static unsigned int utilization_gp_value;
static unsigned int utilization_pp_value;

static void set_num_cores(struct work_struct *work)
{
    int err = mali_perf_set_num_pp_cores(num_cores_enabled);

    MALI_DEBUG_ASSERT(0 == err);
    MALI_IGNORE(err);
}

static void enable_fix_num_cores(unsigned int num)
{
	if(num > 0 && num <= num_cores_total)
	{
		num_cores_enabled = num ;
		schedule_work(&wq_work);
		MALI_DEBUG_PRINT(3, ("Core scaling: Enabling %d cores\n", num_cores_enabled));
	}
}

void mali_core_scaling_init(int num_pp_cores)
{
    INIT_WORK(&wq_work, set_num_cores);

    num_cores_total   = num_pp_cores;
    num_cores_enabled = num_pp_cores;

    /* NOTE: Mali is not fully initialized at this point. */
}

void mali_core_scaling_term(void)
{
    flush_scheduled_work();
}

#define PERCENT_OF(percent, max) ((int) ((percent) * (max) / 100.0 + 0.5))

void mali_core_scaling_update(struct mali_gpu_utilization_data *data)
{
    u32 utilization = data->utilization_pp > data->utilization_gp ? data->utilization_pp : data->utilization_gp;

    utilization_gp_value = data->utilization_gp;
    utilization_pp_value = data->utilization_pp;

    /*
     * This function implements a very trivial PP core scaling algorithm.
     *
     * It is _NOT_ of production quality.
     * The only intention behind this algorithm is to exercise and test the
     * core scaling functionality of the driver.
     * It is _NOT_ tuned for neither power saving nor performance!
     *
     * Other metrics than PP utilization need to be considered as well
     * in order to make a good core scaling algorithm.
     */

    MALI_DEBUG_PRINT(2, ("Utilization: (%3d, %3d, %3d), cores enabled: %d/%d\n", data->utilization_gpu,
                         data->utilization_gp, data->utilization_pp, num_cores_enabled, num_cores_total));

    mali_gpu_utilization_handler(utilization);
}

unsigned int gpu_get_gp_utilization(void)
{
    return utilization_gp_value;
}

unsigned int gpu_get_pp_utilization(void)
{
    return utilization_pp_value;
}

unsigned int gpu_get_pp_scale_cores(void)
{
	return num_cores_enabled ;
}

unsigned int gpu_set_pp_scale_cores(unsigned int num)
{
	if(num > num_cores_total)
	{
		num = num_cores_total ;
	}
	else if(num < 1)
	{
		num = 1;
	}
    
	enable_fix_num_cores(num);

    return num ;
}

