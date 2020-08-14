/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmddTDA18250.h
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

#ifndef _TMDD_TDA18250_H
#define _TMDD_TDA18250_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* ddTDA18250 Error Codes                                                     */
/*============================================================================*/

/* SW Error codes */
#define ddTDA18250_ERR_BASE               (CID_COMP_TUNER | CID_LAYER_HWAPI)
#define ddTDA18250_ERR_COMP               (CID_COMP_TUNER | CID_LAYER_HWAPI | TM_ERR_COMP_UNIQUE_START)

#define ddTDA18250_ERR_BAD_UNIT_NUMBER    (ddTDA18250_ERR_BASE + TM_ERR_BAD_UNIT_NUMBER)
#define ddTDA18250_ERR_NOT_INITIALIZED    (ddTDA18250_ERR_BASE + TM_ERR_NOT_INITIALIZED)
#define ddTDA18250_ERR_INIT_FAILED        (ddTDA18250_ERR_BASE + TM_ERR_INIT_FAILED)
#define ddTDA18250_ERR_BAD_PARAMETER      (ddTDA18250_ERR_BASE + TM_ERR_BAD_PARAMETER)
#define ddTDA18250_ERR_NOT_SUPPORTED      (ddTDA18250_ERR_BASE + TM_ERR_NOT_SUPPORTED)
#define ddTDA18250_ERR_HW_FAILED          (ddTDA18250_ERR_COMP + 0x0001)
#define ddTDA18250_ERR_NOT_READY          (ddTDA18250_ERR_COMP + 0x0002)
#define ddTDA18250_ERR_BAD_VERSION        (ddTDA18250_ERR_COMP + 0x0003)


/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

/* ddTDA18250 specific Power State: */
typedef enum _tmddTDA18250PowerState_t
{
    tmddTDA18250_PowerNormalMode,                             /* Device normal mode */
    tmddTDA18250_PowerStandbyWithLoopThroughOnAndWithXtalOn,  /* Device standby mode with Loop Through and Xtal Output */
    tmddTDA18250_PowerStandbyWithXtalOn,                      /* Device standby mode with Xtal Output */
    tmddTDA18250_PowerStandby,                                /* Device standby mode */
    tmddTDA18250_PowerMax
} tmddTDA18250PowerState_t, *ptmddTDA18250PowerState_t;

/* ddTDA18250 specific device type: */
typedef enum _tmddTDA18250DeviceType_t
{
    tmddTDA18250_DeviceTypeMasterMulti = 0,                    /* TDA18250 Master with slave(s) Device */
    tmddTDA18250_DeviceTypeMasterMultiWoXTout = 1,             /* TDA18250 Master with slave(s) Device Without external clock */
    tmddTDA18250_DeviceTypeMasterSingleWithLTWithXTout = 2,    /* TDA18250 Master Single Device With Loop through and external clock */
    tmddTDA18250_DeviceTypeMasterSingleWithLTWoXTout = 3,      /* TDA18250 Master Single Device Without Loop through, with external clock */
    tmddTDA18250_DeviceTypeMasterSingleWoLTWithXTout = 4,      /* TDA18250 Master Single Device Without Loop through, with external clock */
    tmddTDA18250_DeviceTypeMasterSingleWoLTWoXTout = 5,        /* TDA18250 Master Single Device Without Loop through, without external clock */
    tmddTDA18250_DeviceTypeSlave = 6,                          /* Slave Device */
    tmddTDA18250_DeviceTypeSlaveWithXTout = 7,                 /* Slave Device With external clock */
    tmddTDA18250_DeviceTypeSlaveWithLTWoXTout = 8,             /* Slave Device With Loop through, without external clock */
    tmddTDA18250_DeviceTypeMax = 9
} tmddTDA18250DeviceType_t, *ptmddTDA18250DeviceType_t;

/* TDA18250 Gud Up/Down selection */
typedef enum _tmddTDA18250_Gud_t
{
    tmddTDA18250_GUD_DOWN = 0,
    tmddTDA18250_GUD_UP = 1,
    tmddTDA18250_GUD_Max
} tmddTDA18250_Gud_t, *ptmddTDA18250_Gud_t;

/* TDA18250 Aud Up/Down selection */
typedef enum _tmddTDA18250_Aud_t
{
    tmddTDA18250_AUD_UP = 0,
    tmddTDA18250_AUD_DOWN = 1,
    tmddTDA18250_AUD_Max
} tmddTDA18250_Aud_t, *ptmddTDA18250_Aud_t;

/* TDA18250 State */
typedef enum _tmddTDA18250_State_t
{
    tmddTDA18250_OFF = 0,
    tmddTDA18250_ON = 1,
    tmddTDA18250_State_Max
} tmddTDA18250_State_t, *ptmddTDA18250_State_t;

/* Power Down State */
typedef enum _tmddTDA18250_PD_t
{
    tmddTDA18250_PD_NotActivated = 0,                               
    tmddTDA18250_PD_Activated = 1,              
    tmddTDA18250_PD_Max                 
} tmddTDA18250_PD_t, *ptmddTDA18250_PD_t;

typedef enum _tmddTDA18250HcMode_t
{
    tmddTDA18250_HcModeNormal = 0,
    tmddTDA18250_HcModeIRcalWanted,
    tmddTDA18250_HcModeIRcalImage,
    tmddTDA18250_HcModeRFcalPOR,
    tmddTDA18250_HcModeRFcalAV,
    tmddTDA18250_HcModeSoftreset,
    tmddTDA18250_HcModeRSSI,
    tmddTDA18250_HcMode_MAX
} tmddTDA18250HcMode_t, *ptmddTDA18250HcMode_t;

/* Time transition */
/* NOTES:       000 : no smooth transition      100 : 8   */
/*              001: 1                          101 : 16  */
/*              010: 2                          110 : 32  */
/*              011: 4                          111 : 200 */
typedef enum _tmddTDA18250_TimeCst_t
{
    tmddTDA18250_TCST_0 = 0,
    tmddTDA18250_TCST_1,
    tmddTDA18250_TCST_2,
    tmddTDA18250_TCST_4,
    tmddTDA18250_TCST_8,
    tmddTDA18250_TCST_16,
    tmddTDA18250_TCST_32,
    tmddTDA18250_TCST_200,
    tmddTDA18250_TCST_MAX
} tmddTDA18250_TimeCst_t, *ptmddTDA18250_TimeCst_t;


/* Switch State */
typedef enum _tmddTDA18250SwitchState_t
{
    tmddTDA18250_SW_OPEN  = 0,
    tmddTDA18250_SW_CLOSE = 1,
    tmddTDA18250_SW_State_Max
} tmddTDA18250SwitchState_t, *ptmddTDA18250SwitchState_t;

/* RFcal_modeOFF switch State */
typedef enum _tmddTDA18250_SW_RFcal_modeOFF_t
{
    tmddTDA18250_RFcal_modeOFF_CLOSE = 0,              
    tmddTDA18250_RFcal_modeOFF_OPEN,                               
    tmddTDA18250_RFcal_modeOFF_Max                 
} tmddTDA18250_SW_RFcal_modeOFF_t, *ptmddTDA18250_SW_RFcal_modeOFF_t;

/* AGC1 Gain State */
typedef enum _tmddTDA18250_AGC1_Gain_t
{
    tmddTDA18250_AGC1_Gain_Free = 0,              
    tmddTDA18250_AGC1_Gain_Fixed_Minus2dB = 2,              
    tmddTDA18250_AGC1_Gain_Fixed_2dB = 4,              
    tmddTDA18250_AGC1_Gain_Fixed_4_875dB,              
    tmddTDA18250_AGC1_Gain_Fixed_6_75dB,              
    tmddTDA18250_AGC1_Gain_Fixed_8_625dB,              
    tmddTDA18250_AGC1_Gain_Fixed_10_5dB,              
    tmddTDA18250_AGC1_Gain_Fixed_12_375dB,              
    tmddTDA18250_AGC1_Gain_Fixed_14_25dB,              
    tmddTDA18250_AGC1_Gain_Fixed_16_125dB,              
    tmddTDA18250_AGC1_Gain_Fixed_18dB,              
    tmddTDA18250_AGC1_Gain_Max                 
} tmddTDA18250_AGC1_Gain_t, *ptmddTDA18250_AGC1_Gain_t;
/* AGC1 Gain State */

typedef enum _tmddTDA18250_AGC_Mode_t
{
    tmddTDA18250_AGC_Mode_Free = 0,              
    tmddTDA18250_AGC_Mode_forced ,              
    tmddTDA18250_AGC_Mode_Max                 
} tmddTDA18250_AGC_Mode_t, *ptmddTDA18250_AGC_Mode_t;

typedef enum _tmddTDA18250IF_Level_t
{
    tmddTDA18250_IF_Level_2V   = 0,
    tmddTDA18250_IF_Level_1V   = 2,
    tmddTDA18250_IF_Level_0_5V = 7,
    tmddTDA18250_IF_Level_MAX,
} tmddTDA18250IF_Level_t, *ptmddTDA18250IF_Level_t;


typedef enum _tmddTDA18250HPF_Freq_t
{
    tmddTDA18250_HPF_Freq_400KHz = 0,
    tmddTDA18250_HPF_Freq_850KHz,
    tmddTDA18250_HPF_Freq_1MHz,
    tmddTDA18250_HPF_Freq_1_5MHz,
    tmddTDA18250_HPF_Freq_MAX,
} tmddTDA18250HPF_Freq_t, *ptmddTDA18250HPF_Freq_t;

typedef enum _tmddTDA18250ResetAGC_t
{
    tmddTDA18250_WithoutResetAGC = 0,
    tmddTDA18250_WithResetAGC = 1,
    tmddTDA18250_ResetAGC_Max = 4
} tmddTDA18250ResetAGC_t, *ptmddTDA18250ResetAGC_t;

typedef enum _tmddTDA18250LPF_Sign_t
{
    tmddTDA18250_LPF_Sign_Plus = 0,
    tmddTDA18250_LPF_Sign_Minus
} tmddTDA18250LPF_Sign_t, *ptmddTDA18250LPF_Sign_t;

typedef enum _tmddTDA18250LPF_Offset_t
{
    tmddTDA18250_LPF_Offset_0 = 0,
    tmddTDA18250_LPF_Offset_1,
    tmddTDA18250_LPF_Offset_2,
    tmddTDA18250_LPF_Offset_3
} tmddTDA18250LPF_Offset_t, *ptmddTDA18250LPF_Offset_t;

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

extern tmErrorCode_t
tmddTDA18250Init
(
    tmUnitSelect_t              tUnit,      /* I: Unit number      */
    tmbslFrontEndDependency_t*  psSrvFunc   /* I: setup parameters */
);

extern tmErrorCode_t 
tmddTDA18250DeInit
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
);

extern tmErrorCode_t
tmddTDA18250GetSWVersion
(
    ptmSWVersion_t  pSWVersion  /* I: Receives SW Version */
);

extern tmErrorCode_t
tmddTDA18250SetPowerState
(
    tmUnitSelect_t            tUnit,        /* I: Unit number                */
    tmddTDA18250PowerState_t  powerState    /* I: Power state of this device */
);

extern tmErrorCode_t
tmddTDA18250GetPowerState
(
    tmUnitSelect_t            tUnit,        /* I: Unit number                */
    ptmddTDA18250PowerState_t pPowerState   /* O: Power state of this device */
);

extern tmErrorCode_t
tmddTDA18250Write
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                  */
    UInt32          uIndex,     /* I: Start index to write         */
    UInt32          uNbBytes,   /* I: Number of bytes to write     */
    UInt8*          puBytes     /* I: Pointer on an array of bytes */
);

extern tmErrorCode_t
tmddTDA18250Read
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                  */
    UInt32          uIndex,     /* I: Start index to write         */
    UInt32          uNbBytes,   /* I: Number of bytes to read      */
    UInt8*          puBytes     /* I: Pointer on an array of bytes */
);

extern tmErrorCode_t
tmddTDA18250Reset
(
    tmUnitSelect_t          tUnit,               /* I: Unit number      */
    Bool                    clock_mode_auto,     /* I: clock mode       */
    tmddTDA18250ResetAGC_t  resetAGC             /* I: reset or not AGC */
);

extern tmErrorCode_t
tmddTDA18250ResetXtoutMTO
(
    tmUnitSelect_t           tUnit,      /* I: Unit number */
    tmddTDA18250DeviceType_t deviceType  /* I: device type */
);

extern tmErrorCode_t
tmddTDA18250WriteDataRAM
(
    tmUnitSelect_t tUnit,       /* I: Unit number */
    UInt8          uAddRAM,     /* I: Address RAM */
    UInt8          uDataRAM     /* I: Data RAM    */
);

extern tmErrorCode_t
tmddTDA18250SetLOFreq
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                       */
    UInt32          uLO         /* I: local oscillator frequency in Hz  */
);

extern tmErrorCode_t
tmddTDA18250GetLOFreq
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                       */
    UInt32          *puLO       /* O: local oscillator frequency in Hz  */
);

extern tmErrorCode_t
tmddTDA18250GetIdentity
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetDieTemperature
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetRSSI_Alarm
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetLO_Lock
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetRSSI
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetRSSI
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2b_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetAGC2b_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC1_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetAGC1_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetFlagMSM_OK
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetLaunchMSM
(
    tmUnitSelect_t  tUnit       /* I: Unit number */
);

extern tmErrorCode_t
tmddTDA18250SetResetFlags
(
    tmUnitSelect_t  tUnit       /* I: Unit number */
);

extern tmErrorCode_t
tmddTDA18250SetResetMSM
(
    tmUnitSelect_t  tUnit       /* I: Unit number */
);

extern tmErrorCode_t
tmddTDA18250SetDataRAM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetFreq_prog_Start
(
    tmUnitSelect_t  tUnit       /* I: Unit number */
);

extern tmErrorCode_t
tmddTDA18250SetCAL_Int
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetSM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetSM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetPD_RFAGC_Ifout
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250SetpdAGC1b
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250SetpdAGC2b
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250SetPD_RFAGC_Det
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250SetPDBUF1b
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250GetRFSW_MTO_LT_RFin
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250Sethc_mode
(
    tmUnitSelect_t       tUnit,      /* I: Unit number   */
    tmddTDA18250HcMode_t uHc_mode    /* I: Hc mode value */
);

extern tmErrorCode_t
tmddTDA18250SetBP_Filter
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetBP_Filter
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetIF_level
(
    tmUnitSelect_t          tUnit,      /* I: Unit number    */
    tmddTDA18250IF_Level_t  uLevel      /* I: IF Level value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC_Low_BW
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetpdAGC2a
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250SetRFAGC_OpenLoop
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2b_byte
(
    tmUnitSelect_t tUnit,      /* I: Unit number      */
    UInt8          uValue      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250GetAGC2b_byte
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);
    
extern tmErrorCode_t
tmddTDA18250SetpdAGC2_DETECT2
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250SetpdAGC2_DETECT3
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250SetpdAGC2_DETECT4
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC_On
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250Setpulse_up_auto
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250Getpulse_up_auto
(
    tmUnitSelect_t        tUnit,   /* I: Unit number    */
    tmddTDA18250_State_t* puState  /* O: State returned */
);

extern tmErrorCode_t
tmddTDA18250SetPSM_StoB
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetSTOB_DC
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetAGC1_au_ptr
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetAGC1_aud_sel
(
    tmUnitSelect_t     tUnit,   /* I: Unit number       */
    tmddTDA18250_Aud_t uAud     /* I: Up/Down selection */
);

extern tmErrorCode_t
tmddTDA18250SetAGC1_aud
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetAGC1_Gup_sel
(
    tmUnitSelect_t     tUnit,   /* I: Unit number       */
    tmddTDA18250_Gud_t uGud     /* I: Up/Down selection */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2_Gup_sel
(
    tmUnitSelect_t     tUnit,   /* I: Unit number       */
    tmddTDA18250_Gud_t uGud     /* I: Up/Down selection */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2_Gud_ptr_sel
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetAGC1_Gud
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetRFagc_fast
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetAGC1_Do_step
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetAGC1_bypass
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetAGC1_loop_off
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2_Gud_valid
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2_Gud
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetAGC1_aud_valid
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC1_Gud_valid
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2_bypass
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2_gain_Force
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250SetRFAGC_Top
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetRSSI_Byte
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetRSSI_Byte
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetIR_Freq
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetBUF1b_DCloopIH
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetSTOB_gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetSTOB_gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetRFcal_modeOFF
(
    tmUnitSelect_t                  tUnit,      /* I: Unit number */
    tmddTDA18250_SW_RFcal_modeOFF_t uSwitch     /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250SetRefineAV
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250SetSel_3dB
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250Setcapw_filter_sel
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250GetFilterIndex
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetLF_nHF
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetFreqCut
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetFreqCut
(
    tmUnitSelect_t  tUnit,      /* I: Unit number    */
    UInt32*         puValue     /* O: FreqCut in MHz */
);

extern tmErrorCode_t
tmddTDA18250SetFreqCut_sel
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetNmeasAV
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetNmeasAV
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetGAIN_threshold
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250Setcapw_gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250Setcapw_cad
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250Setcapw_ctank
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetWRI2CRAM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetModeI2CRAM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetSelRAM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetAddRAM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetDTA_NotMOCA
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetAGC1_smooth_t_cst
(
    tmUnitSelect_t          tUnit,      /* I: Unit number    */
    tmddTDA18250_TimeCst_t  uTimeCst    /* I: Time Cst value */
);

extern tmErrorCode_t
tmddTDA18250_SetRFSW_T_CST
(
    tmUnitSelect_t          tUnit,      /* I: Unit number    */
    tmddTDA18250_TimeCst_t  uTimeCst    /* I: Time Cst value */
);

extern tmErrorCode_t
tmddTDA18250SetLT_smooth_t_cst
(
    tmUnitSelect_t          tUnit,      /* I: Unit number    */
    tmddTDA18250_TimeCst_t  uTimeCst    /* I: Time Cst value */
);

extern tmErrorCode_t
tmddTDA18250SetLT_smooth_offset
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetHP_MTO_T_CST
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetHP_MTO_T_CST
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           *puValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetInd_Div2
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250Setshift_GD_offset
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250SetRFSW_ANALOG
(
    tmUnitSelect_t            tUnit,        /* I: Unit number */
    tmddTDA18250SwitchState_t uSwitchState  /* I: Switch State value */
);

extern tmErrorCode_t
tmddTDA18250GetRFSW_ANALOG
(
    tmUnitSelect_t             tUnit,         /* I: Unit number           */
    tmddTDA18250SwitchState_t* puSwitchState  /* O: Switch State returned */
);

extern tmErrorCode_t
tmddTDA18250SetRFSW_ANALOG_MOVE
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250SetRFSW_LP
(
    tmUnitSelect_t            tUnit,        /* I: Unit number        */
    tmddTDA18250SwitchState_t uSwitchState  /* I: Switch State value */
);

extern tmErrorCode_t
tmddTDA18250GetRFSW_LP
(
    tmUnitSelect_t             tUnit,         /* I: Unit number           */
    tmddTDA18250SwitchState_t* puSwitchState  /* O: Switch State returned */
);

extern tmErrorCode_t
tmddTDA18250SetRFSW_LP_MOVE
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250SetDTA_bypass
(
    tmUnitSelect_t            tUnit,        /* I: Unit number        */
    tmddTDA18250SwitchState_t uSwitchState  /* I: Switch State value */
);

extern tmErrorCode_t
tmddTDA18250SetBP_Ind1
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetCap_HP_Left
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetCap_HP_Right
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetAgc1bis_gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250Setpd_AGC1bis
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250Setnotch_select
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetLF_freq
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetShift_LF_rccal
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetPD_hpf
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
);

extern tmErrorCode_t
tmddTDA18250Sethpf_freq
(
    tmUnitSelect_t         tUnit,  /* I: Unit number     */
    tmddTDA18250HPF_Freq_t uValue  /* I: HPF_Freq value  */
);

extern tmErrorCode_t
tmddTDA18250SetRFSW_HP
(
    tmUnitSelect_t            tUnit,        /* I: Unit number        */
    tmddTDA18250SwitchState_t uSwitchState  /* I: Switch State value */
);

extern tmErrorCode_t
tmddTDA18250GetRFSW_HP
(
    tmUnitSelect_t             tUnit,         /* I: Unit number           */
    tmddTDA18250SwitchState_t* puSwitchState  /* O: Switch State returned */
);

extern tmErrorCode_t
tmddTDA18250SetRFSW_HP_MOVE
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);

extern tmErrorCode_t
tmddTDA18250SetCap_HP_Mid
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetCap_L10n
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetCap_L6n
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetBP_Ind2
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250_Setrfcal_clock_en
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250_Getrfcal_clock_en
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250_SetAGC1_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    tmddTDA18250_AGC1_Gain_t           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250_GetAGC1_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    tmddTDA18250_AGC1_Gain_t*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250_SetAGC1_gain_Force
(

    tmUnitSelect_t  tUnit,      /* I: Unit number */
    tmddTDA18250_AGC_Mode_t           uValue      /* I: item value  */
);
extern tmErrorCode_t
tmddTDA18250_GetAGC1_gain_Force
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    tmddTDA18250_AGC_Mode_t*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetMainPLL
(
    tmUnitSelect_t tUnit,         /* I: Unit number                      */
    UInt32         uLO,           /* I: local oscillator frequency in Hz */
    UInt8*         puPreScaler,   /* O: PreScaler                        */
    UInt8*         puPostDiv      /* O: PostDiv                          */
);

extern tmErrorCode_t
tmddTDA18250SetMainPLL
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                       */
    UInt32          uLO         /* I: local oscillator frequency in Hz  */
);


extern tmErrorCode_t
tmddTDA18250SetFreq_prog_En
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetFreq_prog_En
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetLP_Fc
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetLP_Fc
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetHP_MTO2_MOVE
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);


extern tmErrorCode_t
tmddTDA18250SetHP_MTO3_MOVE
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
);


extern tmErrorCode_t
tmddTDA18250SetHP_MTO2
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetHP_MTO2
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetHP_MTO3
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetHP_MTO3
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetHP_MTO2_MOVE
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetHP_MTO3_MOVE
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250Getagc1_smooth_transition_on
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetAGC1_loop_off
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2_loop_off
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetAGC2_loop_off
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2_Speed
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetAGC2_Speed
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2_Do_step
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetAGC2_Do_step
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2_Up_step
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetAGC2_Up_step
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetAGC2_Speed_Up
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetAGC2_Speed_Up
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetRC_GD_sign
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetRC_GD_offset
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetRC_ReadCal_3_to_0
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetRC_ReadCal_4
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetXtOut
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetXtOut
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetpdLT
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetpdLT
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetpdAGC1b 
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetpdAGC2b
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetManual_LT
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetManual_LT
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);


extern tmErrorCode_t
tmddTDA18250SetLoopThrough_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetLoopThrough_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250Setshift
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250Getshift
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250Gethpf_freq
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetPD_hpf
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250GetIF_level
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);


extern tmErrorCode_t
tmddTDA18250GetRSSI_Sense_OK
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetPSM_Buf1b
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250SetPD_LO_Synthe
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

tmErrorCode_t
tmddTDA18250SetpdDETECT1
(
    tmUnitSelect_t    tUnit,    /* I: Unit number      */
    tmddTDA18250_PD_t uPdDetect /* I: Power Down detect value */
);

extern tmErrorCode_t
tmddTDA18250SetRC_LPF_sign
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetRC_LPF_sign
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
tmddTDA18250SetRC_LPF_offset
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
);

extern tmErrorCode_t
tmddTDA18250GetRC_LPF_offset
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
);
#ifdef __cplusplus
}
#endif

#endif /* _TMDD_TDA18250_H */
