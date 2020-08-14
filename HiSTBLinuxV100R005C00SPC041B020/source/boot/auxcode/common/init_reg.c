/******************************************************************************
 * init boot reg
 *****************************************************************************/
#include <config.h>
#include <aux_common.h>
#include <platform.h>
/*****************************************************************************/

#define REG_MODULE_HEAD_SIZE 2
#define REG_LEN_SIZE         2
#define REG_BASE_SIZE        3
#define REG_CFG_LEN_SIZE     2
#define REG_FMT_SIZE         3
#define REG_TABLE_END_FLAG   0
#define REG_OFFSET_POS       10
#define REG_MAX_LENGTH       8192
#define REG_VERSION_V120     0x30323176  /* v120 */

union reg_module_head {
	struct {
		unsigned char ca_flag : 1;     /* whether execute when ca chip */
		unsigned char normal_flag : 1; /* whether execute when normal(no-ca) chip */
		unsigned char wakeup_flag : 1; /* whether execute when wakeup from standby */
		unsigned char boot_flag : 1;   /* whether execute when power on */
		unsigned char module_type : 4; /* module type, like: spi,nand...  */
	} member;
	unsigned char val;
};

union reg_format {
	struct {
		/* byte 0 */
		unsigned char rw_flag : 1;     /* read or write: 0-write; 1-read */
		unsigned char reserve : 1;
		unsigned char offset : 6;      /* real offset = (offset << 2)*/

		/* byte 1 */
		unsigned char bit_off : 5;     /* bit offset of read/write */
		unsigned char val_len : 3;     /* byte length of val to be read/write: 0-0; 1~3 - 1~3byte; */

		/* byte 2 */
		unsigned char bit_num : 5;     /* bit num of read/write, real bit number = bit_num + 1 */
		unsigned char delay_len : 3;   /* byte length of delay: 0-endless; 1~3 - 1~3byte; */
	} member;
	unsigned char val[3];
};
/*****************************************************************************/

extern void set_core_voltage(void);
extern void start_ddr_hw_training(void);
extern int ddr_sw_training_if(void *ddrtr_result);
extern void reset_cpu(void);
extern void ddr_wakeup_restore(void);
extern void ddr_scramble(void);
extern void ddrt_test(void);

/*****************************************************************************/

static void reg_read_write(unsigned int addr,
		unsigned int val,
		unsigned int wait,
		union reg_format *reg)
{
	unsigned int tmp_val;
	unsigned int bit_num = reg->member.bit_num + 1;
	int forever = 0;

#ifdef CONFIG_REG_DEBUG
	serial_puts("\n r/w ");
	serial_put_hex(reg->member.rw_flag);
	serial_puts(" addr ");
	serial_put_hex(addr);
	serial_puts(" bn ");
	serial_put_hex(reg->member.bit_num);
	serial_puts(" bo ");
	serial_put_hex(reg->member.bit_off);
	serial_puts(" v ");
	serial_put_hex(val);
	serial_puts(" d ");
	serial_put_hex(wait);
#endif

	if (reg->member.rw_flag) { /* read */
		/* if wait == 0, then wait endless */
		if (!wait)
			forever = 1;

		do {
			tmp_val = readl(addr);
			if (bit_num != 32) {
				tmp_val >>= reg->member.bit_off;
				tmp_val &= ((1<<bit_num)-1);
			}

			/* timeout */
			if (!forever && (0 == wait--))
				break;

			delay(1);
		} while (tmp_val != val);

		if (tmp_val != val) {
			serial_puts("Warning: read reg: ");
			serial_put_hex(addr);
			serial_puts(" fail.\n");
		}

	} else { /* write */
		if (bit_num != 32) {
			tmp_val = readl(addr);
			tmp_val &= (~(((1<<bit_num)-1)<<reg->member.bit_off));
			val = val<<reg->member.bit_off;
			val |= tmp_val;
		}
		writel(val, addr);

		delay(wait);
	}
}
/*****************************************************************************/

static void reg_parse_register(unsigned char *buf, unsigned int base, int length)
{
	unsigned char *pbuf = buf;
	union reg_format reg;
	unsigned int reg_addr;
	unsigned int val, delay;

	while (pbuf < (buf + length)) {
		reg.val[0] = pbuf[0];
		reg.val[1] = pbuf[1];
		reg.val[2] = pbuf[2];
		pbuf += REG_FMT_SIZE;

		val = delay = 0;

		while (reg.member.val_len--)
			val = (val << 8) | (*pbuf++);

		while (reg.member.delay_len--)
			delay = (delay << 8) | (*pbuf++);

		reg_addr = base + (reg.member.offset<<2);

		reg_read_write(reg_addr, val, delay, &reg);
	}
}
/*****************************************************************************/

static void reg_parse_group(unsigned char *buf, int length)
{
	unsigned int base;
	int regcfg_len;
	unsigned char *pbuf = buf;

	while (pbuf < (buf + length)) {
		base = (pbuf[0]<<24) | (pbuf[1]<<16) | (pbuf[2]<<8);
		regcfg_len = (pbuf[3]<<8) | pbuf[4];

		pbuf += REG_BASE_SIZE + REG_CFG_LEN_SIZE;
		reg_parse_register(pbuf, base, regcfg_len);
		pbuf += regcfg_len;
	}
}
/*****************************************************************************/

void reg_init_module(struct reg_init_param *param)
{
	unsigned int regbase = (unsigned int)(param->buf);
	unsigned int pm_flag = !!(param->pm_flag);
	unsigned int ca_flag = !!(param->ca_flag);
	unsigned int module_type;
	union reg_module_head head;
	int length = 0;
	unsigned short offset = *(unsigned short *)(regbase + REG_OFFSET_POS);
	unsigned char *regbuf = (unsigned char *)(regbase + offset);
	int boot_mode;

	boot_mode = get_boot_mode();
	module_type = boottype2modtype(boot_mode);

	while ((REG_TABLE_END_FLAG != (*(unsigned int *)regbuf)) && (length < REG_MAX_LENGTH)) {
		length = (regbuf[2]<<8) | regbuf[3];
		head.val = regbuf[1];
		regbuf += REG_MODULE_HEAD_SIZE + REG_LEN_SIZE;
		if ((!head.member.module_type) || (head.member.module_type == module_type)) {
			if ((head.member.boot_flag & (!pm_flag)) || (head.member.wakeup_flag & pm_flag))
				if ((head.member.normal_flag & (!ca_flag)) || (head.member.ca_flag & ca_flag)) {
					reg_parse_group(regbuf, length);
				}
		}

		length = (length + 3) & (~3);
		regbuf += length;
	}
}
/*****************************************************************************/

static void reg_show_info(unsigned int regbase)
{
#ifndef CONFIG_BOOT_SIMULATE
	serial_puts("\nReg Version:  ");
	serial_puts((char *)(regbase + 0x4));

	serial_puts("\nReg Time:     ");
	serial_puts((char *)(regbase + 0xc));
#endif
	serial_puts("\nReg Name:     ");
	serial_puts((char *)(regbase + 0x20));
	serial_puts("\n");
	return;
}

/*****************************************************************************/
int enable_timer_counter(void)
{
	unsigned int val;

	/* Enable Timer counter */
	writel(0x1, REG_SYS_CNTFID0);
	writel(0x103, REG_SYS_CNTCR);

	/* Set the CNTFRQ to 24M */
	val = 24000000;
	__asm__ __volatile__(
		"dsb\n\r"
		"mcr p15, 0, %0, c14, c0, 0\n\r"
		"isb\n\r"::"r"(val));
	return 0;
}
/*****************************************************************************/

static void resume(void)
{
	/* Restore DDRPHY Register */
	ddr_wakeup_restore();
}

/*****************************************************************************/

int init_register(struct reg_init_param *param)
{
	int ret = 0;
	unsigned int regbase = (unsigned int)(param->buf);
	unsigned int version = *(unsigned int *)regbase;

	step(4);

	if (version != REG_VERSION_V120) {
		serial_puts("Invalid reg version\n");
		return -1;
	}

	reg_show_info(regbase);
	step(5);

	reg_init_module(param);
	step(6);

#ifndef CONFIG_FPGA
	step(8);

	if (param->pm_flag) {
		step(9);
		/* restore parameter */
		resume();
	} else {
		/* Start the hardware ddr training after SVB procedure.
		This step SHOULD not be removed, even that the hardware training
		disabled in boot reg, because some steps,
		such as clearing-fifo, needed after SVB */
		step(10);
		if (!(readl(REG_BASE_SCTL + REG_SC_GEN18) & SVB_ENABLE)) {
			step(7);
			set_core_voltage();
		}

		start_ddr_hw_training();
		step(22);

		/* Start the software ddr training */
		ret = ddr_sw_training_if(0);
	}

#ifdef CONFIG_BOOT_SIMULATE
	writel(0x12345678, 0x1000000);
	if (readl(0x1000000) != 0x12345678)
		step(25);
	else
		step(26);
#endif

	ddr_scramble();
	step(27);
#endif

	ddrt_test();

	enable_timer_counter();

	return ret;
}

