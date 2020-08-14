/*****************************************************************************************
 *
 * FILE NAME          : MxL_HRCLS_PhyCtrl.h
 * 
 * AUTHOR             : Mariusz Murawski
 *
 * DATE CREATED       : 11/19/2011
 *
 * DESCRIPTION        : Header file for MxL_HRCLS PHY control
 *                             
 *****************************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ****************************************************************************************/

#ifndef __MXL_HRCLS_PHY_CFG_H__
#define __MXL_HRCLS_PHY_CFG_H__

/*****************************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
*****************************************************************************************/
#include "MaxLinearDataTypes.h"
#include "MxL_HRCLS_CommonApi.h"
#include "MxL_HRCLS_OEM_Drv.h"
#include "MxL_HRCLS_SOC_Registers.h"

#ifdef _MXL_DIAG_ENABLED_
#include "../MxLDiag/MxL_DIAG_Api.h"
#endif // _MXL_DIAG_ENABLED_

/*****************************************************************************************
    Macros
*****************************************************************************************/
// Host command - HOST_CMD_CFG_DEV_UPSTREAM_PARAMS
#define MXL_HRCLS_HW_TXDAC_ENABLE           1
#define MXL_HRCLS_HW_TXDAC_DISABLE          0
#define MXL_HRCLS_HW_TXDAC_EXTERNAL         2

#define MXL_HRCLS_HW_TXDAC_SWING_MIN        0
#define MXL_HRCLS_HW_TXDAC_SWING_MAX        2   
#define MXL_HRCLS_HW_TXDAC_SWING_DEFAULT    1


// Verified defines start here
#define MXL_HRCLS_HW_16QAM                  0
#define MXL_HRCLS_HW_64QAM                  1
#define MXL_HRCLS_HW_256QAM                 2
#define MXL_HRCLS_HW_1024QAM                3
#define MXL_HRCLS_HW_32QAM                  4
#define MXL_HRCLS_HW_128QAM                 5
#define MXL_HRCLS_HW_QPSK                   6

#define MXL_HRCLS_HW_ANNEX_A                1
#define MXL_HRCLS_HW_ANNEX_B                0

#define MXL_HRCLS_READ_FFE_START_ADDRESS    0x000 
#define MXL_HRCLS_READ_SPUR_START_ADDRESS   0x020
#define MXL_HRCLS_READ_DFE_START_ADDRESS    0x040
#define MXL_HRCLS_READ_EQU_FAGC_GAIN_ADDRESS 0x086
#define MXL_HRCLS_READ_CARRIER_OFFSET_ADDRESS  0x087
#define MXL_HRCLS_READ_MSE_START_ADDRESS    0x089

#define MAILBOX_REG_MSE_BASE                0xF138
#define MAILBOX_REG_FAGC_BASE               0xF14A
#define MAILBOX_REG_CARRIER_OFFSET_BASE     0xF15C

#define MXL_HRCLS_MSE_AVG_COEF              0x1B

#define MXL_HRCLS_CW_COUNT_SEL              0
#define MXL_HRCLS_CW_CORR_COUNT_SEL         1
#define MXL_HRCLS_CW_ERR_COUNT_SEL          2
#define MXL_HRCLS_CORR_BITS_SEL             3

#define MXL_HRCLS_DEFAULT_REF_PLL_FREQ_HZ   2700000000U
#define MXL_HRCLS_DEFAULT_SERDES_LINERATE   MXL_HRCLS_SERDES_LINERATE_5400MHZ

#define MXL_HRCLS_DEFAULT_SYMBOLRATE_ANNEX_A        6890000U
#define MXL_HRCLS_DEFAULT_SYMBOLRATE_ANNEX_B_QAM64  5056941U
#define MXL_HRCLS_DEFAULT_SYMBOLRATE_ANNEX_B_QAM256 5360537U

#define MXL_HRCLS_CLOCK_OUT_AMPLITUDE_MAX   32

#define MXL_HRCLS_INTERRUPT_ON_PID_CNT      16

#define MXL_HRCLS_DEMOD_FREQ_SEARCH_RANGE_IDX_MAX 15

#define MXL_HRCLS_WB_CHANNEL_FREQ_MAX_HZ    1054687500
#define MXL_HRCLS_WB_CHANNEL_FREQ_MIN_HZ    42187500

#define MXL_HRCLS_NB_CHANNEL_FREQ_MAX_HZ    1200000000
#define MXL_HRCLS_NB_CHANNEL_FREQ_MIN_HZ    40000000

#define MXL_HRCLS_DEMOD_SYMBOLRATE_MAX_SPS  7200000 
#define MXL_HRCLS_DEMOD_SYMBOLRATE_MIN_SPS  2000000 

#define MXL_HRCLS_IFOUT_FREQ_MIN_HZ         3500000
#define MXL_HRCLS_IFOUT_FREQ_MAX_HZ         44000000

#define MXL_HRCLS_IFOUT_LEVEL_MIN           (60*2)
#define MXL_HRCLS_IFOUT_LEVEL_MAX           (100*2)

#define MXL_HRCLS_FFT_START_FREQ_HZ_MIN     1000
#define MXL_HRCLS_FFT_END_FREQ_HZ_MAX       1300000000
#define MXL_HRCLS_FFT_STEP_SIZE_HZ_MIN      10000
#define MXL_HRCLS_FFT_STEP_SIZE_HZ_MAX      10000000

#define MXL_HRCLS_FFT_AVG_SETTING_MIN       1
#define MXL_HRCLS_FFT_AVG_SETTING_MAX       128


#define MXL_HRCLS_AFE_MODE_CHANGE_TIMEOUT_MS 200 

#define MXL_FLVR_MXL265                     0x05
#define MXL_FLVR_MXL258                     0x06
#define MXL_FLVR_MXL267                     0x07
#define MXL_FLVR_MXL269                     0x0E
#define MXL_FLVR_MXL254                     0x0A
#define MXL_FLVR_MXL256                     0x0B
#define MXL_FLVR_MXL252                     0x0C
#define MXL_FLVR_MXL255                     0x0D
#define MXL_FLVR_MXL259                     0x0F
#define MXL_FLVR_MXL212                     0x02
#define MXL_FLVR_MXL213                     0x03
#define MXL_FLVR_MXL214                     0x04

#define MXL_HRCLS_XPT_BASEADDR              0x30000U
#define MXL_HRCLS_XPT_MAX_PIDS              65
#define MXL_HRCLS_XPT_MAX_KNOWN_PID_VALUE   31U
#define MXL_HRCLS_XPT_MAX_HEADER_WORDS      3
#define MXL_HRCLS_XPT_CLKDIV_27_00MHz       13 

// Verified defines end here

#define SYM_NUM_B_BANK                      3
#define BYPASS_INTERPOLATOR                 0x0010

#define COMMAND_HEADER_LENGTH               4

#define MBIN_FORMAT_VERSION                 '1'
#define MBIN_FILE_HEADER_ID                 'M'
#define MBIN_SEGMENT_HEADER_ID              'S'

#define INVALID_ANNEX_QAM                   0xFF

#define MXL_HRCLS_DFE_EQUALIZER_TAPS_COUNT  36
#define MXL_HRCLS_DFE_EQUALIZER_TAPS_COUNT_V1 28

// Mailbox registers
#define MAILBOX_REG_NUM_PENDING_HOST_CMDS  0xF000
#define MAILBOX_REG_FIRMWARE_VER_AB        0xF004
#define MAILBOX_REG_FIRMWARE_VER_CD        0xF006
#define MAILBOX_REG_FIRMWARE_VER_RC        0xF008
#define MAILBOX_REG_BOOTLOADER_VER_AB      0xF00A
#define MAILBOX_REG_BOOTLOADER_VER_CD      0xF00C
#define MAILBOX_REG_BOOTLOADER_VER_RC      0xF00E
#define MAILBOX_REG_LAST_ERROR_CODE        0xF010
#define MAILBOX_REG_LAST_ERROR_PARM_HI     0xF022
#define MAILBOX_REG_LAST_ERROR_PARM_LO     0xF024
#define MAILBOX_REG_AFE_TILT_MODE          0xF028
#define MAILBOX_REG_AFE_TUNER_STATUS       0xF02A

#define MAILBOX_REG_RX_PWR                 0xF0A6
#define MAILBOX_REG_TEMPERATURE_CODE       0xF09C
#define MAILBOX_REG_TEMPERATURE_DEGREES    0xF17E

#define MEM_I2C_AUTO_INC            0x4
#define MEM_I2C_DATA_16             0xF801
#define MEM_I2C_DATA_16_AI          (MEM_I2C_DATA_16   | MEM_I2C_AUTO_INC)
#define MEM_I2C_ADDR_OFFSET         0xF80F 
#define MEM_I2C_TABLE_ID            0xF80D
#define MAILBOX_REG_DMD_EQUALIZER_RSP_LEN       	  0xF178

#define MAILBOX_REG_MBVAR_FFT_SOURCE_TYPE           0xF04E
#define MAILBOX_REG_FFT_SOURCE                      0xF050 
#define MAILBOX_REG_FFT_WINDOW_EN                   0xF052
#define MAILBOX_REG_FFT_WINDOW_SCALE                0xF054
#define MAILBOX_REG_FFT_RANK                        0xF056
#define MAILBOX_REG_FFT_AVG_SIZE                    0xF058
#define MAILBOX_REG_FFT_SYNC_DLY                    0xF05A
#define MAILBOX_REG_FFT_WAIT                        0xF05C
#define MAILBOX_REG_FFT_STATE                       0xF060
#define MAILBOX_REG_FFT_RUN_MODE                    0xF062
#define MAILBOX_REG_FFT_DURATION                    0xF070
#define MAILBOX_REG_ANTENNA_TO_FFT_GAIN_DB          0xF05E
#define MAILBOX_REG_AFE_HOST_BO_CTRL                0xF0F6
#define MAILBOX_REG_EXT_SPACE_AUTOREAD_EN           0xF09E
#define MAILBOX_REG_NB_HOST_HANDOVER_BKP            0xF102
#define MAILBOX_REG_MBVAR_HOSTINT_CONTROL           0xF110
#define MAILBOX_REG_MBVAR_HOSTINT_MASK              0xF112
#define MAILBOX_REG_MBVAR_HOSTINT_STATUS            0xF114
#define MAILBOX_REG_FFT_START_FREQ_HI 				      0xF116
#define MAILBOX_REG_FFT_START_FREQ_LO 				      0xF118
#define MAILBOX_REG_FFT_END_FREQ_HI   				      0xF11A
#define MAILBOX_REG_FFT_END_FREQ_LO   				      0xF11C
#define MAILBOX_REG_FFT_FREQ_STEP_HI  				      0xF11E
#define MAILBOX_REG_FFT_FREQ_STEP_LO  				      0xF120
#define MAILBOX_REG_FFT_SPEC_ENA      				      0xF122
#define MAILBOX_REG_FFT_RSP_LEN       				      0xF124
#define MAILBOX_REG_FFT_REQ_LEN       				      0xF126
#define MAILBOX_REG_FFT_ERR_CODE                    0xF170
#define MAILBOX_REG_SPEC_API_AVG_SETTING            0xF182
#define MAILBOX_REG_SPEC_API_WIN_SETTING            0xF19E 

#define MXL_HRCLS_FFT_ERROR_OK                      0
#define MXL_HRCLS_FFT_ERROR_FAILURE                 1
#define MXL_HRCLS_FFT_ERROR_RESOLUTION              2
#define MXL_HRCLS_FFT_ERROR_RESOLUTION_NB           3
#define MXL_HRCLS_FFT_ERROR_RESOLUTION_INVALID      4

#define SPECTRUM_BIN_LIMIT            512
#define SPECTRUM_NB_BW_8_MHZ          8000000

#define MXL_HRCLS_FW_FFT_WINDOW_NONE        0
#define MXL_HRCLS_FW_FFT_WINDOW_TRIANGLE    1
#define MXL_HRCLS_FW_FFT_WINDOW_RAISED_COS  2

#define MXL_HRCLS_EQU_NOISE_BW_NONE        100
#define MXL_HRCLS_EQU_NOISE_BW_TRIANGLE    109
#define MXL_HRCLS_EQU_NOISE_BW_RAISED_COS  110

// Macro to return a mask of 'nbits' ones from bit 'lsbloc'.
// (please use constants for lsbloc and nbits if possible, so that compiler/preprocessor 
// can expand most of calculations)
#define MXL_MASK(lsbloc, nbits)           ((0xFFFFFFFFU>>(32-(nbits))) << (lsbloc))

#define HWD_MASK(lsbloc, nbits)           ((0xFFFF>>(16-(nbits))) << (lsbloc))
#define EXTRACT_ADDR(r)                   MxL_HRCLS_Ctrl_ExtractRegAddr(r) 
#define EXTRACT_ADDR32(r)                 MxL_HRCLS_Ctrl_ExtractRegAddr32(r) 
#define EXTRACT_MASK(r)                   MxL_HRCLS_Ctrl_ExtractRegMask(r) 
#define EXTRACT_MASK32(r)                 MxL_HRCLS_Ctrl_ExtractRegMask32(r) 
#define EXTRACT_LSBLOC(r)                 MxL_HRCLS_Ctrl_ExtractRegLsbLoc(r) 
#define EXTRACT_LSBLOC32(r)               MxL_HRCLS_Ctrl_ExtractRegLsbLoc32(r) 
#define EXTRACT_NBITS(r)                  MxL_HRCLS_Ctrl_ExtractRegNumBits(r) 
#define EXTRACT_NBITS32(r)                MxL_HRCLS_Ctrl_ExtractRegNumBits32(r) 

#define MXL_HRCLS_LSB_MASK(bitWidth)      ((1 << bitWidth) - 1)

#define DFE_CHN_BW_6_MODE                 0   /* 6MHz mode         */
#define DFE_CHN_BW_8_MODE                 1   /* 8MHz mode         */

#define MXL_HRCLS_INTR_CTRL_GLOBAL_ENABLE (1 << 0)
#define MXL_HRCLS_INTR_CTRL_EDGE_TRIGGER  (1 << 1)
#define MXL_HRCLS_INTR_CTRL_ACTIVE_HIGH   (1 << 2)
#define MXL_HRCLS_INTR_CTRL_GPIO_DISABLE  (1 << 3)

#define MXL_HRCLS_DEFAULT_DEMOD_FREQ_SEARCH_RANGE_IDX 6

#define MXL_HRCLS_MAX_CHANNELS_MINOS_V1     24
#define MXL_HRCLS_MAX_CHANNELS_HERCULES_V1  24
#define MXL_HRCLS_MAX_CHANNELS_HERCULES_V2  10
#define MXL_HRCLS_MAX_CHANNELS_MIN          MXL_MIN(MXL_MIN(MXL_HRCLS_MAX_CHANNELS_MINOS_V1, MXL_HRCLS_MAX_CHANNELS_HERCULES_V1), MXL_HRCLS_MAX_CHANNELS_HERCULES_V2)

// -20	     0
// (-20,0]	 1
// (0,20]	   3
// (20,40]	 2
// (40,60]	 6
// (60,80]	 7
// (80,100]  5
// >100	     4
#define MxL_HRCLS_TEMPERATURE_INDEX(T) (((SINT32)T) <= -20)?0:(((SINT32)T)>100?7:((((SINT32)T)+ 19)/20 + 1))
#define MxL_HRCLS_GRAY_CODE                                {0, 1, 3, 2, 6, 7, 5, 4}
#define MxL_HRCLS_DEFAULT_TEMPERATURE                      70
#define MxL_HRCLS_DEFAULT_TEMPERATURE_TSENS_NOTUSED        50
#define MxL_HRCLS_GRAY_CODE_TEMPERATURE                    {-20, -10, 30, 10, 100, 90, 50, 70}

#define MXL_HRCLS_TSENS_MODE_MAX          7
#define MXL_HRCLS_TSENS_MODE_NVBE         0x7F

#define MXL_HRCLS_TEMP_CODE_MAP_LEN        32
#define MXL_HRCLS_TEMP_CODE_MAP_MASK       0x1F // 31

#define MXL_HRCLS_HERCULES_HW_ID 0x01
#define MXL_HRCLS_MINOS_HW_ID    0x11

/*****************************************************************************************
    User-Defined Types (Typedefs)
*****************************************************************************************/

#define MXL_HRCLS_HOST_COMMAND_FB_TUNER_ID 0
#define MXL_HRCLS_HOST_COMMAND_NB_TUNER_ID 1


#define MXL_HRCLS_HOST_COMMAND_SYNC_BYTE   0xE8
#define MXL_HRCLS_HOST_REGULAR_COMMAND_SEQ_NUM 1

#define MXL_HRCLS_SERDES_SWAP(serdesId) serdesId=(serdesId == MXL_HRCLS_SERDES_LANE0)?MXL_HRCLS_SERDES_LANE1:((serdesId == MXL_HRCLS_SERDES_LANE1)?MXL_HRCLS_SERDES_LANE0:serdesId); 

#define MXL_HRCLS_DEMOD_INTERRUPTS_MASK (MXL_HRCLS_INTR_MASK_DMD0 | MXL_HRCLS_INTR_MASK_DMD1 | MXL_HRCLS_INTR_MASK_DMD2 | MXL_HRCLS_INTR_MASK_DMD3 \
                                         | MXL_HRCLS_INTR_MASK_DMD4 | MXL_HRCLS_INTR_MASK_DMD5 | MXL_HRCLS_INTR_MASK_DMD6 | MXL_HRCLS_INTR_MASK_DMD7 \
                                         | MXL_HRCLS_INTR_MASK_DMD8)

/* I2C COMMAND Communication: Command Code ID */
typedef enum
{
    HOST_CMD_CFG_SET_ADDR                     = 1,
    HOST_CMD_CFG_DOWNLOAD_SEGMENT             = 2,
    HOST_CMD_CFG_DOWNLOAD_DONE                = 3,

    HOST_CMD_CFG_DEV_REF_CLK_OUT              = 7,
    HOST_CMD_CFG_DEV_POWER_MODE               = 8,
    HOST_CMD_CFG_DEV_UPSTREAM_PARAMS          = 9,
    HOST_CMD_CFG_DEV_CHAN_CSF_COEF            = 10,
    HOST_CMD_CFG_DEV_CHAN_CSF_COEF_OOB        = 11,
    HOST_CMD_CFG_DEV_HOST_INTERRUPT_CONTROL   = 12,

    HOST_CMD_CFG_TUNER_CHAN_TUNE              = 13,
    HOST_CMD_CFG_TUNER_CHAN_FINE_RETUNE       = 14,
    HOST_CMD_CFG_TUNER_NARROWBAND_OUT_INTERFACE = 15,
    HOST_CMD_CFG_TUNER_ENABLE                 = 16,
    HOST_CMD_CFG_TUNER_DSCAL                  = 17,
    HOST_CMD_CFG_TUNER_IFOUT_PARAMS           = 18,
    HOST_CMD_CFG_FFT_STATE                    = 19,
    HOST_CMD_TEST                             = 20,
    HOST_CMD_REQ_DEMOD_EQUALIZER_COEFFICIENTS = 21,

    HOST_CMD_CFG_DEMOD_SYMBOL_RATE            = 26,
    HOST_CMD_CFG_DEMOD_OUT_PARAMS_OOB         = 27,
    HOST_CMD_CFG_DEMOD_ERRORS_OOB             = 28,
    HOST_CMD_CFG_DEMOD_MPEGOUT_PARAMS         = 29,
    HOST_CMD_CFG_DEMOD_MPEGOUT_GLOBAL_PARAMS  = 30,
    HOST_CMD_CFG_DEMOD_ANNEX_QAM_TYPE         = 31,
    HOST_CMD_CFG_DEMOD_INTR_MASK              = 32,
    HOST_CMD_CFG_DEMOD_ADC_IQ_FLIP            = 33,
    HOST_CMD_CFG_DEMOD_EQUALIZER_FILTER       = 34,
    HOST_CMD_CFG_DEMOD_ENABLE                 = 35,
    HOST_CMD_CFG_DEMOD_RESTART                = 36,
    HOST_CMD_REQ_DEMOD                        = 37,
    HOST_CMD_CFG_DEMOD_CARRIER_OFFSET_FLIP    = 38,
    HOST_CMD_CFG_DEMOD_RESAMPLE_RATE_RATIO    = 39,

    HOST_CMD_CFG_SERDES_MODE                  = 43,
    HOST_CMD_CFG_SERDES_DS_PARAMS             = 44,
    HOST_CMD_CFG_SERDES_DS_PRBS_PARAMS        = 45,
    HOST_CMD_CFG_SERDES_DS_PRE_EMPHASIS       = 46,
    HOST_CMD_CFG_SERDES_DS_AMPLITUDE          = 47,
    HOST_CMD_CFG_SERDES_DS_CROSSBAR           = 48,
    HOST_CMD_CFG_SERDES_US_PARAMS             = 49,
    HOST_CMD_CFG_SERDES_US_SYNC_PARAMS        = 50,
    HOST_CMD_CFG_SERDES_US_EQUALIZER          = 51,
    HOST_CMD_CFG_SERDES_US_LANE_SWITCH        = 52,
    HOST_CMD_CFG_SERDES_US_PRBS               = 53,
    HOST_CMD_CFG_SERDES_CLEAR                 = 54,
    HOST_CMD_REQ_SERDES                       = 55,
    HOST_CMD_PID_CFG                          = 56,
    HOST_CMD_PIF_CFG_ALL_PIDFILTERS           = 57,
    HOST_CMD_PID_READ_ANY_TSPACKET            = 58,

    HOST_CMD_XPT_INITPIDS                     = 60,
    HOST_CMD_XPT_SET_SCHEME                   = 61,
    HOST_CMD_CFG_FW_DEBUG                     = 63,
  // MAX_NUM_HOST_COMMAND
    MXL_HRCLS_HOST_CMD_MAX
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
  UINT8/*MXL_CMD_ID_E*/ commandId;   /* 0x01: command opcode = 0x02 for firmware download */
  UINT8 payloadCheckSum;             /* 0x02: Checksum of packet payload */
  UINT8 payloadLen;                  /* 0x03: Length of packet payload: 1 - 252 */
  UINT8 data[MXL_HRCLS_MAX_COMMAND_DATA_SIZE]; /* 0x04-0x0xFF: command field data */
} HOST_COMMAND_T;

/* Firmware response data structure */
typedef struct
{
  UINT8 syncChar;                    /* 0x00: Bit[7-4]:Packet sync character (0x0E),  bit[3-0]: serial */
  MXL_RSP_ID_E errorCode;            /* 0x01: Error code, 0x00 means ok. */
  UINT8 payloadCheckSum;             /* 0x02: Checksum of packet payload */
  UINT8 payloadLen;                  /* 0x03: Length of packet payload: 1 - 252 */
  UINT8 data[MXL_HRCLS_MAX_COMMAND_DATA_SIZE]; /* 0x04-0xFF: response field data */
} FW_RESPONSE_T, *PFW_RESPONSE_T;

typedef struct
{
  UINT8 id;                 /* 0x00-0x00: file identifier ('M') */
  UINT8 fmtVersion;         /* 0x01-0x01: file format version in ascii (default to '1') */
  UINT8 headerLen;          /* 0x02-0x02: header length (<256) in bytes, should be sizeof(MBIN_FILE_HEADER_T) */
  UINT8 numSegments;        /* 0x03-0x03: number of segments in the image (<256) */
  UINT8 entryAddress[4];    /* 0x04-0x07: entry address of execution, in big endian */
  UINT8 imageSize24[3];     /* 0x08-0x0A: 24-bit image size in bytes, not including header, in big endian */
  UINT8 imageChecksum;      /* 0x0B-0x0B: 8-bit checksum of file data, not including header */
  UINT8 reserved[4];        /* 0x0C-0x0F: reserved for future use */
} MBIN_FILE_HEADER_T;

typedef struct
{
  MBIN_FILE_HEADER_T  header;    /* 0x00: file header */
  UINT8 data[1];                 /* 0x10: place holder for accessing starting of filedata */
} MBIN_FILE_T;

typedef struct
{
  UINT8 id;                      /* 0x00-0x00: segment identifier (always 'S' for now) */
  UINT8 len24[3];                /* 0x01-0x03: data length in 24-bit big endian, not including header */
                                 /* PLEASE note: */
                                 /*    len24 is the real length of following data segement, and the next memory */
                                 /*    segment (if any) will start from next 32-bit aligned address, which is   */
                                 /*    &data[(len24+3)&(~3)]                                                    */
  UINT8 address[4];              /* 0x04-0x07: loading address of this segment, in big endian                   */
} MBIN_SEGMENT_HEADER_T;


typedef struct
{
  MBIN_SEGMENT_HEADER_T header;  /* 0x00: segment header */
  UINT8 data[1];                 /* 0x08: place holder for accessing starting of data */
} MBIN_SEGMENT_T;


typedef enum           
{                      
  MXL_HRCLS_FFT_MODE_IDLE = 0,
  MXL_HRCLS_FFT_MODE_SPECTRUM_API_DIAG = 1,
  MXL_HRCLS_FFT_MODE_SCAN = 2,   
  MXL_HRCLS_FFT_MODE_DEBUG = 3,    
  MXL_HRCLS_FFT_MODE_SPECTRUM_API = 4,
  MXL_HRCLS_FFT_MODE_INVALID = 5,  
} MXL_HRCLS_FFT_RUN_MODE_E;

typedef enum
{
  MXL_HRCLS_FFT_FB_SB = 0,
  MXL_HRCLS_FFT_FB_CHAN = 1,
  MXL_HRCLS_FFT_NB_CHAN = 2
} MXL_HRCLS_FFT_SOURCE_TYPE_E;

typedef enum         
{                      
  MXL_HRCLS_FFT_DEBUG_FFT = 0,     
  MXL_HRCLS_FFT_DEBUG_TIME = 1,    
  MXL_HRCLS_FFT_DEBUG_WINDOWED_TIME = 2,
} MXL_HRCLS_FFT_DEBUG_TYPE_E;

typedef struct
{
  UINT16 regAddr;
  UINT16 mask;
  UINT16 data;
} REG_CTRL_INFO_T, *PREG_CTRL_INFO_T;

typedef struct
{
  UINT16 regAddr;
  UINT8  lsbPos;
  UINT8  fieldWidth;
} MXL_HRCLS_FIELD_T;  

typedef struct
{
  UINT32 regAddr;
  UINT8  lsbPos;
  UINT8  fieldWidth;
} MXL_HRCLS_FIELD32_T;  

#ifdef _MXL_HRCLS_DEMOD_ENABLED_
typedef struct
{
  UINT8 interDepthI;
  UINT8 interDepthJ;
} INTERLEAVER_LOOKUP_INFO_T, *PINTERLEAVER_LOOKUP_INFO_T;

typedef struct
{
  MXL_HRCLS_ANNEX_TYPE_E annexType;
  MXL_HRCLS_QAM_TYPE_E   qamType;
  UINT8  depthI;
  UINT8  depthJ;
  UINT16 dmdMaxFreezeWinLen;
} BURST_NOISE_LOOKUP_T, *PBURST_NOISE_LOOKUP_T;
#endif// _MXL_HRCLS_DEMOD_ENABLED_

typedef enum
{
  FIRMWARE_STATUS_UNKNOWN = 0,
  FIRMWARE_STATUS_BOOTLOADER,
  FIRMWARE_STATUS_LOADED,
} FIRMWARE_STATUS_E;

typedef enum
{
  MXL_HRCLS_XPT_MAP_1_TO_1 = 0,
  MXL_HRCLS_XPT_MAP_2_TO_1,
  MXL_HRCLS_XPT_MAP_4_TO_1,
  MXL_HRCLS_XPT_MAP_PARALLEL,
  MXL_HRCLS_XPT_MAP_CABLECARD,

  MXL_HRCLS_XPT_MAP_INVALID
} MXL_HRCLS_XPT_MAP_E;

typedef struct
{
  MXL_HRCLS_CHAN_ID_E   phyChan;
  UINT8                 ifMask;
} MXL_HRCLS_CHAN_T;

typedef enum
{
  MXL_HRCLS_AFE_RESTORE=0,
  MXL_HRCLS_AFE_FLAT_LNABO_0 = 1,
  MXL_HRCLS_AFE_FLAT_LNABO_NON0 = 2,
  MXL_HRCLS_AFE_TILT1 = 3,
  MXL_HRCLS_AFE_TILT2 = 4,
  MXL_HRCLS_AFE_TILT3 = 5,
  MXL_HRCLS_AFE_TILT4=6, 
  MXL_HRCLS_AFE_FLAT_LNABO_FREE=7, // may not need
} MXL_HRCLS_FW_MXL_HRCLS_AFE_STATE_E;

typedef enum         
{                  
  MXL_HRCLS_BLK_READ_DMD_EQUALIZER_FILTER             = 0,
  MXL_HRCLS_BLK_READ_SPEC_API                         = 1, 
  MXL_HRCLS_BLK_READ_TSPACKET_API                     = 2,
#ifdef _I2C_BLOCK_DOWNLOAD_ENABLED_ 
  MXL_HRCLS_BLK_READ_CFG_DOWNLOAD_DONE                = 0xFE,
#endif             
  MXL_HRCLS_BLK_READ_ID_MAX                           = 0xFF,
} MXL_HRCLS_BLOCK_READ_ID_E;

typedef enum
{
  MXL_HRCLS_EQUALIZER_FFE, 
  MXL_HRCLS_EQUALIZER_SPUR, 
  MXL_HRCLS_EQUALIZER_DFE
} MXL_HRCLS_EQUALIZER_COEF_TYPE_E;

#define MXL_HRCLS_XPT_212_SCHEME_NO_MUX_4     0

#define MXL_HRCLS_XPT_213_SCHEME_NO_MUX_4     0

#define MXL_HRCLS_XPT_214_SCHEME_NO_MUX_4     0

#define MXL_HRCLS_XPT_258_SCHEME_NO_MUX_4     0
#define MXL_HRCLS_XPT_258_SCHEME_MUX_4        1
#define MXL_HRCLS_XPT_258_SCHEME_MUX_2        2
#define MXL_HRCLS_XPT_258_SCHEME_PARALLEL     3

#define MXL_HRCLS_XPT_252_SCHEME_NO_MUX_2     0
#define MXL_HRCLS_XPT_252_SCHEME_PARALLEL     1

#define MXL_HRCLS_XPT_254_SCHEME_NO_MUX_4     0
#define MXL_HRCLS_XPT_254_SCHEME_MUX_2        1
#define MXL_HRCLS_XPT_254_SCHEME_MUX_1        2
#define MXL_HRCLS_XPT_254_SCHEME_PARALLEL     3

#define MXL_HRCLS_XPT_255_SCHEME_NO_MUX_4     0

#define MXL_HRCLS_XPT_256_SCHEME_NO_MUX_4     0
#define MXL_HRCLS_XPT_256_SCHEME_MUX_4        1
#define MXL_HRCLS_XPT_256_SCHEME_MUX_3        2
#define MXL_HRCLS_XPT_256_SCHEME_MUX_2        3
#define MXL_HRCLS_XPT_256_SCHEME_MUX_2_B      4
#define MXL_HRCLS_XPT_256_SCHEME_PARALLEL     5

#ifdef MXL_HRCLS_265_ENABLE
  #define MXL_HRCLS_MAX_DFE_CHANNELS_265_5400 16
  #define MXL_HRCLS_MAX_DFE_CHANNELS_265_5184 12
  #define MXL_HRCLS_SERDES_DS_LANES_CNT_265   1
  #define MXL_HRCLS_DEMODS_CNT_265            1
  #define MXL_HRCLS_IFOUT_SUPPORT_265         MXL_TRUE
  #define MXL_HRCLS_IFOUT_CNT_265             4
  #define MXL_HRCLS_OOB_SUPPORT_265           MXL_FALSE
  #define MXL_HRCLS_OOB_DEMOD_265             MXL_HRCLS_DEMOD0 
  #define MXL_HRLCS_WAKE_ON_WAN_DEMOD_265     MXL_HRCLS_DEMOD0
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_265_HERCULES  MXL_FALSE
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_265_MINOS     MXL_TRUE
#endif  

#ifdef MXL_HRCLS_267_ENABLE
  #define MXL_HRCLS_MAX_DFE_CHANNELS_267_5400 24 
  #define MXL_HRCLS_MAX_DFE_CHANNELS_267_5184 16
  #define MXL_HRCLS_SERDES_DS_LANES_CNT_267   2
  #define MXL_HRCLS_DEMODS_CNT_267            1
  #define MXL_HRCLS_IFOUT_SUPPORT_267         MXL_TRUE
  #define MXL_HRCLS_IFOUT_CNT_267             4
  #define MXL_HRCLS_OOB_SUPPORT_267           MXL_FALSE
  #define MXL_HRCLS_OOB_DEMOD_267             MXL_HRCLS_DEMOD0
  #define MXL_HRLCS_WAKE_ON_WAN_DEMOD_267     MXL_HRCLS_DEMOD0
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_267_HERCULES  MXL_FALSE
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_267_MINOS     MXL_TRUE
#endif
    
#ifdef MXL_HRCLS_212_ENABLE
  #define MXL_HRCLS_IFOUT_CNT_212_V2    4
  #define MXL_HRCLS_IFOUT_SUPPORT_212   MXL_TRUE
  #define MXL_HRCLS_DEMODS_CNT_212      2
  #define MXL_HRCLS_XPT_OUTPUTS_212     4
  #define MXL_HRCLS_MAX_DFE_CHANNELS_212_XPT_NOMUX_4 6
  #define MXL_HRCLS_OOB_DEMOD_212     MXL_HRCLS_DEMOD0
  #define MXL_HRLCS_WAKE_ON_WAN_DEMOD_212 	MXL_HRCLS_DEMOD0
#endif

#ifdef MXL_HRCLS_213_ENABLE
  #define MXL_HRCLS_IFOUT_CNT_213_V2    4
  #define MXL_HRCLS_IFOUT_SUPPORT_213   MXL_TRUE
  #define MXL_HRCLS_DEMODS_CNT_213      3 
  #define MXL_HRCLS_XPT_OUTPUTS_213     4
  #define MXL_HRCLS_MAX_DFE_CHANNELS_213_XPT_NOMUX_4 7
  #define MXL_HRCLS_OOB_DEMOD_213 	MXL_HRCLS_DEMOD0
  #define MXL_HRLCS_WAKE_ON_WAN_DEMOD_213   MXL_HRCLS_DEMOD0
#endif

#ifdef MXL_HRCLS_214_ENABLE
  #define MXL_HRCLS_IFOUT_CNT_214_V2    4
  #define MXL_HRCLS_IFOUT_SUPPORT_214   MXL_TRUE
  #define MXL_HRCLS_DEMODS_CNT_214      4 
  #define MXL_HRCLS_XPT_OUTPUTS_214     4
  #define MXL_HRCLS_MAX_DFE_CHANNELS_214_XPT_NOMUX_4 8
  #define MXL_HRCLS_OOB_DEMOD_214 	MXL_HRCLS_DEMOD0
  #define MXL_HRLCS_WAKE_ON_WAN_DEMOD_214   MXL_HRCLS_DEMOD0
#endif

#ifdef MXL_HRCLS_258_ENABLE
  #define MXL_HRCLS_MAX_DFE_CHANNELS_258      9
  #define MXL_HRCLS_SERDES_DS_LANES_CNT_258   0
  #define MXL_HRCLS_DEMODS_CNT_258            9
  #define MXL_HRCLS_DEMODS_CNT_258_NOMUX      5
  #define MXL_HRCLS_IFOUT_SUPPORT_258         MXL_FALSE
  #define MXL_HRCLS_IFOUT_SUPPORT_258_V2      MXL_TRUE
  #define MXL_HRCLS_IFOUT_CNT_258_V2          1
  #define MXL_HRCLS_OOB_SUPPORT_258           MXL_TRUE
  #define MXL_HRCLS_OOB_DEMOD_258             MXL_HRCLS_DEMOD0
  #define MXL_HRLCS_WAKE_ON_WAN_DEMOD_258     MXL_HRCLS_DEMOD0 
  #define MXL_HRCLS_XPT_OUTPUTS_258           4
  #define MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_NOMUX_4    6  
  #define MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_MUX_4     10
  #define MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_MUX_2     10
  #define MXL_HRCLS_MAX_DFE_CHANNELS_258_XPT_PAR       10
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_258     MXL_FALSE
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_258_V2  MXL_TRUE
#endif

#ifdef MXL_HRCLS_254_ENABLE
  #define MXL_HRCLS_MAX_DFE_CHANNELS_254      9
  #define MXL_HRCLS_SERDES_DS_LANES_CNT_254   0
  #define MXL_HRCLS_DEMODS_CNT_254            5
  #define MXL_HRCLS_IFOUT_SUPPORT_254         MXL_TRUE
  #define MXL_HRCLS_IFOUT_SUPPORT_254_V2      MXL_TRUE
  #define MXL_HRCLS_IFOUT_CNT_254_V1          4
  #define MXL_HRCLS_IFOUT_CNT_254_V2          4
  #define MXL_HRCLS_OOB_SUPPORT_254           MXL_TRUE
  #define MXL_HRCLS_OOB_DEMOD_254             MXL_HRCLS_DEMOD4 
  #define MXL_HRLCS_WAKE_ON_WAN_DEMOD_254     MXL_HRCLS_DEMOD4
  #define MXL_HRCLS_XPT_OUTPUTS_254           4
  #define MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_NOMUX_4    8 
  #define MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_MUX_2      8
  #define MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_MUX_1      8
  #define MXL_HRCLS_MAX_DFE_CHANNELS_254_XPT_PAR        8
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_254     MXL_FALSE
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_254_V2  MXL_TRUE
#endif

#ifdef MXL_HRCLS_252_ENABLE
  #define MXL_HRCLS_MAX_DFE_CHANNELS_252      3
  #define MXL_HRCLS_SERDES_DS_LANES_CNT_252   0
  #define MXL_HRCLS_DEMODS_CNT_252            3
  #define MXL_HRCLS_IFOUT_SUPPORT_252         MXL_FALSE
  #define MXL_HRCLS_IFOUT_SUPPORT_252_V2      MXL_TRUE
  #define MXL_HRCLS_IFOUT_CNT_252_V2          1
  #define MXL_HRCLS_OOB_SUPPORT_252           MXL_TRUE
  #define MXL_HRCLS_OOB_DEMOD_252             MXL_HRCLS_DEMOD2 
  #define MXL_HRLCS_WAKE_ON_WAN_DEMOD_252     MXL_HRCLS_DEMOD2
  #define MXL_HRCLS_XPT_OUTPUTS_252           4
  #define MXL_HRCLS_MAX_DFE_CHANNELS_252_XPT_NOMUX_2    4 
  #define MXL_HRCLS_MAX_DFE_CHANNELS_252_XPT_PAR        4
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_252_V2  MXL_TRUE
#endif

#ifdef MXL_HRCLS_255_ENABLE
  #define MXL_HRCLS_MAX_DFE_CHANNELS_255      5
  #define MXL_HRCLS_SERDES_DS_LANES_CNT_255   0
  #define MXL_HRCLS_DEMODS_CNT_255            5
  #define MXL_HRCLS_IFOUT_SUPPORT_255         MXL_FALSE
  #define MXL_HRCLS_IFOUT_SUPPORT_255_V2      MXL_TRUE
  #define MXL_HRCLS_IFOUT_CNT_255_V2          1
  #define MXL_HRCLS_OOB_SUPPORT_255           MXL_TRUE
  #define MXL_HRCLS_OOB_DEMOD_255             MXL_HRCLS_DEMOD4 
  #define MXL_HRLCS_WAKE_ON_WAN_DEMOD_255     MXL_HRCLS_DEMOD4
  #define MXL_HRCLS_XPT_OUTPUTS_255           4
  #define MXL_HRCLS_MAX_DFE_CHANNELS_255_XPT_NOMUX_4    6 
  #define MXL_HRCLS_MAX_DFE_CHANNELS_255_XPT_MUX_2      6
  #define MXL_HRCLS_MAX_DFE_CHANNELS_255_XPT_MUX_1      6
  #define MXL_HRCLS_MAX_DFE_CHANNELS_255_XPT_PAR        6
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_255_V2  MXL_TRUE
#endif

#ifdef MXL_HRCLS_256_ENABLE
  #define MXL_HRCLS_MAX_DFE_CHANNELS_256      7
  #define MXL_HRCLS_SERDES_DS_LANES_CNT_256   0
  #define MXL_HRCLS_DEMODS_CNT_256            7
  #define MXL_HRCLS_DEMODS_CNT_256_NOMUX      5
  #define MXL_HRCLS_IFOUT_SUPPORT_256         MXL_FALSE
  #define MXL_HRCLS_IFOUT_SUPPORT_256_V2      MXL_TRUE
  #define MXL_HRCLS_IFOUT_CNT_256_V2          1
  #define MXL_HRCLS_OOB_SUPPORT_256           MXL_TRUE
  #define MXL_HRCLS_OOB_DEMOD_256             MXL_HRCLS_DEMOD0
  #define MXL_HRLCS_WAKE_ON_WAN_DEMOD_256     MXL_HRCLS_DEMOD0
  #define MXL_HRCLS_XPT_OUTPUTS_256           4
  #define MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_NOMUX_4    6
  #define MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_MUX_4      8
  #define MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_MUX_3      8 
  #define MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_MUX_2      8 
  #define MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_MUX_2b     8 
  #define MXL_HRCLS_MAX_DFE_CHANNELS_256_XPT_PAR        8
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_256     MXL_FALSE
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_256_V2  MXL_TRUE
#endif
    
#ifdef MXL_HRCLS_269_ENABLE
  #define MXL_HRCLS_MAX_DFE_CHANNELS_269_5400 24 
  #define MXL_HRCLS_MAX_DFE_CHANNELS_269_5184 24 
  #define MXL_HRCLS_SERDES_DS_LANES_CNT_269   2
  #define MXL_HRCLS_DEMODS_CNT_269            9
  #define MXL_HRCLS_IFOUT_SUPPORT_269         MXL_TRUE
  #define MXL_HRCLS_IFOUT_CNT_269             4
  #define MXL_HRCLS_OOB_SUPPORT_269           MXL_TRUE
  #define MXL_HRCLS_OOB_DEMOD_269             MXL_HRCLS_DEMOD0
  #define MXL_HRLCS_WAKE_ON_WAN_DEMOD_269     MXL_HRCLS_DEMOD0
  #define MXL_HRCLS_AUTO_SPECTRUM_INV_269     MXL_FALSE
#endif


#define MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT 8
#define MXL_HRCLS_CAL_POINTS_PER_SEGMENT 5
#define MXL_HRCLS_CAL_MAX_FREQ_POINTS (MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT * MXL_HRCLS_CAL_POINTS_PER_SEGMENT + 1)
#define MXL_HRCLS_CAL_MIN_FREQ_POINTS ((MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT - 1) * MXL_HRCLS_CAL_POINTS_PER_SEGMENT + 1)

#define MXL_HRCLS_CAL_TILT_MODES_COUNT  6

/** @brief REFSX Tune timeout in micro secs */
// TODO: Check with RFIC team on default value
#define MXL_HRCLS_REF_SX_TUNE_TO_IN_US            (1000)  

/** @brief REFSX VCO Tune timeout in micro secs */
// TODO: Check with RFIC team on default value
#define MXL_HRCLS_RCAL_TO_IN_US                   (10) 
#define MXL_HRCLS_RCTUNE_TO_IN_US                 (10)

#define MXL_HRCLS_BITMASK_RFSX_SPARE_RCAL         (0x0F)

typedef struct
{
  UINT16                freqMHz;
  UINT16                refPowerdBuV;
  SINT16                deltaPowerdB;

  UINT8                 normFreqx100;
  UINT8                 segmentIdx;
} MXL_HRCLS_CAL_FREQ_POINT_T;

typedef struct
{
  SINT32                c2;
  SINT32                c1;
  SINT32                c0;
} MXL_HRCLS_CAL_COEFF_T;

typedef struct
{
  MXL_BOOL_E            calDone;
  UINT8                 segmentsCount;
  UINT8                 freqPointsCount;

  MXL_HRCLS_CAL_FREQ_POINT_T freqPoints[MXL_HRCLS_CAL_MAX_FREQ_POINTS];

  MXL_CALLBACK_FN_T     callbackFnPtr;
  UINT8                 currentTiltIndex;
} MXL_HRCLS_CAL_DATA_T;

#ifdef _MXL_DIAG_ENABLED_
#pragma pack(push)
#pragma pack(4)
#endif // _MXL_DIAG_ENABLED_
typedef struct
{
  UINT8                     fileSignature;
  UINT8                     fileDataFormat;
  UINT8                     fileRecordSize;
  UINT8                     fileRecordsCnt;
  UINT8                     fileCheckSum;
  MXL_BOOL_E                dataLoaded;

  UINT16                    firstFreqMHz;
  MXL_HRCLS_CAL_COEFF_T     pwrCorrCoeffs[MXL_HRCLS_CAL_TILT_MODES_COUNT][MXL_HRCLS_CAL_MAX_SEGMENTS_COUNT];

} MXL_HRCLS_PWR_REPORTING_DATA_T;
#ifdef _MXL_DIAG_ENABLED_
#pragma pack(pop)
#endif // _MXL_DIAG_ENABLED_

typedef struct
{
  UINT8                 devId;
  UINT8                 driverInitialized;
  MXL_HRCLS_CHIP_ID_E   chipId;
  UINT16                chipVersion;       // 0 for FPGA platform, 1 for ES1, 2 for ES2
  UINT32                adcSampRateInHz;

  MXL_HRCLS_FW_MXL_HRCLS_AFE_STATE_E  currentAfeTiltMode;
  UINT8                 currentAfeBO;

  FIRMWARE_STATUS_E     firmwareStatus;

  MXL_HRCLS_PWR_REPORTING_DATA_T  pwrReportData;
  
  UINT8                 currentTemp;

  MXL_HRCLS_DEVICE_E    deviceType;
  UINT32                interruptMask;

#ifdef _MXL_HRCLS_SERDES_ENABLED_
  MXL_HRCLS_SERDES_LINERATE_E serDesLineRate;
  MXL_HRCLS_SERDES_MODE_E     serDesMode[MXL_HRCLS_SERDES_DS_LANES_CNT];
  UINT8                       serDesDSLanesCnt;
  MXL_HRCLS_CHAN_ID_E         serDesSlot2Chan[MXL_HRCLS_SERDES_DS_LANES_CNT]; 
  UINT8                       serDesSlotsPerLane[MXL_HRCLS_SERDES_DS_LANES_CNT];
  MXL_BOOL_E                  serDesUsPrbsMode;
#endif

  MXL_BOOL_E            nbTunerPowerState;
  MXL_BOOL_E            fbTunerPowerState;
  MXL_HRCLS_CHAN_T *    dfeChanMap;
  struct
  {
    UINT32              freqHz;
  } dfeChannels[MXL_HRCLS_MAX_NUM_CHANNELS];
  UINT8                 dfeChanCount;
  UINT8                 dfeMaxChannels;

#ifdef _MXL_HRCLS_DEMOD_ENABLED_
  struct
  {
    MXL_HRCLS_QAM_TYPE_E      curQamType;
    MXL_HRCLS_ANNEX_TYPE_E    curAnnexType;
    UINT32                    curSymbolRateInHz;
    UINT32                    curResampleRate[SYM_NUM_B_BANK];
    MXL_BOOL_E                curBypassInterpolator;
    MXL_BOOL_E                oobMode;
    MXL_BOOL_E                enabled;
    UINT8                     demodFreqRangeIdx;
    MXL_BOOL_E                autoSpectrumInversionEnabled;
  } demods[MXL_HRCLS_MAX_NUM_DEMOD];
  UINT8                       demodsCnt;
  MXL_HRCLS_DMD_ID_E *        demodsMap;
  MXL_BOOL_E                  autoSpectrumInversionSupported;
#ifdef _MXL_HRCLS_OOB_ENABLED_
  MXL_BOOL_E                  oobSupported;
  MXL_BOOL_E                  oobFec_55_2_Supported;
  MXL_BOOL_E                  oobTsOutputSupported;
  MXL_HRCLS_DMD_ID_E          oobDemod;
  MXL_HRCLS_OOB_TYPE_E        oobType;
#endif    
  MXL_HRCLS_DMD_ID_E          wakeOnWanDemod;
  struct
  {
    MXL_BOOL_E                supported;
#ifdef _MXL_HRCLS_XPT_ENABLED_  
    UINT8                     modesCnt;
    UINT8                     outputsCnt;
    struct
    {
      MXL_HRCLS_XPT_MODE_E      mode;
      MXL_HRCLS_DMD_ID_E *      demodsMap;
      MXL_HRCLS_XPT_OUTPUT_ID_E * outputMap;
      MXL_HRCLS_XPT_MAP_E       xptMap;
      UINT8                     demodsCnt;
      UINT8                     dfeChanCount;
      MXL_HRCLS_CHAN_T *        dfeChanMap;
      UINT8                     demodScheme;
    } modes[MXL_HRCLS_XPT_MODE_MAX], *currentMode;
    MXL_BOOL_E                  commonClockEnabled;
    MXL_HRCLS_MPEG_CLK_RATE_E   commonClkFreq; 
    UINT16                      enableMap;
    MXL_BOOL_E                  fourWireModeSupported;
    MXL_HRCLS_XPT_OUTPUT_ID_E   clkSrcOutputId;
#endif   
  } xpt;
#endif
#ifdef _MXL_HRCLS_IFOUT_ENABLED_
  MXL_BOOL_E                  ifOutSupported;
  UINT8                       ifOutCnt;
  MXL_HRCLS_IF_ID_E *         ifOutMap;
#endif  
  MXL_BOOL_E                  clockOutSupported;
} MXL_HRCLS_DEV_CONTEXT_T;

#define MXL_HRCLS_TABLE_ID_FFT_HOST_CMD_DATA  0x00
#define MXL_HRCLS_TABLE_ID_PID_TS_PACKET      0x0b

#define MXL_HRCLS_TS_BUFFER_NHALFWORDS        94

/******************************************************************************
    Global Variable Declarations
******************************************************************************/
 
//extern MXL_HRCLS_DEV_CONTEXT_T MxL_HRCLS_Dev[MXL_HRCLS_MAX_NUM_DEVICES];

/******************************************************************************
    Prototypes
******************************************************************************/

#ifdef _MXL_HRCLS_DEMOD_ENABLED_
MXL_STATUS_E MxL_HRCLS_Ctrl_ProgramRegisters(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId , PREG_CTRL_INFO_T ctrlRegInfoPtr);
MXL_STATUS_E MxL_HRCLS_Ctrl_ReadDemodRegisterField(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, UINT16 regAddr, UINT8 lsbPos, UINT8 fieldWidth, /*@out@*/ UINT16 * Value); 
MXL_STATUS_E MxL_HRCLS_Ctrl_UpdateDemodRegisterField(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, UINT16 regAddr, UINT8 lsbPos, UINT8 fieldWidth, UINT16 newValue);
MXL_STATUS_E MxL_HRCLS_Ctrl_WriteDemodRegister(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, UINT16 regAddr, UINT16 newValue);
MXL_STATUS_E MxL_HRCLS_Ctrl_ReadDemodRegister(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, UINT16 regAddr, /*@out@*/ UINT16 * Value);
MXL_STATUS_E MxL_HRCLS_Ctrl_ConfigDemodEqualizer(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId);
MXL_STATUS_E MxL_HRCLS_Ctrl_ConfigDemodResampRatio(UINT8 devId, MXL_HRCLS_DMD_ID_E demodId, UINT32 symbolRate[], UINT8 bank[], UINT8 numBank);
MXL_STATUS_E MxL_HRCLS_Ctrl_CfgDemReset(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_DMD_ID_E demodId);
MXL_HRCLS_DMD_ID_E MxL_HRCLS_Ctrl_ConvertLogical2PhysicalDemodId(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_DMD_ID_E logicalDemodId);
#endif // _MXL_HRCLS_DEMOD_ENABLED_

#ifdef _MXL_HRCLS_IFOUT_ENABLED_
MXL_HRCLS_IF_ID_E MxL_HRCLS_Ctrl_GetPhysicalIfOutId(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_IF_ID_E logicalIfOutId);
#endif

MXL_STATUS_E MxL_HRCLS_Ctrl_DownloadFirmwareInMbin(MXL_HRCLS_DEV_CONTEXT_T* devContextPtr, const MBIN_FILE_T* mbinPtr,UINT8 enableRun, /*@null@*/ MXL_CALLBACK_FN_T fwCallbackFn);
MXL_STATUS_E MxL_HRCLS_Ctrl_SendHostCommand(UINT8 devId, HOST_COMMAND_T* cmdPtr, MXL_CMD_ID_E cmdId, UINT8 seqNum);
MXL_STATUS_E MxL_HRCLS_Ctrl_ReadRegisterField(UINT8 devId, UINT16 regAddr, UINT8 lsbPos, UINT8 fieldWidth, /*@out@*/ UINT16* valuePtr);
MXL_STATUS_E MxL_HRCLS_Ctrl_UpdateRegisterField(UINT8 devId, UINT16 regAddr, UINT8 lsbPos, UINT8 fieldWidth, UINT16 newValue);
MXL_STATUS_E MxL_HRCLS_Ctrl_DeviceStandby(UINT8 devId);
MXL_HRCLS_DEV_CONTEXT_T * MxL_HRCLS_Ctrl_GetDeviceContext(UINT8 devId);
UINT16       MxL_HRCLS_Ctrl_ExtractRegAddr(UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth) /*@modifies nothing@*/;
UINT32       MxL_HRCLS_Ctrl_ExtractRegAddr32(UINT32 regAddr, UINT8 bitLocation, UINT8 bitWidth) /*@modifies nothing@*/;
UINT8        MxL_HRCLS_Ctrl_ExtractRegLsbLoc(UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth) /*@modifies nothing@*/;
UINT8        MxL_HRCLS_Ctrl_ExtractRegLsbLoc32(UINT32 regAddr, UINT8 bitLocation, UINT8 bitWidth) /*@modifies nothing@*/;
UINT8        MxL_HRCLS_Ctrl_ExtractRegNumBits(UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth) /*@modifies nothing@*/; 
UINT8        MxL_HRCLS_Ctrl_ExtractRegNumBits32(UINT32 regAddr, UINT8 bitLocation, UINT8 bitWidth) /*@modifies nothing@*/; 
UINT16       MxL_HRCLS_Ctrl_ExtractRegMask(UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth) /*@modifies nothing@*/;
UINT32       MxL_HRCLS_Ctrl_ExtractRegMask32(UINT32 regAddr, UINT8 bitLocation, UINT8 bitWidth) /*@modifies nothing@*/;
UINT16       MxL_HRCLS_Ctrl_ExtractFieldFromRegister(UINT16 regValue, UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth) /*@modifies nothing@*/;
MXL_STATUS_E MxL_HRCLS_Ctrl_WaitForFieldValue(UINT8 devId, UINT16 regAddr, UINT8 lsbPos, UINT8 fieldWidth, UINT16 expectedValue, MXL_BOOL_E waitForEqual, UINT16 timeoutMs, /*@out@*/ /*@null@*/ UINT16 * returnedValue);
MXL_STATUS_E MxL_HRCLS_Ctrl_ReadTable16(UINT8 devId, UINT16 tableId, UINT16* readBufPtr, UINT16 length, UINT16 offsetBytes); 
MXL_STATUS_E MxL_HRCLS_Ctrl_ReadBlock16(UINT8 devId, UINT16 cmdId, UINT16 offset, UINT16 ReadWords, /*@out@*/ UINT16 *BufPtr);
MXL_STATUS_E MxL_HRCLS_Ctrl_ToggleRegisterBitZeroOne(UINT8 devId, UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth);
MXL_STATUS_E MxL_HRCLS_Ctrl_ToggleRegisterBitOneZero(UINT8 devId, UINT16 regAddr, UINT8 bitLocation, UINT8 bitWidth);
MXL_HRCLS_CHAN_ID_E MxL_HRCLS_Ctrl_ConvertLogical2PhysicalChanId(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_CHAN_ID_E logicalChanId);
MXL_STATUS_E MxL_HRCLS_Ctrl_InitializeDeviceParameters(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr);
MXL_STATUS_E MxL_HRCLS_Ctrl_ReqTunerRxPwr(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_CHAN_ID_E phyChanId, 
                                          /*@null@*/ /*@out@*/ UINT16* rxRawPwrIndBuVPtr, 
                                          /*@null@*/ /*@out@*/ MXL_HRCLS_RX_PWR_ACCURACY_E * accuracyPtr);
MXL_STATUS_E MxL_HRCLS_Ctrl_CfgTunerChanTune(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_CHAN_ID_E phyChanId,              
                                          UINT8 bandWidthInMhz, UINT32 centerFrequencyInHz);
MXL_STATUS_E MxL_HRCLS_Ctrl_SelectAFETiltMode(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_FW_MXL_HRCLS_AFE_STATE_E newTilt, UINT16 timeoutMs);
MXL_STATUS_E MxL_HRCLS_Ctrl_CfgDevXtalSetting(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, UINT8 xtalCap);
MXL_HRCLS_FW_MXL_HRCLS_AFE_STATE_E MxL_HRCLS_Ctrl_ReqDevCurrentAFETiltMode(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr);
UINT8 MxL_HRCLS_Ctrl_ReqDevCurrentAFEBackOff(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr);
MXL_STATUS_E MxL_HRCLS_Ctrl_CfgTunerFFTRunMode(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_FFT_RUN_MODE_E runMode, MXL_HRCLS_FFT_SOURCE_TYPE_E sourceType, 
                                          UINT8 sourceId, MXL_HRCLS_FFT_DEBUG_TYPE_E debugType, UINT32 freqStartInHz,
                                                                                                UINT32 freqStepInHz,
                                                                                                UINT16 stepsReq);
UINT32 MxL_HRCLS_Ctrl_ConvertDemod2PhysicalInterruptMask(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, UINT32 intrMask);
UINT32 MxL_HRCLS_Ctrl_ConvertDemod2LogicalInterruptMask(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, UINT32 intrMask);
MXL_STATUS_E MxL_HRCLS_Ctrl_GetTemperatureCode(MXL_HRCLS_DEV_CONTEXT_T *devContextPtr, /*@out@*/ UINT16* tsensCode);
MXL_STATUS_E MxL_HRCLS_Ctrl_ReqTunerTemperatureCode(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr);
MXL_STATUS_E MxL_HRCLS_Ctrl_MapCodeToTempInDegrees(UINT8 code, /*@out@*/ UINT8* temperature);

MXL_STATUS_E MxL_HRCLS_Ctrl_WriteClockRegister(UINT8 devId, UINT32 newValue);
MXL_STATUS_E MxL_HRCLS_Ctrl_ReadClockRegister(UINT8 devId, /*@out@*/ UINT32 * readValue);
MXL_HRCLS_CHAN_ID_E MxL_HRCLS_Ctrl_GetOOBPhysicalChannelId(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr);

#ifdef _MXL_HRCLS_XPT_ENABLED_
MXL_BOOL_E MxLWare_HRCLS_Ctrl_ValidateXPTMode(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_XPT_MODE_E mode);
MXL_STATUS_E MxLWare_Ctrl_ConfigureCommonClock(MXL_HRCLS_DEV_CONTEXT_T * devContextPtr, MXL_HRCLS_MPEG_CLK_RATE_E mpegClkFreq, MXL_HRCLS_MPEG_CLK_FMT_E mpegClkPol, MXL_HRCLS_MPEG_DRV_MODE_E mpegClkPadDrv);
#endif // _MXL_HRCLS_XPT_ENABLED_

#endif /* __MXL_HRCLS_PHY_CFG_H__*/

