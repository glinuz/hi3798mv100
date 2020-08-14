#ifndef __IR_H__
#define __IR_H__

extern void IR_Isr(void);
extern void ir_resetParam(void);
extern void ir_getParam(void);
extern void IR_Init(void);

//extern void ir_enable(void); 
extern void IR_Disable(void); 
extern void IR_Start(void); 

#endif

