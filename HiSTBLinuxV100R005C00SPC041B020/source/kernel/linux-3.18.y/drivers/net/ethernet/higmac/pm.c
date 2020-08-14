#include <linux/crc16.h>
#include "ctrl.h"

#define N			(31)
#define FILTERS			(4)
struct pm_config {
	unsigned char index;	/* bit0--eth0 bit1--eth1 */
	unsigned char uc_pkts_enable;
	unsigned char magic_pkts_enable;
	unsigned char wakeup_pkts_enable;
	struct {
		unsigned int mask_bytes:N;
		unsigned int reserved:1;	/* userspace ignore this bit */
		unsigned char offset;	/* >= 12 */
		unsigned char value[N];	/* byte string */
		unsigned char valid;	/* valid filter */
	} filter[FILTERS];
};

struct pm_reg_config {
	unsigned int pmt_ctrl;
	unsigned int pmt_mask0;
	unsigned int pmt_mask1;
	unsigned int pmt_mask2;
	unsigned int pmt_mask3;
	unsigned int pmt_cmd;
	unsigned int pmt_offset;
	unsigned int pmt_crc1_0;
	unsigned int pmt_crc3_2;
};

struct pm_reg_config pm_reg_config_backup;

#define PMT_CTRL		0xa00
#define PMT_MASK0		0xa04
#define PMT_MASK1		0xa08
#define PMT_MASK2		0xa0c
#define PMT_MASK3		0xa10
#define PMT_CMD			0xa14
#define PMT_OFFSET		0xa18
#define PMT_CRC1_0		0xa1c
#define PMT_CRC3_2		0xa20
#define MASK_INVALID_BIT	(1 << 31)

static void init_crc_table(void);
static unsigned short compute_crc(char *message, int nbytes);
static unsigned short calculate_crc16(char *buf, unsigned int mask)
{
	char data[N];
	int i, len = 0;

	memset(data, 0, sizeof(data));

	for (i = 0; i < N; i++) {
		if (mask & 0x1)
			data[len++] = buf[i];

		mask >>= 1;
	}

	return compute_crc(data, len);
}

/* use this func in config pm func */
void _pmt_reg_backup(struct higmac_netdev_local *ld)
{
	pm_reg_config_backup.pmt_ctrl = readl(ld->gmac_iobase + PMT_CTRL);
	pm_reg_config_backup.pmt_mask0 = readl(ld->gmac_iobase + PMT_MASK0);
	pm_reg_config_backup.pmt_mask1 = readl(ld->gmac_iobase + PMT_MASK1);
	pm_reg_config_backup.pmt_mask2 = readl(ld->gmac_iobase + PMT_MASK2);
	pm_reg_config_backup.pmt_mask3 = readl(ld->gmac_iobase + PMT_MASK3);
	pm_reg_config_backup.pmt_cmd = readl(ld->gmac_iobase + PMT_CMD);
	pm_reg_config_backup.pmt_offset = readl(ld->gmac_iobase + PMT_OFFSET);
	pm_reg_config_backup.pmt_crc1_0 = readl(ld->gmac_iobase + PMT_CRC1_0);
	pm_reg_config_backup.pmt_crc3_2 = readl(ld->gmac_iobase + PMT_CRC3_2);
}

#define	PM_SET			(1)
#define PM_CLEAR		(0)

int pmt_config_gmac(struct pm_config *config, struct higmac_netdev_local *ld)
{
	unsigned int v = 0, cmd = 0, offset = 0;
	unsigned short crc[FILTERS] = { 0 };
	unsigned long flags;
	int reg_mask = 0;
	int i;

	if (!ld)
		return -EINVAL;

	spin_lock_irqsave(&ld->pmtlock, flags);
	if (config->wakeup_pkts_enable) {
		/* disable wakeup_pkts_enable before reconfig? */
		v = readl(ld->gmac_iobase + PMT_CTRL);
		v &= ~(1 << 2);
		writel(v, ld->gmac_iobase + PMT_CTRL);	/* any side effect? */
	} else {
		goto config_ctrl;
	}

/* filter.valid		mask.valid	mask_bytes	effect
 *	0		*		*		no use the filter
 *	1		0		*	all pkts can wake-up(non-exist)
 *	1		1		0		all pkts can wake-up
 *	1		1		!0		normal filter
 */
	/* setup filter */
	for (i = 0; i < FILTERS; i++) {
		if (config->filter[i].valid) {
			if (config->filter[i].offset < 12)
				continue;
			/* offset and valid bit */
			offset |= config->filter[i].offset << (i * 8);
			cmd |= 1 << (i * 8);	/* valid bit */
			/* mask */
			reg_mask = PMT_MASK0 + (i * 4);

			/* for logic, mask valid bit(bit31) must set to 0,
			 * 0 is enable
			 */
			v = config->filter[i].mask_bytes;
			v &= ~(1 << 31);
			writel(v, ld->gmac_iobase + reg_mask);

			/* crc */
			crc[i] = calculate_crc16(config->filter[i].value, v);
			if (i <= 1) {	/* for filter0 and filter 1 */
				v = readl(ld->gmac_iobase + PMT_CRC1_0);
				v &= ~(0xFFFF << (16 * i));
				v |= crc[i] << (16 * i);
				writel(v, ld->gmac_iobase + PMT_CRC1_0);
			} else {	/* filter2 and filter3 */
				v = readl(ld->gmac_iobase + PMT_CRC3_2);
				v &= ~(0xFFFF << (16 * (i - 2)));
				v |= crc[i] << (16 * (i - 2));
				writel(v, ld->gmac_iobase + PMT_CRC3_2);
			}
		}
	}

	if (cmd) {
		writel(offset, ld->gmac_iobase + PMT_OFFSET);
		writel(cmd, ld->gmac_iobase + PMT_CMD);
	}

config_ctrl:
	v = 0;
	if (config->uc_pkts_enable)
		v |= 1 << 9;	/* uc pkts wakeup */
	if (config->wakeup_pkts_enable)
		v |= 1 << 2;	/* use filter framework */
	if (config->magic_pkts_enable)
		v |= 1 << 1;	/* magic pkts wakeup */

	v |= 3 << 5;		/* clear irq status */
	writel(v, ld->gmac_iobase + PMT_CTRL);

	_pmt_reg_backup(ld);

	spin_unlock_irqrestore(&ld->pmtlock, flags);

	return 0;
}

/* pmt_config will overwrite pre-config */
int pmt_config(struct net_device *ndev, struct pm_config *config)
{
	static int init;
	int ret = -EINVAL;
	struct higmac_netdev_local *priv = netdev_priv(ndev);

	if (!init)
		init_crc_table();

	ret = pmt_config_gmac(config, priv);
	if (ret)
		return ret;

	priv->pm_state = PM_SET;
	priv->wol_enable = true;
	device_set_wakeup_enable(priv->dev, 1);

	return ret;
}

inline bool pmt_enter(struct higmac_netdev_local *ld)
{
	int v, pm = false;
	unsigned long flags;

	spin_lock_irqsave(&ld->pmtlock, flags);
	if (ld->pm_state == PM_SET) {
		v = readl(ld->gmac_iobase + PMT_CTRL);
		v |= 1 << 0;	/* enter power down */
		v |= 1 << 3;	/* enable wakeup irq */
		v |= 3 << 5;	/* clear irq status */
		writel(v, ld->gmac_iobase + PMT_CTRL);

		ld->pm_state = PM_CLEAR;
		pm = true;
	}
	spin_unlock_irqrestore(&ld->pmtlock, flags);
	return pm;
}

inline void pmt_exit(struct higmac_netdev_local *ld)
{
	int v;
	unsigned long flags;

	/* logic auto exit power down mode */
	spin_lock_irqsave(&ld->pmtlock, flags);

	v = readl(ld->gmac_iobase + PMT_CTRL);
	v &= ~(1 << 0);		/* enter power down */
	v &= ~(1 << 3);		/* enable wakeup irq */

	v |= 3 << 5;		/* clear irq status */
	writel(v, ld->gmac_iobase + PMT_CTRL);

	spin_unlock_irqrestore(&ld->pmtlock, flags);

	ld->wol_enable = false;
	/* device_set_wakeup_enable(ld->dev, 0); */
}

void pmt_reg_restore(struct higmac_netdev_local *ld)
{
	unsigned int v;
	unsigned long flags;

	spin_lock_irqsave(&ld->pmtlock, flags);
	v = pm_reg_config_backup.pmt_mask0;
	writel(v, ld->gmac_iobase + PMT_MASK0);

	v = pm_reg_config_backup.pmt_mask1;
	writel(v, ld->gmac_iobase + PMT_MASK1);

	v = pm_reg_config_backup.pmt_mask2;
	writel(v, ld->gmac_iobase + PMT_MASK2);

	v = pm_reg_config_backup.pmt_mask3;
	writel(v, ld->gmac_iobase + PMT_MASK3);

	v = pm_reg_config_backup.pmt_cmd;
	writel(v, ld->gmac_iobase + PMT_CMD);

	v = pm_reg_config_backup.pmt_offset;
	writel(v, ld->gmac_iobase + PMT_OFFSET);

	v = pm_reg_config_backup.pmt_crc1_0;
	writel(v, ld->gmac_iobase + PMT_CRC1_0);

	v = pm_reg_config_backup.pmt_crc3_2;
	writel(v, ld->gmac_iobase + PMT_CRC3_2);

	v = pm_reg_config_backup.pmt_ctrl;
	writel(v, ld->gmac_iobase + PMT_CTRL);
	spin_unlock_irqrestore(&ld->pmtlock, flags);
}

/* ========the following code copy from Synopsys DWC_gmac_crc_example.c====== */
#define CRC16			/* Change it to CRC16 for CRC16 Computation */

#if defined(CRC16)
#define CRC_NAME		"CRC-16"
#define POLYNOMIAL		0x8005
#define INITIAL_REMAINDER	0xFFFF
#define FINAL_XOR_VALUE		0x0000
#define REVERSE_DATA
#undef REVERSE_REMAINDER
#endif

#define WIDTH    (8 * sizeof(unsigned short))
#define TOPBIT   (1 << (WIDTH - 1))

#ifdef REVERSE_DATA
#undef  REVERSE_DATA
#define REVERSE_DATA(X)		((unsigned char)reverse((X), 8))
#else
#undef  REVERSE_DATA
#define REVERSE_DATA(X)		(X)
#endif

#ifdef REVERSE_REMAINDER
#undef  REVERSE_REMAINDER
#define REVERSE_REMAINDER(X)	((unsigned short)reverse((X), WIDTH))
#else
#undef  REVERSE_REMAINDER
#define REVERSE_REMAINDER(X)	(X)
#endif

static unsigned short crc_table[256];

/* Reverse the data
 * Input1: Data to be reversed
 * Input2: number of bits in the data
 * Output: The reversed data
 */
static unsigned int reverse(unsigned int data, unsigned char nbits)
{
	unsigned int reversed = 0x00000000;
	unsigned char bit;

	/* Reverse the data about the center bit. */
	for (bit = 0; bit < nbits; ++bit) {
		/* If the LSB bit is set, set the reflection of it. */
		if (data & 0x01)
			reversed |= (1 << ((nbits - 1) - bit));

		data = (data >> 1);
	}
	return reversed;
}

/* This Initializes the partial CRC look up table */
static void init_crc_table(void)
{
	unsigned short remainder;
	int dividend;
	unsigned char bit;

	/* Compute the remainder of each possible dividend. */
	for (dividend = 0; dividend < 256; ++dividend) {
		/* Start with the dividend followed by zeros. */
		remainder = (unsigned short)(dividend << (WIDTH - 8));

		/* Perform modulo-2 division, a bit at a time. */
		for (bit = 8; bit > 0; --bit) {
			/* Try to divide the current data bit. */
			if (remainder & TOPBIT)
				remainder = (remainder << 1) ^ POLYNOMIAL;
			else
				remainder = (remainder << 1);
		}

		/* Store the result into the table. */
		crc_table[dividend] = remainder;
	}
}

static unsigned short compute_crc(char *message, int nbytes)
{
	unsigned short remainder = INITIAL_REMAINDER;
	int byte;
	unsigned char data;

	/* Divide the message by the polynomial, a byte at a time. */
	for (byte = 0; byte < nbytes; ++byte) {
		data = REVERSE_DATA(message[byte]) ^ (remainder >> (WIDTH - 8));
		remainder = crc_table[data] ^ (remainder << 8);
	}

	/* The final remainder is the CRC. */
	return (REVERSE_REMAINDER(remainder) ^ FINAL_XOR_VALUE);
}
