/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-02-07
 *
******************************************************************************/

#define DRVNAME    "hinand"
#define pr_fmt(fmt) DRVNAME ": " fmt

#include "hinfc610_os.h"
#include "hinfc610.h"
#include "hinfc610_cache.h"
#include <linux/err.h>
#include <linux/completion.h>
#include <linux/of.h>
#include <linux/kernel.h>
#include <mach/hardware.h>
/*****************************************************************************/
#define MAX_MTD_PARTITIONS         (32)

struct partition_entry
{
	char name[16];
	unsigned long long start;
	unsigned long long length;
	unsigned int flags;
};

struct partition_info
{
	int parts_num;
	struct partition_entry entry[MAX_MTD_PARTITIONS];
	struct mtd_partition parts[MAX_MTD_PARTITIONS];
};

#ifdef CONFIG_MTD_PART_CHANGE
extern int register_mtd_partdev(struct mtd_info *mtd);
extern int unregister_mtd_partdev(struct mtd_info *mtd);
#else
static int register_mtd_partdev(struct mtd_info *mtd){ return 0; };
static int unregister_mtd_partdev(struct mtd_info *mtd){return 0;};
#endif

static const char *part_probes_type[] = { "cmdlinepart", NULL, };

static struct partition_info ptn_info = {0};

/*****************************************************************************/
static int __init parse_nand_partitions(const struct tag *tag)
{
	int i;

	if (tag->hdr.size <= 2) {
		hinfc_pr_bug("tag->hdr.size <= 2\n");
		return 0;
	}
	ptn_info.parts_num = (tag->hdr.size - 2)
		/ (sizeof(struct partition_entry)/sizeof(int));
	memcpy(ptn_info.entry,
		&tag->u,
		ptn_info.parts_num * sizeof(struct partition_entry));

	for (i = 0; i < ptn_info.parts_num; i++) {
		ptn_info.parts[i].name   = ptn_info.entry[i].name;
		ptn_info.parts[i].size   = (ptn_info.entry[i].length);
		ptn_info.parts[i].offset = (ptn_info.entry[i].start);
		ptn_info.parts[i].mask_flags = 0;
		ptn_info.parts[i].ecclayout  = 0;
	}

	return 0;
}

/* turn to ascii is "HiNp" */
__tagtable(0x48694E70, parse_nand_partitions);
/*****************************************************************************/
static unsigned int  nand_otp_len = 0;
static unsigned char nand_otp[128] = {0};

/* Get NAND parameter table. */
static int __init parse_nand_param(const struct tag *tag)
{
	if (tag->hdr.size <= 2)
		return 0;

	nand_otp_len = ((tag->hdr.size << 2) - sizeof(struct tag_header));

	if (nand_otp_len > sizeof(nand_otp)) {
		hinfc_pr_bug("tag->hdr.size <= 2\n");
		return 0;
	}
	memcpy(nand_otp, &tag->u, nand_otp_len);
	return 0;
}
/* 0x48694E77 equal to fastoot ATAG_NAND_PARAM */
__tagtable(0x48694E77, parse_nand_param);
/*****************************************************************************/

static int hinfc610_os_enable(struct hinfc_host *host, int enable)
{
	if (enable == ENABLE)
		clk_prepare_enable(host->clk);
	else
		clk_disable_unprepare(host->clk);
	return 0;
}
/*****************************************************************************/

static int hinfc610_os_probe(struct platform_device * pltdev)
{
	int size;
	int result = 0;
	struct hinfc_host *host;
	struct nand_chip *chip;
	struct mtd_info *mtd;
	struct resource *res;
	size = sizeof(struct hinfc_host) + sizeof(struct nand_chip)
		+ sizeof(struct mtd_info);
	host = devm_kzalloc(&pltdev->dev, size, GFP_KERNEL);
	if (!host) {
		pr_err("%s: allocate memroy fail.\n", __func__);
		return -ENOMEM;
	}
	memset((char *)host, 0, size);

	platform_set_drvdata(pltdev, host);

	host->dev  = &pltdev->dev;
	host->chip = chip = (struct nand_chip *)&host[1];
	host->mtd  = mtd  = (struct mtd_info *)&chip[1];

	res = platform_get_resource(pltdev, IORESOURCE_MEM, 0);
	host->iobase = devm_ioremap_resource(&pltdev->dev, res);
	if (IS_ERR_OR_NULL(host->iobase)) {
		pr_err("%s: regbase ioremap fail.\n", __func__);
		return PTR_ERR(host->iobase);
	}

	mtd->priv  = chip;
	mtd->owner = THIS_MODULE;
	mtd->name  = DRVNAME;

	res = platform_get_resource(pltdev, IORESOURCE_MEM, 1);

	chip->IO_ADDR_R = chip->IO_ADDR_W = ioremap_nocache(res->start,
		res->end - res->start + 1);
	if (!chip->IO_ADDR_R) {
		hinfc_pr_bug("ioremap failed\n");
		return -EIO;
	}

	host->clk = devm_clk_get(&pltdev->dev, "clk");
	if (IS_ERR_OR_NULL(host->clk)) {
		pr_err("get clock fail.\n");
		return PTR_ERR(host->clk);
	}

	host->enable = hinfc610_os_enable;

	host->enable(host, ENABLE);

	chip->priv        = host;
	host->chip        = chip;
	chip->cmd_ctrl    = hinfc610_cmd_ctrl;
	chip->dev_ready   = hinfc610_dev_ready;
	chip->select_chip = hinfc610_select_chip;
	chip->read_byte   = hinfc610_read_byte;
	chip->read_word   = hinfc610_read_word;
	chip->write_buf   = hinfc610_write_buf;
	chip->read_buf    = hinfc610_read_buf;

	chip->chip_delay = HINFC610_CHIP_DELAY;
	chip->options    = NAND_NEED_READRDY | NAND_SKIP_BBTSCAN;
	chip->ecc.layout = NULL;
	chip->ecc.mode   = NAND_ECC_NONE;

	if (hinfc610_nand_init(host, chip)) {
		hinfc_pr_bug("failed to allocate device buffer.\n");
		return -EIO;
	}

	if (nand_otp_len) {
		hinfc_pr_msg("Copy Nand read retry parameter from boot,"
		       " parameter length %d.\n", nand_otp_len);
		memcpy(host->rr_data, nand_otp, nand_otp_len);
	}

	if (nand_scan(mtd, CONFIG_HINFC610_MAX_CHIP)) {
		result = -ENXIO;
		goto fail;
	}

	register_mtd_partdev(host->mtd);

	if (!mtd_device_parse_register(mtd, part_probes_type,
		NULL, ptn_info.parts, ptn_info.parts_num))
		return 0;

	unregister_mtd_partdev(host->mtd);

	result = -ENODEV;
	nand_release(mtd);

fail:
	hinfc610_nand_destory(host);
	host->enable(host, DISABLE);

	iounmap(chip->IO_ADDR_W);
	iounmap(host->iobase);
	devm_kfree(&pltdev->dev, host);
	platform_set_drvdata(pltdev, NULL);

	return result;
}
/*****************************************************************************/

static int hinfc610_os_remove(struct platform_device *pltdev)
{
	struct hinfc_host *host = platform_get_drvdata(pltdev);

	host->enable(host, DISABLE);

	hinfc610_nand_destory(host);

	unregister_mtd_partdev(host->mtd);

	nand_release(host->mtd);

	iounmap(host->chip->IO_ADDR_W);
	iounmap(host->iobase);
	kfree(host);
	platform_set_drvdata(pltdev, NULL);

	return 0;
}
/*****************************************************************************/

#ifdef CONFIG_PM
static int hinfc610_os_suspend(struct platform_device *pltdev,
			       pm_message_t state)
{
	struct hinfc_host *host = platform_get_drvdata(pltdev);

	while ((hinfc_read(host, HINFC610_STATUS ) & 0x1) == 0x0)
		;

	while ((hinfc_read(host, HINFC610_DMA_CTRL))
		& HINFC610_DMA_CTRL_DMA_START)
		_cond_resched();

	host->enable(host, DISABLE);

	return 0;
}
/*****************************************************************************/

static int hinfc610_os_resume(struct platform_device *pltdev)
{
	int cs;
	struct hinfc_host *host = platform_get_drvdata(pltdev);
	struct nand_chip *chip = host->chip;

	host->enable(host, ENABLE);
	for (cs = 0; cs < chip->numchips; cs++)
		host->send_cmd_reset(host, cs);
	hinfc_write(host,
		SET_HINFC610_PWIDTH(CONFIG_HINFC610_W_LATCH,
			CONFIG_HINFC610_R_LATCH, CONFIG_HINFC610_RW_LATCH),
		HINFC610_PWIDTH);

	return 0;
}
#endif /* CONFIG_PM */
/*****************************************************************************/

static const struct of_device_id
hi3716cv200_hinfc610_match[] __maybe_unused = {
	{ .compatible = "hi3716cv200.hinfc610", },
	{ .compatible = "hi3798mv100.hinfc610", },
	{},
};
MODULE_DEVICE_TABLE(of, hi3716cv200_hinfc610_match);

static struct platform_driver hinfc610_os_pltdrv =
{
	.probe  = hinfc610_os_probe,
	.remove = hinfc610_os_remove,
#ifdef CONFIG_PM
	.suspend  = hinfc610_os_suspend,
	.resume   = hinfc610_os_resume,
#endif /* CONFIG_PM */
	.driver = {
		.name = DRVNAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hi3716cv200_hinfc610_match),
	},
};
/*****************************************************************************/

static int __init hinfc610_os_module_init(void)
{
	pr_notice("registerd new interface driver hinfc610.\n");
	return platform_driver_register(&hinfc610_os_pltdrv);
}
/*****************************************************************************/

static void __exit hinfc610_os_module_exit (void)
{
	platform_driver_unregister(&hinfc610_os_pltdrv);
}
/*****************************************************************************/

module_init(hinfc610_os_module_init);
module_exit(hinfc610_os_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Czyong");
MODULE_DESCRIPTION("Hisilicon Nand Flash Controller V610 Device Driver,"
	" Version 1.30");

/*****************************************************************************/
