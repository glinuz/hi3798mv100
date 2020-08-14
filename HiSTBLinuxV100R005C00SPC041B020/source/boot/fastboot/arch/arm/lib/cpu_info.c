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
#include <asm/cpu_info.h>

/*****************************************************************************/

extern struct cpu_info_t cpu_info[];

extern long long get_chipid_reg(void);

extern unsigned int get_ca_vendor_reg(void);

#ifndef CONFIG_SUPPORT_CA_RELEASE
extern void uart_early_puts(const char *s);
#else
#define uart_early_puts(_s)
#endif
/*****************************************************************************/

static struct cpu_info_t *get_cpuinfo(void)
{
	static struct cpu_info_t *info = NULL;
	long long chipid = get_chipid_reg();

	if (info)
		return info;

	for (info = cpu_info; info->name; info++) {
		if ((info->chipid & info->chipid_mask)
			== (chipid & info->chipid_mask))
			return info;
	}
	uart_early_puts("\r\nUnkown ChipID!\r\n");
	while(1);
}
/*****************************************************************************/

long long get_chipid(void)
{
	static long long chipid = 0;

	if (!chipid) {
		struct cpu_info_t *info = get_cpuinfo();
		chipid = (info->chipid & info->chipid_mask);
	}

	return chipid;
}
/******************************************************************************/

int get_bootmedia(char **media, void *arg)
{
	static char *media_name = NULL;
	static unsigned int media_type = 0;

	if (!media_name)
		media_type = get_cpuinfo()->boot_media(&media_name);
	if (media)
		(*media) = media_name;

	return media_type;
}
/******************************************************************************/
/* sdk use it */
int get_boot_media(void)
{
	return get_bootmedia(NULL, NULL);
}
/*****************************************************************************/

unsigned int get_timer_clock(void)
{
	unsigned int timer_clk = 0;
	if (get_cpuinfo()->get_clock)
		get_cpuinfo()->get_clock(NULL, &timer_clk);
	return timer_clk;
}
/*****************************************************************************/

const char *get_cpu_name(void)
{
	return get_cpuinfo()->name;
}
/*****************************************************************************/

unsigned int get_max_ddr_size(void)
{
	return get_cpuinfo()->max_ddr_size;
}
/******************************************************************************/

unsigned int get_ca_vendor(void)
{
	static unsigned long ca_vendor = 0;
	if (ca_vendor)
		return ca_vendor;
	ca_vendor = get_ca_vendor_reg();
	return ca_vendor;
}
/******************************************************************************/

const int get_cpu_version(char **version)
{
	static char *cpu_ver_str = NULL;
	static int cpu_ver = 0;

	if (cpu_ver_str)
		goto exit;

	if (get_cpuinfo()->get_cpu_version)
		cpu_ver = get_cpuinfo()->get_cpu_version(&cpu_ver_str);
	else
		cpu_ver_str = "";

exit:
	if (version)
		(*version) = cpu_ver_str;

	return cpu_ver;
}
/*****************************************************************************/

int get_cpudevs(void)
{
	static unsigned int devs = 0;
	if (!devs)
		devs = get_cpuinfo()->devs;
	return (int) devs;
}
/*****************************************************************************/
/* check if chipset is secure chipset or not. */
int get_ca_type(void)
{
	static unsigned int ca_type = CHIPSET_CATYPE_INVALID;

	if (CHIPSET_CATYPE_INVALID != ca_type)
		goto exit;

	if (get_cpuinfo()->get_ca_type)
		ca_type = get_cpuinfo()->get_ca_type();
	else
		ca_type = CHIPSET_CATYPE_NORMAL;

exit:
	return ca_type;
}

