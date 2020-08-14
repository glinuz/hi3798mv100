#ifdef CONFIG_NET_HISFV300_GODBOX
#  include <common.h>
#  include <asm/arch/platform.h>

#  define HIETH_SYSREG_BASE               REG_BASE_CRG
#  define HIETH_SYSREG_REG                REG_PERI_CRG26

#  define HI3716MV300_ETH_SYSREG_RST_BIT  (1 << 1)

#  define HI3716MV300_ETH_GPIO_REG        (0x016c)
#  define HI3716MV300_ETH_GPIO_BASE       (REG_BASE_IO_CONFIG)

#  define HI3712_ETH_SYSREG_RST_BIT       (1 << 1)

#  define HI3712_ETH_GPIO_REG             (0x28)
#  define HI3712_ETH_GPIO_BASE            (REG_BASE_IO_CONFIG)

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
	hieth_set_regbit(HIETH_SYSREG_BASE + HIETH_SYSREG_REG, rst, 0);
	udelay(100);
}

static inline void hieth_clk_ena(void)
{
	unsigned int val;

	val = _readl(HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
	val |= (1 << 8);
	_writel(val, HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
}

static inline void hieth_clk_dis(void)
{
	unsigned int val;

	val = _readl(HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
	val &= ~(1 << 8);
	_writel(val, HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
}

static void hieth_phy_reset(void)
{
	long long chipid;
	unsigned int val;
	unsigned int old;

	chipid = get_chipid();

	if (_HI3712_V100 == chipid) {
		/* TODO: HI3712 ASIC should config GPIO for phy reset!!. */

		/* config pin re-use to miirst */
		old = val = _readl(HI3712_ETH_GPIO_BASE + HI3712_ETH_GPIO_REG);
		val |= 0x1;
		_writel(val, HI3712_ETH_GPIO_BASE + HI3712_ETH_GPIO_REG);

		/* do phy reset */
		val = _readl(HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
		val |= HI3712_ETH_SYSREG_RST_BIT;
		_writel(val, HIETH_SYSREG_BASE + HIETH_SYSREG_REG);

		udelay(20000);

		val = _readl(HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
		val &= ~HI3712_ETH_SYSREG_RST_BIT;
		_writel(val, HIETH_SYSREG_BASE + HIETH_SYSREG_REG);

		udelay(20000);

		_writel(old, HI3712_ETH_GPIO_BASE + HI3712_ETH_GPIO_REG);

	} else if (chipid == _HI3716M_V300) {

		/* config pin re-use to miirst */
		old = val =
		    _readl(HI3716MV300_ETH_GPIO_BASE +
			   HI3716MV300_ETH_GPIO_REG);
		val |= 0x1;
		_writel(val,
			HI3716MV300_ETH_GPIO_BASE + HI3716MV300_ETH_GPIO_REG);

		/* do phy reset */
		val = _readl(HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
		val |= HI3716MV300_ETH_SYSREG_RST_BIT;
		_writel(val, HIETH_SYSREG_BASE + HIETH_SYSREG_REG);

		udelay(20000);

		val = _readl(HIETH_SYSREG_BASE + HIETH_SYSREG_REG);
		val &= ~HI3716MV300_ETH_SYSREG_RST_BIT;
		_writel(val, HIETH_SYSREG_BASE + HIETH_SYSREG_REG);

		udelay(20000);

		_writel(old,
			HI3716MV300_ETH_GPIO_BASE + HI3716MV300_ETH_GPIO_REG);
	} else {
#  ifdef HISFV_RESET_GPIO_EN
		/* gpiox[x] set to reset, then delay 200ms */
		val = __raw_readb(HISFV_RESET_GPIO_BASE + HISFV_RESET_GPIO_DIR);
		val |= (HISFV_RESET_GPIO_DIR_OUT << HISFV_RESET_GPIO_BIT);
		__raw_writeb(val, HISFV_RESET_GPIO_BASE + HISFV_RESET_GPIO_DIR);
		__raw_writeb(HISFV_RESET_GPIO_DATA,
			     HISFV_RESET_GPIO_BASE +
			     (4 << HISFV_RESET_GPIO_BIT));

		udelay(200000);

		/* then,cancel reset,and should delay 200ms */
		val = __raw_readb(HISFV_RESET_GPIO_BASE + HISFV_RESET_GPIO_DIR);
		val |= (HISFV_RESET_GPIO_DIR_OUT << HISFV_RESET_GPIO_BIT);
		__raw_writeb(val, HISFV_RESET_GPIO_BASE + HISFV_RESET_GPIO_DIR);
		__raw_writeb(((!HISFV_RESET_GPIO_DATA) << HISFV_RESET_GPIO_BIT),
			     HISFV_RESET_GPIO_BASE +
			     (4 << HISFV_RESET_GPIO_BIT));

		udelay(20000);
#  endif
	}
}

static void hieth_funsel_config(void)
{
}

static void hieth_funsel_restore(void)
{
}

#endif /*CONFIG_NET_HISFV300_GODBOX */
