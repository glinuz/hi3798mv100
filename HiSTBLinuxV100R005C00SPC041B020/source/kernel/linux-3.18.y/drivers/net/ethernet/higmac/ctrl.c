#include <linux/skbuff.h>
#include <linux/dma-mapping.h>
#include <linux/clk.h>
#include "util.h"
#include "higmac.h"
#include "ctrl.h"

#include "board.c"

#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/tcp.h>
#include <net/protocol.h>
#include <net/ipv6.h>

/* config hardware queue depth */
void higmac_hw_set_desc_queue_depth(struct higmac_netdev_local *ld)
{
	unsigned int v;

	if (HIGMAC_HWQ_RX_FQ_DEPTH > HIGMAC_MAX_QUEUE_DEPTH ||
	    HIGMAC_HWQ_RX_BQ_DEPTH > HIGMAC_MAX_QUEUE_DEPTH ||
	    HIGMAC_HWQ_TX_BQ_DEPTH > HIGMAC_MAX_QUEUE_DEPTH ||
	    HIGMAC_HWQ_TX_RQ_DEPTH > HIGMAC_MAX_QUEUE_DEPTH)
		BUG();

	v = readl(ld->gmac_iobase + RX_FQ_REG_EN);
	v |= BITS_RX_FQ_DEPTH_EN;
	writel(v, ld->gmac_iobase + RX_FQ_REG_EN);
	writel(HIGMAC_HWQ_RX_FQ_DEPTH << DESC_WORD_SHIFT,
	       ld->gmac_iobase + RX_FQ_DEPTH);
	v &= ~BITS_RX_FQ_DEPTH_EN;
	writel(v, ld->gmac_iobase + RX_FQ_REG_EN);

	v = readl(ld->gmac_iobase + RX_BQ_REG_EN);
	v |= BITS_RX_BQ_DEPTH_EN;
	writel(v, ld->gmac_iobase + RX_BQ_REG_EN);
	writel(HIGMAC_HWQ_RX_BQ_DEPTH << DESC_WORD_SHIFT,
	       ld->gmac_iobase + RX_BQ_DEPTH);
	v &= ~BITS_RX_BQ_DEPTH_EN;
	writel(v, ld->gmac_iobase + RX_BQ_REG_EN);

	v = readl(ld->gmac_iobase + TX_BQ_REG_EN);
	v |= BITS_TX_BQ_DEPTH_EN;
	writel(v, ld->gmac_iobase + TX_BQ_REG_EN);
	writel(HIGMAC_HWQ_TX_BQ_DEPTH << DESC_WORD_SHIFT,
	       ld->gmac_iobase + TX_BQ_DEPTH);
	v &= ~BITS_TX_BQ_DEPTH_EN;
	writel(v, ld->gmac_iobase + TX_BQ_REG_EN);

	v = readl(ld->gmac_iobase + TX_RQ_REG_EN);
	v |= BITS_TX_RQ_DEPTH_EN;
	writel(v, ld->gmac_iobase + TX_RQ_REG_EN);
	writel(HIGMAC_HWQ_TX_RQ_DEPTH << DESC_WORD_SHIFT,
	       ld->gmac_iobase + TX_RQ_DEPTH);
	v &= ~BITS_TX_RQ_DEPTH_EN;
	writel(v, ld->gmac_iobase + TX_RQ_REG_EN);
}

void higmac_hw_mac_core_init(struct higmac_netdev_local *ld)
{
	unsigned int v;

	/* disable and clear all interrupts */
	writel(0, ld->gmac_iobase + ENA_PMU_INT);
	writel(~0, ld->gmac_iobase + RAW_PMU_INT);

	/* enable CRC erro packets filter */
	v = readl(ld->gmac_iobase + REC_FILT_CONTROL);
	v |= BIT_CRC_ERR_PASS;
	writel(v, ld->gmac_iobase + REC_FILT_CONTROL);

	/* fix bug for udp and ip error check */
	writel(CONTROL_WORD_CONFIG, ld->gmac_iobase + CONTROL_WORD);

	writel(0, ld->gmac_iobase + COL_SLOT_TIME);

	writel(DUPLEX_HALF, ld->gmac_iobase + MAC_DUPLEX_HALF_CTRL);

	/* FIXME: interrupt when rcv packets >= RX_BQ_INT_THRESHOLD */
	v = RX_BQ_INT_THRESHOLD |
	    (TX_RQ_INT_THRESHOLD << BITS_OFFSET_TX_RQ_IN_TH);
	writel(v, ld->gmac_iobase + IN_QUEUE_TH);

	/* FIXME: rx_bq/tx_rq in timeout threshold */
	writel(0x10000, ld->gmac_iobase + RX_BQ_IN_TIMEOUT_TH);

	writel(0x18000, ld->gmac_iobase + TX_RQ_IN_TIMEOUT_TH);

	higmac_hw_set_desc_queue_depth(ld);
}

void higmac_set_rx_fq_hwq_addr(struct higmac_netdev_local *ld,
			       dma_addr_t phy_addr)
{
	unsigned int v;

	v = readl(ld->gmac_iobase + RX_FQ_REG_EN);
	v |= BITS_RX_FQ_START_ADDR_EN;
	writel(v, ld->gmac_iobase + RX_FQ_REG_EN);

	writel(phy_addr, ld->gmac_iobase + RX_FQ_START_ADDR);

	v &= ~BITS_RX_FQ_START_ADDR_EN;
	writel(v, ld->gmac_iobase + RX_FQ_REG_EN);
}

void higmac_set_rx_bq_hwq_addr(struct higmac_netdev_local *ld,
			       dma_addr_t phy_addr)
{
	unsigned int v;

	v = readl(ld->gmac_iobase + RX_BQ_REG_EN);
	v |= BITS_RX_BQ_START_ADDR_EN;
	writel(v, ld->gmac_iobase + RX_BQ_REG_EN);

	writel(phy_addr, ld->gmac_iobase + RX_BQ_START_ADDR);

	v &= ~BITS_RX_BQ_START_ADDR_EN;
	writel(v, ld->gmac_iobase + RX_BQ_REG_EN);
}

void higmac_set_tx_bq_hwq_addr(struct higmac_netdev_local *ld,
			       dma_addr_t phy_addr)
{
	unsigned int v;

	v = readl(ld->gmac_iobase + TX_BQ_REG_EN);
	v |= BITS_TX_BQ_START_ADDR_EN;
	writel(v, ld->gmac_iobase + TX_BQ_REG_EN);

	writel(phy_addr, ld->gmac_iobase + TX_BQ_START_ADDR);

	v &= ~BITS_TX_BQ_START_ADDR_EN;
	writel(v, ld->gmac_iobase + TX_BQ_REG_EN);
}

void higmac_set_tx_rq_hwq_addr(struct higmac_netdev_local *ld,
			       dma_addr_t phy_addr)
{
	unsigned int v;

	v = readl(ld->gmac_iobase + TX_RQ_REG_EN);
	v |= BITS_TX_RQ_START_ADDR_EN;
	writel(v, ld->gmac_iobase + TX_RQ_REG_EN);

	writel(phy_addr, ld->gmac_iobase + TX_RQ_START_ADDR);

	v &= ~BITS_TX_RQ_START_ADDR_EN;
	writel(v, ld->gmac_iobase + TX_RQ_REG_EN);
}

void higmac_hw_set_desc_queue_addr(struct higmac_netdev_local *ld)
{
	higmac_set_rx_fq_hwq_addr(ld, ld->rx_fq.phys_addr);
	higmac_set_rx_bq_hwq_addr(ld, ld->rx_bq.phys_addr);
	higmac_set_tx_rq_hwq_addr(ld, ld->tx_rq.phys_addr);
	higmac_set_tx_bq_hwq_addr(ld, ld->tx_bq.phys_addr);
}

int higmac_read_irqstatus(struct higmac_netdev_local *ld)
{
	int status;

	status = readl(ld->gmac_iobase + RAW_PMU_INT);

	return status;
}

int higmac_clear_irqstatus(struct higmac_netdev_local *ld, int irqs)
{
	int status;

	writel(irqs, ld->gmac_iobase + RAW_PMU_INT);
	status = higmac_read_irqstatus(ld);

	return status;
}

void higmac_irq_enable(struct higmac_netdev_local *ld)
{
	writel(RX_BQ_IN_INT | RX_BQ_IN_TIMEOUT_INT
	       | TX_RQ_IN_INT | TX_RQ_IN_TIMEOUT_INT,
	       ld->gmac_iobase + ENA_PMU_INT);
}

void higmac_irq_disable(struct higmac_netdev_local *ld)
{
	writel(0, ld->gmac_iobase + ENA_PMU_INT);
}

void higmac_hw_desc_enable(struct higmac_netdev_local *ld)
{
	writel(0xF, ld->gmac_iobase + DESC_WR_RD_ENA);
}

void higmac_hw_desc_disable(struct higmac_netdev_local *ld)
{
	writel(0, ld->gmac_iobase + DESC_WR_RD_ENA);
}

void higmac_port_enable(struct higmac_netdev_local *ld)
{
	writel(BITS_TX_EN | BITS_RX_EN, ld->gmac_iobase + PORT_EN);
}

void higmac_port_disable(struct higmac_netdev_local *ld)
{
	writel(0, ld->gmac_iobase + PORT_EN);
}

int higmac_xmit_release_skb(struct higmac_netdev_local *ld)
{
	struct sk_buff *skb = NULL;
	int tx_rq_wr_offset, tx_rq_rd_offset, pos;
	struct higmac_desc *tx_rq_desc;
	struct higmac_tso_desc *tx_rq_tso_desc = NULL;
	dma_addr_t dma_addr;
	int ret = 0;
	bool release = false;

	tx_rq_wr_offset = readl(ld->gmac_iobase + TX_RQ_WR_ADDR);
	tx_rq_rd_offset = readl(ld->gmac_iobase + TX_RQ_RD_ADDR);

	while (tx_rq_rd_offset != tx_rq_wr_offset) {
		pos = tx_rq_rd_offset >> DESC_BYTE_SHIFT;
reload:
		tx_rq_desc = ld->tx_rq.desc + pos;

		skb = ld->tx_skb[pos];
		if (!skb) {
			pr_err("tx_rq: desc consistent warning\n");
			pr_err("tx_rq:rd=0x%x, wr=0x%x, tx_fq.skb[%d](0x%p)\n",
			       tx_rq_rd_offset, tx_rq_wr_offset,
			       pos, ld->tx_bq.skb[pos]);
			/* logic bug, cause it update tx_rq_wr pointer first
			 * before loading the desc from fifo into tx_rq.
			 * so try to read it again until desc reached the tx_rq
			 * FIXME: use volatile or __iomem to avoid compiler
			 * optimize?
			 */
			goto reload;
		}

		if (ld->tx_bq.skb[pos] != skb) {
			pr_err("wired, tx skb[%d](%p) != skb(%p)\n",
			       pos, ld->tx_bq.skb[pos], skb);
			if (ld->tx_bq.skb[pos] == SKB_MAGIC)
				goto next;
		}

		if (ld->tso_supported) {
			tx_rq_tso_desc = (struct higmac_tso_desc *)tx_rq_desc;
			ret = higmac_xmit_release_gso(ld, tx_rq_tso_desc, pos);
			if (ret < 0)
				break;
		} else {
			dma_addr = tx_rq_desc->data_buff_addr;
			dma_unmap_single(ld->dev, dma_addr, skb->len,
					 DMA_TO_DEVICE);
		}

		ld->tx_bq.skb[pos] = NULL;
		dev_kfree_skb_any(skb);
next:
		ld->tx_skb[pos] = NULL;

		tx_rq_rd_offset += DESC_SIZE;
		if (tx_rq_rd_offset ==
		    (HIGMAC_HWQ_TX_RQ_DEPTH << DESC_BYTE_SHIFT))
			tx_rq_rd_offset = 0;

		writel(tx_rq_rd_offset, ld->gmac_iobase + TX_RQ_RD_ADDR);
		release = true;
	}

	if (release && netif_queue_stopped(ld->netdev)) {
		netif_wake_queue(ld->netdev);
		if (netif_msg_tx_err(ld))
			pr_info("netif_wake_queue(gmac%d)\n", ld->index);
	}

	return ret;
}

#ifdef HIGMAC_TSO_DEBUG
unsigned int id_send;
unsigned int id_free;
struct send_pkt_info pkt_rec[MAX_RECORD];
#endif

int higmac_check_tx_err(struct higmac_netdev_local *ld,
			struct higmac_tso_desc *tx_bq_desc,
			unsigned int desc_pos)
{
	unsigned int tx_err = tx_bq_desc->tx_err;

	if (unlikely(tx_err & ERR_ALL)) {
		struct sg_desc *desc_cur;
		int *sg_word;
		int i;

		WARN((tx_err & ERR_ALL),
		     "TX ERR: desc1=0x%x, desc2=0x%x, desc5=0x%x\n",
		     tx_bq_desc->data_buff_addr,
		     tx_bq_desc->desc1.val, tx_bq_desc->tx_err);

		desc_cur = ld->dma_sg_desc + ld->tx_bq.sg_desc_offset[desc_pos];
		sg_word = (int *)desc_cur;
		for (i = 0; i < sizeof(struct sg_desc) / sizeof(int); i++)
			pr_err("%s,%d: sg_desc word[%d]=0x%x\n",
			       __func__, __LINE__, i, sg_word[i]);

		return -1;
	}

	return 0;
}

int higmac_xmit_release_gso(struct higmac_netdev_local *ld,
			    struct higmac_tso_desc *tx_bq_desc,
			    unsigned int desc_pos)
{
	struct sk_buff *skb = ld->tx_skb[desc_pos];
	int pkt_type;
	int nfrags = skb_shinfo(skb)->nr_frags;

	if (unlikely(higmac_check_tx_err(ld, tx_bq_desc, desc_pos) < 0)) {
		/* dev_close */
		higmac_irq_disable(ld);
		higmac_hw_desc_disable(ld);

		netif_carrier_off(ld->netdev);
		netif_stop_queue(ld->netdev);

		phy_stop(ld->phy);
		del_timer_sync(&ld->monitor);
		return -1;
	}

	if (skb_is_gso(skb) || nfrags)
		pkt_type = PKT_SG;
	else
		pkt_type = PKT_NORMAL;

	if (pkt_type == PKT_NORMAL) {
		dma_addr_t dma_addr;

		dma_addr = tx_bq_desc->data_buff_addr;
		dma_unmap_single(ld->dev, dma_addr, skb->len, DMA_TO_DEVICE);
	} else {
		struct sg_desc *desc_cur;
		unsigned int desc_offset;
		int i;

		desc_offset = ld->tx_bq.sg_desc_offset[desc_pos];
		BUG_ON(desc_offset != ld->sg_tail);
		desc_cur = ld->dma_sg_desc + desc_offset;

		dma_unmap_single(ld->dev, desc_cur->linear_addr,
				 desc_cur->linear_len, DMA_TO_DEVICE);
		for (i = 0; i < nfrags; i++) {
			dma_unmap_page(ld->dev, desc_cur->frags[i].addr,
				       desc_cur->frags[i].size, DMA_TO_DEVICE);
		}

		ld->sg_tail = (ld->sg_tail + 1) % ld->sg_count;
	}

#ifdef HIGMAC_TSO_DEBUG
	pkt_rec[id_free].status = 0;
	id_free++;
	if (id_free == MAX_RECORD)
		id_free = 0;
#endif

	return 0;
}

int higmac_get_pkt_info(struct higmac_netdev_local *ld, struct sk_buff *skb,
			struct higmac_tso_desc *tx_bq_desc)
{
	int nfrags = skb_shinfo(skb)->nr_frags;

	__be16 l3_proto;	/* level 3 protocol */
	unsigned int l4_proto = IPPROTO_MAX;
	unsigned int mtu = 0;
	unsigned char coe_enable = 0;

	if (likely(skb->ip_summed == CHECKSUM_PARTIAL))
		coe_enable = 1;

	tx_bq_desc->desc1.val = 0;

	mtu = skb_shinfo(skb)->gso_size;

	if (skb_is_gso(skb)) {
		tx_bq_desc->desc1.tx.tso_flag = 1;
		tx_bq_desc->desc1.tx.sg_flag = 1;
	} else if (nfrags) {
		tx_bq_desc->desc1.tx.sg_flag = 1;
	}

	/* deal with VLAN flag */
	l3_proto = skb->protocol;
	if (skb->protocol == htons(ETH_P_8021Q)) {
		struct vlan_hdr *vhdr;

		vhdr = (struct vlan_hdr *)(skb->data + ETH_HLEN);
		l3_proto = vhdr->h_vlan_encapsulated_proto;
		tx_bq_desc->desc1.tx.vlan_flag = 1;
	}

	if (l3_proto == htons(ETH_P_IP)) {
		const struct iphdr *iph;

		iph = ip_hdr(skb);
		tx_bq_desc->desc1.tx.ip_ver = PKT_IPV4;
		tx_bq_desc->desc1.tx.ip_hdr_len = iph->ihl;

		mtu += iph->ihl * WORD_TO_BYTE;
		l4_proto = iph->protocol;
		l4_proto = l4_proto & (MAX_INET_PROTOS - 1);
	} else if (l3_proto == htons(ETH_P_IPV6)) {
		const struct ipv6hdr *hdr;

		tx_bq_desc->desc1.tx.ip_ver = PKT_IPV6;
		tx_bq_desc->desc1.tx.ip_hdr_len = PKT_IPV6_HDR_LEN;
		mtu += PKT_IPV6_HDR_LEN * WORD_TO_BYTE;
		hdr = ipv6_hdr(skb);
		l4_proto = hdr->nexthdr;
	} else {
		coe_enable = 0;
		/* pr_err("unknown level 3 prot=0x%x\n", ntohs(l3_proto)); */
	}

	if (l4_proto == IPPROTO_TCP) {
		const struct tcphdr *th;

		th = tcp_hdr(skb);
		tx_bq_desc->desc1.tx.prot_type = PKT_TCP;
		tx_bq_desc->desc1.tx.prot_hdr_len = th->doff;
		mtu += th->doff * WORD_TO_BYTE;
	} else if (l4_proto == IPPROTO_UDP) {
		tx_bq_desc->desc1.tx.prot_type = PKT_UDP;
		tx_bq_desc->desc1.tx.prot_hdr_len = PKT_UDP_HDR_LEN;
		if (l3_proto == htons(ETH_P_IPV6))
			mtu += sizeof(struct frag_hdr);
	} else {
		coe_enable = 0;
		/* pr_err("unknown level 4 prot=0x%x\n", l4_proto); */
		/* TODO: when l3_proto == ETH_P_IPV6,
		 * we need to deal with IPV6 next header.
		 */
		if (l3_proto == htons(ETH_P_IPV6) &&
		    skb->ip_summed == CHECKSUM_PARTIAL)
			return -E_MAC_SW_GSO;
	}

	if (skb_is_gso(skb))
		tx_bq_desc->desc1.tx.data_len = skb_shinfo(skb)->gso_size;
	else
		tx_bq_desc->desc1.tx.data_len = skb->len;

	if (coe_enable) {
		if (skb_is_gso(skb) && (l4_proto == IPPROTO_UDP)) {
			struct ethhdr *eth;

			/* TODO: self checksum caculate */
			int offset;
			__wsum csum;
			__sum16 udp_csum;

			/* hardware can't dea with UFO broadcast packet */
			eth = (struct ethhdr *)(skb->data);
			if (unlikely(is_broadcast_ether_addr(eth->h_dest)))
				return -E_MAC_UFO_BROADCAST;

			offset = skb_checksum_start_offset(skb);
			WARN_ON(offset >= skb_headlen(skb));
			csum = skb_checksum(skb, offset, skb->len - offset, 0);

			offset += skb->csum_offset;
			BUG_ON(offset + sizeof(__sum16) > skb_headlen(skb));
			udp_csum = csum_fold(csum);
			if (udp_csum == 0)
				udp_csum = CSUM_MANGLED_0;

			*(__sum16 *)(skb->data + offset) = udp_csum;

			skb->ip_summed = CHECKSUM_NONE;
		}

		tx_bq_desc->desc1.tx.coe_flag = 1;
	}

	tx_bq_desc->desc1.tx.nfrags_num = nfrags;

	tx_bq_desc->desc1.tx.hw_own = DESC_VLD_BUSY;

	return 0;
}

static int higmac_sw_gso(struct higmac_netdev_local *ld, struct sk_buff *skb)
{
	struct sk_buff *segs, *curr_skb;
	int gso_segs = skb_shinfo(skb)->gso_segs;

	if (gso_segs == 0 && skb_shinfo(skb)->gso_size != 0)
		gso_segs = DIV_ROUND_UP(skb->len, skb_shinfo(skb)->gso_size);

	/* Estimate the number of fragments in the worst case */
	if (unlikely(higmac_tx_avail(ld) < gso_segs)) {
		netif_stop_queue(ld->netdev);
		if (higmac_tx_avail(ld) < gso_segs)
			return -EBUSY;

		netif_wake_queue(ld->netdev);
	}

	segs = skb_gso_segment(skb, ld->netdev->features &
			       ~(NETIF_F_ALL_CSUM | NETIF_F_GSO_SOFTWARE));

	if (IS_ERR(segs))
		goto sw_tso_end;

	do {
		curr_skb = segs;
		segs = segs->next;
		curr_skb->next = NULL;
		higmac_xmit_real_send(ld, curr_skb);
	} while (segs);

sw_tso_end:
	dev_kfree_skb(skb);

	return 0;
}

int higmac_xmit_gso(struct higmac_netdev_local *ld,
		    struct sk_buff *skb, struct higmac_tso_desc *tx_bq_desc,
		    unsigned int desc_pos)
{
	int pkt_type = PKT_NORMAL;
	int nfrags = skb_shinfo(skb)->nr_frags;
	int ret;

	if (skb_is_gso(skb) || nfrags) {
		/* TSO pkt or SG pkt */
		pkt_type = PKT_SG;
	} else {		/* Normal pkt */
		pkt_type = PKT_NORMAL;
	}

	ret = higmac_get_pkt_info(ld, skb, tx_bq_desc);
	if (unlikely(ret < 0))
		return ret;

	if (pkt_type == PKT_NORMAL) {
		tx_bq_desc->data_buff_addr =
		    dma_map_single(ld->dev, skb->data, skb->len, DMA_TO_DEVICE);
		if (unlikely(dma_mapping_error(ld->dev,
					       tx_bq_desc->data_buff_addr))) {
			pr_err("Normal Packet DMA Mapping fail.\n");
			return -E_MAC_TX_FAIL;
		}
	} else {
		struct sg_desc *desc_cur;
		int i;

		if (unlikely(((ld->sg_head + 1) % ld->sg_count) ==
			     ld->sg_tail)) {
			/* SG pkt, but sg desc all used */
			pr_err("WARNING: sg desc all used.\n");
			return -E_MAC_TX_FAIL;
		}

		desc_cur = ld->dma_sg_desc + ld->sg_head;

		/* TODO: deal with ipv6_id */
		if (tx_bq_desc->desc1.tx.tso_flag &&
		    tx_bq_desc->desc1.tx.ip_ver == PKT_IPV6 &&
		    tx_bq_desc->desc1.tx.prot_type == PKT_UDP) {
			desc_cur->ipv6_id = ntohl(skb_shinfo(skb)->ip6_frag_id);
		}

		desc_cur->total_len = skb->len;
		desc_cur->linear_len = skb_headlen(skb);
		desc_cur->linear_addr = dma_map_single(ld->dev, skb->data,
				desc_cur->linear_len, DMA_TO_DEVICE);
		if (unlikely(dma_mapping_error(ld->dev,
					       desc_cur->linear_addr))) {
			pr_err("DMA Mapping fail.");
			return -E_MAC_TX_FAIL;
		}

		for (i = 0; i < nfrags; i++) {
			skb_frag_t *frag = &skb_shinfo(skb)->frags[i];
			int len = frag->size;

			desc_cur->frags[i].addr = skb_frag_dma_map(ld->dev,
					frag, 0, len, DMA_TO_DEVICE);
			ret = dma_mapping_error(ld->dev,
						desc_cur->frags[i].addr);
			if (unlikely(ret)) {
				pr_err("skb frag DMA Mapping fail.");
				return -E_MAC_TX_FAIL;
			}
			desc_cur->frags[i].size = len;
		}
		tx_bq_desc->data_buff_addr = ld->dma_sg_phy +
		    ld->sg_head * sizeof(struct sg_desc);
		ld->tx_bq.sg_desc_offset[desc_pos] = ld->sg_head;

		ld->sg_head = (ld->sg_head + 1) % ld->sg_count;
	}

#ifdef HIGMAC_TSO_DEBUG
	memcpy(&pkt_rec[id_send].desc, tx_bq_desc,
	       sizeof(struct higmac_tso_desc));
	pkt_rec[id_send].status = 1;
	id_send++;
	if (id_send == MAX_RECORD)
		id_send = 0;
#endif
	return 0;
}

int higmac_xmit_real_send(struct higmac_netdev_local *ld, struct sk_buff *skb)
{
	int tx_bq_wr_offset, tx_bq_rd_offset, tmp, pos;
	struct higmac_desc *tx_bq_desc;
	unsigned long txflags;
	int ret;

	/* software write pointer */
	tx_bq_wr_offset = readl(ld->gmac_iobase + TX_BQ_WR_ADDR);
	/* logic read pointer */
	tx_bq_rd_offset = readl(ld->gmac_iobase + TX_BQ_RD_ADDR);

	pos = tx_bq_wr_offset >> DESC_BYTE_SHIFT;
	tmp = tx_bq_wr_offset + DESC_SIZE;
	if (tmp == (HIGMAC_HWQ_TX_BQ_DEPTH << DESC_BYTE_SHIFT))
		tmp = 0;
	if (tmp == tx_bq_rd_offset || ld->tx_bq.skb[pos]) {
		if (netif_msg_tx_err(ld))
			pr_err("txq full!bq[wr=%x,rd=%x]rq[wr=%x,rd=%x]sf=%x\n",
			       tx_bq_wr_offset, tx_bq_rd_offset,
			       readl(ld->gmac_iobase + TX_RQ_WR_ADDR),
			       readl(ld->gmac_iobase + TX_RQ_RD_ADDR), pos);
		/* we will stop the queue outside of this func */
		return -EBUSY;
	}

	spin_lock_irqsave(&ld->txlock, txflags);

	if (unlikely(ld->tx_skb[pos])) {
		spin_unlock_irqrestore(&ld->txlock, txflags);
		return -EBUSY;
	}

	if (unlikely(ld->tx_bq.skb[pos])) {
		spin_unlock_irqrestore(&ld->txlock, txflags);
		return -EBUSY;
	}

	ld->tx_bq.skb[pos] = skb;

	if (!ld->tso_supported)
		spin_unlock_irqrestore(&ld->txlock, txflags);

	tx_bq_desc = ld->tx_bq.desc + pos;

	ld->tx_skb[pos] = skb;

	if (ld->tso_supported) {
		ret = higmac_xmit_gso(ld, skb,
				      (struct higmac_tso_desc *)tx_bq_desc,
				      pos);
		if (unlikely(ret < 0)) {
			ld->tx_skb[pos] = NULL;
			ld->tx_bq.skb[pos] = NULL;
			spin_unlock_irqrestore(&ld->txlock, txflags);
			if (ret == -E_MAC_SW_GSO || ret == -E_MAC_UFO_BROADCAST)
				return higmac_sw_gso(ld, skb);
			else
				return -EBUSY;
		}
		spin_unlock_irqrestore(&ld->txlock, txflags);
	} else {
		tx_bq_desc->data_buff_addr =
			dma_map_single(ld->dev, skb->data, skb->len,
				       DMA_TO_DEVICE);
		WARN_ON(dma_mapping_error(ld->dev, tx_bq_desc->data_buff_addr));
		tx_bq_desc->buffer_len = HIETH_MAX_FRAME_SIZE - 1;
		tx_bq_desc->data_len = skb->len;
		tx_bq_desc->fl = DESC_FL_FULL;
		tx_bq_desc->descvid = DESC_VLD_BUSY;
	}

	tx_bq_wr_offset += DESC_SIZE;
	if (tx_bq_wr_offset >= (HIGMAC_HWQ_TX_BQ_DEPTH << DESC_BYTE_SHIFT))
		tx_bq_wr_offset = 0;

	/* This barrier is important here.  It is required to ensure
	 * the ARM CPU flushes it's DMA write buffers before proceeding
	 * to the next instruction, to ensure that GMAC will see
	 * our descriptor changes in memory
	 */
	HIGMAC_SYNC_BARRIER();

	/* update software write pointer */
	writel(tx_bq_wr_offset, ld->gmac_iobase + TX_BQ_WR_ADDR);

	return 0;
}

int higmac_feed_hw(struct higmac_netdev_local *ld)
{
	int rx_fq_wr_offset, rx_fq_rd_offset;
	struct higmac_desc *rx_fq_desc;
	struct sk_buff *skb;
	int wr_rd_dist;
	int i = 0;
	int start, end, num = 0;

	rx_fq_wr_offset = readl(ld->gmac_iobase + RX_FQ_WR_ADDR);
	rx_fq_rd_offset = readl(ld->gmac_iobase + RX_FQ_RD_ADDR);

	if (rx_fq_wr_offset >= rx_fq_rd_offset)
		wr_rd_dist = (HIGMAC_HWQ_RX_FQ_DEPTH << DESC_BYTE_SHIFT)
		    - (rx_fq_wr_offset - rx_fq_rd_offset);
	else
		wr_rd_dist = rx_fq_rd_offset - rx_fq_wr_offset;

	/* offset was counted on bytes */
	wr_rd_dist >>= DESC_BYTE_SHIFT;

	higmac_trace(5, "rx_fq_wr_offset is %x, rx_fq_rd_offset is %x\n",
		     rx_fq_wr_offset, rx_fq_rd_offset);

	start = rx_fq_wr_offset >> DESC_BYTE_SHIFT;
	/* wr_rd_dist - 1 for logic reason.
	 * Logic think the desc pool is full filled, ...?
	 */
	for (i = 0; i < wr_rd_dist - 1; i++) {
		int pos = rx_fq_wr_offset >> DESC_BYTE_SHIFT;

		if (ld->rx_fq.skb[pos] || ld->rx_skb[pos]) {
			goto out;
		} else {
			skb = dev_alloc_skb(SKB_SIZE);
			if (!skb)
				goto out;

			ld->rx_fq.skb[pos] = skb;
			num++;
		}

		rx_fq_desc = ld->rx_fq.desc +
		    (rx_fq_wr_offset >> DESC_BYTE_SHIFT);

		skb_reserve(skb, 2);
		rx_fq_desc->data_buff_addr =
		    dma_map_single(ld->dev, skb->data,
				   HIETH_MAX_FRAME_SIZE, DMA_FROM_DEVICE);
		WARN_ON(dma_mapping_error(ld->dev, rx_fq_desc->data_buff_addr));
		if (ld->tso_supported) {
			struct higmac_tso_desc *rx_fq_tso_desc;

			rx_fq_tso_desc = (struct higmac_tso_desc *)rx_fq_desc;
			rx_fq_tso_desc->desc1.rx.buffer_len =
			    HIETH_MAX_FRAME_SIZE - 1;
			rx_fq_tso_desc->desc1.rx.data_len = 0;
			rx_fq_tso_desc->desc1.rx.fl = 0;
			rx_fq_tso_desc->desc1.rx.descvid = DESC_VLD_FREE;
		} else {
			rx_fq_desc->buffer_len = HIETH_MAX_FRAME_SIZE - 1;
			rx_fq_desc->data_len = 0;
			rx_fq_desc->fl = 0;
			rx_fq_desc->descvid = DESC_VLD_FREE;
		}
		ld->rx_skb[pos] = skb;

		rx_fq_wr_offset += DESC_SIZE;
		if (rx_fq_wr_offset >=
		    (HIGMAC_HWQ_RX_FQ_DEPTH << DESC_BYTE_SHIFT))
			rx_fq_wr_offset = 0;

		/* This barrier is important here.  It is required to ensure
		 * the ARM CPU flushes it's DMA write buffers before proceeding
		 * to the next instruction, to ensure that GMAC will see
		 * our descriptor changes in memory
		 */
		HIGMAC_SYNC_BARRIER();

		writel(rx_fq_wr_offset, ld->gmac_iobase + RX_FQ_WR_ADDR);
	}
out:
	end = rx_fq_wr_offset >> DESC_BYTE_SHIFT;
	if (netif_msg_rx_err(ld))
		pr_info("gmac%d feed skb[%d-%d)\n", ld->index, start, end);

	return end - start;
}
