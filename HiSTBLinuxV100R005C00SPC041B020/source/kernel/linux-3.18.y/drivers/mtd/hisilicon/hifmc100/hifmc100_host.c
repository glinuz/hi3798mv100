/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.6.25
 *
******************************************************************************/

#define DEVNAME  "hifmc100"
#define pr_fmt(fmt) DEVNAME": " fmt

#include <linux/hikapi.h>
#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/mtd/mtd.h>
#include <linux/hisilicon/freq.h>
#include <linux/delay.h>

#include "hifmc100_reg.h"
#include "hifmc100_host.h"

/******************************************************************************/

static struct hifmc_host *hifmc100_get_resource(struct platform_device *pdev)
{
	struct hifmc_host *host;
	struct resource *res;

	host = devm_kzalloc(&pdev->dev, sizeof(struct hifmc_host), GFP_KERNEL);
	if (!host) {
		pr_err("failed to allocate host structure.\n");
		return NULL;
	}

	platform_set_drvdata(pdev, host);

	host->dev = &pdev->dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	host->regbase = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR_OR_NULL(host->regbase)) {
		pr_err("%s: regbase ioremap fail.\n", __func__);
		return NULL;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	host->iobase = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR_OR_NULL(host->iobase)) {
		pr_err("%s: read io addr ioremap fail.\n", __func__);
		return NULL;
	}
	host->sz_iobase = res->end - res->start + 1;

	host->irq = platform_get_irq(pdev, 0);
	if (host->irq < 0) {
		pr_err("no irq defined\n");
		return NULL;
	}

	host->clk = devm_clk_get(&pdev->dev, "clk");
	if (IS_ERR_OR_NULL(host->clk)) {
		pr_err("get clock fail.\n");
		return NULL;
	}

	return host;
}
/******************************************************************************/

static void hifmc100_set_ifmode(struct hifmc_host *host, int ifmode)
{
	u32 regval;

	if (host->ifmode == ifmode)
		return;

	host->ifmode = ifmode;

	hifmc_write(host, 0, HIFMC100_DMA_SADDR_OOB);
	hifmc_write(host, 0, HIFMC100_DMA_SADDR_D0);
	hifmc_write(host, 0, HIFMC100_DMA_SADDR_D1);
	hifmc_write(host, 0, HIFMC100_DMA_SADDR_D2);
	hifmc_write(host, 0, HIFMC100_DMA_SADDR_D3);

	if (host->ifmode == HIFMC_IFMODE_INVALID)
		return;

	regval = hifmc_read(host, HIFMC100_CFG);
	regval &= ~HIFMC100_CFG_FLASH_SEL_MASK;

	switch (host->ifmode) {
	case HIFMC_IFMODE_SPINOR:
		regval |= HIFMC100_CFG_FLASH_SEL_SPINOR;
		/* dynamic clock, in every read/write/erase operation */
		break;
	case HIFMC_IFMODE_NAND:
		regval |= HIFMC100_CFG_FLASH_SEL_NAND;
		clk_set_rate(host->clk, host->nand->clk_rate);
		break;
	case HIFMC_IFMODE_SPINAND:
		regval |= HIFMC100_CFG_FLASH_SEL_SPINAND;
		clk_set_rate(host->clk, host->spinand->clk_rate);
		break;
	default:
		BUG_ON(1);
	}

	/* change to normal mode. */
	regval |= HIFMC100_CFG_OP_MODE_NORMAL;
	hifmc_write(host, regval, HIFMC100_CFG);

	/* sleep 1ms, change ifmode need delay. */
	msleep(1);
}
/******************************************************************************/

static void hifmc100_irq_enable(struct hifmc_host *host)
{
	/* clear all interrupt */
	hifmc_write(host, HIFMC100_INT_CLR_ALL, HIFMC100_INT_CLR);

	init_completion(&host->dma_done);

	/* only enable dma done interrupt. */
	hifmc_write(host, HIFMC100_INT_EN_OP_DONE, HIFMC100_INT_EN);
}
/******************************************************************************/

static int hifmc100_wait_dma_finish(struct hifmc_host *host)
{
	int ret;

	ret = wait_for_completion_timeout(&host->dma_done,
		msecs_to_jiffies(2000));
	if (!ret) {
		pr_err("wait dma transfer complete timeout, hifmc100 register:\n");
		print_hex_dump(KERN_ERR, "", DUMP_PREFIX_OFFSET, 16, 4,
			host->regbase, 0x200, false);
	}

	return ret;
}
/*****************************************************************************/

static irqreturn_t hifmc100_irq_handle(int irq, void *dev_id)
{
	struct hifmc_host *host = (struct hifmc_host *)dev_id;

	host->intr_status = hifmc_read(host, HIFMC100_INT);

	/* clear opdone interrupt */
	if (host->intr_status & HIFMC100_INT_OP_DONE) {
		hifmc_write(host, HIFMC100_INT_CLR_OP_DONE, HIFMC100_INT_CLR);
		complete(&host->dma_done);
	}

	return IRQ_HANDLED;
}
/******************************************************************************/

static int hifmc100_wait_host_ready(struct hifmc_host *host)
{
	u32 regval;
	ulong deadline = jiffies + 4 * HZ;

	do {
		regval = hifmc_read(host, HIFMC100_OP);
		if (!(regval & HIFMC100_OP_REG_OP_START))
			return 0;
	} while (!time_after_eq(jiffies, deadline));

	pr_err("wait host ready timeout, hifmc100 register:\n");
	print_hex_dump(KERN_ERR, "", DUMP_PREFIX_OFFSET, 16, 4,
		host->regbase, 0x200, false);

	return -1;
}
/******************************************************************************/

int hifmc100_write_reg(struct flash_regop_info *info)
{
	u32 regval;
	u32 cfg_val;
	u32 global_cfg_val;
	struct hifmc_host *hifmc = (struct hifmc_host *)info->priv;

	BUG_ON(!(hifmc->chipselect == 0 || hifmc->chipselect == 1));

	/* set chip select and address number */
	regval = 0;
	if (hifmc->chipselect)
		regval |= HIFMC100_OP_CFG_FM_CS;
	regval |= HIFMC100_OP_CFG_ADDR_NUM(info->addr_cycle);
	regval |= HIFMC100_OP_CFG_DUMMY_NUM(info->dummy);
	hifmc_write(hifmc, regval, HIFMC100_OP_CFG);

	/* disable ecc */
	regval = cfg_val = hifmc_read(hifmc, HIFMC100_CFG);
	regval &= ~HIFMC100_CFG_ECC_TYPE_MASK;
	hifmc_write(hifmc, regval, HIFMC100_CFG);

	hifmc_write(hifmc, 0, HIFMC100_OP_PARA);

	/* disable randomizer */
	regval = global_cfg_val = hifmc_read(hifmc, HIFMC100_GLOBAL_CFG);
	regval &= ~HIFMC100_GLOBAL_CFG_RANDOMIZER_EN;
	hifmc_write(hifmc, regval, HIFMC100_GLOBAL_CFG);

	/* set command */
	hifmc_write(hifmc, (u32)info->cmd, HIFMC100_CMD);

	/* set address */
	if (info->addr_cycle > 0) {
		if (info->addr_cycle > 4)
			hifmc_write(hifmc, info->addrh, HIFMC100_ADDRH);
		else
			hifmc_write(hifmc, 0, HIFMC100_ADDRH);

		hifmc_write(hifmc, info->addrl, HIFMC100_ADDRL);
	} else {
		/* no address parameter */
		hifmc_write(hifmc, 0, HIFMC100_ADDRH);
		hifmc_write(hifmc, 0, HIFMC100_ADDRL);
	}

	regval = HIFMC100_OP_REG_OP_START;

	if (info->dummy)
		regval |= HIFMC100_OP_DUMMY_EN;

	/* only nand have wait ready feature, spinand/spinor not support */
	if (info->wait_ready)
		regval |= HIFMC100_OP_WAIT_READY_EN;

	if (info->nr_cmd >= 1) {
		regval |= HIFMC100_OP_CMD1_EN;
		if (info->nr_cmd >= 2)
			regval |= HIFMC100_OP_CMD2_EN;
	}

	/* set data */
	if (info->sz_buf) {
		if (info->sz_buf > hifmc->sz_iobase)
			info->sz_buf = hifmc->sz_iobase;
		regval |= HIFMC100_OP_WRITE_DATA_EN;
		if (info->buf)
			memcpy(hifmc->iobase, info->buf, info->sz_buf);
	}

	hifmc_write(hifmc, HIFMC100_DATA_NUM_CNT(info->sz_buf),
		HIFMC100_DATA_NUM);

	if (info->addr_cycle)
		regval |= HIFMC100_OP_ADDR_EN;

	hifmc_write(hifmc, regval, HIFMC100_OP);

	hifmc100_wait_host_ready(hifmc);

	/* restore ecc configure for debug */
	hifmc_write(hifmc, cfg_val, HIFMC100_CFG);
	/* restore randomizer config for debug */
	hifmc_write(hifmc, global_cfg_val, HIFMC100_GLOBAL_CFG);

	return 0;
}
/******************************************************************************/

int hifmc100_read_reg(struct flash_regop_info *info)
{
	u32 regval;
	u32 cfg_val;
	u32 global_cfg_val;
	struct hifmc_host *hifmc = (struct hifmc_host *)info->priv;

	BUG_ON(!(hifmc->chipselect == 0 || hifmc->chipselect == 1));

	/* set chip select, address number, dummy */
	regval = 0;
	if (hifmc->chipselect)
		regval |= HIFMC100_OP_CFG_FM_CS;
	regval |= HIFMC100_OP_CFG_ADDR_NUM(info->addr_cycle);
	regval |= HIFMC100_OP_CFG_DUMMY_NUM(info->dummy);
	hifmc_write(hifmc, regval, HIFMC100_OP_CFG);

	/* disable ecc */
	regval = cfg_val = hifmc_read(hifmc, HIFMC100_CFG);
	regval &= ~HIFMC100_CFG_ECC_TYPE_MASK;
	hifmc_write(hifmc, regval, HIFMC100_CFG);

	hifmc_write(hifmc, 0, HIFMC100_OP_PARA);

	/* disable randomizer */
	regval = global_cfg_val = hifmc_read(hifmc, HIFMC100_GLOBAL_CFG);
	regval &= ~HIFMC100_GLOBAL_CFG_RANDOMIZER_EN;
	hifmc_write(hifmc, regval, HIFMC100_GLOBAL_CFG);

	/* set command */
	hifmc_write(hifmc, (u32)info->cmd, HIFMC100_CMD);

	/* set address */
	if (info->addr_cycle > 0) {
		if (info->addr_cycle > 4)
			hifmc_write(hifmc, info->addrh, HIFMC100_ADDRH);
		else
			hifmc_write(hifmc, 0, HIFMC100_ADDRH);

		hifmc_write(hifmc, info->addrl, HIFMC100_ADDRL);
	} else {
		hifmc_write(hifmc, 0, HIFMC100_ADDRH);
		hifmc_write(hifmc, 0, HIFMC100_ADDRL);
	}

	/* set data length */
	if (info->sz_buf > hifmc->sz_iobase)
		info->sz_buf = hifmc->sz_iobase;

	hifmc_write(hifmc, HIFMC100_DATA_NUM_CNT(info->sz_buf),
		HIFMC100_DATA_NUM);

	regval = HIFMC100_OP_REG_OP_START;

	if (info->dummy)
		regval |= HIFMC100_OP_DUMMY_EN;

	/* only nand have wait ready feature, spinand/spinor not support */
	if (info->wait_ready)
		regval |= HIFMC100_OP_WAIT_READY_EN;

	if (info->nr_cmd >= 1) {
		regval |= HIFMC100_OP_CMD1_EN;
		if (info->nr_cmd >= 2)
			regval |= HIFMC100_OP_CMD2_EN;
	}

	if (info->sz_buf)
		regval |= HIFMC100_OP_READ_DATA_EN;

	if (info->addr_cycle)
		regval |= HIFMC100_OP_ADDR_EN;

	hifmc_write(hifmc, regval, HIFMC100_OP);

	hifmc100_wait_host_ready(hifmc);

	if (info->sz_buf && info->buf)
		memcpy(info->buf, hifmc->iobase, info->sz_buf);

	/* restore ecc configure for debug */
	hifmc_write(hifmc, cfg_val, HIFMC100_CFG);
	/* restore randomizer config for debug */
	hifmc_write(hifmc, global_cfg_val, HIFMC100_GLOBAL_CFG);

	return 0;
}
/******************************************************************************/

static void hifmc100_controller_init(struct hifmc_host *host)
{
	u32 regval;

	/* disable all interrupt */
	regval = hifmc_read(host, HIFMC100_INT_EN);
	regval &= ~HIFMC100_INT_EN_ALL;
	hifmc_write(host, regval, HIFMC100_INT_EN);

	/* clean all interrupt */
	hifmc_write(host, HIFMC100_INT_CLR_ALL, HIFMC100_INT_CLR);

	/* configure dma burst width */
	hifmc_write(host, 0xF, HIFMC100_DMA_AHB_CTRL);

	/* restore default value. */
	hifmc_write(host, 0xC4, HIFMC100_GLOBAL_CFG);

	/* set nand/spinand default value */
	hifmc_write(host, 0xaaa, HIFMC100_PND_PWIDTH_CFG);

	hifmc_write(host, 0x6F, HIFMC100_TIMING_SPI_CFG);
}
/******************************************************************************/

static int hifmc100_driver_probe(struct platform_device *pdev)
{
	int ret;
	u32 regval;
	struct hifmc_host *host;

	host = hifmc100_get_resource(pdev);
	if (!host)
		return -ENODEV;

	clk_prepare_enable(host->clk);

	regval = hifmc_read(host, HIFMC100_VERSION);
	if (regval != HIFMC100_VERSION_VALUE)
		return -ENODEV;

	pr_notice("Found flash memory controller hifmc100.\n");

	hifmc100_controller_init(host);

	host->ifmode = HIFMC_IFMODE_INVALID;
	host->set_ifmode = hifmc100_set_ifmode;
	host->irq_enable = hifmc100_irq_enable;
	host->wait_dma_finish = hifmc100_wait_dma_finish;
	host->wait_host_ready = hifmc100_wait_host_ready;

	/*
	 * get ecctype and pagesize from controller,
	 * controller should not reset after boot.
	 */
	regval = hifmc_read(host, HIFMC100_CFG);
	host->reg.fmc_cfg_ecc_type = HIFMC100_CFG_ECC_TYPE_MASK & regval;
	host->reg.fmc_cfg_page_size = HIFMC100_CFG_PAGE_SIZE_MASK & regval;

	mutex_init(&host->lock);

	ret = request_irq(host->irq, hifmc100_irq_handle, 0, DEVNAME"-irq", host);
	if (ret) {
		pr_err("unable to request irq %d\n", host->irq);
		ret = -EIO;
		goto fail;
	}

	ret = hifmc100_spinor_probe(pdev, host);
	if (ret)
		pr_info("no found spi-nor device.\n");

	ret = hifmc100_xnand_probe(pdev, host, HIFMC_IFMODE_NAND);
	if (ret)
		pr_info("no found nand device.\n");

	ret = hifmc100_xnand_probe(pdev, host, HIFMC_IFMODE_SPINAND);
	if (ret)
		pr_info("no found spi-nand device.\n");

	if (!host->spinor && !host->nand && !host->spinand) {
		clk_disable(host->clk);
		ret = -ENODEV;
		goto fail;
	}

	return 0;

fail:
	mutex_destroy(&host->lock);

	return ret;
}
/*****************************************************************************/
#define DEV_FUN(_host, _dev, _fun) \
	if (_host->_dev && _host->_dev->_fun) _host->_dev->_fun(_host->_dev)

static void hifmc100_driver_shutdown(struct platform_device *pdev)
{
	struct hifmc_host *host = platform_get_drvdata(pdev);
	if (!host)
		return;

	DEV_FUN(host, spinor, shutdown);
	DEV_FUN(host, nand, shutdown);
	DEV_FUN(host, spinand, shutdown);
}
/*****************************************************************************/
#ifdef CONFIG_PM
static int hifmc100_driver_suspend(struct platform_device *pdev,
				   pm_message_t state)
{
	struct hifmc_host *host = platform_get_drvdata(pdev);
	if (!host)
		return -EINVAL;

	DEV_FUN(host, spinor, suspend);
	DEV_FUN(host, nand, suspend);
	DEV_FUN(host, spinand, suspend);

	return 0;
}
/*****************************************************************************/

static int hifmc100_driver_resume(struct platform_device *pdev)
{
	struct hifmc_host *host = platform_get_drvdata(pdev);
	if (!host)
		return -EINVAL;

	hifmc100_controller_init(host);

	DEV_FUN(host, spinor, resume);
	DEV_FUN(host, nand, resume);
	DEV_FUN(host, spinand, resume);

	return 0;
}
/*****************************************************************************/
#else
#  define hifmc100_driver_suspend NULL
#  define hifmc100_driver_resume  NULL
#endif /* CONFIG_PM */
/*****************************************************************************/

static const struct of_device_id
hisilicon_hifmc100_match[] __maybe_unused = {
	{ .compatible = "hisilicon.hifmc100", },
	{},
};
MODULE_DEVICE_TABLE(of, hisilicon_hifmc100_match);

static struct platform_driver hifmc100_pltm_driver = {
	.probe  = hifmc100_driver_probe,
	.shutdown = hifmc100_driver_shutdown,
	.suspend = hifmc100_driver_suspend,
	.resume = hifmc100_driver_resume,
	.driver = {
		.name = DEVNAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hisilicon_hifmc100_match),
	},
};
/*****************************************************************************/

static int __init hifmc100_module_init(void)
{
	pr_info("registerd new interface driver hifmc100.\n");
	return platform_driver_register(&hifmc100_pltm_driver);
}
module_init(hifmc100_module_init);

/*****************************************************************************/

static void __exit hifmc100_module_exit(void)
{
	platform_driver_unregister(&hifmc100_pltm_driver);
}
module_exit(hifmc100_module_exit);

/******************************************************************************/

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Yan Jiantao");
MODULE_AUTHOR("Cai Zhiyong");
MODULE_DESCRIPTION("Hisilicon Flash Controller V100 Device Driver");
