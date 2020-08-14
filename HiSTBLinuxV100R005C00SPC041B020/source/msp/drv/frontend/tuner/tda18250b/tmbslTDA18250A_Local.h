/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18250A_Local.h
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

#ifndef _TMBSL_TDA18250A_LOCAL_H
#define _TMBSL_TDA18250A_LOCAL_H

#ifdef __cplusplus
extern "C"
{
#endif


/*============================================================================*/
/* Standard Types and defines:                                                */
/*============================================================================*/

/* Maximum number of systems supported by driver */
#define TDA18250A_MAX_UNITS                      3

/* Driver version definition */
#define TDA18250A_COMP_NUM                       0  /* Major protocol change - Specification change required */
#define TDA18250A_MAJOR_VER                      1  /* Minor protocol change - Specification change required */
#define TDA18250A_MINOR_VER                      19 /* Software update - No protocol change - No specification change required */

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
#define TDA18250A_MUTEX_TIMEOUT                  (5000)
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

typedef enum _TDA18250A_power_state_mode_Item_t {
    TDA18250A_power_state_mode_NORMAL_MODE_VALUE_0 = 0,		/*  Normal mode  */
    TDA18250A_power_state_mode_STDBY_PLUS16M_VALUE_1 = 1,		/*  Stdby +16M  */
    TDA18250A_power_state_mode_STDBY_PLUS16M_PLUS_LT_VALUE_2 = 2,		/*  Stdby +16M + LT  */
    TDA18250A_power_state_mode_STDBY_PLUS16M_PLUS_PLL_VALUE_3 = 3,		/*  Stdby +16M + PLL  */
    TDA18250A_power_state_mode_STDBY_PLUS_16M_PLUS_LT_PLUS_PLL_VALUE_4 = 4		/*  Stdby + 16M + LT + PLL  */
} TDA18250A_power_state_mode_t, *pTDA18250A_power_state_mode_t;

/*============================================================================*/
/* Registers definitions:                                                     */
/*============================================================================*/

#define TDA18250A_REG_ADD_SZ                             (0x01)
#define TDA18250A_REG_DATA_MAX_SZ                        (0x01)
#define TDA18250A_REG_MAP_NB_BYTES                       (0x5D)

#define TDA18250A_REG_DATA_LEN(_FIRST_REG, _LAST_REG)    ( (_LAST_REG.Address - _FIRST_REG.Address) + 1)


/* TDA18250A Register ID_byte_1 0x00 */
static const TDA18250A_BitField_t gTDA18250A_Reg_ID_byte_1 = { 0x00, 0x00, 0x08, 0x00 };
/* Master_Not_Slave bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_ID_byte_1__Master_Not_Slave = { 0x00, 0x07, 0x01, 0x00 };
/* Ident_1 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_ID_byte_1__Ident_1 = { 0x00, 0x00, 0x07, 0x00 };


/* TDA18250A Register ID_byte_2 0x01 */
static const TDA18250A_BitField_t gTDA18250A_Reg_ID_byte_2 = { 0x01, 0x00, 0x08, 0x00 };
/* Ident_2 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_ID_byte_2__Ident_2 = { 0x01, 0x00, 0x08, 0x00 };


/* TDA18250A Register ID_byte_3 0x02 */
static const TDA18250A_BitField_t gTDA18250A_Reg_ID_byte_3 = { 0x02, 0x00, 0x08, 0x00 };
/* Major_rev bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_ID_byte_3__Major_rev = { 0x02, 0x04, 0x04, 0x00 };
/* Minor_rev bit(s): frq04920: Metalefixableewitheivfix remettreeàe00 */
static const TDA18250A_BitField_t gTDA18250A_Reg_ID_byte_3__Minor_rev = { 0x02, 0x00, 0x04, 0x00 };


/* TDA18250A Register Thermo_byte_1 0x03 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Thermo_byte_1 = { 0x03, 0x00, 0x08, 0x00 };
/* TM_D bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Thermo_byte_1__TM_D = { 0x03, 0x00, 0x07, 0x00 };


/* TDA18250A Register Thermo_byte_2 0x04 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Thermo_byte_2 = { 0x04, 0x00, 0x08, 0x00 };
/* TM_ON bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Thermo_byte_2__TM_ON = { 0x04, 0x00, 0x01, 0x00 };


/* TDA18250A Register Power_state_byte_1 0x05 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_state_byte_1 = { 0x05, 0x00, 0x08, 0x00 };
/* POR bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_state_byte_1__POR = { 0x05, 0x07, 0x01, 0x00 };
/* AGC_Lock bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_state_byte_1__AGC_Lock = { 0x05, 0x01, 0x01, 0x00 };
/* LO_Lock bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_state_byte_1__LO_Lock = { 0x05, 0x00, 0x01, 0x00 };


/* TDA18250A Register Power_state_byte_2 0x06 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_state_byte_2 = { 0x06, 0x00, 0x08, 0x00 };
/* Power_State_Mode bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_state_byte_2__Power_State_Mode = { 0x06, 0x00, 0x03, 0x00 };


/* TDA18250A Register Gpio_ctrl_byte 0x07 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Gpio_ctrl_byte = { 0x07, 0x00, 0x08, 0x00 };
/* GPIO2 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Gpio_ctrl_byte__GPIO2 = { 0x07, 0x01, 0x01, 0x00 };
/* GPIO1 bit(s): frq05249: wheneGPIOe=e1e=>eabsorbetheecurrente(theneifepulleupetoeVcceoneapplicatione=>e0V) */
static const TDA18250A_BitField_t gTDA18250A_Reg_Gpio_ctrl_byte__GPIO1 = { 0x07, 0x00, 0x01, 0x00 };


/* TDA18250A Register IRQ_status 0x08 */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_status = { 0x08, 0x00, 0x08, 0x00 };
/* IRQ_status bit(s): Interruptegénérée: -eàelaefinedesecalibrations -eàelaefinedue'newechanel'  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_status__IRQ_status = { 0x08, 0x07, 0x01, 0x00 };
/* XtalCal_End bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_status__XtalCal_End = { 0x08, 0x05, 0x01, 0x00 };
/* MSM_RSSI_End bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_status__MSM_RSSI_End = { 0x08, 0x04, 0x01, 0x00 };
/* MSM_LOCalc_End bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_status__MSM_LOCalc_End = { 0x08, 0x03, 0x01, 0x00 };
/* MSM_IRCAL_End bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_status__MSM_IRCAL_End = { 0x08, 0x01, 0x01, 0x00 };
/* MSM_RCCal_End bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_status__MSM_RCCal_End = { 0x08, 0x00, 0x01, 0x00 };


/* TDA18250A Register IRQ_enable 0x09 */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_enable = { 0x09, 0x00, 0x08, 0x00 };
/* IRQ_Enable bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_enable__IRQ_Enable = { 0x09, 0x07, 0x01, 0x00 };
/* XtalCal_Enable bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_enable__XtalCal_Enable = { 0x09, 0x05, 0x01, 0x00 };
/* MSM_RSSI_Enable bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_enable__MSM_RSSI_Enable = { 0x09, 0x04, 0x01, 0x00 };
/* MSM_LOCalc_Enable bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_enable__MSM_LOCalc_Enable = { 0x09, 0x03, 0x01, 0x00 };
/* MSM_IRCAL_Enable bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_enable__MSM_IRCAL_Enable = { 0x09, 0x01, 0x01, 0x00 };
/* MSM_RCCal_Enable bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_enable__MSM_RCCal_Enable = { 0x09, 0x00, 0x01, 0x00 };


/* TDA18250A Register IRQ_clear 0x0A */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_clear = { 0x0A, 0x00, 0x08, 0x00 };
/* IRQ_Clear bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_clear__IRQ_Clear = { 0x0A, 0x07, 0x01, 0x00 };
/* XtalCal_Clear bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_clear__XtalCal_Clear = { 0x0A, 0x05, 0x01, 0x00 };
/* MSM_RSSI_Clear bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_clear__MSM_RSSI_Clear = { 0x0A, 0x04, 0x01, 0x00 };
/* MSM_LOCalc_Clear bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_clear__MSM_LOCalc_Clear = { 0x0A, 0x03, 0x01, 0x00 };
/* MSM_IRCAL_Clear bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_clear__MSM_IRCAL_Clear = { 0x0A, 0x01, 0x01, 0x00 };
/* MSM_RCCal_Clear bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_clear__MSM_RCCal_Clear = { 0x0A, 0x00, 0x01, 0x00 };


/* TDA18250A Register IRQ_set 0x0B */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_set = { 0x0B, 0x00, 0x08, 0x00 };
/* IRQ_Set bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_set__IRQ_Set = { 0x0B, 0x07, 0x01, 0x00 };
/* XtalCal_Set bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_set__XtalCal_Set = { 0x0B, 0x05, 0x01, 0x00 };
/* MSM_RSSI_Set bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_set__MSM_RSSI_Set = { 0x0B, 0x04, 0x01, 0x00 };
/* MSM_LOCalc_Set bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_set__MSM_LOCalc_Set = { 0x0B, 0x03, 0x01, 0x00 };
/* MSM_IRCAL_Set bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_set__MSM_IRCAL_Set = { 0x0B, 0x01, 0x01, 0x00 };
/* MSM_RCCal_Set bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_set__MSM_RCCal_Set = { 0x0B, 0x00, 0x01, 0x00 };


/* TDA18250A Register AGC1_byte_1 0x0C */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_1 = { 0x0C, 0x00, 0x08, 0x00 };
/* PD_LNA bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_1__PD_LNA = { 0x0C, 0x07, 0x01, 0x00 };
/* AGC1_Top_Strategy bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_1__AGC1_Top_Strategy = { 0x0C, 0x04, 0x03, 0x00 };
/* AGC1_Det_Speed bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_1__AGC1_Det_Speed = { 0x0C, 0x03, 0x01, 0x00 };
/* AGC1_Top bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_1__AGC1_Top = { 0x0C, 0x00, 0x03, 0x00 };


/* TDA18250A Register AGC1_byte_2 0x0D */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_2 = { 0x0D, 0x00, 0x08, 0x00 };
/* AGC1_Up_step bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_2__AGC1_Up_step = { 0x0D, 0x06, 0x02, 0x00 };
/* Fast_Mode_AGC bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_2__Fast_Mode_AGC = { 0x0D, 0x05, 0x01, 0x00 };
/* LNA_Zin bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_2__LNA_Zin = { 0x0D, 0x04, 0x01, 0x00 };
/* AGC1_Do_Step_asym_En bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_2__AGC1_Do_Step_asym_En = { 0x0D, 0x03, 0x01, 0x00 };
/* AGC1_Up_Step_asym_En bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_2__AGC1_Up_Step_asym_En = { 0x0D, 0x02, 0x01, 0x00 };
/* LNA_Range bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_2__LNA_Range = { 0x0D, 0x00, 0x02, 0x00 };


/* TDA18250A Register AGC1_byte_3 0x0E */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_3 = { 0x0E, 0x00, 0x08, 0x00 };
/* AGC1_loop_off bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_3__AGC1_loop_off = { 0x0E, 0x07, 0x01, 0x00 };
/* AGC1_Do_step bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_3__AGC1_Do_step = { 0x0E, 0x05, 0x02, 0x00 };
/* Force_AGC1_gain bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain = { 0x0E, 0x04, 0x01, 0x00 };
/* AGC1_Gain bit(s): Gains-12,-9,-6,-3,0,3,6,9,12,15dB */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain = { 0x0E, 0x00, 0x04, 0x00 };


/* TDA18250A Register AGC1_byte_4 0x0F */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_4 = { 0x0F, 0x00, 0x08, 0x00 };
/* AGC1_rst_do_enable bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_4__AGC1_rst_do_enable = { 0x0F, 0x07, 0x01, 0x00 };
/* AGC1_rst_up_enable bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_4__AGC1_rst_up_enable = { 0x0F, 0x06, 0x01, 0x00 };
/* AGC1_smooth_t_cst_fast bit(s): frq05249: Inefastemode,efixetheetimeeconstanteforetheeLteswitchecontrol: '000':enoesmoothetransition,e'001':e1,e'010':e2,e'011':e4,e'100':e8,e'101':e16,e'110':e32,e'111':enoteused */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_4__AGC1_smooth_t_cst_fast = { 0x0F, 0x03, 0x03, 0x00 };
/* AGC1_smooth_t_cst bit(s): frq05249: Inenormalemode,efixetheetimeeconstanteforetheeAGC1eswitchecontrol: '000':enoesmoothetransition,e'001':e1,e'010':e2,e'011':e4,e'100':e8,e'101':e16,e'110':e32,e'111':enoteused */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_byte_4__AGC1_smooth_t_cst = { 0x0F, 0x00, 0x03, 0x00 };


/* TDA18250A Register LT_byte_1 0x10 */
static const TDA18250A_BitField_t gTDA18250A_Reg_LT_byte_1 = { 0x10, 0x00, 0x08, 0x00 };
/* PD_LT bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_LT_byte_1__PD_LT = { 0x10, 0x07, 0x01, 0x00 };
/* Force_LT_gain bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_LT_byte_1__Force_LT_gain = { 0x10, 0x04, 0x01, 0x00 };
/* LT_Gain bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_LT_byte_1__LT_Gain = { 0x10, 0x00, 0x03, 0x00 };


/* TDA18250A Register LT_byte_2 0x11 */
static const TDA18250A_BitField_t gTDA18250A_Reg_LT_byte_2 = { 0x11, 0x00, 0x08, 0x00 };
/* RFsw_LT_RFin bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_LT_byte_2__RFsw_LT_RFin = { 0x11, 0x06, 0x01, 0x00 };
/* LT_smooth_t_cst_fast bit(s): frq05249: Inefasteemode,efixetheetimeeconstanteforetheeLteswitchecontrol: '000':enoesmoothetransition,e'001':e1,e'010':e2,e'011':e4,e'100':e8,e'101':e16,e'110':e32,e'111':enoteused */
static const TDA18250A_BitField_t gTDA18250A_Reg_LT_byte_2__LT_smooth_t_cst_fast = { 0x11, 0x03, 0x03, 0x00 };
/* LT_smooth_t_cst bit(s): frq05249: Inenormalemode,efixetheetimeeconstanteforetheeLteswitchecontrol: '000':enoesmoothetransition,e'001':e1,e'010':e2,e'011':e4,e'100':e8,e'101':e16,e'110':e32,e'111':enoteused */
static const TDA18250A_BitField_t gTDA18250A_Reg_LT_byte_2__LT_smooth_t_cst = { 0x11, 0x00, 0x03, 0x00 };


/* TDA18250A Register AGC2_byte_1 0x12 */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_1 = { 0x12, 0x00, 0x08, 0x00 };
/* AGC2_top_range_adapt bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_1__AGC2_top_range_adapt = { 0x12, 0x07, 0x01, 0x00 };
/* AGC2_cfi_shift bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_1__AGC2_cfi_shift = { 0x12, 0x05, 0x02, 0x00 };
/* Force_AGC2_gain bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_1__Force_AGC2_gain = { 0x12, 0x04, 0x01, 0x00 };
/* AGC2_Up_step bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_1__AGC2_Up_step = { 0x12, 0x02, 0x02, 0x00 };
/* AGC2_Do_step bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_1__AGC2_Do_step = { 0x12, 0x00, 0x02, 0x00 };


/* TDA18250A Register AGC2_byte_2 0x13 */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_2 = { 0x13, 0x00, 0x08, 0x00 };
/* AGC2_loop_off bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_2__AGC2_loop_off = { 0x13, 0x07, 0x01, 0x00 };
/* AGC2_Do_Step_asym_En bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_2__AGC2_Do_Step_asym_En = { 0x13, 0x06, 0x01, 0x00 };
/* AGC2_Up_Step_asym_En bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_2__AGC2_Up_Step_asym_En = { 0x13, 0x05, 0x01, 0x00 };
/* AGC2_TOP_DO bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_2__AGC2_TOP_DO = { 0x13, 0x00, 0x05, 0x00 };


/* TDA18250A Register AGC2_byte_3 0x14 */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_3 = { 0x14, 0x00, 0x08, 0x00 };
/* AGC2_Det_Speed bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_3__AGC2_Det_Speed = { 0x14, 0x07, 0x01, 0x00 };
/* AGC2_Gain_Control_En bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_3__AGC2_Gain_Control_En = { 0x14, 0x06, 0x01, 0x00 };
/* AGC2_Gain_Control_Speed bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_3__AGC2_Gain_Control_Speed = { 0x14, 0x05, 0x01, 0x00 };
/* AGC2_TOP_UP bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_3__AGC2_TOP_UP = { 0x14, 0x00, 0x05, 0x00 };


/* TDA18250A Register AGC2_byte_4 0x15 */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_4 = { 0x15, 0x00, 0x08, 0x00 };
/* AGC2_Gain bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain = { 0x15, 0x00, 0x08, 0x00 };


/* TDA18250A Register AGC2_byte_5 0x16 */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_5 = { 0x16, 0x00, 0x08, 0x00 };
/* AGC2_adapt_top23_delta bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_5__AGC2_adapt_top23_delta = { 0x16, 0x04, 0x04, 0x00 };
/* AGC2_adapt_top23_enable bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_5__AGC2_adapt_top23_enable = { 0x16, 0x03, 0x01, 0x00 };
/* AGC2_HP_EN_BP bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_5__AGC2_HP_EN_BP = { 0x16, 0x01, 0x01, 0x00 };
/* AGC2_HP_EN bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_byte_5__AGC2_HP_EN = { 0x16, 0x00, 0x01, 0x00 };


/* TDA18250A Register AGC3_byte_1 0x17 */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_1 = { 0x17, 0x00, 0x08, 0x00 };
/* AGC3_TOP_DO bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_1__AGC3_TOP_DO = { 0x17, 0x04, 0x04, 0x00 };
/* AGC3_TOP_UP bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_1__AGC3_TOP_UP = { 0x17, 0x00, 0x04, 0x00 };


/* TDA18250A Register AGC3_byte_2 0x18 */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_2 = { 0x18, 0x00, 0x08, 0x00 };
/* AGC3_loop_off bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_2__AGC3_loop_off = { 0x18, 0x07, 0x01, 0x00 };
/* IF_Det_Caprst_En bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_2__IF_Det_Caprst_En = { 0x18, 0x06, 0x01, 0x00 };
/* AGC3_Do_Step_asym_En bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_2__AGC3_Do_Step_asym_En = { 0x18, 0x05, 0x01, 0x00 };
/* AGC3_Up_Step_asym_En bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_2__AGC3_Up_Step_asym_En = { 0x18, 0x04, 0x01, 0x00 };
/* Force_AGC3_gain bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_2__Force_AGC3_gain = { 0x18, 0x03, 0x01, 0x00 };
/* AGC3_Gain bit(s): GainsedependeoneLPFeFreqeandeCalemodee(seeeTable):  6MHzeeee:e-6,e-3,e0,e3,e6,e9,e12,e15edB 7MHzeeee:e??? 8MHzeeee:e??? 9MHzeeee:e??? 1.5MHze:e???  RFCALee:e2dBe(LPF=1.5MHz) IRCALee:e30dBe(LPF=1.5MHz) */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_2__AGC3_Gain = { 0x18, 0x00, 0x03, 0x00 };


/* TDA18250A Register AGC3_byte_3 0x19 */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_3 = { 0x19, 0x00, 0x08, 0x00 };
/* DET3_FastAtt bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_3__DET3_FastAtt = { 0x19, 0x07, 0x01, 0x00 };
/* DET3_LowBW bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_3__DET3_LowBW = { 0x19, 0x06, 0x01, 0x00 };
/* RSSI_HP_Fc bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_3__RSSI_HP_Fc = { 0x19, 0x04, 0x02, 0x00 };
/* AGC3_Do_step bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_3__AGC3_Do_step = { 0x19, 0x02, 0x02, 0x00 };
/* AGC3_Up_step bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_byte_3__AGC3_Up_step = { 0x19, 0x00, 0x02, 0x00 };


/* TDA18250A Register AGCK_byte_1 0x1A */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGCK_byte_1 = { 0x1A, 0x00, 0x08, 0x00 };
/* AGC1_top_reduce bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGCK_byte_1__AGC1_top_reduce = { 0x1A, 0x06, 0x01, 0x00 };
/* DET12_Cint bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGCK_byte_1__DET12_Cint = { 0x1A, 0x04, 0x02, 0x00 };
/* AGCK_Step bit(s): 00e:e1e<=>e0.2dB 01e:e2e<=>e0.4dB 10e:e3e<=>e0.6dB 11e:e4e<=>e0.8dB */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGCK_byte_1__AGCK_Step = { 0x1A, 0x02, 0x02, 0x00 };
/* AGCK_Mode bit(s): 00e:eVsynce/8 01e:eVsync 10e:e16MHze/e2^15e(Digital) 11e:e16MHze/e2^17e(Digital) */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGCK_byte_1__AGCK_Mode = { 0x1A, 0x00, 0x02, 0x00 };


/* TDA18250A Register AGC1_LT_Gain_status 0x1B */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_LT_Gain_status = { 0x1B, 0x00, 0x08, 0x00 };
/* AGC1_Smooth_Transition_ON bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_LT_Gain_status__AGC1_Smooth_Transition_ON = { 0x1B, 0x07, 0x01, 0x00 };
/* AGC1_gain_read bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_LT_Gain_status__AGC1_gain_read = { 0x1B, 0x03, 0x04, 0x00 };
/* LT_Gain_Read bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC1_LT_Gain_status__LT_Gain_Read = { 0x1B, 0x00, 0x03, 0x00 };


/* TDA18250A Register AGC2_Gain_status 0x1C */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_Gain_status = { 0x1C, 0x00, 0x08, 0x00 };
/* AGC2_gain_read bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_Gain_status__AGC2_gain_read = { 0x1C, 0x00, 0x08, 0x00 };


/* TDA18250A Register AGC3_gain_status 0x1D */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_gain_status = { 0x1D, 0x00, 0x08, 0x00 };
/* AGC1_FREEZE bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_gain_status__AGC1_FREEZE = { 0x1D, 0x07, 0x01, 0x00 };
/* AGC2_FREEZE bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_gain_status__AGC2_FREEZE = { 0x1D, 0x06, 0x01, 0x00 };
/* AGC3_FREEZE bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_gain_status__AGC3_FREEZE = { 0x1D, 0x05, 0x01, 0x00 };
/* AGCK_FREEZE bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_gain_status__AGCK_FREEZE = { 0x1D, 0x04, 0x01, 0x00 };
/* RSSI_ALARM bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_gain_status__RSSI_ALARM = { 0x1D, 0x03, 0x01, 0x00 };
/* AGC3_Gain_Read bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC3_gain_status__AGC3_Gain_Read = { 0x1D, 0x00, 0x03, 0x00 };


/* TDA18250A Register W_Filter_byte 0x1E */
static const TDA18250A_BitField_t gTDA18250A_Reg_W_Filter_byte = { 0x1E, 0x00, 0x08, 0x00 };
/* VHF_III_mode bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_W_Filter_byte__VHF_III_mode = { 0x1E, 0x07, 0x01, 0x00 };
/* W_Filter_Enable bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_W_Filter_byte__W_Filter_Enable = { 0x1E, 0x05, 0x01, 0x00 };
/* W_Filter_Bypass bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_W_Filter_byte__W_Filter_Bypass = { 0x1E, 0x04, 0x01, 0x00 };
/* W_Filter bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_W_Filter_byte__W_Filter = { 0x1E, 0x02, 0x02, 0x00 };
/* W_Filter_Offset bit(s): 00:e+4% 01:enominal 10:e-4% 11:e-8% */
static const TDA18250A_BitField_t gTDA18250A_Reg_W_Filter_byte__W_Filter_Offset = { 0x1E, 0x00, 0x02, 0x00 };


/* TDA18250A Register RF_Band_Pass_Filter_byte 0x1F */
static const TDA18250A_BitField_t gTDA18250A_Reg_RF_Band_Pass_Filter_byte = { 0x1F, 0x00, 0x08, 0x00 };
/* RF_BPF_Bypass bit(s): BypassealleBPFeselection */
static const TDA18250A_BitField_t gTDA18250A_Reg_RF_Band_Pass_Filter_byte__RF_BPF_Bypass = { 0x1F, 0x07, 0x01, 0x00 };
/* RF_BPF bit(s): H3/H5eROMebypass */
static const TDA18250A_BitField_t gTDA18250A_Reg_RF_Band_Pass_Filter_byte__RF_BPF = { 0x1F, 0x00, 0x03, 0x00 };


/* TDA18250A Register IR_Mixer_byte_1 0x20 */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Mixer_byte_1 = { 0x20, 0x00, 0x08, 0x00 };
/* Mixer_GateDC bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Mixer_byte_1__Mixer_GateDC = { 0x20, 0x03, 0x02, 0x00 };
/* S2D_Gain bit(s): StoBegain:  00:e+3dB 01:e+6dB 1x:e+9dB */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Mixer_byte_1__S2D_Gain = { 0x20, 0x01, 0x02, 0x00 };
/* DC_Notch bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Mixer_byte_1__DC_Notch = { 0x20, 0x00, 0x01, 0x00 };


/* TDA18250A Register IF_AGC_byte 0x21 */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_AGC_byte = { 0x21, 0x00, 0x08, 0x00 };
/* IFnotchToRSSI bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_AGC_byte__IFnotchToRSSI = { 0x21, 0x07, 0x01, 0x00 };
/* LPF_DCOffset_Corr bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_AGC_byte__LPF_DCOffset_Corr = { 0x21, 0x06, 0x01, 0x00 };
/* IFAgcToRSSI bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_AGC_byte__IFAgcToRSSI = { 0x21, 0x05, 0x01, 0x00 };
/* IF_level bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_AGC_byte__IF_level = { 0x21, 0x00, 0x03, 0x00 };


/* TDA18250A Register IF_Byte_1 0x22 */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_1 = { 0x22, 0x00, 0x08, 0x00 };
/* IF_LPF_Offset bit(s): ValeuredeedécalageedeeRCCaleIF_LPFeenenonesigné 00:eNominal 01:e-4% 10:e-8% 11:-12% */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_1__IF_LPF_Offset = { 0x22, 0x05, 0x02, 0x00 };
/* IF_LPF_sign bit(s): frq05249: '0':eshifteposiitfe(1eintroduitseparelaedroite) '1':eshiftenégatife(0eintroduitseparelaegauche) */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_1__IF_LPF_sign = { 0x22, 0x04, 0x01, 0x00 };
/* LP_Fc bit(s): IF_ATSC_NotchefrequencyecorrelatedewitheLPF  000e:e6MHz 001e:e7MHz 010e:e8MHz 011:e9MHzesieROWe(1),e8MHzesieUSe(0) 1xx:e1.5MHz */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_1__LP_Fc = { 0x22, 0x00, 0x03, 0x00 };


/* TDA18250A Register IF_Byte_2 0x23 */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_2 = { 0x23, 0x00, 0x08, 0x00 };
/* IF_HPF_Offset bit(s): frq05249: ValeuredeedécalageedeeRCCaleIF_HPFeenenonesigné */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_2__IF_HPF_Offset = { 0x23, 0x05, 0x02, 0x00 };
/* IF_HPF_sign bit(s): frq05249: '0':eshifteposiitfe(1eintroduitseparelaedroite) '1':eshiftenégatife(0eintroduitseparelaegauche) */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_2__IF_HPF_sign = { 0x23, 0x04, 0x01, 0x00 };
/* IF_HP_Fc bit(s): IFeHigh-PasseFilterefrequency */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_2__IF_HP_Fc = { 0x23, 0x01, 0x03, 0x00 };
/* IF_HP_Enable bit(s): Enableehigh-passefilter */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_2__IF_HP_Enable = { 0x23, 0x00, 0x01, 0x00 };


/* TDA18250A Register IF_Byte_3 0x24 */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_3 = { 0x24, 0x00, 0x08, 0x00 };
/* IF_Notch_Offset bit(s): frq05249: ValeuredeedécalageedeeRCCaleIF_Notcheenenonesigné */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_3__IF_Notch_Offset = { 0x24, 0x05, 0x02, 0x00 };
/* IF_Notch_sign bit(s): frq05249: '0':eshifteposiitfe(1eintroduitseparelaedroite) '1':eshiftenégatife(0eintroduitseparelaegauche) */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_3__IF_Notch_sign = { 0x24, 0x04, 0x01, 0x00 };
/* IF_Notch_Freq bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_3__IF_Notch_Freq = { 0x24, 0x01, 0x03, 0x00 };
/* IF_Notch_Enable bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Byte_3__IF_Notch_Enable = { 0x24, 0x00, 0x01, 0x00 };


/* TDA18250A Register Reference_Byte 0x25 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Reference_Byte = { 0x25, 0x00, 0x08, 0x00 };
/* Dig_Clock bit(s): x0e:e16MHz 01e:eSigma-delta 11e:eSub-LO */
static const TDA18250A_BitField_t gTDA18250A_Reg_Reference_Byte__Dig_Clock = { 0x25, 0x06, 0x02, 0x00 };
/* Xtout_Amp bit(s): jian: '000':e200emV '001':e400emV '010':e600emV '011':e800emV '100':e600emV '101':e800emV '110':e1000emV '111':e1200emV */
static const TDA18250A_BitField_t gTDA18250A_Reg_Reference_Byte__Xtout_Amp = { 0x25, 0x03, 0x03, 0x00 };
/* XTout bit(s): AllumerelaesortieeAsymeeteéteindreeSym 000,100e:eNoesignal 001eeeeeeee:eFdivePLL 010eeeeeeee:eDigClock:e eeeeeeeeeeeeeeeeSubLOeoreSDeoreXTeeeeeeeeforeXTeise16MHz eeeeeeeeeeeeeeeeSubLOeoreSDeoreXT/2eeeeforeXTeisenote16MHz 011,111e:e16-30MHzesinus 101,110e:e16-30eXTeerail-to-rail  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Reference_Byte__XTout = { 0x25, 0x00, 0x03, 0x00 };


/* TDA18250A Register IF_Frequency_byte 0x26 */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Frequency_byte = { 0x26, 0x00, 0x08, 0x00 };
/* IF_Freq bit(s): Stepe50ekHz  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IF_Frequency_byte__IF_Freq = { 0x26, 0x00, 0x08, 0x00 };


/* TDA18250A Register RF_Frequency_byte_1 0x27 */
static const TDA18250A_BitField_t gTDA18250A_Reg_RF_Frequency_byte_1 = { 0x27, 0x00, 0x08, 0x00 };
/* RF_Freq_1 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RF_Frequency_byte_1__RF_Freq_1 = { 0x27, 0x00, 0x04, 0x00 };


/* TDA18250A Register RF_Frequency_byte_2 0x28 */
static const TDA18250A_BitField_t gTDA18250A_Reg_RF_Frequency_byte_2 = { 0x28, 0x00, 0x08, 0x00 };
/* RF_Freq_2 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RF_Frequency_byte_2__RF_Freq_2 = { 0x28, 0x00, 0x08, 0x00 };


/* TDA18250A Register RF_Frequency_byte_3 0x29 */
static const TDA18250A_BitField_t gTDA18250A_Reg_RF_Frequency_byte_3 = { 0x29, 0x00, 0x08, 0x00 };
/* RF_Freq_3 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RF_Frequency_byte_3__RF_Freq_3 = { 0x29, 0x00, 0x08, 0x00 };


/* TDA18250A Register MSM_byte_1 0x2A */
static const TDA18250A_BitField_t gTDA18250A_Reg_MSM_byte_1 = { 0x2A, 0x00, 0x08, 0x00 };
/* RSSI_Meas bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_MSM_byte_1__RSSI_Meas = { 0x2A, 0x07, 0x01, 0x00 };
/* IR_CAL_Loop bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_MSM_byte_1__IR_CAL_Loop = { 0x2A, 0x06, 0x01, 0x00 };
/* IR_Cal_Image bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_MSM_byte_1__IR_Cal_Image = { 0x2A, 0x05, 0x01, 0x00 };
/* IR_CAL_Wanted bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_MSM_byte_1__IR_CAL_Wanted = { 0x2A, 0x04, 0x01, 0x00 };
/* RC_Cal bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_MSM_byte_1__RC_Cal = { 0x2A, 0x01, 0x01, 0x00 };
/* Calc_PLL bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_MSM_byte_1__Calc_PLL = { 0x2A, 0x00, 0x01, 0x00 };


/* TDA18250A Register MSM_byte_2 0x2B */
static const TDA18250A_BitField_t gTDA18250A_Reg_MSM_byte_2 = { 0x2B, 0x00, 0x08, 0x00 };
/* Fast_Auto_Launch bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_MSM_byte_2__Fast_Auto_Launch = { 0x2B, 0x02, 0x01, 0x00 };
/* XtalCal_Launch bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_MSM_byte_2__XtalCal_Launch = { 0x2B, 0x01, 0x01, 0x00 };
/* MSM_Launch bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_MSM_byte_2__MSM_Launch = { 0x2B, 0x00, 0x01, 0x00 };


/* TDA18250A Register PowerSavingMode_byte_1 0x2C */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_1 = { 0x2C, 0x00, 0x08, 0x00 };
/* PSM_RFpoly bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_RFpoly = { 0x2C, 0x06, 0x02, 0x00 };
/* PSM_Mixer bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Mixer = { 0x2C, 0x04, 0x02, 0x00 };
/* PSM_Ifpoly bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Ifpoly = { 0x2C, 0x02, 0x02, 0x00 };
/* PSM_Lodriver bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Lodriver = { 0x2C, 0x00, 0x02, 0x00 };


/* TDA18250A Register PowerSavingMode_byte_2 0x2D */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_2 = { 0x2D, 0x00, 0x08, 0x00 };
/* H3H5_boost_stage_2_3 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_2__H3H5_boost_stage_2_3 = { 0x2D, 0x07, 0x01, 0x00 };
/* H3H5_boost bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_2__H3H5_boost = { 0x2D, 0x06, 0x01, 0x00 };
/* PSM_H3H5 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_H3H5 = { 0x2D, 0x04, 0x02, 0x00 };
/* AGC1_boost bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_2__AGC1_boost = { 0x2D, 0x02, 0x02, 0x00 };
/* PSM_VCO bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_VCO = { 0x2D, 0x00, 0x02, 0x00 };


/* TDA18250A Register PowerSavingMode_byte_3 0x2E */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_3 = { 0x2E, 0x00, 0x08, 0x00 };
/* PSM_DCC bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_DCC = { 0x2E, 0x04, 0x01, 0x00 };
/* PSM_lpf bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_lpf = { 0x2E, 0x02, 0x02, 0x00 };
/* PSM_hpf_ifn bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_hpf_ifn = { 0x2E, 0x00, 0x02, 0x00 };


/* TDA18250A Register RSSI_byte_1 0x2F */
static const TDA18250A_BitField_t gTDA18250A_Reg_RSSI_byte_1 = { 0x2F, 0x00, 0x08, 0x00 };
/* RSSI bit(s): SortieedirecteedueRSSI */
static const TDA18250A_BitField_t gTDA18250A_Reg_RSSI_byte_1__RSSI = { 0x2F, 0x00, 0x08, 0x00 };


/* TDA18250A Register RSSI_byte_2 0x30 */
static const TDA18250A_BitField_t gTDA18250A_Reg_RSSI_byte_2 = { 0x30, 0x00, 0x08, 0x00 };
/* RSSI_Gv bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RSSI_byte_2__RSSI_Gv = { 0x30, 0x06, 0x02, 0x00 };
/* RSSI_AV bit(s): RSSIeaverage e0e:elente:e5bits 1e:erapidee:e2bitse(noneconseilléeeneQAM)  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RSSI_byte_2__RSSI_AV = { 0x30, 0x05, 0x01, 0x00 };
/* RSSI_Mode bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RSSI_byte_2__RSSI_Mode = { 0x30, 0x04, 0x01, 0x00 };
/* RSSI_Cap_Reset_En bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RSSI_byte_2__RSSI_Cap_Reset_En = { 0x30, 0x03, 0x01, 0x00 };
/* RSSI_Cap_Val bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RSSI_byte_2__RSSI_Cap_Val = { 0x30, 0x02, 0x01, 0x00 };
/* RSSI_Ck_Speed bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RSSI_byte_2__RSSI_Ck_Speed = { 0x30, 0x01, 0x01, 0x00 };
/* RSSI_Dicho_not bit(s): RSSI_Dicho_not=0e=>emodeeDichoeON RSSI_Dicho_not=1e=>emodeeCountereON  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RSSI_byte_2__RSSI_Dicho_not = { 0x30, 0x00, 0x01, 0x00 };


/* TDA18250A Register IRQ_Ctrl_byte 0x31 */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_Ctrl_byte = { 0x31, 0x00, 0x08, 0x00 };
/* DDS_Polarity bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_Ctrl_byte__DDS_Polarity = { 0x31, 0x04, 0x01, 0x00 };
/* IRQ_Mode bit(s): 0:eIRQ 1:eVsynce(Boosterel'interfaceeIRQ,eidemeScantest) */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_Ctrl_byte__IRQ_Mode = { 0x31, 0x01, 0x01, 0x00 };
/* IRQ_Polarity bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IRQ_Ctrl_byte__IRQ_Polarity = { 0x31, 0x00, 0x01, 0x00 };


/* TDA18250A Register Dummy_byte 0x32 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Dummy_byte = { 0x32, 0x00, 0x08, 0x00 };
/* Dummy bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Dummy_byte__Dummy = { 0x32, 0x00, 0x08, 0x00 };


/* TDA18250A Register Test_byte 0x33 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Test_byte = { 0x33, 0x00, 0x08, 0x00 };
/* Dig_debug_mode bit(s): permetedeeprendreelaemainesurelaeclockeduedigitalepouredebugeetecaracedeseblocseanalogeenemodeedeeCAL,e... */
static const TDA18250A_BitField_t gTDA18250A_Reg_Test_byte__Dig_debug_mode = { 0x33, 0x04, 0x01, 0x00 };
/* RSSICALEN bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Test_byte__RSSICALEN = { 0x33, 0x02, 0x01, 0x00 };
/* RF_Test bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Test_byte__RF_Test = { 0x33, 0x01, 0x01, 0x00 };
/* AGCK_Test bit(s): Resete&eexternalesynchroeviaeVsync foretestepurpose. */
static const TDA18250A_BitField_t gTDA18250A_Reg_Test_byte__AGCK_Test = { 0x33, 0x00, 0x01, 0x00 };


/* TDA18250A Register Main_Post_Divider_byte 0x34 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Main_Post_Divider_byte = { 0x34, 0x00, 0x08, 0x00 };
/* LOPostDiv bit(s): 1e:e1 2e:e2 3e:e4 4e:e8 5e:e16 6e:eTesteinput */
static const TDA18250A_BitField_t gTDA18250A_Reg_Main_Post_Divider_byte__LOPostDiv = { 0x34, 0x04, 0x03, 0x00 };
/* LOPresc bit(s): 7e:e7 8e:e8 ….. 15e:e15 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Main_Post_Divider_byte__LOPresc = { 0x34, 0x00, 0x04, 0x00 };


/* TDA18250A Register Sigma_delta_byte_1 0x35 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Sigma_delta_byte_1 = { 0x35, 0x00, 0x08, 0x00 };
/* LO_Int bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Sigma_delta_byte_1__LO_Int = { 0x35, 0x00, 0x07, 0x00 };


/* TDA18250A Register Sigma_delta_byte_2 0x36 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Sigma_delta_byte_2 = { 0x36, 0x00, 0x08, 0x00 };
/* LO_Frac_2 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Sigma_delta_byte_2__LO_Frac_2 = { 0x36, 0x00, 0x07, 0x00 };


/* TDA18250A Register Sigma_delta_byte_3 0x37 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Sigma_delta_byte_3 = { 0x37, 0x00, 0x08, 0x00 };
/* LO_Frac_1 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Sigma_delta_byte_3__LO_Frac_1 = { 0x37, 0x00, 0x08, 0x00 };


/* TDA18250A Register Sigma_delta_byte_4 0x38 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Sigma_delta_byte_4 = { 0x38, 0x00, 0x08, 0x00 };
/* LO_Frac_0 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Sigma_delta_byte_4__LO_Frac_0 = { 0x38, 0x00, 0x08, 0x00 };


/* TDA18250A Register Sigma_delta_byte_5 0x39 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Sigma_delta_byte_5 = { 0x39, 0x00, 0x08, 0x00 };
/* LO_SetAll bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Sigma_delta_byte_5__LO_SetAll = { 0x39, 0x02, 0x01, 0x00 };
/* N_K_correct_manual bit(s): DisableeN,Kecorrection */
static const TDA18250A_BitField_t gTDA18250A_Reg_Sigma_delta_byte_5__N_K_correct_manual = { 0x39, 0x01, 0x01, 0x00 };
/* LO_Calc_Disable bit(s): Disableesyntheeautoecalculation */
static const TDA18250A_BitField_t gTDA18250A_Reg_Sigma_delta_byte_5__LO_Calc_Disable = { 0x39, 0x00, 0x01, 0x00 };


/* TDA18250A Register SigmaDelta_Test_byte 0x3A */
static const TDA18250A_BitField_t gTDA18250A_Reg_SigmaDelta_Test_byte = { 0x3A, 0x00, 0x08, 0x00 };
/* LOSD_clock bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_SigmaDelta_Test_byte__LOSD_clock = { 0x3A, 0x06, 0x01, 0x00 };
/* LOSD_Static_N bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_SigmaDelta_Test_byte__LOSD_Static_N = { 0x3A, 0x05, 0x01, 0x00 };
/* LOSD_BascD bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_SigmaDelta_Test_byte__LOSD_BascD = { 0x3A, 0x04, 0x01, 0x00 };
/* LOSD_dpa2 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_SigmaDelta_Test_byte__LOSD_dpa2 = { 0x3A, 0x03, 0x01, 0x00 };
/* LOSD_dpa1 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_SigmaDelta_Test_byte__LOSD_dpa1 = { 0x3A, 0x02, 0x01, 0x00 };
/* LOSDrstN bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_SigmaDelta_Test_byte__LOSDrstN = { 0x3A, 0x01, 0x01, 0x00 };
/* LOSD_test_En bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_SigmaDelta_Test_byte__LOSD_test_En = { 0x3A, 0x00, 0x01, 0x00 };


/* TDA18250A Register Regulators_byte 0x3B */
static const TDA18250A_BitField_t gTDA18250A_Reg_Regulators_byte = { 0x3B, 0x00, 0x08, 0x00 };
/* Dig_Reg bit(s): 00e:e1.6V 01e:e1.8Ve->enominal 10e:e1.8Ve+e10mAemode 11e:e2.0V */
static const TDA18250A_BitField_t gTDA18250A_Reg_Regulators_byte__Dig_Reg = { 0x3B, 0x06, 0x02, 0x00 };
/* IF_Reg bit(s): 00e:e2.6V 01e:e2.8Ve->enominal 1Xe:e3.0V */
static const TDA18250A_BitField_t gTDA18250A_Reg_Regulators_byte__IF_Reg = { 0x3B, 0x04, 0x02, 0x00 };
/* RF_Reg bit(s): 00e:e2.6V 01e:e2.8Ve->enominal 1Xe:e3.0V */
static const TDA18250A_BitField_t gTDA18250A_Reg_Regulators_byte__RF_Reg = { 0x3B, 0x02, 0x02, 0x00 };
/* VCO_Reg bit(s): 00e:e2.6V 01e:e2.8Ve->enominal 1Xe:e3.0V */
static const TDA18250A_BitField_t gTDA18250A_Reg_Regulators_byte__VCO_Reg = { 0x3B, 0x00, 0x02, 0x00 };


/* TDA18250A Register RC_Cal_byte_1 0x3C */
static const TDA18250A_BitField_t gTDA18250A_Reg_RC_Cal_byte_1 = { 0x3C, 0x00, 0x08, 0x00 };
/* RC_Cal_bypass bit(s): PermetedeeforcerelaeRCCAL RC_Cal_bypass[3:0]  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RC_Cal_byte_1__RC_Cal_bypass = { 0x3C, 0x07, 0x01, 0x00 };
/* RC_Cal_Offset bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RC_Cal_byte_1__RC_Cal_Offset = { 0x3C, 0x05, 0x02, 0x00 };
/* RC_Cal_force bit(s): ModifierelaeRCeCalepourequeeinfoseRead=Write! */
static const TDA18250A_BitField_t gTDA18250A_Reg_RC_Cal_byte_1__RC_Cal_force = { 0x3C, 0x00, 0x05, 0x00 };


/* TDA18250A Register RC_Cal_byte_2 0x3D */
static const TDA18250A_BitField_t gTDA18250A_Reg_RC_Cal_byte_2 = { 0x3D, 0x00, 0x08, 0x00 };
/* RCcal bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_RC_Cal_byte_2__RCcal = { 0x3D, 0x00, 0x05, 0x00 };


/* TDA18250A Register IR_Cal_byte_2 0x3E */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_2 = { 0x3E, 0x00, 0x08, 0x00 };
/* IR_Mixer_Atten bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_2__IR_Mixer_Atten = { 0x3E, 0x02, 0x03, 0x00 };
/* IRCal_EndLoop bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_2__IRCal_EndLoop = { 0x3E, 0x00, 0x02, 0x00 };


/* TDA18250A Register IR_Cal_byte_3 0x3F */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_3 = { 0x3F, 0x00, 0x08, 0x00 };
/* IR_Step_Delta bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_3__IR_Step_Delta = { 0x3F, 0x04, 0x04, 0x00 };
/* IR_Cal_loop bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_3__IR_Cal_loop = { 0x3F, 0x00, 0x04, 0x00 };


/* TDA18250A Register IR_Cal_byte_4 0x40 */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_4 = { 0x40, 0x00, 0x08, 0x00 };
/* IR_R2x_only bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_4__IR_R2x_only = { 0x40, 0x06, 0x01, 0x00 };
/* Rx_Force bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_4__Rx_Force = { 0x40, 0x05, 0x01, 0x00 };
/* IR_Freq_Bypass bit(s): BypasseMixeregainevaluee&eBypasseIR_FreqPtr */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_4__IR_Freq_Bypass = { 0x40, 0x04, 0x01, 0x00 };
/* IR_FreqPtr bit(s): Pointeuredeefreqedeecorrection */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_4__IR_FreqPtr = { 0x40, 0x02, 0x02, 0x00 };
/* IR_RxPtr bit(s): PointeureR2I,R2Q,R3IQn,R3InQe:euseeforeread/write */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_4__IR_RxPtr = { 0x40, 0x00, 0x02, 0x00 };


/* TDA18250A Register IR_Cal_byte_5 0x41 */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_5 = { 0x41, 0x00, 0x08, 0x00 };
/* IR_target bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_5__IR_target = { 0x41, 0x06, 0x02, 0x00 };
/* Rx_bp bit(s): ValeuredeecorrectioneR2,R3eduemixer */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_5__Rx_bp = { 0x41, 0x00, 0x06, 0x00 };


/* TDA18250A Register IR_Cal_byte_6 0x42 */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_6 = { 0x42, 0x00, 0x08, 0x00 };
/* Rx bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_IR_Cal_byte_6__Rx = { 0x42, 0x00, 0x06, 0x00 };


/* TDA18250A Register Power_Down_byte_1 0x43 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_Down_byte_1 = { 0x43, 0x00, 0x08, 0x00 };
/* S2B_Dcboost bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_Down_byte_1__S2B_Dcboost = { 0x43, 0x06, 0x02, 0x00 };
/* AGC2_Dcboost bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_Down_byte_1__AGC2_Dcboost = { 0x43, 0x03, 0x01, 0x00 };
/* PD_Lotest bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_Down_byte_1__PD_Lotest = { 0x43, 0x02, 0x01, 0x00 };
/* PD_Synthe bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_Down_byte_1__PD_Synthe = { 0x43, 0x01, 0x01, 0x00 };
/* PD_VCO bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_Down_byte_1__PD_VCO = { 0x43, 0x00, 0x01, 0x00 };


/* TDA18250A Register Power_Down_byte_2 0x44 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_Down_byte_2 = { 0x44, 0x00, 0x08, 0x00 };
/* PD_AGC2 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_Down_byte_2__PD_AGC2 = { 0x44, 0x04, 0x01, 0x00 };
/* PD_S2B_IF bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_Down_byte_2__PD_S2B_IF = { 0x44, 0x03, 0x01, 0x00 };
/* PD_Det3 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_Down_byte_2__PD_Det3 = { 0x44, 0x02, 0x01, 0x00 };
/* PD_Det2 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_Down_byte_2__PD_Det2 = { 0x44, 0x01, 0x01, 0x00 };
/* PD_Det1 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Power_Down_byte_2__PD_Det1 = { 0x44, 0x00, 0x01, 0x00 };


/* TDA18250A Register Powerdown 0x45 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Powerdown = { 0x45, 0x00, 0x08, 0x00 };
/* force_soft_reset bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Powerdown__force_soft_reset = { 0x45, 0x01, 0x01, 0x00 };
/* Soft_reset bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Powerdown__Soft_reset = { 0x45, 0x00, 0x01, 0x00 };


/* TDA18250A Register Charge_pump_byte 0x46 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Charge_pump_byte = { 0x46, 0x00, 0x08, 0x00 };
/* ICP_Force_Sink bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Charge_pump_byte__ICP_Force_Sink = { 0x46, 0x06, 0x01, 0x00 };
/* ICP_Force_Src bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Charge_pump_byte__ICP_Force_Src = { 0x46, 0x05, 0x01, 0x00 };
/* ICP_offset bit(s): DCeoffsetecurrent: 00e=e10% 01e=e5% 1xe=e0% */
static const TDA18250A_BitField_t gTDA18250A_Reg_Charge_pump_byte__ICP_offset = { 0x46, 0x03, 0x02, 0x00 };
/* ICP bit(s): Inenormalemode:einternalyecontrolede=ef(Ndiv)  00:e500µA 01:e150µA 10:e300µA 11:e100µA */
static const TDA18250A_BitField_t gTDA18250A_Reg_Charge_pump_byte__ICP = { 0x46, 0x00, 0x03, 0x00 };


/* TDA18250A Register lna_polar_casc_byte_1 0x47 */
static const TDA18250A_BitField_t gTDA18250A_Reg_lna_polar_casc_byte_1 = { 0x47, 0x00, 0x08, 0x00 };
/* lna_Vcollec bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_lna_polar_casc_byte_1__lna_Vcollec = { 0x47, 0x04, 0x03, 0x00 };
/* lna_Vcasc bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_lna_polar_casc_byte_1__lna_Vcasc = { 0x47, 0x00, 0x03, 0x00 };


/* TDA18250A Register smooth_test_byte_1 0x48 */
static const TDA18250A_BitField_t gTDA18250A_Reg_smooth_test_byte_1 = { 0x48, 0x00, 0x08, 0x00 };
/* rf_mux_test bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_smooth_test_byte_1__rf_mux_test = { 0x48, 0x00, 0x07, 0x00 };


/* TDA18250A Register smooth_test_byte_2 0x49 */
static const TDA18250A_BitField_t gTDA18250A_Reg_smooth_test_byte_2 = { 0x49, 0x00, 0x08, 0x00 };
/* agc1_smooth_test bit(s): frq05249: '0':enormalemode '1':eagc1esmoothetestemode */
static const TDA18250A_BitField_t gTDA18250A_Reg_smooth_test_byte_2__agc1_smooth_test = { 0x49, 0x07, 0x01, 0x00 };
/* agc1_cmd_test bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_smooth_test_byte_2__agc1_cmd_test = { 0x49, 0x00, 0x06, 0x00 };


/* TDA18250A Register smooth_test_byte_3 0x4A */
static const TDA18250A_BitField_t gTDA18250A_Reg_smooth_test_byte_3 = { 0x4A, 0x00, 0x08, 0x00 };
/* lt_smooth_test bit(s): frq05249: '0':enormalemode '1':eLTesmootheinetestemode */
static const TDA18250A_BitField_t gTDA18250A_Reg_smooth_test_byte_3__lt_smooth_test = { 0x4A, 0x07, 0x01, 0x00 };
/* lt_cmd_test bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_smooth_test_byte_3__lt_cmd_test = { 0x4A, 0x00, 0x04, 0x00 };


/* TDA18250A Register XtalOsc_byte_1 0x4B */
static const TDA18250A_BitField_t gTDA18250A_Reg_XtalOsc_byte_1 = { 0x4B, 0x00, 0x08, 0x00 };
/* XT_Cal_Bypass bit(s): PermetedeeforcerelaevaleuredueDAC */
static const TDA18250A_BitField_t gTDA18250A_Reg_XtalOsc_byte_1__XT_Cal_Bypass = { 0x4B, 0x07, 0x01, 0x00 };
/* xt_cal_mode bit(s): frq05249: '0':eslowemode '1':efastemode */
static const TDA18250A_BitField_t gTDA18250A_Reg_XtalOsc_byte_1__xt_cal_mode = { 0x4B, 0x06, 0x01, 0x00 };
/* xt_cal_timer bit(s): frq05249: xtalecaletimereselection '00':e5,12ems '01':e10,24ems '10':e20,48ems '11':e40,96ems */
static const TDA18250A_BitField_t gTDA18250A_Reg_XtalOsc_byte_1__xt_cal_timer = { 0x4B, 0x04, 0x02, 0x00 };
/* XT_Dac_Force bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_XtalOsc_byte_1__XT_Dac_Force = { 0x4B, 0x00, 0x04, 0x00 };


/* TDA18250A Register XtalOsc_byte_2 0x4C */
static const TDA18250A_BitField_t gTDA18250A_Reg_XtalOsc_byte_2 = { 0x4C, 0x00, 0x08, 0x00 };
/* XTSel bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_XtalOsc_byte_2__XTSel = { 0x4C, 0x04, 0x03, 0x00 };
/* XT_Dac bit(s): RésultatsedeelaeCAL */
static const TDA18250A_BitField_t gTDA18250A_Reg_XtalOsc_byte_2__XT_Dac = { 0x4C, 0x00, 0x04, 0x00 };


/* TDA18250A Register Xtal_Flex_byte_1 0x4D */
static const TDA18250A_BitField_t gTDA18250A_Reg_Xtal_Flex_byte_1 = { 0x4D, 0x00, 0x08, 0x00 };
/* XT_Freq_2 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Xtal_Flex_byte_1__XT_Freq_2 = { 0x4D, 0x00, 0x07, 0x00 };


/* TDA18250A Register Xtal_Flex_byte_2 0x4E */
static const TDA18250A_BitField_t gTDA18250A_Reg_Xtal_Flex_byte_2 = { 0x4E, 0x00, 0x08, 0x00 };
/* XT_Freq_1 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Xtal_Flex_byte_2__XT_Freq_1 = { 0x4E, 0x00, 0x08, 0x00 };


/* TDA18250A Register Xtal_Flex_byte_3 0x4F */
static const TDA18250A_BitField_t gTDA18250A_Reg_Xtal_Flex_byte_3 = { 0x4F, 0x00, 0x08, 0x00 };
/* R_Div bit(s): Jian:eR_Div[1:0]: 00e:eforbidden 01e:edive1 10e:edive2 11e:edive3 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Xtal_Flex_byte_3__R_Div = { 0x4F, 0x06, 0x02, 0x00 };
/* N_Div bit(s): Jian:eNediv: 0:edive1e 1:edive2e */
static const TDA18250A_BitField_t gTDA18250A_Reg_Xtal_Flex_byte_3__N_Div = { 0x4F, 0x05, 0x01, 0x00 };
/* Force_Icp_256us_2 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Xtal_Flex_byte_3__Force_Icp_256us_2 = { 0x4F, 0x00, 0x05, 0x00 };


/* TDA18250A Register Xtal_Flex_byte_4 0x50 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Xtal_Flex_byte_4 = { 0x50, 0x00, 0x08, 0x00 };
/* Force_Icp_256us_1 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Xtal_Flex_byte_4__Force_Icp_256us_1 = { 0x50, 0x00, 0x08, 0x00 };


/* TDA18250A Register Xtal_flex_byte_5 0x51 */
static const TDA18250A_BitField_t gTDA18250A_Reg_Xtal_flex_byte_5 = { 0x51, 0x00, 0x08, 0x00 };
/* RC_Delay_Ck_Length bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_Xtal_flex_byte_5__RC_Delay_Ck_Length = { 0x51, 0x00, 0x06, 0x00 };


/* TDA18250A Register ircal_loop_0 0x52 */
static const TDA18250A_BitField_t gTDA18250A_Reg_ircal_loop_0 = { 0x52, 0x00, 0x08, 0x00 };
/* ircal_loop_0 bit(s): frq04920: ir_newevalue:eirelevel */
static const TDA18250A_BitField_t gTDA18250A_Reg_ircal_loop_0__ircal_loop_0 = { 0x52, 0x00, 0x08, 0x00 };


/* TDA18250A Register ircal_loop_1 0x53 */
static const TDA18250A_BitField_t gTDA18250A_Reg_ircal_loop_1 = { 0x53, 0x00, 0x08, 0x00 };
/* ircal_loop_1 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_ircal_loop_1__ircal_loop_1 = { 0x53, 0x00, 0x08, 0x00 };


/* TDA18250A Register ircal_loop_2 0x54 */
static const TDA18250A_BitField_t gTDA18250A_Reg_ircal_loop_2 = { 0x54, 0x00, 0x08, 0x00 };
/* ircal_loop_2 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_ircal_loop_2__ircal_loop_2 = { 0x54, 0x00, 0x08, 0x00 };


/* TDA18250A Register ircal_loop_3 0x55 */
static const TDA18250A_BitField_t gTDA18250A_Reg_ircal_loop_3 = { 0x55, 0x00, 0x08, 0x00 };
/* ircal_loop_3 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_ircal_loop_3__ircal_loop_3 = { 0x55, 0x00, 0x08, 0x00 };


/* TDA18250A Register ircal_loop_4 0x56 */
static const TDA18250A_BitField_t gTDA18250A_Reg_ircal_loop_4 = { 0x56, 0x00, 0x08, 0x00 };
/* ircal_loop_4 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_ircal_loop_4__ircal_loop_4 = { 0x56, 0x00, 0x08, 0x00 };


/* TDA18250A Register pll_log 0x57 */
static const TDA18250A_BitField_t gTDA18250A_Reg_pll_log = { 0x57, 0x00, 0x08, 0x00 };
/* ES3_info bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_pll_log__ES3_info = { 0x57, 0x07, 0x01, 0x00 };
/* pll_log bit(s): frq04920: pll_state_oute<=e'000'ewhene(state_plle=ewt_boot)eelse eee'001'ewhene(state_plle=ecal_start)eelse eee'010'ewhene(state_plle=ecal_PostDiv)eelsee eee'011'ewhene(state_plle=ecal_fvco_compute)eelsee eee'100'ewhene(state_plle=ecal_Prescaler)eelsee eee'101'ewhene(state_plle=ecal_NK_div)eelsee eee'110'ewhene(state_plle=ecal_NK_calc)eelseeee eee'111' ee--epll_end  */
static const TDA18250A_BitField_t gTDA18250A_Reg_pll_log__pll_log = { 0x57, 0x00, 0x03, 0x00 };


/* TDA18250A Register AGC2_UDLD_byte_1 0x58 */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_UDLD_byte_1 = { 0x58, 0x00, 0x08, 0x00 };
/* AGC2_Up_udld_Step_asym_En bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_UDLD_byte_1__AGC2_Up_udld_Step_asym_En = { 0x58, 0x07, 0x01, 0x00 };
/* AGC2_Up_udld_step bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_UDLD_byte_1__AGC2_Up_udld_step = { 0x58, 0x05, 0x02, 0x00 };
/* AGC2_TOP_UP_UDLD bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_UDLD_byte_1__AGC2_TOP_UP_UDLD = { 0x58, 0x00, 0x05, 0x00 };


/* TDA18250A Register AGC2_UDLD_byte_2 0x59 */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_UDLD_byte_2 = { 0x59, 0x00, 0x08, 0x00 };
/* AGC2_detect2_gain_plus7 bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_UDLD_byte_2__AGC2_detect2_gain_plus7 = { 0x59, 0x07, 0x01, 0x00 };
/* AGC2_FAST_AUTO_DELTA bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC2_UDLD_byte_2__AGC2_FAST_AUTO_DELTA = { 0x59, 0x00, 0x04, 0x00 };


/* TDA18250A Register H3H5_CSTAB_byte 0x5A */
static const TDA18250A_BitField_t gTDA18250A_Reg_H3H5_CSTAB_byte = { 0x5A, 0x00, 0x08, 0x00 };
/* CSTAB_EN bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_H3H5_CSTAB_byte__CSTAB_EN = { 0x5A, 0x07, 0x01, 0x00 };
/* CSTAB bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_H3H5_CSTAB_byte__CSTAB = { 0x5A, 0x00, 0x05, 0x00 };


/* TDA18250A Register AGC_fast_auto_byte_1 0x5B */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC_fast_auto_byte_1 = { 0x5B, 0x00, 0x08, 0x00 };
/* AGC_detect_gain_select bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC_fast_auto_byte_1__AGC_detect_gain_select = { 0x5B, 0x06, 0x02, 0x00 };
/* AGC2_up_udld_fast_reduce bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC_fast_auto_byte_1__AGC2_up_udld_fast_reduce = { 0x5B, 0x05, 0x01, 0x00 };
/* AGC2_up_fast_reduce bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC_fast_auto_byte_1__AGC2_up_fast_reduce = { 0x5B, 0x04, 0x01, 0x00 };
/* AGC_timer_mod_fast_auto bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC_fast_auto_byte_1__AGC_timer_mod_fast_auto = { 0x5B, 0x02, 0x02, 0x00 };
/* AGC_type_fast_auto bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC_fast_auto_byte_1__AGC_type_fast_auto = { 0x5B, 0x00, 0x02, 0x00 };


/* TDA18250A Register AGC_debug_byte_1 0x5C */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC_debug_byte_1 = { 0x5C, 0x00, 0x08, 0x00 };
/* AGC_detect_gain bit(s):  */
static const TDA18250A_BitField_t gTDA18250A_Reg_AGC_debug_byte_1__AGC_detect_gain = { 0x5C, 0x00, 0x05, 0x00 };



/*============================================================================*/
/* End of registers definitions:                                              */
/*============================================================================*/

/*============================================================================*/
/* TDA18250A Types and defines:                                               */
/*============================================================================*/
/* Middle of VCO frequency excursion : VCOmin + (VCOmax - VCOmin)/2 in KHz */
#define TDA18250A_MIDDLE_FVCO_RANGE ((6818000 - 5965750) / 2 + 5965750)

typedef struct _TDA18250A_VCOMode_t
{
    UInt32 uVCOFreq; /* Lowest limit */
    UInt8 uRDiv;
    UInt8 uNDiv;
    UInt8 uICP;
}TDA18250A_VCOMode_t;

typedef struct _TDA18250A_RDivNDivTableDef_
{
    UInt32 uNbMode;
    TDA18250A_VCOMode_t* psVCOModes;
	UInt32 uXtalFreq;
	UInt32 uForceIcp256us;
	UInt32 uRcDelayLength;
} TDA18250A_RDivNDivTableDef;

typedef struct _TDA18250A_PostDivPrescalerTableDef_
{
    UInt32 LO_max;
    UInt32 LO_min;
    UInt8 Prescaler;
    UInt8 PostDiv;
} TDA18250A_PostDivPrescalerTableDef;

typedef enum  _TDA18250ALoopThrough_t {
	TDA18250A_LoopThrough_Enabled = 0,
	TDA18250A_LoopThrough_Disabled,
    TDA18250A_LoopThrough_Max 
} TDA18250ALoopThrough_t, *pTDA18250ALoopThrough_t;

/* TDA18250A Driver State Machine */
typedef enum _TDA18250AHwState_t {
    TDA18250A_HwState_Unknown = 0,   /* Hw State Unknown */
    TDA18250A_HwState_InitNotDone,   /* Hw Init Not Done */
    TDA18250A_HwState_InitPending,   /* Hw Init Pending */
    TDA18250A_HwState_InitDone,      /* Hw Init Done */
    TDA18250A_HwState_SetStdDone,    /* Set Standard Done */
    TDA18250A_HwState_SetRFDone,     /* Set RF Done */
    TDA18250A_HwState_SetFineRFDone, /* Set Fine RF Done */
    TDA18250A_HwState_Max
} TDA18250AHwState_t, *pTDA18250AHwState_t;

typedef enum _TDA18250AHwStateCaller_t {
    TDA18250A_HwStateCaller_Unknown = 0, /* Caller Unknown */
    TDA18250A_HwStateCaller_SetPower,    /* Caller SetPowerState */
    TDA18250A_HwStateCaller_HwInit,      /* Caller HwInit */
    TDA18250A_HwStateCaller_SetStd,      /* Caller SetStandardMode */
    TDA18250A_HwStateCaller_SetRF,       /* Caller SetRF */
    TDA18250A_HwStateCaller_SetFineRF,   /* Caller SetFineRF */
    TDA18250A_HwStateCaller_GetRSSI,     /* Caller GetRSSI */
    TDA18250A_HwStateCaller_SetRawRF,    /* Caller SetRawRF */
    TDA18250A_HwStateCaller_Max
} TDA18250AHwStateCaller_t, *pTDA18250AHwStateCaller_t;

/* TDA18250A specific IRQ clear: */
typedef enum _TDA18250AIRQ_t {
    TDA18250A_IRQ_MSM_RCCal      = 0x01, /* MSM_RCCal bit */
    TDA18250A_IRQ_MSM_IRCAL      = 0x02, /* MSM_IRCAL bit */
    TDA18250A_IRQ_MSM_LOCalc     = 0x08, /* MSM_LOCalc bit */
    TDA18250A_IRQ_MSM_RSSI       = 0x10, /* MSM_RSSI bit */
    TDA18250A_IRQ_XtalCal        = 0x20, /* XtalCal bit */
    TDA18250A_IRQ_Global         = 0x80, /* IRQ_status bit */
    TDA18250A_IRQ_HwInit         = TDA18250A_IRQ_Global\
                                    |TDA18250A_IRQ_MSM_IRCAL,
    TDA18250A_IRQ_IrCal          = TDA18250A_IRQ_MSM_IRCAL\
                                    |TDA18250A_IRQ_MSM_LOCalc\
                                    |TDA18250A_IRQ_MSM_RSSI,
    TDA18250A_IRQ_SetRF          = TDA18250A_IRQ_Global\
                                    |TDA18250A_IRQ_MSM_LOCalc,
    TDA18250A_IRQ_GetPowerLevel  = TDA18250A_IRQ_MSM_RSSI
} TDA18250AIRQ_t, *pTDA18250AIRQ_t;

/* TDA18250A specific MSM: */
typedef enum _TDA18250AMSM_t {
    TDA18250A_MSM_Calc_PLL       = 0x01, /* Calc_PLL bit */
    TDA18250A_MSM_RC_Cal         = 0x02, /* RC_Cal bit */
    TDA18250A_MSM_IR_CAL_Wanted  = 0x10, /* IR_CAL_Wanted bit */
    TDA18250A_MSM_IR_Cal_Image   = 0x20, /* IR_Cal_Image bit */
    TDA18250A_MSM_IR_CAL_Loop    = 0x40, /* IR_CAL_Loop bit */
    TDA18250A_MSM_RSSI_Meas      = 0x80, /* RSSI_Meas bit */
    /* Performs all CALs */
    TDA18250A_MSM_HwInit         = TDA18250A_MSM_IR_Cal_Image\
                                    |TDA18250A_MSM_IR_CAL_Wanted\
                                    |TDA18250A_MSM_IR_CAL_Loop,
    /* Performs all IR_CAL */
    TDA18250A_MSM_IrCal          = TDA18250A_MSM_IR_Cal_Image\
                                    |TDA18250A_MSM_IR_CAL_Loop,
    TDA18250A_MSM_SetRF          = TDA18250A_MSM_Calc_PLL,
    TDA18250A_MSM_GetPowerLevel  = TDA18250A_MSM_RSSI_Meas
} TDA18250AMSM_t, *pTDA18250AMSM_t;

/* TDA18250A Standard settings: */
typedef enum _TDA18250ALPF_t {
    TDA18250A_LPF_6MHz = 0,  /* 6MHz LPFc */
    TDA18250A_LPF_7MHz,      /* 7MHz LPFc */
    TDA18250A_LPF_8MHz,      /* 8MHz LPFc */
    TDA18250A_LPF_9MHz,      /* 9MHz LPFc */
    TDA18250A_LPF_10MHz,     /* 10MHz LPFc */
    TDA18250A_LPF_11_2MHz,   /* 11.2MHz LPFc */
    TDA18250A_LPF_1_5MHz,    /* 1.5MHz LPFc */
    TDA18250A_LPF_1_5MHzbis, /* 1.5MHz LPFc */
    TDA18250A_LPF_Max
} TDA18250ALPF_t, *pTDA18250ALPF_t;

typedef enum _TDA18250ALPFOffset_t {
    TDA18250A_LPFOffset_0pc = 0,    /* LPFc 0%   */
    TDA18250A_LPFOffset_plus_4pc = 0x10,   /* LPFc 4%   */
    TDA18250A_LPFOffset_plus_8pc = 0x20,   /* LPFc 8%   */
    TDA18250A_LPFOffset_plus_12pc = 0x30,  /* LPFc 12%  */
    TDA18250A_LPFOffset_0pcbis = 0x01,     /* LPFc 0%   */
    TDA18250A_LPFOffset_min_4pc = 0x11,    /* LPFc -4%  */
    TDA18250A_LPFOffset_min_8pc = 0x21,    /* LPFc -8%  */
    TDA18250A_LPFOffset_min_12pc = 0x31 ,   /* LPFc -12% */
    TDA18250A_LPFOffset_Max
} TDA18250ALPFOffset_t, *pTDA18250ALPFOffset_t;

typedef enum TDA18250ADC_Notch_IF_PPF_t {
    TDA18250A_DC_Notch_IF_PPF_Disabled = 0,  /* IF Notch Disabled */
    TDA18250A_DC_Notch_IF_PPF_Enabled,       /* IF Notch Enabled */
    TDA18250A_DC_Notch_IF_PPF_Max
} TDA18250ADC_Notch_IF_PPF_t, *pTDA18250ADC_Notch_IF_PPF_t;

typedef enum _TDA18250AHPF_t {
    TDA18250A_HPF_Disabled = 0x00,   /* IF HPF disabled */
    TDA18250A_HPF_0_5MHz = 0x01,         /* IF HPF 0.5MHz   */
    TDA18250A_HPF_1MHz = 0x11,           /* IF HPF 1MHz     */
    TDA18250A_HPF_1_6MHz = 0x21,         /* IF HPF 1.6MHz   */
    TDA18250A_HPF_2_3MHz = 0x31,         /* IF HPF 2.3MHz   */
    TDA18250A_HPF_3_5MHz = 0x41,         /* IF HPF 3.5MHz   */
    TDA18250A_HPF_Max
} TDA18250AHPF_t, *pTDA18250AHPF_t;

typedef enum _TDA18250AHPFOffset_t {
    TDA18250A_HPFOffset_0pc = 0,    /* LPFc 0%   */
    TDA18250A_HPFOffset_plus_4pc = 0x10,   /* LPFc 4%   */
    TDA18250A_HPFOffset_plus_8pc = 0x20,   /* LPFc 8%   */
    TDA18250A_HPFOffset_plus_12pc = 0x30,  /* LPFc 12%  */
    TDA18250A_HPFOffset_0pcbis = 0x01,     /* LPFc 0%   */
    TDA18250A_HPFOffset_min_4pc = 0x11,    /* LPFc -4%  */
    TDA18250A_HPFOffset_min_8pc = 0x21,    /* LPFc -8%  */
    TDA18250A_HPFOffset_min_12pc = 0x31,   /* LPFc -12% */
    TDA18250A_HPFOffset_Max
} TDA18250AHPFOffset_t, *pTDA18250AHPFOffset_t;

typedef enum _TDA18250AIF_Notch_Freq_t {
    TDA18250A_IF_Notch_Freq_6_25MHz = 0,     /* IF Notch Freq 6.25MHz   */
    TDA18250A_IF_Notch_Freq_7_25MHz,         /* IF Notch Freq 7.25MHz   */
    TDA18250A_IF_Notch_Freq_8_25MHz,         /* IF Notch Freq 8.25MHz   */
    TDA18250A_IF_Notch_Freq_9_25MHz,         /* IF Notch Freq 9.25MHz   */
    TDA18250A_IF_Notch_Freq_10_25MHz,        /* IF Notch Freq 10.25MHz  */
    TDA18250A_IF_Notch_Freq_11_45MHz,        /* IF Notch Freq 11.45MHz  */
    TDA18250A_IF_Notch_Freq_Max
} TDA18250AIF_Notch_Freq_t, *pTDA18250AIF_Notch_Freq_t;

typedef enum _TDA18250AIF_Notch_Offset_t {
    TDA18250A_IF_Notch_Offset_0pc = 0,    /* IF Notch Offset 0%   */
    TDA18250A_IF_Notch_Offset_plus_4pc = 0x10,   /* IF Notch Offset 4%   */
    TDA18250A_IF_Notch_Offset_plus_8pc = 0x20,   /* IF Notch Offset 8%   */
    TDA18250A_IF_Notch_Offset_plus_12pc = 0x30,  /* IF Notch Offset 12%  */
    TDA18250A_IF_Notch_Offset_0pcbis = 0x01,     /* IF Notch Offset 0%   */
    TDA18250A_IF_Notch_Offset_min_4pc = 0x11,    /* IF Notch Offset -4%  */
    TDA18250A_IF_Notch_Offset_min_8pc = 0x21,    /* IF Notch Offset -8%  */
    TDA18250A_IF_Notch_Offset_min_12pc = 0x31 ,  /* IF Notch Offset -12% */
    TDA18250A_IF_Notch_Offset_Max
} TDA18250AIF_Notch_Offset_t, *pTDA18250AIF_Notch_Offset_t;

typedef enum _TDA18250AIF_Notch_Enable_t {
    TDA18250A_IF_Notch_Disabled = 0, /* IF Notch Disabled */
    TDA18250A_IF_Notch_Enabled,      /* IF Notch Enabled */
    TDA18250A_IF_Notch_Max
} TDA18250AIF_Notch_Enable_t, *pTDA18250AIF_Notch_Enable_t;

typedef enum _TDA18250AIFnotchToRSSI_t {
    TDA18250A_IFnotchToRSSI_Disabled = 0,    /* IFnotchToRSSI Disabled */
    TDA18250A_IFnotchToRSSI_Enabled,         /* IFnotchToRSSI Enabled */
    TDA18250A_IFnotchToRSSI_Max
} TDA18250AIFnotchToRSSI_t, *pTDA18250AIFnotchToRSSI_t;

typedef enum _TDA18250AAGC1_TOP_I2C_DN_UP_t {
    TDA18250A_AGC1_TOP_I2C_DN_UP_d87_u81dBuV = 0,    /* AGC1 TOP I2C DN/UP down 87 up 81 dBuV */
    TDA18250A_AGC1_TOP_I2C_DN_UP_d90_u83dBuV,        /* AGC1 TOP I2C DN/UP down 90 up 83 dBuV */
    TDA18250A_AGC1_TOP_I2C_DN_UP_d92_u86wdBuV,       /* AGC1 TOP I2C DN/UP down 92 up 86 dBuV */
    TDA18250A_AGC1_TOP_I2C_DN_UP_d94_u88dBuV,        /* AGC1 TOP I2C DN/UP down 94 up 88 dBuV */
    TDA18250A_AGC1_TOP_I2C_DN_UP_d97_u91dBuV,        /* AGC1 TOP I2C DN/UP down 97 up 91 dBuV */
    TDA18250A_AGC1_TOP_I2C_DN_UP_d99_u93dBuV,        /* AGC1 TOP I2C DN/UP down 99 up 93 dBuV */
    TDA18250A_AGC1_TOP_I2C_DN_UP_d102_u96dBuV,        /* AGC1 TOP I2C DN/UP down 102 up 96 dBuV */
    TDA18250A_AGC1_TOP_I2C_DN_UP_d102_u96dBuV_bis,        /* AGC1 TOP I2C DN/UP down 102 up 96 dBuV */
    TDA18250A_AGC1_TOP_I2C_DN_UP_Max
} TDA18250AAGC1_TOP_I2C_DN_UP_t, *pTDA18250AAGC1_TOP_I2C_DN_UP_t;

typedef enum _TDA18250AAGC1_GAIN_t {
    TDA18250AAGC1_GAIN_Free = 0,                    /* AGC1 Gain Free */
    TDA18250AAGC1_GAIN_Minus_11dB,                  /* AGC1 Gain -11dB */
    TDA18250AAGC1_GAIN_Minus_8dB,                   /* AGC1 Gain -8dB */
    TDA18250AAGC1_GAIN_Minus_5dB,                   /* AGC1 Gain -5dB */
    TDA18250AAGC1_GAIN_Minus_2dB,                   /* AGC1 Gain -2dB */
    TDA18250AAGC1_GAIN_Plus_1dB,                    /* AGC1 Gain 1dB */
    TDA18250AAGC1_GAIN_Plus_4dB,                    /* AGC1 Gain 4dB */
    TDA18250AAGC1_GAIN_Plus_7dB,                    /* AGC1 Gain 7dB */
    TDA18250AAGC1_GAIN_Plus_10dB,                   /* AGC1 Gain 10dB */
    TDA18250AAGC1_GAIN_Plus_13dB,                   /* AGC1 Gain 13dB */
    TDA18250AAGC1_GAIN_Plus_16dB,                   /* AGC1 Gain 16dB */
    TDA18250AAGC1_GAIN_Plus_19dB,                   /* AGC1 Gain 19dB */
    TDA18250AAGC1_GAIN_Plus_22dB,                   /* AGC1 Gain 22dB */
    TDA18250AAGC1_GAIN_Free_Frozen,                /* Free at lock, then Frozen */
    TDA18250AAGC1_GAIN_Max
} TDA18250AAGC1_GAIN_t, *pTDA18250AAGC1_GAIN_t;

typedef enum _TDA18250AAGC1_GAIN_SMOOTH_ALGO_t {
    TDA18250AAGC1_GAIN_SMOOTH_ALGO_Enabled = 0,                   /* AGC1 Gain Smooth Enabled */
    TDA18250AAGC1_GAIN_SMOOTH_ALGO_Disabled,                      /* AGC1 Gain Smooth Disabled */
    TDA18250AAGC1_GAIN_SMOOTH_ALGO_Max
} TDA18250AAGC1_GAIN_SMOOTH_ALGO_t, *pTDA18250AAGC1_GAIN_SMOOTH_ALGO_t;

typedef enum _TDA18250AAGC1_TOP_STRATEGY_t {
    TDA18250A_AGC1_TOP_STRATEGY_0 = 0,               /* AGC1 TOP STRATEGY 0 */
    TDA18250A_AGC1_TOP_STRATEGY_1,                   /* AGC1 TOP STRATEGY 1 */
    TDA18250A_AGC1_TOP_STRATEGY_2,                   /* AGC1 TOP STRATEGY 2 */
    TDA18250A_AGC1_TOP_STRATEGY_3,                   /* AGC1 TOP STRATEGY 3 */
    TDA18250A_AGC1_TOP_STRATEGY_4,                   /* AGC1 TOP STRATEGY 4 */
    TDA18250A_AGC1_TOP_STRATEGY_5,                   /* AGC1 TOP STRATEGY 5 */
    TDA18250A_AGC1_TOP_STRATEGY_6,                   /* AGC1 TOP STRATEGY 6 */
    TDA18250A_AGC1_TOP_STRATEGY_7,                   /* AGC1 TOP STRATEGY 7 */
    TDA18250A_AGC1_TOP_STRATEGY_Max
} TDA18250AAGC1_TOP_STRATEGY_t, *pTDA18250AAGC1_TOP_STRATEGY_t;

typedef enum _TDA18250AAGC1_SMOOTH_T_CST_t {
    TDA18250A_AGC1_SMOOTH_T_CST_Instantaneous = 0,      /* AGC1 Smooth T Cst Instantaneous */
    TDA18250A_AGC1_SMOOTH_T_CST_1_6ms,                  /* AGC1 Smooth T Cst 1.6ms */
    TDA18250A_AGC1_SMOOTH_T_CST_3_2ms,                  /* AGC1 Smooth T Cst 3.1ms */
    TDA18250A_AGC1_SMOOTH_T_CST_6_4ms,                  /* AGC1 Smooth T Cst 6.4ms */
    TDA18250A_AGC1_SMOOTH_T_CST_12_8ms,                 /* AGC1 Smooth T Cst 12.8ms */
    TDA18250A_AGC1_SMOOTH_T_CST_25_6ms,                 /* AGC1 Smooth T Cst 25.6ms */
    TDA18250A_AGC1_SMOOTH_T_CST_51_2ms,                 /* AGC1 Smooth T Cst 51.2ms */
    TDA18250A_AGC1_SMOOTH_T_CST_320ms,                  /* AGC1 Smooth T Cst 320ms */
    TDA18250A_AGC1_SMOOTH_T_CST_Max
} TDA18250AAGC1_SMOOTH_T_CST_t, *pTDA18250AAGC1_SMOOTH_T_CST_t;

typedef enum _TDA18250AAGC1_DET_SPEED_t {
    TDA18250A_AGC1_DET_SPEED_62_5KHz = 0,               /* 62.5KHz */
    TDA18250A_AGC1_DET_SPEED_125KHz,               /* 125KHz */
    TDA18250A_AGC1_DET_SPEED_Max
} TDA18250AAGC1_DET_SPEED_t, *pTDA18250AAGC1_DET_SPEED_t;

typedef enum _TDA18250ALNA_RANGE_t {
    TDA18250A_LNA_RANGE_Minus11dB_Plus22dB = 0,      /* LNA RANGE -11dB 22dB */
    TDA18250A_LNA_RANGE_Minus8dB_Plus22dB,           /* LNA RANGE -8dB 22dB  */
    TDA18250A_LNA_RANGE_Plus7dB_Plus22dB,            /* LNA RANGE 7dB 22dB   */
    TDA18250A_LNA_RANGE_Plus7dB_Plus22dBbis,         /* LNA RANGE 7dB 22dB   */
    TDA18250A_LNA_RANGE_Max
} TDA18250ALNA_RANGE_t, *pTDA18250ALNA_RANGE_t;

typedef enum _TDA18250ALNA_ZIN_t {
    TDA18250A_LNA_ZIN_NF = 0,      /* better NF for 7,10,13dB agc1 gain */
    TDA18250A_LNA_ZIN_S11,           /* better S11 for 7,10,13dB agc1 gain  */
    TDA18250A_LNA_ZIN_Max
} TDA18250ALNA_ZIN_t, *pTDA18250ALNA_ZIN_t;

typedef enum _TDA18250AAGC2_DET_SPEED_t {
    TDA18250A_AGC2_DET_SPEED_62_5KHz = 0,               /* 62.5KHz */
    TDA18250A_AGC2_DET_SPEED_125KHz,               /* 125KHz */
    TDA18250A_AGC2_DET_SPEED_Max
} TDA18250AAGC2_DET_SPEED_t, *pTDA18250AAGC2_DET_SPEED_t;

typedef enum _TDA18250ADET12_CINT_t {
    TDA18250A_DET12_CINT_50fF = 0,               /* 50fF */
    TDA18250A_DET12_CINT_100fF,               /* 100fF */
    TDA18250A_DET12_CINT_150fF,               /* 150fF */
    TDA18250A_DET12_CINT_200fF,               /* 200fF */
    TDA18250A_DET12_CINT_Max
} TDA18250ADET12_CINT_t, *pTDA18250ADET12_CINT_t;

typedef enum _TDA18250AIRQ_POLARITY_t {
    TDA18250A_IRQ_POLARITY_LOW = 0,      /* pin goes low when IRQ */
    TDA18250A_IRQ_POLARITY_HIGH,           /* pin goes high when IRQ  */
    TDA18250A_IRQ_POLARITY_Max
} TDA18250AIRQ_POLARITY_t, *pTDA18250AIRQ_POLARITY_t;

typedef enum _TDA18250AAGC3_HP_FC_t {
    TDA18250A_AGC3_HP_FC_0_3M = 0,               /* RSSI_HP_FC 0.3MHz  */
    TDA18250A_AGC3_HP_FC_2_25M,                  /* RSSI_HP_FC 2.25MHz */
    TDA18250A_AGC3_HP_FC_5M,                     /* RSSI_HP_FC 5MHz    */
    TDA18250A_AGC3_HP_FC_5Mbis,                  /* RSSI_HP_FC 5MHz    */
    TDA18250A_AGC3_HP_FC_Max 
} TDA18250AAGC3_HP_FC_t, *pTDA18250AAGC3_HP_FC_t;

typedef enum _TDA18250AAGC3_GAIN_t {
    TDA18250AAGC3_GAIN_Free = 0,                    /* AGC3 Gain Free */
    TDA18250AAGC3_GAIN_Minus_6dB,                   /* AGC3 Gain -6dB */
    TDA18250AAGC3_GAIN_Minus_3dB,                   /* AGC3 Gain -3dB */
    TDA18250AAGC3_GAIN_0dB,                         /* AGC3 Gain 0dB */
    TDA18250AAGC3_GAIN_Plus_3dB,                    /* AGC3 Gain 3dB */
    TDA18250AAGC3_GAIN_Plus_6dB,                    /* AGC3 Gain 6dB */
    TDA18250AAGC3_GAIN_Plus_9dB,                    /* AGC3 Gain 9dB */
    TDA18250AAGC3_GAIN_Plus_12dB,                   /* AGC3 Gain 12dB */
    TDA18250AAGC3_GAIN_Plus_15dB,                   /* AGC3 Gain 15dB */
    TDA18250AAGC3_GAIN_Max
} TDA18250AAGC3_GAIN_t, *pTDA18250AAGC3_GAIN_t;

typedef enum _TDA18250ARSSI_HP_FC_t {
    TDA18250A_RSSI_HP_FC_0_3M = 0,               /* RSSI_HP_FC 0.3MHz  */
    TDA18250A_RSSI_HP_FC_2_25M,                  /* RSSI_HP_FC 2.25MHz */
    TDA18250A_RSSI_HP_FC_5M,                     /* RSSI_HP_FC 5MHz    */
    TDA18250A_RSSI_HP_FC_5Mbis,                  /* RSSI_HP_FC 5MHz    */
    TDA18250A_RSSI_HP_FC_Max
} TDA18250ARSSI_HP_FC_t, *pTDA18250ARSSI_HP_FC_t;

typedef enum _TDA18250AS2D_Gain_t {
    TDA18250A_S2D_Gain_3dB = 0,  /* 3dB */
    TDA18250A_S2D_Gain_6dB,      /* 6dB */
    TDA18250A_S2D_Gain_9dB,      /* 9dB */
    TDA18250A_S2D_Gain_Max
} TDA18250AS2D_Gain_t, *pTDA18250AS2D_Gain_t;

typedef enum _TDA18250AAGCK_Time_Constant_t {
    TDA18250A_AGCK_Time_Constant_1_STEP_EACH_VSYNC_RISING_EDGE = 0,  /* 1 Step Each VSYNC Rising Edge */
    TDA18250A_AGCK_Time_Constant_0_512ms,                            /* 0.512ms                       */
    TDA18250A_AGCK_Time_Constant_8_192ms,                            /* 8.192ms                       */
    TDA18250A_AGCK_Time_Constant_32_768ms,                           /* 32.768ms                      */
    TDA18250A_AGCK_Time_Constant_Max
} TDA18250AAGCK_Time_Constant_t, *pTDA18250AAGCK_Time_Constant_t;

typedef enum _TDA18250AVHF_III_Mode_t {
    TDA18250A_VHF_III_Mode_Disabled = 0, /* VHF_III_Mode Disabled */
    TDA18250A_VHF_III_Mode_Enabled,      /* VHF_III_Mode Enabled  */
    TDA18250A_VHF_III_Mode_Max
} TDA18250AVHF_III_Mode_t, *pTDA18250AVHF_III_Mode_t;

typedef enum _TDA18250ARSSI_CAP_VAL_t {
    TDA18250A_RSSI_CAP_VAL_0_5pF = 0, /* 0.5pF */
    TDA18250A_RSSI_CAP_VAL_3pF,      /* 3pF */
    TDA18250A_RSSI_CAP_VAL_Max
} TDA18250ARSSI_CAP_VAL_t, *pTDA18250ARSSI_CAP_VAL_t;

typedef enum _TDA18250ARSSI_CK_SPEED_t {
    TDA18250A_RSSI_CK_SPEED_31_25kHz = 0, /* 31.25KHz */
    TDA18250A_RSSI_CK_SPEED_62_5kHz,      /* 62.5KHz  */
    TDA18250A_RSSI_CK_SPEED_Max
} TDA18250ARSSI_CK_SPEED_t, *pTTDA18250ARSSI_CK_SPEED_t;

typedef enum _TDA18250AXTOutAmp_t {
    TDA18250A_XTOUT_Amp_400mV = 1,
    TDA18250A_XTOUT_Amp_800mV = 3,
    TDA18250A_XTOUT_Amp_1200mV = 7,
    TDA18250A_XTOUT_Amp_Max = 8
} TDA18250AXTOutAmp_t, *pTDA18250AXTOutAmp_t;

typedef enum _TDA18250AXTOut_t {
    TDA18250A_XTOUT_Off = 0,
    TDA18250A_XTOUT_FDiv_Pll_2 = 1,
    TDA18250A_XTOUT_DigClock = 2,
    TDA18250A_XTOUT_Sinusoidal = 3,
    TDA18250A_XTOUT_RailRail = 5,
    TDA18250A_XTOUT_Max
} TDA18250AXTOut_t, *pTDA18250AXTOut_t;

/*
typedef enum _TDA18250AAGC2_Do_Step_t {
    TDA18250A_AGC2_Do_Step_8_192ms = 0,
    TDA18250A_AGC2_Do_Step_2_048ms = 1,
    TDA18250A_AGC2_Do_Step_1_048ms = 2,
    TDA18250A_AGC2_Do_Step_0_512ms = 3,
} TDA18250AAGC2_Do_Step_t, *pTDA18250AAGC2_Do_Step_t;
*/
typedef enum _TDA18250AAGC2_Up_Step_t {
    TDA18250A_AGC2_Up_Step_262ms = 0,
    TDA18250A_AGC2_Up_Step_131ms = 1,
    TDA18250A_AGC2_Up_Step_65_5ms = 2,
    TDA18250A_AGC2_Up_Step_23_8 = 3,
} TDA18250AAGC2_Up_Step_t, *pTDA18250AAGC2_Up_Step_t;

typedef enum _TDA18250AAGC2_Up_Udld_Step_t {
    TDA18250A_AGC2_Up_Udld_Step_262ms = 0,
    TDA18250A_AGC2_Up_Udld_Step_131ms = 1,
    TDA18250A_AGC2_Up_Udld_Step_65_5ms = 2,
    TDA18250A_AGC2_Up_Udld_Step_23_8 = 3,
} TDA18250AAGC2_Up_Udld_Step_t, *pTDA18250AAGC2_Up_Udld_Step_t;

typedef enum _TDA18250AAGC_Timer_Mod_Fast_Auto_t {
    TDA18250A_AGC_Timer_Mod_Fast_Auto_00 = 0,
    TDA18250A_AGC_Timer_Mod_Fast_Auto_01 = 1,
    TDA18250A_AGC_Timer_Mod_Fast_Auto_10 = 2,
    TDA18250A_AGC_Timer_Mod_Fast_Auto_11 = 3,
} TDA18250AAGC_Timer_Mod_Fast_Auto_t, *pTDA18250AAGC_Timer_Mod_Fast_Auto_t;

typedef enum _TDA18250AAGC_Type_Fast_Auto_t {
    TDA18250A_AGC_Type_Fast_Auto_00 = 0,
    TDA18250A_AGC_Type_Fast_Auto_01 = 1,
    TDA18250A_AGC_Type_Fast_Auto_10 = 2,
    TDA18250A_AGC_Type_Fast_Auto_11 = 3,
} TDA18250AAGC_Type_Fast_Auto_t, *pTDA18250AAGC_Type_Fast_Auto_t;

typedef enum _TDA18250AAGC1_Do_Step_t {
    TDA18250A_AGC1_Do_Step_8_182ms = 0,
    TDA18250A_AGC1_Do_Step_2_048ms = 1,
    TDA18250A_AGC1_Do_Step_1_024ms = 2,
    TDA18250A_AGC1_Do_Step_0_512ms = 3,
} TDA18250AAGC1_Do_Step_t, *pTDA18250AAGC1_Do_Step_t;

typedef enum _TDA18250AAGC2_Do_Step_t {
    TDA18250A_AGC2_Do_Step_8_182ms = 0,
    TDA18250A_AGC2_Do_Step_2_048ms = 1,
    TDA18250A_AGC2_Do_Step_1_024ms = 2,
    TDA18250A_AGC2_Do_Step_0_512ms = 3,
} TDA18250AAGC2_Do_Step_t, *pTDA18250AAGC2_Do_Step_t;

typedef struct _TDA18250AStdAGC1TOP
{
    UInt32 uFreq;
    TDA18250AAGC1_TOP_I2C_DN_UP_t uTOPDnUp;
} TDA18250AStdAGC1TOP;

typedef struct _TDA18250AStdAGC2TOP
{
    UInt32 uFreq;
    UInt8 uTOPDn;
    UInt8 uTOPUp;
} TDA18250AStdAGC2TOP;

typedef struct _TDA18250AStdS2DGain
{
    UInt32 uFreq;
    TDA18250AS2D_Gain_t eS2DGain;
} TDA18250AStdS2DGain;

typedef enum _TDA18250A_Coeff_X_t
{
    TDA18250A_Coeff_X0 = 0,
    TDA18250A_Coeff_X1 = 1,
    TDA18250A_Coeff_X2 = 2,
    TDA18250A_Coeff_X3 = 3,
	TDA18250A_Coeff_X_Code = 4,
    TDA18250A_Coeff_X_Max = 5
} TDA18250A_Coeff_X_t, *pTDA18250A_Coeff_X_t;


typedef enum _TDA18250A_Coeff_Y_t
{
    TDA18250A_Coeff_Y0 = 0,
    TDA18250A_Coeff_Y1 = 1,
    TDA18250A_Coeff_Y2 = 2,
    TDA18250A_Coeff_Y_Code = 3,
    TDA18250A_Coeff_Y_Max = 4
} TDA18250A_Coeff_Y_t, *pTDA18250A_Coeff_Y_t;

typedef struct _TDA18250AStdCoefficients
{
    /****************************************************************/
    /* IF Settings                                                  */
    /****************************************************************/
    UInt32                                      IF;                          /* IF Frequency */
    Int32                                      CF_Offset;                   /* CF Offset */

    /****************************************************************/
    /* IF SELECTIVITY Settings                                      */
    /****************************************************************/
    TDA18250ALPF_t                               LPF;                        /* LPF Cut off */
    TDA18250ALPFOffset_t                         LPF_Offset;                 /* LPF offset */
    TDA18250ADC_Notch_IF_PPF_t                   DC_Notch_IF_PPF;            /* DC notch IF PPF */
    TDA18250AHPF_t                               HPF;                        /* Hi Pass */
    TDA18250AHPFOffset_t                         HPF_Offset;                 /* HPF offset */
    TDA18250AIF_Notch_Enable_t                   IF_Notch_Enable;            /* IF notch enable */
    TDA18250AIF_Notch_Freq_t                     IF_Notch_Freq;              /* IF Notch Freq */
    TDA18250AIF_Notch_Offset_t                   IF_Notch_Offset;            /* IF Notch Offset */
    TDA18250AIFnotchToRSSI_t                     IFnotchToRSSI;              /* IFnotchToRSSI */


    /****************************************************************/
    /* AGC Settings                                                 */
    /****************************************************************/
    TDA18250AAGC1_GAIN_t                         AGC1_GAIN;                  /* AGC1 GAIN */
    TDA18250AAGC1_GAIN_SMOOTH_ALGO_t             AGC1_GAIN_SMOOTH_ALGO;      /* AGC1 GAIN SMOOTH ALGO */
    TDA18250AStdAGC1TOP                          AGC1_TOP_ES1[TDA18250A_CONFIG_STD_FREQ_NUM]; /* AGC1 TOP DN/UP ES1 */
    TDA18250AStdAGC1TOP                          AGC1_TOP_ES2[TDA18250A_CONFIG_STD_FREQ_NUM]; /* AGC1 TOP DN/UP ES2 */
    TDA18250AStdAGC1TOP                          AGC1_TOP_ES3[TDA18250A_CONFIG_STD_FREQ_NUM]; /* AGC1 TOP DN/UP ES3 */
    TDA18250AAGC1_TOP_STRATEGY_t                 AGC1_TOP_STRATEGY;          /* AGC1 TOP STRATEGY */
    TDA18250AAGC1_DET_SPEED_t                    AGC1_DET_SPEED;             /* AGC1 DET SPEED */
    TDA18250AAGC1_SMOOTH_T_CST_t                 AGC1_Smooth_T_Cst;          /* AGC1 Smooth T Cst */
    TDA18250AAGC1_Do_Step_t                      AGC1_Do_Step;               /* AGC1 Do Step */
    TDA18250ALNA_ZIN_t							 LNA_Zin;
    Bool                                         AGC2_GAIN_CONTROL_EN_ES1;   /* AGC2 Gain Control En ES1 */
    Bool                                         AGC2_GAIN_CONTROL_EN_ES2;   /* AGC2 Gain Control En ES2 */
    Bool                                         AGC2_GAIN_CONTROL_EN_ES3;   /* AGC2 Gain Control En ES3 */
    TDA18250AStdAGC2TOP                          AGC2_TOP_ES1[TDA18250A_CONFIG_STD_FREQ_NUM]; /* AGC2 TOP ES1 */
    TDA18250AStdAGC2TOP                          AGC2_TOP_ES2[TDA18250A_CONFIG_STD_FREQ_NUM]; /* AGC2 TOP ES2 */
    TDA18250AStdAGC2TOP                          AGC2_TOP_ES3[TDA18250A_CONFIG_STD_FREQ_NUM]; /* AGC2 TOP ES3 */
    TDA18250AAGC2_DET_SPEED_t                    AGC2_Det_Speed;             /* AGC2 Det Speed */
    Bool                                         AGC2_Adapt_TOP23_Enable_ES2;    /* AGC2 Adapt TOP23_Enable ES2 */
    UInt8                                        AGC2_Adapt_TOP23_Delta_ES2;     /* AGC2 Adapt TOP23 Delta (in dB) ES2 */
    Bool                                         AGC2_Adapt_TOP23_Enable_ES3;    /* AGC2 Adapt TOP23_Enable  ES2*/
    UInt8                                        AGC2_Adapt_TOP23_Delta_ES3;     /* AGC2 Adapt TOP23 Delta (in dB)  ES2 */    
    Bool                                         AGC2_Gain_Control_Speed;    /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */
    TDA18250AAGC2_Do_Step_t                      AGC2_Do_Step;               /* AGC2 Do Step */
    TDA18250AAGC2_Up_Step_t                      AGC2_Up_Step;               /* AGC2 Up Step */
    TDA18250AAGC2_Up_Udld_Step_t                 AGC2_Up_Udld_Step;          /* AGC2 Ip Udld Step */
    Int8                                         AGC2_TOP_Up_Udld;           /* AGC2 TOP Up Udld */
    UInt8                                        AGC2_Fast_Auto_Delta;       /* AGC2 Fast Auto Delta */
    TDA18250ADET12_CINT_t                        DET12_CINT;                 /* DET12 CINT */
    TDA18250AStdAGC2TOP                           AGC3_TOP[TDA18250A_CONFIG_STD_FREQ_NUM]; /* AGC3 TOP */
    TDA18250AIF_Output_Level_t                   IF_Output_Level;            /* IF Output Level */
    TDA18250AStdS2DGain                          S2D_Gain[TDA18250A_CONFIG_STD_FREQ_NUM]; /* S2D gain */
    Bool                                         Fast_Auto_Launch;           /* Fast Auto Launch */
    TDA18250AAGC_Timer_Mod_Fast_Auto_t           AGC_Timer_Mod_Fast_Auto;    /* AGC Timer Mod Fast Auto */
    Bool                                         AGC2_Up_Udld_Fast_Reduce;   /* AGC2 Up Udld Fast Reduce */
    Bool                                         AGC2_Up_Fast_Reduce;        /* AGC2 Up Fast Reduce */
    TDA18250AAGC_Type_Fast_Auto_t                AGC_Type_Fast_Auto;         /* AGC Type Fast Auto */
	
    /****************************************************************/
    /* GSK Settings                                                 */
    /****************************************************************/
    TDA18250AAGCK_Time_Constant_t                AGCK_Time_Constant;         /* AGCK Time Constant */
    TDA18250ARSSI_HP_FC_t                        RSSI_HP_FC;                 /* RSSI HP FC */
    
    /****************************************************************/
    /* H3H5 Settings                                                */
    /****************************************************************/
    TDA18250AVHF_III_Mode_t                      VHF_III_Mode;               /* VHF_III_Mode */

    /****************************************************************/
    /*RSSI Settings                                                 */
    /****************************************************************/
	TDA18250ARSSI_CAP_VAL_t                      RSSI_Cap_Val;				 /* RSSI Cap Val */
    TDA18250ARSSI_CK_SPEED_t                     RSSI_Ck_Speed;				 /* RSSI Ck Speed */

	/****************************************************************/
    /*PSM Settings                                                  */
    /****************************************************************/
    UInt8                                        uES1PSMByte1;               /* ES1 Power Saving Byte 1 */
    UInt8                                        uES1PSMByte2;               /* ES1 Power Saving Byte 2 */
    UInt8                                        uES1PSMByte3;               /* ES1 Power Saving Byte 3 */
    UInt8                                        uES2PSMByte1;               /* ES2 Power Saving Byte 1 */
    UInt8                                        uES2PSMByte2;               /* ES2 Power Saving Byte 2 */
    UInt8                                        uES2PSMByte3;               /* ES2 Power Saving Byte 3 */
    UInt8                                        uES3PSMByte1;               /* ES3 Power Saving Byte 1 */
    UInt8                                        uES3PSMByte2;               /* ES3 Power Saving Byte 2 */
    UInt8                                        uES3PSMByte3;               /* ES3 Power Saving Byte 3 */

} TDA18250AStdCoefficients, *pTDA18250AStdCoefficients;

typedef struct _TDA18250ASmoothCurrentStateSave_t
{
    UInt8 uAGC2LoopOff;
    UInt8 uAGC2;
    UInt8 uDigClock;
    UInt8 uPSMRFPoly;
    UInt8 uPSMIfPoly;
    UInt8 uPSMMixer;
    UInt8 uPSMLodriver;
    UInt8 uAGC1Boost;
    UInt8 uPSMHpfIfn;
    UInt8 uPSMLpf;
    UInt8 uPSMDCC;
    UInt8 uPSMH3H5;
    UInt8 uPSMVCO;
    UInt8 uPDDet1;
    UInt8 uPDDet2;
    UInt8 uPDDet3;
    UInt8 uPDVCO;
    UInt8 uPDSynthe;
    UInt8 uIFHPEnable;
    UInt8 uIFNotchEnable;
    UInt8 uDCNotch;
    UInt8 uGPIO1;
    UInt8 uGPIO2;
    UInt8 uWFilterEnable;
    UInt8 uXtOut;
    UInt8 uPDLNA;
    UInt8 uPDLT;
    UInt8 uPDS2BIF;
} TDA18250ASmoothCurrentStateSave_t;

typedef struct _TDA18250AObject_t
{
    tmUnitSelect_t                  tUnit;
    tmUnitSelect_t                  tUnitW;
    ptmbslFrontEndMutexHandle       pMutex;
    Bool                            init;
    tmbslFrontEndIoFunc_t           sIo;
    tmbslFrontEndTimeFunc_t         sTime;
    tmbslFrontEndDebugFunc_t        sDebug;
    tmbslFrontEndMutexFunc_t        sMutex;
    GetMasterContributionPointer    getMasterContribution;
    TDA18250AVersion_t              eChipVersion;
    Bool                            bMasterNotSlave;
    tmUnitSelect_t                  tOtherUnit;
    TDA18250ASmoothCurrentStateSave_t sSmoothCurrentStateSave;
    TDA18250AScanning_Mode_t        eScanningMode;
    /* Device specific part: */
    tmPowerState_t                  curPowerState;
    TDA18250APowerState_t            curLLPowerState;
    TDA18250APowerState_t            mapLLPowerState[tmPowerMax];
    UInt32                          uRF;
    UInt32                          uProgRF;
    TDA18250AStandardMode_t          StandardMode;
    pTDA18250AStdCoefficients        pStandard;
    Bool                            bBufferMode;        /* Indicates if bufferMode is enabled on the tuner */
    Bool                            bOverridePLL;       /* Indicates if PLL is put into manual mode during setRF */
	TDA18250ALoopThrough_t          eLoopThrough;     /* Digital only or Hybrid , used in Dual or Single */
	TDA18250AModeInUse_t             TunerModeInUse;     /* Digital only or Hybrid , used in Dual or Single */
    /* Settings independant of standard */
	TDA18250AXtalFreq_t				 eXtalFreq;
    TDA18250AXTOutAmp_t              eXTOutAmp;             /* Current XTOUT Amplitude */
    TDA18250AXTOut_t                 eXTOut;                /* XTOut Type */
	TDA18250ALNA_RANGE_t             eLNA_Range;            /* LNA_RANGE */
    TDA18250AIRQ_POLARITY_t          eIRQPolarity;             /* IRQ Polarity */
    TDA18250AHwState_t               eHwState;                /* Indicates HwInit state */
    /* Settings dependant of standard */
    TDA18250AStdCoefficients         Std_Array[TDA18250A_StandardMode_Max-1];
#ifdef _TDA18250A_REGMAP_BITFIELD_DEFINED
    TDA18250A_Reg_Map_t              RegMap;
#else
    UInt8                           RegMap[TDA18250A_REG_MAP_NB_BYTES];
#endif
} TDA18250AObject_t, *pTDA18250AObject_t, **ppTDA18250AObject_t;



/*============================================================================*/
/* Internal Prototypes:                                                       */
/*============================================================================*/
extern tmErrorCode_t
iTDA18250A_CheckHwState(pTDA18250AObject_t pObj, TDA18250AHwStateCaller_t caller);

extern tmErrorCode_t
iTDA18250A_SetRF(pTDA18250AObject_t pObj);

extern tmErrorCode_t
iTDA18250A_SetFreqDependantStandard(pTDA18250AObject_t pObj, UInt32 uRF);

extern tmErrorCode_t
iTDA18250A_SetMSM(pTDA18250AObject_t pObj, UInt8 uValue, Bool bLaunch);

extern tmErrorCode_t
iTDA18250A_WaitIRQ(pTDA18250AObject_t pObj, UInt32 timeOut, UInt32 waitStep, UInt8 irqStatus);

extern tmErrorCode_t
iTDA18250A_CalculateNIntKInt(UInt32 LO, UInt8 PostDiv, UInt8 Prescaler, UInt32* NInt, UInt32* KInt, UInt32 uXtalFreqId);

extern tmErrorCode_t
iTDA18250A_CalculatePostDivAndPrescalerBetterMargin(UInt32 LO, Bool growingOrder, UInt8* PostDiv, UInt8* Prescaler);

extern tmErrorCode_t
iTDA18250A_SetLLPowerState(pTDA18250AObject_t pObj, TDA18250APowerState_t powerState);

extern tmErrorCode_t
iTDA18250A_PowerSavingMode(pTDA18250AObject_t pObj/*, TDA18250APowerSavingMode_t PowerSavingMode*/);

extern tmErrorCode_t
iTDA18250A_Write(pTDA18250AObject_t pObj, const TDA18250A_BitField_t* pBitField, UInt8 uData, tmbslFrontEndBusAccess_t eBusAccess);

extern tmErrorCode_t
iTDA18250A_Read(pTDA18250AObject_t pObj, const TDA18250A_BitField_t* pBitField, UInt8* puData, tmbslFrontEndBusAccess_t eBusAccess);

extern tmErrorCode_t
iTDA18250A_WriteRegMap(pTDA18250AObject_t pObj, UInt8 uAddress, UInt32 uWriteLen);

extern tmErrorCode_t
iTDA18250A_ReadRegMap(pTDA18250AObject_t pObj, UInt8 uAddress, UInt32 uReadLen);

extern tmErrorCode_t
iTDA18250A_Wait(pTDA18250AObject_t pObj, UInt32 Time);

extern tmErrorCode_t
iTDA18250A_AllocInstance(tmUnitSelect_t tUnit, ppTDA18250AObject_t ppDrvObject);

extern tmErrorCode_t
iTDA18250A_DeAllocInstance(pTDA18250AObject_t pDrvObject);

extern tmErrorCode_t
iTDA18250A_GetInstance(tmUnitSelect_t tUnit, ppTDA18250AObject_t ppDrvObject);

extern tmErrorCode_t
iTDA18250A_ResetInstance(pTDA18250AObject_t pDrvObject);

extern tmErrorCode_t
iTDA18250A_SetNDivRDiv(pTDA18250AObject_t pObj);

extern tmErrorCode_t
iTDA18250A_GetRSSI(pTDA18250AObject_t pObj, UInt8* puValue);

extern tmErrorCode_t
iTDA18250A_LvlAgc1NBMasterContribution(pTDA18250AObject_t pObj, UInt32 uRF, Int32* pDeltaGain);

#ifdef _TVFE_IMPLEMENT_MUTEX
 extern tmErrorCode_t iTDA18250A_MutexAcquire(pTDA18250AObject_t pObj, UInt32 timeOut);
 extern tmErrorCode_t iTDA18250A_MutexRelease(pTDA18250AObject_t pObj);
#endif




#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA18250A_LOCAL_H */

