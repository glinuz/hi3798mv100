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
 * Create By Cai Zhiyong 2015.6.17
 *
******************************************************************************/

#define DRVNAME    "hiahci-phy"
#define pr_fmt(fmt) DRVNAME ": " fmt

#include <linux/io.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/phy/phy.h>
#include <linux/platform_device.h>

#include <linux/delay.h>

/******************************************************************************/

struct hiahci_phy_priv {
	void __iomem *iobase;
	void __iomem *peri_ctrl_base;
};
/******************************************************************************/

static int hiahci_phy_init(struct phy *phy)
{
	struct hiahci_phy_priv *priv = phy_get_drvdata(phy);

	/* Config and reset the SATA PHY, SSC enabled */
	writel(0x49000679, priv->iobase + 0xA0);
	msleep(10);

	writel(0x49000678, priv->iobase + 0xA0);
	msleep(10);

	/* Config PHY controller register 1 */
	writel(0x345cb8, priv->iobase + 0x148);
	msleep(10);

	/* Config PHY controller register 2, and reset SerDes lane */
	writel(0x00060545, priv->iobase + 0x14C);
	msleep(10);
	writel(0x00020545, priv->iobase + 0x14C);
	msleep(10);

	/* Data invert between phy and sata controller*/
	writel(0x8, priv->iobase + 0xA4);
	msleep(10);

	/* Config Spin-up */
	writel(0x600000, priv->iobase + 0x118);
	msleep(10);
	writel(0x600002, priv->iobase + 0x118);
	msleep(10);

	/*
	 * Config SATA Port phy controller.
	 * To take effect for 0xF990014C, 
	 * we should force controller to 1.5G mode first
	 * and then force it to 6G mode.
	 */
	writel(0xE100000, priv->iobase + 0x174);
	msleep(10);
	writel(0xE5A0000, priv->iobase + 0x174);
	msleep(10);
	writel(0xE4A0000, priv->iobase + 0x174);
	msleep(10);
	writel(0xE250000,priv->iobase + 0x174);
	msleep(10);

	return 0;
}
/******************************************************************************/

static int hiahci_phy_power_on(struct phy *phy)
{
	u32 regval;
	struct hiahci_phy_priv *priv = phy_get_drvdata(phy);

	/* Power on SATA disk */
	regval = readl(priv->peri_ctrl_base + 0x08);
	regval |= BIT(10);
	writel(regval, priv->peri_ctrl_base + 0x08);

	return 0;
}
/******************************************************************************/

static int hiahci_phy_power_off(struct phy *phy)
{
	u32 regval;
	struct hiahci_phy_priv *priv = phy_get_drvdata(phy);

	/* Power off SATA disk */
	regval = readl(priv->peri_ctrl_base + 0x08);
	regval &= ~BIT(10);
	writel(regval, priv->peri_ctrl_base + 0x08);

	return 0;
}
/******************************************************************************/

static struct phy_ops hiahci_phy_ops = {
	.init = hiahci_phy_init,
	.power_on = hiahci_phy_power_on,
	.power_off = hiahci_phy_power_off,
	.owner = THIS_MODULE,
};
/******************************************************************************/

static int hiahci_phy_probe(struct platform_device *pdev)
{
	struct phy *phy;
	struct resource *res;
	struct phy_provider *phy_provider;
	struct hiahci_phy_priv *priv;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	priv->iobase = devm_ioremap(&pdev->dev, res->start, resource_size(res));
	if (IS_ERR_OR_NULL(priv->iobase)) {
		pr_err("%s: iobase ioremap fail.\n", __func__);
		return PTR_ERR(priv->iobase);
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	priv->peri_ctrl_base = devm_ioremap(&pdev->dev, res->start, resource_size(res));
	if (IS_ERR_OR_NULL(priv->peri_ctrl_base)) {
		pr_err("%s: peri_ctrl_base ioremap fail.\n", __func__);
		return PTR_ERR(priv->peri_ctrl_base);
	}

	phy = devm_phy_create(&pdev->dev, NULL, &hiahci_phy_ops, NULL);
	if (IS_ERR_OR_NULL(phy)) {
		pr_err("failed to create PHY, %ld\n", PTR_ERR(phy));
		return PTR_ERR(phy);
	}

	phy_set_drvdata(phy, priv);
	phy_provider = devm_of_phy_provider_register(&pdev->dev,
		of_phy_simple_xlate);
	if (IS_ERR_OR_NULL(phy_provider)) {
		pr_err("failed to register phy provider, %ld\n", PTR_ERR(phy));
		return PTR_ERR(phy_provider);
	}

	return 0;
}
/******************************************************************************/

static const struct of_device_id hi3716cv200_hiahci_phy_of_match[] = {
	{.compatible = "hi3716cv200.hiahci-phy",},
	{ },
};
MODULE_DEVICE_TABLE(of, hi3716cv200_hiahci_phy_of_match);

static struct platform_driver hiahci_phy_driver = {
	.probe  = hiahci_phy_probe,
	.driver = {
		.name = DRVNAME,
		.of_match_table = hi3716cv200_hiahci_phy_of_match,
	}
};
/*****************************************************************************/

static int __init hiahci_phy_module_init(void)
{
	pr_info("registered new sata phy driver\n");
	return platform_driver_register(&hiahci_phy_driver);
}
module_init(hiahci_phy_module_init);
/*****************************************************************************/

static void __exit hiahci_phy_module_exit(void)
{
	platform_driver_unregister(&hiahci_phy_driver);
}
module_exit(hiahci_phy_module_exit);

MODULE_AUTHOR("Cai Zhiyong");
MODULE_DESCRIPTION("Hisilicon hi3716cv200 sata phy driver");
MODULE_LICENSE("GPL v2");
