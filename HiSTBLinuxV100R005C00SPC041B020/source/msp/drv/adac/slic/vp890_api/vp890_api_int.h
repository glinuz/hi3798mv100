/** \file vp890_api_int.h
 * vp890_api_int.h
 *
 * Header file for the vp890 series API-II c files.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7545 $
 * $LastChangedDate: 2010-07-23 15:58:04 -0500 (Fri, 23 Jul 2010) $
 */

#ifndef VP890_API_INT_H
#define VP890_API_INT_H

#include "vp_api.h"

/**< No Operation 06h */
#define VP890_NO_OP_WRT     0x06

/* This is to be backward compatible with VVA P1.1.0 */
#define VP890_NO_OP         VP890_NO_OP_WRT

/**< Read Revision and Product Code Number (RCN, PCN)) 73h */
#define VP890_DEVTYPE_RD            0x73
#define VP890_DEVTYPE_LEN           0x02    /**< RCN = 1st Byte, PCN = 2nd Byte */
#define VP890_RCN_PCN_RD            0x73
#define VP890_RCN_PCN_LEN           0x02    /**< RCN = 1st Byte, PCN = 2nd Byte */
#define VP890_RCN_LOCATION          0x00
#define VP890_PCN_LOCATION          0x01

/**<
 *  GLOBAL REGISTERS (Effects all SLAC device channels)
 */

/**< Hardware Reset 04h */
#define VP890_HW_RESET_WRT          0x04
#define VP890_HW_RESET_LEN          0x00

#define VP890_TEST_REG1_WRT     0x30
#define VP890_TEST_REG1_RD      0x31
#define VP890_TEST_REG1_LEN     0x01

#define VP890_TEST_REG1_FUSE_TEST   0x03

#define VP890_FUSE_CTRL_REG_WRT     0x34
#define VP890_FUSE_CTRL_REG_RD      0x35
#define VP890_FUSE_CTRL_REG_LEN     0x01

#define VP890_FUSE1_REG_WRT         0x36
#define VP890_FUSE1_REG_RD          0x37
#define VP890_FUSE1_REG_LEN         0x01

#define VP890_FUSE1_REG_ILAF_TRIM   0x40

#define VP890_FUSE5_REG_WRT         0x3E
#define VP890_FUSE5_REG_RD          0x3F
#define VP890_FUSE5_REG_LEN         0x01

#define VP890_FUSE6_REG_WRT         0xF0
#define VP890_FUSE6_REG_RD          0xF1
#define VP890_FUSE6_REG_LEN         0x01

#define VP890_FUSE7_REG_WRT         0xF8
#define VP890_FUSE7_REG_RD          0xF9
#define VP890_FUSE7_REG_LEN         0x01

/**< Transmit/Receive Clock Slot Command info 44/45h */
#define VP890_TX_RX_CSLOT_WRT       0x44    /**< Tx/Rx clock slot register write */
#define VP890_TX_RX_CSLOT_RD        0x45    /**< Tx/Rx clock slot register read */
#define VP890_TX_RX_CSLOT_LEN       0x01

#define VP890_TX_CSLOT_WRT          0x44    /**< Tx clock slot register write */
#define VP890_TX_CSLOT_RD           0x45    /**< Tx clock slot register read */
#define VP890_TX_CSLOT_LEN          0x01

#define VP890_RX_CSLOT_WRT          0x44    /**< Rx clock slot register write */
#define VP890_RX_CSLOT_RD           0x45    /**< Rx clock slot register read */
#define VP890_RX_CSLOT_LEN          0x01

#define VP890_TX_SLOT_MASK          0x07
#define VP890_RX_SLOT_MASK          0x38

/**< Transmit Edge Command info */
#define VP890_TX_EDGE_WRT           0x44    /**< Tx clock slot register write */
#define VP890_TX_EDGE_RD            0x45    /**< Tx clock slot register read */
#define VP890_TX_EDGE_LEN           0x01

#define VP890_TX_EDGE_MASK          0x40


/**< Device Configuration Command info 46/47h */
#define VP890_DCR_WRT               0x46
#define VP890_DCR_RD                0x47
#define VP890_DCR_LEN               0x01

#define VP890_DCR_INTMODE_MASK      0x80

/**< Master Clock Command info */
#define VP890_MCLK_CNT_WRT          0x46
#define VP890_MCLK_CNT_RD           0x47
#define VP890_MCLK_CNT_LEN          0x01

#define VP890_MCLK_CLKSEL_MASK      0x0F


/**< Operating Mode Command info 4A/4Bh */
#define VP890_OP_MODE_WRT           0x4A
#define VP890_OP_MODE_RD            0x4B
#define VP890_OP_MODE_LEN           0x01

#define VP890_TEST_MODE_MASK        0x80
#define VP890_WBAND_MODE_MASK       0x20
#define VP890_DSPBYP_MODE_MASK      0x10

#define VP890_TEST_MODE_ENABLE      0x80
#define VP890_WIDEBAND_MODE         0x20
#define VP890_DSP_BYPASS            0x10

/**< Channel Enable Command info */
#define VP890_EC_WRT                0x4A
#define VP890_EC_RD                 0x4B
#define VP890_EC_LEN                0x01

#define VP890_EC_STATE_AT_RESET     0x03
#define VP890_EC_BITS_MASK          0x03
#ifndef VP890_EC_CH1
#define VP890_EC_CH1                0x01
#define VP890_EC_CH2                0x02
#endif

#define VP890_EC_TEST_MODE_EN       0x80

/**< Signaling Register Command info 4D/4Fh */
#define VP890_NO_UL_SIGREG_RD       0x4D    /**< Read w/o unlock signaling reg */
#define VP890_NO_UL_SIGREG_LEN      0x02

#define VP890_UL_SIGREG_RD          0x4F    /**< Read w/unlock signaling reg */

#ifndef VP890_UL_SIGREG_LEN
#define VP890_UL_SIGREG_LEN         0x02
#endif

/**< Interrupt Mask Register info 6C/6Dh */
#define VP890_INT_MASK_WRT          0x6C
#define VP890_INT_MASK_RD           0x6D
#define VP890_INT_MASK_LEN          0x02

/**< Bit definitions for Signaling and Interrupt Mask Register Commands */
    /**< Byte 1 */
#define VP890_CFAIL_MASK            0x80
#define VP890_OCALMY_MASK           0x40
#define VP890_TEMPA_MASK            0x20
#define VP890_IO2_MASK              0x10
#define VP890_CAD1_INT_MASK         0x08
#define VP890_CID_RDY_MASK          0x04
#define VP890_GNK_MASK              0x02
#define VP890_HOOK_MASK             0x01
    /**< Byte 2 */
#define VP890_DAT_MASK              0x80
#define VP890_POL_MASK              0x40
#define VP890_VISTAT_MASK           0x20  /* MDAT, typo in table for interrupts? */
#define VP890_OVIR_MASK             0x10
#define VP890_CAD2_INT_MASK         0x08
#define VP890_LDN_MASK              0x04
#define VP890_RING_DET_MASK         0x02
#define VP890_POH_DET_MASK          0x02
#define VP890_LIU_MASK              0x01
#define VP890_POH_SIGN_MASK         0x01


/**< Write/Read Input/Output Data Register 52/53h */
#define VP890_IODATA_REG_WRT        0x52    /**< I/O register write */
#define VP890_IODATA_REG_RD         0x53    /**< I/O register read */
#define VP890_IODATA_REG_LEN        0x01

#define VP890_IODATA_IOMASK         0x0F
#define VP890_IODATA_IO4            0x08
#define VP890_IODATA_IO3            0x04
#define VP890_IODATA_IO2            0x02
#define VP890_IODATA_IO1            0x01


/**< Write/Read Input/Output Direction Register 54/55h */
#define VP890_IODIR_REG_WRT         0x54    /**< I/O direction register write */
#define VP890_IODIR_REG_RD          0x55    /**< I/O direction register read */
#define VP890_IODIR_REG_LEN         0x01

#define VP890_IODIR_IO11_MASK       0x01
#define VP890_IODIR_IO12_MASK       0x02
#define VP890_IODIR_IO1_MASK        0x03
#define VP890_IODIR_IO2_MASK        0x04
#define VP890_IODIR_IO3_MASK        0x08
#define VP890_IODIR_IO4_MASK        0x10

#define VP890_IODIR_IO1_INPUT       0x00
#define VP890_IODIR_IO1_OUTPUT      0x01
#define VP890_IODIR_IO1_OPEN_DRAIN  0x02

#define VP890_IODIR_IO2_OUTPUT      0x04
#define VP890_IODIR_IO3_OUTPUT      0x08
#define VP890_IODIR_IO4_OUTPUT      0x10


/**< Write/Read Device Mode Register 5E/5Fh */
#define VP890_DEV_MODE_WRT          0x5E
#define VP890_DEV_MODE_RD           0x5F

#ifndef VP890_DEV_MODE_LEN
#define VP890_DEV_MODE_LEN          0x01
#endif

#define VP890_DEV_MODE_TEST_DATA    0x01

#define VP890_DEV_MODE_CTTI         0x80  /**< Control Tick Timer Interrupt */
#define VP890_DEV_MODE_CTTR         0x40  /**< Control Tick Timer Run */
#define VP890_DEV_MODE_CTS          0x20  /**< Timer Channel Select */
#define VP890_DEV_MODE_CH21PT       0x08  /**< Ch 1->2 Pass Through */
#define VP890_DEV_MODE_CH12PT       0x04  /**< Ch 2->1 Pass Through */
#define VP890_DEV_MODE_CBS          0x02  /**< Channel Buffer Select */
#define VP890_DEV_MODE_CBS_CH0      0x00
#define VP890_DEV_MODE_CBS_CH1      0x02
#define VP890_DEV_MODE_TDIM         0x01  /**< Test Data Interrupt Mode */

#define VP890_DEV_MODE_CHAN0_SEL    0x00
#define VP890_DEV_MODE_CHAN1_SEL    0x02


/**< Read Internal Revision Information B9h */
#define VP890_REV_INFO_RD           0xB9
#define VP890_REV_INFO_LEN          0x01


/**< Read Test Data Buffer CFh */
#define VP890_TEST_DATA_RD          0xCF
#ifndef VP890_TEST_DATA_LEN
#define VP890_TEST_DATA_LEN         0x0E
#endif
    /**< Byte 1 */
#define VP890_TEST_DATA_LEN_MASK    0x70  /**< LEN[2:0] */
#define VP890_TEST_DATA_LEN_INDEX   0
#define VP890_TEST_DATA_RNG25       0x08
#define VP890_TEST_DATA_RNG24       0x04
#define VP890_TEST_DATA_RNG23       0x02
#define VP890_TEST_DATA_RNG22       0x01
    /**< Byte 2 */
#define VP890_TEST_DATA_RNG21       0x80
#define VP890_TEST_DATA_RNG20       0x40
#define VP890_TEST_DATA_HK15        0x20
#define VP890_TEST_DATA_HK14        0x10
#define VP890_TEST_DATA_HK13        0x08
#define VP890_TEST_DATA_HK12        0x04
#define VP890_TEST_DATA_HK11        0x02
#define VP890_TEST_DATA_HK10        0x01

#define VP890_TEST_DATA_RNG_MSB_MASK    0x0F
#define VP890_TEST_DATA_RNG_MSK_INDEX   0
#define VP890_TEST_DATA_RNG_LSB_MASK    0xC0
#define VP890_TEST_DATA_RNG_LSB_INDEX   1
#define VP890_TEST_DATA_HK_MASK         0x3F
#define VP890_TEST_DATA_HK_INDEX        1

/**< Write/Read Switching Regulator Parameters E4/E5h */
#define VP890_REGULATOR_PARAM_WRT   0xE4
#define VP890_REGULATOR_PARAM_RD    0xE5
#ifndef VP890_REGULATOR_PARAM_LEN
#define VP890_REGULATOR_PARAM_LEN   0x03
#endif
#define VP890_REGULATOR_TRACK_INDEX     0x00
#define VP890_REGULATOR_FIXED_RING      0x02
#define VP890_REGULATOR_INVERT_BOOST    0x20

#define VP890_RING_MODE_BYTE        0x00
#define VP890_RING_MODE_FIXED       0x02

#define VP890_FLOOR_VOLTAGE_BYTE    0x01
#define VP890_FLOOR_VOLTAGE_MASK    0x1F

#define VP890_BAT_CALIBRATION_WRT   0xE8
#define VP890_BAT_CALIBRATION_RD    0xE9
#define VP890_BAT_CALIBRATION_LEN   0x02
#define VP890_BAT_CAL_SWCAL_MASK    0x38
#define VP890_BAT_CAL_SWCAL_SIGN    0x20

#define VP890_CONVERTER_MODE_WRT    0xE4
#define VP890_CONVERTER_MODE_RD     0xE5
#define VP890_CONVERTER_MODE_LEN    0x03

/**< Switching Regulator Parameters bit definitions */
#define VP890_FLY_BB_MODE_MASK      0x20
#define VP890_FLYBACK_MODE          0x00
#define VP890_BUCKBOOST_MODE        0x20

#define VP890_YRING_TRACK_MASK      0x02
#define VP890_YRING_TRACK_EN        0x00
#define VP890_YRING_TRACK_DIS       0x02

#define VP890_ZRING_TRACK_MASK      0x80
#define VP890_ZRING_TRACK_EN        0x00
#define VP890_ZRING_TRACK_DIS       0x80

#define VP890_SWY_AUTOPOWER_INDEX   0x01
#define VP890_SWY_AUTOPOWER_MASK    0x80
#define VP890_SWY_AUTOPOWER_EN      0x00
#define VP890_SWY_AUTOPOWER_DIS     0x80

#define VP890_SWFS_MASK             0x60
#define VP890_SWFS_FLYBACK_MODE     0x00
#define VP890_SWFS_BUCKBOOST_MODE   0x40

#define VP890_SWREG_TRACKING_BYTE   0
#define VP890_SWREG_TRACKING_MASK   0x02

#define VP890_SWREG_FLOOR_V_BYTE    1
#define VP890_SWREG_FLOOR_V_MASK    0x1F

#define VP890_SWREG_RING_V_BYTE     2
#define VP890_SWREG_RING_V_MASK     0x1F

#define VP890_VOLTAGE_MASK          0x1F
#define VP890_SWY_LOCATION          1
#define VP890_SWZ_LOCATION          2

#define VP890_VOLTAGE_60V           0x0B


/**< Internal Switching Regulator Parameters Command info */
#define VP890_INT_SWREG_PARAM_WRT   0xF6
#define VP890_INT_SWREG_PARAM_RD    0xF7

#ifndef VP890_INT_SWREG_PARAM_LEN
#define VP890_INT_SWREG_PARAM_LEN   0x06
#endif

#define VP890_SWREG_RING_V_BYTE     2
#define VP890_SWREG_FLOOR_V_BYTE    1


/**< Write/Read Switching Regulator Control E6/E7h */
#define VP890_REGULATOR_CTRL_WRT    0xE6
#define VP890_REGULATOR_CTRL_RD     0xE7
#define VP890_REGULATOR_CTRL_LEN    0x01

#define VP890_SWOVP_MASK            0x10
#define VP890_SWOVP_EN              0x10
#define VP890_SWOVP_DIS             0x00

#define VP890_SWY_MODE_MASK         0x03
#define VP890_SWY_OFF               0x00
#define VP890_SWY_LP                0x01
#define VP890_SWY_MP                0x02
#define VP890_SWY_HP                0x03

#define VP890_PWR_SWITCH_DEBOUNCE_FUT   (90)
#define VP890_PWR_SWITCH_DEBOUNCE_FXT   (130)

/**< Write/Read Switching Regulator Timing Parameters F6/F7h */
#define VP890_REGULATOR_TIMING_WRT   0xF6
#define VP890_REGULATOR_TIMING_RD    0xF7
#define VP890_REGULATOR_TIMING_LEN   0x06

/**<
 *  CHANNEL REGISTERS (must set EC register first)
 */

/**< Software Reset 02h */
#define VP890_SW_RESET_WRT          0x02    /**< Software reset */
#define VP890_SW_RESET_LEN          0x00


/**< Read Period Detector 1Fh */
#define VP890_PERIOD_DET_RD         0x1F
#define VP890_PERIOD_DET_LEN        0x01    /**< Time between successive pulses */

#define VP890_PULSE_PERIOD_DIVISOR      100ul
#define VP890_PULSE_PERIOD_STEPSIZE     25ul    /**< 0.25mS/step */


/**< Write/Read Transmit Time Slot 40/41h */
#define VP890_TX_TS_WRT             0x40
#define VP890_TX_TS_RD              0x41
#define VP890_TX_TS_LEN             0x01
#define VP890_TX_TS_MASK            0x7F
#define VP890_TX_TS_MAX             127


/**< Write/Read Receive Time Slot 42/43h */
#define VP890_RX_TS_WRT             0x42
#define VP890_RX_TS_RD              0x43
#define VP890_RX_TS_LEN             0x01
#define VP890_RX_TS_MASK            0x7F
#define VP890_RX_TS_MAX             127


/**< Write/Read Channel 1 Voice Path Gains 50/51h */
#define VP890_VP_GAIN_WRT           0x50
#define VP890_VP_GAIN_RD            0x51
#define VP890_VP_GAIN_LEN           0x01

#define VP890_AX_MASK               0x40
#define VP890_AR_MASK               0x30
#define VP890_AR_0DB_LOSS           0x00
#define VP890_AR_6DB_LOSS           0x10
#define VP890_AR_6DB_GAIN           0x20
#define VP890_AR_RSVD1              0x30
#define VP890_DR_LOSS_MASK          0x08  /**< Digital Receive Loss = 0dB (0) or
                                           * 6.02dB (1) */
#define VP890_DEFAULT_VP_GAIN       0x00

/**< Write/Read Channel 2 Voice Path Gains */
#define VP890_CIDCOR_MASK           0x80  /**< CID Amplification/Correction */
#define VP890_ERFB_MASK             0x40  /**< Receive Path Linearizing Feedback */
#define VP890_DRL_MASK              0x0C
#define VP890_DRL_0DB               0x00
#define VP890_DRL_6DB               0x04
#define VP890_DRL_12DB              0x08
#define VP890_DRL_18DB              0x0C
#define VP890_DRL_BITSHIFT          2
#define VP890_DTG_MASK              0x03
#define VP890_DTG_0DB               0x00
#define VP890_DTG_6DB               0x01
#define VP890_DTG_12DB              0x02


/**< Write/Read Channel 1 System State 56/57h */
#define VP890_SYS_STATE_WRT         0x56
#define VP890_SYS_STATE_RD          0x57

#ifndef VP890_SYS_STATE_LEN
#define VP890_SYS_STATE_LEN         0x01
#endif

#define VP890_SLIC_STATE_WRT        VP890_SYS_STATE_WRT
#define VP890_SLIC_STATE_RD         VP890_SYS_STATE_RD
#define VP890_SLIC_STATE_LEN        VP890_SYS_STATE_LEN

#define VP890_SS_RING_EXIT_MASK     0x80
#define VP890_SS_ACTIVATE_MASK      0x20    /**< (1) = CODEC Active, (0) =
                                             * Deactive */
#define VP890_SS_POLARITY_MASK      0x10    /**< (0) = Normal, (1) = Reverse */

#define VP890_SS_STATE_MASK         0x3F
#define VP890_SS_LINE_FEED_MASK     0x0F

#define VP890_SS_DISCONNECT         0x00
#define VP890_SS_TIP_OPEN           0x01
#define VP890_SS_RING_OPEN          0x02
#define VP890_SS_ACTIVE             (0x03 | VP890_SS_ACTIVATE_MASK)

#define VP890_SS_ACTIVE_POLREV      (0x03 | VP890_SS_ACTIVATE_MASK \
                                          | VP890_SS_POLARITY_MASK)

#define VP890_SS_IDLE               0x04
#define VP890_SS_IDLE_POLREV        (0x04 | VP890_SS_POLARITY_MASK)

#define VP890_SS_LONGITUDINAL_TEST  (0x05 | VP890_SS_ACTIVATE_MASK)
#define VP890_SS_METALLIC_TEST      (0x06 | VP890_SS_ACTIVATE_MASK)
#define VP890_SS_LOW_GAIN           (0x08 | VP890_SS_ACTIVATE_MASK)

#define VP890_SS_FEED_BALANCED_RINGING      0x07
#define VP890_SS_FEED_UNBALANCED_RINGING    0x0A

#define VP890_SS_BALANCED_RINGING   (VP890_SS_FEED_BALANCED_RINGING | VP890_SS_ACTIVATE_MASK)
#define VP890_SS_BALANCED_RINGING_PR    (VP890_SS_BALANCED_RINGING \
                                       | VP890_SS_POLARITY_MASK)

 /* Unbalanced ringing is undocumented, but needed for test primitives */
#define VP890_SS_UNBALANCED_RINGING (0x0A | VP890_SS_ACTIVATE_MASK)
#define VP890_SS_UNBALANCED_RINGING_PR  (VP890_SS_UNBALANCED_RINGING \
                                       | VP890_SS_POLARITY_MASK)

#define VP890_SS_SHUTDOWN           0x0F

/**< Write/Read Channel 2 System State */
#define VP890_SS_PFAIL_MASK         0x80  /**< (0) = PLL in Sync, (1) = PLL failed */
#define VP890_SS_SPE_MASK           0x40
#define VP890_SS_FXO_STATE_BITS     0x03
#define VP890_SS_FXO_STATE_MASK     (VP890_SS_FXO_STATE_BITS | VP890_SS_ACTIVATE_MASK)
#define VP890_SS_FXO_DISCONNECT     0x00
#define VP890_SS_FXO_ONHOOK         (0x01 | VP890_SS_ACTIVATE_MASK)
#define VP890_SS_FXO_OFFHOOK        (0x02 | VP890_SS_ACTIVATE_MASK)

/**< Write/Read Operating Functions 60/61h */
#define VP890_OP_FUNC_WRT           0x60
#define VP890_OP_FUNC_RD            0x61

#ifndef VP890_OP_FUNC_LEN
#define VP890_OP_FUNC_LEN           0x01
#endif

#define VP890_ENABLE_GR             0x20
#define VP890_ENABLE_GX             0x10
#define VP890_ENABLE_X              0x08
#define VP890_ENABLE_R              0x04
#define VP890_ENABLE_Z              0x02
#define VP890_ENABLE_B              0x01
#define VP890_ENABLE_LOADED_COEFFICIENTS 0x3F

#define VP890_DEFAULT_OP_FUNC_MODE 0x00

/**< Bit definitions for Codec Compression Command */
/* Note:  If Linear Mode is selected, u-Law/A-Law selection is ignored */
#define VP890_CODEC_COMPRESSION_MASK    0xC0
#define VP890_ALAW_CODEC            0x00    /**< a-Law compression is used */
#define VP890_ULAW_CODEC            0x40    /**< u-law compression is used */
#define VP890_LINEAR_CODEC          0x80    /**< Linear mode is used */


/**< Write/Read Channel 1 System State Configuration 68/69h */
#define VP890_SS_CONFIG_WRT         0x68
#define VP890_SS_CONFIG_RD          0x69
#define VP890_SS_CONFIG_LEN         0x01

/**< Automatic Clock Fault Switching */
#define VP890_ACFS_MASK             0x20
#define VP890_ACFS_EN               0x20
#define VP890_ACFS_DIS              0x00

/**< Auto Thermal Fault Switching */
#define VP890_ATFS_MASK             0x10
#define VP890_ATFS_EN               0x10
#define VP890_ATFS_DIS              0x00

/**< Zero Cross Ring Entry/Exit */
#define VP890_ZXR_MASK              0x08
#define VP890_ZXR_EN                0x00
#define VP890_ZXR_DIS               0x08

/**< Smooth Polarity Reversal */
#define VP890_SMOOTH_PR_MASK        0x04
#define VP890_SMOOTH_PR_EN          0x04
#define VP890_SMOOTH_PR_DIS         0x00

/**< Automatic System State Control */
#define VP890_AUTO_SSC              0x02
#define VP890_AUTO_SSC_EN           0x00
#define VP890_AUTO_SSC_DIS          0x02

/**< Automatic Battery Shutdown for Tracker */
#define VP890_ABAT                  0x01
#define VP890_AUTO_BAT_SHUTDOWN_EN  0x01
#define VP890_AUTO_BAT_SHUTDOWN_DIS 0x00

/**< Write/Read Channel 2 System State Configuration */
/**< Original Send Disable */
#define VP890_OSD_MASK              0x80
#define VP890_OSD_EN                0x00
#define VP890_OSD_DIS               0x80

/**< Automatic Overvoltage/Overcurrent Switching */
#define VP890_AOVS_MASK             0x40
#define VP890_AOVS_EN               0x40
#define VP890_AOVS_DIS              0x00

/**< Automatic Clock Fault Switching */
  /* Same as channel 1 */

/**< Line Down Enable */
#define VP890_LDE_MASK              0x10
#define VP890_LDE_EN                0x00
#define VP890_LDE_DIS               0x10

/**< Auto Polling Enable/Disable */
#define VP890_APOL_MASK             0x0C
#define VP890_APOL_DIS              0x00
#define VP890_APOL_8KHZ             0x04
#define VP890_APOL_200HZ            0x08
#define VP890_APOL_60HZ             0x0C


/**< Automatic System State Control */
  /* Same as channel 1 */

/**< Caller ID Tuning */
#define VP890_CIT_MASK              0x01
#define VP890_CIT_DIS               0x00  /**< CID normal gain */
#define VP890_CIT_EN                0x01  /**< CID signal increased gain */


/**< Write/Read Operating Conditions 70/71h */
#define VP890_OP_COND_WRT           0x70
#define VP890_OP_COND_RD            0x71

#ifndef VP890_OP_COND_LEN
#define VP890_OP_COND_LEN           0x01
#endif

/**< Map the loop back register to operating conditions register */
#define VP890_LOOPBACK_WRT          VP890_OP_COND_WRT
#define VP890_LOOPBACK_RD           VP890_OP_COND_RD
#define VP890_LOOPBACK_LEN          VP890_OP_COND_LEN

/**< Bit definitions for Operating Conditions Command */
#define VP890_TX_PATH_MASK          0x80
#define VP890_CUT_TXPATH            0x80
#define VP890_TXPATH_EN             0x00
#define VP890_TXPATH_DIS            0x80

#define VP890_RX_PATH_MASK          0x40
#define VP890_CUT_RXPATH            0x40
#define VP890_RXPATH_EN             0x00
#define VP890_RXPATH_DIS            0x40

#define VP890_HIGH_PASS_MASK        0x20
#define VP890_HIGH_PASS_EN          0x00
#define VP890_HIGH_PASS_DIS         0x20

#define VP890_LOWER_RX_GAIN_MASK    0x10
#define VP890_RX_GAIN_6DB_LOSS      0x10
#define VP890_RX_GAIN_0DB_LOSS      0x00

#define VP890_NOISE_GEN_MASK        0x08
#define VP890_NOISE_GEN_EN          0x08
#define VP890_NOISE_GEN_DIS         0x00

#define VP890_INTERFACE_LOOPBACK_EN 0x04

#define VP890_1KHZ_TONE_ON          0x01

#define VP890_OPCOND_RSVD_MASK      (0x0A | VP890_1KHZ_TONE_ON)
#define VP890_NORMAL_OP_COND_MODE   0x00

/**< Write/Read Line Side Device Control 76/77h */
#define VP890_LSD_CTL_WRT           0x76
#define VP890_LSD_CTL_RD            0x77
#define VP890_LSD_CTL_LEN           0x01

#define VP890_LSD_VAS_MASK          0x60  /**< Line overhead voltage control */
#define VP890_LSD_VAS_2_4V          0x00
#define VP890_LSD_VAS_1_8V          0x20
#define VP890_LSD_VAS_1_2V          0x40
#define VP890_LSD_VAS_0_6V          0x60

#define VP890_LSD_IMP_MASK          0x1C  /**< Base line impedance control */

#define VP890_LSD_CSE_MASK          0x01  /**< Current sensor enable */
#define VP890_LSD_CSE_EN            0x00
#define VP890_LSD_CSE_DIS           0x01


/**< Write/Read Line Side Device Status 78/79h */
#define VP890_LSD_STAT_WRT          0x78
#define VP890_LSD_STAT_RD           0x79
#define VP890_LSD_STAT_LEN          0x01

#define VP890_LSD_TM2               0x04


/**< Write/Read GX Filter Coefficients 80/81h */
#define VP890_GX_GAIN_WRT           0x80
#define VP890_GX_GAIN_RD            0x81
#define VP890_GX_GAIN_LEN           0x02


/**< Write/Read GR Filter Coefficients 82/83h */
#define VP890_GR_GAIN_WRT           0x82
#define VP890_GR_GAIN_RD            0x83
#define VP890_GR_GAIN_LEN           0x02


/**< Write/Read B Filter FIR Coefficients 86/87h */
#define VP890_B1_FILTER_WRT         0x86
#define VP890_B1_FILTER_RD          0x87

#ifndef VP890_B1_FILTER_LEN
#define VP890_B1_FILTER_LEN         0x0E
#endif

/**< Write/Read B Filter IIR Coefficients 96/97h */
#define VP890_B2_FILTER_WRT         0x96
#define VP890_B2_FILTER_RD          0x97

#ifndef VP890_B2_FILTER_LEN
#define VP890_B2_FILTER_LEN         0x02
#endif


/**< Write/Read X Filter Coefficients 88/89h */
#define VP890_X_FILTER_WRT          0x88
#define VP890_X_FILTER_RD           0x89
#define VP890_X_FILTER_LEN          0x0C


/**< Write/Read R Filter Coefficients 8A/8Bh */
#define VP890_R_FILTER_WRT          0x8A
#define VP890_R_FILTER_RD           0x8B
#define VP890_R_FILTER_LEN          0x0E


/**< Write/Read Z Filter FIR Coefficients 98/99h */
#define VP890_Z1_FILTER_WRT         0x98
#define VP890_Z1_FILTER_RD          0x99
#define VP890_Z1_FILTER_LEN         0x0A


/**< Write/Read Z Filter IIR Coefficients 9A/9Bh */
#define VP890_Z2_FILTER_WRT         0x9A
#define VP890_Z2_FILTER_RD          0x9B
#define VP890_Z2_FILTER_LEN         0x05


/**< Write/Read Channel 1 Converter Configuration A6/A7h */
#define VP890_CONV_CFG_WRT          0xA6
#define VP890_CONV_CFG_RD           0xA7

#ifndef VP890_CONV_CFG_LEN
#define VP890_CONV_CFG_LEN          0x01
#endif

#define VP890_ATI_MASK              0x80  /**< Arm Transmit PCM Data Interrupt */
#define VP890_ATI_EN                0x80
#define VP890_ATI_DIS               0x00

#define VP890_CC_RATE_MASK          0x70
#define VP890_CC_500HZ_RATE         0x40
#define VP890_CC_1KHZ_RATE          0x30
#define VP890_CC_2KHZ_RATE          0x20
#define VP890_CC_4KHZ_RATE          0x10
#define VP890_CC_8KHZ_RATE          0x00

#define VP890_CONV_CONNECT_BITS     0x0F
#define VP890_METALLIC_AC_V         0x00
#define VP890_SWITCHER_Y            0x01
#define VP890_TIP_TO_GND_V          0x04
#define VP890_RING_TO_GND_V         0x05
#define VP890_METALLIC_DC_V         0x06
#define VP890_METALLIC_DC_I         0x07
#define VP890_LONGITUDINAL_DC_I     0x08
#define VP890_CALIBRATION_I         0x09
#define VP890_VOICE_DAC             0x0A
#define VP890_NO_CONNECT            0x0B
#define VP890_LOW_TIP_TO_GND_V      0x0D
#define VP890_LOW_RING_TO_GND_V     0x0E

/**< Write/Read Channel 2 Converter Configuration */
#define VP890_INSEL_MASK            0x07
/*      VP890_METALLIC_AC_V         0x00 - as above*/
#define VP890_DAC_PWM_OUT           0x01
#define VP890_INST_ONHOOK_V         0x02
#define VP890_AVG_ONHOOK_V          0x03
#define VP890_INST_OFFHOOK_I        0x04
#define VP890_AVG_OFFHOOK_I         0x05
#define VP890_ENH_B_FILTER_AVG_DET  0x06

#define VP890_ILA_SCALE_1MA     (568)
#define VP890_ILA_SCALE_2MA     (1136)
#define VP890_ILA_SCALE_18MA    (10227)

/**< Write/Read Channel 1 Loop Supervision Parameters C2/C3h */
#define VP890_LOOP_SUP_WRT          0xC2
#define VP890_LOOP_SUP_RD           0xC3
#ifndef VP890_LOOP_SUP_LEN
#define VP890_LOOP_SUP_LEN          0x04
#endif

#define VP890_LOOP_SUP_THRESH_BYTE  0x00
#define VP890_LOOP_SUP_DEBOUNCE_BYTE 0x01
#define VP890_LOOP_SUP_RT_MODE_BYTE 0x02
#define VP890_LOOP_SUP_ILR_BYTE     0x03

#define VP890_GKEY_THRESH_MASK      0x38
#define VP890_SWHOOK_THRESH_MASK    0x07
#define VP890_GKEY_DEBOUNCE_MASK    0xE0
#define VP890_SWHOOK_DEBOUNCE_MASK  0x1F
#define VP890_RINGTRIP_DCAC_MASK    0x80
#define VP890_RINGTRIP_THRESH_MASK  0x7F
#define VP890_RING_I_LIMIT_MASK     0x0F

/**< Command to control how Ring Trip will work */
#define VP890_RINGTRIP_MODE_WRT     0xC2
#define VP890_RINGTRIP_MODE_RD      0xC3
#define VP890_RINGTRIP_MODE_LEN     0x04

#define VP890_RING_TRIP_DC          0x00
#define VP890_RING_TRIP_AC          0x80

/**< Write/Read Channel 2 Loop Supervision Parameters */
    /* Byte 1 */
#define VP890_RDSEL_MASK            0x80
#define VP890_POH_THRESH_MASK       0x70
#define VP890_LIU_THRESH_MASK       0x0E
#define VP890_TRAMP_MSB_LOC         0x00
#define VP890_TRAMP_MSB_MASK        0x01
    /* Byte 2 */
#define VP890_TRAMP_LSB_LOC         0x01
#define VP890_TRAMP_LSB_MASK        0xC0
#define VP890_LDN_DISC_THRESH_MASK  0x30
#define VP890_POH_INT_TIME_MASK     0x0C
#define VP890_IOVER_THRESH_MASK     0x03

#define VP890_INVERT_BOOST_DISABLE_TIME (150)
#define VP890_FIXED_TRACK_DISABLE_TIME  (400)

/**< Write/Read Channel 1 DC Feed Parameters C6/C7h */
#define VP890_DC_FEED_WRT           0xC6
#define VP890_DC_FEED_RD            0xC7
#define VP890_DC_FEED_LEN           0x02
#define VP890_DC_CAL_VOC_DIS        0x10
#define VP890_VAS_MAX               (14250) /**< 14.25V per silicon */

#define VP890_VOC_12_33_SETTING     0x40

#define VP890_LONG_IMP_MASK         0x20
#define VP890_LONG_IMP_50           0x20
#define VP890_LONG_IMP_100          0x00

/*  Calibration  Values */
#define VP890_ABV_INIT_WAIT     (10)

#define VP890_ADC_MAX_POS       0x7FFF

#define VP890_CAL_SET           (10)
#define VP890_CAL_ABV_LONG      (100)

#define VP890_CONVERTER_MEAS_DELAY      (10)

#define VP890_CAL_VOC_SHORT     (20)
#define VP890_CAL_VOC_LONG      (100)

#define VP890_VAS_INIT_WAIT     (20)
#define VP890_VAS_MEAS_DELAY    (60)

#define VP890_VAS_MEAS_ERR      (100)   /**< Times 1.817ua = 187uA */
#define VP890_IMT_AVERAGE_CNT   (1)
#define VP890_VAS_OVERHEAD      (3000)  /**< 3V for Temperature Variation */

#define VP890_ABV_STEP          (170)   /**< Based on 1.25V / 7.324mV/step */

#define VP890_V_PCM_LSB         (7324)  /**< 7.324V per LSB from PCM Data */
#define VP890_V_SCALE           ((int32)10000) /**< Scale to put in 10mV */

#define VP890_V_1V_SCALE        ((int32)13654)  /**< Based on 7.324V per LSB from PCM Data */
#define VP890_V_1V_RANGE        (100)   /**< To scale back to PCM range used */

typedef enum {
    VP890_NORM_POLARITY,
    VP890_REV_POLARITY,
    VP890_NUM_POLARITY
} Vp890CalPolarityType;

#ifdef VP890_FXS_SUPPORT
bool
Vp890UpdateCalValue(
    VpLineCtxType *pLineCtx);
#endif

#define VP890_VOC_MASK              0x1C
#define VP890_VOC_SHIFT_MASK        0x40
#define VP890_VOC_LOW_RANGE         0x40

#define VP890_VOC_VALUE_MASK        (VP890_VOC_MASK | VP890_VOC_LOW_RANGE)

#define VP890_ILA_MASK              0x1F
#define VP890_ILA_INDEX             0x01
#define VP890_VOC_INDEX             0x00


#define VP890_ABV_MASK              0x1F
#define VP890_VAS_MASK_LO           0x03
#define VP890_VAS_MASK_HI           0xC0

#define VP890_VOC_57V               0x1C
#define VP890_VOC_54V               0x18
#define VP890_VOC_51V               0x14
#define VP890_VOC_48V               0x10
#define VP890_VOC_45V               0x0C
#define VP890_VOC_42V               0x08
#define VP890_VOC_39V               0x04
#define VP890_VOC_36V               0x00

#define VP890_VAS_MSB_LOC           0x00
#define VP890_VAS_MSB_MASK          0x03

#define VP890_VAS_LSB_LOC           0x01
#define VP890_VAS_LSB_MASK          0xC0

#define VP890_I_LIM_ACTIVE_MASK     0x1F

#define VP890_VAS_CONVERSION(msb, lsb)  (750*(((msb << 2) & 0xC) | ((lsb >> 6) & 0x3))+3000)

/* Position in DC Profile */
#define VP890_VOC_PROFILE_POSITION_V0 12
#define VP890_VOC_PROFILE_POSITION_V1 7
#define VP890_HOOK_HYST_POSITION      12

/**< Write/Read Digital Impedance Scaling Network (DISN) CA/CBh */
#define VP890_DISN_WRT              0xCA
#define VP890_DISN_RD               0xCB

#ifndef VP890_DISN_LEN
#define VP890_DISN_LEN              0x01
#endif

#ifndef VP890_VP_GAIN_LEN
#define VP890_VP_GAIN_LEN           0x01
#endif

/**< Read Channel 1 Transmit PCM/Test Data CDh */
#define VP890_TX_PCM_DATA_RD        0xCD
#define VP890_TX_PCM_DATA_LEN       0x02

/**< Read Channel 2 Transmit PCM/Test Data */
#define VP890_FXO_OVERVOLTAGE_MASK  0x80   /**< 1 means >82V on Tip and Ring */
#define VP890_FXO_LINE_V_MASK       0x7F   /**< LSB = 1.28V */
#define VP890_FXO_LINE_V_SIGN_BIT   0x40
/* overcurrent bit somewhere */


/**< Write/Read Channel 1 Signal Generator A, B and Bias Parameters D2/D3h */
#define VP890_SIGA_PARAMS_WRT       0xD2
#define VP890_SIGA_PARAMS_RD        0xD3
#define VP890_SIGA_PARAMS_LEN       0x0B

#define VP890_SIGAB_PARAMS_WRT      VP890_SIGA_PARAMS_WRT
#define VP890_SIGAB_PARAMS_RD       VP890_SIGA_PARAMS_RD
#define VP890_SIGAB_PARAMS_LEN      VP890_SIGA_PARAMS_LEN

#define VP890_RINGER_PARAMS_WRT     VP890_SIGA_PARAMS_WRT
#define VP890_RINGER_PARAMS_RD      VP890_SIGA_PARAMS_RD
#define VP890_RINGER_PARAMS_LEN     VP890_SIGA_PARAMS_LEN

/* Map of the Signal Generator A/B Byte Locations */
#define VP890_SIGA_BIAS_MSB         0x01
#define VP890_SIGA_BIAS_LSB         0x02
#define VP890_SIGAB_FREQ_START      0x03
#define VP890_SIGA_FREQ_MSB         0x03
#define VP890_SIGA_FREQ_LSB         0x04
#define VP890_SIGA_AMP_MSB          0x05
#define VP890_SIGA_AMP_LSB          0x06
#define VP890_SIGB_FREQ_MSB         0x07
#define VP890_SIGB_FREQ_LSB         0x08
#define VP890_SIGB_AMP_MSB          0x09
#define VP890_SIGB_AMP_LSB          0x0A

#define VP890_RINGING_AMP_SCALE     473
#define VP890_RINGING_AMP_FACTOR    100000

#define VP890_RINGING_BIAS_SCALE    473
#define VP890_RINGING_BIAS_FACTOR   100000


/**< Signal Generator bit definitions */
#define VP890_RAMP_SLOPE_MASK       0x04
#define VP890_RAMP_POSITIVE         0x00
#define VP890_RAMP_NEGATIVE         0x04

#define VP890_SIGGEN1_OP_MASK       0x02
#define VP890_SIGGEN1_OUT_RAMP      0x02
#define VP890_SIGGEN1_OUT_CONT      0x00

#define VP890_SIGGEN1_SINTRAP_MASK  0x01
#define VP890_SIGGEN1_SIN           0x00
#define VP890_SIGGEN1_TRAP          0x01


/**< Write/Read Signal Generator C and D Parameters D4/D5h */
#define VP890_SIGC_PARAMS_WRT       0xD4
#define VP890_SIGC_PARAMS_RD        0xD5
#define VP890_SIGC_PARAMS_LEN       0x08

#define VP890_SIGD_PARAMS_WRT       0xD4
#define VP890_SIGD_PARAMS_RD        0xD5
#define VP890_SIGD_PARAMS_LEN       0x08

#define VP890_SIGCD_PARAMS_WRT      0xD4
#define VP890_SIGCD_PARAMS_RD       0xD5
#define VP890_SIGCD_PARAMS_LEN      0x08

/* Map of the Signal Generator C/D Byte Locations */
#define VP890_SIGC_FREQ_MSB         0x00
#define VP890_SIGC_FREQ_LSB         0x01
#define VP890_SIGC_AMP_MSB          0x02
#define VP890_SIGC_AMP_LSB          0x03
#define VP890_SIGD_FREQ_MSB         0x04
#define VP890_SIGD_FREQ_LSB         0x05
#define VP890_SIGD_AMP_MSB          0x06
#define VP890_SIGD_AMP_LSB          0x07

/**< Write/Read Signal Generator Control DE/DFh */
#define VP890_GEN_CTRL_WRT          0xDE
#define VP890_GEN_CTRL_RD           0xDF

#ifndef VP890_GEN_CTRL_LEN
#define VP890_GEN_CTRL_LEN          0x01
#endif

#define VP890_GEN_CAD               0x80
#define VP890_GEN_CNTOS             0x40
#define VP890_GEN_DP                0x20
#define VP890_GEN_BIAS              0x10
#define VP890_GEND_EN               0x08
#define VP890_GENC_EN               0x04
#define VP890_GENB_EN               0x02
#define VP890_GENA_EN               0x01

#define VP890_GEN_EN_MASK           0x0F
#define VP890_GEN_CH2_EN_MASK       0x0C
#define VP890_GEN_ALLOFF            0x00
#define VP890_GEN_ALLON             0x0F

/**< Signal Generator Control bit definitions */
#define VP890_GEN_CAD_EN            0x80
#define VP890_GEN_CAD_DIS           0x00

#define VP890_GEN_CTRL_CONT         0x00
#define VP890_GEN_CTRL_OS           0x40

#define VP890_GEN_CTRL_DPGEN_EN     0x20
#define VP890_GEN_CTRL_DPGEN_DIS    0x00

#define VP890_GEN_CTRL_EN_BIAS      0x10
#define VP890_GEN_CTRL_DIS_BIAS     0x00

#define VP890_GEN_CTRL_EN_D         0x08
#define VP890_GEN_CTRL_DIS_D        0x00

#define VP890_GEN_CTRL_EN_C         0x04
#define VP890_GEN_CTRL_DIS_C        0x00

#define VP890_GEN_CTRL_EN_B         0x02
#define VP890_GEN_CTRL_DIS_B        0x00

#define VP890_GEN_CTRL_EN_A         0x01
#define VP890_GEN_CTRL_DIS_A        0x00


/**< Write/Read Cadence Timer E0/E1h */
#define VP890_CADENCE_TIMER_WRT     0xE0
#define VP890_CADENCE_TIMER_RD      0xE1
#define VP890_CADENCE_TIMER_LEN     0x04


/**< Write/Read Channel 1 Caller Identification Number Data E2/E3h */
#define VP890_CID_DATA_WRT          0xE2
#define VP890_CID_DATA_RD           0xE3
#define VP890_CID_DATA_LEN          0x01


/**< Write/Read Channel 1 Caller Identification Number Parameters EA/EBh */
#define VP890_CID_PARAM_WRT         0xEA
#define VP890_CID_PARAM_RD          0xEB
#define VP890_CID_PARAM_LEN         0x01

#define VP890_CID_EOM_MASK          0x10
#define VP890_CID_EOM               0x10
#define VP890_CID_NEOM              0x00
#define VP890_CID_FBDIS_MASK        0x08
#define VP890_CID_FBSRT_MASK        0x04
#define VP890_CID_FBSTP_MASK        0x02
#define VP890_CID_DISABLE_MASK      0x01

#define VP890_CID_STATE_MASK        0xE0
#define VP890_CID_STATE_IDLE        0x00
#define VP890_CID_STATE_RDY         0x20
#define VP890_CID_STATE_FULL        0x40
#define VP890_CID_STATE_LBYTE       0x60
#define VP890_CID_STATE_L2BYTE      0x80
#define VP890_CID_STATE_URUN        0xA0

/**< Write/Read Channel 1 Internal Configuration Register 1 EC/EDh */
#define VP890_ICR1_WRT              0xEC
#define VP890_ICR1_RD               0xED
#define VP890_ICR1_LEN              0x04

#define VP890_ICR1_BIAS_OVERRIDE_LOCATION   0x00
#define VP890_ICR1_TIP_BIAS_OVERRIDE        0xF0
#define VP890_ICR1_LINE_BIAS_OVERRIDE       0x0F
#define VP890_ICR1_LINE_BIAS_OVERRIDE_NORM  0x08

#define VP890_ICR1_LINE_BIAS_LOCATION       0x01
#define VP890_ICR1_LINE_BIAS                0x0F
#define VP890_ICR1_TIP_BIAS_LOCATION        0x01
#define VP890_ICR1_TIP_BIAS                 0xF0

#define VP890_ICR1_RING_BIAS_OVERRIDE_LOCATION  0x02
#define VP890_ICR1_RING_BIAS_OVERRIDE           0x0F

#define VP890_ICR1_RING_AND_DAC_LOCATION    0x02
#define VP890_ICR1_RING_AND_DAC_B2_3        0x06
#define VP890_ICR1_RING_BIAS_DAC_MASK       0x0C

#define VP890_ICR1_RING_BIAS_LOCATION       0x03
#define VP890_ICR1_RING_BIAS                0x0F

#define VP890_M_D_IBT_MASK          0xF0
#define VP890_M_D_IBO_MASK          0x0F

#define VP890_D_IBT_MASK            0xF0
#define VP890_D_IBO_MASK            0x0F

#define VP890_M_D_IBR_MASK          0x0F

#define VP890_D_IBR_MASK            0x0F

#define VP890_SUB_ST_IDX                0x03
#define VP890_SUB_ST_MASK               0x07
#define VP890_SUB_ST_FXO_ON_HOOK_SUP    0x03
#define VP890_SUB_ST_FXO_OFF_HOOK_SUP   0x04

/**< Write/Read Channel 2 Internal Configuration Register 1 */
#define VP890_C_FXO_ANALOG_EN       0x80
#define VP890_C_FXO_ANALOG_RESET_N  0x40
#define VP890_ICR1_M_HOK            0x01
#define VP890_ICR1_HOK              0x01
#define VP890_ICR1_RGT              0x40
#define VP890_ICR1_RO0              0x20
#define VP890_ICR1_LDN              0x04
#define VP890_ICR1_DDY              0x02


/**< Write/Read Channel 1 Internal Configuration Register 2 EE/EFh */
#define VP890_ICR2_WRT              0xEE
#define VP890_ICR2_RD               0xEF
#define VP890_ICR2_LEN              0x04


#define VP890_ICR2_SENSE_INDEX      0x00
#define VP890_ICR2_ILA_DAC          0x80
#define VP890_ICR2_DAC_SENSE        0x80
#define VP890_ICR2_FEED_SENSE       0x40
#define VP890_ICR2_VOC_DAC_SENSE    0x20
#define VP890_ICR2_TIP_SENSE        0x08
#define VP890_ICR2_RING_SENSE       0x04
#define VP890_ICR2_VOC_DAC_INDEX    0x00

/*
 * These are all different ways to refer to bits in the same byte location
 * ICR2 (index = 2)
 */
#define VP890_ICR2_MET_SPEED_INDEX  0x02
#define VP890_ICR2_MET_SPEED_CTRL   0x80

#define VP890_ICR2_FEED_CTRL_INDEX  0x02
#define VP890_ICR2_FEED_CTRL        0x40

#define VP890_ICR2_SWY_CTRL_INDEX   0x02
#define VP890_ICR2_SWY_CTRL_EN      0x20
#define VP890_ICR2_SWY_LIM_CTRL1    0x08
#define VP890_ICR2_SWY_LIM_CTRL     0x04

/**< Write/Read Internal Configuration Register 3 F2/F3h */
#define VP890_ICR3_WRT              0xF2
#define VP890_ICR3_RD               0xF3
#ifndef VP890_ICR3_LEN
#define VP890_ICR3_LEN              0x04
#endif

#define VP890_ICR3_LINE_CTRL_INDEX      0x00
#define VP890_ICR3_LINE_CTRL            0x20
#define VP890_ICR3_VREF_CTRL            0x01

#define VP890_ICR3_LONG_UNCLAMP         0x40
#define VP890_ICR3_LONG_UNCLAMP_INDEX   0x02
#define VP890_ICR3_LONG_FIXED           0x10
#define VP890_ICR3_LONG_FIXED_INDEX     0x00

#define VP890_ICR3_LONG_LOOP_CTRL_LOCATION  0x02
#define VP890_ICR3_LONG_LOOP_CONTROL        0x80
#define VP890_ICR3_SE_RINGING_CONTROL       0x01


/**< Write/Read Internal Configuration Register 4 F4/F5h */
#define VP890_ICR4_WRT              0xF4
#define VP890_ICR4_RD               0xF5
#define VP890_ICR4_LEN              0x04

#define VP890_ICR4_VOICE_DAC_INDEX      0x00
#define VP890_ICR4_VOICE_DAC_CTRL       0x01

#define VP890_ICR4_SUP_INDEX            0x02
#define VP890_ICR4_SUP_DAC_CTRL         0x08
#define VP890_ICR4_SUP_DET_CTRL         0x04
#define VP890_ICR4_SUP_POL_CTRL         0x02

#define VP890_ICR4_GKEY_DET_LOCATION    0x02
#define VP890_ICR4_GKEY_POL             0x10
#define VP890_ICR4_GKEY_DET             0x01


/**< Write/Read Internal Configuration Register 5 FA/FBh */
#define VP890_ICR5_WRT              0xFA
#define VP890_ICR5_RD               0xFB
#define VP890_ICR5_LEN              0x02

#define VP890_ICR5_FEED_HOLD_INDEX  0x00

#define VP890_ICR5_FEED_HOLD_MASK   0xF0
#define VP890_ICR5_BAT_HOLD_MASK    0x0F
#define VP890_ICR5_H2MS_BYTE        1
#define VP890_ICR5_H2MS_MASK        0xF0
#define VP890_ICR5_SUBST_MASK       0x0F


/**< Write Read Channel 1 Internal Configuration Register 7 E8/E9h */


/**< Write/Read DC Calibration FC/FDh */
#define VP890_DC_CAL_REG_WRT        0xFC
#define VP890_DC_CAL_REG_RD         0xFD

#ifndef VP890_DC_CAL_REG_LEN
#define VP890_DC_CAL_REG_LEN        0x02
#endif

#define VP890_DC_CAL_CUT_INDEX      0x01
#define VP890_C_YBAT_SNS_CUT        0x80
#define VP890_C_RING_SNS_CUT        0x40
#define VP890_C_TIP_SNS_CUT         0x20
#define VP890_DCCAL_RSVD1           0x02

#define VP890_DC_CAL_CUT_MASK       (VP890_C_YBAT_SNS_CUT | VP890_C_RING_SNS_CUT \
                                    | VP890_C_TIP_SNS_CUT)

#define VP890_C_IBUF5_VOC_OFF       0x10

#define VP890_DC_CAL_ABS_INDEX  0
#define VP890_DC_CAL_ABS_MASK   0xF0

#define VP890_DC_CAL_BLIM_INDEX 1
#define VP890_DC_CAL_BLIM       0x08

/* New CID States for Rev D Silicon */
#define VP890_CID_STATE_IDLE_D      0x00
#define VP890_CID_STATE_EMPTY_D     0x20
#define VP890_CID_STATE_HALF_FULL_D 0x40
#define VP890_CID_STATE_LBYTE_D     0x60
#define VP890_CID_STATE_L2BYTE_D    0x80
#define VP890_CID_STATE_URUN_D      0xA0
#define VP890_CID_STATE_FULL_D      0xC0
#define VP890_CID_STATE_L3BYTE_D    0xE0

#define VP890_CID_EOM_MASK      0x10
#define VP890_CID_EOM           0x10
#define VP890_CID_NEOM          0x00

#define VP890_CID_FBDIS_MASK    0x08
#define VP890_CID_FBIT_DIS      0x08
#define VP890_CID_FBIT_EN       0x00

#define VP890_CID_FB_START_MASK 0x04
#define VP890_CID_FB_START_0    0x00
#define VP890_CID_FB_START_1    0x04

#define VP890_CID_FB_STOP_MASK  0x02
#define VP890_CID_FB_STOP_0     0x00
#define VP890_CID_FB_STOP_1     0x02

#define VP890_CID_FRAME_BITS    0x06

#define VP890_CID_CTRL_MASK     0x01
#define VP890_CID_EN            0x00
#define VP890_CID_DIS           0x01


#define VP890_EC_CH1            0x01
#define VP890_EC_CH2            0x02
#define VP890_WIDEBAND_MODE     0x20


/*
 * In ms, before transitioning from Disconnect to feed state to prevent ping
 * on the phone. This exists because the device does a short pol rev on the
 * line when transitioning from Disconnect to a Feed state.
 */
#define VP890_PING_TIME         (30)

#define VP890_TRACKER_DISABLE_TIME          (500)

/* Defines whether the Parallel Off-Hook event is supported */
#undef VP890_POH_SUPPORTED

#ifdef VP890_POH_SUPPORTED
#define VP890_POH_EVENT VP_LINE_EVID_POH
#else
#define VP890_POH_EVENT VP890_NO_MASK
#endif /* VP890_POH_SUPPORTED */

/**< Define the mask that will report device busy if there is a currently active
 * event when the user is attempting to perform another "read" transaction.
 */
#define VP890_NO_MASK   0x0000

#define VP890_READ_RESPONSE_MASK (VP_LINE_EVID_LLCMD_RX_CMP \
                                 | VP_LINE_EVID_RD_OPTION \
                                 | VP_LINE_EVID_GAIN_CMP)

#define VP890_NONSUPPORT_FAULT_EVENTS  (~(VP_DEV_EVID_BAT_FLT \
                                       | VP_DEV_EVID_CLK_FLT \
                                       | VP_LINE_EVID_THERM_FLT \
                                       | VP_LINE_EVID_RES_LEAK_FLT \
                                       | VP_LINE_EVID_DC_FLT))

#define VP890_NONSUPPORT_SIGNALING_EVENTS   (~(VP_LINE_EVID_HOOK_OFF \
                                           | VP_LINE_EVID_HOOK_ON \
                                           | VP_LINE_EVID_GKEY_DET \
                                           | VP_LINE_EVID_GKEY_REL \
                                           | VP_LINE_EVID_FLASH \
                                           | VP_LINE_EVID_STARTPULSE \
                                           | VP_LINE_EVID_PULSE_DIG \
                                           | VP_DEV_EVID_TS_ROLLOVER \
                                           | VP_LINE_EVID_EXTD_FLASH))

#define VP890_FXS_SIGNALING_EVENTS  (VP_LINE_EVID_HOOK_OFF \
                                   | VP_LINE_EVID_HOOK_ON \
                                   | VP_LINE_EVID_GKEY_DET \
                                   | VP_LINE_EVID_GKEY_REL \
                                   | VP_LINE_EVID_FLASH \
                                   | VP_LINE_EVID_STARTPULSE \
                                   | VP_LINE_EVID_PULSE_DIG \
                                   | VP_LINE_EVID_BREAK_MAX \
                                   | VP_LINE_EVID_EXTD_FLASH)

#define VP890_NONSUPPORT_RESPONSE_EVENTS    (~(VP_LINE_EVID_LLCMD_TX_CMP \
                                           | VP_LINE_EVID_LLCMD_RX_CMP \
                                           | VP_LINE_EVID_RD_OPTION \
                                           | VP_EVID_CAL_CMP \
                                           | VP_EVID_CAL_BUSY \
                                           | VP_LINE_EVID_GAIN_CMP \
                                           | VP_DEV_EVID_DEV_INIT_CMP \
                                           | VP_LINE_EVID_LINE_INIT_CMP \
                                           | VP_DEV_EVID_IO_ACCESS_CMP \
                                           | VP_LINE_EVID_GAIN_CMP \
                                           | VP_LINE_EVID_GAIN_CMP))

#define VP890_NONSUPPORT_TEST_EVENTS        (VP_LINE_EVID_TEST_RSVD1 \
                                           | VP_LINE_EVID_DTONE_DET \
                                           | VP_LINE_EVID_DTONE_LOSS \
                                           | VP_DEV_EVID_STEST_CMP \
                                           | VP_DEV_EVID_CHKSUM)

#define VP890_NONSUPPORT_PROCESS_EVENTS VP890_NO_MASK

#define VP890_NONSUPPORT_FXO_EVENTS     VP890_POH_EVENT

/***** Timer lengths *****/
#define VP890_PLL_RECOVER_INIT_DELAY    (250)
#define VP890_PLL_RECOVER_MEAS_DELAY_1  (150)   /*  10 */
#define VP890_PLL_RECOVER_MEAS_DELAY_2  (10)    /* 20 */

/* Make sure to not set these higher than each timer's data type allows */
/* 1ms increments.  Time to ignore detectors when going onhook */
#define VP890_FXO_ONHOOK_CHANGE_DEBOUNCE    200

/* 1ms increments.  Time to ignore all detectors when going offhook */
#define VP890_FXO_OFFHOOK_CHANGE_DEBOUNCE   350

/* For polrevs before ring detect. 100ms + max ring period, 1ms increments */
#define VP890_FXO_POLREV_SILENCE            (100 + (pLineObj->ringDetMax + 1) / 2)

/* Time limit for ringTimer.  1ms increments, from ringDetMax rounded up */
#define VP890_FXO_RING_MAX  ((pLineObj->ringDetMax + (pLineObj->ringDetMax / 4)) / 2)

/* With some DC biased ringing, LIU can show up before any other detection.
   Length of 1 max ring period in 1ms increments */
#define VP890_FXO_LIU_DEBOUNCE              ((pLineObj->ringDetMax + 1) / 2)

/* POH bit is always activated following a polrev, so we have to ignore it for
 * a time based on DPOH.  1ms increments */
#define VP890_FXO_POH_IGNORE                ((32 << pLineObj->dPoh) * 2)

#define VP890_PULSE_DETECT_ADJUSTMENT       48      /* 6ms To adjust for slow onhook
                                                       detection.  125us increments */
/* Time to delay offhook event when pulse detection mode is off.  1ms increments */
#define VP890_OFFHOOK_EVENT_DELAY           (VP890_PULSE_DETECT_ADJUSTMENT / 8)

/* Time to ignore hook changes after going offhook during ringing. 1ms increments */
#define VP890_RING_TRIP_DEBOUNCE            VP_FXO_RING_TRIP_DEBOUNCE

#define VP890_FXO_RING_OFF_DEBOUNCE_DISC    200     /* 200ms to ignore disconnect
                                                       after RING_OFF */
#define VP890_FXO_RING_OFF_DEBOUNCE_LIU     110     /* 110ms to ignore LIU
                                                       after RING_OFF */

#define VP890_FXO_CID_CORRECTION_START      150      /* Start measuring avg voltage
                                                     * 20ms after FEED_EN or init */
#define VP890_FXO_CID_CORRECTION_END        210      /* Read measurement and calculate
                                                     * adjustment 60ms after start */


/* The initial settings of the fake test load state take a while to settle the
 * DC values of the battery.  Wait this time before disabling tip and ring bias
 * to make tip and ring outputs high impedance, and tend to pull to battery. */
#define VP890_INTERNAL_TESTTERM_SETTLING_TIME   200  /* ms */

/* Time in ms between measurements for the low voltage LIU/disconnect
 * distinction workaround.  The averaged sample updates every 50ms, so use a
 * longer timer to make sure we get different samples each time. */
#define VP890_FXO_LOW_VOLTAGE_TIMER_LEN     60

/* Number of successive voltage readings needed to indicate true disconnect */
#define VP890_FXO_LOW_VOLTAGE_DISC_COUNT    2

/* Number of successive voltage readings needed to clear true disconnect */
#define VP890_FXO_LOW_VOLTAGE_NOTDISC_COUNT 2


/* Current monitor constants */
#define VP890_CM_OFFSET_TOLERANCE   16  /* Maximum difference allowed between
                                         * min/max of offset samples */
#define VP890_CM_OFFSET_NUM         4   /* Number of consecutive offset samples
                                         * to test for consistency */
#define VP890_CM_OFFSET_NUM_LIMIT   15  /* Maximum number of offset measurements
                                         * before giving up */

typedef enum {
    /**< One Channel Devices */
#ifdef VP890_FXS_SUPPORT
    VP890_DEV_PCN_89116 = 0x00,     /**< FXS - Wideband */
#endif

    VP890_DEV_PCN_89316 = 0x01,     /**< FXO/FXS-Tracker - Wideband */
    /*
     * Single Channel FXO is same PCN as 89316, except always has register 0x3F
     * set to 0x01. This distinction is made inside the API-II.
     */
#ifdef VP890_FXO_SUPPORT
    VP890_DEV_PCN_89010 = 0x02,     /**< Single Channel FXO */
#endif

    /*
     * HV devices have the same silicon PCN has their LV equivalent. The SW has
     * to determine if HV/LV and set the PCN value returned by GetDeviceInfo().
     */
#ifdef VP890_FXS_SUPPORT
    VP890_DEV_PCN_89136 = 0x04,     /**< HV FXS - Wideband */
#endif

    VP890_DEV_PCN_89336 = 0x05,     /**< FXO/FXS-Tracker - Wideband */

    VP890_LAST_PCN,
    VP890_PCN_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} Vp890PcnType;

/*******************************************************************************
 *              890 specific API function prototypes
 ******************************************************************************/
/* VP-API Config Functions */
VpStatusType
VpMakeVp890DeviceObject(
    VpDevCtxType    *pDevCtx,
    Vp890DeviceObjectType *pDevObj);


VpStatusType
VpMakeVp890DeviceCtx(
    VpDevCtxType *pDevCtx,
    Vp890DeviceObjectType *pDevObj);

VpStatusType
Vp890MakeLineObject(
    VpTermType termType,
    uint8 channelId,
    VpLineCtxType *pLineCtx,
    void *pLineObj,
    VpDevCtxType *pDevCtx);

/* Initialization functions */
EXTERN VpStatusType
Vp890InitDevice(
    VpDevCtxType        *pDevCtx,
    VpProfilePtrType    pDevProfile,
    VpProfilePtrType    pAcProfile,
    VpProfilePtrType    pDcProfile,
    VpProfilePtrType    pRingProfile,
    VpProfilePtrType    pFxoAcProfile,
    VpProfilePtrType    pFxoCfgProfile);

EXTERN VpStatusType
Vp890InitLine(
    VpLineCtxType       *pLineCtx,
    VpProfilePtrType    pAcProfile,
    VpProfilePtrType    pDcOrFxoProfile,
    VpProfilePtrType    pRingProfile);

EXTERN VpStatusType
Vp890ConfigLine(
    VpLineCtxType       *pLineCtx,
    VpProfilePtrType    pAcProfile,
    VpProfilePtrType    pDcProfile,
    VpProfilePtrType    pRingProfile);

EXTERN VpStatusType
Vp890CalLine(
    VpLineCtxType       *pLineCtx);

EXTERN VpStatusType
Vp890CalLineInt(
    VpLineCtxType       *pLineCtx);

EXTERN VpStatusType
Vp890Cal(
    VpLineCtxType       *pLineCtx,
    VpCalType           calType,
    void                *inputArgs);

#ifdef VP890_FXO_SUPPORT
EXTERN VpStatusType
Vp890CalBFilterInt(
    VpLineCtxType       *pLineCtx);

EXTERN VpStatusType
Vp890CalMeasureBFilterInt(
    VpLineCtxType       *pLineCtx);
#endif

#ifdef VP890_FXS_SUPPORT
EXTERN bool
Vp890AdjustIla(
    VpLineCtxType *pLineCtx,
    uint8 targetIla);

EXTERN void
Vp890BatteryAdjust(
    VpLineCtxType *pLineCtx);

EXTERN bool
Vp890AdjustVoc(
    VpLineCtxType *pLineCtx,
    uint8 targetVoc,
    bool previousCal);
#endif

#if defined (VP890_FXS_SUPPORT) && defined (VP_CSLAC_SEQ_EN)
EXTERN VpStatusType
Vp890InitRing(
    VpLineCtxType       *pLineCtx,
    VpProfilePtrType    pCadProfile,
    VpProfilePtrType    pCidProfile);

EXTERN VpStatusType
Vp890InitCid(
    VpLineCtxType       *pLineCtx,
    uint8               length,
    uint8p              pCidData);

EXTERN VpStatusType
Vp890SendCid(
    VpLineCtxType       *pLineCtx,
    uint8               length,
    VpProfilePtrType    pCidProfile,
    uint8p              pCidData);

EXTERN VpStatusType
Vp890ContinueCid(
    VpLineCtxType       *pLineCtx,
    uint8               length,
    uint8p              pCidData);
#endif

EXTERN VpStatusType
Vp890InitProf(
    VpDevCtxType        *pDevCtx,
    VpProfileType       type,
    VpProfilePtrType    pProfileIndex,
    VpProfilePtrType    pProfile);

/* VP-API Control Functions */
EXTERN VpStatusType
Vp890ApiTick(
    VpDevCtxType        *pDevCtx,
    bool                *pEventStatus);

#if defined (VP890_FXS_SUPPORT)
EXTERN VpStatusType
Vp890FreeRun(
    VpDevCtxType *pDevCtx,
    VpFreeRunModeType freeRunMode);

EXTERN void
Vp890ServiceFxsInterrupts(
    VpLineCtxType *pLineCtx);

EXTERN bool
Vp890IsSupportedFxsState(
    VpLineStateType state);

EXTERN void
Vp890ProcessFxsLine(
    Vp890DeviceObjectType *pDevObj,
    VpLineCtxType *pLineCtx);

EXTERN void
Vp890ServiceFxsTimers(
    VpDevCtxType *pDevCtx,
    VpLineCtxType *pLineCtx,
    Vp890LineObjectType *pLineObj,
    VpDeviceIdType deviceId,
    uint8 ecVal);

EXTERN VpStatusType
Vp890SetRelayState(
    VpLineCtxType       *pLineCtx,
    VpRelayControlType  rState);

EXTERN VpStatusType
Vp890GetRelayState(
    VpLineCtxType *pLineCtx,
    VpRelayControlType *pRstate);
#endif

EXTERN VpStatusType
Vp890VirtualISR(
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
Vp890SetLineState(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

EXTERN uint8
LineStateMap(
    VpLineStateType state);

EXTERN VpStatusType
Vp890SetLineTone(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,
    VpProfilePtrType pCadProfile,
    VpDtmfToneGenType *pDtmfControl);

EXTERN VpStatusType
Vp890SetRelGain(
    VpLineCtxType       *pLineCtx,
    uint16              txLevel,
    uint16              rxLevel,
    uint16              handle);

EXTERN VpStatusType
Vp890SendSignal(
    VpLineCtxType       *pLineCtx,
    VpSendSignalType    signalType,
    void                *pSignalData);

EXTERN VpStatusType
Vp890SetOption(
    VpLineCtxType       *pLineCtx,
    VpDevCtxType        *pDevCtx,
    VpOptionIdType      option,
    void                *pValue);

EXTERN VpStatusType
Vp890DeviceIoAccess(
    VpDevCtxType        *pDevCtx,
    VpDeviceIoAccessDataType *pDeviceIoData);

EXTERN VpStatusType
Vp890LowLevelCmd(
    VpLineCtxType       *pLineCtx,
    uint8p              cmd,
    uint8               len,
    uint16              handle);

EXTERN VpStatusType
Vp890SetCodec(
    VpLineCtxType           *pLineCtx,
    VpOptionCodecType       codec);

/* VP-API Status and Query Functions */
EXTERN bool
Vp890GetEvent(
    VpDevCtxType        *pDevCtx,
    VpEventType         *pEvent);

EXTERN VpStatusType
Vp890GetDeviceStatus(
    VpDevCtxType        *pDevCtx,
    VpInputType         input,
    uint32              *pDeviceStatus);

EXTERN VpStatusType
Vp890GetOption(
    VpLineCtxType       *pLineCtx,
    VpDevCtxType        *pDevCtx,
    VpOptionIdType      option,
    uint16              handle);

EXTERN VpStatusType
Vp890FlushEvents(
    VpDevCtxType        *pDevCtx);

EXTERN VpStatusType
Vp890GetResults(
    VpEventType         *pEvent,
    void                *pResults);

#if (VP_CC_DEBUG_SELECT & VP_DBG_ERROR)
EXTERN VpStatusType
Vp890RegisterDump(
    VpDevCtxType *pDevCtx);
#endif

#ifdef VP890_INCLUDE_TESTLINE_CODE
/* VP-API Line Tests Structs */
EXTERN VpStatusType
Vp890TestLine(
    VpLineCtxType       *pLineCtx,
    VpTestIdType        test,
    const void          *pArgs,
    uint16              handle);

EXTERN VpStatusType
Vp890TestLineCallback(
    VpLineCtxType *pLineCtx,
    VpPcmOperationResultsType *pResults);
#endif

/**************V*****V*****V*****V*****V*****V*****V*****V*****V****************
 * The following prototypes are for functions that are not externally avaiable
 * but are called by multiple functions internally.
 ***************V*****V*****V*****V*****V*****V*****V*****V*****V***************/
bool
Vp890IsDevReady(
    VpDeviceStateType state,
    bool checkCal);

bool
Vp890IsChnlUndrTst(
    Vp890DeviceObjectType *pDevObj,
    uint8 channelId);

bool
Vp890IsProfileValid(
    VpProfileType       profType,
    int16               tableSize,
    uint16              profEntry,
    VpProfilePtrType    *profTable,
    VpProfilePtrType    pProfileInput,
    VpProfilePtrType    *pProfileRslt);

#ifdef VP890_FXS_SUPPORT
#ifdef VP_CSLAC_SEQ_EN
void
Vp890LoadCidBuffers(
    uint8               length,
    VpCallerIdType      *pCid,
    uint8p              pCidData);
#endif

/* Called by Sequencer and by Vp890SetLineState */
VpStatusType
Vp890SetFxsLineState(
    VpLineCtxType       *pLineCtx,
    VpLineStateType     state);
#endif

/* Called by Sequencer and by Vp890SetLineState */
#ifdef VP890_FXO_SUPPORT
VpStatusType
Vp890SetFxoLineState(
    VpLineCtxType       *pLineCtx,
    VpLineStateType     state);

void
Vp890ProcessFxoLine(
    Vp890DeviceObjectType   *pDevObj,
    VpLineCtxType           *pLineCtx);

void
Vp890ServiceFxoTimers(
    VpDevCtxType            *pDevCtx,
    VpLineCtxType           *pLineCtx,
    Vp890LineObjectType     *pLineObj,
    VpDeviceIdType          deviceId,
    uint8                   ecVal);

bool
Vp890IsSupportedFxoState(
    VpLineStateType state);
#endif

/* Called by Vp890SetFxsLineState and Vp890CommandInstruction */
void
Vp890LLSetSysState(
    VpDeviceIdType      deviceId,
    VpLineCtxType       *pLineCtx,
    uint8               lineState,
    bool                writeToDevice);

void
Vp890MuteChannel(
    VpLineCtxType       *pLineCtx,
    bool                mode);

VpStatusType
Vp890GetTxRxPcmMode(
    Vp890LineObjectType     *pLineObj,
    VpLineStateType         state,
    uint8                   *mpiByte);

#ifdef VP890_LP_SUPPORT
void
Vp890LowPowerMode(
    VpDevCtxType *pDevCtx);

uint16
Vp890SetDiscTimers(
    Vp890DeviceObjectType *pDevObj);

void
Vp890RunLPDisc(
    VpLineCtxType *pLineCtx,
    bool discMode,
    uint8 nextSlicByte);

void
Vp890SetLPRegisters(
    Vp890DeviceObjectType *pDevObj,
    Vp890LineObjectType *pLineObj,
    bool lpModeTo);

void
Vp890WriteLPExitRegisters(
    VpLineCtxType *pLineOtx,
    VpDeviceIdType deviceId,
    uint8 ecVal,
    uint8 *lineState);
#endif

VpStatusType
Vp890SetRelGainInt(
    VpLineCtxType *pLineCtx);

bool
Vp890ProtectedWriteICR1(
    Vp890LineObjectType *pLineObj,
    VpDeviceIdType deviceId,
    uint8 *icr1Values,
    bool writeToDevice);

#ifdef VP_CSLAC_SEQ_EN
/* VP-API CSLAC SEQ HELPERS */
VpStatusType
Vp890CommandInstruction(
    VpLineCtxType       *pLineCtx,
    VpProfilePtrType    pSeqData);

bool
Vp890FSKGeneratorReady(
    VpLineCtxType       *pLineCtx);

bool
Vp890CliGetEncodedByte(
    VpLineCtxType       *pLineCtx,
    uint8               *pByte);

VpStatusType
Vp890CtrlSetCliTone(
    VpLineCtxType       *pLineCtx,
    bool                mode);

void
Vp890CtrlSetFSKGen(
    VpLineCtxType       *pLineCtx,
    VpCidGeneratorControlType mode,
    uint8 data);
#endif

VpStatusType
Vp890SetDTMFGenerators(
    VpLineCtxType       *pLineCtx,
    VpCidGeneratorControlType mode,
    VpDigitType digit);

#ifdef VP890_INCLUDE_TESTLINE_CODE
/* Testing functions */
VpStatusType
Vp890TestLine(
    VpLineCtxType *pLineCtx,
    VpTestIdType test,
    const void *pArgsUntyped,
    uint16 handle);

VpStatusType
Vp890TestLineInt(
    VpLineCtxType *pLineCtx,
    VpTestIdType test,
    const void *pArgsUntyped,
    uint16 handle,
    bool callback);
#endif

/* Functions required for Calibration */
VpStatusType
Vp890CalLineInt(
    VpLineCtxType *pLineCtx);

#endif  /* Vp890_API_INT_H */

