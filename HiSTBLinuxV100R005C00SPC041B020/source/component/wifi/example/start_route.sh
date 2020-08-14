#!/bin/sh
################################################################################
#    Create By Czy
################################################################################
#    This is a wifi application sample
#
#                router           Internet, dhcp-server
#                  |
#                  | eth0         IFu=eth0, dhcp-client
#          --------|------------
#          |       | NAT       |
#          |       |           |
#          |    |--+--|        |  IFd=br0, IPd=192.168.0.1, dhcp-server
#          |    |     | bridge |
#          -----|-----|---------
#               |eth1 |wlan0      IFd1=eth1, IFd2=wlan0 
#              /       \
#            pc0       pc1        dhcp-client
#
################################################################################

# Uplink port. This port link to Internet
IFu=eth0
# downlink port. This port link pc
IFd=br0
IFd0=eth1
IFd1=wlan0

IPd=192.168.0.1
IPd_mask=255.255.255.0
# dhcp client address pool
IPd_start=192.168.0.2
IPd_end=192.168.0.254

################################################################################
# local variable
IPu=
DNS=
################################################################################
# $1 - deamon name
daemon_stop ()
{
	local ix=0
	local MAXTIMES=20
	local DEAMON=${1}
	local MSG="busybox ps | grep -r ${DEAMON}"

	if [ ss"$(eval ${MSG})" == ss"" ]; then
		return 0
	fi

	echo -n "$(start-stop-daemon -K -n ${DEAMON}) wait"

	while [ ${ix} -lt ${MAXTIMES} ]; do
		if [ ss"$(eval ${MSG})" != ss"" ]; then
			sleep 1s
			ix=$((${ix}+1))
		else
			echo "ok"
			return 0
		fi
		echo -n "."
	done

	echo "fail"
	return 1
}
################################################################################
# config bridge
ifconfig ${IFd0} 0.0.0.0 
ifconfig ${IFd1} 0.0.0.0
ifconfig ${IFd} down
brctl delbr ${IFd}
brctl addbr ${IFd}
brctl addif ${IFd} ${IFd0} 
brctl addif ${IFd} ${IFd1}
ifconfig ${IFd} ${IPd} up

################################################################################
# get internet ip from uplink port
daemon_stop udhcpc
udhcpc -i ${IFu}
TMP=$(ifconfig ${IFu} | grep "inet addr:")
IPu=$(echo ${TMP:20} | sed -e 's/Bcast.*$/ /')
echo "Uplink port ip: ${IPu}"

################################################################################
# get internet dns from uplink port
TMP=$(cat /etc/resolv.conf | grep "nameserver" | sed -e 's/nameserver/ /g')
DNS=$(echo ${TMP})
echo "DNS: ${DNS}"

################################################################################
# config downlink port dhcp server
DHCPD_CFG=/etc/udhcpd.conf
{
	echo start ${IPd_start}
	echo end   ${IPd_end}
	echo interface ${IFd}
	echo;
	echo option dns ${DNS}
	echo option lease 864000
	echo option router ${IPd}
	echo option subnet ${IPd_mask}
} > ${DHCPD_CFG}
daemon_stop udhcpd
udhcpd ${DHCPD_CFG}
echo "start udhcpd at interface ${IFd}"

################################################################################
# config nat
echo "1" >/proc/sys/net/ipv4/ip_forward
iptables -F
iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -t nat -A POSTROUTING -s ${IPd}/${IPd_mask} -o ${IFu} -j SNAT --to-source ${IPu}
echo "wait for connect"
