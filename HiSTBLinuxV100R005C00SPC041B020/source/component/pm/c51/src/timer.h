#ifndef __TIMER_H__
#define __TIMER_H__


/* function */
extern void timer_resetParam(void);
extern void TIMER_Enable(void);
extern void TIMER_Disable(void);
extern void TIMER_Init(void);
extern void timer_getParam(void);
extern void TIMER_Isr(void);
extern void do_wdgon(void);
//extern void suspend_start(void);
//extern void suspend_end(void);
//extern HI_U32 time_suspend_start;
//extern HI_U32 time_suspend_end;
//extern HI_VOID TIMER_GetTime(HI_U32* u32Time);

#endif