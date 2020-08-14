
static void hi_mci_sys_init(void)
{
	unsigned int tmp_reg;

	HIMCI_DEBUG_FUN("Function Call");

	/* eMMC clock phase */
	tmp_reg = himci_readl(REG_BASE_CRG + REG_EMMC_PERI_CRG);
	tmp_reg &= ~EMMC_CLK_MASK;
	tmp_reg |= EMMC_CLK_MODE0_50M;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_EMMC_PERI_CRG);

	/* eMMC soft reset */
	tmp_reg |= EMMC_SRST_REQ;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_EMMC_PERI_CRG);
	udelay(1000);
	tmp_reg &= ~EMMC_SRST_REQ;
	tmp_reg |= EMMC_CKEN | EMMC_BUS_CKEN;
	himci_writel(tmp_reg, REG_BASE_CRG + REG_EMMC_PERI_CRG);

	udelay(1000);
}

static unsigned int himci_get_clock(void)
{
	unsigned int ix,clk,tmp_reg;
	unsigned int crgclk[] = {
		100000000, EMMC_CLK_MODE0_100M,
		50000000, EMMC_CLK_MODE0_50M,
		25000000, EMMC_CLK_MODE0_25M,
		200000000, EMMC_CLK_MODE0_200M,
		150000000, EMMC_CLK_MODE0_150M,
		300000000, EMMC_CLK_MODE0_300M,
		337500000, EMMC_CLK_MODE0_337DOT5M,
		0,0,
	};

	clk = EMMC_CLK_MODE0_100M;
	tmp_reg = himci_readl(REG_BASE_CRG + REG_EMMC_PERI_CRG);
	tmp_reg &= EMMC_CLK_MASK;

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

	if (timing == MMC_TIMING_MMC_HS) {

		unsigned int ioshare[] = {
			0, DRV_STENGTH_18V_3MA,
			1, DRV_STENGTH_18V_3MA,
			2, DRV_STENGTH_18V_3MA,
			3, DRV_STENGTH_18V_3MA,
			4, DRV_STENGTH_18V_3MA,
			5, DRV_STENGTH_18V_3MA,
			6, DRV_STENGTH_18V_3MA,
			7, DRV_STENGTH_18V_3MA,
			9, DRV_STENGTH_18V_3MA,
			10, DRV_STENGTH_18V_3MA,
			0xff, 0xff,
		};

		for (ix = 0; ioshare[ix] != 0xff; ix += 2) {
			tmp_reg = himci_readl( REG_BASE_IOSHARE+ioshare[ix]*4);
			tmp_reg &= ~(DRV_STENGTH_MASK);
			tmp_reg |= DRV_SLEV_RATE;
			tmp_reg |= ioshare[ix+1];
			himci_writel(tmp_reg, REG_BASE_IOSHARE+ioshare[ix]*4);
		}

		/* no config drv_strength by chip */
		tmp_reg = himci_readl(REG_BASE_CRG + REG_EMMC_PERI_CRG);
		tmp_reg &= ~(EMMC_CLK_MASK
			|EMMC_CLK_MODE
			|EMMC_DRV_PS_MASK
			|EMMC_SAP_PS_MASK);
		tmp_reg |= EMMC_CLK_MODE0_50M 
			| EMMC_DRV_PS_180_90
			| EMMC_SAP_PS_45_22DOT5;
		himci_writel(tmp_reg,REG_BASE_CRG + REG_EMMC_PERI_CRG);
	}

	if (timing == MMC_TIMING_UHS_DDR50) {

		unsigned int ioshare[] = {
			0, DRV_STENGTH_18V_3MA,
			1, DRV_STENGTH_18V_3MA,
			2, DRV_STENGTH_18V_3MA,
			3, DRV_STENGTH_18V_3MA,
			4, DRV_STENGTH_18V_3MA,
			5, DRV_STENGTH_18V_3MA,
			6, DRV_STENGTH_18V_3MA,
			7, DRV_STENGTH_18V_3MA,
			9, DRV_STENGTH_18V_3MA,
			10, DRV_STENGTH_18V_3MA,
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
		tmp_reg &= ~(EMMC_CLK_MASK | EMMC_CLK_MODE | EMMC_DRV_PS_MASK |EMMC_SAP_PS_MASK);
		tmp_reg |= EMMC_CLK_MODE0_50M  | EMMC_DRV_PS_90_45;
		if (get_mmc_io_voltage() == EMMC_IO_VOL_1_8V) {
			tmp_reg |= EMMC_SAP_PS_45_22DOT5;
		} else {
			tmp_reg |= EMMC_SAP_PS_135_67DOT5;
		}
		himci_writel(tmp_reg,REG_BASE_CRG + REG_EMMC_PERI_CRG);
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
			10, DRV_STENGTH_18V_8MA,
			0xff, 0xff,
		};

		for (ix = 0; ioshare[ix] != 0xff; ix += 2) {
			tmp_reg = himci_readl( REG_BASE_IOSHARE+ioshare[ix]*4);
			tmp_reg &= ~(DRV_STENGTH_MASK);
			tmp_reg |= DRV_SLEV_RATE;
			tmp_reg |= ioshare[ix+1];
			himci_writel(tmp_reg, REG_BASE_IOSHARE+ioshare[ix]*4);
		}

		/* hs200 clk slew rate config 0 */
		tmp_reg = himci_readl( REG_IOSHARE_10);
		tmp_reg &= ~DRV_SLEV_RATE;
		himci_writel(tmp_reg, REG_IOSHARE_10);

		tmp_reg = himci_readl(REG_BASE_CRG + REG_EMMC_PERI_CRG);
		tmp_reg &= ~(EMMC_CLK_MASK|EMMC_CLK_MODE|EMMC_DRV_PS_MASK);
		tmp_reg |= EMMC_CLK_MODE0_200M | EMMC_DRV_PS_135_67DOT5;
		himci_writel(tmp_reg,REG_BASE_CRG + REG_EMMC_PERI_CRG);
	}

	if (timing == MMC_TIMING_MMC_HS400) {
		unsigned int ioshare[] = {
			0, DRV_STENGTH_18V_8MA,
			1, DRV_STENGTH_18V_8MA,
			2, DRV_STENGTH_18V_8MA,
			3, DRV_STENGTH_18V_8MA,
			4, DRV_STENGTH_18V_8MA,
			5, DRV_STENGTH_18V_8MA,
			6, DRV_STENGTH_18V_8MA,
			7, DRV_STENGTH_18V_8MA,
			9, DRV_STENGTH_18V_8MA,
			10, DRV_STENGTH_18V_8MA,
			0xff, 0xff,
		};

		for (ix = 0; ioshare[ix] != 0xff; ix += 2) {
			tmp_reg = himci_readl( REG_BASE_IOSHARE+ioshare[ix]*4);
			tmp_reg &= ~(DRV_STENGTH_MASK);
			tmp_reg &= ~DRV_SLEV_RATE;
			tmp_reg |= ioshare[ix+1];
			himci_writel(tmp_reg, REG_BASE_IOSHARE+ioshare[ix]*4);
		}

		tmp_reg = himci_readl(REG_BASE_CRG + REG_EMMC_PERI_CRG);
		tmp_reg &= ~(EMMC_CLK_MASK|EMMC_CLK_MODE|EMMC_DRV_PS_MASK | EMMC_SAP_PS_MASK);
		tmp_reg |= EMMC_CLK_MODE0_150M | EMMC_DRV_PS_90_45 | (emmc_samplephase<< 12);
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
		*mask =0xff00;
		*offset = 8;
		*reg_addr = REG_BASE_CRG + REG_PERI_CRG164;
	} else {
		*min = 0;
		*max = EMMC_PHASE_NUM;
		*mask = EMMC_SAP_PS_MASK;
		*offset = 12;
		*reg_addr =REG_BASE_CRG + REG_PERI_CRG40;
	}
}
