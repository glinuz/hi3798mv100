#ifndef __HIETH_H
#define __HIETH_H

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/list.h>
#include <linux/phy.h>
#include <linux/io.h>

#ifndef CONFIG_ARM64
#include <mach/platform.h>
#include <mach/irqs.h>
#endif

#define HIETH_MIIBUS_NAME	"himii"
#define HIETH_DRIVER_NAME	"hieth"

/* hieth max port */
#define HIETH_MAX_PORT	2

/* invalid phy addr */
#define HIETH_INVALID_PHY_ADDR  31

/* hieth monitor timer, 10ms */
#define HIETH_MONITOR_TIMER	10

/* hieth hardware queue send fifo depth, increase to optimize TX performance. */
#define HIETH_HWQ_XMIT_DEPTH	12

/* set irq affinity to cpu1 when multi-processor */
#define HIETH_IRQ_AFFINITY_CPU	1

#define HIETH_MAX_QUEUE_DEPTH	64
#define HIETH_MAX_RX_HEAD_LEN	(10000)  /* max skbs for rx */
#define HIETH_MAX_RCV_LEN	1535     /* max receive length */

#define HIETH_NAPI_WEIGHT 32
/*  mmu should be less than 1600 Bytes
 */

#define HIETH_MAX_FRAME_SIZE	(1600)
#define HIETH_INVALID_RXPKG_LEN(len) (!((len) >= 42 && \
				      (len) <= HIETH_MAX_FRAME_SIZE))

#define HIETH_MAX_MAC_FILTER_NUM	8
#define HIETH_MAX_UNICAST_ADDRESSES	2
#define HIETH_MAX_MULTICAST_ADDRESSES	(HIETH_MAX_MAC_FILTER_NUM - \
		HIETH_MAX_UNICAST_ADDRESSES)

/* Register Definition
 */
/*------------------------- port register -----------------------------------*/
/* Mac port sel */
#define HIETH_P_MAC_PORTSEL             0x0200
#define  HIETH_P_MAC_PORTSEL_STAT       0
#define   HIETH_P_MAC_PORTSEL_STAT_MDIO 0
#define   HIETH_P_MAC_PORTSEL_STAT_CPU  1
#define  HIETH_P_MAC_PORTSEL_MII_MODE   1
#define   HIETH_P_MAC_PORTSEL_MII       ~BIT(1)
#define   HIETH_P_MAC_PORTSEL_RMII      BIT(1)
/* Mac ro status */
#define HIETH_P_MAC_RO_STAT             0x0204
/* Mac port status set */
#define HIETH_P_MAC_PORTSET             0x0208
#define  HIETH_P_MAC_PORTSET_SPD_100M   BIT(2)
#define  HIETH_P_MAC_PORTSET_LINKED     BIT(1)
#define  HIETH_P_MAC_PORTSET_DUP_FULL   BIT(0)
/* Mac status change */
#define HIETH_P_MAC_STAT_CHANGE         0x020C
/* Mac set */
#define HIETH_P_MAC_SET                 0x0210
#define  HIETH_P_MAC_SET_LEN_MAX(n)     ((n) & 0x7FF)
#define  HIETH_P_MAC_SET_LEN_MAX_MSK    GENMASK(10, 0)

#define HIETH_P_MAC_RX_IPGCTRL          0x0214
#define HIETH_P_MAC_TX_IPGCTRL          0x0218
#define  HIETH_P_MAC_TX_IPGCTRL_PRE_CNT_LMT_SHIFT 23
#define  HIETH_P_MAC_TX_IPGCTRL_PRE_CNT_LMT_MSK   GENMASK(25, 23)
/* queue length set */
#define HIETH_P_GLB_QLEN_SET            0x0344
#define  HIETH_P_GLB_QLEN_SET_TXQ_DEP_MSK GENMASK(5, 0)
#define  HIETH_P_GLB_QLEN_SET_TXQ_DEP(n)  ((n) << 0)
#define  HIETH_P_GLB_QLEN_SET_RXQ_DEP_MSK GENMASK(13, 8)
#define  HIETH_P_GLB_QLEN_SET_RXQ_DEP(n)  ((n) << 8)
/* Rx frame start addr */
#define HIETH_P_GLB_RXFRM_SADDR         0x0350
/* Rx (read only) Queue-ID and LEN */
#define HIETH_P_GLB_RO_IQFRM_DES        0x0354
#define  HIETH_P_GLB_RO_IQFRM_DES_FDIN_LEN_MSK GENMASK(11, 0)
/* Rx ADDR */
#define HIETH_P_GLB_IQ_ADDR             0x0358
/* Tx ADDR and LEN */
#define HIETH_P_GLB_EQ_ADDR             0x0360
#define HIETH_P_GLB_EQFRM_LEN           0x0364
#define  HIETH_P_GLB_EQFRM_TXINQ_LEN_MSK GENMASK(10, 0)
/* Rx/Tx Queue ID */
#define HIETH_P_GLB_RO_QUEUE_ID         0x0368
/* Rx/Tx Queue staus  */
#define HIETH_P_GLB_RO_QUEUE_STAT       0x036C
/* check this bit to see if we can add a Tx package */
#define  HIETH_P_GLB_RO_QUEUE_STAT_XMITQ_RDY_MSK BIT(24)
/* check this bit to see if we can add a Rx addr */
#define  HIETH_P_GLB_RO_QUEUE_STAT_RECVQ_RDY_MSK BIT(25)
/* counts in queue, include currently sending */
#define  HIETH_P_GLB_RO_QUEUE_STAT_XMITQ_CNT_INUSE_MSK GENMASK(5, 0)

/*------------------------- global register --------------------------------*/
/* host mac address  */
#define HIETH_GLB_HOSTMAC_L32           0x1300
#define HIETH_GLB_HOSTMAC_H16           0x1304
/* soft reset */
#define HIETH_GLB_SOFT_RESET            0x1308
#define  HIETH_GLB_SOFT_RESET_ALL       BIT(0)
#define  HIETH_GLB_SOFT_RESET_P0        BIT(2)
#define  HIETH_GLB_SOFT_RESET_P1        BIT(3)
/* forward contrl */
#define HIETH_GLB_FWCTRL                0x1310
#define  HIETH_GLB_FWCTRL_VLAN_ENABLE   BIT(0)
#define  HIETH_GLB_FWCTRL_FW2CPU_ENA_U  BIT(5)
#define  HIETH_GLB_FWCTRL_FW2CPU_ENA_D  BIT(9)
#define  HIETH_GLB_FWCTRL_FWALL2CPU_U   BIT(7)
#define  HIETH_GLB_FWCTRL_FWALL2CPU_D   BIT(11)
#define  HIETH_GLB_FWCTRL_FW2OTHPORT_ENA_U   BIT(4)
#define  HIETH_GLB_FWCTRL_FW2OTHPORT_ENA_D   BIT(8)
#define  HIETH_GLB_FWCTRL_FW2OTHPORT_FORCE_U BIT(6)
#define  HIETH_GLB_FWCTRL_FW2OTHPORT_FORCE_D BIT(10)
/* Mac filter table control */
#define HIETH_GLB_MACTCTRL              0x1314
#define  HIETH_GLB_MACTCTRL_MACT_ENA_U  BIT(7)
#define  HIETH_GLB_MACTCTRL_MACT_ENA_D  BIT(15)
#define  HIETH_GLB_MACTCTRL_BROAD2CPU_U BIT(5)
#define  HIETH_GLB_MACTCTRL_BROAD2CPU_D BIT(13)
#define  HIETH_GLB_MACTCTRL_BROAD2OTHPORT_U  BIT(4)
#define  HIETH_GLB_MACTCTRL_BROAD2OTHPORT_D  BIT(12)
#define  HIETH_GLB_MACTCTRL_MULTI2CPU_U      BIT(3)
#define  HIETH_GLB_MACTCTRL_MULTI2CPU_D      BIT(11)
#define  HIETH_GLB_MACTCTRL_MULTI2OTHPORT_U  BIT(2)
#define  HIETH_GLB_MACTCTRL_MULTI2OTHPORT_D  BIT(10)
#define  HIETH_GLB_MACTCTRL_UNI2CPU_U        BIT(1)
#define  HIETH_GLB_MACTCTRL_UNI2CPU_D        BIT(9)
#define  HIETH_GLB_MACTCTRL_UNI2OTHPORT_U    BIT(0)
#define  HIETH_GLB_MACTCTRL_UNI2OTHPORT_D    BIT(8)
/* Host mac address */
#define HIETH_GLB_DN_HOSTMAC_L32        0x1340
#define HIETH_GLB_DN_HOSTMAC_H16        0x1344
#define HIETH_GLB_DN_HOSTMAC_ENA        0x1348
#define  HIETH_GLB_DN_HOSTMAC_ENA_BIT   BIT(0)
/* Mac filter */
#define HIETH_GLB_MAC_L32_BASE          0x1400
#define HIETH_GLB_MAC_H16_BASE          0x1404
#define HIETH_GLB_MAC_L32_BASE_D        (0x1400 + 16 * 0x8)
#define HIETH_GLB_MAC_H16_BASE_D        (0x1404 + 16 * 0x8)
#define  HIETH_GLB_MACFLT_HI16          GENMASK(15, 0)
#define  HIETH_GLB_MACFLT_FW2CPU_U      BIT(21)
#define  HIETH_GLB_MACFLT_FW2CPU_D      BIT(19)
#define  HIETH_GLB_MACFLT_FW2PORT_U     BIT(20)
#define  HIETH_GLB_MACFLT_FW2PORT_D     BIT(18)
#define  HIETH_GLB_MACFLT_ENA_U         BIT(17)
#define  HIETH_GLB_MACFLT_ENA_D         BIT(16)
/* ENDIAN */
#define HIETH_GLB_ENDIAN_MOD            0x1318
#define  HIETH_GLB_ENDIAN_MOD_IN        BIT(1)
#define  HIETH_GLB_ENDIAN_MOD_OUT       BIT(0)
/* IRQs */
#define HIETH_GLB_IRQ_STAT              0x1330
#define HIETH_GLB_IRQ_ENA               0x1334
#define  HIETH_GLB_IRQ_ENA_IEN_A        BIT(19)
#define  HIETH_GLB_IRQ_ENA_IEN_U        BIT(18)
#define  HIETH_GLB_IRQ_ENA_IEN_D        BIT(17)
#define  HIETH_GLB_IRQ_ENA_BIT_U        GENMASK(7, 0)
#define  HIETH_GLB_IRQ_ENA_BIT_D        GENMASK(27, 20)
#define HIETH_GLB_IRQ_RAW               0x1338
#define  HIETH_GLB_IRQ_INT_MULTI_RXRDY_U BIT(7)
#define  HIETH_GLB_IRQ_INT_MULTI_RXRDY_D BIT(27)
#define  HIETH_GLB_IRQ_INT_TXQUE_RDY_U  BIT(6)
#define  HIETH_GLB_IRQ_INT_TXQUE_RDY_D  BIT(26)
#define  HIETH_GLB_IRQ_INT_RX_RDY_U     BIT(0)
#define  HIETH_GLB_IRQ_INT_RX_RDY_D     BIT(20)

/* ***********************************************************
*
* Only for internal used!
*
* ***********************************************************
*/

/* read/write IO */
#define hieth_readl(base, ofs) \
	readl(base + (ofs))
#define hieth_writel(base, v, ofs) \
	writel(v, base + (ofs))

#define hieth_trace_level 8
#define hireg_trace(level, msg...) do { \
if ((level) >= hieth_trace_level) { \
	pr_info("hireg_trace:%s:%d: ", __func__, __LINE__); \
	pr_info(msg); \
	pr_info("\n"); \
} \
} while (0)

#define hireg_readl(base, ofs) ({ unsigned long reg = readl((base) + (ofs)); \
	hireg_trace(2, "_readl(0x%04X) = 0x%08lX", (ofs), reg); \
	reg; })

#define hireg_writel(base, v, ofs) do { writel((v), (base) + (ofs)); \
	hireg_trace(2, "_writel(0x%04X) = 0x%08lX", \
		    (ofs), (unsigned long)(v)); \
} while (0)

#define hieth_dump_buf(buf, len) do {\
	int i;\
	char *p = (void *)(buf);\
	pr_info("%s->%d, buf=0x%.8x, len=%d\n", \
			__func__, __LINE__, \
			(int)(buf), (int)(len)); \
	for (i = 0; i < (len); i++) {\
		pr_info("0x%.2x ", *(p+i));\
		if (!((i+1) & 0x07)) \
			pr_info("\n");\
	} \
	pr_info("\n");\
} while (0)

/* port */
enum hieth_port_e {
	HIETH_PORT_0 = 0,
	HIETH_PORT_1,
	HIETH_PORT_NUM,
};

struct hieth_netdev_priv {
	void __iomem *glb_base;     /* virtual io global addr */
	void __iomem *port_base;    /* virtual to port addr:
				     * port0-0; port1-0x2000
				     */
	int port;                   /* 0 => up port, 1 => down port */
	int irq;

	struct device *dev;
	struct net_device_stats stats;
	struct phy_device *phy;
	struct device_node *phy_node;
	phy_interface_t	phy_mode;

	struct mii_bus *mii_bus;

	struct sk_buff_head rx_head;   /*received pkgs */
	struct sk_buff_head rx_hw;     /*rx pkgs in hw */
	struct sk_buff_head tx_hw;     /*tx pkgs in hw */
	u32 tx_hw_cnt;

	struct timer_list monitor;

	struct {
		int hw_xmitq;
	} depth;

	struct {
		unsigned long rx_pool_dry_times;
	} stat;

#define SKB_SIZE		(HIETH_MAX_FRAME_SIZE)
	struct rx_skb_pool {
		struct sk_buff *sk_pool[CONFIG_HIETH_MAX_RX_POOLS];/*skb pool*/
		int next_free_skb;	/*next free skb*/
	} rx_pool;

	struct napi_struct napi;

	int link_stat;
	int (*eee_init)(struct phy_device *phy_dev);

	spinlock_t lock; /* lock for reg rw */
	unsigned long lockflags;

	spinlock_t mdio_lock; /* lock for mdio reg */
	unsigned long mdio_lockflags;

	struct clk *clk;
};

/* phy parameter */
struct hieth_phy_param_s {
	bool isvalid;     /* valid or not */
	bool isinternal;  /* internal phy or external phy */
	int phy_addr;
	u32 trim_params;
	phy_interface_t phy_mode;
	const char *macaddr;

	/* gpio reset pin if has */
	void __iomem *gpio_base;
	u32 gpio_bit;
};

#define local_lock_init(priv)	spin_lock_init(&(priv)->lock)
#define local_lock_exit(priv)
#define local_lock(priv)	spin_lock_irqsave(&(priv)->lock, \
							(priv)->lockflags)
#define local_unlock(priv)	spin_unlock_irqrestore(&(priv)->lock, \
							(priv)->lockflags)

#define hieth_mdio_lock_init(priv) spin_lock_init(&(priv)->mdio_lock)
#define hieth_mdio_lock_exit(priv)
#define hieth_mdio_lock(priv)      spin_lock_irqsave(&(priv)->mdio_lock, \
						     (priv)->mdio_lockflags)
#define hieth_mdio_unlock(priv)    spin_unlock_irqrestore(&(priv)->mdio_lock, \
							 (priv)->mdio_lockflags)

#define UD_BIT_NAME(name)       ((priv->port == HIETH_PORT_0) ? \
				 name##_U : name##_D)

#define GLB_MAC_H16(port, reg)	((((port) == HIETH_PORT_0) ? \
				 HIETH_GLB_MAC_H16_BASE : \
				 HIETH_GLB_MAC_H16_BASE_D) + (reg * 0x8))
#define GLB_MAC_L32(port, reg)	((((port) == HIETH_PORT_0) ? \
				 HIETH_GLB_MAC_L32_BASE : \
				 HIETH_GLB_MAC_L32_BASE_D) + (reg * 0x8))

#define SIOCGETMODE	(SIOCDEVPRIVATE)	/* get work mode */
#define SIOCSETMODE	(SIOCDEVPRIVATE + 1)	/* set work mode */
#define SIOCGETFWD	(SIOCDEVPRIVATE + 2)	/* get forcing forward config */
#define SIOCSETFWD	(SIOCDEVPRIVATE + 3)	/* set forcing forward config */
#define SIOCSETPM	(SIOCDEVPRIVATE + 4)	/* set pmt wake up config */
#define SIOCSETSUSPEND	(SIOCDEVPRIVATE + 5)	/* call dev->suspend */
#define SIOCSETRESUME	(SIOCDEVPRIVATE + 6)	/* call dev->resume */

extern struct hieth_phy_param_s hieth_phy_param[];

void hieth_autoeee_init(struct hieth_netdev_priv *priv, int link_stat);
void hieth_phy_register_fixups(void);
void hieth_phy_reset(void);
void hieth_fix_festa_phy_trim(struct mii_bus *bus);
#endif

/* vim: set ts=8 sw=8 tw=78: */
