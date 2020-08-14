#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

################################################################################
ifneq ($(HI_OUT_DIR),)
DOSFSTOOLS_DIR := $(HI_OUT_DIR)/obj/source/rootfs/dosfstools
else
DOSFSTOOLS_DIR := $(CURDIR)
endif

PREFIX ?= $(HI_ROOTFS_DIR)

BUILD_DIR   := $(DOSFSTOOLS_DIR)/tmp
HOST        := $(CFG_HI_ARM_TOOLCHAINS_NAME)
STRIP       := $(CFG_HI_ARM_TOOLCHAINS_NAME)-strip
DOSFS       := dosfstools-3.0.25

################################################################################
.PHONY: all clean
################################################################################
all: $(PREFIX)/sbin/fsck.fat
	$(AT)cd $(PREFIX)/sbin; ln -sf fsck.fat dosfsck

clean:
	$(AT)rm -rf $(DOSFSTOOLS_DIR)/$(DOSFS) $(BUILD_DIR)

$(PREFIX)/sbin/fsck.fat: $(DOSFSTOOLS_DIR)/$(DOSFS)/fsck.fat
	$(AT)test -d $(@D) || mkdir -p $(@D)
	$(AT)install -v -t $(@D) $<

$(DOSFSTOOLS_DIR)/$(DOSFS)/fsck.fat: $(DOSFSTOOLS_DIR)/$(DOSFS)/tar
	$(AT)cd $(DOSFSTOOLS_DIR)/$(DOSFS) && make CC=$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc

$(DOSFSTOOLS_DIR)/$(DOSFS)/tar:
	$(AT)test -d $(DOSFSTOOLS_DIR) || mkdir -p $(DOSFSTOOLS_DIR)
	$(AT)test -d $(DOSFSTOOLS_DIR)/$(DOSFS) || tar -xjf $(OPEN_SOURCE_DIR)/$(DOSFS).tar.bz2 -C $(DOSFSTOOLS_DIR)
	$(AT)touch $@
