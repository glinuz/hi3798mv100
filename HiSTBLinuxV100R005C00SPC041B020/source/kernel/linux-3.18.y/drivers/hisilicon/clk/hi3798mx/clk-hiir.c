#include "clk-hisi.h"

#define SC_CLKGATE_SRST_CTRL 0xF8000048
#define IR_CLK_EN            BIT(4)
#define IR_CLK_RST           BIT(5)

int hiclk_enable_ir(struct clk_hw *hw)
{
	u32 reg;
	void __iomem *sysctl_base = ioremap_nocache(SC_CLKGATE_SRST_CTRL, sizeof(u32));

	if (!sysctl_base)
		panic("ir ioremap fail.\n");

	reg = readl(sysctl_base);
	reg |= IR_CLK_EN;
	writel(reg, sysctl_base);

	reg |= IR_CLK_RST;
	writel(reg, sysctl_base);

	reg &= ~IR_CLK_RST;
	writel(reg, sysctl_base);

	iounmap(sysctl_base);

	return 0;
}

void hiclk_disable_ir(struct clk_hw *hw)
{
	u32 reg;
	void __iomem *sysctl_base = ioremap_nocache(SC_CLKGATE_SRST_CTRL, sizeof(u32));

	if (!sysctl_base)
		panic("ir ioremap fail.\n");

	reg = readl(sysctl_base);
	reg &= ~IR_CLK_EN;
	writel(reg, sysctl_base);

	iounmap(sysctl_base);
}

struct clk_ops clk_ops_hiir = {
	.enable = hiclk_enable_ir,
	.disable = hiclk_disable_ir,
};
