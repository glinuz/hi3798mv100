/*******************************************************************************
 *
 * FILE NAME          : MxL68x_PhyCfg.h
 * 
 * AUTHOR             : Brenndon Lee
 *                      Dong Liu
 *                      Jay Hung
 *  
 * DATE CREATED       : 05/18/2009
 *                      05/08/2011
 *                      03/09/2015
 *
 * DESCRIPTION        : This file contains MxL68x common control register 
 *                      definitions
 *
 *******************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MXL68X_PHY_CFG_H__
#define __MXL68X_PHY_CFG_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/
#include "MaxLinearDataTypes.h"
#include "MxL68x_PhyCtrlApi.h"
#include "MxL68x_PhySpurTable.h"
#include "MxL68x_OEM_Drv.h"
/******************************************************************************
    Macros
******************************************************************************/

#define GINJO_FIRST_PAGE_BOUNDARY         0x0100 
#define GINJO_DFE_REGISTER_BOUNDARY       0x1000
#define GINJO_DFE_REG_WIDTH               8      // Ginjo DFE register address width is 8 bit   
#define GINJO_DFE_REG_ADDR_MASK           0x00FF // Ginjo DFE register address width is 8 bit 
#define GINJO_DFE_REG_PAGE_MASK           0x0300 // Bit8 and bit9 of register address is page number from bit8 position

#define DFE_PAGE_CTRL_REG                 0x0000

#define DEVICE_BASE_I2C_ADDRESS           96
#define MXL68X_MAX_DEVICES                6 

#define MXL681_DEVICE_ID                  0x03 
#define MXL683_DEVICE_ID                  0x02 

#define MAX_READ_TRY                      10

// Mailbox registers
#define MAILBOX_REG_BASE_ADDR             0xA800
#define MAILBOX_REG_NUM_PENDING_HOST_CMDS 0xA800
#define MAILBOX_REG_FIRMWARE_RC_VER       0xA802

#define COMMAND_HEADER_LENGTH             4
#define MAX_COMMAND_DATA_SIZE             252 
#define MAX_BLOCK_WRITE_LENGTH            256  // Depends on I2C hardware limitation   
#define HOST_COMMAND_SYNC_IND             0xE0

// Following macro is used to populate Host command header that goes to MxL68x along with payload */
#define FORM_CMD_HEADER(Header, id, seqNum, len)  Header.syncChar = HOST_COMMAND_SYNC_IND + (seqNum % 0x0F); \
                                                  Header.commandId = id;  \
                                                  Header.payloadCheckSum = 0;\
                                                  Header.payloadLen = (UINT8)len

#define RF_BW_6MHz                        6 
#define RF_BW_7MHz                        7 
#define RF_BW_8MHz                        8 

#define MxL_BER_STAT_TIME_INTERVAL        100   // 100ms time interval 

#define ISDBT_DATA_CARRIES_Seg            96    // Data carries bit number for 1 Seg 
#define ISDBT_FRAME_BIT_COUNT             204   // constent data, bit count in one frame 
#define ISDBT_TSP_FRAME                   8     // div factor to calculate TSP   

#define ISDBT_AC_DATA_LENGTH              24    // Total is 192 bits 
#define FW_CHAN_TUNE_DATA_LENGTH          8  
#define MAX_FW_RETURN_DATA_LENGTH         24

#define TUNE_DECREASE                     0x1
#define TUNE_INCREASE                     0x2

#define SPUR_SHIFT_CLOCK_ADJUST_MIN       205 
#define SPUR_SHIFT_CLOCK_ADJUST_MAX       227 

#define SEQ_STATE_RB_POLLING_TIMES        16

#define DFE_SEQ_STATE_VALUE               0x0A
#define DFE_SEQ_JUMP_VALUE                0x18

#define SPUR_SHIFT_FREQ_WINDOW            500000  // +- 0.5MHz

/* MxL68x Device/Tuner Registers */

#define POWER_SUPPLY_REG                    0x000E
#define XTAL_CFG_REG                        0x0001
#define XTAL_SHARING_REG                    0x0002
#define XTAL_CALI_SET_REG                   0x0003 // Xtal calibration enable register enable register 
#define DFE_XTAL_EXT_BIAS_REG               0x006D
#define DFE_DIG_ANA_XTAL_TH                 0x006E

#define DFE_CTRL_ACCUM_LOW_REG              0x0023 // Bit<7:0> 
#define DFE_CTRL_ACCUM_MID_REG              0x0024 // Bit<7:0>
#define DFE_CTRL_ACCUM_HI_REG               0x0025 // Bit<1:0>

#define DFE_REFSX_INTMOD_REG                0x004F
#define DFE_REFSX_INTMOD_BYP_REG            0x0050

#define TUNER_ENABLE_REG                    0x000B // Power up register, bit<0>
#define DFE_SEQ_BREAK_REG                   0x014B
#define START_TUNE_REG                      0x0012 // sequencer setting register 

#define DFE_SPUR_SHIFT_CTRL_REG             0x00F0 
#define DFE_SPUR_SHIFT_ADJ_REG              0x00F1 

#define DFE_SEQ_STATE_RB_REG                0x01D5

#define DFE_AGC_AUTO_REG                    0xB6
#define DFE_SEQ_TUNE_RF1_BO_REG             0x015F
#define DFE_SEQ_CDC_RF1_BO_REG              0x0160
#define DFE_GCOMP_IQ_SWITCH                 0x0086

#define IF_FREQ_SEL_REG                     0x0004 // IF frequency selection and manual set bypass register  
#define DIG_ANA_IF_CFG_0                    0x005A
#define DIG_ANA_IF_CFG_1                    0x005B
#define DIG_INT_AAF_EN_REG                  0x0059
#define DIG_ANA_IF_PWR                      0x005C
#define DFE_CSF_SS_SEL                      0x00EA
#define DFE_DACIF_GAIN                      0x00DC
#define DFE_LT_CONFIG_REG                   0x0196 

#define AGC_CONFIG_REG                      0x0008 // AGC configuration, include AGC select and AGC type  
#define AGC_SET_POINT_REG                   0x0009
#define AGC_FLIP_REG                        0x005E

#define IF_FREQ_SEL_REG                     0x0004 // IF frequency selection and manual set bypass register  
#define IF_PATH_GAIN_REG                    0x0005 // IF gain level and path selection register  
#define IF_FCW_LOW_REG                      0x0006 // Low register of IF FCW set when manual program IF frequency   
#define IF_FCW_HIGH_REG                     0x0007 // High register of IF FCW set when manual program IF frequency 

#define FINE_TUNE_SET_REG                   0x0013 // Fine tune operation register 
#define FINE_TUNE_CTRL_REG_0                0x0013 // Fine tune operation register 
#define FINE_TUNE_CTRL_REG_1                0x0014 // Fine tune operation register 

#define FINE_TUNE_OFFSET_LOW_REG            0x0014 // Fine tune frequency offset low byte 
#define FINE_TUNE_OFFSET_HIGH_REG           0x0015 // Fine tune frequency offset high byte

#define AGC_SAGCLOCK_STATUS_REG             0x002C 
#define RF_TOTAL_GAIN_RB_REG                0x0033 

#define RFPIN_RB_LOW_REG                    0x001D // RF power low 8 bit register 
#define RFPIN_RB_HIGH_REG                   0x001E // RF power high 8 bit register 

#define SIGNAL_TYPE_REG                     0x001E // Signal type

#define RF_REF_STATUS_REG                   0x002B // RF/REF lock status register 

#define DMD_RS_BER_PER_FRAME_MAILBOX_REG    0xA884
#define DMD_RS_BER_PER_FRAME                0x101C
#define DMD_RS_BER_PER_START                0x101B
#define DMD_RS_BER_PER_CLR                  0x101B
#define DMD_FEC_OUT_CTRL                    0x101D

#define DFE_RFLUT_SWP1_REG                  0x0049 
#define DIG_ANA_RFRSSI_REG                  0x0057 
#define DFE_RFLUT_BYP_REG                   0x00DB  
#define DFE_RFLUT_DIV_MOD_REG               0x00DB  
#define DFE_REFLUT_BYP_REG                  0x00EA  
#define DFE_REFSX_INT_MOD_REG               0x00EB  

#define DFE_SEQ_JUMP_REG                    0x014A
#define DFE_SEQ_BREAK_REG                   0x014B

#define ANA_DIG_RCTUNE_REG                  0x01A9 
#define DFE_SEQ_DIGANA_AGC_RCTUNE_REG       0x0153
#define DFE_SEQ_DIGANA_ADC_RCTUNE_REG       0x0154
#define DFE_SEQ_DIGANA_DNX_TC_RCTUNE_REG    0x0155

#define ANA_DIG_REFSX_XTAL_AMP_RB_REG       0x01A2
#define ANA_DIG_REFSX_XTAL_AMP_LIN_RB_REG   0x01A1
#define DIG_ANA_XTAL_XTAL_EXT_BIAS          0x006D

#define TUNE_MODE_REG                       0x000D
#define CHAN_TUNE_BW_REG                    0x000F // Band width register 
#define CHAN_TUNE_LOW_REG                   0x0010 // Tune frequency set low byte 
#define CHAN_TUNE_HI_REG                    0x0011 // Tune frequency set high byte 

#define FIRMWARE_DOWNLOAD_DONE_PERCENT      100 
#define FIRMWARE_DOWNLOAD_FAIL_PERCENT      0 

#define MAX_TRY_TIMES_FOR_RC_TUNE           10 
/* MxL68x Demod Register List */

#define DMD_CCTL_RSTRQ_N_REG                      0x3003 // SW reset from Host, bit[24], 1 is reset   
#define DMD_CCTL_DEMRQ_REG                        0x301F // Demod request, set it start demod, bit 0, 1 is enable. 
#define DMD_GPIO_FUNC_MODE_REG                    0x300B   

#define DMD_CHAN_SCAN_TUNE_XTAL_REG               0x3028 
#define DMD_CHAN_SCAN_TUNE_FREQ_REG               0x3029 

#define DFE_FW_REG_BUFFER_MSB                     0x302C
#define DFE_FW_REG_BUFFER_LSB                     0x302D

#define DFE_TT_SPUR_ENABLE                        0xA8B8

#define FIRMWARE_LOADED_CHECK_REG                 0x302B  // Bit<0>   
#define DMD_ID_VERSION_REG                        0x3001  // Chip Id & Chip Ver

//Customer Settings register
#define CUST_SET_REG                              0x302F
#define CUST_SET_FRM_SYNC_IMPRV                   0x0001
#define CUST_SET_NOISE_FLOOR_RAISE                0x0002
#define CUST_SET_DISABLE_SPUR                     0x0004

#define MPEG_OUT_CFG_MAILBOX_REG                  0xA8B6
#define MPEG_OUT_CFG_REG                          0x1122
#define MPEG_CLK_CFG_REG                          0x11AD
#define MPEG_CLK_PHASE_REG                        0x300D
#define MPEG_TS_DRV_STR_REG_1                     0x3007
#define MPEG_TS_DRV_STR_REG_2                     0x3009

#define DMD_TMCC_INFO_REG_0                       0x1126
#define DMD_TMCC_INFO_REG_1                       0x1127
#define DMD_TMCC_INFO_REG_2                       0x1128
#define DMD_TMCC_INFO_REG_3                       0x1129
#define DMD_STATUS_INFO_REG                       0x1148

#define DMD_TMCC_MGI_REG                          0x1125
#define DMD_TMCC_LOCKED_REG                       0x1125
#define DMD_TMCC_SEG_TYPE_REG                     0x1125
#define DMD_TMCC_SYS_IDENT_REG                    0x1126
#define DMD_TMCC_ALARM_REG                        0x1126
#define DMD_TMCC_MOD_A_REG                        0x1126
#define DMD_TMCC_MOD_B_REG                        0x1126
#define DMD_TMCC_MOD_C_REG                        0x1126
#define DMD_TMCC_CONV_A_REG                       0x1127
#define DMD_TMCC_CONV_B_REG                       0x1127
#define DMD_TMCC_CONV_C_REG                       0x1127
#define DMD_TMCC_INTV_A_REG                       0x1127
#define DMD_TMCC_INTV_B_REG                       0x1128
#define DMD_TMCC_INTV_C_REG                       0x1128
#define DMD_TMCC_SEG_A_REG                        0x1128
#define DMD_TMCC_SEG_B_REG                        0x1128
#define DMD_TMCC_SEG_C_REG                        0x1129
#define DMD_TMCC_PARTIAL_REG                      0x1129
#define DMD_CTRL_MODE_GI                          0x1148
#define DMD_CNR_DATA_ALL                          0x112D
#define DMD_AGC_INTGOUT_RB                        0x1190
#define DMD_RS_LOCK_A_B                           0x1190
#define DMD_RS_LOCK_B                             0x1190
#define DMD_RS_LOCK_C                             0x1191
#define DMD_STATUS                                0x1147
#define DMD_RS_BER_PER_END_FLG                    0x112E
#define DMD_RS_BER_A_DATA                         0x1130
#define DMD_RS_BER_B_DATA                         0x1132
#define DMD_RS_BER_C_DATA                         0x1134
#define DMD_RS_PER_A_DATA                         0x1136
#define DMD_RS_PER_B_DATA                         0x1138
#define DMD_RS_PER_C_DATA                         0x113A
#define DMD_RS_BER_ITER_A_DATA                    0x113C
#define DMD_RS_BER_ITER_B_DATA                    0x113E
#define DMD_RS_BER_ITER_C_DATA                    0x1140
#define DMD_RS_PER_ITER_A_DATA                    0x1142
#define DMD_RS_PER_ITER_B_DATA                    0x1144
#define DMD_RS_PER_ITER_C_DATA                    0x1146

#define DMD_RS_BER_PER_DATA_LO_MAILBOX            0xA8B4
#define DMD_RS_BER_PER_DATA_HI_MAILBOX            0xA8C6
#define DMD_RS_BER_PER_ITER_DATA_LO_MAILBOX       0xA8C8
#define DMD_RS_BER_PER_ITER_DATA_HI_MAILBOX       0xA8CA

//Mailbox regs used for CNR RB are same as ones used for BER PER RB
#define DMD_CNR_RB_LO_MAILBOX       0xA8C8
#define DMD_CNR_RB_HI_MAILBOX       0xA8CA

#define DMD_CARRIER_FREQ_OFF_LO_MAILBOX           0xA8C0
#define DMD_CARRIER_FREQ_OFF_HI_MAILBOX           0xA8BE
#define DMD_SAMP_FREQ_OFF_LO_MAILBOX              0xA8C4
#define DMD_SAMP_FREQ_OFF_HI_MAILBOX              0xA8C2

#define DMD_CNST_CTRL                             0x101C

#define MXL68X_FW_MAIL_BOX_REG            0xA85C 
#define DMD_INTERRUPT_MAILBOX_REG         0xA80A // interrupt mailbox register 

#define DMD_INT_MASK_AC_DATA              0x0001 // Bit<0>
#define DMD_INT_MASK_FW_CHAN_TUNE         0x0002 // Bit<1>

#define LAYER_A_MODULATION_MASK           0x0070
#define LAYER_A_MODULATION_BIT_SHIFT      4

#define LAYER_B_MODULATION_MASK           0x0700
#define LAYER_B_MODULATION_BIT_SHIFT      8

#define LAYER_C_MODULATION_MASK           0x7000
#define LAYER_C_MODULATION_BIT_SHIFT      12

#define LAYER_A_CR_MASK                   0x0007
#define LAYER_A_CR_BIT_SHIFT              0

#define LAYER_B_CR_MASK                   0x0070
#define LAYER_B_CR_BIT_SHIFT              4

#define LAYER_C_CR_MASK                   0x0700
#define LAYER_C_CR_BIT_SHIFT              8

#define LAYER_A_IL_MASK                   0x7000
#define LAYER_A_IL_BIT_SHIFT              12

#define LAYER_B_IL_MASK                   0x0007
#define LAYER_B_IL_BIT_SHIFT              0

#define LAYER_C_IL_MASK                   0x0070
#define LAYER_C_IL_BIT_SHIFT              4

#define LAYER_A_SEG_MASK                   0x0F00
#define LAYER_A_SEG_BIT_SHIFT              8

#define LAYER_B_SEG_MASK                   0xF000
#define LAYER_B_SEG_BIT_SHIFT              12

#define LAYER_C_SEG_MASK                   0x000F
#define LAYER_C_SEG_BIT_SHIFT              0

typedef struct
{
  UINT16 modulationMask;
  UINT16 codeRateMask;
  UINT16 interleaverMask;
  UINT16 segmentMask;
  UINT8  modulationShift;
  UINT8  codeRateShift;
  UINT8  interleaverShift;
  UINT8  segmentShift;
} MXL68x_TMCC_REG_MASK;

/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/

typedef enum
{
  MXL68x_DEV_ADDR1 = DEVICE_BASE_I2C_ADDRESS,
  MXL68x_DEV_ADDR2,
  MXL68x_DEV_ADDR3,
  MXL68x_DEV_ADDR4
} MXL68x_I2C_ADDRESS_E; // Permitted I2C address 

typedef enum
{
  MXL68x_INT_SOURCE1 = 0,  
  MXL68x_INT_SOURCE2 = 1,
  MXL68x_INT_SOURCE3 = 2,
  MXL68x_INT_SOURCE4 = 3,
  MXL68x_INT_NONE = 0xFF 
} MXL68x_INT_SOURCE_E;

typedef struct
{
  UINT16 regAddr;
  UINT16 mask;
  UINT16 data;
} REG_CTRL_INFO_T, *PREG_CTRL_INFO_T;

typedef enum
{
  MXL68x_FW_STATUS_UNKNOWN = 0,
  MXL68x_FW_STATUS_BOOTLOADER,
  MXL68x_FW_STATUS_LOADED,
  MXL68x_FW_STATUS_CLKGATED,
} MXL68x_FW_STATUS_E;

typedef enum
{
  MXL68x_SPIN_UNTIL_EQ  = 0,
  MXL68x_SPIN_UNTIL_NEQ = 1,
} MXL68X_SPIN_CONDITION_E;

/* I2C COMMAND Communication: Command Code ID */
typedef enum
{
  HOST_CMD_SET_ADDR_CFG = 1,
  HOST_CMD_DOWNLOAD_SEGMENT_CFG,
  HOST_CMD_DOWNLOAD_DONE_CFG,

  HOST_CMD_RC_TUNE_CFG = 5,

  HOST_CMD_DEV_DEVICE_RESET_CFG = 10,
  HOST_CMD_DEV_XTAL_SETTINGS_CFG = 11,

// Perform Overwrite default in the Initialization procedure as shown in programming guide ¨C No need to use this as of now, done automatically by FW on start-up
  HOST_CMD_DEV_OVERWRITE_DEFAULT_CFG    = 13,

//FW Version info 
  HOST_CMD_DEV_VERSION_INFO_REQ         = 14,
//Power Mode to standby
  HOST_CMD_DEV_POWER_MODE_STANDBY_CFG   = 15,
//Wake up from Standby or EWS
  HOST_CMD_DEV_POWER_MODE_ON_CFG        = 16,
//Go to Auto EWS Mode
  HOST_CMD_DEV_POWER_MODE_AUTO_EWS_CFG  = 17,
// Read from multi-byte DFE register
  HOST_CMD_DEV_DFE_REG_READ             = 19,
// Write to multi-byte DFE register
  HOST_CMD_DEV_DFE_REG_WRITE            = 20,  

//Set the Int to Host (nINT Pin to 0)    
  HOST_CMD_DEMOD_INTR_CLEAR_CFG         = 24,
//Start streaming of AC Data through GPO pins (used instead of getting AC Data through Block Read command)
  HOST_CMD_DEMOD_ACDATA_GPO_CFG         = 27,  
//Retrieve AC Data through Block read
  HOST_CMD_DEMOD_AC_DATA_REQ            = 39,

// BER PER RB Host commands
  HOST_CMD_DEMOD_LAYER_A_BER_RB           = 40,
  HOST_CMD_DEMOD_LAYER_B_BER_RB           = 41,
  HOST_CMD_DEMOD_LAYER_C_BER_RB           = 42,    
  HOST_CMD_DEMOD_LAYER_A_PER_RB           = 43,
  HOST_CMD_DEMOD_LAYER_B_PER_RB           = 44,
  HOST_CMD_DEMOD_LAYER_C_PER_RB           = 45,
  HOST_CMD_DEMOD_CNR_RB                   = 46,
   

// Channel Tune (for digital ISDBT Channels in ISDB-T SoC Mode only)
  HOST_CMD_TUNER_CHAN_TUNE_CFG          = 51,
//App Mode dependent settings 
  HOST_CMD_TUNER_APP_DEP_SETTING        = 52,
// Reset DFE registers to Default
  HOST_CMD_TUNER_DFE_RESET_CFG          = 53,
// RC Tune procedure according to Prog Guide in the initialization procedures ¨C No need to use this as of now, done automatically by FW on start-up
  HOST_CMD_TUNER_INIT_RC_TUNE_CFG       = 54,
// Block read command for Tuner result (channel info like TMCC and such)
  HOST_CMD_TUNER_TUNE_RESULT_REQ        = 62,
// Enable Teletext Spur Mitigation algo in FW (should be enabled for analog mode)
  HOST_CMD_TUNER_TT_SPUR_MIT_EN_CFG     = 63,

  // MAX_NUM_HOST_COMMAND
} MXL_CMD_ID_E;

/* I2C COMMAND Communication: Response Code ID */
typedef enum
{
  RESPONSE_ID_SUCCESS = 0,
  RESPONSE_ID_NOT_READY,
  RESPONSE_ID_NOT_INTEGRATE,
  MAX_NUM_RESPONSE_ID
} MXL_RSP_ID_E;

/* Host command data structure */
typedef struct
{
  UINT8 syncChar;                    /* 0x00: Bit[7-4]:Packet sync character (0x0E),  bit[3-0]: serial */
  MXL_CMD_ID_E commandId;            /* 0x01: command opcode = 0x02 for firmware download */
  UINT8 payloadCheckSum;             /* 0x02: Checksum of packet payload */
  UINT8 payloadLen;                  /* 0x03: Length of packet payload: 1 - 252 */
  UINT8 data[MAX_COMMAND_DATA_SIZE]; /* 0x04-0x0xFF: command field data */
} HOST_COMMAND_T, *PHOST_COMMAND_T;

/* Firmware response data structure */
typedef struct
{
  UINT8 syncChar;                    /* 0x00: Bit[7-4]:Packet sync character (0x0E),  bit[3-0]: serial */
  MXL_RSP_ID_E errorCode;            /* 0x01: Error code, 0x00 means ok. */
  UINT8 payloadCheckSum;             /* 0x02: Checksum of packet payload */
  UINT8 payloadLen;                  /* 0x03: Length of packet payload: 1 - 252 */
  UINT8 data[MAX_COMMAND_DATA_SIZE]; /* 0x04-0xFF: response field data */
} FW_RESPONSE_T, *PFW_RESPONSE_T;

typedef struct
{
  MXL_BOOL_E ValidFlag;                  /* OUT, indicate if the data read back from FW is validate or not */
  UINT16 DataLength;                     /* OUT, read back data length */
  UINT8 Data[MAX_FW_RETURN_DATA_LENGTH]; /* OUT, store the AC data that read from firmware */  
} MXL68x_FW_INT_RET_DATA_T, *PMXL68x_FW_INT_RET_DATA_T;

typedef enum
{
  MXL68x_VER_ES1 = 0,
  MXL68x_VER_NA,
} MXL68x_DEV_VERSION;

typedef enum
{
  MXL68x_DRV_UNINITIALISED = 0,
  MXL68x_DRV_INITIALISED,
} MXL68x_DRV_STATUS;

typedef struct
{
  UINT8                     i2cAddress;     // I2C address corresponding to this device 
  MXL68x_INT_SOURCE_E       intSrcId;       // Interrupt source ID  
  MXL68x_DRV_STATUS         driverStatus;   // Driver status 
  MXL68x_FW_STATUS_E        firmwareStatus; // Firmware status, unload or loaded. 
  MXL68x_FW_INT_RET_DATA_T  fwIntRetData;   // Firmware interrupt return data structure 
  MXL68x_DEV_VERSION        devVersion;     // Device version 
  MXL68x_XTAL_FREQ_E        xtalFreqInMHz;  // Device Xtal frequency selection 
  UINT32                    ifOutFreqInKHz; // IF out frequency in KHz unit 
  UINT8                     ifGainLevel;    // IF gain level 
  MXL_BOOL_E                pllClkReady;    // PLL clock readay status 
  MXL68x_DEV_MODE_E         devOpMode;      // Device operation mode, either tuner + demod or tuner - only 
  MXL68x_SIGNAL_MODE_E      signalType;     // current device application mode
  MXL68x_DEVICE_TYPE_E      mxl68xType;     // Indicate device is MxL681 or MxL683  
  MXL_BOOL_E                acDataGpoOut;   // Indicate if the AC data shall be stream out to GPO 
  MXL_BOOL_E                rfLoopThrough;  
  SINT16                    fwDownloadPercent; // Indicate firmware download percent. 
} MXL68x_DEV_CONTEXT_T, *PMXL68x_DEV_CONTEXT_T;

/******************************************************************************
    Global Variable Declarations
******************************************************************************/

extern REG_CTRL_INFO_T MxL68x_AnalogNtsc[];
extern REG_CTRL_INFO_T MxL68x_AnalogPal[];
extern REG_CTRL_INFO_T MxL68x_AnalogSecam[];
extern REG_CTRL_INFO_T MxL68x_DigitalDvbc[];
extern REG_CTRL_INFO_T MxL68x_DigitalIsdbtAtsc[];
extern REG_CTRL_INFO_T MxL68x_DigitalDvbt[];

extern REG_CTRL_INFO_T MxL68x_Ntsc_RfLutSwpHIF[];
extern REG_CTRL_INFO_T MxL68x_Ntsc_16MHzRfLutSwpLIF[];
extern REG_CTRL_INFO_T MxL68x_Ntsc_24MHzRfLutSwpLIF[];

extern REG_CTRL_INFO_T MxL68x_Pal_RfLutSwpLIF[];
extern REG_CTRL_INFO_T MxL68x_PalD_RfLutSwpHIF[];
extern REG_CTRL_INFO_T MxL68x_PalI_RfLutSwpHIF[];
extern REG_CTRL_INFO_T MxL68x_PalBG_8MHzBW_RfLutSwpHIF[];
extern REG_CTRL_INFO_T MxL68x_PalBG_7MHzBW_RfLutSwpHIF[];
extern REG_CTRL_INFO_T MxL68x_Ntsc_HRCRfLutSwpLIF[];

extern MXL68x_DEV_CONTEXT_T  MxL68x_Dev[];
/******************************************************************************
    Prototypes
******************************************************************************/

PMXL68x_DEV_CONTEXT_T MxL68x_Ctrl_GetDeviceContext(UINT8 devId);

// General functions' prototype for Host - Device communication 
MXL_STATUS_E Ctrl_SendHostCommand(UINT8 devId, PHOST_COMMAND_T commandPtr);
MXL_STATUS_E Ctrl_GetDeviceResp(UINT8 devId, UINT16 regAddr, PFW_RESPONSE_T responsePtr);
MXL_STATUS_E Ctrl_HostComm(UINT8 devId, PHOST_COMMAND_T commandPtr, PFW_RESPONSE_T responsePtr);

// Functions for register write operation 
MXL_STATUS_E MxL68x_Ctrl_ProgramRegisters(UINT8 devId, PREG_CTRL_INFO_T ctrlRegInfoPtr);
MXL_STATUS_E Ctrl_SetRfFreqLutReg(UINT8 devId, UINT32 FreqInHz, MXL68x_CHAN_DEP_FREQ_TABLE_T *freqLutPtr);

// Register access
MXL_STATUS_E MxL68x_Ctrl_WriteRegister(UINT8 devId, UINT16 regAddr, UINT16 regData);
MXL_STATUS_E MxL68x_Ctrl_ReadRegister(UINT8 devId, UINT16 regAddr, UINT16 *dataPtr);

MXL68x_FW_STATUS_E Ctrl_CheckFirmwareStatus(UINT8 devId);
MXL_STATUS_E Ctrl_CfgDevReset(UINT8 devId, MXL68x_RESET_TYPE_E resetType);

MXL_STATUS_E Ctrl_AssignNumDenCR(UINT8 *num, UINT8 *den,  UINT8 cr);

MXL_STATUS_E Ctrl_DfeRegRead(UINT8 devId, UINT16 regAddr, UINT8 numRegs, UINT32* regDataPtr );
MXL_STATUS_E Ctrl_DfeRegWrite(UINT8 devId, UINT16 regAddr, UINT8 numRegs, UINT32 regData );

// Interrupt process functions 
MXL_STATUS_E Ctrl_RegisterIntHandlerFunc(UINT8 devId);
MXL_STATUS_E Ctrl_UnRegisterIntHandlerFunc(UINT8 devId);
void Ctrl_IntHandlerForIntSource1(void);
void Ctrl_IntHandlerForIntSource2(void);
void Ctrl_IntHandlerForIntSource3(void);
void Ctrl_IntHandlerForIntSource4(void);
void Ctrl_InterruptProcess(UINT8 devId);

// Fixed point process functions 
MXL_STATUS_E Ctrl_MulDiv32Bit(UINT32 MulData1, UINT32 MulData2, UINT32 DivData, UINT32* RtnVal);
MXL_STATUS_E Ctrl_Decomp32Bit(UINT32 In, UINT16* b, SINT16* p);
MXL_STATUS_E Ctrl_SubMulDiv(UINT16 b1,UINT16 b2,UINT16 b3,SINT16 p1,SINT16 p2,SINT16 p3,UINT32 *RetVal);
UINT8 Ctrl_HighBit(UINT32 In);
#endif /* __MXL68X_PHY_CFG_H__*/




