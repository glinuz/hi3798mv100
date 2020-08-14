#ifndef __DRV_TUNER_IOCTL_H__
#define __DRV_TUNER_IOCTL_H__

/*#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <linux/miscdevice.h>
#include <asm/io.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/pm.h>*/

#include "hi_type.h"
#include "hi_drv_i2c.h"
#include "hi_drv_gpioi2c.h"
#include "hi_debug.h"
#include "hi_drv_struct.h"
#include "drv_i2c_ext.h"
#include "drv_gpioi2c_ext.h"
#include "drv_gpio_ext.h"
#include "hi_drv_tuner.h"
#include "hi_unf_frontend.h"

//#include "common_dev.h"
//#include "common_proc.h"
//#include "common_mem.h"
//#include "common_stat.h"

#define QAM_PORT0_ADDR 0xA0
#define QAM_PORT1_ADDR 0xA2
#define QAM_PORT2_ADDR 0xA0
#define QAM_PORT3_ADDR 0xA2
#define QAM_PORT4_ADDR 0xA0
#define TUNER_PORT0_ADDR 0xC0
#define TUNER_PORT1_ADDR 0xC0
#define TUNER_PORT2_ADDR 0xC0
#define TUNER_PORT3_ADDR 0xC0
#define TUNER_PORT4_ADDR 0xC0
#define XTAL_CLK 28800

#define TUNER_NUM 5

typedef enum hiTUNER_DATA_SRC_E
{
    TUNER_DATA_SRC_ADC = 0,
    TUNER_DATA_SRC_EQU,
    TUNER_DATA_SRC_CIR_PLOT,
    TUNER_DATA_SRC_BUTT
}TUNER_DATA_SRC_E;

typedef enum hiTUNER_QAM_TYPE_E
{
    QAM_TYPE_16 = 0,
    QAM_TYPE_32,
    QAM_TYPE_64,
    QAM_TYPE_128,
    QAM_TYPE_256
}TUNER_QAM_TYPE_E;

typedef struct hiTUNER_ACC_QAM_PARAMS_S
{
    HI_U32 u32Frequency;            /* frequency kHz*/
    union
    {
        HI_U32 u32SymbolRate;           /* symbolrate Hz*/
        HI_U32 u32BandWidth;           /* BandWidth KHz*/
    } unSRBW;
	HI_U32 u23ScrambleCode;
    TUNER_QAM_TYPE_E enQamType;
    HI_BOOL bSI;              /*reverse*/
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar; 
	union
	{
		HI_UNF_TUNER_TER_ACC_S enDVBT2;
		HI_UNF_TUNER_TS_PRIORITY_E enDVBT;
	}unTer;	
	HI_U8 u8DVBTMode;   //0  dvb-t2;1  dvb-t
}TUNER_ACC_QAM_PARAMS_S;

typedef struct  hiTUNER_DATA_S
{
    HI_U32        u32Port;
    HI_U32        u32Data;
}TUNER_DATA_S, *PTR_TUNER_DATA_S;

typedef struct  hiTUNER_DATABUF_S
{
    HI_U32         u32Port;
    HI_U32         u32DataBuf[3];
}TUNER_DATABUF_S, *PTR_TUNER_DATABUF_S;

typedef struct hiTUNER_ATTR_S
{
    HI_U32              u32Port;
    HI_UNF_TUNER_ATTR_S stAttr;
} TUNER_ATTR_S,*PTR_TUNER_ATTR_S;

typedef struct  hiTUNER_TSOUTSET_S
{
    HI_U32        u32Port;
    HI_UNF_TUNER_TSOUT_SET_S    stTSOutSetAttr;
}TUNER_TSOUTSET_S, *PTR_TUNER_TSOUTSET_S;

typedef struct  hiTUNER_SETSATATTR_S
{
    HI_U32        u32Port;
    HI_UNF_TUNER_SAT_ATTR_S    stSetSatAttr;
}TUNER_SETSATATTR_S, *PTR_TUNER_SETSATATTR_S;

typedef struct  hiTUNER_SETCABATTR_S
{
    HI_U32        u32Port;
    HI_UNF_TUNER_CAB_ATTR_S    stSetCatAttr;
}TUNER_SETCABATTR_S, *PTR_TUNER_SETCABATTR_S;

typedef struct  hiTUNER_SETTERATTR_S
{
    HI_U32        u32Port;
    HI_UNF_TUNER_TER_ATTR_S    stSetTerAttr;
}TUNER_SETTERATTR_S, *PTR_TUNER_SETTERATTR_S;

typedef struct  hiTUNER_ISDBTLAYER_S
{
    HI_U32        u32Port;
    HI_UNF_TUNER_MONITOR_LAYERS_CONFIG_S    stISDBTLayerCfg;
}TUNER_ISDBTLAYER_S, *PTR_TUNER_ISDBTLAYER_S;

typedef struct hiTUNER_SAMPLE_DATA_PARAM_S
{
    TUNER_DATA_SRC_E enDataSrc;
    HI_U32 u32DataLen;
    HI_UNF_TUNER_SAMPLE_DATA_S *pstData;
}TUNER_SAMPLE_DATA_PARAM_S;

typedef struct  hiTUNER_SAMPLEDATA_S
{
    HI_U32        u32Port;
    TUNER_SAMPLE_DATA_PARAM_S    stSampleData;
}TUNER_SAMPLEDATA_S, *PTR_TUNER_SAMPLEDATA_S;


typedef struct hiTUNER_POWER_SPECTRUM_DATA_PARAM_S
{
   HI_U32 u32freqStartInHz;
   HI_U32 u32freqStepInHz;
   HI_U32 u32numOfFreqSteps;
   HI_S16 *ps16powerData;
   
}TUNER_POWER_SPECTRUM_DATA_PARAM_S;

typedef struct  hiTUNER_SPECTRUMDATA_S
{
    HI_U32        u32Port;
    TUNER_POWER_SPECTRUM_DATA_PARAM_S    stSpectrumData;
}TUNER_SPECTRUMDATA_S, *PTR_TUNER_SPECTRUMDATA_S;

typedef struct  hiTUNER_SIGNALINFO_S
{
    HI_U32         u32Port;
    HI_UNF_TUNER_SIGNALINFO_S stInfo;
}TUNER_SIGNALINFO_S, *PTR_TUNER_SIGNALINFO_S;

typedef struct  hiTUNER_LNBCONFIG_S
{
    HI_U32         u32Port;
    HI_UNF_TUNER_FE_LNB_CONFIG_S stLNB;
}TUNER_LNBCONFIG_S, *PTR_TUNER_LNBCONFIG_S;

#define MAX_TP_NUM (32)

typedef struct  hiTUNER_BLINDSCAN_INITPARA_S
{
    HI_U32 u32Data;                 /*For extend*/
}TUNER_BLINDSCAN_INITPARA_S, *PTR_TUNER_BLINDSCAN_INITPARA_S;

typedef struct  hiTUNER_BLINDSCAN_S
{
    HI_U32         u32Port;
    TUNER_BLINDSCAN_INITPARA_S stPara;
}TUNER_BLINDSCAN_S, *PTR_TUNER_BLINDSCAN_S;

typedef struct  hiTUNER_BLINDSCAN_PARA_S
{
    HI_U32 u32CentreFreq;           /*Blind scan centre frequency, kHz*/
    HI_U16 u16Count;                /*Channel find in this scanning */
    union
    {
        HI_UNF_TUNER_SAT_TPINFO_S astSat[MAX_TP_NUM];
    }unResult;
}TUNER_BLINDSCAN_PARA_S, *PTR_TUNER_BLINDSCAN_PARA_S;

typedef struct hiTUNER_TER_SCAN_PARA_S
{
	HI_UNF_TUNER_TER_SCAN_ATTR_S stTer;
	HI_UNF_TUNER_TER_TPINFO_S enTpInfo;		/**<result of tp array*/                /**<CNcomment:扫描频点用数组形式存储*/
}TUNER_TER_SCAN_PARA_S;

#if 0
typedef struct  hiTUNER_BLINDSCAN_PARA_S
{
    HI_U32 u32CentreFreq;           /*Blind scan centre frequency, kHz*/
    HI_U16 u16Count;                /*Channel find in this scanning */
    HI_UNF_TUNER_SAT_TPINFO_S astSat[MAX_TP_NUM];
}TUNER_BLINDSCAN_PARA_S, *PTR_TUNER_BLINDSCAN_PARA_S;
#endif

typedef struct  hiTUNER_BLINDSCAN_INFO_S
{
    HI_U32         u32Port;
    TUNER_BLINDSCAN_PARA_S stPara;
}TUNER_BLINDSCAN_INFO_S, *PTR_TUNER_BLINDSCAN_INFO_S;

typedef struct  hiTUNER_TERSCAN_INFO_S
{
    HI_U32         u32Port;
    TUNER_TER_SCAN_PARA_S stScanPara;
}TUNER_TERSCAN_INFO_S, *PTR_TUNER_TERSCAN_INFO_S;

typedef enum  hiTUNER_SETFUNC_MODE_E
{
    FunctMode_Demod = 0,
    FunctMode_BlindScan = 1
}TUNER_FUNC_MODE_E;

/* LNB out voltage enum */
typedef enum hiTUNER_LNB_OUT_E
{
    TUNER_LNB_OUT_0V = 0,
    TUNER_LNB_OUT_13V = 13,
    TUNER_LNB_OUT_14V = 14, /* 14 or 15V */
    TUNER_LNB_OUT_18V = 18,
    TUNER_LNB_OUT_19V = 19, /* 19 or 20V */
    TUNER_LNB_OUT_BUTT
}TUNER_LNB_OUT_E;

/* LNB out voltage struct */
typedef struct  hiTUNER_LNB_OUT_S
{
    HI_U32          u32Port;
    TUNER_LNB_OUT_E enOut;
}TUNER_LNB_OUT_S, *PTR_TUNER_LNB_OUT_S;

/* DiSEqC send message */
typedef struct  hiTUNER_DISEQC_SENDMSG_S
{
    HI_U32         u32Port;
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
}TUNER_DISEQC_SENDMSG_S, *PTR_TUNER_DISEQC_SENDMSG_S;

typedef struct  hiTUNER_DISEQC_RECVMSG_S
{
    HI_U32         u32Port;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;  /* Read data */
}TUNER_DISEQC_RECVMSG_S, *PTR_TUNER_DISEQC_RECVMSG_S;

typedef struct hiTUNER_SET_TUNER_S
{
    HI_U32 u32Freq;
    HI_S32 s32Times;    
}TUNER_SET_TUNER_S, *PTR_TUNER_SET_TUNER_S;


typedef struct  hiTUNER_SIGNAL_S
{
    HI_UNF_TUNER_SIG_TYPE_E enSigType;
    HI_U32 u32Port;
    TUNER_ACC_QAM_PARAMS_S  stSignal;
}TUNER_SIGNAL_S, *PTR_TUNER_SIGNAL_S;

typedef struct hiTUNER_TP_VERIFY_PARAMS_S
{
    HI_U32 u32Frequency;            /**<frequency kHz*/                            /**<CNcomment:频率，单位kHz*/
    HI_U32 u32SymbolRate;           /**<symbolrate kBaud*/                         /**<CNcomment:符号率，单位kBaud*/
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar; /**<TP polarization*/                    /**<CNcomment:TP的极化方式*/
    HI_U8 cbs_reliablity;                           /**<TP reliability*/             /**<CNcomment:TP的可靠度*/
    HI_S32 CBS_TH;                                   /**<blindscan threshold*/        /**<CNcomment:盲扫阈值*/
    HI_U8 fs_grade;
    HI_U32 fec_ok_cnt;                             /**<TP number of fec ok*/        /**<CNcomment:fec ok的TP数量*/
    HI_U32 fec_no_ok_cnt;                          /**<TP number of fec  not ok*/   /**<CNcomment:fec not ok的TP数量*/
}TUNER_TP_VERIFY_PARAMS_S;

typedef struct  hiTUNER_TP_VERIFY_INFO_S
{
    HI_U32 u32Port;
    TUNER_TP_VERIFY_PARAMS_S  stTPVerifyPrm;
}TUNER_TP_VERIFY_INFO_S, *PTR_TUNER_TP_VERIFY_INFO_S;

typedef enum hiTUNER_QAMINSIDE_E
{
    TUNER_QAMINSIDE_IN = 0,
    TUNER_QAMINSIDE_OUT,
    TUNER_QAMINSIDE_INVALID,
}TUNER_QAMINSIDE_E;

typedef struct tag_TunerAttr_S
{
    HI_UNF_TUNER_SIG_TYPE_E     enSigType;
    HI_UNF_TUNER_DEV_TYPE_E     enTunerDevType;
    HI_U32                      u32TunerAddr;
} HI_TunerAttr_S;

typedef struct tag_DemodAttr_S
{
    HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType;     /**<QAM type*/                                /**<CNcomment:QAM类型*/
    HI_U32 u32DemodAddr;
	HI_U32 u32DemodXtalClk;
} HI_DemodAttr_S;

typedef struct hiTUNER_I2C_DATA_S
{       
    HI_U8* pu8SendBuf;
    HI_U32 u32SendLength;
    HI_U8* pu8ReceiveBuf;
    HI_U32 u32ReceiveLength;
} TUNER_I2C_DATA_S, *PTR_TUNER_I2C_DATA_S; 

typedef struct  hiAGC_TEST_S 
{
    HI_U32      u32Port;
    HI_U32		u32Agc1;
    HI_U32		u32Agc2;
    HI_BOOL     bLockFlag; 
    HI_BOOL		bAgcLockFlag;
    HI_U8 		u8BagcCtrl12;
	HI_U32		u32Count;
}AGC_TEST_S;

/* standard tuner operation */
typedef struct _TUNER_OPS_S
{
    HI_UNF_TUNER_SIG_TYPE_E enSigType;
    HI_U8 enI2cChannel;
    HI_UNF_TUNER_DEV_TYPE_E   enTunerDevType;
    HI_UNF_DEMOD_DEV_TYPE_E	  enDemodDevType;
	TUNER_ACC_QAM_PARAMS_S    stCurrPara;
	HI_UNF_TUNER_OUPUT_MODE_E      enTsType;
	
	HI_U32	u32DemodAddress;
	HI_U32	u32XtalClk;
	HI_U32  u32TunerAddress;
	HI_U32  u32CurrQamMode;
	HI_U8   u8AdcType;
	HI_U8   u8AgcOutputSel;
	HI_U8   u8AdcDataFmt;
	
    HI_S32 (*tuner_connect)(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S * pstChannel);
    HI_S32 (*tuner_get_status)(HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E * penTunerStatus);
    HI_S32 (*tuner_get_ber)(HI_U32 u32TunerPort, HI_U32 * pu32BERa);
    HI_S32 (*tuner_get_snr)(HI_U32 u32TunerPort, HI_U32 * pu32SignalStrength);
    HI_S32 (*tuner_get_signal_strength)(HI_U32 u32TunerPort, HI_U32 * pu32strength);
    HI_S32 (*tuner_get_signal_quality)(HI_U32 u32TunerPort, HI_U32 * pu32quality);
    HI_S32 (*tuner_set_ts_type)(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType);
    HI_S32 (*set_tuner)(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32RF);
	HI_S32 (*tuner_get_freq_symb_offset)(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb );
	HI_S32  (*tuner_get_rs)(HI_U32 u32TunerPort, HI_U32 *pu32Rs);
    HI_VOID (*manage_after_chipreset)(HI_U32 u32TunerPort);
    HI_VOID (*recalculate_signal_strength)(HI_U32 u32TunerPort, HI_U32 * pu32Strength);
    HI_VOID (*tuner_test_single_agc)( HI_U32 u32TunerPort, AGC_TEST_S * pstAgcTest );	/* just for test */
	HI_VOID (*tuner_resume)( HI_U32 u32TunerPort );
	HI_VOID (*tuner_get_registers)(HI_U32 u32TunerPort, void *p);
	HI_VOID (*tuner_connect_timeout)(HI_U32 u32ConnectTimeout);
    /* Added begin:l00185424 2011-11-28 For DVB-S/S2 */
    HI_S32 (*tuner_init)(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType);
    HI_S32 (*tuner_set_sat_attr)(HI_U32 u32TunerPort, HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr);
    HI_S32 (*tuner_set_ter_attr)(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ATTR_S *pstTerTunerAttr);
    HI_S32 (*tuner_set_cab_attr)(HI_U32 u32TunerPort, HI_UNF_TUNER_CAB_ATTR_S *pstTerTunerAttr);
    HI_S32 (*tuner_get_signal_info)(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo);
    HI_S32 (*tuner_blindscan_init)(HI_U32 u32TunerPort, TUNER_BLINDSCAN_INITPARA_S *pstPara);
    HI_S32 (*tuner_blindscan_action)(HI_U32 u32TunerPort, TUNER_BLINDSCAN_PARA_S *pstPara);
	HI_S32 (*tuner_terscan_action)(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_SCAN_ATTR_S *pstScanPara, HI_UNF_TUNER_TER_TPINFO_S *pstTpPara);
    HI_S32 (*tuner_lnbctrl_dev_init)(HI_U32 u32TunerPort, HI_U32 u32I2CNum, HI_U8 u8DevAddr, 
                            HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType);
    HI_S32 (*tuner_lnbctrl_dev_standby)(HI_U32 u32TunerPort, HI_U32 u32Standby);
    HI_S32 (*tuner_set_lnb_out)(HI_U32 u32TunerPort, TUNER_LNB_OUT_E enOut);
    HI_S32 (*tuner_send_continuous_22K)(HI_U32 u32TunerPort, HI_U32 u32Continuous22K);
    HI_S32 (*tuner_send_tone)(HI_U32 u32TunerPort, HI_U32 u32Tone);
    HI_S32 (*tuner_DiSEqC_send_msg)(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_SENDMSG_S *pstSendMsg);
    HI_S32 (*tuner_DiSEqC_recv_msg)(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_RECVMSG_S *pstRecvMsg);
    HI_S32 (*tuner_standby)(HI_U32 u32TunerPort, HI_U32 u32Standby);
    HI_S32 (*tuner_setfuncmode)(HI_U32 u32TunerPort, TUNER_FUNC_MODE_E enFuncMode);
    HI_S32 (*tuner_setplpid)(HI_U32 u32TunerPort, HI_U8 u8PLpId, HI_U32 u32Mode);
	HI_S32 (*tuner_set_common_plp_id)(HI_U32 u32TunerPort, HI_U8 u8PLpId);
	HI_S32 (*tuner_set_common_plp_combination)(HI_U32 u32TunerPort, HI_U8 u8ComPlpEna);
    HI_S32 (*tuner_get_plp_num)(HI_U32 u32TunerPort, HI_U8 *pu8PLPNum);
    HI_S32 (*tuner_get_current_plp_type)(HI_U32 u32TunerPort, HI_UNF_TUNER_T2_PLP_TYPE_E *pstPLPType);
	HI_S32 (*tuner_get_plp_id)(HI_U32 u32TunerPort, HI_U8 *pu8PLPId);
	HI_S32 (*tuner_get_group_plp_id)(HI_U32 u32TunerPort, HI_U8 *pu8PLPGrpId);
	HI_S32 (*tuner_set_antena_power)(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ANTENNA_POWER_E enPower);
    HI_S32 (*tuner_tp_verify)(HI_U32 u32TunerPort, TUNER_TP_VERIFY_PARAMS_S * pstChannel);
    HI_S32 (*tuner_set_ts_out)(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut);
    HI_S32 (*tuner_get_data_sample)(HI_U32 u32TunerPort, TUNER_DATA_SRC_E enDataSrc, HI_U32 u32DataLen, HI_UNF_TUNER_SAMPLE_DATA_S *pstData);
	HI_S32 (*tuner_set_vcm_acm_stream)(HI_U32 u32TunerPort, HI_U8 u8IsiId);
	HI_S32 (*tuner_get_vcm_acm_stream)(HI_U32 u32TunerPort, HI_U32 *u32TotalStream);
	HI_S32 (*tuner_get_isi_id)(HI_U32 u32TunerPort,HI_U8 u8Stream, HI_U8 *u8IsiId);
	HI_S32 (*tuner_get_agc)(HI_U32 u32TunerPort,HI_U32 Fcenter,HI_U32 *u32Agc );
	HI_S32 (*tuner_set_scramble)(HI_U32 u32TunerPort,HI_U32 u32Value );
	HI_S32 (*tuner_get_powerspecdata)(HI_U32 u32TunerPort, HI_U32 u32freqStartInHz,HI_U32 u32freqStepInHz,HI_U32 u32numOfFreqSteps,HI_S16 *ps16powerData);
    HI_S32 (*tuner_set_multi_mode_chip_attr)(HI_U32  u32TunerPort, HI_UNF_TUNER_MULTI_MODE_CHIP_ATTR_S *pstPortAttr);
    HI_S32 (*tuner_deinit)(HI_VOID);
    HI_S32 (*tuner_monitor_ISDBT_layer)(HI_U32 u32TunerId, HI_UNF_TUNER_MONITOR_LAYERS_CONFIG_S* pMonLayerConfig);
    HI_S32 (*tuner_ts_output_swap)(HI_U32 u32TunerId, HI_BOOL bSwapFlag);
    /* Added end:l00185424 2011-11-28 For DVB-S/S2 */
} TUNER_OPS_S;

extern TUNER_OPS_S g_stTunerOps[TUNER_NUM];
extern HI_U32 s_u32ResetGpioNo[TUNER_NUM];
extern HI_U32 g_u32ResetCruGpioNo;

extern  I2C_EXT_FUNC_S *s_tuner_pI2cFunc;
extern  GPIO_EXT_FUNC_S *s_tuner_pGpioFunc;
extern  GPIO_I2C_EXT_FUNC_S *s_tuner_pGpioI2cFunc;

/*---- TUNER COMMAND----*/
#define HI_TUNER_IOC_MAGIC 't'
#define TUNER_CONNECT_CMD _IOW(HI_TUNER_IOC_MAGIC, 1, TUNER_SIGNAL_S)
#define TUNER_GET_STATUS_CMD _IOWR(HI_TUNER_IOC_MAGIC, 2, TUNER_DATA_S)
#define TUNER_GET_SIGNALSTRENGTH_CMD _IOWR(HI_TUNER_IOC_MAGIC, 3, TUNER_DATABUF_S)
#define TUNER_GET_BER_CMD _IOWR(HI_TUNER_IOC_MAGIC, 4, TUNER_DATABUF_S)
#define TUNER_GET_SNR_CMD _IOWR(HI_TUNER_IOC_MAGIC, 5, TUNER_DATA_S)
#define TUNER_SET_TSTYPE_CMD _IOW(HI_TUNER_IOC_MAGIC, 6, TUNER_ATTR_S)
#define TUNER_SELECT_TYPE_CMD _IOW(HI_TUNER_IOC_MAGIC, 7, TUNER_ATTR_S)
#define TUNER_SELECT_I2C_CMD _IOW(HI_TUNER_IOC_MAGIC, 8, TUNER_ATTR_S)
#define TUNER_SELECT_RW_CMD _IOWR(HI_TUNER_IOC_MAGIC, 9, TUNER_RegRW_S)
#define TUNER_SET_TUNER_CMD _IOWR(HI_TUNER_IOC_MAGIC, 10, TUNER_SET_TUNER_S)
#define TUNER_LOW_CONS_CMD _IO(HI_TUNER_IOC_MAGIC, 11)
#define TUNER_NORMAL_MODE_CMD _IO(HI_TUNER_IOC_MAGIC, 12)
#define TUNER_SET_QAMINSIDE_CMD _IOR(HI_TUNER_IOC_MAGIC, 13, TUNER_QAMINSIDE_E)
#define TUNER_CONNECTUNBLOCK_CMD _IOW(HI_TUNER_IOC_MAGIC, 14, TUNER_SIGNAL_S)
#define TUNER_SELECT_SYMBOLRATE_CMD _IOR(HI_TUNER_IOC_MAGIC, 15, TUNER_DATA_S)
#define TUNER_CHECK_VALID_I2CADDR _IOW(HI_TUNER_IOC_MAGIC, 16, TUNER_DATA_S) 
#define TUNER_TEST_SINGLE_AGC _IOWR(HI_TUNER_IOC_MAGIC, 17, AGC_TEST_S) 
#define TUNER_GET_FREQ_SYMB_OFFSET _IOWR(HI_TUNER_IOC_MAGIC, 18, TUNER_DATABUF_S)
#define TUNER_CONNECT_TIMEOUT_CMD _IOWR(HI_TUNER_IOC_MAGIC, 19, TUNER_DATABUF_S)
#define TUNER_GET_SIGANLQUALITY_CMD _IOWR(HI_TUNER_IOC_MAGIC, 20, TUNER_DATA_S)
#define TUNER_GET_SIGANLINFO_CMD _IOWR(HI_TUNER_IOC_MAGIC, 21, TUNER_SIGNALINFO_S)
#define TUNER_BLINDSCAN_INIT_CMD _IOWR(HI_TUNER_IOC_MAGIC, 22, TUNER_BLINDSCAN_S)
#define TUNER_BLINDSCAN_ACTION_CMD _IOWR(HI_TUNER_IOC_MAGIC, 23, TUNER_BLINDSCAN_INFO_S)
#define TUNER_SET_LNBOUT_CMD _IOWR(HI_TUNER_IOC_MAGIC, 24, TUNER_LNB_OUT_S)
#define TUNER_SEND_CONTINUOUS_22K_CMD _IOWR(HI_TUNER_IOC_MAGIC, 25, TUNER_DATA_S)
#define TUNER_SEND_TONE_CMD _IOWR(HI_TUNER_IOC_MAGIC, 26, TUNER_DATA_S)
#define TUNER_DISEQC_SEND_MSG_CMD _IOWR(HI_TUNER_IOC_MAGIC, 27, TUNER_DISEQC_SENDMSG_S)
#define TUNER_DISEQC_RECV_MSG_CMD _IOWR(HI_TUNER_IOC_MAGIC, 28, TUNER_DISEQC_RECVMSG_S)
#define TUNER_STANDBY_CMD _IOWR(HI_TUNER_IOC_MAGIC, 29, TUNER_DATA_S)
#define TUNER_DISABLE_CMD _IOWR(HI_TUNER_IOC_MAGIC, 30, TUNER_DATA_S)
#define TUNER_SETFUNCMODE_CMD _IOW(HI_TUNER_IOC_MAGIC, 31, TUNER_DATA_S) 
#define TUNER_SETPLPNO_CMD _IOW(HI_TUNER_IOC_MAGIC, 32, TUNER_DATABUF_S) 
#define TUNER_GETPLPNUM_CMD _IOWR(HI_TUNER_IOC_MAGIC, 33, TUNER_DATA_S)
#define TUNER_GETCURPLPTYPE_CMD _IOWR(HI_TUNER_IOC_MAGIC, 34, TUNER_DATA_S)
#define TUNER_SETTSOUT_CMD _IOWR(HI_TUNER_IOC_MAGIC, 35, TUNER_TSOUTSET_S)
#define TUNER_TPVERIFY_CMD _IOWR(HI_TUNER_IOC_MAGIC, 36, TUNER_TP_VERIFY_INFO_S)
#define TUNER_SETSATATTR_CMD _IOWR(HI_TUNER_IOC_MAGIC, 37, TUNER_SETSATATTR_S)
#define TUNER_SAMPLE_DATA_CMD _IOWR(HI_TUNER_IOC_MAGIC, 38, TUNER_SAMPLEDATA_S)
#define TUNER_SET_ISI_ID_CMD _IOW(HI_TUNER_IOC_MAGIC, 39, TUNER_DATA_S)
#define TUNER_GET_TOTAL_STREAM_CMD _IOWR(HI_TUNER_IOC_MAGIC, 40, TUNER_DATA_S)
#define TUNER_GET_ISI_ID_CMD _IOWR(HI_TUNER_IOC_MAGIC, 41, TUNER_DATABUF_S)
#define TUNER_GET_AGC _IOWR(HI_TUNER_IOC_MAGIC, 42, TUNER_DATABUF_S)
#define TUNER_SET_COMMONPLP_CMD _IOW(HI_TUNER_IOC_MAGIC, 43, TUNER_DATA_S)
#define TUNER_SET_COMMONPLP_COMBINATION_CMD _IOW(HI_TUNER_IOC_MAGIC, 44, TUNER_DATA_S)
#define TUNER_GET_PLP_ID_CMD _IOWR(HI_TUNER_IOC_MAGIC, 45, TUNER_DATA_S)
#define TUNER_GET_GROUP_PLP_ID_CMD _IOWR(HI_TUNER_IOC_MAGIC, 46, TUNER_DATA_S)
#define TUNER_SET_ANTENNA_POWER_CMD _IOW(HI_TUNER_IOC_MAGIC, 47, TUNER_DATA_S)
#define TUNER_TERSCAN_ACTION_CMD _IOWR(HI_TUNER_IOC_MAGIC, 48, TUNER_TERSCAN_INFO_S)
#define TUNER_SET_SCRAMBLE_CMD   _IOW(HI_TUNER_IOC_MAGIC, 49, TUNER_DATA_S)
#define TUNER_TUENR_POWER_SPECTRUM_DATA_CMD _IOWR(HI_TUNER_IOC_MAGIC, 50, TUNER_SPECTRUMDATA_S)
#define TUNER_SET_MULTI_MODE_CHIP_ATTR_CMD _IOWR(HI_TUNER_IOC_MAGIC, 51, TUNER_ATTR_S)
#define TUNER_CLOSE_CMD _IOWR(HI_TUNER_IOC_MAGIC, 52, TUNER_DATA_S)
#define TUNER_MONITOR_ISDBT_LAYER_CMD _IOWR(HI_TUNER_IOC_MAGIC, 53, TUNER_ISDBTLAYER_S)
#define TUNER_TS_OUTPUT_SWAP_CMD _IOW(HI_TUNER_IOC_MAGIC, 54, TUNER_DATA_S)
#define TUNER_SETTERATTR_CMD _IOWR(HI_TUNER_IOC_MAGIC, 55, TUNER_SETTERATTR_S)
#define TUNER_SETCABATTR_CMD _IOWR(HI_TUNER_IOC_MAGIC, 56, TUNER_SETCABATTR_S)

/*
#define TUNER_BASE_CONNECT_CMD _IOW(HI_TUNER_IOC_MAGIC, 20, TUNER_SIGNAL_S)
#define TUNER_BASE_SELECT_TYPE_CMD _IOW(HI_TUNER_IOC_MAGIC, 21, TUNER_DATABUF_S)
#define TUNER_BASE_SELECT_I2C_CMD _IOW(HI_TUNER_IOC_MAGIC, 22, TUNER_DATA_S)
#define TUNER_BASE_SET_TSTYPE_CMD _IOW(HI_TUNER_IOC_MAGIC, 23, TUNER_DATA_S)
*/

extern HI_S32 tuner_i2c_send_data(HI_U32 u32I2cChannel, HI_U8 u8DevAddress, PTR_TUNER_I2C_DATA_S pstDataStruct);
extern HI_S32 tuner_i2c_receive_data(HI_U32 u32I2cChannel, HI_U8 u8DevAddress, PTR_TUNER_I2C_DATA_S pstDataStruct);
extern HI_S32 tuner_i2c_receive_data_si(HI_U32 u32I2cChannel, HI_U8 u8DevAddress, PTR_TUNER_I2C_DATA_S pstDataStruct);
extern HI_VOID reset_special_process_flag(HI_BOOL flag);
extern HI_VOID tuner_mdelay(HI_U32 u32MS);

#endif

