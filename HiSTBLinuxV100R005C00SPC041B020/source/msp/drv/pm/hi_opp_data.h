/* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. */

#ifndef __ARCH_ARM_MACH_HI_OPP_DATA_H
#define __ARCH_ARM_MACH_HI_OPP_DATA_H

struct hi_opp_def
{
    unsigned long freq;
    unsigned long u_volt;

    bool default_available;
};

/*
 * Initialization wrapper used to define an OPP for OMAP variants.
 */
#define OPP_INITIALIZER(_enabled, _freq, _uv)   \
    {                               \
        .default_available = _enabled,         \
        .freq = _freq,                \
        .u_volt = _uv,                  \
    }

/* Use this to initialize the default table */
extern int __init hi_init_opp_table(struct hi_opp_def *opp_def,
                                    u32         opp_def_size);

#endif      /* __ARCH_ARM_MACH_HI_OPP_DATA_H */
