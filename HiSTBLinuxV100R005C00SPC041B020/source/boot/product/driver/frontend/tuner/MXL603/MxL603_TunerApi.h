/*******************************************************************************
 *
 * FILE NAME          : MxL603_TunerApi.h
 * 
 * AUTHOR             : Dong Liu
 *
 * DATE CREATED       : 07/10/2011
 *                      11/26/2011
 *
 * DESCRIPTION        : This file is header file for MxL603_TunerApi.cpp 
 *
 *******************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MXL603_TUNER_API_H__
#define __MXL603_TUNER_API_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/

#include "MaxLinearDataTypes.h"

/******************************************************************************
    Macros
******************************************************************************/

#define MXL603_VERSION_SIZE     5

#define MXL603_MAX_NUM_DEVICES  4

/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/
 
typedef struct
{
  UINT8 chipId;                           /* OUT, Device chip ID information  */
  UINT8 chipVersion;                      /* OUT, Device chip revision  */
  UINT8 mxlwareVer[MXL603_VERSION_SIZE];  /* OUT, MxLWare version information */    
} MXL603_VER_INFO_T, *PMXL603_VER_INFO_T;

typedef enum
{
  MXL603_XTAL_16MHz = 0,
  MXL603_XTAL_24MHz
} MXL603_XTAL_FREQ_E;

typedef struct
{
  MXL603_XTAL_FREQ_E xtalFreqSel;  /* IN, XTAL Frequency, refers above */
  UINT8 xtalCap;                   /* IN, XTAL capacity, 1 LSB = 1pF, maximum is 31pF */
  MXL_BOOL clkOutEnable;           /* IN, enable or disable clock out */
  MXL_BOOL clkOutDiv;              /* IN, indicate if XTAL frequency is dived by 4 or not */
  MXL_BOOL clkOutExt;              /* IN, enable or disable external clock out*/
  MXL_BOOL singleSupply_3_3V;      /* IN, Single Supply 3.3v */
  MXL_BOOL XtalSharingMode;        /* IN, XTAL sharing mode. default Master, MXL_ENABLE to config Slave mode */
} MXL603_XTAL_SET_CFG_T, *PMXL603_XTAL_SET_CFG_T;

typedef enum
{
  MXL603_PWR_MODE_SLEEP = 0,      /* power saving mode - Sleep */
  MXL603_PWR_MODE_ACTIVE,         /* power saving mode - Active */
  MXL603_PWR_MODE_STANDBY         /* power saving mode - Standby */
} MXL603_PWR_MODE_E;    

typedef enum
{
  MXL603_GPO_LOW = 0,
  MXL603_GPO_HIGH = 1,
  MXL603_GPO_AUTO_CTRL = 2
} MXL603_GPO_STATE_E;         /* GPO control*/

typedef enum
{
  MXL603_DIG_DVB_C,         /* DVB-C mode */
  MXL603_DIG_ISDBT_ATSC,    /* ATSC/ISDB-T mode */
  MXL603_DIG_DVB_T,         /* DVB-T/DVB-T2 mode */
  MXL603_DIG_J83B           /* J.83B mode */
} MXL603_SIGNAL_MODE_E;   /* MxL603 Application mode */

typedef enum
{
  MXL603_IF_3_65MHz  = 0,
  MXL603_IF_4MHz     = 1,
  MXL603_IF_4_1MHz   = 2,
  MXL603_IF_4_15MHz  = 3,
  MXL603_IF_4_5MHz   = 4, 
  MXL603_IF_4_57MHz  = 5,
  MXL603_IF_5MHz     = 6,
  MXL603_IF_5_38MHz  = 7, 
  MXL603_IF_6Hz      = 8,
  MXL603_IF_6_28MHz  = 9,
  MXL603_IF_7_2MHz   = 10, 
  MXL603_IF_8_25MHz  = 11,
  MXL603_IF_35_25MHz = 12,
  MXL603_IF_36MHz    = 13, 
  MXL603_IF_36_15MHz = 14, 
  MXL603_IF_36_65MHz = 15,
  MXL603_IF_44MHz    = 16
} MXL603_IF_FREQ_E;       /* Pre-Define IF out values */

typedef struct
{
  MXL603_SIGNAL_MODE_E signalMode;       /* IN , Tuner application mode */
  MXL603_XTAL_FREQ_E xtalFreqSel;        /* XTAL Frequency, refers above */
  UINT32 ifOutFreqinKHz;                 /* IN, IF Frequency in KHz */
  UINT8 ifOutGainLevel;                  /* IF out gain level */ 
} MXL603_TUNER_MODE_CFG_T, *PMXL603_TUNER_MODE_CFG_T;

typedef enum
{
  MXL603_AGC_SELF = 0,   
  MXL603_AGC_EXTERNAL = 1 
} MXL603_AGC_TYPE_E;

typedef struct
{
  UINT8 setPoint;                   /* IN, AGC attack point set value */
  MXL_BOOL agcPolarityInverstion;   /* IN, Config AGC Polarity inversion */  
  MXL603_AGC_TYPE_E agcType;        /* IN, AGC mode selection, self or closed loop */
} MXL603_AGC_CFG_T, *PMXL603_AGC_CFG_T;

typedef enum
{
  MXL603_CABLE_BW_6MHz = 0x00,    /* Digital Cable Mode 6 MHz */      
  MXL603_CABLE_BW_7MHz = 0x01,    /* Digital Cable Mode 7 MHz */            
  MXL603_CABLE_BW_8MHz = 0x02,    /* Digital Cable Mode 8 MHz */              
  MXL603_TERR_BW_6MHz = 0x20,     /* Digital Terrestrial Mode 6 MHz */     
  MXL603_TERR_BW_7MHz = 0x21,     /* Digital Terrestrial Mode 7 MHz */    
  MXL603_TERR_BW_8MHz = 0x22      /* Digital Terrestrial Mode 8 MHz */    
} MXL603_BW_E;

typedef struct
{
  UINT32 freqInHz;                     /* IN, Radio Frequency in Hz */ 
  MXL_BOOL startTune;                  /* IN, MXL_ENABLE to start tune, MXL_DISABLE to Abort tune */
  MXL603_BW_E bandWidth;               /* IN, band width in MHz. Refer "MXL603_BW_E" */
  MXL603_SIGNAL_MODE_E signalMode;     /* IN, Tuner application mode */
  MXL603_XTAL_FREQ_E xtalFreqSel;      /* IN, Xtal frequency */
} MXL603_CHAN_TUNE_CFG_T, *PMXL603_CHAN_TUNE_CFG_T;

typedef struct
{
  MXL603_IF_FREQ_E ifOutFreq;       /* IN, band width of IF out signal */
  MXL_BOOL manualFreqSet;           /* IN, IF out frequency is set by manual or not */ 
  MXL_BOOL ifInversion;             /* IN, IF spectrum is inverted or not */
  UINT8 gainLevel;                  /* IN, IF out gain level */
  UINT32 manualIFOutFreqInKHz;      /* IN, IF out frequency selection when manualFreqSet = 0*/
}MXL603_IF_OUT_CFG_T, *PMXL603_IF_OUT_CFG_T;

typedef struct
{
	UINT8 i2c_address;
//	HI_U8 i2c_bus;
}user_data_t;

/******************************************************************************
    Global Variable Declarations
******************************************************************************/

/******************************************************************************
    Prototypes
******************************************************************************/
MXL_STATUS MxLWare603_API_CfgDrvInit(UINT8 devId, void* oemDataPtr);

MXL_STATUS MxLWare603_API_CfgDevSoftReset(UINT8 devId);
MXL_STATUS MxLWare603_API_CfgDevOverwriteDefaults(UINT8 devId, MXL_BOOL singleSupply_3_3V);
MXL_STATUS MxLWare603_API_CfgDevXtal(UINT8 devId, MXL603_XTAL_SET_CFG_T xtalCfg);
MXL_STATUS MxLWare603_API_CfgDevPowerMode(UINT8 devId, MXL603_PWR_MODE_E powerMode);
MXL_STATUS MxLWare603_API_CfgDevGPO(UINT8 devId, MXL603_GPO_STATE_E gpoState);

MXL_STATUS MxLWare603_API_ReqDevVersionInfo(UINT8 devId, MXL603_VER_INFO_T* mxlDevVerInfoPtr);
MXL_STATUS MxLWare603_API_ReqDevGPOStatus(UINT8 devId, MXL603_GPO_STATE_E* gpoStatusPtr);

MXL_STATUS MxLWare603_API_CfgTunerMode(UINT8 devId, MXL603_TUNER_MODE_CFG_T tunerModeCfg);
MXL_STATUS MxLWare603_API_CfgTunerAGC(UINT8 devId, MXL603_AGC_CFG_T agcCfg);
MXL_STATUS MxLWare603_API_CfgTunerLoopThrough(UINT8 devId, MXL_BOOL loopThroughCtrl);
MXL_STATUS MxLWare603_API_CfgTunerIFOutParam(UINT8 devId, MXL603_IF_OUT_CFG_T ifOutCfg);
MXL_STATUS MxLWare603_API_CfgTunerChanTune(UINT8 devId, MXL603_CHAN_TUNE_CFG_T chanTuneCfg);

MXL_STATUS MxLWare603_API_ReqTunerAGCLock(UINT8 devId, MXL_BOOL* agcLockStatusPtr);
MXL_STATUS MxLWare603_API_ReqTunerLockStatus(UINT8 devId, MXL_BOOL* rfLockPtr, MXL_BOOL* refLockPtr);
MXL_STATUS MxLWare603_API_ReqTunerRxPower(UINT8 devId, SINT16* rxPwrPtr);

#endif /* __MXL603_TUNER_API_H__*/




