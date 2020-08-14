 	.equ DO1STROUNDING, 0
	.global vp56_edge_filter_armv6
	    .type vp56_edge_filter_armv6, function

.text	
vp56_edge_filter_armv6:
	push {r4,r5-r12,lr}
	
	mov r4,#12
	mov r10,r0
	mov r11,r1 

	eor r5, r5, r5
	sub r5, r5, r11
		
process_loop:	
	ldrb r7,[r10]	
	ldrb r6,[r10,r5,lsl #1]
	
	mov r12, r7
	ldrb r8,[r10,r5]
	ldrb r9,[r10,r11]
	
	sub r7,r7,r8
	add r7,r7 ,lsl #1
	add r6,r6,r7
	rsb r9,r9,#4
	add r6,r6,r9
	asr r6,#3

	mov r0, r6
	mov r1, r6, asr #31
	eor r0, r0, r1
	sub r0, r0, r1
	
	cmp r0, r3, lsl #1
	bge loop_end
	sub r0, r0, r3
	
	mov r1, r0, asr #31
	eor r0, r0, r1
	sub r0, r0, r1
	
	rsb r0, r0, r3
	
	cmp r6, #0
	rsblt r0, r0, #0
	
adjust_end:		
	add r9,r8,r0

	usat r9,#8,r9

	strb r9,[r10,-r11]
	
	sub r12,r12,r0

	usat r12, #8, r12 
		
	strb r12,[r10]
loop_end:	
	add r10,r10,r2
	subs r4,#1
	bne  process_loop
	pop {r4,r5-r12,pc}
    
