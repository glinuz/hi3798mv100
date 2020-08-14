/******************************************************************************
 * init boot reg
 *****************************************************************************/
#include <config.h>
#include <aux_common.h>
#include <platform.h>
#include "mmc_reg.h"
#include "dma.h"
/*****************************************************************************/

/*
 * Seperate the emmc clock to different path to keep in mind 
 * that the emmc clock maybe different for FPGA and ASIC
 */
#ifndef CONFIG_FPGA
#define MMC_SRC_CLK                  (25*1000*1000)       /* 25M */
#else
#define MMC_SRC_CLK                  (25*1000*1000)       /* 25M */
#endif
#define MMC_ID_CLK                   (400 * 1000)         /* 400K */

#define MMC_SRST_REQ                 (0x01 << 4)
#define MMC_CKEN                     (0x01 << 1)
#define MMC_BUS_CKEN                 (0x01 << 0)

#define MMC_CLK_MASK                 (0x07 << 8)
#define MMC_CLK_MODE_MASK            (0x01 << 19)
#define MMC_CLK_25M                  (0x02 << 8)

#define MMC_DRV_PS_MASK              (0x7 << 16)
#define MMC_SAP_PS_MASK              (0x7 << 12)
#define MMC_DRV_PS_180_90            (0b100 << 16)
#define MMC_SAP_PS_0_0               (0b000 << 12)


#ifndef  CONFIG_EDA
#define MMC_WAIT_PWR_TIME             20          /*3004:change to 40ms 3006:change to 20ms */
#else
#define MMC_WAIT_PWR_TIME             1           /* 1ms */
#endif

#define DELAY_3S                      (1000*3)    /* 3s */
#define MMC_WAIT_BDS_TIME             1000        /* 1S */

#define ENABLE                        1
#define DISABLE                       0

#define mmc_reg_get(offset)           readl(REG_MMC_BASE + (offset))
#define mmc_reg_set(offset,val)       writel(val, REG_MMC_BASE + (offset))

#define MMC_FUNC(func) if ((func) != 0) return -1
/*****************************************************************************/

static int mmc_poweron(void)
{
	int i, ret = -1;
	unsigned int wait_time = MMC_WAIT_PWR_TIME * timer_get_divider();

	mmc_reg_set(MMC_PWREN, 0); 

	/* reset all */
	mmc_reg_set(MMC_CTRL, 7);

	/* wait */
	timer_start();
	while( timer_get_val() < wait_time )
		;

	for (i = 0; i < 0xf00; ++i) {
		if ((mmc_reg_get(MMC_CTRL) & 7) == 0) {
			ret = 0;
			break;
		}
	}

	/* power on */
	mmc_reg_set(MMC_PWREN, 1);

	/* wait */
	timer_start();
	while (timer_get_val() < wait_time) 
		;

	return ret;
}
/*****************************************************************************/

int init_emmc(void)
{
	int ret = 0;
	int boot_mode;
	unsigned int regval;

	boot_mode = get_boot_mode();
	if (boot_mode != BOOT_MODE_EMMC)
		return 0;

	regval = mmc_reg_get(MMC_CMD);
	regval |= USE_HOLD_REG;
	mmc_reg_set(MMC_CMD, (1 << 31) | (1 << 26) | regval);

	/* add function: support rst */
	udelay(200);
	mmc_reg_set(MMC_RSTN, 0);
	/* For eMMC, minimum is 1us but give it 10us for good measure */
	udelay(100);
	mmc_reg_set(MMC_RSTN, 1);
	/* For eMMC, minimum is 200us but give it 300us for good measure */
	udelay(1000);

	mmc_reg_set(MMC_CTRL, 7);

	/* power down */
	mmc_reg_set(MMC_PWREN, 0);

	/* open clock, clear reset */
	regval = readl(REG_BASE_CRG + REG_PERI_CRG40);
	regval &= ~(MMC_CLK_MASK|MMC_CLK_MODE_MASK|MMC_DRV_PS_MASK);
	regval |= MMC_CLK_25M;
	regval |= (0x4 << 16);
	writel(regval, REG_BASE_CRG + REG_PERI_CRG40);

	regval |= (MMC_CKEN|MMC_BUS_CKEN);
	writel(regval, REG_BASE_CRG + REG_PERI_CRG40);

	regval &= ~(MMC_SRST_REQ);
	writel(regval, REG_BASE_CRG + REG_PERI_CRG40);

	ret = mmc_poweron();
	
	regval = mmc_reg_get(MMC_CMD);
	regval |= USE_HOLD_REG;
	mmc_reg_set(MMC_CMD,regval);

	return ret;
}

/******************************************************************************/

static int himci_set_sclk(unsigned int crg)
{
	unsigned int regval = 0;

	regval = readl(CRG_MMC_CTRL);
	regval &= ~(MMC_CLK_MASK | MMC_CLK_MODE_MASK);
	regval |= (crg&(MMC_CLK_MASK | MMC_CLK_MODE_MASK));
	writel(regval, CRG_MMC_CTRL);

	return 0;
}
/******************************************************************************/

static void mmc_init_controller(void)
{
	unsigned int val;

	mmc_reg_set(MMC_INTMASK, 0x0);
	mmc_reg_set(MMC_RINTSTS, 0xFFFFFFFF);
	mmc_reg_set(MMC_IDSTS, 0xFFFFFFFF);

	mmc_reg_set(MMC_CTRL, (1<<4));

	mmc_reg_set(MMC_CARDTHRCTL, THRESHOLD);
    
	/* set max timeout param */
	mmc_reg_set(MMC_TMOUT, DATA_TIMEOUT | RESPONSE_TIMEOUT);

	/* set FIFO param with rx th 0x100/2-1 */
	val = mmc_reg_get(MMC_FIFOTH);
	val = (val & ~(0x7FF<<16)) | (0x7f << 16);
	mmc_reg_set(MMC_FIFOTH, val);

	/* set 1 bit data width */
	mmc_reg_set(MMC_CTYPE, CARD_WIDTH_1BIT);

	/* set use_hold_reg by chip */
	mmc_reg_set(MMC_CMD, USE_HOLD_REG);
}
/******************************************************************************/

static int mmc_update_clk(void)
{
	unsigned int val, i, j;

	for(i = 0; i < 3; ++i)
	{
		val = mmc_reg_get(MMC_CMD);
		val &= USE_HOLD_REG;
		mmc_reg_set(MMC_CMD, (1<<31) | (1<<21) | (1<<13) | val); /* 0x80202000*/

		for(j = 0; j < 0xf00; ++j) {
			val = mmc_reg_get(MMC_CMD);
			if ((val & (1<<31)) == 0)
				return 0;

			val = mmc_reg_get(MMC_RINTSTS);
			if ((val & (1<<12)) != 0)
				break;
		}
	}

    return -1;
}
/******************************************************************************/

static int himci_control_clk(unsigned int flag)
{
	unsigned int regval = 0;

	regval = mmc_reg_get(MMC_CLKENA);
	if (flag == ENABLE)
		regval |= CCLK_ENABLE;
	else
		regval &= ~CCLK_ENABLE;
	mmc_reg_set(MMC_CLKENA, regval);

	MMC_FUNC( mmc_update_clk() );

	return 0;
}
/******************************************************************************/

static  int himci_set_clkdiv(int srcclk, int clk)
{
	unsigned int regval = 0;

	if (clk < srcclk) {
		regval = srcclk / (clk * 2);
		if (srcclk % (clk * 2))
			regval++;
		if (regval > 0xFF)
			regval = 0xFF;
	}
	mmc_reg_set(MMC_CLKDIV, regval);
	mmc_reg_set(MMC_CLKSRC, 0);
	MMC_FUNC( mmc_update_clk() );

	return 0;
}
/******************************************************************************/

static int mmc_hwreset(void)
{
	udelay(200);
	mmc_reg_set(MMC_RSTN, 0);
	/* For eMMC, minimum is 1us but give it 10us for good measure */
	udelay(100);
	mmc_reg_set(MMC_RSTN, 1);
	/* For eMMC, minimum is 200us but give it 300us for good measure */
	udelay(1000);

	return 0;
}
/******************************************************************************/

void mmc_deinit(void)
{
	unsigned int regval;

	regval = mmc_reg_get(MMC_CMD);
	regval &= USE_HOLD_REG;
	mmc_reg_set(MMC_CMD, (1<<31) | (1<<26) | regval);

	/* add function: support rst */
	mmc_hwreset();

	mmc_reg_set(MMC_CTRL, 7);

	/* power down */
	mmc_reg_set(MMC_PWREN, 0);
}
/******************************************************************************/

unsigned int mmc_boot_read(void *ptr, unsigned int size)
{
	unsigned int val;
	unsigned int *tmp = (unsigned int*) ptr;
	unsigned int count;
	int ret = AUTH_FAILURE;
	dma_cfg_t st_dma_cfg;
	unsigned int wait_time = DELAY_3S*timer_get_divider();

	size = (size + 3) >> 2;
    
	while (size > 0) {
		timer_start();
		do {
			val = mmc_reg_get(MMC_RINTSTS);
			mmc_reg_set(MMC_RINTSTS, val);

			if( val & MMC_INT_DATA_FIFO_ERROR ) {
				return -1;    /* error! return */
			}
			if ( ( val & MMC_INT_RXDR ) || ( val & MMC_INT_DTO ) ) {
				break;
			}
		} while (timer_get_val() < wait_time);

		if ((val&(MMC_INT_RXDR | MMC_INT_DTO)) == 0)
			serial_puts("eMMC/SD: read timeout.\n");

		count = ( (mmc_reg_get(MMC_STATUS) >> 17) & 0x1fff );

		if (count > size)
			count = size;

		if (count > 0) {
			st_dma_cfg.burst_size = 0;  /* 1burst */
			st_dma_cfg.src_inc = 0;
			st_dma_cfg.dest_inc = 1;
			ret = read_with_dma(tmp, (void *)(REG_MMC_BASE + MMC_FIFO_START),
						count * 4, &st_dma_cfg);
			if (AUTH_SUCCESS == ret) {
				size = size - count;
				tmp = tmp + count;
			} else {
				return -1;
			}
		}

		/* all finished */
		if( val & MMC_INT_DTO )
			break;
	}
	return (unsigned int)(tmp - (unsigned int*)ptr);
}

/******************************************************************************/

static void send_cmd(unsigned cmd, unsigned arg, unsigned wait_busy)
{
	unsigned int tmp_reg;
	unsigned int wait_time = DELAY_3S*timer_get_divider();

	mmc_reg_set(MMC_RINTSTS, ALL_INT_CLR);
	mmc_reg_set(MMC_CMDARG, arg);

	tmp_reg = mmc_reg_get(MMC_CMD);
	tmp_reg &= USE_HOLD_REG;
	mmc_reg_set(MMC_CMD, cmd | WT_PD_CPT | START_CMD | tmp_reg);

	/* wait cmd send ok */
	timer_start();
	do {
		tmp_reg = mmc_reg_get(MMC_CMD);
	} while ((tmp_reg & START_CMD)&&(timer_get_val() < wait_time)); 

	if (tmp_reg & START_CMD)
		 serial_puts("eMMC: wait cmd send timeout.\n");

	/* wait cmd complete ok */
	timer_start();
	do {
		tmp_reg = mmc_reg_get(MMC_RINTSTS);
	} while ((!(tmp_reg & MMC_INT_CD))&&(timer_get_val() < wait_time));

	if ((tmp_reg&MMC_INT_CD) == 0)
		serial_puts("eMMC: wait cmd complete timeout.\n");

	/* wait card ok */
	if (wait_busy) {
		timer_start();
		do {
			tmp_reg = mmc_reg_get(MMC_STATUS);
		} while ((tmp_reg & DATA_BUSY)&&(timer_get_val() < wait_time)); 
		if (tmp_reg & DATA_BUSY)
			serial_puts("eMMC: wait card complete timeout.\n");
	}
}
/******************************************************************************/

static unsigned int read_step(void *ptr, unsigned int size, unsigned int offset, unsigned int hcs)
{
	unsigned int readbytes;

	/* set host block size 512 */
	mmc_reg_set(MMC_BLKSIZ, 512);
	/* set byte count for 512*n */
	mmc_reg_set(MMC_BYTCNT,(size + 511) & (~511));

	/* Send CMD16 to set blocksize*/
	send_cmd(MMC_CMD_SET_BLOCKLEN | RESP_EXPECT | CHECK_RESP_CRC,
		512, 0);

	/* Send CMD18 for multiple block read*/
	if (hcs)
    {
		send_cmd(MMC_CMD_READ_MULTIPLE_BLOCK | RESP_EXPECT
			| CHECK_RESP_CRC | DATA_EXPECT, offset/512, 0);
	} else {
		send_cmd(MMC_CMD_READ_MULTIPLE_BLOCK | RESP_EXPECT
			| CHECK_RESP_CRC | DATA_EXPECT, offset, 0);
	}

	readbytes = mmc_boot_read(ptr, size);

	/* Send CMD12 to stop transfer*/
	send_cmd(MMC_CMD_STOP_TRANSMISSION | RESP_EXPECT | CHECK_RESP_CRC, 0, 1);

	return readbytes;
}
/******************************************************************************/

static int mmc_cmds(unsigned int *hcs)
{
	unsigned int tmp_reg=0;
	unsigned int wait_retry_count = 0;

	/* Send CMD0 to reset card into idle state */
	udelay(1000);
	send_cmd(MMC_CMD_GO_IDLE_STATE | SEND_INIT, 0, 0);
	udelay(2000);

	/* Send CMD1  initialization and wait to complete*/
	do {
		send_cmd(MMC_CMD_SEND_OP_COND | RESP_EXPECT, 
			OCR_HCS | MMC_VDD_32_33 | MMC_VDD_33_34, 0);
		tmp_reg = mmc_reg_get(MMC_RESP0);
		udelay(5000);
		if (tmp_reg & OCR_BUSY)
			break;	
		wait_retry_count ++;
	} while (wait_retry_count < 600);

	/* card capacity */
	*hcs = ((tmp_reg & OCR_HCS) == OCR_HCS);

	/* Send CMD2 to get card cid numbers*/
	send_cmd(MMC_CMD_ALL_SEND_CID | RESP_EXPECT | RESP_LENGTH
		| CHECK_RESP_CRC, 0, 0);

	/* Send CMD3  to get card new address with default address(0000) */
	send_cmd(MMC_CMD_SET_RELATIVE_ADDR | RESP_EXPECT
		| CHECK_RESP_CRC, 0, 0);

	/* Send CMD7 for card from standby state to transfer state*/
	send_cmd(MMC_CMD_SELECT_CARD | RESP_EXPECT | CHECK_RESP_CRC,
		0, 1);

	/* Send CMD6 */
	send_cmd(MMC_CMD_SWITCH | RESP_EXPECT | CHECK_RESP_CRC,
		(MMC_SWITCH_MODE_WRITE_BYTE << 24) |
		(EXT_CSD_BUS_WIDTH << 16) | (EXT_CSD_BUS_WIDTH_8 << 8),
		1);

	/* set data width */
	mmc_reg_set(MMC_CTYPE, CARD_WIDTH_8BIT);

	return 0;
}
/******************************************************************************/

static unsigned int  mmc_sys_init(void)
{
	unsigned int value = 0;
	unsigned int crg = 0;

	/* get srcclk */
	crg = value = readl(CRG_MMC_CTRL);

	/* reset */
	value |= MMC_SRST_REQ;
	writel(value, CRG_MMC_CTRL);
	udelay(1000);

	value &=~(MMC_CLK_MASK | MMC_CLK_MODE_MASK);
	value |= MMC_CLK_25M;
	writel(value, CRG_MMC_CTRL);

	/* open clock/clear reset */
	value &= ~MMC_SRST_REQ;
	value |= MMC_CKEN | MMC_BUS_CKEN;
	writel(value, CRG_MMC_CTRL);

	return crg;
}
/******************************************************************************/

static int mmc_init(unsigned int *hcs)
{
	unsigned int crg;

	crg = mmc_sys_init();

	MMC_FUNC(mmc_poweron());

	mmc_init_controller();

	/* 400k */
	MMC_FUNC(himci_control_clk(DISABLE));
	MMC_FUNC(himci_set_clkdiv(MMC_SRC_CLK, MMC_ID_CLK));
	MMC_FUNC(himci_control_clk(ENABLE));
	
	MMC_FUNC( mmc_hwreset() );
	MMC_FUNC( mmc_cmds(hcs));

	/*25M: source clk and interface clk from excel */
	MMC_FUNC(himci_control_clk(DISABLE));
	MMC_FUNC(himci_set_sclk(crg));
	MMC_FUNC(himci_set_clkdiv(1, 1));
	MMC_FUNC(himci_control_clk(ENABLE));

	return 0;
}
/******************************************************************************/

unsigned int mmc_read(void *ptr, unsigned int size, unsigned int offset)
{
	unsigned int hcs;
	unsigned int regval;

	regval = readl(REG_BASE_SCTL + REG_SC_GEN5);
	mmc_deinit();
	mmc_init(&hcs);
	writel(regval | 0x1, REG_BASE_SCTL + REG_SC_GEN5);

	/* read_step function is the normal mode read function of emmc/sd */
	return read_step(ptr, size, offset, hcs);
}
