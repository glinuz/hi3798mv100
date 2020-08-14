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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@  void backtracking(uint32 fp);
@
.text
.align
.global backtracking
.type	backtracking, %function
backtracking:
	stmfd	sp!, {r4 - r7, lr}
	cmp	r0, #0
	moveq	r4, fp
	movne	r4, r0

1:	stmfd	sp!, {pc}
	ldr	r0, [sp], #4
	adr	r1, 1b
	sub	r7, r0, r1

next_fp:
	ldr	r6, [r4, #0]
	ldr	r5, [r4, #-12]
	sub	r6, r6, r7
	ldr	r2, [r6, #-4]
	ldr	r3, L0+4
	teq	r3, r2, lsr #10
	subne	r0, r6, #4
	subeq	r0, r6, #8

	ldr	r1, [r4, #-4]
	mov	r2, r4
	bl	dump_trace_symbol

	ldr	r1, [r6, #-4]
	ldr	r3, L0+4
	teq	r3, r1, lsr #10
	ldreq	r0, [r4, #-8]
	subeq	r0, r0, #4

	ldr	r1, [r6, #0]
	ldr	r3, L0
	teq	r3, r1, lsr #10
	subeq	r0, r4, #16

	teq	r5, #0
	beq	exit

	cmp	r5, r4
	mov	r4, r5
	bhi	next_fp
exit:
	ldmfd	sp!, {r4 - r7, pc}
.align
L0:
	.word	0xe92dd800 >> 10
	.word	0xe92d0000 >> 10
