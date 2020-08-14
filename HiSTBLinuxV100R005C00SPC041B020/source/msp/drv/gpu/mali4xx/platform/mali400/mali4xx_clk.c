/*#include <linux/clk.h>*/
/*#include <mach/platform.h>*/
#include <linux/clkdev.h>
#include <asm/clkdev.h>
/*#include <mach/clock.h>*/
#include <linux/io.h>
#include <linux/module.h>   /* kernel module definitions */
#include <linux/fs.h>       /* file system operations */
#include <linux/cdev.h>     /* character device definitions */
#include <linux/delay.h>
#include "mali_kernel_common.h"
#include "linux/mali/mali_utgard.h"
#include "mali_hw_core.h"

#include "mali4xx_cfg.h"
#include "mali4xx_reg.h"
#include "mali4xx_clk.h"
#include "mali4xx_pmu.h"

#ifdef CONFIG_HI_PMU_DEVICE_SELECT
 #include "hi_module.h"
 #include "drv_i2c_ext.h"
 #include "hi_drv_module.h"
#endif

typedef struct hiGPU_VF_S
{
	unsigned int freq;       		/* unit: KHZ */
	unsigned int voltage;    		/* unit: mv  */
	unsigned int hpmrecord;  	/* hpm record */
	unsigned int regvalue;		/* register value */
} GPU_VF_S;

#define MAX_FREQ_NUM 8
#if defined (CHIP_TYPE_hi3719mv100)
GPU_VF_S gpu_freq_volt_table[MAX_FREQ_NUM] =
{
    {432000, 1130, 0x161,0x0}, 
    {400000, 1090, 0x146,0x1}, 
    {375000, 1060, 0x141,0x2}, 
    {345600, 1030, 0x120,0x3},
    {300000, 1000, 0x110,0x4}, 
    {250000, 1000, 0x110,0x5}, 
    {200000, 1000, 0x110,0x6}, 
    {150000, 1000, 0x110,0x7}
};
#elif defined(CHIP_TYPE_hi3718mv100)
GPU_VF_S gpu_freq_volt_table[MAX_FREQ_NUM] =
{
    {432000, 1190, 0x188,0x0}, 
    {400000, 1140, 0x150,0x1}, 
    {375000, 1120, 0x147,0x2}, 
    {345600, 1080, 0x120,0x3},
    {300000, 1020, 0x110,0x4}, 
    {250000, 1000, 0x110,0x5}, 
    {200000, 1000, 0x110,0x6}, 
    {150000, 1000, 0x110,0x7}
};
#elif defined (CHIP_TYPE_hi3716mv420) || defined (CHIP_TYPE_hi3716mv410)
GPU_VF_S gpu_freq_volt_table[MAX_FREQ_NUM] =
{
    {500000, 0, 0,0x0}, 
    {432000, 0, 0,0x1}, 
    {400000, 0, 0,0x2},
    {345600, 0, 0,0x3}, 
    {300000, 0, 0,0x4},
    {250000, 0, 0,0x5}, 
    {200000, 0, 0,0x7}, 
    {150000, 0, 0,0x6}
};
#else
GPU_VF_S gpu_freq_volt_table[MAX_FREQ_NUM] =
{
    {432000, 1215, 0x148,0x0}, 
    {400000, 1170, 0x128,0x1}, 
    {375000, 1125, 0x120,0x2}, 
    {345600, 1080, 0x100,0x3},
    {300000, 1020, 0x0e8,0x4}, 
    {250000, 1000, 0x110,0x5}, 
    {200000, 1000, 0x110,0x6}, 
    {150000, 1000, 0x110,0x7}
};
#endif

#if defined (HI_DONGLE_MODE_NORMAL)
#define DEFAULT_FREQ 		250000
#define MAX_FREQ 			250000
#define DEFAULT_INDEX		5
#elif  defined (HI_DONGLE_MODE_LOW)
#define DEFAULT_FREQ 		150000
#define MAX_FREQ 			150000
#define DEFAULT_INDEX		7
#elif defined (HI_DONGLE_MODE_CRITICAL)
#define DEFAULT_FREQ 		150000
#define MAX_FREQ 			150000
#define DEFAULT_INDEX		7
#else
#define DEFAULT_FREQ 		345600
#define MAX_FREQ 			432000
#define DEFAULT_INDEX		3
#endif

#define MAX_STEP_CHANGE 2

#ifdef CONFIG_HI_PMU_DEVICE_SELECT
#define VMAX 1500          /*mv*/
#define VMIN 870           /*mv*/
#else
#define VMAX 1320          /*mv*/
#define VMIN 900           /*mv*/
#endif
#define PWM_STEP 5       /*mv*/
#define PWM_CLASS 2

#define PWM_DUTY_MASK 0xffff0000
#define PWM_PERIOD_MASK 0xffff
#define PWM_ENABLE_BIT 0x2

#ifdef CONFIG_HI_PMU_DEVICE_SELECT
#define GPU_I2C_NUM 0
#define GPU_PMU_DEVICE_ADDRESS 0x90
#define GPU_ADDRESS 0x2
#define GPU_PMU_STEP 10
#endif

unsigned int gpu_cur_volt = 0;

static struct mali_hw_core *s_hisi_crg = NULL;
static struct mali_hw_core *s_hisi_pmc = NULL;

#ifdef GPU_MAXFREQ_CONFIG_SUPPORT
unsigned int mali_dvfs_max_freqency = MAX_FREQ;
module_param(mali_dvfs_max_freqency, int, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH); /* rw-rw-r-- */
MODULE_PARM_DESC(mali_dvfs_max_freqency, "max freqency");
#endif

#ifdef GPU_AVS_SUPPORT

 #define GPU_MAX_VOLTAGE_DEC 120

int gpu_avs_init(void)
{
    u32 regval;
    u32 hpm_ctl = PMC_REG_ADDR_HPM1_CTL;
    u32 hpm_monitor = PMC_REG_ADDR_HPM1_MONITOR;

    /* set monitor period to 4ms */
    regval  = mali_hw_core_register_read(s_hisi_pmc, hpm_monitor);
    regval &= 0x00ffffff;
    regval |= (1 << 24);
    mali_hw_core_register_write(s_hisi_pmc, hpm_monitor, regval);

    /* hpm enable */
    regval  = mali_hw_core_register_read(s_hisi_pmc, hpm_ctl);
    regval |= (1 << 24);
    mali_hw_core_register_write(s_hisi_pmc, hpm_ctl, regval);

    /* hpm monitor enable */
    regval  = mali_hw_core_register_read(s_hisi_pmc, hpm_ctl);
    regval |= (1 << 26);
    mali_hw_core_register_write(s_hisi_pmc, hpm_ctl, regval);

    return 0;
}

int gpu_avs_sethpm(int gpufreq)
{
    u32 regval;
    u32 hpm_ctl = PMC_REG_ADDR_HPM1_CTL;

    int div = gpufreq / 50000;

    if ((gpufreq - div * 50000) >= 25000)
    {
        div += 1;
    }

    div -= 1;

    /* set time division */
    regval  = mali_hw_core_register_read(s_hisi_pmc, hpm_ctl);
    regval &= 0xffffffc0;
    regval |= div;
    mali_hw_core_register_write(s_hisi_pmc, hpm_ctl, regval);

    return 0;
}

int gpu_avs_start(int profile)
{
    u16 hpm_code0, hpm_code1, hpm_code2, hpm_code3, hpm_code_avg;
    u32 hpm_code;
    int hpm_delta;

    hpm_code = mali_hw_core_register_read(s_hisi_pmc, PMC_REG_ADDR_HPM1_RECORD1);

    hpm_code0 = hpm_code & 0x3ff;
    hpm_code1 = (hpm_code >> 12) & 0x3ff;

    hpm_code  = mali_hw_core_register_read(s_hisi_pmc, PMC_REG_ADDR_HPM1_RECORD2);
    hpm_code2 = hpm_code & 0x3ff;
    hpm_code3 = (hpm_code >> 12) & 0x3ff;

    hpm_code_avg = (hpm_code0 + hpm_code1 + hpm_code2 + hpm_code3) / 4;
    MALI_DEBUG_PRINT(1, ("hpm_code1 = (0x%x- 0x%x- 0x%x- 0x%x- 0x%x)\n", hpm_code0, hpm_code1, hpm_code2, hpm_code3,
                         hpm_code_avg));

    hpm_code = mali_hw_core_register_read(s_hisi_pmc, PMC_REG_ADDR_HPM1_RECORD1);

    hpm_code0 = hpm_code & 0x3ff;
    hpm_code1 = (hpm_code >> 12) & 0x3ff;

    hpm_code  = mali_hw_core_register_read(s_hisi_pmc, PMC_REG_ADDR_HPM1_RECORD2);
    hpm_code2 = hpm_code & 0x3ff;
    hpm_code3 = (hpm_code >> 12) & 0x3ff;

    hpm_code_avg = (hpm_code_avg * 4 + hpm_code0 + hpm_code1 + hpm_code2 + hpm_code3) / 8;
    MALI_DEBUG_PRINT(1, ("hpm_code2 = (0x%x- 0x%x- 0x%x- 0x%x- 0x%x)\n", hpm_code0, hpm_code1, hpm_code2, hpm_code3,
                         hpm_code_avg));

    hpm_delta = hpm_code_avg - gpu_freq_volt_table[profile].hpmrecord;

    if (hpm_delta <= 0x1)
    {
        /* The voltage should not larger than vmin */
        if ((gpu_cur_volt + 10) <= gpu_freq_volt_table[profile].voltage)
        {
            /* increase 10mv */
            clk_gpu_set_voltage(gpu_cur_volt + 10);
        }
    }
    else if (hpm_delta >= 0xa)
    {
        /* The voltage should not be decrease than GPU_MAX_VOLTAGE_DEC */
        if ((gpu_cur_volt - 10) >= (gpu_freq_volt_table[profile].voltage - GPU_MAX_VOLTAGE_DEC))
        {
            /* decrease 10mv */
            clk_gpu_set_voltage(gpu_cur_volt - 10);
        }
    }

    return 0;
}

#endif

/* GPU reset */
void hisi_crg_reset(void)
{
    u32 gpuclock;

    gpuclock = mali_hw_core_register_read(s_hisi_crg, CRG_REG_ADDR_SOFT_RST);

    /* reset */
    gpuclock = (gpuclock | GPU_ALL_RESET_MASK);
    mali_hw_core_register_write(s_hisi_crg, CRG_REG_ADDR_SOFT_RST, gpuclock);

    _mali_osk_time_ubusydelay(1);

    /* cancel reset */
    gpuclock = (gpuclock & (~GPU_ALL_RESET_MASK));
    mali_hw_core_register_write(s_hisi_crg, CRG_REG_ADDR_SOFT_RST, gpuclock);
}

/* GPU clock on                                                         */
void hisi_crg_clockon(void)
{
    u32 gpuclock;

    gpuclock = mali_hw_core_register_read(s_hisi_crg, CRG_REG_ADDR_SOFT_RST);

    /* clock on */
    gpuclock = (gpuclock | GPU_CLOCK_ENABLE_MASK);

#if defined (CHIP_TYPE_hi3716mv400) || defined (CHIP_TYPE_hi3716mv420) || defined (CHIP_TYPE_hi3716mv410)
    gpuclock = (gpuclock | GPU_CLOCK_PP0_ENABLE_MASK);
#elif defined (CHIP_TYPE_hi3719mv100) || defined (CHIP_TYPE_hi3718mv100)
    gpuclock = (gpuclock | GPU_CLOCK_PP0_ENABLE_MASK | GPU_CLOCK_PP1_ENABLE_MASK);
#elif defined (CHIP_TYPE_hi3716cv200) || defined (CHIP_TYPE_hi3719cv100) || defined (CHIP_TYPE_hi3718cv100)
    gpuclock = (gpuclock | GPU_CLOCK_PP0_ENABLE_MASK | GPU_CLOCK_PP1_ENABLE_MASK | GPU_CLOCK_PP2_ENABLE_MASK | GPU_CLOCK_PP3_ENABLE_MASK);
#endif

    mali_hw_core_register_write(s_hisi_crg, CRG_REG_ADDR_SOFT_RST, gpuclock);
    MALI_DEBUG_PRINT(2, ("Hisi gpu clock on\n"));
}

/* GPU clock off                                                        */
void hisi_crg_clockoff(void)
{
    u32 gpuclock = mali_hw_core_register_read(s_hisi_crg, CRG_REG_ADDR_SOFT_RST);

    /* clock off */
    gpuclock = (gpuclock & (~GPU_CLOCK_ENABLE_MASK));

#if defined (CHIP_TYPE_hi3716mv400) || defined (CHIP_TYPE_hi3716mv420) || defined (CHIP_TYPE_hi3716mv410)
    gpuclock = (gpuclock & (~(GPU_CLOCK_PP0_ENABLE_MASK)));
#elif defined (CHIP_TYPE_hi3719mv100) || defined (CHIP_TYPE_hi3718mv100)
    gpuclock = (gpuclock & (~(GPU_CLOCK_PP0_ENABLE_MASK | GPU_CLOCK_PP1_ENABLE_MASK)));
#elif defined (CHIP_TYPE_hi3716cv200) || defined (CHIP_TYPE_hi3719cv100) || defined (CHIP_TYPE_hi3718cv100)
    gpuclock = (gpuclock & (~(GPU_CLOCK_PP0_ENABLE_MASK | GPU_CLOCK_PP1_ENABLE_MASK | GPU_CLOCK_PP2_ENABLE_MASK | GPU_CLOCK_PP3_ENABLE_MASK)));
#endif

    mali_hw_core_register_write(s_hisi_crg, CRG_REG_ADDR_SOFT_RST, gpuclock);
    MALI_DEBUG_PRINT(2, ("Hisi gpu clock off\n"));
}

#ifdef GPU_MAXFREQ_CONFIG_SUPPORT
int clk_gpu_get_max_index(void)
{
    int i = 0;

    for (; i < MAX_FREQ_NUM - 1; i++)
    {
        if(gpu_freq_volt_table[i].freq <= mali_dvfs_max_freqency)
        {
            break;
        }
    }

    mali_dvfs_max_freqency = gpu_freq_volt_table[i].freq;

    return i;
}
#endif

int clk_gpu_get_index(unsigned rate)
{
    int i = MAX_FREQ_NUM - 1;
#ifdef GPU_MAXFREQ_CONFIG_SUPPORT
    int j = clk_gpu_get_max_index();
#else
   int j = 0;
#endif

    for (; i > j; i--)
    {
        if (gpu_freq_volt_table[i].freq >= rate)
        {
            break;
        }
    }

    return i;
}
#if defined (CHIP_TYPE_hi3716mv420) || defined (CHIP_TYPE_hi3716mv410)
int clk_gpu_set_voltage(unsigned int volt)
{
    return 0;
}
#else
int clk_gpu_set_voltage(unsigned int volt)
{
#if defined (CONFIG_HI_PMU_DEVICE_SELECT)
    u32 s32Ret;
    u8 u8VoltReg;
    static I2C_EXT_FUNC_S *s_pI2CFunc = NULL;

    if ((volt < VMIN) || (volt > VMAX))
    {
        return 0;
    }

    if (NULL == s_pI2CFunc)
    {
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_I2C, (HI_VOID**)&s_pI2CFunc);
        if (HI_SUCCESS != s32Ret)
        {
            MALI_DEBUG_PRINT(1, ("HI_DRV_MODULE_GetFunction is failed\n"));
            return -1;
        }
    }

    /* transfer volt from 1100mv to 1150mv */

    u8VoltReg = ((((volt * 110) / 115) - VMIN) / GPU_PMU_STEP) << 2;
    if (s_pI2CFunc->pfnI2cWrite)
    {
        s32Ret = (s_pI2CFunc->pfnI2cWrite)(GPU_I2C_NUM, GPU_PMU_DEVICE_ADDRESS, GPU_ADDRESS, 1, &u8VoltReg, 1);
        if (s32Ret != HI_SUCCESS)
        {
            MALI_DEBUG_PRINT(1, ("call HI_I2C_Write failed.\n", volt));
            return s32Ret;
        }
    }

    gpu_cur_volt = volt;
    MALI_DEBUG_PRINT(1, ("@@Mali DVFS: set voltage = %d@@\n", volt));
    return 0;
#else
    unsigned int duty, v, tmp;
    unsigned int vmax, vmin, pwc, pws;

    if ((volt < VMIN) || (volt > VMAX))
    {
        return 0;
    }

    vmax = VMAX;
    vmin = VMIN;
    pwc = PWM_CLASS;
    pws = PWM_STEP;

    duty = (((vmax - volt) / pws) * pwc) - 1;
    v   = mali_hw_core_register_read(s_hisi_pmc, PMC_REG_ADDR_PWM2_DUTY);
    tmp = PWM_DUTY_MASK;
    v &= ~tmp;
    v |= duty << 16;
    mali_hw_core_register_write(s_hisi_pmc, PMC_REG_ADDR_PWM2_DUTY, v);

    gpu_cur_volt = volt;

    MALI_DEBUG_PRINT(1, ("Mali DVFS: set voltage = %d\n", volt));
    return 0;
#endif
}
#endif
int mali_gpu_set_voltage(unsigned int freq)
{
    int profile = clk_gpu_get_index(freq);

    if (gpu_cur_volt != gpu_freq_volt_table[profile].voltage)
    {
        clk_gpu_set_voltage(gpu_freq_volt_table[profile].voltage);
    }

    return 0;
}

/* set the div for the rate ,the parent must be set firstly */
int clk_gpu_set_rate(struct mali_clk *clk, unsigned rate)
{
    u32 freqctrl;
    u32 lowpower;
    int profile = clk_gpu_get_index(rate);

    freqctrl = mali_hw_core_register_read(s_hisi_pmc, PMC_REG_ADDR_GPU_FREQ);

    if (gpu_freq_volt_table[profile].freq > clk->rate)
    {
        /* increase frequency  */
        freqctrl = freqctrl | GPU_FREQ_SW_TREND_PMC_MASK;
    }
    else
    {
        /* decrease frequency  */
        freqctrl = freqctrl & (~GPU_FREQ_SW_TREND_PMC_MASK);
    }

    mali_hw_core_register_write(s_hisi_pmc, PMC_REG_ADDR_GPU_FREQ, freqctrl);
    lowpower = mali_hw_core_register_read(s_hisi_crg, CRG_REG_ADDR_LOW_POWER);

    /* set freq   */
    lowpower = lowpower & (~GPU_FREQ_SEL_CFG_CRG_MASK);
    lowpower = lowpower | (gpu_freq_volt_table[profile].regvalue);

    mali_hw_core_register_write(s_hisi_crg, CRG_REG_ADDR_LOW_POWER, lowpower);

    /* frequency request */
    freqctrl = freqctrl | GPU_FREQ_SW_REQ_PMC_MASK;

    mali_hw_core_register_write(s_hisi_pmc, PMC_REG_ADDR_GPU_FREQ, freqctrl);

    /* cancel frequency request */
    freqctrl = freqctrl & (~GPU_FREQ_SW_REQ_PMC_MASK);
    mali_hw_core_register_write(s_hisi_pmc, PMC_REG_ADDR_GPU_FREQ, freqctrl);

    clk->rate = gpu_freq_volt_table[profile].freq;

    MALI_DEBUG_PRINT(1, ("Mali DVFS:adjust frequency to %d\n", gpu_freq_volt_table[profile].freq));

#ifdef GPU_AVS_SUPPORT
    gpu_avs_sethpm(clk->rate);
#endif

#ifdef S40V200_VMIN_TEST
    gpu_set_freq_reg(clk->rate);
#endif
    return 0;
}

unsigned int clk_gpu_get_rate(struct mali_clk *clk)
{
    return clk->rate;
}

long clk_gpu_round_rate(struct mali_clk *clk, long rate)
{
    int i = clk_gpu_get_index(rate);
    int cur_step = clk_gpu_get_index(clk->rate);

    /* Make sure we do not jump large than 2 steps */
    if (i > (cur_step + MAX_STEP_CHANGE))
    {
        i = cur_step + MAX_STEP_CHANGE;
    }
    else if ((i + MAX_STEP_CHANGE) < cur_step)
    {
        i = cur_step - MAX_STEP_CHANGE;
    }

    return gpu_freq_volt_table[i].freq;
}

struct mali_clk clk_gpu =
{
	.name     = "clk_gpu",
	.rate     = DEFAULT_FREQ,
	.max_rate = MAX_FREQ,
};

void hisi_pmc_setparameter(void)
{
    u32 freqctrl;
    u32 lowpower;
    u32 pmu_select;

    pmu_select = mali_hw_core_register_read(s_hisi_pmc, PMC_REG_ADDR_GPU_CTRL);
#ifdef GPU_PMU_HISI
    /* pmu selection: use external pmu */
    pmu_select = pmu_select | (GPU_PMU_BYPASS_MASK);
#else
    /* pmu selection: use mali pmu */
    pmu_select = pmu_select & (~GPU_PMU_BYPASS_MASK);
#endif

    /* use cnt */
    pmu_select = pmu_select & (~GPU_PMU_CNT_BYPASS_MASK);

    mali_hw_core_register_write(s_hisi_pmc, PMC_REG_ADDR_GPU_CTRL, pmu_select);

    freqctrl = mali_hw_core_register_read(s_hisi_pmc, PMC_REG_ADDR_GPU_FREQ);

    /* use hardware status machine  */
    freqctrl = freqctrl | (GPU_FREQ_FSM_ENABLE_MASK);

    /* clear power good value  GPU  */
    freqctrl = freqctrl & (~GPU_FREQ_TIME_WAIT_POWER_GOOD_MASK);

    /* set power good value  GPU, set in the initial status */
    freqctrl = freqctrl | (GPU_FREQ_TIME_WAIT_POWER_GOOD_VALUE);

    /* use hardware power good   */
    freqctrl = freqctrl & (~GPU_USE_POWER_GOOD_CPU);

    mali_hw_core_register_write(s_hisi_pmc, PMC_REG_ADDR_GPU_FREQ, freqctrl);

    /* set power good wait time */
    mali_hw_core_register_write(s_hisi_pmc, PMC_REG_ADDR_POWER_TIME, GPU_PMC_POWER_TIME_VALUE);

    lowpower = mali_hw_core_register_read(s_hisi_crg, CRG_REG_ADDR_LOW_POWER);

    /* use status machine output */
    lowpower = (lowpower
               & (~(GPU_BEGIN_CFG_BYPASS_MASK | GPU_DIV_CFG_BYPASS_MASK | GPU_FREQ_DIV_CFG_CRG_MASK)))
               | GPU_FREQ_SEL_CFG_CRG_MASK;
    mali_hw_core_register_write(s_hisi_crg, CRG_REG_ADDR_LOW_POWER, lowpower);

#ifdef GPU_AVS_SUPPORT
    gpu_avs_init();
#endif
}

void clk_gpu_setdefault(void)
{
    /* set the default voltage */
    clk_gpu_set_voltage( gpu_freq_volt_table[DEFAULT_INDEX].voltage );

    /* set the default frequency */
    clk_gpu_set_rate( &clk_gpu, gpu_freq_volt_table[DEFAULT_INDEX].freq );
}

static inline void clk_gpu_initial(void)
{
    hisi_pmc_setparameter();
    clk_gpu_setdefault();
}

void __init gpu_init_clocks(void)
{
	s_hisi_pmc = hisi_pmu_get();
	s_hisi_crg = hisi_crg_get();

    hisi_crg_clockon();

    hisi_crg_reset();

#ifdef S40V200_VMIN_TEST
    gpu_vmin_init();
#endif

    clk_gpu_initial();
}

void gpu_deinit_clocks(void)
{
    hisi_crg_clockoff();
#ifdef S40V200_VMIN_TEST
    gpu_vmin_deinit();
#endif
}

#ifdef S40V200_VMIN_TEST
 #define GPU_REG_FREQ 0xF80000F8
 #define GPU_REG_UTILIZATION 0xF80000FC

static volatile unsigned int *g_pu32GPUFreq;
static volatile unsigned int *g_pu32GPUUtilization;

void gpu_vmin_init()
{
    g_pu32GPUFreq = ioremap_nocache(GPU_REG_FREQ, 32);
    g_pu32GPUUtilization = ioremap_nocache(GPU_REG_UTILIZATION, 32);
}

void gpu_vmin_deinit()
{
    iounmap(g_pu32GPUFreq);
    iounmap(g_pu32GPUUtilization);
}

void gpu_set_freq_reg(unsigned int freq)
{
    *g_pu32GPUFreq = freq;
}

void gpu_set_utilization_reg(unsigned int utilization)
{
    *g_pu32GPUUtilization = (utilization * 100) / 256;
}

#endif

int gpu_dvfs_set_voltage(unsigned int voltage)
{
    return clk_gpu_set_voltage(voltage);
}

int gpu_dvfs_set_freq(unsigned int freq)
{
    return clk_gpu_set_rate( &clk_gpu, freq);
}

unsigned int gpu_dvfs_get_voltage(void)
{
    return gpu_cur_volt;
}

unsigned int gpu_dvfs_get_freq(void)
{
    if (0 == gpu_is_powerdown())
    {
        return clk_gpu_get_rate(&clk_gpu);
    }
    else
    {
        return 0;
    }
}
