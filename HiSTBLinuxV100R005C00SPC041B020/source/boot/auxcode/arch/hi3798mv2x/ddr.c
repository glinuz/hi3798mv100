#include <config.h>
#include <aux_common.h>
#include <platform.h>

extern void reset_cpu(void);

/******************************************************************************/

#ifdef CONFIG_DDRT_TEST
void ddrt_test(void)
{
	char *ddrt_base = (char *)0xF8A50000;
	unsigned int value;
	static unsigned int count = 1;

	if (readl(0xf8000088) != 0)
		return;

	writel(0x4f, ddrt_base + 0x8); //burst16
	writel(0x1152, ddrt_base + 0xc);

	/* only for fangzhen. */
	writel(0xff, ddrt_base + 0x10); //burst16 number per loop
	writel(0xffffffff, ddrt_base + 0x14);

	/* only for fangzhen. */
	writel(0xff, ddrt_base + 0x18); //number of loop
	writel(0x0, ddrt_base + 0x1c);
#define DDRT_TEST_ADDR 0x4800000
	writel(DDRT_TEST_ADDR, ddrt_base + 0x20); //ddrt mem base, 0x20000 bytes
	writel(0x5a5a5a5a, ddrt_base + 0x30);
	writel(0x1, ddrt_base);

	value = readl(ddrt_base+0x4);
	value = value & 0x1;

	serial_puts("DDRT Start\n");
	while (value != 1) {
		value = readl(ddrt_base+0x4);
		value = value & 0x1;
		if (!(count & (16 - 1))) {
			count = 1;
			serial_puts("(");
			serial_put_hex(readl(ddrt_base));
			serial_puts(")\n");
		} else {
			serial_puts("#");
		}
		count++;
		udelay(10000);
	}

	value = readl(ddrt_base+0x4);
	if (value == 0x3) {
		serial_puts("DDRT Test Pass ###\n");
	} else {
		serial_puts("DDRT Test Failed (");
		serial_put_hex(value);
		serial_puts(") ###\n");
	}
}
#else
void ddrt_test(void) { return;}
#endif

/*****************************************************************************/
void ddr_set_sref(unsigned int val)
{
	int cnt = 0;

	writel(val, REG_DDR_DMC0_CTRL_SREF);
	while ((!(readl(REG_DDR_DMC0_IN_SREF) & 0x1)) && (cnt < 10000)) {
		cnt++;
	}

	if (cnt >= 10000) {
		serial_puts("SREF Time out!\n");
		reset_cpu();
	}

	//serial_puts("DDR IN SREF(");
	//serial_put_hex(readl(REG_DDR_DMC0_IN_SREF));
	//serial_puts(")\n");
	
}

void ddr_scramble(void)
{
	if (readl(REG_KLD_SEC_FLAG) == NONSEC_ID_WORD) {
		if (readl(REG_BASE_SCTL + REG_SC_GEN29) & 2) {
			ddr_set_sref(0x1);
			writel(0xA,REG_KLD_DDRSCRAMBLE_EN);
			ddr_set_sref(0x2);
		}
	} else {
		if (readl(REG_OTPC_DDRSCRAMBLE_EN) & REG_OTPC_DDRSCRAMBLE_MASK) {
			int cnt = 0;
			unsigned int rng_data_l, rng_data_h;

			while (((readl(REG_RNG_CNT)>>8)&0xFF) < 2 && cnt<10000) {
				cnt++;
			}

			if (cnt >= 10000) {
				serial_puts("RNG Time out!\n");
				reset_cpu();
			}

			rng_data_l = readl(REG_RNG_DATA);
			rng_data_h = readl(REG_RNG_DATA);
			writel(rng_data_l, REG_SYS_DDRSCRAMBLE_SEED_L);
			writel(rng_data_h, REG_SYS_DDRSCRAMBLE_SEED_H);
			writel(0x1, REG_SYS_DDRSCRAMBLE_LOCK);
			ddr_set_sref(0x1);
			writel(0xA, REG_KLD_DDRSCRAMBLE_EN);
			ddr_set_sref(0x2);
		}
	}

	/* Enable MDDRC Auto-Lowpower mode */
	writel(0x401, REG_DDR_DMC0_LPCTRL);
}

/***********************************************************************************
* Function: ddr_wakeup_restore
* Description: restore ddrc register when wake up from standby.
* format: base + 0x0 -> num
* base + 0x4 -> first_reg_addr
* base + 0x8 -> val[0] (for first_reg_addr)
* base + 0xc -> val[1] (for first_reg_addr + 0x4)
* ...
* base + 0x8 + (num-1)*4 -> val[num-1] (for first_reg_addr + (num-1)*0x4)
* base + xx -> num
* base + xx+4 -> second_reg_addr
* ...
* end -> 0 (end)
***********************************************************************************/

void ddr_wakeup_restore(void)
{
	unsigned int i, num;
	unsigned int addr;
	unsigned int val;
	unsigned int *base = (unsigned int *)(*(unsigned int*)(REG_BASE_SCTL + REG_SC_GEN16)); /* GEN16 store base address. */

	/* if register equal to 0, no need restore. */
	if(0 == base)
		return;

	while(0 != (num = *(base++))) {
		addr = *(base++);
		for(i = 0; i < num; i++) {
			val = *(base++);
			*(unsigned int*)(addr + (i<<2)) = val;
		}
	}
}