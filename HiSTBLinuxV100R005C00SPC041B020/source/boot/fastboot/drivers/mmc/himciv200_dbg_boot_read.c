/*
 * this function is only for debug.
 */
#include <asm/io.h>
#include <asm/arch/platform.h>

#include <common.h>
#include <himciv200_reg.h>

#define MMC_INT_DATA_FIFO_ERROR         (0x8E80)

#define MMC_SRC_CLK     (25* 1000 * 1000)
#define MMC_BOOT_CLK    (400 * 1000)

#define MMC_WAIT_PWR_TIME     20
#define MMC_WAIT_BDS_TIME     1000

#define reg_get(X) readl(X)
#define reg_set(X, Y) writel((Y), (X))
#define mmc_reg_get(offset)     reg_get(mmc_base + (offset))
#define mmc_reg_set(offset,val) reg_set(mmc_base + (offset), (val))

#define MMC_FUNC(func) if ((func) != 0) return -1
#define PRIVATE static
#define FOREVER while(1)

static u32 mmc_base;

PRIVATE int mmc_step1(void)
{
	int i, ret = -1;
	mmc_base = REG_BASE_MCI; 

	//power off
	mmc_reg_set(MCI_PWREN, 0);

	//reset all
	mmc_reg_set(MCI_CTRL, 7);
	for (i = 0; i < 0xf00; ++i)
		if (0 == (mmc_reg_get(MCI_CTRL) & 7)) {
			ret = 0;
			break;
		}

		//wait
		mdelay(MMC_WAIT_PWR_TIME);

		//power on
		mmc_reg_set(MCI_PWREN, 1);

		//wait
		mdelay(MMC_WAIT_PWR_TIME);

		return ret;
}

PRIVATE int mmc_step2(void)
{
	mmc_reg_set(MCI_INTMASK, 0x0);
	mmc_reg_set(MCI_RINTSTS, 0xFFFFFFFF);
	mmc_reg_set(MCI_IDSTS, 0xFFFFFFFF);
	return 0;
}

PRIVATE int mmc_step3(void)
{
	mmc_reg_set(MCI_CTRL, (1<<4));
	return 0;
}

PRIVATE int mmc_update_clk(void)
{
	u32 val, i, j;

	for(i = 0; i < 3; ++i)
	{
		mmc_reg_set(MCI_CMD, (1<<31) | (1<<21) | (1<<13));

		for(j = 0; j < 0xf00; ++j)
		{
			val = mmc_reg_get(MCI_CMD);
			if ((val & (1<<31)) == 0)
				return 0;

			val = mmc_reg_get(MCI_RINTSTS);
			if ((val & (1<<12)) != 0)
				break;
		}
	}

	return -1;
}

PRIVATE int mmc_step4(void)
{
	mmc_reg_set(MCI_CLKENA, 0);
	MMC_FUNC( mmc_update_clk() );

	mmc_reg_set(MCI_CLKDIV, MMC_SRC_CLK / (MMC_BOOT_CLK * 2));
	mmc_reg_set(MCI_CLKSRC, 0);
	MMC_FUNC( mmc_update_clk() );

	mmc_reg_set(MCI_CLKENA, 1);
	MMC_FUNC( mmc_update_clk() );

	return 0;
}

PRIVATE int mmc_step5(void)
{
	mmc_reg_set(MCI_TIMEOUT, (mmc_reg_get(MCI_TIMEOUT) & 0xff) | 0xFFFFF00); // nearly 2.6s at 400kHz
	return 0;
}

PRIVATE int mmc_step5_1(void)
{
	udelay(200);
	mmc_reg_set(MCI_RESET_N, 0);
	/* For eMMC, minimum is 1us but give it 10us for good measure */
	udelay(100);
	mmc_reg_set(MCI_RESET_N, 1);
	/* For eMMC, minimum is 200us but give it 300us for good measure */
	udelay(1000);
	return 0;
}

PRIVATE int mmc_step6(void)
{
	mmc_reg_set(MCI_BLKSIZ, 0x200);
	return 0;
}

PRIVATE int mmc_step7(void)
{
	//1024k boot is big engough
	mmc_reg_set(MCI_BYTCNT, 0x100000);
	return 0;
}

PRIVATE int mmc_step8(void)
{
	u32 val;
	val = mmc_reg_get(MCI_FIFOTH);
	val = (val & ~(0xFFF<<16)) | (0x7f << 16);
	mmc_reg_set(MCI_FIFOTH, val);

#if 0
	val = mmu_open? reg_get(PERI_V_START_MODE) : reg_get(PERI_P_START_MODE);
	if (val & START_NF_BOOTBW)
	{
		/*1 for 8bit*/
		mmc_reg_set(MCI_CTYPE, 0x10000);
	}
	else
	{
		/*0 for 4bit, default*/
		mmc_reg_set(MCI_CTYPE, 1);
	}
#endif /* 0 */

	/*0 for 4bit, default*/
	mmc_reg_set(MCI_CTYPE, 1);

	return 0;
}

PRIVATE int mmc_step9(void)
{
	/*enable_boot_ack = 0*/
	mmc_reg_set(MCI_CMD, (1<<31) | (1<<24) | (0<<16) | (1<<9));
	return 0;
}

PRIVATE int mmc_wait_bds(void)
{
	u32 val;
	u32 timeout = MMC_WAIT_BDS_TIME;

	do
	{
		val = mmc_reg_get(MCI_RINTSTS);
		if (val & DRTO_INT_STATUS)
			break;

		mdelay(1);
	}while(timeout--);

	mmc_reg_set(MCI_RINTSTS, 0xFFFFFFFF);

	if (!(val & DRTO_INT_STATUS))
	{
		mmc_reg_set(MCI_CMD, (1<<31) | (1<<26));
		printf("bootmoderead:mmc_wait_bds timeout,Failed!!!\n");
		return -1;
	}
	else
	{
		return 0;
	}
}

PRIVATE int mmc_init(void)
{
	u32 value = 0;
#if 0
	int index = 0;
	for(index = 0xe8; index <= 0x108; index += 4)
	{
		if(2 != reg_get(REG_BASE_IO_CONFIG + index))
		{
			printf("emmc pin_mux ebidq_gpio_sdio%d is not equal to 0x2, bootmoderead failed!!!\n", index);
			return -1;
		}
	}
#endif
	/* open clock, clear reset */
	reg_set(REG_BASE_CRG + 0xa0, value);
	value |= (1<<4);                  /* reset */
	reg_set(REG_BASE_CRG + 0xa0, value);  
	value = reg_get(REG_BASE_CRG + 0xa0);  /* SDIO1 CRG: 0xf8a220a0 */
	value |= (3<<8);                   /* sdio1_clk_sel, select 25MHz */
	reg_set(REG_BASE_CRG + 0xa0, value);
	value |= 3;                        /* open clock */
	reg_set(REG_BASE_CRG + 0xa0, value);
	value &= ~(1<<4);                  /* clear reset */
	reg_set(REG_BASE_CRG + 0xa0, value);  
	
	MMC_FUNC( mmc_step1() );
	MMC_FUNC( mmc_step2() );
	MMC_FUNC( mmc_step3() );
	MMC_FUNC( mmc_step4() );
	MMC_FUNC( mmc_step5() );
	MMC_FUNC( mmc_step5_1() );
	MMC_FUNC( mmc_step6() );
	MMC_FUNC( mmc_step7() );
	MMC_FUNC( mmc_step8() );
	MMC_FUNC( mmc_step9() );
	MMC_FUNC( mmc_wait_bds() );

	return 0;
}

PRIVATE void mmc_deinit(void)
{
	mmc_reg_set(MCI_CMD, (1<<31) | (1<<26));
	/* add function: support rst */
	mmc_step5_1();
	mmc_reg_set(MCI_CTRL, 7);
	mmc_reg_set(MCI_PWREN, 0);
}

PRIVATE size_t mmc_read(void *ptr, size_t size)
{
	u32 val;
	u32 *tmp = (u32*) ptr;
	size_t count;

	size = (size + 3) >> 2;
	while (size > 0)
	{
		FOREVER
		{
			val = mmc_reg_get(MCI_RINTSTS);
			if ( ( val & RXDR_INT_STATUS ) || ( val & DTO_INT_STATUS ) )
			{
				break;
			}

			if( val & MMC_INT_DATA_FIFO_ERROR )
			{
				return -1;    //error! return
			}
		}

		count = ( (mmc_reg_get(MCI_STATUS) >> 17) & 0x1fff );

		if (count > size)
			count = size;

		mmc_reg_set(MCI_RINTSTS, 0xFFFFFFFF);

		for (; count > 0; --count)
		{
			*tmp++ = mmc_reg_get(MCI_FIFO_START);
			--size;
		}
		// all finished
		if( val & DTO_INT_STATUS )
			break;

	}
	return (size_t)(tmp - (u32*)ptr);
}

void emmc_bootmode_read(void *ptr, unsigned int size)
{
	if (!size) {
		printf("parameter error: read count is 0.\n");
		return;
	}

	if (0 == mmc_init())
		mmc_read((void*)ptr, size);

	mmc_deinit();
	return;
}
