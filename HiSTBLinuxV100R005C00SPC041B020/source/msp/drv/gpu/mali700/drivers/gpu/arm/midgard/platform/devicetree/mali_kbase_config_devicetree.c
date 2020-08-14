/*
 *
 * (C) COPYRIGHT 2015 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained
 * from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 */

#include <linux/of.h>
#include <linux/of_platform.h>
#ifdef CONFIG_DEVFREQ_THERMAL
#include <linux/devfreq_cooling.h>
#endif
#include <linux/thermal.h>

#include <mali_kbase.h>
#include <mali_kbase_proc.h>
#ifdef MALI_AVS_SUPPORT
#include <mali_kbase_avs.h>
#endif
#include <mali_kbase_proc.h>
#include <mali_kbase_config_platform.h>


#define KBASE_HISI_GPU_SVB_BASE     0xf8000000
#define KBASE_HISI_GPU_SVB_SIZE		0x100
#define KBASE_HISI_GPU_SVB_OFFSET   0xc4

static u32 s_svb_type = 0;

static int kbase_platform_svb_identify(void)
{
    volatile u32* pu32SVBBase = (u32*)ioremap_nocache(KBASE_HISI_GPU_SVB_BASE,  KBASE_HISI_GPU_SVB_SIZE);

    s_svb_type = *(pu32SVBBase +  (KBASE_HISI_GPU_SVB_OFFSET >> 2));
    s_svb_type = (s_svb_type >> 24) & 0xff;    

    iounmap(pu32SVBBase);

    if((s_svb_type != KBASE_CHIP_TYPE_FF) && (s_svb_type != KBASE_CHIP_TYPE_TT) && (s_svb_type != KBASE_CHIP_TYPE_SS))
    {
        printk("Unidentified SVB type 0x%x!\n", s_svb_type);
        s_svb_type = KBASE_CHIP_TYPE_SS;
    }

    return 0;
}

int kbase_svb_identify(void)
{
    return s_svb_type;
}


int kbase_platform_early_init(void)
{
	kbase_platform_svb_identify();
	return 0;
}

static int platform_callback_init(struct kbase_device *kbdev)
{
#ifdef MALI_AVS_SUPPORT
	kbase_avs_init();
#endif
	kbase_proc_create();
	return 0;
}

static void platform_callback_term(struct kbase_device *kbdev)
{
#ifdef MALI_AVS_SUPPORT
    kbase_avs_deinit();
#endif
	kbase_proc_destroy();
}

struct kbase_platform_funcs_conf platform_callbacks = {
	.platform_init_func = platform_callback_init,
	.platform_term_func = platform_callback_term	
};
