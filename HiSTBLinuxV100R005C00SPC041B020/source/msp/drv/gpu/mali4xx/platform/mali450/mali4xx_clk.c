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
#if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3796mv200)
	unsigned int voltage_ff;    	/* unit: mv for f3 corner */
	unsigned int voltage_ss;    	/* unit: mv for s3 corner */
#endif	
} GPU_VF_S;

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

#define MAX_FREQ_NUM	8
#define DEFAULT_FREQ 	345600
#define MAX_FREQ 		432000
#define DEFAULT_INDEX 	4

/*voltage switch parameters*/
#define VOLT_MAX 1400
#define VOLT_MIN 900
#define VOLT_STEP 5
#define VMAX_REG_BASE 0x000100C9
#define PWM_REG_OFFSET (1<<17)

GPU_VF_S gpu_freq_volt_table[MAX_FREQ_NUM] =
{
    {500000, 0x0, 0x0, 0x7},
    {432000, 0x0, 0x0, 0x0},
    {400000, 0x0, 0x0, 0x1},
    {375000, 0x0, 0x0, 0x2},
    {345600, 0x0, 0x0, 0x3},
    {300000, 0x0, 0x0, 0x4},
    {250000, 0x0, 0x0, 0x5},
    {200000, 0x0, 0x0, 0x6}
};

#elif defined(CHIP_TYPE_hi3798cv100) || defined(CHIP_TYPE_hi3796cv100)

#define MAX_FREQ_NUM	8
#define DEFAULT_FREQ 	345600
#define MAX_FREQ 		432000
#define DEFAULT_INDEX 	3

/*voltage switch parameters*/
#define VOLT_MAX 1400
#define VOLT_MIN 900
#define VOLT_STEP 5
#define VMAX_REG_BASE 0x000100C9
#define PWM_REG_OFFSET (1<<17)

GPU_VF_S gpu_freq_volt_table[MAX_FREQ_NUM] =
{
    {432000, 1160, 0x138, 0x0}, 
    {400000, 1120, 0x126, 0x1}, 
    {375000, 1090, 0x126, 0x2}, 
    {345600, 1050, 0x0, 0x3},
    {300000, 1050, 0x0, 0x4}, 
    {250000, 1000, 0x0, 0x5}, 
    {200000, 1000, 0x0, 0x6}, 
    {150000, 1000, 0x0, 0x7}
};

#elif defined(CHIP_TYPE_hi3798mv200_a)

#define MAX_FREQ_NUM	7
#define DEFAULT_FREQ 	500000
#define MAX_FREQ 		675000
#define DEFAULT_INDEX 	3

/*voltage switch parameters*/
#define VOLT_MAX 1250
#define VOLT_MIN 700
#define VOLT_STEP 5
#define VMAX_REG_BASE 0x000100DD
#define PWM_REG_OFFSET (1<<17)

GPU_VF_S gpu_freq_volt_table[MAX_FREQ_NUM] =
{
    {675000, 1000, 0x0, 0x5}, 
    {600000, 1000, 0x0, 0x3},
    {540000, 1000, 0x0, 0x0}, 
    {500000, 900, 0x0, 0x6}, 
    /*{430000, 1200, 0x0, 0x2},*/
    {400000, 900, 0x0, 0x1}, 
    {300000, 900, 0x0, 0x4}, 
    {200000, 900, 0x0, 0x7}
};

#elif defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3796mv200)

#define MAX_FREQ_NUM	8
#define DEFAULT_FREQ 	600000
#define MAX_FREQ 		800000
#define DEFAULT_INDEX 	3

/*voltage switch parameters*/
#define VOLT_MAX 1250
#define VOLT_MIN 700
#define VOLT_STEP 5
#define VMAX_REG_BASE 0x000100DD
#define PWM_REG_OFFSET (1<<17)
#define PWM_STEP  5
#define PWM_CLASS 2

GPU_VF_S gpu_freq_volt_table[MAX_FREQ_NUM] =
{
    {800000, 930, 0x0, 0x0, 900, 960},
    {750000, 910, 0x0, 0x2, 890, 940}, 
    {675000, 900, 0x0, 0x5, 850, 910}, 
    {600000, 870, 0x0, 0x3, 830, 910}, 
    {500000, 870, 0x0, 0x6, 830, 910},
    {400000, 870, 0x0, 0x1, 830, 910},
    {300000, 870, 0x0, 0x4, 830, 910},
    {200000, 870, 0x0, 0x7, 830, 910}
};

#endif


#define MAX_STEP_CHANGE 2

unsigned int gpu_cur_volt = 0;

static struct mali_hw_core *s_hisi_crg = NULL;
static struct mali_hw_core *s_hisi_pmc = NULL;
static struct mali_hw_core *s_hisi_sec = NULL;

#ifdef GPU_MAXFREQ_CONFIG_SUPPORT
unsigned int mali_dvfs_max_freqency = MAX_FREQ;
module_param(mali_dvfs_max_freqency, int, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH); /* rw-rw-r-- */
MODULE_PARM_DESC(mali_dvfs_max_freqency, "max freqency");
#endif

#if defined(CHIP_TYPE_hi3798mv200)
#define HISI_GPU_SVB_BASE 0xf8000000
#define HISI_GPU_SVB_SIZE 0x100
#define HISI_GPU_SVB_REGISTER1 0xc4		/*about: type - ff - ss*/
#define HISI_GPU_SVB_REGISTER2 0xc8		/*about: tt*/

typedef enum{    
	HISI_CHIP_TYPE_FF = 1,
	HISI_CHIP_TYPE_TT = 2,
	HISI_CHIP_TYPE_SS = 3,
}HISI_CHIP_TYPE;

int gpu_svb_identify(void)
{    
	volatile u32* pu32SVBBase = (u32*)ioremap_nocache(HISI_GPU_SVB_BASE,  HISI_GPU_SVB_SIZE);
	unsigned int core_vmin;
	int svb_type, svb_corner[3];
	int reg1, reg2;
	int i = 0;

	reg1 = *(pu32SVBBase +  (HISI_GPU_SVB_REGISTER1 >> 2));
	reg2 = *(pu32SVBBase +  (HISI_GPU_SVB_REGISTER2 >> 2));

	svb_type = (reg1 >> 24) & 0xff;
	svb_corner[0] = (reg1 >> 16) & 0xff;	/*ff*/
	svb_corner[1] = (reg1 >> 8) & 0xff;			/*tt*/
	svb_corner[2] = reg1 & 0xff;			/*ss*/

	iounmap(pu32SVBBase);

	if((svb_type != HISI_CHIP_TYPE_FF) && (svb_type != HISI_CHIP_TYPE_TT) && (svb_type != HISI_CHIP_TYPE_SS))    
	{
		svb_type = HISI_CHIP_TYPE_SS;
	}

	/*step 1: select the suitable vmin table*/
	switch(svb_type)
	{
		case HISI_CHIP_TYPE_FF:
			for(i = 0; i < MAX_FREQ_NUM; i++)
			{
				gpu_freq_volt_table[i].voltage = gpu_freq_volt_table[i].voltage_ff;
			}
			break;

		case HISI_CHIP_TYPE_SS:
			for(i = 0; i < MAX_FREQ_NUM; i++)
			{
				gpu_freq_volt_table[i].voltage = gpu_freq_volt_table[i].voltage_ss;
			}
			break;

		default:
			break;
	}

	/*step 2: compute the core vmin*/
	core_vmin = VOLT_MAX - ((svb_corner[svb_type-1] - 1) * PWM_STEP) / PWM_CLASS;

	if((core_vmin > VOLT_MAX) || (core_vmin < VOLT_MIN))
	{
		core_vmin = gpu_freq_volt_table[MAX_FREQ_NUM - 1].voltage;
	}

	/*step 3: correct the gpu vmin if its value smaller than the core vmin*/
	for(i = 0; i < MAX_FREQ_NUM; i++)
	{
		if(gpu_freq_volt_table[i].voltage < core_vmin)
			gpu_freq_volt_table[i].voltage = core_vmin;
	}

	printk("SVB: type = %d, core vmin %dmv\n", svb_type, core_vmin);

	return 0;
}

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
	MALI_DEBUG_PRINT(2, ("Hisi gpu clock reset\n"));
}

/* GPU clock on                                                         */
void hisi_crg_clockon(void)
{
    u32 gpuclock;

    gpuclock = mali_hw_core_register_read(s_hisi_crg, CRG_REG_ADDR_SOFT_RST);

    /* clock on */
    gpuclock = (gpuclock | GPU_CLOCK_ENABLE_MASK);
#if defined(CHIP_TYPE_hi3798cv100) || defined(CHIP_TYPE_hi3796cv100)
	gpuclock = (gpuclock | GPU_CLOCK_PP0_ENABLE_MASK | GPU_CLOCK_PP1_ENABLE_MASK | GPU_CLOCK_PP2_ENABLE_MASK | GPU_CLOCK_PP3_ENABLE_MASK);
#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a) \
   || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3796mv200)
	gpuclock = (gpuclock | GPU_CLOCK_PP0_ENABLE_MASK | GPU_CLOCK_PP1_ENABLE_MASK | GPU_CLOCK_PP2_ENABLE_MASK);    
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
#if defined(CHIP_TYPE_hi3798cv100) || defined(CHIP_TYPE_hi3796cv100)    
	gpuclock = (gpuclock & (~(GPU_CLOCK_PP0_ENABLE_MASK | GPU_CLOCK_PP1_ENABLE_MASK | GPU_CLOCK_PP2_ENABLE_MASK | GPU_CLOCK_PP3_ENABLE_MASK)));
#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a) \
   || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3796mv200)
	gpuclock = (gpuclock & (~(GPU_CLOCK_PP0_ENABLE_MASK | GPU_CLOCK_PP1_ENABLE_MASK | GPU_CLOCK_PP2_ENABLE_MASK)));
#endif

    mali_hw_core_register_write(s_hisi_crg, CRG_REG_ADDR_SOFT_RST, gpuclock);
    MALI_DEBUG_PRINT(2, ("Hisi gpu clock off\n"));
}

void hisi_sec_reset_enable(void)
{
#if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3796mv200)
	int value ;

    mali_hw_core_register_write(s_hisi_sec, SEC_REG_ADDR_IDLES_COUNT, 0x1ff);    

    value = mali_hw_core_register_read(s_hisi_sec, SEC_REG_ADDR_SWITCH);
    value = value | 0x1;

    mali_hw_core_register_write(s_hisi_sec, SEC_REG_ADDR_SWITCH, value);

    value = 0 ;
    while(!value)
    {
        value = mali_hw_core_register_read(s_hisi_sec, SEC_REG_ADDR_STATUS);
    }
#endif
}

void hisi_sec_reset_disable(void)
{
#if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3796mv200)
	int value ;

    mali_hw_core_register_write(s_hisi_sec, SEC_REG_ADDR_IDLES_COUNT, 0x1ff); 

    value = mali_hw_core_register_read(s_hisi_sec, SEC_REG_ADDR_SWITCH);
    value = value & 0xfffffffe;

    mali_hw_core_register_write(s_hisi_sec, SEC_REG_ADDR_SWITCH, value);

    value = 0 ;
    while(!value)
    {
        value = mali_hw_core_register_read(s_hisi_sec, SEC_REG_ADDR_STATUS);
    }
#endif
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

void clk_gpu_switch_freqency_normally(struct mali_clk *clk, int profile)
{
	u32 freqctrl;
	u32 lowpower;
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

	return ;
}

int mali_gpu_set_voltage(unsigned int freq)
{
    int profile = clk_gpu_get_index(freq);

    if (gpu_cur_volt != gpu_freq_volt_table[profile].voltage)
    {
        clk_gpu_set_voltage(gpu_freq_volt_table[profile].voltage);
    }

    return 0;
}

int clk_gpu_set_voltage(unsigned int volt)
{
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
	/*because gpu and the core share voltage control in chip hi3798mv100 */
	return 0;
#else
	unsigned int vmax, vmin, offset, vstep, base;
	unsigned int value;

	if ((volt < VOLT_MIN) || (volt > VOLT_MAX))
	{
		return 0;
	}

	vmax = VOLT_MAX;
	vmin = VOLT_MIN;
	vstep = VOLT_STEP;
	base = VMAX_REG_BASE;
	offset = PWM_REG_OFFSET;

	value = base + ( (vmax - volt)/vstep )*offset  ;

#if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
	/*because core and gpu use the same voltage, avoid to set core voltage frequently*/
	if( mali_hw_core_register_read(s_hisi_pmc, PMC_REG_ADDR_PWM2_DUTY) != value)
	{
		mali_hw_core_register_write(s_hisi_pmc, PMC_REG_ADDR_PWM2_DUTY, value);
	}
#else
	mali_hw_core_register_write(s_hisi_pmc, PMC_REG_ADDR_PWM1_DUTY, value);
#endif

	gpu_cur_volt = volt;
	MALI_DEBUG_PRINT(1, ("Mali DVFS: set voltage = %d\n", volt));

	return 0;
#endif
}

int clk_gpu_set_rate(struct mali_clk *clk, unsigned rate)
{
	int profile = clk_gpu_get_index(rate);

	clk_gpu_switch_freqency_normally(clk, profile) ;

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

#if defined(CHIP_TYPE_hi3798cv100) || defined(CHIP_TYPE_hi3796cv100)

#define SC_SYS_DBG  	0xf8000100
#define SC_CI_ADD0      0xf8b29400
#define SC_CI_ADD1      0xf8b29040
#define SC_CI_ADD2      0xf8a21178
	mali_io_address* addr_DBG = ioremap_nocache(SC_SYS_DBG,0x10);
	mali_io_address* addr0 = ioremap_nocache(SC_CI_ADD0,0x10);
	mali_io_address* addr1 = ioremap_nocache(SC_CI_ADD1,0x10);
	mali_io_address* addr2 = ioremap_nocache(SC_CI_ADD2,0x10);

	iowrite32(0x2, (u8*)addr2);
	if(0 == (ioread32((u8*)addr_DBG) & 0x1))
	{
		iowrite32(0x10, (u8*)addr0);
		iowrite32(0x0, (u8*)addr1);
	}
	iounmap(addr_DBG);
	iounmap(addr0);
	iounmap(addr1);
	iounmap(addr2);

#endif

    pmu_select = mali_hw_core_register_read(s_hisi_pmc, PMC_REG_ADDR_GPU_CTRL);

    /* pmu selection: use mali pmu */
    pmu_select = pmu_select & (~GPU_PMU_BYPASS_MASK);

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
	s_hisi_sec = hisi_sec_get();

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

