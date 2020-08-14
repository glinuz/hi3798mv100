/*
 * Hisilicon SoCs Reset Controller driver
 *
 * Copyright 2015
 *
 * Li Dongpo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/reset-controller.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/types.h>

#define HISI_RESET_BIT_SHIFT    0
#define HISI_RESET_BIT_WIDTH    16
#define HISI_RESET_OFFSET_SHIFT 16
#define HISI_RESET_OFFSET_WIDTH 16

struct hisilicon_reset_data {
	spinlock_t			lock;
	void __iomem			*membase;
	struct reset_controller_dev	rcdev;
};

/*31                        16                         0
  |---reset_spec->args[0]---|---reset_spec->args[1]---|
  |-------reg_offset--------|--------reg_bit----------|*/
static int hisilicon_reset_of_xlate(struct reset_controller_dev *rcdev,
		const struct of_phandle_args *reset_spec)
{
	unsigned int offset, bit, id;

	if (WARN_ON(reset_spec->args_count != rcdev->of_reset_n_cells))
		return -EINVAL;

	if (reset_spec->args[0] >= rcdev->nr_resets)
		return -EINVAL;

	offset = reset_spec->args[0] & (BIT(HISI_RESET_OFFSET_WIDTH) - 1);
	bit = reset_spec->args[1] & (BIT(HISI_RESET_BIT_WIDTH) - 1);
	id = (offset << HISI_RESET_OFFSET_SHIFT) | (bit << HISI_RESET_BIT_SHIFT);

	return id;
}

static int hisilicon_reset_deassert(struct reset_controller_dev *rcdev,
			      unsigned long id)
{
	struct hisilicon_reset_data *data = container_of(rcdev,
					struct hisilicon_reset_data,
					rcdev);
	int offset;
	int bit;
	unsigned long flags;
	u32 reg;

	offset = id >> HISI_RESET_OFFSET_SHIFT;
	offset &= (BIT(HISI_RESET_OFFSET_WIDTH) - 1);

	bit = id >> HISI_RESET_BIT_SHIFT;
	bit &= (BIT(HISI_RESET_BIT_WIDTH) - 1);

	spin_lock_irqsave(&data->lock, flags);

	reg = readl(data->membase + offset);
	writel(reg & ~BIT(bit), data->membase + offset);

	spin_unlock_irqrestore(&data->lock, flags);

	return 0;
}

static int hisilicon_reset_assert(struct reset_controller_dev *rcdev,
				unsigned long id)
{
	struct hisilicon_reset_data *data = container_of(rcdev,
					struct hisilicon_reset_data,
					rcdev);
	int offset;
	int bit;
	unsigned long flags;
	u32 reg;

	offset = id >> HISI_RESET_OFFSET_SHIFT;
	offset &= (BIT(HISI_RESET_OFFSET_WIDTH) - 1);

	bit = id >> HISI_RESET_BIT_SHIFT;
	bit &= (BIT(HISI_RESET_BIT_WIDTH) - 1);

	spin_lock_irqsave(&data->lock, flags);

	reg = readl(data->membase + offset);
	writel(reg | BIT(bit), data->membase + offset);

	spin_unlock_irqrestore(&data->lock, flags);

	return 0;
}

static struct reset_control_ops hisilicon_reset_ops = {
	.assert		= hisilicon_reset_assert,
	.deassert	= hisilicon_reset_deassert,
};

static int hisilicon_reset_init_node(struct device_node *np)
{
	struct hisilicon_reset_data *data;
	struct resource res;
	resource_size_t size;
	int ret;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	ret = of_address_to_resource(np, 0, &res);
	if (ret)
		goto err_alloc;

	size = resource_size(&res);
	if (!request_mem_region(res.start, size, np->name)) {
		ret = -EBUSY;
		goto err_alloc;
	}

	data->membase = ioremap(res.start, size);
	if (!data->membase) {
		ret = -ENOMEM;
		goto err_alloc;
	}

	spin_lock_init(&data->lock);

	data->rcdev.owner = THIS_MODULE;
	data->rcdev.nr_resets = size;
	data->rcdev.ops = &hisilicon_reset_ops;
	data->rcdev.of_node = np;
	data->rcdev.of_reset_n_cells = 2;
	data->rcdev.of_xlate = hisilicon_reset_of_xlate;

	reset_controller_register(&data->rcdev);

	return 0;

err_alloc:
	kfree(data);
	return ret;
};

/*
 * These are the reset controller we need to initialize early on in
 * our system, before we can even think of using a regular device
 * driver for it.
 */
static const struct of_device_id hisilicon_early_reset_dt_ids[] __initconst = {
	{ .compatible = "hisilicon,ahb1-reset", },
	{ /* others */ },
};

void __init hisilicon_reset_init(void)
{
	struct device_node *np;

	for_each_matching_node(np, hisilicon_early_reset_dt_ids)
		hisilicon_reset_init_node(np);
}

/*
 * And these are the controllers we can register through the regular
 * device model.
 */
static const struct of_device_id hisilicon_reset_dt_ids[] = {
	 { .compatible = "hisilicon,clock-reset", },
	 { /* others */ },
};
MODULE_DEVICE_TABLE(of, hisilicon_reset_dt_ids);

static int hisilicon_reset_probe(struct platform_device *pdev)
{
	struct hisilicon_reset_data *data;
	struct resource *res;

	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
#if 0 //TODO: hifmc should fix it
	data->membase = devm_ioremap_resource(&pdev->dev, res);
#else
	data->membase = ioremap(res->start, resource_size(res));
#endif
	if (IS_ERR(data->membase))
		return PTR_ERR(data->membase);

	spin_lock_init(&data->lock);

	data->rcdev.owner = THIS_MODULE;
	data->rcdev.nr_resets = resource_size(res);
	data->rcdev.ops = &hisilicon_reset_ops;
	data->rcdev.of_node = pdev->dev.of_node;
	data->rcdev.of_reset_n_cells = 2;
	data->rcdev.of_xlate = hisilicon_reset_of_xlate;

	return reset_controller_register(&data->rcdev);
}

static int hisilicon_reset_remove(struct platform_device *pdev)
{
	struct hisilicon_reset_data *data = platform_get_drvdata(pdev);

	reset_controller_unregister(&data->rcdev);

	return 0;
}

static struct platform_driver hisilicon_reset_driver = {
	.probe	= hisilicon_reset_probe,
	.remove	= hisilicon_reset_remove,
	.driver = {
		.name		= "hisilicon-reset",
		.owner		= THIS_MODULE,
		.of_match_table	= hisilicon_reset_dt_ids,
	},
};
module_platform_driver(hisilicon_reset_driver);

MODULE_AUTHOR("Li Dongpo");
MODULE_DESCRIPTION("Hisilicon SoCs Reset Controller Driver");
MODULE_LICENSE("GPL");
