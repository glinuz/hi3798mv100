/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2009, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    sta_cfg.h

    Abstract:

    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------

*/

#ifndef __STA_CFG_H__
#define __STA_CFG_H__

INT RTMPSTAPrivIoctlSet(IN RTMP_ADAPTER * pAd, IN PSTRING SetProcName,
			IN PSTRING ProcArg);
INT RtmpIoctl_rt_private_get_statistics(IN RTMP_ADAPTER * pAd,
			IN VOID * pData, IN ULONG Data);
#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
/* set WOW enable */
INT Set_WOW_Enable(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set GPIO pin for wake-up signal */
INT Set_WOW_GPIO(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set delay time for WOW really enable */
INT Set_WOW_Delay(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set wake up hold time */
INT Set_WOW_Hold(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set wakeup signal type */
INT Set_WOW_InBand(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set GPIO high/low active */
INT Set_WOW_HighActive(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set advanced wow enable */
INT Set_Adv_WOW_Enable(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set advanced  wow sleep cycle time */
INT Set_Adv_WOW_SleepTime(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set GPIO configure by wmt commnd */
INT Set_WOW_WmtGPIOCmd(IN PRTMP_ADAPTER	 pAd, IN PSTRING arg);
/* set always trigger wakeup */
INT Set_WOW_Always_Trigger(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set WOW wake-on port enable */
INT Set_WOW_WakeOnPort_Enable(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set IPv4 TCP port for wake up  */
INT Set_WOW_TcpPort_v4(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set IPv6 TCP port for wake up  */
INT Set_WOW_TcpPort_v6(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set IPv4 UDP port for wake up  */
INT Set_WOW_UdpPort_v4(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set IPv6 UDP port for wake up  */
INT Set_WOW_UdpPort_v6(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set IP for wake up  */
INT Set_WOW_IP(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set GPIO Mode two-pin/share/dedicated in wmt*/
INT Set_Wmt_GPIOMode(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set WiFi GPIO high/low active in wmt*/
INT Set_Wmt_WiFiHighActive(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set BT GPIO high/low active in wmt*/
INT Set_Wmt_BTHighActive(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set the enable toggle for WOW in wmt*/
INT Set_Wmt_WiFiWake1Toggle(IN PRTMP_ADAPTER	pAd, IN PSTRING	arg);
/* set the enable toggle for WOBLE on mode in wmt */
INT Set_Wmt_WiFiWake2Toggle(IN PRTMP_ADAPTER pAd, IN PSTRING	arg);
/* set the enable toggle for WOBLE ambilight mode in wmt */
INT Set_Wmt_WiFiWake3Toggle(IN PRTMP_ADAPTER	pAd, IN PSTRING	arg);
/* set the enable toggle for WOW in wmt*/
INT Set_Wmt_BTWake1Toggle(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set the enable toggle for WOBLE on mode in wmt */
INT Set_Wmt_BTWake2Toggle(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set the enable toggle for WOBLE ambilight mode in wmt */
INT Set_Wmt_BTWake3Toggle(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set the hold time for WOW in wmt */
INT Set_Wmt_WiFiWake1HoldTime(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set the hold time for WOBLE on mode in wmt */
INT Set_Wmt_WiFiWake2HoldTime(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set the hold time for WOBLE ambilight mode in wmt */
INT Set_Wmt_WiFiWake3HoldTime(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set thel hold time for WOW in wmt*/
INT Set_Wmt_BTWake1HoldTime(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set the hold time for WOBLE on mode in wmt */
INT Set_Wmt_BTWake2HoldTime(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* set the hold time for WOBLE ambilight mode in wmt */
INT Set_Wmt_BTWake3HoldTime(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */

#ifdef THERMAL_PROTECT_SUPPORT
INT set_thermal_protection_criteria_proc(IN RTMP_ADAPTER * pAd, IN PSTRING arg);
INT set_1t_Enable(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
#endif /* THERMAL_PROTECT_SUPPORT */

#ifdef RTMP_MAC_USB
/* Sets the FW into WOW Suspend mode */
INT Set_UsbWOWSuspend(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
/* Resume the FW to Normal mode */
INT Set_UsbWOWResume(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

/* set USB2 Disconnect mode in USB disconnect feature */
INT	Set_usb2_Disconnect(
		IN PRTMP_ADAPTER		pAd,
		IN PSTRING				arg);
/* set GPIO notify interrupt mode in USB disconnect feature */
INT	Set_GPIO_Notify_Mode(
		IN PRTMP_ADAPTER		pAd,
		IN PSTRING				arg);
/* set notify from GPIO pin in USB disconnect feature */
INT	Set_GPIO_Notify_Pin(
		IN PRTMP_ADAPTER		pAd,
		IN PSTRING				arg);


INT Set_Register_Dump(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

INT Set_Chip_Reset(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

/* USB2.0 & USB3.0 switch */
INT Set_UsbSwitch(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
#endif /* RTMP_MAC_USB */

INT Set_Enable_Vendor_IE_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

#endif /* __STA_CFG_H__ */
