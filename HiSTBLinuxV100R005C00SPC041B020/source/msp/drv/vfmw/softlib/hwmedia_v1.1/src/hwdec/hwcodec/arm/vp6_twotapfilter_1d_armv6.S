        .equ DO1STROUNDING, 0
	.global twotap_vp6_asm_h
	.global twotap_vp6_asm_v
	    .type twotap_vp6_asm_h, function
	    .type twotap_vp6_asm_v, function

@twotap_vp6_asm(uint8_t *dst/*align 8*/, uint8_t *src/*align 1*/, int stride, int h, int x, int y);
@
@	const int A=(8-x)*(8-y);
@	const int B=(  x)*(8-y);
@	const int C=(8-x)*(  y);
@	int i;
@
@	int E= B+C;
@	int step= C ? stride : 1;
@	for(i=0; i<h; i++)
@	{
@			dst[0] = ((A*src[0] + E*src[step+0])+32 )>>6;
@			dst[1] = ((A*src[1] + E*src[step+1])+32) >>6;
@			dst[2] = ((A*src[2] + E*src[step+2])+32) >>6;
@			dst[3] = ((A*src[3] + E*src[step+3])+32) >>6;
@			dst[4] = ((A*src[4] + E*src[step+4])+32 )>>6;
@			dst[5] = ((A*src[5] + E*src[step+5])+32) >>6;
@			dst[6] = ((A*src[6] + E*src[step+6])+32) >>6;
@			dst[7] = ((A*src[7] + E*src[step+7])+32) >>6;
@			dst+= stride;
@			src+= stride;
@	}

.text	
twotap_vp6_asm_h:
	stmfd  sp!, {r4-r12,lr}    @   r0:dst, r1:src, r2:stride,r3:filter
	ldr    r6, [r3]         @   r6 = E|A           
	mov    r8, #8              @   r8 = 8 loop times
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
	
	smuad  r9, r3, r6                  @r9 = 3|2 * E|A
	 
	
	ldrb   r3, [r1,#6]
	
	add    r4, r4, #4
	asr    r4, #3
	
	pkhbt  r11, r7, r5, lsl #16        @r11 = 5|4 
	
	smuad  r7, r10, r6                 @r7 = 4|3 * E|A
	
		
	orr    lr, r12, r4, lsl #8         @lr = [][][1][0]
	
	
	ldrb   r4, [r1,#7]
	smuad  r12, r11, r6          @r12 = 5|4 * E|A
	
	pkhbt  r10, r5, r3, lsl #16   @r10 = 6|5
	
	ldrb   r11, [r1,#8]
	
	add    r12, r12, #4
	asr    r12, #3
	
	add    r9, r9, #4
	asr    r9, #3
	
	orr    lr, lr, r9, lsl #16  @lr = [][2][1][0]
	
	add    r7, r7, #4
	asr    r7, #3
		
	smuad  r9, r10, r6          @r9 = 6|5 * E|A 
	pkhbt  r10, r3, r4, lsl #16@r10 = 7|6	 
	
	orr    lr, lr, r7, lsl #24  @lr = [3][2][1][0]
	
	smuad  r3, r10, r6          @r3 = 7|6 * E|A     
	str    lr, [r0]
	add    r9, r9, #4
	asr    r9, #3
	
	pkhbt  r7, r4, r11, lsl #16  @r7 = 8|7 	
	orr    r12, r12, r9, lsl #8  @ r12 = [][][5][4]
	
	add    r3, r3, #4
	asr    r3, #3
	
	smuad  r9, r7, r6            @r9 = 8|7 * E|A
	
	orr    r12, r12, r3, lsl #16 @r12 = [][6][5][4]
	add    r9, r9, #4
	asr    r9, #3
	orr    r12, r12, r9, lsl #24 @r12 = [7][6][5][4]
	str    r12, [r0, #4]
	
	subs   r8, r8, #1
	add    r0, r2
	add    r1, r2
	bne    h_y_loop           
	           
h_y_loop_end:	
	ldmfd sp!,{r4-r12,pc}


twotap_vp6_asm_v:
	stmfd  sp!, {r4-r12,lr}    @   r0:dst, r1:src, r2:stride,r3:filter
	ldr    r6, [r3]         @   r6 = E|A           
	mov    r8, #8              @   r8 = 8 loop times
v_x_loop:	
	ldrb   r3, [r1],r2                 @ r3 = 0 r1 = src + stride
	ldrb   r4, [r1],r2                 @ r4 = 1 r1 = src + 2stride
	ldrb   r5, [r1],r2                 @ r5 = 2 r1 = src + 3stride
	ldrb   r9, [r1],r2                 @ r9 = 3 r1 = src + 4 stride
	
	pkhbt  r10, r3, r4, lsl #16        @r10 = 1|0
	pkhbt  r11, r4, r5, lsl #16        @r11 = 2|1
	
	smuad  r12, r10, r6                @r12 = 1|0 * E|A
	smuad  r4,  r11, r6                @r4 = 2|1 * E|A
	
	ldrb   r7, [r1], r2                @r7 = 4, r1 = src + 5stride
	ldrb   r3, [r1], r2                @r3 = 5, r1 = src + 6stride 
	
	pkhbt  r10, r5, r9, lsl #16         @r10 = 3|2
	add    r12, r12, #4
	asr    r12, #3
	strb   r12, [r0], r2               @r0 = dst +stride
	
	ldrb lr, [r1], r2                  @lr = 6 ,r1 = src +7stride 
	
	smuad  r5, r10, r6                 @r5 = 3|2 * E|A
	
	add    r4, r4, #4
	asr    r4,  #3
	strb   r4, [r0], r2                @r0 = dst + 2stride
	
	pkhbt  r11, r9, r7, lsl #16        @r11=4|3 
	
	add    r5, r5, #4
	
	pkhbt r4, r7, r3, lsl #16          @r4 = 5|4  
	
	asr    r5, #3
	
	smuad  r12, r11, r6                @r12 = 4|3 * E|A	 
		 
	strb   r5, [r0], r2                @r0 = dst + 3stride
	
	pkhbt  r10, r3, lr, lsl #16        @r10 = 6|5
	
	ldrb   r3, [r1], r2                @r3 = 7, r1 = src + 8stride
	add    r12, r12, #4
	
	asr    r12, #3
	ldrb   r5,  [r1]                   @r5 = 8
	
	pkhbt  r11, lr, r3, lsl#16         @r11 = 7|6
	strb   r12, [r0], r2               @r0 = dst +4stride              
	smuad  r7, r10, r6                 @r7 = 6|5 * E|A  
	smuad  r10, r4, r6                 @r10 = 5|4 * E|A
	 
	pkhbt  r12, r3, r5, lsl #16        @r12 = 8|7
	smuad  r9, r11, r6                 @r9 = 7|6 *E|A
	add    r10, r10, #4  
	asr    r10, #3
	strb   r10, [r0], r2               @r0 = dst + 5stride
	smuad  r3, r12, r6                  @r3 = 8|7 * E|A
	add    r7, r7, #4
	asr    r7, #3
	strb   r7, [r0], r2                @r0 = dst + 6stride 
	
	add    r9, r9, #4
	asr    r9, #3
	strb   r9, [r0], r2                @r0 = dst + 7stride
	
	add    r3, r3, #4
	asr    r3, #3
	strb   r3, [r0]
	
	subs  r8, r8, #1
	beq   v_x_loop_end
	
        rsb  r9, r2, r2, lsl #3
	sub  r1, r1, r2, lsl #3
	add  r1, r1, #1
	sub  r0, r0, r9
	add  r0, r0, #1
	b    v_x_loop         
	           
v_x_loop_end:	
	ldmfd sp!,{r4-r12,pc}
