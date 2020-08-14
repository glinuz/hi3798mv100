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
FW_INSTALL_DIR := $(HI_ROOTFS_DIR)/lib/firmware/ath6k/AR6004
else
INSTALL_DIR := $(ANDROID_PRODUCT_OUT)/system/lib/modules
FW_INSTALL_DIR := $(ANDROID_PRODUCT_OUT)/system/vendor/firmware/ath6k/AR6004
endif
HOST := $(patsubst %-,%,$(CROSS_COMPILE))

MODULE_SRC := WLAN-AIO-5.0.0.110
MODULE := cfg80211_ath6k.ko ath6kl_usb.ko
MODULE_PATH := $(BUILD_DIR)/$(MODULE_SRC)/drivers/cfg80211/cfg80211_ath6k.ko \
	$(BUILD_DIR)/$(MODULE_SRC)/drivers/ath6kl/ath6kl_usb.ko

FIRMWARE := fw.ram.bin bdata.bin otp.bin utf.bin
FIRMWARE_PATH := $(shell pwd)/firmware/hw1.3 \
	$(shell pwd)/firmware/hw3.0

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
	@cp -v $(MODULE_PATH) $(INSTALL_DIR)/
	@test -d $(FW_INSTALL_DIR) || mkdir -p $(FW_INSTALL_DIR)
	@cp -vr $(FIRMWARE_PATH) $(FW_INSTALL_DIR)/

uninstall: $(UNINSTALL_MODULES)
	@rm -rf $(FW_INSTALL_DIR)

#==================================================================
#                          modules
#==================================================================
%_build: $(BUILD_DIR)
	@echo -e "\033[31m""Build $(patsubst %_build,%,$@) ...""\033[00m"
	@test -d $(BUILD_DIR)/$(patsubst %_build,%,$@) \
		|| tar -zxf $(patsubst %_build,%,$@).tar.gz -C $(BUILD_DIR)
	cd $(BUILD_DIR)/$(patsubst %_build,%,$@)/build && \
	make ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) \
			KERNELPATH=$(LINUX_DIR) BOARD_TYPE=hisilicon && \
	cd -

$(BUILD_DIR):
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)

%_uninstall:
	@rm -f $(INSTALL_DIR)/$(patsubst %_uninstall,%,$@)