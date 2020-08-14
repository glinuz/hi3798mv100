/*
 * Copyright (C) Linaro 2012
 * Author: <benjamin.gaignard at linaro.org> for ST-Ericsson.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include "../ion_priv.h"

static int num_heaps;
static struct ion_device *idev;
static struct ion_heap **heaps;

struct ion_client *hisi_ion_client_create(const char *name)
{
	return ion_client_create(idev, name);
}
EXPORT_SYMBOL(hisi_ion_client_create);

void hisi_ion_client_destroy(struct ion_client *client)
{
	ion_client_destroy(client);
}
EXPORT_SYMBOL(hisi_ion_client_destroy);

static int hisi_ion_probe(struct platform_device *pdev)
{
	struct ion_platform_data *pdata = pdev->dev.platform_data;
	int err;
	int i;

	num_heaps = pdata->nr;

	heaps = kzalloc(sizeof(struct ion_heap *) * pdata->nr, GFP_KERNEL);

	idev = ion_device_create(NULL);
	if (IS_ERR_OR_NULL(idev)) {
		kfree(heaps);
		return PTR_ERR(idev);
	}

	/* create the heaps as specified in the board file */
	for (i = 0; i < num_heaps; i++) {
		struct ion_platform_heap *heap_data = &pdata->heaps[i];
		if (!heap_data->name)
			continue;

		heaps[i] = ion_heap_create(heap_data);
		if (IS_ERR_OR_NULL(heaps[i])) {
			err = PTR_ERR(heaps[i]);
			goto err;
		}
		ion_device_add_heap(idev, heaps[i]);
	}
	platform_set_drvdata(pdev, idev);
	return 0;
err:
	for (i = 0; i < num_heaps; i++) {
		if (heaps[i])
			ion_heap_destroy(heaps[i]);
	}
	kfree(heaps);
	return err;
}

static int hisi_ion_remove(struct platform_device *pdev)
{
	int i;
	struct ion_device *idev = platform_get_drvdata(pdev);

	ion_device_destroy(idev);
	for (i = 0; i < num_heaps; i++)
		ion_heap_destroy(heaps[i]);

	kfree(heaps);

	return 0;
}

static struct platform_driver hisi_ion_driver = {
	.probe = hisi_ion_probe,
	.remove = hisi_ion_remove,
	.driver = {
		.name = "hisi-ion",
	}
};

static int __init hisi_ion_init(void)
{
	hisi_register_ion_device();

	return platform_driver_register(&hisi_ion_driver);
}

static void __exit hisi_ion_exit(void)
{
	if (idev)
		ion_device_destroy(idev);

	platform_driver_unregister(&hisi_ion_driver);
}

subsys_initcall(hisi_ion_init);
module_exit(hisi_ion_exit);
