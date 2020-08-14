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


#ifdef MT76XX_BTCOEX_SUPPORT
#include	"rt_config.h"
#ifdef DEBUGFS_SUPPORT
static struct {
	PSTRING op;

	INT(*handler) (PRTMP_ADAPTER pAdapter, PSTRING arg);
} *coex_dbgfs_op, COEX_DEBUGFS_OP[] = {
	{
	"mode", Set_ChangeCOEXMode_Proc},
	{
	"rssi", SetRssiPolicyProc},
	{
	"per", SetPerPolicyProc},
	{
	"ba", SetCOEXBa},
};

INT CoexSetPolicy(RTMP_ADAPTER *pAd, UCHAR *arg)
{
	INT ret = 0;
	PSTRING op;
	PSTRING str_param;
	INT str_len = strlen(arg);

	arg[str_len-1] = '\0';
	DBGPRINT(RT_DEBUG_OFF, ("%s, str:%s\n", __func__, arg));
	op = rstrtok(arg, ":");
	str_param = rstrtok(NULL, ":");

	if (!op || !str_param)
		return -1;

	for (coex_dbgfs_op = COEX_DEBUGFS_OP;
	     coex_dbgfs_op->op; coex_dbgfs_op++) {
		if (!strcmp(op, coex_dbgfs_op->op)) {
			if (!coex_dbgfs_op->handler(pAd, str_param))
				ret = -EINVAL;
			break;
		}
	}
	return ret;
}

INT CoexNotifyEvent(RTMP_ADAPTER *pAd, UCHAR *arg, size_t size)
{
	INT ret = 0;
	char *p = arg;

	p += sprintf(p, "DRI VER-%s\n", STA_DRIVER_VERSION);
	ret = ((size_t)p - (size_t)arg);

	return ret;
}
#endif /* DEBUGFS_SUPPORT */
INT SetCoexDbgEnable(RTMP_ADAPTER *pAd, PSTRING arg)
{
	INT ret = 0;
	PSTRING op;
	PSTRING str_param;
	UINT32 coexCfg3 = 0;

	op = rstrtok(arg, ":");
	str_param = rstrtok(NULL, ":");

	DBGPRINT(RT_DEBUG_OFF, ("%s, str:%s\n", __func__, arg));
	if (!op)
		return -EINVAL;
	if (!strcmp(op, "enable") && str_param) {
		ULONG mode = 0;
		UINT32 mode_mask = 0;

		ret = kstrtol(str_param, 10, &mode);
		/* 0x4c[10:9] */
		if (mode > 2)
			return -EINVAL;
		pAd->CoexMode.dbgEnable = TRUE;
		mode_mask = mode << 9;
		RTMP_IO_READ32(pAd, COEXCFG3, &coexCfg3);
		coexCfg3 = ((coexCfg3 | COEXCFG3_PTA_CNT_EN) | mode_mask);
		RTMP_IO_WRITE32(pAd, COEXCFG3, coexCfg3);
		DBGPRINT(RT_DEBUG_OFF,
			("%s, enable, param:%s, 0x123c:%08x, mode%lx, mode_mask:%08x\n"
			, __func__, str_param, coexCfg3, mode, mode_mask));
	} else if (!strcmp(op, "disable")) {
		RTMP_IO_READ32(pAd, COEXCFG3, &coexCfg3);
		coexCfg3 &= ~COEXCFG3_PTA_CNT_EN;
		RTMP_IO_WRITE32(pAd, COEXCFG3, coexCfg3);
		pAd->CoexMode.dbgEnable = FALSE;
		DBGPRINT(RT_DEBUG_OFF, ("%s, disable, 0x123c:%08x\n", __func__, coexCfg3));
	} else if (!strcmp(op, "reset")) {
		RTMP_IO_READ32(pAd, COEXCFG3, &coexCfg3);
		coexCfg3 |= COEXCFG3_PTA_CNT_CLEAR;
		RTMP_IO_WRITE32(pAd, COEXCFG3, coexCfg3);
		coexCfg3 &= ~COEXCFG3_PTA_CNT_CLEAR;
		RTMP_IO_WRITE32(pAd, COEXCFG3, coexCfg3);

		RTMP_IO_READ32(pAd, BT_COEX_CFG_9, &coexCfg3);
		coexCfg3 |= BT_COEX_CFG_9_UPDATE_CNT;
		RTMP_IO_WRITE32(pAd, BT_COEX_CFG_9, coexCfg3);
		coexCfg3 &= ~BT_COEX_CFG_9_UPDATE_CNT;
		RTMP_IO_WRITE32(pAd, BT_COEX_CFG_9, coexCfg3);

		DBGPRINT(RT_DEBUG_OFF, ("%s, reset\n", __func__));
	} else {
		DBGPRINT(RT_DEBUG_OFF, ("%s, unkown operation\n", __func__));
		return -EINVAL;
	}
	return ret;
}

INT SetRssiPolicyProc(RTMP_ADAPTER *ad, PSTRING arg)
{
	INT ret = 0;
	LONG param = 0;

	ret = kstrtol(arg, 10, &param);
	DBGPRINT(RT_DEBUG_OFF, ("%s, param:%ld\n", __func__, param));
	return ret;
}

INT SetPerPolicyProc(RTMP_ADAPTER *ad, PSTRING arg)
{
	INT ret = 0;
	LONG param = 0;

	ret = kstrtol(arg, 10, &param);

	DBGPRINT(RT_DEBUG_OFF, ("%s, param:%ld\n", __func__, param));
	return ret;
}

INT SetCOEXBa(RTMP_ADAPTER *pAd, PSTRING arg)
{
	LONG ba_size = COEX_BARXSIZE_A2DP;
	MAC_TABLE_ENTRY *pEntry;
	INT ret = 0;

	pEntry = &pAd->MacTab.Content[BSSID_WCID];

	ret = kstrtol(arg, 10, &ba_size);
	pAd->CoexMode.ForceBA = TRUE;
	pAd->CommonCfg.BACapability.field.RxBAWinLimit = ba_size;
	pAd->CoexMode.RxBAWinLimit = ba_size;
	if (!ba_size)
		pAd->CommonCfg.bBADecline = TRUE;
	else {
		pAd->CommonCfg.bBADecline = FALSE;
		if (ba_size == 255) {
			pAd->CoexMode.ForceBA = FALSE;
			ba_size = 64;
			DBGPRINT(RT_DEBUG_WARN,
				("COEX:%s: Not Force BA\n",
				__func__));
		}
	}

	BAOriSessionTearDown(pAd, BSSID_WCID, 0, FALSE, FALSE);
	BARecSessionTearDown(pAd, BSSID_WCID, 0, FALSE);
	/* expect to build BA */
	DBGPRINT(RT_DEBUG_OFF,
		 ("COEX:%s:RxBASize %d  BADecline:%x, Force:%x\n",
		  __func__, pAd->CoexMode.RxBAWinLimit,
		  pAd->CommonCfg.bBADecline, pAd->CoexMode.ForceBA));
	BAOriSessionSetUp(pAd, pEntry, 0, 0, 100, TRUE);
	return ret;
}

static INT SendEepromUpdate(RTMP_ADAPTER *ad, UINT32 eeprom)
{
	return andes_fun_set(ad, UPDATE_EEPROM, eeprom);
}

INT Set_ChangeCOEXMode_Proc(RTMP_ADAPTER *ad, PSTRING arg)
{
	INT ret = 0;
	UINT32 chmod = -1;
	UINT32 ant_setting = -1;

	if (!strcmp(arg, "FDD")) {
		chmod = COEX_MODE_FDD;
		ant_setting = COEX_FDD_ANT_SET;
	} else if (!strcmp(arg, "TDD")) {
		chmod = COEX_MODE_TDD;
		ant_setting = COEX_TDD_ANT_SET;
	} else if (!strcmp(arg, "HYBRID")) {
		chmod = COEX_MODE_HYBRID;
		ant_setting = COEX_FDD_ANT_SET;
	} else {
		DBGPRINT(RT_DEBUG_TRACE, ("%s, %s not support\n", __func__, arg));
		goto skip_chmod;
	}

	if (ad->CoexMode.CurrentMode == chmod)
		goto skip_chmod;

	ret = SendAndesTFSWITCH(ad, chmod);

	DBGPRINT(RT_DEBUG_WARN, ("===> %s to mode %x\n", __func__, chmod));
skip_chmod:
	return ret;
}

static INT SendAndesCoexFrameInfo(IN PRTMP_ADAPTER pAd, IN UINT32 TriggerNumber)
{
	INT ret;
	struct cmd_msg *msg;
	/* struct CMD_UNIT CmdUnit; */
	COEX_PROTECTION_FRAME_INFO coexProtectionFrameInfo = { 0 };
	USHORT coexProtectionFrameInfoLength = 0;

	DBGPRINT(RT_DEBUG_ERROR, ("%s: -->\n", __func__));
	coexProtectionFrameInfo.CoexOperation = TypeProtectionFrame;
	coexProtectionFrameInfo.Triggernumber = pAd->NullFrameSpace[TriggerNumber].Triggernumber;
	coexProtectionFrameInfo.Valid = pAd->NullFrameSpace[TriggerNumber].Valid;
	coexProtectionFrameInfo.NodeType = pAd->NullFrameSpace[TriggerNumber].NodeType;
	coexProtectionFrameInfo.BssHashID = pAd->NullFrameSpace[TriggerNumber].BssHashID;
	coexProtectionFrameInfo.FrameType = pAd->NullFrameSpace[TriggerNumber].FrameType;

	coexProtectionFrameInfoLength = sizeof(coexProtectionFrameInfo);

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s: Triggernumber = %u, Valid = %u, NodeType = %u, BssHashID = %u\n", __func__,
		  coexProtectionFrameInfo.Triggernumber, coexProtectionFrameInfo.Valid,
		  coexProtectionFrameInfo.NodeType, coexProtectionFrameInfo.BssHashID));

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s: FrameType = %u, CmdParametersLength = %d\n", __func__,
		  coexProtectionFrameInfo.FrameType, coexProtectionFrameInfoLength));

	msg = andes_alloc_cmd_msg(pAd, coexProtectionFrameInfoLength);
	if (!msg) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: can not allocate net_pkt or cmd\n", __func__));
		return -1;

	}

	andes_init_cmd_msg(msg, PKT_CMD_TYPE_COEX_OP, FALSE, 0, TRUE, FALSE, 0, NULL, NULL);
	andes_append_cmd_msg(msg, (char *)&coexProtectionFrameInfo, coexProtectionFrameInfoLength);
	ret = andes_send_cmd_msg(pAd, msg);
	return ret;
}

VOID UpdateAndesNullFrameSpace(IN PRTMP_ADAPTER pAd)
{
	INT ret = 0;
	UINT32 iter = 0;
	/* only TDD mode requires the protection frame */
	if (pAd->CoexMode.CurrentMode == COEX_MODE_FDD)
		return;
	RTMP_SPIN_LOCK(&pAd->CoexMode.UpdateLock);
	for (iter = 0; iter < NULLFRAMESPACE; iter++) {
		if ((pAd->NullFrameSpace[iter].Occupied != 0)
			&& (pAd->NullFrameSpace[iter].ToBeUpdate)) {
			DBGPRINT(RT_DEBUG_OFF,
					("%s Coex: Send protection frame %u\n"
					, __func__, iter));
			pAd->NullFrameSpace[iter].ToBeUpdate = FALSE;
			ret = SendAndesCoexFrameInfo(pAd, iter);
		}
	}
	pAd->CoexMode.ProtectionUpdated = FALSE;
	RTMP_SPIN_UNLOCK(&pAd->CoexMode.UpdateLock);
}

INT SendAndesAFH(IN PRTMP_ADAPTER pAd,
		 IN UCHAR BBPCurrentBW,
		 IN UCHAR Channel, IN UCHAR CentralChannel, IN BOOLEAN Disable, IN UINT32 BssHashID)
{

	COEX_AFH coexAFH = { 0 };
	USHORT coexAFHLength = 0;
	INT ret = 0;
	/* struct CMD_UNIT CmdUnit; */
	struct cmd_msg *msg;

	DBGPRINT(RT_DEBUG_TRACE, ("%s: -->\n", __func__));
	coexAFH.CoexOperation = TypeAFH;
	if (!IS_MT76XXBTCOMBO(pAd))
		return ret;

	if (BBPCurrentBW == BW_40) {
		coexAFH.BW = BW_40 + COEXNOZEROSHIFT;
		coexAFH.Channel = CentralChannel;
	} else if (BBPCurrentBW == BW_20) {
		coexAFH.BW = BW_20 + COEXNOZEROSHIFT;
		coexAFH.Channel = Channel;
	} else if (BBPCurrentBW == BW_80) {
		coexAFH.BW = BW_80 + COEXNOZEROSHIFT;
		coexAFH.Channel = Channel;
	}

	if (Disable == FALSE)
		coexAFH.LinkStatus = COEX_WIFI_LINK_UP;
	else
		coexAFH.LinkStatus = COEX_WIFI_LINK_DOWN;

	coexAFH.BssHashID = BssHashID;
	coexAFHLength = sizeof(coexAFH);

	msg = andes_alloc_cmd_msg(pAd, coexAFHLength);
	if (!msg) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: can not allocate net_pkt or cmd\n", __func__));
		return -1;

	}
	andes_init_cmd_msg(msg, PKT_CMD_TYPE_COEX_OP, FALSE, 0, TRUE, FALSE, 0, NULL, NULL);
	andes_append_cmd_msg(msg, (char *)&coexAFH, coexAFHLength);
	ret = andes_send_cmd_msg(pAd, msg);

	DBGPRINT(RT_DEBUG_TRACE, ("%s: LinkStatus = %u, BW = %u, Channel = %u\n",
				  __func__,
				  coexAFH.LinkStatus,
				  coexAFH.BW, coexAFH.Channel));

	DBGPRINT(RT_DEBUG_TRACE, ("%s: BssHashID = %u PktLength = %d\n",
				  __func__,
				  coexAFH.BssHashID, coexAFHLength));
	DBGPRINT(RT_DEBUG_TRACE, ("%s: <--\n", __func__));
	return ret;

}

VOID BtAFHCtl(IN PRTMP_ADAPTER pAd,
	      IN UCHAR BBPCurrentBW, IN UCHAR Channel, IN UCHAR CentralChannel, IN BOOLEAN Disable)
{
	UCHAR Kstart = 0, Kend = 0;
	BT_FUN_INFO_STRUC btFunInfo;

	if (!(IS_MT76XXBTCOMBO(pAd) || Channel > 14))
		return;

	memset(&btFunInfo, 0, sizeof(BT_FUN_INFO_STRUC));
	if (BBPCurrentBW == BW_40) {
		if (CentralChannel <= 4) {
			Kstart = 0;
			Kend = 53;
		} else if (CentralChannel >= 10) {
			Kstart = 25;
			Kend = 78;
		} else {
			Kstart = 2 + (CentralChannel - 5) * 5;
			Kend = Kstart + 55;
		}
	} else {
		if (Channel <= 3) {
			Kstart = 0;
			Kend = 47;
		} else if (Channel >= 10) {
			Kstart = 31;
			Kend = 78;
		} else {
			Kstart = 1 + (Channel - 4) * 5;
			Kend = Kstart + 48;
		}
	}

	RTMP_IO_READ32(pAd, BT_FUN_INFO, &btFunInfo.word);
	btFunInfo.word &= ~(0x3FFFFFFF);	/* Clear Power and AFH but keep active bit */
	if (!Disable) {
		btFunInfo.field.AFH_START_CH = Kstart;
		btFunInfo.field.AFH_END_CH = Kend;
		/* These Code and Definition are gone
		   if (pAd->CommonCfg.BBPCurrentBW == BW_40)
		   //0x04 // -14
		   btFunInfo.field.BTPower0 = 0x0c;     //-8
		   else
		   btFunInfo.field.BTPower0 = 0x1c;     //-2
		 */
	}

	DBGPRINT(RT_DEBUG_TRACE, ("%s: COEX AFH Start Ch = %d, AFH End Ch = %d, Channel = %d, CentralChannel = %d\n",
				  __func__,
				  btFunInfo.field.AFH_START_CH,
				  btFunInfo.field.AFH_END_CH, Channel, CentralChannel));

	RTMP_IO_WRITE32(pAd, BT_FUN_INFO, btFunInfo.word);
	/* Hook andes AFH command */

	/* High BT Priority Mode */
	/* RTMP_IO_WRITE32(pAd, 0x5c, 0x8000); */
}

UINT32 QueryHashID(IN PRTMP_ADAPTER pAd, PUCHAR pAddr, BOOLEAN RemoveBSS)
{
	UINT32 HashIdx;
	UCHAR j = 0;

	for (j = 1; j < NULLFRAMESPACE; j++) {
		if (MAC_ADDR_EQUAL(&pAd->HASH_BSSID[j][0], pAddr)) {
			DBGPRINT(RT_DEBUG_TRACE, ("COEX: j = %d\n", j));
			DBGPRINT(RT_DEBUG_TRACE,
				 ("COEX: Find BSSID Address = %02X:%02X:%02X:%02X:%02X:%02X\n",
				  pAd->HASH_BSSID[j][0], pAd->HASH_BSSID[j][1],
				  pAd->HASH_BSSID[j][2], pAd->HASH_BSSID[j][3],
				  pAd->HASH_BSSID[j][4], pAd->HASH_BSSID[j][5]));
			HashIdx = j;
			if (RemoveBSS == TRUE) {
				pAd->HASH_BSSID[j][MAC_ADDR_LEN] = 0;
				DBGPRINT(RT_DEBUG_TRACE,
					 ("COEX: Remove BSSID Address  = %02X:%02X:%02X:%02X:%02X:%02X\n",
					  pAd->HASH_BSSID[j][0], pAd->HASH_BSSID[j][1],
					  pAd->HASH_BSSID[j][2], pAd->HASH_BSSID[j][3],
					  pAd->HASH_BSSID[j][4], pAd->HASH_BSSID[j][5]));

			}
			return HashIdx;
		}
		/* MAC_ADDR_EQUAL(&pAd->HASH_BSSID[j][0],) */
		DBGPRINT(RT_DEBUG_TRACE, ("COEX:Miss search  j = %d\n", j));
	}
	for (j = 1; j < NULLFRAMESPACE; j++) {
		if (pAd->HASH_BSSID[j][MAC_ADDR_LEN] == 0) {
			COPY_MAC_ADDR(&pAd->HASH_BSSID[j][0], pAddr);
			pAd->HASH_BSSID[j][MAC_ADDR_LEN] = 1;
			HashIdx = j;

			DBGPRINT(RT_DEBUG_TRACE, ("COEX: free space j = %d\n", j));
			DBGPRINT(RT_DEBUG_TRACE,
				 ("COEX: Insert MAC Address  = %02X:%02X:%02X:%02X:%02X:%02X\n",
				  pAd->HASH_BSSID[j][0], pAd->HASH_BSSID[j][1],
				  pAd->HASH_BSSID[j][2], pAd->HASH_BSSID[j][3],
				  pAd->HASH_BSSID[j][4], pAd->HASH_BSSID[j][5]));
			return HashIdx;
		}
		/* debug if there is an overflow issue */
		if (j == (NULLFRAMESPACE - 1)) {
			COPY_MAC_ADDR(&pAd->HASH_BSSID[j][0], pAddr);
			pAd->HASH_BSSID[1][MAC_ADDR_LEN] = 1;
			pAd->HASH_BSSID[2][MAC_ADDR_LEN] = 0;
			pAd->HASH_BSSID[3][MAC_ADDR_LEN] = 0;
			HashIdx = 1;

			return HashIdx;
		}
	}
	return 0;
}

VOID RemoveProtectionFrameSpace(IN PRTMP_ADAPTER pAd, IN PUCHAR pAddr)
{
	UINT32 BssHashID = QueryHashID(pAd, pAddr, FALSE);
	UCHAR iter = 0;

	RTMP_SPIN_LOCK(&pAd->CoexMode.UpdateLock);
	for (iter = 0; iter < NULLFRAMESPACE; iter++) {
		if (pAd->NullFrameSpace[iter].BssHashID == BssHashID)
			pAd->NullFrameSpace[iter].Occupied = FALSE;
	}
	pAd->CoexMode.ProtectionUpdated = TRUE;
	RTMP_SPIN_UNLOCK(&pAd->CoexMode.UpdateLock);
}

static VOID FillProtectionFrameSpace(IN PRTMP_ADAPTER pAd,
				     IN UINT32 Triggernumber,
				     IN UINT32 Valid,
				     IN UINT32 NodeType, IN PUCHAR pAddr, IN UINT32 FrameType)
{
	RTMP_SPIN_LOCK(&pAd->CoexMode.UpdateLock);
	pAd->NullFrameSpace[Triggernumber].Occupied = TRUE;
	pAd->NullFrameSpace[Triggernumber].Triggernumber = Triggernumber;
	pAd->NullFrameSpace[Triggernumber].Valid = Valid;
	pAd->NullFrameSpace[Triggernumber].NodeType = NodeType;
	pAd->NullFrameSpace[Triggernumber].BssHashID = QueryHashID(pAd, pAddr, FALSE);
	pAd->NullFrameSpace[Triggernumber].FrameType = FrameType;
	pAd->NullFrameSpace[Triggernumber].ToBeUpdate = TRUE;
	RTMP_SPIN_UNLOCK(&pAd->CoexMode.UpdateLock);
}

static VOID PrepareProtectionFrame(IN PRTMP_ADAPTER pAd,
				   IN UINT32 Type,
				   IN UINT32 Number,
				   IN UINT32 NAV,
				   IN PUCHAR pAddr, IN UINT32 OPMode, IN PMAC_TABLE_ENTRY pEntry)
{
	HEADER_802_11 ProtectionFrame;
	TXWI_STRUC ProtectionFrameTxWI;
	TXWI_STRUC *pTxWI;
	UCHAR *ptr = NULL;
	UINT i = 0;
	/* PHY_CFG PhyCfg = {0}; */
	UCHAR Wcid = 0;
	UCHAR Length = 0;
	UINT32 FrameAddress = 0;
	BOOLEAN Ack = FALSE;

	DBGPRINT(RT_DEBUG_TRACE, ("==>PrepareProtectionFrame\n"));
	NdisZeroMemory(&ProtectionFrameTxWI, sizeof(TXWI_STRUC));
	NdisZeroMemory(&ProtectionFrame, sizeof(HEADER_802_11));

	pAd->CoexMode.ProtectionUpdated = TRUE;
	switch (Type) {
	case CTSTOSELF:
		ProtectionFrame.FC.Type = FC_TYPE_CNTL;
		ProtectionFrame.FC.SubType = SUBTYPE_CTS;
		break;

	case CFEND:
		ProtectionFrame.FC.Type = FC_TYPE_CNTL;
		ProtectionFrame.FC.SubType = SUBTYPE_CFEND;
		break;

	case POWERSAVE1:
		ProtectionFrame.FC.Type = FC_TYPE_DATA;
		ProtectionFrame.FC.SubType = SUBTYPE_DATA_NULL;
		ProtectionFrame.FC.PwrMgmt = PWR_SAVE;
		ProtectionFrame.FC.ToDs = 1;
		ProtectionFrame.FC.FrDs = 0;
		break;

	case POWERSAVE0:
		ProtectionFrame.FC.Type = FC_TYPE_DATA;
		ProtectionFrame.FC.SubType = SUBTYPE_DATA_NULL;
		ProtectionFrame.FC.PwrMgmt = PWR_ACTIVE;
		ProtectionFrame.FC.ToDs = 1;
		ProtectionFrame.FC.FrDs = 0;
		break;

	default:
		DBGPRINT(RT_DEBUG_ERROR, ("%s: invalid coex mode %u\n", __func__,
			Type));
		return;
	}

	if (Type == CTSTOSELF) {
		COPY_MAC_ADDR(ProtectionFrame.Addr1, pAddr);
		ProtectionFrame.Duration = (USHORT) NAV;
		Wcid = 0xff;
		Length = 10;
	} else if (Type == CFEND) {
		COPY_MAC_ADDR(ProtectionFrame.Addr1, BROADCAST_ADDR);
		COPY_MAC_ADDR(ProtectionFrame.Addr2, pAddr);
		ProtectionFrame.Duration = 0;
		Wcid = 0xff;
		Length = 16;
	} else {
		pAd->Sequence = (pAd->Sequence + 1) & MAXSEQ;	/* ((pAd->Sequence) + 1) & (MAX_SEQ_NUMBER); */
		ProtectionFrame.Sequence = pAd->Sequence;
		ProtectionFrame.Duration = RTMPCalcDuration(pAd, RATE_1, 14);

		COPY_MAC_ADDR(ProtectionFrame.Addr1, pEntry->Addr);
		COPY_MAC_ADDR(ProtectionFrame.Addr2, pAddr);
		COPY_MAC_ADDR(ProtectionFrame.Addr3, pAd->CommonCfg.Bssid);
		Ack = TRUE;
		Wcid = BSSID_WCID;
		Length = sizeof(HEADER_802_11);
	}

	pTxWI = (TXWI_STRUC *) &ProtectionFrameTxWI;
	RTMPWriteTxWI(pAd,
		      pTxWI,
		      FALSE,
		      FALSE,
		      FALSE,
		      FALSE,
		      FALSE,
		      0,
		      0,
		      pEntry->Aid,
		      Length,
		      0,
		      0,
		      (UCHAR) pAd->CommonCfg.MlmeTransmit.field.MCS,
		      IFS_HTTXOP, &pAd->CommonCfg.MlmeTransmit);
	pTxWI->TXWI_N.PHYMODE = MODE_CCK;
	pTxWI->TXWI_N.MCS = 0;

	if (Number == 0) {
		FrameAddress = HW_NULL_BASE + 0x4000;
	} else if (Number == 1) {
		FrameAddress = HW_NULL2_BASE + 0x4000;
	} else {
		/* trigger number 0/1 is belong to null number address */
		/* Beacon address from D000 */
		FrameAddress = 0xD000 + (0x200 * (Number - 2));
	}

	DBGPRINT(RT_DEBUG_TRACE, ("Protection FrameAddress =0x%08x\n", (UINT32) FrameAddress));
	/*  */
	/* Move TXWI and frame content to on-chip memory */
	/*  */
	ptr = (PUCHAR) &ProtectionFrameTxWI;
	for (i = 0; i < pAd->chipCap.TXWISize; i += 4) {	/* 24-byte TXINFO field */
		RTMP_IO_WRITE32(pAd, FrameAddress + i, *((UINT32 *) ptr));
		ptr += 4;
	}

	ptr = (PUCHAR) &ProtectionFrame;
	DBGPRINT(RT_DEBUG_TRACE, ("Type =%u\n", Type));
	hex_dump("COEX PrepareProtectionFrame", ptr, Length);
	for (i = 0; i < Length; i += 4) {
		RTMP_IO_WRITE32(pAd, FrameAddress + pAd->chipCap.TXWISize + i, *((UINT32 *) ptr));
		ptr += 4;
	}
}

VOID InvalidProtectionFrameSpace(IN PRTMP_ADAPTER pAd, IN PUCHAR pAddr)
{
	UINT32 BssHashID = QueryHashID(pAd, pAddr, FALSE);
	UCHAR iter = 0;

	RTMP_SPIN_LOCK(&pAd->CoexMode.UpdateLock);
	for (iter = 0; iter < NULLFRAMESPACE; iter++) {
		if (pAd->NullFrameSpace[iter].BssHashID == BssHashID) {
			DBGPRINT(RT_DEBUG_WARN,
				("%s,trigger:%x, COEX MAC:%02x:%02x:%02x:%02x:%02x:%02x\n"
				, __func__, iter, PRINT_MAC(pAddr)));
			pAd->CoexMode.ProtectionUpdated = TRUE;
			pAd->NullFrameSpace[iter].Valid = FALSE;
			pAd->NullFrameSpace[iter].ToBeUpdate = TRUE;
		}
	}
	RTMP_SPIN_UNLOCK(&pAd->CoexMode.UpdateLock);
}

VOID EstablishFrameBundle(IN PRTMP_ADAPTER pAd,
			  IN PUCHAR pAddr, IN PUCHAR bssid,
			  UINT32 OPMode, IN PMAC_TABLE_ENTRY pEntry)
{
	UCHAR n0, n1, n2, n3 = 0;

	DBGPRINT(RT_DEBUG_TRACE, ("%s, OwnMac:%02x:%02x:%02x:%02x:%02x:%02x\n"
			, __func__, PRINT_MAC(pAddr)));
	n0 = PF_FRONT_0;
	if ((n0 != NULLFRAMESPACE) && !(pAd->CoexMode.LinkStatus&COEX_GO_LINK)) {
		DBGPRINT(RT_DEBUG_TRACE, ("COEX: CTS2Self Trigger#:%d\n", n0));
		PrepareProtectionFrame(pAd, CTSTOSELF, n0, 2500, pAddr, OPMode, pEntry);
		FillProtectionFrameSpace(pAd, n0, PROTECTIONFRAMEREADY, OPMode, bssid, CTSTOSELF);
	} else {
		DBGPRINT(RT_DEBUG_TRACE, ("COEX, %s: Op mode:%x, p2p GO On:%x\n"
				, __func__
				, OPMode
				, RTMP_CFG80211_VIF_P2P_GO_ON(pAd)));
	}

	n1 = PF_FRONT_1;
	/* Only use Null frame for STA */
	if ((n1 != NULLFRAMESPACE) && (OPMode == COEX_OPMODE_STA)) {
		DBGPRINT(RT_DEBUG_TRACE, ("COEX: PS1 Trigger#:%d\n", n1));
		PrepareProtectionFrame(pAd, POWERSAVE1, n1, 0, pAddr, OPMode, pEntry);
		FillProtectionFrameSpace(pAd, n1, PROTECTIONFRAMEREADY, OPMode, bssid, POWERSAVE1);
	}

	n2 = PF_REAR_3;
	if ((n2 != NULLFRAMESPACE) && !(pAd->CoexMode.LinkStatus&COEX_GO_LINK)) {
		DBGPRINT(RT_DEBUG_TRACE, ("COEX: CFE Trigger#:%d\n", n2));
		PrepareProtectionFrame(pAd, CFEND, n2, 0, pAddr, OPMode, pEntry);
		FillProtectionFrameSpace(pAd, n2, PROTECTIONFRAMEREADY, OPMode, bssid, CFEND);
	}

	n3 = PF_REAR_4;
	/* Only use Null frame for STA */
	if ((n3 != NULLFRAMESPACE) && (OPMode == COEX_OPMODE_STA)) {
		DBGPRINT(RT_DEBUG_TRACE, ("COEX: PS0 Trigger#:%d\n", n3));
		PrepareProtectionFrame(pAd, POWERSAVE0, n3, 0, pAddr, OPMode, pEntry);
		FillProtectionFrameSpace(pAd, n3, PROTECTIONFRAMEREADY, OPMode, bssid, POWERSAVE0);
	}
}

INT SendAndesWLANStatus(IN PRTMP_ADAPTER pAd,
			IN UCHAR WlanStatus, IN UINT32 PrivilegeTime, IN UINT32 BssHashID)
{

	COEX_WLAN_STATUS wlanStatus = { 0 };
	USHORT wlanStatusLength = 0;
	INT ret = 0;
	/* struct CMD_UNIT CmdUnit; */
	struct cmd_msg *msg;

	DBGPRINT(RT_DEBUG_INFO, ("%s: -->\n", __func__));
	wlanStatus.CoexOperation = TypeWiFiStatus;
	wlanStatus.WLANStatus = WlanStatus;
	wlanStatus.PrivilegeTime = PrivilegeTime;
	wlanStatus.BssHashID = BssHashID;
	wlanStatusLength = sizeof(wlanStatus);

	msg = andes_alloc_cmd_msg(pAd, wlanStatusLength);
	if (!msg) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: can not allocate net_pkt or cmd\n", __func__));
		return -1;

	}
	andes_init_cmd_msg(msg, PKT_CMD_TYPE_COEX_OP, FALSE, 0, FALSE, FALSE, 0, NULL, NULL);
	andes_append_cmd_msg(msg, (char *)&wlanStatus, wlanStatusLength);
	ret = andes_send_cmd_msg(pAd, msg);

	DBGPRINT(RT_DEBUG_INFO, ("%s: CoexOperation = %u, WlanStatus = %u, PrivilegeTime = %u\n",
				  __func__,
				  wlanStatus.CoexOperation,
				  wlanStatus.WLANStatus,
				  wlanStatus.PrivilegeTime));
	DBGPRINT(RT_DEBUG_INFO, ("%s: BssHashID = %u, PktLength = %d\n",
				  __func__,
				  wlanStatus.BssHashID, wlanStatusLength));

	DBGPRINT(RT_DEBUG_INFO, ("%s: <--\n", __func__));
	return ret;

}

VOID CoexFDDRXAGCGain(IN PRTMP_ADAPTER pAd, IN CHAR rssi)
{
/* the following block should not be used. */
}

VOID CoexTDDRXAGCGain(IN PRTMP_ADAPTER pAd)
{
	UINT32 bbp_val, bbp_reg = AGC1_R8;

	if (pAd->coexRXManualAGCGain.bStopAGC) {
		DBGPRINT(RT_DEBUG_TRACE, ("COEX: Stop AGC "));
	} else {
		DBGPRINT(RT_DEBUG_TRACE, ("COEX: %s --> Adjust AGC\n ", __func__));
		RTMP_BBP_IO_READ32(pAd, bbp_reg, &bbp_val);
		BTCOEX_BB_BITWISE_WRITE(bbp_val, (BIT6 + BIT7), (BIT6 + BIT7));
		RTMP_BBP_IO_WRITE32(pAd, bbp_reg, bbp_val);
	}
}

VOID TDDFDDCoexBACapability(IN PRTMP_ADAPTER pAd, UCHAR CoexMode)
{
	/* PRTMP_PORT    pPort = RTMPGetActivePort(pAd); */
	MAC_TABLE_ENTRY *pEntry;

	pEntry = &pAd->MacTab.Content[BSSID_WCID];

	if (!(IS_MT7650(pAd) || IS_MT7630(pAd) || IS_MT76x2(pAd)))
		return;

	if (!(INFRA_ON(pAd)))
		return;

	if (CoexMode == COEX_MODE_TDD) {
		if (pAd->CoexMode.ForceBA)
			return;
		if (BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_SYNC)) {
			/* Update BAWinLimit */
			if (pAd->CommonCfg.BACapability.field.RxBAWinLimit !=
			    COEX_BARXSIZE_SCO_ESCO) {
				pAd->CommonCfg.BACapability.field.RxBAWinLimit =
				    COEX_BARXSIZE_SCO_ESCO;
				pAd->CoexMode.RxBAWinLimit = COEX_BARXSIZE_SCO_ESCO;
				BAOriSessionTearDown(pAd, BSSID_WCID, 0, FALSE, FALSE);
				BARecSessionTearDown(pAd, BSSID_WCID, 0, FALSE);
				/* expect to build BA */
				/* pAd->CommonCfg.bBADecline = TRUE; */
				DBGPRINT(RT_DEBUG_OFF,
					 ("COEX:TDD  SCO/HID: Set RxBASize to %d\n",
					  pAd->CoexMode.RxBAWinLimit));
				BAOriSessionSetUp(pAd, pEntry, 0, 0, 100, TRUE);
			}
		} else if (BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_BWL)) {
			/* Update BAWinLimit */
			if (pAd->CommonCfg.BACapability.field.RxBAWinLimit != COEX_BARXSIZE_A2DP) {
				pAd->CommonCfg.BACapability.field.RxBAWinLimit = COEX_BARXSIZE_A2DP;
				pAd->CoexMode.RxBAWinLimit = COEX_BARXSIZE_A2DP;
				pAd->CommonCfg.bBADecline = FALSE;
				BAOriSessionTearDown(pAd, BSSID_WCID, 0, FALSE, FALSE);
				BARecSessionTearDown(pAd, BSSID_WCID, 0, FALSE);
				/* expect to build BA */
				DBGPRINT(RT_DEBUG_OFF,
					 ("COEX:TDD A2DP: Set RxBASize to %d\n",
					  pAd->CoexMode.RxBAWinLimit));
				BAOriSessionSetUp(pAd, pEntry, 0, 0, 100, TRUE);
			}
		} else {
			/* Update BAWinLimit */
			if (pAd->CommonCfg.BACapability.field.RxBAWinLimit != COEX_BARXSIZE_OPP) {
				pAd->CommonCfg.BACapability.field.RxBAWinLimit = COEX_BARXSIZE_OPP;
				pAd->CoexMode.RxBAWinLimit = COEX_BARXSIZE_OPP;
				BAOriSessionTearDown(pAd, BSSID_WCID, 0, FALSE, FALSE);
				BARecSessionTearDown(pAd, BSSID_WCID, 0, FALSE);
				pAd->CommonCfg.bBADecline = FALSE;
				/* expect to build BA */
				DBGPRINT(RT_DEBUG_OFF,
					 ("COEX:TDD OPP/ELSE: Set RxBASize to %d\n",
					  pAd->CoexMode.RxBAWinLimit));
				BAOriSessionSetUp(pAd, pEntry, 0, 0, 100, TRUE);
			}
		}
	} else if (CoexMode == COEX_MODE_HYBRID) {
		if (pAd->CoexMode.ForceBA)
			return;
		if (BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_BWL)) {
			/* Update BAWinLimit */
			if (pAd->CommonCfg.BACapability.field.RxBAWinLimit != COEX_BARXSIZE_A2DP) {
				pAd->CommonCfg.BACapability.field.RxBAWinLimit = COEX_BARXSIZE_A2DP;
				pAd->CoexMode.RxBAWinLimit = COEX_BARXSIZE_A2DP;
				pAd->CommonCfg.bBADecline = FALSE;
				BAOriSessionTearDown(pAd, BSSID_WCID, 0, FALSE, FALSE);
				BARecSessionTearDown(pAd, BSSID_WCID, 0, FALSE);
				/* expect to build BA */
				DBGPRINT(RT_DEBUG_OFF,
						("COEX:HYBRID A2DP: Set RxBASize to %d\n",
						pAd->CoexMode.RxBAWinLimit));
				BAOriSessionSetUp(pAd, pEntry, 0, 0, 100, TRUE);
			}
		} else {
			/* Update BAWinLimit */
			if (pAd->CommonCfg.BACapability.field.RxBAWinLimit != COEX_BARXSIZE_OPP) {
				pAd->CommonCfg.BACapability.field.RxBAWinLimit = COEX_BARXSIZE_OPP;
				pAd->CoexMode.RxBAWinLimit = COEX_BARXSIZE_OPP;
				BAOriSessionTearDown(pAd, BSSID_WCID, 0, FALSE, FALSE);
				BARecSessionTearDown(pAd, BSSID_WCID, 0, FALSE);
				pAd->CommonCfg.bBADecline = FALSE;
				/* expect to build BA */
				DBGPRINT(RT_DEBUG_OFF,
					 ("COEX:HYBRID OPP/ELSE: Set RxBASize to %d\n",
					  pAd->CoexMode.RxBAWinLimit));
				BAOriSessionSetUp(pAd, pEntry, 0, 0, 100, TRUE);
			}
		}
	} else if ((CoexMode == COEX_MODE_FDD) || (CoexMode == COEX_MODE_RESET)) {
		if (pAd->CoexMode.ForceBA)
			return;
		/* Update BAWinLimit */
		if (pAd->CommonCfg.BACapability.field.RxBAWinLimit !=
		    pAd->CommonCfg.REGBACapability.field.RxBAWinLimit) {
			pAd->CommonCfg.BACapability.field.RxBAWinLimit =
			    pAd->CommonCfg.REGBACapability.field.RxBAWinLimit;
			pAd->CommonCfg.BACapability.field.TxBAWinLimit =
			    pAd->CommonCfg.REGBACapability.field.TxBAWinLimit;
			pAd->CoexMode.RxBAWinLimit =
			    pAd->CommonCfg.REGBACapability.field.RxBAWinLimit;
			BAOriSessionTearDown(pAd, BSSID_WCID, 0, FALSE, FALSE);
			BARecSessionTearDown(pAd, BSSID_WCID, 0, FALSE);
			pAd->CommonCfg.bBADecline = FALSE;
			DBGPRINT(RT_DEBUG_OFF,
				 ("COEX: FDD (Reset) mode: Set RxBASize to %d\n",
				  pAd->CoexMode.RxBAWinLimit));
			/* expect to build BA */
			BAOriSessionSetUp(pAd, pEntry, 0, 0, 100, TRUE);
		}
	}
}

VOID TDDFDDExclusiveRequest(IN PRTMP_ADAPTER pAd, UCHAR CoexMode)
{
	if (CoexMode == COEX_MODE_FDD) {
		pAd->CoexMode.FDDRequest = TRUE;
		pAd->CoexMode.TDDRequest = FALSE;
	} else if (CoexMode == COEX_MODE_TDD) {
		pAd->CoexMode.FDDRequest = FALSE;
		pAd->CoexMode.TDDRequest = TRUE;
	} else if (CoexMode == COEX_MODE_RESET) {
		pAd->CoexMode.FDDRequest = FALSE;
		pAd->CoexMode.TDDRequest = FALSE;
	}

}

INT SendAndesTFSWITCH(IN PRTMP_ADAPTER pAd, IN UCHAR CoexMode)
{

	COEX_TF_SWITCH coexTF = { 0 };
	USHORT coexTFLength = 0;
	INT ret = 0;
	/* struct CMD_UNIT CmdUnit; */
	struct cmd_msg *msg;

	DBGPRINT(RT_DEBUG_ERROR, ("%s: CoexMode=%d-->\n", __func__, CoexMode));
	coexTF.CoexOperation = TypeCoexCCUForceMode;
	coexTF.CoexMode = CoexMode;

	coexTFLength = sizeof(coexTF);

	msg = andes_alloc_cmd_msg(pAd, coexTFLength);
	if (!msg) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: can not allocate net_pkt or cmd\n", __func__));
		return -1;

	}
	andes_init_cmd_msg(msg, PKT_CMD_TYPE_COEX_OP, FALSE, 0, TRUE, FALSE, 0, NULL, NULL);
	andes_append_cmd_msg(msg, (char *)&coexTF, coexTFLength);
	ret = andes_send_cmd_msg(pAd, msg);

	DBGPRINT(RT_DEBUG_ERROR, ("%s: CoexOperation = %u, CoexMode = %u\n, PktLength = %d\n",
				  __func__, coexTF.CoexOperation, coexTF.CoexMode, coexTFLength));
	TDDFDDExclusiveRequest(pAd, CoexMode);
	DBGPRINT(RT_DEBUG_ERROR, ("%s: <--\n", __func__));

	return ret;
}

VOID InitBTCoexistence(IN PRTMP_ADAPTER pAd)
{
	RTMP_CHIP_OP *pChipOps;
	UINT16 eeprom = 0;

	if (pAd == NULL)
		return;

	pChipOps = &pAd->chipOps;
	pChipOps->eeread(pAd, EXTERN_COMPONENT_SETTING_3, &eeprom);
	eeprom &= 0x00ff;

	NdisAllocateSpinLock(pAd, &pAd->CoexMode.UpdateLock);
	if (IS_MT7650(pAd) || IS_MT7630(pAd) || IS_MT76x2(pAd)) {
		pAd->CommonCfg.CoexFDDRSSITreshold = 70;
		pAd->CommonCfg.CoexTDDRSSITreshold = 76;
		pAd->CommonCfg.CoexRXAGCLMTreshold = 51;
		pAd->CommonCfg.CoexRXAGCMHTreshold = 66;
		pAd->CommonCfg.CoexWLANPrivilegeTime = 0x50000044;
		/* Coex paramter */
		if (IS_MT7662(pAd)) {
			pAd->CommonCfg.CoexDefaultMode = COEX_MODE_FDD;
			if (eeprom == COEX_TDD_ANT_SET)
				pAd->CommonCfg.CoexDefaultMode = COEX_MODE_TDD;
		} else
			pAd->CommonCfg.CoexDefaultMode = COEX_MODE_TDD;
		if (pAd->CommonCfg.CoexDefaultMode & 0xF0000000)
			pAd->CoexMode.DefaultMode = (pAd->CommonCfg.CoexDefaultMode & 0x0FFFFFFF);
		else
			pAd->CoexMode.DefaultMode = pAd->CommonCfg.CoexDefaultMode;

		DBGPRINT(RT_DEBUG_ERROR, ("==>InitBTCoexistence CoexMode.DefaultMode=%s(%u)\n",
					  pAd->CoexMode.DefaultMode ==
					  COEX_MODE_TDD ? "TDD" : (pAd->CoexMode.DefaultMode ==
								   COEX_MODE_FDD ? "FDD" : "Other"),
					  pAd->CoexMode.DefaultMode));
		pAd->CoexMode.CurrentMode = pAd->CoexMode.DefaultMode;
		pAd->CoexMode.CoexFDDRSSITreshold = -pAd->CommonCfg.CoexFDDRSSITreshold;
		pAd->CoexMode.CoexTDDRSSITreshold = -pAd->CommonCfg.CoexTDDRSSITreshold;
		pAd->coexRXManualAGCGain.LMthreshold = -pAd->CommonCfg.CoexRXAGCLMTreshold;
		pAd->coexRXManualAGCGain.MHthreshold = -pAd->CommonCfg.CoexRXAGCMHTreshold;
		if (pAd->E2pAccessMode != E2P_EFUSE_MODE && IS_MT7662(pAd)) {
			/* BIT[7:0]:EEPROM[23], BIT[15:8]:EEPROM[24], BIT[23:16]:EEPROM[25] */
			UINT32 eeprom = 0;

			eeprom = (eeprom | pAd->EEPROMImage[EXTERN_COMPONENT_SETTING_3]) << 8;
			eeprom = (eeprom | pAd->EEPROMImage[EXTERN_COMPONENT_SETTING_2]) << 8;
			eeprom = eeprom | pAd->EEPROMImage[EXTERN_COMPONENT_SETTING_1];
			DBGPRINT(RT_DEBUG_TRACE, ("%s, update eeprom:%08x\n", __func__, eeprom));
			SendEepromUpdate(pAd, eeprom);
		}
	}
}

VOID MLMEHook(IN PRTMP_ADAPTER pAd, IN UCHAR WlanStatus, IN UINT32 BssHashID)
{
	/* 5ms*100 = 500 */
	UINT32 unit = ((pAd->CommonCfg.CoexWLANPrivilegeTime & 0xF0000000) >> 28) * 100;
	UINT32 times = 0;
	INT ret = 0;

	if (!(IS_MT7650(pAd) || IS_MT7630(pAd) || IS_MT7662(pAd) || IS_MT7632(pAd)))
		return;

	DBGPRINT(RT_DEBUG_INFO,
		 ("COEX: CoexWLANPrivilegeTime = %lu\n", pAd->CommonCfg.CoexWLANPrivilegeTime));
	DBGPRINT(RT_DEBUG_INFO, ("COEX: unit = %u, WlanStatus:%x\n", unit, WlanStatus));

	switch (WlanStatus) {
	case WLAN_Device_ON:
		ret = SendAndesWLANStatus(pAd, WlanStatus, 0, 0);
		break;

	case WLAN_CONNECTION_START:
		times = (pAd->CommonCfg.CoexWLANPrivilegeTime & 0xF0) >> 4;
		ret = SendAndesWLANStatus(pAd, WlanStatus, unit * times, BssHashID);
		break;
	case WLAN_Device_OFF:
		break;

	default:
		ret = SendAndesWLANStatus(pAd, WlanStatus, 0, BssHashID);
	}

}

/* COEX periodic checking */
VOID CoexistencePeriodicRoutine(IN PRTMP_ADAPTER pAd)
{
	WLAN_FUN_INFO_STRUC wlanFunInfo = {.word = 0 };

	RTMP_IO_READ32(pAd, WLAN_FUN_INFO, &wlanFunInfo.word);
	/* Process Block Ack for aggregation */
	if (IS_MT76XXBTCOMBO(pAd) && (BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_ACTIVE))) {
		if (pAd->CoexMode.CurrentMode == COEX_MODE_FDD) {
			TDDFDDCoexBACapability(pAd, COEX_MODE_FDD);
		} else if (pAd->CoexMode.CurrentMode == COEX_MODE_TDD) {
			TDDFDDCoexBACapability(pAd, COEX_MODE_TDD);
		} else if (pAd->CoexMode.CurrentMode == COEX_MODE_HYBRID) {
			TDDFDDCoexBACapability(pAd, COEX_MODE_HYBRID);
		} else {
			DBGPRINT(RT_DEBUG_ERROR,
				 ("!!COEX:Strange CoexMode = %u\n", pAd->CoexMode.CurrentMode));
		}
	} else if ((IS_MT7650(pAd) || IS_MT7630(pAd) || IS_MT76x2(pAd))) {
		TDDFDDCoexBACapability(pAd, COEX_MODE_RESET);
	}
#ifdef COEX_FDD_AUTO_SWITCH_TO_BW20
	if (IS_MT76XXBTCOMBO(pAd))
		CoexFDD_HtBwSwitchCheck(pAd);
#endif /* COEX_FDD_AUTO_SWITCH_TO_BW20 */

#ifdef COEX_DYNAMIC_MODE_SWITCH	/* disabled by default */
	/* Process TDD/FDD CR */
	if ((IS_MT7650(pAd) || IS_MT7630(pAd) || IS_MT76x2(pAd)) && INFRA_ON(pAd)) {
		DBGPRINT(RT_DEBUG_INFO,
			 ("COEX: pAd->CoexMode.CoexTDDRSSITreshold = %d\n",
			  pAd->CoexMode.CoexTDDRSSITreshold));
		DBGPRINT(RT_DEBUG_INFO,
			 ("COEX: pAd->CoexMode.CoexFDDRSSITreshold = %d\n",
			  pAd->CoexMode.CoexFDDRSSITreshold));
		/* MT76xx coex RX LNA gain adjustment */
		if (pAd->CoexMode.CurrentMode == COEX_MODE_FDD) {
			if (pAd->StaCfg.RssiSample.AvgRssi0 < (pAd->CoexMode.CoexTDDRSSITreshold)) {
				if (pAd->CoexMode.TDDRequest == FALSE)
					;/* SendAndesTFSWITCH(pAd, COEX_MODE_TDD); */

			}
			if (pAd->StaCfg.RssiSample.AvgRssi0 > (pAd->CoexMode.CoexFDDRSSITreshold)) {
				if (pAd->CoexMode.FDDRequest == FALSE)
					;/* SendAndesTFSWITCH(pAd, COEX_MODE_FDD); */

			}
			/* CoexFDDRXAGCGain(pAd, pAd->StaCfg.RssiSample.AvgRssi0); */
		} else if (pAd->CoexMode.CurrentMode == COEX_MODE_TDD) {
			if (pAd->StaCfg.RssiSample.AvgRssi0 > (pAd->CoexMode.CoexFDDRSSITreshold)) {
				if (pAd->CoexMode.FDDRequest == FALSE) {
					/* 20130523-sarick
					   Do not set to FDD now. */
					/* SendAndesTFSWITCH(pAd, COEX_MODE_FDD); */
				}
			}
			if (pAd->StaCfg.RssiSample.AvgRssi0 < (pAd->CoexMode.CoexTDDRSSITreshold)) {
				if (pAd->CoexMode.TDDRequest == FALSE)
					;/* SendAndesTFSWITCH(pAd, COEX_MODE_TDD); */
			}
			CoexTDDRXAGCGain(pAd);
		}
	}
#endif /* COEX_DYNAMIC_MODE_SWITCH */

	CoexFDDCRFineTune(pAd);
	do {
		UINT32 current_mode = pAd->CoexMode.CurrentMode;
		UINT32 update_mode = pAd->CoexMode.UpdateMode;
		BOOLEAN protect_updated = pAd->CoexMode.ProtectionUpdated;

		if ((current_mode == update_mode) &&
			!(protect_updated && (current_mode != COEX_MODE_FDD)))
			break;
		if (!INFRA_ON(pAd) && !RTMP_CFG80211_VIF_P2P_CLI_ON(pAd)
		    && !RTMP_CFG80211_VIF_P2P_GO_ON(pAd)) {
			pAd->CoexMode.CurrentMode = update_mode;
			break;
		}
		/* TODO::Need check flag bForce is on */
		DBGPRINT(RT_DEBUG_OFF,
			 ("%s, switch to %u, current:%u\n", __func__, update_mode, current_mode));
		if (!pAd->CoexMode.bForce)
			current_mode = update_mode;
		if (current_mode == COEX_MODE_TDD) {
			pAd->CoexMode.CurrentMode = current_mode;
			UpdateAndesNullFrameSpace(pAd);
		} else if (current_mode == COEX_MODE_FDD) {
			SendAndesAFH(pAd, pAd->CommonCfg.BBPCurrentBW, pAd->CommonCfg.Channel,
				     pAd->CommonCfg.CentralChannel, FALSE, QueryHashID(pAd,
									   pAd->CommonCfg.
									   Bssid, FALSE));
			pAd->CoexMode.CurrentMode = current_mode;
		} else if (current_mode == COEX_MODE_HYBRID) {
			pAd->CoexMode.CurrentMode = current_mode;
			UpdateAndesNullFrameSpace(pAd);
			SendAndesAFH(pAd, pAd->CommonCfg.BBPCurrentBW, pAd->CommonCfg.Channel,
					pAd->CommonCfg.CentralChannel, FALSE, QueryHashID(pAd,
									   pAd->CommonCfg.
									   Bssid, FALSE));
		} else {
			DBGPRINT(RT_DEBUG_WARN,
				 ("Un-expected mode change, current:%u, update:%u\n",
				  current_mode, update_mode));
		}
	} while (0);
}

VOID COEXLinkDown(IN PRTMP_ADAPTER pAd, PUCHAR pAddr)
{
	INT ret = 0;

	InvalidProtectionFrameSpace(pAd, pAddr);
	UpdateAndesNullFrameSpace(pAd);
	DBGPRINT(RT_DEBUG_OFF, ("%s, BSSID:%02x:%02x:%02x:%02x:%02x:%02x]\n"
			, __func__, PRINT_MAC(pAddr)));

	TDDFDDExclusiveRequest(pAd, COEX_MODE_RESET);
	MLMEHook(pAd, WLAN_CONNECTION_CANCEL_IND, QueryHashID(pAd, pAddr, TRUE));

	if (pAd->CoexMode.LinkStatus&COEX_GO_LINK) {
		DBGPRINT(RT_DEBUG_TRACE, ("COEX, %s, GO still ON\n", __func__));
		EstablishFrameBundle(pAd, pAd->cfg80211_ctrl.P2PCurrentAddress,
					pAd->cfg80211_ctrl.P2PCurrentAddress, COEX_OPMODE_GO,
					&pAd->MacTab.Content[0]);
		return;
	} else if (pAd->CoexMode.LinkStatus&COEX_GC_LINK) {
		DBGPRINT(RT_DEBUG_TRACE, ("COEX, %s, GC still ON\n", __func__));
		return;
	} else if (INFRA_ON(pAd)) {
		DBGPRINT(RT_DEBUG_TRACE, ("COEX, %s, Infra still ON\n", __func__));
		EstablishFrameBundle(pAd, pAd->CurrentAddress, pAd->CommonCfg.Bssid,
					COEX_OPMODE_STA, &pAd->MacTab.Content[BSSID_WCID]);
		return;
	}

	if (pAd->CoexMode.CurrentMode == COEX_MODE_FDD) {
		BtAFHCtl(pAd, pAd->CommonCfg.BBPCurrentBW, pAd->CommonCfg.Channel,
			 pAd->CommonCfg.CentralChannel, TRUE);
		ret =
		    SendAndesAFH(pAd, pAd->CommonCfg.BBPCurrentBW, pAd->CommonCfg.Channel,
				 pAd->CommonCfg.CentralChannel, TRUE, QueryHashID(pAd, pAddr,
										  FALSE));
	}
}

VOID CoexParseBTStatus(IN PRTMP_ADAPTER pAd)
{
	PBT_STATUS_REPORT_STRUC pBtstatus = (PBT_STATUS_REPORT_STRUC) &(pAd->BTStatusFlags);

	if (BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_ACTIVE))
		DBGPRINT(RT_DEBUG_TRACE, ("COEX: BT ACTIVE !!!!\n"));
	if (BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_BWL))
		DBGPRINT(RT_DEBUG_TRACE, ("COEX:  bandwidth-limited link such as A2DP\n"));
	if (BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_BE))
		DBGPRINT(RT_DEBUG_TRACE,
			 ("COEX: best effort to serve non-QoS-guaranteed link such as PAN, FTP\n"));
	if (BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_SYNC))
		DBGPRINT(RT_DEBUG_TRACE, ("COEX: synchronous link(s) such as SCO, eSCO\n"));
	if (BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_SNIFF))
		DBGPRINT(RT_DEBUG_TRACE,
			 ("COEX: periodic transport(s) such as SNIFF, LE_CONN, LE_ADV\n"));
	if (BT_STATUS_TEST_FLAG(pAd, fBTSTATUS_BT_SCATTER))
		DBGPRINT(RT_DEBUG_TRACE, ("COEX: scatter operation either PAGE or INQUIRY\n"));

	DBGPRINT(RT_DEBUG_TRACE,
		 ("COEX: BT_LINK_CNT = %d, BTE_LINK_CNT= %d, TotalCount = %d, BT_SLAVE_IND = %d\n",
		  pBtstatus->field.BT_LINK_CNT, pBtstatus->field.BLE_LINK_CNT,
		  pBtstatus->field.BT_LINK_CNT + pBtstatus->field.BLE_LINK_CNT,
		  pBtstatus->field.BT_SLAVE_IND));
}

VOID CoexFDDCRFineTune(IN PRTMP_ADAPTER pAd)
{
	/* According to MT7662 SA comment:
	   To fix WiFi sensitivity degrade issue at some BT channels, we need to apply:
	   (1) Narrowest Analog Filter BW(RF 0x24c=0x3F7F7F7F)
	   (2) PD Threshold (0x238C[31:24]=0x16)
	 */
	if (IS_MT7662(pAd) && (pAd->CoexMode.CurrentMode == COEX_MODE_FDD)) {
		UINT32 val = 0;

		if (pAd->CoexMode.FDD_CRUpdated) {
			/* FDD CRs have been updated, check if BT is inactive, then we need to restore the FDD CRs */
			if (!BT_STATUS_TEST_LINK_UP(pAd)) {
				if (pAd->CoexMode.RF0NarrAnaFiltBW_OrigVal != 0) {
					DBGPRINT(RT_DEBUG_ERROR,
						 ("* BT is inactive, restore coex RF and PD CR settings\n"));
					DBGPRINT(RT_DEBUG_TRACE,
						 ("%s:%d, restore rf0_24c to 0x%08x\n",
						  __func__, __LINE__,
						  pAd->CoexMode.RF0NarrAnaFiltBW_OrigVal));
					mt_rf_write(pAd, 0, 0x24C,
						    (u32) pAd->CoexMode.RF0NarrAnaFiltBW_OrigVal);
					pAd->CoexMode.RF0NarrAnaFiltBW_OrigVal = 0;
				}
				if (pAd->CoexMode.RF1NarrAnaFiltBW_OrigVal != 0) {
					DBGPRINT(RT_DEBUG_TRACE,
						 ("%s:%d, restore rf1_24c to 0x%08x\n",
						  __func__, __LINE__,
						  pAd->CoexMode.RF1NarrAnaFiltBW_OrigVal));
					mt_rf_write(pAd, 1, 0x24C,
						    (u32) pAd->CoexMode.RF1NarrAnaFiltBW_OrigVal);
					pAd->CoexMode.RF1NarrAnaFiltBW_OrigVal = 0;
				}
				if (pAd->CoexMode.PDThreashold_OrigVal != 0) {
					DBGPRINT(RT_DEBUG_TRACE, ("%s:%d, restore PD to 0x%02x\n",
								  __func__, __LINE__,
								  pAd->
								  CoexMode.PDThreashold_OrigVal));
					RTMP_BBP_IO_READ32(pAd, AGC1_R35, &val);
					val =
					    ((val & 0x00ffffff) |
					     ((pAd->CoexMode.PDThreashold_OrigVal) << 24));
					RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, val);
					pAd->CoexMode.PDThreashold_OrigVal = 0;
				}
				pAd->CoexMode.FDD_CRUpdated = 0;
			}
		} else {
			/* FDD CR not been updated, check if BT is active, need to apply the FDD CRs */
			if (BT_STATUS_TEST_LINK_UP(pAd)) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("* BT is active, apply coex RF and PD CR settings\n"));
				/****** (1) Narrowest Analog Filter BW(RF 0x24c=0x3F7F7F7F) ********/
#define NARROWEST_ANALOG_FILTER_BW_VAL		(0x3F7F7F7F)
				/* backup rf original values */
				mt_rf_read(pAd, 0, 0x24C, &val);
				pAd->CoexMode.RF0NarrAnaFiltBW_OrigVal = val;
				mt_rf_read(pAd, 1, 0x24C, &val);
				pAd->CoexMode.RF1NarrAnaFiltBW_OrigVal = val;
				DBGPRINT(RT_DEBUG_TRACE,
					 ("%s:%d, original rf0_24c=0x%08x, rf1_24c=0x%08x\n",
					  __func__, __LINE__,
					  pAd->CoexMode.RF0NarrAnaFiltBW_OrigVal,
					  pAd->CoexMode.RF1NarrAnaFiltBW_OrigVal));
				/* replace with new values */
				mt_rf_write(pAd, 0, 0x24C, (u32) NARROWEST_ANALOG_FILTER_BW_VAL);
				mt_rf_write(pAd, 1, 0x24C, (u32) NARROWEST_ANALOG_FILTER_BW_VAL);
				/* read again to confirm */
				mt_rf_read(pAd, 0, 0x24C, &val);
				DBGPRINT(RT_DEBUG_TRACE,
					 ("%s:%d, new rf0_24c=0x%08x\n", __func__, __LINE__, val));
				mt_rf_read(pAd, 1, 0x24C, &val);
				DBGPRINT(RT_DEBUG_TRACE,
					 ("%s:%d, new rf1_24c=0x%08x\n", __func__, __LINE__, val));
				/****** (2) PD Threshold (0x238C[31:24]=0x16) AGC1_R35 ********/
#define PD_THRESHOLD_COEX	(0x16)
				/* backup pd threshold original value */
				RTMP_BBP_IO_READ32(pAd, AGC1_R35, &val);
				pAd->CoexMode.PDThreashold_OrigVal = ((val >> 24) & 0xff);
				DBGPRINT(RT_DEBUG_TRACE,
					 ("%s:%d, original pd_thres=0x%08x(b[31:24]=0x%x)\n",
					  __func__, __LINE__, val,
					  pAd->CoexMode.PDThreashold_OrigVal));
				/* replace with new value */
				val = ((val & 0x00ffffff) | (PD_THRESHOLD_COEX << 24));
				RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, val);
				/* read again to confirm */
				RTMP_BBP_IO_READ32(pAd, AGC1_R35, &val);
				DBGPRINT(RT_DEBUG_TRACE,
					 ("%s:%d, new pd_thres=0x%08x\n", __func__, __LINE__, val));

				pAd->CoexMode.FDD_CRUpdated = 1;
			}
		}
	}

}

VOID CoexLinkUpHandler(RTMP_ADAPTER *pAd, struct COEX_LINKUP_INFO info)
{
	INT ret = 0;

	if (MAC_ADDR_EQUAL(info.Bssid, ZERO_MAC_ADDR)) {
		DBGPRINT(RT_DEBUG_ERROR,
		 ("%s, CommonCfg.Bssid Zero Addr\n",
		  __func__));
		return;
	}

	if (!info.wdev) {
		DBGPRINT(RT_DEBUG_ERROR,
		 ("%s, Null wdev, coex type:0x%x\n",
		  __func__, info.coexType));
		return;
	}

	if (!info.pEntry) {
		DBGPRINT(RT_DEBUG_ERROR,
		 ("%s, Null MacTable, coex type:0x%x\n",
		  __func__, info.coexType));
		return;
	}

	if (IS_MT76XXBTCOMBO(pAd)) {
		if (IS_MT7650(pAd) || IS_MT7610(pAd)) {
			/* Only needed by MT7650 */
			BtAFHCtl(pAd, info.BBPCurrentBW, info.Channel,
				 info.CentralChannel, FALSE);
		}

		ret = SendAndesAFH(pAd, info.BBPCurrentBW, info.Channel,
				info.CentralChannel, FALSE, QueryHashID(pAd, info.Bssid, FALSE));
		TDDFDDExclusiveRequest(pAd, COEX_MODE_RESET);
		if (info.Channel > 14) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s, channel %x > 14\n",
					__func__, info.Channel));
			if (pAd->CoexMode.CurrentMode != COEX_MODE_FDD)
				Set_ChangeCOEXMode_Proc(pAd, "FDD");
		} else {
			RTMP_CHIP_OP *pChipOps = &pAd->chipOps;
			UINT16 eeprom = 0;

			pChipOps->eeread(pAd, EXTERN_COMPONENT_SETTING_3, &eeprom);
			eeprom &= 0x00ff;
			EstablishFrameBundle(pAd, info.OwnMac, info.Bssid,
						info.coexType, info.pEntry);
			if ((eeprom == COEX_TDD_ANT_SET) &&
				(pAd->CoexMode.CurrentMode != COEX_MODE_TDD)) {
					Set_ChangeCOEXMode_Proc(pAd, "TDD");
				DBGPRINT(RT_DEBUG_ERROR, ("%s, channel %x eprom 2 ant\n",
					__func__, info.Channel));
			}
		}
		pAd->CoexMode.LinkStatus |= (1<<info.coexType);

		DBGPRINT(RT_DEBUG_TRACE, ("%s, type:%x, BW:%x, ch:%x, ctrl_ch:%x, link stat:%x\n"
			    , __func__, info.coexType, info.BBPCurrentBW
			    , info.Channel, info.CentralChannel, pAd->CoexMode.LinkStatus));
		DBGPRINT(RT_DEBUG_TRACE, ("%s, BSSID:%02x:%02x:%02x:%02x:%02x:%02x, Hash:%x\n"
			    , __func__, PRINT_MAC(info.Bssid)
			    , QueryHashID(pAd, info.Bssid, FALSE)));
	}
}

#ifdef COEX_FDD_AUTO_SWITCH_TO_BW20
VOID CoexFDD_HtBwSwitchCheck(RTMP_ADAPTER *pAd)
{
	/* General rule:
	   When BT link is ON, check if Infra is connected with non-BW20 AP.
	   - If not, do nothing
	   - If yes, do LinkDown first, then set BW to BW_20 (and remeber origianl BW settings)
	   When BT link is OFF, check if the BW has been forced changed.
	   - If yes, switch back to original settings
	 */
	static INT32 CoexFddEverForceSwitchBw20;
	static INT32 CoexFddOriginalBw = BW_20;
	static INT32 CoexFddBTLinkDownCount;
	CHAR TmpStr[8] = "";

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s --> BW = %d , ch = %d , crl_ch = %d , infra_on = %d , BT_STATUS_TEST_LINK_UP = %d\n",
		  __func__, pAd->CommonCfg.RegTransmitSetting.field.BW, pAd->CommonCfg.Channel,
		  pAd->CommonCfg.CentralChannel, INFRA_ON(pAd), BT_STATUS_TEST_LINK_UP(pAd)));

	if (!CoexFddEverForceSwitchBw20 && pAd->CommonCfg.RegTransmitSetting.field.BW == BW_20) {
		/* already at BW20, don't need switch */
		return;
	}
	if (BT_STATUS_TEST_LINK_UP(pAd)) {
		/* If we never forced a BW switch, then we shall switch */
		if (CoexFddEverForceSwitchBw20 == 0) {
			if (INFRA_ON(pAd) && (pAd->CommonCfg.Channel <= 14) &&
			    (pAd->CommonCfg.Channel != pAd->CommonCfg.CentralChannel)) {
				/* backup original BW setting */
				CoexFddOriginalBw = pAd->CommonCfg.RegTransmitSetting.field.BW;
				CoexFddEverForceSwitchBw20 = 1;
				/* When
				   - Infra is connected &&
				   - Running on 2.4G band &&
				   - Running on non-BW20 mode
				   we need to set the driver to BW20 mode, and trigger disconnect
				 */
				DBGPRINT(RT_DEBUG_ERROR,
					 ("%s: BT Linkup and currently WiFi associated on 2.4G with BW40 mode, force link down\n",
					  __func__));
				LinkDown(pAd, FALSE);
				DBGPRINT(RT_DEBUG_ERROR,
					 ("%s: FDD WiFi at 2.4G band with non-BW20 mode, switching to BW20 mode...\n",
					  __func__));
				Set_HtBw_Proc(pAd, "0");
			}
		} else {
			/* do nothing */
		}
		CoexFddBTLinkDownCount = 0;
	} else {
		/* BT link is gone */
		if (CoexFddEverForceSwitchBw20) {
			if (!INFRA_ON(pAd)) {
				if (++CoexFddBTLinkDownCount >= 30) {
					/* Do recover to original mode only when BT link down 10 seconds.
					   This is to avoid ping-pong issue if BT link changes very frequently. */
					sprintf(TmpStr, "%d", CoexFddOriginalBw);
					DBGPRINT(RT_DEBUG_ERROR,
						 ("%s: BT link is off for %d sec, switch back to original mode (BW=%s)...\n",
						  __func__, CoexFddBTLinkDownCount, TmpStr));
					Set_HtBw_Proc(pAd, TmpStr);
					CoexFddEverForceSwitchBw20 = 0;
					CoexFddBTLinkDownCount = 0;
				}
			} else
				CoexFddBTLinkDownCount = 0;
		}
	}

}

#endif /* COEX_FDD_AUTO_SWITCH_TO_BW20 */

#endif /* MT76XX_BTCOEX_SUPPORT */
