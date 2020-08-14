        .equ DO1STROUNDING, 0
	.global vp6_filter_hv4asm_armv6
	    .type vp6_filter_hv4asm_armv6, function

.text	
@               uint8_t *dst
@               uint8_t *src
@               int stride
@               const int16_t *h_weights
@               const int16_t *v_weights

vp6_filter_hv4asm_armv6:
	stmfd  sp!, {r4-r12,lr}
	@ldr r4, [sp, #40]                     @r0=dst,r1=src,r2=stride,r3=h_weights,r4=v_weights
	push {r0}                              @dst入栈
	sub  sp, sp, #352                      @分配8*11*4空间大小
	ldm  r3, {r7, r8}
	mov r5, #11
	mov r0, sp
	sub r1, r2                             @r1 = src -stride
	add r1, #1                        
h_y_loop:	
	ldrb r3,[r1,#-2]                       @r3=src-1
	ldrb r6,[r1,#-1]                       @r6=src 
	
	pkhbt lr, r3, r6, lsl #16              @lr=0 |-1
	
	ldrb r9,[r1],#2                        @r9=1,r1=src+3 
	ldrb  r10,[r1,#-1]                     @r10=2
		
	pkhbt r3, r6, r9, lsl #16              @r3=1|0	
	pkhbt r6, r9, r10, lsl #16             @r6=2|1
	
	smuad lr, lr, r7                       @lr=0|-1 X 1|0	
	ldrb  r9, [r1], #2                     @r9=3, r1= src+5
	
	smlad r11, r6, r8, lr                  @第一个点
	 
	pkhbt r4, r10, r9, lsl #16             @r4= 3|2
	smuad lr, r3, r7                       @lr = 1|0 X 1|0                    
	
	add   r11, r11, #0x40
	smlad r12, r4, r8, lr                  @第二个点 若2 = 1|0 × 1|0 + 
	usat  r11, #8, r11, asr #7 
	
	ldrb  r3,  [r1, #-1]                   @r3=4
	strh  r11, [r0], #24                   @存储第一个点 ,r0 = t +16
	
	ldrb  r10,  [r1], #2                   @r10=5, r1= src+7
	
	add   r12, r12, #0x40
	pkhbt r9, r9, r3, lsl #16              @r9=4|3  
	usat  r12, #8, r12, asr #7
	
	pkhbt r3, r3, r10, lsl #16            @r3 = 5|4 	
	strh  r12, [r0], #24                     @存储第二个点, r0 = t + 32
	
	smuad r11, r6, r7                      @r11= 2|1 X 1|0
	
	ldrb  r6, [r1, #-1]                    @r6 =6
	smlad lr, r9, r8, r11                  @lr= 2|1 X 1|0 + 4|3 X 2|1   
	
	smuad  r11, r4, r7                     @r11 = 3|2 X 1|0
	add   lr, #0x40
	usat  lr, #8, lr, asr #7
	strh  lr, [r0], #24                     @存储第三个点, r0 = t+ 48
	
	smlad r12, r3, r8, r11
	pkhbt r10, r10, r6, lsl #16            @r10= 6|5
	add   r12, #0x40
	usat  r12, #8, r12, asr #7
	strh  r12, [r0], #24                    @存储第四个点, r0 = t+64
	
	smuad r11, r9, r7                      @r11 = 4|3 X 1|0
	ldrb  r4, [r1], #2                     @ r4= 7 r1 = src+9
	smuad r9, r3, r7                       @r9= 5|4 X 1|0
	smlad lr, r10, r8, r11                 @lr 为第5个点
	pkhbt r3, r6, r4, lsl #16              @r3 = 7|6 
	smlad r12, r3, r8, r9                  @r12 = 5|4 × 1|0 + 7|6 × 3 | 2第6个点
	
	add lr, #0x40
	usat  lr, #8, lr, asr #7
	
	add   r12, #0x40
	strh  lr, [r0], #24                    @存储第5个点, r0 = t+ 80
	
	usat  r12, #8, r12, asr #7
	ldrb  r9, [r1, #-1]                   @r9 = 8
	strh  r12, [r0], #24                    @存储第6个点, r0 = t + 96
	
	pkhbt r6, r4, r9, lsl #16             @r6 = 8|7
	ldrb  r4, [r1]                        @r4 = 9
	
	smuad r11, r10, r7                    @r11=6|5 X 1|0
	pkhbt r12, r9, r4, lsl #16            @r12 = 9|8
	smlad lr, r6, r8, r11                 @lr为第7个点
	
	smuad r4, r3, r7                      @r3=7|6 X 1|0
	
	add   lr, #0x40
	usat  lr, #8, lr, asr #7
	strh  lr, [r0], #24                     @存储第7个点, r0 = t + 112
	
	smlad r11, r12, r8, r4                    @r11为第8个点
	add   r11, #0x40
	usat  r11, #8, r11, asr #7
	strh  r11, [r0]                       @存储第8个点
	  
	subs  r5, r5, #1
	add   r0, #2
	sub   r0, #168
	add   r1, r2
	sub   r1, #8
	bne  h_y_loop
	
	@进入垂直方向插值
	ldr r4, [sp, #396]
	ldr r0, [sp, #352]          @ r0 = dst
	mov r1, sp                  @ r1 = t 
	ldm r4,{r7,r8}
	mov r5, #8	
	mov r3, r8, lsl #16
	pkhtb r6, r3, r7, asr #16            @ r6 = 2|1
	 
	ldr r3, [r1], #4                @ r3 = 0|-8, r1 = t + 4
	ldr r4, [r1], #4                @ r4 = 16|8, r1 = t + 8 
v_y_loop:
		
	smuad r10, r3, r7               @ r10 = 0|-8 * 1|0 
	smuad r12, r4, r6               @r12 = 16|8 * 2|1
	
	ldr r9, [r1], #4                @ r9 = 32|24, r1 = t + 12
	smlad r10, r4, r8, r10          @r10 = 0|-8 * 1|0 +16|8 * 3|2 @第一个点
	           
	smlabt  r12, r7, r3, r12             @r12 = 16|8 *2|1 + 0 * 0  
	ldr r3, [r1], #4                @r3 = 64|48 , r1 = t + 16  
	
	add     r10, #0x40
	usat    r10, #8, r10, asr #7         
	strb    r10, [r0], r2                 @保存第一个点
	
	smlabt  r12, r9, r8, r12             @r12 = 16|8 *2|1 + 0 * 0 + 24 * 3 第二个点
		
	smuad   r10, r4, r7               @第三个点, r10 = 16|8 * 1|0
	
	add     r12, #0x40
	usat    r12, #8, r12, asr #7  
	strb    r12, [r0], r2             @保存第二个点 
	
	smuad   r11 , r9, r6            @第四个点, r11 = 32|24 * 2|1
	smlad   r10, r9, r8, r10        @第三个点，r10 = 16|8 * 1|0 + 32|24 * 3|2 
	
	smlatb  r11, r4,r7, r11
	ldr     r4, [r1], #4            @r4 = 96 | 80 , r1 = t + 20
	
	add     r10, #0x40
	usat    r10, #8, r10, asr #7  
	
	smlabt  r11, r3, r8, r11        @第四个点，r11 = 32|24 * 2|1 + 16 *0 + 48 * 3
	strb    r10, [r0], r2           @保存第三个点
	
	smuad   r12, r9, r7             @第5个点，r12 = 32|24 * 1|0
	
	add     r11, #0x40
	usat    r11, #8, r11, asr #7
	
	smuad   r10, r3, r6            @第6个点，r10 = 64 | 48 * 2|1
	strb    r11, [r0], r2          @保存第四个点
	
	smlad   r12, r3, r8, r12       @第5个点,r12= 32|24 * 1|0 + 64|48 * 3|2
	smlatb  r10, r9, r7, r10       @第6个点, r10 = 64 | 48 * 2|1 + 32 * 0
	
	ldr    r9, [r1], #4            @ r9 = 128 | 112 , r1 =t + 24
	add    r12, #0x40
	usat   r12, #8, r12, asr #7  
	smlabt r10, r4, r8, r10        @第6个点 r10 = 64 | 48 * 2|1 + 32 * 0 + 80* 3
	
	strb   r12, [r0], r2           @保存第5个点
	
	smuad r11, r3, r7              @第7个点 r11 = 64|48 * 1|0
	add    r10, #0x40
	usat   r10, #8, r10, asr #7
	
	smuad  r12, r4, r6             @第8个点 r12 = 96 | 80 * 2 | 1
	strb   r10, [r0], r2           @保存第6个点
	
	smlad  r11, r4, r8, r11        @第7个点 r11 = 64|48 * 1|0 + 96 | 80 * 3| 2 
	smlatb r12, r3, r7, r12        @第8个点 r12 =  96 | 80 * 2 | 1 + 64 * 0
	smlabt r12, r9, r8, r12        @第8个点 r12 =  96 | 80 * 2 | 1 + 64 * 0 + 112 * 3
	
	add     r11, #0x40
	usat    r11, #8, r11, asr #7
	
	add    r12, #0x40
	usat   r12, #8, r12, asr #7 
	
	strb   r11, [r0], r2
	strb   r12, [r0]
	
	rsb    r3, r2, r2, lsl #3
	sub    r0, r3
	add    r0, #1
	
	subs   r5, #1
	ldr r3, [r1], #4                @ r3 = 0|-8, r1 = t + 4
	ldr r4, [r1], #4                @ r4 = 16|8, r1 = t + 8
	bne  v_y_loop
	
	add sp, sp, #356	
	ldmfd sp!,{r4-r12,pc}

