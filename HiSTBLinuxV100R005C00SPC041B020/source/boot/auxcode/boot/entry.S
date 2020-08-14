/*
 * int auxcode(int cmd, void *args)
 */
 .macro step, num
@#ifdef CONFIG_BOOT_SIMULATE
        @push {r11, r12}
        ldr r11, =0xf8000000
        mov r12, #\num
        str r12, [r11, #0xfc]
        @pop {r11, r12}
@#endif
.endm

.global auxcode
.type   auxcode, %function

.globl _start
_start:
auxcode:
	step 1
	push	{r4-r11, lr}
	bl	start_auxcode
	pop	{r4-r11, pc}

