/*
  * Copyright (c) 2016 MediaTek Inc.  All rights reserved.
  *
  * This software is available to you under a choice of one of two
  * licenses.  You may choose to be licensed under the terms of the GNU
  * General Public License (GPL) Version 2, available from the file
  * COPYING in the main directory of this source tree, or the
  * BSD license below:
  *
  *     Redistribution and use in source and binary forms, with or
  *     without modification, are permitted provided that the following
  *     conditions are met:
  *
  *      - Redistributions of source code must retain the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer.
  *
  *      - Redistributions in binary form must reproduce the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer in the documentation and/or other materials
  *        provided with the distribution.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */

/****************************************************************************

    Module Name:
	rt_os_net.h

	Abstract:
	All function prototypes are defined in NETIF modules.

    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------

***************************************************************************/

#ifndef __RT_OS_NET_H__
#define __RT_OS_NET_H__

#include "chip/chip_id.h"

typedef VOID * (*RTMP_NET_ETH_CONVERT_DEV_SEARCH) (VOID * net_dev, UCHAR * pData);
typedef int (*RTMP_NET_PACKET_TRANSMIT) (VOID *pPacket);

/* ========================================================================== */
/* operators used in DRIVER module */
typedef void (*RTMP_DRV_USB_COMPLETE_HANDLER) (VOID *pURB);

typedef struct _RTMP_NET_ABL_OPS {

#ifdef RTMP_USB_SUPPORT
/* net complete handlers */
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpNetUsbBulkOutDataPacketComplete;
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpNetUsbBulkOutMLMEPacketComplete;
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpNetUsbBulkOutNullFrameComplete;
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpNetUsbBulkOutRTSFrameComplete;
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpNetUsbBulkOutPsPollComplete;
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpNetUsbBulkRxComplete;
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpNetUsbBulkCmdRspEventComplete;

/* drv complete handlers */
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkOutDataPacketComplete;
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkOutMLMEPacketComplete;
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkOutNullFrameComplete;
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkOutRTSFrameComplete;
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkOutPsPollComplete;
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkRxComplete;
	RTMP_DRV_USB_COMPLETE_HANDLER RtmpDrvUsbBulkCmdRspEventComplete;

#endif				/* RTMP_USB_SUPPORT */

} RTMP_NET_ABL_OPS;

extern RTMP_NET_ABL_OPS *pRtmpDrvNetOps;

VOID RtmpNetOpsInit(VOID *pNetOpsOrg);
VOID RtmpNetOpsSet(VOID *pNetOpsOrg);

/* ========================================================================== */

NDIS_STATUS RTMPAllocAdapterBlock(struct os_cookie *handle, VOID **ppAdapter);
#ifdef RESUME_WITH_USB_RESET_SUPPORT
struct os_cookie *RTMPCheckOsCookie(struct os_cookie *handle, VOID **ppAdapter);
#endif /* RESUME_WITH_USB_RESET_SUPPORT */

VOID RTMPFreeAdapter(VOID *pAd);
BOOLEAN RtmpRaDevCtrlExit(VOID *pAd);
INT RtmpRaDevCtrlInit(VOID *pAd, RTMP_INF_TYPE infType);
VOID RTMPHandleInterrupt(VOID *pAd);

INT RTMP_COM_IoctlHandle(IN VOID *pAd,
			 IN RTMP_IOCTL_INPUT_STRUCT * wrq,
			 IN INT cmd, IN USHORT subcmd, IN VOID *pData, IN ULONG Data);

int RTMPSendPackets(IN NDIS_HANDLE MiniportAdapterContext,
		    IN PPNDIS_PACKET ppPacketArray,
		    IN UINT NumberOfPackets,
		    IN UINT32 PktTotalLen, IN RTMP_NET_ETH_CONVERT_DEV_SEARCH Func);

int P2P_PacketSend(IN PNDIS_PACKET pPktSrc, IN PNET_DEV pDev, IN RTMP_NET_PACKET_TRANSMIT Func);

#ifdef CONFIG_AP_SUPPORT
INT RTMP_AP_IoctlHandle(IN VOID *pAd,
			IN RTMP_IOCTL_INPUT_STRUCT * wrq,
			IN INT cmd, IN USHORT subcmd, IN VOID *pData, IN ULONG Data);
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
INT RTMP_STA_IoctlHandle(IN VOID *pAd,
			 IN RTMP_IOCTL_INPUT_STRUCT * wrq,
			 IN INT cmd,
			 IN USHORT subcmd, IN VOID *pData, IN ULONG Data, IN USHORT priv_flags);
#endif /* CONFIG_STA_SUPPORT */

VOID RTMPDrvOpen(VOID *pAd);
VOID RTMPDrvClose(VOID *pAd, VOID *net_dev);
VOID RTMPInfClose(VOID *pAd);

int rt28xx_init(IN VOID *pAd, IN PSTRING pDefaultMac, IN PSTRING pHostName);

PNET_DEV RtmpPhyNetDevMainCreate(VOID *pAd);

/* ========================================================================== */
/* public function prototype */
int MainVirtualIF_close(IN struct net_device *net_dev);
int MainVirtualIF_open(struct net_device *net_dev);
int rt28xx_close(VOID *dev);
int rt28xx_open(VOID *dev);

static inline INT VIRTUAL_IF_UP(VOID *pAd)
{
	RT_CMD_INF_UP_DOWN InfConf = { rt28xx_open, rt28xx_close };

	if (RTMP_COM_IoctlHandle(pAd, NULL, CMD_RTPRIV_IOCTL_VIRTUAL_INF_UP,
				 0, &InfConf, 0) != NDIS_STATUS_SUCCESS)
		return -1;
	return 0;
}

static inline VOID VIRTUAL_IF_DOWN(VOID *pAd)
{
	RT_CMD_INF_UP_DOWN InfConf = { rt28xx_open, rt28xx_close };

	RTMP_COM_IoctlHandle(pAd, NULL, CMD_RTPRIV_IOCTL_VIRTUAL_INF_DOWN, 0, &InfConf, 0);
}

#ifdef RTMP_MODULE_OS

#ifdef CONFIG_AP_SUPPORT
INT rt28xx_ap_ioctl(IN PNET_DEV net_dev, IN OUT struct ifreq *rq, IN INT cmd);
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
INT rt28xx_sta_ioctl(IN PNET_DEV net_dev, IN OUT struct ifreq *rq, IN INT cmd);
#endif /* CONFIG_STA_SUPPORT */

PNET_DEV RtmpPhyNetDevInit(IN VOID *pAd, IN RTMP_OS_NETDEV_OP_HOOK * pNetHook);

BOOLEAN RtmpPhyNetDevExit(IN VOID *pAd, IN PNET_DEV net_dev);

VOID *RtmpNetEthConvertDevSearch(VOID *net_dev_, UCHAR *pData);

#endif /* RTMP_MODULE_OS */

int rt_ioctl_siwscan(struct net_device *dev,
		     struct iw_request_info *info, union iwreq_data *wreq, char *extra);

VOID RT28xx_MBSS_Init(VOID *pAd, PNET_DEV main_dev_p);
INT MBSS_VirtualIF_Open(PNET_DEV dev_p);
INT MBSS_VirtualIF_Close(PNET_DEV dev_p);
VOID RT28xx_MBSS_Remove(VOID *pAd);

VOID RT28xx_WDS_Init(VOID *pAd, PNET_DEV net_dev);
INT WdsVirtualIF_open(PNET_DEV dev);
INT WdsVirtualIF_close(PNET_DEV dev);
VOID RT28xx_WDS_Remove(VOID *pAd);

VOID RT28xx_Monitor_Init(VOID *pAd, PNET_DEV main_dev_p);
VOID RT28xx_Monitor_Remove(VOID *pAd);

VOID RT28xx_ApCli_Init(VOID *pAd, PNET_DEV main_dev_p);
INT ApCli_VirtualIF_Open(PNET_DEV dev_p);
INT ApCli_VirtualIF_Close(PNET_DEV dev_p);
VOID RT28xx_ApCli_Remove(VOID *pAd);

VOID RTMP_Mesh_Init(VOID *pAd, PNET_DEV main_dev_p, PSTRING pHostName);
INT Mesh_VirtualIF_Open(PNET_DEV pDev);
INT Mesh_VirtualIF_Close(PNET_DEV pDev);
VOID RTMP_Mesh_Remove(VOID *pAd);

VOID RTMP_P2P_Init(VOID *pAd, PNET_DEV main_dev_p);
INT P2P_VirtualIF_Open(PNET_DEV dev_p);
INT P2P_VirtualIF_Close(PNET_DEV dev_p);
INT P2P_VirtualIF_PacketSend(IN PNDIS_PACKET skb_p, IN PNET_DEV dev_p);
VOID RTMP_P2P_Remove(VOID *pAd);

#ifdef RT_CFG80211_SUPPORT
#define CFG_P2PGO_ON(__pAd)  RTMP_CFG80211_VIF_P2P_GO_ON(__pAd)
#define CFG_P2PCLI_ON(__pAd) RTMP_CFG80211_VIF_P2P_CLI_ON(__pAd)

PNET_DEV RTMP_CFG80211_FindVifEntry_ByMacAddr(VOID *pAdSrc, UCHAR *pAddr);
PNET_DEV RTMP_CFG80211_FindVifEntry_ByType(VOID *pAdSrc, UINT32 devType);
PWIRELESS_DEV RTMP_CFG80211_FindVifEntryWdev_ByType(VOID *pAdSrc, UINT32 devType);
VOID RTMP_CFG80211_AddVifEntry(VOID *pAdSrc, PNET_DEV pNewNetDev, UINT32 DevType);
VOID RTMP_CFG80211_RemoveVifEntry(VOID *pAdSrc, PNET_DEV pNewNetDev);
#endif /* RT_CFG80211_SUPPORT */

#ifdef RT_CFG80211_AP_SUPPORT
#define CFG_AP_ON(__pAd) RTMP_CFG80211_VIF_AP_ON(__pAd)

BOOLEAN RTMP_CFG80211_VIF_AP_ON(VOID *pAdSrc);

VOID RTMP_CFG80211_ApIf_Init(VOID *pAdSrc);
VOID RTMP_CFG80211_ApIf_Remove(VOID *pAdSrc);
#else
#define CFG_AP_ON(__pAd) FALSE
#endif


#ifdef RT_CFG80211_P2P_SUPPORT

BOOLEAN RTMP_CFG80211_VIF_P2P_GO_ON(IN VOID *pAdSrc);

BOOLEAN RTMP_CFG80211_VIF_P2P_CLI_ON(IN VOID *pAdSrc);

BOOLEAN RTMP_CFG80211_VIF_P2P_CLI_CONNECTED(IN VOID *pAdSrc);

BOOLEAN RTMP_CFG80211_ROC_ON(IN VOID *pAdSrc);

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
VOID RTMP_CFG80211_DummyP2pIf_Init(VOID *pAdSrc);
VOID RTMP_CFG80211_DummyP2pIf_Remove(VOID *pAdSrc);
VOID RTMP_CFG80211_DummyP2pIf_Finalize(VOID *pAdSrc);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))
extern const struct ieee80211_iface_combination *p_ra_iface_combinations_ap_sta;
extern const INT ra_iface_combinations_ap_sta_num;
#endif
PNET_DEV RTMP_CFG80211_VirtualIF_Get(VOID *pAdSrc);
VOID RTMP_CFG80211_VirtualIF_CancelP2pClient(VOID *pAdSrc);
BOOLEAN RTMP_CFG80211_VirtualIF_Init(VOID *pAd, CHAR *pIfName, UINT32 DevType);
VOID RTMP_CFG80211_VirtualIF_Remove(VOID *pAd, PNET_DEV dev_p, UINT32 DevType);
VOID RTMP_CFG80211_AllVirtualIF_Remove(VOID *pAdSrc);
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

#else /* RT_CFG80211_P2P_SUPPORT */
static inline BOOLEAN RTMP_CFG80211_VIF_P2P_GO_ON(IN VOID *pAdSrc)
{
	return FALSE;
}

static inline BOOLEAN RTMP_CFG80211_VIF_P2P_CLI_ON(IN VOID *pAdSrc)
{
	return FALSE;
}

static inline BOOLEAN RTMP_CFG80211_VIF_P2P_CLI_CONNECTED(IN VOID *pAdSrc)
{
	return FALSE;
}

#endif /* RT_CFG80211_P2P_SUPPORT */

#ifdef CFG80211_STA_AP_CONCURRENT
VOID RTMP_CFG80211_MultiApIf_Remove(VOID *pAdSrc);
VOID RTMP_CFG80211_MultiApIf_Init(VOID *pAdSrc);
BOOLEAN RTMP_CFG80211_MultiApIf_ON(VOID *pAdSrc);
#endif /* CFG80211_STA_AP_CONCURRENT */

#ifdef RT_CFG80211_ANDROID_PRIV_LIB_SUPPORT
INT rt_android_private_command_entry(VOID *pAdSrc, struct net_device *net_dev, struct ifreq *ifr,
				     int cmd);
#endif /* RT_CFG80211_ANDROID_PRIV_LIB_SUPPORT */

/* FOR communication with RALINK DRIVER module in NET module */
/* general */
#define RTMP_DRIVER_NET_DEV_GET(__pAd, __pNetDev)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_NETDEV_GET, 0, __pNetDev, 0)

#define RTMP_DRIVER_NET_DEV_SET(__pAd, __pNetDev)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_NETDEV_SET, 0, __pNetDev, 0)

#define RTMP_DRIVER_OP_MODE_GET(__pAd, __pOpMode)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_OPMODE_GET, 0, __pOpMode, 0)

#define RTMP_DRIVER_IW_STATS_GET(__pAd, __pIwStats)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_INF_IW_STATUS_GET, 0, __pIwStats, 0)

#define RTMP_DRIVER_INF_STATS_GET(__pAd, __pInfStats)						\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_INF_STATS_GET, 0, __pInfStats, 0)

#define RTMP_DRIVER_INF_TYPE_GET(__pAd, __pInfType)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_INF_TYPE_GET, 0, __pInfType, 0)

#define RTMP_DRIVER_TASK_LIST_GET(__pAd, __pList)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_TASK_LIST_GET, 0, __pList, 0)

#define RTMP_DRIVER_NIC_NOT_EXIST_SET(__pAd)								\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_NIC_NOT_EXIST, 0, NULL, 0)

#define RTMP_DRIVER_MCU_SLEEP_CLEAR(__pAd)	\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_MCU_SLEEP_CLEAR, 0, NULL, 0)

#ifdef CONFIG_STA_SUPPORT
#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND

#define RTMP_DRIVER_USB_DEV_GET(__pAd, __pUsbDev)                                                       \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_USB_DEV_GET, 0, __pUsbDev, 0)

#define RTMP_DRIVER_USB_INTF_GET(__pAd, __pUsbIntf)                                                     \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_USB_INTF_GET, 0, __pUsbIntf, 0)

#define RTMP_DRIVER_ADAPTER_SUSPEND_SET(__pAd)								\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_SUSPEND_SET, 0, NULL, 0)

#define RTMP_DRIVER_ADAPTER_SUSPEND_CLEAR(__pAd)								\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_SUSPEND_CLEAR, 0, NULL, 0)

#define RTMP_DRIVER_ADAPTER_END_DISSASSOCIATE(__pAd)								\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_SEND_DISSASSOCIATE, 0, NULL, 0)

#define RTMP_DRIVER_ADAPTER_SUSPEND_TEST(__pAd, __flag)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_SUSPEND_TEST, 0,  __flag, 0)

#define RTMP_DRIVER_ADAPTER_IDLE_RADIO_OFF_TEST(__pAd, __flag)               \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_IDLE_RADIO_OFF_TEST, 0,  __flag, 0)
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */

#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
#define RTMP_DRIVER_ADAPTER_RT28XX_WOW_STATUS(__pAd, __flag)	                     \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_WOW_STATUS, 0, __flag, 0)

#define RTMP_DRIVER_ADAPTER_RT28XX_WOW_ENABLE(__pAd)								\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_WOW_ENABLE, 0, NULL, 0)

#define RTMP_DRIVER_ADAPTER_RT28XX_WOW_DISABLE(__pAd)								\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_WOW_DISABLE, 0, NULL, 0)

#define RTMP_DRIVER_ADAPTER_RT28XX_WOW_RUNSTATUS(__pAd, __flag)	\
	RTMP_COM_IoctlHandle(__pAd, NULL,		\
		CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_WOW_RUNSTATUS, 0, __flag, 0)

#define RTMP_DRIVER_ADAPTER_RT28XX_WOW_READY(__pAd, __flag)		\
		RTMP_COM_IoctlHandle(__pAd, NULL,	\
		CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_WOW_READY, 0, __flag, 0)

#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */

#define RTMP_DRIVER_ADAPTER_RT28XX_CMD_RADIO_OFF(__pAd)			\
	RTMP_COM_IoctlHandle(__pAd, NULL,		\
		CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_CMD_RADIO_OFF, 0, NULL, 0)

#define RTMP_DRIVER_ADAPTER_RT28XX_CMD_RADIO_ON(__pAd)			\
	RTMP_COM_IoctlHandle(__pAd, NULL,		\
		CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_CMD_RADIO_ON, 0, NULL, 0)

#endif /* CONFIG_PM */

#define RTMP_DRIVER_AP_SSID_GET(__pAd, pData)				\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_AP_BSSID_GET, 0, pData, 0)
#endif /* CONFIG_STA_SUPPORT */

#define RTMP_DRIVER_ADAPTER_RT28XX_USB_ASICRADIO_OFF(__pAd)                              \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_USB_ASICRADIO_OFF, 0, NULL, 0)

#define RTMP_DRIVER_ADAPTER_RT28XX_USB_ASICRADIO_ON(__pAd)                      \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_USB_ASICRADIO_ON, 0, NULL, 0)

#define RTMP_DRIVER_ADAPTER_SUSPEND_SET(__pAd)								\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_SUSPEND_SET, 0, NULL, 0)

#define RTMP_DRIVER_ADAPTER_SUSPEND_CLEAR(__pAd)								\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_SUSPEND_CLEAR, 0, NULL, 0)

#define RTMP_DRIVER_VIRTUAL_INF_NUM_GET(__pAd, __pIfNum)					\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_VIRTUAL_INF_GET, 0, __pIfNum, 0)

#define RTMP_DRIVER_CHANNEL_GET(__pAd, __Channel)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_SIOCGIWFREQ, 0, __Channel, 0)

#define RTMP_DRIVER_IOCTL_SANITY_CHECK(__pAd, __SetCmd)								\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_SANITY_CHECK, 0, __SetCmd, 0)

#define RTMP_DRIVER_BITRATE_GET(__pAd, __pBitRate)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_AP_SIOCGIWRATEQ, 0, __pBitRate, 0)

#define RTMP_DRIVER_MAIN_INF_CREATE(__pAd, __ppNetDev)						\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_INF_MAIN_CREATE, 0, __ppNetDev, 0)

#define RTMP_DRIVER_MAIN_INF_GET(__pAd, __pInfId)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_INF_MAIN_ID_GET, 0, __pInfId, 0)

#define RTMP_DRIVER_MAIN_INF_CHECK(__pAd, __InfId)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_INF_MAIN_CHECK, 0, NULL, __InfId)

#define RTMP_DRIVER_P2P_INF_CHECK(__pAd, __InfId)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_INF_P2P_CHECK, 0, NULL, __InfId)

#ifdef EXT_BUILD_CHANNEL_LIST
#define RTMP_DRIVER_SET_PRECONFIG_VALUE(__pAd)								\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_SET_PRECONFIG_VALUE, 0, NULL, 0)
#endif /* EXT_BUILD_CHANNEL_LIST */

#ifdef RT_CFG80211_SUPPORT
/* General Part */
#define RTMP_DRIVER_CFG80211_START(__pAd)									\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_CFG80211_CFG_START, 0, NULL, 0)

#define RTMP_DRIVER_80211_UNREGISTER(__pAd, __pNetDev)					\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_UNREGISTER, 0, __pNetDev, 0)

#define RTMP_DRIVER_80211_CB_GET(__pAd, __ppCB)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_CB_GET, 0, __ppCB, 0)

#define RTMP_DRIVER_80211_CB_SET(__pAd, __pCB)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_CB_SET, 0, __pCB, 0)

#define RTMP_DRIVER_80211_RESET(__pAd)				\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_RESET,  0, NULL, 0)

/* STA Part */
#define RTMP_DRIVER_80211_SCAN_CHANNEL_LIST_SET(__pAd, __pData, __Len) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_CHANNEL_LIST_SET, 0, __pData, __Len)

#define RTMP_DRIVER_80211_SCAN(__pAd, __IfType)									\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_SCAN, 0, NULL, __IfType)

#define RTMP_DRIVER_80211_SCAN_STATUS_LOCK_INIT(__pAd, __isInit)	\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_SCAN_STATUS_LOCK_INIT, 0, NULL, __isInit)

#define RTMP_DRIVER_80211_SCAN_EXTRA_IE_SET(__pAd)				\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_EXTRA_IES_SET,  0, NULL, 0)

#define RTMP_DRIVER_80211_STA_KEY_ADD(__pAd, __pKeyInfo)					\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_STA_KEY_ADD, 0, __pKeyInfo, 0)

#define RTMP_DRIVER_80211_STA_KEY_DEFAULT_SET(__pAd, __KeyId)				\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_STA_KEY_DEFAULT_SET, 0, NULL, __KeyId)

#ifdef DOT11W_PMF_SUPPORT
#define RTMP_DRIVER_80211_STA_MGMT_KEY_DEFAULT_SET(__pAd, __KeyId)				\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_STA_MGMT_KEY_DEFAULT_SET, 0, NULL, __KeyId)
#endif /* DOT11W_PMF_SUPPORT */

#define RTMP_DRIVER_80211_POWER_MGMT_SET(__pAd, __enable)				\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_POWER_MGMT_SET, 0, NULL, __enable)

#define RTMP_DRIVER_80211_STA_LEAVE(__pAd, __ifType)								\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_STA_LEAVE, 0, NULL, __ifType)

#define RTMP_DRIVER_80211_CONNECT(__pAd, __pConnInfo, __devType)					\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_CONNECT_TO, 0, __pConnInfo, __devType)

/* Information Part */
#define RTMP_DRIVER_80211_BANDINFO_GET(__pAd, __pBandInfo)				\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_BANDINFO_GET, 0, __pBandInfo, 0)

#define RTMP_DRIVER_80211_CHANGE_BSS_PARM(__pAd, __pBssInfo) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_CHANGE_BSS_PARM, 0, __pBssInfo, 0)

#define RTMP_DRIVER_80211_CHAN_SET(__pAd, __pChan)						\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_CHAN_SET, 0, __pChan, 0)

#define RTMP_DRIVER_80211_RFKILL(__pAd, __pActive)						\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_RFKILL, 0, __pActive, 0)

#define RTMP_DRIVER_80211_REG_NOTIFY(__pAd, __pNotify)					\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_REG_NOTIFY_TO, 0, __pNotify, 0)

#define RTMP_DRIVER_80211_SURVEY_GET(__pAd, __pSurveyInfo)				\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_SURVEY_GET, 0, __pSurveyInfo, 0)

#define RTMP_DRIVER_80211_NETDEV_EVENT(__pAd, __pDev, __state)				\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_NETDEV_EVENT, 0, __pDev, __state)

/* AP Part */
#define RTMP_DRIVER_80211_BEACON_DEL(__pAd, __pNetDev) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_BEACON_DEL, 0, __pNetDev, 0)

#define RTMP_DRIVER_80211_BEACON_ADD(__pAd, __pBeacon) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_BEACON_ADD, 0, __pBeacon, 0)

#define RTMP_DRIVER_80211_BEACON_SET(__pAd, __pBeacon) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_BEACON_SET, 0, __pBeacon, 0)

#define RTMP_DRIVER_80211_BITRATE_SET(__pAd, __pMask) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_BITRATE_SET, 0, __pMask, 0)

#define RTMP_DRIVER_80211_AP_KEY_DEL(__pAd, __pKeyInfo) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_AP_KEY_DEL, 0, __pKeyInfo, 0)

#define RTMP_DRIVER_80211_AP_KEY_ADD(__pAd, __pKeyInfo) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_AP_KEY_ADD, 0, __pKeyInfo, 0)

#define RTMP_DRIVER_80211_RTS_THRESHOLD_ADD(__pAd, __Rts_thresold) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_RTS_THRESHOLD_ADD, 0, \
	__Rts_thresold, 0)

#define RTMP_DRIVER_80211_FRAG_THRESHOLD_ADD(__pAd, __Frag_thresold) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_FRAG_THRESHOLD_ADD, 0, \
	__Frag_thresold, 0)

#define RTMP_DRIVER_80211_AP_KEY_DEFAULT_SET(__pAd, __pMbssidIdx, __KeyId) \
	RTMP_COM_IoctlHandle(__pAd, NULL, \
			     CMD_RTPRIV_IOCTL_80211_AP_KEY_DEFAULT_SET, 0, \
			     __pMbssidIdx, __KeyId)

#define RTMP_DRIVER_80211_AP_PROBE_RSP(__pAd, __pFrame, __Len) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_AP_PROBE_RSP_EXTRA_IE, 0, __pFrame, __Len)

#define RTMP_DRIVER_80211_AP_ASSOC_RSP(__pAd, __pFrame, __Len) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_AP_ASSOC_RSP_EXTRA_IE, 0, __pFrame, __Len)

/* P2P Part */
#define RTMP_DRIVER_80211_ACTION_FRAME_REG(__pAd, __devPtr, __Reg) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_ACTION_FRAME_REG, 0, __devPtr, __Reg)

#define RTMP_DRIVER_80211_REMAIN_ON_CHAN_DUR_IMER_INIT(__pAd)                       \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_REMAIN_ON_CHAN_DUR_TIMER_INIT, 0, NULL, 0)

#define RTMP_DRIVER_80211_REMAIN_ON_CHAN_SET(__pAd, __pChan, __Duration)  \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_REMAIN_ON_CHAN_SET, 0, __pChan, __Duration)

#define RTMP_DRIVER_80211_CANCEL_REMAIN_ON_CHAN_SET(__pAd, __cookie) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_CANCEL_REMAIN_ON_CHAN_SET, 0, NULL, __cookie)

#define RTMP_DRIVER_80211_CHANNEL_LOCK(__pAd, __Chan)                   \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_CHANNEL_LOCK, 0, NULL , __Chan)

#define RTMP_DRIVER_80211_MGMT_FRAME_REG(__pAd, __devPtr, __Reg) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_MGMT_FRAME_REG, 0, __devPtr, __Reg)

#define RTMP_DRIVER_80211_TX_NETDEV_SET(__pAd, __netdev)                   \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_TX_NETDEV_SET, 0, __netdev , 0)

#define RTMP_DRIVER_80211_MGMT_FRAME_SEND(__pAd, __pFrame)                       \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_MGMT_FRAME_SEND, 0, __pFrame, 0)

#define RTMP_DRIVER_80211_P2P_CHANNEL_RESTORE(__pAd)				\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_CHANNEL_RESTORE,  0, NULL, 0)

#define RTMP_DRIVER_80211_P2P_CLIENT_KEY_ADD(__pAd, __pKeyInfo)					\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_P2P_CLIENT_KEY_ADD, 0, __pKeyInfo, 0)

/* VIF Part */
#define RTMP_DRIVER_80211_VIF_ADD(__pAd, __pInfo) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_VIF_ADD, 0, __pInfo, 0)

#define RTMP_DRIVER_80211_VIF_DEL(__pAd, __devPtr, __type) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_VIF_DEL, 0, __devPtr, __type)

#define RTMP_DRIVER_80211_VIF_CHG(__pAd, __pVifInfo)			\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_VIF_CHG, 0, __pVifInfo, 0)

#ifdef RT_P2P_SPECIFIC_WIRELESS_EVENT
#define RTMP_DRIVER_80211_SEND_WIRELESS_EVENT(__pAd, __pMacAddr)					\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_SEND_WIRELESS_EVENT, 0, __pMacAddr, 0)
#endif /* RT_P2P_SPECIFIC_WIRELESS_EVENT */

#ifdef CONFIG_MULTI_CHANNEL
#define RTMP_DRIVER_ADAPTER_MCC_DHCP_PROTECT_STATUS(__pAd, __flag)	             \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_MCC_DHCP_PROTECT_STATUS, 0, __flag, 0)

#define RTMP_DRIVER_80211_SET_NOA(__pAd, __Devname)                                 \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_80211_SET_NOA, 0, __Devname, 0)
#endif /* CONFIG_MULTI_CHANNEL */
#endif /* RT_CFG80211_SUPPORT */

/* mesh */
#define RTMP_DRIVER_MESH_REMOVE(__pAd)										\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_MESH_REMOVE, 0, NULL, 0)

/* inf ppa */
#define RTMP_DRIVER_INF_PPA_INIT(__pAd)										\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_INF_PPA_INIT, 0, NULL, 0)

#define RTMP_DRIVER_INF_PPA_EXIT(__pAd)										\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_INF_PPA_EXIT, 0, NULL, 0)

/* pci */
#define RTMP_DRIVER_IRQ_INIT(__pAd)                                          \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_IRQ_INIT, 0, NULL, 0)

#define RTMP_DRIVER_IRQ_RELEASE(__pAd)										\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_IRQ_RELEASE, 0, NULL, 0)

#define RTMP_DRIVER_PCI_MSI_ENABLE(__pAd, __pPciDev)						\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_MSI_ENABLE, 0, __pPciDev, 0)

#define RTMP_DRIVER_PCI_SUSPEND(__pAd)										\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_PCI_SUSPEND, 0, NULL, 0)

#define RTMP_DRIVER_PCI_RESUME(__pAd)										\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_PCI_RESUME, 0, NULL, 0)

#define RTMP_DRIVER_PCI_CSR_SET(__pAd, __Address)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_PCI_CSR_SET, 0, NULL, __Address)

#define RTMP_DRIVER_PCIE_INIT(__pAd, __pPciDev)								\
{                                                                          \
	RT_CMD_PCIE_INIT __Config, *__pConfig = &__Config;		\
	__pConfig->pPciDev = __pPciDev;	                                                  \
	__pConfig->ConfigDeviceID = PCI_DEVICE_ID;			                 \
	__pConfig->ConfigSubsystemVendorID = PCI_SUBSYSTEM_VENDOR_ID;			 \
	__pConfig->ConfigSubsystemID = PCI_SUBSYSTEM_ID;				\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_PCIE_INIT, 0, __pConfig, 0);\
}

/* usb */
#define RTMP_DRIVER_USB_MORE_FLAG_SET(__pAd, __pConfig)						\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_USB_MORE_FLAG_SET, 0, __pConfig, 0)

#define RTMP_DRIVER_USB_CONFIG_INIT(__pAd, __pConfig)						\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_USB_CONFIG_INIT, 0, __pConfig, 0)

#define RTMP_DRIVER_USB_SUSPEND(__pAd, __bIsRunning)						\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_USB_SUSPEND, 0, NULL, __bIsRunning)

#define RTMP_DRIVER_USB_RESUME(__pAd)										\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_USB_RESUME, 0, NULL, 0)

#define RTMP_DRIVER_USB_INIT(__pAd, __pUsbDev, __driver_info)	\
do {	\
	RT_CMD_USB_INIT __Config, *__pConfig = &__Config;	\
	__pConfig->pUsbDev = __pUsbDev;	\
	__pConfig->driver_info = __driver_info;	\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_USB_INIT, 0, __pConfig, 0);	\
} while (0)

/* ap */
#define RTMP_DRIVER_AP_BITRATE_GET(__pAd, __pConfig)							\
	RTMP_AP_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_AP_SIOCGIWRATEQ, 0, __pConfig, 0)

#define RTMP_DRIVER_AP_MAIN_OPEN(__pAd)										\
	RTMP_AP_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_MAIN_OPEN, 0, NULL, 0)

/* sta */
#ifdef PROFILE_PATH_DYNAMIC
#define RTMP_DRIVER_STA_PROFILEPATH_SET(__pAd, __Type)							\
	RTMP_STA_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_PROFILEPATH_SET, 0, NULL, __Type, __Type)
#endif /* PROFILE_PATH_DYNAMIC */

#define RTMP_DRIVER_STA_DEV_TYPE_SET(__pAd, __Type)							\
	RTMP_STA_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ORI_DEV_TYPE_SET, 0, NULL, __Type, __Type)

#define RTMP_DRIVER_MAC_ADDR_GET(__pAd, __pMacAddr)                                 \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_MAC_ADDR_GET, 0, __pMacAddr, 0)

#define RTMP_DRIVER_ADAPTER_CSO_SUPPORT_TEST(__pAd, __flag)                         \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_CSO_SUPPORT_TEST, 0,  __flag, 0)

#define RTMP_DRIVER_ADAPTER_TSO_SUPPORT_TEST(__pAd, __flag)                          \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_ADAPTER_TSO_SUPPORT_TEST, 0,  __flag, 0)

#ifdef CONFIG_HAS_EARLYSUSPEND
#define RTMP_DRIVER_SET_SUSPEND_FLAG(__pAd) \
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_SET_SUSPEND_FLAG, 0, NULL, 0)

#define RTMP_DRIVER_LOAD_FIRMWARE_CHECK(__pAd)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_LOAD_FIRMWARE_CHECK, 0, NULL, 0)

#define RTMP_DRIVER_OS_COOKIE_GET(__pAd, __os_cookie)							\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_OS_COOKIE_GET, 0, __os_cookie, 0)

#define RTMP_DRIVER_ADAPTER_REGISTER_EARLYSUSPEND(__pAd)	\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_REGISTER_EARLYSUSPEND, 0, NULL, 0)

#define RTMP_DRIVER_ADAPTER_UNREGISTER_EARLYSUSPEND(__pAd)	\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_UNREGISTER_EARLYSUSPEND, 0, NULL, 0)

#define RTMP_DRIVER_ADAPTER_CHECK_EARLYSUSPEND(__pAd, __flag)	\
	RTMP_COM_IoctlHandle(__pAd, NULL, CMD_RTPRIV_IOCTL_CHECK_EARLYSUSPEND, 0, __flag, 0)
#endif /* CONFIG_HAS_EARLYSUSPEND */

#endif /* __RT_OS_NET_H__ */
