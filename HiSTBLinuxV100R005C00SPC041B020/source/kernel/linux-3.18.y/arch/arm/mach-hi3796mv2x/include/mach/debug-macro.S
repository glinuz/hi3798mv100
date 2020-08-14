#include <mach/platform.h>
#include <linux/amba/serial.h>

.macro  addruart, rp, rv, tmp
	ldr	\rp, =CONFIG_DEFAULT_UART_BASE_ADDR @physical base
	ldr     \rv, =(CONFIG_DEFAULT_UART_BASE_ADDR +IO_OFFSET_LOW)
	@ldr	\rv, =0xFEC00000 @IO_ADDRESS(CONFIG_DEFAULT_UART_BASE_ADDR +IO_OFFSET_LOW) @virtual base
.endm

#include <debug/pl01x.S>
