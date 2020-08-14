#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/../../../../..
include $(SDK_DIR)/base.mak
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   variable
#   Linux_BT_USB_rtl8761au
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
EXTRA_CFLAGS  +=  $(CFG_HI_KMOD_CFLAGS)

INSTALL_DIR := $(HI_MODULE_DIR)
FIRMWARE_DIR := $(HI_ROOTFS_DIR)/lib/firmware
BUILD_DIR  := $(shell pwd)

obj-m := rtk_8761btusb.o
rtk_8761btusb-y = rtk_coex.o rtk_bt.o

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"
#==================================================================
#                          all
#==================================================================
.PHONY: all clean install uninstall
all: 
	$(AT)make -C $(LINUX_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(BUILD_DIR) modules

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

install: all
	$(AT)test -d $(INSTALL_DIR) || $(AT)mkdir -p $(INSTALL_DIR)
	$(AT)cp -v $(BUILD_DIR)/rtk_8761btusb.ko $(INSTALL_DIR)/
	$(AT)cp -v $(BUILD_DIR)/rtl8761au_fw $(FIRMWARE_DIR)/
	
uninstall: 
	$(AT)rm -f $(INSTALL_DIR)/rtk_8761btusb.ko
	$(AT)rm -f $(FIRMWARE_DIR)/rtl8761au_fw
