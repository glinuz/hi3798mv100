/*
  Copyright (C) 2012 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA182I5a_Local.h
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
 * \par Changelog
 *  -2.1.3 RFT423 provide a way to control IFAGC inversion
 *     
 *  -2.1.2 RFT421 tmbslTDA182I5a_GetIF returns bad value when spectral inversion is activated
 *  -2.1.1 RFT382 remove DIVERSITY_ADAPTICP
 *  -2.1.0 New PLD coefficients for analogue standards, settings changes (top AGC1, top AGC2, AGC2 HighPass) and new diversity handling to allow parameters to be frequency/standard dependant in instancecustom file.
 *  -2.0.1 New AGC1 top and AGC2 HighPass settings for Analog DK and new AGC2 top for QAM 8MHz
 *  -2.0.0 Change IFnotchToRSSI management and setting improvements
 *  -1.0.10 RFT326; use SubLo + new tuner settings
 *  -1.0.9 RFT324: improve MSG test QAM8
 *  -1.0.8 RTF323: improve PLD measurement repeatability
 *  -1.0.7 RSSI calc update
 *  -1.0.7 RSSI calc update
 *  -1.0.7 RSSI calc update.
 *  -1.0.6 RFT283: change PSM IF Poly for all Digital standards except ATSC and DTMB.
 *  -1.0.5 VCO calibration changes
 *  -1.0.4 PAL G/H AGC2 top setting changed
 *  -1.0.3 Setting changes QAM8
 *  -1.0.2 Changes for QAM 6/8Mhz after first Eval
 *  -1.0.1 Spectral Inversion correction
 *     VCO 2.8V
 *     VCO selection minor bug correction report from 275
 *     Correction for AGC2 Top Strategy in PAL B
 *     
 *  -1.0.0 Fisrt delivery to test first chips
 *  -0.0.1 Initial version based on TDA18275
 *
 *
 * \par Version_Definition
 *  VERSION_TAG:TDA182I5a_COMP_NUM.TDA182I5a_MAJOR_VER.TDA182I5a_MINOR_VER
 *
*/

#ifndef _TMBSL_TDA182I5a_LOCAL_H
#define _TMBSL_TDA182I5a_LOCAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/


/* Driver version definition */
#define TDA182I5a_COMP_NUM  2  /* Major protocol change - Specification change required */
#define TDA182I5a_MAJOR_VER 1  /* Minor protocol change - Specification change required */
#define TDA182I5a_MINOR_VER 3  /* Software update - No protocol change - No specification change required */

/* Instance macros */
#define P_OBJ_VALID                             (pObj != Null)

/* I/O Functions macros */
#define P_SIO                                   pObj->sIo
#define P_SIO_READ                              P_SIO.Read
#define P_SIO_WRITE                             P_SIO.Write
#define P_SIO_READ_VALID                        (P_OBJ_VALID && (P_SIO_READ != Null))
#define P_SIO_WRITE_VALID                       (P_OBJ_VALID && (P_SIO_WRITE != Null))

/* Time Functions macros */
#define P_STIME                                 pObj->sTime
#define P_STIME_WAIT                            P_STIME.Wait
#define P_STIME_WAIT_VALID                      (P_OBJ_VALID && (P_STIME_WAIT != Null))

/* Debug Functions macros */
#define P_SDEBUG                                pObj->sDebug
#define P_DBGPRINTEx                            P_SDEBUG.Print
#define P_DBGPRINTVALID                         (P_OBJ_VALID && (P_DBGPRINTEx != Null))

/* Mutex Functions macros */
#define P_SMUTEX                                pObj->sMutex
#ifdef _TVFE_SW_ARCH_V4
 #define P_SMUTEX_OPEN                           P_SMUTEX.Open
 #define P_SMUTEX_CLOSE                          P_SMUTEX.Close
#else
 #define P_SMUTEX_OPEN                           P_SMUTEX.Init
 #define P_SMUTEX_CLOSE                          P_SMUTEX.DeInit
#endif
#define P_SMUTEX_ACQUIRE                        P_SMUTEX.Acquire
#define P_SMUTEX_RELEASE                        P_SMUTEX.Release

#define P_SMUTEX_OPEN_VALID                     (P_OBJ_VALID && (P_SMUTEX_OPEN != Null))
#define P_SMUTEX_CLOSE_VALID                    (P_OBJ_VALID && (P_SMUTEX_CLOSE != Null))
#define P_SMUTEX_ACQUIRE_VALID                  (P_OBJ_VALID && (P_SMUTEX_ACQUIRE != Null))
#define P_SMUTEX_RELEASE_VALID                  (P_OBJ_VALID && (P_SMUTEX_RELEASE != Null))

/* Driver Mutex macros */
#define TDA182I5a_MUTEX_TIMEOUT                  (5000)
#define P_MUTEX                                 pObj->pMutex
#define P_MUTEX_VALID                           (P_MUTEX != Null)

#ifdef _TVFE_IMPLEMENT_MUTEX
 #define _MUTEX_ACQUIRE(_NAME) \
     if(err == TM_OK) \
     { \
         /* Try to acquire driver mutex */ \
         err = i##_NAME##_MutexAcquire(pObj, _NAME##_MUTEX_TIMEOUT); \
     } \
     if(err == TM_OK) \
     {

 #define _MUTEX_RELEASE(_NAME) \
         (void)i##_NAME##_MutexRelease(pObj); \
     }
#else
 #define _MUTEX_ACQUIRE(_NAME) \
     if(err == TM_OK) \
     {

 #define _MUTEX_RELEASE(_NAME) \
     }
#endif

    
#define TDA182I5a_INSTANCE_REGMAP_DEF \
    { \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  \
    }
#define TDA182I5a_Identity 0x275A
#define TDA182I5a_VCO_LOW_MIN 5700000
#define TDA182I5a_VCO_LOW_MAX 7800000
#define TDA182I5a_VCO_MED_MIN 6400000
#define TDA182I5a_VCO_MED_MAX 8700000
#define TDA182I5a_VCO_HIGH_MIN 7100000
#define TDA182I5a_VCO_HIGH_MAX 9300000

#define TDA182I5_Mode_LTO_Used 0x01
#define TDA182I5_Mode_LTO_Not_Used 0x00
#define TDA182I5_Mode_XTOUT_Used 0x02
#define TDA182I5_Mode_XTOUT_Not_Used 0x00


/* TDA182I5a Driver State Machine */
typedef enum _TDA182I5aHwState_t {
    TDA182I5a_HwState_Unknown = 0,   /* Hw State Unknown */
    TDA182I5a_HwState_InitNotDone,   /* Hw Init Not Done */
    TDA182I5a_HwState_InitPending,   /* Hw Init Pending */
    TDA182I5a_HwState_InitDone,      /* Hw Init Done */
    TDA182I5a_HwState_SetStdDone,    /* Set Standard Done */
    TDA182I5a_HwState_SetRFDone,     /* Set RF Done */
    TDA182I5a_HwState_SetFineRFDone, /* Set Fine RF Done */
    TDA182I5a_HwState_Max
} TDA182I5aHwState_t, *pTDA182I5aHwState_t;

typedef enum _TDA182I5aHwStateCaller_t {
    TDA182I5a_HwStateCaller_Unknown = 0, /* Caller Unknown */
    TDA182I5a_HwStateCaller_SetPower,    /* Caller SetPowerState */
    TDA182I5a_HwStateCaller_HwInit,      /* Caller HwInit */
    TDA182I5a_HwStateCaller_SetStd,      /* Caller SetStandardMode */
    TDA182I5a_HwStateCaller_SetRF,       /* Caller SetRF */
    TDA182I5a_HwStateCaller_SetFineRF,   /* Caller SetFineRF */
    TDA182I5a_HwStateCaller_GetRSSI,     /* Caller GetRSSI */
    TDA182I5a_HwStateCaller_SetRawRF,    /* Caller SetRawRF */
    TDA182I5a_HwStateCaller_Max
} TDA182I5aHwStateCaller_t, *pTDA182I5aHwStateCaller_t;

/* TDA182I5a specific  TDA182I5a MSM */
typedef enum _TDA182I5aMSM_t {
    TDA182I5a_MSM_VCO_Freq_Meas       = 0x01,
    TDA182I5a_MSM_VCO_Ampl_Cal        = 0x02,  
    TDA182I5a_MSM_RC_Cal              = 0x04,
    TDA182I5a_MSM_IR_CAL_Image        = 0x08,
    TDA182I5a_MSM_IR_Cal_Loop         = 0x10,
    TDA182I5a_MSM_RSSI_Meas           = 0x20
} TDA182I5aMSM_t, *pTDA182I5aMSM_t;

typedef enum _TDA182I5aPowerSavingMode_t {
    TDA182I5aPowerSavingMode_Digital_nominal = 0,
    TDA182I5aPowerSavingMode_Analog_nominal = 1,
    TDA182I5aPowerSavingMode_QAM_nominal = 2,
    TDA182I5aPowerSavingMode_ATSC_nominal = 3,
    TDA182I5aPowerSavingMode_DTMB_nominal = 4,
    TDA182I5aPowerSavingMode_Max
} TDA182I5aPowerSavingMode_t, *pTDA182I5aPowerSavingMode_t;

typedef enum _TDA182I5a_HighPass_AGC2_t {
    TDA182I5a_HighPass_AGC2_Digital = 0,
    TDA182I5a_HighPass_AGC2_Analog = 1,
    TDA182I5a_HighPass_AGC2_Analog_GH = 2,
    TDA182I5a_HighPass_AGC2_QAM6 = 3,
    TDA182I5a_HighPass_AGC2_QAM8 = 4,
    TDA182I5a_HighPass_AGC2_20MHZ = 5,
    TDA182I5a_HighPass_AGC2_Max
} TDA182I5a_HighPass_AGC2_t , *pTDA182I5a_HighPass_AGC2_t;


typedef struct _TDA182I5aStdSettings
{
    /****************************************************************/
    /* IF Settings                                                  */
    /****************************************************************/
    UInt32                                      IF;                         /* IF Frequency */
    UInt32                                      Offset_fmax;
    TDA182I5a_hrm_swap_t                        SpectralInversion;          /* Spectral Inversion */

    /****************************************************************/
    /* IF SELECTIVITY Settings                                      */
    /****************************************************************/
    TDA182I5a_LP_Fc_t                               LPF;                        /* LPF Cut off */
    TDA182I5a_IF_LPF_Offset_t                       LPF_Offset;                 /* LPF offset */
    TDA182I5a_IF_LPF_sign_t                         LPF_sign;                   /* LPF offset sign */
    TDA182I5a_DC_Notch_t                            DC_Notch_IF_PPF;            /* DC notch IF PPF */
    TDA182I5a_IF_HP_Enable_t                        HP_Enable;                  /* Hi Pass Enable */
    TDA182I5a_IF_HP_Fc_t                            HPF;                        /* Hi Pass */
    TDA182I5a_IF_HPF_Offset_t                       HPF_Offset;                 /* HPF offset */
    TDA182I5a_IF_HPF_sign_t                         HPF_sign;                   /* HPF offset sign */
    TDA182I5a_IF_Notch_Enable_t                     IF_Notch_Enable;            /* IF_Notch_Enable */
    TDA182I5a_IF_Notch_Freq_t                       IF_Notch_Freq;              /* IF Notch Freq */
    TDA182I5a_IF_Notch_Offset_t                     IF_Notch_Offset;            /* IF Notch Offset */
    TDA182I5a_IF_Notch_sign_t                       IF_Notch_sign;              /* IF Notch Offset sign */
    TDA182I5a_IFnotchToRSSI_t                       IFnotchToRSSI;              /* IFnotchToRSSI */

    /****************************************************************/
    /* AGC  Settings                                                */
    /****************************************************************/
    TDA182I5a_AGC1_Top_t                            AGC1_TOP_I2C_DN_UP;         /* AGC1 TOP I2C DN/UP */
    TDA182I5a_AGC1_Top_Strategy_t                   AGC1_TOP_STRATEGY;          /* AGC1 TOP STRATEGY */
    TDA182I5a_AGC1_Do_step_t                        AGC1_Do_step;               /* AGC1_Do_step */
    TDA182I5a_AGC2_TOP_t                            AGC2_TOP_I2C_DN_UP;         /* AGC2 TOP I2C DN/UP */
    TDA182I5a_AGC2_Top_Strategy_t                   AGC2_Top_Strategy;          /* AGC2 TOP STRATEGY */
    TDA182I5a_AGC2_Do_step_t                        AGC2_Do_step;               /* AGC2_Do_step */
    TDA182I5a_AGC2_Up_step_t                        AGC2_Up_step;               /* AGC2_Up_step */
    TDA182I5a_AGC4_TOP_t                            AGC4_TOP_I2C_DN_UP;         /* AGC4 TOP I2C DN/UP */
    TDA182I5a_AGC4_Do_step_t                        AGC4_Do_step;               /* AGC4_Do_step */
    TDA182I5a_AGC5_TOP_t                            AGC5_TOP_I2C_DN_UP;         /* AGC5 TOP I2C DN/UP */
    TDA182I5a_AGC5_Do_step_t                        AGC5_Do_step;               /* AGC5_Do_step */
    TDA182I5a_AGC145_Up_step_t                      AGC145_Up_step;             /* AGC145_Up_step */
    TDA182I5a_Top_adapt_AGC1_t                      AGC1_Adapt_TOP;             /* AGC1 TOP adapt */
    TDA182I5a_Top_adapt_AGC2_t                      AGC2_Adapt_TOP;             /* AGC2 TOP adapt */
    TDA182I5a_IF_level_t                            IF_Output_Level;            /* IF Output Level */
    TDA182I5a_AGC_Ovld_TOP_t                        AGC_Overload_TOP;           /* AGC Overload TOP */
    Bool                                            AGC_Fast_RF_Ovld;           /* AGC fast RF Overload */
    Bool                                            AGC_Fast_RF_Udld;           /* AGC fast RF Underload */
    Bool                                            AGC_Fast_IF_Ovld;           /* AGC fast IF Overload */
    Bool                                            AGC_Fast_IF_Udld;           /* AGC fast IF Underload */
    Bool                                            AGC1_cc_Freeze_Enable;      /* AGC1_cc_Freeze */
    Bool                                            AGC1_cc_Wide_Enable;        /* AGC1_cc_Wide_Enable */
    TDA182I5a_PD_Vsync_Mgt_t                        PD_Vsync_Mgt;               /* PD_Vsync_Mgt */
    TDA182I5a_Neg_Modulation_t                      Neg_Modulation;             /* Negative modulation */
    TDA182I5a_AGCK_Mode_t                           AGCK_Mode;                  /* AGCK Mode */
    TDA182I5a_AGC5_HP_Fc_t                          AGC5_HP_Fc;                 /* AGC5_HP_Fc */
    TDA182I5a_RSSI_HP_Fc_t                          RSSI_HP_Fc;                 /* RSSI_HP_Fc */
    TDA182I5a_Det6_Cap_Val_t                        Det6_Cap_Val;               /* Det6_Cap_Val */
    TDA182I5a_RSSI_AV_t                             RSSI_AV;                    /* RSSI_AV */
    TDA182I5a_RSSI_Ck_Speed_t                       RSSI_Ck_Speed;              /* RSSI_Ck_Speed*/
    TDA182I5a_LNA_Highpass_t                        LNA_Highpass;               /* LNA_Highpass */
    TDA182I5a_HighPass_AGC2_t                       HighPass_AGC2_algo;         /* HighPass_AGC2 algo  */ 
    UInt8                                           cp_cursel_algo;             /* current charge pump selection algo */
    TDA182I5a_DIFOUT_En_t                           DIFOUT_En;                  /* DIFOUT_En */
    UInt8                                           DVIFAGC_En;                 /* DVIFAGC_En */
    TDA182I5aPowerSavingMode_t					PowerSavingMode;            /* normal or low */

} TDA182I5aStdSettings, *pTDA182I5aStdSettings;

typedef struct _TDA182I5a_VCO
{
    UInt32 fmax;
    UInt32 fmin;
} TDA182I5a_VCO, *pTDA182I5a_VCO;

typedef struct _TDA182I5aObject_t
{
    tmUnitSelect_t                  tUnit;
    tmUnitSelect_t                  tUnitW;
    ptmbslFrontEndMutexHandle       pMutex;
    Bool                            init;
    tmbslFrontEndIoFunc_t           sIo;
    tmbslFrontEndTimeFunc_t         sTime;
    tmbslFrontEndDebugFunc_t        sDebug;
    tmbslFrontEndMutexFunc_t        sMutex;
    /* Device specific part: */
    UInt16                          Identity;
    tmPowerState_t                  curPowerState;
    TDA182I5aPowerState_t           curLLPowerState;
    TDA182I5aPowerState_t           mapLLPowerState[tmPowerMax];
    TDA182I5aPowerSavingMode_t      curPowerSavingMode;
    TDA182I5a_VCO                   VCO[3];
    UInt8                           Temp_Meas;
    UInt32                          uRF;
    UInt32                          uIF;
    UInt32                          uLO;
    TDA182I5a_hrm_swap_t            SpectralInversion;          /* Spectral Inversion */
    UInt32                          uFVCO;
    TDA182I5aStandardMode_t         StandardMode;
    pTDA182I5aStdSettings           pStandard;
    UInt8                           TunerModeInUse;     /* define LTO/XTOUT option */
    /* Settings independant of standard */
    TDA182I5aHwState_t              eHwState;                /* Indicates HwInit state */
    UInt32                          uLUCodeProgrammed;
    /* Settings dependant of standard */
    TDA182I5aStdSettings            Std_Array[TDA182I5a_StandardMode_Max-1];
#ifdef _TDA182I5a_REGMAP_BITFIELD_DEFINED
    TDA182I5a_Reg_Map_t             RegMap;
#else
    UInt8                           RegMap[TDA182I5a_REG_MAP_NB_BYTES];
#endif
} TDA182I5aObject_t, *pTDA182I5aObject_t, **ppTDA182I5aObject_t;


/*============================================================================*/
/* Internal Prototypes:                                                       */
/*============================================================================*/

extern tmErrorCode_t
iTDA182I5a_GetIdent(pTDA182I5aObject_t pObj, UInt16* puIdentity, UInt8* puMajorRev, UInt8* puMinorRev);

extern tmErrorCode_t
iTDA182I5a_CheckCalcPLL(pTDA182I5aObject_t pObj);

extern tmErrorCode_t
iTDA182I5a_CheckHwState(pTDA182I5aObject_t pObj, TDA182I5aHwStateCaller_t caller);

extern tmErrorCode_t
iTDA182I5a_SetRF(pTDA182I5aObject_t pObj);

extern tmErrorCode_t
iTDA182I5a_SetRF_NoFast(pTDA182I5aObject_t pObj);

extern tmErrorCode_t
iTDA182I5a_SetLO(pTDA182I5aObject_t pObj);

extern tmErrorCode_t
iTDA182I5a_ProgramLPF(pTDA182I5aObject_t pObj, UInt32 uIndex);

extern tmErrorCode_t
iTDA182I5a_SetRF_Dependencies(pTDA182I5aObject_t pObj);

extern tmErrorCode_t
iTDA182I5a_SetRF_Freq(pTDA182I5aObject_t pObj);


extern tmErrorCode_t iTDA182I5a_SetPLL(pTDA182I5aObject_t pObj);

extern tmErrorCode_t iTDA182I5a_Adapt_vco_swingctrl(pTDA182I5aObject_t pObj, UInt8  adc_vtune, UInt8   VCO_select);

extern tmErrorCode_t iTDA182I5a_Force_vco_currentctrl(pTDA182I5aObject_t pObj, UInt8  adc_vtune, UInt8   VCO_select);

extern tmErrorCode_t
iTDA182I5a_MSMLaunch( pTDA182I5aObject_t pObj, UInt8 uValue);

extern tmErrorCode_t
iTDA182I5a_AdaptICP(pTDA182I5aObject_t pObj, UInt8 uadc_vtune, UInt8 uVCO_select );


extern tmErrorCode_t
iTDA182I5a_SetLLPowerState(
    pTDA182I5aObject_t       pObj,       /* I: Driver object */
    TDA182I5aPowerState_t    powerState  /* I: Power state of TDA182I5a */
);
extern tmErrorCode_t
iTDA182I5a_WaitIRQ(pTDA182I5aObject_t pObj, UInt32 timeOut, UInt32 waitStep);

 tmErrorCode_t
iTDA182I5a_Write(pTDA182I5aObject_t pObj, const TDA182I5a_BitField_t* pBitField, UInt8 uData, tmbslFrontEndBusAccess_t eBusAccess);

extern tmErrorCode_t
iTDA182I5a_Read(pTDA182I5aObject_t pObj, const TDA182I5a_BitField_t* pBitField, UInt8* puData, tmbslFrontEndBusAccess_t eBusAccess);

extern tmErrorCode_t
iTDA182I5a_WriteRegMap(pTDA182I5aObject_t pObj, UInt8 uAddress, UInt32 uWriteLen);

extern tmErrorCode_t
iTDA182I5a_ReadRegMap(pTDA182I5aObject_t pObj, UInt8 uAddress, UInt32 uReadLen);

extern tmErrorCode_t
iTDA182I5a_Wait(pTDA182I5aObject_t pObj, UInt32 Time);

extern tmErrorCode_t 
iTDA182I5a_PowerSavingMode( pTDA182I5aObject_t   pObj, TDA182I5aPowerSavingMode_t PowerSavingMode);

extern tmErrorCode_t 
iTDA182I5a_GetIdentity(pTDA182I5aObject_t   pObj, UInt16*  puIdentity);

#ifdef _TVFE_IMPLEMENT_MUTEX
 extern tmErrorCode_t iTDA182I5a_MutexAcquire(pTDA182I5aObject_t pObj, UInt32 timeOut);
 extern tmErrorCode_t iTDA182I5a_MutexRelease(pTDA182I5aObject_t pObj);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA182I5a_LOCAL_H */

