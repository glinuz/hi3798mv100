/*
 * hisilicon mpu clock management Routines
 *
 * Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/clk.h>

#include <linux/clkdev.h>
#include <asm/clkdev.h>

#include <linux/device.h>
#include "clock.h"
#include "hi_reg_common.h"
#include "hi_drv_reg.h"
#include "hi_drv_pmoc.h"
#include "hi_dvfs.h"
#include "hi_drv_sys.h"

struct device *mpu_dev;

static DEFINE_SPINLOCK(mpu_clock_lock);

#if defined(HI_DONGLE_MODE_CRITICAL)
#define MAX_FREQ 800000
#elif defined(HI_DONGLE_MODE_LOW)
#define MAX_FREQ 1000000
#elif  defined(CHIP_TYPE_hi3798cv100) \
    || defined(CHIP_TYPE_hi3796cv100)
#define MAX_FREQ 1500000
#elif  defined(CHIP_TYPE_hi3716mv410)
#define MAX_FREQ 1000000
#elif  defined(CHIP_TYPE_hi3798cv200_a) \
    || defined(CHIP_TYPE_hi3798cv200_b) \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a) \
    || defined(CHIP_TYPE_hi3798mv200)
#define MAX_FREQ 1600000
#else
#define MAX_FREQ 1200000
#endif // end of defined(HI_DONGLE_MODE_CRITICAL)

#define MIN_FREQ 400000

#define CLOCK_24M (24 * 1000) //KHz

#define CHIP_TYPE_FF 1
#define CHIP_TYPE_TT 2
#define CHIP_TYPE_SS 3

CPU_VF_S cpu_freq_hpm_table[MAX_FREQ_NUM] =
{
#if   (!defined(CHIP_TYPE_hi3798mv100)) \
    && (!defined(CHIP_TYPE_hi3796mv100)) \
    && (!defined(CHIP_TYPE_hi3716dv100))
    { 400000, HI_VDD_MPU_OPP1_HPM, HI_VDD_MPU_OPP1_VMIN, HI_VDD_MPU_OPP1_UV},
#endif
    { 600000, HI_VDD_MPU_OPP2_HPM, HI_VDD_MPU_OPP2_VMIN, HI_VDD_MPU_OPP2_UV},
    { 800000, HI_VDD_MPU_OPP3_HPM, HI_VDD_MPU_OPP3_VMIN, HI_VDD_MPU_OPP3_UV},
#if   (!defined(CHIP_TYPE_hi3798cv200))   \
    && (!defined(CHIP_TYPE_hi3798mv200_a)) \
    && (!defined(CHIP_TYPE_hi3798mv200))
    {1000000, HI_VDD_MPU_OPP4_HPM, HI_VDD_MPU_OPP4_VMIN, HI_VDD_MPU_OPP4_UV},
#endif
#if   !defined(CHIP_TYPE_hi3716mv410)
    {1200000, HI_VDD_MPU_OPP5_HPM, HI_VDD_MPU_OPP5_VMIN, HI_VDD_MPU_OPP5_UV},
#endif
#if    defined(CHIP_TYPE_hi3798cv100) \
    || defined(CHIP_TYPE_hi3796cv100) \
    || defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) \
    || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3798cv200_a) \
    || defined(CHIP_TYPE_hi3798cv200_b) \
    || defined(CHIP_TYPE_hi3798cv200)
    {1500000, HI_VDD_MPU_OPP6_HPM, HI_VDD_MPU_OPP6_VMIN, HI_VDD_MPU_OPP6_UV},
#endif
#if    defined(CHIP_TYPE_hi3798cv200_a) \
    || defined(CHIP_TYPE_hi3798cv200_b) \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a) \
    || defined(CHIP_TYPE_hi3798mv200)
    {1600000, HI_VDD_MPU_OPP7_HPM, HI_VDD_MPU_OPP7_VMIN, HI_VDD_MPU_OPP7_UV},
#endif
};

#if defined(CHIP_TYPE_hi3798cv200)
CPU_HPM_S cpu_hpm_table[MAX_FREQ_NUM] =
{
    { 400000, {HI_VDD_MPU_OPP1_HPM, HI_VDD_MPU_OPP1_HPM_FF, HI_VDD_MPU_OPP1_HPM_TT, HI_VDD_MPU_OPP1_HPM_SS}},
    { 600000, {HI_VDD_MPU_OPP2_HPM, HI_VDD_MPU_OPP2_HPM_FF, HI_VDD_MPU_OPP2_HPM_TT, HI_VDD_MPU_OPP2_HPM_SS}},
    { 800000, {HI_VDD_MPU_OPP3_HPM, HI_VDD_MPU_OPP3_HPM_FF, HI_VDD_MPU_OPP3_HPM_TT, HI_VDD_MPU_OPP3_HPM_SS}},
    {1000000, {HI_VDD_MPU_OPP4_HPM, HI_VDD_MPU_OPP4_HPM_FF, HI_VDD_MPU_OPP4_HPM_TT, HI_VDD_MPU_OPP4_HPM_SS}},
    {1200000, {HI_VDD_MPU_OPP5_HPM, HI_VDD_MPU_OPP5_HPM_FF, HI_VDD_MPU_OPP5_HPM_TT, HI_VDD_MPU_OPP5_HPM_SS}},
    {1500000, {HI_VDD_MPU_OPP6_HPM, HI_VDD_MPU_OPP6_HPM_FF, HI_VDD_MPU_OPP6_HPM_TT, HI_VDD_MPU_OPP6_HPM_SS}},
    {1600000, {HI_VDD_MPU_OPP7_HPM, HI_VDD_MPU_OPP7_HPM_FF, HI_VDD_MPU_OPP7_HPM_TT, HI_VDD_MPU_OPP7_HPM_SS}},
};
#endif

#ifdef HI_AVS_SUPPORT
unsigned int cur_cpu_hpm;
unsigned int cur_cpu_vmin;
unsigned int cur_cpu_vmax;
HI_U8 g_u8CpuHpmOffset = 0;
#endif

static int cpu_init_hpm(unsigned int rate, unsigned int offset)
{
    unsigned int regval, div;

#ifdef HI_AVS_SUPPORT
    unsigned int i;
    unsigned int corner_type;

 #if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    corner_type = (g_pstRegSysCtrl->SC_GENm[17] >> 24) & 0xff;
 #else
    corner_type = (g_pstRegSysCtrl->SC_GEN17 >> 24) & 0xff;
 #endif
    if (corner_type > CHIP_TYPE_SS)
    {
        corner_type = 0;
    }
#endif

    div = (rate / 50000) - 1;

    if (div > 31)
    {
        HI_ERR_PM("\n Rate value is out of range \n");
        return -1;
    }

    /* hpm 2             */
    /* set time division */
    HI_REG_READ32(PERI_PMC30, regval);
    regval &= 0xffffffc0;
    regval |= div;
    HI_REG_WRITE32(PERI_PMC30, regval);

    /* set monitor period to 2ms */
    HI_REG_READ32(PERI_PMC33, regval);
    regval &= 0x00ffffff;
    regval |= (1 << 24);
    HI_REG_WRITE32(PERI_PMC33, regval);

    /* hpm enable */
    HI_REG_READ32(PERI_PMC30, regval);
    regval |= (1 << 24);
    HI_REG_WRITE32(PERI_PMC30, regval);

    /* hpm monitor enable */
    HI_REG_READ32(PERI_PMC30, regval);
    regval |= (1 << 26);
    HI_REG_WRITE32(PERI_PMC30, regval);

#ifdef HI_AVS_HARDWARE
    /* init hardware hpm */
    HI_REG_READ32(PERI_PMC218, regval);
    regval &= 0x1;
    regval |= 0x02028000;
    HI_REG_WRITE32(PERI_PMC218, regval);
#endif

#ifdef HI_AVS_SUPPORT
    /* get default hpm record */
    for (i = 0; i < MAX_FREQ_NUM; i++)
    {
        if (rate == cpu_freq_hpm_table[i].freq)
        {
  #ifndef HI_AVS_HARDWARE
            cur_cpu_hpm	 = cpu_freq_hpm_table[i].hpmrecord + g_u8CpuHpmOffset + offset;
            cur_cpu_vmin = cpu_freq_hpm_table[i].vmin;
            cur_cpu_vmax = cpu_freq_hpm_table[i].vsetting;
  #else
   #if defined(CHIP_TYPE_hi3798cv200)
            cur_cpu_hpm = cpu_hpm_table[i].hpmrecord[corner_type] + g_u8CpuHpmOffset + offset;
   #else
            cur_cpu_hpm = cpu_freq_hpm_table[i].hpmrecord + g_u8CpuHpmOffset + offset;
   #endif


            HI_REG_READ32(PERI_PMC33, regval);
            regval &= 0xffc00c00;
            regval |= cur_cpu_hpm + 0x6;
            regval |= (cur_cpu_hpm << 12);
            HI_REG_WRITE32(PERI_PMC33, regval);
 #endif
            break;
        }
    }
#endif
    return 0;
}

#ifdef HI_AVS_SUPPORT
void core_init_hpm(void)
{
    unsigned int regval;
    unsigned int * hpm_cfg;
    unsigned int * hpm_time_cfg;

    /* hpm 0 */
    hpm_cfg = PERI_PMC22;
    hpm_time_cfg = PERI_PMC25;

    /* clock is 200M, set time division to (200/50 - 1) */
    HI_REG_READ32(hpm_cfg, regval);
    regval &= 0xffffffc0;
    regval |= 3;
    HI_REG_WRITE32(hpm_cfg, regval);

    /* set monitor period to 2ms */
    HI_REG_READ32(hpm_time_cfg, regval);
    regval &= 0x00ffffff;
    regval |= (1 << 24);
    HI_REG_WRITE32(hpm_time_cfg, regval);

    /* hpm enable */
    HI_REG_READ32(hpm_cfg, regval);
    regval |= (1 << 24);
    HI_REG_WRITE32(hpm_cfg, regval);

    /* hpm monitor enable */
    HI_REG_READ32(hpm_cfg, regval);
    regval |= (1 << 26);
    HI_REG_WRITE32(hpm_cfg, regval);
}

#endif //end of HI_AVS_SUPPORT

struct clk mpu_ck = {
    .name   = "mpu_ck",
    .parent = NULL,
};

#if defined(CHIP_TYPE_hi3798cv200_a) \
    || defined(CHIP_TYPE_hi3798cv200_b)  \
    || defined(CHIP_TYPE_hi3798cv200)    \
    || defined(CHIP_TYPE_hi3798mv200)    \
    || defined(CHIP_TYPE_hi3798mv200_a)
static unsigned int g_postdiv1;
static unsigned int g_postdiv2;
static unsigned int g_refdiv;
static int mpu_clk_set_rate(struct clk *clk, unsigned int rate)
{
    unsigned int fbdiv;
    unsigned long flag;
    unsigned int offset = 0;
    unsigned int i;
    
#if defined(CHIP_TYPE_hi3798cv200)
    HI_S16 s16Temperature;
#endif

    U_PERI_CRG_PLL0 unTmpValuePLL0;
    U_PERI_CRG_PLL1 unTmpValuePLL1;

#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)
    /* use cpll */
    U_PERI_CRG125 unTmpValueCRG125;
    U_PERI_CRG126 unTmpValueCRG126;
    U_PERI_CRG127 unTmpValueCRG127;
    U_PERI_CRG128 unTmpValueCRG128;
    U_PERI_CRG129 unTmpValueCRG129;
#else
    /* use apll */
    U_PERI_CRG105 unTmpValueCRG105;
    U_PERI_CRG106 unTmpValueCRG106;
    U_PERI_CRG107 unTmpValueCRG107;
    U_PERI_CRG108 unTmpValueCRG108;
    U_PERI_CRG109 unTmpValueCRG109;
#endif

    for (i = 0; i < MAX_FREQ_NUM; i++)
    {
        if (rate == cpu_freq_hpm_table[i].freq)
        {
            break;
        }
    }

    if (i == MAX_FREQ_NUM)
    {
        HI_ERR_PM("mpu_clk_set_rate %dk not support\n", rate);
        return -1;
    }

    unTmpValuePLL0.u32 = g_pstRegCrg->PERI_CRG_PLL0.u32;
    g_postdiv1 = (unTmpValuePLL0.bits.cpu_pll_cfg0_apb >> 24) & 0x7; //bit[26:24]
    g_postdiv2 = (unTmpValuePLL0.bits.cpu_pll_cfg0_apb >> 28) & 0x7; //bit[30:28]

    unTmpValuePLL1.u32 = g_pstRegCrg->PERI_CRG_PLL1.u32;
    g_refdiv = (unTmpValuePLL1.bits.cpu_pll_cfg1_apb >> 12) & 0x3f; //bit[17:12]

    /* do not set frac */
    fbdiv = (rate * g_postdiv1 * g_postdiv2 * g_refdiv) / CLOCK_24M;

#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)
    /* set fbdiv of target cpll */
    unTmpValueCRG125.u32 = g_pstRegCrg->PERI_CRG125.u32;
    unTmpValueCRG125.bits.cpll_tune_int_cfg = fbdiv;
    g_pstRegCrg->PERI_CRG125.u32 = unTmpValueCRG125.u32;

    /* set frac of target cpll to 0 */
    unTmpValueCRG126.u32 = g_pstRegCrg->PERI_CRG126.u32;
    unTmpValueCRG126.bits.cpll_tune_frac_cfg = 0;
    g_pstRegCrg->PERI_CRG126.u32 = unTmpValueCRG126.u32;

    /* set integer of tune step */
    unTmpValueCRG127.u32 = g_pstRegCrg->PERI_CRG127.u32;
    unTmpValueCRG127.bits.cpll_tune_step_int = 1;
    g_pstRegCrg->PERI_CRG127.u32 = unTmpValueCRG127.u32;

    /* set fraction of tune setp */
    unTmpValueCRG128.u32 = g_pstRegCrg->PERI_CRG128.u32;
    unTmpValueCRG128.bits.cpll_tune_step_frac = 0;
    g_pstRegCrg->PERI_CRG128.u32 = unTmpValueCRG128.u32;

    /* enter ssmod */
    unTmpValueCRG129.u32 = g_pstRegCrg->PERI_CRG129.u32;
    unTmpValueCRG129.bits.cpll_tune_mode = 0x1;
    g_pstRegCrg->PERI_CRG129.u32 = unTmpValueCRG129.u32;

    /* begin one tune process */
    unTmpValueCRG129.u32 = g_pstRegCrg->PERI_CRG129.u32;
    unTmpValueCRG129.bits.cpll_tune_en = 0x0;
    g_pstRegCrg->PERI_CRG129.u32 = unTmpValueCRG129.u32;

    unTmpValueCRG129.u32 = g_pstRegCrg->PERI_CRG129.u32;
    unTmpValueCRG129.bits.cpll_tune_en = 0x1;
    g_pstRegCrg->PERI_CRG129.u32 = unTmpValueCRG129.u32;

    /* check whether pll switch is over */
    while (1)
    {
        if (0x0 == g_pstRegCrg->PERI_CRG173.bits.cpll_tune_busy)
        {
            break;
        }

        udelay(10);
    }

#else
    /* set fbdiv of target apll */
    unTmpValueCRG105.u32 = g_pstRegCrg->PERI_CRG105.u32;
    unTmpValueCRG105.bits.apll_tune_int_cfg = fbdiv;
    g_pstRegCrg->PERI_CRG105.u32 = unTmpValueCRG105.u32;

    /* set frac of target apll to 0 */
    unTmpValueCRG106.u32 = g_pstRegCrg->PERI_CRG106.u32;
    unTmpValueCRG106.bits.apll_tune_frac_cfg = 0;
    g_pstRegCrg->PERI_CRG106.u32 = unTmpValueCRG106.u32;

    /* set integer of tune step */
    unTmpValueCRG107.u32 = g_pstRegCrg->PERI_CRG107.u32;
    unTmpValueCRG107.bits.apll_tune_step_int = 1;
    g_pstRegCrg->PERI_CRG107.u32 = unTmpValueCRG107.u32;

    /* set fraction of tune setp */
    unTmpValueCRG108.u32 = g_pstRegCrg->PERI_CRG108.u32;
    unTmpValueCRG108.bits.apll_tune_step_frac = 0;
    g_pstRegCrg->PERI_CRG108.u32 = unTmpValueCRG108.u32;

    /* enter ssmod */
    unTmpValueCRG109.u32 = g_pstRegCrg->PERI_CRG109.u32;
    unTmpValueCRG109.bits.apll_tune_mode = 0x1;
    g_pstRegCrg->PERI_CRG109.u32 = unTmpValueCRG109.u32;

    /* begin one tune process */
    unTmpValueCRG109.u32 = g_pstRegCrg->PERI_CRG109.u32;
    unTmpValueCRG109.bits.apll_tune_en = 0x0;
    g_pstRegCrg->PERI_CRG109.u32 = unTmpValueCRG109.u32;

    unTmpValueCRG109.u32 = g_pstRegCrg->PERI_CRG109.u32;
    unTmpValueCRG109.bits.apll_tune_en = 0x1;
    g_pstRegCrg->PERI_CRG109.u32 = unTmpValueCRG109.u32;

    /* check whether pll switch is over */
    while (1)
    {
        if (0x0 == g_pstRegCrg->PERI_CRG165.bits.apll_tune_busy)
        {
            break;
        }

        udelay(10);
    }
#endif

    /* in low temp. add volt 50 mv */
#if defined(CHIP_TYPE_hi3798cv200)
    mpu_get_temperature(&s16Temperature);
    if (s16Temperature <= LOW_TEMPERATURE)
    {
        if (rate == 800000)
        {
            offset = 0x19;
        }
    }
#endif

    /* After change clock, we need to reinitialize hpm */
    cpu_init_hpm(rate, offset);

    spin_lock_irqsave(&mpu_ck.spinlock, flag);
    clk->rate = rate;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    g_pstRegSysCtrl->SC_GENx[2] = rate;
#else
    g_pstRegSysCtrl->SC_GEN26 = rate;
#endif
    spin_unlock_irqrestore(&mpu_ck.spinlock, flag);

    return 0;
}

static unsigned int find_clk_in_table(unsigned int rate)
{
    unsigned int i, clk_in_table;

    for (i = 0; i < MAX_FREQ_NUM; i++)
    {
        if (rate >= cpu_freq_hpm_table[i].freq)
        {
            if ((rate - cpu_freq_hpm_table[i].freq) < 10000) // 10M
            {
                break;
            }
        }
        else
        {
            if ((cpu_freq_hpm_table[i].freq - rate) < 10000)
            {
                break;
            }
        }
    }

    if (i >= MAX_FREQ_NUM)
    {
        return DEFAULT_INIT_FREQ;
    }

    clk_in_table = cpu_freq_hpm_table[i].freq;

    return clk_in_table;
}

static unsigned int mpu_clk_get_rate(void)
{
    unsigned int rate, fbdiv, reg_clk;

    reg_clk = g_pstRegCrg->PERI_CRG18.bits.cpu_freq_sel_cfg_crg;

    switch (reg_clk)
    {
    case 0:
    {
        g_postdiv1 = (g_pstRegCrg->PERI_CRG_PLL0.bits.cpu_pll_cfg0_apb >> 24) & 0x7;     //bit[26:24]
        g_postdiv2 = (g_pstRegCrg->PERI_CRG_PLL0.bits.cpu_pll_cfg0_apb >> 28) & 0x7;     //bit[30:28]
        g_refdiv = (g_pstRegCrg->PERI_CRG_PLL1.bits.cpu_pll_cfg1_apb >> 12) & 0x3f;     //bit[17:12]

        if (g_pstRegCrg->PERI_CRG165.bits.apll_tune_int)
        {
            fbdiv = g_pstRegCrg->PERI_CRG165.bits.apll_tune_int;
        }
        else
        {
            fbdiv = g_pstRegCrg->PERI_CRG_PLL1.bits.cpu_pll_cfg1_apb & 0xfff;
        }

        rate = (fbdiv * CLOCK_24M) / (g_postdiv1 * g_postdiv2 * g_refdiv);
        rate = find_clk_in_table(rate);
        break;
    }
    case 1:
        rate = 200000;
        break;
    case 2:
        rate = 800000;
        break;
    case 3:
        rate = 1350000;
        break;
    case 4:
        rate = 24000;
        break;
    case 5:
        rate = 1200000;
        break;
    case 6:
        rate = 400000;
        break;
    case 7:
        rate = 600000;
        break;
    default:
        HI_ERR_PM("mpu_clk_get_rate %dk not support\n", reg_clk);
        return 0;
    }

    return rate;
}

#else
static int mpu_clk_set_sel(unsigned int sel)
{
    U_PERI_CRG18 unTmpValue;

    HI_INFO_PM("enter %s\n", __FUNCTION__);

    unTmpValue.u32 = g_pstRegCrg->PERI_CRG18.u32;
    unTmpValue.bits.cpu_sw_begin_cfg = 0;
    g_pstRegCrg->PERI_CRG18.u32 = unTmpValue.u32;

    unTmpValue.bits.cpu_freq_sel_cfg_crg = sel;
    g_pstRegCrg->PERI_CRG18.u32 = unTmpValue.u32;

    unTmpValue.bits.cpu_sw_begin_cfg = 1;
    g_pstRegCrg->PERI_CRG18.u32 = unTmpValue.u32;

    return 0;
}

/*
 * mpu_clksel_set_rate() - program clock rate in hardware
 * @clk: struct clk * to program rate
 * @rate: target rate to program
 */
static int mpu_clk_set_rate(struct clk *clk, unsigned rate)
{
    unsigned int sel;
    unsigned int offset = 0;
    unsigned long flag;

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    unsigned int corner_type;
    unsigned int rate_change_flag = 0;
    HI_CHIP_PACKAGE_TYPE_E enPackageType = HI_CHIP_PACKAGE_TYPE_BUTT;
#endif

    HI_INFO_PM("mpu_clk_set_rate %dk\n", rate);

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    HI_DRV_SYS_GetChipPackageType(&enPackageType);

    corner_type = (g_pstRegSysCtrl->SC_GEN17 >> 24) & 0xff;

    if (1200000 == rate)
    {
        if ((CHIP_TYPE_FF == corner_type) || (CHIP_TYPE_SS == corner_type)
            || (HI_CHIP_PACKAGE_TYPE_QFP_216 == enPackageType))
        {
            rate = 1000000;
            rate_change_flag = 1;
        }
    }

    if (HI_CHIP_PACKAGE_TYPE_QFP_216 == enPackageType)
    {
        if (600000 == rate)
        {
            offset = 0x10;
        }
        else
        {
            offset = 0x25;
        }
    }
#endif
    switch (rate)
    {
#if    defined(CHIP_TYPE_hi3716mv410) \
        || defined(CHIP_TYPE_hi3716mv420)
    case 1000000:
        sel = 0; // APLL
        break;
    case 800000:
        sel = 1;
        break;
    case 600000:
        sel = 2;
        break;
    case 1200000:
        sel = 3;
        break;
    case 24000:
        sel = 4;
        break;
    case 400000:
        sel = 5;
        break;
    case 300000:
        sel = 6;
        break;
    case 200000:
        sel = 7;
        break;
#else
    case 1000000:
        sel = 0; // APLL
        break;
    case 800000:
        sel = 1;
        break;
    case 1500000:
        sel = 3;
        break;
    case 24000:
        sel = 4;
        break;
    case 1200000:
        sel = 5;
        break;
    case 400000:
        sel = 6;
        break;
    case 600000:
        sel = 7;
        break;
#endif
    default:
        HI_ERR_PM("mpu_clk_set_rate %dk not support\n", rate);
        return -1;
    }

    HI_INFO_PM("mpu_clk_set_rate sel=%u\n", sel);

    /* After change clock, we need to reinitialize hpm */
    cpu_init_hpm(rate, offset);

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    if (1 == rate_change_flag)
    {
        rate = 1200000;
    }
#endif
    mpu_clk_set_sel(sel);

    spin_lock_irqsave(&mpu_ck.spinlock, flag);
    clk->rate = rate;
    g_pstRegSysCtrl->SC_GEN26 = rate;
    spin_unlock_irqrestore(&mpu_ck.spinlock, flag);

    return 0;
}

static unsigned int mpu_clk_get_rate(void)
{
    unsigned int reg_clk, val_clk;

    reg_clk = g_pstRegCrg->PERI_CRG18.bits.cpu_freq_sel_cfg_crg;

    switch (reg_clk)
    {
#if    defined(CHIP_TYPE_hi3716mv410) \
        || defined(CHIP_TYPE_hi3716mv420)
    case 0:
        val_clk = 1000000;     // APLL
        break;
    case 1:
        val_clk = 800000;
        break;
    case 2:
        val_clk = 600000;
        break;
    case 3:
        val_clk = 1200000;
        break;
    case 4:
        val_clk = 24000;
        break;
    case 5:
        val_clk = 400000;
        break;
    case 6:
        val_clk = 300000;
        break;
    case 7:
        val_clk = 200000;
        break;
#else
    case 0:
        val_clk = 1000000;     // APLL
        break;
    case 1:
        val_clk = 800000;
        break;
    case 3:
        val_clk = 1500000;
        break;
    case 4:
        val_clk = 24000;
        break;
    case 5:
        val_clk = 1200000;
        break;
    case 6:
        val_clk = 400000;
        break;
    case 7:
        val_clk = 600000;
        break;
#endif
    default:
        HI_ERR_PM("mpu_clk_get_rate %dk not support\n", reg_clk);
        return 0;
    }

    return val_clk;
}

#endif

static void mpu_clk_init(struct clk *clk)
{
    U_PERI_CRG18 unTmpValue;

    /* set cpu switch by software register */
    unTmpValue.u32 = g_pstRegCrg->PERI_CRG18.u32;
#if  defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)   \
    || defined(CHIP_TYPE_hi3798cv100_a) \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3796cv100_a) \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3719mv100_a)
    unTmpValue.bits.cpu_div_cfg_bypass = 0x1;
#endif
    unTmpValue.bits.cpu_begin_cfg_bypass = 0x1;
    g_pstRegCrg->PERI_CRG18.u32 = unTmpValue.u32;

    return;
}

static struct clk_ops mpu_clock_ops = {
    .set_rate = mpu_clk_set_rate,
    .init     = mpu_clk_init,
};

void  mpu_init_clocks(void)
{
    HI_INFO_PM("Enter %s\n", __FUNCTION__);
    mpu_ck.rate = mpu_clk_get_rate();
    if (0 == mpu_ck.rate)
    {
        mpu_ck.rate = DEFAULT_INIT_FREQ;
    }

    mpu_ck.ops = &mpu_clock_ops;
    mpu_ck.max_rate = MAX_FREQ;
    mpu_ck.min_rate = MIN_FREQ;

    mpu_ck.spinlock = mpu_clock_lock;
    hi_clk_init(&mpu_ck);

    mpu_clk_set_rate(&mpu_ck, mpu_ck.rate);

    return;
}

void  mpu_resume_clocks(void)
{
    mpu_clk_set_rate(&mpu_ck, mpu_ck.rate);

    return;
}

void  mpu_exit_clocks(void)
{
    hi_clk_exit(&mpu_ck);
}
