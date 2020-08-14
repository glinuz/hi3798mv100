#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

################################################################################
ifneq ($(HI_OUT_DIR),)
E2FSPROGS_DIR := $(HI_OUT_DIR)/obj/source/rootfs/e2fsprogs
else
E2FSPROGS_DIR := $(CURDIR)
endif

PREFIX ?= $(HI_ROOTFS_DIR)

BUILD_DIR   := $(E2FSPROGS_DIR)/tmp
HOST        := $(CFG_HI_ARM_TOOLCHAINS_NAME)
STRIP       := $(CFG_HI_ARM_TOOLCHAINS_NAME)-strip
E2FSPROGS   := e2fsprogs-1.42.13

TARGETS := sbin/mke2fs sbin/blkid

################################################################################
.PHONY: all clean
################################################################################
all: $(addprefix $(PREFIX)/,$(TARGETS))
	$(AT)cd $(PREFIX)/sbin; ln -sf mke2fs mkfs.ext2; ln -sf mke2fs mkfs.ext3; ln -sf mke2fs mkfs.ext4

clean:
	$(AT)rm -rf $(E2FSPROGS_DIR)/$(E2FSPROGS) $(BUILD_DIR)

$(PREFIX)/%: $(BUILD_DIR)/%
	$(AT)test -d $(@D) || mkdir -p $(@D)
	$(AT)install -v -t $(@D) $<

$(addprefix $(BUILD_DIR)/,$(TARGETS)): $(E2FSPROGS_DIR)/$(E2FSPROGS)/tar
	$(AT)cd $(E2FSPROGS_DIR)/$(E2FSPROGS) && make && make install-progs-recursive

$(E2FSPROGS_DIR)/$(E2FSPROGS)/tar:
	$(AT)test -d $(E2FSPROGS_DIR) || mkdir -p $(E2FSPROGS_DIR)
	$(AT)test -d $(E2FSPROGS_DIR)/$(E2FSPROGS) || (tar -xjf $(OPEN_SOURCE_DIR)/$(E2FSPROGS).tar.bz2 -C $(E2FSPROGS_DIR); \
        cd $(E2FSPROGS_DIR)/$(E2FSPROGS); \
        ./configure --host=$(HOST) --prefix=$(BUILD_DIR) --disable-tls)
	$(AT)touch $@

################################################################################
