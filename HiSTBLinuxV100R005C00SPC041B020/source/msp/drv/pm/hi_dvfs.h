/*
 * hisilicon Management Routines
 *
 * Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
 */

#ifndef __ARCH_ARM_MACH_HI_DVFS_H
#define __ARCH_ARM_MACH_HI_DVFS_H

#include "hi_type.h"

#ifndef PMC_BASE
#define PMC_BASE    0xF8A23000
#endif

extern HI_U32 * g_pPMCStart;
#define PERI_PMC6   (g_pPMCStart + 6)
#define PERI_PMC7   (g_pPMCStart + 7)
#define PERI_PMC8   (g_pPMCStart + 8)
#define PERI_PMC9   (g_pPMCStart + 9)
#define PERI_PMC10  (g_pPMCStart + 10)
#define PERI_PMC12  (g_pPMCStart + 12)

#define PERI_PMC22  (g_pPMCStart + 22)
#define PERI_PMC23  (g_pPMCStart + 23)
#define PERI_PMC24  (g_pPMCStart + 24)
#define PERI_PMC25  (g_pPMCStart + 25)
#define PERI_PMC26  (g_pPMCStart + 26)
#define PERI_PMC29  (g_pPMCStart + 29)
#define PERI_PMC30  (g_pPMCStart + 30)
#define PERI_PMC31  (g_pPMCStart + 31)
#define PERI_PMC32  (g_pPMCStart + 32)
#define PERI_PMC33  (g_pPMCStart + 33)

#define PERI_PMC218  (g_pPMCStart + 218)

#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200)
 #define HI_AVS_HARDWARE
#endif

#define PWM_DUTY_MASK 0xffff0000
#define PWM_PERIOD_MASK 0xffff
#define PWM_CPU_ENABLE_BIT 0x0
#define PWM_CORE_ENABLE_BIT 0x2
#define HPM_PC_RECORED_MASK 0x3ff

#define LOW_TEMPERATURE 30

extern int hi_device_scale(struct device *target_dev,unsigned int old_freq,unsigned int new_freq);
extern void mpu_get_temperature(HI_S16 * ps16Temp);

#endif
