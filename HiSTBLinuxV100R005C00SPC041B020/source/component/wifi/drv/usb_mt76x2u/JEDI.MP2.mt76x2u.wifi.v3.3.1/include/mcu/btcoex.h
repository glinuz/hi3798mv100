/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	mcu.h

	Abstract:
	MCU related information

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/

#ifndef __BTCOEX_H__
#define __BTCOEX_H__

struct _RTMP_ADAPTER;
struct _MAC_TABLE_ENTRY;

#define IS_MT76XXBTCOMBO(_pAd) (IS_MT7650(_pAd)  || IS_MT7630(_pAd) || IS_MT7662(_pAd) || IS_MT7632(_pAd))
/*  */
/* Antenna mode (ANT_MODE_XXX) */
/*  */
#define ANT_MODE_DUAL (0x01)	/* Chip is in dual antenna mode */
#define ANT_MODE_SINGLE (0x02)	/* Chip is in single antenna mode */

#define TypeTFSwitch						0x1
#define TypeProtectionFrame					0x2
#define TypeAFH							0x3
#define TypeWiFiStatus                                  0x4
#define TypeHostLoopBackTFSwitch							0xFFF1
#define TypeCoexCCUForceMode							0xFFF2

#define COEX_MODE_RESET           0
#define COEX_MODE_TDD               1
#define COEX_MODE_HYBRID          2
#define COEX_MODE_FDD               3
#define COEX_FDD_ANT_SET       0xC0
#define COEX_TDD_ANT_SET       0x00

#define COEX_OPMODE_STA                  1
#define COEX_OPMODE_AP                   2
#define COEX_OPMODE_GC                  3
#define COEX_OPMODE_GO               4
#define COEX_OPMODE_BT                   5
#define COEX_OPMODE_AD_HOC            6
#define COEX_GO_LINK	(1<<COEX_OPMODE_GO)
#define COEX_GC_LINK	(1<<COEX_OPMODE_GC)
#define COEX_STA_LINK	(1<<COEX_OPMODE_STA)

#define COEX_BARXSIZE_A2DP       8
#define COEX_BARXSIZE_SCO_ESCO   2
#define COEX_BARXSIZE_OPP        32

#define WLAN_NO_BSSID               0x0
#define WLAN_Device_OFF                 0x1
#define WLAN_Device_ON    0x2
#define WLAN_SCANREQEST                         0x3
#define WLAN_SCANDONE                              0x4
#define WLAN_SCANCANCEL                             0x5
#define WLAN_SCAN2GREQUET                             0x6
#define WLAN_SCAN5GREQUET                             0x7
#define WLAN_CONNECTION_START		0x14
#define WLAN_CONNECTION_COMPLETION	0x15
#define WLAN_CONNECTION_CANCEL_IND	0x16
#define WLAN_CONNECTION_JOIN_FAIL	0x17
#define WLAN_CONNECTION_AUTH_FAIL	0x18
#define WLAN_CONNECTION_ASSOC_FAIL	0x19
#define WLAN_DISCONNECT				0x1A
#define WLAN_ROAMING_START			0x1B
#define WLAN_ROAMING_COMPLETE             0x1C
#define WLAN_DeAuthConfirm	0x8
#define WLAN_AssocRequest	0x9
#define WLAN_AssocConfirm	0xA
#define WLAN_ReAssocRequest	0xB
#define WLAN_ReAssocConfirm	0xC
#define WLAN_DisAssocRequest	0xD
#define WLAN_INVINCIBLE_REQ 0xF1
#define WLAN_INVINCIBLE_ABORT 0xF2
#define WLAN_BEACON_SERVICE_REQ 0xF3
#define WLAN_BEACON_SERVICE_CONF 0xF4

#define fBTSTATUS_BT_DISABLE                  0x00000001
#define fBTSTATUS_BT_INACTIVE                0x00000002
#define fBTSTATUS_BT_SCANONLY               0x00000004
#define fBTSTATUS_BT_HOLD                       0x00000008
#define fBTSTATUS_BT_ACTIVE                    0x00000010

#define BT_STATUS_SET_FLAG(_pAd, _F)       ((_pAd)->BTStatusFlags |= (_F))
#define BT_STATUS_CLEAR_FLAG(_pAd, _F)     ((_pAd)->BTStatusFlags &= ~(_F))
#define BT_STATUS_TEST_FLAG(_pAd, _F)      (((_pAd)->BTStatusFlags & (_F)) != 0)
#define BT_STATUS_TEST_LINK_UP(_pAd)		(BT_STATUS_TEST_FLAG(_pAd, fBTSTATUS_BT_ACTIVE) && \
						(BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_BWL) || \
						BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_BE)   || \
						BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_SYNC) || \
						BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_SNIFF)))

#define NULLFRAMESPACE                  10
#define COEXNOZEROSHIFT                1
#define CTSTOSELF                  1
#define CFEND                         2
#define POWERSAVE0               3
#define POWERSAVE1               4
#define PROTECTIONFRAMEREADY  1
#define PROTECTIONFRAMECANCEL 2
#define PF_FRONT_0	0
#define PF_FRONT_1	1
#define PF_REAR_3	3
#define PF_REAR_4	4

#define COEX_WIFI_LINK_UP              1
#define COEX_WIFI_LINK_DOWN         2

struct COEX_LINKUP_INFO {
	UINT32 coexType;
	/* BW_10, BW_20, BW_40, BW_80 */
	UCHAR BBPCurrentBW;
	UCHAR Channel;
	/* Central Channel when using 40MHz is indicating. not real channel. */
	UCHAR CentralChannel;
	UCHAR *OwnMac;
	UCHAR *Bssid;
	struct _MAC_TABLE_ENTRY *pEntry;
	struct wifi_dev *wdev;
};

typedef struct PROTECTION_FRAME_STRUCT {
	BOOLEAN Occupied;
	UINT32 Triggernumber;
	UINT32 Valid;
	UINT32 NodeType;
	UINT32 BssHashID;
	UINT32 FrameType;
	BOOLEAN ToBeUpdate;
} PROTECTION_FRAME_STRUCT, *PPROTECTION_FRAME_STRUCT;

typedef struct _COEX_RX_MANUAL_AGC_GAIN {
	BOOLEAN bEnable;
	BOOLEAN bStopAGC;
	CHAR LMthreshold;
	CHAR MHthreshold;
} COEX_RX_MANUAL_AGC_GAIN, *PCOEX_RX_MANUAL_AGC_GAIN;

typedef struct _COEX_MODE_STRUCTURE {
	BOOLEAN dbgEnable;
	BOOLEAN ProtectionUpdated;
	BOOLEAN bForce;
	BOOLEAN FDDRequest;
	BOOLEAN TDDRequest;
	BOOLEAN ForceBA;
	UINT32 LinkStatus;
	UINT32 TDD_Power;
	UINT32 FDD_Power;
	UINT32 DefaultMode;
	UINT32 CurrentMode;
	UINT32 UpdateMode;
	CHAR CoexTDDRSSITreshold;
	CHAR CoexFDDRSSITreshold;
	UCHAR RxBAWinLimit;	/* 0 = not set, use original value */
	UCHAR TxBAWinLimit;	/* 0 = not set, use origianl value */
	UINT32 RF0NarrAnaFiltBW_OrigVal;	/* Original value of RF 0x24c (Narrowest Analog Filter BW) */
	UINT32 RF1NarrAnaFiltBW_OrigVal;	/* Original value of RF 0x24c (Narrowest Analog Filter BW) */
	UINT32 FDD_CRUpdated;	/* flag indicating whether CR has been udpated for FDD mode */
	UINT8 PDThreashold_OrigVal;	/* Original value of PD threshold (0x238c[31:24]) */
	NDIS_SPIN_LOCK UpdateLock;	/* for access NullFrameSpace structure */
} COEX_MODE_STRUCTURE, *PCOEX_MODE_STRUCTURE;

typedef struct _COEX_PROTECTION_FRAME_INFO {
	UINT32 CoexOperation;
	UINT32 Triggernumber;
	UINT32 Valid;
	UINT32 NodeType;
	UINT32 BssHashID;
	UINT32 FrameType;
} COEX_PROTECTION_FRAME_INFO, *PCOEX_PROTECTION_FRAME_INFO;

typedef struct _COEX_AFH {
	UINT32 CoexOperation;
	UINT32 LinkStatus;
	UINT32 BW;
	UINT32 Channel;
	UINT32 BssHashID;
} COEX_AFH, *PCOEX_AFH;

typedef struct _COEX_TF_SWITCH {
	UINT32 CoexOperation;
	UINT32 CoexMode;

} COEX_TF_SWITCH, *PCOEX_TF_SWITCH;

typedef struct _COEX_WLAN_STATUS {
	UINT32 CoexOperation;
	UINT32 WLANStatus;
	UINT32 PrivilegeTime;
	UINT32 BssHashID;

} COEX_WLAN_STATUS, *PCOEX_WLAN_STATUS;

#define PKT_CMD_TYPE_COEX_OP			(17)

#define BTCOEX_BB_BITWISE_WRITE(_BbReg, _BitLocation, _BitValue) \
	(_BbReg = (((_BbReg) & ~(_BitLocation)) | (_BitValue)))

#define BTCOEX_BB_BITMASK_READ(_BbReg, _BitLocation) \
	((_BbReg) & (_BitLocation))

#define MAC_BITWISE_WRITE(_MacReg, _BitLocation, _BitValue) \
	(_MacReg = (((_MacReg) & ~(_BitLocation)) | (_BitValue)))

#define MAC_BITMASK_READ(_MacReg, _BitLocation) \
	((_MacReg) & (_BitLocation))

#define BT_STATUS_SET_FLAG(_pAd, _F)       ((_pAd)->BTStatusFlags |= (_F))
#define BT_STATUS_CLEAR_FLAG(_pAd, _F)     ((_pAd)->BTStatusFlags &= ~(_F))
#define BT_STATUS_TEST_FLAG(_pAd, _F)      (((_pAd)->BTStatusFlags & (_F)) != 0)

typedef union _BT_STATUS_REPORT_STRUC {
	struct {
		UINT32 BT_DISABLE:1;
		UINT32 BT_INACTIVE:1;
		UINT32 BT_SCANONLY:1;
		UINT32 BT_HOLD:1;
		UINT32 BT_ACTIVE:1;
		UINT32 RESERVE1:3;
		UINT32 BT_TRANSPORT:5;
		UINT32 RESERVE2:3;
		UINT32 BT_LINK_CNT:3;
		UINT32 BLE_LINK_CNT:2;
		UINT32 RESERVE3:3;
		UINT32 BT_SLAVE_IND:7;
		UINT32 RESERVE4:1;
	} field;
	UINT32 word;
} BT_STATUS_REPORT_STRUC, *PBT_STATUS_REPORT_STRUC;

VOID BtAFHCtl(IN struct _RTMP_ADAPTER *pAd,
	      IN UCHAR BBPCurrentBW, IN UCHAR Channel, IN UCHAR CentralChannel, IN BOOLEAN Disable);

VOID TDDFDDExclusiveRequest(IN struct _RTMP_ADAPTER *pAd, UCHAR CoexMode);

VOID EstablishFrameBundle(IN struct _RTMP_ADAPTER *pAd,
			  IN PUCHAR pAddr, IN PUCHAR bssid,
			  IN UINT32 OPMode, IN struct _MAC_TABLE_ENTRY *pEntry);

VOID UpdateAndesNullFrameSpace(IN struct _RTMP_ADAPTER *pAd);

VOID InvalidProtectionFrameSpace(IN struct _RTMP_ADAPTER *pAd, IN PUCHAR pAddr);

VOID RemoveProtectionFrameSpace(IN struct _RTMP_ADAPTER *pAd, IN PUCHAR pAddr);

VOID TDDFDDCoexBACapability(IN struct _RTMP_ADAPTER *pAd, UCHAR CoexMode);

INT SendAndesTFSWITCH(IN struct _RTMP_ADAPTER *pAd, IN UCHAR CoexMode);

VOID CoexFDDRXAGCGain(IN struct _RTMP_ADAPTER *pAd, IN CHAR rssi);

VOID CoexTDDRXAGCGain(IN struct _RTMP_ADAPTER *pAd);

INT SendAndesWLANStatus(IN struct _RTMP_ADAPTER *pAd,
			IN UCHAR WlanStatus, IN UINT32 PrivilegeTime, IN UINT32 BssHashID);

VOID InitBTCoexistence(IN struct _RTMP_ADAPTER *pAd);

INT SendAndesAFH(IN struct _RTMP_ADAPTER *pAd,
		 IN UCHAR BBPCurrentBW,
		 IN UCHAR Channel, IN UCHAR CentralChannel,
		 IN BOOLEAN Disable, IN UINT32 BssHashID);

VOID MLMEHook(IN struct _RTMP_ADAPTER *pAd, IN UCHAR WlanStatus, IN UINT32 BssHashID);

VOID CoexistencePeriodicRoutine(IN struct _RTMP_ADAPTER *pAd);

VOID COEXLinkDown(IN struct _RTMP_ADAPTER *pAd, PUCHAR pAddr);

UINT32 QueryHashID(IN struct _RTMP_ADAPTER *pAd, PUCHAR pAddr, BOOLEAN RemoveBSS);

VOID CoexParseBTStatus(IN PRTMP_ADAPTER pAd);

VOID CoexFDDCRFineTune(IN PRTMP_ADAPTER pAd);

VOID CoexLinkUpHandler(RTMP_ADAPTER *pAd, struct COEX_LINKUP_INFO info);
INT SetCOEXBa(RTMP_ADAPTER *ad, PSTRING arg);
INT Set_ChangeCOEXMode_Proc(RTMP_ADAPTER *ad, PSTRING arg);
INT SetRssiPolicyProc(RTMP_ADAPTER *ad, PSTRING arg);
INT SetPerPolicyProc(RTMP_ADAPTER *ad, PSTRING arg);
#ifdef COEX_FDD_AUTO_SWITCH_TO_BW20
VOID CoexFDD_HtBwSwitchCheck(RTMP_ADAPTER *pAd);
#endif /* COEX_FDD_AUTO_SWITCH_TO_BW20 */
#ifdef DEBUGFS_SUPPORT
INT CoexSetPolicy(RTMP_ADAPTER *pAd, UCHAR *arg);
INT CoexNotifyEvent(RTMP_ADAPTER *pAd, UCHAR *arg, size_t size);
#endif /* DEBUGFS_SUPPORT */
INT SetCoexDbgEnable(RTMP_ADAPTER *ad, PSTRING arg);
#endif /* endif */
