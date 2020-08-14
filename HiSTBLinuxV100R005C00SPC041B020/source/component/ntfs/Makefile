################################################################################
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

NTFS_DIR := $(COMPONENT_DIR)/ntfs

ifeq ($(CFG_HI_CPU_ARCH),arm64)
NTFS_MODULE_NAME := tntfs_$(subst _dbg_,_,$(CFG_HI_KERNEL_CFG))_64bit.ko
else
NTFS_MODULE_NAME := tntfs_$(subst _dbg_,_,$(CFG_HI_KERNEL_CFG)).ko
endif

.PHONY: all clean

all:
ifeq ($(CFG_HI_NTFS_TOOLS_SUPPORT),y)
	$(AT)test -d $(HI_ROOTFS_DIR)/bin || mkdir -p $(HI_ROOTFS_DIR)/bin
	$(AT)cp -f $(NTFS_DIR)/tools/mkntfs $(HI_ROOTFS_DIR)/bin
	$(AT)cp -f $(NTFS_DIR)/tools/ntfsck $(HI_ROOTFS_DIR)/bin
endif
ifeq ($(CFG_HI_NTFS_MODULE_SUPPORT),y)
	$(AT)test -d $(HI_MODULE_DIR) || mkdir -p $(HI_MODULE_DIR)
	$(AT)cp -f $(NTFS_DIR)/$(CFG_HI_KERNEL_VERSION)/$(NTFS_MODULE_NAME) $(HI_MODULE_DIR)/tntfs.ko
endif

clean:
ifeq ($(CFG_HI_NTFS_TOOLS_SUPPORT),y)
	$(AT)rm -rf $(HI_ROOTFS_DIR)/bin/mkntfs $(HI_ROOTFS_DIR)/bin/ntfsck
endif
ifeq ($(CFG_HI_NTFS_MODULE_SUPPORT),y)
	$(AT)rm -rf $(HI_MODULE_DIR)/tntfs.ko
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


