/**
 * ddr_training_impl.h
 *
 * Copyright (c) 2009-2014, HiSilicon Technologies Co., Ltd.
 * All rights reserved.
 *
 * DDR training implement define.
 */

#ifndef __ASSEMBLY__

#include "ddr_training_custom.h"
#include "ddr_training_internal_config.h"

/*******DMC register*********************************************/
#ifndef DDR_REG_BASE_DMC0
/* [CUSTOM] DMC0 register base address */
#define DDR_REG_BASE_DMC0                 0xf8a38000
#endif
#ifndef DDR_REG_BASE_DMC1
/* [CUSTOM] DMC1 register base address */
#define DDR_REG_BASE_DMC1                 0xf8a39000
#endif
/*******PHY register*********************************************/
#ifndef DDR_PHY_NUM
/* [CUSTOM] the number of PHY */
#define DDR_PHY_NUM                       2
#endif
#ifndef DDR_REG_BASE_PHY0
/* [CUSTOM] PHY0 register base address */
#define DDR_REG_BASE_PHY0                 0xf8a3c000
#endif
#ifndef DDR_REG_BASE_PHY1
/* [CUSTOM] PHY1 register base address */
#define DDR_REG_BASE_PHY1                 0xf8a3e000
#endif
/*******DDRT register*********************************************/
#ifndef DDR_REG_BASE_DDRT
#define DDR_REG_BASE_DDRT                 0xf8a50000
#endif
/*******AXI register*********************************************/
#ifndef DDR_REG_BASE_AXI
#define DDR_REG_BASE_AXI                  0xf8a30000
#endif

/****** ddr training sysctrl control register ********************************/
#ifndef DDR_REG_BASE_SYSCTRL
/* [CUSTOM] ddr training control */
#define DDR_REG_BASE_SYSCTRL              0xf8000000
#endif
#ifndef SYSCTRL_DDRT_PATTERN
/* [CUSTOM] ddrt reversed data */
#define SYSCTRL_DDRT_PATTERN              0xa0
#endif
#ifndef SYSCTRL_DDRT_PATTERN_SEC
/* [CUSTOM] ddrt reversed data for second PHY. If not defined, keep same.*/
#define SYSCTRL_DDRT_PATTERN_SEC          (SYSCTRL_DDRT_PATTERN)
#endif
#ifndef SYSCTRL_DDR_TRAINING_CFG
/* [CUSTOM] ddr training item */
#define SYSCTRL_DDR_TRAINING_CFG          0xd0
#endif
#ifndef SYSCTRL_DDR_TRAINING_STAT
/* [CUSTOM] ddr training stat */
#define SYSCTRL_DDR_TRAINING_STAT         0x9c
#endif

/****** special config define*******************************************/
#ifdef DDR_DATAEYE_NORMAL_NOT_ADJ_CONFIG
/* Adjust dataeye window consume a lot of time, disable it will make boot
 * faster.
 * NOTE: The WDQ Phase and RDQS MUST be config a good value in the init table
 * to avoid window trend to one side.
 */
#define DDR_DATAEYE_NORMAL_ADJUST   (DDR_FALSE)
#else
#define DDR_DATAEYE_NORMAL_ADJUST   (DDR_TRUE)
#endif
/* MUST adjust dataeye window after HW or MPR training */
#define DDR_DATAEYE_ABNORMAL_ADJUST       (DDR_TRUE)

/****** ddr training item bypass mask define ****************************/
#define DDR_BYPASS_PHY0_MASK        0x1 /* [0]PHY0 training */
#define DDR_BYPASS_PHY1_MASK        0x2 /* [1]PHY1 training */
#define DDR_BYPASS_WL_MASK          0x10 /* [4]Write leveling */
#define DDR_BYPASS_GATE_MASK        0x100 /* [8]Gate training */
#define DDR_BYPASS_DATAEYE_MASK     0x10000 /* [16]Dataeye training */
#define DDR_BYPASS_HW_MASK          0x100000 /* [20]Hardware read training */
#define DDR_BYPASS_MPR_MASK         0x200000 /* [21]MPR training */
#define DDR_BYPASS_AC_MASK          0x400000 /* [22]AC training */
#define DDR_BYPASS_LPCA_MASK        0x800000 /* [23]LPDDR CA training */
#define DDR_BYPASS_VREF_HOST_MASK   0x1000000 /* [24]Host Vref training */
#define DDR_BYPASS_VREF_DRAM_MASK   0x2000000 /* [25]DRAM Vref training */
#define DDR_BYPASS_DATAEYE_ADJ_MASK 0x10000000 /* [28]Dataeye adjust */
#define DDR_BYPASS_WL_ADJ_MASK      0x20000000 /* [29]WL write adjust */
#define DDR_BYPASS_HW_ADJ_MASK      0x40000000 /* [30]HW read adjust */
#define DDR_BYPASS_ALL_MASK         0xffffffff /* all bypass */

/****** common define **********************************************/
/* special ddrt need special read and write register */
#ifdef DDR_DDRT_SPECIAL_CONFIG
#define DDRT_REG_READ(addr)        ddr_ddrt_read(addr)
#define DDRT_REG_WRITE(val, addr)  ddr_ddrt_write(val, addr)
#else
#define DDRT_REG_READ(addr)        (*(volatile unsigned int *)(addr))
#define DDRT_REG_WRITE(val, addr)  ((*(volatile unsigned *) (addr)) = (val))
#endif

#define DDR_MODE_READ                     (1 << 0)
#define DDR_MODE_WRITE                    (1 << 1)

/* DSB to make sure the operation is complete */
#ifndef DDR_ASM_DSB
#define DDR_ASM_DSB()                     { __asm__ __volatile__("dsb"); }
#endif

#define DDR_HWR_WAIT_TIMEOUT	          (1000)
#define DDR_SFC_WAIT_TIMEOUT	          (1000)
#define DDR_LPCA_WAIT_TIMEOUT	          (1000)

#ifdef CFG_EDA_VERIFY
#define DDR_AUTO_TIMING_DELAY	          (1)
#else
#define DDR_AUTO_TIMING_DELAY	          (1000)
#endif

#define DDR_FIND_DQ_BOTH                  (1 << 0) /* find a valid value*/
/* x is valid, (x-1) is invalid*/
#define DDR_FIND_DQ_LEFT                  (1 << 1)
/* x is valid, (x+1) is invalid*/
#define DDR_FIND_DQ_RIGHT                 (1 << 2)

#define DDR_VREF_HOST_VAL_MAX             (0x1f)        /* 78.75%*VDDIO */
#define DDR_VREF_HOST_VAL_MIN             (0x0)         /* 40.00%*VDDIO */

#define DDR_VREF_DRAM_VAL_MAX             (0x32)        /* 92.50%*VDDIO */
#define DDR_VREF_DRAM_VAL_MIN             (0x0)         /* 60.00%*VDDIO */

#define DDR_PHY_BYTE_MAX                  4
#define DDR_PHY_BIT_NUM                   8
/* support max bit 32*/
#define DDR_PHY_BIT_MAX                   (DDR_PHY_BYTE_MAX * DDR_PHY_BIT_NUM)
#define DDR_PHY_REG_DQ_NUM                4  /* one register has 4 DQ BDL */

#define DDR_PHY_CA_MAX                    10
#define DDR_PHY_CA_REG_MAX                (DDR_PHY_CA_MAX >> 1)

#define DDR_TRUE                          1
#define DDR_FALSE                         0

#define DDR_WIN_MIDDLE                    (1 << 0)
#define DDR_WIN_LEFT                      (1 << 1)
#define DDR_WIN_RIGHT                     (1 << 2)

#define DDR_DELAY_PHASE                   1
#define DDR_DELAY_BDL                     2

#ifndef DDR_DATAEYE_WIN_NUM
/* Dateeye window number. More bigger more slower when Vref training. */
#define DDR_DATAEYE_WIN_NUM               8
#endif
#ifndef DDR_LOOP_TIMES_LMT
/* Dataeye DQ deskew times for best result. More bigger more slower. */
#define DDR_LOOP_TIMES_LMT                1
#endif
#ifndef DDR_VREF_COMPARE_TIMES
/* Compare times when find best vref value. More bigger more slower. */
#define DDR_VREF_COMPARE_TIMES            3
#endif
#ifndef DDR_MPR_RDQS_FIND_TIMES
/* MPR Find first start rdqs times. More bigger, start rdqs more bigger. */
#define DDR_MPR_RDQS_FIND_TIMES           3
#endif
#ifndef DDR_VREF_COMPARE_STEP
/* Compare step when begin to find. More bigger, more mistake, more stable. */
#define DDR_VREF_COMPARE_STEP             3
#endif

#define DDR_DATAEYE_RESULT_MASK           0xffff
#define DDR_DATAEYE_RESULT_BIT            16

#define DDR_WL_BDL_STEP                   2 /* wl bdl step */
#define DDR_GATE_BDL_STEP                 2 /* gate bdl step */
#define DDR_DQS_ADJ_STEP                  1 /* WR/RD DQS adjust step */

#define DDR_DDRT_MODE_GATE                (1 << 0)
#define DDR_DDRT_MODE_DATAEYE             (1 << 1)

#define DDR_CHECK_TYPE_DDRT               (1 << 0)
#define DDR_CHECK_TYPE_MPR                (1 << 1)

#define DDR_MPR_BYTE_MASK                 0xff
#define DDR_MPR_BIT_MASK                  0x1
#define DDR_MPR_BYTE_BIT                  16          /* 16 bit (2 byte) */

#define DDR_PHY_AC_TEST_VAL0              0x0
#define DDR_PHY_AC_TEST_VAL1              0xffffffff
#define DDR_PHY_AC_TEST_VAL2              0x55555555
#define DDR_PHY_AC_TEST_VAL3              0xaaaaaaaa

/*******log define ***********************************************/
#if defined(DDR_TRAINING_CMD) && defined(DDR_TRAINING_LOG_CONFIG)
#define DDR_INFO(fmt...)     ddr_training_log(__func__, DDR_LOG_INFO, fmt)
#define DDR_DEBUG(fmt...)    ddr_training_log(__func__, DDR_LOG_DEBUG, fmt)
#define DDR_WARNING(fmt...)  ddr_training_log(__func__, DDR_LOG_WARNING, fmt)
#define DDR_ERROR(fmt...)    ddr_training_log(__func__, DDR_LOG_ERROR, fmt)
#define DDR_FATAL(fmt...)    ddr_training_log(__func__, DDR_LOG_FATAL, fmt)
#else
#define DDR_INFO(fmt...)
#define DDR_DEBUG(fmt...)
#define DDR_WARNING(fmt...)
#define DDR_ERROR(fmt...)
#define DDR_FATAL(fmt...)
#endif /* DDR_TRAINING_CMD && DDR_TRAINING_LOG_CONFIG */

/* [11:0] Error type */
/* 0x00000001 Write Leveling error */
#define DDR_ERR_WL                        (1 << 0)
/* 0x00000002 Hardware Gatining error */
#define DDR_ERR_HW_GATING                 (1 << 1)
/* 0x00000004 Sofeware Gatining error */
#define DDR_ERR_GATING                    (1 << 2)
/* 0x00000008 DDRT test time out */
#define DDR_ERR_DDRT_TIME_OUT             (1 << 3)
/* 0x00000010 Hardware read dataeye error */
#define DDR_ERR_HW_RD_DATAEYE			  (1 << 4)
/* 0x00000020 MPR error */
#define DDR_ERR_MPR                       (1 << 5)
/* 0x00000040 Dataeye error */
#define DDR_ERR_DATAEYE                   (1 << 6)
/* 0x00000080 LPDDR CA error */
#define DDR_ERR_LPCA                      (1 << 7)

/* [13:12] Error phy */
/* 0x00001000 PHY0 training error */
#define DDR_ERR_PHY0                      (1 << 12)
/* 0x00002000 PHY1 training error */
#define DDR_ERR_PHY1                      (1 << 13)

#define DDR_ERR_BYTE_BIT                  24 /* [28:24] Error DQ0-31 */
#define DDR_ERR_DQ_BIT                    20 /* [22:20] Error Byte0-3 */

/*******data define*********************************************/
#ifndef DDR_RELATE_REG_DECLARE
struct tr_custom_reg {
};
#endif

struct training_data {
	unsigned short int ddr_byte_num;
	unsigned int base_phy;
	unsigned int base_dmc;
	unsigned int ddrt_reversed_data;
	unsigned int dq_check_type;
	unsigned int ddr_bit_result[DDR_PHY_BIT_MAX];
	unsigned int ddr_bit_best[DDR_PHY_BIT_MAX];
	unsigned int ddr_win_sum;
};

struct ddr_delay_st {
	unsigned int phase[DDR_PHY_BYTE_MAX];
	unsigned int bdl[DDR_PHY_BYTE_MAX];
};

struct tr_relate_reg {
	unsigned int auto_ref_timing[DDR_PHY_NUM];
	unsigned int power_down[DDR_PHY_NUM];
	unsigned int dmc_scramb[DDR_PHY_NUM];
	unsigned int dmc_scramb_cfg[DDR_PHY_NUM];
	unsigned int misc_scramb[DDR_PHY_NUM];
	unsigned int ac_phy_ctl[DDR_PHY_NUM];
	unsigned int swapdfibyte_en[DDR_PHY_NUM];
	struct tr_custom_reg custom;
	struct ddr_ddrc_data ddrc;
};

struct tr_dq_data {
	unsigned int dq03[DDR_PHY_BYTE_MAX]; /* DQ0-DQ3 BDL */
	unsigned int dq47[DDR_PHY_BYTE_MAX]; /* DQ4-DQ7 BDL */
	unsigned int wdm[DDR_PHY_BYTE_MAX];  /* WDM */
};

struct ca_bit_st {
	unsigned int bit_p;
	unsigned int bit_n;
};

struct ca_data_st {
	unsigned int base_dmc;
	unsigned int base_phy;
	unsigned int done; /* whether all ca found bdl range */
	unsigned int min; /* min left bound */
	unsigned int max; /* max right bound */
	unsigned def[DDR_PHY_CA_REG_MAX];
	int left[DDR_PHY_CA_MAX];
	int right[DDR_PHY_CA_MAX];
	struct ca_bit_st bits[DDR_PHY_CA_MAX];
};

/*******function interface define*********************************************/
#ifndef DDR_SW_TRAINING_FUNC
#define DDR_SW_TRAINING_FUNC_PUBLIC
#define DDR_SW_TRAINING_FUNC        ddr_sw_training_func
#endif
/*******Custom function ***********************************************/
#ifndef DDR_TRAINING_DDRT_PREPARE_FUNC
#define DDR_TRAINING_DDRT_PREPARE_FUNC()
#endif
#ifndef DDR_TRAINING_SAVE_REG_FUNC
#define DDR_TRAINING_SAVE_REG_FUNC(relate_reg, mask)
#endif
#ifndef DDR_TRAINING_RESTORE_REG_FUNC
#define DDR_TRAINING_RESTORE_REG_FUNC(relate_reg)
#endif
/*******function define*********************************************/
int ddr_sw_training_func(void *ddrtr_result);

void ddr_phy_cfg_update(unsigned int base_phy);
void ddr_phy_set_dq_bdl(unsigned int base_phy, unsigned int deskew_value,
	unsigned int bytenum, unsigned int dqindex, unsigned int mode);
int ddr_hw_training(unsigned int base_dmc, unsigned int base_phy);
int ddr_mpr_check(unsigned int base_dmc, unsigned int byte_index,
	int dq_index);
int ddr_mpr_training(unsigned int base_dmc, unsigned int base_phy);
int ddr_write_leveling(unsigned int base_dmc, unsigned int base_phy);
int ddr_gate_training(unsigned int base_dmc, unsigned int base_phy);
int ddr_dataeye_training(unsigned int base_dmc, unsigned int base_phy,
	void *ddrtr_result, unsigned int adjust);
int ddr_vref_training(unsigned int base_dmc, unsigned int base_phy,
	void *ddrtr_result, unsigned int mode);
int ddr_ac_training(unsigned int base_dmc, unsigned int base_phy);
int ddr_lpca_training(unsigned int base_dmc, unsigned int base_phy,
	void *ddrtr_result);
int ddr_dataeye_deskew(struct training_data *training,
	unsigned int byte_index, unsigned int mode);
void ddr_adjust_dataeye(struct training_data *training,
	unsigned int mode);
void ddr_result_data_save(void *ddrtr_result,
	struct training_data *training, unsigned int mode);
void ddr_lpca_data_save(void *ddrtr_result, struct ca_data_st *data);
int ddr_ddrt_get_test_addr(void);
int ddr_ddrt_test(unsigned int mask, int byte, int dq);
int ddr_dataeye_check_dq(const struct training_data *training,
	unsigned int byte_index, unsigned int dq_index);
void ddr_ddrt_init(unsigned int base_dmc, unsigned int mode);
int ddr_training_check_bypass(unsigned int mask);
int ddr_training_phy_disable(int index);
void ddr_training_save_reg(struct tr_relate_reg *relate_reg,
	unsigned int mask);
void ddr_training_restore_reg(struct tr_relate_reg *relate_reg);
void ddr_training_get_base(int index, unsigned int *base_dmc,
	unsigned int *base_phy);
void ddr_training_switch_axi(int index, struct tr_relate_reg *relate_reg);
unsigned int ddr_dmc_get_reversed(unsigned int base_dmc);
void ddr_training_log(const char *func, int level, const char *fmt, ...);
void ddr_training_stat(unsigned int mask, unsigned int phy, int byte, int dq);
void ddr_training_error(unsigned int mask, unsigned int phy, int byte, int dq);
void ddr_training_start(void);
void ddr_training_suc(void);
unsigned int ddr_phy_get_byte_num(unsigned int base_dmc);
void ddr_training_set_timing(unsigned int base_dmc, unsigned int timing);
#endif /* __ASSEMBLY__ */
