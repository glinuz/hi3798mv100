/*
  Copyright (C) 2011 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA182I5a_RegDef.h
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


#pragma once
#ifndef _TMBSL_TDA182I5a_REGDEF_H
#define _TMBSL_TDA182I5a_REGDEF_H

#ifdef __cplusplus
extern "C"
{
#endif


/*============================================================================*/
/* Registers definitions:                                                     */
/*============================================================================*/

#define TDA182I5a_REG_ADD_SZ                             (0x01)
#define TDA182I5a_REG_DATA_MAX_SZ                        (0x01)
#define TDA182I5a_REG_MAP_NB_BYTES                       (0x5E)

#define TDA182I5a_REG_DATA_LEN(_FIRST_REG, _LAST_REG)    ( (_LAST_REG.Address - _FIRST_REG.Address) + 1)

/* TDA182I5a Register ID_byte_1 0x00 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_1;
/* Ident_1 bit(s): type number information (MSB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_1__Ident_1;


/* TDA182I5a Register ID_byte_2 0x01 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_2;
/* Ident_2 bit(s): type number information (LSB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_2__Ident_2;


typedef enum _TDA182I5a_Fab_rev_Item_t {
    TDA182I5a_Fab_rev_SSMC = 0,		/*  SSMC   */
    TDA182I5a_Fab_rev_ICN8 = 1		/*  ICN8  */
} TDA182I5a_Fab_rev_t, *pTDA182I5a_Fab_rev_t;

/* TDA182I5a Register ID_byte_3 0x02 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_3;
/* Major_rev bit(s): device major revision */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_3__Major_rev;
/* Fab_rev bit(s): fabrication unit revision */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_3__Fab_rev;
/* Minor_rev bit(s): device minor revision */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_ID_byte_3__Minor_rev;


/* TDA182I5a Register Thermo_byte_1 0x03 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Thermo_byte_1;
/* TM_D bit(s): junction temperature measurement */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Thermo_byte_1__TM_D;


/* TDA182I5a Register Thermo_byte_2 0x04 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Thermo_byte_2;
/* TM_ON bit(s): temperature sensor ON or OFF */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Thermo_byte_2__TM_ON;


/* TDA182I5a Register Power_state_byte_1 0x05 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_1;
/* POR bit(s): Power On Reset bit */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_1__POR;
/* AGCs_Max bit(s): maximum gain status indicator */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_1__AGCs_Max;
/* AGCs_Lock bit(s): AGC convergence status indicator */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_1__AGCs_Lock;
/* Vsync_Lock bit(s): Vertical Blanking Interval status indicator */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_1__Vsync_Lock;
/* LO_Lock bit(s): PLL lock status indicator */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_1__LO_Lock;


typedef enum _TDA182I5a_power_state_mode_Item_t {
    TDA182I5a_power_state_mode_Normal = 0,		/*  Normal   */
    TDA182I5a_power_state_mode_Standby = 1,		/*  Standby   */
    TDA182I5a_power_state_mode_Standby_p_LT = 2,		/*  Standby + LT   */
    TDA182I5a_power_state_mode_Image_Rejection_mode = 3		/*  Image Rejection mode  */
} TDA182I5a_power_state_mode_t, *pTDA182I5a_power_state_mode_t;

/* TDA182I5a Register Power_state_byte_2 0x06 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_2;
/* power_state_mode bit(s): selects the power state mode */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_state_byte_2__power_state_mode;


/* TDA182I5a Register Power_Level_byte_1 0x07 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_Level_byte_1;
/* Det6_level bit(s): Detector 6 level indicator */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_Level_byte_1__Det6_level;


/* TDA182I5a Register IRQ_status 0x08 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status;
/* IRQ_status bit(s): Interupt Request Status */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__IRQ_status;
/* MSM_IRCAL_End bit(s): Image Rejection Calibration completion */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__MSM_IRCAL_End;
/* XtalCal_End bit(s): Xtal oscillator calibration completion */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__XtalCal_End;
/* MSM_RSSI_End bit(s): RSSI measurement completion */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__MSM_RSSI_End;
/* MSM_RCCal_End bit(s): RC calibration completion */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__MSM_RCCal_End;
/* VCO_Ampl_End bit(s): VCO amplitude calibration completion */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__VCO_Ampl_End;
/* VCO_Freq_End bit(s): VCO frequency calibration completion */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_status__VCO_Freq_End;


/* TDA182I5a Register IRQ_clear 0x0A */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear;
/* IRQ_Clear bit(s): Clear IRQ */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__IRQ_Clear;
/* MSM_IRCAL_Clear bit(s): Clear IRCAL */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__MSM_IRCAL_Clear;
/* XtalCal_Clear bit(s): Clear XtalCAL */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__XtalCal_Clear;
/* MSM_RSSI_Clear bit(s): Clear RSSI */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__MSM_RSSI_Clear;
/* MSM_RCCal_Clear bit(s): Clear RCCAL */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__MSM_RCCal_Clear;
/* VCO_Ampl_Clear bit(s): Clear VCO_Amp */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__VCO_Ampl_Clear;
/* VCO_Freq_Clear bit(s): Clear VCO_Freq */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IRQ_clear__VCO_Freq_Clear;


typedef enum _TDA182I5a_GPIO2_Item_t {
    TDA182I5a_GPIO2_Off = 0,		/*  Off   */
    TDA182I5a_GPIO2_On = 1		/*  On  */
} TDA182I5a_GPIO2_t, *pTDA182I5a_GPIO2_t;

typedef enum _TDA182I5a_GPIO1_Item_t {
    TDA182I5a_GPIO1_Off = 0,		/*  Off   */
    TDA182I5a_GPIO1_On = 1		/*  On  */
} TDA182I5a_GPIO1_t, *pTDA182I5a_GPIO1_t;

/* TDA182I5a Register Gpio_ctrl_byte 0x0C */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Gpio_ctrl_byte;
/* GPIO2 bit(s): enables / disables GPO2 output */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Gpio_ctrl_byte__GPIO2;
/* GPIO1 bit(s): enables / disables GPO1 output */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Gpio_ctrl_byte__GPIO1;


typedef enum _TDA182I5a_AGC1_Top_Strategy_Item_t {
    TDA182I5a_AGC1_Top_Strategy_No_dynamic_TOP_change = 0,		/*  No dynamic TOP change   */
    TDA182I5a_AGC1_Top_Strategy_n4_dB_TOP_Do_for_LNA_gain_sup_10_dB = 1,		/*  -4 dB TOP Do for LNA gain &gt; 10 dB   */
    TDA182I5a_AGC1_Top_Strategy_n4_dB_TOP_Do_for_LNA_gain_sup_13_dB = 2,		/*  -4 dB TOP Do for LNA gain &gt; 13 dB   */
    TDA182I5a_AGC1_Top_Strategy_n4_dB_TOP_Do_for_LNA_gain_sup_16_dB = 3,		/*  -4 dB TOP Do for LNA gain &gt; 16 dB   */
    TDA182I5a_AGC1_Top_Strategy_n6_dB_TOP_Do_for_LNA_gain_sup_10_dB = 4,		/*  -6 dB TOP Do for LNA gain &gt; 10 dB   */
    TDA182I5a_AGC1_Top_Strategy_n6_dB_TOP_Do_for_LNA_gain_sup_13_dB = 5,		/*  -6 dB TOP Do for LNA gain &gt; 13 dB   */
    TDA182I5a_AGC1_Top_Strategy_n6_dB_TOP_Do_for_LNA_gain_sup_16_dB = 6,		/*  -6 dB TOP Do for LNA gain &gt; 16 dB   */
    TDA182I5a_AGC1_Top_Strategy_n8_dB_TOP_Do_for_LNA_gain_sup_13_dB = 7		/*  -8 dB TOP Do for LNA gain &gt; 13 dB  */
} TDA182I5a_AGC1_Top_Strategy_t, *pTDA182I5a_AGC1_Top_Strategy_t;

typedef enum _TDA182I5a_AGC1_Top_Item_t {
    TDA182I5a_AGC1_Top_90_84_dBuVrms = 0,		/*  90/84 dBuVrms  */
    TDA182I5a_AGC1_Top_92_86_dBuVrms = 1,		/*  92/86 dBuVrms  */
    TDA182I5a_AGC1_Top_94_88_dBuVrms = 2,		/*  94/88 dBuVrms  */
    TDA182I5a_AGC1_Top_96_90_dBuVrms = 3,		/*  96/90 dBuVrms  */
    TDA182I5a_AGC1_Top_98_92_dBuVrms = 4,		/*  98/92 dBuVrms  */
    TDA182I5a_AGC1_Top_100_94_dBuVrms = 5,		/*  100/94 dBuVrms  */
    TDA182I5a_AGC1_Top_102_96_dBuVrms = 6,		/*  102/96 dBuVrms  */
    TDA182I5a_AGC1_Top_104_98_dBuVrms = 7,		/*  104/98 dBuVrms  */
    TDA182I5a_AGC1_Top_106_100_dBuVrms = 8,		/*  106/100 dBuVrms  */
    TDA182I5a_AGC1_Top_108_102_dBuVrms = 9,		/*  108/102 dBuVrms  */
    TDA182I5a_AGC1_Top_108_102_val10_dBuVrms = 10,		/*  108/102 (val10) dBuVrms  */
    TDA182I5a_AGC1_Top_108_102_val11_dBuVrms = 11,		/*  108/102 (val11) dBuVrms  */
    TDA182I5a_AGC1_Top_108_102_val12_dBuVrms = 12,		/*  108/102 (val12) dBuVrms  */
    TDA182I5a_AGC1_Top_108_102_val13_dBuVrms = 13,		/*  108/102 (val13) dBuVrms  */
    TDA182I5a_AGC1_Top_108_102_val14_dBuVrms = 14,		/*  108/102 (val14) dBuVrms  */
    TDA182I5a_AGC1_Top_108_102_val15_dBuVrms = 15		/*  108/102 (val15) dBuVrms */
} TDA182I5a_AGC1_Top_t, *pTDA182I5a_AGC1_Top_t;

/* TDA182I5a Register AGC1_byte_1 0x0D */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_1;
/* LNA_HiZ bit(s): enables / disables the LNA_Hiz control */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_1__LNA_HiZ;
/* AGC1_Top_Strategy bit(s): selects AGC1 TOP strategy algorithm (function of AGC1 gain) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_1__AGC1_Top_Strategy;
/* AGC1_Top bit(s): selects  AGC1 TOP Down / TOP Up (dBuV) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_1__AGC1_Top;


typedef enum _TDA182I5a_AGC145_Up_step_Item_t {
    TDA182I5a_AGC145_Up_step_524_ms = 0,		/*  524 ms  */
    TDA182I5a_AGC145_Up_step_262_ms = 1,		/*  262 ms  */
    TDA182I5a_AGC145_Up_step_131_ms = 2,		/*  131 ms  */
    TDA182I5a_AGC145_Up_step_65_ms = 3,		/*  65 ms  */
    TDA182I5a_AGC145_Up_step_32_ms = 4,		/*  32 ms  */
    TDA182I5a_AGC145_Up_step_16_ms = 5,		/*  16 ms  */
    TDA182I5a_AGC145_Up_step_16_val6_ms = 6,		/*  16 (val6) ms  */
    TDA182I5a_AGC145_Up_step_16_val7_ms = 7		/*  16 (val7) ms */
} TDA182I5a_AGC145_Up_step_t, *pTDA182I5a_AGC145_Up_step_t;

/* TDA182I5a Register AGC1_byte_2 0x0E */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_2;
/* AGC1_cc_Freeze_Enable bit(s): AGC1 Freeze algorithm selection */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_2__AGC1_cc_Freeze_Enable;
/* AGC1_cc_Wide_Enable bit(s): AGC1 Wide algorithm selection */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_2__AGC1_cc_Wide_Enable;
/* AGC145_Up_step bit(s): selects AGC1, AGC4 & AGC5 up time constant (ms) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_2__AGC145_Up_step;


typedef enum _TDA182I5a_AGC1_Do_step_Item_t {
    TDA182I5a_AGC1_Do_step_8d192_ms = 0,		/*  8.192 ms  */
    TDA182I5a_AGC1_Do_step_2d048_ms = 1,		/*  2.048 ms  */
    TDA182I5a_AGC1_Do_step_0d256_ms = 2,		/*  0.256 ms  */
    TDA182I5a_AGC1_Do_step_0d128_ms = 3		/*  0.128 ms */
} TDA182I5a_AGC1_Do_step_t, *pTDA182I5a_AGC1_Do_step_t;

typedef enum _TDA182I5a_AGC1_Gain_Item_t {
    TDA182I5a_AGC1_Gain_n5_dB = 0,		/*  -5 dB  */
    TDA182I5a_AGC1_Gain_n2_dB = 1,		/*  -2 dB  */
    TDA182I5a_AGC1_Gain_1_dB = 2,		/*  1 dB  */
    TDA182I5a_AGC1_Gain_4_dB = 3,		/*  4 dB  */
    TDA182I5a_AGC1_Gain_7_dB = 4,		/*  7 dB  */
    TDA182I5a_AGC1_Gain_10_dB = 5,		/*  10 dB  */
    TDA182I5a_AGC1_Gain_13_dB = 6,		/*  13 dB  */
    TDA182I5a_AGC1_Gain_16_dB = 7,		/*  16 dB  */
    TDA182I5a_AGC1_Gain_19_dB = 8,		/*  19 dB  */
    TDA182I5a_AGC1_Gain_22_dB = 9,		/*  22 dB  */
    TDA182I5a_AGC1_Gain_25_dB = 10,		/*  25 dB  */
    TDA182I5a_AGC1_Gain_28_dB = 11		/*  28 dB */
} TDA182I5a_AGC1_Gain_t, *pTDA182I5a_AGC1_Gain_t;

/* TDA182I5a Register AGC1_byte_3 0x0F */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_3;
/* AGC1_Do_step bit(s): selects AGC1 down time constant (ms) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_3__AGC1_Do_step;
/* Force_AGC1_gain bit(s): enables / disables the manual control on AGC1 control loop */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_3__Force_AGC1_gain;
/* AGC1_Gain bit(s): selects AGC1 gain value if Force_AGC1_gain = 1 (manual .forced. mode) (dB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_byte_3__AGC1_Gain;


typedef enum _TDA182I5a_LT_offset_Item_t {
    TDA182I5a_LT_offset_No_offset_Rfin_to_LT_gain_0dB = 0,		/*  No offset (Rfin to LT gain : 0dB)   */
    TDA182I5a_LT_offset_1_step_offset_Rfin_to_LT_gain_p3dB = 1,		/*  1 step offset (Rfin to LT gain : +3dB)   */
    TDA182I5a_LT_offset_2_steps_offset_Rfin_to_LT_gain_p6dB = 2,		/*  2 steps offset (Rfin to LT gain : +6dB)   */
    TDA182I5a_LT_offset_2_steps_offset_val3 = 3		/*  2 steps offset (val3)  */
} TDA182I5a_LT_offset_t, *pTDA182I5a_LT_offset_t;

typedef enum _TDA182I5a_LT_gain_Item_t {
    TDA182I5a_LT_gain_7_dB = 0,		/*  7 dB  */
    TDA182I5a_LT_gain_10_dB = 1,		/*  10 dB  */
    TDA182I5a_LT_gain_13_dB = 2,		/*  13 dB  */
    TDA182I5a_LT_gain_16_dB = 3,		/*  16 dB  */
    TDA182I5a_LT_gain_19_dB = 4,		/*  19 dB  */
    TDA182I5a_LT_gain_22_dB = 5,		/*  22 dB  */
    TDA182I5a_LT_gain_25_dB = 6,		/*  25 dB  */
    TDA182I5a_LT_gain_28_dB = 7		/*  28 dB */
} TDA182I5a_LT_gain_t, *pTDA182I5a_LT_gain_t;

/* TDA182I5a Register LT_byte_1 0x10 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_LT_byte_1;
/* LT_offset bit(s): allows to set the LT offset (offset vs LT atten & LNA gain) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_LT_byte_1__LT_offset;
/* LT_force bit(s): enables / disables the manual control on LT control loop */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_LT_byte_1__LT_force;
/* LT_gain bit(s): selects LT attenuation value if LT_force = 1 (manual .forced. mode) (dB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_LT_byte_1__LT_gain;


typedef enum _TDA182I5a_AGC2_Top_Strategy_Item_t {
    TDA182I5a_AGC2_Top_Strategy_No_dynamic_TOP_change = 0,		/*  No dynamic TOP change   */
    TDA182I5a_AGC2_Top_Strategy_n1dB_n6dB_TOP_Do_for_LNA_gain_sup_4dB_10dB = 1,		/*  -1dB /-6dB TOP Do for LNA gain &gt; 4dB/10dB   */
    TDA182I5a_AGC2_Top_Strategy_n1dB_n6dB_TOP_Do_for_LNA_gain_sup_7dB_13dB = 2,		/*  -1dB /-6dB TOP Do for LNA gain &gt; 7dB/13dB   */
    TDA182I5a_AGC2_Top_Strategy_n1dB_n6dB_TOP_Do_for_LNA_gain_sup_10dB_16dB = 3,		/*  -1dB /-6dB TOP Do for LNA gain &gt; 10dB/16dB   */
    TDA182I5a_AGC2_Top_Strategy_n4_dB_TOP_Do_for_LNA_gain_sup_13_dB = 4,		/*  -4 dB TOP Do for LNA gain &gt; 13 dB   */
    TDA182I5a_AGC2_Top_Strategy_n5_dB_TOP_Do_for_LNA_gain_sup_13_dB = 5,		/*  -5 dB TOP Do for LNA gain &gt; 13 dB   */
    TDA182I5a_AGC2_Top_Strategy_n1dB_n7dB_TOP_Do_for_LNA_gain_sup_7dB_13dB = 6,		/*  -1dB /-7dB TOP Do for LNA gain &gt; 7dB/13dB   */
    TDA182I5a_AGC2_Top_Strategy_n2dB_n8dB_TOP_Do_for_LNA_gain_sup_7dB_13dB = 7		/*  -2dB /-8dB TOP Do for LNA gain &gt; 7dB/13dB  */
} TDA182I5a_AGC2_Top_Strategy_t, *pTDA182I5a_AGC2_Top_Strategy_t;

typedef enum _TDA182I5a_AGC2_TOP_Item_t {
    TDA182I5a_AGC2_TOP_99_93_92d5_dBuVrms = 0,		/*  99/93/92.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_101_95_94d5_dBuVrms = 1,		/*  101/95/94.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_103_97_96d5_dBuVrms = 2,		/*  103/97/96.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_105_99_98d5_dBuVrms = 3,		/*  105/99/98.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_107_101_100d5_dBuVrms = 4,		/*  107/101/100.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_109_103_102d5_dBuVrms = 5,		/*  109/103/102.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_110_104_103d5_dBuVrms = 6,		/*  110/104/103.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_111_105_104d5_dBuVrms = 7,		/*  111/105/104.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_112_106_105d5_dBuVrms = 8,		/*  112/106/105.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_113_107_106d5_dBuVrms = 9,		/*  113/107/106.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_114_108_107d5_dBuVrms = 10,		/*  114/108/107.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_115_109_108d5_dBuVrms = 11,		/*  115/109/108.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_116_110_109d5_dBuVrms = 12,		/*  116/110/109.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_117_111_110d5_dBuVrms = 13,		/*  117/111/110.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_118_112_111d5_dBuVrms = 14,		/*  118/112/111.5 dBuVrms  */
    TDA182I5a_AGC2_TOP_119_113_112d5_dBuVrms = 15		/*  119/113/112.5 dBuVrms */
} TDA182I5a_AGC2_TOP_t, *pTDA182I5a_AGC2_TOP_t;

/* TDA182I5a Register AGC2_byte_1 0x11 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_byte_1;
/* AGC2_Top_Strategy bit(s): selects AGC2 TOP strategy algorithm (function of AGC1 gain) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_byte_1__AGC2_Top_Strategy;
/* AGC2_TOP bit(s): selects AGC2 TOP Overload / Down / TOP Up (dBuV) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_byte_1__AGC2_TOP;


/* TDA182I5a Register AGC2_byte_2 0x12 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_byte_2;
/* AGC2_Gain bit(s): selects AGC2 gain value if Force_AGC2_gain = 1 (manual .forced. mode) (dB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_byte_2__AGC2_Gain;


typedef enum _TDA182I5a_AGC2_Do_step_Item_t {
    TDA182I5a_AGC2_Do_step_8d192_ms = 0,		/*  8.192 ms  */
    TDA182I5a_AGC2_Do_step_2d048_ms = 1,		/*  2.048 ms  */
    TDA182I5a_AGC2_Do_step_0d256_ms = 2,		/*  0.256 ms  */
    TDA182I5a_AGC2_Do_step_0d128_ms = 3		/*  0.128 ms */
} TDA182I5a_AGC2_Do_step_t, *pTDA182I5a_AGC2_Do_step_t;

/* TDA182I5a Register RF_byte 0x13 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_byte;
/* PLD_CC_Enable bit(s): Power Level detector configuration */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_byte__PLD_CC_Enable;
/* AGCs_loop_off bit(s): enables / disables all AGC detector clocks */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_byte__AGCs_loop_off;
/* AGC2_Do_step bit(s): selects AGC2 down time constant (ms) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_byte__AGC2_Do_step;
/* Force_AGC2_gain bit(s): enables / disables the manual control on AGC2 control loop */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_byte__Force_AGC2_gain;


typedef enum _TDA182I5a_AGC2_Up_step_Item_t {
    TDA182I5a_AGC2_Up_step_524_ms = 0,		/*  524 ms  */
    TDA182I5a_AGC2_Up_step_262_ms = 1,		/*  262 ms  */
    TDA182I5a_AGC2_Up_step_131_ms = 2,		/*  131 ms  */
    TDA182I5a_AGC2_Up_step_65_ms = 3,		/*  65 ms  */
    TDA182I5a_AGC2_Up_step_32_ms = 4,		/*  32 ms  */
    TDA182I5a_AGC2_Up_step_16_ms = 5,		/*  16 ms  */
    TDA182I5a_AGC2_Up_step_16_val_6_ms = 6,		/*  16 (val 6) ms  */
    TDA182I5a_AGC2_Up_step_16_val_7_ms = 7		/*  16 (val 7) ms */
} TDA182I5a_AGC2_Up_step_t, *pTDA182I5a_AGC2_Up_step_t;

typedef enum _TDA182I5a_AGCK_Mode_Item_t {
    TDA182I5a_AGCK_Mode_1_step_each_VSYNC_rising_edge_ms = 0,		/*  1 step each VSYNC rising edge ms  */
    TDA182I5a_AGCK_Mode_0d512_ms = 1,		/*  0.512 ms  */
    TDA182I5a_AGCK_Mode_8d192_ms = 2,		/*  8.192 ms  */
    TDA182I5a_AGCK_Mode_32d768_ms = 3		/*  32.768 ms */
} TDA182I5a_AGCK_Mode_t, *pTDA182I5a_AGCK_Mode_t;

/* TDA182I5a Register AGCK_byte_1 0x14 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGCK_byte_1;
/* AGC2_Up_step bit(s): selects AGC2 up time constant (ms) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGCK_byte_1__AGC2_Up_step;
/* AGCK_Mode bit(s): selects AGCK clock  (ms) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGCK_byte_1__AGCK_Mode;


typedef enum _TDA182I5a_hrm_swap_Item_t {
    TDA182I5a_hrm_swap_No_RF_spectrum_inversion = 0,		/*  No RF spectrum inversion   */
    TDA182I5a_hrm_swap_RF_spectrum_inversion_n_swap_I = 1,		/*  RF spectrum inversion - swap I   */
    TDA182I5a_hrm_swap_RF_spectrum_inversion_n_swap_Q = 2,		/*  RF spectrum inversion - swap Q   */
    TDA182I5a_hrm_swap_No_RF_spectrum_inversion_n_both_I_and_Q_swap = 3		/*  No RF spectrum inversion - both I and Q swap  */
} TDA182I5a_hrm_swap_t, *pTDA182I5a_hrm_swap_t;

typedef enum _TDA182I5a_AGC4_TOP_Item_t {
    TDA182I5a_AGC4_TOP_105_100_dBuVrms = 0,		/*  105/100 dBuVrms  */
    TDA182I5a_AGC4_TOP_107_102_dBuVrms = 1,		/*  107/102 dBuVrms  */
    TDA182I5a_AGC4_TOP_110_105_dBuVrms = 2,		/*  110/105 dBuVrms  */
    TDA182I5a_AGC4_TOP_112_107_dBuVrms = 3		/*  112/107 dBuVrms */
} TDA182I5a_AGC4_TOP_t, *pTDA182I5a_AGC4_TOP_t;

/* TDA182I5a Register IR_Mixer_byte_1 0x15 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_1;
/* hrm_swap bit(s): Allows to select the RF spectral inversion mode */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_1__hrm_swap;
/* AGC4_TOP bit(s): selects  AGC4 TOP Down / TOP Up (dBuV) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_1__AGC4_TOP;


typedef enum _TDA182I5a_AGC4_Do_step_Item_t {
    TDA182I5a_AGC4_Do_step_8d192_ms = 0,		/*  8.192 ms  */
    TDA182I5a_AGC4_Do_step_2d048_ms = 1,		/*  2.048 ms  */
    TDA182I5a_AGC4_Do_step_0d256_ms = 2,		/*  0.256 ms  */
    TDA182I5a_AGC4_Do_step_0d128_ms = 3		/*  0.128 ms */
} TDA182I5a_AGC4_Do_step_t, *pTDA182I5a_AGC4_Do_step_t;

typedef enum _TDA182I5a_AGC4_Gain_Item_t {
    TDA182I5a_AGC4_Gain_2_dB = 0,		/*  2 dB  */
    TDA182I5a_AGC4_Gain_5_dB = 1,		/*  5 dB  */
    TDA182I5a_AGC4_Gain_8_dB = 2,		/*  8 dB  */
    TDA182I5a_AGC4_Gain_11_dB = 3,		/*  11 dB  */
    TDA182I5a_AGC4_Gain_14_dB = 4,		/*  14 dB  */
    TDA182I5a_AGC4_Gain_val_5_Forbidden_dB = 5,		/*  val 5 Forbidden dB  */
    TDA182I5a_AGC4_Gain_val_6_Forbidden_dB = 6,		/*  val 6 Forbidden dB  */
    TDA182I5a_AGC4_Gain_val_7_Forbidden_dB = 7		/*  val 7 Forbidden dB */
} TDA182I5a_AGC4_Gain_t, *pTDA182I5a_AGC4_Gain_t;

/* TDA182I5a Register IR_Mixer_byte_2 0x16 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_2;
/* AGC4_Do_step bit(s): selects AGC4 down time constant (ms) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_2__AGC4_Do_step;
/* Force_AGC4_gain bit(s): enables / disables the manual control on AGC4 control loop */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_2__Force_AGC4_gain;
/* AGC4_Gain bit(s): selects AGC4 gain value if Force_AGC4_gain = 1 (manual .forced. mode) (dB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Mixer_byte_2__AGC4_Gain;


typedef enum _TDA182I5a_RSSI_HP_Fc_Item_t {
    TDA182I5a_RSSI_HP_Fc_0d3_MHz = 0,		/*  0.3 MHz  */
    TDA182I5a_RSSI_HP_Fc_2d25_MHz = 1		/*  2.25 MHz */
} TDA182I5a_RSSI_HP_Fc_t, *pTDA182I5a_RSSI_HP_Fc_t;

typedef enum _TDA182I5a_AGC5_HP_Fc_Item_t {
    TDA182I5a_AGC5_HP_Fc_0d3_MHz = 0,		/*  0.3 MHz  */
    TDA182I5a_AGC5_HP_Fc_2d25_MHz = 1		/*  2.25 MHz */
} TDA182I5a_AGC5_HP_Fc_t, *pTDA182I5a_AGC5_HP_Fc_t;

typedef enum _TDA182I5a_AGC5_TOP_Item_t {
    TDA182I5a_AGC5_TOP_105_100_dBuVrms = 0,		/*  105/100 dBuVrms  */
    TDA182I5a_AGC5_TOP_107_102_dBuVrms = 1,		/*  107/102 dBuVrms  */
    TDA182I5a_AGC5_TOP_110_105_dBuVrms = 2,		/*  110/105 dBuVrms  */
    TDA182I5a_AGC5_TOP_112_107_dBuVrms = 3		/*  112/107 dBuVrms */
} TDA182I5a_AGC5_TOP_t, *pTDA182I5a_AGC5_TOP_t;

/* TDA182I5a Register AGC5_byte_1 0x17 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_1;
/* RSSI_HP_Fc bit(s): selects RSSI HPF cut off frequency (MHz) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_1__RSSI_HP_Fc;
/* AGC5_HP_Fc bit(s): selects AGC5 highpass filter cut off frequency (MHz) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_1__AGC5_HP_Fc;
/* AGC5_TOP bit(s): selects  AGC5 TOP Down / TOP Up (dBuV) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_1__AGC5_TOP;


typedef enum _TDA182I5a_AGC5_Do_step_Item_t {
    TDA182I5a_AGC5_Do_step_8d192_ms = 0,		/*  8.192 ms  */
    TDA182I5a_AGC5_Do_step_2d048_ms = 1,		/*  2.048 ms  */
    TDA182I5a_AGC5_Do_step_0d256_ms = 2,		/*  0.256 ms  */
    TDA182I5a_AGC5_Do_step_0d128_ms = 3		/*  0.128 ms */
} TDA182I5a_AGC5_Do_step_t, *pTDA182I5a_AGC5_Do_step_t;

typedef enum _TDA182I5a_AGC5_Gain_Item_t {
    TDA182I5a_AGC5_Gain_n6_or_6d4_dB = 0,		/*  -6  or 6.4  dB  */
    TDA182I5a_AGC5_Gain_n3_or_9d4_dB = 1,		/*  -3 or 9.4  dB  */
    TDA182I5a_AGC5_Gain_0_or_12d4_dB = 2,		/*  0  or 12.4  dB  */
    TDA182I5a_AGC5_Gain_3_or_15d4_dB = 3,		/*  3  or 15.4  dB  */
    TDA182I5a_AGC5_Gain_6_or_18d4_dB = 4,		/*  6  or 18.4  dB  */
    TDA182I5a_AGC5_Gain_9_or_21d4_dB = 5,		/*  9  or 21.4  dB  */
    TDA182I5a_AGC5_Gain_12_or_24d4_dB = 6,		/*  12  or 24.4  dB  */
    TDA182I5a_AGC5_Gain_15_or_27d4_dB = 7		/*  15  or 27.4  dB */
} TDA182I5a_AGC5_Gain_t, *pTDA182I5a_AGC5_Gain_t;

/* TDA182I5a Register AGC5_byte_2 0x18 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_2;
/* AGC5_Do_step bit(s): selects AGC5 down time constant (ms) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_2__AGC5_Do_step;
/* Force_AGC5_gain bit(s): enables / disables the manual control on AGC5 control loop */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_2__Force_AGC5_gain;
/* AGC5_Gain bit(s): selects AGC5 gain value if Force_AGC5_gain = 1 (manual .forced. mode) (dB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC5_byte_2__AGC5_Gain;


typedef enum _TDA182I5a_Top_adapt_AGC2_Item_t {
    TDA182I5a_Top_adapt_AGC2_No_dynamic_TOP_change = 0,		/*  No dynamic TOP change   */
    TDA182I5a_Top_adapt_AGC2_n3_dB_TOP_for_IFLPF_gain_inf_9dB_12dB = 1,		/*  -3 dB TOP for IFLPF gain &lt; 9dB / 12dB   */
    TDA182I5a_Top_adapt_AGC2_n5_dB_TOP_for_IFLPF_gain_inf_9dB_12dB = 2,		/*  -5 dB TOP for IFLPF gain &lt; 9dB / 12dB   */
    TDA182I5a_Top_adapt_AGC2_n7_dB_TOP_for_IFLPF_gain_inf_9dB_12dB = 3		/*  -7 dB TOP for IFLPF gain &lt; 9dB / 12dB  */
} TDA182I5a_Top_adapt_AGC2_t, *pTDA182I5a_Top_adapt_AGC2_t;

typedef enum _TDA182I5a_Top_adapt_AGC1_Item_t {
    TDA182I5a_Top_adapt_AGC1_No_dynamic_TOP_change = 0,		/*  No dynamic TOP change   */
    TDA182I5a_Top_adapt_AGC1_n4_dB_TOP_for_IFLPF_gain_inf_9dB_12dB = 1,		/*  -4 dB TOP for IFLPF gain &lt; 9dB / 12dB   */
    TDA182I5a_Top_adapt_AGC1_n6_dB_TOP_for_IFLPF_gain_inf_9dB_12dB = 2,		/*  -6 dB TOP for IFLPF gain &lt; 9dB / 12dB   */
    TDA182I5a_Top_adapt_AGC1_n8_dB_TOP_for_IFLPF_gain_inf_9dB_12dB = 3		/*  -8 dB TOP for IFLPF gain &lt; 9dB / 12dB  */
} TDA182I5a_Top_adapt_AGC1_t, *pTDA182I5a_Top_adapt_AGC1_t;

/* TDA182I5a Register Adapt_Top_byte 0x19 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Adapt_Top_byte;
/* Top_adapt_AGC2 bit(s): selects AGC2 TOP adapt algorithm (function of AGC5 gain) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Adapt_Top_byte__Top_adapt_AGC2;
/* Top_adapt_AGC1 bit(s): selects AGC1 TOP adapt algorithm (function of AGC5 gain) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Adapt_Top_byte__Top_adapt_AGC1;


typedef enum _TDA182I5a_PD_Vsync_Mgt_Item_t {
    TDA182I5a_PD_Vsync_Mgt_Vsync_Management_enabled = 0,		/*  Vsync Management enabled   */
    TDA182I5a_PD_Vsync_Mgt_Vsync_Management_disabled = 1		/*  Vsync Management disabled  */
} TDA182I5a_PD_Vsync_Mgt_t, *pTDA182I5a_PD_Vsync_Mgt_t;

typedef enum _TDA182I5a_Neg_Modulation_Item_t {
    TDA182I5a_Neg_Modulation_Disabled = 0,		/*  Disabled   */
    TDA182I5a_Neg_Modulation_Enabled = 1		/*  Enabled  */
} TDA182I5a_Neg_Modulation_t, *pTDA182I5a_Neg_Modulation_t;

/* TDA182I5a Register Vsync_byte 0x1A */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Vsync_byte;
/* PD_Vsync_Mgt bit(s): enables / disables the automatic management of AGC clocking in ATV reception mode */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Vsync_byte__PD_Vsync_Mgt;
/* Neg_Modulation bit(s): selects whether the received analog TV channel uses negative or positive modulation */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Vsync_byte__Neg_Modulation;


typedef enum _TDA182I5a_AGC_Ovld_TOP_Item_t {
    TDA182I5a_AGC_Ovld_TOP_p3d5_n3d5 = 0,		/*  +3.5/-3.5   */
    TDA182I5a_AGC_Ovld_TOP_p4d5_n26 = 1,		/*  +4.5/-26   */
    TDA182I5a_AGC_Ovld_TOP_p4d5_n3d5 = 2,		/*  +4.5/-3.5   */
    TDA182I5a_AGC_Ovld_TOP_p6_n4d5 = 3,		/*  +6/-4.5   */
    TDA182I5a_AGC_Ovld_TOP_p6_n6 = 4,		/*  +6/-6   */
    TDA182I5a_AGC_Ovld_TOP_p6_n9 = 5,		/*  +6/-9   */
    TDA182I5a_AGC_Ovld_TOP_p7d5_n9 = 6,		/*  +7.5/-9   */
    TDA182I5a_AGC_Ovld_TOP_p7d5_n9_val7 = 7		/*  +7.5/-9 (val7)  */
} TDA182I5a_AGC_Ovld_TOP_t, *pTDA182I5a_AGC_Ovld_TOP_t;

/* TDA182I5a Register Ovld_Udld_byte 0x1B */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Ovld_Udld_byte;
/* RF_udld_disable bit(s): enables / disables the RF underload detection */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Ovld_Udld_byte__RF_udld_disable;
/* IF_ovld_disable bit(s): enables / disables the IF overload detection */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Ovld_Udld_byte__IF_ovld_disable;
/* RF_ovld_disable bit(s): enables / disables the RF overload detection */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Ovld_Udld_byte__RF_ovld_disable;
/* IF_udld_disable bit(s): enables/ disabled the IF underload mechanism */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Ovld_Udld_byte__IF_udld_disable;
/* AGC_Ovld_TOP bit(s): selects IF overload / IF underload ranges (dB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Ovld_Udld_byte__AGC_Ovld_TOP;


typedef enum _TDA182I5a_IF_LPF_Offset_Item_t {
    TDA182I5a_IF_LPF_Offset_0 = 0,		/*  0   */
    TDA182I5a_IF_LPF_Offset_4 = 1,		/*  4   */
    TDA182I5a_IF_LPF_Offset_8 = 2,		/*  8   */
    TDA182I5a_IF_LPF_Offset_12 = 3		/*  12  */
} TDA182I5a_IF_LPF_Offset_t, *pTDA182I5a_IF_LPF_Offset_t;

typedef enum _TDA182I5a_IF_LPF_sign_Item_t {
    TDA182I5a_IF_LPF_sign_Positive = 0,		/*  Positive   */
    TDA182I5a_IF_LPF_sign_Negative = 1		/*  Negative  */
} TDA182I5a_IF_LPF_sign_t, *pTDA182I5a_IF_LPF_sign_t;

typedef enum _TDA182I5a_DC_Notch_Item_t {
    TDA182I5a_DC_Notch_Disabled = 0,		/*  Disabled   */
    TDA182I5a_DC_Notch_Enabled = 1		/*  Enabled  */
} TDA182I5a_DC_Notch_t, *pTDA182I5a_DC_Notch_t;

typedef enum _TDA182I5a_LP_Fc_Item_t {
    TDA182I5a_LP_Fc_6_MHz = 0,		/*  6 MHz  */
    TDA182I5a_LP_Fc_7_MHz = 1,		/*  7 MHz  */
    TDA182I5a_LP_Fc_8_MHz = 2,		/*  8 MHz  */
    TDA182I5a_LP_Fc_9_MHz = 3,		/*  9 MHz  */
    TDA182I5a_LP_Fc_10_MHz = 4,		/*  10 MHz  */
    TDA182I5a_LP_Fc_11d25_MHz = 5,		/*  11.25 MHz  */
    TDA182I5a_LP_Fc_1d5_MHz = 6		/*  1.5 MHz */
} TDA182I5a_LP_Fc_t, *pTDA182I5a_LP_Fc_t;

/* TDA182I5a Register IF_Byte_1 0x1C */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_1;
/* IF_LPF_Offset bit(s): selects the IF LPF cut-off frequency correction factor (%) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_1__IF_LPF_Offset;
/* IF_LPF_sign bit(s): selects the IF LPF cut-off frequency correction sign */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_1__IF_LPF_sign;
/* DC_Notch bit(s): enables / disables the DC Notch in the IF polyphase filter */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_1__DC_Notch;
/* LP_Fc bit(s): selects the IF LPF cut-off frequency (MHz) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_1__LP_Fc;


typedef enum _TDA182I5a_IF_HPF_Offset_Item_t {
    TDA182I5a_IF_HPF_Offset_0 = 0,		/*  0   */
    TDA182I5a_IF_HPF_Offset_4 = 1,		/*  4   */
    TDA182I5a_IF_HPF_Offset_8 = 2,		/*  8   */
    TDA182I5a_IF_HPF_Offset_12 = 3		/*  12  */
} TDA182I5a_IF_HPF_Offset_t, *pTDA182I5a_IF_HPF_Offset_t;

typedef enum _TDA182I5a_IF_HPF_sign_Item_t {
    TDA182I5a_IF_HPF_sign_Positive = 0,		/*  Positive   */
    TDA182I5a_IF_HPF_sign_Negative = 1		/*  Negative  */
} TDA182I5a_IF_HPF_sign_t, *pTDA182I5a_IF_HPF_sign_t;

typedef enum _TDA182I5a_IF_HP_Fc_Item_t {
    TDA182I5a_IF_HP_Fc_0d5_MHz = 0,		/*  0.5 MHz  */
    TDA182I5a_IF_HP_Fc_1_MHz = 1,		/*  1 MHz  */
    TDA182I5a_IF_HP_Fc_1d6_MHz = 2,		/*  1.6 MHz  */
    TDA182I5a_IF_HP_Fc_2d3_MHz = 3,		/*  2.3 MHz  */
    TDA182I5a_IF_HP_Fc_3d5_MHz = 4		/*  3.5 MHz */
} TDA182I5a_IF_HP_Fc_t, *pTDA182I5a_IF_HP_Fc_t;

typedef enum _TDA182I5a_IF_HP_Enable_Item_t {
    TDA182I5a_IF_HP_Enable_Disabled = 0,		/*  Disabled   */
    TDA182I5a_IF_HP_Enable_Enabled = 1		/*  Enabled  */
} TDA182I5a_IF_HP_Enable_t, *pTDA182I5a_IF_HP_Enable_t;

/* TDA182I5a Register IF_Byte_2 0x1D */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_2;
/* IF_HPF_Offset bit(s): selects the IF HPF cut-off frequency correction factor (%) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_2__IF_HPF_Offset;
/* IF_HPF_sign bit(s): selects the IF HPF cut-off frequency correction sign */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_2__IF_HPF_sign;
/* IF_HP_Fc bit(s): selects the IF HPF cut-off frequency (MHz) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_2__IF_HP_Fc;
/* IF_HP_Enable bit(s): enables / disables the IF highpass filter */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_2__IF_HP_Enable;


typedef enum _TDA182I5a_IF_Notch_Offset_Item_t {
    TDA182I5a_IF_Notch_Offset_0 = 0,		/*  0   */
    TDA182I5a_IF_Notch_Offset_4 = 1,		/*  4   */
    TDA182I5a_IF_Notch_Offset_8 = 2,		/*  8   */
    TDA182I5a_IF_Notch_Offset_12 = 3		/*  12  */
} TDA182I5a_IF_Notch_Offset_t, *pTDA182I5a_IF_Notch_Offset_t;

typedef enum _TDA182I5a_IF_Notch_sign_Item_t {
    TDA182I5a_IF_Notch_sign_Positive = 0,		/*  Positive   */
    TDA182I5a_IF_Notch_sign_Negative = 1		/*  Negative  */
} TDA182I5a_IF_Notch_sign_t, *pTDA182I5a_IF_Notch_sign_t;

typedef enum _TDA182I5a_IF_Notch_Freq_Item_t {
    TDA182I5a_IF_Notch_Freq_6d25_MHz = 0,		/*  6.25 MHz  */
    TDA182I5a_IF_Notch_Freq_7d25_MHz = 1,		/*  7.25 MHz  */
    TDA182I5a_IF_Notch_Freq_8d25_MHz = 2,		/*  8.25 MHz  */
    TDA182I5a_IF_Notch_Freq_9d25_MHz = 3,		/*  9.25 MHz  */
    TDA182I5a_IF_Notch_Freq_10d25_MHz = 4,		/*  10.25 MHz  */
    TDA182I5a_IF_Notch_Freq_11d45_MHz = 5		/*  11.45 MHz */
} TDA182I5a_IF_Notch_Freq_t, *pTDA182I5a_IF_Notch_Freq_t;

typedef enum _TDA182I5a_IF_Notch_Enable_Item_t {
    TDA182I5a_IF_Notch_Enable_Disabled = 0,		/*  Disabled   */
    TDA182I5a_IF_Notch_Enable_Enabled = 1		/*  Enabled  */
} TDA182I5a_IF_Notch_Enable_t, *pTDA182I5a_IF_Notch_Enable_t;

/* TDA182I5a Register IF_Byte_3 0x1E */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_3;
/* IF_Notch_Offset bit(s): selects the IF notch center frequency correction factor (%) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_3__IF_Notch_Offset;
/* IF_Notch_sign bit(s): selects the IF notch center frequency correction sign */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_3__IF_Notch_sign;
/* IF_Notch_Freq bit(s): selects the IF notch center frequency (MHz) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_3__IF_Notch_Freq;
/* IF_Notch_Enable bit(s): enables / disables the IF notch filter */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_Byte_3__IF_Notch_Enable;


typedef enum _TDA182I5a_AGC_freq_Item_t {
    TDA182I5a_AGC_freq_62d5_kHz = 0,		/*  62.5 kHz  */
    TDA182I5a_AGC_freq_62d745_kHz = 1		/*  62.745 kHz */
} TDA182I5a_AGC_freq_t, *pTDA182I5a_AGC_freq_t;

typedef enum _TDA182I5a_Dig_Clock_Item_t {
    TDA182I5a_Dig_Clock_16MHz = 0,		/*  16MHz   */
    TDA182I5a_Dig_Clock_SubLO = 1		/*  SubLO  */
} TDA182I5a_Dig_Clock_t, *pTDA182I5a_Dig_Clock_t;

typedef enum _TDA182I5a_Xtout_Amp_Item_t {
    TDA182I5a_Xtout_Amp_200_single_18275A_400_diff_18215_mVpp = 0,		/*  200 single (18275A) / 400 diff (18215) mVpp  */
    TDA182I5a_Xtout_Amp_400_single_18275A_800_diff_18215_mVpp = 1,		/*  400 single (18275A) / 800 diff (18215) mVpp  */
    TDA182I5a_Xtout_Amp_400_single_18275A_800_diff_18215_val2_mVpp = 3,		/*  400 single (18275A) / 800 diff (18215) (val2) mVpp  */
    TDA182I5a_Xtout_Amp_400_single_18275A_800_diff_18215_val3_mVpp = 4		/*  400 single (18275A) / 800 diff (18215) (val3) mVpp */
} TDA182I5a_Xtout_Amp_t, *pTDA182I5a_Xtout_Amp_t;

typedef enum _TDA182I5a_Xtout_Item_t {
    TDA182I5a_Xtout_No_signal = 0,		/*  No signal   */
    TDA182I5a_Xtout_16Mhz_or_24MHz = 1		/*  16Mhz or 24MHz  */
} TDA182I5a_Xtout_t, *pTDA182I5a_Xtout_t;

/* TDA182I5a Register Reference_Byte 0x1F */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Reference_Byte;
/* AGC_freq bit(s): selects the AGC frequency (kHz) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Reference_Byte__AGC_freq;
/* Dig_Clock bit(s): selects the clock of the digital core */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Reference_Byte__Dig_Clock;
/* Xtout_Amp bit(s): selects XTOUT amplitude (mVpp) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Reference_Byte__Xtout_Amp;
/* Xtout bit(s): enables / disables the reference clock output XTOUT  */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Reference_Byte__Xtout;


typedef enum _TDA182I5a_IFnotchToRSSI_Item_t {
    TDA182I5a_IFnotchToRSSI_to_IF_LPF_1st_stage_output = 0,		/*  to IF_LPF 1st stage output   */
    TDA182I5a_IFnotchToRSSI_to_IF_Notch_output = 1		/*  to IF_Notch output  */
} TDA182I5a_IFnotchToRSSI_t, *pTDA182I5a_IFnotchToRSSI_t;

typedef enum _TDA182I5a_DIFOUT_En_Item_t {
    TDA182I5a_DIFOUT_En_Digital_IF_OUT_Disabled = 0,		/*  Digital IF OUT Disabled   */
    TDA182I5a_DIFOUT_En_Digital_IF_OUT_Enabled = 1		/*  Digital IF OUT Enabled  */
} TDA182I5a_DIFOUT_En_t, *pTDA182I5a_DIFOUT_En_t;

typedef enum _TDA182I5a_DVIFAGC_En_Item_t {
    TDA182I5a_DVIFAGC_En_Analog_VIF_AGC = 0,		/*  Analog VIF AGC   */
    TDA182I5a_DVIFAGC_En_Digital_VIF_AGC = 1		/*  Digital VIF AGC  */
} TDA182I5a_DVIFAGC_En_t, *pTDA182I5a_DVIFAGC_En_t;

typedef enum _TDA182I5a_IF_level_Item_t {
    TDA182I5a_IF_level_0_to_30_dB_2_Vpp = 0,		/*  0 to 30 dB (2 Vpp)   */
    TDA182I5a_IF_level_n4_to_26_dB_1d25_Vpp = 1,		/*  -4 to 26 dB (1.25 Vpp)   */
    TDA182I5a_IF_level_n6_to_24_dB_1_Vpp = 2,		/*  -6 to 24 dB (1 Vpp)   */
    TDA182I5a_IF_level_n8_to_22_dB_0d8_Vpp = 3,		/*  -8 to 22 dB (0.8 Vpp)   */
    TDA182I5a_IF_level_n7d5_to_22d5_dB_0d85_Vpp = 4,		/*  -7.5 to 22.5 dB (0.85 Vpp)   */
    TDA182I5a_IF_level_n9_to_21_dB_0d7_Vpp = 5,		/*  -9 to 21 dB (0.7 Vpp)   */
    TDA182I5a_IF_level_n10d3_to_19d7_dB_0d6_Vpp = 6,		/*  -10.3 to 19.7 dB (0.6 Vpp)   */
    TDA182I5a_IF_level_n12_to_18_dB_0d5_Vpp = 7		/*  -12 to 18 dB (0.5 Vpp)  */
} TDA182I5a_IF_level_t, *pTDA182I5a_IF_level_t;

/* TDA182I5a Register IF_AGC_byte 0x20 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_AGC_byte;
/* IFAGCINV_En bit(s): Activates / desactivates the IFAGC gain control inversion */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_AGC_byte__IFAGCINV_En;
/* IFnotchToRSSI bit(s): enables / disables the IF notch to RSSI control */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_AGC_byte__IFnotchToRSSI;
/* DIFOUT_En bit(s): enables / disables the IFOUT_D outputs */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_AGC_byte__DIFOUT_En;
/* DVIFAGC_En bit(s): selects IFAGC control path */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_AGC_byte__DVIFAGC_En;
/* IF_level bit(s): selects the IF amplifier gain range */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IF_AGC_byte__IF_level;


/* TDA182I5a Register MSM_byte_1 0x21 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_1;
/* IR_CAL_Loop bit(s): Image rejection calibration mode */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_1__IR_CAL_Loop;
/* IR_Cal_Image bit(s): Image rejection calibration selection */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_1__IR_Cal_Image;
/* RC_Cal bit(s): RC calibration selection */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_1__RC_Cal;
/* VCO_Ampl_Cal bit(s): VCO amplitude calibration selection */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_1__VCO_Ampl_Cal;
/* VCO_Freq_Meas bit(s): VCO frequency calibration selection */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_1__VCO_Freq_Meas;


/* TDA182I5a Register MSM_byte_2 0x22 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_2;
/* AGC_CC_Fast_Launch bit(s): Fast Mode trigger */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_2__AGC_CC_Fast_Launch;
/* MSM_Launch bit(s): Main State Machine execution */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_MSM_byte_2__MSM_Launch;


typedef enum _TDA182I5a_PSM_LNA_Item_t {
    TDA182I5a_PSM_LNA_0_mA = 0,		/*  0 mA  */
    TDA182I5a_PSM_LNA_n3d5_mA = 1,		/*  -3.5  mA  */
    TDA182I5a_PSM_LNA_n7_mA = 2,		/*  -7 mA  */
    TDA182I5a_PSM_LNA_n10_mA = 3		/*  -10 mA */
} TDA182I5a_PSM_LNA_t, *pTDA182I5a_PSM_LNA_t;

typedef enum _TDA182I5a_PSM_LoMixer_Item_t {
    TDA182I5a_PSM_LoMixer_0_mA = 0,		/*  0 mA  */
    TDA182I5a_PSM_LoMixer_1_mA = 1,		/*  1 mA  */
    TDA182I5a_PSM_LoMixer_2_mA = 2,		/*  2 mA  */
    TDA182I5a_PSM_LoMixer_3_mA = 3		/*  3 mA */
} TDA182I5a_PSM_LoMixer_t, *pTDA182I5a_PSM_LoMixer_t;

typedef enum _TDA182I5a_PSM_AGC2_Item_t {
    TDA182I5a_PSM_AGC2_0_mA = 0,		/*  0 mA  */
    TDA182I5a_PSM_AGC2_n3d4_mA = 1,		/*  -3.4  mA  */
    TDA182I5a_PSM_AGC2_n8_mA = 2,		/*  -8  mA  */
    TDA182I5a_PSM_AGC2_n11_mA = 3		/*  -11  mA */
} TDA182I5a_PSM_AGC2_t, *pTDA182I5a_PSM_AGC2_t;

typedef enum _TDA182I5a_PSM_Det2_Item_t {
    TDA182I5a_PSM_Det2_0_mA = 0,		/*  0 mA  */
    TDA182I5a_PSM_Det2_n1_mA = 1,		/*  -1  mA  */
    TDA182I5a_PSM_Det2_n2_mA = 2,		/*  -2  mA  */
    TDA182I5a_PSM_Det2_n3_mA = 3		/*  -3  mA */
} TDA182I5a_PSM_Det2_t, *pTDA182I5a_PSM_Det2_t;

/* TDA182I5a Register PowerSavingMode_byte_1 0x23 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_1;
/* PSM_LNA bit(s): adjusts the current in LNA amplifier (mA) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_1__PSM_LNA;
/* PSM_LoMixer bit(s): adjusts the current in LO/mixer section (mA) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_1__PSM_LoMixer;
/* PSM_AGC2 bit(s): adjusts the current in AGC2 amplifier (mA) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_1__PSM_AGC2;
/* PSM_Det2 bit(s): adjusts the current in AGC2 detector (mA) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_1__PSM_Det2;


typedef enum _TDA182I5a_Loch_curr_boost_Item_t {
    TDA182I5a_Loch_curr_boost_0 = 0,		/*  0   */
    TDA182I5a_Loch_curr_boost_1 = 1,		/*  1   */
    TDA182I5a_Loch_curr_boost_2 = 2,		/*  2   */
    TDA182I5a_Loch_curr_boost_3 = 3		/*  3  */
} TDA182I5a_Loch_curr_boost_t, *pTDA182I5a_Loch_curr_boost_t;

typedef enum _TDA182I5a_PSM_IFfilter_Item_t {
    TDA182I5a_PSM_IFfilter_0_mA = 0,		/*  0 mA  */
    TDA182I5a_PSM_IFfilter_n4d2_mA = 1,		/*  -4.2 mA  */
    TDA182I5a_PSM_IFfilter_n7_mA = 2,		/*  -7 mA  */
    TDA182I5a_PSM_IFfilter_n8d9_mA = 3		/*  -8.9  mA */
} TDA182I5a_PSM_IFfilter_t, *pTDA182I5a_PSM_IFfilter_t;

typedef enum _TDA182I5a_PSM_IFLPF_Item_t {
    TDA182I5a_PSM_IFLPF_0_mA = 0,		/*  0 mA  */
    TDA182I5a_PSM_IFLPF_n2d3_mA = 1,		/*  -2.3 mA  */
    TDA182I5a_PSM_IFLPF_n3d8_mA = 2,		/*  -3.8 mA  */
    TDA182I5a_PSM_IFLPF_n4d9_mA = 3		/*  -4.9 mA */
} TDA182I5a_PSM_IFLPF_t, *pTDA182I5a_PSM_IFLPF_t;

typedef enum _TDA182I5a_PSM_RF_LPF_Item_t {
    TDA182I5a_PSM_RF_LPF_0_mA = 0,		/*  0 mA  */
    TDA182I5a_PSM_RF_LPF_n7_mA = 1,		/*  -7 mA  */
    TDA182I5a_PSM_RF_LPF_n15_mA = 2,		/*  -15 mA  */
    TDA182I5a_PSM_RF_LPF_n23_mA = 3		/*  -23 mA */
} TDA182I5a_PSM_RF_LPF_t, *pTDA182I5a_PSM_RF_LPF_t;

/* TDA182I5a Register PowerSavingMode_byte_2 0x24 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_2;
/* Loch_curr_boost bit(s): adjusts the current in LO chain (mA) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_2__Loch_curr_boost;
/* PSM_IFfilter bit(s): adjusts the current in IF filter (mA) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_2__PSM_IFfilter;
/* PSM_IFLPF bit(s): adjusts the current in IF LPF (mA) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_2__PSM_IFLPF;
/* PSM_RF_LPF bit(s): adjusts the current in the harmonic filter (mA) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_2__PSM_RF_LPF;


typedef enum _TDA182I5a_LOdiv4_Boost_Item_t {
    TDA182I5a_LOdiv4_Boost_0_mA = 0,		/*  0 mA  */
    TDA182I5a_LOdiv4_Boost_1d8_mA = 1		/*  1.8 mA */
} TDA182I5a_LOdiv4_Boost_t, *pTDA182I5a_LOdiv4_Boost_t;

typedef enum _TDA182I5a_PSM_Mixer_Item_t {
    TDA182I5a_PSM_Mixer_0_mA = 0,		/*  0 mA  */
    TDA182I5a_PSM_Mixer_n0d65_mA = 1,		/*  -0.65  mA  */
    TDA182I5a_PSM_Mixer_n1d4_mA = 2,		/*  -1.4  mA  */
    TDA182I5a_PSM_Mixer_n3_mA = 3		/*  -3  mA */
} TDA182I5a_PSM_Mixer_t, *pTDA182I5a_PSM_Mixer_t;

typedef enum _TDA182I5a_PSM_Ifpoly_Item_t {
    TDA182I5a_PSM_Ifpoly_0_mA = 0,		/*  0 mA  */
    TDA182I5a_PSM_Ifpoly_n1d8_mA = 1,		/*  -1.8 mA  */
    TDA182I5a_PSM_Ifpoly_n3d2_mA = 2,		/*  -3.2 mA  */
    TDA182I5a_PSM_Ifpoly_n4d1_mA = 3		/*  -4.1 mA */
} TDA182I5a_PSM_Ifpoly_t, *pTDA182I5a_PSM_Ifpoly_t;

typedef enum _TDA182I5a_PSM_Lodriver_Item_t {
    TDA182I5a_PSM_Lodriver_0_mA = 0,		/*  0 mA  */
    TDA182I5a_PSM_Lodriver_n3_mA = 1,		/*  -3 mA  */
    TDA182I5a_PSM_Lodriver_n4d5_mA = 2,		/*  -4.5 mA  */
    TDA182I5a_PSM_Lodriver_n6_mA = 3		/*  -6 mA */
} TDA182I5a_PSM_Lodriver_t, *pTDA182I5a_PSM_Lodriver_t;

/* TDA182I5a Register PowerSavingMode_byte_3 0x25 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_3;
/* LOdiv4_Boost bit(s): adjusts the current of the divider by 4 in the LO section */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_3__LOdiv4_Boost;
/* PSM_Mixer bit(s): adjusts the current in the mixer (mA) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_3__PSM_Mixer;
/* PSM_Ifpoly bit(s): adjusts the current in IF polyphase filter (mA) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_3__PSM_Ifpoly;
/* PSM_Lodriver bit(s): adjusts the current in LO driver (mA) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_PowerSavingMode_byte_3__PSM_Lodriver;


typedef enum _TDA182I5a_LNA_Highpass_Item_t {
    TDA182I5a_LNA_Highpass_Disabled = 0,		/*  Disabled   */
    TDA182I5a_LNA_Highpass_Enabled = 1		/*  Enabled  */
} TDA182I5a_LNA_Highpass_t, *pTDA182I5a_LNA_Highpass_t;

typedef enum _TDA182I5a_AGC2_Highpass_Item_t {
    TDA182I5a_AGC2_Highpass_766_MHz = 0,		/*  766 MHz  */
    TDA182I5a_AGC2_Highpass_600_MHz = 1,		/*  600 MHz  */
    TDA182I5a_AGC2_Highpass_470_MHz = 2,		/*  470 MHz  */
    TDA182I5a_AGC2_Highpass_180_MHz = 3,		/*  180 MHz  */
    TDA182I5a_AGC2_Highpass_20_MHz = 4		/*  20 MHz */
} TDA182I5a_AGC2_Highpass_t, *pTDA182I5a_AGC2_Highpass_t;

/* TDA182I5a Register RF_Filters_byte_1 0x26 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Filters_byte_1;
/* LNA_Highpass bit(s): enables / disables the LNA highpass filter */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Filters_byte_1__LNA_Highpass;
/* AGC2_Highpass bit(s): selects AGC2 highpass filter cut off frequency (MHz) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Filters_byte_1__AGC2_Highpass;


/* TDA182I5a Register RF_Low_Pass_Filter_byte_1 0x27 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_1;
/* RF_LPF_1 bit(s): selects harmonic filter RF_LPF coefficients */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_1__RF_LPF_1;


/* TDA182I5a Register RF_Low_Pass_Filter_byte_2 0x28 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_2;
/* RF_LPF_2 bit(s): selects harmonic filter RF_LPF coefficients */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_2__RF_LPF_2;
/* RF_LPF_f_1 bit(s): selects harmonic filter RF_LPF_f coefficients */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_2__RF_LPF_f_1;


/* TDA182I5a Register RF_Low_Pass_Filter_byte_3 0x29 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_3;
/* RF_LPF_f_2 bit(s): selects harmonic filter RF_LPF_f coefficients */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RF_Low_Pass_Filter_byte_3__RF_LPF_f_2;


typedef enum _TDA182I5a_AGC1_Top_Do_Read_Item_t {
    TDA182I5a_AGC1_Top_Do_Read_90_dBuVrms = 0,		/*  90 dBuVrms  */
    TDA182I5a_AGC1_Top_Do_Read_92_dBuVrms = 1,		/*  92 dBuVrms  */
    TDA182I5a_AGC1_Top_Do_Read_94_dBuVrms = 2,		/*  94 dBuVrms  */
    TDA182I5a_AGC1_Top_Do_Read_96_dBuVrms = 3,		/*  96 dBuVrms  */
    TDA182I5a_AGC1_Top_Do_Read_98_dBuVrms = 4,		/*  98 dBuVrms  */
    TDA182I5a_AGC1_Top_Do_Read_100_dBuVrms = 5,		/*  100 dBuVrms  */
    TDA182I5a_AGC1_Top_Do_Read_102_dBuVrms = 6,		/*  102 dBuVrms  */
    TDA182I5a_AGC1_Top_Do_Read_104_dBuVrms = 7,		/*  104 dBuVrms  */
    TDA182I5a_AGC1_Top_Do_Read_106_dBuVrms = 8,		/*  106 dBuVrms  */
    TDA182I5a_AGC1_Top_Do_Read_108_dBuVrms = 9		/*  108 dBuVrms */
} TDA182I5a_AGC1_Top_Do_Read_t, *pTDA182I5a_AGC1_Top_Do_Read_t;

typedef enum _TDA182I5a_AGC1_Top_Up_Read_Item_t {
    TDA182I5a_AGC1_Top_Up_Read_84_dBuVrms = 0,		/*  84 dBuVrms  */
    TDA182I5a_AGC1_Top_Up_Read_86_dBuVrms = 1,		/*  86 dBuVrms  */
    TDA182I5a_AGC1_Top_Up_Read_88_dBuVrms = 2,		/*  88 dBuVrms  */
    TDA182I5a_AGC1_Top_Up_Read_90_dBuVrms = 3,		/*  90 dBuVrms  */
    TDA182I5a_AGC1_Top_Up_Read_92_dBuVrms = 4,		/*  92 dBuVrms  */
    TDA182I5a_AGC1_Top_Up_Read_94_dBuVrms = 5,		/*  94 dBuVrms  */
    TDA182I5a_AGC1_Top_Up_Read_96_dBuVrms = 6,		/*  96 dBuVrms  */
    TDA182I5a_AGC1_Top_Up_Read_98_dBuVrms = 7,		/*  98 dBuVrms  */
    TDA182I5a_AGC1_Top_Up_Read_100_dBuVrms = 8,		/*  100 dBuVrms  */
    TDA182I5a_AGC1_Top_Up_Read_102_dBuVrms = 9		/*  102 dBuVrms */
} TDA182I5a_AGC1_Top_Up_Read_t, *pTDA182I5a_AGC1_Top_Up_Read_t;

/* TDA182I5a Register AGC1_Top_byte 0x2B */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_Top_byte;
/* AGC1_Top_Do_Read bit(s): reads current AGC1 TOP Down (dBuV) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_Top_byte__AGC1_Top_Do_Read;
/* AGC1_Top_Up_Read bit(s): reads current AGC1 TOP Up (dBuV) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_Top_byte__AGC1_Top_Up_Read;


typedef enum _TDA182I5a_AGC2_Top_Do_Read_Item_t {
    TDA182I5a_AGC2_Top_Do_Read_93_dBuVrms = 0,		/*  93 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_95_dBuVrms = 1,		/*  95 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_97_dBuVrms = 2,		/*  97 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_99_dBuVrms = 3,		/*  99 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_101_dBuVrms = 4,		/*  101 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_103_dBuVrms = 5,		/*  103 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_104_dBuVrms = 6,		/*  104 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_105_dBuVrms = 7,		/*  105 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_106_dBuVrms = 8,		/*  106 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_107_dBuVrms = 9,		/*  107 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_108_dBuVrms = 10,		/*  108 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_109_dBuVrms = 11,		/*  109 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_110_dBuVrms = 12,		/*  110 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_111_dBuVrms = 13,		/*  111 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_112_dBuVrms = 14,		/*  112 dBuVrms  */
    TDA182I5a_AGC2_Top_Do_Read_113_dBuVrms = 15		/*  113 dBuVrms */
} TDA182I5a_AGC2_Top_Do_Read_t, *pTDA182I5a_AGC2_Top_Do_Read_t;

/* TDA182I5a Register AGC2_Top_byte 0x2C */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_Top_byte;
/* AGC2_Top_Do_Read bit(s): reads current AGC2 TOP Down (dBuV) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_Top_byte__AGC2_Top_Do_Read;


typedef enum _TDA182I5a_LT_gain_read_Item_t {
    TDA182I5a_LT_gain_read_7_dB = 0,		/*  7 dB  */
    TDA182I5a_LT_gain_read_10_dB = 1,		/*  10 dB  */
    TDA182I5a_LT_gain_read_13_dB = 2,		/*  13 dB  */
    TDA182I5a_LT_gain_read_16_dB = 3,		/*  16 dB  */
    TDA182I5a_LT_gain_read_19_dB = 4,		/*  19 dB  */
    TDA182I5a_LT_gain_read_22_dB = 5,		/*  22 dB  */
    TDA182I5a_LT_gain_read_25_dB = 6,		/*  25 dB  */
    TDA182I5a_LT_gain_read_28_dB = 7		/*  28 dB */
} TDA182I5a_LT_gain_read_t, *pTDA182I5a_LT_gain_read_t;

typedef enum _TDA182I5a_AGC1_Gain_Read_Item_t {
    TDA182I5a_AGC1_Gain_Read_n5_dB = 0,		/*  -5 dB  */
    TDA182I5a_AGC1_Gain_Read_n2_dB = 1,		/*  -2 dB  */
    TDA182I5a_AGC1_Gain_Read_1_dB = 2,		/*  1 dB  */
    TDA182I5a_AGC1_Gain_Read_4_dB = 3,		/*  4 dB  */
    TDA182I5a_AGC1_Gain_Read_7_dB = 4,		/*  7 dB  */
    TDA182I5a_AGC1_Gain_Read_10_dB = 5,		/*  10 dB  */
    TDA182I5a_AGC1_Gain_Read_13_dB = 6,		/*  13 dB  */
    TDA182I5a_AGC1_Gain_Read_16_dB = 7,		/*  16 dB  */
    TDA182I5a_AGC1_Gain_Read_19_dB = 8,		/*  19 dB  */
    TDA182I5a_AGC1_Gain_Read_22_dB = 9,		/*  22 dB  */
    TDA182I5a_AGC1_Gain_Read_25_dB = 10,		/*  25 dB  */
    TDA182I5a_AGC1_Gain_Read_28_dB = 11		/*  28 dB */
} TDA182I5a_AGC1_Gain_Read_t, *pTDA182I5a_AGC1_Gain_Read_t;

/* TDA182I5a Register AGC1_Gain_byte 0x2D */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_Gain_byte;
/* LT_gain_read bit(s): reads current LT attenuation value (dB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_Gain_byte__LT_gain_read;
/* AGC1_Gain_Read bit(s): reads current AGC1 gain value (dB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC1_Gain_byte__AGC1_Gain_Read;


/* TDA182I5a Register AGC2_Gain_byte 0x2E */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_Gain_byte;
/* AGC2_Gain_Read bit(s): reads current AGC2 gain value (dB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_AGC2_Gain_byte__AGC2_Gain_Read;


typedef enum _TDA182I5a_AGC5_Gain_Read_Item_t {
    TDA182I5a_AGC5_Gain_Read_n6_or_6d4_dB = 0,		/*  -6  or 6.4  dB  */
    TDA182I5a_AGC5_Gain_Read_n3_or_9d4_dB = 1,		/*  -3 or 9.4  dB  */
    TDA182I5a_AGC5_Gain_Read_0_or_12d4_dB = 2,		/*  0  or 12.4  dB  */
    TDA182I5a_AGC5_Gain_Read_3_or_15d4_dB = 3,		/*  3  or 15.4  dB  */
    TDA182I5a_AGC5_Gain_Read_6_or_18d4_dB = 4,		/*  6  or 18.4  dB  */
    TDA182I5a_AGC5_Gain_Read_9_or_21d4_dB = 5,		/*  9  or 21.4  dB  */
    TDA182I5a_AGC5_Gain_Read_12_or_24d4_dB = 6,		/*  12  or 24.4  dB  */
    TDA182I5a_AGC5_Gain_Read_15_or_27d4_dB = 7		/*  15  or 27.4  dB */
} TDA182I5a_AGC5_Gain_Read_t, *pTDA182I5a_AGC5_Gain_Read_t;

typedef enum _TDA182I5a_AGC4_Gain_Read_Item_t {
    TDA182I5a_AGC4_Gain_Read_2 = 0,		/*  2   */
    TDA182I5a_AGC4_Gain_Read_5 = 1,		/*  5   */
    TDA182I5a_AGC4_Gain_Read_8 = 2,		/*  8   */
    TDA182I5a_AGC4_Gain_Read_11 = 3,		/*  11   */
    TDA182I5a_AGC4_Gain_Read_14 = 4,		/*  14   */
    TDA182I5a_AGC4_Gain_Read_val_5_NOT_USED = 5,		/*  val 5 NOT USED   */
    TDA182I5a_AGC4_Gain_Read_val_6_NOT_USED = 6,		/*  val 6 NOT USED   */
    TDA182I5a_AGC4_Gain_Read_val_7_NOT_USED = 7		/*  val 7 NOT USED  */
} TDA182I5a_AGC4_Gain_Read_t, *pTDA182I5a_AGC4_Gain_Read_t;

/* TDA182I5a Register IFAGCs_Gain_byte 0x2F */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IFAGCs_Gain_byte;
/* AGC5_Gain_Read bit(s): reads current AGC5 gain value (dB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IFAGCs_Gain_byte__AGC5_Gain_Read;
/* AGC4_Gain_Read bit(s): reads current AGC4 gain value (dB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IFAGCs_Gain_byte__AGC4_Gain_Read;


typedef enum _TDA182I5a_RSSI_AV_Item_t {
    TDA182I5a_RSSI_AV_Slow = 0,		/*  Slow   */
    TDA182I5a_RSSI_AV_Fast = 1		/*  Fast  */
} TDA182I5a_RSSI_AV_t, *pTDA182I5a_RSSI_AV_t;

typedef enum _TDA182I5a_Det6_Cap_Val_Item_t {
    TDA182I5a_Det6_Cap_Val_0d5_pF = 0,		/*  0.5 pF  */
    TDA182I5a_Det6_Cap_Val_3_pF = 1		/*  3 pF */
} TDA182I5a_Det6_Cap_Val_t, *pTDA182I5a_Det6_Cap_Val_t;

typedef enum _TDA182I5a_RSSI_Ck_Speed_Item_t {
    TDA182I5a_RSSI_Ck_Speed_250kHz_ATV_31_25kHz_DTV_KHz = 0,		/*  250kHz (ATV) / 31,25kHz (DTV) KHz  */
    TDA182I5a_RSSI_Ck_Speed_62d5kHz_ATV_250kHz_DTV_KHz = 1		/*  62.5kHz (ATV) / 250kHz (DTV) KHz */
} TDA182I5a_RSSI_Ck_Speed_t, *pTDA182I5a_RSSI_Ck_Speed_t;

/* TDA182I5a Register RSSI_byte_2 0x31 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RSSI_byte_2;
/* RSSI_AV bit(s): RSSI average mode */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RSSI_byte_2__RSSI_AV;
/* Det6_Cap_Val bit(s): selects Detector 6 integration capacitor value (pF) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RSSI_byte_2__Det6_Cap_Val;
/* RSSI_Ck_Speed bit(s): RSSI clock selection (kHz) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RSSI_byte_2__RSSI_Ck_Speed;


/* TDA182I5a Register Sublo_byte 0x34 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Sublo_byte;
/* SUBLO_prog bit(s): SubLO divider ratio */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Sublo_byte__SUBLO_prog;


typedef enum _TDA182I5a_LoChain_ratio_Item_t {
    TDA182I5a_LoChain_ratio_2 = 0,		/*  2   */
    TDA182I5a_LoChain_ratio_3 = 1,		/*  3   */
    TDA182I5a_LoChain_ratio_4 = 2,		/*  4   */
    TDA182I5a_LoChain_ratio_6 = 3,		/*  6   */
    TDA182I5a_LoChain_ratio_8 = 8,		/*  8   */
    TDA182I5a_LoChain_ratio_12 = 9,		/*  12   */
    TDA182I5a_LoChain_ratio_16 = 10,		/*  16   */
    TDA182I5a_LoChain_ratio_24 = 11,		/*  24   */
    TDA182I5a_LoChain_ratio_32 = 12		/*  32  */
} TDA182I5a_LoChain_ratio_t, *pTDA182I5a_LoChain_ratio_t;

/* TDA182I5a Register LoChain_byte 0x35 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_LoChain_byte;
/* PLL_Div2_Highfreq bit(s): PLL Div2 biasing */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_LoChain_byte__PLL_Div2_Highfreq;
/* LoChain_ratio bit(s): selects LO divider ratio */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_LoChain_byte__LoChain_ratio;


/* TDA182I5a Register Sigma_delta_byte_1 0x36 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_delta_byte_1;
/* LO_Int bit(s): selects sigma delta integer ratio */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_delta_byte_1__LO_Int;


/* TDA182I5a Register Sigma_delta_byte_2 0x37 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_delta_byte_2;
/* LO_Frac_2 bit(s): selects sigma delta fractional ratio (MSB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_delta_byte_2__LO_Frac_2;


/* TDA182I5a Register Sigma_delta_byte_3 0x38 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_delta_byte_3;
/* LO_Frac_1 bit(s): selects sigma delta fractional ratio (LSB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_delta_byte_3__LO_Frac_1;


/* TDA182I5a Register Sigma_prog_byte 0x39 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_prog_byte;
/* Update_prg bit(s): PLL divider ratios synchronisation */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Sigma_prog_byte__Update_prg;


typedef enum _TDA182I5a_cp_cursel_Item_t {
    TDA182I5a_cp_cursel_20_uA = 0,		/*  20 uA  */
    TDA182I5a_cp_cursel_150_uA = 1,		/*  150 uA  */
    TDA182I5a_cp_cursel_210_uA = 2,		/*  210 uA  */
    TDA182I5a_cp_cursel_300_uA = 3,		/*  300 uA  */
    TDA182I5a_cp_cursel_420_uA = 4,		/*  420 uA  */
    TDA182I5a_cp_cursel_580_uA = 5,		/*  580 uA  */
    TDA182I5a_cp_cursel_800_uA = 6,		/*  800 uA  */
    TDA182I5a_cp_cursel_1100_uA = 7		/*  1100 uA */
} TDA182I5a_cp_cursel_t, *pTDA182I5a_cp_cursel_t;

/* TDA182I5a Register Charge_pump_byte 0x3B */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Charge_pump_byte;
/* ICP_Force_Sink bit(s): enables / disables the manual control on sinking charge pump */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Charge_pump_byte__ICP_Force_Sink;
/* ICP_Force_Src bit(s): enables / disables the manual control on sourcing charge pump */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Charge_pump_byte__ICP_Force_Src;
/* cpleak0 bit(s): enables / disables charge pump current offset */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Charge_pump_byte__cpleak0;
/* cp_cursel bit(s): selects the charge pump current (uA) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Charge_pump_byte__cp_cursel;


/* TDA182I5a Register SigmaDelta_Test_byte 0x3C */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_SigmaDelta_Test_byte;
/* LOSD_Static_N bit(s): Sigma Delta modulator ON / OFF */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_SigmaDelta_Test_byte__LOSD_Static_N;


typedef enum _TDA182I5a_Dig_Reg_Item_t {
    TDA182I5a_Dig_Reg_1d6_V = 0,		/*  1.6 V  */
    TDA182I5a_Dig_Reg_1d8_V = 1,		/*  1.8 V  */
    TDA182I5a_Dig_Reg_1d8p_10mA_V = 2,		/*  1.8+ 10mA V  */
    TDA182I5a_Dig_Reg_2_V = 3		/*  2 V */
} TDA182I5a_Dig_Reg_t, *pTDA182I5a_Dig_Reg_t;

typedef enum _TDA182I5a_VCO_Reg_Item_t {
    TDA182I5a_VCO_Reg_2d6_V = 0,		/*  2.6 V  */
    TDA182I5a_VCO_Reg_2d8_nom_V = 1,		/*  2.8  nom V  */
    TDA182I5a_VCO_Reg_3_V = 2,		/*  3 V  */
    TDA182I5a_VCO_Reg_val_3_Forbidden_V = 3		/*  val 3 Forbidden V */
} TDA182I5a_VCO_Reg_t, *pTDA182I5a_VCO_Reg_t;

/* TDA182I5a Register Regulators_byte 0x3E */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Regulators_byte;
/* Dig_Reg bit(s): selects the digital regulator output voltage */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Regulators_byte__Dig_Reg;
/* VCO_Reg bit(s): selects VCO regulator voltage */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Regulators_byte__VCO_Reg;


typedef enum _TDA182I5a_VCO_Select_Item_t {
    TDA182I5a_VCO_Select_VCO_High = 0,		/*  VCO_High   */
    TDA182I5a_VCO_Select_VCO_Medium = 1,		/*  VCO_Medium   */
    TDA182I5a_VCO_Select_VCO_Low = 2,		/*  VCO_Low   */
    TDA182I5a_VCO_Select_val_3_Forbidden = 3		/*  val 3 Forbidden  */
} TDA182I5a_VCO_Select_t, *pTDA182I5a_VCO_Select_t;

/* TDA182I5a Register VCO_byte 0x3F */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_byte;
/* VCO_Select bit(s): selects VCO frequency band */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_byte__VCO_Select;


typedef enum _TDA182I5a_VCO_currentctrl_Item_t {
    TDA182I5a_VCO_currentctrl_0 = 0,		/*  0   */
    TDA182I5a_VCO_currentctrl_1 = 1,		/*  1   */
    TDA182I5a_VCO_currentctrl_2 = 2,		/*  2   */
    TDA182I5a_VCO_currentctrl_3 = 3,		/*  3   */
    TDA182I5a_VCO_currentctrl_4 = 4,		/*  4   */
    TDA182I5a_VCO_currentctrl_5 = 5,		/*  5   */
    TDA182I5a_VCO_currentctrl_6 = 6,		/*  6   */
    TDA182I5a_VCO_currentctrl_7 = 7,		/*  7   */
    TDA182I5a_VCO_currentctrl_8 = 8,		/*  8   */
    TDA182I5a_VCO_currentctrl_9 = 9,		/*  9   */
    TDA182I5a_VCO_currentctrl_10 = 10,		/*  10   */
    TDA182I5a_VCO_currentctrl_11 = 11,		/*  11   */
    TDA182I5a_VCO_currentctrl_12 = 12,		/*  12   */
    TDA182I5a_VCO_currentctrl_13 = 13,		/*  13   */
    TDA182I5a_VCO_currentctrl_14 = 14,		/*  14   */
    TDA182I5a_VCO_currentctrl_15 = 15		/*  15  */
} TDA182I5a_VCO_currentctrl_t, *pTDA182I5a_VCO_currentctrl_t;

/* TDA182I5a Register VCO_cal_byte_1 0x40 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_cal_byte_1;
/* VCOampl_force_cur bit(s): Forces a manual VCO amplitude */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_cal_byte_1__VCOampl_force_cur;
/* VCO_currentctrl bit(s): Reads the index returned by the VCO amplitude calibration or set the index value in case of a manual control */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_cal_byte_1__VCO_currentctrl;


typedef enum _TDA182I5a_VCO_swingctrl_Item_t {
    TDA182I5a_VCO_swingctrl_680_mV = 0,		/*  680 mV  */
    TDA182I5a_VCO_swingctrl_640_mV = 1,		/*  640 mV  */
    TDA182I5a_VCO_swingctrl_600_mV = 2,		/*  600 mV  */
    TDA182I5a_VCO_swingctrl_560_mV = 3,		/*  560 mV  */
    TDA182I5a_VCO_swingctrl_520_mV = 4,		/*  520 mV  */
    TDA182I5a_VCO_swingctrl_480_mV = 5,		/*  480 mV  */
    TDA182I5a_VCO_swingctrl_460_mV = 6,		/*  460 mV  */
    TDA182I5a_VCO_swingctrl_420_mV = 7,		/*  420 mV  */
    TDA182I5a_VCO_swingctrl_400_mV = 8,		/*  400 mV  */
    TDA182I5a_VCO_swingctrl_380_mV = 9,		/*  380 mV  */
    TDA182I5a_VCO_swingctrl_360_mV = 10,		/*  360 mV  */
    TDA182I5a_VCO_swingctrl_320_mV = 11,		/*  320 mV  */
    TDA182I5a_VCO_swingctrl_280_mV = 12,		/*  280 mV  */
    TDA182I5a_VCO_swingctrl_240_mV = 13,		/*  240 mV  */
    TDA182I5a_VCO_swingctrl_200_mV = 14,		/*  200 mV  */
    TDA182I5a_VCO_swingctrl_160_mV = 15		/*  160 mV */
} TDA182I5a_VCO_swingctrl_t, *pTDA182I5a_VCO_swingctrl_t;

/* TDA182I5a Register VCO_cal_byte_2 0x41 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_cal_byte_2;
/* VCO_swingctrl bit(s): VCO amplitude calibration target */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_VCO_cal_byte_2__VCO_swingctrl;


typedef enum _TDA182I5a_adc_vtune_Item_t {
    TDA182I5a_adc_vtune_0_to_0d33_V = 0,		/*  0 to 0.33 V  */
    TDA182I5a_adc_vtune_0d33_to_0d45_V = 1,		/*  0.33 to 0.45 V  */
    TDA182I5a_adc_vtune_0d45_to_0d58_V = 2,		/*  0.45 to 0.58 V  */
    TDA182I5a_adc_vtune_0d58_to_1d05_V = 3,		/*  0.58 to 1.05 V  */
    TDA182I5a_adc_vtune_1d05_to_1d62_V = 4,		/*  1.05 to 1.62 V  */
    TDA182I5a_adc_vtune_1d62_to_1d84_V = 5,		/*  1.62 to 1.84 V  */
    TDA182I5a_adc_vtune_1d84_to_2d1_V = 6,		/*  1.84 to 2.1 V  */
    TDA182I5a_adc_vtune_2d1_upper_V = 7		/*  2.1 upper V */
} TDA182I5a_adc_vtune_t, *pTDA182I5a_adc_vtune_t;

/* TDA182I5a Register vco_log 0x42 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_vco_log;
/* RF_udld_read bit(s): reads current status of RF_udld detector */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_vco_log__RF_udld_read;
/* IF_ovld_read bit(s): reads current status of IF_ovld detector */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_vco_log__IF_ovld_read;
/* RF_ovld_read bit(s): reads current status of RF_ovld detector */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_vco_log__RF_ovld_read;
/* IF_udld_read bit(s): reads current status of IF_udld detector */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_vco_log__IF_udld_read;
/* adc_vtune bit(s): vtune voltage measurement value (V) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_vco_log__adc_vtune;


/* TDA182I5a Register FVCO_MAX_BYTE_1 0x43 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_FVCO_MAX_BYTE_1;
/* FVCO_max_1 bit(s): Current VCO frequency calibration result (MSB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_FVCO_MAX_BYTE_1__FVCO_max_1;


/* TDA182I5a Register FVCO_MAX_BYTE_2 0x44 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_FVCO_MAX_BYTE_2;
/* FVCO_max_2 bit(s): Current VCO frequency calibration result (medium byte) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_FVCO_MAX_BYTE_2__FVCO_max_2;


/* TDA182I5a Register FVCO_MAX_BYTE_3 0x45 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_FVCO_MAX_BYTE_3;
/* FVCO_max_3 bit(s): Current VCO frequency calibration result (LSB) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_FVCO_MAX_BYTE_3__FVCO_max_3;


typedef enum _TDA182I5a_RC_Cal_Offset_Item_t {
    TDA182I5a_RC_Cal_Offset_0 = 0,		/*  0   */
    TDA182I5a_RC_Cal_Offset_1 = 1,		/*  1   */
    TDA182I5a_RC_Cal_Offset_2 = 2,		/*  2   */
    TDA182I5a_RC_Cal_Offset_3 = 3		/*  3  */
} TDA182I5a_RC_Cal_Offset_t, *pTDA182I5a_RC_Cal_Offset_t;

/* TDA182I5a Register RC_Cal_byte_1 0x46 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RC_Cal_byte_1;
/* RC_Cal_Offset bit(s): Index offset applied to the RC calibration */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RC_Cal_byte_1__RC_Cal_Offset;


/* TDA182I5a Register RC_Cal_byte_2 0x47 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RC_Cal_byte_2;
/* RCcal bit(s): RC calibration index result (including RC_Cal_Offset) */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_RC_Cal_byte_2__RCcal;


typedef enum _TDA182I5a_IR_ROM_offset_Item_t {
    TDA182I5a_IR_ROM_offset_0 = 0,		/*  0   */
    TDA182I5a_IR_ROM_offset_1 = 1,		/*  1   */
    TDA182I5a_IR_ROM_offset_2 = 2,		/*  2   */
    TDA182I5a_IR_ROM_offset_3 = 3		/*  3  */
} TDA182I5a_IR_ROM_offset_t, *pTDA182I5a_IR_ROM_offset_t;

typedef enum _TDA182I5a_IR_FreqPtr_Item_t {
    TDA182I5a_IR_FreqPtr_NOT_USED = 0,		/*  NOT USED   */
    TDA182I5a_IR_FreqPtr_Low = 1,		/*  Low   */
    TDA182I5a_IR_FreqPtr_Mid = 2,		/*  Mid   */
    TDA182I5a_IR_FreqPtr_high = 3		/*  high  */
} TDA182I5a_IR_FreqPtr_t, *pTDA182I5a_IR_FreqPtr_t;

/* TDA182I5a Register IR_Cal_byte_3 0x48 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Cal_byte_3;
/* IR_ROM_offset bit(s): Image Rejection calibration factor */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Cal_byte_3__IR_ROM_offset;
/* IR_FreqPtr bit(s): Frequency pointer to Image Rejection calibration results */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Cal_byte_3__IR_FreqPtr;


typedef enum _TDA182I5a_ircal_boost_Item_t {
    TDA182I5a_ircal_boost_0 = 0,		/*  0   */
    TDA182I5a_ircal_boost_1 = 1,		/*  1   */
    TDA182I5a_ircal_boost_2 = 2,		/*  2   */
    TDA182I5a_ircal_boost_3 = 3		/*  3  */
} TDA182I5a_ircal_boost_t, *pTDA182I5a_ircal_boost_t;

/* TDA182I5a Register IR_Cal_byte_4 0x49 */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Cal_byte_4;
/* ircal_boost bit(s): Image Rejection calibration generator amplitude index */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_IR_Cal_byte_4__ircal_boost;


/* TDA182I5a Register Power_Down_byte_1 0x4B */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_Down_byte_1;
/* PD_adc_vtune bit(s): enables / disables the ADC connected on vtune */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_Down_byte_1__PD_adc_vtune;
/* PD_SUBLO bit(s): enables / disables the subLO */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_Down_byte_1__PD_SUBLO;
/* PD_LT bit(s): enables / disables the Loopthrough buffer */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Power_Down_byte_1__PD_LT;


/* TDA182I5a Register Powerdown 0x4D */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Powerdown;
/* force_soft_reset bit(s): enables / disables the software reset */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Powerdown__force_soft_reset;
/* Soft_reset bit(s): Requests a software reset */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Powerdown__Soft_reset;


/* TDA182I5a Register Test_byte 0x4E */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Test_byte;
/* agck_step_sync bit(s): enables / disables the first AGC_K 1st step duration correction */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Test_byte__agck_step_sync;
/* reset_i2c_dis bit(s): Defines if I2C core is reset or not when performing a Soft_reset operation */
extern const TDA182I5a_BitField_t gTDA182I5a_Reg_Test_byte__reset_i2c_dis;



#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA182I5a_REGDEF_H */
