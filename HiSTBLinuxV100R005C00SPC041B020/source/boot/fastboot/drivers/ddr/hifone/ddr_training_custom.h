/**
 * ddr_training_custom.h
 *
 * Copyright (c) 2009-2014, HiSilicon Technologies Co., Ltd.
 * All rights reserved.
 *
 * DDR training custom define.
 */

/* config DDRC, PHY, DDRT typte */
#define DDR_DDRC_V510_CONFIG
#define DDR_PHY_T28_CONFIG
#define DDR_DDRT_T28_CONFIG

/* config special item */
/* #define DDR_VREF_TRAINING_CONFIG */
/* #define DDR_MPR_TRAINING_CONFIG */
#define DDR_TRAINING_MINI_LOG_CONFIG
#define DDR_WL_TRAINING_DISABLE
#define DDR_GATE_TRAINING_DISABLE

/* config DDRC, PHY, DDRT base address */
/* [CUSTOM] DDR DMC base register */
#define DDR_REG_BASE_DMC0              0xf8a38000
#define DDR_REG_BASE_DMC1              0xf8a39000
/* [CUSTOM] DDR PHY base register */
#define DDR_REG_BASE_PHY0              0xf8a3c000
#define DDR_REG_BASE_PHY1              0xf8a3e000
/* [CUSTOM] DDR DDRT base register */
#define DDR_REG_BASE_DDRT              0xf8a50000
/* [CUSTOM] DDR training item system control */
#define DDR_REG_BASE_SYSCTRL           0xf8000000

/* config offset address */
/* Assume sysctrl offset address as follows, if not please define. */
#define SYSCTRL_DDRT_PATTERN 0x8C /* [CUSTOM] PHY0 ddrt reversed data */
#define SYSCTRL_DDRT_PATTERN_SEC 0x90 /* [CUSTOM] PHY1 ddrt reversed data */
#define SYSCTRL_DDR_TRAINING_CFG  0xd0   /* [CUSTOM] ddr training item */
#define SYSCTRL_DDR_TRAINING_STAT 0x9c   /* [CUSTOM] ddr training stat */

/* config other special */
/* [CUSTOM] The system DDR spacing start address. DDR_MEM_BASE */
#define DDRT_CFG_BASE_ADDR             0x0
/* [CUSTOM] SRAM start address. NOTE: Makefile will parse it. */
#define DDR_TRAINING_RUN_STACK         0xFFFF0C00
