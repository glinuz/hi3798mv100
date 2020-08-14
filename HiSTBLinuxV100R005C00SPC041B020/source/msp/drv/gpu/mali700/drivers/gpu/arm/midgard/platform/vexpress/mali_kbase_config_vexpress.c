/*
 *
 * (C) COPYRIGHT 2011-2015 ARM Limited. All rights reserved.
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





#include <linux/ioport.h>
#include <asm/delay.h>
#include <mali_kbase.h>
#include <mali_kbase_defs.h>
#include <mali_kbase_config.h>
#include "mali_kbase_config_platform.h"
#include "mali_kbase_cpu_vexpress.h"
#include "mali_gpu_proc.h"


#define HARD_RESET_AT_POWER_OFF 0


#define KBASE_HISI_MAX_FREQ_NUM 	6

#define KBASE_HISI_GPU_VOLT_MIN 750
#define KBASE_HISI_GPU_VOLT_MAX 1150

#ifdef CHIP_TYPE_hi3798cv200_a
#define KBASE_HISI_GPU_VOLT_MIN 750
#define KBASE_HISI_GPU_VOLT_MAX 1150
#endif

#ifdef CHIP_TYPE_hi3798cv200_b
#define KBASE_HISI_GPU_VOLT_MIN 650
#define KBASE_HISI_GPU_VOLT_MAX 1150
#endif

#define KBASE_MAX_VOLTAGE_DEC 	120

#define KBASE_HISI_GPU_BASE_ADDR		0xF9200000
#define KBASE_HISI_GPU_CRG_BASE		0xf8a22000
#define KBASE_HISI_GPU_CRG_SIZE		0x200
#define KBASE_HISI_GPU_PMC_BASE		0xf8a23000
#define KBASE_HISI_GPU_PMC_SIZE		0x200
#define KBASE_HISI_GPU_VMIN_BASE		0xF80000F8
#define KBASE_HISI_GPU_VMIN_SIZE		0x20
#define KBASE_HISI_CRG_FREQ_OFFSET 	0x124
#define KBASE_HISI_CRG_VOLT_OFFSET	0x1020

/* When VSync is being hit aim for utilisation between 70-90% */
#define KBASE_PM_VSYNC_MIN_UTILISATION          70
#define KBASE_PM_VSYNC_MAX_UTILISATION          90
/* Otherwise aim for 10-40% */
#define KBASE_PM_NO_VSYNC_MIN_UTILISATION       10
#define KBASE_PM_NO_VSYNC_MAX_UTILISATION       40


typedef enum
{
	KBASE_HISI_CRG_GPLL_OK				=0x154,
	KBASE_HISI_CRG_REST_OFFSET 			= 0x0d4,
	KBASE_HISI_CRG_GPLL_TUNE_INT 			= 0x208,
	KBASE_HISI_CRG_GPLL_TUNE_FRAC 		= 0x20C,
	KBASE_HISI_CRG_GPLL_TUNE_STEP_INT 	= 0x210,
	KBASE_HISI_CRG_GPLL_TUNE_STEP_FRAC 	= 0x214,
	KBASE_HISI_CRG_GPLL_TUNE_MODE 		= 0x218,
	KBASE_HISI_CRG_GPLL0 					= 0x244,
	KBASE_HISI_CRG_GPLL1 					= 0x248,
	KBASE_HISI_CRG_GPLL_SSMOD 			= 0x270,
	KBASE_HISI_CRG_GPLL_TUNE_BUSY		= 0x2BC
} KBASE_HISI_CRG;

typedef enum
{
	KBASE_HISI_PMC_HPM1_CTRL				= 0x0068,
	KBASE_HISI_PMC_HPM1_RECORD1			= 0x006c,
	KBASE_HISI_PMC_HPM1_RECORD2			= 0x0070,
	KBASE_HISI_PMC_HPM1_MONITOR			= 0x0074
} KBASE_HISI_PMC;


static volatile u32* g_pu32CRGBase = NULL;
static volatile u32* g_pu32PMCBase = NULL;
static volatile u32* g_pu32VminReg = NULL;
static u32 kbase_utilisation;
static u32 kbase_dvfs = 1;
static u32 kbase_avs = 1;
static u32 kbase_gpu_power_state = 0;
static u32 kbase_util_gl_share;
static u32 kbase_util_cl_share[2];
static u32 kbase_max_freq = 600000;
static u32 kbase_min_freq = 198000;


#ifndef CONFIG_OF
static struct kbase_io_resources io_resources = {
	.job_irq_number = 174,
	.mmu_irq_number = 173,
	.gpu_irq_number = 175,
	.io_memory_region = {
	.start = KBASE_HISI_GPU_BASE_ADDR,
	.end = KBASE_HISI_GPU_BASE_ADDR + (4096 * 4) - 1
	}
};
#endif /* CONFIG_OF */

static int kbase_write_crg_reg(u32 reg, u32 value)
{
	*(g_pu32CRGBase +  (reg/4)) = value;
	return 1;
}

static int kbase_read_crg_reg(u32 reg)
{
	return *(g_pu32CRGBase +  (reg/4));
}

static int kbase_write_pmc_reg(u32 reg, u32 value)
{
	*(g_pu32PMCBase +  (reg/4)) = value;
	return 1;
}

static int kbase_read_pmc_reg(u32 reg)
{
	return *(g_pu32PMCBase +  (reg/4));
}

typedef struct FREQ_TABLE
{
	unsigned int freq;       		/* unit: KHZ */
	unsigned int regvalue;		/* register value */
} FREQ_TABLE;

FREQ_TABLE kbase_freq_table[KBASE_HISI_MAX_FREQ_NUM] =
{
    {600000, 0x603},
    {500000, 0x606},
    {432000, 0x600},
    {400000, 0x601},
    {300000, 0x604},
    {200000, 0x607}
};

typedef struct GPLL_TABLE
{
	int freq;       		/* unit: KHZ */
	int volt;			/* unit: mV */
	int hpm;
} GPLL_TABLE;

GPLL_TABLE kbase_gpll_table[KBASE_HISI_MAX_FREQ_NUM] =
{
    {198000, 800, 0x00},
    {300000, 820, 0x00},
    {396000, 840, 0xd0},
    {432000, 860, 0xf0},
    {498000, 910, 0x100},
    {600000, 980, 0x126}
};

static int pm_callback_power_on(struct kbase_device *kbdev)
{
	if(NULL != g_pu32CRGBase)
	{
		/* Rest GPU Clock */
		*(g_pu32CRGBase +  (KBASE_HISI_CRG_REST_OFFSET/4)) = 0x11;
		*(g_pu32CRGBase +  (KBASE_HISI_CRG_REST_OFFSET/4)) = 0x01;
	}
	kbase_gpu_power_state = 1;
	return 1;	
}

static void pm_callback_power_off(struct kbase_device *kbdev)
{
#if HARD_RESET_AT_POWER_OFF
	/* Cause a GPU hard reset to test whether we have actually idled the GPU
	 * and that we properly reconfigure the GPU on power up.
	 * Usually this would be dangerous, but if the GPU is working correctly it should
	 * be completely safe as the GPU should not be active at this point.
	 * However this is disabled normally because it will most likely interfere with
	 * bus logging etc.
	 */
	KBASE_TRACE_ADD(kbdev, CORE_GPU_HARD_RESET, NULL, NULL, 0u, 0);
	kbase_os_reg_write(kbdev, GPU_CONTROL_REG(GPU_COMMAND), GPU_COMMAND_HARD_RESET);
#endif
	kbase_gpu_power_state = 0;
}

static int platform_callback_init(struct kbase_device *kbdev)
{
	return 0;
}

static void platform_callback_term(struct kbase_device *kbdev)
{
	gpu_proc_destroy();
}
	
struct kbase_pm_callback_conf pm_callbacks = {
	.power_on_callback = pm_callback_power_on,
	.power_off_callback = pm_callback_power_off,
	.power_suspend_callback  = NULL,
	.power_resume_callback = NULL
};

struct kbase_platform_funcs_conf platform_callbacks = {
	.platform_init_func = platform_callback_init,
	.platform_term_func = platform_callback_term	
};

/***************************************
 *
 *
 * Initialize GPU while boot up.
 * Reset GPU clock and set voltage to GPLL 500MHz.
 *
 *
 ***************************************/
int kbase_platform_early_init(void)
{
	/* Nothing needed at this stage */
	g_pu32CRGBase = (volatile u32*)ioremap_nocache(KBASE_HISI_GPU_CRG_BASE,  KBASE_HISI_GPU_CRG_SIZE);
	g_pu32PMCBase = (volatile u32*)ioremap_nocache(KBASE_HISI_GPU_PMC_BASE,  KBASE_HISI_GPU_PMC_SIZE);	
	g_pu32VminReg = (volatile u32*)ioremap_nocache(KBASE_HISI_GPU_VMIN_BASE, KBASE_HISI_GPU_VMIN_SIZE);
	
	if(NULL != g_pu32CRGBase)
	{
		 /* Reset GPU Clock */
		kbase_write_crg_reg	(KBASE_HISI_CRG_REST_OFFSET, 0x11);
		kbase_write_crg_reg(KBASE_HISI_CRG_REST_OFFSET, 0x01);

		kbase_set_voltage(910);
		kbase_set_frequency_slowly(498000);		
		
#ifdef CHIP_TYPE_hi3798cv200_a
		kbase_set_max_frequency(498000);
#endif

#ifdef CHIP_TYPE_hi3798cv200_b
		kbase_set_max_frequency(498000);
#endif



		
	}
	kbase_avs_init();
	gpu_proc_create();
	
	return 0;
}

static struct kbase_platform_config versatile_platform_config = {
#ifndef CONFIG_OF
	.io_resources = &io_resources
#endif
};

struct kbase_platform_config *kbase_get_platform_config(void)
{
	return &versatile_platform_config;
}

/** 
 *
 * Initialize avs.
 * 
 */
int kbase_avs_init(void)
{
    u32 regval;

    /* set monitor period to 4ms */
    regval = kbase_read_pmc_reg(KBASE_HISI_PMC_HPM1_MONITOR);
    regval &= 0x00ffffff;
    regval |= (1 << 24);
    kbase_write_pmc_reg(KBASE_HISI_PMC_HPM1_MONITOR, regval);
	
    /* hpm enable */
    regval = kbase_read_pmc_reg(KBASE_HISI_PMC_HPM1_CTRL);
    regval |= (1 << 24);
    kbase_write_pmc_reg(KBASE_HISI_PMC_HPM1_CTRL, regval);

    /* hpm monitor enable */
    regval = kbase_read_pmc_reg(KBASE_HISI_PMC_HPM1_CTRL);
    regval |= (1 << 26);
    kbase_write_pmc_reg(KBASE_HISI_PMC_HPM1_CTRL, regval);

    return 0;
}

int kbase_avs_sethpm(int gpufreq)
{
    u32 regval;

    int div = gpufreq / 50000;

    if ((gpufreq - div * 50000) >= 25000)
    {
        div += 1;
    }

    div -= 1;

    /* set time division */
    regval = kbase_read_pmc_reg(KBASE_HISI_PMC_HPM1_CTRL);
    regval &= 0xffffffc0;
    regval |= div;
    kbase_write_pmc_reg(KBASE_HISI_PMC_HPM1_CTRL, regval);
    return 0;
}

int kbase_avs_start(unsigned int profile)
{
	u16 hpm_code0, hpm_code1, hpm_code2, hpm_code3, hpm_code_avg;
	u32 hpm_code;
	int hpm_delta;

	hpm_code = kbase_read_pmc_reg(KBASE_HISI_PMC_HPM1_RECORD1);

	hpm_code0 = hpm_code & 0x3ff;
    	hpm_code1 = (hpm_code >> 12) & 0x3ff;

	hpm_code  = kbase_read_pmc_reg(KBASE_HISI_PMC_HPM1_RECORD2);
    	hpm_code2 = hpm_code & 0x3ff;
   	hpm_code3 = (hpm_code >> 12) & 0x3ff;

	hpm_code_avg = (hpm_code0 + hpm_code1 + hpm_code2 + hpm_code3) / 4;
    
	hpm_code = kbase_read_pmc_reg(KBASE_HISI_PMC_HPM1_RECORD1);

	hpm_code0 = hpm_code & 0x3ff;
	hpm_code1 = (hpm_code >> 12) & 0x3ff;

	hpm_code  = kbase_read_pmc_reg(KBASE_HISI_PMC_HPM1_RECORD2);
	hpm_code2 = hpm_code & 0x3ff;
	hpm_code3 = (hpm_code >> 12) & 0x3ff;

	hpm_code_avg = (hpm_code_avg * 4 + hpm_code0 + hpm_code1 + hpm_code2 + hpm_code3) / 8;
  
	hpm_delta = hpm_code_avg - kbase_gpll_table[profile].hpm;
	
	if (hpm_delta <= 0x1)
	{
		/* The voltage should not larger than vmin */
	    if ((kbase_get_voltage()+ 10) <= kbase_gpll_table[profile].volt)
	    {
	    	/* increase 10mv */
	        kbase_set_voltage(kbase_get_voltage()+ 10);
	    }
		}
	else if (hpm_delta >= 0xa)
	{
		/* The voltage should not be decrease than GPU_MAX_VOLTAGE_DEC */
	   	if (((kbase_get_voltage() - 10) >= (kbase_gpll_table[profile].volt - KBASE_MAX_VOLTAGE_DEC))&&((kbase_get_voltage() - 10) >= KBASE_HISI_GPU_VOLT_MIN))
	    {
	    	/* decrease 10mv */
	        kbase_set_voltage(kbase_get_voltage()- 10);
	    }
	}
	return 0;
}

/***************************************
 *
 *
 * Get GPU voltage by reading CRG regs.
 *
 *
 ***************************************/
int kbase_get_voltage(void)
{
	int volt;
	int value = kbase_read_crg_reg(KBASE_HISI_CRG_VOLT_OFFSET);
	value = value>>16;
	if( value > 0xa1 ||value < 0x01)
	{
		return 0;
	}

	volt = KBASE_HISI_GPU_VOLT_MAX - (value-1) * (KBASE_HISI_GPU_VOLT_MAX - KBASE_HISI_GPU_VOLT_MIN)/160;	
	return volt;
}

/***************************************
 *
 *
 * Set GPU voltage by writing CRG regs.
 *
 *
 ***************************************/
int kbase_set_voltage(unsigned int volt)
{
	int value;
	if(volt > KBASE_HISI_GPU_VOLT_MAX||volt < KBASE_HISI_GPU_VOLT_MIN)
	{
		return -1;
	}	
	value = 0x01 + (KBASE_HISI_GPU_VOLT_MAX - volt) * 160/(KBASE_HISI_GPU_VOLT_MAX - KBASE_HISI_GPU_VOLT_MIN);
	value = (value<<16) | 0x00a1;
	kbase_write_crg_reg(KBASE_HISI_CRG_VOLT_OFFSET, value);
	return 0;
}

/***************************************
 *
 *
 * Get GPU max frequency.
 *
 *
 ***************************************/
int kbase_get_max_frequency(void)
{		
	return kbase_max_freq;
}

/***************************************
 *
 *
 * Set GPU max frequency.
 *
 *
 ***************************************/
int kbase_set_max_frequency(unsigned int freq)
{
	int i;
	for(i = KBASE_HISI_MAX_FREQ_NUM - 1; i >= 0; i--)
	{
		if(kbase_gpll_table[i].freq <= freq)
		{
			kbase_max_freq = kbase_gpll_table[i].freq;

			if(kbase_get_frequency_slowly() > kbase_max_freq)
			{				
				kbase_set_frequency_slowly(kbase_gpll_table[i].freq);
				kbase_set_voltage(kbase_gpll_table[i].volt);
			}
			return 0;
		}
	}
	
	return -1;
}

/***************************************
 *
 *
 * Get GPU min frequency.
 *
 *
 ***************************************/
int kbase_get_min_frequency(void)
{		
	return kbase_min_freq;
}

/***************************************
 *
 *
 * Set GPU min frequency.
 *
 *
 ***************************************/
int kbase_set_min_frequency(unsigned int freq)
{
	int i;
	for(i = 0; i < KBASE_HISI_MAX_FREQ_NUM; i++)
	{
		if(kbase_gpll_table[i].freq >= freq)
		{
			kbase_min_freq = kbase_gpll_table[i].freq;

			if(kbase_get_frequency_slowly() < kbase_min_freq)
			{
				kbase_set_voltage(kbase_gpll_table[i].volt);
				kbase_set_frequency_slowly(kbase_gpll_table[i].freq);
			}
			return 0;
		}
	}
	
	return -1;
}


/***************************************
 *
 *
 * Get GPU frequency from GPLL by reading CRG regs.
 *
 *
 ***************************************/
int kbase_get_frequency_slowly(void)
{
	int postdiv1,postdiv2,refdiv,fbdiv;
	int reg;
	int freq;

	reg = kbase_read_crg_reg(KBASE_HISI_CRG_GPLL0);
	/*bit[30:28] pll post div1*/
	postdiv1 = (reg>>24)&0x7;
	/*bit[26:24] pll post div2*/
	postdiv2 = (reg>>28)&0x7;

	reg = kbase_read_crg_reg(KBASE_HISI_CRG_GPLL1);
	/*bit[17:12] pll reference div*/
	refdiv = (reg>>12)&0x3f;

	reg = kbase_read_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_BUSY);
	fbdiv = reg&0xfff;

	freq = fbdiv * 24000/(postdiv1 * postdiv2 * refdiv);

	return freq;
}

/***************************************
 *
 *
 * Set GPU frequency from GPLL by writing CRG regs.
 *
 *
 ***************************************/
int kbase_set_frequency_slowly(unsigned int freq)
{
	int postdiv1,postdiv2,refdiv,fbdiv;
	int reg;
	int i;

	*g_pu32VminReg = freq;
	
	/*switch to GPLL*/
	kbase_write_crg_reg(KBASE_HISI_CRG_FREQ_OFFSET, 0x202);
	kbase_write_crg_reg(KBASE_HISI_CRG_FREQ_OFFSET, 0x602);

	reg = kbase_read_crg_reg(KBASE_HISI_CRG_GPLL0);
	/*bit[30:28] pll post div1*/
	postdiv1 = (reg>>24)&0x7;
	/*bit[26:24] pll post div2*/
	postdiv2 = (reg>>28)&0x7;

	reg = kbase_read_crg_reg(KBASE_HISI_CRG_GPLL1);
	/*bit[17:12] pll reference div*/
	refdiv = (reg>>12)&0x3f;
	fbdiv = (freq*postdiv1*postdiv2*refdiv)/(24*1000);

	reg = reg&0xfdffffff;
	kbase_write_crg_reg(KBASE_HISI_CRG_GPLL1, reg);

	reg = kbase_read_crg_reg(KBASE_HISI_CRG_GPLL_SSMOD);
	/*bit[0] SSMOD clock gating enable, bit[4] SSMOD spread control*/
	reg = reg|0x11;
	kbase_write_crg_reg(KBASE_HISI_CRG_GPLL_SSMOD, reg);
	/*bit[2] to 0, SSMOD enable*/
	reg = reg&0xffb;
	kbase_write_crg_reg(KBASE_HISI_CRG_GPLL_SSMOD, reg);

	reg = kbase_read_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_INT);
	/* set fbdiv of target cpll */
	kbase_write_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_INT, ((reg&0xFFFFF000)|fbdiv));

   	reg = kbase_read_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_FRAC);
	/* set frac of target cpll to 0 */
	kbase_write_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_FRAC, reg&0xFF000000);

	reg = kbase_read_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_STEP_INT);
	/* set integer of tune step */
	kbase_write_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_STEP_INT, ((reg&0xFF000000)|0x1));

   	reg = kbase_read_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_STEP_FRAC);
	/* set integer of tune step */
	kbase_write_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_STEP_FRAC, reg&0xFF000000 );
	
	reg = kbase_read_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_MODE);
	/* set gpll tune mode */
	reg = reg|0x4;
	kbase_write_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_MODE, reg);
	/* begin one tune process */
	reg = reg&0xfffffff7;
	kbase_write_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_MODE, reg);		
	reg = reg|0x8;
	kbase_write_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_MODE, reg);
	udelay(1);
	
	/* check if pll switch is over */
	for(i = 0; i < 100; i++)
	{		
		reg = kbase_read_crg_reg(KBASE_HISI_CRG_GPLL_TUNE_BUSY);
		if((reg&0x2000)==0)
		{
			break;
		}
		udelay(10);
	}

	kbase_avs_sethpm(freq);

	return 0;
}

/***************************************
 *
 *
 * Rise GPU frequency by one step.
 * If current frequency equals max frequcy, do not rise.
 *
 *
 ***************************************/
int kbase_rise_frequency_slowly(void)
{
	int freq = kbase_get_frequency_slowly();
	int i;

	if(freq >= kbase_gpll_table[KBASE_HISI_MAX_FREQ_NUM-1].freq)
	{
		return -1;
	}

	for(i = 0; (i < KBASE_HISI_MAX_FREQ_NUM) && (kbase_gpll_table[i].freq <= kbase_max_freq); i++)
	{
		if(kbase_gpll_table[i].freq > freq)
		{
			kbase_set_voltage(kbase_gpll_table[i].volt);
			kbase_set_frequency_slowly(kbase_gpll_table[i].freq);
			return 0;
		}
	}

	return -1;
}

/***************************************
 *
 *
 * Drop GPU frequency by one step.
 * If current frequency equals min frequcy, do not drop.
 *
 *
 ***************************************/
int kbase_drop_frequency_slowly(void)
{
	int freq = kbase_get_frequency_slowly();
	int i;

	if(freq <= kbase_gpll_table[0].freq)
	{
		return -1;
	}
	
	for(i = KBASE_HISI_MAX_FREQ_NUM - 1; (i >= 0) && (kbase_gpll_table[i].freq >= kbase_min_freq); i--)
	{
		if(kbase_gpll_table[i].freq < freq)
		{
			kbase_set_frequency_slowly(kbase_gpll_table[i].freq);
			kbase_set_voltage(kbase_gpll_table[i].volt);
			return 0;
		}
	}

	return -1;
}


/***************************************
 *
 *
 * Get GPU frequency from switch PLL by reading CRG regs.
 *
 *
 ***************************************/
int kbase_get_frequency(void)
{
	int value = kbase_read_crg_reg(KBASE_HISI_CRG_FREQ_OFFSET);
	int i;
	for(i = 0; i < KBASE_HISI_MAX_FREQ_NUM; i++)
	{
		if(kbase_freq_table[i].regvalue==value)
		{			
			return kbase_freq_table[i].freq;
		}
	}
	return -1;
}

/***************************************
 *
 *
 * Set GPU frequency from switch PLL by writing CRG regs.
 *
 *
 ***************************************/
int kbase_set_frequency(unsigned int freq)
{

	int value;
	int i;
	for(i = 0; i < KBASE_HISI_MAX_FREQ_NUM; i++)
	{
		if(kbase_freq_table[i].freq==freq)
		{
			value=kbase_freq_table[i].regvalue;
			kbase_write_crg_reg(KBASE_HISI_CRG_FREQ_OFFSET, (value - 0x400));
			kbase_write_crg_reg(KBASE_HISI_CRG_FREQ_OFFSET, value);
			kbase_avs_sethpm(freq);		
			
			return 0;
		}
	}
	printk("Set frequency failed! Invalid frequency.\n");
	return -1;
}

int kbase_dvfs_enable(unsigned int enable)
{
	kbase_dvfs = enable;
	return 0;
}

int kbase_is_dvfs_enable()
{
	return kbase_dvfs;
}

int kbase_avs_enable(unsigned int enable)
{
	kbase_avs = enable;
	return 0;
}

int kbase_is_avs_enable()
{
	return kbase_avs;
}

int kbase_is_powerup(void)
{
	return kbase_gpu_power_state;
}

/***************************************
 *
 *
 * GPU DVFS event implementation.
 *
 *
 ***************************************/
 
int kbase_platform_dvfs_event(struct kbase_device *kbdev, u32 utilisation, u32 util_gl_share, u32 util_cl_share[2])
{	
	int i;
	int profile;
	int freq;
	
	kbase_pm_dvfs_action action;
	if (kbase_get_vsync()) {
		/* VSync is being met */
		if (utilisation < KBASE_PM_VSYNC_MIN_UTILISATION)
			action = KBASE_PM_DVFS_CLOCK_DOWN;
		else if (utilisation > KBASE_PM_VSYNC_MAX_UTILISATION)
			action = KBASE_PM_DVFS_CLOCK_UP;
		else
			action = KBASE_PM_DVFS_NOP;
	} else {
		/* VSync is being missed */
		if (utilisation < KBASE_PM_NO_VSYNC_MIN_UTILISATION)
			action = KBASE_PM_DVFS_CLOCK_DOWN;
		else if (utilisation > KBASE_PM_NO_VSYNC_MAX_UTILISATION)
			action = KBASE_PM_DVFS_CLOCK_UP;
		else
			action = KBASE_PM_DVFS_NOP;
	}	

	kbase_utilisation = utilisation;
	kbase_util_gl_share = util_gl_share;
	for(i = 0; i < 2; i++)
	{
		kbase_util_cl_share[i] = util_cl_share[i];
	}

	switch(action)
	{
		case KBASE_PM_DVFS_NOP:
		{
			if(kbase_avs == 1)
			{
				freq = kbase_get_frequency_slowly();
				if(freq <= kbase_gpll_table[1].freq)
				{
					/* no avs */  
					
					break;
				}
				profile = kbase_get_profile(freq);
				kbase_avs_start(profile);
			}
			break;
		}
		case KBASE_PM_DVFS_CLOCK_UP:
		{
			if(kbase_dvfs == 1)
			{
				if(kbase_rise_frequency_slowly() < 0)
				{
					if(kbase_avs == 1)
					{
						kbase_avs_start(KBASE_HISI_MAX_FREQ_NUM - 1);
					}
				}
			}
			break;
		}
		case KBASE_PM_DVFS_CLOCK_DOWN:
		{
			if(kbase_dvfs == 1)
			{
				kbase_drop_frequency_slowly();
			}
			break;
		}
		default:
		{
			break;
		}
	}
	return 0;
}


bool kbase_get_vsync(void)
{
	extern bool HIFB_DRV_IsFrameHit(void);
	return HIFB_DRV_IsFrameHit();
}

int kbase_get_utilisation()
{
	return kbase_utilisation;
}

int kbase_get_profile(unsigned int freq)
{	
	int i;
	for(i = 0; i < KBASE_HISI_MAX_FREQ_NUM; i++)
	{
		if(kbase_gpll_table[i].freq == freq)
		{
			return i;
		}
	}
	return -1;
}
