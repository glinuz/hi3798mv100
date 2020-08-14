/* Phase details */
#define SDIO_DRV_PS_MASK                   (0x7 << 16)
#define SDIO_DRV_PS_0_0                    (0b000 << 16)
#define SDIO_DRV_PS_45_22DOT5              (0b001 << 16)
#define SDIO_DRV_PS_90_45                  (0b010 << 16)
#define SDIO_DRV_PS_135_67DOT5             (0b011 << 16)
#define SDIO_DRV_PS_180_90                 (0b100 << 16)
#define SDIO_DRV_PS_225_112DOT5            (0b101 << 16)
#define SDIO_DRV_PS_270_145                (0b110 << 16)
#define SDIO_DRV_PS_315_167DOT5            (0b111 << 16)

#define SDIO_SAP_PS_NUM                    (8)
#define SDIO_SAP_PS_MASK                   (0x7 << 12)
#define SDIO_SAP_PS_OFFSET                 (12)
#define SDIO_SAP_PS_0_0                    (0b000 << 12)
#define SDIO_SAP_PS_45_22DOT5              (0b001 << 12)
#define SDIO_SAP_PS_90_45                  (0b010 << 12)
#define SDIO_SAP_PS_135_67DOT5             (0b011 << 12)
#define SDIO_SAP_PS_180_90                 (0b100 << 12)
#define SDIO_SAP_PS_225_112DOT5            (0b101 << 12)
#define SDIO_SAP_PS_270_145                (0b110 << 12)
#define SDIO_SAP_PS_315_167DOT5            (0b111 << 12)

/* Driver caps details */
#define DRV_CAPS_MASK                       (0xf << 4)
#define DRV_SLEV_RATE                       BIT(8)

#define EMMC_DRV_CAP_DDR50_CMD_DATA         (0b1110 << 4)
#define EMMC_DRV_CAP_DDR50_CLOCK            (0b1101 << 4)

#define EMMC_DRV_CAP_HS200_CMD_DATA         (0b1011 << 4)
#define EMMC_DRV_CAP_HS200_CLOCK            (0b1011 << 4)

#define EMMC_DRV_CAP_HS400_CMD_DATA         (0b1011 << 4)
#define EMMC_DRV_CAP_HS400_CLOCK            (0b1011 << 4)

#define SDIO_DRV_CAP_3V3_50M_CMD_DATA       (0b101 << 4)
#define SDIO_DRV_CAP_1V8_50M_CMD_DATA       (0b011 << 4)
#define SDIO_DRV_CAP_1V8_100M_CMD_DATA      (0b011 << 4)
#define SDIO_DRV_CAP_1V8_200M_CMD_DATA      (0b011 << 4)

#define SDIO_DRV_CAP_3V3_50M_CLOCK          (0b1101 << 4)
#define SDIO_DRV_CAP_1V8_50M_CLOCK          (0b1001 << 4)
#define SDIO_DRV_CAP_1V8_100M_CLOCK         (0b0100 << 4)
#define SDIO_DRV_CAP_1V8_200M_CLOCK         (0b0000 << 4)

#define IOSHARE_OFFSET_EMMC_CLOCK  0x030
#define IOSHARE_OFFSET_EMMC_CMD    0x038
#define IOSHARE_OFFSET_EMMC_DATA0  0x018
#define IOSHARE_OFFSET_EMMC_DATA1  0x020
#define IOSHARE_OFFSET_EMMC_DATA2  0x028
#define IOSHARE_OFFSET_EMMC_DATA3  0x010
#define IOSHARE_OFFSET_EMMC_DATA4  0x014
#define IOSHARE_OFFSET_EMMC_DATA5  0x01C
#define IOSHARE_OFFSET_EMMC_DATA6  0x024
#define IOSHARE_OFFSET_EMMC_DATA7  0x02C

#define IOSHARE_OFFSET_SDIO0_CLOCK  0x00C
#define IOSHARE_OFFSET_SDIO0_CMD    0x010
#define IOSHARE_OFFSET_SDIO0_DATA0  0x008
#define IOSHARE_OFFSET_SDIO0_DATA1  0x004
#define IOSHARE_OFFSET_SDIO0_DATA2  0x018
#define IOSHARE_OFFSET_SDIO0_DATA3  0x014

#define EMMC_IO_VOLTAGE_MASK               (0x01)
#define EMMC_IO_VOL_1_8V                   (0x01)
#define EMMC_IO_VOL_3_3V                   (0x00)

#define HS400_MAX_CLK  150000000;

#ifndef REG_BASE_SCTL
#define REG_BASE_SCTL                      (0xF8000000)
#endif

#ifndef REG_SC_GEN29
#define REG_SC_GEN29                        0x00F4
#endif
/******************************************************************************/

u32 iohs[] = {
	IOSHARE_OFFSET_EMMC_DATA3, EMMC_DRV_CAP_DDR50_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA4, EMMC_DRV_CAP_DDR50_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA0, EMMC_DRV_CAP_DDR50_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA5, EMMC_DRV_CAP_DDR50_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA1, EMMC_DRV_CAP_DDR50_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA6, EMMC_DRV_CAP_DDR50_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA2, EMMC_DRV_CAP_DDR50_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA7, EMMC_DRV_CAP_DDR50_CMD_DATA,
	IOSHARE_OFFSET_EMMC_CLOCK, EMMC_DRV_CAP_DDR50_CLOCK,
	IOSHARE_OFFSET_EMMC_CMD,   EMMC_DRV_CAP_DDR50_CMD_DATA,
	0xff, 0xff,
};

u32 iohs200[] = {
	IOSHARE_OFFSET_EMMC_DATA3, EMMC_DRV_CAP_HS200_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA4, EMMC_DRV_CAP_HS200_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA0, EMMC_DRV_CAP_HS200_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA5, EMMC_DRV_CAP_HS200_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA1, EMMC_DRV_CAP_HS200_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA6, EMMC_DRV_CAP_HS200_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA2, EMMC_DRV_CAP_HS200_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA7, EMMC_DRV_CAP_HS200_CMD_DATA,
	IOSHARE_OFFSET_EMMC_CLOCK, EMMC_DRV_CAP_HS200_CLOCK,
	IOSHARE_OFFSET_EMMC_CMD,   EMMC_DRV_CAP_HS200_CMD_DATA,
	0xff, 0xff,
};

u32 iohs400[] = {
	IOSHARE_OFFSET_EMMC_DATA3, EMMC_DRV_CAP_HS400_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA4, EMMC_DRV_CAP_HS400_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA0, EMMC_DRV_CAP_HS400_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA5, EMMC_DRV_CAP_HS400_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA1, EMMC_DRV_CAP_HS400_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA6, EMMC_DRV_CAP_HS400_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA2, EMMC_DRV_CAP_HS400_CMD_DATA,
	IOSHARE_OFFSET_EMMC_DATA7, EMMC_DRV_CAP_HS400_CMD_DATA,
	IOSHARE_OFFSET_EMMC_CLOCK, EMMC_DRV_CAP_HS400_CLOCK,
	IOSHARE_OFFSET_EMMC_CMD,   EMMC_DRV_CAP_HS400_CMD_DATA,
	0xff, 0xff,
};

u32 io_sdhs[] = {
	IOSHARE_OFFSET_SDIO0_CLOCK, SDIO_DRV_CAP_3V3_50M_CLOCK,
	IOSHARE_OFFSET_SDIO0_CMD,   SDIO_DRV_CAP_3V3_50M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA0, SDIO_DRV_CAP_3V3_50M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA1, SDIO_DRV_CAP_3V3_50M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA2, SDIO_DRV_CAP_3V3_50M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA3, SDIO_DRV_CAP_3V3_50M_CMD_DATA,
	0xff,0xff,
};

u32 io_uhs_sdr25[] = {
	IOSHARE_OFFSET_SDIO0_CLOCK, SDIO_DRV_CAP_1V8_50M_CLOCK,
	IOSHARE_OFFSET_SDIO0_CMD,   SDIO_DRV_CAP_1V8_50M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA0, SDIO_DRV_CAP_1V8_50M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA1, SDIO_DRV_CAP_1V8_50M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA2, SDIO_DRV_CAP_1V8_50M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA3, SDIO_DRV_CAP_1V8_50M_CMD_DATA,
	0xff,0xff,
};

u32 io_uhs_sdr50[] = {
	IOSHARE_OFFSET_SDIO0_CLOCK, SDIO_DRV_CAP_1V8_100M_CLOCK,
	IOSHARE_OFFSET_SDIO0_CMD,   SDIO_DRV_CAP_1V8_100M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA0, SDIO_DRV_CAP_1V8_100M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA1, SDIO_DRV_CAP_1V8_100M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA2, SDIO_DRV_CAP_1V8_100M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA3, SDIO_DRV_CAP_1V8_100M_CMD_DATA,
	0xff,0xff,
};

u32 io_uhs_sdr104[] = {
	IOSHARE_OFFSET_SDIO0_CLOCK, SDIO_DRV_CAP_1V8_200M_CLOCK,
	IOSHARE_OFFSET_SDIO0_CMD,   SDIO_DRV_CAP_1V8_200M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA0, SDIO_DRV_CAP_1V8_200M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA1, SDIO_DRV_CAP_1V8_200M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA2, SDIO_DRV_CAP_1V8_200M_CMD_DATA,
	IOSHARE_OFFSET_SDIO0_DATA3, SDIO_DRV_CAP_1V8_200M_CMD_DATA,
	0xff,0xff,
};

/******************************************************************************/

static void himciv200_set_driver(struct himciv200_host * host, u8 timing)
{
	u32 ix, regval;

	if (strcmp(dev_name(host->dev), "f9830000.himciv200.MMC") == 0) {
		 if ((timing == MMC_TIMING_MMC_HS)
			|| (timing == MMC_TIMING_LEGACY)
			|| (timing == MMC_TIMING_MMC_DDR52)) {

			for (ix = 0; iohs[ix] != 0xff; ix += 2) {
				regval = readl( host->ioshare_addr + iohs[ix]);
				regval &= ~(DRV_CAPS_MASK);
				regval |= (DRV_SLEV_RATE | iohs[ix+1]);
				writel(regval, host->ioshare_addr + iohs[ix]);
			}
		} else if (timing == MMC_TIMING_MMC_HS200) {

			for (ix = 0; iohs200[ix] != 0xff; ix += 2) {
				regval = readl( host->ioshare_addr +iohs200[ix]);
				regval &= ~(DRV_CAPS_MASK);
				regval |= (DRV_SLEV_RATE | iohs200[ix+1]);
				writel(regval, host->ioshare_addr +iohs200[ix]);
			}
			/* hs200 clk slew rate config 0 */
			regval = readl( host->ioshare_addr +0x028);
			regval &= ~DRV_SLEV_RATE;
			writel(regval, host->ioshare_addr +0x028);

		} else if (timing == MMC_TIMING_MMC_HS400) {

			for (ix = 0; iohs400[ix] != 0xff; ix += 2) {
				regval = readl( host->ioshare_addr +iohs400[ix]);
				regval &= ~(DRV_CAPS_MASK);
				regval &= ~DRV_SLEV_RATE;
				regval |=  iohs400[ix+1];
				writel(regval, host->ioshare_addr +iohs400[ix]);
			}
		} 
	} else if(strcmp(dev_name(host->dev), "f9820000.himciv200.SD") == 0) {
		if((timing == MMC_TIMING_SD_HS) 
		   || (timing == MMC_TIMING_UHS_SDR12)
		   || (timing == MMC_TIMING_LEGACY)) {
			for (ix = 0; io_sdhs[ix] != 0xff; ix += 2) {
				regval = readl( host->ioshare_addr + io_sdhs[ix]);
				regval &= ~(DRV_CAPS_MASK);
				regval |= (DRV_SLEV_RATE |io_sdhs[ix+1]);
				writel(regval, host->ioshare_addr + io_sdhs[ix]);
			}
		} else if ((timing == MMC_TIMING_UHS_DDR50)||
		           (timing == MMC_TIMING_UHS_SDR25)){
			for (ix = 0; io_uhs_sdr25[ix] != 0xff; ix += 2) {
				regval = readl( host->ioshare_addr + io_uhs_sdr25[ix]);
				regval &= ~(DRV_CAPS_MASK);
				regval |= (DRV_SLEV_RATE |io_uhs_sdr25[ix+1]);
				writel(regval, host->ioshare_addr + io_uhs_sdr25[ix]);
			}
		} else if (timing == MMC_TIMING_UHS_SDR50) {
			for (ix = 0; io_uhs_sdr50[ix] != 0xff; ix += 2) {
				regval = readl( host->ioshare_addr + io_uhs_sdr50[ix]);
				regval &= ~(DRV_CAPS_MASK);
				regval &= ~DRV_SLEV_RATE;
				regval |=  io_uhs_sdr50[ix+1];
				writel(regval, host->ioshare_addr + io_uhs_sdr50[ix]);
			}
		} else if (timing == MMC_TIMING_UHS_SDR104) {
			for (ix = 0; io_uhs_sdr104[ix] != 0xff; ix += 2) {
				regval = readl( host->ioshare_addr + io_uhs_sdr104[ix]);
				regval &= ~(DRV_CAPS_MASK);
				regval &= ~DRV_SLEV_RATE;
				regval |=  io_uhs_sdr104[ix+1];
				writel(regval, host->ioshare_addr + io_uhs_sdr104[ix]);
			}
		} 
	} 
}

/******************************************************************************/

static void himciv200_set_crgclk(struct himciv200_host * host, u8 timing)
{
	if (strcmp(dev_name(host->dev), "f9830000.himciv200.MMC") == 0) {

		if(timing == MMC_TIMING_MMC_HS) {
			host->mmc->f_max = 50000000;
		} else if (timing == MMC_TIMING_UHS_DDR50) {
			host->mmc->f_max = 50000000;
		} else if (timing == MMC_TIMING_MMC_DDR52) {
			host->mmc->f_max = 50000000;
		} else if (timing == MMC_TIMING_MMC_HS200) {
			host->mmc->f_max = 200000000;
		} else if (timing == MMC_TIMING_MMC_HS400) {
			host->mmc->f_max = HS400_MAX_CLK;
		}

		if (host->clk->ops->set_rate) {
			host->clk->ops->set_rate(host->clk->hw,
					(unsigned long)host->mmc->f_max, 0);
		}
	} else if (strcmp(dev_name(host->dev), "f9820000.himciv200.SD") == 0) {
		if((timing == MMC_TIMING_SD_HS) || (timing == MMC_TIMING_LEGACY)) {
			host->mmc->f_max = 25000000;
		} else if (timing == MMC_TIMING_UHS_DDR50) {
			host->mmc->f_max = 50000000;
		} else if (timing == MMC_TIMING_UHS_SDR12) {
			host->mmc->f_max = 50000000;
		} else if (timing == MMC_TIMING_UHS_SDR25) {
			host->mmc->f_max = 50000000;
		} else if (timing == MMC_TIMING_UHS_SDR50) {
			host->mmc->f_max = 100000000;
		} else if (timing == MMC_TIMING_UHS_SDR104) {
			host->mmc->f_max = 150000000;
		}

		if (host->clk->ops->set_rate) {
			host->clk->ops->set_rate(host->clk->hw,
					(unsigned long)host->mmc->f_max, 0);
		}
	}
}
/******************************************************************************/

static void himciv200_set_phase(struct himciv200_host * host, u8 timing)
{
	u32 regval;

	himci_trace(3, "begin");

	if (strcmp(dev_name(host->dev), "f9830000.himciv200.MMC") == 0) {
		/* config sample clk/drv clk phase shift */
		regval = (u32)host->clk->ops->get_phase(host->clk->hw);
		if ((timing == MMC_TIMING_MMC_HS) || (timing == MMC_TIMING_LEGACY)) {
			regval &= ~(SDIO_SAP_PS_MASK | SDIO_DRV_PS_MASK);
			regval |=  SDIO_SAP_PS_315_167DOT5 | SDIO_DRV_PS_180_90;
		} else if (timing == MMC_TIMING_MMC_DDR52) {
			regval &= ~(SDIO_SAP_PS_MASK| SDIO_DRV_PS_MASK);
				regval |= (SDIO_DRV_PS_135_67DOT5 | BIT(19));
			if (host->iovoltage == EMMC_IO_VOL_1_8V)
				regval |= SDIO_SAP_PS_45_22DOT5; 
			else
				regval |= SDIO_SAP_PS_45_22DOT5;
		} else if (timing == MMC_TIMING_MMC_HS200) {
			regval &= ~(SDIO_DRV_PS_MASK);
			regval |=  SDIO_DRV_PS_135_67DOT5;
		}  else if(timing == MMC_TIMING_MMC_HS400) {
			regval &= ~(SDIO_SAP_PS_MASK | SDIO_DRV_PS_MASK);
			regval |=  (emmc_tuning_phase << SDIO_SAP_PS_OFFSET) | SDIO_DRV_PS_90_45;
		}
		host->clk->ops->set_phase(host->clk->hw, (int)regval);

		/* ddr50 need to enable shift */
		regval = mci_readl(host, MCI_ENABLE_SHIFT);
		 if (timing == MMC_TIMING_MMC_DDR52) 
			regval |= ENABLE_SHIFT_01;
		else 
			regval &= ~ENABLE_SHIFT_01;
		mci_writel(host, MCI_ENABLE_SHIFT, regval);
	} else if(strcmp(dev_name(host->dev), "f9820000.himciv200.SD") == 0) {
		regval = (u32)host->clk->ops->get_phase(host->clk->hw);
		if (timing == MMC_TIMING_UHS_SDR104){
			regval &= ~(SDIO_SAP_PS_MASK| SDIO_DRV_PS_MASK);
			regval |= (SDIO_DRV_PS_135_67DOT5|SDIO_SAP_PS_225_112DOT5);
		} else if (timing == MMC_TIMING_UHS_DDR50) {
			regval &= ~(SDIO_SAP_PS_MASK| SDIO_DRV_PS_MASK);
			regval |= (SDIO_DRV_PS_90_45|SDIO_SAP_PS_135_67DOT5);
		} else if (timing == MMC_TIMING_UHS_SDR50){
			regval &= ~(SDIO_SAP_PS_MASK| SDIO_DRV_PS_MASK);
			regval |= (SDIO_DRV_PS_180_90|SDIO_SAP_PS_90_45);
		} else if((timing == MMC_TIMING_SD_HS) || (timing == MMC_TIMING_LEGACY)) {
			regval &= ~(SDIO_SAP_PS_MASK| SDIO_DRV_PS_MASK);
			regval |= (SDIO_DRV_PS_180_90|SDIO_SAP_PS_90_45);
		} else if (timing == MMC_TIMING_UHS_SDR25){
			regval &= ~(SDIO_SAP_PS_MASK| SDIO_DRV_PS_MASK);
			regval |= (SDIO_DRV_PS_180_90|SDIO_SAP_PS_90_45);
		} else if (timing == MMC_TIMING_UHS_SDR12){
			regval &= ~(SDIO_SAP_PS_MASK| SDIO_DRV_PS_MASK);
			regval |= (SDIO_DRV_PS_180_90|SDIO_SAP_PS_180_90);
		}
		host->clk->ops->set_phase(host->clk->hw, (int)regval);
	}
}
/******************************************************************************/

static void himciv200_set_timing(struct himciv200_host * host, u8 timing)
{
	himciv200_set_driver(host, timing);
	himciv200_set_crgclk(host, timing);
	himciv200_set_phase(host, timing);
}
/******************************************************************************/

static int himciv200_prepare_hs400(struct mmc_host * mmc, struct mmc_ios * ios)
{
	u32 ix, regval;
	struct himciv200_host *host = mmc_priv(mmc);
	himci_trace(3, "begin");

	if (strcmp(dev_name(host->dev), "f9830000.himciv200.MMC") == 0) {
		/* config driver strength */
		for (ix = 0; iohs400[ix] != 0xff; ix += 2) {
			regval = readl( host->ioshare_addr +iohs400[ix]*4);
			regval &= ~(DRV_CAPS_MASK);
			regval &= ~DRV_SLEV_RATE;
			regval |=  iohs400[ix+1];
			writel(regval, host->ioshare_addr +iohs400[ix]*4);
		}

		/* config source clock */
		host->mmc->f_max = HS400_MAX_CLK;
		host->clk->ops->set_rate(host->clk->hw,
				(unsigned long)host->mmc->f_max, 0);

		/* config clk phase */
		regval = (u32)host->clk->ops->get_phase(host->clk->hw);
		regval &= ~(SDIO_DRV_PS_MASK);
		regval |=  SDIO_DRV_PS_90_45;
		host->clk->ops->set_phase(host->clk->hw, (int)regval);
	}

	return 0;
}
/******************************************************************************/

int himciv200_execute_tuning(struct mmc_host * mmc, u32 opcode)
{
	u32 index, regval;
	struct himciv200_host *host = mmc_priv(mmc);

	himci_trace(3, "begin");

#if 1
	if (strcmp(dev_name(host->dev), "f9830000.himciv200.MMC") == 0) {
		index = emmc_tuning_phase;
		regval = (u32)host->clk->ops->get_phase(host->clk->hw);
		regval &= ~SDIO_SAP_PS_MASK;
		regval |= (index << SDIO_SAP_PS_OFFSET);
		host->clk->ops->set_phase(host->clk->hw, (int)regval);
	}
#else

	u32 found = 0, startp =-1, endp = -1;
	int prev_err, err = 0;


	host->tunning = 1;

	for (index = 0; index < SDIO_SAP_PS_NUM; index++) {

		/* set phase shift */
		regval = (u32)host->clk->ops->get_phase(host->clk->hw);
		regval &= ~SDIO_SAP_PS_MASK;
		regval |= (index << SDIO_SAP_PS_OFFSET);
		host->clk->ops->set_phase(host->clk->hw, (int)regval);

		mci_writel(host,  MCI_RINTSTS, ALL_INT_CLR);

		err = mmc_send_tuning(mmc);

		if (!err) {
			/* found first valid phase */
			found = 1;
		}
		if (index > 0) {
			if (err && !prev_err)
				endp = index - 1;

			if (!err && prev_err)
				startp = index;
		}

		if ((startp != -1) && (endp != -1))
			goto tuning_out;

		prev_err = err;
		err = 0;
	}

tuning_out:

	if (found) {

		if (-1 == startp)
			startp = 0;
		if (-1 == endp)
			endp = SDIO_SAP_PS_NUM -1;

		if (endp < startp) {
			if (endp - 0 > SDIO_SAP_PS_NUM -1 - startp)
				index = (endp + 0) / 2;
			else
				index = (SDIO_SAP_PS_NUM -1 + startp) / 2;
		} else
			index = (startp + endp) / 2;

		regval = (u32)host->clk->ops->get_phase(host->clk->hw);
		regval &= ~SDIO_SAP_PS_MASK;
		regval |= (index << SDIO_SAP_PS_OFFSET);
		host->clk->ops->set_phase(host->clk->hw, (int)regval);

		printk(KERN_NOTICE "Tuning clk_sample[%d,%d],set[%d]\n", startp, endp, index);
	} else {
		printk(KERN_NOTICE "No valid phase shift! use default\n");
		return -1;
	}

	host->tunning = 0;
	mci_writel(host,  MCI_RINTSTS, ALL_INT_CLR);
#endif
	return 0;
}
/******************************************************************************/

unsigned int get_mmc_io_voltage(void)
{
	unsigned int voltage = 0;
	void __iomem *virtaddr;

	virtaddr = ioremap_nocache(REG_BASE_SCTL + REG_SC_GEN29, PAGE_SIZE);
	if (!virtaddr) {
		printk("ioremap emmc error.\n");
		return 1;
	}

	voltage = readl(virtaddr);
	voltage &= EMMC_IO_VOLTAGE_MASK;

	iounmap(virtaddr);

	return voltage;
}
