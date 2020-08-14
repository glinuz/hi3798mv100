#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

PREFIX      ?= $(shell pwd)
RULEATTR    := $(CURDIR)/../ruleattr
RULEATTR_LIB ?= $(PREFIX)/../ruleattr/libruleattr.a

################################################################################
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
SQUASHFS       := squashfs4.3
XZ             := xz-5.2.2
LIBSELINUX     := libselinux
################################################################################

all: $(PREFIX)/mksquashfs $(PREFIX)/lzma

$(PREFIX)/mksquashfs: $(BUILD_DIR)/bin/mksquashfs
	@test -d $(@D) || mkdir -p $(@D)
	install -vs -t $(PREFIX) $<

$(PREFIX)/lzma: $(BUILD_DIR)/bin/lzma
	@test -d $(@D) || mkdir -p $(@D)
	install -vs -t $(PREFIX) $<

$(BUILD_DIR)/bin/mksquashfs: \
    $(BUILD_DIR)/lib/libselinux.a \
    $(BUILD_DIR)/lib/liblzma.a \
    $(BUILD_DIR)/$(SQUASHFS)/patch
	make -C $(BUILD_DIR)/$(SQUASHFS)/squashfs-tools \
		EXTRA_CFLAGS="-I$(BUILD_DIR)/include -I$(RULEATTR) -static -m32 " \
		LDFLAGS="-L$(BUILD_DIR)/lib -L$(dir $(RULEATTR_LIB)) -L$(dir $(ZLIB)) -static -m32" -j1
	make -C $(BUILD_DIR)/$(SQUASHFS)/squashfs-tools INSTALL_DIR=$(BUILD_DIR)/bin install -j1

$(BUILD_DIR)/bin/lzma $(BUILD_DIR)/lib/liblzma.a: prepare
	cd $(BUILD_DIR)/$(XZ) && \
		./configure --prefix="$(BUILD_DIR)" --disable-shared LDFLAGS="-static" CFLAGS="-static -m32" AM_LDFLAGS="-all-static" && \
		make AM_LDFLAGS="-all-static" LDFLAGS="-static -L$(BUILD_DIR)/lib" -j1 && \
		make INSTALL_DIR=$(BUILD_DIR)/bin install -j1 && \
	cd -

$(BUILD_DIR)/lib/libselinux.a: prepare
	make -C $(BUILD_DIR)/$(LIBSELINUX) CFLAGS=-m32 LDFLAGS="-static -L$(BUILD_DIR)/lib"
	make -C $(BUILD_DIR)/$(LIBSELINUX) PREFIX=$(BUILD_DIR) install

$(BUILD_DIR)/$(SQUASHFS)/patch: \
    prepare
	cp -rf $(SQUASHFS).patch/. $(BUILD_DIR)/$(SQUASHFS)/
	@touch $@
###############################################################################

clean:
	$(AT)rm -rf $(BUILD_DIR)
	@test ! -e $(PREFIX)/mksquashfs || rm -rfv $(PREFIX)/mksquashfs
	@test ! -e $(PREFIX)/lzma || rm -rfv $(PREFIX)/lzma

distclean: clean

$(BUILD_DIR)/%/tar:
	@test -d $(@D) || mkdir -p $(@D)
	tar -xjf $(notdir $(@:/tar=)).tar.bz2 -C $(BUILD_DIR)
	@touch $@

prepare:
	$(AT)mkdir -p $(BUILD_DIR)
	$(AT)test -d $(BUILD_DIR)/$(SQUASHFS) || tar -xjf $(OPEN_SOURCE_DIR)/$(SQUASHFS).tar.bz2 -C $(BUILD_DIR)
	$(AT)test -d $(BUILD_DIR)/$(XZ) || tar -xjf $(OPEN_SOURCE_DIR)/$(XZ).tar.bz2 -C $(BUILD_DIR)
	$(AT)test -d $(BUILD_DIR)/$(LIBSELINUX) || tar -xjf $(OPEN_SOURCE_DIR)/$(LIBSELINUX).tar.bz2 -C $(BUILD_DIR)

###############################################################################
.PHONY: clean distclean all
###############################################################################
