/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: drv_demod.h
* Description:
*
* History:
* Version   Date             Author     DefectNum        Description
* main\1    2010-09-20   a53372    NULL                Create this file.
***********************************************************************************/
#ifndef __DRV_DEMOD_H__
#define __DRV_DEMOD_H__

//#include <linux/seq_file.h>
#include "hi_type.h"
#include "hi_drv_tuner.h"

typedef struct tagHI3136_CFG_ITEM_S
{
    HI_U16 u16TunerLPF_100Khz;
    HI_UNF_TUNER_IQSPECTRUM_MODE_E enIQSpectrum;
    HI_U32 u32CLK_DEMO_kHz;
} HI3136_CFG_ITEM_S;

typedef struct tagHI3136_TP_INFO_S
{
    HI_U32 u32Frequency; //UNIT:kHz
    HI_U32 u32SymbolRate;//UNIT:kHz
    //HI_U8 agc_h8;
    HI_U8 cbs_reliablity;
} HI3136_TP_INFO_S;

extern HI3136_CFG_ITEM_S hi3136_cfg[];
extern HI_U32 u32LPF_KHz[];

HI_VOID hi3136_config_i2c_out(HI_U32 u32TunerPort, HI_BOOL bTuner);

#define HI3136I_I2C_CHAN 2

#define		HI3136_BS_WINDOW_STEP_KHZ    45000	//27000

/*hi3136 demod reg address*/
//#define		HI3136_SYSCLK_SAMPLE	        	125000//110000  //Unit KHZ

#define  LDPC_ITER_LMT_ON   1
#define  LDPC_ITER_45M      19
#define  LDPC_ITER_MAX      50
#define  LDPC_ITER_ADDR     0x82
#define  LDPC_ITER_DEFAULT  0x50

#define HI_TUNER_CHECKPOINTER(ptr)                                   \
    do                                                      \
    {                                                       \
        if (!(ptr))                                         \
        {                                                   \
            HI_INFO_TUNER("pointer is null\n");             \
            return HI_ERR_TUNER_INVALID_POINT;                     \
        }                                                   \
    } while (0)


#if 0
#define          POLAR_SWITCH					0     /*0 level for 18V,1 level for 13V*/
/*Diseqc Command word*/
/*-----------------------------------
÷°Õ∑+µÿ÷∑+√¸¡Ó+ ˝æ›
-----------------------------------*/
#define		HI3136_DISEQC_CMD1 	              0xE0  //ËÂÕ∑
#define		HI3136_DISEQC_CMD2 	              0x10  //µÿ÷∑
#define		HI3136_DISEQC_CMD3 	              0x38 //√¸¡Ócommitted
#define		HI3136_DISEQC_CMD3_1 	              0x39 //√¸¡Ó uncommitted

#define		HI3136_DISEQC_CMD4_LNB1         0xF0
#define		HI3136_DISEQC_CMD4_SW01        0xF0
#define		HI3136_DISEQC_CMD4_SW02        0xF1
#define		HI3136_DISEQC_CMD4_SW03        0xF2
#define		HI3136_DISEQC_CMD4_SW04        0xF3
#define		HI3136_DISEQC_CMD4_SW05        0xF4
#define		HI3136_DISEQC_CMD4_SW06        0xF5
#define		HI3136_DISEQC_CMD4_SW07        0xF6
#define		HI3136_DISEQC_CMD4_SW08        0xF7
#define		HI3136_DISEQC_CMD4_SW09        0xF8
#define		HI3136_DISEQC_CMD4_SW10        0xF9
#define		HI3136_DISEQC_CMD4_SW11        0xFA
#define		HI3136_DISEQC_CMD4_SW12        0xFB
#define		HI3136_DISEQC_CMD4_SW13        0xFC
#define		HI3136_DISEQC_CMD4_SW14        0xFD
#define		HI3136_DISEQC_CMD4_SW15        0xFE
#define		HI3136_DISEQC_CMD4_SW16        0xFF
#endif

/*PLL module*/
#define		PLL_CTRL1				              0x00
#define		PLL_CTRL2				              0x01
#define		PLL_CTRL3				              0x02
#define		PLL_CTRL4				              0x03
#define		PLL_CTRL5				              0x04
#define		PLL_CTRL6				              0x05
#define		PLL_CTRL7				              0x06
#define		PLL_CTRL8				              0x07
#define		PLL_CTRL9				              0x08
#define		PLL_CTRL10				              0x09
#define		ADC_CTRL1				              0x0a
#define		ADC_CTRL2				              0x0c

#define		CLK_DEMO_L				              0x23
#define		CLK_DEMO_M				              0x24
#define		CLK_DEMO_H				              0x25

#define		CLK_FEC_L				              0x26
#define		CLK_FEC_M				              0x27
#define		CLK_FEC_H				              0x28

/*IP_IO_CONFIG*/
#define		RSTN_CTRL				              0x2e

/*CBS module*/
#define     CBS_TH1_COR_NO              0xa2
#define		CBS_CTRL_RDADDR				0xa8
#define		CBS_FS_L				              0xa9
#define		CBS_FS_H				              0xaa
#define		CBS_FC_L				              0xab
#define		CBS_FC_H				              0xac
#define		CBS_FINISH				              0xad
#define		CBS_RELIABLITY			              0xae
#define		CBS_R2_NUM			              0xaf
/*MAN module*/
#define		LOCK_FLAG			              	0x2c
#define		TUNER_SEL			              	0x2d
/*AGC module*/
#define		AGC_CTRL			              	0x33
#define		AGC_CTRL_L			              	0x37
#define		AGC_CTRL_H			              	0x38


/*TR module*/
#define		FS_L			              	       0x46
#define		FS_H			              	       0x47
#define		CENT_FREQ_L			              0x48
#define		CENT_FREQ_H			              0x49
/*CR module*/
#define		CR_PLS_CODE			              0x5c

#define		NOISE_POW_CPU_L			       0x7e
#define		NOISE_POW_CPU_H			       0x7f

/*CR module*/
#define		SYNC_STATUS			              0x66


/*FEC module*/
#define		LDPC_ITER			              	0x82
#define		BER_CTRL			              	0x83

#define		FEC_BER_L			              	0x84
#define		FEC_BER_H			              	0x85
#define		FEC_FER_L			              	0x86
#define		FEC_FER_H			              	0x87
#define		SEAR_RESULT			             	0x8e


/*OUTP module*/
#define		TS_PARALL_CTRL			              0x90
#define		TS_OUT10_SEL			              0x91
#define		TS_OUT32_SEL			              0x92
#define		TS_OUT54_SEL			              0x93
#define		TS_OUT76_SEL			              0x94
#define		TS_OUT98_SEL			              0x95
#define		TS_CTRL0			              	0x96
#define		TS_CTRL1			              	0x97
#define		TS_CTRL4			              	0x98
#define		MATYPE_OUT		              	0x9c
#define		ISI_NUM			              	0x9d


/*DISEQC module*/
#define		DSEC_ADDR		              		0xb0
#define		DSEC_DATA		              		0xb1
#define		DSEC_RATIO_L		              	0xb2
#define		DSEC_RATIO_H		              	0xb3
#define		TX_CTRL			              		0xb4
#define		DSEC_EN			              	0xb7
#define		RX_STATE			              	0xb8
#define		INT_STATE			              	0xb9

/*QAM modules base address*/
#define		EQU_BASE_ADDR				0x00	
#define		TR_BASE_ADDR				0x10
#define		CR_BASE_ADDR				0x20
#define		MC_BASE_ADDR				0x40
#define		AGC_BASE_ADDR				0x50
#define		BAGC_BASE_ADDR			0x60
#define		DP_BASE_ADDR				0x90
#define		QAM_BASE_ADDR				0xb0
#define		QAM_DEBUG_ADDR			0xc0
#define		DAGC_BASE_ADDR			0xd0

#define	    EQU_CTRL_2_ADDR		    EQU_BASE_ADDR + 0x01  //20130305
#define		EQU_CTRL_3_ADDR			EQU_BASE_ADDR + 0x02
#define		EQU_CTRL_4_ADDR			EQU_BASE_ADDR + 0x03
#define		EQU_CTRL_6_ADDR			EQU_BASE_ADDR + 0x04
#define		EQU_CTRL_7_ADDR			EQU_BASE_ADDR + 0x05
#define		EQU_CTRL_8_ADDR			EQU_BASE_ADDR + 0x06
#define		EQU_CTRL_9_ADDR			EQU_BASE_ADDR + 0x07
#define		EQU_CTRL_10_ADDR		EQU_BASE_ADDR + 0x08

#define		EQU_STAT_2_ADDR			EQU_BASE_ADDR + 0x0A
#define		EQU_STAT_3_ADDR			EQU_BASE_ADDR + 0x0B  
#define		EQU_STAT_4_ADDR			EQU_BASE_ADDR + 0x0C  
#define		EQU_STAT_5_ADDR			EQU_BASE_ADDR + 0x0D  
#define		EQU_STAT_6_ADDR			EQU_BASE_ADDR + 0x0E  
#define		EQU_STAT_7_ADDR			EQU_BASE_ADDR + 0x0F  
//above addr belong to equalizer

#define		TR_CTRL_2_ADDR			TR_BASE_ADDR + 0x00
#define		TR_CTRL_4_ADDR			TR_BASE_ADDR + 0x01
#define		TR_CTRL_5_ADDR			TR_BASE_ADDR + 0x02
#define		TR_CTRL_6_ADDR			TR_BASE_ADDR + 0x03
#define		TR_CTRL_8_ADDR			TR_BASE_ADDR + 0x04
#define		TR_CTRL_9_ADDR			TR_BASE_ADDR + 0x05
#define		TR_CTRL_10_ADDR 		TR_BASE_ADDR + 0x06
#define		TR_CTRL_11_ADDR 		TR_BASE_ADDR + 0x07
#define		TR_CTRL_12_ADDR 		TR_BASE_ADDR + 0x08
#define		TR_CTRL_13_ADDR 		TR_BASE_ADDR + 0x09
#define		TR_CTRL_14_ADDR 		TR_BASE_ADDR + 0x0C
#define		TR_CTRL_15_ADDR 		TR_BASE_ADDR + 0x0D
#define		TR_CTRL_16_ADDR 		TR_BASE_ADDR + 0x0E
#define		TR_CTRL_17_ADDR 		TR_BASE_ADDR + 0x0F
#define		TR_STAT_1_ADDR 			TR_BASE_ADDR + 0x0A
#define		TR_STAT_2_ADDR 			TR_BASE_ADDR + 0x0B  
//above addr belong to timing recovery

#define		CR_CTRL_3_ADDR			CR_BASE_ADDR + 0x00
#define		CR_CTRL_4_ADDR			CR_BASE_ADDR + 0x01
#define		CR_CTRL_16_ADDR 		CR_BASE_ADDR + 0x02
#define		CR_CTRL_17_ADDR 		CR_BASE_ADDR + 0x03
#define		CR_CTRL_20_ADDR 		CR_BASE_ADDR + 0x04
#define		CR_CTRL_21_ADDR 		CR_BASE_ADDR + 0x05
#define		CR_CTRL_22_ADDR 		CR_BASE_ADDR + 0x06
#define		CR_CTRL_23_ADDR 		CR_BASE_ADDR + 0x07
#define		CR_CTRL_24_ADDR 		CR_BASE_ADDR + 0x08
#define		CR_CTRL_25_ADDR 		CR_BASE_ADDR + 0x09
#define		CR_CTRL_26_ADDR 		CR_BASE_ADDR + 0x0A
#define		CR_CTRL_27_ADDR 		CR_BASE_ADDR + 0x0B
#define		CR_CTRL_28_ADDR 		CR_BASE_ADDR + 0x0C
#define		CR_CTRL_29_ADDR 		CR_BASE_ADDR + 0x0D
#define		CR_CTRL_30_ADDR 		CR_BASE_ADDR + 0x0E
#define		CR_CTRL_31_ADDR 		CR_BASE_ADDR + 0x0F
#define		CR_STAT_2_ADDR 			CR_BASE_ADDR + 0x10
#define		CR_STAT_3_ADDR 			CR_BASE_ADDR + 0x11
#define		CR_STAT_4_ADDR 			CR_BASE_ADDR + 0x12
#define		CR_STAT_5_ADDR 			CR_BASE_ADDR + 0x13
#define		CR_STAT_6_ADDR 			CR_BASE_ADDR + 0x14
#define		CR_STAT_7_ADDR 			CR_BASE_ADDR + 0x15 
//above addr belong to carriar recovery 

#define		MCTRL_1_ADDR	      MC_BASE_ADDR + 0x00
#define		MCTRL_2_ADDR	      MC_BASE_ADDR + 0x01
#define		MCTRL_3_ADDR	      MC_BASE_ADDR + 0x02
#define          MCTRL_4_ADDR 	      (MC_BASE_ADDR + 0x03)
#define		MCTRL_5_ADDR	      MC_BASE_ADDR + 0x04
#define		MCTRL_6_ADDR	      MC_BASE_ADDR + 0x05
#define		MCTRL_7_ADDR	      MC_BASE_ADDR + 0x06
#define		MCTRL_8_ADDR	      MC_BASE_ADDR + 0x07
#define		MCTRL_9_ADDR	      MC_BASE_ADDR + 0x08
#define		MCTRL_11_ADDR 	    MC_BASE_ADDR + 0x0A  
//above addr belong to MCTRL

#define		AGC_CTRL_1_ADDR			AGC_BASE_ADDR + 0x00
#define		AGC_CTRL_2_ADDR			AGC_BASE_ADDR + 0x01
#define		AGC_CTRL_3_ADDR			AGC_BASE_ADDR + 0x02
#define		AGC_CTRL_4_ADDR			AGC_BASE_ADDR + 0x03
#define		AGC_CTRL_5_ADDR			AGC_BASE_ADDR + 0x04
#define		AGC_CTRL_6_ADDR			AGC_BASE_ADDR + 0x05
#define		AGC_CTRL_9_ADDR			AGC_BASE_ADDR + 0x06
#define		AGC_CTRL_11_ADDR 		AGC_BASE_ADDR + 0x07
#define		AGC_CTRL_13_ADDR 		AGC_BASE_ADDR + 0x08
#define		AGC_CTRL_15_ADDR 		AGC_BASE_ADDR + 0x09
#define		AGC_CTRL_17_ADDR 		AGC_BASE_ADDR + 0x0A
#define		AGC_CTRL_18_ADDR 		AGC_BASE_ADDR + 0x0B
#define		AGC_CTRL_19_ADDR 		AGC_BASE_ADDR + 0x0C
#define		AGC_CTRL_20_ADDR 		AGC_BASE_ADDR + 0x0D
#define		AGC_CTRL_21_ADDR 		AGC_BASE_ADDR + 0x0E 
#define		AGC_CTRL_22_ADDR 		AGC_BASE_ADDR + 0x0F 
//above addr belong to AGC

#define		BAGC_CTRL_1_ADDR		BAGC_BASE_ADDR + 0x00
#define		BAGC_CTRL_2_ADDR		BAGC_BASE_ADDR + 0x01
#define		BAGC_CTRL_3_ADDR		BAGC_BASE_ADDR + 0x02
#define		BAGC_CTRL_4_ADDR		BAGC_BASE_ADDR + 0x03
#define		BAGC_CTRL_5_ADDR		BAGC_BASE_ADDR + 0x04
#define		BAGC_CTRL_6_ADDR		BAGC_BASE_ADDR + 0x05
#define		BAGC_CTRL_7_ADDR		BAGC_BASE_ADDR + 0x06
#define		BAGC_CTRL_8_ADDR 		BAGC_BASE_ADDR + 0x07
#define		BAGC_CTRL_9_ADDR 		BAGC_BASE_ADDR + 0x08
#define		BAGC_CTRL_10_ADDR 	BAGC_BASE_ADDR + 0x09
#define		BAGC_CTRL_12_ADDR		BAGC_BASE_ADDR + 0x0B
#define		BAGC_CTRL_13_ADDR		BAGC_BASE_ADDR + 0x0E
#define		BAGC_CTRL_14_ADDR		BAGC_BASE_ADDR + 0x0F
#define		BAGC_STAT_1_ADDR		BAGC_BASE_ADDR + 0x10
#define		BAGC_STAT_2_ADDR		BAGC_BASE_ADDR + 0x11
#define		BAGC_STAT_3_ADDR		BAGC_BASE_ADDR + 0x12
#define		BAGC_STAT_4_ADDR		BAGC_BASE_ADDR + 0x13
#define		BAGC_STAT_5_ADDR		BAGC_BASE_ADDR + 0x14
#define		BAGC_STAT_6_ADDR		BAGC_BASE_ADDR + 0x15
#define		BAGC_STAT_7_ADDR		BAGC_BASE_ADDR + 0x19
#define		BAGC_CTRL_15_ADDR		BAGC_BASE_ADDR + 0x16
#define		BAGC_CTRL_16_ADDR		BAGC_BASE_ADDR + 0x17
#define		BAGC_CTRL_17_ADDR		BAGC_BASE_ADDR + 0x18
#define		BAGC_CTRL_18_ADDR		BAGC_BASE_ADDR + 0x1A  //7a
#define		BAGC_CTRL_19_ADDR		BAGC_BASE_ADDR + 0x1B  //7b
//above addr belong to AGC

#define		SYNC_CTRL_1_ADDR		DP_BASE_ADDR + 0x00
#define		SYNC_CTRL_3_ADDR		DP_BASE_ADDR + 0x02
#define		BER_1_ADDR	        DP_BASE_ADDR + 0x03
#define		BER_2_ADDR	        DP_BASE_ADDR + 0x04
#define		BER_3_ADDR	        DP_BASE_ADDR + 0x05
#define     BER_4_ADDR          DP_BASE_ADDR + 0x06
#define		TS_CTRL_1_ADDR			DP_BASE_ADDR + 0x08
#define		TS_CTRL_2_ADDR			DP_BASE_ADDR + 0x09
#define		TS_CTRL_3_ADDR			DP_BASE_ADDR + 0x0A
#define		RS_CTRL_1_ADDR			DP_BASE_ADDR + 0x0B
#define		RS_CTRL_2_ADDR			DP_BASE_ADDR + 0x0C
#define		RS_CTRL_3_ADDR			DP_BASE_ADDR + 0x0D
#define		RS_CTRL_4_ADDR			DP_BASE_ADDR + 0x0E
#define		RS_CTRL_5_ADDR			DP_BASE_ADDR + 0x0F
#define		RS_CTRL_6_ADDR			DP_BASE_ADDR + 0x10
#define		RS_CTRL_8_ADDR			DP_BASE_ADDR + 0x11 
//above addr belong to data process

#define		FS_CTRL_1_ADDR			QAM_BASE_ADDR + 0x01
#define		FS_CTRL_2_ADDR			QAM_BASE_ADDR + 0x02
#define		FS_CTRL_4_ADDR			QAM_BASE_ADDR + 0x04
#define		FS_CTRL_5_ADDR			QAM_BASE_ADDR + 0x05
#define		FS_CTRL_6_ADDR			QAM_BASE_ADDR + 0x06
#define		FS_CTRL_7_ADDR			QAM_BASE_ADDR + 0x07
#define		FS_STAT_1_ADDR			QAM_BASE_ADDR + 0x08
#define		FS_STAT_2_ADDR			QAM_BASE_ADDR + 0x09
//above addr belong to freq scan

#define		QAM_DEBUG_1_ADDR		QAM_DEBUG_ADDR + 0x00
#define		QAM_DEBUG_2_ADDR		QAM_DEBUG_ADDR + 0x01
#define		QAM_DEBUG_3_ADDR		QAM_DEBUG_ADDR + 0x02
#define		QAM_DEBUG_4_ADDR		QAM_DEBUG_ADDR + 0x03
#define		QAM_DEBUG_5_ADDR		QAM_DEBUG_ADDR + 0x04
#define		QAM_DEBUG_6_ADDR		QAM_DEBUG_ADDR + 0x05
#define		QAM_DEBUG_7_ADDR		QAM_DEBUG_ADDR + 0x06
//above addr belong to debug

#define		DAGC_CTRL_1_ADDR		DAGC_BASE_ADDR	+	0x00
#define		DAGC_CTRL_2_ADDR		DAGC_BASE_ADDR	+	0x01
#define		DAGC_CTRL_3_ADDR		DAGC_BASE_ADDR	+	0x02
#define		DAGC_CTRL_4_ADDR		DAGC_BASE_ADDR	+	0x03
#define		DAGC_CTRL_5_ADDR		DAGC_BASE_ADDR	+	0x04
#define		DAGC_CTRL_6_ADDR		DAGC_BASE_ADDR	+	0x05
#define		DAGC_CTRL_7_ADDR		DAGC_BASE_ADDR	+	0x06
#define		DAGC_CTRL_8_ADDR		DAGC_BASE_ADDR	+	0x07
#define		DAGC_CTRL_9_ADDR		DAGC_BASE_ADDR	+	0x08
#define		DAGC_CTRL_10_ADDR		DAGC_BASE_ADDR	+	0x09
#define		DAGC_STAT_1_ADDR		DAGC_BASE_ADDR	+	0x0E
#define		DAGC_STAT_2_ADDR		DAGC_BASE_ADDR	+	0x0F /*c47559 add DAGC_STAT_2 reg's defination (07/04/03) */
//above addr belong to DAGC

#define		BS_CTRL_1_ADDR			DP_BASE_ADDR + 0x16 
#define		BS_CTRL_2_ADDR			DP_BASE_ADDR + 0x17 
#define		BS_CTRL_3_ADDR			DP_BASE_ADDR + 0x18 
#define		BS_CTRL_4_ADDR			DP_BASE_ADDR + 0x19 
#define		BS_CTRL_5_ADDR			DP_BASE_ADDR + 0x1A 
#define		BS_CTRL_6_ADDR			DP_BASE_ADDR + 0x1B 
#define		BS_STAT_1_ADDR			DP_BASE_ADDR + 0x1C 
#define		BS_STAT_2_ADDR			DP_BASE_ADDR + 0x1D 
#define		BS_STAT_3_ADDR			DP_BASE_ADDR + 0x1E 
#define		BS_STAT_4_ADDR			DP_BASE_ADDR + 0x1F
#define		BS_STAT_5_ADDR			DP_BASE_ADDR + 0x20
//above addr belong to sweep scan 

#define		BS_PARA_1_ADDR			DAGC_BASE_ADDR	+	0x0A
#define		BS_PARA_2_ADDR			DAGC_BASE_ADDR	+	0x0B
#define		BS_PARA_3_ADDR			DAGC_BASE_ADDR	+	0x0C
#define		BS_PARA_4_ADDR			DAGC_BASE_ADDR	+	0x0D
/***************************add end***********************************/

/*******************add by huyupeng 2010.05.19 for X5HD***************/
#define     DEPHASE_BASE_ADDR			    0xe0
#define     DEPHASE_CTRL_ADDR		    	DEPHASE_BASE_ADDR + 0x0a
#define     FOUR_REG_SEL_ADDR               DEPHASE_BASE_ADDR + 0x00       //20130723                                                      
#define     DEPHASE_GAIN_K_HI_ADDR			DEPHASE_BASE_ADDR + 0x01
#define     DEPHASE_GAIN_K_LO_ADDR			DEPHASE_BASE_ADDR + 0x02
#define     DEPHASE_STA_NUM_BASE_HI_ADDR	DEPHASE_BASE_ADDR + 0x03
#define     DEPHASE_STA_NUM_BASE_LO_ADDR	DEPHASE_BASE_ADDR + 0x04
#define     DEPHASE_SNR_THRES_HI_ADDR		DEPHASE_BASE_ADDR + 0x05
#define     DEPHASE_SNR_THRES_LO_ADDR		DEPHASE_BASE_ADDR + 0x06
#define     DEPHASE_SELECT_KL_THRES_ADDR	DEPHASE_BASE_ADDR + 0x07
#define     DEPHASE_SELECT_EQ_THRES_ADDR	DEPHASE_BASE_ADDR + 0x08
#define     DEPHASE_STATE4WORK_DELAY_ADDR	DEPHASE_BASE_ADDR + 0x09

#define     CR_STATE1COUNT_ADDR				DEPHASE_BASE_ADDR + 0x0b
#define     CR_STATE2COUNT_ADDR				DEPHASE_BASE_ADDR + 0x0c
#define     CR_STATE3COUNT_ADDR				DEPHASE_BASE_ADDR + 0x0d
#define     CR_STATE4COUNT_ADDR				DEPHASE_BASE_ADDR + 0x0e
#define     CR_STATE5COUNT_ADDR				DEPHASE_BASE_ADDR + 0x0f
#define     CR_STATE6COUNT_ADDR				DEPHASE_BASE_ADDR + 0x10

#define     CR_LOST_UP_THRES_HI_ADDR		DEPHASE_BASE_ADDR + 0x11
#define     CR_LOST_UP_THRES_LO_ADDR		DEPHASE_BASE_ADDR + 0x12
#define     CR_LOST_DOWN_THRES_HI_ADDR		DEPHASE_BASE_ADDR + 0x13
#define     CR_LOST_DOWN_THRES_LO_ADDR		DEPHASE_BASE_ADDR + 0x14

#define     CR_KPROP_SCALE12_ADDR			DEPHASE_BASE_ADDR + 0x15
#define     CR_KPROP_SCALE34_ADDR			DEPHASE_BASE_ADDR + 0x16
#define     CR_KINT_SCALE12_ADDR			DEPHASE_BASE_ADDR + 0x17
#define     CR_KINT_SCALE34_ADDR			DEPHASE_BASE_ADDR + 0x18

#define     SFREQ_CENTER_SEL_ADDR			DEPHASE_BASE_ADDR + 0x19
#define     SFREQ_N6_ADDR					DEPHASE_BASE_ADDR + 0x1a
#define     SFREQ_MASK_ADDR					DEPHASE_BASE_ADDR + 0x1b
#define     SFREQ_STEP_ADDR					DEPHASE_BASE_ADDR + 0x1c
#define     SFREQ_COEFFTHRES_ADDR			DEPHASE_BASE_ADDR + 0x1d
#define     SFREQ_REMOVE_AGC_SCALE_ADDR		DEPHASE_BASE_ADDR + 0x1e
#define     SFREQ_REMOVE_AGC_COEF_ADDR		DEPHASE_BASE_ADDR + 0x1f

#define     SFREQ_ERR_BASE_ADDR				0x80
#define     SFREQ_ERR_CTRL_1				SFREQ_ERR_BASE_ADDR + 0x00
#define     SFREQ_ERR_CTRL_2				SFREQ_ERR_BASE_ADDR + 0x01
#define     SFREQ_ERR_CTRL_3				SFREQ_ERR_BASE_ADDR + 0x02
#define     SFREQ_FIR_SEL_ADDR				SFREQ_ERR_BASE_ADDR + 0x03   // [5] CR Lost Count En [4:0] select tap coef
#define     SFREQ_FIR_READ_ADDR				SFREQ_ERR_BASE_ADDR + 0x04   // Coeff value
#define     SFREQ_CR_LOST_COUNT_ADDR		SFREQ_ERR_BASE_ADDR + 0x05   // CR Lost Counter
#define     SFREQ_ERR_SCALE_ADDR            SFREQ_ERR_BASE_ADDR + 0x06   // [7:4] SCALE1 [3:0] SCALE2 

#define     SFREQ_SCALE55_ADDR SFREQ_SCALE77_ADDR
#define     SFREQ_AGC1_INIT_ADDR            SFREQ_ERR_BASE_ADDR + 0x07
#define     SFREQ_AGC_BIT_SELECT_ADDR       SFREQ_ERR_BASE_ADDR + 0x08
#define     SFREQ_SCALE77_ADDR              SFREQ_ERR_BASE_ADDR + 0x09  //20130305
#define     SFREQ_PHASE_LARGE_ADDR          SFREQ_ERR_BASE_ADDR + 0x0a
#define     SFREQ_PHASE_SMALL_ADDR          SFREQ_ERR_BASE_ADDR + 0x0b
#define     SFREQ_HOLD_COUNT_ADDR           SFREQ_ERR_BASE_ADDR + 0x0c
#define     SFREQ_STA_COUNT_ADDR            SFREQ_ERR_BASE_ADDR + 0x0d
#define     SFREQ_SCALE88_ADDR              SFREQ_ERR_BASE_ADDR + 0x0e  //20130305
#define     SFREQ_FREQ_JIT_ADDR             SFREQ_ERR_BASE_ADDR + 0x0f
#define     SFREQ_COUNT_OUT_1_ADDR          DP_BASE_ADDR + 0x12
#define     SFREQ_COUNT_OUT_2_ADDR          DP_BASE_ADDR + 0x13
#define     SFREQ_COUNT_OUT_3_ADDR          DP_BASE_ADDR + 0x14
#define     SFREQ_TR_GAIN_ADDR              DP_BASE_ADDR + 0x15

/*under addr belong to j83b*/
#define     J83B_MCTRL_0_ADDR            (MC_BASE_ADDR + 0x0b)
#define     J83B_MCTRL_1_ADDR            (MC_BASE_ADDR + 0x0C)
#define     J83B_MCTRL_2_ADDR            (MC_BASE_ADDR + 0x0D)
#define     J83B_MCTRL_3_ADDR            (MC_BASE_ADDR + 0x0E)
#define     J83B_MCTRL_4_ADDR            (MC_BASE_ADDR + 0x0F)

#define     J83B_TCM_1_ADDR              (CR_BASE_ADDR + 0x16)
#define     J83B_TCM_2_ADDR              (CR_BASE_ADDR + 0x17)
#define     J83B_TCM_3_ADDR              (CR_BASE_ADDR + 0x18)
#define     J83B_TCM_4_ADDR              (CR_BASE_ADDR + 0x19)
#define     J83B_TCM_5_ADDR              (CR_BASE_ADDR + 0x1A)
#define     J83B_TCM_6_ADDR              (CR_BASE_ADDR + 0x1B)

#define     J83B_DI_1_ADDR               (CR_BASE_ADDR + 0x1C)
#define     J83B_DI_2_ADDR               (CR_BASE_ADDR + 0x1D)

#define     J83B_MFSYNC_1_ADDR           (CR_BASE_ADDR + 0x1E)
#define     J83B_MFSYNC_2_ADDR           (CR_BASE_ADDR + 0x1F)

#define     J83B_TCM_BER0_ADDR           (BAGC_BASE_ADDR + 0x1C)
#define     J83B_TCM_BER1_ADDR           (BAGC_BASE_ADDR + 0x1D)
#define     J83B_TCM_BER2_ADDR           (BAGC_BASE_ADDR + 0x1E)
#define     J83B_TCM_BER3_ADDR           (BAGC_BASE_ADDR + 0x1F)

#define     J83B_FFSYNC_1_ADDR           (QAM_BASE_ADDR + 0x0A)
#define     J83B_FFSYNC_2_ADDR           (QAM_BASE_ADDR + 0x0B)
#define     J83B_FFSYNC_3_ADDR           (QAM_BASE_ADDR + 0x0C)

#define     J83B_DR_1_ADDR				(QAM_BASE_ADDR + 0x0D)
#define     J83B_DR_2_ADDR				(QAM_BASE_ADDR + 0x0E)
#define     J83B_DR_3_ADDR				(QAM_BASE_ADDR + 0x0F)

#define		J83B_DEBUG_0_ADDR			(QAM_DEBUG_ADDR + 0x07)

#define INVALID_QAM_ADDR 0x00	
#define QAM_REL_REG_NUM_MAX		200	/* register number whose value related with qam type: hi3130, x5hdqam eg */
#define TUNER_REL_REG_NUM_MAX	20	/* register number whose value related with tuner type: cd1616, xg3bl eg */
#define QAM_RF_MIN 45000  //kHz
#define QAM_RF_MAX 870000 //kHz

#define TER_RF_MIN 48000  //kHz
#define TER_RF_MAX 870000 //kHz

#define TER_BW_MIN 6  //MHz
#define TER_BW_MAX 8 //MHz
#define HI3130_IF_DVB_DEF 36130000
#define HI3130_IF_MCNS_DEF 43750000

#define     PI 3.14159265

#define HI_TUNER_QAM_TYPE_MAX 5

#define QAM_AD_INSIDE	0	/* for chip */
#define	QAM_AD_OUTSIDE	1	/* for fpga */

#define	QAM_AGC_CMOS_OUTPUT	0	/* CMOS output */
#define QAM_AGC_OD_OUTPUT	1	/* OD output */

typedef enum
{
    DEMODULATION_MODULE = 0,
    DECODE_MODULE,
    RS_MODULE,
    EQU_MODULE,
    AGC_ADC_MODULE,
    ALL_CHIP
} qam_module_e;

typedef struct  hiREG_VALUE_S 
{
    HI_U8  u8Addr;
    HI_U8  u8Value; 
}REGVALUE_S, *PTR_REGVALUE_S;

typedef struct  tuner_reg_name_S 
{
    HI_U8  addr;
    char    *name; 
}reg_name_S;

//extern HI_S32 tuner_chip_reset(HI_U32 u32ResetGpioNo);

extern HI_S32 qam_read_byte(HI_U32 u32TunerPort, HI_U8 u8RegAddr, HI_U8 *pu8RegVal);
extern HI_S32 qam_read_bit(HI_U32 u32TunerPort, HI_U8 u8RegAddr, HI_U8 u8BitNum, HI_U8 *pu8BitVal);
extern HI_S32 qam_write_byte(HI_U32 u32TunerPort, HI_U8 u8RegAddr, HI_U8 u8RegVal);
extern HI_S32 qam_write_bit(HI_U32 u32TunerPort, HI_U8 u8RegAddr, HI_U8 u8BitNum, HI_U8 u8BitVal);
extern HI_VOID qam_config_i2c_out(HI_U32 u32TunerPort, HI_BOOL bTuner);

/*for v200*/
extern HI_S32 x5hdqamv200_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel);
extern HI_S32 x5hdqamv200_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32 x5hdqamv200_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
extern HI_S32 x5hdqamv200_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
extern HI_S32 x5hdqamv200_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32 x5hdqamv200_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 x5hdqamv200_config_tuner(HI_U32 u32TunerPort, HI_U32 dbRFfreq, HI_S32 times);
extern HI_S32 x5hdqamv200_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb );
extern HI_S32 x5hdqamv200_get_rs(HI_U32 u32TunerPort, HI_U32 *pu32Rs);
extern HI_VOID x5hdqamv200_test_single_agc( HI_U32 u32TunerPort, AGC_TEST_S * pstAgcTest );
extern HI_VOID x5hdqamv200_manage_after_chipreset(HI_U32 u32TunerPort);
extern HI_VOID x5hdqamv200_get_registers(HI_U32 u32TunerPort, void *p);
extern HI_VOID x5hdqamv200_connect_timeout(HI_U32 u32ConnectTimeout);

/*for hi3716cv200*/
extern HI_S32 hi3716cv200_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel);
extern HI_S32 hi3716cv200_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32 hi3716cv200_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
extern HI_S32 hi3716cv200_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
extern HI_S32 hi3716cv200_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32 hi3716cv200_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 hi3716cv200_config_tuner(HI_U32 u32TunerPort, HI_U32 dbRFfreq, HI_S32 times);
extern HI_S32 hi3716cv200_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb );
extern HI_S32 hi3716cv200_get_rs(HI_U32 u32TunerPort, HI_U32 *pu32Rs);
extern HI_VOID hi3716cv200_test_single_agc( HI_U32 u32TunerPort, AGC_TEST_S * pstAgcTest );
extern HI_VOID hi3716cv200_manage_after_chipreset(HI_U32 u32TunerPort);
extern HI_VOID hi3716cv200_get_registers(HI_U32 u32TunerPort, void *p);
extern HI_VOID hi3716cv200_connect_timeout(HI_U32 u32ConnectTimeout);


/*for x5hd*/
extern HI_S32 x5hdqam_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S* pstChannel);
extern HI_S32 x5hdqam_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32 x5hdqam_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
extern HI_S32 x5hdqam_get_ber(HI_U32 u32TunerPort, HI_U32* pu32Ber);
extern HI_S32 x5hdqam_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32 x5hdqam_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 x5hdqam_config_tuner(HI_U32 u32TunerPort, HI_U32 dbRFfreq, HI_S32 times);
extern HI_S32 x5hdqam_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb );
extern HI_S32 x5hdqam_get_rs(HI_U32 u32TunerPort, HI_U32 *pu32Rs);
extern HI_VOID x5hdqam_test_single_agc( HI_U32 u32TunerPort, AGC_TEST_S * pstAgcTest );
extern HI_VOID x5hdqam_manage_after_chipreset(HI_U32 u32TunerPort);
extern HI_VOID x5hdqam_get_registers(HI_U32 u32TunerPort, void *p);
extern HI_VOID x5hdqam_connect_timeout(HI_U32 u32ConnectTimeout);

/*for hi3130*/
extern HI_S32 hi3130_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S* pstChannel);
extern HI_S32 hi3130_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32 hi3130_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
extern HI_S32 hi3130_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
extern HI_S32 hi3130_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32 hi3130_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 hi3130_get_rs(HI_U32 u32TunerPort, HI_U32 *pu32Rs);
extern HI_S32 hi3130_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb );
extern HI_S32 hi3130_config_tuner(HI_U32 u32TunerPort, HI_U32 dbRFfreq, HI_S32 times);
extern HI_VOID hi3130_test_single_agc( HI_U32 u32TunerPort, AGC_TEST_S * pstAgcTest );
extern HI_VOID hi3130_manage_after_chipreset(HI_U32 u32TunerPort);
extern HI_VOID hi3130_get_registers(HI_U32 u32TunerPort, void *p);
extern HI_VOID hi3130_connect_timeout(HI_U32 u32ConnectTimeout);

/*for j83b*/
extern HI_S32 j83b_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel);
extern HI_S32 j83b_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32 j83b_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
extern HI_S32 j83b_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
extern HI_S32 j83b_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32 j83b_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 j83b_config_tuner(HI_U32 u32TunerPort, HI_U32 dbRFfreq, HI_S32 times);
extern HI_S32 j83b_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb );
extern HI_S32 j83b_get_rs(HI_U32 u32TunerPort, HI_U32 *pu32Rs);
extern HI_VOID j83b_test_single_agc( HI_U32 u32TunerPort, AGC_TEST_S * pstAgcTest );
extern HI_VOID j83b_manage_after_chipreset(HI_U32 u32TunerPort);
extern HI_VOID j83b_get_registers(HI_U32 u32TunerPort, void *p);
extern HI_VOID j83b_connect_timeout(HI_U32 u32ConnectTimeout);

/* for av6211 */
extern HI_S32 avl6211_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType);
extern HI_S32 avl6211_set_sat_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr);
extern HI_S32 avl6211_deInit(HI_U32 u32TunerPort);
extern HI_S32 avl6211_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S * pstChannel);
extern HI_S32 avl6211_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32 avl6211_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
extern HI_S32 avl6211_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32 avl6211_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength);
extern HI_S32 avl6211_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb);
extern HI_S32 avl6211_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo);
extern HI_S32 avl6211_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 avl6211_blindscan_init(HI_U32 u32TunerPort, TUNER_BLINDSCAN_INITPARA_S *pstPara);
extern HI_S32 avl6211_blindscan_action(HI_U32 u32TunerPort, TUNER_BLINDSCAN_PARA_S *pstPara);
extern HI_S32 avl6211_lnbctrl_power(HI_U32 u32TunerPort, HI_U8 u8Power);
extern HI_S32 avl6211_set_lnb_out(HI_U32 u32TunerPort, TUNER_LNB_OUT_E enOut);
extern HI_S32 avl6211_send_continuous_22K(HI_U32 u32TunerPort, HI_U32 u32Continuous22K);
extern HI_S32 avl6211_send_tone(HI_U32 u32TunerPort, HI_U32 u32Tone);
extern HI_S32 avl6211_DiSEqC_send_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_SENDMSG_S *pstSendMsg);
extern HI_S32 avl6211_DiSEqC_recv_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_RECVMSG_S *pstRecvMsg);
extern HI_S32 avl6211_standby(HI_U32 u32TunerPort, HI_U32 u32Standby);
extern HI_S32 avl6211_disable(HI_U32 u32TunerPort, HI_U32 u32Disable);
extern HI_S32 avl6211_set_funcmode(HI_U32 u32TunerPort, TUNER_FUNC_MODE_E enFuncMode);
/*for mxl101*/
extern HI_S32 mxl101_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType);
extern HI_S32 mxl101_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel);
extern HI_S32 mxl101_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32 mxl101_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
extern HI_S32 mxl101_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32 mxl101_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength);
extern HI_S32 mxl101_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 mxl101_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo);
extern HI_S32 mxl101_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb);

/*for mn88472*/
extern HI_S32 MN88472_Init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType);
extern HI_S32 MN88472_Connect(HI_U32 TunerID,TUNER_ACC_QAM_PARAMS_S *pstChannel);
extern HI_S32 MN88472_GetStatus (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32 MN88472_SetTsType(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 MN88472_GetBer(HI_U32 u32TunerPort, HI_U32 *pu32ber);
extern HI_S32 MN88472_GetSnr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32 MN88472_GetSignalStrength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
extern HI_S32 MN88472_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo);
extern HI_S32 MN88472_setplpid(HI_U32 u32TunerPort, HI_U8 u8PLPID);
extern HI_S32 MN88472_get_plp_num(HI_U32 u32TunerPort, HI_U8 *pu8PLPNum);
extern HI_S32 MN88472_get_current_plp_type(HI_U32 u32TunerPort, HI_UNF_TUNER_T2_PLP_TYPE_E *penPLPType);
extern HI_S32 MN88472_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb);
extern HI_VOID MN88472_connect_timeout(HI_U32 u32ConnectTimeout);

/*for it9170*/
extern HI_S32 it9170_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType);
extern HI_S32 it9170_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel);
extern HI_S32 it9170_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32 it9170_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
extern HI_S32 it9170_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32 it9170_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength);
extern HI_S32 it9170_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 it9170_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo);
extern HI_S32 it9170_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb);
/*for it9133*/
extern HI_S32 it9133_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType);
extern HI_S32 it9133_connect(HI_U32 u32TunerPort,TUNER_ACC_QAM_PARAMS_S *pstChannel);
extern HI_S32 it9133_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32 it9133_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
extern HI_S32 it9133_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32 it9133_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 it9133_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength);
extern HI_S32 it9133_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo);
extern HI_S32 it9133_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb);

/*for hi3136*/
extern HI_S32 hi3136_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType);
extern HI_S32 hi3136_set_sat_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr);
extern HI_S32 hi3136_deInit(HI_U32 u32TunerPort);
extern HI_S32 hi3136_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S * pstChannel);
extern HI_S32 hi3136_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32 hi3136_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
extern HI_S32 hi3136_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32 hi3136_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength);
extern HI_S32 hi3136_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb);
extern HI_S32 hi3136_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo);
extern HI_S32 hi3136_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_S32 hi3136_blindscan_init(HI_U32 u32TunerPort, TUNER_BLINDSCAN_INITPARA_S *pstPara);
extern HI_S32 hi3136_blindscan_action(HI_U32 u32TunerPort, TUNER_BLINDSCAN_PARA_S *pstPara);
extern HI_S32 hi3136_lnbctrl_power(HI_U32 u32TunerPort, HI_U8 u8Power);
extern HI_S32 hi3136_set_lnb_out(HI_U32 u32TunerPort, TUNER_LNB_OUT_E enOut);
extern HI_S32 hi3136_send_continuous_22K(HI_U32 u32TunerPort, HI_U32 u32Continuous22K);
extern HI_S32 hi3136_send_tone(HI_U32 u32TunerPort, HI_U32 u32Tone);
extern HI_S32 hi3136_DiSEqC_send_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_SENDMSG_S *pstSendMsg);
extern HI_S32 hi3136_DiSEqC_recv_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_RECVMSG_S *pstRecvMsg);
extern HI_S32 hi3136_standby(HI_U32 u32TunerPort, HI_U32 u32Standby);
extern HI_S32 hi3136_disable(HI_U32 u32TunerPort, HI_U32 u32Disable);
extern HI_S32 hi3136_set_funcmode(HI_U32 u32TunerPort, TUNER_FUNC_MODE_E enFuncMode);
extern HI_S32 hi3136_tp_verify(HI_U32 u32TunerPort, TUNER_TP_VERIFY_PARAMS_S * pstChannel);
extern HI_S32 hi3136_set_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut);

extern HI_S32   mxl254_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType);
extern HI_S32   mxl254_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel);
extern HI_S32   mxl254_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR);
extern HI_S32   mxl254_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
extern HI_VOID  mxl254_connect_timeout(HI_U32 u32ConnectTimeout);
extern HI_S32   mxl254_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus);
extern HI_S32   mxl254_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber);
extern HI_S32   mxl254_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength);
extern HI_S32   mxl254_get_powerspecdata(HI_U32 u32TunerPort, HI_U32 u32freqStartInHz,HI_U32 u32freqStepInHz,HI_U32 u32numOfFreqSteps,HI_S16 *ps16powerData);

#endif
