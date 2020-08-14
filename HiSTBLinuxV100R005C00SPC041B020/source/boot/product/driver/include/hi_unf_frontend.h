/******************************************************************************

  Copyright (C), 2001-2011, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name     : hi_unf_frontend.h
Version       : Initial draft
Author        : HiSilicon multimedia software group
Created Date   : 2008-06-05
Last Modified by:
Description   : Application programming interfaces (APIs) of the external chip software (ECS)
Function List :
Change History:
 ******************************************************************************/
#ifndef __HI_UNF_FRONTEND_H__
#define __HI_UNF_FRONTEND_H__

#include "hi_common.h"
//#include "hi_error_mpi.h"

//#include "hi_unf_keyled.h"
//#include "hi_unf_ir.h"
//#include "hi_unf_pmoc.h"
//#include "hi_unf_i2c.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_I2C_MAX_NUM_USER (15)    /**<Maximum I2C channel ID*/ /**<CNcomment:最大I2C通道号*/

#define HI_UNF_DISEQC_MSG_MAX_LENGTH (6)    /**<DiSEqC message length*/  /**<CNcomment:DiSEqC消息长度*/
#define HI_UNF_DISEQC_MAX_REPEAT_TIMES (4)  /**<DiSEqC message max repeat times*/
#define MAX_TS_LINE 11                      /** The NO of ts lines that can be configured*/ /** CNcomment:可配置的ts信号线数量*/

//#ifdef HI_BOOT_DISEQC_SUPPORT
#define DISEQC_MAX_MOTOR_PISITION (255) /**<DiSEqC motor max stored position*/ 
//#endif /* HI_BOOT_DISEQC_SUPPORT */
/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_1 */
/** @{ */  /** <!-- [QAM] */

/**Modulation mode*/
/**CNcomment: 调制方式*/
typedef enum hiUNF_QAM_TYPE_E
{
    HI_UNF_MOD_TYPE_DEFAULT,          /**<Default QAM mode. The default QAM mode is HI_UNF_MOD_TYPE_QAM_64 at present.*/   /**<CNcomment:默认的QAM类型, 当前系统默认为HI_UNF_MOD_TYPE_QAM_64 */
    HI_UNF_MOD_TYPE_QAM_16 = 0x100,   /**<Enumeration corresponding to the 16QAM mode*/                                    /**<CNcomment:16QAM对应的枚举值*/
    HI_UNF_MOD_TYPE_QAM_32,           /**<Enumeration corresponding to the 32QAM mode*/                                    /**<CNcomment:32QAM对应的枚举值*/
    HI_UNF_MOD_TYPE_QAM_64,           /**<Enumeration corresponding to the 64QAM mode*/                                    /**<CNcomment:64QAM对应的枚举值*/
    HI_UNF_MOD_TYPE_QAM_128,          /**<Enumeration corresponding to the 128QAM mode*/                                   /**<CNcomment:128QAM对应的枚举值*/
    HI_UNF_MOD_TYPE_QAM_256,          /**<Enumeration corresponding to the 256QAM mode*/                                   /**<CNcomment:256QAM对应的枚举值*/
    HI_UNF_MOD_TYPE_QAM_512,          /**<Enumeration corresponding to the 512QAM mode*/                                   /**<CNcomment:512QAM对应的枚举值*/

    HI_UNF_MOD_TYPE_BPSK = 0x200,     /**<Enumeration corresponding to the binary phase shift keying (BPSK) mode. */         /**<CNcomment:BPSK对应的枚举值*/
    HI_UNF_MOD_TYPE_QPSK = 0x300,     /**<Enumeration corresponding to the quaternary phase shift keying (QPSK) mode. */     /**<CNcomment:QPSK对应的枚举值*/
    HI_UNF_MOD_TYPE_DQPSK,
    HI_UNF_MOD_TYPE_8PSK,             /**<Enumeration corresponding to the 8 phase shift keying (8PSK) mode*/              /**<CNcomment:8PSK对应的枚举值*/
    HI_UNF_MOD_TYPE_16APSK,           /**<Enumeration corresponding to the 16-Ary Amplitude and Phase Shift Keying (16APSK) mode*/      /**<CNcomment:16APSK对应的枚举值*/
    HI_UNF_MOD_TYPE_32APSK,           /**<Enumeration corresponding to the 32-Ary Amplitude and Phase Shift Keying (32APSK) mode*/      /**<CNcomment:32APSK对应的枚举值*/
    HI_UNF_MOD_TYPE_8VSB,           /**<Enumeration corresponding to (8VSB) mode*/      /**<CNcomment:8VSB对应的枚举值*/
    HI_UNF_MOD_TYPE_16VSB,           /**<Enumeration corresponding to (16VSB) mode*/      /**<CNcomment:16VSB对应的枚举值*/

    /**<Enumeration corresponding to the auto mode. For DVB-S/S2, if detect modulation type fail, it will return auto*/
    /**<CNcomment:卫星信号调制方式自动检测，如果检测失败返回AUTO*/
    HI_UNF_MOD_TYPE_AUTO,

    HI_UNF_MOD_TYPE_BUTT              /**<Invalid Modulation mode*/       /**<CNcomment:非法的调制类型枚举值*/
}HI_UNF_MODULATION_TYPE_E;

/**Frequency locking status of the tuner*/
/**CNcomment:TUNER锁频状态*/
typedef enum  hiUNF_TUNER_LOCK_STATUS_E
{
    HI_UNF_TUNER_SIGNAL_DROPPED = 0,      /**<The signal is not locked.*/  /**<CNcomment:信号未锁定*/
    HI_UNF_TUNER_SIGNAL_LOCKED,            /**<The signal is locked.*/  /**<CNcomment:信号已锁定*/
    HI_UNF_TUNER_SIGNAL_BUTT             /**<Invalid value*/   /**<CNcomment:非法边界值*/
}HI_UNF_TUNER_LOCK_STATUS_E;

/**Output mode of the tuner*/
/**CNcomment:TUNER输出模式 */
typedef enum hiUNF_TUNER_OUTPUT_MODE_E
{
    HI_UNF_TUNER_OUTPUT_MODE_DEFAULT ,            /**<Default mode*/        /**<CNcomment:默认模式*/
    HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A,     /**<Parallel mode A*/     /**<CNcomment:并行模式A*/
    HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B,     /**<Parallel mode B*/     /**<CNcomment:并行模式B*/
    HI_UNF_TUNER_OUTPUT_MODE_SERIAL,              /**<Serial mode 74.25M*/     /**<CNcomment:串行模74.25M*/ 
    HI_UNF_TUNER_OUTPUT_MODE_SERIAL_50,           /**<Serial mode 50M*/  /**<CNcomment:串行模50M*/
    HI_UNF_TUNER_OUTPUT_MODE_SERIAL_2BIT,
    HI_UNF_TUNER_OUTPUT_MODE_BUTT                 /**<Invalid value*/       /**<CNcomment:非法值*/
}HI_UNF_TUNER_OUPUT_MODE_E;

/**Output order*/
/**CNcomment:TUNER输出线序 */
typedef enum hiUNF_TUNER_OUTPUT_TS_E
{
    HI_UNF_TUNER_OUTPUT_TSDAT0,        /**<data0*/                       /**<CNcomment:数据线0bit*/
    HI_UNF_TUNER_OUTPUT_TSDAT1,        /**<data1*/                       /**<CNcomment:数据线1bit*/
    HI_UNF_TUNER_OUTPUT_TSDAT2,        /**<data2*/                       /**<CNcomment:数据线2bit*/
    HI_UNF_TUNER_OUTPUT_TSDAT3,        /**<data3*/                       /**<CNcomment:数据线3bit*/
    HI_UNF_TUNER_OUTPUT_TSDAT4,        /**<data4*/                       /**<CNcomment:数据线4bit*/
    HI_UNF_TUNER_OUTPUT_TSDAT5,        /**<data5*/                       /**<CNcomment:数据线5bit*/
    HI_UNF_TUNER_OUTPUT_TSDAT6,        /**<data6*/                       /**<CNcomment:数据线6bit*/
    HI_UNF_TUNER_OUTPUT_TSDAT7,        /**<data7*/                       /**<CNcomment:数据线7bit*/
    HI_UNF_TUNER_OUTPUT_TSSYNC,        /**<sync*/                        /**<CNcomment:sync信号线*/
    HI_UNF_TUNER_OUTPUT_TSVLD,         /**<valid*/                       /**<CNcomment:valid信号线*/
    HI_UNF_TUNER_OUTPUT_TSERR,         /**<err*/                         /**<CNcomment:err信号线*/
    HI_UNF_TUNER_OUTPUT_BUTT           /**<Invalid value*/               /**<CNcomment:非法值*/
} HI_UNF_TUNER_OUTPUT_TS_E;

typedef struct hiUNF_TUNER_TSOUT_SET_S
{
    HI_UNF_TUNER_OUTPUT_TS_E enTSOutput[MAX_TS_LINE];
}HI_UNF_TUNER_TSOUT_SET_S;

/**Signal type of the tuner*/
/**CNcomment:TUNER信号类型*/
typedef enum    hiTUNER_SIG_TYPE_E
{
    HI_UNF_TUNER_SIG_TYPE_CAB = 1,      /**<Cable signal*/          /**<CNcomment:ITU-T J.83 ANNEX A/C(DVB_C)信号信号*/
    HI_UNF_TUNER_SIG_TYPE_SAT = 2,          /**<Satellite signal*/      /**<CNcomment:卫星信号*/
    HI_UNF_TUNER_SIG_TYPE_DVB_T = 4,        /**<Terrestrial signal*/    /**<CNcomment:地面信号*/
    HI_UNF_TUNER_SIG_TYPE_DVB_T2 = 8,       /**<Terrestrial signal*/    /**<CNcomment:地面信号*/
    HI_UNF_TUNER_SIG_TYPE_ISDB_T = 16,       /**<Terrestrial signal*/    /**<CNcomment:地面信号*/
    HI_UNF_TUNER_SIG_TYPE_ATSC_T = 32,       /**<Terrestrial signal*/    /**<CNcomment:地面信号*/
    HI_UNF_TUNER_SIG_TYPE_DTMB = 64,         /**<Terrestrial signal*/    /**<CNcomment:地面信号*/
    HI_UNF_TUNER_SIG_TYPE_J83B = 128,         /**<Cable signal*/          /**<CNcomment:ITU-T J.83 ANNEX B(US Cable)信号*/

    HI_UNF_TUNER_SIG_TYPE_BUTT          /**<Invalid value*/         /**<CNcomment:非法值*/
} HI_UNF_TUNER_SIG_TYPE_E;

/**type of TUNER device*/
/**CNcomment:TUNER设备类型*/
typedef enum    hiUNF_TUNER_DEV_TYPE_E
{
    HI_UNF_TUNER_DEV_TYPE_XG_3BL,               /**<XG_3BL*/                    /**<CNcomment:支持旭光射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_CD1616,               /**<CD1616*/                    /**<CNcomment:支持CD1616射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_ALPS_TDAE,            /**<ALPS_TDAE*/                 /**<CNcomment:支持ALPS_TDAE射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_TDCC,                 /**<TDCC*/                      /**<CNcomment:支持TDCC射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_TDA18250,             /**<TDA18250*/                  /**<CNcomment:支持TDA18250射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_CD1616_DOUBLE,        /**<CD1616 with double agc*/    /**<CNcomment:支持CD1616带双agc射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_MT2081,               /**<MT2081*/                    /**<CNcomment:支持MT2081射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_TMX7070X,             /**<THOMSON7070X*/              /**<CNcomment:支持THOMSON7070X射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_R820C,                /**<R820C*/                     /**<CNcomment:支持R820C射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_MXL203,               /**<MXL203 */                   /**<CNcomment:支持MXL203射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_AV2011,               /**<AV2011*/                    /**<CNcomment:支持AV2011射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_SHARP7903,            /**<SHARP7903*/                 /**<CNcomment:支持SHARP7903射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_MXL101,               /**<MXL101*/                    /**<CNcomment:支持MXL101射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_MXL603,               /**<MXL603*/                    /**<CNcomment:支持MXL603射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_IT9170,               /**<IT9170*/                    /**<CNcomment:支持IT9170射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_IT9133,               /**<IT9133*/                    /**<CNcomment:支持IT9133射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_TDA6651,              /**<TDA6651*/                   /**<CNcomment:支持TDA6651射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_TDA18250B,            /**<TDA18250B*/                 /**<CNcomment:支持TDA18250B射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_M88TS2022,            /**<M88TS2022*/                 /**<CNcomment:支持M88TS2022射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_RDA5815,              /**<RDA5815*/                   /**<CNcomment:支持RDA5815射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_MXL254,               /**<MXL254*/                    /**<CNcomment:支持MXL254射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_CXD2861,              /**<CXD2861*/                   /**<CNcomment:支持CXD2861射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_SI2147,               /**<Si2147*/                    /**<CNcomment:支持Si2147射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_RAFAEL836,				/**<Rafael836*/					/**<CNcomment:支持Rafael836射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_MXL608,				/**<MXL608*/                    /**<CNcomment:支持MXL608射频芯片*/
    HI_UNF_TUNER_DEV_TYPE_MXL214,               /**<MXL214*/                    /**<CNcomment:支持MXL214射频芯片*/    
    HI_UNF_TUNER_DEV_TYPE_TDA18280,             /**<TDA18280*/                    /**<CNcomment:支持TDA18280射频芯片*/ 
    HI_UNF_TUNER_DEV_TYPE_TDA182I5A,             /**<TDA182I5A*/       			/**<CNcomment:支持TDA182I5A芯片*/
    HI_UNF_TUNER_DEV_TYPE_SI2144,             /**<Si2144*/       			/**<CNcomment:支持Si2144芯片*/
    HI_UNF_TUNER_DEV_TYPE_AV2018,             /**<AV2018*/       			/**<CNcomment:支持AV2018芯片*/
    HI_UNF_TUNER_DEV_TYPE_MXL251,		      /**<MXL251*/				    /**<CNcomment:支持MXL251芯片*/
    HI_UNF_TUNER_DEV_TYPE_M88TC3800,			/**<M88TC3800*/				    /**<CNcomment:支持M88TC3800芯片*/
    HI_UNF_TUNER_DEV_TYPE_BUTT                  /**<Invalid value*/             /**<CNcomment:非法边界值*/
} HI_UNF_TUNER_DEV_TYPE_E ;

/**Type of the demod device*/
/**CNcomment:demod设备类型*/
typedef enum    hiUNF_DEMOD_DEV_TYPE_E
{
    HI_UNF_DEMOD_DEV_TYPE_NONE,                 /**<Not supported*/         /**<CNcomment:不支持*/
    HI_UNF_DEMOD_DEV_TYPE_3130I = 0x100,        /**<Internal QAM*/          /**<CNcomment:内部QAM*/
    HI_UNF_DEMOD_DEV_TYPE_3130E,                /**<External Hi3130*/       /**<CNcomment:外部QAM hi3130芯片*/
    HI_UNF_DEMOD_DEV_TYPE_J83B,                 /**<suppoort j83b*/         /**<CNcomment:支持j83b*/
    HI_UNF_DEMOD_DEV_TYPE_AVL6211,              /**<Avalink 6211*/          /**<CNcomment:支持Avalink 6211*/
    HI_UNF_DEMOD_DEV_TYPE_MXL101,               /**<Maxlinear mxl101*/      /**<CNcomment:支持Maxlinear mxl101*/
    HI_UNF_DEMOD_DEV_TYPE_MN88472,              /**<PANASONIC mn88472*/     /**<CNcomment:支持PANASONIC mn88472*/
    HI_UNF_DEMOD_DEV_TYPE_IT9170,               /**<ITE it9170*/            /**<CNcomment:支持ITE it9170*/
    HI_UNF_DEMOD_DEV_TYPE_IT9133,               /**<ITE it9133*/            /**<CNcomment:支持ITE it9133*/
    HI_UNF_DEMOD_DEV_TYPE_3136,                 /**<External Hi3136*/       /**<CNcomment:外部hi3136芯片*/
    HI_UNF_DEMOD_DEV_TYPE_3136I,                /**<Internal Hi3136*/       /**<CNcomment:内部hi3136芯片*/
    HI_UNF_DEMOD_DEV_TYPE_MXL254,               /**<External mxl254*/       /**<CNcomment:外部MXL254芯片*/
    HI_UNF_DEMOD_DEV_TYPE_CXD2837,              /**Sony cxd2837*/           /**<CNcomment:支持sony cxd2837*/
    HI_UNF_DEMOD_DEV_TYPE_3137,                 /**External Hi3137*/        /**<CNcomment:支持外部hi3137芯片*/
    HI_UNF_DEMOD_DEV_TYPE_MXL214,               /**<External mxl214*/       /**<CNcomment:外部MXL214芯片*/
    HI_UNF_DEMOD_DEV_TYPE_TDA18280,             /**<External tda18280*/       /**<CNcomment:外部tda18280芯片*/
	HI_UNF_DEMOD_DEV_TYPE_BUTT                  /**<Invalid value*/         /**<CNcomment:非法边界值*/
} HI_UNF_DEMOD_DEV_TYPE_E;

/**Defines the cable transmission signal.*/
/**CNcomment:定义CABLE传输信号*/
typedef struct  hiUNF_CAB_CONNECT_PARA_S
{
    HI_U32                      u32Freq;            /**<Frequency, in kHz*/      /**<CNcomment:频率，单位：kHz*/
    HI_U32                      u32SymbolRate ;     /**<Symbol rate, in bit/s*/  /**<CNcomment:符号率，单位bps */
    HI_UNF_MODULATION_TYPE_E    enModType ;         /**<QAM mode*/               /**<CNcomment:QAM调制方式*/
    HI_BOOL                     bReverse ;          /**<Spectrum reverse mode*/  /**<CNcomment:频谱倒置处理方式*/
}HI_UNF_CAB_CONNECT_PARA_S ;

/*Guard interval of OFDM*/
/*CNcomment:多载波调制下的保护间隔*/
typedef enum hiUNF_TUNER_FE_GUARD_INTV_E
{
    HI_UNF_TUNER_FE_GUARD_INTV_DEFALUT = 0 ,    /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_1_128       ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_1_32       ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_1_16       ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_1_8        ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_1_4        ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_19_128       ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_19_256       ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_BUTT     ,       /**< */
}HI_UNF_TUNER_FE_GUARD_INTV_E;

/*Not used currently*/
/*CNcomment:暂未使用*/
typedef enum hiUNF_TUNER_FE_DEPUNC_RATE_E
{
    HI_UNF_TUNER_FE_DEPUNC_RATE_DEFAULT = 0 ,    /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_12      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_23      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_34      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_45      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_56      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_67      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_78      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_89      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_BUTT    ,        /**< */
}HI_UNF_TUNER_FE_DEPUNC_RATE_E ;

/*OFDM Mode, used in multi-carrier modulation*/
/*CNcomment:OFDM模式，用于多载波调制模式下*/
typedef enum hiUNF_TUNER_FE_FFT_E
{
    HI_UNF_TUNER_FE_FFT_DEFAULT = 0,   /**< */
    HI_UNF_TUNER_FE_FFT_1K ,           /**< */
    HI_UNF_TUNER_FE_FFT_2K ,           /**< */
    HI_UNF_TUNER_FE_FFT_4K ,           /**< */
    HI_UNF_TUNER_FE_FFT_8K ,           /**< */
    HI_UNF_TUNER_FE_FFT_16K ,           /**< */
    HI_UNF_TUNER_FE_FFT_32K ,           /**< */
    HI_UNF_TUNER_FE_FFT_BUTT ,         /**< */
}HI_UNF_TUNER_FE_FFT_E;

/*Hierarchical modulation mode, only used in DVB-T*/
/*CNcomment:仅用于DVB-T*/
typedef enum hiUNF_TUNER_FE_HIERARCHY_E
{
    HI_UNF_TUNER_FE_HIERARCHY_DEFAULT = 0 ,   /**< */
    HI_UNF_TUNER_FE_HIERARCHY_NO,             /**< */
    HI_UNF_TUNER_FE_HIERARCHY_ALHPA1,         /**< */
    HI_UNF_TUNER_FE_HIERARCHY_ALHPA2,         /**< */
    HI_UNF_TUNER_FE_HIERARCHY_ALHPA4,         /**< */
    HI_UNF_TUNER_FE_HIERARCHY_BUTT,           /**< */
}HI_UNF_TUNER_FE_HIERARCHY_E ;

/*TS Priority, only used in DVB-T*/
/*CNcomment:仅用于DVB-T*/
typedef enum hiUNF_TUNER_TS_PRIORITY_E
{
    HI_UNF_TUNER_TS_PRIORITY_NONE = 0 ,   /**< */
    HI_UNF_TUNER_TS_PRIORITY_HP,             /**< */
    HI_UNF_TUNER_TS_PRIORITY_LP,         /**< */
    HI_UNF_TUNER_TS_PRIORITY_BUTT,           /**< */
}HI_UNF_TUNER_TS_PRIORITY_E ;

/** CNcomment:仅用于DVB-T2*/
typedef enum hiUNF_TUNER_TER_MODE_E
{
    HI_UNF_TUNER_TER_MODE_BASE = 0,     /**< the channel is base mode*/         /**<CNcomment:通道中仅支持base信号*/
    HI_UNF_TUNER_TER_MODE_LITE,         /**< the channel is lite mode*/         /**<CNcomment:通道中需要支持lite信号*/
    HI_UNF_TUNER_TER_MODE_BUTT      /**<Invalid value*/            /**<CNcomment:非法边界值*/
} HI_UNF_TUNER_TER_MODE_E;

typedef struct  hiUNF_TER_CONNECT_PARA_S
{
    HI_U32          u32Freq     ;                    /**<freq in KHz */
    HI_U32          u32BandWidth    ;                /**<bandwidth in KHz */
    HI_UNF_MODULATION_TYPE_E enModType  ;            /**< */
    HI_BOOL                     bReverse ;          /**<Spectrum reverse mode*/  /**<CNcomment:频谱翻转处理方式*/
    HI_UNF_TUNER_TER_MODE_E     enChannelMode;      //dvb-t2
    HI_UNF_TUNER_TS_PRIORITY_E  enDVBTPrio;         //dvb-t
}HI_UNF_TER_CONNECT_PARA_S ;

#if 0
/*CNcomment:暂未使用*/
typedef struct  hiUNF_TUNER_TER_ATTR_S
{
    HI_UNF_TUNER_FE_DEPUNC_RATE_E   enHpCode ;      /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_E   enLpCode ;      /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_E    enGuardIntv ;   /**< */
    HI_UNF_TUNER_FE_FFT_E       enFftType ;         /**< */
    HI_UNF_TUNER_FE_HIERARCHY_E enHierType ;        /**< */

}HI_UNF_TUNER_TER_ATTR_S;
#endif

/**TS clock polarization*/
/*CNcomment:TS时钟极性*/
typedef enum hiUNF_TUNER_TSCLK_POLAR_E
{
    HI_UNF_TUNER_TSCLK_POLAR_FALLING,       /**<Falling edge*/      /**<CNcomment:下降沿*/
    HI_UNF_TUNER_TSCLK_POLAR_RISING,        /**<Rising edge*/       /**<CNcomment:上升沿*/
    HI_UNF_TUNER_TSCLK_POLAR_BUTT           /**<Invalid value*/     /**<CNcomment:非法边界值*/
} HI_UNF_TUNER_TSCLK_POLAR_E;

/**TS format*/
typedef enum hiUNF_TUNER_TS_FORMAT_E
{
    HI_UNF_TUNER_TS_FORMAT_TS,              /**<188*/
    HI_UNF_TUNER_TS_FORMAT_TSP,             /**<204*/
    HI_UNF_TUNER_TS_FORMAT_BUTT             /**<Invalid value*/     /**<CNcomment:非法边界值*/
} HI_UNF_TUNER_TS_FORMAT_E;

/**TS serial PIN*/
typedef enum hiUNF_TUNER_TS_SERIAL_PIN_E
{
    HI_UNF_TUNER_TS_SERIAL_PIN_0,           /**<Serial pin 0, default*/
    HI_UNF_TUNER_TS_SERIAL_PIN_7,           /**<Serial pin 7*/
    HI_UNF_TUNER_TS_SERIAL_PIN_BUTT         /**<Invalid value*/     /**<CNcomment:非法边界值*/
} HI_UNF_TUNER_TS_SERIAL_PIN_E;

/** Tuner RF AGC mode */
typedef enum hiUNF_TUNER_RFAGC_MODE_E
{
    /**< Inverted polarization, default.This setting is used for a tuner whose gain decreases with increased AGC voltage */
    HI_UNF_TUNER_RFAGC_INVERT, 

    /**< Normal polarization. This setting is used for a tuner whose gain increases with increased AGC voltage */
    HI_UNF_TUNER_RFAGC_NORMAL,
    HI_UNF_TUNER_RFAGC_BUTT
} HI_UNF_TUNER_RFAGC_MODE_E;

/** Tuner IQ spectrum mode */
typedef enum hiUNF_TUNER_IQSPECTRUM_MODE_E
{
    HI_UNF_TUNER_IQSPECTRUM_NORMAL,         /**<The received signal spectrum is not inverted */
    HI_UNF_TUNER_IQSPECTRUM_INVERT,         /**<The received signal spectrum is inverted */
    HI_UNF_TUNER_IQSPECTRUM_BUTT
} HI_UNF_TUNER_IQSPECTRUM_MODE_E;

/** DiSEqC Wave Mode */
typedef enum hiUNF_TUNER_DISEQCWAVE_MODE_E
{
    HI_UNF_TUNER_DISEQCWAVE_NORMAL,         /**<Waveform produced by demod */
    HI_UNF_TUNER_DISEQCWAVE_ENVELOPE,       /**<Waveform produced by LNB control device */
    HI_UNF_TUNER_DISEQCWAVE_BUTT
} HI_UNF_TUNER_DISEQCWAVE_MODE_E;

/** LNB power supply and control device */
typedef enum hiUNF_LNBCTRL_DEV_TYPE_E
{
    HI_UNF_LNBCTRL_DEV_TYPE_NONE,           /**<No LNB control device */
    HI_UNF_LNBCTRL_DEV_TYPE_MPS8125,        /**<MPS8125 */
    HI_UNF_LNBCTRL_DEV_TYPE_ISL9492,        /**<ISL9492 */
    HI_UNF_LNBCTRL_DEV_TYPE_BUTT
} HI_UNF_LNBCTRL_DEV_TYPE_E;

typedef struct  hiUNF_TUNER_SAT_ATTR_S
{
    HI_U32                         u32DemodClk;     /**<Demod reference clock frequency, KHz */
    HI_U16                         u16TunerMaxLPF;  /**<Tuner max LFP, MHz */
    HI_U16                         u16TunerI2CClk;  /**<Tuner I2C clock, kHz */
    HI_UNF_TUNER_RFAGC_MODE_E      enRFAGC;         /**<Tuner RF AGC mode */
    HI_UNF_TUNER_IQSPECTRUM_MODE_E enIQSpectrum;    /**<Tuner IQ spectrum mode */
    HI_UNF_TUNER_TSCLK_POLAR_E     enTSClkPolar;    /**<TS clock polarization */
    HI_UNF_TUNER_TS_FORMAT_E       enTSFormat;      /**<TS format */
    HI_UNF_TUNER_TS_SERIAL_PIN_E   enTSSerialPIN;   /**<TS serial PIN */
    HI_UNF_TUNER_DISEQCWAVE_MODE_E enDiSEqCWave;    /**<DiSEqC Wave Mode */
    HI_UNF_LNBCTRL_DEV_TYPE_E      enLNBCtrlDev;    /**<LNB power supply and control device */
    HI_U16                         u16LNBDevAddress;/**<LNB control device address */
} HI_UNF_TUNER_SAT_ATTR_S;

//typedef struct  hiUNF_TUNER_TER_ATTR_S
//{
//    HI_U32                         u32ResetGpioNo;  /**<Demod reset GPIO NO.*/
//} HI_UNF_TUNER_TER_ATTR_S;
/** Polarization type */
typedef enum hiUNF_TUNER_FE_POLARIZATION_E
{
    HI_UNF_TUNER_FE_POLARIZATION_H,         /**< Horizontal Polarization */
    HI_UNF_TUNER_FE_POLARIZATION_V,         /**< Vertical Polarization */
    HI_UNF_TUNER_FE_POLARIZATION_L,         /**< Left-hand circular Polarization */
    HI_UNF_TUNER_FE_POLARIZATION_R,         /**< Right-hand circular Polarization */
    HI_UNF_TUNER_FE_POLARIZATION_BUTT,      /**< Invalid */
} HI_UNF_TUNER_FE_POLARIZATION_E;

/** FEC Type */
typedef enum hiUNF_TUNER_FE_FECTYPE_E
{
    HI_UNF_TUNER_FE_DVBS,
    HI_UNF_TUNER_FE_DVBS2,
    HI_UNF_TUNER_FE_DIRECTV,
    HI_UNF_TUNER_FE_BUTT
} HI_UNF_TUNER_FE_FECTYPE_E;

/** FEC Rate */
typedef enum hiUNF_TUNER_FE_FECRATE_E
{
    HI_UNF_TUNER_FE_FEC_AUTO = 0,
    HI_UNF_TUNER_FE_FEC_1_2,
    HI_UNF_TUNER_FE_FEC_2_3,
    HI_UNF_TUNER_FE_FEC_3_4,
    HI_UNF_TUNER_FE_FEC_4_5,
    HI_UNF_TUNER_FE_FEC_5_6,
    HI_UNF_TUNER_FE_FEC_6_7,
    HI_UNF_TUNER_FE_FEC_7_8,
    HI_UNF_TUNER_FE_FEC_8_9,
    HI_UNF_TUNER_FE_FEC_9_10,
    HI_UNF_TUNER_FE_FEC_1_4,
    HI_UNF_TUNER_FE_FEC_1_3,
    HI_UNF_TUNER_FE_FEC_2_5,
    HI_UNF_TUNER_FE_FEC_3_5,
    HI_UNF_TUNER_FE_FECRATE_BUTT
} HI_UNF_TUNER_FE_FECRATE_E;

/** LNB type */
typedef enum hiUNF_TUNER_FE_FE_LNBTYPE_E
{
    HI_UNF_TUNER_FE_LNB_SINGLE_FREQUENCY,   /**<Single LO frequency */
    HI_UNF_TUNER_FE_LNB_DUAL_FREQUENCY,     /**<Dual LO frequency */
    HI_UNF_TUNER_FE_LNB_TYPE_BUTT
} HI_UNF_TUNER_FE_LNB_TYPE_E;

/** LNB band type */
typedef enum hiUNF_TUNER_FE_LNB_BAND_E
{
    HI_UNF_TUNER_FE_LNB_BAND_C,             /**<C */
    HI_UNF_TUNER_FE_LNB_BAND_KU,            /**<Ku */
    HI_UNF_TUNER_FE_LNB_BAND_BUTT
} HI_UNF_TUNER_FE_LNB_BAND_E;

/** LNB power control */
typedef enum hiUNF_TUNER_FE_LNB_POWER_E
{
    HI_UNF_TUNER_FE_LNB_POWER_OFF,          /**<LNB power off */
    HI_UNF_TUNER_FE_LNB_POWER_ON,           /**<LNB power auto, 13V/18V, default */
    HI_UNF_TUNER_FE_LNB_POWER_ENHANCED,     /**<LNB power auto, 14V/19V, some LNB control device can support.*/
    HI_UNF_TUNER_FE_LNB_POWER_BUTT
} HI_UNF_TUNER_FE_LNB_POWER_E;

/** LNB 22K tone status, for Ku band LNB */
typedef enum hiUNF_TUNER_FE_LNB_22K_E
{
    HI_UNF_TUNER_FE_LNB_22K_OFF,            /**<default */
    HI_UNF_TUNER_FE_LNB_22K_ON,
    HI_UNF_TUNER_FE_LNB_22K_BUTT
} HI_UNF_TUNER_FE_LNB_22K_E;

/** 0/12V switch */
typedef enum hiUNF_TUNER_SWITCH_0_12V_E
{
    HI_UNF_TUNER_SWITCH_0_12V_NONE,         /**< None, default */
    HI_UNF_TUNER_SWITCH_0_12V_0,            /**< 0V */
    HI_UNF_TUNER_SWITCH_0_12V_12,           /**< 12V */
    HI_UNF_TUNER_SWITCH_0_12V_BUTT
} HI_UNF_TUNER_SWITCH_0_12V_E;

/** 22KHz switch */
typedef enum hiUNF_TUNER_SWITCH_22K_E
{
    HI_UNF_TUNER_SWITCH_22K_NONE,           /**< None, default */
    HI_UNF_TUNER_SWITCH_22K_0,              /**< 0 */
    HI_UNF_TUNER_SWITCH_22K_22,             /**< 22KHz */
    HI_UNF_TUNER_SWITCH_22K_BUTT
} HI_UNF_TUNER_SWITCH_22K_E;

/** Tone burst switch */
typedef enum hiUNF_TUNER_SWITCH_TONEBURST_E
{
    HI_UNF_TUNER_SWITCH_TONEBURST_NONE,     /**< Don't send tone burst, default */
    HI_UNF_TUNER_SWITCH_TONEBURST_0,        /**< Tone burst 0 */
    HI_UNF_TUNER_SWITCH_TONEBURST_1,        /**< Tone burst 1 */
    HI_UNF_TUNER_SWITCH_TONEBURST_BUTT
} HI_UNF_TUNER_SWITCH_TONEBURST_E;

/**Structure of the satellite transmission signal.*/
/**CNcomment:定义Satellite传输信号*/
typedef struct  hiUNF_SAT_CONNECT_PARA_S
{
    HI_U32                         u32Freq;         /**<Downlink frequency, in kHz*/ /**<CNcomment:下行频率，单位：kHz*/
    HI_U32                         u32SymbolRate;   /**<Symbol rate, in bit/s*/      /**<CNcomment:符号率，单位bps */
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar;         /**<Polarization type*/          /**<CNcomment:极化方式*/
	HI_U32                         u32ScrambleValue; /**<initial scrambling code,range 0~262141,when value is not 0,signal is special,
    and the value must be telled by signal owner.default 0 must be configed when TP signal is not special*/
} HI_UNF_SAT_CONNECT_PARA_S;

/**Structure of the satellite transmission signal's detailed information.*/
/**CNcomment:卫星信号详细信息*/
typedef struct  hiUNF_TUNER_SAT_SIGNALINFO_S
{
    HI_U32                         u32Freq;         /**<Downlink frequency, in kHz*/ /**<CNcomment:下行频率，单位：kHz*/
    HI_U32                         u32SymbolRate;   /**<Symbol rate, in bit/s*/      /**<CNcomment:符号率，单位bps */
    HI_UNF_MODULATION_TYPE_E       enModType;       /**<Modulation type*/            /**<CNcomment:调制方式*/
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar;         /**<Polarization type*/          /**<CNcomment:极化方式*/
    HI_UNF_TUNER_FE_FECTYPE_E      enSATType;       /**<Saterllite standard, DVB-S/S2/DIRECTV*/   /**<CNcomment:卫星标准， 支持DVB-S/S2/DIRECTV*/
    HI_UNF_TUNER_FE_FECRATE_E      enFECRate;       /**<FEC rate*/                   /**<CNcomment:前向纠错码率*/
} HI_UNF_TUNER_SAT_SIGNALINFO_S;

typedef enum hiUNF_TUNER_T2_PLP_TYPE_E
{   
    HI_UNF_TUNER_T2_PLP_TYPE_COM=0, 
    HI_UNF_TUNER_T2_PLP_TYPE_DAT1,  
    HI_UNF_TUNER_T2_PLP_TYPE_DAT2,
    HI_UNF_TUNER_T2_PLP_TYPE_BUTT   
} HI_UNF_TUNER_T2_PLP_TYPE_E;

/**Structure of the terrestrial transmission signal's detailed information.*/
/**CNcomment:地面信号详细信息*/
typedef struct  hiUNF_TUNER_TER_SIGNALINFO_S
{
    HI_U32                         u32Freq;         /**<Frequency, in kHz*/ /**<CNcomment:频率，单位：kHz*/
    HI_U32                         u32BandWidth;   /**<Band width, in KHz*/      /**<CNcomment:带宽，单位KHz */
    HI_UNF_MODULATION_TYPE_E       enModType;       /**<Modulation type*/            /**<CNcomment:调制方式*/
    HI_UNF_TUNER_FE_FECRATE_E      enFECRate;       /**<FEC rate*/                   /**<CNcomment:前向纠错码率*/
    HI_UNF_TUNER_FE_GUARD_INTV_E enGuardIntv;
    HI_UNF_TUNER_FE_FFT_E enFFTMode;
    HI_UNF_TUNER_FE_HIERARCHY_E enHierMod; /**<Hierarchical Modulation and alpha, only used in DVB-T*/
    HI_UNF_TUNER_TS_PRIORITY_E enTsPriority; /**<The TS priority, only used in DVB-T*/
} HI_UNF_TUNER_TER_SIGNALINFO_S;
/**CNcomment:TUNER属性*/
typedef struct hiUNF_TUNER_SIGNALINFO_S
{
    HI_UNF_TUNER_SIG_TYPE_E enSigType;              /**<Signal transmission type*/  /**<CNcomment:信号类型*/

    union
    {
        HI_UNF_TUNER_SAT_SIGNALINFO_S stSat;        /**<Signal info of satellite*/  /**<CNcomment:卫星信号信息*/
        HI_UNF_TUNER_TER_SIGNALINFO_S stTer;        /**<Signal info of terrestrial*/  /**<CNcomment:地面信号信息*/
    } unSignalInfo;
} HI_UNF_TUNER_SIGNALINFO_S;

/** LNB configurating parameters */
typedef struct hiUNF_TUNER_FE_LNB_CONFIG_S
{
    HI_UNF_TUNER_FE_LNB_TYPE_E  enLNBType;      /**<LNB type*/                              /**<CNcomment:LNB类型*/
    HI_U32                      u32LowLO;       /**< Low Local Oscillator Frequency, MHz */ /**<CNcomment:LNB低本振频率，单位MHz*/
    HI_U32                      u32HighLO;      /**< High Local Oscillator Frequency, MHz*/ /**<CNcomment:LNB高本振频率，单位MHz*/
    HI_UNF_TUNER_FE_LNB_BAND_E  enLNBBand;      /**< LNB band, C or Ku */                   /**<CNcomment:LNB波段：C或Ku*/
} HI_UNF_TUNER_FE_LNB_CONFIG_S;

/**CNcomment:TUNER属性*/
typedef struct  hiTUNER_ATTR_S
{
    HI_UNF_TUNER_SIG_TYPE_E     enSigType ;         /**<Signal type*/                             /**<CNcomment:信号类型*/
    HI_UNF_TUNER_DEV_TYPE_E enTunerDevType;     /**<Tuner type*/                              /**<CNcomment:TUNER类型*/
    HI_U32 u32TunerAddr;                                      /*The i2c address of tuner, if demod type is 3130I,3130E or J83B, you cannot set it*/
    HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType;     /**<QAM type*/                                /**<CNcomment:QAM类型*/
    HI_U32 u32DemodAddr;                                   /*The i2c address of demod, if demod type is 3130I,3130E or J83B, you cannot set it*/
    HI_UNF_TUNER_OUPUT_MODE_E   enOutputMode ;      /**<Output mode of transport streams (TSs)*/  /**<CNcomment:TS流输出模式*/
    HI_U8   enI2cChannel;       /**<I2C channel used by the tuner*/           /**<CNcomment:TUNER使用的I2C通道*/
    HI_U32                         u32ResetGpioNo;  /**<Demod reset GPIO NO.*/
    //union
    //{
    //    HI_UNF_TUNER_TER_ATTR_S stTer ;         /**<Attributes of the terrestrial signal. */  /**<CNcomment:地面信号属性*/
    //    HI_UNF_TUNER_SAT_ATTR_S stSat;          /**<Attributes of the satellite signal.*/     /**<CNcomment:卫星信号属性*/
    //} unTunerAttr;
} HI_UNF_TUNER_ATTR_S ;

/**Frequency locking parameters of the tuner*/
/**CNcomment:TUNER锁频参数*/
typedef struct  hiUNF_TUNER_CONNECT_PARA_S
{
    HI_UNF_TUNER_SIG_TYPE_E enSigType ;        /**<Signal type*/                 /**<CNcomment:信号类型*/

    union
    {
        HI_UNF_CAB_CONNECT_PARA_S   stCab ;   /**<Cable transmission signal*/    /**<CNcomment:CABLE传输信号*/
        HI_UNF_TER_CONNECT_PARA_S stTer ;       /**<Terrestrial transmission signal*/ /**<CNcomment:地面传输信号*/
        HI_UNF_SAT_CONNECT_PARA_S stSat;        /**<Satellite transmission signal*/   /**<CNcomment:卫星传输信号*/
    } unConnectPara;
} HI_UNF_TUNER_CONNECT_PARA_S ;

/**Frequency locking status and parameters of the tuner*/
/**CNcomment:TUNER锁频状态和锁频参数*/
typedef struct  hiUNF_TUNER_STATUS_S
{
    HI_UNF_TUNER_LOCK_STATUS_E  enLockStatus ;  /**<Frequency locking status*/               /**<CNcomment:锁频状态*/
    HI_UNF_TUNER_CONNECT_PARA_S stConnectPara;  /**<Actual frequency locking parameters*/    /**<CNcomment:实际锁频参数*/
}HI_UNF_TUNER_STATUS_S ;
/**Tuner blind scan type*/
/**CNcomment:TUNER盲扫方式*/
typedef enum hiUNF_TUNER_BLINDSCAN_MODE_E
{
    HI_UNF_TUNER_BLINDSCAN_MODE_AUTO = 0,       /**<Blind scan automatically*/  /**<CNcomment:自动扫描*/
    HI_UNF_TUNER_BLINDSCAN_MODE_MANUAL,         /**<Blind scan manually*/       /**<CNcomment:手动扫描*/
    HI_UNF_TUNER_BLINDSCAN_MODE_BUTT
} HI_UNF_TUNER_BLINDSCAN_MODE_E;

/**Definition of blind scan event type*/
/**CNcomment:TUNER盲扫事件*/
typedef enum hiUNF_TUNER_BLINDSCAN_EVT_E
{
    HI_UNF_TUNER_BLINDSCAN_EVT_STATUS,          /**<New status*/        /**<CNcomment:状态变化*/
    HI_UNF_TUNER_BLINDSCAN_EVT_PROGRESS,        /**<New Porgress */     /**<CNcomment:进度变化*/
    HI_UNF_TUNER_BLINDSCAN_EVT_NEWRESULT,       /**<Find new channel*/  /**<CNcomment:新频点*/
    HI_UNF_TUNER_BLINDSCAN_EVT_BUTT
} HI_UNF_TUNER_BLINDSCAN_EVT_E;

/**Definition of tuner blind scan status*/
/**CNcomment:TUNER盲扫状态*/
typedef enum hiUNF_TUNER_BLINDSCAN_STATUS_E
{
    HI_UNF_TUNER_BLINDSCAN_STATUS_IDLE,         /**<Idel*/              /**<空闲*/
    HI_UNF_TUNER_BLINDSCAN_STATUS_SCANNING,     /**<Scanning*/          /**<扫描中*/
    HI_UNF_TUNER_BLINDSCAN_STATUS_FINISH,       /**<Finish*/            /**<成功完成*/
    HI_UNF_TUNER_BLINDSCAN_STATUS_QUIT,         /**<User quit*/         /**<用户退出*/
    HI_UNF_TUNER_BLINDSCAN_STATUS_FAIL,         /**<Scan fail*/         /**<扫描失败*/
    HI_UNF_TUNER_BLINDSCAN_STATUS_BUTT
} HI_UNF_TUNER_BLINDSCAN_STATUS_E;

/**Structure of satellite TP*/
/**CNcomment:TUNER扫出TP信息*/
typedef struct  hiUNF_TUNER_SAT_TPINFO_S
{
    HI_U32                         u32Freq;         /**<Downlink frequency, in kHz*/ /**<CNcomment:下行频率，单位：kHz*/
    HI_U32                         u32SymbolRate;   /**<Symbol rate, in bit/s*/      /**<CNcomment:符号率，单位bps */
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar;         /**<Polarization type*/          /**<CNcomment:极化方式*/
    //HI_U8 agc_h8;                                   /**<*The high 8 bits of AGC*/    /**<CNcomment:AGC控制字高8bit*/
    HI_U8 cbs_reliablity;                           /**<TP reliability*/             /**<CNcomment:TP的可靠度*/
} HI_UNF_TUNER_SAT_TPINFO_S;

#if 0
/**Result of tuner blind scan */
/**CNcomment:TUNER盲扫结果信息*/
typedef union hiUNF_TUNER_BLINDSCAN_RESULT_U
{
    HI_UNF_TUNER_SAT_TPINFO_S stSat;                /**<Information of new channel*/ /**<CNcomment:新频点信息*/
} HI_UNF_TUNER_BLINDSCAN_RESULT_U;
#endif

/**Notify structure of tuner blind scan */
/**CNcomment:TUNER盲扫通知信息*/
typedef union hiUNF_TUNER_BLINDSCAN_NOTIFY_U
{
    HI_UNF_TUNER_BLINDSCAN_STATUS_E* penStatus;             /**<Scanning status*/
    HI_U16*                          pu16ProgressPercent;   /**<Scanning progress*/
    HI_UNF_TUNER_SAT_TPINFO_S* pstResult;             /**<Scanning result*/
} HI_UNF_TUNER_BLINDSCAN_NOTIFY_U;

/**Parameter of the satellite tuner blind scan */
/**CNcomment:卫星TUNER盲扫参数*/
typedef struct hiUNF_TUNER_SAT_BLINDSCAN_PARA_S
{
    /**<LNB Polarization type, only take effect in manual blind scan mode*/
    /**<CNcomment:LNB极化方式，自动扫描模式设置无效*/
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar;
    
    /**<LNB 22K signal status, for Ku band LNB which has dual LO, 22K ON will select high LO and 22K off select low LO,
        only take effect in manual blind scan mode*/
    /**<CNcomment:LNB 22K状态，对于Ku波段双本振LNB，ON选择高本振，OFF选择低本振，自动扫描模式设置无效*/
    HI_UNF_TUNER_FE_LNB_22K_E      enLNB22K;

    /**<Blind scan start IF, in kHz, only take effect in manual blind scan mode */
    /**<CNcomment:盲扫起始频率(中频)，单位：kHz，自动扫描模式设置无效*/
    HI_U32                         u32StartFreq;   

    /**<Blind scan stop IF, in kHz, only take effect in manual blind scan mode */
    /**<CNcomment:盲扫结束频率(中频)，单位：kHz，自动扫描模式设置无效*/
    HI_U32                         u32StopFreq; 

    /**<The execution of the blind scan may change the 13/18V or 22K status. 
        If you use any DiSEqC device which need send command when 13/18V or 22K status change,
        you should registe a callback here. Otherwise, you can set NULL here.*/
    /**<CNcomment:盲扫过程可能会切换极化方式和22K，如果你用了某些DiSEqC设备需要设置13/18V和22K的，
        请注册这个回调，如果没有用，请可传NULL */
    HI_VOID (*pfnDISEQCSet)(HI_U32 u32TunerId, HI_UNF_TUNER_FE_POLARIZATION_E enPolar,
                            HI_UNF_TUNER_FE_LNB_22K_E enLNB22K);

    /**<Callback when scan status change, scan progress change or find new channel.*/
    /**<CNcomment:扫描状态或进度百分比发生变化时、发现新的频点时回调*/
    HI_VOID (*pfnEVTNotify)(HI_U32 u32TunerId, HI_UNF_TUNER_BLINDSCAN_EVT_E enEVT, HI_UNF_TUNER_BLINDSCAN_NOTIFY_U * punNotify);
} HI_UNF_TUNER_SAT_BLINDSCAN_PARA_S;

/**Parameter of the tuner blind scan */
/**CNcomment:TUNER盲扫参数*/
typedef struct hiUNF_TUNER_BLINDSCAN_PARA_S
{
    HI_UNF_TUNER_BLINDSCAN_MODE_E enMode;
    union
    {
        HI_UNF_TUNER_SAT_BLINDSCAN_PARA_S stSat;
    } unScanPara;
} HI_UNF_TUNER_BLINDSCAN_PARA_S;


/**DiSEqC Level*/
typedef enum hiUNF_TUNER_DISEQC_LEVEL_E
{
    HI_UNF_TUNER_DISEQC_LEVEL_1_X,      /**<1.x, one way*/                  /**<1.x，单向 */
    HI_UNF_TUNER_DISEQC_LEVEL_2_X,      /**<2.x, two way, support reply*/   /**<2.x，双向，支持Reply*/
    HI_UNF_TUNER_DISEQC_LEVEL_BUTT
} HI_UNF_TUNER_DISEQC_LEVEL_E;

/**Receive status of DiSEqC reply massage */
/**CNcomment:DiSEqC消息接收状态*/
typedef enum hiUNF_TUNER_DISEQC_RECV_STATUS_E
{
    HI_UNF_TUNER_DISEQC_RECV_OK,        /**<Receive successfully*/          /**<接收成功*/
    HI_UNF_TUNER_DISEQC_RECV_UNSUPPORT, /**<Device don't support reply*/    /**<设备不支持回传*/    
    HI_UNF_TUNER_DISEQC_RECV_TIMEOUT,   /**<Receive timeout*/               /**<接收超时*/
    HI_UNF_TUNER_DISEQC_RECV_ERROR,     /**<Receive fail*/                  /**<接收出错*/
    HI_UNF_TUNER_DISEQC_RECV_BUTT
} HI_UNF_TUNER_DISEQC_RECV_STATUS_E;

/**Structure of the DiSEqC send massage */
/**CNcomment:DiSEqC发送消息结构*/
typedef struct hiUNF_TUNER_DISEQC_SENDMSG_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E     enLevel;        /**<Device device level*/
    HI_UNF_TUNER_SWITCH_TONEBURST_E enToneBurst;    /**<Tone Burst */
    HI_U8                           au8Msg[HI_UNF_DISEQC_MSG_MAX_LENGTH]; /**<Message data*/
    HI_U8                           u8Length;       /**<Message length*/
    HI_U8                           u8RepeatTimes;  /**<Message repeat times*/
} HI_UNF_TUNER_DISEQC_SENDMSG_S;

/**Structure of the DiSEqC reply massage */
/**CNcomment:DiSEqC接收消息结构*/
typedef struct hiUNF_TUNER_DISEQC_RECVMSG_S
{
    HI_UNF_TUNER_DISEQC_RECV_STATUS_E enStatus;     /**<Recieve status*/
    HI_U8                             au8Msg[HI_UNF_DISEQC_MSG_MAX_LENGTH]; /**<Recieve message data*/
    HI_U8                             u8Length;     /**<Recieve message length*/
} HI_UNF_TUNER_DISEQC_RECVMSG_S;

//#ifdef HI_BOOT_DISEQC_SUPPORT
/**DiSEqC Switch port */
typedef enum hiUNF_TUNER_DISEQC_SWITCH_PORT_E
{
    HI_UNF_TUNER_DISEQC_SWITCH_NONE = 0,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_1,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_2,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_3,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_4,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_5,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_6,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_7,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_8,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_9,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_10,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_11,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_12,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_13,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_14,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_15,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_16,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_BUTT
} HI_UNF_TUNER_DISEQC_SWITCH_PORT_E;

/**Parameter for DiSEqC 1.0/2.0 switch
   Some DiSEqC device need set polarization(13/18V) and 22K, you can set them here. */
/**CNcomment:DiSEqC 1.0/2.0 开关参数
   有些DiSEqC设备需要设置极化方式和22K的，如果用了这种设备，需要在这里设置 */
typedef struct hiUNF_TUNER_DISEQC_SWITCH4PORT_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E       enLevel;  /**<DiSEqC device level*/
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_E enPort;   /**<DiSEqC switch port*/
    HI_UNF_TUNER_FE_POLARIZATION_E    enPolar;  /**<Polarization type */
    HI_UNF_TUNER_FE_LNB_22K_E         enLNB22K; /**<22K status*/
} HI_UNF_TUNER_DISEQC_SWITCH4PORT_S;

/**Parameter for DiSEqC 1.1/2.1 switch */
/**CNcomment:DiSEqC 1.1/2.1 开关参数 */
typedef struct hiUNF_TUNER_DISEQC_SWITCH16PORT_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E       enLevel;  /**<DiSEqC device level*/
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_E enPort;   /**<DiSEqC switch port*/
} HI_UNF_TUNER_DISEQC_SWITCH16PORT_S;

/**DiSEqC motor limit setting*/
/**CNcomment:DiSEqC马达极限设置 */
typedef enum hiUNF_TUNER_DISEQC_LIMIT_E
{
    HI_UNF_TUNER_DISEQC_LIMIT_OFF,              /**<Disable Limits*/
    HI_UNF_TUNER_DISEQC_LIMIT_EAST,             /**<Set East Limit*/
    HI_UNF_TUNER_DISEQC_LIMIT_WEST,             /**<Set West Limit*/
    HI_UNF_TUNER_DISEQC_LIMIT_BUTT
} HI_UNF_TUNER_DISEQC_LIMIT_E;

/**Difinition of DiSEqC motor move direction*/
/**CNcomment:DiSEqC马达移动方向 */
typedef enum hiUNF_TUNER_DISEQC_MOVE_DIR_E
{
    HI_UNF_TUNER_DISEQC_MOVE_DIR_EAST,          /**<Move east*/
    HI_UNF_TUNER_DISEQC_MOVE_DIR_WEST,          /**<Move west*/
    HI_UNF_TUNER_DISEQC_MOVE_DIR_BUTT
} HI_UNF_TUNER_DISEQC_MOVE_DIR_E;

/**Difinition of DiSEqC motor move type*/
/**CNcomment:DiSEqC马达移动方式*/
typedef enum hiUNF_TUNER_DISEQC_MOVE_TYPE_E
{
    HI_UNF_TUNER_DISEQC_MOVE_STEP_SLOW,         /**<1 step one time, default*/
    HI_UNF_TUNER_DISEQC_MOVE_STEP_FAST,         /**<5 step one time*/
    HI_UNF_TUNER_DISEQC_MOVE_CONTINUE,          /**<Continuous moving*/
    HI_UNF_TUNER_DISEQC_MOVE_TYPE_BUTT
} HI_UNF_TUNER_DISEQC_MOVE_TYPE_E;

/**Parameter for DiSEqC motor store position*/
/**CNcomment:天线存储位置参数*/
typedef struct hiUNF_TUNER_DISEQC_POSITION_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E enLevel;        /**<DiSEqC device level*/
    HI_U32                      u32Pos;         /**<Index of position, 0-255*/
} HI_UNF_TUNER_DISEQC_POSITION_S;

/**Parameter for DiSEqC motor limit setting*/
/**CNcomment:天线Limit设置参数*/
typedef struct hiUNF_TUNER_DISEQC_LIMIT_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E enLevel;        /**<DiSEqC device level*/
    HI_UNF_TUNER_DISEQC_LIMIT_E enLimit;        /**<Limit setting*/
} HI_UNF_TUNER_DISEQC_LIMIT_S;

/**Parameter for DiSEqC motor moving*/
/**CNcomment:DiSEqC马达移动参数*/
typedef struct hiUNF_TUNER_DISEQC_MOVE_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E     enLevel;    /**<DiSEqC device level*/
    HI_UNF_TUNER_DISEQC_MOVE_DIR_E  enDir;      /**<Moving direction */
    HI_UNF_TUNER_DISEQC_MOVE_TYPE_E enType;     /**<Moving type */
} HI_UNF_TUNER_DISEQC_MOVE_S;

/**Parameter for DiSEqC motor recalculate*/
/**CNcomment:DiSEqC天线重计算参数*/
typedef struct hiUNF_TUNER_DISEQC_RECALCULATE_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E enLevel;        /**<DiSEqC device level*/
    HI_U8                       u8Para1;        /**<Parameter 1 */
    HI_U8                       u8Para2;        /**<Parameter 2 */
    HI_U8                       u8Para3;        /**<Parameter 3 */
    HI_U8                       u8Reserve;      /**<Reserve */
} HI_UNF_TUNER_DISEQC_RECALCULATE_S;

/**Parameter for USALS*/
/**CNcomment:USALS 参数*/
typedef struct hiUNF_TUNER_DISEQC_USALS_PARA_S
{
    HI_U16 u16LocalLongitude;   /**<local longitude, is 10*longitude, in param, E:0-1800, W:1800-3600(3600-longtitude) */
    HI_U16 u16LocalLatitude;    /**<local latitude, is 10*latitude, in param N:0-900, S:900-1800(1800-latitude)*/
    HI_U16 u16SatLongitude;     /**<sat longitude, is 10*longitude, in param, E:0-1800, W:1800-3600(3600-longtitude)  */
    HI_U16 u16Angular;          /**<calculate result, out param */
} HI_UNF_TUNER_DISEQC_USALS_PARA_S;

/**Parameter for USALS goto angular*/
/**CNcomment:USALS角度参数*/
typedef struct hiUNF_TUNER_DISEQC_USALS_ANGULAR_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E enLevel;        /**<DiSEqC device level*/
    HI_U16                      u16Angular;     /**<Angular, calculated by HI_UNF_TUNER_DISEQC_CalcAngular()*/
} HI_UNF_TUNER_DISEQC_USALS_ANGULAR_S;
//#endif /* HI_BOOT_DISEQC_SUPPORT */

/** @} */  /** <!-- ==== Structure Definition end ==== */


/******************************* API Declaration *****************************/
/** \addtogroup      H_1_4_10 */
/** @{ */  /** <!-- [TUNER] */
/*---TUNER---*/
/** 
\brief Initializes the tuner.
CNcomment:\brief 初始化TUNER设备。

\attention \n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:支持重复初始化TUNER设备，此时直接返回成功。
\param N/A                                    CNcomment:无
\retval ::HI_SUCCESS Success                  CNcomment:成功
\retval ::HI_FAILURE  Calling this API fails. CNcomment:API系统调用失败

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Init(HI_VOID);


/** 
\brief Deinitializes the tuner.
CNcomment:\brief 去初始化TUNER设备。

\attention \n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:支持重复去初始化TUNER设备，此时直接返回成功。
\param  N/A                    CNcomment:无
\retval ::HI_SUCCESS Success   CNcomment:成功
\retval ::HI_FAILURE  Calling this API fails. CNcomment:API系统调用失败

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DeInit(HI_VOID);


/**
\brief  Obtains the default attributes of the tuner. 
CNcomment:\brief  获取TUNER的默认属性。
\attention \n
This API is available only after the tuner is initialized.
CNcomment:此接口在TUNER设备初始化后才能使用。
\param[in] u32tunerId   tuner port ID. The port ID can be 0-2.             CNcomment:TUNER端口号，取值为0-2。
\param[out] pstTunerAttr The default attributes of the tuner are returned. CNcomment:返回当前TUNER的默认属性。
\retval ::HI_SUCCESS Success                                               CNcomment: 成功
\retval ::HI_FAILURE Calling this API fails.                               CNcomment:API系统调用失败

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetDeftAttr(HI_U32  u32tunerId , HI_UNF_TUNER_ATTR_S *pstTunerAttr );


/** 
\brief Sets the current attributes of the tuner.
CNcomment:\brief 设置TUNER当前属性。
\attention \n
N/A
\param[in] u32tunerId   tuner port ID. The port ID can be 0-2.      CNcomment:TUNER端口号，取值为0-2
\param[in] pstTunerAttr tuner attributes                            CNcomment:TUNER的属性。
\retval ::HI_SUCCESS Success                                        CNcomment:成功
\retval ::HI_FAILURE  Calling this API fails.                       CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_SetAttr(HI_U32  u32tunerId , const HI_UNF_TUNER_ATTR_S *pstTunerAttr );

/** 
\brief While using satellite channel, sets the current attributes of the tuner.
CNcomment:\brief 设置卫星TUNER附加属性。CNend
\attention \n
N/A
\param[in] u32tunerId   tuner port ID. The port ID can be 0-2.      CNcomment:卫星TUNER端口号，取值为0-2 CNend
\param[in] pstSatTunerAttr tuner attributes                         CNcomment:卫星TUNER的附加属性。CNend
\retval ::HI_SUCCESS Success                                        CNcomment:成功 CNend
\retval ::HI_FAILURE  Calling this API fails.                       CNcomment:API系统调用失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_SetSatAttr(HI_U32	u32tunerId , const HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr);

/** 
\brief Obtains the attributes of the tuner.
CNcomment:\brief 获取TUNER的属性。

\attention \n
This API is available only after the tuner is initialized.
CNcomment:此接口在TUNER设备初始化后才能使用。

\param[in] u32tunerId  tuner port ID. The port ID can be 0-2.                CNcomment:TUNER端口号，取值为0-2
\param[out] pstTunerAttr   The current attributes of the tuner are returned. CNcomment:返回当前TUNER的属性。
\retval ::HI_SUCCESS Success                                                 CNcomment:成功
\retval ::HI_FAILURE  Calling this API fails.                                CNcomment: API系统调用失败

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetAttr(HI_U32  u32tunerId , HI_UNF_TUNER_ATTR_S *pstTunerAttr );


/** 
\brief Starts the tuner.
CNcomment:\brief 打开TUNER设备。

\attention \n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:支持重复打开TUNER设备。此时，直接返回成功。

\param[in] u32tunerId   tuner port ID. The port ID can be 0-2.      CNcomment:TUNER端口号，取值为0-2
\retval ::HI_SUCCESS Success                                        CNcomment:成功
\retval ::HI_FAILURE Calling this API fails.                        CNcomment:API系统调用失败

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Open (HI_U32    u32tunerId);


/** 
\brief Stops the tuner.
CNcomment:\brief 关闭TUNER设备。

\attention \n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:支持重复关闭TUNER设备。此时，直接返回成功。

\param[in] u32tunerId  tuner port ID. The port ID can be 0-2.     CNcomment:TUNER端口号，取值为0-2 
\retval ::HI_SUCCESS Success                                      CNcomment:成功
\retval ::HI_FAILURE  Calling this API fails.                     CNcomment:API系统调用失败

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Close(HI_U32    u32tunerId);


/** 
\brief Locks the frequency of the tuner.
CNcomment:\brief TUNER锁频。
\attention \n
N/A
\param[in] u32tunerId  tuner port ID. The port ID can be 0-2.             CNcomment:TUNER端口号，取值为0-2
\param[in] pstConnectPara   Information about the frequency of the tuner  CNcomment:TUNER的频点信息。
\param[in] u32TimeOut  Wait timeout (in ms) when the frequency of the tuner is locked. The value 0 indicates no wait, and any other value indicates the maximum wait period.  CNcomment:TUNER的锁定等待超时时间，0为不等待，其他为最长等待时间，单位ms。
\retval ::HI_SUCCESS Success                                              CNcomment:成功
\retval ::HI_FAILURE  Calling this API fails.                             CNcomment:API系统调用失败

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Connect(HI_U32  u32tunerId , const HI_UNF_TUNER_CONNECT_PARA_S  *pstConnectPara,HI_U32 u32TimeOut);


/**
\brief  set TS out.CNcomment:设置TS输出
\attention \n
\Please call this API to set TS out. \n
\There are 12 signals in ts interface, ts_dat[0..7], ts_sync, ts_vld, ts_err, ts_clk.\n 
\There're 12 pins in HI3136 chip, and each pin can be set to output ts_dat[0..7], ts_sync,\n
\ts_vld or ts_err, but ts_clk is binded to one pin, cannot be setted.\n
CNcomment:ts接口共有12根信号线，分别是ts_dat[0..7], ts_sync, ts_vld, ts_err, ts_clk，HI3136芯片上有12个ts管脚，每个管脚可以\n
配置成除ts_clk之外的任意一个管脚，ts_clk是固定在HI3136芯片上的，不支持配置。

\param[in] u32tunerId  tuner port ID. The port ID can be 0-2.             CNcomment:TUNER端口号，取值为0-2
\param[in] pstTSOUT   pointer of ts pin.                                  CNcomment:指针类型，ts管脚定义，请参见::HI_UNF_TUNER_TSOUT_SET_S
\retval ::HI_SUCCESS                                                      CNcomment: success.成功
\retval ::HI_FAILURE  Calling this API fails.                             CNcomment:API系统调用失败
\see \n
none.CNcomment:无
*/
HI_S32 HI_UNF_TUNER_SetTSOUT(HI_U32 u32TunerId, HI_UNF_TUNER_TSOUT_SET_S *pstTSOUT);

/** 
\brief Obtains the frequency locking status and parameters of the tuner.
CNcomment:\brief 获取TUNER锁频状态和锁频参数。
\attention \n
N/A
\param[in] u32tunerId tuner port ID. The port ID can be 0-2.             CNcomment:TUNER端口号，取值为0-2
\param[in] u32TimeOut frequency locking timeout, unit: ms, frequency locking time is related to strength of signal CNcomment:锁频的超时时间，单位是毫秒，锁频时间和信号强度有关系。
               if you want to get the state of locking, the u32TimeOut should be set 100ms at least;
               if the u32TimeOut is 0, just config register without locking state, return HI_SUCCESS
               CNcomment: 如果想得到锁定状态，锁频超时最小设置为100ms；
               CNcomment: 如果超时时间为0，只配置寄存器，不判定锁定状态，返回HI_SUCCESS。
\param[out] pstTunerStatus: The current frequency locking status and parameters of the tuner are returned. Note: This parameter is valid only when HI_SUCCESS is returned.  CNcomment: 返回当前TUNER的锁频状态和锁频参数。 注意：此参数只在函数返回成功的情况下才有意义。
\retval ::HI_SUCCESS Success                    CNcomment:成功
\retval ::HI_FAILURE  Calling this API fails.  CNcomment:API系统调用失败

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetStatus(HI_U32    u32tunerId , HI_UNF_TUNER_STATUS_S  *pstTunerStatus);


/** 
\brief Obtains the current bit error rate (BER) of the tuner. The BER is expressed by using the scientific notation.
CNcomment:\brief 获取当前TUNER 的误码率，用科学计数法表示。
\attention \n
N/A
\param[in] u32tunerId tuner port ID. The port ID can be 0-2.             CNcomment:TUNER端口号，取值为0-2
\param[out] pu32BER  Pointer to the current BER of the tuner The error bit rate consists of three elements. Their definitions are as follows:
                      Their definitions are as follows:
                    pu32BER[0]: integral part of the base number of the BER
                    pu32BER[1]: decimal part of the base number of the BER x 1000
                    pu32BER[2]: absolute value of the exponential of the BER
                    For example, if the BER is 2.156E-7, the values of the three elements are
                    2, 156, and 7 respectively.
                    The three elements are valid only when HI_SUCCESS is returned.
                    Otherwise, the application layer sets a large value as required, for example, 0.5 (the values of the three elements are 5, 0, and 1 respectively).
                         0.5 (the values of the three elements are 5, 0, and 1 respectively).
CNcomment:\param[out] pu32BER  指向当前TUNER 误码率的指针。该指针指向一个包含三个元素的数组，三个元素含义如下:
CNcomment:                    pu32BER[0]:误码率底数的整数部分
CNcomment:                    pu32BER[1]:误码率底数的小数部分乘以1000
CNcomment:                    pu32BER[2]:误码率指数部分取绝对值
CNcomment:                    例如:误码率为2.156E-7，那么三个元素的取值分别为
CNcomment:                    2、156和7
CNcomment:                    这三个元素只在函数返回成功的情况下才有意义，
CNcomment:                    否则应用层可根据实际情况自行设置一个较大值(比如0.5，即三个元素分别为5、0、1)。
\retval ::HI_SUCCESS Success                  CNcomment:成功
\retval ::HI_FAILURE Calling this API fails.  CNcomment:API系统调用失败

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetBER(HI_U32   u32tunerId , HI_U32 *pu32BER);


/** 
\brief Obtains the current signal-to-noise ratio (SNR) of the tuner.
CNcomment:\brief 获取当前TUNER 的信噪比。
\attention \n
N/A
\param[in] u32tunerId  tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[out] pu32SNR  Pointer to the current SNR of the tuner
                    The value of *pu32SNR ranges from 22 to 51.
                    This parameter is valid only when HI_SUCCESS is returned.
                   Otherwise, the application layer sets the parameter to a small value.
CNcomment:\param[out] pu32SNR  指向当前TUNER信噪比的指针。
CNcomment:                    *pu32SNR取值范围为22 ~ 51
CNcomment:                    此参数只在函数返回成功的情况下才有意义，
CNcomment:                    否则，应用层可将此值设置为最小值。
\retval ::HI_SUCCESS Success                   CNcomment:成功
\retval ::HI_FAILURE  Calling this API fails.  CNcomment:API系统调用失败

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetSNR(HI_U32   u32tunerId , HI_U32 *pu32SNR );             /* range : 0-255  */


/** 
\brief Obtains the current signal strength of the tuner.
CNcomment:\brief 获取当前TUNER的信号强度。
\attention \n
N/A
\param[in] u32tunerId  tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[out] pu32SignalStrength  Pointer to the current signal strength of the tuner
                            For cable signal, the value of *pu32SignalStrength ranges from 20~120.
                            For satellite signal, the value is 15~130，unit in dBuv.
                  This parameter is valid only when HI_SUCCESS is returned.
                  Otherwise, the application layer sets the parameter to a small value.
CNcomment:\param[out] pu32SignalStrength  指向当前TUNER信号强度的指针。
CNcomment:                  对于Cable信号，*pu32SignalStrength的取值范围为20~120
CNcomment:                  对于Satellite信号，*pu32SignalStrength的取值范围为15~130，单位为dBuv
CNcomment:                  此参数只在函数返回成功的情况下才有意义，
CNcomment:                  否则，应用层可将此值设置为最小值。
\retval ::HI_SUCCESS Success                  CNcomment:成功
\retval ::HI_FAILURE  Calling this API fails. CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetSignalStrength(HI_U32   u32tunerId , HI_U32 *pu32SignalStrength );

/**
\brief Obtains the current signal quality of the tuner, returns a percentage value. 
CNcomment:\brief 获取当前TUNER的信号质量，返回百分比。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[out] pu32SignalQuality   Output pointer.                       CNcomment:指向信号质量的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetSignalQuality(HI_U32 u32TunerId, HI_U32 *pu32SignalQuality);

/**
\brief Obtains the actual frequency and symbol rate of the current tuner for cable and satellite
signal, for terrestrial signal, symbol rate means nothing, ignore it. 
CNcomment:\brief 获取当前TUNER的实际频率和符号率。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[out] pu32Freq   Pointer to the current frequency of the tuner. CNcomment:指向当前TUNER频点的指针。
\param[out] pu32Symb   Points to the current symbol rate.             CNcomment:指向当前符号率               
\retval ::HI_SUCCESS Success                                          CNcomment:成功
\retval ::HI_FAILURE  Calling this API fails.                         CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetRealFreqSymb( HI_U32 u32TunerId, HI_U32 *pu32Freq, HI_U32 *pu32Symb );

/**
\brief Obtains current signal information of the TUNER, used in satellite and terrestrial, not necessary for cable. 
CNcomment:\brief 获取当前TUNER的信号信息。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[out] pstSignalInfo Pointer to a signal info structure.         CNcomment:指向信号信息结构体的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetSignalInfo(HI_U32 u32TunerId, HI_UNF_TUNER_SIGNALINFO_S *pstSignalInfo);

/**
\brief Sets the LNB parameter. 
CNcomment:\brief 设置LNB参数。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] pstLNB      Pointer to a LNB parameter structure.          CNcomment:指向LNB参数结构体的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_SetLNBConfig(HI_U32 u32TunerId, const HI_UNF_TUNER_FE_LNB_CONFIG_S *pstLNB);

/**
\brief Sets the LNB power. 
CNcomment:\brief 设置LNB供电。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] enLNBPower  The enumeration of the LNB power type.         CNcomment:LNB供电方式枚举值。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_SetLNBPower(HI_U32 u32TunerId, HI_UNF_TUNER_FE_LNB_POWER_E enLNBPower);

/**
\brief Sets PLP ID, only used in DVB-T2. 
CNcomment:\brief 设置物理层管道ID。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] u8PLPID  The PLP ID.         CNcomment:物理层管道ID。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/

HI_S32 HI_UNF_TUNER_SetPLPID(HI_U32 u32TunerId, HI_U8 u8PLPID);

/**
\brief Gets PLP number, only used in DVB-T2. 
CNcomment:\brief 获取物理层管道数量。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[out] pu8PLPNum  The PLP number.         CNcomment:物理层管道数量。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/

HI_S32 HI_UNF_TUNER_GetPLPNum(HI_U32 u32TunerId, HI_U8 *pu8PLPNum);


/**
\brief Gets current PLP type, only used in DVB-T2. 
CNcomment:\brief 获取当前物理层管道类型。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[out] penPLPType  The PLP type.         CNcomment:物理层管道类型。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/

HI_S32 HI_UNF_TUNER_GetCurrentPLPType(HI_U32 u32TunerId, HI_UNF_TUNER_T2_PLP_TYPE_E *penPLPType);


/**
\brief Starts blind scan. 
CNcomment:\brief 开始盲扫。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] pstPara     The pointer to the blind scan parameter.       CNcomment:指向盲扫参数的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see Please refer to definition of HI_UNF_TUNER_BLINDSCAN_PARA_S.
N/A
*/
HI_S32 HI_UNF_TUNER_BlindScanStart(HI_U32 u32TunerId, const HI_UNF_TUNER_BLINDSCAN_PARA_S *pstPara);

/**
\brief Stops blind scan. 
CNcomment:\brief 停止盲扫。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_BlindScanStop(HI_U32 u32TunerId);

/**
\brief TUNER standby. 
CNcomment:\brief TUNER待机
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Standby(HI_U32 u32TunerId);

/**
\brief Wakes up TUNER. 
CNcomment:\brief TUNER唤醒
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_WakeUp( HI_U32 u32TunerId);

/**
\brief Sends and receives DiSEqC message, only the devices supporting DiSEqC 2.x support receive message. 
CNcomment:\brief 发送接收DiSEqC消息，仅支持DiSEqC 2.x的设备支持接收消息。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] pstSendMsg  The pointer to a sending message structure.    CNcomment:指向发送消息结构体的指针。
\param[out] pstRecvMsg The pointer to a receiving message structure.If your device is DiSEqC 1.x, you can pass NULL here.
CNcomment:\param[out] pstRecvMsg 指向接收消息结构体的指针。如果是DiSEqC 1.x设备，这里可以传NULL。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
/*HI_S32 HI_UNF_TUNER_DISEQC_SendRecvMessage(HI_U32 u32TunerId,
                                           HI_UNF_TUNER_DISEQC_SENDMSG_S* pstSendMsg,
                                           HI_UNF_TUNER_DISEQC_RECVMSG_S* pstRecvMsg);*/

/**
\brief Sets 0/12V switch. Don't support now. 
CNcomment:\brief 设置0/12V开关状态，暂不支持。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] enPort      The enumeration of the switch port.            CNcomment:开关枚举值。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Switch012V(HI_U32 u32TunerId, HI_UNF_TUNER_SWITCH_0_12V_E enPort);

/**
\brief Sets 22KHz switch. 
CNcomment:\brief 设置22KHz开关状态。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] enPort      The enumeration of the switch port.            CNcomment:开关枚举值。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Switch22K(HI_U32 u32TunerId, HI_UNF_TUNER_SWITCH_22K_E enPort);

/**
\brief Sets tone burst switch. 
CNcomment:\brief 设置Tone burst开关状态。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] enStatus    The enumeration of the switch port.            CNcomment:开关枚举值。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_SwitchToneBurst(HI_U32 u32TunerId, HI_UNF_TUNER_SWITCH_TONEBURST_E enStatus);

//#ifdef HI_BOOT_DISEQC_SUPPORT
/**
\brief Sets DiSEqC 1.0/2.0 switch, at most 4 port.
CNcomment:\brief 设置DiSEqC 1.0/2.0开关，至多4口。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] pstPara     The pointer to a switch parameter structure.   CNcomment:指向开关参数的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see Please refer to definition of HI_UNF_TUNER_DISEQC_SWITCH4PORT_S.
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Switch4Port(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_SWITCH4PORT_S* pstPara);

/**
\brief Sets DiSEqC 1.1/2.1 switch, supports 8in1, 16in1 switches. 
CNcomment:\brief 设置DiSEqC 1.1/2.1开关，支持8口，16口开关。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] pstPara     The pointer to a switch parameter structure.   CNcomment:指向开关参数的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Switch16Port(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_SWITCH16PORT_S* pstPara);

/**
\brief Lets the DiSEqC motor store current position. 
CNcomment:\brief DiSEqC马达存储当前位置。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] pstPara     The pointer to a position parameter structure. CNcomment:指向位置参数的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_StorePos(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_POSITION_S *pstPara);

/**
\brief Lets the DiSEqC motor move to stored position. 
CNcomment:\brief DiSEqC马达转动至存储位置。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] pstPara     The pointer to a position parameter structure. CNcomment:指向位置参数的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_GotoPos(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_POSITION_S *pstPara);

/**
\brief Enables or disables the DiSEqC motor's limit setting. 
CNcomment:\brief 打开、关闭DiSEqC马达权限设置。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] pstPara     The pointer to a limit parameter structure.    CNcomment:指向limit参数的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_SetLimit(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_LIMIT_S* pstPara);

/**
\brief Drives DiSEqC motor. 
CNcomment:\brief 驱动DiSEqC马达移动。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] pstPara     The pointer to a movement parameter structure. CNcomment:指向移动参数的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Move(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_MOVE_S* pstPara);

/**
\brief Halts DiSEqC motor. 
CNcomment:\brief 停止DiSEqC马达移动。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] enLevel     The command level of the DiSEqC motor.         CNcomment:马达支持的命令等级。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Stop(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_LEVEL_E enLevel);

/**
\brief Lets the DiSEqC motor recalculate its stored positions. 
CNcomment:\brief 重新计算马达存储位置。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] pstPara     The pointer to a recaulculate parameter structure. CNcomment:指向重计算参数的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Recalculate(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_RECALCULATE_S* pstPara);

/**
\brief Calculates the angular, basing on site-longitude, site-latitude and satellite-longitude. 
CNcomment:\brief USALS根据当地经纬度、卫星经度计算卫星角度。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in/out] pstPara The pointer to a USALS parameter structure.    CNcomment:指向USALS计算参数的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_CalcAngular(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_USALS_PARA_S* pstPara);

/**
\brief Gotos the indicated angular. 
CNcomment:\brief USALS设备转至某角度。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] pstPara     The pointer to a USALS angular structure.      CNcomment:指向USALS角度参数的指针。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_GotoAngular(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_USALS_ANGULAR_S* pstPara);

/**
\brief Resets DiSEqC device. 
CNcomment:\brief 复位DiSEqC设备。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] enLevel     The command level of the DiSEqC device.        CNcomment:设备支持的命令等级。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Reset(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel);

/**
\brief DiSEqC device standby. 
CNcomment:\brief DiSEqC设备待机。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] enLevel     The command level of the DiSEqC device.        CNcomment:设备支持的命令等级。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Standby(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel);

/**
\brief Wakes up DiSEqC device. 
CNcomment:\brief 唤醒DiSEqC设备。
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER端口号，取值为0-2
\param[in] enLevel     The command level of the DiSEqC device.        CNcomment:设备支持的命令等级。
\retval ::HI_SUCCESS   Success                                        CNcomment:成功
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:API系统调用失败
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_WakeUp(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel);
//#endif /* HI_BOOT_DISEQC_SUPPORT */

/** @} */  /** <!-- ==== API declaration end ==== */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_ECS_TYPE_H__ */

