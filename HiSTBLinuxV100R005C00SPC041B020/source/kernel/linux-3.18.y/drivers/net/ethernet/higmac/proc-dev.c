#include "sockioctl.h"

/* debug code */
static int set_suspend(int eth_n)
{
	return 0;
}

/* debug code */
static int set_resume(int eth_n)
{
	/* higmac_dev_driver.resume(&higmac_platform_device); */
	return 0;
}

static int hw_states_read(struct seq_file *m, void *v)
{
	struct higmac_netdev_local *ld = NULL;

#define sprintf_io(name, base)					\
	seq_printf(m, name, readl(io_base + base))

#define sprintf_pkts(name, rx_base, tx_base)			\
	seq_printf(m, "rx_" name "%u\t\t\ttx_" name "%u\n",	\
				readl(io_base + rx_base),	\
				readl(io_base + tx_base))

	{
/* need to initialize ld !!!!!!!!! */
		void __iomem *io_base = ld->gmac_iobase;

		if (!ld->phy)
			return 0;
		if (!netif_running(ld->netdev))
			return 0;

		seq_puts(m, "----------------gmac----------------\n");
		sprintf_pkts("ok_bytes:", 0x80, 0x100);
		sprintf_pkts("bad_bytes:", 0x84, 0x104);
		sprintf_pkts("uc_pkts:", 0x88, 0x108);
		sprintf_pkts("mc_pkts:", 0x8c, 0x10c);
		sprintf_pkts("bc_pkts:", 0x90, 0x110);
		sprintf_pkts("pkts_64B:", 0x94, 0x114);
		sprintf_pkts("pkts_65_127B:", 0x98, 0x118);
		sprintf_pkts("pkts_128_255B:", 0x9c, 0x11c);
		sprintf_pkts("pkts_256_511B:", 0xa0, 0x120);
		sprintf_pkts("pkts_512_1023B:", 0xa4, 0x124);
		sprintf_pkts("pkts_1024_1518B:", 0xa8, 0x128);
		sprintf_pkts("pkts_1519_MAX:", 0xac, 0x12c);
		sprintf_io("rx_fcs_errors:%u\n", 0xb0);
		/* add more here */
	}
#undef sprintf_io
#undef sprintf_pkts

	return 0;
}

static struct proc_dir_entry *higmac_proc_root;

#define proc_open(name)	\
static int proc_open_##name(struct inode *inode, struct file *file) \
{ \
	return single_open(file, name, PDE_DATA(inode)); \
} \

proc_open(hw_states_read);

static struct proc_file {
	char *name;
	const struct file_operations ops;

} proc_file[] = {
	{
		.name = "hw_stats",
		.ops = {
			.open           = proc_open_hw_states_read,
			.read           = seq_read,
			.llseek         = seq_lseek,
			.release        = single_release,
		},
	}
};

/* /proc/higmac/
 *	|---hw_stats
 *	|---skb_pools
 */
void higmac_proc_create(void)
{
	struct proc_dir_entry *entry;
	int i;

	higmac_proc_root = proc_mkdir("higmac", NULL);
	if (!higmac_proc_root)
		return;

	for (i = 0; i < ARRAY_SIZE(proc_file); i++) {
		entry = proc_create(proc_file[i].name, 0, higmac_proc_root,
				    &proc_file[i].ops);
	}
}

void higmac_proc_destroy(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(proc_file); i++)
		remove_proc_entry(proc_file[i].name, higmac_proc_root);

	remove_proc_entry("higmac", NULL);
}

int higmac_ioctl(struct net_device *ndev, struct ifreq *rq, int cmd)
{
	struct higmac_netdev_local *priv = netdev_priv(ndev);
	struct pm_config pm_config;
	int val = 0;

	switch (cmd) {
	case SIOCSETPM:
		if (copy_from_user(&pm_config, rq->ifr_data, sizeof(pm_config)))
			return -EFAULT;
		return pmt_config(ndev, &pm_config);

	case SIOCSETSUSPEND:
		if (copy_from_user(&val, rq->ifr_data, sizeof(val)))
			return -EFAULT;
		return set_suspend(val);

	case SIOCSETRESUME:
		if (copy_from_user(&val, rq->ifr_data, sizeof(val)))
			return -EFAULT;
		return set_resume(val);

	default:
		if (!netif_running(ndev))
			return -EINVAL;

		if (!priv->phy)
			return -EINVAL;

		return phy_mii_ioctl(priv->phy, rq, cmd);
	}
	return 0;
}
