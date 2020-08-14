
#include <asm/arch/platform.h>

extern void udelay(unsigned long usec);


static void hi_mci_sys_init(void)
{
	unsigned int tmp_reg;

	HIMCI_DEBUG_FUN("Function Call");

	/* SDIO clock phase */
	tmp_reg = himci_readl(REG_BASE_CRG + REG_PERI_CRG40);
	tmp_reg &= ~SDIO1_CLK_SEL_MASK;
	tmp_reg |= SDIO1_CLK_SEL_100M;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_PERI_CRG40);

	/* SDIO soft reset */
	tmp_reg |= SDIO1_SRST_REQ;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_PERI_CRG40);
	udelay(1000);
	tmp_reg &= ~SDIO1_SRST_REQ;
	tmp_reg |= SDIO1_CKEN | SDIO1_BUS_CKEN;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_PERI_CRG40);
	
	udelay(1000);
}

static unsigned int himci_get_clock(void)
{
	unsigned int clk = MMC_CLK;

	return clk;
}

static void himci_set_timing(unsigned int timing)
{}

static void himci_get_tuning_param(int datastrobe, unsigned int * min,
		unsigned int * max, unsigned int * mask, 
		unsigned int * offset, unsigned int * reg_addr)
{}
struct emmc_dev *mmc_get_ids_dev(uint * cid)
{
	return	NULL;
}