/**
 * ddr_interface.h
 *
 * Copyright (c) 2009-2014, HiSilicon Technologies Co., Ltd.
 * All rights reserved.
 *
 * DDR training interface define.
 */

/* register operations */
#define REG_READ(addr)                (*(volatile unsigned int *)(addr))
#define REG_WRITE(val, addr)          ((*(volatile unsigned *) (addr)) = (val))

#define DDR_BYTE_MAX                  4        /* support max byte */
#define DDR_BIT_MAX                   (DDR_BYTE_MAX * 8) /* support max bit */
#define DDR_REG_NAME_MAX              32       /* register name */

#define DDR_SUPPORT_PHY_MAX           2        /* support max */
/* DDR training register number. */
#define DDR_TRAINING_REG_NUM          50
/* register max. */
#define DDR_TRAINING_REG_MAX	(DDR_TRAINING_REG_NUM * DDR_SUPPORT_PHY_MAX)

#define DDR_TRAINING_CMD_SW           1
#define DDR_TRAINING_CMD_HW           2
#define DDR_TRAINING_CMD_MPR          3
#define DDR_TRAINING_CMD_WL           4
#define DDR_TRAINING_CMD_GATE         5
#define DDR_TRAINING_CMD_DATAEYE      6
#define DDR_TRAINING_CMD_VREF         7
#define DDR_TRAINING_CMD_AC           8
#define DDR_TRAINING_CMD_SW_NO_WL     9

/*******log level ********************/
#define DDR_LOG_INFO_STR              "info"
#define DDR_LOG_DEBUG_STR             "debug"
#define DDR_LOG_WARNING_STR           "warning"
#define DDR_LOG_ERROR_STR             "error"
#define DDR_LOG_FATAL_STR             "fatal"

#define DDR_LOG_INFO                  (1 << 0)
#define DDR_LOG_DEBUG                 (1 << 1)
#define DDR_LOG_WARNING               (1 << 2)
#define DDR_LOG_ERROR                 (1 << 3)
#define DDR_LOG_FATAL                 (1 << 4)

#define DDR_TRAINING_BOOT_RESULT_ADDR (TEXT_BASE + 0x1000000)  /* boot + 16M */

#define DDR_TRAINING_VER "V1.0.1 20151103"

struct ddr_training_data_st {
	unsigned int base_dmc;
	unsigned int base_phy;
	unsigned int byte_num;
	unsigned int rd_bit_result[DDR_BIT_MAX];
	unsigned int wr_bit_result[DDR_BIT_MAX];
	unsigned int rd_bit_best[DDR_BIT_MAX];
	unsigned int wr_bit_best[DDR_BIT_MAX];
	unsigned int wr_win_sum;
	unsigned int rd_win_sum;
};

struct ddr_training_result_st {
	struct ddr_training_data_st ddrtr_data[DDR_SUPPORT_PHY_MAX];
};

struct ddr_reg_val_st {
	char name[DDR_REG_NAME_MAX];
	unsigned int addr;
	unsigned int val;
};

struct ddr_training_reg_st {
	struct ddr_reg_val_st reg[DDR_TRAINING_REG_MAX];
};

struct ddr_cmd_st {
	unsigned int cmd;
	unsigned int level;
	unsigned int start;
	unsigned int length;
};

typedef struct ddr_training_result_st * (*ddr_cmd_entry_func)
	(struct ddr_cmd_st *cmd_st);

/* DDR training interface before boot */
int ddr_sw_training_if(void *ddrtr_result);

/* DDR training check interface when boot */
struct ddr_training_result_st *ddr_cmd_training_if(struct ddr_cmd_st *cmd_st);
int check_ddr_training(void);

/* DDR training command interface after boot */
void ddr_reg_result_display(struct ddr_training_reg_st *ddr_reg);
void ddr_cmd_result_display(struct ddr_training_result_st *ddrtr_result);
void *ddr_cmd_get_entry(void);
char *ddr_cmd_result_dump(struct ddr_training_reg_st *ddr_reg,
	char flags);
void ddr_cmd_prepare_copy(void);
void ddr_cmd_site_save(void);
void ddr_cmd_site_restore(void);
