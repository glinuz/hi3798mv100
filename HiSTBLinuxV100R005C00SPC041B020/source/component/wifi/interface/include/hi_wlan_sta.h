/******************************************************************************
  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
  File Name     : hi_wlan_sta.h
  Version       : Initial Draft
  Author        : Hisilicon sdk software group
  Created       :
  Last Modified :
  Description   : header file for Wi-Fi Station component
  Function List :
  History       :
  1.Date        :
  Author        :
  Modification  : Created file
******************************************************************************/

/**
 * \file
 * \brief describle the information about WiFi STA component. CNcomment:提供WiFi STA组件相关接口、数据结构信息。CNend
 */

#ifndef __HI_WLAN_STA_H__
#define __HI_WLAN_STA_H__

#include "hi_type.h"
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/*************************** Structure Definition ****************************/
/** \addtogroup     WLAN_STA */
/** @{ */  /** <!-- [WLAN_STA] */

#ifndef HI_WLAN_COMMON_DEF
#define HI_WLAN_COMMON_DEF

#define MAX_SSID_LEN    256    /** max len of AP's ssid *//** CNcomment:SSID最大长度 */
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

/** WiFi event type *//** CNcomment:WiFi事件类型 */
typedef enum hiHI_WLAN_STA_EVENT_E
{
    HI_WLAN_STA_EVENT_DISCONNECTED,    /**< network disconnected event *//** CNcomment:网络断开事件 */
    HI_WLAN_STA_EVENT_SCAN_RESULTS_AVAILABLE,    /**< scan done event *//** CNcomment:扫描结束事件 */
    HI_WLAN_STA_EVENT_CONNECTING,      /**< try to connect to network event *//** CNcomment:正在连接AP事件 */
    HI_WLAN_STA_EVENT_CONNECTED,       /**< network connected event *//** CNcomment:连接上AP事件 */
    HI_WLAN_STA_EVENT_SUPP_STOPPED,    /**< supplicant abnormal event *//** CNcomment:wpa_supplicant停止事件 */
    HI_WLAN_STA_EVENT_DRIVER_STOPPED,  /**< driver abnormal event *//** CNcomment:驱动退出事件 */
    HI_WLAN_STA_EVENT_BUTT,
} HI_WLAN_STA_EVENT_E;

/** connection state type *//** CNcomment:网络连接状态类型 */
typedef enum hiHI_WLAN_STA_CONN_STATE_E
{
    HI_WLAN_STA_CONN_STATUS_DISCONNECTED,  /**< not connected to any network *//** CNcomment:网络断开状态 */
    HI_WLAN_STA_CONN_STATUS_CONNECTING,    /**< connecting to a network *//** CNcomment:正在连接AP状态 */
    HI_WLAN_STA_CONN_STATUS_CONNECTED,     /**< connected to a network *//** CNcomment:连接上AP状态 */
    HI_WLAN_STA_CONN_STATUS_BUTT,
} HI_WLAN_STA_CONN_STATE_E;

/** network information *//** CNcomment:AP信息结构体 */
typedef struct hiHI_WLAN_STA_ACCESSPOINT_S
{
    HI_CHAR ssid[MAX_SSID_LEN+1];      /**< AP's SSID *//** CNcomment:AP的SSID */
    HI_CHAR bssid[BSSID_LEN+1];        /**< AP's MAC address *//** CNcomment:AP的MAC地址 */
    HI_U32  level;                     /**< AP's signal level, 0 - 100 *//** CNcomment:AP的信号强度，0 - 100 */
    HI_U32  channel;                   /**< AP's channel number *//** CNcomment:AP的信道 */
    HI_WLAN_SECURITY_E security;       /**< AP's security mode *//** CNcomment:AP的安全模式 */
} HI_WLAN_STA_ACCESSPOINT_S;

/** network configuration *//** CNcomment:需要连接的AP配置 */
typedef struct hiHI_WLAN_STA_CONFIG_S
{
    HI_CHAR   ssid[MAX_SSID_LEN+1];      /**< network's SSID *//** CNcomment:AP的SSID */
    HI_CHAR   bssid[BSSID_LEN+1];        /**< network's MAC address *//** CNcomment:AP的MAC地址 */
    HI_WLAN_SECURITY_E security;         /**< network's security mode *//** CNcomment:AP的安全模式 */
    HI_CHAR   psswd[MAX_PSSWD_LEN+1];    /**< network's password, if not OPEN mode *//** CNcomment:密码 */
    HI_BOOL   hidden_ssid;               /**< whether network hiddens it's SSID *//** CNcomment:AP是否是隐藏SSID */
} HI_WLAN_STA_CONFIG_S;

/** network status information *//** CNcomment:网络连接状态信息 */
typedef struct hiHI_WLAN_STA_CONN_STATUS_S
{
    HI_WLAN_STA_CONN_STATE_E state;    /**< connection state *//** CNcomment:网络的连接状态 */
    HI_WLAN_STA_ACCESSPOINT_S ap;      /**< network information which connected or connecting *//** CNcomment:连接上或者正在连接的AP信息 */
} HI_WLAN_STA_CONN_STATUS_S;


/** Callback function of receiving WiFi events *//** CNcomment:接收WiFi事件的回调函数 */
typedef HI_VOID (*HI_WLAN_STA_Event_CallBack)(HI_WLAN_STA_EVENT_E event, HI_VOID *pstPrivData);

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration *****************************/
/** \addtogroup     WLAN_STA*/
/** @{*/  /** <!-- [WLAN_STA]*/

/**
\brief: Initialize STA.CNcomment:初始化STA CNend
\attention \n
\param    N/A.CNcomment:无 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\see \n
::HI_WLAN_STA_DeInit
*/
HI_S32 HI_WLAN_STA_Init(HI_VOID);

/**
\brief: Deintialize STA.CNcomment:去初始化STA CNend
\attention \n
\param  N/A.CNcomment:无 CNend
\retval N/A.CNcomment:无 CNend
\see \n
::HI_WLAN_STA_Init
*/
HI_VOID HI_WLAN_STA_DeInit(HI_VOID);

/**
\brief: Open WiFi STA device.CNcomment:打开WiFi STA设备 CNend
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
HI_S32 HI_WLAN_STA_Open(HI_CHAR *ifname);

/**
\brief: Close WiFi STA device.CNcomment:关闭WiFi STA设备 CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\see \n
::HI_WLAN_STA_Open
*/
HI_S32 HI_WLAN_STA_Close(HI_CHAR *ifname);

/**
\brief: Start WiFi STA.CNcomment:启动WiFi STA CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\param[in] pfnEventCb  call back function that receives events.CNcomment:接收事件的回调函数 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\retval  ::HI_WLAN_START_SUPPLICANT_FAIL
\retval  ::HI_WLAN_CONNECT_TO_SUPPLICANT_FAIL
\see \n
::HI_WLAN_STA_Stop
*/
HI_S32 HI_WLAN_STA_Start(HI_CHAR *ifname, HI_WLAN_STA_Event_CallBack pfnEventCb);

/**
\brief: Stop WiFi STA.CNcomment:停用WiFi STA CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\see \n
::HI_WLAN_STA_Start
*/
HI_S32 HI_WLAN_STA_Stop(HI_CHAR *ifname);

/**
\brief: Start to scan.CNcomment:开始扫描 CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\retval  ::HI_WLAN_SEND_COMMAND_FAIL
\see \n
*/
HI_S32 HI_WLAN_STA_StartScan(HI_CHAR *ifname);

/**
\brief: Get scan results.CNcomment:获取扫描到的AP CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\param[out] pstApList AP list.CNcomment: 保存扫描到的AP列表 CNend
\param[inout] pstApNum  number of APs.CNcomment: AP列表中AP的数量 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\retval  ::HI_WLAN_SEND_COMMAND_FAIL
\see \n
*/
HI_S32 HI_WLAN_STA_GetScanResults(HI_CHAR *ifname, HI_WLAN_STA_ACCESSPOINT_S *pstApList,
                                    HI_U32 *pstApNum);

/**
\brief: Connect to AP.CNcomment:开始连接AP CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\param[in] pstStaCfg  AP configuration try to connect.CNcomment:需要连接的AP的信息 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\retval  ::HI_WLAN_SEND_COMMAND_FAIL
\see \n
::HI_WLAN_STA_Disconnect
*/
HI_S32 HI_WLAN_STA_Connect(HI_CHAR *ifname, HI_WLAN_STA_CONFIG_S *pstStaCfg);

/**
\brief: Disconnect to AP.CNcomment:断开连接 CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\retval  ::HI_WLAN_SEND_COMMAND_FAIL
\see \n
::HI_WLAN_STA_Connect
*/
HI_S32 HI_WLAN_STA_Disconnect(HI_CHAR *ifname);

/**
\brief: Get current network connection status.CNcomment:获得当前的连接状态信息 CNend
\attention \n
\param[in] ifname  WiFi network interface name.CNcomment:WiFi网络接口名, 如: wlan0 CNend
\param[out] pstConnStatus network connection status.CNcomment:保存连接状态信息 CNend
\retval  ::HI_SUCCESS
\retval  ::HI_FAILURE
\retval  ::HI_WLAN_INVALID_PARAMETER
\retval  ::HI_WLAN_SEND_COMMAND_FAIL
\see \n
*/
HI_S32 HI_WLAN_STA_GetConnectionStatus(HI_CHAR *ifname, HI_WLAN_STA_CONN_STATUS_S *pstConnStatus);

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
HI_S32 HI_WLAN_STA_GetMacAddress(HI_CHAR *ifname, HI_CHAR *pstMac);

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
#endif /* __HI_WLAN_STA_H__ */
