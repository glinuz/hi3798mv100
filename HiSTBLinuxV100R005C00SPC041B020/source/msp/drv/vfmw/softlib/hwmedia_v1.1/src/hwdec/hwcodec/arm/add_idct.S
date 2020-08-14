/* Theorarm library
 Copyright (C) 2009 Robin Watts for Pinknoise Productions Ltd
*/	

#define xC2S6 60547
#define xC3S5 54491
#define xC4S4 46341
#define xC5S3 36410
#define xC6S2 25080
#define xC7S1 12785
#define xC1S7 64277

.text
.align
OC_C1S7:    .long xC1S7
OC_C2S6:    .long xC2S6
OC_C3S5:    .long xC3S5
OC_C4S4:    .long xC4S4
OC_C5S3:    .long xC5S3
OC_C6S2:    .long xC6S2
OC_C7S1:    .long xC7S1

.global idct_add_armv6
idct_add_armv6:
	/* r0 = uint8_t *dst*/
	/* r1 = int          stride*/
	/*r2 = int16_t *input */
	/*r3 = int number*/
	CMP	r3, #3
	BLT	oc_idct8x8_3
	CMP	r3, #6
	BLT	oc_idct8x8_6
	CMP	r3, #10
	BLT	oc_idct8x8_10
oc_idct8x8_slow:
	STMFD	r13!,{r4-r11,r14}
	/*存储dst和stride*/
	push    {r0}
	push    {r1}
	SUB	r13,r13,#64*2

	MOV	r1, r2		/* read from r1*/
	MOV	r0, r13		/* write to temp storage*/
	BL	idct8core
	ADD	r1, r1, #16
	BL	idct8core
	ADD	r1, r1, #16
	BL	idct8core
	ADD	r1, r1, #16
	BL	idct8core
	ADD	r1, r1, #16
	BL	idct8core
	ADD	r1, r1, #16
	BL	idct8core
	ADD	r1, r1, #16
	BL	idct8core
	ADD	r1, r1, #16
	BL	idct8core

	MOV	r1, r13		/* and dst becomes src*/
	BL	idct8core_down
	ADD	r1, r1, #16
	BL	idct8core_down
	ADD	r1, r1, #16
	BL	idct8core_down
	ADD	r1, r1, #16
	BL	idct8core_down
	ADD	r1, r1, #16
	BL	idct8core_down
	ADD	r1, r1, #16
	BL	idct8core_down
	ADD	r1, r1, #16
	BL	idct8core_down
	ADD	r1, r1, #16
	BL	idct8core_down

	ADD	r13,r13,#64*2
	ADD     r13, #8
	LDMFD	r13!,{r4-r11,PC}

oc_idct8x8_10:
	STMFD	r13!,{r4-r11,r14}
	
	/*dst 和  stride 首先入栈 x00141957*/
	push    {r0}
	push    {r1}
	SUB	r13,r13,#64*2
	

	MOV	r1, r2		/* read from r2*/
	MOV	r0, r13		/* write to temp storage*/
	BL	idct4core
	BL	idct3core
	BL	idct2core
	BL	idct1core

	MOV	r1, r13		/* and dst becomes src*/
	BL	idct4core_down
	BL	idct4core_down
	BL	idct4core_down
	BL	idct4core_down
	BL	idct4core_down
	BL	idct4core_down
	BL	idct4core_down
	BL	idct4core_down

	ADD	r13,r13,#64*2
	
	ADD     r13, #8
	
	LDMFD	r13!,{r4-r11,PC}
oc_idct8x8_6:
	STMFD	r13!,{r4-r11,r14}
	push    {r0}
	push    {r1}
	SUB	r13,r13,#64*2

	MOV	r1, r2		/* read from r1*/
	MOV	r0, r13		/* write to temp storage*/
	BL	idct3core
	BL	idct2core
	BL	idct1core

	MOV	r1, r13		/* and dst becomes src*/
	BL	idct3core_down
	BL	idct3core_down
	BL	idct3core_down
	BL	idct3core_down
	BL	idct3core_down
	BL	idct3core_down
	BL	idct3core_down
	BL	idct3core_down

	ADD	r13,r13,#64*2
	ADD     r13, #8
	LDMFD	r13!,{r4-r11,PC}
oc_idct8x8_3:
	STMFD	r13!,{r4-r11,r14}
	
	push    {r0}
	push    {r1}
	
	SUB	r13,r13,#64*2

	MOV	r1, r2		/* read from r1*/
	MOV	r0, r13		/* write to temp storage*/
	BL	idct2core
	BL	idct1core

	MOV	r1, r13		/* and dst becomes src*/
	BL	idct2core_down
	BL	idct2core_down
	BL	idct2core_down
	BL	idct2core_down
	BL	idct2core_down
	BL	idct2core_down
	BL	idct2core_down
	BL	idct2core_down

	ADD	r13,r13,#64*2
	ADD     r13, #8
	LDMFD	r13!,{r4-r11,PC}


idct8_2:
	/* r0 =       ogg_int16_t *y      (destination)
	 r1 = const ogg_int16_t *x      (source)
	*/
	STMFD	r13!,{r4-r11,r14}

	BL	idct2core

	LDMFD	r13!,{r4-r11,PC}
idct8_3:
	/* r0 =       ogg_int16_t *y      (destination)
	 r1 = const ogg_int16_t *x      (source)
	*/
	STMFD	r13!,{r4-r11,r14}

	BL	idct3core

	LDMFD	r13!,{r4-r11,PC}
idct8_4:
	/* r0 =       ogg_int16_t *y      (destination)
	 r1 = const ogg_int16_t *x      (source)
	*/
	STMFD	r13!,{r4-r11,r14}

	BL	idct4core

	LDMFD	r13!,{r4-r11,PC}
idct8_1:

idct1core:
	/* r0 =       ogg_int16_t *y      (destination)
	 r1 = const ogg_int16_t *x      (source)
	*/
	LDRSH	r3, [r1], #16
	MOV	r12,#0x05
	ORR	r12,r12,#0xB500
	MUL	r3, r12, r3
	/* Stall ?*/
	MOV	r3, r3, ASR #16
	STRH	r3, [r0], #2
	STRH	r3, [r0, #14]
	STRH	r3, [r0, #30]
	STRH	r3, [r0, #46]
	STRH	r3, [r0, #62]
	STRH	r3, [r0, #78]
	STRH	r3, [r0, #94]
	STRH	r3, [r0, #110]
	MOV	PC,R14

idct2core:
	/* r0 =       ogg_int16_t *y      (destination)
	 r1 = const ogg_int16_t *x      (source)
	*/
	LDRSH	r2, [r1], #16		/* r2 = x[0]*/
	LDR	r12,OC_C4S4
	LDRSH	r11,[r1, #-14]		/* r11= x[1]*/
	LDR	r3, OC_C7S1
	MUL	r2, r12,r2		/* r2 = t[0]<<16 = OC_C4S4*x[0]*/
	LDR	r10,OC_C1S7
	MUL	r3, r11,r3		/* r3 = t[4]<<16 = OC_C7S1*x[1]*/
	MOV	r2, r2, ASR #16		/* r2 = t[0]*/
	MUL	r11,r10,r11		/* r11= t[7]<<16 = OC_C1S7*x[1]*/
	MOV	r3, r3, ASR #16		/* r3 = t[4]*/
	MUL	r10,r12,r3		/* r10= t[5]<<16 = OC_C4S4*t[4]*/
	MOV	r11,r11,ASR #16		/* r11= t[7]*/
	MUL	r12,r11,r12		/* r12= t[6]<<16 = OC_C4S4*t[7]*/
	MOV	r10,r10,ASR #16		/* r10= t[5]*/
	ADD	r12,r2,r12,ASR #16	/* r12= t[0]+t[6]*/
	ADD	r12,r12,r10		/* r12= t[0]+t2[6] = t[0]+t[6]+t[5]*/
	SUB	r10,r12,r10,LSL #1	/* r10= t[0]+t2[5] = t[0]+t[6]-t[5]*/
	ADD	r3, r3, r2		/* r3 = t[0]+t[4]*/
	ADD	r11,r11,r2		/* r11= t[0]+t[7]*/
	STRH	r11,[r0], #2		/* y[0] = t[0]+t[7]*/
	STRH	r12,[r0, #14]		/* y[1] = t[0]+t[6]*/
	STRH	r10,[r0, #30]		/* y[2] = t[0]+t[5]*/
	STRH	r3, [r0, #46]		/* y[3] = t[0]+t[4]*/
	RSB	r3, r3, r2, LSL #1	/* r3 = t[0]*2-(t[0]+t[4])=t[0]-t[4]*/
	RSB	r10,r10,r2, LSL #1	/* r10= t[0]*2-(t[0]+t[5])=t[0]-t[5]*/
	RSB	r12,r12,r2, LSL #1	/* r12= t[0]*2-(t[0]+t[6])=t[0]-t[6]*/
	RSB	r11,r11,r2, LSL #1	/* r1 = t[0]*2-(t[0]+t[7])=t[0]-t[7]*/
	STRH	r3, [r0, #62]		/* y[4] = t[0]-t[4]*/
	STRH	r10,[r0, #78]		/* y[5] = t[0]-t[5]*/
	STRH	r12,[r0, #94]		/* y[6] = t[0]-t[6]*/
	STRH	r11,[r0, #110]		/* y[7] = t[0]-t[7]*/

	MOV	PC,r14
idct2core_down:
	/* r0 =       ogg_int16_t *y      (destination)
	 r1 = const ogg_int16_t *x      (source)*/
	LDRSH	r2, [r1], #16		
	LDR	r12,OC_C4S4
	LDRSH	r11,[r1, #-14]		
	LDR	r3, OC_C7S1
	MUL	r2, r12,r2		
	LDR	r10,OC_C1S7
	MUL	r3, r11,r3		
	MOV	r2, r2, ASR #16		
	MUL	r11,r10,r11		
	ADD	r2, r2, #8		
	MOV	r3, r3, ASR #16		
	MUL	r10,r12,r3		
	MOV	r11,r11,ASR #16		
	MUL	r12,r11,r12		
	MOV	r10,r10,ASR #16		
	ADD	r12,r2,r12,ASR #16	
	ADD	r12,r12,r10		
	SUB	r10,r12,r10,LSL #1	
	ADD	r3, r3, r2		
	ADD	r11,r11,r2		
	MOV	r4, r11,ASR #4
	MOV	r5, r12,ASR #4
	MOV	r6, r10,ASR #4
	MOV	r7, r3, ASR #4
	RSB	r3, r3, r2, LSL #1	
	RSB	r10,r10,r2, LSL #1	
	RSB	r12,r12,r2, LSL #1	
	RSB	r11,r11,r2, LSL #1	
	MOV	r3, r3, ASR #4
	MOV	r10,r10,ASR #4
	MOV	r12,r12,ASR #4
	MOV	r11,r11,ASR #4
	
			
	/*获得dst和stride*/
	ldr     r9, [r13, #128]
	ldr     r2, [r13, #132]
	add     r8, r2, #1
	
	ldrb    r0, [r2]
	add     r4, r4, r0
	/*将各个值降至8bit*/
	usat    r4, #8, r4
	STRB	r4, [r2], r9
	
	ldrb    r0, [r2]
	add     r5, r5, r0
	usat    r5, #8, r5
	STRB	r5, [r2], r9
	
	ldrb    r0, [r2]
	add     r6, r6, r0
	usat    r6, #8, r6
	STRB	r6, [r2], r9
	
	ldrb    r0, [r2]
	add     r7, r7, r0
	usat    r7, #8, r7
	STRB	r7, [r2], r9
	
	ldrb    r0, [r2]
	add     r3, r3, r0
	usat    r3, #8, r3
	STRB	r3, [r2], r9
	
	ldrb    r0, [r2]
	add     r10, r10, r0
	usat    r10, #8, r10
	STRB	r10, [r2], r9
	
		
	ldrb    r0, [r2]
	add     r12, r12, r0
	usat    r12, #8, r12		
	STRB	r12,  [r2], r9
	
	ldrb    r0, [r2]
	add     r11, r11, r0
	usat    r11, #8, r11
	STRB	r11, [r2], r9
			
	str     r8, [r13, #132]
	
	MOV	PC,r14
idct3core:
	LDRSH	r2, [r1], #16		
	LDR	r12,OC_C4S4		
	LDRSH	r3, [r1, #-12]		
	LDR	r10,OC_C6S2		
	MUL	r2, r12,r2		
	LDR	r4, OC_C2S6		
	MUL	r10,r3, r10		
	LDRSH	r11,[r1, #-14]		
	MUL	r3, r4, r3		
	LDR	r4, OC_C7S1		
	LDR	r5, OC_C1S7		
	MOV	r2, r2, ASR #16		
	MUL	r4, r11,r4		
	ADD	r3, r2, r3, ASR #16	
	MUL	r11,r5, r11		
	MOV	r4, r4, ASR #16		
	MUL	r5, r12,r4		
	MOV	r11,r11,ASR #16		
	MUL	r12,r11,r12		

	ADD	r10,r2, r10,ASR #16	
	RSB	r6, r10,r2, LSL #1	
					
	RSB	r2, r3, r2, LSL #1	
	MOV	r12,r12,ASR #16		
	ADD	r5, r12,r5, ASR #16	
	RSB	r12,r5, r12,LSL #1	

	ADD	r11,r3, r11		
	ADD	r5, r10,r5		
	ADD	r12,r6, r12		
	ADD	r4, r2, r4		
	STRH	r11,[r0], #2		
	STRH	r5, [r0, #14]		
	STRH	r12,[r0, #30]		
	STRH	r4, [r0, #46]		

	RSB	r11,r11,r3, LSL #1	
	RSB	r5, r5, r10,LSL #1	
	RSB	r12,r12,r6, LSL #1	
	RSB	r4, r4, r2, LSL #1	
	STRH	r4, [r0, #62]		
	STRH	r12,[r0, #78]		
	STRH	r5, [r0, #94]		
	STRH	r11,[r0, #110]		

	MOV	PC,R14
idct3core_down:
	LDRSH	r2, [r1], #16		
	LDR	r12,OC_C4S4		
	LDRSH	r3, [r1, #-12]		
	LDR	r10,OC_C6S2		
	MUL	r2, r12,r2		
	LDR	r4, OC_C2S6		
	MUL	r10,r3, r10		
	LDRSH	r11,[r1, #-14]		
	MUL	r3, r4, r3		
	LDR	r4, OC_C7S1		
	LDR	r5, OC_C1S7		
	MOV	r2, r2, ASR #16		
	ADD	r2, r2, #8		
	MUL	r4, r11,r4		
	ADD	r3, r2, r3, ASR #16	
	MUL	r11,r5, r11		
	MOV	r4, r4, ASR #16		
	MUL	r5, r12,r4		
	MOV	r11,r11,ASR #16		
	MUL	r12,r11,r12		

	ADD	r10,r2, r10,ASR #16	
	RSB	r6, r10,r2, LSL #1	
					
	RSB	r2, r3, r2, LSL #1	
	MOV	r12,r12,ASR #16		
	ADD	r5, r12,r5, ASR #16	
	RSB	r12,r5, r12,LSL #1	

	ADD	r11,r3, r11		
	ADD	r5, r10,r5		
	ADD	r12,r6, r12		
	ADD	r4, r2, r4		
	RSB	r3, r11,r3, LSL #1	
	RSB	r10,r5, r10,LSL #1	
	RSB	r6, r12,r6, LSL #1	
	RSB	r2, r4, r2, LSL #1	
	MOV	r11,r11,ASR #4
	MOV	r5, r5, ASR #4
	MOV	r12,r12,ASR #4
	MOV	r4, r4, ASR #4
	MOV	r2, r2, ASR #4
	MOV	r6, r6, ASR #4
	MOV	r10,r10,ASR #4
	MOV	r3, r3, ASR #4

		
	/*获得dst和stride*/
	ldr     r9, [r13, #128]
	ldr     r8, [r13, #132]
	add     r7, r8, #1
	
	ldrb    r0, [r8]
	add     r11, r11, r0
	usat    r11, #8, r11
	STRB	r11, [r8], r9
	
	ldrb    r0, [r8]
	add     r5, r5, r0
	usat    r5, #8, r5
	STRB	r5, [r8], r9
	
			
	ldrb    r0, [r8]
	add     r12, r12, r0
	usat    r12, #8, r12		
	STRB	r12,  [r8], r9
	
	ldrb    r0, [r8]
	add     r4, r4, r0
	/*将各个值降至8bit*/
	usat    r4, #8, r4
	STRB	r4, [r8], r9
	
	ldrb    r0, [r8]
	add     r2, r2, r0
	usat    r2, #8, r2
	STRB	r2, [r8], r9
	
	ldrb    r0, [r8]
	add     r6, r6, r0
	usat    r6, #8, r6
	STRB	r6, [r8], r9
		
	ldrb    r0, [r8]
	add     r10, r10, r0
	usat    r10, #8, r10
	STRB	r10, [r8], r9
		
	ldrb    r0, [r8]
	add     r3, r3, r0
	usat    r3, #8, r3
	STRB	r3, [r8], r9

		
	str     r7, [r13, #132]	
	
	MOV	PC,R14

idct4core:

	LDRSH	r2, [r1], #16		
	LDR	r10,OC_C4S4		
	LDRSH	r12,[r1, #-12]		
	LDR	r4, OC_C6S2		
	MUL	r2, r10,r2		
	LDR	r5, OC_C2S6		
	MUL	r4, r12,r4		
	LDRSH	r3, [r1, #-14]		
	MUL	r5, r12,r5		
	LDR	r6, OC_C7S1		
	LDR	r12,OC_C1S7		
	LDRSH	r11,[r1, #-10]		
	MUL	r6, r3, r6		
	LDR	r7, OC_C5S3		
	MUL	r3, r12,r3		
	LDR	r8, OC_C3S5		
	MUL	r7, r11,r7		
	MOV	r2, r2, ASR #16		
	MUL	r11,r8, r11		

	MOV	r6, r6, ASR #16		
	SUB	r7, r6, r7, ASR #16	
	RSB	r6, r7, r6, LSL #1	
	MUL	r6, r10,r6		

	MOV	r3, r3, ASR #16		
	ADD	r11,r3, r11,ASR #16	
	RSB	r3, r11,r3, LSL #1	
	MUL	r3, r10,r3		

	ADD	r4, r2, r4, ASR #16	
	RSB	r10,r4, r2, LSL #1	

	ADD	r5, r2, r5, ASR #16	
	RSB	r2, r5, r2, LSL #1	

	MOV	r3, r3, ASR #16		
	ADD	r6, r3, r6, ASR #16	
	RSB	r3, r6, r3, LSL #1	

	ADD	r11,r5, r11		
	ADD	r6, r4, r6		
	ADD	r3, r10,r3		
	ADD	r7, r2, r7		
	STRH	r11,[r0], #2		
	STRH	r6, [r0, #14]		
	STRH	r3, [r0, #30]		
	STRH	r7, [r0, #46]		

	RSB	r11,r11,r5, LSL #1	
	RSB	r6, r6, r4, LSL #1	
	RSB	r3, r3, r10,LSL #1	
	RSB	r7, r7, r2, LSL #1	
	STRH	r7, [r0, #62]		
	STRH	r3, [r0, #78]		
	STRH	r6, [r0, #94]		
	STRH	r11, [r0, #110]		

	MOV	PC,r14
idct4core_down:
	  
	LDRSH	r2, [r1], #16		
	LDR	r10,OC_C4S4		
	LDRSH	r12,[r1, #-12]		
	LDR	r4, OC_C6S2		
	MUL	r2, r10,r2		
	LDR	r5, OC_C2S6		
	MUL	r4, r12,r4		
	LDRSH	r3, [r1, #-14]		
	MUL	r5, r12,r5		
	LDR	r6, OC_C7S1		
	LDR	r12,OC_C1S7		
	LDRSH	r11,[r1, #-10]		
	MUL	r6, r3, r6		
	LDR	r7, OC_C5S3		
	MUL	r3, r12,r3		
	LDR	r8, OC_C3S5		
	MUL	r7, r11,r7		
	MOV	r2, r2, ASR #16		
	MUL	r11,r8, r11		

	MOV	r6, r6, ASR #16		
	SUB	r7, r6, r7, ASR #16	
	RSB	r6, r7, r6, LSL #1	
	MUL	r6, r10,r6		

	MOV	r3, r3, ASR #16		
	ADD	r11,r3, r11,ASR #16	
	RSB	r3, r11,r3, LSL #1	
	MUL	r3, r10,r3		

	ADD	r4, r2, r4, ASR #16	
	RSB	r10,r4, r2, LSL #1	

	ADD	r5, r2, r5, ASR #16	
	RSB	r2, r5, r2, LSL #1	

	MOV	r3, r3, ASR #16		
	ADD	r6, r3, r6, ASR #16	
	RSB	r3, r6, r3, LSL #1	

	ADD	r5, r5, r11		
	ADD	r4, r4, r6		
	ADD	r10,r10,r3		
	ADD	r2, r2, r7		
	ADD	r2, r2, #8
	ADD	r10,r10,#8
	ADD	r4, r4, #8
	ADD	r5, r5, #8
	SUB	r11,r5, r11,LSL #1	
	SUB	r6, r4, r6, LSL #1	
	SUB	r3, r10,r3, LSL #1	
	SUB	r7, r2, r7, LSL #1	
	MOV	r11,r11,ASR #4
	MOV	r6, r6, ASR #4
	MOV	r3, r3, ASR #4
	MOV	r7, r7, ASR #4
	MOV	r2, r2, ASR #4
	MOV	r10,r10,ASR #4
	MOV	r4, r4, ASR #4
	MOV	r5, r5, ASR #4

	/*获得dst和stride*/
	ldr     r9, [r13, #128]
	ldr     r8, [r13, #132]
	add     r12, r8, #1	
	
		
	ldrb    r0, [r8]
	add     r5, r5, r0
	usat    r5, #8, r5
	STRB	r5, [r8], r9
	
	ldrb    r0, [r8]
	add     r4, r4, r0
	/*将各个值降至8bit*/
	usat    r4, #8, r4
	STRB	r4, [r8], r9

	ldrb    r0, [r8]
	add     r10, r10, r0
	usat    r10, #8, r10
	STRB	r10, [r8], r9
	
			
	ldrb    r0, [r8]
	add     r2, r2, r0
	usat    r2, #8, r2		
	STRB	r2,  [r8], r9
	
	ldrb    r0, [r8]
	add     r7, r7, r0
	usat    r7, #8, r7
	STRB	r7, [r8], r9
	
			
	ldrb    r0, [r8]
	add     r3, r3, r0
	usat    r3, #8, r3
	STRB	r3, [r8], r9
	
	ldrb    r0, [r8]
	add     r6, r6, r0
	usat    r6, #8, r6
	STRB	r6, [r8], r9
			
	ldrb    r0, [r8]
	add     r11, r11, r0
	usat    r11, #8, r11
	STRB	r11, [r8], r9
		
	str     r12, [r13, #132]

	MOV	PC,r14

idct8:
  
	STMFD	r13!,{r4-r11,r14}

	LDRSH	r2, [r1]		
	LDRSH	r6, [r1, #8]		
	LDR	r12,OC_C4S4		
	LDRSH	r4, [r1, #4]		
	ADD	r2, r2, r6		
	SUB	r6, r2, r6, LSL #1	
	MUL	r2, r12,r2		
	LDRSH	r8, [r1, #12]		
	LDR	r7, OC_C6S2		
	MUL	r6, r12,r6		
	LDR	r14,OC_C2S6		
	MUL	r3, r4, r7		
	LDR	r5, OC_C7S1		
	MUL	r4, r14,r4		
	MOV	r3, r3, ASR #16		
	MUL	r14,r8, r14		
	MOV	r4, r4, ASR #16		
	MUL	r8, r7, r8		
	LDR	r7, OC_C1S7		
	SUB	r3, r3, r14,ASR #16	
	LDRSH	r14,[r1, #2]		
	ADD	r4, r4, r8, ASR #16	
	LDRSH	r8, [r1, #14]		
	MUL	r9, r5, r14		
	LDRSH	r10,[r1, #10]		
	MUL	r14,r7, r14		
	MOV	r9, r9, ASR #16		
	MUL	r7, r8, r7		
	MOV	r14,r14,ASR #16		
	MUL	r8, r5, r8		
	LDRSH	r1, [r1, #6]		
	LDR	r5, OC_C3S5		
	LDR	r11,OC_C5S3		
	ADD	r8, r14,r8, ASR #16	
	MUL	r14,r5, r10		
	SUB	r9, r9, r7, ASR #16	
	MUL	r10,r11,r10		
	MOV	r14,r14,ASR #16		
	MUL	r11,r1, r11		
	MOV	r10,r10,ASR #16		
	MUL	r1, r5, r1		
	SUB	r14,r14,r11,ASR #16	
	ADD	r10,r10,r1, ASR #16	


	ADD	r9, r9, r14		
	SUB	r14,r9, r14, LSL #1	
	MUL	r14,r12,r14		


	ADD	r8, r8, r10		
	SUB	r10,r8, r10, LSL #1	
	MUL	r10,r12,r10		


	ADD	r2, r4, r2, ASR #16	
	SUB	r4, r2, r4, LSL #1	


	ADD	r6, r3, r6, ASR #16	
	SUB	r3, r6, r3, LSL #1	

	MOV	r14,r14,ASR #16		
	ADD	r10,r14,r10,ASR #16	
	SUB	r14,r10,r14,LSL #1	

	ADD	r2, r2, r8		
	ADD	r6, r6, r10		
	ADD	r3, r3, r14		
	ADD	r4, r4, r9		
	SUB	r8, r2, r8, LSL #1	
	SUB	r10,r6, r10,LSL #1	
	SUB	r14,r3, r14,LSL #1	
	SUB	r9, r4, r9, LSL #1	
	STRH	r2, [r0], #2		
	STRH	r6, [r0, #14]		
	STRH	r3, [r0, #30]		
	STRH	r4, [r0, #46]		
	STRH	r9, [r0, #62]		
	STRH	r14,[r0, #78]		
	STRH	r10,[r0, #94]		
	STRH	r8, [r0, #110]		

	LDMFD	r13!,{r4-r11,PC}
idct8core:
	  
	STMFD	r13!,{r1,r14}

	LDRSH	r2, [r1]		
	LDRSH	r6, [r1, #8]		
	LDR	r12,OC_C4S4		
	LDRSH	r4, [r1, #4]		
	ADD	r2, r2, r6		
	SUB	r6, r2, r6, LSL #1	
	MUL	r2, r12,r2		
	LDRSH	r8, [r1, #12]		
	LDR	r7, OC_C6S2		
	MUL	r6, r12,r6		
	LDR	r14,OC_C2S6		
	MUL	r3, r4, r7		
	LDR	r5, OC_C7S1		
	MUL	r4, r14,r4		
	MOV	r3, r3, ASR #16		
	MUL	r14,r8, r14		
	MOV	r4, r4, ASR #16		
	MUL	r8, r7, r8		
	LDR	r7, OC_C1S7		
	SUB	r3, r3, r14,ASR #16	
	LDRSH	r14,[r1, #2]		
	ADD	r4, r4, r8, ASR #16	
	LDRSH	r8, [r1, #14]		
	MUL	r9, r5, r14		
	LDRSH	r10,[r1, #10]		
	MUL	r14,r7, r14		
	MOV	r9, r9, ASR #16		
	MUL	r7, r8, r7		
	MOV	r14,r14,ASR #16		
	MUL	r8, r5, r8		
	LDRSH	r1, [r1, #6]		
	LDR	r5, OC_C3S5		
	LDR	r11,OC_C5S3		
	ADD	r8, r14,r8, ASR #16	
	MUL	r14,r5, r10		
	SUB	r9, r9, r7, ASR #16	
	MUL	r10,r11,r10		
	MOV	r14,r14,ASR #16		
	MUL	r11,r1, r11		
	MOV	r10,r10,ASR #16		
	MUL	r1, r5, r1		
	SUB	r14,r14,r11,ASR #16	
	ADD	r10,r10,r1, ASR #16	

	
	ADD	r9, r9, r14		
	SUB	r14,r9, r14, LSL #1	
	MUL	r14,r12,r14		

	
	ADD	r8, r8, r10		
	SUB	r10,r8, r10, LSL #1	
	MUL	r10,r12,r10		

	
	ADD	r2, r4, r2, ASR #16	
	SUB	r4, r2, r4, LSL #1	

	
	ADD	r6, r3, r6, ASR #16	
	SUB	r3, r6, r3, LSL #1	

	
	MOV	r14,r14,ASR #16		
	ADD	r10,r14,r10,ASR #16	
	SUB	r14,r10,r14,LSL #1	

	
	ADD	r2, r2, r8		
	ADD	r6, r6, r10		
	ADD	r3, r3, r14		
	ADD	r4, r4, r9		
	SUB	r8, r2, r8, LSL #1	
	SUB	r10,r6, r10,LSL #1	
	SUB	r14,r3, r14,LSL #1	
	SUB	r9, r4, r9, LSL #1	
	STRH	r2, [r0], #2		
	STRH	r6, [r0, #14]		
	STRH	r3, [r0, #30]		
	STRH	r4, [r0, #46]		
	STRH	r9, [r0, #62]		
	STRH	r14,[r0, #78]		
	STRH	r10,[r0, #94]		
	STRH	r8, [r0, #110]		

	LDMFD	r13!,{r1,PC}
idct8core_down:
	 
	STMFD	r13!,{r1,r14}

	LDRSH	r2, [r1]		
	LDRSH	r6, [r1, #8]		
	LDR	r12,OC_C4S4		
	LDRSH	r4, [r1, #4]		
	ADD	r2, r2, r6		
	SUB	r6, r2, r6, LSL #1	
	MUL	r2, r12,r2		
	LDRSH	r8, [r1, #12]		
	LDR	r7, OC_C6S2		
	MUL	r6, r12,r6		
	LDR	r14,OC_C2S6		
	MUL	r3, r4, r7		
	LDR	r5, OC_C7S1		
	MUL	r4, r14,r4		
	MOV	r3, r3, ASR #16		
	MUL	r14,r8, r14		
	MOV	r4, r4, ASR #16		
	MUL	r8, r7, r8		
	LDR	r7, OC_C1S7		
	SUB	r3, r3, r14,ASR #16	
	LDRSH	r14,[r1, #2]		
	ADD	r4, r4, r8, ASR #16	
	LDRSH	r8, [r1, #14]		
	MUL	r9, r5, r14		
	LDRSH	r10,[r1, #10]		
	MUL	r14,r7, r14		
	MOV	r9, r9, ASR #16		
	MUL	r7, r8, r7		
	MOV	r14,r14,ASR #16		
	MUL	r8, r5, r8		
	LDRSH	r1, [r1, #6]		
	LDR	r5, OC_C3S5		
	LDR	r11,OC_C5S3		
	ADD	r8, r14,r8, ASR #16	
	MUL	r14,r5, r10		
	SUB	r9, r9, r7, ASR #16	
	MUL	r10,r11,r10		
	MOV	r14,r14,ASR #16		
	MUL	r11,r1, r11		
	MOV	r10,r10,ASR #16		
	MUL	r1, r5, r1		
	SUB	r14,r14,r11,ASR #16	
	ADD	r10,r10,r1, ASR #16	


	ADD	r9, r9, r14		
	SUB	r14,r9, r14, LSL #1	
	MUL	r14,r12,r14		


	ADD	r8, r8, r10		
	SUB	r10,r8, r10, LSL #1	
	MUL	r10,r12,r10		

	
	ADD	r2, r4, r2, ASR #16	
	SUB	r4, r2, r4, LSL #1	


	ADD	r6, r3, r6, ASR #16	
	SUB	r3, r6, r3, LSL #1	


	MOV	r14,r14,ASR #16		
	ADD	r10,r14,r10,ASR #16	
	SUB	r14,r10,r14,LSL #1	

	
	ADD	r2, r2, r8		
	ADD	r6, r6, r10		
	ADD	r3, r3, r14		
	ADD	r4, r4, r9
			
	ADD	r2, r2, #8
	ADD	r6, r6, #8
	ADD	r3, r3, #8
	ADD	r4, r4, #8
		
	SUB	r8, r2, r8, LSL #1	
	SUB	r10,r6, r10,LSL #1	
	SUB	r14,r3, r14,LSL #1	
	SUB	r9, r4, r9, LSL #1
		
	MOV	r2, r2, ASR #4
	MOV	r6, r6, ASR #4
	MOV	r3, r3, ASR #4
	MOV	r4, r4, ASR #4
	MOV	r8, r8, ASR #4
	MOV	r10,r10,ASR #4
	MOV	r14,r14,ASR #4
	MOV	r9, r9, ASR #4
		
	/*获得dst和stride*/
	ldr     r7, [r13, #136]
	ldr     r5, [r13, #140]
	add     r11, r5, #1
	
	ldrb    r0, [r5]
	add     r2, r2, r0
	/*将各个值降至8bit*/
	usat    r2, #8, r2
	STRB	r2, [r5], r7
	
	ldrb    r0, [r5]
	add     r6, r6, r0
	usat    r6, #8, r6
	STRB	r6, [r5], r7
	
	ldrb    r0, [r5]
	add     r3, r3, r0
	usat    r3, #8, r3
	STRB	r3, [r5], r7
	
	ldrb    r0, [r5]
	add     r4, r4, r0
	usat    r4, #8, r4
	STRB	r4, [r5], r7
	
	ldrb    r0, [r5]
	add     r9, r9, r0
	usat    r9, #8, r9
	STRB	r9, [r5], r7
	
	ldrb    r0, [r5]
	add     r14, r14, r0
	usat    r14, #8, r14
	STRB	r14, [r5], r7
	
	ldrb    r0, [r5]
	add     r10, r10, r0
	usat    r10, #8, r10
	STRB	r10, [r5], r7
	
	ldrb    r0, [r5]
	add     r8, r8, r0
	usat    r8, #8, r8		
	STRB	r8,  [r5], r7	
		
	str     r11, [r13, #140]
	LDMFD	r13!,{r1,PC}

