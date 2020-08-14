        .equ DO1STROUNDING, 0
	.global vp6_filter_diag2_asm
	    .type vp6_filter_diag2_asm, function

@vp6_filter_diag2_asm(uint8_t *dst, uint8_t *src, int stride, uint16_t* h_weight, uint16_t* v_weight);
@
@    int32_t tmp[128];
@    uint32_t *t = tmp + 32;
@    twotap_vp6_h((uint8_t *)t, src, stride, 9, h_weight);
@    twotap_vp6_v(dst, (uint8_t *)t, stride, 8,v_weight);
@
.text	
vp6_filter_diag2_asm:
	stmfd  sp!, {r4-r12,lr}    @   r0:dst, r1:src, r2:stride, r3:filter
	ldr    r6, [r3]         @   r6 = E|A
	sub    sp, sp, #288      
	mov    lr, sp     
	mov    r8, #9              @   r8 = 8 loop times
h_y_loop:	
	ldrb   r3, [r1]
	ldrb   r4, [r1,#1]
	ldrb   r5, [r1,#2]
	ldrb   r9, [r1,#3]
	pkhbt  r10, r3, r4, lsl #16        @r10 = 1|0
	pkhbt  r11, r4, r5, lsl #16        @r11 = 2|1
	
	smuad  r12, r10, r6                @r12 = 1|0 * E|A

	ldrb   r7, [r1,#4]
	
	smuad  r4,  r11, r6                @r4 = 2|1 * E|A
	
	pkhbt  r3, r5, r9, lsl #16         @r3 = 3|2
	pkhbt  r10, r9, r7, lsl #16        @r10 = 4|3	
	
	ldrb   r5, [r1,#5]
	
	add    r12, r12,#4
	asr    r12, #3
	
	strh   r12, [lr], #20             @lr = tmp + tmpstride
	
	smuad  r9, r3, r6                  @r9 = 3|2 * E|A
	 
	
	ldrb   r3, [r1,#6]
	
	add    r4, r4, #4
	asr    r4, #3
	
	strh   r4, [lr], #20             @lr = tmp + 2tmpstride
	
	pkhbt  r11, r7, r5, lsl #16        @r11 = 5|4 
	
	smuad  r7, r10, r6                 @r7 = 4|3 * E|A
		
	ldrb   r4, [r1,#7]
	smuad  r12, r11, r6          @r12 = 5|4 * E|A
	
	pkhbt  r10, r5, r3, lsl #16   @r10 = 6|5
	
	ldrb   r11, [r1,#8]
		
	add    r9, r9, #4
	asr    r9, #3
	strh   r9, [lr], #20             @lr = tmp + 3tmpstride
	
	add    r7, r7, #4
	asr    r7, #3
		
	strh   r7, [lr], #20             @lr = tmp + 5tmpstride	
		
	add    r12, r12, #4
	asr    r12, #3
	
	strh   r12, [lr], #20             @lr = tmp + 4tmpstride
		
	smuad  r9, r10, r6          @r9 = 6|5 * E|A 
	pkhbt  r10, r3, r4, lsl #16@r10 = 7|6	 
	
	
	smuad  r3, r10, r6          @r3 = 7|6 * E|A     
	
	add    r9, r9, #4
	asr    r9, #3
	
	strh   r9, [lr], #20             @lr = tmp + 6tmpstride
	
	pkhbt  r7, r4, r11, lsl #16  @r7 = 8|7 	
	
	add    r3, r3, #4
	asr    r3, #3
	
	strh   r3, [lr], #20             @lr = tmp + 7tmpstride
	
	smuad  r9, r7, r6            @r9 = 8|7 * E|A
	

	add    r9, r9, #4
	asr    r9, #3

	strh   r9, [lr]             @lr = tmp + 7tmpstride
	
	
	subs   r8, r8, #1
	add    r1, r2
	sub    lr, lr, #140
	add    lr, lr, #2
	bne    h_y_loop

h_y_loop_end:
	ldr   r3, [sp, #328]	
	ldr    r6, [r3]         @   r6 = E|A    
	mov    lr, sp     
	mov    r8, #8              @   r8 = 8 loop times 
	           
	           
v_y_loop:

	ldr r1, [lr], #4           @r1 = 1|0 lr = tmp + 4
	ldr r3, [lr], #4           @r3 = 3|2 lr = tmp + 8
	ldr r4, [lr], #4           @r4 = 5|4 lr = tmp +12
	 	
	smuad r5, r1, r6           @r5 = 1|0 *E|A  
	
	lsr  r1, #16
	pkhbt r7, r1, r3, lsl #16  @r7 = 2|1
	
	add r5, r5, #4 
	smuad r9, r7, r6           @r9 = 2|1 * E|A
	asr r5, #3
	
	strb r5, [r0], r2          @store 0 r0=dst+stride
	
	smuad r10, r3, r6          @r10 = 3|2 * E|A
	lsr r3, #16
	add r9, #4
	asr r9, #3
	
	strb r9, [r0], r2          @store 1 r0=dst+2stride
	
	ldr  r7, [lr], #4          @r7 = 7|6 lr = tmp +16
	
	pkhbt r11, r3, r4, lsl #16 @ r11 = 4|3
	
	smuad r12, r11, r6         @r12 = 4|3 * E|A
	
	add r10, r10, #4
	
	ldr  r11, [lr], #4          @r11 = 9|8 lr = tmp +20
	
	asr r10, #3
	strb r10, [r0], r2          @store 2 r0=dst+3stride
	
	smuad r9, r4, r6           @r9 = 5|4 * E|A
	add r12, r12, #4
	asr r12, #3
	
	strb r12, [r0], r2          @store 0 r0=dst+4stride
	
	lsr   r4, #16
	pkhbt r10, r4, r7, lsl #16   @ r10 = 6|5	 
	
	smuad r4, r10, r6            @r4 = 6|5 * E|A
	add r9, r9, #4
	asr r9, #3                   
	
	strb r9, [r0], r2          @store 0 r0=dst+5stride
	
	smuad r12, r7, r6            @r12 = 7|6 *E|A
	
	lsr   r7, #16
	pkhbt r5, r7, r11, lsl #16   @r5 = 8|7
	add  r4, #4
	asr  r4, #3
	
	strb r4, [r0], r2          @store 0 r0=dst+6stride
	
	smuad r7, r5, r6             @r7 = 8|7 * E|A
	
	add  r12, r12, #4
	
	asr  r12, #3
	
	strb r12, [r0], r2          @store 0 r0=dst+7stride
	
	add  r7, r7, #4
	asr  r7, #3
	
	strb r7, [r0], r2          @store 0 r0=dst+8stride
	
	subs r8, #1
	beq  v_y_loop_end
	sub r0, r0, r2, lsl #3
	add r0, r0, #1
	b  v_y_loop   
		
v_y_loop_end:
	add    sp, sp, #288 	
	ldmfd sp!,{r4-r12,pc}