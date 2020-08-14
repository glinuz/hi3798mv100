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
 * Create By Liu Hui 2015.6.23
 *
******************************************************************************/
#define DRVNAME "hiotg"
#define pr_fmt(fmt) DRVNAME ": " fmt

#include <linux/err.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <linux/hisilicon/hiotg.h>

#define DWC_OTG_SWITCH                        BIT(28)

#define HOST_EHCI                             0
#define HOST_OHCI                             1

struct hiotg_hcd {
	u32 usbdev_connect;
	u32 usbhost_connect;
	u32 usbehci_connect;
	u32 usbohci_connect;
	u32 usbhost_time;
	u32 usbdev_time;
	resource_size_t otg_ctrl1;
	resource_size_t otg_ctrl2;
	void __iomem * otg_crgx;
	struct task_struct *otg_task;
};

static struct hiotg_hcd *hiotg = NULL;

/******************************************************************************/

/*  CPU is in host status, check if there is device connectted. */
void set_usbhost_connect(struct usb_hcd *hcd,
			 u32 port_index, u32 online, u32 host_type)
{
	if (!hiotg)
		return;

	if (((hcd->rsrc_start == hiotg->otg_ctrl1) ||
	    (hcd->rsrc_start == hiotg->otg_ctrl2)) && (port_index == 0)) {
		if (host_type == HOST_EHCI) {
			hiotg->usbehci_connect = online;
		} else {
			hiotg->usbohci_connect = online;
		}

		hiotg->usbhost_connect = hiotg->usbehci_connect
			| hiotg->usbohci_connect;
	}
}
EXPORT_SYMBOL(set_usbhost_connect);
/******************************************************************************/

/*  CPU is in device status, check if there is host connectted. */
void set_usbdev_connect(u32 index, u32 online)
{
	if (!hiotg)
		return;

	if (index == 0)
		hiotg->usbdev_connect = online;
}
EXPORT_SYMBOL(set_usbdev_connect);
/******************************************************************************/

static int usbotg_thread(void *data)
{
	u32 reg;
	struct hiotg_hcd *hcd = (struct hiotg_hcd *)data;

	while (!kthread_should_stop()) {
		reg = readl(hcd->otg_crgx);

		if (reg & DWC_OTG_SWITCH) {
			msleep(hcd->usbdev_time);

			/* CPU is in device status */
			if (hcd->usbdev_connect)
				continue;

			reg &= ~DWC_OTG_SWITCH;
		} else {
			msleep(hcd->usbhost_time);

			/* CPU is in host status */
			if (hcd->usbhost_connect)
				continue;

			reg |= DWC_OTG_SWITCH;
		}

		writel(reg, hcd->otg_crgx);
	}

	return 0;
}
/******************************************************************************/

static int hiotg_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct device_node *np = pdev->dev.of_node;
	struct hiotg_hcd *hcd;
	int ret = 0;

	pr_info("registered new interface otg driver\n");

	hcd = kzalloc(sizeof(*hcd), GFP_KERNEL);
	if (!hcd) {
		pr_err("hcd alloc failed\n");
		return -1;
	}

	hcd->usbdev_time = 1000;
	hcd->usbhost_time = 1500;
	if (np) {
		ret = of_property_read_u32(np, "device_time", &hcd->usbdev_time);
		if (ret) {
			pr_err("get device_time failed\n");
			goto out;
		}
		ret = of_property_read_u32(np, "host_time", &hcd->usbhost_time);
		if (ret) {
			pr_err("get host_time failed\n");
			goto out;
		}
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		pr_err("no memory resource provided");
		ret = -ENXIO;
		goto out;
	}
	hcd->otg_ctrl1 = res->start;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (!res) {
		pr_err("no memory resource provided");
		ret = -ENXIO;
		goto out;
	}
	hcd->otg_ctrl2 = res->start;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 2);
	if (!res) {
		pr_err("no memory resource provided");
		ret = -ENXIO;
		goto out;
	}

	hcd->otg_crgx = devm_ioremap_resource(&pdev->dev, res);

	if (IS_ERR(hcd->otg_crgx)) {
		ret = PTR_ERR(hcd->otg_crgx);
		goto out;
	}

	hcd->otg_task = kthread_run(usbotg_thread, (void*)hcd, "usb-otg");

	if (IS_ERR(hcd->otg_task)) {
		pr_err("creating kthread failed\n");
		hcd->otg_task = NULL;
		goto out;
	}

	hiotg = hcd;
	platform_set_drvdata(pdev, hcd);

	return ret;
out:
	kfree(hcd);
	return ret;
}
/******************************************************************************/

static int hiotg_remove(struct platform_device *pdev)
{
	struct hiotg_hcd *hcd = platform_get_drvdata(pdev);

	if (hcd && hcd->otg_task) {
		kthread_stop(hcd->otg_task);
		hcd->otg_task = NULL;
	}

	if (hcd)
		kfree(hcd);

	hiotg = NULL;
	platform_set_drvdata(pdev, NULL);

	return 0;
}
/******************************************************************************/

static const struct of_device_id hiotg_ids[] = {
	{ .compatible = "hiusbotg", },
	{ /* null */ }
};
MODULE_DEVICE_TABLE(of, hiotg_ids);

static struct platform_driver hiotg_pltfm_driver = {
	.probe = hiotg_probe,
	.remove = hiotg_remove,
	.driver = {
		.name = DRVNAME,
		.owner = THIS_MODULE,
		.of_match_table = hiotg_ids,
	},
};
/******************************************************************************/

static int __init hiotg_module_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	return platform_driver_register(&hiotg_pltfm_driver);
}
module_init(hiotg_module_init);
/******************************************************************************/

static void __exit hiotg_module_exit(void)
{
	platform_driver_unregister(&hiotg_pltfm_driver);
}
module_exit(hiotg_module_exit);

MODULE_AUTHOR("Hisilicon");
MODULE_DESCRIPTION("USB 2.0 Hisilicon OTG Driver");
MODULE_LICENSE("GPL");
