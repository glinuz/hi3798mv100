/**
 * cmd_ddr_training_v2.c
 *
 * Copyright (c) 2009-2014, HiSilicon Technologies Co., Ltd.
 * All rights reserved.
 *
 * DDR training command define.
 */

#include <common.h>
#include <command.h>
#include "ddr_interface.h"

#define DDR_TRAINING_ENV            "ddrtr"
#define DDR_TRAINING_ENV_UN         "unddrtr"

#define DDR_TRAINING_DDRT_START_OFFSET  0x400000  /* 4M */
#define DDR_TRAINING_DDRT_LENGTH  0x1000000  /* 16M  at lease 0x8000 */

#define DDR_CMD_SW_STR              "training"
#define DDR_CMD_TR_STR              "tr"
#define DDR_CMD_HW_STR              "hw"
#define DDR_CMD_MPR_STR             "mpr"
#define DDR_CMD_WL_STR              "wl"
#define DDR_CMD_GATE_STR            "gate"
#define DDR_CMD_DATAEYE_STR         "dataeye"
#define DDR_CMD_VREF_STR            "vref"
#define DDR_CMD_AC_STR              "ac"
#define DDR_CMD_LPCA_STR            "lpca"
#define DDR_CMD_LOG_STR             "log"
#define DDR_CMD_BOOT_STR            "boot"

static struct ddr_training_result_st ddrtr_result_st; /* DDR training result */
static struct ddr_training_reg_st ddr_reg_st;   /* DDR training register */
static int ddr_log_level = DDR_LOG_ERROR;       /* DDR training log level */

#ifdef CONFIG_DDR_TRAINING_BOOTENV
extern int do_saveenv(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
#endif
#ifdef DDR_TRAINING_EXEC_TIME
/**
 * cmd_exec_timer_start
 * @void
 *
 * Start timer for calculate DDR training execute time.
 * NOTE: Just only for debug.
 */
static void cmd_exec_timer_start(void)
{
	/* timer start */
	REG_WRITE(0, 0xF8002000); /* REG_BASE_TIMER01 + REG_TIMER_RELOAD */
	/* TIMER_EN  | TIMER_MODE |TIMER_PRE | TIMER_SIZE, REG_TIMER_CONTROL */
	REG_WRITE(0xc2, 0xF8002008);
	REG_WRITE(0xffffffff, 0xF8002000);
}

/**
 * cmd_exec_timer_stop
 * @void
 *
 * Stop timer for calculate DDR training execute time.
 * NOTE: Just only for debug.
 */
static void cmd_exec_timer_stop(void)
{
	/* timer stop */
	REG_WRITE(0, 0xF8002008); /* REG_TIMER_CONTROL */
	/* REG_TIMER_VALUE, 24MHz*/
	printf("DDR training execute time: [%d]us\n",
		(0xffffffff - REG_READ(0xF8002004)) / 24);
}
#endif

/**
 * cmd_ddr_match
 * @str
 * @cmd
 *
 * Match string command.
 * NOTE: Write leveling not support run repeatedly,
 * so limit WL only run one time.
 */
static int cmd_ddr_match(const char *str, int *cmd)
{
	static int wl_done;  /* Write leveling control */

	if (!strncmp(str, DDR_CMD_SW_STR, sizeof(DDR_CMD_SW_STR))) {
		*cmd = DDR_TRAINING_CMD_SW_NO_WL;
	} else if (!strncmp(str, DDR_CMD_TR_STR, sizeof(DDR_CMD_TR_STR))) {
		if (wl_done) {
			*cmd = DDR_TRAINING_CMD_SW_NO_WL;
		} else {
			wl_done++;
			*cmd = DDR_TRAINING_CMD_SW;
		}
	} else if (!strncmp(str, DDR_CMD_HW_STR, sizeof(DDR_CMD_HW_STR))) {
		*cmd = DDR_TRAINING_CMD_HW;
	} else if (!strncmp(str, DDR_CMD_MPR_STR, sizeof(DDR_CMD_MPR_STR))) {
		*cmd = DDR_TRAINING_CMD_MPR;
	} else if (!strncmp(str, DDR_CMD_WL_STR, sizeof(DDR_CMD_WL_STR))) {
		if (wl_done) {
			printf("WL not support run repeatedly. %s",
				"Already done once, can not do again.\n");
			return -1;
		} else {
			*cmd = DDR_TRAINING_CMD_WL;
			wl_done++;
		}
	} else if (!strncmp(str, DDR_CMD_GATE_STR, sizeof(DDR_CMD_GATE_STR))) {
		*cmd = DDR_TRAINING_CMD_GATE;
	} else if (!strncmp(str, DDR_CMD_DATAEYE_STR,
				sizeof(DDR_CMD_DATAEYE_STR))) {
		*cmd = DDR_TRAINING_CMD_DATAEYE;
	} else if (!strncmp(str, DDR_CMD_VREF_STR, sizeof(DDR_CMD_VREF_STR))) {
		*cmd = DDR_TRAINING_CMD_VREF;
	} else if (!strncmp(str, DDR_CMD_AC_STR, sizeof(DDR_CMD_AC_STR))) {
		*cmd = DDR_TRAINING_CMD_AC;
	} else if (!strncmp(str, DDR_CMD_LPCA_STR, sizeof(DDR_CMD_LPCA_STR))) {
		*cmd = DDR_TRAINING_CMD_LPCA;
	} else {
		printf("Command [ddr %s] is unsupport.\n", str);
		return -1;
	}

	return 0;
}

/**
 * cmd_ddr_handle
 * @cmd
 *
 * Handle DDR training.
 * Copy training codes from DDR to SRAM.
 */
static int cmd_ddr_handle(int cmd)
{
	struct ddr_training_result_st *result_st;
	struct ddr_cmd_st cmd_st;

	cmd_st.cmd = cmd;
	cmd_st.level = ddr_log_level;
	cmd_st.start = TEXT_BASE + DDR_TRAINING_DDRT_START_OFFSET;
	cmd_st.length = DDR_TRAINING_DDRT_LENGTH;

	printf("DDR training area: 0x%08X - 0x%08X\n",
		   cmd_st.start, cmd_st.start + cmd_st.length);

#ifdef DDR_TRAINING_EXEC_TIME
	cmd_exec_timer_start();
#endif

	result_st = ddr_cmd_training_if(&cmd_st);

#ifdef DDR_TRAINING_EXEC_TIME
	cmd_exec_timer_stop();
#endif

	if (!result_st)
		return -1;

	/* copy training result from SRAM to DDR */
	memcpy((void *)&ddrtr_result_st, result_st,
			sizeof(ddrtr_result_st));
	printf("DDR training finished.\n");

	return 0;
}

/**
 * cmd_ddr_dispatch
 * @cmd
 *
 * DDR training cmd dispatch.
 */
static int cmd_ddr_dispatch(int cmd)
{
	int result = 0;

	result = cmd_ddr_handle(cmd);
	switch (cmd) {
	case DDR_TRAINING_CMD_SW:
	case DDR_TRAINING_CMD_SW_NO_WL:
		ddr_cmd_result_display(&ddrtr_result_st,
			DDR_TRAINING_CMD_DATAEYE
			| DDR_TRAINING_CMD_LPCA);
		break;
	case DDR_TRAINING_CMD_DATAEYE:
	case DDR_TRAINING_CMD_VREF:
		ddr_cmd_result_display(&ddrtr_result_st,
			DDR_TRAINING_CMD_DATAEYE);
		break;
	case DDR_TRAINING_CMD_WL:
	case DDR_TRAINING_CMD_GATE:
	case DDR_TRAINING_CMD_HW:
		break;
	case DDR_TRAINING_CMD_LPCA:
		ddr_cmd_result_display(&ddrtr_result_st,
			DDR_TRAINING_CMD_LPCA);
		break;
	default:
		break;
	}

	ddr_reg_result_display(&ddr_reg_st);
	return result;
}

/**
 * cmd_ddr_set_log_level
 * @argv[]
 *
 * Set DDR training log level.
 */
int cmd_ddr_set_log_level(char *argv[])
{
	int level;
	const char *str;

	str = argv[1];
	if (strncmp(str, DDR_CMD_LOG_STR, sizeof(DDR_CMD_LOG_STR))) {
		printf("Command [ddr %s] is unsupport.\n", str);
		return -1;
	}

	str = argv[2];
	if (!strncmp(str, DDR_LOG_INFO_STR, sizeof(DDR_LOG_INFO_STR))) {
		level = DDR_LOG_INFO;
	} else if (!strncmp(str, DDR_LOG_DEBUG_STR,
				sizeof(DDR_LOG_DEBUG_STR))) {
		level = DDR_LOG_DEBUG;
	} else if (!strncmp(str, DDR_LOG_WARNING_STR,
				sizeof(DDR_LOG_WARNING_STR))) {
		level = DDR_LOG_WARNING;
	} else if (!strncmp(str, DDR_LOG_ERROR_STR,
				sizeof(DDR_LOG_ERROR_STR))) {
		level = DDR_LOG_ERROR;
	} else if (!strncmp(str, DDR_LOG_FATAL_STR,
				sizeof(DDR_LOG_FATAL_STR))) {
		level = DDR_LOG_FATAL;
	} else {
		printf("Command [ddr log %s] is unsupport.\n", str);
		return -1;
	}

	ddr_log_level = level;
	printf("Set DDR training log level [%s] suc.\n", str);

	return 0;
}

#ifdef DDR_TRAINING_DEBUG
/**
 * cmd_ddr_display_boot_result
 * @str
 *
 * Display boot ddr training result for debug.
 */
static int cmd_ddr_display_boot_result(const char *str)
{
	if (strncmp(str, DDR_CMD_BOOT_STR, sizeof(DDR_CMD_BOOT_STR)))
		return -1;

	printf("Display boot result form [0x%08x].\n",
		DDR_TRAINING_BOOT_RESULT_ADDR);

	/* if have not saved when boot, it will be crash */
	ddr_cmd_result_display(
		(struct ddr_training_result_st *)DDR_TRAINING_BOOT_RESULT_ADDR,
		DDR_TRAINING_CMD_DATAEYE);

	ddr_reg_result_display(&ddr_reg_st);

	return 0;
}
#endif

#ifdef CONFIG_DDR_TRAINING_BOOTENV
/**
 * get_ddrtr_result_by_env
 * @ddr_reg
 *
 * Parse bootenv string to get DDR register.
 */
static int get_ddrtr_result_by_env(struct ddr_training_reg_st *ddr_reg)
{
	unsigned int i;
	unsigned int count;
	char *str = getenv(DDR_TRAINING_ENV);

	if (!str)
		return -1;

	while (*str == ' ')
		str++;

	/* format: 0xffffffff=0xffffffff 0xffffffff=0xffffffff */
	for (i = 0; *str && i < DDR_TRAINING_REG_MAX; i++) {
		ddr_reg->reg[i].addr = simple_strtoul(str, &str, 16);
		if (!*str++)
			break;
		ddr_reg->reg[i].val = simple_strtoul(str, &str, 16);
		while (*str == ' ')
			str++;
	}
	count = i;

	/* other register address set 0 */
	for (i = count; i < DDR_TRAINING_REG_MAX; i++)
		ddr_reg->reg[i].addr = 0;

	return ((count > 0) ? 0 : -1);
}

/**
 * check_ddr_training
 * @void
 *
 * Check whether need to do DDR training.
 */
int check_ddr_training(void)
{
	int i;
	struct ddr_training_reg_st *ddr_reg = &ddr_reg_st;
	char *s = getenv(DDR_TRAINING_ENV_UN);

	/* check bootenv "unddrtr" */
	if (s && (*s == 'y' || *s == 'Y'))   /* not need to do DDR training*/
		return 0;

	/* check bootenv "ddrtr" whether has been set */
	if (get_ddrtr_result_by_env(ddr_reg)) {
		/* Do ddr training and set bootenv and save */
		if (cmd_ddr_handle(DDR_TRAINING_CMD_SW))
			return 0;

		setenv(DDR_TRAINING_ENV, ddr_cmd_result_dump(ddr_reg, ' '));
		return do_saveenv(NULL, 0, 0, NULL);
	}

	printf("Set training value from bootenv to DDR controller.\n");
	for (i = 0; i < DDR_TRAINING_REG_MAX; i++) {
		if (0 == ddr_reg->reg[i].addr)
			continue;

		REG_WRITE(ddr_reg->reg[i].val, ddr_reg->reg[i].addr);
	}

	return 0;
}
#else
int check_ddr_training(void) { return 0; }
#endif /* CONFIG_DDR_TRAINING_BOOTENV */

/**
 * do_ddr_training
 * @cmdtp
 * @flag
 * @argc
 * @argv[]
 *
 * Accept DDR training cmd.
 * Set training result to env without save.
 */
int do_ddr_training(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	const char *str;
	int cmd;

	if (argc < 2 || argc > 3) {
		cmd_usage(cmdtp);
		return -1;
	} else if (argc == 3) {
		return cmd_ddr_set_log_level(argv);
	}

	str = argv[1];
#ifdef DDR_TRAINING_DEBUG
	if (!cmd_ddr_display_boot_result(str))
		return 0;
#endif

	if (cmd_ddr_match(str, &cmd))
		return -1;

	if (cmd_ddr_dispatch(cmd))
		return -1;

#ifdef CONFIG_DDR_TRAINING_BOOTENV
	setenv(DDR_TRAINING_ENV, ddr_cmd_result_dump(&ddr_reg_st, ' '));
#endif

	return 0;
}


U_BOOT_CMD(
	ddr, CONFIG_SYS_MAXARGS, 1,	do_ddr_training,
	"ddr training function",
	"training    - DDR sofeware(Gate/Dataeye/Vref) training.\n"
	"ddr tr          - DDR sofeware(WL/Gate/Dataeye/Vref) training.\n"
	"ddr wl          - DDR Write leveling training.\n"
	"ddr gate        - DDR gate training.\n"
	"ddr dataeye     - DDR dataeye training and display training result.\n"
	"ddr vref        - DDR vref training.\n"
	"ddr hw          - DDR hardware training.\n"
	"ddr mpr         - DDR Multi-Purpose Register training.\n"
	"ddr ac          - DDR address command training.\n"
	"ddr lpca        - LPDDR command address training.\n"
	"ddr log [level] - DDR log level. [info,debug,warning,error,fatal]\n"
#ifdef DDR_TRAINING_DEBUG
	"ddr boot        - Display boot DDR training reslut.\n"
#endif
);
