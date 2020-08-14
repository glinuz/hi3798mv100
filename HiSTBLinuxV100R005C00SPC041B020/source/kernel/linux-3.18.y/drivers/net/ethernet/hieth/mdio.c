#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/phy.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/of_mdio.h>
#include <linux/io.h>
#include <asm/irq.h>
#include <linux/uaccess.h>

#include "hieth.h"
#include "mdio.h"

/* MDIO Bus Interface */

static void hieth_mdio_init(struct hieth_netdev_priv *priv)
{
	hieth_mdio_lock_init(priv);
	mdio_reg_reset(priv);
}

static void hieth_mdio_exit(struct hieth_netdev_priv *priv)
{
	hieth_mdio_lock_exit(priv);
}

static int hieth_wait_mdio_ready(struct hieth_netdev_priv *priv)
{
	int timeout_us = 1000;

	while (--timeout_us && !mdio_test_ready(priv))
		udelay(1);

	return timeout_us;
}

int hieth_mdiobus_read(struct mii_bus *bus, int phy_addr, int regnum)
{
	int val = 0;
	struct hieth_netdev_priv *priv = bus->priv;

	hieth_mdio_lock(priv);

	if (!hieth_wait_mdio_ready(priv)) {
		pr_err("mdio busy\n");
		goto error_exit;
	}

	mdio_start_phyread(priv, phy_addr, regnum);

	if (hieth_wait_mdio_ready(priv))
		val = mdio_get_phyread_val(priv);
	else
		pr_err("read timeout\n");

error_exit:

	hieth_mdio_unlock(priv);

	pr_debug("phy_addr = %d, regnum = %d, val = 0x%04x\n", phy_addr,
		    regnum, val);

	return val;
}

int hieth_mdiobus_write(struct mii_bus *bus, int phy_addr, int regnum,
			       u16 val)
{
	int ret = 0;
	struct hieth_netdev_priv *priv = bus->priv;

	pr_debug("phy_addr = %d, regnum = %d\n", phy_addr, regnum);

	hieth_mdio_lock(priv);

	if (!hieth_wait_mdio_ready(priv)) {
		pr_err("mdio busy\n");
		ret = -1;
		goto error_exit;
	}

	mdio_phywrite(priv, phy_addr, regnum, val);

error_exit:

	hieth_mdio_unlock(priv);

	return val;
}

static int hieth_mdiobus_reset(struct mii_bus *bus)
{
	struct hieth_netdev_priv *priv = bus->priv;

	mdio_reg_reset(priv);
	return 0;
}

int hieth_mdiobus_driver_init(struct platform_device *pdev,
			      struct hieth_netdev_priv *priv)
{
	int phy, ret = 0;
	struct mii_bus *bus;
	struct device *dev = &pdev->dev;
	struct device_node *node = dev->of_node;

	hieth_mdio_init(priv);

	/* register MII bus */
	bus = mdiobus_alloc();
	if (!bus) {
		pr_err("get ioresource failed!\n");
		ret = -ENOMEM;
		goto _error_exit;
	}

	bus->name = HIETH_MIIBUS_NAME;

	snprintf(bus->id, MII_BUS_ID_SIZE, "%s", bus->name);
	bus->read = hieth_mdiobus_read;
	bus->write = hieth_mdiobus_write;
	bus->reset = hieth_mdiobus_reset;
	bus->priv = priv;
	priv->mii_bus = bus;
	bus->parent = &pdev->dev;	/*for Power Management */

	bus->irq = kmalloc(sizeof(int) * PHY_MAX_ADDR, GFP_KERNEL);
	if (!bus->irq) {
		ret = -ENOMEM;
		goto _error_free_mdiobus;
	}

	for (phy = 0; phy < PHY_MAX_ADDR; phy++)
		bus->irq[phy] = PHY_POLL;

	hieth_fix_festa_phy_trim(bus);

	ret = of_mdiobus_register(bus, node);
	if (ret) {
		pr_err("failed to register MDIO bus\n");
		goto _error_free_mdiobus;
	}

	return 0;

_error_free_mdiobus:
	kfree(bus->irq);
	mdiobus_free(bus);

_error_exit:
	return ret;
}

void hieth_mdiobus_driver_exit(struct hieth_netdev_priv *priv)
{
	struct mii_bus *bus = priv->mii_bus;

	mdiobus_unregister(bus);
	kfree(bus->irq);
	mdiobus_free(bus);
	hieth_mdio_exit(priv);
}

/* vim: set ts=8 sw=8 tw=78: */
