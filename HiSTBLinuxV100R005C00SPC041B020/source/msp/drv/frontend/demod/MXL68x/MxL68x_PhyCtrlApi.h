/*******************************************************************************
 *
 * FILE NAME          : MxL68x_PhyCtrlApi.h
 * 
 * AUTHOR             : Dong Liu, Mahendra Kondur, Jay Hung
 *
 * DATE CREATED       : 4/11/2012, 3/9/2015
 *
 * DESCRIPTION        : This file is a header file for MxL68x_PhyCtrlApi.c 
 *
 *******************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MXL68X_PHY_CTRL_API_H__
#define __MXL68X_PHY_CTRL_API_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/
#include "MaxLinearDataTypes.h"
/******************************************************************************
    Macros
******************************************************************************/

#define MXL68X_VERSION_SIZE          4
#define MAX_COMMAND_DATA_SIZE        252 

#define ISDBT_AC_DATA_LENGTH         24    // Total AC Data is 192 bits 
#define FW_CHAN_TUNE_DATA_LENGTH     8     // Total FW channel tune result data length 
#define MAX_FW_RETURN_DATA_LENGTH    24

/* IF Frequency in KHz */
#define MXL68x_IF_FREQ_3650_KHZ     3650
#define MXL68x_IF_FREQ_4000_KHZ     4000
#define MXL68x_IF_FREQ_4100_KHZ     4100
#define MXL68x_IF_FREQ_4150_KHZ     4150
#define MXL68x_IF_FREQ_4500_KHZ     4500
#define MXL68x_IF_FREQ_4570_KHZ     4570
#define MXL68x_IF_FREQ_5000_KHZ     5000
#define MXL68x_IF_FREQ_5380_KHZ     5380
#define MXL68x_IF_FREQ_6000_KHZ     6000
#define MXL68x_IF_FREQ_6280_KHZ     6280
#define MXL68x_IF_FREQ_7200_KHZ     7200
#define MXL68x_IF_FREQ_82500_KHZ    8250
#define MXL68x_IF_FREQ_35250_KHZ    35250
#define MXL68x_IF_FREQ_36000_KHZ    36000
#define MXL68x_IF_FREQ_36150_KHZ    36150
#define MXL68x_IF_FREQ_36650_KHZ    36650
#define MXL68x_IF_FREQ_44000_KHZ    44000

#define MAX_PRESET_IF_FREQ          17

/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/

/* MxL68x SOC mode bandwidth type */
typedef enum
{
  MXL681_DEVICE = 0,           // MxL681  
  MXL683_DEVICE = 1,           // MxL683  
  UNKNOWN_DEVICE 
} MXL68x_DEVICE_TYPE_E;

/* Device Reset */
typedef enum
{
  MXL68x_DEMOD_RESET = 0,           /* Device Reset, Firmware should be downloaded after reset */
  MXL68x_DEMOD_DATAPATH_RESET,      /* Demod only reset */
  MXL68x_TUNER_RESET                /* Tuner only reset */
} MXL68x_RESET_TYPE_E;

/* Power supply set */
typedef enum
{
  MXL68x_SINGLE_SUPPLY = 0,
  MXL68x_DUAL_SUPPLY 
} MXL68x_PWR_SUPPLY_E;

/* XTAL set */
typedef enum
{
  MXL68x_XTAL_16MHz = 0,
  MXL68x_XTAL_24MHz 
} MXL68x_XTAL_FREQ_E;

/* Power mode set  */
typedef enum
{
  MXL68x_PWR_MODE_ACTIVE =0,
  MXL68x_PWR_MODE_STANDBY,
  MXL68x_PWR_MODE_EWS
} MXL68x_PWR_MODE_E;

/* Operation mode */
typedef enum
{
  MXL68x_TUNER_ONLY = 0,
  MXL68x_TUNER_DEMOD
} MXL68x_DEV_MODE_E;

/* IF source type */
typedef enum
{
  MXL68x_IF_INTERNAL = 0,
  MXL68x_IF_EXTERNAL = 1,
  MXL68x_IF_NA
} MXL68x_IF_SRC_E;

/* GPIO definition */
typedef enum
{
  MXL68x_GPIO_1 = 1, 
  MXL68x_GPIO_2 = 2,
  MXL68x_GPIO_3 = 3,
  MXL68x_GPIO_4 = 4,
  MXL68x_GPIO_5 = 5,
  MXL68x_GPIO_6 = 6,
  MXL68x_GPIO_7 = 7
} MXL68x_GPIO_ID_E;

/* GPIO direction definition */
typedef enum
{
  MXL68x_GPIO_INPUT = 0, 
  MXL68x_GPIO_OUTPUT = 1
} MXL68x_GPIO_TYPE_E;

/* GPIO data definition */
typedef enum
{
  MXL68x_GPIO_LOW = 0,
  MXL68x_GPIO_HIGH = 1
} MXL68x_GPIO_STATE_E;

typedef struct
{
  MXL68x_GPIO_ID_E gpioId;
  MXL68x_GPIO_TYPE_E gpioType;
  MXL68x_GPIO_STATE_E gpioState;
} MXL68x_GPIO_T, *PMXL68x_GPIO_T;

/* ISDB-T layer definition */
typedef enum
{
  MXL68x_ISDBT_LAYER_A = 0,  
  MXL68x_ISDBT_LAYER_B,
  MXL68x_ISDBT_LAYER_C,
  MXL68x_ISDBT_LAYER_ALL
} MXL68x_ISDBT_LAYER_E;

/* TMCC information */
typedef enum
{
  MXL68x_ISDBT_MODE1 = 0,             // ISDB-T mode1, 2k FFT  
  MXL68x_ISDBT_MODE2 = 1,             // ISDB-T mode2, 4k FFT
  MXL68x_ISDBT_MODE3 = 2,             // ISDB-T mode3, 8k FFT 
  MXL68x_ISDBT_MODE_UNKNOWN = 0xFF
} MXL68x_ISDBT_MODE_E;

typedef enum
{
  MXL68x_MODULATION_DQPSK = 0,
  MXL68x_MODULATION_QPSK  = 1,
  MXL68x_MODULATION_QAM16 = 2,
  MXL68x_MODULATION_QAM64 = 3, 
  MXL68x_MODULATION_UNKNOWN = 0xFF
} MXL68x_MODULATION_E;

typedef enum
{
  MXL68x_GI_1_32 = 0, 
  MXL68x_GI_1_4  = 1,
  MXL68x_GI_1_8  = 2,
  MXL68x_GI_1_16 = 3, 
  MXL68x_GI_UNKNOWN = 0xFF
} MXL68x_GUARD_INTERVAL_E;

typedef enum
{
  MXL68x_CR_1_2 = 0,
  MXL68x_CR_2_3 = 1,
  MXL68x_CR_3_4 = 2,
  MXL68x_CR_5_6 = 3,
  MXL68x_CR_7_8 = 4,
  MXL68x_CR_UNKNOWN = 0xFF
} MXL68x_CODE_RATE_E;

typedef enum
{
  MXL68x_INTERLEAVER_INDEX0 = 0,  
  MXL68x_INTERLEAVER_INDEX1 = 1,
  MXL68x_INTERLEAVER_INDEX2 = 2,
  MXL68x_INTERLEAVER_INDEX3 = 3, 
  MXL68x_INTERLEAVER_UNKNOWN = 0xFF
} MXL68x_INTERLEAVER_LEN_E;

typedef enum
{
  MXL68x_BROADCAST_TV = 0,  
  MXL68x_BROADCAST_AUDIO = 1,
  MXL68x_BROADCAST_NA = 2,
} MXL68x_BROADCAST_TYPE_E;

typedef struct
{
  MXL68x_MODULATION_E       modulationInfo;
  MXL68x_CODE_RATE_E        codeRateInfo;
  MXL68x_INTERLEAVER_LEN_E  interleaverLenInfo;
  UINT8                     numOfSegments;
} MXL68x_TMCC_INFO_T;

/* MPEG Clock frequency */ 
typedef enum  
{
  MXL68x_MPEG_CLK_64MHZ = 0,     // Clock 64MHz  
  MXL68x_MPEG_CLK_32MHZ = 1,     // Clock 32 MHz 
  MXL68x_MPEG_CLK_16MHZ = 2,     // Clock 16 MHz
  MXL68x_MPEG_CLK_NA
} MXL68x_MPEG_CLK_FREQ_E;

/* MPEG Out data format */ 
typedef enum  
{
  MXL68x_MPEG_DATA_MSB_FIRST = 0,  // Serial MSB first
  MXL68x_MPEG_DATA_LSB_FIRST = 1,  // Serial LSB first   
  MXL68x_MPEG_DATA_NA
} MXL68x_MPEG_DATA_FMT_E;

typedef enum  
{
  MXL68x_POLARITY_NORMAL = 0,     // Normal Polarity   
  MXL68x_POLARITY_INVERTED = 1,   // Inverted Polarity
  MXL68x_POLARITY_NA
} MXL68x_MPEG_POLARITY_E;

typedef enum
{
  MXL68x_MPEG_SYNC_PULSE_1BYTE = 0,     // 1 Byte Pulse width
  MXL68x_MPEG_SYNC_PULSE_1BIT = 1,      // 1 Bit Pulse width  
  MXL68x_MPEG_SYNC_PULSE_NA
} MXL68x_MPEG_SYNC_PULSE_E;

typedef enum  
{
  MXL68x_TS_PKT_204_BYTES = 0,      // 1 Bit Pulse width  
  MXL68x_TS_PKT_188_BYTES = 1,      // 1 Byte Pulse width
  MXL68x_TS_PKT_NA
} MXL68x_TS_PKT_SIZE_E;

typedef enum  
{
  MXL68x_MPEG_MODE_NORMAL = 0,  // MERR, MVAL, MSYNC, MCLK & MDAT
  MXL68x_MPEG_MODE_3WIRE = 1,   // MCLK, MSYNC & MDAT
  MXL68x_MPEG_MODE_NA
} MXL68x_MPEG_MODE_E;

/* MPEG out clock phase type */ 
typedef enum  
{
  MXL68x_MPEG_CLK_PHASE_SHIFT_0_DEG = 0,       
  MXL68x_MPEG_CLK_PHASE_SHIFT_90_DEG,
  MXL68x_MPEG_CLK_PHASE_SHIFT_180_DEG,
  MXL68x_MPEG_CLK_PHASE_SHIFT_270_DEG,
  MXL68x_MPEG_CLK_PHASE_SHIFT_NA
} MXL68x_MPEG_CLK_PHASE_E;

/* MPEG out error indicator type */
typedef enum
{
  MXL68x_MPEG_ERROR_IND_ENABLE = 0,
  MXL68x_MPEG_ERROR_IND_DISABLE = 1,
  MXL68x_MPEG_ERROR_IND_NA
} MXL68x_MPEG_ERROR_IND_E;

typedef struct
{
  MXL68x_MPEG_MODE_E        mpegMode;
  MXL68x_MPEG_CLK_FREQ_E    mpegClkFreq;
  MXL68x_MPEG_DATA_FMT_E    mpegDataFmt;
  MXL68x_MPEG_CLK_PHASE_E   mpegClkPhase;
  MXL68x_MPEG_POLARITY_E    mpegClkPol;
  MXL68x_MPEG_POLARITY_E    mpegSyncPol;
  MXL68x_MPEG_POLARITY_E    mpegValidPol;
  MXL68x_MPEG_SYNC_PULSE_E  mpegSyncPulseWidth;
  MXL68x_MPEG_ERROR_IND_E   mpegErrIndicator;
  MXL68x_TS_PKT_SIZE_E      tsPktSize;  
} MXL68x_MPEG_OUT_INF_T;

/* MxL68x SOC mode bandwidth type */
typedef enum
{
  MXL68x_BW_6MHz = 0,           // Bandwidth, 6MHz 
  MXL68x_BW_7MHz,               // Bandwidth, 7MHz 
  MXL68x_BW_8MHz                // Bandwidth, 8MHz 
} MXL68x_BW_E;

/* MxL68x SOC mode bandwidth type */
typedef enum
{
  MXL68x_AUTO_SPEC_INV_DISABLE = 0,           // Disbale Auto Spec Inv 
  MXL68x_AUTO_SPEC_INV_ENABLE,               // Enable Auto Spec Inv till first frm sync 
} MXL68x_AUTO_SPEC_INV;

/* Tuner application mode */
typedef enum
{
  MXL68x_ANA_NTSC_MODE = 0x0,
  MXL68x_ANA_PAL_BG = 0x01,
  MXL68x_ANA_PAL_I = 0x02,
  MXL68x_ANA_PAL_D = 0x03,
  MXL68x_ANA_SECAM_I = 0x04,
  MXL68x_ANA_SECAM_L = 0x05,
  MXL68x_DIG_DVB_C = 0x06, 
  MXL68x_DIG_ATSC = 0x07,
  MXL68x_DIG_DVB_T = 0x08,
  MXL68x_DIG_J83B = 0x09,
  MXL68x_DIG_ISDBT = 0x0A,
  MXL68x_SIGNAL_NA = 99
} MXL68x_SIGNAL_MODE_E;

/* AGC mode */
typedef enum
{
  MXL68x_AGC_SELF = 0,
  MXL68x_AGC_EXTERNAL = 1
} MXL68x_AGC_TYPE_E;

/* Channel Tune mode */
typedef enum
{
  MXL68x_VIEW_MODE = 0, 
  MXL68x_SCAN_MODE 
} MXL68x_TUNE_TYPE_E;

typedef enum
{
  MXL68x_NTSC_CARRIER_NA = 0,
  MXL68x_NTSC_CARRIER_HRC = 1,
  MXL68x_NTSC_CARRIER_IRC = 2
} MXL68x_NTSC_CARRIERS_E;

/* MxL68x Tuner work signal type */
typedef enum
{
  MXL68x_SIGNAL_TYPE_DIGITAL = 0,
  MXL68x_SIGNAL_TYPE_ANALOG 
} MXL68x_SIGNAL_TYPE_E;

typedef enum
{
  MXL68x_FINE_TUNE_STEP_UP = 0,
  MXL68x_FINE_TUNE_STEP_DOWN,
} MXL68x_FINE_TUNE_STEP_E;

typedef enum
{
  MXL68x_PLL_CLOCK = 0, 
  MXL68x_XTAL_CLOCK
} MXL68x_CLK_SWITCH_E;

typedef enum
{
  MXL68x_TS_DRV_STR_1X = 0,
  MXL68x_TS_DRV_STR_3X = 1,
  MXL68x_TS_DRV_STR_5X = 2,
  MXL68x_TS_DRV_STR_7X = 3,
  MXL68x_TS_DRV_STR_NA
} MXL68x_TS_DRV_STR_E;

// PER 
typedef struct
{
  UINT32 pktErrCount_W_Iter;
  UINT32 pktErrCount_WO_Iter;
  UINT32 numOfPkts;
} MXL68x_PER_T;

// BER 
typedef struct
{
  UINT32 bitErrCount_W_Iter;
  UINT32 bitErrCount_WO_Iter;
  UINT32 numOfBits;
} MXL68x_BER_T;

/******************************************************************************
    Global Variable Declarations
******************************************************************************/
extern const UINT8 MxLWareDrvVersion[];
extern const UINT8 BuildNumber;
/******************************************************************************
    Prototypes
******************************************************************************/

MXL_STATUS_E MxLWare68x_API_CfgDrvInit(
	            			UINT8 devId, 
				            void* oemDataPtr);

MXL_STATUS_E MxLWare68x_API_CfgDrvDeInit(UINT8 devId);

MXL_STATUS_E MxLWare68x_API_CfgDevPowerSupply(
                UINT8 devId,
                MXL68x_PWR_SUPPLY_E devPwrSupplyCfg);

MXL_STATUS_E MxLWare68x_API_CfgDevOperationalMode(   
                UINT8 devId,			
                MXL68x_DEV_MODE_E deviceOpMode);

MXL_STATUS_E MxLWare68x_API_CfgDevXtal( 
                UINT8 devId,            
                MXL68x_XTAL_FREQ_E xtalFreqSel,    
                UINT8 xtalCap,                                                                                             
                MXL_BOOL_E clkOutEnable,     
                MXL_BOOL_E clkOutDiv,
                MXL_BOOL_E enableXtalSharing,
                MXL_BOOL_E enableXtalCal);

MXL_STATUS_E MxLWare68x_API_CfgDevInit(
                UINT8 devId,                 
                MXL68x_IF_SRC_E ifSrc,
                MXL_BOOL_E enableRfLoopThru);

MXL_STATUS_E MxLWare68x_API_CfgDevFirmwareDownload( 
                UINT8 devId,
                UINT32 mbinBufferSize,                    
                UINT8 *mbinBufferPtr);

MXL_STATUS_E MxLWare68x_API_CfgDevPowerMode(
                UINT8 devId,
                MXL68x_PWR_MODE_E powerMode);

MXL_STATUS_E MxLWare68x_API_CfgDevSoftwareReset(
                UINT8 devId,
                MXL68x_RESET_TYPE_E rstType);

MXL_STATUS_E MxLWare68x_API_CfgDevGPIO (
                UINT8 devId,
                MXL68x_GPIO_ID_E gpioId,
                MXL68x_GPIO_TYPE_E  gpioType,
                MXL68x_GPIO_STATE_E gpioState);

MXL_STATUS_E MxLWare68x_API_CfgDevCustSpecificSettings(
                UINT8 devId,
                UINT16 mask);
// Device Status Request    

MXL_STATUS_E MxLWare68x_API_ReqDevVersionInfo(
                UINT8 devId,
                UINT8* chipIdPtr,  
                UINT8* chipVersionPtr,
                UINT8 mxlWareVer[MXL68X_VERSION_SIZE],
                UINT8 firmwareVer[MXL68X_VERSION_SIZE],
                UINT8 bootloaderVer[MXL68X_VERSION_SIZE]);

MXL_STATUS_E MxLWare68x_API_ReqDevGPIOStatus( 
                UINT8 devId,
                MXL68x_GPIO_ID_E gpioId,
                MXL68x_GPIO_TYPE_E  *gpioTypePtr,
                MXL68x_GPIO_STATE_E *gpioStatePtr);

MXL_STATUS_E MxLWare68x_API_ReqDevIntrStatus( 
                UINT8 devId,
                UINT8* intrStatusPtr);

// Demod Configuration

MXL_STATUS_E MxLWare68x_API_CfgDemodClearBerPerEndFlag(
                UINT8 devId);

MXL_STATUS_E MxLWare68x_API_CfgDemodBerPer( 
                UINT8 devId,
                UINT8 numFrames);

MXL_STATUS_E MxLWare68x_API_CfgDemodRSDecIterDec(
              UINT8 devId, 
              MXL_BOOL_E enableIterativeDecoder, 
              MXL_BOOL_E enableRsDecoder);

MXL_STATUS_E MxLWare68x_API_CfgDemodNullPacketFilter(
                UINT8 devId, 
                MXL_BOOL_E enableLayerA,
                MXL_BOOL_E enableLayerB,
                MXL_BOOL_E enableLayerC,
                MXL_BOOL_E enableSkipNullPacket);

MXL_STATUS_E MxLWare68x_API_CfgDemodMPEGOut( 
                UINT8 devId,  
                MXL68x_MPEG_MODE_E mpegMode,
                MXL68x_MPEG_CLK_FREQ_E mpegClkFreq,
                MXL68x_MPEG_DATA_FMT_E mpegDataFmt,
                MXL68x_MPEG_CLK_PHASE_E mpegClkPhase,    
                MXL68x_MPEG_POLARITY_E mpegSyncPol,
                MXL68x_MPEG_POLARITY_E mpegValidPol,
                MXL68x_MPEG_SYNC_PULSE_E mpegSyncPulseWidth,
                MXL68x_MPEG_ERROR_IND_E mpegErrIndicator,
                MXL68x_TS_PKT_SIZE_E tsPktSize);

MXL_STATUS_E MxLWare68x_API_CfgDemodMPEGTSDriveStrength(
                UINT8 devId,
                MXL68x_TS_DRV_STR_E tsDriveStrength);

// Demod Status Request
MXL_STATUS_E MxLWare68x_API_ReqDemodBerPerEndFlag(
                UINT8 devId, 
                UINT8 *numFramesPtr,
                MXL_BOOL_E *berEndFlagPtr);

MXL_STATUS_E MxLWare68x_API_ReqDemodCNR(   
                UINT8 devId,
                UINT32* cnrRawPtr,
                SINT32* cnrPtr);

MXL_STATUS_E MxLWare68x_API_ReqDemodLockStatus(        
                UINT8 devId,
                MXL_BOOL_E *tmccLockPtr,				
                MXL_BOOL_E *layerARSLockPtr,
                MXL_BOOL_E *layerBRSLockPtr,
                MXL_BOOL_E *layerCRSLockPtr);

MXL_STATUS_E MxLWare68x_API_ReqDemodNullPacketFilter(
                UINT8 devId, 
											 				MXL_BOOL_E* enableLayerAPtr,
												 			MXL_BOOL_E* enableLayerBPtr,
																MXL_BOOL_E* enableLayerCPtr,
																MXL_BOOL_E* enableSkipNullPacketPtr);

MXL_STATUS_E MxLWare68x_API_ReqDemodRsIterSetting(
                UINT8 devId, 
																MXL_BOOL_E* enableIterativeDecoder, 
																MXL_BOOL_E* enableRsDecoder);

MXL_STATUS_E MxLWare68x_API_ReqDemodMpegOutSet(
                UINT8 devId,
                MXL68x_MPEG_MODE_E *mpegModePtr,
                MXL68x_MPEG_CLK_FREQ_E *mpegClkFreqPtr,
                MXL68x_MPEG_DATA_FMT_E *mpegDataFmtPtr,
                MXL68x_MPEG_CLK_PHASE_E *mpegClkPhasePtr,    
                MXL68x_MPEG_POLARITY_E *mpegSyncPolPtr,
                MXL68x_MPEG_POLARITY_E *mpegValidPolPtr, 
                MXL68x_MPEG_SYNC_PULSE_E *mpegSyncPulseWidthPtr,         
                MXL68x_MPEG_ERROR_IND_E *mpegErrIndicatorPtr,      
                MXL68x_TS_PKT_SIZE_E *tsPktSizePtr);

MXL_STATUS_E MxLWare68x_API_ReqDemodMPEGTSDriveStrength(
                UINT8 devId,
                MXL68x_TS_DRV_STR_E *tsDriveStrengthPtr);

MXL_STATUS_E MxLWare68x_API_ReqDemodBER(
                UINT8 devId,   
                MXL68x_BER_T *layerABerPtr,    
                MXL68x_BER_T *layerBBerPtr,
                MXL68x_BER_T *layerCBerPtr);

MXL_STATUS_E MxLWare68x_API_ReqDemodPER(
                UINT8 devId,   
                MXL68x_PER_T *layerAPktErrCountPtr,    
                MXL68x_PER_T *layerBPktErrCountPtr,
                MXL68x_PER_T *layerCPktErrCountPtr);

MXL_STATUS_E MxLWare68x_API_ReqDemodTMCCParams(
                UINT8 devId,
                MXL_BOOL_E *ewsAlarmPtr,   
                MXL68x_GUARD_INTERVAL_E *guardIntervalInfoPtr, 
                MXL68x_ISDBT_MODE_E *isdbtModeInfoPtr,   
                MXL68x_TMCC_INFO_T *tmccInfoLayerAPtr,  
                MXL68x_TMCC_INFO_T *tmccInfoLayerBPtr,              
                MXL68x_TMCC_INFO_T *tmccInfoLayerCPtr);

MXL_STATUS_E MxLWare68x_API_ReqDemodAcData(
	               UINT8 devId, 
                UINT8 *acDataPtr);

MXL_STATUS_E MxLWare68x_API_ReqDemodFreqOffset(
	               UINT8 devId, 
                 SINT32 *carrierFreqOff,
                 SINT32 *samplingFreqOff);

MXL_STATUS_E MxLWare68x_API_ReqDemodFreqOffsetInKHzAndPPM(
	               UINT8 devId, 
                 SINT32 *carrierFreqOffInKHz,
                 SINT32 *samplingFreqOffInPPM);

MXL_STATUS_E MxLWare68x_API_ReqDemodSpectrumInversion(
                UINT8 devId,
                MXL_BOOL_E *isSpectrumInverted);

// Tuner Configuration
MXL_STATUS_E MxLWare68x_API_CfgTunerIFOutParams(
                UINT8 devId,
                UINT16 ifOutFreqInKHz,
                MXL_BOOL_E invertIFPolarity,
                UINT8 gainLevel);

MXL_STATUS_E MxLWare68x_API_CfgTunerApplicationMode(
                UINT8 devId,
                MXL68x_SIGNAL_MODE_E signalMode);

MXL_STATUS_E MxLWare68x_API_CfgTunerAGC( 
                UINT8 devId,
                MXL68x_AGC_TYPE_E agcType,
                UINT8 setPoint,
                MXL_BOOL_E agcPolarityInversion);

MXL_STATUS_E MxLWare68x_API_CfgTunerChanTune(
                UINT8 devId,
                MXL68x_TUNE_TYPE_E tuneType,
                MXL68x_BW_E bandWidth,
                UINT32 freqInHz,
                MXL68x_AUTO_SPEC_INV autoSpecInv,
                MXL_BOOL_E useChanTuneInt);

MXL_STATUS_E MxLWare68x_API_CfgTunerEnableFineTune(
                UINT8 devId,
                MXL_BOOL_E enableFineTune);

MXL_STATUS_E MxLWare68x_API_CfgTunerFineTune(
                UINT8 devId,
                MXL68x_FINE_TUNE_STEP_E fineTuneStep);

// Tuner Status Request
MXL_STATUS_E MxLWare68x_API_ReqTunerAGCStatus(  
                UINT8 devId,
                MXL_BOOL_E* agcLockStatusPtr,
                UINT16* rawAgcGainPtr);

MXL_STATUS_E MxLWare68x_API_ReqTunerLockStatus(
                UINT8 devId,
                MXL_BOOL_E* rfSynthLockPtr,
                MXL_BOOL_E* refSynthLockPtr);

MXL_STATUS_E MxLWare68x_API_ReqTunerRxPower(
                UINT8 devId,
                SINT32* rxPwrPtr);

// Subrotine used in MxLWare 
MXL_STATUS_E Ctrl_CfgDemodEnable(UINT8 devId, MXL_BOOL_E demodEnable);
MXL_STATUS_E Ctrl_CfgDevSwitchClock(
                       UINT8 devId, 
                       MXL68x_CLK_SWITCH_E clkType);
#endif /* __MXL68X_PHY_CTRL_API_H__*/




