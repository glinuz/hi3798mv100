#ifdef CONFIG_NET_HISFV300_S40
#include <common.h>
#include <asm/arch/platform.h>

#define HIETH_SYSREG_BASE               REG_BASE_CRG
#define HIETH_SYSREG_REG                0x00CC
#define HIETHPHY_SYSREG_REG             0x0120
#define INTERNAL_FEPHY_ADDR             0xF8A20118
#define HIETH_FEPHY_SELECT              0xF8A20008
#define HIETH_FEPHY_LDO_CTRL            0xF8A20844
#define HIETH_INDEX                     0

/* DEFAULT external phy reset pin */
#define HIETH_FEPHY_RST_BASE            0xF8A22168
#define HIETH_FEPHY_RST_BIT             1

static void hieth_set_regbit(unsigned long addr, int bit, int shift)
{
	unsigned long reg;
	reg = _readl(addr);
	bit = bit ? 1 : 0;
	reg &= ~(1<<shift);
	reg |= bit<<shift;
	_writel(reg, addr);
}

static void hieth_reset(int rst)
{
	hieth_set_regbit(HIETH_SYSREG_BASE + HIETH_SYSREG_REG, rst, 0);
	udelay(100);
}


static inline void hieth_clk_ena(void)
{
	unsigned int val;

	val = _readl(HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
	val |= (1 << 1);
	_writel(val, HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
}

static inline void hieth_clk_dis(void)
{
	unsigned int val;

	val = _readl(HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
	val &= ~(1 << 1);
	_writel(val, HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
}

static void hieth_external_phy_reset(void)
{
	u32 gpio_base = 0;
	u32 gpio_bit = 0;
	u32 v = 0;

	/************************************************/
	/* reset external phy with default reset pin */
	v = readl(HIETH_FEPHY_RST_BASE);
	v |= (1 << HIETH_FEPHY_RST_BIT);
	writel(v, HIETH_FEPHY_RST_BASE);

	udelay(1000);

	/* then, cancel reset, and should delay 200ms */
	v &= ~(1 << HIETH_FEPHY_RST_BIT);
	writel(v, HIETH_FEPHY_RST_BASE);

	udelay(1000);
	v |=  1 << HIETH_FEPHY_RST_BIT;
	writel(v, HIETH_FEPHY_RST_BASE);
	udelay(1000);

    /************************************************/
	/* reset external phy with gpio */
#define RESET_DATA	(1)

	if (0 != get_eth_phygpio(HIETH_INDEX, &gpio_base, &gpio_bit)) {
		return;
	}

	/* maybe phygpio=none */
	if (!gpio_base)
		return;

	/* config gpio[x] dir to output */
	v = readb(gpio_base + 0x400);
	v |= (1 << gpio_bit);
	writeb(v, gpio_base + 0x400);

	/* output 1--0--1 */
	writeb(RESET_DATA << gpio_bit,
			gpio_base + (4 << gpio_bit));
	udelay(1000);
	writeb((!RESET_DATA) << gpio_bit,
			gpio_base + (4 << gpio_bit));
	udelay(1000);
	writeb(RESET_DATA << gpio_bit,
			gpio_base + (4 << gpio_bit));
	udelay(1000);
	/************************************************/
}

static void hieth_internal_phy_reset(void)
{
	unsigned int val;

	val = _readl(HIETH_FEPHY_SELECT);
	/* if not use fephy, leave it's clk disabled */
	if ((val & (1 << 8)) != 0)
		return;

	/* LDO output 1.1V */
	_writel(0x68, HIETH_FEPHY_LDO_CTRL);

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
}

static void hieth_phy_reset(void)
{
	unsigned int val;

	val = _readl(HIETH_FEPHY_SELECT);
	/* if not use fephy, leave it's clk disabled */
	if ((val & (1 << 8)) != 0)
		hieth_external_phy_reset();
	else
		hieth_internal_phy_reset();
}

static void hieth_funsel_config(void)
{
}

static void hieth_funsel_restore(void)
{
}

#endif/*CONFIG_NET_HISFV300_S40*/
