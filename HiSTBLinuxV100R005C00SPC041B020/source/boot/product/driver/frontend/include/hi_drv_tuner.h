#ifndef __HI_DRV_TUNER_H__
#define __HI_DRV_TUNER_H__

#include "exports.h"
#include "hi_type.h"
#include "hi_unf_frontend.h"
//#include "hi_tuner.h"

//#include "hi_unf_ecs.h"
//#include "priv_tuner.h"

#define QAM_PORT0_ADDR 0xA0
#define QAM_PORT1_ADDR 0xA2
#define QAM_PORT2_ADDR 0xA0
#define TUNER_PORT0_ADDR 0xC0
#define TUNER_PORT1_ADDR 0xC0
#define TUNER_PORT2_ADDR 0xC0
#define XTAL_CLK 28800


//#define TUNER_DEBUG_ENABLE
//#define TUNER_DEBUG(fmt,args...) printf( "%s,%d: " fmt , __FUNCTION__, __LINE__, ## args)
#define TUNER_DEBUG(fmt,args...)

#define HI_ERR_TUNER TUNER_DEBUG
#define HI_INFO_TUNER TUNER_DEBUG

#ifndef HI_ASSERT
#define HI_ASSERT(expr)                                     \
    do{                                                     \
        if (!(expr)) {                                      \
            TUNER_DEBUG("\nASSERT failed at:\n  >File name: %s\n  >Function : %s\n  >Line No. : %d\n  >Condition: %s\n", \
				__FILE__,__FUNCTION__, __LINE__, #expr);    \
			return HI_ERR_TUNER_INVALID_POINT;              \
        }                                                   \
    }while(0)
#endif

#define printk TUNER_DEBUG
#define msleep_interruptible HI_TIMER_mdelay
#define msleep HI_TIMER_mdelay
#define usleep udelay
#define abs(a) ((a)>=0?(a):-(a))
#define HI_VMALLOC(ID, Size) malloc(Size)
#define HI_VFREE(ID, Addr) free(Addr)

#define HI_ERR_TUNER_NOT_INIT                       (HI_S32)(0x804A0001)
#define HI_ERR_TUNER_NOT_OPEN                       (HI_S32)(0x804A0002)
#define HI_ERR_TUNER_INVALID_POINT                  (HI_S32)(0x804A0003)
#define HI_ERR_TUNER_INVALID_PARA                   (HI_S32)(0x804A0004)
#define HI_ERR_TUNER_FAILED_INIT                    (HI_S32)(0x804A0005)
#define HI_ERR_TUNER_FAILED_CONNECT                 (HI_S32)(0x804A0006)
#define HI_ERR_TUNER_FAILED_GETSTATUS               (HI_S32)(0x804A0007)
#define HI_ERR_TUNER_FAILED_GETSIGNALSTRENGTH       (HI_S32)(0x804A0008)
#define HI_ERR_TUNER_FAILED_GETBER                  (HI_S32)(0x804A0009)
#define HI_ERR_TUNER_FAILED_GETSNR                  (HI_S32)(0x804A000A)
#define HI_ERR_TUNER_FAILED_SETTSTYPE               (HI_S32)(0x804A000B)
#define HI_ERR_TUNER_FAILED_SELECTTUNER             (HI_S32)(0x804A000C)
#define HI_ERR_TUNER_FAILED_SELECTI2CCHANNEL        (HI_S32)(0x804A000D)
#define HI_ERR_TUNER_FAILED_LOWCONS                 (HI_S32)(0x804A000E)
#define HI_ERR_TUNER_FAILED_NORMALMODE              (HI_S32)(0x804A000F)
#define HI_ERR_TUNER_FAILED_REGRW                   (HI_S32)(0x804A0010)
#define HI_ERR_TUNER_FAILED_SETTUNER                (HI_S32)(0x804A0011)
#define HI_ERR_TUNER_NOT_CONNECT                    (HI_S32)(0x804A0012)
#define HI_ERR_TUNER_INVALID_PORT                   (HI_S32)(0x804A0013)
#define HI_ERR_TUNER_FAILED_SETQAMINSIDE            (HI_S32)(0x804A0014)
#define HI_ERR_TUNER_FAILED_SELECTSYMBOLRATE        (HI_S32)(0x804A0015)
#define HI_ERR_TUNER_FAILED_GETSIGNALQUALITY        (HI_S32)(0x804A0016)
#define HI_ERR_TUNER_FAILED_GETSIGNALINFO           (HI_S32)(0x804A0017)
#define HI_ERR_TUNER_FAILED_BLINDSCAN               (HI_S32)(0x804A0018)
#define HI_ERR_TUNER_FAILED_LNBCTRL                 (HI_S32)(0x804A0019)
#define HI_ERR_TUNER_FAILED_SWITCH                  (HI_S32)(0x804A001A)
#define HI_ERR_TUNER_FAILED_DISEQC                  (HI_S32)(0x804A001B)
#define HI_ERR_TUNER_FAILED_STANDBY                 (HI_S32)(0x804A001C)
#define HI_ERR_TUNER_FAILED_WAKEUP                  (HI_S32)(0x804A001D)
#define HI_ERR_TUNER_FAILED_DISABLE                 (HI_S32)(0x804A001E)
#define HI_ERR_TUNER_FAILED_ENABLE                  (HI_S32)(0x804A001F)
#define HI_ERR_TUNER_FAILED_SETPLPID                (HI_S32)(0x804A0020)
#define HI_ERR_TUNER_FAILED_GETPLPNUM               (HI_S32)(0x804A0021)
#define HI_ERR_TUNER_FAILED_GETPLPTYPE              (HI_S32)(0x804A0022)
#define HI_ERR_TUNER_FAILED_SETTSOUT                (HI_S32)(0x804A0023)
#define HI_ERR_TUNER_FAILED_SETSATATTR              (HI_S32)(0x804A0024)

#if 0
#ifdef TUNER_DEBUG_ENABLE
#ifndef API_DEBUG
    #define API_DEBUG(fmt...) HI_TRACE(HI_LOG_LEVEL_ERROR, HI_DEBUG_ID_TUNER, fmt)
#endif
#else
#ifndef TUNER_DEBUG
    #define TUNER_DEBUG(fmt,args...) printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#endif
#endif
#endif
#define CHECK_TUNER_POINTER(ptr)                            \
    do                                                      \
    {                                                       \
        if (!(ptr))                                         \
        {                                                   \
            return HI_ERR_TUNER_INVALID_POINT;              \
        }                                                   \
    } while (0)

#define TUNER_NUM 1
#define UNF_TUNER_NUM TUNER_NUM


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
    TUNER_QAM_TYPE_E enQamType;
    HI_BOOL bSI;              /*reverse*/
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar; 
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

#define MAX_TP_NUM (128)

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
    TUNER_BLINDSCAN_PARA_S* pstPara;
}TUNER_BLINDSCAN_INFO_S, *PTR_TUNER_BLINDSCAN_INFO_S;

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
    HI_UNF_TUNER_DISEQC_RECVMSG_S* pstRecvMsg;  /* Read data */
}TUNER_DISEQC_RECVMSG_S, *PTR_TUNER_DISEQC_RECVMSG_S;

typedef struct hiTUNER_SET_TUNER_S
{
    HI_U32 u32Freq;
    HI_S32 s32Times;    
}TUNER_SET_TUNER_S, *PTR_TUNER_SET_TUNER_S;

#if 0
//jkf33868 2012/11/06 add start 
typedef enum    hiTUNER_SIG_TYPE_E
{
    HI_UNF_TUNER_SIG_TYPE_CAB = 0  ,  
    HI_UNF_TUNER_SIG_TYPE_SAT ,        
    HI_UNF_TUNER_SIG_TYPE_DVB_T  ,   
    HI_UNF_TUNER_SIG_TYPE_DVB_T2  , 
    HI_UNF_TUNER_SIG_TYPE_ISDB_T  ,  
    HI_UNF_TUNER_SIG_TYPE_ATSC_T  ,  
    HI_UNF_TUNER_SIG_TYPE_DTMB  ,     
    HI_UNF_TUNER_SIG_TYPE_BUTT         
}HI_UNF_TUNER_SIG_TYPE_E;
#endif

typedef struct tag_TunerAttr_S
{
    HI_UNF_TUNER_DEV_TYPE_E enTunerDevType;     /**<Tuner type*/                              /**<CNcomment:TUNER类型*/
    HI_U32 u32TunerAddr;
} HI_TunerAttr_S;

typedef struct tag_DemodAttr_S
{
    HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType;     /**<QAM type*/                                /**<CNcomment:QAM类型*/
    HI_U32 u32DemodAddr;
} HI_DemodAttr_S;

typedef struct hiTUNER_TP_VERIFY_PARAMS_S
{
    HI_U32 *pu32Frequency;            /**<frequency kHz*/                            /**<CNcomment:频率，单位kHz*/
    HI_U32 *pu32SymbolRate;           /**<symbolrate kBaud*/                         /**<CNcomment:符号率，单位kBaud*/
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar; /**<TP polarization*/                    /**<CNcomment:TP的极化方式*/
    HI_U8 cbs_reliablity;                           /**<TP reliability*/             /**<CNcomment:TP的可靠度*/
    HI_U8 CBS_TH;                                   /**<blindscan threshold*/        /**<CNcomment:盲扫阈值*/
    HI_U8 fs_grade;
    HI_U32 *fec_ok_cnt;                             /**<TP number of fec ok*/        /**<CNcomment:fec ok的TP数量*/
    HI_U32 *fec_no_ok_cnt;                          /**<TP number of fec  not ok*/   /**<CNcomment:fec not ok的TP数量*/
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
#ifndef BOOT_COMPILE
    HI_S32 (*tuner_get_ber)(HI_U32 u32TunerPort, HI_U32 * pu32BERa);
    HI_S32 (*tuner_get_snr)(HI_U32 u32TunerPort, HI_U32 * pu32SignalStrength);
    HI_S32 (*tuner_get_signal_strength)(HI_U32 u32TunerPort, HI_U32 * pu32strength);
#endif
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

    HI_S32 (*tuner_init)(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType);
    HI_S32 (*tuner_set_sat_attr)(HI_U32 u32TunerPort, HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr);
    HI_S32 (*tuner_lnbctrl_dev_init)(HI_U32 u32TunerPort, HI_U32 u32I2CNum, HI_U8 u8DevAddr, 
                            HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType);
    HI_S32 (*tuner_set_lnb_out)(HI_U32 u32TunerPort, TUNER_LNB_OUT_E enOut);
    HI_S32 (*tuner_send_continuous_22K)(HI_U32 u32TunerPort, HI_U32 u32Continuous22K);
    HI_S32 (*tuner_send_tone)(HI_U32 u32TunerPort, HI_U32 u32Tone);
    HI_S32 (*tuner_DiSEqC_send_msg)(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_SENDMSG_S *pstSendMsg);
    HI_S32 (*tuner_DiSEqC_recv_msg)(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_RECVMSG_S *pstRecvMsg);
    HI_S32 (*tuner_set_ts_out)(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut);

//jkf33868 2012/1106
#if 0
    /* Added begin:l00185424 2011-11-28 For DVB-S/S2 */
    HI_S32 (*tuner_get_signal_info)(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo);
    HI_S32 (*tuner_blindscan_init)(HI_U32 u32TunerPort, TUNER_BLINDSCAN_INITPARA_S *pstPara);
    HI_S32 (*tuner_blindscan_action)(HI_U32 u32TunerPort, TUNER_BLINDSCAN_PARA_S *pstPara);
    HI_S32 (*tuner_lnbctrl_dev_init)(HI_U32 u32TunerPort, HI_U32 u32I2CNum, HI_U8 u8DevAddr, 
                            HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType);
    HI_S32 (*tuner_lnbctrl_dev_standby)(HI_U32 u32TunerPort, HI_U32 u32Standby);
    HI_S32 (*tuner_set_lnb_out)(HI_U32 u32TunerPort, TUNER_LNB_OUT_E enOut);
    HI_S32 (*tuner_send_continuous_22K)(HI_U32 u32TunerPort, HI_U32 u32Continuous22K);
    HI_S32 (*tuner_send_tone)(HI_U32 u32TunerPort, HI_U32 u32Tone);
    HI_S32 (*tuner_DiSEqC_send_msg)(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_SENDMSG_S *pstSendMsg);
    HI_S32 (*tuner_DiSEqC_recv_msg)(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_RECVMSG_S *pstRecvMsg);
    HI_S32 (*tuner_standby)(HI_U32 u32TunerPort, HI_U32 u32Standby);
    HI_S32 (*tuner_disable)(HI_U32 u32TunerPort, HI_U32 u32Disable);
    HI_S32 (*tuner_setfuncmode)(HI_U32 u32TunerPort, TUNER_FUNC_MODE_E enFuncMode);
    HI_S32 (*tuner_setplpid)(HI_U32 u32TunerPort, HI_U8 u8PLPID);
    HI_S32 (*tuner_get_plp_num)(HI_U32 u32TunerPort, HI_U8 *pu8PLPNum);
    HI_S32 (*tuner_get_current_plp_type)(HI_U32 u32TunerPort, HI_UNF_TUNER_T2_PLP_TYPE_E *penPLPType);
    /* Added end:l00185424 2011-11-28 For DVB-S/S2 */
#endif
} TUNER_OPS_S;


extern TUNER_OPS_S g_stTunerOps[TUNER_NUM];
extern HI_U32 g_u32ResetCruGpioNo;


#define IO_ADDRESS(addr) (addr)

#if 0
extern HI_S32 HI_I2C_Write(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 *pData, HI_U32 DataLen);
extern HI_S32 HI_I2C_Read(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 *pData, HI_U32 DataLen);
extern HI_S32 HI_I2C_ReadDirectly(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 *pData, HI_U32 DataLen);
/* For gpio i2c */
extern HI_S32 gpio_i2c_read_ext_directly(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen, HI_U8 *pData, HI_U32 DataLen);
extern HI_S32 gpio_i2c_write_ext(HI_U32 I2cNum, HI_U8 devaddress, HI_U32 address, HI_U32 addresslen, HI_U8 *pData, HI_U32 DataLen);
#endif

extern HI_S32 tuner_i2c_send_data(HI_U32 u32I2cChannel, HI_U8 u8DevAddress, PTR_TUNER_I2C_DATA_S pstDataStruct);
extern HI_S32 tuner_i2c_receive_data(HI_U32 u32I2cChannel, HI_U8 u8DevAddress, PTR_TUNER_I2C_DATA_S pstDataStruct);
extern HI_S32 tuner_i2c_receive_data_si(HI_U32 u32I2cChannel, HI_U8 u8DevAddress, PTR_TUNER_I2C_DATA_S pstDataStruct);
extern HI_VOID HI_TIMER_mdelay(HI_U32 msec);
extern HI_S32 tuner_chip_reset(HI_U32 u32ResetGpioNo);
//extern HI_VOID reset_special_process_flag(HI_BOOL flag);

#endif

