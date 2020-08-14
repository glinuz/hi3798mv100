/* when user specified buffer overflow, we overwrite buffer here!
* for test purpose, we want read/write more data, e.g. 8GBytes
* that is "sata read/write 0xc0000000 0 0x1000000", 0x1000000blk=8GB
* we will read/write sata HDD to/from the buffer repeatedly.
* each time we will read/write 32MBytes, because of ATA cmd's limitation.
*/

#define AHCI_HOST_BASE		(REG_BASE_SATA)	/* ahci controller base addr */
#define BUFFER_START		(MEM_BASE_DDR+0x02000000)
#define BUFFER_END		(MEM_BASE_DDR+0x12000000)

/* buffer len should be 32M aligned */
#if ((BUFFER_END - BUFFER_START) & ((32 << 20) - 1))
#error buffer len should be 32M aligned!
#endif

/* asic reg definition */
#define HI_SATA_PERI_CRG	(REG_BASE_PERI_CTRL + 0x08)
#define HI_SATA_HRST		(1 << 10)

static void hi_sata_poweron(void)
{
	unsigned int tmp_val;

	tmp_val = readl(HI_SATA_PERI_CRG);
	tmp_val |= HI_SATA_HRST;
	writel(tmp_val, HI_SATA_PERI_CRG);
}

void hi_sata_init(void __iomem * mmio)
{
	hi_sata_poweron();
	msleep(10);

	/* Config SATA clock */
	writel(0x1f, REG_BASE_PERI_CTRL + 0x20A8);
	msleep(10);
	writel(0x1, REG_BASE_PERI_CTRL + 0x20AC);
	msleep(10);

	/* Config and reset the SATA PHY, SSC enabled */
	writel(0x49000679, REG_BASE_SATA + 0xA0);
	msleep(10);
	writel(0x49000678, REG_BASE_SATA + 0xA0);
	msleep(10);

	/* Config PHY controller register 1 */
	writel(0x345cb8, REG_BASE_SATA + 0x148);
	msleep(10);
	/* Config PHY controller register 2, and reset SerDes lane */
	writel(0x00060545, REG_BASE_SATA + 0x14C);
	msleep(10);
	writel(0x00020545, REG_BASE_SATA + 0x14C);
	msleep(10);

	/* Data invert between phy and sata controller */
	writel(0x8, REG_BASE_SATA + 0xA4);
	msleep(10);
	/* Config Spin-up */
	writel(0x600000, REG_BASE_SATA + 0x118);
	msleep(10);
	writel(0x600002, REG_BASE_SATA + 0x118);
	msleep(10);
	/*
	 * Config SATA Port phy controller.
	 * To take effect for 0xF990014C,
	 * we should force controller to 1.5G mode first
	 * and then force it to 6G mode.
	 */
	writel(0xE100000, REG_BASE_SATA + 0x174);
	msleep(10);
	writel(0xE5A0000, REG_BASE_SATA + 0x174);
	msleep(10);
	writel(0xE4A0000, REG_BASE_SATA + 0x174);
	msleep(10);

	writel(0xE250000, REG_BASE_SATA + 0x174);
	msleep(10);
}

