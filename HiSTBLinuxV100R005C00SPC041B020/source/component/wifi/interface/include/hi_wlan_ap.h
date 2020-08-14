/******************************************************************************
  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
  File Name     : hi_wlan_ap.h
  Version       : Initial Draft
  Author        : Hisilicon sdk software group
  Created       :
  Last Modified :
  Description   : header file for Wi-Fi SoftAP component
  Function List :
  History       :
  1.Date        :
  Author        :
  Modification  : Created file
******************************************************************************/

/**
 * \file
 * \brief describle the APIs and structs of WiFi SoftAP function. CNcomment:提供WiFi SoftAP功能组件相关接口、数据结构信息。CNend
 */

#ifndef __HI_WLAN_AP_H__
#define __HI_WLAN_AP_H__

#include "hi_type.h"
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/*************************** Structure Definition ****************************/
/** \addtogroup     WLAN_AP */
/** @{ */  /** <!-- [WLAN_AP] */

#ifndef HI_WLAN_COMMON_DEF
#define HI_WLAN_COMMON_DEF

#define MAX_SSID_LEN    32     /** max len of AP's ssid *//** CNcomment:SSID最大长度 */
#define BSSID_LEN       17     /** len of MAC address *//** CNcomment:MAC地址长度 */
#define MAX_PSSWD_LEN   64     /** max len of password *//** CNcomment:密码最大长度 */

/** invalid parameter */
/** CNcomment:无效的参数 -2 */
#define HI_WLAN_INVALID_PARAMETER           -2

/** no supported WiFi device found */
/** CNcomment:没有找到WiFi设备 -3 */
#define HI_WLAN_DEVICE_NOT_FOUND            -3

/** load driver fail */
/** CNcomment:加载驱动失败 -4 */
#define HI_WLAN_LOAD_DRIVER_FAIL            -4

/** run wpa_supplicant process fail */
/** CNcomment:启动wpa_supplicant失败 -5 */
#define HI_WLAN_START_SUPPLICANT_FAIL       -5

/** cann't connect to wpa_supplicant */
/** CNcomment:连接wpa_supplicant失败 -6 */
#define HI_WLAN_CONNECT_TO_SUPPLICANT_FAIL  -6

/** cann't send command to wpa_supplicant */
/** CNcomment:发送命令给wpa_supplicant失败 -7 */
#define HI_WLAN_SEND_COMMAND_FAIL           -7

/** run hostapd process fail */
/** CNcomment:启动hostapd失败 -8 */
#define HI_WLAN_START_HOSTAPD_FAIL          -8

/** network security mode type *//** CNcomment:AP安全模式类型 */
typedef enum hiHI_WLAN_SECURITY_E
{
    HI_WLAN_SECURITY_OPEN,          /**< OPEN mode, not encrypted *//** CNcomment:OPEN模式 */
    HI_WLAN_SECURITY_WEP,           /**< WEP mode *//** CNcomment:WEP模式 */
    HI_WLAN_SECURITY_WPA_WPA2_PSK,  /**< WPA-PSK/WPA2-PSK mode *//** CNcomment:WPA-PSK或WPA2-PSK模式 */
    HI_WLAN_SECURITY_WPA_WPA2_EAP,  /**< WPA-EAP/WPA2-EAP mode *//** CNcomment:WPA-EAP或WPA2-EAP模式 */
    HI_WLAN_SECURITY_WAPI_PSK,      /**< WAPI-PSK mode *//** CNcomment:WAPI-PSK模式 */
    HI_WLAN_SECURITY_WAPI_CERT,     /**< WAPI-CERT mode *//** CNcomment:WAPI-CERT模式 */
    HI_WLAN_SECURITY_BUTT,
} HI_WLAN_SECURITY_E;

#endif /* HI_WLAN_COMMON_DEF */

/** AP's configuration */
typedef struct hiHI_WLAN_AP_CONFIG_S
{
    HI_CHAR   ssid[MAX_SSID_LEN+1];      /**< network's SSID *//** CNcomment:SSID */
    HI_S32    channel;                   /**< network's channel *//** CNcomment:信道号 */
    HI_WLAN_SECURITY_E security;         /**< network's security mode *//** CNcomment:安全模式 */
    HI_CHAR   psswd[MAX_PSSWD_LEN+1];    /**< network's password, if not OPEN mode *//** CNcomment:密码 */
    HI_BOOL   hidden_ssid;               /**< whether network hiddens it's SSID *//** CNcomment:是否隐藏SSID */
} HI_WLAN_AP_CONFIG_S;

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration *****************************/
/** \addtogroup     WLAN_AP*/
/** @{*/  /** <!-- [WLAN_AP]*/

/**
\brief: Initialize SoftAP.CNcomment:初始化SoftAP CNend
\attention \n
\param    N/A.CNcomment:无 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\see \n
::HI_WLAN_AP_DeInit
*/
HI_S32 HI_WLAN_AP_Init(HI_VOID);

/**
\brief: Deintialize SoftAP.CNcomment:去初始化SoftAP CNend
\attention \n
\param  N/A.CNcomment:无 CNend
\retval N/A.CNcomment:无 CNend
\see \n
::HI_WLAN_STA_Init
*/
HI_VOID HI_WLAN_AP_DeInit(HI_VOID);

/**
\brief: Open WiFi SoftAP device.CNcomment:打开WiFi SoftAP设备 CNend
\attention \n
\param[out] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\retval  ::HI_WLAN_DEVICE_NOT_FOUND
\retval  ::HI_WLAN_LOAD_DRIVER_FAIL
\see \n
::HI_WLAN_STA_Close
*/
HI_S32 HI_WLAN_AP_Open(HI_CHAR *ifname);

/**
\brief: Close WiFi SoftAP device.CNcomment:关闭WiFi SoftAP设备 CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\see \n
::HI_WLAN_STA_Open
*/
HI_S32 HI_WLAN_AP_Close(HI_CHAR *ifname);

/**
\brief: start SoftAP with configuration.CNcomment:开启SoftAP CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\param[in] pstApCfg  AP's configuration.CNcomment:AP的配置参数 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\retval  ::HI_WLAN_START_HOSTAPD_FAIL
\see \n
::HI_WLAN_AP_Stop
*/
HI_S32 HI_WLAN_AP_Start(HI_CHAR *ifname, HI_WLAN_AP_CONFIG_S *pstApCfg);

/**
\brief: Stop SoftAP.CNcomment:关闭SoftAP CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\see \n
::HI_WLAN_STA_Start
*/
HI_S32 HI_WLAN_AP_Stop(HI_CHAR *ifname);

/**
\brief: Set SoftAP.CNcomment:设置SoftAP CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\param[in] pstApCfg  AP's configuration.CNcomment:AP的配置参数 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\retval  ::HI_WLAN_START_HOSTAPD_FAIL
\see \n
*/
HI_S32 HI_WLAN_AP_SetSoftap(HI_CHAR *ifname, HI_WLAN_AP_CONFIG_S *pstApCfg);

/**
\brief: Get local WiFi MAC address.CNcomment:获取本地WiFi MAC地址 CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\param[out] pstMac  MAC address of local WiFi.CNcomment:保存本地WiFi MAC地址 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\see \n
*/
HI_S32 HI_WLAN_AP_GetMacAddress(HI_CHAR *ifname, HI_CHAR *pstMac);

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
#endif /*__HI_WLAN_AP_H__*/
