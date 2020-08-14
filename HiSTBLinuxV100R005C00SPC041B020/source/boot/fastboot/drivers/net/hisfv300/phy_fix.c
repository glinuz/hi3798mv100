#include "hieth.h"
#include "mdio.h"
#include "mac.h"
#include "ctrl.h"
#include "glb.h"
#include "sys.h"
#include <config.h>
#include <miiphy.h>
#include <net.h>

#include "phy_fix.h"

//#define CONFIG_FEPHY_TRIM 1

/*
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000ffffe00000.
 */
#define GENMASK(h, l) \
	(((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

#define REG_LD_AM		0x3050
#define LD_AM_MASK		GENMASK(4, 0)
#define REG_LDO_AM		0x3051
#define LDO_AM_MASK		GENMASK(2, 0)
#define REG_R_TUNING		0x3052
#define R_TUNING_MASK		GENMASK(5, 0)
#define REG_WR_DONE		0x3053
#define REG_DEF_ATE		0x3057

static int hieth_use_default_ate(struct eth_device *dev)
{
#if 0
	struct hieth_netdev_local *ld = (struct hieth_netdev_local *)dev->priv;
	unsigned short v = 0;
	int timeout = 3;

	do {
		udelay(250 * 1000);
		v = phy_expanded_read(dev, REG_DEF_ATE);
		v &= (1 << 0);
	} while (!v && --timeout);
	if (!timeout)
		printf("festav271 0x3057 wait bit0 timeout!\n");
#endif
	return 0;
}

static int hieth_config_festa_phy_ate(struct eth_device *dev)
{
#if 0
	struct hieth_netdev_local *ld = (struct hieth_netdev_local *)dev->priv;
	void *otp_base;
	u32 ate_params;
	unsigned short ld_amptlitude;
	unsigned short ldo_amptlitude;
	unsigned short r_tuning_val;
	unsigned short v = 0;
	int timeout = 3000;

	ld_amptlitude = DEF_LD_AM;
	ldo_amptlitude = DEF_LDO_AM;
	r_tuning_val = DEF_R_TUNING;

	otp_base = (void *)REG_BASE_OTP;
	ate_params = readl(otp_base + OTP_FEPHY_ATE);

	printf("fephy otp value %08x\n", ate_params);

	if (!ate_params)
		return hieth_use_default_ate(dev);

	ld_amptlitude = ate_params & LD_AM_MASK;
	ldo_amptlitude = (ate_params >> 8) & LDO_AM_MASK;
	r_tuning_val = (ate_params >> 16) & R_TUNING_MASK;

	if (miiphy_read(dev->name, UD_REG_NAME(PHY_ADDR), PHY_BMCR, &v))
		return -1;
	v |= PHY_BMCR_POWD;
	if (miiphy_write(dev->name, UD_REG_NAME(PHY_ADDR), PHY_BMCR, v))
		return -1;

	v = phy_expanded_read(dev, REG_LD_AM);
	v = (v & ~LD_AM_MASK) | (ld_amptlitude & LD_AM_MASK);
	phy_expanded_write(dev, REG_LD_AM, v);

	v = phy_expanded_read(dev, REG_LDO_AM);
	v = (v & ~LDO_AM_MASK) | (ldo_amptlitude & LDO_AM_MASK);
	phy_expanded_write(dev, REG_LDO_AM, v);

	v = phy_expanded_read(dev, REG_R_TUNING);
	v = (v & ~R_TUNING_MASK) | (r_tuning_val & R_TUNING_MASK);
	phy_expanded_write(dev, REG_R_TUNING, v);

	v = phy_expanded_read(dev, REG_WR_DONE);
	if (v & (1 << 1))
		printf("festav271 0x3053 bit1 CFG_ACK value: 1\n");
	v = v | (1 << 0);
	phy_expanded_write(dev, REG_WR_DONE, v);

	do {
		udelay(100);
		v = phy_expanded_read(dev, REG_WR_DONE);
		v &= (1 << 1);
	} while (!v && --timeout);
	if (!timeout)
		printf("festav271 0x3053 wait bit1 CFG_ACK timeout!\n");

	if (miiphy_read(dev->name, UD_REG_NAME(PHY_ADDR), PHY_BMCR, &v))
		return -1;
	v &= (~PHY_BMCR_POWD);
	if (miiphy_write(dev->name, UD_REG_NAME(PHY_ADDR), PHY_BMCR, v))
		return -1;
#endif
	return 0;
}

#ifdef CONFIG_FEPHY_TRIM
int hieth_fix_festa_phy_ate(struct eth_device *dev)
{
	long long chipid = get_chipid();
	if ((_HI3716M_V330 == chipid) 
	    || (_HI3798M_V200 == chipid) 
	    || (_HI3798M_A_V200 == chipid) 
	    || (_HI3796M_V200 == chipid))
		hieth_config_festa_phy_ate(dev);

	return 0;
}
#else
int hieth_fix_festa_phy_ate(struct eth_device *dev)
{
	mdelay(300);
	return 0;
}
#endif
