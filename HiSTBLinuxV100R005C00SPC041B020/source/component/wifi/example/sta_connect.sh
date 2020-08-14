#!/bin/sh
################################################################################
#    Create By czy
################################################################################
WLAN=${1}
SSID="${2}"
SECURITY=${3}
PASSWORD="${4}"
# For RT3070 RT5370 RT5572, please set DRIVER=wext
DRIVER=nl80211

if [ "$#" != "3" -a "$#" != "4" ]; then
	SELF=`basename $0`
	echo "This script support Atheros and Ralink."
	echo "Usage:"
	echo "    $SELF interface ssid {WEP | WPAPSK | NONE} [password]"
	echo "        interface          - network interface, such as wlan0, ra0"
	echo "        ssid               - SSID string."
	echo "        {WEP|WPAPSK|NONE}  - security mode."
	echo "        password           - password when security mode is WEP"
	echo "                             or WPAPSK."
	echo "example:"
	echo "    $SELF wlan0 dlink WEP 0123456789"
	echo "    $SELF wlan0 dlink WPAPSK password"
	echo "    $SELF wlan0 dlink NONE"
	echo;
	exit 0
fi
################################################################################
NORMAL="\033[00m"
RED="\033[31m"
################################################################################
wpa_supplicant_start ()
{
	echo -e $RED'start wpa_supplicant'$NORMAL
	MSG0=`busybox ps`
	MSG=`echo $MSG0 | grep -r "wpa_supplicant -i$WLAN"`
	if [ ss"$MSG" != ss"" ]; then
		echo "wpa_supplicant is running"
		return 0
	fi

	ifconfig $WLAN down
	test -d /var/run/wpa_supplicant || mkdir /var/run/wpa_supplicant
	echo 'ctrl_interface=/var/run/wpa_supplicant' > /etc/wpa_supplicant.conf
	wpa_supplicant -i$WLAN -B -D$DRIVER -c/etc/wpa_supplicant.conf
}
################################################################################
wpa_supplicant_stop ()
{
	echo -e $RED'stop wpa_supplicant.'$NORMAL
	wpa_cli -i$WLAN terminate
}
################################################################################
run ()
{
	cmd=$1
	echo -ne "$cmd - "`eval $cmd`"\n"
}
################################################################################
wait_connect ()
{
	ix=0
	TRYTIMES=20
	echo -ne "connect "
	while [ $ix -lt $TRYTIMES ]; do
		MSG=`wpa_cli -i$WLAN status | grep -r "wpa_state=COMPLETED"`
		if [ ss"$MSG" = ss"" ]; then
			sleep 1s
			ix=$(($ix+1))
			echo -ne "."
		else
			echo " ok"
			return 0
		fi
	done
	echo " timeout"
	return 1
}
################################################################################
wifi_sta_connect ()
{
	REL=1
	echo -e $RED'prepare connect wifi ap.'$NORMAL
	wpa_cli -i$WLAN disable_network all > /dev/null

	run "wpa_cli -i$WLAN remove_network all"
	ifconfig $WLAN down
	ifconfig $WLAN up

	case "$SECURITY" in
	"WPAPSK" )
		wifi_sta_security_wpa_psk
		REL=$?
	;;
	"WEP" )
		wifi_sta_security_wep
		REL=$?
	;;
	"NONE" )
		wifi_sta_security_none
		REL=$?
	;;
	* )
		echo "Not recognise security mode"
		echo "SECURITY=$SECURITY"
		REL=1
	;;
	esac

	if [ "$REL" = "1" ]; then
		echo -e $RED'connect wifi ap fail.'$NORMAL
		wpa_cli -i$WLAN status
		wpa_cli -i$WLAN remove_network all >/dev/null 2>&1
		ifconfig $WLAN down
		return 1
	else
		echo -e $RED'connect wifi ap success.'$NORMAL
		wpa_cli -i$WLAN status
		return 0
	fi

}
################################################################################
# no password
wifi_sta_security_none ()
{
	NETWORK_ID=`wpa_cli -i$WLAN add_network`
	run "wpa_cli -i$WLAN set_network $NETWORK_ID ssid '\"$SSID\"'"
	run "wpa_cli -i$WLAN set_network $NETWORK_ID key_mgmt NONE"
	run "wpa_cli -i$WLAN set_network $NETWORK_ID scan_ssid 1"
	run "wpa_cli -i$WLAN enable_network $NETWORK_ID"
	wait_connect
	return $?
}
################################################################################
# wep
wifi_sta_security_wep ()
{
	PWD_LEN=`echo "$PASSWORD" | awk '{print length($0)}'`
	NETWORK_ID=`wpa_cli -i$WLAN add_network`
	run "wpa_cli -i$WLAN set_network $NETWORK_ID ssid '\"$SSID\"'"
	run "wpa_cli -i$WLAN set_network $NETWORK_ID key_mgmt NONE"
	run "wpa_cli -i$WLAN set_network $NETWORK_ID scan_ssid 1"
	if [ $PWD_LEN = 5 -o $PWD_LEN = 13 ];then
		run "wpa_cli -i$WLAN set_network $NETWORK_ID wep_key0 '\"$PASSWORD\"'"
	elif [ $PWD_LEN = 10 -o $PWD_LEN = 26 ];then
		run "wpa_cli -i$WLAN set_network $NETWORK_ID wep_key0 '$PASSWORD'"
	else
		echo "Password length is wrong!"
	fi
	run "wpa_cli -i$WLAN enable_network $NETWORK_ID"
	wait_connect
	return $?
}
################################################################################
# wpa-psk
wifi_sta_security_wpa_psk ()
{
	NETWORK_ID=`wpa_cli -i$WLAN add_network`
	run "wpa_cli -i$WLAN set_network $NETWORK_ID scan_ssid 1"
	run "wpa_cli -i$WLAN set_network $NETWORK_ID ssid '\"$SSID\"'"
	run "wpa_cli -i$WLAN set_network $NETWORK_ID proto 'WPA RSN'"
	run "wpa_cli -i$WLAN set_network $NETWORK_ID key_mgmt WPA-PSK"
	run "wpa_cli -i$WLAN set_network $NETWORK_ID pairwise 'CCMP TKIP'"
	run "wpa_cli -i$WLAN set_network $NETWORK_ID group 'CCMP TKIP'"
	run "wpa_cli -i$WLAN set_network $NETWORK_ID psk '\"$PASSWORD\"'"
	run "wpa_cli -i$WLAN enable_network $NETWORK_ID"
	wait_connect
	return $?
}
################################################################################

echo "=========================================================="
echo "WLAN     =$WLAN"
echo "SSID     =$SSID"
echo "PASSWORD =$PASSWORD"
echo "SECURITY =$SECURITY"
echo "=========================================================="

wpa_supplicant_start
wifi_sta_connect
