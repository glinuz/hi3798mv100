/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18250.h
 *
 *                3
 *
 * \date          %modify_time%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18250_Driver_User_Guide.pdf
 *
 * Detailed description may be added here.
 *
 * \section info Change Information
 *
*/

#ifndef _TMBSL_TDA18250_H
#define _TMBSL_TDA18250_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* TDA18250 Error Codes                                                       */
/*============================================================================*/

#define TDA18250_ERR_BASE               (CID_COMP_TUNER | CID_LAYER_BSL)
#define TDA18250_ERR_COMP               (CID_COMP_TUNER | CID_LAYER_BSL | TM_ERR_COMP_UNIQUE_START)

#define TDA18250_ERR_BAD_UNIT_NUMBER    (TDA18250_ERR_BASE + TM_ERR_BAD_UNIT_NUMBER)
#define TDA18250_ERR_NOT_INITIALIZED    (TDA18250_ERR_BASE + TM_ERR_NOT_INITIALIZED)
#define TDA18250_ERR_INIT_FAILED        (TDA18250_ERR_BASE + TM_ERR_INIT_FAILED)
#define TDA18250_ERR_BAD_PARAMETER      (TDA18250_ERR_BASE + TM_ERR_BAD_PARAMETER)
#define TDA18250_ERR_NOT_SUPPORTED      (TDA18250_ERR_BASE + TM_ERR_NOT_SUPPORTED)
#define TDA18250_ERR_HW_FAILED          (TDA18250_ERR_COMP + 0x0001)
#define TDA18250_ERR_NOT_READY          (TDA18250_ERR_COMP + 0x0002)
#define TDA18250_ERR_BAD_VERSION        (TDA18250_ERR_COMP + 0x0003)


/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

/* chips ID definition */
#define tmTDA18250_ID_ES4_A     0xD2
#define tmTDA18250_ID_ES4_B     0xD3
#define tmTDA18250_ID_ES4_C     0xD6
#define tmTDA18250_ID_ES4_D     0xD7
#define tmTDA18250_ID_ES4_E     0xD9

/* TDA18250 specific Power State: */
typedef enum _tmTDA18250PowerState_t
{
    tmTDA18250_PowerNormalMode = 0,                         /* Device normal mode */
    tmTDA18250_PowerStandbyWithLoopThroughOnAndWithXtalOn,  /* Device standby mode with Loop Through and Xtal Output */
    tmTDA18250_PowerStandbyWithXtalOn,                      /* Device standby mode with Xtal Output */
    tmTDA18250_PowerStandby,                                /* Device standby mode */
    tmTDA18250_PowerMax
} tmTDA18250PowerState_t, *ptmTDA18250PowerState_t;

/* TDA18250 specific Power State: */
typedef enum _tmTDA18250PowerTransitionMode_t
{
    tmTDA18250_PowerTransitionMode_Regular = 0,
    tmTDA18250_PowerTransitionMode_Smooth,
    tmTDA18250_PowerTransitionMode_Invalid
} tmTDA18250PowerTransitionMode_t, *ptmTDA18250PowerTransitionMode_t;

/* TDA18250 specific Power State: */
typedef enum _tmTDA18250_State_t
{
    tmTDA18250_OFF = 0,
    tmTDA18250_ON = 1,
    tmTDA18250_State_Max
} tmTDA18250_State_t, *ptmTDA18250_State_t;

typedef enum _tmTDA18250StandardMode_t
{
    tmTDA18250_DIG_6MHz = 0,                        /* Digital TV DVB-C 6MHz */
    tmTDA18250_DIG_7MHz = 1,                        /* Digital TV DVB-C 7MHz */
    tmTDA18250_DIG_8MHz = 2,                        /* Digital TV DVB-C 8MHz */
    tmTDA18250_DIG_9MHz = 3,                        /* Digital TV DVB-C 9MHz */
    tmTDA18250_DIG_xMHz = 4,                        /* Digital TV OOB */
    tmTDA18250_DIG_6MHz_256QAM,                     /* Digital TV DVB-C 6MHz 256QAM */
    tmTDA18250_DIG_7MHz_256QAM,                     /* Digital TV DVB-C 7MHz 256QAM */
    tmTDA18250_DIG_8MHz_256QAM,                     /* Digital TV DVB-C 8MHz 256QAM */
    tmTDA18250_DIG_9MHz_256QAM,                     /* Digital TV DVB-C 9MHz 256QAM */
    tmTDA18250_DIG_6MHz_64QAM,                      /* Digital TV DVB-C 6MHz 64QAM */
    tmTDA18250_DIG_7MHz_64QAM,                      /* Digital TV DVB-C 7MHz 64QAM */
    tmTDA18250_DIG_8MHz_64QAM,                      /* Digital TV DVB-C 8MHz 64QAM */
    tmTDA18250_DIG_9MHz_64QAM,                      /* Digital TV DVB-C 9MHz 64QAM */
    tmTDA18250_StandardMode_Max
} tmTDA18250StandardMode_t, *ptmTDA18250StandardMode_t;

typedef enum _tmTDA18250_AGC1_Gud_t
{
    tmTDA18250_AGC1_Gud_Up = 0,
    tmTDA18250_AGC1_Gud_Down = 1,
    tmTDA18250_AGC1_Gud_Max = 2
} tmTDA18250_AGC1_Gud_t, *ptmTDA18250_AGC1_Gud_t;

typedef enum _tmTDA18250_AGC1_Aup_t
{
    tmTDA18250_AGC1_Aup_Minus2dB = 0,
    tmTDA18250_AGC1_Aup_2dB = 1,
    tmTDA18250_AGC1_Aup_4_875dB = 2,
    tmTDA18250_AGC1_Aup_6_75dB = 3,
    tmTDA18250_AGC1_Aup_8_625dB = 4,
    tmTDA18250_AGC1_Aup_10_5dB = 5,
    tmTDA18250_AGC1_Aup_12_375dB = 6,
    tmTDA18250_AGC1_Aup_14_25dB = 7,
    tmTDA18250_AGC1_Aup_16_125dB = 8,
    tmTDA18250_AGC1_Aup_Max = 9
} tmTDA18250_AGC1_Aup_t, *ptmTDA18250_AGC1_Aup_t;

typedef enum _tmTDA18250_AGC1_Ado_t
{
    tmTDA18250_AGC1_Ado_2dB = 0,
    tmTDA18250_AGC1_Ado_4_875dB = 1,
    tmTDA18250_AGC1_Ado_6_75dB = 2,
    tmTDA18250_AGC1_Ado_8_625dB = 3,
    tmTDA18250_AGC1_Ado_10_5dB = 4,
    tmTDA18250_AGC1_Ado_12_375dB = 5,
    tmTDA18250_AGC1_Ado_14_25dB = 6,
    tmTDA18250_AGC1_Ado_16_125dB = 7,
    tmTDA18250_AGC1_Ado_18dB = 8,
    tmTDA18250_AGC1_Ado_Max = 9
} tmTDA18250_AGC1_Ado_t, *ptmTDA18250_AGC1_Ado_t;

typedef enum _tmTDA18250_BpFilter_t
{
    tmTDA18250_BP_Filter0 = 0,
    tmTDA18250_BP_Filter1 = 1,
    tmTDA18250_BP_Filter2 = 2,
    tmTDA18250_BP_Filter3 = 3,
    tmTDA18250_BP_Filter4 = 4,
    tmTDA18250_BP_Filter5 = 5,
    tmTDA18250_BP_Filter6 = 6,
    tmTDA18250_BP_Filter7 = 7,
    tmTDA18250_BP_Filter_Max = 8
} tmTDA18250_BpFilter_t, *ptmTDA18250_BpFilter_t;

typedef enum _tmTDA18250_AGC2_Detector_t
{
    tmTDA18250_AGC2_Detector2 = 1,
    tmTDA18250_AGC2_Detector3 = 2,
    tmTDA18250_AGC2_Detector4 = 3,
    tmTDA18250_AGC2_Detector_Max = 4
} tmTDA18250_AGC2_Detector_t, *ptmTDA18250_AGC2_Detector_t;

/* Switch Name */
typedef enum _tmTDA18250Switch_t
{
    tmTDA18250_RFSW_ANALOG,
    tmTDA18250_RFSW_LP,                            
    tmTDA18250_RFSW_HP,
    tmTDA18250_DTA_bypass,
    tmTDA18250_SW_Type_Max
} tmTDA18250Switch_t, *ptmTDA18250Switch_t;

typedef struct _tmTDA18250AnalogParameters_t
{
    UInt8   LF_nHF;
    UInt8   capw_gain;
    UInt8   capw_cad;
    UInt8   capw_ctank;
    UInt8   LF_freq;
    UInt8   Sel_3dB;
    UInt8   Shift_LF_rccal;
} tmTDA18250AnalogParameters_t, *ptmTDA18250AnalogParameters_t;

typedef enum _tmTDA18250FilterType_t
{
    tmTDA18250_FILTER_BYPASS = 0,
    tmTDA18250_FILTER_LPF,
    tmTDA18250_FILTER_BPF,
    tmTDA18250_FILTER_HPF,
    tmTDA18250_FILTER_ANLG,
    tmTDA18250_FILTER_Max
} tmTDA18250FilterType_t, *ptmTDA18250FilterType_t;


typedef struct _tmTDA18250Filter_t
{
    tmTDA18250FilterType_t type;
    UInt8                  number;
} tmTDA18250Filter_t, *ptmTDA18250Filter_t;

typedef enum _tmTDA18250ChannelType_t
{
    tmTDA18250AnalogChannel = 0,
    tmTDA18250DigitalChannel
} tmTDA18250ChannelType_t, *ptmTDA18250ChannelType_t;

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

extern tmErrorCode_t
tmbslTDA18250Init
(
    tmUnitSelect_t              tUnit,      /* I: Unit number      */
    tmbslFrontEndDependency_t*  psSrvFunc   /* I: setup parameters */
);

extern tmErrorCode_t 
tmbslTDA18250DeInit
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
);

extern tmErrorCode_t
tmbslTDA18250GetSWVersion
(
    ptmSWVersion_t  pSWVersion  /* I: Receives SW Version */
);

extern tmErrorCode_t
tmbslTDA18250_GetSWSettingsVersion
(
    ptmSWSettingsVersion_t pSWSettingsVersion   /* O: Receives SW Settings Version */
);

extern tmErrorCode_t
tmbslTDA18250SetPowerTransitionMode
(
    tmUnitSelect_t                  tUnit,                /* I: Unit number                   */
    tmTDA18250PowerTransitionMode_t ePowerTransitionMode  /* I: Power state mode              */
);

extern tmErrorCode_t
tmbslTDA18250SetPowerState
(
    tmUnitSelect_t          tUnit,      /* I: Unit number                */
    tmTDA18250PowerState_t  powerState  /* I: Power state of this device */
);

extern tmErrorCode_t
tmbslTDA18250GetPowerState
(
    tmUnitSelect_t            tUnit,        /* I: Unit number                */
    tmTDA18250PowerState_t    *pPowerState  /* O: Power state of this device */
);

extern tmErrorCode_t
tmbslTDA18250Reset
(
    tmUnitSelect_t tUnit    /* I: Unit number */
);

tmErrorCode_t
tmbslTDA18250SetStandardMode
(
    tmUnitSelect_t              tUnit,          /* I: Unit number                  */
    tmTDA18250StandardMode_t    StandardMode    /* I: Standard mode of this device */
);

extern tmErrorCode_t
tmbslTDA18250GetStandardMode
(
    tmUnitSelect_t              tUnit,          /* I: Unit number                  */
    tmTDA18250StandardMode_t    *pStandardMode  /* O: Standard mode of this device */
);

extern tmErrorCode_t
tmbslTDA18250SetRf
(
    tmUnitSelect_t  tUnit,  /* I: Unit number           */
    UInt32          uRF     /* I: RF frequency in hertz */
);

extern tmErrorCode_t
tmbslTDA18250GetRf
(
    tmUnitSelect_t  tUnit,  /*  I: Unit number           */
    UInt32*         pRF     /*  O: RF frequency in hertz */
);

extern tmErrorCode_t
tmbslTDA18250ReadLO
(
    tmUnitSelect_t  tUnit,  /* I: Unit number                         */
    UInt32          *puLO   /* O: local oscillator frequency in hertz */
);

extern tmErrorCode_t
tmbslTDA18250GetIF
(
    tmUnitSelect_t  tUnit,  /* I: Unit number           */
    UInt32*         puIF    /* O: IF Frequency in hertz */
);

extern tmErrorCode_t
tmbslTDA18250GetLockStatus
(
    tmUnitSelect_t          tUnit,      /* I: Unit number     */
    tmbslFrontEndState_t*   pLockStatus /* O: PLL Lock status */
);

extern tmErrorCode_t
tmbslTDA18250GetPowerLevel
(
    tmUnitSelect_t  tUnit,      /* I: Unit number         */
    Int32*          pPowerLevel /* O: Power Level in dBmV */
);

extern tmErrorCode_t
tmbslTDA18250ResetAGC
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
);

extern tmErrorCode_t
tmbslTDA18250SetIfLevel
(
    tmUnitSelect_t         tUnit,     /* I: Unit number */
    tmddTDA18250IF_Level_t uIF_Level  /* I: IF Level    */
);

#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA18250_H */
