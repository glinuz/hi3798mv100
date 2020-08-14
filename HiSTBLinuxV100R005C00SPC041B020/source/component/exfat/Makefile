################################################################################
#  Copyright (C) 2014 Hisilicon STB Development Dept
#  All rights reserved.
#  Create by zyxing
#
################################################################################
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

HOST            := $(CFG_HI_ARM_TOOLCHAINS_NAME)
CROSS_COMPILE   := $(HOST)-
STRIP           := $(CROSS_COMPILE)strip
EXFAT           := exfat-1.1.1
LIBFUSE         := fuse-2.9.4

EXFAT_DIR := $(HI_OUT_DIR)/obj/source/component/exfat

PREFIX := $(EXFAT_DIR)/rootfs
BUILD_DIR := $(EXFAT_DIR)/tmp

exFAT_utils_tgt := usr/sbin/dumpexfat \
	usr/sbin/exfatfsck  \
	usr/sbin/exfatlabel \
	usr/sbin/mkexfatfs  \
    usr/sbin/mount.exfat-fuse

libFuse_tgt := lib/libfuse.so.2 lib/libfuse.so lib/libfuse.so.2.9.4

USR_SBIN_DIR := $(HI_ROOTFS_DIR)/usr/sbin

################################################################################
.PHONY: all clean
################################################################################
all: $(addprefix $(PREFIX)/,$(exFAT_utils_tgt) $(libFuse_tgt))
	$(AT)mkdir -p $(USR_SBIN_DIR)
	$(AT)cp -rf $(PREFIX)/usr/sbin/exfatfsck $(USR_SBIN_DIR)/fsck.exfat
	$(AT)cp -rf $(PREFIX)/usr/sbin/mkexfatfs $(USR_SBIN_DIR)/mkfs.exfat
	$(AT)cp -rf $(PREFIX)/usr/sbin/dumpexfat $(USR_SBIN_DIR)/dump.exfat
	$(AT)cp -rf $(PREFIX)/usr/sbin/mount.exfat-fuse $(USR_SBIN_DIR)/mount.exfat

clean:
	$(AT)rm -rf $(EXFAT_DIR)/$(EXFAT) $(EXFAT_DIR)/$(LIBFUSE) $(BUILD_DIR) $(PREFIX)
	$(AT)rm -rf $(USR_SBIN_DIR)/fsck.exfat $(USR_SBIN_DIR)/mkfs.exfat \
                $(USR_SBIN_DIR)/dump.exfat $(USR_SBIN_DIR)/mount.exfat

$(PREFIX)/%: $(BUILD_DIR)/%
	$(AT)test -d $(@D) || mkdir -p $(@D)
	$(AT)install -v -t $(@D) $<
	$(AT)[ s"`file "$@" | grep "not stripped"`" = s"" ] || $(STRIP) $@

################################################################################
$(addprefix $(BUILD_DIR)/,$(exFAT_utils_tgt)): $(EXFAT_DIR)/$(EXFAT)/build_finish
	@

$(EXFAT_DIR)/$(EXFAT)/build_finish: $(EXFAT_DIR)/$(EXFAT)/tar $(BUILD_DIR)/lib/libfuse.a
	$(AT)make -C $(EXFAT_DIR)/$(EXFAT) CROSS_COMPILE=$(CROSS_COMPILE) FUSE_PATH="$(BUILD_DIR)" EXFAT_PATH="$(BUILD_DIR)/$(EXFAT)"
	$(AT)make -C $(EXFAT_DIR)/$(EXFAT) install CROSS_COMPILE=$(CROSS_COMPILE) PREFIX=$(BUILD_DIR)
	$(AT)touch $@

################################################################################
$(addprefix $(BUILD_DIR)/,$(libFuse_tgt)) \
$(BUILD_DIR)/lib/libfuse.a: $(EXFAT_DIR)/$(LIBFUSE)/build_finish
	@

$(EXFAT_DIR)/$(LIBFUSE)/build_finish: $(EXFAT_DIR)/$(LIBFUSE)/tar
	$(AT)cd $(EXFAT_DIR)/$(LIBFUSE) && make && make install
	$(AT)touch $@

$(EXFAT_DIR)/$(EXFAT)/tar:
	$(AT)test -d $(EXFAT_DIR) || mkdir -p $(EXFAT_DIR)
	$(AT)test -d $(EXFAT_DIR)/$(EXFAT) || (tar -xf $(OPEN_SOURCE_DIR)/$(EXFAT).tar.gz -C $(EXFAT_DIR) \
        && cp -f Makefile.exfat-1.1.1 $(EXFAT_DIR)/$(EXFAT)/Makefile)
	$(AT)touch $@

$(EXFAT_DIR)/$(LIBFUSE)/tar:
	$(AT)test -d $(EXFAT_DIR) || mkdir -p $(EXFAT_DIR)
	$(AT)test -d $(EXFAT_DIR)/$(LIBFUSE) || (tar -xf $(OPEN_SOURCE_DIR)/$(LIBFUSE).tar.gz -C $(EXFAT_DIR); \
        cd $(EXFAT_DIR)/$(LIBFUSE); \
        ./configure --host=$(HOST) --prefix=$(BUILD_DIR) CFLAGS="-DGLIBC_VERSION -O2 -D_XOPEN_SOURCE=700" \
            DESTDIR=$(BUILD_DIR) MOUNT_FUSE_PATH=$(BUILD_DIR)/sbin INIT_D_PATH=$(BUILD_DIR)/etc/init.d \
            UDEV_RULES_PATH=$(BUILD_DIR)/etc/udev/rules.d)
	$(AT)touch $@
