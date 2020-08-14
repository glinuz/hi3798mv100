/* chip.c - chipset relevant */

#include <config.h>
#include <platform.h>
#include <aux_common.h>

typedef unsigned char u8;

extern void ddr_set_sref(unsigned int val);
/******************************************************************************/

struct lsadc_volt_region {
	u8 vmin;
	u8 vmax;
	u8 vaver;
};

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static struct lsadc_volt_region lsadc_volt_regions[] = {
	{217, 255},
	{174, 208},
	{132, 166},
	{89,  123},
	{47,  81},
	{0,   38},
};
/******************************************************************************/

static void get_lsadc_volt_region(struct lsadc_volt_region *region)
{
	int ix;
	unsigned int regval;
	u8 lsadc_val;
	unsigned int sum;

	/* enable LSADC module, use INB. */
	regval = readl(REG_BASE_SCTL + REG_SC_CLKGATE_SRST_CTRL);
	regval |= REG_SC_LSADC_CKEN;
	regval &= ~REG_SC_LSADC_SRST_REQ;
	writel(regval, REG_BASE_SCTL + REG_SC_CLKGATE_SRST_CTRL);

	regval = readl(REG_BASE_SCTL + REG_SC_LSADC_CTRL);
	regval &= ~REG_SC_LSADC_CTRL_PD_CTRL;
	writel(regval, REG_BASE_SCTL + REG_SC_LSADC_CTRL);

	regval = readl(REG_BASE_SCTL + REG_SC_LSADC_CTRL);
	regval &= ~REG_SC_LSADC_CTRL_RESET;
	writel(regval, REG_BASE_SCTL + REG_SC_LSADC_CTRL);
	delay(12000);

	regval = readl(REG_BASE_SCTL + REG_SC_LSADC_CTRL);
	regval |= REG_SC_LSADC_CTRL_EN_B;
	writel(regval, REG_BASE_SCTL + REG_SC_LSADC_CTRL);

	delay(12000);

	lsadc_val = (u8)((readl(REG_BASE_SCTL + REG_SC_LSADC_DATA)
				& REG_SC_LSADC_DATA_B_MASK) >> 8);

	sum = region->vmin = region->vmax = lsadc_val;

	for (ix = 1; ix < 16; ix++) {
		/* delay 100us. cpu freq is 1.2G; "nop" equals 1 cpu cycle */
		delay(120000);

		lsadc_val = (u8)((readl(REG_BASE_SCTL + REG_SC_LSADC_DATA)
					& REG_SC_LSADC_DATA_B_MASK) >> 8);

		sum += lsadc_val;

		if (lsadc_val < region->vmin)
			region->vmin = lsadc_val;
		else if (lsadc_val > region->vmax)
			region->vmax = lsadc_val;
	}

	/* get average of lsadc volt. */
	region->vaver = sum >> 4;

}
/******************************************************************************/

static int get_lsadc_volt_region_index(struct lsadc_volt_region *region)
{
	int ix;

	for (ix = 0; ix < ARRAY_SIZE(lsadc_volt_regions); ix++) {
		if (lsadc_volt_regions[ix].vmin <= region->vaver &&
		    region->vaver <= lsadc_volt_regions[ix].vmax)
			return ix;
	}

	return -1;
}
/******************************************************************************/

void show_lsadc_volt_region(void)
{
	int ix;

	serial_puts("\nlsadc valid voltage region:");

	for (ix = 0; ix < ARRAY_SIZE(lsadc_volt_regions); ix++) {
		serial_puts("\n");
		serial_put_hex(lsadc_volt_regions[ix].vmin);
		serial_puts(", ");
		serial_put_hex(lsadc_volt_regions[ix].vmax);
		serial_puts(", ");
		serial_put_hex(lsadc_volt_regions[ix].vaver);
	}
}
/******************************************************************************/

int get_board_type(void)
{
	int type = -1;
	unsigned int regval;
	struct lsadc_volt_region region;

	do {
		get_lsadc_volt_region(&region);

		serial_puts("\nlsadc voltage min: ");
		serial_put_hex(region.vmin);
		serial_puts(", max: ");
		serial_put_hex(region.vmax);
		serial_puts(", aver: ");
		serial_put_hex(region.vaver);

		type = get_lsadc_volt_region_index(&region);

		if (type == -1) {
			serial_puts(", invalid voltage region.");
			show_lsadc_volt_region();
		} else {
			serial_puts(", index: ");
			serial_put_hex(type);
		}

	} while (type == -1);

	/* save board type. */
	regval = readl(REG_BASE_SCTL + REG_SC_GEN28);
	regval &= ~BOARD_TYPE_MASK;
	regval |= type;
	writel(regval ,REG_BASE_SCTL + REG_SC_GEN28);

	serial_puts("\n\n");

	return type;
}
/******************************************************************************/
#define PMC_CORE_PWM   0xf8a2301c
#define PMC_GPU_PWM    0xf8a23020
#define PMC_CORE_SET1  0xf8a23058
#define PMC_CORE_DATA1 0xf8a2305c
#define PMC_CORE_DATA2 0xf8a23060
#define PMC_CORE_SET2  0xf8a23064
//#if defined(CONFIG_AVS_SUPPORT) && !defined(CONFIG_PMU_DEVICE)
void set_core_voltage(void)
{
	unsigned int regval, hpm_code, hpm_code_average, volt_reg_val, corner_type;
	unsigned int max_threshold =readl(REG_BASE_SCTL+REG_SC_GEN6) >>16;
	unsigned int min_threshold = readl(REG_BASE_SCTL+REG_SC_GEN6) & 0xffff;
	unsigned int max_volt = readl(REG_BASE_SCTL+REG_SC_GEN17) >> 16;
	unsigned int min_volt = readl(REG_BASE_SCTL+REG_SC_GEN17) & 0xffff;
	unsigned int middle_volt = readl(REG_BASE_SCTL+REG_SC_GEN18) & 0xffff;

	/* hpm 0	     */
	/* clock is 200M, set time division to (200/50 - 1) */
	regval	= readl(PMC_CORE_SET1);
	regval &= 0xffffffc0;
	regval |= 3;
	writel(regval, PMC_CORE_SET1);

	/* set monitor period to 2ms */
	regval = readl(PMC_CORE_SET2);
	regval &= 0x00ffffff;
	regval |= (1 << 24);
	writel(regval, PMC_CORE_SET2);

	/* hpm enable */
	regval = readl(PMC_CORE_SET1);
	regval |= (1 << 24);
	writel(regval, PMC_CORE_SET1);

	/* hpm monitor enable */
	regval = readl(PMC_CORE_SET1);
	regval |= (1 << 26);
	writel(regval, PMC_CORE_SET1);

	delay(6000000);

	regval = readl(PMC_CORE_DATA1);
	hpm_code = (regval & 0x3ff);
	hpm_code_average = hpm_code;
	hpm_code = ((regval >> 12) & 0x3ff);
	hpm_code_average += hpm_code;

	regval = readl(PMC_CORE_DATA2);
	hpm_code = (regval & 0x3ff);
	hpm_code_average += hpm_code;
	hpm_code = ((regval >> 12) & 0x3ff);
	hpm_code_average += hpm_code;

	hpm_code_average = (hpm_code_average>>2);

	volt_reg_val  = readl(PMC_CORE_PWM);
	volt_reg_val &= 0xffff;
	if (hpm_code_average >= max_threshold) {
		volt_reg_val |= max_volt << 16; /* ff chip */
		corner_type = 1;
	} else if (hpm_code_average < min_threshold) {
		volt_reg_val |= min_volt << 16; /* ss chip */
		corner_type = 3;
	} else {
		volt_reg_val |= middle_volt << 16; /* tt chip */
		corner_type = 2;
	}
	writel(volt_reg_val, PMC_CORE_PWM);

	/* add for core&gpu controlled by one power */
	writel(volt_reg_val, PMC_GPU_PWM);

	regval = readl(REG_BASE_SCTL+REG_SC_GEN17);
	regval &= 0xffffff;
	regval |= (corner_type << 24);
	writel(regval, REG_BASE_SCTL+REG_SC_GEN17);
	delay(8000000);
}
//#endif
/******************************************************************************/

long long get_chipid_reg(void)
{
	long long chipid = 0;
	long long val = 0;

	chipid = (long long)readl(REG_BASE_SCTL + REG_SC_SYSID0);
	val = (long long)readl(REG_BASE_PERI_CTRL + REG_PERI_SOC_FUSE);
	chipid = ((val & (0x1F << 16)) << 16) | (chipid & 0xFFFFFFFF);

	return chipid;
}
/******************************************************************************/

unsigned int get_ca_vendor_reg(void)
{
	unsigned long ca_vendor = 0;

	// TODO: XXXX

	return ca_vendor;
}

/******************************************************************************/

void reset_cpu(void)
{
	unsigned int val;
	/* enable the wdg0 crg clock */
	val = readl(REG_BASE_CRG + REG_PERI_CRG94);
	val &= ~(1<<4);
	val |= 1;
	writel(val, REG_BASE_CRG + REG_PERI_CRG94);
	delay(1000);

	/* unclock wdg */
	writel(0x1ACCE551, REG_BASE_WDG0 + 0x0C00);
	/* wdg load value */
	writel(0x00000100, REG_BASE_WDG0 + 0x0000);
	/* bit0: int enable bit1: reboot enable */
	writel(0x00000003, REG_BASE_WDG0 + 0x0008);

	while (1)
		;
}
/******************************************************************************/

int get_boot_mode(void)
{
	int boot_media;

	/* read from pin */
	boot_media = readl(REG_BASE_PERI_CTRL + REG_START_MODE);
	boot_media = ((boot_media >> NORMAL_BOOTMODE_OFFSET)
		& NORMAL_BOOTMODE_MASK);

	return boot_media;
}
#ifdef CONFIG_EN_JTAG
/*****************************************************************************/
void set_jtag_pinmux(void)
{
	writel(0x130, 0xf8a210ec);
	writel(0x130, 0xf8a210f0);
	writel(0x130, 0xf8a210f4);
	writel(0x130, 0xf8a210f8);
	writel(0x130, 0xf8a210fc);
	writel(0x130, 0xf8a21100);
}
#endif

/******************************************************************************/

void start_ddr_hw_training(void)
{
	unsigned int initctrl_rank0, initctrl_rank1, rank_num;

	/* Read rank0 HW DDR training value */
	initctrl_rank0 = readl(REG_BASE_SCTL + REG_SC_GEN32);
	/* Read rank1 HW DDR training value */
	initctrl_rank1 = readl(REG_BASE_SCTL + REG_SC_GEN33);
	/* 0: one rank, 1: two rank */
	rank_num = (readl(REG_DDR_DMC0_CFG_DDRMODE) >> 20) & 0x3;

	if (initctrl_rank0 || (initctrl_rank1 && rank_num)) {
		unsigned int timing;
		unsigned int timing_backup;
		unsigned int rank_ctrl;

		step(11);
		/* Save the Auto-self refresh timing */
		timing = readl(REG_DDR_DMC0_CFG_TIMING2);
		timing_backup = timing & 0x7FF;
		/* Set Auto-self refresh timing to 0 */
		step(12);
		writel((timing & 0xFFFFF800), REG_DDR_DMC0_CFG_TIMING2);
		step(13);

		rank_ctrl = readl(REG_DDR_PHY0_TRAINCTRL);
		if (initctrl_rank0) {
			/* set bit[0:3] = 0 to training rank0 */
			writel(rank_ctrl & 0xfffffff0, REG_DDR_PHY0_TRAINCTRL);
			/* Start HW DDR training */
			writel(initctrl_rank0, REG_DDR_PHY0_INITCTRL);
			/* wait HW DDR training finish */
			do {
				initctrl_rank0 = readl(REG_DDR_PHY0_INITCTRL) & 0x7FFFF;
				step(14);
			} while(initctrl_rank0);
		}

		if (initctrl_rank1 && rank_num) {
			/* set bit[0:3] = 1 to training rank1 */
			writel((rank_ctrl & 0xfffffff0) | 0x1, REG_DDR_PHY0_TRAINCTRL);
			/* Start HW DDR training */
			writel(initctrl_rank1, REG_DDR_PHY0_INITCTRL);
			/* wait HW DDR training finish */
			do {
				initctrl_rank1 = readl(REG_DDR_PHY0_INITCTRL) & 0x7FFFF;
				step(15);
			} while(initctrl_rank1);
		}
		writel(rank_ctrl, REG_DDR_PHY0_TRAINCTRL); /* restore */

		/* Exit Auto-self refresh */
		ddr_set_sref(0x2);
		step(16);

		/* Restore the Auto-self refresh timing */
		timing = readl(REG_DDR_DMC0_CFG_TIMING2);
		step(17);
		timing = (timing & 0xFFFFF800) | timing_backup;
		writel(timing, REG_DDR_DMC0_CFG_TIMING2);
		step(18);
	}

	/* Clear fifo */
	initctrl_rank0 = readl(REG_DDR_PHY0_INITCTRL);
	step(19);
	initctrl_rank0 = initctrl_rank0 | (1<<15);
	writel(initctrl_rank0, REG_DDR_PHY0_INITCTRL);
	step(20);
	initctrl_rank0 = readl(REG_DDR_PHY0_INITCTRL);
	initctrl_rank0 = initctrl_rank0 & ~(1<<15);
	writel(initctrl_rank0, REG_DDR_PHY0_INITCTRL);
	step(21);
}
/*****************************************************************************/

unsigned int boottype2modtype(unsigned int boot_type)
{
	int i, size;
	unsigned int regval;
	unsigned int array[][2] = { 
		{BOOT_MODE_SPI, MOD_TYPE_SPI},
		{BOOT_MODE_NAND, MOD_TYPE_NAND},
		{BOOT_MODE_EMMC, MOD_TYPE_EMMC},
		{BOOT_MODE_SD, MOD_TYPE_SD},
	}; 

	size = sizeof(array) / sizeof((array)[0]);

	for (i = 0; i < size; i++) {
		if (array[i][0] == boot_type)
			return array[i][1];
	}

	return MOD_TYPE_NORMAL;
}
/*****************************************************************************/
