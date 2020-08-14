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
STA_SRC := DPA_MT7601U_ANDROID_JB_3.0.0.6_20130830_SOFTAP
STA_MODULE := mt7601Usta.ko mtprealloc7601Usta.ko
STA_MODULE_PATH := $(BUILD_DIR)/$(STA_SRC)/os/linux/mt7601Usta.ko \
                   $(BUILD_DIR)/$(STA_SRC)/PREALLOC/os/linux/mtprealloc7601Usta.ko
STA_CFG := RT2870STA_7601.dat
STA_CFG_PATH := $(BUILD_DIR)/$(STA_SRC)/RT2870STA_7601.dat
endif

ifeq ($(CFG_HI_WIFI_MODE_AP),y)
AP_SRC := DPO_GPL_MT7601UAP_JB_SDK42_20130329
AP_MODULE := mt7601Uap.ko
AP_MODULE_PATH := $(BUILD_DIR)/$(AP_SRC)/os/linux/mt7601Uap.ko
AP_CFG := RT2870AP_7601.dat
AP_CFG_PATH := $(BUILD_DIR)/$(AP_SRC)/RT2870AP_7601.dat
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
		|| tar -zxf $(patsubst %_build,%,$@).tar.gz -C $(BUILD_DIR)
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) LINUX_SRC=$(LINUX_DIR) \
		-C $(BUILD_DIR)/$(patsubst %_build,%,$@)

$(BUILD_DIR):
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)

%_uninstall:
	@rm -f $(INSTALL_DIR)/$(patsubst %_uninstall,%,$@)
