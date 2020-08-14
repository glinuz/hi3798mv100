/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : hi_loader_info_v2.h
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by:
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 ******************************************************************************/

#ifndef __HI_LOADER_INFO_V2_H__
#define __HI_LOADER_INFO_V2_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_type.h"
#include "hi_unf_frontend.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup      LOADER */
/** @{ */  /** <!-- [LOADER] */

/****** Macro switch definition ***********/

/****** Macro constants definition ********/

/** Maximum length of the upgrade filename */
#define HI_LOADER_FILENAME_LEN          (128)

/** Maximum length of user name */
#define HI_LOADER_USERNAME_LEN          (32)

/** Maximum length of password */
#define HI_LOADER_PASSWORD_LEN          (32)

/** Maximum length of partition name */
#define HI_LOADER_PARTITION_NAME_LEN    (32)

/** IP address definition */
typedef HI_U32 HI_LOADER_IPADDR;

/** IP port number definition*/
typedef HI_U16 HI_LOADER_IPPORT;

/****** Macro Functions definition ********/

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      LOADER */
/** @{ */  /** <!-- [LOADER] */

/****** Enumeration definition ************/

/** Upgrade type definition*/
typedef enum hiLOADER_UPGRADE_TYPE_E
{
    HI_LOADER_UPGRADE_TYPE_OTA = 0,     /**< Upgrade via OTA */
    HI_LOADER_UPGRADE_TYPE_IP,          /**< Upgrade via IP Channel */
    HI_LOADER_UPGRADE_TYPE_USB,         /**< Upgrade via USB */
    HI_LOADER_UPGRADE_TYPE_FS,  		/**< Upgrade via File System */
    HI_LOADER_UPGRADE_TYPE_RESET,		/**< Partition reset */
    HI_LOADER_UPGRADE_TYPE_BUTT         /**< Error Upgrade type */

} HI_LOADER_UPGRADE_TYPE_E;

/** Upgrade mode definition*/
typedef enum hiLOADER_UPGRADE_MODE_E
{
    HI_LOADER_UPGRADE_MODE_BASIC = 0,   /**< Basic upgrade mode, The software version number of the upgrade stream must larger than the software version number of the device. */
    HI_LOADER_UPGRADE_MODE_MANDATORY,   /**< Mandatory upgrade, Do not compare the software version number */
    HI_LOADER_UPGRADE_MODE_BUTT

} HI_LOADER_UPGRADE_MODE_E;

/** Network Transfer Protocol definition */
typedef enum hiLOADER_IPPROT_E
{
    HI_LOADER_IPPROT_TFTP = 0,          /**< TFTP Transfer Protocol */
    HI_LOADER_IPPROT_FTP,               /**< FTP Transfer Protocol */
    HI_LOADER_IPPROT_HTTP,              /**< HTTP Transfer Protocol */
    HI_LOADER_IPPROT_BUTT

} HI_LOADER_IPPROT_E;

/** Host Configuration method definition */
typedef enum hiLOADER_IPCFG_E
{
    HI_LOADER_IPCFG_STATIC = 0,         /**< Static Configuration method */
    HI_LOADER_IPCFG_DHCP,               /**< DHCP Configuration method */
    HI_LOADER_IPCFG_BUTT

} HI_LOADER_IPCFG_E;

/** Satellite mototype definition */
typedef enum hiLOADER_MOTORTYPE_E
{
    HI_LOADER_MOTORTYPE_NONE = 0,       /**< None */
    HI_LOADER_MOTORTYPE_DISEQC12,       /**< DiSEqC1.2 protocol */
    HI_LOADER_MOTORTYPE_USLAS,          /**< USLAS protocol */
    HI_LOADER_MOTORTYPE_BUTT

} HI_LOADER_MOTORTYPE_E;

/****** Structure definition **************/

/** Cable upgrade parameter structure */
typedef struct hiLOADER_PARAM_CAB_S
{
    HI_UNF_CAB_CONNECT_PARA_S           stConnectParam;     /**< Cable Connect Parameters */

} HI_LOADER_PARAM_CAB_S;

/** Terestrial upgrade parameter structure */
typedef struct hiLOADER_PARAM_TER_S
{
    HI_UNF_TER_CONNECT_PARA_S           stConnectParam;     /**< Terestrial Connect Parameters */
    HI_U32                              u32PLPId;           /**< The PLP ID */

} HI_LOADER_PARAM_TER_S;

/** Satellite upgrade parameter structure */
typedef struct hiLOADER_PARAM_SAT_S
{
    HI_UNF_SAT_CONNECT_PARA_S           stConnectParam;     /**< Satellite Connect Parameters*/
    HI_UNF_TUNER_FE_LNB_POWER_E         enLNBPower;         /**< LNB Power */
    HI_UNF_TUNER_FE_LNB_CONFIG_S        stLNBConfig;        /**< LNB configurating parameters */
    HI_UNF_TUNER_SWITCH_22K_E           enSwitch22K;        /**< 22KHz switch */
    HI_UNF_TUNER_DISEQC_SWITCH4PORT_S   stSwitch4Port;      /**< DiSEqC 1.0 Switch Parameters */
    HI_UNF_TUNER_DISEQC_SWITCH16PORT_S  stSwitch16Port;     /**< DiSEqC 1.1 Switch Parameters */
    HI_LOADER_MOTORTYPE_E               enMotorType;        /**< Motor type */
    HI_UNF_TUNER_DISEQC_USALS_PARA_S    stUSALSParam;       /**< USALS Config Parameters */
    HI_UNF_TUNER_DISEQC_POSITION_S      stPosition;         /**< Position Config Parameters */
    HI_U32                              u32IsiID;           /**< isi id,vcm stream id transmitted by server */
} HI_LOADER_PARAM_SAT_S;

/** OTA upgrade parameter structure */
typedef struct hiLOADER_PARAM_OTA_S
{
    HI_UNF_TUNER_SIG_TYPE_E enSigType;          /**< Signal type */
    HI_U32                  u32TunerID;         /**< Tuner identification */
    HI_U32                  u32Pid;             /**< TS PID. The parameter is vital and set it with caution */

    union
    {
        HI_LOADER_PARAM_CAB_S stCab;            /**< Cable transmission signal */
        HI_LOADER_PARAM_SAT_S stSat;            /**< Satellite transmission signal */
        HI_LOADER_PARAM_TER_S stTer;            /**< Terestrial transmission signal */
    } unParam;

} HI_LOADER_PARAM_OTA_S;

/** USB upgrade parameter structure */
typedef struct hiLOADER_PARAM_USB_S
{
    HI_S8   as8FileName[HI_LOADER_FILENAME_LEN];  /**< filename of the upgrade image */

} HI_LOADER_PARAM_USB_S;

typedef struct hiLOADER_PARAM_FS_S
{
    HI_S8   as8FileName[HI_LOADER_FILENAME_LEN];            /**< filename of the upgrade image */
    HI_S8   as8PartitonName[HI_LOADER_PARTITION_NAME_LEN];  /**< Partition name of the upgrade image */

} HI_LOADER_PARAM_FS_S;

/** IP upgrade parameter structure */
typedef struct hiLOADER_IP_PARAM_S
{
    HI_LOADER_IPPROT_E  enProtType;             /**< Protocol of data download */
    HI_LOADER_IPCFG_E   enIPCfgType;            /**< IP obtaining mode */

    /** static IP addresses config */
    HI_LOADER_IPADDR    ipOurIp;                /**< Local IP address */
    HI_LOADER_IPADDR    ipGateway;              /**< Gateway */
    HI_LOADER_IPADDR    ipSubnetMask;           /**< Subnet mask */
    HI_LOADER_IPADDR    ipServer;               /**< Server IP address */
    HI_LOADER_IPPORT    ipServerPort;           /**< Server port */

    HI_U32  u32FileSize;                        /**< Size of upgrade file */
    HI_S8   as8FileName[HI_LOADER_FILENAME_LEN];/**< Upgrade file name */
    HI_S8   as8UserName[HI_LOADER_USERNAME_LEN];/**< Username, used by FTP */
    HI_S8   as8Password[HI_LOADER_PASSWORD_LEN];/**< Password, used by FTP */

} HI_LOADER_PARAM_IP_S;

/** Upgrade parameter structure */
typedef struct hiHI_LOADER_PARAM_S
{
    HI_LOADER_UPGRADE_TYPE_E    enUpgradeType;          /**< Loader Update type */
    HI_LOADER_UPGRADE_MODE_E    enUpgradeMode;          /**< Loader Update Mode */
    HI_U32                      u32SoftwareVersion;     /**< The software version of the image to upgrade */

    union
    {
        HI_LOADER_PARAM_USB_S   stUSBParam;     /**< USB upgrade parameters */
        HI_LOADER_PARAM_OTA_S   stOTAParam;     /**< OTA upgrade parameters */
        HI_LOADER_PARAM_IP_S    stIPParam;      /**< IP upgrade parameters */
        HI_LOADER_PARAM_FS_S    stFSParam;      /**< FS upgrade parameters */
    } unParam;


} HI_LOADER_PARAM_S;


/** Device information structure */
typedef struct hiLOADER_DEVICEINFO_S
{
    HI_U32  u32OUI;                             /**< Vendor ID */
    HI_U32  u32AreaCode;                        /**< Region code */
    HI_U32  u32HWVersion;                       /**< Hardware version */
    HI_U32  u32HWSerialNum;                     /**< Hardware serial number */

} HI_LOADER_DEVICEINFO_S;

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API declaration *****************************/
/** \addtogroup      LOADER */
/** @{ */  /** <!-- [LOADER] */

/**
 \brief Get upgrade parameter
 \attention \n
 \param[out] pstParameter Upgrade parameters
 \retval ::HI_SUCCESS Success
 \retval ::ERROR CODE Failure
 */
HI_S32 HI_LOADER_GetParameter(HI_LOADER_PARAM_S* pstParameter);

/**
 \brief Update upgrade parameters.
 \attention \n
 \param[in] pstParameter Upgrade parameters to be updated.
 \retval ::HI_SUCCESS Success
 \retval ::ERROR CODE Failure
 */
HI_S32 HI_LOADER_SetParameter(HI_LOADER_PARAM_S* pstParameter);

/**
 \brief Get software information
 \attention \n
 \param[out] pu32SoftwareVersion software version
 \retval ::HI_SUCCESS Success.
 \retval ::ERROR CODE Failure.
 */
HI_S32 HI_LOADER_GetSoftwareVersion(HI_U32* pu32SoftwareVersion);

/**
 \brief Set software information
 \attention \n
 \param[out] u32SoftwareVersion software version
 \retval ::HI_SUCCESS Success.
 \retval ::ERROR CODE Failure.
 */
HI_S32 HI_LOADER_SetSoftwareVersion(HI_U32 u32SoftwareVersion);

/**
 \brief Get device's information.
 \attention \n
 \param[out] pstDeviceInfo Device information.
 \retval ::HI_SUCCESS Success
 \retval ::ERROR CODE Failure
 */
HI_S32 HI_LOADER_GetDeviceInfo(HI_LOADER_DEVICEINFO_S* pstDeviceInfo);

/**
 \brief Set device's information.
 \attention \n
 \param[in] pstDeviceInfo Device information.
 \retval ::HI_SUCCESS Success
 \retval ::ERROR CODE Failure
 */
HI_S32 HI_LOADER_SetDeviceInfo(HI_LOADER_DEVICEINFO_S* pstDeviceInfo);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__HI_LOADER_INFO_V2_H__*/

