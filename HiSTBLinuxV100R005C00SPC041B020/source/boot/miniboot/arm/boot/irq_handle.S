/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Cai Zhiyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#include <config.h>

#define MODE_MASK  0x1F
#define MODE_IRQ   0x12
#define MODE_SVC   0x13
#define MODE_ABT   0x17
#define MODE_UND   0x1B

#define VECTOR_RESET                     0
#define VECTOR_UNDEFINED_INSTRUCTION     1
#define VECTOR_SOFTWARE_IRQ              2
#define VECTOR_PREFETCH_ABORT            3
#define VECTOR_DATA_ABORT                4
#define VECTOR_RESERVED                  5
#define VECTOR_IRQ                       6
#define VECTOR_FIRQ                      7
#define VECTOR_SIZE                      8
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.macro entry_mode, mode
	mrs	r1, cpsr
	bic	r1, r1, #MODE_MASK
	orr	r1, r1, #\mode
	msr	cpsr, r1
.endm

.macro entry_exception, mode, vector
	sub	sp, sp, #72
	stm	sp, {r0 - r12}
	mov	r0, #\vector
	str	r0, [sp, #68]
	mrs	r0, spsr
	str	r0, [sp, #64]
	str	lr, [sp, #60]

	mov	r0, sp
	entry_mode MODE_SVC
	str	sp, [r0, #52]
	str	lr, [r0, #56]
	entry_mode \mode

	bl	__entry_exception_handle

	ldr	r6, [sp, #64]
	msr	spsr_cxsf, r6
	ldr	lr, [sp, #60]
	ldm	sp, {r0 - r12}
	add	sp, sp, #72
	subs	pc, lr, #4
.endm

.text
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@    void set_irq_stack(unsigned int sp);
@
.align	2
.global set_irq_stack
.type	set_irq_stack, %function
set_irq_stack:
	push	{r0, r1, lr}

	entry_mode MODE_IRQ
	mov	sp, r0
	entry_mode MODE_ABT
	mov	sp, r0
	entry_mode MODE_UND
	mov	sp, r0
	entry_mode MODE_SVC

	pop	{r0, r1, pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@    void __entry_irq(void);
@
.align	2
.type	__entry_irq, %function
__entry_irq:
	sub	sp, sp, #56
	sub	lr, lr, #4
	stm	sp, {r0 - r12, lr}
	mrs	r6, spsr

	mov	r0, sp
	bl	__entry_irq_handle

	msr	spsr_cxsf, r6
	mov	lr, sp
	add	sp, sp, #56
	ldm	lr, {r0 - r12, pc}^

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@    void __entry_und_inst(void);
@
.align	2
.type	__entry_und_inst, %function
__entry_und_inst:
	entry_exception MODE_UND, VECTOR_UNDEFINED_INSTRUCTION

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@    void __entry_data_abort(void);
@
.align	2
.type	__entry_data_abort, %function
__entry_data_abort:
	entry_exception MODE_ABT, VECTOR_DATA_ABORT

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@    void __entry_prefect_abort(void);
@
.align	2
.type	__entry_prefect_abort, %function
__entry_prefect_abort:
	entry_exception MODE_ABT, VECTOR_PREFETCH_ABORT

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.align	2
.global __vector_start
__vector_start:
	ldr	pc, _reset
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefect_abort
	ldr	pc, _data_abort
	ldr	pc, _unused
	ldr	pc, _irq
	ldr	pc, _firq
_reset:
	b .
_undefined_instruction:
	.word __entry_und_inst
_software_interrupt: 
	b .
_prefect_abort:
	.word __entry_prefect_abort
_data_abort:
	.word __entry_data_abort
_unused:
	b .
_irq:
	.word __entry_irq
_firq:
	.word __entry_irq

.global __vector_end
__vector_end:
