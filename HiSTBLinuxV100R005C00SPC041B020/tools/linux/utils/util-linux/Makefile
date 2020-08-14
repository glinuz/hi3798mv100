#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/../../../..
include $(SDK_DIR)/base.mak
endif

PREFIX         ?= $(shell pwd)

###############################################################################
#
#  options below should be redefined here, don't use definitions in Makefile
#  in SDK top dir.
#
CC=gcc
OBJDUMP=objdump
HOST=
CPP=
LD=
NM=

###############################################################################
BUILD_DIR      := $(PREFIX)/tmp
STRIP          := strip
CRAMFS         := util-linux-2.22-rc2
###############################################################################

all: $(PREFIX)/mkfs.cramfs

$(PREFIX)/mkfs.cramfs: $(BUILD_DIR)/sbin/mkfs.cramfs
	@test -d $(@D) || mkdir -p $(@D)
	install -vs -t $(PREFIX) $<

$(BUILD_DIR)/sbin/mkfs.cramfs: prepare
	cd $(BUILD_DIR)/$(CRAMFS) && \
		./configure --prefix="$(BUILD_DIR)" CFLAGS="-m32" \
		 LDFLAGS="-L$(dir $(ZLIB))" \
		 --enable-static \
		 --disable-libuuid --disable-libblkid --disable-libmount \
		 --disable-mount --disable-losetup --disable-fsck \
		 --disable-partx --disable-uuidd --disable-mountpoint \
		 --disable-fallocate --disable-unshare --disable-eject \
		 --disable-agetty --disable-switch_root --disable-pivot_root \
		 --disable-kill --disable-utmpdump --disable-rename \
		 --disable-login --disable-sulogin --disable-su \
		 --disable-schedutils --disable-wall && \
		make && \
		./libtool  --tag=CC   --mode=link gcc -all-static -std=gnu99 -fsigned-char -fno-common -Wall -Werror=sequence-point -Wextra -Wmissing-declarations -Wmissing-parameter-type -Wmissing-prototypes -Wno-missing-field-initializers -Wredundant-decls -Wsign-compare -Wtype-limits -Wuninitialized -Wunused-but-set-parameter -Wunused-but-set-variable -Wunused-parameter -Wunused-result -Wunused-variable -m32  -static -L/home/lyjun/build/v1r5/Code/tools/linux/utils/zlib -o mkfs.cramfs disk-utils/mkfs.cramfs.o disk-utils/cramfs_common.o  -lz libcommon.la && \
		make install && \
	cd -

###############################################################################

clean:
	$(AT)rm -rf $(BUILD_DIR)
	@test ! -e $(PREFIX)/mkfs.cramfs || rm -rfv $(PREFIX)/mkfs.cramfs

distclean: clean

prepare:
	$(AT)mkdir -p $(BUILD_DIR)
	$(AT)test -d $(BUILD_DIR)/$(CRAMFS) || tar -xjf $(OPEN_SOURCE_DIR)/$(CRAMFS).tar.bz2 -C $(BUILD_DIR)

###############################################################################
.PHONY: clean distclean all prepare
###############################################################################
