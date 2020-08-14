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
FW_INSTALL_DIR := $(HI_ROOTFS_DIR)/lib/firmware/brcm
else
INSTALL_DIR := $(ANDROID_PRODUCT_OUT)/system/lib/modules
FW_INSTALL_DIR := $(ANDROID_PRODUCT_OUT)/system/lib/firmware/brcm
endif

ifeq ($(CFG_HI_KERNEL_VERSION),linux-3.10.y)
LINUXVER := 3.10.0
endif
ifeq ($(CFG_HI_KERNEL_VERSION),linux-3.4.y)
LINUXVER := 3.4.35
endif

BUILDDCFG := dhd-cdc-usb-reqfw-android-stb-jellybean-cfg80211

MODULE_SRC := dhd_rel_1_141_5_20
MODULE := bcmdhd.ko
MODULE_PATH := $(BUILD_DIR)/$(MODULE_SRC)/src/dhd/linux/$(BUILDDCFG)-$(LINUXVER)/bcmdhd.ko

FIRMWARE := bcm43236-firmware.bin bcm43236-firmware.bin_apsta.trx bcm43236.nvm
FIRMWARE_PATH := $(shell pwd)/firmware/bcm43236/bcm43236-firmware.bin \
	$(shell pwd)/firmware/bcm43236/bcm43236-firmware.bin_apsta.trx \
	$(shell pwd)/firmware/bcm43236/bcm43236.nvm

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
		|| tar -jxf $(patsubst %_build,%,$@).tar.bz2 -C $(BUILD_DIR)
	make -C $(BUILD_DIR)/$(MODULE_SRC)/src/dhd/linux $(BUILDDCFG) LINUXVER=$(LINUXVER) LINUXDIR=$(LINUX_DIR) \
		ARCH=$(ARCH) CC=$(CROSS_COMPILE)gcc STRIP=$(CROSS_COMPILE)strip V=1

$(BUILD_DIR):
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)

%_uninstall:
	@rm -f $(INSTALL_DIR)/$(patsubst %_uninstall,%,$@)
