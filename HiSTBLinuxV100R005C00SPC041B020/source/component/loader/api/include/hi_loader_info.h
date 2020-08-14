/*******************************************************************************
 *             Copyright 2013 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_loader_info.h
 * Description:
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 ******************************************************************************/
#ifndef __HI_LOADER_INFO_H__
#define __HI_LOADER_INFO_H__

#include "hi_type.h"
#include "hi_unf_frontend.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/*************************** Structure Definition ****************************/
/** \addtogroup      LOADER */
/** @{ */  /** <!--[LOADER] */

/**Maximum length of upgrade file name*/
/**CNcomment:升级文件名称最大长度*/
#define LOADER_FILE_NAME_LEN 128

/**Maximum length of user name.*/
/**CNcomment:用户名的最大长度 */
#define LOADER_USER_NAME_LEN 32

/**Maximum length of password.*/
/**CNcomment:密码的最大长度 */
#define LOADER_PASSWORD_LEN 32

/**IP address definition*/
/**CNcomment:IP地址定义*/
typedef HI_U32 HI_IPAddr;

/**IP port number definition*/
/**CNcomment:IP端口号定义*/
typedef HI_U16 HI_IPPort;

#define DEF_CHECK_FLAG 0x5a5a5a5a

/**Upgrade type definition*/
/**CNcomment:升级类型定义*/
typedef enum tagLOADER_TYPE_E
{
    HI_LOADER_TYPE_OTA = 0, /**< Cable upgrade *//**<CNcomment: CABLE 升级 */
    HI_LOADER_TYPE_IP, /**< IP upgrade *//**<CNcomment: IP升级 */
    HI_LOADER_TYPE_USB, /**< USB upgrade *//**<CNcomment: USB升级 */
    HI_LOADER_TYPE_BUTT, /**< Do not upgrade *//**<CNcomment: 不升级 */
} HI_LOADER_TYPE_E;

/**Definition of the IP address type used in IP upgrade*/
/**CNcomment:升级时使用下载协议定义*/
typedef enum tagProto_TYPE_E
{
    HI_PROTO_TFTP = 0, /**<Use TFTP protocol*//**<CNcomment: TFTP协议下载 */
    HI_PROTO_FTP, /**<Use FTP protocol*//**<CNcomment: FTP协议下载 */
    HI_PROTO_HTTP, /**<Use HTTP protocol*//**<CNcomment: HTTP协议下载*/

    HI_PROTO_BUTT
} HI_PROTO_TYPE_E;

/**CNcomment:IP升级时使用IP地址类型定义*/
typedef enum tagIP_TYPE_E
{
    HI_IP_STATIC = 0, /**< Use static IP *//**<CNcomment: 使用静态IP */
    HI_IP_DHCP, /**< Use dynamic IP *//**<CNcomment: 使用动态IP */
    HI_IP_BUTT,
} HI_IP_TYPE_E;

/**TUNER signal type*/
typedef enum tagTunerSignalType_E
{
    HI_TUNER_SIG_TYPE_CAB = 0,     /**<Cable signal*//**<CNcomment: 有线信号 */
    HI_TUNER_SIG_TYPE_TER,         /**<Terrestrial signal*//**<CNcomment: 地面信号 */
    HI_TUNER_SIG_TYPE_SAT,          /**<Satellite signal*//**<CNcomment: 卫星信号 */

    HI_TUNER_SIG_TYPE_BUTT           /**invalid type*//**<CNcomment: 无效信号类型 */
} HI_TUNER_SIG_TYPE_E;

typedef struct hiLOADER_USB_PARAM_S
{
    HI_S8 as8FileName[LOADER_FILE_NAME_LEN];
} HI_LOADER_USB_PARAM_S;

/**Parameter for DiSEqC 1.0/2.0 switch
   Some DiSEqC device need set polarization(13/18V) and 22K, you can set them here. */

/**CNcomment:DiSEqC 1.0/2.0 开关参数有些DiSEqC设备需要设置极化方式和22K的，如果用了这种设备，需要在这里设置 */
typedef struct hiTunerSwitch4Port_S
{
    HI_U32 u32Level;  /**<DiSEqC device level*//**<CNcomment: 级别 */
    HI_U32 u32Port;   /**<DiSEqC switch port*//**<CNcomment:  开关端口*/
    HI_U32 u32Polar;  /**<Polarization type *//**<CNcomment:  极化方式*/
    HI_U32 u32LNB22K; /**<22K status*//**<CNcomment: 22K开关状态 */
} HI_TUNER_SWITCH4PORT_S;

/**Parameter for DiSEqC 1.1/2.1 switch */
/**CNcomment:DiSEqC 1.1/2.1 开关参数 */
typedef struct hiTunerSwitch16PORT_S
{
    HI_U32 u32Level;          /**<DiSEqC device level*//**<CNcomment: 级别 */
    HI_U32 u32Port;           /**<DiSEqC switch port*//**<CNcomment: 开关端口 */
} HI_TUNER_SWITCH16PORT_S;

/*Define Cable tuner parameter */
typedef struct tagLoaderCabPara
{
    HI_U32 u32OtaPid;          /**< TS PID. The parameter is vital and set it with caution..*//**<CNcomment: PID */
    HI_U32 u32OtaFreq;         /**< frequency pointer in kHz, for example, 403000. *//**<CNcomment: 频点，单位 kHz */
    HI_U32 u32OtaSymbRate;     /**< symbol rate in kbps, for example, 6875. *//**<CNcomment: 符号率，单位 kbps */
    HI_U32 u32OtaModulation;   /**< constellation count.0:16QAM 1:32QAM 2:64QAM 3:128QAM 4:256QAM  *//**<CNcomment: QAM方式，0:16QAM 1:32QAM 2:64QAM 3:128QAM 4:256QAM  */
} HI_LOADER_CAB_PARA_S;

/*Define Terrestrial tuner parameter */
typedef struct tagLoaderTerPara
{
    HI_U32 u32OtaPid;          /**< TS PID. The parameter is vital and set it with caution..*//**<CNcomment: PID */
    HI_U32 u32OtaFreq;         /**< frequency pointer in kHz, for example, 177500. *//**<CNcomment: 频点，单位 kHz */
    HI_U32 u32OtaBandWidth;     /**< BandWidth in kHz, for example, 7000. *//**<CNcomment: 带宽，单位 kHz */
    HI_U32 u32OtaModulation;   /**< constellation count.0:16QAM 1:32QAM 2:64QAM 3:128QAM 4:256QAM  *//**<CNcomment: QAM方式，0:16QAM 1:32QAM 2:64QAM 3:128QAM 4:256QAM  */
	HI_UNF_TUNER_TER_MODE_E     enChannelMode;      //dvb-t2
    HI_UNF_TUNER_TS_PRIORITY_E  enDVBTPrio;         //dvb-t
} HI_LOADER_TER_PARA_S;

/*Define Sat tuner parameter */
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
} HI_LOADER_SAT_PARA_S;

/*Define Turner parameter */
typedef struct tagOTAParaInfo
{
    HI_UNF_TUNER_SIG_TYPE_E eSigType;         /**<Signal type*//**<CNcomment: 信号类型 */
    union
    {
        HI_LOADER_CAB_PARA_S stCab;       /**<Cable signal parameter *//**<CNcomment: 有线信号参数 */
        HI_LOADER_SAT_PARA_S stSat;        /**<Satellite signal parameter*//**<CNcomment: 卫星信号参数 */
        HI_LOADER_TER_PARA_S stTer;        /**<Terrestrial signal parameter*//**<CNcomment: 地面信号参数 */
    } unConnPara;
} HI_LOADER_OTA_PARA_S;

/* Define IP upgrade parameter. */
typedef struct tagIpParam_S
{
    HI_PROTO_TYPE_E eProtoType; /**< Protocol of data download 0:TFTP, 1:FTP, 2 HTTP>*//**<CNcomment: 数据下载协议 0:TFTP, 1:FTP, 2 HTTP */
    HI_IP_TYPE_E    eIpType; /**< IP obtaining mode.*/            /**<CNcomment: ip获取类型 */

    /*Configure the following parameters only when static IP addresses are used.*/
    HI_IPAddr ipOurIp; /**< Local IP address. *//**<CNcomment: 本机ip */
    HI_IPAddr ipGateway; /**< Gateway*//**<CNcomment: 网关 */
    HI_IPAddr ipSubnetMask; /**< Subnet mask*//**<CNcomment: 子网掩码 */
    HI_IPAddr ipServer; /**< Server IP address. *//**<CNcomment: 服务器ip */
    HI_IPPort ipServerPort; /**< Server port.*/ /**<CNcomment: 服务器port */
    HI_S8     as8FileName[LOADER_FILE_NAME_LEN]; /**< Upgrade file name.*/    /**<CNcomment:升级文件名 */
    HI_U32    u32FileSize; /* Size of upgrade file.*/          /**<CNcomment: 升级文件大小 */
    HI_S8     as8UserName[LOADER_USER_NAME_LEN];    /**<Username, used by FTP.*//**<CNcomment: 用户名 */
    HI_S8     as8Password[LOADER_USER_NAME_LEN];    /**<Password, used by FTP .*//**<CNcomment: 密码 */
} HI_LOADER_IP_PARA_S;

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
} HI_LOADER_PARAMETER_S;

/**STB infor */
typedef struct hiLOADER_STBINFO_S
{
    HI_U32 u32OUI; /**< Vendor ID *//**<CNcomment: 厂商id */
    HI_U32 u32AreaCode; /**< Region code *//**<CNcomment: 区域码*/
    HI_U32 u32HWVersion; /**< Hardware version *//**<CNcomment: 硬件版本号 */
    HI_U32 u32HWSerialNum; /**< Hardware serial number *//**<CNcomment: 硬件系列号*/
} HI_LOADER_STB_INFO_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      LOADER */
/** @{ */  /** <!-- [LOADER] */

/**
 \brief Obtains upgrade parameter data from the flash or E2ROM. CNcomment:从FLASH或E2ROM中获取升级参数数据。CNend
 \attention \n
 \param[out] info    Upgrade parameters. The memory is allocated externally.
CNcomment:\param[out] info  loader升级参数,外部分配内存。CNend
 \retval ::HI_SUCCESS Success. CNcomment:成功。CNend
 \retval ::HI_FAILURE This API fails to be called.  CNcomment:API系统调用失败。CNend
 \see ::HI_LOADER_PARAMETER_S \n
For details, see the definition in HI_LOADER_PARAMETER_S.
CNcomment:详细信息,请参考HI_LOADER_PARAMETER_S定义。CNend
 */
HI_S32 HI_LOADER_ReadParameters(HI_LOADER_PARAMETER_S* info);

/**
 \brief This API is used to update upgrade parameters to the flash or E2ROM. CNcomment:更新升级参数数据到FLASH或E2ROM。CNend
 \attention \n
 \param[in] info  Upgrade parameters to be updated.
CNcomment:\param[in] info   要更新的升级参数。CNend
 \retval ::HI_SUCCESS Success. CNcomment:成功。CNend
 \retval ::HI_FAILURE This API fails to be called. CNcomment:API系统调用失败。CNend
 \see ::HI_LOADER_PARAMETER_S \n
For details, see the definition in HI_LOADER_PARAMETER_S.
CNcomment:详细信息,请参考HI_LOADER_PARAMETER_S定义。CNend
 */
HI_S32 HI_LOADER_WriteParameters(HI_LOADER_PARAMETER_S* info);

/**
 \brief Obtains STB serial number from the flash or E2ROM.CNcomment:从FLASH或E2ROM中获取机顶盒序列号信息。CNend
 \attention \n
Serial number information includes the hardware version and vendor ID to be verified during the upgrade. Different service provider and vendors have different serial numbers for STBs.\n
CNcomment:序列号含有升级时需要比较的硬件版本号,厂商编号。不同的运营商和厂家有不同的规格机顶盒的序列号。\n CNend
A serial number contains the following information:\n
          External serial number, such as  00000003-00000001-1d000021      \n
          External serial number is in the following format:\n
          AAAAAAAA-BBBBBBBB-CCCCCCCC ....\n
          AAAAAAAA 	Terminal product vendor authentication number\n
          BBBBBBBB 	Hardware version\n
          CCCCCCCC 	Hareware serial number.\n

CNcomment:序列号的格式组成如下: \n
          外置序列号如:  00000003-00000001-1d000021。 \n
          外置序列号格式如下: \n
          AAAAAAAA-BBBBBBBB-CCCCCCCC ....\n
          AAAAAAAA 	终端产品生产厂商认证编号。 \n
          BBBBBBBB 	硬件版本号。\n
          CCCCCCCC 	硬件系列号。\n CNend
 \param[out] pstStbinfo  STB serial number information. The memory is allocated externally.CNcomment:机顶盒序列号信息.外部分配内存。 CNend
 \retval ::HI_SUCCESS Success. CNcomment:成功。 CNend
 \retval ::HI_FAILURE  This API fails to be called. CNcomment:API系统调用失败。 CNend
 \see ::HI_LOADER_STB_INFO_S \n
For details, see the definition in HI_LOADER_STB_INFO_S.
CNcomment:详细信息,请参考HI_LOADER_STB_INFO_S定义。 CNend
 */
HI_S32 HI_LOADER_ReadSTBInfo(HI_LOADER_STB_INFO_S* pstStbinfo);

/**
 \brief Upgrades flash or E2ROM STB serial number.CNcomment:更新FLASH或E2ROM机顶盒序列号信息。 CNend
 \attention \n
Serial number information includes the hardware version and vendor ID to be verified during the upgrade. \n
Different carriers and vendors have different serial numbers for STBs.\n
You can change the serial number rules as required. \n
This API responds to the au8OutSn(STB  external serial number).
CNcomment:序列号含有升级时需要比较的硬件版本号,厂商编号。\n CNend
CNcomment:不同的运营商和厂家有不同的规格机顶盒的序列号，\n CNend
CNcomment:用户可根据实际需要在修改序列号规则。\n CNend
CNcomment:该接口只理会au8OutSn(机顶盒外部序列号)参数。\n CNend
A serial number contains the following information:\n
          External serial number, such as  00000003-00000001-1d000021      \n
          External serial number is in the following format:\n
          AAAAAAAA-BBBBBBBB-CCCCCCCC ....\n
          AAAAAAAA 	Terminal product vendor authentication number\n
          BBBBBBBB 	Hardware version\n
          CCCCCCCC 	Hareware serial number.\n

CNcomment:序列号的格式组成如下: \n
          外置序列号如:  00000003-00000001-1d000021。 \n
          外置序列号格式如下: \n
          AAAAAAAA-BBBBBBBB-CCCCCCCC ....\n
          AAAAAAAA 	终端产品生产厂商认证编号。 \n
          BBBBBBBB 	硬件版本号。\n
          CCCCCCCC 	硬件系列号。\n CNend
 \param[out] pstStbinfo  STB serial number information. The memory is allocated externally.CNcomment:机顶盒序列号信息.外部分配内存。\nCNend
 \retval ::HI_SUCCESS Success. CNcomment:成功。CNend
 \retval ::HI_FAILURE  This API fails to be called. CNcomment:API系统调用失败。CNend
 \see ::HI_LOADER_STB_INFO_S \n
For details, see the definition in HI_LOADER_STB_INFO_S
CNcomment:详细信息,请参考HI_LOADER_STB_INFO_S定义。CNend
 */
HI_S32 HI_LOADER_WriteSTBInfo(HI_LOADER_STB_INFO_S* pstStbinfo);

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
 #if __cplusplus
}

 #endif
#endif

#endif
