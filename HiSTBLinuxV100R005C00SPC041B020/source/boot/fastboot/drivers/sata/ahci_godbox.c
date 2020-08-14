
/* when user specified buffer overflow, we overwrite buffer here!
* for test purpose, we want read/write more data, e.g. 8GBytes
* that is "sata read/write 0xc0000000 0 0x1000000", 0x1000000blk=8GB
* we will read/write sata HDD to/from the buffer repeatedly.
* each time we will read/write 32MBytes, because of ATA cmd's limitation.
*/

#define AHCI_HOST_BASE			REG_BASE_SATA	/* ahci controller base addr */
#define BUFFER_START			(MEM_BASE_DDR+0x03000000)
#define BUFFER_END			(MEM_BASE_DDR+0x13000000)
#define HI_SATA_PERI_CRG		(REG_BASE_CRG + 0xcc)
#define HI_SATA_HRST			(1 << 0)
#define HI_SATA_PHYRST			(1 << 1)
#define HI_SATA_RST			(1 << 2)
#define HI_SATA_TX0_RST			(1 << 3)
#define HI_SATA_TX1_RST			(1 << 4)
#define HI_SATA_RX0_RST			(1 << 5)
#define HI_SATA_RX1_RST			(1 << 6)
#define HI_SATA_ALIVE_RST		(1 << 7)
#define HI_SATA_CKEN			(1 << 8)
#define HI_SATA_PORT0_CKEN		(1 << 9)
#define HI_SATA_PORT1_CKEN		(1 << 10)
#define HI_SATA_CLK_SEL			(1 << 16)
#define HI_GPIO_BASE			REG_BASE_GPIO11
#define HI_GPIO_DIR			(HI_GPIO_BASE + 0x400)
#define HI_GPIO_BIT			4
#define HI_GPIO_BIT_ADDR		(HI_GPIO_BASE + (4 << HI_GPIO_BIT))
#define PHY_CONFIG_3_0_G		0x0e662753;
#define PHY_CONFIG_1_5_G		0x0e262734;

void hi_sata_poweron(void)
{
	unsigned int tmp_val;
	unsigned long flags;
	
	local_irq_save(flags);
	tmp_val = readl(HI_GPIO_DIR);
	tmp_val |= (1 << HI_GPIO_BIT);
	writel(tmp_val,HI_GPIO_DIR);
	writel((1 << HI_GPIO_BIT),HI_GPIO_BIT_ADDR);
	local_irq_restore(flags);
}

void hi_sata_poweroff(void)
{
	unsigned int tmp_val;
	unsigned long flags;

	local_irq_save(flags);
	tmp_val = readl(HI_GPIO_DIR);
	tmp_val |= (1 << HI_GPIO_BIT);
	writel(tmp_val,HI_GPIO_DIR);
	writel(0,HI_GPIO_BIT_ADDR);
	local_irq_restore(flags);
}

void hi_sata_reset(void)
{
	unsigned int tmp_val;

	tmp_val = readl(HI_SATA_PERI_CRG);
	tmp_val |= HI_SATA_HRST;
	writel(tmp_val,HI_SATA_PERI_CRG);
}

void hi_sata_unreset(void)
{
	unsigned int tmp_val;

	tmp_val = readl(HI_SATA_PERI_CRG);
	tmp_val &= ~HI_SATA_HRST;
	writel(tmp_val,HI_SATA_PERI_CRG);
}

void hi_sata_phy_reset(void)
{
	unsigned int tmp_val;

	tmp_val = readl(HI_SATA_PERI_CRG);
	tmp_val |= HI_SATA_PHYRST;
	writel(tmp_val,HI_SATA_PERI_CRG);
}

void hi_sata_phy_unreset(void)
{
	unsigned int tmp_val;

	tmp_val = readl(HI_SATA_PERI_CRG);
	tmp_val &= ~HI_SATA_PHYRST;
	writel(tmp_val,HI_SATA_PERI_CRG);
}

void hi_sata_clk_open(void)
{
	unsigned int tmp_val;

	tmp_val = readl(HI_SATA_PERI_CRG);
	tmp_val |= (HI_SATA_CKEN 
		| HI_SATA_PORT0_CKEN 
		| HI_SATA_PORT1_CKEN);
	writel(tmp_val,HI_SATA_PERI_CRG);
}

void hi_sata_clk_close(void)
{
	unsigned int tmp_val;

	tmp_val = readl(HI_SATA_PERI_CRG);
	tmp_val &= ~(HI_SATA_CKEN 
		| HI_SATA_PORT0_CKEN 
		| HI_SATA_PORT1_CKEN);
	writel(tmp_val,HI_SATA_PERI_CRG);
}

void hi_sata_clk_reset(void)
{
	unsigned int tmp_val;

	tmp_val = readl(HI_SATA_PERI_CRG);
	tmp_val |= (HI_SATA_RST 
		| HI_SATA_TX0_RST 
		| HI_SATA_TX1_RST
		| HI_SATA_RX0_RST 
		| HI_SATA_RX1_RST 
		| HI_SATA_ALIVE_RST);
	writel(tmp_val,HI_SATA_PERI_CRG);
}

void hi_sata_clk_unreset(void)
{
	unsigned int tmp_val;

	tmp_val = readl(HI_SATA_PERI_CRG);
	tmp_val &= ~(HI_SATA_RST 
		| HI_SATA_TX0_RST 
		| HI_SATA_TX1_RST
		| HI_SATA_RX0_RST 
		| HI_SATA_RX1_RST 
		| HI_SATA_ALIVE_RST);
	writel(tmp_val,HI_SATA_PERI_CRG);
}

void hi_sata_phy_clk_sel(void)
{
	unsigned int tmp_val;

	tmp_val = readl(HI_SATA_PERI_CRG);
	tmp_val |= HI_SATA_CLK_SEL;
	writel(tmp_val,HI_SATA_PERI_CRG);
}

void hi_sata_init(void __iomem *mmio)
{
	unsigned int tmp;
	unsigned int phy_config;
	int i;

	hi_sata_poweron();
	msleep(20);
	hi_sata_clk_open();
	hi_sata_phy_clk_sel();
	hi_sata_unreset();
	msleep(20);
	hi_sata_phy_unreset();
	msleep(20);
	tmp = readl(mmio + HI_SATA_PHY0_CTLH);
	tmp |= HI_SATA_DIS_CLK;
	writel(tmp, (mmio + HI_SATA_PHY0_CTLH));
	
	tmp = readl(mmio + HI_SATA_PHY1_CTLH);
	tmp |= HI_SATA_DIS_CLK;
	writel(tmp, (mmio + HI_SATA_PHY1_CTLH));
	
	if (HOST_MODE) {/* 1.5G or 3.0G */
		tmp = 0x8a0ec888;
		phy_config = PHY_CONFIG_3_0_G;
	} else {
		tmp = 0x8a0ec788;
		phy_config = PHY_CONFIG_1_5_G;
	}
	
	writel(tmp, (mmio + HI_SATA_PHY0_CTLL));
	writel(0x2121, (mmio + HI_SATA_PHY0_CTLH));
	writel(tmp, (mmio + HI_SATA_PHY1_CTLL));
	writel(0x2121, (mmio + HI_SATA_PHY1_CTLH));
	writel(0x84060c15, (mmio + HI_SATA_OOB_CTL));
	
	for (i = 0; i < AHCI_MAX_PORTS; i++)
		writel(phy_config, (mmio + 0x100 + i*0x80
			+ HI_SATA_PORT_PHYCTL));

	hi_sata_phy_reset();
	msleep(20);
	hi_sata_phy_unreset();
	msleep(20);
	hi_sata_clk_unreset();
	msleep(20);

}
