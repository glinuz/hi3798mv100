/******************************************************************************
*    Copyright (c) 2009-2011 Hisilicon.
*    All rights reserved.
* ***
*    Create by Czyong. 2011-12-02
*
******************************************************************************/

#include <common.h>
#include <nand.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <malloc.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/mtd.h>
#include <hinfc_gen.h>

#include "../hifmc100/hifmc100_host.h"
/*****************************************************************************/

#ifdef CONFIG_NAND_FLASH_HINFC301
extern int hinfc301_init(struct nand_chip *chip);
#endif

#ifdef CONFIG_NAND_FLASH_HINFC504
int hinfc504_init(struct nand_chip *chip);
extern int hinfc504_nand_rr_param_init(void);
extern int hinfc504_nand_get_rr_param(char *param);
#endif

#ifdef CONFIG_NAND_FLASH_HINFC610
int hinfc610_init(struct nand_chip *chip);
extern int hinfc610_nand_rr_param_init(void);
extern int hinfc610_nand_get_rr_param(char *param);
#endif

#ifdef CONFIG_NAND_FLASH_HISFC400
int hisfc400_init(struct nand_chip *chip);
#endif

#if (defined CONFIG_HIFMC100_NAND_SUPPORT) || (defined CONFIG_HIFMC100_SPI_NAND_SUPPORT)
extern int hifmc100_xnand_init(struct nand_chip *chip, int if_mode);
#endif

/*****************************************************************************/

int board_nand_init(struct nand_chip *chip)
{
	int ret;
	unsigned int devs = (get_cpudevs() & DEV_HINFC_MASK);

	if (!devs)
		return -ENODEV;

#ifdef CONFIG_NAND_FLASH_HINFC301
	if ((devs == DEV_HINFC_AUTO) || (devs == DEV_HINFC301)) {
		ret = hinfc301_init(chip);
		if (ret != -ENODEV)
			return ret;
	}
#endif

#ifdef CONFIG_NAND_FLASH_HINFC504
	if ((devs == DEV_HINFC_AUTO) || (devs == DEV_HINFC504)) {
		ret = hinfc504_init(chip);
		if (ret != -ENODEV)
			return ret;
	}
#endif

#ifdef CONFIG_NAND_FLASH_HINFC610
	if ((devs == DEV_HINFC_AUTO) || (devs == DEV_HINFC610)) {
		ret = hinfc610_init(chip);
		if (ret != -ENODEV)
			return ret;
	}
#endif

#ifdef CONFIG_HIFMC100_NAND_SUPPORT
	if ((devs == DEV_HINFC_AUTO) || (devs == DEV_HIFMC100)) {
		ret = hifmc100_xnand_init(chip, HIFMC_IFMODE_NAND);
		if (ret != -ENODEV)
			return ret;
	}
#endif

#ifdef CONFIG_HIFMC100_SPI_NAND_SUPPORT
	if ((devs == DEV_HINFC_AUTO) || (devs == DEV_HIFMC100)) {
		ret = hifmc100_xnand_init(chip, HIFMC_IFMODE_SPINAND);
		if (ret != -ENODEV)
			return ret;
	}
#endif

	return -ENODEV;
}

/*****************************************************************************/
#ifdef CONFIG_NAND_FLASH_HISFC400
int board_spi_nand_init(struct nand_chip *chip)
{
	int ret;
	static unsigned int devs;

	devs = (get_cpudevs() & DEV_HISFC_MASK);
	if (!devs)
		return -ENODEV;

	if (devs & DEV_HISFC400) {
		ret = hisfc400_init(chip);
		if (ret != -ENODEV)
			return ret;
	}

	return -ENODEV;
}
#endif /* #ifdef CONFIG_NAND_FLASH_HISFC400 */
/*****************************************************************************/

int nand_rr_param_init(void)
{
#if (defined(CONFIG_NAND_FLASH_HINFC504) || defined(CONFIG_NAND_FLASH_HINFC610))
	struct mtd_info_ex * nand_info = get_nand_info();
#endif

#ifdef CONFIG_NAND_FLASH_HINFC504
	if (nand_info->hostver == HINFC_VER_504)
		return hinfc504_nand_rr_param_init();
#endif /* CONFIG_NAND_FLASH_HINFC504 */

#ifdef CONFIG_NAND_FLASH_HINFC610
	if (nand_info->hostver == HINFC_VER_610)
		return hinfc610_nand_rr_param_init();
#endif /* CONFIG_NAND_FLASH_HINFC610 */

	return 0;
}
/*****************************************************************************/

int nand_get_rr_param(char *param)
{
#if (defined(CONFIG_NAND_FLASH_HINFC504) || defined(CONFIG_NAND_FLASH_HINFC610))
	struct mtd_info_ex * nand_info = get_nand_info();
#endif

#ifdef CONFIG_NAND_FLASH_HINFC504
	if (nand_info->hostver == HINFC_VER_504)
		return hinfc504_nand_get_rr_param(param);
#endif /* CONFIG_NAND_FLASH_HINFC504 */

#ifdef CONFIG_NAND_FLASH_HINFC610
	if (nand_info->hostver == HINFC_VER_610)
		return hinfc610_nand_get_rr_param(param);
#endif /* CONFIG_NAND_FLASH_HINFC610 */

	return 0;
}
