/*
 * Copyright (C) 2010-2012 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @file mali_platform.c
 * Platform specific Mali driver functions for a default platform
 */

/*#include <linux/clk.h>*/
/*#include <mach/platform.h>*/
#include <linux/clkdev.h>
#include <asm/clkdev.h>
/*#include <mach/clock.h>*/
#include <linux/module.h>   /* kernel module definitions */
#include <linux/fs.h>       /* file system operations */
#include <linux/cdev.h>     /* character device definitions */
#include "mali_kernel_common.h"
#include "linux/mali/mali_utgard.h"

#include "mali4xx_cfg.h"
#include "mali4xx_reg.h"
#include "mali4xx_pmu.h"
#include "mali4xx_clk.h"

#ifdef GPU_PROC_SUPPORT
 #include "mali4xx_gpu_proc.h"
#endif

#include "mali4xx_core_scaling.h"

#define HISI_PMU_NO_VSYNC_MIN_UTILISATION 76      /* 256 * 30% = 76 */
#define HISI_PMU_NO_VSYNC_MAX_UTILISATION 128     /* 256 * 50% = 128 */

/* Module parameter to control dvfs utilization */
int mali_dvfs_min_utilization = HISI_PMU_NO_VSYNC_MIN_UTILISATION;
module_param(mali_dvfs_min_utilization, int, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH); /* rw-rw-r-- */
MODULE_PARM_DESC(mali_dvfs_min_utilization, "min utilization");

/* Module parameter to control dvfs utilization */
int mali_dvfs_max_utilization = HISI_PMU_NO_VSYNC_MAX_UTILISATION;
module_param(mali_dvfs_max_utilization, int, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH); /* rw-rw-r-- */
MODULE_PARM_DESC(mali_dvfs_max_utilization, "max utilization");

#ifdef CONFIG_GPU_DVFS_ENABLE
int mali_dvfs_enable = 1;
#else
int mali_dvfs_enable = 0;
#endif
module_param(mali_dvfs_enable, int, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH); /* rw-rw-r-- */
MODULE_PARM_DESC(mali_dvfs_enable, "enable dvfs");

#ifdef CONFIG_GPU_AVS_ENABLE
int mali_avs_enable = 1;
#else
int mali_avs_enable = 0;
#endif
module_param(mali_avs_enable, int, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH); /* rw-rw-r-- */
MODULE_PARM_DESC(mali_avs_enable, "enable avs");

#ifdef GPU_PP_SCALING_ENABLE
unsigned int mali_pp_scale_cores = 0;
module_param(mali_pp_scale_cores, int, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH); /* rw-rw-r-- */
MODULE_PARM_DESC(mali_pp_scale_cores, "pp scaling");
#endif

unsigned int mali_dvfs_utilization = 0;

#ifdef GPU_DVFS_ENABLE
static void mali_dvfs_work_handler(struct work_struct *w);

static struct workqueue_struct *mali_dvfs_wq = 0;
static DECLARE_WORK(mali_dvfs_work, mali_dvfs_work_handler);
#endif

static u32 bPowerOff = 1;

typedef enum hiGPU_CORE_E
{
    GPU_CORE_GP,
    GPU_CORE_PP0,
    GPU_CORE_PP1,
    GPU_CORE_PP2,
    GPU_CORE_PP3
} GPU_CORE_E;

struct mali_hw_core *hisi_crg = NULL;
struct mali_hw_core *hisi_pmc = NULL;

#ifdef GPU_DVFS_ENABLE
mali_bool init_mali_dvfs_status(void)
{
    /*default status
    add here with the right function to get initilization value.
     */

    if (!mali_dvfs_wq)
    {
        mali_dvfs_wq = create_singlethread_workqueue("mali_dvfs");
    }

    return MALI_TRUE;
}

void deinit_mali_dvfs_status(void)
{
    if (mali_dvfs_wq)
    {
        destroy_workqueue(mali_dvfs_wq);
        mali_dvfs_wq = NULL;
    }
}

#endif

struct mali_hw_core * hisi_pmu_get(void)
{
    return hisi_pmc;
}

struct mali_hw_core * hisi_crg_get(void)
{
    return hisi_crg;
}

static struct mali_hw_core * hisi_hw_core_create(u32 phys_addr, u32 size, char* description)
{
    struct mali_hw_core *hwcore = (struct mali_hw_core *)_mali_osk_malloc(sizeof(struct mali_hw_core));

    if (NULL == hwcore)
    {
        MALI_PRINT_ERROR(("Failed to malloc %s hardware!\n", description));
        return NULL;
    }

    hwcore->phys_addr = phys_addr;
    hwcore->size = size;
    hwcore->description = NULL;
    hwcore->mapped_registers = _mali_osk_mem_mapioregion(hwcore->phys_addr, hwcore->size, hwcore->description);
    if (NULL == hwcore->mapped_registers)
    {
        _mali_osk_free(hwcore);
        hwcore = NULL;
        MALI_PRINT_ERROR(("Failed to map memory region for %s phys_addr 0x%08X\n", description, phys_addr));
        return NULL;
    }

    return hwcore;
}

static void hisi_hw_core_destroy(struct mali_hw_core *hwcore)
{
    if (NULL == hwcore)
    {
        return;
    }

    _mali_osk_mem_unmapioregion(hwcore->phys_addr, hwcore->size, hwcore->mapped_registers);

    _mali_osk_free(hwcore);
}

_mali_osk_errcode_t mali_platform_init(void)
{
    if (NULL != hisi_crg)
    {
        MALI_SUCCESS;
    }

    hisi_crg = hisi_hw_core_create(HISI_CRG_BASE, CRG_REG_ADDR_SIZE, "hisi_crg");
    if (NULL == hisi_crg)
    {
        return _MALI_OSK_ERR_FAULT;
    }

    hisi_pmc = hisi_hw_core_create(HISI_PMC_BASE, PMC_REG_ADDR_SIZE, "hisi_pmc");
    if (NULL == hisi_pmc)
    {
        hisi_hw_core_destroy(hisi_crg);
        return _MALI_OSK_ERR_FAULT;
    }

    gpu_init_clocks();

#ifdef GPU_DVFS_ENABLE
    if (!init_mali_dvfs_status())
    {
        MALI_DEBUG_PRINT(1, ("mali_platform_init failed\n"));
    }
#endif


#ifdef GPU_PROC_SUPPORT
    gpu_proc_create();
#endif
    MALI_DEBUG_PRINT(2, ("Hisi platform init successful\n"));
    MALI_SUCCESS;
}

_mali_osk_errcode_t mali_platform_deinit(void)
{
    MALI_DEBUG_PRINT(2, ("Hisi platform deinit Enter\n"));

#ifdef GPU_PROC_SUPPORT
    gpu_proc_destroy();
#endif

#ifdef GPU_DVFS_ENABLE
    deinit_mali_dvfs_status();
#endif

    gpu_deinit_clocks();

    if (NULL == hisi_crg)
    {
        MALI_SUCCESS;
    }

    hisi_hw_core_destroy(hisi_crg);
    hisi_hw_core_destroy(hisi_pmc);

    hisi_crg = NULL;
    hisi_pmc = NULL;

    MALI_DEBUG_PRINT(2, ("Hisi platform deinit successful\n"));
    MALI_SUCCESS;
}

_mali_osk_errcode_t mali_platform_power_mode_change(mali_power_mode power_mode, mali_bool isruntime)
{
    switch (power_mode)
    {
    case MALI_POWER_MODE_ON:
        if (bPowerOff == 1)
        {
            hisi_crg_clockon();
            hisi_crg_reset();

            /* the pmc will be reset when system suspend, so we need to restore the default pmc setting */
            if (!isruntime)
            {
                hisi_pmc_setparameter();
                clk_gpu_setdefault();
            }

            bPowerOff = 0;
 #ifdef S40V200_VMIN_TEST
            {
				extern struct mali_clk clk_gpu;
				gpu_set_freq_reg(clk_gpu_get_rate(&clk_gpu));
            }
 #endif
        }

        break;
    case MALI_POWER_MODE_LIGHT_SLEEP:
    case MALI_POWER_MODE_DEEP_SLEEP:
        if (bPowerOff == 0)
        {
            /* if dvfs enable, we need to set the voltage and frequency to default, so that the performance is more powerful in most case */
            if (mali_dvfs_enable)
            {
                clk_gpu_setdefault();
            }
            if (!isruntime)
            {
            	hisi_crg_clockoff();
            }
            
            bPowerOff = 1;
 #ifdef S40V200_VMIN_TEST
            gpu_set_freq_reg(0);
            gpu_set_utilization_reg(0);
 #endif
        }

        break;
    }

    MALI_SUCCESS;
}

unsigned int gpu_is_powerdown(void)
{
    return bPowerOff;
}

#ifdef  GPU_DVFS_ENABLE

static inline unsigned clk_gpu_get_next_rate(unsigned current_rate, unsigned utilization)
{
    if (0 == (mali_dvfs_min_utilization + mali_dvfs_max_utilization))
    {
        return 0xffff;
    }

    return current_rate * utilization * 2 / (mali_dvfs_min_utilization + mali_dvfs_max_utilization);
}

mali_bool mali_dvfs_handler(unsigned int utilization)
{
    mali_dvfs_utilization = utilization;

    queue_work(mali_dvfs_wq, &mali_dvfs_work);

    /*add error handle here*/

    return MALI_TRUE;
}

extern struct mali_clk clk_gpu;

static void mali_dvfs_work_handler(struct work_struct *w)
{
    struct mali_clk *gpu_clk;
    u32 cur_rate;
    long next_rate;
    unsigned int utilization = mali_dvfs_utilization;

    int profile = 0;

    if (0 == mali_dvfs_enable)
    {
        return;
    }

    gpu_clk   = &clk_gpu;
    cur_rate  = clk_gpu_get_rate(gpu_clk);
    next_rate = clk_gpu_get_next_rate(cur_rate, utilization);
    next_rate = clk_gpu_round_rate(gpu_clk, next_rate);
    profile = clk_gpu_get_index(cur_rate);

    if (((utilization >= mali_dvfs_min_utilization) && (utilization <= mali_dvfs_max_utilization))
        || (next_rate == cur_rate))

    {
 #ifdef GPU_AVS_SUPPORT
        if (mali_avs_enable && (profile < GPU_NO_AVS_PROFILE))
        {
            gpu_avs_start(profile);
        }
 #endif


        return;
    }

    MALI_DEBUG_PRINT(2, ("Mali DVFS: cur_rate (%d), next_rate (%d) ...\n", cur_rate, next_rate));

    /* if increase frequency, adjust voltage first */
    if (next_rate > cur_rate)
    {
        mali_gpu_set_voltage(next_rate);
    }

    if (clk_gpu_set_rate(gpu_clk, next_rate) < 0)
    {
        /* roll back to previous voltage */
        if (next_rate > cur_rate)
        {
            mali_gpu_set_voltage(cur_rate);
        }

        return;
    }

    /* if decrease frequency, decrease voltage after adjust frequency */
    if (next_rate < cur_rate)
    {
        mali_gpu_set_voltage(next_rate);
    }
}

#endif

void mali_gpu_utilization_handler(u32 utilization)
{
#ifdef GPU_PP_SCALING_ENABLE
	if( mali_pp_scale_cores != gpu_get_pp_scale_cores() )
	{
		mali_pp_scale_cores = gpu_set_pp_scale_cores(mali_pp_scale_cores);
		if( mali_pp_scale_cores == gpu_get_pp_scale_cores() )
		{
			MALI_DEBUG_PRINT(2, ("pp scale success, cores = %d\n", mali_pp_scale_cores));
		}
		else
		{
			MALI_DEBUG_PRINT(2, ("pp scale failed\n"));
		}
	}
#endif

#ifdef S40V200_VMIN_TEST
    gpu_set_utilization_reg(utilization);
#endif

#ifdef GPU_DVFS_ENABLE
    if (0 == bPowerOff)
    {
        if (!mali_dvfs_handler(utilization))
        {
            MALI_DEBUG_PRINT(1, ("error on mali dvfs status in utilization\n"));
        }
    }
#endif
}

void gpu_avs_enable(unsigned int enable)
{
#ifdef GPU_AVS_SUPPORT
    mali_avs_enable = enable ? 1 : 0;
#endif
}

void mali_init_pp_scale_cores(unsigned int num)
{
#ifdef GPU_PP_SCALING_ENABLE
	mali_pp_scale_cores = num ;
#endif
}

EXPORT_SYMBOL_GPL(gpu_avs_enable);
