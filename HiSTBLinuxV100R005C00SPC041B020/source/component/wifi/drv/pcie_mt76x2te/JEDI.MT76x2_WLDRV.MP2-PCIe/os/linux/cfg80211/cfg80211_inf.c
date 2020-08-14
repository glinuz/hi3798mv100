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

#define RTMP_MODULE_OS

#include "rt_config.h"
#include "rtmp_comm.h"
#include "rt_os_util.h"
#include "rt_os_net.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 28))
#ifdef RT_CFG80211_SUPPORT

#ifdef CFG80211_STA_AP_CONCURRENT
#define INF_CFG80211_MULTI_AP_NAME "ap0"
#define IS_AP_DEV(_DevName) (strcmp(_DevName, INF_CFG80211_MULTI_AP_NAME) == 0)
#endif

INT RTMP_CFG80211_GET_MBSSID_INDEX(VOID *pAdSrc, PNET_DEV pNetDev)
{
	switch (pNetDev->ieee80211_ptr->iftype) {
	case RT_CMD_80211_IFTYPE_P2P_GO:
		return FIRST_MBSSID;
	case RT_CMD_80211_IFTYPE_AP:
		return SECOND_MBSSID;
	default:
		return FIRST_MBSSID;
	}
}

VOID RTMP_SHIFT_BCN_OFFSET0(VOID *pAdSrc, INT newFirstIdx)
{
	INT32 reg = 0;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;

	RTMP_IO_READ32(pAd, RLT_BCN_OFFSET0, &reg);
	reg &= 0xFFFF0000;

	switch (newFirstIdx) {
	case 0:
		reg |= 0x00001008;
		break;
	case 1:
		reg |= 0x00000010;
		break;
	default:
		DBGPRINT(RT_DEBUG_ERROR,
			 ("%s - Unsuppurted new first index %d\n",
			  __func__, newFirstIdx));
	};

	RTMP_IO_WRITE32(pAd, RLT_BCN_OFFSET0, reg);
}

VOID RTMP_SET_BCN_NUM(
	IN		VOID	*pAdSrc,
	IN		INT		bcnNum)
{
	INT32 reg = 0;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;

	if (bcnNum > 3) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s - unsupported beacon num %d\n",
								  __func__, bcnNum));
		return;
	}

	RTMP_IO_READ32(pAd, MAC_BSSID_DW1, &reg);
	reg &= 0xFFF3FFFF;
	reg |= (bcnNum << 18);
	RTMP_IO_WRITE32(pAd, MAC_BSSID_DW1, reg);
}

VOID RTMP_CFG80211_DISABLE_BEACON(
	IN		VOID	*pAdSrc,
	IN		PNET_DEV	pNetDev,
	IN		UINT8	oldIfType)
{
#ifdef RT_CFG80211_AP_SUPPORT
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;
#endif /* RT_CFG80211_AP_SUPPORT */

	if (oldIfType == RT_CMD_80211_IFTYPE_AP)
		RTMP_SHIFT_BCN_OFFSET0(pAdSrc, 0);
	else if (oldIfType == RT_CMD_80211_IFTYPE_P2P_GO) {
#ifdef RT_CFG80211_AP_SUPPORT
		if (CFG_AP_ON(pAd))
			RTMP_SHIFT_BCN_OFFSET0(pAdSrc, 1);
		else
#endif /* RT_CFG80211_AP_SUPPORT */
			RTMP_SHIFT_BCN_OFFSET0(pAdSrc, 0);

		RTMP_SET_BCN_NUM(pAdSrc, 0);
	}
}

#if defined(CONFIG_AP_SUPPORT) || defined(RT_CFG80211_AP_SUPPORT)
VOID RTMP_CFG80211_ENABLE_BEACON(VOID *pAdSrc, PNET_DEV	pNetDev)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;

	if (pNetDev->ieee80211_ptr->iftype ==
		RT_CMD_80211_IFTYPE_AP) {
#ifdef RT_CFG80211_P2P_SUPPORT
		if (CFG_P2PGO_ON(pAd)) {
			RTMP_SET_BCN_NUM(pAdSrc, 1);
			RTMP_SHIFT_BCN_OFFSET0(pAdSrc, 0);
		} else
#endif /* RT_CFG80211_P2P_SUPPORT */
		{
			RTMP_SET_BCN_NUM(pAdSrc, 0);
			RTMP_SHIFT_BCN_OFFSET0(pAdSrc, 1);
		}
	} else if (pNetDev->ieee80211_ptr->iftype ==
			 RT_CMD_80211_IFTYPE_P2P_GO) {
#ifdef RT_CFG80211_AP_SUPPORT
		if (CFG_AP_ON(pAd))
			RTMP_SET_BCN_NUM(pAdSrc, 1);
		else
#endif /* RT_CFG80211_AP_SUPPORT */
			RTMP_SET_BCN_NUM(pAdSrc, 0);

		RTMP_SHIFT_BCN_OFFSET0(pAdSrc, 0);
	}
}
#endif /* defined(CONFIG_AP_SUPPORT) || defined(RT_CFG80211_AP_SUPPORT) */

extern INT ApCliAllowToSendPacket(RTMP_ADAPTER * pAd, struct wifi_dev *wdev,
				  PNDIS_PACKET pPacket, UCHAR * pWcid);

BOOLEAN CFG80211DRV_OpsChgVirtualInf(RTMP_ADAPTER *pAd, VOID *pData)
{
	CMD_RTPRIV_IOCTL_80211_VIF_PARM *pVifParm = (CMD_RTPRIV_IOCTL_80211_VIF_PARM *) pData;
#if defined(RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE) || defined(CONFIG_AP_SUPPORT)
	struct net_device *pNetDev = pVifParm->net_dev;
	INT mbssidIdx = 0;
#endif /* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE || CONFIG_AP_SUPPORT */
#ifdef RT_CFG80211_P2P_SUPPORT
	struct wifi_dev *wdev = NULL;
	PCFG80211_CTRL pCfg80211_ctrl = &pAd->cfg80211_ctrl;
#endif /* RT_CFG80211_P2P_SUPPORT */
#ifdef RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
	APCLI_STRUCT *pApCliEntry;
#endif /* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE */


#ifdef RT_CFG80211_P2P_SUPPORT
	CFG80211DBG(RT_DEBUG_TRACE,
		    ("80211> %s - %s newType %d oldType %d\n", __func__,
		    pNetDev->name, pVifParm->oldIfType, pVifParm->newIfType));
#endif /* RT_CFG80211_P2P_SUPPORT */

#ifdef RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
	if (strcmp(pVifParm->net_dev->name, "p2p0") == 0) {
		switch (pVifParm->newIfType) {
		case RT_CMD_80211_IFTYPE_MONITOR:
			DBGPRINT(RT_DEBUG_TRACE, ("CFG80211 I/F Monitor Type\n"));
			/* RTMP_OS_NETDEV_SET_TYPE_MONITOR(new_dev_p); */
			break;

		case RT_CMD_80211_IFTYPE_STATION:
			RTMP_CFG80211_RemoveVifEntry(pAd,
						     pCfg80211_ctrl->dummy_p2p_net_dev);
			RTMP_CFG80211_AddVifEntry(pAd,
						  pCfg80211_ctrl->dummy_p2p_net_dev,
						  pVifParm->newIfType);
			break;

		case RT_CMD_80211_IFTYPE_P2P_CLIENT:
			pApCliEntry = &pAd->ApCfg.ApCliTab[MAIN_MBSSID];
			wdev = &pApCliEntry->wdev;
			wdev->wdev_type = WDEV_TYPE_STA;
			wdev->func_dev = pApCliEntry;
			wdev->sys_handle = (void *)pAd;
			wdev->if_dev = pCfg80211_ctrl->dummy_p2p_net_dev;
			wdev->tx_pkt_allowed = ApCliAllowToSendPacket;

			RTMP_CFG80211_RemoveVifEntry(pAd, pCfg80211_ctrl->dummy_p2p_net_dev);
			RTMP_CFG80211_AddVifEntry(pAd, pCfg80211_ctrl->dummy_p2p_net_dev,
						  pVifParm->newIfType);

			RTMP_OS_NETDEV_SET_PRIV(pCfg80211_ctrl->dummy_p2p_net_dev, pAd);
			RTMP_OS_NETDEV_SET_WDEV(pCfg80211_ctrl->dummy_p2p_net_dev, wdev);
			if (rtmp_wdev_idx_reg(pAd, wdev) < 0) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("%s: Assign wdev idx for %s failed, free net device!\n",
					  __func__,
					  RTMP_OS_NETDEV_GET_DEVNAME
					  (pCfg80211_ctrl->dummy_p2p_net_dev)));
				RtmpOSNetDevFree(pCfg80211_ctrl->dummy_p2p_net_dev);
				pCfg80211_ctrl->dummy_p2p_net_dev = NULL;
				break;
			}

			/* init MAC address of virtual network interface */
			COPY_MAC_ADDR(wdev->if_addr, pAd->cfg80211_ctrl.P2PCurrentAddress);

			pAd->flg_apcli_init = TRUE;
			ApCli_Open(pAd, pCfg80211_ctrl->dummy_p2p_net_dev);
			break;

		case RT_CMD_80211_IFTYPE_P2P_GO:
			mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAd, pNetDev);
			/* pNetDevOps->priv_flags = INT_P2P; */
			pAd->ApCfg.MBSSID[mbssidIdx].MSSIDDev = NULL;
			/* The Behivaor in SetBeacon Ops        */
			/* pAd->ApCfg.MBSSID[MAIN_MBSSID].MSSIDDev = new_dev_p; */
			RTMP_CFG80211_RemoveVifEntry(pAd,
						     pCfg80211_ctrl->dummy_p2p_net_dev);
			RTMP_CFG80211_AddVifEntry(pAd,
						  pCfg80211_ctrl->dummy_p2p_net_dev,
						  pVifParm->newIfType);
#if defined(CONFIG_DFS_SLAVE) && defined(CONFIG_P2PGO_DFS_SLAVE)
			wdev = &pAd->ApCfg.MBSSID[mbssidIdx].wdev;
			wdev->wdev_type = WDEV_TYPE_AP;
			wdev->func_dev = &pAd->ApCfg.MBSSID[mbssidIdx];
			wdev->func_idx = mbssidIdx;
			wdev->sys_handle = (void *)pAd;

			RTMP_OS_NETDEV_SET_PRIV(pCfg80211_ctrl->dummy_p2p_net_dev, pAd);
			RTMP_OS_NETDEV_SET_WDEV(pCfg80211_ctrl->dummy_p2p_net_dev, wdev);

			RTMPSetPhyMode(pAd, pAd->CommonCfg.PhyMode);
#endif /* endif */

			pAd->cfg80211_ctrl.isCfgInApMode = RT_CMD_80211_IFTYPE_AP;
			COPY_MAC_ADDR(pAd->ApCfg.MBSSID[mbssidIdx].wdev.if_addr,
				      pAd->cfg80211_ctrl.P2PCurrentAddress);
			COPY_MAC_ADDR(pAd->ApCfg.MBSSID[mbssidIdx].wdev.bssid,
				      pAd->cfg80211_ctrl.P2PCurrentAddress);
			break;

		default:
			DBGPRINT(RT_DEBUG_ERROR, ("Unknown CFG80211 I/F Type (%d)\n",
						  pVifParm->newIfType));
		}

		if ((pVifParm->newIfType == RT_CMD_80211_IFTYPE_STATION) &&
		    (pVifParm->oldIfType == RT_CMD_80211_IFTYPE_P2P_CLIENT) &&
		    (pAd->flg_apcli_init == TRUE)) {
			DBGPRINT(RT_DEBUG_TRACE, ("ApCli_Close\n"));
			CFG80211OS_ScanEnd(pAd->pCfg80211_CB, TRUE);
			if (pAd->cfg80211_ctrl.FlgCfg80211Scanning)
				pAd->cfg80211_ctrl.FlgCfg80211Scanning = FALSE;
			pAd->flg_apcli_init = FALSE;
			RT_MOD_INC_USE_COUNT();
			pAd->cfg80211_ctrl.isCfgInApMode = RT_CMD_80211_IFTYPE_STATION;

			return ApCli_Close(pAd, pCfg80211_ctrl->dummy_p2p_net_dev);
		} else if ((pVifParm->newIfType == RT_CMD_80211_IFTYPE_STATION) &&
			   (pVifParm->oldIfType == RT_CMD_80211_IFTYPE_P2P_GO)) {
			DBGPRINT(RT_DEBUG_TRACE, ("GOi_Close\n"));
		}
	} else
		DBGPRINT(RT_DEBUG_TRACE, ("%s bypass setting\n", pVifParm->net_dev->name));
#endif /* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE */

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
#ifndef RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
	/* After P2P NEGO phase, the device type may be change from GC to GO
	   or no change. We remove the GC in VIF list if nego as GO case.
	 */
	if ((pVifParm->newIfType == RT_CMD_80211_IFTYPE_P2P_GO) &&
	    (pVifParm->oldIfType == RT_CMD_80211_IFTYPE_P2P_CLIENT)) {

		/* Unregister GC's wdev from list */
		wdev = RtmpOsGetNetDevWdev(pVifParm->net_dev);
		if (rtmp_wdev_idx_unreg(pAd, wdev) < 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("Assign %s wdev failed\n",
						RTMP_OS_NETDEV_GET_DEVNAME(pVifParm->net_dev)));
		}

		/* Register GO's wdev and reassign VIF net device's wdev */
		mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAd, pNetDev);
		wdev = &pAd->ApCfg.MBSSID[mbssidIdx].wdev;
		RTMP_OS_NETDEV_SET_PRIV(pVifParm->net_dev, pAd);
		RTMP_OS_NETDEV_SET_WDEV(pVifParm->net_dev, wdev);
		if (rtmp_wdev_idx_reg(pAd, wdev) < 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("Assign %s wdev failed\n",
						RTMP_OS_NETDEV_GET_DEVNAME(pVifParm->net_dev)));
		}

		RTMP_CFG80211_VirtualIF_CancelP2pClient(pAd);
	}
#endif /* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE */
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

#ifdef RT_CFG80211_P2P_SINGLE_DEVICE

	CFG80211DBG(RT_DEBUG_TRACE, ("80211> @@@ Change from %u  to %u Mode\n",
				     pVifParm->oldIfType, pVifParm->newIfType));

	pCfg80211_ctrl->P2POpStatusFlags = CFG_P2P_DISABLE;
	if (pVifParm->newIfType == RT_CMD_80211_IFTYPE_P2P_CLIENT)
		pCfg80211_ctrl->P2POpStatusFlags = CFG_P2P_CLI_UP;
	else if (pVifParm->newIfType == RT_CMD_80211_IFTYPE_P2P_GO)
		pCfg80211_ctrl->P2POpStatusFlags = CFG_P2P_GO_UP;
#endif /* RT_CFG80211_P2P_SINGLE_DEVICE */

#ifdef CONFIG_STA_SUPPORT
	/* Change Device Type */
	if (pVifParm->newIfType == RT_CMD_80211_IFTYPE_ADHOC) {
		Set_NetworkType_Proc(pAd, "Adhoc");
	} else if ((pVifParm->newIfType == RT_CMD_80211_IFTYPE_STATION) ||
		   (pVifParm->newIfType == RT_CMD_80211_IFTYPE_P2P_CLIENT)) {
		CFG80211DBG(RT_DEBUG_TRACE, ("80211> Change the Interface to STA Mode\n"));

#ifdef CONFIG_AP_SUPPORT
		if (pAd->cfg80211_ctrl.isCfgInApMode == RT_CMD_80211_IFTYPE_AP ||
		    pVifParm->oldIfType == RT_CMD_80211_IFTYPE_AP) {
			RTMP_CFG80211_RemoveVifEntry(pAd, pNetDev);
			if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_START_UP))
				CFG80211DRV_DisableApInterface(pAd, pNetDev, pVifParm->oldIfType);
		}
#endif /* CONFIG_AP_SUPPORT */

		pAd->cfg80211_ctrl.isCfgInApMode = RT_CMD_80211_IFTYPE_STATION;
	}
#ifdef CONFIG_AP_SUPPORT
	else if (pVifParm->newIfType == RT_CMD_80211_IFTYPE_AP) {
		struct net_device *extraAP = NULL;

		CFG80211DBG(RT_DEBUG_TRACE, ("80211> Change the Interface to AP Mode\n"));
		CFG80211DBG(RT_DEBUG_OFF, ("80211> net_dev->name=%s\n",
			    pNetDev->name));

		/* Now we only support single AP, we must ensure only one AP entry in Vif list */
		extraAP = RTMP_CFG80211_FindVifEntry_ByType(pAd, RT_CMD_80211_IFTYPE_AP);
		if (extraAP)
			RTMP_CFG80211_RemoveVifEntry(pAd, extraAP);

		RTMP_CFG80211_RemoveVifEntry(pAd, pNetDev);
		RTMP_CFG80211_AddVifEntry(pAd, pNetDev, RT_CMD_80211_IFTYPE_AP);
		mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAd, pNetDev);
		pAd->ApCfg.MBSSID[mbssidIdx].MSSIDDev = pNetDev;
		pAd->ApCfg.MBSSID[mbssidIdx].wdev.if_dev = pNetDev;
		NdisCopyMemory(pAd->cfg80211_ctrl.APCurrentAddress,
			       pNetDev->dev_addr, MAC_ADDR_LEN);
		pAd->cfg80211_ctrl.flg_cfg_ap_init = TRUE;
		pAd->cfg80211_ctrl.isCfgInApMode = RT_CMD_80211_IFTYPE_AP;
		return TRUE;
	}
#endif /* CONFIG_AP_SUPPORT */
#endif /*CONFIG_STA_SUPPORT */
	if (pVifParm->newIfType == RT_CMD_80211_IFTYPE_P2P_GO) {
		CFG80211DBG(RT_DEBUG_TRACE, ("80211> Change the Interface to AP Mode\n"));
		pAd->cfg80211_ctrl.isCfgInApMode = RT_CMD_80211_IFTYPE_AP;
	}
#ifdef CONFIG_STA_SUPPORT
	else if (pVifParm->newIfType == RT_CMD_80211_IFTYPE_MONITOR) {
		/* set packet filter */
		Set_NetworkType_Proc(pAd, "Monitor");

		if (pVifParm->MonFilterFlag != 0) {
			UINT32 Filter;

			RTMP_IO_READ32(pAd, RX_FILTR_CFG, &Filter);

			if ((pVifParm->MonFilterFlag & RT_CMD_80211_FILTER_FCSFAIL) ==
			    RT_CMD_80211_FILTER_FCSFAIL) {
				Filter = Filter & (~0x01);
			} else {
				Filter = Filter | 0x01;
			}

			if ((pVifParm->MonFilterFlag & RT_CMD_80211_FILTER_PLCPFAIL) ==
			    RT_CMD_80211_FILTER_PLCPFAIL) {
				Filter = Filter & (~0x02);
			} else {
				Filter = Filter | 0x02;
			}

			if ((pVifParm->MonFilterFlag & RT_CMD_80211_FILTER_CONTROL) ==
			    RT_CMD_80211_FILTER_CONTROL) {
				Filter = Filter & (~0xFF00);
			} else {
				Filter = Filter | 0xFF00;
			}

			if ((pVifParm->MonFilterFlag & RT_CMD_80211_FILTER_OTHER_BSS) ==
			    RT_CMD_80211_FILTER_OTHER_BSS) {
				Filter = Filter & (~0x08);
			} else {
				Filter = Filter | 0x08;
			}

			RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, Filter);
			pVifParm->MonFilterFlag = Filter;
		}
	}
#endif /*CONFIG_STA_SUPPORT */

	if ((pVifParm->newIfType == RT_CMD_80211_IFTYPE_P2P_CLIENT) ||
	    (pVifParm->newIfType == RT_CMD_80211_IFTYPE_P2P_GO)) {
		COPY_MAC_ADDR(pAd->cfg80211_ctrl.P2PCurrentAddress, pVifParm->net_dev->dev_addr);
	} else {
#ifdef RT_CFG80211_P2P_SUPPORT
		pCfg80211_ctrl->bP2pCliPmEnable = FALSE;
		pCfg80211_ctrl->bPreKeepSlient = FALSE;
		pCfg80211_ctrl->bKeepSlient = FALSE;
		pCfg80211_ctrl->NoAIndex = MAX_LEN_OF_MAC_TABLE;
		pCfg80211_ctrl->MyGOwcid = MAX_LEN_OF_MAC_TABLE;
		pCfg80211_ctrl->CTWindows = 0;	/* CTWindows and OppPS parameter field */
#endif /* RT_CFG80211_P2P_SUPPORT */
	}

	return TRUE;
}

#ifdef RT_CFG80211_P2P_SUPPORT
BOOLEAN RTMP_CFG80211_VIF_P2P_GO_ON(VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	PNET_DEV pNetDev = NULL;

	if (!pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList.size)
		return FALSE;

	pNetDev = RTMP_CFG80211_FindVifEntry_ByType(pAd, RT_CMD_80211_IFTYPE_P2P_GO);
	if (!pNetDev)
		return FALSE;

		return TRUE;
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

#ifdef RT_CFG80211_P2P_SINGLE_DEVICE
	if (CFG80211_P2P_TEST_BIT(pAd->cfg80211_ctrl.P2POpStatusFlags, CFG_P2P_GO_UP))
		return TRUE;
	else
		return FALSE;
#endif /* RT_CFG80211_P2P_SINGLE_DEVICE */

	return FALSE;
}

BOOLEAN RTMP_CFG80211_VIF_P2P_CLI_ON(VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	PNET_DEV pNetDev = NULL;

	pNetDev = RTMP_CFG80211_FindVifEntry_ByType(pAd, RT_CMD_80211_IFTYPE_P2P_CLIENT);
	if ((pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList.size > 0) &&
	    (pNetDev != NULL))
		return TRUE;
	else
		return FALSE;
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

#ifdef RT_CFG80211_P2P_SINGLE_DEVICE
	if (CFG80211_P2P_TEST_BIT(pAd->cfg80211_ctrl.P2POpStatusFlags, CFG_P2P_CLI_UP))
		return TRUE;
	else
		return FALSE;
#endif /* RT_CFG80211_P2P_SINGLE_DEVICE */

	return FALSE;
}

BOOLEAN RTMP_CFG80211_VIF_P2P_CLI_CONNECTED(VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;

	return pAd->ApCfg.ApCliTab[MAIN_MBSSID].CtrlCurrState == APCLI_CTRL_CONNECTED;
}

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
BOOLEAN CFG80211DRV_OpsVifAdd(VOID *pAdOrg, VOID *pData)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	CMD_RTPRIV_IOCTL_80211_VIF_SET *pVifInfo;

	pVifInfo = (CMD_RTPRIV_IOCTL_80211_VIF_SET *) pData;
#ifndef RT_CFG80211_AP_SUPPORT
	/* Already one VIF in list */
	if (pAd->cfg80211_ctrl.Cfg80211VifDevSet.isGoingOn)
		return FALSE;

	pAd->cfg80211_ctrl.Cfg80211VifDevSet.isGoingOn = TRUE;
#endif
	/* Already one p2p VIF in list */
	if (pVifInfo->vifType == RT_CMD_80211_IFTYPE_P2P_GO ||
	   pVifInfo->vifType == RT_CMD_80211_IFTYPE_P2P_CLIENT) {
		if (pAd->cfg80211_ctrl.Cfg80211VifDevSet.isP2PGoingOn)
			return FALSE;
		pAd->cfg80211_ctrl.Cfg80211VifDevSet.isP2PGoingOn = TRUE;
	}

	return RTMP_CFG80211_VirtualIF_Init(pAd, pVifInfo->vifName, pVifInfo->vifType);
}
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
#endif /* RT_CFG80211_P2P_SUPPORT */

#ifdef RT_CFG80211_AP_SUPPORT
BOOLEAN RTMP_CFG80211_VIF_AP_ON(VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;
	PNET_DEV pNetDev = NULL;

	if (pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList.size > 0)
		pNetDev = RTMP_CFG80211_FindVifEntry_ByType(pAd,
							    RT_CMD_80211_IFTYPE_AP);
	if ((pNetDev != NULL) && (pAd->cfg80211_ctrl.cfg_ap_is_working == TRUE))
		return TRUE;
	else
		return FALSE;
}

#endif /* RT_CFG80211_AP_SUPPORT */

BOOLEAN RTMP_CFG80211_VIF_ON(VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;

	return pAd->cfg80211_ctrl.Cfg80211VifDevSet.isGoingOn;
}

BOOLEAN RTMP_CFG80211_ROC_ON(VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;

	return pAd->cfg80211_ctrl.Cfg80211RocTimerRunning;
}

static
PCFG80211_VIF_DEV RTMP_CFG80211_FindVifEntry_ByMac(VOID *pAdSrc, PNET_DEV pNewNetDev)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	PLIST_HEADER pCacheList = &pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList;
	PCFG80211_VIF_DEV pDevEntry = NULL;
	PLIST_ENTRY pListEntry = NULL;

	pListEntry = pCacheList->pHead;
	pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	while (pDevEntry != NULL) {
		if (RTMPEqualMemory
		    (pDevEntry->net_dev->dev_addr, pNewNetDev->dev_addr, MAC_ADDR_LEN))
			return pDevEntry;

		pListEntry = pListEntry->pNext;
		pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	}

	return NULL;
}

PNET_DEV RTMP_CFG80211_FindVifEntry_ByMacAddr(VOID *pAdSrc, UCHAR *pAddr)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	PLIST_HEADER pCacheList = &pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList;
	PCFG80211_VIF_DEV pDevEntry = NULL;
	PLIST_ENTRY pListEntry = NULL;

	pListEntry = pCacheList->pHead;
	pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	while (pDevEntry != NULL) {
		if (RTMPEqualMemory
		    (pDevEntry->net_dev->dev_addr, pAddr, MAC_ADDR_LEN))
			return pDevEntry->net_dev;

		pListEntry = pListEntry->pNext;
		pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	}

	return NULL;
}


PNET_DEV RTMP_CFG80211_FindVifEntry_ByType(VOID *pAdSrc, UINT32 devType)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	PLIST_HEADER pCacheList = &pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList;
	PCFG80211_VIF_DEV pDevEntry = NULL;
	PLIST_ENTRY pListEntry = NULL;

	pListEntry = pCacheList->pHead;
	pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	while (pDevEntry != NULL) {
		if (pDevEntry->devType == devType)
			return pDevEntry->net_dev;

		pListEntry = pListEntry->pNext;
		pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	}

	return NULL;
}

PWIRELESS_DEV RTMP_CFG80211_FindVifEntryWdev_ByType(IN VOID *pAdSrc, UINT32 devType)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	PLIST_HEADER pCacheList = &pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList;
	PCFG80211_VIF_DEV pDevEntry = NULL;
	PLIST_ENTRY pListEntry = NULL;

	pListEntry = pCacheList->pHead;
	pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	while (pDevEntry != NULL) {
		if (pDevEntry->devType == devType)
			return pDevEntry->net_dev->ieee80211_ptr;

		pListEntry = pListEntry->pNext;
		pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	}

	return NULL;
}

VOID RTMP_CFG80211_AddVifEntry(VOID *pAdSrc, PNET_DEV pNewNetDev, UINT32 DevType)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	PCFG80211_VIF_DEV pNewVifDev = NULL;

	os_alloc_mem(NULL, (UCHAR **) &pNewVifDev, sizeof(CFG80211_VIF_DEV));
	if (pNewVifDev) {
		NdisZeroMemory(pNewVifDev, sizeof(CFG80211_VIF_DEV));

		pNewVifDev->pNext = NULL;
		pNewVifDev->net_dev = pNewNetDev;
		pNewVifDev->devType = DevType;
		NdisZeroMemory(pNewVifDev->CUR_MAC, MAC_ADDR_LEN);
		NdisCopyMemory(pNewVifDev->CUR_MAC, pNewNetDev->dev_addr, MAC_ADDR_LEN);

		insertTailList(&pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList,
			       (PLIST_ENTRY) pNewVifDev);
		DBGPRINT(RT_DEBUG_TRACE,
			 ("Add CFG80211 VIF Device, Type: %d.\n", pNewVifDev->devType));
	} else {
		DBGPRINT(RT_DEBUG_ERROR, ("Error in alloc mem in New CFG80211 VIF Function.\n"));
	}
}

VOID RTMP_CFG80211_RemoveVifEntry(VOID *pAdSrc, PNET_DEV pNewNetDev)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	PLIST_ENTRY pListEntry = NULL;

	pListEntry = (PLIST_ENTRY) RTMP_CFG80211_FindVifEntry_ByMac(pAd, pNewNetDev);

	if (pListEntry) {
		delEntryList(&pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList, pListEntry);
		os_free_mem(NULL, pListEntry);
	} else {
		DBGPRINT(RT_DEBUG_ERROR, ("Error in RTMP_CFG80211_RemoveVifEntry.\n"));
	}
}

PNET_DEV RTMP_CFG80211_VirtualIF_Get(IN VOID *pAdSrc)
{
	/* PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc; */
	/* return pAd->Cfg80211VifDevSet.Cfg80211VifDev[0].net_dev; */
	return NULL;
}

#ifdef RT_CFG80211_P2P_SUPPORT
VOID RTMP_CFG80211_VirtualIF_CancelP2pClient(VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	PLIST_HEADER pCacheList = &pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList;
	PCFG80211_VIF_DEV pDevEntry = NULL;
	PLIST_ENTRY pListEntry = NULL;

	DBGPRINT(RT_DEBUG_TRACE, ("==> %s\n", __func__));

	pListEntry = pCacheList->pHead;
	pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	while (pDevEntry != NULL) {
		if (pDevEntry->devType == RT_CMD_80211_IFTYPE_P2P_CLIENT) {
			DBGPRINT(RT_DEBUG_ERROR,
				 ("==> RTMP_CFG80211_VirtualIF_CancelP2pClient HIT.\n"));
			pDevEntry->devType = RT_CMD_80211_IFTYPE_P2P_GO;
			break;
		}

		pListEntry = pListEntry->pNext;
		pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	}

	pAd->flg_apcli_init = FALSE;
	pAd->ApCfg.ApCliTab[MAIN_MBSSID].wdev.if_dev = NULL;

	DBGPRINT(RT_DEBUG_TRACE, ("<== %s\n", __func__));
}
#endif /* RT_CFG80211_P2P_SUPPORT */

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
static INT CFG80211_VirtualIF_Open(PNET_DEV dev_p)
{
	VOID *pAdSrc = RTMP_OS_NETDEV_GET_PRIV(dev_p);
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;

	ASSERT(pAdSrc);
	DBGPRINT(RT_DEBUG_TRACE, ("%s: ===> %d,%s\n", __func__, dev_p->ifindex,
				  RTMP_OS_NETDEV_GET_DEVNAME(dev_p)));
#ifdef CFG80211_STA_AP_CONCURRENT
	if (IS_AP_DEV(dev_p->name)) {
		if (VIRTUAL_IF_UP(pAd) != 0)
			return -1;
	}
#endif
	/* increase MODULE use count */
	RT_MOD_INC_USE_COUNT();

#ifdef RT_CFG80211_P2P_SUPPORT
	if (dev_p->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_P2P_CLIENT) {
		DBGPRINT(RT_DEBUG_TRACE, ("CFG80211_VirtualIF_Open\n"));
		pAd->flg_apcli_init = TRUE;
		ApCli_Open(pAd, dev_p);
		return 0;
	}
#endif /* RT_CFG80211_P2P_SUPPORT */

#ifdef CONFIG_SNIFFER_SUPPORT
#ifdef CONFIG_AP_SUPPORT
	if (dev_p->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_MONITOR) {
		pAd->ApCfg.BssType = BSS_MONITOR;
		pAd->sniffer_ctl.sniffer_type = RADIOTAP_TYPE;
		AsicSetRxFilter(pAd);
	}
#endif /*CONFIG_AP_SUPPORT */
#endif /* CONFIG_SNIFFER_SUPPORT */
	RTMP_OS_NETDEV_START_QUEUE(dev_p);
#ifdef CFG80211_STA_AP_CONCURRENT
	if (IS_AP_DEV(dev_p->name))	{
		netif_carrier_on(dev_p);
		netif_wake_queue(dev_p);
	}
#endif

#ifdef RT_CFG80211_AP_SUPPORT
	if (dev_p->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_AP) {
		RTMP_CFG80211_ENABLE_BEACON(pAd, dev_p);
		pAd->cfg80211_ctrl.cfg_ap_is_working = TRUE;
		netif_carrier_on(dev_p);
		netif_wake_queue(dev_p);
	}
#endif /* RT_CFG80211_AP_SUPPORT */

	DBGPRINT(RT_DEBUG_TRACE, ("%s: <=== %s\n", __func__, RTMP_OS_NETDEV_GET_DEVNAME(dev_p)));

	return 0;
}

static INT CFG80211_VirtualIF_Close(PNET_DEV dev_p)
{
	VOID *pAdSrc = RTMP_OS_NETDEV_GET_PRIV(dev_p);
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;

	ASSERT(pAdSrc);

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s: ===> %s (iftype=0x%x)\n", __func__, RTMP_OS_NETDEV_GET_DEVNAME(dev_p),
		  dev_p->ieee80211_ptr->iftype));

#ifdef RT_CFG80211_P2P_SUPPORT
	if (dev_p->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_P2P_CLIENT) {
		DBGPRINT(RT_DEBUG_TRACE, ("CFG80211_VirtualIF_Close\n"));
		CFG80211DRV_OpsScanInLinkDownAction(pAd);
		RT_MOD_DEC_USE_COUNT();
		return ApCli_Close(pAd, dev_p);
	}
#endif /* RT_CFG80211_P2P_SUPPORT */
#ifdef CONFIG_SNIFFER_SUPPORT
#ifdef CONFIG_AP_SUPPORT
	if (dev_p->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_MONITOR) {
		pAd->ApCfg.BssType = BSS_INFRA;
		AsicSetRxFilter(pAd);
	}
#endif /*CONFIG_AP_SUPPORT */
#endif /*CONFIG_SNIFFER_SUPPORT */

#ifdef RT_CFG80211_AP_SUPPORT
	if (dev_p->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_AP) {
		RTMP_CFG80211_DISABLE_BEACON(pAd, dev_p,
					     RT_CMD_80211_IFTYPE_AP);
		pAd->cfg80211_ctrl.cfg_ap_is_working = FALSE;
	}
#endif /* RT_CFG80211_AP_SUPPORT */

	DBGPRINT(RT_DEBUG_TRACE, ("%s: ===> %s\n", __func__, RTMP_OS_NETDEV_GET_DEVNAME(dev_p)));

	RTMP_OS_NETDEV_STOP_QUEUE(dev_p);

	if (netif_carrier_ok(dev_p))
		netif_carrier_off(dev_p);
#ifdef CONFIG_STA_SUPPORT
	if (INFRA_ON(pAd))
		AsicEnableBssSync(pAd);

	else if (ADHOC_ON(pAd))
		AsicEnableIbssSync(pAd);
#else
	else
		AsicDisableSync(pAd);
#endif /* endif */
#ifdef CFG80211_STA_AP_CONCURRENT
	if (IS_AP_DEV(dev_p->name))
		VIRTUAL_IF_DOWN(pAd);
#endif

	RT_MOD_DEC_USE_COUNT();
	return 0;
}

static INT CFG80211_PacketSend(PNDIS_PACKET pPktSrc, PNET_DEV pDev, RTMP_NET_PACKET_TRANSMIT Func)
{
	PRTMP_ADAPTER pAd;

	pAd = RTMP_OS_NETDEV_GET_PRIV(pDev);
	ASSERT(pAd);

	/* To Indicate from Which VIF */
	switch (pDev->ieee80211_ptr->iftype) {
	case RT_CMD_80211_IFTYPE_AP:
		/* minIdx = MIN_NET_DEVICE_FOR_CFG80211_VIF_AP; */
		RTMP_SET_PACKET_OPMODE(pPktSrc, OPMODE_AP);
		break;

	case RT_CMD_80211_IFTYPE_P2P_GO:
		/* minIdx = MIN_NET_DEVICE_FOR_CFG80211_VIF_P2P_GO; */
		if (!OPSTATUS_TEST_FLAG(pAd, fOP_AP_STATUS_MEDIA_STATE_CONNECTED)) {
			DBGPRINT(RT_DEBUG_TRACE,
				 ("Drop the Packet due P2P GO not in ready state\n"));
			RELEASE_NDIS_PACKET(pAd, pPktSrc, NDIS_STATUS_FAILURE);
			return 0;
		}
		RTMP_SET_PACKET_OPMODE(pPktSrc, OPMODE_AP);
		break;

	case RT_CMD_80211_IFTYPE_P2P_CLIENT:
		/* minIdx = MIN_NET_DEVICE_FOR_CFG80211_VIF_P2P_CLI; */
		RTMP_SET_PACKET_OPMODE(pPktSrc, OPMODE_AP);
		break;

	case RT_CMD_80211_IFTYPE_STATION:
	default:
		DBGPRINT(RT_DEBUG_TRACE,
			 ("Unknown CFG80211 I/F Type(%d)\n", pDev->ieee80211_ptr->iftype));
		RELEASE_NDIS_PACKET(pAd, pPktSrc, NDIS_STATUS_FAILURE);
		return 0;
	}

	DBGPRINT(RT_DEBUG_INFO, ("CFG80211 Packet Type  [%s](%d)\n",
				 pDev->name, pDev->ieee80211_ptr->iftype));
#ifdef RT_CFG80211_AP_SUPPORT
	if (pDev->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_AP)
		RTMP_SET_PACKET_NET_DEVICE_MBSSID(pPktSrc, SECOND_MBSSID);
	else
#endif /* RT_CFG80211_AP_SUPPORT */
#ifdef RT_CFG80211_P2P_SUPPORT
	if (pDev->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_P2P_GO)
		RTMP_SET_PACKET_NET_DEVICE_MBSSID(pPktSrc, FIRST_MBSSID);
	else
#endif /* RT_CFG80211_P2P_SUPPORT */
		RTMP_SET_PACKET_NET_DEVICE_MBSSID(pPktSrc, MAIN_MBSSID);

	return Func(RTPKT_TO_OSPKT(pPktSrc));
}

static INT CFG80211_VirtualIF_PacketSend(struct sk_buff *skb, PNET_DEV dev_p)
{
	struct wifi_dev *wdev;

	DBGPRINT(RT_DEBUG_INFO, ("%s ---> %s(iftype %d)\n", __func__,
				 dev_p->name, dev_p->ieee80211_ptr->iftype));

	if (!(RTMP_OS_NETDEV_STATE_RUNNING(dev_p))) {
		/* the interface is down */
		RELEASE_NDIS_PACKET(NULL, skb, NDIS_STATUS_FAILURE);
		return 0;
	}

	/* The device not ready to send packt. */
	wdev = RTMP_OS_NETDEV_GET_WDEV(dev_p);
	ASSERT(wdev);
	if (!wdev)
		return -1;

	return CFG80211_PacketSend(skb, dev_p, rt28xx_packet_xmit);
}

static INT CFG80211_VirtualIF_Ioctl(IN PNET_DEV dev_p, IN OUT VOID *rq_p, IN INT cmd)
{

	RTMP_ADAPTER *pAd;

	pAd = RTMP_OS_NETDEV_GET_PRIV(dev_p);
	ASSERT(pAd);

	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
		return -ENETDOWN;

	DBGPRINT(RT_DEBUG_TRACE, ("%s --->\n", __func__));

	return rt28xx_ioctl(dev_p, rq_p, cmd);

}

#ifdef RT_CFG80211_AP_SUPPORT
static int RTMP_CFG80211_VirtualIF_SetMacAddress(struct net_device *dev, void *addr)
{
	struct sockaddr *sock_addr = addr;
	VOID *pAdSrc = RTMP_OS_NETDEV_GET_PRIV(dev);
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	INT mbssidIdx = 0;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
	if (!(dev->priv_flags & IFF_LIVE_ADDR_CHANGE) && netif_running(dev))
		return -EBUSY;
#endif

	if (!is_valid_ether_addr(sock_addr->sa_data))
		return -EADDRNOTAVAIL;

	if (!pAd)
		return -EOPNOTSUPP;

	DBGPRINT(RT_DEBUG_OFF, (
		"[%s] %02X:%02X:%02X:%02X:%02X:%02X\n",
		__func__,
		PRINT_MAC(sock_addr->sa_data)));

	mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAd, dev);

	/* Change MAC for virtual interface */
	COPY_MAC_ADDR(dev->dev_addr, sock_addr->sa_data);
	COPY_MAC_ADDR(pAd->ApCfg.MBSSID[mbssidIdx].wdev.if_addr, dev->dev_addr);
	COPY_MAC_ADDR(pAd->ApCfg.MBSSID[mbssidIdx].wdev.bssid, dev->dev_addr);
	COPY_MAC_ADDR(pAd->cfg80211_ctrl.APCurrentAddress, dev->dev_addr);


	AsicSetRepeaterMode(pAd, TRUE);
	AsicSetRepeaterExtAddr(pAd, sock_addr->sa_data);
	AsicSetApCliBssid(pAd, sock_addr->sa_data, 0);
	pAd->cfg80211_ctrl.ap_mac_is_changed = TRUE;

	return 0;
}
#endif /* RT_CFG80211_AP_SUPPORT */

BOOLEAN RTMP_CFG80211_VirtualIF_Init(IN VOID *pAdSrc, IN CHAR * pDevName, IN UINT32 DevType)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	RTMP_OS_NETDEV_OP_HOOK netDevHook, *pNetDevOps;
	PNET_DEV new_dev_p;
#ifdef RT_CFG80211_P2P_SUPPORT
	APCLI_STRUCT *pApCliEntry;
#endif /* RT_CFG80211_P2P_SUPPORT */
	struct wifi_dev *wdev;

	CHAR preIfName[IFNAMSIZ];
	UINT devNameLen = strlen(pDevName);
	UINT preIfIndex;
	CFG80211_CB *p80211CB = pAd->pCfg80211_CB;
	struct wireless_dev *pWdev;
	UINT32 MC_RowID = 0, IoctlIF = 0, Inf = INT_P2P;
	INT mbssidIdx = 0;

	if (devNameLen > IFNAMSIZ)
		devNameLen = IFNAMSIZ;
	preIfIndex = pDevName[devNameLen - 1] - 48;
	ASSERT(pDevName[devNameLen - 1] - 48 >= 0);
	NdisCopyMemory(preIfName, pDevName, devNameLen - 1);
	preIfName[devNameLen - 1] = '\0';

	pNetDevOps = &netDevHook;

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s ---> (%s, %s, %d)\n", __func__, pDevName, preIfName, preIfIndex));

	/* init operation functions and flags */
	NdisZeroMemory(&netDevHook, sizeof(netDevHook));
	netDevHook.open = CFG80211_VirtualIF_Open;	/* device opem hook point */
	netDevHook.stop = CFG80211_VirtualIF_Close;	/* device close hook point */
	netDevHook.xmit = CFG80211_VirtualIF_PacketSend;	/* hard transmit hook point */
	netDevHook.ioctl = CFG80211_VirtualIF_Ioctl;	/* ioctl hook point */

#if WIRELESS_EXT >= 12
	/* netDevHook.iw_handler = (void *)&rt28xx_ap_iw_handler_def; */
#endif /* WIRELESS_EXT >= 12 */

	new_dev_p =
	    RtmpOSNetDevCreate(MC_RowID, &IoctlIF, Inf, preIfIndex, sizeof(PRTMP_ADAPTER),
			       preIfName);

	if (new_dev_p == NULL) {
		/* allocation fail, exit */
		DBGPRINT(RT_DEBUG_ERROR, ("Allocate network device fail (CFG80211)...\n"));
		return FALSE;
	}
	DBGPRINT(RT_DEBUG_TRACE,
		("Register CFG80211 I/F (%s)\n", RTMP_OS_NETDEV_GET_DEVNAME(new_dev_p)));

	new_dev_p->destructor = free_netdev;
	RTMP_OS_NETDEV_SET_PRIV(new_dev_p, pAd);
	pNetDevOps->needProtcted = TRUE;
#ifdef CFG80211_STA_AP_CONCURRENT
	if (IS_AP_DEV(pDevName)) {
		UCHAR PermanentAddress[MAC_ADDR_LEN];

		RTMP_DRIVER_MAC_ADDR_GET(pAd, &PermanentAddress[0]);
		DBGPRINT(RT_DEBUG_TRACE, ("%s():MAC Addr - %02x:%02x:%02x:%02x:%02x:%02x\n",
				__func__, PermanentAddress[0], PermanentAddress[1],
				PermanentAddress[2], PermanentAddress[3],
				PermanentAddress[4], PermanentAddress[5]));

		/* Set up the Mac address */
		RtmpOSNetDevAddrSet(OPMODE_AP, new_dev_p, &PermanentAddress[0], NULL);
		COPY_MAC_ADDR(pNetDevOps->devAddr, PermanentAddress);
		DBGPRINT(RT_DEBUG_TRACE, ("New AP BSSID %02x:%02x:%02x:%02x:%02x:%02x\n",
			PRINT_MAC(new_dev_p->dev_addr)));
		pNetDevOps->needProtcted = FALSE;
	} else
#endif
	{
		/* Shared mac address is not allow for wpa_supplicant */
		NdisMoveMemory(&pNetDevOps->devAddr[0], &pAd->CurrentAddress[0], MAC_ADDR_LEN);

		if (pAd->chipCap.MBSSIDMode == MBSSID_MODE1) {
			/* for 4 BSSID mode, the range of mac addr byte0[2:3]is 0 ~ 2 */
			int i = 0, bssid_num = (1 << MULTI_BSSID_MODE2_4BSSID) - 1;
			unsigned char addr[6] = {0};
			/* We now apply 4 MBSSID mode, so use MAC address byte0[2:3]
			     as bss idx of p2p and softap interfaces */
			pNetDevOps->devAddr[0] &= 0xF3; /* clean bits byte0[2:3] */
			pNetDevOps->devAddr[0] |= 0x02; /* set byte0[1]=1 (admin bit) */

			COPY_MAC_ADDR(addr, pNetDevOps->devAddr);

			for (i = 0; i < bssid_num; ++i) {
				addr[0] = pNetDevOps->devAddr[0] | (i << 2);

				if (NdisEqualMemory(addr,
						pAd->cfg80211_ctrl.APCurrentAddress, 6))
					continue;

				if (NdisEqualMemory(addr,
						pAd->cfg80211_ctrl.P2PCurrentAddress, 6))
					continue;

				pNetDevOps->devAddr[0] = addr[0];
				break;
			}

			if (i == bssid_num) {
				DBGPRINT(RT_DEBUG_ERROR,
					("%s: Error! No available bss idx for %s\n",
					 __func__, preIfName));
				RtmpOSNetDevFree(new_dev_p);
				return FALSE;
			}
		} else {
				pNetDevOps->devAddr[5] += FIRST_MBSSID;
		}
	}

	pWdev = kzalloc(sizeof(*pWdev), GFP_KERNEL);

	new_dev_p->ieee80211_ptr = pWdev;
	pWdev->wiphy = p80211CB->pCfg80211_Wdev->wiphy;
	SET_NETDEV_DEV(new_dev_p, wiphy_dev(pWdev->wiphy));
	pWdev->netdev = new_dev_p;
	pWdev->iftype = DevType;

	switch (DevType) {
	case RT_CMD_80211_IFTYPE_MONITOR:
		DBGPRINT(RT_DEBUG_ERROR, ("CFG80211 I/F Monitor Type\n"));

		/* RTMP_OS_NETDEV_SET_TYPE_MONITOR(new_dev_p); */
		break;
#ifdef RT_CFG80211_P2P_SUPPORT
	case RT_CMD_80211_IFTYPE_P2P_CLIENT:
		pApCliEntry = &pAd->ApCfg.ApCliTab[MAIN_MBSSID];
		wdev = &pApCliEntry->wdev;
		wdev->wdev_type = WDEV_TYPE_STA;
		wdev->func_dev = pApCliEntry;
		wdev->sys_handle = (void *)pAd;
		wdev->if_dev = new_dev_p;
		wdev->tx_pkt_allowed = ApCliAllowToSendPacket;
		RTMP_OS_NETDEV_SET_PRIV(new_dev_p, pAd);
		RTMP_OS_NETDEV_SET_WDEV(new_dev_p, wdev);
		if (rtmp_wdev_idx_reg(pAd, wdev) < 0) {
			DBGPRINT(RT_DEBUG_ERROR,
				 ("%s: Assign wdev idx for %s failed, free net device!\n",
				  __func__, RTMP_OS_NETDEV_GET_DEVNAME(new_dev_p)));
			RtmpOSNetDevFree(new_dev_p);
			new_dev_p = NULL;
			wdev->if_dev = NULL;
			return FALSE;
		}

		/* init MAC address of virtual network interface */
		COPY_MAC_ADDR(wdev->if_addr, pNetDevOps->devAddr);
		break;
	case RT_CMD_80211_IFTYPE_P2P_GO:
		mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAd, new_dev_p);
		pNetDevOps->priv_flags = INT_P2P;
		pAd->ApCfg.MBSSID[mbssidIdx].MSSIDDev = NULL;
		/* The Behivaor in SetBeacon Ops        */
		/* pAd->ApCfg.MBSSID[MAIN_MBSSID].MSSIDDev = new_dev_p; */

#if defined(CONFIG_DFS_SLAVE) && defined(CONFIG_P2PGO_DFS_SLAVE)
		wdev = &pAd->ApCfg.MBSSID[mbssidIdx].wdev;
		wdev->wdev_type = WDEV_TYPE_AP;
		wdev->func_dev = &pAd->ApCfg.MBSSID[mbssidIdx];
		wdev->func_idx = mbssidIdx;
		wdev->sys_handle = (void *)pAd;
		wdev->if_dev = new_dev_p;

		RTMP_OS_NETDEV_SET_PRIV(new_dev_p, pAd);
		RTMP_OS_NETDEV_SET_WDEV(new_dev_p, wdev);

		RTMPSetPhyMode(pAd, pAd->CommonCfg.PhyMode);
#endif /* endif */

		pAd->cfg80211_ctrl.isCfgInApMode = RT_CMD_80211_IFTYPE_AP;
		COPY_MAC_ADDR(pAd->ApCfg.MBSSID[mbssidIdx].wdev.if_addr, pNetDevOps->devAddr);
		COPY_MAC_ADDR(pAd->ApCfg.MBSSID[mbssidIdx].wdev.bssid, pNetDevOps->devAddr);
		break;
#endif /* RT_CFG80211_P2P_SUPPORT */
#ifdef RT_CFG80211_AP_SUPPORT
	case RT_CMD_80211_IFTYPE_AP:
		mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAd, new_dev_p);
		pNetDevOps->priv_flags = INT_AP;
		pAd->ApCfg.MBSSID[mbssidIdx].MSSIDDev = NULL;
		pAd->cfg80211_ctrl.isCfgInApMode = RT_CMD_80211_IFTYPE_AP;

		COPY_MAC_ADDR(pAd->ApCfg.MBSSID[mbssidIdx].wdev.if_addr,
			      pNetDevOps->devAddr);
		COPY_MAC_ADDR(pAd->ApCfg.MBSSID[mbssidIdx].wdev.bssid,
			      pNetDevOps->devAddr);
		NdisCopyMemory(pAd->cfg80211_ctrl.APCurrentAddress,
			       pNetDevOps->devAddr, MAC_ADDR_LEN);

		netDevHook.set_mac_addr = RTMP_CFG80211_VirtualIF_SetMacAddress;
		break;
#endif /* RT_CFG80211_AP_SUPPORT*/
	default:
		DBGPRINT(RT_DEBUG_ERROR, ("Unknown CFG80211 I/F Type (%d)\n", DevType));
	}

	/* CFG_TODO : should be move to VIF_CHG */
	if ((DevType == RT_CMD_80211_IFTYPE_P2P_CLIENT) || (DevType == RT_CMD_80211_IFTYPE_P2P_GO))
		COPY_MAC_ADDR(pAd->cfg80211_ctrl.P2PCurrentAddress, pNetDevOps->devAddr);

	RtmpOSNetDevAttach(pAd->OpMode, new_dev_p, pNetDevOps);

	/* Record the pNetDevice to Cfg80211VifDevList */
	RTMP_CFG80211_AddVifEntry(pAd, new_dev_p, DevType);

	DBGPRINT(RT_DEBUG_TRACE, ("%s <---\n", __func__));
	return TRUE;
}
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

VOID RTMP_CFG80211_VirtualIF_Remove(IN VOID *pAdSrc, IN PNET_DEV dev_p, IN UINT32 DevType)
{
	BOOLEAN isGoOn = FALSE;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	BOOLEAN Cancelled = FALSE;

	if (dev_p) {
		if (pAd->cfg80211_ctrl.Cfg80211RocTimerRunning == TRUE) {
			CFG80211DBG(RT_DEBUG_OFF, ("%s : CANCEL Cfg80211RocTimer\n", __func__));
			RTMPCancelTimer(&pAd->cfg80211_ctrl.Cfg80211RocTimer, &Cancelled);
			pAd->cfg80211_ctrl.Cfg80211RocTimerRunning = FALSE;
		}
		isGoOn = RTMP_CFG80211_VIF_P2P_GO_ON(pAd);
		RTMP_CFG80211_RemoveVifEntry(pAd, dev_p);
		RTMP_OS_NETDEV_STOP_QUEUE(dev_p);
#ifndef RT_CFG80211_AP_SUPPORT
		pAd->cfg80211_ctrl.Cfg80211VifDevSet.isGoingOn = FALSE;
#endif /* RT_CFG80211_AP_SUPPORT */

#ifdef RT_CFG80211_AP_SUPPORT
		if (dev_p->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_AP) {
			if (pAd->cfg80211_ctrl.cfg_ap_net_dev) {
				INT mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAdSrc,
									       dev_p);
				DBGPRINT(RT_DEBUG_TRACE, ("Remove AP device\n"));

				RtmpOSNetDevDetach(dev_p);
				pAd->ApCfg.MBSSID[mbssidIdx].MSSIDDev = NULL;
			}
		} else
#endif /* RT_CFG80211_AP_SUPPORT */
#ifdef RT_CFG80211_P2P_SUPPORT
		if (isGoOn) {
			INT mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAdSrc,
								       dev_p);
#ifdef CONFIG_MULTI_CHANNEL
			PMULTISSID_STRUCT pMbss = &pAd->ApCfg.MBSSID[mbssidIdx];
			struct wifi_dev *pwdev = &pMbss->wdev;

			if (pAd->Mlme.bStartMcc == TRUE) {
				DBGPRINT(RT_DEBUG_TRACE, ("Group remove stop mcc\n"));
				pAd->chipCap.tssi_enable = TRUE;	/* let host do tssi */
				Stop_MCC(pAd, 1);
				pAd->Mlme.bStartMcc = FALSE;
			}

			if (pAd->Mlme.bStartScc == TRUE) {
				DBGPRINT(RT_DEBUG_TRACE,
					 ("GO remove & switch to Infra BW = %d  pAd->StaCfg.wdev.CentralChannel %d\n",
					  pAd->StaCfg.wdev.bw, pAd->StaCfg.wdev.CentralChannel));
				pAd->Mlme.bStartScc = FALSE;
				AsicSwitchChannel(pAd, pAd->StaCfg.wdev.CentralChannel, FALSE);
				AsicLockChannel(pAd, pAd->StaCfg.wdev.CentralChannel);
				bbp_set_bw(pAd, pAd->StaCfg.wdev.bw);
			}

			pwdev->channel = 0;
			pwdev->CentralChannel = 0;
			pwdev->bw = 0;
			pwdev->extcha = EXTCHA_NONE;

/*after p2p cli connect , neet to change to default configure*/
			pAd->CommonCfg.RegTransmitSetting.field.EXTCHA = EXTCHA_BELOW;
			pAd->CommonCfg.RegTransmitSetting.field.BW = BW_40;
			pAd->CommonCfg.HT_Disable = 0;
			SetCommonHT(pAd);

#endif /* CONFI1G_MULTI_CHANNEL */

			/* Always restore to default iftype for avoiding case without iftype change */
			pAd->cfg80211_ctrl.isCfgInApMode = RT_CMD_80211_IFTYPE_STATION;
			RtmpOSNetDevDetach(dev_p);
			pAd->ApCfg.MBSSID[mbssidIdx].MSSIDDev = NULL;
		} else if (pAd->flg_apcli_init) {
			struct wifi_dev *wdev;

			wdev = &pAd->ApCfg.ApCliTab[MAIN_MBSSID].wdev;

#ifdef CONFIG_MULTI_CHANNEL
			/* actually not mcc still need to check this! */

			if (pAd->Mlme.bStartMcc == TRUE) {
				DBGPRINT(RT_DEBUG_TRACE, ("@@@ GC remove stop mcc\n"));
				pAd->chipCap.tssi_enable = TRUE;	/* let host do tssi */
				Stop_MCC(pAd, 1);
				pAd->Mlme.bStartMcc = FALSE;
			} else
				/* if (pAd->Mlme.bStartScc == TRUE) */
			{
				DBGPRINT(RT_DEBUG_TRACE,
	("@@@ GC remove & switch to Infra BW = %d  pAd->StaCfg.wdev.CentralChannel %d\n",
					  pAd->StaCfg.wdev.bw, pAd->StaCfg.wdev.CentralChannel));
				pAd->Mlme.bStartScc = FALSE;
				AsicSwitchChannel(pAd, pAd->StaCfg.wdev.CentralChannel, FALSE);
				AsicLockChannel(pAd, pAd->StaCfg.wdev.CentralChannel);
				bbp_set_bw(pAd, pAd->StaCfg.wdev.bw);
			}

			wdev->CentralChannel = 0;
			wdev->channel = 0;
			wdev->bw = HT_BW_20;
			wdev->extcha = EXTCHA_NONE;
#endif /* CONFIG_MULTI_CHANNEL */
			OPSTATUS_CLEAR_FLAG(pAd, fOP_AP_STATUS_MEDIA_STATE_CONNECTED);
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3, 10, 0))
			if (dev_p->ieee80211_ptr->sme_state == CFG80211_SME_CONNECTED)
#endif
				cfg80211_disconnected(dev_p, WLAN_REASON_DEAUTH_LEAVING, NULL, 0,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 2, 0))
				TRUE,/* locally_generated */
#endif
				GFP_KERNEL);

			NdisZeroMemory(pAd->ApCfg.ApCliTab[MAIN_MBSSID].CfgApCliBssid,
				       MAC_ADDR_LEN);
			RtmpOSNetDevDetach(dev_p);
			rtmp_wdev_idx_unreg(pAd, wdev);
			pAd->flg_apcli_init = FALSE;
			wdev->if_dev = NULL;
		} else
#endif /* RT_CFG80211_P2P_SUPPORT */
		{
			/* Never Opened When New Netdevice on */
			RtmpOSNetDevDetach(dev_p);
		}

		if (!INFRA_ON(pAd)) {
			if (pAd->CommonCfg.Channel != 1) {
				pAd->CommonCfg.Channel = 1;
				pAd->CommonCfg.CentralChannel = 1;
#ifdef DOT11_VHT_AC
				pAd->CommonCfg.vht_cent_ch = 1;
#endif /* DOT11_VHT_AC */
				CFG80211DBG(RT_DEBUG_ERROR,
					    ("80211> %s, Restore to channel %d\n", __func__,
					     pAd->CommonCfg.Channel));
				AsicSwitchChannel(pAd, pAd->CommonCfg.Channel, FALSE);
				AsicLockChannel(pAd, pAd->CommonCfg.Channel);
			}
			/* Restore to BW 20 */
			bbp_set_bw(pAd, BW_20);
		}
		if (!RTMP_CFG80211_VIF_P2P_GO_ON(pAd) && !RTMP_CFG80211_VIF_P2P_CLI_ON(pAd))	{
			PCFG80211_CTRL cfg = &pAd->cfg80211_ctrl;
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
			PNET_DEV dummy = (PNET_DEV)cfg->dummy_p2p_net_dev;

			COPY_MAC_ADDR(cfg->P2PCurrentAddress,
				      dummy->dev_addr);
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
			DBGPRINT(RT_DEBUG_TRACE, ("%s(): P2PCurrentAddress %X:%X:%X:%X:%X:%X\n",
				 __func__, PRINT_MAC(cfg->P2PCurrentAddress)));
		}
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)
/* Do not free wireless dev due to NL80211_FLAG_NEED_WDEV is announced
* by new kernel's VIF remove operation
*/
		if (dev_p->ieee80211_ptr) {
			kfree(dev_p->ieee80211_ptr);
			dev_p->ieee80211_ptr = NULL;
		}
#endif /* LINUX_VERSION_CODE < KERNEL_VERSION(3,6,0) */

	}
}

VOID RTMP_CFG80211_AllVirtualIF_Remove(IN VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	PLIST_HEADER pCacheList = &pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList;
	PCFG80211_VIF_DEV pDevEntry = NULL;
	PLIST_ENTRY pListEntry = NULL;

	pListEntry = pCacheList->pHead;
	pDevEntry = (PCFG80211_VIF_DEV) pListEntry;

	while ((pDevEntry != NULL) && (pCacheList->size != 0)) {
		RtmpOSNetDevProtect(1);
		RTMP_CFG80211_VirtualIF_Remove(pAd, pDevEntry->net_dev,
					       pDevEntry->net_dev->ieee80211_ptr->iftype);
		RtmpOSNetDevProtect(0);

		pListEntry = pListEntry->pNext;
		pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	}
}

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
static INT CFG80211_DummyP2pIf_Open(IN PNET_DEV dev_p)
{
	struct wireless_dev *wdev = dev_p->ieee80211_ptr;
	VOID *pAdSrc;
	PRTMP_ADAPTER pAd;

	pAdSrc = RTMP_OS_NETDEV_GET_PRIV(dev_p);
	pAd = (PRTMP_ADAPTER) pAdSrc;

	if (!wdev)
		return -EINVAL;

	wdev->wiphy->interface_modes |= (BIT(NL80211_IFTYPE_P2P_CLIENT)
					 | BIT(NL80211_IFTYPE_P2P_GO));

	pAd->cfg80211_ctrl.P2POpStatusFlags |= CFG_P2P_IF_UP;

#ifdef RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
	RT_MOD_INC_USE_COUNT();
	/* ApCli_Open move to CFG80211DRV_OpsChgVirtualInf */
	RTMP_OS_NETDEV_START_QUEUE(dev_p);
#endif /* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE */

	AsicSetBssid(pAd, pAd->cfg80211_ctrl.P2PCurrentAddress);

	return 0;
}

static INT CFG80211_DummyP2pIf_Close(IN PNET_DEV dev_p)
{
	struct wireless_dev *wdev = dev_p->ieee80211_ptr;
#ifdef RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
	VOID *pAdSrc;
	PRTMP_ADAPTER pAd;
#endif /* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE */
	if (!wdev)
		return -EINVAL;
#ifdef RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
	pAdSrc = RTMP_OS_NETDEV_GET_PRIV(dev_p);
	ASSERT(pAdSrc);
	pAd = (PRTMP_ADAPTER) pAdSrc;

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s: ===> %s (iftype=0x%x)\n", __func__, RTMP_OS_NETDEV_GET_DEVNAME(dev_p),
		  dev_p->ieee80211_ptr->iftype));

	if (dev_p->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_P2P_CLIENT) {
		DBGPRINT(RT_DEBUG_TRACE, ("CFG80211_VirtualIF_Close\n"));
		CFG80211OS_ScanEnd(pAd->pCfg80211_CB, TRUE);
		if (pAd->cfg80211_ctrl.FlgCfg80211Scanning)
			pAd->cfg80211_ctrl.FlgCfg80211Scanning = FALSE;
		RT_MOD_DEC_USE_COUNT();
		ApCli_Close(pAd, dev_p);
	}

	RTMP_OS_NETDEV_STOP_QUEUE(dev_p);

	if (INFRA_ON(pAd))
		AsicEnableBssSync(pAd);
	else if (ADHOC_ON(pAd))
		AsicEnableIbssSync(pAd);
	else
		AsicDisableSync(pAd);

	/* VIRTUAL_IF_DOWN(pAd); */

	RT_MOD_DEC_USE_COUNT();

#endif /* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE */

	wdev->wiphy->interface_modes = (wdev->wiphy->interface_modes)
	    & (~(BIT(NL80211_IFTYPE_P2P_CLIENT) | BIT(NL80211_IFTYPE_P2P_GO)));

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))
	{
		extern const struct ieee80211_iface_combination *p_ra_iface_combinations_ap_sta;
		extern const INT ra_iface_combinations_ap_sta_num;

		wdev->wiphy->iface_combinations = p_ra_iface_combinations_ap_sta;
		wdev->wiphy->n_iface_combinations = ra_iface_combinations_ap_sta_num;
	}
#endif /* endif */

	wdev->iftype = NL80211_IFTYPE_STATION;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0))
	wdev->wiphy->software_iftypes |= BIT(NL80211_IFTYPE_P2P_DEVICE);
#endif /* LINUX_VERSION_CODE 3.7.0 */

	return 0;
}

static INT CFG80211_DummyP2pIf_Ioctl(IN PNET_DEV dev_p, IN OUT VOID *rq_p, IN INT cmd)
{
	RTMP_ADAPTER *pAd;

	pAd = RTMP_OS_NETDEV_GET_PRIV(dev_p);
	ASSERT(pAd);

	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
		return -ENETDOWN;

	DBGPRINT(RT_DEBUG_TRACE, ("%s --->\n", __func__));

	return rt28xx_ioctl(dev_p, rq_p, cmd);

}

static INT CFG80211_DummyP2pIf_PacketSend(
#ifdef RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
						 struct sk_buff *skb, PNET_DEV dev_p)
#else
						 IN PNDIS_PACKET skb_p, IN PNET_DEV dev_p)
#endif				/* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE */
{
#ifdef RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
	struct wifi_dev *wdev;

	DBGPRINT(RT_DEBUG_INFO, ("%s ---> %d\n", __func__, dev_p->ieee80211_ptr->iftype));

	if (!(RTMP_OS_NETDEV_STATE_RUNNING(dev_p))) {
		/* the interface is down */
		RELEASE_NDIS_PACKET(NULL, skb, NDIS_STATUS_FAILURE);
		return 0;
	}

	/* The device not ready to send packt. */
	wdev = RTMP_OS_NETDEV_GET_WDEV(dev_p);
	ASSERT(wdev);
	if (!wdev)
		return -1;

	return CFG80211_PacketSend(skb, dev_p, rt28xx_packet_xmit);

#else
	return 0;
#endif /* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE */
}

VOID RTMP_CFG80211_DummyP2pIf_Remove(IN VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	PCFG80211_CTRL cfg80211_ctrl = &pAd->cfg80211_ctrl;
	PNET_DEV dummy_p2p_net_dev = (PNET_DEV) cfg80211_ctrl->dummy_p2p_net_dev;
	struct wifi_dev *wdev = &cfg80211_ctrl->dummy_p2p_wdev;

	DBGPRINT(RT_DEBUG_TRACE, (" %s =====>\n", __func__));
	RtmpOSNetDevProtect(1);
	if (dummy_p2p_net_dev) {
#ifdef RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
		RTMP_CFG80211_RemoveVifEntry(pAd, dummy_p2p_net_dev);
#endif /* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE */

		RTMP_OS_NETDEV_STOP_QUEUE(dummy_p2p_net_dev);
#ifdef RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
		if (netif_carrier_ok(dummy_p2p_net_dev))
			netif_carrier_off(dummy_p2p_net_dev);
#endif /* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE */
		RtmpOSNetDevDetach(dummy_p2p_net_dev);
		rtmp_wdev_idx_unreg(pAd, wdev);
		wdev->if_dev = NULL;

		if (dummy_p2p_net_dev->ieee80211_ptr) {
			kfree(dummy_p2p_net_dev->ieee80211_ptr);
			dummy_p2p_net_dev->ieee80211_ptr = NULL;
		}

		RtmpOSNetDevProtect(0);
		RtmpOSNetDevFree(dummy_p2p_net_dev);
		cfg80211_ctrl->dummy_p2p_net_dev = NULL;
		RtmpOSNetDevProtect(1);

		cfg80211_ctrl->flg_cfg_dummy_p2p_init = FALSE;
	}
	RtmpOSNetDevProtect(0);
	DBGPRINT(RT_DEBUG_TRACE, (" %s <=====\n", __func__));
}

VOID RTMP_CFG80211_DummyP2pIf_Init(IN VOID *pAdSrc)
{
#define INF_CFG80211_DUMMY_P2P_NAME "p2p"

	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	CFG80211_CB *p80211CB = pAd->pCfg80211_CB;
	PCFG80211_CTRL cfg80211_ctrl = &pAd->cfg80211_ctrl;
	RTMP_OS_NETDEV_OP_HOOK netDevHook, *pNetDevOps;
	PNET_DEV new_dev_p;
	UINT32 MC_RowID = 0, IoctlIF = 0, Inf = INT_P2P;
	UINT preIfIndex = 0;
	struct wireless_dev *pWdev;
	struct wifi_dev *wdev = NULL;

	DBGPRINT(RT_DEBUG_TRACE, (" %s =====>\n", __func__));
	if (cfg80211_ctrl->flg_cfg_dummy_p2p_init != FALSE) {
		new_dev_p = cfg80211_ctrl->dummy_p2p_net_dev;
		pWdev = new_dev_p->ieee80211_ptr;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0))
		pWdev->iftype = RT_CMD_80211_IFTYPE_P2P_DEVICE;
#else
		pWdev->iftype = RT_CMD_80211_IFTYPE_P2P_CLIENT;
#endif /* LINUX_VERSION_CODE: 3.7.0 */
		/* interface_modes move from IF open to init */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0))
		pWdev->wiphy->interface_modes |= (BIT(NL80211_IFTYPE_P2P_CLIENT)
						  | BIT(NL80211_IFTYPE_P2P_GO));
		pWdev->wiphy->software_iftypes |= BIT(NL80211_IFTYPE_P2P_DEVICE);
#endif /* LINUX_VERSION_CODE 3.7.0 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0))
		{
			extern const struct ieee80211_iface_combination
			*p_ra_iface_combinations_p2p;
			extern const INT ra_iface_combinations_p2p_num;

			pWdev->wiphy->iface_combinations = p_ra_iface_combinations_p2p;
			pWdev->wiphy->n_iface_combinations = ra_iface_combinations_p2p_num;
		}
#endif /* KERNEL_VERSION >= 3.8.0 */
		DBGPRINT(RT_DEBUG_TRACE, (" %s <======== dummy p2p existed\n", __func__));
		return;
	}
#ifdef RT_CFG80211_P2P_SINGLE_DEVICE
	cfg80211_ctrl->P2POpStatusFlags = CFG_P2P_DISABLE;
#endif /* RT_CFG80211_P2P_SINGLE_DEVICE */

#if RT_CFG80211_P2P_SUPPORT
	cfg80211_ctrl->bP2pCliPmEnable = FALSE;
	cfg80211_ctrl->bPreKeepSlient = FALSE;
	cfg80211_ctrl->bKeepSlient = FALSE;
	cfg80211_ctrl->NoAIndex = MAX_LEN_OF_MAC_TABLE;
	cfg80211_ctrl->MyGOwcid = MAX_LEN_OF_MAC_TABLE;
	cfg80211_ctrl->CTWindows = 0;	/* CTWindows and OppPS parameter field */
#endif /* RT_CFG80211_P2P_SUPPORT */

	pNetDevOps = &netDevHook;

	/* init operation functions and flags */
	NdisZeroMemory(&netDevHook, sizeof(netDevHook));
	netDevHook.open = CFG80211_DummyP2pIf_Open;	/* device opem hook point */
	netDevHook.stop = CFG80211_DummyP2pIf_Close;	/* device close hook point */
	netDevHook.xmit = CFG80211_DummyP2pIf_PacketSend;	/* hard transmit hook point */
	netDevHook.ioctl = CFG80211_DummyP2pIf_Ioctl;	/* ioctl hook point */

	new_dev_p =
	    RtmpOSNetDevCreate(MC_RowID, &IoctlIF, Inf, preIfIndex, sizeof(PRTMP_ADAPTER),
			       INF_CFG80211_DUMMY_P2P_NAME);

	if (new_dev_p == NULL) {
		/* allocation fail, exit */
		DBGPRINT(RT_DEBUG_ERROR,
			 ("Allocate network device fail (CFG80211: Dummy P2P IF)...\n"));
		return;
	}
	DBGPRINT(RT_DEBUG_TRACE,
			("Register CFG80211 I/F (%s)\n", RTMP_OS_NETDEV_GET_DEVNAME(new_dev_p)));

	RTMP_OS_NETDEV_SET_PRIV(new_dev_p, pAd);

	/* Set local administration bit for unique mac address of p2p0 */
	NdisMoveMemory(&pNetDevOps->devAddr[0], &pAd->CurrentAddress[0], MAC_ADDR_LEN);
	pNetDevOps->devAddr[0] += 2;
	COPY_MAC_ADDR(pAd->cfg80211_ctrl.P2PCurrentAddress, pNetDevOps->devAddr);
	AsicSetBssid(pAd, pAd->cfg80211_ctrl.P2PCurrentAddress);

	pNetDevOps->needProtcted = TRUE;

	pWdev = kzalloc(sizeof(*pWdev), GFP_KERNEL);
	if (unlikely(!pWdev)) {
		DBGPRINT(RT_DEBUG_ERROR, ("Could not allocate wireless device\n"));
		return;
	}

	new_dev_p->ieee80211_ptr = pWdev;
	pWdev->wiphy = p80211CB->pCfg80211_Wdev->wiphy;
	SET_NETDEV_DEV(new_dev_p, wiphy_dev(pWdev->wiphy));
	pWdev->netdev = new_dev_p;
	/*
	   pWdev->iftype = RT_CMD_80211_IFTYPE_STATION;
	   pWdev->wiphy->interface_modes = (pWdev->wiphy->interface_modes)
	   & (~(BIT(NL80211_IFTYPE_P2P_CLIENT)|
	   BIT(NL80211_IFTYPE_P2P_GO)));
	 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0))
	pWdev->iftype = RT_CMD_80211_IFTYPE_P2P_DEVICE;
#else
	pWdev->iftype = RT_CMD_80211_IFTYPE_P2P_CLIENT;
#endif /* LINUX_VERSION_CODE: 3.7.0 */
	/* interface_modes move from IF open to init */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37))
	pWdev->wiphy->interface_modes |= (BIT(NL80211_IFTYPE_P2P_CLIENT)
					  | BIT(NL80211_IFTYPE_P2P_GO));
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0))
	pWdev->wiphy->software_iftypes |= BIT(NL80211_IFTYPE_P2P_DEVICE);
#endif /* LINUX_VERSION_CODE 3.7.0 */
#endif /* LINUX_VERSION_CODE 2.6.37 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))
	{
		extern const struct ieee80211_iface_combination *p_ra_iface_combinations_p2p;
		extern const INT ra_iface_combinations_p2p_num;

		pWdev->wiphy->iface_combinations = p_ra_iface_combinations_p2p;
		pWdev->wiphy->n_iface_combinations = ra_iface_combinations_p2p_num;
	}
#endif /* KERNEL_VERSION >= 3.8.0 */
	wdev = &cfg80211_ctrl->dummy_p2p_wdev;
	wdev->wdev_type = WDEV_TYPE_STA;
	wdev->sys_handle = (void *)pAd;
	wdev->if_dev = new_dev_p;
	COPY_MAC_ADDR(wdev->if_addr, pNetDevOps->devAddr);
	RTMP_OS_NETDEV_SET_PRIV(new_dev_p, pAd);
	RTMP_OS_NETDEV_SET_WDEV(new_dev_p, wdev);
	if (rtmp_wdev_idx_reg(pAd, wdev) < 0) {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("===============> fail register the wdev for dummy p2p\n"));
	}

	RtmpOSNetDevAttach(pAd->OpMode, new_dev_p, pNetDevOps);
	cfg80211_ctrl->dummy_p2p_net_dev = new_dev_p;
	cfg80211_ctrl->flg_cfg_dummy_p2p_init = TRUE;
#ifdef RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
	/* Record the pNetDevice to Cfg80211VifDevList */
	RTMP_CFG80211_AddVifEntry(pAd, new_dev_p, pWdev->iftype);
#endif /* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE */

	DBGPRINT(RT_DEBUG_TRACE, (" %s <=====\n", __func__));
}

VOID RTMP_CFG80211_DummyP2pIf_Finalize(IN VOID *pAdSrc)
{
	struct net_device_ops *pNetDevOps;
	PNET_DEV pNetDev = NULL;
	struct wireless_dev *wdev = NULL;

	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;
	PCFG80211_CTRL cfg80211_ctrl = &pAd->cfg80211_ctrl;

	DBGPRINT(RT_DEBUG_WARN, ("%s\n", __func__));

	if (!cfg80211_ctrl->flg_cfg_dummy_p2p_init)
		return;

	/* Find activated GO/GC VIF by type, close and remove it */
	if (RTMP_CFG80211_VIF_ON(pAd)) {
		pNetDev = RTMP_CFG80211_FindVifEntry_ByType(pAd, RT_CMD_80211_IFTYPE_P2P_GO);
		if (pNetDev) {
			pNetDevOps = (struct net_device_ops *)pNetDev->netdev_ops;

			if (pNetDevOps->ndo_stop)
				pNetDevOps->ndo_stop(pNetDev);

			RTMP_CFG80211_VirtualIF_Remove(pAd, pNetDev, RT_CMD_80211_IFTYPE_P2P_GO);
		}

		pNetDev = RTMP_CFG80211_FindVifEntry_ByType(pAd, RT_CMD_80211_IFTYPE_P2P_CLIENT);

		if (pNetDev) {
			pNetDevOps = (struct net_device_ops *)pNetDev->netdev_ops;

			if (pNetDevOps->ndo_stop)
				pNetDevOps->ndo_stop(pNetDev);

			RTMP_CFG80211_VirtualIF_Remove(pAd, pNetDev,
								RT_CMD_80211_IFTYPE_P2P_CLIENT);
		}
	}

	/* Close dummy p2p interface */
	pNetDev = cfg80211_ctrl->dummy_p2p_net_dev;
	pNetDevOps = (struct net_device_ops *)pNetDev->netdev_ops;

	if (pNetDevOps->ndo_stop)
		pNetDevOps->ndo_stop(pNetDev);

	wdev = pNetDev->ieee80211_ptr;
	wdev->wiphy->interface_modes &= ~(BIT(NL80211_IFTYPE_P2P_CLIENT)
						| BIT(NL80211_IFTYPE_P2P_GO));

	/* capability of wiphy should be restore to non-p2p */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))
	wdev->wiphy->iface_combinations = p_ra_iface_combinations_ap_sta;
	wdev->wiphy->n_iface_combinations = ra_iface_combinations_ap_sta_num;
#endif

	wdev->iftype = NL80211_IFTYPE_STATION;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0))
	wdev->wiphy->software_iftypes |= BIT(NL80211_IFTYPE_P2P_DEVICE);
#endif /* LINUX_VERSION_CODE 3.7.0 */
}

#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

#ifdef RT_CFG80211_AP_SUPPORT
#define INF_CFG80211_MULTI_AP_NAME "uap0"
VOID RTMP_CFG80211_ApIf_Init(VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;
	CMD_RTPRIV_IOCTL_80211_VIF_SET vifInfo;
	PCFG80211_CTRL cfg80211_ctrl = &pAd->cfg80211_ctrl;

	DBGPRINT(RT_DEBUG_TRACE, ("%s()  =====>\n", __func__));
	if (cfg80211_ctrl->flg_cfg_ap_init == TRUE)
		return;

	cfg80211_ctrl->cfg_ap_is_working = FALSE;
	cfg80211_ctrl->ap_mac_is_changed = FALSE;
	vifInfo.vifType = RT_CMD_80211_IFTYPE_AP;
	vifInfo.vifNameLen = strlen(INF_CFG80211_MULTI_AP_NAME);
	NdisZeroMemory(vifInfo.vifName, sizeof(vifInfo.vifName));
	NdisCopyMemory(vifInfo.vifName, INF_CFG80211_MULTI_AP_NAME,
		       vifInfo.vifNameLen);
	cfg80211_ctrl->flg_cfg_ap_init = TRUE;

	if (RTMP_DRIVER_80211_VIF_ADD(pAd, &vifInfo) != NDIS_STATUS_SUCCESS) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s() VIF Add error.\n", __func__));
		return;
	}

	cfg80211_ctrl->cfg_ap_net_dev =
		RTMP_CFG80211_FindVifEntry_ByType(pAd, RT_CMD_80211_IFTYPE_AP);
}

VOID RTMP_CFG80211_ApIf_Remove(VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;
	PCFG80211_CTRL cfg80211_ctrl = &pAd->cfg80211_ctrl;

	DBGPRINT(RT_DEBUG_TRACE, ("%s()  =====>\n", __func__));

	if (cfg80211_ctrl->cfg_ap_net_dev) {
		RtmpOSNetDevProtect(1);
		RTMP_DRIVER_80211_VIF_DEL(pAd, cfg80211_ctrl->cfg_ap_net_dev,
					  RT_CMD_80211_IFTYPE_AP);
		RtmpOSNetDevProtect(0);
		cfg80211_ctrl->flg_cfg_ap_init = FALSE;
		cfg80211_ctrl->cfg_ap_net_dev = NULL;
	}
}

#endif /* RT_CFG80211_AP_SUPPORT */
#endif /* RT_CFG80211_SUPPORT */
#ifdef CFG80211_STA_AP_CONCURRENT
#define INF_CFG80211_MULTI_AP_NAME "ap0"
BOOLEAN RTMP_CFG80211_MultiApIf_ON(VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;
	UINT bss_idx = 0;
	BOOLEAN isAnyApOn = FALSE;
	MULTISSID_STRUCT *pMbss = NULL;

	for (bss_idx = 0; bss_idx < pAd->ApCfg.BssidNum; bss_idx++) {
		pMbss = &pAd->ApCfg.MBSSID[bss_idx];
		if (pMbss->bBcnSntReq) {
			isAnyApOn = TRUE;
			break;
		}
	}

	return isAnyApOn;
}
VOID RTMP_CFG80211_MultiApIf_Init(VOID *pAdSrc)
{

	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;
	CMD_RTPRIV_IOCTL_80211_VIF_SET vifInfo;
	PCFG80211_CTRL cfg80211_ctrl = &pAd->cfg80211_ctrl;

	DBGPRINT(RT_DEBUG_TRACE, ("%s()  =====>\n", __func__));
	if (cfg80211_ctrl->flg_cfg_multi_ap_init == TRUE)
		return;

	vifInfo.vifType = RT_CMD_80211_IFTYPE_AP;
	vifInfo.vifNameLen = strlen(INF_CFG80211_MULTI_AP_NAME);
	NdisZeroMemory(vifInfo.vifName, sizeof(vifInfo.vifName));
	NdisCopyMemory(vifInfo.vifName, INF_CFG80211_MULTI_AP_NAME, vifInfo.vifNameLen);
	cfg80211_ctrl->flg_cfg_multi_ap_init = TRUE;

	if (RTMP_DRIVER_80211_VIF_ADD(pAd, &vifInfo) != NDIS_STATUS_SUCCESS) {
		DBGPRINT(RT_DEBUG_ERROR, (" %s() VIF Add error.\n", __func__));
		return;
	}

	cfg80211_ctrl->multi_ap_net_dev =
		RTMP_CFG80211_FindVifEntry_ByType(pAd, RT_CMD_80211_IFTYPE_AP);
}
VOID RTMP_CFG80211_MultiApIf_Remove(VOID *pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;
	PCFG80211_CTRL cfg80211_ctrl = &pAd->cfg80211_ctrl;

	DBGPRINT(RT_DEBUG_TRACE, ("%s()  =====>\n", __func__));

	if (cfg80211_ctrl->multi_ap_net_dev)	{
		RtmpOSNetDevProtect(1);
		RTMP_DRIVER_80211_VIF_DEL(pAd, cfg80211_ctrl->multi_ap_net_dev,
					RT_CMD_80211_IFTYPE_AP);
		RtmpOSNetDevProtect(0);
		cfg80211_ctrl->flg_cfg_multi_ap_init = FALSE;
	}
}

#endif /* CFG80211_STA_AP_CONCURRENT */
#endif /* LINUX_VERSION_CODE */
