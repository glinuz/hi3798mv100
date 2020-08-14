/*****************************************************************************************
 *
 * FILE NAME          : MxL101SF_PhyDefs.h
 * 
 * AUTHOR             : Brenndon Lee
 *
 * DATE CREATED       : 5/18/2009
 *
 * DESCRIPTION        : This file contains MxL101SF common control register 
 *                      definitions
 *
 *****************************************************************************************
 *                Copyright (c) 2006, MaxLinear, Inc.
 ****************************************************************************************/

#ifndef __MXL101SF_PHY_DEFS_H__
#define __MXL101SF_PHY_DEFS_H__

/*****************************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
*****************************************************************************************/
#include "MaxLinearDataTypes.h"
#include "MxL101SF_PhyCtrlApi.h"

/*****************************************************************************************
    Macros
*****************************************************************************************/

#define CHIP_ID_REG                 0xFC     
#define TOP_CHIP_REV_ID_REG         0xFA     

#define V6_SNR_RB_LSB_REG            0x27
#define V6_SNR_RB_MSB_REG            0x28

#define V6_N_ACCUMULATE_REG          0x11    
#define V6_RS_AVG_ERRORS_LSB_REG     0x2C    
#define V6_RS_AVG_ERRORS_MSB_REG     0x2D    

#define V6_IRQ_STATUS_REG            0x24
#define  IRQ_MASK_FEC_LOCK       0x10

#define V6_SYNC_LOCK_REG             0x28
#define SYNC_LOCK_MASK           0x10

#define V6_RS_LOCK_DET_REG           0x28
#define  RS_LOCK_DET_MASK        0x08

#define V6_INITACQ_NODETECT_REG    0x20 
#define V6_FORCE_NFFT_CPSIZE_REG   0x20

#define V6_CODE_RATE_TPS_REG       0x29
#define V6_CODE_RATE_TPS_MASK      0x07

#define V6_CP_LOCK_DET_REG        0x28
#define V6_CP_LOCK_DET_MASK       0x04

#define V6_TPS_HIERACHY_REG        0x29
#define V6_TPS_HIERARCHY_INFO_MASK  0x40

#define V6_TPS_HPORLP_REG        0x20
#define V6_TPS_HPORLP_INFO_MASK  0x80

#define V6_MODORDER_TPS_REG        0x2A
#define V6_PARAM_CONSTELLATION_MASK   0x30

#define V6_TPS_ALPHA_REG        0x2A
#define V6_TPS_ALPHA_MASK   0x03

#define V6_MODE_TPS_REG            0x2A
#define V6_PARAM_FFT_MODE_MASK        0x0C

#define V6_CP_TPS_REG             0x29
#define V6_PARAM_GI_MASK              0x30

#define V6_AGC_LOCK_REG             0x26
#define V6_AGC_LOCK_MASK              0x20

#define V6_TPS_LOCK_REG           0x2A
#define V6_PARAM_TPS_LOCK_MASK        0x40

#define V6_FEC_PER_COUNT_REG      0x2E
#define V6_FEC_PER_SCALE_REG      0x2B
#define V6_FEC_PER_SCALE_MASK        0x03

#define V6_PIN_MUX_MODE_REG       0x1B    
#define V6_ENABLE_PIN_MUX            0x1E

#define V6_MPEG_OUT_CLK_INV_REG    0x17    
#define V6_MPEG_OUT_CTRL_REG       0x18    

#define V6_INVERTED_CLK_PHASE       0x20
#define V6_MPEG_DATA_PARALLEL    0x01
#define V6_MPEG_DATA_SERIAL      0x02

#define V6_INVERTED_MPEG_SYNC       0x04
#define V6_INVERTED_MPEG_VALID      0x08

#define V6_MPEG_INOUT_BIT_ORDER_CTRL_REG  0x19
#define V6_MPEG_SER_MSB_FIRST                0x80
#define MPEG_SER_MSB_FIRST_ENABLED        0x01

#define V6_TUNER_LOOP_THRU_CONTROL_REG  0x09
#define V6_ENABLE_LOOP_THRU               0x01

#define TOTAL_NUM_IF_OUTPUT_FREQ       16

#define TUNER_NORMAL_IF_SPECTRUM       0x0
#define TUNER_INVERT_IF_SPECTRUM       0x10

#define V6_TUNER_IF_SEL_REG              0x06
#define V6_TUNER_IF_FCW_REG              0x3C
#define V6_TUNER_IF_FCW_BYP_REG          0x3D
#define V6_RF_LOCK_STATUS_REG            0x23

#define V6_DIG_CLK_FREQ_SEL_REG  0x07
#define V6_REF_SYNTH_INT_REG     0x5C
#define V6_REF_SYNTH_REMAIN_REG  0x58
#define V6_DIG_RFREFSELECT_REG   0x32
#define V6_XTAL_CLK_OUT_GAIN_REG   0x31
#define V6_TUNER_LOOP_THRU_CTRL_REG      0x09
#define V6_DIG_XTAL_ENABLE_REG  0x06
#define V6_DIG_XTAL_BIAS_REG  0x66
#define V6_XTAL_CAP_REG    0x08

#define V6_GPO_CTRL_REG     0x18
#define V6_GPO_0_MASK       0x10
#define V6_GPO_1_MASK       0x20

#define MXL_MODE_REG    0x03
#define START_TUNE_REG  0x1C

#define V6_DIG_RF_PWR_LSB_REG   0x46
#define V6_DIG_RF_PWR_MSB_REG   0x47

#define V6_AGC_LOCK_REG         0x26
#define V6_AGC_LOCK_MASK        0x20

#define V6_TPS_CELL_ID_LSB_REG  0x98
#define V6_TPS_CELL_ID_MSB_REG  0x99

#define V6_FREQ_OFFSET_LSB_REG  0x9D
#define V6_FREQ_OFFSET_MSB_REG  0x9E

#define V6_SPECTRUM_CTRL_REG  0x8D
#define V6_SPECTRUM_MASK  0x01

#define FREQ_OFFSET_SRCH_RANGE_REG  0xEA

#define RS_PKT_THRESH_REG  0xE4
/*****************************************************************************************
    User-Defined Types (Typedefs)
*****************************************************************************************/

typedef struct
{
  UINT8 regAddr;
  UINT8 mask;
  UINT8 data;
} REG_CTRL_INFO_T, *PREG_CTRL_INFO_T;

typedef struct 
{
  UINT32 StartTime;
  UINT32 AgcGain;
  UINT16 Timeout;
  UINT8 TpsCellId;
  UINT8 ChanConfigStatus;
}MXL_TUNER_CHAN_CONFIG_T, *PMXL_TUNER_CHAN_CONFIG_T;
/*****************************************************************************************
    Global Variable Declarations
*****************************************************************************************/

extern REG_CTRL_INFO_T MxL_101OverwriteDefault[]; 
extern REG_CTRL_INFO_T MxL_101SuspendMode[];
extern REG_CTRL_INFO_T MxL_101WakeUp[];
extern REG_CTRL_INFO_T MxL_MpegDataOutToTSIF[];
extern REG_CTRL_INFO_T MxL_MpegDataIn[];
extern REG_CTRL_INFO_T MxL_PhySoftReset[];     
extern REG_CTRL_INFO_T MxL_TunerDemodMode[];
extern REG_CTRL_INFO_T MxL_TunerMode[];
extern REG_CTRL_INFO_T MxL_TopMasterEnable[];
extern REG_CTRL_INFO_T MxL_TopMasterDisable[];
extern REG_CTRL_INFO_T MxL_IrqClear[];
extern REG_CTRL_INFO_T MxL_ResetPerCount[];
extern REG_CTRL_INFO_T MxL_CableSettings[];
extern REG_CTRL_INFO_T MxL_EnableCellId[];
extern REG_CTRL_INFO_T MxL_DisableCellId[];
extern REG_CTRL_INFO_T MxL_EnableChanScan[];
extern REG_CTRL_INFO_T MxL_DisableChanScan[];
extern REG_CTRL_INFO_T MxL_StartTune[];
extern REG_CTRL_INFO_T MxL_EnableHighTempRange[];
extern REG_CTRL_INFO_T MxL_DisableHighTempRange[];
extern REG_CTRL_INFO_T MxL_Config6MHzBandwidth[];
extern REG_CTRL_INFO_T MxL_Config7MHzBandwidth[];

/*****************************************************************************************
    Prototypes
*****************************************************************************************/

MXL_STATUS Ctrl_ProgramRegisters(PREG_CTRL_INFO_T ctrlRegInfoPtr);
PREG_CTRL_INFO_T Ctrl_PhyTune(UINT32 frequency, UINT8 bandwidth);

// OEM specific APIs
MXL_STATUS Ctrl_ReadRegister(UINT8 regAddr, UINT8 *dataPtr);
MXL_STATUS Ctrl_WriteRegister(UINT8 regAddr, UINT8 regData);
#endif /* __MXL101SF_PHY_CFG_H__*/




