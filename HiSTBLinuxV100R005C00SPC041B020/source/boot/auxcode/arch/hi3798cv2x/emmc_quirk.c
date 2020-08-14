#include "aux_common.h"
#include <platform.h>
#include <mmc_reg.h>
#include "emmc_quirk.h"

/* default key length */
#define DEF_KEY_LEN         0x10

#define RSA_2048_LEN                0x100

#define HEAD_AREA_LEN               0x3000
#define KEY_AREA_LEN                0x400
#define UNCHECK_AREA_HISI_LEN       0x40
#define PARA_AREA_LEN               (HEAD_AREA_LEN - KEY_AREA_LEN - UNCHECK_AREA_HISI_LEN)
#define MAX_AUX_LEN                 0x3500

#define CFG_PARAM_LEN \
    (PARA_AREA_LEN - 16 - RSA_2048_LEN - 128)    //sizeof(PARAM_AREA_HEAD_S) = 128, sizeof(PARAM_AREA_TAIL_S) = RSA_2048_LEN + 16

typedef struct
{
    /*external public key area*/
    unsigned char       extern_key[RSA_2048_LEN * 2];
    unsigned int        key_msid;
    unsigned int        key_msid_mask;
    unsigned int        key_version;
    unsigned int        key_version_mask;
    unsigned int        key_rights;
    unsigned int        aux_area_addr;
    unsigned int        aux_area_len;
    unsigned int        boot_entry_point;
    unsigned char       flash_protecting_key_1[DEF_KEY_LEN]; 	    /* For Nagra NOCS3 only */
    unsigned char       flash_protecting_key_2[DEF_KEY_LEN]; 	    /* For Nagra NOCS3 only */
    unsigned char       flash_protecting_key_3[DEF_KEY_LEN]; 	    /* For Nagra NOCS3 only */
    unsigned char       reserved_1[176];
    unsigned char       key_sig[RSA_2048_LEN];
}key_area_s;

typedef struct
{
    unsigned int        check_area_off;
    unsigned int        check_area_len;
    unsigned int        boot_area_len;
}boot_area_info_s;

typedef struct
{
    boot_area_info_s    boot_area_info;                     //PARA_AREA_OFFSET
    unsigned int        boot_enc_flag;                      /* For Nagra NOCS3 only */
    unsigned char       flash_prot_key[DEF_KEY_LEN];        /* For Nagra NOCS3 only */
    unsigned int        default_area_size;                  /* For Nagra NOCS3 only */
    unsigned char       reserved_2[92];
    unsigned char       cfg_param[CFG_PARAM_LEN];
    unsigned int        param_msid;
    unsigned int        param_msid_mask;
    unsigned int        param_version;
    unsigned int        param_version_mask;
    unsigned char       param_sig[RSA_2048_LEN];
}param_area_s;

typedef struct
{
    /* Unchecked Area for Hisilicon */
    unsigned int        reserved_0;
    unsigned int        boot_flag;
    unsigned int        aux_enc_flag;
    unsigned int        key_enc_flag;                       /* For Non-Nagra chipsets */
    unsigned char       boot_prot_key[DEF_KEY_LEN];         /* For Non-Nagra chipsets */
    unsigned int        multi_param_en;
    unsigned int        param_item_start;
    unsigned int        param_item_len;
    unsigned int        boot_store_addr;
    unsigned char       reserved_3[16];
}uncheck_area_hisi_s;

/* the mapping for the FLASH head area */
typedef struct
{
    key_area_s          key_area;
    param_area_s        param_area;
    uncheck_area_hisi_s uncheck_hisi;
}head_area_s;



void udelay(unsigned long us);
void serial_puts (const char *s);

#define debug_info(a, b) serial_puts(a)


typedef unsigned int u32;
typedef unsigned int size_t;

#undef reg_get
#define reg_get(addr) (*(volatile unsigned int *)(addr))

#undef reg_set
#define reg_set(addr, val) (*(volatile unsigned int *)(addr) = (val))

/* eMMC register */
#define REG_MMC_BASE        (0xf9830000)             /* eMMC: 0xf983_0000 */
#define REG_CRG_BASE        (0xf8a22000)                 /* CRG: 0xf8a2_2000 */

/*Seperate the emmc clock to different path to keep in mind that the emmc clock maybe different for FPGA and ASIC*/
#ifndef CFG_FPGA
#define MMC_SRC_CLK     (25 * 1000 * 1000) //25M
#else
#define MMC_SRC_CLK     (25 * 1000 * 1000) //25m
#endif

#define MMC_ID_CLK        (400 * 1000)         // 400k

#define MMC_200M_CLK      (200*1000*1000)      // 200MHz
#define MMC_1665M_CLK     (166500*1000)        // 166.5MHz
#define MMC_150M_CLK      (150*1000*1000)      // 150MHz
#define MMC_125M_CLK      (125*1000*1000)      // 125MHz
#define MMC_100M_CLK      (100*1000*1000)      // 100MHz
#define MMC_75M_CLK       (75*1000*1000)       // 75MHz
#define MMC_50M_CLK       (50*1000*1000)       // 50MHz
#define MMC_25M_CLK       (25*1000*1000)       // 25MHz
#define MMC_6M_CLK        (6*1000*1000)        // 6MHz

#define EMMC_SRST_REQ                  (0x01 << 4)
#define EMMC_CKEN                      (0x01 << 1)
#define EMMC_BUS_CKEN                  (0x01 << 0)

#define EMMC_CLK_SEL_MASK              (0x07 << 8)
#define EMMC_CLK_MODE_MASK             (0x01 << 19)

#define EMMC_CLK_MODE0_100M            (0x00 << 8)
#define EMMC_CLK_MODE0_50M             (0x01 << 8)
#define EMMC_CLK_MODE0_25M             (0x02 << 8)
#define EMMC_CLK_MODE0_200M            (0x03 << 8)
#define EMMC_CLK_MODE0_150M            (0x04 << 8)
#define EMMC_CLK_MODE0_1665M           (0x05 << 8)

#define MMC_NORMAL_MASK  0x01
#define SD_NORMAL_MASK   0x02

#define EMMC_DRV_PS_MASK             (0x7 << 16)
#define EMMC_SAP_PS_MASK             (0x7 << 12)
#define EMMC_CLK_MASK                (0x3 << 8)

#define EMMC_CLK_MODE                (0x1 << 19)

#define EMMC_DRV_PS_0_0              (0b000 << 16)
#define EMMC_DRV_PS_45_22DOT5        (0b001 << 16)
#define EMMC_DRV_PS_90_45            (0b010 << 16)
#define EMMC_DRV_PS_135_67DOT5       (0b011 << 16)
#define EMMC_DRV_PS_180_90           (0b100 << 16)
#define EMMC_DRV_PS_225_112DOT5      (0b101 << 16)
#define EMMC_DRV_PS_270_145          (0b110 << 16)
#define EMMC_DRV_PS_315_167DOT5      (0b111 << 16)

#define EMMC_SAP_PS_0_0              (0b000 << 12)
#define EMMC_SAP_PS_45_22DOT5        (0b001 << 12)
#define EMMC_SAP_PS_90_45            (0b010 << 12)
#define EMMC_SAP_PS_135_67DOT5       (0b011 << 12)
#define EMMC_SAP_PS_180_90           (0b100 << 12)
#define EMMC_SAP_PS_225_112DOT5      (0b101 << 12)
#define EMMC_SAP_PS_270_145          (0b110 << 12)
#define EMMC_SAP_PS_315_167DOT5      (0b111 << 12)

#ifndef  CFG_EDA_VERIFY
#define MMC_WAIT_PWR_TIME     20       //3004:change to 40ms// 3006:change to 20ms
#else
#define MMC_WAIT_PWR_TIME     1       //ms
#endif

#ifndef  CFG_EDA_VERIFY
#define SD_WAIT_PWR_TIME      40       //ms
#else
#define SD_WAIT_PWR_TIME      1       //ms
#endif

#define DELAY_3S		(1000*3)     //3s

#define MMC_WAIT_BDS_TIME     1000    //ms

#define mmc_reg_get(offset)     reg_get(REG_MMC_BASE + (offset))
#define mmc_reg_set(offset,val) reg_set(REG_MMC_BASE + (offset), val)

#define MMC_FUNC(func) if ((func) != 0) return -1

#define FOREVER         while(1)

/*-----------------------------------------------------------------
 * timer interface
------------------------------------------------------------------*/
void timer_init(void);
void timer_deinit(void);
void timer_start(void);
unsigned long timer_get_val(void);
unsigned long timer_get_divider(void);
void jump_to_read_reg(int board_type, unsigned int offset);
int mmc_normal_init_slow(void);
void mmc_deinit(void);


void emmc_quirk(int board_type)
{
	head_area_s *head_info = (head_area_s *)HEAD_INFO_RAM_ADDR;
	unsigned int offset = 0;
	int *continue_boot = (int *)NON_BLANK_FLASH_FLAG_ADDR;

	offset = head_info->uncheck_hisi.param_item_start + board_type * head_info->uncheck_hisi.param_item_len;

	/* For normal booting(not uart or usb bootstrap) or burning by boot on board,  and booting from eMMC, enter eMMC quirk */
	if (((readl(0xf8000088) == 0) || (*continue_boot == 1)) &&  ((readl(0xf8a20000)>>9 & 0x7) == 0x3)) {
		//serial_puts("enter emmc quirk\n");
		mmc_deinit();
		mmc_normal_init_slow();
		jump_to_read_reg(board_type, offset);
	}
}


int mmc_step1(void)
{
	int i, ret = -1;
	u32 wait_time = MMC_WAIT_PWR_TIME * timer_get_divider();

	mmc_reg_set(MMC_PWREN, 0);

	//reset all
	mmc_reg_set(MMC_CTRL, 7);
	for (i = 0; i < 0xf00; ++i) {
		if ((mmc_reg_get(MMC_CTRL) & 7) == 0) {
			ret = 0;
			break;
		}
	}

	//wait
	timer_start();
	while( timer_get_val() < wait_time ) {
	}

	//power on
	mmc_reg_set(MMC_PWREN, 1);

	//wait
	timer_start();
	while( timer_get_val() < wait_time ) {
	}

	return ret;
}

int mmc_step2(void)
{
	mmc_reg_set(MMC_INTMASK, 0x0);
	mmc_reg_set(MMC_RINTSTS, 0xFFFFFFFF);
	mmc_reg_set(MMC_IDSTS, 0xFFFFFFFF);
	return 0;
}

int mmc_step3(void)
{
	mmc_reg_set(MMC_CTRL, (1<<4));
	return 0;
}

int mmc_update_clk(unsigned cmdflag)
{
	u32 val, i, j;

	for(i = 0; i < 3; ++i)
	{
		mmc_reg_set(MMC_CMD, (1<<31) | (1<<21) | (1<<13) | cmdflag); /* 0x80202000*/

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

int mmc_step4(int srcclk, int clk, unsigned cmdflag)
{
	mmc_reg_set(MMC_CLKENA, 0);
	MMC_FUNC( mmc_update_clk(cmdflag) );

	if ((srcclk != (25 * 1000 * 1000)) || (clk != (400 * 1000))) {
		serial_puts("Invalid srcclk or clk\n");
		return -1;
	}

	mmc_reg_set(MMC_CLKDIV, (srcclk + (clk<<1) -1) / (clk<<1));

	mmc_reg_set(MMC_CLKSRC, 0);
	MMC_FUNC( mmc_update_clk(cmdflag) );

	mmc_reg_set(MMC_CLKENA, 1);
	MMC_FUNC( mmc_update_clk(cmdflag) );

	return 0;
}

int mmc_step5_1(void)
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

void set_phase_400k(void)
{
	u32 crg_base = REG_CRG_BASE;
	u32 value = 0;
	/* Set the phase before send cmd0 */
	value = reg_get(crg_base + 0xa0);
	value &= ~EMMC_SAP_PS_MASK;
	value |= EMMC_SAP_PS_45_22DOT5;
	value &= ~EMMC_DRV_PS_MASK;
	value |= EMMC_DRV_PS_180_90;
	reg_set(crg_base + 0xa0, value);
}

void mmc_deinit(void)
{

	set_phase_400k();

	mmc_reg_set(MMC_CMD, (1<<31) | (1<<26) | USE_HOLD_REG);

	/* add function: support rst */
	mmc_step5_1();

	mmc_reg_set(MMC_CTRL, 7);

	//power down
	mmc_reg_set(MMC_PWREN, 0);
}

/************************************sd driver*********************************/
int sd_step2(void)
{
	/* set max timeout param */
	mmc_reg_set(MMC_TMOUT, DATA_TIMEOUT | RESPONSE_TIMEOUT);
	return 0;
}

int sd_step3(void)
{
	/* set FIFO param with rx th 0x100/2-1 */
	u32 val;
	val = mmc_reg_get(MMC_FIFOTH);
	val = (val & ~(0x7FF<<16)) | (0x7f << 16);
	mmc_reg_set(MMC_FIFOTH, val);

	return 0;
}

int sd_step4(void)
{
	/* set 1 bit data width */
	mmc_reg_set(MMC_CTYPE, CARD_WIDTH_1BIT);
	return 0;
}

void send_cmd(unsigned cmd, unsigned arg, unsigned wait_busy, unsigned cmdflag)
{
	unsigned int tmp_reg;
	unsigned int wait_time = DELAY_3S*timer_get_divider();

	mmc_reg_set(MMC_RINTSTS, ALL_INT_CLR);
	mmc_reg_set(MMC_CMDARG, arg);
	mmc_reg_set(MMC_CMD, cmd | WT_PD_CPT | START_CMD | cmdflag);

	/* wait cmd send ok */
	timer_start();
	do {
		tmp_reg = mmc_reg_get(MMC_CMD);
	} while ((tmp_reg & START_CMD)&&(timer_get_val() < wait_time));

	if (tmp_reg & START_CMD)
		 debug_info("wait cmd send timeout\n", 0);

	/* wait cmd complete ok */
	timer_start();
	do {
		tmp_reg = mmc_reg_get(MMC_RINTSTS);
	} while ((!(tmp_reg & MMC_INT_CD))&&(timer_get_val() < wait_time));

	if ((tmp_reg&MMC_INT_CD) == 0)
		debug_info("wait cmd complete timeout\n", 0);

	/* wait card ok */
	if (wait_busy) {
		timer_start();
		do {
			tmp_reg = mmc_reg_get(MMC_STATUS);
		} while ((tmp_reg & DATA_BUSY)&&(timer_get_val() < wait_time));
		if (tmp_reg & DATA_BUSY)
			debug_info("wait card complete timeout\n", 0);
	}
}

int sd_step5(unsigned cmdflag)
{
	/* Send CMD0 to reset card into idle state */
	udelay(1000);
	send_cmd(MMC_CMD_GO_IDLE_STATE | SEND_INIT, 0, 0, cmdflag);
	udelay(2000);

	return 0;
}

void sd_deinit(void)
{
	mmc_reg_set(MMC_CTRL, 7);

	/* power down */
	mmc_reg_set(MMC_PWREN, 0);
}

int mmc_step10(void)
{
	unsigned int tmp_reg=0;
	unsigned int wait_retry_count = 0;

	/* Send CMD1  initialization and wait to complete*/
	do {
		send_cmd(MMC_CMD_SEND_OP_COND | RESP_EXPECT,
			OCR_HCS | MMC_VDD_32_33 | MMC_VDD_33_34, 0, USE_HOLD_REG);
		tmp_reg = mmc_reg_get(MMC_RESP0);
		udelay(5000);
		if (tmp_reg & OCR_BUSY)
			break;
		wait_retry_count ++;
	} while (wait_retry_count < 600);

	/* card capacity */
	hcs = ((tmp_reg & OCR_HCS) == OCR_HCS);

	/* Send CMD2 to get card cid numbers*/
	send_cmd(MMC_CMD_ALL_SEND_CID | RESP_EXPECT | RESP_LENGTH
		| CHECK_RESP_CRC, 0, 0, USE_HOLD_REG);

	/* Send CMD3  to get card new address with default address(0000) */
	send_cmd(MMC_CMD_SET_RELATIVE_ADDR | RESP_EXPECT
		| CHECK_RESP_CRC, 0, 0, USE_HOLD_REG);

	/* Send CMD7 for card from standby state to transfer state*/
	send_cmd(MMC_CMD_SELECT_CARD | RESP_EXPECT | CHECK_RESP_CRC,
		0, 1, USE_HOLD_REG);

	/* Send CMD6 */
	send_cmd(MMC_CMD_SWITCH | RESP_EXPECT | CHECK_RESP_CRC,
		(MMC_SWITCH_MODE_WRITE_BYTE << 24) |
		(EXT_CSD_BUS_WIDTH << 16) | (EXT_CSD_BUS_WIDTH_8 << 8),
		1, USE_HOLD_REG);
	/* set data width */
	mmc_reg_set(MMC_CTYPE, CARD_WIDTH_8BIT);

	return 0;
}

int mmc_normal_init_slow(void)
{
	u32 crg_base = REG_CRG_BASE;
	u32 value = 0;

	/* open clock, clear reset */
	value = reg_get(crg_base + 0xa0);  /* SDIO1 CRG: 0xf8a220a0 */
	value &=~(EMMC_CLK_SEL_MASK|EMMC_CLK_MODE_MASK);
	value |= EMMC_CLK_MODE0_25M;                   /* EMMC_clk_sel, select 25MHz */
	reg_set(crg_base + 0xa0, value);
	value |= (EMMC_CKEN|EMMC_BUS_CKEN);                        /* open clock */
	reg_set(crg_base + 0xa0, value);
	value &= ~(EMMC_SRST_REQ);                  /* clear reset */
	reg_set(crg_base + 0xa0, value);

	set_phase_400k();

	MMC_FUNC( mmc_step1() );
	MMC_FUNC( mmc_step2() );
	MMC_FUNC( mmc_step3() );
	MMC_FUNC( sd_step2());
	MMC_FUNC( sd_step3());
	MMC_FUNC( sd_step4());
	MMC_FUNC( mmc_step4(MMC_SRC_CLK, MMC_ID_CLK, 0));
	MMC_FUNC( mmc_step5_1() );
	MMC_FUNC( sd_step5(0));
	MMC_FUNC( mmc_step10());

	return 0;
}

