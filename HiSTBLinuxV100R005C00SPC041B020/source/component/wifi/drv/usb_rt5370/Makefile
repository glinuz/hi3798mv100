#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/../../../../..
include $(SDK_DIR)/base.mak
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   variable
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BUILD_DIR  := $(shell pwd)/tmp
ifneq ($(ANDROID_BUILD),y)
INSTALL_DIR := $(HI_MODULE_DIR)
STA_CFG_INSTALL_DIR := $(HI_ROOTFS_DIR)/etc/Wireless/RT2870STA
AP_CFG_INSTALL_DIR := $(HI_ROOTFS_DIR)/etc/Wireless/RT2870AP
else
INSTALL_DIR := $(ANDROID_PRODUCT_OUT)/system/lib/modules
STA_CFG_INSTALL_DIR := $(ANDROID_PRODUCT_OUT)/system/etc/Wireless/RT2870STA
AP_CFG_INSTALL_DIR := $(ANDROID_PRODUCT_OUT)/system/etc/Wireless/RT2870AP
endif

ifeq ($(CFG_HI_WIFI_MODE_STA),y)
STA_SRC := DPA_RT5572_LinuxSTA_2.6.1.4_20121211
STA_MODULE := rtutil5572sta.ko rt5572sta.ko rtnet5572sta.ko
STA_MODULE_PATH := $(BUILD_DIR)/$(STA_SRC)/UTIL/os/linux/rtutil5572sta.ko \
    $(BUILD_DIR)/$(STA_SRC)/MODULE/os/linux/rt5572sta.ko \
    $(BUILD_DIR)/$(STA_SRC)/NETIF/os/linux/rtnet5572sta.ko
STA_CFG := RT2870STA.dat
STA_CFG_PATH := $(BUILD_DIR)/$(STA_SRC)/MODULE/RT2870STA.dat
endif

ifeq ($(CFG_HI_WIFI_MODE_AP),y)
AP_SRC := 2011_0517_RT5370_RT5372_RT5390U_Linux_AP_V2.6.0.0_DPA
AP_MODULE := rtutil5370ap.ko rt5370ap.ko rtnet5370ap.ko
AP_MODULE_PATH := $(BUILD_DIR)/$(AP_SRC)/UTIL/os/linux/rtutil5370ap.ko \
    $(BUILD_DIR)/$(AP_SRC)/MODULE/os/linux/rt5370ap.ko \
    $(BUILD_DIR)/$(AP_SRC)/NETIF/os/linux/rtnet5370ap.ko
AP_CFG := RT2870AP.dat
AP_CFG_PATH := $(BUILD_DIR)/$(AP_SRC)/MODULE/RT2870AP.dat
endif

MODULE_SRC = $(STA_SRC) $(AP_SRC)
MODULE = $(STA_MODULE) $(AP_MODULE)

BUILD_MODULES = $(patsubst %,%_build,${MODULE_SRC})
UNINSTALL_MODULES = $(patsubst %,%_uninstall,${MODULE})

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   rules
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#==================================================================
#                          all
#==================================================================
.PHONY: all clean install uninstall
all: $(BUILD_MODULES)

clean:
	@rm -rf $(BUILD_DIR)

install: all
	@test -d $(INSTALL_DIR) || mkdir -p $(INSTALL_DIR)
ifeq ($(CFG_HI_WIFI_MODE_STA),y)
	@cp -v $(STA_MODULE_PATH) $(INSTALL_DIR)/
	@test -d $(STA_CFG_INSTALL_DIR) || mkdir -p $(STA_CFG_INSTALL_DIR)
	@cp -v $(STA_CFG_PATH) $(STA_CFG_INSTALL_DIR)/
endif
ifeq ($(CFG_HI_WIFI_MODE_AP),y)
	@cp -v $(AP_MODULE_PATH) $(INSTALL_DIR)/
	@test -d $(AP_CFG_INSTALL_DIR) || mkdir -p $(AP_CFG_INSTALL_DIR)
	@cp -v $(AP_CFG_PATH) $(AP_CFG_INSTALL_DIR)/
endif

uninstall: $(UNINSTALL_MODULES)
ifeq ($(CFG_HI_WIFI_MODE_STA),y)
	@rm -f $(STA_CFG_INSTALL_DIR)/$(STA_CFG)
endif
ifeq ($(CFG_HI_WIFI_MODE_AP),y)
	@rm -f $(AP_CFG_INSTALL_DIR)/$(AP_CFG)
endif

#==================================================================
#                          modules
#==================================================================
%_build: $(BUILD_DIR)
	@echo -e "\033[31m""Build $(patsubst %_build,%,$@) ...""\033[00m"
	@test -d $(BUILD_DIR)/$(patsubst %_build,%,$@) \
		|| tar -jxf $(patsubst %_build,%,$@).tar.bz2 -C $(BUILD_DIR)
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) LINUX_SRC=$(LINUX_DIR) \
		-C $(BUILD_DIR)/$(patsubst %_build,%,$@)

$(BUILD_DIR):
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)

%_uninstall:
	@rm -f $(INSTALL_DIR)/$(patsubst %_uninstall,%,$@)
