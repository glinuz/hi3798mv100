#include <mali_kbase_avs.h>
#include <types.h>
#include <io.h>
#include <mali_kbase_config_platform.h>

#define KBASE_OPP_COUNT             6

#define KBASE_HISI_GPU_PMC_BASE		0xf8a23000
#define KBASE_HISI_GPU_PMC_SIZE		0x200

static int s_avs_enable = 0;

typedef enum
{
	KBASE_HISI_PMC_HPM1_CTRL			= 0x0068,
	KBASE_HISI_PMC_HPM1_RECORD1			= 0x006c,
	KBASE_HISI_PMC_HPM1_RECORD2			= 0x0070,
	KBASE_HISI_PMC_HPM1_MONITOR			= 0x0074
} KBASE_HISI_PMC;

static volatile u32* g_pu32PMCBase = NULL;


typedef struct FREQ_TABLE
{	
    unsigned long freq;              /* unit: HZ */
    unsigned long volt;              /* unit: uV */
} FREQ_TABLE;

FREQ_TABLE alternative_freq_table_S3[KBASE_OPP_COUNT]=
{    
    {200000000, 900000},
    {300000000, 900000},
    {400000000, 900000},
    {500000000, 900000},
    {600000000, 930000},
    {675000000, 1000000},
};

FREQ_TABLE alternative_freq_table_F3[KBASE_OPP_COUNT]=
{    
    {200000000, 850000},
    {300000000, 850000},
    {400000000, 850000},
    {500000000, 850000}, 
    {600000000, 880000},
    {675000000, 920000},
};

static int kbase_write_pmc_reg(u32 reg, u32 value)
{
	*(g_pu32PMCBase +  (reg >> 2)) = value;
	return 0;
}

static int kbase_read_pmc_reg(u32 reg)
{
	return *(g_pu32PMCBase +  (reg >> 2));
}

/** 
 *
 * Initialize avs.
 * 
 */
int kbase_avs_init(void)
{
    u32 regval;

    g_pu32PMCBase = (volatile u32*)ioremap_nocache(KBASE_HISI_GPU_PMC_BASE,  KBASE_HISI_GPU_PMC_SIZE);

    if(NULL == g_pu32PMCBase)
    {
        printk(KERN_ERR "Init pmc failed\n");
        return -1;
    }


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

int kbase_avs_deinit(void)
{
    if(NULL != g_pu32PMCBase)
    {
        iounmap(g_pu32PMCBase);
        return 0;
    }

    return 0;
}


int kbase_avs_sethpm(int gpufreq)
{
    if(s_avs_enable)
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
    }
    
    return 0;
}

int kbase_avs_start(int gpufreq)
{
#if 0
    if(s_avs_enable)
    {

        u16 hpm_code0, hpm_code1, hpm_code2, hpm_code3, hpm_code_avg;
        u32 hpm_code;
        /* int hpm_delta; */
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
    	    if ((kbase_regulator_get()+ 10) <= kbase_gpll_table[profile].volt)
    	    {
    	    	/* increase 10mv */
    	        kbase_regulator_set(kbase_regulator_get()+ 10);
    	    }
    	}
    	else if (hpm_delta >= 0xa)
    	{
    		/* The voltage should not be decrease than GPU_MAX_VOLTAGE_DEC */
    	   	if (((kbase_regulator_get() - 10) >= (kbase_gpll_table[profile].volt - KBASE_MAX_VOLTAGE_DEC))&&((kbase_regulator_get() - 10) >= KBASE_HISI_GPU_VOLT_MIN))
    	    {
    	    	/* decrease 10mv */
    	        kbase_regulator_set(kbase_regulator_get()- 10);
    	    }
    	}

    }
#endif
        return 0;
}


int kbase_avs_enable(int enable)
{
    s_avs_enable = enable;
    return 0;
}

int kbase_avs_status()
{
    return s_avs_enable;
}

unsigned long kbase_svb_reset_voltage(unsigned long freq, unsigned long voltage)
{
#if defined(CHIP_TYPE_hi3798cv200)
    int i;
	int svb_type = kbase_svb_identify();

    /* SVB Type S3 */
    if(KBASE_CHIP_TYPE_SS == svb_type)
    {
        for(i = 0; i < KBASE_OPP_COUNT; i++) 
        {
            if(freq == alternative_freq_table_S3[i].freq)
            {
                voltage = alternative_freq_table_S3[i].volt;
                return voltage;
            }
        }
    }

    /* SVB Type F3 */
    if(KBASE_CHIP_TYPE_FF == svb_type)
    {
        for(i = 0; i < KBASE_OPP_COUNT; i++) 
        {
            if(freq == alternative_freq_table_F3[i].freq)
            {
                voltage = alternative_freq_table_F3[i].volt;
                return voltage;
            }
        }
    }
#endif
    return voltage;
}
