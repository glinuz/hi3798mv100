/*****************************************************************************/
#include "hisfc400.h"

/*****************************************************************************/
static void hisfc400_spi_search_rw(struct hisfc_nand_flash_special_dev *spiinfo,
				   struct spi_operation *spiop_rw, 
				   unsigned iftype, unsigned max_dummy, 
				   int rw_type)
{
	int ix = 0;
	struct spi_operation **spiop, **fitspiop;

	for (fitspiop = spiop = (rw_type ? spiinfo->write : spiinfo->read);
		(*spiop) && ix < MAX_SPI_NAND_OP; spiop++, ix++) {
		if (((*spiop)->iftype & iftype)
			&& ((*spiop)->dummy <= max_dummy)
			&& ((*fitspiop)->iftype < (*spiop)->iftype)) {
			fitspiop = spiop;
		}
	}
	memcpy(spiop_rw, (*fitspiop), sizeof(struct spi_operation));
}

/*****************************************************************************/
static void hisfc400_spi_get_erase(struct hisfc_nand_flash_special_dev *spiinfo,
				   struct spi_operation *spiop_erase)
{
	int ix;

	spiop_erase->size = 0;
	for (ix = 0; ix < MAX_SPI_NAND_OP; ix++) {
		if (spiinfo->erase[ix] == NULL)
			break;
		if (spiinfo->erasesize == spiinfo->erase[ix]->size) {
			memcpy(&spiop_erase[ix], spiinfo->erase[ix],
					sizeof(struct spi_operation));

			break;
		}
	}
}

/*****************************************************************************/
static void hisfc400_map_iftype_and_clock(struct hisfc_operation *spi)
{
	int ix;
	const int iftype_read[] = {
		SPI_IF_READ_STD,       HISFC400_REG_IFCYCLE_STD,
		SPI_IF_READ_DUAL,      HISFC400_REG_IFCYCLE_DUAL,
		SPI_IF_READ_DUAL_ADDR, HISFC400_REG_IFCYCLE_DUAL_ADDR,
		SPI_IF_READ_QUAD,      HISFC400_REG_IFCYCLE_QUAD,
		SPI_IF_READ_QUAD_ADDR, HISFC400_REG_IFCYCLE_QUAD_ADDR,
		0, 0,
	};
	const int iftype_write[] = {
		SPI_IF_WRITE_STD,       HISFC400_REG_IFCYCLE_STD,
		SPI_IF_WRITE_QUAD,      HISFC400_REG_IFCYCLE_QUAD,
		0, 0,
	};

	for (ix = 0; iftype_write[ix]; ix += 2) {
		if (spi->write->iftype == iftype_write[ix]) {
			spi->write->iftype = iftype_write[ix + 1];
			break;
		}
	}
	hisfc400_get_best_clock(&spi->write->clock);

	for (ix = 0; iftype_read[ix]; ix += 2) {
		if (spi->read->iftype == iftype_read[ix]) {
			spi->read->iftype = iftype_read[ix + 1];
			break;
		}
	}
	hisfc400_get_best_clock(&spi->read->clock);

	hisfc400_get_best_clock(&spi->erase->clock);

	spi->erase->iftype = HISFC400_REG_IFCYCLE_STD;
}

/*****************************************************************************/
static void hisfc400_flash_init(struct hisfc_host *host, unsigned char mfr)
{
	unsigned int regval;
	unsigned int maxtry = 3;
	unsigned int protect = 0;
	unsigned int feature = 0;
	unsigned int protect_mask = 0;

	if (mfr == NAND_MFR_WINBOND)
		protect_mask = WRITE_PROTECTION_ENABLE_WINBOND;
	else
		protect_mask = WRITE_PROTECTION_ENABLE_GIGA;

	spi_feature_op(host, GET_FEATURE, PROTECTION_REGISTER, &protect);

	if (protect & protect_mask) {
		protect &= ~protect_mask;
		do {
			spi_feature_op(host, SET_FEATURE, PROTECTION_REGISTER, &protect);

			regval = 0;
			spi_feature_op(host, GET_FEATURE, PROTECTION_REGISTER, &regval);
			if (!(regval & protect_mask))
				break;

		} while (--maxtry > 0);

		if (regval & protect_mask)
			printk("Write protection disable fail, "
				"Status Register: 0x%x\n", regval);
	}

	maxtry = 3;

	spi_feature_op(host, GET_FEATURE, FEATURE_REGISTER, &feature);

	if (feature & (FEATURE_ECC_ENABLE | FEATURE_OTP_ENABLE)
	    || ((mfr == NAND_MFR_WINBOND) && (!(feature & FEATURE_BUF_ENABLE)))) {
		feature &= ~(FEATURE_ECC_ENABLE | FEATURE_OTP_ENABLE);

		/* for WINBOND chip, force set BUF read mode. */
		if ((mfr == NAND_MFR_WINBOND) && (!(feature & FEATURE_BUF_ENABLE)))
				feature |= FEATURE_BUF_ENABLE;

		do {
			spi_feature_op(host, SET_FEATURE, FEATURE_REGISTER, &feature);

			regval = 0;
			spi_feature_op(host, GET_FEATURE, FEATURE_REGISTER, &regval);

			if (!(regval & (FEATURE_ECC_ENABLE | FEATURE_OTP_ENABLE)))
				break;

			if ((mfr == NAND_MFR_WINBOND) && (!(regval & FEATURE_BUF_ENABLE)))
				break;

		} while ((--maxtry > 0));

		if (regval & (FEATURE_ECC_ENABLE | FEATURE_OTP_ENABLE))
			printk("Internal ECC/OTP disable fail, "
				"Status Register: 0x%x.\n", regval);

		if ((mfr == NAND_MFR_WINBOND) && (!(regval & FEATURE_BUF_ENABLE)))
			printk("enable Buffer Read Mode failed, "
				"Status Register: 0x%x.\n", regval);
	}
}

/*****************************************************************************/
void hisfc400_spi_probe(struct nand_chip *chip, 
			struct hisfc_nand_flash_special_dev *spl_dev)
{
	struct hisfc_host *host = chip->priv;
	struct hisfc_operation *spi = host->spi;
	unsigned char mfr = spl_dev->id[0];

	spi->host   = host;
	spi->driver = spl_dev->driver;

	hisfc400_spi_search_rw(spl_dev, spi->read, HISFC400_SUPPORT_READ,
			HISFC400_SUPPORT_MAX_DUMMY, SPI_NAND_READ);

	hisfc400_spi_search_rw(spl_dev, spi->write, HISFC400_SUPPORT_WRITE,
			HISFC400_SUPPORT_MAX_DUMMY, SPI_NAND_WRITE);

	hisfc400_spi_get_erase(spl_dev, spi->erase);

	hisfc400_map_iftype_and_clock(spi);

	/* if chip support qe mode, set qe mode. */
	if (spi->driver->quad_enable)
		spi->driver->quad_enable(spi);

	hisfc400_flash_init(host, mfr);
}
