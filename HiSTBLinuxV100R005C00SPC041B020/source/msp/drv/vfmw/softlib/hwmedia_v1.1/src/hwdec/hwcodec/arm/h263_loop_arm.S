#include "asm.S"

.text
function h263_h_lf_arm, export=1
            push   {r3, r4-r12, lr}            
            mov r3, #8    
            lsl r12, r2, #1
            mov r11, r0
            
h_filter_loop:                                                   
            ldrb r6, [r11]       
            ldrb r5, [r11, #-1] 
            ldrb r4, [r11, #-2]
            ldrb r7, [r11, #1]
            
            rsb r8, r5, r6
            add r8, r4, r8, lsl #2     
            rsb r9, r7, r8       
                                                      
            asr r8, r9, #31
            eor r9, r8
            rsb r9, r8, r9
            asr r9, r9, #3 
            
            cmp r9, r12
            bge h_loop
            mov r10, r9
            cmp r9, r2
            rsbgt r10, r9, r12         
            eor r10, r8
            rsb r10, r8, r10    
                         
            add r5, r5, r10   
            rsb r6, r10, r6  
            usat16 r5, #8, r5
            usat16 r6, #8, r6
            strb r5, [r11, #-1]  
            strb r6, [r11]       
              
            asr r9, r10, #31
            eor r10, r9
            rsb r9, r9, r10
                     
            cmp r9, #1
            bls h_loop
            asr r9, r9, #1 
            
            rsb r10, r7, r4
            asr r8, r10, #31
            eor r10, r10, r8
            rsb r10, r8, r10 
            asr r10, r10, #2 
            cmp r10, r9
            movgt r10, r9
            
            eor r10,r8
            rsb r10,r8,r10
            
            rsb r6, r10, r4  
            strb r6, [r11, #-2]  
            
            add r5, r10, r7          
            strb r5, [r11, #1] 
h_loop:    
            subs r3, r3, #1              
            add r11, r11, r1                     
            bne h_filter_loop
            pop    {r3, r4-r12, pc}
            .endfunc
            
function h263_v_lf_arm, export=1
            push   {r3, r4-r12, lr}            
            mov r3, #8    
            lsl r12, r2, #1
            mov r11, r0
            
v_filter_loop:                                          
            ldrb r6, [r11]                                 
            ldrb r5, [r11, -r1]
            ldrb r7, [r11, r1]   
            ldrb r4, [r11, -r1, lsl #1]      
                    
            rsb r8, r5, r6     //r8=(C-B)
            rsb r9, r7, r4     //r9=(A-D)    
            add r9, r9, r8, lsl #2  //r9= (A-D + 4(C-B)) 
                
            asr r8, r9, #31
            eor r9, r8
            rsb r9, r8, r9
            asr r9, r9, #3 
            
            cmp r9, r12
            bge v_loop
            mov r10, r9    //r10=d
            cmp r9, r2
            rsbgt r10, r9, r12                                          
            eor r10, r8
            rsb r10, r8, r10    //r10=d1 
                        
            add r5, r5, r10    
            rsb r6, r10, r6  
            usat16 r5, #8, r5
            usat16 r6, #8, r6
            strb r5, [r11, -r1]
            strb r6, [r11]
                       
            asr r9, r10, #31
            eor r10, r9
            rsb r9, r9, r10
            
            cmp r9, #1
            bls v_loop
            asr r9, r9, #1
                   
            rsb r10, r7, r4
            asr r8, r10, #31
            eor r10, r10, r8
            rsb r10, r8, r10 
            asr r10, r10, #2 
            cmp r10, r9
            movgt r10, r9
            
            eor r10,r8
            rsb r10,r8,r10   
            
            rsb r6, r10, r4 
            strb r6, [r11, -r1, lsl #1]  
            add r5, r10, r7                                       
            strb r5, [r11, r1] 
v_loop:      
            subs r3, r3, #1                                  
            add r11, r11, #1
            bne v_filter_loop
            pop    {r3, r4-r12, pc}
	    .endfunc