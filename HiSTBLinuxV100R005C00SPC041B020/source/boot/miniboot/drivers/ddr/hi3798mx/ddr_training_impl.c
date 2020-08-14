/**
 * ddr_training_impl.c
 *
 * Copyright (c) 2009-2014, HiSilicon Technologies Co., Ltd.
 * All rights reserved.
 *
 * DDR training implement.
 */

#include "ddr_training_impl.h"
#include "ddr_interface.h"

#define __common__
#ifdef DDR_TRAINING_STAT_CONFIG
/**
 * ddr_training_save
 * @mask
 * @phy
 * @byte
 * @dq
 *
 * Save training result in stat register.
 */
static void ddr_training_save(unsigned int mask, unsigned int phy,
	int byte, int dq)
{
	unsigned int stat;
	unsigned int phy_index;

	stat = REG_READ(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_STAT);
	/* only record the first error */
	if (stat)
		return;

	stat = mask;

	if (0 != phy) {
		phy_index = (DDR_REG_BASE_PHY0 == phy ?
			DDR_ERR_PHY0 : DDR_ERR_PHY1);
		stat |= phy_index;
	}

	if (-1 != byte)
		stat |= (byte << DDR_ERR_BYTE_BIT);

	if (-1 != dq)
		stat |= (dq << DDR_ERR_DQ_BIT);

	REG_WRITE(stat, DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_STAT);
}
#endif
/**
 * ddr_training_stat
 * @mask
 * @phy
 * @byte
 * @dq
 *
 * Record error code in register.
 */
void ddr_training_stat(unsigned int mask, unsigned int phy, int byte, int dq)
{
	ddr_training_error(mask, phy, byte, dq);
#ifdef DDR_TRAINING_STAT_CONFIG
	ddr_training_save(mask, phy, byte, dq);
#endif
}

/**
 * ddr_training_check_bypass
 * @mask
 *
 * Check DDR training item whether by pass.
 */
int ddr_training_check_bypass(unsigned int mask)
{
	/* training item disable */
	if (REG_READ(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG) & mask) {
		DDR_DEBUG("DDR training itme[%x] is disable. Sysctl[%x = %x]",
			mask, (DDR_REG_BASE_SYSCTRL
			+ SYSCTRL_DDR_TRAINING_CFG),
			REG_READ(DDR_REG_BASE_SYSCTRL
			+ SYSCTRL_DDR_TRAINING_CFG));
		return DDR_TRUE;
	} else {
		return DDR_FALSE;
	}
}

#if !defined(DDR_TRAINING_CUT_CODE_CONFIG) || defined(DDR_TRAINING_CMD)
/**
 * ddr_training_phy_disable
 * @index
 *
 * Check PHY whether disable.
 * DDR_TRUE: PHY is disable.
 * DDR_FALSE: PHY is not disable.
 */
int ddr_training_phy_disable(int index)
{
	unsigned int mask;
	mask = (index == 0 ? DDR_BYPASS_PHY0_MASK : DDR_BYPASS_PHY1_MASK);
	return ddr_training_check_bypass(mask);
}

/**
 * ddr_training_get_base
 * @index
 * @base_dmc
 * @base_phy
 *
 * Get DMC and PHY register base address.
 */
void ddr_training_get_base(int index, unsigned int *base_dmc,
	unsigned int *base_phy)
{
	*base_dmc = (index == 0 ? DDR_REG_BASE_DMC0 : DDR_REG_BASE_DMC1);
	*base_phy = (index == 0 ? DDR_REG_BASE_PHY0 : DDR_REG_BASE_PHY1);
}

/**
 * ddr_training_save_reg
 * @relate_reg
 * @mask
 *
 * Save register value before training.
 */
void ddr_training_save_reg(struct tr_relate_reg *relate_reg, unsigned int mask)
{
	int i = 0;
	unsigned int base_dmc, base_phy;

	for (i = 0; i < DDR_PHY_NUM; i++) {
		if (ddr_training_phy_disable(i))
			continue;

		ddr_training_get_base(i, &base_dmc, &base_phy);

		/* save reg value */
		relate_reg->auto_ref_timing[i] =
			REG_READ(base_dmc + DDR_DMC_TIMING2);
		relate_reg->power_down[i] =
			REG_READ(base_dmc + DDR_DMC_CFG_PD);
		relate_reg->misc_scramb[i] = REG_READ(base_phy + DDR_PHY_MISC);
		relate_reg->ac_phy_ctl[i] =
			REG_READ(base_phy + DDR_PHY_ACPHYCTL4);

		/* set new value */
		switch (mask) {
		case DDR_BYPASS_WL_MASK:
			/* disable auto refresh */
			REG_WRITE(relate_reg->auto_ref_timing[i]
				& DMC_AUTO_TIMING_DIS,
				base_dmc + DDR_DMC_TIMING2);
			break;
		case DDR_BYPASS_GATE_MASK:
			/* disable auto refresh */
			REG_WRITE(relate_reg->auto_ref_timing[i]
				& DMC_AUTO_TIMING_DIS,
				base_dmc + DDR_DMC_TIMING2);

			if (!(REG_READ(base_phy + DDR_PHY_DRAMCFG)
					& PHY_DRAMCFG_MA2T)) /* set 1T */
				REG_WRITE(0x0, base_phy + DDR_PHY_ACPHYCTL4);
			break;
		case DDR_BYPASS_HW_MASK:
			if (!(REG_READ(base_phy + DDR_PHY_DRAMCFG)
					& PHY_DRAMCFG_MA2T)) /* set 1T */
				REG_WRITE(0x0, base_phy + DDR_PHY_ACPHYCTL4);
			break;
		default:
			break;
		}

		REG_WRITE(relate_reg->power_down[i] & DMC_POWER_DOWN_DIS,
			base_dmc + DDR_DMC_CFG_PD);
		REG_WRITE(relate_reg->misc_scramb[i] & PHY_MISC_SCRAMB_DIS,
			base_phy + DDR_PHY_MISC);

		ddr_phy_cfg_update(base_phy);
	}

	DDR_AXI_SAVE_FUNC(relate_reg);

	/* save customer reg */
	DDR_TRAINING_SAVE_REG_FUNC((void *)relate_reg, mask);

	DDR_ASM_DSB();
}

/**
 * ddr_training_restore_reg
 * @relate_reg
 *
 * Restore register value after training.
 */
void ddr_training_restore_reg(struct tr_relate_reg *relate_reg)
{
	int i = 0;
	unsigned int base_dmc, base_phy;

	for (i = 0; i < DDR_PHY_NUM; i++) {
		if (ddr_training_phy_disable(i))
			continue;

		ddr_training_get_base(i, &base_dmc, &base_phy);

		/* enable auto refresh*/
		REG_WRITE(relate_reg->auto_ref_timing[i],
			base_dmc + DDR_DMC_TIMING2);
		REG_WRITE(relate_reg->power_down[i],
			base_dmc + DDR_DMC_CFG_PD);
		REG_WRITE(relate_reg->misc_scramb[i],
			base_phy + DDR_PHY_MISC);
		if (!(REG_READ(base_phy + DDR_PHY_DRAMCFG)
				& PHY_DRAMCFG_MA2T))
			REG_WRITE(relate_reg->ac_phy_ctl[i],
				base_phy + DDR_PHY_ACPHYCTL4);

		ddr_phy_cfg_update(base_phy);
	}

	DDR_AXI_RESTORE_FUNC(relate_reg);

	/* restore customer reg */
	DDR_TRAINING_RESTORE_REG_FUNC((void *)relate_reg);

	DDR_ASM_DSB();
}

/**
 * ddr_training_switch_axi
 * @index
 * @relate_reg
 *
 * Switch AXI to DMC0 or DMC1 for DDRT test.
 */
void ddr_training_switch_axi(int index, struct tr_relate_reg *relate_reg)
{
	DDR_AXI_SWITCH_FUNC(index, relate_reg);
}
#endif

/**
 * ddr_dmc_get_reversed
 * @base_dmc
 *
 * Get reversed data for DDRT.
 */
unsigned int ddr_dmc_get_reversed(unsigned int base_dmc)
{
	unsigned int offset;
	offset = (base_dmc == DDR_REG_BASE_DMC0 ?
		SYSCTRL_DDRT_PATTERN : SYSCTRL_DDRT_PATTERN_SEC);
	return REG_READ(DDR_REG_BASE_SYSCTRL + offset);
}

/**
 * ddr_dmc_sfc_cmd
 * @base_dmc
 * @sfc_cmd
 * @sfc_addr
 * @sfc_bank
 *
 * Excute DMC sfc command.
 */
static void ddr_dmc_sfc_cmd(unsigned int base_dmc, unsigned int sfc_cmd,
	unsigned int sfc_addr, unsigned int sfc_bank)
{
	unsigned int count = 0;

	/* set sfc cmd */
	DMC_SFC_CMD_WRITE(sfc_cmd, base_dmc + DDR_DMC_SFCCMD);
	/* set col and row */
	REG_WRITE(sfc_addr, base_dmc + DDR_DMC_SFCADDR);
	/* set bank */
	DMC_SFC_BANK_WRITE(sfc_bank, base_dmc + DDR_DMC_SFCBANK);
	/* excute cmd */
	REG_WRITE(0x1, base_dmc + DDR_DMC_SFCREQ);

	DDR_ASM_DSB();

	while (count < DDR_SFC_WAIT_TIMEOUT) { /* wait command finished */
		if (!(REG_READ(base_dmc + DDR_DMC_SFCREQ) & 0x1))
			break;

		count++;
	}

	if (count >= DDR_HWR_WAIT_TIMEOUT)
		DDR_ERROR("SFC cmd wait timeout.");
}

/**
 * ddr_phy_cfg_update
 * @base_phy
 *
 * Update delay setting in registers to PHY immediately.
 * Make delay setting take effect.
 */
void ddr_phy_cfg_update(unsigned int base_phy)
{
	unsigned int tmp;

	tmp = REG_READ(base_phy + DDR_PHY_MISC);
	tmp |= (1 << PHY_MISC_UPDATE_BIT);
	/* update new config to PHY */
	REG_WRITE(tmp, base_phy + DDR_PHY_MISC);
	tmp &= ~(1 << PHY_MISC_UPDATE_BIT);
	REG_WRITE(tmp, base_phy + DDR_PHY_MISC);
	tmp = REG_READ(base_phy + DDR_PHY_PHYINITCTRL);
	/* set 1 to issue PHY counter reset signal */
	tmp |= (1 << PHY_PHYCONN_RST_BIT);
	REG_WRITE(tmp, base_phy + DDR_PHY_PHYINITCTRL);
	/* set 0 to end the reset signal */
	tmp &= ~(1 << PHY_PHYCONN_RST_BIT);
	REG_WRITE(tmp, base_phy + DDR_PHY_PHYINITCTRL);

	DDR_ASM_DSB();
}

/**
 * ddr_phy_set_dq_bdl
 * @base_phy
 * @deskew_value
 * @bytenum
 * @dqindex
 * @mode
 *
 * Set delay value of the bit delay line of the DATA block.
 */
void ddr_phy_set_dq_bdl(unsigned int base_phy, unsigned int deskew_value,
	unsigned int bytenum, unsigned int dqindex, unsigned int mode)
{
	unsigned int val;
	unsigned int offset;
	unsigned int dq;

	dq = dqindex & 0x7;
	if (DDR_MODE_WRITE == mode) {
		if (dq < 4)
			offset = DDR_PHY_DXNWDQNBDL0(bytenum);
		else
			offset = DDR_PHY_DXNWDQNBDL1(bytenum);
	} else {
		if (dq < 4)
			offset = DDR_PHY_DXNRDQNBDL0(bytenum);
		else
			offset = DDR_PHY_DXNRDQNBDL1(bytenum);
	}

	dq &= 0x3;
	val = REG_READ(base_phy + offset);
	val &= ~(0xFF << (dq << 3));
	val |= ((PHY_BDL_MASK & deskew_value) << (dq << 3));
	REG_WRITE(val, base_phy + offset);

	ddr_phy_cfg_update(base_phy);
}

/**
 * ddr_phy_get_dq_bdl
 * @base_phy
 * @bytenum
 * @dqindex
 * @mode
 *
 * Get PHY DQ value.
 */
int ddr_phy_get_dq_bdl(unsigned int base_phy, unsigned int bytenum,
	unsigned int dqindex, unsigned int mode)
{
	unsigned int val;
	unsigned int offset;
	unsigned int dq;

	dq = dqindex & 0x7;
	if (DDR_MODE_WRITE == mode) {
		if (dq < 4)
			offset = DDR_PHY_DXNWDQNBDL0(bytenum);
		else
			offset = DDR_PHY_DXNWDQNBDL1(bytenum);
	} else {
		if (dq < 4)
			offset = DDR_PHY_DXNRDQNBDL0(bytenum);
		else
			offset = DDR_PHY_DXNRDQNBDL1(bytenum);
	}

	dq &= 0x3;
	val = (REG_READ(base_phy + offset) >> (dq << 3)) & PHY_BDL_MASK;

	return val;
}

/**
 * ddr_phy_get_byte_num
 * @base_dmc
 *
 * Get byte number.
 */
unsigned int ddr_phy_get_byte_num(unsigned int base_dmc)
{
	/* memery width -> byte number */
	return ((REG_READ(base_dmc + DDR_DMC_CFG_DDRMODE)
		>> DMC_MEM_WIDTH_BIT) & DMC_MEM_WIDTH_MASK) << 1;
}

#define __ddrt__
#ifdef DDR_DDRT_SPECIAL_CONFIG
/**
 * ddr_ddrt_read
 * @addr
 *
 * Some special DDRT need read register repeatedly.
 */
static unsigned int ddr_ddrt_read(unsigned int addr)
{
	int times = 0;
	unsigned int data0, data1, data2;
	do {
		data0 = REG_READ(addr);
		data1 = REG_READ(addr);
		data2 = REG_READ(addr);
		times++;
	} while (((data0 != data1) || (data1 != data2))
			&& (times < DDRT_READ_TIMEOUT));

	if (times >= DDRT_READ_TIMEOUT) {
		DDR_FATAL("DDRT wait timeout.");
		ddr_training_stat(DDR_ERR_DDRT_TIME_OUT, 0, -1, -1);
	}

	return data0;
}

/**
 * ddr_ddrt_write
 * @data
 * @addr
 *
 *	Some special DDRT need write twice register.
 */
static void ddr_ddrt_write(unsigned int data, unsigned int addr)
{
	unsigned int tmp;
	tmp = REG_READ(addr);
	REG_WRITE(data, addr);
	REG_WRITE(data, addr);
}
#endif

/**
 * ddr_ddrt_init
 * @base_dmc
 * @mode
 *
 * Init DDRT register before DDRT test.
 */
void ddr_ddrt_init(unsigned int base_dmc, unsigned int mode)
{
	unsigned int mem_width;
	unsigned int mem_config;

	DDR_DEBUG("DDRT[%x] init.", DDR_REG_BASE_DDRT);

	DDR_TRAINING_DDRT_PREPARE_FUNC();

	mem_width = ((REG_READ(base_dmc + DDR_DMC_CFG_DDRMODE)
				>> DMC_MEM_WIDTH_BIT) & DMC_MEM_WIDTH_MASK);
	mem_config = ((mem_width - 1) << DDRT_DDR_MEM_WIDTH)
				| DDRT_DDR_COL_WIDTH | DDRT_DDR_ROW_WIDTH
				| DDRT_DDR_BANK_WIDTH;
	/* DDRT SDRAM config */
	DDRT_REG_WRITE(mem_config, DDR_REG_BASE_DDRT + DDRT_MEM_CONFIG);
	/* DDR Address Base */
	DDRT_REG_WRITE(DDRT_CFG_BASE_ADDR,
		DDR_REG_BASE_DDRT + DDRT_DDR_BASE_ADDR);
	/* DDRT test DDR using space */
	DDRT_REG_WRITE(ddr_ddrt_get_test_addr(),
		DDR_REG_BASE_DDRT + DDRT_ADDR);
	DDRT_REG_WRITE(DDRT_CFG_SEED, DDR_REG_BASE_DDRT + DDRT_SEED);

	if (DDR_DDRT_MODE_GATE == mode) {
		/* Read or Write Once */
		DDRT_REG_WRITE(DDRT_CFG_BURST_CFG_GATE,
			DDR_REG_BASE_DDRT + DDRT_BURST_CONFIG);
		DDRT_REG_WRITE(0x0,  DDR_REG_BASE_DDRT + DDRT_BURST_NUM);
		DDRT_REG_WRITE(0x0,  DDR_REG_BASE_DDRT + DDRT_ADDR_NUM);
		DDRT_REG_WRITE(0x0,  DDR_REG_BASE_DDRT + DDRT_LOOP_NUM);
		DDRT_REG_WRITE(DDRT_CFG_REVERSED,
			DDR_REG_BASE_DDRT + DDRT_REVERSED_DQ);
	} else {
		/* reversed data form register init table */
		/* 128bit BURST4 */
		DDRT_REG_WRITE(DDRT_CFG_BURST_CFG_DATAEYE,
			DDR_REG_BASE_DDRT + DDRT_BURST_CONFIG);
		DDRT_REG_WRITE(ddr_dmc_get_reversed(base_dmc),
			DDR_REG_BASE_DDRT + DDRT_REVERSED_DQ);
		DDRT_REG_WRITE(DDRT_CFG_BURST_NUM,
			DDR_REG_BASE_DDRT + DDRT_BURST_NUM);
		DDRT_REG_WRITE(DDRT_CFG_ADDR_NUM,
			DDR_REG_BASE_DDRT + DDRT_ADDR_NUM);
		DDRT_REG_WRITE(DDRT_CFG_LOOP_NUM,
			DDR_REG_BASE_DDRT + DDRT_LOOP_NUM);
	}
}

/**
 * ddr_ddrt_test
 * @mask : DDRT option mask.
 * @byte : DDR byte index.
 * @dq   : DDR dq index.
 *
 * DDRT test. Support read_only mode and write_read_compare mode.
 * Success return 0, fail return -1.
 */
int ddr_ddrt_test(unsigned int mask, int byte, int dq)
{
	unsigned int regval;
	unsigned int err_ovfl;
	unsigned int err_cnt;
	unsigned int dq_num;
	unsigned int dq_tmp;
	unsigned int times = 0;

	DDRT_REG_WRITE(mask | DDRT_CFG_START, DDR_REG_BASE_DDRT + DDRT_OP);
	DDRT_REG_WRITE(0, DDR_REG_BASE_DDRT + DDRT_STATUS);

	DDR_ASM_DSB();

	do {
		regval = DDRT_REG_READ(DDR_REG_BASE_DDRT + DDRT_STATUS);
		times++;
	} while ((!(regval & DDRT_TEST_DONE_MASK))
			&& (times < DDRT_WAIT_TIMEOUT));

	if (times >= DDRT_WAIT_TIMEOUT) {
		DDR_FATAL("DDRT wait timeout.");
		ddr_training_stat(DDR_ERR_DDRT_TIME_OUT, 0, -1, -1);
		return -1;
	}

	/* DDRT_READ_ONLY_MODE */
	if (DDRT_READ_ONLY_MODE == (mask & DDRT_TEST_MODE_MASK))
		return 0;   /* return when DDRT finish */

	/* DDRT_WR_COMPRARE_MODE No error occurred, test pass.*/
	if (regval & DDRT_TEST_PASS_MASK)
		return 0;

	if (-1 != dq) { /* check for dq*/
		dq_num = (byte << 3) + dq;
		err_ovfl = DDRT_REG_READ(DDR_REG_BASE_DDRT
			+ DDRT_DQ_ERR_OVFL) & (1 << dq_num);
		if (err_ovfl)
			return -1;

		if (dq > 3)
			dq_tmp = (dq - 4) << 3;
		else
			dq_tmp = dq << 3;
		err_cnt = DDRT_REG_READ(DDR_REG_BASE_DDRT
			+ DDRT_DQ_ERR_CNT((byte << 1) + (dq >> 2)));
		err_cnt = err_cnt & (0xff << dq_tmp);
		if (err_cnt)
			return -1;
	} else if (-1 != byte) { /* check for byte*/
		err_ovfl = DDRT_REG_READ(DDR_REG_BASE_DDRT
			+ DDRT_DQ_ERR_OVFL) & (0xff << byte);
		if (err_ovfl)
			return -1;

		err_cnt  = DDRT_REG_READ(DDR_REG_BASE_DDRT
				+ DDRT_DQ_ERR_CNT(byte << 1));
		err_cnt += DDRT_REG_READ(DDR_REG_BASE_DDRT
				+ DDRT_DQ_ERR_CNT((byte << 1) + 1));
		if (err_cnt)
			return -1;
	}

	return 0;
}

/**
 * ddr_ddrt_check
 * @training
 * @byte_index
 * @dq_index
 *
 * Check ddrt test result. Success return 0, fail return -1.
 */
static int ddr_ddrt_check(unsigned int reversed_data,
			unsigned int byte_index, unsigned int dq_index)
{
	DDRT_REG_WRITE(0, DDR_REG_BASE_DDRT + DDRT_REVERSED_DQ);
	if (ddr_ddrt_test(DDRT_WR_COMPRARE_MODE | DDRT_PATTERM_PRBS9,
		byte_index, dq_index))
		return -1;

	DDRT_REG_WRITE(reversed_data, DDR_REG_BASE_DDRT + DDRT_REVERSED_DQ);
	if (ddr_ddrt_test(DDRT_WR_COMPRARE_MODE | DDRT_PATTERM_PRBS11,
		byte_index, dq_index))
		return -1;

	return 0;
}

#define __dataeye_adjust__
#ifdef DDR_TRAINING_ADJUST_CONFIG
static unsigned int ddr_adjust_get_average(unsigned int base_phy,
			unsigned int byte_index, unsigned int mode)
{
	unsigned int dq0_3, dq4_7, val;

	if (DDR_MODE_WRITE == mode)
		return REG_READ(base_phy + DDR_PHY_DXNWDQNBDL2(byte_index));

	/* read */
	dq0_3 = REG_READ(base_phy + DDR_PHY_DXNRDQNBDL0(byte_index));
	dq4_7 = REG_READ(base_phy + DDR_PHY_DXNRDQNBDL1(byte_index));

	val = (dq0_3 & PHY_BDL_MASK) + ((dq0_3 >> 8) & PHY_BDL_MASK)
		+ ((dq0_3 >> 16) & PHY_BDL_MASK)
		+ ((dq0_3 >> 24) & PHY_BDL_MASK)
		+ (dq4_7 & PHY_BDL_MASK)
		+ ((dq4_7 >> 8) & PHY_BDL_MASK)
		+ ((dq4_7 >> 16) & PHY_BDL_MASK)
		+ ((dq4_7 >> 24) & PHY_BDL_MASK);

	val = val >> 3;
	return val;
}

/**
 * ddr_adjust_trend_check
 * @base_phy
 * @byte_index
 * @accel : Return a value to adjust quickly.
 * @mode
 *
 * Check dataeye DQ window on left or right or middle.
 */
static unsigned int ddr_adjust_trend_check(unsigned int base_phy,
			unsigned int byte_index, int *accel, unsigned int mode)
{
	unsigned int dq_bdl = 0;
	unsigned int size;

	/* 32 BDL middle[13, 17]. 128 BDL middle[40, 56] */
	/* 1 Phase = (DDR_BDL_PHASE_TRANSFORM) BDL */
	size = DDR_BDL_PHASE_TRANSFORM >> 1;

	dq_bdl =  ddr_adjust_get_average(base_phy, byte_index, mode);

	/* increase adjust step to accelerate */
	if (accel) {
		if (dq_bdl > PHY_DQ_BDL_MIDDLE)
			*accel = dq_bdl - PHY_DQ_BDL_MIDDLE;
		else if (dq_bdl < PHY_DQ_BDL_MIDDLE)
			*accel = PHY_DQ_BDL_MIDDLE - dq_bdl;

		DDR_INFO("byte[%x] bdl[%x] middle[%x] accel[%x] rdqs[%x]",
			byte_index, dq_bdl, PHY_DQ_BDL_MIDDLE, *accel,
			REG_READ(base_phy + DDR_PHY_DXNRDQSDLY(byte_index))
			& PHY_RDQS_BDL_MASK);
	}

	/* window on left */
	if (dq_bdl < (PHY_DQ_BDL_MIDDLE - size))
		return DDR_WIN_LEFT;
	/* on right */
	else if (dq_bdl > (PHY_DQ_BDL_MIDDLE + size))
		return DDR_WIN_RIGHT;
	else
		return DDR_WIN_MIDDLE;
}

/**
 * ddr_adjust_check_val
 * @val
 * @mode
 *
 * Check adjust value whether valid.
 */
static int ddr_adjust_check_val(int val, unsigned int mode)
{
	if (DDR_MODE_READ == mode) {
		if (val < 0 || val > PHY_RDQS_BDL_MASK)
			return DDR_FALSE;
	} else {
		if (val < 0 || val > PHY_WDQ_PHASE_MASK)
			return DDR_FALSE;
	}

	return DDR_TRUE;
}

/**
 * ddr_adjust_get_val
 * @base_phy
 * @byte_index
 * @mode
 *
 * Get value which need to adjust.
 */
static int ddr_adjust_get_val(unsigned int base_phy, unsigned int byte_index,
						unsigned int mode)
{
	if (DDR_MODE_READ == mode)
		return REG_READ(base_phy + DDR_PHY_DXNRDQSDLY(byte_index))
		& PHY_RDQS_BDL_MASK;
	else
		return (REG_READ(base_phy + DDR_PHY_DXNWDQDLY(byte_index))
		>> PHY_WDQ_PHASE_BIT) & PHY_WDQ_PHASE_MASK;
}

/**
 * ddr_adjust_set_val
 * @base_phy
 * @byte_index
 * @val
 * @mode
 *
 * Set value which need to adjust.
 */
static void ddr_adjust_set_val(unsigned int base_phy,
			unsigned int byte_index, int val, unsigned int mode)
{
	int delay;
	if (DDR_MODE_READ == mode) {
		delay = REG_READ(base_phy
			+ DDR_PHY_DXNRDQSDLY(byte_index));
		/* clear rdqs bdl */
		delay = delay & (~PHY_RDQS_BDL_MASK);
		REG_WRITE(delay | val,
			base_phy + DDR_PHY_DXNRDQSDLY(byte_index));
	} else {
		delay = REG_READ(base_phy
			+ DDR_PHY_DXNWDQDLY(byte_index));
		/* clear wdq phase */
		delay = delay
			& (~(PHY_WDQ_PHASE_MASK << PHY_WDQ_PHASE_BIT));
		REG_WRITE(delay | (val << PHY_WDQ_PHASE_BIT),
			base_phy + DDR_PHY_DXNWDQDLY(byte_index));
	}

	ddr_phy_cfg_update(base_phy);
}

/**
 * ddr_adjust_change_val
 * @dir
 * @val
 * @step
 * @mode
 *
 * Add or delete value to adjust.
 */
static void ddr_adjust_change_val(unsigned int dir, int *val,
			int step, unsigned int mode)
{
	if (DDR_MODE_READ == mode) {
		if (DDR_WIN_RIGHT == dir)
			(*val) = (*val) + step;
		else
			(*val) = (*val) - step;
	} else {
		/* decrease wdq phase, window move to right */
		if (DDR_WIN_RIGHT == dir)
				(*val) = (*val) - step;
			else
				(*val) = (*val) + step;
	}
}

/**
 * ddr_adjust_move_win
 * @training
 * @byte_index
 * @step
 * @dir : move direction. DDR_TRUE move to right, DDR_FALSE move to left.
 * @mode
 *
 * Move window to specified direction until the best DQ bdl beyond the midline.
 */
static void ddr_adjust_move_win(struct training_data *training,
	unsigned int byte_index, int step, unsigned int dir,
	unsigned int mode)
{
	int cur_val, def_val;
	int i;
	int accel;
	int trend;
	unsigned int max_value;

	max_value = (DDR_MODE_WRITE == mode ?
		PHY_WDQ_PHASE_MASK : PHY_RDQS_BDL_MASK);

	def_val = ddr_adjust_get_val(training->base_phy, byte_index, mode);
	cur_val = def_val;
	for (i = 0; i <= max_value; i++) {
		accel = step;
		/* write mode no need to accelerate */
		if (DDR_MODE_WRITE == mode)
			trend = ddr_adjust_trend_check(training->base_phy,
					byte_index, 0, mode);
		else
			trend = ddr_adjust_trend_check(training->base_phy,
					byte_index, &accel, mode);

		if (DDR_WIN_MIDDLE == trend || dir == trend) {
			DDR_DEBUG("Move byte[%x] window to middle suc.",
				byte_index);
			break;
		}

		ddr_adjust_change_val(dir, &cur_val, accel, mode);
		if (DDR_FALSE == ddr_adjust_check_val(cur_val, mode)) {
			DDR_WARNING("Move byte[%x] to middle fail. [%x]",
						byte_index, cur_val);
			break;
		}

		ddr_adjust_set_val(training->base_phy,
			byte_index, cur_val, mode);
		if (ddr_dataeye_deskew(training, byte_index, mode)) {
			ddr_adjust_set_val(training->base_phy,
				byte_index, def_val, mode);
			/* MUST deskew dataeye after restore rdqs */
			ddr_dataeye_deskew(training, byte_index, mode);
			DDR_ERROR("Byte[%x] deskew fail, restore[%x].",
					byte_index, def_val);
			break;
		}
	}
}

/**
 * ddr_adjust_byte
 * @training
 * @byte_index
 * @mode
 *
 * Adjust specified byte winodw to middle.
 */
static void ddr_adjust_byte(struct training_data *training,
			unsigned int byte_index, unsigned int mode)
{
	unsigned int trend = ddr_adjust_trend_check(training->base_phy,
			byte_index, 0, mode);

	/* window on left, move to right */
	if (DDR_WIN_LEFT == trend)
		ddr_adjust_move_win(training, byte_index,
			DDR_DQS_ADJ_STEP, DDR_WIN_RIGHT, mode);
	/* window on right, move to left */
	else if (DDR_WIN_RIGHT == trend)
		ddr_adjust_move_win(training, byte_index, DDR_DQS_ADJ_STEP,
							DDR_WIN_LEFT, mode);
	/* window on middle, no need to move */
	else
		DDR_DEBUG("PHY[%x] Byte[%x] mode[%x] win on middle.",
					training->base_phy, byte_index, mode);
}

/**
 * ddr_adjust_dataeye
 * @training
 * @mode
 *
 * Adjust PHY dataeye. On normal case,
 * read dateeye window on left after read dataeye hardware training,
 * write dataeye window on left after write leveling training.
 *
 */
void ddr_adjust_dataeye(struct training_data *training, unsigned int mode)
{
	int i;

	DDR_DEBUG("DDR dataeye adjust.");
	/* dataeye adjust disable */
	if (ddr_training_check_bypass(DDR_BYPASS_DATAEYE_ADJ_MASK))
		return;

	for (i = 0; i < training->ddr_byte_num; i++)
		ddr_adjust_byte(training, i, mode);
}
#else
void ddr_adjust_dataeye(struct training_data *training, unsigned int mode)
{ return; }
#endif /* DDR_TRAINING_ADJUST_CONFIG */

#define __dataeye_training__
#ifdef DDR_DATAEYE_TRAINING_CONFIG
/**
 * ddr_dataeye_check_dq
 * @training
 * @byte_index
 * @dq_index
 *
 * Check dataeye dq.
 */
int ddr_dataeye_check_dq(const struct training_data *training,
	unsigned int byte_index, unsigned int dq_index)
{
	if (DDR_CHECK_TYPE_DDRT == training->dq_check_type)
		return ddr_ddrt_check(training->ddrt_reversed_data, byte_index,
							dq_index);
	else if (DDR_CHECK_TYPE_MPR == training->dq_check_type)
		return ddr_mpr_check(training->base_dmc, byte_index, dq_index);
	else
		DDR_ERROR("DDR dataeye dq check type not set.");

	return 0;
}

/**
 * ddr_dataeye_check_dir
 * @direction
 * @left
 * @right
 * @mask
 * @training
 * @byte_index
 * @dq_index
 * @mode
 *
 * Check dq whether valid and set mask to reduce search time.
 */
static int ddr_dataeye_check_dir(unsigned int direction, unsigned int left,
	unsigned int right, unsigned int *mask,
	struct training_data *training, unsigned int byte_index,
	unsigned int dq_index, unsigned int mode)
{
	int result = 0;

	result = ddr_dataeye_check_dq(training, byte_index, dq_index);
	switch (direction) {
	case DDR_FIND_DQ_BOTH:
		*mask = DDR_FIND_DQ_LEFT | DDR_FIND_DQ_RIGHT;
		break;
	case DDR_FIND_DQ_LEFT:
		if (result) {
			/* ddr test error, search opposite side */
			*mask = DDR_FIND_DQ_RIGHT;
		} else {		/* ddr test ok */
			ddr_phy_set_dq_bdl(training->base_phy,
				left, byte_index, dq_index, mode);
			if (!ddr_dataeye_check_dq(training,
				byte_index, dq_index))
				/* test ok, go on search this side */
				*mask = DDR_FIND_DQ_LEFT;
		}
		break;
	case DDR_FIND_DQ_RIGHT:
		if (result) {  /* ddr test error, search opposite side */
			*mask = DDR_FIND_DQ_LEFT;
		} else {		/* ddr test ok */
			ddr_phy_set_dq_bdl(training->base_phy,
				right, byte_index,
				dq_index, mode);
			if (!ddr_dataeye_check_dq(training,
					byte_index, dq_index))
				/* test OK, go on search this side */
				*mask = DDR_FIND_DQ_RIGHT;
		}
		break;
	default:
		break;
	}

	return result;
}

/**
 * ddr_dataeye_search_dq
 * @left
 * @right
 * @target
 * @direction
 * @training
 * @byte_index
 * @dq_index
 * @mode
 *
 * Binary search the valid dq bdl.
 */
static void ddr_dataeye_search_dq(unsigned int left, unsigned int right,
	int *target, unsigned int direction,  struct training_data *training,
	unsigned int byte_index, unsigned int dq_index, unsigned int mode)

{
	unsigned int middle;
	unsigned int mask = 0;

	middle = left + ((right - left) >> 1);

	ddr_phy_set_dq_bdl(training->base_phy, middle,
		byte_index, dq_index, mode);
	if (!ddr_dataeye_check_dir(direction, left, right, &mask, training,
			byte_index, dq_index, mode)) { /* test ok*/

		*target = (int)middle;
		return;
	}

	if (left == middle || middle == right)  /*not found*/
		return;

	/*find left side*/
	if (DDR_FIND_DQ_LEFT & mask)
		ddr_dataeye_search_dq(left, middle, target, direction,
			training, byte_index, dq_index, mode);

	/*find right side*/
	if (DDR_FIND_DQ_RIGHT & mask)
		ddr_dataeye_search_dq(middle, right, target, direction,
			training, byte_index, dq_index, mode);

	return;
}

/**
 * ddr_dataeye_find_dq
 * @training
 * @byte_index
 * @dq_index
 * @mode
 *
 * Find DQ valid range.
 */
static void ddr_dataeye_find_dq(struct training_data *training,
	unsigned int byte_index, unsigned int dq_index, unsigned int mode)
{
	int cur_dq, left_dq, right_dq, def_dq;
	unsigned int dq_num;
	unsigned int win_num;

	dq_num = (byte_index << 3) + dq_index;
	def_dq = ddr_phy_get_dq_bdl(training->base_phy,
			byte_index, dq_index, mode);
	cur_dq = def_dq;

	/* check default dq */
	if (ddr_dataeye_check_dq(training, byte_index, dq_index)) {
		/* test error */
		cur_dq = -1;
		ddr_dataeye_search_dq(0, PHY_BDL_MASK, &cur_dq,
			DDR_FIND_DQ_BOTH, training, byte_index,
			dq_index, mode);
		DDR_DEBUG("DQ[%x] def[%x] nok, find new value[%x]",
					dq_num, def_dq, cur_dq);
		if (-1 == cur_dq) {  /* no valid dq */
			training->ddr_bit_result[dq_num] = 0;
			training->ddr_bit_best[dq_num]   = 0;
			/* restore default value */
			ddr_phy_set_dq_bdl(training->base_phy,
				def_dq, byte_index, dq_index, mode);
			DDR_WARNING("DQ[%x] not found dq. restore[%x]",
						dq_num, def_dq);
			return;
		}
	}

	/* find the left boundary */
	left_dq = cur_dq;
	ddr_dataeye_search_dq(0, cur_dq, &left_dq,
		DDR_FIND_DQ_LEFT, training, byte_index, dq_index, mode);
	while (left_dq > 0) {
		left_dq--;
		ddr_phy_set_dq_bdl(training->base_phy, left_dq, byte_index,
							dq_index, mode);
		if (ddr_dataeye_check_dq(training, byte_index, dq_index)) {
			/* test error */
			left_dq++;
			break;
		}
	}

	/*find the right boundary*/
	right_dq = cur_dq;
	ddr_dataeye_search_dq(cur_dq, PHY_BDL_MASK, &right_dq,
		DDR_FIND_DQ_RIGHT, training, byte_index, dq_index, mode);
	while (right_dq < PHY_BDL_MASK) {
		right_dq++;
		ddr_phy_set_dq_bdl(training->base_phy, right_dq, byte_index,
							dq_index, mode);
		if (ddr_dataeye_check_dq(training, byte_index, dq_index)) {
			/* test error */
			right_dq--;
			break;
		}
	}

	/*reset dq*/
	ddr_phy_set_dq_bdl(training->base_phy, def_dq,
		byte_index, dq_index, mode);

	/**
	 * 0 1 2 3 4 5 6 7 8 9
	 * x x - - - - - x x x
	 *     |       |
	 * left_dq   right_dq
	 *
	 * so left_dq = 2, right_dq = 6
	 */

	/*set result */
	win_num = right_dq - left_dq + 1;
	training->ddr_bit_result[dq_num] = (left_dq << DDR_DATAEYE_RESULT_BIT
		| right_dq);
	training->ddr_bit_best[dq_num] = (win_num << DDR_DATAEYE_RESULT_BIT)
		| ((win_num >> 1) + left_dq);

	DDR_INFO("DQ[%x] range: left[%x] right[%x]. best[%x] mode[%x]", dq_num,
		left_dq, right_dq, training->ddr_bit_best[dq_num], mode);
}


/**
 * ddr_dataeye_deskew
 * @training
 * @byte_index
 * @mode
 *
 * DDR dataeye training one byte.
 */
int ddr_dataeye_deskew(struct training_data *training,
	unsigned int byte_index, unsigned int mode)
{
	unsigned int dq_num;
	unsigned int loop_times = 0;
	unsigned int win_num, dq_sum;
	unsigned int def_dq, best_dq;
	int i;

	dq_sum = 0;
	training->ddr_win_sum = 0;
	for (i = 0; i < DDR_PHY_BIT_NUM; i++) {
		dq_num = (byte_index << 3) + i;
		def_dq = ddr_phy_get_dq_bdl(training->base_phy,
			byte_index, i, mode);
		ddr_dataeye_find_dq(training, byte_index, i, mode);
		win_num = training->ddr_bit_best[dq_num]
			>> DDR_DATAEYE_RESULT_BIT;
		best_dq = training->ddr_bit_best[dq_num]
			& DDR_DATAEYE_RESULT_MASK;
		/* check window number */
		if (win_num < DDR_DATAEYE_WIN_NUM) {
			if (loop_times < DDR_LOOP_TIMES_LMT) {
				loop_times++;
				i--;
				continue;
			} else{
				if (win_num == 0) {
					DDR_WARNING("Byte[%x] DQ[%x] no win.",
						byte_index, dq_num);
					/* restore default value */
					ddr_phy_set_dq_bdl(training->base_phy,
						def_dq, byte_index, i, mode);
					ddr_training_stat(DDR_ERR_DATAEYE,
						training->base_phy,
						byte_index, i);
					return -1;
				}
			}
		}

		loop_times = 0;
		ddr_phy_set_dq_bdl(training->base_phy, best_dq,
			byte_index, i, mode);
		dq_sum = dq_sum + best_dq;
		training->ddr_win_sum = training->ddr_win_sum + win_num;
	}

	dq_sum = dq_sum >> 3;

	/* only DDR_MODE_WRITE need to set */
	if (DDR_MODE_WRITE == mode)
		REG_WRITE(dq_sum, training->base_phy
			+ DDR_PHY_DXNWDQNBDL2(byte_index));

	ddr_phy_cfg_update(training->base_phy);
	return 0;
}

/**
 * ddr_dataeye_process
 * @training
 * @ddrtr_result
 * @adjust
 * @mode
 *
 * DDR write or read dataeye training.
 */
static int ddr_dataeye_process(struct training_data *training,
	void *ddrtr_result, unsigned int adjust, unsigned int mode)
{
	int result = 0;
	int i;

	/* dataeye training */
	for (i = 0; i < training->ddr_byte_num; i++)
		result += ddr_dataeye_deskew(training, i, mode);

	if (result) {
		result = -1;
		DDR_ERROR("PHY[%x] mode[%x] dataeye training fail.",
				training->base_phy, mode);
	} else if (DDR_TRUE == adjust) {
		/* dataeye training result adjust */
		ddr_adjust_dataeye(training, mode);
	}

	/* save training result to printf */
	ddr_result_data_save(ddrtr_result, training, mode);

	return result;
}

/**
 * ddr_dataeye_training
 * @base_dmc
 * @base_phy
 * @ddrtr_result
 * @adjust
 *
 * DDR dataeye training.
 */
int ddr_dataeye_training(unsigned int base_dmc, unsigned int base_phy,
	void *ddrtr_result, unsigned int adjust)
{
	struct training_data tmp_result;
	struct training_data *training = &tmp_result;
	int result_read, result_write;

	DDR_DEBUG("DDR dataeye training.");

	training->ddr_byte_num       = ddr_phy_get_byte_num(base_dmc);
	training->base_phy           = base_phy;
	training->base_dmc           = base_dmc;
	training->ddrt_reversed_data = ddr_dmc_get_reversed(base_dmc);
	training->dq_check_type      = DDR_CHECK_TYPE_DDRT;

	/* write dataeye training */
	result_write = ddr_dataeye_process(training, ddrtr_result,
		adjust, DDR_MODE_WRITE);

	/* read dataeye training */
	result_read = ddr_dataeye_process(training, ddrtr_result,
			adjust, DDR_MODE_READ);

	if (result_read || result_write)
		return -1;
	else
		return 0;
}
#endif  /* DDR_DATAEYE_TRAINING_CONFIG */

#define __hardware_training__
#ifdef DDR_HW_TRAINING_CONFIG
#ifdef DDR_HW_READ_ADJ_CONFIG
/**
 * ddr_hw_read_adj
 * @base_phy
 * @byte_num
 *
 * Adjust rdqs and dq after hw read training.
 * When define DDR_TRAINING_ADJUST_DISABLE, MUST define DDR_HW_READ_ADJ_CONFIG.
 */
static void ddr_hw_read_adj(unsigned int base_phy, unsigned int byte_num)
{
	int i;

	DDR_DEBUG("DDR hw read adjust.");
	/* check hw read adjust bypass bit */
	if (ddr_training_check_bypass(DDR_BYPASS_HW_ADJ_MASK))
		return;

	/* assume read dataeye window on left */
	for (i = 0; i < byte_num; i++) {
		REG_WRITE(REG_READ(base_phy + DDR_PHY_DXNRDQNBDL0(i))
			+ PHY_DQ_MIDDLE_VAL,
			base_phy + DDR_PHY_DXNRDQNBDL0(i));
		REG_WRITE(REG_READ(base_phy + DDR_PHY_DXNRDQNBDL1(i))
			+ PHY_DQ_MIDDLE_VAL,
			base_phy + DDR_PHY_DXNRDQNBDL1(i));
		REG_WRITE(REG_READ(base_phy + DDR_PHY_DXNRDQSDLY(i))
			+ PHY_RDQS_MIDDLE_VAL,
			base_phy + DDR_PHY_DXNRDQSDLY(i));
	}
}
#else
static void ddr_hw_read_adj(unsigned int base_phy, unsigned int byte_num) {}
#endif /* DDR_HW_READ_ADJ_CONFIG */
/**
 * ddr_hw_dataeye_read
 * @base_phy
 * @byte_num
 *
 * Dataeye hardware training.
 */
static int ddr_hw_dataeye_read(unsigned int base_phy,
				unsigned int byte_num)
{
	int i;
	unsigned int count = 0;

	/* clear */
	for (i = 0; i < byte_num; i++) {
		REG_WRITE(0, base_phy + DDR_PHY_DXNRDQNBDL0(i));
		REG_WRITE(0, base_phy + DDR_PHY_DXNRDQNBDL1(i));
		REG_WRITE(0, base_phy + DDR_PHY_DXNRDQSDLY(i));
	}

	ddr_phy_cfg_update(base_phy);

	/* hardware read dataeye training enable */
	REG_WRITE(PHY_PHYINITCTRL_RDET_EN
			| PHY_PHYINITCTRL_INIT_EN,
			base_phy + DDR_PHY_PHYINITCTRL);

	/* auto cleared to 0 after training finished */
	while (count < DDR_HWR_WAIT_TIMEOUT) {
		if (!(REG_READ(base_phy + DDR_PHY_PHYINITCTRL)
			& PHY_PHYINITCTRL_MASK))
			break;

		count++;
	}

	if (count >= DDR_HWR_WAIT_TIMEOUT)
		DDR_ERROR("HWR wait timeout.");

	if (REG_READ(base_phy + DDR_PHY_PHYINITSTATUS)
				& PHY_PHYINITSTATUS_RDET_ERR) {
		/* read dataeye calibration error */
		DDR_FATAL("Phy[%x] hw read dataeye fail", base_phy);
		ddr_training_stat(DDR_ERR_HW_RD_DATAEYE, base_phy, -1, -1);
		return -1;
	}

	ddr_hw_read_adj(base_phy, byte_num);

	return 0;
}

/**
 * ddr_hw_training
 * @base_dmc
 * @base_phy
 *
 * DDR hardware training.
 */
int ddr_hw_training(unsigned int base_dmc, unsigned int base_phy)
{
	DDR_DEBUG("DDR hardware training.");

	/* hardware read dataeye training */
	if (ddr_hw_dataeye_read(base_phy, ddr_phy_get_byte_num(base_dmc)))
		return -1;

	return 0;
}
#endif /* DDR_HW_TRAINING_CONFIG */

#define __mpr_training__
#ifdef DDR_MPR_TRAINING_CONFIG
/**
 * ddr_mpr_switch
 * @base_dmc
 * @base_phy
 * @val
 *
 * Switch MPR function.
 */
static void ddr_mpr_switch(unsigned int base_dmc, int val)
{
	unsigned int sfc_cmd;
	if (DDR_TRUE == val)
		sfc_cmd = (DMC_CMD_MRS_MR3 << DMC_SFC_CMD_MRS_BIT)
			| DMC_CMD_TYPE_LMR;
	else
		sfc_cmd = DMC_CMD_TYPE_LMR;

	ddr_dmc_sfc_cmd(base_dmc, sfc_cmd, 0x0, DMC_BANK_MR3);

	/* clear */
	if (DDR_FALSE == val) {
		REG_WRITE(0x0, base_dmc + DDR_DMC_SFCBANK);
		REG_WRITE(0x0, base_dmc + DDR_DMC_SFCREQ);
	}
}

/**
 * ddr_mpr_judge
 * @data1
 * @data2
 * @data3
 * @data4
 * @dq_index
 *
 * Judge MPR data.
 */
static int ddr_mpr_judge(unsigned int data1, unsigned int data2,
	unsigned int data3, unsigned int data4, unsigned int dq_index)
{
	/* check byte */
	if (-1 == dq_index) {
		if (DDR_MPR_BYTE_MASK == data1 && 0x0 == data2
			&& DDR_MPR_BYTE_MASK == data3 && 0x0 == data4)
			return 0;
		else
			return -1;
	} else {
		/* check DQ */
		data1 = (data1 >> dq_index) & DDR_MPR_BIT_MASK;
		data2 = (data2 >> dq_index) & DDR_MPR_BIT_MASK;
		data3 = (data3 >> dq_index) & DDR_MPR_BIT_MASK;
		data4 = (data4 >> dq_index) & DDR_MPR_BIT_MASK;
		if (DDR_MPR_BIT_MASK == data1 && 0x0 == data2
			&& DDR_MPR_BIT_MASK == data3 && 0x0 == data4)
			return 0;
		else
			return -1;
	}
}

/**
 * ddr_mpr_extract
 * @base_dmc
 * @byte_index
 * @dq_index
 * @offset0
 * @offset1
 * @offset2
 * @offset3
 *
 * Extract MPR read data to judge.
 */
static int ddr_mpr_extract(unsigned int base_dmc, unsigned int byte_index,
	int dq_index, unsigned int offset0, unsigned int offset1,
	unsigned int offset2, unsigned int offset3)
{
	unsigned int data1, data2, data3, data4;

	data1 = REG_READ(base_dmc + offset0);  /* [127:96] or [255:224] */
	data2 = REG_READ(base_dmc + offset1);  /* [95:64] or [223:192] */
	data3 = REG_READ(base_dmc + offset2);  /* [63:32] or [191:160] */
	data4 = REG_READ(base_dmc + offset3);  /* [31:0] or [159:128] */

	DDR_INFO("byte[%x] data[%x=%x][%x=%x][%x=%x][%x=%x]",
		byte_index,
		base_dmc + offset0, data1, base_dmc + offset1, data2,
		base_dmc + offset2, data3, base_dmc + offset3, data4);

	if (DDR_PHY_BYTE_MAX == ddr_phy_get_byte_num(base_dmc)) {
		/* four byte: data1[0xFFFFFFFF] data2[0x00000000]
		data3[0xFFFFFFFF] data4[0x00000000] */
		data1 = (data1 >> (byte_index << 3)) & DDR_MPR_BYTE_MASK;
		data2 = (data2 >> (byte_index << 3)) & DDR_MPR_BYTE_MASK;
		data3 = (data3 >> (byte_index << 3)) & DDR_MPR_BYTE_MASK;
		data4 = (data4 >> (byte_index << 3)) & DDR_MPR_BYTE_MASK;
	} else {
		/* two byte: data1[0xFFFF0000] data2[0xFFFF0000]
		data3[0xFFFF0000] data4[0xFFFF0000] */
		data1 = ((data1 >> DDR_MPR_BYTE_BIT) >> (byte_index << 3))
				& DDR_MPR_BYTE_MASK;
		data2 = (data2 >> (byte_index << 3)) & DDR_MPR_BYTE_MASK;
		data3 = ((data3 >> DDR_MPR_BYTE_BIT) >> (byte_index << 3))
				& DDR_MPR_BYTE_MASK;
		data4 = (data4 >> (byte_index << 3)) & DDR_MPR_BYTE_MASK;
		if (ddr_mpr_judge(data1, data2, data3, data4, dq_index))
			return -1;

		/* two byte need to swap data and check again */
		data1 = ((REG_READ(base_dmc + DDR_DMC_SFC_RDATA1)
				>> DDR_MPR_BYTE_BIT) >> (byte_index << 3))
				& DDR_MPR_BYTE_MASK;
		data2 = (REG_READ(base_dmc + DDR_DMC_SFC_RDATA0)
				>> (byte_index << 3)) & DDR_MPR_BYTE_MASK;
		data3 = ((REG_READ(base_dmc + DDR_DMC_SFC_RDATA3)
			>> DDR_MPR_BYTE_BIT) >> (byte_index << 3))
			& DDR_MPR_BYTE_MASK;
		data4 = (REG_READ(base_dmc + DDR_DMC_SFC_RDATA2)
			>> (byte_index << 3)) & DDR_MPR_BYTE_MASK;
	}

	return ddr_mpr_judge(data1, data2, data3, data4, dq_index);
}

/**
 * ddr_mpr_check
 * @base_dmc
 * @byte_index
 * @dq_index
 *
 * Check MPR read data.
 */
int ddr_mpr_check(unsigned int base_dmc, unsigned int byte_index,
				int dq_index)
{
	/* read data */
	ddr_dmc_sfc_cmd(base_dmc, DMC_CMD_TYPE_READ, 0x0, 0x0);
	return DMC_MPR_CHECK_BIT_0_127(base_dmc, byte_index, dq_index);
}

/**
 * ddr_mpr_find_rdq
 * @base_dmc
 * @base_phy
 *
 * Find RDQ via MPR.
 */
static int ddr_mpr_find_rdq(unsigned int base_dmc, unsigned int base_phy)
{
	struct training_data tmp_result;
	struct training_data *training = &tmp_result;
	unsigned int dq_num;
	unsigned int win_num;
	unsigned int def_dq, best_dq;
	unsigned int byte_index, dq_index;

	training->ddr_byte_num       = ddr_phy_get_byte_num(base_dmc);
	training->base_phy           = base_phy;
	training->base_dmc           = base_dmc;
	training->ddrt_reversed_data = ddr_dmc_get_reversed(base_dmc);
	/* find rdq via mpr */
	training->dq_check_type      = DDR_CHECK_TYPE_MPR;

	/* find rdq */
	for (byte_index = 0;
		byte_index < training->ddr_byte_num; byte_index++) {
		for (dq_index = 0; dq_index < DDR_PHY_BIT_NUM; dq_index++) {
			dq_num = (byte_index << 3) + dq_index;
			def_dq = ddr_phy_get_dq_bdl(training->base_phy,
				byte_index, dq_index, DDR_MODE_READ);
			ddr_dataeye_find_dq(training, byte_index,
				dq_index, DDR_MODE_READ);
			win_num = training->ddr_bit_best[dq_num]
				>> DDR_DATAEYE_RESULT_BIT;
			best_dq = training->ddr_bit_best[dq_num]
				& DDR_DATAEYE_RESULT_MASK;
			if (win_num > 0)
				ddr_phy_set_dq_bdl(training->base_phy, best_dq,
					byte_index, dq_index, DDR_MODE_READ);
			else {
				/* In normal case, not reach here */
				/* restore default value */
				ddr_phy_set_dq_bdl(training->base_phy, def_dq,
					byte_index, dq_index, DDR_MODE_READ);

				DDR_FATAL("PHY[%x] Byte[%x] DQ[%x] MPR fail",
					base_phy, byte_index, dq_index);
				ddr_training_stat(DDR_ERR_MPR, base_phy,
					byte_index, dq_index);
				return -1;
			}
		}
	}

	return 0;

}

/**
 * ddr_mpr_find_rdqs
 * @base_dmc
 * @base_phy
 * @byte_index
 *
 * Find RDQS via MPR.
 */
static int ddr_mpr_find_rdqs(unsigned int base_dmc, unsigned int base_phy,
	unsigned int byte_index)
{
	unsigned int rdqs_start = 0;
	unsigned int rdqs_end   = PHY_RDQS_BDL_MASK;
	unsigned int rdqs_mid;
	unsigned int val, delay;
	unsigned int count = 0;
	int found = DDR_FALSE;

	/* set rdq to middle value */
	REG_WRITE(PHY_DQ_MIDDLE_VAL,
	base_phy + DDR_PHY_DXNRDQNBDL0(byte_index));
	REG_WRITE(PHY_DQ_MIDDLE_VAL,
		base_phy + DDR_PHY_DXNRDQNBDL1(byte_index));

	/* clear rdqs */
	delay    = REG_READ(base_phy + DDR_PHY_DXNRDQSDLY(byte_index));
	rdqs_mid = delay;   /* if not found, restore default value */
	delay    = delay & (~PHY_RDQS_BDL_MASK);

	/* find rdqs */
	for (val = 0; val <= PHY_RDQS_BDL_MASK; val++) {
		REG_WRITE(delay | val,
			base_phy + DDR_PHY_DXNRDQSDLY(byte_index));
		ddr_phy_cfg_update(base_phy);
		/* check ok */
		if (!ddr_mpr_check(base_dmc, byte_index, -1)) {
			if (DDR_FALSE == found) {
				rdqs_start = val; /* found start value */
				count++;
				if (DDR_MPR_RDQS_FIND_TIMES == count)
					found = DDR_TRUE;
			}
		} else {
			if (DDR_TRUE == found) {
				rdqs_end = val;  /* found end value */
				break;
			}
		}
	}

	if (DDR_TRUE == found) {
		rdqs_mid = ((rdqs_end - rdqs_start) >> 1) + rdqs_start;
		DDR_INFO("PHY[%x] Byte[%x] rdqs_middle[%x]",
				base_phy, byte_index, rdqs_mid);
		DDR_INFO("rdqs_start[%x] rdqs_end[%x]",
				rdqs_start, rdqs_end);
	} else {
		DDR_FATAL("PHY[%x] Byte[%x] not find RDQS, restore.",
			base_phy, byte_index);
		ddr_training_stat(DDR_ERR_MPR, base_phy,
					byte_index, -1);
	}

	REG_WRITE(delay | rdqs_mid, base_phy + DDR_PHY_DXNRDQSDLY(byte_index));
	ddr_phy_cfg_update(base_phy);

	return ((DDR_TRUE == found) ? 0 : -1);
}

/**
 * ddr_mpr_training
 * @base_dmc
 * @base_phy
 *
 * Multi Purpose Register(MPR)
 */
int ddr_mpr_training(unsigned int base_dmc, unsigned int base_phy)
{
	int i;
	int result = 0;
	unsigned int byte_num = ddr_phy_get_byte_num(base_dmc);
	unsigned int mr0;
	unsigned int sfc_cmd;

	DDR_DEBUG("DDR MPR training.");

	/* set DDR bust */
	if (DDR_PHY_BYTE_MAX == byte_num) {
		mr0 = (REG_READ(base_phy + DDR_PHY_MODEREG01)
			& DMC_MRS_MASK)
			& (~DMC_MR0_BL_MASK);
		sfc_cmd = ((mr0 | DMC_MR0_BL_BUST4)
			<< DMC_SFC_CMD_MRS_BIT)
			| DMC_CMD_TYPE_LMR;
		ddr_dmc_sfc_cmd(base_dmc, sfc_cmd, 0x0, 0x0);
	}

	/* precharge all */
	ddr_dmc_sfc_cmd(base_dmc, DMC_CMD_TYPE_PRECHARGE_ALL, 0x0, 0x0);

	/* enable MPR */
	ddr_mpr_switch(base_dmc, DDR_TRUE);

	/* find rdqs */
	for (i = 0; i < byte_num; i++)
		result += ddr_mpr_find_rdqs(base_dmc, base_phy, i);

	/* find rdq */
	if (!result)
		result = ddr_mpr_find_rdq(base_dmc, base_phy);

	/* disable MPR */
	ddr_mpr_switch(base_dmc, DDR_FALSE);

	/* restore DDR bust */
	if (DDR_PHY_BYTE_MAX == byte_num) {
		mr0 = (REG_READ(base_phy + DDR_PHY_MODEREG01)
			& DMC_MRS_MASK);
		sfc_cmd = (mr0 << DMC_SFC_CMD_MRS_BIT)
			| DMC_CMD_TYPE_LMR;
		ddr_dmc_sfc_cmd(base_dmc, sfc_cmd, 0x0, 0x0);
	}
	return result;
}
#else
int ddr_mpr_check(unsigned int base_dmc, unsigned int byte_index,
			int dq_index) { return 0; }
#endif /* DDR_MPR_TRAINING_CONFIG */

#define __vref_training__
#ifdef DDR_VREF_TRAINING_CONFIG
#ifdef DDR_VREF_WITHOUT_BDL_CONFIG
/**
 * ddr_vref_save_bdl
 * @base_phy
 * @bytenum
 * @mode
 * @dq_data
 *
 * Save dataeye dq bdl before vref training.
 */
static void ddr_vref_save_bdl(unsigned int base_phy, unsigned int bytenum,
	unsigned int mode, struct tr_dq_data *dq_data)
{
	int i;

	for (i = 0; i < bytenum; i++) {
		if (DDR_MODE_WRITE == mode) {
			dq_data->dq03[i] = REG_READ(base_phy
				+ DDR_PHY_DXNWDQNBDL0(i));
			dq_data->dq47[i] = REG_READ(base_phy
				+ DDR_PHY_DXNWDQNBDL1(i));
			dq_data->wdm[i] = REG_READ(base_phy
				+ DDR_PHY_DXNWDQNBDL2(i));
		} else {
			dq_data->dq03[i] = REG_READ(base_phy
				+ DDR_PHY_DXNRDQNBDL0(i));
			dq_data->dq47[i] = REG_READ(base_phy
				+ DDR_PHY_DXNRDQNBDL1(i));
		}
	}
}

/**
 * ddr_vref_restore_bdl
 * @base_phy
 * @bytenum
 * @mode
 * @dq_data
 * @training
 *
 * Restore dataeye dq bdl after vref training.
 */
static void ddr_vref_restore_bdl(unsigned int base_phy, unsigned int bytenum,
	unsigned int mode, struct tr_dq_data *dq_data,
	struct training_data *training)
{
	int i, j;
	unsigned int bdl;

	for (i = 0; i < bytenum; i++) {
		if (DDR_MODE_WRITE == mode) {
			REG_WRITE(dq_data->dq03[i],
				base_phy + DDR_PHY_DXNWDQNBDL0(i));
			REG_WRITE(dq_data->dq47[i],
				base_phy + DDR_PHY_DXNWDQNBDL1(i));
			REG_WRITE(dq_data->wdm[i],
				base_phy + DDR_PHY_DXNWDQNBDL2(i));
		} else {
			REG_WRITE(dq_data->dq03[i],
				base_phy + DDR_PHY_DXNRDQNBDL0(i));
			REG_WRITE(dq_data->dq47[i],
				base_phy + DDR_PHY_DXNRDQNBDL1(i));
		}
		for (j = 0; j < DDR_PHY_BIT_NUM; j++) {
			if (j < 4)
				bdl = dq_data->dq03[i];
			else
				bdl = dq_data->dq47[i];

			training->ddr_bit_best[(i << 3) + j] =
			(training->ddr_bit_best[(i << 3) + j]
			 & (~DDR_DATAEYE_RESULT_MASK))
			| ((bdl >> ((j & 0x3) << 3)) & PHY_BDL_MASK);
		}
	}
}
#else
static void ddr_vref_save_bdl(unsigned int base_phy, unsigned int bytenum,
	unsigned int mode, struct tr_dq_data *dq_data)
{
}
static void ddr_vref_restore_bdl(unsigned int base_phy, unsigned int bytenum,
	unsigned int mode, struct tr_dq_data *dq_data,
	struct training_data *training)
{
}
#endif /* DDR_VREF_WITHOUT_BDL_CONFIG */

/**
 * ddr_vref_set
 * @base_phy
 * @byte_index
 * @val
 * @mode
 * @bytenum
 *
 * Set DDR Vref value.
 */
static void ddr_vref_set(unsigned int base_phy, unsigned int val,
	unsigned int byte_index, unsigned int mode, unsigned int bytenum)
{
	if (DDR_MODE_READ == mode) { /* HOST vref */
		DDR_PHY_VREF_HOST_SET(base_phy, val, bytenum);
	} else {	  /*DRAM vref */
		DDR_PHY_VREF_DRAM_SET(base_phy, val, byte_index);
	}
	DDR_INFO("byte[%x] mode[%x] set vref [%x]", byte_index, mode, val);
}

/**
 * ddr_vref_get
 * @base_phy
 * @byte_index
 * @mode
 *
 * Get DDR Vref value.
 */
static unsigned int ddr_vref_get(unsigned int base_phy,
	unsigned int byte_index, unsigned int mode)
{
	unsigned int val = 0;

	if (DDR_MODE_READ == mode) {	/*HOST vref */
		DDR_PHY_VREF_HOST_GET(base_phy, val);
	} else {      /*DRAM vref */
		DDR_PHY_VREF_DRAM_GET(base_phy, val, byte_index);
	}
	DDR_INFO("byte[%x] mode[%x] get vref [%x]", byte_index, mode, val);
	return val;
}

/**
 * ddr_vref_get_win
 * @training
 * @byte_index
 * @mode
 * @vref
 *
 * Get totol win number of training result.
 */
static unsigned int ddr_vref_get_win(struct training_data *training,
	unsigned int byte_index, unsigned int mode, int vref)
{
	unsigned int vref_min = DDR_VREF_HOST_VAL_MIN;
	unsigned int vref_max = DDR_VREF_HOST_VAL_MAX;
	int vref_set;

	training->ddr_win_sum = 0;

	if (DDR_MODE_WRITE == mode) {
		vref_min = DDR_VREF_DRAM_VAL_MIN;
		vref_max = DDR_VREF_DRAM_VAL_MAX;
	}

	if (vref < vref_min)
		vref_set = vref_min;
	else if (vref > vref_max)
		vref_set = vref_max;
	else
		vref_set = vref;

	ddr_vref_set(training->base_phy, vref_set, byte_index, mode,
		training->ddr_byte_num);

	ddr_dataeye_deskew(training, byte_index, mode);

	return training->ddr_win_sum;
}

/**
 * ddr_vref_find_best
 * @training
 * @byte_index
 * @mode
 * @vref
 * @step
 *
 * Find the best vref which win number is max.
 */
static unsigned int ddr_vref_find_best(struct training_data *training,
	unsigned int byte_index, unsigned int mode, unsigned int vref,
	int step)
{
	int cur_vref;
	unsigned int best_vref;
	unsigned int cur_win;
	unsigned int max_win;
	unsigned int lower_times = 0;
	unsigned int vref_min = DDR_VREF_HOST_VAL_MIN;
	unsigned int vref_max = DDR_VREF_HOST_VAL_MAX;

	if (DDR_MODE_WRITE == mode) {
		vref_min = DDR_VREF_DRAM_VAL_MIN;
		vref_max = DDR_VREF_DRAM_VAL_MAX;
	}

	max_win   = training->ddr_win_sum;
	cur_vref  = vref + step;

	if (vref < vref_min)
		best_vref = vref_min;
	else if (vref > vref_max)
		best_vref = vref_max;
	else
		best_vref = vref;

	/*find parabola vertex */
	while (cur_vref >= vref_min
		&& cur_vref <= vref_max) {
		cur_win = ddr_vref_get_win(training, byte_index,
			mode, cur_vref);
		DDR_INFO("PHY[%x] byte[%x] vref[%x] win[%x] mode[%x]",
			training->base_phy, byte_index, cur_vref,
			cur_win, mode);
		if (cur_win < max_win) {
			lower_times++;
			if (DDR_VREF_COMPARE_TIMES == lower_times) {
				/* Continuous decline, mean found vertex */
				break;
			}
		} else {
			lower_times = 0;
			max_win =  cur_win;
			best_vref = cur_vref;
		}
		cur_vref = cur_vref + step;
	}

	return best_vref;
}

/**
 * ddr_vref_cal
 * @training
 * @byte_index
 * @mode
 *
 * DDR Vref calibrate and set the best value.
 */
static void ddr_vref_cal(struct training_data *training,
	unsigned int byte_index, unsigned int mode)
{
	unsigned int def_vref;
	unsigned int best_vref;
	unsigned int def_win;
	unsigned int left_win;
	unsigned int right_win;

	def_vref  = ddr_vref_get(training->base_phy, byte_index, mode);
	def_win   = ddr_vref_get_win(training, byte_index, mode, def_vref);
	left_win  = ddr_vref_get_win(training, byte_index, mode, def_vref - 1);
	right_win = ddr_vref_get_win(training, byte_index, mode, def_vref + 1);

	DDR_INFO("PHY[%x] byte[%x] vref[%x] win[%x][%x][%x] mode[%x]",
		training->base_phy, byte_index, def_vref, left_win,
		def_win, right_win, mode);

	/* With vref increments, WIN number is a parabola.
	   So firstly determine the result on left or right.*/
	/* parabola vertex */
	if (def_win > left_win && def_win > right_win) {
		best_vref = def_vref;
	} else if (left_win < right_win) { /* the result on right*/
		best_vref = ddr_vref_find_best(training,
			byte_index, mode, def_vref + 1, 1);
	} else if (left_win > right_win) { /* the result on left*/
		best_vref = ddr_vref_find_best(training,
			byte_index, mode, def_vref - 1, -1);
	} else {
		/* when [-1, 1] cat not determine the direction,
		   the cur vref should be or very close to best vref */
		best_vref = def_vref;
	}

	ddr_vref_set(training->base_phy, best_vref, byte_index, mode,
		training->ddr_byte_num);
}

/**
 * ddr_vref_training
 * @base_dmc
 * @base_phy
 * @mode
 *
 * DDR vref training.
 */
int ddr_vref_training(unsigned int base_dmc, unsigned int base_phy,
				void *ddrtr_result, unsigned int mode)
{
	struct training_data tmp_result;
	struct training_data *training = &tmp_result;
	struct tr_dq_data dq_data;
	int result = 0;
	int i;

	DDR_DEBUG("DDR Vref[%x] training.", mode);

	training->ddr_byte_num       = ddr_phy_get_byte_num(base_dmc);
	training->base_phy           = base_phy;
	training->base_dmc           = base_dmc;
	training->ddrt_reversed_data = ddr_dmc_get_reversed(base_dmc);
	training->dq_check_type      = DDR_CHECK_TYPE_DDRT;

	ddr_vref_save_bdl(base_phy, training->ddr_byte_num, mode, &dq_data);

	/* vref calibrate */
	if (DDR_MODE_READ == mode)
		/* only check one byte */
		ddr_vref_cal(training, 0, mode);
	else
		for (i = 0; i < training->ddr_byte_num; i++)
			ddr_vref_cal(training, i, mode);

	/* dataeye deskew again on best vref. */
	for (i = 0; i < training->ddr_byte_num; i++)
		result += ddr_dataeye_deskew(training, i, mode);

	ddr_vref_restore_bdl(base_phy, training->ddr_byte_num, mode,
		&dq_data, training);

	ddr_result_data_save(ddrtr_result, training, mode);

	return result;
}
#endif /* DDR_VREF_TRAINING_CONFIG */

#define __write_leveling__
#ifdef DDR_WL_TRAINING_CONFIG
/**
 * ddr_bdl_add
 * @raw
 * @val
 */
static void ddr_bdl_add(unsigned int *raw, unsigned int val)
{
	if (((*raw) + val) > PHY_BDL_MASK)
		*raw = PHY_BDL_MASK;
	else
		*raw += val;
}

/**
 * ddr_bdl_sub
 * @raw
 * @val
 */
static void ddr_bdl_sub(unsigned int *raw, unsigned int val)
{
	if ((*raw) > val)
		*raw -= val;
	else
		*raw = 0;
}

/**
 * ddr_phase_inc
 * @raw
 *
 * DDR PHY DQ phase increase
 */
static void ddr_phase_inc(unsigned int *raw)
{
#ifdef DDR_PHY_T28_CONFIG
	if ((*raw) < (PHY_WDQS_PHASE_MASK - 1)) {
		if (((*raw) & 0x3) == 0x2)
			*raw += 0x2;
		else
			*raw += 0x1;
	}
#else
	if ((*raw) < PHY_WDQS_PHASE_MASK)
		*raw += 0x1;
#endif
}

/**
 * ddr_phase_dec
 * @raw
 *
 * DDR PHY DQ phase decrease.
 */
static void ddr_phase_dec(unsigned int *raw)
{
#ifdef DDR_PHY_T28_CONFIG
	if ((*raw) > 0x1) {
		if (((*raw) & 0x3) == 0x3)
			*raw -= 0x2;
		else
			*raw -= 0x1;
	}
#else
	if ((*raw) > 0x0)
		*raw -= 0x1;
#endif
}

/**
 * ddr_dq_bdl_operate
 * @base_phy
 * @addr_offset
 * @val
 * @is_add
 *
 * DQ bdl add or sub.
 */
static void ddr_dq_bdl_operate(unsigned int base_phy,
	unsigned int addr_offset, unsigned int val, unsigned int is_add)
{
	unsigned int tmp;
	unsigned int dq_bdl[DDR_PHY_REG_DQ_NUM];
	int i;

	tmp = REG_READ(base_phy + addr_offset);
	dq_bdl[0] = (tmp >> 0)  & PHY_BDL_MASK;
	dq_bdl[1] = (tmp >> 8)  & PHY_BDL_MASK;
	dq_bdl[2] = (tmp >> 16) & PHY_BDL_MASK;
	dq_bdl[3] = (tmp >> 24) & PHY_BDL_MASK;

	for (i = 0; i < DDR_PHY_REG_DQ_NUM; i++) {
		if (is_add)
			ddr_bdl_add(&dq_bdl[i], val);
		else
			ddr_bdl_sub(&dq_bdl[i], val);
	}

	tmp = (dq_bdl[3] << 24) + (dq_bdl[2] << 16)
		+ (dq_bdl[1] << 8)  + dq_bdl[0];
	REG_WRITE(tmp, base_phy + addr_offset);
}

/**
 * ddr_wl_switch
 * @base_dmc
 * @base_phy
 * @val
 *
 * Disable or enable DDR write leveling mode.
 */
static void ddr_wl_switch(unsigned int base_dmc, unsigned int base_phy,
			int val)
{
	unsigned int mr1_raw;
	unsigned int sfc_cmd;
	unsigned int sfc_bank;

	/* Set Rank = 0, Cmd = MRS, No Precharch CMD */
	mr1_raw = REG_READ(base_phy + DDR_PHY_MODEREG01)
		>> PHY_MODEREG01_MR1_BIT;
	sfc_cmd  = DMC_CMD_TYPE_LMR;
	sfc_bank = DMC_BANK_MR1;

	if (DDR_TRUE == val) { /* enable DDR wl */
		/* Set A7 = 1 */
		sfc_cmd  += (mr1_raw | DMC_CMD_MRS_A7) << DMC_SFC_CMD_MRS_BIT;
	} else {

		/* Set A7 = 0 */
		sfc_cmd  += (mr1_raw & ((~DMC_CMD_MRS_A7) & DMC_CMD_MRS_MASK))
					<< DMC_SFC_CMD_MRS_BIT;
	}

	ddr_dmc_sfc_cmd(base_dmc, sfc_cmd, 0x0, sfc_bank);

	/* clear */
	if (DDR_FALSE == val) {
		REG_WRITE(0x0, base_dmc + DDR_DMC_SFCBANK);
		REG_WRITE(0x0, base_dmc + DDR_DMC_SFCREQ);
	}

	/* phy sw write leveling mode */
	REG_WRITE(val, base_phy + DDR_PHY_SWTMODE);
}

#ifdef DDR_WL_DATAEYE_ADJUST_CONFIG
/**
 * ddr_wl_wdq_adjust
 * @base_phy
 * @byte_num
 * @wdqs_new
 * @wdqs_old
 *
 * Adjust dataeye WDQ after Write leveling.
 */
static void ddr_wl_wdq_adjust(unsigned int base_phy, unsigned int byte_num,
	struct ddr_delay_st *wdqs_new, struct ddr_delay_st *wdqs_old)
{
	unsigned int val;
	int i;
	unsigned int phase_adj, bdl_adj = 0;  /* for write dataeye */
	unsigned int wdm_bdl;
	unsigned int wdq_phase;

	DDR_DEBUG("DDR WL write adjust.");

	/* check wl write adjust bypass bit */
	if (ddr_training_check_bypass(DDR_BYPASS_WL_ADJ_MASK))
		return;

	/* adjust wdq phase, wdq bdl, wdm bdl */
	for (i = 0; i < byte_num; i++) {
		if (wdqs_new->phase[i] == wdqs_old->phase[i]
		 && wdqs_new->bdl[i]   == wdqs_old->bdl[i]) {
			continue;
		}

		phase_adj = 0;
		wdq_phase = (REG_READ(base_phy + DDR_PHY_DXNWDQDLY(i))
					>> PHY_WDQ_PHASE_BIT)
					& PHY_WDQ_PHASE_MASK;
		wdm_bdl  = REG_READ(base_phy + DDR_PHY_DXNWDQNBDL2(i))
			& PHY_BDL_MASK;

		if (wdqs_new->bdl[i] > wdqs_old->bdl[i]) {
			val = wdqs_new->bdl[i] - wdqs_old->bdl[i];
			phase_adj = val >> DDR_BDL_PHASE_REL;
			wdq_phase  = wdq_phase + phase_adj;

			if (wdq_phase > PHY_WDQ_PHASE_MASK)
				wdq_phase = PHY_WDQ_PHASE_MASK;

			/* adjust wdq bdl and dm bdl in opposite direction */
			bdl_adj  = phase_adj << DDR_BDL_PHASE_REL;
			ddr_dq_bdl_operate(base_phy, DDR_PHY_DXNWDQNBDL0(i),
				bdl_adj, DDR_FALSE);
			ddr_dq_bdl_operate(base_phy, DDR_PHY_DXNWDQNBDL1(i),
				bdl_adj, DDR_FALSE);
			ddr_bdl_sub(&wdm_bdl, bdl_adj);

		} else if (wdqs_new->bdl[i] < wdqs_old->bdl[i]) {
			val = wdqs_old->bdl[i] - wdqs_new->bdl[i];
			phase_adj = val >> DDR_BDL_PHASE_REL;
			wdq_phase = (wdq_phase > phase_adj)
				? (wdq_phase - phase_adj) : 0;

			/* adjust wdq bdl and dm bdl in opposite direction */
			bdl_adj  = phase_adj << DDR_BDL_PHASE_REL;
			ddr_dq_bdl_operate(base_phy, DDR_PHY_DXNWDQNBDL0(i),
				bdl_adj, DDR_TRUE);
			ddr_dq_bdl_operate(base_phy, DDR_PHY_DXNWDQNBDL1(i),
				bdl_adj, DDR_TRUE);
			ddr_bdl_add(&wdm_bdl, bdl_adj);
		}

		DDR_INFO("Byte[%x] WDQ adjust phase[%x] bdl[%x]",
			i, phase_adj, bdl_adj);

		REG_WRITE(wdq_phase << PHY_WDQ_PHASE_BIT,
					base_phy + DDR_PHY_DXNWDQDLY(i));
		REG_WRITE(wdm_bdl, base_phy + DDR_PHY_DXNWDQNBDL2(i));
	}

	ddr_phy_cfg_update(base_phy);
}
#endif /* DDR_WL_DATAEYE_ADJUST_CONFIG */

/**
 * ddr_wl_bdl_sync
 * @base_phy
 * @byte_num
 * @wdqs_new
 * @wdqs_old
 *
 * Sync WDQ phase, WDQ bdl, WDM bdl, OEN bdl, WDQ SOE bdl by WDQS value.
 */
static void ddr_wl_bdl_sync(unsigned int base_phy, unsigned int byte_num,
	struct ddr_delay_st *wdqs_new, struct ddr_delay_st *wdqs_old)
{
	unsigned int tmp;
	unsigned int val;
	int i;

	unsigned int oen_bdl, wdqsoe_bdl, wdm_bdl;
	unsigned int wdq_phase;

	/* sync wdq phase, wdq bdl, wdm bdl, oen bdl, wdq soe bdl */
	for (i = 0; i < byte_num; i++) {
		if (wdqs_new->phase[i] == wdqs_old->phase[i]
		 && wdqs_new->bdl[i]   == wdqs_old->bdl[i]) {
			continue;
		}

		DDR_DEBUG("Byte[%x] new[%x][%x] old[%x][%x]", i,
			wdqs_new->phase[i],	wdqs_new->bdl[i],
			wdqs_old->phase[i], wdqs_old->bdl[i]);

		/* wdq phase */
		wdq_phase = (REG_READ(base_phy + DDR_PHY_DXNWDQDLY(i))
					>> PHY_WDQ_PHASE_BIT)
					& PHY_WDQ_PHASE_MASK;
		/* always new_phase >= old_phase */
		wdq_phase = wdq_phase
			+ (wdqs_new->phase[i] - wdqs_old->phase[i]);

		/* bdl */
		tmp = REG_READ(base_phy + DDR_PHY_DXNOEBDL(i));
		oen_bdl	 = tmp & PHY_BDL_MASK;
		wdqsoe_bdl = (tmp >> PHY_WDQSOE_BDL_BIT) & PHY_BDL_MASK;
		wdm_bdl	 = REG_READ(base_phy + DDR_PHY_DXNWDQNBDL2(i))
			& PHY_BDL_MASK;

		if (wdqs_new->bdl[i] > wdqs_old->bdl[i]) {
			val = wdqs_new->bdl[i] - wdqs_old->bdl[i];
			ddr_dq_bdl_operate(base_phy,
				DDR_PHY_DXNWDQNBDL0(i), val, DDR_TRUE);
			ddr_dq_bdl_operate(base_phy,
				DDR_PHY_DXNWDQNBDL1(i), val, DDR_TRUE);
			ddr_bdl_add(&oen_bdl, val);
			ddr_bdl_add(&wdqsoe_bdl, val);
			ddr_bdl_add(&wdm_bdl, val);
		} else if (wdqs_new->bdl[i] < wdqs_old->bdl[i]) {
			val = wdqs_old->bdl[i] - wdqs_new->bdl[i];
			ddr_dq_bdl_operate(base_phy, DDR_PHY_DXNWDQNBDL0(i),
						val, DDR_FALSE);
			ddr_dq_bdl_operate(base_phy, DDR_PHY_DXNWDQNBDL1(i),
						val, DDR_FALSE);
			ddr_bdl_sub(&oen_bdl, val);
			ddr_bdl_sub(&wdqsoe_bdl, val);
			ddr_bdl_sub(&wdm_bdl, val);
		}

		if (wdq_phase > PHY_WDQ_PHASE_MASK)
			wdq_phase = PHY_WDQ_PHASE_MASK;

		REG_WRITE(wdq_phase << PHY_WDQ_PHASE_BIT,
			base_phy + DDR_PHY_DXNWDQDLY(i));
		REG_WRITE((wdqsoe_bdl << 16) + oen_bdl,
			base_phy + DDR_PHY_DXNOEBDL(i));
		REG_WRITE(wdm_bdl, base_phy + DDR_PHY_DXNWDQNBDL2(i));
	}

	ddr_phy_cfg_update(base_phy);
}

/**
 * ddr_wl_process
 * @base_phy
 * @byte_num
 * @type
 * @wdqs
 *
 * Write leveling process.
 * WL depend default WDQS phase value in register init table.
 */
static int ddr_wl_process(unsigned int base_phy, unsigned int byte_num,
		unsigned int type, struct ddr_delay_st *wdqs)
{
	int i, j;
	unsigned int wl_result = 0;
	unsigned int length;

	if (DDR_DELAY_PHASE == type)
		length = PHY_WDQS_PHASE_MASK;
	else
		length = PHY_BDL_MASK;

	/* find WDQS phase or bdl, assume CLK Delay > DQS Delay */
	for (i = 0; i <= length; i++) {
		ddr_phy_cfg_update(base_phy);
		REG_WRITE(0x1, base_phy + DDR_PHY_SWTWLDQS);
		DDR_ASM_DSB();
		wl_result = REG_READ(base_phy + DDR_PHY_SWTRLT)
				& PHY_SWTRLT_WL_MASK;
		REG_WRITE(0x0, base_phy + DDR_PHY_SWTWLDQS);

		if (wl_result == ((1 << byte_num) - 1))
			break;

		for (j = 0; j < byte_num; j++) {
			if (!(wl_result & (1 << j))) {
				if (DDR_DELAY_PHASE == type)
					ddr_phase_inc(&wdqs->phase[j]);
				else
					wdqs->bdl[j] += DDR_WL_BDL_STEP;

			REG_WRITE((wdqs->phase[j] << PHY_WDQS_PHASE_BIT)
					+ wdqs->bdl[j],
					base_phy + DDR_PHY_DXWDQSDLY(j));
			}
		}
	}

	if (i > length) {   /* wl error, not find wdqs delay */
		if (DDR_DELAY_BDL == type) {
			DDR_FATAL("PHY[%x] WL fail, result[%x]",
					base_phy, wl_result);
			for (j = 0; j < byte_num; j++)
				if (!(wl_result & (1 << j)))
					ddr_training_stat(DDR_ERR_WL,
						base_phy, j, -1);

		} else
			DDR_DEBUG("PHY[%x] WL not found phase, result[%x]",
					base_phy, wl_result);

		return -1;
	} else
		return 0;
}

/**
 * ddr_write_leveling
 * @base_dmc
 * @base_phy
 *
 * Find WDQS delay, sync to WDQ delay and OE delay.
 * WL depend default WDQS phase value in register init table.
 */
int ddr_write_leveling(unsigned int base_dmc, unsigned int base_phy)
{
	unsigned int i, tmp;
	unsigned int byte_num;
	struct ddr_delay_st wdqs_old;
	struct ddr_delay_st wdqs_new;
	int result = 0;

	DDR_DEBUG("DDR Write Leveling training.");

	byte_num = ddr_phy_get_byte_num(base_dmc);

	/* init wdqs */
	for (i = 0; i < byte_num; i++) {
		tmp = REG_READ(base_phy + DDR_PHY_DXWDQSDLY(i));

		wdqs_old.phase[i] = (tmp >> PHY_WDQS_PHASE_BIT)
					& PHY_WDQS_PHASE_MASK;
		wdqs_old.bdl[i] = tmp & PHY_BDL_MASK;

		wdqs_new.phase[i] = wdqs_old.phase[i];
		wdqs_new.bdl[i] = 0;

		/* clear wdqs bdl*/
		REG_WRITE(wdqs_new.phase[i] << PHY_WDQS_PHASE_BIT,
				base_phy + DDR_PHY_DXWDQSDLY(i));
	}

	/* enable sw write leveling mode*/
	ddr_wl_switch(base_dmc, base_phy, DDR_TRUE);

	/* find first WDQS phase, assume CLK delay > DQS delay.  */
	result = ddr_wl_process(base_phy, byte_num,
		DDR_DELAY_PHASE, &wdqs_new);

	/* check phase result */
	for (i = 0; i < byte_num; i++) {
		/* find phase error, keep max value to find bdl. */
		/* find phase ok, decrease to find bdl. */
		if (!result)
			ddr_phase_dec(&wdqs_new.phase[i]);

		REG_WRITE(wdqs_new.phase[i] << PHY_WDQS_PHASE_BIT,
			base_phy + DDR_PHY_DXWDQSDLY(i));
	}

	/* find WDQS bdl */
	result = ddr_wl_process(base_phy, byte_num,
		DDR_DELAY_BDL, &wdqs_new);

	/* disable sw write leveling mode */
	ddr_wl_switch(base_dmc, base_phy, DDR_FALSE);

	if (result) {
		/* restore default value when find WDQS fail */
		for (i = 0; i < byte_num; i++) {
			tmp = (wdqs_old.phase[i] << PHY_WDQS_PHASE_BIT)
				+ wdqs_old.bdl[i];
			REG_WRITE(tmp, base_phy + DDR_PHY_DXWDQSDLY(i));
		}
		ddr_phy_cfg_update(base_phy);
		return -1;
	}

	/* sync delay */
	ddr_wl_bdl_sync(base_phy, byte_num, &wdqs_new, &wdqs_old);

#ifdef DDR_WL_DATAEYE_ADJUST_CONFIG
	/* adjust WDQ for dataeye */
	ddr_wl_wdq_adjust(base_phy, byte_num, &wdqs_new, &wdqs_old);
#endif
	return 0;
}
 #endif /* DDR_WL_TRAINING_CONFIG */

#define __gate_training__
#ifdef DDR_GATE_TRAINING_CONFIG
/**
 * ddr_gate_find_phase
 * @base_phy
 * @byte_num
 * @rdqsg
 *
 * Find gate phase.
 */
static int ddr_gate_find_phase(unsigned int base_phy, unsigned int byte_num,
						struct ddr_delay_st *rdqsg)
{
	int i;

	for (i = 0; i < byte_num; i++) {
		for (rdqsg->phase[i] = PHY_RDQSG_PHASE_MAX;
			rdqsg->phase[i] > PHY_GATE_PHASE_MARGIN;
			rdqsg->phase[i] -= PHY_RDQSG_PHASE_STEP) {
			REG_WRITE(rdqsg->phase[i] << PHY_RDQSG_PHASE_BIT,
				base_phy + DDR_PHY_DXNRDQSGDLY(i));
			ddr_phy_cfg_update(base_phy);
			if (0 == ddr_ddrt_test(DDRT_WR_COMPRARE_MODE, i, -1))
				break;
		}
		if (rdqsg->phase[i] <= PHY_GATE_PHASE_MARGIN) {
			/* find gate phase fail */
			DDR_FATAL("find gate phase[%x] fail.",
				rdqsg->phase[i]);
			ddr_training_stat(DDR_ERR_GATING, base_phy, -1, -1);
			return -1;
		} else {
			/* decrease one setp to find bdl */
			rdqsg->phase[i] -= PHY_RDQSG_PHASE_STEP;
			REG_WRITE(rdqsg->phase[i] << PHY_RDQSG_PHASE_BIT,
				base_phy + DDR_PHY_DXNRDQSGDLY(i));
		}
	}

	ddr_phy_cfg_update(base_phy);
	return 0;
}

/**
 * ddr_gate_find_bdl
 * @base_phy
 * @byte_num
 * @rdqsg
 *
 * Find gate bdl.
 */
static int ddr_gate_find_bdl(unsigned int base_phy, unsigned int byte_num,
	struct ddr_delay_st *rdqsg)
{
	int i, j;
	unsigned int gate_result;
	unsigned int tmp;
	unsigned int swtmode = REG_READ(base_phy + DDR_PHY_SWTMODE);

	for (i = 0; i < byte_num; i++)
		rdqsg->bdl[i] = 0;

	/* enable phy sw gate training mode */
	REG_WRITE(swtmode | (1 << PHY_SWTMODE_SW_GTMODE_BIT),
		base_phy + DDR_PHY_SWTMODE);

	for (i = 0; i < PHY_GATE_BDL_MAX; i++) {
		ddr_phy_cfg_update(base_phy);
		ddr_ddrt_test(DDRT_READ_ONLY_MODE, -1, -1);
		gate_result = (REG_READ(base_phy + DDR_PHY_SWTRLT) >> 8)
					& PHY_SWTRLT_GATE_MASK;
		if (gate_result == ((1 << byte_num) - 1))
			break;

		for (j = 0; j < byte_num; j++) {
			if (!(gate_result & (1 << j))) {
				rdqsg->bdl[j] += DDR_GATE_BDL_STEP;
				if (rdqsg->bdl[j] > PHY_BDL_MASK) {
					tmp = ((rdqsg->bdl[j]
						- PHY_BDL_MASK - 1)
						<< PHY_RDQSG_TX_BDL_BIT)
						+ (rdqsg->phase[j]
						<< PHY_RDQSG_PHASE_BIT)
						+ (PHY_BDL_MASK - 1);
				} else {
					tmp = (rdqsg->phase[j]
						<< PHY_RDQSG_PHASE_BIT)
						+ rdqsg->bdl[j];
				}
				REG_WRITE(tmp,
					base_phy + DDR_PHY_DXNRDQSGDLY(j));
			}
		}
	}

	/* disable phy sw gate training mode */
	REG_WRITE(swtmode & (~(1 << PHY_SWTMODE_SW_GTMODE_BIT)),
		base_phy + DDR_PHY_SWTMODE);

	if (i == PHY_GATE_BDL_MAX) {  /* find gate bdl fail */
		DDR_FATAL("PHY[%x] find gate bdl fail. result[%x]",
				base_phy, gate_result);
		for (j = 0; j < byte_num; j++)
			if (!(gate_result & (1 << j)))
				ddr_training_stat(DDR_ERR_GATING,
					base_phy, j, -1);
		return -1;
	} else
		return 0;
}

/**
 * ddr_gate_training
 * @base_dmc
 * @base_phy
 *
 * DDR gate training
 */
int ddr_gate_training(unsigned int base_dmc, unsigned int base_phy)
{
	unsigned int i, tmp;
	unsigned int byte_num;
	struct ddr_delay_st rdqsg;
	unsigned int def_delay[DDR_PHY_BYTE_MAX];
	int result;

	DDR_DEBUG("DDR Gate training.");

	byte_num = ddr_phy_get_byte_num(base_dmc);

	for (i = 0; i < byte_num; i++)
		def_delay[i] = REG_READ(base_phy + DDR_PHY_DXNRDQSGDLY(i));

	/* find phase first */
	result = ddr_gate_find_phase(base_phy, byte_num, &rdqsg);

	/* find bdl */
	if (!result)
		result = ddr_gate_find_bdl(base_phy, byte_num, &rdqsg);

	/* set new phase */
	if (!result) {
		for (i = 0; i < byte_num; i++) {
			rdqsg.phase[i] -= PHY_GATE_PHASE_MARGIN;
			tmp = REG_READ(base_phy + DDR_PHY_DXNRDQSGDLY(i));
			tmp &= ~(PHY_RDQSG_PHASE_MASK << PHY_RDQSG_PHASE_BIT);
			tmp |= rdqsg.phase[i] << PHY_RDQSG_PHASE_BIT;
			REG_WRITE(tmp, base_phy + DDR_PHY_DXNRDQSGDLY(i));
		}
	} else {
		/* restore default value */
		for (i = 0; i < byte_num; i++)
			REG_WRITE(def_delay[i],
			base_phy + DDR_PHY_DXNRDQSGDLY(i));
	}

	ddr_phy_cfg_update(base_phy);
	return 0; /* use default value and not reset */
}
#endif /* DDR_GATE_TRAINING_CONFIG */

#define __ac_training__
#ifdef DDR_AC_TRAINING_CONFIG
/**
 * ddr_ac_get_clk
 * @base_phy
 *
 * Get clk value.
 * Assume clk0 and clk1 is the same.
 */
static int ddr_ac_get_clk(unsigned int base_phy)
{
	unsigned int val;
	unsigned int ac_phy_ctl;
	ac_phy_ctl = REG_READ(base_phy + DDR_PHY_ACPHYCTL7);
	 /* halft_dramclk0 */
	val = (ac_phy_ctl >> PHY_ACPHY_DRAMCLK0_BIT)
		& PHY_ACPHY_DRAMCLK_MASK;
	val = (val << PHY_ACPHY_DRAMCLK_EXT_BIT)
		| ((ac_phy_ctl >> PHY_ACPHY_DCLK0_BIT)
			& PHY_ACPHY_DCLK_MASK);
	return val;
}

/**
 * ddr_ac_set_clk
 * @base_phy
 * @val
 *
 * Set clk0 and clk1 the same value.
 */
static void ddr_ac_set_clk(unsigned int base_phy, unsigned int val)
{
	unsigned int ac_phy_ctl, dramclk, dclk;
	dclk       = val & PHY_ACPHY_DCLK_MASK;
	dramclk    = (val >> PHY_ACPHY_DRAMCLK_EXT_BIT)
		& PHY_ACPHY_DRAMCLK_MASK;
	ac_phy_ctl = REG_READ(base_phy + DDR_PHY_ACPHYCTL7);
	/* clear cp1p_dclk0 */
	ac_phy_ctl &= (~(PHY_ACPHY_DCLK_MASK << PHY_ACPHY_DCLK0_BIT));
	/* clear ck2p_dclk1 */
	ac_phy_ctl &= (~(PHY_ACPHY_DCLK_MASK << PHY_ACPHY_DCLK1_BIT));
	/* clear halft_dramclk0 */
	ac_phy_ctl &= (~(PHY_ACPHY_DRAMCLK_MASK << PHY_ACPHY_DRAMCLK0_BIT));
	/* clear halft_dramclk1 */
	ac_phy_ctl &= (~(PHY_ACPHY_DRAMCLK_MASK << PHY_ACPHY_DRAMCLK1_BIT));

	ac_phy_ctl |= (dclk << PHY_ACPHY_DCLK0_BIT);  /* set cp1p_dclk0 */
	ac_phy_ctl |= (dclk << PHY_ACPHY_DCLK1_BIT);  /* set cp2p_dclk1 */
	/* set halft_dramclk0 */
	ac_phy_ctl |= (dramclk << PHY_ACPHY_DRAMCLK0_BIT);
	/* set halft_dramclk1 */
	ac_phy_ctl |= (dramclk << PHY_ACPHY_DRAMCLK1_BIT);
	REG_WRITE(ac_phy_ctl, base_phy + DDR_PHY_ACPHYCTL7);
}

/**
 * ddr_ac_get_cs
 * @base_phy
 *
 * Get cs bdl value.
 * Assume cs0 and cs 1 is the same.
 */
static int ddr_ac_get_cs(unsigned int base_phy)
{
	return REG_READ(base_phy + DDR_PHY_ACCMDBDL2) & PHY_BDL_MASK;
}

/**
 * ddr_ac_set_cs
 * @base_phy
 * @val
 *
 * Set CS value.
 */
static void ddr_ac_set_cs(unsigned int base_phy, unsigned int val)
{
	unsigned int ac_cmd_bdl;
	ac_cmd_bdl = REG_READ(base_phy + DDR_PHY_ACCMDBDL2);
	ac_cmd_bdl &= (~PHY_BDL_MASK); /* clear cs0_bdl */
	/* clear cs1_bdl */
	ac_cmd_bdl &= (~(PHY_BDL_MASK << PHY_ACCMD_CS1_BIT));
	ac_cmd_bdl |= val; /* set cs0_bdl */
	ac_cmd_bdl |= (val << PHY_ACCMD_CS1_BIT); /* set cs1_bdl */
	REG_WRITE(ac_cmd_bdl, base_phy + DDR_PHY_ACCMDBDL2);
}

static int ddr_ac_ddrt_test(unsigned int mask)
{
	unsigned int regval;
	unsigned int times = 0;

	DDRT_REG_WRITE(mask | DDRT_CFG_START, DDR_REG_BASE_DDRT + DDRT_OP);
	DDRT_REG_WRITE(0, DDR_REG_BASE_DDRT + DDRT_STATUS);

	do {
		regval = DDRT_REG_READ(DDR_REG_BASE_DDRT + DDRT_STATUS);
		times++;
	} while ((!(regval & DDRT_TEST_DONE_MASK))
			&& (times < DDRT_WAIT_TIMEOUT));

	if (times >= DDRT_WAIT_TIMEOUT) {
		DDR_FATAL("DDRT wait timeout.");
		ddr_training_stat(DDR_ERR_DDRT_TIME_OUT, 0, -1, -1);
		return -1;
	}

	/* DDRT_WRITE_ONLY_MODE */
	if (DDRT_WRITE_ONLY_MODE == (mask & DDRT_TEST_MODE_MASK))
		return 0;

	/* DDRT_READ_ONLY_MODE */
	if (regval & DDRT_TEST_PASS_MASK) /* No error occurred, test pass. */
		return 0;
	else
		return -1;

}

/**
 * ddr_ac_check_cs
 * @base_phy
 * @def_cs
 * @step
 *
 * Cehck CS value.
 */
static int ddr_ac_check_cs(unsigned int base_phy, unsigned int def_cs,
					unsigned int step)
{
	ddr_ac_set_cs(base_phy, def_cs + step);
	ddr_phy_cfg_update(base_phy);

	ddr_ac_ddrt_test(DDRT_WRITE_ONLY_MODE);

	ddr_ac_set_cs(base_phy, def_cs); /* restore default to check */
	ddr_phy_cfg_update(base_phy);

	return ddr_ac_ddrt_test(DDRT_READ_ONLY_MODE);
}

/**
 * ddr_ac_check_clk
 * @base_phy
 * @def_clk
 * @byte_num
 * @def_phase
 * @step
 *
 * Check CLK value.
 */
static int ddr_ac_check_clk(unsigned int base_phy, unsigned int def_clk,
	unsigned int byte_num, struct ddr_delay_st *def_phase,
	unsigned int step)
{
	int i;
	unsigned int wdqs_phase_range, wdq_phase_range, phase_range;

	/* set new value */
	ddr_ac_set_clk(base_phy, def_clk + step);
	for (i = 0; i < byte_num; i++) {
		wdqs_phase_range = PHY_WDQS_PHASE_MASK
			- ((def_phase->phase[i] >> PHY_WDQS_PHASE_BIT)
				& PHY_WDQS_PHASE_MASK);
		wdq_phase_range  = PHY_WDQ_PHASE_MASK
			- ((def_phase->bdl[i] >> PHY_WDQ_PHASE_BIT)
				& PHY_WDQ_PHASE_MASK);
		phase_range = (wdqs_phase_range < wdq_phase_range)
			? wdqs_phase_range : wdq_phase_range;
		phase_range = (phase_range < step) ? phase_range : step;

		REG_WRITE(def_phase->phase[i]
				+ (phase_range << PHY_WDQS_PHASE_BIT),
				base_phy + DDR_PHY_DXWDQSDLY(i));
		REG_WRITE(def_phase->bdl[i]
				+ (phase_range << PHY_WDQ_PHASE_BIT),
				base_phy + DDR_PHY_DXNWDQDLY(i));
	}
	ddr_phy_cfg_update(base_phy);

	ddr_ac_ddrt_test(DDRT_WRITE_ONLY_MODE);

	/* restore default to check */
	ddr_ac_set_clk(base_phy, def_clk);
	for (i = 0; i < byte_num; i++) {
		REG_WRITE(def_phase->phase[i],
			base_phy + DDR_PHY_DXWDQSDLY(i));
		REG_WRITE(def_phase->bdl[i],
			base_phy + DDR_PHY_DXNWDQDLY(i));
	}
	ddr_phy_cfg_update(base_phy);

	return ddr_ac_ddrt_test(DDRT_READ_ONLY_MODE);
}

/**
 * ddr_ac_find_cs
 * @base_phy
 *
 * Find CS difference.
 */
static int ddr_ac_find_cs(unsigned int base_phy)
{
	unsigned int def_cs, step;

	def_cs = ddr_ac_get_cs(base_phy);
	for (step = 1; step <= (PHY_BDL_MASK - def_cs); step++) {
		if (ddr_ac_check_cs(base_phy, def_cs, step)) {
			DDR_DEBUG("PHY[%x] default cs[%x], find diff_cs[%x]",
					base_phy, def_cs, step);
			break;
		}
	}

	return step;
}

/**
 * ddr_ac_find_clk
 * @base_phy
 * @byte_num
 *
 * Find CLK difference.
 */
static int ddr_ac_find_clk(unsigned int base_phy, unsigned int byte_num)
{
	int i;
	unsigned int def_clk, step;
	struct ddr_delay_st def_phase;

	def_clk = ddr_ac_get_clk(base_phy);
	for (i = 0; i < byte_num; i++) {
		/* WDQS phase */
		def_phase.phase[i] = REG_READ(base_phy + DDR_PHY_DXWDQSDLY(i));
		/* WDQ phase */
		def_phase.bdl[i]   = REG_READ(base_phy + DDR_PHY_DXNWDQDLY(i));
	}

	for (step = 1; step <= (PHY_ACPHY_CLK_MAX - def_clk); step++) {
		if (ddr_ac_check_clk(base_phy, def_clk, byte_num,
					&def_phase, step)) {
			DDR_DEBUG("PHY[%x] default clk[%x], find diff_clk[%x]",
				base_phy, def_clk, step);
			break;
		}
	}

	return step;
}

/**
 * ddr_ac_training
 * @base_dmc
 * @base_phy
 *
 * DDR AC training.
 */
int ddr_ac_training(unsigned int base_dmc, unsigned int base_phy)
{
	unsigned int diff_cs, diff_clk;
	unsigned int clk_phase, cs_bdl, phase_tmp;
	unsigned int byte_num;
	unsigned int wdqs_phase, wdq_phase;
	unsigned int wdqs_phase_range, wdq_phase_range, phase_range;
	unsigned int def_clk, def_cs;
	int i;

	DDR_DEBUG("DDR AC training.");

	byte_num = ddr_phy_get_byte_num(base_dmc);

	diff_cs  = ddr_ac_find_cs(base_phy);            /* setup time(bdl) */
	diff_clk = ddr_ac_find_clk(base_phy, byte_num); /* hold time(phase) */
	/* cs bdl transform to clk phase */
	phase_tmp = diff_cs >> DDR_BDL_PHASE_REL;

	if (diff_clk > phase_tmp) {
		clk_phase = (diff_clk - phase_tmp) >> 1;
		def_clk = ddr_ac_get_clk(base_phy);

		/* set new value */
		ddr_ac_set_clk(base_phy, def_clk + clk_phase);
		for (i = 0; i < byte_num; i++) {
			wdqs_phase = REG_READ(base_phy + DDR_PHY_DXWDQSDLY(i));
			wdq_phase  = REG_READ(base_phy + DDR_PHY_DXNWDQDLY(i));

			wdqs_phase_range = PHY_WDQS_PHASE_MASK
				- ((wdqs_phase >> PHY_WDQS_PHASE_BIT)
					& PHY_WDQS_PHASE_MASK);
			wdq_phase_range  = PHY_WDQ_PHASE_MASK
				- ((wdq_phase >> PHY_WDQ_PHASE_BIT)
					& PHY_WDQ_PHASE_MASK);
			phase_range = (wdqs_phase_range < wdq_phase_range)
				? wdqs_phase_range : wdq_phase_range;
			phase_range = (phase_range < clk_phase)
				? phase_range : clk_phase;
			REG_WRITE(wdqs_phase
					+ (phase_range << PHY_WDQS_PHASE_BIT),
					base_phy + DDR_PHY_DXWDQSDLY(i));
			REG_WRITE(wdq_phase
					+ (phase_range << PHY_WDQ_PHASE_BIT),
					base_phy + DDR_PHY_DXNWDQDLY(i));
		}
		DDR_DEBUG("PHY[%x] def clk[%x] add phase[%x]",
			base_phy, def_clk, clk_phase);
	} else {
		def_cs = ddr_ac_get_cs(base_phy);
		cs_bdl = 0;
		if (diff_cs > (diff_clk << DDR_BDL_PHASE_REL))
			cs_bdl = diff_cs - (diff_clk << DDR_BDL_PHASE_REL);

		ddr_ac_set_cs(base_phy, def_cs + cs_bdl);
		DDR_DEBUG("PHY[%x] def cs[%x] add bdl[%x]",
			base_phy, def_cs, cs_bdl);
	}

	ddr_phy_cfg_update(base_phy);
	return 0;
}
#endif /* DDR_AC_TRAINING_CONFIG */
