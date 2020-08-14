#include "emmc_quirk.h"

.align  2
.global jump_to_read_reg
.type   jump_to_read_reg, %function
jump_to_read_reg:
	mov	r11, r0
	mov	r12, r1

	//restore stack
	ldr	r0, =0xfffe0f80
	ldr	r1, =0xFFFE3520
	str	r1, [r0]

	ldr	r0, =0xfffe0f84
	ldr	r1, =0xFFFF5CB8
	str	r1, [r0]

	ldr	r0, =0xfffe0f88
	ldr	r1, =0x965A4B88
	str	r1, [r0]

	//Set board_type
	ldr	r0, =BOARD_TYPE_ADDR
	str	r11, [r0]

	/* Restore CPSR */
	ldr	r0, =0x400001D3
	msr	CPSR_cxsf, r0
	ldr	r0, =0xFFFE39A0
	ldr	r1, =0x00002000

	//Set reg location
	mov	r2, r12

	//Enable user hold reg
	ldr	r3, =(1<<29)
	ldr	r4, =0x00000000
	ldr	r5, =0xFFFE00DC
	ldr	r6, =0x3C69A587
	ldr	r7, =0xFFFE26B0
	ldr	r8, =0xFFFE39A0
	//Reg location
	mov	r9, r2
	ldr	r10, =0x00002FE8
	ldr	r11, =0xF8000000
	ldr	r12, =0x00000004
	ldr	sp, =0xFFFE0F80
	ldr	lr, =0xFFFF3DBC
	isb
	dsb
	ldr	pc, =0xFFFF969C
	b	.
