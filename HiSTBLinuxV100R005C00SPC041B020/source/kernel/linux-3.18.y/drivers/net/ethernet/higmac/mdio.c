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

int higmac_mdio_read(struct mii_bus *bus, int phy, int reg)
{
	struct higmac_netdev_local *ld = bus->priv;
	int timeout = 1000;
	int val;

	if (!wait_mdio_ready(ld))
		return 0;

	mdio_start_phyread(ld, phy, reg);

	while (!wait_mdio_ready(ld) && timeout-- > 0)
		udelay(1);

	if (timeout <= 0 || !test_mdio_read_data_done(ld))
		return 0;

	val = mdio_get_phyread_val(ld);

	higmac_trace(2, "mdio read phy:%x, reg:%x = %x\n", phy, reg, val);

	return val;
}

int higmac_mdio_write(struct mii_bus *bus, int phy, int reg, u16 val)
{
	struct higmac_netdev_local *ld = bus->priv;

	if (!wait_mdio_ready(ld))
		return 0;

	higmac_trace(2, "mdio write phy:%x, reg:%x = %x\n", phy, reg, val);

	mdio_set_phywrite_val(ld, val);
	mdio_phywrite(ld, phy, reg);

	return val;
}
