#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pm.h>
#include <linux/suspend.h>
#include <asm/memory.h>
#include <mach/early-debug.h>
#include <linux/delay.h>
#include <linux/suspend.h>
#include <linux/syscalls.h>
#include <asm/mach/time.h>
#include <linux/slab.h>
#include <asm/hardware/arm_timer.h>
#include <linux/kmemleak.h>
#include <linux/device.h>
#include <linux/irqchip/arm-gic.h>
#include <mach/hardware.h>
#include <linux/hikapi.h>

#include "l2cache.h"

#define CA_DDR_WAKEUPCHECK_START	0xffff2000
#define CA_DDR_WAKEUPCHECK_LENTH	0x2000

void __iomem *hi_sc_virtbase = NULL;
void __iomem *hi_uart_virtbase = NULL;
void __iomem *hi_crg_virtbase = NULL;
void __iomem *hi_scu_virtbase = NULL;
void __iomem *hi_ca_ddrwakeupcheck_virt = NULL;
unsigned long hi_ca_ddrwakeupcheck_phys = CA_DDR_WAKEUPCHECK_START;

/*ddr address for save cpu context*/
extern unsigned int hi_pm_ddrbase;
extern unsigned int hi_pm_phybase;

/**/
#define MCU_START_CTRL  0xf840f000      /* mcu start control */
#define OTP_IDWORD_ADDR 0xf8ab0128      /* OTP shadow register to indicate if it is advca chipset */
#define OTP_CA_ID_WORD  0x6EDBE953

extern void *hi_otp_idword_addr;
extern void *hi_mcu_start_ctrl;

asmlinkage void hi_pm_sleep(void);

int (* ca_pm_suspend)(void *sramaddr, int lenth) = NULL;
EXPORT_SYMBOL(ca_pm_suspend);

/*****************************************************************************/

void do_ca_pm_suspend(void)
{
	int ret = 0;
	unsigned int regval = readl(hi_otp_idword_addr);

	if (regval == OTP_CA_ID_WORD) {
		if (!hi_ca_ddrwakeupcheck_virt) {
			hi_ca_ddrwakeupcheck_virt = ioremap(hi_ca_ddrwakeupcheck_phys,
				CA_DDR_WAKEUPCHECK_LENTH);
		}

		BUG_ON(NULL == ca_pm_suspend);

		if (ca_pm_suspend)
			ca_pm_suspend(hi_ca_ddrwakeupcheck_virt, CA_DDR_WAKEUPCHECK_LENTH);
	}
}
/*****************************************************************************/

static int hi_pm_suspend(void)
{
	/* int ret = 0; */
	unsigned long flage = 0;

	/* disable irq */
	local_irq_save(flage);

	hi_pm_disable_l2cache();

	sram_suspend();

	do_ca_pm_suspend();

	hi_pm_sleep();

	sram_resume();

	hi_pm_enable_l2cache();

	/* enable irq */
	local_irq_restore(flage);

	return 0;
}

static int hi_pm_enter(suspend_state_t state)
{
	int ret = 0;
	switch (state) {
	case PM_SUSPEND_STANDBY:
	case PM_SUSPEND_MEM:
		ret = hi_pm_suspend();
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}

int hi_pm_valid(suspend_state_t state)
{
	return 1;
}

static const struct platform_suspend_ops hi_pm_ops = {
	.enter = hi_pm_enter,
	.valid = hi_pm_valid,
};
/*****************************************************************************/

static int __init hi_pm_init(void)
{
	hi_sc_virtbase = (void __iomem *)IO_ADDRESS(REG_BASE_SCTL);
	hi_crg_virtbase = (void __iomem *)IO_ADDRESS(REG_BASE_CRG);

	hi_otp_idword_addr = (void __iomem *)ioremap_nocache(OTP_IDWORD_ADDR, 0x1000);
	hi_mcu_start_ctrl = (void __iomem *)ioremap_nocache(MCU_START_CTRL, 0x1000);
	hi_uart_virtbase = (void __iomem *)IO_ADDRESS(REG_BASE_UART0);

	hi_pm_ddrbase = (unsigned int)kzalloc(1024, GFP_DMA | GFP_KERNEL);
	hi_pm_phybase = __pa(hi_pm_ddrbase);
	/*
	 * Because hi_pm_ddrbase is saved in .text of hi_pm_sleep.S, the kmemleak,
	 * which not check the .text, reports a mem leak here ,
	 * so we suppress kmemleak messages.
	 */
	kmemleak_not_leak((void *)hi_pm_ddrbase);

	suspend_set_ops(&hi_pm_ops);

	return 0;
}

module_init(hi_pm_init);
