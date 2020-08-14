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
BUILD_DIR  := $(LINUX_DIR)
ifneq ($(ANDROID_BUILD),y)
INSTALL_DIR := $(HI_MODULE_DIR)
FW_INSTALL_DIR := $(HI_ROOTFS_DIR)/lib/firmware
else
INSTALL_DIR := $(ANDROID_PRODUCT_OUT)/system/lib/modules
FW_INSTALL_DIR := $(ANDROID_PRODUCT_OUT)/system/lib/firmware
endif
HOST := $(patsubst %-,%,$(CROSS_COMPILE))

MODULE_SRC  := 
MODULE := cfg80211.ko mac80211.ko ath.ko ath9k_hw.ko \
	ath9k_common.ko ath9k_htc.ko ath9k.ko
MODULE_PATH := $(BUILD_DIR)/drivers/net/wireless/ath/ath9k/ath9k_htc.ko \
	$(BUILD_DIR)/drivers/net/wireless/ath/ath9k/ath9k_hw.ko \
	$(BUILD_DIR)/drivers/net/wireless/ath/ath9k/ath9k_common.ko \
	$(BUILD_DIR)/drivers/net/wireless/ath/ath9k/ath9k.ko \
	$(BUILD_DIR)/drivers/net/wireless/ath/ath.ko \
	$(BUILD_DIR)/net/mac80211/mac80211.ko \
	$(BUILD_DIR)/net/wireless/cfg80211.ko

FIRMWARE := htc_9271.fw
FIRMWARE_PATH := $(shell pwd)/firmware/htc_9271.fw

BUILD_MODULES = $(patsubst %,%_build,${MODULE_SRC})
UNINSTALL_MODULES = $(patsubst %,%_uninstall,${MODULE})
UNINSTALL_FIRMWARE = $(patsubst %,%_fw_uninstall,${FIRMWARE})

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
	@cp -v $(FIRMWARE_PATH) $(FW_INSTALL_DIR)/

uninstall: $(UNINSTALL_MODULES) $(UNINSTALL_FIRMWARE)

#==================================================================
#                          modules
#==================================================================
%_build: $(BUILD_DIR)
	@echo -e "\033[31m""Build $(patsubst %_build,%,$@) ...""\033[00m"
	@test -d $(BUILD_DIR)/$(patsubst %_build,%,$@) \
		|| tar -zxf $(patsubst %_build,%,$@).tar.gz -C $(BUILD_DIR)
	cd $(BUILD_DIR)/$(patsubst %_build,%,$@) && \
	./scripts/driver-select ath9k_htc && \
	make ARCH=$(ARCH) CC=$(HOST)-gcc LD=$(HOST)-ld \
			KLIB=$(LINUX_DIR) KLIB_BUILD=$(LINUX_DIR) && \
	cd -

$(BUILD_DIR):
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)

%_uninstall:
	@rm -f $(INSTALL_DIR)/$(patsubst %_uninstall,%,$@)

%_fw_uninstall:
	@rm -f $(FW_INSTALL_DIR)/$(patsubst %_fw_uninstall,%,$@)
