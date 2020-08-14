#include <asm_macros.S>

.global c51_cpu_suspend

#define REG_GEN9	0xf80000a4
#define REG_GEN13	0xf80000b4
#define REG_GEN30	0xf80000f8

#define MCU_START_CTRL  0xf840f000 /* mcu start control */
#define OTP_IDWORD_ADDR 0xf8ab0128 /* otp idword */
#define OTP_CA_ID_WORD  0x6EDBE953

#define CA_DDR_WAKEUPCHECK_START	0xffff2000
.section .rodata.pm_debug_str, "aS"
	debug_msg: .asciz "INFO: enter CA\n"

func c51_cpu_suspend

	/* set scu boot entry point */
	ldr	w0,	=resume_entry_start_32
	ldr	x1,	=REG_GEN9
	str	w0,	[x1]

	/* set 64bit entry point */
	ldr	w0,	=psci_entrypoint
	ldr	x1,	=REG_GEN30
	str	w0,	[x1]	

	/* if it is not advca chipset, jump to enable_mcu */
	ldr	x0, =OTP_IDWORD_ADDR
	ldr	w0, [x0]
	ldr	w1, =OTP_CA_ID_WORD
	cmp	w0, w1
	bne	enable_mcu

	/* print before jump to exec CA code in sram. */
	mov	x5, x30
	bl	plat_crash_console_init
	/* Check if the console is initialized */
	cbz	x0, _panic_loop
	/* The console is initialized */
	adr	x4, debug_msg
	bl	asm_print_str
	mov	x30, x5	

	/* disable mmu_el3. */
	ldr	x2, =CA_DDR_WAKEUPCHECK_START
	bl	disable_mmu_el3
	br	x2
enable_mcu:
	/* start mcu */
	ldr	x0,	=MCU_START_CTRL
	ldr	w1,	[x0]
	orr	w1, 	w1, #0x1
	str	w1,	[x0]

#if 0
	/* release the mcu */
	ldr	w0,	=0x12345678
	ldr	x1,	=REG_GEN13
	str	w0,	[x1]

loop_wfi:
	dsb	sy
	isb
	wfi
	b	loop_wfi

#endif
	ret

_panic_loop:
	b	_panic_loop
