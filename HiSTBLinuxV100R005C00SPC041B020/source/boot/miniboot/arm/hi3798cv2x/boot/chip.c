/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#include <config.h>

#define dsb() __asm__ __volatile__ ("dsb" : : : "memory")

#define readl(_a)        ({ unsigned int __v = (*(volatile unsigned int *)(_a)); dsb(); __v; })
#define writel(_v, _a)   ({  dsb(); (*(volatile unsigned int *)(_a) = (_v)); })

/******************************************************************************/

static inline void delay(unsigned int cnt)
{
	while (cnt--)
		__asm__ __volatile__("nop");
}

/******************************************************************************/

void reg_read_write(volatile unsigned int reg_addr, unsigned int val, unsigned int mask)
{
	unsigned int v = readl(reg_addr);
	v &= ~mask;
	v |= val;
	writel(v, reg_addr);
}

/******************************************************************************/
//TODO: XXX
//#if defined(CONFIG_AVS_SUPPORT) && !defined(CONFIG_PMU_DEVICE)
void set_core_voltage(void)
{
	unsigned int regval, hpm_code, hpm_code_average, volt_reg_val;
	unsigned int max_threshold =readl(REG_BASE_SCTL+REG_SC_GEN6) >>16;
	unsigned int min_threshold = readl(REG_BASE_SCTL+REG_SC_GEN6) & 0xffff;
	unsigned int max_volt = readl(REG_BASE_SCTL+REG_SC_GEN17) >> 16;
	unsigned int min_volt = readl(REG_BASE_SCTL+REG_SC_GEN17) & 0xffff;
	unsigned int middle_volt = readl(REG_BASE_SCTL+REG_SC_GEN18) & 0xffff;
	/* set voltage to 900mv */
	writel(0x6500a1, 0xf8a2301c);
	delay(6000000);

	/* hpm 0             */
	/* clock is 200M, set time division to (200/50 - 1) */
	regval  = readl(0xf8a23058);
	regval &= 0xffffffc0;
	regval |= 3;
	writel(regval, 0xf8a23058);

	/* set monitor period to 2ms */
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

	volt_reg_val  = readl(0xf8a2301c);
	volt_reg_val &= 0xffff;
	if (hpm_code_average >= max_threshold)
	{
		volt_reg_val |= max_volt << 16; /* ff chip */
	}
	else if (hpm_code_average < min_threshold)
	{
		volt_reg_val |= min_volt << 16; /* ss chip */
	}
	else
	{
		volt_reg_val |= middle_volt << 16; /* tt chip */
	}

	writel(volt_reg_val, 0xf8a2301c);
	delay(8000000);

	return;
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
void check_bootfromsd(void)
{
	if (BOOT_FROM_SD == get_boot_mode()) {
		reg_read_write(REG_IOSHARE_EBI_DQ7, 0x0, 0x3);
		reg_read_write(REG_IOSHARE_EBI_DQ6, 0x0, 0x1);
		reg_read_write(REG_IOSHARE_EBI_DQ5, 0x0, 0x1);
		reg_read_write(REG_IOSHARE_EBI_DQ4, 0x0, 0x1);
	}
}

/******************************************************************************/
int get_board_type(void)
{
	static int boardtype = -1;
	unsigned int regval;

	if (boardtype != -1)
		return boardtype;

	regval = readl(REG_BASE_SCTL + REG_SC_GEN28) & BOARD_TYPE_MASK;
	switch(regval) {
	case 0:
		boardtype = BOARD_TYPE_0;
		break;
	case 1:
		boardtype = BOARD_TYPE_1;
		break;
	case 2:
		boardtype = BOARD_TYPE_2;
		break;
	case 3:
	case 4:
	case 5:
		boardtype = BOARD_TYPE_RESERVED;
		break;
	default:
		boardtype = BOARD_TYPE_INVALID;
		break;
	}

	return boardtype;
}
/******************************************************************************/

unsigned int get_mmc_io_voltage(void)
{
	unsigned int voltage = 0;

	voltage = readl(REG_BASE_SCTL+REG_SC_GEN29);
	voltage &= EMMC_IO_VOLTAGE_MASK;

	return voltage;
}

/******************************************************************************/
