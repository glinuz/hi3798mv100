#ifndef __HI3137_H__
#define __HI3137_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define Hi3137_ADC_CTRL0                0x00	
#define Hi3137_ADC_CTRL1                0x01	
#define Hi3137_ADC_CTRL2                0x02	
#define Hi3137_ADC_CTRL3                0x03	
#define Hi3137_ADC_FSCTRL               0x04	
#define Hi3137_PLL_LOCK                 0x05	
#define Hi3137_PLL0_FRAC_L              0x06	
#define Hi3137_PLL0_PD                  0x07	
#define Hi3137_PLL0_POSTDIV             0x08	
#define Hi3137_PLL0_FBDIV               0x09	
#define Hi3137_PLL0_REFDIV              0x0A	
#define Hi3137_PLL0_SPREAD              0x0B	
#define Hi3137_PLL_DIVVAL               0x0C	
#define Hi3137_PLL1_FRAC_L              0x0D	
#define Hi3137_PLL1_PD                  0x0E	
#define Hi3137_PLL1_POSTDIV             0x0F	
#define Hi3137_PLL1_FBDIV               0x10	
#define Hi3137_PLL1_REFDIV              0x11	
#define Hi3137_PLL1_SPREAD              0x12	
#define Hi3137_IO_CTRL0                 0x13	
#define Hi3137_IO_CTRL1                 0x14	
#define Hi3137_IO_CTRL2                 0x15	
#define Hi3137_IO_CTRL3                 0x16	
#define Hi3137_IO_CTRL4                 0x17	
#define Hi3137_SDR_CTRL                 0x18	
#define Hi3137_BIST_MODE_STATUS		 0x19
#define Hi3137_BIST_LEN				 0x1E
#define Hi3137_SDRAM_BIST				 0x1F

#define Hi3137_MAN_RST_CTRL0            0x20	
#define Hi3137_MAN_RST_CTRL1            0x21	
#define Hi3137_STATE_WAITS              0x22	
#define Hi3137_CLK_DEMO_L               0x23	
#define Hi3137_CLK_DEMO_M               0x24	
#define Hi3137_CHIP_ID_L                0x25	    /*区别3138，但3138没使用*/
#define Hi3137_CLK_FEC_L                0x26	
#define Hi3137_CLK_FEC_M                0x27	
#define Hi3137_CHIP_ID_H                0x28	     /*区别3138，但3138没使用*/
#define Hi3137_CLK_SDC_L                0x29	
#define Hi3137_CLK_SDC_M                0x2A	
#define Hi3137_SDC_CTRL                 0x2B	
#define Hi3137_LOCK_FLAG                0x2C	
#define Hi3137_TUNER_SEL                0x2D	
#define Hi3137_RSTN_CTRL                0x2E	
#define Hi3137_ILA_SEL                  0x2F	
#define Hi3137_AGC_SPEED_BOUND          0x30	
#define Hi3137_AGC_GOAL                 0x31	
#define Hi3137_AGCOK_WAIT               0x32	
#define Hi3137_AGC_CTRL                 0x33	
#define Hi3137_AGC_DC_I                 0x34	
#define Hi3137_AGC_DC_Q                 0x35	
#define Hi3137_DAGC_CTRL                0x36	
#define Hi3137_AGC_CTRL_L               0x37	
#define Hi3137_AGC_CTRL_H               0x38	
#define Hi3137_AMP_ERR_IIR              0x39	
#define Hi3137_PDM_CTRL_L               0x3A	
#define Hi3137_PDM_CTRL_H               0x3B	
#define Hi3137_USE_PWM                  0x3C	
#define Hi3137_FEF_TH					  0x3D  /*区别3138，但3138没使用*/
#define Hi3137_MF_SEL                   0x40	            /*区别3138，名称已改变*/
#define Hi3137_SF_RMV                   0x41	            
#define Hi3137_DAGC_REF                 0x42	
#define Hi3137_DAGC_SPEED               0x43	
#define Hi3137_IF_FREQ_L                0x4A	
#define Hi3137_IF_FREQ_H                0x4B	
#define Hi3137_BAND_WIDTH               0x4E	
#define Hi3137_SYN_CTRL0                0x50	
#define Hi3137_CORR_HIGH_TH             0x51	
#define Hi3137_CORR_LOW_TH              0x52	
#define Hi3137_P2_POS_MOD               0x53	
#define Hi3137_P1_THRES                 0x54	
#define Hi3137_CHN_FFT_GI               0x55	
#define Hi3137_P1_SIGNAL                0x56	
#define Hi3137_TIM_OFFSET               0x57	
#define Hi3137_CAR_OFFSET_L             0x58	
#define Hi3137_CAR_OFFSET_H             0x59	
#define Hi3137_T2_CHK_CTRL              0x5D	
#define Hi3137_SOAC_TH                  0x5E	
#define Hi3137_OUTP_RAND                0x5F	
#define Hi3137_LOOP_BW                  0x60	
#define Hi3137_FD_GRP                   0x61	
#define Hi3137_NP_IIR_SFT               0x64	
#define Hi3137_ECHO_THRES               0x67	
#define Hi3137_MIN_THRES                0x69	
#define Hi3137_NP_GRP                   0x6A	
#define Hi3137_TS_A9_SEL                0x6B	
#define Hi3137_TS_87_SEL                0x6C	
#define Hi3137_TS_65_SEL                0x6D	
#define Hi3137_TS_43_SEL                0x6E	
#define Hi3137_TS_21_SEL                0x6F	
#define Hi3137_TIM_LOOP_L				 0x70
#define Hi3137_TIM_LOOP_H				 0x71
#define Hi3137_TS_0_SEL                 0x75	
#define Hi3137_CIR_DIST_0               0x76	
#define Hi3137_CIR_DIST_1               0x77	
#define Hi3137_CIR_DIST_2               0x78	
#define Hi3137_CIR_DIST_3               0x79	
#define Hi3137_SNR_L                    0x7A	
#define Hi3137_SNR_H                    0x7B	
#define Hi3137_DOPPLER                  0x7C	
#define Hi3137_CW_FREQ_L                0x80	
#define Hi3137_CW_FREQ_H                0x81	
#define Hi3137_CLK_ADC_L                0x85	
#define Hi3137_CLK_ADC_M                0x86	
#define Hi3137_CLK_ADC_H                0x87	
#define Hi3137_ATV_STATE                0x88	
#define Hi3137_MCU_HOLD                 0x8f
#define Hi3137_ITER_CTRL                0x91	
#define Hi3137_BER_CTRL                 0x92	
#define Hi3137_AUTO_DLY                 0x93	
#define Hi3137_ITER_NUM                 0x94	
#define Hi3137_ITER_NUM_POST            0x95	
#define Hi3137_FEC_BER_L                0x96	
#define Hi3137_FEC_BER_H                0x97	
#define Hi3137_FEC_FER_L                0x98	
#define Hi3137_FEC_FER_H                0x99	
#define Hi3137_SWITCH_DLY               0x9C	
#define Hi3137_T2_SUCCESS               0x9E	
#define Hi3137_OUTP_ISSY                0xA0	
#define Hi3137_OUTP_DCAP_SET            0xA1	
#define Hi3137_OUTP_CCAP_SET            0xA2	
#define Hi3137_OUTP_PLL0                0xA3	
#define Hi3137_OUTP_PLL1                0xA4	
#define Hi3137_OUTP_PLL2                0xA5	
#define Hi3137_OUTP_PLL3                0xA6	
#define Hi3137_OUTP_PLL4                0xA7	
#define Hi3137_OUTP_CLK_SET             0xA8	
#define Hi3137_OUTP_CLK_SETH            0xA9	
#define Hi3137_OUTP_CLK_SETL            0xAA	
#define Hi3137_OUTP_MODE_SET            0xAB	
#define Hi3137_OUTP_TS_MODE             0xAC	
#define Hi3137_OUTP_PKT_STA             0xAE	
#define Hi3137_OUTP_LIMIT_EN            0xAF	
#define Hi3137_PLP_CTRL                 0xB0	
#define Hi3137_PLP_ID0                  0xB1	
#define Hi3137_PLP_ID1                  0xB2	
#define Hi3137_TPS                      0xB3	
#define Hi3137_STREAM_TYPE              0xB4	
#define Hi3137_CODE_RATE_DVBT			 0xB4
#define Hi3137_TPS_DVBT                 0xB5
#define Hi3137_PAPR_L1MOD               0xB6	
#define Hi3137_PP_VERSION               0xB8	
#define Hi3137_NUM_T2_FRM               0xB9	
#define Hi3137_LDATA_L                  0xBA	
#define Hi3137_LDATA_H                  0xBB	
#define Hi3137_NUM_PLP                  0xBF	
#define Hi3137_PLP_ID                   0xC6	
#define Hi3137_PLP_TYPE                 0xC7	
#define Hi3137_PLP_GRP_ID               0xC8	
#define Hi3137_PLP_PARAM                0xC9	

typedef enum tagHI3137_LOCK_FLAG_E
{
    AGC_LOCK_FLAG = 0,
    SYNC_LOCK_FLAG,
    SYNC_PRE_LOCK_FLAG,
    SIG_T2_LOCK_FLAG,
    TPS_T_LOCK_FLAG,
    FEC_LOCK_FLAG,
    RESERVED_LOCK_2,
    RESERVED_LOCK_3
} HI3137_LOCK_FLAG_E;

HI_S32 hi3137_get_lock_flag(HI_U32 u32TunerPort,HI3137_LOCK_FLAG_E stLockFlag);

extern HI_S32 hi3137_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType);
extern HI_S32 hi3137_set_ter_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ATTR_S *pstTerTunerAttr);
extern HI_S32 hi3137_connect(HI_U32 u32TunerPort,TUNER_ACC_QAM_PARAMS_S *pstChannel);
extern HI_S32 hi3137_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_VOID hi3137_read_tuner_registers(HI_U32 u32TunerPort, void * p);
extern HI_S32 hi3137_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
extern HI_S32 hi3137_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)  ;
extern HI_S32 hi3137_tp_scan(HI_U32 u32TunerPort,HI_UNF_TUNER_TER_SCAN_ATTR_S *pstScanAttr, HI_UNF_TUNER_TER_TPINFO_S *pstTpInfo);
extern HI_S32 hi3137_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 hi3137_set_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut);
extern HI_S32 hi3137_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength);
extern HI_S32 hi3137_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo);
extern HI_S32 hi3137_setplpid(HI_U32 u32TunerPort, HI_U8 u8PLpId, HI_U32 u32Mode);
extern HI_S32 hi3137_set_common_plp_id(HI_U32 u32TunerPort, HI_U8 u8PLpId);
extern HI_S32 hi3137_set_common_plp_combination(HI_U32 u32TunerPort, HI_U8 u8ComPlpEna);
extern HI_S32 hi3137_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb);
extern HI_VOID hi3137_connect_timeout(HI_U32 u32ConnectTimeout);
extern HI_S32 hi3137_get_plp_num(HI_U32 u32TunerPort, HI_U8 *pu8PLPNum);
extern HI_S32 hi3137_get_current_plp_type(HI_U32 u32TunerPort,  HI_UNF_TUNER_T2_PLP_TYPE_E *pstPLPType);
extern HI_S32 hi3137_get_plp_id(HI_U32 u32TunerPort, HI_U8 *u8PlpId);
extern HI_S32 hi3137_get_plp_group_id(HI_U32 u32TunerPort, HI_U8 *u8GrpPlpId);
extern HI_S32 hi3137_set_antena_power(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ANTENNA_POWER_E enPower);
extern HI_S32 hi3137_standby(HI_U32 u32TunerPort, HI_U32 u32Status);
extern HI_S32 hi3137_config_i2c_out(HI_U32 u32TunerPort);
extern HI_S32 hi3137_config_i2c_close(HI_U32 u32TunerPort);


#ifdef __cplusplus
}
#endif

#endif