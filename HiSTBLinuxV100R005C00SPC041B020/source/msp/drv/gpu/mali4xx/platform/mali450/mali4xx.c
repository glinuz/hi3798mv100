/*
 * Copyright (C) 2010, 2012-2013 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @file mali_platform.c
 * Platform specific Mali driver functions for:
 * - Realview Versatile platforms with ARM11 Mpcore and virtex 5.
 * - Versatile Express platforms with ARM Cortex-A9 and virtex 6.
 */
#include <linux/platform_device.h>
#include <linux/version.h>
#include <linux/pm.h>
#ifdef CONFIG_PM_RUNTIME
 #include <linux/pm_runtime.h>
#endif
#include <asm/io.h>
#include <linux/mali/mali_utgard.h>
#include "mali_kernel_common.h"
#include "mali_osk.h"

#include <linux/dma-mapping.h>

#include "mali4xx_cfg.h"
#include "mali4xx_pmu.h"

#include "mali4xx_core_scaling.h"

#if defined(CHIP_TYPE_hi3798cv100) || defined(CHIP_TYPE_hi3796cv100)
#define MALI_BASE_ADDRESS 0xff500000
#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a) \
   || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3796mv200)
#define MALI_BASE_ADDRESS 0xf9200000
#endif

#define INTERRUPT_SIGNAL 94 + 32

#define GP_IRQ	INTERRUPT_SIGNAL
#define PP0_IRQ	INTERRUPT_SIGNAL
#define PP1_IRQ	INTERRUPT_SIGNAL
#define PP2_IRQ	INTERRUPT_SIGNAL
#define PP3_IRQ	INTERRUPT_SIGNAL
#define PP4_IRQ	INTERRUPT_SIGNAL
#define PP5_IRQ	INTERRUPT_SIGNAL
#define PP6_IRQ	INTERRUPT_SIGNAL
#define PP7_IRQ	INTERRUPT_SIGNAL

#define GP_MMU_IRQ	INTERRUPT_SIGNAL
#define PP0_MMU_IRQ	INTERRUPT_SIGNAL
#define PP1_MMU_IRQ	INTERRUPT_SIGNAL
#define PP2_MMU_IRQ	INTERRUPT_SIGNAL
#define PP3_MMU_IRQ	INTERRUPT_SIGNAL
#define PP4_MMU_IRQ	INTERRUPT_SIGNAL
#define PP5_MMU_IRQ	INTERRUPT_SIGNAL
#define PP6_MMU_IRQ	INTERRUPT_SIGNAL
#define PP7_MMU_IRQ	INTERRUPT_SIGNAL

#define PP_BCAST	INTERRUPT_SIGNAL

#define HISI_GPU_RESOURCES_MALI450_MP2_PMU(base_addr, gp_irq, gp_mmu_irq, \
	pp0_irq, pp0_mmu_irq, pp1_irq, pp1_mmu_irq, pp_bcast_irq) \
	MALI_GPU_RESOURCE_L2(base_addr + 0x10000) \
	MALI_GPU_RESOURCE_L2(base_addr + 0x01000) \
	MALI_GPU_RESOURCE_BCAST(base_addr + 0x13000) \
	MALI_GPU_RESOURCE_DLBU(base_addr + 0x14000) \
	MALI_GPU_RESOURCE_DMA(base_addr + 0x12000) \
	MALI_GPU_RESOURCE_PP_BCAST(base_addr + 0x16000, pp_bcast_irq) \
	MALI_GPU_RESOURCE_PP_MMU_BCAST(base_addr + 0x15000)\
	MALI_GPU_RESOURCE_PMU(base_addr + 0x2000) \
	MALI_GPU_RESOURCE_GP_WITH_MMU(base_addr + 0x00000, gp_irq, base_addr + 0x03000, gp_mmu_irq) \
	MALI_GPU_RESOURCE_PP_WITH_MMU(0, base_addr + 0x08000, pp0_irq, base_addr + 0x04000, pp0_mmu_irq) \
	MALI_GPU_RESOURCE_PP_WITH_MMU(1, base_addr + 0x0A000, pp1_irq, base_addr + 0x05000, pp1_mmu_irq)  

#define HISI_GPU_RESOURCES_MALI450_MP6_PMU(base_addr, gp_irq, gp_mmu_irq, \
	pp0_irq, pp0_mmu_irq, pp1_irq, pp1_mmu_irq, pp2_irq, pp2_mmu_irq, \
	pp4_irq, pp4_mmu_irq, pp5_irq, pp5_mmu_irq, pp6_irq, pp6_mmu_irq, pp_bcast_irq) \
	MALI_GPU_RESOURCE_L2(base_addr + 0x10000) \
	MALI_GPU_RESOURCE_L2(base_addr + 0x01000) \
	MALI_GPU_RESOURCE_L2(base_addr + 0x11000) \
	MALI_GPU_RESOURCE_BCAST(base_addr + 0x13000) \
	MALI_GPU_RESOURCE_DLBU(base_addr + 0x14000) \
	MALI_GPU_RESOURCE_DMA(base_addr + 0x12000) \
	MALI_GPU_RESOURCE_PP_BCAST(base_addr + 0x16000, pp_bcast_irq) \
	MALI_GPU_RESOURCE_PP_MMU_BCAST(base_addr + 0x15000)\
	MALI_GPU_RESOURCE_PMU(base_addr + 0x2000) \
	MALI_GPU_RESOURCE_GP_WITH_MMU(base_addr + 0x00000, gp_irq, base_addr + 0x03000, gp_mmu_irq) \
	MALI_GPU_RESOURCE_PP_WITH_MMU(0, base_addr + 0x08000, pp0_irq, base_addr + 0x04000, pp0_mmu_irq) \
	MALI_GPU_RESOURCE_PP_WITH_MMU(1, base_addr + 0x0A000, pp1_irq, base_addr + 0x05000, pp1_mmu_irq) \
	MALI_GPU_RESOURCE_PP_WITH_MMU(2, base_addr + 0x0C000, pp2_irq, base_addr + 0x06000, pp2_mmu_irq) \
	MALI_GPU_RESOURCE_PP_WITH_MMU(4, base_addr + 0x28000, pp4_irq, base_addr + 0x1C000, pp4_mmu_irq) \
	MALI_GPU_RESOURCE_PP_WITH_MMU(5, base_addr + 0x2A000, pp5_irq, base_addr + 0x1D000, pp5_mmu_irq) \
	MALI_GPU_RESOURCE_PP_WITH_MMU(6, base_addr + 0x2C000, pp6_irq, base_addr + 0x1E000, pp6_mmu_irq)  


static void mali_platform_device_release(struct device *device);
static int	mali_os_suspend(struct device *device);
static int	mali_os_resume(struct device *device);
static int	mali_os_freeze(struct device *device);
static int	mali_os_thaw(struct device *device);
#ifdef CONFIG_PM_RUNTIME
static int	mali_runtime_suspend(struct device *device);
static int	mali_runtime_resume(struct device *device);
static int	mali_runtime_idle(struct device *device);
#endif

void	mali_gpu_utilization_callback(struct mali_gpu_utilization_data *data);

static struct dev_pm_ops mali_gpu_device_type_pm_ops =
{
	.suspend         = mali_os_suspend,
	.resume          = mali_os_resume,
	.freeze          = mali_os_freeze,
	.thaw            = mali_os_thaw,
	.restore         = mali_os_thaw,
#ifdef CONFIG_PM_RUNTIME
	.runtime_suspend = mali_runtime_suspend,
	.runtime_resume  = mali_runtime_resume,
	.runtime_idle    = mali_runtime_idle,
#endif
};

static struct device_type mali_gpu_device_device_type =
{
	.pm = &mali_gpu_device_type_pm_ops,
};

static struct platform_device mali_gpu_device =
{
	.name                   = MALI_GPU_NAME_UTGARD,
	.id                     = 0,
	.dev.release            = mali_platform_device_release,
	.dev.dma_mask           = &mali_gpu_device.dev.coherent_dma_mask,
	.dev.coherent_dma_mask  = DMA_BIT_MASK(32),
	.dev.type               = &mali_gpu_device_device_type, /* We should probably use the pm_domain instead of type on newer kernels */
#if defined(CONFIG_ARM64)
	.dev.archdata.dma_ops   = &noncoherent_swiotlb_dma_ops,
#endif
};

static struct mali_gpu_device_data mali_gpu_data =
{
	.shared_mem_size        = CONFIG_GPU_MAX_SHARE_MEM_SIZE,
	.fb_start               = 0x00000000,
	.fb_size                = 0x80000000, /* 2G */
	.dedicated_mem_start    = GPU_DEDICATE_MEM_START,
	.dedicated_mem_size     = GPU_DEDICATE_MEM_SIZE,
	.control_interval       = 100, /*100ms*/
	.utilization_callback   = mali_gpu_utilization_callback,
	.pmu_switch_delay       = 0x1ff, /* do not have to be this high on FPGA, but it is good for testing to have a delay */
	.max_job_runtime        = 5000, /* 5 seconds */
#if defined(CHIP_TYPE_hi3798cv100) || defined(CHIP_TYPE_hi3796cv100)
    .pmu_domain_config      = {0x1, 0x2, 0x4, 0x4, 0x0, 0x8, 0x8, 0x8, 0x0, 0x1, 0x2, 0x8},
#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a) \
   || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3796mv200)
    .pmu_domain_config      = {0x1, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x0},
#endif
	.get_clock_info 		= NULL,
	.get_freq 				= NULL,
	.set_freq 				= NULL,
	.secure_mode_init                   = mali_secure_mode_init_hisi,
	.secure_mode_deinit                 = mali_secure_mode_deinit_hisi,
	.gpu_reset_and_secure_mode_enable   = mali_gpu_reset_and_secure_mode_enable_hisi,
	.gpu_reset_and_secure_mode_disable  = mali_gpu_reset_and_secure_mode_disable_hisi,
};

int mali_platform_device_register(void)
{
	int err = -1;

#if defined(CHIP_TYPE_hi3798cv100) || defined(CHIP_TYPE_hi3796cv100)
	int num_pp_cores = 6;
#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a) \
   || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3796mv200)
	int num_pp_cores = 2;
#else
	int num_pp_cores = 0;
#endif

	struct resource mali_gpu_resources[] =
	{
#if defined(CHIP_TYPE_hi3798cv100) || defined(CHIP_TYPE_hi3796cv100)
		HISI_GPU_RESOURCES_MALI450_MP6_PMU(MALI_BASE_ADDRESS, GP_IRQ, GP_MMU_IRQ, \
			PP0_IRQ, PP0_MMU_IRQ, PP1_IRQ, PP1_MMU_IRQ, PP2_IRQ, PP2_MMU_IRQ, \
			PP4_IRQ, PP4_MMU_IRQ, PP5_IRQ, PP5_MMU_IRQ, PP6_IRQ, PP6_MMU_IRQ, PP_BCAST)
#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a) \
   || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3796mv200)
		HISI_GPU_RESOURCES_MALI450_MP2_PMU(MALI_BASE_ADDRESS, GP_IRQ, GP_MMU_IRQ, \
			PP0_IRQ, PP0_MMU_IRQ, PP1_IRQ, PP1_MMU_IRQ, PP_BCAST)
#endif
	} ;
    	MALI_DEBUG_PRINT(4, ("mali_platform_device_register() called\n"));

	mali_init_pp_scale_cores(num_pp_cores);
	mali_platform_init();

	err = platform_device_add_resources(&mali_gpu_device, mali_gpu_resources, sizeof(mali_gpu_resources) / sizeof(mali_gpu_resources[0])  );
	
	if (0 == err)
	{
		err = platform_device_add_data(&mali_gpu_device, &mali_gpu_data, sizeof(mali_gpu_data));
		if (0 == err)
		{
			/* Register the platform device */
			err = platform_device_register(&mali_gpu_device);
			if (0 == err)
				{
#ifdef CONFIG_PM_RUNTIME
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37))
				pm_runtime_set_autosuspend_delay(&(mali_gpu_device.dev), 1000);
				pm_runtime_use_autosuspend(&(mali_gpu_device.dev));
#endif
				pm_runtime_enable(&(mali_gpu_device.dev));
#endif
				MALI_DEBUG_ASSERT(0 < num_pp_cores);
				mali_core_scaling_init(num_pp_cores);
				return 0;
			}
		}
		else
		{
			platform_device_unregister(&mali_gpu_device);
		}
	}

	return err;
}

void mali_platform_device_unregister(void)
{
    MALI_DEBUG_PRINT(4, ("mali_platform_device_unregister() called\n"));

    mali_core_scaling_term();

    platform_device_unregister(&mali_gpu_device);

    mali_platform_deinit();
}

static void mali_platform_device_release(struct device *device)
{
    MALI_DEBUG_PRINT(4, ("mali_platform_device_release() called\n"));
}

void mali_gpu_utilization_callback(struct mali_gpu_utilization_data *data)
{
    mali_core_scaling_update(data);
}

static int mali_os_suspend(struct device *device)
{
    int ret = 0;

    MALI_DEBUG_PRINT(4, ("mali_os_suspend() called\n"));

    if ((NULL != device->driver)
        && (NULL != device->driver->pm)
        && (NULL != device->driver->pm->suspend))
    {
        /* Need to notify Mali driver about this event */
        ret = device->driver->pm->suspend(device);
    }

    if(0 == ret)
    {
        mali_platform_power_mode_change(MALI_POWER_MODE_DEEP_SLEEP, MALI_FALSE);
    }
    return ret;
}

static int mali_os_resume(struct device *device)
{
    int ret = 0;

    MALI_DEBUG_PRINT(4, ("mali_os_resume() called\n"));

    mali_platform_power_mode_change(MALI_POWER_MODE_ON, MALI_FALSE);

    if ((NULL != device->driver)
        && (NULL != device->driver->pm)
        && (NULL != device->driver->pm->resume))
    {
        /* Need to notify Mali driver about this event */
        ret = device->driver->pm->resume(device);
    }

    return ret;
}

static int mali_os_freeze(struct device *device)
{
    int ret = 0;

    MALI_DEBUG_PRINT(4, ("mali_os_freeze() called\n"));

    mali_platform_power_mode_change(MALI_POWER_MODE_ON, MALI_FALSE);

    if ((NULL != device->driver)
        && (NULL != device->driver->pm)
        && (NULL != device->driver->pm->freeze))
    {
        /* Need to notify Mali driver about this event */
        ret = device->driver->pm->freeze(device);
    }

    if(0 == ret)
    {
        mali_platform_power_mode_change(MALI_POWER_MODE_DEEP_SLEEP, MALI_FALSE);
    }
    return ret;
}

static int mali_os_thaw(struct device *device)
{
    int ret = 0;

    MALI_DEBUG_PRINT(4, ("mali_os_thaw() called\n"));

    mali_platform_power_mode_change(MALI_POWER_MODE_ON, MALI_FALSE);

    if ((NULL != device->driver)
        && (NULL != device->driver->pm)
        && (NULL != device->driver->pm->thaw))
    {
        /* Need to notify Mali driver about this event */
        ret = device->driver->pm->thaw(device);
    }

    return ret;
}

#ifdef CONFIG_PM_RUNTIME
static int mali_runtime_suspend(struct device *device)
{
    int ret = 0;

    MALI_DEBUG_PRINT(4, ("mali_runtime_suspend() called\n"));

    if ((NULL != device->driver)
        && (NULL != device->driver->pm)
        && (NULL != device->driver->pm->runtime_suspend))
    {
        /* Need to notify Mali driver about this event */
        ret = device->driver->pm->runtime_suspend(device);
    }

    if(0 == ret)
    {
        mali_platform_power_mode_change(MALI_POWER_MODE_LIGHT_SLEEP, MALI_TRUE);
    }
    return ret;
}

static int mali_runtime_resume(struct device *device)
{
    int ret = 0;

    MALI_DEBUG_PRINT(4, ("mali_runtime_resume() called\n"));

    mali_platform_power_mode_change(MALI_POWER_MODE_ON, MALI_TRUE);

    if ((NULL != device->driver)
        && (NULL != device->driver->pm)
        && (NULL != device->driver->pm->runtime_resume))
    {
        /* Need to notify Mali driver about this event */
        ret = device->driver->pm->runtime_resume(device);
    }

    return ret;
}

static int mali_runtime_idle(struct device *device)
{
    MALI_DEBUG_PRINT(4, ("mali_runtime_idle() called\n"));

    if ((NULL != device->driver)
        && (NULL != device->driver->pm)
        && (NULL != device->driver->pm->runtime_idle))
    {
        /* Need to notify Mali driver about this event */
        int ret = device->driver->pm->runtime_idle(device);
        if (0 != ret)
        {
            return ret;
        }
    }

    pm_runtime_suspend(device);

    return 0;
}

#endif
