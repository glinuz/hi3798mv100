/*
  Copyright (C) 2011 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18204_RegDef.h
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


#pragma once
#ifndef _TMBSL_TDA18204_REGDEF_H
#define _TMBSL_TDA18204_REGDEF_H

#ifdef __cplusplus
extern "C"
{
#endif


/*============================================================================*/
/* Registers definitions:                                                     */
/*============================================================================*/

#define TDA18204_REG_ADD_SZ                             (0x01)
#define TDA18204_REG_DATA_MAX_SZ                        (0x01)
#define TDA18204_REG_MAP_NB_BYTES                       (0x76)

#define TDA18204_REG_DATA_LEN(_FIRST_REG, _LAST_REG)    ( (_LAST_REG.Address - _FIRST_REG.Address) + 1)

/* TDA18204 Register ID_1 0x00 */
extern const TDA18204_BitField_t gTDA18204_Reg_ID_1;
/* IDENT_1 bit(s): IDENT_1 & IDENT_2 = 18204 for TDA18204 */
extern const TDA18204_BitField_t gTDA18204_Reg_ID_1__IDENT_1;


/* TDA18204 Register ID_2 0x01 */
extern const TDA18204_BitField_t gTDA18204_Reg_ID_2;
/* IDENT_2 bit(s): IDENT_1 & IDENT_2 = 18204 for TDA18204 */
extern const TDA18204_BitField_t gTDA18204_Reg_ID_2__IDENT_2;


/* TDA18204 Register ID_3 0x02 */
extern const TDA18204_BitField_t gTDA18204_Reg_ID_3;
/* MAJOR_REV bit(s): Metal fixable with ivfix. . 1 --> ES1 */
extern const TDA18204_BitField_t gTDA18204_Reg_ID_3__MAJOR_REV;
/* MINOR_REV bit(s): Metal fixable with ivfix. . 0 --> version A. ,B,C,D */
extern const TDA18204_BitField_t gTDA18204_Reg_ID_3__MINOR_REV;


/* TDA18204 Register THERMO_1 0x03 */
extern const TDA18204_BitField_t gTDA18204_Reg_THERMO_1;
/* TM_D bit(s): Temperature Display */
extern const TDA18204_BitField_t gTDA18204_Reg_THERMO_1__TM_D;


/* TDA18204 Register THERMO_2 0x04 */
extern const TDA18204_BitField_t gTDA18204_Reg_THERMO_2;
/* TM_ON bit(s): 0 : Thermo Off  -  1 : Launch Temperature Measurement */
extern const TDA18204_BitField_t gTDA18204_Reg_THERMO_2__TM_ON;


/* TDA18204 Register POWER_STATUS 0x05 */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_STATUS;
/* LO_Lock bit(s): status read only */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_STATUS__LO_Lock;
/* POR bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_STATUS__POR;


typedef enum _TDA18204_XTAL_IN_CTRL_Item_t {
    TDA18204_XTAL_IN_CTRL_NO_XTAL_NO_BUFFER_IN_VALUE_0 = 0,		/*  no xtal, no buffer in  */
    TDA18204_XTAL_IN_CTRL_NO_XTAL_NO_BUFFER_IN_VALUE_1 = 1,		/*  no xtal, no buffer in  */
    TDA18204_XTAL_IN_CTRL_NO_XTAL_NO_BUFFER_IN_VALUE_2 = 2,		/*  no xtal, no buffer in  */
    TDA18204_XTAL_IN_CTRL_NO_XTAL_NO_BUFFER_IN_VALUE_3 = 3,		/*  no xtal, no buffer in  */
    TDA18204_XTAL_IN_CTRL_BUFFER_IN_16MHZ_VALUE_4 = 4,		/*  buffer in 16MHz  */
    TDA18204_XTAL_IN_CTRL_BUFFER_IN_27MHZ_VALUE_5 = 5,		/*  buffer in 27MHz  */
    TDA18204_XTAL_IN_CTRL_XTAL_16MHZ_VALUE_6 = 6,		/*  xtal 16MHz  */
    TDA18204_XTAL_IN_CTRL_XTAL_27MHZ_VALUE_7 = 7		/*  xtal 27MHz  */
} TDA18204_XTAL_IN_CTRL_t, *pTDA18204_XTAL_IN_CTRL_t;

/* TDA18204 Register XTAL_IN_CTRL 0x06 */
extern const TDA18204_BitField_t gTDA18204_Reg_XTAL_IN_CTRL;
/* XTAL_IN_CTRL bit(s): .111.: xtal 27MHz. .110.: xtal 16MHz. .101.: buffer in 27MHz. .100.: buffer in 16MHz. others: no xtal, no buffer in */
extern const TDA18204_BitField_t gTDA18204_Reg_XTAL_IN_CTRL__XTAL_IN_CTRL;


typedef enum _TDA18204_Xtout_Amp_Item_t {
    TDA18204_Xtout_Amp_200_VALUE_0 = 0,		/*  200  */
    TDA18204_Xtout_Amp_400_VALUE_1 = 1,		/*  400  */
    TDA18204_Xtout_Amp_600_VALUE_2 = 2,		/*  600  */
    TDA18204_Xtout_Amp_800_VALUE_3 = 3,		/*  800  */
    TDA18204_Xtout_Amp_600_VALUE_4 = 4,		/*  600  */
    TDA18204_Xtout_Amp_800_VALUE_5 = 5,		/*  800  */
    TDA18204_Xtout_Amp_1000_VALUE_6 = 6,		/*  1000  */
    TDA18204_Xtout_Amp_1200_VALUE_7 = 7		/*  1200  */
} TDA18204_Xtout_Amp_t, *pTDA18204_Xtout_Amp_t;

typedef enum _TDA18204_XTout_Item_t {
    TDA18204_XTout_NO_SIGNAL_16MINUS27_MHZ_SINUS_16MINUS27_MHZ_RAIL_TO_RAIL_VALUE_0 = 0,		/*  No signal / 16-27 MHz sinus / 16-27 MHz rail to rail  */
    TDA18204_XTout_FDIV_PLL_VALUE_1 = 1,		/*  Fdiv PLL  */
    TDA18204_XTout_FREF_PLL_VALUE_2 = 2,		/*  Fref PLL  */
    TDA18204_XTout_SUBLO_VALUE_3 = 3,		/*  sublo  */
    TDA18204_XTout_CLOCK_OSC_VALUE_4 = 4,		/*   clock osc  */
    TDA18204_XTout_SYNC_VALUE_5 = 5		/*  Sync  */
} TDA18204_XTout_t, *pTDA18204_XTout_t;

/* TDA18204 Register XTAL_OUT_CTRL 0x07 */
extern const TDA18204_BitField_t gTDA18204_Reg_XTAL_OUT_CTRL;
/* Xtout_Amp bit(s): .000.: 200 mV. . .001.: 400 mV. . .010.: 600 mV. . .011.: 800 mV. . .100.: 600 mV. . .101.: 800 mV. . .110.: 1000 mV. . .111.: 1200 mV */
extern const TDA18204_BitField_t gTDA18204_Reg_XTAL_OUT_CTRL__Xtout_Amp;
/* XTout bit(s): select Xtout. . 000 : No signal / 16-27 MHz sinus / 16-27 MHz rail to rail. 001 : Fdiv PLL. . 010 : Fref PLL. . 011 : sublo. . 100 : clock osc. . 101 : Sync */
extern const TDA18204_BitField_t gTDA18204_Reg_XTAL_OUT_CTRL__XTout;


/* TDA18204 Register IRQ_status 0x08 */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_status;
/* IRQ_status bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__IRQ_status;
/* MSM_error_no_clock_available bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__MSM_error_no_clock_available;
/* MSM_error_config bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__MSM_error_config;
/* MSM_LOCalc_End bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__MSM_LOCalc_End;
/* MSM_RSSI_End bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__MSM_RSSI_End;
/* MSM_IRCAL_End bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__MSM_IRCAL_End;
/* MSM_RCCal_End bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__MSM_RCCal_End;
/* XtalCal_End bit(s): read value: . . .0. : cal not done. . .1. : cal done */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_status__XtalCal_End;


/* TDA18204 Register IRQ_enable 0x09 */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable;
/* IRQ_Enable bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__IRQ_Enable;
/* MSM_error_no_clock_available_Enable bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__MSM_error_no_clock_available_Enable;
/* MSM_error_config_Enable bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__MSM_error_config_Enable;
/* MSM_LOCalc_Enable bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__MSM_LOCalc_Enable;
/* MSM_RSSI_Enable bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__MSM_RSSI_Enable;
/* MSM_IRCAL_Enable bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__MSM_IRCAL_Enable;
/* MSM_RCCal_Enable bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__MSM_RCCal_Enable;
/* XtalCal_Enable bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_enable__XtalCal_Enable;


/* TDA18204 Register IRQ_clear 0x0A */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear;
/* IRQ_Clear bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__IRQ_Clear;
/* MSM_error_no_clock_available_Clear bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__MSM_error_no_clock_available_Clear;
/* MSM_error_config_Clear bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__MSM_error_config_Clear;
/* MSM_LOCalc_Clear bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__MSM_LOCalc_Clear;
/* MSM_RSSI_Clear bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__MSM_RSSI_Clear;
/* MSM_IRCAL_Clear bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__MSM_IRCAL_Clear;
/* MSM_RCCal_Clear bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__MSM_RCCal_Clear;
/* XtalCal_Clear bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_clear__XtalCal_Clear;


/* TDA18204 Register IRQ_set 0x0B */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_set;
/* IRQ_Set bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__IRQ_Set;
/* MSM_error_no_clock_available_Set bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__MSM_error_no_clock_available_Set;
/* MSM_error_config_Set bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__MSM_error_config_Set;
/* MSM_LOCalc_Set bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__MSM_LOCalc_Set;
/* MSM_RSSI_Set bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__MSM_RSSI_Set;
/* MSM_IRCAL_Set bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__MSM_IRCAL_Set;
/* MSM_RCCal_Set bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__MSM_RCCal_Set;
/* XtalCal_Set bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_IRQ_set__XtalCal_Set;


/* TDA18204 Register GPIO_CTRL 0x0C */
extern const TDA18204_BitField_t gTDA18204_Reg_GPIO_CTRL;
/* GPIO1 bit(s): 0 : Off  -  1 : absorb the current (then if pull up to Vcc on application => 0V) */
extern const TDA18204_BitField_t gTDA18204_Reg_GPIO_CTRL__GPIO1;


/* TDA18204 Register POWER_DOWN_1 0x0D */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1;
/* PD_REG2V8RF bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8RF;
/* PD_REG2V8IF bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8IF;
/* PD_REG2V8SYNT bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8SYNT;
/* PSM bit(s): 0 : No PSM  -  1 : PSM Bytes activated */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PSM;
/* PD_AGC3 bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_AGC3;
/* PD_AGC2_NB bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_AGC2_NB;
/* PD_AGC2 bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_AGC2;
/* PD_AGC1 bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_1__PD_AGC1;


/* TDA18204 Register POWER_DOWN_2 0x0E */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2;
/* PD_H3H5 bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_H3H5;
/* PD_MTO_BUF1 bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_MTO_BUF1;
/* PD_LT bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_LT;
/* PD_DETECT3_NB bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_DETECT3_NB;
/* PD_DETECT2_NB bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2_NB;
/* PD_DETECT2 bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2;
/* PD_DETECT1 bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_2__PD_DETECT1;


/* TDA18204 Register POWER_DOWN_3 0x0F */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3;
/* PD_VCO_LOW bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__PD_VCO_LOW;
/* PD_VCO_HIGH bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__PD_VCO_HIGH;
/* PD_VCO_EXT bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__PD_VCO_EXT;
/* AGCK_NB_DIGCLK_ON bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__AGCK_NB_DIGCLK_ON;
/* AGC3_NB_DIGCLK_ON bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__AGC3_NB_DIGCLK_ON;
/* AGC2_NB_DIGCLK_ON bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__AGC2_NB_DIGCLK_ON;
/* AGC2_DIGCLK_ON bit(s): 0: Clock of the AGC2 internal strategy off . . 1: Clock of the AGC2 internal strategy on */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__AGC2_DIGCLK_ON;
/* AGC1_DIGCLK_ON bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_3__AGC1_DIGCLK_ON;


/* TDA18204 Register POWER_DOWN_4 0x10 */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4;
/* PD_STOB_NB bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_STOB_NB;
/* PD_LPF_IFAGC bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_LPF_IFAGC;
/* PD_MIX bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_MIX;
/* PD_S2B_RFPPF_IFPPF bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_S2B_RFPPF_IFPPF;
/* PD_GPIO bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_GPIO;
/* PD_STOB bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_STOB;
/* PD_AAF_BUF2 bit(s): 0 : buf2 ON  -  1 : buf2 OFF */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_AAF_BUF2;
/* PD_TEQ_BUF bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_4__PD_TEQ_BUF;


/* TDA18204 Register POWER_DOWN_5 0x11 */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5;
/* pd_lochain bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain;
/* pd_lochain_calmode bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_calmode;
/* pd_lochain_div1p5 bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div1p5;
/* pd_lochain_div2a1 bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2a1;
/* pd_lochain_div2b bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2b;
/* pd_lochain_div2c bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2c;
/* pd_lochain_div2d bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2d;
/* pd_lochain_div2e bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2e;


/* TDA18204 Register POWER_DOWN_6 0x12 */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_6;
/* pd_xtal_div1p5 bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_6__pd_xtal_div1p5;
/* pd_pll bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_6__pd_pll;
/* pd_maindivcmos bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_6__pd_maindivcmos;
/* pd_md_counter bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_6__pd_md_counter;


/* TDA18204 Register POWER_DOWN_7 0x13 */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_7;
/* pd_calgen bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_7__pd_calgen;
/* pd_xtout bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_7__pd_xtout;
/* pd_xtal_mux bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_7__pd_xtal_mux;
/* pd_xtout_asym bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_7__pd_xtout_asym;
/* pd_sublo bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_DOWN_7__pd_sublo;


typedef enum _TDA18204_lochain_mux_div2d_Item_t {
    TDA18204_lochain_mux_div2d_DIVIDE_BY_32_OR_48_VALUE_27 = 27,		/*  Divide by 32 or 48  */
    TDA18204_lochain_mux_div2d_DIVIDE_BY_16_OR_24_MINUS_CALIBRATION_110MHZ_VALUE_43 = 43,		/*  Divide by 16 or 24 - calibration 110MHz  */
    TDA18204_lochain_mux_div2d_CALIBRATION_440MHZ_VALUE_46 = 46,		/*  calibration 440MHz  */
    TDA18204_lochain_mux_div2d_DIVIDE_BY_8_OR_12_VALUE_51 = 51,		/*  Divide by 8 or 12  */
    TDA18204_lochain_mux_div2d_DIVIDE_BY_4_OR_6_VALUE_53 = 53,		/*  Divide by 4 or 6  */
    TDA18204_lochain_mux_div2d_DIVIDE_BY_2_OR_3_MINUS_CALIBRATION_880MHZ_VALUE_54 = 54,		/*  Divide by 2 or 3 - calibration 880MHz  */
    TDA18204_lochain_mux_div2d_SLEEP_MODE_VALUE_63 = 63		/*  Sleep mode  */
} TDA18204_lochain_mux_div2d_t, *pTDA18204_lochain_mux_div2d_t;

/* TDA18204 Register LO_CHAIN_MUX_DIV2D 0x14 */
extern const TDA18204_BitField_t gTDA18204_Reg_LO_CHAIN_MUX_DIV2D;
/* lochain_mux_div2d bit(s): .63d. : Sleep. .54d. : Divide by 2. .54d. : Divide by 3. .53d. : Divide by 4. .53d. : Divide by 6. .51d. : Divide by 8. .51d. : Divide by 12. .43d. : Divide by 16. .43d. : Divide by 24. .27d. : Divide by 32. .27d. : Divide by 48. -----------------------. .54d. : calibration 880MHz. .46d. : calibration 440MHz. .43d. : calibration 110MHz */
extern const TDA18204_BitField_t gTDA18204_Reg_LO_CHAIN_MUX_DIV2D__lochain_mux_div2d;


typedef enum _TDA18204_SUBLO_PRESC_Item_t {
    TDA18204_SUBLO_PRESC_0_VALUE_0 = 0,		/*  0  */
    TDA18204_SUBLO_PRESC_1_VALUE_1 = 1,		/*  1  */
    TDA18204_SUBLO_PRESC_3_VALUE_3 = 3,		/*  3  */
    TDA18204_SUBLO_PRESC_4_VALUE_4 = 4,		/*  4  */
    TDA18204_SUBLO_PRESC_6_VALUE_6 = 6,		/*  6  */
    TDA18204_SUBLO_PRESC_8_VALUE_8 = 8,		/*  8  */
    TDA18204_SUBLO_PRESC_10_VALUE_10 = 10,		/*  10  */
    TDA18204_SUBLO_PRESC_12_VALUE_12 = 12,		/*  12  */
    TDA18204_SUBLO_PRESC_14_VALUE_14 = 14,		/*  14  */
    TDA18204_SUBLO_PRESC_15_VALUE_15 = 15,		/*  15  */
    TDA18204_SUBLO_PRESC_16_VALUE_16 = 16,		/*  16  */
    TDA18204_SUBLO_PRESC_20_VALUE_20 = 20,		/*  20  */
    TDA18204_SUBLO_PRESC_24_VALUE_24 = 24,		/*  24  */
    TDA18204_SUBLO_PRESC_27_VALUE_27 = 27,		/*  27  */
    TDA18204_SUBLO_PRESC_29_VALUE_29 = 29		/*  29  */
} TDA18204_SUBLO_PRESC_t, *pTDA18204_SUBLO_PRESC_t;

/* TDA18204 Register SUBLO_PRESC 0x15 */
extern const TDA18204_BitField_t gTDA18204_Reg_SUBLO_PRESC;
/* SUBLO_PRESC bit(s): Value of sublo swallow divider */
extern const TDA18204_BitField_t gTDA18204_Reg_SUBLO_PRESC__SUBLO_PRESC;


typedef enum _TDA18204_SUBLO_BYPASS_Item_t {
    TDA18204_SUBLO_BYPASS_0_VALUE_0 = 0,		/*  0  */
    TDA18204_SUBLO_BYPASS_1_VALUE_1 = 1,		/*  1  */
    TDA18204_SUBLO_BYPASS_3_VALUE_3 = 3,		/*  3  */
    TDA18204_SUBLO_BYPASS_7_VALUE_7 = 7,		/*  7  */
    TDA18204_SUBLO_BYPASS_15_VALUE_15 = 15		/*  15  */
} TDA18204_SUBLO_BYPASS_t, *pTDA18204_SUBLO_BYPASS_t;

/* TDA18204 Register SUBLO_BYPASS 0x16 */
extern const TDA18204_BitField_t gTDA18204_Reg_SUBLO_BYPASS;
/* SUBLO_BYPASS bit(s): Value of sublo divider */
extern const TDA18204_BitField_t gTDA18204_Reg_SUBLO_BYPASS__SUBLO_BYPASS;


/* TDA18204 Register RCCAL_READ 0x17 */
extern const TDA18204_BitField_t gTDA18204_Reg_RCCAL_READ;
/* RCCAL bit(s): Value of RC CAL */
extern const TDA18204_BitField_t gTDA18204_Reg_RCCAL_READ__RCCAL;


/* TDA18204 Register RCCAL_FORCE 0x18 */
extern const TDA18204_BitField_t gTDA18204_Reg_RCCAL_FORCE;
/* RCCAL_FORCE bit(s): 0 : Off  -  1: Enable to force the RCCAL value via RCCAL_BYPASS */
extern const TDA18204_BitField_t gTDA18204_Reg_RCCAL_FORCE__RCCAL_FORCE;
/* RCCAL_BYPASS_VALUE bit(s): 0: Low - RC with high value +16%. . 31: High - RC with low value -16% */
extern const TDA18204_BitField_t gTDA18204_Reg_RCCAL_FORCE__RCCAL_BYPASS_VALUE;


typedef enum _TDA18204_RCCAL_OFFSET_Item_t {
    TDA18204_RCCAL_OFFSET_2_VALUE_0 = 0,		/*  2  */
    TDA18204_RCCAL_OFFSET_1_VALUE_1 = 1,		/*  1  */
    TDA18204_RCCAL_OFFSET_0_VALUE_2 = 2,		/*  0  */
    TDA18204_RCCAL_OFFSET_MINUS1_VALUE_3 = 3		/*  -1  */
} TDA18204_RCCAL_OFFSET_t, *pTDA18204_RCCAL_OFFSET_t;

/* TDA18204 Register RCCAL_OFFSET 0x19 */
extern const TDA18204_BitField_t gTDA18204_Reg_RCCAL_OFFSET;
/* RCCAL_OFFSET bit(s): Global offset value of RCCAL */
extern const TDA18204_BitField_t gTDA18204_Reg_RCCAL_OFFSET__RCCAL_OFFSET;


typedef enum _TDA18204_AGC1_Top_Strategy_Item_t {
    TDA18204_AGC1_Top_Strategy_TOP_CONSTANT_HYST_4DB_VALUE_0 = 0,		/*  TOP constant hyst : 4dB  */
    TDA18204_AGC1_Top_Strategy_TOP_CONSTANT_HYST_5DB_VALUE_1 = 1,		/*  TOP constant hyst : 5dB  */
    TDA18204_AGC1_Top_Strategy_TOP_STEP_5DB_GAIN_1DB_HYST_4DB_VALUE_2 = 2,		/*  TOP step 5dB; gain 1dB hyst : 4dB  */
    TDA18204_AGC1_Top_Strategy_TOP_STEP_5DB_GAIN_1DB_HYST_5DB_VALUE_3 = 3,		/*  TOP step 5dB; gain 1dB hyst : 5dB  */
    TDA18204_AGC1_Top_Strategy_TOP_STEP_3DB_GAIN_1DB_HYST_4DB_VALUE_4 = 4,		/*  TOP step 3dB; gain 1dB hyst : 4dB  */
    TDA18204_AGC1_Top_Strategy_TOP_STEP_3DB_GAIN_1DB_HYST_5DB_VALUE_5 = 5,		/*  TOP step 3dB; gain 1dB hyst : 5dB  */
    TDA18204_AGC1_Top_Strategy_TOP_STEP_3DB_GAIN_7DB_HYST_4DB_VALUE_6 = 6,		/*  TOP step 3dB; gain 7dB hyst : 4dB  */
    TDA18204_AGC1_Top_Strategy_TOP_STEP_3DB_GAIN_7DB_HYST_5DB_VALUE_7 = 7		/*  TOP step 3dB; gain 7dB hyst : 5dB  */
} TDA18204_AGC1_Top_Strategy_t, *pTDA18204_AGC1_Top_Strategy_t;

typedef enum _TDA18204_AGC1_Top_Item_t {
    TDA18204_AGC1_Top_MIN_94_91_VALUE_0 = 0,		/*  Min (94/91)  */
    TDA18204_AGC1_Top_MAX_MINUS6DB_97_91_VALUE_1 = 1,		/*  Max -6dB (97/91)  */
    TDA18204_AGC1_Top_MAX_MINUS5DB_98_92_VALUE_2 = 2,		/*  Max -5dB (98/92)  */
    TDA18204_AGC1_Top_MAX_MINUS4DB_99_93_VALUE_3 = 3,		/*  Max -4dB (99/93)  */
    TDA18204_AGC1_Top_MAX_MINUS3DB_100_94_VALUE_4 = 4,		/*  Max -3dB (100/94)  */
    TDA18204_AGC1_Top_MAX_MINUS2DB_101_95_VALUE_5 = 5,		/*  Max -2dB (101/95)  */
    TDA18204_AGC1_Top_MAX_MINUS1DB_102_96_VALUE_6 = 6,		/*  Max -1dB (102/96)  */
    TDA18204_AGC1_Top_MAX_103_97_VALUE_7 = 7		/*  Max (103/97)  */
} TDA18204_AGC1_Top_t, *pTDA18204_AGC1_Top_t;

/* TDA18204 Register AGC1_byte_1 0x1B */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_1;
/* AGC1_Top_Strategy bit(s): Set the AGC1  TOP strategy  */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_1__AGC1_Top_Strategy;
/* AGC1_Det_Speed bit(s): change detector speed */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_1__AGC1_Det_Speed;
/* AGC1_Top bit(s): Set TOP of AGC1 (link to AGC1_Top_Strategy[2:0]) */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_1__AGC1_Top;


typedef enum _TDA18204_LNA_Range_Item_t {
    TDA18204_LNA_Range_RANGE_MINUS11DB_PLUS22DB_VALUE_0 = 0,		/*  range -11dB +22dB  */
    TDA18204_LNA_Range_RANGE_MINUS8DB_PLUS22DB_VALUE_1 = 1,		/*  range -8dB +22dB  */
    TDA18204_LNA_Range_RANGE_7DB_PLUS22DB_VALUE_2 = 2,		/*  range 7dB +22dB  */
    TDA18204_LNA_Range_RANGE_MINUS8DB_PLUS19DB_VALUE_3 = 3		/*  range -8dB +19dB  */
} TDA18204_LNA_Range_t, *pTDA18204_LNA_Range_t;

/* TDA18204 Register AGC1_byte_2 0x1C */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_2;
/* LNA_Zin bit(s): 1: On  -  0: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_2__LNA_Zin;
/* LNA_Range bit(s): change LNA gain Range */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_2__LNA_Range;


typedef enum _TDA18204_AGC1_Gain_Item_t {
    TDA18204_AGC1_Gain_MINUS11_VALUE_0 = 0,		/*  -11  */
    TDA18204_AGC1_Gain_MINUS8_VALUE_1 = 1,		/*  -8  */
    TDA18204_AGC1_Gain_MINUS5_VALUE_2 = 2,		/*  -5  */
    TDA18204_AGC1_Gain_MINUS2_VALUE_3 = 3,		/*  -2  */
    TDA18204_AGC1_Gain_1_VALUE_4 = 4,		/*  1  */
    TDA18204_AGC1_Gain_4_VALUE_5 = 5,		/*  4  */
    TDA18204_AGC1_Gain_7_VALUE_6 = 6,		/*  7  */
    TDA18204_AGC1_Gain_10_VALUE_7 = 7,		/*  10  */
    TDA18204_AGC1_Gain_13_VALUE_8 = 8,		/*  13  */
    TDA18204_AGC1_Gain_16_VALUE_9 = 9,		/*  16  */
    TDA18204_AGC1_Gain_19_VALUE_10 = 10,		/*  19  */
    TDA18204_AGC1_Gain_22_VALUE_11 = 11		/*  22  */
} TDA18204_AGC1_Gain_t, *pTDA18204_AGC1_Gain_t;

/* TDA18204 Register AGC1_byte_3 0x1D */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_3;
/* Force_AGC1_gain bit(s): 0 : Off  -  1 : Force AGC1 gain */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_3__Force_AGC1_gain;
/* AGC1_Gain bit(s): Field to force the AGC1 Gain */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_3__AGC1_Gain;


typedef enum _TDA18204_AGC1_smooth_t_cst_Item_t {
    TDA18204_AGC1_smooth_t_cst_NO_SMOOTH_TRANSITION_VALUE_0 = 0,		/*  No smooth transition  */
    TDA18204_AGC1_smooth_t_cst_1_VALUE_1 = 1,		/*  1  */
    TDA18204_AGC1_smooth_t_cst_2_VALUE_2 = 2,		/*  2  */
    TDA18204_AGC1_smooth_t_cst_4_VALUE_3 = 3,		/*  4  */
    TDA18204_AGC1_smooth_t_cst_8_VALUE_4 = 4,		/*  8  */
    TDA18204_AGC1_smooth_t_cst_16_VALUE_5 = 5,		/*  16  */
    TDA18204_AGC1_smooth_t_cst_32_VALUE_6 = 6,		/*  32  */
    TDA18204_AGC1_smooth_t_cst_200_VALUE_7 = 7		/*  200  */
} TDA18204_AGC1_smooth_t_cst_t, *pTDA18204_AGC1_smooth_t_cst_t;

/* TDA18204 Register AGC1_byte_4 0x1E */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_4;
/* AGC1_smooth_t_cst bit(s): Fix the time constant for the AGC1 switch control */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_byte_4__AGC1_smooth_t_cst;


typedef enum _TDA18204_LT_smooth_t_cst_Item_t {
    TDA18204_LT_smooth_t_cst_NO_SMOOTH_TRANSITION_VALUE_0 = 0,		/*  No smooth transition  */
    TDA18204_LT_smooth_t_cst_1_VALUE_1 = 1,		/*  1  */
    TDA18204_LT_smooth_t_cst_2_VALUE_2 = 2,		/*  2  */
    TDA18204_LT_smooth_t_cst_4_VALUE_3 = 3,		/*  4  */
    TDA18204_LT_smooth_t_cst_8_VALUE_4 = 4,		/*  8  */
    TDA18204_LT_smooth_t_cst_16_VALUE_5 = 5,		/*  16  */
    TDA18204_LT_smooth_t_cst_32_VALUE_6 = 6,		/*  32  */
    TDA18204_LT_smooth_t_cst_200_VALUE_7 = 7		/*  200  */
} TDA18204_LT_smooth_t_cst_t, *pTDA18204_LT_smooth_t_cst_t;

/* TDA18204 Register LT_byte_2 0x20 */
extern const TDA18204_BitField_t gTDA18204_Reg_LT_byte_2;
/* RFsw_LT_RFin bit(s): Switch Buf1 to LTO/MTO : 0 : Openned  -  1 : Closed  */
extern const TDA18204_BitField_t gTDA18204_Reg_LT_byte_2__RFsw_LT_RFin;
/* LT_smooth_t_cst bit(s): Fix the time constant for the Lt switch control */
extern const TDA18204_BitField_t gTDA18204_Reg_LT_byte_2__LT_smooth_t_cst;


typedef enum _TDA18204_AGC1_gain_read_Item_t {
    TDA18204_AGC1_gain_read_MINUS11_VALUE_0 = 0,		/*  -11  */
    TDA18204_AGC1_gain_read_MINUS8_VALUE_1 = 1,		/*  -8  */
    TDA18204_AGC1_gain_read_MINUS5_VALUE_2 = 2,		/*  -5  */
    TDA18204_AGC1_gain_read_MINUS2_VALUE_3 = 3,		/*  -2  */
    TDA18204_AGC1_gain_read_1_VALUE_4 = 4,		/*  1  */
    TDA18204_AGC1_gain_read_4_VALUE_5 = 5,		/*  4  */
    TDA18204_AGC1_gain_read_7_VALUE_6 = 6,		/*  7  */
    TDA18204_AGC1_gain_read_10_VALUE_7 = 7,		/*  10  */
    TDA18204_AGC1_gain_read_13_VALUE_8 = 8,		/*  13  */
    TDA18204_AGC1_gain_read_16_VALUE_9 = 9,		/*  16  */
    TDA18204_AGC1_gain_read_19_VALUE_10 = 10,		/*  19  */
    TDA18204_AGC1_gain_read_22_VALUE_11 = 11		/*  22  */
} TDA18204_AGC1_gain_read_t, *pTDA18204_AGC1_gain_read_t;

typedef enum _TDA18204_LT_Gain_Read_Item_t {
    TDA18204_LT_Gain_Read_MINUS4_5_VALUE_0 = 0,		/*  -4.5  */
    TDA18204_LT_Gain_Read_MINUS4_5_VALUE_1 = 1,		/*  -4.5  */
    TDA18204_LT_Gain_Read_MINUS4_5_VALUE_2 = 2,		/*  -4.5  */
    TDA18204_LT_Gain_Read_MINUS7_5_VALUE_3 = 3,		/*  -7.5  */
    TDA18204_LT_Gain_Read_MINUS10_5_VALUE_4 = 4,		/*  -10.5  */
    TDA18204_LT_Gain_Read_MINUS13_5_VALUE_5 = 5,		/*  -13.5  */
    TDA18204_LT_Gain_Read_MINUS16_5_VALUE_6 = 6,		/*  -16.5  */
    TDA18204_LT_Gain_Read_MINUS19_5_VALUE_7 = 7		/*  -19.5  */
} TDA18204_LT_Gain_Read_t, *pTDA18204_LT_Gain_Read_t;

/* TDA18204 Register AGC1_LT_Gain_status 0x21 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_LT_Gain_status;
/* AGC1_Smooth_Transition_ON bit(s): enable smoth transition 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_LT_Gain_status__AGC1_Smooth_Transition_ON;
/* AGC1_gain_read bit(s): Value of the AGC1 gain loop detection (not forced value) */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_LT_Gain_status__AGC1_gain_read;
/* LT_Gain_Read bit(s): Gain given without offset */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC1_LT_Gain_status__LT_Gain_Read;


/* TDA18204 Register AGC2_CONTROL 0x22 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_CONTROL;
/* AGC2_EXT_CTRL bit(s): 0 : Internal AGC strategy control  -  1 : External AGC strategy control */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_CONTROL__AGC2_EXT_CTRL;
/* AGC2_BYPASS bit(s): 0 : Off  -  1 : Bypass AGC2 internal strategy */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_CONTROL__AGC2_BYPASS;
/* AGC2_GAIN_FORCE bit(s): 0 : Off  -  1: Force AGC2 gain via AGC2_GAIN */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_CONTROL__AGC2_GAIN_FORCE;


typedef enum _TDA18204_AGC2_SPEED_Item_t {
    TDA18204_AGC2_SPEED_36_VALUE_0 = 0,		/*  36  */
    TDA18204_AGC2_SPEED_72_DEF_VALUE_1 = 1,		/*  72 (def)  */
    TDA18204_AGC2_SPEED_144_VALUE_2 = 2,		/*  144  */
    TDA18204_AGC2_SPEED_288_VALUE_3 = 3		/*  288  */
} TDA18204_AGC2_SPEED_t, *pTDA18204_AGC2_SPEED_t;

/* TDA18204 Register AGC2_SPEED 0x23 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_SPEED;
/* AGC2_LOOP_OFF bit(s): 0 : Off  -  1: Freeze AGC2 gain */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_SPEED__AGC2_LOOP_OFF;
/* AGC2_SPEED_DOWN bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_SPEED__AGC2_SPEED_DOWN;
/* AGC2_SPEED_UP bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_SPEED__AGC2_SPEED_UP;
/* AGC2_SPEED bit(s): Select the clock of the AGC2 digital control  */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_SPEED__AGC2_SPEED;


typedef enum _TDA18204_AGC2_DOWN_STEP_Item_t {
    TDA18204_AGC2_DOWN_STEP_3_42_VALUE_0 = 0,		/*  3.42  */
    TDA18204_AGC2_DOWN_STEP_1_7_VALUE_1 = 1,		/*  1.7  */
    TDA18204_AGC2_DOWN_STEP_0_85_VALUE_2 = 2,		/*  0.85  */
    TDA18204_AGC2_DOWN_STEP_0_42_VALUE_3 = 3,		/*  0.42  */
    TDA18204_AGC2_DOWN_STEP_0_2_VALUE_4 = 4,		/*  0.2  */
    TDA18204_AGC2_DOWN_STEP_0_09_VALUE_5 = 5,		/*  0.09  */
    TDA18204_AGC2_DOWN_STEP_0_04_VALUE_6 = 6		/*  0.04  */
} TDA18204_AGC2_DOWN_STEP_t, *pTDA18204_AGC2_DOWN_STEP_t;

typedef enum _TDA18204_AGC2_UP_STEP_Item_t {
    TDA18204_AGC2_UP_STEP_219_93_VALUE_0 = 0,		/*  219.93  */
    TDA18204_AGC2_UP_STEP_109_96_VALUE_1 = 1,		/*  109.96  */
    TDA18204_AGC2_UP_STEP_54_97_VALUE_2 = 2,		/*  54.97  */
    TDA18204_AGC2_UP_STEP_27_48_VALUE_3 = 3,		/*  27.48  */
    TDA18204_AGC2_UP_STEP_13_73_VALUE_4 = 4,		/*  13.73  */
    TDA18204_AGC2_UP_STEP_6_86_VALUE_5 = 5,		/*  6.86  */
    TDA18204_AGC2_UP_STEP_3_42_VALUE_6 = 6,		/*  3.42  */
    TDA18204_AGC2_UP_STEP_1_7_VALUE_7 = 7,		/*  1.7  */
    TDA18204_AGC2_UP_STEP_0_85_VALUE_8 = 8,		/*  0.85  */
    TDA18204_AGC2_UP_STEP_0_41_VALUE_9 = 9,		/*  0.41  */
    TDA18204_AGC2_UP_STEP_0_2_VALUE_10 = 10,		/*  0.2  */
    TDA18204_AGC2_UP_STEP_0_09_VALUE_11 = 11,		/*  0.09  */
    TDA18204_AGC2_UP_STEP_0_04_VALUE_12 = 12		/*  0.04  */
} TDA18204_AGC2_UP_STEP_t, *pTDA18204_AGC2_UP_STEP_t;

/* TDA18204 Register AGC2_STEP 0x24 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_STEP;
/* AGC2_DOWN_STEP bit(s): With 74.5 kHz clock */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_STEP__AGC2_DOWN_STEP;
/* AGC2_UP_STEP bit(s): Enables to change the averaging time constant of the up path accumulator of AGC2. The higher it is, the faster is the averaging (with 74kHz clock) */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_STEP__AGC2_UP_STEP;


/* TDA18204 Register AGC2_G_UP 0x25 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_G_UP;
/* AGC2_G_UP bit(s): TOP up AGC2  :  AGC2_G_UP (dBuVrms) = 103.8  -  0.5 x AGC2_G_UP */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_G_UP__AGC2_G_UP;


/* TDA18204 Register AGC2_G_DOWN 0x26 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_G_DOWN;
/* AGC2_G_DOWN bit(s): TOP down AGC2  :  AGC2_G_DOWN (dBuVrms) = 103.8  -  0.5 x AGC2_G_DOWN */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_G_DOWN__AGC2_G_DOWN;


/* TDA18204 Register AGC2_GAIN 0x27 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_GAIN;
/* AGC2_GAIN bit(s): Field to force the AGC2 Gain  :  AGC2_GAIN (dB) = -12  +  0.2 x AGC2_GAIN */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_GAIN__AGC2_GAIN;


/* TDA18204 Register AGC2_GAIN_STATUS 0x29 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_GAIN_STATUS;
/* AGC2_GAIN_READ bit(s): Value of the AGC2 gain  :  AGC2_GAIN_READ (dB) = -12  +  0.2 x AGC2_GAIN_READ */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_GAIN_STATUS__AGC2_GAIN_READ;


typedef enum _TDA18204_AGC2_NB_Up_step_Item_t {
    TDA18204_AGC2_NB_Up_step_262_VALUE_0 = 0,		/*  262  */
    TDA18204_AGC2_NB_Up_step_131_VALUE_1 = 1,		/*  131  */
    TDA18204_AGC2_NB_Up_step_66_VALUE_2 = 2,		/*  66  */
    TDA18204_AGC2_NB_Up_step_66_VALUE_3 = 3		/*  66  */
} TDA18204_AGC2_NB_Up_step_t, *pTDA18204_AGC2_NB_Up_step_t;

typedef enum _TDA18204_AGC2_NB_Do_step_Item_t {
    TDA18204_AGC2_NB_Do_step_8_192_VALUE_0 = 0,		/*  8.192  */
    TDA18204_AGC2_NB_Do_step_2_048_VALUE_1 = 1,		/*  2.048  */
    TDA18204_AGC2_NB_Do_step_1_024_VALUE_2 = 2,		/*  1.024  */
    TDA18204_AGC2_NB_Do_step_0_512_VALUE_3 = 3		/*  0.512  */
} TDA18204_AGC2_NB_Do_step_t, *pTDA18204_AGC2_NB_Do_step_t;

/* TDA18204 Register AGC2_NB_byte_1 0x2A */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_1;
/* Force_AGC2_NB_gain bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_1__Force_AGC2_NB_gain;
/* AGC2_NB_Up_step bit(s): Enables to change the averaging time constant of the up path accumulator of AGC2. The higher it is, the faster is the averaging */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_1__AGC2_NB_Up_step;
/* AGC2_NB_Do_step bit(s): Enables to change the averaging time constant of the down path accumulator of AGC2. The higher it is, the faster is the averaging */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_1__AGC2_NB_Do_step;


/* TDA18204 Register AGC2_NB_byte_2 0x2B */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_2;
/* AGC2_NB_loop_off bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_loop_off;
/* AGC2_NB_TOP_DO bit(s): TOP down AGC2  :  AGC2_G_DOWN (dBuVrms) = 103.8  -  0.5 x AGC2_G_DOWN */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_TOP_DO;


/* TDA18204 Register AGC2_NB_byte_3 0x2C */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_3;
/* AGC2_NB_Det_Speed bit(s): Select the clock of the AGC2 digital control  */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_3__AGC2_NB_Det_Speed;
/* AGC2_Gain_Control_En bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_3__AGC2_Gain_Control_En;
/* AGC2_NB_TOP_UP bit(s): TOP up AGC2  :  AGC2_G_UP (dBuVrms) = 103.8  -  0.5 x AGC2_G_UP */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_3__AGC2_NB_TOP_UP;


/* TDA18204 Register AGC2_NB_byte_4 0x2D */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_4;
/* AGC2_NB_GAIN bit(s): Field to force the AGC2 Gain  :  AGC2_GAIN (dB) = -12  +  0.2 x AGC2_GAIN */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_4__AGC2_NB_GAIN;


/* TDA18204 Register AGC2_NB_byte_5 0x2E */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_5;
/* AGC2_NB_HP_EN bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_byte_5__AGC2_NB_HP_EN;


/* TDA18204 Register AGC2_NB_Gain_status 0x2F */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_Gain_status;
/* AGC2_NB_GAIN_READ bit(s): Value of the AGC2 gain  :  AGC2_GAIN_READ (dB) = -12  +  0.2 x AGC2_GAIN_READ */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC2_NB_Gain_status__AGC2_NB_GAIN_READ;


typedef enum _TDA18204_DET12_Cint_Item_t {
    TDA18204_DET12_Cint_50_VALUE_0 = 0,		/*  50  */
    TDA18204_DET12_Cint_100_VALUE_1 = 1,		/*  100  */
    TDA18204_DET12_Cint_150_VALUE_2 = 2,		/*  150  */
    TDA18204_DET12_Cint_200_VALUE_3 = 3		/*  200  */
} TDA18204_DET12_Cint_t, *pTDA18204_DET12_Cint_t;

typedef enum _TDA18204_AGCK_Step_Item_t {
    TDA18204_AGCK_Step_0_2_VALUE_0 = 0,		/*  0.2  */
    TDA18204_AGCK_Step_0_4_VALUE_1 = 1,		/*  0.4  */
    TDA18204_AGCK_Step_0_6_VALUE_2 = 2,		/*  0.6  */
    TDA18204_AGCK_Step_0_8_VALUE_3 = 3		/*  0.8  */
} TDA18204_AGCK_Step_t, *pTDA18204_AGCK_Step_t;

typedef enum _TDA18204_AGCK_Mode_Item_t {
    TDA18204_AGCK_Mode_FORBIDDEN_VALUE_0 = 0,		/*  forbidden  */
    TDA18204_AGCK_Mode_0_512_VALUE_1 = 1,		/*  0.512  */
    TDA18204_AGCK_Mode_8_192_VALUE_2 = 2,		/*  8.192  */
    TDA18204_AGCK_Mode_32_768_DEF_VALUE_3 = 3		/*  32.768 (def)  */
} TDA18204_AGCK_Mode_t, *pTDA18204_AGCK_Mode_t;

/* TDA18204 Register AGCK_byte_1 0x30 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGCK_byte_1;
/* DET12_Cint bit(s): change analog part of detector speed */
extern const TDA18204_BitField_t gTDA18204_Reg_AGCK_byte_1__DET12_Cint;
/* AGCK_Step bit(s): AGCK gain step */
extern const TDA18204_BitField_t gTDA18204_Reg_AGCK_byte_1__AGCK_Step;
/* AGCK_Mode bit(s): AGCK time constant */
extern const TDA18204_BitField_t gTDA18204_Reg_AGCK_byte_1__AGCK_Mode;


/* TDA18204 Register AGC3_NB_byte_1 0x31 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_1;
/* AGC3_NB_TOP_DO bit(s): TOP down AGC3  :  AGC3_G_DOWN (dBuVrms) = 117  -  AGC3_G_DOWN */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_1__AGC3_NB_TOP_DO;
/* AGC3_NB_TOP_UP bit(s): TOP up AGC3  :  AGC3_G_UP (dBuVrms) = 117  -  AGC3_G_UP */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_1__AGC3_NB_TOP_UP;


typedef enum _TDA18204_AGC3_NB_GAIN_Item_t {
    TDA18204_AGC3_NB_GAIN_MINUS6_VALUE_0 = 0,		/*  -6  */
    TDA18204_AGC3_NB_GAIN_MINUS3_VALUE_1 = 1,		/*  -3  */
    TDA18204_AGC3_NB_GAIN_0_VALUE_2 = 2,		/*  0  */
    TDA18204_AGC3_NB_GAIN_3_VALUE_3 = 3,		/*  3  */
    TDA18204_AGC3_NB_GAIN_6_VALUE_4 = 4,		/*  6  */
    TDA18204_AGC3_NB_GAIN_9_VALUE_5 = 5,		/*  9  */
    TDA18204_AGC3_NB_GAIN_12_VALUE_6 = 6,		/*  12  */
    TDA18204_AGC3_NB_GAIN_15_VALUE_7 = 7		/*  15  */
} TDA18204_AGC3_NB_GAIN_t, *pTDA18204_AGC3_NB_GAIN_t;

/* TDA18204 Register AGC3_NB_byte_2 0x32 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_2;
/* AGC3_NB_loop_off bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_2__AGC3_NB_loop_off;
/* Force_AGC3_NB_gain bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_2__Force_AGC3_NB_gain;
/* AGC3_NB_GAIN bit(s): Gain setting of the AGC3_NB (if Force_AGC3_NB_gain=.1.) */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_2__AGC3_NB_GAIN;


typedef enum _TDA18204_RSSI_HP_Fc_Item_t {
    TDA18204_RSSI_HP_Fc_0_3_VALUE_0 = 0,		/*  0.3  */
    TDA18204_RSSI_HP_Fc_2_25_VALUE_1 = 1,		/*  2.25  */
    TDA18204_RSSI_HP_Fc_5_VALUE_2 = 2,		/*  5  */
    TDA18204_RSSI_HP_Fc_5_VALUE_3 = 3		/*  5  */
} TDA18204_RSSI_HP_Fc_t, *pTDA18204_RSSI_HP_Fc_t;

typedef enum _TDA18204_AGC3_NB_Do_step_Item_t {
    TDA18204_AGC3_NB_Do_step_8_176_VALUE_0 = 0,		/*  8.176  */
    TDA18204_AGC3_NB_Do_step_2_044_VALUE_1 = 1,		/*  2.044  */
    TDA18204_AGC3_NB_Do_step_1_022_VALUE_2 = 2,		/*  1.022  */
    TDA18204_AGC3_NB_Do_step_0_512_VALUE_3 = 3		/*  0.512  */
} TDA18204_AGC3_NB_Do_step_t, *pTDA18204_AGC3_NB_Do_step_t;

typedef enum _TDA18204_AGC3_NB_Up_step_Item_t {
    TDA18204_AGC3_NB_Up_step_262_VALUE_0 = 0,		/*  262  */
    TDA18204_AGC3_NB_Up_step_131_VALUE_1 = 1,		/*  131  */
    TDA18204_AGC3_NB_Up_step_66_VALUE_2 = 2,		/*  66  */
    TDA18204_AGC3_NB_Up_step_66_VALUE_3 = 3		/*  66  */
} TDA18204_AGC3_NB_Up_step_t, *pTDA18204_AGC3_NB_Up_step_t;

/* TDA18204 Register AGC3_NB_byte_3 0x33 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_3;
/* DET3_FastAtt bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_3__DET3_FastAtt;
/* DET3_LowBW bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_3__DET3_LowBW;
/* RSSI_HP_Fc bit(s): change RSSI cut-off frequency */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_3__RSSI_HP_Fc;
/* AGC3_NB_Do_step bit(s): Enables to change the averaging time constant of the down path accumulator of AGC3_NB. The higher it is, the faster is the averaging */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_3__AGC3_NB_Do_step;
/* AGC3_NB_Up_step bit(s): time constant for up step */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_byte_3__AGC3_NB_Up_step;


typedef enum _TDA18204_AGC3_NB_GAIN_READ_Item_t {
    TDA18204_AGC3_NB_GAIN_READ_MINUS6_VALUE_0 = 0,		/*  -6  */
    TDA18204_AGC3_NB_GAIN_READ_MINUS3_VALUE_1 = 1,		/*  -3  */
    TDA18204_AGC3_NB_GAIN_READ_0_VALUE_2 = 2,		/*  0  */
    TDA18204_AGC3_NB_GAIN_READ_3_VALUE_3 = 3,		/*  3  */
    TDA18204_AGC3_NB_GAIN_READ_6_VALUE_4 = 4,		/*  6  */
    TDA18204_AGC3_NB_GAIN_READ_9_VALUE_5 = 5,		/*  9  */
    TDA18204_AGC3_NB_GAIN_READ_12_VALUE_6 = 6,		/*  12  */
    TDA18204_AGC3_NB_GAIN_READ_15_VALUE_7 = 7		/*  15  */
} TDA18204_AGC3_NB_GAIN_READ_t, *pTDA18204_AGC3_NB_GAIN_READ_t;

/* TDA18204 Register AGC3_NB_gain_status 0x34 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_gain_status;
/* RSSI_ALARM bit(s): read value */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_gain_status__RSSI_ALARM;
/* AGC3_NB_GAIN_READ bit(s): Field to read the AGC3 Gain */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_NB_gain_status__AGC3_NB_GAIN_READ;


typedef enum _TDA18204_AGC3_GAIN_Item_t {
    TDA18204_AGC3_GAIN_2_VALUE_0 = 0,		/*  2  */
    TDA18204_AGC3_GAIN_5_DEF_VALUE_2 = 2,		/*  5 (def)  */
    TDA18204_AGC3_GAIN_11_VALUE_3 = 3,		/*  11  */
    TDA18204_AGC3_GAIN_8_VALUE_4 = 4		/*  8  */
} TDA18204_AGC3_GAIN_t, *pTDA18204_AGC3_GAIN_t;

/* TDA18204 Register AGC3_GAIN_WB 0x35 */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_GAIN_WB;
/* AGC3_GAIN bit(s): Gain setting of the AGC3_WB */
extern const TDA18204_BitField_t gTDA18204_Reg_AGC3_GAIN_WB__AGC3_GAIN;


typedef enum _TDA18204_TEQ_SELECT_Item_t {
    TDA18204_TEQ_SELECT_15_VALUE_0 = 0,		/*  15  */
    TDA18204_TEQ_SELECT_10_VALUE_1 = 1,		/*  10  */
    TDA18204_TEQ_SELECT_5_VALUE_2 = 2,		/*  5  */
    TDA18204_TEQ_SELECT_3_VALUE_3 = 3,		/*  3  */
    TDA18204_TEQ_SELECT_0_DEF_VALUE_4 = 4,		/*  0 (def)  */
    TDA18204_TEQ_SELECT_MINUS5_VALUE_5 = 5,		/*  -5  */
    TDA18204_TEQ_SELECT_MINUS10_VALUE_6 = 6,		/*  -10  */
    TDA18204_TEQ_SELECT_I2C_VALUE_7 = 7		/*  I2C  */
} TDA18204_TEQ_SELECT_t, *pTDA18204_TEQ_SELECT_t;

/* TDA18204 Register TEQ_SELECT 0x36 */
extern const TDA18204_BitField_t gTDA18204_Reg_TEQ_SELECT;
/* TEQ_SELECT bit(s): slope choice:. . TEQ : +15dB, +10dB, +5dB, +3dB, bypass, -5dB, -10dB */
extern const TDA18204_BitField_t gTDA18204_Reg_TEQ_SELECT__TEQ_SELECT;


/* TDA18204 Register POWERDOWN 0x37 */
extern const TDA18204_BitField_t gTDA18204_Reg_POWERDOWN;
/* FORCE_SOFT_RESET bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_POWERDOWN__FORCE_SOFT_RESET;
/* SOFT_RESET bit(s): Reset all the digital part if FORCE_SOFT_RESET=1, . . place the state machine in idle mode. . . DOES NOT reset the I2C register */
extern const TDA18204_BitField_t gTDA18204_Reg_POWERDOWN__SOFT_RESET;


typedef enum _TDA18204_PSM_MTOBUF1_Item_t {
    TDA18204_PSM_MTOBUF1_22_VALUE_0 = 0,		/*  22  */
    TDA18204_PSM_MTOBUF1_19_5_VALUE_1 = 1,		/*  19.5  */
    TDA18204_PSM_MTOBUF1_17_VALUE_2 = 2,		/*  17  */
    TDA18204_PSM_MTOBUF1_17_VALUE_3 = 3,		/*  17  */
    TDA18204_PSM_MTOBUF1_14_5_VALUE_4 = 4,		/*  14.5  */
    TDA18204_PSM_MTOBUF1_12_VALUE_8 = 8		/*  12  */
} TDA18204_PSM_MTOBUF1_t, *pTDA18204_PSM_MTOBUF1_t;

/* TDA18204 Register POWER_SAVING_MODE_1 0x41 */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_1;
/* BOOST_VCO bit(s): 1: On  -  0: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_1__BOOST_VCO;
/* PSM_MTOBUF1 bit(s): 22 mA POR state,. . 17 mA PSM=1 state. */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_1__PSM_MTOBUF1;


typedef enum _TDA18204_PSM_AGC1_Item_t {
    TDA18204_PSM_AGC1_16_8_VALUE_0 = 0,		/*  16.8  */
    TDA18204_PSM_AGC1_21_8_VALUE_1 = 1,		/*  21.8  */
    TDA18204_PSM_AGC1_26_9_VALUE_2 = 2,		/*  26.9  */
    TDA18204_PSM_AGC1_31_8_VALUE_3 = 3		/*  31.8  */
} TDA18204_PSM_AGC1_t, *pTDA18204_PSM_AGC1_t;

typedef enum _TDA18204_PSM_AGC2_Item_t {
    TDA18204_PSM_AGC2_34_9_VALUE_0 = 0,		/*  34.9  */
    TDA18204_PSM_AGC2_28_9_VALUE_1 = 1,		/*  28.9  */
    TDA18204_PSM_AGC2_22_9_VALUE_2 = 2,		/*  22.9  */
    TDA18204_PSM_AGC2_17_4_VALUE_3 = 3		/*  17.4  */
} TDA18204_PSM_AGC2_t, *pTDA18204_PSM_AGC2_t;

typedef enum _TDA18204_PSM_AGC3_Item_t {
    TDA18204_PSM_AGC3_32_2_VALUE_0 = 0,		/*  32.2  */
    TDA18204_PSM_AGC3_26_3_VALUE_1 = 1,		/*  26.3  */
    TDA18204_PSM_AGC3_21_6_VALUE_2 = 2,		/*  21.6  */
    TDA18204_PSM_AGC3_16_4_VALUE_3 = 3		/*  16.4  */
} TDA18204_PSM_AGC3_t, *pTDA18204_PSM_AGC3_t;

/* TDA18204 Register POWER_SAVING_MODE_2 0x42 */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_2;
/* PSM_AGC1 bit(s): 31.8 mA POR state,. . 21.8 mA PSM=1 state */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_2__PSM_AGC1;
/* PSM_AGC2 bit(s): 28.9 mA POR state,. . 22.9 mA PSM=1 state */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_2__PSM_AGC2;
/* PSM_AGC3 bit(s): 32.2 mA POR state,. . 26.3 mA PSM=1 state */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_2__PSM_AGC3;


typedef enum _TDA18204_PSM_AAF_BUF2_Item_t {
    TDA18204_PSM_AAF_BUF2_11_VALUE_0 = 0,		/*  11  */
    TDA18204_PSM_AAF_BUF2_8_63_VALUE_1 = 1,		/*  8.63  */
    TDA18204_PSM_AAF_BUF2_6_22_VALUE_2 = 2,		/*  6.22  */
    TDA18204_PSM_AAF_BUF2_5_31_VALUE_4 = 4,		/*  5.31  */
    TDA18204_PSM_AAF_BUF2_4_39_VALUE_8 = 8		/*  4.39  */
} TDA18204_PSM_AAF_BUF2_t, *pTDA18204_PSM_AAF_BUF2_t;

typedef enum _TDA18204_PSM_TEQ_BUF_Item_t {
    TDA18204_PSM_TEQ_BUF_10_VALUE_0 = 0,		/*  10  */
    TDA18204_PSM_TEQ_BUF_8_6_VALUE_1 = 1,		/*  8.6  */
    TDA18204_PSM_TEQ_BUF_7_5_VALUE_2 = 2,		/*  7.5  */
    TDA18204_PSM_TEQ_BUF_6_3_VALUE_4 = 4,		/*  6.3  */
    TDA18204_PSM_TEQ_BUF_5_VALUE_8 = 8		/*  5  */
} TDA18204_PSM_TEQ_BUF_t, *pTDA18204_PSM_TEQ_BUF_t;

/* TDA18204 Register POWER_SAVING_MODE_3 0x43 */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_3;
/* PSM_AAF_BUF2 bit(s): PSM of AAF Buf2. . 6.22 mA for 1S2D, POR state, PSM=0. . 5.31 mA, PSM=1 state */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_3__PSM_AAF_BUF2;
/* PSM_TEQ_BUF bit(s): 10 mA POR state,. . 8.6 mA PSM=1 state */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_3__PSM_TEQ_BUF;


typedef enum _TDA18204_PSM_RFPPF_Item_t {
    TDA18204_PSM_RFPPF_0_VALUE_0 = 0,		/*  0  */
    TDA18204_PSM_RFPPF_MINUS4_2_VALUE_1 = 1,		/*  -4.2  */
    TDA18204_PSM_RFPPF_MINUS7_2_VALUE_2 = 2,		/*  -7.2  */
    TDA18204_PSM_RFPPF_MINUS9_3_VALUE_3 = 3		/*  -9.3  */
} TDA18204_PSM_RFPPF_t, *pTDA18204_PSM_RFPPF_t;

typedef enum _TDA18204_PSM_Mixer_Item_t {
    TDA18204_PSM_Mixer_0_VALUE_0 = 0,		/*  0  */
    TDA18204_PSM_Mixer_MINUS1_2_VALUE_1 = 1,		/*  -1.2  */
    TDA18204_PSM_Mixer_MINUS1_8_VALUE_2 = 2,		/*  -1.8  */
    TDA18204_PSM_Mixer_MINUS2_3_VALUE_3 = 3		/*  -2.3  */
} TDA18204_PSM_Mixer_t, *pTDA18204_PSM_Mixer_t;

typedef enum _TDA18204_PSM_Ifpoly_Item_t {
    TDA18204_PSM_Ifpoly_0_VALUE_0 = 0,		/*  0  */
    TDA18204_PSM_Ifpoly_MINUS1_8_VALUE_1 = 1,		/*  -1.8  */
    TDA18204_PSM_Ifpoly_MINUS3_2_VALUE_2 = 2,		/*  -3.2  */
    TDA18204_PSM_Ifpoly_MINUS4_1_VALUE_3 = 3		/*  -4.1  */
} TDA18204_PSM_Ifpoly_t, *pTDA18204_PSM_Ifpoly_t;

typedef enum _TDA18204_PSM_Lodriver_Item_t {
    TDA18204_PSM_Lodriver_0_VALUE_0 = 0,		/*  0  */
    TDA18204_PSM_Lodriver_MINUS3_VALUE_1 = 1,		/*  -3  */
    TDA18204_PSM_Lodriver_MINUS4_5_VALUE_2 = 2,		/*  -4.5  */
    TDA18204_PSM_Lodriver_MINUS6_VALUE_3 = 3		/*  -6  */
} TDA18204_PSM_Lodriver_t, *pTDA18204_PSM_Lodriver_t;

/* TDA18204 Register POWER_SAVING_MODE_4 0x44 */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_4;
/* PSM_RFPPF bit(s): .00. : 0 mA. . .01. : -4.2 mA. . .10. : -7.2 mA. . .11. : -9.3 mA */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_4__PSM_RFPPF;
/* PSM_Mixer bit(s): .00. : nominal. . .01. : -1.2mA. . .10. : -1.8mA. . .11. : -2.3mA */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_4__PSM_Mixer;
/* PSM_Ifpoly bit(s): .00. : nominal. . .01. : -1.8 mA. . .10. : -3.2 mA. . .11. : -4.1 mA */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_4__PSM_Ifpoly;
/* PSM_Lodriver bit(s): .00. : nominal. . .01. : -3.0 mA. . .10. : -4.5 mA. . .11. : -6.0 mA */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_4__PSM_Lodriver;


typedef enum _TDA18204_PSM_lpf_Item_t {
    TDA18204_PSM_lpf_0_VALUE_0 = 0,		/*  0  */
    TDA18204_PSM_lpf_MINUS2_3_VALUE_1 = 1,		/*  -2.3  */
    TDA18204_PSM_lpf_MINUS3_8_VALUE_2 = 2,		/*  -3.8  */
    TDA18204_PSM_lpf_MINUS4_9_VALUE_3 = 3		/*  -4.9  */
} TDA18204_PSM_lpf_t, *pTDA18204_PSM_lpf_t;

typedef enum _TDA18204_PSM_hpf_ifn_Item_t {
    TDA18204_PSM_hpf_ifn_0_VALUE_0 = 0,		/*  0  */
    TDA18204_PSM_hpf_ifn_MINUS1_6_VALUE_1 = 1,		/*  -1.6  */
    TDA18204_PSM_hpf_ifn_MINUS2_7_VALUE_2 = 2,		/*  -2.7  */
    TDA18204_PSM_hpf_ifn_MINUS3_4_VALUE_3 = 3		/*  -3.4  */
} TDA18204_PSM_hpf_ifn_t, *pTDA18204_PSM_hpf_ifn_t;

typedef enum _TDA18204_PSM_H35_Item_t {
    TDA18204_PSM_H35_21_3_VALUE_0 = 0,		/*  21.3  */
    TDA18204_PSM_H35_19_4_VALUE_1 = 1,		/*  19.4  */
    TDA18204_PSM_H35_17_4_VALUE_2 = 2,		/*  17.4  */
    TDA18204_PSM_H35_15_4_VALUE_3 = 3		/*  15.4  */
} TDA18204_PSM_H35_t, *pTDA18204_PSM_H35_t;

typedef enum _TDA18204_PSM_VCO_Item_t {
    TDA18204_PSM_VCO_2_LOW_OR_5_HIGH_VALUE_0 = 0,		/*  2 (low) or 5 (high)  */
    TDA18204_PSM_VCO_3_LOW_OR_6_25_HIGH_VALUE_1 = 1,		/*  3 (low) or 6.25 (high)  */
    TDA18204_PSM_VCO_3_5_LOW_OR_6_75_HIGH_VALUE_2 = 2,		/*  3.5 (low) or 6.75 (high)  */
    TDA18204_PSM_VCO_4_5_LOW_OR_8_HIGH_VALUE_3 = 3		/*  4.5 (low) or 8 (high)  */
} TDA18204_PSM_VCO_t, *pTDA18204_PSM_VCO_t;

/* TDA18204 Register POWER_SAVING_MODE_5 0x45 */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_5;
/* PSM_lpf bit(s): .00. : nominal. . .01. : -2.3 mA. . .10. : -3.8 mA. . .11. : -4.9 mA */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_lpf;
/* PSM_hpf_ifn bit(s): .00. : nominal. . .01. : -1.6 mA. . .10. : -2.7 mA. . .11. : -3.4 mA */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_hpf_ifn;
/* PSM_H35 bit(s): Power saving mode H3H5 */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_H35;
/* PSM_VCO bit(s): .00.: 2 mA (low) - 5 mA (high). . .01.: 3 mA (low) - 6.25 mA (high). . .10.: 3.5 mA (low) - 6.75 mA (high). . .11.: 4.5 mA (low) - 8 mA (high) */
extern const TDA18204_BitField_t gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_VCO;


typedef enum _TDA18204_SWITCH_STAB_CAP_Item_t {
    TDA18204_SWITCH_STAB_CAP_0_VALUE_0 = 0,		/*  0  */
    TDA18204_SWITCH_STAB_CAP_530_VALUE_1 = 1,		/*  530  */
    TDA18204_SWITCH_STAB_CAP_380_VALUE_2 = 2,		/*  380  */
    TDA18204_SWITCH_STAB_CAP_910_VALUE_3 = 3,		/*  910  */
    TDA18204_SWITCH_STAB_CAP_500_VALUE_4 = 4,		/*  500  */
    TDA18204_SWITCH_STAB_CAP_1030_VALUE_5 = 5,		/*  1030  */
    TDA18204_SWITCH_STAB_CAP_880_VALUE_6 = 6,		/*  880  */
    TDA18204_SWITCH_STAB_CAP_1410_VALUE_7 = 7,		/*  1410  */
    TDA18204_SWITCH_STAB_CAP_190_VALUE_8 = 8,		/*  190  */
    TDA18204_SWITCH_STAB_CAP_720_VALUE_9 = 9,		/*  720  */
    TDA18204_SWITCH_STAB_CAP_570_VALUE_10 = 10,		/*  570  */
    TDA18204_SWITCH_STAB_CAP_1100_VALUE_11 = 11,		/*  1100  */
    TDA18204_SWITCH_STAB_CAP_690_VALUE_12 = 12,		/*  690  */
    TDA18204_SWITCH_STAB_CAP_1220_VALUE_13 = 13,		/*  1220  */
    TDA18204_SWITCH_STAB_CAP_1070_VALUE_14 = 14,		/*  1070  */
    TDA18204_SWITCH_STAB_CAP_1600_VALUE_15 = 15,		/*  1600  */
    TDA18204_SWITCH_STAB_CAP_140_VALUE_16 = 16,		/*  140  */
    TDA18204_SWITCH_STAB_CAP_670_VALUE_17 = 17,		/*  670  */
    TDA18204_SWITCH_STAB_CAP_520_VALUE_18 = 18,		/*  520  */
    TDA18204_SWITCH_STAB_CAP_1050_VALUE_19 = 19,		/*  1050  */
    TDA18204_SWITCH_STAB_CAP_640_VALUE_20 = 20,		/*  640  */
    TDA18204_SWITCH_STAB_CAP_1170_VALUE_21 = 21,		/*  1170  */
    TDA18204_SWITCH_STAB_CAP_1020_VALUE_22 = 22,		/*  1020  */
    TDA18204_SWITCH_STAB_CAP_1550_VALUE_23 = 23,		/*  1550  */
    TDA18204_SWITCH_STAB_CAP_330_VALUE_24 = 24,		/*  330  */
    TDA18204_SWITCH_STAB_CAP_860_VALUE_25 = 25,		/*  860  */
    TDA18204_SWITCH_STAB_CAP_710_VALUE_26 = 26,		/*  710  */
    TDA18204_SWITCH_STAB_CAP_1240_VALUE_27 = 27,		/*  1240  */
    TDA18204_SWITCH_STAB_CAP_830_VALUE_28 = 28,		/*  830  */
    TDA18204_SWITCH_STAB_CAP_1360_VALUE_29 = 29,		/*  1360  */
    TDA18204_SWITCH_STAB_CAP_1210_VALUE_30 = 30,		/*  1210  */
    TDA18204_SWITCH_STAB_CAP_1740_VALUE_31 = 31		/*  1740  */
} TDA18204_SWITCH_STAB_CAP_t, *pTDA18204_SWITCH_STAB_CAP_t;

/* TDA18204 Register H3H5_byte 0x47 */
extern const TDA18204_BitField_t gTDA18204_Reg_H3H5_byte;
/* SWITCH_STAB_CAP bit(s): Select switching stability capacitor :. 00000: 0 fF. 00001: 530 fF. 00010: 380 fF. 00011: 910 fF. 00100: 500 fF. 00101: 1030 fF. 00110: 880 fF. 00111: 1410 fF. 01000: 190 fF. 01001: 720 fF. 01010: 570 fF. 01011: 1100 fF. 01100: 690 fF. 01101: 1220 fF. 01110: 1070 fF. 01111: 1600 fF. 10000: 140 fF. 10001: 530 fF. 10010: 380 fF. 10011: 910 fF. 10100: 500 fF. 10101: 1030 fF. 10110: 880 fF. 10111: 1410 fF. 11000: 190 fF. 11001: 720 fF. 11010: 570 fF. 11011: 1100 fF. 11100: 690 fF. 11101: 1220 fF. 11110: 1070 fF. 11111: 1600 fF */
extern const TDA18204_BitField_t gTDA18204_Reg_H3H5_byte__SWITCH_STAB_CAP;
/* PSM_H35_EXTRA_BIT bit(s): 0: PSM0 or PSM1 or PSM2  -  1: PSM3= 48.28 mA (Only available on ES2) */
extern const TDA18204_BitField_t gTDA18204_Reg_H3H5_byte__PSM_H35_EXTRA_BIT;
/* H3H5_Dcboost bit(s): 0: Off  -  1: On (Only available on ES2) */
extern const TDA18204_BitField_t gTDA18204_Reg_H3H5_byte__H3H5_Dcboost;


typedef enum _TDA18204_W_Filter_Item_t {
    TDA18204_W_Filter_WIRELESS_FILTER_NOTCH_2_4_MINUS_2_6GHZ_VALUE_1 = 1,		/*  Wireless_filter : Notch 2.4 - 2.6GHz  */
    TDA18204_W_Filter_WIRELESS_FILTER_NOTCH_1_98_MINUS_2_17GHZ_VALUE_2 = 2,		/*  Wireless_filter : Notch 1.98 - 2.17GHz  */
    TDA18204_W_Filter_WIRELESS_FILTER_NOTCH_1_7_MINUS_1_98GHZ_VALUE_3 = 3		/*  Wireless_filter : Notch 1.7 - 1.98GHz  */
} TDA18204_W_Filter_t, *pTDA18204_W_Filter_t;

typedef enum _TDA18204_W_Filter_Offset_Item_t {
    TDA18204_W_Filter_Offset_4_VALUE_0 = 0,		/*  4  */
    TDA18204_W_Filter_Offset_NOMINAL_VALUE_1 = 1,		/*  nominal  */
    TDA18204_W_Filter_Offset_MINUS4_VALUE_2 = 2,		/*  -4  */
    TDA18204_W_Filter_Offset_MINUS8_VALUE_3 = 3		/*  -8  */
} TDA18204_W_Filter_Offset_t, *pTDA18204_W_Filter_Offset_t;

/* TDA18204 Register W_Filter_byte 0x49 */
extern const TDA18204_BitField_t gTDA18204_Reg_W_Filter_byte;
/* W_Filter_Enable bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_W_Filter_byte__W_Filter_Enable;
/* W_Filter bit(s): Select wireless filter frequency */
extern const TDA18204_BitField_t gTDA18204_Reg_W_Filter_byte__W_Filter;
/* W_Filter_Offset bit(s): .00. : +4%. . .01. : nominal. . .10. : -4%. . .11. : -8% */
extern const TDA18204_BitField_t gTDA18204_Reg_W_Filter_byte__W_Filter_Offset;


typedef enum _TDA18204_RF_BPF_Item_t {
    TDA18204_RF_BPF_BP_FILTER6_244MINUS365MHZ_VALUE_0 = 0,		/*  BP_Filter6 : 244-365MHz  */
    TDA18204_RF_BPF_BP_FILTER5_173MINUS288MHZ_VALUE_16 = 16,		/*  BP_Filter5 : 173-288MHz  */
    TDA18204_RF_BPF_BP_FILTER4_123MINUS204MHZ_VALUE_24 = 24,		/*  BP_Filter4 : 123-204MHz  */
    TDA18204_RF_BPF_BP_FILTER3_87MINUS145MHZ_VALUE_28 = 28,		/*  BP_Filter3 : 87-145MHz  */
    TDA18204_RF_BPF_BP_FILTER2_62MINUS103MHZ_VALUE_30 = 30,		/*  BP_Filter2 : 62-103MHz  */
    TDA18204_RF_BPF_BP_FILTER1_44MINUS73MHZ_VALUE_31 = 31		/*  BP_Filter1 : 44-73MHz  */
} TDA18204_RF_BPF_t, *pTDA18204_RF_BPF_t;

/* TDA18204 Register RF_Band_Pass_Filter_byte 0x4A */
extern const TDA18204_BitField_t gTDA18204_Reg_RF_Band_Pass_Filter_byte;
/* RF_BPF bit(s): Select H3H5 filter frequency */
extern const TDA18204_BitField_t gTDA18204_Reg_RF_Band_Pass_Filter_byte__RF_BPF;


typedef enum _TDA18204_Mixer_GateDC_Item_t {
    TDA18204_Mixer_GateDC_2_1_VALUE_0 = 0,		/*  2.1  */
    TDA18204_Mixer_GateDC_1_9_VALUE_1 = 1,		/*  1.9  */
    TDA18204_Mixer_GateDC_1_7_VALUE_2 = 2,		/*  1.7  */
    TDA18204_Mixer_GateDC_1_5_VALUE_3 = 3		/*  1.5  */
} TDA18204_Mixer_GateDC_t, *pTDA18204_Mixer_GateDC_t;

typedef enum _TDA18204_S2D_Gain_Item_t {
    TDA18204_S2D_Gain_3_VALUE_0 = 0,		/*  3  */
    TDA18204_S2D_Gain_6_VALUE_1 = 1,		/*  6  */
    TDA18204_S2D_Gain_9_VALUE_2 = 2		/*  9  */
} TDA18204_S2D_Gain_t, *pTDA18204_S2D_Gain_t;

/* TDA18204 Register IR_Mixer_byte_1 0x4B */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Mixer_byte_1;
/* Mixer_GateDC bit(s): Improves mixer gain and linearity when psm_mixer=3 */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Mixer_byte_1__Mixer_GateDC;
/* S2D_Gain bit(s): change gain */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Mixer_byte_1__S2D_Gain;
/* DC_Notch bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Mixer_byte_1__DC_Notch;


typedef enum _TDA18204_IF_level_Item_t {
    TDA18204_IF_level_2_VALUE_0 = 0,		/*  2  */
    TDA18204_IF_level_1_25_VALUE_1 = 1,		/*  1.25  */
    TDA18204_IF_level_1_VALUE_2 = 2,		/*  1  */
    TDA18204_IF_level_0_8_VALUE_3 = 3,		/*  0.8  */
    TDA18204_IF_level_0_85_VALUE_4 = 4,		/*  0.85  */
    TDA18204_IF_level_0_7_VALUE_5 = 5,		/*  0.7  */
    TDA18204_IF_level_0_6_VALUE_6 = 6,		/*  0.6  */
    TDA18204_IF_level_0_5_VALUE_7 = 7		/*  0.5  */
} TDA18204_IF_level_t, *pTDA18204_IF_level_t;

/* TDA18204 Register IF_AGC_byte 0x4C */
extern const TDA18204_BitField_t gTDA18204_Reg_IF_AGC_byte;
/* LPF_DCOffset_Corr bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_IF_AGC_byte__LPF_DCOffset_Corr;
/* IF_level bit(s): IF output level selection */
extern const TDA18204_BitField_t gTDA18204_Reg_IF_AGC_byte__IF_level;


typedef enum _TDA18204_IF_LPF_Offset_Item_t {
    TDA18204_IF_LPF_Offset_NOMINAL_VALUE_0 = 0,		/*  Nominal  */
    TDA18204_IF_LPF_Offset_4_VALUE_1 = 1,		/*  4  */
    TDA18204_IF_LPF_Offset_8_VALUE_2 = 2,		/*  8  */
    TDA18204_IF_LPF_Offset_12_VALUE_3 = 3		/*  12  */
} TDA18204_IF_LPF_Offset_t, *pTDA18204_IF_LPF_Offset_t;

typedef enum _TDA18204_LP_Fc_Item_t {
    TDA18204_LP_Fc_6_VALUE_0 = 0,		/*  6  */
    TDA18204_LP_Fc_7_VALUE_1 = 1,		/*  7  */
    TDA18204_LP_Fc_8_VALUE_2 = 2,		/*  8  */
    TDA18204_LP_Fc_9_VALUE_3 = 3,		/*  9  */
    TDA18204_LP_Fc_10_VALUE_4 = 4,		/*  10  */
    TDA18204_LP_Fc_11_2_VALUE_5 = 5,		/*  11.2  */
    TDA18204_LP_Fc_1_5_VALUE_6 = 6,		/*  1.5  */
    TDA18204_LP_Fc_1_5_VALUE_7 = 7		/*  1.5  */
} TDA18204_LP_Fc_t, *pTDA18204_LP_Fc_t;

/* TDA18204 Register IF_Byte_1 0x4D */
extern const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_1;
/* IF_LPF_Offset bit(s): shift value RCCal IF_LPF (depends of IF_LPF_sign). . 00:  Nominal. . 01: + or - 4%. . 10: + or - 8%. . 11: + or - 12% */
extern const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_1__IF_LPF_Offset;
/* IF_LPF_sign bit(s): .0. : positive shift. . .1. : negative shift */
extern const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_1__IF_LPF_sign;
/* LP_Fc bit(s): Select low pass filter cut-off frequency. .000. : 6 MHz. .001. : 7 MHz. .010. : 8 MHz. .011. : 9 MHz. .100. : 10 MHz. .101. : 11.2 MHz. .110. : 1.5 MHz. .111. : 1.5 MHz */
extern const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_1__LP_Fc;


typedef enum _TDA18204_IF_HPF_Offset_Item_t {
    TDA18204_IF_HPF_Offset_NOMINAL_VALUE_0 = 0,		/*  Nominal  */
    TDA18204_IF_HPF_Offset_4_VALUE_1 = 1,		/*  4  */
    TDA18204_IF_HPF_Offset_8_VALUE_2 = 2,		/*  8  */
    TDA18204_IF_HPF_Offset_12_VALUE_3 = 3		/*  12  */
} TDA18204_IF_HPF_Offset_t, *pTDA18204_IF_HPF_Offset_t;

typedef enum _TDA18204_IF_HP_Fc_Item_t {
    TDA18204_IF_HP_Fc_0_5_VALUE_0 = 0,		/*  0.5  */
    TDA18204_IF_HP_Fc_1_VALUE_1 = 1,		/*  1  */
    TDA18204_IF_HP_Fc_1_6_VALUE_2 = 2,		/*  1.6  */
    TDA18204_IF_HP_Fc_2_3_VALUE_3 = 3,		/*  2.3  */
    TDA18204_IF_HP_Fc_3_5_DEF_VALUE_4 = 4,		/*  3.5 (def)  */
    TDA18204_IF_HP_Fc_3_5_VALUE_5 = 5,		/*  3.5  */
    TDA18204_IF_HP_Fc_3_5_VALUE_6 = 6,		/*  3.5  */
    TDA18204_IF_HP_Fc_3_5_VALUE_7 = 7		/*  3.5  */
} TDA18204_IF_HP_Fc_t, *pTDA18204_IF_HP_Fc_t;

/* TDA18204 Register IF_Byte_2 0x4E */
extern const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_2;
/* IF_HPF_Offset bit(s): shift value RCCal IF_HPF (depends of IF_HPF_sign). . 00:  Nominal. . 01: + or - 4%. . 10: + or - 8%. . 11: + or - 12% */
extern const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_2__IF_HPF_Offset;
/* IF_HPF_sign bit(s): .0. : positive shift. . .1. : negative shift */
extern const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_2__IF_HPF_sign;
/* IF_HP_Fc bit(s): Select high pass filter cut-off frequency. . 000 : 0,5 MHz. . 001 : 1 MHz. . 010 : 1.6 MHz. . 011 : 2.3 MHz. . 100 :  3.5 MHz. . others : 3.5 MHz */
extern const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_2__IF_HP_Fc;
/* IF_HP_Enable bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_IF_Byte_2__IF_HP_Enable;


typedef enum _TDA18204_Dig_Clock_Item_t {
    TDA18204_Dig_Clock_INTERNAL_OSCILLATOR_VALUE_0 = 0,		/*  internal oscillator  */
    TDA18204_Dig_Clock_XTAL_IN_OR_BUFFER_VALUE_1 = 1,		/*  Xtal in or buffer  */
    TDA18204_Dig_Clock_AGC2CLKGATED_VALUE_2 = 2,		/*  Agc2clkgated  */
    TDA18204_Dig_Clock_SUBLO_VALUE_3 = 3		/*  subLO  */
} TDA18204_Dig_Clock_t, *pTDA18204_Dig_Clock_t;

/* TDA18204 Register clock_ctrl_byte_1 0x4F */
extern const TDA18204_BitField_t gTDA18204_Reg_clock_ctrl_byte_1;
/* Dig_Clock bit(s): Select he digital after calibration clock is:. . .00. : internal oscillator. . .01. : xtal in or buffer. . .10. : agc2clkgated. . .11. : sub lo */
extern const TDA18204_BitField_t gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock;


/* TDA18204 Register FVCO_MAX_BYTE_1 0x51 */
extern const TDA18204_BitField_t gTDA18204_Reg_FVCO_MAX_BYTE_1;
/* FVCO_max_1 bit(s): Highest frequency of VCO high in [Hz] */
extern const TDA18204_BitField_t gTDA18204_Reg_FVCO_MAX_BYTE_1__FVCO_max_1;


/* TDA18204 Register FVCO_MAX_BYTE_2 0x52 */
extern const TDA18204_BitField_t gTDA18204_Reg_FVCO_MAX_BYTE_2;
/* FVCO_max_2 bit(s): Highest frequency of VCO high in [Hz] */
extern const TDA18204_BitField_t gTDA18204_Reg_FVCO_MAX_BYTE_2__FVCO_max_2;


/* TDA18204 Register FVCO_MAX_BYTE_3 0x53 */
extern const TDA18204_BitField_t gTDA18204_Reg_FVCO_MAX_BYTE_3;
/* FVCO_max_3 bit(s): Highest frequency of VCO high in [Hz] */
extern const TDA18204_BitField_t gTDA18204_Reg_FVCO_MAX_BYTE_3__FVCO_max_3;


/* TDA18204 Register MSM_byte_1 0x54 */
extern const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_1;
/* RSSI_Meas bit(s): launch rssi measurement */
extern const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_1__RSSI_Meas;
/* IR_Cal_Image bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_1__IR_Cal_Image;
/* IR_CAL_Wanted bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_1__IR_CAL_Wanted;
/* RC_Cal bit(s): Launch the RC cal */
extern const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_1__RC_Cal;
/* FVCO_max_calc bit(s): Launch Fvco max_calculation */
extern const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_1__FVCO_max_calc;


/* TDA18204 Register MSM_byte_2 0x55 */
extern const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_2;
/* TEQ_Launch bit(s): launch TEQ change Slope */
extern const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_2__TEQ_Launch;
/* XtalCal_Launch bit(s): launch Xtal cal */
extern const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_2__XtalCal_Launch;
/* MSM_Launch bit(s): MSM */
extern const TDA18204_BitField_t gTDA18204_Reg_MSM_byte_2__MSM_Launch;


/* TDA18204 Register RSSI_byte_1 0x56 */
extern const TDA18204_BitField_t gTDA18204_Reg_RSSI_byte_1;
/* RSSI bit(s): rssi agc gain */
extern const TDA18204_BitField_t gTDA18204_Reg_RSSI_byte_1__RSSI;


typedef enum _TDA18204_RSSI_Gv_Item_t {
    TDA18204_RSSI_Gv_MINUS11_5_VALUE_0 = 0,		/*  -11.5  */
    TDA18204_RSSI_Gv_MINUS9_5_VALUE_1 = 1,		/*  -9.5  */
    TDA18204_RSSI_Gv_MINUS6_5_VALUE_2 = 2,		/*  -6.5  */
    TDA18204_RSSI_Gv_MINUS4_5_VALUE_3 = 3		/*  -4.5  */
} TDA18204_RSSI_Gv_t, *pTDA18204_RSSI_Gv_t;

/* TDA18204 Register RSSI_byte_2 0x57 */
extern const TDA18204_BitField_t gTDA18204_Reg_RSSI_byte_2;
/* RSSI_Gv bit(s): RSSI input gain */
extern const TDA18204_BitField_t gTDA18204_Reg_RSSI_byte_2__RSSI_Gv;
/* RSSI_Cap_Val bit(s): Capacitor value */
extern const TDA18204_BitField_t gTDA18204_Reg_RSSI_byte_2__RSSI_Cap_Val;
/* RSSI_Ck_Speed bit(s): Clock value */
extern const TDA18204_BitField_t gTDA18204_Reg_RSSI_byte_2__RSSI_Ck_Speed;


/* TDA18204 Register Sigma_delta_byte_1 0x59 */
extern const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_1;
/* LO_Int bit(s): Integer maindiv programming (N) */
extern const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_1__LO_Int;


/* TDA18204 Register Sigma_delta_byte_2 0x5A */
extern const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_2;
/* LO_Frac_2 bit(s): Fractional maindiv programming (K) */
extern const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_2__LO_Frac_2;


/* TDA18204 Register Sigma_delta_byte_3 0x5B */
extern const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_3;
/* LO_Frac_1 bit(s): Fractional maindiv programming (K) */
extern const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_3__LO_Frac_1;


/* TDA18204 Register Sigma_delta_byte_4 0x5C */
extern const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_4;
/* LO_Frac_0 bit(s): Fractional maindiv programming (K) */
extern const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_byte_4__LO_Frac_0;


/* TDA18204 Register Sigma_delta_Test_byte 0x5D */
extern const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_Test_byte;
/* LOSDrstN bit(s): 1: On  -  0: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_Sigma_delta_Test_byte__LOSDrstN;


/* TDA18204 Register PLL_byte 0x5E */
extern const TDA18204_BitField_t gTDA18204_Reg_PLL_byte;
/* PLL_lock_reset bit(s): 1: On  -  0: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_PLL_byte__PLL_lock_reset;


/* TDA18204 Register rf_switch_smooth_state 0x60 */
extern const TDA18204_BitField_t gTDA18204_Reg_rf_switch_smooth_state;
/* rf_mux_agc2_wb_smooth_state bit(s): switch state:. .0. : switch opened. .1. : switch closed */
extern const TDA18204_BitField_t gTDA18204_Reg_rf_switch_smooth_state__rf_mux_agc2_wb_smooth_state;


/* TDA18204 Register rf_switchs 0x61 */
extern const TDA18204_BitField_t gTDA18204_Reg_rf_switchs;
/* RF_mux_acg2_nb bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_rf_switchs__RF_mux_acg2_nb;
/* rf_mux_acg2_wb_smooth_move bit(s): 0: On  -  1: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_rf_switchs__rf_mux_acg2_wb_smooth_move;


/* TDA18204 Register calgen_byte_1 0x62 */
extern const TDA18204_BitField_t gTDA18204_Reg_calgen_byte_1;
/* vco_IR_high bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_calgen_byte_1__vco_IR_high;
/* calgen_pph_sel bit(s): 0: Off  -  1: On */
extern const TDA18204_BitField_t gTDA18204_Reg_calgen_byte_1__calgen_pph_sel;


typedef enum _TDA18204_IR_Mixer_Atten_Item_t {
    TDA18204_IR_Mixer_Atten_MINUS21_VALUE_0 = 0,		/*  -21  */
    TDA18204_IR_Mixer_Atten_MINUS18_VALUE_1 = 1,		/*  -18  */
    TDA18204_IR_Mixer_Atten_MINUS15_VALUE_2 = 2,		/*  -15  */
    TDA18204_IR_Mixer_Atten_MINUS12_VALUE_3 = 3,		/*  -12  */
    TDA18204_IR_Mixer_Atten_MINUS9_VALUE_4 = 4,		/*  -9  */
    TDA18204_IR_Mixer_Atten_MINUS6_VALUE_5 = 5,		/*  -6  */
    TDA18204_IR_Mixer_Atten_MINUS3_VALUE_6 = 6,		/*  -3  */
    TDA18204_IR_Mixer_Atten_MAX_VALUE_7 = 7		/*  Max  */
} TDA18204_IR_Mixer_Atten_t, *pTDA18204_IR_Mixer_Atten_t;

/* TDA18204 Register IR_Cal_byte_1 0x63 */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_1;
/* IR_IF_LPF_1p5M bit(s): select mode Ircal (xtal :16MHz) for LPF */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_1__IR_IF_LPF_1p5M;
/* IR_Mixer_Atten bit(s): Selects amplitude for the IR calibration */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_1__IR_Mixer_Atten;


typedef enum _TDA18204_IR_Step_Delta_Item_t {
    TDA18204_IR_Step_Delta_0_VALUE_0 = 0,		/*  0  */
    TDA18204_IR_Step_Delta_1_VALUE_1 = 1,		/*  1  */
    TDA18204_IR_Step_Delta_2_VALUE_2 = 2,		/*  2  */
    TDA18204_IR_Step_Delta_3_VALUE_3 = 3,		/*  3  */
    TDA18204_IR_Step_Delta_4_VALUE_4 = 4,		/*  4  */
    TDA18204_IR_Step_Delta_5_VALUE_5 = 5,		/*  5  */
    TDA18204_IR_Step_Delta_6_VALUE_6 = 6,		/*  6  */
    TDA18204_IR_Step_Delta_7_VALUE_7 = 7		/*  7  */
} TDA18204_IR_Step_Delta_t, *pTDA18204_IR_Step_Delta_t;

typedef enum _TDA18204_IR_FreqPtr_Item_t {
    TDA18204_IR_FreqPtr_NOT_USED_IR_NATURAL_VALUE_0 = 0,		/*  NOT_USED_IR_natural  */
    TDA18204_IR_FreqPtr_LOW_VALUE_1 = 1,		/*  low  */
    TDA18204_IR_FreqPtr_MID_VALUE_2 = 2,		/*  mid  */
    TDA18204_IR_FreqPtr_HIGH_VALUE_3 = 3		/*  high  */
} TDA18204_IR_FreqPtr_t, *pTDA18204_IR_FreqPtr_t;

/* TDA18204 Register IR_Cal_byte_2 0x64 */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_2;
/* IR_Step_Delta bit(s): select the step of convergence */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_2__IR_Step_Delta;
/* IR_FreqPtr bit(s): Selects the frequency band for IRCAL */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_2__IR_FreqPtr;


typedef enum _TDA18204_IR_target_Item_t {
    TDA18204_IR_target_55_ES1_MINUS_61_ES2_VALUE_0 = 0,		/*  55 (ES1) - 61 (ES2)  */
    TDA18204_IR_target_57_ES1_MINUS_63_ES2_VALUE_1 = 1,		/*  57 (ES1) - 63 (ES2)  */
    TDA18204_IR_target_59_ES1_MINUS_65_ES2_VALUE_2 = 2,		/*  59 (ES1) - 65 (ES2)  */
    TDA18204_IR_target_61_ES1_MINUS_67_ES2_VALUE_3 = 3		/*  61 (ES1) - 67 (ES2)  */
} TDA18204_IR_target_t, *pTDA18204_IR_target_t;

/* TDA18204 Register IR_Cal_byte_3 0x65 */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_3;
/* IR_target bit(s): Select IR target */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_3__IR_target;


/* TDA18204 Register IR_Cal_byte_4 0x66 */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_4;
/* Rx bit(s): read value of Rx for IR compensation */
extern const TDA18204_BitField_t gTDA18204_Reg_IR_Cal_byte_4__Rx;


typedef enum _TDA18204_ICP_select_Item_t {
    TDA18204_ICP_select_500_VALUE_0 = 0,		/*  500  */
    TDA18204_ICP_select_150_VALUE_1 = 1,		/*  150  */
    TDA18204_ICP_select_300_VALUE_2 = 2,		/*  300  */
    TDA18204_ICP_select_100_VALUE_3 = 3		/*  100  */
} TDA18204_ICP_select_t, *pTDA18204_ICP_select_t;

/* TDA18204 Register Charge_pump_byte 0x67 */
extern const TDA18204_BitField_t gTDA18204_Reg_Charge_pump_byte;
/* ICP_Force_Src bit(s): 1: On  -  0: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_Charge_pump_byte__ICP_Force_Src;
/* ICP_low bit(s): 1: On  -  0: Off */
extern const TDA18204_BitField_t gTDA18204_Reg_Charge_pump_byte__ICP_low;
/* ICP_select bit(s): select offset current of charge pump:. . 00: 500microA. . 01: 150microA. . 10: 300microA. . 11: 100microA */
extern const TDA18204_BitField_t gTDA18204_Reg_Charge_pump_byte__ICP_select;



#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA18204_REGDEF_H */
