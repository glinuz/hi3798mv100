For open Wi-Fi, please follow bellow steps:

1. RTL8188ETV and RTL8188EUS
    1) for STA
        insmod /kmod/cfg80211.ko
        insmod /kmod/rtl8188eu.ko
        wpa_supplicant -iwlan0 -Dnl80211 -c/etc/Wireless/wpa_supplicant.conf &
    2) for SoftAP
        insmod /kmod/cfg80211.ko
        insmod /kmod/rtl8188eu.ko
        hostapd /etc/Wireless/hostapd.conf &
        ifconfig wlan0 192.168.49.1
        udhcpd -fS /etc/udhcpd.conf
2. MT7601U
    1) for STA
        insmod /kmod/cfg80211.ko
        insmod /kmod/mtprealloc7601Usta.ko
        insmod /kmod/mt7601Usta.ko
        wpa_supplicant -iwlan0 -Dnl80211 -c/etc/Wireless/wpa_supplicant.conf &
    2) for SoftAP
        insmod /kmod/mt7601Uap.ko
        ifconfig wlan0 up
        ifconfig wlan0 192.168.49.1
        udhcpd -fS /etc/udhcpd.conf
3. RTL8192EU
    1) for STA
        insmod /kmod/cfg80211.ko
        insmod /kmod/rtl8192eu.ko
        wpa_supplicant -iwlan0 -Dnl80211 -c/etc/Wireless/wpa_supplicant.conf &
    2) for SoftAP
        insmod /kmod/cfg80211.ko
        insmod /kmod/rtl8192eu.ko
        hostapd /etc/Wireless/hostapd.conf &
        ifconfig wlan0 192.168.49.1
        udhcpd -fS /etc/udhcpd.conf
4. RT3070 and RT5370
    1) for STA
        insmod /kmod/rtutil5572sta.ko
        insmod /kmod/rt5572sta.ko
        insmod /kmod/rtnet5572sta.ko
        wpa_supplicant -iwlan0 -Dwext -c/etc/Wireless/wpa_supplicant.conf &
    2) for SoftAP
        insmod /kmod/rtutil5370ap.ko
        insmod /kmod/rt5370ap.ko
        insmod /kmod/rtnet5370ap.ko
        ifconfig wlan0 up
        ifconfig wlan0 192.168.49.1
        udhcpd -fS /etc/udhcpd.conf
5. RT5572
    1) for STA
        insmod /kmod/rtutil5572sta.ko
        insmod /kmod/rt5572sta.ko
        insmod /kmod/rtnet5572sta.ko
        wpa_supplicant -iwlan0 -Dwext -c/etc/Wireless/wpa_supplicant.conf &
    2) for SoftAP
        insmod /kmod/rtutil5572ap.ko
        insmod /kmod/rt5572ap.ko
        insmod /kmod/rtnet5572ap.ko
        ifconfig wlan0 up
        ifconfig wlan0 192.168.49.1
        udhcpd -fS /etc/udhcpd.conf
6. RTL8188CUS and RTL8192CU
    1) for STA
        insmod /kmod/cfg80211.ko
        insmod /kmod/rtl8192cu.ko
        wpa_supplicant -iwlan0 -Dnl80211 -c/etc/Wireless/wpa_supplicant.conf &
    2) for SoftAP
        insmod /kmod/cfg80211.ko
        insmod /kmod/rtl8192cu.ko
        hostapd /etc/Wireless/hostapd.conf &
        ifconfig wlan0 192.168.49.1
        udhcpd -fS /etc/udhcpd.conf
7. RTL8192DU
    1) for STA
        insmod /kmod/cfg80211.ko
        insmod /kmod/rtl8192du.ko
        wpa_supplicant -iwlan0 -Dnl80211 -c/etc/Wireless/wpa_supplicant.conf &
    2) for SoftAP
        insmod /kmod/cfg80211.ko
        insmod /kmod/rtl8192du.ko
        hostapd /etc/Wireless/hostapd.conf &
        ifconfig wlan0 192.168.49.1
        udhcpd -fS /etc/udhcpd.conf
8. AR9271
    1) for STA
        insmod /kmod/cfg80211.ko
        insmod /kmod/mac80211.ko
        insmod /kmod/ath.ko
        insmod /kmod/ath9k_hw.ko
        insmod /kmod/ath9k_common.ko
        insmod /kmod/ath9k_htc.ko
        insmod /kmod/ath9k.ko
        wpa_supplicant -iwlan0 -Dnl80211 -c/etc/Wireless/wpa_supplicant.conf &
    2) for SoftAP
        insmod /kmod/cfg80211.ko
        insmod /kmod/mac80211.ko
        insmod /kmod/ath.ko
        insmod /kmod/ath9k_hw.ko
        insmod /kmod/ath9k_common.ko
        insmod /kmod/ath9k_htc.ko
        insmod /kmod/ath9k.ko
        hostapd /etc/Wireless/hostapd.conf &
        ifconfig wlan0 192.168.49.1
        udhcpd -fS /etc/udhcpd.conf
9. AR9374 and QCA1021G and QCA1021X
    1) for STA
        insmod /kmod/cfg80211_ath6k.ko
        insmod /kmod/ath6kl_usb.ko
        wpa_supplicant -iwlan0 -Dnl80211 -c/etc/Wireless/wpa_supplicant.conf &
    2) for SoftAP
        insmod /kmod/cfg80211_ath6k.ko
        insmod /kmod/ath6kl_usb.ko
        hostapd /etc/Wireless/hostapd.conf &
        ifconfig wlan0 192.168.49.1
        udhcpd -fS /etc/udhcpd.conf
10. BCM43236
    1) for STA
        insmod /kmod/cfg80211.ko
        insmod /kmod/bcmdhd.ko
        wpa_supplicant -iwlan0 -Dnl80211 -c/etc/Wireless/wpa_supplicant.conf &
    2) for SoftAP
        insmod /kmod/cfg80211.ko
        insmod /kmod/bcmdhd.ko
        hostapd /etc/Wireless/hostapd.conf &
        ifconfig wlan0 192.168.49.1
        udhcpd -fS /etc/udhcpd.conf
11. RTL8723BU
    1) for STA
        insmod /kmod/cfg80211.ko
        insmod /kmod/rtl8723bu.ko
        wpa_supplicant -iwlan0 -Dnl80211 -c/etc/Wireless/wpa_supplicant.conf &
    2) for SoftAP
        insmod /kmod/cfg80211.ko
        insmod /kmod/rtl8723bu.ko
        hostapd /etc/Wireless/hostapd.conf &
        ifconfig wlan0 192.168.49.1
        udhcpd -fS /etc/udhcpd.conf
12. MT7632U
    1) for STA
        insmod /kmod/cfg80211.ko
        insmod /kmod/mt76x2u_sta.ko
        wpa_supplicant -iwlan0 -Dnl80211 -c/etc/Wireless/wpa_supplicant.conf &
    2) for SoftAP
        insmod /kmod/cfg80211.ko
        insmod /kmod/mt76x2u_sta.ko
        hostapd /etc/Wireless/hostapd.conf &
        ifconfig wlan0 192.168.49.1
        udhcpd -fS /etc/udhcpd.conf


Notes:
1. For MT7601U and RT5370, please set "coherent_pool=1M" in bootargs.
2. For MT7601U, when rmmod driver, don't rmmod mtprealloc7601Usta.ko.
