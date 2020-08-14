
#define HIETH_SFV300

#include "hieth.h"
#include "mac.h"

static int _set_linkstat(struct hieth_netdev_local *ld, int mode)
{
	int old;

	old = hieth_readl_bits(ld, UD_REG_NAME(MAC_PORTSET), BITS_MACSTAT);
	hieth_writel_bits(ld, mode, UD_REG_NAME(MAC_PORTSET), BITS_MACSTAT);

	return old;
}

static int _set_negmode(struct hieth_netdev_local *ld, int mode)
{
	int old;

	old = hieth_readl_bits(ld, UD_REG_NAME(MAC_PORTSEL), BITS_NEGMODE);
	hieth_writel_bits(ld, mode, UD_REG_NAME(MAC_PORTSEL), BITS_NEGMODE);

	return old;
}

static int _get_negmode(struct hieth_netdev_local *ld)
{
	int old;

	old = hieth_readl_bits(ld, UD_REG_NAME(MAC_PORTSEL), BITS_NEGMODE);

	return old;
}

int hieth_set_linkstat(struct hieth_netdev_local *ld, int mode)
{
	unsigned long old;

	local_lock(ld);
	old = _set_linkstat(ld, mode);
	local_unlock(ld);

	return old;
}

int hieth_get_linkstat(struct hieth_netdev_local *ld)
{
	unsigned long old;

	local_lock(ld);
	old = hieth_readl_bits(ld, UD_REG_NAME(MAC_RO_STAT), BITS_MACSTAT);
	local_unlock(ld);

	return old;
}

int hieth_set_mac_leadcode_cnt_limit(struct hieth_netdev_local *ld, int cnt)
{
	int old;

	local_lock(ld);
	old =
	    hieth_readl_bits(ld, UD_REG_NAME(MAC_TX_IPGCTRL),
			     BITS_PRE_CNT_LIMIT);
	hieth_writel_bits(ld, cnt, UD_REG_NAME(MAC_TX_IPGCTRL),
			  BITS_PRE_CNT_LIMIT);
	local_unlock(ld);

	return old;
}

int hieth_set_mac_trans_interval_bits(struct hieth_netdev_local *ld, int nbits)
{
	int old;
	int linkstat, negmode;

	local_lock(ld);

	negmode = _set_negmode(ld, HIETH_NEGMODE_CPUSET);
	linkstat = _set_linkstat(ld, 0);
	udelay(1000);

	old = hieth_readl_bits(ld, UD_REG_NAME(MAC_TX_IPGCTRL), BITS_IPG);
	hieth_writel_bits(ld, nbits, UD_REG_NAME(MAC_TX_IPGCTRL), BITS_IPG);
	udelay(100);

	_set_negmode(ld, negmode);
	_set_linkstat(ld, linkstat);

	local_unlock(ld);

	return old;
}

int hieth_set_mac_fc_interval(struct hieth_netdev_local *ld, int para)
{
	int old;

	local_lock(ld);
	old = hieth_readl_bits(ld, UD_REG_NAME(MAC_TX_IPGCTRL), BITS_FC_INTER);
	hieth_writel_bits(ld, para, UD_REG_NAME(MAC_TX_IPGCTRL), BITS_FC_INTER);
	local_unlock(ld);

	return old;
}

int hieth_set_negmode(struct hieth_netdev_local *ld, int mode)
{
	int old;

	local_lock(ld);
	old = _set_negmode(ld, mode);
	local_unlock(ld);

	return old;
}

int hieth_get_negmode(struct hieth_netdev_local *ld)
{
	int old;

	local_lock(ld);
	old = _get_negmode(ld);
	local_unlock(ld);

	return old;
}

int hieth_set_mii_mode(struct hieth_netdev_local *ld, int mode)
{
	int old;

	local_lock(ld);
	old = hieth_readl_bits(ld, UD_REG_NAME(MAC_PORTSEL), BITS_MII_MODE);
	hieth_writel_bits(ld, mode, UD_REG_NAME(MAC_PORTSEL), BITS_MII_MODE);
	local_unlock(ld);

	return old;
}
