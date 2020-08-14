
#include <config.h>
#include <platform.h>
#include <himciv200_reg.h>

#define MCI_SRC_CLK		(50*1000*1000)	/* 50MHz */
#define MCI_ID_CLK		(400*1000)	/* 400KHz */
#define MCI_BOOT_CLK		(20*1000*1000)	/* 20MHz */

/* mmc.h */
#define MMC_CMD_GO_IDLE_STATE		0
#define MMC_CMD_SEND_OP_COND		1
#define MMC_CMD_ALL_SEND_CID		2
#define MMC_CMD_SET_RELATIVE_ADDR	3
#define MMC_CMD_SET_DSR			4
#define MMC_CMD_SWITCH			6
#define MMC_CMD_SELECT_CARD		7
#define MMC_CMD_SEND_EXT_CSD		8
#define MMC_CMD_SEND_CSD		9
#define MMC_CMD_SEND_CID		10
#define MMC_CMD_STOP_TRANSMISSION	12
#define MMC_CMD_SEND_STATUS		13
#define MMC_CMD_SET_BLOCKLEN		16
#define MMC_CMD_READ_SINGLE_BLOCK	17
#define MMC_CMD_READ_MULTIPLE_BLOCK	18
#define MMC_CMD_WRITE_SINGLE_BLOCK	24
#define MMC_CMD_WRITE_MULTIPLE_BLOCK	25
#define MMC_CMD_APP_CMD			55
#define SD_CMD_SEND_IF_COND		8
#define SD_CMD_APP_SEND_OP_COND		41
#define SD_CMD_APP_SET_BUS_WIDTH	6

#define OCR_BUSY		0x80000000
#define OCR_HCS			0x40000000

#define MMC_VDD_32_33		0x00100000	/* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34		0x00200000	/* VDD voltage 3.3 ~ 3.4 */

#define MMC_SWITCH_MODE_WRITE_BYTE	0x03 /* Set target byte to value */

#define EXT_CSD_BUS_WIDTH	183	/* R/W */
#define EXT_CSD_HS_TIMING	185	/* R/W */

#define EXT_CSD_BUS_WIDTH_1	0	/* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4	1	/* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8	2	/* Card is in 8 bit mode */

#define ERR_TIMEOUT_UPDATE_CLK      0
#define ERR_TIMEOUT_SEND_CMD        1
#define ERR_TIMEOUT_CMD_COMP        2
#define ERR_TIMEOUT_CARD_COMP       3
#define ERR_TIMEOUT_CMD1_COMP       4
#define ERR_TIMEOUT_RXDR_INT        5
#define ERR_TIMEOUT_ACMD41_COMP     6

extern void emmc_timeout_print(int num);

static inline unsigned int himci_readl(unsigned addr)
{
	return *((volatile unsigned *) (addr));
}

static inline void himci_writel(unsigned val, unsigned addr)
{
	(*(volatile unsigned *) (addr)) = (val);
}

static inline void emmc_io_init(void)
{
}

#include "early_timer.c"
#include "himci_boot_hi3716mv410.c"

static void emmc_update_clk(void)
{
	unsigned int tmp_reg;
	unsigned int mmc_base = REG_BASE_MCI;

	tmp_reg = (START_CMD | USE_HOLD_REG | UP_CLK_ONLY);
	himci_writel(tmp_reg, mmc_base + MCI_CMD);

	timer_start();
	do {
		tmp_reg = himci_readl(mmc_base + MCI_CMD);
	} while ((tmp_reg & START_CMD)&&(timer_get_val() < DELAY_10S)); 
	timer_stop();
	
	if (tmp_reg & START_CMD)
		emmc_timeout_print(ERR_TIMEOUT_UPDATE_CLK);
}

static void emmc_send_cmd(unsigned cmd, unsigned arg, unsigned wait_busy)
{
	unsigned int tmp_reg;
	unsigned int mmc_base = REG_BASE_MCI;

	himci_writel(ALL_INT_CLR, mmc_base + MCI_RINTSTS);
	himci_writel(arg, mmc_base + MCI_CMDARG);
	himci_writel(cmd | USE_HOLD_REG | WT_PD_CPT | START_CMD, mmc_base + MCI_CMD);

	timer_start();
	do {
		tmp_reg = himci_readl(mmc_base + MCI_CMD);
	} while ((tmp_reg & START_CMD)&&(timer_get_val() < DELAY_10S)); 
	timer_stop();
	
	if (tmp_reg & START_CMD)
		emmc_timeout_print(ERR_TIMEOUT_SEND_CMD);
	
	timer_start();
	do {
		tmp_reg = himci_readl(mmc_base + MCI_RINTSTS);
	} while (((tmp_reg&CD_INT_STATUS) == 0)&&(timer_get_val() < DELAY_10S)); 
	timer_stop();
	
	if ((tmp_reg&CD_INT_STATUS) == 0)
		emmc_timeout_print(ERR_TIMEOUT_CMD_COMP);
	
	if (wait_busy) {
		timer_start();
		do {
			tmp_reg = himci_readl(mmc_base + MCI_STATUS);
		} while ((tmp_reg & DATA_BUSY)&&(timer_get_val() < DELAY_10S)); 
		timer_stop();
		
		if (tmp_reg & DATA_BUSY)
			emmc_timeout_print(ERR_TIMEOUT_CARD_COMP);
	}
}

static unsigned int emmc_init(void)
{
	unsigned int tmp_reg, wait_retry_count = 0;
	unsigned int mmc_base = REG_BASE_MCI;
	unsigned int hcs;

	emmc_io_init();

	emmc_sys_init();

	/* reset all */
	tmp_reg = himci_readl(mmc_base + MCI_CTRL);
	tmp_reg |= CTRL_RESET | FIFO_RESET | DMA_RESET;
	himci_writel(tmp_reg, mmc_base + MCI_CTRL);

	/* card reset  */
	himci_writel(0, mmc_base + MCI_RESET_N);
	early_udelay(30);

	/*
	 * card power off and power on
	 * 84ms for hi3716mv300; 55ms for hi3716c
	 * 50ms for hi3798mv100
	 */
	himci_writel(0, mmc_base + MCI_PWREN);
	early_udelay(50000);
	himci_writel(1, mmc_base + MCI_PWREN);
	early_udelay(50000);

	/* card reset cancel */
	himci_writel(1, mmc_base + MCI_RESET_N);
	early_udelay(300);

	/* clear MMC host intr */
	himci_writel(ALL_INT_CLR, mmc_base + MCI_RINTSTS);

	/* MASK MMC host intr */
	tmp_reg = himci_readl(mmc_base + MCI_INTMASK);
	tmp_reg &= ~ALL_INT_MASK;
	himci_writel(tmp_reg, mmc_base + MCI_INTMASK);

	/* disable inner DMA mode and close intr of MMC host controler */
	tmp_reg = himci_readl(mmc_base + MCI_CTRL);
	tmp_reg &= ~(INTR_EN | USE_INTERNAL_DMA);
	himci_writel(tmp_reg, mmc_base + MCI_CTRL);

	/* set timeout param */
	himci_writel(DATA_TIMEOUT | RESPONSE_TIMEOUT, mmc_base + MCI_TIMEOUT);

	/* set FIFO param */
	himci_writel(BURST_SIZE | RX_WMARK | TX_WMARK, mmc_base + MCI_FIFOTH);

	/* set data width */
	himci_writel(CARD_WIDTH_1BIT, mmc_base + MCI_CTYPE);

	/* clk update */
	himci_writel(0, mmc_base + MCI_CLKENA);
	emmc_update_clk();

	tmp_reg = MCI_SRC_CLK / (MCI_ID_CLK * 2);
	if (MCI_SRC_CLK % (MCI_ID_CLK * 2))
		tmp_reg++;
	himci_writel(tmp_reg, mmc_base + MCI_CLKDIV);
	emmc_update_clk();

	himci_writel(1, mmc_base + MCI_CLKENA);
	emmc_update_clk();

	/* Send CMD0 */
	early_udelay(5000);
	emmc_send_cmd(MMC_CMD_GO_IDLE_STATE | SEND_INIT, 0, 0);
	early_udelay(10000);

	/* Send CMD1 */
	wait_retry_count = 0;
	do {
		emmc_send_cmd(MMC_CMD_SEND_OP_COND | RESP_EXPECT,
			 OCR_HCS | MMC_VDD_32_33 | MMC_VDD_33_34, 0);
		tmp_reg = himci_readl(mmc_base + MCI_RESP0);
		early_udelay(5000);
		if (tmp_reg & OCR_BUSY)
			break;	
		wait_retry_count ++;
	} while (wait_retry_count < 2000);

	if (wait_retry_count == 2000)
		emmc_timeout_print(ERR_TIMEOUT_CMD1_COMP);

	hcs = ((tmp_reg & OCR_HCS) == OCR_HCS);

	/* Send CMD2 */
	emmc_send_cmd(MMC_CMD_ALL_SEND_CID | RESP_EXPECT | RESP_LENGTH
		 | CHECK_RESP_CRC, 0, 0);

	/* Send CMD3 */
	emmc_send_cmd(MMC_CMD_SET_RELATIVE_ADDR | RESP_EXPECT
		 | CHECK_RESP_CRC, 0, 0);

	/* Send CMD7 */
	emmc_send_cmd(MMC_CMD_SELECT_CARD | RESP_EXPECT | CHECK_RESP_CRC,
		 0, 1);

	/* Send CMD6 */
	emmc_send_cmd(MMC_CMD_SWITCH | RESP_EXPECT | CHECK_RESP_CRC,
		 (MMC_SWITCH_MODE_WRITE_BYTE << 24) |
		 (EXT_CSD_BUS_WIDTH << 16) | (EXT_CSD_BUS_WIDTH_8 << 8),
		 1);

	/* set data width */
	himci_writel(CARD_WIDTH_8BIT, mmc_base + MCI_CTYPE);

	tmp_reg = MCI_SRC_CLK / (MCI_BOOT_CLK * 2);
	if (MCI_SRC_CLK % (MCI_BOOT_CLK * 2))
		tmp_reg++;
	himci_writel(tmp_reg, mmc_base + MCI_CLKDIV);
	emmc_update_clk();

	himci_writel(1, mmc_base + MCI_CLKENA);
	emmc_update_clk();

	return hcs;
}

static void emmc_deinit(void)
{
	unsigned int mmc_base = REG_BASE_MCI;

	/* clear MMC host intr */
	himci_writel(ALL_INT_CLR, mmc_base + MCI_RINTSTS);

	/* disable clk */
	himci_writel(0, mmc_base + MCI_CLKENA);
	emmc_update_clk();
}

static void emmc_read(void *ptr, unsigned int size, unsigned int hcs)
{
	unsigned int count, val, tmp_reg = 0;
	unsigned int mmc_base = REG_BASE_MCI;
	unsigned int *buf, data;

	himci_writel(512, mmc_base + MCI_BLKSIZ);
	himci_writel((size + 511) & (~511), mmc_base + MCI_BYTCNT);

	/* Send CMD7 */
	emmc_send_cmd(MMC_CMD_SET_BLOCKLEN | RESP_EXPECT | CHECK_RESP_CRC,
		512, 0);

	/* Send CMD18 */
	if (hcs)
		emmc_send_cmd(MMC_CMD_READ_MULTIPLE_BLOCK | RESP_EXPECT
			| CHECK_RESP_CRC | DATA_EXPECT, CONFIG_MMC_BOOT_ADDR/512, 0);
	else
		emmc_send_cmd(MMC_CMD_READ_MULTIPLE_BLOCK | RESP_EXPECT
			| CHECK_RESP_CRC | DATA_EXPECT, CONFIG_MMC_BOOT_ADDR, 0);

	buf = ptr;
	size = (size+3) >> 2;
	while (size > 0) {
		/*start to read data*/
		timer_start();
		do {
			tmp_reg = himci_readl(mmc_base + MCI_RINTSTS);
			if ((tmp_reg & RXDR_INT_STATUS) || (tmp_reg & DTO_INT_STATUS))
				break;

			val = (DRTO_INT_STATUS | HTO_INT_STATUS
				 | HLE_INT_STATUS | EBE_INT_STATUS);
			if (tmp_reg & val) {
				emmc_timeout_print(val);
				timer_stop();
				return;
			}
		} while (timer_get_val() < DELAY_10S);
		timer_stop();

		if (!(tmp_reg & (RXDR_INT_STATUS|DTO_INT_STATUS)))
			emmc_timeout_print(ERR_TIMEOUT_RXDR_INT);
		
		himci_writel(tmp_reg, mmc_base + MCI_RINTSTS);

		tmp_reg = himci_readl(mmc_base + MCI_STATUS);
		count = (tmp_reg >> 17) & 0x1fff;

		if (count > size)
			count = size;

		for (; count != 0; --count) {
			data = himci_readl(mmc_base + MCI_FIFO_START);

			*buf = data;
			buf++;
			--size;
		}
	}

	/* Send CMD12 */
	emmc_send_cmd(MMC_CMD_STOP_TRANSMISSION | RESP_EXPECT | CHECK_RESP_CRC,
		 0, 1);
}

void emmc_boot_read(void *ptr, unsigned int size)
{
	unsigned int hcs = 0;

	hcs = emmc_init();

	emmc_read(ptr, size, hcs);

	emmc_deinit();
}

static unsigned int sd_init(void)
{
	unsigned int tmp_reg, wait_retry_count = 0;
	unsigned int mmc_base = REG_BASE_MCI;
	unsigned int hcs;


	emmc_io_init();

	emmc_sys_init();

	/*
	 * card power off and power on
	 * 84ms for hi3716mv300; 55ms for hi3716c
	 */
	himci_writel(0, mmc_base + MCI_PWREN);
	early_udelay(50000);
	himci_writel(1, mmc_base + MCI_PWREN);
	early_udelay(50000);

	/* reset all */
	tmp_reg = himci_readl(mmc_base + MCI_CTRL);
	tmp_reg |= CTRL_RESET | FIFO_RESET | DMA_RESET;
	himci_writel(tmp_reg, mmc_base + MCI_CTRL);

		/* clear MMC host intr */
	himci_writel(ALL_INT_CLR, mmc_base + MCI_RINTSTS);

	/* MASK MMC host intr */
	tmp_reg = himci_readl(mmc_base + MCI_INTMASK);
	tmp_reg &= ~ALL_INT_MASK;
	himci_writel(tmp_reg, mmc_base + MCI_INTMASK);

	/* disable inner DMA mode and close intr of MMC host controler */
	tmp_reg = himci_readl(mmc_base + MCI_CTRL);
	tmp_reg &= ~(INTR_EN | USE_INTERNAL_DMA);
	himci_writel(tmp_reg, mmc_base + MCI_CTRL);

	/* set timeout param */
	himci_writel(DATA_TIMEOUT | RESPONSE_TIMEOUT, mmc_base + MCI_TIMEOUT);

	/* set FIFO param */
	himci_writel(BURST_SIZE | RX_WMARK | TX_WMARK, mmc_base + MCI_FIFOTH);

	/* set data width */
	himci_writel(CARD_WIDTH_1BIT, mmc_base + MCI_CTYPE);

	/* clk update */
	himci_writel(0, mmc_base + MCI_CLKENA);
	emmc_update_clk();

	tmp_reg = MCI_SRC_CLK / (MCI_ID_CLK * 2);
	if (MCI_SRC_CLK % (MCI_ID_CLK * 2))
		tmp_reg++;
	himci_writel(tmp_reg, mmc_base + MCI_CLKDIV);
	emmc_update_clk();

	himci_writel(1, mmc_base + MCI_CLKENA);
	emmc_update_clk();

	/* Send CMD0 */
	early_udelay(5000);
	emmc_send_cmd(MMC_CMD_GO_IDLE_STATE | SEND_INIT, 0, 0);
	early_udelay(10000);

	emmc_send_cmd(SD_CMD_SEND_IF_COND| RESP_EXPECT | CHECK_RESP_CRC,
		(((MMC_VDD_32_33 | MMC_VDD_33_34) & 0xff8000) != 0) << 8 | 0xaa , 0);

	/* Send ACMD41 */
	wait_retry_count = 0;
	do {
		emmc_send_cmd(MMC_CMD_APP_CMD | RESP_EXPECT | CHECK_RESP_CRC,
		0, 0);

		emmc_send_cmd(SD_CMD_APP_SEND_OP_COND | RESP_EXPECT,
			 OCR_HCS | MMC_VDD_32_33 | MMC_VDD_33_34, 0);
		tmp_reg = himci_readl(mmc_base + MCI_RESP0);
		early_udelay(5000);
		if (tmp_reg & OCR_BUSY)
			break;
		wait_retry_count++;
	} while (wait_retry_count < 2000);

	if (wait_retry_count == 2000)
		emmc_timeout_print(ERR_TIMEOUT_ACMD41_COMP);

	hcs = ((tmp_reg & OCR_HCS) == OCR_HCS);

	/* Send CMD2 */
	emmc_send_cmd(MMC_CMD_ALL_SEND_CID | RESP_EXPECT | RESP_LENGTH
		 | CHECK_RESP_CRC, 0, 0);

	/* Send CMD3 */
	emmc_send_cmd(MMC_CMD_SET_RELATIVE_ADDR | RESP_EXPECT
		 | CHECK_RESP_CRC, 0, 0);

	tmp_reg = himci_readl(mmc_base + MCI_RESP0);
	tmp_reg &=0xffff0000;

	/* Send CMD7 */
	emmc_send_cmd(MMC_CMD_SELECT_CARD | RESP_EXPECT | CHECK_RESP_CRC,
		 tmp_reg, 1);

	/* Send ACMD6 */
	emmc_send_cmd(MMC_CMD_APP_CMD | RESP_EXPECT | CHECK_RESP_CRC, tmp_reg, 0);
	emmc_send_cmd(SD_CMD_APP_SET_BUS_WIDTH | RESP_EXPECT | CHECK_RESP_CRC, 2, 0);

	/* set data width */
	himci_writel(CARD_WIDTH_4BIT, mmc_base + MCI_CTYPE);

	tmp_reg = MCI_SRC_CLK / (MCI_BOOT_CLK * 2);
	if (MCI_SRC_CLK % (MCI_BOOT_CLK * 2))
		tmp_reg++;
	himci_writel(tmp_reg, mmc_base + MCI_CLKDIV);
	emmc_update_clk();

	himci_writel(1, mmc_base + MCI_CLKENA);
	emmc_update_clk();

	return hcs;
}

static void sd_deinit(void)
{
	unsigned int mmc_base = REG_BASE_MCI;

	emmc_deinit();

	himci_writel(0, mmc_base + MCI_PWREN);
	early_udelay(20000);
}

void sd_boot_read(void *ptr, unsigned int size)
{
	unsigned int hcs = 0;

	hcs = sd_init();

	emmc_read(ptr, size, hcs);

	sd_deinit();
}

