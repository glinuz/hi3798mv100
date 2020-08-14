#ifndef __AUX_COMMON_H__
#define __AUX_COMMON_H__

#include <platform.h>

#ifdef CONFIG_ARCH_HI3798CV2X
enum cmd_type {
	CMD_GET_BD_TYPE = 0,
	CMD_INIT_REG,
	CMD_BUTT,
};
#endif

#ifdef CONFIG_ARCH_HI3798MV2X
enum cmd_type {
	CMD_GET_BD_TYPE = 0,
	CMD_INIT_REG,
	CMD_READ_EMMC_PARAM,
	CMD_READ_EMMC_BOOT,
	CMD_BUTT,
};
#endif

enum mod_type {
	MOD_TYPE_NORMAL = 0,
	MOD_TYPE_SPI = 1,
	MOD_TYPE_NAND = 2,
	MOD_TYPE_EMMC = 3,
	MOD_TYPE_SYNCNAND = 4,
	MOD_TYPE_SD = 4,
	MOD_TYPE_BUTT,
};

struct reg_init_param {
	void * buf;
	int pm_flag; /* wakeup flag, 0-normal, 1-wakeup */
	int ca_flag; /* secure chip flag, 0-normal, 1-CA */
};

struct emmc_cfg {
	void *ptr;                                /*Destination address*/
	unsigned int size;                        /*Read length*/
	unsigned int offset;                      /*Start offset*/
};

typedef union {
	struct {
		unsigned int reserved_0                  :5;     //[4:0]
		unsigned int uart_disable                :1;     //[5]
		unsigned int reserved_1                  :26;    //[31:6]
	} bits;
	unsigned int u32;
} soc_fuse_1_u;

typedef union {
	struct {
		unsigned int ITCSA4_IMLB                 :16;   //0x9c~0x9d
		unsigned int boot_info_lv                :8;    //0x9e
		unsigned int aux_code_verify_en          :8;    //0x9f
	} bits;
	unsigned int u32;
} otp_boot_info_lv_u;

#define readl(_a)        (*(volatile unsigned int *)(_a))
#define writel(_v, _a)   (*(volatile unsigned int *)(_a) = (_v))

static inline void delay(unsigned int cnt)
{
	while (cnt--)
		__asm__ __volatile__("nop");
}

int get_boot_mode(void);
void serial_put_hex(int hex);
void serial_puts (const char *s);
void serial_putc (const char c);
int init_register(struct reg_init_param *param);
int init_emmc(void);
int get_board_type(void);
unsigned int boottype2modtype(unsigned int boot_type);
void timer_start(void);
void udelay(unsigned long us);
unsigned long timer_get_divider(void);
unsigned long timer_get_val(void);
unsigned int mmc_read(void *ptr, unsigned int size, unsigned int offset);
long long get_chipid_reg(void);
#endif
