/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#include <config.h>
#include <boot.h>
#include <stdio.h>
#include <cpuinfo.h>
#include <module.h>
#include <irqchip.h>
#include <early_uart.h>
#include <irq.h>
#include <task.h>

#include <asm/io.h>

#define MOULE_NAME "timer: "
#include <debug.h>

#define TIMERx_LOAD                                        0x00
#define TIMERx_VALUE                                       0x04
#define TIMERx_CONTROL                                     0x08
#define TIMERx_CONTROL_SIZE                     (1 << 1)
#define TIMERx_CONFROL_256PRE                   (2 << 2)
#define TIMERx_CONTROL_INTR_EN                  (1 << 5)
#define TIMERx_CONTROL_CYC                      (1 << 6)
#define TIMERx_CONTROL_EN                       (1 << 7)
#define TIMERx_INTR_CLR                                    0x0c

struct timer_ctrl_t {
	int init;
	uint32 base;
	int irqnr;    /* irq id */
	uint32 hz;    /* ticks percent usec */
	uint32 cycle; /* timer source cycle */
};

#if defined(CONFIG_ARCH_HI3716MV310)
static struct timer_ctrl_t __timer_ctrl[3] = {
	{
		.init = 0,
		.base = TIMER0_BASE,
		.irqnr = TIMER0_IRQ_NR,
	}, {
		.init = 0,
		.base = TIMER1_BASE,
		.irqnr = TIMER1_IRQ_NR,
	}, {
		.init = 0,
		.base = TIMER2_BASE,
		.irqnr = TIMER2_IRQ_NR,
	}
};
#else
static struct timer_ctrl_t __timer_ctrl[8] = {
	{
		.init = 0,
		.base = TIMER0_BASE,
		.irqnr = TIMER0_IRQ_NR,
	}, {
		.init = 0,
		.base = TIMER1_BASE,
		.irqnr = TIMER1_IRQ_NR,
	}, {
		.init = 0,
		.base = TIMER2_BASE,
		.irqnr = TIMER2_IRQ_NR,
	}, {
		.init = 0,
		.base = TIMER3_BASE,
		.irqnr = TIMER3_IRQ_NR,
	}, {
		.init = 0,
		.base = TIMER4_BASE,
		.irqnr = TIMER4_IRQ_NR,
	}, {
		.init = 0,
		.base = TIMER5_BASE,
		.irqnr = TIMER5_IRQ_NR,
	}, {
		.init = 0,
		.base = TIMER6_BASE,
		.irqnr = TIMER6_IRQ_NR,
	}, {
		.init = 0,
		.base = TIMER7_BASE,
		.irqnr = TIMER7_IRQ_NR,
	}
};
#endif

#define GET_TIMER_CTRL(_index)   (&__timer_ctrl[_index])

#define TM_READ(_tmr, _a)              readl(_a + (_tmr)->base)
#define TM_WRITE(_tmr, _v, _a)         writel(_v, (_tmr)->base + _a)

/*****************************************************************************/

static int clock_tick_timer = -1;
/*****************************************************************************/

void clock_tick_init(int timer)
{
	struct timer_ctrl_t *tmr;

	clock_tick_timer = timer;

	tmr = GET_TIMER_CTRL(clock_tick_timer);

	ASSERT(tmr->init == 0);
	/* fast timer */
	tmr->hz = get_timer_clock() / 1000000;
	tmr->cycle = 0xFFFFFFFF;
	tmr->init = 1;

	TM_WRITE(tmr, 0, TIMERx_CONTROL);
	TM_WRITE(tmr, tmr->cycle, TIMERx_LOAD);
	TM_WRITE(tmr,
		 (TIMERx_CONTROL_SIZE | TIMERx_CONTROL_CYC
		  | TIMERx_CONTROL_EN),
		 TIMERx_CONTROL);

	/* slow timer */
	tmr++;
	ASSERT(tmr->init == 0);

	tmr->hz = get_timer_clock() / (1000 * 256);
	tmr->cycle =0xFFFFFFFF;
	tmr->init = 1;

	TM_WRITE(tmr, 0, TIMERx_CONTROL);
	TM_WRITE(tmr, tmr->cycle, TIMERx_LOAD);
	TM_WRITE(tmr,
		 (TIMERx_CONTROL_SIZE | TIMERx_CONTROL_CYC
		  | TIMERx_CONFROL_256PRE | TIMERx_CONTROL_EN),
		 TIMERx_CONTROL);
}
/*****************************************************************************/

static void clock_source_irq_handle(void *arg)
{
	struct timer_ctrl_t *tmr = (struct timer_ctrl_t *)arg;
	TM_WRITE(tmr, 1, TIMERx_INTR_CLR);
	task_loop();
}
/*****************************************************************************/

void clock_source_init(int timer, int msec)
{
	struct timer_ctrl_t *tmr = GET_TIMER_CTRL(timer);

	ASSERT(tmr->init == 0);

	tmr->hz = get_timer_clock() / 1000000;
	tmr->cycle = msec * tmr->hz * 1000;
	tmr->init = 1;

	TM_WRITE(tmr, 0, TIMERx_CONTROL);
	TM_WRITE(tmr, tmr->cycle, TIMERx_LOAD);
	TM_WRITE(tmr, 1, TIMERx_INTR_CLR);
	TM_WRITE(tmr,
		 (TIMERx_CONTROL_SIZE | TIMERx_CONTROL_CYC
		  | TIMERx_CONTROL_INTR_EN | TIMERx_CONTROL_EN),
		 TIMERx_CONTROL);

	irq_connect(tmr->irqnr, clock_source_irq_handle, (void *)tmr);
}
/*****************************************************************************/

unsigned int get_timer_value(void)
{
	struct timer_ctrl_t *tmr = GET_TIMER_CTRL(clock_tick_timer);
	return TM_READ(tmr, TIMERx_VALUE);
}
/*****************************************************************************/

unsigned int get_ticks(void)
{
	struct timer_ctrl_t *tmr = GET_TIMER_CTRL(clock_tick_timer);
	uint32 ticks = TM_READ(tmr, TIMERx_VALUE);

	return ((0xFFFFFFFF - ticks) / tmr->hz);
}
/*****************************************************************************/

unsigned int get_ms_ticks(void)
{
	struct timer_ctrl_t *tmr = GET_TIMER_CTRL(clock_tick_timer + 1);
	uint32 ticks = TM_READ(tmr, TIMERx_VALUE);

	return ((0xFFFFFFFF - ticks) / tmr->hz);
}
/*****************************************************************************/

unsigned int sys_now(void)
{
	return get_ms_ticks();
}
/*****************************************************************************/

unsigned int do_gettime(unsigned long *sec, unsigned short *msec,
			unsigned short *usec)
{
	unsigned int ticks = get_ticks();
	unsigned int ulapsed;

	if (usec)
		*usec = ticks % 1000;

	ulapsed = ticks / 1000;

	if (msec)
		*msec = ulapsed % 1000;

	ulapsed = ulapsed / 1000;

	if (sec)
		*sec = ulapsed;

	return ticks;
}
/*****************************************************************************/

static void __udelay(unsigned int usec)
{
	uint64 tmo;
	uint32 start = get_ticks();
	uint64 end = (uint64)(start + usec);
next:
	tmo = (uint64)get_ticks();

	if ((uint32)tmo < start)
		tmo += 0xFFFFFFFFULL;

	if (tmo < end)
		goto next;
}
/*****************************************************************************/

void udelay(unsigned int usec)
{
	unsigned int delay = 1000000;

	while (usec) {
		if (usec < delay)
			delay = usec;
		__udelay(delay);
		usec -= delay;
	}
}
/*****************************************************************************/

void mdelay(unsigned int msec)
{
	unsigned int delay = 1000;

	while (msec) {
		if (msec < delay)
			delay = msec;
		__udelay(delay * 1000);
		msec -= delay;
	}
}
