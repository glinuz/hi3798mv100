
#include <config.h>

.align  4
.globl _slave_cpu_startup
.globl _slave_cpu_end
.globl _slave_start

_slave_cpu_startup:
__slave_start:
	B      __slave_reset
	B      .
	B      __slave_smc_start
	B      .
	B      .
	B      .
	B      .
	B      .

__slave_reset:
	/* set the cpu to SVC32 mode */
	mrs	r0, cpsr
	bic	r0, r0, #0x1f  /* r0 = ((~0x1F) & r0) */
	orr	r0, r0, #0xd3  /* r0 = (0xd3 | r0)    */
	msr	cpsr, r0


	/* add for set NSACR*/
	ldr	r0,=0x00060c00           @Bit 10:unsecure access cp10       Bit 11:unsecure access cp11
	mcr	p15, 0, r0, c1, c1, 2    @Bit 17:non_secure can access L2ECTLR   Bit 18:non_secure can access actlr smp bit

	/* Set GIC */
	/* Get the address of the GIC */
	ldr	r0, =0xF8A01000         @ GICD_BASE

	/* Set all SGI PPI non-secure*/
	ldr	r1,=0xffffffff
	str	r1,[r0,#0x80]

	ldr	r0, =0xF8A02000         @ GICC_BASE
	ldr	r1,[r0]
	orr	r1,r1,#0x2              @ bit3:FIQEn 0 bit2:AckCtl 0  bit1:EnableNS 1 bit0:EnableS 0
	str	r1,[r0]

	/* Unmask all interrupts */
	mov	r1, #0xFF
	str	r1, [r0, #0x4]	        @ Write	the Priority Mask register

	ldr    r1, =0xFFFF0000          @ set Monitor Vector Base Address of secondary cpus
	mcr    p15, 0, r1, c12, c0, 1   @ set MVBAR

	/* Prepare the kernel start address */
	ldr    r1, =REG_BASE_SCTL
	ldr    r0, [R1, #REG_SC_GEN1]
	isb
	dsb
	.arch_extension sec             @ gcc version > 40600
	smc    #0
	nop
	b      .

__slave_smc_start:
	mov	lr, r0
	mrc	p15, 0, r1, c1, c1, 0                   @ Read Secure Configuration Register
	orr	r1, r1, #(1<<0)                         @ Switch to Non-secure world
	orr	r1, r1, #(1<<5)                         @ CPSR.A bit can be modified in any security state.
	orr	r1, r1, #(1<<7)	                        @ The SMC instruction is UNDEFINED in Non-secure state.
	mcr	p15, 0, r1, c1, c1, 0                   @ Write Secure Configuration Register
	movs	pc, lr                                  @ Jump to the non-secure kernel initializtion
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop


_slave_cpu_end:

