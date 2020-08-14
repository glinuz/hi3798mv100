/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA182I5a_InstanceCustom.h
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
 * TVFE SW Arch V4 Template: Author Christophe CAZETTES
 *
 * \section info Change Information
 * \par Changelog
 *  -0.1.1.1 RFT413/417 Instability on AGC loop: TOP adapt loop
 *     
 *  -0.1.1.0 Fisrt version
 *  -0.1.1.0 First version
 *  -0.1.1.0 New version
 *  -0.1.1.0 First version
 *  -0.0.0.1 Initial version based on GENERIC 0.1.0.10
 *
 *
 * \par Version_Definition
 *  VERSION_TAG:TDA182I5a_SETTINGS_CUSTOMER_NUM.TDA182I5a_SETTINGS_PROJECT_NUM.TDA182I5a_SETTINGS_MAJOR_VER.TDA182I5a_SETTINGS_MINOR_VER
 *
*/

#ifndef _TDA182I5a_INSTANCE_CUSTOM_H
#define _TDA182I5a_INSTANCE_CUSTOM_H


#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

/* Driver settings version definition */
#define TDA182I5a_SETTINGS_CUSTOMER_NUM 0                       /* SW Settings Customer Number */
#define TDA182I5a_SETTINGS_PROJECT_NUM  1                       /* SW Settings Project Number  */
#define TDA182I5a_SETTINGS_MAJOR_VER    1                       /* SW Settings Major Version   */
#define TDA182I5a_SETTINGS_MINOR_VER    1                       /* SW Settings Minor Version   */

/*  number of tuner supported by driver */
#define TDA182I5a_UNITS  1

/* RC Cal Offset  0x00:+4%; 0x01:+2%; 0x02:0%  0x03:-2%  */
#define TDA182I5a_RC_Cal_Offset 0x01

#define TDA182I5a_D_VIFAGC_En   TDA182I5a_DVIFAGC_En_Analog_VIF_AGC

#define TDA182I5a_cp_cursel_Analog 0
#define TDA182I5a_cp_cursel_Digital 1

#define TDA18225_NOT_SUPPORTED 0

/* Standard Preset Definitions: */
#define TDA182I5a_INSTANCE_CUSTOM_UNSUPPORTED \
    {                                                           /* Unsupported Format */ \
        TDA18225_NOT_SUPPORTED,                                 /* IF */ \
        TDA18225_NOT_SUPPORTED,                                 /* Offset_fmax */ \
        (TDA182I5a_hrm_swap_t)TDA18225_NOT_SUPPORTED,           /* Spectral Inversion */\
        (TDA182I5a_LP_Fc_t)TDA18225_NOT_SUPPORTED,                                  /* LPF */ \
        (TDA182I5a_IF_LPF_Offset_t)TDA18225_NOT_SUPPORTED,                              /* LPF Offset */ \
        (TDA182I5a_IF_LPF_sign_t)TDA18225_NOT_SUPPORTED,                         /* LPF Offset sign */ \
        (TDA182I5a_DC_Notch_t)TDA18225_NOT_SUPPORTED,                             /* DC notch IF PPF */ \
        (TDA182I5a_IF_HP_Enable_t)TDA18225_NOT_SUPPORTED,                         /* HP Enable */ \
        (TDA182I5a_IF_HP_Fc_t)TDA18225_NOT_SUPPORTED,                               /* HPF Fc */ \
        (TDA182I5a_IF_HPF_Offset_t)TDA18225_NOT_SUPPORTED,                              /* HPF Offset */ \
        (TDA182I5a_IF_HPF_sign_t)TDA18225_NOT_SUPPORTED,                         /* HPF Offset sign */ \
        (TDA182I5a_IF_Notch_Enable_t)TDA18225_NOT_SUPPORTED,                     /* IF_Notch_Enable */ \
        (TDA182I5a_IF_Notch_Freq_t)TDA18225_NOT_SUPPORTED,                       /* IF _Notch Freq */ \
        (TDA182I5a_IF_Notch_Offset_t)TDA18225_NOT_SUPPORTED,                            /* IF Notch Offset */ \
        (TDA182I5a_IF_Notch_sign_t)TDA18225_NOT_SUPPORTED,                       /* IF Notch Offset sign */ \
        (TDA182I5a_IFnotchToRSSI_t)TDA18225_NOT_SUPPORTED,     /*  IFnotchToRSSI  */ \
        (TDA182I5a_AGC1_Top_t)TDA18225_NOT_SUPPORTED,                     /* AGC1 TOP I2C DN/UP */ \
        (TDA182I5a_AGC1_Top_Strategy_t)TDA18225_NOT_SUPPORTED,      /* AGC1 TOP STRATEGY */ \
        (TDA182I5a_AGC1_Do_step_t)TDA18225_NOT_SUPPORTED,                        /* AGC1_Do_step */ \
        (TDA182I5a_AGC2_TOP_t)TDA18225_NOT_SUPPORTED,               /* AGC2 TOP I2C DN/UP */ \
        (TDA182I5a_AGC2_Top_Strategy_t)TDA18225_NOT_SUPPORTED,      /* AGC2 TOP STRATEGY */ \
        (TDA182I5a_AGC2_Do_step_t)TDA18225_NOT_SUPPORTED,                        /* AGC2_Do_step */ \
        (TDA182I5a_AGC2_Up_step_t)TDA18225_NOT_SUPPORTED,                           /* AGC2_Up_step */ \
        (TDA182I5a_AGC4_TOP_t)TDA18225_NOT_SUPPORTED,                     /* AGC4 TOP I2C DN/UP */ \
        (TDA182I5a_AGC4_Do_step_t)TDA18225_NOT_SUPPORTED,                        /* AGC4_Do_step */ \
        (TDA182I5a_AGC5_TOP_t)TDA18225_NOT_SUPPORTED,                     /* AGC5 TOP I2C DN/UP */ \
        (TDA182I5a_AGC5_Do_step_t)TDA18225_NOT_SUPPORTED,                        /* AGC5_Do_step */ \
        (TDA182I5a_AGC145_Up_step_t)TDA18225_NOT_SUPPORTED,                        /* AGC145_Up_step */ \
        (TDA182I5a_Top_adapt_AGC1_t)TDA18225_NOT_SUPPORTED,         /* AGC1 TOP adapt */ \
        (TDA182I5a_Top_adapt_AGC2_t)TDA18225_NOT_SUPPORTED,         /* AGC2 TOP adapt */ \
        (TDA182I5a_IF_level_t)TDA18225_NOT_SUPPORTED,                   /* IF Output Level */ \
        (TDA182I5a_AGC_Ovld_TOP_t)TDA18225_NOT_SUPPORTED,                       /* AGC Overload TOP */ \
        (Bool)TDA18225_NOT_SUPPORTED,                           /* AGC fast RF Overload */ \
        (Bool)TDA18225_NOT_SUPPORTED,                           /* AGC fast RF Underload */ \
        (Bool)TDA18225_NOT_SUPPORTED,                           /* AGC fast IF Overload */ \
        (Bool)TDA18225_NOT_SUPPORTED,                           /* AGC fast IF Underload */ \
        (Bool)TDA18225_NOT_SUPPORTED,                           /* AGC1_cc_Freeze */ \
        (Bool)TDA18225_NOT_SUPPORTED,                           /* AGC1_cc_Wide_Enable */ \
        (TDA182I5a_PD_Vsync_Mgt_t)TDA18225_NOT_SUPPORTED,       /* PD_Vsync_Mgt */ \
        (TDA182I5a_Neg_Modulation_t)TDA18225_NOT_SUPPORTED,                      /* Negative modulation */ \
        (TDA182I5a_AGCK_Mode_t)TDA18225_NOT_SUPPORTED,                           /* AGCK Mode */ \
        (TDA182I5a_AGC5_HP_Fc_t)TDA18225_NOT_SUPPORTED,                           /* AGC5_HP_Fc */ \
        (TDA182I5a_RSSI_HP_Fc_t)TDA18225_NOT_SUPPORTED,                           /* RSSI_HP_Fc */ \
        (TDA182I5a_Det6_Cap_Val_t)TDA18225_NOT_SUPPORTED,                            /* Det6_Cap_Val */ \
        (TDA182I5a_RSSI_AV_t)TDA18225_NOT_SUPPORTED,                                 /* RSSI_AV */ \
        (TDA182I5a_RSSI_Ck_Speed_t)TDA18225_NOT_SUPPORTED,     /* RSSI_Ck_Speed*/ \
        (TDA182I5a_LNA_Highpass_t)TDA18225_NOT_SUPPORTED,                        /* LNA_Highpass */ \
        (TDA182I5a_HighPass_AGC2_t)TDA18225_NOT_SUPPORTED,                           /* HighPass_AGC2_algo */ \
        TDA18225_NOT_SUPPORTED,                            /* current charge pump */ \
        (TDA182I5a_DIFOUT_En_t)TDA18225_NOT_SUPPORTED,            /* DIFOUT_En (don't care for TDA18215, disabled by driver)*/ \
        (TDA182I5a_DVIFAGC_En_t)TDA18225_NOT_SUPPORTED,                    /* DVIFAGC_En (don't care for TDA18215, set to Analog by driver)*/ \
        (TDA182I5aPowerSavingMode_t)TDA18225_NOT_SUPPORTED,                   /* Normal or Low */ \
    }

#define TDA182I5a_INSTANCE_CUSTOM_STD_ATSC_6MHz \
    {                                                           /* ATSC */ \
        5250000,                                                /* IF */ \
        3000000,                                                /* Offset_fmax */ \
        TDA182I5a_hrm_swap_No_RF_spectrum_inversion,            /* Spectral Inversion */\
        TDA182I5a_LP_Fc_8_MHz,                                  /* LPF */ \
        TDA182I5a_IF_LPF_Offset_0,                              /* LPF Offset */ \
        TDA182I5a_IF_LPF_sign_Positive,                         /* LPF Offset sign */ \
        TDA182I5a_DC_Notch_Enabled,                             /* DC notch IF PPF */ \
        TDA182I5a_IF_HP_Enable_Enabled,                         /* HP Enable */ \
        TDA182I5a_IF_HP_Fc_3d5_MHz,                             /* HPF Fc */ \
        TDA182I5a_IF_HPF_Offset_8,                              /* HPF Offset */ \
        TDA182I5a_IF_HPF_sign_Negative,                         /* HPF Offset sign */ \
        TDA182I5a_IF_Notch_Enable_Enabled,                      /* IF_Notch_Enable */ \
        TDA182I5a_IF_Notch_Freq_8d25_MHz,                       /* IF _Notch Freq */ \
        TDA182I5a_IF_Notch_Offset_4,                            /* IF Notch Offset */ \
        TDA182I5a_IF_Notch_sign_Positive,                       /* IF Notch Offset sign */ \
        TDA182I5a_IFnotchToRSSI_to_IF_LPF_1st_stage_output,     /*  IFnotchToRSSI  */ \
        TDA182I5a_AGC1_Top_108_102_dBuVrms,                     /* AGC1 TOP I2C DN/UP */ \
        TDA182I5a_AGC1_Top_Strategy_n6_dB_TOP_Do_for_LNA_gain_sup_13_dB,    /* AGC1 TOP STRATEGY */ \
        TDA182I5a_AGC1_Do_step_0d128_ms,                        /* AGC1_Do_step */ \
        TDA182I5a_AGC2_TOP_117_111_110d5_dBuVrms,               /* AGC2 TOP I2C DN/UP */ \
        TDA182I5a_AGC2_Top_Strategy_n1dB_n6dB_TOP_Do_for_LNA_gain_sup_7dB_13dB,   /* AGC2 TOP STRATEGY */ \
        TDA182I5a_AGC2_Do_step_0d128_ms,                        /* AGC2_Do_step */ \
        TDA182I5a_AGC2_Up_step_131_ms,                          /* AGC2_Up_step */ \
        TDA182I5a_AGC4_TOP_112_107_dBuVrms,                     /* AGC4 TOP I2C DN/UP */ \
        TDA182I5a_AGC4_Do_step_0d128_ms,                        /* AGC4_Do_step */ \
        TDA182I5a_AGC5_TOP_112_107_dBuVrms,                     /* AGC5 TOP I2C DN/UP */ \
        TDA182I5a_AGC5_Do_step_0d128_ms,                        /* AGC5_Do_step */ \
        TDA182I5a_AGC145_Up_step_524_ms,                        /* AGC145_Up_step */ \
        TDA182I5a_Top_adapt_AGC1_No_dynamic_TOP_change,         /* AGC1 TOP adapt */ \
        TDA182I5a_Top_adapt_AGC2_No_dynamic_TOP_change,         /* AGC2 TOP adapt */ \
        TDA182I5a_IF_level_n10d3_to_19d7_dB_0d6_Vpp,            /* IF Output Level */ \
        TDA182I5a_AGC_Ovld_TOP_p6_n6,                           /* AGC Overload TOP */ \
        True,                                                   /* AGC fast RF Overload */ \
        True,                                                   /* AGC fast RF Underload */ \
        True,                                                   /* AGC fast IF Overload */ \
        True,                                                   /* AGC fast IF Underload */ \
        False,                                                  /* AGC1_cc_Freeze */ \
        False,                                                  /* AGC1_cc_Wide_Enable */ \
        TDA182I5a_PD_Vsync_Mgt_Vsync_Management_disabled,       /* PD_Vsync_Mgt */ \
        TDA182I5a_Neg_Modulation_Disabled,                      /* Negative modulation */ \
        TDA182I5a_AGCK_Mode_8d192_ms,                           /* AGCK Mode */ \
        TDA182I5a_AGC5_HP_Fc_0d3_MHz,                           /* AGC5_HP_Fc */ \
        TDA182I5a_RSSI_HP_Fc_0d3_MHz,                           /* RSSI_HP_Fc */ \
        TDA182I5a_Det6_Cap_Val_3_pF,                            /* Det6_Cap_Val */ \
        TDA182I5a_RSSI_AV_Fast,                                 /* RSSI_AV */ \
        TDA182I5a_RSSI_Ck_Speed_62d5kHz_ATV_250kHz_DTV_KHz,     /* RSSI_Ck_Speed*/ \
        TDA182I5a_LNA_Highpass_Disabled,                        /* LNA_Highpass */ \
        TDA182I5a_HighPass_AGC2_20MHZ,                          /* HighPass_AGC2_algo */ \
        TDA182I5a_cp_cursel_Digital,                            /* current charge pump */ \
        TDA182I5a_DIFOUT_En_Digital_IF_OUT_Disabled,            /* DIFOUT_En (don't care for TDA18215, disabled by driver)*/ \
        TDA182I5a_DVIFAGC_En_Analog_VIF_AGC,                    /* DVIFAGC_En (don't care for TDA18215, set to Analog by driver)*/ \
        TDA182I5aPowerSavingMode_ATSC_nominal,                  /* Normal or Low */ \
    }

#define TDA182I5a_INSTANCE_CUSTOM_STD_ISdBT_6MHz \
    {                                                           /* ISdBT */ \
        3683000,                                                /* IF */ \
        3000000,                                                /* Offset_fmax */ \
        TDA182I5a_hrm_swap_No_RF_spectrum_inversion,            /* Spectral Inversion */\
        TDA182I5a_LP_Fc_7_MHz,                                  /* LPF */ \
        TDA182I5a_IF_LPF_Offset_8,                              /* LPF Offset */ \
        TDA182I5a_IF_LPF_sign_Negative,                         /* LPF Offset sign */ \
        TDA182I5a_DC_Notch_Enabled,                             /* DC notch IF PPF */ \
        TDA182I5a_IF_HP_Enable_Enabled,                         /* HP Enable */ \
        TDA182I5a_IF_HP_Fc_1_MHz,                               /* HPF Fc */ \
        TDA182I5a_IF_HPF_Offset_0,                              /* HPF Offset */ \
        TDA182I5a_IF_HPF_sign_Positive,                         /* HPF Offset sign */ \
        TDA182I5a_IF_Notch_Enable_Enabled,                      /* IF_Notch_Enable */ \
        TDA182I5a_IF_Notch_Freq_7d25_MHz,                       /* IF _Notch Freq */ \
        TDA182I5a_IF_Notch_Offset_0,                            /* IF Notch Offset */ \
        TDA182I5a_IF_Notch_sign_Positive,                       /* IF Notch Offset sign */ \
        TDA182I5a_IFnotchToRSSI_to_IF_LPF_1st_stage_output,     /*  IFnotchToRSSI  */ \
        TDA182I5a_AGC1_Top_108_102_dBuVrms,                     /* AGC1 TOP I2C DN/UP */ \
        TDA182I5a_AGC1_Top_Strategy_No_dynamic_TOP_change,      /* AGC1 TOP STRATEGY */ \
        TDA182I5a_AGC1_Do_step_0d128_ms,                        /* AGC1_Do_step */ \
        TDA182I5a_AGC2_TOP_115_109_108d5_dBuVrms,               /* AGC2 TOP I2C DN/UP */ \
        TDA182I5a_AGC2_Top_Strategy_No_dynamic_TOP_change,      /* AGC2 TOP STRATEGY */ \
        TDA182I5a_AGC2_Do_step_0d128_ms,                        /* AGC2_Do_step */ \
        TDA182I5a_AGC2_Up_step_131_ms,                          /* AGC2_Up_step */ \
        TDA182I5a_AGC4_TOP_112_107_dBuVrms,                     /* AGC4 TOP I2C DN/UP */ \
        TDA182I5a_AGC4_Do_step_0d128_ms,                        /* AGC4_Do_step */ \
        TDA182I5a_AGC5_TOP_107_102_dBuVrms,                     /* AGC5 TOP I2C DN/UP */ \
        TDA182I5a_AGC5_Do_step_0d128_ms,                        /* AGC5_Do_step */ \
        TDA182I5a_AGC145_Up_step_524_ms,                        /* AGC145_Up_step */ \
        TDA182I5a_Top_adapt_AGC1_n4_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC1 TOP adapt */ \
        TDA182I5a_Top_adapt_AGC2_n5_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC2 TOP adapt */ \
        TDA182I5a_IF_level_n6_to_24_dB_1_Vpp,                   /* IF Output Level */ \
        TDA182I5a_AGC_Ovld_TOP_p6_n6,                           /* AGC Overload TOP */ \
        True,                                                   /* AGC fast RF Overload */ \
        True,                                                   /* AGC fast RF Underload */ \
        True,                                                   /* AGC fast IF Overload */ \
        True,                                                   /* AGC fast IF Underload */ \
        False,                                                  /* AGC1_cc_Freeze */ \
        False,                                                  /* AGC1_cc_Wide_Enable */ \
        TDA182I5a_PD_Vsync_Mgt_Vsync_Management_disabled,       /* PD_Vsync_Mgt */ \
        TDA182I5a_Neg_Modulation_Disabled,                      /* Negative modulation */ \
        TDA182I5a_AGCK_Mode_8d192_ms,                           /* AGCK Mode */ \
        TDA182I5a_AGC5_HP_Fc_0d3_MHz,                           /* AGC5_HP_Fc */ \
        TDA182I5a_RSSI_HP_Fc_0d3_MHz,                           /* RSSI_HP_Fc */ \
        TDA182I5a_Det6_Cap_Val_3_pF,                            /* Det6_Cap_Val */ \
        TDA182I5a_RSSI_AV_Fast,                                 /* RSSI_AV */ \
        TDA182I5a_RSSI_Ck_Speed_62d5kHz_ATV_250kHz_DTV_KHz,     /* RSSI_Ck_Speed*/ \
        TDA182I5a_LNA_Highpass_Enabled,                         /* LNA_Highpass */ \
        TDA182I5a_HighPass_AGC2_Digital,                        /* HighPass_AGC2_algo */ \
        TDA182I5a_cp_cursel_Digital,                            /* current charge pump */ \
        TDA182I5a_DIFOUT_En_Digital_IF_OUT_Disabled,            /* DIFOUT_En (don't care for TDA18215, disabled by driver)*/ \
        TDA182I5a_DVIFAGC_En_Analog_VIF_AGC,                    /* DVIFAGC_En (don't care for TDA18215, set to Analog by driver)*/ \
        TDA182I5aPowerSavingMode_Digital_nominal,               /* Normal or Low */ \
    }

#define TDA182I5a_INSTANCE_CUSTOM_STD_DVBT_1_7MHz \
    {                                                           /* DVB-T/T2 1.7MHz */ \
        850000,                                                 /* IF */ \
        1350000,                                                /* Offset_fmax */ \
        TDA182I5a_hrm_swap_No_RF_spectrum_inversion,            /* Spectral Inversion */\
        TDA182I5a_LP_Fc_1d5_MHz,                                /* LPF */ \
        TDA182I5a_IF_LPF_Offset_4,                              /* LPF Offset */ \
        TDA182I5a_IF_LPF_sign_Positive,                         /* LPF Offset sign */ \
        TDA182I5a_DC_Notch_Disabled,                            /* DC notch IF PPF */ \
        TDA182I5a_IF_HP_Enable_Disabled,                        /* HP Enable */ \
        TDA182I5a_IF_HP_Fc_0d5_MHz,                             /* HPF Fc */ \
        TDA182I5a_IF_HPF_Offset_0,                              /* HPF Offset */ \
        TDA182I5a_IF_HPF_sign_Positive,                         /* HPF Offset sign */ \
        TDA182I5a_IF_Notch_Enable_Disabled,                     /* IF_Notch_Enable */ \
        TDA182I5a_IF_Notch_Freq_6d25_MHz,                       /* IF _Notch Freq */ \
        TDA182I5a_IF_Notch_Offset_0,                            /* IF Notch Offset */ \
        TDA182I5a_IF_Notch_sign_Positive,                       /* IF Notch Offset sign */ \
        TDA182I5a_IFnotchToRSSI_to_IF_LPF_1st_stage_output,     /*  IFnotchToRSSI  */ \
        TDA182I5a_AGC1_Top_108_102_dBuVrms,                     /* AGC1 TOP I2C DN/UP */ \
        TDA182I5a_AGC1_Top_Strategy_No_dynamic_TOP_change,      /* AGC1 TOP STRATEGY */ \
        TDA182I5a_AGC1_Do_step_0d128_ms,                        /* AGC1_Do_step */ \
        TDA182I5a_AGC2_TOP_115_109_108d5_dBuVrms,               /* AGC2 TOP I2C DN/UP */ \
        TDA182I5a_AGC2_Top_Strategy_No_dynamic_TOP_change,      /* AGC2 TOP STRATEGY */ \
        TDA182I5a_AGC2_Do_step_0d128_ms,                        /* AGC2_Do_step */ \
        TDA182I5a_AGC2_Up_step_131_ms,                          /* AGC2_Up_step */ \
        TDA182I5a_AGC4_TOP_112_107_dBuVrms,                     /* AGC4 TOP I2C DN/UP */ \
        TDA182I5a_AGC4_Do_step_0d128_ms,                        /* AGC4_Do_step */ \
        TDA182I5a_AGC5_TOP_107_102_dBuVrms,                     /* AGC5 TOP I2C DN/UP */ \
        TDA182I5a_AGC5_Do_step_0d128_ms,                        /* AGC5_Do_step */ \
        TDA182I5a_AGC145_Up_step_524_ms,                        /* AGC145_Up_step */ \
        TDA182I5a_Top_adapt_AGC1_n4_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC1 TOP adapt */ \
        TDA182I5a_Top_adapt_AGC2_n5_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC2 TOP adapt */ \
        TDA182I5a_IF_level_n6_to_24_dB_1_Vpp,                   /* IF Output Level */ \
        TDA182I5a_AGC_Ovld_TOP_p6_n6,                           /* AGC Overload TOP */ \
        True,                                                   /* AGC fast RF Overload */ \
        True,                                                   /* AGC fast RF Underload */ \
        True,                                                   /* AGC fast IF Overload */ \
        True,                                                   /* AGC fast IF Underload */ \
        False,                                                  /* AGC1_cc_Freeze */ \
        False,                                                  /* AGC1_cc_Wide_Enable */ \
        TDA182I5a_PD_Vsync_Mgt_Vsync_Management_disabled,       /* PD_Vsync_Mgt */ \
        TDA182I5a_Neg_Modulation_Disabled,                      /* Negative modulation */ \
        TDA182I5a_AGCK_Mode_8d192_ms,                           /* AGCK Mode */ \
        TDA182I5a_AGC5_HP_Fc_0d3_MHz,                           /* AGC5_HP_Fc */ \
        TDA182I5a_RSSI_HP_Fc_0d3_MHz,                           /* RSSI_HP_Fc */ \
        TDA182I5a_Det6_Cap_Val_3_pF,                            /* Det6_Cap_Val */ \
        TDA182I5a_RSSI_AV_Fast,                                 /* RSSI_AV */ \
        TDA182I5a_RSSI_Ck_Speed_62d5kHz_ATV_250kHz_DTV_KHz,     /* RSSI_Ck_Speed*/ \
        TDA182I5a_LNA_Highpass_Disabled,                        /* LNA_Highpass */ \
        TDA182I5a_HighPass_AGC2_Digital,                        /* HighPass_AGC2_algo */ \
        TDA182I5a_cp_cursel_Digital,                            /* current charge pump */ \
        TDA182I5a_DIFOUT_En_Digital_IF_OUT_Disabled,            /* DIFOUT_En (don't care for TDA18215, disabled by driver)*/ \
        TDA182I5a_DVIFAGC_En_Analog_VIF_AGC,                    /* DVIFAGC_En (don't care for TDA18215, set to Analog by driver)*/ \
        TDA182I5aPowerSavingMode_Digital_nominal,               /* Normal or Low */ \
    }

#define TDA182I5a_INSTANCE_CUSTOM_STD_DVBT_6MHz \
    {                                                           /* DVB-T/T2 6MHz */ \
		5000000,												/* IF */ \
        3000000,                                                /* Offset_fmax */ \
        TDA182I5a_hrm_swap_No_RF_spectrum_inversion,            /* Spectral Inversion */\
        TDA182I5a_LP_Fc_7_MHz,                                  /* LPF */ \
        TDA182I5a_IF_LPF_Offset_8,                              /* LPF Offset */ \
        TDA182I5a_IF_LPF_sign_Negative,                         /* LPF Offset sign */ \
        TDA182I5a_DC_Notch_Disabled,                            /* DC notch IF PPF */ \
        TDA182I5a_IF_HP_Enable_Enabled,                         /* HP Enable */ \
        TDA182I5a_IF_HP_Fc_1_MHz,                               /* HPF Fc */ \
        TDA182I5a_IF_HPF_Offset_0,                              /* HPF Offset */ \
        TDA182I5a_IF_HPF_sign_Positive,                         /* HPF Offset sign */ \
        TDA182I5a_IF_Notch_Enable_Enabled,                      /* IF_Notch_Enable */ \
        TDA182I5a_IF_Notch_Freq_7d25_MHz,                       /* IF _Notch Freq */ \
        TDA182I5a_IF_Notch_Offset_0,                            /* IF Notch Offset */ \
        TDA182I5a_IF_Notch_sign_Positive,                       /* IF Notch Offset sign */ \
        TDA182I5a_IFnotchToRSSI_to_IF_LPF_1st_stage_output,     /*  IFnotchToRSSI  */ \
        TDA182I5a_AGC1_Top_108_102_dBuVrms,                     /* AGC1 TOP I2C DN/UP */ \
        TDA182I5a_AGC1_Top_Strategy_No_dynamic_TOP_change,      /* AGC1 TOP STRATEGY */ \
        TDA182I5a_AGC1_Do_step_0d128_ms,                        /* AGC1_Do_step */ \
        TDA182I5a_AGC2_TOP_115_109_108d5_dBuVrms,               /* AGC2 TOP I2C DN/UP */ \
        TDA182I5a_AGC2_Top_Strategy_No_dynamic_TOP_change,      /* AGC2 TOP STRATEGY */ \
        TDA182I5a_AGC2_Do_step_0d128_ms,                        /* AGC2_Do_step */ \
        TDA182I5a_AGC2_Up_step_131_ms,                          /* AGC2_Up_step */ \
        TDA182I5a_AGC4_TOP_112_107_dBuVrms,                     /* AGC4 TOP I2C DN/UP */ \
        TDA182I5a_AGC4_Do_step_0d128_ms,                        /* AGC4_Do_step */ \
        TDA182I5a_AGC5_TOP_107_102_dBuVrms,                     /* AGC5 TOP I2C DN/UP */ \
        TDA182I5a_AGC5_Do_step_0d128_ms,                        /* AGC5_Do_step */ \
        TDA182I5a_AGC145_Up_step_524_ms,                        /* AGC145_Up_step */ \
        TDA182I5a_Top_adapt_AGC1_n4_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC1 TOP adapt */ \
        TDA182I5a_Top_adapt_AGC2_n5_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC2 TOP adapt */ \
        TDA182I5a_IF_level_n6_to_24_dB_1_Vpp,                   /* IF Output Level */ \
        TDA182I5a_AGC_Ovld_TOP_p6_n6,                           /* AGC Overload TOP */ \
        True,                                                   /* AGC fast RF Overload */ \
        True,                                                   /* AGC fast RF Underload */ \
        True,                                                   /* AGC fast IF Overload */ \
        True,                                                   /* AGC fast IF Underload */ \
        False,                                                  /* AGC1_cc_Freeze */ \
        False,                                                  /* AGC1_cc_Wide_Enable */ \
        TDA182I5a_PD_Vsync_Mgt_Vsync_Management_disabled,       /* PD_Vsync_Mgt */ \
        TDA182I5a_Neg_Modulation_Disabled,                      /* Negative modulation */ \
        TDA182I5a_AGCK_Mode_8d192_ms,                           /* AGCK Mode */ \
        TDA182I5a_AGC5_HP_Fc_0d3_MHz,                           /* AGC5_HP_Fc */ \
        TDA182I5a_RSSI_HP_Fc_0d3_MHz,                           /* RSSI_HP_Fc */ \
        TDA182I5a_Det6_Cap_Val_3_pF,                            /* Det6_Cap_Val */ \
        TDA182I5a_RSSI_AV_Fast,                                 /* RSSI_AV */ \
        TDA182I5a_RSSI_Ck_Speed_62d5kHz_ATV_250kHz_DTV_KHz,     /* RSSI_Ck_Speed*/ \
        TDA182I5a_LNA_Highpass_Disabled,                        /* LNA_Highpass */ \
        TDA182I5a_HighPass_AGC2_Digital,                        /* HighPass_AGC2_algo */ \
        TDA182I5a_cp_cursel_Digital,                            /* current charge pump */ \
        TDA182I5a_DIFOUT_En_Digital_IF_OUT_Disabled,            /* DIFOUT_En (don't care for TDA18215, disabled by driver)*/ \
        TDA182I5a_DVIFAGC_En_Analog_VIF_AGC,                    /* DVIFAGC_En (don't care for TDA18215, set to Analog by driver)*/ \
        TDA182I5aPowerSavingMode_Digital_nominal,               /* Normal or Low */ \
    }

#define TDA182I5a_INSTANCE_CUSTOM_STD_DVBT_7MHz \
    {                                                           /* DVB-T/T2 7MHz */ \
		5000000,												/* IF */ \
        3500000,                                                /* Offset_fmax */ \
        TDA182I5a_hrm_swap_No_RF_spectrum_inversion,            /* Spectral Inversion */\
        TDA182I5a_LP_Fc_8_MHz,                                  /* LPF */ \
        TDA182I5a_IF_LPF_Offset_4,                              /* LPF Offset */ \
        TDA182I5a_IF_LPF_sign_Negative,                         /* LPF Offset sign */ \
        TDA182I5a_DC_Notch_Disabled,                            /* DC notch IF PPF */ \
        TDA182I5a_IF_HP_Enable_Enabled,                         /* HP Enable */ \
        TDA182I5a_IF_HP_Fc_1d6_MHz,                             /* HPF Fc */ \
        TDA182I5a_IF_HPF_Offset_8,                              /* HPF Offset */ \
        TDA182I5a_IF_HPF_sign_Negative,                         /* HPF Offset sign */ \
        TDA182I5a_IF_Notch_Enable_Enabled,                      /* IF_Notch_Enable */ \
        TDA182I5a_IF_Notch_Freq_8d25_MHz,                       /* IF _Notch Freq */ \
        TDA182I5a_IF_Notch_Offset_0,                            /* IF Notch Offset */ \
        TDA182I5a_IF_Notch_sign_Positive,                       /* IF Notch Offset sign */ \
        TDA182I5a_IFnotchToRSSI_to_IF_LPF_1st_stage_output,     /*  IFnotchToRSSI  */ \
        TDA182I5a_AGC1_Top_108_102_dBuVrms,                     /* AGC1 TOP I2C DN/UP */ \
        TDA182I5a_AGC1_Top_Strategy_No_dynamic_TOP_change,      /* AGC1 TOP STRATEGY */ \
        TDA182I5a_AGC1_Do_step_0d128_ms,                        /* AGC1_Do_step */ \
        TDA182I5a_AGC2_TOP_115_109_108d5_dBuVrms,               /* AGC2 TOP I2C DN/UP */ \
        TDA182I5a_AGC2_Top_Strategy_No_dynamic_TOP_change,      /* AGC2 TOP STRATEGY */ \
        TDA182I5a_AGC2_Do_step_0d128_ms,                        /* AGC2_Do_step */ \
        TDA182I5a_AGC2_Up_step_131_ms,                          /* AGC2_Up_step */ \
        TDA182I5a_AGC4_TOP_112_107_dBuVrms,                     /* AGC4 TOP I2C DN/UP */ \
        TDA182I5a_AGC4_Do_step_0d128_ms,                        /* AGC4_Do_step */ \
        TDA182I5a_AGC5_TOP_107_102_dBuVrms,                     /* AGC5 TOP I2C DN/UP */ \
        TDA182I5a_AGC5_Do_step_0d128_ms,                        /* AGC5_Do_step */ \
        TDA182I5a_AGC145_Up_step_524_ms,                        /* AGC145_Up_step */ \
        TDA182I5a_Top_adapt_AGC1_n4_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC1 TOP adapt */ \
        TDA182I5a_Top_adapt_AGC2_n5_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC2 TOP adapt */ \
        TDA182I5a_IF_level_n6_to_24_dB_1_Vpp,                   /* IF Output Level */ \
        TDA182I5a_AGC_Ovld_TOP_p6_n6,                           /* AGC Overload TOP */ \
        True,                                                   /* AGC fast RF Overload */ \
        True,                                                   /* AGC fast RF Underload */ \
        True,                                                   /* AGC fast IF Overload */ \
        True,                                                   /* AGC fast IF Underload */ \
        False,                                                  /* AGC1_cc_Freeze */ \
        False,                                                  /* AGC1_cc_Wide_Enable */ \
        TDA182I5a_PD_Vsync_Mgt_Vsync_Management_disabled,       /* PD_Vsync_Mgt */ \
        TDA182I5a_Neg_Modulation_Disabled,                      /* Negative modulation */ \
        TDA182I5a_AGCK_Mode_8d192_ms,                           /* AGCK Mode */ \
        TDA182I5a_AGC5_HP_Fc_0d3_MHz,                           /* AGC5_HP_Fc */ \
        TDA182I5a_RSSI_HP_Fc_0d3_MHz,                           /* RSSI_HP_Fc */ \
        TDA182I5a_Det6_Cap_Val_3_pF,                            /* Det6_Cap_Val */ \
        TDA182I5a_RSSI_AV_Fast,                                 /* RSSI_AV */ \
        TDA182I5a_RSSI_Ck_Speed_62d5kHz_ATV_250kHz_DTV_KHz,     /* RSSI_Ck_Speed*/ \
        TDA182I5a_LNA_Highpass_Disabled,                        /* LNA_Highpass */ \
        TDA182I5a_HighPass_AGC2_Digital,                        /* HighPass_AGC2_algo */ \
        TDA182I5a_cp_cursel_Digital,                            /* current charge pump */ \
        TDA182I5a_DIFOUT_En_Digital_IF_OUT_Disabled,            /* DIFOUT_En (don't care for TDA18215, disabled by driver)*/ \
        TDA182I5a_DVIFAGC_En_Analog_VIF_AGC,                    /* DVIFAGC_En (don't care for TDA18215, set to Analog by driver)*/ \
        TDA182I5aPowerSavingMode_Digital_nominal,               /* Normal or Low */ \
    }

#define TDA182I5a_INSTANCE_CUSTOM_STD_DVBT_8MHz \
    {                                                           /* DVB-T/T2 8MHz */ \
        5000000,                                                /* IF */ \
        4000000,                                                /* Offset_fmax */ \
        TDA182I5a_hrm_swap_No_RF_spectrum_inversion,            /* Spectral Inversion */\
        TDA182I5a_LP_Fc_8_MHz,                                  /* LPF */ \
        TDA182I5a_IF_LPF_Offset_8,                              /* LPF Offset */ \
        TDA182I5a_IF_LPF_sign_Positive,                         /* LPF Offset sign */ \
        TDA182I5a_DC_Notch_Disabled,                            /* DC notch IF PPF */ \
        TDA182I5a_IF_HP_Enable_Enabled,                         /* HP Enable */ \
        TDA182I5a_IF_HP_Fc_1d6_MHz,                             /* HPF Fc */ \
        TDA182I5a_IF_HPF_Offset_8,                              /* HPF Offset */ \
        TDA182I5a_IF_HPF_sign_Negative,                         /* HPF Offset sign */ \
        TDA182I5a_IF_Notch_Enable_Enabled,                      /* IF_Notch_Enable */ \
        TDA182I5a_IF_Notch_Freq_9d25_MHz,                       /* IF _Notch Freq */ \
        TDA182I5a_IF_Notch_Offset_0,                            /* IF Notch Offset */ \
        TDA182I5a_IF_Notch_sign_Positive,                       /* IF Notch Offset sign */ \
        TDA182I5a_IFnotchToRSSI_to_IF_LPF_1st_stage_output,     /*  IFnotchToRSSI  */ \
        TDA182I5a_AGC1_Top_108_102_dBuVrms,                     /* AGC1 TOP I2C DN/UP */ \
        TDA182I5a_AGC1_Top_Strategy_No_dynamic_TOP_change,      /* AGC1 TOP STRATEGY */ \
        TDA182I5a_AGC1_Do_step_0d128_ms,                        /* AGC1_Do_step */ \
        TDA182I5a_AGC2_TOP_115_109_108d5_dBuVrms,               /* AGC2 TOP I2C DN/UP */ \
        TDA182I5a_AGC2_Top_Strategy_No_dynamic_TOP_change,      /* AGC2 TOP STRATEGY */ \
        TDA182I5a_AGC2_Do_step_0d128_ms,                        /* AGC2_Do_step */ \
        TDA182I5a_AGC2_Up_step_131_ms,                          /* AGC2_Up_step */ \
        TDA182I5a_AGC4_TOP_112_107_dBuVrms,                     /* AGC4 TOP I2C DN/UP */ \
        TDA182I5a_AGC4_Do_step_0d128_ms,                        /* AGC4_Do_step */ \
        TDA182I5a_AGC5_TOP_107_102_dBuVrms,                     /* AGC5 TOP I2C DN/UP */ \
        TDA182I5a_AGC5_Do_step_0d128_ms,                        /* AGC5_Do_step */ \
        TDA182I5a_AGC145_Up_step_524_ms,                        /* AGC145_Up_step */ \
        TDA182I5a_Top_adapt_AGC1_n4_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC1 TOP adapt */ \
        TDA182I5a_Top_adapt_AGC2_n5_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC2 TOP adapt */ \
        TDA182I5a_IF_level_n6_to_24_dB_1_Vpp,                   /* IF Output Level */ \
        TDA182I5a_AGC_Ovld_TOP_p6_n6,                           /* AGC Overload TOP */ \
        True,                                                   /* AGC fast RF Overload */ \
        True,                                                   /* AGC fast RF Underload */ \
        True,                                                   /* AGC fast IF Overload */ \
        True,                                                   /* AGC fast IF Underload */ \
        False,                                                  /* AGC1_cc_Freeze */ \
        False,                                                  /* AGC1_cc_Wide_Enable */ \
        TDA182I5a_PD_Vsync_Mgt_Vsync_Management_disabled,       /* PD_Vsync_Mgt */ \
        TDA182I5a_Neg_Modulation_Disabled,                      /* Negative modulation */ \
        TDA182I5a_AGCK_Mode_8d192_ms,                           /* AGCK Mode */ \
        TDA182I5a_AGC5_HP_Fc_0d3_MHz,                           /* AGC5_HP_Fc */ \
        TDA182I5a_RSSI_HP_Fc_0d3_MHz,                           /* RSSI_HP_Fc */ \
        TDA182I5a_Det6_Cap_Val_3_pF,                            /* Det6_Cap_Val */ \
        TDA182I5a_RSSI_AV_Fast,                                 /* RSSI_AV */ \
        TDA182I5a_RSSI_Ck_Speed_62d5kHz_ATV_250kHz_DTV_KHz,     /* RSSI_Ck_Speed*/ \
        TDA182I5a_LNA_Highpass_Disabled,                        /* LNA_Highpass */ \
        TDA182I5a_HighPass_AGC2_Digital,                        /* HighPass_AGC2_algo */ \
        TDA182I5a_cp_cursel_Digital,                            /* current charge pump */ \
        TDA182I5a_DIFOUT_En_Digital_IF_OUT_Disabled,            /* DIFOUT_En (don't care for TDA18215, disabled by driver)*/ \
        TDA182I5a_DVIFAGC_En_Analog_VIF_AGC,                    /* DVIFAGC_En (don't care for TDA18215, set to Analog by driver)*/ \
        TDA182I5aPowerSavingMode_Digital_nominal,               /* Normal or Low */ \
    }

#define TDA182I5a_INSTANCE_CUSTOM_STD_DVBT_10MHz \
    {                                                           /* DVB-T/T2 10MHz */ \
        6000000,                                                /* IF */ \
        5000000,                                                /* Offset_fmax */ \
        TDA182I5a_hrm_swap_No_RF_spectrum_inversion,            /* Spectral Inversion */\
        TDA182I5a_LP_Fc_11d25_MHz,                              /* LPF */ \
        TDA182I5a_IF_LPF_Offset_8,                              /* LPF Offset */ \
        TDA182I5a_IF_LPF_sign_Negative,                         /* LPF Offset sign */ \
        TDA182I5a_DC_Notch_Enabled,                             /* DC notch IF PPF */ \
        TDA182I5a_IF_HP_Enable_Enabled,                         /* HP Enable */ \
        TDA182I5a_IF_HP_Fc_1_MHz,                               /* HPF Fc */ \
        TDA182I5a_IF_HPF_Offset_0,                              /* HPF Offset */ \
        TDA182I5a_IF_HPF_sign_Positive,                         /* HPF Offset sign */ \
        TDA182I5a_IF_Notch_Enable_Enabled,                      /* IF_Notch_Enable */ \
        TDA182I5a_IF_Notch_Freq_11d45_MHz,                      /* IF _Notch Freq */ \
        TDA182I5a_IF_Notch_Offset_0,                            /* IF Notch Offset */ \
        TDA182I5a_IF_Notch_sign_Positive,                       /* IF Notch Offset sign */ \
        TDA182I5a_IFnotchToRSSI_to_IF_LPF_1st_stage_output,     /*  IFnotchToRSSI  */ \
        TDA182I5a_AGC1_Top_108_102_dBuVrms,                     /* AGC1 TOP I2C DN/UP */ \
        TDA182I5a_AGC1_Top_Strategy_No_dynamic_TOP_change,      /* AGC1 TOP STRATEGY */ \
        TDA182I5a_AGC1_Do_step_0d128_ms,                        /* AGC1_Do_step */ \
        TDA182I5a_AGC2_TOP_115_109_108d5_dBuVrms,               /* AGC2 TOP I2C DN/UP */ \
        TDA182I5a_AGC2_Top_Strategy_No_dynamic_TOP_change,      /* AGC2 TOP STRATEGY */ \
        TDA182I5a_AGC2_Do_step_0d128_ms,                        /* AGC2_Do_step */ \
        TDA182I5a_AGC2_Up_step_131_ms,                          /* AGC2_Up_step */ \
        TDA182I5a_AGC4_TOP_112_107_dBuVrms,                     /* AGC4 TOP I2C DN/UP */ \
        TDA182I5a_AGC4_Do_step_0d128_ms,                        /* AGC4_Do_step */ \
        TDA182I5a_AGC5_TOP_107_102_dBuVrms,                     /* AGC5 TOP I2C DN/UP */ \
        TDA182I5a_AGC5_Do_step_0d128_ms,                        /* AGC5_Do_step */ \
        TDA182I5a_AGC145_Up_step_524_ms,                        /* AGC145_Up_step */ \
        TDA182I5a_Top_adapt_AGC1_n4_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC1 TOP adapt */ \
        TDA182I5a_Top_adapt_AGC2_n5_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC2 TOP adapt */ \
        TDA182I5a_IF_level_n6_to_24_dB_1_Vpp,                   /* IF Output Level */ \
        TDA182I5a_AGC_Ovld_TOP_p6_n6,                           /* AGC Overload TOP */ \
        True,                                                   /* AGC fast RF Overload */ \
        True,                                                   /* AGC fast RF Underload */ \
        True,                                                   /* AGC fast IF Overload */ \
        True,                                                   /* AGC fast IF Underload */ \
        False,                                                  /* AGC1_cc_Freeze */ \
        False,                                                  /* AGC1_cc_Wide_Enable */ \
        TDA182I5a_PD_Vsync_Mgt_Vsync_Management_disabled,       /* PD_Vsync_Mgt */ \
        TDA182I5a_Neg_Modulation_Disabled,                      /* Negative modulation */ \
        TDA182I5a_AGCK_Mode_8d192_ms,                           /* AGCK Mode */ \
        TDA182I5a_AGC5_HP_Fc_0d3_MHz,                           /* AGC5_HP_Fc */ \
        TDA182I5a_RSSI_HP_Fc_0d3_MHz,                           /* RSSI_HP_Fc */ \
        TDA182I5a_Det6_Cap_Val_3_pF,                            /* Det6_Cap_Val */ \
        TDA182I5a_RSSI_AV_Fast,                                 /* RSSI_AV */ \
        TDA182I5a_RSSI_Ck_Speed_62d5kHz_ATV_250kHz_DTV_KHz,     /* RSSI_Ck_Speed*/ \
        TDA182I5a_LNA_Highpass_Disabled,                        /* LNA_Highpass */ \
        TDA182I5a_HighPass_AGC2_Digital,                        /* HighPass_AGC2_algo */ \
        TDA182I5a_cp_cursel_Digital,                            /* current charge pump */ \
        TDA182I5a_DIFOUT_En_Digital_IF_OUT_Disabled,            /* DIFOUT_En (don't care for TDA18215, disabled by driver)*/ \
        TDA182I5a_DVIFAGC_En_Analog_VIF_AGC,                    /* DVIFAGC_En (don't care for TDA18215, set to Analog by driver)*/ \
        TDA182I5aPowerSavingMode_Digital_nominal,               /* Normal or Low */ \
    }

#define TDA182I5a_INSTANCE_CUSTOM_STD_DMBT_8MHz \
    {                                                           /* DMB-T */ \
        5000000,                                                /* IF */ \
        4000000,                                                /* Offset_fmax */ \
        TDA182I5a_hrm_swap_No_RF_spectrum_inversion,            /* Spectral Inversion */\
        TDA182I5a_LP_Fc_9_MHz,                                  /* LPF */ \
        TDA182I5a_IF_LPF_Offset_8,                              /* LPF Offset */ \
        TDA182I5a_IF_LPF_sign_Negative,                         /* LPF Offset sign */ \
        TDA182I5a_DC_Notch_Disabled,                            /* DC notch IF PPF */ \
        TDA182I5a_IF_HP_Enable_Enabled,                         /* HP Enable */ \
        TDA182I5a_IF_HP_Fc_1_MHz,                               /* HPF Fc */ \
        TDA182I5a_IF_HPF_Offset_0,                              /* HPF Offset */ \
        TDA182I5a_IF_HPF_sign_Positive,                         /* HPF Offset sign */ \
        TDA182I5a_IF_Notch_Enable_Enabled,                      /* IF_Notch_Enable */ \
        TDA182I5a_IF_Notch_Freq_9d25_MHz,                       /* IF _Notch Freq */ \
        TDA182I5a_IF_Notch_Offset_4,                            /* IF Notch Offset */ \
        TDA182I5a_IF_Notch_sign_Positive,                       /* IF Notch Offset sign */ \
        TDA182I5a_IFnotchToRSSI_to_IF_LPF_1st_stage_output,     /*  IFnotchToRSSI  */ \
        TDA182I5a_AGC1_Top_108_102_dBuVrms,                     /* AGC1 TOP I2C DN/UP */ \
        TDA182I5a_AGC1_Top_Strategy_No_dynamic_TOP_change,      /* AGC1 TOP STRATEGY */ \
        TDA182I5a_AGC1_Do_step_0d128_ms,                        /* AGC1_Do_step */ \
        TDA182I5a_AGC2_TOP_115_109_108d5_dBuVrms,               /* AGC2 TOP I2C DN/UP */ \
        TDA182I5a_AGC2_Top_Strategy_No_dynamic_TOP_change,      /* AGC2 TOP STRATEGY */ \
        TDA182I5a_AGC2_Do_step_0d128_ms,                        /* AGC2_Do_step */ \
        TDA182I5a_AGC2_Up_step_131_ms,                          /* AGC2_Up_step */ \
        TDA182I5a_AGC4_TOP_112_107_dBuVrms,                     /* AGC4 TOP I2C DN/UP */ \
        TDA182I5a_AGC4_Do_step_0d128_ms,                        /* AGC4_Do_step */ \
        TDA182I5a_AGC5_TOP_107_102_dBuVrms,                     /* AGC5 TOP I2C DN/UP */ \
        TDA182I5a_AGC5_Do_step_0d128_ms,                        /* AGC5_Do_step */ \
        TDA182I5a_AGC145_Up_step_524_ms,                        /* AGC145_Up_step */ \
        TDA182I5a_Top_adapt_AGC1_n4_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC1 TOP adapt */ \
        TDA182I5a_Top_adapt_AGC2_n5_dB_TOP_for_IFLPF_gain_inf_9dB_12dB,      /* AGC2 TOP adapt */ \
        TDA182I5a_IF_level_n6_to_24_dB_1_Vpp,                   /* IF Output Level */ \
        TDA182I5a_AGC_Ovld_TOP_p6_n6,                           /* AGC Overload TOP */ \
        True,                                                   /* AGC fast RF Overload */ \
        True,                                                   /* AGC fast RF Underload */ \
        True,                                                   /* AGC fast IF Overload */ \
        True,                                                   /* AGC fast IF Underload */ \
        False,                                                  /* AGC1_cc_Freeze */ \
        False,                                                  /* AGC1_cc_Wide_Enable */ \
        TDA182I5a_PD_Vsync_Mgt_Vsync_Management_disabled,       /* PD_Vsync_Mgt */ \
        TDA182I5a_Neg_Modulation_Disabled,                      /* Negative modulation */ \
        TDA182I5a_AGCK_Mode_8d192_ms,                           /* AGCK Mode */ \
        TDA182I5a_AGC5_HP_Fc_0d3_MHz,                           /* AGC5_HP_Fc */ \
        TDA182I5a_RSSI_HP_Fc_0d3_MHz,                           /* RSSI_HP_Fc */ \
        TDA182I5a_Det6_Cap_Val_3_pF,                            /* Det6_Cap_Val */ \
        TDA182I5a_RSSI_AV_Fast,                                 /* RSSI_AV */ \
        TDA182I5a_RSSI_Ck_Speed_62d5kHz_ATV_250kHz_DTV_KHz,     /* RSSI_Ck_Speed*/ \
        TDA182I5a_LNA_Highpass_Disabled,                        /* LNA_Highpass */ \
        TDA182I5a_HighPass_AGC2_Digital,                        /* HighPass_AGC2_algo */ \
        TDA182I5a_cp_cursel_Digital,                            /* current charge pump */ \
        TDA182I5a_DIFOUT_En_Digital_IF_OUT_Disabled,            /* DIFOUT_En (don't care for TDA18215, disabled by driver)*/ \
        TDA182I5a_DVIFAGC_En_Analog_VIF_AGC,                    /* DVIFAGC_En (don't care for TDA18215, set to Analog by driver)*/ \
        TDA182I5aPowerSavingMode_DTMB_nominal,               /* Normal or Low */ \
    }


/* Default configuration */
#define TDA182I5a_INSTANCE_CUSTOM_CURPOWERSTATE_DEF tmPowerMax
#define TDA182I5a_INSTANCE_CUSTOM_CURLLPOWERSTATE_DEF TDA182I5a_PowerMax
#define TDA182I5a_INSTANCE_CUSTOM_SPECINV_DEF TDA182I5a_hrm_swap_No_RF_spectrum_inversion
#define TDA182I5a_INSTANCE_CUSTOM_RF_DEF 0
#define TDA182I5a_INSTANCE_CUSTOM_IF_DEF 5380000
#define TDA182I5a_INSTANCE_CUSTOM_LO_DEF 0
#define TDA182I5a_INSTANCE_CUSTOM_SPECTRUN_INV_DEF 0
#define TDA182I5a_INSTANCE_CUSTOM_STANDARDMODE_DEF TDA182I5a_StandardMode_Max
#define TDA182I5a_INSTANCE_CUSTOM_CURPOWERSAVINGMODE TDA182I5aPowerSavingMode_Max

/* Power settings */
#define TDA182I5a_INSTANCE_CUSTOM_POWER_DEF \
    TDA182I5a_INSTANCE_CUSTOM_CURPOWERSTATE_DEF,                /* curPowerState */ \
    TDA182I5a_INSTANCE_CUSTOM_CURLLPOWERSTATE_DEF,              /* curLLPowerState */ \
    {                                                           /* mapLLPowerState */ \
        TDA182I5a_PowerNormalMode,                              /* tmPowerOn (D0) */ \
        TDA182I5a_PowerStandby_LT,                              /* tmPowerStandby (D1) */ \
        TDA182I5a_PowerStandby_LT,                              /* tmPowerSuspend (D2) */ \
        TDA182I5a_PowerStandby_LT                               /* tmPowerOff (D3) */ \
    }, \
    TDA182I5a_INSTANCE_CUSTOM_CURPOWERSAVINGMODE

/* Standard Presets Aggregation: */
#define TDA182I5a_INSTANCE_CUSTOM_STD_DEF \
    { \
        TDA182I5a_INSTANCE_CUSTOM_UNSUPPORTED, \
        TDA182I5a_INSTANCE_CUSTOM_UNSUPPORTED, \
        TDA182I5a_INSTANCE_CUSTOM_STD_ATSC_6MHz, \
        TDA182I5a_INSTANCE_CUSTOM_STD_ISdBT_6MHz, \
        TDA182I5a_INSTANCE_CUSTOM_STD_DVBT_1_7MHz, \
        TDA182I5a_INSTANCE_CUSTOM_STD_DVBT_6MHz, \
        TDA182I5a_INSTANCE_CUSTOM_STD_DVBT_7MHz, \
        TDA182I5a_INSTANCE_CUSTOM_STD_DVBT_8MHz, \
        TDA182I5a_INSTANCE_CUSTOM_STD_DVBT_10MHz, \
        TDA182I5a_INSTANCE_CUSTOM_STD_DMBT_8MHz, \
        TDA182I5a_INSTANCE_CUSTOM_UNSUPPORTED, \
        TDA182I5a_INSTANCE_CUSTOM_UNSUPPORTED, \
        TDA182I5a_INSTANCE_CUSTOM_UNSUPPORTED, \
        TDA182I5a_INSTANCE_CUSTOM_UNSUPPORTED, \
        TDA182I5a_INSTANCE_CUSTOM_UNSUPPORTED, \
        TDA182I5a_INSTANCE_CUSTOM_UNSUPPORTED, \
        TDA182I5a_INSTANCE_CUSTOM_UNSUPPORTED, \
        TDA182I5a_INSTANCE_CUSTOM_UNSUPPORTED, \
    }

#define TDA182I5a_VCO_DEF \
    { \
       {TDA182I5a_VCO_HIGH_MAX, TDA182I5a_VCO_HIGH_MIN }, \
       {TDA182I5a_VCO_MED_MAX, TDA182I5a_VCO_MED_MIN }, \
       {TDA182I5a_VCO_LOW_MAX, TDA182I5a_VCO_LOW_MIN }, \
    }

#define TDA182I5a_INSTANCE_CUSTOM_MASTER_0 \
    0, /* Identity unknown, auto-detected by the driver */ \
    TDA182I5a_INSTANCE_CUSTOM_POWER_DEF, \
    TDA182I5a_VCO_DEF, \
    50, /* temp_meas */ \
    TDA182I5a_INSTANCE_CUSTOM_RF_DEF,                                /* uRF */ \
    TDA182I5a_INSTANCE_CUSTOM_IF_DEF,                                /* uIF */ \
    TDA182I5a_INSTANCE_CUSTOM_LO_DEF,                                /* uProgRF */ \
    TDA182I5a_INSTANCE_CUSTOM_SPECTRUN_INV_DEF,                      /* uSpectrumInv */ \
    0,                                                               /* uFVCO */ \
    TDA182I5a_INSTANCE_CUSTOM_STANDARDMODE_DEF,                      /* StandardMode */ \
    Null,                                                            /* pStandard */ \
    (TDA182I5_Mode_LTO_Used |  TDA182I5_Mode_XTOUT_Used),            /* Tuner Mode uses LTO and XTOUT (nota: TDA18275A LTO forced to UnUsed by driver*/ \
    TDA182I5a_HwState_InitNotDone,                                   /* eHwState */ \
    0,                                                               /* uLUCodeProgrammed */ \
    TDA182I5a_INSTANCE_CUSTOM_STD_DEF, \

/* Custom Driver Instance Parameters: (Path 0) */
#define TDA182I5a_INSTANCE_CUSTOM_0 \
    TDA182I5a_INSTANCE_CUSTOM_MASTER_0

#ifdef __cplusplus
}
#endif

#endif /* _TDA182I5a_INSTANCE_CUSTOM_H */

