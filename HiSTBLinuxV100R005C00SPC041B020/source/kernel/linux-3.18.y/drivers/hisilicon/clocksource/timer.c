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
******************************************************************************/

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/clockchips.h>
#include <linux/cpu.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/percpu.h>
#include <linux/clkdev.h>
#include <linux/clk-private.h>
#include <linux/sched_clock.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <asm/hardware/arm_timer.h>

#ifdef CONFIG_ARCH_S40
#include <linux/kernel_stat.h>
#include <mach/platform.h>
/*****************************************************************************/

#define TIMER89_BASE		IO_ADDRESS(REG_BASE_TIMER89)
#define TIMER89_IRQ		60
#define TIMER89_NAME		"timer89"
#endif
/*****************************************************************************/
//TODO: ARM64 Timer
#ifdef CONFIG_ARM64
#define IOMEM(x)	((void __force __iomem *)(x))
#endif

struct hisi_clocksource {
	void __iomem *base;
	struct clocksource clksrc;
};

struct hisi_clock_event_device {
		struct clock_event_device evt;
		void __iomem *base;
		char name[10];
};

static struct hisi_clocksource hisi_clocksource = {0};
static struct hisi_clock_event_device __percpu *hisi_local_timer_evt;
static struct irqaction __percpu *hisi_event_irq;
static void __iomem *hisi_timer_base[NR_CPUS] = {0};
static int hisi_timer_irqs[NR_CPUS] = {0};
static unsigned long hisi_clkevt_reload;
static unsigned long hisi_clk_rate;
/*****************************************************************************/

static int hisi_set_next_event(unsigned long next,
		struct clock_event_device *evt)
{
	unsigned long ctrl;
	struct hisi_clock_event_device *hisi_evt;

	hisi_evt = container_of(evt, struct hisi_clock_event_device, evt);
	ctrl = readl(hisi_evt->base + TIMER_CTRL);
	writel(next, hisi_evt->base + TIMER_LOAD);
	writel(ctrl | TIMER_CTRL_ENABLE, hisi_evt->base + TIMER_CTRL);

	return 0;
}
/*****************************************************************************/

static void hisi_set_mode(enum clock_event_mode mode,
		struct clock_event_device *evt)
{
	unsigned long ctrl = TIMER_CTRL_32BIT | TIMER_CTRL_IE;
	struct hisi_clock_event_device *hisi_evt;

	hisi_evt = container_of(evt, struct hisi_clock_event_device, evt);

	writel(ctrl, hisi_evt->base + TIMER_CTRL);

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		writel(hisi_clkevt_reload, hisi_evt->base + TIMER_LOAD);
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

	writel(ctrl, hisi_evt->base + TIMER_CTRL);
}
/*****************************************************************************/

static irqreturn_t hisi_evt_isr(int irq, void *dev_id)
{
	struct hisi_clock_event_device *hisi_evt = dev_id;
	struct clock_event_device *evt = &hisi_evt->evt;

	/* clear the interrupt */
	writel(1, hisi_evt->base + TIMER_INTCLR);

	evt->event_handler(evt);

	return IRQ_HANDLED;
}
/*****************************************************************************/

static inline struct hisi_clocksource *to_hisi_clksrc(struct clocksource *cs)
{
	return container_of(cs, struct hisi_clocksource, clksrc);
}
/*****************************************************************************/

static void hisi_clocksource_start(void __iomem *base)
{
	writel(0, IOMEM(base + TIMER_CTRL));
	writel(0xffffffff, IOMEM(base + TIMER_LOAD));
	writel(0xffffffff, IOMEM(base + TIMER_VALUE));
	writel(TIMER_CTRL_32BIT | TIMER_CTRL_ENABLE | TIMER_CTRL_PERIODIC,
		IOMEM(base + TIMER_CTRL));
}
/*****************************************************************************/

static cycle_t hisi_clocksource_read(struct clocksource *cs)
{
	return ~readl_relaxed(to_hisi_clksrc(cs)->base + TIMER_VALUE);
}
/*****************************************************************************/

static notrace u64 hisi_sched_clock_read(void)
{
	u32 regval = ~readl_relaxed(hisi_clocksource.base + TIMER_VALUE);

	return (u64)regval;
}
/*****************************************************************************/

static void hisi_clocksource_resume(struct clocksource *cs)
{
	hisi_clocksource_start(to_hisi_clksrc(cs)->base);
}
/*****************************************************************************/

static void __init hisi_clocksource_init(void __iomem *base, struct clk *clk)
{
	struct clocksource *clksrc = &hisi_clocksource.clksrc;

	clksrc->name   = clk->name;
	clksrc->rating = 200;
	clksrc->read   = hisi_clocksource_read;
	clksrc->mask   = CLOCKSOURCE_MASK(32),
	clksrc->flags  = CLOCK_SOURCE_IS_CONTINUOUS,
	clksrc->resume = hisi_clocksource_resume,

	hisi_clocksource.base = base;

	hisi_clocksource_start(base);

	clocksource_register_hz(clksrc, hisi_clk_rate);
	sched_clock_register(hisi_sched_clock_read, 32, hisi_clk_rate);
}
//TODO: ARM64 Timer
u64 hisi_get_clocksource_val(void)
{
	struct clocksource *clksrc = &hisi_clocksource.clksrc;
	if (!clksrc || !clksrc->read)
		BUG();
	return clksrc->read(clksrc);
}
/*****************************************************************************/

static int hisi_local_timer_setup(struct clock_event_device *clk)
{
	struct hisi_clock_event_device *hisi_evt;
	unsigned int cpu = smp_processor_id();
	struct irqaction *action = this_cpu_ptr(hisi_event_irq);

	hisi_evt = container_of(clk, struct hisi_clock_event_device, evt);

	hisi_evt->base = hisi_timer_base[cpu];
	snprintf(hisi_evt->name, sizeof(hisi_evt->name), "tick%d", cpu);

	clk->name = hisi_evt->name;
	clk->cpumask = cpumask_of(cpu);
	clk->set_next_event = hisi_set_next_event;
	clk->set_mode = hisi_set_mode;
	clk->features = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT;
	clk->rating = 450;
	clk->irq = hisi_timer_irqs[cpu];
	clk->mode = CLOCK_EVT_MODE_UNUSED;

	action->name = clk->name;
	action->dev_id = hisi_evt;
	action->irq = clk->irq;
	action->flags = IRQF_TIMER | IRQF_NOBALANCING;
	action->handler = hisi_evt_isr;
	BUG_ON(setup_irq(clk->irq, action));
	irq_force_affinity(clk->irq, clk->cpumask);

	clockevents_config_and_register(clk, hisi_clk_rate, 0xf, 0x7fffffff);

	return 0;
}
/*****************************************************************************/

static void hisi_local_timer_stop(struct clock_event_device *clk)
{
	pr_info("hisi_local_timer_teardown disable IRQ%d cpu #%d\n",
			 clk->irq, smp_processor_id());

	disable_irq(clk->irq);
	remove_irq(clk->irq, this_cpu_ptr(hisi_event_irq));

	clk->set_mode(CLOCK_EVT_MODE_UNUSED, clk);
}
/*****************************************************************************/

static int hisi_local_timer_cpu_notify(struct notifier_block *self,
				unsigned long action, void *hcpu)
{
	struct hisi_clock_event_device *hisi_evt;
	/*
	 * Grab cpu pointer in each case to avoid spurious
	 * preemptible warnings
	 */
	switch (action & ~CPU_TASKS_FROZEN) {
	case CPU_STARTING:
		hisi_evt = this_cpu_ptr(hisi_local_timer_evt);
		hisi_local_timer_setup(&hisi_evt->evt);
		break;
	case CPU_DYING:
		hisi_evt = this_cpu_ptr(hisi_local_timer_evt);
		hisi_local_timer_stop(&hisi_evt->evt);
		break;
	}

	return NOTIFY_OK;
}
/*****************************************************************************/

static struct notifier_block hisi_local_timer_cpu_nb = {
	.notifier_call = hisi_local_timer_cpu_notify,
};
/*****************************************************************************/
#ifdef CONFIG_ARCH_S40
static void __iomem *timer89_base = (void __iomem *) (IO_ADDRESS(REG_BASE_TIMER89));
static void __iomem *timer2_base = (void __iomem *) (IO_ADDRESS(REG_BASE_TIMER23));

static void check_timer_register(void __iomem *timer_base)
{
	unsigned int timer_load, timer_value;

	timer_load = readl(timer_base + TIMER_LOAD);
	timer_value = readl(timer_base + TIMER_VALUE);

	if (timer_value > timer_load) {
		printk("LOAD: 0x%08x, VALUE: 0x%08x\n", timer_load, timer_value);

		writel(timer_load, timer_base + TIMER_LOAD);

		printk("LOAD: 0x%08x, VALUE: 0x%08x\n", 
			readl(timer_base + TIMER_LOAD), 
			readl(timer_base + TIMER_VALUE));
	}
}
/******************************************************************************/
static irqreturn_t timer_irq_handle(int irq, void *dev_id)
{
	/* clear interrupts.*/
	writel(1, timer89_base + TIMER_INTCLR);

	check_timer_register(timer2_base);

	return IRQ_HANDLED;
}
/******************************************************************************/

static int hisi_local_timer_init(void __iomem *base, unsigned int irq, const char *name)
{
	int ret;

	writel(0, base + TIMER_CTRL);

	writel(0x000750B6, base + TIMER_BGLOAD);
	writel(0x000750B6, base + TIMER_LOAD);

	ret = request_irq(irq, timer_irq_handle, 0, name, NULL);

	writel(TIMER_CTRL_32BIT | TIMER_CTRL_ENABLE | TIMER_CTRL_PERIODIC | TIMER_CTRL_IE,
		base + TIMER_CTRL);

	return ret;
}
#endif
/******************************************************************************/
static int __init hisi_local_timer_register(void)
{
	int err;
	int cpu = smp_processor_id();
	struct hisi_clock_event_device *hisi_evt;

	hisi_local_timer_evt = alloc_percpu(struct hisi_clock_event_device);
	if (!hisi_local_timer_evt) {
		err = -ENOMEM;
		goto out;
	}

	hisi_event_irq = alloc_percpu(struct irqaction);
	if (!hisi_event_irq) {
		err = -ENOMEM;
		goto out_event_irq;
	}

	irq_set_affinity(hisi_timer_irqs[cpu], cpumask_of(0));

	err = register_cpu_notifier(&hisi_local_timer_cpu_nb);
	if (err)
		goto out_notifier;

	/* Immediately configure the timer on the boot CPU */
	hisi_evt = this_cpu_ptr(hisi_local_timer_evt);
	hisi_local_timer_setup(&hisi_evt->evt);
#ifdef CONFIG_ARCH_S40
	hisi_local_timer_init((void *)TIMER89_BASE, TIMER89_IRQ, TIMER89_NAME);
#endif

	return 0;

out_notifier:
	free_percpu(hisi_event_irq);

out_event_irq:
	free_percpu(hisi_local_timer_evt);

out:
	return err;
}

static void __init hisi_timer_init(struct device_node *node)
{
	u32 i, nr_irqs;
	struct clk *clk;
	void __iomem *base;

	nr_irqs = of_irq_count(node);
	if (nr_irqs > num_possible_cpus())
		nr_irqs = num_possible_cpus();

	for (i = 0; i < nr_irqs; i++) {
		hisi_timer_irqs[i] = irq_of_parse_and_map(node, i);
		hisi_timer_base[i] = of_iomap(node, i + 1);
		if (!hisi_timer_base[i]) {
			pr_err("can not iomap timer %d\n", i);
			goto out_unmap;
		}
	}

	base = of_iomap(node, 0);
	if (WARN_ON(!base))
		goto out_unmap;

	clk = of_clk_get(node, 0);
	if (IS_ERR(clk))
		goto out_clk;

	clk_prepare_enable(clk);

	hisi_clk_rate = clk_get_rate(clk);
	hisi_clkevt_reload = DIV_ROUND_CLOSEST(hisi_clk_rate, HZ);

	hisi_clocksource_init(base, clk);
	hisi_local_timer_register();

	return;

out_clk:
	iounmap(base);

out_unmap:
	for (i = 0; i < nr_irqs; i++) {
		if (hisi_timer_base[i])
			iounmap(hisi_timer_base[i]);
	}
}
//TODO: ARM64 Timer
u32 hisi_timer_get_rate(void)
{
	return hisi_clk_rate;
}

CLOCKSOURCE_OF_DECLARE(hisi_timer, "hisilicon,timer",
			hisi_timer_init);
