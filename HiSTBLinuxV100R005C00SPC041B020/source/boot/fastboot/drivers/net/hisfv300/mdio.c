
#define HIETH_SFV300

#include "hieth.h"
#include "mdio.h"

static int wait_mdio_ready(struct hieth_mdio_local *ld)
{
	int timeout_us = 100;

	while (--timeout_us && !test_mdio_ready(ld))
		udelay(1);

	return timeout_us;
}

int hieth_mdio_read(struct hieth_mdio_local *ld, int phy_addr, int regnum)
{
	int val = 0;
	hieth_assert((!(phy_addr & (~0x1F))) && (!(regnum & (~0x1F))));

	local_lock(ld);

	if (!wait_mdio_ready(ld)) {
		hieth_error("mdio busy");
		goto error_exit;
	}

	mdio_start_phyread(ld, phy_addr, regnum);

	if (wait_mdio_ready(ld))
		val = mdio_get_phyread_val(ld);
	else
		hieth_error("read timeout");

error_exit:

	local_unlock(ld);

	hieth_trace(4, "phy_addr = %d, regnum = %d, val = 0x%04x", phy_addr,
		    regnum, val);

	return val;
}

int hieth_mdio_write(struct hieth_mdio_local *ld, int phy_addr, int regnum,
		     int val)
{
	hieth_assert((!(phy_addr & (~0x1F))) && (!(regnum & (~0x1F))));

	hieth_trace(4, "phy_addr = %d, regnum = %d", phy_addr, regnum);

	local_lock(ld);

	if (!wait_mdio_ready(ld)) {
		hieth_error("mdio busy");
		goto error_exit;
	}

	mdio_phywrite(ld, phy_addr, regnum, val);

error_exit:

	local_unlock(ld);

	return val;
}

int hieth_mdio_reset(struct hieth_mdio_local *ld)
{
	mdio_reg_reset(ld);

	return 0;
}

int hieth_mdio_init(struct hieth_mdio_local *ld)
{
	local_lock_init(ld);

	hieth_mdio_reset(ld);

	return 0;
}

void hieth_mdio_exit(struct hieth_mdio_local *ld)
{
	local_lock_exit(ld);
}
