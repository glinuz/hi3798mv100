#ifndef __VARIABLE_H__
#define __VARIABLE_H__
// ----- LL variables -----



#define OVA_BASE							0x4C00							// omega variable address base

//#define OVA_PRECHIP_VERSION_7_0
#define OVA_LINK_VERSION					(OVA_BASE-4)					// 4 byte
#define OVA_LINK_VERSION_31_24			(OVA_LINK_VERSION+0)
#define OVA_LINK_VERSION_23_16			(OVA_LINK_VERSION+1)
#define OVA_LINK_VERSION_15_8				(OVA_LINK_VERSION+2)
#define OVA_LINK_VERSION_7_0				(OVA_LINK_VERSION+3)
#define OVA_SECOND_DEMOD_I2C_ADDR		(OVA_BASE-5)

#define OVA_IR_TABLE                    (OVA_BASE-361)                   // 7 * 50 + 6 bytes
#define OVA_HID_TABLE					OVA_IR_TABLE                     // 7 * 50 bytes
#define OVA_IR_TOGGLE_MASK				(OVA_HID_TABLE+7*50)             // 2 bytes
#define OVA_IR_nKEYS					(OVA_IR_TOGGLE_MASK+2)           // 1 byte
#define OVA_IR_FN_EXPIRE_TIME			(OVA_IR_nKEYS+1)                 // 1 byte
#define OVA_IR_REPEAT_PERIOD			(OVA_IR_FN_EXPIRE_TIME+1)        // 1 byte
#define OVA_IR_RESERVED_PARAM			(OVA_IR_REPEAT_PERIOD+1)         // 1 byte

#define OVA_IR_TABLE_ADDR					(OVA_BASE-363)					// 2 bytes pointer point to IR_TABLE
#define OVA_IR_TABLE_ADDR_15_18			(OVA_IR_TABLE_ADDR+0)
#define OVA_IR_TABLE_ADDR_7_0				(OVA_IR_TABLE_ADDR+1)
#define OVA_HOST_REQ_IR_MODE				(OVA_BASE-364)
#define OVA_EEPROM_CFG						(OVA_BASE-620)					// 256bytes
#define OVA_XC4000_PKTCNT					(OVA_BASE-621)
#define OVA_XC4000_CLKCNT1				(OVA_BASE-623)			// 2 bytes
#define OVA_XC4000_CLKCNT2				(OVA_BASE-625)			// 2 bytes
#define OVA_I2C_NO_STOPBIT_PKTCNT	(OVA_BASE-626)
#define OVA_CLK_STRETCH						(OVA_BASE-643)
#define OVA_DUMMY0XX							(OVA_BASE-644)
#define OVA_HW_VERSION						(OVA_BASE-645)
#define OVA_TMP_HW_VERSION			(OVA_BASE-646)

// for API variable name, not use in firmware

#define second_i2c_address				OVA_SECOND_DEMOD_I2C_ADDR	//0x417F
#define ir_table_start_15_8				OVA_IR_TABLE_ADDR_15_18		//0x417F
#define ir_table_start_7_0				OVA_IR_TABLE_ADDR_7_0			//0x4180
#define prechip_version_7_0				0x384F
#define chip_version_7_0					0x1222
#define link_version_31_24				OVA_LINK_VERSION_31_24			//0x83E9
#define link_version_23_16				OVA_LINK_VERSION_23_16			//0x83EA
#define link_version_15_8				OVA_LINK_VERSION_15_8			//0x83EB
#define link_version_7_0					OVA_LINK_VERSION_7_0			//0x83EC
//this file define variable which initialized by AP
//CFOE------------------------------------------

//These variables are initialized by API.
//Don't change the order of the definition of these variables.



//2k
//BASE Address 0x418B
#define var_addr_base                       0x418b
//YUMIN for Samba -- Remove unused log_addr_base _at_ 0x418D;
//YUMIN for Samba -- Remove unused log_data_base _at_ 0x418F;
//YUMIN for Samba -- Remove unused LowerLocalRetrain  _at_ 0x43BB;
//scmd  _at_ 0x4496;
// Do NOT touch the following line: used by script
// --- @xx++ Start variable block
//Initialization..
//API relative
//BASE Address 0x0000
#define trigger_ofsm                        0x0000
#define cfoe_NS_2048_coeff1_25_24           0x0001
#define cfoe_NS_2048_coeff1_23_16           0x0002
#define cfoe_NS_2048_coeff1_15_8            0x0003
#define cfoe_NS_2048_coeff1_7_0             0x0004
#define cfoe_NS_2k_coeff2_24                0x0005
#define cfoe_NS_2k_coeff2_23_16             0x0006
#define cfoe_NS_2k_coeff2_15_8              0x0007
#define cfoe_NS_2k_coeff2_7_0               0x0008

//8k..

#define cfoe_NS_8191_coeff1_25_24           0x0009
#define cfoe_NS_8191_coeff1_23_16           0x000a
#define cfoe_NS_8191_coeff1_15_8            0x000b
#define cfoe_NS_8191_coeff1_7_0             0x000c
#define cfoe_NS_8192_coeff1_25_24           0x000d
#define cfoe_NS_8192_coeff1_23_16           0x000e
#define cfoe_NS_8192_coeff1_15_8            0x000f
#define cfoe_NS_8192_coeff1_7_0             0x0010
#define cfoe_NS_8193_coeff1_25_24           0x0011
#define cfoe_NS_8193_coeff1_23_16           0x0012
#define cfoe_NS_8193_coeff1_15_8            0x0013
#define cfoe_NS_8193_coeff1_7_0             0x0014

#define cfoe_NS_8k_coeff2_24                0x0015
#define cfoe_NS_8k_coeff2_23_16             0x0016
#define cfoe_NS_8k_coeff2_15_8              0x0017
#define cfoe_NS_8k_coeff2_7_0               0x0018

//4k
#define cfoe_NS_4096_coeff1_25_24           0x0019
#define cfoe_NS_4096_coeff1_23_16           0x001a
#define cfoe_NS_4096_coeff1_15_8            0x001b
#define cfoe_NS_4096_coeff1_7_0             0x001c
#define cfoe_NS_4k_coeff2_24                0x001d
#define cfoe_NS_4k_coeff2_23_16             0x001e
#define cfoe_NS_4k_coeff2_15_8              0x001f
#define cfoe_NS_4k_coeff2_7_0               0x0020

#define bfsfcw_fftindex_ratio_7_0           0x0021
#define bfsfcw_fftindex_ratio_15_8          0x0022
#define fftindex_bfsfcw_ratio_7_0           0x0023
#define fftindex_bfsfcw_ratio_15_8          0x0024



#define crystal_clk_7_0                     0x0025
#define crystal_clk_15_8                    0x0026
#define crystal_clk_23_16                   0x0027
#define crystal_clk_31_24                   0x0028


#define bfs_fcw_7_0                         0x0029
#define bfs_fcw_15_8                        0x002a
#define bfs_fcw_22_16                       0x002b

//----------------------------------------------
//statistic performance..

#define qnt_vbc_err_7_0                     0x002c
#define qnt_vbc_err_15_8                    0x002d
#define qnt_vbc_err_23_16                   0x002e
#define r_qnt_vbc_sframe_num                0x002f
#define carmod                              0x0030
#define txmod                               0x0031
 
#define rsd_abort_packet_cnt_7_0            0x0032
#define rsd_abort_packet_cnt_15_8           0x0033
#define rsd_bit_err_cnt_7_0                 0x0034
#define rsd_bit_err_cnt_15_8                0x0035
#define rsd_bit_err_cnt_23_16               0x0036
#define r_rsd_packet_unit_7_0               0x0037
#define r_rsd_packet_unit_15_8              0x0038

#define qnt_vbc_sframe_num                  0x0039
#define rsd_packet_unit_7_0                 0x003a
#define rsd_packet_unit_15_8                0x003b

#define tpsd_lock                           0x003c
#define mpeg_lock                           0x003d
#define RsdSequence                         0x003e
#define VtbSequence                         0x003f

#define Training_Mode                       0x0040
#define RESET_STATE                         0x0041
#define EXT_LNA_OFF                         0x0042
#define aci_0                               0x0043
#define aci_1                               0x0044

#define adcx2                               0x0045
#define tuner_ID                            0x0046
#define empty_channel_status                0x0047
#define signal_strength                     0x0048
#define signal_quality                      0x0049
#define est_rf_level_dbm                    0x004a
#define FreBand                             0x004b
#define suspend_flag                        0x004c
#define SupportRelayCommandWrite            0x004d
#define RelayCommandWrite                   0x004e

//GUI relative
//Initial OFSM
#define OfdmGuiRCN_H                        0x004f
#define OfdmGuiRCN_L                        0x0050
#define MaxTimingRecoveringNo               0x0051
#define strong_signal_detected              0x0052
#define channelFlatnessInd                  0x0053
#define Flatness_Ind_nonCmb                 0x0054
#define AutoDetectedSpectrumInv             0x0055
#define IsSpectrumInv                       0x0056
#define strong_detect_bypass                0x0057
#define ss_dtop_bypass                      0x0058
#define is_low_if                           0x0059
#define EnableTimeSlice                     0x005a
#define SynchronizationType                 0x005b
#define ApplyFastSynchronizationToEchoChannel 0x005c
#define ApplyPwmToRfIf                      0x005d
#define ChannelNo                           0x005e

//release to AAGC document..
#define csi_bypass                          0x005f
#define mobile_bypass                       0x0060
#define EnableSpeedLog                      0x0061

//regression used only..
#define r_rsd_abort_total_packet_7_0        0x0062
#define r_rsd_abort_total_packet_15_8       0x0063
#define r_rsd_abort_total_packet_23_16      0x0064
#define MaxRsdSequence                      0x0065
#define RsdFrameNo                          0x0066
#define MPESuperFrameNo                     0x0067

#define ACI_0_WIN                           0x0068
#define MinRfGain                           0x0069
#define PGA1_GMAX                           0x006a
#define RssiOffset                          0x006b
#define RssiResidual                        0x006c
//Dtop

#define strong_weak_signal_default          0x006d
#define PionDiodeDefaultResetInVHF          0x006e
#define afe_mem4_rssi_comp                  0x006f

#define aagc_speed_detect_count             0x0070
#define aagc_mobile_thr                     0x0071
#define aagc_nonmobile_thr                  0x0072
#define agc_counter                         0x0073
#define DisableAagcTop                      0x0074
#define var_ADC_DESIRED_AWGN_UHF            0x0075
#define var_ADC_DESIRED_DACI_FAR_UHF        0x0076
#define var_ADC_DESIRED_AACI_FAR            0x0077
#define var_ADC_DESIRED_AACI_NEAR           0x0078
#define PwmCtrlHw                           0x0079
#define MaxAgcGain                          0x007a
#define IniAgcGain                          0x007b
#define mccid_bypass                        0x007c
#define CdpfEnDefaultEchoRange              0x007d
#define CdpfIniTestNoSteady                 0x007e
#define aci_thr_shift                       0x007f
#define timing_retrain_cnt                  0x0080
#define ChannelDiffThrSteady                0x0081

#define ACI_2_UTH                           0x0082
#define var_PGA1_GMAX_ACI                   0x0083
#define ACI_2_LTH                           0x0084
#define var_PGA1_GMAX_AWGN                  0x0085
#define RSSI_TARGET_FAR_DACI_UHF            0x0086
#define RSSI_TARGET_NEAR_DACI_UHF           0x0087
#define RSSI_TARGET_FAR_AACI                0x0088
#define RSSI_TARGET_NEAR_AACI               0x0089

#define gui_tdi_lms_en                      0x008a
#define fccid_strobe_scale                  0x008b
#define fccid_strobe_numerator              0x008c
#define fccid_strobe_base                   0x008d
#define use_fccid                           0x008e
#define fft_ave_symbol_num                  0x008f
#define large_tone_num_th_7_0               0x0090
#define large_tone_num_th_15_8              0x0091
#define use_3m_lpf_th                       0x0092
#define ce_var_min_8k                       0x0093
#define ce_var_min_4k                       0x0094
#define ce_var_min_2k                       0x0095
#define ce_var_min_8k_non_flat              0x0096
#define flatness_thr                        0x0097
#define non_mobile_signal_level_offset      0x0098
#define gui_ar_csi_en                       0x0099
#define h2_echo_detected                    0x009a
#define gain_offset_O                       0x009b
#define gain_slope_O                        0x009c
#define pin_diode_gain_loss                 0x009d
#define signal_strength_if_low              0x009e
//flatness
#define flatness_thr_high                   0x009f
#define flatness_thr_low                    0x00a0

//softbit quality
#define sbq1                                0x00a1
#define sbq2                                0x00a2

//DCA
#define dyna_dca_offset_en                  0x00a3
#define ACI_0_UTH                           0x00a4
#define detect_timing_err_en                0x00a5
#define flatness_from_h2_echo               0x00a6

#define timging_error_detection             0x00a7
#define ce_forced_by_rotate                 0x00a8
#define fccid_fft_mask_en                   0x00a9
#define second_fctrl_unforce_en             0x00aa
#define patch_fccid_search_not_rdy          0x00ab
#define high_mobile_detected                0x00ac
#define aci_pindiode_on_th                  0x00ad
#define ChooseFsteCostFunctionFromCdpf      0x00ae
#define signal_level                        0x00af
#define TryConf2En                          0x00b0
#define Lower_tpsd_lock                     0x00b1
#define Upper_tpsd_lock                     0x00b2

#define AgcCtrlType                         0x00b3
#define AACI_RSSI_RANGE                     0x00b4
#define StopByTcl                           0x00b5
#define AACI_CNT_TH                         0x00b6
#define aci_pindiode_off_th_U               0x00b7
#define aci_pindiode_off_th_V               0x00b8
#define ECO_ASIC                            0x00b9
#define NXP_USE_I2C                         0x00ba

//pictor
#define rf_freq_23_16                       0x00bb
#define rf_freq_15_8                        0x00bc
#define rf_freq_7_0                         0x00bd
#define iqik_en                             0x00be
#define dcc_en                              0x00bf
#define agc_mode                            0x00c0
#define aci_2                               0x00c1

#define CdpfIniTestNoMobile                 0x00c2
#define ACIdetection                        0x00c3
#define PinDiode                            0x00c4
#define LNA_Gain                            0x00c5
#define ChannelDiffThrMobile                0x00c6

#define aaci                                0x00c7
#define gain_compensation_O                 0x00c8

#define fbc_m_bdry_7_0                      0x00c9
#define fbc_m_bdry_12_8                     0x00ca
#define rssi_cpll_power                     0x00cb
#define rf_gmax                             0x00cc
#define total_gmax                          0x00cd
#define p_inband_shift                      0x00ce

#define rssi_lo_freq_7_0                    0x00cf
#define rssi_lo_freq_15_8                   0x00d0
#define rssi_cal_freq_7_0                   0x00d1
#define rssi_cal_freq_15_8                  0x00d2

#define lnac_lo_freq_0_7_0                  0x00d3
#define lnac_lo_freq_0_15_8                 0x00d4
#define lnac_lo_freq_1_7_0                  0x00d5
#define lnac_lo_freq_1_15_8                 0x00d6
#define lnac_lo_freq_2_7_0                  0x00d7
#define lnac_lo_freq_2_15_8                 0x00d8
#define lnac_lo_freq_3_7_0                  0x00d9
#define lnac_lo_freq_3_15_8                 0x00da
#define lnac_lo_freq_4_7_0                  0x00db
#define lnac_lo_freq_4_15_8                 0x00dc
#define lnac_lo_freq_5_7_0                  0x00dd
#define lnac_lo_freq_5_15_8                 0x00de
#define lnac_lo_freq_6_7_0                  0x00df
#define lnac_lo_freq_6_15_8                 0x00e0
#define lnac_lo_freq_7_7_0                  0x00e1
#define lnac_lo_freq_7_15_8                 0x00e2
#define lnac_cal_freq_0_7_0                 0x00e3
#define lnac_cal_freq_0_15_8                0x00e4
#define lnac_cal_freq_1_7_0                 0x00e5
#define lnac_cal_freq_1_15_8                0x00e6
#define lnac_cal_freq_2_7_0                 0x00e7
#define lnac_cal_freq_2_15_8                0x00e8
#define lnac_cal_freq_3_7_0                 0x00e9
#define lnac_cal_freq_3_15_8                0x00ea
#define lnac_cal_freq_4_7_0                 0x00eb
#define lnac_cal_freq_4_15_8                0x00ec
#define lnac_cal_freq_5_7_0                 0x00ed
#define lnac_cal_freq_5_15_8                0x00ee
#define lnac_cal_freq_6_7_0                 0x00ef
#define lnac_cal_freq_6_15_8                0x00f0
#define lnac_cal_freq_7_7_0                 0x00f1
#define lnac_cal_freq_7_15_8                0x00f2

// SAMBA // Weicheng 2010122
#define rsd_abort_packet_cnt_B_7_0          0x00f3
#define rsd_abort_packet_cnt_B_15_8         0x00f4
#define rsd_bit_err_cnt_B_7_0               0x00f5
#define rsd_bit_err_cnt_B_15_8              0x00f6
#define rsd_bit_err_cnt_B_23_16             0x00f7
#define r_rsd_packet_unit_B_7_0             0x00f8
#define r_rsd_packet_unit_B_15_8            0x00f9
#define rsd_packet_unit_B_7_0               0x00fa
#define rsd_packet_unit_B_15_8              0x00fb
#define rsd_abort_packet_cnt_C_7_0          0x00fc
#define rsd_abort_packet_cnt_C_15_8         0x00fd
#define rsd_bit_err_cnt_C_7_0               0x00fe
#define rsd_bit_err_cnt_C_15_8              0x00ff
#define rsd_bit_err_cnt_C_23_16             0x0100
#define r_rsd_packet_unit_C_7_0             0x0101
#define r_rsd_packet_unit_C_15_8            0x0102
#define rsd_packet_unit_C_7_0               0x0103
#define rsd_packet_unit_C_15_8              0x0104

#define r_rsd_abort_total_packet_B_7_0      0x0105
#define r_rsd_abort_total_packet_B_15_8     0x0106
#define r_rsd_abort_total_packet_B_23_16    0x0107
#define r_rsd_abort_total_packet_C_7_0      0x0108
#define r_rsd_abort_total_packet_C_15_8     0x0109
#define r_rsd_abort_total_packet_C_23_16    0x010a

#define RsdFrameNo_B                        0x010b
#define RsdFrameNo_C                        0x010c
#define RsdSequence_B                       0x010d
#define RsdSequence_C                       0x010e

#define qnt_vbc_err_b_7_0                   0x010f
#define qnt_vbc_err_b_15_8                  0x0110
#define qnt_vbc_err_b_23_16                 0x0111
#define carmod_b                            0x0112
#define qnt_vbc_err_c_7_0                   0x0113
#define qnt_vbc_err_c_15_8                  0x0114
#define qnt_vbc_err_c_23_16                 0x0115
#define carmod_c                            0x0116
#define sbq1_b                              0x0117
#define sbq2_b                              0x0118
#define sbq1_c                              0x0119
#define sbq2_c                              0x011a

// SAMBA // Weicheng 20110215
#define sfoe_NS_2k_GI32_15_8                0x011b
#define sfoe_NS_2k_GI32_7_0                 0x011c
#define sfoe_NS_2k_GI16_15_8                0x011d
#define sfoe_NS_2k_GI16_7_0                 0x011e
#define sfoe_NS_2k_GI8_15_8                 0x011f
#define sfoe_NS_2k_GI8_7_0                  0x0120
#define sfoe_NS_2k_GI4_15_8                 0x0121
#define sfoe_NS_2k_GI4_7_0                  0x0122

#define sfoe_NS_4k_GI32_15_8                0x0123
#define sfoe_NS_4k_GI32_7_0                 0x0124
#define sfoe_NS_4k_GI16_15_8                0x0125
#define sfoe_NS_4k_GI16_7_0                 0x0126
#define sfoe_NS_4k_GI8_15_8                 0x0127
#define sfoe_NS_4k_GI8_7_0                  0x0128
#define sfoe_NS_4k_GI4_15_8                 0x0129
#define sfoe_NS_4k_GI4_7_0                  0x012a

#define sfoe_NS_8k_GI32_15_8                0x012b
#define sfoe_NS_8k_GI32_7_0                 0x012c
#define sfoe_NS_8k_GI16_15_8                0x012d
#define sfoe_NS_8k_GI16_7_0                 0x012e
#define sfoe_NS_8k_GI8_15_8                 0x012f
#define sfoe_NS_8k_GI8_7_0                  0x0130
#define sfoe_NS_8k_GI4_15_8                 0x0131
#define sfoe_NS_8k_GI4_7_0                  0x0132

// SAMBA // Weicheng 20110315
#define cste2_offset_15_8                   0x0133
#define cste2_offset_7_0                    0x0134
#define cste2_offset_acc_15_8               0x0135
#define cste2_offset_acc_7_0                0x0136
#define cste2_offset_range                  0x0137
#define cste2_offset_detect                 0x0138
#define cste2_avg_num                       0x0139

#define var_NeedTryCEConf2                  0x013a

// variable copy from omega2
#define var_aaci_cnt_detect_UpperTH         0x013b
#define RSSI_TARGET_NEAR_DACI_VHF           0x013c
#define ADC_DESIRED_MOBILE                  0x013d
#define var_signal_strength                 0x013e
#define var_p_inband                        0x013f
#define var_p_inband_lower_limit_qpsk       0x0140
#define var_p_inband_lower_limit_16qam      0x0141
#define var_p_inband_lower_limit_64qam      0x0142
#define var_p_inband_detect                 0x0143
#define var_PGA1_GMAX_DACI_NEAR             0x0144
#define var_PGA1_GMAX_DACI_FAR              0x0145
#define var_PGA1_GMAX_STRONG                0x0146
#define var_ACI_1_UTH_15_8                  0x0147
#define var_ACI_1_UTH_7_0                   0x0148
#define var_ADC_DESIRED_DACI_NEAR_UHF       0x0149
#define var_ADC_DESIRED_AWGN_2K             0x014a
#define var_aaci_cnt_detect_LowerTH         0x014b
#define RSSI_TARGET_FAR_DACI_VHF            0x014c
#define var_predetect_AACI_TH               0x014d
#define var_reset_count                     0x014e
#define var_clean_near_TH                   0x014f
#define test_is_pure_doppler                0x0150
#define test_large_h2                       0x0151
#define test_small_h2                       0x0152
#define test_feq_out_data_h2_byte1          0x0153
#define test_feq_out_data_h2_byte2          0x0154
#define test_feq_out_data_h2_byte3          0x0155
#define test_feq_out_data_h2_byte4          0x0156
#define test_feq_read_eh2_byte1             0x0157
#define test_feq_read_eh2_byte2             0x0158
#define test_feq_read_eh2_byte3             0x0159
#define test_feq_read_eh2_byte4             0x015a
#define var_doppler_UpperTH                 0x015b
#define var_doppler_LowerTH                 0x015c
#define var_doppler_OFF_count               0x015d
#define var_pre_lo_freq_7_0                 0x015e
#define var_pre_lo_freq_15_8                0x015f
#define var_pre_lna_cap_sel                 0x0160
#define RSSI_TARGET_NEAR_AACI_GI32          0x0161
#define var_ACI_case_now                    0x0162
#define var_ACI_case_pre                    0x0163
#define var_change_target_ing               0x0164
#define var_LNA_max                         0x0165
#define var_P_total_max                     0x0166
#define var_P_total_TH_MAXLNA2              0x0167
#define var_P_total_TH_MAXLNA3              0x0168
#define var_P_total_TH_MAXLNA4              0x0169
#define var_P_total_TH_MAXLNA5              0x016a
#define var_LNA_max_pre                     0x016b
#define var_P_total_1                       0x016c
#define var_P_total_2                       0x016d
#define var_P_total_3                       0x016e
#define var_P_total_4                       0x016f
#define var_P_total_5                       0x0170
#define var_P_total_6                       0x0171
#define var_P_total_7                       0x0172
#define var_P_total_8                       0x0173
#define var_LNA_max_force_en                0x0174
#define var_LNA_max_force_value             0x0175
#define var_modify_target_en                0x0176
#define var_ACI_1_LTH_15_8                  0x0177
#define var_P_total_max_pos                 0x0178
#define var_judge_Nordig_DBook              0x0179
#define var_P_total_TH_MAXLNA2_Nordig_DBook 0x017a
#define var_P_total_TH_MAXLNA2_Nordig_DBook_2 0x017b
/////////////////////////////////////////////////////////////////////////////////////
#define var_RFtable_AWGN                    0x017c
#define var_RFtable_DACI                    0x017d
#define var_RFtable_AACI                    0x017e
#define var_ADC_DESIRED_AWGN_VHF            0x017f
#define var_ADC_DESIRED_STRONG_UHF          0x0180
#define var_ADC_DESIRED_STRONG_VHF          0x0181
#define var_ADC_DESIRED_DACI_NEAR_VHF       0x0182
#define var_ADC_DESIRED_DACI_FAR_VHF        0x0183
#define var_ACI_1_LTH_7_0                   0x0184
#define var_ADC_DESIRED_AACI_NEAR_VHF       0x0185
#define var_ADC_DESIRED_AACI_FAR_VHF        0x0186
#define RSSI_TARGET_NEAR_AACI_VHF           0x0187
#define RSSI_TARGET_FAR_AACI_VHF            0x0188
#define var_PGA1_GMAX_AACI_VHF              0x0189
#define var_p_total                         0x018a
#define var_ACI_2_detect_range              0x018b
#define var_FW_init_ready                   0x018c
#define RSSI_TARGET_FAR_AACI_GI32           0x018d

// SAMBA // Weicheng 20110406
#define CDPF_TMCCACErr_testing              0x018e
#define CDPF_SBX_testing                    0x018f
#define CDPF_outside                        0x0190
#define CDPF_outsi_fftrot_shift             0x0191
#define GOOD_SBQ                            0x0192

//omega2
#define var_P_diff                          0x0193
#define test_RF_window                      0x0194
#define test_near_count_H                   0x0195
#define test_near_count_L                   0x0196
#define var_UHF_DACI_NEAR_RFtable           0x0197
#define var_bypass_scan                     0x0198
#define RSSI_TARGET_NEAR_DACI_UHF_GI32      0x0199
#define var_ADC_DESIRED_DACI_NEAR_UHF_GI32  0x019a

//samba for pictor_rom
#define pictor_isr5_cnt1_15_8               0x019b
#define pictor_isr5_cnt1_7_0                0x019c
#define pictor_isr5_cnt2                    0x019d

// SAMBA
#define Board_ID                            0x019e
#define var_FDI7_NO_STUCK_THR               0x019f
#define var_TIMING_BIASED_COUNT_H_HARDSLICE 0x01a0
#define var_TIMING_BIASED_COUNT_L_HARDSLICE 0x01a1
#define var_TIMING_ERR_TONE_THR_HARDSLICE   0x01a2

#define var_channel_power_0_15_8            0x01a3
#define var_channel_power_0_7_0             0x01a4
#define var_channel_power_1_15_8            0x01a5
#define var_channel_power_1_7_0             0x01a6
#define var_channel_power_2_15_8            0x01a7
#define var_channel_power_2_7_0             0x01a8
#define var_channel_power_3_15_8            0x01a9
#define var_channel_power_3_7_0             0x01aa

#define var_channel_location_0_15_8         0x01ab
#define var_channel_location_0_7_0          0x01ac
#define var_channel_location_1_15_8         0x01ad
#define var_channel_location_1_7_0          0x01ae
#define var_channel_location_2_15_8         0x01af
#define var_channel_location_2_7_0          0x01b0
#define var_channel_location_3_15_8         0x01b1
#define var_channel_location_3_7_0          0x01b2

#define var_cohe_segnum_stes                0x01b3
#define var_cohe_segnum_cste2               0x01b4
#define var_fec_rs_th_2_stationary          0x01b5
#define var_fec_rs_th_2_mobile              0x01b6

#define samba_dyn_bb_th_en                  0x01b7
#define pictor_bb_log_2_acc_stationary      0x01b8
#define pictor_bb_log_2_acc_mobile          0x01b9
#define pictor_rssi_log_2_accu_stationary   0x01ba
#define pictor_rssi_log_2_accu_mobile       0x01bb
#define pictor_rssi_median_length_stationary 0x01bc
#define pictor_rssi_median_length_mobile    0x01bd
#define pictor_bb_first_th_mobile           0x01be
#define pictor_bb_first_th_stationary       0x01bf
#define pictor_bb_second_th_mobile          0x01c0
#define pictor_bb_second_th_stationary      0x01c1
#define test_RF_window_mobile               0x01c2
#define pictor_init_busy                    0x01c3
#define pictor_init_status                  0x01c4

#define CDPF_outsi_timing_shift             0x01c5
#define var_API_PLL1_CTRL_EN                0x01c6
#define large_tone_num_th_15_8_ACI          0x01c7
#define var_t_lo_freq_pos                   0x01c8
#define var_SNR                             0x01c9
#define far_aci_p_total_8                   0x01ca

#define var_UART_RX_length                  0x01cb

#define ccif0_state_aci                     0x01cc
#define ccif0_scstrobe_aci                  0x01cd
#define ccif0_fcwccif_7_0_aci               0x01ce
#define ccif0_fcwccif_13_8_aci              0x01cf
#define ccif1_state_aci                     0x01d0
#define ccif1_scstrobe_aci                  0x01d1
#define ccif1_fcwccif_7_0_aci               0x01d2
#define ccif1_fcwccif_13_8_aci              0x01d3

#define lyra_loopthru_control               0x01d4
#define var_LNActrl_AWGN                    0x01d5
#define var_LNActrl_FAR                     0x01d6
#define var_LNActrl_NEAR                    0x01d7
#define var_LNActrl_STRONG                  0x01d8
#define ce_setFlatTH_layerA                 0x01d9

#define var_end                             0x01da

//BASE Address 0xFFFF
#define ofdm_version_31_24                  0x4191
#define ofdm_version_23_16                  0x4192
#define ofdm_version_15_8                   0x4193
#define ofdm_version_7_0                    0x4194
#endif

