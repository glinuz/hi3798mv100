/******************************************************************************
*    Copyright (c) 2009-2012 by Hisi.
*    All rights reserved.
* ***
*    Create by Czy. 2012-03-19
*
******************************************************************************/

#include <config.h>

#define readl(_a)        (*(volatile unsigned int *)(_a))
#define writel(_v, _a)   (*(volatile unsigned int *)(_a) = (_v))

/******************************************************************************/

static inline void delay(unsigned int cnt)
{
	while (cnt--)
		__asm__ __volatile__("nop");
}

/******************************************************************************/

void reg_read_write(volatile unsigned int *reg_addr, unsigned int val, unsigned int mask)
{
	unsigned int v = readl(reg_addr);
	v &= ~mask;
	v |= val;
	writel(v, reg_addr);
}

/******************************************************************************/
#if defined(CONFIG_AVS_SUPPORT) && !defined(CONFIG_PMU_DEVICE)
void set_core_voltage(void)
{
	unsigned int regval, hpm_code, hpm_code_average, delay_count, core_volt_reg_val, corner_type;
	unsigned int max_threshold =readl(REG_BASE_SCTL+REG_SC_GEN6) >>16;
	unsigned int min_threshold = readl(REG_BASE_SCTL+REG_SC_GEN6) & 0xffff;
	unsigned int max_volt = readl(REG_BASE_SCTL+REG_SC_GEN17) >> 16;
	unsigned int min_volt = readl(REG_BASE_SCTL+REG_SC_GEN17) & 0xffff;
	unsigned int mid_volt = readl(REG_BASE_SCTL+REG_SC_GEN18) & 0xffff;

	/* hpm 0             */
	/* clock is 200M, set time division to (200/50 - 1) */
	regval = readl(0xf8a23058);
	regval &= 0xffffffc0;
	regval |= 3;
	writel(regval, 0xf8a23058);

	regval = readl(0xf8a23064);
	regval &= 0x00ffffff;
	regval |= (1 << 24);
	writel(regval, 0xf8a23064);

	/* hpm enable */
	regval = readl(0xf8a23058);
	regval |= (1 << 24);
	writel(regval, 0xf8a23058);

	/* hpm monitor enable */
	regval = readl(0xf8a23058);
	regval |= (1 << 26);
	writel(regval, 0xf8a23058);

	delay(6000000);

	regval = readl(0xf8a2305c);
	hpm_code = (regval & 0x3ff);
	hpm_code_average = hpm_code;
	hpm_code = ((regval >> 12) & 0x3ff);
	hpm_code_average += hpm_code;

	regval = readl(0xf8a23060);
	hpm_code = (regval & 0x3ff);
	hpm_code_average += hpm_code;
	hpm_code = ((regval >> 12) & 0x3ff);
	hpm_code_average += hpm_code;

	hpm_code_average = (hpm_code_average>>2);

	core_volt_reg_val = readl(0xf8a2301c);
	core_volt_reg_val &= 0xffff;
	if (hpm_code_average >= max_threshold) {
		core_volt_reg_val |= max_volt<<16; /* ff chip */
		corner_type = 1;
	} else if (hpm_code_average < min_threshold) {
		core_volt_reg_val |= min_volt<<16; /* ss chip */
		corner_type = 3;
	} else {
		core_volt_reg_val |= mid_volt<<16; /* tt chip */
		corner_type = 2;
	}
	writel(core_volt_reg_val, 0xf8a2301c);

	regval = readl(REG_BASE_SCTL+REG_SC_GEN17);
	regval &= 0xffffff;
	regval |= (corner_type << 24);
	writel(regval, REG_BASE_SCTL+REG_SC_GEN17);

	delay(8000000);

	return;
}
#endif
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

void reset_cpu(unsigned long addr)
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

/******************************************************************************/

