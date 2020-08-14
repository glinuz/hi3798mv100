        .equ DO1STROUNDING, 0
	.global vp6_filter_hv4asm_h
	    .type vp6_filter_hv4asm_h, function
	.global vp6_filter_hv4asm_v
            .type vp6_filter_hv4asm_v, function 

.text	
vp6_filter_hv4asm_h:
	stmfd  sp!, {r4-r12,lr}
	ldr r4, [sp, #40]                     @r0=dst,r1=src,r2=stride,r3=1,r4=weights
	add r1, #1                          @r1=src+1  
	ldm r4,{r7, r8}                         @r7=1|0 r8=3|2
	mov r5, #8
	
h_y_loop:	
	ldrb r3,[r1,#-2]                       @r3=src-1
	ldrb r6,[r1,#-1]                       @r6=src 
	pkhbt lr, r3, r6, lsl #16              @lr=0|-1
	ldrb r9,[r1],#2                        @r9=1,r1=src+3 
	ldrb  r10,[r1,#-1]                     @r10=2	
	pkhbt r3, r6, r9, lsl #16              @r3=1|0
	pkhbt r6, r9, r10, lsl #16             @r6=2|1
	
	smuad lr, lr, r7                       @lr=0|-1 X 1|0	
	ldrb  r9, [r1], #2                     @r9=3, r1= src+5
	
	smlad r11, r6, r8, lr                  @第一个点
	 
	pkhbt r4, r10, r9, lsl #16             @r4= 3|2
	smuad lr, r3, r7                       @第二个点 lr = 1|0 X 1|0                    
	
	add   r11, r11, #0x40
	smlad r12, r4, r8, lr                  @第二个点
	usat  r11, #8, r11, asr #7 
	
	ldrb  r3,  [r1, #-1]                   @r3=4
	strb  r11, [r0]                        @存储第一个点
	
	ldrb  r10,  [r1], #2                   @r10=5, r1= src+7
	
	add   r12, r12, #0x40
	pkhbt r9, r9, r3, lsl #16              @r9=4|3  
	usat  r12, #8, r12, asr #7
	
	pkhbt r3, r3, r10, lsl #16            @r3 = 5|4 	
	strb  r12, [r0,#1]                     @存储第二个点
	
	smuad r11, r6, r7                      @r11= 2|1 X 1|0
	
	ldrb  r6, [r1, #-1]                    @r6 =6
	smlad lr, r9, r8, r11                  @lr= 2|1 X 1|0 + 4|3 X 2|1   
	
	smuad  r11, r4, r7                     @r11 = 3|2 X 1|0
	add   lr, #0x40
	usat  lr, #8, lr, asr #7
	strb  lr, [r0, #2]                     @存储第三个点
	
	smlad r12, r3, r8, r11
	pkhbt r10, r10, r6, lsl #16            @r10= 6|5
	add   r12, #0x40
	usat  r12, #8, r12, asr #7
	strb  r12, [r0, #3]                    @存储第四个点
	
	smuad r11, r9, r7                      @r11 = 4|3 X 1|0
	ldrb  r4, [r1], #2                     @ r4= 7 r1 = src+9
	smuad r9, r3, r7                       @r9=5|4 X 1|0
	smlad lr, r10, r8, r11                 @lr 为第5个点
	pkhbt r3, r6, r4, lsl #16              @r3 = 7|6 
	smlad r12, r3, r8, r9                  @r12 =第6个点
	
	add lr, #0x40
	usat  lr, #8, lr, asr #7
	
	add   r12, #0x40
	strb  lr, [r0, #4]                    @存储第5个点
	
	usat  r12, #8, r12, asr #7
	ldrb  r9, [r1, #-1]                   @r9 = 8
	strb  r12, [r0, #5]                    @存储第6个点
	
	pkhbt r6, r4, r9, lsl #16             @r6 = 8|7
	ldrb  r4, [r1]                        @r4 = 9
	
	smuad r11, r10, r7                    @r11=6|5 X 1|0
	pkhbt r12, r9, r4, lsl #16            @r12 = 9|8
	smlad lr, r6, r8, r11                 @lr为第7个点
	
	smuad r4, r3, r7                      @r3=7|6 X 1|0
	
	add   lr, #0x40
	usat  lr, #8, lr, asr #7
	strb  lr, [r0, #6]                     @存储第7个点
	
	smlad r11, r12, r8, r4                    @r11为第8个点
	add   r11, #0x40
	usat  r11, #8, r11, asr #7
	strb  r11, [r0, #7]                     @存储第8个点
	  
	subs  r5, r5, #1
	add   r0, r2
	add   r1, r2
	sub   r1, #8
	bne  h_y_loop
h_y_loop_end:	
	ldmfd sp!,{r4-r12,pc}

@------------------------------------
@ r0    uint8_t *dst,
@ r1    uint8_t *src,
@ r2    int stride
@ r3    int delta,
@ stack const int16_t *weights
@------------------------------------	
vp6_filter_hv4asm_v:
	stmfd  sp!, {r4-r12,lr}
	ldr r4, [sp, #40]                     @r0=dst,r1=src,r2=stride,r3=1,r4=weights
	sub r1, r2                            @r1=src-stride  
	ldm r4,{r7, r8}                         @r7=1|0 r8=3|2
	mov r5, #8
	
v_y_loop:	
	ldrb r3,[r1], r2                       @r3=-1,r1=src
	ldrb r6,[r1], r2                       @r6=0 ,r1=src+stride
	pkhbt lr, r3, r6, lsl #16              @lr=0|-1

	ldrb  r9,[r1],r2                        @r9=1,r1=src+2stride 
	ldrb  r10,[r1],r2                      @r10=2,r1=src+3stride	

	pkhbt r3, r6, r9, lsl #16              @r3=1|0
	pkhbt r6, r9, r10, lsl #16             @r6=2|1
	
	smuad lr, lr, r7                       @lr=0|-1 * 1|0	
	ldrb  r9, [r1], r2                     @r9=3, r1= src+4stride
	
	smlad r11, r6, r8, lr                  @第一个点 r11 = 0|-1 * 1|0 + 2|1 * 3|2
	 
	pkhbt r4, r10, r9, lsl #16             @r4= 3|2
	smuad lr, r3, r7                       @lr = 1|0 X 1|0                    
	
	add   r11, r11, #0x40
	smlad r12, r4, r8, lr                  @第二个点 r12 = 1|0 X 1|0 + 3|2  * 3 | 2 
	usat  r11, #8, r11, asr #7 
	
	ldrb  r3,  [r1], r2                   @r3=4, r1=src+5stride
	strb  r11, [r0],r2                        @存储第一个点 , r0 = dst + stride
	
	ldrb  r10,  [r1], r2                   @r10=5, r1= src+6stride
	
	add   r12, r12, #0x40
	pkhbt r9, r9, r3, lsl #16              @r9=4|3  
	usat  r12, #8, r12, asr #7
	
	pkhbt r3, r3, r10, lsl #16            @r3 = 5|4 	
	strb  r12, [r0], r2                     @存储第二个点, r0 = dst + 2stride
	
	smuad r11, r6, r7                      @r11= 2|1 X 1|0
	
	ldrb  r6, [r1],r2                    @r6 =6,r1=src + 7stride
	smlad lr, r9, r8, r11                  @lr= 2|1 X 1|0 + 4|3 X 2|1   
	
	smuad  r11, r4, r7                     @r11 = 3|2 X 1|0
	add   lr, #0x40
	usat  lr, #8, lr, asr #7
	strb  lr, [r0], r2                     @存储第三个点 ,r0 = dst + 3stride
	
	smlad r12, r3, r8, r11
	pkhbt r10, r10, r6, lsl #16            @r10= 6|5
	add   r12, #0x40
	usat  r12, #8, r12, asr #7
	strb  r12, [r0], r2                    @存储第四个点, r0 = dst + 4stride
	
	smuad r11, r9, r7                      @r11 = 4|3 X 1|0
	ldrb  r4, [r1], r2                     @ r4= 7 r1 = src+8stride
	smuad r9, r3, r7                       @r9=5|4 X 1|0
	smlad lr, r10, r8, r11                 @lr 为第5个点
	pkhbt r3, r6, r4, lsl #16              @r3 = 7|6 
	smlad r12, r3, r8, r9                  @r12 =第6个点
	
	add lr, #0x40
	usat  lr, #8, lr, asr #7
	
	add   r12, #0x40
	strb  lr, [r0], r2                    @存储第5个点 , r0 = dst + 5stride
	
	usat  r12, #8, r12, asr #7
	ldrb  r9, [r1],r2                   @r9 = 8,r1=src+9stride
	strb  r12, [r0], r2                    @存储第6个点, r0 = dst + 6stride
	
	pkhbt r6, r4, r9, lsl #16             @r6 = 8|7
	ldrb  r4, [r1]                        @r4 = 9 ,r1 = src+9stride
	
	smuad r11, r10, r7                    @r11=6|5 X 1|0
	pkhbt r12, r9, r4, lsl #16            @r12 = 9|8
	smlad lr, r6, r8, r11                 @lr为第7个点
	
	smuad r4, r3, r7                      @r3=7|6 X 1|0
	
	add   lr, #0x40
	usat  lr, #8, lr, asr #7
	strb  lr, [r0], r2                     @存储第7个点, r0 = dst + 7stride
	
	smlad r11, r12, r8, r4                    @r11为第8个点
	add   r11, #0x40
	usat  r11, #8, r11, asr #7
	strb  r11, [r0]                     @存储第8个点
	
	subs  r5, r5, #1
	
	add r4, r2, r2, lsl #3
	add r4, r4, r2
	rsb r3, r2, r2, lsl #3
	  
	sub r0, r0, r3
	sub r1, r1, r4
	
	add r0, r0, #1
	add r1, r1, #1
	  
	bne  v_y_loop
v_y_loop_end:	
	ldmfd sp!,{r4-r12,pc}