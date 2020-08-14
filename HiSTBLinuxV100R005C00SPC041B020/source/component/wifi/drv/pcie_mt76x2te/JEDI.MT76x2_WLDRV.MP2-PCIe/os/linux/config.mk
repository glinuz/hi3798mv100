# Support Multiple Interface within single driver
HAS_MULTI_INF=n

# Support ATE function
HAS_ATE=n

# Support QA ATE function
HAS_QA_SUPPORT=n
# Support DYNAMIC_VGA on ATE mode
HAS_ATE_DYNAMIC_VGA_SUPPORT=n
HAS_RSSI_FEEDBACK=n

# Support XLINK mode
HAS_XLINK=n

# Support WSC function
HAS_WSC=n
HAS_WSC_V2=n
HAS_WSC_LED=n
HAS_IWSC_SUPPORT=n
HAS_WSC_NFC=n

# Support WDS function
HAS_WDS=n

# Support AP-Client function
HAS_APCLI=n
# Support AP-Client TGn certification
HAS_APCLI_CERT=n
# Support AP-Client Connection Train
HAS_APCLI_CONNECTION_TRIAL=n

# Support Wpa_Supplicant
# i.e. wpa_supplicant -Dralink
HAS_WPA_SUPPLICANT=y


# Support Native WpaSupplicant for Network Maganger
# i.e. wpa_supplicant -Dwext
HAS_NATIVE_WPA_SUPPLICANT_SUPPORT=n

# if platform support fast connection, this define should be enable;
# This scheme is not need modify USB disconnect function in Linux core;
# This will be the default scheme.
HAS_NEW_FAST_CONNECT_SCHEME_SUPPORT=n
HAS_NEW_WOW_SUPPORT=n
HAS_WOW_STA_MODE=y
HAS_WOW_BUF_PKT_SUPPORT=y

#Support Net interface block while Tx-Sw queue full
HAS_BLOCK_NET_IF=n

#Support IGMP-Snooping function.
HAS_IGMP_SNOOP_SUPPORT=n

#Support DFS function
HAS_DFS_SUPPORT=n

#Support Carrier-Sense function
HAS_CS_SUPPORT=n

# Support ED CCA monitor
HAS_ED_MONITOR_SUPPORT=y


# Support user specific transmit rate of Multicast packet.
HAS_MCAST_RATE_SPECIFIC_SUPPORT=n

# Support for Multiple Cards
HAS_MC_SUPPORT=n

#Support for PCI-MSI
HAS_MSI_SUPPORT=n


#Support for IEEE802.11e DLS
HAS_QOS_DLS_SUPPORT=n

#Support for EXT_CHANNEL
HAS_EXT_BUILD_CHANNEL_LIST=n

#Support customized channel list
HAS_FORCE_CUSTOMIZED_COUNTRY_REGION=y

#Support for IDS
HAS_IDS_SUPPORT=n

#Support for Net-SNMP
HAS_SNMP_SUPPORT=n

#Support features of Single SKU.
HAS_SINGLE_SKU_SUPPORT=n

#Support features of Single SKU.
HAS_SINGLE_SKU_V2_SUPPORT=n

#Support features of Customization Single SKU.
HAS_CUSTOMIZE_SINGLE_SKU_V2=n

#Support for 802.11ac VHT
HAS_DOT11_VHT_SUPPORT=y

#Support for WAPI
HAS_WAPI_SUPPORT=n



#Support features of Debugfs
HAS_DEBUGFS_SUPPORT=n



#Support for 2860/2880 co-exist
HAS_RT2880_RT2860_COEXIST=n

HAS_KTHREAD_SUPPORT=n



#Support for dot11z TDLS
HAS_DOT11Z_TDLS_SUPPORT=n


#Support for WiFi Display
HAS_WFD_SUPPORT=n

#Support for Auto channel select enhance
HAS_AUTO_CH_SELECT_ENHANCE=n

#Support statistics count
HAS_STATS_COUNT=y

#Support TSSI Antenna Variation
HAS_TSSI_ANTENNA_VARIATION=n

#Support USB_BULK_BUF_ALIGMENT
HAS_USB_BULK_BUF_ALIGMENT=n

#Support USB_BULK Small Buffer
HAS_USB_BULK_BUF_PREALLOC=n

#Support for USB_SUPPORT_SELECTIVE_SUSPEND
HAS_USB_SUPPORT_SELECTIVE_SUSPEND=n

#Support USB load firmware by multibyte
HAS_USB_FIRMWARE_MULTIBYTE_WRITE=n

#Support ANDROID_SUPPORT (2.X with WEXT-based)
HAS_ANDROID_SUPPORT=n

#Support for dot11w Protected Management Frame
HAS_DOT11W_PMF_SUPPORT=y


#Support TXRX SW Antenna Diversity
HAS_TXRX_SW_ANTDIV_SUPPORT=n

#Support for Bridge Fast Path & Bridge Fast Path function open to other module
HAS_BGFP_SUPPORT=n
HAS_BGFP_OPEN_SUPPORT=n

# Support HOSTAPD function
HAS_HOSTAPD_SUPPORT=n

#Support GreenAP function
HAS_GREENAP_SUPPORT=n

#Support cfg80211 function with Linux Only.
#Please make sure insmod the cfg80211.ko before our driver,
#our driver references to its symbol.
HAS_CFG80211_SUPPORT=y

#smooth the scan signal for cfg80211 based driver
HAS_CFG80211_SCAN_SIGNAL_AVG_SUPPORT=y

#Cfg80211-based P2P Support
HAS_CFG80211_P2P_SUPPORT=y
#Cfg80211-based P2P Mode Selection (must one be chosen)
HAS_CFG80211_P2P_CONCURRENT_DEVICE=y
HAS_CFG80211_P2P_SINGLE_DEVICE=n
HAS_CFG80211_P2P_STATIC_CONCURRENT_DEVICE=n

HAS_CFG80211_P2P_MULTI_CHAN_SUPPORT=n
# Support NEW TDLS using supplicant
HAS_CFG80211_TDLS_SUPPORT=n

# Support SoftAP (hostapd cfg80211)
HAS_CFG80211_AP_SUPPORT=y

#For android wifi priv-lib (cfg80211_based wpa_supplicant cmd expansion)
HAS_CFG80211_ANDROID_PRIV_LIB_SUPPORT=n

# Driver sends channel switch notification event to hostapd to switch SoftAP op channel
HAS_CFG80211_CHANNEL_SWITCH_NOTIFICATION=y

#Support RFKILL hardware block/unblock LINUX-only function
HAS_RFKILL_HW_SUPPORT=n



HAS_MT76XX_BT_COEXISTENCE_SUPPORT=y

HAS_TEMPERATURE_TX_ALC=n

HAS_APCLI_WPA_SUPPLICANT=n

HAS_RTMP_FLASH_SUPPORT=n

#Support delay TCP Ack
HAS_DELAYED_TCP_ACK=n

HAS_LED_CONTROL_SUPPORT=n


#Support WIDI feature
#Must enable HAS_WSC at the same time.

HAS_TXBF_SUPPORT=y
HAS_VHT_TXBF_SUPPORT=y

HAS_STREAM_MODE_SUPPORT=n

HAS_NEW_RATE_ADAPT_SUPPORT=n

HAS_RATE_ADAPT_AGS_SUPPORT=n




#MT7601
HAS_RX_CSO_SUPPORT=y



HAS_ANDES_FIRMWARE_SUPPORT=n

HAS_HDR_TRANS_SUPPORT=n

HAS_MULTI_CHANNEL=n

HAS_MICROWAVE_OVEN_SUPPORT=n

HAS_MAC_REPEATER_SUPPORT=n

HAS_SWITCH_CHANNEL_OFFLOAD=n

HAS_FPGA_MODE=y

HAS_WIFI_TEST=n

HAS_SNIFFER_SUPPORT=n

HAS_CALIBRATION_COLLECTION_SUPPORT=n

HAS_FW_DEBUG_SUPPORT=y

HAS_FW_CORE_DUMP_SUPPORT=y

HAS_HOST_DYNAMIC_VGA_SUPPORT=y

HAS_CAL_FREE_IC_SUPPORT=y

# Support submitting multiple URB
HAS_MULTI_URB=n

# Assign specified profile path when insmod
HAS_PROFILE_DYN_SUPPORT=y

# Coex FDD Auto switch to BW20 feature
HAS_COEX_FDD_AUTO_SWITCH_TO_BW20=n

# Support for WebOS platform
HAS_WEBOS_SUPPORT=n

# Support only scan active-scan channels and skip passive-scan channels.
HAS_ACTIVE_CHANNEL_SCAN_SUPPORT=y

# Support delay to return from module_init function
#   The delay used is non-busy waiting, so actually the driver will get into sleep for a while.
#   This function is useful in Android system that system may start-up wpa_supplicant right after
#   the insmod call. This will make our probe function not completed before starting wpa_supplicant.
#   Causing wpa_supplicant failed to init wifi. Adding this delay sleep can reserve some time for
#   USB host driver to probe our device and make wifi interface ready before starting up wpa_supplicant.
HAS_MODULE_INIT_DELAY_SLEEP=n

#Workaround for IOT issues in certification
HAS_CERTIFICATION_SIGMA_SUPPORT=y

#Firmware load from binary file
#   If disabled (default), firmware will be loaded from header files which are generated at compile time.
#   If enabled, driver will try to load firmware from binary file located at filesystem such as "/lib/firmware/"
#   or "/etc/firmware".
HAS_FW_LOAD_FROM_BIN=y

# Support channel status reporting to customized wpa_supplicant for p2p auto channel selection
HAS_SUPPORT_ACS_ALL_CHANNEL_RANK=y
HAS_SUPPORT_ACS_BY_SCAN=y

#On Android platform with other kernel version(eg. < 3.2)
#For hostapd, the IEs from the (Re)Association Request frame need to be made available
#If get error, be sure patch the cfg80211_new_sta.patch into kernel
HAS_SUPPORT_ANDROID_HOSTAPD_ASSOC_REQ_IES=n


# Small DMA Pool Size Support (6xx KB Reduced Allocation Size)
HAS_SMALL_DMA_POOL_SUPPORT=y

# Treat compile Warning as Error
HAS_COMPILE_WARNING_AS_ERROR=n

# Support STA/AP Concurrent
HAS_STA_AP_CONCURRENT=n

# Add interrupt transfer type endpoint switch support
HAS_INTERRUPT_EP_SWITCH_SUPPORT=n

# Set USB 3.0 runs only 2.5Gbit/s (proprietary to platform)
HAS_USB3_HALF_SPEED_SUPPORT=n

# Support kmalloc or usb_alloc_coherent
HAS_KMALLOC_MEM_SUPPORT=y

# Support RX Packet Thread
HAS_RXPKT_THREAD=n

# Jungle Sloane customization
HAS_JUNGLE_SUPPORT=n

# CE customization
HAS_CUSTOMIZED_BW_SETTING_SUPPORT=n
HAS_CUSTOMIZED_COUNTRY_REGION_CE_1=n

# Thermal protection
HAS_THERMAL_PROTECT_SUPPORT=n

# Define max client could associate with in AP mode
HAS_MAX_ALLOW_CLIENT=n

# Enable it only when usb host will need to reset after system resume
# Note: Need to rework host platform, so set to n by default, and only be enabled on specific customer branch
HAS_RESUME_WITH_USB_RESET_SUPPORT=n

#################################################
# Platform specific configurations
# Set up your platform specific settings here
#################################################

# ============ MT53XX ===========
ifeq ($(PLATFORM),MT53XX)
HAS_CUSTOMIZED_ACL_SUPPORT=y
HAS_CUSTOMIZED_BW_SETTING_SUPPORT=y
HAS_CUSTOMIZED_COUNTRY_REGION_CE_1=y
HAS_DYNAMIC_PWR_TABLE_MAPPED_BY_COUNTRY_REGION=y
HAS_NEW_WOW_REBOOT_USB_POWERON_SUPPORT=y
HAS_DFS_SLAVE=y
HAS_PRE_ASSIGN_MAC_ADDR=y
HAS_CUSTOMIZED_FEATURE_SUPPORT=y
endif
# ----------- End of MT53XX -----------

# ============ MT85XX_BDP ===========
#HE BDP Project
ifeq ($(PLATFORM),MT85XX_BDP)
HAS_FW_LOAD_FROM_BIN=n
endif
# ----------- End of MT85XX_BDP -----------

# ============ MT85XX_AUDIO ===========
#HE Audio Project
ifeq ($(PLATFORM),MT85XX_AUDIO)
HAS_FW_LOAD_FROM_BIN=n
HAS_PRE_ASSIGN_MAC_ADDR=y
HAS_MULTI_URB=n
HAS_SMALL_DMA_POOL_SUPPORT=n
HAS_FAKE_STANDBY=y
HAS_CONFIG_ELIAN_SUPPORT=y
HAS_SNIFFER_SUPPORT=y
endif
# ----------- End of MT85XX_AUDIO -----------

# ============ LINUX PC ===========
ifeq ($(PLATFORM),PC)
ifeq ($(HAS_CFG80211_SUPPORT),y)
HAS_CUSTOMIZED_ACL_SUPPORT=y
endif
endif
# ----------- End of LINUX PC -----------

# ============ MSTAR ===========
ifeq ($(PLATFORM),MSTAR)
HAS_MS_TV_SUPPORT = y
endif
# ----------- End of MSTAR -----------

# ============ MSTAR_LINUX ===========
ifeq ($(PLATFORM),MSTAR_LINUX)
endif
# ----------- End of MSTAR_LINUX -----------

# ============ HUAWEI ===========
ifeq ($(PLATFORM),HUAWEI)
endif
# ----------- End of HUAWEI -----------

# ============ ALPS_MT8173 ===========
ifeq ($(PLATFORM),ALPS_MT8173)
endif
# ----------- ALPS_MT8173 -----------
#################################################

CC ?= $(CROSS_COMPILE)gcc
LD ?= $(CROSS_COMPILE)ld

WFLAGS := -g -DAGGREGATION_SUPPORT -DPIGGYBACK_SUPPORT -DWMM_SUPPORT  -DLINUX -Wall -Wstrict-prototypes -Wno-trigraphs
WFLAGS += -DSYSTEM_LOG_SUPPORT -DRT28xx_MODE=$(RT28xx_MODE) -DCHIPSET=$(MODULE) -DDBG_DIAGNOSE -DDBG_RX_MCS -DDBG_TX_MCS
WFLAGS += -Wframe-larger-than=1024
#APsoc Specific
WFLAGS += -DCONFIG_RA_NAT_NONE
#end of /* APsoc Specific */

WFLAGS += -I$(RT28xx_DIR)/include

ifeq ($(HAS_KMALLOC_MEM_SUPPORT),y)
WFLAGS += -DKMALLOC_MEM_SUPPORT
endif

ifeq ($(HAS_SMALL_DMA_POOL_SUPPORT),y)
WFLAGS += -DSMALL_DMA_POOL_SUPPORT
endif

ifeq ($(HAS_CONFIG_ELIAN_SUPPORT),y)
WFLAGS += -DCONFIG_ELIAN_SUPPORT
endif

ifeq ($(HAS_FAKE_STANDBY),y)
WFLAGS += -DFAKE_STANDBY_SUPPORT
endif

ifeq ($(PREALLOC),YES)
# need mtprealloc.ko
# use new prealloc: multiple smaller for each queue
HAS_USB_BULK_BUF_ALIGMENT=y
HAS_USB_BULK_BUF_PREALLOC=y
else
# start of PREALLOC != YES
HAS_USB_BULK_BUF_PREALLOC=n
# not change HAS_USB_BULK_BUF_ALIGMENT: both y or n are ok
endif # end of PREALLOC

ifeq ($(HAS_USB_BULK_BUF_ALIGMENT),y)
WFLAGS += -DUSB_BULK_BUF_ALIGMENT
ifeq ($(HAS_USB_BULK_BUF_PREALLOC),y)
WFLAGS += -DUSB_BULK_BUF_PREALLOC
endif
endif


ifeq ($(HAS_MULTI_INF),y)
WFLAGS += -DMULTI_INF_SUPPORT
endif

ifeq ($(HAS_KTHREAD_SUPPORT),y)
WFLAGS += -DKTHREAD_SUPPORT
endif

ifeq ($(HAS_RTMP_FLASH_SUPPORT),y)
WFLAGS += -DRTMP_FLASH_SUPPORT
endif

ifeq ($(HAS_STREAM_MODE_SUPPORT),y)
WFLAGS += -DSTREAM_MODE_SUPPORT
endif

ifeq ($(HAS_SINGLE_SKU_SUPPORT),y)
WFLAGS += -DSINGLE_SKU
endif

ifeq ($(HAS_SINGLE_SKU_V2_SUPPORT),y)
WFLAGS += -DSINGLE_SKU_V2
endif

#Support features of Customization Single SKU.
ifeq ($(HAS_CUSTOMIZE_SINGLE_SKU_V2),y)
WFLAGS += -DSINGLE_SKU_V2
WFLAGS += -DCUSTOMIZE_SINGLE_SKU_V2
endif

ifeq ($(HAS_DOT11_VHT_SUPPORT),y)
WFLAGS += -DDOT11_VHT_AC
endif

ifeq ($(HAS_ANDES_FIRMWARE_SUPPORT),y)
WFLAGS += -DANDES_FIRMWARE_SUPPORT
endif

ifeq ($(HAS_HDR_TRANS_SUPPORT),y)
WFLAGS += -DHDR_TRANS_SUPPORT
endif

ifeq ($(HAS_MAC_REPEATER_SUPPORT),y)
WFLAGS += -DMAC_REPEATER_SUPPORT
endif

ifeq ($(HAS_ATE),y)
WFLAGS += -DRALINK_ATE
WFLAGS += -DCONFIG_RT2880_ATE_CMD_NEW
WFLAGS += -I$(RT28xx_DIR)/ate/include
ifeq ($(HAS_QA_SUPPORT),y)
WFLAGS += -DRALINK_QA
endif
ifeq ($(HAS_ATE_DYNAMIC_VGA_SUPPORT),y)
WFLAGS += -DATE_DYNAMIC_VGA_SUPPORT
endif
endif #HAS_ATE

ifeq ($(HAS_CAL_FREE_IC_SUPPORT),y)
WFLAGS += -DCAL_FREE_IC_SUPPORT
endif

ifeq ($(HAS_TEMPERATURE_TX_ALC),y)
WFLAGS += -DRTMP_TEMPERATURE_TX_ALC
endif

ifeq ($(HAS_THERMAL_PROTECT_SUPPORT),y)
WFLAGS += -DTHERMAL_PROTECT_SUPPORT
endif

ifeq ($(HAS_ED_MONITOR_SUPPORT),y)
WFLAGS += -DED_MONITOR
endif

ifeq ($(HAS_MS_TV_SUPPORT),y)
WFLAGS += -DMS_TV_SUPPORT
endif

ifeq ($(HAS_CUSTOMIZED_ACL_SUPPORT),y)
WFLAGS += -DCUSTOMIZED_ACL_SUPPORT
endif

ifeq ($(HAS_MULTI_URB),y)
WFLAGS += -DMULTI_URB
endif

ifeq ($(HAS_SUPPORT_ACS_ALL_CHANNEL_RANK),y)
WFLAGS += -DSUPPORT_ACS_ALL_CHANNEL_RANK
endif

ifeq ($(HAS_SUPPORT_ACS_BY_SCAN),y)
WFLAGS += -DSUPPORT_ACS_BY_SCAN
endif

ifeq ($(HAS_CUSTOMIZED_BW_SETTING_SUPPORT),y)
WFLAGS += -DCUSTOMIZED_BW_SETTING
endif

ifeq ($(HAS_CUSTOMIZED_COUNTRY_REGION_CE_1),y)
WFLAGS += -DCUSTOMIZED_COUNTRY_REGION_CE_1
endif

ifeq ($(HAS_FORCE_CUSTOMIZED_COUNTRY_REGION),y)
WFLAGS += -DFORCE_CUSTOMIZED_COUNTRY_REGION
endif

ifeq ($(HAS_DYNAMIC_PWR_TABLE_MAPPED_BY_COUNTRY_REGION),y)
WFLAGS += -DDYNAMIC_PWR_TABLE_MAPPED_BY_COUNTRY_REGION
endif

ifeq ($(HAS_COEX_FDD_AUTO_SWITCH_TO_BW20),y)
WFLAGS += -DCOEX_FDD_AUTO_SWITCH_TO_BW20
endif

ifeq ($(HAS_MODULE_INIT_DELAY_SLEEP),y)
WFLAGS += -DMODULE_INIT_DELAY_SLEEP
endif

ifeq ($(HAS_PRE_ASSIGN_MAC_ADDR),y)
WFLAGS += -DPRE_ASSIGN_MAC_ADDR
endif

ifeq ($(HAS_CUSTOMIZED_FEATURE_SUPPORT),y)
WFLAGS += -DCUSTOMIZED_FEATURE_SUPPORT
endif

ifeq ($(HAS_CERTIFICATION_SIGMA_SUPPORT),y)
WFLAGS += -DCERTIFICATION_SIGMA_SUPPORT
endif

ifeq ($(HAS_WEBOS_SUPPORT),y)
WFLAGS += -DWEBOS_SUPPORT
endif

ifeq ($(HAS_COMPILE_WARNING_AS_ERROR),y)
WFLAGS += -Werror
endif

ifeq ($(HAS_INTERRUPT_EP_SWITCH_SUPPORT),y)
WFLAGS += -DINTERRUPT_EP_SWITCH_SUPPORT
endif

ifeq ($(HAS_MAX_ALLOW_CLIENT),y)
WFLAGS += -DMAX_ALLOW_CLIENT
endif

########################################################
# config for STA mode
########################################################

ifeq ($(RT28xx_MODE),STA)
WFLAGS += -DCONFIG_STA_SUPPORT -DSCAN_SUPPORT -DDBG

ifeq ($(HAS_MT76XX_BT_COEXISTENCE_SUPPORT),y)
WFLAGS += -DMT76XX_BTCOEX_SUPPORT
endif

ifeq ($(HAS_XLINK),y)
WFLAGS += -DXLINK_SUPPORT
endif

WFLAGS += -DADHOC_WPA2PSK_SUPPORT

ifeq ($(HAS_WPA_SUPPLICANT),y)
WFLAGS += -DWPA_SUPPLICANT_SUPPORT
ifeq ($(HAS_NATIVE_WPA_SUPPLICANT_SUPPORT),y)
WFLAGS += -DNATIVE_WPA_SUPPLICANT_SUPPORT
endif
endif #HAS_WPA_SUPPLICANT

ifeq ($(HAS_WSC),y)
WFLAGS += -DWSC_STA_SUPPORT
ifeq ($(HAS_WSC_V2),y)
WFLAGS += -DWSC_V2_SUPPORT
endif
ifeq ($(HAS_WSC_LED),y)
WFLAGS += -DWSC_LED_SUPPORT
endif
ifeq ($(HAS_IWSC_SUPPORT),y)
WFLAGS += -DIWSC_SUPPORT
endif
endif #HAS_WSC

ifeq ($(HAS_SNMP_SUPPORT),y)
WFLAGS += -DSNMP_SUPPORT
endif

ifeq ($(HAS_QOS_DLS_SUPPORT),y)
WFLAGS += -DQOS_DLS_SUPPORT
endif

ifeq ($(HAS_TXBF_SUPPORT),y)
WFLAGS += -DTXBF_SUPPORT
endif

ifeq ($(HAS_VHT_TXBF_SUPPORT),y)
WFLAGS += -DVHT_TXBF_SUPPORT
endif

ifeq ($(HAS_NEW_RATE_ADAPT_SUPPORT),y)
WFLAGS += -DNEW_RATE_ADAPT_SUPPORT
endif

ifeq ($(HAS_DOT11Z_TDLS_SUPPORT),y)
WFLAGS += -DDOT11Z_TDLS_SUPPORT -DUAPSD_SUPPORT
endif

ifeq ($(HAS_WFD_SUPPORT),y)
HAS_P2P_SUPPORT=y
HAS_P2P_ODD_MAC_ADJUST=y
HAS_P2P_SPECIFIC_WIRELESS_EVENT=y
WFLAGS += -DWFD_SUPPORT
endif

ifeq ($(HAS_CS_SUPPORT),y)
WFLAGS += -DCARRIER_DETECTION_SUPPORT
endif

ifeq ($(HAS_STATS_COUNT),y)
WFLAGS += -DSTATS_COUNT_SUPPORT
endif

ifeq ($(HAS_TSSI_ANTENNA_VARIATION),y)
WFLAGS += -DTSSI_ANTENNA_VARIATION
endif

ifeq ($(HAS_ANDROID_SUPPORT),y)
WFLAGS += -DANDROID_SUPPORT
endif

ifeq ($(HAS_USB_SUPPORT_SELECTIVE_SUSPEND),y)
WFLAGS += -DUSB_SUPPORT_SELECTIVE_SUSPEND
endif

ifeq ($(HAS_USB_FIRMWARE_MULTIBYTE_WRITE),y)
WFLAGS += -DUSB_FIRMWARE_MULTIBYTE_WRITE -DMULTIWRITE_BYTES=4
endif

ifeq ($(HAS_CFG80211_SUPPORT),y)
WFLAGS += -DRT_CFG80211_SUPPORT -DWPA_SUPPLICANT_SUPPORT
ifeq ($(HAS_RFKILL_HW_SUPPORT),y)
WFLAGS += -DRFKILL_HW_SUPPORT
endif

ifeq ($(HAS_CFG80211_SCAN_SIGNAL_AVG_SUPPORT),y)
WFLAGS += -DCFG80211_SCAN_SIGNAL_AVG
endif

ifeq ($(HAS_CFG80211_P2P_SUPPORT),y)
WFLAGS += -DRT_CFG80211_P2P_SUPPORT
WFLAGS += -DUAPSD_SUPPORT -DUAPSD_SP_ACCURATE -DMBSS_SUPPORT -DAP_SCAN_SUPPORT
WFLAGS += -DCONFIG_AP_SUPPORT -DAPCLI_SUPPORT

ifeq ($(HAS_CFG80211_P2P_CONCURRENT_DEVICE),y)
WFLAGS += -DRT_CFG80211_P2P_CONCURRENT_DEVICE
endif #HAS_CFG80211_P2P_CONCURRENT_DEVICE

ifeq ($(HAS_CFG80211_P2P_SINGLE_DEVICE),y)
WFLAGS += -DRT_CFG80211_P2P_SINGLE_DEVICE
else
ifeq ($(HAS_CFG80211_P2P_STATIC_CONCURRENT_DEVICE),y)
WFLAGS += -DRT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
else
WFLAGS += -DRT_CFG80211_P2P_CONCURRENT_DEVICE

ifeq ($(HAS_CFG80211_P2P_MULTI_CHAN_SUPPORT),y)
WFLAGS += -DRT_CFG80211_P2P_MULTI_CHAN_SUPPORT
endif #HAS_CFG80211_P2P_MULTI_CHAN_SUPPORT
endif #HAS_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
endif #HAS_CFG80211_P2P_SINGLE_DEVICE
endif #HAS_CFG80211_P2P_SUPPORT

ifeq ($(HAS_CFG80211_ANDROID_PRIV_LIB_SUPPORT),y)
WFLAGS += -DRT_RT_CFG80211_ANDROID_PRIV_LIB_SUPPORT
endif #HAS_CFG80211_ANDROID_PRIV_LIB_SUPPORT

ifeq ($(HAS_CFG80211_TDLS_SUPPORT),y)
WFLAGS += -DCFG_TDLS_SUPPORT -DUAPSD_SUPPORT
endif #HAS_CFG80211_TDLS_SUPPORT

ifeq ($(HAS_CFG80211_AP_SUPPORT),y)
WFLAGS += -DRT_CFG80211_AP_SUPPORT
WFLAGS += -DUAPSD_SUPPORT -DMBSS_SUPPORT
WFLAGS += -DCONFIG_AP_SUPPORT

ifeq ($(HAS_CFG80211_CHANNEL_SWITCH_NOTIFICATION),y)
WFLAGS += -DCFG80211_CHANNEL_SWITCH_NOTIFICATION
endif #HAS_CFG80211_CHANNEL_SWITCH_NOTIFICATION

endif #HAS_CFG80211_AP_SUPPORT

endif #HAS_CFG80211_SUPPORT

ifeq ($(HAS_TXRX_SW_ANTDIV_SUPPORT),y)
WFLAGS += -DTXRX_SW_ANTDIV_SUPPORT
endif

ifeq ($(HAS_WIDI_SUPPORT),y)
WFLAGS += -DWIDI_SUPPORT
ifeq ($(HAS_INTEL_L2SD_TOGGLE_SCAN_SUPPORT),y)
WFLAGS += -DINTEL_L2SD_TOGGLE_SCAN_SUPPORT
endif
ifeq ($(HAS_P2P_SUPPORT),y)
ifeq ($(HAS_INTEL_WFD_SUPPORT),y)
WFLAGS += -DINTEL_WFD_SUPPORT
endif
ifeq ($(HAS_WFA_WFD_SUPPORT),y)
WFLAGS += -DWFA_WFD_SUPPORT
endif
endif #HAS_P2P_SUPPORT
endif #HAS_WIDI_SUPPORT

ifeq ($(HAS_PROFILE_DYN_SUPPORT),y)
WFLAGS += -DPROFILE_PATH_DYNAMIC
endif

ifeq ($(HAS_WOW_IFDOWN_SUPPORT),y)
WFLAGS += -DWOW_IFDOWN_SUPPORT
endif

ifeq ($(HAS_NEW_WOW_SUPPORT),y)
WFLAGS += -DNEW_WOW_SUPPORT
endif

ifeq ($(HAS_WOW_STA_MODE),y)
WFLAGS += -DWOW_STA_MODE
endif

ifeq ($(HAS_NEW_WOW_REBOOT_USB_POWERON_SUPPORT),y)
WFLAGS += -DNEW_WOW_REBOOT_USB_POWERON_SUPPORT
endif

ifeq ($(HAS_RESUME_WITH_USB_RESET_SUPPORT),y)
WFLAGS += -DRESUME_WITH_USB_RESET_SUPPORT
endif

ifeq ($(HAS_NEW_FAST_CONNECT_SCHEME_SUPPORT),y)
ifeq ($(HAS_NEW_WOW_SUPPORT),y)
WFLAGS += -DNEW_FAST_CONNECT_SCHEME_SUPPORT
endif
endif #HAS_NEW_FAST_CONNECT_SCHEME_SUPPORT

ifeq ($(HAS_WOW_BUF_PKT_SUPPORT),y)
WFLAGS += -DWOW_BUF_PKT_SUPPORT
endif

ifeq ($(HAS_DFS_SLAVE),y)
WFLAGS += -DCONFIG_DFS_SLAVE

# Because HAS_P2PGO_DFS_SLAVE is dependent on HAS_DFS_SLAVE for:
#   i. Add CSA IE to P2PGO beacon where the CSA IE comes from
#      the beacon received by wlan0
#  ii. Report the target ch to wpa_supplicant to restart
#      P2PGO on the target ch where the target ch comes from
#      the beacon received by wlan0 too
# Thus, HAS_P2PGO_DFS_SLAVE can be enabled only when
# HAS_DFS_SLAVE is enabled
ifeq ($(HAS_P2PGO_DFS_SLAVE),y)
WFLAGS += -DCONFIG_P2PGO_DFS_SLAVE
endif
endif

ifeq ($(HAS_ACTIVE_CHANNEL_SCAN_SUPPORT),y)
WFLAGS += -DACTIVE_CHANNEL_SCAN_SUPPORT
endif

endif #($(RT28xx_MODE),STA)

##########################################################
#
# Common compiler flag
#
##########################################################

ifeq ($(HAS_USB3_HALF_SPEED_SUPPORT),y)
WFLAGS += -DUSB3_HALF_SPEED_SUPPORT
endif

ifeq ($(HAS_EXT_BUILD_CHANNEL_LIST),y)
WFLAGS += -DEXT_BUILD_CHANNEL_LIST
endif

ifeq ($(HAS_IDS_SUPPORT),y)
WFLAGS += -DIDS_SUPPORT
endif

ifeq ($(HAS_WAPI_SUPPORT),y)
WFLAGS += -DWAPI_SUPPORT -DSOFT_ENCRYPT -DEXPORT_SYMTAB
endif

ifeq ($(HAS_DEBUGFS_SUPPORT),y)
WFLAGS += -DDEBUGFS_SUPPORT
endif

ifeq ($(HAS_BGFP_SUPPORT),y)
WFLAGS += -DBG_FT_SUPPORT
endif

ifeq ($(HAS_BGFP_OPEN_SUPPORT),y)
WFLAGS += -DBG_FT_OPEN_SUPPORT
endif

ifeq ($(HAS_DOT11W_PMF_SUPPORT),y)
WFLAGS += -DDOT11W_PMF_SUPPORT -DSOFT_ENCRYPT
endif

ifeq ($(HAS_LED_CONTROL_SUPPORT),y)
WFLAGS += -DLED_CONTROL_SUPPORT
endif

ifeq ($(HAS_HOTSPOT_SUPPORT),y)
WFLAGS += -DCONFIG_DOT11U_INTERWORKING -DCONFIG_DOT11V_WNM -DCONFIG_HOTSPOT
endif

ifeq ($(HAS_MULTI_CHANNEL),y)
WFLAGS += -DCONFIG_MULTI_CHANNEL
endif

ifeq ($(HAS_MICROWAVE_OVEN_SUPPORT),y)
WFLAGS += -DMICROWAVE_OVEN_SUPPORT
endif

ifeq ($(HAS_FPGA_MODE),y)
WFLAGS += -DCONFIG_FPGA_MODE
endif

ifeq ($(HAS_WIFI_TEST),y)
WFLAGS += -DCONFIG_WIFI_TEST
endif

ifeq ($(HAS_SNIFFER_SUPPORT),y)
WFLAGS += -DCONFIG_SNIFFER_SUPPORT
endif

ifeq ($(HAS_CALIBRATION_COLLECTION_SUPPORT),y)
WFLAGS += -DCONFIG_CALIBRATION_COLLECTION
endif

ifeq ($(HAS_FW_DEBUG_SUPPORT),y)
WFLAGS += -DCONFIG_FW_DEBUG
endif

ifeq ($(HAS_FW_CORE_DUMP_SUPPORT),y)
WFLAGS += -DCONFIG_FW_CORE_DUMP
endif

ifeq ($(HAS_HOST_DYNAMIC_VGA_SUPPORT),y)
WFLAGS += -DHOST_DYNAMIC_VGA_SUPPORT
endif

ifeq ($(HAS_FW_LOAD_FROM_BIN),y)
WFLAGS += -DFW_LOAD_FROM_BIN
endif

ifeq ($(HAS_SUPPORT_ANDROID_HOSTAPD_ASSOC_REQ_IES),y)
WFLAGS += -DSUPPORT_ANDROID_HOSTAPD_ASSOC_REQ_IES
endif

ifeq ($(HAS_RXPKT_THREAD),y)
WFLAGS += -DRXPKT_THREAD
endif

ifeq ($(HAS_JUNGLE_SUPPORT),y)
WFLAGS += -DJUNGLE_SUPPORT
endif

#################################################
# ChipSet specific definitions.
#################################################

# MT7662E
ifneq ($(or $(findstring mt7662e,$(CHIPSET)),$(findstring mt7612e,$(CHIPSET))),)
WFLAGS += -DMT76x2 -DRT65xx -DRLT_MAC -DRLT_BBP -DMT_RF -DRTMP_MAC_PCI -DRTMP_PCI_SUPPORT -DA_BAND_SUPPORT -DRX_DMA_SCATTER -DNEW_MBSSID_MODE -DRTMP_EFUSE_SUPPORT -DCONFIG_ANDES_SUPPORT -DRTMP_RF_RW_SUPPORT -DDYNAMIC_VGA_SUPPORT -DRESOURCE_PRE_ALLOC
HAS_NEW_RATE_ADAPT_SUPPORT=y
ifeq ($(HAS_NEW_RATE_ADAPT_SUPPORT),y)
WFLAGS += -DNEW_RATE_ADAPT_SUPPORT
endif
WFLAGS += -DFIFO_EXT_SUPPORT
HAS_RLT_BBP=y
HAS_RLT_MAC=y

CHIPSET_DAT = 2860

ifneq ($(findstring mt7662e,$(CHIPSET)),)
WFLAGS += -DMT7662
endif

ifneq ($(findstring mt7662e,$(CHIPSET)),)
WFLAGS += -DMT7632
endif

ifneq ($(findstring mt7612e,$(CHIPSET)),)
WFLAGS += -DMT7612
endif
endif
## End of MT7662E ##

# MT7662U
ifneq ($(or $(findstring mt7662u,$(CHIPSET)),$(findstring mt7662u,$(CHIPSET))),)
WFLAGS += -DMT76x2 -DRT65xx -DRLT_MAC -DRLT_BBP -DMT_RF -DRTMP_MAC_USB -DRTMP_USB_SUPPORT -DRTMP_TIMER_TASK_SUPPORT -DA_BAND_SUPPORT -DRTMP_EFUSE_SUPPORT -DNEW_MBSSID_MODE -DCONFIG_ANDES_SUPPORT -DRTMP_RF_RW_SUPPORT -DDYNAMIC_VGA_SUPPORT
HAS_NEW_RATE_ADAPT_SUPPORT=y
ifeq ($(HAS_NEW_RATE_ADAPT_SUPPORT),y)
WFLAGS += -DNEW_RATE_ADAPT_SUPPORT
endif
WFLAGS += -DFIFO_EXT_SUPPORT
HAS_RLT_BBP=y
HAS_RLT_MAC=y

ifneq ($(findstring mt7662u,$(CHIPSET)),)
WFLAGS += -DMT7662
endif

ifneq ($(findstring mt7612u,$(CHIPSET)),)
WFLAGS += -DMT7612
endif

ifeq ($(HAS_CSO_SUPPORT), y)
WFLAGS += -DCONFIG_CSO_SUPPORT -DCONFIG_TSO_SUPPORT
endif

ifneq ($(findstring $(RT28xx_MODE),STA APSTA),)
WFLAGS += -DRTMP_FREQ_CALIBRATION_SUPPORT
endif

CHIPSET_DAT = 2870
endif
## End of 7662U ##

#################################################
# Platform Related definitions
#################################################

ifeq ($(HAS_BLOCK_NET_IF),y)
WFLAGS += -DBLOCK_NET_IF
endif

ifeq ($(HAS_DFS_SUPPORT),y)
WFLAGS += -DDFS_SUPPORT
endif

ifeq ($(HAS_MC_SUPPORT),y)
WFLAGS += -DMULTIPLE_CARD_SUPPORT
endif

ifeq ($(PLATFORM),PC)
EXTRA_CFLAGS := $(WFLAGS)
endif

ifeq (MT85XX,$(findstring MT85XX,$(PLATFORM)))
EXTRA_CFLAGS += $(WFLAGS) -I$(RT28xx_DIR)/include -DMT85XX
EXTRA_CFLAGS += -D _NO_TYPEDEF_BOOL_ \
                -D _NO_TYPEDEF_UCHAR_ \
                -D _NO_TYPEDEF_UINT8_ \
                -D _NO_TYPEDEF_UINT16_ \
                -D _NO_TYPEDEF_UINT32_ \
                -D _NO_TYPEDEF_UINT64_ \
                -D _NO_TYPEDEF_CHAR_ \
                -D _NO_TYPEDEF_INT32_ \
                -D _NO_TYPEDEF_INT64_
endif

ifeq ($(PLATFORM),MT53XX)
EXTRA_CFLAGS += $(WFLAGS) -DMT53XX
endif

ifeq ($(PLATFORM),MSTAR)
EXTRA_CFLAGS += -fno-stack-protector -fno-inline-functions-called-once
EXTRA_CFLAGS += $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),ALPS_MT8173)
EXTRA_CFLAGS += -fno-stack-protector -fno-inline-functions-called-once
EXTRA_CFLAGS += $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),MSTAR_LINUX)
EXTRA_CFLAGS += $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),HUAWEI)
EXTRA_CFLAGS += $(WFLAGS)
export EXTRA_CFLAGS
endif

ifeq ($(PLATFORM),WEBOS)
EXTRA_CFLAGS := $(WFLAGS)
endif

