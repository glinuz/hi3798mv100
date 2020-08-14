/*
 * (C) Copyright 2002-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * To match the U-Boot user interface on ARM platforms to the U-Boot
 * standard (as on PPC platforms), some messages with debug character
 * are removed from the default U-Boot build.
 *
 * Define DEBUG here if you want additional info as shown below
 * printed upon startup:
 *
 * U-Boot code: 00F00000 -> 00F3C774  BSS: -> 00FC3274
 * IRQ Stack: 00ebff7c
 * FIQ Stack: 00ebef7c
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <stdio_dev.h>
#include <timestamp.h>
#include <version.h>
#include <net.h>
#include <serial.h>
#include <nand.h>
#include <onenand_uboot.h>
#include <spi_flash.h>
#include <mmc.h>
#include <boot/customer.h>
#include <asm/io.h>
#include <environment.h>
#include <ethcfg.h>

#ifdef CONFIG_BITBANGMII
#include <miiphy.h>
#endif

#ifdef CONFIG_DRIVER_SMC91111
#include "../drivers/net/smc91111.h"
#endif
#ifdef CONFIG_DRIVER_LAN91C96
#include "../drivers/net/lan91c96.h"
#endif

DECLARE_GLOBAL_DATA_PTR;

ulong monitor_flash_len;

#ifdef CONFIG_HAS_DATAFLASH
extern int  AT91F_DataflashInit(void);
extern void dataflash_print_info(void);
#endif

#ifndef CONFIG_IDENT_STRING
#define CONFIG_IDENT_STRING ""
#endif

#ifndef CONFIG_SUPPORT_CA_RELEASE
const char version_string[] =
	U_BOOT_VERSION" (" U_BOOT_DATE " - " U_BOOT_TIME ")"CONFIG_IDENT_STRING;
#else
const char version_string[] = "";
#endif

#ifdef CONFIG_DRIVER_RTL8019
extern void rtl8019_get_enetaddr (uchar * addr);
#endif

#if defined(CONFIG_HARD_I2C) || \
    defined(CONFIG_SOFT_I2C)
#include <i2c.h>
#endif

#ifdef CONFIG_CMD_NAND
extern int nand_rr_param_init(void);
#endif

#ifdef CONFIG_GENERIC_MMC
extern int mmc_flash_init(void);
#endif

#ifdef CONFIG_DDR_TRAINING
extern int check_ddr_training(void);
#endif /* CONFIG_DDR_TRAINING */

#ifdef CONFIG_PRODUCT_WITH_BOOT
extern int fastapp_entry(int argc, char *argv[]);
extern int product_init(void);
#endif

extern int reserve_mem_init(void);
extern int show_reserve_mem(void);
extern void init_reg2(void);
/*
 * Phy address should pass to kernel, even if u-boot is not support net driver
 * The value priority: (The left side config will replace the right side config)
 * Environment > compile config > uboot default value
 *     Environment           - setenv "phyaddr"
 *     Compile config        - HISFV_PHY_U
 *     u-boot default value  - HISFV_DEFAULT_PHY_U
 */
unsigned char U_PHY_ADDR = HISFV_DEFAULT_PHY_U;
unsigned char D_PHY_ADDR = HISFV_DEFAULT_PHY_D;


#if defined(CONFIG_ARM_DCC) && !defined(CONFIG_BAUDRATE)
#define CONFIG_BAUDRATE 115200
#endif
static int init_baudrate (void)
{
	char tmp[64];	/* long enough for environment variables */
	int i = getenv_r ("baudrate", tmp, sizeof (tmp));
	gd->bd->bi_baudrate = gd->baudrate = (i > 0)
			? (int) simple_strtoul (tmp, NULL, 10)
			: CONFIG_BAUDRATE;

	return (0);
}

static int display_banner (void)
{
	printf ("\n\n%s\n\n", version_string);
	return (0);
}

#ifndef CONFIG_SYS_NO_FLASH
static void display_flash_config (ulong size)
{
	puts ("Flash: ");
	print_size (size, "\n");
}
#endif /* CONFIG_SYS_NO_FLASH */

/*
 * Breathe some life into the board...
 *
 * Initialize a serial port as console, and carry out some hardware
 * tests.
 *
 * The first part of initialization is running from Flash memory;
 * its main purpose is to initialize the RAM so that we
 * can relocate the monitor code to RAM.
 */

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependent #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */
typedef int (init_fnc_t) (void);

int print_cpuinfo (void);

init_fnc_t *init_sequence[] = {
#if defined(CONFIG_ARCH_CPU_INIT)
	arch_cpu_init,		/* basic arch cpu dependent setup */
#endif
	board_init,		/* basic board dependent setup */
#if defined(CONFIG_USE_IRQ)
	interrupt_init,		/* set up exceptions */
#endif
	timer_init,		/* initialize timer */
#ifdef CONFIG_FSL_ESDHC
	get_clocks,
#endif
	env_init,		/* initialize environment */
	init_baudrate,		/* initialze baudrate settings */
	serial_init,		/* serial communications setup */
	console_init_f,		/* stage 1 init of console */
	display_banner,		/* say that we are here */
#if defined(CONFIG_DISPLAY_CPUINFO)
	print_cpuinfo,		/* display cpu info (and speed) */
#endif
#if defined(CONFIG_DISPLAY_BOARDINFO)
	checkboard,		/* display board info */
#endif
	dram_init,		/* configure available RAM banks */
	arch_usb_init,
	NULL,
};

static void show_boot_env(void)
{
	int env_media;

	printf("\nBoot Env on %s\n", env_get_media(&env_media));
	printf("    Env Offset:          0x%08X\n", CONFIG_ENV_ADDR);
#ifdef CONFIG_ENV_BACKUP
	printf("    Backup Env Offset:   0x%08X\n", CONFIG_ENV_BACKUP_ADDR);
#endif /* CONFIG_ENV_BACKUP */
	printf("    Env Size:            0x%08X\n", CONFIG_ENV_SIZE);
	printf("    Env Range:           0x%08X\n", CONFIG_ENV_RANGE);

	/* check if env addr is alignment block size */
#if defined(CONFIG_GENERIC_SF)
	if ((env_media == BOOT_MEDIA_SPIFLASH) &&
	    ((get_spiflash_info()->erasesize - 1) & CONFIG_ENV_OFFSET))
		printf("*** Warning - Env offset is NOT aligned to SPI Flash "
		       "block size, environment value is read only.\n\n");
#endif /* CONFIG_GENERIC_SF */

#if defined(CONFIG_GENERIC_NAND)
	if (((env_media == BOOT_MEDIA_NAND) || (env_media == BOOT_MEDIA_SPI_NAND)) &&
	    ((get_nand_info()->erasesize - 1) & CONFIG_ENV_OFFSET))
		printf("*** Warning - Env offset is NOT aligned to NAND Flash "
		       "block size, environment value is read only.\n\n");
#endif /* CONFIG_GENERIC_SF */
}

static void bootargs_prepare(void)
{
#ifdef CONFIG_BOOTARGS_512M
	{
		char *bootargs = getenv("bootargs_512M");
		if (!bootargs)
			setenv("bootargs_512M", CONFIG_BOOTARGS_512M);
	}
#endif /* CONFIG_BOOTARGS_512M */

#ifdef CONFIG_BOOTARGS_1G
	{
		char *bootargs = getenv("bootargs_1G");
		if (!bootargs)
			setenv("bootargs_1G", CONFIG_BOOTARGS_1G);
	}
#endif /* CONFIG_BOOTARGS_1G */

#ifdef CONFIG_BOOTARGS_2G
	{
		char *bootargs = getenv("bootargs_2G");
		if (!bootargs)
			setenv("bootargs_2G", CONFIG_BOOTARGS_2G);
	}
#endif /* CONFIG_BOOTARGS_2G */
}

#ifndef CONFIG_BOOT_SIMULATE
#define step(x)
#endif

void start_armboot (void)
{
	init_fnc_t **init_fnc_ptr;
	char *s;
#if defined(CONFIG_VFD) || defined(CONFIG_LCD)
	unsigned long addr;
#endif
	step(81);

#ifdef CONFIG_ARCH_S5
	/* Initilize reg2 */
	init_reg2();
#endif

	/* Pointer is writable since we allocated a register for it */
	gd = (gd_t*)(_armboot_start - CONFIG_BOOTHEAD_GAP
		- CONFIG_SYS_MALLOC_LEN - sizeof(gd_t));
	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("": : :"memory");

	step(82);

	memset ((void*)gd, 0, sizeof (gd_t));
	gd->bd = (bd_t*)((char*)gd - sizeof(bd_t));
	memset (gd->bd, 0, sizeof (bd_t));
	insert_ddr_layout((unsigned int)gd->bd,
		(unsigned int)((char *)gd + sizeof(gd_t)), "global data");

	gd->flags |= GD_FLG_RELOC;

	monitor_flash_len = _bss_start - _armboot_start;

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		if ((*init_fnc_ptr)() != 0) {
			hang ();
		}
	}

	/* armboot_start is defined in the board-specific linker script */
	mem_malloc_init (_armboot_start - CONFIG_BOOTHEAD_GAP
		- CONFIG_SYS_MALLOC_LEN,
		CONFIG_SYS_MALLOC_LEN);

#ifndef CONFIG_SYS_NO_FLASH
	/* configure available FLASH banks */
	display_flash_config (flash_init ());
#endif /* CONFIG_SYS_NO_FLASH */

#ifdef CONFIG_VFD
#	ifndef PAGE_SIZE
#	  define PAGE_SIZE 4096
#	endif
	/*
	 * reserve memory for VFD display (always full pages)
	 */
	/* bss_end is defined in the board-specific linker script */
	addr = (_bss_end + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
	vfd_setmem (addr);
	gd->fb_base = addr;
#endif /* CONFIG_VFD */

#ifdef CONFIG_LCD
	/* board init may have inited fb_base */
	if (!gd->fb_base) {
#		ifndef PAGE_SIZE
#		  define PAGE_SIZE 4096
#		endif
		/*
		 * reserve memory for LCD display (always full pages)
		 */
		/* bss_end is defined in the board-specific linker script */
		addr = (_bss_end + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
		lcd_setmem (addr);
		gd->fb_base = addr;
	}
#endif /* CONFIG_LCD */

#if defined(CONFIG_CMD_NAND)
	nand_init();        /* go init the NAND */
#endif

#if defined(CONFIG_CMD_ONENAND)
	onenand_init();
#endif

#ifdef CONFIG_HAS_DATAFLASH
	AT91F_DataflashInit();
	dataflash_print_info();
#endif

#if defined(CONFIG_CMD_SF)
	puts("\n");
	spi_flash_probe(0, 0, 0, 0);
#endif

#ifdef CONFIG_GENERIC_MMC
	mmc_flash_init();
#endif

#if defined(CONFIG_CMD_NAND)
	nand_rr_param_init();
#endif

	/* initialize environment */
	env_relocate ();

#ifdef CONFIG_VFD
	/* must do this after the framebuffer is allocated */
	drv_vfd_init();
#endif /* CONFIG_VFD */

#ifdef CONFIG_SERIAL_MULTI
	serial_initialize();
#endif

	/* IP Address */
	gd->bd->bi_ip_addr = getenv_IPaddr ("ipaddr");

	stdio_init ();	/* get the devices list going. */

	jumptable_init ();

#if defined(CONFIG_API)
	/* Initialize API */
	api_init ();
#endif

	console_init_r ();	/* fully init console as a device */

#if defined(CONFIG_ARCH_MISC_INIT)
	/* miscellaneous arch dependent initialisations */
	arch_misc_init ();
#endif
#if defined(CONFIG_MISC_INIT_R)
	/* miscellaneous platform dependent initialisations */
	misc_init_r ();
#endif

#ifdef HISFV_PHY_U
	U_PHY_ADDR = HISFV_PHY_U;
#endif /* HISFV_PHY_U */
	U_PHY_ADDR = get_eth_phyaddr(0, U_PHY_ADDR);

#ifdef HISFV_PHY_D
	D_PHY_ADDR = HISFV_PHY_D;
#endif /* HISFV_PHY_D */
	D_PHY_ADDR = get_eth_phyaddr(1, D_PHY_ADDR);

	/* enable exceptions */
	enable_interrupts ();

	/* Perform network card initialisation if necessary */
#ifdef CONFIG_DRIVER_TI_EMAC
	/* XXX: this needs to be moved to board init */
extern void davinci_eth_set_mac_addr (const u_int8_t *addr);
	if (getenv ("ethaddr")) {
		uchar enetaddr[6];
		eth_getenv_enetaddr("ethaddr", enetaddr);
		davinci_eth_set_mac_addr(enetaddr);
	}
#endif

#if defined(CONFIG_DRIVER_SMC91111) || defined (CONFIG_DRIVER_LAN91C96)
	/* XXX: this needs to be moved to board init */
	if (getenv ("ethaddr")) {
		uchar enetaddr[6];
		eth_getenv_enetaddr("ethaddr", enetaddr);
		smc_set_mac_addr(enetaddr);
	}
#endif /* CONFIG_DRIVER_SMC91111 || CONFIG_DRIVER_LAN91C96 */

	/* Initialize from environment */
	if ((s = getenv ("loadaddr")) != NULL) {
		load_addr = simple_strtoul (s, NULL, 16);
	}
#if defined(CONFIG_CMD_NET)
	if ((s = getenv ("bootfile")) != NULL) {
		copy_filename (BootFile, s, sizeof (BootFile));
	}
#endif

#ifdef BOARD_LATE_INIT
	board_late_init ();
#endif

#ifdef CONFIG_BITBANGMII
	bb_miiphy_init();
#endif
#if defined(CONFIG_CMD_NET)
#if defined(CONFIG_NET_MULTI)
	puts ("Net:   ");
#endif
	eth_initialize(gd->bd);
#if defined(CONFIG_RESET_PHY_R)
	debug ("Reset Ethernet PHY\n");
	reset_phy();
#endif
#endif

#if defined(CONFIG_SHOW_MEMORY_LAYOUT)
	show_ddr_layout();
#endif /* CONFIG_SHOW_MEMORY_LAYOUT */

	reserve_mem_init();

	show_boot_env();

#ifdef CONFIG_PRODUCT_WITH_BOOT
	product_init();
#endif

#if defined(CONFIG_BOOTROM_SUPPORT) || defined(CONFIG_BOOTROM_CA_SUPPORT)
#ifndef CONFIG_SUPPORT_CA_RELEASE
	extern void download_boot(const int (*handle)(void));
	download_boot(NULL);
#endif
#endif

#ifdef CONFIG_DDR_TRAINING
	check_ddr_training();
#endif /* CONFIG_DDR_TRAINING */

	bootargs_prepare();

	printf("\n");

#ifdef CONFIG_PRODUCT_WITH_BOOT
	fastapp_entry(0, NULL);
#endif

#if defined(CONFIG_SHOW_RESERVE_MEM_LAYOUT)
	show_reserve_mem();
#endif
	/* main_loop() can return to retry autoboot, if so just run it again. */
	for (;;) {
		main_loop ();
	}

	/* NOTREACHED - no way out of command loop except booting */
}

void hang (void)
{
	puts ("### ERROR ### Please RESET the board ###\n");
	for (;;);
}

#if 0
/*
 * this is usb keyboard test code.
 */
wait_keybard_input(int c, int timeout)
{
	int jx = 0, ix = 0;
	int chr = -1;
	extern struct stdio_dev* stdio_get_by_name(char* name);
	struct stdio_dev* std_dev = NULL;

	usb_stop();
	printf("start usb ...\n");
	usb_init();
	drv_usb_kbd_init();
	std_dev =  stdio_get_by_name("usbkbd");
	if (std_dev) {
		while (ix < 20000) {
			if (jx++ > 100) {
				if (!(ix % 100))
					printf(" \rWait for keyboard CTRL press %d ...",
					(ix/100));
				jx = 0;
				ix++;
			}
			if (std_dev->tstc()) {
				chr = std_dev->getc();
				printf(" \b%c", chr);
				if (chr == 0)
					break;
			}
			udelay(10);
		}
		printf("\n");
	}
}

#endif

