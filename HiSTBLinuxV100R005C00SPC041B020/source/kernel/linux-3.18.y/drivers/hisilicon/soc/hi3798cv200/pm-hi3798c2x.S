#include <asm/assembler.h>
#include <linux/linkage.h>

.text
ENTRY(cpu_powerup_hi3798cv2x)
	safe_svcmode_maskall	r1

	/* Set CPUECTLR.SMPEN */
	mrrc	p15, 1, r0, r2, c15
	orr	r0, #(1 << 6)
	mcrr	p15, 1, r0, r2, c15

	b	cpu_resume
ENDPROC(cpu_powerup_hi3798cv2x)

ENTRY(cpu_powerdown_hi3798cv2x)
	clrex

	/* Clear  CPUECTLR.SMPEN */
	mrrc	p15, 1, r0, r1, c15
	bic	r0, #(1 << 6)
	mcrr	p15, 1, r0, r1, c15

	isb
	dsb

1:	wfi
	b 1b

ENDPROC(cpu_powerdown_hi3798cv2x)

ENTRY(flash_l1_cache_hi3798cv2x)
	push	{r0-r6, lr}
	mov	r0, #0
	mcr	p15, 2, r0, c0, c0, 0
	mrc	p15, 1, r1, c0, c0, 0
	bic	r1, r1, #0xF0000000
	mov	r2, r1, lsr #13
	add	r2, r2, #1
	mov	r4, r1, lsr #3
	and	r4, r4, #0xFF
	add	r4, r4, #1
	mov	r1, #0
loop1:	mov	r0, #0
loop2:	mov	r3, r0, lsl #6
	add	r3, r3, r1, lsl #30
	mcr	p15, 0, r3, c7, c10, 2
	add	r0, r0, #1
	cmp	r0, r2
	bne	loop2
	add	r1, r1, #1
	cmp	r1, r4
	bne	loop1
	isb
	dsb
	pop	{r0-r6, pc}
ENDPROC(flash_l1_cache_hi3798cv2x)

