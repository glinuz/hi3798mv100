#include <linux/reset.h>
#include <linux/clk-provider.h>
#include <linux/hikapi.h>

#ifdef HIGMAC_HAS_INTERNAL_PHY
#define HIGMAC_FEPHY_CRG_CTRL	((void __iomem *)(IO_ADDRESS(0xF8A22120)))
#define HIGMAC_FEPHY_PHY_ADDR	((void __iomem *)(IO_ADDRESS(0xF8A20118)))
#define HIGMAC_FEPHY_LDO_CTRL	((void __iomem *)(IO_ADDRESS(0xF8A20844)))
#define LDO_VSET_MASK		0xF
#define LDO_VSET_VAL		0x8
#define LDO_EN_OFFSET		4
#endif

void higmac_mac_core_reset(struct higmac_netdev_local *priv)
{
	/* undo reset */
	reset_control_deassert(priv->port_rst);
	usleep_range(50, 60);

	/* soft reset mac port */
	reset_control_assert(priv->port_rst);
	usleep_range(50, 60);
	/* undo reset */
	reset_control_deassert(priv->port_rst);
}

void higmac_hw_internal_phy_reset(struct higmac_netdev_local *priv)
{
#ifdef HIGMAC_HAS_INTERNAL_PHY
	unsigned int v = 0;

	writel(0, HIGMAC_FEPHY_CRG_CTRL);/* disable clk */

	v = readl(HIGMAC_FEPHY_LDO_CTRL);
	/* set internal FEPHY LDO_VSET value */
	v = (v & ~LDO_VSET_MASK) | LDO_VSET_VAL;
#ifdef CONFIG_ARCH_HIFONE
	/* enalbe internal FEPHY LDO_EN for hifone_b02 */
	v |= (0x1 << LDO_EN_OFFSET);
#endif
	writel(v, HIGMAC_FEPHY_LDO_CTRL);/* LDO output 1.1V */

	/* suppose internal phy can only be used as mac0's phy */
	writel(priv->phy_addr, HIGMAC_FEPHY_PHY_ADDR);

	v = readl(HIGMAC_FEPHY_CRG_CTRL);
	v |= 0x1; /* use 25MHz clock, enable clk */
	writel((u32)v, HIGMAC_FEPHY_CRG_CTRL);

	udelay(10);

	v = readl(HIGMAC_FEPHY_CRG_CTRL);
	v |= (0x1 << 4); /* set reset bit */
	writel((u32)v, HIGMAC_FEPHY_CRG_CTRL);

	udelay(10);

	v = readl(HIGMAC_FEPHY_CRG_CTRL);
	v &= ~(0x1 << 4); /* clear reset bit */
	writel((u32)v, HIGMAC_FEPHY_CRG_CTRL);

	msleep(20); /* delay at least 15ms for MDIO operation */
#endif
}

void higmac_hw_phy_reset(struct higmac_netdev_local *priv)
{
	if (priv->internal_phy)
		higmac_hw_internal_phy_reset(priv);
	else
		higmac_hw_external_phy_reset(priv);
}

static void higmac_phy_reset_assert(struct reset_control *rst,
				    bool rst_when_set)
{
	if (rst_when_set)
		reset_control_assert(rst);
	else
		reset_control_deassert(rst);
}

static void higmac_phy_reset_deassert(struct reset_control *rst,
				      bool rst_when_set)
{
	if (rst_when_set)
		reset_control_deassert(rst);
	else
		reset_control_assert(rst);
}

#define RESET_DATA		1
#define GPIO_DIR		0x400
void higmac_hw_external_phy_reset(struct higmac_netdev_local *priv)
{
	u64 chipid;
	bool rst_when_set;

	/* HIFONE or 98cv200 use CRG register to reset phy */
	/* RST_BIT, write 0 to reset phy, write 1 to cancel reset */
	chipid = get_chipid(0ULL);
	if ((chipid == _HI3798CV200) || (chipid == _HI3798CV200_A) ||
		(chipid == _HI3798CV200_B))
		rst_when_set = false;
	else
		rst_when_set = true;

	if (priv->phy_rst) {
		higmac_phy_reset_deassert(priv->phy_rst, rst_when_set);
		msleep(10);

		higmac_phy_reset_assert(priv->phy_rst, rst_when_set);

		/* delay some time to ensure reset ok,
		 * this depends on PHY hardware feature
		 */
		msleep(20);

		higmac_phy_reset_deassert(priv->phy_rst, rst_when_set);
		/* delay some time to ensure later MDIO access */
		msleep(50);
	} else if (priv->gpio_base) {
#ifdef HIGMAC_RESET_PHY_BY_GPIO
		/* use gpio to control mac's phy reset */
		void __iomem *gpio_base;
		u32 gpio_bit;
		u32 v;

		gpio_base = (void __iomem *)IO_ADDRESS(
				(unsigned int)priv->gpio_base);
		gpio_bit = priv->gpio_bit;

		/* config gpip[x] dir to output */
		v = readb(gpio_base + GPIO_DIR);
		v |= (1 << gpio_bit);
		writeb(v, gpio_base + GPIO_DIR);

		/* gpiox[x] set to reset, then delay 200ms */
		writeb(RESET_DATA << gpio_bit, gpio_base + (4 << gpio_bit));
		msleep(20);
		/* then,cancel reset,and should delay 200ms */
		writeb((!RESET_DATA) << gpio_bit, gpio_base + (4 << gpio_bit));
		msleep(20);
		writeb(RESET_DATA << gpio_bit, gpio_base + (4 << gpio_bit));

		/* add some delay in case mdio cann't access now! */
		msleep(30);
#endif
	}
}

void higmac_internal_phy_clk_disable(struct higmac_netdev_local *priv)
{
#ifdef HIGMAC_HAS_INTERNAL_PHY
	writel(0, HIGMAC_FEPHY_CRG_CTRL);/* inside fephy clk disable */
#endif
}

void higmac_internal_phy_clk_enable(struct higmac_netdev_local *priv)
{
#ifdef HIGMAC_HAS_INTERNAL_PHY
	u32 v = 0;

	v = readl(HIGMAC_FEPHY_CRG_CTRL);
	v |= 0x1;
	writel(v, HIGMAC_FEPHY_CRG_CTRL);/* inside fephy clk enable */
#endif
}

void higmac_hw_all_clk_disable(struct higmac_netdev_local *priv)
{
	/* If macif clock is enabled when suspend, we should
	 * disable it here.
	 * Because when resume, PHY will link up again and
	 * macif clock will be enabled too. If we don't disable
	 * macif clock in suspend, macif clock will be enabled twice.
	 */
	if (__clk_is_enabled(priv->macif_clk))
		clk_disable_unprepare(priv->macif_clk);

	/* This is called in suspend, when net device is down,
	 * MAC clk is disabled.
	 * So we need to judge whether MAC clk is enabled,
	 * otherwise kernel will WARNING if clk disable twice.
	 */
	if (__clk_is_enabled(priv->clk))
		clk_disable_unprepare(priv->clk);

	if (priv->internal_phy)
		higmac_internal_phy_clk_disable(priv);
}

void higmac_hw_all_clk_enable(struct higmac_netdev_local *priv)
{
	if (priv->internal_phy)
		higmac_internal_phy_clk_enable(priv);

	if (!__clk_is_enabled(priv->macif_clk) &&
	    (priv->netdev->flags & IFF_UP))
		clk_prepare_enable(priv->macif_clk);

	/* If net device is down when suspend, we should not enable MAC clk. */
	if (!__clk_is_enabled(priv->clk) && (priv->netdev->flags & IFF_UP))
		clk_prepare_enable(priv->clk);
}
