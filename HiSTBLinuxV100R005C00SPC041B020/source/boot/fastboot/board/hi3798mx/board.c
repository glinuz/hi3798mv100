
#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/sizes.h>
#include <asm/arch/platform.h>
#include <version.h>

extern void eth_config_init(void);

static inline void delay (unsigned long loops)
{
	__asm__ volatile ("1:\n"
		"subs %0, %1, #1\n"
		"bne 1b":"=r" (loops):"0" (loops));
}

static void board_set_ddr_size(void)
{
	unsigned int tmp = readl(REG_MEM_COMB);
	unsigned int mem_mode = (tmp & (MEM_MODE_MASK << MEM_MODE_SHIFT));
	unsigned int mem_comb;

	if (!mem_mode)
		return;

	mem_comb = (tmp >> MEM_COMB_SHIFT) & MEM_COMB_MASK;
	tmp = (SZ_256M << mem_comb) + (SZ_128M << mem_comb);
	preset_ddr_size(tmp);
}

/*
 * Miscellaneous platform dependent initialisations
 */
int board_init (void)
{
	unsigned int size;
	unsigned int ddr_base;

	extern unsigned int _text_end;
	DECLARE_GLOBAL_DATA_PTR;

	gd->bd->bi_arch_number = MACH_TYPE_GODBOX;
	gd->bd->bi_boot_params = CFG_BOOT_PARAMS;
	gd->flags = 0;

	board_set_ddr_size();

	ddr_base = MEM_BASE_DDR;
	size = mmu_init(ddr_base, MEM_BASE_DDR, get_ddr_size());
	insert_ddr_layout(ddr_base, ddr_base + size, "page table");

	insert_ddr_layout(ddr_base + size,
		(unsigned int)(TEXT_BASE - CONFIG_BOOTHEAD_GAP
		- CONFIG_SYS_MALLOC_LEN - CONFIG_SYS_GBL_DATA_SIZE - 12),
		"stack");

	insert_ddr_layout((unsigned int)TEXT_BASE,
		(unsigned int)_text_end, ".text");

	insert_ddr_layout((unsigned int)_text_end,
		(unsigned int)_bss_end, "data");

	return 0;
}

int misc_init_r (void)
{
#ifdef CONFIG_RANDOM_ETHADDR
	random_init_r();
#endif
	eth_config_init();
	return (0);
}

static void display_info(void)
{
	char *str;

	printf("Fastboot:      Version %d.%d.%d\n",
		VERSION, PATCHLEVEL, SUBLEVEL);
	printf("Build Date:    "__DATE__", "__TIME__"\n");
	printf("CPU:           %s ", get_cpu_name());
	if (CHIPSET_CATYPE_CA == get_ca_type())
		printf("(CA)");
	printf("\n");
	get_bootmedia(&str, NULL);
	printf("Boot Media:    %s\n", str);
	printf("DDR Size:      %sB\n", ultohstr(get_ddr_size()));

	printf("\n");
}

int dram_init (void)
{
	DECLARE_GLOBAL_DATA_PTR;
	gd->bd->bi_dram[0].start = MEM_BASE_DDR;
	gd->bd->bi_dram[0].size  = get_ddr_size();

	display_info();
	return 0;
}

int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_NET_HISFV300
	rc = hieth_initialize();
#endif
	return rc;
}
