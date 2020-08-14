#!/bin/sh
###############################################################################
SSID=
SECURITY=
PASSWD=
CHANNEL=
USB_DEVICES=/sys/bus/usb/devices
DEVICE_LIST="cf3/9271 cf3/9374 148f/3070 148f/5370 148f/5372 148f/5572 148f/7601 \
             bda/8176 bda/8178 bda/8179 bda/179 bda/8194 bda/818b"
IFACE=wlan0
DEVICE_NAME=""
USE_HOSTAPD=0
HOSTAPD_CFG=/var/run/hostapd/hostapd.conf
###############################################################################
find_device ()
{
	local product=${1}
	local device=""

	for id in $DEVICE_LIST
	do
		if [ "$product" = "$id" ]; then
			echo "Device matched"
			device=$product
			break
		fi
	done

	if [ -z $device ]; then
		return
	fi

	case ${device} in
	cf3/9271)
		DEVICE_NAME=usb_ar9271
		USE_HOSTAPD=1
		;;
	cf3/9374)
		DEVICE_NAME=usb_ar9374
		USE_HOSTAPD=1
		;;
	148f/3070)
		DEVICE_NAME=usb_rt3070
		;;
	148f/5370)
		DEVICE_NAME=usb_rt5370
		;;
	148f/5372)
		DEVICE_NAME=usb_rt5372
		;;
	148f/5572)
		DEVICE_NAME=usb_rt5572
		;;
	148f/7601)
		DEVICE_NAME=usb_mt7601u
		;;
	bda/8176)
		DEVICE_NAME=usb_rtl8188cu
		USE_HOSTAPD=1
		;;
	bda/8178)
		DEVICE_NAME=usb_rtl8192cu
		USE_HOSTAPD=1
		;;
	bda/8179)
		DEVICE_NAME=usb_rtl8188eu
		USE_HOSTAPD=1
		;;
	bda/179)
		DEVICE_NAME=usb_rtl8188etv
		USE_HOSTAPD=1
		;;
	bda/8194)
		DEVICE_NAME=usb_rtl8192du
		USE_HOSTAPD=1
		;;
	bda/818b)
		DEVICE_NAME=usb_rtl8192eu
		USE_HOSTAPD=1
		;;
	*)
		echo "Device is not supported."
		;;
	esac
}

start_ap_with_hostapd ()
{
	test -d /var/run/hostapd || mkdir -p /var/run/hostapd

	killall hostapd

	(echo "interface=$IFACE"; \
		echo "driver=nl80211"; \
		echo "ctrl_interface=/var/run/hostapd"; \
		echo "ssid=$SSID"; \
		echo "channel=$CHANNEL"; \
		echo "hw_mode=g"; \
		echo "ieee80211n=1") > $HOSTAPD_CFG

		pwd_len=`echo "$PASSWD" | awk '{print length($0)}'`

		if [ "$SECURITY" = "WEP" ]; then
			if [ $pwd_len = 5 -o $pwd_len = 13 ]; then
				(echo "wep_default_key=0"; \
					echo "wep_key0=\"$PASSWD\"") >> $HOSTAPD_CFG
			elif [ $pwd_len = 10 -o $pwd_len = 26 ]; then
				(echo "wep_default_key=0"; \
					echo "wep_key0=$PASSWD") >> $HOSTAPD_CFG
			else
				echo "Password length is wrong!"
				return
			fi
		fi

		if [ "$SECURITY" = "WPAPSK" ]; then
			if [ $pwd_len -le 8 ]; then
				echo "Password length is wrong!"
				return
			fi
			(echo "wpa=3"; \
				echo "wpa_key_mgmt=WPA-PSK"; \
				echo "wpa_pairwise=TKIP CCMP"; \
				echo "wpa_passphrase=$PASSWD") >> $HOSTAPD_CFG
		fi

		hostapd -B $HOSTAPD_CFG
}

start_ap_with_privcmd ()
{
	ifconfig $IFACE up
	
	iwpriv $IFACE set Channel=$CHANNEL
	if [ "$SECURITY" = "NONE" ]; then
		iwpriv $IFACE set AuthMode=OPEN
		iwpriv $IFACE set EncrypType=NONE
	fi

	if [ "$SECURITY" = "WEP" ]; then
		iwpriv $IFACE set AuthMode=WEPAUTO
		iwpriv $IFACE set EncrypType=WEP
		iwpriv $IFACE set DefaultKeyID=1
		iwpriv $IFACE set Key1=$PASSWD
	fi

	iwpriv $IFACE set SSID=$SSID

	if [ "$SECURITY" = "WPAPSK" ]; then
		iwpriv $IFACE set AuthMode=WPA2PSK
		iwpriv $IFACE set EncrypType=AES
		iwpriv $IFACE set WPAPSK=$PASSWD
		iwpriv $IFACE set SSID=$SSID
	fi

}

start_ap ()
{
	for device in `ls $USB_DEVICES`
	do
		product=`cat $USB_DEVICES/$device/uevent`
		product=$(echo $product | sed 's/.*PRODUCT=//' | awk '{print $1}')
		product=$(echo $product | sed 's/\//#/2' | sed 's/\#.*$//')

		find_device $product
		if [ "$DEVICE_NAME" ]; then
			break
		fi
	done

	if [ -z "$DEVICE_NAME" ]; then
		echo "Device not matched"
		return
	fi

	echo "Loading driver..."
	MSG=`ifconfig -a | grep -r $IFACE`
	if [ ss"$MSG" = ss"" ]; then
		wifi_drv_load.${DEVICE_NAME}ap
		sleep 2
	fi

	echo "AP start..."
	if [ $USE_HOSTAPD = 1 ]; then
		start_ap_with_hostapd
	else
		start_ap_with_privcmd
	fi

}

do_help ()
{
	self=`basename $0`
	echo "Usage:"
	echo "  ./$self ssid security password channel"
	echo "    ssid      -  SSID"
	echo "    security  -  such as NONE, WEP, WPAPSK"
	echo "    password  -  password for security"
	echo "    channel   -  1 - 11"
	echo "Example:"
	echo "  ./$self HisiAP NONE 1"
	echo "  ./$self HisiAP WEP 12345 1"
	echo "  ./$self HisiAP WPAPSK 123456789a 1"
	exit 0
}
###############################################################################
if [ $# = 3 ]; then
	SSID="$1"
	SECURITY="$2"
	CHANNEL=$3
	start_ap
elif [ $# = 4 ]; then
	SSID="$1"
	SECURITY="$2"
	PASSWD="$3"
	CHANNEL=$4
	start_ap
else
	do_help
fi
