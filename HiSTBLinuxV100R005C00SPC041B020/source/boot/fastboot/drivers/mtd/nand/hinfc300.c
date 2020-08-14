/******************************************************************************
*	 NAND Flash	Controller V300	Device Driver
*	 Copyright (c) 2009-2010 by	Hisilicon.
*	 All rights	reserved.
******************************************************************************/

#include <common.h>
#include <nand.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <malloc.h>
#include <linux/mtd/nand.h>


#include "hinfc300.h"

/*****************************************************************************/

extern void nand_spl_ids_register(void);
extern int (*nand_base_oob_resize)(struct mtd_info *mtd, 
	struct nand_chip *chip, char *args);
unsigned int nand_ip_version = 0x0300;
static struct hinfc_host *host = NULL;

/*****************************************************************************/

static char	*get_pagesize_str(enum page_type pagetype)
{
	static char	*pagesize_str[]	= {"512", "2K",	"4K", "8K",	"unknown" };
	return pagesize_str[pagetype];
}
/*****************************************************************************/

static unsigned	int	get_pagesize(enum page_type	pagetype)
{
	unsigned int pagesize[]	= {_512B, _2K, _4K,	_8K, 0};
	return pagesize[pagetype];
}
/*****************************************************************************/

static void	hinfc300_dma_transfer(struct hinfc_host	*host, int todev)
{
	unsigned long reg_val;
	hinfc_write(host, host->dma_buffer,	HINFC300_DMA_ADDR_DATA);
	hinfc_write(host, host->dma_oob, HINFC300_DMA_ADDR_OOB);

	if (host->ecctype == hinfc504_ecc_none)
	{
		hinfc_write(host, 
			((host->oobsize	& HINFC300_DMA_LEN_OOB_MASK) <<	HINFC300_DMA_LEN_OOB_SHIFT),
			HINFC300_DMA_LEN);

		hinfc_write(host,
			HINFC300_DMA_PARA_DATA_RW_EN
			| HINFC300_DMA_PARA_OOB_RW_EN
			| ((host->n24bit_ext_len & HINFC300_DMA_PARA_EXT_LEN_MASK) << HINFC300_DMA_PARA_EXT_LEN_SHIFT),
			HINFC300_DMA_PARA);
	}
	else
	{
		hinfc_write(host,
			HINFC300_DMA_PARA_DATA_RW_EN
			| HINFC300_DMA_PARA_OOB_RW_EN
			| HINFC300_DMA_PARA_DATA_EDC_EN
			| HINFC300_DMA_PARA_OOB_EDC_EN
			| HINFC300_DMA_PARA_DATA_ECC_EN
			| HINFC300_DMA_PARA_OOB_ECC_EN
			| ((host->n24bit_ext_len & HINFC300_DMA_PARA_EXT_LEN_MASK) << HINFC300_DMA_PARA_EXT_LEN_SHIFT),
			HINFC300_DMA_PARA);
	}
	
	reg_val	= (HINFC300_DMA_CTRL_DMA_START 
		| HINFC300_DMA_CTRL_BURST4_EN
		| HINFC300_DMA_CTRL_BURST8_EN
		| HINFC300_DMA_CTRL_BURST16_EN
		| ((host->addr_cycle ==	4 ?	1 :	0) << HINFC300_DMA_CTRL_ADDR_NUM_SHIFT)
		| ((host->chipselect & HINFC300_DMA_CTRL_CS_MASK) << HINFC300_DMA_CTRL_CS_SHIFT));

	if (todev)
	{
		reg_val	|= HINFC300_DMA_CTRL_WE;
	}
	else
	{
		reg_val	|= (0x03 <<	2);	// XXX fix mpw chip	bug!
	}

	hinfc_write(host, reg_val, HINFC300_DMA_CTRL);

	while ((hinfc_read(host, HINFC300_DMA_CTRL)) & HINFC300_DMA_CTRL_DMA_START)
		;
}
/*****************************************************************************/

static void	hinfc300_cmd_ctrl(struct mtd_info *mtd,	int	dat, unsigned int ctrl)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host	= chip->priv;

	if (ctrl & NAND_ALE)
	{
		unsigned int addr_value	= 0;
		unsigned int addr_offset = 0;

		if (ctrl & NAND_CTRL_CHANGE)
		{
			host->addr_cycle = 0x0;
			host->addr_value[0]	= 0x0;
			host->addr_value[1]	= 0x0;
		}
		addr_offset	=  host->addr_cycle	<< 3;

		if (host->addr_cycle >=	HINFC300_ADDR_CYCLE_MASK)
		{
			addr_offset	= (host->addr_cycle	- HINFC300_ADDR_CYCLE_MASK)	<< 3;
			addr_value = 1;
		}

		host->addr_value[addr_value] |=	((dat &	0xff) << addr_offset);

		host->addr_cycle ++;
	}

	if ((ctrl &	NAND_CLE) && (ctrl & NAND_CTRL_CHANGE))
	{
		host->command =	dat	& 0xff;
		switch (host->command)
		{
		case NAND_CMD_PAGEPROG:

			hinfc_write(host, host->NFC_CON, HINFC300_CON);
			hinfc_write(host, host->addr_value[0] &	0xffff0000,	HINFC300_ADDRL);
			hinfc_write(host, host->addr_value[1], HINFC300_ADDRH);
			hinfc_write(host, NAND_CMD_PAGEPROG	<< 8 | NAND_CMD_SEQIN, HINFC300_CMD);

			hinfc300_dma_transfer(host,	1);

			break;

		case NAND_CMD_READSTART: 

			hinfc_write(host, HINFC300_INTCLR_UE, HINFC300_INTCLR);
			hinfc_write(host, host->NFC_CON, HINFC300_CON);
			hinfc_write(host, host->addr_value[0] &	0xffff0000,	HINFC300_ADDRL);
			hinfc_write(host, host->addr_value[1], HINFC300_ADDRH);
			hinfc_write(host, NAND_CMD_READSTART <<	8 |	NAND_CMD_READ0,	HINFC300_CMD);

			hinfc300_dma_transfer(host,	0);

			host->uc_er	= ((hinfc_read(host, HINFC300_INTS)	& HINFC300_INTS_UE)	? 1	: 0);


			break;

		case NAND_CMD_ERASE2:
			hinfc_write(host, host->NFC_CON_ECC_NONE, HINFC300_CON);
			hinfc_write(host, host->addr_value[0], HINFC300_ADDRL);
			hinfc_write(host, (NAND_CMD_ERASE2 << 8) | NAND_CMD_ERASE1,	HINFC300_CMD);

			hinfc_write(host,
				HINFC300_OP_WAIT_READY_EN
				| HINFC300_OP_CMD2_EN
				| HINFC300_OP_CMD1_EN
				| HINFC300_OP_ADDR_EN
				| ((host->chipselect & HINFC300_OP_NF_CS_MASK) << HINFC300_OP_NF_CS_SHIFT)
				| ((host->addr_cycle & HINFC300_OP_ADDR_CYCLE_MASK)	<< HINFC300_OP_ADDR_CYCLE_SHIFT),
				HINFC300_OP);

			while ((hinfc_read(host, HINFC300_STATUS) &	0x1) ==	0x0)
				;
			break;

		case NAND_CMD_READID:

			memset((unsigned char *)(chip->IO_ADDR_R), 0, 0x10);

			hinfc_write(host, host->NFC_CON_ECC_NONE, HINFC300_CON);
			hinfc_write(host, NAND_CMD_READID, HINFC300_CMD);
			hinfc_write(host, 0, HINFC300_ADDRL);

			hinfc_write(host,  
				HINFC300_OP_CMD1_EN
				| HINFC300_OP_ADDR_EN
				| HINFC300_OP_READ_DATA_EN
				| HINFC300_OP_WAIT_READY_EN
				| ((host->chipselect & HINFC300_OP_NF_CS_MASK) << HINFC300_OP_NF_CS_SHIFT)
				| (1 <<	HINFC300_OP_ADDR_CYCLE_SHIFT),
				HINFC300_OP);

			host->addr_cycle = 0x0;

			while ((hinfc_read(host, HINFC300_STATUS) &	0x1) ==	0x0)
				;

			break;

		case NAND_CMD_STATUS:

			hinfc_write(host, host->NFC_CON_ECC_NONE, HINFC300_CON);
			hinfc_write(host, NAND_CMD_STATUS, HINFC300_CMD);
			hinfc_write(host,
				HINFC300_OP_CMD1_EN
				| HINFC300_OP_READ_DATA_EN
				| HINFC300_OP_WAIT_READY_EN
				| ((host->chipselect & HINFC300_OP_NF_CS_MASK) << HINFC300_OP_NF_CS_SHIFT),
				HINFC300_OP);

			while ((hinfc_read(host, HINFC300_STATUS) &	0x1) ==	0x0)
				;

			break;

		case NAND_CMD_SEQIN:
		case NAND_CMD_ERASE1:
		case NAND_CMD_READ0:
			break;
		case NAND_CMD_RESET:

			hinfc_write(host, NAND_CMD_RESET, HINFC300_CMD);

			hinfc_write(host,
				(HINFC300_OP_CMD1_EN
				| ((host->chipselect & HINFC300_OP_NF_CS_MASK) << HINFC300_OP_NF_CS_SHIFT)
				| HINFC300_OP_WAIT_READY_EN),
				HINFC300_OP);

			while ((hinfc_read(host, HINFC300_STATUS) &	0x1) ==	0x0)
				;

			break;

		default	:
			break;
		}
	}
	/*
	 *  pass pagesize and ecctype to kernel when startup.
	 */
	hinfc_write(host, host->NFC_CON, HINFC300_CON);

	if ((dat ==	NAND_CMD_NONE) && host->addr_cycle)
	{
		if (host->command == NAND_CMD_SEQIN	
			|| host->command ==	NAND_CMD_READ0 
			|| host->command ==	NAND_CMD_READID)
		{
			host->offset = 0x0;
			host->column = (host->addr_value[0]	& 0xffff);
		}
	}
}
/*****************************************************************************/

static int hinfc300_dev_ready(struct mtd_info *mtd)
{
	mtd = mtd;
	return 0x1;
}
/*****************************************************************************/

static void	hinfc300_select_chip(struct	mtd_info *mtd, int chipselect)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host	= chip->priv;

	if (chipselect < 0)
		return;

	if (chipselect > 2)
	{
		DBG_BUG("invalid chipselect: %d\n",	chipselect);
	}

	host->chipselect = chipselect;
}
/*****************************************************************************/

static uint8_t hinfc300_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host	= chip->priv;

	if (host->command == NAND_CMD_STATUS)
	{
		return readb(chip->IO_ADDR_R);
	}

	host->offset++;

	if (host->command == NAND_CMD_READID)
	{
		return readb(chip->IO_ADDR_R + host->offset	- 1);
	}

	return readb(host->buffer +	host->column + host->offset	- 1);
}
/*****************************************************************************/

static u16 hinfc300_read_word(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host	= chip->priv;

	host->offset +=	2;

	return readw(host->buffer +	host->column + host->offset	- 2);
}
/*****************************************************************************/

static void	hinfc300_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host	= chip->priv;

	memcpy(host->buffer	+ host->column + host->offset, buf,	len);
	host->offset +=	len;
}
/*****************************************************************************/

static void	hinfc300_read_buf(struct mtd_info *mtd,	uint8_t	*buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host	= chip->priv;

	memcpy(buf,	host->buffer + host->column	+ host->offset,	len);
	host->offset +=	len;
}
/*****************************************************************************/

static struct nand_ecclayout nand_ecc_default =	
{
	.oobfree = {{2,	30}}
};

static struct nand_ecclayout nand_ecc_2K_1bit =
{
	.oobfree = {{22, 30}}
};
/*****************************************************************************/

static struct page_page_ecc_info page_page_ecc_info[] =
{
	{hinfc504_pagesize_8K, hinfc504_ecc_24bit1k, 368, &nand_ecc_default},
	{hinfc504_pagesize_8K, hinfc504_ecc_none,	 32,  &nand_ecc_default},

	{hinfc504_pagesize_4K, hinfc504_ecc_24bit1k, 200, &nand_ecc_default},
	{hinfc504_pagesize_4K, hinfc504_ecc_4bytes,	 128, &nand_ecc_default},
	{hinfc504_pagesize_4K, hinfc504_ecc_1bit,	 128, &nand_ecc_default},
	{hinfc504_pagesize_4K, hinfc504_ecc_none,	 32,  &nand_ecc_default},

	{hinfc504_pagesize_2K, hinfc504_ecc_1bit,	 64,  &nand_ecc_2K_1bit},
	{hinfc504_pagesize_2K, hinfc504_ecc_none,	 32,  &nand_ecc_default},

	{0},
};
/*****************************************************************************/
/* 
* used the best	correct	arithmetic.
*/
struct page_page_ecc_info *hinfc300_get_best_ecc(struct	mtd_info *mtd)
{
	struct page_page_ecc_info *best	= NULL;
	struct page_page_ecc_info *info	= page_page_ecc_info;

	for	(; info->layout; info++)
	{
		if (get_pagesize(info->pagetype) !=	mtd->writesize)
			continue;

		if (mtd->oobsize < info->oobsize)
			continue;

		if (!best || (best->ecctype	< info->ecctype))
		{
			best = info;
		}
	}

	if (!best)
	{
		DBG_BUG("not support this pagesize(%d) and oobsize(%d).\n",
			mtd->writesize,	mtd->oobsize);
	}

	return best;
}
/*****************************************************************************/
/* force the pagesize and ecctype */
struct page_page_ecc_info *hinfc300_force_ecc
(
	struct mtd_info	*mtd, 
	enum page_type pagetype, 
	enum hinfc504_ecc_reg ecctype, 
	char *cfgmsg,
	int	allow_pagediv
)
{
	int	pagesize;
	struct page_page_ecc_info *fit = NULL;
	struct page_page_ecc_info *info	= page_page_ecc_info;

	for	(; info->layout; info++)
	{
		if (info->pagetype == pagetype
			&& info->ecctype ==	ecctype)
		{
			fit	= info;
			break;
		}
	}

	if (!fit)
	{
		DBG_BUG("system	not	support	this nand %s: pagesize:%s, ecctype:%s\n",
			cfgmsg,	
			get_pagesize_str(pagetype),
			get_ecctype_str(ecctype));
		return NULL;
	}

	pagesize = get_pagesize(pagetype);
	if ((pagesize != mtd->writesize)
		&& (pagesize > mtd->writesize || !allow_pagediv))
	{
		DBG_BUG("%s	is inconsistent, config	pagesize %d, "
			"but the nand chip pageszie	is %d\n",
			cfgmsg,
			pagesize,
			mtd->writesize);
		return NULL;
	}

	if (fit->oobsize > mtd->oobsize)
	{
		DBG_BUG("%s	is inconsistent, config	%s require oobsize(%d),	"
			"but the nand chip oobsize is %d, littler than require.\n",
			cfgmsg,	get_ecctype_str(ecctype),
			fit->oobsize,
			mtd->oobsize);
		return NULL;
	}

	return fit;
}
/*****************************************************************************/

static int hinfc300_ecc_probe(struct mtd_info *mtd, struct nand_chip *chip, char *args)
{
	char *start_type = "unknown";
	struct page_page_ecc_info *best	= NULL;
	struct hinfc_host *host	= chip->priv;

#ifdef CONFIG_HINFC300_AUTO_PAGESIZE_ECC
	best = hinfc300_get_best_ecc(mtd);
	start_type = "Auto";
#endif /* CONFIG_HINFC300_AUTO_PAGESIZE_ECC	*/

#ifdef CONFIG_HINFC300_HARDWARE_PAGESIZE_ECC
	best = hinfc300_force_ecc(mtd,
		((host->NFC_CON	>> HINFC300_CON_PAGEISZE_SHIFT)	& HINFC300_CON_PAGESIZE_MASK),
		((host->NFC_CON	>> HINFC300_CON_ECCTYPE_SHIFT) & HINFC300_CON_ECCTYPE_MASK),
		"hardware config", 0);
	start_type = "Hardware";
#endif /* CONFIG_HINFC300_HARDWARE_PAGESIZE_ECC	*/

#ifdef CONFIG_HINFC300_PAGESIZE_AUTO_ECC_NONE
	{
		int	pagetype;

		switch (mtd->writesize)
		{
		case _512B:	pagetype = hinfc504_pagesize_512;	break;
		case _2K:	pagetype = hinfc504_pagesize_2K;	break;
		case _4K:	pagetype = hinfc504_pagesize_4K;	break;
		case _8K:	pagetype = hinfc504_pagesize_8K;	break;
		default:	pagetype = hinfc504_pagesize_2K;	break;
		}
		best = hinfc300_force_ecc(mtd, pagetype, hinfc504_ecc_none, "force config", 0);
		start_type = "AutoForce";
	}
#endif /* CONFIG_HINFC300_PAGESIZE_AUTO_ECC_NONE */

	if (!best)
	{
		DBG_BUG("Please	select nand	flash pagesize and ecc type!\n");
	}

	if (best->ecctype != hinfc504_ecc_none)
		mtd->oobsize = best->oobsize;
	chip->ecc.layout = best->layout;

	host->ecctype  = best->ecctype;
	host->pagesize = get_pagesize(best->pagetype);
	host->oobsize  = mtd->oobsize;
	*(int *)args = host->ecctype;

	if (host->ecctype == hinfc504_ecc_24bit1k)
	{
		if (host->pagesize == _4K)
		{
			host->n24bit_ext_len = 0x03; //	8bytes;
		}
		else if	(host->pagesize	== _8K)
		{
			host->n24bit_ext_len = 0x01; //	4bytes;
		}
	}
	host->dma_oob =	host->dma_buffer + host->pagesize;

	host->NFC_CON  = (HINFC300_CON_OP_MODE_NORMAL
		| ((best->pagetype & HINFC300_CON_PAGESIZE_MASK) <<	HINFC300_CON_PAGEISZE_SHIFT)
		| HINFC300_CON_READY_BUSY_SEL
		| ((best->ecctype &	HINFC300_CON_ECCTYPE_MASK) << HINFC300_CON_ECCTYPE_SHIFT));

	host->NFC_CON_ECC_NONE	= (HINFC300_CON_OP_MODE_NORMAL
		| ((best->pagetype & HINFC300_CON_PAGESIZE_MASK) <<	HINFC300_CON_PAGEISZE_SHIFT)
		| HINFC300_CON_READY_BUSY_SEL);

	if (mtd->writesize > NAND_MAX_PAGESIZE 
		|| mtd->oobsize	> NAND_MAX_OOBSIZE)
	{
		DBG_BUG("kernel	not	support	this NAND. "
			"please	increase NAND_MAX_PAGESIZE and NAND_MAX_OOBSIZE.\n");
	}

	if (mtd->writesize != host->pagesize)
	{
		unsigned int shift = 0;
		unsigned int writesize = mtd->writesize;
		while (writesize > host->pagesize)
		{
			writesize >>= 1;
			shift++;
		}
		chip->chipsize = chip->chipsize	>> shift;
		mtd->erasesize = mtd->erasesize	>> shift;
		mtd->writesize = host->pagesize;
		printk("Nand divide	into 1/%u\n", (1 <<	shift));
	}


	printk("Nand(%s): ", start_type);
	return 0;
}
/*****************************************************************************/

static int hinfc300_nand_init(struct hinfc_host	*host, struct nand_chip	*chip)
{
	chip->priv		  =	host;
	chip->cmd_ctrl	  =	hinfc300_cmd_ctrl;
	chip->dev_ready	  =	hinfc300_dev_ready;
	chip->select_chip =	hinfc300_select_chip;
	chip->read_byte	  =	hinfc300_read_byte;
	chip->read_word	  =	hinfc300_read_word;
	chip->write_buf	  =	hinfc300_write_buf;
	chip->read_buf	  =	hinfc300_read_buf;

	chip->chip_delay = HINFC300_CHIP_DELAY;
	chip->options	 = NAND_NO_AUTOINCR	|  NAND_SKIP_BBTSCAN;

	chip->ecc.layout = NULL;
	chip->ecc.mode	 = NAND_ECC_NONE;

	host->chip			= chip;
	host->addr_cycle	= 0;
	host->addr_value[0]	= 0;
	host->addr_value[1]	= 0;
	host->chipselect	= 0;

	host->NFC_CON =	(hinfc_read(host, HINFC300_CON)	
		| HINFC300_CON_OP_MODE_NORMAL |	HINFC300_CON_READY_BUSY_SEL);
	host->NFC_CON_ECC_NONE = (host->NFC_CON	
		& (~(HINFC300_CON_ECCTYPE_MASK << HINFC300_CON_ECCTYPE_SHIFT)));

	host->buffer = kmalloc((NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE),
		GFP_KERNEL);
	if (!host->buffer)
		return 1;
    host->dma_buffer = (unsigned int)host->buffer; 
    host->dma_oob = host->dma_buffer + host->pagesize;
    memset((char *)chip->IO_ADDR_R, 0xff, HINFC300_BUFFER_BASE_ADDRESS_LEN);
	memset(host->buffer, 0xff, (NAND_MAX_PAGESIZE +	NAND_MAX_OOBSIZE));

	hinfc_write(host, SET_HINFC300_PWIDTH(5, 5,	3),	HINFC300_PWIDTH);

	nand_base_oob_resize = hinfc300_ecc_probe;

	return 0;
}
/*****************************************************************************/

static void	hinfc300_controller_enable(struct hinfc_host *host,	int	enable)
{
	unsigned int reg_val = readl(host->sysreg +	PERI_CRG30);
	if (enable)
	{
		reg_val	|= (PERI_CRG30_CLK_EN |	PERI_CRG30_CLK_SEL_99M);
	}
	else
	{
		reg_val	&= ~PERI_CRG30_CLK_EN;
	}
	writel(reg_val,	(host->sysreg +	PERI_CRG30));
}
/*****************************************************************************/
int board_nand_init(struct nand_chip* chip) 
{
    int result = 0;
	if (host)
		return -1;
    int size = sizeof(struct hinfc_host); 

    host = kmalloc(size, GFP_KERNEL);
    if (!host)
    {
        printf("failed to allocate device structure.\n");
        return -1;
    }
    memset((char *)host, 0, size);

    host->chip = chip;
    chip->priv = host;
    host->iobase = (void*)CONFIG_HINFC300_REG_BASE_ADDRESS; 
    chip->IO_ADDR_R = chip->IO_ADDR_W = (void*)CONFIG_HINFC300_BUFFER_BASE_ADDRESS;
    host->sysreg = (void*)CONFIG_HINFC300_PERIPHERY_REGBASE;

    hinfc300_controller_enable(host, 1);

    if (hinfc300_nand_init(host, chip))
    {
        printf( "failed to allocate device buffer.\n");
        result = -1;
    }
	nand_spl_ids_register();
   return result;
}

/*****************************************************************************/

int nand_get_ecctype(void)
{
    if (!host)
    {
        printf("nand flash uninitialized.\n");
        return -1;
    }
    return host->ecctype;
}
