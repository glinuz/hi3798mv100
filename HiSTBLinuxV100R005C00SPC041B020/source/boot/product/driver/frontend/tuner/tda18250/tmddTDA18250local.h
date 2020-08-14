/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmddTDA18250local.h
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

#ifndef _TMDD_TDA18250_LOCAL_H
#define _TMDD_TDA18250_LOCAL_H


#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

#define TMDD_TDA18250_MAX_UNITS                 (4)

#define TMDD_TDA18250_COMP_NUM                  0  /* Major protocol change - Specification change required */
#define TMDD_TDA18250_MAJOR_VER                 3  /* Minor protocol change - Specification change required */
#define TMDD_TDA18250_MINOR_VER                 33 /* Software update - No protocol change - No specification change required */

#define ddTDA18250_MUTEX_TIMEOUT                (4000)

#define POBJ_SRVFUNC_SIO                        pObj->sRWFunc
#define POBJ_SRVFUNC_STIME                      pObj->sTime

#define P_DBGPRINTVALID                         ((pObj != Null) && (pObj->sDebug.Print != Null))
#define P_DBGPRINTEx                            pObj->sDebug.Print


#define TDA18250_POWER_LEVEL_MIN                (44)
#define TDA18250_POWER_LEVEL_MAX                (100)

#define TDA18250_I2C_MAP_NB_BYTES               (69)

#define TDA18250_MAIN_PLL_MAP_ID                0
#define TDA18250_MAIN_PLL_MAP_NB_COLUMNS        3
#define TDA18250_MAIN_PLL_MAP_NB_ROWS           40

/*=========*/
/*  Index  */
/*=========*/
#define INT_CHAR  1
#define TMDD_TDA18250_0x00_ID_byte             (0x00*INT_CHAR)
#define TMDD_TDA18250_0x01_Read_byte_1         (0x01*INT_CHAR)
#define TMDD_TDA18250_0x02_Read_byte_2         (0x02*INT_CHAR)
#define TMDD_TDA18250_0x03_Read_byte_3         (0x03*INT_CHAR)
#define TMDD_TDA18250_0x04_Read_byte_4         (0x04*INT_CHAR)
#define TMDD_TDA18250_0x05_Read_byte_5         (0x05*INT_CHAR)
#define TMDD_TDA18250_0x06_Read_byte_6         (0x06*INT_CHAR)
#define TMDD_TDA18250_0x07_PSM_byte_1          (0x07*INT_CHAR)
#define TMDD_TDA18250_0x08_Main_Divider_byte_1 (0x08*INT_CHAR)
#define TMDD_TDA18250_0x09_Main_Divider_byte_2 (0x09*INT_CHAR)
#define TMDD_TDA18250_0x0A_Main_Divider_byte_3 (0x0A*INT_CHAR)
#define TMDD_TDA18250_0x0B_Main_Divider_byte_4 (0x0B*INT_CHAR)
#define TMDD_TDA18250_0x0C_Main_Divider_byte_5 (0x0C*INT_CHAR)
#define TMDD_TDA18250_0x0D_Main_Divider_byte_6 (0x0D*INT_CHAR)
#define TMDD_TDA18250_0x0E_Main_Divider_byte_7 (0x0E*INT_CHAR)
#define TMDD_TDA18250_0x0F_Main_Divider_byte_8 (0x0F*INT_CHAR)
#define TMDD_TDA18250_0x10_Cal_Divider_byte_1  (0x10*INT_CHAR)
#define TMDD_TDA18250_0x11_Cal_Divider_byte_2  (0x11*INT_CHAR)
#define TMDD_TDA18250_0x12_Cal_Divider_byte_3  (0x12*INT_CHAR)
#define TMDD_TDA18250_0x13_Cal_Divider_byte_4  (0x13*INT_CHAR)
#define TMDD_TDA18250_0x14_Cal_Divider_byte_5  (0x14*INT_CHAR)
#define TMDD_TDA18250_0x15_Cal_Divider_byte_6  (0x15*INT_CHAR)
#define TMDD_TDA18250_0x16_Cal_Divider_byte_7  (0x16*INT_CHAR)
#define TMDD_TDA18250_0x17_Power_Down_byte_1   (0x17*INT_CHAR)
#define TMDD_TDA18250_0x18_Power_Down_byte_2   (0x18*INT_CHAR)
#define TMDD_TDA18250_0x19_Xtout_Byte          (0x19*INT_CHAR)
#define TMDD_TDA18250_0x1A_IF_Byte_1           (0x1A*INT_CHAR)
#define TMDD_TDA18250_0x1B_IF_Byte_2           (0x1B*INT_CHAR)
#define TMDD_TDA18250_0x1C_AGC2b_byte          (0x1C*INT_CHAR)
#define TMDD_TDA18250_0x1D_PSM_byte_2          (0x1D*INT_CHAR)
#define TMDD_TDA18250_0x1E_PSM_byte_3          (0x1E*INT_CHAR)
#define TMDD_TDA18250_0x1F_PSM_byte_4          (0x1F*INT_CHAR)
#define TMDD_TDA18250_0x20_AGC1_byte_1         (0x20*INT_CHAR)
#define TMDD_TDA18250_0x21_AGC1_byte_2         (0x21*INT_CHAR)
#define TMDD_TDA18250_0x22_AGC1_byte_3         (0x22*INT_CHAR)
#define TMDD_TDA18250_0x23_AGC2_byte_1         (0x23*INT_CHAR)
#define TMDD_TDA18250_0x24_AGC2_byte_2         (0x24*INT_CHAR)
#define TMDD_TDA18250_0x25_Analog_AGC_byte     (0x25*INT_CHAR)
#define TMDD_TDA18250_0x26_RC_byte             (0x26*INT_CHAR)
#define TMDD_TDA18250_0x27_RSSI_byte           (0x27*INT_CHAR)
#define TMDD_TDA18250_0x28_IR_CAL_byte_1       (0x28*INT_CHAR)
#define TMDD_TDA18250_0x29_IR_CAL_byte_2       (0x29*INT_CHAR)
#define TMDD_TDA18250_0x2A_IR_CAL_byte_3       (0x2A*INT_CHAR)
#define TMDD_TDA18250_0x2B_IR_CAL_byte_4       (0x2B*INT_CHAR)
#define TMDD_TDA18250_0x2C_RF_Cal_byte_1       (0x2C*INT_CHAR)
#define TMDD_TDA18250_0x2D_RF_Cal_byte_2       (0x2D*INT_CHAR)
#define TMDD_TDA18250_0x2E_RF_Cal_byte_3       (0x2E*INT_CHAR)
#define TMDD_TDA18250_0x2F_RF_Cal_byte_4       (0x2F*INT_CHAR)
#define TMDD_TDA18250_0x30_RF_Cal_byte_5       (0x30*INT_CHAR)
#define TMDD_TDA18250_0x31_RF_Cal_byte_6       (0x31*INT_CHAR)
#define TMDD_TDA18250_0x32_RF_Cal_byte_7       (0x32*INT_CHAR)
#define TMDD_TDA18250_0x33_RF_Cal_byte_8       (0x33*INT_CHAR)
#define TMDD_TDA18250_0x34_RF_Cal_byte_9       (0x34*INT_CHAR)
#define TMDD_TDA18250_0x35_RF_Cal_byte_10      (0x35*INT_CHAR)
#define TMDD_TDA18250_0x36_RF_Cal_RAM_byte_1   (0x36*INT_CHAR)
#define TMDD_TDA18250_0x37_RF_Cal_RAM_byte_2   (0x37*INT_CHAR)
#define TMDD_TDA18250_0x38_DEBUG_byte          (0x38*INT_CHAR)
#define TMDD_TDA18250_0x39_Fmax_byte_1         (0x39*INT_CHAR)
#define TMDD_TDA18250_0x3A_Fmax_byte_2         (0x3A*INT_CHAR)
#define TMDD_TDA18250_0x3B_RCCAL_delay         (0x3B*INT_CHAR)
#define TMDD_TDA18250_0x3C_AGC1_smooth         (0x3C*INT_CHAR)
#define TMDD_TDA18250_0x3D_LT_smooth           (0x3D*INT_CHAR)
#define TMDD_TDA18250_0x3E_RF_MUX_test         (0x3E*INT_CHAR)
#define TMDD_TDA18250_0x3F_DTA_Filter          (0x3F*INT_CHAR)
#define TMDD_TDA18250_0x40_AGC1bis             (0x40*INT_CHAR)
#define TMDD_TDA18250_0x41_IF_byte_1           (0x41*INT_CHAR)
#define TMDD_TDA18250_0x42_IF_byte_2           (0x42*INT_CHAR)
#define TMDD_TDA18250_0x43_RFSW_SMOOTH         (0x43*INT_CHAR)
#define TMDD_TDA18250_0x44_RFSW_NOTCH          (0x44*INT_CHAR)

#ifdef _TMDDTDA18250_MSB_FIRST
#warning "MSB FISRT SELECTED FOR BIT FIELD"
#endif


typedef struct _tmddTDA18250_PLL_Map_t
{
    UInt32 uLO_Max;
    UInt8  uPost_Div;
    UInt8  uDiv;
} tmddTDA18250_PLL_Map_t, *ptmddTDA18250_PLL_Map_t;


typedef struct _tmddTDA18250Config_t
{
    tmddTDA18250_PLL_Map_t   MAIN_PLL_Map[TDA18250_MAIN_PLL_MAP_NB_ROWS];
} tmddTDA18250Config_t, *ptmddTDA18250Config_t;


typedef struct _TDA18250_I2C_Map_t
{                            
    union                        
    {                        
        UInt8 ID_byte;                    /* address  0x00 */
        UInt8 UNUSED_0x00;
    } __attribute__((packed))uBx00;                        

    union                        
    {                        
        UInt8 Read_byte_1;                /* address 0x01 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x01_1  :1;
            UInt8 LO_Lock        :1;
            UInt8 UNUSED_0x01_0  :1;
            UInt8 RSSI_Alarm     :1;
            UInt8 TM_D           :4;
#else
            UInt8 TM_D           :4;
            UInt8 RSSI_Alarm     :1;
            UInt8 UNUSED_0x01_0  :1;
            UInt8 LO_Lock        :1;
            UInt8 UNUSED_0x01_1  :1;
#endif            
        } __attribute__((packed))bF;        
    } __attribute__((packed))uBx01;                    

    union                    
    {                    
        UInt8 Read_byte_2;                /* address 0x02 */
        UInt8 RSSI;
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 RSSI_7        :1;
            UInt8 RSSI_6        :1;
            UInt8 RSSI_5        :1;
            UInt8 RSSI_4        :1;
            UInt8 RSSI_3        :1;
            UInt8 RSSI_2        :1;
            UInt8 RSSI_1        :1;
            UInt8 RSSI_0        :1;
#else
            UInt8 RSSI_0        :1;
            UInt8 RSSI_1        :1;
            UInt8 RSSI_2        :1;
            UInt8 RSSI_3        :1;
            UInt8 RSSI_4        :1;
            UInt8 RSSI_5        :1;
            UInt8 RSSI_6        :1;
            UInt8 RSSI_7        :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx02;            

    union            
    {            
        UInt8 Read_byte_3;                /* address 0x03 */
        UInt8 AGC2;
    } __attribute__((packed))uBx03;            

    union                                
    {                                
        UInt8 Read_byte_4;                /* address 0x04 */
        struct                       
        {                
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 LT            :4;
            UInt8 AGC1          :4;
#else
            UInt8 AGC1          :4;
            UInt8 LT            :4;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx04;                                

    union                                
    {        
        UInt8 Read_byte_5;                /* address 0x05 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 msm_rst                   :1;
            UInt8 FlagClear                 :1;
            UInt8 launch_msm                :1;
            UInt8 FlagMSM_OK                :1;
            UInt8 RSSI_Sense_OK             :1;
            UInt8 UNUSED_0x05_0             :3;
#else
            UInt8 UNUSED_0x05_0             :3;
            UInt8 RSSI_Sense_OK             :1;
            UInt8 FlagMSM_OK                :1;
            UInt8 launch_msm                :1;
            UInt8 FlagClear                 :1;
            UInt8 msm_rst                   :1;
#endif
        } __attribute__((packed))bF;    
    } __attribute__((packed))uBx05;        

    union        
    {                    
        UInt8 Read_byte_6;                /* address 0x06 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x06               :2;
            UInt8 DataRAM                   :6;
#else
            UInt8 DataRAM                   :6;
            UInt8 UNUSED_0x06               :2;
#endif
        } __attribute__((packed))bF;        
    } __attribute__((packed))uBx06;                    

    union                    
    {                    
        UInt8 PSM_byte_1;                /* address 0x07 */
        struct                       
        {                
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x07               :1;
            UInt8 Freq_prog_En              :1;
            UInt8 PSM_LOVCO                 :2;
            UInt8 PSM_Mixer                 :2;
            UInt8 PSM_Lodriver              :2;
#else
            UInt8 PSM_Lodriver              :2;
            UInt8 PSM_Mixer                 :2;
            UInt8 PSM_LOVCO                 :2;
            UInt8 Freq_prog_En              :1;
            UInt8 UNUSED_0x07               :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx07;            

    union            
    {            
        UInt8 Main_Divider_byte_1;        /* address 0x08 */
        UInt8 UNUSED_0x08;
    } __attribute__((packed))uBx08;            

    union            
    {            
        UInt8 Main_Divider_byte_2;        /* address 0x09 */
        struct               
        {        
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x09               :1;
            UInt8 LO_Int                    :7;
#else
            UInt8 LO_Int                    :7;
            UInt8 UNUSED_0x09               :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx09;            

    union            
    {            
        UInt8 Main_Divider_byte_3;        /* address 0x0A */
        UInt8 LO_Frac_31_to_24;
    } __attribute__((packed))uBx0A;            

    union            
    {            
        UInt8 Main_Divider_byte_4;        /* address 0x0B */
        UInt8 LO_Frac_23_to_16;
    } __attribute__((packed))uBx0B;            

    union            
    {            
        UInt8 Main_Divider_byte_5;        /* address 0x0C */
        UInt8 LO_Frac_15_to_8;
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 LO_Frac_15_to_12          :4;
            UInt8 LO_Frac_11_to_8           :4;
#else
            UInt8 LO_Frac_11_to_8           :4;
            UInt8 LO_Frac_15_to_12          :4;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx0C;            

    union            
    {            
        UInt8 Main_Divider_byte_6;        /* address 0x0D */
        UInt8 LO_Frac_7_to_0;
    } __attribute__((packed))uBx0D;            

    union            
    {            
        UInt8 Main_Divider_byte_7;        /* address 0x0E */
        UInt8 UNUSED_0x0E;
    } __attribute__((packed))uBx0E;            

    union            
    {            
        UInt8 Main_Divider_byte_8;        /* address 0x0F */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x0F_1             :1;
            UInt8 Freq_prog_Start           :1;
            UInt8 UNUSED_0x0F_0             :6;
#else
            UInt8 UNUSED_0x0F_0             :6;
            UInt8 Freq_prog_Start           :1;
            UInt8 UNUSED_0x0F_1             :1;
#endif
        } __attribute__((packed))bF;                    
    } __attribute__((packed))uBx0F;                            

    union                            
    {                            
        UInt8 Cal_Divider_byte_1;        /* address 0x10 */
        UInt8 UNUSED_0x10;
    } __attribute__((packed))uBx10;            

    union            
    {            
        Int8 Cal_Divider_byte_2;        /* address 0x11 */
        Int8 CAL_Int;
    } __attribute__((packed))uBx11;            

    union            
    {            
        UInt8 Cal_Divider_byte_3;        /* address 0x12 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x12               :1;
            UInt8 CAL_Frac_22_to_16         :7;
#else
            UInt8 CAL_Frac_22_to_16         :7;
            UInt8 UNUSED_0x12               :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx12;            

    union            
    {            
        UInt8 Cal_Divider_byte_4;        /* address 0x13 */
        UInt8 CAL_Frac_15_to_8;
    } __attribute__((packed))uBx13;            

    union            
    {            
        UInt8 Cal_Divider_byte_5;        /* address 0x14 */
        UInt8 CAL_Frac_7_to_0;
    } __attribute__((packed))uBx14;            

    union            
    {            
        UInt8 Cal_Divider_byte_6;        /* address 0x15 */
        UInt8 UNUSED_0x15;
    } __attribute__((packed))uBx15;            

    union            
    {            
        UInt8 Cal_Divider_byte_7;        /* address 0x16 */
        UInt8 UNUSED_0x16;
    } __attribute__((packed))uBx16;            

    union            
    {            
        UInt8 Power_Down_byte_1;        /* address 0x17 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x17_2             :1;
            UInt8 pdLT                      :1;
            UInt8 UNUSED_0x17_1             :2;
            UInt8 pdAGC1b                   :1;
            UInt8 PD_RFAGC_Ifout            :1;
            UInt8 PD_LO_Synthe              :1;
            UInt8 SM                        :1;
#else
            UInt8 SM                        :1;
            UInt8 PD_LO_Synthe              :1;
            UInt8 PD_RFAGC_Ifout            :1;
            UInt8 pdAGC1b                   :1;
            UInt8 UNUSED_0x17_1             :2;
            UInt8 pdLT                      :1;
            UInt8 UNUSED_0x17_2             :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx17;            

    union            
    {            
        UInt8 Power_Down_byte_2;        /* address 0x18 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 PDBUF1b                   :1;
            UInt8 RFSW_MTO_LT_RFin          :1;
            UInt8 UNUSED_0x18_1             :2;
            UInt8 PD_RFAGC_Det              :1;
            UInt8 pdDETECT1                 :1;
            UInt8 pdAGC2b                   :1;
            UInt8 UNUSED_0x18_0             :1;
#else
            UInt8 UNUSED_0x18_0             :1;
            UInt8 pdAGC2b                   :1;
            UInt8 pdDETECT1                 :1;
            UInt8 PD_RFAGC_Det              :1;
            UInt8 UNUSED_0x18_1             :2;
            UInt8 RFSW_MTO_LT_RFin          :1;
            UInt8 PDBUF1b                   :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx18;            

    union            
    {            
        UInt8 Xtout_Byte;                /* address 0x19 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 hc_mode                   :4;
            UInt8 XtOut                     :4;
#else
            UInt8 XtOut                     :4;
            UInt8 hc_mode                   :4;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx19;            

    union            
    {            
        UInt8 IF_Byte_1;                    /* address 0x1A */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x1A_1             :1;
            UInt8 IF_level                  :3;
            UInt8 UNUSED_0x1A_0             :1;
            UInt8 BP_Filter                 :3;
#else
            UInt8 BP_Filter                 :3;
            UInt8 UNUSED_0x1A_0             :1;
            UInt8 IF_level                  :3;
            UInt8 UNUSED_0x1A_1             :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx1A;            

    union            
    {            
        UInt8 IF_Byte_2;                    /* address 0x1B */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 RFAGC_OpenLoop            :1;
            UInt8 UNUSED_0x1B_1             :1;
            UInt8 pdAGC2a                   :1;
            UInt8 AGC_Low_BW                :1;
            UInt8 AGC2_Up_step              :2;
            UInt8 LP_Fc                     :2;
#else
            UInt8 LP_Fc                     :2;
            UInt8 AGC2_Up_step              :2;
            UInt8 AGC_Low_BW                :1;
            UInt8 pdAGC2a                   :1;
            UInt8 UNUSED_0x1B_1             :1;
            UInt8 RFAGC_OpenLoop            :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx1B;            

    union            
    {            
        UInt8 AGC2b_byte;                /* address 0x1C */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 pulse_up_auto             :1;
            UInt8 UNUSED_0x1C_1             :2;
            UInt8 AGC_On                    :1;
            UInt8 UNUSED_0x1C_0             :1;
            UInt8 pdAGC2_DETECT4            :1;
            UInt8 pdAGC2_DETECT3            :1;
            UInt8 pdAGC2_DETECT2            :1;
#else
            UInt8 pdAGC2_DETECT2            :1;
            UInt8 pdAGC2_DETECT3            :1;
            UInt8 pdAGC2_DETECT4            :1;
            UInt8 UNUSED_0x1C_0             :1;
            UInt8 AGC_On                    :1;
            UInt8 UNUSED_0x1C_1             :2;
            UInt8 pulse_up_auto             :1;
#endif
        } __attribute__((packed))bF;                
    } __attribute__((packed))uBx1C;                            

    union                            
    {                            
        UInt8 PSM_byte_2;                /* address 0x1D */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x1D_1             :1;
            UInt8 TM_ON                     :1;
            UInt8 UNUSED_0x1D_0             :6;
#else
            UInt8 UNUSED_0x1D_0             :6;
            UInt8 TM_ON                     :1;
            UInt8 UNUSED_0x1D_1             :1;
#endif
        } __attribute__((packed))bF;                
    } __attribute__((packed))uBx1D;                            

    union                            
    {                            
        UInt8 PSM_byte_3;                /* address 0x1E */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x1E_1             :2;
            UInt8 STOB_DC                   :2;
            UInt8 PSM_StoB                  :2;
            UInt8 UNUSED_0x1E_0             :2;
#else
            UInt8 UNUSED_0x1E_0             :2;
            UInt8 PSM_StoB                  :2;
            UInt8 STOB_DC                   :2;
            UInt8 UNUSED_0x1E_1             :2;
#endif
        } __attribute__((packed))bF;                
    } __attribute__((packed))uBx1E;                            

    union                            
    {                            
        UInt8 PSM_byte_4;                /* address 0x1F */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x1F               :3;
            UInt8 AGC1_aud_sel              :1;
            UInt8 AGC1_au_ptr               :4;
#else
            UInt8 AGC1_au_ptr               :4;
            UInt8 AGC1_aud_sel              :1;
            UInt8 UNUSED_0x1F               :3;
#endif
        } __attribute__((packed))bF;            
    } __attribute__((packed))uBx1F;                        

    union                        
    {                        
        UInt8 AGC1_byte_1;                /* address 0x20 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 AGC2_Gud_ptr_sel          :2;
            UInt8 AGC2_Gup_sel              :1;
            UInt8 AGC1_Gup_sel              :1;
            UInt8 Manual_LT                 :1;
            UInt8 AGC1_aud                  :3;
#else
            UInt8 AGC1_aud                  :3;
            UInt8 Manual_LT                 :1;
            UInt8 AGC1_Gup_sel              :1;
            UInt8 AGC2_Gup_sel              :1;
            UInt8 AGC2_Gud_ptr_sel          :2;
#endif
        } __attribute__((packed))bF;                
    } __attribute__((packed))uBx20;                            

    union                            
    {                            
        UInt8 AGC1_byte_2;                /* address 0x21 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 AGC2_Speed                :2;
            UInt8 RFagc_fast                :1;
            UInt8 AGC1_Gud                  :5;
#else
            UInt8 AGC1_Gud                  :5;
            UInt8 RFagc_fast                :1;
            UInt8 AGC2_Speed                :2;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx21;            

    union            
    {            
        UInt8 AGC1_byte_3;                /* address 0x22 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x22_1             :1;
            UInt8 AGC2_Gud_valid            :1;
            UInt8 AGC1_loop_off             :1;
            UInt8 AGC1_bypass               :1;
            UInt8 UNUSED_0x22_0             :2;
            UInt8 AGC1_Do_step              :2;
#else
            UInt8 AGC1_Do_step              :2;
            UInt8 UNUSED_0x22_0             :2;
            UInt8 AGC1_bypass               :1;
            UInt8 AGC1_loop_off             :1;
            UInt8 AGC2_Gud_valid            :1;
            UInt8 UNUSED_0x22_1             :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx22;            

    union            
    {            
        UInt8 AGC2_byte_1;                /* address 0x23 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x23               :1;
            UInt8 AGC1_Gud_valid            :1;
            UInt8 AGC1_aud_valid            :1;
            UInt8 AGC2_Gud                  :5;
#else
            UInt8 AGC2_Gud                  :5;
            UInt8 AGC1_aud_valid            :1;
            UInt8 AGC1_Gud_valid            :1;
            UInt8 UNUSED_0x23               :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx23;            

    union            
    {            
        UInt8 AGC2_byte_2;                /* address 0x24 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 AGC1_gain_Force           :1;
            UInt8 AGC2_gain_Force           :1;
            UInt8 AGC2_loop_off             :1;
            UInt8 AGC2_bypass               :1;
            UInt8 AGC2_Speed_Up             :1;
            UInt8 AGC2_Speed_Do             :1;
            UInt8 AGC2_Do_step              :2;
#else
            UInt8 AGC2_Do_step              :2;
            UInt8 AGC2_Speed_Do             :1;
            UInt8 AGC2_Speed_Up             :1;
            UInt8 AGC2_bypass               :1;
            UInt8 AGC2_loop_off             :1;
            UInt8 AGC2_gain_Force           :1;
            UInt8 AGC1_gain_Force           :1;
#endif
        } __attribute__((packed))bF;        
    } __attribute__((packed))uBx24;                    

    union                    
    {                    
        UInt8 Analog_AGC_byte;            /* address 0x25 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x25_1             :1;
            UInt8 RFAGC_Top                 :3;
            UInt8 UNUSED_0x25_0             :4;
#else
            UInt8 UNUSED_0x25_0             :4;
            UInt8 RFAGC_Top                 :3;
            UInt8 UNUSED_0x25_1             :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx25;            

    union            
    {            
        UInt8 RC_byte;                    /* address 0x26 */
        struct               
        {        
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x26               :4;
            UInt8 RC_ReadCal_3_to_0         :4;
#else
            UInt8 RC_ReadCal_3_to_0         :4;
            UInt8 UNUSED_0x26               :4;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx26;            

    union                            
    {                            
        UInt8 RSSI_byte;                /* address 0x27 */
        struct                               
        {                        
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x27_1             :1;
            UInt8 RC_ReadCal_4              :1;
            UInt8 UNUSED_0x27_0             :6;
#else
            UInt8 UNUSED_0x27_0             :6;
            UInt8 RC_ReadCal_4              :1;
            UInt8 UNUSED_0x27_1             :1;
#endif
        } __attribute__((packed))bF;                
    } __attribute__((packed))uBx27;                            

    union            
    {            
        UInt8 IR_CAL_byte_1;            /* address 0x28 */
        UInt8 UNUSED_0x28;
    } __attribute__((packed))uBx28;            

    union            
    {            
        UInt8 IR_CAL_byte_2;            /* address 0x29 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x29_1             :4;
            UInt8 BUF1b_DCloopIH            :1;
            UInt8 STOB_gain                 :1;
            UInt8 IR_Freq                   :2;
#else
            UInt8 IR_Freq                   :2;
            UInt8 STOB_gain                 :1;
            UInt8 BUF1b_DCloopIH            :1;
            UInt8 UNUSED_0x29_1             :4;
#endif
        } __attribute__((packed))bF;                
    } __attribute__((packed))uBx29;   
    
    union                            
    {                            
        UInt8 IR_CAL_byte_3;            /* address 0x2A */
        struct                               
        {                        
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 shift                     :2;
            UInt8 UNUSED_0x2A_1             :6;
#else
            UInt8 UNUSED_0x2A_1             :6;
            UInt8 shift                     :2;
#endif
        }__attribute__((packed))bF;                    
    } __attribute__((packed))uBx2A;                                

    union                                
    {                                
        UInt8 IR_CAL_byte_4;            /* address 0x2B */
        UInt8 UNUSED_0x2B;
    } __attribute__((packed))uBx2B;                                

    union                                
    {                                
        UInt8 RF_Cal_byte_1;            /* address 0x2C */
        UInt8 UNUSED_0x2C;
    } __attribute__((packed))uBx2C;                                

    union                                
    {                                
        UInt8 RF_Cal_byte_2;            /* address 0x2D */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x2D_1             :2;
            UInt8 capw_filter_sel           :1;
            UInt8 Sel_3dB                   :1;
            UInt8 RefineAV                  :1;
            UInt8 UNUSED_0x2D_0             :1;
            UInt8 rfcal_clock_en            :1;
            UInt8 RFcal_modeOFF             :1;
#else
            UInt8 RFcal_modeOFF             :1;
            UInt8 rfcal_clock_en            :1;
            UInt8 UNUSED_0x2D_0             :1;
            UInt8 RefineAV                  :1;
            UInt8 Sel_3dB                   :1;
            UInt8 capw_filter_sel           :1;
            UInt8 UNUSED_0x2D_1             :2;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx2D;            

    union            
    {            
        UInt8 RF_Cal_byte_3;            /* address 0x2E */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 LF_nHF                    :1;
            UInt8 FilterIndex               :7;
#else
            UInt8 FilterIndex               :7;
            UInt8 LF_nHF                    :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx2E;            

    union            
    {            
        UInt8 RF_Cal_byte_4;            /* address 0x2F */
        UInt8 UNUSED_0x2F;
    } __attribute__((packed))uBx2F;            

    union            
    {            
        UInt8 RF_Cal_byte_5;            /* address 0x30 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 FreqCut_sel               :1;
            UInt8 FreqCut                   :7;
#else
            UInt8 FreqCut                   :7;
            UInt8 FreqCut_sel               :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx30;            

    union            
    {            
        UInt8 RF_Cal_byte_6;            /* address 0x31 */
        UInt8 UNUSED_0x31;
    } __attribute__((packed))uBx31;            

    union            
    {            
        UInt8 RF_Cal_byte_7;            /* address 0x32 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 GAIN_threshold            :3;
            UInt8 UNUSED_0x31               :2;
            UInt8 NmeasAV                   :3;
#else
            UInt8 NmeasAV                   :3;
            UInt8 UNUSED_0x31               :2;
            UInt8 GAIN_threshold            :3;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx32;            

    union            
    {            
        UInt8 RF_Cal_byte_8;            /* address 0x33 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x33               :1;
            UInt8 capw_gain                 :7;
#else
            UInt8 capw_gain                 :7;
            UInt8 UNUSED_0x33               :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx33;            

    union            
    {            
        UInt8 RF_Cal_byte_9;            /* address 0x34 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x34               :1;    
            UInt8 capw_cad                  :7;    
#else
            UInt8 capw_cad                  :7;    
            UInt8 UNUSED_0x34               :1;    
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx34;            

    union            
    {            
        UInt8 RF_Cal_byte_10;            /* address 0x35 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x35               :1;
            UInt8 capw_ctank                :7;
#else
            UInt8 capw_ctank                :7;
            UInt8 UNUSED_0x35               :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx35;            

    union            
    {            
        UInt8 RF_Cal_RAM_byte_1;        /* address 0x36 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x36_2             :3;
            UInt8 SelRAM                    :1;
            UInt8 UNUSED_0x36_1             :1;
            UInt8 ModeI2CRAM                :1;
            UInt8 UNUSED_0x36_0             :1;
            UInt8 WRI2CRAM                  :1;
#else
            UInt8 WRI2CRAM                  :1;
            UInt8 UNUSED_0x36_0             :1;
            UInt8 ModeI2CRAM                :1;
            UInt8 UNUSED_0x36_1             :1;
            UInt8 SelRAM                    :1;
            UInt8 UNUSED_0x36_2             :3;
#endif
        } __attribute__((packed))bF;                
    } __attribute__((packed))uBx36;                            

    union                            
    {                            
        UInt8 RF_Cal_RAM_byte_2;        /* address 0x37 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x37               :4;
            UInt8 AddRAM                    :4;
#else
            UInt8 AddRAM                    :4;
            UInt8 UNUSED_0x37               :4;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx37;            

    union            
    {            
        UInt8 Debug_byte;                /* address 0x38 */
        UInt8 UNUSED_0x38;
    } __attribute__((packed))uBx38;            

    union            
    {            
        UInt8 Fmax_byte_1;                /* address 0x39 */
        UInt8 UNUSED_0x39;
    } __attribute__((packed))uBx39;            

    union            
    {            
        UInt8 Fmax_byte_2;                /* address 0x3A */
        UInt8 UNUSED_0x3A;
    } __attribute__((packed))uBx3A;
    
    union            
    {            
        UInt8 RCCAL_delay;                /* address 0x3B */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 UNUSED_0x3B_1                  :1;
            UInt8 RC_GD_offset                   :2;
            UInt8 RC_GD_sign                     :1;
            UInt8 DTA_NotMOCA                    :1;
            UInt8 RC_LPF_offset                  :2;
            UInt8 RC_LPF_sign                    :1;
#else
            UInt8 RC_LPF_sign                    :1;
            UInt8 RC_LPF_offset                  :2;
            UInt8 DTA_NotMOCA                    :1;
            UInt8 RC_GD_sign                     :1;
            UInt8 RC_GD_offset                   :2;
            UInt8 UNUSED_0x3B_1                  :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx3B;
    
    union            
    {            
        UInt8 AGC1_smooth;                /* address 0x3C */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 RFSW_T_CST                     :3;
            UInt8 AGC1_smooth_t_cst              :3;
            UInt8 UNUSED_0x3E                    :2;
#else
            UInt8 UNUSED_0x3E                    :2;
            UInt8 AGC1_smooth_t_cst              :3;
            UInt8 RFSW_T_CST                     :3;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx3C;
    
    union            
    {            
        UInt8 LT_smooth;                  /* address 0x3D */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 agc1_smooth_transition_on      :1;
            UInt8 Ind_Div2                       :1;
            UInt8 LT_smooth_t_cst                :3;
            UInt8 LT_smooth_offset               :2;
            UInt8 UNUSED_0x3D_0                  :1;
#else
            UInt8 UNUSED_0x3D_0                  :1;
            UInt8 LT_smooth_offset               :2;
            UInt8 LT_smooth_t_cst                :3;
            UInt8 Ind_Div2                       :1;
            UInt8 agc1_smooth_transition_on      :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx3D;
    
    union            
    {            
        UInt8 RF_MUX_test;                /* address 0x3E */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 RFSW_LP_MOVE                   :1;
            UInt8 RFSW_LP                        :1;
            UInt8 UNUSED_0x3E                    :6;
#else
            UInt8 UNUSED_0x3E                    :6;
            UInt8 RFSW_LP                        :1;
            UInt8 RFSW_LP_MOVE                   :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx3E;
    
    union            
    {            
        UInt8 DTA_Filter;                 /* address 0x3F */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 Cap_HP_Right                   :2;
            UInt8 Cap_HP_Left                    :2;
            UInt8 BP_Ind1                        :3;
            UInt8 DTA_bypass                     :1;
#else
            UInt8 DTA_bypass                     :1;
            UInt8 BP_Ind1                        :3;
            UInt8 Cap_HP_Left                    :2;
            UInt8 Cap_HP_Right                   :2;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx3F;
    
    union            
    {            
        UInt8 AGC1bis;                    /* address 0x40 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 PSM_Buf1b                      :2;
            UInt8 UNUSED_0x40                    :2;
            UInt8 pd_AGC1bis                     :1;
            UInt8 Agc1bis_gain                   :3;
#else
            UInt8 Agc1bis_gain                   :3;
            UInt8 pd_AGC1bis                     :1;
            UInt8 UNUSED_0x40                    :2;
            UInt8 PSM_Buf1b                      :2;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx40;

    union            
    {            
        UInt8 IF_byte_1;                  /* address 0x41 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 HP_MTO_T_CST                   :3;
            UInt8 notch_select                   :2;
            UInt8 UNUSED_0x41_0                  :2;
            UInt8 shift_GD_offset                :1;
#else
            UInt8 shift_GD_offset                :1;
            UInt8 UNUSED_0x41_0                  :2;
            UInt8 notch_select                   :2;
            UInt8 HP_MTO_T_CST                   :3;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx41;
    
    union            
    {            
        UInt8 IF_byte_2;                  /* address 0x42 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 PD_hpf                         :1;
            UInt8 hpf_freq                       :2;
            UInt8 Shift_LF_rccal                 :1;
            UInt8 LF_freq                        :4;
#else
            UInt8 LF_freq                        :4;
            UInt8 Shift_LF_rccal                 :1;
            UInt8 hpf_freq                       :2;
            UInt8 PD_hpf                         :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx42;

    union            
    {            
        UInt8 RFSW_SMOOTH;                /* address 0x43 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 HP_MTO3_MOVE                   :1;
            UInt8 HP_MTO2_MOVE                   :1;
            UInt8 HP_MTO3                        :1;
            UInt8 HP_MTO2                        :1;
            UInt8 RFSW_HP_MOVE                   :1;
            UInt8 RFSW_ANALOG_MOVE               :1;
            UInt8 RFSW_HP                        :1;
            UInt8 RFSW_ANALOG                    :1;
#else
            UInt8 RFSW_ANALOG                    :1;
            UInt8 RFSW_HP                        :1;
            UInt8 RFSW_ANALOG_MOVE               :1;
            UInt8 RFSW_HP_MOVE                   :1;
            UInt8 HP_MTO2                        :1;
            UInt8 HP_MTO3                        :1;
            UInt8 HP_MTO2_MOVE                   :1;
            UInt8 HP_MTO3_MOVE                   :1;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx43;
    
    union            
    {            
        UInt8 RFSW_NOTCH;                 /* address 0x44 */
        struct
        {
#ifdef _TMDDTDA18250_MSB_FIRST
            UInt8 BP_Ind2                        :2;
            UInt8 Cap_L6n                        :2;
            UInt8 Cap_L10n                       :2;
            UInt8 Cap_HP_Mid                     :2;
#else
            UInt8 Cap_HP_Mid                     :2;
            UInt8 Cap_L10n                       :2;
            UInt8 Cap_L6n                        :2;
            UInt8 BP_Ind2                        :2;
#endif
        } __attribute__((packed))bF;
    } __attribute__((packed))uBx44;

} __attribute__((packed))TDA18250_I2C_Map_t, *pTDA18250_I2C_Map_t;


typedef struct _tmddTDA18250Object_t {
    tmUnitSelect_t              tUnit;
    tmUnitSelect_t              tUnitW;
    ptmbslFrontEndMutexHandle   pMutex;
    Bool                        init;
    tmbslFrontEndIoFunc_t       sRWFunc;
    tmbslFrontEndTimeFunc_t     sTime;
    tmbslFrontEndDebugFunc_t    sDebug;
    tmbslFrontEndMutexFunc_t    sMutex;
    tmddTDA18250PowerState_t    curPowerState;
    tmddTDA18250Config_t        Config;
    TDA18250_I2C_Map_t          I2CMap;
    tmddTDA18250DeviceType_t    deviceType;
} tmddTDA18250Object_t, *ptmddTDA18250Object_t, **pptmddTDA18250Object_t;


extern tmErrorCode_t ddTDA18250Write(ptmddTDA18250Object_t pObj, UInt8 uSubAddress, UInt8 uNbData);
extern tmErrorCode_t ddTDA18250Read(ptmddTDA18250Object_t pObj, UInt8 uSubAddress, UInt8 uNbData);
extern tmErrorCode_t ddTDA18250Wait(ptmddTDA18250Object_t pObj, UInt32 Time);

extern tmErrorCode_t ddTDA18250MutexAcquire(ptmddTDA18250Object_t   pObj, UInt32 timeOut);
extern tmErrorCode_t ddTDA18250MutexRelease(ptmddTDA18250Object_t   pObj);

#ifdef __cplusplus
}
#endif

#endif /* _TMDD_TDA18250_LOCAL_H */
