/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18204_Local.h
 *
 *                %version: 1 %
 *
 * \date          %modify_time%
 *
 * \author        Bruno LEMESLE
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18204_Driver_User_Guide.pdf
 *
 * \section info Change Information
 *
 * \par Changelog
 *  -1.0.44 Skip I2C calls in tmbslTDA18204_GetPowerLevel when NB path is off
 *  -1.0.43 PR 534 : Uncor in WB when NB is switched on/off
 *  -1.0.42 fix prototype error on tmbslTDA18204_GetDriverSize() parameter Size
 *  -1.0.41 new APIs to save/restore driver in memory
 *  -1.0.40 Add new error code TDA18204_ERR_UNLOCK
 *  -1.0.39 Update all documentation for delivery of TDA18256 + TDA18204 to customers
 *  -1.0.38 Remove warning reported by some embedded compilers :
 *     In all "while" instructions containing exp1 and exp2, 
 *     order of exp1 and exp2 is very important in order to avoid out of bounds read (overrun)
 *  -1.0.37 Change way to calculate AGC2 NB contribution in RSSI, now using 2 polynoms
 *  -1.0.36 Add OOB standard (BW=2MHz) and allow High-Pass filter enabling
 *  -1.0.35 Set AGC2 NB TOP Down/Up to 100/99 if RF > 793 MHz
 *  -1.0.34 Set AGC2 NB TOP Down/Up to 100/99 if RF > 865 MHz
 *  -1.0.33 Set bAgc2Active flag in pObj for multiple instance purpose potential bug
 *     Change implementation of AGC2 NB TOP programmation with a state machine + addition of a medium level of AGC2 NB TOP
 *     PR#549 Set AGC3_NB_Do_step = 2.044ms, AGC3_NB_UP_STEP= 66ms, AGCK_mode=8.192ms
 *  -1.0.32 Set AGC2_NB_UP_STEP to 66 ms instead of 262 ms
 *  -1.0.31 Set SWITCH_STAB_CAP to 380 ms instead of 880 ms for Filter 2
 *  -1.0.30 Set PSM_Mixer to 0 mA when PSM=1
 *     Set AGC2 NB TOP Down/Up to 100/99 if RF < 123 MHz instead of 103/102
 *  -1.0.29 Set S2D_Gain to 6dB when RF > 123 MHz else set to 9dB
 *     Set AGC2 NB TOP Down/Up to 100/99 if 244 MHz < RF < 600 MHz else set AGC2 NB TOP Down/Up to 103/102
 *  -1.0.28 Set AGC2_NB_Up_step to 262 ms
 *  -1.0.27 TDA18204 : Set uNbSmoothTransition to 80ms
 *  -1.0.26 Update GetAgc2WBCtrl and SetAgc2WBCtrl to improve external AGC2 support in GUI
 *  -1.0.25 Set S2D_Gain to 6dB at any frequency
 *  -1.0.24 Code refactoring for AGC2 WB internal/external loop
 *  -1.0.23 Force IF_HPF_Enable to 0 in tmbslTDA18204_SetStandardMode()
 *     Set PSM_RFPPF from -9.3 mA to -4.2 mA when PSM=1
 *     Set PSM_Lodriver from -6 mA to -3 mA when PSM=1
 *     Set SWITCH_STAB_CAP from 1210 to 880
 *     When RF > 244 MHz don't differentiate filters handling between ES1 and ES1
 *     Dont differentiate AGC2_TOP_DO and AGC2_TOP_UP for Filter6
 *     Set S2D_Gain to 6dB when RF > 320 MHz
 *     PR#504 : Loop off all AGCs before launch RSSI NB
 *     Set AGC2_NB_UP_STEP to 66 ms instead of 262 ms
 *  -1.0.22 Improve lock time by adjusting wait time in function iTDA18204_SetLO()
 *  -1.0.21 Make AGC 3 visible in normal mode
 *     Set AGC3 value to 11dB
 *     Set agc2WBTop configurable in function of RF
 *  -1.0.20 Add configuration of AGC2 depending on RF frequency
 *  -1.0.20 keep AGC3 switch off but set its value to 11dB
 *  -1.0.19 Improve power level calculation by changing equation order of AGC2 NB coefficients for ES2
 *  -1.0.18 Remove AGCK_Time_Constant as configurable parameter, in order not to be reset when doing a tune
 *  -1.0.17 Improve tmbslTDA18204_SetStandardMode in case of user want to change values of AGC3_NB_DO_step, AGC3_NB_UP_step, AGCK_mode, AGCK_step and keep it after tune
 *  -1.0.16 PR#338 : Chande decoding of XTout + when XTout is set to value different of 0, Xtout_Amp can only be set to 200 mV
 *  -1.0.15 Remove set TEQ_Launch = 0 at the end of function iTDA18204_SetTiltEqualizer()
 *     PR#488 :TDA18204_ES2 LTO NF, internal oscillator create spur
 *  -1.0.14 Correction of wrong WB power level if RF> 918MHz and TEQ=15
 *  -1.0.13 Update coefficients for RSSI WB ES1 equation
 *  -1.0.12 Improve error handling during HwInit
 *  -1.0.11 Improve error handling in HwInit
 *  -1.0.11 Set AGC1_TOP and AGC1_TOP_STRATEGY at init, befause previously was set in SetStandard function 
 *  -1.0.10 Regression when implemented PR#466 : restoration of AGC2 loop off done at wrong moment
 *     RSSI, take into account that coefficients are different between ES1 and ES2
 *     RSSI, update ES2 coefficients for WB and NB paths
 *  -1.0.9 PR#466 : AGC2 loop off get devalidated when the frequency change around 600 MHz
 *  -1.0.8 PR#460 : TOP AGC2_NB versus CSO/CTB, take into account order of top writing
 *     PR#466 : AGC2 loop off get devalidated when the frequency changes
 *     Force LNA_Zin to 1 in any mode
 *  -1.0.7 Correction of init failed after entering in calibration mode
 *  -1.0.6 PR#458 : Software modification for ES2
 *     PR#459 : FVCO max drift versus temperature
 *     PR#460 : TOP AGC2_NB versus CSO/CTB
 *     Patch ES2 : Retrieve Dig_Clock in order to restore it after iTDA18204_SetMSM()
 *  -1.0.5 Reset bAgc2Active flag (useful when resetting alimention in automation)
 *     Set SWITCH_STAB_CAP to 0 instead of 140 for filter 5 for ES2
 *     Set Dcboost to 0 for ES2
 *  -1.0.4 PR#456 : rework W_Filter decoding
 *  -1.0.3 PR#453 : Uncor on WB when switching NB AGC2 high-pass
 *  -1.0.2 setRF without Boost_VCO
 *  -1.0.1 Rework AGC2_WB contribution in RSSI
 *  -1.0.0 Set AGC2_NB_Gain_Control_En to O at POR
 *  -0.0.40 Error in Power Level for WB (AGC2 coefficients)
 *  -0.0.39 Remove S2D_Gain value 3 (9dB)
 *     Rework RSSI and Power Level for WB and NB path (AGC2 contribution)
 *  -0.0.38 Implement RSSI and Power Level for NB path (back to previous AGC3 contribution)
 *  -0.0.37 tmbslTDA18204 : Implement RSSI and Power Level for WB path (AGC2 contribution)
 *  -0.0.36 Implement RSSI and Power Level for NB path (AGC3 contribution)
 *  -0.0.35 Rssi NB, remove iTDA18204_LvlRFFilterContribution() and add iTDA18204_GetRefMaxGainNB()
 *  -0.0.34 Implement RSSI and Power Level for NB path
 *     Set VCO Switch configurable
 *     PR#442 Software modification arround H3H5 programmability
 *  -0.0.33 Implement RSSI and Power Level for NB path
 *  -0.0.32 Set AGC2_NB_HP_EN = 0 if RF < 600 MHz
 *  -0.0.31 IR calibration for low frequency only
 *     ES2, wait for LOCalc_End occurs in FVCO max calibration
 *     PR#438 : Enable the RF filter below 44MHzES2
 *     PR#437 : Implement high pass filter of AGC2
 *  -0.0.30 Re-implement iTDA18204_SelectVCO() for improve C/N
 *     IR calibration for low frequency only
 *     ES2, wait for LOCalc_End occurs in FVCO max calibration
 *  -0.0.29 If tuner not locked, wait and re-read LO_Lock bit
 *  -0.0.28 Re-implement iTDA18204_SelectVCO() for improve C/N
 *  -0.0.27 Improve LO programmation
 *  -0.0.26 Improve LO programmation
 *  -0.0.25 Set VCO Max margin to 100 MHz
 *  -0.0.24 Make VCO Max margin configurable
 *     If Xtal_div_1p5 set ICP_select to 100
 *  -0.0.23 Modification to handle ES1 MF version
 *  -0.0.22 Force LO_Frac_0 to 0 and mask LO_Frac_1 with 0xF0
 *  -0.0.21 Make AGC2 WB gain change speed configurable
 *     Implementation of NB RSSI
 *  -0.0.20 Rework AGC1 contribution in RSSI
 *  -0.0.19 Rework switch on/off AGC2 NB sequence (PR422)
 *  -0.0.18 Add a state machine in iTDA18204_SetLLPowerState to handle power transition correctly
 *     modification of iTDA18204_SelectVCO for ES2
 *  -0.0.17 Remove BOOST_VCO in function of VCO used
 *  -0.0.16 Set BOOST_VCO and ICP_select in function of VCO used
 *     Return error in set power function if Hw Init has never been done
 *  -0.0.15 In set RF, minimum value should be 42 instead of 50 MHz
 *     Uncors improvement in ES1, in VCO selection condition
 *     
 *  -0.0.14 Change AGC1 contribution equation in RSSI calculation
 *     In mode PowerBufMtoLtoStandby set XTAL to NO_XTAL_NO_BUFFER_IN
 *     In mode PowerBufMtoLtoStandby set pd_xtout_asym to 1
 *     In mode PowerBufMtoLtoStandby set pd_md_counter to 1
 *     In mode PowerBufMtoLtoStandby set rf_mux_acg2_wb_smooth_move to 1
 *     In mode PowerBufMtoLtoStandby set IF_HP_Enable to 0
 *     Set AGC1 smooth time constant to 8;
 *     Set AGC2_NB_TOP_DO to 96 dBuVms and AGC2_NB_TOP_UP to 94 dBuVms
 *     In PowerNbNormalMode, set pd_xtout_asym to 1
 *  -0.0.14 Change AGC1 contribution equation in RSSI calculation
 *     In mode PowerBufMtoLtoStandby set XTAL to NO_XTAL_NO_BUFFER_IN
 *     In mode PowerBufMtoLtoStandby set pd_xtout_asym to 1
 *     In mode PowerBufMtoLtoStandby set pd_md_counter to 1
 *     In mode PowerBufMtoLtoStandby set rf_mux_acg2_wb_smooth_move to 1
 *     In mode PowerBufMtoLtoStandby set IF_HP_Enable to 0
 *     Set AGC1 smooth time constant to 8;
 *     Set AGC2_NB_TOP_DO to 96 dBuVms and AGC2_NB_TOP_UP to 94 dBuVms
 *     In PowerNbNormalMode, set pd_xtout_asym to 1
 *  -0.0.13 Remove PowerSavingMode in standard configuration
 *     When setting Power State mode, force DIG clock to INTERNAL
 *     Change TEQ contribution equation in RSSI calculation
 *     In mode PowerBufMtoLtoStandby set RFsw_LT_RFin
 *     In PSM mode, force  PSM_RFPPF to -9.3 mA
 *     Error in LT_Gain_Read values decoding
 *     
 *  -0.0.13 Remove PowerSavingMode in standard configuration
 *     When setting Power State mode, force DIG clock to INTERNAL
 *     Change TEQ contribution equation in RSSI calculation
 *     In mode PowerBufMtoLtoStandby set RFsw_LT_RFin
 *     In PSM mode, force  PSM_RFPPF to -9.3 mA
 *     Error in LT_Gain_Read values decoding
 *     
 *  -0.0.12 At init force digital clock to xtal_in mode
 *  -0.0.12 At init force digital clock to xtal_in mode
 *  -0.0.11 At begining of set RF, set digital in Xtal IN mode
 *     At end of set RF, set digital in mode SubLo
 *     In set RF, remove BP Filter 6
 *     Force LO_Frac_0 and  LO_Frac_1 to 0xFF
 *     Force ICP_low to 0
 *  -0.0.11 At begining of set RF, set digital in Xtal IN mode
 *     At end of set RF, set digital in mode SubLo
 *     In set RF, remove BP Filter 6
 *     Force LO_Frac_0 and  LO_Frac_1 to 0xFF
 *     Force ICP_low to 0
 *  -0.0.10 At begining of set RF, set digital in Xtal IN mode
 *     At end of set RF, set digital in mode SubLo
 *     In set RF, remove BP Filter 6
 *     Force LO_Frac_0 and  LO_Frac_1 to 0xFF
 *  -0.0.10 At begining of set RF, set digital in Xtal IN mode
 *     At end of set RF, set digital in mode SubLo
 *     In set RF, remove BP Filter 6
 *     Force LO_Frac_0 and  LO_Frac_1 to 0xFF
 *  -0.0.9 Rework driver for NB Normal Mode (new ES MF)
 *     Remove TDA18204_PowerNbWbMode (new ES MF)
 *  -0.0.9 Rework driver for NB Normal Mode (new ES MF)
 *     Remove TDA18204_PowerNbWbMode (new ES MF)
 *  -0.0.8 By default in all mode select ICP_LOW + set ICP_select to 100 mA
 *     RSSI : rework equations for AGC1 and TEQ contribution
 *  -0.0.8 By default in all mode select ICP_LOW + set ICP_select to 100 mA
 *     RSSI : rework equations for AGC1 and TEQ contribution
 *  -0.0.8 Correction of VCO High/Low switching
 *     By default set DET3_LowBW to 0
 *  -0.0.8 Correction of VCO High/Low switching
 *     By default set DET3_LowBW to 0
 *  -0.0.7 Select VCO calculation : Transition should occur at FVCOmax*49/60 and not at FVCOmax*47/60
 *     rework settings of SCI normal mode
 *     rework SW to be compatible with TDA18204 ES1 MF and ES2
 *  -0.0.7 Select VCO calculation : Transition should occur at FVCOmax*49/60 and not at FVCOmax*47/60
 *     rework settings of SCI normal mode
 *     rework SW to be compatible with TDA18204 ES1 MF and ES2
 *  -0.0.6 Set PD_MTO_BUF1 to 1 in any mode exept TDA18204_PowerBufMtoLtoStandby
 *     Remove warnings
 *  -0.0.6 Set PD_MTO_BUF1 to 1 in any mode exept TDA18204_PowerBufMtoLtoStandby
 *     Remove warnings
 *  -0.0.5 In AGC2 WB internal control mode set AGC2_LOOP_OFF to 0 in NB mode
 *     Force ICP_low to 0
 *     when setting RF tuner always remains in TDA18204_PowerSciNbNormalMode
 *     move all accessed registers from tmbslTDA18204_RegDef_Expert.h to tmbslTDA18204_RegDef.h
 *  -0.0.4 Add TDA18204_PowerNbWbMode (NB Tuner using AGC2 WB)
 *  -0.0.3 Set LPF and HPF in function of standard
 *     Disable vco_IR_high after IR calibration
 *     Disable calgen_pph_sel after IR calibration
 *     In NbNormalMode set PD_DETECT2 to 1
 *  -0.0.2 IR cal: Enable High Pass + select cutoff 1M+ IR_Step_Delta 5 + IR_Mixer_Atten -21
 *  -0.0.1 First delivery
 *  -0.0.10 minor changes
 *  -0.0.10 Add component under XML
 *  -0.0.9 Internal RF calculation in MHz
 *  -0.0.8 First integration in Full Spectrum Receiver system solution
 *
 * \par Version_Definition
 *  VERSION_TAG:TDA18204_COMP_NUM.TDA18204_MAJOR_VER.TDA18204_MINOR_VER
 *
*/

#pragma once
#ifndef _TMBSL_TDA18204_LOCAL_H
#define _TMBSL_TDA18204_LOCAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/
/* Maximum number of systems supported by driver */
#define TDA18204_MAX_UNITS                      4

/* Driver version definition */
#define TDA18204_COMP_NUM  1 /* Major protocol change - Specification change required */
#define TDA18204_MAJOR_VER 0 /* Minor protocol change - Specification change required */
#define TDA18204_MINOR_VER 44 /* Software update - No protocol change - No specification change required */

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
#define TDA18204_MUTEX_TIMEOUT                  (5000)
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

#define TDA18204_RCCAL_DONE_MASK 0x20

/* TDA18204 Driver State Machine */
typedef enum _TDA18204HwState_t {
    TDA18204_HwState_Unknown = 0,   /* Hw State Unknown */
    TDA18204_HwState_InitNotDone,   /* Hw Init Not Done */
    TDA18204_HwState_InitPending,   /* Hw Init Pending */
    TDA18204_HwState_InitDone,      /* Hw Init Done */
    TDA18204_HwState_SetModeDone,   /* Set Mode Done */
	TDA18204_HwState_SetStdDone,    /* Set Standard Done */
    TDA18204_HwState_SetRFDone,     /* Set RF Done */
    TDA18204_HwState_Max
} TDA18204HwState_t, *pTDA18204HwState_t;

typedef enum _TDA18204HwStateCaller_t {
    TDA18204_HwStateCaller_Unknown = 0, /* Caller Unknown */
    TDA18204_HwStateCaller_SetPower,    /* Caller SetPowerState */
    TDA18204_HwStateCaller_HwInit,      /* Caller HwInit */
    TDA18204_HwStateCaller_SetStd,      /* Caller SetStandardMode */
    TDA18204_HwStateCaller_SetRF,       /* Caller SetRF */
    TDA18204_HwStateCaller_GetRSSI,     /* Caller GetRSSI */
    TDA18204_HwStateCaller_GetVCOmax,   /* Caller GetVCOmax */
    TDA18204_HwStateCaller_GetIF,       /* Caller GetIF */
    TDA18204_HwStateCaller_GetLO,       /* Caller GetLO */
    TDA18204_HwStateCaller_GetVCO,      /* Caller GetVCO */
	TDA18204_HwStateCaller_Max
} TDA18204HwStateCaller_t, *pTDA18204HwStateCaller_t;

/* TDA18204 specific IRQ clear: */
typedef enum _TDA18204IRQ_t {
    TDA18204_IRQ_XtalCal                      = 0x01, /* XtalCal bit */
    TDA18204_IRQ_MSM_RCCal                    = 0x02, /* MSM_RCCal bit */
    TDA18204_IRQ_MSM_IRCAL                    = 0x04, /* MSM_IRCAL bit */
    TDA18204_IRQ_MSM_RSSI                     = 0x08, /* MSM_RSSI bit */
    TDA18204_IRQ_MSM_LOCalc                   = 0x10, /* MSM_LOCalc bit */
    TDA18204_IRQ_MSM_error_config             = 0x20, /* MSM_error_config bit */
    TDA18204_IRQ_MSM_error_no_clock_available = 0x40, /* MSM_error_no_clock_available bit */
	TDA18204_IRQ_Global                       = 0x80, /* IRQ_status bit */
    TDA18204_IRQ_HwInit         =   TDA18204_IRQ_MSM_RCCal\
                                    |TDA18204_IRQ_MSM_IRCAL,
    TDA18204_IRQ_GetPowerLevel  = TDA18204_IRQ_MSM_RSSI
} TDA18204IRQ_t, *pTDA18204IRQ_t;

/* TDA18204 specific MSM: */
typedef enum _TDA18204MSM_t {
    TDA18204_MSM_FVCO_max_calc  = 0x01, /* FVCO_max_calc bit */
    TDA18204_MSM_RC_Cal         = 0x02, /* RC_Cal bit */ 
    TDA18204_MSM_IR_CAL_Wanted  = 0x10, /* IR_CAL_Wanted bit */
    TDA18204_MSM_IR_Cal_Image   = 0x20, /* IR_Cal_Image bit */
    TDA18204_MSM_RSSI_Meas      = 0x80, /* RSSI_Meas bit */
    /* Performs all CALs */
    TDA18204_MSM_HwInit         = TDA18204_MSM_RC_Cal \
								  |TDA18204_MSM_IR_CAL_Wanted \
                                  |TDA18204_MSM_IR_Cal_Image,

    /* Performs all IR_CAL */
    TDA18204_MSM_IrCal          = TDA18204_MSM_IR_Cal_Image \
								  |TDA18204_MSM_IR_CAL_Wanted,
    TDA18204_MSM_GetPowerLevel  = TDA18204_MSM_RSSI_Meas
} TDA18204MSM_t, *pTDA18204MSM_t;


/* TDA18204 specific AGC Top: */
/* AGC1 */
typedef enum _TDA18204AGC1_Gud_t
{
    TDA18204_AGC1_Gud_Up = 0,
    TDA18204_AGC1_Gud_Down = 1,
    TDA18204_AGC1_Gud_Max = 2
} TDA18204AGC1_Gud_t, *pTDA18204AGC1_Gud_t;

typedef enum _TDA18204AGC1_Aup_t
{
    TDA18204_AGC1_Aup_0dB = 0,
    TDA18204_AGC1_Aup_3dB = 1,
    TDA18204_AGC1_Aup_4_875dB = 2,
    TDA18204_AGC1_Aup_6_75dB = 3,
    TDA18204_AGC1_Aup_8_625dB = 4,
    TDA18204_AGC1_Aup_10_5dB = 5,
    TDA18204_AGC1_Aup_12_375dB = 6,
    TDA18204_AGC1_Aup_14_25dB = 7,
    TDA18204_AGC1_Aup_16_125dB = 8,
    TDA18204_AGC1_Aup_Max = 9
} TDA18204AGC1_Aup_t, *pTDA18204AGC1_Aup_t;

typedef enum _TDA18204AGC1_Ado_t
{
    TDA18204_AGC1_Ado_3dB = 0,
    TDA18204_AGC1_Ado_4_875dB = 1,
    TDA18204_AGC1_Ado_6_75dB = 2,
    TDA18204_AGC1_Ado_8_625dB = 3,
    TDA18204_AGC1_Ado_10_5dB = 4,
    TDA18204_AGC1_Ado_12_375dB = 5,
    TDA18204_AGC1_Ado_14_25dB = 6,
    TDA18204_AGC1_Ado_16_125dB = 7,
    TDA18204_AGC1_Ado_18dB = 8,
    TDA18204_AGC1_Ado_Max = 9
} TDA18204AGC1_Ado_t, *pTDA18204AGC1_Ado_t;

typedef enum _TDA18204AGC1_LoopOff_t {
    TDA18204_AGC1_LoopOff_Off = 0, /* Off */
    TDA18204_AGC1_LoopOff_On = 1, /* On */
    TDA18204_AGC1_LoopOff_Max
} TDA18204AGC1_LoopOff_t, *pTDA18204AGC1_LoopOff_t;

/* AGC2 */
typedef enum _TDA18204AGC2_Gud_t
{
    TDA18204_AGC2_Gud_Up = 0,
    TDA18204_AGC2_Gud_Down = 1,
    TDA18204_AGC2_Gud_Max = 2
} TDA18204AGC2_Gud_t, *pTDA18204AGC2_Gud_t;

typedef enum _TDA18204AGC2WB_Ctrl_t {
    TDA18204_AGC2_WB_Ctrl_Internal = 0, /* AGC2 strategy is Internal */
    TDA18204_AGC2_WB_Ctrl_External = 1, /* AGC2 strategy is External */
    TDA18204_AGC2_WB_Ctrl_Max
} TDA18204AGC2WB_Ctrl_t, *pTDA18204AGC2WB_Ctrl_t;

typedef enum _TDA18204_Coeff_X_t
{
    TDA18204_Coeff_X0 = 0,
    TDA18204_Coeff_X1 = 1,
    TDA18204_Coeff_X2 = 2,
    TDA18204_Coeff_X3 = 3,
	TDA18204_Coeff_X_Code = 4,
    TDA18204_Coeff_X_Max = 5
} TDA18204_Coeff_X_t, *pTDA18204_Coeff_X_t;

typedef enum _TDA18204_Coeff_Y_t
{
    TDA18204_Coeff_Y0 = 0,
    TDA18204_Coeff_Y1 = 1,
    TDA18204_Coeff_Y2 = 2,
    TDA18204_Coeff_Y_Code = 3,
    TDA18204_Coeff_Y_Max = 4
} TDA18204_Coeff_Y_t, *pTDA18204_Coeff_Y_t;

typedef enum _TDA18204_Coeff_Z_t
{
    TDA18204_Coeff_Z0 = 0,
    TDA18204_Coeff_Z1 = 1,
    TDA18204_Coeff_Z2 = 2,
    TDA18204_Coeff_Z3 = 3,
    TDA18204_Coeff_Z4 = 4,
    TDA18204_Coeff_Z5 = 5,
    TDA18204_Coeff_Z6 = 6,
    TDA18204_Coeff_Z_Code = 7,
    TDA18204_Coeff_Z_Max = 8
} TDA18204_Coeff_Z_t, *pTDA18204_Coeff_Z_t;

typedef enum _TDA18204PowerSavingMode_t {
    TDA18204PowerSavingMode_normal = 0,   /* Set of normal consumption */
    TDA18204PowerSavingMode_Low,          /* Set of low level comsuption   */
    TDA18204PowerSavingMode_Max
} TDA18204PowerSavingMode_t, *pTDA18204PowerSavingMode_t;

typedef enum _TDA18204BoostH3H5_t {
    TDA18204BoostH3H5_Psm0 = 0,     /* 21.3 mA (ES1)  -  29 mA   (ES2) */
    TDA18204BoostH3H5_Psm1,         /* 19.4 mA (ES1)  -  36.3 mA (ES2) */
    TDA18204BoostH3H5_Psm2,         /* 17.4 mA (ES1)  -  39.3 mA (ES2) */
    TDA18204BoostH3H5_Psm3,         /* 15.4 mA (ES1)  -  48.3 mA (ES2) */
    TDA18204BoostH3H5_Max
} TDA18204BoostH3H5_t, *pTDA18204BoostH3H5_t;

typedef enum _TDA18204_AGC2NBTOP_t {
    TDA18204_AGC2NBTOP_LevelLow,    /* minimum values */
    TDA18204_AGC2NBTOP_LevelMedium,
    TDA18204_AGC2NBTOP_LevelHigh,    /* maximum values */
    TDA18204_AGC2NBTOP_LevelInvalid
} TDA18204_AGC2NBTOP_t, *pTDA18204_AGC2NBTOP_t;

typedef enum _TDA18204IF_LP_Offset_t {
    TDA18204_LPFOffset_0pc = 0,				/* LPFc 0%   */
    TDA18204_LPFOffset_plus_4pc =  0x1,		/* LPFc 4%   */
    TDA18204_LPFOffset_plus_8pc =  0x2,		/* LPFc 8%   */
    TDA18204_LPFOffset_plus_12pc = 0x3,		/* LPFc 12%  */
    TDA18204_LPFOffset_0pcbis =    0x10,    /* LPFc 0%   */
    TDA18204_LPFOffset_min_4pc =   0x11,    /* LPFc -4%  */
    TDA18204_LPFOffset_min_8pc =   0x12,    /* LPFc -8%  */
    TDA18204_LPFOffset_min_12pc =  0x13 ,   /* LPFc -12% */
    TDA18204_LPFOffset_Max
} TDA18204IF_LP_Offset_t, *pTDA18204IF_LP_Offset_t;

typedef enum TDA18204DC_Notch_IF_PPF_t {
    TDA18204_DC_Notch_IF_PPF_Disabled = 0,  /* IF Notch Disabled */
    TDA18204_DC_Notch_IF_PPF_Enabled,       /* IF Notch Enabled */
    TDA18204_DC_Notch_IF_PPF_Max
} TDA18204DC_Notch_IF_PPF_t, *pTDA18204DC_Notch_IF_PPF_t;

typedef enum _TDA18204IF_HP_Offset_t {
    TDA18204_HPFOffset_0pc = 0,				/* HPFc 0%   */
    TDA18204_HPFOffset_plus_4pc =  0x1,		/* HPFc 4%   */
    TDA18204_HPFOffset_plus_8pc =  0x2,		/* HPFc 8%   */
    TDA18204_HPFOffset_plus_12pc = 0x3, 	/* HPFc 12%  */
    TDA18204_HPFOffset_0pcbis =    0x10,	/* HPFc 0%   */
    TDA18204_HPFOffset_min_4pc =   0x11,	/* HPFc -4%  */
    TDA18204_HPFOffset_min_8pc =   0x12,	/* HPFc -8%  */
    TDA18204_HPFOffset_min_12pc =  0x13,	/* HPFc -12% */
    TDA18204_HPFOffset_Max
} TDA18204IF_HP_Offset_t, *pTDA18204IF_HP_Offset_t;

typedef enum _TDA18204IF_HP_Enable_t {
    TDA18204_HPFEnable_Disabled = 0,        /* HPF Disabled */
    TDA18204_HPFEnable_Enabled  = 1,        /* HPF Enabled  */
    TDA18204_HPFEnable_Max
} TDA18204IF_HP_Enable_t, *pTDA18204IF_HP_Enable_t;

typedef enum _TDA18204AGC_DET_SPEED_t {
    TDA18204_AGC_DET_SPEED_62_5KHz = 0,		/* 62.5KHz */
    TDA18204_AGC_DET_SPEED_125KHz,			/* 125KHz */
    TDA18204_AGC_DET_SPEED_Max
} TDA18204AGC_DET_SPEED_t, *pTDA18204AGC_DET_SPEED_t;

typedef enum _TDA18204LNA_ZIN_t {
    TDA18204_LNA_ZIN_NF = 0,      /* better NF for 7,10,13dB agc1 gain */
    TDA18204_LNA_ZIN_S11,           /* better S11 for 7,10,13dB agc1 gain  */
    TDA18204_LNA_ZIN_Max
} TDA18204LNA_ZIN_t, *pTDA18204LNA_ZIN_t;

typedef enum _TDA18204RSSI_CAP_VAL_t {
    TDA18204_RSSI_CAP_VAL_0_5pF = 0, /* 0.5pF */
    TDA18204_RSSI_CAP_VAL_3pF,      /* 3pF */
    TDA18204_RSSI_CAP_VAL_Max
} TDA18204RSSI_CAP_VAL_t, *pTDA18204RSSI_CAP_VAL_t;

typedef enum _TDA18204RSSI_CK_SPEED_t {
    TDA18204_RSSI_CK_SPEED_31_25kHz = 0, /* 31.25KHz */
    TDA18204_RSSI_CK_SPEED_62_5kHz,      /* 62.5KHz  */
    TDA18204_RSSI_CK_SPEED_Max
} TDA18204RSSI_CK_SPEED_t, *pTTDA18204RSSI_CK_SPEED_t;

typedef enum _TDA18204BandIndex_t
{
    TDA18204_IND_DIG_BAND_1 = 0,
    TDA18204_IND_DIG_BAND_2 = 1,
    TDA18204_IND_DIG_BAND_3 = 2,
    TDA18204_IND_DIG_BAND_4 = 3,
    TDA18204_IND_DIG_BAND_5 = 4,
    TDA18204_IND_DIG_BAND_6 = 5,
    TDA18204_IND_DIG_BAND_7 = 6,
    TDA18204_IND_BAND_Max = 7
} TDA18204BandIndex_t, *pTDA18204BandIndex_t;

typedef struct _TDA18204StdCoefficients
{
	/****************************************************************/
	/* IF Settings                                                  */
	/****************************************************************/
	UInt32									uIF;					/* IF Frequency */

	/****************************************************************/
	/* IF SELECTIVITY Settings                                      */
	/****************************************************************/
	TDA18204_LP_Fc_t						LPF;					/* LPF Cut off */
	TDA18204IF_LP_Offset_t					LPF_Offset;				/* LPF offset */
	TDA18204DC_Notch_IF_PPF_t				DC_Notch_IF_PPF;		/* DC notch IF PPF */
	TDA18204_IF_HP_Fc_t						HPF;					/* Hi Pass */
	TDA18204IF_HP_Offset_t					HPF_Offset;				/* HPF offset */
    TDA18204IF_HP_Enable_t                  HPF_Enable;             /* HPF enable */

	/****************************************************************/
	/* NB AGC TOP Settings                                          */
	/****************************************************************/
	UInt8									AGC2_TOP_DN_HIGH;		/* AGC2 NB TOP DN High */
	UInt8									AGC2_TOP_UP_HIGH;		/* AGC2 NB TOP UP High */
	UInt8									AGC2_TOP_DN_MEDIUM;		/* AGC2 NB TOP DN Medium */
	UInt8									AGC2_TOP_UP_MEDIUM;		/* AGC2 NB TOP UP Medium */
    UInt8									AGC2_TOP_DN_LOW;        /* AGC2 NB TOP DN Low */
	UInt8									AGC2_TOP_UP_LOW;        /* AGC2 NB TOP UP Low */
	TDA18204AGC_DET_SPEED_t					AGC2_DET_SPEED;			/* AGC2 NB DET SPEED */
	TDA18204_DET12_Cint_t					DET12_CINT;				/* DET12 CINT */
	UInt8									AGC3_TOP_DN;			/* AGC3 NB TOP DN */
	UInt8									AGC3_TOP_UP;			/* AGC3 NB TOP UP */
	TDA18204_IF_level_t						IF_Output_Level;        /* IF Output Level */
	TDA18204_S2D_Gain_t						S2D_Gain;				/* S2D gain */

	/****************************************************************/
	/* GSK Settings                                                 */
	/****************************************************************/
	TDA18204_RSSI_HP_Fc_t					RSSI_HP_FC;				/* RSSI HP FC */

	/****************************************************************/
	/* RSSI Settings                                                */
	/****************************************************************/
	TDA18204RSSI_CAP_VAL_t					RSSI_Cap_Val;			/* RSSI Cap Val */
	TDA18204RSSI_CK_SPEED_t					RSSI_Ck_Speed;			/* RSSI Ck Speed */
} TDA18204StdCoefficients, *pTDA18204StdCoefficients;

typedef struct _TDA18204Agc2WBTop
{
    UInt32  uRF; /* RF Frequency in Hz*/
    UInt8   AGC2_Gud_Up;
    UInt8   AGC2_Gud_Down;
} TDA18204Agc2WBTop, *pTDA18204Agc2WBTop;

typedef struct _TDA18204Object_t
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
	tmPowerState_t                  curPowerState;
	TDA18204PowerState_t            curLLPowerState;
	TDA18204PowerState_t            mapLLPowerState[tmPowerMax];
	TDA18204PowerSavingMode_t       PowerSavingMode;
	UInt32							PowerDownSmoothDelay;
    UInt16							Identity;
    UInt16							Revision;
	UInt32                          uRF;
	UInt32                          VCOmax;
	UInt8                           Temperature;
    TDA18204StandardMode_t          StandardMode;
	pTDA18204StdCoefficients        pStandard;
	/* TEQ settings */
	TDA18204TEQSelect_t             Tilt;
	UInt32							TiltDelay;
	/* XTal settings */
	TDA18204_XTAL_IN_CTRL_t         XTin;
	TDA18204_XTout_t				XTOut;
	TDA18204_Xtout_Amp_t            XTOutAmp;

	TDA18204HwState_t               eHwState;
    /* NB : Settings dependant of standard */
    TDA18204StdCoefficients         Std_Array[TDA18204_StandardMode_Max-1];
	/* WB + NB : AGC1 */
	TDA18204_AGC1_Top_t				agc1TopDownUp;
	TDA18204_AGC1_Top_Strategy_t	agc1TopStrategy;
	TDA18204AGC_DET_SPEED_t			agc1DetSpeed;
    TDA18204_AGC1_smooth_t_cst_t    agc1TimeCst;
    TDA18204_LT_smooth_t_cst_t      ltTimeCst;
	/* WB : AGC2 */
	UInt8                           agc2WBGud[TDA18204_AGC2_Gud_Max];
    TDA18204Agc2WBTop               agc2WBTop[TDA18204_INSTANCE_CUSTOM_AGC2WB_TOP_NB];
    TDA18204AGC2WB_Ctrl_t           agc2WBCtrl;
	TDA18204_AGC2_UP_STEP_t			agc2UpStep;
	TDA18204_AGC2_DOWN_STEP_t		agc2DownStep;
    Bool                            bAgc2Active;
    TDA18204_AGC2NBTOP_t            eAGC2NBTopCurrent;
#ifdef _TDA18204_REGMAP_BITFIELD_DEFINED
    TDA18204_Reg_Map_t              RegMap;
#else
    UInt8                           RegMap[TDA18204_REG_MAP_NB_BYTES];
#endif
    UInt32                          Marker;
} TDA18204Object_t, *pTDA18204Object_t, **ppTDA18204Object_t;


/*============================================================================*/
/* Internal Prototypes:                                                       */
/*============================================================================*/
extern tmErrorCode_t
iTDA18204_CheckHwState(pTDA18204Object_t pObj, TDA18204HwStateCaller_t caller);

static tmErrorCode_t iTDA18204_rf_mux_agc2_wb_smooth_switch               (pTDA18204Object_t pObj, UInt8 bValue);
static tmErrorCode_t iTDA18204_PowerTransition_Init                       (pTDA18204Object_t pObj);
static tmErrorCode_t iTDA18204_PowerTransition_PowerFullStandbyWithGPIO   (pTDA18204Object_t pObj);
static tmErrorCode_t iTDA18204_PowerTransition_PowerBufMtoLtoStandby      (pTDA18204Object_t pObj);
static tmErrorCode_t iTDA18204_PowerTransition_PowerCalibrationNormalMode (pTDA18204Object_t pObj);
static tmErrorCode_t iTDA18204_PowerTransition_Standby                    (pTDA18204Object_t pObj);
static tmErrorCode_t iTDA18204_PowerTransition_WbOn                       (pTDA18204Object_t pObj);
static tmErrorCode_t iTDA18204_PowerTransition_WbOff                      (pTDA18204Object_t pObj);
static tmErrorCode_t iTDA18204_PowerTransition_NbOn                       (pTDA18204Object_t pObj);
static tmErrorCode_t iTDA18204_PowerTransition_NbOff                      (pTDA18204Object_t pObj);
static tmErrorCode_t iTDA18204_PowerTransition_CommonOn                   (pTDA18204Object_t pObj);

extern tmErrorCode_t
iTDA18204_SetLLPowerState(pTDA18204Object_t pObj, TDA18204PowerState_t powerState);

extern tmErrorCode_t
iTDA18204_FVCOMaxCalibration(pTDA18204Object_t   pObj);

extern tmErrorCode_t
iTDA18204_XtalCalibration(pTDA18204Object_t   pObj);

extern tmErrorCode_t
iTDA18204_RCCalibration(pTDA18204Object_t   pObj);

extern tmErrorCode_t
iTDA18204_IRCalibration(pTDA18204Object_t   pObj);

extern tmErrorCode_t 
iTDA18204_SetRFFilters(pTDA18204Object_t pObj);

extern tmErrorCode_t
iTDA18204_SetLO(pTDA18204Object_t pObj);

extern tmErrorCode_t
iTDA18204_CalculateNLO(pTDA18204Object_t pObj, UInt32 LO, UInt8* NLO);

extern tmErrorCode_t
iTDA18204_FindXtalDiv1p5(UInt32 VCO, Bool* Xtal_div_1p5);

extern tmErrorCode_t
iTDA18204_SelectVCO(pTDA18204Object_t pObj, UInt32 VCO ,Bool* VCOHighUsed);

extern tmErrorCode_t
iTDA18204_CalculateNIntKInt(UInt32 VCO, Bool Xtal_div_1p5, UInt8* NInt, UInt32*	KInt);

extern tmErrorCode_t
iTDA18204_SetSubLO(pTDA18204Object_t pObj, UInt32 LO);

extern tmErrorCode_t 
iTDA18204_PowerSavingMode( pTDA18204Object_t   pObj, TDA18204PowerSavingMode_t PowerSavingMode);

extern tmErrorCode_t
iTDA18204_SetMSM(pTDA18204Object_t pObj, UInt8 uValue, Bool bLaunch);

extern tmErrorCode_t
iTDA18204_WaitIRQ(pTDA18204Object_t pObj, UInt32 timeOut, UInt32 waitStep, UInt8 irqStatus);

extern tmErrorCode_t
iTDA18204_SetRF(pTDA18204Object_t pObj);

extern tmErrorCode_t
iTDA18204_ResetAgc2WB(pTDA18204Object_t pObj);

extern tmErrorCode_t
iTDA18204_StartRCCalibration(pTDA18204Object_t pObj);

extern tmErrorCode_t
iTDA18204_SetAgc2WB(pTDA18204Object_t pObj);

extern tmErrorCode_t
iTDA18204_SetAgc2NB(pTDA18204Object_t pObj, Bool Agc2NBEnable);

extern tmErrorCode_t
iTDA18204_SoftReset(pTDA18204Object_t pObj);

extern tmErrorCode_t
iTDA18204_GetRSSI(pTDA18204Object_t pObj, UInt8* puValue);

extern tmErrorCode_t
iTDA18204_GetPowerLevel(pTDA18204Object_t pObj, UInt8 uRSSIValue, Int32* pPowerLevel);

extern tmErrorCode_t
iTDA18204_CalculateGain(pTDA18204Object_t pObj,UInt32 uRF,Int32* pGain);

extern tmErrorCode_t
iTDA18204_LvlAgc1Contribution(pTDA18204Object_t   pObj, UInt32 uRF, Int32* pDeltaGain);

extern tmErrorCode_t
iTDA18204_LvlAgc2Contribution(pTDA18204Object_t   pObj, UInt32 uRF, Int32* pDeltaGain);

extern tmErrorCode_t
iTDA18204_GetRefMaxGainNB(pTDA18204Object_t   pObj, Int32* pMaxGain);

extern tmErrorCode_t
iTDA18204_LvlAgc1NBContribution(pTDA18204Object_t   pObj, Int32* pDeltaGain);

extern tmErrorCode_t
iTDA18204_LvlAgc2NBContribution(pTDA18204Object_t   pObj, Int32* pDeltaGain);

extern tmErrorCode_t
iTDA18204_LvlAgc3NBContribution(pTDA18204Object_t   pObj, Int32* pDeltaGain);

extern tmErrorCode_t
iTDA18204_LvlTeqContribution(pTDA18204Object_t   pObj, UInt32 uRF, Int32* pGain);

extern tmErrorCode_t
iTDA18204_SetTiltEqualizer(pTDA18204Object_t pObj, TDA18204TEQSelect_t teqSelect);

extern tmErrorCode_t
iTDA18204_SetBoostH3H5(pTDA18204Object_t pObj, TDA18204BoostH3H5_t boostH3H5);

extern tmErrorCode_t
iTDA18204_SetAGC2NBTOP( pTDA18204Object_t pObj );

extern tmErrorCode_t
iTDA18204_Write(pTDA18204Object_t pObj, const TDA18204_BitField_t* pBitField, UInt8 uData, tmbslFrontEndBusAccess_t eBusAccess);

extern tmErrorCode_t
iTDA18204_Read(pTDA18204Object_t pObj, const TDA18204_BitField_t* pBitField, UInt8* puData, tmbslFrontEndBusAccess_t eBusAccess);

extern tmErrorCode_t
iTDA18204_WriteRegMap(pTDA18204Object_t pObj, UInt8 uAddress, UInt32 uWriteLen);

extern tmErrorCode_t
iTDA18204_ReadRegMap(pTDA18204Object_t pObj, UInt8 uAddress, UInt32 uReadLen);

extern tmErrorCode_t
iTDA18204_Wait(pTDA18204Object_t pObj, UInt32 Time);

extern tmErrorCode_t
iTDA18204_AllocInstance(tmUnitSelect_t tUnit, ppTDA18204Object_t ppDrvObject);

extern tmErrorCode_t
iTDA18204_DeAllocInstance(pTDA18204Object_t pDrvObject);

extern tmErrorCode_t
iTDA18204_GetInstance(tmUnitSelect_t tUnit, ppTDA18204Object_t ppDrvObject);

extern tmErrorCode_t
iTDA18204_ResetInstance(pTDA18204Object_t pDrvObject);

#ifdef _TVFE_IMPLEMENT_MUTEX
 extern tmErrorCode_t iTDA18204_MutexAcquire(pTDA18204Object_t pObj, UInt32 timeOut);
 extern tmErrorCode_t iTDA18204_MutexRelease(pTDA18204Object_t pObj);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA18204_LOCAL_H */

