/*
*  sata_ahci.h - Common AHCI SATA definitions and declarations
*
*  Maintained by:  Jeff Garzik <jgarzik@pobox.com>
*                  Please ALWAYS copy linux-ide@vger.kernel.org
*                  on emails.
*
*  Copyright 2004-2005 Red Hat, Inc.
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2, or (at your option)
*  any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; see the file COPYING.  If not, write to
*  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*
*
* libata documentation is available via 'make {ps|pdf}docs',
* as Documentation/DocBook/libata.*
*
* AHCI hardware documentation:
* http://www.intel.com/technology/serialata/pdf/rev1_0.pdf
* http://www.intel.com/technology/serialata/pdf/rev1_1.pdf
*
*/
#ifndef _SATA_AHCI_H_

#include <linux/compiler.h>
#include <linux/types.h>

/* Global controller registers */
#define HOST_CAP                0x00 /* host capabilities */
#define HOST_CTL                0x04 /* global host control */
#define HOST_IRQ_STAT           0x08 /* interrupt status */
#define HOST_PORTS_IMPL         0x0c /* bitmap of implemented ports */
#define HOST_VERSION            0x10 /* AHCI spec. version compliancy */

/* Registers for each SATA port */
#define PORT_LST_ADDR           0x00 /* command list DMA addr */
#define PORT_LST_ADDR_HI        0x04 /* command list DMA addr hi */
#define PORT_FIS_ADDR           0x08 /* FIS rx buf addr */
#define PORT_FIS_ADDR_HI        0x0c /* FIS rx buf addr hi */
#define PORT_IRQ_STAT           0x10 /* interrupt status */
#define PORT_IRQ_MASK           0x14 /* interrupt enable/disable mask */
#define PORT_CMD                0x18 /* port command */
#define PORT_TFDATA             0x20 /* taskfile data */
#define PORT_SIG                0x24 /* device TF signature */
#define PORT_CMD_ISSUE          0x38 /* command issue */
#define PORT_SCR                0x28 /* SATA phy register block */
#define PORT_SCR_STAT           0x28 /* SATA phy register: SStatus */
#define PORT_SCR_CTL            0x2c /* SATA phy register: SControl */
#define PORT_SCR_ERR            0x30 /* SATA phy register: SError */
#define PORT_SCR_ACT            0x34 /* SATA phy register: SActive */

/* PORT_CMD bits */
#define PORT_CMD_ATAPI          (1 << 24) /* Device is ATAPI */
#define PORT_CMD_LIST_ON        (1 << 15) /* cmd list DMA engine running */
#define PORT_CMD_FIS_ON         (1 << 14) /* FIS DMA engine running */
#define PORT_CMD_FIS_RX         (1 << 4) /* Enable FIS receive DMA engine */
#define PORT_CMD_CLO            (1 << 3) /* Command list override */
#define PORT_CMD_POWER_ON       (1 << 2) /* Power up device */
#define PORT_CMD_SPIN_UP        (1 << 1) /* Spin up device */
#define PORT_CMD_START          (1 << 0) /* Enable port DMA engine */

#define PORT_CMD_ICC_ACTIVE     (0x1 << 28) /* Put i/f in active state */
#define PORT_CMD_ICC_PARTIAL    (0x2 << 28) /* Put i/f in partial state */
#define PORT_CMD_ICC_SLUMBER    (0x6 << 28) /* Put i/f in slumber state */

/* PORT_IRQ_{STAT,MASK} bits */
#define PORT_IRQ_COLD_PRES      (1 << 31) /* cold presence detect */
#define PORT_IRQ_TF_ERR         (1 << 30) /* task file error */
#define PORT_IRQ_HBUS_ERR       (1 << 29) /* host bus fatal error */
#define PORT_IRQ_HBUS_DATA_ERR  (1 << 28) /* host bus data error */
#define PORT_IRQ_IF_ERR         (1 << 27) /* interface fatal error */
#define PORT_IRQ_IF_NONFATAL    (1 << 26) /* interface non-fatal error */
#define PORT_IRQ_OVERFLOW       (1 << 24) /* xfer exhausted available S/G */
#define PORT_IRQ_BAD_PMP        (1 << 23) /* incorrect port multiplier */

#define PORT_IRQ_PHYRDY         (1 << 22) /* PhyRdy changed */
#define PORT_IRQ_DEV_ILCK       (1 << 7) /* device interlock */
#define PORT_IRQ_CONNECT        (1 << 6) /* port connect change status */
#define PORT_IRQ_SG_DONE        (1 << 5) /* descriptor processed */
#define PORT_IRQ_UNK_FIS        (1 << 4) /* unknown FIS rx'd */
#define PORT_IRQ_SDB_FIS        (1 << 3) /* Set Device Bits FIS rx'd */
#define PORT_IRQ_DMAS_FIS       (1 << 2) /* DMA Setup FIS rx'd */
#define PORT_IRQ_PIOS_FIS       (1 << 1) /* PIO Setup FIS rx'd */
#define PORT_IRQ_D2H_REG_FIS    (1 << 0) /* D2H Register FIS rx'd */

/* HOST_CTL bits */
#define HOST_RESET              (1 << 0)  /* reset controller; self-clear */
#define HOST_IRQ_EN             (1 << 1)  /* global IRQ enable */
#define HOST_AHCI_EN            (1 << 31) /* AHCI enabled */

#define PORT_IRQ_FATAL          (PORT_IRQ_TF_ERR | PORT_IRQ_HBUS_ERR     \
				| PORT_IRQ_HBUS_DATA_ERR | PORT_IRQ_IF_ERR)

#define DEF_PORT_IRQ            (PORT_IRQ_FATAL | PORT_IRQ_PHYRDY        \
				| PORT_IRQ_CONNECT | PORT_IRQ_SG_DONE   \
				| PORT_IRQ_UNK_FIS | PORT_IRQ_SDB_FIS   \
				| PORT_IRQ_DMAS_FIS | PORT_IRQ_PIOS_FIS \
				| PORT_IRQ_D2H_REG_FIS)

/* hisilicon sata reg */
#define HI_SATA_PHY0_CTLL       0x54
#define HI_SATA_PHY0_CTLH       0x58
#define HI_SATA_PHY1_CTLL       0x60
#define HI_SATA_PHY1_CTLH       0x64
#define HI_SATA_DIS_CLK         (1 << 12)
#define HI_SATA_OOB_CTL         0x6c
#define HI_SATA_PORT_PHYCTL     0x74

#define HBA_PORT_DET_PRESENT	0x3 /* no device detected */
#define HBA_PORT_IPM_ACTIVE	1 /* active state */

#define SATA_SIG_ATA		0x00000101 /*  SATA drive */
#define SATA_SIG_ATAPI		0xEB140101 /*  SATAPI drive */
#define SATA_SIG_SEMB		0xC33C0101 /*  Enclosure management bridge */
#define SATA_SIG_PM		0x96690101 /*  Port multiplier */

enum _AHCI_DEV_TYPE {
	AHCI_DEV_NULL,
	AHCI_DEV_SATAPI,
	AHCI_DEV_SEMB,
	AHCI_DEV_PM,
	AHCI_DEV_SATA
};

#define READ_CMD		0
#define WRITE_CMD		1

#define AHCI_FLAGS_DMA		0x00000000
#define AHCI_FLAGS_FPDMA	0x00000001 /* First-party DMA, NCQ */

#define AHCI_MAX_PORTS		2
#define AHCI_MAX_SG		8 /* hardware max is 64K */
#define AHCI_DMA_BOUNDARY	0xffffffff
#define AHCI_MAX_CMDS		32 /* max slot */
#define AHCI_CMD_SZ		32 /* cmd slot header size */
#define AHCI_CMD_SLOT_SZ	(AHCI_MAX_CMDS * AHCI_CMD_SZ)
#define AHCI_RX_FIS_SZ		256
#define AHCI_CMD_TBL_CDB	0x40
#define AHCI_CMD_TBL_HDR_SZ	0x80
#define AHCI_CMD_TBL_SZ		(AHCI_CMD_TBL_HDR_SZ + (AHCI_MAX_SG * 16))
#define AHCI_CMD_TBL_AR_SZ	(AHCI_CMD_TBL_SZ * AHCI_MAX_CMDS)
#define AHCI_PORT_PRIV_DMA_SZ	(AHCI_CMD_SLOT_SZ + AHCI_CMD_TBL_AR_SZ +\
					AHCI_RX_FIS_SZ)
/* 32slot header + 32 tbl + rx_fis */
#define AHCI_PORT_PRIV_FBS_DMA_SZ	(AHCI_CMD_SLOT_SZ +\
				AHCI_CMD_TBL_AR_SZ + (AHCI_RX_FIS_SZ * 16))

/* 32 bytes */
struct ahci_cmd_hdr {
	u32     opts;
	u32     status;
	u32     tbl_addr;
	u32     tbl_addr_hi;
	u32     reserved[4];
};

/* 16 bytes */
struct ahci_sg {
	u32     addr;
	u32     addr_hi;
	u32     reserved;
	u32     flags_size;
};

/* 0x80 + 16 * 10 */
struct ahci_cmd_tbl {
	u8		cfis[64];
	u8		acmd[16];
	u8		res[48];/* 64+16+48 = 0x80 */
	struct ahci_sg	sg_tbl[AHCI_MAX_SG];/* 16 * 8 */
};

struct ahci_ioports {
	u32			cmd_addr;
	u32			scr_addr;
	u32			port_mmio;

	struct ahci_cmd_hdr	*cmd_slot[AHCI_MAX_CMDS];
	struct ahci_cmd_tbl	*cmd_tbl[AHCI_MAX_CMDS];
	u32			 rx_fis;

	/* cmd_slot, cmd_tbl, rx_fis have already initialized */
	int			init_finished:1;

	/* the host we belong to */
	struct ahci_host	*host;

	/* device attribute */
	int			lba48;
	int			queue_depth;    /* Max NCQ queue depth */
	u16			pio;
	u16			mwdma;
	u16			udma;
	int			wcache;
	int			flush;
	int			flush_ext;
};

struct ahci_host {
	void __iomem		*mmio;		/* bus-independent mem map */
	u32			flags;		/* AHCI_HFLAG_* */
	u32                     cap;		/* cap to use */
	u32                     cap2;		/* cap2 to use */
	u32                     em_loc;		/* enclosure management loc */
	u32                     em_buf_sz;	/* EM buffer size in byte */
	u32                     em_msg_type;	/* EM message type */
	u32                     port_map;	/* host implment port */
	u32			link_port_map;	/* port who has phy linked! */

	int			nr_ports;	/* how many ports host has */
	struct ahci_ioports	*ports[0];
};

extern block_dev_desc_t sata_dev_desc[CONFIG_SYS_SATA_MAX_DEVICE];

#define trace_ahci(fmt, args...)	/* printf(fmt, ##args) */

#define _1ms			1000ull /* 1000us */

#define writel_with_flush(a, b) do { writel(a, b); readl(b); } while (0)
#define msleep(a)		udelay(a * 1000)
#define ssleep(a)		msleep(a * 1000)

#endif /* end of _SATA_AHCI_H_ */
