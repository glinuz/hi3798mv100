/*
  Copyright (C) 2011 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18204_RegDef.c
 *
 * \date          %modify_time%
 *
 * \author        TVFE SW Group
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18204_Driver_User_Guide.pdf
 *
 * TVFE SW Arch V4 Template
 *
 * \section info Change Information
 *
*/

/* File generated automatically from register description file */


/*============================================================================*/
/* Standard include files:                                                    */
/*============================================================================*/
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"

/*============================================================================*/
/* Project include files:                                                     */
/*============================================================================*/
#include "tmbslTDA18204.h"
#include "tmbslTDA18204_RegDef.h"

/*============================================================================*/
/* Global data:                                                               */
/*============================================================================*/

/* TDA18204 Register ID_1 0x00 */
const TDA18204_BitField_t gTDA18204_Reg_ID_1 = { 0x00, 0x00, 0x08, 0x00 };
/* IDENT_1 bit(s): IDENT_1 & IDENT_2 = 18204 for TDA18204 */
const TDA18204_BitField_t gTDA18204_Reg_ID_1__IDENT_1 = { 0x00, 0x00, 0x07, 0x00 };


/* TDA18204 Register ID_2 0x01 */
const TDA18204_BitField_t gTDA18204_Reg_ID_2 = { 0x01, 0x00, 0x08, 0x00 };
/* IDENT_2 bit(s): IDENT_1 & IDENT_2 = 18204 for TDA18204 */
const TDA18204_BitField_t gTDA18204_Reg_ID_2__IDENT_2 = { 0x01, 0x00, 0x08, 0x00 };


/* TDA18204 Register ID_3 0x02 */
const TDA18204_BitField_t gTDA18204_Reg_ID_3 = { 0x02, 0x00, 0x08, 0x00 };
/* MAJOR_REV bit(s): Metal fixable with ivfix. . 1 --> ES1 */
const TDA18204_BitField_t gTDA18204_Reg_ID_3__MAJOR_REV = { 0x02, 0x04, 0x04, 0x00 };
/* MINOR_REV bit(s): Metal fixable with ivfix. . 0 --> version A. ,B,C,D */
const TDA18204_BitField_t gTDA18204_Reg_ID_3__MINOR_REV = { 0x02, 0x00, 0x04, 0x00 };


/* TDA18204 Register THERMO_1 0x03 */
const TDA18204_BitField_t gTDA18204_Reg_THERMO_1 = { 0x03, 0x00, 0x08, 0x00 };
/* TM_D bit(s): Temperature Display */
const TDA18204_BitField_t gTDA18204_Reg_THERMO_1__TM_D = { 0x03, 0x00, 0x07, 0x00 };


/* TDA18204 Register THERMO_2 0x04 */
const TDA18204_BitField_t gTDA18204_Reg_THERMO_2 = { 0x04, 0x00, 0x08, 0x00 };
/* TM_ON bit(s): 0 : Thermo Off  -  1 : Launch Temperature Measurement */
const TDA18204_BitField_t gTDA18204_Reg_THERMO_2__TM_ON = { 0x04, 0x00, 0x01, 0x00 };


/* TDA18204 Register POWER_STATUS 0x05 */
const TDA18204_BitField_t gTDA18204_Reg_POWER_STATUS = { 0x05, 0x00, 0x08, 0x00 };
/* LO_Lock bit(s): status read only */
const TDA18204_BitField_t gTDA18204_Reg_POWER_STATUS__LO_Lock = { 0x05, 0x07, 0x01, 0x00 };
/* POR bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_STATUS__POR = { 0x05, 0x00, 0x01, 0x00 };


/* TDA18204 Register XTAL_IN_CTRL 0x06 */
const TDA18204_BitField_t gTDA18204_Reg_XTAL_IN_CTRL = { 0x06, 0x00, 0x08, 0x00 };
/* XTAL_IN_CTRL bit(s): .111.: xtal 27MHz. .110.: xtal 16MHz. .101.: buffer in 27MHz. .100.: buffer in 16MHz. others: no xtal, no buffer in */
const TDA18204_BitField_t gTDA18204_Reg_XTAL_IN_CTRL__XTAL_IN_CTRL = { 0x06, 0x00, 0x03, 0x00 };


/* TDA18204 Register XTAL_OUT_CTRL 0x07 */
const TDA18204_BitField_t gTDA18204_Reg_XTAL_OUT_CTRL = { 0x07, 0x00, 0x08, 0x00 };
/* Xtout_Amp bit(s): .000.: 200 mV. . .001.: 400 mV. . .010.: 600 mV. . .011.: 800 mV. . .100.: 600 mV. . .101.: 800 mV. . .110.: 1000 mV. . .111.: 1200 mV */
const TDA18204_BitField_t gTDA18204_Reg_XTAL_OUT_CTRL__Xtout_Amp = { 0x07, 0x03, 0x03, 0x00 };
/* XTout bit(s): select Xtout. . 000 : No signal / 16-27 MHz sinus / 16-27 MHz rail to rail. 001 : Fdiv PLL. . 010 : Fref PLL. . 011 : sublo. . 100 : clock osc. . 101 : Sync */
const TDA18204_BitField_t gTDA18204_Reg_XTAL_OUT_CTRL__XTout = { 0x07, 0x00, 0x03, 0x00 };


/* TDA18204 Register IRQ_status 0x08 */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_status = { 0x08, 0x00, 0x08, 0x00 };
/* IRQ_status bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__IRQ_status = { 0x08, 0x07, 0x01, 0x00 };
/* MSM_error_no_clock_available bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__MSM_error_no_clock_available = { 0x08, 0x06, 0x01, 0x00 };
/* MSM_error_config bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__MSM_error_config = { 0x08, 0x05, 0x01, 0x00 };
/* MSM_LOCalc_End bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__MSM_LOCalc_End = { 0x08, 0x04, 0x01, 0x00 };
/* MSM_RSSI_End bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__MSM_RSSI_End = { 0x08, 0x03, 0x01, 0x00 };
/* MSM_IRCAL_End bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__MSM_IRCAL_End = { 0x08, 0x02, 0x01, 0x00 };
/* MSM_RCCal_End bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__MSM_RCCal_End = { 0x08, 0x01, 0x01, 0x00 };
/* XtalCal_End bit(s): read value: . . .0. : cal not done. . .1. : cal done */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__XtalCal_End = { 0x08, 0x00, 0x01, 0x00 };


/* TDA18204 Register IRQ_enable 0x09 */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable = { 0x09, 0x00, 0x08, 0x00 };
/* IRQ_Enable bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__IRQ_Enable = { 0x09, 0x07, 0x01, 0x00 };
/* MSM_error_no_clock_available_Enable bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__MSM_error_no_clock_available_Enable = { 0x09, 0x06, 0x01, 0x00 };
/* MSM_error_config_Enable bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__MSM_error_config_Enable = { 0x09, 0x05, 0x01, 0x00 };
/* MSM_LOCalc_Enable bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__MSM_LOCalc_Enable = { 0x09, 0x04, 0x01, 0x00 };
/* MSM_RSSI_Enable bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__MSM_RSSI_Enable = { 0x09, 0x03, 0x01, 0x00 };
/* MSM_IRCAL_Enable bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__MSM_IRCAL_Enable = { 0x09, 0x02, 0x01, 0x00 };
/* MSM_RCCal_Enable bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__MSM_RCCal_Enable = { 0x09, 0x01, 0x01, 0x00 };
/* XtalCal_Enable bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__XtalCal_Enable = { 0x09, 0x00, 0x01, 0x00 };


/* TDA18204 Register IRQ_clear 0x0A */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear = { 0x0A, 0x00, 0x08, 0x00 };
/* IRQ_Clear bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__IRQ_Clear = { 0x0A, 0x07, 0x01, 0x00 };
/* MSM_error_no_clock_available_Clear bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__MSM_error_no_clock_available_Clear = { 0x0A, 0x06, 0x01, 0x00 };
/* MSM_error_config_Clear bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__MSM_error_config_Clear = { 0x0A, 0x05, 0x01, 0x00 };
/* MSM_LOCalc_Clear bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__MSM_LOCalc_Clear = { 0x0A, 0x04, 0x01, 0x00 };
/* MSM_RSSI_Clear bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__MSM_RSSI_Clear = { 0x0A, 0x03, 0x01, 0x00 };
/* MSM_IRCAL_Clear bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__MSM_IRCAL_Clear = { 0x0A, 0x02, 0x01, 0x00 };
/* MSM_RCCal_Clear bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__MSM_RCCal_Clear = { 0x0A, 0x01, 0x01, 0x00 };
/* XtalCal_Clear bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__XtalCal_Clear = { 0x0A, 0x00, 0x01, 0x00 };


/* TDA18204 Register IRQ_set 0x0B */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_set = { 0x0B, 0x00, 0x08, 0x00 };
/* IRQ_Set bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__IRQ_Set = { 0x0B, 0x07, 0x01, 0x00 };
/* MSM_error_no_clock_available_Set bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__MSM_error_no_clock_available_Set = { 0x0B, 0x06, 0x01, 0x00 };
/* MSM_error_config_Set bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__MSM_error_config_Set = { 0x0B, 0x05, 0x01, 0x00 };
/* MSM_LOCalc_Set bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__MSM_LOCalc_Set = { 0x0B, 0x04, 0x01, 0x00 };
/* MSM_RSSI_Set bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__MSM_RSSI_Set = { 0x0B, 0x03, 0x01, 0x00 };
/* MSM_IRCAL_Set bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__MSM_IRCAL_Set = { 0x0B, 0x02, 0x01, 0x00 };
/* MSM_RCCal_Set bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__MSM_RCCal_Set = { 0x0B, 0x01, 0x01, 0x00 };
/* XtalCal_Set bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__XtalCal_Set = { 0x0B, 0x00, 0x01, 0x00 };


/* TDA18204 Register GPIO_CTRL 0x0C */
const TDA18204_BitField_t gTDA18204_Reg_GPIO_CTRL = { 0x0C, 0x00, 0x08, 0x00 };
/* GPIO1 bit(s): 0 : Off  -  1 : absorb the current (then if pull up to Vcc on application => 0V) */
const TDA18204_BitField_t gTDA18204_Reg_GPIO_CTRL__GPIO1 = { 0x0C, 0x00, 0x01, 0x00 };


/* TDA18204 Register POWER_DOWN_1 0x0D */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1 = { 0x0D, 0x00, 0x08, 0x00 };
/* PD_REG2V8RF bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8RF = { 0x0D, 0x07, 0x01, 0x00 };
/* PD_REG2V8IF bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8IF = { 0x0D, 0x06, 0x01, 0x00 };
/* PD_REG2V8SYNT bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8SYNT = { 0x0D, 0x05, 0x01, 0x00 };
/* PSM bit(s): 0 : No PSM  -  1 : PSM Bytes activated */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PSM = { 0x0D, 0x04, 0x01, 0x00 };
/* PD_AGC3 bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_AGC3 = { 0x0D, 0x03, 0x01, 0x00 };
/* PD_AGC2_NB bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_AGC2_NB = { 0x0D, 0x02, 0x01, 0x00 };
/* PD_AGC2 bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_AGC2 = { 0x0D, 0x01, 0x01, 0x00 };
/* PD_AGC1 bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_AGC1 = { 0x0D, 0x00, 0x01, 0x00 };


/* TDA18204 Register POWER_DOWN_2 0x0E */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2 = { 0x0E, 0x00, 0x08, 0x00 };
/* PD_H3H5 bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_H3H5 = { 0x0E, 0x07, 0x01, 0x00 };
/* PD_MTO_BUF1 bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_MTO_BUF1 = { 0x0E, 0x05, 0x01, 0x00 };
/* PD_LT bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_LT = { 0x0E, 0x04, 0x01, 0x00 };
/* PD_DETECT3_NB bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_DETECT3_NB = { 0x0E, 0x03, 0x01, 0x00 };
/* PD_DETECT2_NB bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2_NB = { 0x0E, 0x02, 0x01, 0x00 };
/* PD_DETECT2 bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2 = { 0x0E, 0x01, 0x01, 0x00 };
/* PD_DETECT1 bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_DETECT1 = { 0x0E, 0x00, 0x01, 0x00 };


/* TDA18204 Register POWER_DOWN_3 0x0F */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3 = { 0x0F, 0x00, 0x08, 0x00 };
/* PD_VCO_LOW bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__PD_VCO_LOW = { 0x0F, 0x07, 0x01, 0x00 };
/* PD_VCO_HIGH bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__PD_VCO_HIGH = { 0x0F, 0x06, 0x01, 0x00 };
/* PD_VCO_EXT bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__PD_VCO_EXT = { 0x0F, 0x05, 0x01, 0x00 };
/* AGCK_NB_DIGCLK_ON bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__AGCK_NB_DIGCLK_ON = { 0x0F, 0x04, 0x01, 0x00 };
/* AGC3_NB_DIGCLK_ON bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__AGC3_NB_DIGCLK_ON = { 0x0F, 0x03, 0x01, 0x00 };
/* AGC2_NB_DIGCLK_ON bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__AGC2_NB_DIGCLK_ON = { 0x0F, 0x02, 0x01, 0x00 };
/* AGC2_DIGCLK_ON bit(s): 0: Clock of the AGC2 internal strategy off . . 1: Clock of the AGC2 internal strategy on */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__AGC2_DIGCLK_ON = { 0x0F, 0x01, 0x01, 0x00 };
/* AGC1_DIGCLK_ON bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__AGC1_DIGCLK_ON = { 0x0F, 0x00, 0x01, 0x00 };


/* TDA18204 Register POWER_DOWN_4 0x10 */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4 = { 0x10, 0x00, 0x08, 0x00 };
/* PD_STOB_NB bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_STOB_NB = { 0x10, 0x07, 0x01, 0x00 };
/* PD_LPF_IFAGC bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_LPF_IFAGC = { 0x10, 0x06, 0x01, 0x00 };
/* PD_MIX bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_MIX = { 0x10, 0x05, 0x01, 0x00 };
/* PD_S2B_RFPPF_IFPPF bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_S2B_RFPPF_IFPPF = { 0x10, 0x04, 0x01, 0x00 };
/* PD_GPIO bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_GPIO = { 0x10, 0x03, 0x01, 0x00 };
/* PD_STOB bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_STOB = { 0x10, 0x02, 0x01, 0x00 };
/* PD_AAF_BUF2 bit(s): 0 : buf2 ON  -  1 : buf2 OFF */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_AAF_BUF2 = { 0x10, 0x01, 0x01, 0x00 };
/* PD_TEQ_BUF bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_TEQ_BUF = { 0x10, 0x00, 0x01, 0x00 };


/* TDA18204 Register POWER_DOWN_5 0x11 */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5 = { 0x11, 0x00, 0x08, 0x00 };
/* pd_lochain bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain = { 0x11, 0x07, 0x01, 0x00 };
/* pd_lochain_calmode bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_calmode = { 0x11, 0x06, 0x01, 0x00 };
/* pd_lochain_div1p5 bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div1p5 = { 0x11, 0x05, 0x01, 0x00 };
/* pd_lochain_div2a1 bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2a1 = { 0x11, 0x04, 0x01, 0x00 };
/* pd_lochain_div2b bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2b = { 0x11, 0x03, 0x01, 0x00 };
/* pd_lochain_div2c bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2c = { 0x11, 0x02, 0x01, 0x00 };
/* pd_lochain_div2d bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2d = { 0x11, 0x01, 0x01, 0x00 };
/* pd_lochain_div2e bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2e = { 0x11, 0x00, 0x01, 0x00 };


/* TDA18204 Register POWER_DOWN_6 0x12 */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_6 = { 0x12, 0x00, 0x08, 0x00 };
/* pd_xtal_div1p5 bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_6__pd_xtal_div1p5 = { 0x12, 0x03, 0x01, 0x00 };
/* pd_pll bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_6__pd_pll = { 0x12, 0x02, 0x01, 0x00 };
/* pd_maindivcmos bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_6__pd_maindivcmos = { 0x12, 0x01, 0x01, 0x00 };
/* pd_md_counter bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_6__pd_md_counter = { 0x12, 0x00, 0x01, 0x00 };


/* TDA18204 Register POWER_DOWN_7 0x13 */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_7 = { 0x13, 0x00, 0x08, 0x00 };
/* pd_calgen bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_7__pd_calgen = { 0x13, 0x04, 0x01, 0x00 };
/* pd_xtout bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_7__pd_xtout = { 0x13, 0x03, 0x01, 0x00 };
/* pd_xtal_mux bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_7__pd_xtal_mux = { 0x13, 0x02, 0x01, 0x00 };
/* pd_xtout_asym bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_7__pd_xtout_asym = { 0x13, 0x01, 0x01, 0x00 };
/* pd_sublo bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_7__pd_sublo = { 0x13, 0x00, 0x01, 0x00 };


/* TDA18204 Register LO_CHAIN_MUX_DIV2D 0x14 */
const TDA18204_BitField_t gTDA18204_Reg_LO_CHAIN_MUX_DIV2D = { 0x14, 0x00, 0x08, 0x00 };
/* lochain_mux_div2d bit(s): .63d. : Sleep. .54d. : Divide by 2. .54d. : Divide by 3. .53d. : Divide by 4. .53d. : Divide by 6. .51d. : Divide by 8. .51d. : Divide by 12. .43d. : Divide by 16. .43d. : Divide by 24. .27d. : Divide by 32. .27d. : Divide by 48. -----------------------. .54d. : calibration 880MHz. .46d. : calibration 440MHz. .43d. : calibration 110MHz */
const TDA18204_BitField_t gTDA18204_Reg_LO_CHAIN_MUX_DIV2D__lochain_mux_div2d = { 0x14, 0x00, 0x06, 0x00 };


/* TDA18204 Register SUBLO_PRESC 0x15 */
const TDA18204_BitField_t gTDA18204_Reg_SUBLO_PRESC = { 0x15, 0x00, 0x08, 0x00 };
/* SUBLO_PRESC bit(s): Value of sublo swallow divider */
const TDA18204_BitField_t gTDA18204_Reg_SUBLO_PRESC__SUBLO_PRESC = { 0x15, 0x00, 0x05, 0x00 };


/* TDA18204 Register SUBLO_BYPASS 0x16 */
const TDA18204_BitField_t gTDA18204_Reg_SUBLO_BYPASS = { 0x16, 0x00, 0x08, 0x00 };
/* SUBLO_BYPASS bit(s): Value of sublo divider */
const TDA18204_BitField_t gTDA18204_Reg_SUBLO_BYPASS__SUBLO_BYPASS = { 0x16, 0x00, 0x04, 0x00 };


/* TDA18204 Register RCCAL_READ 0x17 */
const TDA18204_BitField_t gTDA18204_Reg_RCCAL_READ = { 0x17, 0x00, 0x08, 0x00 };
/* RCCAL bit(s): Value of RC CAL */
const TDA18204_BitField_t gTDA18204_Reg_RCCAL_READ__RCCAL = { 0x17, 0x00, 0x05, 0x00 };


/* TDA18204 Register RCCAL_FORCE 0x18 */
const TDA18204_BitField_t gTDA18204_Reg_RCCAL_FORCE = { 0x18, 0x00, 0x08, 0x00 };
/* RCCAL_FORCE bit(s): 0 : Off  -  1: Enable to force the RCCAL value via RCCAL_BYPASS */
const TDA18204_BitField_t gTDA18204_Reg_RCCAL_FORCE__RCCAL_FORCE = { 0x18, 0x05, 0x01, 0x00 };
/* RCCAL_BYPASS_VALUE bit(s): 0: Low - RC with high value +16%. . 31: High - RC with low value -16% */
const TDA18204_BitField_t gTDA18204_Reg_RCCAL_FORCE__RCCAL_BYPASS_VALUE = { 0x18, 0x00, 0x05, 0x00 };


/* TDA18204 Register RCCAL_OFFSET 0x19 */
const TDA18204_BitField_t gTDA18204_Reg_RCCAL_OFFSET = { 0x19, 0x00, 0x08, 0x00 };
/* RCCAL_OFFSET bit(s): Global offset value of RCCAL */
const TDA18204_BitField_t gTDA18204_Reg_RCCAL_OFFSET__RCCAL_OFFSET = { 0x19, 0x00, 0x02, 0x00 };


/* TDA18204 Register AGC1_byte_1 0x1B */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_1 = { 0x1B, 0x00, 0x08, 0x00 };
/* AGC1_Top_Strategy bit(s): Set the AGC1  TOP strategy  */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_1__AGC1_Top_Strategy = { 0x1B, 0x04, 0x03, 0x00 };
/* AGC1_Det_Speed bit(s): change detector speed */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_1__AGC1_Det_Speed = { 0x1B, 0x03, 0x01, 0x00 };
/* AGC1_Top bit(s): Set TOP of AGC1 (link to AGC1_Top_Strategy[2:0]) */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_1__AGC1_Top = { 0x1B, 0x00, 0x03, 0x00 };


/* TDA18204 Register AGC1_byte_2 0x1C */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_2 = { 0x1C, 0x00, 0x08, 0x00 };
/* LNA_Zin bit(s): 1: On  -  0: Off */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_2__LNA_Zin = { 0x1C, 0x04, 0x01, 0x00 };
/* LNA_Range bit(s): change LNA gain Range */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_2__LNA_Range = { 0x1C, 0x00, 0x02, 0x00 };


/* TDA18204 Register AGC1_byte_3 0x1D */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_3 = { 0x1D, 0x00, 0x08, 0x00 };
/* Force_AGC1_gain bit(s): 0 : Off  -  1 : Force AGC1 gain */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_3__Force_AGC1_gain = { 0x1D, 0x04, 0x01, 0x00 };
/* AGC1_Gain bit(s): Field to force the AGC1 Gain */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_3__AGC1_Gain = { 0x1D, 0x00, 0x04, 0x00 };


/* TDA18204 Register AGC1_byte_4 0x1E */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_4 = { 0x1E, 0x00, 0x08, 0x00 };
/* AGC1_smooth_t_cst bit(s): Fix the time constant for the AGC1 switch control */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_4__AGC1_smooth_t_cst = { 0x1E, 0x00, 0x03, 0x00 };


/* TDA18204 Register LT_byte_2 0x20 */
const TDA18204_BitField_t gTDA18204_Reg_LT_byte_2 = { 0x20, 0x00, 0x08, 0x00 };
/* RFsw_LT_RFin bit(s): Switch Buf1 to LTO/MTO : 0 : Openned  -  1 : Closed  */
const TDA18204_BitField_t gTDA18204_Reg_LT_byte_2__RFsw_LT_RFin = { 0x20, 0x06, 0x01, 0x00 };
/* LT_smooth_t_cst bit(s): Fix the time constant for the Lt switch control */
const TDA18204_BitField_t gTDA18204_Reg_LT_byte_2__LT_smooth_t_cst = { 0x20, 0x00, 0x03, 0x00 };


/* TDA18204 Register AGC1_LT_Gain_status 0x21 */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_LT_Gain_status = { 0x21, 0x00, 0x08, 0x00 };
/* AGC1_Smooth_Transition_ON bit(s): enable smoth transition 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_LT_Gain_status__AGC1_Smooth_Transition_ON = { 0x21, 0x07, 0x01, 0x00 };
/* AGC1_gain_read bit(s): Value of the AGC1 gain loop detection (not forced value) */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_LT_Gain_status__AGC1_gain_read = { 0x21, 0x03, 0x04, 0x00 };
/* LT_Gain_Read bit(s): Gain given without offset */
const TDA18204_BitField_t gTDA18204_Reg_AGC1_LT_Gain_status__LT_Gain_Read = { 0x21, 0x00, 0x03, 0x00 };


/* TDA18204 Register AGC2_CONTROL 0x22 */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_CONTROL = { 0x22, 0x00, 0x08, 0x00 };
/* AGC2_EXT_CTRL bit(s): 0 : Internal AGC strategy control  -  1 : External AGC strategy control */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_CONTROL__AGC2_EXT_CTRL = { 0x22, 0x02, 0x01, 0x00 };
/* AGC2_BYPASS bit(s): 0 : Off  -  1 : Bypass AGC2 internal strategy */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_CONTROL__AGC2_BYPASS = { 0x22, 0x01, 0x01, 0x00 };
/* AGC2_GAIN_FORCE bit(s): 0 : Off  -  1: Force AGC2 gain via AGC2_GAIN */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_CONTROL__AGC2_GAIN_FORCE = { 0x22, 0x00, 0x01, 0x00 };


/* TDA18204 Register AGC2_SPEED 0x23 */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_SPEED = { 0x23, 0x00, 0x08, 0x00 };
/* AGC2_LOOP_OFF bit(s): 0 : Off  -  1: Freeze AGC2 gain */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_SPEED__AGC2_LOOP_OFF = { 0x23, 0x04, 0x01, 0x00 };
/* AGC2_SPEED_DOWN bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_SPEED__AGC2_SPEED_DOWN = { 0x23, 0x03, 0x01, 0x00 };
/* AGC2_SPEED_UP bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_SPEED__AGC2_SPEED_UP = { 0x23, 0x02, 0x01, 0x00 };
/* AGC2_SPEED bit(s): Select the clock of the AGC2 digital control  */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_SPEED__AGC2_SPEED = { 0x23, 0x00, 0x02, 0x00 };


/* TDA18204 Register AGC2_STEP 0x24 */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_STEP = { 0x24, 0x00, 0x08, 0x00 };
/* AGC2_DOWN_STEP bit(s): With 74.5 kHz clock */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_STEP__AGC2_DOWN_STEP = { 0x24, 0x04, 0x03, 0x00 };
/* AGC2_UP_STEP bit(s): Enables to change the averaging time constant of the up path accumulator of AGC2. The higher it is, the faster is the averaging (with 74kHz clock) */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_STEP__AGC2_UP_STEP = { 0x24, 0x00, 0x04, 0x00 };


/* TDA18204 Register AGC2_G_UP 0x25 */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_G_UP = { 0x25, 0x00, 0x08, 0x00 };
/* AGC2_G_UP bit(s): TOP up AGC2  :  AGC2_G_UP (dBuVrms) = 103.8  -  0.5 x AGC2_G_UP */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_G_UP__AGC2_G_UP = { 0x25, 0x00, 0x05, 0x00 };


/* TDA18204 Register AGC2_G_DOWN 0x26 */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_G_DOWN = { 0x26, 0x00, 0x08, 0x00 };
/* AGC2_G_DOWN bit(s): TOP down AGC2  :  AGC2_G_DOWN (dBuVrms) = 103.8  -  0.5 x AGC2_G_DOWN */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_G_DOWN__AGC2_G_DOWN = { 0x26, 0x00, 0x05, 0x00 };


/* TDA18204 Register AGC2_GAIN 0x27 */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_GAIN = { 0x27, 0x00, 0x08, 0x00 };
/* AGC2_GAIN bit(s): Field to force the AGC2 Gain  :  AGC2_GAIN (dB) = -12  +  0.2 x AGC2_GAIN */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_GAIN__AGC2_GAIN = { 0x27, 0x00, 0x08, 0x00 };


/* TDA18204 Register AGC2_GAIN_STATUS 0x29 */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_GAIN_STATUS = { 0x29, 0x00, 0x08, 0x00 };
/* AGC2_GAIN_READ bit(s): Value of the AGC2 gain  :  AGC2_GAIN_READ (dB) = -12  +  0.2 x AGC2_GAIN_READ */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_GAIN_STATUS__AGC2_GAIN_READ = { 0x29, 0x00, 0x08, 0x00 };


/* TDA18204 Register AGC2_NB_byte_1 0x2A */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_1 = { 0x2A, 0x00, 0x08, 0x00 };
/* Force_AGC2_NB_gain bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_1__Force_AGC2_NB_gain = { 0x2A, 0x04, 0x01, 0x00 };
/* AGC2_NB_Up_step bit(s): Enables to change the averaging time constant of the up path accumulator of AGC2. The higher it is, the faster is the averaging */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_1__AGC2_NB_Up_step = { 0x2A, 0x02, 0x02, 0x00 };
/* AGC2_NB_Do_step bit(s): Enables to change the averaging time constant of the down path accumulator of AGC2. The higher it is, the faster is the averaging */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_1__AGC2_NB_Do_step = { 0x2A, 0x00, 0x02, 0x00 };


/* TDA18204 Register AGC2_NB_byte_2 0x2B */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_2 = { 0x2B, 0x00, 0x08, 0x00 };
/* AGC2_NB_loop_off bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_loop_off = { 0x2B, 0x07, 0x01, 0x00 };
/* AGC2_NB_TOP_DO bit(s): TOP down AGC2  :  AGC2_G_DOWN (dBuVrms) = 103.8  -  0.5 x AGC2_G_DOWN */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_TOP_DO = { 0x2B, 0x00, 0x05, 0x00 };


/* TDA18204 Register AGC2_NB_byte_3 0x2C */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_3 = { 0x2C, 0x00, 0x08, 0x00 };
/* AGC2_NB_Det_Speed bit(s): Select the clock of the AGC2 digital control  */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_3__AGC2_NB_Det_Speed = { 0x2C, 0x07, 0x01, 0x00 };
/* AGC2_Gain_Control_En bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_3__AGC2_Gain_Control_En = { 0x2C, 0x06, 0x01, 0x00 };
/* AGC2_NB_TOP_UP bit(s): TOP up AGC2  :  AGC2_G_UP (dBuVrms) = 103.8  -  0.5 x AGC2_G_UP */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_3__AGC2_NB_TOP_UP = { 0x2C, 0x00, 0x05, 0x00 };


/* TDA18204 Register AGC2_NB_byte_4 0x2D */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_4 = { 0x2D, 0x00, 0x08, 0x00 };
/* AGC2_NB_GAIN bit(s): Field to force the AGC2 Gain  :  AGC2_GAIN (dB) = -12  +  0.2 x AGC2_GAIN */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_4__AGC2_NB_GAIN = { 0x2D, 0x00, 0x08, 0x00 };


/* TDA18204 Register AGC2_NB_byte_5 0x2E */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_5 = { 0x2E, 0x00, 0x08, 0x00 };
/* AGC2_NB_HP_EN bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_5__AGC2_NB_HP_EN = { 0x2E, 0x01, 0x01, 0x00 };


/* TDA18204 Register AGC2_NB_Gain_status 0x2F */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_Gain_status = { 0x2F, 0x00, 0x08, 0x00 };
/* AGC2_NB_GAIN_READ bit(s): Value of the AGC2 gain  :  AGC2_GAIN_READ (dB) = -12  +  0.2 x AGC2_GAIN_READ */
const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_Gain_status__AGC2_NB_GAIN_READ = { 0x2F, 0x00, 0x08, 0x00 };


/* TDA18204 Register AGCK_byte_1 0x30 */
const TDA18204_BitField_t gTDA18204_Reg_AGCK_byte_1 = { 0x30, 0x00, 0x08, 0x00 };
/* DET12_Cint bit(s): change analog part of detector speed */
const TDA18204_BitField_t gTDA18204_Reg_AGCK_byte_1__DET12_Cint = { 0x30, 0x04, 0x02, 0x00 };
/* AGCK_Step bit(s): AGCK gain step */
const TDA18204_BitField_t gTDA18204_Reg_AGCK_byte_1__AGCK_Step = { 0x30, 0x02, 0x02, 0x00 };
/* AGCK_Mode bit(s): AGCK time constant */
const TDA18204_BitField_t gTDA18204_Reg_AGCK_byte_1__AGCK_Mode = { 0x30, 0x00, 0x02, 0x00 };


/* TDA18204 Register AGC3_NB_byte_1 0x31 */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_1 = { 0x31, 0x00, 0x08, 0x00 };
/* AGC3_NB_TOP_DO bit(s): TOP down AGC3  :  AGC3_G_DOWN (dBuVrms) = 117  -  AGC3_G_DOWN */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_1__AGC3_NB_TOP_DO = { 0x31, 0x04, 0x04, 0x00 };
/* AGC3_NB_TOP_UP bit(s): TOP up AGC3  :  AGC3_G_UP (dBuVrms) = 117  -  AGC3_G_UP */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_1__AGC3_NB_TOP_UP = { 0x31, 0x00, 0x04, 0x00 };


/* TDA18204 Register AGC3_NB_byte_2 0x32 */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_2 = { 0x32, 0x00, 0x08, 0x00 };
/* AGC3_NB_loop_off bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_2__AGC3_NB_loop_off = { 0x32, 0x07, 0x01, 0x00 };
/* Force_AGC3_NB_gain bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_2__Force_AGC3_NB_gain = { 0x32, 0x03, 0x01, 0x00 };
/* AGC3_NB_GAIN bit(s): Gain setting of the AGC3_NB (if Force_AGC3_NB_gain=.1.) */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_2__AGC3_NB_GAIN = { 0x32, 0x00, 0x03, 0x00 };


/* TDA18204 Register AGC3_NB_byte_3 0x33 */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_3 = { 0x33, 0x00, 0x08, 0x00 };
/* DET3_FastAtt bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_3__DET3_FastAtt = { 0x33, 0x07, 0x01, 0x00 };
/* DET3_LowBW bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_3__DET3_LowBW = { 0x33, 0x06, 0x01, 0x00 };
/* RSSI_HP_Fc bit(s): change RSSI cut-off frequency */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_3__RSSI_HP_Fc = { 0x33, 0x04, 0x02, 0x00 };
/* AGC3_NB_Do_step bit(s): Enables to change the averaging time constant of the down path accumulator of AGC3_NB. The higher it is, the faster is the averaging */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_3__AGC3_NB_Do_step = { 0x33, 0x02, 0x02, 0x00 };
/* AGC3_NB_Up_step bit(s): time constant for up step */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_3__AGC3_NB_Up_step = { 0x33, 0x00, 0x02, 0x00 };


/* TDA18204 Register AGC3_NB_gain_status 0x34 */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_gain_status = { 0x34, 0x00, 0x08, 0x00 };
/* RSSI_ALARM bit(s): read value */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_gain_status__RSSI_ALARM = { 0x34, 0x04, 0x01, 0x00 };
/* AGC3_NB_GAIN_READ bit(s): Field to read the AGC3 Gain */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_gain_status__AGC3_NB_GAIN_READ = { 0x34, 0x00, 0x04, 0x00 };


/* TDA18204 Register AGC3_GAIN_WB 0x35 */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_GAIN_WB = { 0x35, 0x00, 0x08, 0x00 };
/* AGC3_GAIN bit(s): Gain setting of the AGC3_WB */
const TDA18204_BitField_t gTDA18204_Reg_AGC3_GAIN_WB__AGC3_GAIN = { 0x35, 0x00, 0x03, 0x00 };


/* TDA18204 Register TEQ_SELECT 0x36 */
const TDA18204_BitField_t gTDA18204_Reg_TEQ_SELECT = { 0x36, 0x00, 0x08, 0x00 };
/* TEQ_SELECT bit(s): slope choice:. . TEQ : +15dB, +10dB, +5dB, +3dB, bypass, -5dB, -10dB */
const TDA18204_BitField_t gTDA18204_Reg_TEQ_SELECT__TEQ_SELECT = { 0x36, 0x00, 0x03, 0x00 };


/* TDA18204 Register POWERDOWN 0x37 */
const TDA18204_BitField_t gTDA18204_Reg_POWERDOWN = { 0x37, 0x00, 0x08, 0x00 };
/* FORCE_SOFT_RESET bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_POWERDOWN__FORCE_SOFT_RESET = { 0x37, 0x01, 0x01, 0x00 };
/* SOFT_RESET bit(s): Reset all the digital part if FORCE_SOFT_RESET=1, . . place the state machine in idle mode. . . DOES NOT reset the I2C register */
const TDA18204_BitField_t gTDA18204_Reg_POWERDOWN__SOFT_RESET = { 0x37, 0x00, 0x01, 0x00 };


/* TDA18204 Register POWER_SAVING_MODE_1 0x41 */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_1 = { 0x41, 0x00, 0x08, 0x00 };
/* BOOST_VCO bit(s): 1: On  -  0: Off */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_1__BOOST_VCO = { 0x41, 0x04, 0x01, 0x00 };
/* PSM_MTOBUF1 bit(s): 22 mA POR state,. . 17 mA PSM=1 state. */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_1__PSM_MTOBUF1 = { 0x41, 0x00, 0x04, 0x00 };


/* TDA18204 Register POWER_SAVING_MODE_2 0x42 */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_2 = { 0x42, 0x00, 0x08, 0x00 };
/* PSM_AGC1 bit(s): 31.8 mA POR state,. . 21.8 mA PSM=1 state */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_2__PSM_AGC1 = { 0x42, 0x04, 0x02, 0x00 };
/* PSM_AGC2 bit(s): 28.9 mA POR state,. . 22.9 mA PSM=1 state */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_2__PSM_AGC2 = { 0x42, 0x02, 0x02, 0x00 };
/* PSM_AGC3 bit(s): 32.2 mA POR state,. . 26.3 mA PSM=1 state */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_2__PSM_AGC3 = { 0x42, 0x00, 0x02, 0x00 };


/* TDA18204 Register POWER_SAVING_MODE_3 0x43 */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_3 = { 0x43, 0x00, 0x08, 0x00 };
/* PSM_AAF_BUF2 bit(s): PSM of AAF Buf2. . 6.22 mA for 1S2D, POR state, PSM=0. . 5.31 mA, PSM=1 state */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_3__PSM_AAF_BUF2 = { 0x43, 0x04, 0x04, 0x00 };
/* PSM_TEQ_BUF bit(s): 10 mA POR state,. . 8.6 mA PSM=1 state */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_3__PSM_TEQ_BUF = { 0x43, 0x00, 0x04, 0x00 };


/* TDA18204 Register POWER_SAVING_MODE_4 0x44 */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_4 = { 0x44, 0x00, 0x08, 0x00 };
/* PSM_RFPPF bit(s): .00. : 0 mA. . .01. : -4.2 mA. . .10. : -7.2 mA. . .11. : -9.3 mA */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_4__PSM_RFPPF = { 0x44, 0x06, 0x02, 0x00 };
/* PSM_Mixer bit(s): .00. : nominal. . .01. : -1.2mA. . .10. : -1.8mA. . .11. : -2.3mA */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_4__PSM_Mixer = { 0x44, 0x04, 0x02, 0x00 };
/* PSM_Ifpoly bit(s): .00. : nominal. . .01. : -1.8 mA. . .10. : -3.2 mA. . .11. : -4.1 mA */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_4__PSM_Ifpoly = { 0x44, 0x02, 0x02, 0x00 };
/* PSM_Lodriver bit(s): .00. : nominal. . .01. : -3.0 mA. . .10. : -4.5 mA. . .11. : -6.0 mA */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_4__PSM_Lodriver = { 0x44, 0x00, 0x02, 0x00 };


/* TDA18204 Register POWER_SAVING_MODE_5 0x45 */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_5 = { 0x45, 0x00, 0x08, 0x00 };
/* PSM_lpf bit(s): .00. : nominal. . .01. : -2.3 mA. . .10. : -3.8 mA. . .11. : -4.9 mA */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_lpf = { 0x45, 0x06, 0x02, 0x00 };
/* PSM_hpf_ifn bit(s): .00. : nominal. . .01. : -1.6 mA. . .10. : -2.7 mA. . .11. : -3.4 mA */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_hpf_ifn = { 0x45, 0x04, 0x02, 0x00 };
/* PSM_H35 bit(s): Power saving mode H3H5 */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_H35 = { 0x45, 0x02, 0x02, 0x00 };
/* PSM_VCO bit(s): .00.: 2 mA (low) - 5 mA (high). . .01.: 3 mA (low) - 6.25 mA (high). . .10.: 3.5 mA (low) - 6.75 mA (high). . .11.: 4.5 mA (low) - 8 mA (high) */
const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_VCO = { 0x45, 0x00, 0x02, 0x00 };


/* TDA18204 Register H3H5_byte 0x47 */
const TDA18204_BitField_t gTDA18204_Reg_H3H5_byte = { 0x47, 0x00, 0x08, 0x00 };
/* SWITCH_STAB_CAP bit(s): Select switching stability capacitor :. 00000: 0 fF. 00001: 530 fF. 00010: 380 fF. 00011: 910 fF. 00100: 500 fF. 00101: 1030 fF. 00110: 880 fF. 00111: 1410 fF. 01000: 190 fF. 01001: 720 fF. 01010: 570 fF. 01011: 1100 fF. 01100: 690 fF. 01101: 1220 fF. 01110: 1070 fF. 01111: 1600 fF. 10000: 140 fF. 10001: 530 fF. 10010: 380 fF. 10011: 910 fF. 10100: 500 fF. 10101: 1030 fF. 10110: 880 fF. 10111: 1410 fF. 11000: 190 fF. 11001: 720 fF. 11010: 570 fF. 11011: 1100 fF. 11100: 690 fF. 11101: 1220 fF. 11110: 1070 fF. 11111: 1600 fF */
const TDA18204_BitField_t gTDA18204_Reg_H3H5_byte__SWITCH_STAB_CAP = { 0x47, 0x02, 0x05, 0x00 };
/* PSM_H35_EXTRA_BIT bit(s): 0: PSM0 or PSM1 or PSM2  -  1: PSM3= 48.28 mA (Only available on ES2) */
const TDA18204_BitField_t gTDA18204_Reg_H3H5_byte__PSM_H35_EXTRA_BIT = { 0x47, 0x01, 0x01, 0x00 };
/* H3H5_Dcboost bit(s): 0: Off  -  1: On (Only available on ES2) */
const TDA18204_BitField_t gTDA18204_Reg_H3H5_byte__H3H5_Dcboost = { 0x47, 0x00, 0x01, 0x00 };


/* TDA18204 Register W_Filter_byte 0x49 */
const TDA18204_BitField_t gTDA18204_Reg_W_Filter_byte = { 0x49, 0x00, 0x08, 0x00 };
/* W_Filter_Enable bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_W_Filter_byte__W_Filter_Enable = { 0x49, 0x04, 0x01, 0x00 };
/* W_Filter bit(s): Select wireless filter frequency */
const TDA18204_BitField_t gTDA18204_Reg_W_Filter_byte__W_Filter = { 0x49, 0x02, 0x02, 0x00 };
/* W_Filter_Offset bit(s): .00. : +4%. . .01. : nominal. . .10. : -4%. . .11. : -8% */
const TDA18204_BitField_t gTDA18204_Reg_W_Filter_byte__W_Filter_Offset = { 0x49, 0x00, 0x02, 0x00 };


/* TDA18204 Register RF_Band_Pass_Filter_byte 0x4A */
const TDA18204_BitField_t gTDA18204_Reg_RF_Band_Pass_Filter_byte = { 0x4A, 0x00, 0x08, 0x00 };
/* RF_BPF bit(s): Select H3H5 filter frequency */
const TDA18204_BitField_t gTDA18204_Reg_RF_Band_Pass_Filter_byte__RF_BPF = { 0x4A, 0x00, 0x05, 0x00 };


/* TDA18204 Register IR_Mixer_byte_1 0x4B */
const TDA18204_BitField_t gTDA18204_Reg_IR_Mixer_byte_1 = { 0x4B, 0x00, 0x08, 0x00 };
/* Mixer_GateDC bit(s): Improves mixer gain and linearity when psm_mixer=3 */
const TDA18204_BitField_t gTDA18204_Reg_IR_Mixer_byte_1__Mixer_GateDC = { 0x4B, 0x05, 0x02, 0x00 };
/* S2D_Gain bit(s): change gain */
const TDA18204_BitField_t gTDA18204_Reg_IR_Mixer_byte_1__S2D_Gain = { 0x4B, 0x01, 0x02, 0x00 };
/* DC_Notch bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_IR_Mixer_byte_1__DC_Notch = { 0x4B, 0x00, 0x01, 0x00 };


/* TDA18204 Register IF_AGC_byte 0x4C */
const TDA18204_BitField_t gTDA18204_Reg_IF_AGC_byte = { 0x4C, 0x00, 0x08, 0x00 };
/* LPF_DCOffset_Corr bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_IF_AGC_byte__LPF_DCOffset_Corr = { 0x4C, 0x06, 0x01, 0x00 };
/* IF_level bit(s): IF output level selection */
const TDA18204_BitField_t gTDA18204_Reg_IF_AGC_byte__IF_level = { 0x4C, 0x00, 0x03, 0x00 };


/* TDA18204 Register IF_Byte_1 0x4D */
const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_1 = { 0x4D, 0x00, 0x08, 0x00 };
/* IF_LPF_Offset bit(s): shift value RCCal IF_LPF (depends of IF_LPF_sign). . 00:  Nominal. . 01: + or - 4%. . 10: + or - 8%. . 11: + or - 12% */
const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_1__IF_LPF_Offset = { 0x4D, 0x05, 0x02, 0x00 };
/* IF_LPF_sign bit(s): .0. : positive shift. . .1. : negative shift */
const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_1__IF_LPF_sign = { 0x4D, 0x04, 0x01, 0x00 };
/* LP_Fc bit(s): Select low pass filter cut-off frequency. .000. : 6 MHz. .001. : 7 MHz. .010. : 8 MHz. .011. : 9 MHz. .100. : 10 MHz. .101. : 11.2 MHz. .110. : 1.5 MHz. .111. : 1.5 MHz */
const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_1__LP_Fc = { 0x4D, 0x00, 0x03, 0x00 };


/* TDA18204 Register IF_Byte_2 0x4E */
const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_2 = { 0x4E, 0x00, 0x08, 0x00 };
/* IF_HPF_Offset bit(s): shift value RCCal IF_HPF (depends of IF_HPF_sign). . 00:  Nominal. . 01: + or - 4%. . 10: + or - 8%. . 11: + or - 12% */
const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_2__IF_HPF_Offset = { 0x4E, 0x05, 0x02, 0x00 };
/* IF_HPF_sign bit(s): .0. : positive shift. . .1. : negative shift */
const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_2__IF_HPF_sign = { 0x4E, 0x04, 0x01, 0x00 };
/* IF_HP_Fc bit(s): Select high pass filter cut-off frequency. . 000 : 0,5 MHz. . 001 : 1 MHz. . 010 : 1.6 MHz. . 011 : 2.3 MHz. . 100 :  3.5 MHz. . others : 3.5 MHz */
const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_2__IF_HP_Fc = { 0x4E, 0x01, 0x03, 0x00 };
/* IF_HP_Enable bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_2__IF_HP_Enable = { 0x4E, 0x00, 0x01, 0x00 };


/* TDA18204 Register clock_ctrl_byte_1 0x4F */
const TDA18204_BitField_t gTDA18204_Reg_clock_ctrl_byte_1 = { 0x4F, 0x00, 0x08, 0x00 };
/* Dig_Clock bit(s): Select he digital after calibration clock is:. . .00. : internal oscillator. . .01. : xtal in or buffer. . .10. : agc2clkgated. . .11. : sub lo */
const TDA18204_BitField_t gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock = { 0x4F, 0x00, 0x02, 0x00 };


/* TDA18204 Register FVCO_MAX_BYTE_1 0x51 */
const TDA18204_BitField_t gTDA18204_Reg_FVCO_MAX_BYTE_1 = { 0x51, 0x00, 0x08, 0x00 };
/* FVCO_max_1 bit(s): Highest frequency of VCO high in [Hz] */
const TDA18204_BitField_t gTDA18204_Reg_FVCO_MAX_BYTE_1__FVCO_max_1 = { 0x51, 0x00, 0x08, 0x00 };


/* TDA18204 Register FVCO_MAX_BYTE_2 0x52 */
const TDA18204_BitField_t gTDA18204_Reg_FVCO_MAX_BYTE_2 = { 0x52, 0x00, 0x08, 0x00 };
/* FVCO_max_2 bit(s): Highest frequency of VCO high in [Hz] */
const TDA18204_BitField_t gTDA18204_Reg_FVCO_MAX_BYTE_2__FVCO_max_2 = { 0x52, 0x00, 0x08, 0x00 };


/* TDA18204 Register FVCO_MAX_BYTE_3 0x53 */
const TDA18204_BitField_t gTDA18204_Reg_FVCO_MAX_BYTE_3 = { 0x53, 0x00, 0x08, 0x00 };
/* FVCO_max_3 bit(s): Highest frequency of VCO high in [Hz] */
const TDA18204_BitField_t gTDA18204_Reg_FVCO_MAX_BYTE_3__FVCO_max_3 = { 0x53, 0x00, 0x08, 0x00 };


/* TDA18204 Register MSM_byte_1 0x54 */
const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_1 = { 0x54, 0x00, 0x08, 0x00 };
/* RSSI_Meas bit(s): launch rssi measurement */
const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_1__RSSI_Meas = { 0x54, 0x07, 0x01, 0x00 };
/* IR_Cal_Image bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_1__IR_Cal_Image = { 0x54, 0x05, 0x01, 0x00 };
/* IR_CAL_Wanted bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_1__IR_CAL_Wanted = { 0x54, 0x04, 0x01, 0x00 };
/* RC_Cal bit(s): Launch the RC cal */
const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_1__RC_Cal = { 0x54, 0x01, 0x01, 0x00 };
/* FVCO_max_calc bit(s): Launch Fvco max_calculation */
const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_1__FVCO_max_calc = { 0x54, 0x00, 0x01, 0x00 };


/* TDA18204 Register MSM_byte_2 0x55 */
const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_2 = { 0x55, 0x00, 0x08, 0x00 };
/* TEQ_Launch bit(s): launch TEQ change Slope */
const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_2__TEQ_Launch = { 0x55, 0x02, 0x01, 0x00 };
/* XtalCal_Launch bit(s): launch Xtal cal */
const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_2__XtalCal_Launch = { 0x55, 0x01, 0x01, 0x00 };
/* MSM_Launch bit(s): MSM */
const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_2__MSM_Launch = { 0x55, 0x00, 0x01, 0x00 };


/* TDA18204 Register RSSI_byte_1 0x56 */
const TDA18204_BitField_t gTDA18204_Reg_RSSI_byte_1 = { 0x56, 0x00, 0x08, 0x00 };
/* RSSI bit(s): rssi agc gain */
const TDA18204_BitField_t gTDA18204_Reg_RSSI_byte_1__RSSI = { 0x56, 0x00, 0x08, 0x00 };


/* TDA18204 Register RSSI_byte_2 0x57 */
const TDA18204_BitField_t gTDA18204_Reg_RSSI_byte_2 = { 0x57, 0x00, 0x08, 0x00 };
/* RSSI_Gv bit(s): RSSI input gain */
const TDA18204_BitField_t gTDA18204_Reg_RSSI_byte_2__RSSI_Gv = { 0x57, 0x06, 0x02, 0x00 };
/* RSSI_Cap_Val bit(s): Capacitor value */
const TDA18204_BitField_t gTDA18204_Reg_RSSI_byte_2__RSSI_Cap_Val = { 0x57, 0x02, 0x01, 0x00 };
/* RSSI_Ck_Speed bit(s): Clock value */
const TDA18204_BitField_t gTDA18204_Reg_RSSI_byte_2__RSSI_Ck_Speed = { 0x57, 0x01, 0x01, 0x00 };


/* TDA18204 Register Sigma_delta_byte_1 0x59 */
const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_1 = { 0x59, 0x00, 0x08, 0x00 };
/* LO_Int bit(s): Integer maindiv programming (N) */
const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_1__LO_Int = { 0x59, 0x00, 0x07, 0x00 };


/* TDA18204 Register Sigma_delta_byte_2 0x5A */
const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_2 = { 0x5A, 0x00, 0x08, 0x00 };
/* LO_Frac_2 bit(s): Fractional maindiv programming (K) */
const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_2__LO_Frac_2 = { 0x5A, 0x00, 0x07, 0x00 };


/* TDA18204 Register Sigma_delta_byte_3 0x5B */
const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_3 = { 0x5B, 0x00, 0x08, 0x00 };
/* LO_Frac_1 bit(s): Fractional maindiv programming (K) */
const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_3__LO_Frac_1 = { 0x5B, 0x00, 0x08, 0x00 };


/* TDA18204 Register Sigma_delta_byte_4 0x5C */
const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_4 = { 0x5C, 0x00, 0x08, 0x00 };
/* LO_Frac_0 bit(s): Fractional maindiv programming (K) */
const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_4__LO_Frac_0 = { 0x5C, 0x00, 0x08, 0x00 };


/* TDA18204 Register Sigma_delta_Test_byte 0x5D */
const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_Test_byte = { 0x5D, 0x00, 0x08, 0x00 };
/* LOSDrstN bit(s): 1: On  -  0: Off */
const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_Test_byte__LOSDrstN = { 0x5D, 0x01, 0x01, 0x00 };


/* TDA18204 Register PLL_byte 0x5E */
const TDA18204_BitField_t gTDA18204_Reg_PLL_byte = { 0x5E, 0x00, 0x08, 0x00 };
/* PLL_lock_reset bit(s): 1: On  -  0: Off */
const TDA18204_BitField_t gTDA18204_Reg_PLL_byte__PLL_lock_reset = { 0x5E, 0x00, 0x01, 0x00 };


/* TDA18204 Register rf_switch_smooth_state 0x60 */
const TDA18204_BitField_t gTDA18204_Reg_rf_switch_smooth_state = { 0x60, 0x00, 0x08, 0x00 };
/* rf_mux_agc2_wb_smooth_state bit(s): switch state:. .0. : switch opened. .1. : switch closed */
const TDA18204_BitField_t gTDA18204_Reg_rf_switch_smooth_state__rf_mux_agc2_wb_smooth_state = { 0x60, 0x00, 0x01, 0x00 };


/* TDA18204 Register rf_switchs 0x61 */
const TDA18204_BitField_t gTDA18204_Reg_rf_switchs = { 0x61, 0x00, 0x08, 0x00 };
/* RF_mux_acg2_nb bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_rf_switchs__RF_mux_acg2_nb = { 0x61, 0x01, 0x01, 0x00 };
/* rf_mux_acg2_wb_smooth_move bit(s): 0: On  -  1: Off */
const TDA18204_BitField_t gTDA18204_Reg_rf_switchs__rf_mux_acg2_wb_smooth_move = { 0x61, 0x00, 0x01, 0x00 };


/* TDA18204 Register calgen_byte_1 0x62 */
const TDA18204_BitField_t gTDA18204_Reg_calgen_byte_1 = { 0x62, 0x00, 0x08, 0x00 };
/* vco_IR_high bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_calgen_byte_1__vco_IR_high = { 0x62, 0x04, 0x01, 0x00 };
/* calgen_pph_sel bit(s): 0: Off  -  1: On */
const TDA18204_BitField_t gTDA18204_Reg_calgen_byte_1__calgen_pph_sel = { 0x62, 0x00, 0x01, 0x00 };


/* TDA18204 Register IR_Cal_byte_1 0x63 */
const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_1 = { 0x63, 0x00, 0x08, 0x00 };
/* IR_IF_LPF_1p5M bit(s): select mode Ircal (xtal :16MHz) for LPF */
const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_1__IR_IF_LPF_1p5M = { 0x63, 0x06, 0x01, 0x00 };
/* IR_Mixer_Atten bit(s): Selects amplitude for the IR calibration */
const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_1__IR_Mixer_Atten = { 0x63, 0x02, 0x03, 0x00 };


/* TDA18204 Register IR_Cal_byte_2 0x64 */
const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_2 = { 0x64, 0x00, 0x08, 0x00 };
/* IR_Step_Delta bit(s): select the step of convergence */
const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_2__IR_Step_Delta = { 0x64, 0x04, 0x04, 0x00 };
/* IR_FreqPtr bit(s): Selects the frequency band for IRCAL */
const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_2__IR_FreqPtr = { 0x64, 0x02, 0x02, 0x00 };


/* TDA18204 Register IR_Cal_byte_3 0x65 */
const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_3 = { 0x65, 0x00, 0x08, 0x00 };
/* IR_target bit(s): Select IR target */
const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_3__IR_target = { 0x65, 0x06, 0x02, 0x00 };


/* TDA18204 Register IR_Cal_byte_4 0x66 */
const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_4 = { 0x66, 0x00, 0x08, 0x00 };
/* Rx bit(s): read value of Rx for IR compensation */
const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_4__Rx = { 0x66, 0x00, 0x06, 0x00 };


/* TDA18204 Register Charge_pump_byte 0x67 */
const TDA18204_BitField_t gTDA18204_Reg_Charge_pump_byte = { 0x67, 0x00, 0x08, 0x00 };
/* ICP_Force_Src bit(s): 1: On  -  0: Off */
const TDA18204_BitField_t gTDA18204_Reg_Charge_pump_byte__ICP_Force_Src = { 0x67, 0x05, 0x01, 0x00 };
/* ICP_low bit(s): 1: On  -  0: Off */
const TDA18204_BitField_t gTDA18204_Reg_Charge_pump_byte__ICP_low = { 0x67, 0x02, 0x01, 0x00 };
/* ICP_select bit(s): select offset current of charge pump:. . 00: 500microA. . 01: 150microA. . 10: 300microA. . 11: 100microA */
const TDA18204_BitField_t gTDA18204_Reg_Charge_pump_byte__ICP_select = { 0x67, 0x00, 0x02, 0x00 };


