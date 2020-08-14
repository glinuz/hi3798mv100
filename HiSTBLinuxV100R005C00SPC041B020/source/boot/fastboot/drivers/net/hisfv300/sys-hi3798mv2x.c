#ifdef CONFIG_NET_HISFV300_HI3798MV2X
#include <common.h>
#include <asm/arch/platform.h>
#include <ethcfg.h>

#define HIETH_SYSREG_BASE               REG_BASE_CRG
#define HIETH_SYSREG_REG                0x00D0

#define HIETHPHY_SYSREG_REG             0x0388
#define HIETH_SF_BUS_SRST_REQ_BIT       3

#define INTERNAL_FEPHY_ADDR             0xF8A20118
#define HIETH_FEPHY_LDO_CTRL            0xF8A20844

static void hieth_set_regbit(unsigned long addr, int bit, int shift)
{
	unsigned long reg;
	reg = _readl(addr);
	bit = bit ? 1 : 0;
	reg &= ~(1 << shift);
	reg |= bit << shift;
	_writel(reg, addr);
}

static void hieth_reset(int rst)
{
#ifndef CONFIG_HI3798MV2X_FPGA
	hieth_set_regbit(HIETH_SYSREG_BASE + HIETH_SYSREG_REG, rst, HIETH_SF_BUS_SRST_REQ_BIT);
	udelay(100);
#endif
}

static inline void hieth_clk_ena(void)
{
#ifndef CONFIG_HI3798MV2X_FPGA
	unsigned int val;

	val = _readl(HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
	val |= (3 << 0);
	_writel(val, HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
#endif
}

static inline void hieth_clk_dis(void)
{
#ifndef CONFIG_HI3798MV2X_FPGA
	unsigned int val;

	val = _readl(HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
	val &= ~(3 << 0);
	_writel(val, HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
#endif
}

static void hieth_internal_phy_reset(void)
{
#ifndef CONFIG_HI3798MV2X_FPGA
	unsigned int val;

	/* PERI_FEPHY_LDO_CTRL config. */
	val = _readl(HIETH_FEPHY_LDO_CTRL);
	val |= ((1 << 4) | (1 << 6));
	val &= ~((1 << 5) | (1 << 7));
	_writel(val, HIETH_FEPHY_LDO_CTRL);

	/* FEPHY enable clock */
	val = _readl(HIETH_SYSREG_BASE + HIETHPHY_SYSREG_REG);
	val |= (1);
	_writel(val, HIETH_SYSREG_BASE + HIETHPHY_SYSREG_REG);

	/* set FEPHY address */
	val = _readl(INTERNAL_FEPHY_ADDR);
	val &= ~(0x1F);
	val |= (U_PHY_ADDR & 0x1F);
	_writel(val, INTERNAL_FEPHY_ADDR);

	/* FEPHY set reset */
	val = _readl(HIETH_SYSREG_BASE + HIETHPHY_SYSREG_REG);
	val |= (1 << 4);
	_writel(val, HIETH_SYSREG_BASE + HIETHPHY_SYSREG_REG);

	udelay(10);

	/* FEPHY cancel reset */
	val = _readl(HIETH_SYSREG_BASE + HIETHPHY_SYSREG_REG);
	val &= ~(1 << 4);
	_writel(val, HIETH_SYSREG_BASE + HIETHPHY_SYSREG_REG);

	udelay(20000);
#endif
}

static void hieth_phy_reset(void)
{
	hieth_internal_phy_reset();
}

static void hieth_funsel_config(void)
{
}

static void hieth_funsel_restore(void)
{
}

#endif /*CONFIG_NET_HISFV300_HI3798MV2X */
