/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-02-27
 *
******************************************************************************/

#include <common.h>
#include <command.h>
#include <asm/system.h>
#include <asm/cache.h>
#include <asm/io.h>
#include <config.h>
#include <asm/sizes.h>
#include <asm/cpu_info.h>
#ifndef CONFIG_L2_OFF
#include <asm/arch/sys_proto.h>
#endif

/******************************************************************************/

int cleanup_before_linux(void)
{
	unsigned int i;

	/*
	 * this function is called just before we call linux
	 * it prepares the processor for linux
	 *
	 * we turn off caches etc ...
	 */
	disable_interrupts();

	/* turn off I/D-cache */
	icache_disable();
	dcache_disable();

	/* invalidate I-cache */
	asm ("mcr p15, 0, %0, c7, c5, 0": :"r" (0));

	i = 0;
	/* mem barrier to sync up things */
	asm("mcr p15, 0, %0, c7, c10, 4": :"r"(i));


	return 0;
}
/******************************************************************************/

static int hi3798mx_boot_media(char **media)
{
	int boot_media;
	char *boot_media_str = "Unknown";
	int boot_sel;

	/*read bootsel_ctrl from OTP controller*/
	boot_sel = readl(REG_OTP_CTRL + REG_OTP_BOOT_SEL);
	boot_sel = (boot_sel & OTP_BOOT_SEL_MASK)  >> OTP_BOOT_SEL_OFFSET;

	if (0 == boot_sel) {
		/* read from pin */
		boot_media = readl(REG_BASE_PERI_CTRL + REG_START_MODE);
		boot_media = ((boot_media >> NORMAL_BOOTMODE_OFFSET)
		& NORMAL_BOOTMODE_MASK);
	} else {
		/* read from OTP controller */
		boot_media = readl(REG_OTP_CTRL + REG_OTP_BOOT_MODE);
		boot_media = (boot_media & NORMAL_BOOTMODE_MASK);
	}

	switch (boot_media) {
	case 0x0:
		boot_media     = BOOT_MEDIA_SPIFLASH;
		boot_media_str = "SPI Flash";
		break;
	case 0x1:
		boot_media     = BOOT_MEDIA_NAND;
		boot_media_str = "NAND";
		break;
	case 0x2:
		boot_media     = BOOT_MEDIA_SD;
		boot_media_str = "SD";
		break;
	case 0x3:
		boot_media     = BOOT_MEDIA_EMMC;
		boot_media_str = "eMMC";
		break;
	}


	if (media)
		*media = boot_media_str;

	return boot_media;
}
/*****************************************************************************/

static void get_hi3798mx_clock(unsigned int *cpu, unsigned int *timer)
{
	unsigned long FREF = HI3798MX_OSC_FREQ;

	if (cpu)
		(*cpu) = 0;
	if (timer)
		(*timer) = FREF;
}
/*****************************************************************************/
/* return CHIPSET_CATYPE_NORMAL - normal, CHIPSET_CATYPE_CA - secure */
static int hi3798mx_get_ca_type(void)
{
	unsigned int id_word;

	id_word = readl(REG_OTP_ID_WORD);

	if (OTP_ID_WORD_VALUE_SECURE == id_word)
		return CHIPSET_CATYPE_CA;
	else
		return CHIPSET_CATYPE_NORMAL;
}
/*****************************************************************************/

struct cpu_info_t cpu_info[] = {
	{
		.name   = "Unknown",
		.chipid = 0,
		.chipid_mask = ~0,
		.devs = 0,
		.max_ddr_size = 0,
		.boot_media = NULL,
		.get_clock  = NULL,
		.get_cpu_version = NULL,
		.get_ca_type = NULL,
	},
	{
		.name   = "Hi3798Mv100_a",
		.chipid = _HI3798M_V100_A,
		.chipid_mask = _HI3798M_V100_A_MASK,
		.devs = DEV_HINFC610 | DEV_EMMCV200,
#ifdef CONFIG_HI3798MX_FPGA
		.max_ddr_size = SZ_1G,
#else
		.max_ddr_size = (SZ_3G + SZ_750M),
#endif
		.boot_media = hi3798mx_boot_media,
		.get_clock  = get_hi3798mx_clock,
		.get_cpu_version = NULL,
		.get_ca_type = hi3798mx_get_ca_type,
	},
	{
		.name   = "Hi3798Mv100",
		.chipid = _HI3798M_V100,
		.chipid_mask = _HI3798M_V100_MASK,
		.devs = DEV_HINFC610 | DEV_EMMCV200,
#ifdef CONFIG_HI3798MX_FPGA
		.max_ddr_size = SZ_1G,
#else
		.max_ddr_size = (SZ_3G + SZ_750M),
#endif
		.boot_media = hi3798mx_boot_media,
		.get_clock  = get_hi3798mx_clock,
		.get_cpu_version = NULL,
		.get_ca_type = hi3798mx_get_ca_type,
	},
	{
		.name   = "Hi3796Mv100",
		.chipid = _HI3796M_V100,
		.chipid_mask = _HI3798M_V100_MASK,
		.devs = DEV_HINFC610 | DEV_EMMCV200,
		.max_ddr_size = (SZ_3G + SZ_750M),
		.boot_media = hi3798mx_boot_media,
		.get_clock  = get_hi3798mx_clock,
		.get_cpu_version = NULL,
		.get_ca_type = hi3798mx_get_ca_type,
	},
	{
		.name   = "Hi3716Dv100",
		.chipid = _HI3716D_V100,
		.chipid_mask = _HI3798M_V100_MASK,
		.devs = DEV_HINFC610 | DEV_EMMCV200,
		.max_ddr_size = (SZ_3G + SZ_750M),
		.boot_media = hi3798mx_boot_media,
		.get_clock  = get_hi3798mx_clock,
		.get_cpu_version = NULL,
		.get_ca_type = hi3798mx_get_ca_type,
	},
	{0},
};
/*****************************************************************************/

int get_cpuno(void)
{
	int cpuno = 0;
	asm("mrc	p15, 0, %0, c0, c0, 5" :"=r"(cpuno) : :"memory");
	return (cpuno & 0x03);
}
/*****************************************************************************/
