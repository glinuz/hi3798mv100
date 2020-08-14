#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/../../../../..
include $(SDK_DIR)/base.mak
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   variable
#   Linux_BT_USB
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
EXTRA_CFLAGS  +=  $(CFG_HI_KMOD_CFLAGS)

INSTALL_DIR := $(HI_MODULE_DIR)
FIRMWARE_DIR := $(HI_ROOTFS_DIR)/lib/firmware
BUILD_DIR  := $(shell pwd)

CFILES := \
	btmtk_usb_main.c \
	btmtk_usb_dbgfs.c \
	btmtk_usb_fifo.c

MOD_NAME = btmtk_usb

obj-m := $(MOD_NAME).o

$(MOD_NAME)-objs := $(CFILES:.c=.o)

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

#==================================================================
#                          all
#==================================================================
.PHONY: all clean install uninstall 

all:
	$(AT)make -C $(LINUX_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(CURDIR) modules

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf
	
install: all
	$(AT)test -d $(INSTALL_DIR) || $(AT)mkdir -p $(INSTALL_DIR)
	$(AT)cp -v $(BUILD_DIR)/$(MOD_NAME).ko $(INSTALL_DIR)/
	$(AT)cp -v $(BUILD_DIR)/mcu/bin/mt7662_patch_e3_hdr.bin $(FIRMWARE_DIR)/
	$(AT)cp -v $(BUILD_DIR)/mcu/bin/mt7662t_patch_e1_hdr.bin $(FIRMWARE_DIR)/
	
uninstall: 
	$(AT)rm -f $(INSTALL_DIR)/$(MOD_NAME).ko
	$(AT)rm -f $(FIRMWARE_DIR)/mt7662_patch_e3_hdr.bin
	$(AT)rm -f $(FIRMWARE_DIR)/mt7662t_patch_e1_hdr.bin
