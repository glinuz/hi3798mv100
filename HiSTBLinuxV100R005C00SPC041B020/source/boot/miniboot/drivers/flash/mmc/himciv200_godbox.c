
static void hi_mci_sys_init(void)
{
	unsigned int tmp_reg;

	HIMCI_DEBUG_FUN("Function Call");

	/* enable SDIO clock and clock 50MHz */
	tmp_reg = himci_readl(REG_BASE_CRG + REG_PERI_CRG46);
	tmp_reg &= ~(SDIO_SRST_REQ | SDIO_CKEN | SDIO_HCLKEN
			 | SDIO_CLK_50M | SDIO_CLK_PCTRL);
	tmp_reg |= SDIO_CKEN | SDIO_HCLKEN | SDIO_CLK_50M;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_PERI_CRG46);

	/* SDIO soft reset */
	tmp_reg |= SDIO_SRST_REQ;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_PERI_CRG46);
	udelay(1000);
	tmp_reg &= ~SDIO_SRST_REQ;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_PERI_CRG46);
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
struct emmc_dev *mmc_get_ids_dev(unsigned int * cid)
{
	return	NULL;
}