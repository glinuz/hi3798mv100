#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

ifneq ($(HI_OUT_DIR),)
MTDUTILS_DIR := $(HI_OUT_DIR)/obj/source/rootfs/mtd-utils
else
MTDUTILS_DIR := $(CURDIR)
endif

PREFIX ?= $(HI_ROOTFS_DIR)

################################################################################
HOST := $(CFG_HI_ARM_TOOLCHAINS_NAME)
STRIP := $(CFG_HI_ARM_TOOLCHAINS_NAME)-strip

UBI-UTILS := ubi-utils-1.5.0

TARGETS := sbin/ubiutils

################################################################################
.PHONY: all clean
################################################################################
all: $(addprefix $(PREFIX)/,$(TARGETS))

clean:
	$(AT)rm -rf $(MTDUTILS_DIR)/$(UBI-UTILS)

$(PREFIX)/sbin/ubiutils: $(MTDUTILS_DIR)/$(UBI-UTILS)/tar
	$(AT)make -C $(MTDUTILS_DIR)/$(UBI-UTILS) CROSS_COMPILE=$(CFG_HI_ARM_TOOLCHAINS_NAME)-
	$(AT)make -C $(MTDUTILS_DIR)/$(UBI-UTILS) CROSS_COMPILE=$(CFG_HI_ARM_TOOLCHAINS_NAME)- PREFIX=$(PREFIX) install
	$(AT)$(STRIP) $@

$(MTDUTILS_DIR)/$(UBI-UTILS)/tar:
	$(AT)test -d $(MTDUTILS_DIR) || mkdir -p $(MTDUTILS_DIR)
	$(AT)test -d $(MTDUTILS_DIR)/$(UBI-UTILS) || tar -xf $(OPEN_SOURCE_DIR)/$(UBI-UTILS).tar.bz2 -C $(MTDUTILS_DIR)
	$(AT)touch $@
