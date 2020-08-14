#ifndef SP804_TIMER_H
#define SP804_TIMER_H

typedef void (* timer_callback_func)(void);

void sp804_init(timer_callback_func func);

void sp804_set_nextevent(unsigned long next);

unsigned long long sp804_clocksource_gettime();

unsigned long usec_to_clocktime(unsigned long us);

#define TIMER_CLK_RATE	24000000
#define TIMER1_IRQ	50
#define TIMER3_IRQ	88

#define TIMER_1_BASE	0xf8008100
#define TIMER_3_BASE	0xf8009100

#define TIMER_LOAD	0x00			/* ACVR rw */
#define TIMER_VALUE	0x04			/* ACVR ro */
#define TIMER_CTRL	0x08			/* ACVR rw */
#define TIMER_CTRL_ONESHOT	(1 << 0)	/*  CVR */
#define TIMER_CTRL_32BIT	(1 << 0)	/*  CVR */
#define TIMER_CTRL_64BIT	(1 << 1)	/*  CVR */
#define TIMER_CTRL_DIV1		(0 << 2)	/* ACVR */
#define TIMER_CTRL_DIV16	(1 << 2)	/* ACVR */
#define TIMER_CTRL_DIV256	(2 << 2)	/* ACVR */
#define TIMER_CTRL_IE		(1 << 5)	/*   VR */
#define TIMER_CTRL_PERIODIC	(1 << 6)	/* ACVR */
#define TIMER_CTRL_ENABLE	(1 << 7)	/* ACVR */

#define TIMER_INTCLR	0x0c			/* ACVR wo */
#define TIMER_RIS	0x10			/*  CVR ro */
#define TIMER_MIS	0x14			/*  CVR ro */
#define TIMER_BGLOAD	0x18			/*  CVR rw */
#define TIMER_LOCK	0x1c			
#define TIMER_HIGH_LOAD	0x20
#define TIMER_HIGH_VALUE	0x24
#define TIMER_HIGH_BGLOAD	0x38


static inline unsigned int readl(const volatile void *addr)
{
	return *(const volatile unsigned int*)addr;
}

static inline writel(unsigned int value, volatile void *addr)
{
	*(volatile unsigned int *)addr = value;
}

#endif
