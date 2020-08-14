/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18250A.h
 *
 * \date          %modify_time%
 *
 * \author        Alexandre TANT
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18250A_Driver_User_Guide.pdf
 *
 * \section info Change Information
 *
*/

#ifndef _TMBSL_TDA18250A_H
#define _TMBSL_TDA18250A_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* TDA18250A Error Codes                                                       */
/*============================================================================*/

#define TDA18250A_ERR_BASE                       (CID_COMP_TUNER | CID_LAYER_BSL)
#define TDA18250A_ERR_COMP                       (CID_COMP_TUNER | CID_LAYER_BSL | TM_ERR_COMP_UNIQUE_START)

#define TDA18250A_ERR_BAD_UNIT_NUMBER            (TDA18250A_ERR_BASE + TM_ERR_BAD_UNIT_NUMBER)
#define TDA18250A_ERR_ERR_NO_INSTANCES           (TDA18250A_ERR_BASE + TM_ERR_NO_INSTANCES)
#define TDA18250A_ERR_NOT_INITIALIZED            (TDA18250A_ERR_BASE + TM_ERR_NOT_INITIALIZED)
#define TDA18250A_ERR_ALREADY_SETUP              (TDA18250A_ERR_BASE + TM_ERR_ALREADY_SETUP)
#define TDA18250A_ERR_INIT_FAILED                (TDA18250A_ERR_BASE + TM_ERR_INIT_FAILED)
#define TDA18250A_ERR_BAD_PARAMETER              (TDA18250A_ERR_BASE + TM_ERR_BAD_PARAMETER)
#define TDA18250A_ERR_NOT_SUPPORTED              (TDA18250A_ERR_BASE + TM_ERR_NOT_SUPPORTED)
#define TDA18250A_ERR_NULL_CONTROLFUNC           (TDA18250A_ERR_BASE + TM_ERR_NULL_CONTROLFUNC)
#define TDA18250A_ERR_HW_FAILED                  (TDA18250A_ERR_COMP + 0x0001)
#define TDA18250A_ERR_NOT_READY                  (TDA18250A_ERR_COMP + 0x0002)
#define TDA18250A_ERR_BAD_VERSION                (TDA18250A_ERR_COMP + 0x0003)
#define TDA18250A_ERR_STD_NOT_SET                (TDA18250A_ERR_COMP + 0x0004)
#define TDA18250A_ERR_RF_NOT_SET                 (TDA18250A_ERR_COMP + 0x0005)

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/
#define TDA18250A_DVBC_SUPPORTED
#define TDA18250A_DVBT_SUPPORTED
#define TDA18250A_DTMB_SUPPORTED
#define TDA18250A_ISDBT_SUPPORTED
#define TDA18250A_ATSC_SUPPORTED

#define TDA18250A_CONFIG_STD_FREQ_NUM 8

typedef enum _TDA18250AVersion_t {
    TDA18250A_VersionES1 = 0,                                       /* ES1 */
    TDA18250A_VersionES2,                                           /* ES2 */
    TDA18250A_VersionES3,                                           /* ES3 */
    TDA18250A_VersionES4,                                           /* ES4 */
    TDA18250A_VersionMax
} TDA18250AVersion_t, *pTDA18250AVersion_t;

typedef struct _TDA18250A_AdditionnalData_t
{
    tmUnitSelect_t tOtherUnit; /* tUnit of the instance containing the tUnit of the other path */
}TDA18250A_AdditionnalData_t;

typedef enum _TDA18250APowerState_t {
    TDA18250A_PowerNormalMode = 0,                                 /* Device normal mode */
    TDA18250A_PowerStandbyWithXtalOn,                              /* Device standby mode with Xtal Output */
    TDA18250A_PowerStandbyWithLtOnWithXtalOn,                      /* Device standby mode with LT on and with Xtal Output */
    TDA18250A_PowerStandbyWithPllOnWithXtalOn,                     /* Device standby mode with PLL on and with Xtal Output */
    TDA18250A_PowerStandbyWithLtOnWithPllOnWithXtalOn,             /* Device standby mode with LT on with PLL on and with Xtal Output */
    TDA18250A_PowerStandbySmoothLTOnXtOutOn,
    TDA18250A_PowerStandbySmoothLTOffXtOutOff,
    TDA18250A_PowerStandbySmoothLTOffLNAOnXtOutOn,
    TDA18250A_PowerMax
} TDA18250APowerState_t, *pTDA18250APowerState_t;

typedef enum  _TDA18250AModeInUse_t {
	TDA18250A_Mode_Single = 0,
	TDA18250A_Mode_Dual = 1,
	TDA18250A_ModeMax = 3
} TDA18250AModeInUse_t, *pTDA18250AModeInUse_t;

typedef enum _TDA18250AStandardMode_t {
    TDA18250A_StandardMode_Unknown = 0,                  /* Unknown standard */
#ifdef TDA18250A_DVBC_SUPPORTED
    TDA18250A_QAM_6MHz,                                  /* Digital TV QAM 6MHz */
    TDA18250A_QAM_8MHz,                                  /* Digital TV QAM 8MHz */
    TDA18250A_QAM_6MHz_64QAM,
    TDA18250A_QAM_6MHz_256QAM,
    TDA18250A_QAM_8MHz_64QAM,
    TDA18250A_QAM_8MHz_256QAM,
#endif
#ifdef TDA18250A_DVBT_SUPPORTED
    TDA18250A_DVBT_1_7MHz,                               /* Digital TV DVB-T/T2 6MHz */
    TDA18250A_DVBT_6MHz,                                 /* Digital TV DVB-T/T2 6MHz */
    TDA18250A_DVBT_7MHz,                                 /* Digital TV DVB-T/T2 7MHz */
    TDA18250A_DVBT_8MHz,                                 /* Digital TV DVB-T/T2 8MHz */
    TDA18250A_DVBT_10MHz,                                /* Digital TV DVB-T/T2 10MHz */
#endif
#ifdef TDA18250A_DTMB_SUPPORTED
    TDA18250A_DTMB_8MHz,                                 /* Digital TV DMB-T 8MHz */
#endif
#ifdef TDA18250A_ISDBT_SUPPORTED
    TDA18250A_ISDBT_6MHz,                                /* Digital TV ISDB-T 6MHz*/
#endif
#ifdef TDA18250A_ANALOG_SUPPORTED
    TDA18250A_FM_Radio,                                  /* Analog FM Radio */
    TDA18250A_ANLG_MN,                                   /* Analog TV M/N */
    TDA18250A_ANLG_B,                                    /* Analog TV B */
    TDA18250A_ANLG_GH,                                   /* Analog TV G/H */
    TDA18250A_ANLG_I,                                    /* Analog TV I */
    TDA18250A_ANLG_DK,                                   /* Analog TV D/K */
    TDA18250A_ANLG_L,                                    /* Analog TV L */
    TDA18250A_ANLG_LL,                                   /* Analog TV L' */
    TDA18250A_Scanning,                                  /* Analog Preset Blind Scanning */
#endif
#ifdef TDA18250A_ATSC_SUPPORTED
    TDA18250A_ATSC_6MHz,
#endif
    TDA18250A_StandardMode_Max
} TDA18250AStandardMode_t, *pTDA18250AStandardMode_t;

/* Register Bit-Field Definition */
typedef struct _TDA18250A_BitField_t
{
    UInt8   Address;
    UInt8   PositionInBits;
    UInt8   WidthInBits;
    UInt8   Attributes;
}
TDA18250A_BitField_t, *pTDA18250A_BitField_t;

typedef enum _TDA18250AXtalFreq_t {
	TDA18250A_XtalFreq_Unknown,
	TDA18250A_XtalFreq_16000000,
	TDA18250A_XtalFreq_24000000,
	TDA18250A_XtalFreq_25000000,
	TDA18250A_XtalFreq_27000000,
	TDA18250A_XtalFreq_30000000
} TDA18250AXtalFreq_t, *pTDA18250AXtalFreq_t;

typedef enum _TDA18250AIF_Output_Level_t {
    TDA18250A_IF_Output_Level_2Vpp_0_30dB = 0,           /* 2Vpp       0 - 30dB      */
    TDA18250A_IF_Output_Level_1_25Vpp_min_4_26dB,        /* 1.25Vpp   -4 - 26dB      */
    TDA18250A_IF_Output_Level_1Vpp_min_6_24dB,           /* 1Vpp      -6 - 24dB      */
    TDA18250A_IF_Output_Level_0_8Vpp_min_8_22dB,         /* 0.8Vpp    -8 - 22dB      */
    TDA18250A_IF_Output_Level_0_85Vpp_min_7_5_22_5dB,    /* 0.85Vpp   -7.5 - 22.5dB  */
    TDA18250A_IF_Output_Level_0_7Vpp_min_9_21dB,         /* 0.7Vpp    -9 - 21dB      */
    TDA18250A_IF_Output_Level_0_6Vpp_min_10_3_19_7dB,    /* 0.6Vpp    -10.3 - 19.7dB */
    TDA18250A_IF_Output_Level_0_5Vpp_min_12_18dB,        /* 0.5Vpp    -12 - 18dB     */
    TDA18250A_IF_Output_Level_Max
} TDA18250AIF_Output_Level_t, *pTDA18250AIF_Output_Level_t;

typedef enum _TDA18250AScanning_Mode_t {
    TDA18250A_Scanning_Mode_Disabled = 0,
    TDA18250A_Scanning_Mode_Enabled,
    TDA18250A_Scanning_Mode_Max
} TDA18250AScanning_Mode_t, *pTDA18250AScanning_Mode_t;

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

tmErrorCode_t
tmbslTDA18250A_Open(
    tmUnitSelect_t              tUnit,      /*  I: Unit number */
    tmbslFrontEndDependency_t*  psSrvFunc   /*  I: setup parameters */
);

tmErrorCode_t
tmbslTDA18250A_HwInit(
    tmUnitSelect_t tUnit    /* I: Unit number */
);

tmErrorCode_t
tmbslTDA18250A_SetPowerState(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    tmPowerState_t  powerState  /* I: Power state */
);

tmErrorCode_t
tmbslTDA18250A_SetStandardMode(
    tmUnitSelect_t              tUnit,          /*  I: Unit number */
    TDA18250AStandardMode_t    StandardMode    /*  I: Standard mode of this device */
);

tmErrorCode_t
tmbslTDA18250A_SetRF(
    tmUnitSelect_t  tUnit,  /*  I: Unit number */
    UInt32          uRF     /*  I: RF frequency in hertz */
);

tmErrorCode_t
tmbslTDA18250A_Close(
    tmUnitSelect_t  tUnit   /*  I: Unit number */
);

tmErrorCode_t
tmbslTDA18250A_GetIF(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*         puIF    /* O: IF Frequency in hertz */
);

tmErrorCode_t
tmbslTDA18250A_SetIFLevel(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    TDA18250AIF_Output_Level_t  eIFLevel    /* I: IF Level */
);

tmErrorCode_t
tmbslTDA18250A_GetIFLevel(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    TDA18250AIF_Output_Level_t  *peIFLevel  /* I: IF Level */
);

tmErrorCode_t
tmbslTDA18250A_GetPowerLevel(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    Int32*          pPowerLevel /* O: Power Level in (x100) dBµVrms */
);

tmErrorCode_t
tmbslTDA18250A_CheckHWVersion(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    pTDA18250AVersion_t pEsVersion  /*  o: ES version of the HW */
);
tmErrorCode_t
tmbslTDA18250A_Channel_Switching_Complete(
    tmUnitSelect_t  tUnit      /* I: FrontEnd unit number */
);

#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA18250A_H */

