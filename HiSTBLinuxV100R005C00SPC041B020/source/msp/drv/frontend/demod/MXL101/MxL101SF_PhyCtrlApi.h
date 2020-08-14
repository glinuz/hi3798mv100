/*****************************************************************************************
 *
 * FILE NAME          : MxL101SF_PhyCtrlApi.h
 * 
 * AUTHOR             : Brenndon Lee
 *                      Mahendra Kondur - code seperation
 *
 * DATE CREATED       : 5/18/2009
 *                      10/22/2009 
 *
 * DESCRIPTION        : This file is header file for MxL101SF_PhyCtrlApi.cpp
 *
 *****************************************************************************************
 *                Copyright (c) 2006, MaxLinear, Inc.
 ****************************************************************************************/

#ifndef __MXL101SF_PHY_CTRL_API_H__
#define __MXL101SF_PHY_CTRL_API_H__

/*****************************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
*****************************************************************************************/
#include "MaxLinearDataTypes.h"

/*****************************************************************************************
    Macros
*****************************************************************************************/

#define CALCULATE_SNR(data)        ((1563 * data) - 25000)
#define CALCULATE_BER(avg_errors, count)  (UINT32)(avg_errors * 4 * 3247)

#define MXL_TUNER_MODE         0
#define MXL_SOC_MODE           1
#define MXL_DEV_MODE_MASK      0x01

/*****************************************************************************************
    User-Defined Types (Typedefs)
*****************************************************************************************/

/* Command Types  */
typedef enum
{
  /* MxL Device configure command */
  MXL_DEV_SOFT_RESET_CFG = 0,
  MXL_DEV_XTAL_SETTINGS_CFG,
  MXL_DEV_ID_VERSION_REQ,
  MXL_DEV_OPERATIONAL_MODE_CFG,
  MXL_DEV_GPO_PINS_CFG,
  MXL_DEV_CABLE_CFG,
  
  /* MxL101SF Config specific commands */
  MXL_DEV_101SF_OVERWRITE_DEFAULTS_CFG,
  MXL_DEV_101SF_POWER_MODE_CFG,
  MXL_DEV_MPEG_OUT_CFG,
  MXL_DEV_TS_CTRL_CFG,
  
  /* Reset/Clear Demod status */
  MXL_DEMOD_RESET_IRQ_CFG,
  MXL_DEMOD_RESET_PEC_CFG,
  MXL_DEMOD_TS_PRIORITY_CFG,

  /* Demod Config commands */
  MXL_DEMOD_SPECTRUM_CFG,
  MXL_DEMOD_RS_PKT_THRESH_CFG,
  
  /* Demod Status and Info command */
  MXL_DEMOD_SNR_REQ,
  MXL_DEMOD_BER_REQ,
  MXL_DEMOD_TPS_CODE_RATE_REQ,
  MXL_DEMOD_TPS_HIERARCHY_REQ,
  MXL_DEMOD_TPS_CONSTELLATION_REQ,
  MXL_DEMOD_TPS_FFT_MODE_REQ,
  MXL_DEMOD_TPS_HIERARCHICAL_ALPHA_REQ,
  MXL_DEMOD_TPS_GUARD_INTERVAL_REQ,
  MXL_DEMOD_TPS_CELL_ID_REQ,
  MXL_DEMOD_TPS_LOCK_REQ,
  MXL_DEMOD_PACKET_ERROR_COUNT_REQ,
  MXL_DEMOD_SYNC_LOCK_REQ,
  MXL_DEMOD_RS_LOCK_REQ,
  MXL_DEMOD_CP_LOCK_REQ,
  MXL_DEMOD_FEC_LOCK_REQ,
  
  /* Tuner Config commands */
  MXL_TUNER_TOP_MASTER_CFG,
  MXL_TUNER_CHAN_TUNE_CFG,
  MXL_TUNER_IF_OUTPUT_FREQ_CFG, 
  //MXL_TUNER_CHAN_TUNE_EXT_CFG,

  /* Tuner Status and Info command */
  MXL_TUNER_LOCK_STATUS_REQ,
  MXL_TUNER_SIGNAL_STRENGTH_REQ,
  
  /* Channel Scan commands */
  MXL_TUNER_CHAN_SCAN_CFG,
  MXL_TUNER_CHECK_CHAN_STATUS_REQ,
  MXL_TUNER_CHAN_OFFSET_REQ,
  MXL_TUNER_FREQ_OFFSET_SRCH_RANGE_CFG,
    
  MAX_NUM_CMD
} MXL_CMD_TYPE_E;

/* MXL_DEV_SOFT_RESET_CFG  */
typedef struct
{
  UINT8 DevId;                    /* OUT, Device Id of MxL101SF Device */
  UINT8 DevVer;                   /* OUT, Device Version of MxL101SF Device */
} MXL_DEV_INFO_T, *PMXL_DEV_INFO_T;

/* MXL_DEV_POWER_MODE_CFG */
typedef enum
{
  STANDBY_ON = 0,           /* Standby mode of Mxl101SF SoC */
  SLEEP_ON,                 /* Sleep mode of Mxl101SF SoC */
  STANDBY_OFF,              /* Power On (Standby off) mode of Mxl101SF SoC */
  SLEEP_OFF,                /* Power On (Sleep off) mode of Mxl101SF SoC */
} MXL_PWR_MODE_E; 

typedef struct
{
  MXL_PWR_MODE_E PowerMode;       /* IN, Power mode for MxL101SF Device */
} MXL_PWR_MODE_CFG_T, *PMXL_PWR_MODE_CFG_T;

/* MXL_TUNER_TOP_MASTER_CFG */
typedef struct
{
  MXL_BOOL  TopMasterEnable;      /* IN, Enable or Disable MxL101SF Tuner */
}MXL_TOP_MASTER_CFG_T, *PMXL_TOP_MASTER_CFG_T;

/* MXL_DEV_OPERATIONAL_MODE_CFG */
typedef struct
{
  UINT8 DeviceMode;             /* IN, Operational mode of MxL101SF */
} MXL_DEV_MODE_CFG_T, *PMXL_DEV_MODE_CFG_T;

// MXL_DEMOD_SNR_REQ
typedef struct
{
  UINT32 SNR;                   /* OUT, SNR data from MXL101SF */
} MXL_DEMOD_SNR_INFO_T, *PMXL_DEMOD_SNR_INFO_T;

/* MXL_DEMOD_BER_REQ */
typedef struct
{
  UINT32 BER;                 /* OUT, BER data from MXL101SF */
} MXL_DEMOD_BER_INFO_T, *PMXL_DEMOD_BER_INFO_T;

/* MXL_DEMOD_PACKET_ERROR_COUNT_REQ */
typedef struct
{
  UINT32 PEC;               /* OUT, PEC data from MXL101SF */
}MXL_DEMOD_PEC_INFO_T, *PMXL_DEMOD_PEC_INFO_T;

/* MXL_TUNER_CHAN_TUNE_CFG */
typedef struct
{
  UINT32 Frequency;                       /* IN, Frequency in MHz */
  UINT8 Bandwidth;                        /* IN, Channel Bandwidth in MHz */
  MXL_BOOL TpsCellIdRbCtrl;               /* IN, Enable TPS Cell ID Read Back feature */
} MXL_RF_TUNE_CFG_T, *PMXL_RF_TUNE_CFG_T;

/* Tune RF Lock Status MXL_TUNER_LOCK_STATUS_REQ */
typedef struct
{
  MXL_BOOL RfSynthLock;               /* OUT, RF SYNT Lock status of MXL101SF Tuner */
  MXL_BOOL RefSynthLock;              /* OUT, REF SYNT Lock status of MXL101SF Tuner */
} MXL_TUNER_LOCK_STATUS_T, *PMXL_TUNER_LOCK_STATUS_T;

/* MXL_DEMOD_TPS_CONSTELLATION_REQ */
/* MXL_DEMOD_TPS_CODE_RATE_REQ */
/* MXL_DEMOD_TPS_FFT_MODE_REQ */
/* MXL_DEMOD_TPS_GUARD_INTERVAL_REQ */
/* MXL_DEMOD_TPS_HIERARCHY_REQ */
typedef struct 
{
  UINT8 TpsInfo;                      /* OUT, TPS data for respective TPS command */
}MXL_DEMOD_TPS_INFO_T, *PMXL_DEMOD_TPS_INFO_T;
 
/* MXL_DEMOD_TPS_LOCK_REQ */
/* MXL_DEMOD_RS_LOCK_REQ */
/* MXL_DEMOD_CP_LOCK_REQ */
/* MXL_DEMOD_RS_LOCK_REQ */
typedef struct
{
  MXL_BOOL Status;                  /* OUT, Lock status of MxL101SF */ 
} MXL_DEMOD_LOCK_STATUS_T, *PMXL_DEMOD_LOCK_STATUS_T;

/* MXL_DEMOD_TS_PRIORITY_CFG */
typedef enum
{
  HP_STREAM = 0,       /* High Priority stream */
  LP_STREAM,           /* Low Priority stream */
}MXL_HPORLP_E;

typedef struct
{
  MXL_HPORLP_E StreamPriority;         /* IN,  Value to select transport stream priority*/ 
} MXL_DEMOD_TS_PRIORITY_CFG_T, *PMXL_DEMOD_TS_PRIORITY_CFG_T;

/* MXL_TUNER_IF_OUTPUT_FREQ_CFG */
typedef enum
{
  IF_OTHER_12MHZ = 0,           /* IF value less than 12MHz */
  IF_4_0MHZ,                    /* IF value 4 MHz */
  IF_4_5MHZ,                    /* IF value 4.5 MHz */
  IF_4_57MHZ,                   /* IF value 4.57 MHz */ 
  IF_5_0MHZ,                    /* IF value 5 MHz */
  IF_5_38MHZ,                   /* IF value 5.38 MHz */
  IF_6_0MHZ,                    /* IF value 6 MHz */
  IF_6_28MHZ,                   /* IF value 6.28 MHz */
  IF_7_2MHZ,                    /* IF value 7.2 MHz */
  IF_35_250MHZ,                 /* IF value 35.25 MHz */
  IF_36_0MHZ,                   /* IF value 36 MHz */
  IF_36_15MHZ,                  /* IF value 36.15 MHz */ 
  IF_44_0MHZ,                   /* IF value 44 MHz */ 
  IF_OTHER_35MHZ_45MHZ = 0x0F,  /* IF value greater than 35 MHz & less than 45 MHz */
} MXL_IF_FREQ_E; 

typedef struct
{
  MXL_IF_FREQ_E   IF_Index;               /* IN, Index for predefined IF frequency */
  UINT8           IF_Polarity;            /* IN, IF Spectrum - Normal or Inverted */
  UINT32          IF_Freq;                /* IN, IF Frequency in Hz for non-predefined frequencies */
} MXL_TUNER_IF_FREQ_T, *PMXL_TUNER_IF_FREQ_T;

/* MXL_DEV_XTAL_SETTINGS_CFG */
typedef enum
{
  XTAL_24MHz = 4,       /* XTAL frequency of 24 MHz */
  XTAL_27MHz = 7,       /* XTAL frequency of 27 MHz */
  XTAL_28_8MHz = 8,     /* XTAL frequency of 28.8 MHz */
  XTAL_48MHz = 12,      /* XTAL frequency of 48 MHz */
  XTAL_NA = 13          /* INVALID XTAL frequency */
} MXL_XTAL_FREQ_E;

typedef enum
{
  I123_5uA = 0,       /* XTAL bias value of 123.5 uA */
  I238_8uA,           /* XTAL bias value of 238.8 uA */
  I351_4uA,           /* XTAL bias value of 351.4 uA */
  I462_4uA,           /* XTAL bias value of 462.4 uA */
  I572_0uA,           /* XTAL bias value of 572.0 uA */
  I680_4uA,           /* XTAL bias value of 680.4 uA */
  I787_8uA,           /* XTAL bias value of 787.8 uA */
  I894_2uA,           /* XTAL bias value of 894.2 uA */
  XTAL_BIAS_NA        /* INVALID XTAL bias value*/
} MXL_XTAL_BIAS_E;

typedef enum
{
  CLK_OUT_0dB = 0,        /* XTAL clock out value of 0.0 dB */
  CLK_OUT_1_25dB,         /* XTAL clock out value of 1.25 dB */
  CLK_OUT_2_50dB,         /* XTAL clock out value of 2.50 dB */ 
  CLK_OUT_3_75dB,         /* XTAL clock out value of 3.75 dB */
  CLK_OUT_5_00dB,         /* XTAL clock out value of 5.00 dB */
  CLK_OUT_6_25dB,         /* XTAL clock out value of 6.25 dB */
  CLK_OUT_7_50dB,         /* XTAL clock out value of 7.50 dB */
  CLK_OUT_8_75dB,         /* XTAL clock out value of 8.75 dB */
  CLK_OUT_10_0dB,         /* XTAL clock out value of 10.0 dB */
  CLK_OUT_11_25dB,        /* XTAL clock out value of 11.25 dB */
  CLK_OUT_12_50dB,        /* XTAL clock out value of 12.50 dB */
  CLK_OUT_13_75dB,        /* XTAL clock out value of 13.75 dB */
  CLK_OUT_15_00dB,        /* XTAL clock out value of 15.00 dB */
  CLK_OUT_16_25dB,        /* XTAL clock out value of 16.25 dB */
  CLK_OUT_17_50dB,        /* XTAL clock out value of 17.50 dB */
  CLK_OUT_18_75dB,        /* XTAL clock out value of 18.75 dB */
  CLK_OUT_NA              /* INVLAID XTAL Clock out value */
} MXL_XTAL_CLK_OUT_GAIN_E;

typedef struct
{
  MXL_XTAL_FREQ_E           XtalFreq;             /* IN, XTAL frequency */
  MXL_XTAL_BIAS_E           XtalBiasCurrent;      /* IN, XTAL Bias current */
  UINT8                     XtalCap;              /* IN, XTAL Capacitance value */
  MXL_BOOL                  XtalClkOutEnable;     /* IN, XTAL Clock out control */
  MXL_XTAL_CLK_OUT_GAIN_E   XtalClkOutGain;       /* IN, XTAL Clock out gain value */
  MXL_BOOL                  LoopThruEnable;       /* IN, Frequency loop thru */
} MXL_XTAL_CFG_T, *PMXL_XTAL_CFG_T;

/* MXL_DEMOD_MPEG_OUT_CFG */
typedef enum
{
  MPEG_CLOCK_36_571429MHz = 0,       /* MPEG clock frequency of 36.571429 MHz */
  MPEG_CLOCK_2_285714MHz,            /* MPEG clock frequency of 2.285714 MHz */
  MPEG_CLOCK_4_571429MHz,            /* MPEG clock frequency of 4.571429 MHz */
  MPEG_CLOCK_6_857143MHz,            /* MPEG clock frequency of 6.857143 MHz */
  MPEG_CLOCK_9_142857MHz,            /* MPEG clock frequency of 9.142857 MHz */
  MPEG_CLOCK_13_714286MHz,           /* MPEG clock frequency of 13.714286 MHz */
  MPEG_CLOCK_18_285714MHz,           /* MPEG clock frequency of 18.285714 MHz */
  MPEG_CLOCK_27_428571MHz            /* MPEG clock frequency of 27.428571 MHz */
} MPEG_CLOCK_FREQ_E;

typedef enum
{
  MPEG_ACTIVE_LOW = 0,           /* MPEG clock polaity value of Low */ 
  MPEG_ACTIVE_HIGH,              /* MPEG clock polaity value of High */ 

  MPEG_CLK_POSITIVE  = 0,        /* Positive MPEG clock polaity */ 
  MPEG_CLK_NEGATIVE,             /* Negative MPEG clock polaity */ 
    
  MPEG_CLK_IN_PHASE = 0,         /* In-Phase MPEG clock polaity */   
  MPEG_CLK_INVERTED,             /* Inverted MPEG clock polaity */ 

  MPEG_CLK_EXTERNAL = 0,         /* External MPEG clock source */ 
  MPEG_CLK_INTERNAL              /* Internal MPEG clock source */    
} MPEG_CLK_FMT_E;

typedef enum
{
  MPEG_SERIAL_LSB_1ST = 0,       /* Serial MPEG date with LSB first */ 
  MPEG_SERIAL_MSB_1ST,           /* Serial MPEG date with MSB first */  
  
  MPEG_DATA_SERIAL  = 0,         /* MPEG data transmission using serial interface */  
  MPEG_DATA_PARALLEL,            /* MPEG data transmission using parallel interface */   

  MPEG_SYNC_WIDTH_BIT = 0,       /* 1 bit width of MPEG SYNC data */  
  MPEG_SYNC_WIDTH_BYTE           /* 8 bit width of MPEG SYNC data */  
} MPEG_DATA_FMT_E;

typedef struct
{ 
  MPEG_DATA_FMT_E   SerialOrPar;             /* IN, Serial or Parallel mode */
  MPEG_DATA_FMT_E   LsbOrMsbFirst;           /* IN, Serial mode MSB or LSB first */
  MPEG_CLOCK_FREQ_E MpegClkFreq;             /* IN, MPEG Clock frequency */ 
  MPEG_CLK_FMT_E    MpegValidPol;            /* IN, MPEG Valid polarity */
  MPEG_CLK_FMT_E    MpegClkPhase;            /* IN, MPEG Clock phase */
  MPEG_CLK_FMT_E    MpegSyncPol;             /* IN, MPEG SYNC Polarity */
} MXL_MPEG_CFG_T, *PMXL_MPEG_CFG_T;

/* MXL_DEV_TS_CTRL_CFG */
typedef enum
{
  TS_DRIVE_STRENGTH_DEFAULT = 0,       /* Default (1x) transport stream driver strength */
  TS_DRIVE_STRENGTH_1X = 0,            /* 1x transport stream driver strength */ 
  TS_DRIVE_STRENGTH_2X                 /* 2x transport stream driver strength */
} MXL_TS_DRIVE_STRENGTH_E;

typedef struct
{ 
  MXL_BOOL                    TsCtrl;             /* IN, Control to turn TS ON or OFF  */
  MXL_TS_DRIVE_STRENGTH_E     TsDriveStrength;    /* IN, Control MPEG drive strength */
} MXL_TS_CTRL_T, *PMXL_TS_CTRL_T;

/* MXL_DEV_GPO_PINS_CFG */
typedef enum
{
  MXL_GPO_0 = 0,                  /* GPO pin 0 */
  MXL_GPO_1,                      /* GPO pin 1 */
  MXL_GPO_NA                      /* INVALID GPO pin number */
} MXL_GPO_E;

typedef struct
{
  MXL_GPO_E           GpoPinId;           /* IN, GPIO Pin ID */
  MXL_BOOL            GpoPinCtrl;         /* IN, GPIO Pin control */
} MXL_DEV_GPO_CFG_T, *PMXL_DEV_GPO_CFG_T;

/* MXL_TUNER_SIGNAL_STRENGTH_REQ */
typedef struct
{
  SINT32 SignalStrength;                    /* OUT, Tuner Signal strength in dBm */
} MXL_SIGNAL_STATS_T, *PMXL_SIGNAL_STATS_T;

/* MXL_DEMOD_TPS_CELL_ID_REQ */
typedef struct 
{
  UINT16 TpsCellId;                      /* OUT, TPS Cell ID Info */
}MXL_DEMOD_CELL_ID_INFO_T, *PMXL_DEMOD_CELL_ID_INFO_T;

/* MXL_TUNER_CHECK_CHAN_STATUS_REQ */
typedef enum
{
  MXL_BREAK_AT_CP_LOCK = 1,       /* Break after CP Lock */
  MXL_BREAK_AT_TPS_LOCK,          /* Break after TPS Lock */
  MXL_BREAK_AT_RS_LOCK,           /* Break after RS Lock */
  MXL_FULL_MODE                   /* Default mode, break after FEC lcok */
}MXL_CHAN_SCAN_CTRL_E;

typedef struct
{
  MXL_CHAN_SCAN_CTRL_E ChanScanCtrl;   /* IN, value of Highest lock level check for a frequency */   
  UINT32 Frequency;                    /* IN, Frequency in MHz */
  UINT8 Bandwidth;                     /* IN, Channel Bandwidth in MHz */
  MXL_BOOL TpsCellIdRbCtrl;            /* IN, Enable TPS Cell ID Read Back feature */
  //MXL_BOOL IncreaseMaxTempRange;       /* IN, Enable or disable Max temp range while tuning */
  MXL_STATUS ChanPresent;              /* OUT, Indicates presence of a frequency */
} MXL_TUNER_CHECK_CHAN_REQ_T, *PMXL_TUNER_CHECK_CHAN_REQ_T;

/* MXL_TUNER_CHAN_OFFSET_REQ */
typedef struct 
{
  SINT32 ChanOffset;                      /* OUT, Frequency Offset for a Channel in Hz*/
}MXL_TUNER_CHAN_OFFSET_T, *PMXL_TUNER_CHAN_OFFSET_T;

/* MXL_DEMOD_SPECTRUM_CFG */
typedef struct 
{
  MXL_BOOL ChanScanCfg;                      /* IN, Enable or Disable Channel Scan */
}MXL_TUNER_CHAN_SCAN_CFG_T, *PMXL_TUNER_CHAN_SCAN_CFG_T;

// MXL_DEMOD_SPECTRUM_CFG 
typedef struct 
{
  MXL_BOOL SpectrumCfg;                      /* IN, Enable or Disable inverted signal processing */
}MXL_DEMOD_SPECTRUM_CFG_T, *PMXL_DEMOD_SPECTRUM_CFG_T;

// MXL_TUNER_FREQ_OFFSET_SRCH_RANGE_CFG 
typedef struct 
{
  MXL_BOOL MaxFreqOffsetRangeCfg;   /* IN, Enable or Disable Maximum frequency offset search range */
}MXL_TUNER_FREQ_OFFSET_CFG_T, *PMXL_TUNER_FREQ_OFFSET_CFG_T;

// MXL_DEMOD_RS_PKT_THRESH_CFG 
typedef struct 
{
  UINT8 RsPktThresh;                      /* IN, RS Pkt threshold for demod */
}MXL_DEMOD_RS_PKT_THRESH_T, *PMXL_DEMOD_RS_PKT_THRESH_T;
/*****************************************************************************************
    Global Variable Declarations
*****************************************************************************************/

/*****************************************************************************************
    Prototypes
*****************************************************************************************/

MXL_STATUS MxLWare_API_ConfigDevice(MXL_CMD_TYPE_E CmdType, void *ParamPtr);
MXL_STATUS MxLWare_API_GetDeviceStatus(MXL_CMD_TYPE_E CmdType, void *ParamPtr);
MXL_STATUS MxLWare_API_ConfigDemod(MXL_CMD_TYPE_E CmdType, void *ParamPtr);
MXL_STATUS MxLWare_API_GetDemodStatus(MXL_CMD_TYPE_E CmdType, void *ParamPtr);
MXL_STATUS MxLWare_API_ConfigTuner(MXL_CMD_TYPE_E CmdType, void *ParamPtr);
MXL_STATUS MxLWare_API_GetTunerStatus(MXL_CMD_TYPE_E CmdType, void *ParamPtr);

#endif /* __MXL1X1SF_PHY_CTRL_API_H__*/

