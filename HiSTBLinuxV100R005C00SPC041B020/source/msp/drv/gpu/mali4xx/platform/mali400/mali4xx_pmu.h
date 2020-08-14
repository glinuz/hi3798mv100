/*
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009-2012 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

/**
 * @file s40v200_pmu.h
 * Platform specific Mali driver functions
 */

#ifndef __S40V200_PMU_H__
#define __S40V200_PMU_H__

#include "mali_hw_core.h"
#include "mali_osk.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief description of power change reasons
 */
typedef enum mali_power_mode_tag
{
    MALI_POWER_MODE_ON,
    MALI_POWER_MODE_LIGHT_SLEEP,
    MALI_POWER_MODE_DEEP_SLEEP,
} mali_power_mode;

/** @brief Platform specific setup and initialisation of MALI
 *
 * This is called from the entrypoint of the driver to initialize the platform
 *
 * @return _MALI_OSK_ERR_OK on success otherwise, a suitable _mali_osk_errcode_t error.
 */
_mali_osk_errcode_t   mali_platform_init(void);

/** @brief Platform specific deinitialisation of MALI
 *
 * This is called on the exit of the driver to terminate the platform
 *
 * @return _MALI_OSK_ERR_OK on success otherwise, a suitable _mali_osk_errcode_t error.
 */
_mali_osk_errcode_t   mali_platform_deinit(void);

/** @brief Platform specific powerdown sequence of MALI
 *
 * Call as part of platform init if there is no PMM support, else the
 * PMM will call it.
 * There are three power modes defined:
 *  1) MALI_POWER_MODE_ON
 *  2) MALI_POWER_MODE_LIGHT_SLEEP
 *  3) MALI_POWER_MODE_DEEP_SLEEP
 * MALI power management module transitions to MALI_POWER_MODE_LIGHT_SLEEP mode when MALI is idle
 * for idle timer (software timer defined in mali_pmm_policy_jobcontrol.h) duration, MALI transitions
 * to MALI_POWER_MODE_LIGHT_SLEEP mode during timeout if there are no more jobs queued.
 * MALI power management module transitions to MALI_POWER_MODE_DEEP_SLEEP mode when OS does system power
 * off.
 * Customer has to add power down code when MALI transitions to MALI_POWER_MODE_LIGHT_SLEEP or MALI_POWER_MODE_DEEP_SLEEP
 * mode.
 * MALI_POWER_MODE_ON mode is entered when the MALI is to powered up. Some customers want to control voltage regulators during
 * the whole system powers on/off. Customer can track in this function whether the MALI is powered up from
 * MALI_POWER_MODE_LIGHT_SLEEP or MALI_POWER_MODE_DEEP_SLEEP mode and manage the voltage regulators as well.
 * @param power_mode defines the power modes
 * @return _MALI_OSK_ERR_OK on success otherwise, a suitable _mali_osk_errcode_t error.
 */
_mali_osk_errcode_t mali_platform_power_mode_change(mali_power_mode power_mode, mali_bool isruntime);

/** @brief Platform specific handling of GPU utilization data
 *
 * When GPU utilization data is enabled, this function will be
 * periodically called.
 *
 * @param utilization The workload utilization of the Mali GPU. 0 = no utilization, 256 = full utilization.
 */
void mali_gpu_utilization_handler(unsigned int utilization);

struct mali_hw_core * hisi_pmu_get(void);
struct mali_hw_core * hisi_crg_get(void);

unsigned int gpu_is_powerdown(void);

void mali_init_pp_scale_cores(unsigned int num);

#ifdef __cplusplus
}
#endif
#endif
