#include "util.h"
#include "higmac.h"
#include "ctrl.h"


/*FIXME*/
int higmac_set_linkstat(struct higmac_netdev_local *ld, int mode)
{
	unsigned long old;
	unsigned long p = 0;
	unsigned int val = 0;

	higmac_writel_bits(ld, 1, MODE_CHANGE_EN, BIT_MODE_CHANGE_EN);

	old = higmac_readl_bits(ld, PORT_MODE, BITS_PORT_MODE); 
	higmac_writel_bits(ld, mode, PORT_MODE, BITS_PORT_MODE); 

	higmac_writel_bits(ld, 0, MODE_CHANGE_EN, BIT_MODE_CHANGE_EN);

	/*soft reset*/
	p = (unsigned long)(HIGMAC_SYS_CTL_IO_BASE);
	val = readl(p + PERI_CRG21);
	val &= (~(3 << 4));
	
	if(mode <= 1)
	{
		val |= (0 << 4);
	}
	else if(mode == 3)
	{
		val |= (1 << 4);
	}
	else if(mode == 4)
	{
		val |= (2 << 4);
	}
	else if(mode == 5)
	{
		val |= (3 << 4);
	}
	writel(val, p + PERI_CRG21);

	return old;
}

int higmac_hw_set_macaddress(struct higmac_netdev_local *ld, unsigned char *mac)
{
	unsigned long reg;

	reg = mac[1] | (mac[0] <<8);
	higmac_writel(ld, reg, STATION_ADDR_HIGH);

	reg = mac[5] | (mac[4]<<8) | (mac[3]<<16) | (mac[2]<<24);
	higmac_writel(ld, reg, STATION_ADDR_LOW);

	return 0;
}

int higmac_hw_get_macaddress(struct higmac_netdev_local *ld, unsigned char *mac)
{
	unsigned long reg;

	reg = higmac_readl(ld, STATION_ADDR_HIGH);
	mac[0] = (reg>>8) & 0xff;
	mac[1] = reg & 0xff;

	reg = higmac_readl(ld, STATION_ADDR_LOW);
	mac[2] = (reg>>24) & 0xff;
	mac[3] = (reg>>16) & 0xff;
	mac[4] = (reg>>8) & 0xff;
	mac[5] = reg & 0xff;

	return 0;
}

static inline int _higmac_read_irqstatus(struct higmac_netdev_local *ld)
{
	int status;

	status = higmac_readl(ld, STATUS_PMU_INT);

	return status;
}

int higmac_clear_irqstatus(struct higmac_netdev_local *ld, int irqs)
{
	int status;

	higmac_writel(ld, irqs, RAW_PMU_INT);
	status = _higmac_read_irqstatus(ld);

	return status;
}

/*FIXME*/
int higmac_glb_preinit_dummy(struct higmac_netdev_local *ld)
{	
	unsigned long p = 0;
	volatile unsigned int v = 0;

	/*soft reset*/
	p = (unsigned long)(HIGMAC_SYS_CTL_IO_BASE);
	
	v = readl(p + PERI_CRG21);
	v |= (1 << 1);
	writel(v, p + PERI_CRG21);

	v = readl(p + PERI_CRG21);
	v |= (1 << 0);
	writel(v, p + PERI_CRG21);

	udelay(200);
	
	v = readl(p + PERI_CRG21);
	v &= (~(1 << 0));
	writel(v, p + PERI_CRG21);

	/* drop packet enable */
	higmac_writel(ld, 0x3f, REC_FILT_CONTROL);

	/*clear all interrupt status*/
	higmac_clear_irqstatus(ld, RAW_INT_ALL_MASK);
	
	/* disable interrupts */
	higmac_writel(ld, ~RAW_INT_ALL_MASK, ENA_PMU_INT);

	return 0;
}

/*FIXME*/
void higmac_sys_init(void)
{

}

void higmac_sys_exit(void)
{

}

void higmac_sys_allstop(void)
{

}

int higmac_set_hwq_depth(struct higmac_netdev_local *ld)
{
	if(HIGMAC_HWQ_RX_FQ_DEPTH > HIGMAC_MAX_QUEUE_DEPTH) 
	{
		BUG();
		return -1;
	}

	higmac_writel_bits(ld, 1, RX_FQ_REG_EN, \
		BITS_RX_FQ_DEPTH_EN);

	higmac_writel_bits(ld, HIGMAC_HWQ_RX_FQ_DEPTH<<3, RX_FQ_DEPTH, \
		BITS_RX_FQ_DEPTH);

	higmac_writel_bits(ld, 0, RX_FQ_REG_EN, \
		BITS_RX_FQ_DEPTH_EN);

	if(HIGMAC_HWQ_RX_BQ_DEPTH > HIGMAC_MAX_QUEUE_DEPTH) 
	{
		BUG();
		return -1;
	}

	higmac_writel_bits(ld, 1, RX_BQ_REG_EN, \
		BITS_RX_BQ_DEPTH_EN);

	higmac_writel_bits(ld, HIGMAC_HWQ_RX_BQ_DEPTH<<3, RX_BQ_DEPTH, \
		BITS_RX_BQ_DEPTH);

	higmac_writel_bits(ld, 0, RX_BQ_REG_EN, \
		BITS_RX_BQ_DEPTH_EN);

	if(HIGMAC_HWQ_TX_BQ_DEPTH > HIGMAC_MAX_QUEUE_DEPTH) 
	{
		BUG();
		return -1;
	}

	higmac_writel_bits(ld, 1, TX_BQ_REG_EN, \
		BITS_TX_BQ_DEPTH_EN);

	higmac_writel_bits(ld, HIGMAC_HWQ_TX_BQ_DEPTH<<3, TX_BQ_DEPTH, \
		BITS_TX_BQ_DEPTH);

	higmac_writel_bits(ld, 0, TX_BQ_REG_EN, \
		BITS_TX_BQ_DEPTH_EN);

	if(HIGMAC_HWQ_TX_RQ_DEPTH > HIGMAC_MAX_QUEUE_DEPTH) 
	{
		BUG();
		return -1;
	}

	higmac_writel_bits(ld, 1, TX_RQ_REG_EN, \
		BITS_TX_RQ_DEPTH_EN);

	higmac_writel_bits(ld, HIGMAC_HWQ_TX_RQ_DEPTH<<3, TX_RQ_DEPTH, \
		BITS_TX_RQ_DEPTH);

	higmac_writel_bits(ld, 0, TX_RQ_REG_EN, \
		BITS_TX_RQ_DEPTH_EN);

	return 0;
}

int higmac_set_rx_fq_hwq_addr(struct higmac_netdev_local *ld, void* phy_addr)
{
	higmac_writel_bits(ld, 1, RX_FQ_REG_EN, \
		BITS_RX_FQ_START_ADDR_EN);

	higmac_writel(ld, phy_addr, RX_FQ_START_ADDR);

	higmac_writel_bits(ld, 0, RX_FQ_REG_EN, \
		BITS_RX_FQ_START_ADDR_EN);

	return 0;
}

int higmac_set_rx_bq_hwq_addr(struct higmac_netdev_local *ld, void* phy_addr)
{
	higmac_writel_bits(ld, 1, RX_BQ_REG_EN, \
		BITS_RX_BQ_START_ADDR_EN);

	higmac_writel(ld, phy_addr, RX_BQ_START_ADDR);

	higmac_writel_bits(ld, 0, RX_BQ_REG_EN, \
		BITS_RX_BQ_START_ADDR_EN);

	return 0;
}

int higmac_set_tx_bq_hwq_addr(struct higmac_netdev_local *ld, void* phy_addr)
{
	higmac_writel_bits(ld, 1, TX_BQ_REG_EN, \
		BITS_TX_BQ_START_ADDR_EN);

	higmac_writel(ld, phy_addr, TX_BQ_START_ADDR);

	higmac_writel_bits(ld, 0, TX_BQ_REG_EN, \
		BITS_TX_BQ_START_ADDR_EN);

	return 0;
}

int higmac_set_tx_rq_hwq_addr(struct higmac_netdev_local *ld, void* phy_addr)
{
	higmac_writel_bits(ld, 1, TX_RQ_REG_EN, \
		BITS_TX_RQ_START_ADDR_EN);

	higmac_writel(ld, phy_addr, TX_RQ_START_ADDR);

	higmac_writel_bits(ld, 0, TX_RQ_REG_EN, \
		BITS_TX_RQ_START_ADDR_EN);

	return 0;
}

int higmac_desc_enable(struct higmac_netdev_local *ld, int desc_ena)
{
	int old;

	old = higmac_readl(ld, DESC_WR_RD_ENA);
	higmac_writel(ld, old | desc_ena, DESC_WR_RD_ENA);

	return old;
}

int higmac_desc_disable(struct higmac_netdev_local *ld, int desc_dis)
{
	int old;

	old = higmac_readl(ld, DESC_WR_RD_ENA);
	higmac_writel(ld, old & (~desc_dis), DESC_WR_RD_ENA);

	return old;
}

void higmac_desc_flush(struct higmac_netdev_local *ld)
{
	higmac_writel_bits(ld, 1, STOP_CMD, BITS_TX_STOP_EN);
	while(higmac_readl_bits(ld,FLUSH_CMD, BITS_TX_FLUSH_FLAG) != 1);
	higmac_writel_bits(ld, 1, FLUSH_CMD, BITS_TX_FLUSH_CMD);
	while(higmac_readl_bits(ld,FLUSH_CMD, BITS_TX_FLUSH_FLAG) != 0);
	higmac_writel_bits(ld, 0, FLUSH_CMD, BITS_TX_FLUSH_CMD);
	higmac_writel_bits(ld, 0, STOP_CMD, BITS_TX_STOP_EN);

	higmac_writel_bits(ld, 1, STOP_CMD, BITS_RX_STOP_EN);
	while(higmac_readl_bits(ld,FLUSH_CMD, BITS_RX_FLUSH_FLAG) != 1);
	higmac_writel_bits(ld, 1, FLUSH_CMD, BITS_RX_FLUSH_CMD);
	while(higmac_readl_bits(ld,FLUSH_CMD, BITS_RX_FLUSH_FLAG) != 0);
	higmac_writel_bits(ld, 0, FLUSH_CMD, BITS_RX_FLUSH_CMD);
	higmac_writel_bits(ld, 0, STOP_CMD, BITS_RX_STOP_EN);
	
}


