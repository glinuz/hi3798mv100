/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA182I5a.h
 *
 * \date          %modify_time%
 *
 * \author        Michael VANNIER
 *                
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA182I5a_Driver_User_Guide.pdf
 *
 *
 * \section info Change Information
 *
*/

#ifndef _TMBSL_TDA182I5a_H
#define _TMBSL_TDA182I5a_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* TDA182I5a Error Codes                                                       */
/*============================================================================*/

#define TDA182I5a_ERR_BASE                       (CID_COMP_TUNER | CID_LAYER_BSL)
#define TDA182I5a_ERR_COMP                       (CID_COMP_TUNER | CID_LAYER_BSL | TM_ERR_COMP_UNIQUE_START)

#define TDA182I5a_ERR_BAD_UNIT_NUMBER            (TDA182I5a_ERR_BASE + TM_ERR_BAD_UNIT_NUMBER)
#define TDA182I5a_ERR_ERR_NO_INSTANCES           (TDA182I5a_ERR_BASE + TM_ERR_NO_INSTANCES)
#define TDA182I5a_ERR_NOT_INITIALIZED            (TDA182I5a_ERR_BASE + TM_ERR_NOT_INITIALIZED)
#define TDA182I5a_ERR_ALREADY_SETUP              (TDA182I5a_ERR_BASE + TM_ERR_ALREADY_SETUP)
#define TDA182I5a_ERR_INIT_FAILED                (TDA182I5a_ERR_BASE + TM_ERR_INIT_FAILED)
#define TDA182I5a_ERR_BAD_PARAMETER              (TDA182I5a_ERR_BASE + TM_ERR_BAD_PARAMETER)
#define TDA182I5a_ERR_NOT_SUPPORTED              (TDA182I5a_ERR_BASE + TM_ERR_NOT_SUPPORTED)
#define TDA182I5a_ERR_NULL_CONTROLFUNC           (TDA182I5a_ERR_BASE + TM_ERR_NULL_CONTROLFUNC)
#define TDA182I5a_ERR_HW_FAILED                  (TDA182I5a_ERR_COMP + 0x0001)
#define TDA182I5a_ERR_NOT_READY                  (TDA182I5a_ERR_COMP + 0x0002)
#define TDA182I5a_ERR_BAD_VERSION                (TDA182I5a_ERR_COMP + 0x0003)
#define TDA182I5a_ERR_STD_NOT_SET                (TDA182I5a_ERR_COMP + 0x0004)
#define TDA182I5a_ERR_RF_NOT_SET                 (TDA182I5a_ERR_COMP + 0x0005)

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

typedef enum _TDA182I5aPowerState_t {
    TDA182I5a_PowerNormalMode = 0,                                 /* Device normal mode */
    TDA182I5a_PowerStandby,                                        /* Device standby mode full( only I2C ) */
    TDA182I5a_PowerStandby_LT,                                     /* Device standby mode with Loop Through */
    TDA182I5a_ImageRejection,                                      /* Device image rejection mode */
    TDA182I5a_PowerMax
} TDA182I5aPowerState_t, *pTDA182I5aPowerState_t;

typedef enum _TDA182I5aStandardMode_t {
    TDA182I5a_StandardMode_Unknown = 0,                  /* Unknown standard */
    TDA182I5a_QAM_6MHz,                                  /* Digital TV QAM 6MHz (TDA18225: not supported)*/
    TDA182I5a_QAM_8MHz,                                  /* Digital TV QAM 8MHz (TDA18225: not supported)*/
    TDA182I5a_ATSC_6MHz,                                 /* Digital TV ATSC 6MHz */
    TDA182I5a_ISDBT_6MHz,                                /* Digital TV ISDBT 6MHz */
    TDA182I5a_DVBT_1_7MHz,                               /* Digital TV DVB-T/T2 1.7MHz */
    TDA182I5a_DVBT_6MHz,                                 /* Digital TV DVB-T/T2 6MHz */
    TDA182I5a_DVBT_7MHz,                                 /* Digital TV DVB-T/T2 7MHz */
    TDA182I5a_DVBT_8MHz,                                 /* Digital TV DVB-T/T2 8MHz */
    TDA182I5a_DVBT_10MHz,                                /* Digital TV DVB-T/T2 10MHz */     
    TDA182I5a_DMBT_8MHz,                                 /* Digital TV DMB-T 8MHz */
    TDA182I5a_FM_Radio,                                  /* Analog FM Radio (TDA18225: not supported)*/
    TDA182I5a_ANLG_MN,                                   /* Analog TV M/N (TDA18225: not supported)*/
    TDA182I5a_ANLG_B,                                    /* Analog TV B (TDA18225: not supported)*/
    TDA182I5a_ANLG_GH,                                   /* Analog TV G/H (TDA18225: not supported)*/
    TDA182I5a_ANLG_I,                                    /* Analog TV I (TDA18225: not supported)*/
    TDA182I5a_ANLG_DK,                                   /* Analog TV D/K (TDA18225: not supported)*/
    TDA182I5a_ANLG_L,                                    /* Analog TV L (TDA18225: not supported)*/
    TDA182I5a_ANLG_LL,                                   /* Analog TV L' (TDA18225: not supported)*/
    TDA182I5a_StandardMode_Max
} TDA182I5aStandardMode_t, *pTDA182I5aStandardMode_t;

/* Register Bit-Field Definition */
typedef struct _TDA182I5a_BitField_t
{
    UInt8   Address;
    UInt8   PositionInBits;
    UInt8   WidthInBits;
    UInt8   Attributes;
}
TDA182I5a_BitField_t, *pTDA182I5a_BitField_t;

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I5a_Open(
    tmUnitSelect_t              tUnit,      /*  I: Unit number */
    tmbslFrontEndDependency_t*  psSrvFunc   /*  I: setup parameters */
);

tmErrorCode_t
tmbslTDA182I5a_Close(
    tmUnitSelect_t  tUnit   /*  I: Unit number */
);

tmErrorCode_t
tmbslTDA182I5a_GetSWVersion(
    ptmSWVersion_t  pSWVersion  /*  I: Receives SW Version */
);

tmErrorCode_t
tmbslTDA182I5a_GetSWSettingsVersion(
    ptmSWSettingsVersion_t pSWSettingsVersion   /* O: Receives SW Settings Version */
);

tmErrorCode_t
tmbslTDA182I5a_SetPowerState(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    tmPowerState_t  powerState  /* I: Power state */
);

tmErrorCode_t
tmbslTDA182I5a_GetPowerState(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    tmPowerState_t* pPowerState /* O: Power state */
);

tmErrorCode_t
tmbslTDA182I5a_SetStandardMode(
    tmUnitSelect_t              tUnit,          /*  I: Unit number */
    TDA182I5aStandardMode_t    StandardMode    /*  I: Standard mode of this device */
);

tmErrorCode_t
tmbslTDA182I5a_GetStandardMode(
    tmUnitSelect_t              tUnit,          /*  I: Unit number */
    TDA182I5aStandardMode_t    *pStandardMode  /*  O: Standard mode of this device */
);

tmErrorCode_t
tmbslTDA182I5a_SetRF(
    tmUnitSelect_t  tUnit,  /*  I: Unit number */
    UInt32          uRF     /*  I: RF frequency in hertz */
);

tmErrorCode_t
tmbslTDA182I5a_GetRF(
    tmUnitSelect_t  tUnit,  /*  I: Unit number */
    UInt32*         pRF     /*  O: RF frequency in hertz */
);

tmErrorCode_t
tmbslTDA182I5a_HwInit(
    tmUnitSelect_t tUnit    /* I: Unit number */
);

tmErrorCode_t
tmbslTDA182I5a_GetIF(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*         puIF    /* O: IF Frequency in hertz */
);

tmErrorCode_t
tmbslTDA182I5a_GetLockStatus(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    tmbslFrontEndState_t*   pLockStatus /* O: PLL Lock status */
);

tmErrorCode_t
tmbslTDA182I5a_GetPowerLevel(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8*          pPowerLevel /* O: Power Level in 1/2 steps dBµV */
);

tmErrorCode_t
tmbslTDA182I5a_GetThermo(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8*          pThermo /* O: temperature in °C */
);

/* You can only add one step (-1 or +1) at a time along -125 KHz <-> -62.5 KHz <-> 0 <-> 62.5 KHz <-> 125 KHz */
/* After calling tmbslTDA182I5a_SetRF, step is reset at 0 */
tmErrorCode_t
tmbslTDA182I5a_SetFineRF(
    tmUnitSelect_t tUnit,      /* I: Unit number */
    Int8           step        /* I: step (-1, +1) */
);

tmErrorCode_t
tmbslTDA182I5a_SetFineRF_Step(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    Int32           step_val  /* signed step value  in hz */
);

tmErrorCode_t
tmbslTDA182I5a_Write(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    const TDA182I5a_BitField_t*  pBitField,  /* I: Bitfield structure */
    UInt8                       uData,      /* I: Data to write */
    tmbslFrontEndBusAccess_t    eBusAccess  /* I: Access to bus */
);

tmErrorCode_t
tmbslTDA182I5a_Read(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    const TDA182I5a_BitField_t*  pBitField,  /* I: Bitfield structure */
    UInt8*                      puData,     /* I: Data to read */
    tmbslFrontEndBusAccess_t    eBusAccess  /* I: Access to bus */
);

/*============================================================================*/
/* Legacy compatibility:                                                      */
/*============================================================================*/

#ifndef tmbslTDA182I5aInit
 #define tmbslTDA182I5aInit tmbslTDA182I5a_Open
#endif

#ifndef tmbslTDA182I5aDeInit
 #define tmbslTDA182I5aDeInit tmbslTDA182I5a_Close
#endif

#ifndef tmbslTDA182I5aGetSWVersion
 #define tmbslTDA182I5aGetSWVersion tmbslTDA182I5a_GetSWVersion
#endif

#ifndef tmbslTDA182I5aGetSWSettingsVersion
 #define tmbslTDA182I5aGetSWSettingsVersion tmbslTDA182I5a_GetSWSettingsVersion
#endif

#ifndef tmbslTDA182I5aSetPowerState
 #define tmbslTDA182I5aSetPowerState tmbslTDA182I5a_SetPowerState
#endif

#ifndef tmbslTDA182I5aGetPowerState
 #define tmbslTDA182I5aGetPowerState tmbslTDA182I5a_GetPowerState
#endif

#ifndef tmbslTDA182I5aSetStandardMode
 #define tmbslTDA182I5aSetStandardMode tmbslTDA182I5a_SetStandardMode
#endif

#ifndef tmbslTDA182I5aGetStandardMode
 #define tmbslTDA182I5aGetStandardMode tmbslTDA182I5a_GetStandardMode
#endif

#ifndef tmbslTDA182I5aSetSpectralInversion
 #define tmbslTDA182I5aSetSpectralInversion tmbslTDA182I5a_SetSpectralInversion
#endif

#ifndef tmbslTDA182I5aGetSpectralInversion
 #define tmbslTDA182I5aGetSpectralInversion tmbslTDA182I5a_GetSpectralInversion
#endif

#ifndef tmbslTDA182I5aSetRf
 #define tmbslTDA182I5aSetRf tmbslTDA182I5a_SetRF
#endif

#ifndef tmbslTDA182I5aGetRf
 #define tmbslTDA182I5aGetRf tmbslTDA182I5a_GetRF
#endif

#ifndef tmbslTDA182I5aReset
 #define tmbslTDA182I5aReset tmbslTDA182I5a_HwInit
#endif

#ifndef tmbslTDA182I5aGetIF
 #define tmbslTDA182I5aGetIF tmbslTDA182I5a_GetIF
#endif

#ifndef tmbslTDA182I5aGetLockStatus
 #define tmbslTDA182I5aGetLockStatus tmbslTDA182I5a_GetLockStatus
#endif

#ifndef tmbslTDA182I5aGetPowerLevel
 #define tmbslTDA182I5aGetPowerLevel tmbslTDA182I5a_GetPowerLevel
#endif

#ifndef tmbslTDA182I5aRFFineTuning
 #define tmbslTDA182I5aRFFineTuning tmbslTDA182I5a_SetFineRF
#endif

#ifndef tmbslTDA182I5aWrite
 #define tmbslTDA182I5aWrite tmbslTDA182I5a_Write
#endif

#ifndef tmbslTDA182I5aRead
 #define tmbslTDA182I5aRead tmbslTDA182I5a_Read
#endif

#define tmTDA182I5aPowerState_t TDA182I5aPowerState_t
#define tmTDA182I5a_PowerNormalMode TDA182I5a_PowerNormalMode
#define tmTDA182I5a_PowerStandby TDA182I5a_PowerStandby
#define tmTDA182I5a_PowerStandby_LT TDA182I5a_PowerStandby_LT
#define tmTDA182I5a_ImageRejection TDA182I5a_ImageRejection
#define tmTDA182I5a_PowerMax TDA182I5a_PowerMax

#define tmTDA182I5aStandardMode_t TDA182I5aStandardMode_t
#define tmTDA182I5a_QAM_6MHz TDA182I5a_QAM_6MHz
#define tmTDA182I5a_QAM_8MHz TDA182I5a_QAM_8MHz
#define tmTDA182I5a_ATSC_6MHz TDA182I5a_ATSC_6MHz
#define tmTDA182I5a_ISDBT_6MHz TDA182I5a_ISDBT_6MHz
#define tmTDA182I5a_DVBT_1_7MHz TDA182I5a_DVBT_1_7MHz
#define tmTDA182I5a_DVBT_6MHz TDA182I5a_DVBT_6MHz
#define tmTDA182I5a_DVBT_7MHz TDA182I5a_DVBT_7MHz
#define tmTDA182I5a_DVBT_8MHz TDA182I5a_DVBT_8MHz
#define tmTDA182I5a_DVBT_10MHz TDA182I5a_DVBT_10MHz
#define tmTDA182I5a_DMBT_8MHz TDA182I5a_DMBT_8MHz
#define tmTDA182I5a_FM_Radio TDA182I5a_FM_Radio
#define tmTDA182I5a_ANLG_MN TDA182I5a_ANLG_MN
#define tmTDA182I5a_ANLG_B TDA182I5a_ANLG_B
#define tmTDA182I5a_ANLG_GH TDA182I5a_ANLG_GH
#define tmTDA182I5a_ANLG_I TDA182I5a_ANLG_I
#define tmTDA182I5a_ANLG_DK TDA182I5a_ANLG_DK
#define tmTDA182I5a_ANLG_L TDA182I5a_ANLG_L
#define tmTDA182I5a_ANLG_LL TDA182I5a_ANLG_LL
#define tmTDA182I5a_StandardMode_Max TDA182I5a_StandardMode_Max



#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA182I5a_H */

