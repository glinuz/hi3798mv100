/**
 * ddr_training_cmd.c
 *
 * Copyright (c) 2009-2014, HiSilicon Technologies Co., Ltd.
 * All rights reserved.
 *
 * DDR training command implement.
 */
#include <stdarg.h>
#include "ddr_interface.h"
#include <config.h>
#include "ddr_training_impl.h"

#ifndef CONFIG_SUPPORT_CA_RELEASE
extern void uart_early_puts(const char *s);
extern void uart_early_put_hex(int hex);
extern void uart_early_putc(int chr);
#else
#define uart_early_puts(_s)
#define uart_early_put_hex(_h)
#define uart_early_putc(_c)
#endif

/* ddr training cmd result */
static struct ddr_training_result_st ddrt_result_sram;
static unsigned int ddr_training_addr_start;
static unsigned int ddr_training_addr_end;
static int ddr_print_level = DDR_LOG_ERROR;

#ifdef DDR_TRAINING_LOG_CONFIG
/**
 * ddr_training_log
 * @func
 * @level
 * @fmt
 *
 * Log ddr training info.
 */
void ddr_training_log(const char *func, int level, const char *fmt, ...)
{
	va_list args;

	if (ddr_print_level > level)
		return;

	uart_early_putc('[');
	switch (level) {
	case DDR_LOG_INFO:
		uart_early_puts("INFO");
		 break;
	case DDR_LOG_DEBUG:
		uart_early_puts("DEBUG");
		break;
	case DDR_LOG_WARNING:
		 uart_early_puts("WARNING");
		break;
	case DDR_LOG_ERROR:
		uart_early_puts("ERROR");
		break;
	case DDR_LOG_FATAL:
		uart_early_puts("FATAL");
		break;
	default:
		break;
	}
	uart_early_putc(']');
	uart_early_putc('[');
	uart_early_puts(func);
	uart_early_putc(']');

	va_start(args, fmt);
	while ('\0' != *fmt) {
		if ('%' != *fmt) {
			uart_early_putc(*fmt);
		} else {
			fmt++;
			switch (*fmt) {
			case 'x':
			case 'X':
				uart_early_puts("0x");
				uart_early_put_hex(va_arg(args, int));
				break;
			default:
				uart_early_putc('%');
				uart_early_putc(*fmt);
				break;
			} /* switch */
		}
		fmt++;
	} /* while */
	va_end(args);
	uart_early_puts("\r\n");
}

/**
 * ddr_training_error
 * @mask
 * @phy
 * @byte
 * @dq
 *
 * Nothing to do in DDR command when defined DDR_TRAINING_LOG_CONFIG.
 */
void ddr_training_error(unsigned int mask, unsigned int phy, int byte, int dq)
{
	return;
}
#else
/**
 * ddr_training_error
 * @mask
 * @phy
 * @byte
 * @dq
 *
 * Display DDR training error.
 */
void ddr_training_error(unsigned int mask, unsigned int phy, int byte, int dq)
{
	switch (mask) {
	case DDR_ERR_WL:
		uart_early_puts("WL");
		break;
	case DDR_ERR_HW_GATING:
		uart_early_puts("HW Gate");
		break;
	case DDR_ERR_GATING:
		uart_early_puts("Gate");
		break;
	case DDR_ERR_DDRT_TIME_OUT:
		uart_early_puts("DDRT");
		break;
	case DDR_ERR_HW_RD_DATAEYE:
		uart_early_puts("HW Dataeye");
		break;
	case DDR_ERR_MPR:
		uart_early_puts("MPR");
		break;
	case DDR_ERR_DATAEYE:
		uart_early_puts("Dataeye");
		break;
	default:
		break;
	}

	uart_early_puts(" Err:");

	if (0 != phy) {
		uart_early_puts(" Phy:");
		uart_early_put_hex(phy);
	}

	if (-1 != byte) {
		uart_early_puts(" Byte:");
		uart_early_put_hex(byte);
	}

	if (-1 != dq) {
		uart_early_puts(" DQ:");
		uart_early_put_hex(dq);
	}

	uart_early_puts("\r\n");
}
#endif

/**
 * ddr_training_result_init
 * @void
 *
 * Inint ddr training cmd result.
 */
static void ddr_training_result_init(void)
{
	int i = 0;
	unsigned int base_dmc, base_phy;

	for (i = 0; i < DDR_PHY_NUM; i++) {
		ddr_training_get_base(i, &base_dmc, &base_phy);
		ddrt_result_sram.ddrtr_data[i].base_dmc = base_dmc;
		ddrt_result_sram.ddrtr_data[i].base_phy = base_phy;
		ddrt_result_sram.ddrtr_data[i].byte_num
			= ddr_phy_get_byte_num(base_dmc);
	}
}

/**
 * ddr_result_data_save
 * @ddrtr_result
 * @training
 * @mode
 *
 * Save ddr training cmd result.
 */
void ddr_result_data_save(void *ddrtr_result,
			struct training_data *training, unsigned int mode)
{
	int index = 0;
	int i;
	struct ddr_training_result_st *result_st
		= (struct ddr_training_result_st *)ddrtr_result;
	struct ddr_training_data_st *data;

	for (i = 0; i < DDR_SUPPORT_PHY_MAX; i++) {
		if (result_st->ddrtr_data[i].base_dmc == training->base_dmc) {
			index = i;
			break;
		}
	}

	data = &result_st->ddrtr_data[index];

	if (DDR_MODE_READ == mode) {
		for (i = 0; i < training->ddr_byte_num << 3; i++) {
			data->rd_bit_result[i] = training->ddr_bit_result[i];
			data->rd_bit_best[i]   = training->ddr_bit_best[i];
		}
		data->rd_win_sum  = training->ddr_win_sum;
	} else {
		for (i = 0; i < training->ddr_byte_num << 3; i++) {
			data->wr_bit_result[i] = training->ddr_bit_result[i];
			data->wr_bit_best[i]   = training->ddr_bit_best[i];
		}
		data->wr_win_sum  = training->ddr_win_sum;
	}

}

/**
 * ddr_ddrt_get_test_addr
 * @void
 *
 * Get DDRT test addrress.
 */
int ddr_ddrt_get_test_addr(void)
{
	if (ddr_training_addr_start <= DDRT_CFG_TEST_ADDR_CMD
	&&	ddr_training_addr_end >= DDRT_CFG_TEST_ADDR_CMD) {
		return DDRT_CFG_TEST_ADDR_CMD;
	} else {
		DDR_ERROR("DDRT test address[%x] out of range[%x, %x].",
				DDRT_CFG_TEST_ADDR_CMD,
				ddr_training_addr_start,
				ddr_training_addr_end);
		return ddr_training_addr_start;
	}
}

/**
 * ddr_training_suc
 * @void
 *
 * Nothing to do in DDR command.
 */
void ddr_training_suc(void) { return; }

/**
 * ddr_training_start
 * @void
 *
 * Nothing to do in DDR command.
 */
void ddr_training_start(void) { return; }

#ifdef DDR_HW_TRAINING_CONFIG
/**
 * ddr_hw_training_func
 * @void
 *
 *
 */
static int ddr_hw_training_func(void)
{
	unsigned int base_dmc, base_phy;
	struct tr_relate_reg relate_reg;
	int result = 0;
	int i;

	/* hardware training disable */
	if (ddr_training_check_bypass(DDR_BYPASS_HW_MASK))
		return 0;

	ddr_training_save_reg(&relate_reg, DDR_BYPASS_HW_MASK);
	for (i = 0; i < DDR_PHY_NUM; i++) {
		if (ddr_training_phy_disable(i))
			continue;

		ddr_training_get_base(i, &base_dmc, &base_phy);
		result += ddr_hw_training(base_dmc, base_phy);
	}
	ddr_training_restore_reg(&relate_reg);

	return result;
}
#else
static int ddr_hw_training_func(void)
{
	DDR_WARNING("Not support DDR HW training.");
	return 0;
}
#endif /* DDR_HW_TRAINING_CONFIG */

#ifdef DDR_MPR_TRAINING_CONFIG
/**
 * ddr_mpr_training_func
 * @void
 *
 *
 */
static int ddr_mpr_training_func(void)
{
	unsigned int base_dmc, base_phy;
	struct tr_relate_reg relate_reg;
	int result = 0;
	int i;

	/* MPR training disable */
	if (ddr_training_check_bypass(DDR_BYPASS_MPR_MASK))
		return 0;

	ddr_training_save_reg(&relate_reg, DDR_BYPASS_MPR_MASK);
	for (i = 0; i < DDR_PHY_NUM; i++) {
		if (ddr_training_phy_disable(i))
			continue;

		ddr_training_get_base(i, &base_dmc, &base_phy);
		result += ddr_mpr_training(base_dmc, base_phy);
	}
	ddr_training_restore_reg(&relate_reg);

	return result;
}
#else
static int ddr_mpr_training_func(void)
{
	DDR_WARNING("Not support DDR MPR training.");
	return 0;
}
#endif /* DDR_MPR_TRAINING_CONFIG */

#ifdef DDR_WL_TRAINING_CONFIG
/**
 * ddr_wl_func
 * @void
 *
 *
 */
static int ddr_wl_func(void)
{
	unsigned int base_dmc, base_phy;
	struct tr_relate_reg relate_reg;
	int result = 0;
	int i;

	/* write leveling disable */
	if (ddr_training_check_bypass(DDR_BYPASS_WL_MASK))
		return 0;

	ddr_training_save_reg(&relate_reg, DDR_BYPASS_WL_MASK);
	for (i = 0; i < DDR_PHY_NUM; i++) {
		if (ddr_training_phy_disable(i))
			continue;

		ddr_training_get_base(i, &base_dmc, &base_phy);
		result += ddr_write_leveling(base_dmc, base_phy);
	}
	ddr_training_restore_reg(&relate_reg);

	return result;
}
#else
static int ddr_wl_func(void)
{
	DDR_WARNING("Not support DDR WL training.");
	return 0;
}
#endif /* DDR_WL_TRAINING_CONFIG */

#ifdef DDR_GATE_TRAINING_CONFIG
/**
 * ddr_gating_func
 * @void
 *
 *
 */
static int ddr_gating_func(void)
{
	unsigned int base_dmc, base_phy;
	struct tr_relate_reg relate_reg;
	int result = 0;
	int i;

	/* gate training disable */
	if (ddr_training_check_bypass(DDR_BYPASS_GATE_MASK)) {
		for (i = 0; i < DDR_PHY_NUM; i++) {
			if (ddr_training_phy_disable(i))
				continue;

			ddr_training_get_base(i, &base_dmc, &base_phy);
			/* check hardware gating */
			if (REG_READ(base_phy + DDR_PHY_PHYINITSTATUS)
				& PHY_INITSTATUS_GT_MASK) {
				DDR_FATAL("PHY[%x] hw gating fail.", base_phy);
				ddr_training_stat(DDR_ERR_HW_GATING,
					base_phy, -1, -1);
				return -1;
			}
		}
		return 0;
	}

	ddr_training_save_reg(&relate_reg, DDR_BYPASS_GATE_MASK);
	for (i = 0; i < DDR_PHY_NUM; i++) {
		if (ddr_training_phy_disable(i))
			continue;

		ddr_training_get_base(i, &base_dmc, &base_phy);
		ddr_ddrt_init(base_dmc, DDR_DDRT_MODE_GATE);
		ddr_training_switch_axi(i, &relate_reg);
		result += ddr_gate_training(base_dmc, base_phy);
	}
	ddr_training_restore_reg(&relate_reg);

	return result;
}
#else
static int ddr_gating_func(void)
{
	DDR_WARNING("Not support DDR gate training.");
	return 0;
}
#endif /* DDR_GATE_TRAINING_CONFIG */

#ifdef DDR_DATAEYE_TRAINING_CONFIG
/**
 * ddr_dataeye_training_func
 * @ddrtr_result
 *
 *
 */
static int ddr_dataeye_training_func(void *ddrtr_result)
{
	unsigned int base_dmc, base_phy;
	struct tr_relate_reg relate_reg;
	int result = 0;
	int i;

	/* dataeye training disable */
	if (ddr_training_check_bypass(DDR_BYPASS_DATAEYE_MASK))
		return 0;

	ddr_training_save_reg(&relate_reg, DDR_BYPASS_DATAEYE_MASK);
	for (i = 0; i < DDR_PHY_NUM; i++) {
		if (ddr_training_phy_disable(i))
			continue;

		ddr_training_get_base(i, &base_dmc, &base_phy);
		ddr_ddrt_init(base_dmc, DDR_DDRT_MODE_DATAEYE);
		ddr_training_switch_axi(i, &relate_reg);
		result += ddr_dataeye_training(base_dmc, base_phy,
			ddrtr_result, DDR_DATAEYE_NORMAL_ADJUST);
	}
	ddr_training_restore_reg(&relate_reg);

	return result;
}
#else
static int ddr_dataeye_training_func(void *ddrtr_result)
{
	DDR_WARNING("Not support DDR dataeye training.");
	return 0;
}
#endif /* DDR_DATAEYE_TRAINING_CONFIG */


#ifdef DDR_VREF_TRAINING_CONFIG
/**
 * ddr_vref_training_func
 * @void
 *
 *
 */
static int ddr_vref_training_func(void *ddrtr_result)
{
	unsigned int base_dmc, base_phy;
	struct tr_relate_reg relate_reg;
	int result = 0;
	int i;

	ddr_training_save_reg(&relate_reg, DDR_BYPASS_VREF_HOST_MASK);
	for (i = 0; i < DDR_PHY_NUM; i++) {
		if (ddr_training_phy_disable(i))
			continue;

		ddr_training_get_base(i, &base_dmc, &base_phy);
		ddr_ddrt_init(base_dmc, DDR_DDRT_MODE_DATAEYE);
		ddr_training_switch_axi(i, &relate_reg);

		/* host vref training disable */
		if (!ddr_training_check_bypass(DDR_BYPASS_VREF_HOST_MASK))
			result += ddr_vref_training(base_dmc, base_phy,
				ddrtr_result, DDR_MODE_READ);

#ifdef DDR_PHY_T28_CONFIG
		/* dram vref training enable && DDR4 */
		if (!ddr_training_check_bypass(DDR_BYPASS_VREF_DRAM_MASK)
			&& DMC_CFG_DRAM_TYPE_DDR4 ==
				(REG_READ(base_dmc + DDR_DMC_CFG_DDRMODE)
					& DMC_CFG_DRAM_TYPE_MASK)) {
			result += ddr_vref_training(base_dmc, base_phy,
				ddrtr_result, DDR_MODE_WRITE);
		}
#endif /* DDR_PHY_T28_CONFIG */
	}
	ddr_training_restore_reg(&relate_reg);

	return result;
}
#else
static int ddr_vref_training_func(void *ddrtr_result)
{
	DDR_WARNING("Not support DDR vref training.");
	return 0;
}
#endif /* DDR_VREF_TRAINING_CONFIG */

#ifdef DDR_AC_TRAINING_CONFIG
/**
 * ddr_ac_training_func
 * @void
 *
 *
 */
static int ddr_ac_training_func(void)
{
	int result = 0;
	int i;
	unsigned int base_dmc, base_phy;
	struct tr_relate_reg relate_reg;

	/* AC training disable */
	if (ddr_training_check_bypass(DDR_BYPASS_AC_MASK))
		return 0;

	ddr_training_save_reg(&relate_reg, DDR_BYPASS_AC_MASK);
	for (i = 0; i < DDR_PHY_NUM; i++) {
		if (ddr_training_phy_disable(i))
			continue;

		ddr_training_get_base(i, &base_dmc, &base_phy);
		ddr_ddrt_init(base_dmc, DDR_DDRT_MODE_DATAEYE);
		result += ddr_ac_training(base_dmc, base_phy);
	}
	ddr_training_restore_reg(&relate_reg);

	return result;
}
#else
static int ddr_ac_training_func(void)
{
	DDR_WARNING("Not support DDR AC training.");
	return 0;
}
#endif /* DDR_AC_TRAINING_CONFIG */

/**
 * ddr_training_cmd_result
 * @void
 *
 * Display ddr training result before return to DDR.
 */
static void ddr_training_cmd_result(void)
{
	int phy, i;
	unsigned int base_dmc, base_phy, byte_num;
	unsigned int cfg;

	cfg = REG_READ(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG);

	for (phy = 0; phy < DDR_PHY_NUM; phy++) {

		/* PHY0 bypass */
		if ((cfg & DDR_BYPASS_PHY0_MASK) && 0 == phy)
			continue;

		/* PHY1 bypass */
		if ((cfg & DDR_BYPASS_PHY1_MASK) && 1 == phy)
			continue;

		ddr_training_get_base(phy, &base_dmc, &base_phy);
		byte_num = ddr_phy_get_byte_num(base_dmc);

		DDR_INFO("PHY[%x]:", base_phy);

		/* WDQS */
		for (i = 0; i < byte_num; i++) {
			DDR_INFO("[%x = %x] WDQS Byte(%x) ",
				base_phy + DDR_PHY_DXWDQSDLY(i),
				REG_READ(base_phy + DDR_PHY_DXWDQSDLY(i)), i);
		}

		/* WDQ Phase */
		for (i = 0; i < byte_num; i++) {
			DDR_INFO("[%x = %x] WDQ Phase Byte(%x)",
				base_phy + DDR_PHY_DXNWDQDLY(i),
				REG_READ(base_phy + DDR_PHY_DXNWDQDLY(i)), i);
		}

		/* WDQ BDL */
		for (i = 0; i < byte_num; i++) {
			/* DQ0-DQ3 */
			DDR_INFO("[%x = %x] WDQ BDL DQ(%x-%x)",
				base_phy + DDR_PHY_DXNWDQNBDL0(i),
				REG_READ(base_phy + DDR_PHY_DXNWDQNBDL0(i)),
				(i << 3), ((i << 3) + 3));

			/* DQ4-DQ7 */
			DDR_INFO("[%x = %x] WDQ BDL DQ(%x-%x)",
				base_phy + DDR_PHY_DXNWDQNBDL1(i),
				REG_READ(base_phy + DDR_PHY_DXNWDQNBDL1(i)),
				((i << 3) + 4), ((i << 3) + 7));
		}

		/* WDM */
		for (i = 0; i < byte_num; i++) {
			DDR_INFO("[%x = %x] WDM Byte(%x)",
				base_phy + DDR_PHY_DXNWDQNBDL2(i),
				REG_READ(base_phy + DDR_PHY_DXNWDQNBDL2(i)), i);
		}

		/* Write DO/DOS OE */
		for (i = 0; i < byte_num; i++) {
			DDR_INFO("[%x = %x] Write DQ/DQS OE Byte(%x)",
				base_phy + DDR_PHY_DXNOEBDL(i),
				REG_READ(base_phy + DDR_PHY_DXNOEBDL(i)), i);
		}

		/* RDQS */
		for (i = 0; i < byte_num; i++) {
			DDR_INFO("[%x = %x] RDQS Byte(%x)",
				base_phy + DDR_PHY_DXNRDQSDLY(i),
				REG_READ(base_phy + DDR_PHY_DXNRDQSDLY(i)), i);
		}

		/* RDQ BDL */
		for (i = 0; i < byte_num; i++) {
			/* DQ0-DQ3 */
			DDR_INFO("[%x = %x] RDQ BDL DQ(%x-%x)",
				base_phy + DDR_PHY_DXNRDQNBDL0(i),
				REG_READ(base_phy + DDR_PHY_DXNRDQNBDL0(i)),
				(i << 3), ((i << 3) + 3));

			/* DQ4-DQ7 */
			DDR_INFO("[%x = %x] RDQ BDL DQ(%x-%x)",
				base_phy + DDR_PHY_DXNRDQNBDL1(i),
				REG_READ(base_phy + DDR_PHY_DXNRDQNBDL1(i)),
				((i << 3) + 4), ((i << 3) + 7));
		}

		/* Gate */
		for (i = 0; i < byte_num; i++) {
			DDR_INFO("[%x = %x] Gate Byte(%x)",
				base_phy + DDR_PHY_DXNRDQSGDLY(i),
				REG_READ(base_phy + DDR_PHY_DXNRDQSGDLY(i)), i);
		}

		DDR_INFO("[%x = %x] CS",
				base_phy + DDR_PHY_ACCMDBDL2,
				REG_READ(base_phy + DDR_PHY_ACCMDBDL2));

		DDR_INFO("[%x = %x] CLK",
				base_phy + DDR_PHY_ACPHYCTL7,
				REG_READ(base_phy + DDR_PHY_ACPHYCTL7));

		/* HOST Vref */
		DDR_PHY_VREF_HOST_DISPLAY_CMD(base_phy, byte_num);

		/* DRAM Vref */
		DDR_PHY_VREF_DRAM_DISPLAY_CMD(base_phy, byte_num);
	}
}

/**
 * ddr_training_cmd_entry
 * @cmd_st
 *
 * DDR training command entry. Call by cmd_ddr_handle().
 */
struct ddr_training_result_st *ddr_training_cmd_entry(
		struct ddr_cmd_st *cmd_st)
{
	int result = 0;
	unsigned int cfg;

	ddr_training_addr_start = cmd_st->start;
	ddr_training_addr_end   = cmd_st->start + cmd_st->length;
	ddr_print_level         = cmd_st->level;

	cfg = REG_READ(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG);

	DDR_INFO("DDR Training Version: "DDR_TRAINING_VER);

	DDR_DEBUG("DDR training command entry. Sysctl[%x = %x]",
		(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG), cfg);

	ddr_training_result_init();

	switch (cmd_st->cmd) {
	case DDR_TRAINING_CMD_SW:
		result = ddr_sw_training_if((void *)&ddrt_result_sram);
		break;
	case DDR_TRAINING_CMD_DATAEYE:
		result = ddr_dataeye_training_func((void *)&ddrt_result_sram);
		break;
	case DDR_TRAINING_CMD_HW:
		result = ddr_hw_training_func();
		break;
	case DDR_TRAINING_CMD_MPR:
		result = ddr_mpr_training_func();
		break;
	case DDR_TRAINING_CMD_WL:
		result = ddr_wl_func();
		break;
	case DDR_TRAINING_CMD_GATE:
		result = ddr_gating_func();
		break;
	case DDR_TRAINING_CMD_VREF:
		result = ddr_vref_training_func((void *)&ddrt_result_sram);
		break;
	case DDR_TRAINING_CMD_AC:
		result = ddr_ac_training_func();
		break;
	case DDR_TRAINING_CMD_SW_NO_WL:
		/* wl bypass */
		REG_WRITE(cfg | DDR_BYPASS_WL_MASK,
			DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG);
		result = ddr_sw_training_if((void *)&ddrt_result_sram);
		/* restore cfg */
		REG_WRITE(cfg,
			DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG);
		break;
	default:
		result = -1;
		break;
	}

	DDR_DEBUG("DDR training result[%x]", result);
	ddr_training_cmd_result();

	if (!result)
		return &ddrt_result_sram;
	else
		return 0;
}
