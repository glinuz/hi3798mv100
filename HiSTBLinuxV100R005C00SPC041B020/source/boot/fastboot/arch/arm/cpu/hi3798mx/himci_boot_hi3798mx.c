
#include <asm/arch/platform.h>

static inline void emmc_sys_init(void)
{

	unsigned int tmp_reg;

	/* SDIO clock phase */
	tmp_reg = himci_readl(REG_BASE_CRG + REG_PERI_CRG40);
	tmp_reg &= ~SDIO1_CLK_SEL_MASK;
	tmp_reg |= SDIO1_CLK_SEL_50M; /* phasic move to xls table. */
	himci_writel(tmp_reg, REG_BASE_CRG + REG_PERI_CRG40);

	/* SDIO soft reset */
	tmp_reg |= SDIO1_SRST_REQ;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_PERI_CRG40);
	early_udelay(1000);
	tmp_reg &= ~SDIO1_SRST_REQ;
	tmp_reg |= SDIO1_CKEN | SDIO1_BUS_CKEN;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_PERI_CRG40);

}

