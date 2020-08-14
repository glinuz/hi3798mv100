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

	Abstract:

	All related CFG80211 P2P function body.

	History:

***************************************************************************/

#define RTMP_MODULE_OS

#ifdef RT_CFG80211_SUPPORT

#include "rt_config.h"
#include "rtmp_comm.h"
#include "rt_os_util.h"
#include "rt_os_net.h"

VOID CFG80211_SwitchTxChannel(RTMP_ADAPTER *pAd, ULONG Data)
{
	/* UCHAR lock_channel = CFG80211_getCenCh(pAd, Data); */
	UCHAR lock_channel = Data;

#ifdef CONFIG_MULTI_CHANNEL
	PMULTISSID_STRUCT pMbss = &pAd->ApCfg.MBSSID[MAIN_MBSSID];
	struct wifi_dev *wdev = &pMbss->wdev;

	if ((pAd->MCC_InfraConnect_Protect == TRUE) || (pAd->Mlme.bStartMcc == TRUE)
	    || (pAd->MCC_GOConnect_Protect == TRUE)) {
		/* DBGPRINT(RT_DEBUG_TRACE,
	* ("Connecting Or MCC Enabled, Do not switch channel for Tx  lock_channel %d\n",
	* lock_channel));
		*/
		return;
	}
	if (pAd->Mlme.bStartScc == TRUE) {
		/* DBGPRINT(RT_DEBUG_TRACE,
		* ("SCC Enabled, Do not switch channel for Tx  %d\n",lock_channel));
		*/
		return;
	}
	if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd) && (wdev->channel == lock_channel) && (wdev->bw == 1)) {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("40 BW Enabled || GO enable , wait for CLI connect, Do not switch channel for Tx\n"));
		DBGPRINT(RT_DEBUG_TRACE,
			 ("GO wdev->channel  %d  lock_channel %d\n", wdev->channel, lock_channel));

		return;
	}
#endif /* CONFIG_MULTI_CHANNEL */

#ifdef CONFIG_MULTI_CHANNEL
	if ((INFRA_ON(pAd) && ((pAd->LatchRfRegs.Channel != pAd->StaCfg.wdev.CentralChannel)
			       && (pAd->StaCfg.wdev.CentralChannel != 0)))
	    || (pAd->LatchRfRegs.Channel != lock_channel)
	    )
#else /* CONFIG_MULTI_CHANNEL */
	if (RTMP_GetPrimaryCh(pAd, pAd->LatchRfRegs.Channel) != lock_channel)
#endif /* CONFIG_MULTI_CHANNEL */
		{

		if (INFRA_ON(pAd)) {
			DBGPRINT(RT_DEBUG_TRACE, ("CFG80211_NULL: STA PWR_SAVE ROC_START\n"));
			RTMPSendNullFrame(pAd, pAd->CommonCfg.TxRate,
					  (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_WMM_INUSED) ? TRUE :
					   FALSE), PWR_SAVE);
		}

		DBGPRINT(RT_DEBUG_TRACE, ("Off-Channel Send Packet: From(%d)-To(%d)\n",
					  pAd->LatchRfRegs.Channel, lock_channel));

		if (pAd->CommonCfg.BBPCurrentBW != BW_20)
			bbp_set_bw(pAd, BW_20);

		AsicSwitchChannel(pAd, lock_channel, FALSE);
		AsicLockChannel(pAd, lock_channel);

	} else
		DBGPRINT(RT_DEBUG_INFO,
			 ("Off-Channel Channel Equal: %d\n", pAd->LatchRfRegs.Channel));

}

#ifdef CONFIG_AP_SUPPORT

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
static UCHAR *CFG80211_find_ie(UCHAR eid, UCHAR *ies, int len)
{
	while (len > 2 && ies[0] != eid) {
		len -= ies[1] + 2;
		ies += ies[1] + 2;
	}
	if (len < 2)
		return NULL;
	if (len < 2 + ies[1])
		return NULL;
	return ies;
}

static UCHAR *CFG80211_find_vendor_ie(UINT oui, UCHAR oui_type, UCHAR *ies, int len)
{
	struct ieee80211_vendor_ie *ie;
	UCHAR *pos = ies, *end = ies + len;
	int ie_oui;

	while (pos < end) {
		pos = CFG80211_find_ie(WLAN_EID_VENDOR_SPECIFIC, pos, end - pos);
		if (!pos)
			return NULL;

		ie = (struct ieee80211_vendor_ie *)pos;
		if (ie->len < sizeof(*ie))
			goto cont;

		ie_oui = ie->oui[0] << 16 | ie->oui[1] << 8 | ie->oui[2];
		if (ie_oui == oui && ie->oui_type == oui_type)
			return pos;
cont:
		pos += 2 + ie->len;
	}
	return NULL;
}

BOOLEAN CFG80211_SyncPacketWmmIe(RTMP_ADAPTER *pAd, UINT mbss_idx, VOID *pData, ULONG dataLen)
{
	UINT WFA_OUI = 0x0050F2;
	UCHAR WMM_OUI_TYPE = 0x2;
	UCHAR *wmm_ie = NULL;
	UINT i = QID_AC_BE;

	/* hex_dump("probe_rsp_in:", pData, dataLen); */

	wmm_ie = CFG80211_find_vendor_ie(WFA_OUI, WMM_OUI_TYPE, pData, dataLen);
	if (wmm_ie != NULL) {
#ifdef UAPSD_SUPPORT
		if (pAd->ApCfg.MBSSID[mbss_idx].UapsdInfo.bAPSDCapable == TRUE)
			wmm_ie[8] |= 0x80;
#endif /* UAPSD_SUPPORT */

		/* WMM: sync from driver's EDCA paramter */
		for (i = QID_AC_BE; i <= QID_AC_VO; i++) {
			wmm_ie[10 + (i * 4)] = (i << 5) +	/* b5-6 is ACI */
			    ((UCHAR) pAd->ApCfg.BssEdcaParm.bACM[i] << 4) +	/* b4 is ACM */
			    (pAd->ApCfg.BssEdcaParm.Aifsn[i] & 0x0f);	/* b0-3 is AIFSN */

			wmm_ie[11 + (i * 4)] = (pAd->ApCfg.BssEdcaParm.Cwmax[i] << 4) +	/* b5-8 is CWMAX */
			    (pAd->ApCfg.BssEdcaParm.Cwmin[i] & 0x0f);	/* b0-3 is CWMIN */
			wmm_ie[12 + (i * 4)] = (UCHAR) (pAd->ApCfg.BssEdcaParm.Txop[i] & 0xff);	/* low byte of TXOP */
			wmm_ie[13 + (i * 4)] = (UCHAR) (pAd->ApCfg.BssEdcaParm.Txop[i] >> 8);	/* high byte of TXOP */
		}

		return TRUE;
	}

	return FALSE;

}
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0) */

VOID CFG80211_ParseBeaconIE(RTMP_ADAPTER *pAd, MULTISSID_STRUCT *pMbss, struct wifi_dev *wdev,
			    const UCHAR *wpa_ie, const UCHAR *rsn_ie)
{
	PEID_STRUCT pEid;
	PUCHAR pTmp;
	NDIS_802_11_ENCRYPTION_STATUS TmpCipher;
	NDIS_802_11_ENCRYPTION_STATUS PairCipher;	/* Unicast cipher 1, this one has more secured cipher suite */
	NDIS_802_11_ENCRYPTION_STATUS PairCipherAux;	/* Unicast cipher 2 if AP announce two unicast cipher suite */
	PAKM_SUITE_STRUCT pAKM;
	USHORT Count;
	BOOLEAN bWPA = FALSE;
	BOOLEAN bWPA2 = FALSE;
	BOOLEAN bMix = FALSE;

	/* Security */
	PairCipher = Ndis802_11WEPDisabled;
	PairCipherAux = Ndis802_11WEPDisabled;

	if ((wpa_ie == NULL) && (rsn_ie == NULL)) {	/* open case */
		DBGPRINT(RT_DEBUG_TRACE, ("%s:: Open/None case\n", __func__));
		wdev->AuthMode = Ndis802_11AuthModeOpen;
		wdev->WepStatus = Ndis802_11WEPDisabled;
		wdev->WpaMixPairCipher = MIX_CIPHER_NOTUSE;
	}

	if (wpa_ie != NULL) {	/* wpapsk/tkipaes case */
		pEid = (PEID_STRUCT) wpa_ie;
		pTmp = (PUCHAR) pEid;
		if (NdisEqualMemory(pEid->Octet, WPA_OUI, 4)) {
			wdev->AuthMode = Ndis802_11AuthModeOpen;
			DBGPRINT(RT_DEBUG_TRACE, ("%s:: WPA case\n", __func__));
			bWPA = TRUE;
			pTmp += 11;
			switch (*pTmp) {
			case 1:
				DBGPRINT(RT_DEBUG_TRACE, ("Group Ndis802_11GroupWEP40Enabled\n"));
				wdev->GroupKeyWepStatus = Ndis802_11GroupWEP40Enabled;
				break;
			case 5:
				DBGPRINT(RT_DEBUG_TRACE, ("Group Ndis802_11GroupWEP104Enabled\n"));
				wdev->GroupKeyWepStatus = Ndis802_11GroupWEP104Enabled;
				break;
			case 2:
				DBGPRINT(RT_DEBUG_TRACE, ("Group Ndis802_11TKIPEnable\n"));
				wdev->GroupKeyWepStatus = Ndis802_11TKIPEnable;
				break;
			case 4:
				DBGPRINT(RT_DEBUG_TRACE, (" Group Ndis802_11AESEnable\n"));
				wdev->GroupKeyWepStatus = Ndis802_11AESEnable;
				break;
			default:
				break;
			}
			/* number of unicast suite */
			pTmp += 1;

			/* skip all unicast cipher suites */
			/*Count = *(PUSHORT) pTmp;                              */
			Count = (pTmp[1] << 8) + pTmp[0];
			pTmp += sizeof(USHORT);

			/* Parsing all unicast cipher suite */
			while (Count > 0) {
				/* Skip OUI */
				pTmp += 3;
				TmpCipher = Ndis802_11WEPDisabled;
				switch (*pTmp) {
				case 1:
				case 5:	/* Although WEP is not allowed in WPA related auth mode, we parse it anyway */
					TmpCipher = Ndis802_11WEPEnabled;
					break;
				case 2:
					TmpCipher = Ndis802_11TKIPEnable;
					break;
				case 4:
					TmpCipher = Ndis802_11AESEnable;
					break;
				default:
					break;
				}
				if (TmpCipher > PairCipher) {
					/* Move the lower cipher suite to PairCipherAux */
					PairCipherAux = PairCipher;
					PairCipher = TmpCipher;
				} else {
					PairCipherAux = TmpCipher;
				}
				pTmp++;
				Count--;
			}
			switch (*pTmp) {
			case 1:
				/* Set AP support WPA-enterprise mode */
				wdev->AuthMode = Ndis802_11AuthModeWPA;
				break;
			case 2:
				/* Set AP support WPA-PSK mode */
				wdev->AuthMode = Ndis802_11AuthModeWPAPSK;
				break;
			default:
				break;
			}
			pTmp += 1;

			DBGPRINT(RT_DEBUG_TRACE, ("AuthMode = %s\n", GetAuthMode(wdev->AuthMode)));
			if (wdev->GroupKeyWepStatus == PairCipher) {
				wdev->WpaMixPairCipher = MIX_CIPHER_NOTUSE;
				pMbss->wdev.WepStatus = wdev->GroupKeyWepStatus;
			} else {
				DBGPRINT(RT_DEBUG_TRACE, ("WPA Mix TKIPAES\n"));

				bMix = TRUE;
			}
			pMbss->RSNIE_Len[0] = wpa_ie[1];
			NdisMoveMemory(pMbss->RSN_IE[0], wpa_ie + 2, wpa_ie[1]);	/* copy rsn ie */
		} else {
			DBGPRINT(RT_DEBUG_TRACE, ("%s:: Open/None case\n", __func__));
			wdev->AuthMode = Ndis802_11AuthModeOpen;
		}
	}
	if (rsn_ie != NULL) {
		PRSN_IE_HEADER_STRUCT pRsnHeader;
		PCIPHER_SUITE_STRUCT pCipher;

		pEid = (PEID_STRUCT) rsn_ie;
		pTmp = (PUCHAR) pEid;
		pRsnHeader = (PRSN_IE_HEADER_STRUCT) pTmp;

		/* 0. Version must be 1 */
		if (le2cpu16(pRsnHeader->Version) == 1) {
			pTmp += sizeof(RSN_IE_HEADER_STRUCT);

			/* 1. Check group cipher */
			pCipher = (PCIPHER_SUITE_STRUCT) pTmp;

			if (NdisEqualMemory(pTmp, RSN_OUI, 3)) {
				DBGPRINT(RT_DEBUG_TRACE, ("%s:: WPA2 case\n", __func__));
				bWPA2 = TRUE;
				wdev->AuthMode = Ndis802_11AuthModeOpen;
				switch (pCipher->Type) {
				case 1:
					DBGPRINT(RT_DEBUG_TRACE, ("Ndis802_11GroupWEP40Enabled\n"));
					wdev->GroupKeyWepStatus = Ndis802_11GroupWEP40Enabled;
					break;
				case 5:
					DBGPRINT(RT_DEBUG_TRACE,
						 ("Ndis802_11GroupWEP104Enabled\n"));
					wdev->GroupKeyWepStatus = Ndis802_11GroupWEP104Enabled;
					break;
				case 2:
					DBGPRINT(RT_DEBUG_TRACE, ("Ndis802_11TKIPEnable\n"));
					wdev->GroupKeyWepStatus = Ndis802_11TKIPEnable;
					break;
				case 4:
					DBGPRINT(RT_DEBUG_TRACE, ("Ndis802_11AESEnable\n"));
					wdev->GroupKeyWepStatus = Ndis802_11AESEnable;
					break;
				default:
					break;
				}

				/* set to correct offset for next parsing */
				pTmp += sizeof(CIPHER_SUITE_STRUCT);

				/* 2. Get pairwise cipher counts */
				/*Count = *(PUSHORT) pTmp; */
				Count = (pTmp[1] << 8) + pTmp[0];
				pTmp += sizeof(USHORT);

				/* 3. Get pairwise cipher */
				/* Parsing all unicast cipher suite */
				while (Count > 0) {
					/* Skip OUI */
					pCipher = (PCIPHER_SUITE_STRUCT) pTmp;
					TmpCipher = Ndis802_11WEPDisabled;
					switch (pCipher->Type) {
					case 1:
					case 5:
						/* Although WEP is not allowed in WPA
						* related auth mode,we parse it anyway
						*/
						TmpCipher = Ndis802_11WEPEnabled;
						break;
					case 2:
						TmpCipher = Ndis802_11TKIPEnable;
						break;
					case 4:
						TmpCipher = Ndis802_11AESEnable;
						break;
					default:
						break;
					}

					/* pMbss->wdev.WepStatus = TmpCipher; */
					if (TmpCipher > PairCipher) {
						/* Move the lower cipher suite to PairCipherAux */
						PairCipherAux = PairCipher;
						PairCipher = TmpCipher;
					} else {
						PairCipherAux = TmpCipher;
					}
					pTmp += sizeof(CIPHER_SUITE_STRUCT);
					Count--;
				}

				/* 4. get AKM suite counts */
				/*Count = *(PUSHORT) pTmp; */
				Count = (pTmp[1] << 8) + pTmp[0];
				pTmp += sizeof(USHORT);

				/* 5. Get AKM ciphers */
				/* Parsing all AKM ciphers */
				while (Count > 0) {
					pAKM = (PAKM_SUITE_STRUCT) pTmp;
					if (!RTMPEqualMemory(pTmp, RSN_OUI, 3))
						break;

					switch (pAKM->Type) {
					case 0:
						wdev->AuthMode = Ndis802_11AuthModeWPANone;
						break;
					case 1:
						/* Set AP support WPA-enterprise mode */
						wdev->AuthMode = Ndis802_11AuthModeWPA2;
						break;
					case 2:
						/* Set AP support WPA-PSK mode */
						wdev->AuthMode = Ndis802_11AuthModeWPA2PSK;
						break;
					default:
						wdev->AuthMode = Ndis802_11AuthModeMax;
						break;
					}
					pTmp += sizeof(AKM_SUITE_STRUCT);
					Count--;
				}
				DBGPRINT(RT_DEBUG_TRACE,
					 ("AuthMode = %s\n", GetAuthMode(wdev->AuthMode)));
				if (wdev->GroupKeyWepStatus == PairCipher) {
					wdev->WpaMixPairCipher = MIX_CIPHER_NOTUSE;
					pMbss->wdev.WepStatus = wdev->GroupKeyWepStatus;
				} else {
					DBGPRINT(RT_DEBUG_TRACE, ("WPA2 Mix TKIPAES\n"));
					bMix = TRUE;
				}
				pMbss->RSNIE_Len[0] = rsn_ie[1];
				NdisMoveMemory(pMbss->RSN_IE[0], rsn_ie + 2, rsn_ie[1]);	/* copy rsn ie */
			} else {
				DBGPRINT(RT_DEBUG_TRACE, ("%s:: Open/None case\n", __func__));
				wdev->AuthMode = Ndis802_11AuthModeOpen;
			}
		}
	}

	if (bWPA2 && bWPA) {
		if (bMix) {
			DBGPRINT(RT_DEBUG_TRACE, ("WPAPSK/WPA2PSK Mix TKIPAES\n"));
			wdev->WpaMixPairCipher = WPA_TKIPAES_WPA2_TKIPAES;
			wdev->WepStatus = Ndis802_11TKIPAESMix;
		}
	} else if (bWPA2) {
		if (bMix) {
			DBGPRINT(RT_DEBUG_TRACE, ("WPA2PSK Mix TKIPAES\n"));
			wdev->WpaMixPairCipher = WPA_NONE_WPA2_TKIPAES;
			wdev->WepStatus = Ndis802_11TKIPAESMix;
		}
	} else if (bWPA) {
		if (bMix) {
			DBGPRINT(RT_DEBUG_TRACE, ("WPAPSK Mix TKIPAES\n"));
			wdev->WpaMixPairCipher = WPA_TKIPAES_WPA2_NONE;
			wdev->WepStatus = Ndis802_11TKIPAESMix;
		}
	}
}

#endif /* CONFIG_AP_SUPPORT */

INT CFG80211_SetTxNdev(RTMP_ADAPTER *pAd, PNET_DEV pNetdev)
{
	PCFG80211_CTRL pCfg80211_ctrl = &pAd->cfg80211_ctrl;

	pCfg80211_ctrl->TxNdev = pNetdev;

	return TRUE;
}


INT CFG80211_SendMgmtFrame(RTMP_ADAPTER *pAd, VOID *pMgmtFrame)
{
	struct CMD_RTPRIV_IOCTL_CFG80211_MGMT_FRAME *mgmtFrame =
		(struct CMD_RTPRIV_IOCTL_CFG80211_MGMT_FRAME *)pMgmtFrame;
	struct net_device *pNetDev = mgmtFrame->pNetDev;

	DBGPRINT(RT_DEBUG_TRACE, ("%s =>\n", __func__));

	if (mgmtFrame->pBuf != NULL) {
		PCFG80211_CTRL pCfg80211_ctrl = &pAd->cfg80211_ctrl;

		pCfg80211_ctrl->TxStatusInUsed = TRUE;
		pCfg80211_ctrl->TxStatusSeq = pAd->Sequence;

#ifdef CONFIG_MULTI_CHANNEL
		if (pAd->MCC_InfraConnect_Protect == TRUE) {
			if (FALSE == pAd->cfg80211_ctrl.IsNeedTxStatus)
				CFG80211_SendMgmtFrameDone(pAd, 5678, FALSE);
			else
				CFG80211_SendMgmtFrameDone(pAd, (UCHAR)
					pCfg80211_ctrl->TxStatusSeq, FALSE);

			return 0;
		}
#endif /* CONFIG_MULTI_CHANNEL */

		RTMP_OS_INIT_COMPLETION(&pCfg80211_ctrl->fw_event_done);
		if (pCfg80211_ctrl->pTxStatusBuf != NULL) {
			os_free_mem(NULL, pCfg80211_ctrl->pTxStatusBuf);
			pCfg80211_ctrl->pTxStatusBuf = NULL;
		}

		os_alloc_mem(NULL, (UCHAR **) &pCfg80211_ctrl->pTxStatusBuf,
			     mgmtFrame->len);
		if (pCfg80211_ctrl->pTxStatusBuf != NULL) {
			NdisCopyMemory(pCfg80211_ctrl->pTxStatusBuf,
				       mgmtFrame->pBuf, mgmtFrame->len);
			pCfg80211_ctrl->TxStatusBufLen = mgmtFrame->len;
		} else {
			pCfg80211_ctrl->TxStatusBufLen = 0;
			DBGPRINT(RT_DEBUG_ERROR, ("CFG_TX_STATUS: MEM ALLOC ERROR\n"));
			return NDIS_STATUS_FAILURE;
		}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 3, 0)
		CFG80211_IsNeedTxStatus(pAd,
						       (UCHAR *)mgmtFrame->pBuf,
						       mgmtFrame->len,
						       pAd->cfg80211_ctrl.IsNeedTxStatus);
#else
		pAd->cfg80211_ctrl.IsNeedTxStatus = CFG80211_IsNeedTxStatus(pAd,
						       (UCHAR *)mgmtFrame->pBuf,
						       mgmtFrame->len,
						       pAd->cfg80211_ctrl.IsNeedTxStatus);
#endif
		if (TRUE == pAd->cfg80211_ctrl.IsNeedTxStatus) {
			int ret = 0;
			/* send fw cmd to FW */
			ret =
			    CFG80211_TxStatusCmd(pAd, TXS_TIMEOUT,
						 (UCHAR) pCfg80211_ctrl->TxStatusSeq,
						 TXS_P2P_WCID,
						 TXS_P2P_MgmtID,
						 mgmtFrame->len);
			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s, send tx status cmd status(%d)\n", __func__, ret));
		}
#ifdef CONFIG_MULTI_CHANNEL
		if ((pAd->Mlme.bStartMcc == TRUE) && RTMP_CFG80211_VIF_P2P_GO_ON(pAd)) {
			/*when MCC STAR, map MGMPACKET to HCCA */
			MiniportMMRequest(pAd, QID_HCCA, (UCHAR *) mgmtFrame->pBuf,
					  mgmtFrame->len);
		} else
#endif /* endif */
		{
#ifdef CONFIG_AP_SUPPORT
			struct ieee80211_mgmt *mgmt;

			mgmt = (struct ieee80211_mgmt *)mgmtFrame->pBuf;
			if (ieee80211_is_probe_resp(mgmt->frame_control)) {
				/* BOOLEAN res; */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0))
				INT offset = sizeof(HEADER_802_11) + 12;

				CFG80211_SyncPacketWmmIe(pAd,
						RTMP_CFG80211_GET_MBSSID_INDEX(pAd, pNetDev),
						(VOID *)(mgmtFrame->pBuf + offset),
						mgmtFrame->len - offset);
#endif /* endif */
				/* hex_dump("probe_rsp:", pData, Data); */
			}
#endif /* CONFIG_AP_SUPPORT */

			if (VENDOR_IE_ON(pAd) && pAd->vendorIELen != 0 &&
			    (pNetDev->ieee80211_ptr->iftype ==
			     RT_CMD_80211_IFTYPE_AP)) {
				UCHAR *tmp_buf = NULL;
				UINT32 tmp_buf_len = mgmtFrame->len +
						     pAd->vendorIELen;
				if (mgmtFrame->len != 0) {
					os_alloc_mem(NULL, (UCHAR **)&tmp_buf,
						     tmp_buf_len);
					if (tmp_buf != NULL) {
						NdisCopyMemory(tmp_buf,
							       mgmtFrame->pBuf,
							       mgmtFrame->len);
						NdisCopyMemory(tmp_buf +
							       mgmtFrame->len,
							       pAd->pVendorIE,
							       pAd->vendorIELen
							      );

						MiniportMMRequest(pAd, 0,
								  tmp_buf,
								  tmp_buf_len);
						os_free_mem(NULL, tmp_buf);
						return 0;
					}
				}
			}

			MiniportMMRequest(pAd, 0, (UCHAR *) mgmtFrame->pBuf,
					  mgmtFrame->len);
		}
		if (FALSE == pAd->cfg80211_ctrl.IsNeedTxStatus) {
			CFG80211_SendMgmtFrameDone(pAd, 5678, TRUE);
		} else {
			long expire = RTMPMsecsToJiffies(DRIVER_TXS_TIMEOUT);

			DBGPRINT(RT_DEBUG_ERROR,
				 ("%s,  start wait the pkt ack =========>\n", __func__));
			if (!RTMP_OS_WAIT_FOR_COMPLETION_TIMEOUT
			    (&pCfg80211_ctrl->fw_event_done, expire)) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("%s,  can't get tx status Timeout(%d)ms\n", __func__,
					  (DRIVER_TXS_TIMEOUT)));
				CFG80211_SendMgmtFrameDone(pAd,
					(UCHAR) pCfg80211_ctrl->TxStatusSeq,
					FALSE);
			}
			DBGPRINT(RT_DEBUG_ERROR,
				 ("%s,  end wait the pkt ack <=========\n", __func__));
		}
		RTMP_OS_EXIT_COMPLETION(&pCfg80211_ctrl->fw_event_done);
	}
	return 0;
}

VOID CFG80211_SendMgmtFrameDone(RTMP_ADAPTER *pAd, USHORT Sequence, BOOLEAN Ack)
{
/* RTMP_USB_SUPPORT/RTMP_PCI_SUPPORT */
	PCFG80211_CTRL pCfg80211_ctrl = &pAd->cfg80211_ctrl;

	if (pCfg80211_ctrl->pTxStatusBuf) {
		DBGPRINT(RT_DEBUG_INFO,
			 ("[%s] CFG_TX_STATUS: REAL send %d\n", pCfg80211_ctrl->TxNdev->name,
			  Sequence));
		CFG80211OS_TxStatus(pCfg80211_ctrl->TxNdev, Sequence, pCfg80211_ctrl->pTxStatusBuf,
				    pCfg80211_ctrl->TxStatusBufLen, Ack);
		pCfg80211_ctrl->TxStatusInUsed = FALSE;
	} else {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("[%s] TxStatusBuf = NULL, Seq (%d)\n", pCfg80211_ctrl->TxNdev->name,
			  Sequence));
	}
}

/*Check if need tx status filter*/
BOOLEAN CFG80211_IsNeedTxStatus(
				VOID *pAdOrg,
				UCHAR *pFrame,
				IN UINT32 FrameLength,
				BOOLEAN NeedTxStatus)
{
	BOOLEAN IsNeedTxStatus = NeedTxStatus;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)pFrame;

	DBGPRINT(RT_DEBUG_INFO, ("%s --->\n", __func__));
	if (FrameLength < sizeof(P2P_PUBLIC_FRAME)) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: Frame length(%d) < P2P_PUBLIC_FRAME size(%zd)\n",
					  __func__, FrameLength, sizeof(P2P_PUBLIC_FRAME)));
		return IsNeedTxStatus;
	}

	if (ieee80211_is_action(mgmt->frame_control)) {
		PP2P_PUBLIC_FRAME pP2PFrame = (PP2P_PUBLIC_FRAME) pFrame;
		if (pP2PFrame->Category == CATEGORY_PUBLIC) {
			switch (pP2PFrame->Action) {
			case ACTION_WIFI_DIRECT:
				DBGPRINT(RT_DEBUG_ERROR,
					("Tx %s - %s" MACSTR "->" MACSTR "in CH: %d\n",
					p2p_action_frame_txt(pP2PFrame->Subtype),
					IsNeedTxStatus == TRUE?"Need Tx Status":"",
					PRINT_MAC(mgmt->sa), PRINT_MAC(mgmt->da),
					pAd->LatchRfRegs.Channel));
				IsNeedTxStatus = TRUE;
				break;
			case ACTION_GAS_INITIAL_REQ:
				DBGPRINT(RT_DEBUG_ERROR,
					("ACTION_GAS_INITIAL_REQ\n"));
				break;
			case ACTION_GAS_INITIAL_RSP:
				DBGPRINT(RT_DEBUG_ERROR,
					("ACTION_GAS_INITIAL_RSP\n"));
				break;
			case ACTION_GAS_COMEBACK_REQ:
				DBGPRINT(RT_DEBUG_ERROR,
					("ACTION_GAS_COMEBACK_REQ\n"));
				break;
			case ACTION_GAS_COMEBACK_RSP:
				DBGPRINT(RT_DEBUG_ERROR,
					("ACTION_GAS_COMEBACK_RSP\n"));
				break;
			default:
				DBGPRINT(RT_DEBUG_ERROR,
					("unknown action frame\n"));
				break;
			}
		}
	}
#ifdef JUNGLE_SUPPORT
	else if (ieee80211_is_probe_resp(mgmt->frame_control)) {
		pAd->cfg80211_ctrl.IsNeedTxStatus = FALSE;
		DBGPRINT(RT_DEBUG_TRACE,
					 ("%s force probe_resp no ack, Seq = %d\n", __func__, pAd->Sequence));
	}
#endif  /* JUNGLE_SUPPORT */
	return IsNeedTxStatus;
}

/*
Send command to FW for polling pkt tx ok for specific WCID
FW must send the event of tx status when:
1. FW got the tx ok when polling the corresponding CR
2. CR polling timeout
we use fw_event_done to prevent that FW does not have response

for "1."  send txstatus to cfg80211 with ack
for "2." and no event,  send txstatus to cfg80211 with no ack

this is per packet action, we use CFG80211_IsNeedTxStatus() to filter which packet we want to watch

CMD behavior
WCID_X_SELECT (offset:0x178C, default: 0x0000_0000)
WCID_A_TX_CNT (offset:0x176C, default: 0x0000_0000) => A~H total 8 sets (wcid) & Read-Clear CR
1. set 0x178c = WCID
2. Polling (0x176c + (MgmtID*4)) for tx OK count until timeout
3. send event to driver
*/

int CFG80211_TxStatusCmd(RTMP_ADAPTER *pAd, UINT8 Timeout, UINT8 Seq, UINT8 WCID, UINT8 MgmtID,
			 UINT32 PktLen)
{
	int ret = 0;
	TX_STATUS_CTRL *TXStatus = &pAd->cfg80211_ctrl.TxStatusCtrl;

	CFG80211DBG(RT_DEBUG_INFO, ("%s --->\n", __func__));
	if (MgmtID > MAX_MGMTID) {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("%s, MgmtID(%d) exceeds max(%d)\n", __func__, MgmtID, MAX_MGMTID));
	}
	TXStatus->Timeout = Timeout;
	TXStatus->Seq = Seq;
	TXStatus->WCID = WCID;
	TXStatus->MgmtID = MgmtID;
	TXStatus->PktLen = PktLen;
	ret = andes_tx_status(pAd, TXStatus);
	return ret;
}

/*
Event command of TxStatus
TX_STATUS_CTRL_EVENT.Reg shows the tx ok and retry count

*/
VOID CFG80211_TxStatusEvent(RTMP_ADAPTER *pAd, UCHAR *EventContent, UINT32 Length)
{
	TX_STATUS_CTRL_EVENT *StatusEvent = (TX_STATUS_CTRL_EVENT *) EventContent;
	CFG80211_CTRL *pCfg80211_ctrl = &pAd->cfg80211_ctrl;
	BOOLEAN ack = FALSE;

	CFG80211DBG(RT_DEBUG_INFO, ("%s --->\n", __func__));
	if (Length >= sizeof(TX_STATUS_CTRL_EVENT)) {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s RX Seq = %d, WCID = %d,  MgmtID = %d, Timeout = %dms\n", __func__,
			  StatusEvent->Seq, StatusEvent->WCID, StatusEvent->MgmtID,
			  StatusEvent->Timeout));
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s Tx OK count (%d), Tx Retry count(%d)\n", __func__,
			  StatusEvent->Reg & 0xffff, StatusEvent->Reg >> 16));
		if ((StatusEvent->Reg & 0xffff) != 0)
			ack = TRUE;
		else
			ack = FALSE;

	} else {
		DBGPRINT(RT_DEBUG_ERROR, ("%s length(%d) smaller than expected %zd\n",
					  __func__, Length, sizeof(TX_STATUS_CTRL_EVENT)));
		ack = FALSE;
	}
	CFG80211_SendMgmtFrameDone(pAd, StatusEvent->Seq, ack);
	RTMP_OS_COMPLETE(&pCfg80211_ctrl->fw_event_done);
}
#endif /* RT_CFG80211_SUPPORT */
