/*
 * hi-cpufreq.c - hisilicon Processor cpufreq Driver
 *
 * Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/smp.h>
#include <linux/sched.h>
#include <linux/cpufreq.h>
#include <linux/compiler.h>
#include <linux/dmi.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/version.h>
//#include <linux/opp.h>
#include "opp.h"
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include "clock.h"
#include <linux/platform_device.h>

#include <asm/processor.h>
#include <linux/cpu.h>
#include <asm/smp_plat.h>
#include <asm/cpu.h>
#include "hi_dvfs.h"

//#include "mach/hipm.h"
#include "hipm.h"
#include "hi_drv_pmoc.h"



static struct cpufreq_frequency_table *freq_table;

static atomic_t freq_table_users = ATOMIC_INIT(0);
static unsigned int max_freq;
static unsigned int current_target_freq;
static bool hi_cpufreq_ready;
unsigned int cpu_dvfs_enable = HI_TRUE;

static DEFINE_MUTEX(hi_cpufreq_lock);

struct clk *pMpuClk;
static unsigned int hi_cpufreq_getspeed(unsigned int cpu)
{
    unsigned int rate;

    if (cpu >= NR_CPUS)
    {
        return 0;
    }

    rate = (unsigned int)hi_clk_get_rate(pMpuClk);

    return rate;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0))
static int hi_cpufreq_scale(unsigned int target_freq, unsigned int cur_freq)
#else
static int hi_cpufreq_scale(struct cpufreq_policy *policy, unsigned int target_freq, unsigned int cur_freq)
#endif
{


    int ret;
    struct cpufreq_freqs freqs;

    freqs.new = target_freq;
    freqs.old = hi_cpufreq_getspeed(0);

    if ((freqs.old == freqs.new) && (cur_freq == freqs.new))
    {
        return 0;
    }

    get_online_cpus();

    /* notifiers */
    for_each_online_cpu(freqs.cpu)
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0))
    cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);
#else
 #if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
    cpufreq_notify_transition(policy, &freqs, CPUFREQ_PRECHANGE);
 #else
    cpufreq_freq_transition_begin(policy, &freqs);
 #endif
#endif
    //HI_INFO_PM("hi_cpufreq: transition: %u --> %u\n", freqs.old, freqs.new);
    ret = hi_device_scale(&mpu_dev, freqs.old, freqs.new );

    freqs.new = hi_cpufreq_getspeed(0);


    /* notifiers */
    for_each_online_cpu(freqs.cpu)
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0))
    cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
#else
 #if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
    cpufreq_notify_transition(policy, &freqs, CPUFREQ_POSTCHANGE);
 #else
    cpufreq_freq_transition_end(policy, &freqs, 0);
 #endif
#endif
    put_online_cpus();

    return ret;
}

static int hi_cpufreq_target(struct cpufreq_policy *policy,
                             unsigned int target_freq, unsigned int relation)
{
    unsigned int i;
    int ret = 0;

    if (!freq_table)
    {
        HI_ERR_PM("%s: cpu%d: no freq table!\n", __func__,
                  policy->cpu);
        return -EINVAL;
    }

    ret = cpufreq_frequency_table_target(policy, freq_table, target_freq,
                                         relation, &i);
    if (ret)
    {
        HI_WARN_PM("%s: cpu%d: no freq match for %d(ret=%d)\n",
                   __func__, policy->cpu, target_freq, ret);
        return ret;
    }

    mutex_lock(&hi_cpufreq_lock);

    current_target_freq = freq_table[i].frequency;

    if (cpu_dvfs_enable)
    {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0))
    ret = hi_cpufreq_scale(current_target_freq, policy->cur);
#else
        ret = hi_cpufreq_scale(policy, current_target_freq, policy->cur);
#endif
    }

    mutex_unlock(&hi_cpufreq_lock);

    return ret;
}

static int hi_cpufreq_verify(struct cpufreq_policy *policy)
{
    if (!freq_table)
    {
        return -EINVAL;
    }

    return cpufreq_frequency_table_verify(policy, freq_table);
}

static inline void freq_table_free(void)
{
    if (atomic_dec_and_test(&freq_table_users))
    {
        opp_free_cpufreq_table(&mpu_dev, &freq_table);
    }
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 72))
extern void cpufreq_interactive_boost(void);
#else
extern int cpufreq_interactive_boostpulse(void);
#endif

int pm_cpufreq_boost(void)
{
    int ret;
    struct cpufreq_policy cur_policy;
    const char name[CPUFREQ_NAME_LEN] = "interactive";

    ret = cpufreq_get_policy(&cur_policy, 0);
    if (ret)
    {
        return ret;
    }

    if (!strnicmp(cur_policy.user_policy.governor->name, name, CPUFREQ_NAME_LEN))
    {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 72))
        cpufreq_interactive_boost();
#else
        ret = cpufreq_interactive_boostpulse();
        if (ret)
        {
            return ret;
        }
#endif
    }

    return 0;
}

#ifndef HI_ADVCA_FUNCTION_RELEASE
 #ifndef MODULE
static void cpufreq_interactive_input_event(struct input_handle *handle,
                                            unsigned int type,
                                            unsigned int code, int value)
{
    if ((type == EV_SYN) && (code == SYN_REPORT))
    {
        pm_cpufreq_boost();
    }

    return;
}

static int cpufreq_interactive_input_connect(struct input_handler *handler,
                                          struct input_dev *dev,
                                          const struct input_device_id *id)
{
	struct input_handle *handle;
	int error;

	handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
	if (!handle)
	{
		return -ENOMEM;
	}
    
	handle->dev = dev;
	handle->handler = handler;
	handle->name = "cpufreq_interactive";

	error = input_register_handle(handle);
	if (error) 
    {
		pr_err("Failed to register input cpufreq_interactive handler, error %d\n",
			   error);
		kfree(handle);
		return error;
	}

	error = input_open_device(handle);
	if (error) 
    {
		pr_err("Failed to open input cpufreq_interactive device, error %d\n", error);
		input_unregister_handle(handle);
		kfree(handle);
		return error;
	}

	return 0;
}

static void cpufreq_interactive_input_disconnect(struct input_handle *handle)
{
	input_close_device(handle);
	input_unregister_handle(handle);
	kfree(handle);
}

static const struct input_device_id cpufreq_interactive_ids[] = {
        { .driver_info = 1 },   /* Matches all devices */
        { },                    /* Terminating zero entry */
};

static struct input_handler cpufreq_interactive_input_handler = {  
       .event          = cpufreq_interactive_input_event,  
       .connect        = cpufreq_interactive_input_connect,  
       .disconnect     = cpufreq_interactive_input_disconnect,  
       .name           = "cpufreq_interactive",  
       .id_table       = cpufreq_interactive_ids,  
};  
#endif
#endif

extern struct clk mpu_ck;
static int hi_cpufreq_cpu_init(struct cpufreq_policy *policy)
{
    int result = 0;
    int i;

    HI_INFO_PM("Enter %s\n", __FUNCTION__);
    pMpuClk = &mpu_ck;
    if (IS_ERR(pMpuClk))
    {
        return PTR_ERR(pMpuClk);
    }

    if (policy->cpu >= 1)
    {
        result = -EINVAL;
        goto fail_ck;
    }

    policy->cur = policy->min = policy->max = hi_cpufreq_getspeed(policy->cpu);

    if (atomic_inc_return(&freq_table_users) == 1)
    {
        result = opp_init_cpufreq_table(&mpu_dev, &freq_table);
    }

    if (result)
    {
        HI_ERR_PM("%s: cpu%d: failed creating freq table[%d]\n",
                __func__, policy->cpu, result);
        goto fail_ck;
    }
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
    result = cpufreq_frequency_table_cpuinfo(policy, freq_table);
    if (result)
    {
        goto fail_table;
    }

    cpufreq_frequency_table_get_attr(freq_table, policy->cpu);
#else
    result = cpufreq_table_validate_and_show(policy, freq_table);
    if (result)
    {
        goto fail_table;
    }
#endif

    policy->min = policy->cpuinfo.min_freq;
    policy->max = policy->cpuinfo.max_freq;
    policy->cur = hi_cpufreq_getspeed(policy->cpu);

    for (i = 0; freq_table[i].frequency != CPUFREQ_TABLE_END; i++)
    {
        max_freq = max(freq_table[i].frequency, max_freq);
    }

    /*
     * On hisilicon SMP configuartion, both processors share the voltage
     * and clock. So both CPUs needs to be scaled together and hence
     * needs software co-ordination. Use cpufreq affected_cpus
     * interface to handle this scenario. Additional is_smp() check
     * is to keep SMP_ON_UP build working.
     */

#ifdef CONFIG_SMP
    policy->shared_type = CPUFREQ_SHARED_TYPE_ANY;
    cpumask_setall(policy->cpus);
#endif

    /* FIXME: what's the actual transition time? */
    policy->cpuinfo.transition_latency = 300 * 1000;

#ifndef HI_ADVCA_FUNCTION_RELEASE
#ifndef MODULE
    result = input_register_handler(&cpufreq_interactive_input_handler);
    if (result)
    {
        goto fail_ck;
    }
#endif
#endif

    return 0;

fail_table:
    freq_table_free();
fail_ck:
    return result;
}

static int hi_cpufreq_cpu_exit(struct cpufreq_policy *policy)
{
    freq_table_free();
    return 0;
}

static int hi_cpufreq_resume(struct cpufreq_policy *policy)
{
    return 0;
}

static int hi_cpufreq_resume_noirq(struct device *dev)
{
    return 0;
}

static int hi_cpufreq_suspend_noirq(struct device *dev)
{
    return 0;
}

static struct freq_attr *hi_cpufreq_attr[] = {
    &cpufreq_freq_attr_scaling_available_freqs,
    NULL,
};

static struct cpufreq_driver hi_cpufreq_driver = {
    .verify = hi_cpufreq_verify,
    .target = hi_cpufreq_target,
    .get  = hi_cpufreq_getspeed,
    .init = hi_cpufreq_cpu_init,
    .exit = hi_cpufreq_cpu_exit,
    .resume = hi_cpufreq_resume,
    .name  = "hi-cpufreq",
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))    
    .owner = THIS_MODULE,
#endif
    .attr  = hi_cpufreq_attr,
};

static struct dev_pm_ops hi_cpufreq_driver_pm_ops = {
    .suspend_noirq = hi_cpufreq_suspend_noirq,
    .resume_noirq  = hi_cpufreq_resume_noirq,
};

static struct platform_driver hi_cpufreq_platform_driver = {
    .driver.name = "hi_cpufreq",
    .driver.pm   = &hi_cpufreq_driver_pm_ops,
};

static void cf_platform_device_release(struct device* dev){}

static struct platform_device hi_cpufreq_device = {
    .name = "mpu_pm_dev",
    .dev  = {
        .platform_data = NULL,
        .release       = cf_platform_device_release,
    },
};

int  hi_cpufreq_init(void)
{
    int ret;

    HI_INFO_PM("Enter %s\n", __FUNCTION__);
    hi_opp_init();
    ret = cpufreq_register_driver(&hi_cpufreq_driver);
    hi_cpufreq_ready = !ret;

    if (!ret)
    {
        int t;

        t = platform_device_register(&hi_cpufreq_device);
        if (t)
        {
            HI_ERR_PM("%s_init: platform_device_register failed\n",
                      __func__);
        }

        t = platform_driver_register(&hi_cpufreq_platform_driver);
        if (t)
        {
            HI_WARN_PM("%s_init: platform_driver_register failed\n",
                       __func__);
        }
    }

    return ret;
}

void  hi_cpufreq_exit(void)
{
    HI_INFO_PM("hi_cpufreq_exit\n");

    platform_driver_unregister(&hi_cpufreq_platform_driver);
    platform_device_unregister(&hi_cpufreq_device);
    cpufreq_unregister_driver(&hi_cpufreq_driver);
}

//MODULE_DESCRIPTION("cpufreq driver for hisilicon SOCs");
MODULE_LICENSE("GPL");
#if 0
#ifndef MODULE
late_initcall(hi_cpufreq_init);
module_exit(hi_cpufreq_exit);
#endif
#endif
