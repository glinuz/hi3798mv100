/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.6.13
 *
******************************************************************************/

#define pr_fmt(fmt) "hisp804: " fmt

#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/clockchips.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/percpu.h>
#include <linux/clkdev.h>

#include <mach/hardware.h>
#include <mach/platform.h>
#include <mach/irqs.h>
#include <asm/hardware/timer-sp.h>
#include <asm/mach/time.h>
#include <asm/hardware/arm_timer.h>
#include <asm/smp_twd.h>
#include <linux/sched_clock.h>
#include <linux/clk-provider.h>
#include <linux/hikapi.h>

#if !defined(CONFIG_LOCAL_TIMERS)
#  undef CONFIG_HAVE_ARM_LOCAL_TIMER_HI3796MV2X
#  undef CONFIG_HAVE_SP804_LOCAL_TIMER_HI3796MV2X
#endif

/*****************************************************************************/

struct hi_timer_t {
	int init;          /* timer initialize times */
	int index;         /* timer index */
	char *name;
	unsigned int addr; /* timer control base address */
	struct irqaction irq;
	void *priv;
};

#define HI_TIMER(_index, _irq, _addr) { \
	.init = 0, \
	.index = _index, \
	.name = "timer"#_index, \
	.addr = IO_ADDRESS(_addr), \
	.priv = NULL, \
	.irq.irq  = _irq,\
	.irq.name = "timer"#_index, \
	.irq.flags = (IRQF_TIMER | IRQF_NOBALANCING | IRQF_PERCPU), \
}

static struct hi_timer_t hi_timer[] = {
	HI_TIMER(0, INTNR_TIMER_0_1, REG_BASE_TIMER01),
	HI_TIMER(1, INTNR_TIMER_0_1, REG_BASE_TIMER01 + 0x20),
	HI_TIMER(2, INTNR_TIMER_2, REG_BASE_TIMER23),
	HI_TIMER(3, INTNR_TIMER_3, REG_BASE_TIMER23 + 0x20),
	HI_TIMER(4, INTNR_TIMER_4, REG_BASE_TIMER45),
	HI_TIMER(5, INTNR_TIMER_5, REG_BASE_TIMER45 + 0x20),
	HI_TIMER(6, INTNR_TIMER_6, REG_BASE_TIMER67),
	HI_TIMER(7, INTNR_TIMER_7, REG_BASE_TIMER67 + 0x20),
};

#define TIMER(_index)        (&hi_timer[_index])

/* cpux-timerx: 0-4,1-6,2-5,3-7*/
#define GET_SMP_TIMER(_cpu)  (&hi_timer[((_cpu%2) << 1) + 4 + (_cpu/2)])
/*****************************************************************************/

static long __init sp804_get_clock_rate(const char *name)
{
	struct clk *clk;
	long rate;
	int err;

	clk = clk_get_sys(NULL, name);
	if (IS_ERR(clk)) {
		pr_err("%s clock not found: %d\n", name,
			(int)PTR_ERR(clk));
		return PTR_ERR(clk);
	}

	err = clk_prepare(clk);
	if (err) {
		pr_err("%s clock failed to prepare: %d\n", name, err);
		clk_put(clk);
		return err;
	}

	err = clk_enable(clk);
	if (err) {
		pr_err("%s clock failed to enable: %d\n", name, err);
		clk_unprepare(clk);
		clk_put(clk);
		return err;
	}

	rate = clk_get_rate(clk);
	if (rate < 0) {
		pr_err("%s clock failed to get rate: %ld\n", name, rate);
		clk_disable(clk);
		clk_unprepare(clk);
		clk_put(clk);
	}

	return rate;
}
#ifdef CONFIG_HAVE_SP804_LOCAL_TIMER_HI3796MV2X

static unsigned long local_timer_rate;

static void sp804_set_mode(enum clock_event_mode mode,
			   struct clock_event_device *evt)
{
	struct hi_timer_t *timer = GET_SMP_TIMER(smp_processor_id());
	unsigned long ctrl = TIMER_CTRL_32BIT | TIMER_CTRL_IE;
	unsigned int clkevt_base = timer->addr;

	writel(ctrl, IOMEM(clkevt_base + TIMER_CTRL));

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		writel(DIV_ROUND_CLOSEST(local_timer_rate, HZ),
		       IOMEM(clkevt_base + TIMER_LOAD));
		ctrl |= TIMER_CTRL_PERIODIC | TIMER_CTRL_ENABLE;
		break;

	case CLOCK_EVT_MODE_ONESHOT:
		/* period set, and timer enabled in 'next_event' hook */
		ctrl |= TIMER_CTRL_ONESHOT;
		break;

	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
	default:
		break;
	}

	writel(ctrl, IOMEM(clkevt_base + TIMER_CTRL));
}

static int sp804_set_next_event(unsigned long next,
				struct clock_event_device *evt)
{
	struct hi_timer_t *timer = GET_SMP_TIMER(smp_processor_id());
	unsigned int clkevt_base = timer->addr;
	unsigned long ctrl = readl(IOMEM(clkevt_base + TIMER_CTRL));

	writel(next, IOMEM(clkevt_base + TIMER_LOAD));
	writel(ctrl | TIMER_CTRL_ENABLE, IOMEM(clkevt_base + TIMER_CTRL));

	return 0;
}

static irqreturn_t sp804_timer_isr(int irq, void *dev_id)
{
	struct hi_timer_t *timer = (struct hi_timer_t *)dev_id;
	unsigned int clkevt_base = timer->addr;
	struct clock_event_device *evt
		= (struct clock_event_device *)timer->priv;

	/* clear the interrupt */
	writel(1, IOMEM(clkevt_base + TIMER_INTCLR));

	evt->event_handler(evt);

	return IRQ_HANDLED;
}
/*****************************************************************************/

/* every should have it's timer irq. */
static int __cpuinit hi3796mv2x_local_timer_setup(struct clock_event_device *evt)
{
	unsigned int cpu = smp_processor_id();
	struct hi_timer_t *timer = GET_SMP_TIMER(cpu);
	struct irqaction *irq = &timer->irq;

	evt->name = timer->name;
	evt->irq  = irq->irq;
	evt->features = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT
		| CLOCK_EVT_FEAT_C3STOP;

	evt->set_mode = sp804_set_mode;
	evt->set_next_event = sp804_set_next_event;
	evt->rating = 350;

	timer->priv = (void *)evt;

	clockevents_config_and_register(evt, local_timer_rate, 0xf, 0xffffffff);
	irq_set_affinity(evt->irq, evt->cpumask);
	enable_irq(evt->irq);

	return 0;
}

static void hi3796mv2x_local_timer_stop(struct clock_event_device *evt)
{
	evt->set_mode(CLOCK_EVT_MODE_UNUSED, evt);
	disable_irq(evt->irq);
}
/*****************************************************************************/

static struct local_timer_ops hi3796mv2x_timer_tick_ops __cpuinitdata = {
	.setup	= hi3796mv2x_local_timer_setup,
	.stop	= hi3796mv2x_local_timer_stop,
};

static void __init hi3796mv2x_local_timer_init(void)
{
	unsigned int cpu = 0;
	unsigned int ncores = num_possible_cpus();

	local_timer_rate = sp804_get_clock_rate("sp804");

	for(cpu = 0; cpu < ncores; cpu++) {
		struct hi_timer_t *cpu_timer = GET_SMP_TIMER(cpu);

		cpu_timer->irq.handler = sp804_timer_isr;
		cpu_timer->irq.dev_id = (void*)cpu_timer;
		setup_irq(cpu_timer->irq.irq, &cpu_timer->irq);
		disable_irq(cpu_timer->irq.irq);
	}

	local_timer_register(&hi3796mv2x_timer_tick_ops);
}
#endif /* CONFIG_HAVE_SP804_LOCAL_TIMER_HI3796MV2X */

/*****************************************************************************/
#ifdef CONFIG_HAVE_ARM_LOCAL_TIMER_HI3796MV2X
DEFINE_TWD_LOCAL_TIMER(twd_localtimer,
		      (resource_size_t)(REG_BASE_A9_PERI + REG_A9_PERI_PRI_TIMER_WDT),
		      (resource_size_t)IRQ_LOCALTIMER);
#endif

/*****************************************************************************/

struct hi3796mv2x_clocksource {
	void __iomem *base;
	struct clocksource clksrc;
};

static struct hi3796mv2x_clocksource hi3796mv2x_clocksource = {0};

static inline struct hi3796mv2x_clocksource *to_hi3796mv2x_clksrc(struct clocksource *cs)
{
	return container_of(cs, struct hi3796mv2x_clocksource, clksrc);
}

static void hi3796mv2x_clocksource_start(void __iomem *base)
{
	writel(0, IOMEM(base + TIMER_CTRL));
	writel(0xffffffff, IOMEM(base + TIMER_LOAD));
	writel(0xffffffff, IOMEM(base + TIMER_VALUE));
	writel(TIMER_CTRL_32BIT | TIMER_CTRL_ENABLE | TIMER_CTRL_PERIODIC,
		IOMEM(base + TIMER_CTRL));
}

static cycle_t hi3796mv2x_clocksource_read(struct clocksource *cs)
{
	return ~readl_relaxed(to_hi3796mv2x_clksrc(cs)->base + TIMER_VALUE);
}

static notrace u64 hi3796mv2x_sched_clock_read(void)
{
	u32 regval = ~readl_relaxed(hi3796mv2x_clocksource.base + TIMER_VALUE);
	return (u64)regval;
}

static void hi3796mv2x_clocksource_resume(struct clocksource *cs)
{
	hi3796mv2x_clocksource_start(to_hi3796mv2x_clksrc(cs)->base);
}

static void __init hi3796mv2x_clocksource_init(void __iomem *base, const char *name)
{
	long rate = sp804_get_clock_rate(name);
	struct clocksource *clksrc = &hi3796mv2x_clocksource.clksrc;

	if (rate < 0)
		return;

	clksrc->name   = name;
	clksrc->rating = 200;
	clksrc->read   = hi3796mv2x_clocksource_read;
	clksrc->mask   = CLOCKSOURCE_MASK(32),
	clksrc->flags  = CLOCK_SOURCE_IS_CONTINUOUS,
	clksrc->resume = hi3796mv2x_clocksource_resume,

	hi3796mv2x_clocksource.base = base;

	hi3796mv2x_clocksource_start(base);

	clocksource_register_hz(clksrc, rate);

	sched_clock_register(hi3796mv2x_sched_clock_read, 32, rate);
}

/*****************************************************************************/
extern void __init hi3796mv2x_clock_init(void);
u64 get_cpuectrl(void)
{
	u64 cval;

	isb();
	asm volatile("mrrc p15, 1, %Q0, %R0, c15" : "=r" (cval));
	printk("cpuectrl=0x%llu\n", cval);
	return cval;
}

void __init hi3796mv2x_timer_init(void)
{
	hi3796mv2x_clock_init();
	//TODO: to be remove
	get_cpuectrl();

#ifdef CONFIG_HAVE_ARM_LOCAL_TIMER_HI3796MV2X
	twd_local_timer_register(&twd_localtimer);
#endif

#ifdef CONFIG_HAVE_SP804_LOCAL_TIMER_HI3796MV2X
	hi3796mv2x_local_timer_init();
#endif
	of_clk_init(NULL);

	hi3796mv2x_clocksource_init((void *)TIMER(2)->addr,
		TIMER(2)->name);

	sp804_clockevents_init((void *)TIMER(3)->addr,
		TIMER(3)->irq.irq, TIMER(3)->name);
}
