/*
 *
 * (C) COPYRIGHT 2014-2015 ARM Limited. All rights reserved.
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

#ifndef _KBASE_CONFIG_PLATFORM_H_
#define _KBASE_CONFIG_PLATFORM_H_

#include "mali_kbase_cpu_vexpress.h"

/**
 * Maximum frequency GPU will be clocked at. Given in kHz.
 * This must be specified as there is no default value.
 *
 * Attached value: number in kHz
 * Default value: NA
 */
#define GPU_FREQ_KHZ_MAX (600000)
/**
 * Minimum frequency GPU will be clocked at. Given in kHz.
 * This must be specified as there is no default value.
 *
 * Attached value: number in kHz
 * Default value: NA
 */
#define GPU_FREQ_KHZ_MIN (200000)
/**
 * CPU_SPEED_FUNC - A pointer to a function that calculates the CPU clock
 *
 * CPU clock speed of the platform is in MHz - see kbase_cpu_clk_speed_func
 * for the function prototype.
 *
 * Attached value: A kbase_cpu_clk_speed_func.
 * Default Value:  NA
 */
#define CPU_SPEED_FUNC (&kbase_get_vexpress_cpu_clock_speed)

/**
 * GPU_SPEED_FUNC - A pointer to a function that calculates the GPU clock
 *
 * GPU clock speed of the platform in MHz - see kbase_gpu_clk_speed_func
 * for the function prototype.
 *
 * Attached value: A kbase_gpu_clk_speed_func.
 * Default Value:  NA
 */
#define GPU_SPEED_FUNC (NULL)

/**
 * Power management configuration
 *
 * Attached value: pointer to @ref kbase_pm_callback_conf
 * Default value: See @ref kbase_pm_callback_conf
 */
#define POWER_MANAGEMENT_CALLBACKS (&pm_callbacks)

/**
 * Platform specific configuration functions
 *
 * Attached value: pointer to @ref kbase_platform_funcs_conf
 * Default value: See @ref kbase_platform_funcs_conf
 */
#define PLATFORM_FUNCS (&platform_callbacks)

/** Power model for IPA
 *
 * Attached value: pointer to @ref mali_pa_model_ops
 */
#define POWER_MODEL_CALLBACKS (NULL)

/**
 * Secure mode switch
 *
 * Attached value: pointer to @ref kbase_secure_ops
 */
#define SECURE_CALLBACKS (NULL)

extern struct kbase_pm_callback_conf pm_callbacks;
extern struct kbase_platform_funcs_conf platform_callbacks;

typedef enum 
{
	KBASE_PM_DVFS_NOP,         
	KBASE_PM_DVFS_CLOCK_UP,    
	KBASE_PM_DVFS_CLOCK_DOWN
}kbase_pm_dvfs_action;

int kbase_avs_init(void);
int kbase_avs_sethpm(int gpufreq);
int kbase_avs_start(unsigned int profile);


int kbase_get_voltage(void);
int kbase_set_voltage(unsigned int volt);

int kbase_get_max_frequency(void);
int kbase_set_max_frequency(unsigned int freq);
int kbase_get_min_frequency(void);
int kbase_set_min_frequency(unsigned int freq);

int kbase_get_frequency_slowly(void);
int kbase_set_frequency_slowly(unsigned int freq);

int kbase_rise_frequency_slowly(void);
int kbase_drop_frequency_slowly(void);

int kbase_get_frequency(void);
int kbase_set_frequency(unsigned int freq);

int kbase_is_powerup(void);

int kbase_dvfs_enable(unsigned int enable);
int kbase_is_dvfs_enable(void);

int kbase_avs_enable(unsigned int enable);
int kbase_is_avs_enable(void);

int kbase_get_ultilisation(void);

bool kbase_get_vsync(void);

int kbase_get_utilisation(void);
int kbase_get_profile(unsigned int freq);

#endif
