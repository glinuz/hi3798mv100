
#include <asm/arch/platform.h>

extern void udelay(unsigned long usec);


static void hi_mci_sys_init(void)
{
	unsigned int tmp_reg;

	HIMCI_DEBUG_FUN("Function Call");

	/* eMMC clock phase */
	tmp_reg = himci_readl(REG_BASE_CRG + REG_EMMC_PERI_CRG);
	tmp_reg &= ~SDIO_CLK_MASK;
	tmp_reg |= SDIO_CLK_100M;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_EMMC_PERI_CRG);

	/* eMMC soft reset */
	tmp_reg |= SDIO_SRST_REQ;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_EMMC_PERI_CRG);
	udelay(1000);
	tmp_reg &= ~SDIO_SRST_REQ;
	tmp_reg |= SDIO_CKEN | SDIO_BUS_CKEN;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_EMMC_PERI_CRG);
	
	udelay(1000);
}

static unsigned int himci_get_clock(void)
{
	unsigned int ix,clk,tmp_reg;
	unsigned int crgclk[] = {
		75000000, SDIO_CLK_75M,
		100000000, SDIO_CLK_100M,
		50000000, SDIO_CLK_50M,
		25000000, SDIO_CLK_25M,
		200000000, SDIO_CLK_200M,
		150000000, SDIO_CLK_150M,
		0,0,
	};

	clk = SDIO_CLK_100M;
	tmp_reg = himci_readl(REG_BASE_CRG + REG_EMMC_PERI_CRG);
	tmp_reg &= SDIO_CLK_MASK;
	
	for (ix = 0; crgclk[ix]; ix += 2) {
		if (tmp_reg == crgclk[ix+1]) {
			clk = crgclk[ix];
			break;
		}
	}

	return clk;
}

static void himci_set_timing(unsigned int timing)
{
	unsigned int ix,tmp_reg;

	tmp_reg = himci_readl(REG_BASE_IOSHARE);
	if ((tmp_reg&0x07) != 0x01) {
		printf("ioshare emmc config err!\n");
		return;
	}

	if (timing == MMC_TIMING_MMC_HS200) {
		unsigned int ioshare[] = {
			0, DRV_STENGTH_18V_6MA,
			1, DRV_STENGTH_18V_6MA,
			2, DRV_STENGTH_18V_6MA,
			3, DRV_STENGTH_18V_6MA,
			4, DRV_STENGTH_18V_6MA,
			5, DRV_STENGTH_18V_6MA,
			6, DRV_STENGTH_18V_6MA,
			7, DRV_STENGTH_18V_6MA,
			9, DRV_STENGTH_18V_6MA,
			10, DRV_STENGTH_18V_13MA,
			0xff, 0xff,
		};
		
		for (ix = 0; ioshare[ix] != 0xff; ix += 2) {
			tmp_reg = himci_readl( REG_BASE_IOSHARE+ioshare[ix]*4);
			tmp_reg &= ~(DRV_STENGTH_MASK);
			tmp_reg |= DRV_SLEV_RATE;
			tmp_reg |= ioshare[ix+1];
			himci_writel(tmp_reg, REG_BASE_IOSHARE+ioshare[ix]*4);
		}

		tmp_reg = himci_readl(REG_BASE_CRG + REG_EMMC_PERI_CRG);
		tmp_reg &= ~(SDIO_CLK_MASK|SDIO_CLK_MODE);
		tmp_reg |= SDIO_CLK_200M;
		himci_writel(tmp_reg,REG_BASE_CRG + REG_EMMC_PERI_CRG);
	}

	if (timing == MMC_TIMING_MMC_HS400) {
		unsigned int ioshare[] = {
			0, DRV_STENGTH_18V_13MA,
			1, DRV_STENGTH_18V_13MA,
			2, DRV_STENGTH_18V_13MA,
			3, DRV_STENGTH_18V_13MA,
			4, DRV_STENGTH_18V_13MA,
			5, DRV_STENGTH_18V_13MA,
			6, DRV_STENGTH_18V_13MA,
			7, DRV_STENGTH_18V_13MA,
			9, DRV_STENGTH_18V_13MA,
			10, DRV_STENGTH_18V_13MA,
			0xff, 0xff,
		};
		
		for (ix = 0; ioshare[ix] != 0xff; ix += 2) {
			tmp_reg = himci_readl( REG_BASE_IOSHARE+ioshare[ix]*4);
			tmp_reg &= ~(DRV_STENGTH_MASK);
			tmp_reg |= DRV_SLEV_RATE;
			tmp_reg |= ioshare[ix+1];
			himci_writel(tmp_reg, REG_BASE_IOSHARE+ioshare[ix]*4);
		}

		tmp_reg = himci_readl(REG_BASE_CRG + REG_EMMC_PERI_CRG);
		tmp_reg &= ~(SDIO_CLK_MASK|SDIO_CLK_MODE|SDIO_DRV_PS_MASK);
		tmp_reg |= SDIO_CLK_150M | SDIO_DRV_PS_135_67DOT5;
		himci_writel(tmp_reg,REG_BASE_CRG + REG_EMMC_PERI_CRG);
	}
}

static void himci_get_tuning_param(int datastrobe, unsigned int * min,
		unsigned int * max, unsigned int * mask,
		unsigned int * offset, unsigned int * reg_addr)
{
	if (datastrobe) {
		*min =1;
		*max = 16;
		*mask =0xf0;
		*offset = 4;
		*reg_addr = REG_BASE_CRG + REG_PERI_CRG164;
	} else {
		*min = 0;
		*max = SDIO_PHASE_NUM;
		*mask = SDIO_SAP_PS_MASK;
		*offset = 12;
		*reg_addr =REG_BASE_CRG + REG_PERI_CRG40;
	}
}