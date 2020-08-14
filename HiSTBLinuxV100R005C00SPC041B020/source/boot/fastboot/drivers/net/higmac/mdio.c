#include "mdio.h"
#include "util.h"
#include "higmac.h"

static int wait_mdio_ready(struct higmac_mdio_local* ld)
{
	int timeout_us = 1000;

	while (--timeout_us && !test_mdio_ready(ld))
	{
		udelay(1);
	}

	return timeout_us;
}

int higmac_mdio_read(struct higmac_mdio_local* ld, int phy, int reg)
{
	int timeout = 1000;
	int val = 0;
	
	if (!wait_mdio_ready(ld))
	{
		return 0;
	}
	mdio_start_phyread(ld, phy, reg);

	while (!wait_mdio_ready(ld) && timeout-- > 0)
		udelay(1);

	if (timeout <= 0 || !test_mdio_read_data_done(ld))
	{
		return 0;
	}

	val = mdio_get_phyread_val(ld);

	higmac_trace(2, "mdio read phy:%x, reg:%x = %x\n",phy,reg, val);
	
	return val;
}

int higmac_mdio_write(struct higmac_mdio_local *ld, int phy, int reg, int val)
{	
	if (!wait_mdio_ready(ld))
	{
		return 0;
	}
	
	higmac_trace(2, "mdio write phy:%x, reg:%x = %x\n", phy, reg, val);
	
	mdio_set_phywrite_val(ld, val);
	mdio_phywrite(ld, phy, reg);
	
	return val;
}
/*FIXME: check the system control reg and fix it*/
int  higmac_mdio_reset(struct higmac_mdio_local *ld)
{
#if 0
	unsigned char* p = (unsigned char*)IO_ADDRESS(HIGMAC_SYS_CTL_IO_BASE);
	volatile unsigned int v = 0;

	local_lock(ld);
	v = readl(p + SC_PERCTRL8);
	v &= (~(1 << 19));
	writel(v, p + SC_PERCTRL8);
	local_unlock(ld);

	udelay(10);

	local_lock(ld);
	v = readl(p + SC_PERCTRL8);
	v |= (1 << 19);
	writel(v, p + SC_PERCTRL8);
	local_unlock(ld);
#endif	

	return 0;
}

int  higmac_mdio_init(struct higmac_mdio_local *ld)
{	
	return   higmac_mdio_reset(ld);
}

void higmac_mdio_exit(struct higmac_mdio_local *ld)
{

}

