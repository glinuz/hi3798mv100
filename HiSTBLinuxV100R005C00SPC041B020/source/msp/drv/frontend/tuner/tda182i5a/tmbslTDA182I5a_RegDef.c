/*
  Copyright (C) 2011 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA182I5a_RegDef.c
 *
 * \date          %modify_time%
 *
 * \author        TVFE SW Group
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA182I5a_Driver_User_Guide.pdf
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
#include "tmbslTDA182I5a.h"
#include "tmbslTDA182I5a_RegDef.h"

/*============================================================================*/
/* Global data:                                                               */
/*============================================================================*/

/* TDA182I5a Register ID_byte_1 0x00 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_1 = { 0x00, 0x00, 0x08, 0x00 };
/* Ident_1 bit(s): type number information (MSB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_1__Ident_1 = { 0x00, 0x00, 0x07, 0x00 };


/* TDA182I5a Register ID_byte_2 0x01 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_2 = { 0x01, 0x00, 0x08, 0x00 };
/* Ident_2 bit(s): type number information (LSB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_2__Ident_2 = { 0x01, 0x00, 0x08, 0x00 };


/* TDA182I5a Register ID_byte_3 0x02 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_3 = { 0x02, 0x00, 0x08, 0x00 };
/* Major_rev bit(s): device major revision */
const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_3__Major_rev = { 0x02, 0x04, 0x04, 0x00 };
/* Fab_rev bit(s): fabrication unit revision */
const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_3__Fab_rev = { 0x02, 0x03, 0x01, 0x00 };
/* Minor_rev bit(s): device minor revision */
const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_3__Minor_rev = { 0x02, 0x00, 0x03, 0x00 };


/* TDA182I5a Register Thermo_byte_1 0x03 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Thermo_byte_1 = { 0x03, 0x00, 0x08, 0x00 };
/* TM_D bit(s): junction temperature measurement */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Thermo_byte_1__TM_D = { 0x03, 0x00, 0x07, 0x00 };


/* TDA182I5a Register Thermo_byte_2 0x04 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Thermo_byte_2 = { 0x04, 0x00, 0x08, 0x00 };
/* TM_ON bit(s): temperature sensor ON or OFF */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Thermo_byte_2__TM_ON = { 0x04, 0x00, 0x01, 0x00 };


/* TDA182I5a Register Power_state_byte_1 0x05 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_1 = { 0x05, 0x00, 0x08, 0x00 };
/* POR bit(s): Power On Reset bit */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_1__POR = { 0x05, 0x06, 0x01, 0x00 };
/* AGCs_Max bit(s): maximum gain status indicator */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_1__AGCs_Max = { 0x05, 0x03, 0x01, 0x00 };
/* AGCs_Lock bit(s): AGC convergence status indicator */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_1__AGCs_Lock = { 0x05, 0x02, 0x01, 0x00 };
/* Vsync_Lock bit(s): Vertical Blanking Interval status indicator */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_1__Vsync_Lock = { 0x05, 0x01, 0x01, 0x00 };
/* LO_Lock bit(s): PLL lock status indicator */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_1__LO_Lock = { 0x05, 0x00, 0x01, 0x00 };


/* TDA182I5a Register Power_state_byte_2 0x06 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_2 = { 0x06, 0x00, 0x08, 0x00 };
/* power_state_mode bit(s): selects the power state mode */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_2__power_state_mode = { 0x06, 0x00, 0x02, 0x00 };


/* TDA182I5a Register Power_Level_byte_1 0x07 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_Level_byte_1 = { 0x07, 0x00, 0x08, 0x00 };
/* Det6_level bit(s): Detector 6 level indicator */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_Level_byte_1__Det6_level = { 0x07, 0x00, 0x08, 0x00 };


/* TDA182I5a Register IRQ_status 0x08 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status = { 0x08, 0x00, 0x08, 0x00 };
/* IRQ_status bit(s): Interupt Request Status */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__IRQ_status = { 0x08, 0x07, 0x01, 0x00 };
/* MSM_IRCAL_End bit(s): Image Rejection Calibration completion */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__MSM_IRCAL_End = { 0x08, 0x06, 0x01, 0x00 };
/* XtalCal_End bit(s): Xtal oscillator calibration completion */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__XtalCal_End = { 0x08, 0x05, 0x01, 0x00 };
/* MSM_RSSI_End bit(s): RSSI measurement completion */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__MSM_RSSI_End = { 0x08, 0x04, 0x01, 0x00 };
/* MSM_RCCal_End bit(s): RC calibration completion */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__MSM_RCCal_End = { 0x08, 0x03, 0x01, 0x00 };
/* VCO_Ampl_End bit(s): VCO amplitude calibration completion */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__VCO_Ampl_End = { 0x08, 0x02, 0x01, 0x00 };
/* VCO_Freq_End bit(s): VCO frequency calibration completion */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__VCO_Freq_End = { 0x08, 0x01, 0x01, 0x00 };


/* TDA182I5a Register IRQ_clear 0x0A */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear = { 0x0A, 0x00, 0x08, 0x00 };
/* IRQ_Clear bit(s): Clear IRQ */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__IRQ_Clear = { 0x0A, 0x07, 0x01, 0x00 };
/* MSM_IRCAL_Clear bit(s): Clear IRCAL */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__MSM_IRCAL_Clear = { 0x0A, 0x06, 0x01, 0x00 };
/* XtalCal_Clear bit(s): Clear XtalCAL */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__XtalCal_Clear = { 0x0A, 0x05, 0x01, 0x00 };
/* MSM_RSSI_Clear bit(s): Clear RSSI */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__MSM_RSSI_Clear = { 0x0A, 0x04, 0x01, 0x00 };
/* MSM_RCCal_Clear bit(s): Clear RCCAL */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__MSM_RCCal_Clear = { 0x0A, 0x03, 0x01, 0x00 };
/* VCO_Ampl_Clear bit(s): Clear VCO_Amp */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__VCO_Ampl_Clear = { 0x0A, 0x02, 0x01, 0x00 };
/* VCO_Freq_Clear bit(s): Clear VCO_Freq */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__VCO_Freq_Clear = { 0x0A, 0x01, 0x01, 0x00 };


/* TDA182I5a Register Gpio_ctrl_byte 0x0C */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Gpio_ctrl_byte = { 0x0C, 0x00, 0x08, 0x00 };
/* GPIO2 bit(s): enables / disables GPO2 output */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Gpio_ctrl_byte__GPIO2 = { 0x0C, 0x01, 0x01, 0x00 };
/* GPIO1 bit(s): enables / disables GPO1 output */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Gpio_ctrl_byte__GPIO1 = { 0x0C, 0x00, 0x01, 0x00 };


/* TDA182I5a Register AGC1_byte_1 0x0D */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_1 = { 0x0D, 0x00, 0x08, 0x00 };
/* LNA_HiZ bit(s): enables / disables the LNA_Hiz control */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_1__LNA_HiZ = { 0x0D, 0x07, 0x01, 0x00 };
/* AGC1_Top_Strategy bit(s): selects AGC1 TOP strategy algorithm (function of AGC1 gain) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_1__AGC1_Top_Strategy = { 0x0D, 0x04, 0x03, 0x00 };
/* AGC1_Top bit(s): selects  AGC1 TOP Down / TOP Up (dBuV) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_1__AGC1_Top = { 0x0D, 0x00, 0x04, 0x00 };


/* TDA182I5a Register AGC1_byte_2 0x0E */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_2 = { 0x0E, 0x00, 0x08, 0x00 };
/* AGC1_cc_Freeze_Enable bit(s): AGC1 Freeze algorithm selection */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_2__AGC1_cc_Freeze_Enable = { 0x0E, 0x06, 0x01, 0x00 };
/* AGC1_cc_Wide_Enable bit(s): AGC1 Wide algorithm selection */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_2__AGC1_cc_Wide_Enable = { 0x0E, 0x05, 0x01, 0x00 };
/* AGC145_Up_step bit(s): selects AGC1, AGC4 & AGC5 up time constant (ms) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_2__AGC145_Up_step = { 0x0E, 0x02, 0x03, 0x00 };


/* TDA182I5a Register AGC1_byte_3 0x0F */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_3 = { 0x0F, 0x00, 0x08, 0x00 };
/* AGC1_Do_step bit(s): selects AGC1 down time constant (ms) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_3__AGC1_Do_step = { 0x0F, 0x05, 0x02, 0x00 };
/* Force_AGC1_gain bit(s): enables / disables the manual control on AGC1 control loop */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_3__Force_AGC1_gain = { 0x0F, 0x04, 0x01, 0x00 };
/* AGC1_Gain bit(s): selects AGC1 gain value if Force_AGC1_gain = 1 (manual .forced. mode) (dB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_3__AGC1_Gain = { 0x0F, 0x00, 0x04, 0x00 };


/* TDA182I5a Register LT_byte_1 0x10 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_LT_byte_1 = { 0x10, 0x00, 0x08, 0x00 };
/* LT_offset bit(s): allows to set the LT offset (offset vs LT atten & LNA gain) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_LT_byte_1__LT_offset = { 0x10, 0x05, 0x02, 0x00 };
/* LT_force bit(s): enables / disables the manual control on LT control loop */
const TDA182I5a_BitField_t gTDA182I5a_Reg_LT_byte_1__LT_force = { 0x10, 0x03, 0x01, 0x00 };
/* LT_gain bit(s): selects LT attenuation value if LT_force = 1 (manual .forced. mode) (dB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_LT_byte_1__LT_gain = { 0x10, 0x00, 0x03, 0x00 };


/* TDA182I5a Register AGC2_byte_1 0x11 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_byte_1 = { 0x11, 0x00, 0x08, 0x00 };
/* AGC2_Top_Strategy bit(s): selects AGC2 TOP strategy algorithm (function of AGC1 gain) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_byte_1__AGC2_Top_Strategy = { 0x11, 0x05, 0x03, 0x00 };
/* AGC2_TOP bit(s): selects AGC2 TOP Overload / Down / TOP Up (dBuV) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_byte_1__AGC2_TOP = { 0x11, 0x00, 0x04, 0x00 };


/* TDA182I5a Register AGC2_byte_2 0x12 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_byte_2 = { 0x12, 0x00, 0x08, 0x00 };
/* AGC2_Gain bit(s): selects AGC2 gain value if Force_AGC2_gain = 1 (manual .forced. mode) (dB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_byte_2__AGC2_Gain = { 0x12, 0x00, 0x08, 0x00 };


/* TDA182I5a Register RF_byte 0x13 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_byte = { 0x13, 0x00, 0x08, 0x00 };
/* PLD_CC_Enable bit(s): Power Level detector configuration */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_byte__PLD_CC_Enable = { 0x13, 0x07, 0x01, 0x00 };
/* AGCs_loop_off bit(s): enables / disables all AGC detector clocks */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_byte__AGCs_loop_off = { 0x13, 0x06, 0x01, 0x00 };
/* AGC2_Do_step bit(s): selects AGC2 down time constant (ms) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_byte__AGC2_Do_step = { 0x13, 0x04, 0x02, 0x00 };
/* Force_AGC2_gain bit(s): enables / disables the manual control on AGC2 control loop */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_byte__Force_AGC2_gain = { 0x13, 0x02, 0x01, 0x00 };


/* TDA182I5a Register AGCK_byte_1 0x14 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGCK_byte_1 = { 0x14, 0x00, 0x08, 0x00 };
/* AGC2_Up_step bit(s): selects AGC2 up time constant (ms) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGCK_byte_1__AGC2_Up_step = { 0x14, 0x04, 0x03, 0x00 };
/* AGCK_Mode bit(s): selects AGCK clock  (ms) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGCK_byte_1__AGCK_Mode = { 0x14, 0x01, 0x02, 0x00 };


/* TDA182I5a Register IR_Mixer_byte_1 0x15 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_1 = { 0x15, 0x00, 0x08, 0x00 };
/* hrm_swap bit(s): Allows to select the RF spectral inversion mode */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_1__hrm_swap = { 0x15, 0x06, 0x02, 0x00 };
/* AGC4_TOP bit(s): selects  AGC4 TOP Down / TOP Up (dBuV) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_1__AGC4_TOP = { 0x15, 0x00, 0x02, 0x00 };


/* TDA182I5a Register IR_Mixer_byte_2 0x16 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_2 = { 0x16, 0x00, 0x08, 0x00 };
/* AGC4_Do_step bit(s): selects AGC4 down time constant (ms) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_2__AGC4_Do_step = { 0x16, 0x05, 0x02, 0x00 };
/* Force_AGC4_gain bit(s): enables / disables the manual control on AGC4 control loop */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_2__Force_AGC4_gain = { 0x16, 0x04, 0x01, 0x00 };
/* AGC4_Gain bit(s): selects AGC4 gain value if Force_AGC4_gain = 1 (manual .forced. mode) (dB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_2__AGC4_Gain = { 0x16, 0x01, 0x03, 0x00 };


/* TDA182I5a Register AGC5_byte_1 0x17 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_1 = { 0x17, 0x00, 0x08, 0x00 };
/* RSSI_HP_Fc bit(s): selects RSSI HPF cut off frequency (MHz) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_1__RSSI_HP_Fc = { 0x17, 0x03, 0x01, 0x00 };
/* AGC5_HP_Fc bit(s): selects AGC5 highpass filter cut off frequency (MHz) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_1__AGC5_HP_Fc = { 0x17, 0x02, 0x01, 0x00 };
/* AGC5_TOP bit(s): selects  AGC5 TOP Down / TOP Up (dBuV) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_1__AGC5_TOP = { 0x17, 0x00, 0x02, 0x00 };


/* TDA182I5a Register AGC5_byte_2 0x18 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_2 = { 0x18, 0x00, 0x08, 0x00 };
/* AGC5_Do_step bit(s): selects AGC5 down time constant (ms) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_2__AGC5_Do_step = { 0x18, 0x04, 0x02, 0x00 };
/* Force_AGC5_gain bit(s): enables / disables the manual control on AGC5 control loop */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_2__Force_AGC5_gain = { 0x18, 0x03, 0x01, 0x00 };
/* AGC5_Gain bit(s): selects AGC5 gain value if Force_AGC5_gain = 1 (manual .forced. mode) (dB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_2__AGC5_Gain = { 0x18, 0x00, 0x03, 0x00 };


/* TDA182I5a Register Adapt_Top_byte 0x19 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Adapt_Top_byte = { 0x19, 0x00, 0x08, 0x00 };
/* Top_adapt_AGC2 bit(s): selects AGC2 TOP adapt algorithm (function of AGC5 gain) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Adapt_Top_byte__Top_adapt_AGC2 = { 0x19, 0x02, 0x02, 0x00 };
/* Top_adapt_AGC1 bit(s): selects AGC1 TOP adapt algorithm (function of AGC5 gain) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Adapt_Top_byte__Top_adapt_AGC1 = { 0x19, 0x00, 0x02, 0x00 };


/* TDA182I5a Register Vsync_byte 0x1A */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Vsync_byte = { 0x1A, 0x00, 0x08, 0x00 };
/* PD_Vsync_Mgt bit(s): enables / disables the automatic management of AGC clocking in ATV reception mode */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Vsync_byte__PD_Vsync_Mgt = { 0x1A, 0x07, 0x01, 0x00 };
/* Neg_Modulation bit(s): selects whether the received analog TV channel uses negative or positive modulation */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Vsync_byte__Neg_Modulation = { 0x1A, 0x06, 0x01, 0x00 };


/* TDA182I5a Register Ovld_Udld_byte 0x1B */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Ovld_Udld_byte = { 0x1B, 0x00, 0x08, 0x00 };
/* RF_udld_disable bit(s): enables / disables the RF underload detection */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Ovld_Udld_byte__RF_udld_disable = { 0x1B, 0x07, 0x01, 0x00 };
/* IF_ovld_disable bit(s): enables / disables the IF overload detection */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Ovld_Udld_byte__IF_ovld_disable = { 0x1B, 0x06, 0x01, 0x00 };
/* RF_ovld_disable bit(s): enables / disables the RF overload detection */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Ovld_Udld_byte__RF_ovld_disable = { 0x1B, 0x05, 0x01, 0x00 };
/* IF_udld_disable bit(s): enables/ disabled the IF underload mechanism */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Ovld_Udld_byte__IF_udld_disable = { 0x1B, 0x04, 0x01, 0x00 };
/* AGC_Ovld_TOP bit(s): selects IF overload / IF underload ranges (dB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Ovld_Udld_byte__AGC_Ovld_TOP = { 0x1B, 0x01, 0x03, 0x00 };


/* TDA182I5a Register IF_Byte_1 0x1C */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_1 = { 0x1C, 0x00, 0x08, 0x00 };
/* IF_LPF_Offset bit(s): selects the IF LPF cut-off frequency correction factor (%) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_1__IF_LPF_Offset = { 0x1C, 0x05, 0x02, 0x00 };
/* IF_LPF_sign bit(s): selects the IF LPF cut-off frequency correction sign */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_1__IF_LPF_sign = { 0x1C, 0x04, 0x01, 0x00 };
/* DC_Notch bit(s): enables / disables the DC Notch in the IF polyphase filter */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_1__DC_Notch = { 0x1C, 0x03, 0x01, 0x00 };
/* LP_Fc bit(s): selects the IF LPF cut-off frequency (MHz) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_1__LP_Fc = { 0x1C, 0x00, 0x03, 0x00 };


/* TDA182I5a Register IF_Byte_2 0x1D */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_2 = { 0x1D, 0x00, 0x08, 0x00 };
/* IF_HPF_Offset bit(s): selects the IF HPF cut-off frequency correction factor (%) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_2__IF_HPF_Offset = { 0x1D, 0x05, 0x02, 0x00 };
/* IF_HPF_sign bit(s): selects the IF HPF cut-off frequency correction sign */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_2__IF_HPF_sign = { 0x1D, 0x04, 0x01, 0x00 };
/* IF_HP_Fc bit(s): selects the IF HPF cut-off frequency (MHz) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_2__IF_HP_Fc = { 0x1D, 0x01, 0x03, 0x00 };
/* IF_HP_Enable bit(s): enables / disables the IF highpass filter */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_2__IF_HP_Enable = { 0x1D, 0x00, 0x01, 0x00 };


/* TDA182I5a Register IF_Byte_3 0x1E */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_3 = { 0x1E, 0x00, 0x08, 0x00 };
/* IF_Notch_Offset bit(s): selects the IF notch center frequency correction factor (%) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_3__IF_Notch_Offset = { 0x1E, 0x05, 0x02, 0x00 };
/* IF_Notch_sign bit(s): selects the IF notch center frequency correction sign */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_3__IF_Notch_sign = { 0x1E, 0x04, 0x01, 0x00 };
/* IF_Notch_Freq bit(s): selects the IF notch center frequency (MHz) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_3__IF_Notch_Freq = { 0x1E, 0x01, 0x03, 0x00 };
/* IF_Notch_Enable bit(s): enables / disables the IF notch filter */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_3__IF_Notch_Enable = { 0x1E, 0x00, 0x01, 0x00 };


/* TDA182I5a Register Reference_Byte 0x1F */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Reference_Byte = { 0x1F, 0x00, 0x08, 0x00 };
/* AGC_freq bit(s): selects the AGC frequency (kHz) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Reference_Byte__AGC_freq = { 0x1F, 0x06, 0x01, 0x00 };
/* Dig_Clock bit(s): selects the clock of the digital core */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Reference_Byte__Dig_Clock = { 0x1F, 0x03, 0x01, 0x00 };
/* Xtout_Amp bit(s): selects XTOUT amplitude (mVpp) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Reference_Byte__Xtout_Amp = { 0x1F, 0x01, 0x02, 0x00 };
/* Xtout bit(s): enables / disables the reference clock output XTOUT  */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Reference_Byte__Xtout = { 0x1F, 0x00, 0x01, 0x00 };


/* TDA182I5a Register IF_AGC_byte 0x20 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_AGC_byte = { 0x20, 0x00, 0x08, 0x00 };
/* IFAGCINV_En bit(s): Activates / desactivates the IFAGC gain control inversion */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_AGC_byte__IFAGCINV_En = { 0x20, 0x07, 0x01, 0x00 };
/* IFnotchToRSSI bit(s): enables / disables the IF notch to RSSI control */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_AGC_byte__IFnotchToRSSI = { 0x20, 0x06, 0x01, 0x00 };
/* DIFOUT_En bit(s): enables / disables the IFOUT_D outputs */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_AGC_byte__DIFOUT_En = { 0x20, 0x04, 0x01, 0x00 };
/* DVIFAGC_En bit(s): selects IFAGC control path */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_AGC_byte__DVIFAGC_En = { 0x20, 0x03, 0x01, 0x00 };
/* IF_level bit(s): selects the IF amplifier gain range */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_AGC_byte__IF_level = { 0x20, 0x00, 0x03, 0x00 };


/* TDA182I5a Register MSM_byte_1 0x21 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_1 = { 0x21, 0x00, 0x08, 0x00 };
/* IR_CAL_Loop bit(s): Image rejection calibration mode */
const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_1__IR_CAL_Loop = { 0x21, 0x04, 0x01, 0x00 };
/* IR_Cal_Image bit(s): Image rejection calibration selection */
const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_1__IR_Cal_Image = { 0x21, 0x03, 0x01, 0x00 };
/* RC_Cal bit(s): RC calibration selection */
const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_1__RC_Cal = { 0x21, 0x02, 0x01, 0x00 };
/* VCO_Ampl_Cal bit(s): VCO amplitude calibration selection */
const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_1__VCO_Ampl_Cal = { 0x21, 0x01, 0x01, 0x00 };
/* VCO_Freq_Meas bit(s): VCO frequency calibration selection */
const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_1__VCO_Freq_Meas = { 0x21, 0x00, 0x01, 0x00 };


/* TDA182I5a Register MSM_byte_2 0x22 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_2 = { 0x22, 0x00, 0x08, 0x00 };
/* AGC_CC_Fast_Launch bit(s): Fast Mode trigger */
const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_2__AGC_CC_Fast_Launch = { 0x22, 0x02, 0x01, 0x00 };
/* MSM_Launch bit(s): Main State Machine execution */
const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_2__MSM_Launch = { 0x22, 0x00, 0x01, 0x00 };


/* TDA182I5a Register PowerSavingMode_byte_1 0x23 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_1 = { 0x23, 0x00, 0x08, 0x00 };
/* PSM_LNA bit(s): adjusts the current in LNA amplifier (mA) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_1__PSM_LNA = { 0x23, 0x06, 0x02, 0x00 };
/* PSM_LoMixer bit(s): adjusts the current in LO/mixer section (mA) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_1__PSM_LoMixer = { 0x23, 0x04, 0x02, 0x00 };
/* PSM_AGC2 bit(s): adjusts the current in AGC2 amplifier (mA) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_1__PSM_AGC2 = { 0x23, 0x02, 0x02, 0x00 };
/* PSM_Det2 bit(s): adjusts the current in AGC2 detector (mA) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_1__PSM_Det2 = { 0x23, 0x00, 0x02, 0x00 };


/* TDA182I5a Register PowerSavingMode_byte_2 0x24 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_2 = { 0x24, 0x00, 0x08, 0x00 };
/* Loch_curr_boost bit(s): adjusts the current in LO chain (mA) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_2__Loch_curr_boost = { 0x24, 0x06, 0x02, 0x00 };
/* PSM_IFfilter bit(s): adjusts the current in IF filter (mA) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_2__PSM_IFfilter = { 0x24, 0x04, 0x02, 0x00 };
/* PSM_IFLPF bit(s): adjusts the current in IF LPF (mA) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_2__PSM_IFLPF = { 0x24, 0x02, 0x02, 0x00 };
/* PSM_RF_LPF bit(s): adjusts the current in the harmonic filter (mA) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_2__PSM_RF_LPF = { 0x24, 0x00, 0x02, 0x00 };


/* TDA182I5a Register PowerSavingMode_byte_3 0x25 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_3 = { 0x25, 0x00, 0x08, 0x00 };
/* LOdiv4_Boost bit(s): adjusts the current of the divider by 4 in the LO section */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_3__LOdiv4_Boost = { 0x25, 0x06, 0x01, 0x00 };
/* PSM_Mixer bit(s): adjusts the current in the mixer (mA) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_3__PSM_Mixer = { 0x25, 0x04, 0x02, 0x00 };
/* PSM_Ifpoly bit(s): adjusts the current in IF polyphase filter (mA) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_3__PSM_Ifpoly = { 0x25, 0x02, 0x02, 0x00 };
/* PSM_Lodriver bit(s): adjusts the current in LO driver (mA) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_3__PSM_Lodriver = { 0x25, 0x00, 0x02, 0x00 };


/* TDA182I5a Register RF_Filters_byte_1 0x26 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Filters_byte_1 = { 0x26, 0x00, 0x08, 0x00 };
/* LNA_Highpass bit(s): enables / disables the LNA highpass filter */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Filters_byte_1__LNA_Highpass = { 0x26, 0x03, 0x01, 0x00 };
/* AGC2_Highpass bit(s): selects AGC2 highpass filter cut off frequency (MHz) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Filters_byte_1__AGC2_Highpass = { 0x26, 0x00, 0x03, 0x00 };


/* TDA182I5a Register RF_Low_Pass_Filter_byte_1 0x27 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_1 = { 0x27, 0x00, 0x08, 0x00 };
/* RF_LPF_1 bit(s): selects harmonic filter RF_LPF coefficients */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_1__RF_LPF_1 = { 0x27, 0x00, 0x08, 0x00 };


/* TDA182I5a Register RF_Low_Pass_Filter_byte_2 0x28 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_2 = { 0x28, 0x00, 0x08, 0x00 };
/* RF_LPF_2 bit(s): selects harmonic filter RF_LPF coefficients */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_2__RF_LPF_2 = { 0x28, 0x04, 0x04, 0x00 };
/* RF_LPF_f_1 bit(s): selects harmonic filter RF_LPF_f coefficients */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_2__RF_LPF_f_1 = { 0x28, 0x00, 0x04, 0x00 };


/* TDA182I5a Register RF_Low_Pass_Filter_byte_3 0x29 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_3 = { 0x29, 0x00, 0x08, 0x00 };
/* RF_LPF_f_2 bit(s): selects harmonic filter RF_LPF_f coefficients */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_3__RF_LPF_f_2 = { 0x29, 0x00, 0x08, 0x00 };


/* TDA182I5a Register AGC1_Top_byte 0x2B */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_Top_byte = { 0x2B, 0x00, 0x08, 0x00 };
/* AGC1_Top_Do_Read bit(s): reads current AGC1 TOP Down (dBuV) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_Top_byte__AGC1_Top_Do_Read = { 0x2B, 0x04, 0x04, 0x00 };
/* AGC1_Top_Up_Read bit(s): reads current AGC1 TOP Up (dBuV) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_Top_byte__AGC1_Top_Up_Read = { 0x2B, 0x00, 0x04, 0x00 };


/* TDA182I5a Register AGC2_Top_byte 0x2C */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_Top_byte = { 0x2C, 0x00, 0x08, 0x00 };
/* AGC2_Top_Do_Read bit(s): reads current AGC2 TOP Down (dBuV) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_Top_byte__AGC2_Top_Do_Read = { 0x2C, 0x00, 0x05, 0x00 };


/* TDA182I5a Register AGC1_Gain_byte 0x2D */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_Gain_byte = { 0x2D, 0x00, 0x08, 0x00 };
/* LT_gain_read bit(s): reads current LT attenuation value (dB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_Gain_byte__LT_gain_read = { 0x2D, 0x04, 0x03, 0x00 };
/* AGC1_Gain_Read bit(s): reads current AGC1 gain value (dB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_Gain_byte__AGC1_Gain_Read = { 0x2D, 0x00, 0x04, 0x00 };


/* TDA182I5a Register AGC2_Gain_byte 0x2E */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_Gain_byte = { 0x2E, 0x00, 0x08, 0x00 };
/* AGC2_Gain_Read bit(s): reads current AGC2 gain value (dB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_Gain_byte__AGC2_Gain_Read = { 0x2E, 0x00, 0x08, 0x00 };


/* TDA182I5a Register IFAGCs_Gain_byte 0x2F */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IFAGCs_Gain_byte = { 0x2F, 0x00, 0x08, 0x00 };
/* AGC5_Gain_Read bit(s): reads current AGC5 gain value (dB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IFAGCs_Gain_byte__AGC5_Gain_Read = { 0x2F, 0x03, 0x03, 0x00 };
/* AGC4_Gain_Read bit(s): reads current AGC4 gain value (dB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IFAGCs_Gain_byte__AGC4_Gain_Read = { 0x2F, 0x00, 0x03, 0x00 };


/* TDA182I5a Register RSSI_byte_2 0x31 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RSSI_byte_2 = { 0x31, 0x00, 0x08, 0x00 };
/* RSSI_AV bit(s): RSSI average mode */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RSSI_byte_2__RSSI_AV = { 0x31, 0x05, 0x01, 0x00 };
/* Det6_Cap_Val bit(s): selects Detector 6 integration capacitor value (pF) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RSSI_byte_2__Det6_Cap_Val = { 0x31, 0x04, 0x01, 0x00 };
/* RSSI_Ck_Speed bit(s): RSSI clock selection (kHz) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RSSI_byte_2__RSSI_Ck_Speed = { 0x31, 0x01, 0x01, 0x00 };


/* TDA182I5a Register Sublo_byte 0x34 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Sublo_byte = { 0x34, 0x00, 0x08, 0x00 };
/* SUBLO_prog bit(s): SubLO divider ratio */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Sublo_byte__SUBLO_prog = { 0x34, 0x00, 0x06, 0x00 };


/* TDA182I5a Register LoChain_byte 0x35 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_LoChain_byte = { 0x35, 0x00, 0x08, 0x00 };
/* PLL_Div2_Highfreq bit(s): PLL Div2 biasing */
const TDA182I5a_BitField_t gTDA182I5a_Reg_LoChain_byte__PLL_Div2_Highfreq = { 0x35, 0x04, 0x01, 0x00 };
/* LoChain_ratio bit(s): selects LO divider ratio */
const TDA182I5a_BitField_t gTDA182I5a_Reg_LoChain_byte__LoChain_ratio = { 0x35, 0x00, 0x04, 0x00 };


/* TDA182I5a Register Sigma_delta_byte_1 0x36 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_delta_byte_1 = { 0x36, 0x00, 0x08, 0x00 };
/* LO_Int bit(s): selects sigma delta integer ratio */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_delta_byte_1__LO_Int = { 0x36, 0x00, 0x08, 0x00 };


/* TDA182I5a Register Sigma_delta_byte_2 0x37 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_delta_byte_2 = { 0x37, 0x00, 0x08, 0x00 };
/* LO_Frac_2 bit(s): selects sigma delta fractional ratio (MSB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_delta_byte_2__LO_Frac_2 = { 0x37, 0x00, 0x07, 0x00 };


/* TDA182I5a Register Sigma_delta_byte_3 0x38 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_delta_byte_3 = { 0x38, 0x00, 0x08, 0x00 };
/* LO_Frac_1 bit(s): selects sigma delta fractional ratio (LSB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_delta_byte_3__LO_Frac_1 = { 0x38, 0x00, 0x08, 0x00 };


/* TDA182I5a Register Sigma_prog_byte 0x39 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_prog_byte = { 0x39, 0x00, 0x08, 0x00 };
/* Update_prg bit(s): PLL divider ratios synchronisation */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_prog_byte__Update_prg = { 0x39, 0x00, 0x01, 0x00 };


/* TDA182I5a Register Charge_pump_byte 0x3B */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Charge_pump_byte = { 0x3B, 0x00, 0x08, 0x00 };
/* ICP_Force_Sink bit(s): enables / disables the manual control on sinking charge pump */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Charge_pump_byte__ICP_Force_Sink = { 0x3B, 0x06, 0x01, 0x00 };
/* ICP_Force_Src bit(s): enables / disables the manual control on sourcing charge pump */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Charge_pump_byte__ICP_Force_Src = { 0x3B, 0x05, 0x01, 0x00 };
/* cpleak0 bit(s): enables / disables charge pump current offset */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Charge_pump_byte__cpleak0 = { 0x3B, 0x04, 0x01, 0x00 };
/* cp_cursel bit(s): selects the charge pump current (uA) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Charge_pump_byte__cp_cursel = { 0x3B, 0x00, 0x03, 0x00 };


/* TDA182I5a Register SigmaDelta_Test_byte 0x3C */
const TDA182I5a_BitField_t gTDA182I5a_Reg_SigmaDelta_Test_byte = { 0x3C, 0x00, 0x08, 0x00 };
/* LOSD_Static_N bit(s): Sigma Delta modulator ON / OFF */
const TDA182I5a_BitField_t gTDA182I5a_Reg_SigmaDelta_Test_byte__LOSD_Static_N = { 0x3C, 0x05, 0x01, 0x00 };


/* TDA182I5a Register Regulators_byte 0x3E */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Regulators_byte = { 0x3E, 0x00, 0x08, 0x00 };
/* Dig_Reg bit(s): selects the digital regulator output voltage */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Regulators_byte__Dig_Reg = { 0x3E, 0x06, 0x02, 0x00 };
/* VCO_Reg bit(s): selects VCO regulator voltage */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Regulators_byte__VCO_Reg = { 0x3E, 0x00, 0x02, 0x00 };


/* TDA182I5a Register VCO_byte 0x3F */
const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_byte = { 0x3F, 0x00, 0x08, 0x00 };
/* VCO_Select bit(s): selects VCO frequency band */
const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_byte__VCO_Select = { 0x3F, 0x00, 0x02, 0x00 };


/* TDA182I5a Register VCO_cal_byte_1 0x40 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_cal_byte_1 = { 0x40, 0x00, 0x08, 0x00 };
/* VCOampl_force_cur bit(s): Forces a manual VCO amplitude */
const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_cal_byte_1__VCOampl_force_cur = { 0x40, 0x04, 0x01, 0x00 };
/* VCO_currentctrl bit(s): Reads the index returned by the VCO amplitude calibration or set the index value in case of a manual control */
const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_cal_byte_1__VCO_currentctrl = { 0x40, 0x00, 0x04, 0x00 };


/* TDA182I5a Register VCO_cal_byte_2 0x41 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_cal_byte_2 = { 0x41, 0x00, 0x08, 0x00 };
/* VCO_swingctrl bit(s): VCO amplitude calibration target */
const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_cal_byte_2__VCO_swingctrl = { 0x41, 0x00, 0x04, 0x00 };


/* TDA182I5a Register vco_log 0x42 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_vco_log = { 0x42, 0x00, 0x08, 0x00 };
/* RF_udld_read bit(s): reads current status of RF_udld detector */
const TDA182I5a_BitField_t gTDA182I5a_Reg_vco_log__RF_udld_read = { 0x42, 0x07, 0x01, 0x00 };
/* IF_ovld_read bit(s): reads current status of IF_ovld detector */
const TDA182I5a_BitField_t gTDA182I5a_Reg_vco_log__IF_ovld_read = { 0x42, 0x06, 0x01, 0x00 };
/* RF_ovld_read bit(s): reads current status of RF_ovld detector */
const TDA182I5a_BitField_t gTDA182I5a_Reg_vco_log__RF_ovld_read = { 0x42, 0x05, 0x01, 0x00 };
/* IF_udld_read bit(s): reads current status of IF_udld detector */
const TDA182I5a_BitField_t gTDA182I5a_Reg_vco_log__IF_udld_read = { 0x42, 0x04, 0x01, 0x00 };
/* adc_vtune bit(s): vtune voltage measurement value (V) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_vco_log__adc_vtune = { 0x42, 0x01, 0x03, 0x00 };


/* TDA182I5a Register FVCO_MAX_BYTE_1 0x43 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_FVCO_MAX_BYTE_1 = { 0x43, 0x00, 0x08, 0x00 };
/* FVCO_max_1 bit(s): Current VCO frequency calibration result (MSB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_FVCO_MAX_BYTE_1__FVCO_max_1 = { 0x43, 0x00, 0x08, 0x00 };


/* TDA182I5a Register FVCO_MAX_BYTE_2 0x44 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_FVCO_MAX_BYTE_2 = { 0x44, 0x00, 0x08, 0x00 };
/* FVCO_max_2 bit(s): Current VCO frequency calibration result (medium byte) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_FVCO_MAX_BYTE_2__FVCO_max_2 = { 0x44, 0x00, 0x08, 0x00 };


/* TDA182I5a Register FVCO_MAX_BYTE_3 0x45 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_FVCO_MAX_BYTE_3 = { 0x45, 0x00, 0x08, 0x00 };
/* FVCO_max_3 bit(s): Current VCO frequency calibration result (LSB) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_FVCO_MAX_BYTE_3__FVCO_max_3 = { 0x45, 0x00, 0x08, 0x00 };


/* TDA182I5a Register RC_Cal_byte_1 0x46 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RC_Cal_byte_1 = { 0x46, 0x00, 0x08, 0x00 };
/* RC_Cal_Offset bit(s): Index offset applied to the RC calibration */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RC_Cal_byte_1__RC_Cal_Offset = { 0x46, 0x05, 0x02, 0x00 };


/* TDA182I5a Register RC_Cal_byte_2 0x47 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RC_Cal_byte_2 = { 0x47, 0x00, 0x08, 0x00 };
/* RCcal bit(s): RC calibration index result (including RC_Cal_Offset) */
const TDA182I5a_BitField_t gTDA182I5a_Reg_RC_Cal_byte_2__RCcal = { 0x47, 0x00, 0x05, 0x00 };


/* TDA182I5a Register IR_Cal_byte_3 0x48 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Cal_byte_3 = { 0x48, 0x00, 0x08, 0x00 };
/* IR_ROM_offset bit(s): Image Rejection calibration factor */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Cal_byte_3__IR_ROM_offset = { 0x48, 0x04, 0x02, 0x00 };
/* IR_FreqPtr bit(s): Frequency pointer to Image Rejection calibration results */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Cal_byte_3__IR_FreqPtr = { 0x48, 0x02, 0x02, 0x00 };


/* TDA182I5a Register IR_Cal_byte_4 0x49 */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Cal_byte_4 = { 0x49, 0x00, 0x08, 0x00 };
/* ircal_boost bit(s): Image Rejection calibration generator amplitude index */
const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Cal_byte_4__ircal_boost = { 0x49, 0x06, 0x02, 0x00 };


/* TDA182I5a Register Power_Down_byte_1 0x4B */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_Down_byte_1 = { 0x4B, 0x00, 0x08, 0x00 };
/* PD_adc_vtune bit(s): enables / disables the ADC connected on vtune */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_Down_byte_1__PD_adc_vtune = { 0x4B, 0x05, 0x01, 0x00 };
/* PD_SUBLO bit(s): enables / disables the subLO */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_Down_byte_1__PD_SUBLO = { 0x4B, 0x02, 0x01, 0x00 };
/* PD_LT bit(s): enables / disables the Loopthrough buffer */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_Down_byte_1__PD_LT = { 0x4B, 0x00, 0x01, 0x00 };


/* TDA182I5a Register Powerdown 0x4D */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Powerdown = { 0x4D, 0x00, 0x08, 0x00 };
/* force_soft_reset bit(s): enables / disables the software reset */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Powerdown__force_soft_reset = { 0x4D, 0x01, 0x01, 0x00 };
/* Soft_reset bit(s): Requests a software reset */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Powerdown__Soft_reset = { 0x4D, 0x00, 0x01, 0x00 };


/* TDA182I5a Register Test_byte 0x4E */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Test_byte = { 0x4E, 0x00, 0x08, 0x00 };
/* agck_step_sync bit(s): enables / disables the first AGC_K 1st step duration correction */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Test_byte__agck_step_sync = { 0x4E, 0x05, 0x01, 0x00 };
/* reset_i2c_dis bit(s): Defines if I2C core is reset or not when performing a Soft_reset operation */
const TDA182I5a_BitField_t gTDA182I5a_Reg_Test_byte__reset_i2c_dis = { 0x4E, 0x04, 0x01, 0x00 };


