/* serial.c - print to uart */
#include <platform.h>
#include <aux_common.h>
#include "serial.h"

static void pl01x_putc(char c)
{
	/* Wait until there is space in the FIFO */
	while (readl(SERIAL_BASE + UART_PL01x_FR) & UART_PL01x_FR_TXFF);

	/* Send the character */
	writel(c, SERIAL_BASE + UART_PL01x_DR);
}

static int serial_test_enable(void)
{
	otp_boot_info_lv_u un_otp_boot_info_lv;
	soc_fuse_1_u un_soc_fuse_1;

	//uart关闭的情况下，不输出串口打印
	un_soc_fuse_1.u32 = readl(PERI_SOC_FUSE_1);
	if (un_soc_fuse_1.bits.uart_disable)
	{
		return 0;
	}

	/* Output the debug info only when boot_info_lv is 0x3c */
	un_otp_boot_info_lv.u32 = readl(OTPCTRL_DEBUG_INFO);
	if (BOOTROM_DEBUG_UART == un_otp_boot_info_lv.bits.boot_info_lv)
	{
		return 1;
	}

	return 0;
}

void serial_putc(const char c)
{
	if (!serial_test_enable())
		return;

	if (c == '\n')
		pl01x_putc('\r');

	pl01x_putc(c);
}

void serial_puts (const char *s)
{
	if (!serial_test_enable())
		return;

	while (*s) {
		serial_putc(*s++);
	}
}

void serial_put_hex(int hex)
{
	int i;
	char c;

	if (!serial_test_enable())
		return;

	for (i = 28; i >= 0; i-=4) {
		c = (hex >> i) & 0x0F;
		if (c < 10)
			c += '0';
		else
			c += 'A' - 10;
		pl01x_putc(c);
	}
}

