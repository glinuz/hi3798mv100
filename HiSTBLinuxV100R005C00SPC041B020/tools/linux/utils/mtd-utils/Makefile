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

###############################################################################
CC          := gcc
BUILD_DIR   := $(PREFIX)/tmp
STRIP       := strip

MTD_UTILS   := mtd-utils-1.5.0
# this lib should be install to host
LZO         := lzo-2.09
ZLIB        ?= $(shell pwd)/../libs/libz.a

TARGETS     := mkfs.jffs2 mkfs.ubifs ubinize

###############################################################################
all: $(addprefix $(PREFIX)/,$(TARGETS))

$(PREFIX)/%: $(BUILD_DIR)/sbin/%
	@test -d $(@D) || mkdir -p $(@D)
	install -vs -t $(PREFIX) $<
	$(STRIP) $<

$(addprefix $(BUILD_DIR)/sbin/,$(TARGETS)): \
    $(BUILD_DIR)/$(MTD_UTILS)/build_finish

###############################################################################

$(BUILD_DIR)/$(MTD_UTILS)/build_finish: $(BUILD_DIR)/lib/liblzo2.a $(ZLIB)
	cd $(BUILD_DIR)/$(MTD_UTILS) && \
		make CFLAGS="-I$(BUILD_DIR)/include -I$(RULEATTR) -static -m32"  \
			LDFLAGS="-L$(BUILD_DIR)/lib -L$(dir $(RULEATTR_LIB)) -L$(dir $(ZLIB)) -static" \
			ZLIBCPPFLAGS="-I$(BUILD_DIR)/include -I$(RULEATTR) -static -m32" \
			WITHOUT_XATTR=1 \
			ZLIBLDFLAGS="-L$(BUILD_DIR)/lib -L$(dir $(RULEATTR_LIB)) -static"&& \
		make DESTDIR=$(BUILD_DIR) SBINDIR=sbin install && \
	cd -

$(BUILD_DIR)/lib/liblzo2.a: prepare
	cd $(BUILD_DIR)/$(LZO) && \
		./configure --prefix=$(BUILD_DIR) CC=gcc CFLAGS="-static -m32" && \
		make -j1 CC=gcc CFLAGS="-static -m32" && \
		make install CC=gcc && \
	cd -
###############################################################################

clean:
	$(AT)rm -rf $(BUILD_DIR)
	@test ! -e $(PREFIX)/mkfs.jffs2 || rm -rfv $(PREFIX)/mkfs.jffs2
	@test ! -e $(PREFIX)/mkfs.ubifs || rm -rfv $(PREFIX)/mkfs.ubifs
	@test ! -e $(PREFIX)/ubinize || rm -rfv $(PREFIX)/ubinize

distclean: clean

prepare:
	$(AT)mkdir -p $(BUILD_DIR)
	$(AT)test -d $(BUILD_DIR)/$(MTD_UTILS) || tar -xjf $(OPEN_SOURCE_DIR)/$(MTD_UTILS).tar.bz2 -C $(BUILD_DIR)
	$(AT)cp -rf $(MTD_UTILS).patch/. $(BUILD_DIR)/$(MTD_UTILS)
	$(AT)test -d $(BUILD_DIR)/$(LZO) || tar -xf $(OPEN_SOURCE_DIR)/$(LZO).tar.gz -C $(BUILD_DIR)

###############################################################################
.PHONY: clean prepare
###############################################################################
