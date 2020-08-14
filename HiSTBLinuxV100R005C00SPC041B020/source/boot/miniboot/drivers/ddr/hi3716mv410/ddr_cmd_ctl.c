/**
 * ddr_cmd_ctl.c
 *
 * Copyright (c) 2009-2014, HiSilicon Technologies Co., Ltd.
 * All rights reserved.
 *
 * DDR training command control.
 */

//#include <common.h>
#include <stdio.h>
#include <string.h>
#include "ddr_interface.h"
#include "ddr_training_impl.h"

#if PHY_DQ_BDL_LEVEL == 32
#define PRINT_DATAEYE_WIN(dq_num, range, dqs, dq, win) \
	do {\
		printf("%-4d", dq_num);\
		for (k = 0; k < PHY_DQ_BDL_LEVEL; k++) {\
			if (k >= (range >> DDR_DATAEYE_RESULT_BIT)\
				&& k <= (range & DDR_DATAEYE_RESULT_MASK)) {\
				printf("%-3s", "-");\
			} else {\
				printf("%-3s", "X");\
			} \
		} \
		printf(" 0x%08x  0x%-4x%-4d%-4d\n", range, dqs, dq, win); \
	} while (0)

#define PRINT_DATAEYE_TITLE(phase) \
	do { \
		printf("%-4s", "DQ"); \
		for (k = 0; k < PHY_DQ_BDL_LEVEL; k++) { \
			printf("%-3d", k); \
		} \
		printf(" %-10s  %-6s%-4s%-4s\n", \
			"RANGE", phase, "DQ", "WIN"); \
	} while (0)
#else
#define PRINT_DATAEYE_WIN(dq_num, range, dqs, dq, win) \
	do {\
		printf("%-4d", dq_num); \
		for (k = 0; k < PHY_DQ_BDL_LEVEL; k++) { \
			if (k >= (range >> DDR_DATAEYE_RESULT_BIT)\
				&& k <= (range & DDR_DATAEYE_RESULT_MASK)) {\
				printf("%-1s", "-"); \
			} else {\
				printf("%-1s", "X"); \
			} \
		} \
		printf(" 0x%08x  0x%-4x%-4d%-4d\n", range, dqs, dq, win); \
	} while (0)

#define PRINT_DATAEYE_TITLE(phase) \
	do { \
		printf("%-4s", "DQ");\
		for (k = 0; k < PHY_DQ_BDL_LEVEL; k++) { \
			if (0 == k % 4) \
				printf("%-4d", k); \
		} \
		printf(" %-10s  %-6s%-4s%-4s\n", \
			"RANGE", phase, "DQ", "WIN"); \
	} while (0)
#endif

extern char ddr_training_cmd_start[];  /* DDR training code start address */
extern char ddr_training_cmd_end[];    /* DDR training code end address */

/**
 * ddr_training_reg_read_phy
 * @base_dmc
 * @base_phy
 * @ddr_reg
 * @arr_index
 *
 * Read DDR training register per PHY.
 */
static void ddr_training_reg_read_phy(unsigned int base_dmc,
		unsigned int base_phy, struct ddr_training_reg_st *ddr_reg,
		unsigned int arr_index)
{
	int byte_num;
	int index = 0;
	int i;

	byte_num = ((REG_READ(base_dmc + DDR_DMC_CFG_DDRMODE)
			>> DMC_MEM_WIDTH_BIT) & DMC_MEM_WIDTH_MASK) << 1;

	index    = arr_index;

	if ((DDR_TRAINING_REG_MAX - index) < DDR_TRAINING_REG_NUM) {
		printf("Not enough space. Free[%d] Need[%d]",
				(DDR_TRAINING_REG_MAX - index),
				DDR_TRAINING_REG_NUM);
		return;
	}

	/* WDQS */
	for (i = 0; i < byte_num; i++) {
		snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX,
			"WDQS Byte%d", i);
		ddr_reg->reg[index++].addr = base_phy + DDR_PHY_DXWDQSDLY(i);
	}

	/* WDQ Phase */
	for (i = 0; i < byte_num; i++) {
		snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX,
			"WDQ Phase Byte%d", i);
		ddr_reg->reg[index++].addr = base_phy + DDR_PHY_DXNWDQDLY(i);
	}

	/* WDQ BDL */
	for (i = 0; i < byte_num; i++) {
		/* DQ0-DQ3 */
		snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX,
			"WDQ BDL DQ%d-DQ%d", (i << 3), ((i << 3) + 3));
		ddr_reg->reg[index++].addr = base_phy + DDR_PHY_DXNWDQNBDL0(i);
		/* DQ4-DQ7 */
		snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX,
			"WDQ BDL DQ%d-DQ%d", ((i << 3) + 4), ((i << 3) + 7));
		ddr_reg->reg[index++].addr = base_phy + DDR_PHY_DXNWDQNBDL1(i);
	}

	/* WDM */
	for (i = 0; i < byte_num; i++) {
		snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX,
			"WDM Byte%d", i);
		ddr_reg->reg[index++].addr = base_phy + DDR_PHY_DXNWDQNBDL2(i);
	}

	/* Write DO/DOS OE */
	for (i = 0; i < byte_num; i++) {
		snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX,
			"Write DQ/DQS OE Byte%d", i);
		ddr_reg->reg[index++].addr = base_phy + DDR_PHY_DXNOEBDL(i);
	}

	/* RDQS */
	for (i = 0; i < byte_num; i++) {
		snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX,
			"RDQS Byte%d", i);
		ddr_reg->reg[index++].addr = base_phy + DDR_PHY_DXNRDQSDLY(i);
	}

	/* RDQ BDL */
	for (i = 0; i < byte_num; i++) {
		/* DQ0-DQ3 */
		snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX,
			"RDQ BDL DQ%d-DQ%d", (i << 3), ((i << 3) + 3));
		ddr_reg->reg[index++].addr = base_phy + DDR_PHY_DXNRDQNBDL0(i);
		/* DQ4-DQ7 */
		snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX,
			"RDQ BDL DQ%d-DQ%d", ((i << 3) + 4), ((i << 3) + 7));
		ddr_reg->reg[index++].addr = base_phy + DDR_PHY_DXNRDQNBDL1(i);
	}

	/* Gate */
	for (i = 0; i < byte_num; i++) {
		snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX,
			"Gate Byte%d", i);
		ddr_reg->reg[index++].addr = base_phy + DDR_PHY_DXNRDQSGDLY(i);
	}

	/* AC CS */
	snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX, "CS");
	ddr_reg->reg[index++].addr = base_phy + DDR_PHY_ACCMDBDL2;

	/* AC CLK */
	snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX, "CLK");
	ddr_reg->reg[index++].addr = base_phy + DDR_PHY_ACPHYCTL7;

	/* HOST Vref */
	DDR_PHY_VREF_HOST_DISPLAY(base_phy, ddr_reg, index, byte_num);

	/* DRAM Vref */
	DDR_PHY_VREF_DRAM_DISPLAY(base_phy, ddr_reg, index, byte_num);

	/* read register */
	for (i = 0; i < index; i++) {
		if (0 == ddr_reg->reg[i].addr)
			continue;

		ddr_reg->reg[i].val = REG_READ(ddr_reg->reg[i].addr);
	}
}

/**
 * ddr_training_reg_read
 * @ddr_reg
 *
 * Read DDR training register.
 */
static void ddr_training_reg_read(struct ddr_training_reg_st *ddr_reg)
{
	int i;
	unsigned int cfg;

	cfg = REG_READ(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG);

	/* init */
	for (i = 0; i < DDR_TRAINING_REG_MAX; i++)
		ddr_reg->reg[i].addr = 0;

	for (i = 0; i < DDR_PHY_NUM; i++) {
		if (0 == i && !(cfg & DDR_BYPASS_PHY0_MASK))
			ddr_training_reg_read_phy(DDR_REG_BASE_DMC0,
				DDR_REG_BASE_PHY0, ddr_reg, 0);
		else if (1 == i && !(cfg & DDR_BYPASS_PHY1_MASK))
			ddr_training_reg_read_phy(DDR_REG_BASE_DMC1,
				DDR_REG_BASE_PHY1, ddr_reg,
				DDR_TRAINING_REG_NUM);
	}
}

/**
 * ddr_cmd_result_print
 * @ddrtr_data
 *
 *
 */
static int ddr_cmd_result_print(struct ddr_training_data_st *ddrtr_data)
{
	unsigned int i, k, j;
	unsigned int dq_num, dqs, dq, win, win_min, win_max, win_sum;

	win_min = PHY_DQ_BDL_LEVEL;
	win_max = 0;
	win_sum = 0;
	printf("Write window of prebit-deskew:\n");
	printf("--------------------------------------------------------\n");
	PRINT_DATAEYE_TITLE("DQPH");
	for (j = 0; j < ddrtr_data->byte_num; j++) {
		dqs = (REG_READ(ddrtr_data->base_phy + DDR_PHY_DXNWDQDLY(j))
				>> PHY_WDQ_PHASE_BIT) & PHY_WDQ_PHASE_MASK;
		for (i = 0; i < DDR_PHY_BIT_NUM; i++) {
			dq_num = (j << 3) + i;
			win = ddrtr_data->wr_bit_best[dq_num]
				>> DDR_DATAEYE_RESULT_BIT;
			if (win < win_min)
				win_min = win;
			if (win > win_max)
				win_max = win;
			win_sum += win;
			dq =  ddrtr_data->wr_bit_best[dq_num]
				& DDR_DATAEYE_RESULT_MASK;
			PRINT_DATAEYE_WIN(dq_num,
					ddrtr_data->wr_bit_result[dq_num],
					dqs, dq, win);
		}
	}
	printf("--------------------------------------------------------\n");
	printf("Sum WIN: %d. Avg WIN: %d\n", win_sum,
		win_sum / (ddrtr_data->byte_num * DDR_PHY_BIT_NUM));
	printf("Min WIN: %d. DQ Index: ", win_min);
	for (i = 0; i < DDR_BIT_MAX; i++) {
		win = ddrtr_data->wr_bit_best[i]
			>> DDR_DATAEYE_RESULT_BIT;
		if (win == win_min)
			printf("%d ", i);
	}
	printf("\nMax WIN: %d. DQ Index: ", win_max);
	for (i = 0; i < DDR_BIT_MAX; i++) {
		win = ddrtr_data->wr_bit_best[i]
			>> DDR_DATAEYE_RESULT_BIT;
		if (win == win_max)
			printf("%d ", i);
	}
	printf("\n\n");

	win_min = PHY_DQ_BDL_LEVEL;
	win_max = 0;
	win_sum = 0;
	printf("Read window of prebit-deskew:\n");
	printf("--------------------------------------------------------\n");
	PRINT_DATAEYE_TITLE("DQS");
	for (j = 0; j < ddrtr_data->byte_num; j++) {
		dqs = REG_READ(ddrtr_data->base_phy + DDR_PHY_DXNRDQSDLY(j))
				& PHY_RDQS_BDL_MASK;
		for (i = 0; i < DDR_PHY_BIT_NUM; i++) {
			dq_num = (j << 3) + i;
			win = ddrtr_data->rd_bit_best[dq_num]
				>> DDR_DATAEYE_RESULT_BIT;
			if (win < win_min)
				win_min = win;
			if (win > win_max)
				win_max = win;
			win_sum += win;
			dq =  ddrtr_data->rd_bit_best[dq_num]
				& DDR_DATAEYE_RESULT_MASK;
			PRINT_DATAEYE_WIN(dq_num,
					ddrtr_data->rd_bit_result[dq_num],
					dqs, dq, win);
		}
	}
	printf("--------------------------------------------------------\n");
	printf("Sum WIN: %d. Avg WIN: %d\n", win_sum,
		win_sum / (ddrtr_data->byte_num * DDR_PHY_BIT_NUM));
	printf("Min WIN: %d. DQ Index: ", win_min);
	for (i = 0; i < DDR_BIT_MAX; i++) {
		win = ddrtr_data->rd_bit_best[i]
			>> DDR_DATAEYE_RESULT_BIT;
		if (win == win_min)
			printf("%d ", i);
	}
	printf("\nMax WIN: %d. DQ Index: ", win_max);
	for (i = 0; i < DDR_BIT_MAX; i++) {
		win = ddrtr_data->rd_bit_best[i]
			>> DDR_DATAEYE_RESULT_BIT;
		if (win == win_max)
			printf("%d ", i);
	}
	printf("\n\n");
	return 0;
}

/**
 * ddr_cmd_result_display
 * @ddrtr_result
 *
 */
void ddr_cmd_result_display(struct ddr_training_result_st *ddrtr_result)
{
	int i;
	unsigned int cfg;

	cfg = REG_READ(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG);

	for (i = 0; i < DDR_PHY_NUM; i++) {

		/* PHY0 bypass */
		if ((cfg & DDR_BYPASS_PHY0_MASK) && 0 == i)
			continue;

		/* PHY1 bypass */
		if ((cfg & DDR_BYPASS_PHY1_MASK) && 1 == i)
			continue;

		if (2 == DDR_PHY_NUM)
			printf("\r\n[PHY%d]:\r\n", i);
		ddr_cmd_result_print(&ddrtr_result->ddrtr_data[i]);
	}
}

/**
 * ddr_reg_result_display
 * @ddr_reg
 *
 * Display DDR training register.
 */
void ddr_reg_result_display(struct ddr_training_reg_st *ddr_reg)
{
	int i;
	unsigned int cfg;

	ddr_training_reg_read(ddr_reg);
	cfg = REG_READ(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG);

	for (i = 0; i < DDR_TRAINING_REG_MAX; i++) {
		if (0 == ddr_reg->reg[i].addr)
			continue;

		/* PHY0 bypass */
		if ((cfg & DDR_BYPASS_PHY0_MASK)
			&& i < DDR_TRAINING_REG_NUM)
			continue;

		/* PHY1 bypass */
		if ((cfg & DDR_BYPASS_PHY1_MASK)
			&& i >= DDR_TRAINING_REG_NUM)
			continue;

		if (0 == i && 2 == DDR_PHY_NUM)
			printf("\r\n[PHY0]:\r\n");

		if (DDR_TRAINING_REG_NUM  == i && 2 == DDR_PHY_NUM)
			printf("\r\n[PHY1]:\r\n");

		printf("[0x%08x = 0x%08x] %-32s", ddr_reg->reg[i].addr,
				ddr_reg->reg[i].val, ddr_reg->reg[i].name);

		if (i % 2)
			printf("\r\n");
	}

	printf("\r\n");
}

/**
 * ddr_cmd_get_entry
 * @void
 *
 * Get DDR training command function entry address.
 */
void *ddr_cmd_get_entry(void)
{
	char *src_ptr = 0;
	char *dst_ptr;
	unsigned int length = 0;
	unsigned int cfg;

	cfg = REG_READ(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG);

	src_ptr = ddr_training_cmd_start;
	dst_ptr = (char *)(DDR_TRAINING_RUN_STACK);
	length  = ddr_training_cmd_end - src_ptr;

	if (!src_ptr || !length) {
		printf("DDR training is unsupport.\n");
		return 0;
	}

	printf("%s entry[0x%08X] size[%d]byte cfg[0x%08X = 0x%08X]\n",
		"DDR training cmd", DDR_TRAINING_RUN_STACK, length,
		(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG), cfg);

	if (((cfg & DDR_BYPASS_PHY0_MASK) && (cfg & DDR_BYPASS_PHY1_MASK))
		|| ((1 == DDR_PHY_NUM) && (cfg & DDR_BYPASS_PHY0_MASK))) {
		printf("Please config DDR training item. Bypass bit:\n"
			"[0]PHY0            : 0x1\n"
			"[1]PHY1            : 0x2\n"
			"[4]Write Leveling  : 0x10\n"
			"[8]Gate            : 0x100\n"
			"[16]Dataeye        : 0x10000\n"
			"[20]HW             : 0x100000\n"
			"[21]MPR            : 0x200000\n"
			"[22]AC             : 0x400000\n"
			"[24]Host Vref      : 0x1000000\n"
			"[25]Dram Vref      : 0x2000000\n"
			"[28]Dataeye Adjust : 0x10000000\n"
			"[29]WL Write Adjust: 0x20000000\n"
			"[30]HW Read Adjust : 0x40000000\n");
		return 0;
	}

	ddr_cmd_prepare_copy();
	memcpy(dst_ptr, src_ptr, length);
	return (void *) dst_ptr;
}

/**
 * ddr_cmd_training_if
 * @cmd_st
 *
 * Copy training codes from DDR to SRAM and do ddr training.
 */
struct ddr_training_result_st *ddr_cmd_training_if(struct ddr_cmd_st *cmd_st)
{
	ddr_cmd_entry_func entry;
	struct ddr_training_result_st *result_st;

	entry = (ddr_cmd_entry_func)ddr_cmd_get_entry();
	if (!entry)
		return 0;

	asm("mcr p15, 0, r0, c7, c5, 0");
	asm("mcr p15, 0, r0, c7, c10, 4");

	/* save site before execute cmd */
	ddr_cmd_site_save();

	/* entry = ddr_training_cmd_entry()*/
	result_st = entry(cmd_st);

	/* restore site before execute cmd */
	ddr_cmd_site_restore();

	if (!result_st) {
		printf("DDR training fail\n");
		return 0;
	}

	return result_st;
}

#ifdef CONFIG_DDR_TRAINING_BOOTENV
/**
 * ddr_cmd_result_dump
 * @result:
 * @flags
 *
 * Dump DDR training register value.
 */
char *ddr_cmd_result_dump(struct ddr_training_reg_st *ddr_reg, char flags)
{
	int i;
	char *ptr;
	/* 22 = strlen("0xffffffff=0xffffffff\n") */
	static char buf[(DDR_TRAINING_REG_MAX * 22) + 1] = {0};
	unsigned int length = 22;

	ddr_training_reg_read(ddr_reg);

	ptr = buf;
	buf[0] = '\0';
	for (i = 0;
		i < DDR_TRAINING_REG_MAX && (ptr - buf) < sizeof(buf); i++) {
		if (0 == ddr_reg->reg[i].addr)
			break;

		ptr += snprintf(ptr, length,
				"0x%08x=0x%08x%c",
				ddr_reg->reg[i].addr,
				ddr_reg->reg[i].val, flags);
	}

	return buf;
}
#endif
