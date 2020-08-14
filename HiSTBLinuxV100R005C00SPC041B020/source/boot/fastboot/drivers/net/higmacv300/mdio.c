#include <asm/errno.h>
#include "mdio.h"
#include "util.h"
#include "higmac.h"

static int wait_mdio_ready(struct higmac_netdev_local *ld)
{
	int timeout_us = 1000;

	while (--timeout_us && !test_mdio_ready(ld))
		udelay(1);

	return timeout_us;
}

static int higmac_mdio_read(struct higmac_netdev_local *ld,
		char *devname, unsigned char phy,
		unsigned char reg, unsigned short *value)
{
	int timeout = 1000;

	if (!wait_mdio_ready(ld))
		return -1;

	mdio_start_phyread(ld, phy, reg);

	while (!wait_mdio_ready(ld) && timeout-- > 0)
		udelay(1);

	if (timeout <= 0 || !test_mdio_read_data_done(ld)) {
		*value = 0;
		/* it should return Error(-1), but miiphy_read() will
		 * print error info, it's annoying
		 */
		return 0;
	}

	*value = mdio_get_phyread_val(ld);

	higmac_trace(2, "mdio read phy:%x, reg:%x = %x\n", phy, reg, *value);

	return 0;
}

static int higmac_mdio_write(struct higmac_netdev_local *ld,
		char *devname, unsigned char phy,
		unsigned char reg, unsigned short val)
{
	if (!wait_mdio_ready(ld))
		return -1;

	higmac_trace(2, "mdio write phy:%x, reg:%x = %x\n", phy, reg, val);

	mdio_set_phywrite_val(ld, val);
	mdio_phywrite(ld, phy, reg);

	return 0;
}

static struct higmac_netdev_local *higmac_get_netdev_by_name(char *devname)
{
	int i;

	for (i = 0; i < CONFIG_GMAC_NUMS; i++) {
		if (!strcmp(devname, higmac_board_info[i].mii_name))
			return &higmac_board_info[i].higmac_ld;
	}

	return NULL;
}

int higmac_mdiobus_read(char *devname, unsigned char phy,
        unsigned char reg, unsigned short *value)
{
	struct higmac_netdev_local *ld = higmac_get_netdev_by_name(devname);

	if (!ld)
		return -EINVAL;

	return higmac_mdio_read(ld, devname, phy, reg, value);
}

int higmac_mdiobus_write(char *devname, unsigned char phy,
		unsigned char reg, unsigned short val)
{
	struct higmac_netdev_local *ld = higmac_get_netdev_by_name(devname);

	if (!ld)
		return -EINVAL;

	return higmac_mdio_write(ld, devname, phy, reg, val);
}
