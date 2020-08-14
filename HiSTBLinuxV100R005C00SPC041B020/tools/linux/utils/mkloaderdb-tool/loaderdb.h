/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

/*******************************************************************************/

#ifndef __LOADERDB_H__
#define __LOADERDB_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#define LOADER_INFOBASIC  "loaderdb"

typedef unsigned short HI_U16;
typedef unsigned char  HI_U8;
typedef unsigned int   HI_U32;

typedef short        HI_S16;
typedef char         HI_S8;
typedef int          HI_S32;
typedef void         HI_VOID;
typedef unsigned int HI_HANDLE;

typedef enum {
    HI_FALSE    = 0,
    HI_TRUE     = 1,
} HI_BOOL;

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_2*/
/** @{ */  /** <!- [Data structure of loader upgrade parameter area] */

/**LOADER magic number of DB and OTA parameters. it is used as a flag of data verification.*/
/**CNcomment:LOADER DB与OTA参数魔数,用作校验数据的标识*/
#define LOADER_MAGIC_NUMBER 0x4C4F4144

/**Maximum length of upgrade file name*/
/**CNcomment:升级文件名称最大长度*/
#define LOADER_FILE_NAME_LEN       128

/**Maximum length of user name.*/
/**CNcomment:用户名的最大长度 */
#define LOADER_USER_NAME_LEN        32

/**Maximum length of password.*/
/**CNcomment:密码的最大长度 */
#define LOADER_PASSWORD_LEN         32

/**IP address definition*/
/**CNcomment:IP地址定义*/
typedef HI_U32 HI_IPAddr;

/**IP port number definition*/
/**CNcomment:IP端口号定义*/
typedef HI_U16 HI_IPPort;

/**Magic check flag*/
/**CNcomment:标示魔术值*/
#define DEF_CHECK_FLAG  		0x5a5a5a5a

/**Upgrade type definition*/
/**CNcomment:升级类型定义*/
typedef enum tagLOADER_TYPE_E {
    HI_UPDATE_OTA  = 0,    /**< Cable upgrade *//**CNcomment:< CABLE 升级 */
    HI_UPDATE_IP,          /**< IP upgrade *//**CNcomment:< IP升级 */
    HI_UPDATE_USB,         /**< USB upgrade *//** CNcomment:< USB 升级 */
    HI_UPDATE_NULL,        /**< Do not upgrade *//**CNcomment:< 不升级 */
}HI_LOADER_TYPE_E;

/**Definition of the IP address type used in IP upgrade*/
/*CNcomment:*升级时使用下载协议定义*/
typedef enum tagProto_TYPE_E {
    HI_PROTO_TFTP = 0,     /**<Use TFTP protocol*//**CNcomment:< TFTP协议下载 */
    HI_PROTO_FTP,          /**<Use FTP protocol*//**CNcomment:< FTP协议下载 */
    HI_PROTO_HTTP,         /**<Use HTTP protocol*//**CNcomment:< HTTP协议下载*/

    HI_PROTO_BUTT
}HI_PROTO_TYPE_E;

/**Definition of ip update addr type*/
/**CNcomment:IP升级时使用IP地址类型定义*/
typedef enum tagIP_TYPE_E {
    HI_IP_STATIC = 0,    /**< Use static IP *//**CNcomment:< 使用静态IP */
    HI_IP_DHCP,          /**< Use dynamic IP *//**CNcomment:< 使用动态IP */
    HI_IP_BUTT,
}HI_IP_TYPE_E;

/**Definition of OSD UI language type*/
/**CNcomment:定义OSD使用何种语言显示信息*/
typedef enum tagUILang
{
    HI_UI_LANG_EN,    /**< Use chinese UI *//**CNcomment:< 使用英文显示 */    
    HI_UI_LANG_CN,    /**< Use english UI *//**CNcomment:< 使用中文显示 */

    HI_UI_LANG_BUTT
}HI_UI_LANG_E;

/**Definition of TUNER signal type*/
/**CNcomment:定义TUNER信号类型*/
typedef enum tagTunerSignalType_E
{
    HI_UNF_TUNER_SIG_TYPE_CAB = 1,      /**<Cable signal*/          /**<CNcomment:ITU-T J.83 ANNEX A/C(DVB_C)信号*/
    HI_UNF_TUNER_SIG_TYPE_SAT = 2,          /**<Satellite signal*/      /**<CNcomment:卫星信号*/
    HI_UNF_TUNER_SIG_TYPE_DVB_T = 4,        /**<Terrestrial signal*/    /**<CNcomment:地面信号*/
    HI_UNF_TUNER_SIG_TYPE_DVB_T2 = 8,       /**<Terrestrial signal*/    /**<CNcomment:地面信号*/
    HI_UNF_TUNER_SIG_TYPE_ISDB_T = 16,       /**<Terrestrial signal*/    /**<CNcomment:地面信号*/
    HI_UNF_TUNER_SIG_TYPE_ATSC_T = 32,       /**<Terrestrial signal*/    /**<CNcomment:地面信号*/
    HI_UNF_TUNER_SIG_TYPE_DTMB = 64,         /**<Terrestrial signal*/    /**<CNcomment:地面信号*/
    HI_UNF_TUNER_SIG_TYPE_J83B = 128,         /**<Cable signal*/          /**<CNcomment:ITU-T J.83 ANNEX B(US Cable)信号*/

    HI_UNF_TUNER_SIG_TYPE_BUTT          /**<Invalid value*/         /**<CNcomment:非法值*/
}HI_TUNER_SIG_TYPE_E;

/**Parameter for DiSEqC 1.0/2.0 switch
   Some DiSEqC device need set polarization(13/18V) and 22K, you can set them here. */
/**CNcomment:DiSEqC 1.0/2.0 开关参数有些DiSEqC设备需要设置极化方式和22K的，如果用了这种设备，需要在这里设置 */
typedef struct hiTunerSwitch4Port_S
{
    HI_U32    u32Level;  /**<DiSEqC device level*/
    HI_U32    u32Port;   /**<DiSEqC switch port*/
    HI_U32    u32Polar;  /**<Polarization type */
    HI_U32    u32LNB22K; /**<22K status*/
} HI_TUNER_SWITCH4PORT_S;

/**Parameter for DiSEqC 1.1/2.1 switch */
/**CNcomment:DiSEqC 1.1/2.1 开关参数 */
typedef struct hiTunerSwitch16PORT_S
{
    HI_U32       u32Level;          /**<DiSEqC device level*/
    HI_U32       u32Port;           /**<DiSEqC switch port*/
} HI_TUNER_SWITCH16PORT_S;

/*TS Priority, only used in DVB-T*/
/*CNcomment:仅用于DVB-T*/
typedef enum hiUNF_TUNER_TS_PRIORITY_E
{
    HI_UNF_TUNER_TS_PRIORITY_NONE = 0 ,   /**< */
    HI_UNF_TUNER_TS_PRIORITY_HP,             /**< */
    HI_UNF_TUNER_TS_PRIORITY_LP,         /**< */
    HI_UNF_TUNER_TS_PRIORITY_BUTT,           /**< */
}HI_UNF_TUNER_TS_PRIORITY_E ;

/** base channel or lite channel, only used in DVB-T2*/
/** CNcomment:仅用于DVB-T2*/
typedef enum hiUNF_TUNER_TER_MODE_E
{
    HI_UNF_TUNER_TER_MODE_BASE = 0,     /**< the channel is base mode*/         /**<CNcomment:通道中仅支持base信号*/
    HI_UNF_TUNER_TER_MODE_LITE,         /**< the channel is lite mode*/         /**<CNcomment:通道中需要支持lite信号*/
    HI_UNF_TUNER_TER_MODE_BUTT      /**<Invalid value*/            /**<CNcomment:非法边界值*/
} HI_UNF_TUNER_TER_MODE_E;

/**Define Cable TUNER parameter */
/**CNcomment:Cable TUNER的配置参数*/
typedef struct tagLoaderCabPara
{
    HI_U32      u32OtaPid;          /**< Upgrade data stream TS PID. The parameter is vital and set it with caution..*/
    HI_U32      u32OtaFreq;         /**< Upgrade data stream frequency pointer in kHz, for example, 403000. */
    HI_U32      u32OtaSymbRate;     /**< Upgrade data stream symbol rate in kbps, for example, 6875. */
    HI_U32      u32OtaModulation;   /**< Upgrade data stream constellation count.0:16QM 1:32QAM 2:64QAM 3:128QAM 4:256QAM  */

}HI_LOADER_CAB_PARA_S;

/*Define Terrestrial tuner parameter */
typedef struct tagLoaderTerPara
{
    HI_U32 u32OtaPid;          /**< TS PID. The parameter is vital and set it with caution..*//**<CNcomment: PID */
    HI_U32 u32OtaFreq;         /**< frequency pointer in kHz, for example, 177500. *//**<CNcomment: 频点，单位 kHz */
    HI_U32 u32OtaBandWidth;     /**< BandWidth in kHz, for example, 7000. *//**<CNcomment: 带宽，单位 kHz */
    HI_U32 u32OtaModulation;   /**< constellation count.0:16QAM 1:32QAM 2:64QAM 3:128QAM 4:256QAM  *//**<CNcomment: QAM方式，0:16QAM 1:32QAM 2:64QAM 3:128QAM 4:256QAM  */
	HI_UNF_TUNER_TER_MODE_E     enChannelMode;          //dvb-t2
    HI_UNF_TUNER_TS_PRIORITY_E  enDVBTPrio;             //dvb-t
} HI_LOADER_TER_PARA_S;

/**Define Sat TUNER parameter */
/**CNcomment:卫星TUNER的配置参数*/
typedef struct tagLoaderSatPara
{
    HI_U32 u32OtaPid;          /**< TS PID. The parameter is vital and set it with caution..*//**<CNcomment: PID */
    HI_U32 u32OtaFreq;         /**< frequency pointer in kHz, for example, 403000. *//**<CNcomment: 频点，单位 kHz */
    HI_U32 u32OtaSymbRate;     /**< symbol rate in kbps, for example, 27500. *//**<CNcomment: 符号率，单位 kbps */
    HI_U32 u32Polar;                /**<Polarization type:
                                                 0:Horizontal Polarization
                                                 1:Vertical Polarization
                                                 2:Left-hand circular polarization
                                                 3:Right-hand circular polarization>*/

    HI_U32 u32LowLO;           /**< Low Local Oscillator Frequency, MHz *//**<CNcomment: 低本振 */
    HI_U32 u32HighLO;          /**< High Local Oscillator Frequency, MHz*//**<CNcomment: 高本振 */

    /*support kinds of switch, support class-relation*/
    /**CNcomment:支持各种开关，支持级联 */
    HI_TUNER_SWITCH16PORT_S st16Port;
    HI_TUNER_SWITCH4PORT_S  st4Port;
    HI_U32                  u32Switch22K; /** 22KHz switch *//**<CNcomment: 22K开关 */
    HI_U32                  u32LNBPower;/*LNB Power*//**<CNcomment: LNB供电控制 */
    HI_BOOL   u32UnicFlag;             /**<u32UnicFlag, used by unicable .*//**<CNcomment: unicable使用标志位 */
}HI_LOADER_SAT_PARA_S;

/**Define TUNER type parameter */
/**CNcomment:定义TUNER类型匹配的参数*/
typedef struct tagTunerParaInfo
{
    HI_TUNER_SIG_TYPE_E eSigType;         /**<Signal type*/
    union
    {
        HI_LOADER_CAB_PARA_S stCab;       /**<Cable transmission signal*/
        HI_LOADER_SAT_PARA_S stSat;        /**<Satellite transmission signal*/
		HI_LOADER_TER_PARA_S stTer;        /**<Terrestrial signal parameter*//**<CNcomment: 地面信号参数 */
    }unConnPara;

}HI_LOADER_OTA_PARA_S;

/**Define IP update parameter */
/**CNcomment:定义IP升级相关的参数*/
typedef struct tagIpParam_S
{
    HI_PROTO_TYPE_E  eProtoType;    /**< Protocol of data download 0:TFTP, 1:FTP, 2 HTTP>*//**CNcomment:< 数据下载协议 0:TFTP, 1:FTP, 2 HTTP >*/
    HI_IP_TYPE_E     eIpType;            /**< Used by the IP loader. IP obtaining mode.*//**CNcomment:< ip  loader使用, ip获取类型*/

    /*Configure the following parameters only when static IP addresses are used.*/
    HI_IPAddr    ipOurIp;            /**< Used by the IP loader. Local IP address. *//**CNcomment:< ip  loader使用, 本机ip*/
    HI_IPAddr    ipGateway;          /**< Used by the IP loader. Gateway*//**CNcomment:< ip  loader使用, 网关*/
    HI_IPAddr    ipSubnetMask;       /**< Used by the IP loader. Subnet mask*//**CNcomment:< ip  loader使用, 子网掩码*/
    HI_IPAddr    ipServer;           /**< Used by the IP loader. Server IP address. *//**CNcomment:< ip  loader使用, 服务器ip*/
    HI_IPPort    ipServerPort;       /**< Used by the IP loader. Server port.*/ /**CNcomment:< ip  loader使用, 服务器port*/
    HI_S8        as8FileName[LOADER_FILE_NAME_LEN]; /**<IP Used by the IP loader. Upgrade file name.*/ /**CNcomment:<IP ip  loader使用, 升级文件名*/
    HI_U32       u32FileSize;        /*Used by the IP loader. Size of upgrade file.*//*CNcomment:*<IP ip  loader使用, 升级文件大小 */
    HI_S8        as8UserName[LOADER_USER_NAME_LEN]; /**<Username, used by FTP.>*/
    HI_S8        as8Password[LOADER_USER_NAME_LEN]; /**<Password, used by FTP .>*/

}HI_LOADER_IP_PARA_S;

/**Define USB update parameter */
/**CNcomment:定义USB升级相关的参数*/
typedef struct tagUSBParam_S
{
    HI_S8 as8FileName[LOADER_FILE_NAME_LEN];
} HI_LOADER_USB_PARAM_S;

/** Data structure for defining upgrade parameters*/
/**CNcomment:定义升级参数数接口结构体*/
typedef struct hiLOADER_PARAMETER_S
{
      /* Variable for the interaction between the application and loader */
    HI_LOADER_TYPE_E eUpdateType;        /**< Update type*//**<CNcomment: 升级类型*/

    union
    {
        HI_LOADER_OTA_PARA_S stOTAPara;        /**< ota upgrade parameters *//**<CNcomment: ota升级*/
        HI_LOADER_IP_PARA_S stIPParam;          /**< ip upgrade parameters*//**<CNcomment: ip升级参数*/
        HI_LOADER_USB_PARAM_S stUSBParam;    /*usb upgrade parameters*//**<CNcomment: usb升级参数*/
    } stPara;

    HI_U32 u32CurSoftware;    /**< application software version *//**<CNcomment: 软件版本号*/

    HI_U8 au8AppUse[20];      /**< Private upper-layer application *//**<CNcomment: App 数据*/
}HI_LOADER_PARAMETER_S;

typedef struct tagParaHead
{
    HI_U32   u32MagicNumber;     /**<  magic number name */
    HI_U32   u32Crc;             /**< CRC verification value */
    HI_U32   u32Length;          /*length of upgrade parameter.*/
}UPGRD_PARA_HEAD_S;


/*Define upgrade tag parameter.*/
typedef struct tagLoaderUpgrdTagParam
{
    HI_BOOL         bTagNeedUpgrd;        /*  whether normal upgrade.*/
    HI_BOOL         bTagManuForceUpgrd;   /*  whether force upgrade.*/
	HI_BOOL 		bTagUsbBurn;   		  /*  whether burn empty chipset from usb disk.*/
}UPGRD_TAG_PARA_S;

typedef struct tagLoaderPrivateParam
{
    HI_U32  u32FailedCnt;                /**< Number of upgrade failures. It can be ignored for user.*/
    HI_BOOL bTagDestroy;        /**< It can be ignored.*/
    HI_U8   au8Reserved[20];
} UPGRD_PRIVATE_PARAM_S;

typedef struct tagLoaderinfo
{
    UPGRD_PARA_HEAD_S     stTagParaHead;
    UPGRD_TAG_PARA_S      stTagPara;
    UPGRD_PARA_HEAD_S     stLoaderParaHead;
    HI_LOADER_PARAMETER_S  stLoaderParam;
	UPGRD_PRIVATE_PARAM_S stLoaderPrivate;
	
}UPGRD_LOADER_INFO_S;

#ifdef __cplusplus
 #if __cplusplus
}

 #endif
#endif

#endif

