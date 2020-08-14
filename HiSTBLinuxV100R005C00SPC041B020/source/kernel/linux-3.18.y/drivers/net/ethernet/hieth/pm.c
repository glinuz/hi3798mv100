#include <linux/crc16.h>

#define HIETH_PM_N              (31)
#define HIETH_PM_FILTERS        (4)

struct hieth_pm_config {
	unsigned char index;		/* bit0--eth0 bit1--eth1 */
	unsigned char uc_pkts_enable;
	unsigned char magic_pkts_enable;
	unsigned char wakeup_pkts_enable;
	struct {
		unsigned int	mask_bytes : HIETH_PM_N;
		unsigned int	reserved   : 1;/* userspace ignore this bit */
		unsigned char	offset;	/* >= 12 */
		unsigned char	value[HIETH_PM_N];/* byte string */
		unsigned char	valid;	/* valid filter */
	} filter[HIETH_PM_FILTERS];
};

#define HIETH_PMT_CTRL		0x0500
#define HIETH_PMT_MASK0		0x0504
#define HIETH_PMT_MASK1		0x0508
#define HIETH_PMT_MASK2		0x050c
#define HIETH_PMT_MASK3		0x0510
#define HIETH_PMT_CMD		0x0514
#define HIETH_PMT_OFFSET	0x0518
#define HIETH_PMT_CRC1_0	0x051c
#define HIETH_PMT_CRC3_2	0x0520

static void hieth_initCrcTable(void);
static unsigned short hieth_computeCrc(char* message, int nBytes);
static unsigned short calculate_crc16(char *buf, unsigned int mask)
{
	char data[HIETH_PM_N];
	int i, len = 0;

	memset(data, 0, sizeof(data));

	for (i = 0; i < HIETH_PM_N; i++) {
		if (mask & 0x1)
			data[len++] = buf[i];

		mask >>= 1;
	}

	return hieth_computeCrc(data, len);
}


#define	HIETH_PM_SET			(1)
#define HIETH_PM_CLEAR		(0)
static char pm_state[HIETH_MAX_PORT] = {HIETH_PM_CLEAR, HIETH_PM_CLEAR};

int hieth_pmt_config_eth(struct hieth_pm_config *config, struct hieth_netdev_priv *priv)
{
	unsigned int v = 0, cmd = 0, offset = 0;
	unsigned short crc[HIETH_PM_FILTERS] = {0};
	int reg_mask = 0;
	int i;

	if (!priv)
		return -EINVAL;

	local_lock(priv);
	if (config->wakeup_pkts_enable) {
		/* disable wakeup_pkts_enable before reconfig? */
		v = hieth_readl(priv->port_base, HIETH_PMT_CTRL);
		v &= ~(1 << 2);
		 hieth_writel(priv->port_base, v, HIETH_PMT_CTRL);/* any side effect? */
	} else
		goto config_ctrl;

/*
 * filter.valid		mask.valid	mask_bytes	effect
 *	0		*		*		no use the filter
 *	1		0		*		all pkts can wake-up(non-exist)
 *	1		1		0		all pkts can wake-up
 *	1		1		!0		normal filter
 */
	/* setup filter */
	for (i = 0; i < HIETH_PM_FILTERS; i++) {
		if (config->filter[i].valid) {
			if (config->filter[i].offset < 12)
				continue;
			/* offset and valid bit */
			offset |= config->filter[i].offset << (i * 8);
			cmd    |= 1 << (i * 8); /* valid bit */
			/* mask */
			reg_mask = HIETH_PMT_MASK0 + (i * 4);

			/*
			 * for logic, mask valid bit(bit31) must set to 0,
			 * 0 is enable
			 */
			v = config->filter[i].mask_bytes;
			v &= ~(1 << 31);
			hieth_writel(priv->port_base, v, reg_mask);

			/* crc */
			crc[i] = calculate_crc16(config->filter[i].value, v);
			if (i <= 1) {/* for filter0 and filter 1 */
				v = hieth_readl(priv->port_base, HIETH_PMT_CRC1_0);
				v &= ~(0xFFFF << (16 * i));
				v |= crc[i] << (16 * i);
				hieth_writel(priv->port_base, v, HIETH_PMT_CRC1_0);
			} else {/* filter2 and filter3 */
				v = hieth_readl(priv->port_base, HIETH_PMT_CRC3_2);
				v &= ~(0xFFFF << (16 * (i - 2)));
				v |= crc[i] << (16 * (i - 2));
				hieth_writel(priv->port_base, v, HIETH_PMT_CRC3_2);
			}
		}
	}

	if (cmd) {
		hieth_writel(priv->port_base, offset, HIETH_PMT_OFFSET);
		hieth_writel(priv->port_base, cmd, HIETH_PMT_CMD);
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
	hieth_writel(priv->port_base, v, HIETH_PMT_CTRL);

	local_unlock(priv);

	return 0;
}

/* pmt_config will overwrite pre-config */
int hieth_pmt_config(struct hieth_pm_config *config)
{
	static int init;
	int map = config->index, i, ret = -EINVAL;
	struct hieth_netdev_priv *priv;

	if (!init)
		hieth_initCrcTable();

	for (i = 0; i < HIETH_MAX_PORT; i++) {
		if (!hieth_devs_save[i])
			continue;

		priv = netdev_priv(hieth_devs_save[i]);

		if (map & 0x1) {
			ret = hieth_pmt_config_eth(config, priv);
			if (ret)
				return ret;
			else {
				pm_state[i] = HIETH_PM_SET;
				device_set_wakeup_enable(priv->dev, 1);
			}
		}
		map >>= 1;
	}

	return ret;
}

bool inline hieth_pmt_enter(void)
{
	int i, v, pm = false;
	struct hieth_netdev_priv *priv;

	for (i = 0; i < HIETH_MAX_PORT; i++) {
		if (!hieth_devs_save[i])
			continue;

		priv = netdev_priv(hieth_devs_save[i]);

		local_lock(priv);
		if (pm_state[i] == HIETH_PM_SET) {

			v = hieth_readl(priv->port_base, HIETH_PMT_CTRL);
			v |= 1 << 0;	/* enter power down */
			v |= 1 << 3;	/* enable wakeup irq */
			v |= 3 << 5;	/* clear irq status */
			hieth_writel(priv->port_base, v, HIETH_PMT_CTRL);

			pm_state[i] = HIETH_PM_CLEAR;
			pm = true;
		}
		local_unlock(priv);
	}
	return pm;
}

void inline hieth_pmt_exit(void)
{
	int i, v;
	struct hieth_netdev_priv *priv;

	for (i = 0; i < HIETH_MAX_PORT; i++) {
		if (!hieth_devs_save[i])
			continue;

		priv = netdev_priv(hieth_devs_save[i]);

		/* logic auto exit power down mode */
		local_lock(priv);

		v = hieth_readl(priv->port_base, HIETH_PMT_CTRL);
		v &= ~(1 << 0);	/* enter power down */
		v &= ~(1 << 3);	/* enable wakeup irq */

		v |= 3 << 5;	/* clear irq status */
		hieth_writel(priv->port_base, v, HIETH_PMT_CTRL);

		local_unlock(priv);
	}

	//device_set_wakeup_enable(priv->dev, 0);
}

/* ========the following code copy from Synopsys DWC_gmac_crc_example.c====== */
#define CRC16			/* Change it to CRC16 for CRC16 Computation*/

#define FALSE	0
#define TRUE	!FALSE

#if defined(CRC16)
typedef unsigned short  crc;
#define CRC_NAME		"CRC-16"
#define POLYNOMIAL		0x8005
#define INITIAL_REMAINDER	0xFFFF
#define FINAL_XOR_VALUE		0x0000
#define REVERSE_DATA		TRUE
#define REVERSE_REMAINDER	FALSE
#endif

#define WIDTH    (8 * sizeof(crc))
#define TOPBIT   (1 << (WIDTH - 1))

#if (REVERSE_DATA == TRUE)
#undef  REVERSE_DATA
#define REVERSE_DATA(X)		((unsigned char) reverse((X), 8))
#else
#undef  REVERSE_DATA
#define REVERSE_DATA(X)		(X)
#endif

#if (REVERSE_REMAINDER == TRUE)
#undef  REVERSE_REMAINDER
#define REVERSE_REMAINDER(X)	((crc) reverse((X), WIDTH))
#else
#undef  REVERSE_REMAINDER
#define REVERSE_REMAINDER(X)	(X)
#endif

static crc crctable[256];

/* Reverse the data
 *
 * Input1: Data to be reversed
 * Input2: number of bits in the data
 * Output: The reversed data
 */
static unsigned long reverse(unsigned long data, unsigned char nbits)
{
	unsigned long  reversed = 0x00000000;
	unsigned char  bit;

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
static void hieth_initCrcTable(void)
{
	crc remainder;
	int dividend;
	unsigned char  bit;

	/* Compute the remainder of each possible dividend. */
	for (dividend = 0; dividend < 256; ++dividend) {
		/* Start with the dividend followed by zeros. */
		remainder = (crc)(dividend << (WIDTH - 8));

		/* Perform modulo-2 division, a bit at a time. */
		for (bit = 8; bit > 0; --bit) {
			/* Try to divide the current data bit. */
			if (remainder & TOPBIT)
				remainder = (remainder << 1) ^ POLYNOMIAL;
			else
				remainder = (remainder << 1);
		}

		/* Store the result into the table. */
		crctable[dividend] = remainder;
	}
}

static unsigned short hieth_computeCrc(char *message, int nBytes)
{
	crc	remainder = INITIAL_REMAINDER;
	int	byte;
	unsigned char  data;

	/* Divide the message by the polynomial, a byte at a time. */
	for (byte = 0; byte < nBytes; ++byte) {
		data = REVERSE_DATA(message[byte]) ^ (remainder >> (WIDTH - 8));
		remainder = crctable[data] ^ (remainder << 8);
	}

	/* The final remainder is the CRC. */
	return (REVERSE_REMAINDER(remainder) ^ FINAL_XOR_VALUE);
}
