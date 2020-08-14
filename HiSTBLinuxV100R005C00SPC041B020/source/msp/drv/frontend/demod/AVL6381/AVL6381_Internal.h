#ifndef __AVL6381_INTERNAL_H__
#define __AVL6381_INTERNAL_H__


#include <linux/delay.h>
#include <linux/module.h>
#include <linux/init.h>

#include "drv_tuner_ext.h"
#include "hi_drv_i2c.h"
#include "hi_type.h"
#include "drv_demod.h"
#include "hi_kernel_adapt.h"

#include "AVL6381_iic_rw.h"
#include "AVL6381_data_type.h"
#include "AVL6381_API.h"

#define AVL6381_OP_RX_IDLE              0x00
#define AVL6381_OP_RX_LD_DEFAULT        0x01
#define AVL6381_OP_RX_ACQUIRE           0x02
#define AVL6381_OP_RX_HALT              0x03
#define AVL6381_OP_RX_CMD_DEBUG         0x04    
#define AVL6381_OP_RX_SDRAM_TEST        0x05 
#define AVL6381_OP_RX_SLEEP             0x06
#define AVL6381_OP_RX_WAKE_UP           0x07
#define AVL6381_OP_RX_INIT_SDRAM        0x08
#define AVL6381_OP_RX_INIT_ADC          0x09
#define AVL6381_OP_RX_CHANGE_MODE       0x0A
#define AVL6381_OP_RX_ACQUIRE_DVBC_QM   0x0C


// Defines the AVL6381 device spectrum inversion mode
typedef enum AVL6381_SpectrumInversion
{
    SPECTRUM_NORMAL = 0,
    SPECTRUM_INVERTED = 1,
    SPECTRUM_AUTO = 2
}AVL6381_SpectrumInversion;


typedef struct AVL6381_PLL_Conf
{
    AVL6381_uint32 m_RefFrequency_Hz;           // The reference clock frequency in units of Hz.
    AVL6381_uchar m_Demod_PLL_DivR;           // PLL reference clock divider value
    AVL6381_uchar m_Demod_PLL_DivF;            // PLL feedback clock divider value
    AVL6381_uchar m_Demod_PLL_bwadj;
    AVL6381_uchar m_Demod_PLL_od;
    AVL6381_uint32 m_DemodFrequency_Hz;         // The internal demod clock frequency in units of Hz.
    AVL6381_uchar m_Aux_PLL_DivR;           // PLL reference clock divider value
    AVL6381_uchar m_Aux_PLL_DivF;            // PLL feedback clock divider value
    AVL6381_uchar m_Aux_PLL_bwadj;
    AVL6381_uchar m_Aux_PLL_od;                     // PLL output divider value
    AVL6381_uchar m_Aux_PLL_od2;                    // PLL output divider 2
    AVL6381_uchar m_Aux_PLL_od3;                    // PLL output divider 3
    AVL6381_uchar m_use_pll_adc;
    AVL6381_uint32 m_DDLL_out_phase;            // DDLL SDRAM OC Clock Phase Adjust value
    AVL6381_uint32 m_DDLL_read_phase;           // DDLL Read Clock Phase Adj Register
    AVL6381_uint32 m_FEC_Frequency_Hz;          // The FEC clock frequency in units of Hz.
    AVL6381_uint32 m_SDRAM_Frequency_Hz;        // The SDRAM clock frequency in units of Hz.
    AVL6381_uint32 m_ADC_Frequency_Hz;          // The FEC clock frequency in units of Hz.
}AVL6381_PLL_Conf;

// Used to configure the AVL PART device SDRAM controller. 
typedef struct AVL6381_SDRAM_Conf
{
    AVL6381_uint16 m_SDRAM_TRC;             // SDRAM active to active command period in ns. 
    AVL6381_uint16 m_SDRAM_TXSR;            // SDRAM exit self-refresh to active command period in ns.
    AVL6381_uint16 m_SDRAM_TRCAR;           // SDRAM auto refresh period in ns.  Minimum time between two auto refresh commands.
    AVL6381_uint16 m_SDRAM_TWR;             // SDRAM write recovery time in SDRAM clock cycles->  The delay from the last data in to the next precharge command.  Valid range is 1 to 4 clock cycles->
    AVL6381_uint16 m_SDRAM_TRP;             // SDRAM precharge period in ns.
    AVL6381_uint16 m_SDRAM_TRCD;            // SDRAM minimum delay between active and read/write commands in ns.
    AVL6381_uint16 m_SDRAM_TRAS;            // SDRAM minimum delay between active and precharge commands in ns.
    AVL6381_uint16 m_SDRAM_CAS_Latency;     // SDRAM delay between read command and availability of first data in SDRAM clock cycles->  Valid range is 1 to 4 clock cycles->
}AVL6381_SDRAM_Conf;

// Defines the polarity of the analog AGC control signal.  It must be set to match the requirement of the tuner being used.
// The default value in the AVL6381 device is AGC_INVERTED
typedef enum AVL6381_AGCPola
{
    AGC_NORMAL = 0,             // = 0  Normal. Used for a tuner whose gain increases with increased AGC voltage.
    AGC_INVERTED = 1            // = 1  Inverted. Used for tuner whose gain decreases with increased AGC voltage.
}AVL6381_AGCPola;

typedef enum AVL6381_MPEG_SerialDataPin
{
    AVL6381_MPEG_SERIAL_D0 = 0,
    AVL6381_MPEG_SERIAL_D1 = 1
}AVL6381_MPEG_SerialDataPin;

typedef enum AVL6381_MPEG_SERIAL_ORDER
{
    AVL6381_MPEG_SERIAL_ORDER_MSB = 0,
    AVL6381_MPEG_SERIAL_ORDER_LSB = 1
}AVL6381_MPEG_SERIAL_ORDER;

typedef enum AVL6381_MPEG_SERIAL_SYNC
{
    AVL6381_MPEG_SERIAL_SYNC_8_PULSE = 0,
    AVL6381_MPEG_SERIAL_SYNC_1_PULSE = 1
}AVL6381_MPEG_SERIAL_SYNC;

typedef enum AVL6381_MPEG_ErrorBit
{
    AVL6381_MPEG_ERROR_BIT_DISABLE = 0,
    AVL6381_MPEG_ERROR_BIT_ENABLE = 1
}AVL6381_MPEG_ErrorBit;

typedef enum AVL6381_MPEG_ErrorBitPola
{
    AVL6381_MPEG_ERROR_HIGH = 0,
    AVL6381_MPEG_ERROR_LOW = 1
}AVL6381_MPEG_ErrorBitPola;

typedef enum AVL6381_MPEG_ValidPola
{
    AVL6381_MPEG_VALID_HIGH = 0,
    AVL6381_MPEG_VALID_LOW = 1
}AVL6381_MPEG_ValidPola;

typedef enum AVL6381_MPEG_Packet_Len
{
    AVL6381_MPEG_188 = 0,
    AVL6381_MPEG_204 = 1
}AVL6381_MPEG_Packet_Len;

typedef enum AVL6381_MPEG_PARALLEL_ORDER
{
    AVL6381_MPEG_PARALLEL_ORDER_NORMAL = 0,
    AVL6381_MPEG_PARALLEL_ORDER_INVERT = 1
}AVL6381_MPEG_PARALLEL_ORDER;




// Defines Error statistics mode
typedef enum AVL6381_ErrorStat_Mode
{
    ERROR_STAT_MANUAL   =   0,    // manual error statistics means application should reset the error statistics manually.
    ERROR_STAT_AUTO     =   1     // auto error statistics which means error statistics will be reset automatically according to the setting of auto error statistics. Please refer to following data structure.
}AVL6381_ErrorStat_Mode;

// Defines the type of auto error statistics 
typedef enum AVL6381_AutoErrorStat_Type
{
    ERROR_STAT_BYTE     =   0,    // error statistics will be reset according to the number of received bytes->
    ERROR_STAT_TIME     =   1     // error statistics will be reset according to time interval.
}AVL6381_AutoErrorStat_Type;

// Defines the test pattern being used for BER/PER measurements.
typedef enum AVL6381_TestPattern
{
    AVL6381_TEST_LFSR_15    =   0, // BER test pattern is LFSR15
    AVL6381_TEST_LFSR_23    =   1  // BER test pattern is LFSR23        
}AVL6381_TestPattern;

// Defines whether the feeback bit of the LFSR used to generate the BER/PER test pattern is inverted.
typedef enum AVL6381_LFSR_FbBit
{
    AVL6381_LFSR_FB_NOT_INVERTED    =   0, // LFSR feedback bit isn't inverted
    AVL6381_LFSR_FB_INVERTED        =   1  // LFSR feedback bit is inverted
}AVL6381_LFSR_FbBit;

typedef struct AVL6381_ErrorStat_Info
{
    AVL6381_ErrorStat_Mode      m_ErrorStat_Mode;           // indicates the error statistics mode. 
    AVL6381_AutoErrorStat_Type  m_AutoErrorStat_Type;       // indicates the MPEG data sampling clock mode.
    AVL6381_TestPattern         m_BER_Test_Pattern;         // indicates the pattern of LFSR.
    AVL6381_LFSR_FbBit          m_BER_FB_Inversion;         // indicates LFSR feedback bit inversion.
    AVL6381_uint32                  m_LFSR_Sync;            // indicates the LFSR synchronization status.
    AVL6381_uint32                  m_Time_Threshold_Ms;    // used to set time interval for auto error statistics.
    AVL6381_uint32                  m_Number_Threshold_Byte;// used to set the received byte number threshold for auto error statistics.
}AVL6381_ErrorStat_Info;

typedef enum _AVL6381_I2CREADWRITE_STATUS
{    
    AVL6381_I2CREADWRITE_OK    =0,
    AVL6381_I2CREADWRITE_ERROR
}AVL6381_I2CREADWRITE_STATUS;


typedef struct _AVL6381_CFG
{
    AVL6381_uint16 m_SlaveAddr;                 // Seven bit I2C slave address of the device. 
    AVL6381_uchar ucStartupMode;
    AVL6381_uchar m_current_demod_mode;         // Indicates the current standard mode that the device is configured to operate in. It is set each time a lock channel operation is requested on the part-supported standard.
    AVL6381_puchar m_pInitialData;              // Store the start address of patch array
    AVL6381_uint32 m_DemodFrequency_Hz;         // The demodulator internal clock frequency in Hz. 
    AVL6381_uint32 m_SDRAMFrequency_Hz;         // The SDRAM clock frequency in Hz. 
    AVL6381_uint32 m_FECFrequency_Hz;           // The FEC clock frequency in Hz. 
    AVL6381_uint32 m_RefFrequency_Hz;           // The reference clock frequency in units of Hz.
    AVL6381_uint32 m_ADCFrequency_Hz;           // ADC clock frequency from PLL
    struct semaphore *  m_semRx;                // A semaphore used to protect the receiver.
	AVL6381_MPEGConfig  m_MPEG_Info;            // Stores the MPEG configuration structure information  for the whole chip.
    AVL6381_InputConfig m_InputSignal_Info;     // The info of signal which is inputted into demod
    AVL6381_ErrorStat_Info m_ErrorStat_Info;
}AVL6381_CFG;

// Contains variables for storing error statistics used in the BER and PER calculations.
typedef  struct AVL6381_ErrorStats
{
    AVL6381_uint16 m_LFSR_Sync;       // Indicates whether the receiver is synchronized with the transmitter generating the BER test pattern.
    AVL6381_uint16 m_LostLock;        // Indicates whether the receiver has lost lock since the BER/PER measurement was started.
    AVL6381_uint64 m_SwCntNumBits;    // A software counter which stores the number of bits which have been received.
    AVL6381_uint64 m_SwCntBitErrors;  // A software counter which stores the number of bit errors which have been detected.
    AVL6381_uint64 m_NumBits;         // The total number of bits which have been received.
    AVL6381_uint64 m_BitErrors;       // The total number of bit errors which have been detected.
    AVL6381_uint64 m_SwCntNumPkts;    // A software counter which stores the number of packets which have been received.
    AVL6381_uint64 m_SwCntPktErrors;  // A software counter which stores the number of packet errors which have been detected.
    AVL6381_uint64 m_NumPkts;         // The total number of packets which have been received.
    AVL6381_uint64 m_PktErrors;       // The total number of packet errors which have been detected.
    AVL6381_uint32 m_BER;             // The bit error rate scaled by 1e9.
    AVL6381_uint32 m_PER;             // The packet error rate scaled by 1e9.
}AVL6381_ErrorStats;

// Defines the AVL6381 device running levels.
typedef enum AVL6381_RunningLevel
{
    AVL6381_RL_HALT =  0,       // = 0 Halt running level.
    AVL6381_RL_NORMAL = 2       // = 2 Normal running level.
}AVL6381_RunningLevel;


typedef enum AVL_SpectrumInversion
{
    AVL_SPECTRUM_NORMAL = 0,
    AVL_SPECTRUM_INVERTED = 1,
    AVL_SPECTRUM_AUTO = 2
}AVL_SpectrumInversion;

typedef enum AVL6381_DTMB_CarrierMode
{
    RX_MULTI_CARRIER = 0,
    RX_SINGLE_CARRIER = 1
}AVL6381_DTMB_CarrierMode;

typedef enum AVL6381_DTMB_FrameHeaderMode
{
    RX_PN420 = 1,
    RX_PN595 = 2,
    RX_PN945 = 3
}AVL6381_DTMB_FrameHeaderMode;

typedef enum AVL6381_DTMB_InterleaveMode
{
    RX_INTERLEAVE_240 = 0,
    RX_INTERLEAVE_720 = 1
}AVL6381_DTMB_InterleaveMode;

typedef enum AVL6381_DTMB_NR_Mode
{
    RX_NR_OFF = 0,
    RX_NR_ON = 1
}AVL6381_DTMB_NR_Mode;

typedef enum AVL6381_DTMB_FEC_Rate
{
    RX_FEC_2_5 = 0,
    RX_FEC_3_5 = 1,
    RX_FEC_4_5 = 2
}AVL6381_DTMB_FEC_Rate;

typedef enum AVL6381_DTMB_ModulationMode
{
    RX_DTMB_4QAM = 0,
    RX_DTMB_16QAM = 1,
    RX_DTMB_32QAM = 2,
    RX_DTMB_64QAM = 3,
}AVL6381_DTMB_ModulationMode;


typedef struct AVL6381_DTMB_SignalInfo
{
    AVL6381_DTMB_CarrierMode m_CarrierMode;
    AVL6381_DTMB_FEC_Rate m_CodeRate;
    AVL6381_DTMB_ModulationMode m_Modulation;
    AVL6381_DTMB_NR_Mode m_MappingMode;
    AVL6381_DTMB_InterleaveMode m_InterleaveMode;
    AVL6381_DTMB_FrameHeaderMode m_HeaderMode;
}AVL6381_DTMB_SignalInfo;


//used for pass on and pass off
#define AVL6381_BIT_REPEATER_ENABLE     1
#define AVL6381_BIT_REPEATER_SEL        2
#define AVL6381_BIT_REPEATER_WAIT_ACK   4

#define AVL6381_DEMOD_MODE_DTMB     0
#define AVL6381_DEMOD_MODE_DVBC     1
#define AVL6381_DEMOD_MODE_UNKNOWN  2

/*
**AVL6381 Config Registers Addr
*/
#define rc_AVL6381_DEMOD_config_regs_base (0x00000204)
#define rc_AVL6381_fw_command_iaddr                      (rc_AVL6381_DEMOD_config_regs_base + 0x00000000)
#define rc_AVL6381_sdram_test_return_iaddr               (rc_AVL6381_DEMOD_config_regs_base + 0x00000004)
#define rc_AVL6381_sdram_test_result_iaddr               (rc_AVL6381_DEMOD_config_regs_base + 0x00000008)
#define rc_AVL6381_sdram_config_iaddr                    (rc_AVL6381_DEMOD_config_regs_base + 0x0000000c)
#define rc_AVL6381_sdram_timing_config_0_iaddr           (rc_AVL6381_DEMOD_config_regs_base + 0x00000010)
#define rc_AVL6381_sdram_timing_config_1_iaddr           (rc_AVL6381_DEMOD_config_regs_base + 0x00000014)
#define rc_AVL6381_get_config_from_tps_caddr             (rc_AVL6381_DEMOD_config_regs_base + 0x0000001b)
#define rc_AVL6381_demod_mode_caddr                      (rc_AVL6381_DEMOD_config_regs_base + 0x00000022)

#define core_io__base 0x00108000
#define core_io__chip_id__offset                    0x00000004
#define core_io__mpeg_bus_tri_enb__offset           0x00000030
#define core_io__rfagc_tri_enb__offset              0x00000034

#define adc_wrapper__base                           0x00104000
#define adc_wrapper__adc_sel_control__offset        0x0000003c

#define clock_reset__base                           0x00100000
#define clock_reset__reset_register__offset         0x00000000
#define clock_reset__dll_init__offset               0x00000008
#define clock_reset__dll_deglitch_mode__offset      0x00000010
#define clock_reset__dll_dly_control_lin__offset    0x00000014
#define clock_reset__dll_out_phase__offset          0x00000018
#define clock_reset__dll_rd_phase__offset           0x0000001c
#define clock_reset__pll_aux_clkr__offset           0x00000080
#define clock_reset__pll_aux_clkf__offset           0x00000084
#define clock_reset__pll_aux_od__offset             0x00000088
#define clock_reset__pll_aux_od2__offset            0x0000008c
#define clock_reset__pll_aux_od3__offset            0x00000090
#define clock_reset__pll_aux_bwadj__offset          0x00000094
#define clock_reset__pll_aux_reg_mode__offset       0x0000009c
#define clock_reset__pll_demod_clkr__offset         0x000000c0
#define clock_reset__pll_demod_clkf__offset         0x000000c4
#define clock_reset__pll_demod_od__offset           0x000000c8
#define clock_reset__pll_demod_bwadj__offset        0x000000d4

#define c306_top__base                              0x00110800
#define c306_top__srst__offset                      0x00000040

#define osprey_esm__base                            0x00149100
#define osprey_esm__esm_cntrl__offset               0x00000004
#define osprey_esm__srst__offset                    0x00000000
#define osprey_esm__esm_cntrl__offset               0x00000004
#define osprey_esm__ber_err_cnt__offset             0x00000008
#define osprey_esm__byte_num__offset                0x0000000c
#define osprey_esm__packet_err_cnt__offset          0x00000010
#define osprey_esm__packet_num__offset              0x00000014
#define osprey_esm__ber_err_rollover_cnt__offset    0x00000018
#define osprey_esm__packet_err_rollover_cnt__offset 0x0000001c
#define osprey_esm__byte_rollover_num__offset       0x00000020
#define osprey_esm__packet_rollover_num__offset     0x00000024
#define osprey_esm__tick_clear_req__offset          0x00000028
#define osprey_esm__timetick1_bytetick0__offset     0x0000002c
#define osprey_esm__time_tick_low__offset           0x00000030
#define osprey_esm__time_tick_high__offset          0x00000034
#define osprey_esm__byte_tick_low__offset           0x00000038
#define osprey_esm__byte_tick_high__offset          0x0000003c
#define osprey_esm__ber_err_num_low__offset         0x00000040
#define osprey_esm__packet_err_num_low__offset      0x00000044
#define osprey_esm__byte_num_low__offset            0x00000048
#define osprey_esm__packet_num_low__offset          0x0000004c
#define osprey_esm__ber_err_num_high__offset        0x00000050
#define osprey_esm__packet_err_num_high__offset     0x00000054
#define osprey_esm__byte_num_high__offset           0x00000058
#define osprey_esm__packet_num_high__offset         0x0000005c
#define osprey_esm__auto1_manual0_mode__offset      0x00000060
#define osprey_esm__total_null_pkt_cnt__offset      0x00000064
#define osprey_esm__esm_cntrl_1__offset             0x00000068
#define osprey_esm__pid_mask__offset                0x0000006c

#define osprey_tuner_i2c__base                      0x00118000
#define osprey_tuner_i2c__tuner_i2c_srst__offset    0x00000000
#define osprey_tuner_i2c__tuner_i2c_cntrl__offset   0x00000004
#define osprey_tuner_i2c__tuner_hw_i2c_bit_rpt_clk_div__offset   0x00000018
#define osprey_tuner_i2c__tuner_hw_i2c_bit_rpt_cntrl__offset     0x0000001c

#define beryl_cpu__base                             0x00110000
#define beryl_cpu__cpu_ddc_srst_b_reg__offset       0x00000084
#define beryl_cpu__cpu_valu_srst_b_reg__offset      0x00000088

#define beryl_ddc__base                             0x00160000
#define beryl_ddc__rx_if_aagc_gain__offset          0x0000004c


#define hw_AVL6381_rom_ver                          0x00040000

#define hw_AVL6381_mpeg_bus_tri_enb                 (core_io__base + core_io__mpeg_bus_tri_enb__offset)
#define hw_AVL6381_chip_id                          (core_io__base + core_io__chip_id__offset)

#define hw_AVL6381_rfagc_tri_enb                    (core_io__base + core_io__rfagc_tri_enb__offset)

#define hw_AVL6381_c306_top_srst                    (c306_top__base + c306_top__srst__offset)

#define hw_AVL6381_adc_clock_sel                    (adc_wrapper__base + adc_wrapper__adc_sel_control__offset)

#define hw_AVL6381_clock_reset                      (clock_reset__base + clock_reset__reset_register__offset)
#define hw_AVL6381_dll_init                         (clock_reset__base + clock_reset__dll_init__offset)
#define hw_AVL6381_dll_dly_control_lin              (clock_reset__base + clock_reset__dll_dly_control_lin__offset)
#define hw_AVL6381_dll_out_phase                    (clock_reset__base + clock_reset__dll_out_phase__offset)
#define hw_AVL6381_dll_rd_phase                     (clock_reset__base + clock_reset__dll_rd_phase__offset)
#define hw_AVL6381_pll_aux_clkr                     (clock_reset__base + clock_reset__pll_aux_clkr__offset)
#define hw_AVL6381_pll_aux_clkf                     (clock_reset__base + clock_reset__pll_aux_clkf__offset)
#define hw_AVL6381_pll_aux_od                       (clock_reset__base + clock_reset__pll_aux_od__offset)
#define hw_AVL6381_pll_aux_od2                      (clock_reset__base + clock_reset__pll_aux_od2__offset)
#define hw_AVL6381_pll_aux_od3                      (clock_reset__base + clock_reset__pll_aux_od3__offset)
#define hw_AVL6381_pll_aux_bwadj                    (clock_reset__base + clock_reset__pll_aux_bwadj__offset)
#define hw_AVL6381_pll_demod_clkr                   (clock_reset__base + clock_reset__pll_demod_clkr__offset)
#define hw_AVL6381_pll_demod_clkf                   (clock_reset__base + clock_reset__pll_demod_clkf__offset)
#define hw_AVL6381_pll_demod_od                     (clock_reset__base + clock_reset__pll_demod_od__offset)
#define hw_AVL6381_pll_demod_bwadj                  (clock_reset__base + clock_reset__pll_demod_bwadj__offset)
#define hw_AVL6381_dll_deglitch_mode                (clock_reset__base + clock_reset__dll_deglitch_mode__offset)


#define hw_AVL6381_esm_cntrl                        (osprey_esm__base + osprey_esm__esm_cntrl__offset)
#define hw_AVL6381_byte_num                         (osprey_esm__base + osprey_esm__byte_num__offset)
#define hw_AVL6381_ber_err_cnt                      (osprey_esm__base + osprey_esm__ber_err_cnt__offset)
#define hw_AVL6381_packet_num                       (osprey_esm__base + osprey_esm__packet_num__offset)
#define hw_AVL6381_packet_err_cnt                   (osprey_esm__base + osprey_esm__packet_err_cnt__offset)
#define hw_AVL6381_auto1_manual0_mode               (osprey_esm__base + osprey_esm__auto1_manual0_mode__offset)
#define hw_AVL6381_tick_clear_req                   (osprey_esm__base + osprey_esm__tick_clear_req__offset)
#define hw_AVL6381_timetick1_bytetick0              (osprey_esm__base + osprey_esm__timetick1_bytetick0__offset)
#define hw_AVL6381_time_tick_low                    (osprey_esm__base + osprey_esm__time_tick_low__offset)
#define hw_AVL6381_time_tick_high                   (osprey_esm__base + osprey_esm__time_tick_high__offset)
#define hw_AVL6381_byte_tick_low                    (osprey_esm__base + osprey_esm__byte_tick_low__offset)
#define hw_AVL6381_byte_tick_high                   (osprey_esm__base + osprey_esm__byte_tick_high__offset)


#define hw_AVL6381_tuner_i2c_srst                   (osprey_tuner_i2c__base + osprey_tuner_i2c__tuner_i2c_srst__offset)
#define hw_AVL6381_tuner_hw_i2c_bit_rpt_clk_div     (osprey_tuner_i2c__base + osprey_tuner_i2c__tuner_hw_i2c_bit_rpt_clk_div__offset)
#define hw_AVL6381_tuner_hw_i2c_bit_rpt_cntrl       (osprey_tuner_i2c__base + osprey_tuner_i2c__tuner_hw_i2c_bit_rpt_cntrl__offset)
#define hw_AVL6381_tuner_i2c_cntrl                  (osprey_tuner_i2c__base + osprey_tuner_i2c__tuner_i2c_cntrl__offset)

#define hw_AVL6381_cpu_ddc_srst                     (beryl_cpu__base + beryl_cpu__cpu_ddc_srst_b_reg__offset)
#define hw_AVL6381_cpu_valu_srst                    (beryl_cpu__base + beryl_cpu__cpu_valu_srst_b_reg__offset)

#define hw_AVL6381_if_aagc_gain                     (beryl_ddc__base + beryl_ddc__rx_if_aagc_gain__offset)

#define rs_AVL6381_core_ready_word_iaddr            0x000000a0
#define rs_AVL6381_current_active_mode_iaddr        0x200
#define rc_mode_configuration_saddr                 0x2d0000
#define rc_mode_switch_saddr                        0x2d0002
#define hw_AVL6381_digital_core_reset               0x38fffc
/*
**DTMB Mode Registers Addr
*/
#define rc_AVL6381_DTMB_config_regs_base (0x00000300)
#define rc_AVL6381_DTMB_symbol_rate_Hz_iaddr             (rc_AVL6381_DTMB_config_regs_base + 0x00000000)
#define rc_AVL6381_DTMB_sample_rate_Hz_iaddr             (rc_AVL6381_DTMB_config_regs_base + 0x00000004)
#define rs_AVL6381_DTMB_fixed_header_caddr               (rs_AVL6381_DTMB_status_regs_base + 0x00000009)
#define rc_AVL6381_DTMB_ifagc_pol_caddr                  (rc_AVL6381_DTMB_config_regs_base + 0x0000000b)
#define rc_AVL6381_DTMB_iq_comp_enable_caddr             (rc_AVL6381_DTMB_config_regs_base + 0x00000019)
#define rc_AVL6381_DTMB_ddc_input_select_iaddr           (rc_AVL6381_DTMB_config_regs_base + 0x00000020)
#define rc_AVL6381_DTMB_tuner_type_caddr                 (rc_AVL6381_DTMB_config_regs_base + 0x00000020)
#define rc_AVL6381_DTMB_input_format_caddr               (rc_AVL6381_DTMB_config_regs_base + 0x00000021)
#define rc_AVL6381_DTMB_spectrum_invert_caddr            (rc_AVL6381_DTMB_config_regs_base + 0x00000022)
#define rc_AVL6381_DTMB_input_select_caddr               (rc_AVL6381_DTMB_config_regs_base + 0x00000023)
#define rc_AVL6381_DTMB_nom_carrier_freq_Hz_iaddr        (rc_AVL6381_DTMB_config_regs_base + 0x00000024)
#define rc_AVL6381_DTMB_equalizer_control_iaddr          (rc_AVL6381_DTMB_config_regs_base + 0x00000028)
#define rc_AVL6381_DTMB_equalizer_mode_caddr             (rc_AVL6381_DTMB_config_regs_base + 0x0000002b)
#define rc_AVL6381_DTMB_dmd_clk_Hz_iaddr                 (rc_AVL6381_DTMB_config_regs_base + 0x00000038)
#define rc_AVL6381_DTMB_sdram_clk_Hz_iaddr               (rc_AVL6381_DTMB_config_regs_base + 0x0000003c)
#define rc_AVL6381_DTMB_mpeg_seq_caddr                   (rc_AVL6381_DTMB_config_regs_base + 0x00000050)
#define rc_AVL6381_DTMB_serial_outpin_sel_caddr          (rc_AVL6381_DTMB_config_regs_base + 0x00000051)
#define rc_AVL6381_DTMB_mpeg_posedge_caddr               (rc_AVL6381_DTMB_config_regs_base + 0x00000052)
#define rc_AVL6381_DTMB_mpeg_serial_caddr                (rc_AVL6381_DTMB_config_regs_base + 0x00000053)
#define rc_AVL6381_DTMB_pkt_err_pol_caddr                (rc_AVL6381_DTMB_config_regs_base + 0x00000054)
#define rc_AVL6381_DTMB_ts0_tsp1_caddr                   (rc_AVL6381_DTMB_config_regs_base + 0x00000057)
#define rc_AVL6381_DTMB_mpeg_ts_error_bit_en_caddr       (rc_AVL6381_DTMB_config_regs_base + 0x00000078)
#define rs_AVL6381_DTMB_demod_status_caddr               (rs_AVL6381_DTMB_status_regs_base + 0x00000080)
#define rc_AVL6381_DTMB_fec_clk_Hz_iaddr                 (rc_AVL6381_DTMB_config_regs_base + 0x00000084)
#define rc_AVL6381_DTMB_cont_mpegclk_en_caddr            (rc_AVL6381_DTMB_config_regs_base + 0x0000008b)
#define rc_AVL6381_DTMB_cntns_pkt_para_rate_frac_n_iaddr (rc_AVL6381_DTMB_config_regs_base + 0x0000008c)
#define rc_AVL6381_DTMB_cntns_pkt_para_rate_frac_d_iaddr (rc_AVL6381_DTMB_config_regs_base + 0x00000090)
#define rc_AVL6381_DTMB_cntns_pkt_seri_rate_frac_n_iaddr (rc_AVL6381_DTMB_config_regs_base + 0x00000094)
#define rc_AVL6381_DTMB_cntns_pkt_seri_rate_frac_d_iaddr (rc_AVL6381_DTMB_config_regs_base + 0x00000098)
#define rs_AVL6381_DTMB_signal_strength_iaddr            (rs_AVL6381_DTMB_status_regs_base + 0x0000009c)
#define rc_AVL6381_DTMB_ADC_I0_Q1_caddr                  (rc_AVL6381_DTMB_config_regs_base + 0x000001d7)
#define rc_AVL6381_DTMB_mpeg_clk_phase_iaddr             (rc_AVL6381_DTMB_config_regs_base + 0x000001e0)
#define rc_AVL6381_DTMB_tps_manual_pn_caddr              (rc_AVL6381_DTMB_config_regs_base + 0x000001e4)
#define rc_AVL6381_DTMB_pn_chk_en_caddr                  (rc_AVL6381_DTMB_config_regs_base + 0x000001e5)
#define rc_AVL6381_DTMB_seri_sync_1_pulse_caddr          (rc_AVL6381_DTMB_config_regs_base + 0x000001e6)
#define rc_AVL6381_DTMB_mpeg_valid_inv_caddr             (rc_AVL6381_DTMB_config_regs_base + 0x000001e7)

#define rs_AVL6381_DTMB_status_regs_base (0x000000a4)
#define rs_AVL6381_DTMB_lock_status_iaddr                (rs_AVL6381_DTMB_status_regs_base + 0x00000000)
#define rs_AVL6381_DTMB_lost_lock_caddr                  (rs_AVL6381_DTMB_status_regs_base + 0x00000001)
#define rs_AVL6381_DTMB_fec_lock_caddr                   (rs_AVL6381_DTMB_status_regs_base + 0x00000002)
#define rs_AVL6381_DTMB_mode_status_iaddr                (rs_AVL6381_DTMB_status_regs_base + 0x00000004)
#define rs_AVL6381_DTMB_interleaver_mode_caddr           (rs_AVL6381_DTMB_status_regs_base + 0x00000004)
#define rs_AVL6381_DTMB_carrier_mode_caddr               (rs_AVL6381_DTMB_status_regs_base + 0x00000005)
#define rs_AVL6381_DTMB_fec_rate_caddr                   (rs_AVL6381_DTMB_status_regs_base + 0x00000006)
#define rs_AVL6381_DTMB_header_mode_caddr                (rs_AVL6381_DTMB_status_regs_base + 0x00000007)
#define rs_AVL6381_DTMB_modulation_status_iaddr          (rs_AVL6381_DTMB_status_regs_base + 0x00000008)
#define rs_AVL6381_DTMB_nr_mode_caddr                    (rs_AVL6381_DTMB_status_regs_base + 0x0000000a)
#define rs_AVL6381_DTMB_constellation_mode_caddr         (rs_AVL6381_DTMB_status_regs_base + 0x0000000b)
#define rs_AVL6381_DTMB_patch_ver_iaddr                  (rs_AVL6381_DTMB_status_regs_base + 0x0000004c)
#define rs_AVL6381_DTMB_carrier_freq_bin_Hz_iaddr        (rs_AVL6381_DTMB_status_regs_base + 0x00000050)
#define rs_AVL6381_DTMB_demod_lock_status_iaddr          (rs_AVL6381_DTMB_status_regs_base + 0x00000068)
#define rs_AVL6381_DTMB_snr_dB_x100_avg_saddr            (rs_AVL6381_DTMB_status_regs_base + 0x00000078)
#define rs_AVL6381_DTMB_num_early_reacq_saddr            (rs_AVL6381_DTMB_status_regs_base + 0x000000a2)

#define hw_AVL6381_DTMB_cfo_freq_est                    0x001600C0
#define hw_AVL6381_DTMB_scatter_data                    0x002fdc00

/*
**DVBC Mode Registers Addr
*/
#define rc_AVL6381_DVBC_config_regs_base (0x00000558)
#define rc_AVL6381_DVBC_symbol_rate_Hz_iaddr             (rc_AVL6381_DVBC_config_regs_base + 0x00000000)
#define rc_AVL6381_DVBC_sample_rate_Hz_iaddr             (rc_AVL6381_DVBC_config_regs_base + 0x00000004)
#define rc_AVL6381_DVBC_dmd_clk_Hz_iaddr                 (rc_AVL6381_DVBC_config_regs_base + 0x00000008)
#define rc_AVL6381_DVBC_sdram_clk_Hz_iaddr               (rc_AVL6381_DVBC_config_regs_base + 0x0000000c)
#define rc_AVL6381_DVBC_mpeg_seq_caddr                   (rc_AVL6381_DVBC_config_regs_base + 0x00000014)
#define rc_AVL6381_DVBC_serial_outpin_sel_caddr          (rc_AVL6381_DVBC_config_regs_base + 0x00000015)
#define rc_AVL6381_DVBC_mpeg_posedge_caddr               (rc_AVL6381_DVBC_config_regs_base + 0x00000016)
#define rc_AVL6381_DVBC_mpeg_serial_caddr                (rc_AVL6381_DVBC_config_regs_base + 0x00000017)
#define rc_AVL6381_DVBC_pkt_err_pol_caddr                (rc_AVL6381_DVBC_config_regs_base + 0x00000018)
#define rc_AVL6381_DVBC_ts0_tsp1_caddr                   (rc_AVL6381_DVBC_config_regs_base + 0x0000001b)
#define rc_AVL6381_DVBC_mpeg_ts_error_bit_en_caddr       (rc_AVL6381_DVBC_config_regs_base + 0x00000020)
#define rc_AVL6381_DVBC_ddc_input_select_iaddr           (rc_AVL6381_DVBC_config_regs_base + 0x00000024)
#define rc_AVL6381_DVBC_tuner_type_caddr                 (rc_AVL6381_DVBC_config_regs_base + 0x00000024)
#define rc_AVL6381_DVBC_input_format_caddr               (rc_AVL6381_DVBC_config_regs_base + 0x00000025)
#define rc_AVL6381_DVBC_spectrum_invert_caddr            (rc_AVL6381_DVBC_config_regs_base + 0x00000026)
#define rc_AVL6381_DVBC_input_select_caddr               (rc_AVL6381_DVBC_config_regs_base + 0x00000027)
#define rc_AVL6381_DVBC_if_freq_Hz_iaddr                 (rc_AVL6381_DVBC_config_regs_base + 0x00000028)
#define rc_AVL6381_DVBC_qam_mode_iaddr                   (rc_AVL6381_DVBC_config_regs_base + 0x0000002c)
#define rc_AVL6381_DVBC_ifagc_pol_caddr                  (rc_AVL6381_DVBC_config_regs_base + 0x00000047)
#define rc_AVL6381_DVBC_fec_clk_Hz_iaddr                 (rc_AVL6381_DVBC_config_regs_base + 0x00000050)
#define rc_AVL6381_DVBC_get_btr_crl_iaddr                (rc_AVL6381_DVBC_config_regs_base + 0x00000080)
#define rc_AVL6381_DVBC_cont_mpegclk_en_2_saddr          (rc_AVL6381_DVBC_config_regs_base + 0x00000096)
#define rc_AVL6381_DVBC_cntns_pkt_para_rate_frac_n_32bit_iaddr (rc_AVL6381_DVBC_config_regs_base + 0x00000098)
#define rc_AVL6381_DVBC_cntns_pkt_para_rate_frac_d_32bit_iaddr (rc_AVL6381_DVBC_config_regs_base + 0x0000009c)
#define rc_AVL6381_DVBC_cntns_pkt_seri_rate_frac_n_32bit_iaddr (rc_AVL6381_DVBC_config_regs_base + 0x000000a0)
#define rc_AVL6381_DVBC_cntns_pkt_seri_rate_frac_d_32bit_iaddr (rc_AVL6381_DVBC_config_regs_base + 0x000000a4)
#define rc_AVL6381_DVBC_ADC_I0_Q1_caddr                  (rc_AVL6381_DVBC_config_regs_base + 0x000001ef)
#define rc_AVL6381_DVBC_mpeg_clk_phase_iaddr             (rc_AVL6381_DVBC_config_regs_base + 0x000001f0)
#define rc_AVL6381_DVBC_seri_sync_1_pulse_caddr          (rc_AVL6381_DVBC_config_regs_base + 0x000001f6)
#define rc_AVL6381_DVBC_mpeg_valid_inv_caddr             (rc_AVL6381_DVBC_config_regs_base + 0x000001f7)
#define rc_AVL6381_DVBC_ddc__clks_per_2x_symbol_dvbc_iaddr (rc_AVL6381_DVBC_config_regs_base + 0x0000019c)

#define rs_AVL6381_DVBC_status_regs_base (0x000001a0)
#define rs_AVL6381_DVBC_lock_status_iaddr                (rs_AVL6381_DVBC_status_regs_base + 0x00000000)
#define rs_AVL6381_DVBC_lost_lock_saddr                  (rs_AVL6381_DVBC_status_regs_base + 0x00000002)
#define rs_AVL6381_DVBC_mode_status_iaddr                (rs_AVL6381_DVBC_status_regs_base + 0x00000004)
#define rs_AVL6381_DVBC_demod_lock_indicator_iaddr       (rs_AVL6381_DVBC_status_regs_base + 0x00000008)
#define rs_AVL6381_DVBC_snr_dB_x100_saddr                (rs_AVL6381_DVBC_status_regs_base + 0x0000000e)
#define rs_AVL6381_DVBC_patch_ver_iaddr                  (rs_AVL6381_DVBC_status_regs_base + 0x00000010)
#define rs_AVL6381_DVBC_carrier_freq_offset_Hz_iaddr     (rs_AVL6381_DVBC_status_regs_base + 0x00000014)
#define rs_AVL6381_DVBC_no_signal_input_iaddr            (rs_AVL6381_DVBC_status_regs_base + 0x00000018)
#define rs_AVL6381_DVBC_aagc_gain_iaddr                  (rs_AVL6381_DVBC_status_regs_base + 0x00000048)
#define rs_AVL6381_DVBC_berBeforRS_iaddr                 (rs_AVL6381_DVBC_status_regs_base + 0x0000004c)

#define hw_AVL6381_DVBC_scatter_data         0x002edd70
/*
**avl6381 registers addr end
*/

extern AVL6381_PLL_Conf gAVL6381PLLConfig[8];
extern AVL6381_SDRAM_Conf gAVL6381SDRAMConfig;
extern AVL6381_CFG gsAVL6381_Config;;
extern AVL6381_ErrorStats gsAVL6381ErrorStat;
extern AVL6381_AGCPola geAVL6381AGCPola;
extern AVL6381_MPEG_SerialDataPin geAVL6381SerialPin;
extern AVL6381_MPEG_SERIAL_ORDER geAVL6381SerialOrder;
extern AVL6381_MPEG_SERIAL_SYNC geAVL6381SerialSync;
extern AVL6381_MPEG_ErrorBit geAVL6381ErrorBit;
extern AVL6381_MPEG_ErrorBitPola geAVL6381ErrorPola;
extern AVL6381_MPEG_ValidPola geAVL6381ValidPola;
extern AVL6381_MPEG_Packet_Len geAVL6381PacketLen;
extern AVL6381_uint32 guiDTMBSerialMPEGContinuousHz;
extern AVL6381_uint32 guiDVBCSerialMPEGContinuousHz;
extern AVL6381_uint32 guiParallelMPEGContinuousHz;
extern AVL6381_uint32 guiDTMBSymbolRateSps;

extern AVL6381_uchar AVL6381_FwData_DTMB[];
extern AVL6381_uchar AVL6381_freezeData_DTMB[];
//extern AVL6381_uchar gAVL6381freezeflag;
//extern AVL6381_uchar AVL6381_FwData_DVBC[];

extern AVL6381_ErrorCode SetPLL_6381(AVL6381_PLL_Conf *pPLL_Conf, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DigitalCoreReset_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode IBase_Initialize_6381(AVL6381_PLL_Conf *pPLL_Conf, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode CheckChipReady_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode GetRxOP_Status_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode SendRxOP_6381(AVL6381_uchar ucOpCmd, struct AVL6381_Chip * pAVLChip);
extern AVL6381_uint32 	 ConvertNsToSDRAM_Clocks(AVL6381_uint32 SDRAM_ClkFrequency_Hz, AVL6381_uint16 Value_ns);
extern AVL6381_ErrorCode InitSDRAM_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode IRx_Initialize_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode SetMpegMode_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode SetMpegSerialOrder_6381(AVL6381_MPEG_SERIAL_ORDER enumSerialOrder, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode SetMpegSerialSyncPulse_6381(AVL6381_MPEG_SERIAL_SYNC enumSerialSyncPulse, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode SetMpegErrorBit_6381(AVL6381_MPEG_ErrorBit enumErrorBit, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode SetMpegErrorPola_6381(AVL6381_MPEG_ErrorBitPola enumErrorPola, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode SetMpegValidPola_6381(AVL6381_MPEG_ValidPola enumValidPola, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode SetMpegPacketLen_6381(AVL6381_MPEG_Packet_Len enumPacketLen, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode EnableMpegOutput_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode TunerI2C_Initialize_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode SetAGCPola_6381(AVL6381_AGCPola enumAGCPola, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode EnableAGC_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode ResetPER_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode ResetErrorStat_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode InitErrorStat_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode SetMpegSerialPin_6381(AVL6381_MPEG_SerialDataPin enumSerialPin, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode AVL6381_GetLockStatus(AVL6381_puint32 puiLockStatus, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode AVL6381_Halt(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode AVL6381_AutoLock(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode AVL6381_SetMode(AVL6381_uchar mode, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode AVL6381_NoSignalDetection(AVL6381_uint32 *puiNoSignal,struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode AVL6381_GetSQI(AVL6381_puint32 puiSignalQuality, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode AVL6381_GetBER(AVL6381_puint32 puiBER_x10e9, struct AVL6381_Chip * pAVLChip);


extern AVL6381_ErrorCode DTMB_SetSpectrumPola_6381(AVL6381_SpectrumInversion enumSpectrumPola, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_InitRx_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_InitADC_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_SetSymbolRate_6381(AVL6381_uint32 uiSymbolRateHz, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_SetMpegMode_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_SetMpegSerialPin_6381(AVL6381_MPEG_SerialDataPin enumSerialPin, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_SetMpegSerialOrder_6381(AVL6381_MPEG_SERIAL_ORDER enumSerialOrder, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_SetMpegSerialSyncPulse_6381(AVL6381_MPEG_SERIAL_SYNC enumSerialSyncPulse, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_SetMpegErrorBit_6381(AVL6381_MPEG_ErrorBit enumErrorBit, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_SetMpegErrorPola_6381(AVL6381_MPEG_ErrorBitPola enumErrorPola, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_SetMpegValidPola_6381(AVL6381_MPEG_ValidPola enumValidPola, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_SetMpegPacketLen_6381(AVL6381_MPEG_Packet_Len enumPacketLen, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_EnableMpegContinuousMode_6381(AVL6381_uint32 uiClockFreq_Hz, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_SetAGCPola_6381(AVL6381_AGCPola enumAGCPola, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DTMB_GetLockStatus_6381(AVL6381_puint32 pucLockStatus, struct AVL6381_Chip * pAVLChip);

extern AVL6381_ErrorCode AVL6381_DTMB_GetSignalInfo(AVL6381_DTMB_SignalInfo *pSignalInfo, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode AVL6381_DTMB_GetSNR(AVL6381_puint32 puiSNR_db, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode AVL6381_DTMB_GetSignalLevel(AVL6381_puint32 puiSignalLevel, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode AVL6381_DTMB_Get_CarrierFreq_Offset(AVL6381_puint32 puiFreq, AVL6381_pint32 piFreqOffsetHz, struct AVL6381_Chip * pAVLChip);

#if 0
extern AVL6381_ErrorCode DVBC_InitRx_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DVBC_InitADC_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DVBC_SetMpegMode_6381(struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DVBC_SetMpegSerialOrder_6381(AVL6381_MPEG_SERIAL_ORDER enumSerialOrder, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DVBC_SetMpegSerialSyncPulse_6381(AVL6381_MPEG_SERIAL_SYNC enumSerialSyncPulse, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DVBC_SetMpegErrorBit_6381(AVL6381_MPEG_ErrorBit enumErrorBit, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DVBC_SetMpegErrorPola_6381(AVL6381_MPEG_ErrorBitPola enumErrorPola, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DVBC_SetMpegValidPola_6381(AVL6381_MPEG_ValidPola enumValidPola, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DVBC_SetMpegPacketLen_6381(AVL6381_MPEG_Packet_Len enumPacketLen, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DVBC_EnableMpegContinuousMode_6381(AVL6381_uint32 uiClockFreq_Hz, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DVBC_GetLockStatus_6381(AVL6381_puint32 puiLockStatus, struct AVL6381_Chip * pAVLChip);
extern AVL6381_ErrorCode DVBC_SetAGCPola_6381(AVL6381_AGCPola enumAGCPola);
#endif

#endif


