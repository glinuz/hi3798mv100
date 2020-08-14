/**
 * ddr_phy_s40.h
 *
 * Copyright (c) 2009-2014, HiSilicon Technologies Co., Ltd.
 * All rights reserved.
 *
 * DDR PHY register offset address, mask, bit.
 */

/* register offset address */
/* base address: DDR_REG_BASE_PHY0 DDR_REG_BASE_PHY1 */
/* control the initialization of the PHY */
#define DDR_PHY_PHYINITCTRL	0x4
#define DDR_PHY_PHYINITSTATUS	0x8	/* Read Data Eye Calibration Error*/
#define DDR_PHY_DRAMCFG		0x2c    /* DRAM config register */
#define DDR_PHY_MODEREG01	0x64	/* Extend Mode Register 01 */
#define DDR_PHY_MODEREG23	0x68	/* Extend Mode Register 23 */
/* update delay setting in registers to PHY */
#define DDR_PHY_MISC		0x70
#define DDR_PHY_SWTMODE		0xa0	/* S/W training mode */
/* issue one DQS pulse from PHY to DRAM */
#define DDR_PHY_SWTWLDQS	0xa4
#define DDR_PHY_SWTRLT		0xa8	/* S/W training result*/
/* Host vref. [5:0]range [17:12]refsel */
#define DDR_PHY_IOCTL2		0xB4
/* AC command bit delay line setting */
#define DDR_PHY_ACCMDBDL2		0x128
#define DDR_PHY_ACPHYCTL4	0x180  /* AC block PHY control register*/
/* AC block PHY control register */
#define DDR_PHY_ACPHYCTL7		0x18C
/* WR DQ0-DQ3 [4:0] [12:8] [20:16] [28:24] delay value of the bit delay line
   on write path */
#define DDR_PHY_DXNWDQNBDL0(n)	(0x210 + ((n) << 7))
/* WR DQ4-DQ7 [4:0] [12:8] [20:16] [28:24] */
#define DDR_PHY_DXNWDQNBDL1(n)	(0x214 + ((n) << 7))
/* WR DM [6:0] the delay value of the bit delay line on DQM */
#define DDR_PHY_DXNWDQNBDL2(n)	(0x218 + ((n) << 7))
 /* RD DQ0-DQ3 [4:0] [12:8] [20:16] [28:24] delay value of the bit delay line
 on read path */
#define DDR_PHY_DXNRDQNBDL0(n)	(0x21C + ((n) << 7))
/* RD DQ4-DQ7 [4:0] [12:8] [20:16] [28:24] delay value of the bit delay line
on read path */
#define DDR_PHY_DXNRDQNBDL1(n)	(0x220 + ((n) << 7))
/* [6:0] RD DM*/
#define DDR_PHY_DXNRDQNBDL2(n)	(0x224 + ((n) << 7))
/* [CUSTOM] */
#define DDR_PHY_DXNOEBDL(n)	(0x258 + ((n) << 7))
/* [8:0] rdqs_bdl [24:16]rdqs_cyc.
phase shift of the Read DQS to create 90 degree delays*/
#define DDR_PHY_DXNRDQSDLY(n)	(0x22C + ((n) << 7))
/* [6:0] the delay value of delay applied on WDQS for write leveling */
#define DDR_PHY_DXWDQSDLY(n)	(0x230 + ((n) << 7))
/* WR DQ phase BIT 12:8 */
#define DDR_PHY_DXNWDQDLY(n)	(0x234 + ((n) << 7))
/* [CUSTOM] rddqs gating*/
#define DDR_PHY_DXNRDQSGDLY(n)	(0x23c + ((n) << 7))
/* read boundary  right 8:0 left 24:16 */
#define DDR_PHY_DXNRDBOUND(n)	(0x250 + ((n) << 7))
/* write boundary  right 4:0 left 20:16 */
#define DDR_PHY_DXNWDBOUND(n)	(0x254 + ((n) << 7))

/* register mask */
#define PHY_BDL_MASK			0x1f	/* [4:0] */
#define PHY_WDQS_PHASE_MASK		0xf	/* [11:8] */
#define PHY_RDQS_BDL_MASK		0x7f	/* [CUSTOM] [6:0] rdqsbdl*/
#define PHY_RDQSG_PHASE_MASK		0x3f	/* [14:9] rdqsgtxbdl */
/* hardware gate training result */
#define PHY_INITSTATUS_GT_MASK		0x20
#define PHY_SWTRLT_WL_MASK		0xf
#define PHY_SWTRLT_GATE_MASK	0xf
#define PHY_WDQ_PHASE_MASK		0x1f
#define PHY_PHYINITCTRL_MASK		0xffff	/* [15:0] all stat */
/* Read Data Eye Calibration Error */
#define PHY_PHYINITSTATUS_RDET_ERR	0x100
#define PHY_ACPHY_DCLK_MASK		0x7	/* cp1p_dclk0 mask */
#define PHY_ACPHY_DRAMCLK_MASK		0x1	/* halft_dramclk0 mask */

/* register bit */
#define PHY_MISC_UPDATE_BIT	20	/* [CUSTOM] delay config update bit */
#define PHY_PHYCONN_RST_BIT	15	/* issue reset signal to PHY counter */
#define PHY_RDQSG_PHASE_BIT	8	/* [CUSTOM] */
#define PHY_RDQSG_TX_BDL_BIT	16	/* [22:16] rdqsgtxbdl */
#define PHY_WDQS_PHASE_BIT	8
#define PHY_WDQ_PHASE_BIT	8
/* [20:16] Write DQS Output Enable Delay Control */
#define PHY_WDQSOE_BDL_BIT	16
/* Mode Register 1. Defines the MR3/MR9 of the mode register */
#define PHY_MODEREG01_MR1_BIT	16
/* Bit delay line setting of CS1 */
#define PHY_ACCMD_CS1_BIT		16
#define PHY_ACPHY_DCLK0_BIT		6	/* [8:6] cp1p_dclk0 */
#define PHY_ACPHY_DCLK1_BIT		9	/* [11:9] ck2p_dclk1 */
#define PHY_ACPHY_DRAMCLK0_BIT		25	/* [25] halft_dramclk0 */
#define PHY_ACPHY_DRAMCLK1_BIT		24	/* [24] halft_dramclk1 */
#define PHY_ACPHY_DRAMCLK_EXT_BIT	3 /* [3] halft_dramclk0 */
#define PHY_SWTMODE_SW_GTMODE_BIT	1 /* [1] SW gate training */

/* value */
/* Read Data Eye Training Enable. */
#define PHY_PHYINITCTRL_RDET_EN		0x100
#define PHY_PHYINITCTRL_DLYMEAS_EN	0x4	/* Delay Measurement Enable */
/* PHY Initialization Enable. */
#define PHY_PHYINITCTRL_INIT_EN		0x1
/* RDQS range[0, 0x1f],  middle value is 0x10 */
#define PHY_RDQS_MIDDLE_VAL		0x10
/* DQ range[0, 0x1f],  middle value is 0x10 */
#define PHY_DQ_MIDDLE_VAL		0x10101010
#define PHY_MISC_SCRAMB_DIS		0xfffeffff	/* scrambler disable */
/* NOTE: rdqsg_bdl is [5:0] in register manual, actual use [4:0] */
#define PHY_GATE_BDL_MAX        0x40 /* [4:0]rdqsg_bdl + [20:16]rdqsgtxbdl */
/* [5] two cycle on address or command.(2T timing) */
#define PHY_DRAMCFG_MA2T		0x20
#define PHY_DRAMCFG_TYPE_LPDDR3 0x5 /* [2:0] DRAM Type LPDDR3 */

/* other */
#define PHY_RDQSG_PHASE_STEP	4 /* gate training phase step. */
#define PHY_GATE_PHASE_MARGIN	8 /* gate phase margin */
#define PHY_DQ_BDL_LEVEL	32 /* [CUSTOM] DQ BDL range */
#define PHY_DQ_BDL_MIDDLE	15 /* middle DQ BDL value */
#define PHY_RDQSG_PHASE_MAX	0x3c /* RDQSG phase max value */
#define PHY_ACPHY_CLK_MAX	0xf /* halft_dramclk0 + cp1p_dclk0 */
/**
 * DDR_BDL_PHASE_REL Calculation Method:
 * 1. Calculation How many picosecond to one phase.
 *    PICOSECOND : 1 second is (1000 * 1000 * 1000) picosecond
 *    WAVE       : 1 cycle is 2
 *    RATE       : DDR rate is 1600 Mbps, is (1600 * 1000) bps
 *    PHASE      : 1 wave is 16 phase
 *    phase = (((PICOSECOND * WAVE) / RATE) / PHASE)
 *          = (((1000 * 1000 * 1000 * 2) / (1600 * 1000)) / 16)
 *          = 78.125 ps.
 * 2. Calculation How many bdl to one phase.
 *    one BDL is 20 ps.
 *    result = phase/bdl = 78.125 / 20 = 3.9 approximately equal to 4
 * 3. 4 = 1 << 2, so the relation is 2.
 */
#ifndef DDR_BDL_PHASE_TRANSFORM
/* [CUSTOM] one Phase equal how much BDL. 1 phase = 4 bdl */
#define DDR_BDL_PHASE_TRANSFORM		4
#endif
#ifndef DDR_BDL_PHASE_REL
/* [CUSTOM] relation between BDL and Phase. 1 phase = 4 bdl, 4 = 1 << 2 */
#define DDR_BDL_PHASE_REL		2
#endif

#define DDR_VARIABLE_DECLARE(var)

#define DDR_PHY_VREF_HOST_SET(base_phy, val, bytenum) \
	REG_WRITE(((val & 0x7) << 12) | ((val >> 3) & 0x3), \
		base_phy + DDR_PHY_IOCTL2)

#define DDR_PHY_VREF_HOST_GET(base_phy, val) \
	do { \
		unsigned int ref_range; \
		unsigned int ref_sel; \
		val = REG_READ(base_phy + DDR_PHY_IOCTL2); \
		ref_range = val & 0x3; \
		ref_sel   = (val >> 12) & 0x7; \
		val = (ref_range << 3) | ref_sel; \
	} while (0)

#define DDR_PHY_VREF_HOST_DISPLAY(base_phy, ddr_reg, index, byte_num) \
	do { \
		snprintf(ddr_reg->reg[index].name, DDR_REG_NAME_MAX, \
			"Host Vref"); \
		ddr_reg->reg[index++].addr = base_phy + DDR_PHY_IOCTL2; \
	} while (0)

#define DDR_PHY_VREF_HOST_DISPLAY_CMD(base_phy, byte_num) \
	DDR_INFO("[%x = %x] Host Vref", \
		base_phy + DDR_PHY_IOCTL2, \
		REG_READ(base_phy + DDR_PHY_IOCTL2));

/* phy s40 not support DRAM vref */
#define DDR_PHY_VREF_DRAM_SET(base_phy, val, byte_index)
#define DDR_PHY_VREF_DRAM_GET(base_phy, val, byte_index)
#define DDR_PHY_VREF_DRAM_DISPLAY(base_phy, ddr_reg, index, byte_num)
#define DDR_PHY_VREF_DRAM_DISPLAY_CMD(base_phy, byte_num)

/* phy s40 not support Lowpower ddr ca */
#define DDR_PHY_ADDRPH_DISPLAY(base_phy, ddr_reg, index)
#define DDR_PHY_ADDRBDL_DISPLAY(base_phy, ddr_reg, index)
#define DDR_PHY_ADDRPH_DISPLAY_CMD(base_phy)
#define DDR_PHY_ADDRBDL_DISPLAY_CMD(base_phy, byte_num)

/* phy s40 not support DDR4 */
#define DDR_PHY_RDQS_SYNC_RDM(base_dmc, base_phy, delay, byte_index)

/* phy s40 not support dqs swap */
#define DDR_DQSSWAP_SAVE_FUNC(swapdfibyte_en, base_phy)
#define DDR_DQSSWAP_RESTORE_FUNC(swapdfibyte_en, base_phy)
