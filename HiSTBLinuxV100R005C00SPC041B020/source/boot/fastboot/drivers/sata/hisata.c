/*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston,
* MA 02111-1307 USA
*
* with the reference on libata and ahci drvier in kernel
*
*/
#include <common.h>
#include <command.h>
#include <pci.h>
#include <asm/processor.h>
#include <linux/err.h>
#include <asm/io.h>
#include <malloc.h>
#include <libata.h>
#include <linux/ctype.h>
#include <fis.h>
#include <div64.h>
#include "hisata.h"

/*
* data: 20120330
* This driver only support these devices:
* a. LBA48
* b. device's ncq-depth = 32
* c. READ_EXT/WRITE_EXT cmd for devices which not support ncq
* d. FPDMA_READ/FPDMA_WRITE cmd for ncq
*/

#define DEV_TO_PORT(dev)	((struct ahci_ioports *)sata_dev_desc[dev].priv)
#define DEV_TO_HOST(dev)	(DEV_TO_PORT(dev)->host)
#define CMD_HDR(port, slot)	(DEV_TO_PORT(port)->cmd_slot[slot])
#define CMD_TBL(port, slot)	(DEV_TO_PORT(port)->cmd_tbl[slot])
#define SG(port, slot, num)	(&(CMD_TBL(port, slot)->sg_tbl[num]))

static u16 ataid[256];

/* 0 -- 1.5G, 1 -- 3.0G */
#define HOST_MODE		1

#ifdef CONFIG_SATA_S40
#include "ahci_s40.c"
#endif

#ifdef CONFIG_SATA_GODBOX
#include "ahci_godbox.c"
#endif


static inline u32 get_port_base(u32 host_base, u32 port)
{
	return host_base + 0x100 + (port * 0x80);
}

static void set_port_base(struct ahci_ioports *port, unsigned long host_base,
			    unsigned int port_idx)
{
	u32 base = get_port_base(host_base, port_idx);

	port->port_mmio = base;
	port->cmd_addr = base;
	port->scr_addr = base + PORT_SCR;
}

static int waiting_for_cmd_completed(volatile u8 *offset,
				     u64 timeout_usec,
				     u32 sign)
{
	int i;
	u32 status;

	for (i = 0; ((status = readl(offset)) & sign) && i < timeout_usec; i++)
		udelay(1);

	return (i < timeout_usec) ? 0 : -1;
}

/*
 * reset and enable ahci controller.
 * if ahci_host is NULL, we malloc the host and the port;
 * else we reuse the ahci_host and the port.
 */
static struct ahci_host *ahci_host_init(void __iomem *host_base, u32 ahci_host)
{
	volatile u8 *mmio = (volatile u8 *)host_base;
	u32 tmp, cap, nr_ports, port_map, sz;
	int i, j;
	volatile u8 *port_mmio;
	struct ahci_host *host = (struct ahci_host *)ahci_host;

	cap = readl(mmio + HOST_CAP);

	/* global controller reset */
	tmp = readl(mmio + HOST_CTL);
	if ((tmp & HOST_RESET) == 0)
		writel_with_flush(tmp | HOST_RESET, mmio + HOST_CTL);

	/* reset must complete within 1 second, or
	 * the hardware should be considered fried.
	 */
	ssleep(1);

	tmp = readl(mmio + HOST_CTL);
	if (tmp & HOST_RESET) {
		printf("controller reset failed (0x%x)\n", tmp);
		return NULL;
	}

	writel_with_flush(HOST_AHCI_EN, mmio + HOST_CTL);

	port_map = readl(mmio + HOST_PORTS_IMPL);
	nr_ports = (cap & 0x1f) + 1; /* CAP.NP */

	/* CAP.NP sometimes indicate the index of the last enabled
	 * port, at other times, that of the last possible port, so
	 * determining the maximum port number requires looking at
	 * both CAP.NP and port_map.
	 */
	nr_ports = max(nr_ports, fls(port_map));

	if (!ahci_host) {
		sz = sizeof(struct ahci_host) + nr_ports * sizeof(void *);

		host = (struct ahci_host *)malloc(sz);
		if (host == NULL) {
			printf("No mem for ahci_host, failed!\n");
			return NULL;
		}

		trace_ahci("malloc: host: [0x%p, 0x%x)\n",
				host, (u32)host + sz);

		memset((void *)host, 0, sz);
	}

	host->mmio = (void __iomem *)mmio;
	host->nr_ports = nr_ports;
	host->port_map = port_map;
	host->cap = readl(mmio + HOST_CAP);
	host->flags = (host->cap & (1 << 30)) ?
		AHCI_FLAGS_FPDMA : AHCI_FLAGS_DMA;
	host->link_port_map = 0;

	for (i = 0; i < host->nr_ports; i++) {
		if (!ahci_host) {
			struct ahci_ioports *pp;

			sz = sizeof(struct ahci_ioports);

			pp = (struct ahci_ioports *)malloc(sz);
			if (pp == NULL) {
				printf("No mem for port[%d]!\n", i);

				free((void *)host);
				while (--i > 0)
					free(host->ports[i]);

				return NULL;
			}
			trace_ahci("malloc: ports[%d]: [0x%p, 0x%x)\n",
					i, pp, (u32)pp + sz);
			memset((void *)pp, 0, sz);

			pp->host = host;

			/* set ports[i] base */
			set_port_base(pp, (unsigned long)mmio, i);

			host->ports[i] = pp;
		}

		port_mmio = (volatile u8 *)get_port_base((u32)mmio, i);

		/* make sure port is not active */
		tmp = readl(port_mmio + PORT_CMD);
		if (tmp & (PORT_CMD_LIST_ON | PORT_CMD_FIS_ON |
			   PORT_CMD_FIS_RX | PORT_CMD_START)) {
			tmp &= ~(PORT_CMD_LIST_ON | PORT_CMD_FIS_ON |
				 PORT_CMD_FIS_RX | PORT_CMD_START);
			writel_with_flush(tmp, port_mmio + PORT_CMD);

			/* spec says 500 msecs for each bit, so
			 * this is slightly incorrect.
			 */
			msleep(500);
		}

		writel(PORT_CMD_SPIN_UP, port_mmio + PORT_CMD);

		j = 0;
		while (j < 100) {
			msleep(10);
			tmp = readl(port_mmio + PORT_SCR_STAT);
			/* device ready & commucation is ok */
			if ((tmp & 0xf) == 0x3)
				break;
			j++;
		}

		/* clear errors */
		tmp = readl(port_mmio + PORT_SCR_ERR);
		writel(tmp, port_mmio + PORT_SCR_ERR);

		/* ack any pending irq events for this port */
		tmp = readl(port_mmio + PORT_IRQ_STAT);
		if (tmp)
			writel(tmp, port_mmio + PORT_IRQ_STAT);/* clear irqs */

		writel(1 << i, mmio + HOST_IRQ_STAT);/* clear port_n irq */

		/* set irq mask (enables interrupts) */
		writel(DEF_PORT_IRQ, port_mmio + PORT_IRQ_MASK);

		/*register linkup ports */
		tmp = readl(port_mmio + PORT_SCR_STAT);
		if ((tmp & 0xf) == 0x03)/* goood port */
			host->link_port_map |= (0x01 << i);
		trace_ahci("port phy status: PORT_SCR_STAT = 0x%x\n", tmp);
	}

	tmp = readl(mmio + HOST_CTL);
	writel(tmp | HOST_IRQ_EN, mmio + HOST_CTL);/* host irq enable */
	tmp = readl(mmio + HOST_CTL);

	return host;
}

static void ahci_print_info(struct ahci_host *host)
{
	volatile u8 *mmio = (volatile u8 *)host->mmio;
	u32 vers, cap, impl, speed;
	const char *speed_s;

	vers = readl(mmio + HOST_VERSION);
	cap = host->cap;
	impl = host->port_map;

	speed = (cap >> 20) & 0xf;
	if (speed == 1)
		speed_s = "1.5";
	else if (speed == 2)
		speed_s = "3";
	else
		speed_s = "?";

	trace_ahci("AHCI %02x%02x.%02x%02x "
	       "%u slots %u ports %s Gbps 0x%x impl\n",
	       (vers >> 24) & 0xff,
	       (vers >> 16) & 0xff,
	       (vers >> 8) & 0xff,
	       vers & 0xff,
	       ((cap >> 8) & 0x1f) + 1, (cap & 0x1f) + 1, speed_s, impl);

	trace_ahci("flags: "
	       "%s%s%s%s%s%s"
	       "%s%s%s%s%s%s%s\n",
	       cap & (1 << 31) ? "64bit " : "",
	       cap & (1 << 30) ? "ncq " : "",
	       cap & (1 << 28) ? "ilck " : "",
	       cap & (1 << 27) ? "stag " : "",
	       cap & (1 << 26) ? "pm " : "",
	       cap & (1 << 25) ? "led " : "",
	       cap & (1 << 24) ? "clo " : "",
	       cap & (1 << 19) ? "nz " : "",
	       cap & (1 << 18) ? "only " : "",
	       cap & (1 << 17) ? "pmp " : "",
	       cap & (1 << 15) ? "pio " : "",
	       cap & (1 << 14) ? "slum " : "",
	       cap & (1 << 13) ? "part " : "");
}

#define MAX_DATA_u8_COUNT  (4*1024*1024)
static int ahci_fill_sg(u8 port, u8 slot, const void *buf, int buf_len)
{
	struct ahci_sg *ahci_sg = SG(port, slot, 0);
	u32 sg_count;
	int i;

	sg_count = ((buf_len - 1) / MAX_DATA_u8_COUNT) + 1;
	if (sg_count > AHCI_MAX_SG) {
		trace_ahci("Error:Too much sg!\n");
		return -1;
	}

	for (i = 0; i < sg_count; i++) {
		ahci_sg->addr =
		    cpu_to_le32((u32) buf + i * MAX_DATA_u8_COUNT);
		ahci_sg->addr_hi = 0;
		ahci_sg->flags_size = cpu_to_le32(0x3fffff &
					  (buf_len < MAX_DATA_u8_COUNT
					   ? (buf_len - 1)
					   : (MAX_DATA_u8_COUNT - 1)));
		ahci_sg++;
		buf_len -= MAX_DATA_u8_COUNT;
	}

	return sg_count;
}

static void ahci_fill_cmd_slot(struct ahci_ioports *pp, u8 slot, u32 opts)
{
	pp->cmd_slot[slot]->opts = cpu_to_le32(opts);
	pp->cmd_slot[slot]->status = 0;
	pp->cmd_slot[slot]->tbl_addr = cpu_to_le32((u32)pp->cmd_tbl[slot]
			& 0xffffffff);
	pp->cmd_slot[slot]->tbl_addr_hi = 0;
}

static void ahci_set_feature(u8 port)
{
	struct ahci_ioports *pp = DEV_TO_PORT(port);
	volatile u8 *port_mmio = (volatile u8 *)pp->port_mmio;
	u32 cmd_fis_len = 5;	/* five dwords */
	u8 fis[20];

	/*set feature */
	memset(fis, 0, 20);
	fis[0] = 0x27;
	fis[1] = 1 << 7;/* set C, means command */
	fis[2] = ATA_CMD_SET_FEATURES;
	fis[3] = SETFEATURES_XFER;

	/* 01000 -- 110 = 0x40 + 7 - 1 */
	fis[12] = fls(pp->udma) + 0x40 - 0x01;

	memcpy((void *)pp->cmd_tbl[0], fis, 20);/* copy fis to cmd_tbl->cfis */
	ahci_fill_cmd_slot(pp, 0, cmd_fis_len);
	writel_with_flush(1, port_mmio + PORT_CMD_ISSUE);

	if (waiting_for_cmd_completed(port_mmio + PORT_CMD_ISSUE,
				5000 * _1ms, 0x1)) {
		printf("set feature timeout!\n");
	}
}

static int ahci_port_start(struct ahci_host *host, u8 port)
{
	struct ahci_ioports *pp = host->ports[port];
	volatile u8 *port_mmio = (volatile u8 *)pp->port_mmio;
	u32 port_status;
	u32 mem;
	u32 align = 0x800;
	int i;

	port_status = readl(port_mmio + PORT_SCR_STAT);
	if ((port_status & 0xf) != 0x03) {
		printf("No Link on this port: %d!\n", port);
		return -1;
	}

	if (!pp->init_finished) {
		align = 0x800;	/* align to 2K, I think align to 1K is fine! */
		mem = (u32) malloc(AHCI_PORT_PRIV_DMA_SZ + align);
		if (!mem) {
			free(pp);
			printf("No mem for table!\n");
			return -ENOMEM;
		}
		/* Aligned to 2048-bytes */
		mem = (mem + align - 1) & (~(align - 1));
		trace_ahci("buffer: mem=0x%x, size=0x%x ",
				mem, AHCI_PORT_PRIV_DMA_SZ);
		/* slot_header(32 * 32) + tbl(0x80 + 16 * sg_nums)*32 */
		memset((u8 *) mem, 0, AHCI_PORT_PRIV_DMA_SZ);
		/*
		 * ---------------------- align to 0x800 <= cmd_slot
		 *  slot header=32B * 32
		 * --------------------- align to 0xff <= rx_fis
		 *  rx_fis=0x100
		 * --------------------- <--table <= cmd_tbl
		 *  cfis
		 *  acmd
		 *  ...
		 *  -------------------- <--PRDT <= cmd_tbl_sg
		 *  8 items(each item=16B)
		 * ---------------------
		 */
		/*
		 * First item in chunk of DMA memory: 32-slot command table,
		 * 32 bytes each in size
		 */
		for (i = 0; i < AHCI_MAX_CMDS; i++) {
			pp->cmd_slot[i] = (struct ahci_cmd_hdr *)
				((u8 *)mem + i * AHCI_CMD_SZ);
		}
		trace_ahci("cmd_slot=0x%x ", (u32)pp->cmd_slot[0]);
		mem += AHCI_CMD_SLOT_SZ;/* align=0xFF(fis base require) */

		/*
		 * Second item: Received-FIS area, must aligned in 0xFF
		 */
		pp->rx_fis = mem;
		mem += AHCI_RX_FIS_SZ;

		/*
		 * Third item: data area for storing a single command
		 * and its scatter-gather table
		 */
		for (i = 0; i < AHCI_MAX_CMDS; i++)
			pp->cmd_tbl[i] = (struct ahci_cmd_tbl *)
				((u8 *)mem + i * AHCI_CMD_TBL_SZ);

		trace_ahci("cmd_tbl=0x%x\n", (u32)pp->cmd_tbl[0]);

		pp->init_finished = 1;
	}
	/* cmd list addr */
	writel_with_flush((u32)pp->cmd_slot[0], port_mmio + PORT_LST_ADDR);
	/* rx_fis buffer addr */
	writel_with_flush(pp->rx_fis, port_mmio + PORT_FIS_ADDR);
	/* start port */
	writel_with_flush(PORT_CMD_ICC_ACTIVE | PORT_CMD_FIS_RX |
			  PORT_CMD_POWER_ON | PORT_CMD_SPIN_UP |
			  PORT_CMD_START, port_mmio + PORT_CMD);

	return 0;
}

/*
 * port	-- to select sata port register
 * fis		-- cfis
 * fis_len	-- sizeof(fis)
 * buf		-- data buffer/sg_buffer
 * buf_len	-- data len(in Bytes)
 */
static int get_ahci_device_data(u8 port, u8 *fis, int fis_len, const void *buf,
				u64 buf_len)
{
	struct ahci_ioports *pp = DEV_TO_PORT(port);
	volatile u8 *port_mmio = (volatile u8 *)pp->port_mmio;
	u32 opts;
	u32 status;
	int sg_count;

	if (port > pp->host->nr_ports) {
		printf("Invaild port number %d\n", port);
		return -1;
	}

	status = readl(port_mmio + PORT_SCR_STAT);
	if ((status & 0xf) != 0x03) {
		printf("No Link on port %d!\n", port);
		return -1;
	}

	/* check BSY state */
	status = waiting_for_cmd_completed(port_mmio + PORT_SCR_STAT,
			300 * _1ms, ATA_BUSY);
	if (status & ATA_BUSY) {
		printf("BSY = 0 check. timeout.\n");
		return -1;
	}

	memcpy((void *)pp->cmd_tbl[0]->cfis, fis, fis_len);

	sg_count = ahci_fill_sg(port, 0, buf, buf_len);
	opts = (fis_len >> 2) | (sg_count << 16);
	ahci_fill_cmd_slot(pp, 0, opts);

	writel_with_flush(1, port_mmio + PORT_CMD_ISSUE);

	if (waiting_for_cmd_completed(port_mmio + PORT_CMD_ISSUE,
				5000 * _1ms, 0x1)) {
		trace_ahci("\n wait cmd complete timeout, exit!\n");
		return -1;
	}

	return 0;
}

/* Check device type */
static int check_type(struct ahci_ioports *port)
{
	u32 ssts = readl(port->port_mmio + PORT_SCR_STAT);
	u32 sig  = readl(port->port_mmio + PORT_SIG);

	u8 ipm = (ssts >> 8) & 0x0F;
	u8 detect = ssts & 0x0F;

	/* Check drive status */
	if (detect != HBA_PORT_DET_PRESENT)
		return AHCI_DEV_NULL;
	if (ipm != HBA_PORT_IPM_ACTIVE)
		return AHCI_DEV_NULL;

	switch (sig) {
	case SATA_SIG_ATAPI:
		return AHCI_DEV_SATAPI;
	case SATA_SIG_SEMB:
		return AHCI_DEV_SEMB;
	case SATA_SIG_PM:
		return AHCI_DEV_PM;
	default:
		return AHCI_DEV_SATA;
	}
}

static void detect_port_dev_type(struct ahci_ioports *ahci_port, int port)
{
	int dev_type = check_type(ahci_port);

	switch (dev_type) {
	case AHCI_DEV_SATA:
		trace_ahci("SATA drive found at port %d\n", port);
		break;
	case AHCI_DEV_SATAPI:
		trace_ahci("SATAPI drive found at port %d\n", port);
		break;
	case AHCI_DEV_SEMB:
		trace_ahci("SEMB drive found at port %d\n", port);
		break;
	case AHCI_DEV_PM:
		trace_ahci("PM drive found at port %d\n", port);
		break;
	default:
		trace_ahci("No drive found at port %d\n", port);
	}
}
/*
 * each time we call "sata init", command layer will memset
 * sata_dev_desc[dev] to 0!
 * so we need a global pointer to store the host.
 * ahci_host only initialize once.
 */
static struct ahci_host *ahci_host;

int init_sata(int dev)
{
	struct ahci_host *host = NULL;
	u32 linkmap;
	int i;

	static u32 init_cnt;

	/*
	 * if a host have n ports, cmd_sata.c will
	 * call init_sata(i) n times, for us one time
	 * is enough!
	 */
	if (init_cnt++ % CONFIG_SYS_SATA_MAX_DEVICE)
		return 0;

	/* power on, select clk, and config phy */
	hi_sata_init((void __iomem *)AHCI_HOST_BASE);

	/* initialize host controller */
	host = ahci_host_init((void __iomem *)AHCI_HOST_BASE, (u32)ahci_host);
	if (host == NULL)
		return -1;

	ahci_host = host;

	ahci_print_info(host);

	linkmap = host->link_port_map;
	if (!linkmap)
		printf("phy not link on %d ports!\n", host->nr_ports);
	else
		for (i = 0; i < AHCI_MAX_PORTS; i++) {
			if (((linkmap >> i) & 0x01)) {
				/* start port */
				if (ahci_port_start(host, (u8)i)) {
					printf("Can not start port %d\n", i);
					continue;
				}
				detect_port_dev_type(host->ports[i], i);
			}
		}

	return 0;
}

static int read_ata_id(u16 *id, u32 port)
{
	u8 fis[20];

	memset(fis, 0, 20);
	/* Construct the FIS */
	fis[0] = 0x27;		/* Host to device FIS. */
	fis[1] = 1 << 7;	/* Command FIS. */
	fis[2] = ATA_CMD_ID_ATA;/* Command byte. */

	/* Read id from sata */
	if (get_ahci_device_data(port, (u8 *)&fis, 20,
				 (u8 *)id, 256*2)) {
		trace_ahci("ahci: read id faild!.\n");
		return -EIO;
	}

	return 0;
}

static void sata_xfer_mode(int dev, u16 *id)
{
	struct ahci_ioports *pp = DEV_TO_PORT(dev);

	pp->pio = id[ATA_ID_PIO_MODES];
	pp->mwdma = id[ATA_ID_MWDMA_MODES];
	pp->udma = id[ATA_ID_UDMA_MODES] & 0xff;
	if (!pp->udma) {
		pp->udma = 0x3f;
		trace_ahci("weird, read id is empty!\n");
	}

	/* Get the NCQ queue depth from device */
	pp->queue_depth = ata_id_queue_depth(id);

}

static void sata_init_wcache(int dev, u16 *id)
{
	struct ahci_ioports *pp = DEV_TO_PORT(dev);

	if (ata_id_has_wcache(id) && ata_id_wcache_enabled(id))
		pp->wcache = 1;
	if (ata_id_has_flush(id))
		pp->flush = 1;
	if (ata_id_has_flush_ext(id))
		pp->flush_ext = 1;
}

int scan_sata(int dev)
{
	struct ahci_host *host = ahci_host;
	unsigned char serial[ATA_ID_SERNO_LEN + 1];
	unsigned char firmware[ATA_ID_FW_REV_LEN + 1];
	unsigned char product[ATA_ID_PROD_LEN + 1];
	u16 *id = ataid;
	u64 n_sectors;
	u32 status;

	/*
	 * Save the private struct to block device struct,
	 * from now on we can use macro DEV_TO_PORT/HOST.
	 */
	if (host)
		sata_dev_desc[dev].priv = (void *)host->ports[dev];
	else
		return 0;

	/* linked ready? */
	if (!(host->link_port_map & (1 << dev)))
		return 0;

	printf("\n-----------------------------------------------\n");
	memset(id, 0, sizeof(ataid));
	read_ata_id(id, dev);

	/* Serial number */
	ata_id_c_string(id, serial, ATA_ID_SERNO, sizeof(serial));
	memcpy(sata_dev_desc[dev].product, serial, sizeof(serial));

	/* Firmware version */
	ata_id_c_string(id, firmware, ATA_ID_FW_REV, sizeof(firmware));
	memcpy(sata_dev_desc[dev].revision, firmware, sizeof(firmware));

	/* Product model */
	ata_id_c_string(id, product, ATA_ID_PROD, sizeof(product));
	memcpy(sata_dev_desc[dev].vendor, product, sizeof(product));

	/* Totoal sectors */
	n_sectors = ata_id_n_sectors(id);
	if (n_sectors > 0xFFFFFFFFull) {
		printf("n_sectors=%llx, truncate to 2TB\n", n_sectors);
		n_sectors = 0xFFFFFFFFull;
	}
	sata_dev_desc[dev].lba = (u32)n_sectors;

	/* Check if support LBA48 */
	if (ata_id_has_lba48(id))
		sata_dev_desc[dev].lba48 = 1;

	trace_ahci("Device %ssupport LBA48\n\r",
			sata_dev_desc[dev].lba48 ? "" : "not ");

	/* Get the xfer mode from device */
	sata_xfer_mode(dev, id);

	/* Get the write cache status from device */
	sata_init_wcache(dev, id);

	/* Set the xfer mode to highest speed */
	ahci_set_feature(dev);

	/* we change the setting, so read it again! */
	read_ata_id(id, dev);
	ata_dump_id(id);

	/* our driver simplely only support ncq queue_depth = 32 */
	trace_ahci("port(%d)->queue_depth = %d\n",
			dev, DEV_TO_PORT(dev)->queue_depth);
	/* BUG_ON(queue_depth != 32); */

	status = readl(DEV_TO_PORT(dev)->port_mmio + PORT_SCR_STAT);
	if ((status & 0xf) == 0x3) {
		char *spd_str;
		switch ((status >> 4) & 0xf) {
		case 1:
			spd_str = "1.5 Gps";
			break;
		case 2:
			spd_str = "3.0 Gps";
			break;
		case 3:
			spd_str = "6.0 Gps";
			break;
		default:
			spd_str = "no dev/communication";
			break;
		}
		printf("SATA link up %s\n", spd_str);
	}

	return 0;
}

/*
 * one slot can transfer 65536 blk data(32M)
 * fis.sector = 0 means we transfer 65536 blk
 * 1--65535 <--> blkcnt[1--65535]
 * 65536    <--> blkcnt[0]
 */
static int fill_one_slot(int port, int slot, lbaint_t blknr, ulong blkcnt,
		const void *buffer, int is_write, int is_ncq)
{
	sata_fis_h2d_t fis;
	u32 sg_count, opts;

	trace_ahci("\nport=%d, slot=%d, blknr=0x%x, blkcnt=%u, buffer=%p",
			port, slot, (u32)blknr, (u32)blkcnt, buffer);

	/* ATA cmd blkcnt limit is 0x10000 = 65536blkcnt */
	if ((!blkcnt) || (blkcnt > 0x10000)) {
		printf("fis: blkcnt is invalid(0 < blkcnt <= 0x%x)!\n",
				0x10000);
		return -1;
	}

	memset(&fis, 0, sizeof(fis));

	/* Construct the FIS */
	fis.fis_type = SATA_FIS_TYPE_REGISTER_H2D;
	fis.pm_port_c = 1 << 7; /* is command */

	if (is_ncq) {
		fis.command = is_write ?
			ATA_CMD_FPDMA_WRITE : ATA_CMD_FPDMA_READ;

		fis.features = (u8)blkcnt;
		fis.features_exp = (u8)(blkcnt >> 8);

		fis.sector_count = (u8)slot << 3;
	} else {
		fis.command = is_write ? ATA_CMD_WRITE_EXT : ATA_CMD_READ_EXT;

		/* Sector Count */
		fis.sector_count = (u8)blkcnt;
		fis.sector_count_exp = (u8)(blkcnt >> 8);
	}

	fis.lba_low = (u8)blknr;
	fis.lba_mid = (u8)(blknr >> 8);
	fis.lba_high = (u8)(blknr >> 16);
	fis.lba_low_exp = (u8)(blknr >> 24);
	fis.lba_mid_exp = 0;
	fis.lba_high_exp = 0;

	/* in ncq, FUA = 0 */
	fis.device = 1 << 6; /* LBA48 mode */

	/* h2d fis = 20Bytes */
	memcpy((void *)CMD_TBL(port, slot), (void *)&fis, sizeof(fis));

	sg_count = ahci_fill_sg(port, slot, buffer, blkcnt << 9);
	opts = (sizeof(fis) >> 2) | (sg_count << 16);

	/* fill cmd slot header */
	ahci_fill_cmd_slot(DEV_TO_PORT(port), slot, opts);

	return 0;
}

static int ahci_exec_cmd(u32 slot_map, u8 port, u32 timeout_ms, int is_ncq)
{
	volatile u8 *port_mmio = (volatile u8 *)DEV_TO_PORT(port)->port_mmio;
	u32 port_status;

	port_status = readl(port_mmio + PORT_SCR_STAT);
	if ((port_status & 0xf) != 0x03) {
		trace_ahci("No Link on port %d, port_status=0x%x\n!\n",
				port, port_status);
		return -1;
	}

	if (is_ncq)
		writel_with_flush(slot_map, port_mmio + PORT_SCR_ACT);

	/* start cmd slot */
	writel_with_flush(slot_map, port_mmio + PORT_CMD_ISSUE);

	/* which register we use to check if cmd was finished */
	port_status = is_ncq ? PORT_SCR_ACT : PORT_CMD_ISSUE;

	/* loop to read status */
	if (waiting_for_cmd_completed(port_mmio + port_status,
				timeout_ms * _1ms, slot_map)) {
		trace_ahci("\n wait cmd complete timeout, slot_map=0x%x!\n",
				readl(port_mmio + PORT_CMD_ISSUE));
		return -1;
	}

	return 0;
}

#define MAX_BLKCNT		(~0ul - 1)
#define MAX_BLKCNT_PER_SLOT	(0x1000)
#define BLKCNT2TIMEOUT(blkcnt)	(((blkcnt) > 1000 ?\
			((blkcnt) >> 6) : 4000))/* 4000 ms */
#define CYC_PER_MS		(CFG_HZ / 1000)
ulong sata_read_write(int dev, ulong blknr, lbaint_t blkcnt, void *buffer,
		int is_write)
{
	struct ahci_ioports *pp = DEV_TO_PORT(dev);
	struct ahci_host *host = pp->host;
	volatile u8 *port_mmio = (volatile u8 *)pp->port_mmio;
	u32 status, slot_map;
	ulong orig_blkcnt = blkcnt, start = blknr;
	int i;

	u32 rc;
	u32 cyc_last, cyc_now;
	u64 cyc_delta = 0;
	u32 ms, rate, print_rate = (blkcnt > 1) ? 1 : 0;

	if (dev > host->nr_ports) {
		printf("\nInvaild port number %d\n", dev);
		return 0;
	}

	if ((!blkcnt) || (blkcnt > MAX_BLKCNT)
			|| (blknr > sata_dev_desc[dev].lba)
			|| ((u64)blknr + blkcnt > sata_dev_desc[dev].lba)) {
		printf("\nInvalid blkcnt/blknr(out of range)!\n");
		printf("blkcnt %lu, blknr %lu, dev lba %lu \n", blkcnt, blknr, sata_dev_desc[dev].lba);
		return 0;
	}

	status = readl(port_mmio + PORT_SCR_STAT);
	if ((status & 0xf) != 0x03) {
		printf("\nNo Link on port %d!\n", dev);
		return 0;
	}

	slot_map = readl(port_mmio + PORT_CMD_ISSUE);
	if (slot_map) {
		printf("\nslot map is not empty! slot\n");
		return 0;
	}

	/* check BSY state */
	status = waiting_for_cmd_completed(port_mmio + PORT_SCR_STAT,
			30 * _1ms, ATA_BUSY);
	if (status & ATA_BUSY) {
		printf("\nBSY check. timeout.\n");
		return 0;
	}

	printf("\n");

	while (1) {
		u32 num = 0, percent_complete = -1;
		if (ctrlc()) {
			printf("\nAborted!\n");
			return 0;
		}

		if (print_rate) {
			u64 n = (u64)(blknr - start) * 100;
			u32 percent;

			do_div(n, orig_blkcnt);
			percent = (u32)n;

			if (percent != percent_complete) {
				percent_complete = percent;

				trace_ahci("\n");
				printf("\r%s at sector 0x%lx -- %3d%% "
					"complete.", is_write ?
					"Write" : "Read", blknr, percent);
			}
		}

		if (!blkcnt)
			break;

		/* step1: fill slots */
		for (i = 0, slot_map = 0; i < AHCI_MAX_CMDS; i++) {
			u32 blkcount = (blkcnt > MAX_BLKCNT_PER_SLOT)
				? MAX_BLKCNT_PER_SLOT : blkcnt;

			blkcnt -= blkcount;

			fill_one_slot(dev, i, blknr, blkcount, buffer, is_write,
					host->flags & AHCI_FLAGS_FPDMA);

			blknr += blkcount;
			buffer += blkcount << 9;

			/* buffer overflow? */
			if (((u32)buffer == 0) || ((u32)buffer >= BUFFER_END))
				buffer = (void *)BUFFER_START;

			slot_map |= 1 << i;

			num += blkcount;

			if (!blkcnt)
				break;
		}

		cyc_last = get_timer_masked();

		/* step2: start ata cmd */
		rc = ahci_exec_cmd(slot_map, dev, BLKCNT2TIMEOUT(num),
				host->flags & AHCI_FLAGS_FPDMA);

		if (!rc) {
			cyc_now = get_timer_masked();
			if (cyc_now >= cyc_last)
				cyc_delta += cyc_now - cyc_last;
			else
				print_rate = 0;/* time overflow */
		} else {
			trace_ahci("dev=%d, slot_map=%x, rc=%d\n",
					dev, slot_map, rc);
			print_rate = 0;
			return 0;
		}
	}

	if ((!blkcnt) && print_rate) {
		u32 ms_1;

		ms = cyc_delta / CYC_PER_MS;
		if (!ms)
			ms_1 = 1;/* divisor cann't be 0! */
		else
			ms_1 = ms;

		rate = ((u64)orig_blkcnt << 9) / ms_1;

		/* byte*1000/(time*1024*1024) */
		printf("\n\ntimecost: %d.%03ds, datalen: %dM\n"
			"%s rate: %d B/ms(%llu M/s)\n",
				ms_1/1000, ms_1%1000, (int)(orig_blkcnt >> 11),
				is_write ? "write" : "read",
				rate, ((u64)rate*1000)>>20);
	}

	printf("\n");

	return orig_blkcnt;
}

ulong sata_read(int dev, ulong blknr, lbaint_t blkcnt, void *buffer)
{
	return sata_read_write(dev, blknr, blkcnt, buffer, READ_CMD);
}

ulong sata_write(int dev, ulong blknr, lbaint_t blkcnt, void *buffer)
{
	return sata_read_write(dev, blknr, blkcnt, buffer, WRITE_CMD);
}
