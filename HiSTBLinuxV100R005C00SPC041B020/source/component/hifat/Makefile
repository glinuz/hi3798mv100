################################################################################
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

################################################################################
FAT_LIB := lib
FAT_TOOLS := tools
FAT_INCLUDE := include
USR_BIN := $(HI_ROOTFS_DIR)/usr/bin
BIN := $(HI_ROOTFS_DIR)/bin

################################################################################
.PHONY: all clean

all:
	$(AT)test -d $(USR_BIN) || mkdir -p $(USR_BIN)
	$(AT)cp -rf $(FAT_TOOLS)/himount $(USR_BIN)/himount
	$(AT)chmod +x $(USR_BIN)/himount
	$(AT)test -d $(HI_INCLUDE_DIR) || mkdir -p $(HI_INCLUDE_DIR)
	$(AT)cp -rf $(FAT_INCLUDE)/himount_api.h $(HI_INCLUDE_DIR)
	$(AT)test -d $(HI_STATIC_LIB_DIR) || mkdir -p $(HI_STATIC_LIB_DIR)
	$(AT)test -d $(HI_SHARED_LIB_DIR) || mkdir -p $(HI_SHARED_LIB_DIR)
	$(AT)cp -rf $(FAT_LIB)/libhimount_gcc-4.4.1-vfp.a $(HI_STATIC_LIB_DIR)/libhimount.a
	$(AT)cp -rf $(FAT_LIB)/libhimount_gcc-4.4.1-vfp.so $(HI_SHARED_LIB_DIR)/libhimount.so
ifdef CFG_HI_ADVCA_FUNCTION
ifneq ($(CFG_HI_ADVCA_FUNCTION), DEBUG)
	$(AT)cp -rf $(FAT_LIB)/libhimount_gcc-4.4.1-vfp_ca.a $(HI_STATIC_LIB_DIR)/libhimount.a
	$(AT)test -d $(BIN) || mkdir -p $(BIN)
	$(AT)cp -rf $(FAT_TOOLS)/himount_ca $(BIN)/himount

	$(AT)chmod +x $(HI_ROOTFS_DIR)/bin/himount
endif
endif

clean:
	$(AT)rm -rf $(USR_BIN)/himount $(HI_SHARED_LIB_DIR)/libhimount.so $(HI_STATIC_LIB_DIR)/libhimount.a $(HI_INCLUDE_DIR)/himount_api.h

