#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/etherdevice.h>
#include <linux/platform_device.h>
#include <linux/of_net.h>
#include <linux/of_mdio.h>
#include <linux/clk.h>
#include <linux/circ_buf.h>
#include <linux/hikapi.h>
#include <linux/netdevice.h>

#include "hieth.h"
#include "mdio.h"
#include "hieth_dbg.h"


/*----------------------------Global variable-------------------------------*/
struct hieth_phy_param_s hieth_phy_param[HIETH_MAX_PORT];

/*----------------------------Local variable-------------------------------*/
static struct net_device *hieth_devs_save[HIETH_MAX_PORT] = { NULL, NULL };
static struct hieth_netdev_priv hieth_priv;

/* real port count */
static int hieth_real_port_cnt;
/* default, eth enable */
static bool hieth_disable;
/* autoeee, enabled by dts */
static bool hieth_enable_autoeee;

static int __init hieth_noeth(char *str)
{
	hieth_disable = true;

	return 0;
}

early_param("noeth", hieth_noeth);

#include "pm.c"

static int hieth_hw_set_macaddress(struct hieth_netdev_priv *priv,
				   unsigned char *mac)
{
	u32 reg;

	if (priv->port == HIETH_PORT_1) {
		reg = hieth_readl(priv->glb_base, HIETH_GLB_DN_HOSTMAC_ENA);
		reg |= HIETH_GLB_DN_HOSTMAC_ENA_BIT;
		hieth_writel(priv->glb_base, reg, HIETH_GLB_DN_HOSTMAC_ENA);
	}

	reg = mac[1] | (mac[0] << 8);
	if (priv->port == HIETH_PORT_0)
		hieth_writel(priv->glb_base, reg, HIETH_GLB_HOSTMAC_H16);
	else
		hieth_writel(priv->glb_base, reg, HIETH_GLB_DN_HOSTMAC_H16);

	reg = mac[5] | (mac[4] << 8) | (mac[3] << 16) | (mac[2] << 24);
	if (priv->port == HIETH_PORT_0)
		hieth_writel(priv->glb_base, reg, HIETH_GLB_HOSTMAC_L32);
	else
		hieth_writel(priv->glb_base, reg, HIETH_GLB_DN_HOSTMAC_L32);

	return 0;
}

static void hieth_irq_enable(struct hieth_netdev_priv *priv, int irqs)
{
	u32 val;

	local_lock(priv);
	val = hieth_readl(priv->glb_base, HIETH_GLB_IRQ_ENA);
	hieth_writel(priv->glb_base, val | irqs, HIETH_GLB_IRQ_ENA);
	local_unlock(priv);
}

static void hieth_irq_disable(struct hieth_netdev_priv *priv, int irqs)
{
	u32 val;

	local_lock(priv);
	val = hieth_readl(priv->glb_base, HIETH_GLB_IRQ_ENA);
	hieth_writel(priv->glb_base, val & (~irqs), HIETH_GLB_IRQ_ENA);
	local_unlock(priv);
}

static void hieth_clear_irqstatus(struct hieth_netdev_priv *priv, int irqs)
{
	local_lock(priv);
	hieth_writel(priv->glb_base, irqs, HIETH_GLB_IRQ_RAW);
	local_unlock(priv);
}

static int hieth_port_reset(struct hieth_netdev_priv *priv)
{
	u32 rst_bit = 0;
	u32 val;

	if (hieth_real_port_cnt == 1)
		rst_bit = HIETH_GLB_SOFT_RESET_ALL;
	else {
		if (priv->port == HIETH_PORT_0) {
			rst_bit |= HIETH_GLB_SOFT_RESET_P0;
		} else if (priv->port == HIETH_PORT_1) {
			rst_bit |= HIETH_GLB_SOFT_RESET_P1;
		} else {
			BUG();
		}
	}

	val = hieth_readl(priv->glb_base, HIETH_GLB_SOFT_RESET);

	val |= rst_bit;
	hieth_writel(priv->glb_base, val, HIETH_GLB_SOFT_RESET);
	usleep_range(1000, 10000);
	val &= ~rst_bit;
	hieth_writel(priv->glb_base, val, HIETH_GLB_SOFT_RESET);
	usleep_range(1000, 10000);
	val |= rst_bit;
	hieth_writel(priv->glb_base, val, HIETH_GLB_SOFT_RESET);
	usleep_range(1000, 10000);
	val &= ~rst_bit;
	hieth_writel(priv->glb_base, val, HIETH_GLB_SOFT_RESET);

	return 0;
}

static void hieth_port_init(struct hieth_netdev_priv *priv)
{
	u32 val;
	int phy_intf = (priv->phy_mode == PHY_INTERFACE_MODE_MII ?
			HIETH_P_MAC_PORTSEL_MII : HIETH_P_MAC_PORTSEL_RMII);

	/* set little endian */
	val = hieth_readl(priv->glb_base, HIETH_GLB_ENDIAN_MOD);
	val |= HIETH_GLB_ENDIAN_MOD_IN;
	val |= HIETH_GLB_ENDIAN_MOD_OUT;
	hieth_writel(priv->glb_base, val, HIETH_GLB_ENDIAN_MOD);

	/* set stat ctrl to cpuset, and MII or RMII mode */
	hieth_writel(priv->port_base, phy_intf | HIETH_P_MAC_PORTSEL_STAT_CPU,
		     HIETH_P_MAC_PORTSEL);

	/*clear all interrupt status */
	hieth_clear_irqstatus(priv, UD_BIT_NAME(HIETH_GLB_IRQ_ENA_BIT));

	/*disable interrupts */
	hieth_irq_disable(priv, UD_BIT_NAME(HIETH_GLB_IRQ_ENA_BIT) |
			  UD_BIT_NAME(HIETH_GLB_IRQ_ENA_IEN));

	/* disable vlan, enable UpEther<->CPU */
	val = hieth_readl(priv->glb_base, HIETH_GLB_FWCTRL);
	val &= ~HIETH_GLB_FWCTRL_VLAN_ENABLE;
	val |= UD_BIT_NAME(HIETH_GLB_FWCTRL_FW2CPU_ENA);
	val &= ~(UD_BIT_NAME(HIETH_GLB_FWCTRL_FWALL2CPU));
	hieth_writel(priv->glb_base, val, HIETH_GLB_FWCTRL);
	val = hieth_readl(priv->glb_base, HIETH_GLB_MACTCTRL);
	val |= UD_BIT_NAME(HIETH_GLB_MACTCTRL_BROAD2CPU);
	val |= UD_BIT_NAME(HIETH_GLB_MACTCTRL_MACT_ENA);
	hieth_writel(priv->glb_base, val, HIETH_GLB_MACTCTRL);

	/* set pre count limit */
	val = hieth_readl(priv->port_base, HIETH_P_MAC_TX_IPGCTRL);
	val &= ~HIETH_P_MAC_TX_IPGCTRL_PRE_CNT_LMT_MSK;
	val |= 0;
	hieth_writel(priv->port_base, val, HIETH_P_MAC_TX_IPGCTRL);

	/* set max receive length */
	val = hieth_readl(priv->port_base, HIETH_P_MAC_SET);
	val &= ~HIETH_P_MAC_SET_LEN_MAX_MSK;
	val |= HIETH_P_MAC_SET_LEN_MAX(HIETH_MAX_RCV_LEN);
	hieth_writel(priv->port_base, val, HIETH_P_MAC_SET);
}

static void hieth_set_hwq_depth(struct hieth_netdev_priv *priv)
{
	u32 val;

	val = hieth_readl(priv->port_base, HIETH_P_GLB_QLEN_SET);
	val &= ~HIETH_P_GLB_QLEN_SET_TXQ_DEP_MSK;
	val |= HIETH_P_GLB_QLEN_SET_TXQ_DEP(priv->depth.hw_xmitq);
	val &= ~HIETH_P_GLB_QLEN_SET_RXQ_DEP_MSK;
	val |= HIETH_P_GLB_QLEN_SET_RXQ_DEP(HIETH_MAX_QUEUE_DEPTH -
					    priv->depth.hw_xmitq);
	hieth_writel(priv->port_base, val, HIETH_P_GLB_QLEN_SET);
}

static int hieth_hw_xmitq_ready(struct hieth_netdev_priv *priv)
{
	int ret;

	local_lock(priv);
	ret = hieth_readl(priv->port_base, HIETH_P_GLB_RO_QUEUE_STAT);
	ret &= HIETH_P_GLB_RO_QUEUE_STAT_XMITQ_RDY_MSK;
	local_unlock(priv);

	return ret;
}

static int hieth_xmit_release_skb(struct hieth_netdev_priv *priv)
{
	u32 val;
	int ret = 0;
	struct sk_buff *skb;

	local_lock(priv);

	val = hieth_readl(priv->port_base, HIETH_P_GLB_RO_QUEUE_STAT) &
			  HIETH_P_GLB_RO_QUEUE_STAT_XMITQ_CNT_INUSE_MSK;
	while (val < priv->tx_hw_cnt) {
		skb = skb_dequeue(&priv->tx_hw);

		if (!skb) {
			pr_err("hw_xmitq_cnt_inuse=%d, tx_hw_cnt=%d\n",
			       val, priv->tx_hw_cnt);

			BUG();
			ret = -1;
			goto error_exit;
		}
		dev_kfree_skb_any(skb);

		priv->tx_hw_cnt--;

		val = hieth_readl(priv->port_base, HIETH_P_GLB_RO_QUEUE_STAT) &
				HIETH_P_GLB_RO_QUEUE_STAT_XMITQ_CNT_INUSE_MSK;
	}

error_exit:
	local_unlock(priv);
	return ret;
}

static int hieth_xmit_real_send(struct hieth_netdev_priv *priv,
				struct sk_buff *skb)
{
	int ret = 0;
	u32 val;

	local_lock(priv);

	val = hieth_readl(priv->port_base, HIETH_P_GLB_RO_QUEUE_STAT);
	val &= HIETH_P_GLB_RO_QUEUE_STAT_XMITQ_RDY_MSK;
	if (!val) {
		pr_err("hw xmit queue is not ready\n");
		ret = -1;
		goto _trans_exit;
	}

	/* for recalc CRC, 4 bytes more is needed */
	hieth_writel(priv->port_base, virt_to_phys(skb->data),
		     HIETH_P_GLB_EQ_ADDR);
	val = hieth_readl(priv->port_base, HIETH_P_GLB_EQFRM_LEN);
	val &= ~HIETH_P_GLB_EQFRM_TXINQ_LEN_MSK;
	val |= skb->len + 4;
	hieth_writel(priv->port_base, skb->len + 4, HIETH_P_GLB_EQFRM_LEN);

	skb_queue_tail(&priv->tx_hw, skb);

	priv->tx_hw_cnt++;

_trans_exit:
	local_unlock(priv);

	return ret;
}

static struct sk_buff *hieth_platdev_alloc_skb(struct hieth_netdev_priv *priv)
{
	struct sk_buff *skb;
	int i;

	skb = priv->rx_pool.sk_pool[priv->rx_pool.next_free_skb++];

	if (priv->rx_pool.next_free_skb == CONFIG_HIETH_MAX_RX_POOLS)
		priv->rx_pool.next_free_skb = 0;

	/*current skb is used by kernel or other process,find another skb*/
	if (skb_shared(skb) || (atomic_read(&(skb_shinfo(skb)->dataref)) > 1)) {
		for (i = 0; i < CONFIG_HIETH_MAX_RX_POOLS; i++) {
			skb = priv->rx_pool.sk_pool[priv->
						    rx_pool.next_free_skb++];
			if (priv->rx_pool.next_free_skb ==
			    CONFIG_HIETH_MAX_RX_POOLS)
				priv->rx_pool.next_free_skb = 0;

			if ((skb_shared(skb) == 0) &&
			    (atomic_read(&(skb_shinfo(skb)->dataref)) <= 1))
				break;
		}

		if (i == CONFIG_HIETH_MAX_RX_POOLS) {
			priv->stat.rx_pool_dry_times++;
			pr_debug("%ld: no free skb\n",
				    priv->stat.rx_pool_dry_times);
			skb = dev_alloc_skb(SKB_SIZE);
			return skb;
		}
	}
	memset(skb, 0, offsetof(struct sk_buff, tail));

	skb->data = skb->head;
	skb_reset_tail_pointer(skb);
	WARN(skb->end != (skb->tail + SKB_DATA_ALIGN(SKB_SIZE + NET_SKB_PAD)),
	     "head=%p, tail=%x, end=%x\n", skb->head, (unsigned int)skb->tail,
	     (unsigned int)skb->end);
	skb->end = skb->tail + SKB_DATA_ALIGN(SKB_SIZE + NET_SKB_PAD);

	skb_reserve(skb, NET_SKB_PAD);
	skb->len = 0;
	skb->data_len = 0;
	skb->cloned = 0;
	atomic_inc(&skb->users);
	return skb;
}

static int hieth_feed_hw(struct hieth_netdev_priv *priv)
{
	struct sk_buff *skb;
	int cnt = 0;
	int rx_head_len;

	/* if skb occupied too much, then do not alloc any more. */
	rx_head_len = skb_queue_len(&priv->rx_head);
	if (rx_head_len > HIETH_MAX_RX_HEAD_LEN)
		return 0;

	local_lock(priv);

	while (hieth_readl(priv->port_base, HIETH_P_GLB_RO_QUEUE_STAT) &
		HIETH_P_GLB_RO_QUEUE_STAT_RECVQ_RDY_MSK) {
		skb = hieth_platdev_alloc_skb(priv);
		if (!skb)
			break;

		dma_map_single(priv->dev, skb->data, HIETH_MAX_FRAME_SIZE,
			       DMA_FROM_DEVICE);
		hieth_writel(priv->port_base, virt_to_phys(skb->data + 2),
			     HIETH_P_GLB_IQ_ADDR);
		skb_queue_tail(&priv->rx_hw, skb);
		cnt++;
	}

	local_unlock(priv);
	return cnt;
}

int hieth_recv_budget(struct hieth_netdev_priv *priv, int budget)
{
	struct sk_buff *skb;
	uint32_t rlen;
	int cnt = 0;

	local_lock(priv);

	while ((hieth_readl(priv->glb_base, HIETH_GLB_IRQ_RAW) &
		(UD_BIT_NAME(HIETH_GLB_IRQ_INT_RX_RDY)))) {

		if (budget > 0 && cnt >= budget)
			break;

		rlen = hieth_readl(priv->port_base, HIETH_P_GLB_RO_IQFRM_DES);
		rlen &= HIETH_P_GLB_RO_IQFRM_DES_FDIN_LEN_MSK;
		rlen -= 4; /* remove FCS 4Bytes */

		/* hw set rx pkg finish */
		hieth_writel(priv->glb_base,
			     UD_BIT_NAME(HIETH_GLB_IRQ_INT_RX_RDY),
			     HIETH_GLB_IRQ_RAW);

		skb = skb_dequeue(&priv->rx_hw);

		if (!skb) {
			pr_err("chip told us to receive pkg,"
			       "but no more can be received!\n");
			BUG();
			break;
		}

		dma_map_single(priv->dev, skb->data, HIETH_MAX_FRAME_SIZE,
			DMA_FROM_DEVICE);
		skb_reserve(skb, 2);
		skb_put(skb, rlen);

		skb_queue_tail(&priv->rx_head, skb);
		cnt++;
	}

	local_unlock(priv);

	/* fill hardware receive queue again */
	hieth_feed_hw(priv);

	return cnt;
}

static void hieth_adjust_link(struct net_device *dev)
{
	int stat = 0;
	struct hieth_netdev_priv *priv = netdev_priv(dev);

	stat |= (priv->phy->link) ? HIETH_P_MAC_PORTSET_LINKED : 0;
	stat |= (priv->phy->duplex == DUPLEX_FULL) ?
		HIETH_P_MAC_PORTSET_DUP_FULL : 0;
	stat |= (priv->phy->speed == SPEED_100) ?
		HIETH_P_MAC_PORTSET_SPD_100M : 0;

	/* The following expression
	 * "(stat | priv->link_stat) & HIETH_P_MAC_PORTSET_LINKED"
	 * means we only consider three link status change as valid:
	 * 1) down -> up;
	 * 2) up -> down;
	 * 3) up -> up; (maybe the link speed and duplex changed)
	 * We will ignore the "down -> down" condition.
	 */
	if ((stat != priv->link_stat) &&
	    ((stat | priv->link_stat) & HIETH_P_MAC_PORTSET_LINKED)) {
		hieth_writel(priv->port_base, stat, HIETH_P_MAC_PORTSET);
		phy_print_status(priv->phy);
		priv->link_stat = stat;

		if (hieth_enable_autoeee)
			hieth_autoeee_init(priv, stat);
	}
}

static int hieth_init_skb_buffers(struct hieth_netdev_priv *priv)
{
	int i;
	struct sk_buff *skb;

	for (i = 0; i < CONFIG_HIETH_MAX_RX_POOLS; i++) {
		skb = dev_alloc_skb(SKB_SIZE);
		if (!skb)
			break;
		priv->rx_pool.sk_pool[i] = skb;
	}

	if (i < CONFIG_HIETH_MAX_RX_POOLS) {
		pr_err("no mem\n");
		for (i--; i > 0; i--)
			dev_kfree_skb_any(priv->rx_pool.sk_pool[i]);
		return -ENOMEM;
	}

	priv->rx_pool.next_free_skb = 0;
	priv->stat.rx_pool_dry_times = 0;
	return 0;
}

static void hieth_destroy_skb_buffers(struct hieth_netdev_priv *priv)
{
	int i;

	for (i = 0; i < CONFIG_HIETH_MAX_RX_POOLS; i++)
		dev_kfree_skb_any(priv->rx_pool.sk_pool[i]);

	priv->rx_pool.next_free_skb = 0;
	priv->stat.rx_pool_dry_times = 0;
}

static void hieth_net_isr_proc(struct net_device *ndev, int ints)
{
	struct hieth_netdev_priv *priv = netdev_priv(ndev);

	if (ints & UD_BIT_NAME(HIETH_GLB_IRQ_INT_MULTI_RXRDY)) {

		if (likely(napi_schedule_prep(&priv->napi))) {

			hieth_irq_disable(priv, UD_BIT_NAME(HIETH_GLB_IRQ_INT_MULTI_RXRDY));
			__napi_schedule(&priv->napi);
		}
	}

	if (ints & UD_BIT_NAME(HIETH_GLB_IRQ_INT_TXQUE_RDY)) {
		hieth_irq_disable(priv,
				  UD_BIT_NAME(HIETH_GLB_IRQ_INT_TXQUE_RDY));
		netif_wake_queue(ndev);
	}
}

static irqreturn_t hieth_net_isr(int irq, void *dev_id)
{
	int ints;
	struct net_device *dev = (struct net_device *)dev_id;
	struct hieth_netdev_priv *priv = netdev_priv(dev);

	/*mask the all interrupt */
	hieth_irq_disable(priv, HIETH_GLB_IRQ_ENA_IEN_A);

	ints = hieth_readl(priv->glb_base, HIETH_GLB_IRQ_STAT);

	if ((HIETH_PORT_0 == priv->port) &&
	    likely(ints & HIETH_GLB_IRQ_ENA_BIT_U)) {
		hieth_net_isr_proc(dev, (ints & HIETH_GLB_IRQ_ENA_BIT_U));
		hieth_clear_irqstatus(priv, (ints & HIETH_GLB_IRQ_ENA_BIT_U));
		ints &= ~HIETH_GLB_IRQ_ENA_BIT_U;
	}

	if ((HIETH_PORT_1 == priv->port) &&
	    likely(ints & HIETH_GLB_IRQ_ENA_BIT_D)) {
		hieth_net_isr_proc(dev, (ints & HIETH_GLB_IRQ_ENA_BIT_D));
		hieth_clear_irqstatus(priv, (ints & HIETH_GLB_IRQ_ENA_BIT_D));
		ints &= ~HIETH_GLB_IRQ_ENA_BIT_D;
	}

	/*unmask the all interrupt */
	hieth_irq_enable(priv, HIETH_GLB_IRQ_ENA_IEN_A);

	return IRQ_HANDLED;
}

static void hieth_monitor_func(unsigned long arg)
{
	struct net_device *dev = (struct net_device *)arg;
	struct hieth_netdev_priv *priv = netdev_priv(dev);

	if (!priv || !netif_running(dev)) {
		pr_debug("network driver is stopped.\n");
		return;
	}

	hieth_feed_hw(priv);
	hieth_xmit_release_skb(priv);

	priv->monitor.expires =
	    jiffies + msecs_to_jiffies(HIETH_MONITOR_TIMER);
	add_timer(&priv->monitor);
}

static int hieth_net_open(struct net_device *dev)
{
	int ret = 0;
	struct cpumask cpumask;
	struct hieth_netdev_priv *priv = netdev_priv(dev);

	ret = request_irq(dev->irq, hieth_net_isr, IRQF_SHARED,
			  dev->name, dev);
	if (ret) {
		pr_err("request_irq %d failed!\n", dev->irq);
		return ret;
	}

	/* set irq affinity */
	if ((num_online_cpus() > 1) && cpu_online(HIETH_IRQ_AFFINITY_CPU)) {
		cpumask_clear(&cpumask);
		cpumask_set_cpu(HIETH_IRQ_AFFINITY_CPU, &cpumask);
		irq_set_affinity(dev->irq, &cpumask);
	}

	if (!is_valid_ether_addr(dev->dev_addr))
		random_ether_addr(dev->dev_addr);

	hieth_hw_set_macaddress(priv, dev->dev_addr);

	/* setup hardware */
	hieth_set_hwq_depth(priv);
	hieth_clear_irqstatus(priv, UD_BIT_NAME(HIETH_GLB_IRQ_ENA_BIT));

	netif_carrier_off(dev);
	hieth_feed_hw(priv);
	
	netif_wake_queue(dev);
	napi_enable(&priv->napi);

	priv->link_stat = 0;
	if (priv->phy)
		phy_start(priv->phy);

	hieth_irq_enable(priv, UD_BIT_NAME(HIETH_GLB_IRQ_INT_MULTI_RXRDY) |
			UD_BIT_NAME(HIETH_GLB_IRQ_ENA_IEN) |
			HIETH_GLB_IRQ_ENA_IEN_A);

	priv->monitor.expires =
	    jiffies + msecs_to_jiffies(HIETH_MONITOR_TIMER);
	add_timer(&priv->monitor);

	return 0;
}

static int hieth_net_close(struct net_device *dev)
{
	struct hieth_netdev_priv *priv = netdev_priv(dev);

	hieth_irq_disable(priv, UD_BIT_NAME(HIETH_GLB_IRQ_INT_MULTI_RXRDY));
	napi_disable(&priv->napi);
	netif_stop_queue(dev);
	if (priv->phy)
		phy_stop(priv->phy);

	del_timer_sync(&priv->monitor);

	/* reset and init port */
	hieth_port_reset(priv);

	skb_queue_purge(&priv->rx_head);
	skb_queue_purge(&priv->rx_hw);
	skb_queue_purge(&priv->tx_hw);
	priv->tx_hw_cnt = 0;

	free_irq(dev->irq, dev);
	return 0;
}

static void hieth_net_timeout(struct net_device *dev)
{
	pr_err("tx timeout\n");
}

static int hieth_net_hard_start_xmit(struct sk_buff *skb,
				     struct net_device *dev)
{
	int ret;
	struct hieth_netdev_priv *priv = netdev_priv(dev);

	hieth_xmit_release_skb(priv);
	dma_map_single(priv->dev, skb->data, skb->len, DMA_TO_DEVICE);

	ret = hieth_xmit_real_send(priv, skb);
	if (ret < 0) {
		priv->stats.tx_dropped++;
		pr_err("tx bug, drop packet\n");
		BUG();
		return NETDEV_TX_BUSY;
	}

	dev->trans_start = jiffies;
	priv->stats.tx_packets++;
	priv->stats.tx_bytes += skb->len;
	hieth_clear_irqstatus(priv, UD_BIT_NAME(HIETH_GLB_IRQ_INT_TXQUE_RDY));

	if (!hieth_hw_xmitq_ready(priv)) {
		netif_stop_queue(dev);
		hieth_irq_enable(priv,
				 UD_BIT_NAME(HIETH_GLB_IRQ_INT_TXQUE_RDY));
	}

	return NETDEV_TX_OK;
}

static struct net_device_stats *hieth_net_get_stats(struct net_device *dev)
{
	struct hieth_netdev_priv *priv = netdev_priv(dev);

	return &priv->stats;
}

static int hieth_net_set_mac_address(struct net_device *dev, void *p)
{
	struct hieth_netdev_priv *priv = netdev_priv(dev);
	struct sockaddr *skaddr = p;

	memcpy(dev->dev_addr, skaddr->sa_data, dev->addr_len);
	dev->addr_assign_type &= ~NET_ADDR_RANDOM;

	hieth_hw_set_macaddress(priv, dev->dev_addr);

	return 0;
}

static inline void hieth_enable_mac_addr_filter(struct hieth_netdev_priv *priv,
						unsigned int reg_n, int enable)
{
	u32 val;

	val = hieth_readl(priv->glb_base, GLB_MAC_H16(priv->port, reg_n));
	if (enable)
		val |= UD_BIT_NAME(HIETH_GLB_MACFLT_ENA);
	else
		val &= ~(UD_BIT_NAME(HIETH_GLB_MACFLT_ENA));
	hieth_writel(priv->glb_base, val, GLB_MAC_H16(priv->port, reg_n));
}

static void hieth_set_mac_addr(struct hieth_netdev_priv *priv, u8 addr[6],
			       unsigned int high, unsigned int low)
{
	u32 val;
	u32 data;

	val = hieth_readl(priv->glb_base, high);
	val |= UD_BIT_NAME(HIETH_GLB_MACFLT_ENA);
	hieth_writel(priv->glb_base, val, high);

	val &= ~HIETH_GLB_MACFLT_HI16;
	val |= ((addr[0] << 8) | addr[1]);
	hieth_writel(priv->glb_base, val, high);

	data = (addr[2] << 24) | (addr[3] << 16) | (addr[4] << 8) | addr[5];
	hieth_writel(priv->glb_base, data, low);

	val |= UD_BIT_NAME(HIETH_GLB_MACFLT_FW2CPU);
	hieth_writel(priv->glb_base, val, high);
}

static inline void hieth_set_mac_addr_filter(struct hieth_netdev_priv *priv,
					     unsigned char *addr,
					     unsigned int reg_n)
{
	hieth_set_mac_addr(priv, addr, GLB_MAC_H16(priv->port, reg_n),
			   GLB_MAC_L32(priv->port, reg_n));
}

static void hieth_net_set_rx_mode(struct net_device *dev)
{
	u32 val;
	struct hieth_netdev_priv *priv = netdev_priv(dev);

	local_lock(priv);
	multicast_dump_netdev_flags(dev->flags);
	val = hieth_readl(priv->glb_base, HIETH_GLB_FWCTRL);
	if (dev->flags & IFF_PROMISC) {
		val |= ((priv->port == HIETH_PORT_0) ?
			HIETH_GLB_FWCTRL_FWALL2CPU_U :
			HIETH_GLB_FWCTRL_FWALL2CPU_D);
		hieth_writel(priv->glb_base, val, HIETH_GLB_FWCTRL);
	} else {
		val &= ~((priv->port == HIETH_PORT_0) ?
			HIETH_GLB_FWCTRL_FWALL2CPU_U :
			HIETH_GLB_FWCTRL_FWALL2CPU_D);
		hieth_writel(priv->glb_base, val, HIETH_GLB_FWCTRL);

		val = hieth_readl(priv->glb_base, HIETH_GLB_MACTCTRL);
		if ((netdev_mc_count(dev) > HIETH_MAX_MULTICAST_ADDRESSES) ||
		    (dev->flags & IFF_ALLMULTI)) {
			val |= UD_BIT_NAME(HIETH_GLB_MACTCTRL_MULTI2CPU);
		} else {
			int reg = HIETH_MAX_UNICAST_ADDRESSES;
			int i, nr = 0;
			struct netdev_hw_addr *ha;

			for (i = reg; i < HIETH_MAX_MAC_FILTER_NUM; i++)
				hieth_enable_mac_addr_filter(priv, i, 0);

			netdev_for_each_mc_addr(ha, dev) {
				hieth_set_mac_addr_filter(priv, ha->addr, reg);
				multicast_dump_macaddr(nr++, ha->addr);
				reg++;
			}

			val &= ~(UD_BIT_NAME(HIETH_GLB_MACTCTRL_MULTI2CPU));
		}

		/* Handle multiple unicast addresses (perfect filtering)*/
		if (netdev_uc_count(dev) > HIETH_MAX_UNICAST_ADDRESSES) {
			val |= UD_BIT_NAME(HIETH_GLB_MACTCTRL_UNI2CPU);
		} else {
			int reg = 0;
			int i;
			struct netdev_hw_addr *ha;

			for (i = reg; i < HIETH_MAX_UNICAST_ADDRESSES; i++)
				hieth_enable_mac_addr_filter(priv, i, 0);

			netdev_for_each_uc_addr(ha, dev) {
				hieth_set_mac_addr_filter(priv, ha->addr, reg);
				reg++;
			}

			val &= ~(UD_BIT_NAME(HIETH_GLB_MACTCTRL_UNI2CPU));
		}
		hieth_writel(priv->glb_base, val, HIETH_GLB_MACTCTRL);
	}

	local_unlock(priv);
}

static int hieth_net_ioctl(struct net_device *net_dev,
			   struct ifreq *ifreq, int cmd)
{
	struct hieth_netdev_priv *priv = netdev_priv(net_dev);
	struct hieth_pm_config pm_config;

	switch (cmd) {
	case SIOCSETPM:
		if (copy_from_user(&pm_config, ifreq->ifr_data,
				   sizeof(pm_config)))
			return -EFAULT;
		return hieth_pmt_config(&pm_config);

	default:
		if (!netif_running(net_dev))
			return -EINVAL;

		if (!priv->phy)
			return -EINVAL;

		return phy_mii_ioctl(priv->phy, ifreq, cmd);
	}

	return 0;
}

static void hieth_ethtools_get_drvinfo(struct net_device *net_dev,
				       struct ethtool_drvinfo *info)
{
	strcpy(info->driver, "hieth driver");
	strcpy(info->version, "v300");
	strcpy(info->bus_info, "platform");
}

static u32 hieth_ethtools_get_link(struct net_device *net_dev)
{
	struct hieth_netdev_priv *priv = netdev_priv(net_dev);

	return ((priv->phy->link) ? HIETH_P_MAC_PORTSET_LINKED : 0);
}

static int hieth_ethtools_get_settings(struct net_device *net_dev,
				       struct ethtool_cmd *cmd)
{
	struct hieth_netdev_priv *priv = netdev_priv(net_dev);

	if (priv->phy)
		return phy_ethtool_gset(priv->phy, cmd);

	return -EINVAL;
}

static int hieth_ethtools_set_settings(struct net_device *net_dev,
				       struct ethtool_cmd *cmd)
{
	struct hieth_netdev_priv *priv = netdev_priv(net_dev);

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (priv->phy)
		return phy_ethtool_sset(priv->phy, cmd);

	return -EINVAL;
}

static struct ethtool_ops hieth_ethtools_ops = {
	.get_drvinfo = hieth_ethtools_get_drvinfo,
	.get_link = hieth_ethtools_get_link,
	.get_settings = hieth_ethtools_get_settings,
	.set_settings = hieth_ethtools_set_settings,
};

static const struct net_device_ops hieth_netdev_ops = {
	.ndo_open = hieth_net_open,
	.ndo_stop = hieth_net_close,
	.ndo_start_xmit = hieth_net_hard_start_xmit,
	.ndo_tx_timeout = hieth_net_timeout,
	.ndo_do_ioctl = hieth_net_ioctl,
	.ndo_set_mac_address = hieth_net_set_mac_address,
	.ndo_set_rx_mode	= hieth_net_set_rx_mode,
	.ndo_change_mtu		= eth_change_mtu,
	.ndo_get_stats = hieth_net_get_stats,
};

void hieth_clean_rx(struct hieth_netdev_priv *priv, unsigned int *workdone, int budget)
{
	unsigned int nr_recv = 0;
	struct sk_buff *skb;
	struct net_device *dev = hieth_devs_save[priv->port];
	int ret = 0;

	hieth_recv_budget(priv, budget);

	while ((skb = skb_dequeue(&priv->rx_head)) != NULL) {

		skb->protocol = eth_type_trans(skb, dev);

		if (HIETH_INVALID_RXPKG_LEN(skb->len)) {
			pr_err("pkg len error");
			priv->stats.rx_errors++;
			priv->stats.rx_length_errors++;
			dev_kfree_skb_any(skb);
			continue;
		}

		priv->stats.rx_packets++;
		priv->stats.rx_bytes += skb->len;
		dev->last_rx = jiffies;
		skb->dev = dev;

		ret = netif_receive_skb(skb);
		if (ret) {
			priv->stats.rx_dropped++;
		}

		nr_recv++;
	}

	if (workdone)
		*workdone = nr_recv;
}


static int hieth_poll(struct napi_struct *napi, int budget)
{
	struct hieth_netdev_priv *priv = NULL;
	unsigned int work_done = 0;
	priv= container_of(napi, struct hieth_netdev_priv, napi);

	hieth_clean_rx(priv, &work_done, budget);

	if (work_done < budget) {
		napi_complete(napi);
		hieth_irq_enable(priv, UD_BIT_NAME(HIETH_GLB_IRQ_INT_MULTI_RXRDY));
	}
	return work_done;
}


static int hieth_platdev_probe_port(struct platform_device *pdev,
				    struct hieth_netdev_priv *com_priv,
				    int port)
{
	int ret = -1;
	struct net_device *netdev = NULL;
	struct device *dev = &pdev->dev;
	struct hieth_netdev_priv *priv;

	if ((HIETH_PORT_0 != port) && (HIETH_PORT_1 != port)) {
		pr_err("port error!\n");
		ret = -ENODEV;
		goto _error_exit;
	}

	netdev = alloc_etherdev(sizeof(*priv));
	if (netdev == NULL) {
		pr_err("alloc_etherdev fail!\n");
		ret = -ENOMEM;
		goto _error_exit;
	}

	platform_set_drvdata(pdev, netdev);
	SET_NETDEV_DEV(netdev, &pdev->dev);

	netdev->irq = com_priv->irq;

	netdev->watchdog_timeo = 3 * HZ;
	netdev->netdev_ops = &hieth_netdev_ops;
	netdev->ethtool_ops = &hieth_ethtools_ops;

	netdev->priv_flags |= IFF_UNICAST_FLT;

	if (hieth_phy_param[port].macaddr)
		ether_addr_copy(netdev->dev_addr,
				hieth_phy_param[port].macaddr);

	if (!is_valid_ether_addr(netdev->dev_addr))
		eth_hw_addr_random(netdev);

	/* init hieth_global somethings... */
	hieth_devs_save[port] = netdev;

	/* init hieth_local_driver */
	priv = netdev_priv(netdev);
	memset(priv, 0, sizeof(*priv));
	memcpy(priv, com_priv, sizeof(*priv));

	local_lock_init(priv);

	priv->port = port;

	if (port == HIETH_PORT_0)
		priv->port_base = priv->glb_base;
	else	
		priv->port_base = priv->glb_base + 0x2000;

	priv->dev = dev;

	init_timer(&priv->monitor);
	priv->monitor.function = hieth_monitor_func;
	priv->monitor.data = (unsigned long)netdev;
	priv->monitor.expires =
	    jiffies + msecs_to_jiffies(HIETH_MONITOR_TIMER);

	/* wol need */
	device_set_wakeup_capable(priv->dev, 1);
	/* TODO: when we can let phy powerdown?
	 * In forcing fwd mode, we don't want phy powerdown,
	 * so I set wakeup enable all the time
	 */
	device_set_wakeup_enable(priv->dev, 1);

	/* reset and init port */
	hieth_port_init(priv);

	priv->depth.hw_xmitq = HIETH_HWQ_XMIT_DEPTH;

	priv->phy = of_phy_connect(netdev, priv->phy_node,
				   hieth_adjust_link, 0, priv->phy_mode);
	if (!(priv->phy) || IS_ERR(priv->phy)) {
		pr_info("connect to port[%d] PHY failed!\n", port);
		priv->phy = NULL;
		goto _error_phy_connect;
	}

	pr_info("attached port %d PHY %d to driver %s\n",
		port, priv->phy->addr, priv->phy->drv->name);

	if (hieth_enable_autoeee)
		hieth_autoeee_init(priv, 0);

	skb_queue_head_init(&priv->rx_head);
	skb_queue_head_init(&priv->rx_hw);
	skb_queue_head_init(&priv->tx_hw);
	priv->tx_hw_cnt = 0;

	ret = hieth_init_skb_buffers(priv);
	if (ret) {
		pr_err("hieth_init_skb_buffers failed!\n");
		goto _error_init_skb_buffers;
	}

	netif_napi_add(netdev, &priv->napi, hieth_poll, HIETH_NAPI_WEIGHT);

	ret = register_netdev(netdev);
	if (ret) {
		pr_err("register_netdev %s failed!\n", netdev->name);
		goto _error_register_netdev;
	}

	return ret;

_error_register_netdev:
	hieth_destroy_skb_buffers(priv);

_error_init_skb_buffers:
	phy_disconnect(priv->phy);
	priv->phy = NULL;

_error_phy_connect:
	local_lock_exit();
	hieth_devs_save[port] = NULL;
	free_netdev(netdev);

_error_exit:
	return ret;
}

static int hieth_platdev_remove_port(struct platform_device *pdev, int port)
{
	struct net_device *ndev;
	struct hieth_netdev_priv *priv;

	hieth_dbg_deinit();
	ndev = hieth_devs_save[port];

	if (!ndev)
		goto _ndev_exit;

	priv = netdev_priv(ndev);

	unregister_netdev(ndev);
	hieth_destroy_skb_buffers(priv);

	phy_disconnect(priv->phy);
	priv->phy = NULL;

	iounmap((void *)priv->glb_base);

	local_lock_exit();

	hieth_devs_save[port] = NULL;
	free_netdev(ndev);

_ndev_exit:
	return 0;
}

#define DEFAULT_LD_AM          0xe
#define DEFAULT_LDO_AM         0x3
#define DEFAULT_R_TUNING       0x16
static void hieth_of_get_phy_trim_params(int port_index)
{
	struct device_node *chiptrim_node;
	u32 phy_trim_val = 0;
	u8 ld_am, ldo_am, r_tuning;
	int ret;

	/* currently only one internal PHY */
	if (port_index == HIETH_PORT_1)
		return;

	chiptrim_node = of_find_node_by_path("chiptrim");
	if (!chiptrim_node)
		return;

	ld_am = DEFAULT_LD_AM;
	ldo_am = DEFAULT_LDO_AM;
	r_tuning = DEFAULT_R_TUNING;

	ret = of_property_read_u32(chiptrim_node, "chiptrim0", &phy_trim_val);
	if (ret) {
		pr_err("%s,%d: chiptrim0 property not found\n", __func__, __LINE__);
		return;
	}

	if (phy_trim_val) {
		ld_am = (phy_trim_val >> 11) & 0x1f;
		ldo_am = (phy_trim_val >> 8) & 0x7;
		r_tuning = phy_trim_val & 0x3f;
	}

	hieth_phy_param[port_index].trim_params = (r_tuning << 16) | (ldo_am << 8) | ld_am;
}

static int hieth_of_get_param(struct device_node *node)
{
	struct device_node *child = NULL;
	int idx = 0;
	int data;

	/* get auto eee */
	hieth_enable_autoeee = of_property_read_bool(node, "autoeee");

	for_each_available_child_of_node(node, child) {
		/* get phy-addr */
		if (of_property_read_u32(child, "reg", &data))
			return -EINVAL;
		if ((data < 0) || (data >= PHY_MAX_ADDR)) {
			pr_info("%s has invalid PHY address\n",
				child->full_name);
			data = HIETH_INVALID_PHY_ADDR;
		}

		hieth_phy_param[idx].phy_addr = data;
		if (data != HIETH_INVALID_PHY_ADDR)
			hieth_phy_param[idx].isvalid = true;

		/* get phy_mode */
		hieth_phy_param[idx].phy_mode = of_get_phy_mode(child);

		/* get mac */
		hieth_phy_param[idx].macaddr = of_get_mac_address(child);

		/* get gpio_base and bit */
		of_property_read_u32(child, "phy-gpio-base",
				     (u32 *)(&hieth_phy_param[idx].gpio_base));
		of_property_read_u32(child, "phy-gpio-bit",
				     &hieth_phy_param[idx].gpio_bit);

		/* get internal flag */
		hieth_phy_param[idx].isinternal =
			of_property_read_bool(child, "internal-phy");

		hieth_of_get_phy_trim_params(idx);

		if (++idx >= HIETH_MAX_PORT)
			break;
	}

	return 0;
}

static int hieth_plat_driver_probe(struct platform_device *pdev)
{
	int ret = 0;
	int irq;
	struct net_device *ndev = NULL;
	struct device *dev = &pdev->dev;
	struct device_node *node = dev->of_node;
	struct resource *res;
	struct hieth_netdev_priv *priv = &hieth_priv;
	struct device_node *child = NULL;
	int port = -1;

	memset(hieth_devs_save, 0, sizeof(hieth_devs_save));
	memset(hieth_phy_param, 0, sizeof(hieth_phy_param));
	memset(priv, 0, sizeof(*priv));

	if (hieth_of_get_param(node)) {
		pr_err("of get parameter fail\n");
		ret = -ENODEV;
		goto exit;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	priv->glb_base = devm_ioremap_resource(dev, res);
	if (IS_ERR(priv->glb_base)) {
		pr_err("error ioremap resource[%x - %x]\n", res->start, res->end);
		ret = PTR_ERR(priv->glb_base);
		goto exit;
	}
	hieth_dbg_init(priv->glb_base);

	priv->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(priv->clk)) {
		pr_err("failed to get clk\n");
		ret = -ENODEV;
		goto exit;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		pr_err("no IRQ defined!\n");
		ret = -ENODEV;
		goto exit;
	}
	priv->irq = irq;

	/* first disable ETH clock, then reset PHY to load PHY address */
	hieth_phy_reset();

	ret = clk_prepare_enable(priv->clk);
	if (ret < 0) {
		pr_err("failed to enable clk %d\n", ret);
		goto exit;
	}
	/* After MDCK clock giving, wait 5ms before MDIO access */
	msleep(5);

	if (hieth_mdiobus_driver_init(pdev, priv)) {
		pr_err("mdio bus init error!\n");
		ret = -ENODEV;
		goto exit_clk_disable;
	}

	/* phy param */
	hieth_phy_register_fixups();

	for_each_available_child_of_node(node, child) {
		if (++port >= HIETH_MAX_PORT)
			break;

		if (!hieth_phy_param[port].isvalid)
			continue;

		priv->phy_node = of_parse_phandle(node, "phy-handle", port);
		if (!priv->phy_node) {
			pr_err("not find phy-handle [%d]\n", port);
			continue;
		}

		priv->phy_mode = hieth_phy_param[port].phy_mode;

		if (!hieth_platdev_probe_port(pdev, priv, port))
			hieth_real_port_cnt++;
	}

	if (hieth_devs_save[HIETH_PORT_0])
		ndev = hieth_devs_save[HIETH_PORT_0];
	else if (hieth_devs_save[HIETH_PORT_1])
		ndev = hieth_devs_save[HIETH_PORT_1];

	if (!ndev) {
		pr_err("no dev probed!\n");
		ret = -ENODEV;
		goto exit_mdiobus;
	}

	return ret;

exit_mdiobus:
	hieth_mdiobus_driver_exit(priv);

exit_clk_disable:
	clk_disable_unprepare(priv->clk);

exit:

	return ret;
}

static int hieth_plat_driver_remove(struct platform_device *pdev)
{
	int i;
	struct net_device *ndev = NULL;
	struct hieth_netdev_priv *priv = netdev_priv(ndev);

	if (hieth_devs_save[HIETH_PORT_0])
		ndev = hieth_devs_save[HIETH_PORT_0];
	else if (hieth_devs_save[HIETH_PORT_1])
		ndev = hieth_devs_save[HIETH_PORT_1];

	free_irq(ndev->irq, hieth_devs_save);

	for (i = 0; i < HIETH_MAX_PORT; i++)
		hieth_platdev_remove_port(pdev, i);

	hieth_mdiobus_driver_exit(priv);

	clk_disable_unprepare(priv->clk);

	memset(hieth_devs_save, 0, sizeof(hieth_devs_save));

	return 0;
}

#ifdef CONFIG_PM
static int hieth_plat_driver_suspend_port(struct platform_device *pdev,
					  pm_message_t state, int port)
{
	struct net_device *ndev = hieth_devs_save[port];

	if (ndev) {
		if (netif_running(ndev)) {
			hieth_net_close(ndev);
			netif_device_detach(ndev);
		}
	}

	return 0;
}

int hieth_plat_driver_suspend(struct platform_device *pdev,
			      pm_message_t state)
{
	int i;
	bool power_off = true;
	struct hieth_netdev_priv *priv = NULL;

	for (i = 0; i < HIETH_MAX_PORT; i++)
		hieth_plat_driver_suspend_port(pdev, state, i);

	if (hieth_pmt_enter())
		power_off = false;

	if (power_off) {
		for (i = 0; i < HIETH_MAX_PORT; i++) {
			if (hieth_devs_save[i]) {
				priv = netdev_priv(hieth_devs_save[i]);
				genphy_suspend(priv->phy);/* power down phy */
			}
		}

		/* need some time before phy suspend finished. */
		usleep_range(1000, 10000);

		if (priv)
			clk_disable_unprepare(priv->clk);
	}

	return 0;
}

static int hieth_plat_driver_resume_port(struct platform_device *pdev, int port)
{
	struct net_device *ndev = hieth_devs_save[port];
	struct hieth_netdev_priv *priv = netdev_priv(ndev);

	if (ndev) {
		if (netif_running(ndev)) {
			hieth_port_init(priv);
			hieth_net_open(ndev);
			netif_device_attach(ndev);
		}
	}

	return 0;
}

int hieth_plat_driver_resume(struct platform_device *pdev)
{
	int i;
	struct hieth_netdev_priv *priv = &hieth_priv;

	/* first disable ETH clock, then reset PHY to load PHY address */
	clk_prepare_enable(priv->clk);
	clk_disable_unprepare(priv->clk);
	hieth_phy_reset();
	/* enable clk */
	clk_prepare_enable(priv->clk);
	/* After MDCK clock giving, wait 5ms before MDIO access */
	msleep(5);

	for (i = 0; i < HIETH_MAX_PORT; i++)
		hieth_plat_driver_resume_port(pdev, i);

	hieth_pmt_exit();
	return 0;
}
#else
#  define hieth_plat_driver_suspend	NULL
#  define hieth_plat_driver_resume	NULL
#endif

static const struct of_device_id hieth_of_match[] = {
	{.compatible = "hisilicon,hieth", },
	{},
};

MODULE_DEVICE_TABLE(of, hieth_of_match);

static struct platform_driver hieth_platform_driver = {
	.probe = hieth_plat_driver_probe,
	.remove = hieth_plat_driver_remove,
	.suspend = hieth_plat_driver_suspend,
	.resume = hieth_plat_driver_resume,
	.driver = {
		   .owner = THIS_MODULE,
		   .name = HIETH_DRIVER_NAME,
		   .bus = &platform_bus_type,
		   .of_match_table = of_match_ptr(hieth_of_match),
		   },
};

static int hieth_mod_init(void)
{
	int ret = 0;

	if (hieth_disable)
		return 0;

	ret = platform_driver_register(&hieth_platform_driver);
	if (ret)
		pr_err("register platform driver failed!\n");

	return ret;
}

static void hieth_mod_exit(void)
{
	if (hieth_disable)
		return;

	platform_driver_unregister(&hieth_platform_driver);
}

module_init(hieth_mod_init);
module_exit(hieth_mod_exit);

MODULE_DESCRIPTION("Hisilicon ETH driver whith MDIO support");
MODULE_LICENSE("GPL");

/* vim: set ts=8 sw=8 tw=78: */
