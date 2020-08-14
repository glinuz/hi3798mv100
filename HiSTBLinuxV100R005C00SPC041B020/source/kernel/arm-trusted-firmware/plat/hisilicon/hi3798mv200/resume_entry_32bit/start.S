.balign 4

.globl atf_32bit_resume
atf_32bit_resume:
	/*enable debug first */
#if 0
	mov	r2, #0
	ldr	r3, =0xf8a210ec
	str	r2, [r3]

	ldr	r3, =0xf8a210f0
	str	r2, [r3]

	ldr	r3, =0xf8a210f4
	str	r2, [r3]

	ldr	r3, =0xf8a210f8
	str	r2, [r3]

	ldr	r3, =0xf8a210fc
	str	r2, [r3]
#endif
	/* Load 64bit resume address from GEN30 */
	mov     r3, #0xf8000000
	ldr     r2, [r3, #0xf8]

	/* Set 64bit resume address to PERI_CPU_RVBARADDR */
	mov	r3, #0x0
	movt	r3, #0xf8a8
	str     r2, [r3, #0x34]

	/* Set all cpus booting-up in 64bit mode */
	ldr     r2, [r3, #0x30]
	orr     r2, r2, #0xF
	str     r2, [r3, #0x30]

	/* Set the arm generic timer freq */
	mov     r3, #0x0
	movt	r3, 0xf8bb
	mov     r2, #1
	str     r2, [r3, #0x20]
	movw    r2, #0x103
	str     r2, [r3]

	/* Warm reset */
	isb
	dsb
	mrc     15, 0, r3, cr12, cr0, 2
	orr     r3, r3, #0x3
	mcr     15, 0, r3, cr12, cr0, 2
	isb
1:	wfi
	b       1b


